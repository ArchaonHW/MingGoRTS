# Smart Workflow Selector - Simplified Version
# Analyzes project state and recommends workflows

param(
    [string]$ProjectRoot = "f:\MingGoRTS",
    [switch]$AutoExecute = $false
)

function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "HH:mm:ss"
    $color = switch ($Level) {
        "INFO" { "White" }
        "SUCCESS" { "Green" }
        "WARNING" { "Yellow" }
        "ERROR" { "Red" }
        "WORKFLOW" { "Cyan" }
    }
    Write-Host "[$timestamp] [$Level] $Message" -ForegroundColor $color
}

function Get-ProjectStatus {
    param([string]$ProjectPath)
    
    $status = @{
        HasUncommittedChanges = $false
        HasCompilationErrors = $false
        HasMemoryLeaks = $false
        HasTODOItems = $false
        TotalFiles = 0
        GitStatus = ""
    }
    
    # Check git status
    $gitStatus = & git -C $ProjectPath status --short 2>$null
    if ($gitStatus) {
        $status.HasUncommittedChanges = $true
        $status.GitStatus = $gitStatus
    }
    
    # Count source files
    $sourceFiles = Get-ChildItem -Path "$ProjectPath\Source" -Filter "*.cpp" -Recurse -ErrorAction SilentlyContinue
    $status.TotalFiles = $sourceFiles.Count
    
    # Check for TODO comments
    $todoCount = 0
    foreach ($file in $sourceFiles) {
        $content = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
        if ($content -match "TODO|FIXME|HACK") {
            $todoCount++
        }
    }
    $status.HasTODOItems = $todoCount -gt 0
    
    # Check for memory leak patterns
    $memoryLeakFiles = @()
    foreach ($file in $sourceFiles) {
        $content = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
        if ($content -match "new\s+\w+" -and $content -notmatch "delete|TSharedPtr|MakeShareable|UPROPERTY") {
            $memoryLeakFiles += $file.Name
        }
    }
    $status.HasMemoryLeaks = $memoryLeakFiles.Count -gt 0
    
    return $status
}

function Show-WorkflowMenu {
    Write-Log "Available Workflows:" "WORKFLOW"
    Write-Host ""
    Write-Host "  1. Version Control Workflow" -ForegroundColor Cyan
    Write-Host "     - Stage and commit uncommitted changes" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  2. Debug Workflow" -ForegroundColor Cyan
    Write-Host "     - Scan for memory leaks and TODO items" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  3. Compilation Workflow" -ForegroundColor Cyan
    Write-Host "     - Check compilation status and logs" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  4. Batch Processing Workflow" -ForegroundColor Cyan
    Write-Host "     - Process multiple TODO/FIXME items" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  5. Generate TODO Tasks" -ForegroundColor Cyan
    Write-Host "     - Scan and generate BMAD tasks from TODOs" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  6. Import TODO to BMAD" -ForegroundColor Cyan
    Write-Host "     - Import generated TODO tasks into BMAD system" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  7. Exit" -ForegroundColor Red
    Write-Host ""
}

function Invoke-Workflow {
    param([int]$Choice, [string]$ProjectRoot)
    
    switch ($Choice) {
        1 {
            Write-Log "Executing Version Control Workflow..." "WORKFLOW"
            $scriptPath = Join-Path $PSScriptRoot "VersionControl-Simple.ps1"
            if (Test-Path $scriptPath) {
                & $scriptPath -ProjectRoot $ProjectRoot
            } else {
                Write-Log "Script not found: $scriptPath" "ERROR"
            }
        }
        2 {
            Write-Log "Executing Debug Workflow..." "WORKFLOW"
            $scriptPath = Join-Path $PSScriptRoot "Debug-Workflow-Simple.ps1"
            if (Test-Path $scriptPath) {
                & $scriptPath -Analyze -GenerateReport
            } else {
                Write-Log "Script not found: $scriptPath" "ERROR"
            }
        }
        3 {
            Write-Log "Executing Compilation Workflow..." "WORKFLOW"
            $scriptPath = Join-Path $PSScriptRoot "Compilation-Simple.ps1"
            if (Test-Path $scriptPath) {
                & $scriptPath -ProjectRoot $ProjectRoot
            } else {
                Write-Log "Script not found: $scriptPath" "ERROR"
            }
        }
        4 {
            Write-Log "Executing Batch Processing Workflow..." "WORKFLOW"
            $scriptPath = Join-Path $PSScriptRoot "BatchProcessing-Simple.ps1"
            if (Test-Path $scriptPath) {
                & $scriptPath -ProjectRoot $ProjectRoot
            } else {
                Write-Log "Script not found: $scriptPath" "ERROR"
            }
        }
        5 {
            Write-Log "Generating TODO Tasks..." "WORKFLOW"
            $scriptPath = Join-Path $PSScriptRoot "Generate-TODOTasks.ps1"
            if (Test-Path $scriptPath) {
                & $scriptPath -ProjectRoot $ProjectRoot
            } else {
                Write-Log "Script not found: $scriptPath" "ERROR"
            }
        }
        6 {
            Write-Log "Importing TODO to BMAD..." "WORKFLOW"
            $scriptPath = Join-Path $PSScriptRoot "Import-TODOToBMAD.ps1"
            if (Test-Path $scriptPath) {
                & $scriptPath -ProjectRoot $ProjectRoot
            } else {
                Write-Log "Script not found: $scriptPath" "ERROR"
            }
        }
        default {
            Write-Log "Invalid selection" "ERROR"
        }
    }
}

# Main execution
Write-Log "=== Smart Workflow Selector ===" "SUCCESS"
Write-Log "Project: $ProjectRoot" "INFO"
Write-Host ""

# Analyze project status
Write-Log "Analyzing project status..." "INFO"
$status = Get-ProjectStatus -ProjectPath $ProjectRoot

Write-Host ""
Write-Log "Project Analysis Results:" "WORKFLOW"
Write-Host "  Source Files: $($status.TotalFiles)" -ForegroundColor White
Write-Host "  Uncommitted Changes: $(if ($status.HasUncommittedChanges) { 'Yes' } else { 'No' })" -ForegroundColor $(if ($status.HasUncommittedChanges) { "Yellow" } else { "Green" })
Write-Host "  TODO Items Found: $(if ($status.HasTODOItems) { 'Yes' } else { 'No' })" -ForegroundColor $(if ($status.HasTODOItems) { "Yellow" } else { "Green" })
Write-Host "  Memory Leak Patterns: $(if ($status.HasMemoryLeaks) { 'Yes' } else { 'No' })" -ForegroundColor $(if ($status.HasMemoryLeaks) { "Yellow" } else { "Green" })
Write-Host ""

# Recommendations
Write-Log "Recommendations:" "WORKFLOW"
if ($status.HasUncommittedChanges) {
    Write-Host "  [HIGH] Run Version Control Workflow to commit changes" -ForegroundColor Red
}
if ($status.HasTODOItems) {
    Write-Host "  [MEDIUM] Run Debug Workflow to check TODO items" -ForegroundColor Yellow
}
if ($status.HasMemoryLeaks) {
    Write-Host "  [MEDIUM] Run Debug Workflow to check memory leaks" -ForegroundColor Yellow
}
if (!$status.HasUncommittedChanges -and !$status.HasTODOItems -and !$status.HasMemoryLeaks) {
    Write-Host "  [INFO] Project looks healthy! Consider running Batch Processing for optimization" -ForegroundColor Green
}
Write-Host ""

# Show menu
Show-WorkflowMenu

# Get user selection
$selection = Read-Host "Enter your choice (1-7)"

if ($selection -match '^[1-7]$') {
    if ($selection -eq '7') {
        Write-Log "Exiting Smart Workflow Selector" "INFO"
    } else {
        Execute-Workflow -Choice ([int]$selection) -ProjectRoot $ProjectRoot
    }
} else {
    Write-Log "Invalid selection. Please enter a number 1-7." "ERROR"
}

Write-Log "Smart Workflow Selector Complete" "SUCCESS"
