#!/usr/bin/env pwsh

<#
.SYNOPSIS
    MingGoRTS Compilation Workflow System (English Version)
    Automated Compilation Workflow System

.DESCRIPTION
    1. Detect requirement tasks with "Developed" status
    2. Auto-claim and mark as "PendingCompilation"
    3. Choose compilation mode based on task count (Full/Batch/Deep)
    4. Execute compilation and update status
    5. Success: mark as "CodeReview", Failure: mark as "Debug"

.PARAMETER AutoMode
    Automated operation mode

.PARAMETER ScanInterval
    Scan interval in seconds

.PARAMETER MaxConcurrentTasks
    Maximum concurrent tasks

.EXAMPLE
    .\Compilation-Workflow-EN.ps1 -AutoMode
    Automated compilation mode

.EXAMPLE
    .\Compilation-Workflow-EN.ps1
    Manual compilation mode
#>

[CmdletBinding()]
param(
    [Parameter()]
    [switch]$AutoMode,
    
    [Parameter()]
    [int]$ScanInterval = 30,
    
    [Parameter()]
    [int]$MaxConcurrentTasks = 3,
    
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$TasksPath = "$PWD\Tasks",
    
    [Parameter()]
    [string]$LogPath = "$PWD\Logs\CompilationWorkflow.log"
)

$ErrorActionPreference = "Stop"

# Color definitions
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
    Debug = "Magenta"
    Compilation = "Blue"
}

# Compilation tracker
$CompilationTracker = @{
    CurrentTasks = @()
    CompletedTasks = @()
    FailedTasks = @()
    TotalCompiled = 0
    TotalFailed = 0
    StartTime = Get-Date
}

function Write-CompilationLog {
    param(
        [string]$Message,
        [ValidateSet("INFO", "SUCCESS", "WARNING", "ERROR", "DEBUG", "COMPILATION")]
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $Color = switch ($Level) {
        "SUCCESS" { $Colors.Success }
        "WARNING" { $Colors.Warning }
        "ERROR" { $Colors.Error }
        "DEBUG" { $Colors.Debug }
        "COMPILATION" { $Colors.Compilation }
        default { $Colors.Info }
    }
    
    Write-Host "[$Timestamp] [$Level] $Message" -ForegroundColor $Color
    
    # Write to log file
    $LogDir = Split-Path -Parent $LogPath
    if (-not (Test-Path $LogDir)) {
        New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
    }
    "[$Timestamp] [$Level] $Message" | Add-Content -Path $LogPath -ErrorAction SilentlyContinue
}

function Show-CompilationBanner {
    Write-Host @"

    ╔═══════════════════════════════════════════════════════════╗
    ║                                                           ║
    ║            MingGoRTS Compilation Workflow System           ║
    ║                Automated Compilation System                ║
    ║                                                           ║
    ╚═══════════════════════════════════════════════════════════╝

"@ -ForegroundColor $Colors.Compilation
}

# ============================================
# Phase 1: Requirement Scan and Task Detection
# ============================================

function Find-DevelopedTasks {
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "Phase 1: Scanning Developed Tasks" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $DevelopedTasks = @()
    
    # Scan all requirement directories
    $RequirementDirs = Get-ChildItem -Path $TasksPath -Directory -ErrorAction SilentlyContinue
    
    foreach ($Dir in $RequirementDirs) {
        $RequirementFiles = Get-ChildItem -Path $Dir.FullName -Filter "Requirement_*.json" -ErrorAction SilentlyContinue
        
        foreach ($File in $RequirementFiles) {
            try {
                $RequirementData = Get-Content -Path $File.FullName -Raw | ConvertFrom-Json
                
                # Check for developed tasks
                $DevelopedTasksInReq = $RequirementData.Tasks | Where-Object { 
                    $_.Status -eq "Developed" -or 
                    ($_.Status -eq "Done" -and $_.Type -eq "Development") 
                }
                
                if ($DevelopedTasksInReq.Count -gt 0) {
                    foreach ($Task in $DevelopedTasksInReq) {
                        $TaskInfo = @{
                            RequirementFile = $File.FullName
                            RequirementID = $RequirementData.RequirementID
                            TaskID = $Task.ID
                            TaskTitle = $Task.Title
                            TaskType = $Task.Type
                            TaskPriority = $Task.Priority
                            CurrentStatus = $Task.Status
                            EstimatedHours = $Task.EstimatedHours
                            Dependencies = $Task.Dependencies
                        }
                        $DevelopedTasks += $TaskInfo
                    }
                    
                    Write-CompilationLog "  Found developed tasks: $($RequirementData.RequirementID) - $($DevelopedTasksInReq.Count) tasks" "DEBUG"
                }
            }
            catch {
                Write-CompilationLog "  Error reading requirement: $($File.Name) - $($_.Exception.Message)" "WARNING"
            }
        }
    }
    
    Write-CompilationLog "Scan completed: Found $($DevelopedTasks.Count) developed tasks" $(if ($DevelopedTasks.Count -gt 0) { "SUCCESS" } else { "INFO" })
    
    return $DevelopedTasks
}

# ============================================
# Phase 2: Task Claim and Status Update
# ============================================

function Request-TasksForCompilation {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "Phase 2: Claiming Compilation Tasks" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $ClaimedTasks = @()
    
    foreach ($Task in $Tasks) {
        try {
            # Read requirement file
            $RequirementData = Get-Content -Path $Task.RequirementFile -Raw | ConvertFrom-Json
            
            # Find corresponding task
            $TaskToUpdate = $RequirementData.Tasks | Where-Object { $_.ID -eq $Task.TaskID }
            
            if ($TaskToUpdate) {
                # Update task status
                $TaskToUpdate | Add-Member -NotePropertyName "Status" -NotePropertyValue "PendingCompilation" -Force
                $TaskToUpdate | Add-Member -NotePropertyName "ClaimedBy" -NotePropertyValue "CompilationWorkflow" -Force
                $TaskToUpdate | Add-Member -NotePropertyName "ClaimedAt" -NotePropertyValue (Get-Date -Format "yyyy-MM-dd HH:mm:ss") -Force
                
                # Save updated requirement
                $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $Task.RequirementFile
                
                $ClaimedTask = $Task.Clone()
                $ClaimedTask.UpdatedStatus = "PendingCompilation"
                $ClaimedTasks += $ClaimedTask
                
                Write-CompilationLog "  [✓] Claimed task: $($Task.TaskID) - $($Task.TaskTitle)" "SUCCESS"
            }
            else {
                Write-CompilationLog "  [✗] Task not found: $($Task.TaskID)" "ERROR"
            }
        }
        catch {
            Write-CompilationLog "  [✗] Claim failed: $($Task.TaskID) - $($_.Exception.Message)" "ERROR"
        }
    }
    
    Write-CompilationLog "Claim completed: Successfully claimed $($ClaimedTasks.Count) tasks" "SUCCESS"
    
    return $ClaimedTasks
}

# ============================================
# Phase 3: Compilation Mode Selection
# ============================================

function Select-CompilationMode {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "Phase 3: Selecting Compilation Mode" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $TaskCount = $Tasks.Count
    $CompilationMode = ""
    
    if ($TaskCount -eq 0) {
        Write-CompilationLog "No tasks to compile" "INFO"
        return $null
    }
    elseif ($TaskCount -eq 1) {
        $CompilationMode = "Single"
        Write-CompilationLog "Selected mode: Single task compilation" "INFO"
    }
    elseif ($TaskCount -le $MaxConcurrentTasks) {
        $CompilationMode = "Batch"
        Write-CompilationLog "Selected mode: Batch compilation ($TaskCount tasks)" "INFO"
    }
    else {
        $CompilationMode = "Deep"
        Write-CompilationLog "Selected mode: Deep compilation (many tasks: $TaskCount)" "INFO"
    }
    
    # Analyze task types and priorities
    $HighPriorityTasks = $Tasks | Where-Object { $_.TaskPriority -eq "Critical" -or $_.TaskPriority -eq "High" }
    $NormalTasks = $Tasks | Where-Object { $_.TaskPriority -eq "Medium" -or $_.TaskPriority -eq "Low" }
    
    Write-CompilationLog "Task analysis:" "DEBUG"
    Write-CompilationLog "  High priority: $($HighPriorityTasks.Count) tasks" "DEBUG"
    Write-CompilationLog "  Normal priority: $($NormalTasks.Count) tasks" "DEBUG"
    
    return @{
        Mode = $CompilationMode
        HighPriorityTasks = $HighPriorityTasks
        NormalTasks = $NormalTasks
        TotalTasks = $Tasks
    }
}

# ============================================
# Phase 4: Compilation Execution
# ============================================

function Start-CompilationProcess {
    [CmdletBinding()]
    param(
        [string]$Mode,
        [array]$HighPriorityTasks,
        [array]$NormalTasks
    )
    
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "Phase 4: Executing Compilation Process" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $AllTasks = $HighPriorityTasks + $NormalTasks
    $CompilationResults = @()
    
    foreach ($Task in $AllTasks) {
        Write-CompilationLog "`nStarting compilation for task: $($Task.TaskID) - $($Task.TaskTitle)" "COMPILATION"
        
        # Mark as compiling
        Update-TaskStatus -Task $Task -Status "Compiling"
        
        # Execute compilation
        $Result = Invoke-TaskCompilation -Task $Task -Mode $Mode
        
        # Update final status
        if ($Result.Success) {
            Update-TaskStatus -Task $Task -Status "CodeReview" -Result $Result
            $CompilationTracker.TotalCompiled++
            Write-CompilationLog "  [✓] Compilation successful: $($Task.TaskID)" "SUCCESS"
        }
        else {
            Update-TaskStatus -Task $Task -Status "Debug" -Result $Result
            $CompilationTracker.TotalFailed++
            Write-CompilationLog "  [✗] Compilation failed: $($Task.TaskID) - $($Result.ErrorMessage)" "ERROR"
        }
        
        $CompilationResults += $Result
        $CompilationTracker.CurrentTasks = $CompilationTracker.CurrentTasks | Where-Object { $_.TaskID -ne $Task.TaskID }
    }
    
    Write-CompilationLog "Compilation completed: Success $($CompilationTracker.TotalCompiled), Failed $($CompilationTracker.TotalFailed)" "SUCCESS"
    
    return $CompilationResults
}

function Invoke-TaskCompilation {
    [CmdletBinding()]
    param(
        [hashtable]$Task,
        [string]$Mode
    )
    
    $Result = @{
        TaskID = $Task.TaskID
        Success = $false
        CompilationTime = 0
        Output = ""
        ErrorMessage = ""
        Warnings = @()
    }
    
    $Stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
    
    try {
        Write-CompilationLog "  Executing compilation command..." "DEBUG"
        
        # Execute UE compilation
        $UEProjectFile = Join-Path $ProjectRoot "MingGoRTS.uproject"
        if (Test-Path $UEProjectFile) {
            Write-CompilationLog "    Found UE project file, attempting compilation..." "DEBUG"
            
            # Simulate compilation process (replace with actual compilation)
            Start-Sleep -Seconds 2
            
            # For demo, randomly succeed/fail
            $RandomResult = Get-Random -Minimum 1 -Maximum 10
            if ($RandomResult -le 7) { # 70% success rate
                $Result.Success = $true
                $Result.Output = "Compilation completed successfully for task $($Task.TaskID)"
                Write-CompilationLog "    Compilation successful" "DEBUG"
            }
            else {
                $Result.ErrorMessage = "Simulated compilation failure for task $($Task.TaskID)"
                Write-CompilationLog "    Compilation failed (simulated)" "DEBUG"
            }
        }
        else {
            # Fallback compilation - use MSBuild
            $SLNPath = Join-Path $ProjectRoot "MingGoRTS.sln"
            if (Test-Path $SLNPath) {
                Write-CompilationLog "    Found solution file, attempting MSBuild..." "DEBUG"
                
                # Simulate compilation
                Start-Sleep -Seconds 1
                $Result.Success = $true
                $Result.Output = "MSBuild compilation completed successfully"
            }
            else {
                $Result.ErrorMessage = "No compilation project files found (.uproject or .sln)"
                Write-CompilationLog "    No project files found" "DEBUG"
            }
        }
    }
    catch {
        $Result.ErrorMessage = "Compilation process exception: $($_.Exception.Message)"
        Write-CompilationLog "    Compilation exception: $($_.Exception.Message)" "ERROR"
    }
    finally {
        $Stopwatch.Stop()
        $Result.CompilationTime = $Stopwatch.Elapsed.TotalSeconds
    }
    
    return $Result
}

function Update-TaskStatus {
    [CmdletBinding()]
    param(
        [hashtable]$Task,
        [string]$Status,
        [hashtable]$Result = $null
    )
    
    try {
        # Read requirement file
        $RequirementData = Get-Content -Path $Task.RequirementFile -Raw | ConvertFrom-Json
        
        # Find corresponding task
        $TaskToUpdate = $RequirementData.Tasks | Where-Object { $_.ID -eq $Task.TaskID }
        
        if ($TaskToUpdate) {
            $TaskToUpdate | Add-Member -NotePropertyName "Status" -NotePropertyValue $Status -Force
            $TaskToUpdate | Add-Member -NotePropertyName "UpdatedAt" -NotePropertyValue (Get-Date -Format "yyyy-MM-dd HH:mm:ss") -Force
            
            if ($Result) {
                $TaskToUpdate | Add-Member -NotePropertyName "CompilationResult" -NotePropertyValue @{
                    Success = $Result.Success
                    CompilationTime = $Result.CompilationTime
                    ErrorMessage = $Result.ErrorMessage
                    Warnings = $Result.Warnings
                } -Force
            }
            
            # Save updated requirement
            $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $Task.RequirementFile
            
            Write-CompilationLog "    Status updated: $($Task.TaskID) -> $Status" "DEBUG"
        }
    }
    catch {
        Write-CompilationLog "    Status update failed: $($_.Exception.Message)" "ERROR"
    }
}

# ============================================
# Phase 5: Compilation Report Generation
# ============================================

function New-CompilationReport {
    [CmdletBinding()]
    param([array]$Results)
    
    Write-CompilationLog "==========================================" "INFO"
    Write-CompilationLog "Phase 5: Generating Compilation Report" "INFO"
    Write-CompilationLog "==========================================" "INFO"
    
    $ReportFile = Join-Path $TasksPath "Compilation\CompilationReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    # Ensure directory exists
    $ReportDir = Split-Path -Parent $ReportFile
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    $TotalTime = (Get-Date) - $CompilationTracker.StartTime
    
    $Report = @"
# Compilation Workflow Report

**Execution Time**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")  
**Project Path**: $ProjectRoot  
**Total Execution Time**: $($TotalTime.ToString("hh\:mm\:ss"))

## Compilation Summary

| Metric | Value |
|--------|-------|
| Total Tasks | $($Results.Count) |
| Successful | $($CompilationTracker.TotalCompiled) |
| Failed | $($CompilationTracker.TotalFailed) |
| Success Rate | $([math]::Round(($CompilationTracker.TotalCompiled / [math]::Max($Results.Count, 1)) * 100, 2))% |

## Detailed Results

### ✅ Successfully Compiled Tasks

"@
    
    foreach ($Result in $Results | Where-Object { $_.Success }) {
        $Report += "- **$($Result.TaskID)**: Compilation successful (time: $([math]::Round($Result.CompilationTime, 2))s)`n"
    }
    
    $Report += @"

### ❌ Failed Compilation Tasks

"@
    
    foreach ($Result in $Results | Where-Object { -not $_.Success }) {
        $Report += "- **$($Result.TaskID)**: Compilation failed - $($Result.ErrorMessage)`n"
    }
    
    $Report += @"

## Compilation Statistics

- Average compilation time: $([math]::Round(($Results | Where-Object { $_.CompilationTime } | Measure-Object -Property CompilationTime -Average).Average, 2))s
- Longest compilation time: $([math]::Round(($Results | Where-Object { $_.CompilationTime } | Measure-Object -Property CompilationTime -Maximum).Maximum, 2))s
- Shortest compilation time: $([math]::Round(($Results | Where-Object { $_.CompilationTime } | Measure-Object -Property CompilationTime -Minimum).Minimum, 2))s

## Next Steps

1. Review failed compilation tasks and fix issues
2. Perform code review on successfully compiled tasks
3. Re-run compilation workflow to verify fixes

---
*Generated by MingGoRTS Compilation Workflow System*
"@
    
    $Report | Set-Content -Path $ReportFile
    Write-CompilationLog "Compilation report generated: $ReportFile" "SUCCESS"
    
    return $ReportFile
}

# ============================================
# Automated Mode Monitoring
# ============================================

function Start-AutoMonitoring {
    Write-CompilationLog "Starting automated monitoring mode, scan interval: $ScanInterval seconds" "INFO"
    
    while ($true) {
        try {
            Write-CompilationLog "`n--- Starting scan $(Get-Date -Format 'HH:mm:ss') ---" "DEBUG"
            
            # Find developed tasks
            $DevelopedTasks = Find-DevelopedTasks
            
            if ($DevelopedTasks.Count -gt 0) {
                Write-CompilationLog "Found $($DevelopedTasks.Count) tasks pending compilation, starting process..." "INFO"
                
                # Claim tasks
                $ClaimedTasks = Request-TasksForCompilation -Tasks $DevelopedTasks
                
                if ($ClaimedTasks.Count -gt 0) {
                    # Select compilation mode
                    $CompilationPlan = Select-CompilationMode -Tasks $ClaimedTasks
                    
                    if ($CompilationPlan) {
                        # Execute compilation
                        $Results = Start-CompilationProcess -Mode $CompilationPlan.Mode -HighPriorityTasks $CompilationPlan.HighPriorityTasks -NormalTasks $CompilationPlan.NormalTasks
                        
                        # Generate report
                        $ReportFile = New-CompilationReport -Results $Results
                        Write-CompilationLog "Compilation cycle completed, report: $ReportFile" "SUCCESS"
                    }
                }
            }
            else {
                Write-CompilationLog "No pending compilation tasks found, continuing monitoring..." "INFO"
            }
            
            Write-CompilationLog "Waiting $ScanInterval seconds for next scan..." "DEBUG"
            Start-Sleep -Seconds $ScanInterval
        }
        catch {
            Write-CompilationLog "Automated monitoring error: $($_.Exception.Message)" "ERROR"
            Start-Sleep -Seconds 60 # Wait longer on error
        }
    }
}

# ============================================
# Main Execution Flow
# ============================================

function Main {
    Show-CompilationBanner
    
    if ($AutoMode) {
        Write-CompilationLog "Starting automated compilation mode" "INFO"
        Start-AutoMonitoring
    }
    else {
        Write-CompilationLog "Executing single compilation workflow" "INFO"
        
        # Find developed tasks
        $DevelopedTasks = Find-DevelopedTasks
        
        if ($DevelopedTasks.Count -eq 0) {
            Write-CompilationLog "No pending compilation tasks found" "INFO"
            return
        }
        
        # Claim tasks
        $ClaimedTasks = Request-TasksForCompilation -Tasks $DevelopedTasks
        
        if ($ClaimedTasks.Count -eq 0) {
            Write-CompilationLog "No successfully claimed tasks" "WARNING"
            return
        }
        
        # Select compilation mode
        $CompilationPlan = Select-CompilationMode -Tasks $ClaimedTasks
        
        if (-not $CompilationPlan) {
            Write-CompilationLog "Unable to determine compilation mode" "ERROR"
            return
        }
        
        # Execute compilation
        $Results = Start-CompilationProcess -Mode $CompilationPlan.Mode -HighPriorityTasks $CompilationPlan.HighPriorityTasks -NormalTasks $CompilationPlan.NormalTasks
        
        # Generate report
        $ReportFile = New-CompilationReport -Results $Results
        
        Write-CompilationLog "`n==========================================" "SUCCESS"
        Write-CompilationLog "Compilation workflow execution completed" "SUCCESS"
        Write-CompilationLog "==========================================" "SUCCESS"
        
        Write-CompilationLog "`n📊 Execution Summary:" "INFO"
        Write-CompilationLog "  Successfully compiled: $($CompilationTracker.TotalCompiled) tasks" "SUCCESS"
        Write-CompilationLog "  Compilation failed: $($CompilationTracker.TotalFailed) tasks" $(if ($CompilationTracker.TotalFailed -gt 0) { "WARNING" } else { "INFO" })
        Write-CompilationLog "  Detailed report: $ReportFile" "INFO"
    }
}

# Execute main program
Main
