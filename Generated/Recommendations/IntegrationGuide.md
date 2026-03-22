# 智能建議系統集成指南

## 系統概述

智能建議系統是 MingGoRTS 的核心 AI 組件，提供基於多維度分析的智能建議。

## 集成步驟

### 1. 系統初始化

在遊戲管理器或相關系統中初始化建議系統：

`cpp
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
`

### 2. 事件處理

`cpp
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
`

### 3. 定期生成建議

`cpp
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
`

### 4. UI 集成

`cpp
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
`

## 配置建議

### 基礎配置

`cpp
FRecommendationConfig Config;
Config.MaxActiveRecommendations = 10;
Config.UpdateInterval = 30.0f;
Config.ExpiryHours = 24.0f;
Config.MinConfidenceThreshold = 0.3f;
Config.EnableMachineLearning = true;
Config.EnablePersonalization = true;

RecommendationSystem->SetRecommendationConfig(Config);
`

### 個人化設置

`cpp
TMap<FString, float> Preferences;
Preferences.Add(TEXT("Strategic"), 0.8f);
Preferences.Add(TEXT("Tactical"), 0.6f);
Preferences.Add(TEXT("Diplomatic"), 0.4f);

RecommendationSystem->SetPersonalizationPreferences(Preferences);
`

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
`cpp
// 添加新的建議類型
enum class ERecommendationType : uint8
{
    // 現有類型...
    CustomType     UMETA(DisplayName = "自定義類型")
};
`

### 自定義分析引擎
`cpp
// 繼承並擴展分析功能
class UMyCustomRecommendationEngine : public UMingIntelligentRecommendationSystem
{
    // 實現自定義分析邏輯
};
`

通過遵循此指南，您可以成功集成智能建議系統到您的遊戲中。
