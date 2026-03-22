#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"
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
};
