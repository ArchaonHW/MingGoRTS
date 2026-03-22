# UE5 環境設置腳本
Write-Host "=== 設置 UE5 環境變數 ===" -ForegroundColor Green

# 檢查 UE5 安裝路徑
$UE5Path = "C:\Program Files\Epic Games\UE_5.7"
if (-not (Test-Path $UE5Path)) {
    Write-Host "錯誤：找不到 UE5 安裝路徑：$UE5Path" -ForegroundColor Red
    Write-Host "請確認 UE5 已正確安裝" -ForegroundColor Yellow
    Read-Host "按 Enter 鍵退出"
    exit 1
}

Write-Host "找到 UE5 安裝路徑：$UE5Path" -ForegroundColor Green

# 檢查 UnrealEditor.exe 是否存在
$UEEditorPath = "$UE5Path\Engine\Binaries\Win64"
$UnrealEditorExe = "$UEEditorPath\UnrealEditor.exe"
if (-not (Test-Path $UnrealEditorExe)) {
    Write-Host "錯誤：找不到 UnrealEditor.exe" -ForegroundColor Red
    Write-Host "路徑：$UnrealEditorExe" -ForegroundColor Yellow
    Read-Host "按 Enter 鍵退出"
    exit 1
}

Write-Host "找到 UnrealEditor.exe：$UnrealEditorExe" -ForegroundColor Green

# 獲取當前用戶 PATH
$CurrentUserPath = [Environment]::GetEnvironmentVariable("PATH", "User")
if ($null -eq $CurrentUserPath) {
    $CurrentUserPath = ""
}

# 檢查是否已經在 PATH 中
if ($CurrentUserPath -like "*$UEEditorPath*") {
    Write-Host "UE5 路徑已存在於用戶環境變數中" -ForegroundColor Yellow
} else {
    # 添加到用戶環境變數
    $NewUserPath = $CurrentUserPath + ";" + $UEEditorPath
    try {
        [Environment]::SetEnvironmentVariable("PATH", $NewUserPath, "User")
        Write-Host "✓ 成功添加到用戶環境變數" -ForegroundColor Green
    } catch {
        Write-Host "⚠ 添加到用戶環境變數時出現錯誤" -ForegroundColor Yellow
        Write-Host "可能需要管理員權限" -ForegroundColor Yellow
    }
}

# 添加到當前會話
$CurrentSessionPath = $env:PATH
if ($CurrentSessionPath -like "*$UEEditorPath*") {
    Write-Host "UE5 路徑已存在於當前會話中" -ForegroundColor Yellow
} else {
    $env:PATH = $CurrentSessionPath + ";" + $UEEditorPath
    Write-Host "✓ 已將 UE5 添加到當前會話 PATH" -ForegroundColor Green
}

Write-Host "`n測試 UE5 命令..." -ForegroundColor Cyan

# 測試 UnrealEditor 命令
try {
    $UnrealEditor = Get-Command "UnrealEditor" -ErrorAction Stop
    Write-Host "✓ UnrealEditor 命令可用" -ForegroundColor Green
    Write-Host "  路徑：$($UnrealEditor.Source)" -ForegroundColor Gray
} catch {
    Write-Host "⚠ UnrealEditor 命令當前不可用" -ForegroundColor Yellow
    Write-Host "  請重新啟動命令提示字元或重新登錄以使環境變數生效" -ForegroundColor Yellow
}

Write-Host "`n=== UE5 環境設置完成 ===" -ForegroundColor Green
Write-Host "`n您現在可以使用以下命令：" -ForegroundColor Cyan
Write-Host "  UnrealEditor                    - 啟動 UE 編輯器" -ForegroundColor White
Write-Host "  UnrealEditor.exe                 - 啟動 UE 編輯器" -ForegroundColor White
Write-Host "  UnrealEditor.exe [專案路徑]      - 打開指定專案" -ForegroundColor White
Write-Host "`n示例：" -ForegroundColor Cyan
Write-Host '  UnrealEditor.exe "c:\HW\MingGoRTS\MingGoRTS.uproject"' -ForegroundColor Gray

# 創建快速啟動腳本
$QuickStartScript = '@echo off
echo 啟動 MingGoRTS 專案...
UnrealEditor.exe "c:\HW\MingGoRTS\MingGoRTS.uproject"'

$QuickStartPath = "c:\HW\MingGoRTS\LaunchMingGoRTS.bat"
$QuickStartScript | Out-File -FilePath $QuickStartPath -Encoding ASCII
Write-Host "`n✓ 已創建快速啟動腳本：$QuickStartPath" -ForegroundColor Green

Read-Host "`n按 Enter 鍵退出"
