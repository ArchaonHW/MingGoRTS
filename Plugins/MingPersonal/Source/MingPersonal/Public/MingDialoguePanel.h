#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingRelationshipManager.h"
#include "MingDialoguePanel.generated.h"

UENUM(BlueprintType)
enum class EDialogueState : uint8
{
    Idle,           // Á©∫È?
    Speaking,       // Ë™™Ë©±‰∏?    Listening,      // ?ΩË©±‰∏?    Choosing,       // ?∏Ê?‰∏?    Finished        // ÂÆåÊ?
};

USTRUCT(BlueprintType)
struct FDialoguePanelOption
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Option")
    FString OptionText;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Option")
    int32 OptionID;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Option")
    bool bIsAvailable;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Option")
    FString RequirementText;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Option")
    FLinearColor OptionColor;

    FDialoguePanelOption()
    {
        OptionText = TEXT("");
        OptionID = 0;
        bIsAvailable = true;
        RequirementText = TEXT("");
        OptionColor = FLinearColor::White;
    }
};

USTRUCT(BlueprintType)
struct FDialogueData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    FString CharacterName;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    FString DialogueText;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    EDialogueType DialogueType;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    ERelationshipType RelationshipType;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    FLinearColor DialogueColor;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    TArray<FDialoguePanelOption> AvailableOptions;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    float EmotionalIntensity;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    bool bIsImportant;

    FDialogueData()
    {
        CharacterID = TEXT("");
        CharacterName = TEXT("");
        DialogueText = TEXT("");
        DialogueType = EDialogueAudioType::Neutral;
        RelationshipType = ERelationshipType::Neutral;
        DialogueColor = FLinearColor::White;
        EmotionalIntensity = 1.0f;
        bIsImportant = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStarted, const FString&, CharacterID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueFinished, const FString&, CharacterID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueOptionSelected, int32, OptionID, const FString&, OptionText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStateChanged, EDialogueState, NewState);

/**
 * Â∞çË©±?¢ÊùøWidget
 * ÁÆ°Á?ËßíËâ≤Â∞çË©±?åÈù¢
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingDialoguePanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingDialoguePanel(const FObjectInitializer& ObjectInitializer);

    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void InitializeDialoguePanel(UMingRelationshipManager* InRelationshipManager);

    // Â∞çË©±?ßÂà∂
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void StartDialogue(const FString& CharacterID, const FString& DialogueText, EDialogueType DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void EndDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ContinueDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void PauseDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ResumeDialogue();

    // ?∏È??ïÁ?
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ShowDialogueOptions(const TArray<FDialoguePanelOption>& Options);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void HideDialogueOptions();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SelectDialogueOption(int32 OptionID);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void OnOptionClicked(int32 OptionID);

    // Â∞çË©±?ßÂÆπ
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SetDialogueText(const FString& Text);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void AddDialogueText(const FString& Text);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ClearDialogueText();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SetCharacterName(const FString& Name);

    // ?Ä?ãÊü•Ë©?    UFUNCTION(BlueprintPure, Category = "Dialogue Panel")
    bool IsDialogueActive() const;

    UFUNCTION(BlueprintPure, Category = "Dialogue Panel")
    EDialogueState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintPure, Category = "Dialogue Panel")
    FString GetCurrentCharacterID() const { return CurrentCharacterID; }

    UFUNCTION(BlueprintPure, Category = "Dialogue Panel")
    bool AreOptionsVisible() const { return bOptionsVisible; }

    // Ë¶ñË¶∫?àÊ?
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void PlayTypewriterEffect(const FString& Text, float TypingSpeed = 0.05f);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void StopTypewriterEffect();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ShowCharacterPortrait(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void HideCharacterPortrait();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SetDialogueEmotion(EDialogueType DialogueType);

    // ?≥È†ª?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void PlayDialogueAudio(EDialogueType DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void StopDialogueAudio();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SetAudioVolume(float Volume);

    // Ê≠∑Âè≤Ë®òÈ?
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void AddToHistory(const FString& CharacterID, const FString& Text);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ShowDialogueHistory();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void HideDialogueHistory();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ClearDialogueHistory();

    // Âø´Êç∑?ç‰?
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void QuickResponse(const FString& ResponseText);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SkipDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void AutoAdvance(bool bEnabled);

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueFinished OnDialogueFinished;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueOptionSelected OnDialogueOptionSelected;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueStateChanged OnDialogueStateChanged;

protected:
    // Á≥ªÁµ±ÂºïÁî®
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // Â∞çË©±?∏Ê?
    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    FDialogueData CurrentDialogue;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    TArray<FDialoguePanelOption> CurrentOptions;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    TArray<FString> DialogueHistory;

    // ?Ä??    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    EDialogueState CurrentState;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    FString CurrentCharacterID;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    bool bOptionsVisible = false;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    bool bAutoAdvance = false;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    bool bTypewriterActive = false;

    // Ë®≠ÁΩÆ
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Settings")
    float TypewriterSpeed = 0.05f;

    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Settings")
    float AutoAdvanceDelay = 3.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Settings")
    float AudioVolume = 1.0f;

    // UIÁµÑ‰ª∂ÂºïÁî®ÔºàÈ?Ë¶ÅÂú®Blueprint‰∏≠Á?ÂÆöÔ?
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CharacterNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DialogueText;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* OptionsContainer;

    UPROPERTY(meta = (BindWidget))
    class UImage* CharacterPortrait;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* HistoryScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UButton* ContinueButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SkipButton;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* TypewriterProgress;

    // ?õÊì¨?ΩÊï∏
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ?ßÈÉ®?ΩÊï∏
    void UpdateDialogueUI();
    void UpdateCharacterUI();
    void UpdateOptionsUI();
    void SetDialogueState(EDialogueState NewState);
    FLinearColor GetDialogueColor(EDialogueType DialogueType, ERelationshipType RelationshipType) const;
    FString GetCharacterDisplayName(const FString& CharacterID) const;
    void ProcessTypewriterEffect();
    void OnAutoAdvanceTimer();

    // ‰∫ã‰ª∂Á∂ÅÂ?
    void SetupEventBindings();
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);

private:
    // ËºîÂä©?ΩÊï∏
    void CreateOptionWidgets();
    void ClearOptionWidgets();
    bool IsOptionAvailable(const FDialoguePanelOption& Option) const;
    void PlayTypewriterSound();
    void StopTypewriterSound();

    // ÂÆöÊ???    FTimerHandle TypewriterTimer;
    FTimerHandle AutoAdvanceTimer;

    // ?ìÂ?Ê©üÊ???    FString TargetText;
    FString CurrentText;
    int32 TypewriterIndex;
};
