#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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

protected:
    void OnExperienceGained(int32 Amount);
    void OnLevelUp();

private:
    bool bIsInitialized;
    int32 CurrentLevel;
    int32 CurrentExperience;
    int32 ExperienceToNextLevel;
};
