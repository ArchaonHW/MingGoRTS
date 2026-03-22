#include "UI/MingDecisionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Platform/MingPlatformManager.h"

UMingDecisionWidget::UMingDecisionWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UMingDecisionWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 設置平台適配
    SetupForPlatform();
    
    // 綁定延後按鈪態
    if (PostponeButton)
    {
        PostponeButton->OnClicked.AddDynamic(this, &UMingDecisionWidget::OnPostponeClicked);
    }
}

void UMingDecisionWidget::SetupDecision(const FMingDecision& Decision)
{
    CurrentDecisionId = Decision.DecisionId;
    
    // 設置標題
    if (DecisionTitleText)
    {
        DecisionTitleText->SetText(Decision.DecisionTitle);
    }
    
    // 設置背景描述
    if (BackgroundDescriptionText)
    {
        BackgroundDescriptionText->SetText(Decision.BackgroundDescription);
    }
    
    // 設置背景圖片
    if (BackgroundImage && Decision.BackgroundImage)
    {
        BackgroundImage->SetBrushFromTexture(Decision.BackgroundImage);
    }
    
    // 創建選項按鈪態
    CreateOptionButtons(Decision.Options);
    
    // 設置延後按鈪態可見性
    if (PostponeButton)
    {
        PostponeButton->SetVisibility(Decision.bCanPostpone ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

void UMingDecisionWidget::ShowDecision()
{
    PlayShowAnimation();
    SetVisibility(ESlateVisibility::Visible);
}

void UMingDecisionWidget::HideDecision()
{
    PlayHideAnimation();
    // 延遲隱藏，等待動畫完成
    FTimerHandle HideTimer;
    GetWorld()->GetTimerManager().SetTimer(HideTimer, [this]()
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }, 0.5f, false);
}

void UMingDecisionWidget::CreateOptionButtons(const TArray<FMingDecisionOption>& Options)
{
    // 清空現有按鈪態
    OptionButtons.Empty();
    
    if (!OptionsContainer)
    {
        return;
    }
    
    // 清空容器
    OptionsContainer->ClearChildren();
    
    // 為每個選項創建按鈪態
    for (const auto& Option : Options)
    {
        // 檢查選項是否可用
        if (!Option.IsAvailable())
        {
            continue;
        }
        
        // 創建按鈪態 (這裡應該使用預設的按鈪態類或動態創建)
        UButton* OptionButton = NewObject<UButton>(this);
        if (OptionButton)
        {
            // 創建按鈪態文本
            UTextBlock* ButtonText = NewObject<UTextBlock>(OptionButton);
            if (ButtonText)
            {
                // 使用預覽文本或標題
                FText ButtonLabel = Option.OptionPreview.IsEmpty() ? Option.OptionTitle : Option.OptionPreview;
                ButtonText->SetText(ButtonLabel);
                
                // 設置字體樣式
                ButtonText->SetAutoWrapText(true);
                
                // 添加到按鈪態
                OptionButton->AddChild(ButtonText);
            }
            
            // 綁定點擊事件
            FString CapturedOptionId = Option.OptionId;
            OptionButton->OnClicked.AddDynamic(this, &UMingDecisionWidget::OnOptionButtonClicked);
            
            // 添加到容器
            OptionsContainer->AddChild(OptionButton);
            
            // 保存引用
            OptionButtons.Add(OptionButton);
        }
    }
}

void UMingDecisionWidget::OnOptionButtonClicked(const FString& OptionId)
{
    // 發布選擇事件
    OnOptionSelected.Broadcast(CurrentDecisionId, OptionId);
    
    // 隱藏界面
    HideDecision();
}

void UMingDecisionWidget::OnPostponeClicked()
{
    // 隱藏界面，但不執行任何選項
    HideDecision();
    
    UE_LOG(LogTemp, Log, TEXT("Decision %s postponed"), *CurrentDecisionId);
}

void UMingDecisionWidget::SetupForPlatform()
{
    // 獲取平台管理器
    UMingPlatformManager* PlatformManager = UMingPlatformManager::Get();
    if (!PlatformManager)
    {
        return;
    }
    
    EInputType InputType = PlatformManager->GetPlatformCapabilities().DefaultInputType;
    
    if (InputType == EInputType::Touch)
    {
        // 移動端適配：增大按鈪態尺寸
        // 設置最小觸控目標尺寸
        for (auto& Button : OptionButtons)
        {
            if (Button)
            {
                // 移動端使用更大的按鈪態
                // TODO: 設置按鈪態樣式
            }
        }
    }
}
