@echo off
:: ============================================================
:: build_exe.bat  –  Build "RC Engine Sound Configurator.exe"
:: ============================================================
:: Produces:  dist\RC Engine Sound Configurator.exe  (~40-80 MB)
:: No Python installation required on the target machine.
:: ============================================================

cd /d "%~dp0"

:: ── locate Python ────────────────────────────────────────────
set PY=
where python >nul 2>&1 && set PY=python
if not defined PY (
    set PY=C:\Program Files\Inkscape\bin\python.exe
)
echo [build] Using Python: %PY%
"%PY%" --version

:: ── install / upgrade PyInstaller ───────────────────────────
echo.
echo [build] Installing / verifying PyInstaller...
"%PY%" -m pip install --quiet --upgrade pyinstaller
if errorlevel 1 (
    echo ERROR: pip failed. Check your Python / internet connection.
    pause
    exit /b 1
)

:: ── clean previous build ─────────────────────────────────────
if exist build\RC* rmdir /s /q build
if exist "dist\RC Engine Sound Configurator.exe" del /q "dist\RC Engine Sound Configurator.exe"

:: ── run PyInstaller ──────────────────────────────────────────
echo.
echo [build] Running PyInstaller (this takes 1-3 minutes)...
"%PY%" -m PyInstaller RC_Engine_Sound.spec --noconfirm

if errorlevel 1 (
    echo.
    echo BUILD FAILED.  Check the output above for errors.
    pause
    exit /b 1
)

:: ── success ──────────────────────────────────────────────────
echo.
echo ============================================================
echo  BUILD COMPLETE
echo ============================================================
echo  EXE:  %~dp0dist\RC Engine Sound Configurator.exe
echo.
echo  Just double-click that file.  No Python needed.
echo  Vehicle configs are saved to:
echo    %%LOCALAPPDATA%%\RC_Engine_Sound_ESP32\src\vehicles\
echo.
echo  NOTE: Build / Flash buttons require PlatformIO installed
echo        separately (pio command in PATH).
echo ============================================================
echo.
pause
