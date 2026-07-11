<p align="center">
  <img src="web/logo.jpg" alt="TheDIYGuy999 — Light, Sound &amp; Speed Controller" width="680">
</p>

<h1 align="center">Light, Sound &amp; Speed Controller — Configurator &amp; Flasher</h1>

<p align="center">
A point-and-click app for setting up and flashing the
<a href="https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32">TheDIYGuy999 RC engine sound &amp; light controller</a>.
Pick your truck, tweak the sounds and lights with sliders, and flash it to your ESP32 over USB —
no code editing, no Arduino IDE, no driver hunting.
</p>

> New here? Jump to **[Quick Start](#-quick-start)**.

## ⬇️ Download the app

Grab the ready-to-run app from the **[Releases page](../../releases/latest)**:

| Your computer | Download |
|---|---|
| **Windows** | `RC-Sound-Configurator-Windows.zip` |
| **Mac** (Intel or Apple Silicon) | `RC-Sound-Configurator-macOS.zip` |

Unzip it, then run the app inside (details in [Quick Start](#-quick-start) below).

---

## ✨ What you can do

- 🎚️ **Configure everything with sliders & switches** — engine, ESC, transmission, lights, servos, sounds (80+ vehicle profiles included)
- 🔊 **Sound Forge** — preview any of 580+ engine/horn/brake sounds, swap them per slot, or **upload your own WAV** (converted automatically)
- 🔌 **Flash over USB in one click** — uses the built-in **native uploader** (the same reliable tool the command line uses), so it works on any board
- 💾 **Presets, import/export, copy & reset** — never lose a setup you like

---

## 🟢 Before you start (one-time)

| Platform | What you need |
|---|---|
| **Windows** | **Nothing** — the app is a single `.exe` with everything inside (no Python, no Arduino IDE). |
| **Mac / Linux** | **[Python 3](https://www.python.org/downloads/)** (on Mac, `brew install python3` works too). |

Any modern web browser works (Chrome, Edge, Firefox, Safari). The first time you flash, the app quietly downloads its own compiler — **you never install the Arduino IDE.**

---

## 🚀 Quick Start

### Windows
1. Download **`RC-Sound-Configurator-Windows.zip`** from the **[Releases page](../../releases/latest)**, then **right-click it → Extract All…** ⚠️ **Don't run it from inside the zip** — extract it to a real folder (e.g. your Desktop) first, or the app can't find its files.
2. Open the **extracted folder** and double-click **`RC Sound Configurator.exe`**.
   - If Windows shows **"Windows protected your PC"** (it isn't code-signed): **More info → Run anyway**.
3. Your browser opens to **`http://localhost:8080`**.

> Prefer your own Python? Double-click **`start_webui.bat`** — same app.

### Mac
1. Download **`RC-Sound-Configurator-macOS.zip`** from the **[Releases page](../../releases/latest)** and unzip it.
2. Double-click **`Start RC Sound Configurator.command`**.
   - macOS says "unidentified developer" (not signed): **right-click → Open → Open**. Once only.
3. Your browser opens to **`http://localhost:8080`**.

### Linux
```bash
python3 configure.py
```
Then open **`http://localhost:8080`**.

> To stop the app, close the little window it opens (or press `Ctrl+C` in it).

---

## 🧭 Using the app (quick tour)

Tabs run along the top:

| Tab | What it's for |
|-----|---------------|
| **Vehicle** dropdown (top bar) | Pick which truck/machine you're building — everything updates to match. |
| **Vehicle Tuning** | The chosen vehicle's own settings (engine volumes, RPM, knock, turbo…). Also **Copy / Reset / Export / Import** and **Presets**. |
| **General, Remote, ESC, Transmission, Shaker, Lights, Servos, Sound, Dashboard, Trailer** | All the board settings as friendly sliders and switches. Hover any item for a plain-English explanation. |
| **🔊 Sound Forge** | Master volume + a **＋ Change** button on each sound slot to browse/preview sounds or upload your own WAV. |
| **⚡ Flash** | Compile and upload to your board (see below). |

**Saving:** edits show "● unsaved changes" up top — click **Save** to write them. (Flashing saves first automatically.)

---

## ⚡ Flashing your ESP32

1. Open the **⚡ Flash** tab.
2. **Disconnect the battery** from the controller. *(Important — a connected battery pulls GPIO12 high and the upload fails.)*
3. Plug the ESP32 into USB with a **data** cable (not charge-only), straight into the computer (no hub).
4. Click **🔍 Detect board** → pick your board's port from the dropdown (the one marked **✅**) → **🔌 Flash**.
   - First flash takes a few minutes while it downloads the toolchain — later ones are quick.
5. Watch the log write to **100%** and reset. When it says **✓ Flashed**, reconnect the battery. 🎉

---

## 🆘 Troubleshooting

| Problem | Fix |
|---------|-----|
| **Board not detected** / not in the port list | Try a different **USB data cable** and a port **directly on the computer** (no hub). Install your board's USB driver (Google your board name + **"CP2102 driver"** or **"CH340 driver"**). |
| **Upload fails partway** | **Battery disconnected?** Swap the USB cable (a weak cable is the #1 cause), avoid hubs, then retry. |
| **"Python is not installed"** (Mac/Linux) | Install [Python 3](https://www.python.org/downloads/); on Windows, tick **"Add Python to PATH"** if you use the `.bat`. |
| **Page won't open** | Make sure the little app window is still running. Re-launch it. |
| **Ran from inside the zip** (old built-in page shows) | Close it, **Extract All** first, then run the app from the extracted folder. |
| **A setting looks broken** | On **Vehicle Tuning**, click **Reset** to restore that profile, then redo your changes. |
| **Mac: "permission denied"** | Terminal in the folder → `chmod +x "Start RC Sound Configurator.command"`. |

---

## 🔌 Hardware (pin reference)

ESP32-WROOM-32 (30-pin) on the TheDIYGuy999 sound & light controller board.

| Pin | Function |
|-----|----------|
| GPIO25 / 26 | Audio out (to PAM8403 amp) |
| GPIO33 | ESC signal |
| GPIO13 / 12 / 14 / 27 | Servo CH1–CH4 |
| GPIO0 | Neopixel data |
| GPIO34 | Receiver input (PWM/PPM/SBUS/IBUS) |

Schematics & PCB files are in the **[`hardware/`](hardware/)** folder.

---

## 📁 What's in here

```
RC Sound Configurator.exe    ← Windows: double-click this (no Python needed)
start_webui.bat / .command   ← launch with your own Python (Win / Mac)
configure.py                 ← the local app server (runs everything)
web/                         ← the browser UI (+ logo)
libraries/                   ← the Arduino libraries the firmware needs (bundled)
src/                         ← the ESP32 firmware
  ├── src.ino                   main firmware
  ├── 0_… 10_….h                the setting "tabs"
  └── vehicles/                 80+ vehicle profiles + sound library
tools/                       ← WAV ↔ header converters
hardware/                    ← schematic & PCB files
```

---

## 🙌 Credits

| | |
|---|---|
| **Configurator & Flasher** | [turbotike](https://github.com/turbotike) |
| **Original firmware** | [TheDIYGuy999](https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32) |

Built on top of TheDIYGuy999's incredible firmware — go give that project a ⭐.
