#!/usr/bin/env pwsh

<#
.SYNOPSIS
    MingGoRTS Debug Workflow System (English Version)
    Automated Bug Detection and BMAD Task Assignment System

.DESCRIPTION
    1. Scan project for bugs (syntax errors, compile errors, etc.)
    2. If bugs found, automatically report to BMAD analysis
    3. Generate Debug task list and assign to requirement system
    4. Mark as Debug type tasks

.PARAMETER AutoFix
    Try to automatically fix simple bugs

.PARAMETER DeepScan
    Execute deep scan (including static analysis)

.PARAMETER ReportToBMAD
    Report results to BMAD system

.EXAMPLE
    .\Debug-Workflow-EN.ps1
    Execute standard bug detection workflow

.EXAMPLE
    .\Debug-Workflow-EN.ps1 -DeepScan -ReportToBMAD
    Deep scan and report to BMAD system
#>

[CmdletBinding()]
param(
    [Parameter()]
    [switch]$AutoFix,
    
    [Parameter()]
    [switch]$DeepScan,
    
    [Parameter()]
    [switch]$ReportToBMAD,
    
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$TaskOutputPath = "$PWD\Tasks\Debug",
    
    [Parameter()]
    [string]$LogPath = "$PWD\Logs\DebugWorkflow.log"
)

$ErrorActionPreference = "Stop"

# Color definitions
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
    Debug = "Magenta"
}

# Bug tracker
$BugTracker = @{
    SyntaxErrors = @()
    CompileErrors = @()
    StaticAnalysisIssues = @()
    Warnings = @()
    AutoFixable = @()
    TotalBugs = 0
}

function Write-DebugLog {
    param(
        [string]$Message,
        [ValidateSet("INFO", "SUCCESS", "WARNING", "ERROR", "DEBUG")]
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $Color = switch ($Level) {
        "SUCCESS" { $Colors.Success }
        "WARNING" { $Colors.Warning }
        "ERROR" { $Colors.Error }
        "DEBUG" { $Colors.Debug }
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

function Show-DebugBanner {
    Write-Host @"

    ╔═══════════════════════════════════════════════════════════╗
    ║                                                           ║
    ║              MingGoRTS Debug Workflow System              ║
    ║                Automated Bug Detection System              ║
    ║                                                           ║
    ╚═══════════════════════════════════════════════════════════╝

"@ -ForegroundColor $Colors.Debug
}

# ============================================
# Phase 1: Bug Detection
# ============================================

function Start-BugDetection {
    Write-DebugLog "==========================================" "INFO"
    Write-DebugLog "Phase 1: Starting Bug Detection" "INFO"
    Write-DebugLog "==========================================" "INFO"
    
    # 1.1 Syntax error detection
    Test-SyntaxErrors
    
    # 1.2 Compile error detection
    Test-CompileErrors
    
    # 1.3 Static analysis (deep scan)
    if ($DeepScan) {
        Start-StaticAnalysis
    }
    
    # Count total bugs
    $BugTracker.TotalBugs = $BugTracker.SyntaxErrors.Count + 
                           $BugTracker.CompileErrors.Count + 
                           $BugTracker.StaticAnalysisIssues.Count
    
    Write-DebugLog "Bug detection completed - Found $($BugTracker.TotalBugs) issues" $(if ($BugTracker.TotalBugs -gt 0) { "WARNING" } else { "SUCCESS" })
    
    return $BugTracker.TotalBugs -gt 0
}

function Test-SyntaxErrors {
    Write-DebugLog "`n[1.1] Detecting syntax errors..." "DEBUG"
    
    # Scan C++ files
    $CppFiles = Get-ChildItem -Path "$ProjectRoot\Source" -Recurse -Filter "*.cpp" -ErrorAction SilentlyContinue
    $HeaderFiles = Get-ChildItem -Path "$ProjectRoot\Source" -Recurse -Filter "*.h" -ErrorAction SilentlyContinue
    
    Write-DebugLog "  Scanning C++ files: $($CppFiles.Count) .cpp, $($HeaderFiles.Count) .h" "INFO"
    
    # Common syntax error patterns
    $SyntaxPatterns = @(
        @{ Pattern = "overHide"; Description = "Incorrect override keyword"; Fix = "override" }
        @{ Pattern = "Virtuai"; Description = "Incorrect virtual keyword"; Fix = "virtual" }
        @{ Pattern = "void void"; Description = "Duplicate void keyword"; Fix = "void" }
        @{ Pattern = "class.*:.*public.*{[^}]"; Description = "Missing class closing brace"; Fix = "Check class definition" }
        @{ Pattern = "\(\s*\)\s*;"; Description = "Possible empty function body"; Fix = "Confirm if implementation needed" }
    )
    
    foreach ($File in ($CppFiles + $HeaderFiles)) {
        $Content = Get-Content -Path $File.FullName -Raw -ErrorAction SilentlyContinue
        if (-not $Content) { continue }
        
        foreach ($Pattern in $SyntaxPatterns) {
            if ($Content -match $Pattern.Pattern) {
                $BugTracker.SyntaxErrors += @{
                    File = $File.FullName
                    Line = 0
                    Type = "Syntax"
                    Description = $Pattern.Description
                    Pattern = $Pattern.Pattern
                    SuggestedFix = $Pattern.Fix
                    Severity = "High"
                }
                Write-DebugLog "    Found syntax error: $($Pattern.Description) in $($File.Name)" "WARNING"
            }
        }
    }
    
    Write-DebugLog "  [✓] Syntax error detection completed: $($BugTracker.SyntaxErrors.Count) issues" $(if ($BugTracker.SyntaxErrors.Count -gt 0) { "WARNING" } else { "SUCCESS" })
}

function Test-CompileErrors {
    Write-DebugLog "`n[1.2] Detecting compile errors..." "DEBUG"
    
    # Check build logs in Intermediate directory
    $BuildLogs = Get-ChildItem -Path "$ProjectRoot\Intermediate\Build" -Recurse -Filter "*.log" -ErrorAction SilentlyContinue | 
                 Select-Object -First 5
    
    if ($BuildLogs) {
        foreach ($Log in $BuildLogs) {
            $Content = Get-Content -Path $Log.FullName -ErrorAction SilentlyContinue | Select-Object -Last 100
            if ($Content -match "error[\s:]+|fatal error") {
                $MatchResults = $Content | Select-String -Pattern "error[\s:]+(.*)" | Select-Object -First 3
                foreach ($Match in $MatchResults) {
                    $BugTracker.CompileErrors += @{
                        File = $Log.FullName
                        Line = 0
                        Type = "Compile"
                        Description = $Match.Matches[0].Groups[1].Value
                        LogEntry = $Match.Line
                        Severity = "Critical"
                    }
                }
            }
        }
    }
    
    # Check UHT errors
    $UHTManifests = Get-ChildItem -Path "$ProjectRoot\Intermediate" -Recurse -Filter "*.uhtmanifest" -ErrorAction SilentlyContinue
    if (-not $UHTManifests) {
        Write-DebugLog "    Warning: No UHT manifest found, may need to recompile" "WARNING"
    }
    
    Write-DebugLog "  [✓] Compile error detection completed: $($BugTracker.CompileErrors.Count) issues" $(if ($BugTracker.CompileErrors.Count -gt 0) { "ERROR" } else { "SUCCESS" })
}

function Start-StaticAnalysis {
    Write-DebugLog "`n[1.3] Executing static analysis..." "DEBUG"
    
    # Simple static analysis rules
    $AnalysisRules = @(
        @{ Pattern = "TODO|FIXME|HACK|XXX"; Description = "Code markers need attention"; Severity = "Low" }
        @{ Pattern = "GetWorld\(\)->SpawnActor"; Description = "Unchecked SpawnActor return value"; Severity = "Medium" }
        @{ Pattern = "NewObject<.*>\(\)"; Description = "Unchecked NewObject return value"; Severity = "Low" }
        @{ Pattern = "//.*TODO"; Description = "TODO comments present"; Severity = "Info" }
    )
    
    $SourceFiles = Get-ChildItem -Path "$ProjectRoot\Source" -Recurse -Filter "*.cpp" -ErrorAction SilentlyContinue
    
    foreach ($File in $SourceFiles) {
        $Lines = Get-Content -Path $File.FullName -ErrorAction SilentlyContinue
        $LineNumber = 0
        
        foreach ($Line in $Lines) {
            $LineNumber++
            foreach ($Rule in $AnalysisRules) {
                if ($Line -match $Rule.Pattern) {
                    $BugTracker.StaticAnalysisIssues += @{
                        File = $File.FullName
                        Line = $LineNumber
                        Type = "StaticAnalysis"
                        Description = $Rule.Description
                        Code = $Line.Trim()
                        Severity = $Rule.Severity
                    }
                }
            }
        }
    }
    
    Write-DebugLog "  [✓] Static analysis completed: $($BugTracker.StaticAnalysisIssues.Count) issues" $(if ($BugTracker.StaticAnalysisIssues.Count -gt 0) { "WARNING" } else { "SUCCESS" })
}

# ============================================
# Phase 2: Auto Fix (Optional)
# ============================================

function Start-AutoFix {
    if (-not $AutoFix) { return $false }
    
    Write-DebugLog "==========================================" "INFO"
    Write-DebugLog "Phase 2: Attempting Auto Fix" "INFO"
    Write-DebugLog "==========================================" "INFO"
    
    $FixedCount = 0
    
    foreach ($Bug in $BugTracker.SyntaxErrors) {
        if ($Bug.Description -match "override") {
            $Content = Get-Content -Path $Bug.File -Raw
            $NewContent = $Content -replace "overHide", "override"
            if ($NewContent -ne $Content) {
                $NewContent | Set-Content -Path $Bug.File
                $FixedCount++
                $BugTracker.AutoFixable += $Bug
                Write-DebugLog "  [✓] Auto fixed: $($Bug.File) - overHide -> override" "SUCCESS"
            }
        }
        elseif ($Bug.Description -match "virtual") {
            $Content = Get-Content -Path $Bug.File -Raw
            $NewContent = $Content -replace "Virtuai", "virtual"
            if ($NewContent -ne $Content) {
                $NewContent | Set-Content -Path $Bug.File
                $FixedCount++
                $BugTracker.AutoFixable += $Bug
                Write-DebugLog "  [✓] Auto fixed: $($Bug.File) - Virtuai -> virtual" "SUCCESS"
            }
        }
    }
    
    Write-DebugLog "Auto fix completed: $FixedCount issues fixed" $(if ($FixedCount -gt 0) { "SUCCESS" } else { "INFO" })
    
    return $FixedCount -gt 0
}

# ============================================
# Phase 3: BMAD Task Generation
# ============================================

function New-BMADDebugTasks {
    Write-DebugLog "==========================================" "INFO"
    Write-DebugLog "Phase 3: Generating BMAD Debug Tasks" "INFO"
    Write-DebugLog "==========================================" "INFO"
    
    $Tasks = @()
    
    # Group by severity
    $CriticalBugs = $BugTracker.SyntaxErrors + $BugTracker.CompileErrors | Where-Object { $_.Severity -eq "Critical" }
    $HighBugs = $BugTracker.SyntaxErrors + $BugTracker.CompileErrors | Where-Object { $_.Severity -eq "High" }
    $MediumBugs = $BugTracker.StaticAnalysisIssues | Where-Object { $_.Severity -eq "Medium" }
    
    # Generate Critical level tasks
    if ($CriticalBugs.Count -gt 0) {
        $Tasks += @{
            ID = "DEBUG-001"
            Title = "Fix Critical Compile Errors ($($CriticalBugs.Count) issues)"
            Description = "Fix critical errors causing compilation failure`nAffected files: $($CriticalBugs.File -join "`n")"
            Type = "Debug"
            SubType = "CompileFix"
            Priority = "Critical"
            Severity = "Critical"
            EstimatedHours = $CriticalBugs.Count * 2
            RelatedBugs = $CriticalBugs
            Dependencies = @()
        }
        Write-DebugLog "  [✓] Created critical task: DEBUG-001" "DEBUG"
    }
    
    # Generate High level tasks
    if ($HighBugs.Count -gt 0) {
        $Tasks += @{
            ID = "DEBUG-002"
            Title = "Fix Syntax Errors ($($HighBugs.Count) issues)"
            Description = "Fix syntax errors in source code`nAffected files: $($HighBugs.File -join "`n")"
            Type = "Debug"
            SubType = "SyntaxFix"
            Priority = "High"
            Severity = "High"
            EstimatedHours = $HighBugs.Count * 1.5
            RelatedBugs = $HighBugs
            Dependencies = @("DEBUG-001")
        }
        Write-DebugLog "  [✓] Created high priority task: DEBUG-002" "DEBUG"
    }
    
    # Generate Medium level tasks
    if ($MediumBugs.Count -gt 0) {
        $Tasks += @{
            ID = "DEBUG-003"
            Title = "Handle Static Analysis Warnings ($($MediumBugs.Count) issues)"
            Description = "Handle code quality warnings and potential issues`nIncluding: $($MediumBugs.Description | Select-Object -Unique -First 5)"
            Type = "Debug"
            SubType = "CodeQuality"
            Priority = "Medium"
            Severity = "Medium"
            EstimatedHours = $MediumBugs.Count * 0.5
            RelatedBugs = $MediumBugs
            Dependencies = @()
        }
        Write-DebugLog "  [✓] Created medium priority task: DEBUG-003" "DEBUG"
    }
    
    # Generate verification task
    if ($Tasks.Count -gt 0) {
        $Tasks += @{
            ID = "DEBUG-004"
            Title = "Verify Fix Results"
            Description = "Run full compilation and testing to verify all fixes"
            Type = "Debug"
            SubType = "Verification"
            Priority = "High"
            Severity = "Medium"
            EstimatedHours = 2
            RelatedBugs = @()
            Dependencies = $Tasks | Where-Object { $_.ID -ne "DEBUG-004" } | ForEach-Object { $_.ID }
        }
        Write-DebugLog "  [✓] Created verification task: DEBUG-004" "DEBUG"
    }
    
    Write-DebugLog "[✓] BMAD task generation completed: $($Tasks.Count) tasks" "SUCCESS"
    
    return $Tasks
}

# ============================================
# Phase 4: Requirement System Sync
# ============================================

function Sync-ToRequirementSystem {
    [CmdletBinding()]
    param([array]$Tasks)
    
    Write-DebugLog "==========================================" "INFO"
    Write-DebugLog "Phase 4: Sync to Requirement System" "INFO"
    Write-DebugLog "==========================================" "INFO"
    
    # Ensure output directory exists
    if (-not (Test-Path $TaskOutputPath)) {
        New-Item -ItemType Directory -Path $TaskOutputPath -Force | Out-Null
    }
    
    # Generate requirement data
    $RequirementData = @{
        RequirementID = "REQ-BUG-$(Get-Date -Format 'yyyyMMdd')"
        Title = "MingGoRTS Debug Requirement - $(Get-Date -Format 'yyyy-MM-dd')"
        Description = "Automated bug detection generated fix requirement`nFound $($BugTracker.TotalBugs) issues`nGenerated $($Tasks.Count) fix tasks"
        Type = "Debug"
        Priority = if ($BugTracker.CompileErrors.Count -gt 0) { "Critical" } else { "High" }
        Status = "Open"
        CreatedAt = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
        Tasks = $Tasks
        BugSummary = @{
            Total = $BugTracker.TotalBugs
            SyntaxErrors = $BugTracker.SyntaxErrors.Count
            CompileErrors = $BugTracker.CompileErrors.Count
            StaticIssues = $BugTracker.StaticAnalysisIssues.Count
            AutoFixed = $BugTracker.AutoFixable.Count
        }
    }
    
    # Save JSON
    $JsonFile = Join-Path $TaskOutputPath "DebugRequirement_$($RequirementData.RequirementID).json"
    $RequirementData | ConvertTo-Json -Depth 10 | Set-Content -Path $JsonFile
    Write-DebugLog "  [✓] Requirement data saved: $JsonFile" "SUCCESS"
    
    # Generate Markdown report
    $ReportFile = Join-Path $TaskOutputPath "DebugReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').md"
    
    $Report = @"
# Debug Workflow Report

**Execution Time**: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")  
**Project Path**: $ProjectRoot  
**Scan Mode**: $(if ($DeepScan) { "Deep Scan" } else { "Standard Scan" })

## Bug Detection Summary

| Type | Count | Severity |
|------|-------|----------|
| Syntax Errors | $($BugTracker.SyntaxErrors.Count) | High |
| Compile Errors | $($BugTracker.CompileErrors.Count) | Critical |
| Static Analysis Issues | $($BugTracker.StaticAnalysisIssues.Count) | Medium |
| **Total** | **$($BugTracker.TotalBugs)** | - |

$(if ($BugTracker.AutoFixable.Count -gt 0) { "`n## Auto Fixes`n`nAutomatically fixed $($BugTracker.AutoFixable.Count) issues`n" })

## Generated BMAD Tasks

| Task ID | Title | Priority | Estimated Hours |
|----------|-------|----------|-----------------|
"@

    foreach ($Task in $Tasks) {
        $Report += "| $($Task.ID) | $($Task.Title) | $($Task.Priority) | $($Task.EstimatedHours)h |`n"
    }
    
    $Report += @"

## Detailed Bug List

### Syntax Errors

"@
    foreach ($Bug in $BugTracker.SyntaxErrors) {
        $Report += "- **$($Bug.File.Split('\')[-1])**: $($Bug.Description)`n"
    }
    
    $Report += @"

### Compile Errors

"@
    foreach ($Bug in $BugTracker.CompileErrors) {
        $Report += "- **$($Bug.File.Split('\')[-1])**: $($Bug.Description)`n"
    }
    
    $Report += @"

## Next Steps

1. View detailed report: $ReportFile
2. Use Visual Studio to open project and fix errors
3. Run compilation to verify fixes
4. Re-run Debug Workflow to confirm issues resolved

---
*Generated by MingGoRTS Debug Workflow System*
"@

    $Report | Set-Content -Path $ReportFile
    Write-DebugLog "  [✓] Report generated: $ReportFile" "SUCCESS"
    
    # Report to BMAD system (if needed)
    if ($ReportToBMAD) {
        Write-DebugLog "`n  Reporting to BMAD system..." "INFO"
        
        # Create BMAD workflow trigger file
        $BMADTrigger = @{
            WorkflowType = "Debug"
            RequirementID = $RequirementData.RequirementID
            Priority = $RequirementData.Priority
            TaskCount = $Tasks.Count
            TriggerTime = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
            DataFile = $JsonFile
            ReportFile = $ReportFile
        }
        
        $BMADTriggerFile = Join-Path $TaskOutputPath "BMAD_Trigger_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
        $BMADTrigger | ConvertTo-Json | Set-Content -Path $BMADTriggerFile
        
        Write-DebugLog "  [✓] BMAD trigger file created: $BMADTriggerFile" "SUCCESS"
        Write-DebugLog "  BMAD system will automatically process this Debug requirement" "INFO"
    }
    
    return @{
        RequirementFile = $JsonFile
        ReportFile = $ReportFile
        BMADTriggerFile = if ($ReportToBMAD) { $BMADTriggerFile } else { $null }
        TotalTasks = $Tasks.Count
    }
}

# ============================================
# Main Execution Flow
# ============================================

Show-DebugBanner

# Phase 1: Bug Detection
$HasBugs = Start-BugDetection

if (-not $HasBugs) {
    Write-DebugLog "`n✅ No bugs detected! Project status is good." "SUCCESS"
    Write-DebugLog "`n==========================================" "SUCCESS"
    exit 0
}

# Phase 2: Auto Fix (if enabled)
$Fixed = Start-AutoFix

if ($Fixed -and -not $DeepScan) {
    Write-DebugLog "`nSome issues automatically fixed, suggest re-running detection" "INFO"
}

# Phase 3: Generate BMAD Tasks
$BMADTasks = New-BMADDebugTasks

# Phase 4: Sync to Requirement System
$SyncResult = Sync-ToRequirementSystem -Tasks $BMADTasks

# Display final summary
Write-DebugLog "`n==========================================" "SUCCESS"
Write-DebugLog "Debug Workflow Execution Completed" "SUCCESS"
Write-DebugLog "==========================================" "SUCCESS"

Write-DebugLog "`n📊 Execution Summary:" "INFO"
Write-DebugLog "  Total Bugs: $($BugTracker.TotalBugs)" "INFO"
Write-DebugLog "  Syntax Errors: $($BugTracker.SyntaxErrors.Count)" "INFO"
Write-DebugLog "  Compile Errors: $($BugTracker.CompileErrors.Count)" $(if ($BugTracker.CompileErrors.Count -gt 0) { "ERROR" } else { "INFO" })
Write-DebugLog "  Auto Fixed: $($BugTracker.AutoFixable.Count)" $(if ($BugTracker.AutoFixable.Count -gt 0) { "SUCCESS" } else { "INFO" })
Write-DebugLog "  Generated Tasks: $($SyncResult.TotalTasks)" "INFO"

Write-DebugLog "`n📁 Output Files:" "INFO"
Write-DebugLog "  Requirement: $($SyncResult.RequirementFile)" "INFO"
Write-DebugLog "  Report: $($SyncResult.ReportFile)" "INFO"
if ($SyncResult.BMADTriggerFile) {
    Write-DebugLog "  BMAD Trigger: $($SyncResult.BMADTriggerFile)" "INFO"
}

Write-DebugLog "`n💡 Recommended Actions:" "INFO"
Write-DebugLog "  1. View detailed report to understand issues" "INFO"
Write-DebugLog "  2. Fix marked errors in Visual Studio" "INFO"
Write-DebugLog "  3. Re-run Debug-Workflow-EN.ps1 to verify fixes" "INFO"

exit $(if ($BugTracker.CompileErrors.Count -gt 0) { 1 } else { 0 })
