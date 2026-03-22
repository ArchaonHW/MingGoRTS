#include "UI/MingUniversityGuideWidget.h"
#include "MingUniversityGuideManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

void UMingUniversityGuideWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 綁定按鈕事件
    if (NextButton)
    {
        NextButton->OnClicked.AddDynamic(this, &UMingUniversityGuideWidget::OnNextButtonClicked);
    }
    
    if (PreviousButton)
    {
        PreviousButton->OnClicked.AddDynamic(this, &UMingUniversityGuideWidget::OnPreviousButtonClicked);
    }
    
    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UMingUniversityGuideWidget::OnCloseButtonClicked);
    }
    
    // 綁定管理器事件
    if (GuideManager)
    {
        GuideManager->OnGuideContentUpdated.AddDynamic(this, &UMingUniversityGuideWidget::OnGuideContentUpdated);
        GuideManager->OnGuideCompleted.AddDynamic(this, &UMingUniversityGuideWidget::OnGuideCompleted);
        GuideManager->OnGuideStopped.AddDynamic(this, &UMingUniversityGuideWidget::OnGuideStopped);
    }
    
    // 播放進入動畫
    if (EnterAnimation)
    {
        PlayAnimation(EnterAnimation);
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingUniversityGuideWidget constructed"));
}

void UMingUniversityGuideWidget::NativeDestruct()
{
    // 解除綁定管理器事件
    if (GuideManager)
    {
        GuideManager->OnGuideContentUpdated.RemoveDynamic(this, &UMingUniversityGuideWidget::OnGuideContentUpdated);
        GuideManager->OnGuideCompleted.RemoveDynamic(this, &UMingUniversityGuideWidget::OnGuideCompleted);
        GuideManager->OnGuideStopped.RemoveDynamic(this, &UMingUniversityGuideWidget::OnGuideStopped);
    }
    
    // 解除綁定按鈕事件
    if (NextButton)
    {
        NextButton->OnClicked.RemoveDynamic(this, &UMingUniversityGuideWidget::OnNextButtonClicked);
    }
    
    if (PreviousButton)
    {
        PreviousButton->OnClicked.RemoveDynamic(this, &UMingUniversityGuideWidget::OnPreviousButtonClicked);
    }
    
    if (CloseButton)
    {
        CloseButton->OnClicked.RemoveDynamic(this, &UMingUniversityGuideWidget::OnCloseButtonClicked);
    }
    
    Super::NativeDestruct();
    
    UE_LOG(LogTemp, Log, TEXT("MingUniversityGuideWidget destructed"));
}

void UMingUniversityGuideWidget::SetGuideManager(UMingUniversityGuideManager* Manager)
{
    // 解除舊的管理器事件綁定
    if (GuideManager)
    {
        GuideManager->OnGuideContentUpdated.RemoveDynamic(this, &UMingUniversityGuideWidget::OnGuideContentUpdated);
        GuideManager->OnGuideCompleted.RemoveDynamic(this, &UMingUniversityGuideWidget::OnGuideCompleted);
        GuideManager->OnGuideStopped.RemoveDynamic(this, &UMingUniversityGuideWidget::OnGuideStopped);
    }
    
    GuideManager = Manager;
    
    // 綁定新管理器的事件
    if (GuideManager)
    {
        GuideManager->OnGuideContentUpdated.AddDynamic(this, &UMingUniversityGuideWidget::OnGuideContentUpdated);
        GuideManager->OnGuideCompleted.AddDynamic(this, &UMingUniversityGuideWidget::OnGuideCompleted);
        GuideManager->OnGuideStopped.AddDynamic(this, &UMingUniversityGuideWidget::OnGuideStopped);
        
        // 更新顯示
        UpdateContentDisplay();
        UpdateProgressBar();
        UpdateNavigationButtons();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Guide manager set"));
}

void UMingUniversityGuideWidget::DisplayContent(const FUniversityGuideContent& Content)
{
    // 更新標題
    if (TitleText)
    {
        TitleText->SetText(FText::FromString(Content.ContentTitle));
    }
    
    // 更新描述
    if (DescriptionText)
    {
        DescriptionText->SetText(FText::FromString(Content.ContentDescription));
    }
    
    // 更新導航按鈕可見性
    if (PreviousButton)
    {
        PreviousButton->SetVisibility(Content.bHasPreviousButton ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    
    if (NextButton)
    {
        FText ButtonText = Content.bHasNextButton ? 
            FText::FromString(TEXT("下一步 →")) : 
            FText::FromString(TEXT("完成 ✓"));
        
        NextButton->SetVisibility(ESlateVisibility::Visible);
        
        // 更新按鈕文字（如果按鈕有文本組件）
        if (UTextBlock* ButtonText = Cast<UTextBlock>(NextButton->GetChildAt(0)))
        {
            ButtonText->SetText(ButtonText);
        }
    }
    
    // 更新內容索引顯示
    if (GuideManager)
    {
        FString IndexText = FString::Printf(TEXT("%d / %d"), 
            GuideManager->GetCurrentContentIndex() + 1, 
            GuideManager->GetTotalContentCount());
        
        // 如果有索引文本組件，更新它
    }
    
    // 播放內容更新動畫
    if (ContentUpdateAnimation)
    {
        PlayAnimation(ContentUpdateAnimation);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Content displayed: %s"), *Content.ContentTitle);
}

void UMingUniversityGuideWidget::UpdateProgressBar()
{
    if (!GuideManager || !ProgressBar)
    {
        return;
    }
    
    float Progress = GuideManager->GetGuideProgress();
    ProgressBar->SetPercent(Progress);
    
    UE_LOG(LogTemp, Log, TEXT("Progress bar updated: %.2f%%"), Progress * 100.0f);
}

void UMingUniversityGuideWidget::UpdateContentDisplay()
{
    if (!GuideManager)
    {
        return;
    }
    
    FUniversityGuideContent CurrentContent = GuideManager->GetCurrentContent();
    DisplayContent(CurrentContent);
}

void UMingUniversityGuideWidget::UpdateNavigationButtons()
{
    if (!GuideManager)
    {
        return;
    }
    
    int32 CurrentIndex = GuideManager->GetCurrentContentIndex();
    int32 TotalCount = GuideManager->GetTotalContentCount();
    
    // 更新上一步按鈕
    if (PreviousButton)
    {
        bool bCanGoPrevious = (CurrentIndex > 0);
        PreviousButton->SetIsEnabled(bCanGoPrevious);
        PreviousButton->SetVisibility(bCanGoPrevious ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    
    // 更新下一步按鈕
    if (NextButton)
    {
        bool bIsLastContent = (CurrentIndex >= TotalCount - 1);
        
        // 更新按鈕文字
        FText ButtonText = bIsLastContent ? 
            FText::FromString(TEXT("完成 ✓")) : 
            FText::FromString(TEXT("下一步 →"));
        
        if (UTextBlock* ButtonTextBlock = Cast<UTextBlock>(NextButton->GetChildAt(0)))
        {
            ButtonTextBlock->SetText(ButtonText);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Navigation buttons updated"));
}

void UMingUniversityGuideWidget::ShowWidget()
{
    SetVisibility(ESlateVisibility::Visible);
    
    // 播放進入動畫
    if (EnterAnimation)
    {
        PlayAnimation(EnterAnimation);
    }
    
    // 更新顯示
    UpdateContentDisplay();
    UpdateProgressBar();
    UpdateNavigationButtons();
    
    UE_LOG(LogTemp, Log, TEXT("Widget shown"));
}

void UMingUniversityGuideWidget::HideWidget()
{
    // 播放退出動畫
    if (ExitAnimation)
    {
        PlayAnimation(ExitAnimation);
        
        // 動畫完成後隱藏
        FTimerHandle HideTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, [this]()
        {
            SetVisibility(ESlateVisibility::Hidden);
        }, ExitAnimation->GetEndTime(), false);
    }
    else
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Widget hidden"));
}

void UMingUniversityGuideWidget::OnGuideContentUpdated(int32 ContentIndex, const FString& ContentTitle)
{
    // 更新顯示
    UpdateContentDisplay();
    UpdateProgressBar();
    UpdateNavigationButtons();
    
    UE_LOG(LogTemp, Log, TEXT("Guide content updated: %d - %s"), ContentIndex, *ContentTitle);
}

void UMingUniversityGuideWidget::OnGuideCompleted()
{
    // 顯示完成界面
    if (TitleText)
    {
        TitleText->SetText(FText::FromString(TEXT("引導完成！")));
    }
    
    if (DescriptionText)
    {
        DescriptionText->SetText(FText::FromString(TEXT("恭喜您完成所有引導內容！現在您可以開始您的民國策略之旅了。")));
    }
    
    // 隱藏導航按鈕，只顯示關閉按鈕
    if (PreviousButton)
    {
        PreviousButton->SetVisibility(ESlateVisibility::Hidden);
    }
    
    if (NextButton)
    {
        NextButton->SetVisibility(ESlateVisibility::Hidden);
    }
    
    if (CloseButton)
    {
        CloseButton->SetVisibility(ESlateVisibility::Visible);
    }
    
    // 播放完成動畫
    if (CompleteAnimation)
    {
        PlayAnimation(CompleteAnimation);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Guide completed"));
}

void UMingUniversityGuideWidget::OnGuideStopped()
{
    HideWidget();
    
    UE_LOG(LogTemp, Log, TEXT("Guide stopped"));
}

void UMingUniversityGuideWidget::OnNextButtonClicked()
{
    if (GuideManager)
    {
        GuideManager->NavigateToNextContent();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Next button clicked"));
}

void UMingUniversityGuideWidget::OnPreviousButtonClicked()
{
    if (GuideManager)
    {
        GuideManager->NavigateToPreviousContent();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Previous button clicked"));
}

void UMingUniversityGuideWidget::OnCloseButtonClicked()
{
    if (GuideManager)
    {
        GuideManager->StopUniversityGuide();
    }
    
    HideWidget();
    
    UE_LOG(LogTemp, Log, TEXT("Close button clicked"));
}

void UMingUniversityGuideWidget::PlayEnterAnimation()
{
    if (EnterAnimation)
    {
        PlayAnimation(EnterAnimation);
    }
}

void UMingUniversityGuideWidget::PlayExitAnimation()
{
    if (ExitAnimation)
    {
        PlayAnimation(ExitAnimation);
    }
}

void UMingUniversityGuideWidget::PlayContentUpdateAnimation()
{
    if (ContentUpdateAnimation)
    {
        PlayAnimation(ContentUpdateAnimation);
    }
}

void UMingUniversityGuideWidget::PlayCompleteAnimation()
{
    if (CompleteAnimation)
    {
        PlayAnimation(CompleteAnimation);
    }
}
