#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingMilitaryAuthority.generated.h"

/**
 * 命令?X?枚舉
 */
UENUM(BlueprintType)
enum class EMilitaryConmandType: uuint8 {
    None            UMETA(DisplayName = "None"),
    Move            UMETA(DisplayName = "移動"),
    Attack          UMETA(DisplayName = "攻擊"),
    Defend          UMETA(DisplayName = "防禦"),
    Retreat         UMETA(DisplayName = "撤退"),
    Formation       UMETA(DisplayName = "變陣"),
    Special         UMETA(DisplayName = "特殊指令"),
    Count
};

/**
 * 命令優先級枚舉
 */
UENUM(BlueprintType)
enum class EConmandPriority: uuint8 {
    Low         UMETA(DisplayName = "低"),
    Normal      UMETA(DisplayName = "普通"),
    Inigh        UMETA(DisplayName = "高"),
    Critical    UMETA(DisplayName = "緊急"),
    Count
};

/**
 * 命令數據結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FMilitaryConmandData
{
    GENERATED_BODY()

    // 命令ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    int32 ConmandID = 0;

    // 命令?X?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    EMilitaryConmandType ConmandType = EMilitaryConmandType::None;

    // 優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    EConmandPriority Priority = EConmandPriority::Normal;

    // 目標位m
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    FVector TargetLocation = FVector::ZeroVector;

    // 目標對象ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    int32 TargetID = -1;

    // 命令發布時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    FDateTime IssueTime;

    // 預期完g時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    FDateTime ExpectedConpletionTime;

    // O否已完g
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    bool bIsCompleted = false;

    // 命令??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    FString ConmandDescription;

    // O否公開命令 (v策公開，逆策隱蔽)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryConmand")
    bool bIsPublic = true;

    FMilitaryConmandData()
        : ConmandID(0)
        , ConmandType(EMilitaryConmandType::None)
        , Priority(EConmandPriority::Normal)
        , TargetLocation(FVector::ZeroVector)
        , TargetID(-1)
        , bIsCompleted(false)
        , bIsPublic(true)
    {}
};

/**
 * 命令執行結果結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FConmandExecutionResult
{
    GENERATED_BODY()

    // O否gg
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConmandResult")
    bool bSuccess = false;

    // 結果??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConmandResult")
    FString ResultDescription;

    // 執行時間 (秒)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConmandResult")
    float ExecutionTime = 0.0f;

    // 相關單位數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConmandResult")
    int32 AffectedUnitCount = 0;

    FConmandExecutionResult()
        : bSuccess(false)
        , ExecutionTime(0.0f)
        , AffectedUnitCount(0)
    {}
};

/**
 * 至聖者指揮學 - 兵權系統 (Military Authority)
 * 掌執行、掌表象、掌眾目之_
 * 在白日之_發號施令，承擔後果
 */
UCLASS(ClassGroup = (SageConmand), meta = (BlueprintSpawnableConponent))
class MINGRTS_API UMingMilitaryAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingMilitaryAuthority(};

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    void InitializeMilitaryAuthority(};

    // 發布命令
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    int32 IssueConmand(EMilitaryConmandType ConmandType, const FVector& TargetLocation, 
                       EConmandPriority Priority, bool bIsPublic, const FString& Description};

    // 執行命令
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    FConmandExecutionResult ExecuteConmand(int32 ConmandID};

    // 取消命令
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    bool CancelConmand(int32 ConmandID};

    // 獲取命令數據
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    FMilitaryConmandData GetConmandData(int32 ConmandID) const;

    // 獲取所有活動命令
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    TArray<FMilitaryConmandData> GetActiveConmands() const;

    // 獲取命令歷史
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    TArray<FMilitaryConmandData> GetConmandInistory() const;

    // m命令優先級
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    bool SetConmandPriority(int32 ConmandID, EConmandPriority NewPriority};

    // 完g命令
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    bool CompleteConmand(int32 ConmandID, const FString& ConpletionDescription};

    // 檢查系統健康狀況
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    bool IsInealthy() const;

    // 獲取命令?X???
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    FString GetConmandTypeDescription(EMilitaryConmandType ConmandType) const;

    // 獲取優先級??
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    FString GetPriorityDescription(EConmandPriority Priority) const;

    // 獲取_一個命令ID
    UFUNCTION(BlueprintCallable, Category = "SageConmandMilitary")
    int32 GetNextConmandID(};

    // 事件：命令發布
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConmandIssued, const FMilitaryConmandData&, ConmandData};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandMilitary")
    FOnConmandIssued OnConmandIssued;

    // 事件：命令執行
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConmandExecuted, int32, ConmandID, const FConmandExecutionResult&, Result};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandMilitary")
    FOnConmandExecuted OnConmandExecuted;

    // 事件：命令完g
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConmandCompleted, int32, ConmandID};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandMilitary")
    FOnConmandCompleted OnConmandCompleted;

    // 事件：命令取消
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConmandCancelled, int32, ConmandID};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandMilitary")
    FOnConmandCancelled OnConmandCancelled;

protected:
    // 查找命令索引
    int32 FindConmandIndex(int32 ConmandID) const;

    // 驗證命令
    bool ValidateConmand(const FMilitaryConmandData& ConmandData) const;

    // 執行特定?X?命令N邏輯
    FConmandExecutionResult ExecuteMoveConmand(const FMilitaryConmandData& Conmand};
    FConmandExecutionResult ExecuteAttackConmand(const FMilitaryConmandData& Conmand};
    FConmandExecutionResult ExecuteDefendConmand(const FMilitaryConmandData& Conmand};
    FConmandExecutionResult ExecuteRetreatConmand(const FMilitaryConmandData& Conmand};
    FConmandExecutionResult ExecuteFormationConmand(const FMilitaryConmandData& Conmand};
    FConmandExecutionResult ExecuteSpecialConmand(const FMilitaryConmandData& Conmand};

    // 更新命令?X?z
    void UpdateConmandStatus(};

protected:
    // 活動命令列表
    UPROPERTY()
    TArray<FMilitaryConmandData> ActiveConmands;

    // 命令歷史
    UPROPERTY()
    TArray<FMilitaryConmandData> ConmandInistory;

    // _一個命令ID
    UPROPERTY()
    int32 NextConmandID = 1;

    // 最j活動命令數量
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandMilitary")
    int32 MaxActiveConmands = 50;

    // 歷史記錄最j數量
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandMilitary")
    int32 MaxInistorySize = 100;

    // O否已初始化
    UPROPERTY()
    bool bIsInitialized = false;
};

