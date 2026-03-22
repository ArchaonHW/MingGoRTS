// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingGameInitializer.generated.h"

UENUM(BlueprintType)
enum class EGameInitPhase: uint8 {
	PreInit					UMETA(DisplayName = "預初始化"),
	Engine					UMETA(DisplayName = "引擎初始化"),
	CoreSystems				UMETA(DisplayName = "核心系統"),
	Database				UMETA(DisplayName = "資料庫系統"),
	Network					UMETA(DisplayName = "網絡系統"),
	Audio					UMETA(DisplayName = "音頻系統"),
	UI						UMETA(DisplayName = "UI系統"),
	AI						UMETA(DisplayName = "AI系統"),
	Localization				UMETA(DisplayName = "本地化系統"),
	SaveLoad				UMETA(DisplayName = "存檔系統"),
	GameMode				UMETA(DisplayName = "遊戲模式"),
	Content					UMETA(DisplayName = "內容加載"),
	PlayerProfile				UMETA(DisplayName = "玩家配置"),
	Ready					UMETA(DisplayName = "就緒"),
	Error					UMETA(DisplayName = "錯誤")
};

USTRUCT(BlueprintType)
struct FInitPhaseConfig
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "GameInit")
	EGameInitPhase Phase;

	UPROPERTY(BlueprintReadWrite, Category = "GameInit")
	bool bEnabled;

	UPROPERTY(BlueprintReadWrite, Category = "GameInit")
	float TimeoutSeconds;

	UPROPERTY(BlueprintReadWrite, Category = "GameInit")
	TArray<FString> Dependencies;

	FInitPhaseConfig()
		: Phase(EGameInitPhase::PreInit)
		, bEnabled(true)
		, TimeoutSeconds(30.0f)
	{
	}
};

USTRUCT(BlueprintType)
struct FInitPhaseStatus
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "GameInit")
	EGameInitPhase Phase;

	UPROPERTY(BlueprintReadOnly, Category = "GameInit")
	bool bStarted;

	UPROPERTY(BlueprintReadOnly, Category = "GameInit")
	bool bCompleted;

	UPROPERTY(BlueprintReadOnly, Category = "GameInit")
	bool bFailed;

	UPROPERTY(BlueprintReadOnly, Category = "GameInit")
	float Progress;

	UPROPERTY(BlueprintReadOnly, Category = "GameInit")
	float StartTime;

	UPROPERTY(BlueprintReadOnly, Category = "GameInit")
	float EndTime;

	UPROPERTY(BlueprintReadOnly, Category = "GameInit")
	FString ErrorMessage;

	FInitPhaseStatus()
		: Phase(EGameInitPhase::PreInit)
		, bStarted(false)
		, bCompleted(false)
		, bFailed(false)
		, Progress(0.0f)
		, StartTime(0.0f)
		, EndTime(0.0f)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitPhaseStarted, EGameInitPhase, Phase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInitPhaseCompleted, EGameInitPhase, Phase, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInitPhaseFailed, EGameInitPhase, Phase, FString, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitProgressUpdated, float, OverallProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameFullyInitialized);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGCORE_API UMingGameInitializer : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMingGameInitializer();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	void StartGameInitialization();

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	void SkipToPhase(EGameInitPhase TargetPhase);

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	void RestartInitialization();

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	EGameInitPhase GetCurrentPhase() const;

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	float GetOverallProgress() const;

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	bool IsInitializationComplete() const;

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	bool IsPhaseComplete(EGameInitPhase Phase) const;

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	TArray<FInitPhaseStatus> GetAllPhaseStatuses() const;

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	void ConfigurePhase(const FInitPhaseConfig& Config);

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	void SetPhaseEnabled(EGameInitPhase Phase, bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	void RegisterPhaseDependency(EGameInitPhase Phase, EGameInitPhase Dependency);

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	void ReportPhaseProgress(EGameInitPhase Phase, float Progress);

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	void ReportPhaseComplete(EGameInitPhase Phase);

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	void ReportPhaseFailed(EGameInitPhase Phase, const FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "GameInit")
	FString GetInitializationReport() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "GameInit|Events")
	FOnInitPhaseStarted OnInitPhaseStarted;

	UPROPERTY(BlueprintAssignable, Category = "GameInit|Events")
	FOnInitPhaseCompleted OnInitPhaseCompleted;

	UPROPERTY(BlueprintAssignable, Category = "GameInit|Events")
	FOnInitPhaseFailed OnInitPhaseFailed;

	UPROPERTY(BlueprintAssignable, Category = "GameInit|Events")
	FOnInitProgressUpdated OnInitProgressUpdated;

	UPROPERTY(BlueprintAssignable, Category = "GameInit|Events")
	FOnGameFullyInitialized OnGameFullyInitialized;

protected:
	UPROPERTY()
	EGameInitPhase CurrentPhase;

	UPROPERTY()
	TMap<EGameInitPhase, FInitPhaseStatus> PhaseStatuses;

	UPROPERTY()
	TMap<EGameInitPhase, FInitPhaseConfig> PhaseConfigs;

	UPROPERTY()
	TMap<EGameInitPhase, TArray<EGameInitPhase>> PhaseDependencies;

	UPROPERTY()
	bool bIsInitializing;

	UPROPERTY()
	float InitializationStartTime;

	UPROPERTY()
	FTimerHandle InitTimerHandle;

protected:
	void ExecuteCurrentPhase();
	void CompleteCurrentPhase();
	void FailCurrentPhase(const FString& ErrorMessage);
	void AdvanceToNextPhase();
	bool CanStartPhase(EGameInitPhase Phase) const;
	EGameInitPhase GetNextPhase() const;
	void UpdateOverallProgress();
	void OnInitTimeout();

	void InitializeEngine();
	void InitializeCoreSystems();
	void InitializeDatabase();
	void InitializeNetwork();
	void InitializeAudio();
	void InitializeUI();
	void InitializeAI();
	void InitializeLocalization();
	void InitializeSaveLoad();
	void InitializeGameMode();
	void InitializeContent();
	void InitializePlayerProfile();
};
