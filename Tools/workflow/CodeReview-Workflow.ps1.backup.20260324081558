# Code Review Workflow System
# Automated Code Review and Security Scanning for MingGoRTS

# Enhanced logging system
function Write-CodeReviewLog {
    param(
        [string]$Message,
        [string]$Level = "INFO",
        [string]$Component = "CODEREVIEW"
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
    $logFile = "Logs\CodeReview_$(Get-Date -Format 'yyyyMMdd').log"
    if (!(Test-Path "Logs")) { New-Item -ItemType Directory -Path "Logs" | Out-Null }
    Add-Content -Path $logFile -Value $logEntry
}

# Code Review requirement system
class CodeReviewRequirementSystem {
    [System.Collections.Generic.List[PSCustomObject]]$Requirements
    [hashtable]$StatusPriority
    
    CodeReviewRequirementSystem() {
        $this.Requirements = [System.Collections.Generic.List[PSCustomObject]]::new()
        $this.StatusPriority = @{
            "CodeReview" = 1
            "ToOptimize" = 2
            "Done" = 3
        }
        $this.InitializeSampleRequirements()
    }
    
    [void]InitializeSampleRequirements() {
        Write-CodeReviewLog "Initializing sample Code Review requirements..." "INFO" "REQUIREMENTS"
        
        $sampleTasks = @(
            @{ Name = "UI System Enhancement Code"; Priority = 1; Status = "CodeReview"; Complexity = "High"; FilePath = "Source/UI/MingUIManager.cpp" },
            @{ Name = "Audio System Integration Code"; Priority = 2; Status = "CodeReview"; Complexity = "Medium"; FilePath = "Source/Audio/MingAudioManager.cpp" },
            @{ Name = "Network Synchronization Code"; Priority = 1; Status = "CodeReview"; Complexity = "High"; FilePath = "Source/Network/MingNetworkManager.cpp" },
            @{ Name = "Performance Optimization Code"; Priority = 3; Status = "ToOptimize"; Complexity = "Medium"; FilePath = "Source/Performance/MingOptimizer.cpp" },
            @{ Name = "Save/Load System Code"; Priority = 2; Status = "Done"; Complexity = "Medium"; FilePath = "Source/SaveLoad/MingSaveManager.cpp" }
        )
        
        foreach ($task in $sampleTasks) {
            $requirement = [PSCustomObject]@{
                ID = $this.Requirements.Count + 1
                Name = $task.Name
                Priority = $task.Priority
                Status = $task.Status
                Complexity = $task.Complexity
                FilePath = $task.FilePath
                CreatedTime = Get-Date
                UpdatedTime = Get-Date
                AssignedTo = $null
                Progress = 0
                ReviewResults = @{}
                SecurityScanResults = @{}
            }
            $this.Requirements.Add($requirement)
            Write-CodeReviewLog "Added requirement: $($requirement.Name) (Status: $($requirement.Status))" "INFO" "REQUIREMENTS"
        }
    }
    
    [System.Collections.Generic.List[PSCustomObject]]GetTasksByStatus([string]$Status) {
        return $this.Requirements | Where-Object { $_.Status -eq $Status }
    }
    
    [System.Collections.Generic.List[PSCustomObject]]GetCodeReviewTasks() {
        return $this.Requirements | Where-Object { $_.Status -eq "CodeReview" } | Sort-Object { $_.Priority }
    }
    
    [void]UpdateRequirementStatus([int]$ID, [string]$NewStatus, [string]$AssignedTo = $null) {
        $requirement = $this.Requirements | Where-Object { $_.ID -eq $ID }
        if ($requirement) {
            $oldStatus = $requirement.Status
            $requirement.Status = $NewStatus
            $requirement.UpdatedTime = Get-Date
            if ($AssignedTo) { $requirement.AssignedTo = $AssignedTo }
            
            Write-CodeReviewLog "Task [$ID] status updated: $oldStatus -> $NewStatus" "SUCCESS" "REQUIREMENTS"
            Write-CodeReviewLog "Task: $($requirement.Name) | Assigned: $($requirement.AssignedTo)" "INFO" "REQUIREMENTS"
        } else {
            Write-CodeReviewLog "Task ID $ID not found" "ERROR" "REQUIREMENTS"
        }
    }
    
    [void]UpdateProgress([int]$ID, [int]$Progress) {
        $requirement = $this.Requirements | Where-Object { $_.ID -eq $ID }
        if ($requirement) {
            $requirement.Progress = [Math]::Min(100, [Math]::Max(0, $Progress))
            Write-CodeReviewLog "Task [$ID] progress: $($requirement.Progress)%" "INFO" "REQUIREMENTS"
        }
    }
    
    [void]StoreReviewResults([int]$ID, [hashtable]$Results) {
        $requirement = $this.Requirements | Where-Object { $_.ID -eq $ID }
        if ($requirement) {
            $requirement.ReviewResults = $Results
            Write-CodeReviewLog "Stored review results for task [$ID]" "INFO" "REQUIREMENTS"
        }
    }
    
    [void]StoreSecurityScanResults([int]$ID, [hashtable]$Results) {
        $requirement = $this.Requirements | Where-Object { $_.ID -eq $ID }
        if ($requirement) {
            $requirement.SecurityScanResults = $Results
            Write-CodeReviewLog "Stored security scan results for task [$ID]" "INFO" "REQUIREMENTS"
        }
    }
}

# Code Standards Checker
class CodeStandardsChecker {
    [hashtable]$StandardsRules
    
    CodeStandardsChecker() {
        $this.StandardsRules = @{
            "NamingConvention" = $true
            "Documentation" = $true
            "CodeStructure" = $true
            "ErrorHandling" = $true
            "Performance" = $true
            "Security" = $true
        }
    }
    
    [hashtable]CheckCodeStandards([string]$FilePath) {
        Write-CodeReviewLog "Checking code standards for: $FilePath" "INFO" "STANDARDS"
        
        $results = @{
            "Passed" = $true
            "Issues" = @()
            "Score" = 0
            "MaxScore" = 100
        }
        
        if (!(Test-Path $FilePath)) {
            $results.Passed = $false
            $results.Issues += "File not found: $FilePath"
            $results.Score = 0
            return $results
        }
        
        try {
            $content = Get-Content -Path $FilePath -Raw
            
            # Check naming conventions
            $namingIssues = $this.CheckNamingConventions($content)
            $results.Issues += $namingIssues
            
            # Check documentation
            $docIssues = $this.CheckDocumentation($content)
            $results.Issues += $docIssues
            
            # Check code structure
            $structureIssues = $this.CheckCodeStructure($content)
            $results.Issues += $structureIssues
            
            # Check error handling
            $errorIssues = $this.CheckErrorHandling($content)
            $results.Issues += $errorIssues
            
            # Check performance patterns
            $performanceIssues = $this.CheckPerformancePatterns($content)
            $results.Issues += $performanceIssues
            
            # Calculate score (subtract 10 points per issue)
            $results.Score = [Math]::Max(0, $results.MaxScore - ($results.Issues.Count * 10))
            $results.Passed = $results.Score -ge 70  # Pass threshold is 70%
            
            Write-CodeReviewLog "Code standards check completed. Score: $($results.Score)/100" "INFO" "STANDARDS"
        }
        catch {
            $results.Passed = $false
            $results.Issues += "Error reading file: $($_.Exception.Message)"
            $results.Score = 0
            Write-CodeReviewLog "Error checking code standards: $($_.Exception.Message)" "ERROR" "STANDARDS"
        }
        
        return $results
    }
    
    [string[]]CheckNamingConventions([string]$Content) {
        $issues = @()
        
        # Check for proper function naming (PascalCase for C++ functions)
        if ($Content -match '\b[a-z][a-zA-Z0-9]*\s*\(') {
            $issues += "Function names should follow PascalCase convention"
        }
        
        # Check for proper variable naming (camelCase)
        if ($Content -match '\b[A-Z][a-zA-Z0-9]*\s*=') {
            $issues += "Variable names should follow camelCase convention"
        }
        
        # Check for proper class naming (PascalCase)
        if ($Content -match 'class\s+[a-z]') {
            $issues += "Class names should follow PascalCase convention"
        }
        
        return $issues
    }
    
    [string[]]CheckDocumentation([string]$Content) {
        $issues = @()
        
        # Check for function documentation
        if ($Content -match '^\s*[a-zA-Z_][a-zA-Z0-9_]*\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\([^)]*\)\s*\{' -and 
            $Content -notmatch '/\*\*.*?\*/') {
            $issues += "Functions should have documentation comments"
        }
        
        # Check for class documentation
        if ($Content -match 'class\s+\w+' -and $Content -notmatch '/\*\*.*?class.*?\*/') {
            $issues += "Classes should have documentation comments"
        }
        
        return $issues
    }
    
    [string[]]CheckCodeStructure([string]$Content) {
        $issues = @()
        
        # Check for long functions (> 50 lines)
        $functions = [regex]::Matches($Content, '\{[\s\S]*?\}')
        foreach ($func in $functions) {
            $lines = ($func.Value -split "`n").Count
            if ($lines -gt 50) {
                $issues += "Function exceeds 50 lines, consider breaking it down"
            }
        }
        
        # Check for deep nesting (> 4 levels)
        $nestingLevel = 0
        $maxNesting = 0
        foreach ($char in $Content.ToCharArray()) {
            if ($char -eq '{') { 
                $nestingLevel++
                $maxNesting = [Math]::Max($maxNesting, $nestingLevel)
            }
            elseif ($char -eq '}') { 
                $nestingLevel--
            }
        }
        if ($maxNesting -gt 4) {
            $issues += "Code nesting level exceeds 4, consider refactoring"
        }
        
        return $issues
    }
    
    [string[]]CheckErrorHandling([string]$Content) {
        $issues = @()
        
        # Check for proper exception handling in C++
        if ($Content -match 'new\s+\w+' -and $Content -notmatch 'try\s*\{') {
            $issues += "Memory allocation should be wrapped in try-catch blocks"
        }
        
        # Check for null pointer checks
        if ($Content -match '->' -and $Content -notmatch 'if\s*\([^)]*\s*==\s*nullptr\)') {
            $issues += "Pointer access should be preceded by null checks"
        }
        
        return $issues
    }
    
    [string[]]CheckPerformancePatterns([string]$Content) {
        $issues = @()
        
        # Check for inefficient loops
        if ($Content -match 'for\s*\([^)]*\.size\(\)[^)]*\)') {
            $issues += "Consider caching .size() result in loop conditions"
        }
        
        # Check for unnecessary copies
        if ($Content -match 'std::string\s+\w+\s*=\s*\w+\s*;') {
            $issues += "Consider using const reference for string assignments"
        }
        
        return $issues
    }
}

# Security Vulnerability Scanner
class SecurityVulnerabilityScanner {
    [hashtable]$VulnerabilityPatterns
    
    SecurityVulnerabilityScanner() {
        $this.VulnerabilityPatterns = @{
            "SQLInjection" = @("execute\s*\(", "query\s*\(")
            "XSS" = @("innerHTML\s*=", "document\.write")
            "BufferOverflow" = @("strcpy\s*\(", "gets\s*\(", "sprintf\s*\(")
            "HardcodedPassword" = @("password\s*=\s*`"", "pwd\s*=\s*`"")
            "InsecureRandom" = @("rand\s*\(", "srand\s*\(")
            "PathTraversal" = @("\.\.\/", "\.\.\\")
            "CommandInjection" = @("system\s*\(", "exec\s*\(", "shell_exec")
        }
    }
    
    [hashtable]ScanForVulnerabilities([string]$FilePath) {
        Write-CodeReviewLog "Scanning for security vulnerabilities in: $FilePath" "INFO" "SECURITY"
        
        $results = @{
            "Passed" = $true
            "Vulnerabilities" = @()
            "Score" = 100
            "MaxScore" = 100
        }
        
        if (!(Test-Path $FilePath)) {
            $results.Passed = $false
            $results.Vulnerabilities += @{ "Type" = "FileError"; "Description" = "File not found: $FilePath"; "Severity" = "High" }
            $results.Score = 0
            return $results
        }
        
        try {
            $content = Get-Content -Path $FilePath -Raw
            
            foreach ($pattern in $this.VulnerabilityPatterns.GetEnumerator()) {
                $vulnerabilityType = $pattern.Key
                $patterns = $pattern.Value
                
                foreach ($pat in $patterns) {
                    $patternMatches = [regex]::Matches($content, $pat, "IgnoreCase")
                    foreach ($match in $patternMatches) {
                        $lineNumber = ($content.Substring(0, $match.Index).ToCharArray() | Where-Object { $_ -eq "`n" }).Count + 1
                        
                        $severity = switch ($vulnerabilityType) {
                            "SQLInjection" { "Critical" }
                            "BufferOverflow" { "Critical" }
                            "HardcodedPassword" { "High" }
                            "CommandInjection" { "Critical" }
                            "PathTraversal" { "High" }
                            "XSS" { "Medium" }
                            "InsecureRandom" { "Low" }
                            default { "Medium" }
                        }
                        
                        $scoreDeduction = switch ($severity) {
                            "Critical" { 25 }
                            "High" { 15 }
                            "Medium" { 10 }
                            "Low" { 5 }
                            default { 10 }
                        }
                        
                        $results.Vulnerabilities += @{
                            "Type" = $vulnerabilityType
                            "Description" = "Potential $($vulnerabilityType) vulnerability detected"
                            "Line" = $lineNumber
                            "Match" = $match.Value
                            "Severity" = $severity
                            "ScoreDeduction" = $scoreDeduction
                        }
                        
                        $results.Score = [Math]::Max(0, $results.Score - $scoreDeduction)
                    }
                }
            }
            
            $results.Passed = $results.Score -ge 80  # Security threshold is 80%
            
            Write-CodeReviewLog "Security scan completed. Score: $($results.Score)/100" "INFO" "SECURITY"
            Write-CodeReviewLog "Found $($results.Vulnerabilities.Count) potential vulnerabilities" "INFO" "SECURITY"
        }
        catch {
            $results.Passed = $false
            $results.Vulnerabilities += @{ "Type" = "ScanError"; "Description" = "Error scanning file: $($_.Exception.Message)"; "Severity" = "High" }
            $results.Score = 0
            Write-CodeReviewLog "Error scanning for vulnerabilities: $($_.Exception.Message)" "ERROR" "SECURITY"
        }
        
        return $results
    }
}

# Code Review Workflow Engine
class CodeReviewWorkflowEngine {
    [CodeReviewRequirementSystem]$RequirementSystem
    [CodeStandardsChecker]$StandardsChecker
    [SecurityVulnerabilityScanner]$SecurityScanner
    [hashtable]$ReviewerPool
    [bool]$AutoMode
    
    CodeReviewWorkflowEngine([CodeReviewRequirementSystem]$ReqSystem, [bool]$AutoMode = $true) {
        $this.RequirementSystem = $ReqSystem
        $this.AutoMode = $AutoMode
        $this.StandardsChecker = [CodeStandardsChecker]::new()
        $this.SecurityScanner = [SecurityVulnerabilityScanner]::new()
        $this.InitializeReviewerPool()
    }
    
    [void]InitializeReviewerPool() {
        $this.ReviewerPool = @{
            "Reviewer-Alice" = @{ Status = "Available"; Expertise = @("UI", "Audio"); CurrentTask = $null }
            "Reviewer-Bob" = @{ Status = "Available"; Expertise = @("Network", "Security"); CurrentTask = $null }
            "Reviewer-Charlie" = @{ Status = "Available"; Expertise = @("Performance", "Backend"); CurrentTask = $null }
        }
        Write-CodeReviewLog "Reviewer pool initialized with $($this.ReviewerPool.Count) reviewers" "INFO" "WORKFLOW"
    }
    
    [string]AssignReviewer([PSCustomObject]$Task) {
        $availableReviewers = $this.ReviewerPool.GetEnumerator() | 
            Where-Object { $_.Value.Status -eq "Available" }
        
        if ($availableReviewers.Count -eq 0) {
            Write-CodeReviewLog "No available reviewers for task: $($Task.Name)" "WARNING" "WORKFLOW"
            return $null
        }
        
        $assignedReviewer = $availableReviewers | Select-Object -First 1
        $reviewerName = $assignedReviewer.Key
        $this.ReviewerPool[$reviewerName].Status = "Busy"
        $this.ReviewerPool[$reviewerName].CurrentTask = $Task
        
        Write-CodeReviewLog "Assigned task '$($Task.Name)' to reviewer $reviewerName" "SUCCESS" "WORKFLOW"
        return $reviewerName
    }
    
    [void]ReleaseReviewer([string]$ReviewerName) {
        if ($this.ReviewerPool.ContainsKey($ReviewerName)) {
            $this.ReviewerPool[$ReviewerName].Status = "Available"
            $this.ReviewerPool[$ReviewerName].CurrentTask = $null
            Write-CodeReviewLog "Released reviewer: $ReviewerName" "INFO" "WORKFLOW"
        }
    }
    
    [void]ProcessCodeReview([PSCustomObject]$Task, [string]$ReviewerName) {
        Write-CodeReviewLog "Starting code review: $($Task.Name) by $ReviewerName" "INFO" "REVIEW"
        
        # Step 1: Code Standards Check
        Write-CodeReviewLog "  [$($Task.ID)] Performing code standards check..." "DEBUG" "REVIEW"
        $this.RequirementSystem.UpdateProgress($Task.ID, 20)
        
        $standardsResults = $this.StandardsChecker.CheckCodeStandards($Task.FilePath)
        $this.RequirementSystem.StoreReviewResults($Task.ID, $standardsResults)
        
        # Step 2: Security Vulnerability Scan
        Write-CodeReviewLog "  [$($Task.ID)] Performing security vulnerability scan..." "DEBUG" "REVIEW"
        $this.RequirementSystem.UpdateProgress($Task.ID, 60)
        
        $securityResults = $this.SecurityScanner.ScanForVulnerabilities($Task.FilePath)
        $this.RequirementSystem.StoreSecurityScanResults($Task.ID, $securityResults)
        
        # Step 3: Final Evaluation
        Write-CodeReviewLog "  [$($Task.ID)] Evaluating review results..." "DEBUG" "REVIEW"
        $this.RequirementSystem.UpdateProgress($Task.ID, 80)
        
        $overallPassed = $standardsResults.Passed -and $securityResults.Passed
        $overallScore = [Math]::Round(($standardsResults.Score + $securityResults.Score) / 2)
        
        # Generate detailed report
        $report = @"
Code Review Report for: $($Task.Name)
==========================================
Reviewer: $ReviewerName
File: $($Task.FilePath)
Overall Score: $overallScore/100
Status: $(if ($overallPassed) { "PASSED" } else { "FAILED" })

Code Standards Check:
- Score: $($standardsResults.Score)/100
- Status: $(if ($standardsResults.Passed) { "PASSED" } else { "FAILED" })
- Issues Found: $($standardsResults.Issues.Count)
$(if ($standardsResults.Issues.Count -gt 0) { 
    "Issues:" + ($standardsResults.Issues | ForEach-Object { "  - $_" } | Out-String)
})

Security Vulnerability Scan:
- Score: $($securityResults.Score)/100
- Status: $(if ($securityResults.Passed) { "PASSED" } else { "FAILED" })
- Vulnerabilities Found: $($securityResults.Vulnerabilities.Count)
$(if ($securityResults.Vulnerabilities.Count -gt 0) { 
    "Vulnerabilities:" + ($securityResults.Vulnerabilities | ForEach-Object { 
        "  - [$($_.Severity)] $($_.Type) at line $($_.Line): $($_.Description)"
    } | Out-String)
})

==========================================
"@
        
        Write-CodeReviewLog $report "INFO" "REPORT"
        $this.RequirementSystem.UpdateProgress($Task.ID, 100)
        
        # Update task status based on results
        if ($overallPassed) {
            Write-CodeReviewLog "Code review PASSED for task: $($Task.Name)" "SUCCESS" "REVIEW"
            $this.RequirementSystem.UpdateRequirementStatus($Task.ID, "Done", $ReviewerName)
        } else {
            Write-CodeReviewLog "Code review FAILED for task: $($Task.Name)" "WARNING" "REVIEW"
            $this.RequirementSystem.UpdateRequirementStatus($Task.ID, "ToOptimize", $ReviewerName)
        }
        
        # Release reviewer
        $this.ReleaseReviewer($ReviewerName)
    }
    
    [void]ExecuteCodeReviewCycle() {
        Write-CodeReviewLog "=== Starting Code Review Workflow Cycle ===" "INFO" "WORKFLOW"
        
        $codeReviewTasks = $this.RequirementSystem.GetCodeReviewTasks()
        
        if ($codeReviewTasks.Count -eq 0) {
            Write-CodeReviewLog "No Code Review tasks found" "INFO" "WORKFLOW"
            return
        }
        
        Write-CodeReviewLog "Found $($codeReviewTasks.Count) Code Review tasks" "INFO" "WORKFLOW"
        
        foreach ($task in $codeReviewTasks) {
            # Check if we have available reviewers
            $availableReviewers = $this.ReviewerPool.GetEnumerator() | 
                Where-Object { $_.Value.Status -eq "Available" }
            
            if ($availableReviewers.Count -eq 0) {
                Write-CodeReviewLog "No available reviewers, skipping remaining tasks" "WARNING" "WORKFLOW"
                break
            }
            
            # Assign reviewer and process task
            $assignedReviewer = $this.AssignReviewer($task)
            if ($assignedReviewer) {
                $this.ProcessCodeReview($task, $assignedReviewer)
            }
        }
        
        Write-CodeReviewLog "=== Code Review Workflow Cycle Completed ===" "INFO" "WORKFLOW"
    }
    
    [void]ShowCodeReviewStatus() {
        Write-Host "`n=== Code Review Workflow Status ===" -ForegroundColor Cyan
        
        # Task status summary
        $statusSummary = $this.RequirementSystem.Requirements | Group-Object -Property Status
        Write-Host "`nTask Status Summary:" -ForegroundColor Yellow
        foreach ($group in $statusSummary) {
            $color = switch ($group.Name) {
                "Done" { "Green" }
                "CodeReview" { "Blue" }
                "ToOptimize" { "Yellow" }
                default { "Gray" }
            }
            Write-Host "  [$($group.Name.PadRight(12))] $($group.Count) tasks" -ForegroundColor $color
        }
        
        # Reviewer status
        Write-Host "`nReviewer Status:" -ForegroundColor Yellow
        foreach ($reviewer in $this.ReviewerPool.GetEnumerator()) {
            $statusColor = if ($reviewer.Value.Status -eq "Available") { "Green" } else { "Red" }
            $currentTask = if ($reviewer.Value.CurrentTask) { " - $($reviewer.Value.CurrentTask.Name)" } else { "" }
            Write-Host "  $($reviewer.Key.PadRight(20)): $($reviewer.Value.Status)$currentTask" -ForegroundColor $statusColor
        }
        
        # Pending Code Review tasks
        $pendingTasks = $this.RequirementSystem.GetCodeReviewTasks()
        if ($pendingTasks.Count -gt 0) {
            Write-Host "`nNext Code Review Tasks:" -ForegroundColor Yellow
            for ($i = 0; $i -lt [Math]::Min(5, $pendingTasks.Count); $i++) {
                $task = $pendingTasks[$i]
                Write-Host "  [$($task.ID)] $($task.Name) (Priority: $($task.Priority), File: $($task.FilePath))" -ForegroundColor White
            }
        }
        
        Write-Host "`n======================================" -ForegroundColor Cyan
    }
    
    [bool]AllCodeReviewsCompleted() {
        $completedTasks = $this.RequirementSystem.Requirements | Where-Object { $_.Status -eq "Done" }
        return $completedTasks.Count -eq $this.RequirementSystem.Requirements.Count
    }
}

# Main Code Review workflow controller
class CodeReviewWorkflowController {
    [CodeReviewRequirementSystem]$RequirementSystem
    [CodeReviewWorkflowEngine]$WorkflowEngine
    [bool]$AutoMode
    [int]$WorkflowInterval
    
    CodeReviewWorkflowController([bool]$AutoMode = $true, [int]$Interval = 30) {
        $this.RequirementSystem = [CodeReviewRequirementSystem]::new()
        $this.WorkflowEngine = [CodeReviewWorkflowEngine]::new($this.RequirementSystem, $AutoMode)
        $this.AutoMode = $AutoMode
        $this.WorkflowInterval = $Interval
        
        Write-CodeReviewLog "Code Review Workflow Controller initialized" "SUCCESS" "CONTROLLER"
        Write-CodeReviewLog "Auto Mode: $($this.AutoMode)" "INFO" "CONTROLLER"
        Write-CodeReviewLog "Workflow Interval: $($this.WorkflowInterval) seconds" "INFO" "CONTROLLER"
    }
    
    [void]StartCodeReviewWorkflow() {
        Write-CodeReviewLog "=== Starting Code Review Workflow ===" "SUCCESS" "CONTROLLER"
        
        if ($this.AutoMode) {
            Write-CodeReviewLog "Auto mode enabled - running continuous Code Review workflow" "INFO" "CONTROLLER"
            
            while ($true) {
                try {
                    $this.WorkflowEngine.ExecuteCodeReviewCycle()
                    $this.WorkflowEngine.ShowCodeReviewStatus()
                    
                    if ($this.WorkflowEngine.AllCodeReviewsCompleted()) {
                        Write-CodeReviewLog "All Code Reviews completed! Workflow finished." "SUCCESS" "CONTROLLER"
                        break
                    }
                    
                    Write-CodeReviewLog "Waiting $($this.WorkflowInterval) seconds before next cycle..." "INFO" "CONTROLLER"
                    Start-Sleep -Seconds $this.WorkflowInterval
                }
                catch {
                    Write-CodeReviewLog "Workflow error: $($_.Exception.Message)" "ERROR" "CONTROLLER"
                    Start-Sleep -Seconds 10
                }
            }
        } else {
            Write-CodeReviewLog "Manual mode enabled - running single Code Review cycle" "INFO" "CONTROLLER"
            $this.WorkflowEngine.ExecuteCodeReviewCycle()
            $this.WorkflowEngine.ShowCodeReviewStatus()
        }
        
        Write-CodeReviewLog "=== Code Review Workflow Completed ===" "SUCCESS" "CONTROLLER"
    }
    
    [void]AddCodeReviewTask([string]$Name, [int]$Priority, [string]$FilePath, [string]$Complexity = "Medium") {
        $task = [PSCustomObject]@{
            ID = $this.RequirementSystem.Requirements.Count + 1
            Name = $Name
            Priority = $Priority
            Status = "CodeReview"
            Complexity = $Complexity
            FilePath = $FilePath
            CreatedTime = Get-Date
            UpdatedTime = Get-Date
            AssignedTo = $null
            Progress = 0
            ReviewResults = @{}
            SecurityScanResults = @{}
        }
        
        $this.RequirementSystem.Requirements.Add($task)
        Write-CodeReviewLog "New Code Review task added: $($task.Name) (Priority: $($task.Priority))" "SUCCESS" "CONTROLLER"
    }
}

# Simple parameter handling
$ManualMode = $false
$StatusOnly = $false
$IntervalValue = 30
$TaskToAdd = ""
$TaskPriority = 3
$TaskFilePath = ""

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
        "-FilePath" {
            if ($i + 1 -lt $args.Count) {
                $TaskFilePath = $args[$i + 1]
                $i++
            }
        }
    }
}

try {
    # Create Code Review workflow controller
    $controller = [CodeReviewWorkflowController]::new(-not $ManualMode, $IntervalValue)
    
    # Handle commands
    if ($TaskToAdd -and $TaskFilePath) {
        $controller.AddCodeReviewTask($TaskToAdd, $TaskPriority, $TaskFilePath)
        Write-Host "Code Review task added successfully" -ForegroundColor Green
        exit 0
    }
    
    if ($StatusOnly) {
        $controller.WorkflowEngine.ShowCodeReviewStatus()
        exit 0
    }
    
    # Start Code Review workflow
    $controller.StartCodeReviewWorkflow()
}
catch {
    Write-CodeReviewLog "Fatal error: $($_.Exception.Message)" "ERROR" "SYSTEM"
    exit 1
}
