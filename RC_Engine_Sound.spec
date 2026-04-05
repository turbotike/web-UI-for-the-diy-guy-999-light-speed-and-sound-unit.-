# -*- mode: python ; coding: utf-8 -*-
# PyInstaller spec for RC Engine Sound ESP32 Configurator
# Build with:  pyinstaller RC_Engine_Sound.spec
# Output:      dist\RC Engine Sound Configurator.exe

import glob
import os

# ── data files to bundle ──────────────────────────────────────────────────────

# tools/ HTML files  (Audio2Header.html, Header2Audio.html)
tools_datas = [
    (f.replace("\\", "/"), "tools")
    for f in glob.glob("tools/*.html")
]

# src/ top-level .h config headers (0_generalSettings.h … 10_Trailer.h)
src_h_datas = [
    (f.replace("\\", "/"), "src")
    for f in glob.glob("src/*.h")
]

# src/vehicles/ — all vehicle presets (~142 files)
vehicles_datas = [
    (f.replace("\\", "/"), os.path.join("src", "vehicles"))
    for f in glob.glob("src/vehicles/*.h")
]

# src/src.ino — main Arduino sketch
src_ino_datas = [
    ("src/src.ino", "src")
] if os.path.isfile("src/src.ino") else []

# src/src/ — support headers and source files needed by pio compile
src_src_datas = [
    (f.replace("\\", "/"), os.path.join("src", "src"))
    for f in glob.glob("src/src/*")
    if os.path.isfile(f) and not os.path.basename(f).startswith(".")
]

# platformio.ini — project config needed for pio run
pio_ini_datas = [("platformio.ini", ".")] if os.path.isfile("platformio.ini") else []

all_datas = tools_datas + src_h_datas + vehicles_datas + src_ino_datas + src_src_datas + pio_ini_datas

# ── analysis ──────────────────────────────────────────────────────────────────

a = Analysis(
    ["webui_desktop_app.py"],
    pathex=["."],          # ensures configure.py in the same dir is importable
    binaries=[],
    datas=all_datas,
    hiddenimports=[
        # configure.py lives alongside webui_desktop_app.py; import is inside
        # an if-frozen block so PyInstaller's static analyser won't see it.
        "configure",
        # pyserial (used by configure.py for COM-port listing and port testing)
        "serial",
        "serial.tools",
        "serial.tools.list_ports",
        "serial.tools.list_ports_common",
        "serial.tools.list_ports_windows",
    ],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[
        # keep the bundle lean
        "tkinter",
        "unittest",
        "pydoc",
        "doctest",
    ],
    noarchive=False,
    optimize=0,
)

pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name="RC Engine Sound Configurator",
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,          # compress the exe (reduces size ~20-30%)
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,     # no console window
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon='app_icon.ico',  # DIYGuy999 branded icon
)
