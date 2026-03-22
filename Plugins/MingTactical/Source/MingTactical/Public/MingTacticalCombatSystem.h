#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingTacticalCombatSystem.generated.h"

// ?Æ‰?È°ûÂ??öË?
UENUM(BlueprintType)
enum class EMingUnitType : uint8
{
    Infantry,          // Ê≠•ÂÖµ
    Cavalry,           // È®éÂÖµ
    Artillery,         // ?≤ÂÖµ
    Tank,              // ?¶Â?
    Aircraft,          // È£õÊ?
    Naval,             // Êµ∑Ë?
    Support,           // ?ØÊè¥?Æ‰?
    Commander          // ?áÊèÆÂÆ?};

// ?∞Ë?Á∑®È?È°ûÂ?
UENUM(BlueprintType)
enum class EMingTacticalFormationType : uint8
{
    Line,              // Á∑öÂΩ¢???
    Column,            // Á∏±È????
    Wedge,             // Ê•îÂΩ¢???
    Diamond,           // ?±ÂΩ¢???
    Circle,            // ?ìÂΩ¢???
    Skirmish,          // ??Öµ???
    Square,            // ?πÂΩ¢???
    Arrow,             // ÁÆ≠È†≠???
    Crescent,          // ?∞Ê????
    Custom             // ?™Â?Áæ©Èô£??};

// ?∞È¨•?Ä??UENUM(BlueprintType)
enum class EMingCombatState : uint8
{
    Idle,              // Á©∫È?
    Moving,            // ÁßªÂ?‰∏?    Attacking,          // ?ªÊ?‰∏?    Defending,         // ?≤Á¶¶‰∏?    Retreating,         // ?§ÈÄÄ‰∏?    Routed,            // ÊΩ∞Ê?
    Captured,          // Ë¢´‰???    Destroyed          // Ë¢´ÊëßÊØÄ
};

/**
 * int32 ?∏Á??ÖË?ÁµêÊ? (?®Êñº TMap<TArray> ÂµåÂ?)
 */
USTRUCT(BlueprintType)
struct FInt32ArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Items;

    FInt32ArrayWrapper() {}
};

// ?Æ‰??∞È¨•Â±¨ÊÄ?USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingUnitCombatStats
{
    GENERATED_BODY()

    // ?∫Á?Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

    // ?∞Ë?Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Morale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Discipline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Leadership;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Experience;

    // ?πÊ?Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// ?∞Ë?????∏Ê?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingTacticalFormation
{
    GENERATED_BODY()

    // ???ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // ????çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationName;

    // ???È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // ????èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationDescription;

    // ???‰ΩçÁΩÆ?∏Á?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> FormationPositions;

    // ????†Ê?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationBonuses;

    // ???Ê∏õÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationPenalties;

    // ?©Áî®?Æ‰?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMingUnitType> ApplicableUnitTypes;

    // ?ÄÂ∞èÂñÆ‰ΩçÊï∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinUnitCount;

    // ?ÄÂ§ßÂñÆ‰ΩçÊï∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxUnitCount;

    FMingTacticalFormation()
    {
        FormationID = -1;
        FormationName = TEXT("");
        FormationType = EMingFormationType::Line;
        FormationDescription = TEXT("");
        FormationPositions.Empty();
        FormationBonuses.Empty();
        FormationPenalties.Empty();
        ApplicableUnitTypes.Empty();
        MinUnitCount = 1;
        MaxUnitCount = 100;
    }
};

// ?∞È¨•?Æ‰??∏Ê?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingCombatUnit
{
    GENERATED_BODY()

    // ?Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // ?Æ‰??çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnitName;

    // ?Æ‰?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUnitType UnitType;

    // ?∂Â?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentPosition;

    // ?ÆÊ?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetPosition;

    // ?∂Â??Ä??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingCombatState CurrentState;

    // ?∞È¨•Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMingUnitCombatStats CombatStats;

    // ?ÄÂ±¨Á∑®?äID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // Á∑®È??ß‰?ÁΩ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationPosition;

    // ?áÊèÆÂÆòID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommanderID;

    // ?µÂ??Æ‰??óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EnemyUnits;

    // ?ãË??Æ‰??óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> FriendlyUnits;

    // ?ØÂê¶Ë¢´ÈÅ∏‰∏?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSelected;

    // ?ØÂê¶?ØË?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisible;

    // ?∞Áà≠Ëø∑ÈúßÁ≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FogOfWarLevel;

    FMingCombatUnit()
    {
        UnitID = -1;
        UnitName = TEXT("");
        UnitType = EMingUnitType::Infantry;
        CurrentPosition = FVector::ZeroVector;
        TargetPosition = FVector::ZeroVector;
        CurrentState = EMingCombatState::Idle;
        FormationID = -1;
        FormationPosition = -1;
        CommanderID = -1;
        EnemyUnits.Empty();
        FriendlyUnits.Empty();
        bIsSelected = false;
        bIsVisible = true;
        FogOfWarLevel = 0;
    }
};

// ?∞Ë??Ω‰ª§
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingTacticalCommand
{
    GENERATED_BODY()

    // ?Ω‰ª§ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommandID;

    // ?Ω‰ª§È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CommandType;

    // ?ÆÊ??Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetUnitID;

    // ?ÆÊ?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // ?Ω‰ª§?ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CommandParameters;

    // ?Ω‰ª§?™Â?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority;

    // ?Ω‰ª§?∑Ë??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // ?ØÂê¶?∫Ê?Á∫åÂëΩ‰ª?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsContinuous;

    FMingTacticalCommand()
    {
        CommandID = -1;
        CommandType = TEXT("");
        TargetUnitID = -1;
        TargetLocation = FVector::ZeroVector;
        CommandParameters.Empty();
        Priority = 1;
        ExecutionTime = 0.0f;
        bIsContinuous = false;
    }
};

// ?∞È¨•ÁµêÊ?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingCombatResult
{
    GENERATED_BODY()

    // ?ªÊ??Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackerID;

    // ?≤Á¶¶?Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefenderID;

    // ?∑ÂÆ≥?∏ÂÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageDealt;

    // ?ØÂê¶?¥Ê?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCriticalHit;

    // ?ØÂê¶?Ω‰∏≠
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHit;

    // ?∞È¨•?ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CombatTime;

    // ?∞È¨•‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CombatLocation;

    // ?∞È¨•È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CombatType;

    // Á∂ìÈ??ºÁç≤Âæ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceGained;

    FMingCombatResult()
    {
        AttackerID = -1;
        DefenderID = -1;
        DamageDealt = 0.0f;
        bIsCriticalHit = false;
        bIsHit = false;
        CombatTime = FDateTime::Now();
        CombatLocation = FVector::ZeroVector;
        CombatType = TEXT("");
        ExperienceGained = 0.0f;
    }
};

// ?∞Ë??∞È¨•‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitStateChanged, const FMingCombatUnit&, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatOccurred, const FMingCombatResult&, Result, const TArray<FMingCombatUnit>&, InvolvedUnits);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFormationChanged, int32, FormationID, EMingFormationType, NewFormationType, const TArray<FMingCombatUnit>&, Units);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTacticalCommandIssued, const FMingTacticalCommand&, Command);

/**
 * ?∞Ë??∞È¨•Á≥ªÁµ±
 * Ë≤†Ë≤¨ÁÆ°Á?Â§ßË?Ê®°ÂñÆ‰ΩçÊéß?∂Â??∞Ë??∞È¨•
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API AMingTacticalCombatSystem : public AActor
{
    GENERATED_BODY()

public:
    AMingTacticalCombatSystem();

    // ?ùÂ??ñÊà∞Ë°ìÊà∞È¨•Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool InitializeTacticalSystem();

    // Ë®ªÂ??∞È¨•?Æ‰?
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool RegisterCombatUnit(const FMingCombatUnit& Unit);

    // ÁßªÈô§?∞È¨•?Æ‰?
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool RemoveCombatUnit(int32 UnitID);

    // ?≤Â??Æ‰?‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    FMingCombatUnit GetUnitInfo_Impl(int32 UnitID) const;

    // ?∏Ê??Æ‰?
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SelectUnits_Impl(const TArray<int32>& UnitIDs);

    // ?ñÊ??∏Ê??Æ‰?
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool DeselectUnits(const TArray<int32>& UnitIDs);

    // ?≤Â?Â∑≤ÈÅ∏?áÂñÆ‰Ω?    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TArray<int32> GetSelectedUnits() const;

    // ÁßªÂ??Æ‰?
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool MoveUnits(const TArray<int32>& UnitIDs, const FVector& TargetLocation);

    // ?ªÊ??ÆÊ?
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool AttackTarget(int32 AttackerID, int32 TargetID);

    // Ë®≠ÁΩÆÁ∑®È?
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SetFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType);

    // ?≤Â?Á∑®È?‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    FMingTacticalFormation GetFormationInfo(EMingFormationType FormationType) const;

    // ?ºÂ??∞Ë??Ω‰ª§
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool IssueTacticalCommand(const FMingTacticalCommand& Command);

    // ?ïÁ??∞È¨•
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    FMingCombatResult ProcessCombat(int32 AttackerID, int32 DefenderID);

    // ?¥Êñ∞?∞Áà≠Ëø∑Èúß
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    void UpdateFogOfWar();

    // ?≤Â??ØË??Æ‰?
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TArray<int32> GetVisibleUnits_Impl(int32 ViewerUnitID) const;

    // ?≤Â??∞È¨•Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Tactical Combat")
    TMap<EMingUnitType, int32> GetCombatStatistics_Impl() const;

    // ‰øùÂ??∞È¨•?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool SaveCombatData();

    // ËºâÂÖ•?∞È¨•?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    bool LoadCombatData();

    // Ê∏ÖÈô§?Ä?âÂñÆ‰Ω?    UFUNCTION(BlueprintCallable, Category = "Tactical Combat")
    void ClearAllUnits();

    // ‰∫ã‰ª∂ÂßîË?
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

    // ?Æ‰??∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingCombatUnit> UnitDatabase;

    // Á∑®È??∏Ê?Â∫?    UPROPERTY()
    TMap<EMingFormationType, FMingTacticalFormation> FormationDatabase;

    // Â∑≤ÈÅ∏?áÂñÆ‰ΩçÂ?Ë°?    UPROPERTY()
    TArray<int32> SelectedUnits;

    // ?∞È¨•Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY()
    TArray<FMingCombatResult> CombatHistory;

    // ?∞Áà≠Ëø∑Èúß?∏Ê?
    UPROPERTY()
    TMap<int32, FInt32ArrayWrapper> VisibilityMap;

    // ?∂Â?Á∑®È?
    UPROPERTY()
    TMap<int32, EMingFormationType> UnitFormations;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ËºâÂÖ•?êË®≠Á∑®È?
    void LoadDefaultFormations();

    // Ë®àÁ??∞È¨•?∑ÂÆ≥
    float CalculateCombatDamage(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender);

    // Ë®àÁ??Ω‰∏≠Ê¶ÇÁ?
    float CalculateHitChance(const FMingCombatUnit& Attacker, const FMingCombatUnit& Defender);

    // Ë®àÁ??¥Ê?Ê¶ÇÁ?
    float CalculateCriticalChance(const FMingCombatUnit& Attacker);

    // ?¥Êñ∞?Æ‰??Ä??    void UpdateUnitState(int32 UnitID, EMingCombatState NewState);

    // ?ïÁ??Æ‰?ÁßªÂ?
    void ProcessUnitMovement(float DeltaTime);

    // ?ïÁ??∞È¨•?èËºØ
    void ProcessCombatLogic(float DeltaTime);

    // ?¥Êñ∞?Æ‰?Á∂ìÈ?
    void UpdateUnitExperience(int32 UnitID, float ExperienceAmount);

    // Ê™¢Êü•?Æ‰?Ë¶ñÈ?
    bool IsUnitVisible(int32 ViewerID, int32 TargetID) const;

    // ?üÊ?Á∑®È?‰ΩçÁΩÆ
    TArray<FVector> GenerateFormationPositions(EMingFormationType FormationType, const FVector& Center, int32 UnitCount) const;

    // È©óË??Æ‰??∏Ê?
    bool ValidateUnitData(const FMingCombatUnit& Unit) const;
};
