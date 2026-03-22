// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingMilitaryAuthority.generated.h"

/**
 * 命令類型枚舉
 */
UENUM(BlueprintType)
enum class EMilitaryCommandType: uint8 {
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
enum class ECommandPriority: uint8 {
    Low         UMETA(DisplayName = "低"),
    Normal      UMETA(DisplayName = "普通"),
    High        UMETA(DisplayName = "高"),
    Critical    UMETA(DisplayName = "緊急"),
    Count
};

/**
 * 命令數據結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FMilitaryCommandData
{
    GENERATED_BODY()

    // 命令ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    int32 CommandID = 0;

    // 命令類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    EMilitaryCommandType CommandType = EMilitaryCommandType::None;

    // 優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    ECommandPriority Priority = ECommandPriority::Normal;

    // 目標位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    FVector TargetLocation = FVector::ZeroVector;

    // 目標對象ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    int32 TargetID = -1;

    // 命令發布時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    FDateTime IssueTime;

    // 預期完成時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    FDateTime ExpectedCompletionTime;

    // 是否已完成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    bool bIsCompleted = false;

    // 命令描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    FString CommandDescription;

    // 是否公開命令 (正策公開，逆策隱蔽)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryCommand")
    bool bIsPublic = true;

    FMilitaryCommandData()
        : CommandID(0)
        , CommandType(EMilitaryCommandType::None)
        , Priority(ECommandPriority::Normal)
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
struct MINGGORTS_API FCommandExecutionResult
{
    GENERATED_BODY()

    // 是否成功
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommandResult")
    bool bSuccess = false;

    // 結果描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommandResult")
    FString ResultDescription;

    // 執行時間 (秒)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommandResult")
    float ExecutionTime = 0.0f;

    // 相關單位數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommandResult")
    int32 AffectedUnitCount = 0;

    FCommandExecutionResult()
        : bSuccess(false)
        , ExecutionTime(0.0f)
        , AffectedUnitCount(0)
    {}
};

/**
 * 至聖者指揮學 - 兵權系統 (Military Authority)
 * 掌執行、掌表象、掌眾目之下
 * 在白日之下發號施令，承擔後果
 */
UCLASS(ClassGroup = (SageCommand), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingMilitaryAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingMilitaryAuthority();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    void InitializeMilitaryAuthority();

    // 發布命令
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    int32 IssueCommand(EMilitaryCommandType CommandType, const FVector& TargetLocation, 
                       ECommandPriority Priority, bool bIsPublic, const FString& Description);

    // 執行命令
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    FCommandExecutionResult ExecuteCommand(int32 CommandID);

    // 取消命令
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    bool CancelCommand(int32 CommandID);

    // 獲取命令數據
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    FMilitaryCommandData GetCommandData(int32 CommandID) const;

    // 獲取所有活動命令
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    TArray<FMilitaryCommandData> GetActiveCommands() const;

    // 獲取命令歷史
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    TArray<FMilitaryCommandData> GetCommandHistory() const;

    // 設置命令優先級
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    bool SetCommandPriority(int32 CommandID, ECommandPriority NewPriority);

    // 完成命令
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    bool CompleteCommand(int32 CommandID, const FString& CompletionDescription);

    // 檢查系統健康狀況
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    bool IsHealthy() const;

    // 獲取命令類型描述
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    FString GetCommandTypeDescription(EMilitaryCommandType CommandType) const;

    // 獲取優先級描述
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    FString GetPriorityDescription(ECommandPriority Priority) const;

    // 獲取下一個命令ID
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Military")
    int32 GetNextCommandID();

    // 事件：命令發布
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommandIssued, const FMilitaryCommandData&, CommandData);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Military")
    FOnCommandIssued OnCommandIssued;

    // 事件：命令執行
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommandExecuted, int32, CommandID, const FCommandExecutionResult&, Result);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Military")
    FOnCommandExecuted OnCommandExecuted;

    // 事件：命令完成
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommandCompleted, int32, CommandID);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Military")
    FOnCommandCompleted OnCommandCompleted;

    // 事件：命令取消
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommandCancelled, int32, CommandID);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Military")
    FOnCommandCancelled OnCommandCancelled;

protected:
    // 查找命令索引
    int32 FindCommandIndex(int32 CommandID) const;

    // 驗證命令
    bool ValidateCommand(const FMilitaryCommandData& CommandData) const;

    // 執行特定類型命令的邏輯
    FCommandExecutionResult ExecuteMoveCommand(const FMilitaryCommandData& Command);
    FCommandExecutionResult ExecuteAttackCommand(const FMilitaryCommandData& Command);
    FCommandExecutionResult ExecuteDefendCommand(const FMilitaryCommandData& Command);
    FCommandExecutionResult ExecuteRetreatCommand(const FMilitaryCommandData& Command);
    FCommandExecutionResult ExecuteFormationCommand(const FMilitaryCommandData& Command);
    FCommandExecutionResult ExecuteSpecialCommand(const FMilitaryCommandData& Command);

    // 更新命令狀態
    void UpdateCommandStatus();

protected:
    // 活動命令列表
    UPROPERTY()
    TArray<FMilitaryCommandData> ActiveCommands;

    // 命令歷史
    UPROPERTY()
    TArray<FMilitaryCommandData> CommandHistory;

    // 下一個命令ID
    UPROPERTY()
    int32 NextCommandID = 1;

    // 最大活動命令數量
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|Military")
    int32 MaxActiveCommands = 50;

    // 歷史記錄最大數量
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|Military")
    int32 MaxHistorySize = 100;

    // 是否已初始化
    UPROPERTY()
    bool bIsInitialized = false;
};

