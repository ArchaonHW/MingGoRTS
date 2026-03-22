#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSDialogueSystem.generated.h"

UENUM(BlueprintType)
enum class EDialogueType : uint8
{
    Conversation     UMETA(DisplayName = "?ÆÈÄöÂ?Ë©?),
    HistoricalEvent   UMETA(DisplayName = "Ê≠∑Âè≤‰∫ã‰ª∂"),
    MilitaryBriefing  UMETA(DisplayName = "Ëªç‰?Á∞°Â†±"),
    DiplomaticTalk   UMETA(DisplayName = "Â§ñ‰∫§Ë´áË©±"),
    PersonalStory     UMETA(DisplayName = "?ã‰∫∫?Ö‰?"),
    QuestDialogue     UMETA(DisplayName = "‰ªªÂ?Â∞çË©±")
};

UENUM(BlueprintType)
enum class EDialogueAttitude : uint8
{
    Friendly          UMETA(DisplayName = "?ãÂ•Ω"),
    Neutral           UMETA(DisplayName = "‰∏≠Á?"),
    Suspicious        UMETA(DisplayName = "?∑Á?"),
    Hostile          UMETA(DisplayName = "?µÂ?"),
    Respectful        UMETA(DisplayName = "Â∞äÊï¨"),
    Dismissive        UMETA(DisplayName = "ËºïË?")
};

UENUM(BlueprintType)
enum class EDialogueOutcome : uint8
{
    None              UMETA(DisplayName = "?°Ê???),
    RelationshipChange UMETA(DisplayName = "?ú‰?ËÆäÂ?"),
    QuestTrigger      UMETA(DisplayName = "Ëß∏Áôº‰ªªÂ?"),
    InformationGain   UMETA(DisplayName = "?≤Â??ÖÂ†±"),
    SkillGain         UMETA(DisplayName = "?Ä?ΩÊ???),
    EventTrigger      UMETA(DisplayName = "Ëß∏Áôº‰∫ã‰ª∂")
};

USTRUCT(BlueprintType)
struct FDialogueCondition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Condition")
    FString ConditionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Condition")
    FString Parameter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Condition")
    FString Operator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Condition")
    FString Value;

    FDialogueCondition()
    {
        ConditionType = TEXT("");
        Parameter = TEXT("");
        Operator = TEXT("");
        Value = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FDialogueOutcome
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Outcome")
    EDialogueOutcome OutcomeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Outcome")
    FString TargetID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Outcome")
    float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Outcome")
    FString Description;

    FDialogueOutcome()
    {
        OutcomeType = EDialogueOutcome::None;
        TargetID = TEXT("");
        Value = 0.0f;
        Description = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingGameDialogueOption
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    EDialogueAttitude Attitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    FString NextDialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FDialogueCondition> Conditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FDialogueOutcome> Outcomes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    bool bIsAvailable;

    FDialogueOption()
    {
        OptionText = TEXT("");
        Attitude = EDialogueAttitude::Neutral;
        NextDialogueID = TEXT("");
        bIsAvailable = true;
    }
};

USTRUCT(BlueprintType)
struct FMingDialogue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString DialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString SpeakerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString DialogueText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    EDialogueType DialogueType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString SpeakerPortrait;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FDialogueOption> DialogueOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FDialogueCondition> TriggerConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FDialogueOutcome> AutoOutcomes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString HistoricalContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bIsImportant;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bCanBeRepeated;

    FMingDialogue()
    {
        DialogueID = TEXT("");
        SpeakerID = TEXT("");
        DialogueText = TEXT("");
        DialogueType = EDialogueType::Conversation;
        SpeakerPortrait = TEXT("");
        BackgroundMusic = TEXT("");
        bIsImportant = false;
        bCanBeRepeated = true;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueStarted, const FString&, DialogueID, const FString&, SpeakerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueOptionSelected, const FString&, DialogueID, int32, OptionIndex, const FString&, OptionText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueCompleted, const FString&, DialogueID, const TArray<FDialogueOutcome>&, Outcomes);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSDialogueSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSDialogueSystem();

    // Â∞çË©±Á≥ªÁµ±‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
    FOnDialogueOptionSelected OnDialogueOptionSelected;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
    FOnDialogueCompleted OnDialogueCompleted;

    // ?ùÂ??ñÂ?Ë©±Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void InitializeDialogueSystem();

    // ?≤Â??Ä?âÂ?Ë©?    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FMingDialogue> GetAllDialogues() const;

    // ?≤Â?Â∞çË©±
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    FMingDialogue GetDialogue(const FString& DialogueID) const;

    // ?ãÂ?Â∞çË©±
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool StartDialogue(const FString& DialogueID, const FMingCharacterData& PlayerCharacter);

    // ?∏Ê?Â∞çË©±?∏È?
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool SelectDialogueOption(int32 OptionIndex);

    // Ê™¢Êü•Â∞çË©±?ØÂê¶?ØÁî®
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    bool IsDialogueAvailable(const FString& DialogueID, const FMingCharacterData& PlayerCharacter) const;

    // Ê™¢Êü•Â∞çË©±?∏È??ØÂê¶?ØÁî®
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    bool IsDialogueOptionAvailable(const FString& DialogueID, int32 OptionIndex, const FMingCharacterData& PlayerCharacter) const;

    // ?≤Â??ØÁî®Â∞çË©±?∏È?
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FDialogueOption> GetAvailableDialogueOptions(const FString& DialogueID, const FMingCharacterData& PlayerCharacter) const;

    // ?≤Â?ËßíËâ≤Â∞çË©±
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FMingDialogue> GetCharacterDialogues(const FString& CharacterID) const;

    // ?≤Â?Ê≠∑Âè≤‰∫ã‰ª∂Â∞çË©±
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FMingDialogue> GetHistoricalEventDialogues(const FString& EventID) const;

    // ?≤Â??çË?Â∞çË©±
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FMingDialogue> GetImportantDialogues() const;

    // ?≤Â?Â∞çË©±Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FString> GetDialogueHistory() const;

    // Ê∑ªÂ?Â∞çË©±?∞Ê≠∑??    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void AddDialogueToHistory(const FString& DialogueID);

    // Ê∏ÖÁ©∫Â∞çË©±Ê≠∑Âè≤
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void ClearDialogueHistory();

    // Ë®àÁ?Â∞çË©±ÂΩ±Èüø
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FDialogueOutcome> CalculateDialogueOutcomes(const FString& DialogueID, int32 OptionIndex) const;

    // ?âÁî®Â∞çË©±ÁµêÊ?
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void ApplyDialogueOutcomes(const TArray<FDialogueOutcome>& Outcomes);

    // ‰øùÂ?Â∞çË©±?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool SaveDialogueData(const FString& SaveSlotName);

    // ËºâÂÖ•Â∞çË©±?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool LoadDialogueData(const FString& SaveSlotName);

protected:
    // ?Ä?âÂ?Ë©±Êï∏??    UPROPERTY()
    TArray<FMingDialogue> AllDialogues;

    // Â∞çË©±ID?∞Â?Ë©±Á??†Â?
    UPROPERTY()
    TMap<FString, FMingDialogue> DialogueMap;

    // ËßíËâ≤ID?∞Â?Ë©±Â?Ë°®Á??†Â? - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<FString, TArray<FString>> CharacterDialogueMap;

    // ?∂Â?Â∞çË©±
    UPROPERTY()
    FMingDialogue CurrentDialogue;

    // ?∂Â??©ÂÆ∂ËßíËâ≤
    UPROPERTY()
    FMingCharacterData CurrentPlayerCharacter;

    // Â∞çË©±Ê≠∑Âè≤
    UPROPERTY()
    TArray<FString> DialogueHistory;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÂ?Ë©±Â∫´
    void InitializeDialogueLibrary();

    // ?µÂª∫Ê≠∑Âè≤‰∫∫Áâ©Â∞çË©±
    void CreateHistoricalCharacterDialogues();

    // ?µÂª∫Ëªç‰?Â∞çË©±
    void CreateMilitaryDialogues();

    // ?µÂª∫Â§ñ‰∫§Â∞çË©±
    void CreateDiplomaticDialogues();

    // ?µÂª∫?ã‰∫∫?Ö‰?Â∞çË©±
    void CreatePersonalStoryDialogues();

    // ?µÂª∫‰ªªÂ?Â∞çË©±
    void CreateQuestDialogues();

    // È©óË?Â∞çË©±Ê¢ù‰ª∂
    bool ValidateDialogueConditions(const TArray<FDialogueCondition>& Conditions, const FMingCharacterData& PlayerCharacter) const;

    // È©óË??ÆÂÄãÊ?‰ª?    bool ValidateSingleCondition(const FDialogueCondition& Condition, const FMingCharacterData& PlayerCharacter) const;

    // ?∑Ë?Â∞çË©±ÁµêÊ?
    void ExecuteDialogueOutcomes(const TArray<FDialogueOutcome>& Outcomes);

    // ?¥Êñ∞ËßíËâ≤?ú‰?
    void UpdateCharacterRelationship(const FString& CharacterID, float RelationshipChange);

    // Ëß∏Áôº‰ªªÂ?
    void TriggerQuest(const FString& QuestID);

    // Áµ¶‰??ÖÂ†±
    void GrantInformation(const FString& InformationID);

    // ?êÂ??Ä??    void ImproveSkill(const FName& SkillID, float ImprovementAmount);

    // Ëß∏Áôº‰∫ã‰ª∂
    void TriggerEvent(const FString& EventID);

    // ?üÊ?Â∞çË©±ID
    FString GenerateDialogueID(const FString& BaseName, const FString& CharacterID) const;

    // ?≤Â?ËßíËâ≤?ú‰???    float GetCharacterRelationship(const FString& CharacterID) const;

    // Ê™¢Êü•ËßíËâ≤?ØÂê¶Â∑≤Áü•
    bool IsCharacterKnown(const FString& CharacterID) const;

    // ?≤Â??©ÂÆ∂Â±¨ÊÄ?    float GetPlayerAttribute(const FString& AttributeName) const;

    // Ê™¢Êü•‰ªªÂ??Ä??    bool IsQuestCompleted(const FString& QuestID) const;

    // Ê™¢Êü•?Ä?ΩÁ?Á¥?    int32 GetSkillLevel(const FName& SkillID) const;
};
