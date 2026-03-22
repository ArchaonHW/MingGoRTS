#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingFiveElementsRotation.generated.h"

// 輪轉模式枚舉
UENUM(BlueprintType)
enum class ERotationMode: uint8 {
    Natural = 0,       // 自然輪轉 - 按五行相生順序
    Strategic = 1,     // 戰略輪轉 - 基於戰略需求
    Defensive = 2,     // 防禦輪轉 - 優先防禦元素
    Offensive = 3,     // 攻擊輪轉 - 優先攻擊元素
    Adaptive = 4,      // 自適應輪轉 - 根據情況調整
    Manual = 5         // 手動輪轉 - 手動控制
};

// 正邪屬性枚舉
UENUM(BlueprintType)
enum class EAlignmentAttribute: uint8 {
    Righteous = 0,     // 正義 - 堅守正道
    Neutral = 1,       // 中立 - 保持平衡
    Evil = 2,          // 邪惡 - 使用邪術
    Chaotic = 3,       // 混亂 - 無序變化
    Orderly = 4        // 秩序 - 嚴格規律
};

// 輪轉狀態枚舉
UENUM(BlueprintType)
enum class ERotationState: uint8 {
    Idle = 0,          // 閒置狀態
    Rotating = 1,      // 輪轉中
    Transitioning = 2, // 轉換中
    Stabilizing = 3,   // 穩定化
    Locked = 4         // 鎖定狀態
};

// 輪轉效果結構
USTRUCT(BlueprintType)
struct FRotationEffect
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EFiveElements FromElement = EFiveElements::Earth;

    UPROPERTY(BlueprintReadOnly)
    EFiveElements ToElement = EFiveElements::Metal;

    UPROPERTY(BlueprintReadOnly)
    EAlignmentAttribute Alignment = EAlignmentAttribute::Righteous;

    UPROPERTY(BlueprintReadOnly)
    float PowerBonus = 0.0f;           // 能量加成 (0-200)

    UPROPERTY(BlueprintReadOnly)
    float Duration = 0.0f;             // 持續時間 (秒)

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedUnits;     // 影響單位

    UPROPERTY(BlueprintReadOnly)
    FString EffectDescription;         // 效果描述
};

// 輪轉配置
USTRUCT(BlueprintType)
struct FRotationConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ERotationMode Mode = ERotationMode::Natural;

    UPROPERTY(BlueprintReadOnly)
    EAlignmentAttribute DefaultAlignment = EAlignmentAttribute::Righteous;

    UPROPERTY(BlueprintReadOnly)
    float RotationSpeed = 1.0f;        // 輪轉速度 (0.1-5.0)

    UPROPERTY(BlueprintReadOnly)
    float TransitionDuration = 5.0f;    // 轉換持續時間 (秒)

    UPROPERTY(BlueprintReadOnly)
    bool bAutoRotate = true;           // 自動輪轉

    UPROPERTY(BlueprintReadOnly)
    bool bAllowEvilRotation = false;   // 允許邪惡輪轉

    UPROPERTY(BlueprintReadOnly)
    float EvilThreshold = 70.0f;       // 邪惡閾值
};

// 輪轉事件
USTRUCT(BlueprintType)
struct FRotationEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    EFiveElements PreviousElement;

    UPROPERTY(BlueprintReadOnly)
    EFiveElements CurrentElement;

    UPROPERTY(BlueprintReadOnly)
    EAlignmentAttribute Alignment;

    UPROPERTY(BlueprintReadOnly)
    ERotationState RotationState;

    UPROPERTY(BlueprintReadOnly)
    float ImpactLevel = 0.0f;           // 影響程度 (0-100)

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnElementRotated, EFiveElements, NewElement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlignmentChanged, EAlignmentAttribute, NewAlignment);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotationEffectApplied, const FRotationEffect&, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotationEventOccurred, const FRotationEvent&, Event);

/**
 * 五行輪轉系統
 * 管理五行元素的輪轉和正邪屬性的變化
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (SageCommand))
class MINGGORTS_API UMingFiveElementsRotation : public UObject
{
    GENERATED_BODY()

public:
    UMingFiveElementsRotation();

    // 系統初始化和清理
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool Initialize();

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    void Cleanup();

    // 輪轉控制
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool StartRotation(ERotationMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool StopRotation();

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool PauseRotation();

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool ResumeRotation();

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    ERotationState GetRotationState() const;

    // 元素輪轉
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool RotateToElement(EFiveElements TargetElement);

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    EFiveElements GetCurrentElement() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    EFiveElements GetNextElement() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    EFiveElements GetPreviousElement() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool SetRotationOrder(const TArray<EFiveElements>& Order);

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    TArray<EFiveElements> GetRotationOrder() const;

    // 正邪屬性管理
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    EAlignmentAttribute GetCurrentAlignment() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool SetAlignment(EAlignmentAttribute NewAlignment);

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool IsAlignmentRighteous() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    float GetAlignmentPurity() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool PurifyAlignment();

    // 輪轉效果
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    FRotationEffect GetCurrentRotationEffect() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    TArray<FRotationEffect> GetActiveEffects() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool ApplyRotationEffect(const FRotationEffect& Effect);

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool RemoveRotationEffect(const FString& EffectID);

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    void ClearAllEffects();

    // 輪轉配置
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    FRotationConfiguration GetRotationConfiguration() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool SetRotationConfiguration(const FRotationConfiguration& Config);

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool SetRotationSpeed(float Speed);

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    float GetRotationSpeed() const;

    // 防墮機制
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool IsFallPreventionActive() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool ActivateFallPrevention();

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool DeactivateFallPrevention();

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    float GetFallRisk() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool PreventEvilRotation();

    // 戰略應用
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    EFiveElements GetOptimalElementForSituation(const FString& Situation) const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    EAlignmentAttribute GetOptimalAlignmentForSituation(const FString& Situation) const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    TArray<FString> GetStrategicRecommendations() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    float CalculateStrategicAdvantage(EFiveElements Element, EAlignmentAttribute Alignment) const;

    // 歷史記錄
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    TArray<FRotationEvent> GetRotationHistory() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    FRotationEvent GetLastRotationEvent() const;

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    void ClearRotationHistory();

    // 系統狀態
    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    bool IsSystemActive() const { return bSystemActive; }

    UFUNCTION(BlueprintCallable, Category = "Five Elements Rotation")
    float GetSystemStability() const { return SystemStability; }

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnElementRotated OnElementRotated;

    UPROPERTY(BlueprintAssignable)
    FOnAlignmentChanged OnAlignmentChanged;

    UPROPERTY(BlueprintAssignable)
    FOnRotationEffectApplied OnRotationEffectApplied;

    UPROPERTY(BlueprintAssignable)
    FOnRotationEventOccurred OnRotationEventOccurred;

protected:
    // 系統狀態
    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    bool bSystemActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    float SystemStability = 100.0f;

    // 輪轉狀態
    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    ERotationState CurrentRotationState = ERotationState::Idle;

    // 當前元素和屬性
    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    EFiveElements CurrentElement = EFiveElements::Earth;

    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    EAlignmentAttribute CurrentAlignment = EAlignmentAttribute::Righteous;

    // 輪轉配置
    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    FRotationConfiguration RotationConfig;

    // 輪轉順序
    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    TArray<EFiveElements> RotationOrder;

    // 活動效果
    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    TArray<FRotationEffect> ActiveEffects;

    // 輪轉歷史
    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    TArray<FRotationEvent> RotationHistory;

    // 防墮機制
    UPROPERTY(BlueprintReadOnly, Category = "Five Elements Rotation")
    bool bFallPreventionActive = true;

private:
    // 輪轉邏輯
    void ExecuteRotation();
    void TransitionToElement(EFiveElements TargetElement);
    void CompleteRotation();

    // 自然輪轉
    void ExecuteNaturalRotation();

    // 戰略輪轉
    void ExecuteStrategicRotation();

    // 防禦輪轉
    void ExecuteDefensiveRotation();

    // 攻擊輪轉
    void ExecuteOffensiveRotation();

    // 自適應輪轉
    void ExecuteAdaptiveRotation();

    // 正邪屬性管理
    void UpdateAlignment();
    void CheckAlignmentPurity();
    void ApplyAlignmentEffects();

    // 防墮機制
    bool CheckFallRisk();
    void ApplyFallPrevention();
    void HandleEvilRotation();

    // 效果管理
    void ApplyRotationEffects();
    void UpdateActiveEffects(float DeltaTime);
    void RemoveExpiredEffects();

    // 事件記錄
    void RecordRotationEvent(const FString& Description, EFiveElements PreviousElement, EFiveElements CurrentElement, EAlignmentAttribute Alignment, ERotationState State, float Impact);

    // 輔助方法
    FString GetElementName(EFiveElements Element) const;
    FString GetAlignmentName(EAlignmentAttribute Alignment) const;
    FString GetRotationModeName(ERotationMode Mode) const;
    FString GetStateName(ERotationState State) const;

    // 五行相生相克計算
    EFiveElements GetGeneratingElement(EFiveElements Element) const;
    EFiveElements GetOvercomingElement(EFiveElements Element) const;
    bool IsGeneratingRelationship(EFiveElements Source, EFiveElements Target) const;
    bool IsOvercomingRelationship(EFiveElements Source, EFiveElements Target) const;

    // 戰略分析
    float AnalyzeSituationRequirements(const FString& Situation) const;
    EFiveElements SelectOptimalElement(const TArray<EFiveElements>& Candidates) const;
    EAlignmentAttribute SelectOptimalAlignment(const TArray<EAlignmentAttribute>& Candidates) const;

    // 配置參數
    UPROPERTY(Config)
    float DefaultRotationSpeed = 1.0f;

    UPROPERTY(Config)
    float AlignmentDecayRate = 0.1f;

    UPROPERTY(Config)
    float EffectDurationMultiplier = 1.0f;

    UPROPERTY(Config)
    float FallPreventionThreshold = 60.0f;

    // 定時器句柄
    FTimerHandle RotationTimerHandle;
    FTimerHandle EffectUpdateTimerHandle;
    FTimerHandle AlignmentUpdateTimerHandle;
};
