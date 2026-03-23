#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingWuXingRhythmSystem.generated.h"

// 五行類型
UENUM(BlueprintType)
enum class EWuXingType : uint8
{
    None            UMETA(DisplayName = "無"),
    Wood            UMETA(DisplayName = "木"),
    Fire            UMETA(DisplayName = "火"),
    Earth           UMETA(DisplayName = "土"),
    Metal           UMETA(DisplayName = "金"),
    Water           UMETA(DisplayName = "水")
};

// 五行階段狀態
UENUM(BlueprintType)
enum class EWuXingPhase : uint8
{
    None            UMETA(DisplayName = "無"),
    Birth           UMETA(DisplayName = "生發"),
    Growth          UMETA(DisplayName = "壯大"),
    Stabilization   UMETA(DisplayName = "穩固"),
    Contraction     UMETA(DisplayName = "收斂"),
    Storage         UMETA(DisplayName = "藏匿")
};

// 五行生剋關係
UENUM(BlueprintType)
enum class EWuXingRelation : uint8
{
    None            UMETA(DisplayName = "無"),
    Generate        UMETA(DisplayName = "相生"),
    Restrain        UMETA(DisplayName = "相剋"),
    Overcome        UMETA(DisplayName = "相乘"),
    Insult          UMETA(DisplayName = "相侮")
};

// 五行狀態結構
USTRUCT(BlueprintType)
struct FWuXingStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWuXingType WuXingType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWuXingPhase CurrentPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PhaseProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PowerLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Stability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FWuXingStatus> RelatedElements;

    FWuXingStatus()
    {
        WuXingType = EWuXingType::None;
        CurrentPhase = EWuXingPhase::None;
        PhaseProgress = 0.0f;
        PowerLevel = 50.0f;
        Stability = 0.5f;
        bIsActive = false;
        Duration = 0.0f;
    }
};

// 節奏效果結構
USTRUCT(BlueprintType)
struct FRhythmEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWuXingType SourceElement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWuXingType TargetElement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWuXingRelation RelationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsBeneficial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EffectDescription;

    FRhythmEffect()
    {
        SourceElement = EWuXingType::None;
        TargetElement = EWuXingType::None;
        RelationType = EWuXingRelation::None;
        EffectStrength = 0.0f;
        Duration = 0.0f;
        bIsBeneficial = false;
    }
};

// 五行節奏組合
USTRUCT(BlueprintType)
struct FWuXingRhythmCombination
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EWuXingType> ActiveElements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RhythmHarmony;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsBalanced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FRhythmEffect> ActiveEffects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CombinationName;

    FWuXingRhythmCombination()
    {
        RhythmHarmony = 0.0f;
        bIsBalanced = false;
        TotalPower = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWuXingPhaseChanged, EWuXingType, Element, EWuXingPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRhythmEffectTriggered, const FRhythmEffect&, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWuXingCombinationFormed, const FWuXingRhythmCombination&, Combination);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingWuXingRhythmSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingWuXingRhythmSystem();

    // 初始化五行節奏系統
    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm")
    void InitializeWuXingRhythmSystem();

    // 激活五行元素
    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm")
    void ActivateWuXingElement(EWuXingType ElementType);

    // 停用五行元素
    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm")
    void DeactivateWuXingElement(EWuXingType ElementType);

    // 推進節奏階段
    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm")
    void AdvancePhase(EWuXingType ElementType, float ProgressDelta);

    // 獲取五行狀態
    UFUNCTION(BlueprintPure, Category = "WuXing Rhythm")
    FWuXingStatus GetWuXingStatus(EWuXingType ElementType) const;

    // 設置五行威力
    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm")
    void SetWuXingPower(EWuXingType ElementType, float PowerLevel);

    // 計算五行關係
    UFUNCTION(BlueprintPure, Category = "WuXing Rhythm")
    EWuXingRelation CalculateWuXingRelation(EWuXingType Source, EWuXingType Target) const;

    // 獲取節奏效果
    UFUNCTION(BlueprintPure, Category = "WuXing Rhythm")
    FRhythmEffect GetRhythmEffect(EWuXingType Source, EWuXingType Target) const;

    // 檢查五行組合平衡
    UFUNCTION(BlueprintPure, Category = "WuXing Rhythm")
    bool IsRhythmBalanced(const TArray<EWuXingType>& ActiveElements) const;

    // 形成五行組合
    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm")
    FWuXingRhythmCombination FormWuXingCombination(const TArray<EWuXingType>& Elements);

    // 獲取推薦組合
    UFUNCTION(BlueprintPure, Category = "WuXing Rhythm")
    TArray<FWuXingRhythmCombination> GetRecommendedCombinations() const;

    // 五行階段特定效果
    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm|Phases")
    void ApplyBirthPhaseEffect(EWuXingType Element);

    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm|Phases")
    void ApplyGrowthPhaseEffect(EWuXingType Element);

    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm|Phases")
    void ApplyStabilizationPhaseEffect(EWuXingType Element);

    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm|Phases")
    void ApplyContractionPhaseEffect(EWuXingType Element);

    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm|Phases")
    void ApplyStoragePhaseEffect(EWuXingType Element);

    // 五行生剋應用
    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm|Relations")
    void ApplyGeneratingRelation(EWuXingType Source, EWuXingType Target);

    UFUNCTION(BlueprintCallable, Category = "WuXing Rhythm|Relations")
    void ApplyRestrainingRelation(EWuXingType Source, EWuXingType Target);

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "WuXing Rhythm")
    FOnWuXingPhaseChanged OnWuXingPhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "WuXing Rhythm")
    FOnRhythmEffectTriggered OnRhythmEffectTriggered;

    UPROPERTY(BlueprintAssignable, Category = "WuXing Rhythm")
    FOnWuXingCombinationFormed OnWuXingCombinationFormed;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "WuXing Rhythm")
    void OnWuXingRhythmSystemInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "WuXing Rhythm")
    void OnPhaseTransitionCompleted(EWuXingType Element, EWuXingPhase OldPhase, EWuXingPhase NewPhase);

private:
    void InitializeDefaultWuXingElements();
    void InitializeWuXingRelations();
    float CalculatePhaseEffectiveness(EWuXingType Element, EWuXingPhase Phase) const;
    bool CanTransitionToPhase(EWuXingType Element, EWuXingPhase TargetPhase) const;
    EWuXingPhase GetNextPhase(EWuXingPhase CurrentPhase) const;
    float CalculateHarmonyScore(const TArray<EWuXingType>& Elements) const;
    void UpdateElementRelations();
    void TriggerRhythmEffects();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing Rhythm")
    TMap<EWuXingType, FWuXingStatus> WuXingStatusMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing Rhythm")
    TMap<EWuXingType, TMap<EWuXingType, EWuXingRelation>> WuXingRelationMatrix;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing Rhythm")
    TArray<FWuXingRhythmCombination> ActiveCombinations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing Rhythm")
    bool bIsInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing Rhythm")
    float MaxWuXingPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing Rhythm")
    float PhaseTransitionThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing Rhythm")
    TArray<FRhythmEffect> ActiveRhythmEffects;
};
