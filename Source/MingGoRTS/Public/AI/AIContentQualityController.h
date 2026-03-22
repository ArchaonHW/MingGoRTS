#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIContentQualityController.generated.h"

UENUM(BlueprintType)
enum class EAIContentQualityLevel: uint8 {
    VeryLow     UMETA(DisplayName = "Very Low"),
    Low         UMETA(DisplayName = "Low"),
    Medium      UMETA(DisplayName = "Medium"),
    High        UMETA(DisplayName = "High"),
    VeryHigh    UMETA(DisplayName = "Very High"),
    Excellent   UMETA(DisplayName = "Excellent")
};

UENUM(BlueprintType)
enum class EAIContentType: uint8 {
    Image       UMETA(DisplayName = "Image"),
    Video       UMETA(DisplayName = "Video"),
    Audio       UMETA(DisplayName = "Audio"),
    Text        UMETA(DisplayName = "Text"),
    Model       UMETA(DisplayName = "3D Model"),
    Texture     UMETA(DisplayName = "Texture"),
    Animation   UMETA(DisplayName = "Animation"),
    Material    UMETA(DisplayName = "Material"),
    Effect      UMETA(DisplayName = "Visual Effect"),
    Music       UMETA(DisplayName = "Music"),
    Voice       UMETA(DisplayName = "Voice")
};

UENUM(BlueprintType)
enum class EAIProvider: uint8 {
    StableDiffusion  UMETA(DisplayName = "Stable Diffusion"),
    DALLE           UMETA(DisplayName = "DALL-E"),
    Midjourney       UMETA(DisplayName = "Midjourney"),
    AIVA             UMETA(DisplayName = "AIVA"),
    ElevenLabs       UMETA(DisplayName = "ElevenLabs"),
    OpenAI           UMETA(DisplayName = "OpenAI"),
    LocalModel       UMETA(DisplayName = "Local Model"),
    CustomProvider   UMETA(DisplayName = "Custom Provider")
};

USTRUCT(BlueprintType)
struct FAIContentQualityMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float TechnicalQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float AestheticQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float RelevanceScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float ConsistencyScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float OriginalityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    float OverallQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    EAIContentQualityLevel QualityLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    FDateTime EvaluationTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Metrics")
    TArray<FString> QualityIssues;
};

USTRUCT(BlueprintType)
struct FAIProviderConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider Config")
    EAIProvider Provider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider Config")
    FString APIEndpoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider Config")
    FString APIKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider Config")
    int32 MaxRequestsPerMinute;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider Config")
    float CostPerRequest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider Config")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider Config")
    float ReliabilityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider Config")
    TArray<EAIContentType> SupportedContentTypes;
};

USTRUCT(BlueprintType)
struct FAIContentRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Request")
    FString Prompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Request")
    EAIContentType ContentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Request")
    EAIContentQualityLevel TargetQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Request")
    TArray<EAIProvider> PreferredProviders;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Request")
    bool bRequireHighQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Request")
    FString RequestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Request")
    FDateTime RequestTime;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQualityEvaluationCompleted, const FString&, ContentID, FAIContentQualityMetrics, QualityMetrics);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProviderStatusChanged, EAIProvider, Provider, bool, bIsAvailable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCostThresholdExceeded, float, CurrentCost);

/**
 * AI內容質量控制器
 * 負責監控和評估AI生成內容的質量，管理多供應商策略
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UAIContentQualityController : public UObject
{
    GENERATED_BODY()

public:
    UAIContentQualityController();

    // 初始化質量控制器
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void InitializeQualityController();

    // 評估內容質量
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    FAIContentQualityMetrics EvaluateContentQuality(const FString& ContentPath, EAIContentType ContentType);

    // 異步評估內容質量
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void EvaluateContentQualityAsync(const FString& ContentPath, EAIContentType ContentType, const FString& RequestID);

    // 獲取推薦的AI供應商
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    TArray<EAIProvider> GetRecommendedProviders(EAIContentType ContentType, EAIContentQualityLevel TargetQuality);

    // 註冊AI供應商
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void RegisterAIProvider(const FAIProviderConfig& ProviderConfig);

    // 獲取供應商配置
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    FAIProviderConfig GetProviderConfig(EAIProvider Provider) const;

    // 檢查供應商可用性
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    bool IsProviderAvailable(EAIProvider Provider) const;

    // 設置供應商狀態
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void SetProviderStatus(EAIProvider Provider, bool bIsAvailable);

    // 獲取當前成本
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    float GetCurrentCost() const { return CurrentCost; }

    // 設置成本閾值
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void SetCostThreshold(float Threshold) { CostThreshold = Threshold; }

    // 獲取成本閾值
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    float GetCostThreshold() const { return CostThreshold; }

    // 記錄成本
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void RecordCost(EAIProvider Provider, float Cost);

    // 獲取供應商統計
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    TMap<EAIProvider, float> GetProviderStatistics() const;

    // 獲取質量趨勢
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    TArray<float> GetQualityTrend(EAIContentType ContentType, int32 Days = 7) const;

    // 設置質量標準
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    void SetQualityStandards(EAIContentType ContentType, EAIContentQualityLevel MinQuality);

    // 獲取質量標準
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    EAIContentQualityLevel GetQualityStandards(EAIContentType ContentType) const;

    // 生成質量報告
    UFUNCTION(BlueprintCallable, Category = "AI Content Quality")
    FString GenerateQualityReport() const;

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "AI Content Quality")
    FOnQualityEvaluationCompleted OnQualityEvaluationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Content Quality")
    FOnProviderStatusChanged OnProviderStatusChanged;

    UPROPERTY(BlueprintAssignable, Category = "AI Content Quality")
    FOnCostThresholdExceeded OnCostThresholdExceeded;

protected:
    // 計算質量分數
    float CalculateQualityScore(const FAIContentQualityMetrics& Metrics) const;

    // 檢測質量問題
    TArray<FString> DetectQualityIssues(const FString& ContentPath, EAIContentType ContentType) const;

    // 更新供應商統計
    void UpdateProviderStatistics(EAIProvider Provider, float Quality, float Cost);

    // 檢查成本閾值
    void CheckCostThreshold();

    // 獲取最佳供應商
    EAIProvider GetBestProvider(EAIContentType ContentType, EAIContentQualityLevel TargetQuality) const;

protected:
    // 供應商配置映射
    UPROPERTY(BlueprintReadOnly, Category = "AI Content Quality")
    TMap<EAIProvider, FAIProviderConfig> ProviderConfigs;

    // 供應商可用性狀態
    UPROPERTY(BlueprintReadOnly, Category = "AI Content Quality")
    TMap<EAIProvider, bool> ProviderAvailability;

    // 質量標準映射
    UPROPERTY(BlueprintReadOnly, Category = "AI Content Quality")
    TMap<EAIContentType, EAIContentQualityLevel> QualityStandards;

    // 供應商統計數據
    UPROPERTY(BlueprintReadOnly, Category = "AI Content Quality")
    TMap<EAIProvider, TArray<float>> ProviderQualityHistory;

    UPROPERTY(BlueprintReadOnly, Category = "AI Content Quality")
    TMap<EAIProvider, TArray<float>> ProviderCostHistory;

    // 當前總成本
    UPROPERTY(BlueprintReadOnly, Category = "AI Content Quality")
    float CurrentCost;

    // 成本閾值
    UPROPERTY(BlueprintReadOnly, Category = "AI Content Quality")
    float CostThreshold;

    // 質量評估歷史
    UPROPERTY(BlueprintReadOnly, Category = "AI Content Quality")
    TArray<FAIContentQualityMetrics> QualityHistory;
};
