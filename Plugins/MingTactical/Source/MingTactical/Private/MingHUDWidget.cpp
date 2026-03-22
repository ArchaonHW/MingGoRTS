#include "MingHUDWidget.h"
#include "MingSelectionWidget.h"
#include "MingTacticalUnit.h"
#include "MingRTSPlayerController.h"
#include "MingSelectionManager.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UMingHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    bShowDebugInfo = false;
    
    // 獲取玩家控制器
    if (UWorld* World = GetWorld())
    {
        OwningController = Cast<AMingRTSPlayerController>(World->GetFirstPlayerController());
        if (OwningController)
        {
            SelectionManager = OwningController->GetSelectionManager();
            SetupBindings();
        }
    }
    
    // 初始化UI狀態
    HideSelectionWidget();
    ClearUnitInfo();
    HideCommandButtons();
    
    UE_LOG(LogTemp, Log, TEXT("MingHUDWidget constructed"));
}

void UMingHUDWidget::NativeDestruct()
{
    Super::NativeDestruct();
    
    UE_LOG(LogTemp, Log, TEXT("MingHUDWidget destructed"));
}

void UMingHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // 更新單位信息
    if (SelectionManager && SelectionManager->GetSelectedUnitCount() == 1)
    {
        // 獲取選中的單位
        int32 UnitId = SelectionManager->SelectedUnitIds[0];
        if (UWorld* World = GetWorld())
        {
            for (TActorIterator<AMingTacticalUnit> It(World); It; ++It)
            {
                if (It->UnitId == UnitId)
                {
                    UpdateUnitInfo(*It);
                    break;
                }
            }
        }
    }
    else if (SelectionManager && SelectionManager->GetSelectedUnitCount() == 0)
    {
        ClearUnitInfo();
    }
    
    // 更新除錯信息
    if (bShowDebugInfo && DebugInfoText)
    {
        FString DebugString = FString::Printf(
            TEXT("Units: %d | Selected: %d | FPS: %.1f"),
            SelectionManager ? SelectionManager->GetSelectedUnitCount() : 0,
            SelectionManager ? SelectionManager->GetSelectedUnitCount() : 0,
            1.0f / InDeltaTime
        );
        UpdateDebugInfo(DebugString);
    }
}

bool UMingHUDWidget::Initialize()
{
    bool bSuccess = Super::Initialize();
    
    return bSuccess;
}

void UMingHUDWidget::SetupBindings()
{
    if (!SelectionManager)
    {
        return;
    }
    
    // 綁定選擇事件
    SelectionManager->OnUnitSelected.AddDynamic(this, &UMingHUDWidget::OnSelectionChanged);
    SelectionManager->OnUnitDeselected.AddDynamic(this, &UMingHUDWidget::OnSelectionChanged);
}

void UMingHUDWidget::OnSelectionChanged(int32 UnitId)
{
    // 更新UI顯示
    if (SelectionManager)
    {
        int32 SelectedCount = SelectionManager->GetSelectedUnitCount();
        
        if (SelectedCount == 0)
        {
            ClearUnitInfo();
            HideCommandButtons();
        }
        else if (SelectedCount == 1)
        {
            // 顯示單個單位信息
            if (UWorld* World = GetWorld())
            {
                for (TActorIterator<AMingTacticalUnit> It(World); It; ++It)
                {
                    if (It->UnitId == UnitId)
                    {
                        UpdateUnitInfo(*It);
                        break;
                    }
                }
            }
            ShowCommandButtons({FName("Move"), FName("Attack"), FName("Stop")});
        }
        else
        {
            // 多個單位選中
            ClearUnitInfo();
            ShowCommandButtons({FName("Move"), FName("Attack"), FName("Stop"), FName("Hold")});
        }
    }
}

void UMingHUDWidget::ShowSelectionWidget()
{
    if (SelectionWidget)
    {
        SelectionWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingHUDWidget::HideSelectionWidget()
{
    if (SelectionWidget)
    {
        SelectionWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingHUDWidget::UpdateUnitInfo(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        ClearUnitInfo();
        return;
    }
    
    // 更新單位名稱
    if (UnitNameText)
    {
        UnitNameText->SetText(FText::FromString(Unit->UnitName));
    }
    
    // 更新生命值
    if (HealthBar)
    {
        float HealthPercent = Unit->GetHealthPercentage();
        HealthBar->SetPercent(HealthPercent);
        
        // 根據生命值改變顏色
        FLinearColor HealthColor;
        if (HealthPercent > 0.5f)
        {
            HealthColor = FLinearColor::Green;
        }
        else if (HealthPercent > 0.25f)
        {
            HealthColor = FLinearColor::Yellow;
        }
        else
        {
            HealthColor = FLinearColor::Red;
        }
        
        HealthBar->SetFillColorAndOpacity(HealthColor);
    }
    
    // 顯示單位信息面板
    if (UnitInfoPanel)
    {
        UnitInfoPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMingHUDWidget::ClearUnitInfo()
{
    if (UnitNameText)
    {
        UnitNameText->SetText(FText::FromString(TEXT("")));
    }
    
    if (HealthBar)
    {
        HealthBar->SetPercent(0.0f);
    }
    
    if (UnitInfoPanel)
    {
        UnitInfoPanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingHUDWidget::UpdateResources(const TMap<FString, int32>& Resources)
{
    if (!ResourceText)
    {
        return;
    }
    
    FString ResourceString;
    for (const auto& Pair : Resources)
    {
        if (!ResourceString.IsEmpty())
        {
            ResourceString += TEXT(" | ");
        }
        ResourceString += FString::Printf(TEXT("%s: %d"), *Pair.Key, Pair.Value);
    }
    
    ResourceText->SetText(FText::FromString(ResourceString));
}

void UMingHUDWidget::ShowCommandButtons(const TArray<FName>& AvailableCommands)
{
    if (!CommandButtonsContainer)
    {
        return;
    }
    
    CommandButtonsContainer->ClearChildren();
    CommandButtonsContainer->SetVisibility(ESlateVisibility::Visible);
    
    int32 Column = 0;
    int32 Row = 0;
    
    for (const FName& Command : AvailableCommands)
    {
        // 創建命令按鈝
        UButton* CommandButton = NewObject<UButton>(CommandButtonsContainer);
        
        // 設置按鈕文字
        UTextBlock* ButtonText = NewObject<UTextBlock>(CommandButton);
        ButtonText->SetText(FText::FromName(Command));
        CommandButton->AddChild(ButtonText);
        
        // 添加到容器
        UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(CommandButtonsContainer->AddChild(CommandButton));
        if (GridSlot)
        {
            GridSlot->SetColumn(Column);
            GridSlot->SetRow(Row);
        }
        
        // 綁定點擊事件
        CommandButton->OnClicked.AddDynamic(this, &UMingHUDWidget::OnSelectionChanged);
        
        Column++;
        if (Column >= 4) // 每行4個按鈕
        {
            Column = 0;
            Row++;
        }
    }
}

void UMingHUDWidget::HideCommandButtons()
{
    if (CommandButtonsContainer)
    {
        CommandButtonsContainer->ClearChildren();
        CommandButtonsContainer->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingHUDWidget::UpdateDebugInfo(const FString& DebugText)
{
    if (DebugInfoText)
    {
        DebugInfoText->SetText(FText::FromString(DebugText));
    }
}
