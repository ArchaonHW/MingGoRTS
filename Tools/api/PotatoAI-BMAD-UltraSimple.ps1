# Potato AI BMAD Dialogue System - Ultra Simple Version

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
    
    # Generate response based on persona
    $Response = Get-ResponseByPersona -Persona $Persona -Message $UserMessage
    
    Write-Host "Response Generated:" -ForegroundColor Green
    Write-Host $Response -ForegroundColor White
    
    return $Response
}

function Get-ResponseByPersona {
    param([string]$Persona, [string]$Message)
    
    if ($Message -match "how to|如何|怎麼") {
        return Get-HowToResponse -Persona $Persona
    } elseif ($Message -match "what is|什麼|定義") {
        return Get-WhatIsResponse -Persona $Persona
    } elseif ($Message -match "problem|問題|錯誤|bug") {
        return Get-ProblemResponse -Persona $Persona
    } elseif ($Message -match "suggestion|建議|改進") {
        return Get-SuggestionResponse -Persona $Persona
    } else {
        return Get-GeneralResponse -Persona $Persona
    }
}

function Get-HowToResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "Potato Wisdom: For your question, I recommend a systematic approach. First, understand the problem deeply. Second, plan your strategy carefully. Third, practice and verify. Finally, continuously improve. Remember, wisdom comes from learning and practice." 
        }
        "Tech" { 
            return "Potato Tech: For technical questions, I suggest: analyze the requirements, choose appropriate tools, implement step by step, test thoroughly, and document your process. Technology is a tool, thinking is the core." 
        }
        "Creative" { 
            return "Potato Creative: Let us think outside the box! Imagine there are no limitations. Seek inspiration from different fields. Try combining existing ideas in new ways. Creativity comes from breaking conventions." 
        }
        default { 
            return "Potato AI: Thank you for your question. I suggest you break down the problem into smaller steps, seek relevant resources, take action, and learn from the results." 
        }
    }
}

function Get-WhatIsResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "Potato Wisdom: Understanding concepts requires looking beyond surface appearances. Consider multiple perspectives, identify core principles, and connect with practical applications. True understanding comes from deep reflection." 
        }
        "Tech" { 
            return "Potato Tech: Technical concepts often have layers. Start with the basic definition, understand the purpose, learn the implementation details, and practice with examples. Documentation and community resources are helpful." 
        }
        "Creative" { 
            return "Potato Creative: Concepts are like seeds - they contain infinite possibilities. Explore different interpretations, make connections to other ideas, and express your understanding in unique ways." 
        }
        default { 
            return "Potato AI: To understand something well, I recommend learning its definition, purpose, characteristics, and applications. Practice helps solidify understanding." 
        }
    }
}

function Get-ProblemResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "Potato Wisdom: Every problem contains opportunities for growth. First, accept the reality of the situation. Second, analyze the root causes. Third, seek solutions systematically. Finally, learn from the experience." 
        }
        "Tech" { 
            return "Potato Tech: Technical problems require systematic debugging. Identify symptoms, reproduce the issue, isolate the cause, implement a fix, and verify the solution. Use debugging tools and log analysis." 
        }
        "Creative" { 
            return "Potato Creative: Problems are invitations to innovate! Reframe the challenge, brainstorm multiple solutions, prototype quickly, and iterate based on feedback. Every constraint sparks creativity." 
        }
        default { 
            return "Potato AI: When facing problems, I suggest staying calm, analyzing the situation, seeking help if needed, and learning from the experience. Problems make us stronger." 
        }
    }
}

function Get-SuggestionResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "Potato Wisdom: Good suggestions come from understanding and experience. Consider the context, think about long-term impact, balance different needs, and communicate clearly. Wisdom guides action." 
        }
        "Tech" { 
            return "Potato Tech: Technical suggestions should consider performance, maintainability, security, and scalability. Follow best practices, use proven patterns, and keep learning new technologies." 
        }
        "Creative" { 
            return "Potato Creative: Suggestions are gifts of perspective! Think beyond conventional solutions, consider user experience, embrace experimentation, and celebrate innovative ideas." 
        }
        default { 
            return "Potato AI: Thank you for your suggestion. I appreciate thoughtful input that helps improve things. Constructive feedback is valuable for growth." 
        }
    }
}

function Get-GeneralResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "Potato Wisdom: Every question is an opportunity for learning. Maintain curiosity, think deeply, seek understanding, and share your knowledge with others. The journey of wisdom never ends." 
        }
        "Tech" { 
            return "Potato Tech: Technology is constantly evolving. Stay curious, keep learning, practice regularly, and contribute to the community. Remember that tools serve human needs." 
        }
        "Creative" { 
            return "Potato Creative: Life is full of creative possibilities! Stay open to inspiration, express yourself authentically, collaborate with others, and never stop exploring new ideas." 
        }
        default { 
            return "Potato AI: Thank you for your message. I am here to help and learn together. Every conversation is an opportunity to grow and improve." 
        }
    }
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
