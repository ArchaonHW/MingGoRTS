# MingGoRTS 快速修復腳本

Write-Host "🎯 MingGoRTS 快速診斷和修復" -ForegroundColor Green

# 清理並重新生成
Write-Host "🧹 清理建置檔案..." -ForegroundColor Yellow
Remove-Item -Path "C:\HW\MingGoRTS\Intermediate" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "C:\HW\MingGoRTS\Binaries" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "C:\HW\MingGoRTS\Saved" -Recurse -Force -ErrorAction SilentlyContinue

Write-Host "✅ 清理完成" -ForegroundColor Green

# 重新生成專案檔案
Write-Host "🔧 重新生成專案檔案..." -ForegroundColor Yellow
$Process = Start-Process -FilePath "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -ArgumentList "-projectfiles", "-project=C:\HW\MingGoRTS\MingGoRTS.uproject", "-game", "-progress" -Wait -PassThru

if ($Process.ExitCode -eq 0) {
    Write-Host "✅ 專案檔案生成成功" -ForegroundColor Green
} else {
    Write-Host "❌ 專案檔案生成失敗" -ForegroundColor Red
    exit 1
}

# 測試編譯
Write-Host "🎮 測試編譯..." -ForegroundColor Yellow
$Process = Start-Process -FilePath "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -ArgumentList "MingGoRTSEditor", "Win64", "Development", "-project=C:\HW\MingGoRTS\MingGoRTS.uproject", "-progress" -Wait -PassThru

if ($Process.ExitCode -eq 0) {
    Write-Host "✅ 編譯成功！" -ForegroundColor Green
    Write-Host "🚀 現在可以啟動 Unreal Editor" -ForegroundColor Green
} else {
    Write-Host "❌ 編譯失敗" -ForegroundColor Red
    Write-Host "📋 請檢查上述錯誤訊息" -ForegroundColor Yellow
    exit 1
}

Write-Host "🎉 腳本執行完成！" -ForegroundColor Green
