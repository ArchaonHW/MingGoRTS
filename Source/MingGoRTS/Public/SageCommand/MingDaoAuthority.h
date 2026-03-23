#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingDaoAuthority.generated.h"

// 道權狀態枚舉
UENUM(BlueprintType)
enum class EDaoAuthorityState : uint8
{
    Inactive,        // 未激活
    Active,          // 激活中
    Dominant,        // 主導
    Suppressed,      // 被壓制
    Enlightened      // 覺醒狀態
};

// 道權類型枚舉
UENUM(BlueprintType)
enum class EDaoAuthorityType : uint8
{
    Governance,      // 治理權
    Moral,          // 道德權
    Balance,        // 平衡權
    Harmony         // 和諧權
};

// 道權數據結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FDaoAuthorityData
{
    GENERATED_BODY()

    /** 道權狀態 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    EDaoAuthorityState AuthorityState;

    /** 道權類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    EDaoAuthorityType AuthorityType;

    /** 道權值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    float AuthorityValue;

    /** 最大道權值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    float MaxAuthorityValue;

    /** 治理能力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    float GovernanceAbility;

    /** 道德影響力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    float MoralInfluence;

    /** 平衡指數 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    float BalanceIndex;

    /** 和諧度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    float HarmonyLevel;

    /** 影響範圍 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    float InfluenceRange;

    /** 持續時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    float Duration;

    /** 冷卻時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Data")
    float CooldownTime;
};

// 道權效果結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FDaoAuthorityEffect
{
    GENERATED_BODY()

    /** 治理效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Effect")
    float GovernanceEffect;

    /** 道德效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Effect")
    float MoralEffect;

    /** 平衡效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Effect")
    float BalanceEffect;

    /** 和諧效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Effect")
    float HarmonyEffect;

    /** 影響半徑 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Effect")
    float EffectRadius;

    /** 效果強度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dao Authority Effect")
    float EffectIntensity;
};

// 道權事件代理
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDaoAuthorityActivated, EDaoAuthorityType, AuthorityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDaoAuthorityLevelUp, EDaoAuthorityType, AuthorityType, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDaoAuthorityEnlightened, EDaoAuthorityType, AuthorityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGovernanceApplied, float, GovernanceBonus, FString, TargetDescription);

/**
 * 道權系統
 * 管理治理、道德、平衡、和諧四種道權
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingDaoAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingDaoAuthority();

    // ============================================================================
    // 系統初始化
    // ============================================================================

    /** 初始化道權系統 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    void InitializeDaoAuthority();

    /** 重置道權系統 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    void ResetDaoAuthority();

    // ============================================================================
    // 道權管理
    // ============================================================================

    /** 激活道權 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool ActivateDaoAuthority(EDaoAuthorityType AuthorityType);

    /** 停用道權 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool DeactivateDaoAuthority(EDaoAuthorityType AuthorityType);

    /** 獲取道權數據 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    FDaoAuthorityData GetDaoAuthorityData(EDaoAuthorityType AuthorityType) const;

    /** 設置道權數據 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    void SetDaoAuthorityData(EDaoAuthorityType AuthorityType, const FDaoAuthorityData& AuthorityData);

    // ============================================================================
    // 治理權系統
    // ============================================================================

    /** 應用治理效果 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    void ApplyGovernanceEffect(const FString& TargetRegion, float GovernancePower);

    /** 提升治理能力 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool EnhanceGovernanceAbility(float EnhancementAmount);

    /** 獲取治理等級 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    int32 GetGovernanceLevel() const { return GovernanceLevel; }

    // ============================================================================
    // 道德權系統
    // ============================================================================

    /** 應用道德影響 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    void ApplyMoralInfluence(const FString& TargetGroup, float MoralPower);

    /** 檢查道德狀態 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool CheckMoralStatus();

    /** 淨化墮落 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    float PurifyCorruption(float CorruptionAmount);

    // ============================================================================
    // 平衡權系統
    // ============================================================================

    /** 計算平衡指數 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    float CalculateBalanceIndex();

    /** 調整權力平衡 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    void AdjustPowerBalance();

    /** 檢查平衡狀態 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool IsBalanced() const;

    // ============================================================================
    // 和諧權系統
    // ============================================================================

    /** 應用和諧效果 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    void ApplyHarmonyEffect(const FString& TargetArea, float HarmonyPower);

    /** 提升和諧度 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool IncreaseHarmonyLevel(float HarmonyAmount);

    /** 緩解衝突 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool ResolveConflict(float ConflictIntensity);

    // ============================================================================
    // 覺醒系統
    // ============================================================================

    /** 檢查覺醒條件 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool CheckEnlightenment();

    /** 進入覺醒狀態 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    void EnterEnlightenedState(EDaoAuthorityType AuthorityType);

    /** 覺醒效果加成 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    float ApplyEnlightenmentBonus(float BaseValue);

    // ============================================================================
    // 升級系統
    // ============================================================================

    /** 增加道權經驗 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    void AddDaoAuthorityExperience(EDaoAuthorityType AuthorityType, int32 Experience);

    /** 檢查道權升級 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool CheckDaoAuthorityLevelUp(EDaoAuthorityType AuthorityType);

    /** 提升道權等級 */
    UFUNCTION(BlueprintCallable, Category = "Dao Authority")
    bool LevelUpDaoAuthority(EDaoAuthorityType AuthorityType);

    // ============================================================================
    // 事件系統
    // ============================================================================

    /** 道權激活事件 */
    UPROPERTY(BlueprintAssignable, Category = "Dao Authority")
    FOnDaoAuthorityActivated OnDaoAuthorityActivated;

    /** 道權升級事件 */
    UPROPERTY(BlueprintAssignable, Category = "Dao Authority")
    FOnDaoAuthorityLevelUp OnDaoAuthorityLevelUp;

    /** 道權覺醒事件 */
    UPROPERTY(BlueprintAssignable, Category = "Dao Authority")
    FOnDaoAuthorityEnlightened OnDaoAuthorityEnlightened;

    /** 治理應用事件 */
    UPROPERTY(BlueprintAssignable, Category = "Dao Authority")
    FOnGovernanceApplied OnGovernanceApplied;

protected:
    // ============================================================================
    // 數據成員
    // ============================================================================

    /** 道權數據數組 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dao Authority")
    TArray<FDaoAuthorityData> DaoAuthorityDataArray;

    /** 當前道權效果 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dao Authority")
    FDaoAuthorityEffect CurrentEffect;

    /** 治理等級 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dao Authority")
    int32 GovernanceLevel;

    /** 道德等級 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dao Authority")
    int32 MoralLevel;

    /** 平衡等級 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dao Authority")
    int32 BalanceLevel;

    /** 和諧等級 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dao Authority")
    int32 HarmonyLevel;

    /** 系統是否已初始化 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dao Authority")
    bool bSystemInitialized;

    /** 覺醒狀態 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dao Authority")
    bool bIsEnlightened;

private:
    // ============================================================================
    // 私有方法
    // ============================================================================

    /** 初始化道權數據 */
    void InitializeDaoAuthorityData();

    /** 計算道權效果 */
    void CalculateDaoAuthorityEffect(EDaoAuthorityType AuthorityType);

    /** 更新道權狀態 */
    void UpdateDaoAuthorityState(EDaoAuthorityType AuthorityType);

    /** 應用道權特性 */
    void ApplyDaoAuthorityTraits(EDaoAuthorityType AuthorityType);

    /** 檢查道權冷卻 */
    bool CheckDaoAuthorityCooldown(EDaoAuthorityType AuthorityType);

    /** 處理覺醒轉變 */
    void HandleEnlightenmentTransition(EDaoAuthorityType AuthorityType);
};
