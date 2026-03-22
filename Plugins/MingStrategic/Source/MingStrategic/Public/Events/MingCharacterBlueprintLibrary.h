#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingHistoricalCharacter.h"
#include "MingCharacterBlueprintLibrary.generated.h"

/**
 * 人物藍圖函數庫
 * 提供藍圖可調用的歷史人物系統功能
 */
UCLASS()
class MINGSTRATEGIC_API UMingCharacterBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取歷史人物管理器
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static UMingHistoricalCharacterManager* GetHistoricalCharacterManager();

    /**
     * 初始化歷史人物系統
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static void InitializeHistoricalCharacterSystem();

    /**
     * 註冊歷史人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character);

    /**
     * 取消註冊歷史人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UnregisterHistoricalCharacter(const FString& CharacterID);

    /**
     * 獲取歷史人物
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FMingHistoricalCharacter GetHistoricalCharacter(const FString& CharacterID);

    /**
     * 獲取所有歷史人物
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetAllHistoricalCharacters();

    /**
     * 獲取指定陣營的人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharactersByFaction(ECharacterFaction Faction);

    /**
     * 獲取指定職業的人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharactersByProfession(ECharacterProfession Profession);

    /**
     * 獲取關鍵人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetKeyCharacters();

    /**
     * 獲取可招募人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetRecruitableCharacters();

    /**
     * 獲取活躍人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetActiveCharacters();

    /**
     * 人物互動
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FCharacterInteractionResult InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails);

    /**
     * 對話系統
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FDialogueEvent StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic);

    /**
     * 處理對話選項
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FDialogueEvent ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID);

    /**
     * 招募人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool RecruitCharacter(const FString& CharacterID);

    /**
     * 解僱人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool DismissCharacter(const FString& CharacterID);

    /**
     * 提升人物技能
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType);

    /**
     * 獲取人物關係
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FCharacterRelationship> GetCharacterRelationships(const FString& CharacterID);

    /**
     * 添加人物關係
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength);

    /**
     * 更新人物關係
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength);

    /**
     * 獲取人物統計
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static TMap<FString, int32> GetCharacterStatistics();

    /**
     * 獲取人物影響力排名
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharacterInfluenceRanking();

    /**
     * 獲取推薦人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetRecommendedCharacters(const FString& PlayerFaction);

    /**
     * 獲取性別名稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetGenderName(ECharacterGender Gender);

    /**
     * 獲取職業名稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetProfessionName(ECharacterProfession Profession);

    /**
     * 獲取陣營名稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetFactionName(ECharacterFaction Faction);

    /**
     * 獲取狀態名稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetStateName(ECharacterState State);

    /**
     * 獲取關係類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetRelationshipTypeName(ECharacterRelationshipType RelationshipType);

    /**
     * 獲取技能類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetSkillTypeName(ECharacterSkillType SkillType);

    /**
     * 創建歷史人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter CreateHistoricalCharacter(
        const FString& CharacterID,
        const FString& CharacterName,
        const FString& CourtesyName,
        const FString& EnglishName,
        const FString& CharacterDescription,
        ECharacterGender Gender,
        ECharacterProfession Profession,
        ECharacterFaction Faction,
        int32 BirthYear,
        int32 DeathYear,
        bool bIsKeyCharacter,
        bool bIsRecruitable
    );

    /**
     * 設置人物屬性
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter SetCharacterAttributes(
        const FMingHistoricalCharacter& Character,
        float Leadership,
        float Military,
        float Diplomacy,
        float Economics,
        float Intelligence,
        float Charisma,
        float Administration,
        float Strategy,
        float Oratory,
        float Writing,
        float Science,
        float Engineering,
        float Medicine,
        float Art
    );

    /**
     * 添加人物技能
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddCharacterSkill(
        const FMingHistoricalCharacter& Character,
        ECharacterSkillType SkillType,
        const FString& SkillName,
        const FString& SkillDescription,
        int32 SkillLevel,
        bool bIsUnlocked
    );

    /**
     * 添加人物標籤
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddCharacterTag(
        const FMingHistoricalCharacter& Character,
        const FString& Tag
    );

    /**
     * 添加歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddHistoricalEvent(
        const FMingHistoricalCharacter& Character,
        const FString& Event
    );

    /**
     * 添加成就
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddAchievement(
        const FMingHistoricalCharacter& Character,
        const FString& Achievement
    );

    /**
     * 設置影響力和聲望
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter SetInfluenceAndReputation(
        const FMingHistoricalCharacter& Character,
        float Influence,
        float Reputation
    );

    /**
     * 批量互動
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Batch")
    static TArray<FCharacterInteractionResult> BatchInteractWithCharacters(const TArray<FString>& CharacterIDs, const FString& InteractionType);

    /**
     * 批量招募
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Batch")
    static TArray<bool> BatchRecruitCharacters(const TArray<FString>& CharacterIDs);

    /**
     * 獲取系統狀態
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static bool IsHistoricalCharacterSystemInitialized();

    /**
     * 獲取系統版本
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static FString GetHistoricalCharacterSystemVersion();

    /**
     * 獲取註冊人物數量
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetRegisteredCharacterCount();

    /**
     * 獲取活躍人物數量
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetActiveCharacterCount();

    /**
     * 獲取關鍵人物數量
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetKeyCharacterCount();

    /**
     * 獲取可招募人物數量
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetRecruitableCharacterCount();

    /**
     * 獲取人物描述
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static FString GetCharacterDescription(const FString& CharacterID);

    /**
     * 獲取人物年齡
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static int32 GetCharacterAge(const FString& CharacterID);

    /**
     * 檢查人物是否可招募
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static bool IsCharacterRecruitable(const FString& CharacterID);

    /**
     * 檢查人物是否為關鍵人物
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static bool IsCharacterKeyCharacter(const FString& CharacterID);

    /**
     * 獲取人物技能等級
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Skills")
    static int32 GetCharacterSkillLevel(const FString& CharacterID, ECharacterSkillType SkillType);

    /**
     * 獲取人物所有技能
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Skills")
    static TArray<FCharacterSkill> GetCharacterSkills(const FString& CharacterID);

    /**
     * 檢查人物是否擁有技能
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Skills")
    static bool HasCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType);

    /**
     * 獲取關係強度
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static float GetRelationshipStrength(const FString& CharacterID, const FString& TargetCharacterID);

    /**
     * 獲取關係類型
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static ECharacterRelationshipType GetRelationshipType(const FString& CharacterID, const FString& TargetCharacterID);

    /**
     * 檢查是否存在關係
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static bool HasRelationship(const FString& CharacterID, const FString& TargetCharacterID);

    /**
     * 獲取人物影響力
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterInfluence(const FString& CharacterID);

    /**
     * 獲取人物聲望
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterReputation(const FString& CharacterID);

    /**
     * 獲取人物忠誠度
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterLoyalty(const FString& CharacterID);

    /**
     * 獲取人物健康狀況
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterHealth(const FString& CharacterID);

    /**
     * 獲取人物精神狀態
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterMorale(const FString& CharacterID);

    /**
     * 獲取人物等級
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static int32 GetCharacterLevel(const FString& CharacterID);

    /**
     * 獲取人物經驗
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static int32 GetCharacterExperience(const FString& CharacterID);

    /**
     * 獲取人物總體評分
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Analysis")
    static float GetCharacterOverallScore(const FString& CharacterID);

    /**
     * 獲取互動建議
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetInteractionRecommendations(const FString& CharacterID);

    /**
     * 獲取對話建議
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetDialogueRecommendations(const FString& SpeakerID, const FString& AudienceID);

    /**
     * 獲取招募建議
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetRecruitmentRecommendations(const FString& PlayerFaction);

    /**
     * 獲取人物關係網絡
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Network")
    static TMap<FString, TArray<FString>> GetCharacterNetwork(const FString& CharacterID);

    /**
     * 獲取關係影響分析
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Network")
    static TMap<FString, float> GetRelationshipImpactAnalysis(const FString& CharacterID);

    /**
     * 模擬人物互動
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Simulation")
    static TArray<FCharacterInteractionResult> SimulateCharacterInteractions(const FString& CharacterID, int32 SimulationCount);

    /**
     * 模擬對話結果
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Simulation")
    static TArray<FDialogueEvent> SimulateDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, int32 SimulationCount);

    /**
     * 導出人物數據
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Export")
    static bool ExportCharacterData(const FString& FilePath);

    /**
     * 導入人物數據
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Import")
    static bool ImportCharacterData(const FString& FilePath);

    /**
     * 獲取人物報告
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateCharacterReport(const FString& CharacterID);

    /**
     * 獲取統計報告
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateStatisticsReport();

    /**
     * 獲取關係報告
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateRelationshipReport(const FString& CharacterID);

    /**
     * 獲取互動歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|History")
    static TArray<FDialogueEvent> GetDialogueHistory();

    /**
     * 獲取對話歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|History")
    static TArray<FDialogueEvent> GetDialogueHistoryForCharacter(const FString& CharacterID);
};
