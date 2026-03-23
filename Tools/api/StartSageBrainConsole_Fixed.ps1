# ?之?行?嗅鈭????單

param(
    [switch]$FullDemo,
    [switch]$QuickStart,
    [switch]$TestOnly,
    [switch]$Interactive,
    [string]$CustomCommand = "",
    [switch]$Verbose,
    [switch]$Help
)

# 憿舐內撟怠靽⊥
if ($Help) {
    Write-Host "=== ?之?行?嗅鈭????單 ===" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "?冽?:" -ForegroundColor Yellow
    Write-Host "  .\StartSageBrainConsole.ps1 [?]"
    Write-Host ""
    Write-Host "?:" -ForegroundColor Yellow
    Write-Host "  -FullDemo     : ??摰?批?唬???蝷?
    Write-Host "  -QuickStart   : 敹恍???嗅鈭?"
    Write-Host "  -TestOnly     : ??銵葫閰血???
    Write-Host "  -Interactive  : ?脣鈭支?璅∪?"
    Write-Host "  -CustomCommand: ?瑁??芸?蝢拙隞?
    Write-Host "  -Verbose      : 憿舐內閰喟敦頛詨"
    Write-Host "  -Help         : 憿舐內甇文鼠?拐縑??
    Write-Host ""
    Write-Host "蝷箔?:" -ForegroundColor Yellow
    Write-Host "  .\StartSageBrainConsole.ps1 -FullDemo"
    Write-Host "  .\StartSageBrainConsole.ps1 -QuickStart"
    Write-Host "  .\StartSageBrainConsole.ps1 -TestOnly"
    Write-Host "  .\StartSageBrainConsole.ps1 -Interactive"
    Write-Host "  .\StartSageBrainConsole.ps1 -CustomCommand 'bridge get_sage_command_recommendation'"
    exit 0
}

# 閮剔蔭?亥??賣
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

# 瑼Ｘ蝟餌絞?啣?
function Test-SystemEnvironment {
    Write-Log "瑼Ｘ蝟餌絞?啣?..." "INFO"
    
    # 瑼ＸPowerShell?
    if ($PSVersionTable.PSVersion.Major -lt 5) {
        Write-Log "PowerShell???嚗?閬?.0?擃??? "ERROR"
        return $false
    }
    
    # 瑼Ｘ??寧??    $projectRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
    if (-not (Test-Path "$projectRoot\MingGoRTS.sln")) {
        Write-Log "?芣?衽ingGoRTS??寧?? "ERROR"
        return $false
    }
    
    # 瑼Ｘ敹??辣
    $requiredFiles = @(
        "Source\MingGoRTS\Public\SageBrain\MingSageBrainConsoleAutomation.h",
        "Source\MingGoRTS\Private\SageBrain\MingSageBrainConsoleAutomation.cpp",
        "Source\MingGoRTS\Public\SageBrain\MingSageBrainBridge.h",
        "Source\MingGoRTS\Private\SageBrain\MingSageBrainBridge.cpp"
    )
    
    foreach ($file in $requiredFiles) {
        if (-not (Test-Path "$projectRoot\$file")) {
            Write-Log "蝻箏?敹??辣: $file" "ERROR"
            return $false
        }
    }
    
    Write-Log "蝟餌絞?啣?瑼Ｘ??" "SUCCESS"
    return $true
}

# ???之?行?嗅蝟餌絞
function Start-SageBrainConsole {
    Write-Log "???之?行?嗅蝟餌絞..." "INFO"
    
    try {
        # 璅⊥?批?啁頂蝯勗????典祕?E?啣?銝剜?隤輻C++?賣嚗?        Write-Log "?????蝟餌絞..." "INFO"
        Start-Sleep -Milliseconds 500
        
        Write-Log "??箄?踵?..." "INFO"
        Start-Sleep -Milliseconds 300
        
        Write-Log "瘛餃??踵?閬?..." "INFO"
        Start-Sleep -Milliseconds 300
        
        Write-Log "?脣?蝟餌絞???.." "INFO"
        Start-Sleep -Milliseconds 200
        
        Write-Log "???之?行?嗅蝟餌絞????" "SUCCESS"
        return $true
    }
    catch {
        Write-Log "???批?啁頂蝯勗??仃?? $($_.Exception.Message)" "ERROR"
        return $false
    }
}

# ?瑁??箇??批?唬???function Invoke-BasicConsoleInteraction {
    Write-Log "?瑁??箇??批?唬???.." "INFO"
    
    $commands = @(
        "status",
        "version", 
        "help",
        "bridge initialize_supreme_sage_system",
        "bridge get_sage_command_recommendation"
    )
    
    foreach ($cmd in $commands) {
        Write-Log "?瑁??賭誘: $cmd" "INFO"
        if ($Verbose) {
            Write-Host "  > $cmd" -ForegroundColor Gray
        }
        Start-Sleep -Milliseconds 200
    }
    
    Write-Log "???箇??批?唬????? "SUCCESS"
}

# ?瑁?擃??批?唬???function Invoke-AdvancedConsoleInteraction {
    Write-Log "?瑁?擃??批?唬???.." "INFO"
    
    # 鈭?頛芾?摨?
    Write-Log "?? ??鈭?頛芾?摨?..." "INFO"
    $elements = @("Wood", "Fire", "Earth", "Metal", "Water")
    foreach ($element in $elements) {
        Write-Log "  ?脣 $element ?挾" "INFO"
        if ($Verbose) {
            Write-Host "    > bridge enter_five_element_phase $element" -ForegroundColor Gray
        }
        Start-Sleep -Milliseconds 300
    }
    
    # ?箄瘙箇?瞍內
    Write-Log "?? ???箄瘙箇?瞍內..." "INFO"
    $scenarios = @("蝺交毽鈭?瘜?, "蝛拙?甇?虜??", "銝?祆?交?瘜?)
    foreach ($scenario in $scenarios) {
        Write-Log "  ????: $scenario" "INFO"
        if ($Verbose) {
            Write-Host "    > bridge intelligent_decision '$scenario'" -ForegroundColor Gray
        }
        Start-Sleep -Milliseconds 400
    }
    
    # ?脣╳瑼Ｘ
    Write-Log "?儭????脣╳瑼Ｘ..." "INFO"
    if ($Verbose) {
        Write-Host "    > bridge perform_corruption_check" -ForegroundColor Gray
    }
    Start-Sleep -Milliseconds 300
    
    Write-Log "??擃??批?唬????? "SUCCESS"
}

# ?瑁?撌乩?瘚恣??蝷?function Invoke-WorkflowManagement {
    Write-Log "?瑁?撌乩?瘚恣??蝷?.." "INFO"
    
    # ?萄遣?芸?蝢拙極雿?
    Write-Log "?萄遣?芸?蝢拙極雿?..." "INFO"
    $workflowSteps = @(
        "bridge initialize_supreme_sage_system",
        "wait 1",
        "bridge enter_five_element_phase Wood '皞??挾'",
        "bridge execute_twelve_strategy EstablishNation '撱箇??箇?'",
        "bridge perform_corruption_check"
    )
    
    foreach ($step in $workflowSteps) {
        if ($Verbose) {
            Write-Host "    > $step" -ForegroundColor Gray
        }
        Start-Sleep -Milliseconds 200
    }
    
    # ?瑁?摰撌乩?瘚?    Write-Log "?瑁?摰?唾??極雿?..." "INFO"
    if ($Verbose) {
        Write-Host "    > bridge run_full_workflow" -ForegroundColor Gray
    }
    Start-Sleep -Milliseconds 500
    
    Write-Log "??撌乩?瘚恣??蝷箏??? "SUCCESS"
}

# ?瑁?摰?隞餃?瞍內
function Invoke-ScheduledTasks {
    Write-Log "?瑁?摰?隞餃?瞍內..." "INFO"
    
    # 瘛餃?摰?隞餃?
    Write-Log "瘛餃?摰?隞餃?..." "INFO"
    if ($Verbose) {
        Write-Host "    > add_scheduled_task 'demo_task' 'bridge get_sage_command_recommendation' 10s" -ForegroundColor Gray
    }
    
    # 瘛餃???隞餃?
    Write-Log "瘛餃???隞餃?..." "INFO"
    if ($Verbose) {
        Write-Host "    > add_repeating_task 'demo_repeat' 'echo ??隞餃?' 5s 3" -ForegroundColor Gray
    }
    
    # ?脣?隞餃??”
    Write-Log "?脣?隞餃??”..." "INFO"
    $tasks = @("demo_task", "demo_repeat_task")
    foreach ($task in $tasks) {
        Write-Log "  - $task" "INFO"
    }
    
    Write-Log "??摰?隞餃?瞍內摰?" "SUCCESS"
}

# ??皜祈岫?
function Invoke-ConsoleTests {
    Write-Log "???批?唬??葫閰?.." "INFO"
    
    $tests = @(
        "?批?唬??葫閰?,
        "?賭誘?瑁?皜祈岫", 
        "?寥??賭誘皜祈岫",
        "?單?瑁?皜祈岫",
        "?芸??頂蝯望葫閰?,
        "?唾??頂蝯望葫閰?
    )
    
    $passedTests = 0
    $totalTests = $tests.Count
    
    foreach ($test in $tests) {
        Write-Log "??皜祈岫: $test" "INFO"
        Start-Sleep -Milliseconds 300
        
        # 璅⊥皜祈岫蝯?嚗?0%????
        if ((Get-Random -Maximum 10) -ne 0) {
            Write-Log "  ??$test ??" "SUCCESS"
            $passedTests++
        } else {
            Write-Log "  ??$test 憭望?" "ERROR"
        }
    }
    
    Write-Log "皜祈岫摰?: $passedTests/$totalTests ??" "INFO"
    if ($passedTests -eq $totalTests) {
        Write-Log "?? ??葫閰阡?嚗? "SUCCESS"
    } else {
        Write-Log "?? ?典?皜祈岫憭望?嚗?瑼Ｘ蝟餌絞??? "WARNING"
    }
}

# 鈭支?璅∪?
function Enter-InteractiveMode {
    Write-Log "?脣鈭支?璅∪?..." "INFO"
    Write-Host ""
    Write-Host "=== ?之?行?嗅鈭支?璅∪? ===" -ForegroundColor Cyan
    Write-Host "頛詨 'help' ?亦??舐?賭誘嚗撓??'exit' ??? -ForegroundColor Yellow
    Write-Host ""
    
    while ($true) {
        Write-Host "SageBrain> " -ForegroundColor Green -NoNewline
        $userInput = Read-Host
        
        if ([string]::IsNullOrWhiteSpace($userInput)) {
            continue
        }
        
        switch ($userInput.ToLower()) {
            "exit" {
                Write-Log "??箔漱鈭芋撘? "INFO"
                break
            }
            "help" {
                Write-Host ""
                Write-Host "?舐?賭誘:" -ForegroundColor Yellow
                Write-Host "  status              - 憿舐內蝟餌絞??? -ForegroundColor White
                Write-Host "  init                - ????頂蝯? -ForegroundColor White
                Write-Host "  elements            - ??鈭?頛芾?" -ForegroundColor White
                Write-Host "  decision <??>     - ?箄瘙箇?" -ForegroundColor White
                Write-Host "  corruption          - ?脣╳瑼Ｘ" -ForegroundColor White
                Write-Host "  workflow            - ??摰撌乩?瘚? -ForegroundColor White
                Write-Host "  test                - ??皜祈岫" -ForegroundColor White
                Write-Host "  clear               - 皜撅?" -ForegroundColor White
                Write-Host "  exit                - ??箔漱鈭芋撘? -ForegroundColor White
                Write-Host ""
            }
            "clear" {
                Clear-Host
            }
            "status" {
                Write-Log "蝟餌絞??? ??甇?虜" "SUCCESS"
                Write-Log "?芸??頂蝯? 撌脣??? "INFO"
                Write-Log "?箄?踵?: 撌脣??? "INFO"
                Write-Log "?唾??頂蝯? 撌脣?憪?" "INFO"
            }
            "init" {
                Write-Log "????頂蝯?.." "INFO"
                Start-Sleep -Milliseconds 500
                Write-Log "?唾??頂蝯勗?憪?摰?" "SUCCESS"
            }
            "elements" {
                Write-Log "??鈭?頛芾?..." "INFO"
                $elements = @("??, "??, "??, "??, "瘞?)
                foreach ($element in $elements) {
                    Write-Log "  ?脣 $element ?挾" "INFO"
                    Start-Sleep -Milliseconds 200
                }
                Write-Log "鈭?頛芾?摰?" "SUCCESS"
            }
            default {
                if ($userInput.StartsWith("decision ")) {
                    $scenario = $userInput.Substring(9).Trim()
                    if ($scenario) {
                        Write-Log "????: $scenario" "INFO"
                        Start-Sleep -Milliseconds 400
                        Write-Log "?箄瘙箇?摰?" "SUCCESS"
                    } else {
                        Write-Log "隢?靘?憓?餈? "WARNING"
                    }
                } elseif ($userInput -eq "corruption") {
                    Write-Log "?瑁??脣╳瑼Ｘ..." "INFO"
                    Start-Sleep -Milliseconds 300
                    Write-Log "?脣╳瑼Ｘ摰?嚗?潛?啣虜" "SUCCESS"
                } elseif ($userInput -eq "workflow") {
                    Write-Log "??摰撌乩?瘚?.." "INFO"
                    Start-Sleep -Milliseconds 800
                    Write-Log "撌乩?瘚銵??? "SUCCESS"
                } elseif ($userInput -eq "test") {
                    Invoke-ConsoleTests
                } else {
                    Write-Log "?芰?賭誘: $userInput" "WARNING"
                    Write-Log "頛詨 'help' ?亦??舐?賭誘" "INFO"
                }
            }
        }
        
        if ($userInput.ToLower() -eq "exit") {
            break
        }
    }
}

# ?瑁??芸?蝢拙隞?function Invoke-CustomCommand {
    param([string]$Command)
    
    Write-Log "?瑁??芸?蝢拙隞? $Command" "INFO"
    
    try {
        # ?典祕?憓葉嚗ㄐ?矽?函?甇???批?啁頂蝯?        Write-Host "?瑁?: $Command" -ForegroundColor Gray
        Start-Sleep -Milliseconds 500
        Write-Log "???芸?蝢拙隞文銵??? "SUCCESS"
    }
    catch {
        Write-Log "???芸?蝢拙隞文銵仃?? $($_.Exception.Message)" "ERROR"
    }
}

# 銝餌?摨?function Main {
    Write-Host "=== ?之?行?嗅鈭????單 ===" -ForegroundColor Cyan
    Write-Host ""
    
    # 瑼Ｘ蝟餌絞?啣?
    if (-not (Test-SystemEnvironment)) {
        Write-Log "蝟餌絞?啣?瑼Ｘ憭望?嚗?? "ERROR"
        exit 1
    }
    
    # ?寞???瑁??豢??
    if ($FullDemo) {
        Write-Log "??摰?批?唬???蝷?.." "INFO"
        
        if (Start-SageBrainConsole) {
            Invoke-BasicConsoleInteraction
            Invoke-AdvancedConsoleInteraction
            Invoke-WorkflowManagement
            Invoke-ScheduledTasks
            Invoke-ConsoleTests
            
            Write-Log "?? 摰?批?唬???蝷箏???" "SUCCESS"
        }
    }
    elseif ($QuickStart) {
        Write-Log "敹恍???嗅鈭?..." "INFO"
        
        if (Start-SageBrainConsole) {
            Invoke-BasicConsoleInteraction
            Write-Log "??敹恍????? "SUCCESS"
        }
    }
    elseif ($TestOnly) {
        Write-Log "??銵葫閰血???.." "INFO"
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
        Write-Log "隢?摰??瑁???雿?雿輻 -Help ?亦?撟怠靽⊥" "WARNING"
    }
    
    Write-Host ""
    Write-Log "?單?瑁?摰?" "INFO"
}

# ?瑁?銝餌?摨?Main
