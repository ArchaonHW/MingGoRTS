#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingRelationshipManager.h"
#include "MingDialoguePanel.generated.h"

UENUM(BlueprintType)
enum class EDialogueState : uint8
{
    Idle,           // 空??
    Speaking,       // 說話??    Listening,      // ??話??    Choosing,       // ??????    Finished        // 完??
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStarted, const FString&, CharacterID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueFinished, const FString&, CharacterID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueOptionSelected, int32, OptionID, const FString&, OptionText};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStateChanged, EDialogueState, NewState);

/**
 * 對話??板Widget
 * 管??角色對話??面
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingDialoguePanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingDialoguePanel(const FObjectInitializer& ObjectInitializer};

    // ???X    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void InitializeDialoguePanel(UMingRelationshipManager* InRelationshipManager);

    // 對話??制
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

    // ???X???
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ShowDialogueOptions(const TArray<FDialoguePanelOption>& Options);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void HideDialogueOptions();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SelectDialogueOption(int32 OptionID);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void OnOptionClicked(int32 OptionID);

    // 對話??容
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SetDialogueText(const FString& Text);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void AddDialogueText(const FString& Text);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ClearDialogueText();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SetCharacterName(const FString& Name);

    // ????查??    UFUNCTION(BlueprintPure, Category = "Dialogue Panel")
    bool IsDialogueActive() const;

    UFUNCTION(BlueprintPure, Category = "Dialogue Panel")
    EDialogueState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintPure, Category = "Dialogue Panel")
    FString GetCurrentCharacterID() const { return CurrentCharacterID; }

    UFUNCTION(BlueprintPure, Category = "Dialogue Panel")
    bool AreOptionsVisible() const { return bOptionsVisible; }

    // 視覺????
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

    // ??頻????
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void PlayDialogueAudio(EDialogueType DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void StopDialogueAudio();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SetAudioVolume(float Volume);

    // 歷史記??
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void AddToHistory(const FString& CharacterID, const FString& Text);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ShowDialogueHistory();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void HideDialogueHistory();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void ClearDialogueHistory();

    // 快捷????
    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void QuickResponse(const FString& ResponseText);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void SkipDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue Panel")
    void AutoAdvance(bool bEnabled);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueFinished OnDialogueFinished;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueOptionSelected OnDialogueOptionSelected;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueStateChanged OnDialogueStateChanged;

protected:
    // 系統引用
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // 對話????
    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    FDialogueData CurrentDialogue;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    TArray<FDialoguePanelOption> CurrentOptions;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue Data")
    TArray<FString> DialogueHistory;

    // ??X    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    EDialogueState CurrentState;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    FString CurrentCharacterID;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    bool bOptionsVisible = false;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    bool bAutoAdvance = false;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    bool bTypewriterActive = false;

    // 設置
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Settings")
    float TypewriterSpeed = 0.05f;

    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Settings")
    float AutoAdvanceDelay = 3.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Settings")
    float AudioVolume = 1.0f;

    // UI組件引用（??要在Blueprint中??定??
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

    // ??擬??數
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ??部??數
    void UpdateDialogueUI();
    void UpdateCharacterUI();
    void UpdateOptionsUI();
    void SetDialogueState(EDialogueState NewState);
    FLinearColor GetDialogueColor(EDialogueType DialogueType, ERelationshipType RelationshipType) const;
    FString GetCharacterDisplayName(const FString& CharacterID) const;
    void ProcessTypewriterEffect();
    void OnAutoAdvanceTimer();

    // 事件綁??
    void SetupEventBindings();
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason};

private:
    // 輔助??數
    void CreateOptionWidgets();
    void ClearOptionWidgets();
    bool IsOptionAvailable(const FDialoguePanelOption& Option) const;
    void PlayTypewriterSound();
    void StopTypewriterSound();

    // 定?X    FTimerHandle TypewriterTimer;
    FTimerHandle AutoAdvanceTimer;

    // ????機?X    FString TargetText;
    FString CurrentText;
    int32 TypewriterIndex;
);

