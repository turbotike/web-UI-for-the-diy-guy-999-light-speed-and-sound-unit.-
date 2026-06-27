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

// Flash the already-compiled firmware over WebSerial.
// Callbacks: onStatus(text, kind), onLog(text), onProgress(pct 0..100).
export async function flashFirmware({
  baud = 921600, eraseAll = false,
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

  let transport;
  try {
    onStatus("Loading firmware images…", "work");
    const fileArray = await fetchFirmwareImages();
    const totalBytes = fileArray.reduce((n, f) => n + f.data.length, 0);
    onLog(`Firmware: ${fileArray.length} images, ${(totalBytes / 1024).toFixed(0)} KB total.\n`);

    onStatus("Select your board's serial port in the prompt…", "work");
    const port = await navigator.serial.requestPort();
    transport = new Transport(port, true);

    const loader = new ESPLoader({
      transport,
      baudrate: parseInt(baud, 10) || 921600,
      romBaudrate: 115200,
      terminal: espTerminal,
    });

    onStatus("Connecting to ESP32…", "work");
    const chip = await loader.main();
    onLog("Detected: " + chip + "\n");

    onStatus("Flashing… do not unplug the board.", "work");
    onProgress(0);
    await loader.writeFlash({
      fileArray,
      flashSize: "keep",
      flashMode: "keep",
      flashFreq: "keep",
      eraseAll: !!eraseAll,
      compress: true,
      reportProgress: (idx, written) => {
        let done = 0;
        for (let i = 0; i < idx; i++) done += fileArray[i].data.length;
        done += written;
        onProgress((done / totalBytes) * 100);
      },
    });
    onProgress(100);

    onStatus("Resetting board…", "work");
    try { await loader.hardReset(); }
    catch (_) {
      try { await transport.setRTS(true); await sleep(120); await transport.setRTS(false); } catch (_e) {}
    }
    onStatus("✓ Done! Firmware flashed. Reconnect the battery.", "ok");
    onLog("\n=== FLASH COMPLETE ===\n");
  } finally {
    try { if (transport) await transport.disconnect(); } catch (_) {}
  }
}

// Map a raw flash error to a friendly hint.
export function flashErrorHint(err) {
  const m = String((err && err.message) || err);
  if (/No port selected|cancelled|aborted/i.test(m)) return { text: "Cancelled — no port selected.", kind: "" };
  if (/Failed to connect|packet header|Timed out|invalid head|not in flashing/i.test(m))
    return { text: "Couldn't talk to the board. Disconnect battery, hold BOOT, retry — or pick a lower speed.", kind: "err" };
  return { text: "Flash failed: " + m, kind: "err" };
}
