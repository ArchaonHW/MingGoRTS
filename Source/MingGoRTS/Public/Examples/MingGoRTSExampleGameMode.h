#pragma once

// MingGoRTS 系統集g示例
// rw件展示如何在 GameMode 中集g所有核心系統


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingGoRTSExampleGameMode.generated.h"

// 前向聲明所有系統
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
 * 示例 GameMode - 展示如何集g所有 MingGoRTS 系統
 */
UCLASS()
class MINGRTS_API AMingGoRTSExampleGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingGoRTSExampleGameMode(};

    // GameMode 生命週期
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) overHide;
    virtual void StartPlay() overHide;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) overHide;
    virtual void Tick(float DeltaTime) overHide;

    // 系統訪問器
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
    // 核心系統
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

    // 系統初始化
    void InitializeAllSystems(};
    void ShutdownAllSystems(};

    // 事件綁定
    void BindSystemEvents(};

    // 示例g能
    void SetupTutorial(};
    void SetupReplayRecording(};
    void SetupTelemetry(};
    void SetupPerformanceMonitoring(};

    // 事件U理函數
    UFUNCTION()
    void OnTutorialStepCompleted(const FString& StepID};

    UFUNCTION()
    void OnAchievementUnlocked(const FString& AchievementID, const FAchievementData& Data};

    UFUNCTION()
    void OnPlayerRegistered(const FString& TournamentID, const FString& PlayerID};

    UFUNCTION()
    void OnPerformanceLevelChanged(EPerformanceLevel NewLevel};

    // 配m
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
