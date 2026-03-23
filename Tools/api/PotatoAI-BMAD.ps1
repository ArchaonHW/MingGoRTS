# 馬鈴薯智慧AI - BMAD對話系統
# Potato AI BMAD Dialogue System

param(
    [string]$Action = "bmad",
    [string]$Message = ""
)

Write-Host "========================================" -ForegroundColor Yellow
Write-Host "🥔 馬鈴薯智慧AI - BMAD對話系統" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Yellow

switch ($Action.ToLower()) {
    "bmad" {
        if ([string]::IsNullOrEmpty($Message)) {
            Write-Host "🥔 請提供對話訊息" -ForegroundColor Yellow
            Write-Host "用法: .\PotatoAI-BMAD.ps1 bmad '您的訊息'" -ForegroundColor Cyan
            exit 0
        }
        
        $Response = Invoke-PotatoBMADDialogue -UserMessage $Message -Context "AI_Helper" -Persona "Wisdom"
        Write-Host "`n🥔 對話完成！" -ForegroundColor Green
        break
    }
    "chat" {
        Start-PotatoBMADChat -Persona "Wisdom" -Context "Interactive"
        break
    }
    "help" {
        Write-Host "🥔 馬鈴薯BMAD對話系統使用說明:" -ForegroundColor White
        Write-Host "  bmad [message] - 單次BMAD對話" -ForegroundColor Green
        Write-Host "  chat          - 交互式對話模式" -ForegroundColor Green
        Write-Host "  help          - 顯示此幫助信息" -ForegroundColor Green
        Write-Host "`n🥔 人設類型:" -ForegroundColor Cyan
        Write-Host "  Wisdom    - 智慧助手" -ForegroundColor White
        Write-Host "  Tech      - 技術專家" -ForegroundColor White
        Write-Host "  Creative  - 創意夥伴" -ForegroundColor White
        break
    }
    default {
        Write-Host "❌ 未知操作: $Action" -ForegroundColor Red
        Write-Host "🥔 使用 'help' 查看可用操作" -ForegroundColor Yellow
        exit 1
    }
}

# BMAD智慧對話功能
function Invoke-PotatoBMADDialogue {
    param(
        [string]$UserMessage,
        [string]$Context = "AI_Helper",
        [string]$Persona = "Wisdom"
    )
    
    Write-Host "🥔 馬鈴薯BMAD對話系統啟動..." -ForegroundColor Yellow
    Write-Host "🥔 用戶訊息: $UserMessage" -ForegroundColor Cyan
    Write-Host "🥔 上下文: $Context" -ForegroundColor Cyan
    Write-Host "🥔 人設: $Persona" -ForegroundColor Cyan
    
    # 生成回應
    $Response = Get-PotatoBMADResponse -UserMessage $UserMessage -Persona $Persona
    
    Write-Host "🥔 BMAD回應生成完成" -ForegroundColor Green
    Write-Host "🥔 回應內容:" -ForegroundColor White
    Write-Host $Response -ForegroundColor Gray
    
    return $Response
}

function Get-PotatoBMADResponse {
    param([string]$UserMessage, [string]$Persona)
    
    # 分析用戶意圖
    if ($UserMessage -match "如何|怎麼|how to") {
        return Get-PotatoHowToResponse -Persona $Persona
    } elseif ($UserMessage -match "什麼|是什麼|what is") {
        return Get-PotatoWhatIsResponse -Persona $Persona
    } elseif ($UserMessage -match "問題|錯誤|bug|problem") {
        return Get-PotatoProblemResponse -Persona $Persona
    } elseif ($UserMessage -match "建議|改進|suggestion") {
        return Get-PotatoSuggestionResponse -Persona $Persona
    } else {
        return Get-PotatoGeneralResponse -Persona $Persona
    }
}

function Get-PotatoHowToResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 馬鈴薯智慧: 對於您的問題，我建議採用系統性方法。首先深入理解問題核心，其次制定策略規劃，第三實踐驗證效果，最後持續改進。記住，真正的智慧來自於不斷的學習和實踐。" 
        }
        "Tech" { 
            return "🥔 馬鈴薯技術: 對於技術問題，我建議分析需求、選擇合適工具、逐步實施、徹底測試、記錄過程。技術是工具，思維是核心。" 
        }
        "Creative" { 
            return "🥔 馬鈴薯創意: 讓我們跳出框架思考！想像沒有限制，從不同領域尋找靈感，用新方式組合現有想法。創意來自於打破常規。" 
        }
        default { 
            return "🥔 馬鈴薯AI: 感謝您的提問。我建議將問題分解為小步驟，尋求相關資源，採取行動，從結果中學習。" 
        }
    }
}

function Get-PotatoWhatIsResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 馬鈴薯智慧: 理解概念需要超越表面。考慮多個角度，識別核心原則，與實際應用聯繫。真正的理解來自於深度思考。" 
        }
        "Tech" { 
            return "🥔 馬鈴薯技術: 技術概念有多層次。從基本定義開始，理解目的，學習實現細節，通過實例練習。文檔和社區資源很有幫助。" 
        }
        "Creative" { 
            return "🥔 馬鈴薯創意: 概念就像種子，包含無限可能。探索不同解釋，與其他想法建立聯繫，用獨特方式表達理解。" 
        }
        default { 
            return "🥔 馬鈴薯AI: 要深入理解某事物，我建議學習其定義、目的、特徵和應用。實踐有助於鞏固理解。" 
        }
    }
}

function Get-PotatoProblemResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 馬鈴薯智慧: 每個問題都包含成長機會。首先接受現實，其次分析根本原因，第三系統尋求解決方案，最後從經驗中學習。" 
        }
        "Tech" { 
            return "🥔 馬鈴薯技術: 技術問題需要系統調試。識別症狀、重現問題、隔離原因、實施修復、驗證解決方案。使用調試工具和日誌分析。" 
        }
        "Creative" { 
            return "🥔 馬鈴薯創意: 問題是創新的邀請！重新定義挑戰，腦力激盪多種解決方案，快速原型，基於反饋迭代。每個限制都激發創意。" 
        }
        default { 
            return "🥔 馬鈴薯AI: 面對問題時，我建議保持冷靜，分析情況，需要時尋求幫助，從經驗中學習。問題讓我們更強大。" 
        }
    }
}

function Get-PotatoSuggestionResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 馬鈴薯智慧: 好的建議來自理解和經驗。考慮上下文，思考長期影響，平衡不同需求，清晰溝通。智慧指導行動。" 
        }
        "Tech" { 
            return "🥔 馬鈴薯技術: 技術建議應考慮性能、可維護性、安全性和可擴展性。遵循最佳實踐，使用經過驗證的模式，持續學習新技術。" 
        }
        "Creative" { 
            return "🥔 馬鈴薯創意: 建議是視角的禮物！跳出慣常解決方案，考慮用戶體驗，擁抱實驗，慶祝創新想法。" 
        }
        default { 
            return "🥔 馬鈴薯AI: 感謝您的建議。我欣賞有助於改善的深思熟慮輸入。建設性反饋對成長很有價值。" 
        }
    }
}

function Get-PotatoGeneralResponse {
    param([string]$Persona)
    
    switch ($Persona) {
        "Wisdom" { 
            return "🥔 馬鈴薯智慧: 每個問題都是學習機會。保持好奇心，深入思考，尋求理解，與他人分享知識。智慧之旅永無止境。" 
        }
        "Tech" { 
            return "🥔 馬鈴薯技術: 技術不斷發展。保持好奇，持續學習，定期練習，為社區做出貢獻。記住工具服務於人類需求。" 
        }
        "Creative" { 
            return "🥔 馬鈴薯創意: 生活充滿創意可能！保持對靈感開放，真實表達自己，與他人合作，永不停止探索新想法。" 
        }
        default { 
            return "🥔 馬鈴薯AI: 感謝您的訊息。我在這裡幫助和一起學習。每次對話都是成長和改進的機會。" 
        }
    }
}

function Start-PotatoBMADChat {
    param(
        [string]$Persona = "Wisdom",
        [string]$Context = "Interactive"
    )
    
    Write-Host "🥔 開始馬鈴薯BMAD對話..." -ForegroundColor Yellow
    Write-Host "🥔 人設: $Persona" -ForegroundColor Cyan
    Write-Host "🥔 上下文: $Context" -ForegroundColor Cyan
    Write-Host "🥔 輸入 'exit' 結束對話" -ForegroundColor Gray
    Write-Host "========================================" -ForegroundColor Yellow
    
    while ($true) {
        Write-Host "`n您: " -ForegroundColor White -NoNewline
        $UserInput = Read-Host
        
        if ($UserInput -eq "exit") {
            Write-Host "🥔 對話結束，期待下次交流！" -ForegroundColor Green
            break
        }
        
        if ([string]::IsNullOrWhiteSpace($UserInput)) {
            continue
        }
        
        $Response = Invoke-PotatoBMADDialogue -UserMessage $UserInput -Context $Context -Persona $Persona
        Write-Host "`n🥔 馬鈴薯AI: $Response" -ForegroundColor Gray
    }
}

Write-Host "`n🥔 馬鈴薯智慧AI任務完成！" -ForegroundColor Green
