#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingHistoricalCharacter.h"
#include "MingCharacterBlueprintLibrary.generated.h"

/**
 * ‰∫∫Áâ©?çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á?Ê≠∑Âè≤‰∫∫Áâ©Á≥ªÁµ±?üËÉΩ
 */
UCLASS()
class MINGSTRATEGIC_API UMingCharacterBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â?Ê≠∑Âè≤‰∫∫Áâ©ÁÆ°Á???     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static UMingHistoricalCharacterManager* GetHistoricalCharacterManager(};

    /**
     * ?ùÂ??ñÊ≠∑?≤‰∫∫?©Á≥ªÁµ?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static void InitializeHistoricalCharacterSystem(};

    /**
     * Ë®ªÂ?Ê≠∑Âè≤‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character};

    /**
     * ?ñÊ?Ë®ªÂ?Ê≠∑Âè≤‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UnregisterHistoricalCharacter(const FString& CharacterID};

    /**
     * ?≤Â?Ê≠∑Âè≤‰∫∫Áâ©
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FMingHistoricalCharacter GetHistoricalCharacter(const FString& CharacterID};

    /**
     * ?≤Â??Ä?âÊ≠∑?≤‰∫∫??     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetAllHistoricalCharacters(};

    /**
     * ?≤Â??áÂ?????Ñ‰∫∫??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharactersByFaction(ECharacterFaction Faction};

    /**
     * ?≤Â??áÂ??∑Ê•≠?Ñ‰∫∫??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharactersByProfession(ECharacterProfession Profession};

    /**
     * ?≤Â??úÈçµ‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetKeyCharacters(};

    /**
     * ?≤Â??ØÊ??ü‰∫∫??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetRecruitableCharacters(};

    /**
     * ?≤Â?Ê¥ªË?‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetActiveCharacters(};

    /**
     * ‰∫∫Áâ©‰∫íÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FCharacterInteractionResult InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails};

    /**
     * Â∞çË©±Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FDialogueEvent StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic};

    /**
     * ?ïÁ?Â∞çË©±?∏È?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static FDialogueEvent ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID};

    /**
     * ?õÂ?‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool RecruitCharacter(const FString& CharacterID};

    /**
     * Ëß?É±‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool DismissCharacter(const FString& CharacterID};

    /**
     * ?êÂ?‰∫∫Áâ©?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType};

    /**
     * ?≤Â?‰∫∫Áâ©?ú‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FCharacterRelationship> GetCharacterRelationships(const FString& CharacterID};

    /**
     * Ê∑ªÂ?‰∫∫Áâ©?ú‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength};

    /**
     * ?¥Êñ∞‰∫∫Áâ©?ú‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static bool UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength};

    /**
     * ?≤Â?‰∫∫Áâ©Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static TMap<FString, int32> GetCharacterStatistics(};

    /**
     * ?≤Â?‰∫∫Áâ©ÂΩ±Èüø?õÊ???     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetCharacterInfluenceRanking(};

    /**
     * ?≤Â??®Ëñ¶‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    static TArray<FMingHistoricalCharacter> GetRecommendedCharacters(const FString& PlayerFaction};

    /**
     * ?≤Â??ßÂà•?çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetGenderName(ECharacterGender Gender};

    /**
     * ?≤Â??∑Ê•≠?çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetProfessionName(ECharacterProfession Profession};

    /**
     * ?≤Â?????çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetFactionName(ECharacterFaction Faction};

    /**
     * ?≤Â??Ä?ãÂ?Á®?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetStateName(ECharacterState State};

    /**
     * ?≤Â??ú‰?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetRelationshipTypeName(ECharacterRelationshipType RelationshipType};

    /**
     * ?≤Â??Ä?ΩÈ??ãÂ?Á®?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    static FString GetSkillTypeName(ECharacterSkillType SkillType};

    /**
     * ?µÂª∫Ê≠∑Âè≤‰∫∫Áâ©
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
     * Ë®≠ÁΩÆ‰∫∫Áâ©Â±¨ÊÄ?     */
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
     * Ê∑ªÂ?‰∫∫Áâ©?Ä??     */
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
     * Ê∑ªÂ?‰∫∫Áâ©Ê®ôÁ±§
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddCharacterTag(
        const FMingHistoricalCharacter& Character,
        const FString& Tag
    };

    /**
     * Ê∑ªÂ?Ê≠∑Âè≤‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddHistoricalEvent(
        const FMingHistoricalCharacter& Character,
        const FString& Event
    };

    /**
     * Ê∑ªÂ??êÂ∞±
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter AddAchievement(
        const FMingHistoricalCharacter& Character,
        const FString& Achievement
    };

    /**
     * Ë®≠ÁΩÆÂΩ±Èüø?õÂ??≤Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Creation")
    static FMingHistoricalCharacter SetInfluenceAndReputation(
        const FMingHistoricalCharacter& Character,
        float Influence,
        float Reputation
    };

    /**
     * ?πÈ?‰∫íÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Batch")
    static TArray<FCharacterInteractionResult> BatchInteractWithCharacters(const TArray<FString>& CharacterIDs, const FString& InteractionType};

    /**
     * ?πÈ??õÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Batch")
    static TArray<bool> BatchRecruitCharacters(const TArray<FString>& CharacterIDs};

    /**
     * ?≤Â?Á≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static bool IsHistoricalCharacterSystemInitialized(};

    /**
     * ?≤Â?Á≥ªÁµ±?àÊú¨
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static FString GetHistoricalCharacterSystemVersion(};

    /**
     * ?≤Â?Ë®ªÂ?‰∫∫Áâ©?∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetRegisteredCharacterCount(};

    /**
     * ?≤Â?Ê¥ªË?‰∫∫Áâ©?∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetActiveCharacterCount(};

    /**
     * ?≤Â??úÈçµ‰∫∫Áâ©?∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetKeyCharacterCount(};

    /**
     * ?≤Â??ØÊ??ü‰∫∫?©Êï∏??     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|System")
    static int32 GetRecruitableCharacterCount(};

    /**
     * ?≤Â?‰∫∫Áâ©?èËø∞
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static FString GetCharacterDescription(const FString& CharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©Âπ¥ÈΩ°
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static int32 GetCharacterAge(const FString& CharacterID};

    /**
     * Ê™¢Êü•‰∫∫Áâ©?ØÂê¶?ØÊ???     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static bool IsCharacterRecruitable(const FString& CharacterID};

    /**
     * Ê™¢Êü•‰∫∫Áâ©?ØÂê¶?∫È??µ‰∫∫??     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Info")
    static bool IsCharacterKeyCharacter(const FString& CharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©?Ä?ΩÁ?Á¥?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Skills")
    static int32 GetCharacterSkillLevel(const FString& CharacterID, ECharacterSkillType SkillType};

    /**
     * ?≤Â?‰∫∫Áâ©?Ä?âÊ???     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Skills")
    static TArray<FCharacterSkill> GetCharacterSkills(const FString& CharacterID};

    /**
     * Ê™¢Êü•‰∫∫Áâ©?ØÂê¶?ÅÊ??Ä??     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Skills")
    static bool HasCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType};

    /**
     * ?≤Â??ú‰?Âº∑Â∫¶
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static float GetRelationshipStrength(const FString& CharacterID, const FString& TargetCharacterID};

    /**
     * ?≤Â??ú‰?È°ûÂ?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static ECharacterRelationshipType GetRelationshipType(const FString& CharacterID, const FString& TargetCharacterID};

    /**
     * Ê™¢Êü•?ØÂê¶Â≠òÂú®?ú‰?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Relationships")
    static bool HasRelationship(const FString& CharacterID, const FString& TargetCharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©ÂΩ±Èüø??     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterInfluence(const FString& CharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©?≤Ê?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterReputation(const FString& CharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©Âø†Ë?Â∫?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterLoyalty(const FString& CharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©?•Â∫∑?ÄÊ≥?     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterHealth(const FString& CharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©Á≤æÁ??Ä??     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static float GetCharacterMorale(const FString& CharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©Á≠âÁ?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static int32 GetCharacterLevel(const FString& CharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©Á∂ìÈ?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character|Analysis")
    static int32 GetCharacterExperience(const FString& CharacterID};

    /**
     * ?≤Â?‰∫∫Áâ©Á∏ΩÈ?Ë©ïÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Analysis")
    static float GetCharacterOverallScore(const FString& CharacterID};

    /**
     * ?≤Â?‰∫íÂ?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetInteractionRecommendations(const FString& CharacterID};

    /**
     * ?≤Â?Â∞çË©±Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetDialogueRecommendations(const FString& SpeakerID, const FString& AudienceID};

    /**
     * ?≤Â??õÂ?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|AI")
    static TArray<FString> GetRecruitmentRecommendations(const FString& PlayerFaction};

    // ?≤Â?‰∫∫Áâ©?ú‰?Á∂≤Áµ° - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??Blueprint
    static TMap<FString, TArray<FString>> GetCharacterNetwork(const FString& CharacterID};

    /**
     * ?≤Â??ú‰?ÂΩ±Èüø?ÜÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Network")
    static TMap<FString, float> GetRelationshipImpactAnalysis(const FString& CharacterID};

    /**
     * Ê®°Êì¨‰∫∫Áâ©‰∫íÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Simulation")
    static TArray<FCharacterInteractionResult> SimulateCharacterInteractions(const FString& CharacterID, int32 SimulationCount};

    /**
     * Ê®°Êì¨Â∞çË©±ÁµêÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Simulation")
    static TArray<FDialogueEvent> SimulateDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, int32 SimulationCount};

    /**
     * Â∞éÂá∫‰∫∫Áâ©?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Export")
    static bool ExportCharacterData(const FString& FilePath};

    /**
     * Â∞éÂÖ•‰∫∫Áâ©?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Import")
    static bool ImportCharacterData(const FString& FilePath};

    /**
     * ?≤Â?‰∫∫Áâ©?±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateCharacterReport(const FString& CharacterID};

    /**
     * ?≤Â?Áµ±Ë??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateStatisticsReport(};

    /**
     * ?≤Â??ú‰??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|Report")
    static FString GenerateRelationshipReport(const FString& CharacterID};

    /**
     * ?≤Â?‰∫íÂ?Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|History")
    static TArray<FDialogueEvent> GetDialogueHistory(};

    /**
     * ?≤Â?Â∞çË©±Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character|History")
    static TArray<FDialogueEvent> GetDialogueHistoryForCharacter(const FString& CharacterID};
};

