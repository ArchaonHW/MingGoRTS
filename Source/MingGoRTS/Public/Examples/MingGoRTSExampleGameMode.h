// MingGoRTS 系統集成示例
// 本文件展示如何在 GameMode 中集成所有核心系統

#pragma once

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
 * 示例 GameMode - 展示如何集成所有 MingGoRTS 系統
 */
UCLASS()
class MINGGORTS_API AMingGoRTSExampleGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingGoRTSExampleGameMode();

    // GameMode 生命週期
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void StartPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaTime) override;

    // 系統訪問器
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSTutorialSystem* GetTutorialSystem() const { return TutorialSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSSecurityManager* GetSecuritySystem() const { return SecuritySystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSReplaySystem* GetReplaySystem() const { return ReplaySystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSAchievementSystem* GetAchievementSystem() const { return AchievementSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSStatisticsSystem* GetStatisticsSystem() const { return StatisticsSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSSpectatorSystem* GetSpectatorSystem() const { return SpectatorSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSTelemetrySystem* GetTelemetrySystem() const { return TelemetrySystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSModSystem* GetModSystem() const { return ModSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSTournamentSystem* GetTournamentSystem() const { return TournamentSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSCloudSystem* GetCloudSystem() const { return CloudSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSVRSupportSystem* GetVRSupportSystem() const { return VRSupportSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
    UMingRTSMultiplayerSupportSystem* GetMultiplayerSystem() const { return MultiplayerSystem; }

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS|Systems")
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
    void InitializeAllSystems();
    void ShutdownAllSystems();

    // 事件綁定
    void BindSystemEvents();

    // 示例功能
    void SetupTutorial();
    void SetupReplayRecording();
    void SetupTelemetry();
    void SetupPerformanceMonitoring();

    // 事件處理函數
    UFUNCTION()
    void OnTutorialStepCompleted(const FString& StepID);

    UFUNCTION()
    void OnAchievementUnlocked(const FString& AchievementID, const FAchievementData& Data);

    UFUNCTION()
    void OnPlayerRegistered(const FString& TournamentID, const FString& PlayerID);

    UFUNCTION()
    void OnPerformanceLevelChanged(EPerformanceLevel NewLevel);

    // 配置
    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS|Config")
    bool bEnableTutorialSystem = true;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS|Config")
    bool bEnableSecuritySystem = true;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS|Config")
    bool bEnableReplaySystem = true;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS|Config")
    bool bEnableTelemetrySystem = true;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS|Config")
    bool bEnablePerformanceSystem = true;
};
