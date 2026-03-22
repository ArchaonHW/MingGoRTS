#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingMultiUnitCoordinator.generated.h"

class AMingTacticalUnit;
class AMingUnitController;

/**
 * ?Æ‰??ÜÁ?È°ûÂ?
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
 * ?îË™ø?Ω‰ª§È°ûÂ?
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
 * ?îË™ø?åÊ≠•È°ûÂ?
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
 * ?ö‰ø°?îË≠∞È°ûÂ?
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
 * ?Æ‰??ÜÁ?ÂÆöÁæ©
 */
USTRUCT(BlueprintType)
struct FUnitGroup
{
    GENERATED_BODY()

    // ?ÜÁ?ID
    UPROPERTY(BlueprintReadOnly)
    FString GroupID;

    // ?ÜÁ??çÁ®±
    UPROPERTY(BlueprintReadOnly)
    FString GroupName;

    // ?ÜÁ?È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    EUnitGroupType GroupType;

    // ?ÜÁ??èËø∞
    UPROPERTY(BlueprintReadOnly)
    FString GroupDescription;

    // ?ÜÁ??Æ‰?
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> GroupUnits;

    // ?ÜÁ??òÂ???    UPROPERTY(BlueprintReadOnly)
    AMingTacticalUnit* GroupLeader;

    // ?ÜÁ?Á≠âÁ?
    UPROPERTY(BlueprintReadOnly)
    int32 GroupLevel;

    // ?ÜÁ??™Â?Á¥?    UPROPERTY(BlueprintReadOnly)
    int32 GroupPriority;

    // ?ÜÁ??Ä??    UPROPERTY(BlueprintReadOnly)
    FString GroupStatus;

    // ?ÜÁ?Ê®ôÁ±§
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> GroupTags;

    // ?ÜÁ?Â±¨ÊÄ?    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> GroupAttributes;

    // ?µÂª∫?ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float CreationTime;

    // ?ÄÂæåÊõ¥?∞Ê???    UPROPERTY(BlueprintReadOnly)
    float LastUpdateTime;

    // ?ØÂê¶Ê¥ªË?
    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    // ?ØÂê¶?ØËß£??    UPROPERTY(BlueprintReadOnly)
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
 * ?îË™ø?Ω‰ª§ÂÆöÁæ©
 */
USTRUCT(BlueprintType)
struct FCoordinatedCommand
{
    GENERATED_BODY()

    // ?Ω‰ª§ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // ?Ω‰ª§?çÁ®±
    UPROPERTY(BlueprintReadOnly)
    FString CommandName;

    // ?Ω‰ª§È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    ECoordinatedCommandType CommandType;

    // ?Ω‰ª§?èËø∞
    UPROPERTY(BlueprintReadOnly)
    FString CommandDescription;

    // ?ÉË??ÜÁ?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ParticipatingGroups;

    // ?ÉË??Æ‰?
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> ParticipatingUnits;

    // ?Ω‰ª§?òÂ???    UPROPERTY(BlueprintReadOnly)
    AMingTacticalUnit* CommandLeader;

    // ?åÊ≠•È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    ECoordinationSyncType SyncType;

    // ?ö‰ø°?îË≠∞
    UPROPERTY(BlueprintReadOnly)
    ECommunicationProtocol CommunicationProtocol;

    // ?Ω‰ª§?éÊÆµ
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CommandPhases;

    // ?∂Â??éÊÆµ
    UPROPERTY(BlueprintReadOnly)
    FString CurrentPhase;

    // ?Ω‰ª§?ÉÊï∏
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CommandParameters;

    // ?∑Ë??ÇÈ?Ë°?    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> ExecutionSchedule;

    // ?åÊ≠•Èª?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SyncPoints;

    // ‰æùË≥¥?ú‰?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Dependencies;

    // ?Ω‰ª§?Ä??    UPROPERTY(BlueprintReadOnly)
    FString CommandStatus;

    // ?µÂª∫?ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float CreationTime;

    // ?ãÂ??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float StartTime;

    // ÂÆåÊ??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float CompletionTime;

    // ?ØÂê¶Â∑≤Âü∑Ë°?    UPROPERTY(BlueprintReadOnly)
    bool bIsExecuted;

    // ?ØÂê¶Â∑≤Â???    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;

    // ?ØÂê¶Â∑≤Â?Ê∂?    UPROPERTY(BlueprintReadOnly)
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
 * ?îË™øÁµêÊ?ÂÆöÁæ©
 */
USTRUCT(BlueprintType)
struct FCoordinationResult
{
    GENERATED_BODY()

    // ?ØÂê¶?êÂ?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // ?îË™øID
    UPROPERTY(BlueprintReadOnly)
    FString CoordinationID;

    // ?Ω‰ª§ID
    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    // ?∑Ë??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    // ÁµêÊ??èËø∞
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;

    // ?ÉË??ÜÁ?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ParticipatingGroups;

    // ?ÉË??Æ‰?
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> ParticipatingUnits;

    // ?êÂ??Æ‰?
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> SuccessfulUnits;

    // Â§±Ê??Æ‰?
    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> FailedUnits;

    // ?îË™ø?àÊ?
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CoordinationEffects;

    // ?îË™øÁµ±Ë?
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> CoordinationStatistics;

    // ?îË™ø?ÜÊï∏
    UPROPERTY(BlueprintReadOnly)
    float CoordinationScore;

    // ?åÊ≠•Á≤æÂ∫¶
    UPROPERTY(BlueprintReadOnly)
    float SyncAccuracy;

    // ?ö‰ø°Ë≥™È?
    UPROPERTY(BlueprintReadOnly)
    float CommunicationQuality;

    // ?ØË™§‰ø°ÊÅØ
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
 * Â§öÂñÆ‰ΩçÂ?Ë™øÂô®
 * ÁÆ°Á?Â§ßË?Ê®°ÂñÆ‰ΩçÁ??îË™ø?åÂ?Ê≠? */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingMultiUnitCoordinator : public AActor
{
    GENERATED_BODY()

public:
    AMingMultiUnitCoordinator(};

    /**
     * ?ùÂ??ñÂ??Æ‰??îË™ø??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    void InitializeMultiUnitCoordinator(};

    /**
     * ?úÈ?Â§öÂñÆ‰ΩçÂ?Ë™øÂô®
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    void ShutdownMultiUnitCoordinator(};

    /**
     * ?µÂª∫?Æ‰??ÜÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FUnitGroup CreateUnitGroup(
        const FString& GroupName,
        EUnitGroupType GroupType,
        const TArray<AMingTacticalUnit*>& Units,
        AMingTacticalUnit* Leader = nullptr
    };

    /**
     * Ëß?ï£?Æ‰??ÜÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool DisbandUnitGroup(const FString& GroupID};

    /**
     * ?≤Â??Æ‰??ÜÁ?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    FUnitGroup GetUnitGroup(const FString& GroupID) const;

    /**
     * ?≤Â??Ä?âÂñÆ‰ΩçÂ?Áµ?     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    TArray<FUnitGroup> GetAllUnitGroups() const;

    /**
     * ?≤Â??áÂ?È°ûÂ??ÑÂ?Áµ?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    TArray<FUnitGroup> GetGroupsByType(EUnitGroupType GroupType) const;

    /**
     * ?≤Â?Ê¥ªË??ÜÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    TArray<FUnitGroup> GetActiveGroups() const;

    /**
     * Ê∑ªÂ??Æ‰??∞Â?Áµ?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool AddUnitToGroup(const FString& GroupID, AMingTacticalUnit* Unit};

    /**
     * ÂæûÂ?ÁµÑÁßª?§ÂñÆ‰Ω?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool RemoveUnitFromGroup(const FString& GroupID, AMingTacticalUnit* Unit};

    /**
     * Ë®≠ÁΩÆ?ÜÁ??òÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool SetGroupLeader(const FString& GroupID, AMingTacticalUnit* Leader};

    /**
     * ?ºÂ??îË™ø?Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult IssueCoordinatedCommand(const FCoordinatedCommand& Command};

    /**
     * ?∑Ë??îË™ø?ªÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedAttack(
        const TArray<FString>& GroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    };

    /**
     * ?∑Ë??îË™ø?≤Á¶¶
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedDefense(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& DefensePositions,
        ECoordinationSyncType SyncType
    };

    /**
     * ?∑Ë??îË™øÁßªÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedMovement(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& TargetPositions,
        ECoordinationSyncType SyncType
    };

    /**
     * ?∑Ë??îË™ø?§ÈÄÄ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedRetreat(
        const TArray<FString>& GroupIDs,
        const TArray<FVector>& RetreatPositions,
        ECoordinationSyncType SyncType
    };

    /**
     * ?∑Ë??îË™ø?ØÊè¥
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedSupport(
        const TArray<FString>& SupportGroupIDs,
        const TArray<FString>& TargetGroupIDs,
        ECoordinationSyncType SyncType
    };

    /**
     * ?∑Ë??îË™ø‰ºèÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedAmbush(
        const TArray<FString>& AmbusherGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    };

    /**
     * ?∑Ë??îË™ø?¥Áøº?ªÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedFlanking(
        const TArray<FString>& FlankerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    };

    /**
     * ?∑Ë??îË™ø?ÖÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedEncirclement(
        const TArray<FString>& EncirclerGroupIDs,
        const TArray<AMingTacticalUnit*>& Targets,
        ECoordinationSyncType SyncType
    };

    /**
     * ?∑Ë??îË™øÁ™ÅÁ†¥
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    FCoordinationResult ExecuteCoordinatedBreakthrough(
        const TArray<FString>& BreakthroughGroupIDs,
        const TArray<FVector>& BreakthroughPoints,
        ECoordinationSyncType SyncType
    };

    /**
     * ?ñÊ??îË™ø?Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool CancelCoordinatedCommand(const FString& CommandID};

    /**
     * ?≤Â??îË™ø?Ω‰ª§
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    FCoordinatedCommand GetCoordinatedCommand(const FString& CommandID) const;

    /**
     * ?≤Â??Ä?âÂ?Ë™øÂëΩ‰ª?     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    TArray<FCoordinatedCommand> GetAllCoordinatedCommands() const;

    /**
     * ?≤Â??∑Ë?‰∏≠Á??Ω‰ª§
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    TArray<FCoordinatedCommand> GetExecutingCommands() const;

    /**
     * Ê™¢Êü•?îË™ø?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    FString GetCoordinationStatus(const FString& CommandID) const;

    /**
     * ?≤Â??îË™øÁµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    TMap<FString, int32> GetCoordinationStatistics() const;

    /**
     * ?≤Â??îË™ø?ßËÉΩ?áÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    TMap<FString, float> GetCoordinationPerformanceMetrics() const;

    /**
     * ?åÊ≠•?Æ‰??Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool SynchronizeUnitStates(const TArray<AMingTacticalUnit*>& Units};

    /**
     * Âª?í≠Ê∂àÊÅØ?∞Â?Áµ?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool BroadcastToGroup(const FString& GroupID, const FString& Message};

    /**
     * ‰∏≠ÁπºÊ∂àÊÅØ?∞Â?Áµ?     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool RelayMessage(const FString& SourceGroupID, const FString& TargetGroupID, const FString& Message};

    /**
     * Âª∫Á??ö‰ø°??é•
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool EstablishCommunication(const FString& GroupAID, const FString& GroupBID, ECommunicationProtocol Protocol};

    /**
     * ?∑È??ö‰ø°??é•
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator")
    bool DisconnectCommunication(const FString& GroupAID, const FString& GroupBID};

    /**
     * ?≤Â??ö‰ø°Ë≥™È?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator")
    float GetCommunicationQuality(const FString& GroupAID, const FString& GroupBID) const;

    // ========== ‰∏¶Ë??ñÂñÆ‰ΩçÂ?Ë™?(Parallel Processing) ==========
    
    /**
     * ‰∏¶Ë?Ë®àÁ??Æ‰?ÁßªÂ?Ë∑ØÂ? (ParallelFor)
     * ?©Áî®?ºÂ§ßË¶èÊ®°?Æ‰?Áæ§È?ÁßªÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Parallel")
    void CalculateMovementPathsParallel(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation};

    /**
     * ‰∏¶Ë??¥Êñ∞?Æ‰??ÜÁ??Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Parallel")
    void UpdateAllGroupStatusesParallel(};

    /**
     * ‰∏¶Ë?È©óË??Æ‰?Á∑®È?‰ΩçÁΩÆ
     */
    UFUNCTION(BlueprintCallable, Category = "Multi Unit Coordinator|Parallel")
    void ValidateFormationPositionsParallel(const FString& GroupID};

    /**
     * ?≤Â?‰∏äÊ¨°‰∏¶Ë??ïÁ??ÇÈ?
     */
    UFUNCTION(BlueprintPure, Category = "Multi Unit Coordinator|Parallel")
    float GetLastParallelProcessingTimeMs() const { return LastParallelProcessingTimeMs; }

protected:
    // ?Æ‰??ÜÁ??†Â?
    UPROPERTY()
    TMap<FString, FUnitGroup> UnitGroups;

    // ?îË™ø?Ω‰ª§?†Â?
    UPROPERTY()
    TMap<FString, FCoordinatedCommand> CoordinatedCommands;

    // ?ö‰ø°??é•?†Â? - Ê≥®Ê?ÔºöÂ?Â•?TMap ‰∏çÊîØ??UPROPERTY
    TMap<FString, TMap<FString, ECommunicationProtocol>> CommunicationConnections;

    // ?îË™øÁµ±Ë?
    UPROPERTY()
    TMap<FString, int32> CoordinationStatistics;

    // ?ßËÉΩ?áÊ?
    UPROPERTY()
    TMap<FString, float> PerformanceMetrics;

    // ?Æ‰??ßÂà∂?®Â???    UPROPERTY()
    AMingUnitController* UnitController;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ‰∏¶Ë??ñË??ÜÊ??ìË???    float LastParallelProcessingTimeMs = 0.0f;

    // ?∑Ë?Á∑íÂ??®È?
    FCriticalSection ParallelLock;

    // Tick?¥Êñ∞
    virtual void Tick(float DeltaTime) override;

    // ?∑Ë??îË™ø?Ω‰ª§
    void ExecuteCoordinatedCommand(const FCoordinatedCommand& Command};

    // ?ïÁ??åÊ≠•?∑Ë?
    void HandleSimultaneousExecution(const FCoordinatedCommand& Command};

    // ?ïÁ??ÜÂ??∑Ë?
    void HandleSequentialExecution(const FCoordinatedCommand& Command};

    // ?ïÁ?‰∫§ÈåØ?∑Ë?
    void HandleStaggeredExecution(const FCoordinatedCommand& Command};

    // ?ïÁ?Ê≥¢Êµ™?∑Ë?
    void HandleWaveExecution(const FCoordinatedCommand& Command};

    // ?ïÁ??éÊÆµ?∑Ë?
    void HandlePhasedExecution(const FCoordinatedCommand& Command};

    // ?ïÁ?Ê¢ù‰ª∂?∑Ë?
    void HandleConditionalExecution(const FCoordinatedCommand& Command};

    // ?ïÁ?‰∫ã‰ª∂È©ÖÂ??∑Ë?
    void HandleEventDrivenExecution(const FCoordinatedCommand& Command};

    // Ê™¢Êü•?åÊ≠•Ê¢ù‰ª∂
    bool CheckSyncConditions(const FCoordinatedCommand& Command) const;

    // Ë®àÁ??åÊ≠•Á≤æÂ∫¶
    float CalculateSyncAccuracy(const FCoordinatedCommand& Command) const;

    // Ë®àÁ??ö‰ø°Ë≥™È?
    float CalculateCommunicationQuality(const FString& GroupAID, const FString& GroupBID) const;

    // ?¥Êñ∞?ÜÁ??Ä??    void UpdateGroupStatus(const FString& GroupID};

    // ?¥Êñ∞?îË™øÁµ±Ë?
    void UpdateCoordinationStatistics(const FString& CommandID, const FCoordinationResult& Result};

    // ?¥Êñ∞?ßËÉΩ?áÊ?
    void UpdatePerformanceMetrics(};

    // È©óË??ÜÁ??çÁΩÆ
    bool ValidateGroupConfig(const FUnitGroup& Group) const;

    // È©óË??îË™ø?Ω‰ª§
    bool ValidateCoordinatedCommand(const FCoordinatedCommand& Command) const;

    // ?≤Â??ÜÁ??èËø∞
    FString GetGroupDescription(const FString& GroupID) const;

    // ?≤Â??Ω‰ª§?èËø∞
    FString GetCommandDescription(const FString& CommandID) const;

    // ?≤Â??åÊ≠•È°ûÂ??èËø∞
    FString GetSyncTypeDescription(ECoordinationSyncType SyncType) const;

    // ?≤Â??ö‰ø°?îË≠∞?èËø∞
    FString GetProtocolDescription(ECommunicationProtocol Protocol) const;

    // Ë®òÈ??îË™ø‰∫ã‰ª∂
    void LogCoordinationEvent(const FString& EventType, const FString& Details};

    // ?üÊ??îË™ø?±Â?
    FString GenerateCoordinationReport(const FString& CommandID) const;

    // Ë®àÁ??îË™ø?ÜÊï∏
    float CalculateCoordinationScore(const FCoordinationResult& Result) const;

    // ?ïÁ??ö‰ø°Âª∂ÈÅ≤
    float HandleCommunicationDelay(const FString& SourceGroupID, const FString& TargetGroupID) const;

    // ?ïÁ??ö‰ø°Âπ≤Êìæ
    float HandleCommunicationInterference(const FString& SourceGroupID, const FString& TargetGroupID) const;

    // ?ïÁ??ö‰ø°?†Â?
    bool HandleCommunicationEncryption(const FString& SourceGroupID, const FString& TargetGroupID) const;

    // ?≤Â??Ä‰Ω≥Â?Ë™øÁ???    ECoordinationSyncType GetOptimalSyncType(const TArray<FString>& GroupIDs, ECoordinatedCommandType CommandType) const;

    // ?≤Â??Ä‰Ω≥ÈÄö‰ø°?îË≠∞
    ECommunicationProtocol GetOptimalCommunicationProtocol(const TArray<FString>& GroupIDs) const;

    // Ë®àÁ??ÜÁ??îË™ø?ΩÂ?
    float CalculateGroupCoordinationCapability(const FString& GroupID) const;

    // Ë®àÁ??¥È??îË™ø?àÁ?
    float CalculateOverallCoordinationEfficiency() const;

    // ?ïÁ??îË™øÂ§±Ê?
    void HandleCoordinationFailure(const FString& CommandID, const FString& FailureReason};

    // ?ïÁ??îË™ø?êÂ?
    void HandleCoordinationSuccess(const FString& CommandID};

    // Ê∏ÖÁ?Â∑≤Â??êÁ??îË™ø
    void CleanupCompletedCoordinations(};

    // ?ô‰ªΩ?îË™ø?∏Ê?
    void BackupCoordinationData(};

    // ?¢Âæ©?îË™ø?∏Ê?
    void RestoreCoordinationData(};
};

