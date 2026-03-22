// Copyright Epic Games, Inc. All Rights Reserved.

#include "System/MingGameInitializer.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"

UMingGameInitializer::UMingGameInitializer()
	: CurrentPhase(EGameInitPhase::PreInit)
	, bIsInitializing(false)
	, InitializationStartTime(0.0f)
{
}

void UMingGameInitializer::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Initialize all phases with default configs
	for (int32 i = 0; i < static_cast<int32>(EGameInitPhase::Error); ++i)
	{
		EGameInitPhase Phase = static_cast<EGameInitPhase>(i);
		
		FInitPhaseConfig Config;
		Config.Phase = Phase;
		Config.bEnabled = true;
		Config.TimeoutSeconds = 30.0f;
		
		PhaseConfigs.Add(Phase, Config);
		
		FInitPhaseStatus Status;
		Status.Phase = Phase;
		PhaseStatuses.Add(Phase, Status);
	}

	// Set up default dependencies
	PhaseDependencies.Add(EGameInitPhase::CoreSystems, { EGameInitPhase::Engine });
	PhaseDependencies.Add(EGameInitPhase::Database, { EGameInitPhase::CoreSystems });
	PhaseDependencies.Add(EGameInitPhase::Network, { EGameInitPhase::CoreSystems });
	PhaseDependencies.Add(EGameInitPhase::Audio, { EGameInitPhase::CoreSystems });
	PhaseDependencies.Add(EGameInitPhase::UI, { EGameInitPhase::CoreSystems });
	PhaseDependencies.Add(EGameInitPhase::AI, { EGameInitPhase::CoreSystems });
	PhaseDependencies.Add(EGameInitPhase::Localization, { EGameInitPhase::CoreSystems });
	PhaseDependencies.Add(EGameInitPhase::SaveLoad, { EGameInitPhase::Database });
	PhaseDependencies.Add(EGameInitPhase::GameMode, { EGameInitPhase::Network, EGameInitPhase::AI });
	PhaseDependencies.Add(EGameInitPhase::Content, { EGameInitPhase::Localization, EGameInitPhase::SaveLoad });
	PhaseDependencies.Add(EGameInitPhase::PlayerProfile, { EGameInitPhase::SaveLoad });
	PhaseDependencies.Add(EGameInitPhase::Ready, { EGameInitPhase::Content, EGameInitPhase::PlayerProfile });

	UE_LOG(LogTemp, Log, TEXT("Game Initializer Subsystem initialized"));
}

void UMingGameInitializer::Deinitialize()
{
	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(InitTimerHandle);
	}

	Super::Deinitialize();
}

void UMingGameInitializer::StartGameInitialization()
{
	if (bIsInitializing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game initialization already in progress"));
		return;
	}

	bIsInitializing = true;
	InitializationStartTime = FPlatformTime::Seconds();
	CurrentPhase = EGameInitPhase::PreInit;

	// Reset all phase statuses
	for (auto& Pair : PhaseStatuses)
	{
		Pair.Value.bStarted = false;
		Pair.Value.bCompleted = false;
		Pair.Value.bFailed = false;
		Pair.Value.Progress = 0.0f;
		Pair.Value.StartTime = 0.0f;
		Pair.Value.EndTime = 0.0f;
		Pair.Value.ErrorMessage.Empty();
	}

	UE_LOG(LogTemp, Log, TEXT("Starting game initialization sequence..."));
	AdvanceToNextPhase();
}

void UMingGameInitializer::SkipToPhase(EGameInitPhase TargetPhase)
{
	if (!bIsInitializing)
	{
		StartGameInitialization();
	}

	// Mark all phases before target as complete
	for (int32 i = 0; i < static_cast<int32>(TargetPhase); ++i)
	{
		EGameInitPhase Phase = static_cast<EGameInitPhase>(i);
		FInitPhaseStatus& Status = PhaseStatuses.FindOrAdd(Phase);
		Status.bStarted = true;
		Status.bCompleted = true;
		Status.Progress = 1.0f;
	}

	CurrentPhase = TargetPhase;
	ExecuteCurrentPhase();
}

void UMingGameInitializer::RestartInitialization()
{
	bIsInitializing = false;
	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(InitTimerHandle);
	}
	StartGameInitialization();
}

EGameInitPhase UMingGameInitializer::GetCurrentPhase() const
{
	return CurrentPhase;
}

float UMingGameInitializer::GetOverallProgress() const
{
	if (PhaseStatuses.Num() == 0)
	{
		return 0.0f;
	}

	float TotalProgress = 0.0f;
	int32 EnabledPhases = 0;

	for (const auto& Pair : PhaseStatuses)
	{
		const FInitPhaseConfig* Config = PhaseConfigs.Find(Pair.Key);
		if (Config && Config->bEnabled)
		{
			TotalProgress += Pair.Value.Progress;
			EnabledPhases++;
		}
	}

	return EnabledPhases > 0 ? TotalProgress / EnabledPhases : 0.0f;
}

bool UMingGameInitializer::IsInitializationComplete() const
{
	const FInitPhaseStatus* ReadyStatus = PhaseStatuses.Find(EGameInitPhase::Ready);
	return ReadyStatus && ReadyStatus->bCompleted;
}

bool UMingGameInitializer::IsPhaseComplete(EGameInitPhase Phase) const
{
	const FInitPhaseStatus* Status = PhaseStatuses.Find(Phase);
	return Status && Status->bCompleted;
}

TArray<FInitPhaseStatus> UMingGameInitializer::GetAllPhaseStatuses() const
{
	TArray<FInitPhaseStatus> Result;
	PhaseStatuses.GenerateValueArray(Result);
	Result.Sort([](const FInitPhaseStatus& A, const FInitPhaseStatus& B) {
		return static_cast<int32>(A.Phase) < static_cast<int32>(B.Phase);
	});
	return Result;
}

void UMingGameInitializer::ConfigurePhase(const FInitPhaseConfig& Config)
{
	PhaseConfigs.Add(Config.Phase, Config);
}

void UMingGameInitializer::SetPhaseEnabled(EGameInitPhase Phase, bool bEnabled)
{
	FInitPhaseConfig& Config = PhaseConfigs.FindOrAdd(Phase);
	Config.bEnabled = bEnabled;
}

void UMingGameInitializer::RegisterPhaseDependency(EGameInitPhase Phase, EGameInitPhase Dependency)
{
	TArray<EGameInitPhase>& Deps = PhaseDependencies.FindOrAdd(Phase);
	if (!Deps.Contains(Dependency))
	{
		Deps.Add(Dependency);
	}
}

void UMingGameInitializer::ReportPhaseProgress(EGameInitPhase Phase, float Progress)
{
	FInitPhaseStatus& Status = PhaseStatuses.FindOrAdd(Phase);
	Status.Progress = FMath::Clamp(Progress, 0.0f, 1.0f);
	
	if (Status.bStarted && !Status.bCompleted)
	{
		UpdateOverallProgress();
	}
}

void UMingGameInitializer::ReportPhaseComplete(EGameInitPhase Phase)
{
	CompleteCurrentPhase();
}

void UMingGameInitializer::ReportPhaseFailed(EGameInitPhase Phase, const FString& ErrorMessage)
{
	FailCurrentPhase(ErrorMessage);
}

FString UMingGameInitializer::GetInitializationReport() const
{
	FString Report;
	Report += TEXT("=== Game Initialization Report ===\n\n");
	
	float TotalDuration = FPlatformTime::Seconds() - InitializationStartTime;
	Report += FString::Printf(TEXT("Total Duration: %.2f seconds\n\n"), TotalDuration);
	
	Report += TEXT("Phase Statuses:\n");
	for (int32 i = 0; i < static_cast<int32>(EGameInitPhase::Error); ++i)
	{
		EGameInitPhase Phase = static_cast<EGameInitPhase>(i);
		const FInitPhaseStatus* Status = PhaseStatuses.Find(Phase);
		const FInitPhaseConfig* Config = PhaseConfigs.Find(Phase);
		
		if (Config && Config->bEnabled && Status)
		{
			Report += FString::Printf(TEXT("  [%s] %s - Progress: %.0f%%"),
				Status->bCompleted ? TEXT("✓") : (Status->bFailed ? TEXT("✗") : (Status->bStarted ? TEXT("►") : TEXT("○"))),
				*UEnum::GetValueAsString(Phase),
				Status->Progress * 100.0f);
			
			if (Status->bCompleted)
			{
				float Duration = Status->EndTime - Status->StartTime;
				Report += FString::Printf(TEXT(" (%.2fs)"), Duration);
			}
			
			if (!Status->ErrorMessage.IsEmpty())
			{
				Report += FString::Printf(TEXT(" - Error: %s"), *Status->ErrorMessage);
			}
			
			Report += TEXT("\n");
		}
	}
	
	Report += FString::Printf(TEXT("\nOverall Progress: %.0f%%\n"), GetOverallProgress() * 100.0f);
	
	if (IsInitializationComplete())
	{
		Report += TEXT("\nStatus: ✓ Initialization Complete\n");
	}
	else if (!bIsInitializing)
	{
		Report += TEXT("\nStatus: ✗ Initialization Failed or Not Started\n");
	}
	else
	{
		Report += TEXT("\nStatus: ► Initialization In Progress\n");
	}
	
	return Report;
}

void UMingGameInitializer::ExecuteCurrentPhase()
{
	FInitPhaseStatus& Status = PhaseStatuses.FindOrAdd(CurrentPhase);
	Status.bStarted = true;
	Status.StartTime = FPlatformTime::Seconds();
	Status.Progress = 0.0f;

	const FInitPhaseConfig* Config = PhaseConfigs.Find(CurrentPhase);
	float Timeout = Config ? Config->TimeoutSeconds : 30.0f;

	// Set timeout timer
	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().SetTimer(InitTimerHandle, this, &UMingGameInitializer::OnInitTimeout, Timeout, false);
	}

	OnInitPhaseStarted.Broadcast(CurrentPhase);

	UE_LOG(LogTemp, Log, TEXT("Executing initialization phase: %s"), *UEnum::GetValueAsString(CurrentPhase));

	// Execute phase-specific logic
	switch (CurrentPhase)
	{
	case EGameInitPhase::Engine:
		InitializeEngine();
		break;
	case EGameInitPhase::CoreSystems:
		InitializeCoreSystems();
		break;
	case EGameInitPhase::Database:
		InitializeDatabase();
		break;
	case EGameInitPhase::Network:
		InitializeNetwork();
		break;
	case EGameInitPhase::Audio:
		InitializeAudio();
		break;
	case EGameInitPhase::UI:
		InitializeUI();
		break;
	case EGameInitPhase::AI:
		InitializeAI();
		break;
	case EGameInitPhase::Localization:
		InitializeLocalization();
		break;
	case EGameInitPhase::SaveLoad:
		InitializeSaveLoad();
		break;
	case EGameInitPhase::GameMode:
		InitializeGameMode();
		break;
	case EGameInitPhase::Content:
		InitializeContent();
		break;
	case EGameInitPhase::PlayerProfile:
		InitializePlayerProfile();
		break;
	case EGameInitPhase::Ready:
		CompleteCurrentPhase();
		break;
	default:
		CompleteCurrentPhase();
		break;
	}
}

void UMingGameInitializer::CompleteCurrentPhase()
{
	FInitPhaseStatus& Status = PhaseStatuses.FindOrAdd(CurrentPhase);
	Status.bCompleted = true;
	Status.EndTime = FPlatformTime::Seconds();
	Status.Progress = 1.0f;

	float Duration = Status.EndTime - Status.StartTime;

	OnInitPhaseCompleted.Broadcast(CurrentPhase, Duration);

	UE_LOG(LogTemp, Log, TEXT("Phase completed: %s (%.2f seconds)"), 
		*UEnum::GetValueAsString(CurrentPhase), Duration);

	if (CurrentPhase == EGameInitPhase::Ready)
	{
		bIsInitializing = false;
		OnGameFullyInitialized.Broadcast();
		
		float TotalDuration = FPlatformTime::Seconds() - InitializationStartTime;
		UE_LOG(LogTemp, Log, TEXT("=== Game Initialization Complete === (%.2f seconds total)", TotalDuration));
	}
	else
	{
		AdvanceToNextPhase();
	}
}

void UMingGameInitializer::FailCurrentPhase(const FString& ErrorMessage)
{
	FInitPhaseStatus& Status = PhaseStatuses.FindOrAdd(CurrentPhase);
	Status.bFailed = true;
	Status.EndTime = FPlatformTime::Seconds();
	Status.ErrorMessage = ErrorMessage;

	bIsInitializing = false;

	OnInitPhaseFailed.Broadcast(CurrentPhase, ErrorMessage);

	UE_LOG(LogTemp, Error, TEXT("Phase failed: %s - %s"), 
		*UEnum::GetValueAsString(CurrentPhase), *ErrorMessage);
}

void UMingGameInitializer::AdvanceToNextPhase()
{
	EGameInitPhase NextPhase = GetNextPhase();
	
	if (NextPhase == EGameInitPhase::Error)
	{
		// No more phases
		CurrentPhase = EGameInitPhase::Ready;
		ExecuteCurrentPhase();
		return;
	}

	CurrentPhase = NextPhase;

	// Check if phase is enabled and dependencies are met
	if (!CanStartPhase(CurrentPhase))
	{
		// Skip to next phase if dependencies not met
		AdvanceToNextPhase();
		return;
	}

	ExecuteCurrentPhase();
}

bool UMingGameInitializer::CanStartPhase(EGameInitPhase Phase) const
{
	const FInitPhaseConfig* Config = PhaseConfigs.Find(Phase);
	if (!Config || !Config->bEnabled)
	{
		return false;
	}

	// Check dependencies
	const TArray<EGameInitPhase>* Deps = PhaseDependencies.Find(Phase);
	if (Deps)
	{
		for (EGameInitPhase Dep : *Deps)
		{
			const FInitPhaseStatus* DepStatus = PhaseStatuses.Find(Dep);
			if (!DepStatus || !DepStatus->bCompleted)
			{
				return false;
			}
		}
	}

	return true;
}

EGameInitPhase UMingGameInitializer::GetNextPhase() const
{
	int32 CurrentIndex = static_cast<int32>(CurrentPhase);
	int32 NextIndex = CurrentIndex + 1;
	
	if (NextIndex < static_cast<int32>(EGameInitPhase::Error))
	{
		return static_cast<EGameInitPhase>(NextIndex);
	}
	
	return EGameInitPhase::Error;
}

void UMingGameInitializer::UpdateOverallProgress()
{
	float OverallProgress = GetOverallProgress();
	OnInitProgressUpdated.Broadcast(OverallProgress);
}

void UMingGameInitializer::OnInitTimeout()
{
	FailCurrentPhase(TEXT("Initialization timeout"));
}

void UMingGameInitializer::InitializeEngine()
{
	ReportPhaseProgress(EGameInitPhase::Engine, 0.5f);
	
	// Engine is already initialized by this point
	// Just report progress and complete
	
	ReportPhaseProgress(EGameInitPhase::Engine, 1.0f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeCoreSystems()
{
	ReportPhaseProgress(EGameInitPhase::CoreSystems, 0.3f);
	
	// Initialize core subsystems
	// This is handled by the game instance
	
	ReportPhaseProgress(EGameInitPhase::CoreSystems, 0.7f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeDatabase()
{
	ReportPhaseProgress(EGameInitPhase::Database, 0.5f);
	
	// Database initialization will be handled by the database manager
	
	ReportPhaseProgress(EGameInitPhase::Database, 1.0f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeNetwork()
{
	ReportPhaseProgress(EGameInitPhase::Network, 0.5f);
	
	// Network manager will initialize itself
	
	ReportPhaseProgress(EGameInitPhase::Network, 1.0f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeAudio()
{
	ReportPhaseProgress(EGameInitPhase::Audio, 0.5f);
	
	// Audio system initialization
	
	ReportPhaseProgress(EGameInitPhase::Audio, 1.0f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeUI()
{
	ReportPhaseProgress(EGameInitPhase::UI, 0.3f);
	
	// UI system initialization
	
	ReportPhaseProgress(EGameInitPhase::UI, 0.7f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeAI()
{
	ReportPhaseProgress(EGameInitPhase::AI, 0.4f);
	
	// AI system initialization
	
	ReportPhaseProgress(EGameInitPhase::AI, 0.8f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeLocalization()
{
	ReportPhaseProgress(EGameInitPhase::Localization, 0.3f);
	
	// Load localization data
	
	ReportPhaseProgress(EGameInitPhase::Localization, 0.7f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeSaveLoad()
{
	ReportPhaseProgress(EGameInitPhase::SaveLoad, 0.5f);
	
	// Initialize save/load system
	
	ReportPhaseProgress(EGameInitPhase::SaveLoad, 1.0f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeGameMode()
{
	ReportPhaseProgress(EGameInitPhase::GameMode, 0.4f);
	
	// Game mode specific initialization
	
	ReportPhaseProgress(EGameInitPhase::GameMode, 0.8f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializeContent()
{
	ReportPhaseProgress(EGameInitPhase::Content, 0.2f);
	
	// Load game content
	
	ReportPhaseProgress(EGameInitPhase::Content, 0.6f);
	CompleteCurrentPhase();
}

void UMingGameInitializer::InitializePlayerProfile()
{
	ReportPhaseProgress(EGameInitPhase::PlayerProfile, 0.5f);
	
	// Load or create player profile
	
	ReportPhaseProgress(EGameInitPhase::PlayerProfile, 1.0f);
	CompleteCurrentPhase();
}
