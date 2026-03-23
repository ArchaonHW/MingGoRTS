#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingThreePowerSystem.generated.h"

// 三權類型枚舉
UENUM(BlueprintType)
enum class EPowerType : uint8
{
    None            UMETA(DisplayName = "無"),
    MoralAuthority  UMETA(DisplayName = "道權"),
    StrategyAuthority UMETA(DisplayName = "策權"),
    MilitaryAuthority UMETA(DisplayName = "兵權")
};

// 權力狀態結構
USTRUCT(BlueprintType)
struct FPowerStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPowerType PowerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PowerEfficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ControlLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ActiveAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> PowerModifiers;

    FPowerStatus()
    {
        PowerType = EPowerType::None;
        CurrentPower = 50.0f;
        MaxPower = 100.0f;
        PowerEfficiency = 1.0f;
        ControlLevel = 1;
    }
};

// 三權平衡狀態
USTRUCT(BlueprintType)
struct FThreePowerBalance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPowerStatus MoralAuthority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPowerStatus StrategyAuthority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPowerStatus MilitaryAuthority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallBalance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DominantPowerRatio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsBalanced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EPowerType> ConflictingPowers;

    FThreePowerBalance()
    {
        MoralAuthority.PowerType = EPowerType::MoralAuthority;
        StrategyAuthority.PowerType = EPowerType::StrategyAuthority;
        MilitaryAuthority.PowerType = EPowerType::MilitaryAuthority;
        OverallBalance = 50.0f;
        DominantPowerRatio = 0.33f;
        bIsBalanced = true;
    }
};

// 權力轉移事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPowerTransferred, EPowerType, FromPower, EPowerType, ToPower, float, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPowerBalanceChanged, const FThreePowerBalance&, NewBalance, bool, bIsStable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPowerLevelUp, EPowerType, PowerType);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingThreePowerSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingThreePowerSystem();

    // 初始化三權系統
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    void InitializeThreePowerSystem();

    // 設置權力值
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    void SetPowerLevel(EPowerType PowerType, float PowerLevel);

    // 獲取權力狀態
    UFUNCTION(BlueprintPure, Category = "Three Power System")
    FPowerStatus GetPowerStatus(EPowerType PowerType) const;

    // 獲取三權平衡狀態
    UFUNCTION(BlueprintPure, Category = "Three Power System")
    FThreePowerBalance GetBalanceStatus() const { return CurrentBalance; }

    // 轉移權力
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    bool TransferPower(EPowerType FromPower, EPowerType ToPower, float Amount);

    // 消耗權力
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    bool ConsumePower(EPowerType PowerType, float Amount);

    // 恢復權力
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    void RestorePower(EPowerType PowerType, float Amount);

    // 檢查權力平衡
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    void CheckPowerBalance();

    // 計算權力效率
    UFUNCTION(BlueprintPure, Category = "Three Power System")
    float CalculatePowerEfficiency(EPowerType PowerType) const;

    // 獲取主導權力
    UFUNCTION(BlueprintPure, Category = "Three Power System")
    EPowerType GetDominantPower() const;

    // 檢查是否可以執行權力行動
    UFUNCTION(BlueprintPure, Category = "Three Power System")
    bool CanExecutePowerAction(EPowerType PowerType, float RequiredPower) const;

    // 執行權力行動
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    bool ExecutePowerAction(EPowerType PowerType, const FString& ActionName, float PowerCost);

    // 獲取可用權力行動
    UFUNCTION(BlueprintPure, Category = "Three Power System")
    TArray<FString> GetAvailablePowerActions(EPowerType PowerType) const;

    // 升級權力等級
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    bool UpgradePowerLevel(EPowerType PowerType);

    // 應用權力修飾符
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    void ApplyPowerModifier(EPowerType PowerType, const FString& ModifierName, float ModifierValue);

    // 移除權力修飾符
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    void RemovePowerModifier(EPowerType PowerType, const FString& ModifierName);

    // 獲取權力衝突狀態
    UFUNCTION(BlueprintPure, Category = "Three Power System")
    TArray<EPowerType> GetConflictingPowers() const;

    // 解決權力衝突
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    void ResolvePowerConflicts();

    // 重置權力系統
    UFUNCTION(BlueprintCallable, Category = "Three Power System")
    void ResetPowerSystem();

    // 獲取系統穩定性
    UFUNCTION(BlueprintPure, Category = "Three Power System")
    float GetSystemStability() const;

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Three Power System")
    FOnPowerTransferred OnPowerTransferred;

    UPROPERTY(BlueprintAssignable, Category = "Three Power System")
    FOnPowerBalanceChanged OnPowerBalanceChanged;

    UPROPERTY(BlueprintAssignable, Category = "Three Power System")
    FOnPowerLevelUp OnPowerLevelUp;

protected:
    // 藍圖實現事件
    UFUNCTION(BlueprintImplementableEvent, Category = "Three Power System")
    void OnPowerSystemInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Three Power System")
    void OnPowerStatusChanged(EPowerType PowerType, const FPowerStatus& NewStatus);

    UFUNCTION(BlueprintImplementableEvent, Category = "Three Power System")
    void OnPowerConflictDetected(const TArray<EPowerType>& ConflictingPowers);

    UFUNCTION(BlueprintImplementableEvent, Category = "Three Power System")
    void OnPowerActionExecuted(EPowerType PowerType, const FString& ActionName, bool bSuccess);

private:
    // 計算平衡值
    void CalculateBalance();
    
    // 檢測權力衝突
    void DetectPowerConflicts();
    
    // 更新權力效率
    void UpdatePowerEfficiency(EPowerType PowerType);
    
    // 初始化默認權力行動
    void InitializeDefaultPowerActions();
    
    // 獲取權力狀態引用
    FPowerStatus* GetPowerStatusRef(EPowerType PowerType);
    const FPowerStatus* GetPowerStatusRef(EPowerType PowerType) const;

protected:
    // 當前三權平衡狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    FThreePowerBalance CurrentBalance;

    // 系統是否已初始化
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    bool bIsInitialized;

    // 權力恢復速率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    float PowerRestoreRate;

    // 權力轉移效率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    float PowerTransferEfficiency;

    // 平衡閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    float BalanceThreshold;

    // 最大權力值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    float MaxPowerValue;

    // 權力行動映射
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    TMap<EPowerType, TArray<FString>> PowerActionsMap;

    // 權力行動成本映射
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    TMap<FString, float> PowerActionCosts;

    // 權力衝突歷史
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    TArray<FThreePowerBalance> ConflictHistory;

    // 系統穩定性歷史
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Three Power System")
    TArray<float> StabilityHistory;
};
