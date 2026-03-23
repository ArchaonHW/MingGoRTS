# MingGoRTS Branch Management and Merge Workflow System
# 分支管理和合併工作流程系統

param(
    [string]$Action = "status",
    [string]$BranchName = "",
    [string]$TargetBranch = "main",
    [switch]$Force = $false,
    [switch]$DryRun = $false,
    [switch]$Verbose = $false
)

# 配置
$ProjectRoot = "C:\HW\MingGoRTS"
$ConfigPath = "$ProjectRoot\Config\BranchWorkflow.json"
$LogPath = "$ProjectRoot\Logs\BranchWorkflow_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"

# 工作流程配置
$WorkflowConfig = @{
    MainBranch = "main"
    DevelopBranch = "develop"
    FeatureBranchPrefix = "feature/"
    HotfixBranchPrefix = "hotfix/"
    ReleaseBranchPrefix = "release/"
    
    # 必要的檢查步驟
    RequiredChecks = @{
        PreMerge = @(
            "CleanWorkingDirectory",
            "UpToDate", 
            "CodeQuality",
            "Tests",
            "Build",
            "SecurityScan",
            "Documentation"
        )
        PreRelease = @(
            "VersionBump",
            "Changelog",
            "TagCreation",
            "ReleaseNotes"
        )
    }
    
    # 分支保護規則
    BranchProtection = @{
        "main" = @{
            RequirePullRequest = $true
            RequireReview = $true
            RequiredReviewers = 2
            EnforceAdmins = $false
            RequireUpToDate = $true
            RequireStatusChecks = $true
            RequiredStatusChecks = @("build", "test", "code-quality", "security")
        }
        "develop" = @{
            RequirePullRequest = $true
            RequireReview = $true
            RequiredReviewers = 1
            EnforceAdmins = $false
            RequireUpToDate = $true
            RequireStatusChecks = $true
            RequiredStatusChecks = @("build", "test")
        }
    }
    
    # 自動化規則
    AutomationRules = @{
        AutoDeleteMergedBranches = $true
        AutoTagReleases = $true
        AutoUpdateDependencies = $false
        AutoDeployOnMerge = $false
    }
}

# 初始化日誌
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    Write-Host $LogEntry -ForegroundColor $(switch($Level) {"ERROR" {"Red"}; "WARNING" {"Yellow"}; "SUCCESS" {"Green"}; "DEBUG" {"Gray"}; default {"White"}})
    Add-Content -Path $LogPath -Value $LogEntry
}

# 初始化工作流程系統
function Initialize-WorkflowSystem {
    Write-Log "初始化分支管理工作流程系統..." "INFO"
    
    # 創建配置目錄
    $ConfigDir = Split-Path $ConfigPath -Parent
    if (-not (Test-Path $ConfigDir)) {
        New-Item -ItemType Directory -Path $ConfigDir -Force | Out-Null
        Write-Log "創建配置目錄: $ConfigDir" "SUCCESS"
    }
    
    # 創建日誌目錄
    $LogDir = Split-Path $LogPath -Parent
    if (-not (Test-Path $LogDir)) {
        New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
        Write-Log "創建日誌目錄: $LogDir" "SUCCESS"
    }
    
    # 保存配置
    if (-not (Test-Path $ConfigPath)) {
        $WorkflowConfig | ConvertTo-Json -Depth 10 | Set-Content $ConfigPath -Encoding UTF8
        Write-Log "創建工作流程配置: $ConfigPath" "SUCCESS"
    }
    
    # 檢查Git倉庫狀態
    if (-not (Test-Path (Join-Path $ProjectRoot ".git"))) {
        Write-Log "錯誤: 目錄不是Git倉庫: $ProjectRoot" "ERROR"
        return $false
    }
    
    Write-Log "工作流程系統初始化完成" "SUCCESS"
    return $true
}

# 獲取當前分支信息
function Get-BranchInfo {
    try {
        $CurrentBranch = git rev-parse --abbrev-ref HEAD
        $IsClean = (git status --porcelain) -eq ""
        $LastCommit = git log -1 --format="%H %s"
        $UnpushedCommits = git log --oneline origin/$CurrentBranch..HEAD 2>$null
        
        return @{
            CurrentBranch = $CurrentBranch
            IsClean = $IsClean
            LastCommit = $LastCommit
            UnpushedCommits = $UnpushedCommits
            IsMainBranch = $CurrentBranch -eq $WorkflowConfig.MainBranch
            IsDevelopBranch = $CurrentBranch -eq $WorkflowConfig.DevelopBranch
            IsFeatureBranch = $CurrentBranch -like "$($WorkflowConfig.FeatureBranchPrefix)*"
            IsHotfixBranch = $CurrentBranch -like "$($WorkflowConfig.HotfixBranchPrefix)*"
            IsReleaseBranch = $CurrentBranch -like "$($WorkflowConfig.ReleaseBranchPrefix)*"
        }
    } catch {
        Write-Log "獲取分支信息失敗: $($_.Exception.Message)" "ERROR"
        return $null
    }
}

# 檢查工作目錄是否乾淨
function Test-CleanWorkingDirectory {
    Write-Log "檢查工作目錄狀態..." "INFO"
    
    $Status = git status --porcelain
    if ($Status) {
        Write-Log "工作目錄不乾淨，有以下變更:" "WARNING"
        $Status | ForEach-Object { Write-Log "  $_" "WARNING" }
        return $false
    }
    
    Write-Log "工作目錄乾淨" "SUCCESS"
    return $true
}

# 檢查分支是否最新
function Test-UpToDate {
    param([string]$BranchName = $WorkflowConfig.MainBranch)
    
    Write-Log "檢查分支 '$BranchName' 是否最新..." "INFO"
    
    try {
        # 獲取遠端更新
        git fetch origin $BranchName | Out-Null
        
        # 檢查是否有差異
        $Behind = git rev-list --count HEAD..origin/$BranchName 2>$null
        $Ahead = git rev-list --count origin/$BranchName..HEAD 2>$null
        
        if ($Behind -gt 0) {
            Write-Log "分支 '$BranchName' 落後遠端 $Behind 個提交" "WARNING"
            return $false
        }
        
        if ($Ahead -gt 0) {
            Write-Log "分支 '$BranchName' 領先遠端 $Ahead 個提交" "INFO"
        }
        
        Write-Log "分支 '$BranchName' 已是最新" "SUCCESS"
        return $true
    } catch {
        Write-Log "檢查分支更新狀態失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 代碼質量檢查
function Test-CodeQuality {
    Write-Log "執行代碼質量檢查..." "INFO"
    
    try {
        # 檢查PowerShell腳本語法
        $PowerShellFiles = Get-ChildItem -Path $ProjectRoot -Filter "*.ps1" -Recurse
        $SyntaxErrors = 0
        
        foreach ($File in $PowerShellFiles) {
            try {
                $null = [System.Management.Automation.PSParser]::Tokenize((Get-Content $File -Raw), [ref]$null)
            } catch {
                Write-Log "語法錯誤: $($File.FullName) - $($_.Exception.Message)" "ERROR"
                $SyntaxErrors++
            }
        }
        
        if ($SyntaxErrors -gt 0) {
            Write-Log "發現 $SyntaxErrors 個語法錯誤" "ERROR"
            return $false
        }
        
        # 檢查C++代碼格式（如果存在）
        $CppFiles = Get-ChildItem -Path $ProjectRoot -Filter "*.cpp" -Recurse
        if ($CppFiles.Count -gt 0) {
            Write-Log "檢查 C++ 代碼格式..." "DEBUG"
            # 這裡可以添加clang-format或其他代碼格式檢查
        }
        
        Write-Log "代碼質量檢查通過" "SUCCESS"
        return $true
    } catch {
        Write-Log "代碼質量檢查失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 運行測試
function Test-UnitTests {
    Write-Log "運行單元測試..." "INFO"
    
    try {
        # 運行PowerShell測試
        $TestFiles = Get-ChildItem -Path $ProjectRoot -Filter "*Test*.ps1" -Recurse
        $TestResults = @()
        
        foreach ($TestFile in $TestFiles) {
            Write-Log "運行測試: $($TestFile.Name)" "DEBUG"
            
            try {
                # 簡單的測試執行（實際項目中應使用Pester等測試框架）
                $Result = & $TestFile.FullName
                $TestResults += @{
                    File = $TestFile.Name
                    Result = "PASS"
                    Message = "測試通過"
                }
                Write-Log "測試通過: $($TestFile.Name)" "SUCCESS"
            } catch {
                $TestResults += @{
                    File = $TestFile.Name
                    Result = "FAIL"
                    Message = $_.Exception.Message
                }
                Write-Log "測試失敗: $($TestFile.Name) - $($_.Exception.Message)" "ERROR"
            }
        }
        
        $FailedTests = $TestResults | Where-Object { $_.Result -eq "FAIL" }
        if ($FailedTests.Count -gt 0) {
            Write-Log "有 $($FailedTests.Count) 個測試失敗" "ERROR"
            $FailedTests | ForEach-Object { Write-Log "  $($_.File): $($_.Message)" "ERROR" }
            return $false
        }
        
        Write-Log "所有測試通過 ($($TestResults.Count) 個)" "SUCCESS"
        return $true
    } catch {
        Write-Log "測試執行失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 構建檢查
function Test-Build {
    Write-Log "執行構建檢查..." "INFO"
    
    try {
        # 檢查是否有構建腳本
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
            
            $Process = Start-Process -FilePath $BuildScript -Wait -PassThru -NoNewWindow
            if ($Process.ExitCode -eq 0) {
                Write-Log "構建成功" "SUCCESS"
                return $true
            } else {
                Write-Log "構建失敗，退出代碼: $($Process.ExitCode)" "ERROR"
                return $false
            }
        } else {
            Write-Log "未找到構建腳本，跳過構建檢查" "WARNING"
            return $true
        }
    } catch {
        Write-Log "構建檢查失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 安全掃描
function Test-SecurityScan {
    Write-Log "執行安全掃描..." "INFO"
    
    try {
        # 檢查敏感文件
        $SensitivePatterns = @(
            "*.key",
            "*.pem", 
            "*.p12",
            "secrets.json",
            ".env",
            "password*",
            "secret*",
            "token*"
        )
        
        $SecurityIssues = 0
        
        foreach ($Pattern in $SensitivePatterns) {
            $Files = Get-ChildItem -Path $ProjectRoot -Filter $Pattern -Recurse -ErrorAction SilentlyContinue
            foreach ($File in $Files) {
                # 檢查是否在.gitignore中
                $GitIgnore = Get-Content "$ProjectRoot\.gitignore" -ErrorAction SilentlyContinue
                $IsIgnored = $GitIgnore | Where-Object { $File.FullName -like (Join-Path $ProjectRoot $_) }
                
                if (-not $IsIgnored) {
                    Write-Log "安全風險: 敏感文件未忽略 - $($File.FullName)" "ERROR"
                    $SecurityIssues++
                }
            }
        }
        
        # 檢查PowerShell腳本中的硬編碼密碼
        $PowerShellFiles = Get-ChildItem -Path $ProjectRoot -Filter "*.ps1" -Recurse
        foreach ($File in $PowerShellFiles) {
            $Content = Get-Content $File -Raw
            if ($Content -match "(password|secret|token|key)\s*=\s*['\"][^'\"]+['\"]") {
                Write-Log "安全風險: 發現硬編碼密碼 - $($File.FullName)" "ERROR"
                $SecurityIssues++
            }
        }
        
        if ($SecurityIssues -gt 0) {
            Write-Log "發現 $SecurityIssues 個安全問題" "ERROR"
            return $false
        }
        
        Write-Log "安全掃描通過" "SUCCESS"
        return $true
    } catch {
        Write-Log "安全掃描失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 文檔檢查
function Test-Documentation {
    Write-Log "檢查文檔..." "INFO"
    
    try {
        # 檢查README是否存在
        $ReadmePath = "$ProjectRoot\README.md"
        if (-not (Test-Path $ReadmePath)) {
            Write-Log "缺少README.md文件" "WARNING"
        }
        
        # 檢查API文檔
        $ApiDocsPath = "$ProjectRoot\docs\api"
        if (Test-Path $ApiDocsPath) {
            $ApiDocs = Get-ChildItem -Path $ApiDocsPath -Filter "*.md" -Recurse
            Write-Log "找到 $($ApiDocs.Count) 個API文檔文件" "DEBUG"
        }
        
        # 檢查變更日誌
        $ChangelogPath = "$ProjectRoot\CHANGELOG.md"
        if (-not (Test-Path $ChangelogPath)) {
            Write-Log "缺少CHANGELOG.md文件" "WARNING"
        }
        
        Write-Log "文檔檢查完成" "SUCCESS"
        return $true
    } catch {
        Write-Log "文檔檢查失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 執行所有預合併檢查
function Invoke-PreMergeChecks {
    Write-Log "========================================" "INFO"
    Write-Log "執行預合併檢查" "INFO"
    Write-Log "========================================" "INFO"
    
    $Checks = $WorkflowConfig.RequiredChecks.PreMerge
    $Results = @()
    $Passed = 0
    $Failed = 0
    
    foreach ($Check in $Checks) {
        Write-Log "執行檢查: $Check" "INFO"
        
        $Result = switch ($Check) {
            "CleanWorkingDirectory" { Test-CleanWorkingDirectory }
            "UpToDate" { Test-UpToDate }
            "CodeQuality" { Test-CodeQuality }
            "Tests" { Test-UnitTests }
            "Build" { Test-Build }
            "SecurityScan" { Test-SecurityScan }
            "Documentation" { Test-Documentation }
            default { 
                Write-Log "未知檢查: $Check" "WARNING"
                $true
            }
        }
        
        $Results += @{
            Check = $Check
            Result = if ($Result) { "PASS" } else { "FAIL" }
            Timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
        }
        
        if ($Result) {
            $Passed++
            Write-Log "✓ $Check 通過" "SUCCESS"
        } else {
            $Failed++
            Write-Log "✗ $Check 失敗" "ERROR"
        }
        
        Write-Log ""
    }
    
    Write-Log "========================================" "INFO"
    Write-Log "檢查結果: $Passed 通過, $Failed 失敗" "INFO"
    Write-Log "========================================" "INFO"
    
    if ($Failed -gt 0) {
        Write-Log "預合併檢查失敗，無法繼續合併" "ERROR"
        return $false
    }
    
    Write-Log "所有預合併檢查通過" "SUCCESS"
    return $true
}

# 創建功能分支
function New-FeatureBranch {
    param([string]$FeatureName)
    
    if ([string]::IsNullOrEmpty($FeatureName)) {
        $FeatureName = Read-Host "請輸入功能名稱"
    }
    
    $BranchName = "$($WorkflowConfig.FeatureBranchPrefix)$FeatureName"
    
    Write-Log "創建功能分支: $BranchName" "INFO"
    
    try {
        # 確保在develop分支
        git checkout $WorkflowConfig.DevelopBranch | Out-Null
        git pull origin $WorkflowConfig.DevelopBranch | Out-Null
        
        # 創建新分支
        git checkout -b $BranchName | Out-Null
        git push -u origin $BranchName | Out-Null
        
        Write-Log "功能分支創建成功: $BranchName" "SUCCESS"
        return $true
    } catch {
        Write-Log "創建功能分支失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 創建熱修復分支
function New-HotfixBranch {
    param([string]$HotfixName)
    
    if ([string]::IsNullOrEmpty($HotfixName)) {
        $HotfixName = Read-Host "請輸入熱修復名稱"
    }
    
    $BranchName = "$($WorkflowConfig.HotfixBranchPrefix)$HotfixName"
    
    Write-Log "創建熱修復分支: $BranchName" "INFO"
    
    try {
        # 確保在main分支
        git checkout $WorkflowConfig.MainBranch | Out-Null
        git pull origin $WorkflowConfig.MainBranch | Out-Null
        
        # 創建新分支
        git checkout -b $BranchName | Out-Null
        git push -u origin $BranchName | Out-Null
        
        Write-Log "熱修復分支創建成功: $BranchName" "SUCCESS"
        return $true
    } catch {
        Write-Log "創建熱修復分支失敗: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# 合併到目標分支
function Merge-ToTarget {
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
        
        # 推送合併結果
        git push origin $TargetBranch | Out-Null
        
        Write-Log "合併成功: '$SourceBranch' -> '$TargetBranch'" "SUCCESS"
        
        # 自動刪除已合併的分支
        if ($WorkflowConfig.AutomationRules.AutoDeleteMergedBranches) {
            git branch -d $SourceBranch | Out-Null
            git push origin --delete $SourceBranch | Out-Null
            Write-Log "已刪除已合併的分支: $SourceBranch" "INFO"
        }
        
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

# 顯示分支狀態
function Show-BranchStatus {
    $BranchInfo = Get-BranchInfo
    if (-not $BranchInfo) {
        return $false
    }
    
    Write-Log "========================================" "INFO"
    Write-Log "分支狀態" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log "當前分支: $($BranchInfo.CurrentBranch)" "INFO"
    Write-Log "工作目錄: $(if ($BranchInfo.IsClean) { '乾淨' } else { '有變更' })" "INFO"
    Write-Log "最後提交: $($BranchInfo.LastCommit)" "INFO"
    
    if ($BranchInfo.UnpushedCommits) {
        Write-Log "未推送提交:" "INFO"
        $BranchInfo.UnpushedCommits | ForEach-Object { Write-Log "  $_" "INFO" }
    } else {
        Write-Log "所有提交已推送" "INFO"
    }
    
    Write-Log ""
    Write-Log "分支類型:" "INFO"
    if ($BranchInfo.IsMainBranch) { Write-Log "  主分支 (main)" "YELLOW" }
    if ($BranchInfo.IsDevelopBranch) { Write-Log "  開發分支 (develop)" "YELLOW" }
    if ($BranchInfo.IsFeatureBranch) { Write-Log "  功能分支" "CYAN" }
    if ($BranchInfo.IsHotfixBranch) { Write-Log "  熱修復分支" "RED" }
    if ($BranchInfo.IsReleaseBranch) { Write-Log "  發布分支" "GREEN" }
    
    return $true
}

# 顯示幫助
function Show-Help {
    Write-Log "========================================" "INFO"
    Write-Log "MingGoRTS 分支管理工作流程系統" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log ""
    Write-Log "用法:" "YELLOW"
    Write-Log "  .\BranchWorkflow.ps1 -Action <動作> [參數]" "WHITE"
    Write-Log ""
    Write-Log "可用動作:" "YELLOW"
    Write-Log "  status           - 顯示分支狀態" "WHITE"
    Write-Log "  feature          - 創建功能分支" "WHITE"
    Write-Log "  hotfix           - 創建熱修復分支" "WHITE"
    Write-Log "  merge            - 合併到目標分支" "WHITE"
    Write-Log "  checks           - 執行預合併檢查" "WHITE"
    Write-Log "  help             - 顯示幫助" "WHITE"
    Write-Log ""
    Write-Log "參數:" "YELLOW"
    Write-Log "  -BranchName      - 分支名稱" "WHITE"
    Write-Log "  -TargetBranch    - 目標分支 (默認: main)" "WHITE"
    Write-Log "  -Force           - 強制執行" "WHITE"
    Write-Log "  -DryRun          - 預覽模式" "WHITE"
    Write-Log "  -Verbose         - 詳細輸出" "WHITE"
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
    Write-Log "範例:" "YELLOW"
    Write-Log "  .\BranchWorkflow.ps1 -Action feature -BranchName 'new-ui-system'" "WHITE"
    Write-Log "  .\BranchWorkflow.ps1 -Action merge -TargetBranch develop" "WHITE"
    Write-Log "  .\BranchWorkflow.ps1 -Action checks" "WHITE"
}

# 主執行函數
function Main {
    # 初始化系統
    if (-not (Initialize-WorkflowSystem)) {
        exit 1
    }
    
    # 根據動作執行相應功能
    switch ($Action.ToLower()) {
        "status" {
            Show-BranchStatus
        }
        "feature" {
            New-FeatureBranch -FeatureName $BranchName
        }
        "hotfix" {
            New-HotfixBranch -HotfixName $BranchName
        }
        "merge" {
            $BranchInfo = Get-BranchInfo
            if ($BranchInfo) {
                Merge-ToTarget -SourceBranch $BranchInfo.CurrentBranch -TargetBranch $TargetBranch
            } else {
                Write-Log "無法獲取分支信息" "ERROR"
                exit 1
            }
        }
        "checks" {
            Invoke-PreMergeChecks
        }
        "help" {
            Show-Help
        }
        default {
            Write-Log "未知動作: $Action" "ERROR"
            Show-Help
            exit 1
        }
    }
}

# 執行主函數
Main
