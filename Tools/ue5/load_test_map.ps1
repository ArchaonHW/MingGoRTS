# MingWar-RTS 測試地圖加載腳本
param(
    [string]$ProjectRoot = "C:\HW\MingGoRTS",
    [string]$EnginePath = "C:\Program Files\Epic Games\UE_5.4"
)

# 檢查 UE5 是否安裝
if (-not (Test-Path "$EnginePath\Engine\Binaries\Win64\UnrealEditor.exe")) {
    Write-Error "找不到 UE5 Editor，請檢查路徑: $EnginePath"
    Write-Host "您可以透過參數 -EnginePath 指定 UE5 安裝路徑"
    exit 1
}

# 檢查項目文件是否存在
$projectFile = Join-Path $ProjectRoot "MingWar-RTS.uproject"
if (-not (Test-Path $projectFile)) {
    Write-Error "找不到項目文件: $projectFile"
    exit 1
}

# 檢查地圖是否存在
$mapFile = Join-Path $ProjectRoot "Content\Maps\TestMap\map_config.json"
if (-not (Test-Path $mapFile)) {
    Write-Warning "找不到地圖配置文件，請先創建 TestMap"
    Write-Host "您可以在 UE5 編輯器中創建 TestMap 並保存到 Content/Maps/TestMap/"
}

Write-Host "正在啟動 UE5 Editor 並加載 MingWar-RTS 項目..." -ForegroundColor Cyan
Write-Host "項目路徑: $projectFile" -ForegroundColor Gray
Write-Host "地圖: Content/Maps/TestMap/TestMap.umap" -ForegroundColor Gray
Write-Host ""

# 啟動 UE5 Editor
$editorExe = Join-Path $EnginePath "Engine\Binaries\Win64\UnrealEditor.exe"
$arguments = "`"$projectFile`" -game -log"

Start-Process -FilePath $editorExe -ArgumentList $arguments -WorkingDirectory $ProjectRoot

Write-Host "✅ UE5 Editor 已啟動" -ForegroundColor Green
Write-Host ""
Write-Host "下一步操作：" -ForegroundColor Yellow
Write-Host "1. 在 UE5 編輯器中，打開 Content Browser"
Write-Host "2. 導航到 Content/Maps/TestMap/"
Write-Host "3. 雙擊 TestMap.umap 打開地圖"
Write-Host "4. 點擊 Play 按鈕開始測試"
Write-Host ""
Write-Host "控制說明：" -ForegroundColor Cyan
Write-Host "  - 左鍵拖拽: 框選單位"
Write-Host "  - 右鍵點擊: 移動選中單位"
Write-Host "  - Ctrl+A: 選中所有單位"