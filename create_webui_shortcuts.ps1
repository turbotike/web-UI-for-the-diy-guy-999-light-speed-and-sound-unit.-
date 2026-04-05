$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$desktopCandidates = @()
$desktopCandidates += [Environment]::GetFolderPath("Desktop")
$desktopCandidates += Join-Path $env:USERPROFILE "Desktop"
if ($env:OneDrive) {
    $desktopCandidates += Join-Path $env:OneDrive "Desktop"
}
$desktopTargets = $desktopCandidates | Where-Object { $_ -and (Test-Path $_) } | Select-Object -Unique
if (-not $desktopTargets -or $desktopTargets.Count -eq 0) {
    Write-Host "Could not find a valid Desktop folder." -ForegroundColor Red
    exit 1
}

$exePath = Join-Path $root "dist\RC Engine Sound Configurator.exe"
if (-not (Test-Path $exePath)) {
    Write-Host "Missing file: $exePath" -ForegroundColor Red
    exit 1
}

$wsh = New-Object -ComObject WScript.Shell

Write-Host "Created shortcuts:" -ForegroundColor Green
foreach ($desktop in $desktopTargets) {
    $exeLnk = Join-Path $desktop "RC Engine Sound Configurator.lnk"

    $sc3 = $wsh.CreateShortcut($exeLnk)
    $sc3.TargetPath = $exePath
    $sc3.WorkingDirectory = $root
    $sc3.Description = "Open RC Engine Sound Configurator"
    $sc3.Save()

    Write-Host " - $exeLnk"
}
