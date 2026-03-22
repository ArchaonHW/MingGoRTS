#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingGoRTSGameMode.generated.h"

// Forward declarations
class UMingRTSFactionManager;
class UMingRTSDynamicHistorySystem;
class UMingRTSSelfLearningSystem;
class UMingRTSAILearningIntegration;
class UMingAutoSceneGenerator;
class UMingRTSGameAssetGenerator;
class UMingRTSLocalizationEnhancedSystem;
class UMingRTSPerformanceEnhancedSystem;
class UMingPersonalManager;
class UMingRTSUIEnhancedSystem;
class UMingRTSAudioEnhancedSystem;
class UMingSaveGameManager;
class UMingMainGameUI;

/**
 * 主遊戲模式類 - 集成所有MingGoRTS系統
 * 負責系統初始化、協調和生命週期管理
 */
UCLASS()
class MINGRTS_API AMingGoRTSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingGoRTSGameMode();

    // AActor overrides
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "System Integration")
    bool InitializeAllSystems();

    // 系統獲取接口
    UFUNCTION(BlueprintCallable, Category = "System Integration")
    UMingRTSFactionManager* GetFactionManager() const { return FactionManager; }

    UFUNCTION(BlueprintCallable, Category = "System Integration")
    UMingRTSDynamicHistorySystem* GetHistorySystem() const { return HistorySystem; }

    UFUNCTION(BlueprintCallable, Category = "System Integration")
    UMingRTSSelfLearningSystem* GetLearningSystem() const { return LearningSystem; }

    UFUNCTION(BlueprintCallable, Category = "System Integration")
    UMingAutoSceneGenerator* GetSceneGenerator() const { return SceneGenerator; }

    UFUNCTION(BlueprintCallable, Category = "System Integration")
    UMingRTSGameAssetGenerator* GetAssetGenerator() const { return AssetGenerator; }

    UFUNCTION(BlueprintCallable, Category = "System Integration")
    UMingRTSLocalizationEnhancedSystem* GetLocalizationSystem() const { return LocalizationSystem; }

    UFUNCTION(BlueprintCallable, Category = "System Integration")
    UMingPersonalManager* GetPersonalManager() const { return PersonalManager; }

    UFUNCTION(BlueprintCallable, Category = "System Integration")
    UMingMainGameUI* GetMainUI() const { return MainGameUI; }

    // 系統狀態檢查
    UFUNCTION(BlueprintCallable, Category = "System Integration")
    bool AreAllSystemsInitialized() const;

    // 系統性能報告
    UFUNCTION(BlueprintCallable, Category = "System Integration")
    FString GetSystemPerformanceReport() const;

    // 原有遊戲功能
    UFUNCTION(BlueprintCallable, Category = "Game")
    void SpawnInitialUnits();

    UFUNCTION(BlueprintCallable, Category = "Game")
    class AMingGoRTSCamera* GetRTSCamera() const { return RTSCamera; }

protected:
    // 核心系統組件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingRTSFactionManager> FactionManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingRTSDynamicHistorySystem> HistorySystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingRTSSelfLearningSystem> LearningSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingRTSAILearningIntegration> AIIntegration;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingAutoSceneGenerator> SceneGenerator;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingRTSGameAssetGenerator> AssetGenerator;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingRTSLocalizationEnhancedSystem> LocalizationSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingRTSPerformanceEnhancedSystem> PerformanceSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingPersonalManager> PersonalManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingRTSUIEnhancedSystem> UISystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingRTSAudioEnhancedSystem> AudioSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Systems")
    TObjectPtr<UMingSaveGameManager> SaveGameManager;

    // UI組件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    TObjectPtr<UMingMainGameUI> MainGameUI;

    // 原有遊戲組件
    UPROPERTY()
    class AMingGoRTSCamera* RTSCamera;

    UPROPERTY()
    TArray<class AMingGoRTSUnit*> GameUnits;

    // 系統狀態
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System State")
    bool bSystemsInitialized = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System State")
    TArray<FString> InitializationErrors;

private:
    // 初始化輔助函數
    bool InitializeCoreSystems();
    bool InitializeAISystems();
    bool InitializeContentSystems();
    bool InitializeUserInterfaceSystems();
    bool SetupSystemCommunication();
    bool SetupEventBindings();

    // 系統清理
    void CleanupAllSystems();

    // 性能監控
    void UpdatePerformanceMetrics(float DeltaTime);

    // 系統通信
    void SetupSystemCommunicationChannels();
    void BroadcastSystemEvent(const FString& EventName, UObject* EventData);

    // 錯誤處理
    void LogSystemError(const FString& SystemName, const FString& ErrorMessage);
    void HandleSystemInitializationFailure(const FString& SystemName);

    // 配置
    UPROPERTY(Config, EditAnywhere, Category = "System Configuration")
    bool bEnablePerformanceMonitoring = true;

    UPROPERTY(Config, EditAnywhere, Category = "System Configuration")
    bool bEnableDetailedLogging = true;

    UPROPERTY(Config, EditAnywhere, Category = "System Configuration")
    float PerformanceUpdateInterval = 1.0f;

    // 性能追蹤
    float LastPerformanceUpdate = 0.0f;
    TMap<FString, float> SystemPerformanceMetrics;
};
