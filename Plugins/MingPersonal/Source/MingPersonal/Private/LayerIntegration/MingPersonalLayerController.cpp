#include "LayerIntegration/MingPersonalLayerController.h"

UMingPersonalLayerController::UMingPersonalLayerController()
    : CurrentMode(EMingPersonalMode::Exploration)
    , bInDialogue(false)
    , CurrentCameraDistance(300.0f)
    , bIsActive(false)
    , bIsSprinting(false)
    , bIsCrouching(false)
{
}

void UMingPersonalLayerController::InitializePersonalLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Personal Layer Controller"));
    
    // Initialize default characters
    InitializeDefaultCharacters();
    
    // Setup investigation clues
    SetupInteractionZones();
    
    CurrentMode = EMingPersonalMode::Exploration;
    bInDialogue = false;
    
    UE_LOG(LogTemp, Log, TEXT("Personal Layer initialized"));
}

void UMingPersonalLayerController::ShutdownPersonalLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Personal Layer Controller"));
    
    // End any active states
    if (bInDialogue)
    {
        EndDialogue();
    }
    
    if (!CurrentInvestigationID.IsEmpty())
    {
        EndInvestigation();
    }
    
    // Clear all data
    CharacterStates.Empty();
    InvestigationClues.Empty();
    ActiveEncounters.Empty();
    PossessedCharacterID.Empty();
    LockedTargetID.Empty();
    
    bIsActive = false;
}

void UMingPersonalLayerController::ActivatePersonalLayer()
{
    bIsActive = true;
    UE_LOG(LogTemp, Log, TEXT("Personal Layer activated"));
}

void UMingPersonalLayerController::DeactivatePersonalLayer()
{
    bIsActive = false;
    UE_LOG(LogTemp, Log, TEXT("Personal Layer deactivated"));
}

// Character Control
void UMingPersonalLayerController::PossessCharacter(const FString& CharacterID)
{
    FMingPersonalCharacterState* State = CharacterStates.Find(CharacterID);
    if (State)
    {
        PossessedCharacterID = CharacterID;
        UE_LOG(LogTemp, Log, TEXT("Now possessing character: %s"), *CharacterID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot possess character %s - not found"), *CharacterID);
    }
}

void UMingPersonalLayerController::UnpossessCharacter()
{
    UE_LOG(LogTemp, Log, TEXT("Unpossessing character: %s"), *PossessedCharacterID);
    PossessedCharacterID.Empty();
}

bool UMingPersonalLayerController::GetCharacterState(const FString& CharacterID, FMingPersonalCharacterState& OutState) const
{
    const FMingPersonalCharacterState* Found = CharacterStates.Find(CharacterID);
    if (Found)
    {
        OutState = *Found;
        return true;
    }
    return false;
}

void UMingPersonalLayerController::UpdateCharacterPosition(const FString& CharacterID, FVector NewPosition)
{
    FMingPersonalCharacterState* State = CharacterStates.Find(CharacterID);
    if (State)
    {
        State->CurrentPosition = NewPosition;
        
        // Check for nearby interactions
        if (CharacterID == PossessedCharacterID)
        {
            ProcessNearbyInteractions();
        }
    }
}

void UMingPersonalLayerController::UpdateCharacterRotation(const FString& CharacterID, FRotator NewRotation)
{
    FMingPersonalCharacterState* State = CharacterStates.Find(CharacterID);
    if (State)
    {
        State->CurrentRotation = NewRotation;
    }
}

void UMingPersonalLayerController::SetPersonalMode(EMingPersonalMode NewMode)
{
    if (CurrentMode == NewMode)
    {
        return;
    }
    
    // Exit current mode
    switch (CurrentMode)
    {
    case EMingPersonalMode::Combat:
        ExitCombatModeInternal();
        break;
    case EMingPersonalMode::Dialogue:
        ExitDialogueMode();
        break;
    case EMingPersonalMode::Investigation:
        ExitInvestigationMode();
        break;
    default:
        break;
    }
    
    CurrentMode = NewMode;
    
    // Enter new mode
    switch (NewMode)
    {
    case EMingPersonalMode::Combat:
        EnterCombatModeInternal();
        break;
    case EMingPersonalMode::Dialogue:
        // Dialogue mode is entered via StartDialogue
        break;
    case EMingPersonalMode::Investigation:
        EnterInvestigationMode();
        break;
    default:
        break;
    }
    
    NotifyModeChanged();
    
    UE_LOG(LogTemp, Log, TEXT("Personal mode changed to: %d"), (int32)NewMode);
}

// Movement & Interaction
void UMingPersonalLayerController::MoveCharacter(FVector Direction, float Speed)
{
    if (PossessedCharacterID.IsEmpty())
    {
        return;
    }
    
    FMingPersonalCharacterState* State = CharacterStates.Find(PossessedCharacterID);
    if (!State)
    {
        return;
    }
    
    float FinalSpeed = Speed;
    
    if (bIsSprinting && State->CurrentStamina > 0)
    {
        FinalSpeed *= 1.5f;
        State->CurrentStamina = FMath::Max(0, State->CurrentStamina - 1);
    }
    
    if (bIsCrouching)
    {
        FinalSpeed *= 0.5f;
    }
    
    // Update position
    FVector Movement = Direction.GetSafeNormal() * FinalSpeed;
    State->CurrentPosition += Movement;
    
    // Process nearby interactions
    ProcessNearbyInteractions();
}

void UMingPersonalLayerController::Sprint(bool bSprinting)
{
    bIsSprinting = bSprinting;
}

void UMingPersonalLayerController::Crouch(bool bCrouching)
{
    bIsCrouching = bCrouching;
}

void UMingPersonalLayerController::Jump()
{
    if (PossessedCharacterID.IsEmpty())
    {
        return;
    }
    
    FMingPersonalCharacterState* State = CharacterStates.Find(PossessedCharacterID);
    if (State && State->CurrentStamina >= 10)
    {
        State->CurrentStamina -= 10;
        UE_LOG(LogTemp, Log, TEXT("Character jumped"));
    }
}

void UMingPersonalLayerController::InteractWithNearest()
{
    if (PossessedCharacterID.IsEmpty())
    {
        return;
    }
    
    FMingPersonalCharacterState* State = CharacterStates.Find(PossessedCharacterID);
    if (!State || State->AvailableInteractions.Num() == 0)
    {
        return;
    }
    
    // Interact with first available interaction
    FString NearestInteraction = State->AvailableInteractions[0];
    InteractWith(NearestInteraction);
}

void UMingPersonalLayerController::InteractWith(const FString& TargetID)
{
    UE_LOG(LogTemp, Log, TEXT("Interacting with: %s"), *TargetID);
    
    // Check if target is a character (dialogue)
    if (CharacterStates.Contains(TargetID))
    {
        StartDialogue(TargetID, TEXT("Default"));
    }
    // Check if target is a clue
    else if (InvestigationClues.Contains(TargetID))
    {
        ExamineClue(TargetID);
    }
    // Check if target is an encounter
    else if (ActiveEncounters.Contains(TargetID))
    {
        AcceptEncounter(TargetID);
    }
}

void UMingPersonalLayerController::ExamineClue(const FString& ClueID)
{
    FMingInvestigationClue* Clue = InvestigationClues.Find(ClueID);
    if (!Clue)
    {
        return;
    }
    
    if (!Clue->bIsDiscovered)
    {
        DiscoverClue(ClueID);
    }
    else
    {
        ExamineClueDetail(ClueID);
    }
}

void UMingPersonalLayerController::PickUpItem(const FString& ItemID)
{
    UE_LOG(LogTemp, Log, TEXT("Picked up item: %s"), *ItemID);
    // Item pickup logic would go here
}

void UMingPersonalLayerController::UseItem(const FString& ItemID)
{
    UE_LOG(LogTemp, Log, TEXT("Used item: %s"), *ItemID);
    // Item usage logic would go here
}

// Dialogue System
void UMingPersonalLayerController::StartDialogue(const FString& TargetCharacterID, const FString& DialogueID)
{
    if (!CharacterStates.Contains(TargetCharacterID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start dialogue - target character not found: %s"), *TargetCharacterID);
        return;
    }
    
    // Setup dialogue context
    CurrentDialogue.DialogueID = DialogueID;
    CurrentDialogue.SpeakerCharacterID = TargetCharacterID;
    CurrentDialogue.ListenerCharacterID = PossessedCharacterID;
    CurrentDialogue.CurrentLineID = TEXT("Start");
    CurrentDialogue.CurrentLineText = TEXT("Hello, what brings you here?");
    CurrentDialogue.AvailableResponses = {
        TEXT("I'm looking for information."),
        TEXT("I need your help with something."),
        TEXT("Never mind."),
        TEXT("(Attack)")
    };
    CurrentDialogue.bCanExitDialogue = true;
    
    bInDialogue = true;
    
    // Update character states
    FMingPersonalCharacterState* State = CharacterStates.Find(PossessedCharacterID);
    if (State)
    {
        State->bIsInDialogue = true;
    }
    
    EnterDialogueMode(DialogueID);
    
    OnDialogueStarted.Broadcast(CurrentDialogue);
    
    UE_LOG(LogTemp, Log, TEXT("Started dialogue with %s"), *TargetCharacterID);
}

void UMingPersonalLayerController::EndDialogue()
{
    if (!bInDialogue)
    {
        return;
    }
    
    FString DialogueID = CurrentDialogue.DialogueID;
    
    bInDialogue = false;
    
    // Update character states
    FMingPersonalCharacterState* State = CharacterStates.Find(PossessedCharacterID);
    if (State)
    {
        State->bIsInDialogue = false;
    }
    
    ExitDialogueMode();
    
    OnDialogueEnded.Broadcast(DialogueID);
    
    UE_LOG(LogTemp, Log, TEXT("Ended dialogue: %s"), *DialogueID);
}

void UMingPersonalLayerController::SelectDialogueResponse(int32 ResponseIndex)
{
    if (!bInDialogue || ResponseIndex < 0 || ResponseIndex >= CurrentDialogue.AvailableResponses.Num())
    {
        return;
    }
    
    FString SelectedResponse = CurrentDialogue.AvailableResponses[ResponseIndex];
    
    OnDialogueResponseSelected.Broadcast(ResponseIndex, SelectedResponse);
    
    // Process the response
    ProcessDialogueResponse(ResponseIndex);
    
    UE_LOG(LogTemp, Log, TEXT("Selected dialogue response %d: %s"), ResponseIndex, *SelectedResponse);
}

void UMingPersonalLayerController::AdvanceDialogue()
{
    if (!bInDialogue)
    {
        return;
    }
    
    // In a real implementation, this would advance to the next line
    UE_LOG(LogTemp, Log, TEXT("Advanced dialogue"));
}

void UMingPersonalLayerController::SkipDialogueLine()
{
    if (!bInDialogue)
    {
        return;
    }
    
    // In a real implementation, this would skip the current line
    UE_LOG(LogTemp, Log, TEXT("Skipped dialogue line"));
}

void UMingPersonalLayerController::SetDialogueCameraFocus(const FString& CharacterID)
{
    UE_LOG(LogTemp, Log, TEXT("Dialogue camera focused on: %s"), *CharacterID);
}

// Combat System
void UMingPersonalLayerController::EnterCombatMode()
{
    SetPersonalMode(EMingPersonalMode::Combat);
}

void UMingPersonalLayerController::ExitCombatMode()
{
    SetPersonalMode(EMingPersonalMode::Exploration);
}

void UMingPersonalLayerController::Attack()
{
    if (CurrentMode != EMingPersonalMode::Combat)
    {
        return;
    }
    
    if (!LockedTargetID.IsEmpty())
    {
        ProcessCombatHit(LockedTargetID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Attack executed"));
}

void UMingPersonalLayerController::Block()
{
    if (CurrentMode != EMingPersonalMode::Combat)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Block executed"));
}

void UMingPersonalLayerController::Dodge(FVector Direction)
{
    if (CurrentMode != EMingPersonalMode::Combat)
    {
        return;
    }
    
    // Move character in dodge direction
    MoveCharacter(Direction, 500.0f);
    
    UE_LOG(LogTemp, Log, TEXT("Dodge executed"));
}

void UMingPersonalLayerController::UseSkill(const FString& SkillID)
{
    if (CurrentMode != EMingPersonalMode::Combat)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Used skill: %s"), *SkillID);
}

void UMingPersonalLayerController::LockOnTarget(const FString& TargetID)
{
    LockedTargetID = TargetID;
    UE_LOG(LogTemp, Log, TEXT("Locked on target: %s"), *TargetID);
}

void UMingPersonalLayerController::ClearLockOn()
{
    LockedTargetID.Empty();
    UE_LOG(LogTemp, Log, TEXT("Lock-on cleared"));
}

// Investigation System
void UMingPersonalLayerController::StartInvestigation(const FString& InvestigationID)
{
    CurrentInvestigationID = InvestigationID;
    SetPersonalMode(EMingPersonalMode::Investigation);
    
    UE_LOG(LogTemp, Log, TEXT("Started investigation: %s"), *InvestigationID);
}

void UMingPersonalLayerController::EndInvestigation()
{
    if (CurrentMode == EMingPersonalMode::Investigation)
    {
        SetPersonalMode(EMingPersonalMode::Exploration);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Ended investigation: %s"), *CurrentInvestigationID);
    CurrentInvestigationID.Empty();
}

void UMingPersonalLayerController::DiscoverClue(const FString& ClueID)
{
    FMingInvestigationClue* Clue = InvestigationClues.Find(ClueID);
    if (!Clue)
    {
        return;
    }
    
    if (!Clue->bIsDiscovered)
    {
        Clue->bIsDiscovered = true;
        
        OnClueDiscovered.Broadcast(*Clue);
        
        UE_LOG(LogTemp, Log, TEXT("Discovered clue: %s"), *Clue->ClueName);
    }
}

void UMingPersonalLayerController::ExamineClueDetail(const FString& ClueID)
{
    FMingInvestigationClue* Clue = InvestigationClues.Find(ClueID);
    if (Clue && Clue->bIsDiscovered)
    {
        UE_LOG(LogTemp, Log, TEXT("Examining clue: %s - %s"), *Clue->ClueName, *Clue->Description);
    }
}

void UMingPersonalLayerController::ConnectClues(const FString& ClueAID, const FString& ClueBID)
{
    UE_LOG(LogTemp, Log, TEXT("Connecting clues: %s and %s"), *ClueAID, *ClueBID);
}

TArray<FMingInvestigationClue> UMingPersonalLayerController::GetDiscoveredClues() const
{
    TArray<FMingInvestigationClue> Result;
    
    for (const auto& Pair : InvestigationClues)
    {
        if (Pair.Value.bIsDiscovered)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

TArray<FMingInvestigationClue> UMingPersonalLayerController::GetAllCluesInLocation(const FString& LocationID) const
{
    TArray<FMingInvestigationClue> Result;
    
    for (const auto& Pair : InvestigationClues)
    {
        if (Pair.Value.LocationID == LocationID)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

// Encounter System
void UMingPersonalLayerController::TriggerEncounter(const FMingPersonalEncounter& Encounter)
{
    ActiveEncounters.Add(Encounter.EncounterID, Encounter);
    
    OnEncounterStarted.Broadcast(Encounter);
    
    UE_LOG(LogTemp, Log, TEXT("Triggered encounter: %s"), *Encounter.EncounterName);
}

void UMingPersonalLayerController::AvoidEncounter(const FString& EncounterID)
{
    FMingPersonalEncounter* Encounter = ActiveEncounters.Find(EncounterID);
    if (Encounter && Encounter->bCanAvoid)
    {
        ActiveEncounters.Remove(EncounterID);
        UE_LOG(LogTemp, Log, TEXT("Avoided encounter: %s"), *EncounterID);
    }
}

void UMingPersonalLayerController::AcceptEncounter(const FString& EncounterID)
{
    FMingPersonalEncounter* Encounter = ActiveEncounters.Find(EncounterID);
    if (!Encounter)
    {
        return;
    }
    
    if (Encounter->bIsHostile)
    {
        // Enter combat mode
        SetPersonalMode(EMingPersonalMode::Combat);
    }
    else
    {
        // Start dialogue or investigation
        if (!Encounter->EnemyCharacterID.IsEmpty())
        {
            StartDialogue(Encounter->EnemyCharacterID, TEXT("Encounter"));
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Accepted encounter: %s"), *Encounter->EncounterName);
}

void UMingPersonalLayerController::ResolveEncounter(const FString& EncounterID, bool bVictory)
{
    FMingPersonalEncounter* Encounter = ActiveEncounters.Find(EncounterID);
    if (!Encounter)
    {
        return;
    }
    
    OnEncounterResolved.Broadcast(EncounterID, bVictory);
    
    ActiveEncounters.Remove(EncounterID);
    
    UE_LOG(LogTemp, Log, TEXT("Resolved encounter %s: %s"), 
        *EncounterID, bVictory ? TEXT("Victory") : TEXT("Defeat"));
}

void UMingPersonalLayerController::ConvertEncounterToTactical(const FString& EncounterID)
{
    FMingPersonalEncounter* Encounter = ActiveEncounters.Find(EncounterID);
    if (!Encounter)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Converting encounter to tactical: %s"), *EncounterID);
    
    // This would trigger a transition to tactical layer
    // The FourLayerManager would handle the actual layer switch
}

// Camera & View
void UMingPersonalLayerController::SetCameraMode(const FString& CameraMode)
{
    UE_LOG(LogTemp, Log, TEXT("Camera mode set to: %s"), *CameraMode);
}

void UMingPersonalLayerController::SetCameraDistance(float Distance)
{
    CurrentCameraDistance = FMath::Clamp(Distance, 100.0f, 1000.0f);
}

void UMingPersonalLayerController::SetFieldOfView(float FOV)
{
    UE_LOG(LogTemp, Log, TEXT("Field of view set to: %f"), FOV);
}

void UMingPersonalLayerController::FocusCameraOnTarget(const FString& TargetID)
{
    UE_LOG(LogTemp, Log, TEXT("Camera focused on: %s"), *TargetID);
}

void UMingPersonalLayerController::ResetCameraToCharacter()
{
    UE_LOG(LogTemp, Log, TEXT("Camera reset to possessed character"));
}

// Return to other layers
void UMingPersonalLayerController::ReturnToTactical()
{
    UE_LOG(LogTemp, Log, TEXT("Returning to tactical layer"));
    
    // Exit personal mode
    if (bInDialogue)
    {
        EndDialogue();
    }
    
    if (CurrentMode == EMingPersonalMode::Combat)
    {
        ExitCombatMode();
    }
}

void UMingPersonalLayerController::ReturnToStrategic()
{
    UE_LOG(LogTemp, Log, TEXT("Returning to strategic layer"));
    
    // Clear any personal state
    UnpossessCharacter();
}

void UMingPersonalLayerController::TriggerStrategicDecisionFromPersonal(const FMingStrategicDecision& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("Triggering strategic decision from personal layer: %s"), *Decision.DecisionName);
    
    // This would be picked up by the FourLayerManager
}

// Internal functions
void UMingPersonalLayerController::InitializeDefaultCharacters()
{
    // Player character
    FMingPersonalCharacterState PlayerState;
    PlayerState.CharacterID = TEXT("Player");
    PlayerState.CurrentPosition = FVector::ZeroVector;
    PlayerState.CurrentRotation = FRotator::ZeroRotator;
    PlayerState.CurrentLocationID = TEXT("BaseCamp");
    PlayerState.CurrentHealth = 100;
    PlayerState.CurrentStamina = 100;
    CharacterStates.Add(PlayerState.CharacterID, PlayerState);
    
    // NPC characters
    FMingPersonalCharacterState NPC1;
    NPC1.CharacterID = TEXT("NPC_Commander");
    NPC1.CurrentPosition = FVector(100, 0, 0);
    NPC1.CurrentLocationID = TEXT("CommandPost");
    NPC1.CurrentHealth = 100;
    CharacterStates.Add(NPC1.CharacterID, NPC1);
    
    FMingPersonalCharacterState NPC2;
    NPC2.CharacterID = TEXT("NPC_Spy");
    NPC2.CurrentPosition = FVector(-100, 50, 0);
    NPC2.CurrentLocationID = TEXT("Tavern");
    NPC2.CurrentHealth = 80;
    CharacterStates.Add(NPC2.CharacterID, NPC2);
}

void UMingPersonalLayerController::SetupInteractionZones()
{
    // Setup investigation clues
    FMingInvestigationClue Clue1;
    Clue1.ClueID = TEXT("Clue_Letter");
    Clue1.ClueName = TEXT("Mysterious Letter");
    Clue1.Description = TEXT("A letter with a suspicious seal...");
    Clue1.LocationID = TEXT("CommandPost");
    Clue1.Importance = 3;
    InvestigationClues.Add(Clue1.ClueID, Clue1);
    
    FMingInvestigationClue Clue2;
    Clue2.ClueID = TEXT("Clue_Map");
    Clue2.ClueName = TEXT("Tactical Map");
    Clue2.Description = TEXT("A map showing enemy positions");
    Clue2.LocationID = TEXT("CommandPost");
    Clue2.Importance = 4;
    InvestigationClues.Add(Clue2.ClueID, Clue2);
    
    FMingInvestigationClue Clue3;
    Clue3.ClueID = TEXT("Clue_Dagger");
    Clue3.ClueName = TEXT("Ornate Dagger");
    Clue3.Description = TEXT("A dagger with foreign markings");
    Clue3.LocationID = TEXT("Tavern");
    Clue3.Importance = 2;
    InvestigationClues.Add(Clue3.ClueID, Clue3);
}

void UMingPersonalLayerController::ProcessNearbyInteractions()
{
    if (PossessedCharacterID.IsEmpty())
    {
        return;
    }
    
    FMingPersonalCharacterState* State = CharacterStates.Find(PossessedCharacterID);
    if (!State)
    {
        return;
    }
    
    State->AvailableInteractions.Empty();
    State->NearbyCharacters.Empty();
    
    // Check for nearby characters
    for (const auto& Pair : CharacterStates)
    {
        if (Pair.Key != PossessedCharacterID)
        {
            float Distance = FVector::Dist(State->CurrentPosition, Pair.Value.CurrentPosition);
            if (Distance < 200.0f) // Interaction range
            {
                State->NearbyCharacters.Add(Pair.Key);
                State->AvailableInteractions.Add(Pair.Key);
            }
        }
    }
    
    // Check for nearby clues
    for (const auto& Pair : InvestigationClues)
    {
        if (Pair.Value.LocationID == State->CurrentLocationID)
        {
            State->AvailableInteractions.Add(Pair.Key);
        }
    }
}

void UMingPersonalLayerController::UpdateCharacterStamina(float DeltaTime)
{
    for (auto& Pair : CharacterStates)
    {
        // Regenerate stamina over time
        if (Pair.Value.CurrentStamina < 100)
        {
            Pair.Value.CurrentStamina = FMath::Min(100, Pair.Value.CurrentStamina + 5 * DeltaTime);
        }
    }
}

void UMingPersonalLayerController::EnterDialogueMode(const FString& DialogueID)
{
    SetDialogueCameraFocus(CurrentDialogue.SpeakerCharacterID);
    UE_LOG(LogTemp, Log, TEXT("Entered dialogue mode: %s"), *DialogueID);
}

void UMingPersonalLayerController::ExitDialogueMode()
{
    ResetCameraToCharacter();
    UE_LOG(LogTemp, Log, TEXT("Exited dialogue mode"));
}

void UMingPersonalLayerController::ProcessDialogueResponse(int32 ResponseIndex)
{
    // In a real implementation, this would determine the next dialogue state
    switch (ResponseIndex)
    {
    case 0: // Information
        CurrentDialogue.CurrentLineText = TEXT("I might have some information for you...");
        break;
    case 1: // Help
        CurrentDialogue.CurrentLineText = TEXT("What do you need help with?");
        break;
    case 2: // Exit
        EndDialogue();
        break;
    case 3: // Attack
        EndDialogue();
        EnterCombatMode();
        break;
    default:
        break;
    }
}

void UMingPersonalLayerController::EnterInvestigationMode()
{
    ScanForClues();
    UE_LOG(LogTemp, Log, TEXT("Entered investigation mode"));
}

void UMingPersonalLayerController::ExitInvestigationMode()
{
    UE_LOG(LogTemp, Log, TEXT("Exited investigation mode"));
}

void UMingPersonalLayerController::ScanForClues()
{
    if (PossessedCharacterID.IsEmpty())
    {
        return;
    }
    
    FMingPersonalCharacterState* State = CharacterStates.Find(PossessedCharacterID);
    if (!State)
    {
        return;
    }
    
    // Auto-discover clues in current location
    for (auto& Pair : InvestigationClues)
    {
        if (Pair.Value.LocationID == State->CurrentLocationID && !Pair.Value.bIsDiscovered)
        {
            // Check distance (if we had position data for clues)
            DiscoverClue(Pair.Key);
        }
    }
}

void UMingPersonalLayerController::EnterCombatModeInternal()
{
    State->bIsInCombat = true;
    UE_LOG(LogTemp, Log, TEXT("Entered combat mode"));
}

void UMingPersonalLayerController::ExitCombatModeInternal()
{
    FMingPersonalCharacterState* State = CharacterStates.Find(PossessedCharacterID);
    if (State)
    {
        State->bIsInCombat = false;
    }
    LockedTargetID.Empty();
    UE_LOG(LogTemp, Log, TEXT("Exited combat mode"));
}

void UMingPersonalLayerController::ProcessCombatHit(const FString& TargetID)
{
    FMingPersonalCharacterState* Target = CharacterStates.Find(TargetID);
    if (Target)
    {
        // Calculate damage
        int32 Damage = FMath::RandRange(10, 25);
        Target->CurrentHealth = FMath::Max(0, Target->CurrentHealth - Damage);
        
        UE_LOG(LogTemp, Log, TEXT("Hit target %s for %d damage. Health remaining: %d"), 
            *TargetID, Damage, Target->CurrentHealth);
        
        if (Target->CurrentHealth <= 0)
        {
            UE_LOG(LogTemp, Log, TEXT("Target %s defeated!"), *TargetID);
        }
    }
}

void UMingPersonalLayerController::NotifyModeChanged()
{
    OnPersonalModeChanged.Broadcast(CurrentMode);
}

void UMingPersonalLayerController::CheckEncounterTriggers()
{
    // In a real implementation, this would check for random encounters
    // based on location, time, etc.
}
