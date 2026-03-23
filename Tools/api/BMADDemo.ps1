# MingGoRTS BMAD系統演示腳本
# PowerShell Script

param(
    [string]$DemoType = "Full",
    [switch]$Interactive = $false,
    [switch]$Verbose = $false
)

# Configuration
$ProjectRoot = "C:\HW\MingGoRTS"
$LogPath = "$ProjectRoot\Logs\BMADDemo_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"

# Initialize logging
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    Write-Host $LogEntry -ForegroundColor $(switch($Level) {"ERROR" {"Red"}; "WARNING" {"Yellow"}; "SUCCESS" {"Green"}; default {"White"}})
    Add-Content -Path $LogPath -Value $LogEntry
}

# BMAD系統演示
function Invoke-BMADDemo {
    Write-Log "========================================" "INFO"
    Write-Log "MingGoRTS BMAD系統演示" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log "演示類型: $DemoType" "INFO"
    Write-Log "" "INFO"
    
    switch ($DemoType) {
        "Basic" {
            Show-BasicBMAD
        }
        "Discussion" {
            Show-DiscussionSystem
        }
        "Learning" {
            Show-LearningSystem
        }
        "Analytics" {
            Show-AnalyticsSystem
        }
        "Full" {
            Show-BasicBMAD
            Show-DiscussionSystem
            Show-LearningSystem
            Show-AnalyticsSystem
        }
        default {
            Write-Log "無效的演示類型: $DemoType" "ERROR"
            exit 1
        }
    }
    
    Write-Log "" "INFO"
    Write-Log "BMAD系統演示完成!" "SUCCESS"
}

# 基礎BMAD演示
function Show-BasicBMAD {
    Write-Log "=== 基礎BMAD系統演示 ===" "INFO"
    
    # 模擬創建AI分身
    Write-Log "創建AI分身..." "INFO"
    $Agents = @(
        @{ID="ARCH_001"; Name="架構師AI"; Role="Architect"; Personality="戰略性思考者"},
        @{ID="DEV_001"; Name="開發者AI"; Role="Developer"; Personality="實用問題解決者"},
        @{ID="TEST_001"; Name="測試者AI"; Role="Tester"; Personality="質量導向分析師"},
        @{ID="SEC_001"; Name="安全專家AI"; Role="Security"; Personality="風險評估專家"}
    )
    
    foreach ($Agent in $Agents) {
        Write-Log "  創建分身: $($Agent.Name) ($($Agent.Role))" "SUCCESS"
        Write-Log "    個性: $($Agent.Personality)" "INFO"
    }
    
    # 模擬初始化
    Write-Log "初始化BMAD系統..." "INFO"
    Write-Log "  設置討論協議" "SUCCESS"
    Write-Log "  配置學習算法" "SUCCESS"
    Write-Log "  建立分身個性檔案" "SUCCESS"
    
    Write-Log "基礎BMAD系統演示完成" "SUCCESS"
}

# 討論系統演示
function Show-DiscussionSystem {
    Write-Log "=== 討論系統演示 ===" "INFO"
    
    # 創建討論會話
    $SessionID = "BMAD_SESSION_" + (Get-Date -Format "yyyyMMddHHmmss")
    $Topic = "如何優化RTS遊戲的AI決策系統"
    $Context = "MingGoRTS專案 - 大規模單位AI優化"
    
    Write-Log "啟動討論會話..." "INFO"
    Write-Log "  會話ID: $SessionID" "SUCCESS"
    Write-Log "  討論主題: $Topic" "INFO"
    Write-Log "  專案上下文: $Context" "INFO"
    
    # 模擬討論過程
    $Discussion = @(
        @{Sender="ARCH_001"; Message="從架構角度，我建議採用分層決策系統，將戰術和戰略分離"; Emotion="Confident"},
        @{Sender="DEV_001"; Message="我認為可以使用行為樹配合狀態機，這樣能提供更好的性能和靈活性"; Emotion="Collaborative"},
        @{Sender="TEST_001"; Message="需要確保AI決策的可測試性，建議建立完整的測試框架"; Emotion="Supportive"},
        @{Sender="SEC_001"; Message="要注意AI作弊問題，確保決策邏輯不能被玩家利用"; Emotion="Concerned"},
        @{Sender="ARCH_001"; Message="很好的觀點，我們需要在設計階段就考慮安全因素"; Emotion="Innovative"},
        @{Sender="DEV_001"; Message="我可以實現一個決策驗證層，確保所有AI行為都符合規則"; Emotion="Excited"}
    )
    
    Write-Log "開始討論..." "INFO"
    foreach ($Msg in $Discussion) {
        Write-Log "  [$($Msg.Sender)] $($Msg.Message)" "INFO"
        Write-Log "    情緒狀態: $($Msg.Emotion)" "INFO"
        
        if ($Interactive) {
            Write-Host "按Enter繼續..." -ForegroundColor Yellow
            Read-Host
        }
    }
    
    # 生成討論總結
    Write-Log "生成討論總結..." "INFO"
    $Summary = @"
討論總結:
- 達成共識: 採用分層決策架構
- 技術選擇: 行為樹 + 状態機
- 關鍵考量: 性能、安全性、可測試性
- 下一步: 設計決策驗證框架
"@
    
    Write-Log $Summary "SUCCESS"
    
    # 提取洞見
    Write-Log "提取關鍵洞見..." "INFO"
    $Insights = @(
        "分層架構能夠提高AI決策的模組化程度",
        "安全性需要在設計階段就考慮",
        "測試框架對AI系統至關重要",
        "性能和靈活性需要平衡"
    )
    
    foreach ($Insight in $Insights) {
        Write-Log "  洞見: $Insight" "SUCCESS"
    }
    
    Write-Log "討論系統演示完成" "SUCCESS"
}

# 學習系統演示
function Show-LearningSystem {
    Write-Log "=== 學習系統演示 ===" "INFO"
    
    Write-Log "啟動學習模式..." "INFO"
    
    # 模擬學習過程
    $LearningData = @(
        @{Pattern="分層架構"; Outcome="高模組化"; Confidence=0.9},
        @{Pattern="行為樹"; Outcome="良好性能"; Confidence=0.85},
        @{Pattern="安全考慮"; Outcome="減少漏洞"; Confidence=0.95},
        @{Pattern="測試框架"; Outcome="提高質量"; Confidence=0.88}
    )
    
    Write-Log "分析歷史討論數據..." "INFO"
    foreach ($Data in $LearningData) {
        Write-Log "  學習模式: $($Data.Pattern) → $($Data.Outcome) (信心度: $($Data.Confidence))" "SUCCESS"
    }
    
    # 更新AI分身知識
    Write-Log "更新AI分身知識庫..." "INFO"
    $KnowledgeUpdates = @(
        "架構師AI: 增強了對分層設計的理解",
        "開發者AI: 提升了行為樹優化技能",
        "安全專家AI: 加深了安全設計模式認知",
        "測試者AI: 完善了AI測試策略"
    )
    
    foreach ($Update in $KnowledgeUpdates) {
        Write-Log "  $Update" "SUCCESS"
    }
    
    # 適應性優化
    Write-Log "執行適應性優化..." "INFO"
    $Optimizations = @(
        "調整架構師AI的創造力參數: 0.8 → 0.85",
        "優化開發者AI的實用性評分: 0.85 → 0.9",
        "提升安全專家AI的風險識別能力: 0.9 → 0.93",
        "增強測試者AI的質量標準: 0.88 → 0.91"
    )
    
    foreach ($Opt in $Optimizations) {
        Write-Log "  $Opt" "SUCCESS"
    }
    
    Write-Log "學習系統演示完成" "SUCCESS"
}

# 分析系統演示
function Show-AnalyticsSystem {
    Write-Log "=== 分析系統演示 ===" "INFO"
    
    # 討論質量分析
    Write-Log "分析討論質量..." "INFO"
    $QualityMetrics = @{
        "參與度" = "95%";
        "多樣性" = "88%";
        "建設性" = "92%";
        "創新性" = "85%";
        "協作性" = "90%"
    }
    
    Write-Log "討論質量指標:" "SUCCESS"
    foreach ($Metric in $QualityMetrics.GetEnumerator()) {
        Write-Log "  $($Metric.Key): $($Metric.Value)" "INFO"
    }
    
    # 參與者貢獻分析
    Write-Log "分析參與者貢獻..." "INFO"
    $Contributions = @{
        "ARCH_001" = "28% (架構設計)";
        "DEV_001" = "25% (技術實現)";
        "TEST_001" = "23% (質量保證)";
        "SEC_001" = "24% (安全考量)"
    }
    
    Write-Log "參與者貢獻度:" "SUCCESS"
    foreach ($Contrib in $Contributions.GetEnumerator()) {
        Write-Log "  $($Contrib.Key): $($Contrib.Value)" "INFO"
    }
    
    # 洞見重要性評分
    Write-Log "評估洞見重要性..." "INFO"
    $InsightScores = @(
        @{Content="分層架構的模組化優勢"; Importance=0.92; Novelty=0.78};
        @{Content="安全設計的早期考慮"; Importance=0.89; Novelty=0.85};
        @{Content="AI測試的必要性"; Importance=0.87; Novelty=0.72};
        @{Content="性能與靈活性平衡"; Importance=0.85; Novelty=0.80}
    )
    
    Write-Log "洞見評分:" "SUCCESS"
    foreach ($Score in $InsightScores) {
        Write-Log "  $($Score.Content)" "INFO"
        Write-Log "    重要性: $($Score.Importance), 新穎性: $($Score.Novelty)" "INFO"
    }
    
    # 生成分析報告
    Write-Log "生成分析報告..." "INFO"
    $Report = @"
BMAD分析報告
============
討論質量: 優秀 (91%)
參與平衡: 良好 (各角色貢獻均勻)
創新水平: 高 (多個新穎觀點)
協作效果: 优秀 (良好的互動和建設性討論)

建議:
1. 繼續保持多樣化的參與者配置
2. 加強創新性思考的鼓勵
3. 優化決策流程以提高效率
4. 建立知識庫以保存學習成果
"@
    
    Write-Log $Report "SUCCESS"
    
    Write-Log "分析系統演示完成" "SUCCESS"
}

# 生成演示報告
function New-AnalyticsReport {
    Write-Log "生成演示報告..." "INFO"
    
    $ReportPath = "$ProjectRoot\Reports\BMADDemoReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').html"
    
    $ReportContent = @"
<!DOCTYPE html>
<html>
<head>
    <title>MingGoRTS BMAD系統演示報告</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .header { background-color: #2c3e50; color: white; padding: 20px; border-radius: 5px; }
        .section { margin: 20px 0; padding: 15px; border: 1px solid #ddd; border-radius: 5px; }
        .success { background-color: #d4edda; }
        .info { background-color: #e7f3ff; }
        .warning { background-color: #fff3cd; }
        .metric { display: inline-block; margin: 10px; padding: 10px; background-color: #f8f9fa; border-radius: 3px; }
    </style>
</head>
<body>
    <div class="header">
        <h1>MingGoRTS BMAD系統演示報告</h1>
        <p>演示時間: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')</p>
        <p>演示類型: $DemoType</p>
    </div>
    
    <div class="section success">
        <h2>系統狀態</h2>
        <div class="metric">✅ BMAD核心系統: 運行正常</div>
        <div class="metric">✅ AI分身管理: 功能完整</div>
        <div class="metric">✅ 討論系統: 交互流暢</div>
        <div class="metric">✅ 學習引擎: 適應良好</div>
        <div class="metric">✅ 分析模組: 準確可靠</div>
    </div>
    
    <div class="section info">
        <h2>核心功能演示</h2>
        <h3>多AI分身系統</h3>
        <ul>
            <li>成功創建4個專業AI分身</li>
            <li>每個分身具備獨特個性和專業領域</li>
            <li>智能互動和對話生成</li>
        </ul>
        
        <h3>智能討論機制</h3>
        <ul>
            <li>支持多種討論類型和情緒狀態</li>
            <li>上下文感知的對話生成</li>
            <li>自動洞見提取和決策生成</li>
        </ul>
        
        <h3>學習與適應</h3>
        <ul>
            <li>從討論中持續學習</li>
            <li>適應性優化分身參數</li>
            <li>知識庫更新和維護</li>
        </ul>
    </div>
    
    <div class="section warning">
        <h2>性能指標</h2>
        <div class="metric">討論質量: 91%</div>
        <div class="metric">參與度: 95%</div>
        <div class="metric">創新性: 85%</div>
        <div class="metric">協作效果: 90%</div>
        <div class="metric">學習效率: 88%</div>
    </div>
    
    <div class="section info">
        <h2>未來改進建議</h2>
        <ul>
            <li>增加更多專業角色分身</li>
            <li>提升自然語言理解能力</li>
            <li>加強視覺化界面</li>
            <li>擴展多語言支持</li>
            <li>集成更多外部知識源</li>
        </ul>
    </div>
</body>
</html>
"@
    
    # 確保報告目錄存在
    $ReportDir = Split-Path -Parent $ReportPath
    if (-not (Test-Path $ReportDir)) {
        New-Item -ItemType Directory -Path $ReportDir -Force | Out-Null
    }
    
    Set-Content -Path $ReportPath -Value $ReportContent -Encoding UTF8
    Write-Log "演示報告已生成: $ReportPath" "SUCCESS"
}

# 主執行函數
function Main {
    Write-Log "========================================" "INFO"
    Write-Log "MingGoRTS BMAD系統演示開始" "INFO"
    Write-Log "========================================" "INFO"
    Write-Log "參數:" "INFO"
    Write-Log "  演示類型: $DemoType" "INFO"
    Write-Log "  互動模式: $Interactive" "INFO"
    Write-Log "  詳細輸出: $Verbose" "INFO"
    Write-Log "" "INFO"
    
    try {
        Invoke-BMADDemo
        New-AnalyticsReport
        
        Write-Log "" "INFO"
        Write-Log "演示成功完成!" "SUCCESS"
        Write-Log "日誌文件: $LogPath" "INFO"
        
    } catch {
        Write-Log "演示過程中發生錯誤: $($_.Exception.Message)" "ERROR"
        exit 1
    }
}

# 執行主函數
Main
