# Potato AI BMAD Dialogue System - Basic

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
        Write-Host "Usage: .\PotatoAI-BMAD-BasicV2.ps1 bmad 'your message'" -ForegroundColor Cyan
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
        return "Potato Wisdom: I recommend a systematic approach. First understand the problem, then plan your strategy, practice and verify, and continuously improve."
    } elseif ($Message -match "what is|什麼|定義") {
        return "Potato Wisdom: Understanding requires going beyond the surface. Consider multiple angles and connect with practical applications."
    } elseif ($Message -match "problem|問題|錯誤|bug") {
        return "Potato Wisdom: Every problem contains growth opportunities. Accept reality, analyze causes, seek solutions, and learn from experience."
    } elseif ($Message -match "suggestion|建議|改進") {
        return "Potato Wisdom: Good suggestions come from understanding. Consider context, long-term impact, and communicate clearly."
    } else {
        return "Potato AI: Thank you for your message. I am here to help with technical questions, creative ideas, problem solving, and general advice."
    }
}

Write-Host "Potato AI Task Completed!" -ForegroundColor Green
