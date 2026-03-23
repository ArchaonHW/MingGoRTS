# 至聖者指揮學批量開發工作流
# SageCommand Batch Development Workflow

param(
    [string]$ProjectPath = "F:\MingGoRTS",
    [string]$Phase = "Phase1",  # Phase1, Phase2, Phase3, Phase4
    [switch]$DryRun = $false,    # 預演模式，不實際執行
    [switch]$Verbose = $false
)

$ErrorActionPreference = "Stop"
$ProgressPreference = "Continue"

# ============================================================================
# 日誌設定
# ============================================================================
$LogFile = "$ProjectPath\BatchDevelopment_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"

function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $logEntry = "[$timestamp] [$Level] $Message"
    Write-Host $logEntry
    Add-Content -Path $LogFile -Value $logEntry
}

# ============================================================================
# Phase 1: 核心系統開發任務
# ============================================================================
$Phase1Tasks = @(
    @{ ID="SAGE-001"; Name="完善聖者角色系統"; File="MingSageCharacterSystem"; Priority="High"; EstHours=8 },
    @{ ID="SAGE-002"; Name="實現三種角色特性"; File="MingSageTraits"; Priority="High"; EstHours=16 },
    @{ ID="SAGE-003"; Name="完善魔王特性系統"; File="MingDemonTraits"; Priority="High"; EstHours=12 },
    @{ ID="SAGE-004"; Name="完善偽聖者特性系統"; File="MingFalseSageTraits"; Priority="High"; EstHours=12 },
    @{ ID="SAGE-005"; Name="完善三權指揮系統"; File="MingThreePowerSystem"; Priority="High"; EstHours=16 },
    @{ ID="SAGE-006"; Name="完善道權系統"; File="MingMoralAuthority"; Priority="Medium"; EstHours=20 },
    @{ ID="SAGE-007"; Name="完善策權系統"; File="MingStrategyAuthority"; Priority="Medium"; EstHours=20 },
    @{ ID="SAGE-008"; Name="完善兵權系統"; File="MingMilitaryAuthority"; Priority="Medium"; EstHours=20 }
)

# ============================================================================
# Phase 2: 策略系統開發任務
# ============================================================================
$Phase2Tasks = @(
    @{ ID="SAGE-012"; Name="完善六策系統框架"; File="MingSixStrategies"; Priority="Medium"; EstHours=8 },
    @{ ID="SAGE-013"; Name="實現正六策：立國"; File="MingSixStrategies"; Priority="Medium"; EstHours=12 },
    @{ ID="SAGE-014"; Name="實現正六策：立制"; File="MingSixStrategies"; Priority="Medium"; EstHours=12 },
    @{ ID="SAGE-015"; Name="實現正六策：立人"; File="MingSixStrategies"; Priority="Medium"; EstHours=12 },
    @{ ID="SAGE-016"; Name="實現逆六策：破局"; File="MingSixStrategies"; Priority="Medium"; EstHours=16 },
    @{ ID="SAGE-017"; Name="實現逆六策：破結構"; File="MingSixStrategies"; Priority="Medium"; EstHours=16 },
    @{ ID="SAGE-018"; Name="實現逆六策：不破人"; File="MingSixStrategies"; Priority="Medium"; EstHours=16 },
    @{ ID="SAGE-020"; Name="完善五行節奏系統"; File="MingWuXingRhythmSystem"; Priority="Medium"; EstHours=8 },
    @{ ID="SAGE-021"; Name="實現木火土金水五階段"; File="MingYinYangFiveElements"; Priority="Medium"; EstHours=16 }
)

# ============================================================================
# Phase 3: 進階機制開發任務
# ============================================================================
$Phase3Tasks = @(
    @{ ID="SAGE-025"; Name="完善防墮機制系統"; File="MingAntiFallSystem"; Priority="High"; EstHours=20 },
    @{ ID="SAGE-026"; Name="實現三大墮落徵象檢測"; File="MingAntiFallSystem"; Priority="High"; EstHours=16 },
    @{ ID="SAGE-027"; Name="完善智能推薦系統"; File="MingIntelligentRecommendationSystem"; Priority="Medium"; EstHours=24 },
    @{ ID="SAGE-028"; Name="完善角色成長曲線"; File="MingSageCharacter"; Priority="Medium"; EstHours=20 }
)

# ============================================================================
# Phase 4: 集成測試任務
# ============================================================================
$Phase4Tasks = @(
    @{ ID="SAGE-035"; Name="系統集成測試"; File="MingSageCommandTest"; Priority="High"; EstHours=24 },
    @{ ID="SAGE-036"; Name="數值平衡調整"; File="MingSageCommandSystem"; Priority="Medium"; EstHours=16 },
    @{ ID="SAGE-037"; Name="性能優化"; File="MingSageCommandSystem"; Priority="Medium"; EstHours=20 },
    @{ ID="SAGE-038"; Name="API文檔完善"; File="MingRTSSageCommandAPI"; Priority="Low"; EstHours=12 }
)

# ============================================================================
# 獲取當前階段任務
# ============================================================================
function Get-PhaseTasks {
    param([string]$Phase)
    switch ($Phase) {
        "Phase1" { return $Phase1Tasks }
        "Phase2" { return $Phase2Tasks }
        "Phase3" { return $Phase3Tasks }
        "Phase4" { return $Phase4Tasks }
        "All" { return $Phase1Tasks + $Phase2Tasks + $Phase3Tasks + $Phase4Tasks }
        default { return $Phase1Tasks }
    }
}

# ============================================================================
# 檢查文件編碼問題
# ============================================================================
function Test-FileEncoding {
    param([string]$FilePath)
    try {
        $content = Get-Content -Path $FilePath -Raw -Encoding UTF8 -ErrorAction SilentlyContinue
        if ($content -match "[使本成設置正軍的輸]" -or $content -match "Min成|Gene本ated|En成ine") {
            return $true  # 有編碼問題
        }
        return $false
    }
    catch {
        return $false
    }
}

# ============================================================================
# 分析文件狀態
# ============================================================================
function Get-FileStatus {
    param([string]$FileName)
    $publicPath = "$ProjectPath\Source\MingGoRTS\Public\SageCommand\$FileName.h"
    $privatePath = "$ProjectPath\Source\MingGoRTS\Private\SageCommand\$FileName.cpp"
    
    $status = @{
        PublicExists = Test-Path $publicPath
        PrivateExists = Test-Path $privatePath
        PublicEncoding = $false
        PrivateEncoding = $false
        PublicSize = 0
        PrivateSize = 0
    }
    
    if ($status.PublicExists) {
        $status.PublicEncoding = Test-FileEncoding $publicPath
        $status.PublicSize = (Get-Item $publicPath).Length
    }
    if ($status.PrivateExists) {
        $status.PrivateEncoding = Test-FileEncoding $privatePath
        $status.PrivateSize = (Get-Item $privatePath).Length
    }
    
    return $status
}

# ============================================================================
# 執行批量開發
# ============================================================================
function Start-BatchDevelopment {
    param([array]$Tasks)
    
    $totalTasks = $Tasks.Count
    $currentTask = 0
    $totalEstHours = ($Tasks | Measure-Object -Property EstHours -Sum).Sum
    
    Write-Log "開始批量開發：共 $totalTasks 個任務，預估 $totalEstHours 小時" "INFO"
    Write-Log "====================================" "INFO"
    
    $results = @()
    
    foreach ($task in $Tasks) {
        $currentTask++
        $percentComplete = [math]::Round(($currentTask / $totalTasks) * 100, 2)
        
        Write-Progress -Activity "批量開發進行中" -Status "處理任務 $($task.ID): $($task.Name)" `
            -PercentComplete $percentComplete `
            -CurrentOperation "$currentTask / $totalTasks"
        
        Write-Log "[$currentTask/$totalTasks] 處理任務: $($task.ID) - $($task.Name)" "INFO"
        
        # 檢查文件狀態
        $fileStatus = Get-FileStatus -FileName $task.File
        
        $result = @{
            TaskID = $task.ID
            TaskName = $task.Name
            FileName = $task.File
            Status = "分析完成"
            PublicFile = if ($fileStatus.PublicExists) { "存在 ($([math]::Round($fileStatus.PublicSize/1024,2)) KB)" } else { "缺失" }
            PrivateFile = if ($fileStatus.PrivateExists) { "存在 ($([math]::Round($fileStatus.PrivateSize/1024,2)) KB)" } else { "缺失" }
            EncodingIssues = if ($fileStatus.PublicEncoding -or $fileStatus.PrivateEncoding) { "需要修復" } else { "正常" }
        }
        
        if ($Verbose) {
            Write-Log "  - 頭文件: $($result.PublicFile)" "DEBUG"
            Write-Log "  - 實現文件: $($result.PrivateFile)" "DEBUG"
            Write-Log "  - 編碼狀態: $($result.EncodingIssues)" "DEBUG"
        }
        
        $results += $result
    }
    
    Write-Progress -Activity "批量開發進行中" -Completed
    
    return $results
}

# ============================================================================
# 生成報告
# ============================================================================
function Export-DevelopmentReport {
    param([array]$Results, [string]$Phase)
    
    $reportPath = "$ProjectPath\Reports\BatchDevelopment_Report_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $report = @"
# 至聖者指揮學批量開發報告

**階段**: $Phase  
**生成時間**: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')  
**總任務數**: $($Results.Count)

---

## 任務執行摘要

| 任務ID | 任務名稱 | 頭文件 | 實現文件 | 編碼狀態 | 處理狀態 |
|--------|----------|--------|----------|----------|----------|
"@
    
    foreach ($result in $Results) {
        $statusIcon = if ($result.EncodingIssues -eq "正常") { "✓" } else { "⚠" }
        $report += "`n| $($result.TaskID) | $($result.TaskName) | $($result.PublicFile) | $($result.PrivateFile) | $($result.EncodingIssues) $statusIcon | $($result.Status) |"
    }
    
    $report += "`n`n---`n`n## 詳細分析`n`n"
    
    # 統計
    $encodingIssues = $Results | Where-Object { $_.EncodingIssues -eq "需要修復" }
    $missingPublic = $Results | Where-Object { $_.PublicFile -eq "缺失" }
    $missingPrivate = $Results | Where-Object { $_.PrivateFile -eq "缺失" }
    
    $report += @"
### 統計數據

- **編碼問題**: $($encodingIssues.Count) 個任務
- **缺失頭文件**: $($missingPublic.Count) 個任務
- **缺失實現文件**: $($missingPrivate.Count) 個任務

### 需要關注的任務

"@
    
    if ($encodingIssues.Count -gt 0) {
        $report += "**編碼問題任務:**`n"
        foreach ($issue in $encodingIssues) {
            $report += "- $($issue.TaskID): $($issue.TaskName) ($($issue.FileName))`n"
        }
        $report += "`n"
    }
    
    $report | Out-File -FilePath $reportPath -Encoding UTF8
    Write-Log "報告已生成: $reportPath" "INFO"
    
    return $reportPath
}

# ============================================================================
# 主程式
# ============================================================================
Write-Log "至聖者指揮學批量開發工作流啟動" "INFO"
Write-Log "階段: $Phase" "INFO"
Write-Log "專案路徑: $ProjectPath" "INFO"
if ($DryRun) { Write-Log "【預演模式】不會實際修改檔案" "WARN" }

# 獲取任務列表
$tasks = Get-PhaseTasks -Phase $Phase

# 執行批量開發
$results = Start-BatchDevelopment -Tasks $tasks

# 生成報告
$reportPath = Export-DevelopmentReport -Results $results -Phase $Phase

Write-Log "====================================" "INFO"
Write-Log "批量開發分析完成！" "INFO"
Write-Log "已處理任務數: $($results.Count)" "INFO"
Write-Log "詳細報告: $reportPath" "INFO"

# 返回結果
return $results
