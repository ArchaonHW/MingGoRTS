# Potato AI BMAD Dialogue System - Minimal Version

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
    $Response = Get-ResponseByPersona -Intent $Intent -Persona $Persona -Context $Context
    
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

function Get-ResponseByPersona {
    param([string]$Intent, [string]$Persona, [string]$Context)
    
    switch ($Persona) {
        "Wisdom" { return Get-WisdomResponse -Intent $Intent -Context $Context }
        "Tech" { return Get-TechResponse -Intent $Intent -Context $Context }
        "Creative" { return Get-CreativeResponse -Intent $Intent -Context $Context }
        default { return Get-DefaultResponse -Intent $Intent -Context $Context }
    }
}

function Get-WisdomResponse {
    param([string]$Intent, [string]$Context)
    
    switch ($Intent) {
        "HowTo" {
            return "Potato Wisdom: For your question, I recommend a systematic approach: 1) Deep Analysis, 2) Strategy Planning, 3) Practice Verification, 4) Continuous Improvement. Remember: True wisdom comes from continuous learning and practice."
        }
        "WhatIs" {
            return "Potato Wisdom: Let me analyze this concept deeply. Essential Meaning: Not just surface phenomenon, but embodiment of deep principles. Core Features: Systematic and coherent, embodies profound insight, can guide practical application."
        }
        default {
            return "Potato Wisdom: Every question is an opportunity for growth. I suggest you maintain an open mind, think from multiple angles, seek practical solutions, and continuously learn and improve."
        }
    }
}

function Get-TechResponse {
    param([string]$Intent, [string]$Context)
    
    switch ($Intent) {
        "Problem" {
            return "Potato Tech: Problem Diagnosis: 1) Identify Symptoms, 2) Analyze Causes, 3) Assess Impact. Solutions: Immediate measures, fundamental solution, preventive measures. Technical Suggestions: Use debugging tools, check log files, consider performance and security factors."
        }
        "Suggestion" {
            return "Potato Tech: Based on current situation, I suggest improvements in: Architecture Optimization (modular design, design patterns, dependency injection), Performance Enhancement (asynchronous processing, caching, resource pooling), and Quality Assurance (unit testing, code reviews, continuous integration)."
        }
        default {
            return "Potato Tech: As a technical expert, I emphasize Technology Selection (choose based on needs, consider team capabilities, evaluate maturity), Best Practices (follow standards, value documentation, keep code clean), and Continuous Learning (follow trends, participate in communities, share experience)."
        }
    }
}

function Get-CreativeResponse {
    param([string]$Intent, [string]$Context)
    
    switch ($Intent) {
        "HowTo" {
            return "Potato Creative: Let's solve this with innovative thinking! Think Outside the Box: imagine no limitations, seek inspiration from different fields, use reverse thinking. Creative Techniques: associative thinking, analogical thinking, recombine elements. Creativity comes from breaking conventions."
        }
        default {
            return "Potato Creative: Creativity is like a potato -看似普通 but full of possibilities! Cultivate Creative Thinking: maintain curiosity, observe details, exchange ideas. Creative Practice: set challenges, try new tools, learn from failures. Everyone is a source of creativity."
        }
    }
}

function Get-DefaultResponse {
    param([string]$Intent, [string]$Context)
    
    return "Potato AI: Thank you for your question! I understood your intent is: $Intent. In context of $Context, I suggest: 1) Deep Analysis, 2) Seek Resources, 3) Take Action, 4) Evaluate Results. Remember: Every problem is an opportunity for growth. Potato AI: Growing together with you."
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
