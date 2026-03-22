#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingTacticalCombatSystem.generated.h"

// X��X
UENUM(BlueprintType)
enum class EMingUnitType : uint8
{
    Infantry,          // �B�L
    Cavalry,           // �M�L
    Artillery,         // X�L
    Tank,              // X
    Aircraft,          // ��X
    Naval,             // ��X
    Support,           // X��X
    Commander,  // X��X};

// X�sX��X
UENUM(BlueprintType)
enum class EMingTacticalFormationType : uint8
{
    Line,              // �u��X
    Column,            // �aX
    Wedge,             // ����X
    Diamond,           // X��X
    Circle,            // X��X
    Skirmish,          // X
    Square,            // X��X
    Arrow,             // �b�YX
    Crescent,          // X
    Custom,  // X�q�}X};

// X��XUENUM(BlueprintType)
enum class EMingCombatState : uint8
{
    Idle,              // ��X
    Moving,            // ��X    Attacking,          // X    Defending,         // X�mX    Retreating,         // X�hX    Routed,            // ��X
    Captured,          // �QX    Destroyed          // �Q�R��
};

/**
 * int32 X��X (X�� TMap<TArray> �OX)
 */
USTRUCT(BlueprintType)
struct FInt32ArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Items;

    FInt32ArrayWrapper() {}
};

// X����XUSTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingUnitCombatStats
{
    GENERATED_BODY()

    // X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefensePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionRange;

    // X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Morale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Discipline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Leadership;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Experience;

    // X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Accuracy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Evasion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Armor;

    FMingUnitCombatStats()
    {
        Health = 100.0f;
        MaxHealth = 100.0f;
        AttackPower = 20.0f;
        DefensePower = 15.0f;
        MovementSpeed = 5.0f;
        AttackRange = 2.0f;
        VisionRange = 8.0f;
        Morale = 75.0f;
        Discipline = 70.0f;
        Leadership = 50.0f;
        Experience = 0.0f;
        Accuracy = 0.7f;
        CriticalChance = 0.1f;
        Evasion = 0.05f;
        Armor = 0.2f;
    }
};

// X
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingTacticalFormation
{
    GENERATED_BODY()

    // XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationName;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // X�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationDescription;

    // X��mX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> FormationPositions;

    // X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationBonuses;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationPenalties;

    // X��X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMingUnitType> ApplicableUnitTypes;

    // X�p����X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinUnitCount;

    // X�j����X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxUnitCount;

    FMingTacticalFormation()
    {
        FormationID = -1;
        FormationName = TEXT(""};
        FormationType = EMingFormationType::Line;
        FormationDescription = TEXT(""};
        FormationPositions.Empty(};
        FormationBonuses.Empty(};
        FormationPenalties.Empty(};
        ApplicableUnitTypes.Empty(};
        MinUnitCount = 1;
        MaxUnitCount = 100;
    }
};

// X��X
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingCombatUnit
{
    GENERATED_BODY()

    // XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnitName;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUnitType UnitType;

    // X��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentPosition;

    // X��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetPosition;

    // X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingCombatState CurrentState;

    // X����X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMingUnitCombatStats CombatStats;

    // X�ݽsXID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // �sX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationPosition;

    // X���xID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommanderID;

    // X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EnemyUnits;

    // X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> FriendlyUnits;

    // X�_�Q��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSelected;

    // X�_X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisible;

    // X���g����X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FogOfWarLevel;

    FMingCombatUnit()
    {
        UnitID = -1;
        UnitName = TEXT(""};
        UnitType = EMingUnitType::Infantry;
        CurrentPosition = FVector::ZeroVector;
        TargetPosition = FVector::ZeroVector;
        CurrentState = EMingCombatState::Idle;
        FormationID = -1;
        FormationPosition = -1;
        CommanderID = -1;
        EnemyUnits.Empty(};
        FriendlyUnits.Empty(};
        bIsSelected = false;
        bIsVisible = true;
        FogOfWarLevel = 0;
    }
};

// X�O
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingTacticalCommand
{
    GENERATED_BODY()

    // X�OID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommandID;

    // X�O��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CommandType;

    // XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetUnitID;

    // X��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // X�OX��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CommandParameters;

    // X�OX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority;

    // X�OX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // X�_X��RX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsContinuous;

    FMingTacticalCommand()
    {
        CommandID = -1;
        CommandType = TEXT(""};
        TargetUnitID = -1;
        TargetLocation = FVector::ZeroVector;
        CommandParameters.Empty(};
        Priority = 1;
        ExecutionTime = 0.0f;
        bIsContinuous = false;
    }
};

// X����X
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingCombatResult
{
    GENERATED_BODY()

    // XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackerID;

    // X�mXID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefenderID;

    // X�`X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageDealt;

    // X�_X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCriticalHit;

    // X�_X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHit;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CombatTime;

    // X����m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CombatLocation;

    // X����X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CombatType;

    // �gX��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceGained;

    FMingCombatResult()
    {
        AttackerID = -1;
        DefenderID = -1;
        DamageDealt = 0.0f;
        bIsCriticalHit = false;
        bIsHit = false;
        CombatTime = FDateTime::Now(};
        CombatLocation = FVector::ZeroVector;
        CombatType = TEXT(""};
        ExperienceGained = 0.0f;
    }
};

// X���ƥ�eX
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitStateChanged, const FMingCombatUnit&, Unit};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatOccurred, const FMingCombatResult&, Result, const TArray<FMingCombatUnit>&, InvolvedUnits};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFormationChanged, int32, FormationID, EMingFormationType, NewFormationType, const TArray<FMingCombatUnit>&, Units};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTacticalCommandIssued, const FMingTacticalCommand&, Command};

/**
 * X���t��
 * �t�d��X�jX�ҳ�챱X��
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API AMingTacticalCombatSystem : public AActor
{
    GENERATED_BODY()

public:
    AMingTacticalCombatSystem(};

    // X�ԳN�԰��tX    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool InitializeTacticalSystem(};

    // ��X��X
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool RegisterCombatUnit(const FMingCombatUnit& Unit};

    // ����X��X
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool RemoveCombatUnit(int32 UnitID};

    // X�H��
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    FMingCombatUnit GetUnitInfo_Impl(int32 UnitID) const;

    // X
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SelectUnits_Impl(const TArray<int32>& UnitIDs};

    // X
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool DeselectUnits(const TArray<int32>& UnitIDs};

    // X�w��X��X    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TArray<int32> GetSelectedUnits() const;

    // ��X
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool MoveUnits(const TArray<int32>& UnitIDs, const FVector& TargetLocation};

    // X
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool AttackTarget(int32 AttackerID, int32 TargetID};

    // �]�m�sX
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SetFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType};

    // X�sX�H��
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    FMingTacticalFormation GetFormationInfo(EMingFormationType FormationType) const;

    // X�O
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool IssueTacticalCommand(const FMingTacticalCommand& Command};

    // X��
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    FMingCombatResult ProcessCombat(int32 AttackerID, int32 DefenderID};

    // X�sX���g��
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    void UpdateFogOfWar(};

    // X
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TArray<int32> GetVisibleUnits_Impl(int32 ViewerUnitID) const;

    // X����X
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TMap<EMingUnitType, int32> GetCombatStatistics_Impl() const;

    // �OX��X
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SaveCombatData(};

    // ���JX��X
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool LoadCombatData(};

    // �M��X��X    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    void ClearAllUnits(};

    // �ƥ�eX
    UPROPERTY(BlueprintAssignable)
    FOnUnitStateChanged OnUnitStateChanged;

    UPROPERTY(BlueprintAssignable)
    FOnCombatOccurred OnCombatOccurred;

    UPROPERTY(BlueprintAssignable)
    FOnFormationChanged OnFormationChanged;

    UPROPERTY(BlueprintAssignable)
    FOnTacticalCommandIssued OnTacticalCommandIssued;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // X    UPROPERTY()
    TMap<int32, FMingCombatUnit> UnitDatabase;

    // �sX    UPROPERTY()
    TMap<EMingFormationType, FMingTacticalFormation> FormationDatabase;

    // �w��X���X    UPROPERTY()
    TArray<int32> SelectedUnits;

    // X�����v�OX
    UPROPERTY()
    TArray<FMingCombatResult> CombatHistory;

    // X���g��X
    UPROPERTY()
    TMap<int32, FInt32ArrayWrapper> VisibilityMap;

    // X�sX
    UPROPERTY()
    TMap<int32, EMingFormationType> UnitFormations;

    // X�_�wX�lX
    UPROPERTY()
    bool bInitialized;

private:
    // ���JX�]�sX
    void LoadDefaultFormations(};

    // �pX��X�`
    float CalculateCombatDamage(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender};

    // �pX����X
    float CalculateHitChance(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender};

    // �pX��X
    float CalculateCriticalChance(const FMingCombatUnit& Attacker};

    // X�sX    void UpdateUnitState(int32 UnitID, EMingCombatState NewState};

    // X��X
    void ProcessUnitMovement(float DeltaTime};

    // X��X��
    void ProcessCombatLogic(float DeltaTime};

    // X�sX�gX
    void UpdateUnitExperience(int32 UnitID, float ExperienceAmount};

    // �ˬdX��X
    bool IsUnitVisible(int32 ViewerID, int32 TargetID) const;

    // X�sX��m
    TArray<FVector> GenerateFormationPositions(EMingFormationType FormationType, const FVector& Center, int32 UnitCount) const;

    // ��X
    bool ValidateUnitData(const FMingCombatUnit& Unit) const;
};

