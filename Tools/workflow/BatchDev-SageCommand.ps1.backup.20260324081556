# SageCommand Batch Development Workflow
param(
    [string]$ProjectPath = "F:\MingGoRTS",
    [string]$Phase = "Phase1",
    [switch]$DryRun = $false,
    [switch]$Verbose = $false
)

$ErrorActionPreference = "Stop"
$LogFile = "$ProjectPath\BatchDevelopment_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"

function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $logEntry = "[$timestamp] [$Level] $Message"
    Write-Host $logEntry
    Add-Content -Path $LogFile -Value $logEntry
}

# Phase 1 Tasks
$Phase1Tasks = @(
    @{ ID="SAGE-001"; Name="SageCharacterSystem"; File="MingSageCharacterSystem"; Priority="High"; EstHours=8 },
    @{ ID="SAGE-002"; Name="SageTraits"; File="MingSageTraits"; Priority="High"; EstHours=16 },
    @{ ID="SAGE-003"; Name="DemonTraits"; File="MingDemonTraits"; Priority="High"; EstHours=12 },
    @{ ID="SAGE-004"; Name="FalseSageTraits"; File="MingFalseSageTraits"; Priority="High"; EstHours=12 },
    @{ ID="SAGE-005"; Name="ThreePowerSystem"; File="MingThreePowerSystem"; Priority="High"; EstHours=16 },
    @{ ID="SAGE-006"; Name="MoralAuthority"; File="MingMoralAuthority"; Priority="Medium"; EstHours=20 },
    @{ ID="SAGE-007"; Name="StrategyAuthority"; File="MingStrategyAuthority"; Priority="Medium"; EstHours=20 },
    @{ ID="SAGE-008"; Name="MilitaryAuthority"; File="MingMilitaryAuthority"; Priority="Medium"; EstHours=20 }
)

# Phase 2 Tasks  
$Phase2Tasks = @(
    @{ ID="SAGE-012"; Name="SixStrategies"; File="MingSixStrategies"; Priority="Medium"; EstHours=8 },
    @{ ID="SAGE-020"; Name="WuXingRhythmSystem"; File="MingWuXingRhythmSystem"; Priority="Medium"; EstHours=8 },
    @{ ID="SAGE-021"; Name="YinYangFiveElements"; File="MingYinYangFiveElements"; Priority="Medium"; EstHours=16 }
)

# Phase 3 Tasks
$Phase3Tasks = @(
    @{ ID="SAGE-025"; Name="AntiFallSystem"; File="MingAntiFallSystem"; Priority="High"; EstHours=20 },
    @{ ID="SAGE-027"; Name="IntelligentRecommendation"; File="MingIntelligentRecommendationSystem"; Priority="Medium"; EstHours=24 }
)

function Get-PhaseTasks {
    param([string]$Phase)
    switch ($Phase) {
        "Phase1" { return $Phase1Tasks }
        "Phase2" { return $Phase2Tasks }
        "Phase3" { return $Phase3Tasks }
        "All" { return $Phase1Tasks + $Phase2Tasks + $Phase3Tasks }
        default { return $Phase1Tasks }
    }
}

function Test-FileEncoding {
    param([string]$FilePath)
    try {
        $bytes = [System.IO.File]::ReadAllBytes($FilePath)
        $content = [System.Text.Encoding]::UTF8.GetString($bytes)
        # Check for common corrupted Chinese characters
        $corrupted = @(
            "Min\u6210",  # Min成
            "Gene\u672cated",  # Gene本ated
            "En\u6212ine",  # En成ine
            "\u672cet\u4f7fn",  # 本et使n
            "\u6b63oid",  # 正oid
            "\u8ecdSt\u672cin\u6210"  # 軍St本in成
        )
        foreach ($pattern in $corrupted) {
            if ($content -match $pattern) {
                return $true
            }
        }
        return $false
    }
    catch {
        return $false
    }
}

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

function Start-BatchDevelopment {
    param([array]$Tasks)
    
    $totalTasks = $Tasks.Count
    $currentTask = 0
    $totalEstHours = ($Tasks | Measure-Object -Property EstHours -Sum).Sum
    
    Write-Log "Starting batch development: $totalTasks tasks, estimated $totalEstHours hours" "INFO"
    Write-Log "====================================" "INFO"
    
    $results = @()
    
    foreach ($task in $Tasks) {
        $currentTask++
        $percentComplete = [math]::Round(($currentTask / $totalTasks) * 100, 2)
        
        Write-Progress -Activity "Batch Development" -Status "Processing $($task.ID): $($task.Name)" `
            -PercentComplete $percentComplete `
            -CurrentOperation "$currentTask / $totalTasks"
        
        Write-Log "[$currentTask/$totalTasks] Processing: $($task.ID) - $($task.Name)" "INFO"
        
        $fileStatus = Get-FileStatus -FileName $task.File
        
        $result = @{
            TaskID = $task.ID
            TaskName = $task.Name
            FileName = $task.File
            Status = "Analyzed"
            PublicFile = if ($fileStatus.PublicExists) { "Exists ($([math]::Round($fileStatus.PublicSize/1024,2)) KB)" } else { "Missing" }
            PrivateFile = if ($fileStatus.PrivateExists) { "Exists ($([math]::Round($fileStatus.PrivateSize/1024,2)) KB)" } else { "Missing" }
            EncodingIssues = if ($fileStatus.PublicEncoding -or $fileStatus.PrivateEncoding) { "Need Fix" } else { "OK" }
        }
        
        if ($Verbose) {
            Write-Log "  - Header: $($result.PublicFile)" "DEBUG"
            Write-Log "  - Implementation: $($result.PrivateFile)" "DEBUG"
            Write-Log "  - Encoding: $($result.EncodingIssues)" "DEBUG"
        }
        
        $results += $result
    }
    
    Write-Progress -Activity "Batch Development" -Completed
    
    return $results
}

function Export-DevelopmentReport {
    param([array]$Results, [string]$Phase)
    
    $reportPath = "$ProjectPath\Reports\BatchDevelopment_Report_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $report = @"
# SageCommand Batch Development Report

**Phase**: $Phase  
**Generated**: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')  
**Total Tasks**: $($Results.Count)

---

## Task Summary

| Task ID | Task Name | Header | Implementation | Encoding | Status |
|---------|-----------|--------|----------------|----------|--------|
"@
    
    foreach ($result in $Results) {
        $statusIcon = if ($result.EncodingIssues -eq "OK") { "OK" } else { "WARN" }
        $report += "`n| $($result.TaskID) | $($result.TaskName) | $($result.PublicFile) | $($result.PrivateFile) | $($result.EncodingIssues) $statusIcon | $($result.Status) |"
    }
    
    $report += "`n`n---`n`n## Statistics`n`n"
    
    $encodingIssues = $Results | Where-Object { $_.EncodingIssues -eq "Need Fix" }
    $missingPublic = $Results | Where-Object { $_.PublicFile -eq "Missing" }
    $missingPrivate = $Results | Where-Object { $_.PrivateFile -eq "Missing" }
    
    $report += @"
- **Encoding Issues**: $($encodingIssues.Count) tasks
- **Missing Headers**: $($missingPublic.Count) tasks  
- **Missing Implementation**: $($missingPrivate.Count) tasks

### Tasks Needing Attention

"@
    
    if ($encodingIssues.Count -gt 0) {
        $report += "**Encoding Issues:**`n"
        foreach ($issue in $encodingIssues) {
            $report += "- $($issue.TaskID): $($issue.TaskName) ($($issue.FileName))`n"
        }
        $report += "`n"
    }
    
    $report | Out-File -FilePath $reportPath -Encoding UTF8
    Write-Log "Report generated: $reportPath" "INFO"
    
    return $reportPath
}

# Main
Write-Log "SageCommand Batch Development Workflow Started" "INFO"
Write-Log "Phase: $Phase" "INFO"
Write-Log "Project: $ProjectPath" "INFO"
if ($DryRun) { Write-Log "[DRY RUN] No actual changes will be made" "WARN" }

$tasks = Get-PhaseTasks -Phase $Phase
$results = Start-BatchDevelopment -Tasks $tasks
$reportPath = Export-DevelopmentReport -Results $results -Phase $Phase

Write-Log "====================================" "INFO"
Write-Log "Batch development analysis complete!" "INFO"
Write-Log "Tasks processed: $($results.Count)" "INFO"
Write-Log "Report: $reportPath" "INFO"

return $results
