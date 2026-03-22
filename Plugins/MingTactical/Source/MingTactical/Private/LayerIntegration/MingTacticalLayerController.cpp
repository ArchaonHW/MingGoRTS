#include "LayerIntegration/MingTacticalLayerController.h"
#include "Engine/World.h"

UMingTacticalLayerController::UMingTacticalLayerController()
    : NextUnitID(1)
    , NextGroupID(1)
    , bIsActive(false)
    , CameraHeight(2000.0f)
{
}

void UMingTacticalLayerController::InitializeTacticalLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Tactical Layer Controller"));
    
    // Clear any existing data
    ActiveBattles.Empty();
    Units.Empty();
    CommandGroups.Empty();
    SelectedUnitIDs.Empty();
    CurrentBattleID.Empty();
    
    NextUnitID = 1;
    NextGroupID = 1;
    
    UE_LOG(LogTemp, Log, TEXT("Tactical Layer initialized"));
}

void UMingTacticalLayerController::ShutdownTacticalLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Tactical Layer Controller"));
    
    // End all active battles
    TArray<FString> BattleIDs;
    ActiveBattles.GetKeys(BattleIDs);
    for (const FString& BattleID : BattleIDs)
    {
        AbortBattle(BattleID);
    }
    
    ClearSelection();
    Units.Empty();
    CommandGroups.Empty();
    
    bIsActive = false;
}

void UMingTacticalLayerController::ActivateTacticalLayer()
{
    bIsActive = true;
    UE_LOG(LogTemp, Log, TEXT("Tactical Layer activated"));
}

void UMingTacticalLayerController::DeactivateTacticalLayer()
{
    bIsActive = false;
    UE_LOG(LogTemp, Log, TEXT("Tactical Layer deactivated"));
}

// Battle Management
FString UMingTacticalLayerController::StartBattle(const FMingBattleInfo& BattleSetup)
{
    FString BattleID = FGuid::NewGuid().ToString();
    
    FMingBattleInfo Battle = BattleSetup;
    Battle.BattleID = BattleID;
    Battle.CurrentPhase = EMingBattlePhase::Deployment;
    Battle.bIsActive = true;
    Battle.BattleDuration = 0.0f;
    Battle.AttackerCasualties = 0;
    Battle.DefenderCasualties = 0;
    
    ActiveBattles.Add(BattleID, Battle);
    CurrentBattleID = BattleID;
    
    // Initialize default units for this battle
    InitializeDefaultUnits(BattleID);
    
    UE_LOG(LogTemp, Log, TEXT("Battle started: %s - %s vs %s"), 
        *Battle.BattleName, *Battle.AttackingFaction, *Battle.DefendingFaction);
    
    OnBattleStarted.Broadcast(Battle);
    
    return BattleID;
}

void UMingTacticalLayerController::EndBattle(const FString& BattleID, bool bAttackerVictory)
{
    FMingBattleInfo* Battle = ActiveBattles.Find(BattleID);
    if (!Battle)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot end battle - not found: %s"), *BattleID);
        return;
    }
    
    Battle->bIsActive = false;
    Battle->bVictory = bAttackerVictory;
    Battle->CurrentPhase = EMingBattlePhase::Aftermath;
    
    UE_LOG(LogTemp, Log, TEXT("Battle ended: %s - Winner: %s"), 
        *Battle->BattleName, bAttackerVictory ? *Battle->AttackingFaction : *Battle->DefendingFaction);
    
    OnBattleEnded.Broadcast(*Battle);
    
    CleanupBattle(BattleID);
}

void UMingTacticalLayerController::AbortBattle(const FString& BattleID)
{
    FMingBattleInfo* Battle = ActiveBattles.Find(BattleID);
    if (Battle)
    {
        Battle->bIsActive = false;
        UE_LOG(LogTemp, Log, TEXT("Battle aborted: %s"), *Battle->BattleName);
        
        CleanupBattle(BattleID);
    }
}

bool UMingTacticalLayerController::GetBattle(const FString& BattleID, FMingBattleInfo& OutBattle) const
{
    const FMingBattleInfo* Found = ActiveBattles.Find(BattleID);
    if (Found)
    {
        OutBattle = *Found;
        return true;
    }
    return false;
}

void UMingTacticalLayerController::AdvanceBattlePhase(const FString& BattleID)
{
    FMingBattleInfo* Battle = ActiveBattles.Find(BattleID);
    if (!Battle)
    {
        return;
    }
    
    // Advance to next phase
    switch (Battle->CurrentPhase)
    {
    case EMingBattlePhase::Deployment:
        Battle->CurrentPhase = EMingBattlePhase::Opening;
        UE_LOG(LogTemp, Log, TEXT("Battle phase advanced to Opening"));
        break;
    case EMingBattlePhase::Opening:
        Battle->CurrentPhase = EMingBattlePhase::MidBattle;
        UE_LOG(LogTemp, Log, TEXT("Battle phase advanced to Mid Battle"));
        break;
    case EMingBattlePhase::MidBattle:
        Battle->CurrentPhase = EMingBattlePhase::Climax;
        UE_LOG(LogTemp, Log, TEXT("Battle phase advanced to Climax"));
        break;
    case EMingBattlePhase::Climax:
        Battle->CurrentPhase = EMingBattlePhase::Resolution;
        UE_LOG(LogTemp, Log, TEXT("Battle phase advanced to Resolution"));
        break;
    case EMingBattlePhase::Resolution:
        Battle->CurrentPhase = EMingBattlePhase::Aftermath;
        UE_LOG(LogTemp, Log, TEXT("Battle phase advanced to Aftermath"));
        break;
    default:
        break;
    }
}

void UMingTacticalLayerController::SetBattlePhase(const FString& BattleID, EMingBattlePhase NewPhase)
{
    FMingBattleInfo* Battle = ActiveBattles.Find(BattleID);
    if (Battle)
    {
        Battle->CurrentPhase = NewPhase;
        UE_LOG(LogTemp, Log, TEXT("Battle phase set to: %d"), (int32)NewPhase);
    }
}

bool UMingTacticalLayerController::IsBattleActive(const FString& BattleID) const
{
    const FMingBattleInfo* Battle = ActiveBattles.Find(BattleID);
    return Battle && Battle->bIsActive;
}

FMingBattleInfo UMingTacticalLayerController::GetCurrentBattle() const
{
    FMingBattleInfo Battle;
    GetBattle(CurrentBattleID, Battle);
    return Battle;
}

// Unit Management
int32 UMingTacticalLayerController::SpawnUnit(const FMingTacticalUnit& UnitSetup)
{
    int32 UnitID = NextUnitID++;
    
    FMingTacticalUnit Unit = UnitSetup;
    Unit.UnitID = UnitID;
    Unit.bIsAlive = true;
    Unit.Health = Unit.MaxHealth;
    Unit.Morale = 100;
    Unit.Ammunition = 100;
    
    Units.Add(UnitID, Unit);
    
    UE_LOG(LogTemp, Log, TEXT("Unit spawned: ID=%d, Type=%s, Faction=%s"), 
        UnitID, *Unit.UnitType, *Unit.FactionID);
    
    return UnitID;
}

void UMingTacticalLayerController::RemoveUnit(int32 UnitID)
{
    Units.Remove(UnitID);
    SelectedUnitIDs.Remove(UnitID);
    
    // Remove from any command groups
    for (auto& Pair : CommandGroups)
    {
        Pair.Value.UnitIDs.Remove(UnitID);
    }
}

bool UMingTacticalLayerController::GetUnit(int32 UnitID, FMingTacticalUnit& OutUnit) const
{
    const FMingTacticalUnit* Found = Units.Find(UnitID);
    if (Found)
    {
        OutUnit = *Found;
        return true;
    }
    return false;
}

void UMingTacticalLayerController::UpdateUnitPosition(int32 UnitID, FVector NewPosition)
{
    FMingTacticalUnit* Unit = Units.Find(UnitID);
    if (Unit && Unit->bIsAlive)
    {
        Unit->CurrentPosition = NewPosition;
    }
}

void UMingTacticalLayerController::UpdateUnitHealth(int32 UnitID, int32 NewHealth)
{
    FMingTacticalUnit* Unit = Units.Find(UnitID);
    if (Unit)
    {
        int32 OldHealth = Unit->Health;
        Unit->Health = FMath::Clamp(NewHealth, 0, Unit->MaxHealth);
        
        if (Unit->Health <= 0 && OldHealth > 0)
        {
            DestroyUnit(UnitID);
        }
    }
}

void UMingTacticalLayerController::UpdateUnitMorale(int32 UnitID, int32 NewMorale)
{
    FMingTacticalUnit* Unit = Units.Find(UnitID);
    if (Unit)
    {
        Unit->Morale = FMath::Clamp(NewMorale, 0, 100);
    }
}

void UMingTacticalLayerController::DestroyUnit(int32 UnitID)
{
    FMingTacticalUnit* Unit = Units.Find(UnitID);
    if (Unit)
    {
        Unit->bIsAlive = false;
        Unit->Health = 0;
        
        UE_LOG(LogTemp, Log, TEXT("Unit destroyed: ID=%d"), UnitID);
        
        // Remove from selection
        SelectedUnitIDs.Remove(UnitID);
        
        // Notify
        OnUnitDestroyed(UnitID);
    }
}

TArray<FMingTacticalUnit> UMingTacticalLayerController::GetAllUnits() const
{
    TArray<FMingTacticalUnit> Result;
    Units.GenerateValueArray(Result);
    return Result;
}

TArray<FMingTacticalUnit> UMingTacticalLayerController::GetUnitsByFaction(const FString& FactionID) const
{
    TArray<FMingTacticalUnit> Result;
    
    for (const auto& Pair : Units)
    {
        if (Pair.Value.FactionID == FactionID)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

TArray<FMingTacticalUnit> UMingTacticalLayerController::GetUnitsInRadius(FVector Center, float Radius) const
{
    TArray<FMingTacticalUnit> Result;
    float RadiusSquared = Radius * Radius;
    
    for (const auto& Pair : Units)
    {
        float DistSquared = FVector::DistSquared(Pair.Value.CurrentPosition, Center);
        if (DistSquared <= RadiusSquared)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

int32 UMingTacticalLayerController::GetUnitCountByFaction(const FString& FactionID) const
{
    int32 Count = 0;
    for (const auto& Pair : Units)
    {
        if (Pair.Value.FactionID == FactionID)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingTacticalLayerController::GetLiveUnitCountByFaction(const FString& FactionID) const
{
    int32 Count = 0;
    for (const auto& Pair : Units)
    {
        if (Pair.Value.FactionID == FactionID && Pair.Value.bIsAlive)
        {
            Count++;
        }
    }
    return Count;
}

// Unit Selection
void UMingTacticalLayerController::SelectUnit(int32 UnitID)
{
    FMingTacticalUnit* Unit = Units.Find(UnitID);
    if (Unit && !Unit->bIsSelected)
    {
        Unit->bIsSelected = true;
        SelectedUnitIDs.AddUnique(UnitID);
        
        OnUnitSelected.Broadcast(UnitID, true);
    }
}

void UMingTacticalLayerController::DeselectUnit(int32 UnitID)
{
    FMingTacticalUnit* Unit = Units.Find(UnitID);
    if (Unit && Unit->bIsSelected)
    {
        Unit->bIsSelected = false;
        SelectedUnitIDs.Remove(UnitID);
        
        OnUnitSelected.Broadcast(UnitID, false);
    }
}

void UMingTacticalLayerController::SelectUnitsInBox(FVector2D ScreenStart, FVector2D ScreenEnd)
{
    // In a real implementation, this would convert screen coordinates to world
    // and select all units within that box
    UE_LOG(LogTemp, Log, TEXT("Selecting units in box from (%f, %f) to (%f, %f)"), 
        ScreenStart.X, ScreenStart.Y, ScreenEnd.X, ScreenEnd.Y);
}

void UMingTacticalLayerController::SelectAllUnits(const FString& FactionID)
{
    ClearSelection();
    
    for (auto& Pair : Units)
    {
        if (Pair.Value.FactionID == FactionID && Pair.Value.bIsAlive)
        {
            Pair.Value.bIsSelected = true;
            SelectedUnitIDs.Add(Pair.Key);
        }
    }
    
    NotifySelectionChanged();
    UE_LOG(LogTemp, Log, TEXT("Selected all %d units from faction %s"), 
        SelectedUnitIDs.Num(), *FactionID);
}

void UMingTacticalLayerController::ClearSelection()
{
    for (int32 UnitID : SelectedUnitIDs)
    {
        FMingTacticalUnit* Unit = Units.Find(UnitID);
        if (Unit)
        {
            Unit->bIsSelected = false;
        }
    }
    
    SelectedUnitIDs.Empty();
    NotifySelectionChanged();
    
    UE_LOG(LogTemp, Log, TEXT("Selection cleared"));
}

TArray<int32> UMingTacticalLayerController::GetSelectedUnits() const
{
    return SelectedUnitIDs;
}

int32 UMingTacticalLayerController::GetSelectedCount() const
{
    return SelectedUnitIDs.Num();
}

// Command Execution
void UMingTacticalLayerController::ExecuteOrder(const FMingTacticalOrder& Order)
{
    UE_LOG(LogTemp, Log, TEXT("Executing order: %s (Type: %d)"), 
        *Order.OrderID, (int32)Order.CommandType);
    
    // Execute based on command type
    for (int32 UnitID : Order.UnitIDs)
    {
        FMingTacticalUnit* Unit = Units.Find(UnitID);
        if (Unit && Unit->bIsAlive)
        {
            Unit->TargetPosition = Order.TargetLocation;
            Unit->CurrentStance = EMingUnitStance::Hold;
            
            // In real implementation, this would trigger the actual movement/combat logic
        }
    }
    
    OnOrderExecuted.Broadcast(Order);
}

void UMingTacticalLayerController::ExecuteOrderOnSelection(const FMingTacticalOrder& Order)
{
    FMingTacticalOrder SelectionOrder = Order;
    SelectionOrder.UnitIDs = SelectedUnitIDs;
    
    ExecuteOrder(SelectionOrder);
}

void UMingTacticalLayerController::MoveSelectedUnits(FVector TargetLocation, EMingUnitStance Stance)
{
    if (SelectedUnitIDs.Num() == 0)
    {
        return;
    }
    
    FMingTacticalOrder Order;
    Order.OrderID = FGuid::NewGuid().ToString();
    Order.CommandType = EMingTacticalCommand::None; // Move is implicit
    Order.UnitIDs = SelectedUnitIDs;
    Order.TargetLocation = TargetLocation;
    Order.Priority = 1.0f;
    
    for (int32 UnitID : SelectedUnitIDs)
    {
        FMingTacticalUnit* Unit = Units.Find(UnitID);
        if (Unit)
        {
            Unit->TargetPosition = TargetLocation;
            Unit->CurrentStance = Stance;
        }
    }
    
    OnOrderExecuted.Broadcast(Order);
    
    UE_LOG(LogTemp, Log, TEXT("Moving %d units to (%f, %f, %f)"), 
        SelectedUnitIDs.Num(), TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
}

void UMingTacticalLayerController::AttackTarget(int32 TargetUnitID)
{
    if (SelectedUnitIDs.Num() == 0)
    {
        return;
    }
    
    FMingTacticalUnit TargetUnit;
    if (!GetUnit(TargetUnitID, TargetUnit))
    {
        return;
    }
    
    FMingTacticalOrder Order;
    Order.OrderID = FGuid::NewGuid().ToString();
    Order.CommandType = EMingTacticalCommand::Attack;
    Order.UnitIDs = SelectedUnitIDs;
    Order.TargetLocation = TargetUnit.CurrentPosition;
    Order.TargetUnitID = TargetUnitID;
    Order.Priority = 2.0f;
    
    ExecuteOrder(Order);
}

void UMingTacticalLayerController::AttackMove(FVector TargetLocation)
{
    if (SelectedUnitIDs.Num() == 0)
    {
        return;
    }
    
    FMingTacticalOrder Order;
    Order.OrderID = FGuid::NewGuid().ToString();
    Order.CommandType = EMingTacticalCommand::Attack;
    Order.UnitIDs = SelectedUnitIDs;
    Order.TargetLocation = TargetLocation;
    Order.Priority = 1.5f;
    
    ExecuteOrder(Order);
}

void UMingTacticalLayerController::HoldPosition()
{
    SetUnitStance(EMingUnitStance::Hold);
    
    UE_LOG(LogTemp, Log, TEXT("Selected units holding position"));
}

void UMingTacticalLayerController::RetreatAllUnits(const FString& FactionID)
{
    TArray<FMingTacticalUnit> FactionUnits = GetUnitsByFaction(FactionID);
    
    for (auto& Unit : FactionUnits)
    {
        if (Unit.bIsAlive)
        {
            FMingTacticalUnit* UnitPtr = Units.Find(Unit.UnitID);
            if (UnitPtr)
            {
                UnitPtr->CurrentStance = EMingUnitStance::Retreat;
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("All %s units retreating"), *FactionID);
}

void UMingTacticalLayerController::SetUnitStance(EMingUnitStance NewStance)
{
    for (int32 UnitID : SelectedUnitIDs)
    {
        FMingTacticalUnit* Unit = Units.Find(UnitID);
        if (Unit)
        {
            Unit->CurrentStance = NewStance;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Set stance to %d for %d units"), 
        (int32)NewStance, SelectedUnitIDs.Num());
}

void UMingTacticalLayerController::StopCurrentAction()
{
    for (int32 UnitID : SelectedUnitIDs)
    {
        FMingTacticalUnit* Unit = Units.Find(UnitID);
        if (Unit)
        {
            Unit->TargetPosition = Unit->CurrentPosition;
            Unit->CurrentStance = EMingUnitStance::Hold;
        }
    }
}

// Formation Management
int32 UMingTacticalLayerController::CreateCommandGroup(const TArray<int32>& UnitIDs, const FString& GroupName)
{
    int32 GroupID = NextGroupID++;
    
    FMingCommandGroup Group;
    Group.GroupID = GroupID;
    Group.GroupName = GroupName;
    Group.UnitIDs = UnitIDs;
    
    // Calculate formation center
    FVector Center = FVector::ZeroVector;
    int32 ValidCount = 0;
    
    for (int32 UnitID : UnitIDs)
    {
        FMingTacticalUnit* Unit = Units.Find(UnitID);
        if (Unit)
        {
            Center += Unit->CurrentPosition;
            ValidCount++;
        }
    }
    
    if (ValidCount > 0)
    {
        Group.FormationCenter = Center / ValidCount;
    }
    
    CommandGroups.Add(GroupID, Group);
    
    UE_LOG(LogTemp, Log, TEXT("Created command group: %s with %d units"), 
        *GroupName, UnitIDs.Num());
    
    return GroupID;
}

void UMingTacticalLayerController::DisbandCommandGroup(int32 GroupID)
{
    CommandGroups.Remove(GroupID);
    UE_LOG(LogTemp, Log, TEXT("Disbanded command group: %d"), GroupID);
}

void UMingTacticalLayerController::SetFormation(int32 GroupID, const FString& FormationType, float Spacing)
{
    FMingCommandGroup* Group = CommandGroups.Find(GroupID);
    if (!Group)
    {
        return;
    }
    
    Group->FormationType = FormationType;
    
    // In real implementation, this would arrange units in the specified formation
    UE_LOG(LogTemp, Log, TEXT("Set formation %s for group %d with spacing %f"), 
        *FormationType, GroupID, Spacing);
}

void UMingTacticalLayerController::RotateFormation(int32 GroupID, float AngleDegrees)
{
    FMingCommandGroup* Group = CommandGroups.Find(GroupID);
    if (!Group)
    {
        return;
    }
    
    // In real implementation, this would rotate the formation
    UE_LOG(LogTemp, Log, TEXT("Rotated formation for group %d by %f degrees"), 
        GroupID, AngleDegrees);
}

TArray<FMingCommandGroup> UMingTacticalLayerController::GetAllCommandGroups() const
{
    TArray<FMingCommandGroup> Result;
    CommandGroups.GenerateValueArray(Result);
    return Result;
}

// Camera & View
void UMingTacticalLayerController::FocusOnUnit(int32 UnitID)
{
    FMingTacticalUnit Unit;
    if (GetUnit(UnitID, Unit))
    {
        CameraPosition = Unit.CurrentPosition;
        UE_LOG(LogTemp, Log, TEXT("Camera focused on unit %d"), UnitID);
    }
}

void UMingTacticalLayerController::FocusOnSelection()
{
    if (SelectedUnitIDs.Num() == 0)
    {
        return;
    }
    
    FVector Center = FVector::ZeroVector;
    int32 ValidCount = 0;
    
    for (int32 UnitID : SelectedUnitIDs)
    {
        FMingTacticalUnit Unit;
        if (GetUnit(UnitID, Unit))
        {
            Center += Unit.CurrentPosition;
            ValidCount++;
        }
    }
    
    if (ValidCount > 0)
    {
        CameraPosition = Center / ValidCount;
        UE_LOG(LogTemp, Log, TEXT("Camera focused on %d selected units"), ValidCount);
    }
}

void UMingTacticalLayerController::FocusOnBattleCenter(const FString& BattleID)
{
    FMingBattleInfo Battle;
    if (GetBattle(BattleID, Battle))
    {
        CameraPosition = Battle.BattleCenter;
        UE_LOG(LogTemp, Log, TEXT("Camera focused on battle center: %s"), *Battle.BattleName);
    }
}

void UMingTacticalLayerController::SetCameraHeight(float Height)
{
    CameraHeight = FMath::Clamp(Height, 500.0f, 5000.0f);
}

void UMingTacticalLayerController::SetCameraRotation(float Yaw, float Pitch)
{
    // In real implementation, this would set camera rotation
    UE_LOG(LogTemp, Log, TEXT("Camera rotation set: Yaw=%f, Pitch=%f"), Yaw, Pitch);
}

// Combat Statistics
float UMingTacticalLayerController::GetBattleProgress(const FString& BattleID) const
{
    FMingBattleInfo Battle;
    if (!GetBattle(BattleID, Battle))
    {
        return 0.0f;
    }
    
    // Calculate based on casualties or time
    int32 TotalCasualties = Battle.AttackerCasualties + Battle.DefenderCasualties;
    int32 TotalInitialStrength = 1000; // This would be tracked
    
    return FMath::Clamp(static_cast<float>(TotalCasualties) / TotalInitialStrength, 0.0f, 1.0f);
}

void UMingTacticalLayerController::ReportCasualties(int32 UnitID, int32 CasualtyCount)
{
    FMingTacticalUnit* Unit = Units.Find(UnitID);
    if (!Unit)
    {
        return;
    }
    
    FMingBattleInfo* Battle = ActiveBattles.Find(CurrentBattleID);
    if (!Battle)
    {
        return;
    }
    
    if (Unit->FactionID == Battle->AttackingFaction)
    {
        Battle->AttackerCasualties += CasualtyCount;
    }
    else if (Unit->FactionID == Battle->DefendingFaction)
    {
        Battle->DefenderCasualties += CasualtyCount;
    }
    
    OnCasualtiesReported.Broadcast(UnitID, CasualtyCount);
    
    // Check if battle should end
    CheckBattleEndConditions(CurrentBattleID);
}

TMap<FString, int32> UMingTacticalLayerController::GetCasualtyReport(const FString& BattleID) const
{
    TMap<FString, int32> Report;
    
    FMingBattleInfo Battle;
    if (GetBattle(BattleID, Battle))
    {
        Report.Add(Battle.AttackingFaction, Battle.AttackerCasualties);
        Report.Add(Battle.DefendingFaction, Battle.DefenderCasualties);
    }
    
    return Report;
}

float UMingTacticalLayerController::GetFactionStrengthRatio(const FString& BattleID) const
{
    FMingBattleInfo Battle;
    if (!GetBattle(BattleID, Battle))
    {
        return 1.0f;
    }
    
    int32 AttackerLive = GetLiveUnitCountByFaction(Battle.AttackingFaction);
    int32 DefenderLive = GetLiveUnitCountByFaction(Battle.DefendingFaction);
    
    if (DefenderLive == 0)
    {
        return 999.0f; // Attacker overwhelming
    }
    
    return static_cast<float>(AttackerLive) / DefenderLive;
}

// Internal functions
void UMingTacticalLayerController::CheckBattleEndConditions(const FString& BattleID)
{
    FMingBattleInfo* Battle = ActiveBattles.Find(BattleID);
    if (!Battle || !Battle->bIsActive)
    {
        return;
    }
    
    int32 AttackerLive = GetLiveUnitCountByFaction(Battle->AttackingFaction);
    int32 DefenderLive = GetLiveUnitCountByFaction(Battle->DefendingFaction);
    
    // Check victory conditions
    if (DefenderLive == 0)
    {
        EndBattle(BattleID, true); // Attacker wins
    }
    else if (AttackerLive == 0)
    {
        EndBattle(BattleID, false); // Defender wins
    }
    else if (Battle->BattleDuration > 1800.0f) // 30 minute time limit
    {
        // Draw or defender wins by holding
        EndBattle(BattleID, false);
    }
}

void UMingTacticalLayerController::OnUnitDestroyed(int32 UnitID)
{
    // Remove from selection
    SelectedUnitIDs.Remove(UnitID);
    
    // Check battle end conditions
    CheckBattleEndConditions(CurrentBattleID);
}

void UMingTacticalLayerController::NotifySelectionChanged()
{
    UE_LOG(LogTemp, Log, TEXT("Selection changed: %d units selected"), SelectedUnitIDs.Num());
}

void UMingTacticalLayerController::InitializeDefaultUnits(const FString& BattleID)
{
    FMingBattleInfo* Battle = ActiveBattles.Find(BattleID);
    if (!Battle)
    {
        return;
    }
    
    // Spawn attacker units
    for (int32 i = 0; i < 10; i++)
    {
        FMingTacticalUnit Unit;
        Unit.UnitName = FString::Printf(TEXT("Infantry_%d"), i);
        Unit.UnitType = TEXT("Infantry");
        Unit.FactionID = Battle->AttackingFaction;
        Unit.CurrentPosition = Battle->BattleCenter + FVector(-500 + i * 50, -300, 0);
        Unit.MaxHealth = 100;
        
        SpawnUnit(Unit);
    }
    
    // Spawn defender units
    for (int32 i = 0; i < 10; i++)
    {
        FMingTacticalUnit Unit;
        Unit.UnitName = FString::Printf(TEXT("Defender_%d"), i);
        Unit.UnitType = TEXT("Infantry");
        Unit.FactionID = Battle->DefendingFaction;
        Unit.CurrentPosition = Battle->BattleCenter + FVector(-250 + i * 50, 300, 0);
        Unit.MaxHealth = 100;
        
        SpawnUnit(Unit);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Initialized default units for battle: %s"), *Battle->BattleName);
}

void UMingTacticalLayerController::CleanupBattle(const FString& BattleID)
{
    FMingBattleInfo Battle;
    if (GetBattle(BattleID, Battle))
    {
        // Remove all units for this battle
        TArray<int32> UnitsToRemove;
        
        for (const auto& Pair : Units)
        {
            if (Pair.Value.FactionID == Battle.AttackingFaction || 
                Pair.Value.FactionID == Battle.DefendingFaction)
            {
                UnitsToRemove.Add(Pair.Key);
            }
        }
        
        for (int32 UnitID : UnitsToRemove)
        {
            RemoveUnit(UnitID);
        }
    }
    
    ActiveBattles.Remove(BattleID);
}
