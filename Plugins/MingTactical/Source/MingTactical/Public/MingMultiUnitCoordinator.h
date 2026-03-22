#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingMultiUnitCoordinator.generated.h"

class AMingTacticalUnit;
class AMingUnitController;

/**
 * 單位分組類型
 */
UENUM(BlueprintType)
enum class EUnitGroupType : uint8
{
    Squad               UMETA(DisplayName = "Squad"),
    Platoon             UMETA(DisplayName = "Platoon"),
    Company             UMETA(DisplayName = "Company"),
    Battalion           UMETA(DisplayName = "Battalion"),
    Regiment            UMETA(DisplayName = "Regiment"),
    Brigade             UMETA(DisplayName = "Brigade"),
    Division            UMETA(DisplayName = "Division"),
    Corps               UMETA(DisplayName = "Corps"),
    Army                UMETA(DisplayName = "Army"),
    TaskForce           UMETA(DisplayName = "Task Force"),
    SpecialForces       UMETA(DisplayName = "Special Forces"),
    Reserve             UMETA(DisplayName = "Reserve"),
    Support             UMETA(DisplayName = "Support"),
    Logistics           UMETA(DisplayName = "Logistics"),
    Custom              UMETA(DisplayName = "Custom")
};

/**
 * 協調命令類型
 */
UENUM(BlueprintType)
enum class ECoordinatedCommandType : uint8
{
    CoordinatedAttack    UMETA(DisplayName = "Coordinated Attack"),
    CoordinatedDefense   UMETA(DisplayName = "Coordinated Defense"),
    CoordinatedMovement  UMETA(DisplayName = "Coordinated Movement"),
    CoordinatedRetreat   UMETA(DisplayName = "Coordinated Retreat"),
    CoordinatedSupport   UMETA(DisplayName = "Coordinated Support"),
    CoordinatedAmbush    UMETA(DisplayName = "Coordinated Ambush"),
    CoordinatedFlanking  UMETA(DisplayName = "Coordinated Flanking"),
    CoordinatedEncirclement UMETA(DisplayName = "Coordinated Encirclement"),
    CoordinatedBreakthrough UMETA(DisplayName = "Coordinated Breakthrough"),
    CoordinatedWithdrawal UMETA(DisplayName = "Coordinated Withdrawal"),
    CoordinatedReinforcement UMETA(DisplayName = "Coordinated Reinforcement"),
    CoordinatedResupply   UMETA(DisplayName = "Coordinated Resupply"),
    CoordinatedEvacuation UMETA(DisplayName = "Coordinated Evacuation"),
    CoordinatedSecuring   UMETA(DisplayName = "Coordinated Securing"),
    CoordinatedHarassment UMETA(DisplayName = "Coordinated Harassment"),
    CoordinatedDistraction UMETA(DisplayName = "Coordinated Distraction")
};

/**
 * 協調同步類型
 */
UENUM(BlueprintType)
enum class ECoordinationSyncType : uint8
{
    Simultaneous        UMETA(DisplayName = "Simultaneous"),
    Sequential          UMETA(DisplayName = "Sequential"),
    Staggered           UMETA(DisplayName = "Staggered"),
    Wave                UMETA(DisplayName = "Wave"),
    Phased              UMETA(DisplayName = "Phased"),
    Conditional         UMETA(DisplayName = "Conditional"),
    EventDriven         UMETA(DisplayName = "Event Driven"),
    TimeBased           UMETA(DisplayName = "Time Based"),
    PriorityBased       UMETA(DisplayName = "Priority Based"),
    Adaptive            UMETA(DisplayName = "Adaptive"),
    Reactive            UMETA(DisplayName = "Reactive"),
    Proactive           UMETA(DisplayName = "Proactive")
};

/**
 * 通信協議類型
 */
UENUM(BlueprintType)
enum class ECommunicationProtocol : uint8
{
    Direct              UMETA(DisplayName = "Direct"),
    Relay               UMETA(DisplayName = "Relay"),
    Broadcast           UMETA(DisplayName = "Broadcast"),
    Multicast           UMETA(DisplayName = "Multicast"),
    Hierarchical        UMETA(DisplayName = "Hierarchical"),
    PeerToPeer          UMETA(DisplayName = "Peer to Peer"),
    Centralized         UMETA(DisplayName = "Centralized"),
    Decentralized       UMETA(DisplayName = "Decentralized"),
    Hybrid              UMETA(DisplayName = "Hybrid"),
    Secure              UMETA(DisplayName = "Secure"),
    Encrypted           UMETA(DisplayName = "Encrypted"),
    Emergency           UMETA(DisplayName = "Emergency")
};

/**
 * 單位分組定義
 */
USTRUCT(BlueprintType)
struct FUnitGroup
{
    GENERATED_BODY()

    // 分組ID
    UPROPERTY(BlueprintReadOnly)
    FString GroupID;

    // 分組名稱
    UPROPERTY(BlueprintReadOnly)
    FString GroupName;

    // 分組類型
    UPROPERTY(BlueprintReadOnly)
    EUnitGroupType GroupType;

    // 分組描述
    UPROPERTY(BlueprintReadOnly)
    FString GroupDescription;

    // 分組單位
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> GroupUnits;

    // 分組領導者
    UPROPERTY(BlueprintReadOnly)
    AMingTacticalUnit* GroupLeader;

    // 分組等級
    UPROPERTY(BlueprintReadOnly)
    int32 GroupLevel;

    // 分組優先級
    UPROPERTY(BlueprintReadOnly)
    int32 GroupPriority;

    // 分組狀態
    UPROPERTY(BlueprintReadOnly)
    FString GroupStatus;

    // 分組標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> GroupTags;

    // 分組屬性
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> GroupAttributes;

    // 創建時間
    UPROPERTY(BlueprintReadOnly)
    float CreationTime;

    // 最後更新時間
    UPROPERTY(BlueprintReadOnly)
    float LastUpdateTime;

    // 是否活躍
    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    // 是否可解散
    UPROPERTY(BlueprintReadOnly)
    bool bCanDisband;

    FUnitGroup()
        : GroupID(TEXT(""))
        , GroupName(TEXT(""))
        , GroupType(EUnitGroupType::Squad)
        , GroupDescription(TEXT(""))
        , GroupLeader(nullptr)
        , GroupLevel(1)
        , GroupPriority(1)
        , GroupStatus(TEXT("Active"))
        , CreationTime(0.0f)
        , LastUpdateTime(0.0f)
        , bIsActive(true)
        , bCanDisband(true)
    {}
};

/**
 * 協調命令定義
 */
USTRUCT(BlueprintType)
struct FCoordinatedCommand
{
    GENERATED_BODY()

    // 命令ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // 命令名稱
    UPROPERTY(BlueprintReadOnly)
    FString CommandName;

    // 命令類型
    UPROPERTY(BlueprintReadOnly)
    ECoordinatedCommandType CommandType;

    // 命令描述
    UPROPERTY(BlueprintReadOnly)
    FString CommandDescription;

    // 參與分組
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ParticipatingGroups;

    // 參與單位
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> ParticipatingUnits;

    // 命令領導者
    UPROPERTY(BlueprintReadOnly)
    AMingTacticalUnit* CommandLeader;

    // 同步類型
    UPROPERTY(BlueprintReadOnly)
    ECoordinationSyncType SyncType;

    // 通信協議
    UPROPERTY(BlueprintReadOnly)
    ECommunicationProtocol CommunicationProtocol;

    // 命令階段
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CommandPhases;

    // 當前階段
    UPROPERTY(BlueprintReadOnly)
    FString CurrentPhase;

    // 命令參數
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CommandParameters;

    // 執行時間表
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> ExecutionSchedule;

    // 同步點
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SyncPoints;

    // 依賴關係
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Dependencies;

    // 命令狀態
    UPROPERTY(BlueprintReadOnly)
    FString CommandStatus;

    // 創建時間
    UPROPERTY(BlueprintReadOnly)
    float CreationTime;

    // 開始時間
    UPROPERTY(BlueprintReadOnly)
    float StartTime;

    // 完成時間
    UPROPERTY(BlueprintReadOnly)
    float CompletionTime;

    // 是否已執行
    UPROPERTY(BlueprintReadOnly)
    bool bIsExecuted;

    // 是否已完成
    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;

    // 是否已取消
    UPROPERTY(BlueprintReadOnly)
    bool bIsCancelled;

    FCoordinatedCommand()
        : CommandID(TEXT(""))
        , CommandName(TEXT(""))
        , CommandType(ECoordinatedCommandType::CoordinatedAttack)
        , CommandDescription(TEXT(""))
        , CommandLeader(nullptr)
        , SyncType(ECoordinationSyncType::Simultaneous)
        , CommunicationProtocol(ECommunicationProtocol::Direct)
        , CurrentPhase(TEXT("Planning"))
        , CommandStatus(TEXT("Planning"))
        , CreationTime(0.0f)
        , StartTime(0.0f)
        , CompletionTime(0.0f)
        , bIsExecuted(false)
        , bIsCompleted(false)
        , bIsCancelled(false)
    {}
};

/**
 * 協調結果定義
 */
USTRUCT(BlueprintType)
struct FCoordinationResult
{
    GENERATED_BODY()

    // 是否成功
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 協調ID
    UPROPERTY(BlueprintReadOnly)
    FString CoordinationID;

    // 命令ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // 執行時間
    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    // 結果描述
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;

    // 參與分組
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ParticipatingGroups;

    // 參與單位
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> ParticipatingUnits;

    // 成功單位
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> SuccessfulUnits;

    // 失敗單位
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> FailedUnits;

    // 協調效果
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CoordinationEffects;

    // 協調統計
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> CoordinationStatistics;

    // 協調分數
    UPROPERTY(BlueprintReadOnly)
    float CoordinationScore;

    // 同步精度
    UPROPERTY(BlueprintReadOnly)
    float SyncAccuracy;

    // 通信質量
    UPROPERTY(BlueprintReadOnly)
    float CommunicationQuality;

    // 錯誤信息
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    FCoordinationResult()
        : bSuccess(false)
        , CoordinationID(TEXT(""))
        , CommandID(TEXT(""))
        , ExecutionTime(0.0f)
        , ResultDescription(TEXT(""))
        , CoordinationScore(0.0f)
        , SyncAccuracy(0.0f)
        , CommunicationQuality(0.0f)
        , ErrorMessage(TEXT(""))
    {}
};

/**
 * 多單位協調器
 * 管理大規模單位的協調和同步
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingMultiUnitCoordinator : public AActor
{
    GENERATED_BODY()

public:
    AMingMultiUnitCoordinator();

    /**
     * 初始化多單位協調器
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    void InitializeMultiUnitCoordinator();

    /**
     * 關閉多單位協調器
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    void ShutdownMultiUnitCoordinator();

    /**
     * 創建單位分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FUnitGroup CreateUnitGroup(
        const FString& GroupName,
        EUnitGroupType GroupType,
        const TArray<AMingTacticalUnit*>& Units,
        AMingTacticalUnit* Leader = nullptr
    );

    /**
     * 解散單位分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool DisbandUnitGroup(const FString& GroupID);

    /**
     * 獲取單位分組
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    FUnitGroup GetUnitGroup(const FString& GroupID) const;

    /**
     * 獲取所有單位分組
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    TArray<FUnitGroup> GetAllUnitGroups() const;

    /**
     * 獲取指定類型的分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    TArray<FUnitGroup> GetGroupsByType(EUnitGroupType GroupType) const;

    /**
     * 獲取活躍分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    TArray<FUnitGroup> GetActiveGroups() const;

    /**
     * 添加單位到分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool AddUnitToGroup(const FString& GroupID, AMingTacticalUnit* Unit);

    /**
     * 從分組移除單位
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool RemoveUnitFromGroup(const FString& GroupID, AMingTacticalUnit* Unit);

    /**
     * 設置分組領導者
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool SetGroupLeader(const FString& GroupID, AMingTacticalUnit* Leader);

    /**
     * 發布協調命令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult IssueCoordinatedCommand(const FCoordinatedCommand& Command);

    /**
     * 執行協調攻擊
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedAttack(
        const TArray<FString>& GroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調防禦
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedDefense(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& DefensePositions,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調移動
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedMovement(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& TargetPositions,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調撤退
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedRetreat(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& RetreatPositions,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調支援
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedSupport(
        const TArray<FString>& SupportGroupIDs,
        const TArray<FString>& TargetGroupIDs,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調伏擊
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedAmbush(
        const TArray<FString>& AmbusherGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調側翼攻擊
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedFlanking(
        const TArray<FString>& FlankerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調包圍
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedEncirclement(
        const TArray<FString>& EncirclerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    );

    /**
     * 執行協調突破
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedBreakthrough(
        const TArray<FString>& BreakthroughGroupIDs,
        const TArray<FVector>& BreakthroughPoints,
        ECoordinationSyncType SyncType
    );

    /**
     * 取消協調命令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool CancelCoordinatedCommand(const FString& CommandID);

    /**
     * 獲取協調命令
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    FCoordinatedCommand GetCoordinatedCommand(const FString& CommandID) const;

    /**
     * 獲取所有協調命令
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    TArray<FCoordinatedCommand> GetAllCoordinatedCommands() const;

    /**
     * 獲取執行中的命令
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    TArray<FCoordinatedCommand> GetExecutingCommands() const;

    /**
     * 檢查協調狀態
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    FString GetCoordinationStatus(const FString& CommandID) const;

    /**
     * 獲取協調統計
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    TMap<FString, int32> GetCoordinationStatistics() const;

    /**
     * 獲取協調性能指標
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    TMap<FString, float> GetCoordinationPerformanceMetrics() const;

    /**
     * 同步單位狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool SynchronizeUnitStates(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 廣播消息到分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool BroadcastToGroup(const FString& GroupID, const FString& Message);

    /**
     * 中繼消息到分組
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool RelayMessage(const FString& SourceGroupID, const FString& TargetGroupID, const FString& Message);

    /**
     * 建立通信連接
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool EstablishCommunication(const FString& GroupAID, const FString& GroupBID, ECommunicationProtocol Protocol);

    /**
     * 斷開通信連接
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool DisconnectCommunication(const FString& GroupAID, const FString& GroupBID);

    /**
     * 獲取通信質量
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    float GetCommunicationQuality(const FString& GroupAID, const FString& GroupBID) const;

protected:
    // 單位分組映射
    UPROPERTY()
    TMap<FString, FUnitGroup> UnitGroups;

    // 協調命令映射
    UPROPERTY()
    TMap<FString, FCoordinatedCommand> CoordinatedCommands;

    // 通信連接映射
    UPROPERTY()
    TMap<FString, TMap<FString, ECommunicationProtocol>> CommunicationConnections;

    // 協調統計
    UPROPERTY()
    TMap<FString, int32> CoordinationStatistics;

    // 性能指標
    UPROPERTY()
    TMap<FString, float> PerformanceMetrics;

    // 單位控制器引用
    UPROPERTY()
    AMingUnitController* UnitController;

    // 是否已初始化
    bool bIsInitialized;

    // Tick更新
    virtual void Tick(float DeltaTime) override;

    // 執行協調命令
    void ExecuteCoordinatedCommand(const FCoordinatedCommand& Command);

    // 處理同步執行
    void HandleSimultaneousExecution(const FCoordinatedCommand& Command);

    // 處理順序執行
    void HandleSequentialExecution(const FCoordinatedCommand& Command);

    // 處理交錯執行
    void HandleStaggeredExecution(const FCoordinatedCommand& Command);

    // 處理波浪執行
    void HandleWaveExecution(const FCoordinatedCommand& Command);

    // 處理階段執行
    void HandlePhasedExecution(const FCoordinatedCommand& Command);

    // 處理條件執行
    void HandleConditionalExecution(const FCoordinatedCommand& Command);

    // 處理事件驅動執行
    void HandleEventDrivenExecution(const FCoordinatedCommand& Command);

    // 檢查同步條件
    bool CheckSyncConditions(const FCoordinatedCommand& Command) const;

    // 計算同步精度
    float CalculateSyncAccuracy(const FCoordinatedCommand& Command) const;

    // 計算通信質量
    float CalculateCommunicationQuality(const FString& GroupAID, const FString& GroupBID) const;

    // 更新分組狀態
    void UpdateGroupStatus(const FString& GroupID);

    // 更新協調統計
    void UpdateCoordinationStatistics(const FString& CommandID, const FCoordinationResult& Result);

    // 更新性能指標
    void UpdatePerformanceMetrics();

    // 驗證分組配置
    bool ValidateGroupConfig(const FUnitGroup& Group) const;

    // 驗證協調命令
    bool ValidateCoordinatedCommand(const FCoordinatedCommand& Command) const;

    // 獲取分組描述
    FString GetGroupDescription(const FString& GroupID) const;

    // 獲取命令描述
    FString GetCommandDescription(const FString& CommandID) const;

    // 獲取同步類型描述
    FString GetSyncTypeDescription(ECoordinationSyncType SyncType) const;

    // 獲取通信協議描述
    FString GetProtocolDescription(ECommunicationProtocol Protocol) const;

    // 記錄協調事件
    void LogCoordinationEvent(const FString& EventType, const FString& Details);

    // 生成協調報告
    FString GenerateCoordinationReport(const FString& CommandID) const;

    // 計算協調分數
    float CalculateCoordinationScore(const FCoordinationResult& Result) const;

    // 處理通信延遲
    float HandleCommunicationDelay(const FString& SourceGroupID, const FString& TargetGroupID) const;

    // 處理通信干擾
    float HandleCommunicationInterference(const FString& SourceGroupID, const FString& TargetGroupID) const;

    // 處理通信加密
    bool HandleCommunicationEncryption(const FString& SourceGroupID, const FString& TargetGroupID) const;

    // 獲取最佳協調策略
    ECoordinationSyncType GetOptimalSyncType(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType) const;

    // 獲取最佳通信協議
    ECommunicationProtocol GetOptimalCommunicationProtocol(const TArray<FString>& GroupIDs) const;

    // 計算分組協調能力
    float CalculateGroupCoordinationCapability(const FString& GroupID) const;

    // 計算整體協調效率
    float CalculateOverallCoordinationEfficiency() const;

    // 處理協調失敗
    void HandleCoordinationFailure(const FString& CommandID, const FString& FailureReason);

    // 處理協調成功
    void HandleCoordinationSuccess(const FString& CommandID);

    // 清理已完成的協調
    void CleanupCompletedCoordinations();

    // 備份協調數據
    void BackupCoordinationData();

    // 恢復協調數據
    void RestoreCoordinationData();
};
