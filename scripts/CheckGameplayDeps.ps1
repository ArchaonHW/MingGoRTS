# CheckGameplayDeps.ps1 — dependency-direction guard for the Gameplay layer.
# Fails (exit 1) if any Gameplay source includes a forbidden subsystem:
# Rendering / GUI / Platform / OpenGL / glad / glfw / ImGui / raw GL headers.
#
# Usage: powershell -NoProfile -File scripts/CheckGameplayDeps.ps1
# Works from any cwd; resolves the repo root from the script location.

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$gameplayDir = Join-Path $root 'Gameplay'

if (-not (Test-Path $gameplayDir)) {
    Write-Host "SKIP: Gameplay/ not present"
    exit 0
}

$forbidden = 'Rendering/', 'GUI/', 'Platform/', 'OpenGL', 'glad', 'glfw', 'ImGui', 'GL/', 'GLES', 'KHR/'
$files = Get-ChildItem $gameplayDir -Recurse -Include *.h,*.cpp
$violations = @()

foreach ($file in $files) {
    # @(...) forces array even for single-line files
    $lines = @(Get-Content $file.FullName)
    for ($i = 0; $i -lt $lines.Count; ++$i) {
        # normalize Windows-style include separators before token match
        $line = ($lines[$i] -replace '\\', '/')
        if ($line -notmatch '^\s*#\s*include') { continue }
        foreach ($token in $forbidden) {
            if ($line -like "*$token*") {
                $violations += "{0}:{1}: {2}" -f $file.FullName, ($i + 1), $lines[$i].Trim()
            }
        }
    }
}

if ($violations.Count -gt 0) {
    Write-Host "FAIL: forbidden includes in Gameplay/:"
    $violations | ForEach-Object { Write-Host "  $_" }
    exit 1
}

Write-Host ("PASS: {0} files clean" -f $files.Count)
exit 0
