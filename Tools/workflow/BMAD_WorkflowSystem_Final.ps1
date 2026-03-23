# BMAD Workflow System - Clean Version

# Log function
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = switch ($Level) {
        "INFO" { "White" }
        "SUCCESS" { "Green" }
        "WARNING" { "Yellow" }
        "ERROR" { "Red" }
        default { "White" }
    }
    
    Write-Host "[$timestamp] [$Level] $Message" -ForegroundColor $color
}

# Requirement System Manager
class MingRequirementSystem {
    [System.Collections.Generic.List[PSObject]]$Requirements
    
    MingRequirementSystem() {
        $this.Requirements = [System.Collections.Generic.List[PSObject]]::new()
    }
    
    [void]AddRequirement([PSObject]$Requirement) {
        $this.Requirements.Add($Requirement)
        Write-Log "Added requirement: $($Requirement.Name)" "INFO"
    }
    
    [System.Collections.Generic.List[PSObject]]GetPendingRequirements() {
        return $this.Requirements | Where-Object { $_.Status -eq "Pending" } | Sort-Object -Property Priority -Descending
    }
    
    [void]UpdateRequirementStatus([int]$ID, [string]$NewStatus) {
        $requirement = $this.Requirements | Where-Object { $_.ID -eq $ID }
        if ($requirement) {
            $oldStatus = $requirement.Status
            $requirement.Status = $NewStatus
            Write-Log "Requirement [$ID] status updated: $oldStatus -> $NewStatus" "INFO"
        }
    }
}

# Coding API Service
class MingCodingAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingCodingAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartDevelopment() {
        Write-Log "Coding API starting development process..." "INFO"
        
        $pendingTasks = $this.RequirementSystem.GetPendingRequirements()
        
        foreach ($task in $pendingTasks) {
            Write-Log "Starting development task: $($task.ID) - $($task.Name)" "INFO"
            
            # Simulate development process
            $developmentResult = $this.DevelopTask($task)
            
            if ($developmentResult.Success -and $developmentResult.NoBugs) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "Developed")
                Write-Log "Task development completed: $($task.ID)" "SUCCESS"
            } else {
                Write-Log "Task development failed: $($task.ID) - $($developmentResult.Error)" "ERROR"
            }
        }
    }
    
    [PSObject]DevelopTask([PSObject]$Task) {
        # Simulate development process
        Write-Log "  Developing: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 10 -Maximum 30)
        
        # Simulate development result (90% success rate)
        $success = (Get-Random -Maximum 10) -ne 0
        $noBugs = $success -and (Get-Random -Maximum 10) -ne 0
        
        return [PSCustomObject]@{
            Success = $success
            NoBugs = $noBugs
            Error = if (-not $success) { "Technical issues during development" } else { $null }
        }
    }
    
    [void]HandleDebugTasks() {
        Write-Log "Coding API handling Debug status tasks..." "INFO"
        
        $debugTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "debug" }
        
        foreach ($task in $debugTasks) {
            Write-Log "Fixing task: $($task.ID) - $($task.Name)" "INFO"
            
            # Reset to pending status
            $this.RequirementSystem.UpdateRequirementStatus($task.ID, "Pending")
            
            # Re-develop
            $developmentResult = $this.DevelopTask($task)
            
            if ($developmentResult.Success -and $developmentResult.NoBugs) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "Developed")
                Write-Log "Task fix completed: $($task.ID)" "SUCCESS"
            }
        }
    }
    
    [void]HandleOptimizationTasks() {
        Write-Log "Coding API handling optimization tasks..." "INFO"
        
        $optTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "ToOptimize" }
        
        foreach ($task in $optTasks) {
            Write-Log "Optimizing task: $($task.ID) - $($task.Name)" "INFO"
            
            # Optimize development
            $optimizationResult = $this.OptimizeTask($task)
            
            if ($optimizationResult.Success) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "Developed")
                Write-Log "Task optimization completed: $($task.ID)" "SUCCESS"
            }
        }
    }
    
    [PSObject]OptimizeTask([PSObject]$Task) {
        Write-Log "  Optimizing: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 5 -Maximum 15)
        
        # Simulate optimization result (95% success rate)
        $success = (Get-Random -Maximum 20) -ne 0
        
        return [PSCustomObject]@{
            Success = $success
            Error = if (-not $success) { "Issues during optimization" } else { $null }
        }
    }
}

# Compile API Service
class MingCompileAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingCompileAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartCompilation() {
        Write-Log "Compile API starting compilation process..." "INFO"
        
        $developedTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "Developed" }
        
        foreach ($task in $developedTasks) {
            Write-Log "Compiling task: $($task.ID) - $($task.Name)" "INFO"
            
            $compileResult = $this.CompileTask($task)
            
            if ($compileResult.Success) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "CodeReview")
                Write-Log "Compilation successful: $($task.ID)" "SUCCESS"
            } else {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "debug")
                Write-Log "Compilation failed: $($task.ID) - $($compileResult.Error)" "ERROR"
            }
        }
    }
    
    [PSObject]CompileTask([PSObject]$Task) {
        Write-Log "  Compiling: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 5 -Maximum 20)
        
        # Simulate compilation result (85% success rate)
        $success = (Get-Random -Maximum 20) -le 17
        
        return [PSCustomObject]@{
            Success = $success
            Error = if (-not $success) { "Compilation error: syntax issues or missing dependencies" } else { $null }
        }
    }
}

# Code Scan API Service
class MingCodeScanAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingCodeScanAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartCodeScanning() {
        Write-Log "Code Scan API starting scan process..." "INFO"
        
        $reviewTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "CodeReview" }
        
        foreach ($task in $reviewTasks) {
            Write-Log "Scanning task: $($task.ID) - $($task.Name)" "INFO"
            
            $scanResult = $this.ScanCode($task)
            
            if ($scanResult.HasIssues) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "ToOptimize")
                Write-Log "Code issues found: $($task.ID) - $($scanResult.Issues)" "WARNING"
            } else {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "Done")
                Write-Log "Code scan passed: $($task.ID)" "SUCCESS"
            }
        }
    }
    
    [PSObject]ScanCode([PSObject]$Task) {
        Write-Log "  Scanning code: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 3 -Maximum 10)
        
        # Simulate scan result (30% find issues)
        $hasIssues = (Get-Random -Maximum 10) -le 3
        $issues = if ($hasIssues) { 
            @("Code standards issues", "Potential security vulnerabilities", "Performance issues") | Get-Random 
        } else { $null }
        
        return [PSCustomObject]@{
            HasIssues = $hasIssues
            Issues = $issues
        }
    }
}

# Code Standards API Service
class MingCodeStandardAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingCodeStandardAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartStandardCheck() {
        Write-Log "Code Standards API starting check process..." "INFO"
        
        $reviewTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "CodeReview" }
        
        foreach ($task in $reviewTasks) {
            Write-Log "Checking task standards: $($task.ID) - $($task.Name)" "INFO"
            
            $standardResult = $this.CheckStandards($task)
            
            if ($standardResult.Compliant) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "Done")
                Write-Log "Standards check passed: $($task.ID)" "SUCCESS"
            } else {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "ToOptimize")
                Write-Log "Standards check failed: $($task.ID) - $($standardResult.Violations)" "WARNING"
            }
        }
    }
    
    [PSObject]CheckStandards([PSObject]$Task) {
        Write-Log "  Checking standards: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 2 -Maximum 8)
        
        # Simulate standards check result (25% find violations)
        $compliant = (Get-Random -Maximum 10) -gt 2
        $violations = if (-not $compliant) { 
            @("Naming convention violations", "Insufficient comments", "Function too long") | Get-Random 
        } else { $null }
        
        return [PSCustomObject]@{
            Compliant = $compliant
            Violations = $violations
        }
    }
}

# Version Control API Service
class MingVersionControlAPI {
    [MingRequirementSystem]$RequirementSystem
    
    MingVersionControlAPI([MingRequirementSystem]$ReqSystem) {
        $this.RequirementSystem = $ReqSystem
    }
    
    [void]StartSmartCommit() {
        Write-Log "Version Control API starting smart commit process..." "INFO"
        
        $doneTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "Done" }
        
        foreach ($task in $doneTasks) {
            Write-Log "Committing task: $($task.ID) - $($task.Name)" "INFO"
            
            $commitResult = $this.SmartCommit($task)
            
            if ($commitResult.Success) {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "Completed")
                Write-Log "Task commit successful: $($task.ID)" "SUCCESS"
            } else {
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "debug")
                # Reset to uncommitted status
                $this.RequirementSystem.UpdateRequirementStatus($task.ID, "Developed")
                Write-Log "Task commit failed: $($task.ID) - $($commitResult.Error)" "ERROR"
            }
        }
    }
    
    [PSObject]SmartCommit([PSObject]$Task) {
        Write-Log "  Smart committing: $($Task.Name)..." "INFO"
        Start-Sleep -Seconds (Get-Random -Minimum 3 -Maximum 10)
        
        # Simulate commit result (90% success rate)
        $success = (Get-Random -Maximum 10) -ne 0
        
        return [PSCustomObject]@{
            Success = $success
            Error = if (-not $success) { "Commit conflicts or network issues" } else { $null }
        }
    }
}

# BMAD Workflow Coordinator
class MingBMADWorkflowCoordinator {
    [MingRequirementSystem]$RequirementSystem
    [MingCodingAPI]$CodingAPI
    [MingCompileAPI]$CompileAPI
    [MingCodeScanAPI]$CodeScanAPI
    [MingCodeStandardAPI]$CodeStandardAPI
    [MingVersionControlAPI]$VersionControlAPI
    
    MingBMADWorkflowCoordinator() {
        $this.RequirementSystem = [MingRequirementSystem]::new()
        $this.CodingAPI = [MingCodingAPI]::new($this.RequirementSystem)
        $this.CompileAPI = [MingCompileAPI]::new($this.RequirementSystem)
        $this.CodeScanAPI = [MingCodeScanAPI]::new($this.RequirementSystem)
        $this.CodeStandardAPI = [MingCodeStandardAPI]::new($this.RequirementSystem)
        $this.VersionControlAPI = [MingVersionControlAPI]::new($this.RequirementSystem)
    }
    
    [void]StartWorkflow() {
        Write-Log "=== BMAD Intelligent Development Workflow Started ===" "INFO"
        
        # Continuous workflow execution
        while ($true) {
            Write-Log "--- Workflow Cycle Start ---" "INFO"
            
            # 1. Coding API handles pending tasks
            $this.CodingAPI.StartDevelopment()
            
            # 2. Compile API handles developed tasks
            $this.CompileAPI.StartCompilation()
            
            # 3. Coding API handles debug status tasks
            $this.CodingAPI.HandleDebugTasks()
            
            # 4. Code Scan API handles code review status
            $this.CodeScanAPI.StartCodeScanning()
            
            # 5. Code Standards API checks code review status
            $this.CodeStandardAPI.StartStandardCheck()
            
            # 6. Coding API handles optimization tasks
            $this.CodingAPI.HandleOptimizationTasks()
            
            # 7. Version Control API handles done status tasks
            $this.VersionControlAPI.StartSmartCommit()
            
            # Show current status
            $this.ShowWorkflowStatus()
            
            Write-Log "--- Workflow Cycle Complete ---" "INFO"
            Start-Sleep -Seconds 30  # Check every 30 seconds
            
            # Check if all tasks are completed
            if ($this.AllTasksCompleted()) {
                Write-Log "All tasks completed, workflow ending" "SUCCESS"
                break
            }
        }
    }
    
    [void]ShowWorkflowStatus() {
        $status = $this.RequirementSystem.Requirements | Group-Object -Property Status | Sort-Object -Property Name
        
        Write-Host "Current Task Status:" -ForegroundColor Yellow
        foreach ($group in $status) {
            Write-Host "  $($group.Name): $($group.Count) tasks" -ForegroundColor White
        }
        Write-Host ""
    }
    
    [bool]AllTasksCompleted() {
        $pendingTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -ne "Completed" }
        return $pendingTasks.Count -eq 0
    }
    
    [void]InitializeWithTasks([string[]]$TaskNames) {
        Write-Log "Initializing workflow tasks..." "INFO"
        
        foreach ($taskName in $TaskNames) {
            $task = [PSCustomObject]@{
                TaskName = $taskName
                Priority = Get-Random -Minimum 1 -Maximum 10
                Complexity = "Medium"
                Dependencies = @()
                AssignedTeam = "Development Team"
                Status = "Pending"
            }
            
            $this.RequirementSystem.AddRequirement($task)
        }
        
        Write-Log "Workflow initialization complete, $($TaskNames.Count) tasks" "SUCCESS"
    }
}

# BMAD Workflow Launch Script
param(
    [string[]]$Tasks = @(
        "Story 3.6: UI and Interface System",
        "Story 3.8: Multiplayer Support", 
        "Story 3.9: Localization and Internationalization",
        "Story 3.10: Performance Optimization"
    ),
    [switch]$Interactive,
    [switch]$StatusOnly
)

function Start-BMADWorkflow {
    Write-Host "=== BMAD Intelligent Development Workflow System ===" -ForegroundColor Cyan
    Write-Host ""
    
    # Create workflow coordinator
    $coordinator = [MingBMADWorkflowCoordinator]::new()
    
    # Initialize tasks
    $coordinator.InitializeWithTasks($Tasks)
    
    if ($StatusOnly) {
        $coordinator.ShowWorkflowStatus()
        return
    }
    
    if ($Interactive) {
        Write-Host "Interactive mode enabled" -ForegroundColor Green
        Write-Host "Press Ctrl+C to stop workflow" -ForegroundColor Yellow
        Write-Host ""
    }
    
    # Start workflow
    $coordinator.StartWorkflow()
}

# Execute workflow
try {
    Start-BMADWorkflow
}
catch {
    Write-Log "Workflow execution error: $($_.Exception.Message)" "ERROR"
    exit 1
}

# Workflow Monitor Panel
function Show-BMADMonitor() {
    Clear-Host
    Write-Host "=== BMAD Workflow Monitor Panel ===" -ForegroundColor Cyan
    Write-Host ""
    
    while ($true) {
        # Load requirement system status
        $requirementSystem = [MingRequirementSystem]::new()
        $status = $requirementSystem.Requirements | Group-Object -Property Status
        
        # Display status
        Write-Host "Update Time: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" -ForegroundColor Gray
        Write-Host ""
        
        foreach ($group in $status) {
            $color = switch ($group.Name) {
                "Completed" { "Green" }
                "Done" { "Blue" }
                "CodeReview" { "Yellow" }
                "Developed" { "Cyan" }
                "Pending" { "White" }
                "debug" { "Red" }
                "ToOptimize" { "Magenta" }
                default { "Gray" }
            }
            
            Write-Host "[$($group.Name.PadRight(12))] $($group.Count) tasks" -ForegroundColor $color
        }
        
        Write-Host ""
        Write-Host "Press Ctrl+C to exit monitor" -ForegroundColor Gray
        Start-Sleep -Seconds 10
        Clear-Host
        Write-Host "=== BMAD Workflow Monitor Panel ===" -ForegroundColor Cyan
        Write-Host ""
    }
}
