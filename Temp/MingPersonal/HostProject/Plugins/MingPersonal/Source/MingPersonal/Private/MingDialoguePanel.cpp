#include "MingDialoguePanel.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMingDialoguePanel::UMingDialoguePanel(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CurrentState(EDialogueState::Idle)
    , bOptionsVisible(false)
    , bAutoAdvance(false)
    , bTypewriterActive(false)
    , TypewriterSpeed(0.05f)
    , AutoAdvanceDelay(3.0f)
    , AudioVolume(1.0f)
    , CharacterNameText(nullptr)
    , DialogueText(nullptr)
    , OptionsContainer(nullptr)
    , CharacterPortrait(nullptr)
    , HistoryScrollBox(nullptr)
    , ContinueButton(nullptr)
    , SkipButton(nullptr)
    , TypewriterProgress(nullptr)
    , TypewriterIndex(0)
{
}

void UMingDialoguePanel::NativeConstruct()
{
    Super::NativeConstruct();

    SetDialogueState(EDialogueState::Idle);
    SetupEventBindings();
    UpdateDialogueUI();
}

void UMingDialoguePanel::NativeDestruct()
{
    // 清理定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
        GetWorld()->GetTimerManager().ClearTimer(AutoAdvanceTimer);
    }

    Super::NativeDestruct();
}

void UMingDialoguePanel::InitializeDialoguePanel(UMingRelationshipManager* InRelationshipManager)
{
    RelationshipManager = InRelationshipManager;
    
    if (RelationshipManager)
    {
        UE_LOG(LogTemp, Log, TEXT("Dialogue panel initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Relationship manager is null - panel initialization failed"));
    }
}

void UMingDialoguePanel::StartDialogue(const FString& CharacterID, const FString& DialogueText, EDialogueType DialogueType)
{
    CurrentCharacterID = CharacterID;
    
    // 獲取角色關係信息
    ERelationshipType RelationshipType = ERelationshipType::Neutral;
    if (RelationshipManager)
    {
        FRelationshipData RelationshipData = RelationshipManager->GetRelationship(CharacterID);
        RelationshipType = RelationshipData.RelationshipType;
    }
    
    // 設置對話數據
    CurrentDialogue.CharacterID = CharacterID;
    CurrentDialogue.CharacterName = GetCharacterDisplayName(CharacterID);
    CurrentDialogue.DialogueText = DialogueText;
    CurrentDialogue.DialogueType = DialogueType;
    CurrentDialogue.RelationshipType = RelationshipType;
    CurrentDialogue.DialogueColor = GetDialogueColor(DialogueType, RelationshipType);
    CurrentDialogue.EmotionalIntensity = 1.0f;
    CurrentDialogue.bIsImportant = false;
    
    // 更新UI
    UpdateCharacterUI();
    SetDialogueState(EDialogueState::Speaking);
    
    // 播放打字機效果
    PlayTypewriterEffect(DialogueText, TypewriterSpeed);
    
    // 播放音頻
    PlayDialogueAudio(DialogueType);
    
    // 添加到歷史
    AddToHistory(CharacterID, DialogueText);
    
    // 觸發事件
    OnDialogueStarted.Broadcast(CharacterID);
    
    UE_LOG(LogTemp, Log, TEXT("Started dialogue with %s: %s"), *CharacterID, *DialogueText);
}

void UMingDialoguePanel::EndDialogue()
{
    SetDialogueState(EDialogueState::Finished);
    
    // 停止所有效果
    StopTypewriterEffect();
    StopDialogueAudio();
    HideDialogueOptions();
    
    // 清空當前對話
    CurrentDialogue = FDialogueData();
    CurrentCharacterID.Empty();
    
    // 觸發事件
    OnDialogueFinished.Broadcast(CurrentCharacterID);
    
    UE_LOG(LogTemp, Log, TEXT("Ended dialogue"));
}

void UMingDialoguePanel::ContinueDialogue()
{
    if (CurrentState == EDialogueState::Speaking)
    {
        // 如果正在打字，立即完成
        if (bTypewriterActive)
        {
            StopTypewriterEffect();
            SetDialogueText(TargetText);
        }
        else
        {
            // 顯示選項或結束對話
            if (CurrentOptions.Num() > 0)
            {
                ShowDialogueOptions(CurrentOptions);
            }
            else
            {
                EndDialogue();
            }
        }
    }
}

void UMingDialoguePanel::PauseDialogue()
{
    if (CurrentState == EDialogueState::Speaking)
    {
        SetDialogueState(EDialogueState::Listening);
        StopTypewriterEffect();
    }
}

void UMingDialoguePanel::ResumeDialogue()
{
    if (CurrentState == EDialogueState::Listening)
    {
        SetDialogueState(EDialogueState::Speaking);
        
        // 如果還有文本要顯示，繼續打字機效果
        if (CurrentText.Len() < TargetText.Len())
        {
            ProcessTypewriterEffect();
        }
    }
}

void UMingDialoguePanel::ShowDialogueOptions(const TArray<FDialogueOption>& Options)
{
    CurrentOptions = Options;
    bOptionsVisible = true;
    
    SetDialogueState(EDialogueState::Choosing);
    UpdateOptionsUI();
    
    UE_LOG(LogTemp, Log, TEXT("Showing dialogue options: %d options"), Options.Num());
}

void UMingDialoguePanel::HideDialogueOptions()
{
    bOptionsVisible = false;
    ClearOptionWidgets();
    
    UE_LOG(LogTemp, Log, TEXT("Hiding dialogue options"));
}

void UMingDialoguePanel::SelectDialogueOption(int32 OptionID)
{
    // 查找選項
    for (const FDialogueOption& Option : CurrentOptions)
    {
        if (Option.OptionID == OptionID && Option.bIsAvailable)
        {
            // 觸發選項選擇事件
            OnDialogueOptionSelected.Broadcast(OptionID, Option.OptionText);
            
            // 處理選項後的邏輯
            HideDialogueOptions();
            
            // 這裡可以根據選項ID執行不同的操作
            UE_LOG(LogTemp, Log, TEXT("Selected dialogue option %d: %s"), OptionID, *Option.OptionText);
            break;
        }
    }
}

void UMingDialoguePanel::OnOptionClicked(int32 OptionID)
{
    SelectDialogueOption(OptionID);
}

void UMingDialoguePanel::SetDialogueText(const FString& Text)
{
    CurrentText = Text;
    
    if (DialogueText)
    {
        DialogueText->SetText(FText::FromString(Text));
    }
}

void UMingDialoguePanel::AddDialogueText(const FString& Text)
{
    FString NewText = CurrentText + Text;
    SetDialogueText(NewText);
}

void UMingDialoguePanel::ClearDialogueText()
{
    CurrentText.Empty();
    TargetText.Empty();
    
    if (DialogueText)
    {
        DialogueText->SetText(FText::GetEmpty());
    }
}

void UMingDialoguePanel::SetCharacterName(const FString& Name)
{
    CurrentDialogue.CharacterName = Name;
    
    if (CharacterNameText)
    {
        CharacterNameText->SetText(FText::FromString(Name));
    }
}

bool UMingDialoguePanel::IsDialogueActive() const
{
    return CurrentState != EDialogueState::Idle && CurrentState != EDialogueState::Finished;
}

void UMingDialoguePanel::PlayTypewriterEffect(const FString& Text, float TypingSpeed)
{
    TargetText = Text;
    CurrentText.Empty();
    TypewriterIndex = 0;
    bTypewriterActive = true;
    
    // 重置進度條
    if (TypewriterProgress)
    {
        TypewriterProgress->SetPercent(0.0f);
    }
    
    // 開始打字機效果
    ProcessTypewriterEffect();
}

void UMingDialoguePanel::StopTypewriterEffect()
{
    bTypewriterActive = false;
    
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
    }
    
    // 設置最終文本
    SetDialogueText(TargetText);
    
    // 更新進度條
    if (TypewriterProgress)
    {
        TypewriterProgress->SetPercent(1.0f);
    }
    
    StopTypewriterSound();
}

void UMingDialoguePanel::ShowCharacterPortrait(const FString& CharacterID)
{
    if (CharacterPortrait)
    {
        // 這裡應該根據CharacterID加載對應的肖像
        UE_LOG(LogTemp, Log, TEXT("Showing portrait for: %s"), *CharacterID);
    }
}

void UMingDialoguePanel::HideCharacterPortrait()
{
    if (CharacterPortrait)
    {
        CharacterPortrait->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingDialoguePanel::SetDialogueEmotion(EDialogueType DialogueType)
{
    CurrentDialogue.DialogueType = DialogueType;
    CurrentDialogue.DialogueColor = GetDialogueColor(DialogueType, CurrentDialogue.RelationshipType);
    
    UpdateDialogueUI();
    
    UE_LOG(LogTemp, Log, TEXT("Set dialogue emotion: %d"), (int32)DialogueType);
}

void UMingDialoguePanel::PlayDialogueAudio(EDialogueType DialogueType)
{
    // 這裡應該調用音頻系統播放對話音效
    UE_LOG(LogTemp, Log, TEXT("Playing dialogue audio for type: %d"), (int32)DialogueType);
}

void UMingDialoguePanel::StopDialogueAudio()
{
    UE_LOG(LogTemp, Log, TEXT("Stopping dialogue audio"));
}

void UMingDialoguePanel::SetAudioVolume(float Volume)
{
    AudioVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

void UMingDialoguePanel::AddToHistory(const FString& CharacterID, const FString& Text)
{
    FString HistoryEntry = FString::Printf(TEXT("[%s]: %s"), *GetCharacterDisplayName(CharacterID), *Text);
    DialogueHistory.Add(HistoryEntry);
    
    UE_LOG(LogTemp, Log, TEXT("Added to dialogue history: %s"), *HistoryEntry);
}

void UMingDialoguePanel::ShowDialogueHistory()
{
    UE_LOG(LogTemp, Log, TEXT("Showing dialogue history (%d entries)"), DialogueHistory.Num());
}

void UMingDialoguePanel::HideDialogueHistory()
{
    UE_LOG(LogTemp, Log, TEXT("Hiding dialogue history"));
}

void UMingDialoguePanel::ClearDialogueHistory()
{
    DialogueHistory.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared dialogue history"));
}

void UMingDialoguePanel::QuickResponse(const FString& ResponseText)
{
    // 快速回應功能
    AddDialogueText(ResponseText);
    
    UE_LOG(LogTemp, Log, TEXT("Quick response: %s"), *ResponseText);
}

void UMingDialoguePanel::SkipDialogue()
{
    // 跳過當前對話
    StopTypewriterEffect();
    HideDialogueOptions();
    EndDialogue();
    
    UE_LOG(LogTemp, Log, TEXT("Skipped dialogue"));
}

void UMingDialoguePanel::AutoAdvance(bool bEnabled)
{
    bAutoAdvance = bEnabled;
    
    if (bEnabled && CurrentState == EDialogueState::Speaking && !bTypewriterActive)
    {
        // 設置自動推進定時器
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(AutoAdvanceTimer, this, &UMingDialoguePanel::OnAutoAdvanceTimer, AutoAdvanceDelay, false);
        }
    }
    else if (!bEnabled)
    {
        // 清除自動推進定時器
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(AutoAdvanceTimer);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Auto advance %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void UMingDialoguePanel::UpdateDialogueUI()
{
    // 更新對話文本顏色
    if (DialogueText)
    {
        DialogueText->SetColorAndOpacity(CurrentDialogue.DialogueColor);
    }
    
    // 更新角色名稱顏色
    if (CharacterNameText)
    {
        CharacterNameText->SetColorAndOpacity(CurrentDialogue.DialogueColor);
    }
    
    // 更新按鈕狀態
    if (ContinueButton)
    {
        ContinueButton->SetIsEnabled(CurrentState == EDialogueState::Speaking || CurrentState == EDialogueState::Listening);
    }
    
    if (SkipButton)
    {
        SkipButton->SetIsEnabled(IsDialogueActive());
    }
}

void UMingDialoguePanel::UpdateCharacterUI()
{
    SetCharacterName(CurrentDialogue.CharacterName);
    ShowCharacterPortrait(CurrentCharacterID);
    SetDialogueEmotion(CurrentDialogue.DialogueType);
}

void UMingDialoguePanel::UpdateOptionsUI()
{
    ClearOptionWidgets();
    
    if (bOptionsVisible && OptionsContainer)
    {
        CreateOptionWidgets();
    }
}

void UMingDialoguePanel::SetDialogueState(EDialogueState NewState)
{
    CurrentState = NewState;
    
    // 更新UI狀態
    UpdateDialogueUI();
    
    // 觸發狀態變化事件
    OnDialogueStateChanged.Broadcast(NewState);
    
    UE_LOG(LogTemp, Log, TEXT("Dialogue state changed to: %d"), (int32)NewState);
}

FLinearColor UMingDialoguePanel::GetDialogueColor(EDialogueType DialogueType, ERelationshipType RelationshipType) const
{
    // 根據對話類型和關係類型決定顏色
    switch (DialogueType)
    {
        case EDialogueAudioType::Friendly:
            return FLinearColor(0.0f, 0.8f, 0.0f, 1.0f); // 綠色
        case EDialogueAudioType::Hostile:
            return FLinearColor(0.8f, 0.0f, 0.0f, 1.0f); // 紅色
        case EDialogueAudioType::Romantic:
            return FLinearColor(0.8f, 0.0f, 0.8f, 1.0f); // 粉色
        case EDialogueAudioType::Business:
            return FLinearColor(0.0f, 0.8f, 0.8f, 1.0f); // 青色
        case EDialogueAudioType::Intimidation:
            return FLinearColor(0.8f, 0.4f, 0.0f, 1.0f); // 橙色
        case EDialogueAudioType::Persuasion:
            return FLinearColor(0.8f, 0.8f, 0.0f, 1.0f); // 黃色
        case EDialogueAudioType::Request:
            return FLinearColor(0.0f, 0.8f, 0.8f, 1.0f); // 淺藍色
        case EDialogueAudioType::Neutral:
        default:
            return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f); // 灰色
    }
}

FString UMingDialoguePanel::GetCharacterDisplayName(const FString& CharacterID) const
{
    // 這裡應該從角色數據獲取顯示名稱
    // 暫時返回ID
    return CharacterID;
}

void UMingDialoguePanel::ProcessTypewriterEffect()
{
    if (!bTypewriterActive || TypewriterIndex >= TargetText.Len())
    {
        bTypewriterActive = false;
        
        // 更新進度條
        if (TypewriterProgress)
        {
            TypewriterProgress->SetPercent(1.0f);
        }
        
        // 播放完成音效
        StopTypewriterSound();
        
        // 檢查是否需要自動推進
        if (bAutoAdvance)
        {
            AutoAdvance(true);
        }
        
        return;
    }
    
    // 添加下一個字符
    CurrentText += TargetText[TypewriterIndex];
    TypewriterIndex++;
    
    // 更新UI
    SetDialogueText(CurrentText);
    
    // 更新進度條
    if (TypewriterProgress)
    {
        float Progress = (float)TypewriterIndex / TargetText.Len();
        TypewriterProgress->SetPercent(Progress);
    }
    
    // 播放打字音效
    PlayTypewriterSound();
    
    // 設置下一次定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TypewriterTimer, this, &UMingDialoguePanel::ProcessTypewriterEffect, TypewriterSpeed, false);
    }
}

void UMingDialoguePanel::OnAutoAdvanceTimer()
{
    ContinueDialogue();
}

void UMingDialoguePanel::SetupEventBindings()
{
    // 綁定按鈕事件
    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &UMingDialoguePanel::ContinueDialogue);
    }
    
    if (SkipButton)
    {
        SkipButton->OnClicked.AddDynamic(this, &UMingDialoguePanel::SkipDialogue);
    }
    
    // 綁定關係管理器事件
    if (RelationshipManager)
    {
        // 這裡需要綁定關係變化事件
        // RelationshipManager->OnRelationshipChanged.AddDynamic(this, &UMingDialoguePanel::OnRelationshipChanged);
    }
}

void UMingDialoguePanel::OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason)
{
    // 如果正在與該角色對話，更新UI
    if (CurrentCharacterID == CharacterID)
    {
        UpdateCharacterUI();
    }
}

void UMingDialoguePanel::CreateOptionWidgets()
{
    // 這裡應該創建選項按鈕並添加到OptionsContainer
    for (const FDialogueOption& Option : CurrentOptions)
    {
        if (IsOptionAvailable(Option))
        {
            // 創建選項Widget
            // UButton* OptionButton = CreateOptionButton(Option);
            // OptionsContainer->AddChild(OptionButton);
        }
    }
}

void UMingDialoguePanel::ClearOptionWidgets()
{
    if (OptionsContainer)
    {
        OptionsContainer->ClearChildren();
    }
}

bool UMingDialoguePanel::IsOptionAvailable(const FDialogueOption& Option) const
{
    return Option.bIsAvailable;
}

void UMingDialoguePanel::PlayTypewriterSound()
{
    // 播放打字機音效
    UE_LOG(LogTemp, Log, TEXT("Playing typewriter sound"));
}

void UMingDialoguePanel::StopTypewriterSound()
{
    UE_LOG(LogTemp, Log, TEXT("Stopping typewriter sound"));
}
