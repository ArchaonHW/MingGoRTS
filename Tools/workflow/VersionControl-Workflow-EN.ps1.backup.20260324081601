#!/usr/bin/env pwsh

<#
.SYNOPSIS
    MingGoRTS Version Control Workflow System (English Version)
    Version Control Workflow System

.DESCRIPTION
    1. Detect requirement tasks with "Done" status
    2. Start intelligent commit process
    3. Success: mark as "Completed", Failure: mark as "Debug" and return to "Uncommitted"
    4. Enable automated control when necessary

.PARAMETER AutoMode
    Automated operation mode

.PARAMETER ScanInterval
    Scan interval in seconds

.PARAMETER MaxConcurrentCommits
    Maximum concurrent commits

.EXAMPLE
    .\VersionControl-Workflow-EN.ps1 -AutoMode
    Automated version control mode

.EXAMPLE
    .\VersionControl-Workflow-EN.ps1
    Manual version control mode
#>

[CmdletBinding()]
param(
    [Parameter()]
    [switch]$AutoMode,
    
    [Parameter()]
    [int]$ScanInterval = 45,
    
    [Parameter()]
    [int]$MaxConcurrentCommits = 2,
    
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$TasksPath = "$PWD\Tasks",
    
    [Parameter()]
    [string]$LogPath = "$PWD\Logs\VersionControlWorkflow.log"
)

$ErrorActionPreference = "Stop"

# Color definitions
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
    Debug = "Magenta"
    VersionControl = "Blue"
}

# Version control tracker
$VersionControlTracker = @{
    CurrentTasks = @()
    CompletedTasks = @()
    FailedTasks = @()
    TotalCommitted = 0
    TotalFailed = 0
    StartTime = Get-Date
}

function Write-VersionControlLog {
    param(
        [string]$Message,
        [ValidateSet("INFO", "SUCCESS", "WARNING", "ERROR", "DEBUG", "VERSIONCONTROL")]
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $Color = switch ($Level) {
        "SUCCESS" { $Colors.Success }
        "WARNING" { $Colors.Warning }
        "ERROR" { $Colors.Error }
        "DEBUG" { $Colors.Debug }
        "VERSIONCONTROL" { $Colors.VersionControl }
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

function Show-VersionControlBanner {
    Write-Host @"

    ╔═══════════════════════════════════════════════════════════╗
    ║                                                           ║
    ║          MingGoRTS Version Control Workflow System       ║
    ║              Version Control System                        ║
    ║                                                           ║
    ╚═══════════════════════════════════════════════════════════╝

"@ -ForegroundColor $Colors.VersionControl
}

# ============================================
# Phase 1: Requirement Scan and Task Detection
# ============================================

function Find-DoneTasks {
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "Phase 1: Scanning Completed Tasks" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $DoneTasks = @()
    
    # Scan all requirement directories
    $RequirementDirs = Get-ChildItem -Path $TasksPath -Directory -ErrorAction SilentlyContinue
    
    foreach ($Dir in $RequirementDirs) {
        $RequirementFiles = Get-ChildItem -Path $Dir.FullName -Filter "Requirement_*.json" -ErrorAction SilentlyContinue
        
        foreach ($File in $RequirementFiles) {
            try {
                $RequirementData = Get-Content -Path $File.FullName -Raw | ConvertFrom-Json
                
                # Check for tasks with "Done" status
                $DoneTasksInReq = $RequirementData.Tasks | Where-Object { 
                    $_.Status -eq "Done" -or 
                    $_.Status -eq "CodeReview" -or
                    $_.Status -eq "Completed"
                }
                
                if ($DoneTasksInReq.Count -gt 0) {
                    foreach ($Task in $DoneTasksInReq) {
                        # Check if already processed
                        if ($Task.Committed -ne $true) {
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
                                Changes = $Task.Changes
                                Files = $Task.Files
                            }
                            $DoneTasks += $TaskInfo
                        }
                    }
                    
                    Write-VersionControlLog "  Found completed tasks: $($RequirementData.RequirementID) - $($DoneTasksInReq.Count) tasks" "DEBUG"
                }
            }
            catch {
                Write-VersionControlLog "  Error reading requirement: $($File.Name) - $($_.Exception.Message)" "WARNING"
            }
        }
    }
    
    Write-VersionControlLog "Scan completed: Found $($DoneTasks.Count) tasks pending commit" $(if ($DoneTasks.Count -gt 0) { "SUCCESS" } else { "INFO" })
    
    return $DoneTasks
}

# ============================================
# Phase 2: Task Claim and Status Update
# ============================================

function Request-TasksForCommit {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "Phase 2: Claiming Commit Tasks" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $ClaimedTasks = @()
    
    foreach ($Task in $Tasks) {
        try {
            # Read requirement file
            $RequirementData = Get-Content -Path $Task.RequirementFile -Raw | ConvertFrom-Json
            
            # Find corresponding task
            $TaskToUpdate = $RequirementData.Tasks | Where-Object { $_.ID -eq $Task.TaskID }
            
            if ($TaskToUpdate) {
                # Update task status
                $TaskToUpdate | Add-Member -NotePropertyName "Status" -NotePropertyValue "Committing" -Force
                $TaskToUpdate | Add-Member -NotePropertyName "ClaimedBy" -NotePropertyValue "VersionControlWorkflow" -Force
                $TaskToUpdate | Add-Member -NotePropertyName "ClaimedAt" -NotePropertyValue (Get-Date -Format "yyyy-MM-dd HH:mm:ss") -Force
                
                # Save updated requirement
                $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $Task.RequirementFile
                
                $ClaimedTask = $Task.Clone()
                $ClaimedTask.UpdatedStatus = "Committing"
                $ClaimedTasks += $ClaimedTask
                
                Write-VersionControlLog "  [✓] Claimed task: $($Task.TaskID) - $($Task.TaskTitle)" "SUCCESS"
            }
            else {
                Write-VersionControlLog "  [✗] Task not found: $($Task.TaskID)" "ERROR"
            }
        }
        catch {
            Write-VersionControlLog "  [✗] Claim failed: $($Task.TaskID) - $($_.Exception.Message)" "ERROR"
        }
    }
    
    Write-VersionControlLog "Claim completed: Successfully claimed $($ClaimedTasks.Count) tasks" "SUCCESS"
    
    return $ClaimedTasks
}

# ============================================
# Phase 3: Version Control Mode Selection
# ============================================

function Select-VersionControlMode {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "Phase 3: Selecting Version Control Mode" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $TaskCount = $Tasks.Count
    $VersionControlMode = ""
    
    if ($TaskCount -eq 0) {
        Write-VersionControlLog "No tasks to commit" "INFO"
        return $null
    }
    elseif ($TaskCount -eq 1) {
        $VersionControlMode = "Single"
        Write-VersionControlLog "Selected mode: Single task commit" "INFO"
    }
    elseif ($TaskCount -le $MaxConcurrentCommits) {
        $VersionControlMode = "Batch"
        Write-VersionControlLog "Selected mode: Batch commit ($TaskCount tasks)" "INFO"
    }
    else {
        $VersionControlMode = "Sequential"
        Write-VersionControlLog "Selected mode: Sequential commit (many tasks: $TaskCount)" "INFO"
    }
    
    # Analyze task types and priorities
    $HighPriorityTasks = $Tasks | Where-Object { $_.TaskPriority -eq "Critical" -or $_.TaskPriority -eq "High" }
    $NormalTasks = $Tasks | Where-Object { $_.TaskPriority -eq "Medium" -or $_.TaskPriority -eq "Low" }
    
    Write-VersionControlLog "Task analysis:" "DEBUG"
    Write-VersionControlLog "  High priority: $($HighPriorityTasks.Count) tasks" "DEBUG"
    Write-VersionControlLog "  Normal priority: $($NormalTasks.Count) tasks" "DEBUG"
    
    return @{
        Mode = $VersionControlMode
        HighPriorityTasks = $HighPriorityTasks
        NormalTasks = $NormalTasks
        TotalTasks = $Tasks
    }
}

# ============================================
# Phase 4: Version Control Execution
# ============================================

function Start-VersionControlProcess {
    [CmdletBinding()]
    param(
        [string]$Mode,
        [array]$HighPriorityTasks,
        [array]$NormalTasks
    )
    
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "Phase 4: Executing Version Control Process" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $AllTasks = $HighPriorityTasks + $NormalTasks
    $VersionControlResults = @()
    
    foreach ($Task in $AllTasks) {
        Write-VersionControlLog "`nStarting version control for task: $($Task.TaskID) - $($Task.TaskTitle)" "VERSIONCONTROL"
        
        # Mark as committing
        Update-TaskStatus -Task $Task -Status "Committing"
        
        # Execute version control
        $Result = Invoke-TaskVersionControl -Task $Task -Mode $Mode
        
        # Update final status
        if ($Result.Success) {
            Update-TaskStatus -Task $Task -Status "Completed" -Result $Result
            $VersionControlTracker.TotalCommitted++
            Write-VersionControlLog "  [✓] Version control successful: $($Task.TaskID)" "SUCCESS"
        }
        else {
            Update-TaskStatus -Task $Task -Status "Debug" -Result $Result
            # Return to uncommitted status
            Update-TaskStatus -Task $Task -Status "Uncommitted"
            $VersionControlTracker.TotalFailed++
            Write-VersionControlLog "  [✗] Version control failed: $($Task.TaskID) - $($Result.ErrorMessage)" "ERROR"
        }
        
        $VersionControlResults += $Result
        $VersionControlTracker.CurrentTasks = $VersionControlTracker.CurrentTasks | Where-Object { $_.TaskID -ne $Task.TaskID }
    }
    
    Write-VersionControlLog "Version control completed: Success $($VersionControlTracker.TotalCommitted), Failed $($VersionControlTracker.TotalFailed)" "SUCCESS"
    
    return $VersionControlResults
}

function Invoke-TaskVersionControl {
    [CmdletBinding()]
    param(
        [hashtable]$Task,
        [string]$Mode
    )
    
    $Result = @{
        TaskID = $Task.TaskID
        Success = $false
        CommitTime = 0
        Output = ""
        ErrorMessage = ""
        Warnings = @()
        CommitHash = ""
        Branch = ""
    }
    
    $Stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
    
    try {
        Write-VersionControlLog "  Executing version control commands..." "DEBUG"
        
        # Check Git repository status
        $GitRepoPath = Join-Path $ProjectRoot ".git"
        if (Test-Path $GitRepoPath) {
            Write-VersionControlLog "    Git repository detected, starting intelligent commit..." "DEBUG"
            
            # Get current branch
            $CurrentBranch = & git -C $ProjectRoot rev-parse --abbrev-ref HEAD 2>$null
            if ($LASTEXITCODE -eq 0) {
                $Result.Branch = $CurrentBranch
                Write-VersionControlLog "    Current branch: $CurrentBranch" "DEBUG"
            }
            
            # Check for uncommitted changes
            $StatusOutput = & git -C $ProjectRoot status --porcelain 2>$null
            if ($LASTEXITCODE -eq 0 -and $StatusOutput) {
                Write-VersionControlLog "    Uncommitted changes detected, starting commit..." "DEBUG"
                
                # Add all changes
                & git -C $ProjectRoot add . 2>$null
                if ($LASTEXITCODE -ne 0) {
                    throw "Git add failed"
                }
                
                # Create intelligent commit message
                $CommitMessage = "feat: $($Task.TaskTitle) [$($Task.TaskID)]"
                if ($Task.Changes) {
                    $CommitMessage += "`n`n$($Task.Changes -join "`n")"
                }
                
                # Commit changes
                $CommitOutput = & git -C $ProjectRoot commit -m $CommitMessage 2>&1
                if ($LASTEXITCODE -eq 0) {
                    # Get commit hash
                    $CommitHash = & git -C $ProjectRoot rev-parse --short HEAD 2>$null
                    if ($LASTEXITCODE -eq 0) {
                        $Result.CommitHash = $CommitHash
                        $Result.Success = $true
                        $Result.Output = "Successfully committed: $CommitHash"
                        Write-VersionControlLog "    Commit successful: $CommitHash" "DEBUG"
                    }
                }
                else {
                    $Result.ErrorMessage = "Git commit failed: $CommitOutput"
                    Write-VersionControlLog "    Commit failed: $CommitOutput" "DEBUG"
                }
            }
            else {
                Write-VersionControlLog "    No uncommitted changes" "DEBUG"
                $Result.Success = $true
                $Result.Output = "No changes to commit"
            }
        }
        else {
            # Initialize Git repository
            Write-VersionControlLog "    Initializing Git repository..." "DEBUG"
            & git -C $ProjectRoot init 2>$null
            if ($LASTEXITCODE -eq 0) {
                # Configure Git user info
                & git -C $ProjectRoot config user.name "MingGoRTS VersionControl" 2>$null
                & git -C $ProjectRoot config user.email "versioncontrol@minggorts.com" 2>$null
                
                # Add all files
                & git -C $ProjectRoot add . 2>$null
                if ($LASTEXITCODE -eq 0) {
                    # Initial commit
                    $CommitMessage = "feat: Initialize MingGoRTS project [$($Task.TaskID)]"
                    $CommitOutput = & git -C $ProjectRoot commit -m $CommitMessage 2>&1
                    if ($LASTEXITCODE -eq 0) {
                        $CommitHash = & git -C $ProjectRoot rev-parse --short HEAD 2>$null
                        if ($LASTEXITCODE -eq 0) {
                            $Result.CommitHash = $CommitHash
                            $Result.Success = $true
                            $Result.Output = "Git repository initialized successfully: $CommitHash"
                            Write-VersionControlLog "    Git repository initialized successfully: $CommitHash" "DEBUG"
                        }
                    }
                    else {
                        $Result.ErrorMessage = "Initial commit failed: $CommitOutput"
                    }
                }
                else {
                    $Result.ErrorMessage = "Git add failed"
                }
            }
            else {
                $Result.ErrorMessage = "Git repository initialization failed"
            }
        }
    }
    catch {
        $Result.ErrorMessage = "Version control process exception: $($_.Exception.Message)"
        Write-VersionControlLog "    Version control exception: $($_.Exception.Message)" "ERROR"
    }
    finally {
        $Stopwatch.Stop()
        $Result.CommitTime = $Stopwatch.Elapsed.TotalSeconds
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
                $TaskToUpdate | Add-Member -NotePropertyName "VersionControlResult" -NotePropertyValue @{
                    Success = $Result.Success
                    CommitTime = $Result.CommitTime
                    CommitHash = $Result.CommitHash
                    Branch = $Result.Branch
                    ErrorMessage = $Result.ErrorMessage
                    Warnings = $Result.Warnings
                } -Force
                
                # If successful, mark as committed
                if ($Result.Success) {
                    $TaskToUpdate | Add-Member -NotePropertyName "Committed" -NotePropertyValue $true -Force
                }
            }
            
            # Save updated requirement
            $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $Task.RequirementFile
            
            Write-VersionControlLog "    Status updated: $($Task.TaskID) -> $Status" "DEBUG"
        }
    }
    catch {
        Write-VersionControlLog "    Status update failed: $($_.Exception.Message)" "ERROR"
    }
}

# ============================================
# Phase 5: Version Control Report Generation
# ============================================

function New-VersionControlReport {
    [CmdletBinding()]
    param([array]$Results)
    
    Write-VersionControlLog "==========================================" "INFO"
    Write-VersionControlLog "Phase 5: Generating Version Control Report" "INFO"
    Write-VersionControlLog "==========================================" "INFO"
    
    $ReportFile = Join-Path $TasksPath "VersionControl\VersionControlReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    # Ensure directory exists
    $ReportDir = Split-Path -Parent $ReportFile
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    $TotalTime = (Get-Date) - $VersionControlTracker.StartTime
    
    $Report = @"
# Version Control Workflow Report

**Execution Time**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")  
**Project Path**: $ProjectRoot  
**Total Execution Time**: $($TotalTime.ToString("hh\:mm\:ss"))

## Version Control Summary

| Metric | Value |
|--------|-------|
| Total Tasks | $($Results.Count) |
| Successfully Committed | $($VersionControlTracker.TotalCommitted) |
| Commit Failed | $($VersionControlTracker.TotalFailed) |
| Success Rate | $([math]::Round(($VersionControlTracker.TotalCommitted / [math]::Max($Results.Count, 1)) * 100, 2))% |

## Detailed Results

### ✅ Successfully Committed Tasks

"@
    
    foreach ($Result in $Results | Where-Object { $_.Success }) {
        $Report += "- **$($Result.TaskID)**: Commit successful (time: $([math]::Round($Result.CommitTime, 2))s, hash: $($Result.CommitHash))`n"
    }
    
    $Report += @"

### ❌ Failed Commit Tasks

"@
    
    foreach ($Result in $Results | Where-Object { -not $_.Success }) {
        $Report += "- **$($Result.TaskID)**: Commit failed - $($Result.ErrorMessage)`n"
    }
    
    $Report += @"

## Version Control Statistics

- Average commit time: $([math]::Round(($Results | Where-Object { $_.CommitTime } | Measure-Object -Property CommitTime -Average).Average, 2))s
- Longest commit time: $([math]::Round(($Results | Where-Object { $_.CommitTime } | Measure-Object -Property CommitTime -Maximum).Maximum, 2))s
- Shortest commit time: $([math]::Round(($Results | Where-Object { $_.CommitTime } | Measure-Object -Property CommitTime -Minimum).Minimum, 2))s

## Git Status

"@
    
    # Get Git status
    try {
        $GitStatus = & git -C $ProjectRoot status --porcelain 2>$null
        if ($LASTEXITCODE -eq 0) {
            if ($GitStatus) {
                $Report += "Current uncommitted changes:`n````"
                $Report += $GitStatus
                $Report += "`````n"
            }
            else {
                $Report += "Working directory clean, no uncommitted changes.`n"
            }
        }
        
        $CurrentBranch = & git -C $ProjectRoot rev-parse --abbrev-ref HEAD 2>$null
        if ($LASTEXITCODE -eq 0) {
            $Report += "Current branch: $CurrentBranch`n"
        }
        
        $LastCommit = & git -C $ProjectRoot log --oneline -1 2>$null
        if ($LASTEXITCODE -eq 0) {
            $Report += "Latest commit: $LastCommit`n"
        }
    }
    catch {
        $Report += "Unable to get Git status information.`n"
    }
    
    $Report += @"

## Next Steps

1. Review failed commit tasks and fix issues
2. Perform code review on successfully committed tasks
3. Push to remote repository (if needed)
4. Re-run version control workflow to verify fixes

---
*Generated by MingGoRTS Version Control Workflow System*
"@
    
    $Report | Set-Content -Path $ReportFile
    Write-VersionControlLog "Version control report generated: $ReportFile" "SUCCESS"
    
    return $ReportFile
}

# ============================================
# Automated Mode Monitoring
# ============================================

function Start-AutoMonitoring {
    Write-VersionControlLog "Starting automated monitoring mode, scan interval: $ScanInterval seconds" "INFO"
    
    while ($true) {
        try {
            Write-VersionControlLog "`n--- Starting scan $(Get-Date -Format 'HH:mm:ss') ---" "DEBUG"
            
            # Find completed tasks
            $DoneTasks = Find-DoneTasks
            
            if ($DoneTasks.Count -gt 0) {
                Write-VersionControlLog "Found $($DoneTasks.Count) tasks pending commit, starting process..." "INFO"
                
                # Claim tasks
                $ClaimedTasks = Request-TasksForCommit -Tasks $DoneTasks
                
                if ($ClaimedTasks.Count -gt 0) {
                    # Select version control mode
                    $VersionControlPlan = Select-VersionControlMode -Tasks $ClaimedTasks
                    
                    if ($VersionControlPlan) {
                        # Execute version control
                        $Results = Start-VersionControlProcess -Mode $VersionControlPlan.Mode -HighPriorityTasks $VersionControlPlan.HighPriorityTasks -NormalTasks $VersionControlPlan.NormalTasks
                        
                        # Generate report
                        $ReportFile = New-VersionControlReport -Results $Results
                        Write-VersionControlLog "Version control cycle completed, report: $ReportFile" "SUCCESS"
                    }
                }
            }
            else {
                Write-VersionControlLog "No pending commit tasks found, continuing monitoring..." "INFO"
            }
            
            Write-VersionControlLog "Waiting $ScanInterval seconds for next scan..." "DEBUG"
            Start-Sleep -Seconds $ScanInterval
        }
        catch {
            Write-VersionControlLog "Automated monitoring error: $($_.Exception.Message)" "ERROR"
            Start-Sleep -Seconds 60 # Wait longer on error
        }
    }
}

# ============================================
# Main Execution Flow
# ============================================

function Main {
    Show-VersionControlBanner
    
    if ($AutoMode) {
        Write-VersionControlLog "Starting automated version control mode" "INFO"
        Start-AutoMonitoring
    }
    else {
        Write-VersionControlLog "Executing single version control workflow" "INFO"
        
        # Find completed tasks
        $DoneTasks = Find-DoneTasks
        
        if ($DoneTasks.Count -eq 0) {
            Write-VersionControlLog "No pending commit tasks found" "INFO"
            return
        }
        
        # Claim tasks
        $ClaimedTasks = Request-TasksForCommit -Tasks $DoneTasks
        
        if ($ClaimedTasks.Count -eq 0) {
            Write-VersionControlLog "No successfully claimed tasks" "WARNING"
            return
        }
        
        # Select version control mode
        $VersionControlPlan = Select-VersionControlMode -Tasks $ClaimedTasks
        
        if (-not $VersionControlPlan) {
            Write-VersionControlLog "Unable to determine version control mode" "ERROR"
            return
        }
        
        # Execute version control
        $Results = Start-VersionControlProcess -Mode $VersionControlPlan.Mode -HighPriorityTasks $VersionControlPlan.HighPriorityTasks -NormalTasks $VersionControlPlan.NormalTasks
        
        # Generate report
        $ReportFile = New-VersionControlReport -Results $Results
        
        Write-VersionControlLog "`n==========================================" "SUCCESS"
        Write-VersionControlLog "Version control workflow execution completed" "SUCCESS"
        Write-VersionControlLog "==========================================" "SUCCESS"
        
        Write-VersionControlLog "`n📊 Execution Summary:" "INFO"
        Write-VersionControlLog "  Successfully committed: $($VersionControlTracker.TotalCommitted) tasks" "SUCCESS"
        Write-VersionControlLog "  Commit failed: $($VersionControlTracker.TotalFailed) tasks" $(if ($VersionControlTracker.TotalFailed -gt 0) { "WARNING" } else { "INFO" })
        Write-VersionControlLog "  Detailed report: $ReportFile" "INFO"
    }
}

# Execute main program
Main
