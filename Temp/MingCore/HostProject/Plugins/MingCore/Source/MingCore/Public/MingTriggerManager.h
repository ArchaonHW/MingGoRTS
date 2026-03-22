#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEventManager.h"
#include "MingTriggerManager.generated.h"

UENUM(BlueprintType)
enum class EMingTriggerConditionType : uint8
{
    EventOccurred UMETA(DisplayName = "Event Occurred"),
    ResourceThreshold UMETA(DisplayName = "Resource Threshold"),
    UnitCount UMETA(DisplayName = "Unit Count"),
    BuildingConstructed UMETA(DisplayName = "Building Constructed"),
    TimeElapsed UMETA(DisplayName = "Time Elapsed"),
    AreaEntered UMETA(DisplayName = "Area Entered"),
    MissionObjective UMETA(DisplayName = "Mission Objective"),
    GameState UMETA(DisplayName = "Game State"),
    CustomCondition UMETA(DisplayName = "Custom Condition"),
    CompoundAND UMETA(DisplayName = "AND - All Conditions"),
    CompoundOR UMETA(DisplayName = "OR - Any Condition")
};

UENUM(BlueprintType)
enum class EMingTriggerComparison : uint8
{
    Equal UMETA(DisplayName = "Equal"),
    GreaterThan UMETA(DisplayName = "Greater Than"),
    LessThan UMETA(DisplayName = "Less Than"),
    GreaterOrEqual UMETA(DisplayName = "Greater or Equal"),
    LessOrEqual UMETA(DisplayName = "Less or Equal"),
    NotEqual UMETA(DisplayName = "Not Equal")
};

USTRUCT(BlueprintType)
struct FMingTriggerCondition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTriggerConditionType ConditionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTriggerComparison Comparison;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingGameEventType EventType; // For EventOccurred

    // 注意：EMingResourceType 定義在其他插件中，不適合 UPROPERTY
    int32 ResourceTypeInt; // For ResourceThreshold, mapped to EMingResourceType

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 IntValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FloatValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StringValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation; // For AreaEntered

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius;

    // 注意：遞迴結構不能用 UPROPERTY
    TArray<FMingTriggerCondition> SubConditions; // For Compound conditions

    FMingTriggerCondition()
        : ConditionType(EMingTriggerConditionType::EventOccurred)
        , Comparison(EMingTriggerComparison::GreaterOrEqual)
        , EventType(EMingGameEventType::UnitSpawned)
        , ResourceTypeInt(0)
        , IntValue(0)
        , FloatValue(0.0f)
        , Radius(100.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingTriggerAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ActionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> SpawnClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector SpawnLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 IntParameter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FloatParameter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bParameter;

    FMingTriggerAction()
        : IntParameter(0)
        , FloatParameter(0.0f)
        , bParameter(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingGameTrigger
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TriggerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TriggerName;

    TArray<FMingTriggerCondition> Conditions;

    TArray<FMingTriggerAction> Actions;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    UPROPERTY(BlueprintReadOnly)
    bool bTriggered;

    UPROPERTY(BlueprintReadOnly)
    int32 TriggerCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRepeatable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxTriggerCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cooldown;

    UPROPERTY(BlueprintReadOnly)
    float LastTriggerTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bOneTimeOnly;

    FMingGameTrigger()
        : bIsActive(true)
        , bTriggered(false)
        , TriggerCount(0)
        , bRepeatable(false)
        , MaxTriggerCount(1)
        , Cooldown(0.0f)
        , LastTriggerTime(-1.0f)
        , bOneTimeOnly(true)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerActivated, const FMingGameTrigger&, Trigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTriggerConditionMet, const FMingGameTrigger&, Trigger, const FMingTriggerCondition&, Condition);

/**
 * Trigger Manager for MingGoRTS
 * Manages game triggers, conditions, and scripted events
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGCORE_API UMingTriggerManager : public UObject
{
    GENERATED_BODY()

public:
    UMingTriggerManager();

    // Singleton access
    UFUNCTION(BlueprintPure, Category = "Trigger Manager", meta = (WorldContext = "WorldContextObject"))
    static UMingTriggerManager* Get(UObject* WorldContextObject);

    // Trigger Registration
    UFUNCTION(BlueprintCallable, Category = "Triggers")
    FString RegisterTrigger(const FMingGameTrigger& Trigger);

    UFUNCTION(BlueprintCallable, Category = "Triggers")
    void UnregisterTrigger(const FString& TriggerID);

    UFUNCTION(BlueprintCallable, Category = "Triggers")
    void ActivateTrigger(const FString& TriggerID);

    UFUNCTION(BlueprintCallable, Category = "Triggers")
    void DeactivateTrigger(const FString& TriggerID);

    UFUNCTION(BlueprintCallable, Category = "Triggers")
    void ResetTrigger(const FString& TriggerID);

    // Trigger Queries
    UFUNCTION(BlueprintPure, Category = "Triggers")
    bool IsTriggerActive(const FString& TriggerID) const;

    UFUNCTION(BlueprintPure, Category = "Triggers")
    bool HasTriggerFired(const FString& TriggerID) const;

    UFUNCTION(BlueprintPure, Category = "Triggers")
    int32 GetTriggerCount(const FString& TriggerID) const;

    UFUNCTION(BlueprintPure, Category = "Triggers")
    TArray<FMingGameTrigger> GetAllTriggers() const;

    UFUNCTION(BlueprintPure, Category = "Triggers")
    FMingGameTrigger GetTrigger(const FString& TriggerID) const;

    // Update
    UFUNCTION(BlueprintCallable, Category = "Update")
    void UpdateTriggers(float DeltaTime);

    // Event Integration
    UFUNCTION()
    void OnGameEvent(const FMingGameEvent& Event);

    // Predefined Triggers
    UFUNCTION(BlueprintCallable, Category = "Presets")
    FString CreateVictoryTrigger(const FString& TriggerName, const TArray<FMingTriggerCondition>& Conditions);

    UFUNCTION(BlueprintCallable, Category = "Presets")
    FString CreateDefeatTrigger(const FString& TriggerName, const TArray<FMingTriggerCondition>& Conditions);

    UFUNCTION(BlueprintCallable, Category = "Presets")
    FString CreateResourceGoalTrigger(const FString& TriggerName, int32 ResourceTypeInt, int32 TargetAmount);

    UFUNCTION(BlueprintCallable, Category = "Presets")
    FString CreateUnitCountTrigger(const FString& TriggerName, int32 MinUnitCount);

    UFUNCTION(BlueprintCallable, Category = "Presets")
    FString CreateBuildingTrigger(const FString& TriggerName, int32 BuildingTypeInt);

    UFUNCTION(BlueprintCallable, Category = "Presets")
    FString CreateTimeLimitTrigger(const FString& TriggerName, float TimeLimitSeconds);

    UFUNCTION(BlueprintCallable, Category = "Presets")
    FString CreateAreaTrigger(const FString& TriggerName, const FVector& Location, float Radius, int32 RequiredUnits);

    // Event Delegates
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTriggerActivated OnTriggerActivated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTriggerConditionMet OnConditionMet;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static bool EvaluateCondition(const FMingTriggerCondition& Condition, UObject* WorldContext);

    UFUNCTION(BlueprintCallable, Category = "Utility")
    void ClearAllTriggers();

    UFUNCTION(BlueprintPure, Category = "Utility")
    int32 GetActiveTriggerCount() const;

    UFUNCTION(BlueprintPure, Category = "Utility")
    int32 GetTotalTriggerCount() const;

    // Serialization
    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString ExportTriggersToJson();

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void ImportTriggersFromJson(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingGameTrigger> RegisteredTriggers;

    UPROPERTY()
    TWeakObjectPtr<UWorld> CachedWorld;

    UPROPERTY()
    TWeakObjectPtr<class UMingEventManager> EventManager;

    // Evaluation functions
    bool EvaluateTrigger(const FMingGameTrigger& Trigger);
    bool EvaluateConditionInternal(const FMingTriggerCondition& Condition);
    bool EvaluateCompoundAND(const FMingTriggerCondition& Condition);
    bool EvaluateCompoundOR(const FMingTriggerCondition& Condition);
    bool EvaluateResourceThreshold(const FMingTriggerCondition& Condition);
    bool EvaluateUnitCount(const FMingTriggerCondition& Condition);
    bool EvaluateBuildingConstructed(const FMingTriggerCondition& Condition);
    bool EvaluateTimeElapsed(const FMingTriggerCondition& Condition);
    bool EvaluateAreaEntered(const FMingTriggerCondition& Condition);

    // Action execution
    void ExecuteActions(const FMingGameTrigger& Trigger);
    void ExecuteAction(const FMingTriggerAction& Action);

    // Helpers
    bool CompareValues(int32 Value1, int32 Value2, EMingTriggerComparison Comparison);
    bool CompareValues(float Value1, float Value2, EMingTriggerComparison Comparison);
    FString GenerateTriggerID();
};
