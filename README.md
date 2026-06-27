# 🚜 RC Engine Sound ESP32 — Web Configurator & Flasher

A **browser app** for setting up and flashing the [TheDIYGuy999 RC engine sound & light controller](https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32). Pick your truck, tweak the sounds and lights with sliders, and flash it to your ESP32 **straight from your browser** — no code editing, no Arduino IDE, no driver hunting.

> New here? Jump to **[Quick Start](#-quick-start)**. It's three steps.

## ⬇️ Download the app

Grab the ready-to-run app from the **[Releases page](../../releases/latest)** — no Python, no install:

| Your computer | Download |
|---|---|
| **Windows** | `RC-Sound-Configurator-Windows.zip` |
| **Mac** (Intel or Apple Silicon) | `RC-Sound-Configurator-macOS.zip` |

Unzip it, then double-click the app inside (details in [Quick Start](#-quick-start) below).

---

## ✨ What you can do

- 🎚️ **Configure everything with sliders & switches** — engine, ESC, transmission, lights, servos, sounds (80+ vehicle profiles included)
- 🔊 **Sound Forge** — preview any of 580+ engine/horn/brake sounds in your browser, swap them per slot, or **upload your own WAV** (it's converted automatically)
- ⚡ **Flash over USB from the browser** — uses WebSerial, so there's nothing extra to install and no COM-port driver drama
- 💾 **Presets, import/export, copy & reset** — never lose a setup you like

---

## 🟢 Before you start (one-time)

**Windows:** you only need **Google Chrome or Microsoft Edge** ([Chrome](https://www.google.com/chrome/) · [Edge](https://www.microsoft.com/edge)) — used for USB flashing (Firefox/Safari can't do it). **No Python, no Arduino IDE** — the app is a single `.exe` that brings everything with it.

**Mac / Linux:** you need **Chrome/Edge** *and* **[Python 3](https://www.python.org/downloads/)** (on Mac, `brew install python3` works too). The `.exe` is Windows-only, so Mac/Linux run the included launcher script instead.

The first time you build, the app downloads its own compiler automatically — **you never install the Arduino IDE.**

---

## 🚀 Quick Start

### Windows (the easy way — no Python)
1. Download **`RC-Sound-Configurator-Windows.zip`** from the **[Releases page](../../releases/latest)** and unzip it somewhere (e.g. your Desktop).
2. Double-click **`RC Sound Configurator.exe`**.
   - If Windows shows a blue **"Windows protected your PC"** box, that's just because the app isn't code-signed: click **More info → Run anyway**.
3. Your browser opens automatically to **`http://localhost:8080`**. **If it opens in Firefox/Safari, copy that address into Chrome or Edge.**

> Prefer to run it with your own Python instead? Double-click **`start_webui.bat`** — same app.

### Mac (the easy way — no Python)
1. Download **`RC-Sound-Configurator-macOS.zip`** from the **[Releases page](../../releases/latest)** and unzip it.
2. Double-click **`Start RC Sound Configurator.command`**.
   - macOS will say "unidentified developer" (the app isn't signed): **right-click it → Open → Open**. You only do this once.
3. Your browser opens to **`http://localhost:8080`** (use Chrome or Edge).

> Prefer your own Python? Double-click **`start_webui.command`** instead — same app.

### Linux
```bash
python3 configure.py
```
Then open **`http://localhost:8080`** in Chrome/Edge.

> To stop the app, just close the little black window it opens (or press `Ctrl+C` in it).

---

## 🧭 Using the app (a quick tour)

The page is split into **tabs** along the top.

| Tab | What it's for |
|-----|---------------|
| **Vehicle** dropdown (top bar) | Pick which truck/machine you're building. Everything else updates to match. |
| **Vehicle Tuning** | The chosen vehicle's own settings — engine volumes, RPM, knock, turbo, etc. Also **Copy / Reset / Export / Import** and **Presets**. |
| **General, Remote, ESC, Transmission, Shaker, Lights, Servos, Sound, Dashboard, Trailer** | All the board settings, as friendly sliders and on/off switches. Hover any item for a plain-English explanation. |
| **🔊 Sound Forge** | Master volume, and a **＋ Change** button on each sound slot to browse/preview sounds or upload your own WAV. |
| **⚡ Flash** | Build the firmware and send it to your board (see below). |

**Saving:** changes you make show "● unsaved changes" at the top. Click **Save** to write them. (Flashing saves automatically first.)

---

## ⚡ Flashing your ESP32

1. Open the **⚡ Flash** tab.
2. **Disconnect the battery** from the controller. *(This matters — a connected battery pulls GPIO12 high and the upload will fail.)*
3. Plug the ESP32 into your computer with a USB **data** cable (not a charge-only cable).
4. Click **⚡ Build & Flash**.
   - First build takes a few minutes while it downloads the toolchain — later builds are quick.
5. A little window pops up asking for a serial port — pick the one that looks like **"CP2102"**, **"CH340"**, or **"USB Serial"**, and click **Connect**.
6. Watch the progress bar. When it says **✓ Done**, reconnect the battery. 🎉

**If it won't connect:** switch the **Speed** dropdown to **115200 (safe)** and try again, or unplug/replug the USB cable.

---

## 🆘 Troubleshooting

| Problem | Fix |
|---------|-----|
| Browser says flashing isn't supported | You're not in Chrome/Edge. Open `http://localhost:8080` in one of those. |
| "Python is not installed" | Install [Python 3](https://www.python.org/downloads/) and **tick "Add Python to PATH"**, then run the launcher again. |
| Page won't open | Make sure the black terminal window is still running. Re-run `start_webui.bat` / `start_webui.command`. |
| Upload fails / times out | **Battery disconnected?** Then unplug/replug USB, pick **115200 (safe)** speed, try again. |
| Board not detected in the port list | Install your board's USB driver (Google your board name + **"CP2102 driver"** or **"CH340 driver"**). |
| A setting looks broken | On the **Vehicle Tuning** tab, click **Reset** to restore that profile, then redo your changes. |
| Mac: "permission denied" | Open Terminal in the folder and run `chmod +x start_webui.command`. |

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
configure.py                 ← the local web server (runs the whole thing)
web/                         ← the browser app (UI + in-browser flasher)
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
| **Web Configurator & Flasher** | [turbotike](https://github.com/turbotike) |
| **Original firmware** | [TheDIYGuy999](https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32) |

This tool stands on the shoulders of TheDIYGuy999's incredible firmware — go give that project a ⭐.
