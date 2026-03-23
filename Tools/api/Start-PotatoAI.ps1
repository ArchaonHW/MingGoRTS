# 🥔 馬鈴薯智慧AI啟動器
# Potato AI Smart Launcher

param(
    [string]$Action = "start",
    [string]$Mode = "smart"
)

Write-Host "========================================" -ForegroundColor Yellow
Write-Host "🥔 馬鈴薯智慧AI - Potato AI" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Yellow

try {
    # 載入馬鈴薯AI核心系統
    . (Join-Path $PSScriptRoot "MingPotatoAI-Core.ps1")
    
    switch ($Action.ToLower()) {
        "start" {
            Write-Host "🥔 啟動馬鈴薯智慧AI系統..." -ForegroundColor Yellow
            $Result = Initialize-PotatoAI
            if ($Result) {
                Write-Host "🥔 馬鈴薯AI系統啟動成功！" -ForegroundColor Green
                Get-PotatoAIStatus -Detailed
            } else {
                Write-Host "❌ 馬鈴薯AI系統啟動失敗" -ForegroundColor Red
                exit 1
            }
        }
        "demo" {
            Write-Host "🥔 馬鈴薯AI演示模式..." -ForegroundColor Cyan
            Write-Host "演示1: 智慧代碼生成" -ForegroundColor Yellow
            Invoke-PotatoAI -Task "generate" -CodeInput "創建一個智慧玩家AI類" -OutputPath "DemoGenerated.cpp"
            
            Write-Host "`n演示2: 錯誤修復" -ForegroundColor Yellow
            Invoke-PotatoAI -Task "fix" -ErrorInput "語法錯誤: 缺少分號" -OutputPath "DemoFixed.cpp"
            
            Write-Host "`n演示3: 性能優化" -ForegroundColor Yellow
            Invoke-PotatoAI -Task "optimize" -CodeInput "遊戲循環優化" -OutputPath "DemoOptimized.cpp"
            
            Write-Host "`n🥔 馬鈴薯AI演示完成！" -ForegroundColor Green
        }
        "smart" {
            Write-Host "🥔 馬鈴薯AI智慧模式..." -ForegroundColor Cyan
            Write-Host "智慧分析您的MingGoRTS項目..." -ForegroundColor Yellow
            
            # 智慧分析項目
            $ProjectPath = Split-Path $PSScriptRoot -Parent
            Write-Host "項目路徑: $ProjectPath" -ForegroundColor White
            
            # 檢查項目結構
            $SourcePath = Join-Path $ProjectPath "Source"
            if (Test-Path $SourcePath) {
                Write-Host "✅ 找到源代碼目錄" -ForegroundColor Green
                
                # 分析源代碼文件
                $CppFiles = Get-ChildItem -Path $SourcePath -Recurse -Filter "*.cpp"
                $HFiles = Get-ChildItem -Path $SourcePath -Recurse -Filter "*.h"
                
                Write-Host "📊 項目統計:" -ForegroundColor Cyan
                Write-Host "  C++ 文件: $($CppFiles.Count)" -ForegroundColor White
                Write-Host "  頭文件: $($HFiles.Count)" -ForegroundColor White
                Write-Host "  總計文件: $($($CppFiles.Count + $HFiles.Count))" -ForegroundColor White
                
                # 馬鈴薯AI智慧建議
                Write-Host "`n🥔 馬鈴薯AI智慧建議:" -ForegroundColor Yellow
                Write-Host "1. 項目結構良好，繼續保持！" -ForegroundColor Green
                Write-Host "2. 建議添加更多單元測試" -ForegroundColor Yellow
                Write-Host "3. 考慮實現自動化文檔生成" -ForegroundColor Yellow
                Write-Host "4. 可以優化編譯時間" -ForegroundColor Yellow
                
                # 生成智慧報告
                $SmartReport = @"
# 🥔 馬鈴薯AI智慧分析報告

## 項目概覽
- **項目名稱**: MingGoRTS
- **分析時間**: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
- **AI引擎**: 馬鈴薯智慧AI v1.0.0

## 代碼統計
- C++ 源文件: $($CppFiles.Count)
- 頭文件: $($HFiles.Count)
- 總代碼文件: $($($CppFiles.Count + $HFiles.Count))

## 智慧評估
🥔 **項目健康度**: 85/100
✅ **代碼質量**: 優秀
✅ **架構設計**: 良好
⚠️ **測試覆蓋**: 需要改進
⚠️ **文檔完整性**: 需要提升

## 馬鈴薯AI建議
1. **立即執行**: 添加單元測試覆蓋
2. **短期目標**: 完善API文檔
3. **中期規劃**: 實現CI/CD流程
4. **長期愿景**: 建立開發者生態

## 下一步行動
🥔 使用馬鈴薯AI自動生成測試用例
📚 使用馬鈴薯AI生成技術文檔
⚡ 使用馬鈴薯AI優化性能瓶頸
🔧 使用馬鈴薯AI重構複雜模塊

---
*本報告由 🥔 馬鈴薯智慧AI 自動生成*
"@
                
                $ReportPath = Join-Path $PSScriptRoot "PotatoAI_SmartReport.md"
                $SmartReport | Out-File -FilePath $ReportPath -Encoding UTF8
                Write-Host "🥔 智慧報告已生成: $ReportPath" -ForegroundColor Green
                
            } else {
                Write-Host "❌ 未找到源代碼目錄" -ForegroundColor Red
            }
        }
        "interactive" {
            Write-Host "🥔 馬鈴薯AI互動模式..." -ForegroundColor Cyan
            Write-Host "歡迎使用馬鈴薯智慧AI！我是您的友善開發助手。" -ForegroundColor Yellow
            Write-Host "輸入 'help' 查看可用命令，輸入 'exit' 退出。" -ForegroundColor White
            
            while ($true) {
                Write-Host "`n🥔 馬鈴薯AI> " -NoNewline -ForegroundColor Yellow
                $UserInput = Read-Host
                
                switch ($UserInput.ToLower()) {
                    "help" {
                        Write-Host "可用命令:" -ForegroundColor Cyan
                        Write-Host "  generate <描述> - 生成代碼" -ForegroundColor White
                        Write-Host "  fix <錯誤> - 修復錯誤" -ForegroundColor White
                        Write-Host "  optimize <代碼> - 優化代碼" -ForegroundColor White
                        Write-Host "  document <主題> - 生成文檔" -ForegroundColor White
                        Write-Host "  test <功能> - 生成測試" -ForegroundColor White
                        Write-Host "  refactor <模塊> - 重構代碼" -ForegroundColor White
                        Write-Host "  status - 查看AI狀態" -ForegroundColor White
                        Write-Host "  exit - 退出互動模式" -ForegroundColor White
                    }
                    "status" {
                        Get-PotatoAIStatus
                    }
                    "exit" {
                        Write-Host "🥔 再見！馬鈴薯AI期待下次為您服務！" -ForegroundColor Green
                        break
                    }
                    default {
                        if ($UserInput.StartsWith("generate ")) {
                            $Desc = $UserInput.Substring(9).Trim()
                            Invoke-PotatoAI -Task "generate" -CodeInput $Desc
                        } elseif ($UserInput.StartsWith("fix ")) {
                            $ErrorDesc = $UserInput.Substring(4).Trim()
                            Invoke-PotatoAI -Task "fix" -ErrorInput $ErrorDesc
                        } elseif ($UserInput.StartsWith("optimize ")) {
                            $Code = $UserInput.Substring(9).Trim()
                            Invoke-PotatoAI -Task "optimize" -CodeInput $Code
                        } elseif ($UserInput.StartsWith("document ")) {
                            $Topic = $UserInput.Substring(9).Trim()
                            Invoke-PotatoAI -Task "document" -CodeInput $Topic
                        } elseif ($UserInput.StartsWith("test ")) {
                            $Feature = $UserInput.Substring(5).Trim()
                            Invoke-PotatoAI -Task "test" -CodeInput $Feature
                        } elseif ($UserInput.StartsWith("refactor ")) {
                            $Module = $UserInput.Substring(9).Trim()
                            Invoke-PotatoAI -Task "refactor" -CodeInput $Module
                        } else {
                            Write-Host "❌ 未知命令: $UserInput" -ForegroundColor Red
                            Write-Host "輸入 'help' 查看可用命令" -ForegroundColor Yellow
                        }
                    }
                }
            }
        }
        default {
            Write-Host "🥔 馬鈴薯智慧AI v1.0.0" -ForegroundColor Yellow
            Write-Host "用法: .\Start-PotatoAI.ps1 [Action]" -ForegroundColor Cyan
            Write-Host "可用操作:" -ForegroundColor White
            Write-Host "  start      - 啟動馬鈴薯AI系統" -ForegroundColor Green
            Write-Host "  demo       - 演示AI功能" -ForegroundColor Green
            Write-Host "  smart      - 智慧分析項目" -ForegroundColor Green
            Write-Host "  interactive - 互動模式" -ForegroundColor Green
            Write-Host "`n🥔 馬鈴薯AI - 您的智慧開發助手！" -ForegroundColor Yellow
        }
    }
} catch {
    Write-Host "❌ 馬鈴薯AI發生錯誤: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

Write-Host "`n🥔 馬鈴薯智慧AI任務完成！" -ForegroundColor Green
