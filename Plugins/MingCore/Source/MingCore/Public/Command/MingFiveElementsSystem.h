#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingFiveElementsSystem.generated.h"

UENUM(BlueprintType)
enum class EFiveElementsPhase: uint8 {
    Wood,           // 木：立名與正當性
    Fire,           // 火：造勢與加速
    Earth,          // 土：收權與止亂
    Metal,          // 金：裁斷與清除
    Water           // 水：留白與不動
};

UENUM(BlueprintType)
enum class EFiveElementsRelation: uint8 {
    Generating,     // 相生
    Overcoming,     // 相剋
    Insulting,      // 相侮
    Supporting      // 扶助
};

USTRUCT(BlueprintType)
struct FFiveElementsState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFiveElementsPhase CurrentPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFiveElementsPhase NextPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PhaseProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TransitionThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsTransitioning;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EFiveElementsPhase> CompletedPhases;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CycleCount;

    FFiveElementsState()
    {
        CurrentPhase = EFiveElementsPhase::Wood;
        NextPhase = EFiveElementsPhase::Fire;
        PhaseProgress = 0.0f;
        TransitionThreshold = 80.0f;
        bIsTransitioning = false;
        CycleCount = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FFiveElementsStrategy
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFiveElementsPhase TargetPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StrategyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsAvailable;

    FFiveElementsStrategy()
    {
        TargetPhase = EFiveElementsPhase::Wood;
        StrategyName = TEXT("");
        Description = TEXT("");
        ExecutionTime = 0.0f;
        SuccessRate = 0.0f;
        bIsAvailable = true;
    }
};

USTRUCT(BlueprintType)
struct FFiveElementsRhythm
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WoodRhythm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRhythm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EarthRhythm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MetalRhythm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WaterRhythm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallRhythm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHarmonious;

    FFiveElementsRhythm()
    {
        WoodRhythm = 1.0f;
        FireRhythm = 1.0f;
        EarthRhythm = 1.0f;
        MetalRhythm = 1.0f;
        WaterRhythm = 1.0f;
        OverallRhythm = 1.0f;
        bIsHarmonious = true;
    }
};

/**
 * 五行系統
 * 
 * 核心理念：
 * 1. 五行不是屬性，而是指揮節奏
 * 2. 木：立名與正當性
 * 3. 火：造勢與加速
 * 4. 土：收權與止亂
 * 5. 金：裁斷與清除
 * 6. 水：留白與不動
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingFiveElementsSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingFiveElementsSystem();

    // 初始化五行系統
    UFUNCTION(BlueprintCallable)
    void InitializeFiveElementsSystem();

    // 執行五行輪轉
    UFUNCTION(BlueprintCallable)
    FFiveElementsState ExecuteRotation();

    // 獲取當前階段策略
    UFUNCTION(BlueprintCallable)
    TArray<FFiveElementsStrategy> GetCurrentPhaseStrategies();

    // 檢查五行關係
    UFUNCTION(BlueprintCallable)
    EFiveElementsRelation CheckElementsRelation(EFiveElementsPhase Element1, EFiveElementsPhase Element2);

    // 計算五行節奏
    UFUNCTION(BlueprintCallable)
    FFiveElementsRhythm CalculateRhythm();

    // 檢查是否可以轉換到下一階段
    UFUNCTION(BlueprintCallable)
    bool CanTransitionToNext();

    // 執行階段轉換
    UFUNCTION(BlueprintCallable)
    bool ExecutePhaseTransition();

    // 獲取推薦的下一階段
    UFUNCTION(BlueprintCallable)
    EFiveElementsPhase GetRecommendedNextPhase();

    // 檢查五行失序
    UFUNCTION(BlueprintCallable)
    bool IsElementsOrderDisrupted();

    // 修復五行秩序
    UFUNCTION(BlueprintCallable)
    bool RestoreElementsOrder();

    // 獲取當前五行狀態
    UFUNCTION(BlueprintCallable)
    FFiveElementsState GetCurrentState() const { return CurrentState; }

    // 設置五行狀態
    UFUNCTION(BlueprintCallable)
    void SetCurrentState(const FFiveElementsState& NewState) { CurrentState = NewState; }

    // 獲取階段名稱
    UFUNCTION(BlueprintCallable)
    FString GetPhaseName(EFiveElementsPhase Phase);

    // 獲取階段描述
    UFUNCTION(BlueprintCallable)
    FString GetPhaseDescription(EFiveElementsPhase Phase);

protected:
    // 當前五行狀態
    UPROPERTY(BlueprintReadOnly)
    FFiveElementsState CurrentState;

    // 五行策略映射
    UPROPERTY(BlueprintReadOnly)
    TMap<EFiveElementsPhase, TArray<FFiveElementsStrategy>> PhaseStrategies;

    // 五行節奏狀態
    UPROPERTY(BlueprintReadOnly)
    FFiveElementsRhythm CurrentRhythm;

    // 輪轉速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RotationSpeed;

    // 轉換閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TransitionThreshold;

    // 節奏閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RhythmThreshold;

private:
    // 內部輔助函數
    void InitializePhaseStrategies();
    void UpdateRhythm();
    bool IsTransitionValid(EFiveElementsPhase From, EFiveElementsPhase To);
    void RecordPhaseTransition(EFiveElementsPhase From, EFiveElementsPhase To);
    float CalculatePhaseEfficiency(EFiveElementsPhase Phase);
};
