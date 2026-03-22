#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGameFlowManager.generated.h"

UENUM(BlueprintType)
enum class EMingGamePhase : uint8
{
    MainMenu UMETA(DisplayName = "Main Menu"),
    CampaignSelection UMETA(DisplayName = "Campaign Selection"),
    MissionBriefing UMETA(DisplayName = "Mission Briefing"),
    InGame UMETA(DisplayName = "In Game"),
    PauseMenu UMETA(DisplayName = "Pause Menu"),
    MissionComplete UMETA(DisplayName = "Mission Complete"),
    MissionFailed UMETA(DisplayName = "Mission Failed"),
    CampaignComplete UMETA(DisplayName = "Campaign Complete"),
    Credits UMETA(DisplayName = "Credits")
};

UENUM(BlueprintType)
enum class EMingGameMode : uint8
{
    Campaign UMETA(DisplayName = "Campaign"),
    Skirmish UMETA(DisplayName = "Skirmish"),
    Mercenary UMETA(DisplayName = "Mercenary"),
    Tutorial UMETA(DisplayName = "Tutorial"),
    Sandbox UMETA(DisplayName = "Sandbox")
};

UENUM(BlueprintType)
enum class EMingDifficulty : uint8
{
    Easy UMETA(DisplayName = "Easy"),
    Normal UMETA(DisplayName = "Normal"),
    Hard UMETA(DisplayName = "Hard"),
    Expert UMETA(DisplayName = "Expert"),
    Legendary UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FMingGameSession
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SessionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingGameMode GameMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CampaignID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MissionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ElapsedTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPaused;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> CompletedObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> FailedObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> PlayerStats;

    FMingGameSession()
        : GameMode(EMingGameMode::Campaign)
        , Difficulty(EMingDifficulty::Normal)
        , StartTime(0.0f)
        , ElapsedTime(0.0f)
        , bIsPaused(false)
        , bIsCompleted(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingGameFlowState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingGamePhase CurrentPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingGamePhase PreviousPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanTransition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TransitionReason;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMingGamePhase> AllowedTransitions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PhaseStartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PhaseDuration;

    FMingGameFlowState()
        : CurrentPhase(EMingGamePhase::MainMenu)
        , PreviousPhase(EMingGamePhase::MainMenu)
        , bCanTransition(true)
        , PhaseStartTime(0.0f)
        , PhaseDuration(0.0f)
    {}
};

/**
 * Game Flow Manager for MingGoRTS
 * Manages complete game flow from main menu to credits
 */
UCLASS(ClassGroup = (GameFlow), Blueprintable)
class MINGGAMEFLOW_API UMingGameFlowManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGameFlowManager(};

    // Game Flow Management
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void InitializeGameFlow(};

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void StartNewGame(EMingGameMode GameMode, EMingDifficulty Difficulty};

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void LoadGame(const FString& SaveSlot) {};

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void SaveGame(const FString& SaveSlot) {};

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void ExitGame(};

    // Phase Management
    UFUNCTION(BlueprintCallable, Category = "Phase")
    void TransitionToPhase(EMingGamePhase NewPhase, const FString& Reason) {};

    UFUNCTION(BlueprintCallable, Category = "Phase")
    void ReturnToPreviousPhase(};

    UFUNCTION(BlueprintCallable, Category = "Phase")
    void PauseGame(};

    UFUNCTION(BlueprintCallable, Category = "Phase")
    void ResumeGame(};

    UFUNCTION(BlueprintCallable, Category = "Phase")
    void RestartMission(};

    UFUNCTION(BlueprintCallable, Category = "Phase")
    void AbortMission(};

    UFUNCTION(BlueprintPure, Category = "Phase")
    EMingGamePhase GetCurrentPhase() const;

    UFUNCTION(BlueprintPure, Category = "Phase")
    EMingGamePhase GetPreviousPhase() const;

    UFUNCTION(BlueprintPure, Category = "Phase")
    bool CanTransitionToPhase(EMingGamePhase Phase) const;

    // Session Management
    UFUNCTION(BlueprintCallable, Category = "Session")
    void CreateSession(EMingGameMode GameMode, EMingDifficulty Difficulty};

    UFUNCTION(BlueprintCallable, Category = "Session")
    void EndSession(};

    UFUNCTION(BlueprintPure, Category = "Session")
    FMingGameSession GetCurrentSession() const;

    UFUNCTION(BlueprintPure, Category = "Session")
    bool HasActiveSession() const;

    UFUNCTION(BlueprintCallable, Category = "Session")
    void UpdateSessionStats(const FString& StatName, float Value};

    UFUNCTION(BlueprintCallable, Category = "Session")
    void CompleteObjective(const FString& ObjectiveID) {};

    UFUNCTION(BlueprintCallable, Category = "Session")
    void FailObjective(const FString& ObjectiveID) {};

    // Campaign Flow
    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void StartCampaign(const FString& CampaignID) {};

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void StartMission(const FString& MissionID) {};

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void CompleteMission(};

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void FailMission(};

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void NextMission(};

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void SkipMission(};

    UFUNCTION(BlueprintPure, Category = "Campaign")
    FString GetCurrentCampaign() const;

    UFUNCTION(BlueprintPure, Category = "Campaign")
    FString GetCurrentMission() const;

    // Tutorial Flow
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void StartTutorial(};

    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void SkipTutorial(};

    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void CompleteTutorialStep(const FString& StepID) {};

    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void ShowTutorialHint(const FString& HintID) {};

    UFUNCTION(BlueprintPure, Category = "Tutorial")
    bool IsTutorialActive() const;

    // Save/Load Management
    UFUNCTION(BlueprintCallable, Category = "Save/Load")
    void QuickSave(};

    UFUNCTION(BlueprintCallable, Category = "Save/Load")
    void QuickLoad(};

    UFUNCTION(BlueprintCallable, Category = "Save/Load")
    void AutoSave(};

    UFUNCTION(BlueprintPure, Category = "Save/Load")
    TArray<FString> GetAvailableSaveSlots() const;

    UFUNCTION(BlueprintPure, Category = "Save/Load")
    bool HasSaveData(const FString& SaveSlot) const;

    // Game State Validation
    UFUNCTION(BlueprintCallable, Category = "Validation")
    void ValidateGameState(};

    UFUNCTION(BlueprintCallable, Category = "Validation")
    void CheckWinConditions(};

    UFUNCTION(BlueprintCallable, Category = "Validation")
    void CheckLoseConditions(};

    UFUNCTION(BlueprintCallable, Category = "Validation")
    void CheckMissionObjectives(};

    UFUNCTION(BlueprintPure, Category = "Validation")
    bool IsGameValid() const;

    UFUNCTION(BlueprintPure, Category = "Validation")
    bool CanSaveGame() const;

    UFUNCTION(BlueprintPure, Category = "Validation")
    bool CanLoadGame() const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPhaseChanged, EMingGamePhase, OldPhase, EMingGamePhase, NewPhase};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStarted, EMingGameMode, GameMode, EMingDifficulty, Difficulty};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameEnded, const FMingGameSession&, Session, bool, bWasCompleted};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionCompleted, const FString&, MissionID, bool, bSuccess};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, const FString&, ObjectiveID};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPhaseChanged OnPhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGameStarted OnGameStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGameEnded OnGameEnded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMissionCompleted OnMissionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnObjectiveCompleted OnObjectiveCompleted;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetPhaseName(EMingGamePhase Phase};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetGameModeName(EMingGameMode GameMode};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetDifficultyName(EMingDifficulty Difficulty};

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveGameFlowData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadGameFlowData(const FString& JsonString) {};

protected:
    UPROPERTY()
    FMingGameFlowState FlowState;

    UPROPERTY()
    FMingGameSession CurrentSession;

    UPROPERTY()
    TMap<FString, FMingGameSession> SaveSlots;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializePhaseTransitions(};
    void SetupDefaultPhaseSettings(};

    // Phase Management
    void EnterPhase(EMingGamePhase Phase};
    void ExitPhase(EMingGamePhase Phase};
    void UpdatePhaseState(EMingGamePhase Phase};
    bool ValidatePhaseTransition(EMingGamePhase FromPhase, EMingGamePhase ToPhase) const;

    // Session Management
    void InitializeSession(};
    void UpdateSession(};
    void FinalizeSession(};

    // Campaign Flow
    void LoadCampaignData(};
    void LoadMissionData(};
    void ProcessMissionResults(};
    void UpdateCampaignProgress(};

    // Tutorial Flow
    void InitializeTutorial(};
    void ProcessTutorialStep(};
    void CompleteTutorial(};

    // Save/Load Management
    void CreateSaveSlot(const FString& SaveSlot) {};
    void DeleteSaveSlot(const FString& SaveSlot) {};
    void ValidateSaveData(};

    // Game State Management
    void UpdateGameTime(};
    void ProcessGameEvents(};
    void CheckGameIntegrity(};

    // Helpers
    FString GenerateSessionID(};
    void LogGameFlowEvent(const FString& Event, const FString& Details) {};
    void BroadcastPhaseChange(EMingGamePhase OldPhase, EMingGamePhase NewPhase};
};

