#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalCharacterSystem.generated.h"

// ‰∫∫Áâ©È°ûÂ??öË?
UENUM(BlueprintType)
enum class EMingCharacterType : uint8
{
    MilitaryLeader,    // Ëªç‰??òË?
    Politician,        // ?øÊ≤ªÂÆ?    Diplomat,          // Â§ñ‰∫§ÂÆ?    Businessman,       // ?Ü‰∫∫
    Scholar,           // Â≠∏ËÄ?    Revolutionary,     // ?©ÂëΩÂÆ?    Warlord,           // ËªçÈñ•
    ForeignAdvisor     // Â§ñÂ?È°ßÂ?
};

// ‰∫∫Áâ©?ú‰?È°ûÂ?
UENUM(BlueprintType)
enum class EMingRelationshipType : uint8
{
    Ally,              // ?åÁ?
    Enemy,             // ?µÂ?
    Neutral,           // ‰∏≠Á?
    Subordinate,       // ‰∏ãÂ±¨
    Superior,          // ‰∏äÁ?
    Family,            // ÂÆ∂Ê?
    Mentor,            // Â∞éÂ∏´
    Rival              // Á´∂Áà≠Â∞çÊ?
};

// Â∞çË©±È°ûÂ?
UENUM(BlueprintType)
enum class EMingDialogueType : uint8
{
    Greeting,          // ?èÂÄ?    Political,         // ?øÊ≤ªË®éË?
    Military,          // Ëªç‰??ÜË≠∞
    Economic,          // Á∂ìÊ?Ë´áÂà§
    Personal,          // ?ã‰∫∫‰∫§Ë?
    Diplomatic,        // Â§ñ‰∫§‰∫§Ê?
    Threat,            // Â®ÅË?
    Persuasion,        // Ë™™Ê?
    Request,           // Ë´ãÊ?
    Information        // ‰ø°ÊÅØ?∂È?
};

/**
 * ?øÊ≤ª?ØÁ?Ë®òÈ?ÁµêÊ? (?ø‰ª£ TTuple)
 */
USTRUCT(BlueprintType)
struct FMingPoliticalAlliance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID2;

    FMingPoliticalAlliance()
        : CharacterID1(0)
        , CharacterID2(0)
    {}
};

// Â∞çË©±?∏È?ÁµêÊ?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDialogueOption
{
    GENERATED_BODY()

    // ?∏È?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // ?∏È??áÊú¨
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionText;

    // ?∏È??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // ?∏È?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDialogueType DialogueType;

    // ?∏È??çÁΩÆÊ¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Prerequisites;

    // ?∏È?ÂæåÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Consequences;

    // ?ú‰?ÂΩ±Èüø
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> RelationshipChanges;

    // Ëß???ÑÊñ∞Â∞çË©±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedDialogues;

    FMingDialogueOption()
    {
        OptionID = -1;
        OptionText = TEXT("");
        OptionDescription = TEXT("");
        DialogueType = EMingDialogueType::Greeting;
        Prerequisites.Empty();
        Consequences.Empty();
        RelationshipChanges.Empty();
        UnlockedDialogues.Empty();
    }
};

// Â∞çË©±ÁØÄÈªûÁ?Êß?USTRUCT(BlueprintType)
struct FINGCORE_API FMingDialogueNode
{
    GENERATED_BODY()

    // ÁØÄÈªûID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NodeID;

    // ÁØÄÈªûÊ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeText;

    // Ë™™Ë©±??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SpeakerName;

    // Â∞çË©±È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDialogueType DialogueType;

    // Â∞çË©±?∏È?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDialogueOption> Options;

    // Ê¢ù‰ª∂Ê™¢Êü•
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Conditions;

    // Â∞çË©±?≥Ê?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DialogueSound;

    // Â∞çË©±?ïÁï´
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DialogueAnimation;

    FMingDialogueNode()
    {
        NodeID = -1;
        NodeText = TEXT("");
        SpeakerName = TEXT("");
        DialogueType = EMingDialogueType::Greeting;
        Options.Empty();
        Conditions.Empty();
        DialogueSound = TEXT("");
        DialogueAnimation = TEXT("");
    }
};

// Ê≠∑Âè≤‰∫∫Áâ©?∏Ê?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalCharacter
{
    GENERATED_BODY()

    // ‰∫∫Áâ©ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID;

    // ‰∫∫Áâ©ÂßìÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterName;

    // ‰∫∫Áâ©Â≠óË?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CourtesyName;

    // ‰∫∫Áâ©È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingCharacterType CharacterType;

    // ‰∫∫Áâ©?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterDescription;

    // Ê≠∑Âè≤?åÊôØ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    // ‰∫∫Áâ©Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> CharacterAttributes;

    // ‰∫∫Áâ©?Ä??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Skills;

    // ‰∫∫Áâ©?ú‰?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> Relationships;

    // ?∂Â?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CurrentLocation;

    // ?ÄÂ±¨Âã¢??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Faction;

    // ‰∫∫Áâ©?Ä??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CurrentStatus;

    // ?ØÂê¶?ØÁî®?ºÂ?Ë©?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAvailableForDialogue;

    // Â∞çË©±Ê®πÊ†πÁØÄÈª?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RootDialogueNode;

    FMingHistoricalCharacter()
    {
        CharacterID = -1;
        CharacterName = TEXT("");
        CourtesyName = TEXT("");
        CharacterType = EMingCharacterType::MilitaryLeader;
        CharacterDescription = TEXT("");
        HistoricalBackground = TEXT("");
        CharacterAttributes.Empty();
        Skills.Empty();
        Relationships.Empty();
        CurrentLocation = TEXT("");
        Faction = TEXT("");
        CurrentStatus = TEXT("");
        bAvailableForDialogue = true;
        RootDialogueNode = -1;
    }
};

// ‰∫∫Áâ©‰∫íÂ?Ë®òÈ?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingCharacterInteraction
{
    GENERATED_BODY()

    // ‰∫íÂ?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InteractionID;

    // ‰∫∫Áâ©ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID;

    // ‰∫íÂ??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime InteractionTime;

    // ‰∫íÂ??∞È?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString InteractionLocation;

    // Â∞çË©±ÁØÄÈªûID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DialogueNodeID;

    // ?∏Ê??ÑÈÅ∏?ÖID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // ‰∫íÂ?ÁµêÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> InteractionResults;

    // ?ú‰?ËÆäÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> RelationshipChanges;

    // Ëß???ÑÊñ∞?ßÂÆπ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockedContent;

    FMingCharacterInteraction()
    {
        InteractionID = -1;
        CharacterID = -1;
        InteractionTime = FDateTime::Now();
        InteractionLocation = TEXT("");
        DialogueNodeID = -1;
        ChosenOptionID = -1;
        InteractionResults.Empty();
        RelationshipChanges.Empty();
        UnlockedContent.Empty();
    }
};

// ‰∫∫Áâ©‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterEncountered, int32, CharacterID, const FMingHistoricalCharacter&, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueStarted, int32, CharacterID, int32, NodeID, const FMingDialogueNode&, DialogueNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueOptionChosen, int32, CharacterID, int32, OptionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipChanged, const FString&, CharacterName, int32, NewRelationshipValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionCompleted, const FMingCharacterInteraction&, Interaction);

/**
 * Ê≠∑Âè≤‰∫∫Áâ©Á≥ªÁµ±
 * Ë≤†Ë≤¨ÁÆ°Á?Ê≠∑Âè≤‰∫∫Áâ©?ÑÂ?Ë©±Â?‰∫íÂ?
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalCharacterSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalCharacterSystem();

    // ?ùÂ??ñ‰∫∫?©Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool InitializeCharacterSystem();

    // Ë®ªÂ?Ê≠∑Âè≤‰∫∫Áâ©
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character);

    // ?≤Â?‰∫∫Áâ©‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingHistoricalCharacter GetCharacterInfo(int32 CharacterID) const;

    // ?≤Â??∂Â?‰ΩçÁΩÆ?Ñ‰∫∫??    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<int32> GetCharactersAtLocation(const FString& Location);

    // ?ãÂ?Â∞çË©±
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool StartDialogue(int32 CharacterID);

    // ?≤Â?Â∞çË©±ÁØÄÈª?    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingDialogueNode GetDialogueNode(int32 NodeID) const;

    // ?∏Ê?Â∞çË©±?∏È?
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool ChooseDialogueOption(int32 CharacterID, int32 NodeID, int32 OptionID);

    // Ê™¢Êü•Â∞çË©±Ê¢ù‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool CheckDialogueConditions(int32 NodeID) const;

    // ?≤Â?‰∫∫Áâ©?ú‰?
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    int32 GetRelationshipValue(int32 CharacterID, const FString& TargetCharacterName) const;

    // ?¥Êñ∞‰∫∫Áâ©?ú‰?
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpdateRelationship(int32 CharacterID, const FString& TargetCharacterName, int32 ChangeValue);

    // ?≤Â?‰∫íÂ?Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<FMingCharacterInteraction> GetInteractionHistory(int32 CharacterID) const;

    // Âª∫Á??øÊ≤ª?ØÁ?
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool CreatePoliticalAlliance(int32 CharacterID1, int32 CharacterID2);

    // ?¥Â??øÊ≤ª?ØÁ?
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool BreakPoliticalAlliance(int32 CharacterID1, int32 CharacterID2);

    // ?≤Â?‰∫∫Áâ©Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TMap<EMingCharacterType, int32> GetCharacterStatistics() const;

    // ‰øùÂ?‰∫∫Áâ©?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool SaveCharacterData();

    // ËºâÂÖ•‰∫∫Áâ©?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool LoadCharacterData();

    // Ê∏ÖÈô§?Ä?â‰∫∫?©Êï∏??    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void ClearAllCharacterData();

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable)
    FOnCharacterEncountered OnCharacterEncountered;

    UPROPERTY(BlueprintAssignable)
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable)
    FOnDialogueOptionChosen OnDialogueOptionChosen;

    UPROPERTY(BlueprintAssignable)
    FOnRelationshipChanged OnRelationshipChanged;

    UPROPERTY(BlueprintAssignable)
    FOnInteractionCompleted OnInteractionCompleted;

protected:
    // ‰∫∫Áâ©?∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingHistoricalCharacter> CharacterDatabase;

    // Â∞çË©±ÁØÄÈªûÊï∏?öÂ∫´
    UPROPERTY()
    TMap<int32, FMingDialogueNode> DialogueNodeDatabase;

    // ‰∫íÂ?Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY()
    TArray<FMingCharacterInteraction> InteractionHistory;

    // ?∂Â?Â∞çË©±‰∏≠Á?‰∫∫Áâ©
    UPROPERTY()
    TArray<int32> ActiveDialogueCharacters;

    // ?øÊ≤ª?ØÁ?Ë®òÈ? - ‰ΩøÁî®?ÖË?ÁµêÊ??ø‰ª£ TTuple
    UPROPERTY()
    TArray<FMingPoliticalAlliance> PoliticalAlliances;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ËºâÂÖ•?êË®≠‰∫∫Áâ©?∏Ê?
    void LoadDefaultCharacters();

    // ËºâÂÖ•?êË®≠Â∞çË©±?∏Ê?
    void LoadDefaultDialogues();

    // È©óË?‰∫∫Áâ©?∏Ê?
    bool ValidateCharacterData(const FMingHistoricalCharacter& Character) const;

    // È©óË?Â∞çË©±?∏Ê?
    bool ValidateDialogueData(const FMingDialogueNode& DialogueNode) const;

    // ?ïÁ?Â∞çË©±?∏È?ÂæåÊ?
    void ProcessDialogueConsequences(int32 CharacterID, int32 NodeID, int32 OptionID);

    // ?¥Êñ∞‰∫∫Áâ©?Ä??    void UpdateCharacterStatus(int32 CharacterID, const FString& NewStatus);

    // Ë®òÈ?‰∫íÂ?
    void RecordInteraction(const FMingCharacterInteraction& Interaction);

    // Ëß∏Áôº‰∫∫Áâ©‰∫ã‰ª∂
    void TriggerCharacterEvent(int32 CharacterID, const FString& EventType);

    // ?üÊ??ïÊ?Â∞çË©±
    FMingDialogueNode GenerateDynamicDialogue(int32 CharacterID, const FString& Context) const;
};
