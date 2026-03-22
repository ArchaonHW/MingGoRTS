// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SageCommand/MingSageCharacterSystem.h"
#include "SageCommand/MingYinYangFiveElements.h"
#include "MingSageCharacter.generated.h"

/**
 * 至聖者角色類別
 * 繼承自 Character，整合聖者系統和陰陽五行系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API AMingSageCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMingSageCharacter();

    // 角色初始化
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 聖者系統組件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SageCharacter")
    TObjectPtr<UMingSageCharacterSystem> SageCharacterSystem;

    // 陰陽五行系統組件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SageCharacter")
    TObjectPtr<UMingYinYangFiveElements> YinYangSystem;

    // 角色數據
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    FSageCharacterData CharacterData;

    // 角色等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 CharacterLevel = 1;

    // 經驗值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 ExperiencePoints = 0;

    // 靈力值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float SpiritualPower = 100.0f;

    // 最大靈力值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float MaxSpiritualPower = 100.0f;

    // 智慧值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 WisdomPoints = 0;

    // 聖者光環範圍
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float SageAuraRange = 500.0f;

    // 是否顯示聖者光環
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    bool bShowSageAura = true;

    // 角色技能列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    TArray<FString> AvailableSkills;

    // 當前激活的技能
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    TArray<FString> ActiveSkills;

    // 角色狀態效果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    TArray<FString> StatusEffects;

    // 角色成就
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    TArray<FString> Achievements;

    // 角色關係網絡
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    TMap<FString, float> RelationshipNetwork;

    // 角色聲望
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    TMap<FString, int32> ReputationMap;

    // 角色歷史記錄
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    TArray<FString> HistoryRecords;

    // 角色偏好設置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    TMap<FString, float> PreferenceSettings;

    // 角色統計數據
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    TMap<FString, int32> StatisticsData;

    // 初始化角色
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool InitializeCharacter(ESageCharacterType CharacterType, const FString& CharacterName);

    // 升級角色
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool LevelUpCharacter();

    // 獲得經驗值
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void GainExperience(int32 Experience);

    // 使用技能
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool UseSkill(const FString& SkillName);

    // 添加狀態效果
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void AddStatusEffect(const FString& EffectName, float Duration);

    // 移除狀態效果
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void RemoveStatusEffect(const FString& EffectName);

    // 更新關係
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void UpdateRelationship(const FString& TargetName, float Change);

    // 更新聲望
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void UpdateReputation(const FString& Faction, int32 Change);

    // 添加歷史記錄
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void AddHistoryRecord(const FString& Record);

    // 獲得成就
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void UnlockAchievement(const FString& AchievementName);

    // 設置偏好
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void SetPreference(const FString& PreferenceName, float Value);

    // 更新統計
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void UpdateStatistic(const FString& StatName, int32 Value);

    // 獲取角色能力評分
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    float GetCharacterPowerRating() const;

    // 獲取角色狀態描述
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    FString GetCharacterStatusDescription() const;

    // 獲取角色建議
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    TArray<FString> GetCharacterRecommendations() const;

    // 檢查角色是否可以執行特定行動
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool CanPerformAction(const FString& ActionName) const;

    // 執行聖者特殊能力
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool ExecuteSageAbility(const FString& AbilityName);

    // 進行冥想
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool Meditate(float Duration);

    // 進行贖罪
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool PerformAtonement();

    // 平衡陰陽
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool BalanceYinYang();

    // 和諧五行
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool HarmonizeElements();

    // 獲取角色詳細信息
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    FString GetCharacterDetailedInfo() const;

    // 保存角色數據
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool SaveCharacterData();

    // 載入角色數據
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    bool LoadCharacterData();

    // 重置角色
    UFUNCTION(BlueprintCallable, Category = "SageCharacter")
    void ResetCharacter();

    // 事件：角色升級
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLeveledUp, int32, NewLevel);
    UPROPERTY(BlueprintAssignable, Category = "SageCharacter")
    FOnCharacterLeveledUp OnCharacterLeveledUp;

    // 事件：經驗值變化
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceChanged, int32, NewExperience, int32, TotalExperience);
    UPROPERTY(BlueprintAssignable, Category = "SageCharacter")
    FOnExperienceChanged OnExperienceChanged;

    // 事件：靈力值變化
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpiritualPowerChanged, float, NewPower, float, MaxPower);
    UPROPERTY(BlueprintAssignable, Category = "SageCharacter")
    FOnSpiritualPowerChanged OnSpiritualPowerChanged;

    // 事件：技能使用
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillUsed, const FString&, SkillName);
    UPROPERTY(BlueprintAssignable, Category = "SageCharacter")
    FOnSkillUsed OnSkillUsed;

    // 事件：狀態效果變化
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectChanged, const FString&, EffectName, bool, bAdded);
    UPROPERTY(BlueprintAssignable, Category = "SageCharacter")
    FOnStatusEffectChanged OnStatusEffectChanged;

    // 事件：成就解鎖
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, const FString&, AchievementName);
    UPROPERTY(BlueprintAssignable, Category = "SageCharacter")
    FOnAchievementUnlocked OnAchievementUnlocked;

protected:
    // 初始化系統組件
    void InitializeSystems();

    // 更新角色狀態
    void UpdateCharacterState(float DeltaTime);

    // 處理狀態效果
    void ProcessStatusEffects(float DeltaTime);

    // 更新靈力值
    void UpdateSpiritualPower(float DeltaTime);

    // 檢查角色條件
    void CheckCharacterConditions();

    // 應用等級效果
    void ApplyLevelEffects();

    // 應用技能效果
    void ApplySkillEffects(const FString& SkillName);

    // 生成角色建議
    TArray<FString> GenerateRecommendations() const;

    // 計算角色能力
    float CalculateCharacterPower() const;

    // 獲取當前時間
    float GetCurrentGameTime() const;

    // 檢查周圍環境
    void CheckSurroundingEnvironment();

    // 更新聖者光環
    void UpdateSageAura();

    // 處理環境互動
    void HandleEnvironmentInteraction();

private:
    // 是否已初始化
    bool bIsInitialized = false;

    // 上次更新時間
    float LastUpdateTime = 0.0f;

    // 靈力恢復速率
    float SpiritualPowerRegenRate = 1.0f;

    // 經驗值倍率
    float ExperienceMultiplier = 1.0f;

    // 能力倍率
    float PowerMultiplier = 1.0f;

    // 狀態效果持續時間映射
    TMap<FString, float> StatusEffectDurations;

    // 技能冷卻時間映射
    TMap<FString, float> SkillCooldowns;

    // 當前目標
    FString CurrentObjective;

    // 角色心情
    float CharacterMood = 50.0f;

    // 角色健康狀態
    float CharacterHealth = 100.0f;

    // 角色精神狀態
    float CharacterMentalState = 100.0f;

    // 角色疲勞度
    float CharacterFatigue = 0.0f;

    // 角色專注度
    float CharacterFocus = 100.0f;
};
