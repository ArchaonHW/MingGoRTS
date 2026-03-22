#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingTacticalCombatSystem.generated.h"

// ????類??????
UENUM(BlueprintType)
enum class EMingUnitType : uint8
{
    Infantry,          // 步兵
    Cavalry,           // 騎兵
    Artillery,         // ??兵
    Tank,              // ????
    Aircraft,          // 飛??
    Naval,             // 海??
    Support,           // ??援????
    Commander,  // ??揮??};

// ????編??類??
UENUM(BlueprintType)
enum class EMingTacticalFormationType : uint8
{
    Line,              // 線形???
    Column,            // 縱?????
    Wedge,             // 楔形???
    Diamond,           // ??形???
    Circle,            // ??形???
    Skirmish,          // ???????
    Square,            // ??形???
    Arrow,             // 箭頭???
    Crescent,          // ???????
    Custom,  // ????義陣??};

// ??鬥????UENUM(BlueprintType)
enum class EMingCombatState : uint8
{
    Idle,              // 空??
    Moving,            // 移????    Attacking,          // ??????    Defending,         // ??禦??    Retreating,         // ??退??    Routed,            // 潰??
    Captured,          // 被????    Destroyed          // 被摧毀
};

/**
 * int32 ????????結?? (??於 TMap<TArray> 嵌??)
 */
USTRUCT(BlueprintType)
struct FInt32ArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Items;

    FInt32ArrayWrapper() {}
};

// ??????鬥屬??USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingUnitCombatStats
{
    GENERATED_BODY()

    // ????屬??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

    // ????屬??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Morale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Discipline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Leadership;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Experience;

    // ????屬??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// ???????????
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingTacticalFormation
{
    GENERATED_BODY()

    // ???ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // ?????稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationName;

    // ???類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // ?????述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationDescription;

    // ???位置????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> FormationPositions;

    // ???????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationBonuses;

    // ???減??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationPenalties;

    // ??用????類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMingUnitType> ApplicableUnitTypes;

    // ??小單位數??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinUnitCount;

    // ??大單位數??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// ??鬥????????
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingCombatUnit
{
    GENERATED_BODY()

    // ????ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // ??????稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnitName;

    // ????類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUnitType UnitType;

    // ????位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentPosition;

    // ????位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetPosition;

    // ????????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingCombatState CurrentState;

    // ??鬥屬??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMingUnitCombatStats CombatStats;

    // ??屬編??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // 編????????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationPosition;

    // ??揮官ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommanderID;

    // ??????????表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EnemyUnits;

    // ??????????表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> FriendlyUnits;

    // ??否被選??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSelected;

    // ??否????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisible;

    // ??爭迷霧等??
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

// ??????令
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingTacticalCommand
{
    GENERATED_BODY()

    // ??令ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommandID;

    // ??令類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CommandType;

    // ????????ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetUnitID;

    // ????位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // ??令??數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CommandParameters;

    // ??令??????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority;

    // ??令????????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // ??否????續命??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// ??鬥結??
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingCombatResult
{
    GENERATED_BODY()

    // ????????ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackerID;

    // ??禦????ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefenderID;

    // ??害????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageDealt;

    // ??否????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCriticalHit;

    // ??否??中
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHit;

    // ??鬥????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CombatTime;

    // ??鬥位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CombatLocation;

    // ??鬥類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CombatType;

    // 經????獲??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// ??????鬥事件委??
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitStateChanged, const FMingCombatUnit&, Unit};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatOccurred, const FMingCombatResult&, Result, const TArray<FMingCombatUnit>&, InvolvedUnits};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFormationChanged, int32, FormationID, EMingFormationType, NewFormationType, const TArray<FMingCombatUnit>&, Units};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTacticalCommandIssued, const FMingTacticalCommand&, Command};

/**
 * ??????鬥系統
 * 負責管??大??模單位控??????????鬥
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API AMingTacticalCombatSystem : public AActor
{
    GENERATED_BODY()

public:
    AMingTacticalCombatSystem(};

    // ??????戰術戰鬥系??    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool InitializeTacticalSystem(};

    // 註????鬥????
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool RegisterCombatUnit(const FMingCombatUnit& Unit};

    // 移除??鬥????
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool RemoveCombatUnit(int32 UnitID};

    // ????????信息
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    FMingCombatUnit GetUnitInfo_Impl(int32 UnitID) const;

    // ????????
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SelectUnits_Impl(const TArray<int32>& UnitIDs};

    // ????????????
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool DeselectUnits(const TArray<int32>& UnitIDs};

    // ????已選??單??    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TArray<int32> GetSelectedUnits() const;

    // 移??????
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool MoveUnits(const TArray<int32>& UnitIDs, const FVector& TargetLocation};

    // ????????
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool AttackTarget(int32 AttackerID, int32 TargetID};

    // 設置編??
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SetFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType};

    // ????編??信息
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    FMingTacticalFormation GetFormationInfo(EMingFormationType FormationType) const;

    // ??????????令
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool IssueTacticalCommand(const FMingTacticalCommand& Command};

    // ??????鬥
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    FMingCombatResult ProcessCombat(int32 AttackerID, int32 DefenderID};

    // ??新??爭迷霧
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    void UpdateFogOfWar(};

    // ????????????
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TArray<int32> GetVisibleUnits_Impl(int32 ViewerUnitID) const;

    // ??????鬥統??
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TMap<EMingUnitType, int32> GetCombatStatistics_Impl() const;

    // 保????鬥????
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SaveCombatData(};

    // 載入??鬥????
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool LoadCombatData(};

    // 清除????單??    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    void ClearAllUnits(};

    // 事件委??
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

    // ??????????    UPROPERTY()
    TMap<int32, FMingCombatUnit> UnitDatabase;

    // 編????????    UPROPERTY()
    TMap<EMingFormationType, FMingTacticalFormation> FormationDatabase;

    // 已選??單位????    UPROPERTY()
    TArray<int32> SelectedUnits;

    // ??鬥歷史記??
    UPROPERTY()
    TArray<FMingCombatResult> CombatHistory;

    // ??爭迷霧????
    UPROPERTY()
    TMap<int32, FInt32ArrayWrapper> VisibilityMap;

    // ????編??
    UPROPERTY()
    TMap<int32, EMingFormationType> UnitFormations;

    // ??否已??始??
    UPROPERTY()
    bool bInitialized;

private:
    // 載入??設編??
    void LoadDefaultFormations(};

    // 計????鬥??害
    float CalculateCombatDamage(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender};

    // 計????中概??
    float CalculateHitChance(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender};

    // 計??????概??
    float CalculateCriticalChance(const FMingCombatUnit& Attacker};

    // ??新????????    void UpdateUnitState(int32 UnitID, EMingCombatState NewState};

    // ????????移??
    void ProcessUnitMovement(float DeltaTime};

    // ??????鬥??輯
    void ProcessCombatLogic(float DeltaTime};

    // ??新????經??
    void UpdateUnitExperience(int32 UnitID, float ExperienceAmount};

    // 檢查????視??
    bool IsUnitVisible(int32 ViewerID, int32 TargetID) const;

    // ????編??位置
    TArray<FVector> GenerateFormationPositions(EMingFormationType FormationType, const FVector& Center, int32 UnitCount) const;

    // 驗??????????
    bool ValidateUnitData(const FMingCombatUnit& Unit) const;
};

