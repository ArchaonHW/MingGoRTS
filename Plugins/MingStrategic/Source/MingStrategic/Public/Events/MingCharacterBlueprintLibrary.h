#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingHistoricalCharacter.h"
#include "MingCharacterBlueprintLibrary.generated.h"

/**
 * 人物?��X�數�? * ?��X��X�調?��?歷史人物系統?�能
 */
UCLASS()
class MINGSTRATEGIC_API UMingCharacterBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?��?歷史人物管�X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static UMingHistoricalCharacterManager* GetHistoricalCharacterManager(};

    /**
     * ?��X�歷?�人?�系�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static void InitializeHistoricalCharacterSystem(};

    /**
     * 註�?歷史人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character};

    /**
     * ?��?註�?歷史人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UnregisterHistoricalCharacter(const FString& CharacterID};

    /**
     * ?��?歷史人物
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FMingHistoricalCharacter GetHistoricalCharacter(const FString& CharacterID};

    /**
     * ?��X�?�歷?�人X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetAllHistoricalCharacters(};

    /**
     * ?��X��X�人X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharactersByFaction(ECharacterFaction Faction};

    /**
     * ?��X��X�業?�人X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharactersByProfession(ECharacterProfession Profession};

    /**
     * ?��X�鍵人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetKeyCharacters(};

    /**
     * ?��X��X�人X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetRecruitableCharacters(};

    /**
     * ?��?活�?人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetActiveCharacters(};

    /**
     * 人物互�?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FCharacterInteractionResult InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails};

    /**
     * 對話系統
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FDialogueEvent StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic};

    /**
     * ?��?對話?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FDialogueEvent ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID};

    /**
     * ?��?人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool RecruitCharacter(const FString& CharacterID};

    /**
     * �?��人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool DismissCharacter(const FString& CharacterID};

    /**
     * ?��?人物?�X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType};

    /**
     * ?��?人物?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FCharacterRelationship> GetCharacterRelationships(const FString& CharacterID};

    /**
     * 添�?人物?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength};

    /**
     * ?�新人物?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength};

    /**
     * ?��?人物統�?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static TMap<FString, int32> GetCharacterStatistics(};

    /**
     * ?��?人物影響?��X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharacterInfluenceRanking(};

    /**
     * ?��X�薦人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetRecommendedCharacters(const FString& PlayerFaction};

    /**
     * ?��X�別?�稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetGenderName(ECharacterGender Gender};

    /**
     * ?��X�業?�稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetProfessionName(ECharacterProfession Profession};

    /**
     * ?��X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetFactionName(ECharacterFaction Faction};

    /**
     * ?��X�?��?�?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetStateName(ECharacterState State};

    /**
     * ?��X��?類�X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetRelationshipTypeName(ECharacterRelationshipType RelationshipType};

    /**
     * ?��X�?��X��?�?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetSkillTypeName(ECharacterSkillType SkillType};

    /**
     * ?�建歷史人物
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
    };

    /**
     * 設置人物屬�?     */
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
    };

    /**
     * 添�?人物?�X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddCharacterSkill(
        const FMingHistoricalCharacter& Character,
        ECharacterSkillType SkillType,
        const FString& SkillName,
        const FString& SkillDescription,
        int32 SkillLevel,
        bool bIsUnlocked
    };

    /**
     * 添�?人物標籤
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddCharacterTag(
        const FMingHistoricalCharacter& Character,
        const FString& Tag
    };

    /**
     * 添�?歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddHistoricalEvent(
        const FMingHistoricalCharacter& Character,
        const FString& Event
    };

    /**
     * 添�X�就
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddAchievement(
        const FMingHistoricalCharacter& Character,
        const FString& Achievement
    };

    /**
     * 設置影響?��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter SetInfluenceAndReputation(
        const FMingHistoricalCharacter& Character,
        float Influence,
        float Reputation
    };

    /**
     * ?��?互�?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Batch")
    static TArray<FCharacterInteractionResult> BatchInteractWithCharacters(const TArray<FString>& CharacterIDs, const FString& InteractionType};

    /**
     * ?��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Batch")
    static TArray<bool> BatchRecruitCharacters(const TArray<FString>& CharacterIDs};

    /**
     * ?��?系統?�X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static bool IsHistoricalCharacterSystemInitialized(};

    /**
     * ?��?系統?�本
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static FString GetHistoricalCharacterSystemVersion(};

    /**
     * ?��?註�?人物?��?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetRegisteredCharacterCount(};

    /**
     * ?��?活�?人物?��?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetActiveCharacterCount(};

    /**
     * ?��X�鍵人物?��?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetKeyCharacterCount(};

    /**
     * ?��X��X�人?�數X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetRecruitableCharacterCount(};

    /**
     * ?��?人物?�述
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static FString GetCharacterDescription(const FString& CharacterID};

    /**
     * ?��?人物年齡
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static int32 GetCharacterAge(const FString& CharacterID};

    /**
     * 檢查人物?�否?��X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static bool IsCharacterRecruitable(const FString& CharacterID};

    /**
     * 檢查人物?�否?��X�人X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static bool IsCharacterKeyCharacter(const FString& CharacterID};

    /**
     * ?��?人物?�?��?�?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Skills")
    static int32 GetCharacterSkillLevel(const FString& CharacterID, ECharacterSkillType SkillType};

    /**
     * ?��?人物?�?��X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Skills")
    static TArray<FCharacterSkill> GetCharacterSkills(const FString& CharacterID};

    /**
     * 檢查人物?�否?��X�X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Skills")
    static bool HasCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType};

    /**
     * ?��X��?強度
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static float GetRelationshipStrength(const FString& CharacterID, const FString& TargetCharacterID};

    /**
     * ?��X��?類�?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static ECharacterRelationshipType GetRelationshipType(const FString& CharacterID, const FString& TargetCharacterID};

    /**
     * 檢查?�否存在?��?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static bool HasRelationship(const FString& CharacterID, const FString& TargetCharacterID};

    /**
     * ?��?人物影響X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterInfluence(const FString& CharacterID};

    /**
     * ?��?人物?��?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterReputation(const FString& CharacterID};

    /**
     * ?��?人物忠�?�?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterLoyalty(const FString& CharacterID};

    /**
     * ?��?人物?�康?��?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterHealth(const FString& CharacterID};

    /**
     * ?��?人物精�X�X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterMorale(const FString& CharacterID};

    /**
     * ?��?人物等�?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static int32 GetCharacterLevel(const FString& CharacterID};

    /**
     * ?��?人物經�?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static int32 GetCharacterExperience(const FString& CharacterID};

    /**
     * ?��?人物總�?評�?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Analysis")
    static float GetCharacterOverallScore(const FString& CharacterID};

    /**
     * ?��?互�?建議
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetInteractionRecommendations(const FString& CharacterID};

    /**
     * ?��?對話建議
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetDialogueRecommendations(const FString& SpeakerID, const FString& AudienceID};

    /**
     * ?��X��?建議
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetRecruitmentRecommendations(const FString& PlayerFaction};

    // ?��?人物?��?網絡 - 注�?：TMap<TArray> 不支XBlueprint
    static TMap<FString, TArray<FString>> GetCharacterNetwork(const FString& CharacterID};

    /**
     * ?��X��?影響?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Network")
    static TMap<FString, float> GetRelationshipImpactAnalysis(const FString& CharacterID};

    /**
     * 模擬人物互�?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Simulation")
    static TArray<FCharacterInteractionResult> SimulateCharacterInteractions(const FString& CharacterID, int32 SimulationCount};

    /**
     * 模擬對話結�?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Simulation")
    static TArray<FDialogueEvent> SimulateDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, int32 SimulationCount};

    /**
     * 導出人物?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Export")
    static bool ExportCharacterData(const FString& FilePath};

    /**
     * 導入人物?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Import")
    static bool ImportCharacterData(const FString& FilePath};

    /**
     * ?��?人物?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateCharacterReport(const FString& CharacterID};

    /**
     * ?��?統�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateStatisticsReport(};

    /**
     * ?��X��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateRelationshipReport(const FString& CharacterID};

    /**
     * ?��?互�?歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|History")
    static TArray<FDialogueEvent> GetDialogueHistory(};

    /**
     * ?��?對話歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|History")
    static TArray<FDialogueEvent> GetDialogueHistoryForCharacter(const FString& CharacterID};
};

