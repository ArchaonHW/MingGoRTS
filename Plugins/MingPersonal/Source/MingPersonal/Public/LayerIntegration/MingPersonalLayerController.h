#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LayerIntegration/MingFourLayerTypes.h"
#include "MingPersonalLayerController.generated.h"

UENUM(BlueprintType)
enum class EMingPersonalMode : uint8
{
    Exploration UMETA(DisplayName = "Exploration"),
    Combat UMETA(DisplayName = "Combat"),
    Dialogue UMETA(DisplayName = "Dialogue"),
    Investigation UMETA(DisplayName = "Investigation"),
    Negotiation UMETA(DisplayName = "Negotiation"),
    Cutscene UMETA(DisplayName = "Cutscene")
};

USTRUCT(BlueprintType)
struct FMingPersonalCharacterState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    FVector CurrentPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    FRotator CurrentRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    EMingPersonalMode CurrentMode = EMingPersonalMode::Exploration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    bool bIsInCombat = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    bool bIsInDialogue = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    int32 CurrentHealth = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    int32 CurrentStamina = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    FString CurrentLocationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    FString CurrentWeaponID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    TArray<FString> NearbyCharacters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    TArray<FString> AvailableInteractions;

    FMingPersonalCharacterState()
        : CurrentMode(EMingPersonalMode::Exploration)
        , bIsInCombat(false)
        , bIsInDialogue(false)
        , CurrentHealth(100)
        , CurrentStamina(100)
    {}
};

USTRUCT(BlueprintType)
struct FMingDialogueContext
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString DialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString SpeakerCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString ListenerCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString CurrentLineID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString CurrentLineText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FString> AvailableResponses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bCanExitDialogue = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bIsImportantDialogue = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString DialogueTopic;

    FMingDialogueContext()
        : bCanExitDialogue(false)
        , bIsImportantDialogue(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingPersonalEncounter
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    FString EncounterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    FString EncounterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    FString EnemyCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    FString LocationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    bool bIsHostile = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    bool bCanAvoid = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    bool bIsBoss = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    TArray<FString> PossibleOutcomes;

    FMingPersonalEncounter()
        : bIsHostile(true)
        , bCanAvoid(true)
        , bIsBoss(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingInvestigationClue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation")
    FString ClueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation")
    FString ClueName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation")
    FString LocationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation")
    bool bIsDiscovered = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation")
    int32 Importance = 1; // 1-5 scale

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation")
    TArray<FString> RelatedClues;

    FMingInvestigationClue()
        : bIsDiscovered(false)
        , Importance(1)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPersonalModeChanged, EMingPersonalMode, NewMode};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStarted, const FMingDialogueContext&, Dialogue};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueEnded, const FString&, DialogueID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueResponseSelected, int32, ResponseIndex, const FString&, ResponseText};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEncounterStarted, const FMingPersonalEncounter&, Encounter};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEncounterResolved, const FString&, EncounterID, bool, bVictory};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClueDiscovered, const FMingInvestigationClue&, Clue};

/**
 * Personal Layer Controller
 * Manages individual character perspective gameplay, dialogue, and personal encounters
 */
UCLASS(ClassGroup = (Personal), Blueprintable)
class MINGPERSONAL_API UMingPersonalLayerController : public UObject
{
    GENERATED_BODY()

public:
    UMingPersonalLayerController(};

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void InitializePersonalLayer(};

    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void ShutdownPersonalLayer(};

    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void ActivatePersonalLayer(};

    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void DeactivatePersonalLayer(};

    // Character Control
    UFUNCTION(BlueprintCallable, Category = "Character Control")
    void PossessCharacter(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Character Control")
    void UnpossessCharacter(};

    UFUNCTION(BlueprintCallable, Category = "Character Control")
    bool GetCharacterState(const FString& CharacterID, FMingPersonalCharacterState& OutState) const;

    UFUNCTION(BlueprintCallable, Category = "Character Control")
    void UpdateCharacterPosition(const FString& CharacterID, FVector NewPosition};

    UFUNCTION(BlueprintCallable, Category = "Character Control")
    void UpdateCharacterRotation(const FString& CharacterID, FRotator NewRotation};

    UFUNCTION(BlueprintCallable, Category = "Character Control")
    void SetPersonalMode(EMingPersonalMode NewMode};

    UFUNCTION(BlueprintPure, Category = "Character Control")
    EMingPersonalMode GetCurrentMode() const { return CurrentMode; }

    UFUNCTION(BlueprintPure, Category = "Character Control")
    FString GetPossessedCharacterID() const { return PossessedCharacterID; }

    // Movement & Interaction
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveCharacter(FVector Direction, float Speed};

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Sprint(bool bSprinting};

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Crouch(bool bCrouching};

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Jump(};

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void InteractWithNearest(};

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void InteractWith(const FString& TargetID};

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void ExamineClue(const FString& ClueID};

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void PickUpItem(const FString& ItemID};

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void UseItem(const FString& ItemID};

    // Dialogue System
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void StartDialogue(const FString& TargetCharacterID, const FString& DialogueID};

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void EndDialogue(};

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SelectDialogueResponse(int32 ResponseIndex};

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void AdvanceDialogue(};

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SkipDialogueLine(};

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    bool IsInDialogue() const { return bInDialogue; }

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    FMingDialogueContext GetCurrentDialogue() const { return CurrentDialogue; }

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SetDialogueCameraFocus(const FString& CharacterID};

    // Combat System
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EnterCombatMode(};

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ExitCombatMode(};

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Attack(};

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Block(};

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Dodge(FVector Direction};

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void UseSkill(const FString& SkillID};

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void LockOnTarget(const FString& TargetID};

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ClearLockOn(};

    // Investigation System
    UFUNCTION(BlueprintCallable, Category = "Investigation")
    void StartInvestigation(const FString& InvestigationID};

    UFUNCTION(BlueprintCallable, Category = "Investigation")
    void EndInvestigation(};

    UFUNCTION(BlueprintCallable, Category = "Investigation")
    void DiscoverClue(const FString& ClueID};

    UFUNCTION(BlueprintCallable, Category = "Investigation")
    void ExamineClueDetail(const FString& ClueID};

    UFUNCTION(BlueprintCallable, Category = "Investigation")
    void ConnectClues(const FString& ClueAID, const FString& ClueBID};

    UFUNCTION(BlueprintPure, Category = "Investigation")
    TArray<FMingInvestigationClue> GetDiscoveredClues() const;

    UFUNCTION(BlueprintPure, Category = "Investigation")
    TArray<FMingInvestigationClue> GetAllCluesInLocation(const FString& LocationID) const;

    // Encounter System
    UFUNCTION(BlueprintCallable, Category = "Encounter")
    void TriggerEncounter(const FMingPersonalEncounter& Encounter};

    UFUNCTION(BlueprintCallable, Category = "Encounter")
    void AvoidEncounter(const FString& EncounterID};

    UFUNCTION(BlueprintCallable, Category = "Encounter")
    void AcceptEncounter(const FString& EncounterID};

    UFUNCTION(BlueprintCallable, Category = "Encounter")
    void ResolveEncounter(const FString& EncounterID, bool bVictory};

    UFUNCTION(BlueprintCallable, Category = "Encounter")
    void ConvertEncounterToTactical(const FString& EncounterID};

    // Camera & View
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraMode(const FString& CameraMode};

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraDistance(float Distance};

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetFieldOfView(float FOV};

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void FocusCameraOnTarget(const FString& TargetID};

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void ResetCameraToCharacter(};

    // Return to other layers
    UFUNCTION(BlueprintCallable, Category = "Layer Transition")
    void ReturnToTactical(};

    UFUNCTION(BlueprintCallable, Category = "Layer Transition")
    void ReturnToStrategic(};

    UFUNCTION(BlueprintCallable, Category = "Layer Transition")
    void TriggerStrategicDecisionFromPersonal(const FMingStrategicDecision& Decision};

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPersonalModeChanged OnPersonalModeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueEnded OnDialogueEnded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueResponseSelected OnDialogueResponseSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnEncounterStarted OnEncounterStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnEncounterResolved OnEncounterResolved;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnClueDiscovered OnClueDiscovered;

protected:
    UPROPERTY()
    TMap<FString, FMingPersonalCharacterState> CharacterStates;

    UPROPERTY()
    FString PossessedCharacterID;

    UPROPERTY()
    EMingPersonalMode CurrentMode;

    UPROPERTY()
    bool bInDialogue = false;

    UPROPERTY()
    FMingDialogueContext CurrentDialogue;

    UPROPERTY()
    TMap<FString, FMingInvestigationClue> InvestigationClues;

    UPROPERTY()
    TMap<FString, FMingPersonalEncounter> ActiveEncounters;

    UPROPERTY()
    FString CurrentInvestigationID;

    UPROPERTY()
    bool bIsSprinting = false;

    UPROPERTY()
    bool bIsCrouching = false;

    UPROPERTY()
    float CurrentCameraDistance = 300.0f;

    UPROPERTY()
    FString LockedTargetID;

    UPROPERTY()
    bool bIsActive = false;

    // Internal functions
    void InitializeDefaultCharacters(};
    void SetupInteractionZones(};
    void ProcessNearbyInteractions(};
    void UpdateCharacterStamina(float DeltaTime};
    
    void EnterDialogueMode(const FString& DialogueID};
    void ExitDialogueMode(};
    void ProcessDialogueResponse(int32 ResponseIndex};
    
    void EnterInvestigationMode(};
    void ExitInvestigationMode(};
    void ScanForClues(};
    
    void EnterCombatModeInternal(};
    void ExitCombatModeInternal(};
    void ProcessCombatHit(const FString& TargetID};
    
    void NotifyModeChanged(};
    void CheckEncounterTriggers(};
};

