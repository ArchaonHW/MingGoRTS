#include "AIContentQualityController.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"

UAIContentQualityController::UAIContentQualityController()
    : CurrentCost(0.0f)
    , CostThreshold(1000.0f) // 默認成本閾值
{
}

void UAIContentQualityController::InitializeQualityController()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing AI Content Quality Controller"));
    
    // 初始化默認質量標準
    QualityStandards.Add(EAIContentType::Image, EAIContentQualityLevel::High);
    QualityStandards.Add(EAIContentType::Video, EAIContentQualityLevel::High);
    QualityStandards.Add(EAIContentType::Audio, EAIContentQualityLevel::Medium);
    QualityStandards.Add(EAIContentType::Text, EAIContentQualityLevel::High);
    QualityStandards.Add(EAIContentType::Model, EAIContentQualityLevel::High);
    QualityStandards.Add(EAIContentType::Texture, EAIContentQualityLevel::High);
    QualityStandards.Add(EAIContentType::Animation, EAIContentQualityLevel::Medium);
    QualityStandards.Add(EAIContentType::Material, EAIContentQualityLevel::Medium);
    QualityStandards.Add(EAIContentType::Effect, EAIContentQualityLevel::Medium);
    QualityStandards.Add(EAIContentType::Music, EAIContentQualityLevel::Medium);
    QualityStandards.Add(EAIContentType::Voice, EAIContentQualityLevel::High);
    
    // 初始化默認供應商配置
    InitializeDefaultProviders();
    
    UE_LOG(LogTemp, Log, TEXT("AI Content Quality Controller initialized successfully"));
}

void UAIContentQualityController::InitializeDefaultProviders()
{
    // Stable Diffusion 配置
    FAIProviderConfig StableDiffusionConfig;
    StableDiffusionConfig.Provider = EAIProvider::StableDiffusion;
    StableDiffusionConfig.APIEndpoint = TEXT("https://api.stability.ai/v1/generation");
    StableDiffusionConfig.MaxRequestsPerMinute = 60;
    StableDiffusionConfig.CostPerRequest = 0.02f;
    StableDiffusionConfig.bIsActive = true;
    StableDiffusionConfig.ReliabilityScore = 0.85f;
    StableDiffusionConfig.SupportedContentTypes.Add(EAIContentType::Image);
    StableDiffusionConfig.SupportedContentTypes.Add(EAIContentType::Texture);
    ProviderConfigs.Add(EAIProvider::StableDiffusion, StableDiffusionConfig);
    ProviderAvailability.Add(EAIProvider::StableDiffusion, true);
    
    // DALL-E 配置
    FAIProviderConfig DALLEConfig;
    DALLEConfig.Provider = EAIProvider::DALLE;
    DALLEConfig.APIEndpoint = TEXT("https://api.openai.com/v1/images/generations");
    DALLEConfig.MaxRequestsPerMinute = 50;
    DALLEConfig.CostPerRequest = 0.08f;
    DALLEConfig.bIsActive = true;
    DALLEConfig.ReliabilityScore = 0.92f;
    DALLEConfig.SupportedContentTypes.Add(EAIContentType::Image);
    ProviderConfigs.Add(EAIProvider::DALLE, DALLEConfig);
    ProviderAvailability.Add(EAIProvider::DALLE, true);
    
    // ElevenLabs 配置
    FAIProviderConfig ElevenLabsConfig;
    ElevenLabsConfig.Provider = EAIProvider::ElevenLabs;
    ElevenLabsConfig.APIEndpoint = TEXT("https://api.elevenlabs.io/v1/text-to-speech");
    ElevenLabsConfig.MaxRequestsPerMinute = 100;
    ElevenLabsConfig.CostPerRequest = 0.05f;
    ElevenLabsConfig.bIsActive = true;
    ElevenLabsConfig.ReliabilityScore = 0.88f;
    ElevenLabsConfig.SupportedContentTypes.Add(EAIContentType::Voice);
    ProviderConfigs.Add(EAIProvider::ElevenLabs, ElevenLabsConfig);
    ProviderAvailability.Add(EAIProvider::ElevenLabs, true);
    
    // AIVA 配置
    FAIProviderConfig AIVAConfig;
    AIVAConfig.Provider = EAIProvider::AIVA;
    AIVAConfig.APIEndpoint = TEXT("https://api.aiva.ai/v1/generate");
    AIVAConfig.MaxRequestsPerMinute = 30;
    AIVAConfig.CostPerRequest = 0.15f;
    AIVAConfig.bIsActive = true;
    AIVAConfig.ReliabilityScore = 0.80f;
    AIVAConfig.SupportedContentTypes.Add(EAIContentType::Music);
    ProviderConfigs.Add(EAIProvider::AIVA, AIVAConfig);
    ProviderAvailability.Add(EAIProvider::AIVA, true);
}

FAIContentQualityMetrics UAIContentQualityController::EvaluateContentQuality(const FString& ContentPath, EAIContentType ContentType)
{
    FAIContentQualityMetrics Metrics;
    Metrics.EvaluationTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("Evaluating content quality for: %s"), *ContentPath);
    
    // 技術質量評估
    Metrics.TechnicalQuality = EvaluateTechnicalQuality(ContentPath, ContentType);
    
    // 美學質量評估
    Metrics.AestheticQuality = EvaluateAestheticQuality(ContentPath, ContentType);
    
    // 相關性評估
    Metrics.RelevanceScore = EvaluateRelevance(ContentPath, ContentType);
    
    // 一致性評估
    Metrics.ConsistencyScore = EvaluateConsistency(ContentPath, ContentType);
    
    // 原創性評估
    Metrics.OriginalityScore = EvaluateOriginality(ContentPath, ContentType);
    
    // 檢測質量問題
    Metrics.QualityIssues = DetectQualityIssues(ContentPath, ContentType);
    
    // 計算總體質量
    Metrics.OverallQuality = CalculateQualityScore(Metrics);
    
    // 確定質量等級
    Metrics.QualityLevel = DetermineQualityLevel(Metrics.OverallQuality);
    
    // 添加到歷史記錄
    QualityHistory.Add(Metrics);
    
    UE_LOG(LogTemp, Log, TEXT("Content quality evaluation completed: %.2f (%s)"), 
        Metrics.OverallQuality, *UEnum::GetDisplayValueAsText(Metrics.QualityLevel).ToString());
    
    return Metrics;
}

void UAIContentQualityController::EvaluateContentQualityAsync(const FString& ContentPath, EAIContentType ContentType, const FString& RequestID)
{
    // 在實際實現中，這裡應該使用異步任務系統
    // 為了演示，我們使用簡單的延遲機制
    AsyncTask(ENamedThreads::AnyBackgroundThread, [this, ContentPath, ContentType, RequestID]()
    {
        // 模擬異步評估過程
        FPlatformProcess::Sleep(1.0f);
        
        FAIContentQualityMetrics Metrics = EvaluateContentQuality(ContentPath, ContentType);
        
        // 在主線程中觸發事件
        AsyncTask(ENamedThreads::GameThread, [this, RequestID, Metrics]()
        {
            OnQualityEvaluationCompleted.Broadcast(RequestID, Metrics);
        });
    });
}

float UAIContentQualityController::EvaluateTechnicalQuality(const FString& ContentPath, EAIContentType ContentType)
{
    // 根據內容類型評估技術質量
    switch (ContentType)
    {
        case EAIContentType::Image:
            return EvaluateImageTechnicalQuality(ContentPath);
        case EAIContentType::Video:
            return EvaluateVideoTechnicalQuality(ContentPath);
        case EAIContentType::Audio:
            return EvaluateAudioTechnicalQuality(ContentPath);
        case EAIContentType::Model:
            return EvaluateModelTechnicalQuality(ContentPath);
        default:
            return 0.75f; // 默認技術質量
    }
}

float UAIContentQualityController::EvaluateImageTechnicalQuality(const FString& ImagePath)
{
    // 圖像技術質量評估邏輯
    // 檢查解析度、壓縮質量、色彩深度等
    float ResolutionScore = 0.8f; // 模擬解析度評分
    float CompressionScore = 0.7f; // 模擬壓縮質量評分
    float ColorScore = 0.85f; // 模擬色彩質量評分
    
    return (ResolutionScore + CompressionScore + ColorScore) / 3.0f;
}

float UAIContentQualityController::EvaluateVideoTechnicalQuality(const FString& VideoPath)
{
    // 視頻技術質量評估邏輯
    float ResolutionScore = 0.75f;
    float FrameRateScore = 0.8f;
    float BitrateScore = 0.7f;
    
    return (ResolutionScore + FrameRateScore + BitrateScore) / 3.0f;
}

float UAIContentQualityController::EvaluateAudioTechnicalQuality(const FString& AudioPath)
{
    // 音頻技術質量評估邏輯
    float SampleRateScore = 0.8f;
    float BitrateScore = 0.75f;
    float NoiseScore = 0.85f;
    
    return (SampleRateScore + BitrateScore + NoiseScore) / 3.0f;
}

float UAIContentQualityController::EvaluateModelTechnicalQuality(const FString& ModelPath)
{
    // 3D模型技術質量評估邏輯
    float TopologyScore = 0.8f;
    float TextureScore = 0.75f;
    float OptimizationScore = 0.7f;
    
    return (TopologyScore + TextureScore + OptimizationScore) / 3.0f;
}

float UAIContentQualityController::EvaluateAestheticQuality(const FString& ContentPath, EAIContentType ContentType)
{
    // 美學質量評估 - 使用AI模型或預定義規則
    // 這裡使用模擬數據
    switch (ContentType)
    {
        case EAIContentType::Image:
            return 0.82f;
        case EAIContentType::Video:
            return 0.78f;
        case EAIContentType::Audio:
            return 0.75f;
        default:
            return 0.70f;
    }
}

float UAIContentQualityController::EvaluateRelevance(const FString& ContentPath, EAIContentType ContentType)
{
    // 相關性評估 - 檢查內容是否符合預期用途
    // 這裡使用模擬數據
    return 0.80f;
}

float UAIContentQualityController::EvaluateConsistency(const FString& ContentPath, EAIContentType ContentType)
{
    // 一致性評估 - 檢查內容內部一致性
    // 這裡使用模擬數據
    return 0.85f;
}

float UAIContentQualityController::EvaluateOriginality(const FString& ContentPath, EAIContentType ContentType)
{
    // 原創性評估 - 檢查內容的獨特性
    // 這裡使用模擬數據
    return 0.75f;
}

EAIContentQualityLevel UAIContentQualityController::DetermineQualityLevel(float OverallScore)
{
    if (OverallScore >= 0.95f) return EAIContentQualityLevel::Excellent;
    if (OverallScore >= 0.85f) return EAIContentQualityLevel::VeryHigh;
    if (OverallScore >= 0.75f) return EAIContentQualityLevel::High;
    if (OverallScore >= 0.65f) return EAIContentQualityLevel::Medium;
    if (OverallScore >= 0.55f) return EAIContentQualityLevel::Low;
    return EAIContentQualityLevel::VeryLow;
}

TArray<EAIProvider> UAIContentQualityController::GetRecommendedProviders(EAIContentType ContentType, EAIContentQualityLevel TargetQuality)
{
    TArray<EAIProvider> RecommendedProviders;
    
    for (const auto& ProviderPair : ProviderConfigs)
    {
        const FAIProviderConfig& Config = ProviderPair.Value;
        
        // 檢查供應商是否支持該內容類型
        if (!Config.SupportedContentTypes.Contains(ContentType))
        {
            continue;
        }
        
        // 檢查供應商是否可用
        if (!Config.bIsActive || !IsProviderAvailable(ProviderPair.Key))
        {
            continue;
        }
        
        // 根據質量要求和可靠性評分推薦
        if (TargetQuality >= EAIContentQualityLevel::High && Config.ReliabilityScore < 0.8f)
        {
            continue;
        }
        
        RecommendedProviders.Add(ProviderPair.Key);
    }
    
    // 按可靠性排序
    RecommendedProviders.Sort([this](const EAIProvider& A, const EAIProvider& B)
    {
        return ProviderConfigs[A].ReliabilityScore > ProviderConfigs[B].ReliabilityScore;
    });
    
    return RecommendedProviders;
}

void UAIContentQualityController::RegisterAIProvider(const FAIProviderConfig& ProviderConfig)
{
    ProviderConfigs.Add(ProviderConfig.Provider, ProviderConfig);
    ProviderAvailability.Add(ProviderConfig.Provider, ProviderConfig.bIsActive);
    
    UE_LOG(LogTemp, Log, TEXT("Registered AI provider: %s"), *UEnum::GetDisplayValueAsText(ProviderConfig.Provider).ToString());
}

FAIProviderConfig UAIContentQualityController::GetProviderConfig(EAIProvider Provider) const
{
    if (ProviderConfigs.Contains(Provider))
    {
        return ProviderConfigs[Provider];
    }
    return FAIProviderConfig();
}

bool UAIContentQualityController::IsProviderAvailable(EAIProvider Provider) const
{
    if (ProviderAvailability.Contains(Provider))
    {
        return ProviderAvailability[Provider];
    }
    return false;
}

void UAIContentQualityController::SetProviderStatus(EAIProvider Provider, bool bIsAvailable)
{
    bool bOldStatus = IsProviderAvailable(Provider);
    ProviderAvailability[Provider] = bIsAvailable;
    
    if (bOldStatus != bIsAvailable)
    {
        UE_LOG(LogTemp, Log, TEXT("Provider %s status changed to: %s"), 
            *UEnum::GetDisplayValueAsText(Provider).ToString(), 
            bIsAvailable ? TEXT("Available") : TEXT("Unavailable"));
        
        OnProviderStatusChanged.Broadcast(Provider, bIsAvailable);
    }
}

void UAIContentQualityController::RecordCost(EAIProvider Provider, float Cost)
{
    CurrentCost += Cost;
    
    // 記錄到成本歷史
    if (!ProviderCostHistory.Contains(Provider))
    {
        ProviderCostHistory.Add(Provider, TArray<float>());
    }
    ProviderCostHistory[Provider].Add(Cost);
    
    // 檢查成本閾值
    CheckCostThreshold();
    
    UE_LOG(LogTemp, Log, TEXT("Recorded cost %.2f for provider %s. Total cost: %.2f"), 
        Cost, *UEnum::GetDisplayValueAsText(Provider).ToString(), CurrentCost);
}

TMap<EAIProvider, float> UAIContentQualityController::GetProviderStatistics() const
{
    TMap<EAIProvider, float> Statistics;
    
    for (const auto& ProviderPair : ProviderConfigs)
    {
        EAIProvider Provider = ProviderPair.Key;
        const FAIProviderConfig& Config = ProviderPair.Value;
        
        // 計算平均質量和成本
        float AvgQuality = 0.0f;
        float AvgCost = 0.0f;
        
        if (ProviderQualityHistory.Contains(Provider) && ProviderQualityHistory[Provider].Num() > 0)
        {
            float Sum = 0.0f;
            for (float Quality : ProviderQualityHistory[Provider])
            {
                Sum += Quality;
            }
            AvgQuality = Sum / ProviderQualityHistory[Provider].Num();
        }
        
        if (ProviderCostHistory.Contains(Provider) && ProviderCostHistory[Provider].Num() > 0)
        {
            float Sum = 0.0f;
            for (float Cost : ProviderCostHistory[Provider])
            {
                Sum += Cost;
            }
            AvgCost = Sum / ProviderCostHistory[Provider].Num();
        }
        
        // 綜合評分 (質量 * 可靠性 / 成本)
        float Score = (AvgQuality * Config.ReliabilityScore) / FMath::Max(AvgCost, 0.01f);
        Statistics.Add(Provider, Score);
    }
    
    return Statistics;
}

TArray<float> UAIContentQualityController::GetQualityTrend(EAIContentType ContentType, int32 Days) const
{
    TArray<float> Trend;
    
    // 從歷史記錄中提取指定天數的質量數據
    FDateTime CutoffTime = FDateTime::Now() - FTimespan::FromDays(Days);
    
    for (const FAIContentQualityMetrics& Metrics : QualityHistory)
    {
        if (Metrics.EvaluationTime >= CutoffTime)
        {
            Trend.Add(Metrics.OverallQuality);
        }
    }
    
    return Trend;
}

void UAIContentQualityController::SetQualityStandards(EAIContentType ContentType, EAIContentQualityLevel MinQuality)
{
    QualityStandards[ContentType] = MinQuality;
    
    UE_LOG(LogTemp, Log, TEXT("Set quality standard for %s: %s"), 
        *UEnum::GetDisplayValueAsText(ContentType).ToString(),
        *UEnum::GetDisplayValueAsText(MinQuality).ToString());
}

EAIContentQualityLevel UAIContentQualityController::GetQualityStandards(EAIContentType ContentType) const
{
    if (QualityStandards.Contains(ContentType))
    {
        return QualityStandards[ContentType];
    }
    return EAIContentQualityLevel::Medium; // 默認標準
}

FString UAIContentQualityController::GenerateQualityReport() const
{
    FString Report;
    Report += TEXT("=== AI內容質量控制報告 ===\n\n");
    
    // 當前狀態
    Report += FString::Printf(TEXT("報告生成時間: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("當前總成本: $%.2f\n"), CurrentCost);
    Report += FString::Printf(TEXT("成本閾值: $%.2f\n\n"), CostThreshold);
    
    // 供應商狀態
    Report += TEXT("=== 供應商狀態 ===\n");
    for (const auto& ProviderPair : ProviderConfigs)
    {
        const FAIProviderConfig& Config = ProviderPair.Value;
        bool bAvailable = IsProviderAvailable(ProviderPair.Key);
        
        Report += FString::Printf(TEXT("%s: %s (可靠性: %.2f, 成本/請求: $%.3f)\n"),
            *UEnum::GetDisplayValueAsText(Config.Provider).ToString(),
            bAvailable ? TEXT("可用") : TEXT("不可用"),
            Config.ReliabilityScore,
            Config.CostPerRequest);
    }
    
    // 質量統計
    Report += TEXT("\n=== 質量統計 ===\n");
    if (QualityHistory.Num() > 0)
    {
        float TotalQuality = 0.0f;
        for (const FAIContentQualityMetrics& Metrics : QualityHistory)
        {
            TotalQuality += Metrics.OverallQuality;
        }
        float AvgQuality = TotalQuality / QualityHistory.Num();
        
        Report += FString::Printf(TEXT("總評估次數: %d\n"), QualityHistory.Num());
        Report += FString::Printf(TEXT("平均質量分數: %.2f\n"), AvgQuality);
        Report += FString::Printf(TEXT("質量等級: %s\n"), *UEnum::GetDisplayValueAsText(DetermineQualityLevel(AvgQuality)).ToString());
    }
    
    // 供應商統計
    Report += TEXT("\n=== 供應商統計 ===\n");
    TMap<EAIProvider, float> Statistics = GetProviderStatistics();
    for (const auto& StatPair : Statistics)
    {
        Report += FString::Printf(TEXT("%s: %.2f\n"), 
            *UEnum::GetDisplayValueAsText(StatPair.Key).ToString(), 
            StatPair.Value);
    }
    
    // 質量標準
    Report += TEXT("\n=== 質量標準 ===\n");
    for (const auto& StandardPair : QualityStandards)
    {
        Report += FString::Printf(TEXT("%s: %s\n"),
            *UEnum::GetDisplayValueAsText(StandardPair.Key).ToString(),
            *UEnum::GetDisplayValueAsText(StandardPair.Value).ToString());
    }
    
    return Report;
}

float UAIContentQualityController::CalculateQualityScore(const FAIContentQualityMetrics& Metrics) const
{
    // 加權計算總體質量分數
    float Weights[] = {0.25f, 0.20f, 0.20f, 0.15f, 0.20f}; // 技術、美學、相關性、一致性、原創性
    float Values[] = {Metrics.TechnicalQuality, Metrics.AestheticQuality, Metrics.RelevanceScore, 
                     Metrics.ConsistencyScore, Metrics.OriginalityScore};
    
    float WeightedSum = 0.0f;
    float TotalWeight = 0.0f;
    
    for (int32 i = 0; i < 5; ++i)
    {
        WeightedSum += Values[i] * Weights[i];
        TotalWeight += Weights[i];
    }
    
    return TotalWeight > 0.0f ? (WeightedSum / TotalWeight) : 0.0f;
}

TArray<FString> UAIContentQualityController::DetectQualityIssues(const FString& ContentPath, EAIContentType ContentType) const
{
    TArray<FString> Issues;
    
    // 檢測常見質量問題
    switch (ContentType)
    {
        case EAIContentType::Image:
            if (EvaluateTechnicalQuality(ContentPath, ContentType) < 0.6f)
            {
                Issues.Add(TEXT("圖像解析度過低"));
            }
            if (EvaluateAestheticQuality(ContentPath, ContentType) < 0.5f)
            {
                Issues.Add(TEXT("圖像美學質量不佳"));
            }
            break;
            
        case EAIContentType::Audio:
            if (EvaluateTechnicalQuality(ContentPath, ContentType) < 0.6f)
            {
                Issues.Add(TEXT("音頻質量過低"));
            }
            break;
            
        default:
            break;
    }
    
    return Issues;
}

void UAIContentQualityController::UpdateProviderStatistics(EAIProvider Provider, float Quality, float Cost)
{
    // 更新質量歷史
    if (!ProviderQualityHistory.Contains(Provider))
    {
        ProviderQualityHistory.Add(Provider, TArray<float>());
    }
    ProviderQualityHistory[Provider].Add(Quality);
    
    // 更新成本歷史
    if (!ProviderCostHistory.Contains(Provider))
    {
        ProviderCostHistory.Add(Provider, TArray<float>());
    }
    ProviderCostHistory[Provider].Add(Cost);
}

void UAIContentQualityController::CheckCostThreshold()
{
    if (CurrentCost > CostThreshold)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cost threshold exceeded: $%.2f > $%.2f"), CurrentCost, CostThreshold);
        OnCostThresholdExceeded.Broadcast(CurrentCost);
    }
}

EAIProvider UAIContentQualityController::GetBestProvider(EAIContentType ContentType, EAIContentQualityLevel TargetQuality) const
{
    TArray<EAIProvider> Recommended = GetRecommendedProviders(ContentType, TargetQuality);
    return Recommended.Num() > 0 ? Recommended[0] : EAIProvider::StableDiffusion;
}
