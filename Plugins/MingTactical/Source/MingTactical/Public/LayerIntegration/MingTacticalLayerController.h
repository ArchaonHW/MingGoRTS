#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LayerIntegration/MingFourLayerTypes.h"
#include "MingTacticalLayerController.generated.h"

UENUM(BlueprintType)
enum class EMingBattlePhase: uint8 {
    Deployment UMETA(DisplayName = "Deployment"),
    Opening UMETA(DisplayName = "Opening Moves"),
    MidBattle UMETA(DisplayName = "Mid Battle"),
    Climax UMETA(DisplayName = "Climax"),
    Resolution UMETA(DisplayName = "Resolution"),
    Aftermath UMETA(DisplayName = "Aftermath")
};

UENUM(BlueprintType)
enum class EMingBuildingLayerType: uint8 {
    Headquarters UMETA(DisplayName = "Headquarters"),
    Barracks UMETA(DisplayName = "Barracks"),
    Factory UMETA(DisplayName = "Factory"),
    Armory UMETA(DisplayName = "Armory"),
    Warehouse UMETA(DisplayName = "Warehouse"),
    ResearchLab UMETA(DisplayName = "Research Lab"),
    MedicalStation UMETA(DisplayName = "Medical Station"),
    TrainingGround UMETA(DisplayName = "Training Ground"),
    DefenseTower UMETA(DisplayName = "Defense Tower"),
    Wall UMETA(DisplayName = "Wall")
};

UENUM(BlueprintType)
enum class EMingUnitStance: uint8 {
    Aggressive UMETA(DisplayName = "Aggressive"),
    Defensive UMETA(DisplayName = "Defensive"),
    Hold UMETA(DisplayName = "Hold Position"),
    Scout UMETA(DisplayName = "Scouting"),
    Ambush UMETA(DisplayName = "Ambush"),
    Retreat UMETA(DisplayName = "Retreat")
};

USTRUCT(BlueprintType)
struct FMingBattleInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    FString BattleID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    FString BattleName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    EMingBattlePhase CurrentPhase = EMingBattlePhase::Deployment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    FString AttackingFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    FString DefendingFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    FString BattlefieldRegionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    float BattleDuration = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    int32 AttackerCasualties = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    int32 DefenderCasualties = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    bool bIsActive = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    bool bVictory = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    FVector BattleCenter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    float BattleRadius = 1000.0f;

    FMingBattleInfo()
        : CurrentPhase(EMingBattlePhase::Deployment)
        , BattleDuration(0.0f)
        , AttackerCasualties(0)
        , DefenderCasualties(0)
        , bIsActive(false)
        , bVictory(false)
        , BattleCenter(FVector::ZeroVector)
        , BattleRadius(1000.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingGameTacticalUnit
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    int32 UnitID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    FString UnitName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    FString UnitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    FString FactionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    FVector CurrentPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    FVector TargetPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    EMingUnitStance CurrentStance = EMingUnitStance::Hold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    int32 Health = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    int32 MaxHealth = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    int32 Ammunition = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    int32 Morale = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    bool bIsAlive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    bool bIsSelected = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    TArray<int32> GroupIDs;

    FMingGameTacticalUnit()
        : UnitID(-1)
        , CurrentStance(EMingUnitStance::Hold)
        , Health(100)
        , MaxHealth(100)
        , Ammunition(100)
        , Morale(100)
        , bIsAlive(true)
        , bIsSelected(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingCommandGroup
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
    int32 GroupID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
    FString GroupName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
    TArray<int32> UnitIDs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
    EMingUnitStance GroupStance = EMingUnitStance::Hold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
    FVector FormationCenter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
    FString FormationType;

    FMingCommandGroup()
        : GroupID(-1)
        , GroupStance(EMingUnitStance::Hold)
    {}
};







/**
 * Tactical Layer Controller
 * Manages real-time battles, unit control, and tactical command execution
 */
UCLASS(ClassGroup = (Tactical), Blueprintable)
class MINGTACTICAL_API UMingTacticalLayerController : public UObject
{
    GENERATED_BODY()

public:
    UMingTacticalLayerController();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void InitializeTacticalLayer();

    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void ShutdownTacticalLayer();

    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void ActivateTacticalLayer();

    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void DeactivateTacticalLayer();

    // Battle Management
    UFUNCTION(BlueprintCallable, Category = "Battle Management")
    FString StartBattle(const FMingBattleInfo& BattleSetup);

    UFUNCTION(BlueprintCallable, Category = "Battle Management")
    void EndBattle(const FString& BattleID, bool bAttackerVictory);

    UFUNCTION(BlueprintCallable, Category = "Battle Management")
    void AbortBattle(const FString& BattleID);

    UFUNCTION(BlueprintCallable, Category = "Battle Management")
    bool GetBattle(const FString& BattleID, FMingBattleInfo& OutBattle) const;

    UFUNCTION(BlueprintCallable, Category = "Battle Management")
    void AdvanceBattlePhase(const FString& BattleID);

    UFUNCTION(BlueprintCallable, Category = "Battle Management")
    void SetBattlePhase(const FString& BattleID, EMingBattlePhase NewPhase);

    UFUNCTION(BlueprintPure, Category = "Battle Management")
    bool IsBattleActive(const FString& BattleID) const;

    UFUNCTION(BlueprintCallable, Category = "Battle Management")
    FMingBattleInfo GetCurrentBattle() const;

    // Unit Management
    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    int32 SpawnUnit(const FMingGameTacticalUnit& UnitSetup);

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    void RemoveUnit(int32 UnitID);

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    bool GetUnit(int32 UnitID, FMingGameTacticalUnit& OutUnit) const;

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    void UpdateUnitPosition(int32 UnitID, FVector NewPosition);

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    void UpdateUnitHealth(int32 UnitID, int32 NewHealth);

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    void UpdateUnitMorale(int32 UnitID, int32 NewMorale);

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    void DestroyUnit(int32 UnitID);

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    TArray<FMingGameTacticalUnit> GetAllUnits() const;

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    TArray<FMingGameTacticalUnit> GetUnitsByFaction(const FString& FactionID) const;

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    TArray<FMingGameTacticalUnit> GetUnitsInRadius(FVector Center, float Radius) const;

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    int32 GetUnitCountByFaction(const FString& FactionID) const;

    UFUNCTION(BlueprintCallable, Category = "Unit Management")
    int32 GetLiveUnitCountByFaction(const FString& FactionID) const;

    // Unit Selection
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectUnit(int32 UnitID);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void DeselectUnit(int32 UnitID);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectUnitsInBox(FVector2D ScreenStart, FVector2D ScreenEnd);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectAllUnits(const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void ClearSelection();

    UFUNCTION(BlueprintPure, Category = "Selection")
    TArray<int32> GetSelectedUnits() const;

    UFUNCTION(BlueprintPure, Category = "Selection")
    int32 GetSelectedCount() const;

    // Command Execution
    UFUNCTION(BlueprintCallable, Category = "Command")
    void ExecuteOrder(const FMingTacticalOrder& Order);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void ExecuteOrderOnSelection(const FMingTacticalOrder& Order);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void MoveSelectedUnits(FVector TargetLocation, EMingUnitStance Stance);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void AttackTarget(int32 TargetUnitID);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void AttackMove(FVector TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void HoldPosition();

    UFUNCTION(BlueprintCallable, Category = "Command")
    void RetreatAllUnits(const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void SetUnitStance(EMingUnitStance NewStance);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void StopCurrentAction();

    // Formation Management
    UFUNCTION(BlueprintCallable, Category = "Formation")
    int32 CreateCommandGroup(const TArray<int32>& UnitIDs, const FString& GroupName);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void DisbandCommandGroup(int32 GroupID);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void SetFormation(int32 GroupID, const FString& FormationType, float Spacing);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void RotateFormation(int32 GroupID, float AngleDegrees);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    TArray<FMingCommandGroup> GetAllCommandGroups() const;

    // Camera & View
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void FocusOnUnit(int32 UnitID);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void FocusOnSelection();

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void FocusOnBattleCenter(const FString& BattleID);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraHeight(float Height);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraRotation(float Yaw, float Pitch);

    // Combat Statistics
    UFUNCTION(BlueprintCallable, Category = "Combat Stats")
    float GetBattleProgress(const FString& BattleID) const;

    UFUNCTION(BlueprintCallable, Category = "Combat Stats")
    void ReportCasualties(int32 UnitID, int32 CasualtyCount);

    UFUNCTION(BlueprintCallable, Category = "Combat Stats")
    TMap<FString, int32> GetCasualtyReport(const FString& BattleID) const;

    UFUNCTION(BlueprintCallable, Category = "Combat Stats")
    float GetFactionStrengthRatio(const FString& BattleID) const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBattleStarted OnBattleStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBattleEnded OnBattleEnded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnUnitSelected OnUnitSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnOrderExecuted OnOrderExecuted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCasualtiesReported OnCasualtiesReported;

protected:
    UPROPERTY()
    TMap<FString, FMingBattleInfo> ActiveBattles;

    UPROPERTY()
    TMap<int32, FMingGameTacticalUnit> Units;

    UPROPERTY()
    TMap<int32, FMingCommandGroup> CommandGroups;

    UPROPERTY()
    TArray<int32> SelectedUnitIDs;

    UPROPERTY()
    FString CurrentBattleID;

    UPROPERTY()
    int32 NextUnitID = 1;

    UPROPERTY()
    int32 NextGroupID = 1;

    UPROPERTY()
    bool bIsActive = false;

    UPROPERTY()
    FVector CameraPosition;

    UPROPERTY()
    float CameraHeight = 2000.0f;

    // Internal functions
    void ProcessBattleTick(float DeltaTime);
    void UpdateUnitAI(float DeltaTime);
    void CheckBattleEndConditions(const FString& BattleID);
    void CalculateBattleOutcome(const FString& BattleID);
    
    void OnUnitDestroyed(int32 UnitID);
    void NotifySelectionChanged();
    
    bool CanAttack(int32 AttackerID, int32 DefenderID) const;
    float CalculateDamage(const FMingGameTacticalUnit& Attacker, const FMingGameTacticalUnit& Defender) const;
    
    void InitializeDefaultUnits(const FString& BattleID);
    void CleanupBattle(const FString& BattleID);
);

