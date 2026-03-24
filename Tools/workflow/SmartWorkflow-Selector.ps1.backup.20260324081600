# Smart Workflow Selector
# Auto-detect project status and recommend best workflow

param(
    [switch]$AutoExecute = $false,
    [switch]$Interactive = $false,
    [switch]$NoInteractive = $false,
    [switch]$DryRun = $false,
    [string]$ProjectRoot = ".",
    [string]$Priority = "Balanced"
)

function Write-SmartLog {
    param([string]$Message, [string]$Level = "INFO", [string]$Component = "SMART")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = switch ($Level) {
        "INFO" { "White" }
        "SUCCESS" { "Green" }
        "WARNING" { "Yellow" }
        "ERROR" { "Red" }
        "RECOMMEND" { "Cyan" }
        "ANALYSIS" { "Magenta" }
        default { "White" }
    }
    Write-Host "[$timestamp] [$Component] [$Level] $Message" -ForegroundColor $color
}

class ProjectStateAnalyzer {
    [string]$ProjectRoot
    [hashtable]$State
    
    ProjectStateAnalyzer([string]$root) {
        $this.ProjectRoot = $root
        $this.State = @{
            HasCompileErrors = $false
            HasUncommittedChanges = $false
            HasSecurityIssues = $false
            HasDebugIssues = $false
            RecentCommits = 0
            CodeChangeSize = 0
            LastCompileStatus = "Unknown"
            TestResults = "Unknown"
        }
    }
    
    [void]Analyze() {
        Write-SmartLog "Analyzing project state..." "ANALYSIS"
        $this.CheckCompileStatus()
        $this.CheckGitStatus()
        $this.CheckDebugReports()
        $this.CheckRecentChanges()
        Write-SmartLog "Project state analysis complete" "SUCCESS"
    }
    
    [void]CheckCompileStatus() {
        $compileLogPath = Join-Path $this.ProjectRoot "Logs\Compile_*.log"
        $compileLogs = Get-ChildItem -Path $compileLogPath -ErrorAction SilentlyContinue | 
            Sort-Object LastWriteTime -Descending | Select-Object -First 1
        if ($compileLogs) {
            $content = Get-Content $compileLogs.FullName -Raw -ErrorAction SilentlyContinue
            if ($content -match "ERROR|FAILED") {
                $this.State.HasCompileErrors = $true
                $this.State.LastCompileStatus = "Failed"
                Write-SmartLog "Compile errors detected" "WARNING"
            } else {
                $this.State.LastCompileStatus = "Success"
            }
        }
    }
    
    [void]CheckGitStatus() {
        try {
            $gitStatus = & git -C $this.ProjectRoot status --short 2>$null
            if ($gitStatus) {
                $this.State.HasUncommittedChanges = $true
                $changeCount = ($gitStatus -split "`n").Count
                Write-SmartLog "Found $changeCount uncommitted changes" "WARNING"
            }
            $commitCount = & git -C $this.ProjectRoot rev-list --count HEAD --since="24 hours ago" 2>$null
            $this.State.RecentCommits = [int]$commitCount
        } catch {
            Write-SmartLog "Git check failed" "WARNING"
        }
    }
    
    [void]CheckDebugReports() {
        $debugReportPath = Join-Path $this.ProjectRoot "Tasks\Debug\DebugReport_*.md"
        $debugReports = Get-ChildItem -Path $debugReportPath -ErrorAction SilentlyContinue | 
            Sort-Object LastWriteTime -Descending | Select-Object -First 1
        if ($debugReports) {
            $content = Get-Content $debugReports.FullName -Raw -ErrorAction SilentlyContinue
            if ($content -match "HIGH|CRITICAL|Bug") {
                $this.State.HasDebugIssues = $true
                Write-SmartLog "Unresolved debug issues detected" "WARNING"
            }
        }
    }
    
    [void]CheckRecentChanges() {
        try {
            $diffStat = & git -C $this.ProjectRoot diff --stat HEAD~1 2>$null
            if ($diffStat) {
                $lines = $diffStat -split "`n"
                $this.State.CodeChangeSize = $lines.Count
            }
        } catch { }
    }
    
    [string]GetStatusSummary() {
        $issues = @()
        if ($this.State.HasCompileErrors) { $issues += "Compile Errors" }
        if ($this.State.HasUncommittedChanges) { $issues += "Uncommitted Changes" }
        if ($this.State.HasDebugIssues) { $issues += "Debug Issues" }
        if ($issues.Count -eq 0) { return "Project status is good" }
        return "Issues found: $($issues -join ', ')"
    }
}

class WorkflowRecommender {
    [hashtable]$Workflows
    [string]$Priority
    
    WorkflowRecommender([string]$priority) {
        $this.Priority = $priority
        $this.InitializeWorkflows()
    }
    
    [void]InitializeWorkflows() {
        $this.Workflows = @{
            "Debug" = @{
                Name = "Debug Workflow"
                File = "Debug-Workflow.ps1"
                Priority = 100
                Triggers = @("HasDebugIssues", "HasCompileErrors")
                Description = "Auto bug detection and fix"
                Params = "-AutoFix -DeepScan -ReportToBMAD"
                WhenToUse = "When compile errors or debug issues exist"
            }
            "Compilation" = @{
                Name = "Compilation Workflow"
                File = "Compilation-Workflow.ps1"
                Priority = 90
                Triggers = @("HasCompileErrors", "LastCompileStatus")
                Description = "Auto compilation workflow"
                Params = "-AutoMode -ScanInterval 30"
                WhenToUse = "When compilation needed or errors to fix"
            }
            "VersionControl" = @{
                Name = "Version Control Workflow"
                File = "VersionControl-Workflow.ps1"
                Priority = 80
                Triggers = @("HasUncommittedChanges")
                Description = "Version control workflow"
                Params = "-AutoMode -ScanInterval 60"
                WhenToUse = "When uncommitted code changes exist"
            }
            "CodeReview" = @{
                Name = "Code Review Workflow"
                File = "CodeReview-Workflow.ps1"
                Priority = 70
                Triggers = @("CodeChangeSize")
                Description = "Code review and security scan"
                Params = ""
                WhenToUse = "When important code changes need review"
            }
            "BMAD" = @{
                Name = "BMAD Workflow"
                File = "BMAD_CodingWorkflow.ps1"
                Priority = 60
                Triggers = @()
                Description = "BMAD requirement system management"
                Params = ""
                WhenToUse = "When requirement management needed"
            }
        }
    }
    
    [array]GetRecommendations([hashtable]$state) {
        $scores = @()
        foreach ($wf in $this.Workflows.GetEnumerator()) {
            $score = $this.CalculateScore($wf.Value, $state)
            $scores += [PSCustomObject]@{
                Workflow = $wf.Key
                Name = $wf.Value.Name
                Score = $score
                File = $wf.Value.File
                Params = $wf.Value.Params
                Description = $wf.Value.Description
                WhenToUse = $wf.Value.WhenToUse
            }
        }
        return $scores | Sort-Object Score -Descending
    }
    
    [int]CalculateScore([hashtable]$workflow, [hashtable]$state) {
        $score = $workflow.Priority
        foreach ($trigger in $workflow.Triggers) {
            if ($state[$trigger] -eq $true) { $score += 50 }
            elseif ($trigger -eq "LastCompileStatus" -and $state[$trigger] -eq "Failed") { $score += 50 }
            elseif ($trigger -eq "CodeChangeSize" -and $state[$trigger] -gt 10) { $score += 30 }
        }
        switch ($this.Priority) {
            "Speed" { if ($workflow.Name -match "Compilation|Debug") { $score += 20 } }
            "Quality" { if ($workflow.Name -match "CodeReview|Debug") { $score += 20 } }
        }
        return $score
    }
}

class SmartWorkflowController {
    [ProjectStateAnalyzer]$Analyzer
    [WorkflowRecommender]$Recommender
    [string]$ProjectRoot
    [bool]$AutoExecute
    [bool]$Interactive
    [bool]$DryRun
    
    SmartWorkflowController([string]$root, [bool]$auto, [bool]$interactive, [bool]$dryRun, [string]$priority) {
        $this.ProjectRoot = $root
        $this.AutoExecute = $auto
        $this.Interactive = $interactive
        $this.DryRun = $dryRun
        $this.Analyzer = [ProjectStateAnalyzer]::new($root)
        $this.Recommender = [WorkflowRecommender]::new($priority)
    }
    
    [void]Execute() {
        Write-SmartLog "=== Smart Workflow Selector Started ===" "SUCCESS"
        Write-SmartLog "Project: $($this.ProjectRoot)" "INFO"
        Write-SmartLog "Mode: $(if ($this.AutoExecute) { 'Auto' } else { 'Manual' })" "INFO"
        
        $this.Analyzer.Analyze()
        $this.ShowStateSummary()
        
        $recommendations = $this.Recommender.GetRecommendations($this.Analyzer.State)
        $this.ShowRecommendations($recommendations)
        
        if ($this.DryRun) {
            Write-SmartLog "Dry run mode - showing recommendations only" "WARNING"
            return
        }
        
        if ($this.AutoExecute -and $recommendations[0].Score -gt 0) {
            $this.ExecuteWorkflow($recommendations[0])
        } elseif ($this.Interactive) {
            $this.InteractiveSelection($recommendations)
        } else {
            Write-SmartLog "Use -Interactive to enable selection or -AutoExecute to auto-run" "INFO"
        }
        
        Write-SmartLog "=== Smart Workflow Selection Complete ===" "SUCCESS"
    }
    
    [void]ShowStateSummary() {
        Write-Host "`n=== Project Status Summary ===" -ForegroundColor Cyan
        Write-Host "  Compile Errors: $(if ($this.Analyzer.State.HasCompileErrors) { 'Yes' } else { 'No' })" -ForegroundColor $(if ($this.Analyzer.State.HasCompileErrors) { 'Red' } else { 'Green' })
        Write-Host "  Uncommitted: $(if ($this.Analyzer.State.HasUncommittedChanges) { 'Yes' } else { 'No' })" -ForegroundColor $(if ($this.Analyzer.State.HasUncommittedChanges) { 'Red' } else { 'Green' })
        Write-Host "  Debug Issues: $(if ($this.Analyzer.State.HasDebugIssues) { 'Yes' } else { 'No' })" -ForegroundColor $(if ($this.Analyzer.State.HasDebugIssues) { 'Red' } else { 'Green' })
        Write-Host "  Recent Commits: $($this.Analyzer.State.RecentCommits) (24h)" -ForegroundColor White
        Write-Host "  Code Changes: $($this.Analyzer.State.CodeChangeSize) files" -ForegroundColor White
        Write-Host "  Summary: $($this.Analyzer.GetStatusSummary())" -ForegroundColor Yellow
        Write-Host "=====================" -ForegroundColor Cyan
    }
    
    [void]ShowRecommendations([array]$recommendations) {
        Write-Host "`n=== Workflow Recommendations ===" -ForegroundColor Cyan
        for ($i = 0; $i -lt [Math]::Min(3, $recommendations.Count); $i++) {
            $rec = $recommendations[$i]
            $rank = $i + 1
            $color = switch ($i) { 0 { "Green" } 1 { "Yellow" } 2 { "Gray" } default { "White" } }
            Write-Host "  [$rank] $($rec.Name) - Score: $($rec.Score)" -ForegroundColor $color
            Write-Host "      $($rec.Description)" -ForegroundColor Gray
            if ($rec.Params) { Write-Host "      Params: $($rec.Params)" -ForegroundColor DarkCyan }
            Write-Host ""
        }
    }
    
    [void]ExecuteWorkflow([PSCustomObject]$workflow) {
        $scriptPath = Join-Path $this.ProjectRoot "Tools\workflow\$($workflow.File)"
        if (!(Test-Path $scriptPath)) {
            Write-SmartLog "Workflow script not found: $scriptPath" "ERROR"
            return
        }
        Write-SmartLog "Executing: $($workflow.Name)" "RECOMMEND"
        try {
            $cmd = ".\$($workflow.File)"
            if ($workflow.Params) { $cmd += " $($workflow.Params)" }
            Write-SmartLog "Command: $cmd" "INFO"
            Invoke-Expression $cmd
            Write-SmartLog "Workflow completed" "SUCCESS"
        } catch {
            Write-SmartLog "Workflow failed: $($_.Exception.Message)" "ERROR"
        }
    }
    
    [void]InteractiveSelection([array]$recommendations) {
        Write-Host "`nSelect workflow to execute:" -ForegroundColor Yellow
        Write-Host "  [0] Cancel" -ForegroundColor Red
        for ($i = 0; $i -lt [Math]::Min(5, $recommendations.Count); $i++) {
            Write-Host "  [$($i+1)] $($recommendations[$i].Name)" -ForegroundColor White
        }
        $choice = Read-Host "`nEnter option (0-$([Math]::Min(5, $recommendations.Count)))"
        if ($choice -match '^\d+$' -and [int]$choice -gt 0 -and [int]$choice -le $recommendations.Count) {
            $this.ExecuteWorkflow($recommendations[[int]$choice - 1])
        } else {
            Write-SmartLog "Selection cancelled" "INFO"
        }
    }
}

try {
    $enableInteractive = $Interactive -or (-not $AutoExecute -and -not $DryRun -and -not $NoInteractive)
    $controller = [SmartWorkflowController]::new($ProjectRoot, $AutoExecute, $enableInteractive, $DryRun, $Priority)
    $controller.Execute()
} catch {
    Write-SmartLog "System error: $($_.Exception.Message)" "ERROR"
    exit 1
}
