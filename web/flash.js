// Standalone flasher page logic — thin wrapper over the shared flasher core.
import { streamBuild, flashFirmware, requestSerialPort, flashErrorHint, hasWebSerial } from "/web/flasher.js";

const $ = (id) => document.getElementById(id);
const logEl = $("log"), statusEl = $("status"), barEl = $("bar");
const flashBtn = $("flashBtn"), buildBtn = $("buildBtn"), baudSel = $("baud"), eraseChk = $("erase");

function log(t) { logEl.textContent += t; logEl.scrollTop = logEl.scrollHeight; }
function setStatus(t, k = "") { statusEl.textContent = t; statusEl.className = "status " + k; }
function setProgress(p) { barEl.style.width = Math.max(0, Math.min(100, p)) + "%"; }
function busy(on) { flashBtn.disabled = on; buildBtn.disabled = on; }

if (!hasWebSerial) {
  setStatus("This browser can't flash. Use Chrome or Edge on desktop.", "err");
  flashBtn.disabled = true;
}

async function build() {
  busy(true); setStatus("Compiling firmware…", "work"); setProgress(0);
  log("\n=== BUILD ===\n");
  try {
    const ok = await streamBuild({ vehicle: window.SELECTED_VEHICLE || "", onLog: log });
    setStatus(ok ? "Build OK — ready to flash." : "Build failed — see log.", ok ? "ok" : "err");
    return ok;
  } catch (err) {
    log("ERROR: " + err.message + "\n");
    setStatus("Build failed: " + err.message, "err");
    return false;
  } finally { busy(false); }
}

async function buildAndFlash() {
  if (!hasWebSerial) { setStatus("Use Chrome or Edge to flash over USB.", "err"); return; }
  // Grab the port on the click (before the long build) — user-gesture rule.
  let port;
  try { setStatus("Select your board's port…", "work"); port = await requestSerialPort(); }
  catch (e) { setStatus("Cancelled — no port selected.", ""); return; }
  if (!(await build())) return;
  busy(true);
  try {
    await flashFirmware({
      port, baud: baudSel.value, eraseAll: eraseChk.checked,
      onStatus: setStatus, onLog: log, onProgress: setProgress,
    });
  } catch (err) {
    log("ERROR: " + ((err && err.message) || err) + "\n");
    const h = flashErrorHint(err); setStatus(h.text, h.kind);
  } finally { busy(false); }
}

buildBtn.addEventListener("click", build);
flashBtn.addEventListener("click", buildAndFlash);
