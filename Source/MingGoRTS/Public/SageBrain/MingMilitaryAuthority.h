#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingMilitaryAuthority.generated.h"

UENUM(BlueprintType)
enum class EMilitaryCommandType : uint8
{
    None            UMETA(DisplayName = "無"),
    DirectCommand   UMETA(DisplayName = "直接指揮"),
    StrategicCommand UMETA(DisplayName = "戰略指揮"),
    TacticalCommand UMETA(DisplayName = "戰術指揮"),
    SupportCommand  UMETA(DisplayName = "支援指揮"),
    SpecialCommand  UMETA(DisplayName = "特殊指揮")
};

UENUM(BlueprintType)
enum class EUnitType : uint8
{
    None            UMETA(DisplayName = "無"),
    Infantry        UMETA(DisplayName = "步兵"),
    Cavalry         UMETA(DisplayName = "騎兵"),
    Archer          UMETA(DisplayName = "弓兵"),
    Siege           UMETA(DisplayName = "攻城"),
    Special         UMETA(DisplayName = "特殊")
};

USTRUCT(BlueprintType)
struct FMilitaryStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MilitaryCommand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LeadershipAbility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatEffectiveness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TroopMorale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DisciplineLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommandRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalTroops;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EUnitType, int32> UnitCounts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMilitaryCommandType> MasteredCommands;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> MilitaryModifiers;

    FMilitaryStatus()
    {
        MilitaryCommand = 50.0f;
        LeadershipAbility = 50.0f;
        CombatEffectiveness = 50.0f;
        TroopMorale = 50.0f;
        DisciplineLevel = 50.0f;
        CommandRadius = 100;
        TotalTroops = 0;
    }
};

USTRUCT(BlueprintType)
struct FMilitaryOrder
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OrderName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMilitaryCommandType CommandType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EUnitType TargetUnitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OrderPriority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RequiredUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsExecuting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUrgent;

    FMilitaryOrder()
    {
        OrderName = "";
        CommandType = EMilitaryCommandType::None;
        TargetUnitType = EUnitType::None;
        TargetLocation = FVector::ZeroVector;
        ExecutionTime = 0.0f;
        OrderPriority = 1.0f;
        bIsExecuting = false;
        ExecutionProgress = 0.0f;
        bIsUrgent = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMilitaryOrderIssued, const FString&, OrderName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOrderCompleted, const FString&, OrderName, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMilitaryLevelUp, int32, NewLevel);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingMilitaryAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingMilitaryAuthority();

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void InitializeMilitaryAuthority();

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void SetMilitaryCommand(float CommandLevel);

    UFUNCTION(BlueprintPure, Category = "Military Authority")
    FMilitaryStatus GetMilitaryStatus() const { return CurrentMilitaryStatus; }

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    bool IssueMilitaryOrder(const FMilitaryOrder& Order);

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void UpdateOrderExecution(const FString& OrderName, float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    bool CanIssueOrder(EMilitaryCommandType CommandType, float RequiredCommand) const;

    UFUNCTION(BlueprintPure, Category = "Military Authority")
    TArray<FString> GetAvailableOrders(EMilitaryCommandType CommandType) const;

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    bool CancelOrder(const FString& OrderName);

    UFUNCTION(BlueprintPure, Category = "Military Authority")
    TArray<FMilitaryOrder> GetActiveOrders() const;

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void AddTroops(EUnitType UnitType, int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void RemoveTroops(EUnitType UnitType, int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void UpgradeMilitarySkill(EMilitaryCommandType CommandType, float Amount);

    UFUNCTION(BlueprintPure, Category = "Military Authority")
    float CalculateCombatEffectiveness() const;

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void ApplyMilitaryModifier(const FString& ModifierName, float ModifierValue);

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void RemoveMilitaryModifier(const FString& ModifierName);

    UFUNCTION(BlueprintPure, Category = "Military Authority")
    bool HasCommandType(EMilitaryCommandType CommandType) const;

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void MasterCommandType(EMilitaryCommandType CommandType);

    UFUNCTION(BlueprintPure, Category = "Military Authority")
    int32 GetTotalTroopCount() const;

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void BoostTroopMorale(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Military Authority")
    void IncreaseDiscipline(float Amount);

    UFUNCTION(BlueprintPure, Category = "Military Authority")
    FString GetMilitaryRecommendation() const;

    UPROPERTY(BlueprintAssignable, Category = "Military Authority")
    FOnMilitaryOrderIssued OnMilitaryOrderIssued;

    UPROPERTY(BlueprintAssignable, Category = "Military Authority")
    FOnOrderCompleted OnOrderCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Military Authority")
    FOnMilitaryLevelUp OnMilitaryLevelUp;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Military Authority")
    void OnMilitaryAuthorityInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Military Authority")
    void OnMilitaryStatusChanged(const FMilitaryStatus& NewStatus);

    UFUNCTION(BlueprintImplementableEvent, Category = "Military Authority")
    void OnOrderStarted(const FString& OrderName);

    UFUNCTION(BlueprintImplementableEvent, Category = "Military Authority")
    void OnCommandTypeMastered(EMilitaryCommandType CommandType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Military Authority")
    void OnTroopsChanged(EUnitType UnitType, int32 NewCount);

private:
    void InitializeDefaultOrders();
    void UpdateMilitaryStatus();
    bool ValidateMilitaryOrder(const FMilitaryOrder& Order) const;
    void CompleteOrder(const FString& OrderName, bool bSuccess);
    void CalculateTotalTroops();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    FMilitaryStatus CurrentMilitaryStatus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    TArray<FMilitaryOrder> ActiveOrders;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    bool bIsInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    float MaxMilitaryCommand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    float CommandRestoreRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    int32 MaxActiveOrders;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    TMap<EMilitaryCommandType, TArray<FString>> CommandTypeMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    TMap<FString, FMilitaryOrder> OrderLibrary;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    TArray<FString> ExecutedOrders;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    TMap<EMilitaryCommandType, int32> CommandExperience;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    float BaseMoraleDecayRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Military Authority")
    float DisciplineDecayRate;
};
