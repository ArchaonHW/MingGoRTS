#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingConsequenceConflictResolver.generated.h"

// 後果衝突類型
UENUM(BlueprintType)
enum class EMingConsequenceConflictType : uint8
{
    None,
    PoliticalConflict,     // 政治後果衝突
    MilitaryConflict,      // 軍事後果衝突
    EconomicConflict,      // 經濟後果衝突
    SocialConflict,        // 社會後果衝突
    CrossDomainConflict   // 跨領域衝突
};

// 後果衝突
USTRUCT(BlueprintType)
struct MINGCORE_API FMingConsequenceConflict
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ConflictID;

    UPROPERTY(BlueprintReadOnly)
    EMingConsequenceConflictType ConflictType;

    UPROPERTY(BlueprintReadOnly)
    FString ConflictDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConflictingCalculators;

    UPROPERTY(BlueprintReadOnly)
    float ConflictSeverity;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ResolutionOptions;

    UPROPERTY(BlueprintReadOnly)
    FString RecommendedResolution;
};

// 衝突解決策略
UENUM(BlueprintType)
enum class EMingConflictResolutionStrategy : uint8
{
    PriorityBased,         // 基於優先級
    WeightedAverage,      // 加權平均
    DomainOverride,       // 領域覆蓋
    Conservative,         // 保守策略
    Aggressive,           // 激進策略
    Manual               // 手動解決
};

/**
 * 後果衝突解決器
 * 檢測和解決多個計算器之間的後果衝突
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingConsequenceConflictResolver : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingConsequenceConflictResolver();

    // 檢測後果衝突
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    bool DetectConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // 解決衝突
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    void ResolveConflicts(TArray<FMingConsequenceResult>& Results, EMingConflictResolutionStrategy Strategy = EMingConflictResolutionStrategy::PriorityBased);

    // 解決特定衝突
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    void ApplyConflictResolution(const FMingConsequenceConflict& Conflict, FMingConsequenceResult& Result);

    // 獲取推薦解決策略
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    EMingConflictResolutionStrategy GetRecommendedStrategy(const FMingConsequenceConflict& Conflict);

    // 驗證解決結果
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    bool ValidateResolution(const TArray<FMingConsequenceResult>& Results);

protected:
    // 檢測政治衝突
    void DetectPoliticalConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // 檢測軍事衝突
    void DetectMilitaryConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // 檢測經濟衝突
    void DetectEconomicConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // 檢測社會衝突
    void DetectSocialConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // 檢測跨領域衝突
    void DetectCrossDomainConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // 基於優先級解決
    void ResolvePriorityBased(TArray<FMingConsequenceResult>& Results);

    // 加權平均解決
    void ResolveWeightedAverage(TArray<FMingConsequenceResult>& Results);

    // 領域覆蓋解決
    void ResolveDomainOverride(TArray<FMingConsequenceResult>& Results);

    // 保守策略解決
    void ResolveConservative(TArray<FMingConsequenceResult>& Results);

    // 激進策略解決
    void ResolveAggressive(TArray<FMingConsequenceResult>& Results);

    // 計算衝突嚴重程度
    float CalculateConflictSeverity(const FMingConsequenceConflict& Conflict);

    // 生成解決選項
    void GenerateResolutionOptions(const FMingConsequenceConflict& Conflict, TArray<FString>& OutOptions);

    // 生成推薦解決方案
    void GenerateRecommendedResolution(const FMingConsequenceConflict& Conflict, FString& OutResolution);

private:
    // 計算器優先級映射
    TMap<FString, float> CalculatorPriorities;

    // 衝突檢測閾值
    static constexpr float CONFLICT_THRESHOLD = 0.3f;

    // 初始化優先級
    void InitializePriorities();
};
