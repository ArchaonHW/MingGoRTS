#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDialogueSystem.generated.h"

UENUM(BlueprintType)
enum class EMingDialogueSpeaker : uint8
{
    Player UMETA(DisplayName = "Player"),
    NPC UMETA(DisplayName = "NPC"),
    System UMETA(DisplayName = "System"),
    Narrator UMETA(DisplayName = "Narrator")
};

UENUM(BlueprintType)
enum class EMingDialogueType : uint8
{
    Conversation UMETA(DisplayName = "Conversation"),
    Briefing UMETA(DisplayName = "Briefing"),
    Debriefing UMETA(DisplayName = "Debriefing"),
    Historical UMETA(DisplayName = "Historical"),
    Choice UMETA(DisplayName = "Choice"),
    Monologue UMETA(DisplayName = "Monologue")
};

USTRUCT(BlueprintType)
struct FMingDialogueOption
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NextDialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RequiredConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Consequences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority;

    FMingDialogueOption()
        : bIsEnabled(true)
        , Priority(0)
    {}
};

USTRUCT(BlueprintType)
struct FMingDialogueLine
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SpeakerName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingDialogueSpeaker SpeakerType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DialogueText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString AudioPath;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString PortraitImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DisplayDuration;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMingDialogueOption> Options;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString AutoNextDialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bSkippable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsChoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> TriggerEvents;

    FMingDialogueLine()
        : SpeakerType(EMingDialogueSpeaker::NPC)
        , DisplayDuration(3.0f)
        , bSkippable(true)
        , bIsChoice(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingDialogueNode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString NodeID;

    UPROPERTY(BlueprintReadOnly)
    FMingDialogueLine DialogueLine;

    UPROPERTY(BlueprintReadOnly)
    TArray<FMingDialogueNode> ChildNodes;

    UPROPERTY(BlueprintReadOnly)
    bool bVisited;

    FMingDialogueNode()
        : bVisited(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingDialogueConversation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ConversationID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ConversationName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingDialogueType DialogueType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString StartDialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FString, FMingDialogueLine> DialogueLines;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> RequiredConditions;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bRepeatable;

    UPROPERTY(BlueprintReadOnly)
    bool bHasBeenCompleted;

    FMingDialogueConversation()
        : DialogueType(EMingDialogueType::Conversation)
        , bRepeatable(false)
        , bHasBeenCompleted(false)
    {}
};

/**
 * Dialogue System for MingGoRTS
 * Manages conversations, choices, and narrative elements
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGPERSONAL_API UMingDialogueSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingDialogueSystem();

    // Conversation Management
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void StartConversation(const FString& ConversationID);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void EndConversation();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void PauseConversation();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void ResumeConversation();

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    bool IsConversationActive() const;

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    FString GetCurrentConversationID() const;

    // Dialogue Navigation
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SelectDialogueOption(int32 OptionIndex);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void NextDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SkipDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void GoToDialogue(const FString& DialogueID);

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    FMingDialogueLine GetCurrentDialogueLine() const;

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    TArray<FMingDialogueOption> GetCurrentOptions() const;

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    bool HasCurrentDialogue() const;

    // Conversation Loading
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void LoadConversation(const FMingDialogueConversation& Conversation);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void LoadDialogueFromFile(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void AddDialogueLine(const FString& ConversationID, const FMingDialogueLine& DialogueLine);

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    TArray<FMingDialogueConversation> GetAllConversations() const;

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    TArray<FMingDialogueConversation> GetAvailableConversations() const;

    // Historical Context
    UFUNCTION(BlueprintCallable, Category = "Historical")
    void ShowHistoricalContext(const FString& EventID);

    UFUNCTION(BlueprintCallable, Category = "Historical")
    void AddHistoricalNote(const FString& DialogueID, const FString& Note);

    UFUNCTION(BlueprintPure, Category = "Historical")
    TArray<FString> GetHistoricalNotes(const FString& DialogueID) const;

    // Choices and Consequences
    UFUNCTION(BlueprintCallable, Category = "Choices")
    void ProcessChoice(const FString& ChoiceID);

    UFUNCTION(BlueprintCallable, Category = "Choices")
    void RecordPlayerChoice(const FString& DialogueID, int32 OptionIndex);

    UFUNCTION(BlueprintPure, Category = "Choices")
    TMap<FString, int32> GetPlayerChoices() const;

    UFUNCTION(BlueprintPure, Category = "Choices")
    TArray<FString> GetChoiceConsequences(const FString& DialogueID, int32 OptionIndex) const;

    // Conditions and Triggers
    UFUNCTION(BlueprintCallable, Category = "Conditions")
    void SetDialogueCondition(const FString& ConditionName, bool bValue);

    UFUNCTION(BlueprintPure, Category = "Conditions")
    bool GetDialogueCondition(const FString& ConditionName) const;

    UFUNCTION(BlueprintCallable, Category = "Conditions")
    void TriggerDialogueEvent(const FString& EventName);

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueStarted, const FMingDialogueLine&, DialogueLine, const FString&, ConversationID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueEnded, const FString&, ConversationID, bool, bWasCompleted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueOptionSelected, int32, OptionIndex, const FString&, DialogueID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHistoricalContextShown, const FString&, EventID);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueEnded OnDialogueEnded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueOptionSelected OnDialogueOptionSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHistoricalContextShown OnHistoricalContextShown;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetSpeakerTypeName(EMingDialogueSpeaker SpeakerType);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetDialogueTypeName(EMingDialogueType DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveDialogueData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadDialogueData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingDialogueConversation> Conversations;

    UPROPERTY()
    FString CurrentConversationID;

    UPROPERTY()
    FString CurrentDialogueID;

    UPROPERTY()
    TMap<FString, bool> DialogueConditions;

    UPROPERTY()
    TMap<FString, int32> PlayerChoices;

    // 注意：TArray 不能直接作為 TMap 的 UPROPERTY 值類型
    // TMap<FString, TArray<FString>> HistoricalNotes;

    UPROPERTY()
    bool bIsConversationActive;

    UPROPERTY()
    bool bIsPaused;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal functions
    void InitializeDefaultDialogues();
    void SetupHistoricalDialogues();
    void SetupTutorialDialogues();
    void ProcessDialogueTriggers(const FMingDialogueLine& DialogueLine);
    void ProcessChoiceConsequences(const FMingDialogueOption& Option);
    bool CheckDialogueConditions(const TArray<FString>& Conditions) const;
    void UpdateDialogueOptions();
    void AutoAdvanceDialogue();

    // Helpers
    FMingDialogueLine* FindDialogueLine(const FString& DialogueID);
    FMingDialogueConversation* FindConversation(const FString& ConversationID);
    void MarkDialogueAsVisited(const FString& DialogueID);
};
