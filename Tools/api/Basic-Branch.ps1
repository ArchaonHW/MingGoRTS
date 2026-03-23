# MingGoRTS Branch Management - Basic Version
# 基礎分支管理系統

param(
    [string]$Action = "help"
)

Write-Host "========================================" -ForegroundColor Green
Write-Host "MingGoRTS 分支管理系統" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

# 獲取當前分支
function Get-CurrentBranch {
    try {
        $branch = git rev-parse --abbrev-ref HEAD
        return $branch
    } catch {
        return "unknown"
    }
}

# 檢查工作目錄狀態
function Test-WorkingDirectory {
    $status = git status --porcelain
    return ($status -eq "")
}

# 執行預合併檢查
function Invoke-PreMergeChecks {
    Write-Host "執行預合併檢查..." -ForegroundColor Cyan
    
    # 檢查工作目錄
    if (-not (Test-WorkingDirectory)) {
        Write-Host "錯誤: 工作目錄不乾淨，請先提交變更" -ForegroundColor Red
        return $false
    }
    Write-Host "✓ 工作目錄乾淨" -ForegroundColor Green
    
    # 檢查測試
    Write-Host "運行測試..." -ForegroundColor Cyan
    $testFiles = Get-ChildItem -Path "C:\HW\MingGoRTS" -Filter "*Test*.ps1" -Recurse -ErrorAction SilentlyContinue
    foreach ($testFile in $testFiles) {
        try {
            & $testFile.FullName
            Write-Host "✓ 測試通過: $($testFile.Name)" -ForegroundColor Green
        } catch {
            Write-Host "✗ 測試失敗: $($testFile.Name)" -ForegroundColor Red
            return $false
        }
    }
    
    # 檢查構建
    Write-Host "執行構建檢查..." -ForegroundColor Cyan
    $buildScript = "C:\HW\MingGoRTS\BUILD_OPTIMIZED.cmd"
    if (Test-Path $buildScript) {
        try {
            $process = Start-Process -FilePath $buildScript -Wait -PassThru -NoNewWindow
            if ($process.ExitCode -eq 0) {
                Write-Host "✓ 構建成功" -ForegroundColor Green
            } else {
                Write-Host "✗ 構建失敗" -ForegroundColor Red
                return $false
            }
        } catch {
            Write-Host "✗ 構建檢查失敗" -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "! 未找到構建腳本，跳過構建檢查" -ForegroundColor Yellow
    }
    
    Write-Host "所有檢查通過" -ForegroundColor Green
    return $true
}

# 創建功能分支
function New-FeatureBranch {
    $featureName = Read-Host "請輸入功能名稱"
    $branchName = "feature/$featureName"
    
    Write-Host "創建功能分支: $branchName" -ForegroundColor Cyan
    
    try {
        git checkout develop | Out-Null
        git pull origin develop | Out-Null
        git checkout -b $branchName | Out-Null
        git push -u origin $branchName | Out-Null
        
        Write-Host "功能分支創建成功: $branchName" -ForegroundColor Green
    } catch {
        Write-Host "創建功能分支失敗" -ForegroundColor Red
    }
}

# 合併到目標分支
function Merge-ToTarget {
    $currentBranch = Get-CurrentBranch
    $targetBranch = Read-Host "請輸入目標分支 (默認: develop)"
    if ([string]::IsNullOrEmpty($targetBranch)) {
        $targetBranch = "develop"
    }
    
    Write-Host "合併 '$currentBranch' 到 '$targetBranch'" -ForegroundColor Cyan
    
    # 確認操作
    $confirm = Read-Host "確認合併? (y/N)"
    if ($confirm -ne 'y' -and $confirm -ne 'Y') {
        Write-Host "取消合併" -ForegroundColor Yellow
        return
    }
    
    try {
        git checkout $targetBranch | Out-Null
        git pull origin $targetBranch | Out-Null
        
        # 執行預合併檢查
        if (-not (Invoke-PreMergeChecks)) {
            Write-Host "預合併檢查失敗，取消合併" -ForegroundColor Red
            return
        }
        
        git merge $currentBranch --no-ff | Out-Null
        git push origin $targetBranch | Out-Null
        
        Write-Host "合併成功" -ForegroundColor Green
    } catch {
        Write-Host "合併失敗" -ForegroundColor Red
        try {
            git merge --abort | Out-Null
            Write-Host "已取消合併" -ForegroundColor Info
        } catch {
            Write-Host "無法取消合併" -ForegroundColor Warning
        }
    }
}

# 顯示分支狀態
function Show-BranchStatus {
    $currentBranch = Get-CurrentBranch
    $isClean = Test-WorkingDirectory
    
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "分支狀態" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "當前分支: $currentBranch" -ForegroundColor White
    Write-Host "工作目錄: $(if ($isClean) { '乾淨' } else { '有變更' })" -ForegroundColor White
    
    if ($currentBranch -eq "main") {
        Write-Host "分支類型: 主分支" -ForegroundColor Yellow
    } elseif ($currentBranch -eq "develop") {
        Write-Host "分支類型: 開發分支" -ForegroundColor Yellow
    } elseif ($currentBranch -like "feature/*") {
        Write-Host "分支類型: 功能分支" -ForegroundColor Cyan
    } else {
        Write-Host "分支類型: 其他" -ForegroundColor White
    }
}

# 顯示幫助
function Show-Help {
    Write-Host "用法:" -ForegroundColor Yellow
    Write-Host "  .\Basic-Branch.ps1 -Action <動作>" -ForegroundColor White
    Write-Host ""
    Write-Host "可用動作:" -ForegroundColor Yellow
    Write-Host "  status   - 顯示分支狀態" -ForegroundColor White
    Write-Host "  feature  - 創建功能分支" -ForegroundColor White
    Write-Host "  merge    - 合併到目標分支" -ForegroundColor White
    Write-Host "  checks   - 執行預合併檢查" -ForegroundColor White
    Write-Host "  help     - 顯示幫助" -ForegroundColor White
    Write-Host ""
    Write-Host "標準工作流程:" -ForegroundColor Yellow
    Write-Host "  1. 從develop創建功能分支" -ForegroundColor White
    Write-Host "  2. 在功能分支上開發" -ForegroundColor White
    Write-Host "  3. 執行預合併檢查" -ForegroundColor White
    Write-Host "  4. 合併到develop" -ForegroundColor White
    Write-Host "  5. 從develop創建發布分支" -ForegroundColor White
    Write-Host "  6. 測試發布分支" -ForegroundColor White
    Write-Host "  7. 合併到main分支" -ForegroundColor White
    Write-Host ""
    Write-Host "Main分支保護規則:" -ForegroundColor Red
    Write-Host "  - 合併到main前必須通過所有檢查" -ForegroundColor White
    Write-Host "  - 測試必須通過" -ForegroundColor White
    Write-Host "  - 構建必須成功" -ForegroundColor White
    Write-Host "  - 工作目錄必須乾淨" -ForegroundColor White
}

# 主程序
switch ($Action.ToLower()) {
    "status" {
        Show-BranchStatus
    }
    "feature" {
        New-FeatureBranch
    }
    "merge" {
        Merge-ToTarget
    }
    "checks" {
        Invoke-PreMergeChecks
    }
    "help" {
        Show-Help
    }
    default {
        Write-Host "未知動作: $Action" -ForegroundColor Red
        Write-Host "使用 'help' 查看可用動作" -ForegroundColor Yellow
    }
}

Write-Host ""
Write-Host "分支管理操作完成" -ForegroundColor Cyan
