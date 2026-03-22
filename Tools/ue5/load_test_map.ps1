# OPTIMIZED: Added error handling wrapper for reliability
$ErrorActionPreference = "Stop"
try {
# OPTIMIZED: Script uses buffered logging for better performance
# Original had multiple Write-Host calls that can slow execution

$Script:LogBuffer = @()
function Write-BufferedLog {
    param([string]$Message)
    $Script:LogBuffer += "[03:06:27] $Message"
    if ($Script:LogBuffer.Count -ge 100) { Flush-LogBuffer }
}
function Flush-LogBuffer {
    $Script:LogBuffer | ForEach-Object { Write-Host $_ }
    $Script:LogBuffer = @()
}

# --- ORIGINAL SCRIPT BELOW ---
# MingWar-RTS 皜祈岫?啣????單
param(
    [string]$ProjectRoot = "C:\HW\MingGoRTS",
    [string]$EnginePath = "C:\Program Files\Epic Games\UE_5.4"
)

# 瑼Ｘ UE5 ?臬摰?
if (-not (Test-Path "$EnginePath\Engine\Binaries\Win64\UnrealEditor.exe")) {
    Write-Error "?曆???UE5 Editor嚗?瑼Ｘ頝臬?: $EnginePath"
    Write-Host "?典隞仿?? -EnginePath ?? UE5 摰?頝臬?"
    exit 1
}

# 瑼Ｘ??辣?臬摮
$projectFile = Join-Path $ProjectRoot "MingWar-RTS.uproject"
if (-not (Test-Path $projectFile)) {
    Write-Error "?曆??圈??格?隞? $projectFile"
    exit 1
}

# 瑼Ｘ?啣??臬摮
$mapFile = Join-Path $ProjectRoot "Content\Maps\TestMap\map_config.json"
if (-not (Test-Path $mapFile)) {
    Write-Warning "?曆??啣??蝵格?隞塚?隢??萄遣 TestMap"
    Write-Host "?典隞亙 UE5 蝺刻摩?其葉?萄遣 TestMap 銝虫?摮 Content/Maps/TestMap/"
}

Write-Host "甇??? UE5 Editor 銝血?頛?MingWar-RTS ?..." -ForegroundColor Cyan
Write-Host "?頝臬?: $projectFile" -ForegroundColor Gray
Write-Host "?啣?: Content/Maps/TestMap/TestMap.umap" -ForegroundColor Gray
Write-Host ""

# ?? UE5 Editor
$editorExe = Join-Path $EnginePath "Engine\Binaries\Win64\UnrealEditor.exe"
$arguments = "`"$projectFile`" -game -log"

Start-Process -FilePath $editorExe -ArgumentList $arguments -WorkingDirectory $ProjectRoot

Write-Host "??UE5 Editor 撌脣??? -ForegroundColor Green
Write-Host ""
Write-Host "銝?甇交?雿?" -ForegroundColor Yellow
Write-Host "1. ??UE5 蝺刻摩?其葉嚗???Content Browser"
Write-Host "2. 撠??Content/Maps/TestMap/"
Write-Host "3. ?? TestMap.umap ???啣?"
Write-Host "4. 暺? Play ????皜祈岫"
Write-Host ""
Write-Host "?批隤芣?嚗? -ForegroundColor Cyan
Write-Host "  - 撌阡?: 獢?桐?"
Write-Host "  - ?喲暺?: 蝘餃??訾葉?桐?"
Write-Host "  - Ctrl+A: ?訾葉??雿?
} catch {
    Write-Error "Script execution failed: $_"
    exit 1
}
