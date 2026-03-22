#include "Dialogue/MingDialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "Engine/Texture2D.h"
#include "MingPersonal/Public/Relationship/MingRelationshipManager.h"
#include "MingPersonal/Public/Reputation/MingReputationManager.h"

UMingDialogueWidget::UMingDialogueWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CurrentLineIndex(0)
    , TextRevealProgress(0.0f)
    , AutoAdvanceTimer(0.0f)
    , bIsTextRevealing(false)
{
}

void UMingDialogueWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMingDialogueWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMingDialogueWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    if (bIsTextRevealing)
    {
        TextRevealProgress += InDeltaTime;
        if (TextRevealProgress >= 1.0f)
        {
            CompleteTextReveal();
        }
        else if (TextRevealProgressBar)
        {
            TextRevealProgressBar->SetPercent(TextRevealProgress);
        }
    }
    
    UpdateAutoAdvance(InDeltaTime);
}

void UMingDialogueWidget::InitializeDialogue(FName CharacterID, UMingRelationshipManager* InRelationshipManager)
{
    CurrentCharacterID = CharacterID;
    RelationshipManager = InRelationshipManager;
    
    if (RelationshipManager)
    {
        CurrentRelationshipValue = RelationshipManager->GetRelationshipValue(CharacterID);
        CurrentReputationRank = 0; // Get from reputation manager
    }
    
    OnDialogueStarted.Broadcast();
}

void UMingDialogueWidget::SetDialogueMode(EMingDialogueDisplayMode NewMode)
{
    CurrentMode = NewMode;
    
    // Update UI based on mode
    if (LetterboxTop && LetterboxBottom)
    {
        bool bShowLetterbox = (CurrentMode == EMingDialogueDisplayMode::Cinematic);
        LetterboxTop->SetVisibility(bShowLetterbox ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        LetterboxBottom->SetVisibility(bShowLetterbox ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
    
    if (DialogueBox)
    {
        // Adjust dialogue box style based on mode
    }
}

void UMingDialogueWidget::StartDialogue(const TArray<FMingDialogueLine>& Lines)
{
    CurrentDialogueLines = Lines;
    CurrentLineIndex = 0;
    
    DisplayCurrentLine();
}

void UMingDialogueWidget::ShowNextLine()
{
    if (CurrentLineIndex < CurrentDialogueLines.Num() - 1)
    {
        CurrentLineIndex++;
        DisplayCurrentLine();
    }
}

void UMingDialogueWidget::ShowPreviousLine()
{
    if (CurrentLineIndex > 0)
    {
        CurrentLineIndex--;
        DisplayCurrentLine();
    }
}

void UMingDialogueWidget::SkipDialogue()
{
    // Skip to the end of current dialogue or show options
    if (CurrentOptions.Num() > 0)
    {
        ShowOptions(CurrentOptions);
    }
    else
    {
        EndDialogue();
    }
}

void UMingDialogueWidget::EndDialogue()
{
    StopDialogueAudio();
    ClearOptions();
    OnDialogueEnded.Broadcast();
    RemoveFromParent();
}

bool UMingDialogueWidget::HasMoreLines() const
{
    return CurrentLineIndex < CurrentDialogueLines.Num() - 1;
}

bool UMingDialogueWidget::CanGoBack() const
{
    return CurrentLineIndex > 0;
}

void UMingDialogueWidget::ShowOptions(const TArray<FMingDialogueOption>& Options)
{
    CurrentOptions = Options;
    ClearOptionButtons();
    
    for (const FMingDialogueOption& Option : Options)
    {
        CreateOptionButton(Option);
    }
    
    UpdateOptionAvailability();
}

void UMingDialogueWidget::ClearOptions()
{
    ClearOptionButtons();
    CurrentOptions.Empty();
}

void UMingDialogueWidget::SelectOption(int32 OptionID)
{
    OnDialogueOptionSelected.Broadcast(OptionID);
    PlayOptionSound(EMingDialogueOptionType::Normal);
    
    // Find the option and apply its effects
    for (const FMingDialogueOption& Option : CurrentOptions)
    {
        if (Option.OptionID == OptionID)
        {
            // Apply relationship impact
            if (Option.bShowRelationshipHint && RelationshipManager)
            {
                float CurrentValue = RelationshipManager->GetRelationshipValue(CurrentCharacterID);
                OnRelationshipImpacted.Broadcast(CurrentCharacterID, CurrentValue);
            }
            break;
        }
    }
}

void UMingDialogueWidget::RefreshOptionAvailability()
{
    UpdateOptionAvailability();
}

float UMingDialogueWidget::GetCurrentRelationshipValue() const
{
    return CurrentRelationshipValue;
}

int32 UMingDialogueWidget::GetCurrentReputationRank() const
{
    return CurrentReputationRank;
}

void UMingDialogueWidget::ShowRelationshipImpactPreview(float ImpactValue)
{
    ShowImpactPreviewInternal(ImpactValue, 0.0f);
}

void UMingDialogueWidget::ShowReputationImpactPreview(FName RegionID, float ImpactValue)
{
    ShowImpactPreviewInternal(0.0f, ImpactValue);
}

void UMingDialogueWidget::HideImpactPreview()
{
    if (ImpactPreviewWidget)
    {
        ImpactPreviewWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

FString UMingDialogueWidget::GetOptionHintText(const FMingDialogueOption& Option) const
{
    if (!Option.bIsAvailable)
    {
        return Option.AvailabilityReason.ToString();
    }
    
    if (Option.bShowRelationshipHint && Option.RelationshipRequirement > 0)
    {
        return FString::Printf(TEXT("Requires %.0f relationship"), Option.RelationshipRequirement);
    }
    
    if (Option.bShowReputationHint && Option.RequiredRank > 0)
    {
        return FString::Printf(TEXT("Requires Rank %d"), Option.RequiredRank);
    }
    
    if (Option.bHasCustomPrice)
    {
        return FString::Printf(TEXT("Cost: %d"), Option.CustomPrice);
    }
    
    if (Option.EstimatedSuccessChance > 0)
    {
        return FString::Printf(TEXT("Success: %.0f%%"), Option.EstimatedSuccessChance * 100);
    }
    
    return TEXT("");
}

bool UMingDialogueWidget::IsOptionAvailable(const FMingDialogueOption& Option) const
{
    // Check relationship requirement
    if (Option.RelationshipRequirement > 0 && CurrentRelationshipValue < Option.RelationshipRequirement)
    {
        return false;
    }
    
    // Check reputation rank requirement
    if (Option.RequiredRank > 0 && CurrentReputationRank < Option.RequiredRank)
    {
        return false;
    }
    
    return true;
}

float UMingDialogueWidget::CalculateSuccessChance(const FMingDialogueOption& Option) const
{
    float BaseChance = 0.5f;
    
    // Adjust based on relationship
    if (Option.RelationshipRequirement > 0)
    {
        float RelationshipBonus = (CurrentRelationshipValue - Option.RelationshipRequirement) / 100.0f;
        BaseChance += RelationshipBonus * 0.3f;
    }
    
    // Adjust based on option type
    switch (Option.OptionType)
    {
    case EMingDialogueOptionType::Persuasion:
        BaseChance += 0.1f;
        break;
    case EMingDialogueOptionType::Intimidation:
        if (CurrentRelationshipValue < 30.0f)
        {
            BaseChance += 0.2f;
        }
        else
        {
            BaseChance -= 0.1f;
        }
        break;
    default:
        break;
    }
    
    return FMath::Clamp(BaseChance, 0.0f, 1.0f);
}

void UMingDialogueWidget::ShowOptionDetails(const FMingDialogueOption& Option)
{
    // Show detailed information about the option
    FString Details = FString::Printf(
        TEXT("%s\n\nType: %s\nSuccess Chance: %.0f%%\n"),
        *Option.OptionText.ToString(),
        *GetOptionTypeIcon(Option.OptionType),
        CalculateSuccessChance(Option) * 100
    );
    
    if (Option.bShowRelationshipHint)
    {
        Details += FString::Printf(TEXT("Required Relationship: %.0f\n"), Option.RelationshipRequirement);
    }
    
    if (Option.bHasCustomPrice)
    {
        Details += FString::Printf(TEXT("Cost: %d\n"), Option.CustomPrice);
    }
    
    // Display in a tooltip or modal
}

void UMingDialogueWidget::PlayDialogueAudio(FName AudioCue)
{
    // Play audio using audio system
}

void UMingDialogueWidget::StopDialogueAudio()
{
    // Stop playing audio
}

void UMingDialogueWidget::SetAudioVolume(float Volume)
{
    // Set audio volume
}

void UMingDialogueWidget::PlayOptionSound(EMingDialogueOptionType OptionType)
{
    // Play sound based on option type
}

void UMingDialogueWidget::PlayTextRevealAnimation(float Speed)
{
    bIsTextRevealing = true;
    TextRevealProgress = 0.0f;
}

void UMingDialogueWidget::ShowSpeakerTransition(FName NewSpeakerID)
{
    // Animate transition to new speaker
}

void UMingDialogueWidget::ShakeDialogueBox(float Intensity, float Duration)
{
    // Shake the dialogue box
}

void UMingDialogueWidget::FlashOption(int32 OptionID, FLinearColor FlashColor)
{
    // Flash the option button
    if (OptionButtons.Contains(OptionID))
    {
        UButton* Button = OptionButtons[OptionID];
        // Apply flash effect
    }
}

void UMingDialogueWidget::EnableCinematicMode(bool bEnable)
{
    if (bEnable)
    {
        SetDialogueMode(EMingDialogueDisplayMode::Cinematic);
    }
    else
    {
        SetDialogueMode(EMingDialogueDisplayMode::Standard);
    }
}

void UMingDialogueWidget::SetLetterboxVisibility(bool bVisible)
{
    if (LetterboxTop)
    {
        LetterboxTop->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
    if (LetterboxBottom)
    {
        LetterboxBottom->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

void UMingDialogueWidget::FocusOnSpeaker(FName SpeakerID)
{
    // Focus camera or UI on speaker
}

// Button Handlers

void UMingDialogueWidget::OnContinueClicked()
{
    ShowNextLine();
}

void UMingDialogueWidget::OnOptionButtonClicked(int32 OptionID)
{
    SelectOption(OptionID);
}

void UMingDialogueWidget::OnOptionButtonHovered(int32 OptionID)
{
    OnOptionHovered.Broadcast(OptionID);
}

void UMingDialogueWidget::OnSkipClicked()
{
    SkipDialogue();
}

// Internal Functions

void UMingDialogueWidget::DisplayCurrentLine()
{
    if (CurrentDialogueLines.IsValidIndex(CurrentLineIndex))
    {
        FMingDialogueLine& Line = CurrentDialogueLines[CurrentLineIndex];
        UpdateSpeakerDisplay(Line);
        
        if (DialogueText)
        {
            DialogueText->SetText(Line.DialogueText);
        }
        
        // Play text reveal animation
        PlayTextRevealAnimation(1.0f / Line.DisplayDuration);
        
        // Play audio if available
        if (!Line.AssociatedAudioCue.IsNone())
        {
            PlayDialogueAudio(Line.AssociatedAudioCue);
        }
        
        // Set up auto advance
        if (Line.bAutoAdvance)
        {
            AutoAdvanceTimer = Line.AutoAdvanceDelay;
        }
        
        OnDialogueLineShown.Broadcast(CurrentLineIndex);
    }
}

void UMingDialogueWidget::UpdateSpeakerDisplay(const FMingDialogueLine& Line)
{
    if (SpeakerNameText)
    {
        SpeakerNameText->SetText(FText::FromString(Line.SpeakerName));
    }
    
    if (SpeakerPortrait && Line.SpeakerPortrait)
    {
        SpeakerPortrait->SetBrushFromTexture(Line.SpeakerPortrait);
    }
}

void UMingDialogueWidget::CreateOptionButton(const FMingDialogueOption& Option)
{
    if (!OptionsContainer)
    {
        return;
    }
    
    UButton* OptionButton = NewObject<UButton>(this);
    if (OptionButton)
    {
        // Create button content
        UHorizontalBox* ButtonContent = NewObject<UHorizontalBox>(this);
        
        // Option text
        UTextBlock* OptionText = NewObject<UTextBlock>(this);
        OptionText->SetText(Option.OptionText);
        ButtonContent->AddChildToHorizontalBox(OptionText);
        
        // Hint text (if available)
        FString Hint = GetOptionHintText(Option);
        if (!Hint.IsEmpty())
        {
            UTextBlock* HintText = NewObject<UTextBlock>(this);
            HintText->SetText(FText::FromString(TEXT("(") + Hint + TEXT(")")));
            HintText->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
            ButtonContent->AddChildToHorizontalBox(HintText);
        }
        
        // Bind events
        FScriptDelegate ClickDelegate;
        ClickDelegate.BindUFunction(this, FName("OnOptionButtonClicked"), Option.OptionID);
        OptionButton->OnClicked.Add(ClickDelegate);
        
        FScriptDelegate HoverDelegate;
        HoverDelegate.BindUFunction(this, FName("OnOptionButtonHovered"), Option.OptionID);
        OptionButton->OnHovered.Add(HoverDelegate);
        
        // Store reference
        OptionButtons.Add(Option.OptionID, OptionButton);
        
        // Add to container
        OptionsContainer->AddChildToVerticalBox(OptionButton);
        
        // Update visual state
        UpdateOptionVisuals(OptionButton, Option);
    }
}

void UMingDialogueWidget::ClearOptionButtons()
{
    if (OptionsContainer)
    {
        OptionsContainer->ClearChildren();
    }
    OptionButtons.Empty();
}

void UMingDialogueWidget::UpdateOptionAvailability()
{
    for (const FMingDialogueOption& Option : CurrentOptions)
    {
        if (OptionButtons.Contains(Option.OptionID))
        {
            UButton* Button = OptionButtons[Option.OptionID];
            bool bIsAvailable = IsOptionAvailable(Option);
            Button->SetIsEnabled(bIsAvailable);
            UpdateOptionVisuals(Button, Option);
        }
    }
}

void UMingDialogueWidget::UpdateOptionVisuals(UButton* Button, const FMingDialogueOption& Option)
{
    if (!Button)
    {
        return;
    }
    
    // Set color based on option type
    FLinearColor TypeColor = GetOptionTypeColor(Option.OptionType);
    
    // Adjust for availability
    if (!Option.bIsAvailable)
    {
        TypeColor *= 0.5f; // Darken if unavailable
    }
    
    // Apply to button
    FSlateColor SlateColor(TypeColor);
    // Button style updates would go here
}

void UMingDialogueWidget::ShowImpactPreviewInternal(float RelationshipDelta, float ReputationDelta)
{
    if (!ImpactPreviewWidget || !ImpactPreviewText)
    {
        return;
    }
    
    FString PreviewText;
    
    if (RelationshipDelta != 0.0f)
    {
        FString Sign = RelationshipDelta > 0 ? TEXT("+") : TEXT("");
        PreviewText += FString::Printf(TEXT("Relationship: %s%.0f\n"), *Sign, RelationshipDelta);
    }
    
    if (ReputationDelta != 0.0f)
    {
        FString Sign = ReputationDelta > 0 ? TEXT("+") : TEXT("");
        PreviewText += FString::Printf(TEXT("Reputation: %s%.0f"), *Sign, ReputationDelta);
    }
    
    ImpactPreviewText->SetText(FText::FromString(PreviewText));
    ImpactPreviewWidget->SetVisibility(ESlateVisibility::Visible);
}

FLinearColor UMingDialogueWidget::GetOptionTypeColor(EMingDialogueOptionType OptionType) const
{
    switch (OptionType)
    {
    case EMingDialogueOptionType::Persuasion:
        return FLinearColor(0.2f, 0.6f, 1.0f); // Blue
    case EMingDialogueOptionType::Intimidation:
        return FLinearColor(0.8f, 0.2f, 0.2f); // Red
    case EMingDialogueOptionType::Bribe:
        return FLinearColor(0.8f, 0.6f, 0.2f); // Gold
    case EMingDialogueOptionType::Quest:
        return FLinearColor(0.2f, 0.8f, 0.4f); // Green
    case EMingDialogueOptionType::Trade:
        return FLinearColor(0.6f, 0.4f, 0.8f); // Purple
    case EMingDialogueOptionType::Exit:
        return FLinearColor(0.5f, 0.5f, 0.5f); // Gray
    default:
        return FLinearColor(1.0f, 1.0f, 1.0f); // White
    }
}

FString UMingDialogueWidget::GetOptionTypeIcon(EMingDialogueOptionType OptionType) const
{
    switch (OptionType)
    {
    case EMingDialogueOptionType::Persuasion:
        return TEXT("🗣️");
    case EMingDialogueOptionType::Intimidation:
        return TEXT("⚔️");
    case EMingDialogueOptionType::Bribe:
        return TEXT("💰");
    case EMingDialogueOptionType::Relationship:
        return TEXT("🤝");
    case EMingDialogueOptionType::Quest:
        return TEXT("📜");
    case EMingDialogueOptionType::Trade:
        return TEXT("⚖️");
    case EMingDialogueOptionType::Information:
        return TEXT("ℹ️");
    case EMingDialogueOptionType::Exit:
        return TEXT("🚪");
    default:
        return TEXT("💬");
    }
}

void UMingDialogueWidget::PlayTextReveal(float Speed)
{
    bIsTextRevealing = true;
    TextRevealProgress = 0.0f;
}

void UMingDialogueWidget::CompleteTextReveal()
{
    bIsTextRevealing = false;
    TextRevealProgress = 1.0f;
    
    if (TextRevealProgressBar)
    {
        TextRevealProgressBar->SetPercent(1.0f);
    }
}

void UMingDialogueWidget::UpdateAutoAdvance(float DeltaTime)
{
    if (AutoAdvanceTimer > 0.0f)
    {
        AutoAdvanceTimer -= DeltaTime;
        if (AutoAdvanceTimer <= 0.0f)
        {
            ShowNextLine();
        }
    }
}

// Static Utilities

FText UMingDialogueWidget::FormatDialogueText(const FText& RawText, const TMap<FString, FString>& Variables)
{
    FString TextString = RawText.ToString();
    
    for (const auto& Variable : Variables)
    {
        FString Placeholder = TEXT("{") + Variable.Key + TEXT("}");
        TextString = TextString.Replace(*Placeholder, *Variable.Value);
    }
    
    return FText::FromString(TextString);
}

float UMingDialogueWidget::CalculateReadingTime(const FText& Text)
{
    // Average reading speed: 200 words per minute
    // Approximately 3 characters per word in Chinese/English mix
    float CharacterCount = Text.ToString().Len();
    float Words = CharacterCount / 3.0f;
    float Seconds = (Words / 200.0f) * 60.0f;
    
    // Minimum 2 seconds
    return FMath::Max(Seconds, 2.0f);
}

FLinearColor UMingDialogueWidget::GetTypeColor(EMingDialogueOptionType Type)
{
    switch (Type)
    {
    case EMingDialogueOptionType::Persuasion:
        return FLinearColor(0.2f, 0.6f, 1.0f);
    case EMingDialogueOptionType::Intimidation:
        return FLinearColor(0.8f, 0.2f, 0.2f);
    case EMingDialogueOptionType::Bribe:
        return FLinearColor(0.8f, 0.6f, 0.2f);
    case EMingDialogueOptionType::Quest:
        return FLinearColor(0.2f, 0.8f, 0.4f);
    case EMingDialogueOptionType::Trade:
        return FLinearColor(0.6f, 0.4f, 0.8f);
    case EMingDialogueOptionType::Exit:
        return FLinearColor(0.5f, 0.5f, 0.5f);
    default:
        return FLinearColor(1.0f, 1.0f, 1.0f);
    }
}
