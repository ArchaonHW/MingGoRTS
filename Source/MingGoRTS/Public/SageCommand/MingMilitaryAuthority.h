#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingMilitaryAuthority.generated.h"

// ============================================================================
// 前向聲明
// ============================================================================

class UMingThreePowerSystem;

// ============================================================================
// 軍事系統枚舉定義
// ============================================================================

/** 軍事命令類型 */
UENUM(BlueprintType)
enum class EMilitaryCommandType : uint8
{
    None                UMETA(DisplayName = "無"),
    Move                UMETA(DisplayName = "移動"),
    Attack              UMETA(DisplayName = "攻擊"),
    Defend              UMETA(DisplayName = "防禦"),
    Support             UMETA(DisplayName = "支援"),
    Special             UMETA(DisplayName = "特殊"),
    Retreat             UMETA(DisplayName = "撤退"),
    Hold                UMETA(DisplayName = "堅守"),
    Patrol              UMETA(DisplayName = "巡邏")
};

/** 命令優先級 */
UENUM(BlueprintType)
enum class ECommandPriority : uint8
{
    Critical        UMETA(DisplayName = "關鍵"),
    High           UMETA(DisplayName = "高"),
    Medium         UMETA(DisplayName = "中"),
    Low            UMETA(DisplayName = "低"),
    Routine        UMETA(DisplayName = "常規")
};

/** 命令狀態 */
UENUM(BlueprintType)
enum class ECommandStatus : uint8
{
    Pending        UMETA(DisplayName = "待執行"),
    Executing      UMETA(DisplayName = "執行中"),
    Completed      UMETA(DisplayName = "已完成"),
    Failed         UMETA(DisplayName = "失敗"),
    Cancelled      UMETA(DisplayName = "已取消"),
    Suspended      UMETA(DisplayName = "已暫停")
};

/** 後果承擔類型 */
UENUM(BlueprintType)
enum class EConsequenceBearingType : uint8
{
    None                       UMETA(DisplayName = "無"),
    DirectResponsibility        UMETA(DisplayName = "直接責任"),
    ChainOfCommand             UMETA(DisplayName = "指揮鏈責任"),
    CollectiveResponsibility    UMETA(DisplayName = "集體責任"),
    MoralResponsibility        UMETA(DisplayName = "道德責任"),
    StrategicResponsibility     UMETA(DisplayName = "戰略責任")
};

// ============================================================================
// 數據結構定義
// ============================================================================

/** 軍事命令數據 */
USTRUCT(BlueprintType)
struct FMilitaryCommand
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    int32 CommandID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    EMilitaryCommandType CommandType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    ECommandPriority Priority;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    ECommandStatus Status;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    FString Description;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    FVector TargetLocation;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    FString TargetID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    TArray<FString> AssignedUnits;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    float ExecutionTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    float EstimatedDuration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    bool bIsPublic;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    FDateTime IssueTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command")
    FDateTime CompletionTime;
    
    FMilitaryCommand()
        : CommandID(0)
        , CommandType(EMilitaryCommandType::None)
        , Priority(ECommandPriority::Routine)
        , Status(ECommandStatus::Pending)
        , ExecutionTime(0.0f)
        , EstimatedDuration(0.0f)
        , bIsPublic(true)
    {}
};

/** 指揮鏈節點 */
USTRUCT(BlueprintType)
struct FCommandChainNode
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Command Chain")
    FString CommanderID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command Chain")
    FString CommanderName;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command Chain")
    int32 AuthorityLevel;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command Chain")
    TArray<FString> SubordinateIDs;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command Chain")
    bool bCanOverride;
    
    UPROPERTY(BlueprintReadWrite, Category = "Command Chain")
    TArray<int32> CommandHistory;
    
    FCommandChainNode()
        : AuthorityLevel(0)
        , bCanOverride(false)
    {}
};

/** 後果承擔數據 */
USTRUCT(BlueprintType)
struct FConsequenceBearing
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Consequence")
    EConsequenceBearingType BearingType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Consequence")
    FString ResponsiblePartyID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Consequence")
    FString OriginalCommandID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Consequence")
    FString ConsequenceDescription;
    
    UPROPERTY(BlueprintReadWrite, Category = "Consequence")
    float Severity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Consequence")
    FDateTime OccurrenceTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Consequence")
    bool bIsAcknowledged;
    
    UPROPERTY(BlueprintReadWrite, Category = "Consequence")
    TArray<FString> MitigationActions;
    
    FConsequenceBearing()
        : BearingType(EConsequenceBearingType::None)
        , Severity(0.0f)
        , bIsAcknowledged(false)
    {}
};

/** 命令執行結果 */
USTRUCT(BlueprintType)
struct FCommandExecutionResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Execution")
    int32 CommandID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Execution")
    bool bSuccess;
    
    UPROPERTY(BlueprintReadWrite, Category = "Execution")
    FString ResultMessage;
    
    UPROPERTY(BlueprintReadWrite, Category = "Execution")
    float ActualExecutionTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Execution")
    TArray<FString> Casualties;
    
    UPROPERTY(BlueprintReadWrite, Category = "Execution")
    TArray<FString> AssetLosses;
    
    UPROPERTY(BlueprintReadWrite, Category = "Execution")
    TArray<FString> StrategicGains;
    
    UPROPERTY(BlueprintReadWrite, Category = "Execution")
    FConsequenceBearing Consequence;
    
    FCommandExecutionResult()
        : CommandID(0)
        , bSuccess(false)
        , ActualExecutionTime(0.0f)
    {}
};

// ============================================================================
// 委託聲明
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommandIssued,
    const FMilitaryCommand&, Command,
    const FString&, IssuerID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommandExecuted,
    int32, CommandID,
    const FCommandExecutionResult&, Result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsequenceBearing,
    const FConsequenceBearing&, Consequence);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommandChainUpdated,
    const FString&, CommanderID,
    const FCommandChainNode&, NewNode);

// ============================================================================
// 主系統類別
// ============================================================================

/**
 * 軍事權威系統
 * 負責部隊指揮執行、命令發布系統、後果承擔機制、指揮鏈管理
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|Military Authority")
class MINGGORTS_API UMingMilitaryAuthority : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 構造函數與初始化
    // ============================================================================

    UMingMilitaryAuthority();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 初始化軍事權威系統 */
    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    bool InitializeMilitaryAuthority();

    /** 重置系統到初始狀態 */
    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void ResetMilitaryAuthority();

    // ============================================================================
    // 部隊指揮執行
    // ============================================================================

    /** 發布軍事命令 */
    UFUNCTION(BlueprintCallable, Category = "Command Execution")
    int32 IssueCommand(EMilitaryCommandType CommandType, 
                    const FVector& TargetLocation,
                    ECommandPriority Priority,
                    bool bIsPublic,
                    const FString& Description);

    /** 執行特定命令 */
    UFUNCTION(BlueprintCallable, Category = "Command Execution")
    bool ExecuteCommand(int32 CommandID);

    /** 取消命令 */
    UFUNCTION(BlueprintCallable, Category = "Command Execution")
    bool CancelCommand(int32 CommandID);

    /** 暫停命令 */
    UFUNCTION(BlueprintCallable, Category = "Command Execution")
    bool SuspendCommand(int32 CommandID);

    /** 恢復命令 */
    UFUNCTION(BlueprintCallable, Category = "Command Execution")
    bool ResumeCommand(int32 CommandID);

    /** 獲取活動命令列表 */
    UFUNCTION(BlueprintCallable, Category = "Command Execution")
    TArray<FMilitaryCommand> GetActiveCommands() const;

    /** 獲取命令歷史 */
    UFUNCTION(BlueprintCallable, Category = "Command Execution")
    TArray<FMilitaryCommand> GetCommandHistory(int32 MaxRecords = 100) const;

    // ============================================================================
    // 命令發布系統
    // ============================================================================

    /** 廣播命令給所有單位 */
    UFUNCTION(BlueprintCallable, Category = "Command Publishing")
    bool BroadcastCommand(const FMilitaryCommand& Command);

    /** 發布命令給特定單位組 */
    UFUNCTION(BlueprintCallable, Category = "Command Publishing")
    bool PublishCommandToGroup(const FMilitaryCommand& Command, const TArray<FString>& UnitGroupIDs);

    /** 設置命令可見性 */
    UFUNCTION(BlueprintCallable, Category = "Command Publishing")
    void SetCommandVisibility(int32 CommandID, bool bIsPublic);

    /** 獲取命令發布狀態 */
    UFUNCTION(BlueprintCallable, Category = "Command Publishing")
    bool IsCommandPublished(int32 CommandID) const;

    /** 更新命令狀態 */
    UFUNCTION(BlueprintCallable, Category = "Command Publishing")
    bool UpdateCommandStatus(int32 CommandID, ECommandStatus NewStatus);

    // ============================================================================
    // 後果承擔機制
    // ============================================================================

    /** 分配後果承擔 */
    UFUNCTION(BlueprintCallable, Category = "Consequence Bearing")
    bool AssignConsequenceBearing(int32 CommandID, EConsequenceBearingType BearingType, const FString& ResponsibleParty);

    /** 記錄後果 */
    UFUNCTION(BlueprintCallable, Category = "Consequence Bearing")
    bool RecordConsequence(const FConsequenceBearing& Consequence);

    /** 獲取後果承擔列表 */
    UFUNCTION(BlueprintCallable, Category = "Consequence Bearing")
    TArray<FConsequenceBearing> GetConsequenceBearings() const;

    /** 確認後果承擔 */
    UFUNCTION(BlueprintCallable, Category = "Consequence Bearing")
    bool AcknowledgeConsequence(const FString& ConsequenceID);

    /** 計算後果嚴重性 */
    UFUNCTION(BlueprintCallable, Category = "Consequence Bearing")
    float CalculateConsequenceSeverity(const FConsequenceBearing& Consequence) const;

    // ============================================================================
    // 指揮鏈管理
    // ============================================================================

    /** 添加指揮鏈節點 */
    UFUNCTION(BlueprintCallable, Category = "Command Chain")
    bool AddCommandChainNode(const FCommandChainNode& Node);

    /** 移除指揮鏈節點 */
    UFUNCTION(BlueprintCallable, Category = "Command Chain")
    bool RemoveCommandChainNode(const FString& CommanderID);

    /** 更新指揮鏈節點 */
    UFUNCTION(BlueprintCallable, Category = "Command Chain")
    bool UpdateCommandChainNode(const FCommandChainNode& Node);

    /** 獲取指揮鏈 */
    UFUNCTION(BlueprintCallable, Category = "Command Chain")
    TArray<FCommandChainNode> GetCommandChain() const;

    /** 檢查指揮權限 */
    UFUNCTION(BlueprintCallable, Category = "Command Chain")
    bool HasCommandAuthority(const FString& CommanderID, const FString& SubordinateID) const;

    /** 覆蓋下級命令 */
    UFUNCTION(BlueprintCallable, Category = "Command Chain")
    bool OverrideSubordinateCommand(const FString& CommanderID, int32 SubordinateCommandID);

    /** 獲取指揮層級 */
    UFUNCTION(BlueprintCallable, Category = "Command Chain")
    int32 GetCommandLevel(const FString& CommanderID) const;

    // ============================================================================
    // 事件委託
    // ============================================================================

    /** 命令發布事件 */
    UPROPERTY(BlueprintAssignable, Category = "Military Authority Events")
    FOnCommandIssued OnCommandIssued;

    /** 命令執行事件 */
    UPROPERTY(BlueprintAssignable, Category = "Military Authority Events")
    FOnCommandExecuted OnCommandExecuted;

    /** 後果承擔事件 */
    UPROPERTY(BlueprintAssignable, Category = "Military Authority Events")
    FOnConsequenceBearing OnConsequenceBearing;

    /** 指揮鏈更新事件 */
    UPROPERTY(BlueprintAssignable, Category = "Military Authority Events")
    FOnCommandChainUpdated OnCommandChainUpdated;

protected:
    // ============================================================================
    // 內部狀態
    // ============================================================================

    /** 活動命令列表 */
    UPROPERTY(BlueprintReadOnly, Category = "Military Authority")
    TArray<FMilitaryCommand> ActiveCommands;

    /** 命令歷史 */
    UPROPERTY(BlueprintReadOnly, Category = "Military Authority")
    TArray<FMilitaryCommand> CommandHistory;

    /** 指揮鏈 */
    UPROPERTY(BlueprintReadOnly, Category = "Military Authority")
    TArray<FCommandChainNode> CommandChain;

    /** 後果承擔記錄 */
    UPROPERTY(BlueprintReadOnly, Category = "Military Authority")
    TArray<FConsequenceBearing> ConsequenceBearings;

    /** 系統參數 */
    UPROPERTY(BlueprintReadWrite, Category = "Military Authority")
    int32 NextCommandID;

    UPROPERTY(BlueprintReadWrite, Category = "Military Authority")
    int32 MaxActiveCommands;

    UPROPERTY(BlueprintReadWrite, Category = "Military Authority")
    int32 MaxHistorySize;

    /** 系統狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Military Authority")
    bool bIsInitialized;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 驗證命令有效性 */
    bool ValidateCommand(const FMilitaryCommand& Command) const;

    /** 執行命令邏輯 */
    FCommandExecutionResult ExecuteCommandLogic(const FMilitaryCommand& Command);

    /** 更新命令歷史 */
    void UpdateCommandHistory(const FMilitaryCommand& Command);

    /** 清理過期命令 */
    void CleanupExpiredCommands();

    /** 計算命令執行時間 */
    float CalculateCommandExecutionTime(const FMilitaryCommand& Command) const;

    /** 分配責任鏈 */
    void AssignResponsibilityChain(const FConsequenceBearing& Consequence);

    /** 檢查指揮鏈一致性 */
    bool ValidateCommandChain() const;

    /** 觸發命令事件 */
    void TriggerCommandEvent(const FMilitaryCommand& Command, const FString& EventType);

    /** 觸發後果事件 */
    void TriggerConsequenceEvent(const FConsequenceBearing& Consequence);

private:
    /** 定時器句柄 */
    FTimerHandle CommandUpdateTimer;

    /** 最大命令ID */
    static constexpr int32 MAX_COMMAND_ID = 999999;

    /** 命令更新間隔 */
    static constexpr float COMMAND_UPDATE_INTERVAL = 0.1f;
};
