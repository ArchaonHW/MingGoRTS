#include "MingResourceDisplayWidget.h"
#include "MingBuilding/Source/MingBuilding/Public/MingResourceSystem.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Animation/WidgetAnimation.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void UMingResourceDisplayWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 獲取資源系統引用
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            // 這裡需要根據實際項目結構獲取資源系統
            // 暫時創建一個臨時引用，實際應該從玩家控制器或遊戲模式獲取
            ResourceSystem = NewObject<UMingResourceSystem>();
            ResourceSystem->InitializeDefaultResources();
        }
    }

    // 初始化資源顯示
    InitializeResourceDisplays();
    
    // 綁定事件
    BindResourceEvents();

    UE_LOG(LogTemp, Log, TEXT("MingResourceDisplayWidget constructed"));
}

void UMingResourceDisplayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 處理動畫計時器
    for (auto& TimerPair : AnimationTimers)
    {
        if (TimerPair.Value > 0.0f)
        {
            AnimationTimers[TimerPair.Key] = FMath::Max(0.0f, TimerPair.Value - InDeltaTime);
            
            // 動畫結束時恢復正常顏色
            if (AnimationTimers[TimerPair.Key] <= 0.0f)
            {
                FResourceDisplayItem* Item = GetResourceDisplayItem(TimerPair.Key);
                if (Item && ResourceSystem.IsValid())
                {
                    int32 CurrentAmount = ResourceSystem->GetResourceAmount(TimerPair.Key);
                    int32 Capacity = ResourceSystem->GetResourceCapacity(TimerPair.Key);
                    UpdateSingleResourceItem(Item, TimerPair.Key, CurrentAmount, Capacity);
                }
            }
        }
    }
}

bool UMingResourceDisplayWidget::Initialize()
{
    bool bSuccess = Super::Initialize();
    
    SetupResourceDisplays();
    
    return bSuccess;
}

void UMingResourceDisplayWidget::InitializeResourceDisplays()
{
    // 初始化資源顯示映射
    ResourceDisplayMap.Add(EMingResourceType::Food, &FoodDisplay);
    ResourceDisplayMap.Add(EMingResourceType::Money, &MoneyDisplay);
    ResourceDisplayMap.Add(EMingResourceType::Materials, &MaterialsDisplay);
    ResourceDisplayMap.Add(EMingResourceType::Fuel, &FuelDisplay);
    ResourceDisplayMap.Add(EMingResourceType::Ammo, &AmmoDisplay);
    ResourceDisplayMap.Add(EMingResourceType::Manpower, &ManpowerDisplay);

    // 設置資源類型
    FoodDisplay.ResourceType = EMingResourceType::Food;
    MoneyDisplay.ResourceType = EMingResourceType::Money;
    MaterialsDisplay.ResourceType = EMingResourceType::Materials;
    FuelDisplay.ResourceType = EMingResourceType::Fuel;
    AmmoDisplay.ResourceType = EMingResourceType::Ammo;
    ManpowerDisplay.ResourceType = EMingResourceType::Manpower;

    // 初始化動畫計時器
    for (int32 i = 0; i < 6; ++i)
    {
        EMingResourceType ResourceType = static_cast<EMingResourceType>(i);
        AnimationTimers.Add(ResourceType, 0.0f);
        LastResourceAmounts.Add(ResourceType, 0);
    }
}

void UMingResourceDisplayWidget::SetupResourceDisplays()
{
    if (!ResourceContainer)
    {
        UE_LOG(LogTemp, Warning, TEXT("ResourceContainer not found in MingResourceDisplayWidget"));
        return;
    }

    // 清空現有子項目
    ResourceContainer->ClearChildren();

    // 為每種資源創建顯示項目
    TArray<EMingResourceType> ResourceTypes = {
        EMingResourceType::Food,
        EMingResourceType::Money,
        EMingResourceType::Materials,
        EMingResourceType::Fuel,
        EMingResourceType::Ammo,
        EMingResourceType::Manpower
    };

    for (EMingResourceType ResourceType : ResourceTypes)
    {
        // 創建邊框容器
        UBorder* ItemBorder = NewObject<UBorder>(ResourceContainer);
        ItemBorder->SetBorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f));
        ItemBorder->SetPadding(FMargin(8.0f, 4.0f));

        // 創建垂直盒子
        UVerticalBox* VerticalBox = NewObject<UVerticalBox>(ItemBorder);

        // 創建資源名稱文字
        UTextBlock* NameText = NewObject<UTextBlock>(VerticalBox);
        NameText->SetText(FText::FromString(GetResourceDisplayName(ResourceType)));
        NameText->SetColorAndOpacity(GetResourceDisplayColor(ResourceType));
        NameText->SetFont(FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("Fonts"), 12));

        // 創建水平盒子（圖標 + 數量）
        UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(VerticalBox);

        // 創建資源圖標
        UImage* ResourceIcon = NewObject<UImage>(HorizontalBox);
        ResourceIcon->SetBrushColor(GetResourceDisplayColor(ResourceType));
        ResourceIcon->SetDesiredSize(FVector2D(16.0f, 16.0f));

        // 創建資源數量文字
        UTextBlock* AmountText = NewObject<UTextBlock>(HorizontalBox);
        AmountText->SetText(FText::FromString(TEXT("0")));
        AmountText->SetColorAndOpacity(FLinearColor::White);

        // 創建進度條
        UProgressBar* ProgressBar = NewObject<UProgressBar>(VerticalBox);
        ProgressBar->SetPercent(0.0f);
        ProgressBar->SetFillColorAndOpacity(GetResourceDisplayColor(ResourceType));
        ProgressBar->SetBorderBackgroundColor(FLinearColor(0.2f, 0.2f, 0.2f, 0.5f));

        // 組裝UI
        HorizontalBox->AddChild(ResourceIcon);
        HorizontalBox->AddChild(AmountText);
        
        VerticalBox->AddChild(NameText);
        VerticalBox->AddChild(HorizontalBox);
        VerticalBox->AddChild(ProgressBar);

        ItemBorder->AddChild(VerticalBox);
        ResourceContainer->AddChild(ItemBorder);

        // 更新對應的顯示項目引用
        FResourceDisplayItem* Item = GetResourceDisplayItem(ResourceType);
        if (Item)
        {
            Item->ItemBorder = ItemBorder;
            Item->ResourceNameText = NameText;
            Item->ResourceAmountText = AmountText;
            Item->ResourceProgressBar = ProgressBar;
            Item->ResourceIcon = ResourceIcon;
        }
    }

    // 初始更新所有資源
    if (ResourceSystem.IsValid())
    {
        TMap<EMingResourceType, int32> Resources;
        TMap<EMingResourceType, int32> Capacities;
        
        for (int32 i = 0; i < 6; ++i)
        {
            EMingResourceType ResourceType = static_cast<EMingResourceType>(i);
            Resources.Add(ResourceType, ResourceSystem->GetResourceAmount(ResourceType));
            Capacities.Add(ResourceType, ResourceSystem->GetResourceCapacity(ResourceType));
        }
        
        UpdateAllResources(Resources, Capacities);
    }
}

void UMingResourceDisplayWidget::BindResourceEvents()
{
    if (ResourceSystem.IsValid())
    {
        ResourceSystem->OnResourceChanged.AddDynamic(this, &UMingResourceDisplayWidget::OnResourceChanged);
        ResourceSystem->OnResourceProduced.AddDynamic(this, &UMingResourceDisplayWidget::OnResourceProduced);
        ResourceSystem->OnResourceInsufficient.AddDynamic(this, &UMingResourceDisplayWidget::OnResourceInsufficient);
    }
}

void UMingResourceDisplayWidget::UpdateResourceDisplay(EMingResourceType ResourceType, int32 Amount, int32 Capacity)
{
    FResourceDisplayItem* Item = GetResourceDisplayItem(ResourceType);
    if (Item)
    {
        UpdateSingleResourceItem(Item, ResourceType, Amount, Capacity);
    }
}

void UMingResourceDisplayWidget::UpdateAllResources(const TMap<EMingResourceType, int32>& Resources, const TMap<EMingResourceType, int32>& Capacities)
{
    for (const auto& ResourcePair : Resources)
    {
        EMingResourceType ResourceType = ResourcePair.Key;
        int32 Amount = ResourcePair.Value;
        
        const int32* Capacity = Capacities.Find(ResourceType);
        int32 CapacityValue = Capacity ? *Capacity : 999999;
        
        UpdateResourceDisplay(ResourceType, Amount, CapacityValue);
    }
}

void UMingResourceDisplayWidget::SetResourceHighlighted(EMingResourceType ResourceType, bool bHighlighted)
{
    FResourceDisplayItem* Item = GetResourceDisplayItem(ResourceType);
    if (Item && Item->ItemBorder)
    {
        if (bHighlighted)
        {
            Item->ItemBorder->SetBorderBackgroundColor(FLinearColor(0.3f, 0.3f, 0.0f, 0.9f));
        }
        else
        {
            Item->ItemBorder->SetBorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f));
        }
    }
}

void UMingResourceDisplayWidget::ShowResourceChangeAnimation(EMingResourceType ResourceType, int32 ChangeAmount)
{
    FResourceDisplayItem* Item = GetResourceDisplayItem(ResourceType);
    if (Item)
    {
        PlayResourceChangeEffect(Item, ChangeAmount);
    }
}

FResourceDisplayWidget::FResourceDisplayItem* UMingResourceDisplayWidget::GetResourceDisplayItem(EMingResourceType ResourceType)
{
    return ResourceDisplayMap.FindRef(ResourceType);
}

void UMingResourceDisplayWidget::UpdateSingleResourceItem(FResourceDisplayItem* Item, EMingResourceType ResourceType, int32 Amount, int32 Capacity)
{
    if (!Item)
    {
        return;
    }

    // 更新數量文字
    if (Item->ResourceAmountText)
    {
        FString AmountString = FString::Printf(TEXT("%d"), Amount);
        Item->ResourceAmountText->SetText(FText::FromString(AmountString));
    }

    // 更新進度條
    if (Item->ResourceProgressBar)
    {
        float Percentage = Capacity > 0 ? (float)Amount / (float)Capacity : 0.0f;
        Item->ResourceProgressBar->SetPercent(Percentage);
    }

    // 更新顏色
    UpdateResourceColor(Item, Capacity > 0 ? (float)Amount / (float)Capacity : 0.0f);
}

void UMingResourceDisplayWidget::UpdateResourceColor(FResourceDisplayItem* Item, float Percentage)
{
    if (!Item || !Item->ResourceAmountText)
    {
        return;
    }

    // 根據百分比設置顏色
    FLinearColor BaseColor = GetResourceDisplayColor(Item->ResourceType);
    
    if (Percentage > 0.7f)
    {
        Item->ResourceAmountText->SetColorAndOpacity(BaseColor);
    }
    else if (Percentage > 0.3f)
    {
        Item->ResourceAmountText->SetColorAndOpacity(FLinearColor::Yellow);
    }
    else
    {
        Item->ResourceAmountText->SetColorAndOpacity(FLinearColor::Red);
    }
}

FString UMingResourceDisplayWidget::GetResourceDisplayName(EMingResourceType ResourceType) const
{
    return UMingResourceSystem::GetResourceName(ResourceType);
}

FLinearColor UMingResourceDisplayWidget::GetResourceDisplayColor(EMingResourceType ResourceType) const
{
    return UMingResourceSystem::GetResourceColor(ResourceType);
}

void UMingResourceDisplayWidget::OnResourceChanged(EMingResourceType ResourceType, int32 NewAmount)
{
    if (ResourceSystem.IsValid())
    {
        int32 Capacity = ResourceSystem->GetResourceCapacity(ResourceType);
        UpdateResourceDisplay(ResourceType, NewAmount, Capacity);
        
        // 檢查是否有變化並播放動畫
        int32 LastAmount = LastResourceAmounts.FindRef(ResourceType);
        if (NewAmount != LastAmount)
        {
            ShowResourceChangeAnimation(ResourceType, NewAmount - LastAmount);
            LastResourceAmounts[ResourceType] = NewAmount;
        }
    }
}

void UMingResourceDisplayWidget::OnResourceProduced(EMingResourceType ResourceType, int32 Amount)
{
    // 資源生產時的視覺效果
    SetResourceHighlighted(ResourceType, true);
    
    // 設置計時器恢復正常顏色
    AnimationTimers[ResourceType] = 2.0f;
}

void UMingResourceDisplayWidget::OnResourceInsufficient(EMingResourceType ResourceType)
{
    // 資源不足時的視覺效果
    SetResourceHighlighted(ResourceType, true);
    
    // 設置計時器恢復正常顏色
    AnimationTimers[ResourceType] = 1.5f;
}

void UMingResourceDisplayWidget::PlayResourceChangeEffect(FResourceDisplayItem* Item, int32 ChangeAmount)
{
    if (!Item || !Item->ResourceAmountText)
    {
        return;
    }

    // 根據變化量設置顏色
    FLinearColor EffectColor;
    if (ChangeAmount > 0)
    {
        EffectColor = FLinearColor::Green; // 增加
    }
    else if (ChangeAmount < 0)
    {
        EffectColor = FLinearColor::Red; // 減少
    }
    else
    {
        return; // 無變化
    }

    // 設置效果顏色
    Item->ResourceAmountText->SetColorAndOpacity(EffectColor);
    
    // 設置恢復計時器
    AnimationTimers[Item->ResourceType] = 1.0f;
}
