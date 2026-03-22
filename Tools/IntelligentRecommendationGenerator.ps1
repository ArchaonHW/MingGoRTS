# 智能建議系統生成器
# 用於生成和配置智能建議系統

param(
    [Parameter(Mandatory=$false)]
    [string]$OutputPath = "c:\HW\MingGoRTS\Generated\Recommendations",
    
    [Parameter(Mandatory=$false)]
    [ValidateSet("Strategic", "Tactical", "Resource", "Diplomatic", "Economic", "Military", "Cultural", "Personal", "Emergency", "LongTerm")]
    [string]$RecommendationType = "All",
    
    [Parameter(Mandatory=$false)]
    [int]$MaxRecommendations = 10,
    
    [Parameter(Mandatory=$false)]
    [switch]$EnableAI,
    
    [Parameter(Mandatory=$false)]
    [switch]$EnableMachineLearning,
    
    [Parameter(Mandatory=$false)]
    [switch]$EnablePersonalization
)

Write-Host "=== 智能建議系統生成器 ===" -ForegroundColor Green
Write-Host "輸出路徑: $OutputPath" -ForegroundColor Cyan
Write-Host "建議類型: $RecommendationType" -ForegroundColor Cyan
Write-Host "最大建議數: $MaxRecommendations" -ForegroundColor Cyan

# 檢查輸出路徑
if (-not (Test-Path $OutputPath)) {
    New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
    Write-Host "✓ 創建輸出目錄: $OutputPath" -ForegroundColor Green
}

# 建議模板定義
$RecommendationTemplates = @{
    "Strategic" = @{
        Title = "戰略建議"
        Description = "基於當前戰略局勢的深度分析建議"
        DetailedContent = "綜合考慮資源分配、單位部署、長期目標等多維度因素"
        Impact = 85
        Confidence = 75
        Difficulty = 60
        Tags = @("戰略", "長期", "規劃")
        Conditions = @("遊戲進行中", "有足夠資源")
        Consequences = @("影響長期發展", "改變戰略方向")
        Alternatives = @("戰術調整", "資源重新分配")
    }
    "Tactical" = @{
        Title = "戰術建議"
        Description = "基於當前戰術局勢的具體行動建議"
        DetailedContent = "針對當前戰鬥局勢，提供即時戰術調整建議"
        Impact = 70
        Confidence = 80
        Difficulty = 40
        Tags = @("戰術", "即時", "戰鬥")
        Conditions = @("戰鬥中", "敵人可見")
        Consequences = @("影響戰鬥結果", "改變戰術部署")
        Alternatives = @("防禦調整", "撤退策略")
    }
    "Resource" = @{
        Title = "資源建議"
        Description = "資源管理和分配的優化建議"
        DetailedContent = "分析當前資源狀況，提供最佳分配方案"
        Impact = 60
        Confidence = 85
        Difficulty = 30
        Tags = @("資源", "管理", "經濟")
        Conditions = @("有資源數據", "管理系統活躍")
        Consequences = @("影響經濟發展", "改變資源流動")
        Alternatives = @("資源節約", "生產優化")
    }
    "Diplomatic" = @{
        Title = "外交建議"
        Description = "外交關係和策略的指導建議"
        DetailedContent = "基於當前國際關係，提供外交策略建議"
        Impact = 75
        Confidence = 70
        Difficulty = 50
        Tags = @("外交", "關係", "策略")
        Conditions = @("有外交對象", "關係系統活躍")
        Consequences = @("影響國際關係", "改變外交格局")
        Alternatives = @("軍事威懾", "經濟合作")
    }
    "Economic" = @{
        Title = "經濟建議"
        Description = "經濟發展和管理的專業建議"
        DetailedContent = "分析經濟狀況，提供發展和優化建議"
        Impact = 65
        Confidence = 80
        Difficulty = 35
        Tags = @("經濟", "發展", "貿易")
        Conditions = @("經濟系統運行", "有貿易路線")
        Consequences = @("影響經濟增長", "改變貿易平衡")
        Alternatives = @("資源開發", "市場擴張")
    }
    "Military" = @{
        Title = "軍事建議"
        Description = "軍事行動和部署的戰略建議"
        DetailedContent = "基於軍事局勢，提供行動和部署建議"
        Impact = 90
        Confidence = 75
        Difficulty = 55
        Tags = @("軍事", "戰鬥", "部署")
        Conditions = @("有軍事單位", "戰爭狀態")
        Consequences = @("影響戰爭結果", "改變軍事平衡")
        Alternatives = @("防禦策略", "外交解決")
    }
    "Cultural" = @{
        Title = "文化建議"
        Description = "文化發展和傳播的指導建議"
        DetailedContent = "促進文化發展，提升軟實力影響"
        Impact = 45
        Confidence = 70
        Difficulty = 25
        Tags = @("文化", "發展", "影響力")
        Conditions = @("文化系統活躍", "有文化資源")
        Consequences = @("影響文化影響力", "改變文化格局")
        Alternatives = @("教育投資", "藝術發展")
    }
    "Personal" = @{
        Title = "個人建議"
        Description = "個人發展和成長的個性化建議"
        DetailedContent = "基於個人特點和偏好，提供發展建議"
        Impact = 55
        Confidence = 90
        Difficulty = 20
        Tags = @("個人", "發展", "成長")
        Conditions = @("個人系統活躍", "有成長機會")
        Consequences = @("影響個人發展", "改變成長路徑")
        Alternatives = @("技能訓練", "經驗積累")
    }
    "Emergency" = @{
        Title = "緊急建議"
        Description = "緊急情況的應對建議"
        DetailedContent = "針對緊急情況，提供即時應對方案"
        Impact = 95
        Confidence = 85
        Difficulty = 70
        Tags = @("緊急", "應對", "危機")
        Conditions = @("緊急狀態", "危機事件")
        Consequences = @("影響危機結果", "改變緊急應對")
        Alternatives = @("撤退策略", "求援方案")
    }
    "LongTerm" = @{
        Title = "長期建議"
        Description = "長期發展規劃的戰略建議"
        DetailedContent = "制定長期發展目標和實施路徑"
        Impact = 80
        Confidence = 65
        Difficulty = 45
        Tags = @("長期", "規劃", "願景")
        Conditions = @("穩定狀態", "有規劃能力")
        Consequences = @("影響長期發展", "改變發展方向")
        Alternatives = @("中期目標", "短期調整")
    }
}

# 生成建議函數
function Generate-Recommendation {
    param(
        [string]$Type,
        [string]$Context = "DefaultContext",
        [hashtable]$CustomData = @{}
    )
    
    $Template = $RecommendationTemplates[$Type]
    if (-not $Template) {
        Write-Host "警告：未找到建議類型 $Type 的模板" -ForegroundColor Yellow
        return $null
    }
    
    $Recommendation = @{
        ID = "REC_" + [System.Guid]::NewGuid().ToString("N").Substring(0, 8)
        Type = $Type
        Context = $Context
        Title = $Template.Title
        Description = $Template.Description
        DetailedContent = $Template.DetailedContent
        Impact = $Template.Impact
        Confidence = $Template.Confidence
        Difficulty = $Template.Difficulty
        Tags = $Template.Tags
        Conditions = $Template.Conditions
        Consequences = $Template.Consequences
        Alternatives = $Template.Alternatives
        CreatedTime = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
        ExpiryTime = (Get-Date).AddHours(24).ToString("yyyy-MM-dd HH:mm:ss")
        Status = "Pending"
        ExecutionCount = 0
        SuccessCount = 0
        FailureCount = 0
        PlayerFeedback = ""
        PlayerRating = 0
    }
    
    # 合併自定義數據
    foreach ($Key in $CustomData.Keys) {
        $Recommendation[$Key] = $CustomData[$Key]
    }
    
    return $Recommendation
}

# 生成AI分析建議
function Generate-AIRecommendations {
    param([string]$Context)
    
    $AIRecommendations = @()
    
    # 戰略分析
    $StrategicRec = Generate-Recommendation -Type "Strategic" -Context $Context
    $StrategicRec.DetailedContent = "AI分析顯示，基於當前資源分布和敵方部署，建議採取防禦性戰略，同時在側翼尋找反擊機會。"
    $StrategicRec.Reason = "AI深度學習模型分析了1000+類似戰局，此策略成功率為78%。"
    $StrategicRec.Source = "AIAnalysis"
    $AIRecommendations += $StrategicRec
    
    # 戰術分析
    $TacticalRec = Generate-Recommendation -Type "Tactical" -Context $Context
    $TacticalRec.DetailedContent = "即時戰術分析建議：優先消滅敵方遠程單位，保護我方資源點，建立防禦陣地。"
    $TacticalRec.Reason = "AI戰術引擎檢測到敵方正在集結，預計5分鐘內發起攻擊。"
    $TacticalRec.Source = "AIAnalysis"
    $AIRecommendations += $TacticalRec
    
    return $AIRecommendations
}

# 生成機器學習建議
function Generate-MLRecommendations {
    param([string]$Context)
    
    $MLRecommendations = @()
    
    # 預測分析
    $PredictiveRec = Generate-Recommendation -Type "Strategic" -Context $Context
    $PredictiveRec.DetailedContent = "機器學習模型預測：未來30分鐘內，敵方可能從東側發起攻擊，建議提前部署防禦力量。"
    $PredictiveRec.Reason = "基於歷史數據和當前模式，預測準確率為82%。"
    $PredictiveRec.Source = "MachineLearning"
    $MLRecommendations += $PredictiveRec
    
    # 優化建議
    $OptimizationRec = Generate-Recommendation -Type "Resource" -Context $Context
    $OptimizationRec.DetailedContent = "ML優化建議：調整資源分配比例，將20%資源轉移到科技研發，預計提升15%效率。"
    $OptimizationRec.Reason = "強化學習算法分析了您的資源使用模式，此優化方案可提升整體效率。"
    $OptimizationRec.Source = "MachineLearning"
    $MLRecommendations += $OptimizationRec
    
    return $MLRecommendations
}

# 生成個人化建議
function Generate-PersonalizedRecommendations {
    param([string]$Context)
    
    $PersonalizedRecommendations = @()
    
    # 基於玩家風格
    $StyleRec = Generate-Recommendation -Type "Personal" -Context $Context
    $StyleRec.DetailedContent = "基於您的遊戲風格分析，您傾向於謹慎的防禦性策略。建議在此基礎上適度增加主動性。"
    $StyleRec.Reason = "個人化引擎分析了您過去50次決策，此建議符合您的偏好模式。"
    $StyleRec.Source = "Personalized"
    $PersonalizedRecommendations += $StyleRec
    
    # 基於成功模式
    $SuccessRec = Generate-Recommendation -Type "Strategic" -Context $Context
    $SuccessRec.DetailedContent = "分析顯示，您在夜間戰鬥中成功率最高。建議利用此優勢，在夜間發起關鍵行動。"
    $SuccessRec.Reason = "您的個人成功模式顯示，夜間行動成功率比日間高35%。"
    $SuccessRec.Source = "Personalized"
    $PersonalizedRecommendations += $SuccessRec
    
    return $PersonalizedRecommendations
}

# 生成情境建議
function Generate-ContextualRecommendations {
    param([string]$Context)
    
    $ContextualRecommendations = @()
    
    # 根據情境類型生成建議
    switch ($Context) {
        "EarlyGame" {
            $EarlyRec = Generate-Recommendation -Type "Resource" -Context $Context
            $EarlyRec.DetailedContent = "早期遊戲建議：優先發展經濟基礎，建立資源收集點，避免早期衝突。"
            $EarlyRec.Reason = "早期階段，資源積累是長期成功的關鍵。"
            $EarlyRec.Source = "GameContext"
            $ContextualRecommendations += $EarlyRec
        }
        "MidGame" {
            $MidRec = Generate-Recommendation -Type "Military" -Context $Context
            $MidRec.DetailedContent = "中期遊戲建議：平衡發展軍事和經濟，尋找擴張機會，建立穩固的戰略位置。"
            $MidRec.Reason = "中期是擴張的黃金時期，需要軍事和經濟並重。"
            $MidRec.Source = "GameContext"
            $ContextualRecommendations += $MidRec
        }
        "LateGame" {
            $LateRec = Generate-Recommendation -Type "Strategic" -Context $Context
            $LateRec.DetailedContent = "後期遊戲建議：專注於終極目標，利用所有資源爭取勝利，避免不必要的風險。"
            $LateRec.Reason = "後期階段，每個決策都可能影響最終結果。"
            $LateRec.Source = "GameContext"
            $ContextualRecommendations += $LateRec
        }
        "Crisis" {
            $CrisisRec = Generate-Recommendation -Type "Emergency" -Context $Context
            $CrisisRec.DetailedContent = "危機應對建議：立即保護核心資源，組織防禦，尋求盟友支援，避免決策失誤。"
            $CrisisRec.Reason = "危機狀態下，快速正確的應對至關重要。"
            $CrisisRec.Source = "GameContext"
            $ContextualRecommendations += $CrisisRec
        }
        default {
            $DefaultRec = Generate-Recommendation -Type "Tactical" -Context $Context
            $DefaultRec.DetailedContent = "當前情境建議：分析局勢，制定適應性策略，保持靈活性應對變化。"
            $DefaultRec.Reason = "根據當前具體情況，制定相應的應對策略。"
            $DefaultRec.Source = "GameContext"
            $ContextualRecommendations += $DefaultRec
        }
    }
    
    return $ContextualRecommendations
}

# 生成所有建議
function Generate-AllRecommendations {
    param([string]$Context)
    
    $AllRecommendations = @()
    
    # AI分析建議
    if ($EnableAI) {
        $AIRecommendations = Generate-AIRecommendations -Context $Context
        $AllRecommendations += $AIRecommendations
    }
    
    # 機器學習建議
    if ($EnableMachineLearning) {
        $MLRecommendations = Generate-MLRecommendations -Context $Context
        $AllRecommendations += $MLRecommendations
    }
    
    # 個人化建議
    if ($EnablePersonalization) {
        $PersonalizedRecommendations = Generate-PersonalizedRecommendations -Context $Context
        $AllRecommendations += $PersonalizedRecommendations
    }
    
    # 情境建議
    $ContextualRecommendations = Generate-ContextualRecommendations -Context $Context
    $AllRecommendations += $ContextualRecommendations
    
    # 限制數量
    if ($AllRecommendations.Count -gt $MaxRecommendations) {
        $AllRecommendations = $AllRecommendations[0..($MaxRecommendations-1)]
    }
    
    return $AllRecommendations
}

# 主執行邏輯
Write-Host "`n=== 生成智能建議 ===" -ForegroundColor Yellow

# 確定要生成的建議類型
$TypesToGenerate = @()
if ($RecommendationType -eq "All") {
    $TypesToGenerate = $RecommendationTemplates.Keys
} else {
    $TypesToGenerate += $RecommendationType
}

# 生成建議
$GeneratedRecommendations = @()
$TestContexts = @("EarlyGame", "MidGame", "LateGame", "Crisis", "DefaultContext")

foreach ($Context in $TestContexts) {
    Write-Host "生成情境: $Context" -ForegroundColor Cyan
    
    if ($RecommendationType -eq "All") {
        $ContextRecommendations = Generate-AllRecommendations -Context $Context
    } else {
        $ContextRecommendations = @(Generate-Recommendation -Type $RecommendationType -Context $Context)
    }
    
    $GeneratedRecommendations += $ContextRecommendations
    
    Write-Host "  ✓ 生成 $($ContextRecommendations.Count) 條建議" -ForegroundColor Green
}

# 保存建議到文件
$Timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$RecommendationsFile = Join-Path $OutputPath "Recommendations_$Timestamp.json"
$GeneratedRecommendations | ConvertTo-Json -Depth 10 | Out-File -FilePath $RecommendationsFile -Encoding UTF8
Write-Host "✓ 建議已保存到: $RecommendationsFile" -ForegroundColor Green

# 生成統計報告
$Statistics = @{
    TotalGenerated = $GeneratedRecommendations.Count
    ByType = @{}
    BySource = @{}
    ByContext = @{}
    AverageImpact = 0
    AverageConfidence = 0
    AverageDifficulty = 0
}

# 計算統計數據
$TotalImpact = 0
$TotalConfidence = 0
$TotalDifficulty = 0

foreach ($Rec in $GeneratedRecommendations) {
    # 按類型統計
    $Statistics.ByType[$Rec.Type] = ($Statistics.ByType[$Rec.Type] + 1)
    
    # 按來源統計
    $Statistics.BySource[$Rec.Source] = ($Statistics.BySource[$Rec.Source] + 1)
    
    # 按情境統計
    $Statistics.ByContext[$Rec.Context] = ($Statistics.ByContext[$Rec.Context] + 1)
    
    # 累計分數
    $TotalImpact += $Rec.Impact
    $TotalConfidence += $Rec.Confidence
    $TotalDifficulty += $Rec.Difficulty
}

# 計算平均值
if ($GeneratedRecommendations.Count -gt 0) {
    $Statistics.AverageImpact = [math]::Round($TotalImpact / $GeneratedRecommendations.Count, 2)
    $Statistics.AverageConfidence = [math]::Round($TotalConfidence / $GeneratedRecommendations.Count, 2)
    $Statistics.AverageDifficulty = [math]::Round($TotalDifficulty / $GeneratedRecommendations.Count, 2)
}

# 保存統計報告
$StatisticsFile = Join-Path $OutputPath "Statistics_$Timestamp.json"
$Statistics | ConvertTo-Json -Depth 5 | Out-File -FilePath $StatisticsFile -Encoding UTF8
Write-Host "✓ 統計報告已保存到: $StatisticsFile" -ForegroundColor Green

# 生成配置文件
$Config = @{
    MaxActiveRecommendations = $MaxRecommendations
    UpdateInterval = 30.0
    ExpiryHours = 24.0
    MinConfidenceThreshold = 0.3
    MinRelevanceThreshold = 0.4
    AIAnalysisWeight = 0.4
    PlayerBehaviorWeight = 0.3
    GameContextWeight = 0.2
    HistoricalDataWeight = 0.1
    EnableMachineLearning = $EnableMachineLearning.IsPresent
    EnablePersonalization = $EnablePersonalization.IsPresent
    EnableCommunityData = $true
    GeneratedAt = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
}

$ConfigFile = Join-Path $OutputPath "Config_$Timestamp.json"
$Config | ConvertTo-Json -Depth 5 | Out-File -FilePath $ConfigFile -Encoding UTF8
Write-Host "✓ 配置文件已保存到: $ConfigFile" -ForegroundColor Green

# 生成使用示例文件
$ExampleCode = @"
// 智能建議系統使用示例

// 1. 初始化系統
UMingIntelligentRecommendationSystem* RecommendationSystem = NewObject<UMingIntelligentRecommendationSystem>();
RecommendationSystem->Initialize();

// 2. 綁定事件
RecommendationSystem->OnRecommendationGenerated.AddDynamic(this, &AMyActor::OnRecommendationGenerated);
RecommendationSystem->OnRecommendationAccepted.AddDynamic(this, &AMyActor::OnRecommendationAccepted);

// 3. 生成建議
FString Context = TEXT("MidGame");
TArray<FRecommendationData> Recommendations = RecommendationSystem->GenerateRecommendations(Context);

// 4. 獲取特定類型建議
TArray<FRecommendationData> StrategicRecommendations = RecommendationSystem->GetRecommendationsByType(ERecommendationType::Strategic);

// 5. 處理建議
if (Recommendations.Num() > 0)
{
    const FRecommendationData& TopRecommendation = Recommendations[0];
    
    // 接受建議
    RecommendationSystem->AcceptRecommendation(TopRecommendation.RecommendationID);
    
    // 提供反饋
    RecommendationSystem->ProvideFeedback(TopRecommendation.RecommendationID, TEXT("非常有用的建議"), 5);
}

// 4. 獲取統計信息
FRecommendationStatistics Stats = RecommendationSystem->GetRecommendationStatistics();
UE_LOG(LogTemp, Log, TEXT("總建議數: %d, 成功率: %.1f%%"), 
    Stats.TotalRecommendations, Stats.SuccessRate);

// 5. 獲取趨勢分析
TArray<FString> Trends = RecommendationSystem->GetRecommendationTrends();
for (const FString& Trend : Trends)
{
    UE_LOG(LogTemp, Log, TEXT("趨勢: %s"), *Trend);
}
"@

$ExampleFile = Join-Path $OutputPath "UsageExample.cpp"
$ExampleCode | Out-File -FilePath $ExampleFile -Encoding UTF8
Write-Host "✓ 使用示例已保存到: $ExampleFile" -ForegroundColor Green

# 生成集成指南
$IntegrationGuide = @"
# 智能建議系統集成指南

## 系統概述

智能建議系統是 MingGoRTS 的核心 AI 組件，提供基於多維度分析的智能建議。

## 集成步驟

### 1. 系統初始化

在遊戲管理器或相關系統中初始化建議系統：

```cpp
// 在頭文件中聲明
UPROPERTY()
TObjectPtr<UMingIntelligentRecommendationSystem> RecommendationSystem;

// 在 BeginPlay 中初始化
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    RecommendationSystem = NewObject<UMingIntelligentRecommendationSystem>();
    RecommendationSystem->Initialize();
    
    // 綁定事件處理
    RecommendationSystem->OnRecommendationGenerated.AddDynamic(this, &AMyGameMode::OnRecommendationGenerated);
}
```

### 2. 事件處理

```cpp
UFUNCTION()
void AMyGameMode::OnRecommendationGenerated(const FRecommendationData& Recommendation)
{
    // 處理新生成的建議
    UE_LOG(LogTemp, Log, TEXT("新建議: %s"), *Recommendation.Title);
    
    // 可以添加到 UI 或執行相應邏輯
    DisplayRecommendationToUI(Recommendation);
}

UFUNCTION()
void AMyGameMode::OnRecommendationAccepted(const FRecommendationData& Recommendation)
{
    // 處理玩家接受的建議
    UE_LOG(LogTemp, Log, TEXT("建議被接受: %s"), *Recommendation.Title);
    
    // 執行建議的後果
    ApplyRecommendationConsequences(Recommendation);
}
```

### 3. 定期生成建議

```cpp
void AMyGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 每隔一段時間生成建議
    static float LastRecommendationTime = 0.0f;
    LastRecommendationTime += DeltaTime;
    
    if (LastRecommendationTime >= 30.0f) // 30秒
    {
        FString CurrentContext = GetCurrentGameContext();
        RecommendationSystem->GenerateRecommendations(CurrentContext);
        LastRecommendationTime = 0.0f;
    }
}
```

### 4. UI 集成

```cpp
// 顯示建議到 UI
void AMyGameMode::DisplayRecommendationToUI(const FRecommendationData& Recommendation)
{
    // 獲取 UI 系統
    UMyGameUI* GameUI = GetGameUI();
    if (GameUI)
    {
        // 添加建議到 UI
        GameUI->AddRecommendation(Recommendation);
    }
}
```

## 配置建議

### 基礎配置

```cpp
FRecommendationConfig Config;
Config.MaxActiveRecommendations = 10;
Config.UpdateInterval = 30.0f;
Config.ExpiryHours = 24.0f;
Config.MinConfidenceThreshold = 0.3f;
Config.EnableMachineLearning = true;
Config.EnablePersonalization = true;

RecommendationSystem->SetRecommendationConfig(Config);
```

### 個人化設置

```cpp
TMap<FString, float> Preferences;
Preferences.Add(TEXT("Strategic"), 0.8f);
Preferences.Add(TEXT("Tactical"), 0.6f);
Preferences.Add(TEXT("Diplomatic"), 0.4f);

RecommendationSystem->SetPersonalizationPreferences(Preferences);
```

## 最佳實踐

### 1. 性能優化
- 避免在 Tick 中頻繁生成建議
- 使用異步處理大量建議
- 定期清理過期建議

### 2. 用戶體驗
- 提供清晰的建議說明
- 允許玩家提供反饋
- 記住玩家偏好

### 3. 系統穩定性
- 處理系統初始化失敗
- 提供降級方案
- 記錄和分析錯誤

## 故障排除

### 常見問題

1. **建議未生成**
   - 檢查系統是否正確初始化
   - 確認配置參數是否合理
   - 查看日誌錯誤信息

2. **建議質量不佳**
   - 調整可信度和相關性閾值
   - 檢查 AI 分析權重設置
   - 增加訓練數據

3. **性能問題**
   - 減少最大建議數量
   - 增加更新間隔
   - 啟用建議過濾

## 擴展開發

### 自定義建議類型
```cpp
// 添加新的建議類型
enum class ERecommendationType : uint8
{
    // 現有類型...
    CustomType     UMETA(DisplayName = "自定義類型")
};
```

### 自定義分析引擎
```cpp
// 繼承並擴展分析功能
class UMyCustomRecommendationEngine : public UMingIntelligentRecommendationSystem
{
    // 實現自定義分析邏輯
};
```

通過遵循此指南，您可以成功集成智能建議系統到您的遊戲中。
"@

$IntegrationFile = Join-Path $OutputPath "IntegrationGuide.md"
$IntegrationGuide | Out-File -FilePath $IntegrationFile -Encoding UTF8
Write-Host "✓ 集成指南已保存到: $IntegrationFile" -ForegroundColor Green

# 生成摘要報告
Write-Host "`n=== 生成摘要 ===" -ForegroundColor Yellow
Write-Host "總建議數: $($GeneratedRecommendations.Count)" -ForegroundColor White
Write-Host "平均影響分數: $($Statistics.AverageImpact)" -ForegroundColor White
Write-Host "平均可信度: $($Statistics.AverageConfidence)%" -ForegroundColor White
Write-Host "平均難度: $($Statistics.AverageDifficulty)" -ForegroundColor White

Write-Host "`n=== 按類型統計 ===" -ForegroundColor Yellow
foreach ($Type in $Statistics.ByType.Keys) {
    Write-Host "$Type : $($Statistics.ByType[$Type])" -ForegroundColor Gray
}

Write-Host "`n=== 按來源統計 ===" -ForegroundColor Yellow
foreach ($Source in $Statistics.BySource.Keys) {
    Write-Host "$Source : $($Statistics.BySource[$Source])" -ForegroundColor Gray
}

Write-Host "`n=== 按情境統計 ===" -ForegroundColor Yellow
foreach ($Context in $Statistics.ByContext.Keys) {
    Write-Host "$Context : $($Statistics.ByContext[$Context])" -ForegroundColor Gray
}

Write-Host "`n=== 生成文件 ===" -ForegroundColor Yellow
Write-Host "建議數據: $RecommendationsFile" -ForegroundColor Gray
Write-Host "統計報告: $StatisticsFile" -ForegroundColor Gray
Write-Host "系統配置: $ConfigFile" -ForegroundColor Gray
Write-Host "使用示例: $ExampleFile" -ForegroundColor Gray
Write-Host "集成指南: $IntegrationFile" -ForegroundColor Gray

Write-Host "`n=== 使用建議 ===" -ForegroundColor Yellow
Write-Host "1. 將生成的配置文件集成到 UE5 項目中" -ForegroundColor White
Write-Host "2. 參考使用示例進行系統集成" -ForegroundColor White
Write-Host "3. 根據集成指南進行詳細配置" -ForegroundColor White
Write-Host "4. 測試建議系統的各項功能" -ForegroundColor White

Write-Host "`n✓ 智能建議系統生成完成！" -ForegroundColor Green
Write-Host "祝您開發順利！" -ForegroundColor Cyan

# 可選：打開輸出目錄
$OpenFolder = Read-Host "`n是否打開輸出目錄？(Y/N)"
if ($OpenFolder -eq "Y" -or $OpenFolder -eq "y") {
    Start-Process "explorer.exe" -ArgumentList $OutputPath
}
