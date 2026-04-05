#!/usr/bin/env python3
"""Single-exe launcher for the RC Engine Sound ESP32 Configurator.

1. Extracts bundled data to %LOCALAPPDATA%\\RC_Engine_Sound_ESP32 (writable).
2. Starts the HTTP server in a daemon thread.
3. Opens Microsoft Edge (or Chrome) in --app mode so it looks like a native app.
4. Blocks until the browser window closes, then exits cleanly.
"""

import os
import shutil
import subprocess
import sys
import threading
import time
import urllib.request

PORT = 8080
URL = f"http://127.0.0.1:{PORT}/"

# ── helpers ──────────────────────────────────────────────────────────────────

_APP_BROWSER_FLAGS = [
    f"--app={URL}",
    "--new-window",
    "--disable-extensions",
]


def _find_browser() -> str:
    """Return path to Edge or Chrome, preference: Edge > Chrome > None."""
    candidates = [
        # Microsoft Edge (most Windows 10/11 systems)
        os.path.expandvars(r"%ProgramFiles(x86)%\Microsoft\Edge\Application\msedge.exe"),
        os.path.expandvars(r"%ProgramFiles%\Microsoft\Edge\Application\msedge.exe"),
        os.path.expandvars(r"%LOCALAPPDATA%\Microsoft\Edge\Application\msedge.exe"),
        # Google Chrome
        os.path.expandvars(r"%ProgramFiles%\Google\Chrome\Application\chrome.exe"),
        os.path.expandvars(r"%ProgramFiles(x86)%\Google\Chrome\Application\chrome.exe"),
        os.path.expandvars(r"%LOCALAPPDATA%\Google\Chrome\Application\chrome.exe"),
    ]
    for path in candidates:
        if os.path.isfile(path):
            return path
    return ""


def _open_app_window() -> "subprocess.Popen | None":
    """Open Edge/Chrome in --app mode. Returns the process, or None on failure."""
    browser = _find_browser()
    if not browser:
        # Fallback: open in whatever the OS default browser is
        import webbrowser
        webbrowser.open(URL)
        return None
    return subprocess.Popen([browser] + _APP_BROWSER_FLAGS)


def is_server_up(url: str) -> bool:
    try:
        with urllib.request.urlopen(url, timeout=0.8) as resp:
            return resp.status == 200
    except Exception:
        return False


def wait_for_server(url: str, timeout_sec: float = 10.0) -> bool:
    start = time.time()
    while time.time() - start < timeout_sec:
        if is_server_up(url):
            return True
        time.sleep(0.2)
    return False


# ── frozen-mode helpers ───────────────────────────────────────────────────────

def get_app_data_dir() -> str:
    """Persistent, writable directory for app data (vehicle .h files etc.)."""
    base = os.environ.get("LOCALAPPDATA") or os.path.expanduser("~")
    return os.path.join(base, "RC_Engine_Sound_ESP32")


def _sync_dir(src_dir: str, dst_dir: str, overwrite: bool) -> None:
    """Recursively copy src_dir → dst_dir.
    If overwrite=False, existing destination files are preserved (user edits).
    """
    os.makedirs(dst_dir, exist_ok=True)
    for entry in os.scandir(src_dir):
        dst_path = os.path.join(dst_dir, entry.name)
        if entry.is_dir():
            _sync_dir(entry.path, dst_path, overwrite)
        else:
            if overwrite or not os.path.exists(dst_path):
                shutil.copy2(entry.path, dst_path)


def sync_bundle_to_data_dir(bundle_dir: str, data_dir: str) -> None:
    """Copy bundled files from PyInstaller extraction dir to writable app-data dir.

    Strategy:
    - tools/*.html  → always overwrite  (app code, not user data)
    - src/*.h       → copy only if missing  (preserve user edits)
    - src/vehicles/ → copy only if missing  (preserve user edits)
    """
    os.makedirs(data_dir, exist_ok=True)

    # tools/ — always overwrite (HTML shipped with the app)
    tools_src = os.path.join(bundle_dir, "tools")
    if os.path.isdir(tools_src):
        _sync_dir(tools_src, os.path.join(data_dir, "tools"), overwrite=True)

    # src/ — never overwrite (user may have edited their .h files)
    # This also covers src/src.ino and src/src/*.h|cpp bundled for pio compilation.
    src_src = os.path.join(bundle_dir, "src")
    if os.path.isdir(src_src):
        _sync_dir(src_src, os.path.join(data_dir, "src"), overwrite=False)

    # platformio.ini — always overwrite (app config, not user data)
    pio_ini_src = os.path.join(bundle_dir, "platformio.ini")
    if os.path.isfile(pio_ini_src):
        shutil.copy2(pio_ini_src, os.path.join(data_dir, "platformio.ini"))

    os.makedirs(os.path.join(data_dir, "presets"), exist_ok=True)


def start_server_thread(data_dir: str) -> None:
    """Import configure, patch its globals to data_dir, start HTTPServer in daemon thread."""
    from http.server import HTTPServer
    import configure as _cfg  # bundled via hiddenimports

    # Redirect all file I/O to the writable app-data directory.
    _cfg.ROOT = data_dir
    _cfg.SRC = os.path.join(data_dir, "src")
    _cfg.TOOLS = os.path.join(data_dir, "tools")
    _cfg.PRESETS = os.path.join(data_dir, "presets")

    server = HTTPServer(("127.0.0.1", PORT), _cfg.Handler)
    t = threading.Thread(target=server.serve_forever, daemon=True)
    t.start()


def start_server_subprocess() -> "subprocess.Popen":
    """Start configure.py as a hidden subprocess (plain .py / development mode)."""
    creationflags = 0
    startupinfo = None
    if os.name == "nt":
        creationflags = getattr(subprocess, "CREATE_NO_WINDOW", 0)
        startupinfo = subprocess.STARTUPINFO()
        startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW

    root = os.path.dirname(os.path.abspath(__file__))
    script = os.path.join(root, "configure.py")
    return subprocess.Popen(
        [sys.executable, script],
        cwd=root,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        creationflags=creationflags,
        startupinfo=startupinfo,
    )


# ── main ──────────────────────────────────────────────────────────────────────

def main() -> int:
    if getattr(sys, "frozen", False):
        # ── Running as PyInstaller .exe ───────────────────────────────────────
        bundle_dir = sys._MEIPASS  # type: ignore[attr-defined]
        data_dir = get_app_data_dir()
        sync_bundle_to_data_dir(bundle_dir, data_dir)
        start_server_thread(data_dir)
        if not wait_for_server(URL):
            return 1
    else:
        # ── Running as plain .py (development) ───────────────────────────────
        if not is_server_up(URL):
            srv = start_server_subprocess()
            if not wait_for_server(URL):
                srv.terminate()
                print("Failed to start configurator server.")
                return 1

    # ── open Edge / Chrome in --app mode ─────────────────────────────────────
    _open_app_window()

    # Keep the server (daemon thread) alive while the app is in use.
    # IMPORTANT: if Edge/Chrome is already running, the spawned process
    # exits immediately (the browser reuses its existing instance), so we
    # cannot rely on browser_proc.wait().  We just sleep until the user
    # kills the exe (Task Manager / Ctrl-C / system shutdown).
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        pass

    return 0


if __name__ == "__main__":
    sys.exit(main())
