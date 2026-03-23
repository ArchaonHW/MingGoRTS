# Potato AI BMAD Dialogue System - Simplified Version

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
            return @"
Potato Wisdom Response:

For your question, I recommend a systematic approach:

1. Deep Analysis - Understand the core of the problem
2. Strategy Planning - Plan solution steps
3. Practice Verification - Validate through actual operation
4. Continuous Improvement - Optimize based on results

Remember: True wisdom comes from continuous learning and practice.

Potato Wisdom: Be patient, one step at a time
"@
        }
        "WhatIs" {
            return @"
Potato Concept Analysis:

Let me analyze this concept deeply:

Essential Meaning:
- Not just surface phenomenon, but embodiment of deep principles
- Need to understand from multiple angles

Core Features:
- Systematic and coherent
- Embodies profound insight
- Can guide practical application

Practical Significance:
- Provide basis for decision-making
- Guide action direction
- Promote deep thinking

Potato Wisdom: Understand essence, master patterns
"@
        }
        default {
            return @"
Potato Wisdom Sharing:

Every question is an opportunity for growth. I suggest you:

- Maintain an open mind
- Think from multiple angles
- Seek practical solutions
- Continuously learn and improve

Wisdom is not in knowing all answers, but in asking the right questions.

Potato Wisdom: Questions are the beginning of wisdom
"@
        }
    }
}

function Get-TechResponse {
    param([string]$Intent, [string]$Context)
    
    switch ($Intent) {
        "Problem" {
            return @"
Potato Technical Analysis:

Problem Diagnosis:
1. Identify Symptoms - Clarify specific manifestations
2. Analyze Causes - Trace root causes
3. Assess Impact - Determine scope of impact

Solutions:
- Immediate measures: alleviate current problems
- Fundamental solution: eliminate root causes
- Preventive measures: avoid recurrence

Technical Suggestions:
- Use debugging tools to locate problems
- Check log files for detailed information
- Consider performance and security factors

Potato Tech: Problems are opportunities for optimization
"@
        }
        "Suggestion" {
            return @"
Potato Technical Suggestions:

Based on current situation, I suggest the following improvements:

Architecture Optimization:
- Modular design improves maintainability
- Adopt design patterns to improve code structure
- Implement dependency injection to reduce coupling

Performance Enhancement:
- Asynchronous processing improves response speed
- Caching mechanisms reduce redundant calculations
- Resource pooling optimizes memory usage

Quality Assurance:
- Unit testing ensures functional correctness
- Code reviews improve code quality
- Continuous integration ensures delivery quality

Potato Tech: Optimization is an eternal theme
"@
        }
        default {
            return @"
Potato Technical Sharing:

As a technical expert, I emphasize the following principles:

Technology Selection:
- Choose appropriate technology based on actual needs
- Consider team technical capabilities and learning costs
- Evaluate technology maturity and community support

Best Practices:
- Follow coding standards and design principles
- Value documentation writing and knowledge sharing
- Keep code clean and readable

Continuous Learning:
- Pay attention to technology trends and development directions
- Participate in open source projects and tech communities
- Share experience and help others grow

Potato Tech: Technology is a tool, thinking is the core
"@
        }
    }
}

function Get-CreativeResponse {
    param([string]$Intent, [string]$Context)
    
    switch ($Intent) {
        "HowTo" {
            return @"
Potato Creative Inspiration:

Let's solve this problem with innovative thinking!

Think Outside the Box:
- Imagine if there were no limitations, what would you do?
- Seek inspiration from completely different fields
- Reverse thinking: deduce steps from the goal

Creative Techniques:
- Associative Thinking: connect unrelated concepts
- Analogical Thinking: find similar situations in nature or life
- Recombine Elements: combine existing elements in new ways

Implementation Suggestions:
- First brainstorm without limits
- Then filter and optimize the most promising ideas
- Finally formulate feasible implementation plans

Potato Creative: Creativity comes from breaking conventions
"@
        }
        default {
            return @"
Potato Creative Dialogue:

Creativity is like a potato,看似普通 but full of possibilities!

Cultivate Creative Thinking:
- Maintain curiosity, ask "why not?"
- Observe details and patterns in life
- Exchange ideas with people from different backgrounds

Creative Practice:
- Set creative challenges for yourself
- Try new ways of expression and tools
- Learn and adjust from failures

Sharing and Collaboration:
- Creativity appreciates in sharing
- Cooperate with others to inspire more ideas
- Build an environment that supports creativity

Potato Creative: Everyone is a source of creativity
"@
        }
    }
}

function Get-DefaultResponse {
    param([string]$Intent, [string]$Context)
    
    return @"
Potato AI Response:

Thank you for your question! I have understood your intent is: $Intent

In the context of $Context, I suggest:

1. Deep Analysis - Understand the essence of the problem
2. Seek Resources - Utilize available tools and knowledge
3. Take Action - Formulate and execute solutions
4. Evaluate Results - Learn and improve from experience

Remember: Every problem is an opportunity for growth, every challenge is a step of progress.

Potato AI: Growing together with you
"@
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
