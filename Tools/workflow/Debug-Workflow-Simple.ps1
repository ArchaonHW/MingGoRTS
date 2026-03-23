# Debug Workflow - Simple Working Version
# Auto bug detection for MingGoRTS

param(
    [switch]$AutoFix = $false,
    [switch]$DeepScan = $false,
    [switch]$ReportToBMAD = $false,
    [string]$ProjectRoot = "f:\MingGoRTS"
)

function Write-DebugLog {
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

Write-DebugLog "=== Debug Workflow Started ===" "SUCCESS"
Write-DebugLog "Project: $ProjectRoot" "INFO"
Write-DebugLog "AutoFix: $AutoFix | DeepScan: $DeepScan" "INFO"

# Create Logs directory
$logDir = Join-Path $ProjectRoot "Logs"
if (!(Test-Path $logDir)) { New-Item -ItemType Directory -Path $logDir -Force | Out-Null }

# Find debug reports
$debugReportPath = Join-Path $ProjectRoot "Tasks\Debug\DebugReport_*.md"
$debugReports = Get-ChildItem -Path $debugReportPath -ErrorAction SilentlyContinue | 
    Sort-Object LastWriteTime -Descending

if ($debugReports) {
    Write-DebugLog "Found $($debugReports.Count) debug report(s)" "INFO"
    $latestReport = $debugReports | Select-Object -First 1
    $content = Get-Content $latestReport.FullName -Raw
    
    # Count issues
    $highIssues = ([regex]::Matches($content, "HIGH|Critical")).Count
    $mediumIssues = ([regex]::Matches($content, "MEDIUM")).Count
    $lowIssues = ([regex]::Matches($content, "LOW")).Count
    
    Write-DebugLog "Issues: $highIssues High, $mediumIssues Medium, $lowIssues Low" "WARNING"
    
    # Analyze Source files for common issues
    $sourcePath = Join-Path $ProjectRoot "Source\MingGoRTS\Private"
    if (Test-Path $sourcePath) {
        $cppFiles = Get-ChildItem -Path $sourcePath -Filter "*.cpp" -Recurse -ErrorAction SilentlyContinue
        Write-DebugLog "Scanning $($cppFiles.Count) source files..." "INFO"
        
        $issuesFound = 0
        foreach ($file in $cppFiles | Select-Object -First 20) {
            $fileContent = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
            if (!$fileContent) { continue }
            
            # Check for common issues
            $issues = @()
            if ($fileContent -match "TODO|FIXME|HACK" ) { $issues += "Has TODO/FIXME comments" }
            if ($fileContent -match "NULL|nullptr" -and $fileContent -notmatch "CheckNull|nullptr check") { $issues += "Potential null pointer issues" }
            if ($fileContent -match "new\s+\w+" -and $fileContent -notmatch "delete\s+\w+") { $issues += "Memory allocation without deallocation" }
            
            if ($issues.Count -gt 0) {
                Write-DebugLog "[$($file.Name)]: $($issues -join ', ')" "WARNING"
                $issuesFound++
            }
        }
        
        if ($issuesFound -eq 0) {
            Write-DebugLog "No major issues found in source files" "SUCCESS"
        } else {
            Write-DebugLog "Found issues in $issuesFound files" "WARNING"
        }
    }
} else {
    Write-DebugLog "No debug reports found" "INFO"
}

# Generate summary report
$reportPath = Join-Path $ProjectRoot "Logs\Debug_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"
"Debug Workflow Report - $(Get-Date)" | Out-File $reportPath
"AutoFix: $AutoFix | DeepScan: $DeepScan" | Out-File $reportPath -Append
"ReportToBMAD: $ReportToBMAD" | Out-File $reportPath -Append
"Status: Completed" | Out-File $reportPath -Append

Write-DebugLog "Debug Workflow Completed" "SUCCESS"
Write-DebugLog "Log saved to: $reportPath" "INFO"
