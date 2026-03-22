#include "MingDialoguePanel.h"
#include "MingRelationshipManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

UMingDialoguePanel::UMingDialoguePanel()
{
    bIsPlaying = false;
    bIsTyping = false;
    CurrentDialogueIndex = 0;
    TypingSpeed = 0.05f;
    SelectedOptionIndex = -1;
    bAutoAdvance = false;
    AutoAdvanceDelay = 3.0f;
}

void UMingDialoguePanel::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 綁定按鈕事件
    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &UMingDialoguePanel::OnContinueClicked);
    }
    
    if (SkipButton)
    {
        SkipButton->OnClicked.AddDynamic(this, &UMingDialoguePanel::OnSkipClicked);
    }
    
    if (AutoAdvanceButton)
    {
        AutoAdvanceButton->OnClicked.AddDynamic(this, &UMingDialoguePanel::OnAutoAdvanceClicked);
    }
    
    // 隱藏初始狀態
    HideDialogue();
}

void UMingDialoguePanel::StartDialogue(const FDialogueData& InDialogueData)
{
    if (bIsPlaying)
    {
        StopDialogue();
    }
    
    DialogueData = InDialogueData;
    CurrentDialogueIndex = 0;
    SelectedOptionIndex = -1;
    bIsPlaying = true;
    
    // 顯示對話面板
    ShowDialogue();
    
    // 開始播放第一句對話
    PlayCurrentDialogue();
    
    UE_LOG(LogTemp, Log, TEXT("開始播放對話：%s"), *DialogueData.DialogueID);
}

void UMingDialoguePanel::StopDialogue()
{
    bIsPlaying = false;
    bIsTyping = false;
    
    // 停止打字機效果
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
    }
    
    // 停止自動前進
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoAdvanceTimerHandle);
    }
    
    // 隱藏對話面板
    HideDialogue();
    
    // 觸發完成事件
    OnDialogueFinished.Broadcast(DialogueData.DialogueID);
    
    UE_LOG(LogTemp, Log, TEXT("停止播放對話：%s"), *DialogueData.DialogueID);
}

void UMingDialoguePanel::ContinueDialogue()
{
    if (!bIsPlaying || bIsTyping)
    {
        return;
    }
    
    // 檢查是否有選項需要處理
    if (SelectedOptionIndex >= 0)
    {
        ProcessDialogueOption(SelectedOptionIndex);
        SelectedOptionIndex = -1;
        return;
    }
    
    // 移動到下一句對話
    CurrentDialogueIndex++;
    
    if (CurrentDialogueIndex >= DialogueData.DialogueEntries.Num())
    {
        // 對話結束
        StopDialogue();
    }
    else
    {
        // 播放下一句對話
        PlayCurrentDialogue();
    }
}

void UMingDialoguePanel::SkipDialogue()
{
    if (!bIsPlaying)
    {
        return;
    }
    
    // 停止打字機效果
    if (bIsTyping)
    {
        bIsTyping = false;
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        }
        
        // 立即顯示完整文本
        if (DialogueText)
        {
            DialogueText->SetText(FText::FromString(GetCurrentDialogueText()));
        }
    }
    else
    {
        // 直接跳到對話結尾
        CurrentDialogueIndex = DialogueData.DialogueEntries.Num() - 1;
        PlayCurrentDialogue();
    }
}

void UMingDialoguePanel::SetTypingSpeed(float NewSpeed)
{
    TypingSpeed = FMath::Clamp(NewSpeed, 0.01f, 0.2f);
}

void UMingDialoguePanel::SetAutoAdvance(bool bEnabled, float Delay)
{
    bAutoAdvance = bEnabled;
    AutoAdvanceDelay = FMath::Clamp(Delay, 1.0f, 10.0f);
    
    // 更新按鈕狀態
    if (AutoAdvanceButton)
    {
        AutoAdvanceButton->SetIsEnabled(!bAutoAdvance);
    }
}

void UMingDialoguePanel::SelectDialogueOption(int32 OptionIndex)
{
    if (!bIsPlaying || bIsTyping || OptionIndex < 0 || OptionIndex >= CurrentDialogueOptions.Num())
    {
        return;
    }
    
    SelectedOptionIndex = OptionIndex;
    
    // 更新選項視覺狀態
    UpdateOptionSelection(OptionIndex);
    
    // 觸發選項選擇事件
    OnDialogueOptionSelected.Broadcast(DialogueData.DialogueID, OptionIndex, CurrentDialogueOptions[OptionIndex]);
}

void UMingDialoguePanel::PlayTypewriterEffect(const FString& Text)
{
    if (!DialogueText)
    {
        return;
    }
    
    bIsTyping = true;
    CurrentTypingText = TEXT("");
    TargetTypingText = Text;
    CurrentCharIndex = 0;
    
    // 開始打字機效果
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &UMingDialoguePanel::OnTypingTick, TypingSpeed, true);
    }
}

void UMingDialoguePanel::ShowCharacterPortrait(const FString& CharacterID)
{
    if (!CharacterPortraitImage)
    {
        return;
    }
    
    // 這裡應該根據CharacterID加載對應的角色頭像
    // 暫時使用日誌記錄
    UE_LOG(LogTemp, Log, TEXT("顯示角色頭像：%s"), *CharacterID);
}

void UMingDialoguePanel::HideCharacterPortrait()
{
    if (CharacterPortraitImage)
    {
        CharacterPortraitImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingDialoguePanel::PlayDialogueSound(const FString& SoundID)
{
    // 播放對話音效
    UE_LOG(LogTemp, Log, TEXT("播放對話音效：%s"), *SoundID);
}

// 按鈕事件處理
void UMingDialoguePanel::OnContinueClicked()
{
    if (bIsTyping)
    {
        SkipDialogue();
    }
    else
    {
        ContinueDialogue();
    }
}

void UMingDialoguePanel::OnSkipClicked()
{
    SkipDialogue();
}

void UMingDialoguePanel::OnAutoAdvanceClicked()
{
    SetAutoAdvance(!bAutoAdvance, AutoAdvanceDelay);
}

void UMingDialoguePanel::OnOptionClicked(int32 OptionIndex)
{
    SelectDialogueOption(OptionIndex);
}

void UMingDialoguePanel::OnTypingTick()
{
    if (CurrentCharIndex >= TargetTypingText.Len())
    {
        // 打字機效果完成
        bIsTyping = false;
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        }
        
        // 設置完整文本
        if (DialogueText)
        {
            DialogueText->SetText(FText::FromString(TargetTypingText));
        }
        
        // 處理自動前進
        if (bAutoAdvance && CurrentDialogueOptions.Num() == 0)
        {
            if (GetWorld())
            {
                GetWorld()->GetTimerManager().SetTimer(AutoAdvanceTimerHandle, this, &UMingDialoguePanel::ContinueDialogue, AutoAdvanceDelay, false);
            }
        }
        
        // 觸發文本完成事件
        OnDialogueTextCompleted.Broadcast(DialogueData.DialogueID, CurrentDialogueIndex, TargetTypingText);
        
        return;
    }
    
    // 添加下一個字符
    CurrentTypingText += TargetTypingText[CurrentCharIndex];
    CurrentCharIndex++;
    
    // 更新顯示
    if (DialogueText)
    {
        DialogueText->SetText(FText::FromString(CurrentTypingText));
    }
    
    // 播放打字音效（每幾個字符播放一次）
    if (CurrentCharIndex % 3 == 0)
    {
        PlayDialogueSound(TEXT("TypingSound"));
    }
}

// 私有輔助函數
void UMingDialoguePanel::PlayCurrentDialogue()
{
    if (CurrentDialogueIndex >= DialogueData.DialogueEntries.Num())
    {
        return;
    }
    
    const FDialogueEntry& Entry = DialogueData.DialogueEntries[CurrentDialogueIndex];
    
    // 更新說話者名稱
    if (SpeakerNameText)
    {
        SpeakerNameText->SetText(FText::FromString(Entry.SpeakerName));
    }
    
    // 顯示角色頭像
    if (!Entry.CharacterID.IsEmpty())
    {
        ShowCharacterPortrait(Entry.CharacterID);
    }
    else
    {
        HideCharacterPortrait();
    }
    
    // 播放對話音效
    if (!Entry.SoundID.IsEmpty())
    {
        PlayDialogueSound(Entry.SoundID);
    }
    
    // 播放打字機效果
    PlayTypewriterEffect(Entry.DialogueText);
    
    // 處理對話選項
    if (Entry.Options.Num() > 0)
    {
        CurrentDialogueOptions = Entry.Options;
        CreateDialogueOptions(Entry.Options);
    }
    else
    {
        CurrentDialogueOptions.Empty();
        ClearDialogueOptions();
    }
    
    // 觸發對話開始事件
    OnDialogueStarted.Broadcast(DialogueData.DialogueID, CurrentDialogueIndex, Entry.SpeakerName);
}

FString UMingDialoguePanel::GetCurrentDialogueText()
{
    if (CurrentDialogueIndex >= DialogueData.DialogueEntries.Num())
    {
        return TEXT("");
    }
    
    return DialogueData.DialogueEntries[CurrentDialogueIndex].DialogueText;
}

void UMingDialoguePanel::CreateDialogueOptions(const TArray<FDialogueOption>& Options)
{
    if (!OptionsContainer)
    {
        return;
    }
    
    // 清空現有選項
    ClearDialogueOptions();
    
    // 創建選項按鈕
    for (int32 i = 0; i < Options.Num(); ++i)
    {
        UButton* OptionButton = CreateOptionButton(Options[i], i);
        if (OptionButton)
        {
            OptionsContainer->AddChild(OptionButton);
        }
    }
    
    // 顯示選項容器
    OptionsContainer->SetVisibility(ESlateVisibility::Visible);
}

void UMingDialoguePanel::ClearDialogueOptions()
{
    if (OptionsContainer)
    {
        OptionsContainer->ClearChildren();
        OptionsContainer->SetVisibility(ESlateVisibility::Hidden);
    }
    
    CurrentDialogueOptions.Empty();
}

UButton* UMingDialoguePanel::CreateOptionButton(const FDialogueOption& Option, int32 Index)
{
    UButton* Button = NewObject<UButton>(this);
    if (Button)
    {
        // 設置按鈕文本
        UTextBlock* TextBlock = NewObject<UTextBlock>(Button);
        if (TextBlock)
        {
            TextBlock->SetText(FText::FromString(Option.OptionText));
            FSlateFontInfo FontInfo = TextBlock->GetFont();
            FontInfo.Size = 14;
            TextBlock->SetFont(FontInfo);
            Button->AddChild(TextBlock);
        }
        
        // 綁定點擊事件
        FScriptDelegate Delegate;
        Delegate.BindUFunction(this, TEXT("OnOptionClicked"), Index);
        Button->OnClicked.Add(Delegate);
        
        // 設置樣式
        Button->SetMargin(FMargin(5.0f, 2.0f));
    }
    
    return Button;
}

void UMingDialoguePanel::UpdateOptionSelection(int32 SelectedIndex)
{
    if (!OptionsContainer)
    {
        return;
    }
    
    // 更新所有選項的視覺狀態
    for (int32 i = 0; i < OptionsContainer->GetChildrenCount(); ++i)
    {
        UButton* OptionButton = Cast<UButton>(OptionsContainer->GetChildAt(i));
        if (OptionButton)
        {
            if (i == SelectedIndex)
            {
                // 選中狀態
                OptionButton->SetBackgroundColor(FLinearColor(0.2f, 0.4f, 0.8f, 0.8f));
            }
            else
            {
                // 未選中狀態
                OptionButton->SetBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.5f));
            }
        }
    }
}

void UMingDialoguePanel::ProcessDialogueOption(int32 OptionIndex)
{
    if (OptionIndex < 0 || OptionIndex >= CurrentDialogueOptions.Num())
    {
        return;
    }
    
    const FDialogueOption& SelectedOption = CurrentDialogueOptions[OptionIndex];
    
    // 執行選項動作
    ExecuteDialogueAction(SelectedOption.Action);
    
    // 跳轉到指定對話或結束
    if (SelectedOption.NextDialogueIndex >= 0)
    {
        CurrentDialogueIndex = SelectedOption.NextDialogueIndex;
        PlayCurrentDialogue();
    }
    else
    {
        // 繼續到下一句對話
        ContinueDialogue();
    }
}

void UMingDialoguePanel::ExecuteDialogueAction(const FDialogueAction& Action)
{
    // 根據動作類型執行相應操作
    switch (Action.ActionType)
    {
        case EDialogueActionType::GiveItem:
            UE_LOG(LogTemp, Log, TEXT("給予物品：%s"), *Action.ActionData);
            break;
            
        case EDialogueActionType::ModifyRelationship:
            UE_LOG(LogTemp, Log, TEXT("修改關係：%s"), *Action.ActionData);
            break;
            
        case EDialogueActionType::ModifyReputation:
            UE_LOG(LogTemp, Log, TEXT("修改聲望：%s"), *Action.ActionData);
            break;
            
        case EDialogueActionType::StartQuest:
            UE_LOG(LogTemp, Log, TEXT("開始任務：%s"), *Action.ActionData);
            break;
            
        case EDialogueActionType::CompleteQuest:
            UE_LOG(LogTemp, Log, TEXT("完成任務：%s"), *Action.ActionData);
            break;
            
        case EDialogueActionType::PlaySound:
            PlayDialogueSound(Action.ActionData);
            break;
            
        case EDialogueActionType::PlayAnimation:
            UE_LOG(LogTemp, Log, TEXT("播放動畫：%s"), *Action.ActionData);
            break;
            
        case EDialogueActionType::Custom:
            UE_LOG(LogTemp, Log, TEXT("執行自定義動作：%s"), *Action.ActionData);
            break;
    }
    
    // 觸發動作執行事件
    OnDialogueActionExecuted.Broadcast(DialogueData.DialogueID, Action);
}

void UMingDialoguePanel::ShowDialogue()
{
    if (DialoguePanel)
    {
        DialoguePanel->SetVisibility(ESlateVisibility::Visible);
        PlayShowAnimation();
    }
}

void UMingDialoguePanel::HideDialogue()
{
    if (DialoguePanel)
    {
        PlayHideAnimation();
        // 延遲隱藏以等待動畫完成
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (DialoguePanel)
            {
                DialoguePanel->SetVisibility(ESlateVisibility::Hidden);
            }
        }, 0.3f, false);
    }
}

void UMingDialoguePanel::PlayShowAnimation()
{
    // 播放顯示動畫
    UE_LOG(LogTemp, Log, TEXT("播放對話面板顯示動畫"));
}

void UMingDialoguePanel::PlayHideAnimation()
{
    // 播放隱藏動畫
    UE_LOG(LogTemp, Log, TEXT("播放對話面板隱藏動畫"));
}

void UMingDialoguePanel::UpdateContinueButton()
{
    if (!ContinueButton)
    {
        return;
    }
    
    if (bIsTyping)
    {
        ContinueButton->SetText(FText::FromString(TEXT("跳過")));
    }
    else if (CurrentDialogueOptions.Num() > 0)
    {
        ContinueButton->SetText(FText::FromString(TEXT("選擇")));
        ContinueButton->SetIsEnabled(false);
    }
    else
    {
        ContinueButton->SetText(FText::FromString(TEXT("繼續")));
        ContinueButton->SetIsEnabled(true);
    }
}

void UMingDialoguePanel::UpdateAutoAdvanceButton()
{
    if (!AutoAdvanceButton)
    {
        return;
    }
    
    if (bAutoAdvance)
    {
        AutoAdvanceButton->SetText(FText::FromString(TEXT("自動前進：開")));
        AutoAdvanceButton->SetIsEnabled(false);
    }
    else
    {
        AutoAdvanceButton->SetText(FText::FromString(TEXT("自動前進：關")));
        AutoAdvanceButton->SetIsEnabled(true);
    }
}
