# BMAD Coding Workflow System - Simple Version
# Automated development workflow for MingGoRTS

# Enhanced logging system
function Write-WorkflowLog {
    param(
        [string]$Message,
        [string]$Level = "INFO",
        [string]$Component = "WORKFLOW"
    )
    
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = switch ($Level) {
        "INFO" { "White" }
        "SUCCESS" { "Green" }
        "WARNING" { "Yellow" }
        "ERROR" { "Red" }
        "DEBUG" { "Gray" }
        default { "White" }
    }
    
    $logEntry = "[$timestamp] [$Component] [$Level] $Message"
    Write-Host $logEntry -ForegroundColor $color
    
    # Log to file for persistence
    $logFile = "Logs\Workflow_$(Get-Date -Format 'yyyyMMdd').log"
    if (!(Test-Path "Logs")) { New-Item -ItemType Directory -Path "Logs" | Out-Null }
    Add-Content -Path $logFile -Value $logEntry
}

# Requirement system with status tracking
class MingRequirementSystem {
    [System.Collections.Generic.List[PSCustomObject]]$Requirements
    [hashtable]$StatusPriority
    
    MingRequirementSystem() {
        $this.Requirements = [System.Collections.Generic.List[PSCustomObject]]::new()
        $this.StatusPriority = @{
            "debug" = 1
            "Pending" = 2
            "ToOptimize" = 3
            "Developed" = 4
            "Completed" = 5
        }
        $this.InitializeSampleRequirements()
    }
    
    [void]InitializeSampleRequirements() {
        Write-WorkflowLog "Initializing sample requirements..." "INFO" "REQUIREMENTS"
        
        $sampleTasks = @(
            @{ Name = "UI System Enhancement"; Priority = 1; Status = "Pending"; Complexity = "High" },
            @{ Name = "Audio System Integration"; Priority = 2; Status = "Pending"; Complexity = "Medium" },
            @{ Name = "Performance Optimization"; Priority = 3; Status = "ToOptimize"; Complexity = "High" },
            @{ Name = "Bug Fix: Memory Leak"; Priority = 1; Status = "debug"; Complexity = "Critical" },
            @{ Name = "Network Synchronization"; Priority = 2; Status = "Pending"; Complexity = "High" },
            @{ Name = "Save/Load System"; Priority = 3; Status = "Developed"; Complexity = "Medium" }
        )
        
        foreach ($task in $sampleTasks) {
            $requirement = [PSCustomObject]@{
                ID = $this.Requirements.Count + 1
                Name = $task.Name
                Priority = $task.Priority
                Status = $task.Status
                Complexity = $task.Complexity
                CreatedTime = Get-Date
                UpdatedTime = Get-Date
                AssignedTo = $null
                Progress = 0
            }
            $this.Requirements.Add($requirement)
            Write-WorkflowLog "Added requirement: $($requirement.Name) (Status: $($requirement.Status))" "INFO" "REQUIREMENTS"
        }
    }
    
    [System.Collections.Generic.List[PSCustomObject]]GetTasksByStatus([string]$Status) {
        return $this.Requirements | Where-Object { $_.Status -eq $Status }
    }
    
    [System.Collections.Generic.List[PSCustomObject]]GetPrioritizedTasks() {
        return $this.Requirements | 
            Where-Object { $_.Status -in @("debug", "Pending", "ToOptimize") } |
            Sort-Object { $this.StatusPriority[$_.Status] }, { $_.Priority }
    }
    
    [void]UpdateRequirementStatus([int]$ID, [string]$NewStatus, [string]$AssignedTo = $null) {
        $requirement = $this.Requirements | Where-Object { $_.ID -eq $ID }
        if ($requirement) {
            $oldStatus = $requirement.Status
            $requirement.Status = $NewStatus
            $requirement.UpdatedTime = Get-Date
            if ($AssignedTo) { $requirement.AssignedTo = $AssignedTo }
            
            Write-WorkflowLog "Task [$ID] status updated: $oldStatus -> $NewStatus" "SUCCESS" "REQUIREMENTS"
            Write-WorkflowLog "Task: $($requirement.Name) | Assigned: $($requirement.AssignedTo)" "INFO" "REQUIREMENTS"
        } else {
            Write-WorkflowLog "Task ID $ID not found" "ERROR" "REQUIREMENTS"
        }
    }
    
    [void]UpdateProgress([int]$ID, [int]$Progress) {
        $requirement = $this.Requirements | Where-Object { $_.ID -eq $ID }
        if ($requirement) {
            $requirement.Progress = [Math]::Min(100, [Math]::Max(0, $Progress))
            Write-WorkflowLog "Task [$ID] progress: $($requirement.Progress)%" "INFO" "REQUIREMENTS"
        }
    }
}

# Coding workflow engine
class MingCodingWorkflowEngine {
    [MingRequirementSystem]$RequirementSystem
    [hashtable]$DeveloperPool
    [bool]$AutoMode
    
    MingCodingWorkflowEngine([MingRequirementSystem]$ReqSystem, [bool]$AutoMode = $true) {
        $this.RequirementSystem = $ReqSystem
        $this.AutoMode = $AutoMode
        $this.InitializeDeveloperPool()
    }
    
    [void]InitializeDeveloperPool() {
        $this.DeveloperPool = @{
            "Dev-Alan" = @{ Status = "Available"; Skills = @("UI", "Audio"); CurrentTask = $null }
            "Dev-Betty" = @{ Status = "Available"; Skills = @("Network", "Performance"); CurrentTask = $null }
            "Dev-Charlie" = @{ Status = "Available"; Skills = @("Backend", "Database"); CurrentTask = $null }
        }
        Write-WorkflowLog "Developer pool initialized with $($this.DeveloperPool.Count) developers" "INFO" "WORKFLOW"
    }
    
    [string]AssignDeveloper([PSCustomObject]$Task) {
        $availableDevs = $this.DeveloperPool.GetEnumerator() | 
            Where-Object { $_.Value.Status -eq "Available" }
        
        if ($availableDevs.Count -eq 0) {
            Write-WorkflowLog "No available developers for task: $($Task.Name)" "WARNING" "WORKFLOW"
            return $null
        }
        
        $assignedDev = $availableDevs | Select-Object -First 1
        $devName = $assignedDev.Key
        $this.DeveloperPool[$devName].Status = "Busy"
        $this.DeveloperPool[$devName].CurrentTask = $Task
        
        Write-WorkflowLog "Assigned task '$($Task.Name)' to developer $devName" "SUCCESS" "WORKFLOW"
        return $devName
    }
    
    [void]ReleaseDeveloper([string]$DeveloperName) {
        if ($this.DeveloperPool.ContainsKey($DeveloperName)) {
            $this.DeveloperPool[$DeveloperName].Status = "Available"
            $this.DeveloperPool[$DeveloperName].CurrentTask = $null
            Write-WorkflowLog "Released developer: $DeveloperName" "INFO" "WORKFLOW"
        }
    }
    
    [void]ProcessTaskDevelopment([PSCustomObject]$Task, [string]$DeveloperName) {
        Write-WorkflowLog "Starting development: $($Task.Name) by $DeveloperName" "INFO" "DEVELOPMENT"
        
        $developmentSteps = @(
            @{ Step = "Analysis"; Duration = 2 },
            @{ Step = "Design"; Duration = 3 },
            @{ Step = "Implementation"; Duration = 5 },
            @{ Step = "Testing"; Duration = 2 },
            @{ Step = "Bug Fixing"; Duration = 3 }
        )
        
        foreach ($step in $developmentSteps) {
            Write-WorkflowLog "  [$($Task.ID)] $($step.Step) phase..." "DEBUG" "DEVELOPMENT"
            Start-Sleep -Seconds $step.Duration
            
            $progress = [Math]::Round(($developmentSteps.IndexOf($step) + 1) / $developmentSteps.Count * 100)
            $this.RequirementSystem.UpdateProgress($Task.ID, $progress)
        }
        
        $hasBugs = $false
        if ($Task.Complexity -eq "Critical" -or $Task.Complexity -eq "High") {
            $hasBugs = (Get-Random -Maximum 10) -le 3
        } else {
            $hasBugs = (Get-Random -Maximum 10) -le 1
        }
        
        if ($hasBugs) {
            Write-WorkflowLog "Development completed with bugs detected for task: $($Task.Name)" "WARNING" "DEVELOPMENT"
            $this.RequirementSystem.UpdateRequirementStatus($Task.ID, "debug", $DeveloperName)
        } else {
            Write-WorkflowLog "Development completed successfully for task: $($Task.Name)" "SUCCESS" "DEVELOPMENT"
            $this.RequirementSystem.UpdateRequirementStatus($Task.ID, "Developed", $DeveloperName)
            $this.RequirementSystem.UpdateProgress($Task.ID, 100)
        }
        
        $this.ReleaseDeveloper($DeveloperName)
    }
    
    [void]ProcessOptimization([PSCustomObject]$Task, [string]$DeveloperName) {
        Write-WorkflowLog "Starting optimization: $($Task.Name) by $DeveloperName" "INFO" "OPTIMIZATION"
        
        $optimizationSteps = @(
            @{ Step = "Performance Analysis"; Duration = 2 },
            @{ Step = "Bottleneck Identification"; Duration = 2 },
            @{ Step = "Optimization Implementation"; Duration = 4 },
            @{ Step = "Performance Testing"; Duration = 2 }
        )
        
        foreach ($step in $optimizationSteps) {
            Write-WorkflowLog "  [$($Task.ID)] $($step.Step) phase..." "DEBUG" "OPTIMIZATION"
            Start-Sleep -Seconds $step.Duration
            
            $progress = [Math]::Round(($optimizationSteps.IndexOf($step) + 1) / $optimizationSteps.Count * 100)
            $this.RequirementSystem.UpdateProgress($Task.ID, $progress)
        }
        
        $success = (Get-Random -Maximum 10) -ne 0
        
        if ($success) {
            Write-WorkflowLog "Optimization completed successfully for task: $($Task.Name)" "SUCCESS" "OPTIMIZATION"
            $this.RequirementSystem.UpdateRequirementStatus($Task.ID, "Developed", $DeveloperName)
            $this.RequirementSystem.UpdateProgress($Task.ID, 100)
        } else {
            Write-WorkflowLog "Optimization failed for task: $($Task.Name)" "WARNING" "OPTIMIZATION"
        }
        
        $this.ReleaseDeveloper($DeveloperName)
    }
    
    [void]ProcessDebugging([PSCustomObject]$Task, [string]$DeveloperName) {
        Write-WorkflowLog "Starting debugging: $($Task.Name) by $DeveloperName" "INFO" "DEBUG"
        
        $debugSteps = @(
            @{ Step = "Bug Reproduction"; Duration = 1 },
            @{ Step = "Root Cause Analysis"; Duration = 2 },
            @{ Step = "Fix Implementation"; Duration = 3 },
            @{ Step = "Verification"; Duration = 1 }
        )
        
        foreach ($step in $debugSteps) {
            Write-WorkflowLog "  [$($Task.ID)] $($step.Step) phase..." "DEBUG" "DEBUG"
            Start-Sleep -Seconds $step.Duration
            
            $progress = [Math]::Round(($debugSteps.IndexOf($step) + 1) / $debugSteps.Count * 100)
            $this.RequirementSystem.UpdateProgress($Task.ID, $progress)
        }
        
        $success = (Get-Random -Maximum 10) -le 7
        
        if ($success) {
            Write-WorkflowLog "Debugging completed successfully for task: $($Task.Name)" "SUCCESS" "DEBUG"
            $this.RequirementSystem.UpdateRequirementStatus($Task.ID, "Developed", $DeveloperName)
            $this.RequirementSystem.UpdateProgress($Task.ID, 100)
        } else {
            Write-WorkflowLog "Debugging incomplete, task remains in debug state: $($Task.Name)" "WARNING" "DEBUG"
        }
        
        $this.ReleaseDeveloper($DeveloperName)
    }
    
    [void]ExecuteWorkflowCycle() {
        Write-WorkflowLog "=== Starting Workflow Cycle ===" "INFO" "WORKFLOW"
        
        $pendingTasks = $this.RequirementSystem.GetPrioritizedTasks()
        
        if ($pendingTasks.Count -eq 0) {
            Write-WorkflowLog "No pending tasks found" "INFO" "WORKFLOW"
            return
        }
        
        Write-WorkflowLog "Found $($pendingTasks.Count) pending tasks" "INFO" "WORKFLOW"
        
        foreach ($task in $pendingTasks) {
            $availableDevs = $this.DeveloperPool.GetEnumerator() | 
                Where-Object { $_.Value.Status -eq "Available" }
            
            if ($availableDevs.Count -eq 0) {
                Write-WorkflowLog "No available developers, skipping remaining tasks" "WARNING" "WORKFLOW"
                break
            }
            
            $assignedDev = $this.AssignDeveloper($task)
            if ($assignedDev) {
                switch ($task.Status) {
                    "Pending" {
                        $this.ProcessTaskDevelopment($task, $assignedDev)
                    }
                    "ToOptimize" {
                        $this.ProcessOptimization($task, $assignedDev)
                    }
                    "debug" {
                        $this.ProcessDebugging($task, $assignedDev)
                    }
                    default {
                        Write-WorkflowLog "Unknown task status: $($task.Status)" "WARNING" "WORKFLOW"
                        $this.ReleaseDeveloper($assignedDev)
                    }
                }
            }
        }
        
        Write-WorkflowLog "=== Workflow Cycle Completed ===" "INFO" "WORKFLOW"
    }
    
    [void]ShowWorkflowStatus() {
        Write-Host "`n=== BMAD Coding Workflow Status ===" -ForegroundColor Cyan
        
        $statusSummary = $this.RequirementSystem.Requirements | Group-Object -Property Status
        Write-Host "`nTask Status Summary:" -ForegroundColor Yellow
        foreach ($group in $statusSummary) {
            $color = switch ($group.Name) {
                "Completed" { "Green" }
                "Developed" { "Blue" }
                "Pending" { "White" }
                "ToOptimize" { "Yellow" }
                "debug" { "Red" }
                default { "Gray" }
            }
            Write-Host "  [$($group.Name.PadRight(12))] $($group.Count) tasks" -ForegroundColor $color
        }
        
        Write-Host "`nDeveloper Status:" -ForegroundColor Yellow
        foreach ($dev in $this.DeveloperPool.GetEnumerator()) {
            $statusColor = if ($dev.Value.Status -eq "Available") { "Green" } else { "Red" }
            $currentTask = if ($dev.Value.CurrentTask) { " - $($dev.Value.CurrentTask.Name)" } else { "" }
            Write-Host "  $($dev.Key.PadRight(15)): $($dev.Value.Status)$currentTask" -ForegroundColor $statusColor
        }
        
        $pendingTasks = $this.RequirementSystem.GetPrioritizedTasks()
        if ($pendingTasks.Count -gt 0) {
            Write-Host "`nNext Priority Tasks:" -ForegroundColor Yellow
            for ($i = 0; $i -lt [Math]::Min(5, $pendingTasks.Count); $i++) {
                $task = $pendingTasks[$i]
                Write-Host "  [$($task.ID)] $($task.Name) (Priority: $($task.Priority), Status: $($task.Status))" -ForegroundColor White
            }
        }
        
        Write-Host "`n======================================" -ForegroundColor Cyan
    }
    
    [bool]AllTasksCompleted() {
        $completedTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "Completed" }
        return $completedTasks.Count -eq $this.RequirementSystem.Requirements.Count
    }
}

# Main workflow controller
class MingBMADWorkflowController {
    [MingRequirementSystem]$RequirementSystem
    [MingCodingWorkflowEngine]$WorkflowEngine
    [bool]$AutoMode
    [int]$WorkflowInterval
    
    MingBMADWorkflowController([bool]$AutoMode = $true, [int]$Interval = 30) {
        $this.RequirementSystem = [MingRequirementSystem]::new()
        $this.WorkflowEngine = [MingCodingWorkflowEngine]::new($this.RequirementSystem, $AutoMode)
        $this.AutoMode = $AutoMode
        $this.WorkflowInterval = $Interval
        
        Write-WorkflowLog "BMAD Workflow Controller initialized" "SUCCESS" "CONTROLLER"
        Write-WorkflowLog "Auto Mode: $($this.AutoMode)" "INFO" "CONTROLLER"
        Write-WorkflowLog "Workflow Interval: $($this.WorkflowInterval) seconds" "INFO" "CONTROLLER"
    }
    
    [void]StartWorkflow() {
        Write-WorkflowLog "=== Starting BMAD Coding Workflow ===" "SUCCESS" "CONTROLLER"
        
        if ($this.AutoMode) {
            Write-WorkflowLog "Auto mode enabled - running continuous workflow" "INFO" "CONTROLLER"
            
            while ($true) {
                try {
                    $this.WorkflowEngine.ExecuteWorkflowCycle()
                    $this.WorkflowEngine.ShowWorkflowStatus()
                    
                    if ($this.WorkflowEngine.AllTasksCompleted()) {
                        Write-WorkflowLog "All tasks completed! Workflow finished." "SUCCESS" "CONTROLLER"
                        break
                    }
                    
                    Write-WorkflowLog "Waiting $($this.WorkflowInterval) seconds before next cycle..." "INFO" "CONTROLLER"
                    Start-Sleep -Seconds $this.WorkflowInterval
                }
                catch {
                    Write-WorkflowLog "Workflow error: $($_.Exception.Message)" "ERROR" "CONTROLLER"
                    Start-Sleep -Seconds 10
                }
            }
        } else {
            Write-WorkflowLog "Manual mode enabled - running single cycle" "INFO" "CONTROLLER"
            $this.WorkflowEngine.ExecuteWorkflowCycle()
            $this.WorkflowEngine.ShowWorkflowStatus()
        }
        
        Write-WorkflowLog "=== BMAD Coding Workflow Completed ===" "SUCCESS" "CONTROLLER"
    }
    
    [void]AddTask([string]$Name, [int]$Priority, [string]$Status = "Pending", [string]$Complexity = "Medium") {
        $task = [PSCustomObject]@{
            ID = $this.RequirementSystem.Requirements.Count + 1
            Name = $Name
            Priority = $Priority
            Status = $Status
            Complexity = $Complexity
            CreatedTime = Get-Date
            UpdatedTime = Get-Date
            AssignedTo = $null
            Progress = 0
        }
        
        $this.RequirementSystem.Requirements.Add($task)
        Write-WorkflowLog "New task added: $($task.Name) (Priority: $($task.Priority))" "SUCCESS" "CONTROLLER"
    }
}

# Simple parameter handling
$ManualMode = $false
$StatusOnly = $false
$IntervalValue = 30
$TaskToAdd = ""
$TaskPriority = 3

# Parse command line arguments
for ($i = 0; $i -lt $args.Count; $i++) {
    switch ($args[$i]) {
        "-Manual" { $ManualMode = $true }
        "-Status" { $StatusOnly = $true }
        "-Interval" { 
            if ($i + 1 -lt $args.Count) { 
                $IntervalValue = [int]$args[$i + 1]
                $i++
            }
        }
        "-AddTask" {
            if ($i + 1 -lt $args.Count) {
                $TaskToAdd = $args[$i + 1]
                $i++
            }
        }
        "-Priority" {
            if ($i + 1 -lt $args.Count) {
                $TaskPriority = [int]$args[$i + 1]
                $i++
            }
        }
    }
}

try {
    # Create workflow controller
    $controller = [MingBMADWorkflowController]::new(-not $ManualMode, $IntervalValue)
    
    # Handle commands
    if ($TaskToAdd) {
        $controller.AddTask($TaskToAdd, $TaskPriority)
        Write-Host "Task added successfully" -ForegroundColor Green
        exit 0
    }
    
    if ($StatusOnly) {
        $controller.WorkflowEngine.ShowWorkflowStatus()
        exit 0
    }
    
    # Start workflow
    $controller.StartWorkflow()
}
catch {
    Write-WorkflowLog "Fatal error: $($_.Exception.Message)" "ERROR" "SYSTEM"
    exit 1
}
