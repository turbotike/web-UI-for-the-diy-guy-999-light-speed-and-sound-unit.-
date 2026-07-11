// Shared build + WebSerial flash core. Used by both the standalone flasher
// (flash.html) and the full configurator SPA (index.html).
//
// Build = server-side arduino-cli compile (streamed log).
// Flash = browser-side esptool-js over WebSerial. No pyserial, no drivers.

import { ESPLoader, Transport } from "/web/vendor/esptool-js/bundle.js";

export const hasWebSerial = "serial" in navigator;

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
const noop = () => {};

// Run a server build. Streams the compiler log to onLog(text).
// Returns true on a clean compile (exit 0).
export async function streamBuild({ vehicle = "", onLog = noop } = {}) {
  const res = await fetch("/run", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ cmd: "build", vehicle }),
  });
  if (!res.ok) {
    let msg = "Build request failed (HTTP " + res.status + ")";
    try { const j = await res.json(); if (j.error) msg = j.error; } catch (_) {}
    throw new Error(msg);
  }
  const reader = res.body.getReader();
  const dec = new TextDecoder();
  let all = "";
  while (true) {
    const { value, done } = await reader.read();
    if (done) break;
    const chunk = dec.decode(value, { stream: true });
    all += chunk;
    onLog(chunk);
  }
  return all.includes("--- DONE (exit 0) ---");
}

async function fetchFirmwareImages() {
  const res = await fetch("/api/firmware");
  const j = await res.json();
  if (!j.ok) throw new Error(j.error || "Could not load firmware images");
  // esptool-js wants each image's data as a binary string (one char per byte).
  return j.firmware.parts.map((p) => ({ data: atob(p.data), address: p.offset }));
}

// Ask the user to pick a serial port. MUST be called directly from a click
// handler (before any long await like the build) — Chrome only shows the port
// picker while the click's "user gesture" is still active.
export async function requestSerialPort() {
  if (!hasWebSerial) {
    throw new Error("This browser can't flash. Use Chrome or Edge on desktop.");
  }
  return navigator.serial.requestPort();
}

// Flash the already-compiled firmware over WebSerial.
// Pass `port` (from requestSerialPort) so the picker isn't shown after the build.
// Callbacks: onStatus(text, kind), onLog(text), onProgress(pct 0..100).
export async function flashFirmware({
  port = null, baud = 921600, eraseAll = false,
  onStatus = noop, onLog = noop, onProgress = noop,
} = {}) {
  if (!hasWebSerial) {
    throw new Error("This browser can't flash. Use Chrome or Edge on desktop.");
  }

  const espTerminal = {
    clean: noop,
    writeLine: (d) => onLog(d + "\n"),
    write: (d) => onLog(d),
  };

  onStatus("Loading firmware images…", "work");
  const fileArray = await fetchFirmwareImages();
  const totalBytes = fileArray.reduce((n, f) => n + f.data.length, 0);
  onLog(`Firmware: ${fileArray.length} images, ${(totalBytes / 1024).toFixed(0)} KB total.\n`);

  if (!port) {
    // Fallback (e.g. standalone page) — may fail after a long build due to
    // the browser's user-gesture rule; the SPA passes `port` from the click.
    onStatus("Select your board's serial port in the prompt…", "work");
    port = await navigator.serial.requestPort();
  }

  // Try the chosen speed, then progressively slower ones. Many USB-serial chips
  // (notably CP2102) throw "Invalid head of packet" at 921600 but flash fine at
  // a lower baud — so we retry automatically instead of failing.
  const chosen = parseInt(baud, 10) || 460800;
  // Progressively more conservative [baud, compress] attempts. A flaky USB
  // cable/port can corrupt data mid-write ("Failed to write... Invalid head of
  // packet") — slower and uncompressed is the most reliable, so we fall back to
  // it automatically instead of failing.
  const plan = [[chosen, true]];
  for (const b of [230400, 115200]) if (b < chosen) plan.push([b, true]);
  plan.push([115200, false]); // last resort: slowest + uncompressed
  const attempts = plan.filter((p, i) => i === 0 || p[0] !== plan[i - 1][0] || p[1] !== plan[i - 1][1]);
  const isRetryable = (e) =>
    /invalid head|packet|Timed out|Failed to connect|corruption|not in flashing|Failed to write|to flash|status \d/i
      .test(String((e && e.message) || e));

  let lastErr;
  for (let i = 0; i < attempts.length; i++) {
    const [b, compress] = attempts[i];
    let transport;
    try {
      if (i > 0) { onLog(`\n--- retrying (${b} baud${compress ? "" : ", uncompressed"}) ---\n`); await sleep(600); }
      transport = new Transport(port, true);
      const loader = new ESPLoader({ transport, baudrate: b, romBaudrate: 115200, terminal: espTerminal });

      onStatus(`Connecting to ESP32 (${b} baud)…`, "work");
      const chip = await loader.main();
      onLog("Detected: " + chip + "\n");

      onStatus("Flashing… do not unplug the board.", "work");
      onProgress(0);
      await loader.writeFlash({
        fileArray,
        flashSize: "keep", flashMode: "keep", flashFreq: "keep",
        eraseAll: !!eraseAll, compress,
        reportProgress: (idx, written) => {
          let done = 0;
          for (let j = 0; j < idx; j++) done += fileArray[j].data.length;
          done += written;
          onProgress((done / totalBytes) * 100);
        },
      });
      onProgress(100);

      onStatus("Resetting board…", "work");
      try { await loader.hardReset(); }
      catch (_) { try { await transport.setRTS(true); await sleep(120); await transport.setRTS(false); } catch (_e) {} }
      try { await transport.disconnect(); } catch (_) {}

      onStatus("✓ Done! Firmware flashed. Reconnect the battery.", "ok");
      onLog("\n=== FLASH COMPLETE ===\n");
      return; // success
    } catch (e) {
      lastErr = e;
      try { if (transport) await transport.disconnect(); } catch (_) {}
      onLog(`Attempt at ${b} baud failed: ${(e && e.message) || e}\n`);
      if (!(i < attempts.length - 1 && isRetryable(e))) throw e;
    }
  }
  throw lastErr;
}

// Map a raw flash error to a friendly hint.
export function flashErrorHint(err) {
  const m = String((err && err.message) || err);
  if (/No port selected|cancelled|aborted/i.test(m)) return { text: "Cancelled — no port selected.", kind: "" };
  if (/Failed to write|to flash|corruption|status \d/i.test(m))
    return { text: "Flash write failed — usually a weak USB cable/port. Try a different (data) cable, plug straight into the PC (no hub), keep the battery disconnected, then retry.", kind: "err" };
  if (/Failed to connect|packet header|Timed out|invalid head|not in flashing/i.test(m))
    return { text: "Couldn't talk to the board. Disconnect battery, unplug/replug USB, hold BOOT, then retry.", kind: "err" };
  return { text: "Flash failed: " + m, kind: "err" };
}
