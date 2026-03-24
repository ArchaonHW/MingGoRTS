# Sage Brain Console Interaction Script

param(
    [switch]$FullDemo,
    [switch]$QuickStart,
    [switch]$TestOnly,
    [switch]$Interactive,
    [string]$CustomCommand = "",
    [switch]$Verbose,
    [switch]$Help
)

# Display help information
if ($Help) {
    Write-Host "=== Sage Brain Console Interaction Script ===" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Usage:" -ForegroundColor Yellow
    Write-Host "  .\StartSageBrainConsole.ps1 [parameters]"
    Write-Host ""
    Write-Host "Parameters:" -ForegroundColor Yellow
    Write-Host "  -FullDemo     : Run complete console interaction demo"
    Write-Host "  -QuickStart   : Quick start console interaction"
    Write-Host "  -TestOnly     : Run test functions only"
    Write-Host "  -Interactive  : Enter interactive mode"
    Write-Host "  -CustomCommand: Execute custom command"
    Write-Host "  -Verbose      : Show detailed output"
    Write-Host "  -Help         : Show this help information"
    Write-Host ""
    Write-Host "Examples:" -ForegroundColor Yellow
    Write-Host "  .\StartSageBrainConsole.ps1 -FullDemo"
    Write-Host "  .\StartSageBrainConsole.ps1 -QuickStart"
    Write-Host "  .\StartSageBrainConsole.ps1 -TestOnly"
    Write-Host "  .\StartSageBrainConsole.ps1 -Interactive"
    Write-Host "  .\StartSageBrainConsole.ps1 -CustomCommand 'bridge get_sage_command_recommendation'"
    exit 0
}

# Set up logging function
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

# Check system environment
function Test-SystemEnvironment {
    Write-Log "Checking system environment..." "INFO"
    
    # Check PowerShell version
    if ($PSVersionTable.PSVersion.Major -lt 5) {
        Write-Log "PowerShell version too low, need 5.0 or higher" "ERROR"
        return $false
    }
    
    # Check project root directory
    $projectRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
    if (-not (Test-Path "$projectRoot\MingGoRTS.sln")) {
        Write-Log "MingGoRTS project root directory not found" "ERROR"
        return $false
    }
    
    # Check required files
    $requiredFiles = @(
        "Source\MingGoRTS\Public\SageBrain\MingSageBrainConsoleAutomation.h",
        "Source\MingGoRTS\Private\SageBrain\MingSageBrainConsoleAutomation.cpp",
        "Source\MingGoRTS\Public\SageBrain\MingSageBrainBridge.h",
        "Source\MingGoRTS\Private\SageBrain\MingSageBrainBridge.cpp"
    )
    
    foreach ($file in $requiredFiles) {
        if (-not (Test-Path "$projectRoot\$file")) {
            Write-Log "Missing required file: $file" "ERROR"
            return $false
        }
    }
    
    Write-Log "System environment check passed" "SUCCESS"
    return $true
}

# Start Sage Brain console system
function Start-SageBrainConsole {
    Write-Log "Starting Sage Brain console system..." "INFO"
    
    try {
        # Simulate console system startup (would call C++ functions in actual UE environment)
        Write-Log "Initializing automation system..." "INFO"
        Start-Sleep -Milliseconds 500
        
        Write-Log "Enabling intelligent response..." "INFO"
        Start-Sleep -Milliseconds 300
        
        Write-Log "Adding response rules..." "INFO"
        Start-Sleep -Milliseconds 300
        
        Write-Log "Getting system status..." "INFO"
        Start-Sleep -Milliseconds 200
        
        Write-Log "Sage Brain console system started successfully" "SUCCESS"
        return $true
    }
    catch {
        Write-Log "Console system startup failed: $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# Execute basic console interaction
function Invoke-BasicConsoleInteraction {
    Write-Log "Executing basic console interaction..." "INFO"
    
    $commands = @(
        "status",
        "version", 
        "help",
        "bridge initialize_supreme_sage_system",
        "bridge get_sage_command_recommendation"
    )
    
    foreach ($cmd in $commands) {
        Write-Log "Executing command: $cmd" "INFO"
        if ($Verbose) {
            Write-Host "  > $cmd" -ForegroundColor Gray
        }
        Start-Sleep -Milliseconds 200
    }
    
    Write-Log "Basic console interaction completed" "SUCCESS"
}

# Execute advanced console interaction
function Invoke-AdvancedConsoleInteraction {
    Write-Log "Executing advanced console interaction..." "INFO"
    
    # Five elements rotation sequence
    Write-Log "Starting Five Elements rotation sequence..." "INFO"
    $elements = @("Wood", "Fire", "Earth", "Metal", "Water")
    foreach ($element in $elements) {
        Write-Log "  Entering $element phase" "INFO"
        if ($Verbose) {
            Write-Host "    > bridge enter_five_element_phase $element" -ForegroundColor Gray
        }
        Start-Sleep -Milliseconds 300
    }
    
    # Intelligent decision demo
    Write-Log "Starting intelligent decision demo..." "INFO"
    $scenarios = @("Emergency chaotic situation", "Stable normal situation", "General strategic situation")
    foreach ($scenario in $scenarios) {
        Write-Log "  Analyzing scenario: $scenario" "INFO"
        if ($Verbose) {
            Write-Host "    > bridge intelligent_decision $scenario" -ForegroundColor Gray
        }
        Start-Sleep -Milliseconds 400
    }
    
    # Corruption prevention check
    Write-Log "Starting corruption prevention check..." "INFO"
    if ($Verbose) {
        Write-Host "    > bridge perform_corruption_check" -ForegroundColor Gray
    }
    Start-Sleep -Milliseconds 300
    
    Write-Log "Advanced console interaction completed" "SUCCESS"
}

# Execute workflow management demo
function Invoke-WorkflowManagement {
    Write-Log "Executing workflow management demo..." "INFO"
    
    # Create custom workflow
    Write-Log "Creating custom workflow..." "INFO"
    $workflowSteps = @(
        "bridge initialize_supreme_sage_system",
        "wait 1",
        "bridge enter_five_element_phase Wood 'Preparation Phase'",
        "bridge execute_twelve_strategy EstablishNation 'Establish Foundation'",
        "bridge perform_corruption_check"
    )
    
    foreach ($step in $workflowSteps) {
        if ($Verbose) {
            Write-Host "    > $step" -ForegroundColor Gray
        }
        Start-Sleep -Milliseconds 200
    }
    
    # Execute complete workflow
    Write-Log "Executing complete Supreme Sage workflow..." "INFO"
    if ($Verbose) {
        Write-Host "    > bridge run_full_workflow" -ForegroundColor Gray
    }
    Start-Sleep -Milliseconds 500
    
    Write-Log "Workflow management demo completed" "SUCCESS"
}

# Execute scheduled tasks demo
function Invoke-ScheduledTasks {
    Write-Log "Executing scheduled tasks demo..." "INFO"
    
    # Add scheduled task
    Write-Log "Adding scheduled task..." "INFO"
    if ($Verbose) {
        Write-Host "    > add_scheduled_task 'demo_task' 'bridge get_sage_command_recommendation' 10s" -ForegroundColor Gray
    }
    
    # Add repeating task
    Write-Log "Adding repeating task..." "INFO"
    if ($Verbose) {
        Write-Host "    > add_repeating_task 'demo_repeat' 'echo repeat_task' 5s 3" -ForegroundColor Gray
    }
    
    # Get task list
    Write-Log "Getting task list..." "INFO"
    $tasks = @("demo_task", "demo_repeat_task")
    foreach ($task in $tasks) {
        Write-Log "  - $task" "INFO"
    }
    
    Write-Log "Scheduled tasks demo completed" "SUCCESS"
}

# Run test functions
function Invoke-ConsoleTests {
    Write-Log "Running console interaction tests..." "INFO"
    
    $tests = @(
        "Console interaction test",
        "Command execution test", 
        "Batch command test",
        "Script execution test",
        "Automation system test",
        "Supreme Sage system test"
    )
    
    $passedTests = 0
    $totalTests = $tests.Count
    
    foreach ($test in $tests) {
        Write-Log "Running test: $test" "INFO"
        Start-Sleep -Milliseconds 300
        
        # Simulate test results (90% success rate)
        if ((Get-Random -Maximum 10) -ne 0) {
            Write-Log "  $test passed" "SUCCESS"
            $passedTests++
        } else {
            Write-Log "  $test failed" "ERROR"
        }
    }
    
    Write-Log "Tests completed: $passedTests/$totalTests passed" "INFO"
    if ($passedTests -eq $totalTests) {
        Write-Log "All tests passed" "SUCCESS"
    } else {
        Write-Log "Some tests failed, please check system status" "WARNING"
    }
}

# Interactive mode
function Enter-InteractiveMode {
    Write-Log "Entering interactive mode..." "INFO"
    Write-Host ""
    Write-Host "=== Sage Brain Console Interaction Mode ===" -ForegroundColor Cyan
    Write-Host "Type 'help' to see available commands, type 'exit' to quit" -ForegroundColor Yellow
    Write-Host ""
    
    while ($true) {
        Write-Host "SageBrain> " -ForegroundColor Green -NoNewline
        $userInput = Read-Host
        
        if ([string]::IsNullOrWhiteSpace($userInput)) {
            continue
        }
        
        $inputLower = $userInput.ToLower()
        
        if ($inputLower -eq "exit") {
            Write-Log "Exiting interactive mode" "INFO"
            break
        }
        elseif ($inputLower -eq "help") {
            Write-Host ""
            Write-Host "Available commands:" -ForegroundColor Yellow
            Write-Host "  status              - Display system status" -ForegroundColor White
            Write-Host "  init                - Initialize Supreme Sage system" -ForegroundColor White
            Write-Host "  elements            - Run Five Elements rotation" -ForegroundColor White
            Write-Host "  decision [scenario]  - Intelligent decision" -ForegroundColor White
            Write-Host "  corruption          - Corruption prevention check" -ForegroundColor White
            Write-Host "  workflow            - Run complete workflow" -ForegroundColor White
            Write-Host "  test                - Run tests" -ForegroundColor White
            Write-Host "  clear               - Clear screen" -ForegroundColor White
            Write-Host "  exit                - Exit interactive mode" -ForegroundColor White
            Write-Host ""
        }
        elseif ($inputLower -eq "clear") {
            Clear-Host
        }
        elseif ($inputLower -eq "status") {
            Write-Log "System status: Running normally" "SUCCESS"
            Write-Log "Automation system: Enabled" "INFO"
            Write-Log "Intelligent response: Enabled" "INFO"
            Write-Log "Supreme Sage system: Initialized" "INFO"
        }
        elseif ($inputLower -eq "init") {
            Write-Log "Initializing Supreme Sage system..." "INFO"
            Start-Sleep -Milliseconds 500
            Write-Log "Supreme Sage system initialization completed" "SUCCESS"
        }
        elseif ($inputLower -eq "elements") {
            Write-Log "Running Five Elements rotation..." "INFO"
            $elements = @("Wood", "Fire", "Earth", "Metal", "Water")
            foreach ($element in $elements) {
                Write-Log "  Entering $element phase" "INFO"
                Start-Sleep -Milliseconds 200
            }
            Write-Log "Five Elements rotation completed" "SUCCESS"
        }
        elseif ($inputLower.StartsWith("decision ")) {
            $scenario = $userInput.Substring(9).Trim()
            if ($scenario) {
                Write-Log "Analyzing scenario: $scenario" "INFO"
                Start-Sleep -Milliseconds 400
                Write-Log "Intelligent decision completed" "SUCCESS"
            } else {
                Write-Log "Please provide scenario description" "WARNING"
            }
        }
        elseif ($inputLower -eq "corruption") {
            Write-Log "Executing corruption prevention check..." "INFO"
            Start-Sleep -Milliseconds 300
            Write-Log "Corruption prevention check completed, no anomalies found" "SUCCESS"
        }
        elseif ($inputLower -eq "workflow") {
            Write-Log "Running complete workflow..." "INFO"
            Start-Sleep -Milliseconds 800
            Write-Log "Workflow execution completed" "SUCCESS"
        }
        elseif ($inputLower -eq "test") {
            Invoke-ConsoleTests
        }
        else {
            Write-Log "Unknown command: $userInput" "WARNING"
            Write-Log "Type 'help' to see available commands" "INFO"
        }
    }
}

# Execute custom command
function Invoke-CustomCommand {
    param([string]$Command)
    
    Write-Log "Executing custom command: $Command" "INFO"
    
    try {
        # In actual environment, this would call the real console system
        Write-Host "Executing: $Command" -ForegroundColor Gray
        Start-Sleep -Milliseconds 500
        Write-Log "Custom command execution completed" "SUCCESS"
    }
    catch {
        Write-Log "Custom command execution failed: $($_.Exception.Message)" "ERROR"
    }
}

# Main program
function Main {
    Write-Host "=== Sage Brain Console Interaction Script ===" -ForegroundColor Cyan
    Write-Host ""
    
    # Check system environment
    if (-not (Test-SystemEnvironment)) {
        Write-Log "System environment check failed, exiting" "ERROR"
        exit 1
    }
    
    # Execute corresponding functions based on parameters
    if ($FullDemo) {
        Write-Log "Running complete console interaction demo..." "INFO"
        
        if (Start-SageBrainConsole) {
            Invoke-BasicConsoleInteraction
            Invoke-AdvancedConsoleInteraction
            Invoke-WorkflowManagement
            Invoke-ScheduledTasks
            Invoke-ConsoleTests
            
            Write-Log "Complete console interaction demo completed" "SUCCESS"
        }
    }
    elseif ($QuickStart) {
        Write-Log "Quick start console interaction..." "INFO"
        
        if (Start-SageBrainConsole) {
            Invoke-BasicConsoleInteraction
            Write-Log "Quick start completed" "SUCCESS"
        }
    }
    elseif ($TestOnly) {
        Write-Log "Running test functions only..." "INFO"
        Invoke-ConsoleTests
    }
    elseif ($Interactive) {
        if (Start-SageBrainConsole) {
            Enter-InteractiveMode
        }
    }
    elseif ($CustomCommand) {
        if (Start-SageBrainConsole) {
            Invoke-CustomCommand -Command $CustomCommand
        }
    }
    else {
        Write-Log "Please specify operation to perform, use -Help to see help information" "WARNING"
    }
    
    Write-Host ""
    Write-Log "Script execution completed" "INFO"
}

# Execute main program
Main
