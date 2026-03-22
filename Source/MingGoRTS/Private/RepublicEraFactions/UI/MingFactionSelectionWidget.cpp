// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 势力选择UI界面实现

#include "RepublicEraFactions/UI/MingFactionSelectionWidget.h"
#include "RepublicEraFactions/MingRTSFactionManager.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

// ==================== UMingFactionCardWidget ====================

void UMingFactionCardWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (SelectButton)
    {
        SelectButton->OnClicked.AddDynamic(this, &UMingFactionCardWidget::OnCardClicked);
        SelectButton->OnHovered.AddDynamic(this, &UMingFactionCardWidget::OnCardHovered);
        SelectButton->OnUnhovered.AddDynamic(this, &UMingFactionCardWidget::OnCardUnhovered);
    }
}

void UMingFactionCardWidget::SetFactionData(UMingRTSFactionBase* Faction)
{
    if (!Faction) return;
    
    AssociatedFaction = Faction;
    AssociatedFactionID = Faction->GetFactionAttributes().FactionID;
    
    FFactionAttributes Attrs = Faction->GetFactionAttributes();
    
    // 设置名称
    if (FactionNameText)
    {
        FactionNameText->SetText(Attrs.FactionName);
    }
    
    if (FactionEnglishNameText)
    {
        FactionEnglishNameText->SetText(Attrs.FactionNameEnglish);
    }
    
    // 设置难度
    if (DifficultyText)
    {
        FString DifficultyStr;
        switch (Attrs.Difficulty)
        {
            case EFactionDifficulty::VeryEasy: DifficultyStr = TEXT("★☆☆☆☆"); break;
            case EFactionDifficulty::Easy: DifficultyStr = TEXT("★★☆☆☆"); break;
            case EFactionDifficulty::Normal: DifficultyStr = TEXT("★★★☆☆"); break;
            case EFactionDifficulty::Hard: DifficultyStr = TEXT("★★★★☆"); break;
            case EFactionDifficulty::VeryHard: DifficultyStr = TEXT("★★★★★"); break;
            default: DifficultyStr = TEXT("★★★☆☆"); break;
        }
        DifficultyText->SetText(FText::FromString(DifficultyStr));
    }
    
    // 计算势力强度（基于起始资源）
    float MilitaryStrength = FMath::Clamp((float)Attrs.StartingPosition.InitialManpower / 800.0f, 0.1f, 1.0f);
    float EconomicStrength = FMath::Clamp((float)Attrs.StartingPosition.InitialGold / 2000.0f, 0.1f, 1.0f);
    float DiplomaticStrength = FMath::Clamp((float)Attrs.StartingPosition.InitialReputation / 80.0f, 0.1f, 1.0f);
    
    if (MilitaryPowerBar)
    {
        MilitaryPowerBar->SetPercent(MilitaryStrength);
    }
    
    if (EconomicPowerBar)
    {
        EconomicPowerBar->SetPercent(EconomicStrength);
    }
    
    if (DiplomaticPowerBar)
    {
        DiplomaticPowerBar->SetPercent(DiplomaticStrength);
    }
    
    // 设置势力颜色
    if (CardBorder)
    {
        FSlateBrush Brush;
        Brush.TintColor = FSlateColor(Attrs.FactionColor);
        CardBorder->SetBrushColor(Attrs.FactionColor);
    }
}

void UMingFactionCardWidget::SetSelected(bool bSelected)
{
    if (HighlightBorder)
    {
        HighlightBorder->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    
    if (CardBorder)
    {
        // 调整透明度表示选中状态
        FLinearColor Color = CardBorder->GetBrushColor();
        Color.A = bSelected ? 1.0f : 0.7f;
        CardBorder->SetBrushColor(Color);
    }
}

void UMingFactionCardWidget::OnCardClicked()
{
    OnFactionCardClicked.Broadcast(AssociatedFactionID);
}

void UMingFactionCardWidget::OnCardHovered()
{
    // 悬停效果
    if (CardBorder)
    {
        FLinearColor Color = CardBorder->GetBrushColor();
        Color.A = 0.9f;
        CardBorder->SetBrushColor(Color);
    }
}

void UMingFactionCardWidget::OnCardUnhovered()
{
    // 恢复默认透明度
    if (CardBorder)
    {
        FLinearColor Color = CardBorder->GetBrushColor();
        Color.A = HighlightBorder && HighlightBorder->GetVisibility() == ESlateVisibility::Visible ? 1.0f : 0.7f;
        CardBorder->SetBrushColor(Color);
    }
}

// ==================== UMingFactionDetailPanel ====================

void UMingFactionDetailPanel::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (ConfirmSelectionButton)
    {
        ConfirmSelectionButton->OnClicked.AddDynamic(this, &UMingFactionDetailPanel::OnConfirmClicked);
    }
    
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UMingFactionDetailPanel::OnBackClicked);
    }
}

void UMingFactionDetailPanel::ShowFactionDetails(UMingRTSFactionBase* Faction)
{
    if (!Faction) return;
    
    CurrentDisplayedFaction = Faction;
    FFactionAttributes Attrs = Faction->GetFactionAttributes();
    
    // 设置基本信息
    if (DetailFactionName)
    {
        FText FullName = FText::Format(FText::FromString(TEXT("{0} ({1})")), 
            Attrs.FactionName, Attrs.FactionNameEnglish);
        DetailFactionName->SetText(FullName);
    }
    
    if (CapitalCityText)
    {
        FText CapitalText = FText::Format(FText::FromString(TEXT("首都: {0}")), Attrs.StartingPosition.CapitalCity);
        CapitalCityText->SetText(CapitalText);
    }
    
    if (StartingYearText)
    {
        FText YearText = FText::Format(FText::FromString(TEXT("起始年份: {0}")), 
            FText::AsNumber(Attrs.StartingPosition.StartingYear));
        StartingYearText->SetText(YearText);
    }
    
    if (InitialResourcesText)
    {
        FText ResourcesText = FText::Format(FText::FromString(TEXT("起始资源: 黄金{0} | 人力{1} | 声望{2}")),
            FText::AsNumber(Attrs.StartingPosition.InitialGold),
            FText::AsNumber(Attrs.StartingPosition.InitialManpower),
            FText::AsNumber(Attrs.StartingPosition.InitialReputation));
        InitialResourcesText->SetText(ResourcesText);
    }
    
    // 清空并填充优势列表
    if (AdvantagesList)
    {
        AdvantagesList->ClearChildren();
        for (const FText& Advantage : Attrs.Traits.Advantages)
        {
            UTextBlock* AdvantageText = NewObject<UTextBlock>(this);
            AdvantageText->SetText(FText::Format(FText::FromString(TEXT("✓ {0}")), Advantage));
            AdvantageText->SetColorAndOpacity(FLinearColor(0.2f, 0.8f, 0.2f, 1.0f));
            AdvantagesList->AddChildToVerticalBox(AdvantageText);
        }
    }
    
    // 清空并填充劣势列表
    if (DisadvantagesList)
    {
        DisadvantagesList->ClearChildren();
        for (const FText& Disadvantage : Attrs.Traits.Disadvantages)
        {
            UTextBlock* DisadvantageText = NewObject<UTextBlock>(this);
            DisadvantageText->SetText(FText::Format(FText::FromString(TEXT("✗ {0}")), Disadvantage));
            DisadvantageText->SetColorAndOpacity(FLinearColor(0.9f, 0.2f, 0.2f, 1.0f));
            DisadvantagesList->AddChildToVerticalBox(DisadvantageText);
        }
    }
    
    // 清空并填充专属单位列表
    if (UniqueUnitsList)
    {
        UniqueUnitsList->ClearChildren();
        TArray<FFactionUnit> Units = Faction->GetUniqueUnits();
        for (const FFactionUnit& Unit : Units)
        {
            UTextBlock* UnitText = NewObject<UTextBlock>(this);
            UnitText->SetText(FText::Format(FText::FromString(TEXT("• {0}: {1}")), 
                Unit.UnitName, Unit.Description));
            UniqueUnitsList->AddChildToVerticalBox(UnitText);
        }
    }
    
    // 清空并填充独特机制列表
    if (UniqueMechanicsList)
    {
        UniqueMechanicsList->ClearChildren();
        TArray<FFactionMechanic> Mechanics = Faction->GetUniqueMechanics();
        for (const FFactionMechanic& Mechanic : Mechanics)
        {
            UTextBlock* MechanicText = NewObject<UTextBlock>(this);
            FText MechanicInfo = FText::Format(FText::FromString(TEXT("• {0}: {1} (效果×{2})"))),
                Mechanic.MechanicName, Mechanic.Description, FText::AsNumber(Mechanic.EffectMultiplier));
            MechanicText->SetText(MechanicInfo);
            UniqueMechanicsList->AddChildToVerticalBox(MechanicText);
        }
    }
    
    // 设置游戏指南
    if (GameplayGuideText)
    {
        FGameplayGuide Guide = Faction->GetGameplayGuide();
        FText GuideInfo = FText::Format(FText::FromString(TEXT(
            "【早期策略】{0}\n\n"
            "【中期策略】{1}\n\n"
            "【后期策略】{2}\n\n"
            "【军事战术】{3}\n\n"
            "【外交建议】{4}")),
            Guide.EarlyGameStrategy,
            Guide.MidGameStrategy,
            Guide.LateGameStrategy,
            Guide.MilitaryTactics,
            Guide.DiplomaticAdvice);
        GameplayGuideText->SetText(GuideInfo);
    }
    
    // 显示面板
    SetVisibility(ESlateVisibility::Visible);
}

void UMingFactionDetailPanel::ClearDetails()
{
    CurrentDisplayedFaction = nullptr;
    
    if (DetailFactionName) DetailFactionName->SetText(FText::GetEmpty());
    if (CapitalCityText) CapitalCityText->SetText(FText::GetEmpty());
    if (StartingYearText) StartingYearText->SetText(FText::GetEmpty());
    if (InitialResourcesText) InitialResourcesText->SetText(FText::GetEmpty());
    if (GameplayGuideText) GameplayGuideText->SetText(FText::GetEmpty());
    
    if (AdvantagesList) AdvantagesList->ClearChildren();
    if (DisadvantagesList) DisadvantagesList->ClearChildren();
    if (UniqueUnitsList) UniqueUnitsList->ClearChildren();
    if (UniqueMechanicsList) UniqueMechanicsList->ClearChildren();
    
    SetVisibility(ESlateVisibility::Hidden);
}

void UMingFactionDetailPanel::OnConfirmClicked()
{
    OnConfirmSelection.Broadcast();
}

void UMingFactionDetailPanel::OnBackClicked()
{
    OnBackClicked.Broadcast();
}

// ==================== UMingFactionSelectionWidget ====================

UMingFactionSelectionWidget::UMingFactionSelectionWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UMingFactionSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 初始化势力管理器
    FactionManager = NewObject<UMingRTSFactionManager>(this);
    if (FactionManager)
    {
        FactionManager->InitializeFactionManager();
    }
    
    // 绑定按钮事件
    if (FilterEasyButton)
    {
        FilterEasyButton->OnClicked.AddDynamic(this, &UMingFactionSelectionWidget::OnFilterEasyClicked);
    }
    
    if (FilterNormalButton)
    {
        FilterNormalButton->OnClicked.AddDynamic(this, &UMingFactionSelectionWidget::OnFilterNormalClicked);
    }
    
    if (FilterHardButton)
    {
        FilterHardButton->OnClicked.AddDynamic(this, &UMingFactionSelectionWidget::OnFilterHardClicked);
    }
    
    if (FilterAllButton)
    {
        FilterAllButton->OnClicked.AddDynamic(this, &UMingFactionSelectionWidget::OnFilterAllClicked);
    }
    
    if (RandomSelectButton)
    {
        RandomSelectButton->OnClicked.AddDynamic(this, &UMingFactionSelectionWidget::OnRandomSelectClicked);
    }
    
    // 初始化详情面板回调
    if (DetailPanel)
    {
        DetailPanel->OnConfirmSelection.AddDynamic(this, &UMingFactionSelectionWidget::OnDetailConfirmClicked);
    }
    
    // 初始化势力列表
    InitializeFactionSelection();
}

void UMingFactionSelectionWidget::NativeDestruct()
{
    // 清理资源
    for (auto Card : FactionCards)
    {
        if (Card)
        {
            Card->OnFactionCardClicked.Clear();
        }
    }
    FactionCards.Empty();
    
    Super::NativeDestruct();
}

void UMingFactionSelectionWidget::InitializeFactionSelection()
{
    // 设置标题
    if (TitleText)
    {
        TitleText->SetText(FText::FromString(TEXT("选择你的势力")));
    }
    
    if (SubtitleText)
    {
        SubtitleText->SetText(FText::FromString(TEXT("选择12个民国势力之一开始你的征程")));
    }
    
    // 刷新势力列表
    RefreshFactionList();
    
    // 默认显示详情面板
    if (DetailPanel)
    {
        DetailPanel->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMingFactionSelectionWidget::RefreshFactionList()
{
    if (!FactionCardsContainer) return;
    
    // 清空现有卡片
    FactionCardsContainer->ClearChildren();
    FactionCards.Empty();
    
    if (!FactionManager) return;
    
    // 获取所有势力
    TArray<UMingRTSFactionBase*> AllFactions = FactionManager->GetAllFactions();
    
    // 按难度排序
    AllFactions.Sort([](UMingRTSFactionBase* A, UMingRTSFactionBase* B)
    {
        return (int32)A->GetFactionAttributes().Difficulty < (int32)B->GetFactionAttributes().Difficulty;
    });
    
    // 创建势力卡片
    for (UMingRTSFactionBase* Faction : AllFactions)
    {
        if (Faction && ShouldShowFaction(Faction))
        {
            CreateFactionCard(Faction);
        }
    }
}

bool UMingFactionSelectionWidget::ShouldShowFaction(UMingRTSFactionBase* Faction)
{
    if (!Faction) return false;
    
    // 如果没有筛选或筛选为全部，显示所有
    if (CurrentFilter == EFactionDifficulty::Normal && FilterAllButton && FilterAllButton->IsPressed())
    {
        return true;
    }
    
    return Faction->GetFactionAttributes().Difficulty == CurrentFilter;
}

void UMingFactionSelectionWidget::CreateFactionCard(UMingRTSFactionBase* Faction)
{
    if (!Faction || !FactionCardsContainer) return;
    
    // 创建卡片widget（假设有对应的Blueprint类）
    TSubclassOf<UMingFactionCardWidget> CardWidgetClass = LoadClass<UMingFactionCardWidget>(nullptr, 
        TEXT("/Game/UI/FactionCardWidget.FactionCardWidget_C"));
    
    if (!CardWidgetClass)
    {
        // 如果没有Blueprint，使用默认创建
        UE_LOG(LogTemp, Warning, TEXT("FactionCardWidget Blueprint not found"));
        return;
    }
    
    UMingFactionCardWidget* CardWidget = CreateWidget<UMingFactionCardWidget>(GetWorld(), CardWidgetClass);
    if (CardWidget)
    {
        CardWidget->SetFactionData(Faction);
        CardWidget->OnFactionCardClicked.AddDynamic(this, &UMingFactionSelectionWidget::OnFactionCardClicked);
        
        UHorizontalBoxSlot* Slot = FactionCardsContainer->AddChildToHorizontalBox(CardWidget);
        if (Slot)
        {
            Slot->SetPadding(FMargin(10.0f, 5.0f, 10.0f, 5.0f));
            Slot->SetSize(ESlateSizeRule::Automatic);
        }
        
        FactionCards.Add(CardWidget);
    }
}

void UMingFactionSelectionWidget::OnFactionCardClicked(FName FactionID)
{
    if (!FactionManager) return;
    
    // 查找选中的势力
    UMingRTSFactionBase* SelectedFaction = FactionManager->GetFactionByID(FactionID);
    if (SelectedFaction)
    {
        CurrentlySelectedFaction = SelectedFaction;
        
        // 更新选中显示
        UpdateSelectionVisuals();
        
        // 显示详情
        if (DetailPanel)
        {
            DetailPanel->ShowFactionDetails(SelectedFaction);
        }
        
        // 更新选中文本
        if (SelectedFactionText)
        {
            FText SelectedText = FText::Format(FText::FromString(TEXT("已选择: {0}")), 
                SelectedFaction->GetFactionAttributes().FactionName);
            SelectedFactionText->SetText(SelectedText);
        }
    }
}

void UMingFactionSelectionWidget::UpdateSelectionVisuals()
{
    // 更新所有卡片的选中状态
    for (UMingFactionCardWidget* Card : FactionCards)
    {
        if (Card && CurrentlySelectedFaction)
        {
            bool bIsSelected = Card->GetFactionID() == CurrentlySelectedFaction->GetFactionAttributes().FactionID;
            Card->SetSelected(bIsSelected);
        }
    }
}

void UMingFactionSelectionWidget::FilterByDifficulty(EFactionDifficulty Difficulty)
{
    CurrentFilter = Difficulty;
    RefreshFactionList();
}

void UMingFactionSelectionWidget::ShowAllFactions()
{
    CurrentFilter = EFactionDifficulty::Normal; // 使用Normal作为默认值，实际不过滤
    RefreshFactionList();
}

void UMingFactionSelectionWidget::RandomSelectFaction()
{
    if (!FactionManager || FactionCards.Num() == 0) return;
    
    // 随机选择一个势力
    int32 RandomIndex = FMath::RandRange(0, FactionCards.Num() - 1);
    UMingFactionCardWidget* RandomCard = FactionCards[RandomIndex];
    
    if (RandomCard)
    {
        OnFactionCardClicked(RandomCard->GetFactionID());
    }
}

UMingRTSFactionBase* UMingFactionSelectionWidget::GetSelectedFaction() const
{
    return CurrentlySelectedFaction;
}

void UMingFactionSelectionWidget::ConfirmFactionSelection()
{
    if (CurrentlySelectedFaction)
    {
        OnFactionSelected.Broadcast(CurrentlySelectedFaction);
    }
}

void UMingFactionSelectionWidget::OnFilterEasyClicked()
{
    FilterByDifficulty(EFactionDifficulty::Easy);
}

void UMingFactionSelectionWidget::OnFilterNormalClicked()
{
    FilterByDifficulty(EFactionDifficulty::Normal);
}

void UMingFactionSelectionWidget::OnFilterHardClicked()
{
    FilterByDifficulty(EFactionDifficulty::Hard);
}

void UMingFactionSelectionWidget::OnFilterAllClicked()
{
    ShowAllFactions();
}

void UMingFactionSelectionWidget::OnRandomSelectClicked()
{
    RandomSelectFaction();
}

void UMingFactionSelectionWidget::OnDetailConfirmClicked()
{
    ConfirmFactionSelection();
}
