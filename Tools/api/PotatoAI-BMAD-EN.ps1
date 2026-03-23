# Potato AI BMAD Dialogue System

param(
    [string]$Action = "bmad",
    [string]$Message = ""
)

Write-Host "========================================" -ForegroundColor Yellow
Write-Host "🥔 Potato AI - BMAD Dialogue System" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Yellow

switch ($Action.ToLower()) {
    "bmad" {
        if ([string]::IsNullOrEmpty($Message)) {
            Write-Host "🥔 Please provide a message" -ForegroundColor Yellow
            Write-Host "Usage: .\PotatoAI-BMAD.ps1 bmad 'your message'" -ForegroundColor Cyan
            exit 0
        }
        
        $Response = Invoke-PotatoBMADDialogue -UserMessage $Message -Context "AI_Helper" -Persona "Wisdom"
        Write-Host "`n🥔 Dialogue completed!" -ForegroundColor Green
        break
    }
    "chat" {
        Start-PotatoBMADChat -Persona "Wisdom" -Context "Interactive"
        break
    }
    "help" {
        Write-Host "🥔 Potato BMAD Dialogue System Usage:" -ForegroundColor White
        Write-Host "  bmad [message] - Single BMAD dialogue" -ForegroundColor Green
        Write-Host "  chat          - Interactive chat mode" -ForegroundColor Green
        Write-Host "  help          - Show this help" -ForegroundColor Green
        Write-Host "`n🥔 Persona Types:" -ForegroundColor Cyan
        Write-Host "  Wisdom    - Wisdom Assistant" -ForegroundColor White
        Write-Host "  Tech      - Technical Expert" -ForegroundColor White
        Write-Host "  Creative  - Creative Partner" -ForegroundColor White
        break
    }
    default {
        Write-Host "❌ Unknown action: $Action" -ForegroundColor Red
        Write-Host "🥔 Use 'help' to see available actions" -ForegroundColor Yellow
        exit 1
    }
}

function Invoke-PotatoBMADDialogue {
    param(
        [string]$UserMessage,
        [string]$Context = "AI_Helper",
        [string]$Persona = "Wisdom"
    )
    
    Write-Host "🥔 Potato BMAD Dialogue System Starting..." -ForegroundColor Yellow
    Write-Host "🥔 User Message: $UserMessage" -ForegroundColor Cyan
    Write-Host "🥔 Context: $Context" -ForegroundColor Cyan
    Write-Host "🥔 Persona: $Persona" -ForegroundColor Cyan
    
    $Response = Get-PotatoBMADResponse -UserMessage $UserMessage -Persona $Persona
    
    Write-Host "🥔 BMAD Response Generated" -ForegroundColor Green
    Write-Host "🥔 Response Content:" -ForegroundColor White
    Write-Host $Response -ForegroundColor Gray
    
    return $Response
}

function Get-PotatoBMADResponse {
    param([string]$UserMessage, [string]$Persona)
    
    if ($UserMessage -match "how to|如何|怎麼") {
        return Get-PotatoHowToResponse -Persona $Persona
    } elseif ($UserMessage -match "what is|什麼|定義") {
        return Get-PotatoWhatIsResponse -Persona $Persona
    } elseif ($UserMessage -match "problem|問題|錯誤|bug") {
        return Get-PotatoProblemResponse -Persona $Persona
    } elseif ($UserMessage -match "suggestion|建議|改進") {
        return Get-PotatoSuggestionResponse -Persona $Persona
    } else {
        return Get-PotatoGeneralResponse -Persona $Persona
    }
}

function Get-PotatoHowToResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 Potato Wisdom: For your question, I recommend a systematic approach. First, deeply understand the problem core. Second, formulate strategic planning. Third, practice and verify effects. Finally, continuously improve. Remember, true wisdom comes from continuous learning and practice." 
        }
        "Tech" { 
            return "🥔 Potato Tech: For technical questions, I suggest analyzing requirements, choosing appropriate tools, implementing step by step, testing thoroughly, and documenting the process. Technology is a tool, thinking is the core." 
        }
        "Creative" { 
            return "🥔 Potato Creative: Let us think outside the box! Imagine no limitations, seek inspiration from different fields, combine existing ideas in new ways. Creativity comes from breaking conventions." 
        }
        default { 
            return "🥔 Potato AI: Thank you for your question. I suggest breaking the problem into small steps, seeking relevant resources, taking action, and learning from results." 
        }
    }
}

function Get-PotatoWhatIsResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 Potato Wisdom: Understanding concepts requires going beyond the surface. Consider multiple angles, identify core principles, and connect with practical applications. True understanding comes from deep thinking." 
        }
        "Tech" { 
            return "🥔 Potato Tech: Technical concepts have multiple layers. Start with basic definitions, understand the purpose, learn implementation details, and practice with examples. Documentation and community resources are helpful." 
        }
        "Creative" { 
            return "🥔 Potato Creative: Concepts are like seeds, containing infinite possibilities. Explore different interpretations, establish connections with other ideas, and express understanding in unique ways." 
        }
        default { 
            return "🥔 Potato AI: To deeply understand something, I suggest learning its definition, purpose, characteristics, and applications. Practice helps consolidate understanding." 
        }
    }
}

function Get-PotatoProblemResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 Potato Wisdom: Every problem contains opportunities for growth. First, accept reality. Second, analyze root causes. Third, systematically seek solutions. Finally, learn from experience." 
        }
        "Tech" { 
            return "🥔 Potato Tech: Technical problems require systematic debugging. Identify symptoms, reproduce the issue, isolate causes, implement fixes, and verify solutions. Use debugging tools and log analysis." 
        }
        "Creative" { 
            return "🥔 Potato Creative: Problems are invitations to innovate! Redefine challenges, brainstorm multiple solutions, rapid prototyping, and iterate based on feedback. Every constraint sparks creativity." 
        }
        default { 
            return "🥔 Potato AI: When facing problems, I suggest staying calm, analyzing the situation, seeking help when needed, and learning from experience. Problems make us stronger." 
        }
    }
}

function Get-PotatoSuggestionResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 Potato Wisdom: Good suggestions come from understanding and experience. Consider context, think about long-term impact, balance different needs, and communicate clearly. Wisdom guides action." 
        }
        "Tech" { 
            return "🥔 Potato Tech: Technical suggestions should consider performance, maintainability, security, and scalability. Follow best practices, use proven patterns, and continuously learn new technologies." 
        }
        "Creative" { 
            return "🥔 Potato Creative: Suggestions are gifts of perspective! Break out of conventional solutions, consider user experience, embrace experimentation, and celebrate innovative ideas." 
        }
        default { 
            return "🥔 Potato AI: Thank you for your suggestion. I appreciate thoughtful input that helps improve things. Constructive feedback is valuable for growth." 
        }
    }
}

function Get-PotatoGeneralResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 Potato Wisdom: Every question is an opportunity to learn. Maintain curiosity, think deeply, seek understanding, and share knowledge with others. The journey of wisdom never ends." 
        }
        "Tech" { 
            return "🥔 Potato Tech: Technology is constantly evolving. Stay curious, keep learning, practice regularly, and contribute to the community. Remember that tools serve human needs." 
        }
        "Creative" { 
            return "🥔 Potato Creative: Life is full of creative possibilities! Stay open to inspiration, express yourself authentically, collaborate with others, and never stop exploring new ideas." 
        }
        default { 
            return "🥔 Potato AI: Thank you for your message. I am here to help and learn together. Every conversation is an opportunity to grow and improve." 
        }
    }
}

function Start-PotatoBMADChat {
    param(
        [string]$Persona = "Wisdom",
        [string]$Context = "Interactive"
    )
    
    Write-Host "🥔 Starting Potato BMAD Chat..." -ForegroundColor Yellow
    Write-Host "🥔 Persona: $Persona" -ForegroundColor Cyan
    Write-Host "🥔 Context: $Context" -ForegroundColor Cyan
    Write-Host "🥔 Type 'exit' to end conversation" -ForegroundColor Gray
    Write-Host "========================================" -ForegroundColor Yellow
    
    while ($true) {
        Write-Host "`nYou: " -ForegroundColor White -NoNewline
        $UserInput = Read-Host
        
        if ($UserInput -eq "exit") {
            Write-Host "🥔 Chat ended, looking forward to next conversation!" -ForegroundColor Green
            break
        }
        
        if ([string]::IsNullOrWhiteSpace($UserInput)) {
            continue
        }
        
        $Response = Invoke-PotatoBMADDialogue -UserMessage $UserInput -Context $Context -Persona $Persona
        Write-Host "`n🥔 Potato AI: $Response" -ForegroundColor Gray
    }
}

Write-Host "`n🥔 Potato AI Task Completed!" -ForegroundColor Green
