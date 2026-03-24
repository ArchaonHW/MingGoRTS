# Import TODO Tasks to BMAD System
param(
    [string]$ProjectRoot = "f:\MingGoRTS",
    [string]$TaskListPath = "",
    [switch]$DryRun = $false,
    [switch]$SkipBackup = $false
)

function Write-ImportLog {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = switch ($Level) { "INFO" { "White" } "SUCCESS" { "Green" } "WARNING" { "Yellow" } "ERROR" { "Red" } }
    Write-Host "[$timestamp] [$Level] $Message" -ForegroundColor $color
}

function Parse-TODOTaskList {
    param([string]$filePath)
    
    Write-ImportLog "Parsing task list: $filePath" "INFO"
    
    $content = Get-Content $filePath -Raw
    $tasks = @()
    
    $taskPattern = '### BMAD-TODO-(\d+)\s*\r?\n.*?\*\*Category:\*\* (.+?)\s*\r?\n.*?\*\*File:\*\* (.+?):(\d+)\s*\r?\n.*?\*\*Type:\*\* (.+?)\s*\r?\n.*?\*\*Description:\*\* (.+?)\s*\r?\n.*?\*\*Estimated Hours:\*\* (\d+)'
    
    $matches = [regex]::Matches($content, $taskPattern, [System.Text.RegularExpressions.RegexOptions]::Singleline)
    
    foreach ($match in $matches) {
        $task = [PSCustomObject]@{
            ID = [int]$match.Groups[1].Value
            Category = $match.Groups[2].Value.Trim()
            FilePath = $match.Groups[3].Value.Trim()
            LineNumber = [int]$match.Groups[4].Value
            Type = $match.Groups[5].Value.Trim()
            Description = $match.Groups[6].Value.Trim()
            EstimatedHours = [int]$match.Groups[7].Value
            Status = "Pending"
            AssignedTo = "Unassigned"
        }
        $tasks += $task
    }
    
    Write-ImportLog "Parsed $($tasks.Count) tasks from file" "SUCCESS"
    return $tasks
}

function Convert-ToBMADTask {
    param($todoTask, $baseId)
    
    $bmadPriority = switch ($todoTask.Type) {
        "FIXME" { 5 }
        "XXX" { 4 }
        "HACK" { 3 }
        "TODO" { 2 }
        default { 1 }
    }
    
    $taskName = "TODO-$($todoTask.ID): $($todoTask.Description)"
    if ($taskName.Length -gt 80) {
        $taskName = $taskName.Substring(0, 77) + "..."
    }
    
    return [PSCustomObject]@{
        ID = $baseId + $todoTask.ID
        Name = $taskName
        Category = $todoTask.Category
        OriginalFile = $todoTask.FilePath
        LineNumber = $todoTask.LineNumber
        OriginalType = $todoTask.Type
        Description = $todoTask.Description
        Status = "Pending"
        Priority = $bmadPriority
        EstimatedHours = $todoTask.EstimatedHours
        AssignedTo = ""
        CreatedTime = Get-Date
        UpdatedTime = Get-Date
        Progress = 0
        SourceTaskID = "BMAD-TODO-$($todoTask.ID)"
    }
}

Write-ImportLog "=== BMAD TODO Task Import System ===" "SUCCESS"

# Find the task list file
if (!$TaskListPath) {
    $reportsDir = Join-Path $ProjectRoot "Reports"
    $latestTaskList = Get-ChildItem -Path $reportsDir -Filter "BMAD_TODO_Tasks_*.md" | 
        Sort-Object LastWriteTime -Descending | 
        Select-Object -First 1
    
    if (!$latestTaskList) {
        Write-ImportLog "No BMAD TODO task list found. Run Generate-TODOTasks.ps1 first." "ERROR"
        exit 1
    }
    
    $TaskListPath = $latestTaskList.FullName
}

Write-ImportLog "Importing from: $TaskListPath" "INFO"

# Parse tasks
$parsedTasks = Parse-TODOTaskList $TaskListPath

if ($parsedTasks.Count -eq 0) {
    Write-ImportLog "No tasks found to import" "WARNING"
    exit 0
}

# Get next available ID
$bmadDataPath = Join-Path $ProjectRoot "Tasks\BMAD_Tasks.json"
$nextId = 1000

if (Test-Path $bmadDataPath) {
    try {
        $existingTasks = Get-Content $bmadDataPath -Raw | ConvertFrom-Json
        if ($existingTasks -and $existingTasks.Count -gt 0) {
            $maxId = ($existingTasks | Measure-Object -Property ID -Maximum).Maximum
            $nextId = $maxId + 1
        }
    } catch {
        Write-ImportLog "Could not read existing BMAD tasks, starting fresh" "WARNING"
    }
}

Write-ImportLog "Next available ID: $nextId" "INFO"

# Convert to BMAD format
$bmadTasks = @()
foreach ($task in $parsedTasks) {
    $bmadTask = Convert-ToBMADTask $task $nextId
    $bmadTasks += $bmadTask
    $nextId++
}

# Show preview
Write-Host "`n=== Import Preview ===" -ForegroundColor Cyan
Write-Host "Total tasks to import: $($bmadTasks.Count)" -ForegroundColor White

$categorySummary = $bmadTasks | Group-Object -Property Category | Sort-Object Count -Descending
foreach ($cat in ($categorySummary | Select-Object -First 5)) {
    Write-Host "  $($cat.Name): $($cat.Count) tasks" -ForegroundColor Gray
}

$totalHours = ($bmadTasks | Measure-Object -Property EstimatedHours -Sum).Sum
Write-Host "Total estimated hours: $totalHours" -ForegroundColor White
Write-Host "======================" -ForegroundColor Cyan

if ($DryRun) {
    Write-ImportLog "Dry run mode - no changes made" "WARNING"
    
    Write-Host "`nFirst 5 tasks to import:" -ForegroundColor Yellow
    foreach ($task in ($bmadTasks | Select-Object -First 5)) {
        Write-Host "  [$($task.ID)] $($task.Name)" -ForegroundColor White
        Write-Host "      Category: $($task.Category) | Hours: $($task.EstimatedHours)" -ForegroundColor Gray
    }
    if ($bmadTasks.Count -gt 5) {
        Write-Host "  ... and $($bmadTasks.Count - 5) more" -ForegroundColor Gray
    }
    
    exit 0
}

# Perform import
# Load existing tasks
$existingTasks = @()
if (Test-Path $bmadDataPath) {
    try {
        $existingTasks = Get-Content $bmadDataPath -Raw | ConvertFrom-Json
        if ($existingTasks -isnot [System.Array]) {
            $existingTasks = @($existingTasks)
        }
    } catch {
        Write-ImportLog "Starting with empty task list" "WARNING"
    }
}

# Backup existing file
if ((Test-Path $bmadDataPath) -and !$SkipBackup) {
    $backupPath = $bmadDataPath + ".backup_$(Get-Date -Format 'yyyyMMdd_HHmmss')"
    Copy-Item $bmadDataPath $backupPath -Force
    Write-ImportLog "Backup created: $backupPath" "INFO"
}

# Merge tasks
$allTasks = $existingTasks + $bmadTasks

# Save to JSON
$tasksDir = Join-Path $ProjectRoot "Tasks"
if (!(Test-Path $tasksDir)) {
    New-Item -ItemType Directory -Path $tasksDir -Force | Out-Null
}

$jsonContent = $allTasks | ConvertTo-Json -Depth 10
$jsonContent | Out-File $bmadDataPath -Encoding UTF8

Write-ImportLog "Imported $($bmadTasks.Count) tasks to BMAD system" "SUCCESS"
Write-ImportLog "BMAD data saved: $bmadDataPath" "INFO"

# Generate import report
$reportPath = Join-Path $ProjectRoot "Reports\BMAD_Import_Report_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"

$reportLines = @()
$reportLines += "# BMAD TODO Import Report"
$reportLines += ""
$reportLines += "**Import Date:** $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
$reportLines += "**Imported Tasks:** $($bmadTasks.Count)"
$reportLines += "**Total Estimated Hours:** $totalHours"
$reportLines += ""
$reportLines += "## Import Summary"
$reportLines += ""

foreach ($cat in $categorySummary) {
    $catHours = ($cat.Group | Measure-Object -Property EstimatedHours -Sum).Sum
    $reportLines += "- **$($cat.Name)**: $($cat.Count) tasks, ~$catHours hours"
}

$reportLines += ""
$reportLines += "## Imported Task IDs"
$reportLines += ""
$reportLines += "New tasks have been assigned IDs starting from $($bmadTasks[0].ID)"
$reportLines += ""
$reportLines += "| Original TODO | BMAD ID | Category | Hours |"
$reportLines += "|--------------|---------|----------|-------|"

foreach ($task in ($bmadTasks | Sort-Object SourceTaskID)) {
    $reportLines += "| $($task.SourceTaskID) | $($task.ID) | $($task.Category) | $($task.EstimatedHours) |"
}

$reportLines += ""
$reportLines += "## Next Steps"
$reportLines += ""
$reportLines += "1. Review imported tasks in BMAD system"
$reportLines += "2. Assign tasks to appropriate team members"
$reportLines += "3. Update priorities as needed"
$reportLines += "4. Track progress through BMAD workflow"
$reportLines += ""
$reportLines += "## BMAD Data Location"
$reportLines += ""
$reportLines += "Task data stored in: Tasks\BMAD_Tasks.json"

$reportLines | Out-File $reportPath -Encoding UTF8
Write-ImportLog "Import report saved: $reportPath" "SUCCESS"

Write-ImportLog "=== BMAD Import Complete ===" "SUCCESS"
