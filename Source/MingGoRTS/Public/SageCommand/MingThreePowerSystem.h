#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingThreePowerSystem.generated.h"

// ============================================================================
// 前向聲明
// ============================================================================

class UMingMoralAuthority;
class UMingStrategyAuthority;
class UMingMilitaryAuthority;

// ============================================================================
// 數據結構定義
// ============================================================================

/** 權力分配狀態 */
USTRUCT(BlueprintType)
struct FPowerDistributionState
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Power System")
    ESupremeAuthorityType ActiveAuthority;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power System")
    float DaoAuthorityPower;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power System")
    float StrategyAuthorityPower;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power System")
    float MilitaryAuthorityPower;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power System")
    float TotalBalanceIndex;
    
    FPowerDistributionState()
        : ActiveAuthority(ESupremeAuthorityType::None)
        , DaoAuthorityPower(0.0f)
        , StrategyAuthorityPower(0.0f)
        , MilitaryAuthorityPower(0.0f)
        , TotalBalanceIndex(0.0f)
    {}
};

/** 權力平衡參數 */
USTRUCT(BlueprintType)
struct FPowerBalanceParameters
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Power Balance")
    float MaxImbalanceThreshold;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power Balance")
    float AutoBalanceSpeed;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power Balance")
    bool bEnableAutoBalancing;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power Balance")
    float BalanceCheckInterval;
    
    FPowerBalanceParameters()
        : MaxImbalanceThreshold(0.3f)
        , AutoBalanceSpeed(0.1f)
        , bEnableAutoBalancing(true)
        , BalanceCheckInterval(5.0f)
    {}
};

/** 權力轉移記錄 */
USTRUCT(BlueprintType)
struct FPowerTransferRecord
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Power Transfer")
    ESupremeAuthorityType FromAuthority;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power Transfer")
    ESupremeAuthorityType ToAuthority;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power Transfer")
    FDateTime TransferTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power Transfer")
    FString TransferReason;
    
    UPROPERTY(BlueprintReadWrite, Category = "Power Transfer")
    float TransferAmount;
    
    FPowerTransferRecord()
        : FromAuthority(ESupremeAuthorityType::None)
        , ToAuthority(ESupremeAuthorityType::None)
        , TransferAmount(0.0f)
    {}
};

// ============================================================================
// 委託聲明
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPowerDistributionChanged, 
    ESupremeAuthorityType, ActiveAuthority, 
    const FPowerDistributionState&, NewState, 
    const FPowerDistributionState&, PreviousState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPowerBalanceTriggered,
    ESupremeAuthorityType, ImbalancedAuthority,
    float, ImbalanceAmount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPowerTransferred,
    ESupremeAuthorityType, FromAuthority,
    ESupremeAuthorityType, ToAuthority,
    float, TransferAmount,
    const FString&, Reason);

// ============================================================================
// 主系統類別
// ============================================================================

/**
 * 三權系統管理器
 * 負責道權、策權、兵權的分配、平衡和轉移
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|Three Power System")
class MINGGORTS_API UMingThreePowerSystem : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 構造函數與初始化
    // ============================================================================

    UMingThreePowerSystem();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 初始化三權系統 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    bool InitializeSystem();

    /** 重置系統到初始狀態 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    void ResetSystem();

    // ============================================================================
    // 權力分配邏輯
    // ============================================================================

    /** 分配權力給指定權威 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    bool AllocatePower(ESupremeAuthorityType AuthorityType, float PowerAmount);

    /** 重新分配所有權力 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    bool RedistributeAllPower();

    /** 獲取當前權力分配狀態 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    FPowerDistributionState GetCurrentDistribution() const;

    /** 設置權力分配參數 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    void SetDistributionParameters(const FPowerBalanceParameters& Parameters);

    // ============================================================================
    // 權力平衡機制
    // ============================================================================

    /** 檢查權力平衡狀態 */
    UFUNCTION(BlueprintCallable, Category = "Power Balance")
    bool IsPowerBalanced() const;

    /** 計算權力平衡指數 */
    UFUNCTION(BlueprintCallable, Category = "Power Balance")
    float CalculateBalanceIndex() const;

    /** 執行自動權力平衡 */
    UFUNCTION(BlueprintCallable, Category = "Power Balance")
    void PerformAutoBalance();

    /** 手動觸發權力平衡 */
    UFUNCTION(BlueprintCallable, Category = "Power Balance")
    bool TriggerManualBalance(ESupremeAuthorityType TargetAuthority);

    /** 獲取權力失衡建議 */
    UFUNCTION(BlueprintCallable, Category = "Power Balance")
    TArray<ESupremeAuthorityType> GetBalanceSuggestions() const;

    // ============================================================================
    // 權力轉移系統
    // ============================================================================

    /** 轉移權力從一個權威到另一個 */
    UFUNCTION(BlueprintCallable, Category = "Power Transfer")
    bool TransferPower(ESupremeAuthorityType FromAuthority, 
                     ESupremeAuthorityType ToAuthority, 
                     float Amount, 
                     const FString& Reason = TEXT("手動轉移"));

    /** 檢查權力轉移是否被允許 */
    UFUNCTION(BlueprintCallable, Category = "Power Transfer")
    bool CanTransferPower(ESupremeAuthorityType FromAuthority, 
                        ESupremeAuthorityType ToAuthority, 
                        float Amount) const;

    /** 獲取權力轉移歷史 */
    UFUNCTION(BlueprintCallable, Category = "Power Transfer")
    TArray<FPowerTransferRecord> GetTransferHistory(int32 MaxRecords = 50) const;

    /** 清除轉移歷史 */
    UFUNCTION(BlueprintCallable, Category = "Power Transfer")
    void ClearTransferHistory();

    // ============================================================================
    // 權力狀態查詢
    // ============================================================================

    /** 獲取指定權威的權力值 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    float GetAuthorityPower(ESupremeAuthorityType AuthorityType) const;

    /** 獲取活躍權威 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    ESupremeAuthorityType GetActiveAuthority() const;

    /** 檢查權威是否活躍 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    bool IsAuthorityActive(ESupremeAuthorityType AuthorityType) const;

    /** 獲取總權力值 */
    UFUNCTION(BlueprintCallable, Category = "Power System")
    float GetTotalPower() const;

    // ============================================================================
    // 事件委託
    // ============================================================================

    /** 權力分配改變事件 */
    UPROPERTY(BlueprintAssignable, Category = "Power System Events")
    FOnPowerDistributionChanged OnPowerDistributionChanged;

    /** 權力平衡觸發事件 */
    UPROPERTY(BlueprintAssignable, Category = "Power System Events")
    FOnPowerBalanceTriggered OnPowerBalanceTriggered;

    /** 權力轉移事件 */
    UPROPERTY(BlueprintAssignable, Category = "Power System Events")
    FOnPowerTransferred OnPowerTransferred;

protected:
    // ============================================================================
    // 內部狀態
    // ============================================================================

    /** 當前權力分配狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Power System")
    FPowerDistributionState CurrentDistribution;

    /** 權力平衡參數 */
    UPROPERTY(BlueprintReadWrite, Category = "Power Balance")
    FPowerBalanceParameters BalanceParameters;

    /** 權力轉移歷史記錄 */
    UPROPERTY(BlueprintReadOnly, Category = "Power Transfer")
    TArray<FPowerTransferRecord> TransferHistory;

    /** 權威組件引用 */
    UPROPERTY(BlueprintReadOnly, Category = "Power System")
    TObjectPtr<UMingMoralAuthority> MoralAuthorityComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Power System")
    TObjectPtr<UMingStrategyAuthority> StrategyAuthorityComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Power System")
    TObjectPtr<UMingMilitaryAuthority> MilitaryAuthorityComponent;

    /** 系統狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Power System")
    bool bSystemInitialized;

    UPROPERTY(BlueprintReadOnly, Category = "Power System")
    bool bAutoBalancingActive;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 更新權力分配狀態 */
    void UpdateDistributionState();

    /** 檢查並觸發權力平衡 */
    void CheckAndTriggerBalance();

    /** 記錄權力轉移 */
    void RecordPowerTransfer(ESupremeAuthorityType FromAuthority,
                          ESupremeAuthorityType ToAuthority,
                          float Amount,
                          const FString& Reason);

    /** 驗證權力分配 */
    bool ValidatePowerAllocation(ESupremeAuthorityType AuthorityType, float Amount) const;

    /** 計算權力轉移成本 */
    float CalculateTransferCost(ESupremeAuthorityType FromAuthority,
                             ESupremeAuthorityType ToAuthority,
                             float Amount) const;

    /** 應用權力轉移效果 */
    void ApplyPowerTransferEffects(ESupremeAuthorityType FromAuthority,
                                ESupremeAuthorityType ToAuthority,
                                float Amount);

    /** 觸發權力分配改變事件 */
    void TriggerDistributionChangedEvent(const FPowerDistributionState& PreviousState);

    /** 獲取權力分配建議 */
    TArray<FPowerDistributionState> GenerateDistributionSuggestions() const;

private:
    /** 定時器句柄 */
    FTimerHandle BalanceCheckTimer;

    /** 最大權力值 */
    static constexpr float MAX_POWER_VALUE = 100.0f;

    /** 最小權力轉移值 */
    static constexpr float MIN_TRANSFER_AMOUNT = 1.0f;
};
