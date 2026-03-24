# TODO to BMAD Task Generator - Simple Version
# Converts TODO/FIXME comments into BMAD-trackable tasks

param(
    [string]$ProjectRoot = "f:\MingGoRTS",
    [string]$OutputPath = ""
)

function Write-TODOLog {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $color = switch ($Level) { "INFO" { "White" } "SUCCESS" { "Green" } "WARNING" { "Yellow" } "ERROR" { "Red" } }
    Write-Host "[$timestamp] [$Level] $Message" -ForegroundColor $color
}

function Get-Category {
    param([string]$filePath)
    if ($filePath -match "AI|Brain|Sage") { return "AI System" }
    if ($filePath -match "UI|Widget|HUD") { return "UI System" }
    if ($filePath -match "Network|Multiplayer") { return "Network" }
    if ($filePath -match "Audio|Sound") { return "Audio" }
    if ($filePath -match "Character|NPC") { return "Character" }
    if ($filePath -match "Quest|Dialogue|Story") { return "Quest System" }
    if ($filePath -match "Building|Construction") { return "Building" }
    if ($filePath -match "Resource|Economy|Production") { return "Economy" }
    if ($filePath -match "Skill|Tech|Research") { return "Skill System" }
    if ($filePath -match "Relationship|Social") { return "Social System" }
    if ($filePath -match "Debug|Test") { return "Debug/Test" }
    if ($filePath -match "Command|SageCommand|WuXing|AntiFall") { return "SageCommand" }
    return "General"
}

function Get-Priority {
    param([string]$filePath, [string]$description, [string]$type)
    $score = 0
    if ($type -eq "FIXME") { $score += 30 }
    if ($type -eq "XXX") { $score += 20 }
    if ($type -eq "HACK") { $score += 10 }
    if ($description -match "critical|bug|crash|memory|performance") { $score += 40 }
    if ($description -match "implement|feature|add") { $score += 20 }
    if ($description -match "refactor|optimize") { $score += 15 }
    if ($filePath -match "SageCommand|SageBrain|Core") { $score += 25 }
    if ($score -ge 60) { return "Critical" }
    if ($score -ge 40) { return "High" }
    if ($score -ge 20) { return "Medium" }
    return "Low"
}

function Get-EstimatedHours {
    param([string]$description)
    if ($description -match "implement|create|build") { return 8 }
    if ($description -match "refactor|redesign") { return 6 }
    if ($description -match "optimize|performance") { return 4 }
    if ($description -match "fix|bug|correct") { return 2 }
    if ($description -match "test|verify") { return 3 }
    if ($description -match "documentation|doc|comment") { return 1 }
    return 2
}

Write-TODOLog "=== TODO to BMAD Task Generator ===" "SUCCESS"

# Scan for TODOs
$sourcePath = Join-Path $ProjectRoot "Source\MingGoRTS"
$cppFiles = Get-ChildItem -Path $sourcePath -Filter "*.cpp" -Recurse -ErrorAction SilentlyContinue

$todoItems = @()
$fileCounter = 0

foreach ($file in $cppFiles) {
    $fileCounter++
    if ($fileCounter % 50 -eq 0) {
        Write-TODOLog "Processed $fileCounter files..." "INFO"
    }
    
    $lines = Get-Content $file.FullName -ErrorAction SilentlyContinue
    if (!$lines) { continue }
    
    for ($i = 0; $i -lt $lines.Count; $i++) {
        $line = $lines[$i]
        if ($line -match "(TODO|FIXME|HACK|XXX)[\s:]+(.+)") {
            $type = $matches[1]
            $description = $matches[2].Trim()
            
            $relPath = $file.FullName.Replace($ProjectRoot, "")
            $category = Get-Category $relPath
            $priority = Get-Priority $relPath $description $type
            $hours = Get-EstimatedHours $description
            
            $todoItem = [PSCustomObject]@{
                ID = $todoItems.Count + 1
                File = $relPath
                LineNumber = $i + 1
                Type = $type
                Description = $description
                Category = $category
                Priority = $priority
                EstimatedHours = $hours
            }
            $todoItems += $todoItem
        }
    }
}

Write-TODOLog "Found $($todoItems.Count) TODO/FIXME items" "SUCCESS"

# Generate report
if (!$OutputPath) {
    $OutputPath = Join-Path $ProjectRoot "Reports\BMAD_TODO_Tasks_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
}

$reportsDir = Join-Path $ProjectRoot "Reports"
if (!(Test-Path $reportsDir)) {
    New-Item -ItemType Directory -Path $reportsDir -Force | Out-Null
}

$totalHours = ($todoItems | Measure-Object -Property EstimatedHours -Sum).Sum

$reportLines = @()
$reportLines += "# BMAD TODO Task List"
$reportLines += ""
$reportLines += "**Generated:** $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
$reportLines += "**Total Tasks:** $($todoItems.Count)"
$reportLines += "**Total Estimated Hours:** $totalHours"
$reportLines += ""

# Summary by category
$reportLines += "## Summary by Category"
$reportLines += ""
$categorySummary = $todoItems | Group-Object -Property Category | Sort-Object Count -Descending
foreach ($cat in $categorySummary) {
    $catHours = ($cat.Group | Measure-Object -Property EstimatedHours -Sum).Sum
    $reportLines += "- **$($cat.Name)**: $($cat.Count) tasks, ~$catHours hours"
}
$reportLines += ""

# Summary by priority
$reportLines += "## Summary by Priority"
$reportLines += ""
$prioritySummary = $todoItems | Group-Object -Property Priority
$priorities = @("Critical", "High", "Medium", "Low")
foreach ($p in $priorities) {
    $pGroup = $prioritySummary | Where-Object { $_.Name -eq $p }
    if ($pGroup) {
        $pHours = ($pGroup.Group | Measure-Object -Property EstimatedHours -Sum).Sum
        $reportLines += "- **$p**: $($pGroup.Count) tasks, ~$pHours hours"
    }
}
$reportLines += ""

# Tasks by priority
foreach ($priority in $priorities) {
    $tasks = $todoItems | Where-Object { $_.Priority -eq $priority }
    if ($tasks.Count -eq 0) { continue }
    
    $reportLines += "## $priority Priority Tasks"
    $reportLines += ""
    
    foreach ($task in ($tasks | Sort-Object Category, File)) {
        $reportLines += "### BMAD-TODO-$($task.ID.ToString().PadLeft(3, '0'))"
        $reportLines += ""
        $reportLines += "- **Category:** $($task.Category)"
        $reportLines += "- **File:** $($task.File):$($task.LineNumber)"
        $reportLines += "- **Type:** $($task.Type)"
        $reportLines += "- **Description:** $($task.Description)"
        $reportLines += "- **Estimated Hours:** $($task.EstimatedHours)"
        $reportLines += "- **Status:** Pending"
        $reportLines += "- **Assigned To:** Unassigned"
        $reportLines += ""
        $reportLines += "---"
        $reportLines += ""
    }
}

$reportLines += "## BMAD Integration Notes"
$reportLines += ""
$reportLines += "These tasks should be imported into the BMAD system for tracking."
$reportLines += "- **Critical**: Must be completed immediately"
$reportLines += "- **High**: Should be completed in current sprint"
$reportLines += "- **Medium**: Plan for next sprint"
$reportLines += "- **Low**: Address when convenient"

$reportLines | Out-File $OutputPath -Encoding UTF8
Write-TODOLog "BMAD report saved: $OutputPath" "SUCCESS"

# Show final summary
Write-Host "`n=== Task Generation Summary ===" -ForegroundColor Cyan
foreach ($cat in $categorySummary) {
    Write-Host "  $($cat.Name): $($cat.Count) tasks" -ForegroundColor White
}
Write-Host "Total: $($todoItems.Count) tasks, ~$totalHours hours" -ForegroundColor Yellow
Write-Host "=================================" -ForegroundColor Cyan

Write-TODOLog "Task generation complete!" "SUCCESS"
