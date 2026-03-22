#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"
#include "MingAIUIManager.h"
#include "Save/MingSaveGameManager.h"
#include "Save/MingSaveGame.h"
#include "MingPerformanceManager.h"
#include "MingMemoryOptimizer.h"
#include "Network/MingLobbySystem.h"
#include "Localization/MingLocalizationManager.h"
#include "MingUniversityGuideManager.h"
#include "MingPersonalManager.generated.h"

/**
 * 個人層管理器
 * 管理角色成長和敘事系統
 */
UCLASS()
class MINGPERSONAL_API UMingPersonalManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPersonalManager();

    virtual void Initialize();
    virtual void Shutdown();

    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

    // 角色成長接口
    UFUNCTION(BlueprintCallable, Category = "Character")
    void AddExperience(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Character")
    void LevelUp();

    // 敘事事件
    UFUNCTION(BlueprintCallable, Category = "Narrative")
    void TriggerDialogue(const FString& DialogueId);

    // 關係和聲望系統接口
    UFUNCTION(BlueprintCallable, Category = "Relationship")
    void UpdateCharacterRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Relationship")
    void UpdateRegionReputation(const FString& RegionID, float ChangeAmount, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Relationship")
    FRelationshipData GetCharacterRelationship(const FString& CharacterID) const;

    UFUNCTION(BlueprintCallable, Category = "Relationship")
    FReputationData GetRegionReputation(const FString& RegionID) const;

    UFUNCTION(BlueprintCallable, Category = "Relationship")
    TArray<FString> GetAvailableDialogueOptions(const FString& CharacterID) const;

    UFUNCTION(BlueprintCallable, Category = "Relationship")
    float GetTradePriceModifier(const FString& CharacterID) const;

    UFUNCTION(BlueprintCallable, Category = "Relationship")
    bool CanAcceptQuestByReputation(const FString& QuestID, const FString& RegionID) const;

    // 事件處理
    UFUNCTION(BlueprintCallable, Category = "Relationship")
    void OnQuestCompleted(const FString& QuestID, const FString& RegionID, const FString& QuestGiverID);

    UFUNCTION(BlueprintCallable, Category = "Relationship")
    void OnNPCInteraction(const FString& CharacterID, const FString& InteractionType);

    // 音頻系統接口
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void InitializeAudioSystem(UObject* MetaSoundsSystem);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayRelationshipAudio(const FString& CharacterID, const FString& AudioType);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayReputationAudio(const FString& RegionID, const FString& AudioType);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayDialogueAudio(const FString& CharacterID, const FString& DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayRepublicEraTheme(const FString& ThemeName, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetAudioVolume(float RelationshipVolume, float ReputationVolume, float DialogueVolume);

    // AI UI系統接口
    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void InitializeAIUISystem();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void TrackUserBehavior(const FString& BehaviorType, const FString& Context, float Value = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void OptimizeUIForUser();

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    void ShowContextualHelp(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "AI UI")
    TArray<FString> GetAIRecommendations();

    // 保存和載入系統接口
    UFUNCTION(BlueprintCallable, Category = "Save System")
    void InitializeSaveSystem();

    UFUNCTION(BlueprintCallable, Category = "Save System")
    EMingSaveGameResult SaveGame(int32 SlotIndex, const FString& SaveName);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    EMingSaveGameResult QuickSave();

    UFUNCTION(BlueprintCallable, Category = "Save System")
    EMingSaveGameResult LoadGame(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    EMingSaveGameResult QuickLoad();

    UFUNCTION(BlueprintCallable, Category = "Save System")
    bool DeleteSaveGame(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    void SetAutoSaveEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "Save System")
    bool IsAutoSaveEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "Save System")
    UMingSaveGameManager* GetSaveGameManager() const;

    // 多人遊戲系統接口
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void InitializeMultiplayerSystem();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    bool CreateMultiplayerServer(const FMingNetworkConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    bool ConnectToMultiplayerServer(const FString& ServerAddress, int32 Port);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void DisconnectFromMultiplayerServer();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    bool IsMultiplayerConnected() const;

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    bool IsMultiplayerHost() const;

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    int32 GetLocalPlayerID() const;

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void SetPlayerReady(bool bReady);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void StartMultiplayerGame();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void SendMultiplayerChatMessage(const FString& Message);

    UFUNCTION(BlueprintPure, Category = "Multiplayer")
    UMingNetworkManager* GetNetworkManager() const;

    UFUNCTION(BlueprintPure, Category = "Multiplayer")
    UMingLobbySystem* GetLobbySystem() const;

    // 本地化系統接口
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void InitializeLocalizationSystem();

    UFUNCTION(BlueprintCallable, Category = "Localization")
    bool SetGameLanguage(EMingLanguage NewLanguage);

    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SetGameRegion(EMingRegion NewRegion);

    UFUNCTION(BlueprintPure, Category = "Localization")
    EMingLanguage GetCurrentGameLanguage() const;

    UFUNCTION(BlueprintPure, Category = "Localization")
    EMingRegion GetCurrentGameRegion() const;

    UFUNCTION(BlueprintPure, Category = "Localization")
    FString GetLocalizedString(const FString& Key, const FString& Namespace = TEXT("Default")) const;

    UFUNCTION(BlueprintCallable, Category = "Localization")
    bool DownloadLanguagePack(EMingLanguage Language);

    UFUNCTION(BlueprintPure, Category = "Localization")
    TArray<FLanguagePackInfo> GetAvailableLanguagePacks() const;

    UFUNCTION(BlueprintPure, Category = "Localization")
    UMingLocalizationManager* GetLocalizationManager() const;

    // 性能優化系統接口
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void InitializePerformanceSystem();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetPerformanceLevel(EMingPerformanceLevel NewLevel);

    UFUNCTION(BlueprintPure, Category = "Performance")
    EMingPerformanceLevel GetCurrentPerformanceLevel() const;

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void OptimizePerformance();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTargetFrameRate(float TargetFPS);

    UFUNCTION(BlueprintPure, Category = "Performance")
    FMingPerformanceMetrics GetPerformanceMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartPerformanceMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopPerformanceMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void OptimizeMemory();

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void SetMemoryBudget(int32 MaxMemoryMB);

    UFUNCTION(BlueprintPure, Category = "Memory")
    float GetMemoryUsagePercent() const;

    UFUNCTION(BlueprintPure, Category = "Performance")
    UMingPerformanceManager* GetPerformanceManager() const;

    UFUNCTION(BlueprintPure, Category = "Performance")
    UMingMemoryOptimizer* GetMemoryOptimizer() const;

    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StartUniversityGuide();

    UFUNCTION(BlueprintCallable, Category = "University Guide")
    void StopUniversityGuide();

    UFUNCTION(BlueprintCallable, Category = "University Guide")
    UMingUniversityGuideManager* GetUniversityGuideManager() const;

protected:
    void OnExperienceGained(int32 Amount);
    void OnLevelUp();

private:
    bool bIsInitialized;
    int32 CurrentLevel;
    int32 CurrentExperience;
    int32 ExperienceToNextLevel;

    // 關係和聲望管理器
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // 音頻關係管理器
    UPROPERTY()
    TObjectPtr<UMingAudioRelationshipManager> AudioRelationshipManager;

    // AI UI管理器
    UPROPERTY()
    TObjectPtr<UMingAIUIManager> AIUIManager;

    // 保存遊戲管理器
    UPROPERTY()
    TObjectPtr<UMingSaveGameManager> SaveGameManager;

    // 性能優化管理器
    UPROPERTY()
    TObjectPtr<UMingPerformanceManager> PerformanceManager;

    // 內存優化器
    UPROPERTY()
    TObjectPtr<UMingMemoryOptimizer> MemoryOptimizer;

    // 大廳系統
    UPROPERTY()
    TObjectPtr<UMingLobbySystem> LobbySystem;

    // 本地化系統
    UPROPERTY()
    TObjectPtr<UMingLocalizationManager> LocalizationManager;

    // 高校引導管理器
    UPROPERTY()
    TObjectPtr<UMingUniversityGuideManager> UniversityGuideManager;
};
