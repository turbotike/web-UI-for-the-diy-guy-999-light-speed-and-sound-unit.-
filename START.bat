@echo off
title RC Engine Sound Configurator
color 0A
echo.
echo  ============================================
echo   RC Engine Sound ESP32 Configurator
echo  ============================================
echo.

:: Check if Python is installed
where python >nul 2>&1
if %errorlevel%==0 (
    goto :check_version
)

:: Try py launcher (Windows Store installs)
where py >nul 2>&1
if %errorlevel%==0 (
    set "PYTHON_CMD=py"
    goto :launch
)

:: Python not found — offer to install
echo  Python is not installed on this computer.
echo  It's needed to run the configurator (free, safe, 2 minutes).
echo.
echo  Press any key to download and install Python automatically...
echo  (Or press Ctrl+C to cancel)
pause >nul

echo.
echo  Downloading Python installer...
set "INSTALLER=%TEMP%\python_installer.exe"
powershell -Command "& { [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri 'https://www.python.org/ftp/python/3.12.7/python-3.12.7-amd64.exe' -OutFile '%INSTALLER%' }"

if not exist "%INSTALLER%" (
    echo.
    echo  ERROR: Download failed. Please install Python manually from:
    echo  https://www.python.org/downloads/
    echo.
    pause
    exit /b 1
)

echo  Installing Python (this may take a minute)...
echo  A User Account Control prompt may appear — click Yes.
"%INSTALLER%" /passive InstallAllUsers=0 PrependPath=1 Include_test=0 Include_doc=0

if %errorlevel% neq 0 (
    echo.
    echo  ERROR: Python installation failed or was cancelled.
    echo  Please install Python manually from https://www.python.org/downloads/
    echo  Make sure to check "Add Python to PATH" during install.
    echo.
    pause
    exit /b 1
)

del "%INSTALLER%" >nul 2>&1

echo  Python installed successfully!
echo.

:: Refresh PATH so we can find python
set "PATH=%LOCALAPPDATA%\Programs\Python\Python312\;%LOCALAPPDATA%\Programs\Python\Python312\Scripts\;%PATH%"

:check_version
:: Verify python works
python --version >nul 2>&1
if %errorlevel%==0 (
    set "PYTHON_CMD=python"
    goto :launch
)

:: Fallback to py launcher
py --version >nul 2>&1
if %errorlevel%==0 (
    set "PYTHON_CMD=py"
    goto :launch
)

echo  ERROR: Python installed but not found in PATH.
echo  Please restart your computer and double-click START.bat again.
echo.
pause
exit /b 1

:launch
echo  Starting configurator...
echo.

:: Open browser after a short delay (gives server time to start)
start "" cmd /c "timeout /t 2 /nobreak >nul & start http://localhost:8080"

:: Launch the server (this blocks until Ctrl+C)
cd /d "%~dp0"
%PYTHON_CMD% configure.py

echo.
echo  Server stopped. You can close this window.
pause
