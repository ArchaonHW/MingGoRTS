#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingPersonalLayer.generated.h"

// 角色職業類型
UENUM(BlueprintType)
enum class EMingCharacterProfession : uint8
{
    Military,       // 軍人
    Politician,     // 政治家
    Scholar,        // 學者
    Merchant,       // 商人
    Farmer,         // 農民
    Worker,         // 工人
    Doctor,         // 醫生
    Teacher,        // 教師
    Journalist,     // 記者
    Artist          // 藝術家
};

// 角色技能類型
UENUM(BlueprintType)
enum class EMingCharacterSkill : uint8
{
    Leadership,      // 領導力
    Combat,         // 戰鬥
    Diplomacy,      // 外交
    Economics,      // 經濟
    Scholarship,    // 學術
    Medicine,       // 醫學
    Engineering,    // 工程
    Art,            // 藝術
    Speech,         // 演講
    Survival        // 生存
};

// 關係狀態
UENUM(BlueprintType)
enum class EMingRelationshipStatus : uint8
{
    Stranger,       // 陌生人
    Acquaintance,   // 認識
    Friend,         // 朋友
    CloseFriend,    // 好朋友
    Family,         // 家人
    Lover,          // 情人
    Rival,          // 對手
    Enemy,          // 敵人
    Mentor,         // 導師
    Student         // 學生
};

// 角色狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingCharacterStatus
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    class AActor* CharacterActor;

    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly)
    FString CharacterName;

    UPROPERTY(BlueprintReadOnly)
    EMingCharacterProfession Profession;

    UPROPERTY(BlueprintReadOnly)
    int32 Level;

    UPROPERTY(BlueprintReadOnly)
    float Experience;

    UPROPERTY(BlueprintReadOnly)
    float Health;

    UPROPERTY(BlueprintReadOnly)
    float Morale;

    UPROPERTY(BlueprintReadOnly)
    float Reputation;

    UPROPERTY(BlueprintReadOnly)
    float Influence;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingCharacterSkill, float> Skills;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, EMingRelationshipStatus> Relationships;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Inventory;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ActiveQuests;
};

// 個人決策
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingPersonalDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionType;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingCharacterSkill> RequiredSkills;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RequiredItems;

    UPROPERTY(BlueprintReadOnly)
    float DecisionImpact;

    UPROPERTY(BlueprintReadOnly)
    float PersonalRisk;
};

// 個人事件
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingPersonalEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventDescription;

    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InvolvedCharacters;

    UPROPERTY(BlueprintReadOnly)
    float PersonalImpact;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EventTime;
};

// 角色成長記錄
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingCharacterGrowth
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingCharacterSkill, float> SkillProgress;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> RelationshipChanges;

    UPROPERTY(BlueprintReadOnly)
    float ReputationChange;

    UPROPERTY(BlueprintReadOnly)
    float InfluenceChange;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> UnlockedAbilities;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CompletedQuests;
};

// 個人狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingPersonalState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    UPROPERTY(BlueprintReadOnly)
    FString CurrentLocation;

    UPROPERTY(BlueprintReadOnly)
    FString CurrentActivity;

    UPROPERTY(BlueprintReadOnly)
    float OverallHappiness;

    UPROPERTY(BlueprintReadOnly)
    float SocialStanding;

    UPROPERTY(BlueprintReadOnly)
    float EconomicStatus;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ActiveRelationships;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CurrentGoals;

    FMingPersonalState()
        : bIsActive(true)
        , OverallHappiness(50.0f)
        , SocialStanding(50.0f)
        , EconomicStatus(50.0f)
    {}
};

/**
 * 個人層系統
 * 負責角色扮演、個人成長、關係管理等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingPersonalLayer : public UObject, public IMingGameLayer
{
    GENERATED_BODY()

public:
    // 建構子
    UMingPersonalLayer();

    // 實現介面方法
    virtual void InitializeLayer_Implementation() override;
    virtual void UpdateLayer_Implementation(float DeltaTime) override;
    virtual EMingLayer GetLayerType_Implementation() const override;
    virtual FString GetLayerName_Implementation() const override;
    virtual float GetLayerPriority_Implementation() const override;
    virtual void HandleLayerEvent_Implementation(const FMingStrategicIntegrationEvent& Event) override;
    virtual void HandleLayerDecision_Implementation(const FMingGameDecision& Decision) override;

    // 角色管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void CreateCharacter(const FString& CharacterID, const FString& CharacterName, EMingCharacterProfession Profession);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void UpdateCharacter(const FString& CharacterID, const FMingCharacterStatus& Status);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    FMingCharacterStatus GetCharacter(const FString& CharacterID) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    TArray<FMingCharacterStatus> GetAllCharacters() const;

    // 技能管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void ImproveSkill(const FString& CharacterID, EMingCharacterSkill Skill, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    float GetSkillLevel(const FString& CharacterID, EMingCharacterSkill Skill) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    TMap<EMingCharacterSkill, float> GetAllSkills(const FString& CharacterID) const;

    // 關係管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void UpdateRelationship(const FString& CharacterID, const FString& TargetID, EMingRelationshipStatus Status);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    EMingRelationshipStatus GetRelationship(const FString& CharacterID, const FString& TargetID) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    TMap<FString, EMingRelationshipStatus> GetAllRelationships(const FString& CharacterID) const;

    // 個人決策
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void MakePersonalDecision(const FMingPersonalDecision& Decision);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void ProcessPersonalDecisions();

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    TArray<FMingPersonalDecision> GetPersonalDecisions() const;

    // 角色成長
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void ProcessCharacterGrowth(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    FMingCharacterGrowth GetCharacterGrowth(const FString& CharacterID) const;

    // 獲取個人狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    FMingPersonalState GetPersonalState() const;

    // 獲取主要角色
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    FMingCharacterStatus GetMainCharacter() const;

    // 設置主要角色
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void SetMainCharacter(const FString& CharacterID);

protected:
    // 個人狀態
    UPROPERTY(BlueprintReadOnly)
    FMingPersonalState PersonalState;

    // 角色列表
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingCharacterStatus> Characters;

    // 個人決策
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingPersonalDecision> PersonalDecisions;

    // 個人事件歷史
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingPersonalEvent> PersonalHistory;

    // 角色成長記錄
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingCharacterGrowth> CharacterGrowthRecords;

    // 主要角色ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal|Settings")
    FString MainCharacterID;

    // 角色發展風格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal|Settings")
    FString CharacterDevelopmentStyle;

    // 關係管理偏好
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal|Settings")
    float RelationshipManagementPreference;

    // 技能發展偏好
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal|Settings")
    TMap<EMingCharacterSkill, float> SkillDevelopmentPreferences;

    // 分析角色狀態
    void AnalyzeCharacterStatus(const FString& CharacterID);

    // 評估關係網絡
    void AssessRelationshipNetwork(const FString& CharacterID);

    // 計算角色影響力
    float CalculateCharacterInfluence(const FString& CharacterID);

    // 預測決策後果
    TArray<FString> PredictDecisionOutcome(const FMingPersonalDecision& Decision);

    // 民國特色個人發展
    void ApplyRepublicanEraCharacterDevelopment();

    // 新文化運動影響
    void ApplyNewCultureMovementInfluence(const FString& CharacterID);

    // 教育改革機遇
    void ApplyEducationReformOpportunities(const FString& CharacterID);

    // 女性解放運動
    void ApplyWomenLiberationMovement(const FString& CharacterID);

    // 知識分子選擇
    void ApplyIntellectualChoices(const FString& CharacterID);

    // 商業發展機遇
    void ApplyBusinessDevelopmentOpportunities(const FString& CharacterID);

    // 軍事生涯發展
    void ApplyMilitaryCareerDevelopment(const FString& CharacterID);

private:
    // 個人更新間隔
    float PersonalUpdateInterval;

    // 上次更新時間
    float LastPersonalUpdate;

    // 角色等級上限
    int32 MaxCharacterLevel;

    // 技能等級上限
    float MaxSkillLevel;

    // 關係影響半徑
    float RelationshipInfluenceRadius;

    // 個人威脅評估
    TMap<FString, float> PersonalThreats;

    // 個人機會評估
    TMap<FString, float> PersonalOpportunities;

    // 個人決策歷史
    TArray<FMingPersonalDecision> DecisionHistory;
};
