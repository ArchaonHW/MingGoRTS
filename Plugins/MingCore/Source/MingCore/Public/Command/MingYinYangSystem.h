#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingYinYangSystem.generated.h"

UENUM(BlueprintType)
enum class EYinYangAttribute : uint8
{
    Visible,        // 可見：陽性屬性
    Invisible,      // 不可見：陰性屬性
    Order,          // 秩序：陽性屬性
    Chaos,          // 混亂：陰性屬性
    Public,         // 公開：陽性屬性
    Hidden,         // 隱蔽：陰性屬性
    Sustainable,    // 可持續：陽性屬性
    Urgent          // 緊急：陰性屬性
};

USTRUCT(BlueprintType)
struct FYinYangBalance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YinLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YangLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BalanceRatio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsBalanced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StabilityIndex;

    FYinYangBalance()
    {
        YinLevel = 50.0f;
        YangLevel = 50.0f;
        BalanceRatio = 1.0f;
        bIsBalanced = true;
        StabilityIndex = 100.0f;
    }
};

USTRUCT(BlueprintType)
struct FYinYangAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EYinYangAttribute Attribute;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YinImpact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YangImpact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsReversible;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    FYinYangAction()
    {
        Attribute = EYinYangAttribute::Visible;
        YinImpact = 0.0f;
        YangImpact = 0.0f;
        Duration = 0.0f;
        bIsReversible = true;
        Description = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FYinYangSituation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TransparencyLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChaosLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UrgencyLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StabilityLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequiresYinApproach;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequiresYangApproach;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EYinYangAttribute> DominantAttributes;

    FYinYangSituation()
    {
        TransparencyLevel = 50.0f;
        ChaosLevel = 50.0f;
        UrgencyLevel = 50.0f;
        StabilityLevel = 50.0f;
        bRequiresYinApproach = false;
        bRequiresYangApproach = false;
    }
};

/**
 * 陰陽系統
 * 
 * 核心理念：
 * 1. 陰陽不是善惡，而是可見與不可見
 * 2. 陽：秩序、公開、可承擔
 * 3. 陰：破局、隱蔽、非常態
 * 4. 全透明是最高級的謊言
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingYinYangSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingYinYangSystem();

    // 初始化陰陽系統
    UFUNCTION(BlueprintCallable)
    void InitializeYinYangSystem();

    // 分析當前局勢的陰陽屬性
    UFUNCTION(BlueprintCallable)
    FYinYangSituation AnalyzeYinYangSituation();

    // 計算陰陽平衡
    UFUNCTION(BlueprintCallable)
    FYinYangBalance CalculateBalance();

    // 執行陰陽動作
    UFUNCTION(BlueprintCallable)
    bool ExecuteYinYangAction(const FYinYangAction& Action);

    // 檢查是否可以保持純陽
    UFUNCTION(BlueprintCallable)
    bool CanMaintainPureYang(const FYinYangSituation& Situation);

    // 檢查是否需要使用陰性手段
    UFUNCTION(BlueprintCallable)
    bool RequiresYinApproach(const FYinYangSituation& Situation);

    // 獲取推薦的陰陽策略
    UFUNCTION(BlueprintCallable)
    TArray<EYinYangAttribute> GetRecommendedAttributes(const FYinYangSituation& Situation);

    // 平衡陰陽
    UFUNCTION(BlueprintCallable)
    void BalanceYinYang(float TargetYin = 50.0f, float TargetYang = 50.0f);

    // 計算陰陽轉換成本
    UFUNCTION(BlueprintCallable)
    float CalculateTransitionCost(EYinYangAttribute From, EYinYangAttribute To);

    // 檢查陰陽協調性
    UFUNCTION(BlueprintCallable)
    bool IsYinYangCoherent(const TArray<EYinYangAttribute>& Attributes);

    // 獲取當前陰陽狀態
    UFUNCTION(BlueprintCallable)
    FYinYangBalance GetCurrentBalance() const { return CurrentBalance; }

    // 設置陰陽平衡
    UFUNCTION(BlueprintCallable)
    void SetCurrentBalance(const FYinYangBalance& NewBalance) { CurrentBalance = NewBalance; }

protected:
    // 當前陰陽平衡狀態
    UPROPERTY(BlueprintReadOnly)
    FYinYangBalance CurrentBalance;

    // 陰陽動作歷史
    UPROPERTY(BlueprintReadOnly)
    TArray<FYinYangAction> ActionHistory;

    // 陰陽閾值設定
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YinYangThreshold;

    // 平衡閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BalanceThreshold;

    // 穩定性閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StabilityThreshold;

private:
    // 內部輔助函數
    void UpdateBalance();
    float CalculateStability();
    bool IsActionCompatible(const FYinYangAction& Action, const FYinYangSituation& Situation);
    void RecordAction(const FYinYangAction& Action);
};
