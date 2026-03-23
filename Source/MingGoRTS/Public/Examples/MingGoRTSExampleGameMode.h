#pragma once

// MingGoRTS Á≥ªÁµ±?ÜgÁ§∫‰?
// rw‰ª∂Â?Á§∫Â?‰ΩïÂú® GameMode ‰∏≠È?g?Ä?âÊ†∏ÂøÉÁ≥ªÁµ?


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingGoRTSExampleGameMode.generated.h"

// ?çÂ??≤Ê??Ä?âÁ≥ªÁµ?
class UMingRTSTutorialSystem;
class UMingRTSSecurityManager;
class UMingRTSReplaySystem;
class UMingRTSReplayManager;
class UMingRTSAchievementSystem;
class UMingRTSStatisticsSystem;
class UMingRTSSpectatorSystem;
class UMingRTSTelemetrySystem;
class UMingRTSModSystem;
class UMingRTSTournamentSystem;
class UMingRTSCloudSystem;
class UMingRTSVRSupportSystem;
class UMingRTSMultiplayerSupportSystem;
class UMingRTSPerformanceSystem;

/**
 * Á§∫‰? GameMode - Â±ïÁ§∫Â¶Ç‰??Üg?Ä??MingGoRTS Á≥ªÁµ±
 */
UCLASS()
class MINGRTS_API AMingGoRTSExampleGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingGoRTSExampleGameMode(};

    // GameMode ?üÂëΩ?±Ê?
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void StartPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaTime) override;

    // Á≥ªÁµ±Ë®™Â???
    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSTutorialSystem* GetTutorialSystem() const { return TutorialSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSSecurityManager* GetSecuritySystem() const { return SecuritySystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSReplaySystem* GetReplaySystem() const { return ReplaySystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSAchievementSystem* GetAchievementSystem() const { return AchievementSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSStatisticsSystem* GetStatisticsSystem() const { return StatisticsSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSSpectatorSystem* GetSpectatorSystem() const { return SpectatorSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSTelemetrySystem* GetTelemetrySystem() const { return TelemetrySystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSModSystem* GetModSystem() const { return ModSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSTournamentSystem* GetTournamentSystem() const { return TournamentSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSCloudSystem* GetCloudSystem() const { return CloudSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSVRSupportSystem* GetVRSupportSystem() const { return VRSupportSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSMultiplayerSupportSystem* GetMultiplayerSystem() const { return MultiplayerSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTSSystems")
    UMingRTSPerformanceSystem* GetPerformanceSystem() const { return PerformanceSystem; }

protected:
    // ?∏Â?Á≥ªÁµ±
    UPROPERTY()
    TObjectPtr<UMingRTSTutorialSystem> TutorialSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSSecurityManager> SecuritySystem;

    UPROPERTY()
    TObjectPtr<UMingRTSReplaySystem> ReplaySystem;

    UPROPERTY()
    TObjectPtr<UMingRTSReplayManager> ReplayManager;

    UPROPERTY()
    TObjectPtr<UMingRTSAchievementSystem> AchievementSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSStatisticsSystem> StatisticsSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSSpectatorSystem> SpectatorSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSTelemetrySystem> TelemetrySystem;

    UPROPERTY()
    TObjectPtr<UMingRTSModSystem> ModSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSTournamentSystem> TournamentSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSCloudSystem> CloudSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSVRSupportSystem> VRSupportSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSMultiplayerSupportSystem> MultiplayerSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSPerformanceSystem> PerformanceSystem;

    // Á≥ªÁµ±?ùÂ???
    void InitializeAllSystems(};
    void ShutdownAllSystems(};

    // ‰∫ã‰ª∂Á∂ÅÂ?
    void BindSystemEvents(};

    // Á§∫‰?g??
    void SetupTutorial(};
    void SetupReplayRecording(};
    void SetupTelemetry(};
    void SetupPerformanceMonitoring(};

    // ‰∫ã‰ª∂U?ÜÂáΩ??
    UFUNCTION()
    void OnTutorialStepCompleted(const FString& StepID};

    UFUNCTION()
    void OnAchievementUnlocked(const FString& AchievementID, const FAchievementData& Data};

    UFUNCTION()
    void OnPlayerRegistered(const FString& TournamentID, const FString& PlayerID};

    UFUNCTION()
    void OnPerformanceLevelChanged(EPerformanceLevel NewLevel};

    // ?çm
    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTSConfig")
    bool bEnableTutorialSystem = true;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTSConfig")
    bool bEnableSecuritySystem = true;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTSConfig")
    bool bEnableReplaySystem = true;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTSConfig")
    bool bEnableTelemetrySystem = true;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTSConfig")
    bool bEnablePerformanceSystem = true;
};

