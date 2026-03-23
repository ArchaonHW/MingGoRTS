# Potato AI BMAD Dialogue System - Simple Version

param(
    [string]$Action = "bmad",
    [string]$Message = ""
)

Write-Host "========================================" -ForegroundColor Yellow
Write-Host "Potato AI - BMAD Dialogue System" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Yellow

if ($Action -eq "bmad") {
    if ([string]::IsNullOrEmpty($Message)) {
        Write-Host "Please provide a message" -ForegroundColor Yellow
        Write-Host "Usage: .\PotatoAI-BMAD-SimpleV2.ps1 bmad 'your message'" -ForegroundColor Cyan
        exit 0
    }
    
    Write-Host "Potato BMAD Dialogue System Starting..." -ForegroundColor Yellow
    Write-Host "User Message: $Message" -ForegroundColor Cyan
    
    $Response = Get-PotatoResponse -Message $Message
    
    Write-Host "Response Generated:" -ForegroundColor Green
    Write-Host $Response -ForegroundColor White
    
} elseif ($Action -eq "help") {
    Write-Host "Potato BMAD Dialogue System Usage:" -ForegroundColor White
    Write-Host "bmad [message] - Single BMAD dialogue" -ForegroundColor Green
    Write-Host "help - Show this help" -ForegroundColor Green
    
} else {
    Write-Host "Unknown action: $Action" -ForegroundColor Red
    Write-Host "Use 'help' to see available actions" -ForegroundColor Yellow
}

function Get-PotatoResponse {
    param([string]$Message)
    
    if ($Message -match "how to|如何|怎麼") {
        return "Potato Wisdom: For your question, I recommend a systematic approach. First, deeply understand the problem core. Second, formulate strategic planning. Third, practice and verify effects. Finally, continuously improve. Remember, true wisdom comes from continuous learning and practice."
    } elseif ($Message -match "what is|什麼|定義") {
        return "Potato Wisdom: Understanding concepts requires going beyond the surface. Consider multiple angles, identify core principles, and connect with practical applications. True understanding comes from deep thinking."
    } elseif ($Message -match "problem|問題|錯誤|bug") {
        return "Potato Wisdom: Every problem contains opportunities for growth. First, accept reality. Second, analyze root causes. Third, systematically seek solutions. Finally, learn from experience."
    } elseif ($Message -match "suggestion|建議|改進") {
        return "Potato Wisdom: Good suggestions come from understanding and experience. Consider context, think about long-term impact, balance different needs, and communicate clearly. Wisdom guides action."
    } else {
        return "Potato AI: Thank you for your message. I am here to help and learn together. Every conversation is an opportunity to grow and improve. I can help with technical questions, creative ideas, problem solving, and general advice."
    }
}

Write-Host "Potato AI Task Completed!" -ForegroundColor Green
