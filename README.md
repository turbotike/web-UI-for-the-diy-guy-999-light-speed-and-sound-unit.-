# RC Engine Sound ESP32 — Web Configurator

A browser-based GUI for the ESP32 RC engine sound & light controller. Change every setting from your browser — no code editing required.

---

## Quick Start

1. **Double-click `START.bat`** (Windows) or run `python3 configure.py` (Mac/Linux)
2. Browser opens to `http://localhost:8080`
3. Pick your vehicle, adjust settings, hit **Compile & Flash**

> If Python isn't installed, the launcher will offer to install it.

---

## Web UI Sections

### Vehicle

Select the active vehicle sound profile from the dropdown. The configurator loads all `.h` files from `src/vehicles/`. Over 80 profiles are included (trucks, crawlers, muscle cars, heavy equipment, military, tractors, etc.).

- **Copy Vehicle** — duplicate the current profile so you can customize without touching the original
- **Reset Vehicle** — restore the profile to its factory state (from backup)
- **Export .h** — download the current config as a `.h` file (bakes in your current RPM, loop points, and pitch settings)
- **Import .h** — upload a previously exported config
- **Presets** — save and load named snapshots of the current vehicle config

### Sound Forge

The live sound builder. Load any engine sound, preview it in the browser, and dial in the sound character before flashing.

- **Sound browser** — browse all included sounds by category, preview with one click
- **Install** — copy a sound into the active vehicle profile
- **Loop start / Loop end** — set the sample loop region (where the engine sound repeats)
- **Crossfade** — blend between loop start and end for seamless looping
- **Smoothing** — reduce pops and clicks in the looped audio
- **RPM** — simulate engine RPM to preview how the sound behaves at different speeds
- **Pitch** — adjust base pitch of the idle and rev sounds

> Export bakes in your current RPM, loop points, and pitch settings.

### General Settings (`0_generalSettings.h`)

Board-level options:

| Setting | What it does |
|---------|-------------|
| **Communication** | PWM, PPM, SBUS, or IBUS — match your receiver type |
| **Board version** | Select your PCB revision |
| **Battery protection** | Low voltage cutoff to protect LiPo cells |
| **Neopixel** | Enable WS2812 LED strip on GPIO0 |
| **Debug** | Serial debug output (leave off for normal use) |

### Remote / Communication (`2_Remote.h`)

Maps your transmitter's channels to firmware functions.

| Setting | What it does |
|---------|-------------|
| **Remote profile** | Pre-made profiles for Flysky FS-i6X, FS-i6S, etc. |
| **Channel assignments** | STEERING, GEARBOX, THROTTLE, HORN, FUNCTION_R/L, etc. |
| **Channel reverse** | Flip direction of any channel |
| **Auto-zero** | Enable only for self-centering sticks (NOT switches or pots) |
| **Pulse calibration** | Neutral width and span — tune if your sticks don't center at 1500µs |
| **Exponential curves** | Non-linear throttle/steering for more precision near center |

### ESC (`3_ESC.h`)

Electronic speed controller output on GPIO33.

| Setting | What it does |
|---------|-------------|
| **ESC pulse span** | Total servo pulse range (match your ESC) |
| **Takeoff punch** | Extra throttle burst when starting from standstill |
| **Ramp time** | Acceleration/deceleration smoothing per gear |
| **Braking steps** | How aggressively the ESC brakes |
| **Crawler ESC ramp** | Separate ramp for crawler mode |
| **Reverse plus** | Extra reverse authority |
| **Dragon / Quicrun** | Enable specific ESC brand support |

### Transmission (`4_Transmission.h`)

Simulated gearbox that syncs with engine sounds.

| Setting | What it does |
|---------|-------------|
| **Transmission type** | Manual 3-speed, automatic with torque converter, or double-clutch |
| **Gear ratios** | RPM thresholds for each gear |
| **Shifting time** | How long each gear change takes |
| **Clutch engage RPM** | Below this RPM, engine revs freely (virtual clutch) |
| **Crawler mode** | Minimal inertia for slow precision crawling |

### Shaker Motor (`5_Shaker.h`)

Vibration motor with eccentric weight for engine vibration feel.

| Setting | What it does |
|---------|-------------|
| **Shaker start** | Vibration intensity during engine cranking (0–255) |
| **Shaker idle** | Vibration intensity at idle (0–255) |
| **Shaker running** | Vibration intensity while revving (0–255) |
| **Shaker stop** | Duration of vibration when engine shuts off |

### Lights and Neopixel (`6_Lights.h`)

Up to 12 light outputs plus Neopixel strip.

| Setting | What it does |
|---------|-------------|
| **Headlights** | High beam, low beam, fog lights |
| **Tail / brake lights** | Auto-activate on braking |
| **Indicators** | Turn signals, hazard flasher |
| **Roof / cab lights** | Interior and roof bar lights |
| **Reversing light** | Auto-on in reverse |
| **Blue lights** | Emergency flasher patterns |
| **Xenon flash** | Simulated HID startup flicker |
| **Neopixel** | WS2812 RGB LED effects on GPIO0 |

### Servos (`7_Servos.h`)

Output mode for the 4 servo/PWM channels (GPIO13, 12, 14, 27).

| Mode | What it does |
|------|-------------|
| **SERVOS_DEFAULT** | Standard truck — steering, shifting, aux |
| **SERVOS_EXCAVATOR** | Excavator arm — bucket, dipper, boom, swing with ramps |
| **SERVOS_CRANE** | Crane — raw passthrough (no ramps or limits) |
| **SERVOS_PASSTHROUGH** | Full raw passthrough — sticks map directly to outputs. Good for dozers, dual-track vehicles, or anything needing direct control |

Each mode has per-channel limits (min/max µs) and ramp rates. Passthrough mode sets full range (1000–2000) with zero ramp.

### Sound (`8_Sound.h`)

Master volume and per-sound volume controls.

| Setting | What it does |
|---------|-------------|
| **Master volume** | Overall volume (0–250%) |
| **Engine volumes** | Separate idle, rev, turbo, wastegate, knock volumes |
| **Horn / siren** | Volume and variable-length loop support |
| **Jake brake** | Pneumatic engine brake sound volume |
| **Reversing beep** | Beep volume when in reverse |
| **Air / parking brake** | Brake sound volumes |
| **Track rattle** | Chain drive rattle volume (excavator/dozer mode) |
| **Hydraulic pump** | Pump whine volume (excavator/dozer mode) |

### Dashboard (`9_Dashboard.h`)

Optional LCD dashboard on SPI (ST7735).

### Trailer (`10_Trailer.h`)

ESP-NOW wireless trailer sound and light sync.

---

## Compile & Flash

The web UI has a **Compile & Flash** button that:
1. Runs PlatformIO to compile the firmware
2. Uploads the binary to the ESP32 over USB

**Requirements:**
- PlatformIO Core installed (the launcher checks for this)
- ESP32 connected via USB
- **Disconnect battery before flashing** (GPIO12 held high sets flash voltage to 1.8V and upload fails)

If the build fails, a troubleshooting popup will appear with common fixes.

---

## Tips

- **Always disconnect battery before flashing** — GPIO12 interference will corrupt the upload
- **Center sticks at power-on** — channels with auto-zero calibrate during the first few seconds
- **Don't use auto-zero on switches** — only enable it for self-centering sticks
- **Export before experimenting** — you can always import your last known-good config
- **Reset Vehicle** if things get weird — restores the original profile from backup

---

## Hardware

ESP32-WROOM-32 (30-pin) on the TheDIYGuy999 sound & light controller board.

| Pin | Function |
|-----|----------|
| GPIO25/26 | Audio out (to PAM8403 amp via 10kΩ resistors + pot) |
| GPIO33 | ESC servo signal output |
| GPIO13 | Servo CH1 output |
| GPIO12 | Servo CH2 output |
| GPIO14 | Servo CH3 output |
| GPIO27 | Servo CH4 output |
| GPIO0 | Neopixel data |
| GPIO34 | Receiver input (PWM/PPM/SBUS/IBUS) |

Schematics and PCB files are in the `hardware/` folder.

---

## Project Structure

```
├── START.bat              ← Double-click to launch (Windows)
├── configure.py           ← Web configurator server
├── src/
│   ├── src.ino            ← Main firmware
│   ├── 0_generalSettings.h ... 10_Trailer.h  ← Config tabs
│   └── vehicles/          ← 80+ vehicle sound profiles
├── hardware/              ← Eagle schematic & PCB files
└── tools/                 ← WAV↔header converters
```

---

## Credits

| | |
|---|---|
| **Web Configurator** | turbotike |
| **Original Firmware** | [TheDIYGuy999](https://github.com/TheDIYGuy999/Rc_Engine_Sound_ESP32) |
