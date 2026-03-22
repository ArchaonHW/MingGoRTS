// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingThreePowerSystem.generated.h"

class UMingMoralAuthority;
class UMingStrategyAuthority;
class UMingMilitaryAuthority;

/**
 * 三權類型枚舉
 * 道權：防墮監測 (Moral Authority)
 * 策權：策略切換 (Strategic Authority)
 * 兵權：執行指揮 (Military Authority)
 */
UENUM(BlueprintType)
enum class EThreePowerType: uint8 {
    None            UMETA(DisplayName = "None"),
    Moral           UMETA(DisplayName = "道權"),      // 掌天道、掌大義、掌不傳之秘
    Strategy        UMETA(DisplayName = "策權"),    // 掌正逆、掌陰陽、掌五行節奏
    Military        UMETA(DisplayName = "兵權"),    // 掌執行、掌表象、掌眾目之下
    Count
};

/**
 * 三權狀態結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FThreePowerStatus
{
    GENERATED_BODY()

    // 道權狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    bool bMoralAuthorityActive = true;

    // 策權狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    bool bStrategyAuthorityActive = true;

    // 兵權狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreePower")
    bool bMilitaryAuthorityActive = true;

    // 三權平衡值 (100 = 完全平衡)
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
struct MINGGORTS_API FPowerDistribution
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

    // 是否動態調整
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
 * 管理道權、策權、兵權的協調與分配
 */
UCLASS(ClassGroup = (SageCommand), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingThreePowerSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingThreePowerSystem();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    void InitializeThreePowerSystem();

    // 設置三權子系統
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    void SetPowerAuthorities(UMingMoralAuthority* MoralAuth, 
                             UMingStrategyAuthority* StrategyAuth, 
                             UMingMilitaryAuthority* MilitaryAuth);

    // 協調三權
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    bool CoordinatePowers();

    // 獲取當前狀態
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    FThreePowerStatus GetCurrentStatus() const { return CurrentStatus; }

    // 設置權力分配
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    void SetPowerDistribution(const FPowerDistribution& Distribution);

    // 獲取當前權力分配
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    FPowerDistribution GetPowerDistribution() const { return CurrentDistribution; }

    // 檢查三權是否平衡
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    bool IsPowerBalanced() const;

    // 獲取不平衡的權力類型
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    TArray<EThreePowerType> GetUnbalancedPowers() const;

    // 調整特定權力的活躍狀態
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    void SetPowerActive(EThreePowerType PowerType, bool bActive);

    // 獲取權力顯示名稱
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    FString GetPowerDisplayName(EThreePowerType PowerType) const;

    // 獲取權力描述
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    FString GetPowerDescription(EThreePowerType PowerType) const;

    // 道權檢查 (防墮機制)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    bool CheckMoralAuthority() const;

    // 策權決策 (正逆切換)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    bool ExecuteStrategyDecision() const;

    // 兵權執行 (命令發布)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    bool ExecuteMilitaryCommand() const;

    // 事件：三權失衡
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPowerImbalance, const FThreePowerStatus&, Status);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|ThreePower")
    FOnPowerImbalance OnPowerImbalance;

    // 事件：權力協調完成
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPowerCoordinated);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|ThreePower")
    FOnPowerCoordinated OnPowerCoordinated;

    // 事件：特定權力變化
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPowerChanged, EThreePowerType, PowerType, bool, bActive);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|ThreePower")
    FOnPowerChanged OnPowerChanged;

    // 子系統訪問器
    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    UMingMoralAuthority* GetMoralAuthority() const { return MoralAuthority; }

    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    UMingStrategyAuthority* GetStrategyAuthority() const { return StrategyAuthority; }

    UFUNCTION(BlueprintCallable, Category = "SageCommand|ThreePower")
    UMingMilitaryAuthority* GetMilitaryAuthority() const { return MilitaryAuthority; }

protected:
    // 計算平衡值
    int32 CalculateBalanceValue() const;

    // 自動調整權力分配
    void AutoAdjustDistribution();

    // 檢查權力衝突
    bool CheckPowerConflicts() const;

    // 解決權力衝突
    void ResolvePowerConflicts();

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

    // 當前狀態
    UPROPERTY()
    FThreePowerStatus CurrentStatus;

    // 當前權力分配
    UPROPERTY()
    FPowerDistribution CurrentDistribution;

    // 是否已初始化
    UPROPERTY()
    bool bIsInitialized = false;

    // 自動協調間隔 (秒)
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|ThreePower")
    float AutoCoordinationInterval = 60.0f;

    // 平衡閾值 (低於此值視為不平衡)
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|ThreePower")
    int32 BalanceThreshold = 70;
};
