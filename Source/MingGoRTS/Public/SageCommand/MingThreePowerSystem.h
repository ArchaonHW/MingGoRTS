#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingThreePowerSystem.generated.h"

class UMingMoralAuthority;
class UMingStrategyAuthority;
class UMingMilitaryAuthority;

/**
 * 三權?X?枚舉
 * 道權：防墮監測 (Moral Authority)
 * 策權：策略切換 (Strategic Authority)
 * 兵權：執行指揮 (Military Authority)
 */
UENUM(BlueprintType)
enum class EThreePowerType: uuint8 {
    None            UMETA(DisplayName = "None"),
    Moral           UMETA(DisplayName = "道權"),      // 掌天道、掌j義、掌不傳之秘
    Strategy        UMETA(DisplayName = "策權"),    // 掌v逆、掌陰陽、掌五行節奏
    Military        UMETA(DisplayName = "兵權"),    // 掌執行、掌表象、掌眾目之_
    Count
};

/**
 * 三權?X?z結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FThreePowerStatus
{
    GENERATED_BODY()

    // 道權?X?z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    bool bMoralAuthorityActive = true;

    // 策權?X?z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    bool bStrategyAuthorityActive = true;

    // 兵權?X?z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    bool bMilitaryAuthorityActive = true;

    // 三權平衡z (100 = 完全平衡)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    int32 PowerBalanceValue = 100;

    // 最後協調時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    FDateTime LastCoordinationTime;

    FThreePowerStatus()
        : bMoralAuthorityActive(true)
        , bStrategyAuthorityActive(true)
        , bMilitaryAuthorityActive(true)
        , PowerBalanceValue(100)
    {}
};

/**
 * 權力分配結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FPowerDistribution
{
    GENERATED_BODY()

    // 道權分配比例 (0.0 - 1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    float MoralPowerShare = 0.33f;

    // 策權分配比例
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    float StrategyPowerShare = 0.33f;

    // 兵權分配比例
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    float MilitaryPowerShare = 0.34f;

    // O否動態調整
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    bool bDynamicAdjustment = true;

    FPowerDistribution()
        : MoralPowerShare(0.33f)
        , StrategyPowerShare(0.33f)
        , MilitaryPowerShare(0.34f)
        , bDynamicAdjustment(true)
    {}
};

/**
 * 至聖者指揮學 - 三權指揮系統
 * 管理道權、策權、兵權N協調與分配
 */
UCLASS(ClassGroup = (SageConmand), meta = (BlueprintSpawnableConponent))
class MINGRTS_API UMingThreePowerSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingThreePowerSystem(};

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    void InitializeThreePowerSystem(};

    // m三權子系統
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    void SetPowerAuthorities(UMingMoralAuthority* MoralAuth, 
                             UMingStrategyAuthority* StrategyAuth, 
                             UMingMilitaryAuthority* MilitaryAuth};

    // 協調三權
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    bool CoordinatePowers(};

    // 獲取當前?X?z
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    FThreePowerStatus GetCurrentStatus() const { return CurrentStatus; }

    // m權力分配
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    void SetPowerDistribution(const FPowerDistribution& Distribution};

    // 獲取當前權力分配
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    FPowerDistribution GetPowerDistribution() const { return CurrentDistribution; }

    // 檢查三權O否平衡
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    bool IsPowerBalanced() const;

    // 獲取不平衡N權力?X?
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    TArray<EThreePowerType> GetUnbalancedPowers() const;

    // 調整特定權力N活躍?X?z
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    void SetPowerActive(EThreePowerType PowerType, bool bActive};

    // 獲取權力顯示D??
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    FString GetPowerDisplayName(EThreePowerType PowerType) const;

    // 獲取權力??
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    FString GetPowerDescription(EThreePowerType PowerType) const;

    // 道權檢查 (防墮機制)
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    bool CheckMoralAuthority() const;

    // 策權決策 (v逆切換)
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    bool ExecuteStrategyDecision() const;

    // 兵權執行 (命令發布)
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    bool ExecuteMilitaryConmand() const;

    // 事件：三權失衡
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPowerImbalance, const FThreePowerStatus&, Status};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandThreePower")
    FOnPowerImbalance OnPowerImbalance;

    // 事件：權力協調完g
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPowerCoordinated};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandThreePower")
    FOnPowerCoordinated OnPowerCoordinated;

    // 事件：特定權力變化
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPowerChanged, EThreePowerType, PowerType, bool, bActive};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandThreePower")
    FOnPowerChanged OnPowerChanged;

    // 子系統訪問器
    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    UMingMoralAuthority* GetMoralAuthority() const { return MoralAuthority; }

    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    UMingStrategyAuthority* GetStrategyAuthority() const { return StrategyAuthority; }

    UFUNCTION(BlueprintCallable, Category = "SageConmandThreePower")
    UMingMilitaryAuthority* GetMilitaryAuthority() const { return MilitaryAuthority; }

protected:
    // 計算平衡z
    int32 CalculateBalanceValue() const;

    // 自動調整權力分配
    void AutoAdjustDistribution(};

    // 檢查權力衝突
    bool CheckPowerConflicts() const;

    // 解決權力衝突
    void ResolvePowerConflicts(};

protected:
    // 道權子系統
    UPROPERTY()
    TObjectPtr<UMingMoralAuthority> MoralAuthority;

    // 策權子系統
    UPROPERTY()
    TObjectPtr<UMingStrategyAuthority> StrategyAuthority;

    // 兵權子系統
    UPROPERTY()
    TObjectPtr<UMingMilitaryAuthority> MilitaryAuthority;

    // 當前?X?z
    UPROPERTY()
    FThreePowerStatus CurrentStatus;

    // 當前權力分配
    UPROPERTY()
    FPowerDistribution CurrentDistribution;

    // O否已初始化
    UPROPERTY()
    bool bIsInitialized = false;

    // 自動協調間隔 (秒)
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandThreePower")
    float AutoCoordinationInterval = 60.0f;

    // 平衡閾z (低於此z視?X??不平衡)
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandThreePower")
    int32 BalanceThreshold = 70;
};
