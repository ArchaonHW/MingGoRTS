# MingGoRTS 版本提交準備腳本
param(
    [string]$Version = "1.0.0",
    [string]$CommitMessage = "MingGoRTS v$Version - UE5.7 Standardization and Cleanup",
    [switch]$DryRun = $false
)

Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS 版本提交準備" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host "版本: $Version" -ForegroundColor Yellow
Write-Host "提交訊息: $CommitMessage" -ForegroundColor Yellow
Write-Host "預覽模式: $DryRun" -ForegroundColor Yellow
Write-Host ""

# 檢查Git狀態
Write-Host "檢查Git狀態..." -ForegroundColor Cyan
try {
    $Status = git status --porcelain
    if ($Status) {
        Write-Host "發現未提交的更改:" -ForegroundColor Yellow
        $Status | ForEach-Object { Write-Host "  $_" -ForegroundColor White }
    } else {
        Write-Host "沒有未提交的更改" -ForegroundColor Green
    }
} catch {
    Write-Host "無法檢查Git狀態: $($_.Exception.Message)" -ForegroundColor Red
}

if (-not $DryRun) {
    Write-Host ""
    Write-Host "執行版本提交..." -ForegroundColor Cyan
    
    # 添加檔案
    try {
        git add .
        Write-Host "檔案已添加到暫存區" -ForegroundColor Green
    } catch {
        Write-Host "添加檔案失敗: $($_.Exception.Message)" -ForegroundColor Red
        exit 1
    }
    
    # 創建提交
    try {
        git commit -m $CommitMessage
        Write-Host "提交創建成功" -ForegroundColor Green
    } catch {
        Write-Host "提交失敗: $($_.Exception.Message)" -ForegroundColor Red
        exit 1
    }
    
    # 創建標籤
    try {
        git tag -a "v$Version" -m "MingGoRTS Version $Version"
        Write-Host "標籤 v$Version 創建成功" -ForegroundColor Green
    } catch {
        Write-Host "標籤創建失敗: $($_.Exception.Message)" -ForegroundColor Red
    }
    
    Write-Host ""
    Write-Host "🎉 版本提交準備完成!" -ForegroundColor Green
    Write-Host "版本: v$Version" -ForegroundColor Cyan
} else {
    Write-Host ""
    Write-Host "🔍 預覽模式 - 未執行實際操作" -ForegroundColor Yellow
    Write-Host "如需執行提交，請移除 -DryRun 參數" -ForegroundColor Yellow
}

# 生成簡單報告
$Report = @"
# MingGoRTS 版本提交報告

## 版本資訊
- 版本: $Version
- 提交時間: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
- 提交訊息: $CommitMessage

## 主要變更
- UE5版本統一為5.7
- 專案清理和整理
- 更新.gitignore規則

## 狀態
- 準備提交: 是
- 標籤: v$Version

---
報告生成時間: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
"@

Set-Content -Path "CommitReport_$Version.md" -Value $Report -Encoding UTF8
Write-Host "提交報告已生成: CommitReport_$Version.md" -ForegroundColor Green
