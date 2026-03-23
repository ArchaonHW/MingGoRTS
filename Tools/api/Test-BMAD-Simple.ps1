# Test Potato AI BMAD Dialogue - Simple

Write-Host "Testing Potato AI BMAD Dialogue..." -ForegroundColor Yellow

function Get-PotatoResponse {
    param([string]$Message)
    
    if ($Message -match "how to") {
        return "Potato Wisdom: I recommend a systematic approach. First understand the problem, then plan your strategy, practice and verify, and continuously improve."
    } elseif ($Message -match "what is") {
        return "Potato Wisdom: Understanding requires going beyond the surface. Consider multiple angles and connect with practical applications."
    } elseif ($Message -match "problem") {
        return "Potato Wisdom: Every problem contains growth opportunities. Accept reality, analyze causes, seek solutions, and learn from experience."
    } elseif ($Message -match "suggestion") {
        return "Potato Wisdom: Good suggestions come from understanding. Consider context, long-term impact, and communicate clearly."
    } else {
        return "Potato AI: Thank you for your message. I am here to help with technical questions, creative ideas, problem solving, and general advice."
    }
}

# Test the function
$TestMessage = "how to optimize my code performance"
Write-Host "Test Message: $TestMessage" -ForegroundColor Cyan

$Response = Get-PotatoResponse -Message $TestMessage
Write-Host "Response: $Response" -ForegroundColor Green

Write-Host "Test completed successfully!" -ForegroundColor Yellow
