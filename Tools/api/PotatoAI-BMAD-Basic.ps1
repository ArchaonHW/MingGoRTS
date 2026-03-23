# Potato AI BMAD Dialogue System - Basic Version

Write-Host "Loading Potato AI BMAD Dialogue System..." -ForegroundColor Green

function Invoke-PotatoBMADDialogue {
    param(
        [string]$UserMessage,
        [string]$Context = "General",
        [string]$Persona = "Wisdom"
    )
    
    Write-Host "Potato BMAD Dialogue Started..." -ForegroundColor Yellow
    Write-Host "User Message: $UserMessage" -ForegroundColor Cyan
    Write-Host "Context: $Context" -ForegroundColor Cyan
    Write-Host "Persona: $Persona" -ForegroundColor Cyan
    
    # Analyze user intent
    $Intent = Get-UserIntent -Message $UserMessage
    
    # Generate response based on persona
    $Response = switch ($Persona) {
        "Wisdom" { Get-WisdomResponse -Intent $Intent -Context $Context }
        "Tech" { Get-TechResponse -Intent $Intent -Context $Context }
        "Creative" { Get-CreativeResponse -Intent $Intent -Context $Context }
        default { Get-DefaultResponse -Intent $Intent -Context $Context }
    }
    
    Write-Host "Response Generated:" -ForegroundColor Green
    Write-Host $Response -ForegroundColor White
    
    return $Response
}

function Get-UserIntent {
    param([string]$Message)
    
    if ($Message -match "how to|如何|怎麼") {
        return "HowTo"
    } elseif ($Message -match "what is|什麼|定義") {
        return "WhatIs"
    } elseif ($Message -match "why|為什麼|原因") {
        return "Why"
    } elseif ($Message -match "problem|問題|錯誤|bug") {
        return "Problem"
    } elseif ($Message -match "suggestion|建議|改進") {
        return "Suggestion"
    } else {
        return "General"
    }
}

function Get-WisdomResponse {
    param([string]$Intent, [string]$Context)
    
    switch ($Intent) {
        "HowTo" {
            $Response = "Potato Wisdom Response: `n`n"
            $Response += "For your question, I recommend a systematic approach: `n`n"
            $Response += "1. Deep Analysis - Understand the core of the problem `n"
            $Response += "2. Strategy Planning - Plan solution steps `n"
            $Response += "3. Practice Verification - Validate through actual operation `n"
            $Response += "4. Continuous Improvement - Optimize based on results `n`n"
            $Response += "Remember: True wisdom comes from continuous learning and practice. `n`n"
            $Response += "Potato Wisdom: Be patient, one step at a time"
            return $Response
        }
        "WhatIs" {
            $Response = "Potato Concept Analysis: `n`n"
            $Response += "Let me analyze this concept deeply: `n`n"
            $Response += "Essential Meaning: `n"
            $Response += "- Not just surface phenomenon, but embodiment of deep principles `n"
            $Response += "- Need to understand from multiple angles `n`n"
            $Response += "Core Features: `n"
            $Response += "- Systematic and coherent `n"
            $Response += "- Embodies profound insight `n"
            $Response += "- Can guide practical application `n`n"
            $Response += "Potato Wisdom: Understand essence, master patterns"
            return $Response
        }
        default {
            $Response = "Potato Wisdom Sharing: `n`n"
            $Response += "Every question is an opportunity for growth. I suggest you: `n`n"
            $Response += "- Maintain an open mind `n"
            $Response += "- Think from multiple angles `n"
            $Response += "- Seek practical solutions `n"
            $Response += "- Continuously learn and improve `n`n"
            $Response += "Wisdom is not in knowing all answers, but in asking the right questions. `n`n"
            $Response += "Potato Wisdom: Questions are the beginning of wisdom"
            return $Response
        }
    }
}

function Get-TechResponse {
    param([string]$Intent, [string]$Context)
    
    switch ($Intent) {
        "Problem" {
            $Response = "Potato Technical Analysis: `n`n"
            $Response += "Problem Diagnosis: `n"
            $Response += "1. Identify Symptoms - Clarify specific manifestations `n"
            $Response += "2. Analyze Causes - Trace root causes `n"
            $Response += "3. Assess Impact - Determine scope of impact `n`n"
            $Response += "Solutions: `n"
            $Response += "- Immediate measures: alleviate current problems `n"
            $Response += "- Fundamental solution: eliminate root causes `n"
            $Response += "- Preventive measures: avoid recurrence `n`n"
            $Response += "Technical Suggestions: `n"
            $Response += "- Use debugging tools to locate problems `n"
            $Response += "- Check log files for detailed information `n"
            $Response += "- Consider performance and security factors `n`n"
            $Response += "Potato Tech: Problems are opportunities for optimization"
            return $Response
        }
        "Suggestion" {
            $Response = "Potato Technical Suggestions: `n`n"
            $Response += "Based on current situation, I suggest the following improvements: `n`n"
            $Response += "Architecture Optimization: `n"
            $Response += "- Modular design improves maintainability `n"
            $Response += "- Adopt design patterns to improve code structure `n"
            $Response += "- Implement dependency injection to reduce coupling `n`n"
            $Response += "Performance Enhancement: `n"
            $Response += "- Asynchronous processing improves response speed `n"
            $Response += "- Caching mechanisms reduce redundant calculations `n"
            $Response += "- Resource pooling optimizes memory usage `n`n"
            $Response += "Potato Tech: Optimization is an eternal theme"
            return $Response
        }
        default {
            $Response = "Potato Technical Sharing: `n`n"
            $Response += "As a technical expert, I emphasize the following principles: `n`n"
            $Response += "Technology Selection: `n"
            $Response += "- Choose appropriate technology based on actual needs `n"
            $Response += "- Consider team technical capabilities and learning costs `n"
            $Response += "- Evaluate technology maturity and community support `n`n"
            $Response += "Best Practices: `n"
            $Response += "- Follow coding standards and design principles `n"
            $Response += "- Value documentation writing and knowledge sharing `n"
            $Response += "- Keep code clean and readable `n`n"
            $Response += "Potato Tech: Technology is a tool, thinking is the core"
            return $Response
        }
    }
}

function Get-CreativeResponse {
    param([string]$Intent, [string]$Context)
    
    switch ($Intent) {
        "HowTo" {
            $Response = "Potato Creative Inspiration: `n`n"
            $Response += "Let's solve this problem with innovative thinking! `n`n"
            $Response += "Think Outside the Box: `n"
            $Response += "- Imagine if there were no limitations, what would you do? `n"
            $Response += "- Seek inspiration from completely different fields `n"
            $Response += "- Reverse thinking: deduce steps from the goal `n`n"
            $Response += "Creative Techniques: `n"
            $Response += "- Associative Thinking: connect unrelated concepts `n"
            $Response += "- Analogical Thinking: find similar situations in nature or life `n"
            $Response += "- Recombine Elements: combine existing elements in new ways `n`n"
            $Response += "Potato Creative: Creativity comes from breaking conventions"
            return $Response
        }
        default {
            $Response = "Potato Creative Dialogue: `n`n"
            $Response += "Creativity is like a potato,看似普通 but full of possibilities! `n`n"
            $Response += "Cultivate Creative Thinking: `n"
            $Response += "- Maintain curiosity, ask 'why not?' `n"
            $Response += "- Observe details and patterns in life `n"
            $Response += "- Exchange ideas with people from different backgrounds `n`n"
            $Response += "Creative Practice: `n"
            $Response += "- Set creative challenges for yourself `n"
            $Response += "- Try new ways of expression and tools `n"
            $Response += "- Learn and adjust from failures `n`n"
            $Response += "Potato Creative: Everyone is a source of creativity"
            return $Response
        }
    }
}

function Get-DefaultResponse {
    param([string]$Intent, [string]$Context)
    
    $Response = "Potato AI Response: `n`n"
    $Response += "Thank you for your question! I have understood your intent is: $Intent `n`n"
    $Response += "In the context of $Context, I suggest: `n`n"
    $Response += "1. Deep Analysis - Understand the essence of the problem `n"
    $Response += "2. Seek Resources - Utilize available tools and knowledge `n"
    $Response += "3. Take Action - Formulate and execute solutions `n"
    $Response += "4. Evaluate Results - Learn and improve from experience `n`n"
    $Response += "Remember: Every problem is an opportunity for growth, every challenge is a step of progress. `n`n"
    $Response += "Potato AI: Growing together with you"
    return $Response
}

function Start-PotatoBMADChat {
    param(
        [string]$Persona = "Wisdom",
        [string]$Context = "General"
    )
    
    Write-Host "Starting Potato BMAD Chat..." -ForegroundColor Yellow
    Write-Host "Persona: $Persona" -ForegroundColor Cyan
    Write-Host "Context: $Context" -ForegroundColor Cyan
    Write-Host "Type 'exit' to end conversation" -ForegroundColor Gray
    Write-Host "========================================" -ForegroundColor Yellow
    
    while ($true) {
        Write-Host "`nYou: " -ForegroundColor White -NoNewline
        $UserInput = Read-Host
        
        if ($UserInput -eq "exit") {
            Write-Host "Chat ended, looking forward to next conversation!" -ForegroundColor Green
            break
        }
        
        if ([string]::IsNullOrWhiteSpace($UserInput)) {
            continue
        }
        
        $Response = Invoke-PotatoBMADDialogue -UserMessage $UserInput -Context $Context -Persona $Persona
        Write-Host "`nPotato AI: $Response" -ForegroundColor Gray
    }
}

function Invoke-QuickBMAD {
    param([string]$Message)
    
    return Invoke-PotatoBMADDialogue -UserMessage $Message -Context "QuickChat" -Persona "Wisdom"
}

Write-Host "Potato AI BMAD Dialogue System Loaded Successfully!" -ForegroundColor Green
Write-Host "Available Functions:" -ForegroundColor Cyan
Write-Host "   - Invoke-PotatoBMADDialogue - Detailed BMAD dialogue" -ForegroundColor White
Write-Host "   - Start-PotatoBMADChat - Interactive chat" -ForegroundColor White
Write-Host "   - Invoke-QuickBMAD - Quick dialogue" -ForegroundColor White
Write-Host "Example: Invoke-QuickBMAD 'How to improve work efficiency?'" -ForegroundColor Gray
