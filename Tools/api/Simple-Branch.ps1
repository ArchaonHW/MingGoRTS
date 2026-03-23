# MingGoRTS Simplified Branch Management System
# 簡化分支管理系統

param(
    [string]$Action = "status",
    [string]$BranchName = "",
    [string]$TargetBranch = "main",
    [switch]$DryRun = $false
)

# 配置
$ProjectRoot = "C:\HW\MingGoRTS"
$MainBranch = "main"
$DevelopBranch = "develop"

# 寫日誌
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    
    $Color = switch($Level) {
        "ERROR" { "Red" }
        "WARNING" { "Yellow" }
        "SUCCESS" { "Green" }
        "DEBUG" { "Gray" }
        default { "White" }
    }
    
    Write-Host $LogEntry -ForegroundColor $Color
}

# 獲取分支信息
function Get-CurrentBranch {
    try {
        return git rev-parse --abbrev-ref HEAD
    } catch {
        Write-Log "無法獲取當前分支: $($_.Exception.Message)" "ERROR"
        return $null
    }
}

# 檢查工作目錄
function Test-WorkingDirectory {
    $Status = git status --porcelain
    return ($Status -eq "")
}

# 檢查分支是否最新
function Test-BranchUpToDate {
    param([string]$Branch)
    
    try {
        git fetch origin $Branch | Out-Null
        $Behind = git rev-list --count HEAD..origin/$Branch 2>$null
        return ($Behind -eq 0)
    } catch {
        Write-Log "檢查分支更新失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 運行測試
function Test-RunTests {
    Write-Log "運行測試..." "INFO"
    
    $TestScripts = Get-ChildItem -Path $ProjectRoot -Filter "*Test*.ps1" -Recurse
    $Passed = 0
    $Failed = 0
    
    foreach ($TestScript in $TestScripts) {
        Write-Log "運行測試: $($TestScript.Name)" "DEBUG"
        
        try {
            & $TestScript.FullName
            $Passed++
            Write-Log "✓ 測試通過: $($TestScript.Name)" "SUCCESS"
        } catch {
            $Failed++
            Write-Log "✗ 測試失敗: $($TestScript.Name)" "ERROR"
        }
    }
    
    Write-Log "測試結果: $Passed 通過, $Failed 失敗" "INFO"
    return ($Failed -eq 0)
}

# 構建檢查
function Test-Build {
    Write-Log "執行構建檢查..." "INFO"
    
    $BuildScripts = @(
        "$ProjectRoot\BUILD_OPTIMIZED.cmd",
        "$ProjectRoot\COMPILE_VERIFY.cmd",
        "$ProjectRoot\Tools\build\Build.bat"
    )
    
    $BuildScript = $BuildScripts | Where-Object { Test-Path $_ } | Select-Object -First 1
    
    if ($BuildScript) {
        Write-Log "使用構建腳本: $($BuildScript)" "DEBUG"
        
        if ($DryRun) {
            Write-Log "[DRY RUN] 將執行構建腳本" "INFO"
            return $true
        }
        
        try {
            $Process = Start-Process -FilePath $BuildScript -Wait -PassThru -NoNewWindow
            if ($Process.ExitCode -eq 0) {
                Write-Log "構建成功" "SUCCESS"
                return $true
            } else {
                Write-Log "構建失敗，退出代碼: $($Process.ExitCode)" "ERROR"
                return $false
            }
        } catch {
            Write-Log "構建檢查失敗: $($_.Exception.Message)" "ERROR"
            return $false
        }
    } else {
        Write-Log "未找到構建腳本，跳過構建檢查" "WARNING"
        return $true
    }
}

# 預合併檢查
function Invoke-PreMergeChecks {
    Write-Log "========================================" "INFO"
    Write-Log "執行預合併檢查" "INFO"
    Write-Log "========================================" "INFO"
    
    $Checks = @(
        @{ Name = "工作目錄檢查"; Test = { Test-WorkingDirectory } },
        @{ Name = "測試檢查"; Test = { Test-RunTests } },
        @{ Name = "構建檢查"; Test = { Test-Build } }
    )
    
    $Passed = 0
    $Failed = 0
    
    foreach ($Check in $Checks) {
        Write-Log "執行: $($Check.Name)" "INFO"
        
        if (& $Check.Test) {
            $Passed++
            Write-Log "✓ $($Check.Name) 通過" "SUCCESS"
        } else {
            $Failed++
            Write-Log "✗ $($Check.Name) 失敗" "ERROR"
        }
        Write-Log ""
    }
    
    Write-Log "========================================" "INFO"
    Write-Log "檢查結果: $Passed 通過, $Failed 失敗" "INFO"
    Write-Log "========================================" "INFO"
    
    if ($Failed -gt 0) {
        Write-Log "預合併檢查失敗，無法合併" "ERROR"
        return $false
    }
    
    Write-Log "所有檢查通過" "SUCCESS"
    return $true
}

# 創建功能分支
function New-FeatureBranch {
    param([string]$FeatureName)
    
    $NewBranchName = "feature/$FeatureName"
    
    Write-Log "創建功能分支: $NewBranchName" "INFO"
    
    try {
        # 切換到develop分支
        git checkout $DevelopBranch | Out-Null
        git pull origin $DevelopBranch | Out-Null
        
        # 創建新分支
        git checkout -b $NewBranchName | Out-Null
        git push -u origin $NewBranchName | Out-Null
        
        Write-Log "功能分支創建成功: $NewBranchName" "SUCCESS"
        return $true
    } catch {
        Write-Log "創建功能分支失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 合併分支
function Merge-Branch {
    param([string]$SourceBranch, [string]$TargetBranch)
    
    Write-Log "合併 '$SourceBranch' 到 '$TargetBranch'" "INFO"
    
    try {
        # 切換到目標分支
        git checkout $TargetBranch | Out-Null
        git pull origin $TargetBranch | Out-Null
        
        # 執行預合併檢查
        if (-not (Invoke-PreMergeChecks)) {
            Write-Log "預合併檢查失敗，取消合併" "ERROR"
            return $false
        }
        
        if ($DryRun) {
            Write-Log "[DRY RUN] 將合併 '$SourceBranch' 到 '$TargetBranch'" "INFO"
            return $true
        }
        
        # 執行合併
        git merge $SourceBranch --no-ff | Out-Null
        git push origin $TargetBranch | Out-Null
        
        Write-Log "合併成功: '$SourceBranch' -> '$TargetBranch'" "SUCCESS"
        return $true
    } catch {
        Write-Log "合併失敗: $($_.Exception.Message)" "ERROR"
        
        # 嘗試取消合併
        try {
            git merge --abort | Out-Null
            Write-Log "已取消合併" "INFO"
        } catch {
            Write-Log "無法取消合併" "WARNING"
        }
        
        return $false
    }
}

# 顯示狀態
function Show-Status {
    $CurrentBranch = Get-CurrentBranch
    if (-not $CurrentBranch) {
        return $false
    }
    
    $IsClean = Test-WorkingDirectory
    $IsMain = ($CurrentBranch -eq $MainBranch)
    $IsDevelop = ($CurrentBranch -eq $DevelopBranch)
    $IsFeature = ($CurrentBranch -like "feature/*")
    
    Write-Log "========================================" "INFO"
    Write-Log "分支狀態" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log "當前分支: $CurrentBranch" "INFO"
    Write-Log "工作目錄: $(if ($IsClean) { '乾淨' } else { '有變更' })" "INFO"
    
    if ($IsMain) { Write-Log "分支類型: 主分支" "YELLOW" }
    elseif ($IsDevelop) { Write-Log "分支類型: 開發分支" "YELLOW" }
    elseif ($IsFeature) { Write-Log "分支類型: 功能分支" "CYAN" }
    else { Write-Log "分支類型: 其他" "WHITE" }
    
    return $true
}

# 主程序
Write-Log "========================================" "INFO"
Write-Log "MingGoRTS 分支管理系統" "INFO"
Write-Log "========================================" "INFO"

switch ($Action.ToLower()) {
    "status" {
        Show-Status
    }
    "feature" {
        if (-not $BranchName) {
            $BranchName = Read-Host "請輸入功能名稱"
        }
        New-FeatureBranch -FeatureName $BranchName
    }
    "merge" {
        $CurrentBranch = Get-CurrentBranch
        if ($CurrentBranch) {
            Merge-Branch -SourceBranch $CurrentBranch -TargetBranch $TargetBranch
        } else {
            Write-Log "無法獲取當前分支" "ERROR"
        }
    }
    "checks" {
        Invoke-PreMergeChecks
    }
    "help" {
        Write-Log "用法:" "YELLOW"
        Write-Log "  .\Simple-Branch.ps1 -Action <action> [參數]" "WHITE"
        Write-Log ""
        Write-Log "可用動作:" "YELLOW"
        Write-Log "  status   - 顯示分支狀態" "WHITE"
        Write-Log "  feature  - 創建功能分支" "WHITE"
        Write-Log "  merge    - 合併到目標分支" "WHITE"
        Write-Log "  checks   - 執行預合併檢查" "WHITE"
        Write-Log "  help     - 顯示幫助" "WHITE"
        Write-Log ""
        Write-Log "工作流程:" "YELLOW"
        Write-Log "  1. 從develop創建功能分支" "WHITE"
        Write-Log "  2. 在功能分支上開發" "WHITE"
        Write-Log "  3. 執行預合併檢查" "WHITE"
        Write-Log "  4. 合併到develop" "WHITE"
        Write-Log "  5. 從develop創建發布分支" "WHITE"
        Write-Log "  6. 測試發布分支" "WHITE"
        Write-Log "  7. 合併到main分支" "WHITE"
        Write-Log ""
        Write-Log "Main分支保護:" "RED"
        Write-Log "  - 合併到main前必須通過所有檢查" "WHITE"
        Write-Log "  - 測試必須通過" "WHITE"
        Write-Log "  - 構建必須成功" "WHITE"
        Write-Log "  - 工作目錄必須乾淨" "WHITE"
    }
    default {
        Write-Log "未知動作: $Action" "ERROR"
        Write-Log "使用 'help' 查看可用動作" "WARNING"
    }
}

Write-Log ""
Write-Log "分支管理操作完成" -ForegroundColor Cyan
