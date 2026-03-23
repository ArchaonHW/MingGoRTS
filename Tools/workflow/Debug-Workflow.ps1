#!/usr/bin/env pwsh
#Requires -Version 7.0

<#
.SYNOPSIS
    MingGoRTS Debug Workflow System
    自動化Bug檢測與BMAD任務分配系統

.DESCRIPTION
    1. 掃描專案檢測Bug（語法錯誤、編譯錯誤等）
    2. 若有Bug，自動回報給BMAD分析
    3. 生成Debug任務清單並分配至需求單系統
    4. 標記為Debug類型任務

.PARAMETER AutoFix
    嘗試自動修復簡單的Bug

.PARAMETER DeepScan
    執行深度掃描（包括靜態分析）

.PARAMETER ReportToBMAD
    將結果回報給BMAD系統

.EXAMPLE
    .\Debug-Workflow.ps1
    執行標準Bug檢測流程

.EXAMPLE
    .\Debug-Workflow.ps1 -DeepScan -ReportToBMAD
    深度掃描並回報BMAD系統
#>

[CmdletBinding()]
param(
    [Parameter()]
    [switch]$AutoFix,
    
    [Parameter()]
    [switch]$DeepScan,
    
    [Parameter()]
    [switch]$ReportToBMAD,
    
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$TaskOutputPath = "$PWD\Tasks\Debug",
    
    [Parameter()]
    [string]$LogPath = "$PWD\Logs\DebugWorkflow.log"
)

$ErrorActionPreference = "Stop"

# 顏色定義
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
    Debug = "Magenta"
}

# Bug追蹤器
$BugTracker = @{
    SyntaxErrors = @()
    CompileErrors = @()
    StaticAnalysisIssues = @()
    Warnings = @()
    AutoFixable = @()
    TotalBugs = 0
}

function Write-DebugLog {
    param(
        [string]$Message,
        [ValidateSet("INFO", "SUCCESS", "WARNING", "ERROR", "DEBUG")]
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $Color = switch ($Level) {
        "SUCCESS" { $Colors.Success }
        "WARNING" { $Colors.Warning }
        "ERROR" { $Colors.Error }
        "DEBUG" { $Colors.Debug }
        default { $Colors.Info }
    }
    
    Write-Host "[$Timestamp] [$Level] $Message" -ForegroundColor $Color
    
    # 寫入日誌文件
    $LogDir = Split-Path -Parent $LogPath
    if (-not (Test-Path $LogDir)) {
        New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
    }
    "[$Timestamp] [$Level] $Message" | Add-Content -Path $LogPath -ErrorAction SilentlyContinue
}

function Show-DebugBanner {
    Write-Host @"

    ╔═══════════════════════════════════════════════════════════╗
    ║                                                           ║
    ║              MingGoRTS Debug Workflow System              ║
    ║                   自動化Bug檢測與修復系統                  ║
    ║                                                           ║
    ╚═══════════════════════════════════════════════════════════╝

"@ -ForegroundColor $Colors.Debug
}

# ============================================
# 階段 1: Bug 檢測
# ============================================

function Start-BugDetection {
    Write-DebugLog "==========================================" "INFO"
    Write-DebugLog "階段 1: 啟動Bug檢測" "INFO"
    Write-DebugLog "==========================================" "INFO"
    
    # 1.1 語法錯誤檢測
    Test-SyntaxErrors
    
    # 1.2 編譯錯誤檢測
    Test-CompileErrors
    
    # 1.3 靜態分析（深度掃描時）
    if ($DeepScan) {
        Start-StaticAnalysis
    }
    
    # 統計總Bug數
    $BugTracker.TotalBugs = $BugTracker.SyntaxErrors.Count + 
                           $BugTracker.CompileErrors.Count + 
                           $BugTracker.StaticAnalysisIssues.Count
    
    Write-DebugLog "Bug檢測完成 - 發現 $($BugTracker.TotalBugs) 個問題" $(if ($BugTracker.TotalBugs -gt 0) { "WARNING" } else { "SUCCESS" })
    
    return $BugTracker.TotalBugs -gt 0
}

function Test-SyntaxErrors {
    Write-DebugLog "`n[1.1] 檢測語法錯誤..." "DEBUG"
    
    # 掃描 C++ 文件
    $CppFiles = Get-ChildItem -Path "$ProjectRoot\Source" -Recurse -Filter "*.cpp" -ErrorAction SilentlyContinue
    $HeaderFiles = Get-ChildItem -Path "$ProjectRoot\Source" -Recurse -Filter "*.h" -ErrorAction SilentlyContinue
    
    Write-DebugLog "  掃描 C++ 文件: $($CppFiles.Count) 個 .cpp, $($HeaderFiles.Count) 個 .h" "INFO"
    
    # 常見語法錯誤模式
    $SyntaxPatterns = @(
        @{ Pattern = "overHide"; Description = "錯誤的 override 關鍵字"; Fix = "override" }
        @{ Pattern = "Virtuai"; Description = "錯誤的 virtual 關鍵字"; Fix = "virtual" }
        @{ Pattern = "void void"; Description = "重複的 void 關鍵字"; Fix = "void" }
        @{ Pattern = "class.*:.*public.*{[^}]"; Description = "缺少類結束標記"; Fix = "檢查類定義" }
        @{ Pattern = "\(\s*\)\s*;"; Description = "可能的空函數體"; Fix = "確認是否需要實現" }
    )
    
    foreach ($File in ($CppFiles + $HeaderFiles)) {
        $Content = Get-Content -Path $File.FullName -Raw -ErrorAction SilentlyContinue
        if (-not $Content) { continue }
        
        foreach ($Pattern in $SyntaxPatterns) {
            if ($Content -match $Pattern.Pattern) {
                $BugTracker.SyntaxErrors += @{
                    File = $File.FullName
                    Line = 0
                    Type = "Syntax"
                    Description = $Pattern.Description
                    Pattern = $Pattern.Pattern
                    SuggestedFix = $Pattern.Fix
                    Severity = "High"
                }
                Write-DebugLog "    發現語法錯誤: $($Pattern.Description) in $($File.Name)" "WARNING"
            }
        }
    }
    
    Write-DebugLog "  [✓] 語法錯誤檢測完成: $($BugTracker.SyntaxErrors.Count) 個問題" $(if ($BugTracker.SyntaxErrors.Count -gt 0) { "WARNING" } else { "SUCCESS" })
}

function Test-CompileErrors {
    Write-DebugLog "`n[1.2] 檢測編譯錯誤..." "DEBUG"
    
    # 檢查 Intermediate 目錄中的編譯日誌
    $BuildLogs = Get-ChildItem -Path "$ProjectRoot\Intermediate\Build" -Recurse -Filter "*.log" -ErrorAction SilentlyContinue | 
                 Select-Object -First 5
    
    if ($BuildLogs) {
        foreach ($Log in $BuildLogs) {
            $Content = Get-Content -Path $Log.FullName -ErrorAction SilentlyContinue | Select-Object -Last 100
            if ($Content -match "error[\s:]+|fatal error") {
                $MatchResults = $Content | Select-String -Pattern "error[\s:]+(.*)" | Select-Object -First 3
                foreach ($Match in $MatchResults) {
                    $BugTracker.CompileErrors += @{
                        File = $Log.FullName
                        Line = 0
                        Type = "Compile"
                        Description = $Match.Matches[0].Groups[1].Value
                        LogEntry = $Match.Line
                        Severity = "Critical"
                    }
                }
            }
        }
    }
    
    # 檢查 UHT 錯誤
    $UHTManifests = Get-ChildItem -Path "$ProjectRoot\Intermediate" -Recurse -Filter "*.uhtmanifest" -ErrorAction SilentlyContinue
    if (-not $UHTManifests) {
        Write-DebugLog "    警告: 未找到UHT manifest，可能需要重新編譯" "WARNING"
    }
    
    Write-DebugLog "  [✓] 編譯錯誤檢測完成: $($BugTracker.CompileErrors.Count) 個問題" $(if ($BugTracker.CompileErrors.Count -gt 0) { "ERROR" } else { "SUCCESS" })
}

function Start-StaticAnalysis {
    Write-DebugLog "`n[1.3] 執行靜態分析..." "DEBUG"
    
    # 簡單的靜態分析規則
    $AnalysisRules = @(
        @{ Pattern = "TODO|FIXME|HACK|XXX"; Description = "代碼標記需要處理"; Severity = "Low" }
        @{ Pattern = "GetWorld\(\)->SpawnActor"; Description = "未檢查SpawnActor返回值"; Severity = "Medium" }
        @{ Pattern = "NewObject<.*>\(\)"; Description = "未檢查NewObject返回值"; Severity = "Low" }
        @{ Pattern = "//.*TODO"; Description = "有TODO註釋"; Severity = "Info" }
    )
    
    $SourceFiles = Get-ChildItem -Path "$ProjectRoot\Source" -Recurse -Filter "*.cpp" -ErrorAction SilentlyContinue
    
    foreach ($File in $SourceFiles) {
        $Lines = Get-Content -Path $File.FullName -ErrorAction SilentlyContinue
        $LineNumber = 0
        
        foreach ($Line in $Lines) {
            $LineNumber++
            foreach ($Rule in $AnalysisRules) {
                if ($Line -match $Rule.Pattern) {
                    $BugTracker.StaticAnalysisIssues += @{
                        File = $File.FullName
                        Line = $LineNumber
                        Type = "StaticAnalysis"
                        Description = $Rule.Description
                        Code = $Line.Trim()
                        Severity = $Rule.Severity
                    }
                }
            }
        }
    }
    
    Write-DebugLog "  [✓] 靜態分析完成: $($BugTracker.StaticAnalysisIssues.Count) 個問題" $(if ($BugTracker.StaticAnalysisIssues.Count -gt 0) { "WARNING" } else { "SUCCESS" })
}

# ============================================
# 階段 2: 自動修復（可選）
# ============================================

function Start-AutoFix {
    if (-not $AutoFix) { return $false }
    
    Write-DebugLog "==========================================" "INFO"
    Write-DebugLog "階段 2: 嘗試自動修復" "INFO"
    Write-DebugLog "==========================================" "INFO"
    
    $FixedCount = 0
    
    foreach ($Bug in $BugTracker.SyntaxErrors) {
        if ($Bug.Description -match "overHide") {
            $Content = Get-Content -Path $Bug.File -Raw
            $NewContent = $Content -replace "overHide", "override"
            if ($NewContent -ne $Content) {
                $NewContent | Set-Content -Path $Bug.File
                $FixedCount++
                $BugTracker.AutoFixable += $Bug
                Write-DebugLog "  [✓] 自動修復: $($Bug.File) - overHide -> override" "SUCCESS"
            }
        }
        elseif ($Bug.Description -match "Virtuai") {
            $Content = Get-Content -Path $Bug.File -Raw
            $NewContent = $Content -replace "Virtuai", "virtual"
            if ($NewContent -ne $Content) {
                $NewContent | Set-Content -Path $Bug.File
                $FixedCount++
                $BugTracker.AutoFixable += $Bug
                Write-DebugLog "  [✓] 自動修復: $($Bug.File) - Virtuai -> virtual" "SUCCESS"
            }
        }
    }
    
    Write-DebugLog "自動修復完成: $FixedCount 個問題已修復" $(if ($FixedCount -gt 0) { "SUCCESS" } else { "INFO" })
    
    return $FixedCount -gt 0
}

# ============================================
# 階段 3: BMAD 任務生成
# ============================================

function New-BMADDebugTasks {
    Write-DebugLog "==========================================" "INFO"
    Write-DebugLog "階段 3: 生成BMAD Debug任務" "INFO"
    Write-DebugLog "==========================================" "INFO"
    
    $Tasks = @()
    
    # 按嚴重程度分組
    $CriticalBugs = $BugTracker.SyntaxErrors + $BugTracker.CompileErrors | Where-Object { $_.Severity -eq "Critical" }
    $HighBugs = $BugTracker.SyntaxErrors + $BugTracker.CompileErrors | Where-Object { $_.Severity -eq "High" }
    $MediumBugs = $BugTracker.StaticAnalysisIssues | Where-Object { $_.Severity -eq "Medium" }
    
    # 生成Critical級別任務
    if ($CriticalBugs.Count -gt 0) {
        $Tasks += @{
            ID = "DEBUG-001"
            Title = "修復關鍵編譯錯誤 ($($CriticalBugs.Count) 個)"
            Description = "修復導致編譯失敗的關鍵錯誤`n受影響文件: $($CriticalBugs.File -join "`n")"
            Type = "Debug"
            SubType = "CompileFix"
            Priority = "Critical"
            Severity = "Critical"
            EstimatedHours = $CriticalBugs.Count * 2
            RelatedBugs = $CriticalBugs
            Dependencies = @()
        }
        Write-DebugLog "  [✓] 創建關鍵任務: DEBUG-001" "DEBUG"
    }
    
    # 生成High級別任務
    if ($HighBugs.Count -gt 0) {
        $Tasks += @{
            ID = "DEBUG-002"
            Title = "修復語法錯誤 ($($HighBugs.Count) 個)"
            Description = "修復源代碼中的語法錯誤`n受影響文件: $($HighBugs.File -join "`n")"
            Type = "Debug"
            SubType = "SyntaxFix"
            Priority = "High"
            Severity = "High"
            EstimatedHours = $HighBugs.Count * 1.5
            RelatedBugs = $HighBugs
            Dependencies = @("DEBUG-001")
        }
        Write-DebugLog "  [✓] 創建高優先級任務: DEBUG-002" "DEBUG"
    }
    
    # 生成Medium級別任務
    if ($MediumBugs.Count -gt 0) {
        $Tasks += @{
            ID = "DEBUG-003"
            Title = "處理靜態分析警告 ($($MediumBugs.Count) 個)"
            Description = "處理代碼質量警告和潛在問題`n包括: $($MediumBugs.Description | Select-Object -Unique -First 5)"
            Type = "Debug"
            SubType = "CodeQuality"
            Priority = "Medium"
            Severity = "Medium"
            EstimatedHours = $MediumBugs.Count * 0.5
            RelatedBugs = $MediumBugs
            Dependencies = @()
        }
        Write-DebugLog "  [✓] 創建中優先級任務: DEBUG-003" "DEBUG"
    }
    
    # 生成驗證任務
    if ($Tasks.Count -gt 0) {
        $Tasks += @{
            ID = "DEBUG-004"
            Title = "驗證修復結果"
            Description = "運行完整編譯和測試驗證所有修復"
            Type = "Debug"
            SubType = "Verification"
            Priority = "High"
            Severity = "Medium"
            EstimatedHours = 2
            RelatedBugs = @()
            Dependencies = $Tasks | Where-Object { $_.ID -ne "DEBUG-004" } | ForEach-Object { $_.ID }
        }
        Write-DebugLog "  [✓] 創建驗證任務: DEBUG-004" "DEBUG"
    }
    
    Write-DebugLog "[✓] BMAD任務生成完成: $($Tasks.Count) 個任務" "SUCCESS"
    
    return $Tasks
}

# ============================================
# 階段 4: 需求單系統同步
# ============================================

function Sync-ToRequirementSystem {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-DebugLog "==========================================" "INFO"
    Write-DebugLog "階段 4: 同步至需求單系統" "INFO"
    Write-DebugLog "==========================================" "INFO"
    
    # 確保輸出目錄存在
    if (-not (Test-Path $TaskOutputPath)) {
        New-Item -ItemType Directory -Path $TaskOutputPath -Force | Out-Null
    }
    
    # 生成需求單數據
    $RequirementData = @{
        RequirementID = "REQ-BUG-$(Get-Date -Format 'yyyyMMdd')"
        Title = "MingGoRTS Debug 需求單 - $(Get-Date -Format 'yyyy-MM-dd')"
        Description = "自動化Bug檢測生成的修復需求單`n發現 $($BugTracker.TotalBugs) 個問題`n已生成 $($Tasks.Count) 個修復任務"
        Type = "Debug"
        Priority = if ($BugTracker.CompileErrors.Count -gt 0) { "Critical" } else { "High" }
        Status = "Open"
        CreatedAt = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
        Tasks = $Tasks
        BugSummary = @{
            Total = $BugTracker.TotalBugs
            SyntaxErrors = $BugTracker.SyntaxErrors.Count
            CompileErrors = $BugTracker.CompileErrors.Count
            StaticIssues = $BugTracker.StaticAnalysisIssues.Count
            AutoFixed = $BugTracker.AutoFixable.Count
        }
    }
    
    # 保存 JSON
    $JsonFile = Join-Path $TaskOutputPath "DebugRequirement_$($RequirementData.RequirementID).json"
    $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $JsonFile
    Write-DebugLog "  [✓] 需求單數據已保存: $JsonFile" "SUCCESS"
    
    # 生成 Markdown 報告
    $ReportFile = Join-Path $TaskOutputPath "DebugReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $Report = @"
# Debug 工作流報告

**執行時間**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")  
**專案路徑**: $ProjectRoot  
**掃描模式**: $(if ($DeepScan) { "深度掃描" } else { "標準掃描" })

## Bug 檢測摘要

| 類型 | 數量 | 嚴重程度 |
|------|------|----------|
| 語法錯誤 | $($BugTracker.SyntaxErrors.Count) | High |
| 編譯錯誤 | $($BugTracker.CompileErrors.Count) | Critical |
| 靜態分析問題 | $($BugTracker.StaticAnalysisIssues.Count) | Medium |
| **總計** | **$($BugTracker.TotalBugs)** | - |

$(if ($BugTracker.AutoFixable.Count -gt 0) { "`n## 自動修復`n`n已自動修復 $($BugTracker.AutoFixable.Count) 個問題`n" })

## 生成的 BMAD 任務

| 任務ID | 標題 | 優先級 | 估計工時 |
|--------|------|--------|----------|
"@

    foreach ($Task in $Tasks) {
        $Report += "| $($Task.ID) | $($Task.Title) | $($Task.Priority) | $($Task.EstimatedHours)h |`n"
    }
    
    $Report += @"

## 詳細 Bug 列表

### 語法錯誤

"@
    foreach ($Bug in $BugTracker.SyntaxErrors) {
        $Report += "- **$($Bug.File.Split('\')[-1])**: $($Bug.Description)`n"
    }
    
    $Report += @"

### 編譯錯誤

"@
    foreach ($Bug in $BugTracker.CompileErrors) {
        $Report += "- **$($Bug.File.Split('\')[-1])**: $($Bug.Description)`n"
    }
    
    $Report += @"

## 下一步操作

1. 查看詳細報告: $ReportFile
2. 使用 Visual Studio 打開專案修復錯誤
3. 運行編譯驗證修復結果
4. 重新執行 Debug Workflow 確認問題已解決

---
*由 MingGoRTS Debug Workflow System 自動生成*
"@

    $Report | Set-Content -Path $ReportFile
    Write-DebugLog "  [✓] 報告已生成: $ReportFile" "SUCCESS"
    
    # 回報給 BMAD 系統（如果需要）
    if ($ReportToBMAD) {
        Write-DebugLog "`n  回報至 BMAD 系統..." "INFO"
        
        # 創建 BMAD 工作流觸發文件
        $BMADTrigger = @{
            WorkflowType = "Debug"
            RequirementID = $RequirementData.RequirementID
            Priority = $RequirementData.Priority
            TaskCount = $Tasks.Count
            TriggerTime = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
            DataFile = $JsonFile
            ReportFile = $ReportFile
        }
        
        $BMADTriggerFile = Join-Path $TaskOutputPath "BMAD_Trigger_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
        $BMADTrigger | ConvertTo-Json | Set-Content -Path $BMADTriggerFile
        
        Write-DebugLog "  [✓] BMAD 觸發文件已創建: $BMADTriggerFile" "SUCCESS"
        Write-DebugLog "  BMAD 系統將自動處理此 Debug 需求單" "INFO"
    }
    
    return @{
        RequirementFile = $JsonFile
        ReportFile = $ReportFile
        BMADTriggerFile = if ($ReportToBMAD) { $BMADTriggerFile } else { $null }
        TotalTasks = $Tasks.Count
    }
}

# ============================================
# 主執行流程
# ============================================

Show-DebugBanner

# 階段 1: Bug 檢測
$HasBugs = Start-BugDetection

if (-not $HasBugs) {
    Write-DebugLog "`n✅ 未檢測到Bug！專案狀態良好。" "SUCCESS"
    Write-DebugLog "`n==========================================" "SUCCESS"
    exit 0
}

# 階段 2: 自動修復（如果啟用）
$Fixed = Start-AutoFix

if ($Fixed -and -not $DeepScan) {
    Write-DebugLog "`n已自動修復部分問題，建議重新運行檢測" "INFO"
}

# 階段 3: 生成 BMAD 任務
$BMADTasks = New-BMADDebugTasks

# 階段 4: 同步至需求單系統
$SyncResult = Sync-ToRequirementSystem -Tasks $BMADTasks

# 顯示最終摘要
Write-DebugLog "`n==========================================" "SUCCESS"
Write-DebugLog "Debug Workflow 執行完成" "SUCCESS"
Write-DebugLog "==========================================" "SUCCESS"

Write-DebugLog "`n📊 執行摘要:" "INFO"
Write-DebugLog "  總Bug數: $($BugTracker.TotalBugs)" "INFO"
Write-DebugLog "  語法錯誤: $($BugTracker.SyntaxErrors.Count)" "INFO"
Write-DebugLog "  編譯錯誤: $($BugTracker.CompileErrors.Count)" $(if ($BugTracker.CompileErrors.Count -gt 0) { "ERROR" } else { "INFO" })
Write-DebugLog "  自動修復: $($BugTracker.AutoFixable.Count)" $(if ($BugTracker.AutoFixable.Count -gt 0) { "SUCCESS" } else { "INFO" })
Write-DebugLog "  生成任務: $($SyncResult.TotalTasks)" "INFO"

Write-DebugLog "`n📁 輸出文件:" "INFO"
Write-DebugLog "  需求單: $($SyncResult.RequirementFile)" "INFO"
Write-DebugLog "  報告: $($SyncResult.ReportFile)" "INFO"
if ($SyncResult.BMADTriggerFile) {
    Write-DebugLog "  BMAD觸發: $($SyncResult.BMADTriggerFile)" "INFO"
}

Write-DebugLog "`n💡 建議操作:" "INFO"
Write-DebugLog "  1. 查看詳細報告了解問題詳情" "INFO"
Write-DebugLog "  2. 在 Visual Studio 中修復標記的錯誤" "INFO"
Write-DebugLog "  3. 重新運行 Debug-Workflow.ps1 驗證修復" "INFO"

exit $(if ($BugTracker.CompileErrors.Count -gt 0) { 1 } else { 0 })
