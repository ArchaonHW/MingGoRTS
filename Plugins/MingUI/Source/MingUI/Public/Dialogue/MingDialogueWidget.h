#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingDialogueWidget.generated.h"

UENUM(BlueprintType)
enum class EMingDialogueDisplayMode : uint8
{
    Standard UMETA(DisplayName = "Standard"),
    Cinematic UMETA(DisplayName = "Cinematic"),
    Compact UMETA(DisplayName = "Compact"),
    Narrative UMETA(DisplayName = "Narrative")
};

UENUM(BlueprintType)
enum class EMingDialogueOptionType : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Persuasion UMETA(DisplayName = "Persuasion"),
    Intimidation UMETA(DisplayName = "Intimidation"),
    Bribe UMETA(DisplayName = "Bribe"),
    Relationship UMETA(DisplayName = "Relationship"),
    Quest UMETA(DisplayName = "Quest"),
    Trade UMETA(DisplayName = "Trade"),
    Information UMETA(DisplayName = "Information"),
    Exit UMETA(DisplayName = "Exit")
};

USTRUCT(BlueprintType)
struct FMingDialogueOption
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 OptionID;

    UPROPERTY(BlueprintReadWrite)
    FText OptionText;

    UPROPERTY(BlueprintReadWrite)
    EMingDialogueOptionType OptionType;

    UPROPERTY(BlueprintReadWrite)
    bool bIsAvailable;

    UPROPERTY(BlueprintReadWrite)
    FText AvailabilityReason;

    UPROPERTY(BlueprintReadWrite)
    float RelationshipRequirement;

    UPROPERTY(BlueprintReadWrite)
    float ReputationRequirement;

    UPROPERTY(BlueprintReadWrite)
    FName RequiredRegionID;

    UPROPERTY(BlueprintReadWrite)
    int32 RequiredRank;

    UPROPERTY(BlueprintReadWrite)
    bool bShowRelationshipHint;

    UPROPERTY(BlueprintReadWrite)
    bool bShowReputationHint;

    UPROPERTY(BlueprintReadWrite)
    bool bHasCustomPrice;

    UPROPERTY(BlueprintReadWrite)
    int32 CustomPrice;

    UPROPERTY(BlueprintReadWrite)
    FLinearColor OptionColor;

    UPROPERTY(BlueprintReadWrite)
    UTexture2D* OptionIcon;

    UPROPERTY(BlueprintReadWrite)
    float EstimatedSuccessChance;
};

USTRUCT(BlueprintType)
struct FMingDialogueLine
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName SpeakerID;

    UPROPERTY(BlueprintReadWrite)
    FString SpeakerName;

    UPROPERTY(BlueprintReadWrite)
    FText DialogueText;

    UPROPERTY(BlueprintReadWrite)
    float DisplayDuration;

    UPROPERTY(BlueprintReadWrite)
    bool bAutoAdvance;

    UPROPERTY(BlueprintReadWrite)
    float AutoAdvanceDelay;

    UPROPERTY(BlueprintReadWrite)
    UTexture2D* SpeakerPortrait;

    UPROPERTY(BlueprintReadWrite)
    FLinearColor SpeakerColor;

    UPROPERTY(BlueprintReadWrite)
    FName AssociatedAudioCue;
};

/**
 * Dialogue System Widget
 * Displays conversations with relationship and reputation integration
 */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API UMingDialogueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingDialogueWidget(const FObjectInitializer& ObjectInitializer};

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void InitializeDialogue(FName CharacterID, class UMingRelationshipManager* RelationshipManager};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void SetDialogueMode(EMingDialogueDisplayMode NewMode};

    // Dialogue Flow
    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void StartDialogue(const TArray<FMingDialogueLine>& Lines};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ShowNextLine(};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ShowPreviousLine(};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void SkipDialogue(};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void EndDialogue(};

    UFUNCTION(BlueprintPure, Category = "Dialogue UI")
    bool HasMoreLines() const;

    UFUNCTION(BlueprintPure, Category = "Dialogue UI")
    bool CanGoBack() const;

    // Options
    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ShowOptions(const TArray<FMingDialogueOption>& Options};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ClearOptions(};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void SelectOption(int32 OptionID};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void RefreshOptionAvailability(};

    // Relationship Integration
    UFUNCTION(BlueprintPure, Category = "Dialogue UI")
    float GetCurrentRelationshipValue() const;

    UFUNCTION(BlueprintPure, Category = "Dialogue UI")
    int32 GetCurrentReputationRank() const;

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ShowRelationshipImpactPreview(float ImpactValue};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ShowReputationImpactPreview(FName RegionID, float ImpactValue};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void HideImpactPreview(};

    // Option Hints
    UFUNCTION(BlueprintPure, Category = "Dialogue UI")
    FString GetOptionHintText(const FMingDialogueOption& Option) const;

    UFUNCTION(BlueprintPure, Category = "Dialogue UI")
    bool IsOptionAvailable(const FMingDialogueOption& Option) const;

    UFUNCTION(BlueprintPure, Category = "Dialogue UI")
    float CalculateSuccessChance(const FMingDialogueOption& Option) const;

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ShowOptionDetails(const FMingDialogueOption& Option};

    // Audio Integration
    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void PlayDialogueAudio(FName AudioCue};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void StopDialogueAudio(};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void SetAudioVolume(float Volume};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void PlayOptionSound(EMingDialogueOptionType OptionType};

    // Visual Effects
    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void PlayTextRevealAnimation(float Speed};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ShowSpeakerTransition(FName NewSpeakerID};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void ShakeDialogueBox(float Intensity, float Duration};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void FlashOption(int32 OptionID, FLinearColor FlashColor};

    // Cinematic Mode
    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void EnableCinematicMode(bool bEnable};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void SetLetterboxVisibility(bool bVisible};

    UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
    void FocusOnSpeaker(FName SpeakerID};

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueLineShown, int32, LineIndex};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueOptionSelected, int32, OptionID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipImpacted, FName, CharacterID, float, ImpactValue};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReputationImpacted, FName, RegionID, float, ImpactValue};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueStarted};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOptionHovered, int32, OptionID};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueLineShown OnDialogueLineShown;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueOptionSelected OnDialogueOptionSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRelationshipImpacted OnRelationshipImpacted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnReputationImpacted OnReputationImpacted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDialogueEnded OnDialogueEnded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnOptionHovered OnOptionHovered;

protected:
    // UI Components
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DialogueText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SpeakerNameText;

    UPROPERTY(meta = (BindWidget))
    class UImage* SpeakerPortrait;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* OptionsContainer;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* IndicatorContainer;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* TextRevealProgress;

    UPROPERTY(meta = (BindWidget))
    class UBorder* DialogueBox;

    UPROPERTY(meta = (BindWidget))
    class UImage* LetterboxTop;

    UPROPERTY(meta = (BindWidget))
    class UImage* LetterboxBottom;

    UPROPERTY(meta = (BindWidget))
    class UWidget* ImpactPreviewWidget;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ImpactPreviewText;

    UPROPERTY(meta = (BindWidget))
    class UImage* ContinueIndicator;

    // Data
    UPROPERTY()
    FName CurrentCharacterID;

    UPROPERTY()
    class UMingRelationshipManager* RelationshipManager;

    UPROPERTY()
    EMingDialogueDisplayMode CurrentMode;

    UPROPERTY()
    TArray<FMingDialogueLine> CurrentDialogueLines;

    UPROPERTY()
    int32 CurrentLineIndex;

    UPROPERTY()
    TArray<FMingDialogueOption> CurrentOptions;

    UPROPERTY()
    float CurrentRelationshipValue;

    UPROPERTY()
    int32 CurrentReputationRank;

    UPROPERTY()
    bool bIsTextRevealing;

    UPROPERTY()
    float TextRevealProgress;

    UPROPERTY()
    float AutoAdvanceTimer;

    UPROPERTY()
    TMap<int32, class UButton*> OptionButtons;

    // Button Handlers
    UFUNCTION()
    void OnContinueClicked(};

    UFUNCTION()
    void OnOptionButtonClicked(int32 OptionID};

    UFUNCTION()
    void OnOptionButtonHovered(int32 OptionID};

    UFUNCTION()
    void OnSkipClicked(};

    // Internal Functions
    void DisplayCurrentLine(};
    void UpdateSpeakerDisplay(const FMingDialogueLine& Line};
    void CreateOptionButton(const FMingDialogueOption& Option};
    void ClearOptionButtons(};
    void UpdateOptionAvailability(};
    void UpdateOptionVisuals(class UButton* Button, const FMingDialogueOption& Option};
    void ShowImpactPreviewInternal(float RelationshipDelta, float ReputationDelta};
    FLinearColor GetOptionTypeColor(EMingDialogueOptionType OptionType) const;
    FString GetOptionTypeIcon(EMingDialogueOptionType OptionType) const;
    void PlayTextReveal(float Speed};
    void CompleteTextReveal(};
    void UpdateAutoAdvance(float DeltaTime};

public:
    // Static utilities
    UFUNCTION(BlueprintPure, Category = "Dialogue UI", meta = (StaticFunction))
    static FText FormatDialogueText(const FText& RawText, const TMap<FString, FString>& Variables};

    UFUNCTION(BlueprintPure, Category = "Dialogue UI", meta = (StaticFunction))
    static float CalculateReadingTime(const FText& Text};

    UFUNCTION(BlueprintPure, Category = "Dialogue UI", meta = (StaticFunction))
    static FLinearColor GetTypeColor(EMingDialogueOptionType Type};
};

