# MingGoRTS 腳本整合系統 - 統一整合所有現有腳本
# PowerShell Script

param(
    [string]$Category = "All",
    [string]$Mode = "Sequential",
    [switch]$DryRun = $false,
    [switch]$Verbose = $false
)

# Configuration
$ProjectRoot = "C:\HW\MingGoRTS"
$ToolsPath = "$ProjectRoot\Tools"
$LogPath = "$ProjectRoot\Logs\ScriptIntegration_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"
$ReportPath = "$ProjectRoot\Reports\ScriptIntegrationReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').html"

# Initialize logging
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    Write-Host $LogEntry -ForegroundColor $(switch($Level) {"ERROR" {"Red"}; "WARNING" {"Yellow"}; "SUCCESS" {"Green"}; default {"White"}})
    Add-Content -Path $LogPath -Value $LogEntry
}

# Script categories and their paths
$ScriptCategories = @{
    "Build" = @(
        "$ToolsPath\build\AUTOMATE_BLUEPRINT_BINDING.ps1",
        "$ToolsPath\build\AutoCompileDebugger.ps1",
        "$ToolsPath\build\AutoDebug_Enhanced.ps1",
        "$ToolsPath\build\ComprehensiveSyntaxFix.ps1",
        "$ToolsPath\build\DeveloperAutomation.ps1",
        "$ToolsPath\build\FixAllRemainingSyntaxErrors.ps1",
        "$ToolsPath\build\FixCoreSyntaxErrors.ps1",
        "$ToolsPath\build\FixNetworkSyntaxErrors.ps1",
        "$ToolsPath\build\FixPluginSyntaxErrors.ps1",
        "$ToolsPath\build\FixRemainingSyntaxErrors.ps1",
        "$ToolsPath\build\FixSyntaxErrors.ps1",
        "$ToolsPath\build\FixUHTErrors.ps1",
        "$ToolsPath\build\IntegrationTestRunner.ps1",
        "$ToolsPath\build\PreCommitSyntaxCheck.ps1",
        "$ToolsPath\build\RiskMonitoring.ps1",
        "$ToolsPath\build\ScriptOptimizer.ps1",
        "$ToolsPath\build\Security\VulnerabilityScanner.ps1",
        "$ToolsPath\build\SelfLearningCompile.ps1",
        "$ToolsPath\build\SetupDevelopmentEnvironment.ps1"
    )
    
    "Automation" = @(
        "$ToolsPath\automation\AutoDevelopmentSystem.ps1",
        "$ToolsPath\automation\AutoDevelopmentSystem_Fixed.ps1",
        "$ToolsPath\automation\AutoGenerateAndDeploy.ps1",
        "$ToolsPath\automation\BatchDeployAssets.ps1",
        "$ToolsPath\automation\ContinuousIntegration.ps1",
        "$ToolsPath\automation\DeployUEAssets.ps1",
        "$ToolsPath\automation\ManageAssetConfig.ps1"
    )
    
    "AI" = @(
        "$ToolsPath\ai\auto_generate.ps1",
        "$ToolsPath\ai\auto_generate_clean.ps1",
        "$ToolsPath\ai\epic_assets_generate.ps1",
        "$ToolsPath\ai\epic_assets_generate_clean.ps1",
        "$ToolsPath\ai\generate_music.ps1"
    )
    
    "API" = @(
        "$ToolsPath\api\ImplementAPIEnhancements.ps1"
    )
    
    "Optimization" = @(
        "$ToolsPath\build\AutoCompileDebugger_fixed.ps1",
        "$ToolsPath\build\ExecuteMaterialCreation.ps1",
        "$ToolsPath\build\ExecuteThreePhaseDev.ps1",
        "$ToolsPath\build\ExecuteThreePhaseDevelopment.ps1",
        "$ToolsPath\IntelligentRecommendationGenerator.ps1"
    )
    
    "Debug" = @(
        "$ToolsPath\build\AutoDebug_Enhanced.ps1",
        "$ToolsPath\build\IntegrationTestRunner.ps1",
        "$ToolsPath\build\RiskMonitoring.ps1",
        "$ToolsPath\build\ScriptOptimizer.ps1"
    )
    
    "Security" = @(
        "$ToolsPath\build\Security\VulnerabilityScanner.ps1"
    )
    
    "Maintenance" = @(
        "$ToolsPath\FixGarbledText.ps1",
        "$ToolsPath\FixGarbledText_v2.ps1",
        "$ToolsPath\CreateSageCharacter.ps1",
        "$ToolsPath\SetupUE5Environment.ps1",
        "$ToolsPath\SetupUE5Environment_Fixed.ps1"
    )
}

# Script metadata
$ScriptMetadata = @{}

# Initialize script metadata
function Initialize-ScriptMetadata {
    Write-Log "Initializing script metadata..." "INFO"
    
    foreach ($Category in $ScriptCategories.Keys) {
        $Scripts = $ScriptCategories[$Category]
        foreach ($Script in $Scripts) {
            if (Test-Path $Script) {
                $Metadata = @{
                    "Path" = $Script
                    "Category" = $Category
                    "Name" = (Split-Path $Script -LeafBase)
                    "Size" = (Get-Item $Script).Length
                    "LastModified" = (Get-Item $Script).LastWriteTime
                    "Dependencies" = @()
                    "Parameters" = @()
                    "EstimatedTime" = 5.0
                    "SuccessRate" = 0.95
                }
                
                # Analyze script content for metadata
                $Content = Get-Content $Script -Raw -ErrorAction SilentlyContinue
                if ($Content) {
                    # Extract parameters
                    if ($Content -match "param\((.*?)\)") {
                        $ParamBlock = $matches[1]
                        $Lines = $ParamBlock -split "`n"
                        foreach ($Line in $Lines) {
                            if ($Line -match '\$(\w+)') {
                                $Metadata.Parameters += $matches[1]
                            }
                        }
                    }
                    
                    # Estimate execution time
                    if ($Content -match "Start-Process") { $Metadata.EstimatedTime += 10 }
                    if ($Content -match "Compile|Build") { $Metadata.EstimatedTime += 30 }
                    if ($Content -match "Test|Test-") { $Metadata.EstimatedTime += 15 }
                    if ($Content -match "ForEach|for") { $Metadata.EstimatedTime += 5 }
                    
                    # Extract dependencies
                    if ($Content -match "\.ps1") {
                        $ScriptMatches = [regex]::Matches($Content, '([A-Za-z0-9_]+\.ps1)')
                        foreach ($Match in $ScriptMatches) {
                            $DepScript = $Match.Value
                            if ($DepScript -ne (Split-Path $Script -Leaf)) {
                                $Metadata.Dependencies += $DepScript
                            }
                        }
                    }
                }
                
                $ScriptMetadata[$Script] = $Metadata
            }
        }
    }
    
    Write-Log "Script metadata initialized for $($ScriptMetadata.Count) scripts" "SUCCESS"
}

# Execute individual script
function Invoke-Script {
    param(
        [string]$ScriptPath,
        [hashtable]$Parameters = @{}
    )
    
    if (-not (Test-Path $ScriptPath)) {
        Write-Log "Script not found: $ScriptPath" "ERROR"
        return $false
    }
    
    $ScriptName = Split-Path $ScriptPath -LeafBase
    Write-Log "Executing script: $ScriptName" "INFO"
    
    if ($DryRun) {
        Write-Log "DRY RUN: Would execute $ScriptPath" "WARNING"
        return $true
    }
    
    try {
        # Create a PowerShell instance to run the script
        $PSInstance = [PowerShell]::Create()
        
        # Add the script to the PowerShell instance
        $PSInstance.AddScript($ScriptPath) | Out-Null
        
        # Add parameters if provided
        foreach ($Param in $Parameters.Keys) {
            $PSInstance.AddParameter($Param, $Parameters[$Param]) | Out-Null
        }
        
        # Execute the script
        $PSInstance.Invoke()
        
        # Check for errors
        if ($PSInstance.HadErrors) {
            foreach ($ScriptError in $PSInstance.Streams.Error) {
                Write-Log "Script error: $ScriptError" "ERROR"
            }
            $PSInstance.Dispose()
            return $false
        }
        
        $PSInstance.Dispose()
        
        Write-Log "Script executed successfully: $ScriptName" "SUCCESS"
        return $true
    }
    catch {
        Write-Log "Script execution failed: $ScriptName - $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# Execute scripts sequentially
function Invoke-ScriptsSequential {
    param([array]$Scripts)
    
    Write-Log "Executing $($Scripts.Count) scripts sequentially" "INFO"
    
    $SuccessCount = 0
    $FailureCount = 0
    $TotalTime = 0
    
    foreach ($Script in $Scripts) {
        $StartTime = Get-Date
        
        if (Invoke-Script $Script) {
            $SuccessCount++
        } else {
            $FailureCount++
        }
        
        $EndTime = Get-Date
        $Duration = ($EndTime - $StartTime).TotalSeconds
        $TotalTime += $Duration
        
        Write-Log "Script completed in $Duration seconds" "INFO"
    }
    
    Write-Log "Sequential execution completed: $SuccessCount success, $FailureCount failures, $TotalTime total seconds" "SUCCESS"
    
    return @{
        "SuccessCount" = $SuccessCount
        "FailureCount" = $FailureCount
        "TotalTime" = $TotalTime
        "Mode" = "Sequential"
    }
}

# Execute scripts in parallel
function Invoke-ScriptsParallel {
    param([array]$Scripts)
    
    Write-Log "Executing $($Scripts.Count) scripts in parallel" "INFO"
    
    $Jobs = @()
    $MaxJobs = 4  # Limit parallel jobs to avoid system overload
    
    foreach ($Script in $Scripts) {
        # Wait for available job slot
        while ((Get-Job -State Running).Count -ge $MaxJobs) {
            Start-Sleep -Seconds 1
        }
        
        $ScriptName = Split-Path $Script -LeafBase
        
        if ($DryRun) {
            Write-Log "DRY RUN: Would start parallel job for $ScriptName" "WARNING"
            continue
        }
        
        # Start background job
        $Job = Start-Job -ScriptBlock {
            param($ScriptPath, $LogPath)
            
            try {
                # Import necessary modules
                Import-Module Microsoft.PowerShell.Utility -ErrorAction SilentlyContinue
                
                # Execute the script
                $Result = & $ScriptPath
                
                return @{
                    "Success" = $true
                    "Script" = $ScriptPath
                    "Result" = $Result
                    "Error" = $null
                }
            }
            catch {
                return @{
                    "Success" = $false
                    "Script" = $ScriptPath
                    "Result" = $null
                    "Error" = $_.Exception.Message
                }
            }
        } -ArgumentList $Script, $LogPath
        
        $Jobs += $Job
        Write-Log "Started parallel job for $ScriptName" "INFO"
    }
    
    # Wait for all jobs to complete
    $JobResults = Wait-Job -Job $Jobs | Receive-Job
    
    # Clean up jobs
    Remove-Job -Job $Jobs -Force
    
    # Process results
    $SuccessCount = 0
    $FailureCount = 0
    
    foreach ($JobResult in $JobResults) {
        if ($JobResult.Success) {
            $SuccessCount++
            Write-Log "Parallel job succeeded: $(Split-Path $JobResult.Script -LeafBase)" "SUCCESS"
        } else {
            $FailureCount++
            Write-Log "Parallel job failed: $(Split-Path $JobResult.Script -LeafBase) - $($JobResult.Error)" "ERROR"
        }
    }
    
    Write-Log "Parallel execution completed: $SuccessCount success, $FailureCount failures" "SUCCESS"
    
    return @{
        "SuccessCount" = $SuccessCount
        "FailureCount" = $FailureCount
        "TotalTime" = 0  # Parallel time is harder to measure accurately
        "Mode" = "Parallel"
    }
}

# Execute scripts as pipeline
function Invoke-ScriptsPipeline {
    param([array]$Scripts)
    
    Write-Log "Executing $($Scripts.Count) scripts as pipeline" "INFO"
    
    $SuccessCount = 0
    $FailureCount = 0
    $TotalTime = 0
    $PipelineData = @{}
    
    foreach ($Script in $Scripts) {
        $StartTime = Get-Date
        
        # Pass pipeline data from previous script
        $Parameters = @{}
        if ($PipelineData.Count -gt 0) {
            $Parameters["PipelineData"] = $PipelineData
        }
        
        if (Invoke-Script $Script $Parameters) {
            $SuccessCount++
            
            # Collect output for next script (simplified)
            $ScriptName = Split-Path $Script -LeafBase
            $PipelineData[$ScriptName] = @{
                "Completed" = $true
                "Timestamp" = Get-Date
                "Output" = "Script completed successfully"
            }
        } else {
            $FailureCount++
            # Stop pipeline on failure
            Write-Log "Pipeline stopped due to script failure" "WARNING"
            break
        }
        
        $EndTime = Get-Date
        $Duration = ($EndTime - $StartTime).TotalSeconds
        $TotalTime += $Duration
    }
    
    Write-Log "Pipeline execution completed: $SuccessCount success, $FailureCount failures, $TotalTime total seconds" "SUCCESS"
    
    return @{
        "SuccessCount" = $SuccessCount
        "FailureCount" = $FailureCount
        "TotalTime" = $TotalTime
        "Mode" = "Pipeline"
        "PipelineData" = $PipelineData
    }
}

# Generate HTML report
function New-Report {
    param(
        [hashtable]$ExecutionResults,
        [string]$ReportPath
    )
    
    $ReportContent = @"
<!DOCTYPE html>
<html>
<head>
    <title>MingGoRTS Script Integration Report</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }
        .header { background-color: #2c3e50; color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; }
        .summary { background-color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        .category { background-color: white; padding: 15px; margin-bottom: 15px; border-radius: 5px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        .success { color: #27ae60; }
        .failure { color: #e74c3c; }
        .info { color: #3498db; }
        table { width: 100%; border-collapse: collapse; margin-top: 10px; }
        th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }
        th { background-color: #f8f9fa; }
        .progress-bar { width: 100%; height: 20px; background-color: #ecf0f1; border-radius: 10px; overflow: hidden; }
        .progress-fill { height: 100%; background-color: #3498db; transition: width 0.3s ease; }
    </style>
</head>
<body>
    <div class="header">
        <h1>MingGoRTS Script Integration Report</h1>
        <p>Generated: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')</p>
        <p>Category: $Category | Mode: $Mode</p>
    </div>
    
    <div class="summary">
        <h2>Execution Summary</h2>
        <div class="progress-bar">
            <div class="progress-fill" style="width: $([math]::Round(($ExecutionResults.SuccessCount / ($ExecutionResults.SuccessCount + $ExecutionResults.FailureCount)) * 100))%"></div>
        </div>
        <p><strong>Success Rate:</strong> $([math]::Round(($ExecutionResults.SuccessCount / ($ExecutionResults.SuccessCount + $ExecutionResults.FailureCount)) * 100, 2))%</p>
        <p><strong>Total Scripts:</strong> $($ExecutionResults.SuccessCount + $ExecutionResults.FailureCount)</p>
        <p><strong>Successful:</strong> <span class="success">$($ExecutionResults.SuccessCount)</span></p>
        <p><strong>Failed:</strong> <span class="failure">$($ExecutionResults.FailureCount)</span></p>
        <p><strong>Total Time:</strong> $([math]::Round($ExecutionResults.TotalTime, 2)) seconds</p>
        <p><strong>Execution Mode:</strong> $($ExecutionResults.Mode)</p>
    </div>
"@
    
    # Add category breakdown
    $ReportContent += "<div class='category'><h2>Script Categories</h2><table>"
    $ReportContent += "<tr><th>Category</th><th>Script Count</th><th>Status</th></tr>"
    
    foreach ($Cat in $ScriptCategories.Keys) {
        $ScriptCount = $ScriptCategories[$Cat].Count
        $Status = if ($Category -eq "All" -or $Category -eq $Cat) { "Executed" } else { "Skipped" }
        $StatusClass = if ($Status -eq "Executed") { "success" } else { "info" }
        
        $ReportContent += "<tr><td>$Cat</td><td>$ScriptCount</td><td class='$StatusClass'>$Status</td></tr>"
    }
    
    $ReportContent += "</table></div>"
    
    # Add script details
    $ReportContent += "<div class='category'><h2>Script Details</h2><table>"
    $ReportContent += "<tr><th>Script Name</th><th>Category</th><th>Size (KB)</th><th>Estimated Time (s)</th><th>Parameters</th></tr>"
    
    foreach ($Script in $ScriptMetadata.Keys) {
        $Metadata = $ScriptMetadata[$Script]
        $SizeKB = [math]::Round($Metadata.Size / 1KB, 2)
        $Params = $Metadata.Parameters -join ", "
        
        $ReportContent += "<tr>"
        $ReportContent += "<td>$($Metadata.Name)</td>"
        $ReportContent += "<td>$($Metadata.Category)</td>"
        $ReportContent += "<td>$SizeKB</td>"
        $ReportContent += "<td>$($Metadata.EstimatedTime)</td>"
        $ReportContent += "<td>$Params</td>"
        $ReportContent += "</tr>"
    }
    
    $ReportContent += "</table></div>"
    
    $ReportContent += "</body></html>"
    
    # Ensure report directory exists
    $ReportDir = Split-Path -Parent $ReportPath
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    Set-Content -Path $ReportPath -Value $ReportContent -Encoding UTF8
    Write-Log "HTML report generated: $ReportPath" "SUCCESS"
}

# Main execution function
function Main {
    Write-Log "========================================" "INFO"
    Write-Log "MingGoRTS Script Integration System" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log "Category: $Category" "INFO"
    Write-Log "Mode: $Mode" "INFO"
    Write-Log "Dry Run: $DryRun" "INFO"
    Write-Log "" "INFO"
    
    # Initialize metadata
    Initialize-ScriptMetadata
    
    # Determine which scripts to execute
    $ScriptsToExecute = @()
    
    if ($Category -eq "All") {
        foreach ($Cat in $ScriptCategories.Keys) {
            $ScriptsToExecute += $ScriptCategories[$Cat]
        }
    } elseif ($ScriptCategories.ContainsKey($Category)) {
        $ScriptsToExecute = $ScriptCategories[$Category]
    } else {
        Write-Log "Invalid category: $Category" "ERROR"
        exit 1
    }
    
    # Filter existing scripts
    $ExistingScripts = $ScriptsToExecute | Where-Object { Test-Path $_ }
    $MissingScripts = $ScriptsToExecute | Where-Object { -not (Test-Path $_) }
    
    if ($MissingScripts.Count -gt 0) {
        Write-Log "Found $($MissingScripts.Count) missing scripts:" "WARNING"
        foreach ($Missing in $MissingScripts) {
            Write-Log "  - $Missing" "WARNING"
        }
    }
    
    Write-Log "Found $($ExistingScripts.Count) existing scripts to execute" "INFO"
    
    if ($ExistingScripts.Count -eq 0) {
        Write-Log "No scripts to execute" "WARNING"
        return
    }
    
    # Execute scripts based on mode
    $ExecutionResults = $null
    
    switch ($Mode) {
        "Sequential" {
            $ExecutionResults = Execute-ScriptsSequential $ExistingScripts
        }
        "Parallel" {
            $ExecutionResults = Execute-ScriptsParallel $ExistingScripts
        }
        "Pipeline" {
            $ExecutionResults = Execute-ScriptsPipeline $ExistingScripts
        }
        default {
            Write-Log "Invalid execution mode: $Mode. Using Sequential" "WARNING"
            $ExecutionResults = Execute-ScriptsSequential $ExistingScripts
        }
    }
    
    # Generate report
    Generate-Report $ExecutionResults $ReportPath
    
    Write-Log "" "INFO"
    Write-Log "Script Integration Completed!" "SUCCESS"
    Write-Log "Success: $($ExecutionResults.SuccessCount)" "INFO"
    Write-Log "Failures: $($ExecutionResults.FailureCount)" "INFO"
    Write-Log "Total Time: $([math]::Round($ExecutionResults.TotalTime, 2)) seconds" "INFO"
    Write-Log "Log file: $LogPath" "INFO"
    Write-Log "Report file: $ReportPath" "INFO"
    
    # Exit with appropriate code
    if ($ExecutionResults.FailureCount -gt 0) {
        exit 1
    } else {
        exit 0
    }
}

# Execute main function
Main
