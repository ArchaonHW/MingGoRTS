#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingHistoricalCharacter.h"
#include "MingCharacterBlueprintLibrary.generated.h"

/**
 * H目標數量 * 故事重要性摧毀?vHt??
 */
UCLASS()
class MINGSTRATEGIC_API UMingCharacterBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 摧毀vH?X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static UMingHistoricalCharacterManager* GetHistoricalCharacterManager();

    /**
     * 目標數量H??t??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static void InitializeHistoricalCharacterSystem();

    /**
     * ??vH
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character);

    /**
     * 摧毀??vH
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UnregisterHistoricalCharacter(const FString& CharacterID);

    /**
     * 摧毀vH
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FMingHistoricalCharacter GetHistoricalCharacter(const FString& CharacterID);

    /**
     * 目標數量??HX     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetAllHistoricalCharacters();

    /**
     * 故事重要性?HX     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharactersByFaction(ECharacterFaction Faction);

    /**
     * 故事重要性?~??HX     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharactersByProfession(ECharacterProfession Profession);

    /**
     * ???X?H
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetKeyCharacters();

    /**
     * 故事重要性?HX     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetRecruitableCharacters();

    /**
     * 摧毀??H
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetActiveCharacters();

    /**
     * H??
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FCharacterInteractionResult InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails);

    /**
     * ܨt
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FDialogueEvent StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic);

    /**
     * 摧毀摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FDialogueEvent ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID);

    /**
     * 摧毀H
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool RecruitCharacter(const FString& CharacterID);

    /**
     * 摧毀H
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool DismissCharacter(const FString& CharacterID);

    /**
     * 摧毀H??X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType);

    /**
     * 摧毀H摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FCharacterRelationship> GetCharacterRelationships(const FString& CharacterID);

    /**
     * K??H摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength);

    /**
     * ??sH摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength);

    /**
     * 摧毀H??
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static TMap<FString, int32> GetCharacterStatistics();

    /**
     * 摧毀HvT???X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharacterInfluenceRanking();

    /**
     * ???X?ˤH
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetRecommendedCharacters(const FString& PlayerFaction);

    /**
     * ???X?O??
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetGenderName(ECharacterGender Gender);

    /**
     * ???X?~??
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetProfessionName(ECharacterProfession Profession);

    /**
     * ???X?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetFactionName(ECharacterFaction Faction);

    /**
     * 目標數量摧毀     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetStateName(ECharacterState State);

    /**
     * 目標數量?X?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetRelationshipTypeName(ECharacterRelationshipType RelationshipType);

    /**
     * 目標數量?X摧毀?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetSkillTypeName(ECharacterSkillType SkillType);

    /**
     * ??ؾvH
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
     * ]mH??     */
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
     * K??H??X     */
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
     * K??H
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddCharacterTag(
        const FMingHistoricalCharacter& Character,
        const FString& Tag
    );

    /**
     * K??vƥ
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddHistoricalEvent(
        const FMingHistoricalCharacter& Character,
        const FString& Event
    );

    /**
     * K?X?N
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddAchievement(
        const FMingHistoricalCharacter& Character,
        const FString& Achievement
    );

    /**
     * ]mvT目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter SetInfluenceAndReputation(
        const FMingHistoricalCharacter& Character,
        float Influence,
        float Reputation
    );

    /**
     * 摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Batch")
    static TArray<FCharacterInteractionResult> BatchInteractWithCharacters(const TArray<FString>& CharacterIDs, const FString& InteractionType);

    /**
     * 目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Batch")
    static TArray<bool> BatchRecruitCharacters(const TArray<FString>& CharacterIDs);

    /**
     * 摧毀t??X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static bool IsHistoricalCharacterSystemInitialized();

    /**
     * 摧毀t??
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static FString GetHistoricalCharacterSystemVersion();

    /**
     * 摧毀??H摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetRegisteredCharacterCount();

    /**
     * 摧毀??H摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetActiveCharacterCount();

    /**
     * ???X?H摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetKeyCharacterCount();

    /**
     * 故事重要性?H??X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetRecruitableCharacterCount();

    /**
     * 摧毀H??z
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static FString GetCharacterDescription(const FString& CharacterID);

    /**
     * 摧毀H~
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static int32 GetCharacterAge(const FString& CharacterID);

    /**
     * ˬdH??_???X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static bool IsCharacterRecruitable(const FString& CharacterID);

    /**
     * ˬdH??_???X?HX     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static bool IsCharacterKeyCharacter(const FString& CharacterID);

    /**
     * 摧毀H摧毀摧毀     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Skills")
    static int32 GetCharacterSkillLevel(const FString& CharacterID, ECharacterSkillType SkillType);

    /**
     * 摧毀H摧毀?X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Skills")
    static TArray<FCharacterSkill> GetCharacterSkills(const FString& CharacterID);

    /**
     * ˬdH??_???X?X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Skills")
    static bool HasCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType);

    /**
     * 目標數量j
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static float GetRelationshipStrength(const FString& CharacterID, const FString& TargetCharacterID);

    /**
     * 目標數量??
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static ECharacterRelationshipType GetRelationshipType(const FString& CharacterID, const FString& TargetCharacterID);

    /**
     * ˬd??_sb摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static bool HasRelationship(const FString& CharacterID, const FString& TargetCharacterID);

    /**
     * 摧毀HvTX     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterInfluence(const FString& CharacterID);

    /**
     * 摧毀H摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterReputation(const FString& CharacterID);

    /**
     * 摧毀H摧毀     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterLoyalty(const FString& CharacterID);

    /**
     * 摧毀H??d摧毀     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterHealth(const FString& CharacterID);

    /**
     * 摧毀H?X?X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterMorale(const FString& CharacterID);

    /**
     * 摧毀H??
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static int32 GetCharacterLevel(const FString& CharacterID);

    /**
     * 摧毀Hg??
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static int32 GetCharacterExperience(const FString& CharacterID);

    /**
     * 摧毀H`摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Analysis")
    static float GetCharacterOverallScore(const FString& CharacterID);

    /**
     * 摧毀??ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetInteractionRecommendations(const FString& CharacterID);

    /**
     * 摧毀ܫĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetDialogueRecommendations(const FString& SpeakerID, const FString& AudienceID);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetRecruitmentRecommendations(const FString& PlayerFaction);

    // 摧毀H摧毀 - `??GTMap<TArray> XBlueprint
    static TMap<FString, TArray<FString>> GetCharacterNetwork(const FString& CharacterID);

    /**
     * 目標數量vT摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Network")
    static TMap<FString, float> GetRelationshipImpactAnalysis(const FString& CharacterID);

    /**
     * H??
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Simulation")
    static TArray<FCharacterInteractionResult> SimulateCharacterInteractions(const FString& CharacterID, int32 SimulationCount);

    /**
     * ܵ??
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Simulation")
    static TArray<FDialogueEvent> SimulateDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, int32 SimulationCount);

    /**
     * ɥXH摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Export")
    static bool ExportCharacterData(const FString& FilePath);

    /**
     * ɤJH摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Import")
    static bool ImportCharacterData(const FString& FilePath);

    /**
     * 摧毀H摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateCharacterReport(const FString& CharacterID);

    /**
     * ??目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateStatisticsReport();

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateRelationshipReport(const FString& CharacterID);

    /**
     * 摧毀??v
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|History")
    static TArray<FDialogueEvent> GetDialogueHistory();

    /**
     * 摧毀ܾv
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|History")
    static TArray<FDialogueEvent> GetDialogueHistoryForCharacter(const FString& CharacterID);
);

