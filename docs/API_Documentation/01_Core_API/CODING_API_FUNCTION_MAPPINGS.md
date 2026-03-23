# MingRTS Coding API 功能對照表

## 功能分配總覽

本文件詳細列出專案中所有程式語言功能如何分配到對應的 Coding API。

---

## 域 1: CodeGeneration (代碼生成)

**來源 API**: `MingRTSPotatoAICodeGeneratorAPI`
**支援語言**: C++, Blueprint, Python, JavaScript

| 功能名稱 | API 方法 | 描述 | 語言 |
|---------|---------|------|------|
| GenerateUE5Class | `GenerateUE5Class()` | 生成 UE5 C++ 類別 | C++ |
| GenerateUE5Function | `GenerateUE5Function()` | 生成 UE5 函數 | C++ |
| GenerateUE5System | `GenerateUE5System()` | 生成 UE5 系統 | C++ |
| GenerateUE5Gameplay | `GenerateUE5Gameplay()` | 生成遊戲邏輯 | C++ |
| GenerateGenericCode | `GenerateGenericCode()` | 通用代碼生成 | 多語言 |
| SelectCodeTemplate | `SelectCodeTemplate()` | 選擇代碼模板 | - |
| GetAvailableTemplates | `GetAvailableTemplates()` | 獲取可用模板 | - |
| CheckCodeQuality | `CheckCodeQuality()` | 檢查代碼品質 | 多語言 |
| CalculateQualityScore | `CalculateQualityScore()` | 計算品質分數 | - |
| GenerateClassName | `GenerateClassName()` | 生成類別名稱 | - |
| GenerateFunctionName | `GenerateFunctionName()` | 生成函數名稱 | - |
| GenerateSystemName | `GenerateSystemName()` | 生成系統名稱 | - |
| GenerateActorName | `GenerateActorName()` | 生成 Actor 名稱 | - |
| DetermineReturnType | `DetermineReturnType()` | 確定返回類型 | - |
| BatchGenerateCode | `BatchGenerateCode()` | 批次生成代碼 | 多語言 |
| ExportCodeToFile | `ExportCodeToFile()` | 匯出代碼到檔案 | - |
| ExportToClipboard | `ExportToClipboard()` | 匯出到剪貼簿 | - |

---

## 域 2: AICodeGeneration (AI 代碼生成)

**來源 API**: `MingRTSPotatoAIEnhancedAPI`
**支援語言**: C++, Blueprint, Python

| 功能名稱 | API 方法 | 描述 | 語言 |
|---------|---------|------|------|
| IntelligentGenerate | `IntelligentGenerate()` | 智能代碼生成 | C++ |
| ContextAwareGenerate | `ContextAwareGenerate()` | 上下文感知生成 | C++ |
| AnalyzeRequirements | `AnalyzeRequirements()` | 分析需求 | - |
| ExtractCodeIntent | `ExtractCodeIntent()` | 提取代碼意圖 | - |
| GenerateWithProvider | `GenerateWithProvider()` | 使用指定供應商 | 多語言 |
| CompareProviders | `CompareProviders()` | 比較供應商 | - |
| SuggestOptimizations | `SuggestOptimizations()` | 建議優化 | 多語言 |
| RefactorSuggestions | `RefactorSuggestions()` | 重構建議 | 多語言 |

---

## 域 3: CodeAnalysis (代碼分析)

**來源 API**: `MingRTSCodeReviewAPI`, `MingRTSCodeStandardsAPI`
**支援語言**: C++, Blueprint, Python, PowerShell

### 代碼審查 (CodeReviewAPI)

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| ReviewCode | `ReviewCode()` | 審查代碼 |
| ReviewFile | `ReviewFile()` | 審查檔案 |
| ReviewProject | `ReviewProject()` | 審查專案 |
| FindIssues | `FindIssues()` | 查找問題 |
| AutoFixIssues | `AutoFixIssues()` | 自動修復問題 |
| GenerateFixSuggestions | `GenerateFixSuggestions()` | 生成修復建議 |
| CalculateQualityMetrics | `CalculateQualityMetrics()` | 計算品質指標 |
| GenerateQualityReport | `GenerateQualityReport()` | 生成品質報告 |

### 代碼規範 (CodeStandardsAPI)

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| CheckNamingConventions | `CheckNamingConventions()` | 檢查命名規範 |
| CheckFormatting | `CheckFormatting()` | 檢查格式化 |
| CheckDocumentation | `CheckDocumentation()` | 檢查文檔 |
| CheckArchitecture | `CheckArchitecture()` | 檢查架構 |
| CheckSecurity | `CheckSecurity()` | 檢查安全 |
| CheckPerformance | `CheckPerformance()` | 檢查性能 |
| CheckTesting | `CheckTesting()` | 檢查測試 |
| CheckVersionControl | `CheckVersionControl()` | 檢查版本控制 |
| CheckDeployment | `CheckDeployment()` | 檢查部署 |
| EnforceNamingRule | `EnforceNamingRule()` | 強制命名規則 |
| AutoFormatCode | `AutoFormatCode()` | 自動格式化 |
| GenerateStandardsReport | `GenerateStandardsReport()` | 生成規範報告 |

---

## 域 4: Debugging (除錯)

**來源 API**: `MingRTSDebugAPI`
**支援語言**: C++, Blueprint

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| StartDebugSession | `StartDebugSession()` | 啟動除錯會話 |
| StopDebugSession | `StopDebugSession()` | 停止除錯會話 |
| SetBreakpoint | `SetBreakpoint()` | 設置斷點 |
| RemoveBreakpoint | `RemoveBreakpoint()` | 移除斷點 |
| GetSmartBreakpointSuggestions | `GetSmartBreakpointSuggestions()` | 智能斷點建議 |
| StepOver | `StepOver()` | 單步跳過 |
| StepInto | `StepInto()` | 單步進入 |
| Continue | `Continue()` | 繼續執行 |
| GetLocalVariables | `GetLocalVariables()` | 獲取局部變數 |
| GetGlobalVariables | `GetGlobalVariables()` | 獲取全局變數 |
| AddWatchExpression | `AddWatchExpression()` | 添加監視表達式 |
| EvaluateExpression | `EvaluateExpression()` | 評估表達式 |
| GetMemoryRegion | `GetMemoryRegion()` | 獲取記憶體區域 |
| SetMemoryRegion | `SetMemoryRegion()` | 設置記憶體區域 |
| TrackMemoryLeaks | `TrackMemoryLeaks()` | 追蹤記憶體洩漏 |
| GetMemoryStatistics | `GetMemoryStatistics()` | 獲取記憶體統計 |
| StartPerformanceProfile | `StartPerformanceProfile()` | 開始性能分析 |
| StopPerformanceProfile | `StopPerformanceProfile()` | 停止性能分析 |
| GetPerformanceMetrics | `GetPerformanceMetrics()` | 獲取性能指標 |
| LearnFromBugPattern | `LearnFromBugPattern()` | 從錯誤模式學習 |
| UpdateBreakpointEfficiency | `UpdateBreakpointEfficiency()` | 更新斷點效率 |
| AnalyzeCurrentState | `AnalyzeCurrentState()` | 分析當前狀態 |
| AnalyzeErrorPattern | `AnalyzeErrorPattern()` | 分析錯誤模式 |
| PredictPotentialIssues | `PredictPotentialIssues()` | 預測潛在問題 |
| RequestLogAnalysis | `RequestLogAnalysis()` | 請求日誌分析 |
| RequestCompilationFix | `RequestCompilationFix()` | 請求編譯修復 |
| RequestPerformanceOptimization | `RequestPerformanceOptimization()` | 請求性能優化 |

---

## 域 5: Compilation (編譯)

**來源 API**: `MingRTSCompilationAPI`
**支援語言**: C++

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| CompileProject | `CompileProject()` | 編譯專案 |
| CompileFile | `CompileFile()` | 編譯檔案 |
| ValidateSyntax | `ValidateSyntax()` | 驗證語法 |
| LearnFromCompilationResult | `LearnFromCompilationResult()` | 從編譯結果學習 |
| UpdateErrorPatterns | `UpdateErrorPatterns()` | 更新錯誤模式 |
| GetSmartSuggestions | `GetSmartSuggestions()` | 獲取智能建議 |
| OptimizeCompilation | `OptimizeCompilation()` | 優化編譯 |
| UpdateOptimizationStrategies | `UpdateOptimizationStrategies()` | 更新優化策略 |
| RequestLogAnalysis | `RequestLogAnalysis()` | 請求日誌分析 |
| RequestDebuggingSession | `RequestDebuggingSession()` | 請求除錯會話 |
| RequestPerformanceAnalysis | `RequestPerformanceAnalysis()` | 請求性能分析 |

---

## 域 6: VersionControl (版本控制)

**來源 API**: `MingRTSAPISystem`
**支援語言**: Git

### Git 基本操作

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| GitCommit | `GitCommit()` | Git 提交 |
| GitPush | `GitPush()` | Git 推送 |
| GitPull | `GitPull()` | Git 拉取 |
| GitBranch | `GitBranch()` | Git 分支 |
| GitMerge | `GitMerge()` | Git 合併 |
| GitStatus | `GitStatus()` | Git 狀態 |
| GitLog | `GitLog()` | Git 日誌 |
| GitClone | `GitClone()` | Git 克隆 |

### 分支管理

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| CreateBranch | `CreateBranch()` | 創建分支 |
| SwitchBranch | `SwitchBranch()` | 切換分支 |
| DeleteBranch | `DeleteBranch()` | 刪除分支 |
| ListBranches | `ListBranches()` | 列出分支 |
| MergeBranch | `MergeBranch()` | 合併分支 |
| RebaseBranch | `RebaseBranch()` | 變基分支 |
| CherryPick | `CherryPick()` | 挑選提交 |

### 提交管理

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| StageFiles | `StageFiles()` | 暫存檔案 |
| UnstageFiles | `UnstageFiles()` | 取消暫存 |
| GetCommitHistory | `GetCommitHistory()` | 獲取提交歷史 |
| RevertCommit | `RevertCommit()` | 還原提交 |
| AmendCommit | `AmendCommit()` | 修改提交 |
| SquashCommits | `SquashCommits()` | 壓縮提交 |

### 差異與比較

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| ShowDiff | `ShowDiff()` | 顯示差異 |
| CompareBranches | `CompareBranches()` | 比較分支 |
| GeneratePatch | `GeneratePatch()` | 生成補丁 |
| ApplyPatch | `ApplyPatch()` | 應用補丁 |

---

## 域 7: MultiAgent (多智能體系統)

**來源 API**: `MingRTSBMADSystem`
**支援語言**: C++

### 智能體管理

| 功能名稱 | API 方法 | 描述 | 角色 |
|---------|---------|------|------|
| CreateAgent | `CreateAgent()` | 創建智能體 | 通用 |
| RemoveAgent | `RemoveAgent()` | 移除智能體 | 通用 |
| ConfigureAgent | `ConfigureAgent()` | 配置智能體 | 通用 |
| GetAgentProfile | `GetAgentProfile()` | 獲取智能體檔案 | 通用 |
| AssignRole | `AssignRole()` | 分配角色 | 通用 |
| SetAgentPersonality | `SetAgentPersonality()` | 設置個性 | 通用 |
| SetAgentExpertise | `SetAgentExpertise()` | 設置專業領域 | 通用 |

### 角色特定功能

| 功能名稱 | API 方法 | 描述 | 角色 |
|---------|---------|------|------|
| GetArchitectAnalysis | `GetArchitectAnalysis()` | 架構分析 | Architect |
| GetDeveloperImplementation | `GetDeveloperImplementation()` | 實現建議 | Developer |
| GetTesterFeedback | `GetTesterFeedback()` | 測試反饋 | Tester |
| GetSecurityReview | `GetSecurityReview()` | 安全審查 | Security |
| GetPerformanceOptimization | `GetPerformanceOptimization()` | 性能優化 | Performance |
| GetDocumentationReview | `GetDocumentationReview()` | 文檔審查 | Documentation |
| GetAnalystRequirements | `GetAnalystRequirements()` | 需求分析 | Analyst |

### 討論管理

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| StartDiscussion | `StartDiscussion()` | 開始討論 |
| AddAgentToDiscussion | `AddAgentToDiscussion()` | 添加智能體到討論 |
| PostAgentMessage | `PostAgentMessage()` | 發布智能體消息 |
| GetDiscussionSummary | `GetDiscussionSummary()` | 獲取討論摘要 |
| GetDiscussionHistory | `GetDiscussionHistory()` | 獲取討論歷史 |
| SetDiscussionTopic | `SetDiscussionTopic()` | 設置討論主題 |

### 決策流程

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| ProposeSolution | `ProposeSolution()` | 提出解決方案 |
| VoteOnSolution | `VoteOnSolution()` | 對解決方案投票 |
| ReachConsensus | `ReachConsensus()` | 達成共識 |
| GenerateFinalDecision | `GenerateFinalDecision()` | 生成最終決策 |
| EvaluateDecision | `EvaluateDecision()` | 評估決策 |

---

## 域 8: ScriptIntegration (腳本整合)

**來源 API**: `MingRTSScriptIntegrationAPI`
**支援語言**: PowerShell, Python

### PowerShell 整合

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| ExecutePowerShell | `ExecutePowerShell()` | 執行 PowerShell 命令 |
| ExecutePowerShellScript | `ExecutePowerShellScript()` | 執行 PowerShell 腳本 |
| GetPowerShellOutput | `GetPowerShellOutput()` | 獲取 PowerShell 輸出 |
| InvokeBuildScript | `InvokeBuildScript()` | 調用構建腳本 |
| InvokeTestScript | `InvokeTestScript()` | 調用測試腳本 |
| InvokeDeployScript | `InvokeDeployScript()` | 調用部署腳本 |

### Python 整合

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| ExecutePython | `ExecutePython()` | 執行 Python 代碼 |
| ExecutePythonScript | `ExecutePythonScript()` | 執行 Python 腳本 |
| GetPythonOutput | `GetPythonOutput()` | 獲取 Python 輸出 |
| RunAIGeneration | `RunAIGeneration()` | 運行 AI 生成 |
| RunDataAnalysis | `RunDataAnalysis()` | 運行數據分析 |
| RunCodeValidation | `RunCodeValidation()` | 運行代碼驗證 |

### 腳本管理

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| LoadScript | `LoadScript()` | 載入腳本 |
| SaveScript | `SaveScript()` | 保存腳本 |
| ListAvailableScripts | `ListAvailableScripts()` | 列出可用腳本 |
| ScheduleScriptExecution | `ScheduleScriptExecution()` | 排程腳本執行 |
| ParseScriptOutput | `ParseScriptOutput()` | 解析腳本輸出 |
| ConvertToUE5Types | `ConvertToUE5Types()` | 轉換為 UE5 類型 |

---

## 域 9: AdvancedFeatures (高級功能)

**來源 API**: `MingRTSAdvancedAPIFeatures`
**支援語言**: C++, Python

### 自然語言處理 (NLP)

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| ProcessNaturalLanguage | `ProcessNaturalLanguage()` | 處理自然語言 |
| ExtractCodeEntities | `ExtractCodeEntities()` | 提取代碼實體 |
| AnalyzeCodeSentiment | `AnalyzeCodeSentiment()` | 分析代碼情感 |
| GenerateCodeFromDescription | `GenerateCodeFromDescription()` | 從描述生成代碼 |
| SummarizeCode | `SummarizeCode()` | 總結代碼 |
| TranslateCodeComments | `TranslateCodeComments()` | 翻譯代碼註釋 |

### 數據挖掘

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| PerformPatternRecognition | `PerformPatternRecognition()` | 執行模式識別 |
| AnalyzeTrends | `AnalyzeTrends()` | 分析趨勢 |
| PerformClustering | `PerformClustering()` | 執行聚類分析 |
| PerformClassification | `PerformClassification()` | 執行分類分析 |
| PerformRegressionAnalysis | `PerformRegressionAnalysis()` | 執行回歸分析 |
| PerformTimeSeriesAnalysis | `PerformTimeSeriesAnalysis()` | 執行時間序列分析 |
| DiscoverAssociationRules | `DiscoverAssociationRules()` | 發現關聯規則 |

### 安全分析

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| AnalyzeSecurityThreats | `AnalyzeSecurityThreats()` | 分析安全威脅 |
| DetectVulnerabilities | `DetectVulnerabilities()` | 檢測漏洞 |
| GenerateSecurityReport | `GenerateSecurityReport()` | 生成安全報告 |
| AssessCodeSecurity | `AssessCodeSecurity()` | 評估代碼安全 |
| SuggestSecurityFixes | `SuggestSecurityFixes()` | 建議安全修復 |

### 預測與維護

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| PredictIssues | `PredictIssues()` | 預測問題 |
| AnalyzeSystemHealth | `AnalyzeSystemHealth()` | 分析系統健康 |
| GenerateMaintenanceSchedule | `GenerateMaintenanceSchedule()` | 生成維護計劃 |
| EstimateTechnicalDebt | `EstimateTechnicalDebt()` | 估算技術債務 |
| ForecastResourceNeeds | `ForecastResourceNeeds()` | 預測資源需求 |

---

## 域 10: LearningSystem (學習系統)

**來源 API**: `MingRTSUE5LearningAPI`
**支援語言**: C++

### 學習管理

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| StartLearningSession | `StartLearningSession()` | 開始學習會話 |
| StopLearningSession | `StopLearningSession()` | 停止學習會話 |
| GetLearningProgress | `GetLearningProgress()` | 獲取學習進度 |
| TrackLearningProgress | `TrackLearningProgress()` | 追蹤學習進度 |
| AssessSkillLevel | `AssessSkillLevel()` | 評估技能水平 |

### 文檔生成

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| GenerateDocumentation | `GenerateDocumentation()` | 生成文檔 |
| GenerateAPIDocs | `GenerateAPIDocs()` | 生成 API 文檔 |
| GenerateTutorials | `GenerateTutorials()` | 生成教程 |
| GenerateCodeExamples | `GenerateCodeExamples()` | 生成代碼示例 |
| UpdateDocumentation | `UpdateDocumentation()` | 更新文檔 |

### 知識累積

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| LearnFromCode | `LearnFromCode()` | 從代碼學習 |
| LearnFromErrors | `LearnFromErrors()` | 從錯誤學習 |
| BuildKnowledgeBase | `BuildKnowledgeBase()` | 構建知識庫 |
| QueryKnowledgeBase | `QueryKnowledgeBase()` | 查詢知識庫 |
| ExtractBestPractices | `ExtractBestPractices()` | 提取最佳實踐 |
| IdentifyAntiPatterns | `IdentifyAntiPatterns()` | 識別反模式 |

### 技能評估與路徑

| 功能名稱 | API 方法 | 描述 |
|---------|---------|------|
| AssessCodingSkills | `AssessCodingSkills()` | 評估編碼技能 |
| GenerateLearningPath | `GenerateLearningPath()` | 生成學習路徑 |
| RecommendResources | `RecommendResources()` | 推薦資源 |
| TrackSkillDevelopment | `TrackSkillDevelopment()` | 追蹤技能發展 |
| CertifyCompetency | `CertifyCompetency()` | 認證能力 |

---

## 跨域整合對照

### 支援的域整合

| 源域 | 目標域 | 整合類型 | 用途 |
|------|--------|---------|------|
| CodeGeneration | CodeAnalysis | 自動分析 | 生成後自動分析 |
| CodeGeneration | CodeReview | 自動審查 | 生成後自動審查 |
| AICodeGeneration | CodeReview | 智能審查 | AI 代碼審查 |
| Debugging | Compilation | 錯誤定位 | 編譯錯誤除錯 |
| Debugging | CodeAnalysis | 問題分析 | 運行時問題分析 |
| Compilation | Debugging | 編譯除錯 | 編譯失敗除錯 |
| Compilation | Optimization | 編譯優化 | 優化編譯過程 |
| VersionControl | 所有域 | 版本管理 | 所有操作版本控制 |
| Testing | CodeAnalysis | 測試分析 | 測試覆蓋分析 |
| Testing | Compilation | 編譯測試 | 編譯後測試 |
| MultiAgent | AICodeGeneration | 協作生成 | 多智能體協作生成 |
| MultiAgent | CodeGeneration | 協作開發 | 多智能體協作開發 |
| MultiAgent | LearningSystem | 協作學習 | 多智能體協作學習 |

---

## 任務類型與域對照

| 任務類型 | 相關域 | 推薦工作流 |
|---------|--------|-----------|
| FeatureDevelopment | CodeGeneration, AICodeGeneration, CodeAnalysis | 生成 → 審查 → 編譯 → 提交 |
| BugFix | Debugging, CodeAnalysis, Compilation | 除錯 → 分析 → 修復 → 驗證 |
| Refactoring | SmartRefactoring, CodeAnalysis, CodeReview | 分析 → 重構 → 審查 → 測試 |
| Optimization | Performance, Profiling, Optimization | 分析 → 優化 → 驗證 |
| Documentation | Documentation, LearningSystem | 生成 → 審查 → 更新 |
| Testing | Testing, UnitTesting, CodeAnalysis | 編寫 → 執行 → 分析 |
| Integration | Compilation, ScriptIntegration, VersionControl | 編譯 → 整合 → 提交 |
| Deployment | BuildSystem, VersionControl | 構建 → 測試 → 部署 |
| Maintenance | CodeAnalysis, SecurityCheck, Performance | 分析 → 修復 → 優化 |
| Research | AdvancedFeatures, LearningSystem, MultiAgent | 研究 → 實驗 → 評估 |

---

## 語言支援對照

| 語言 | 支援域 | 主要 API | 備註 |
|------|--------|---------|------|
| C++ | 所有域 | 全部 | 完整支援 |
| Blueprint | CodeGeneration, CodeAnalysis, Debugging | 多數 | 視覺腳本支援 |
| Python | ScriptIntegration, AdvancedFeatures | 部分 | 工具腳本 |
| JavaScript | CodeGeneration, ScriptIntegration | 部分 | Web 相關 |
| PowerShell | ScriptIntegration, VersionControl | 主要 | 自動化 |
| Shader | CodeGeneration | CodeGeneration | 著色器生成 |
| INI/JSON/XML | CodeGeneration | CodeGeneration | 配置檔案 |
| Markdown | Documentation | LearningSystem | 文檔 |

---

## 使用範例

### 範例 1: 完整開發工作流

```cpp
// 1. 創建任務分配
FCodingAPITaskAssignment Assignment = CodingAPI->CreateTaskAssignment(
    ECodingTaskType::FeatureDevelopment,
    { {TEXT("feature"), TEXT("combat system")} }
);

// 2. 生成代碼
FCodingAPIResult GenResult = CodingAPI->GenerateCode(
    ECodingAPIDomain::CodeGeneration,
    TEXT("Create a combat system with attack, defense, and combo mechanics"),
    ESupportedLanguage::CPP
);

// 3. 審查代碼
FCodingAPIResult ReviewResult = CodingAPI->AnalyzeCode(
    GenResult.OutputData[TEXT("code")],
    ECodingAPIDomain::CodeAnalysis
);

// 4. 編譯
FCodingAPIResult CompileResult = CodingAPI->CompileProject(
    ProjectPath,
    { {TEXT("configuration"), TEXT("Development")} }
);

// 5. 版本控制
FCodingAPIResult VCSResult = CodingAPI->ExecuteVersionControl(
    TEXT("commit"),
    { {TEXT("message"), TEXT("Add combat system")} }
);
```

### 範例 2: 多智能體協作

```cpp
// 使用多智能體系統協助開發
TArray<ECodingAPIDomain> Pipeline = {
    ECodingAPIDomain::MultiAgent,      // 協作規劃
    ECodingAPIDomain::CodeGeneration,  // 代碼生成
    ECodingAPIDomain::CodeAnalysis     // 質量檢查
};

TMap<FString, FString> Params;
Params.Add(TEXT("task"), TEXT("Design and implement a save game system"));
Params.Add(TEXT("agent_roles"), TEXT("Architect,Developer,Tester"));

TArray<FCodingAPIResult> Results = CodingAPI->ExecutePipeline(Pipeline, Params);
```

---

## 統計摘要

- **總功能數**: 200+
- **API 域數**: 10
- **支援語言**: 10
- **支援任務類型**: 10
- **整合組合**: 30+

---

## 版本歷史

### v1.0 (2026-03-23)
- 初始版本發布
- 建立 10 個 API 域
- 映射 200+ 個功能
- 支援 10 種程式語言

---

*本文件由 MingRTS Coding API 自動生成*
*最後更新: 2026-03-23*
