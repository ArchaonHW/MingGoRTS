#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCorruptionDetectionSystem.generated.h"

// 墮落徵象詳細類型
UENUM(BlueprintType)
enum class ECorruptionSignDetail : uint8
{
    None                UMETA(DisplayName = "無"),
    // 道德淪喪徵象
    MoralDecay_Light    UMETA(DisplayName = "輕微道德滑坡"),
    MoralDecay_Medium   UMETA(DisplayName = "中度道德淪喪"),
    MoralDecay_Severe   UMETA(DisplayName = "嚴重道德敗壞"),
    // 權力濫用徵象
    PowerAbuse_Light    UMETA(DisplayName = "輕微權力不當"),
    PowerAbuse_Medium   UMETA(DisplayName = "中度權力濫用"),
    PowerAbuse_Severe   UMETA(DisplayName = "嚴重權力腐敗"),
    // 貪慾增長徵象
    GreedGrowth_Light   UMETA(DisplayName = "輕微貪念"),
    GreedGrowth_Medium  UMETA(DisplayName = "中度貪慾"),
    GreedGrowth_Severe  UMETA(DisplayName = "嚴重貪婪")
};

// 徵象嚴重程度
UENUM(BlueprintType)
enum class ECorruptionSeverity : uint8
{
    None        UMETA(DisplayName = "無"),
    Light       UMETA(DisplayName = "輕微"),
    Medium      UMETA(DisplayName = "中等"),
    Severe      UMETA(DisplayName = "嚴重"),
    Critical    UMETA(DisplayName = "危急")
};

// 徵象檢測數據
USTRUCT(BlueprintType)
struct FCorruptionSignData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECorruptionSignDetail SignType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECorruptionSeverity Severity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DetectionValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThresholdValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString WarningMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeDetected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive;

    FCorruptionSignData()
    {
        SignType = ECorruptionSignDetail::None;
        Severity = ECorruptionSeverity::None;
        DetectionValue = 0.0f;
        ThresholdValue = 50.0f;
        TimeDetected = 0.0f;
        bIsActive = false;
    }
};

// 檢測結果
USTRUCT(BlueprintType)
struct FDetectionResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDetected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECorruptionSignDetail DetectedSign;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECorruptionSeverity Severity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConfidenceLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AnalysisReport;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RecommendedActions;

    FDetectionResult()
    {
        bDetected = false;
        DetectedSign = ECorruptionSignDetail::None;
        Severity = ECorruptionSeverity::None;
        ConfidenceLevel = 0.0f;
    }
};

// 預防措施
USTRUCT(BlueprintType)
struct FPreventionMeasure
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MeasureName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Effectiveness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive;

    FPreventionMeasure()
    {
        Effectiveness = 0.5f;
        Cost = 100.0f;
        bIsActive = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCorruptionSignDetected, const FCorruptionSignData&, SignData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeverityLevelChanged, ECorruptionSeverity, NewSeverity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPreventionMeasureTriggered, const FPreventionMeasure&, Measure);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingCorruptionDetectionSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingCorruptionDetectionSystem();

    // 初始化檢測系統
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    void InitializeDetectionSystem();

    // 檢測道德淪喪徵象
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection|Moral Decay")
    FDetectionResult DetectMoralDecay(float CurrentMorality, float HistoricalMorality);

    // 檢測權力濫用徵象
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection|Power Abuse")
    FDetectionResult DetectPowerAbuse(float PowerUsage, float AuthorizedLimit);

    // 檢測貪慾增長徵象
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection|Greed Growth")
    FDetectionResult DetectGreedGrowth(float ResourceDesire, float ActualNeed);

    // 綜合檢測
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    TArray<FDetectionResult> PerformFullDetection();

    // 獲取徵象數據
    UFUNCTION(BlueprintPure, Category = "Corruption Detection")
    FCorruptionSignData GetSignData(ECorruptionSignDetail SignType) const;

    // 更新檢測閾值
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    void UpdateDetectionThreshold(ECorruptionSignDetail SignType, float NewThreshold);

    // 應用預防措施
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    bool ApplyPreventionMeasure(const FPreventionMeasure& Measure);

    // 獲取建議措施
    UFUNCTION(BlueprintPure, Category = "Corruption Detection")
    TArray<FPreventionMeasure> GetRecommendedMeasures(ECorruptionSeverity Severity) const;

    // 計算綜合墮落風險
    UFUNCTION(BlueprintPure, Category = "Corruption Detection")
    float CalculateOverallCorruptionRisk() const;

    // 檢查是否需要緊急干預
    UFUNCTION(BlueprintPure, Category = "Corruption Detection")
    bool RequiresEmergencyIntervention() const;

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Corruption Detection")
    FOnCorruptionSignDetected OnCorruptionSignDetected;

    UPROPERTY(BlueprintAssignable, Category = "Corruption Detection")
    FOnSeverityLevelChanged OnSeverityLevelChanged;

    UPROPERTY(BlueprintAssignable, Category = "Corruption Detection")
    FOnPreventionMeasureTriggered OnPreventionMeasureTriggered;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Corruption Detection")
    void OnDetectionSystemInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Corruption Detection")
    void OnCorruptionAnalysisCompleted(const TArray<FDetectionResult>& Results);

private:
    void InitializeDefaultThresholds();
    void InitializeDefaultMeasures();
    
    ECorruptionSeverity CalculateSeverity(float Value, float Threshold) const;
    float CalculateConfidence(float Value, float Threshold) const;
    FString GenerateAnalysisReport(ECorruptionSignDetail SignType, ECorruptionSeverity Severity) const;
    TArray<FString> GenerateRecommendations(ECorruptionSeverity Severity) const;
    
    bool ValidateDetectionParams(float Value, float Threshold) const;
    void RecordDetection(const FDetectionResult& Result);
    void UpdateSignStatus(ECorruptionSignDetail SignType, const FCorruptionSignData& Data);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption Detection")
    TMap<ECorruptionSignDetail, FCorruptionSignData> SignDatabase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption Detection")
    TMap<ECorruptionSignDetail, float> DetectionThresholds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption Detection")
    TArray<FPreventionMeasure> AvailableMeasures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption Detection")
    TArray<FDetectionResult> DetectionHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption Detection")
    bool bIsInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption Detection")
    float EmergencyThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption Detection")
    ECorruptionSeverity CurrentMaxSeverity;
};
