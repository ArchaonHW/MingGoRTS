#include "MingReputationPanel.h"
#include "MingRelationshipManager.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Engine/Engine.h"

UMingReputationPanel::UMingReputationPanel()
{
    bIsUpdating = false;
    CurrentFilter = EReputationFilter::All;
    CurrentSortType = EReputationSortType::ByName;
    SelectedRegionID = TEXT("");
}

void UMingReputationPanel::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 綁定按鈕事件
    if (FilterAllButton)
    {
        FilterAllButton->OnClicked.AddDynamic(this, &UMingReputationPanel::OnFilterAllClicked);
    }
    
    if (FilterHighButton)
    {
        FilterHighButton->OnClicked.AddDynamic(this, &UMingReputationPanel::OnFilterHighClicked);
    }
    
    if (FilterMediumButton)
    {
        FilterMediumButton->OnClicked.AddDynamic(this, &UMingReputationPanel::OnFilterMediumClicked);
    }
    
    if (FilterLowButton)
    {
        FilterLowButton->OnClicked.AddDynamic(this, &UMingReputationPanel::OnFilterLowClicked);
    }
    
    if (SortByNameButton)
    {
        SortByNameButton->OnClicked.AddDynamic(this, &UMingReputationPanel::OnSortByNameClicked);
    }
    
    if (SortByValueButton)
    {
        SortByValueButton->OnClicked.AddDynamic(this, &UMingReputationPanel::OnSortByValueClicked);
    }
    
    if (SortByChangeButton)
    {
        SortByChangeButton->OnClicked.AddDynamic(this, &UMingReputationPanel::OnSortByChangeClicked);
    }
    
    if (RefreshButton)
    {
        RefreshButton->OnClicked.AddDynamic(this, &UMingReputationPanel::OnRefreshClicked);
    }
    
    // 設置默認過濾器和排序
    UpdateFilterButtons();
    UpdateSortButtons();
}

void UMingReputationPanel::UpdateReputationData(UMingRelationshipManager* InRelationshipManager)
{
    if (bIsUpdating || !InRelationshipManager)
    {
        return;
    }
    
    bIsUpdating = true;
    RelationshipManager = InRelationshipManager;
    
    // 獲取所有聲望數據
    TArray<FReputationData> AllReputations = RelationshipManager->GetAllReputations();
    
    // 應用過濾器
    TArray<FReputationData> FilteredReputations = ApplyFilter(AllReputations, CurrentFilter);
    
    // 應用排序
    TArray<FReputationData> SortedReputations = ApplySort(FilteredReputations, CurrentSortType);
    
    // 更新UI顯示
    UpdateReputationList(SortedReputations);
    
    // 更新統計信息
    UpdateStatistics(AllReputations);
    
    // 更新地圖顯示
    UpdateReputationMap(AllReputations);
    
    bIsUpdating = false;
    
    UE_LOG(LogTemp, Log, TEXT("聲望面板數據已更新，顯示 %d 個地區聲望"), SortedReputations.Num());
}

void UMingReputationPanel::SetReputationFilter(EReputationFilter Filter)
{
    if (CurrentFilter != Filter)
    {
        CurrentFilter = Filter;
        UpdateFilterButtons();
        
        if (RelationshipManager)
        {
            UpdateReputationData(RelationshipManager);
        }
    }
}

void UMingReputationPanel::SetReputationSort(EReputationSortType SortType)
{
    if (CurrentSortType != SortType)
    {
        CurrentSortType = SortType;
        UpdateSortButtons();
        
        if (RelationshipManager)
        {
            UpdateReputationData(RelationshipManager);
        }
    }
}

void UMingReputationPanel::SelectRegion(const FString& RegionID)
{
    if (SelectedRegionID == RegionID)
    {
        return;
    }
    
    SelectedRegionID = RegionID;
    
    // 更新詳細信息面板
    UpdateRegionDetails(RegionID);
    
    // 更新選中狀態
    UpdateSelectionState();
    
    UE_LOG(LogTemp, Log, TEXT("已選中地區：%s"), *RegionID);
}

void UMingReputationPanel::ShowRegionDetails(const FString& RegionID)
{
    SelectRegion(RegionID);
    
    // 顯示詳細信息面板
    if (DetailsPanel)
    {
        DetailsPanel->SetVisibility(ESlateVisibility::Visible);
        PlayDetailsAnimation(true);
    }
}

void UMingReputationPanel::HideRegionDetails()
{
    SelectedRegionID = TEXT("");
    
    // 隱藏詳細信息面板
    if (DetailsPanel)
    {
        PlayDetailsAnimation(false);
        // 延遲隱藏以等待動畫完成
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (DetailsPanel)
            {
                DetailsPanel->SetVisibility(ESlateVisibility::Hidden);
            }
        }, 0.3f, false);
    }
}

void UMingReputationPanel::RefreshReputations()
{
    if (RelationshipManager)
    {
        UpdateReputationData(RelationshipManager);
        PlayRefreshAnimation();
    }
}

void UMingReputationPanel::ShowReputationEffects(const FString& RegionID)
{
    if (!RelationshipManager || RegionID.IsEmpty())
    {
        return;
    }
    
    // 獲取聲望影響
    TArray<FReputationEffect> Effects = RelationshipManager->GetReputationEffects(RegionID);
    
    // 顯示影響列表
    UpdateEffectsList(Effects);
    
    // 顯示影響面板
    if (EffectsPanel)
    {
        EffectsPanel->SetVisibility(ESlateVisibility::Visible);
        PlayEffectsAnimation(true);
    }
}

void UMingReputationPanel::HideReputationEffects()
{
    if (EffectsPanel)
    {
        PlayEffectsAnimation(false);
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (EffectsPanel)
            {
                EffectsPanel->SetVisibility(ESlateVisibility::Hidden);
            }
        }, 0.3f, false);
    }
}

// 按鈕事件處理
void UMingReputationPanel::OnFilterAllClicked()
{
    SetReputationFilter(EReputationFilter::All);
}

void UMingReputationPanel::OnFilterHighClicked()
{
    SetReputationFilter(EReputationFilter::High);
}

void UMingReputationPanel::OnFilterMediumClicked()
{
    SetReputationFilter(EReputationFilter::Medium);
}

void UMingReputationPanel::OnFilterLowClicked()
{
    SetReputationFilter(EReputationFilter::Low);
}

void UMingReputationPanel::OnSortByNameClicked()
{
    SetReputationSort(EReputationSortType::ByName);
}

void UMingReputationPanel::OnSortByValueClicked()
{
    SetReputationSort(EReputationSortType::ByValue);
}

void UMingReputationPanel::OnSortByChangeClicked()
{
    SetReputationSort(EReputationSortType::ByChange);
}

void UMingReputationPanel::OnRefreshClicked()
{
    RefreshReputations();
}

void UMingReputationPanel::OnRegionClicked(const FString& RegionID)
{
    ShowRegionDetails(RegionID);
}

void UMingReputationPanel::OnReputationChanged(const FString& RegionID, float OldValue, float NewValue)
{
    // 如果當前顯示的聲望數據發生變化，更新UI
    if (RelationshipManager && !bIsUpdating)
    {
        UpdateReputationData(RelationshipManager);
    }
}

// 私有輔助函數
TArray<FReputationData> UMingReputationPanel::ApplyFilter(const TArray<FReputationData>& Reputations, EReputationFilter Filter)
{
    TArray<FReputationData> FilteredReputations;
    
    for (const FReputationData& Reputation : Reputations)
    {
        bool bShouldInclude = false;
        
        switch (Filter)
        {
            case EReputationFilter::All:
                bShouldInclude = true;
                break;
                
            case EReputationFilter::High:
                bShouldInclude = Reputation.ReputationValue > 75.0f;
                break;
                
            case EReputationFilter::Medium:
                bShouldInclude = Reputation.ReputationValue >= 25.0f && Reputation.ReputationValue <= 75.0f;
                break;
                
            case EReputationFilter::Low:
                bShouldInclude = Reputation.ReputationValue < 25.0f;
                break;
        }
        
        if (bShouldInclude)
        {
            FilteredReputations.Add(Reputation);
        }
    }
    
    return FilteredReputations;
}

TArray<FReputationData> UMingReputationPanel::ApplySort(const TArray<FReputationData>& Reputations, EReputationSortType SortType)
{
    TArray<FReputationData> SortedReputations = Reputations;
    
    switch (SortType)
    {
        case EReputationSortType::ByName:
            SortedReputations.Sort([](const FReputationData& A, const FReputationData& B)
            {
                return A.RegionName < B.RegionName;
            });
            break;
            
        case EReputationSortType::ByValue:
            SortedReputations.Sort([](const FReputationData& A, const FReputationData& B)
            {
                return A.ReputationValue > B.ReputationValue;
            });
            break;
            
        case EReputationSortType::ByChange:
            SortedReputations.Sort([](const FReputationData& A, const FReputationData& B)
            {
                return A.RecentChange > B.RecentChange;
            });
            break;
    }
    
    return SortedReputations;
}

void UMingReputationPanel::UpdateReputationList(const TArray<FReputationData>& Reputations)
{
    if (!ReputationListScrollBox)
    {
        return;
    }
    
    // 清空現有列表
    ReputationListScrollBox->ClearChildren();
    
    // 創建聲望項目
    for (const FReputationData& Reputation : Reputations)
    {
        UWidget* ReputationItem = CreateReputationItem(Reputation);
        if (ReputationItem)
        {
            ReputationListScrollBox->AddChild(ReputationItem);
        }
    }
    
    // 更新計數
    if (RegionCountText)
    {
        RegionCountText->SetText(FText::FromString(FString::Printf(TEXT("共 %d 個地區"), Reputations.Num())));
    }
}

UWidget* UMingReputationPanel::CreateReputationItem(const FReputationData& Reputation)
{
    // 這裡應該創建一個自定義的聲望項目Widget
    // 暫時返回一個簡單的文本塊作為示例
    UTextBlock* TextBlock = NewObject<UTextBlock>(this);
    if (TextBlock)
    {
        FString ItemText = FString::Printf(TEXT("%s: %.1f"), *Reputation.RegionName, Reputation.ReputationValue);
        TextBlock->SetText(FText::FromString(ItemText));
        
        // 設置字體和樣式
        FSlateFontInfo FontInfo = TextBlock->GetFont();
        FontInfo.Size = 14;
        TextBlock->SetFont(FontInfo);
        
        // 設置邊距
        TextBlock->SetMargin(FMargin(10.0f, 5.0f));
    }
    
    return TextBlock;
}

void UMingReputationPanel::UpdateRegionDetails(const FString& RegionID)
{
    if (!RelationshipManager || RegionID.IsEmpty())
    {
        return;
    }
    
    // 獲取地區詳細信息
    FReputationData ReputationData = RelationshipManager->GetReputation(RegionID);
    
    // 更新詳細信息面板
    if (RegionNameText)
    {
        RegionNameText->SetText(FText::FromString(ReputationData.RegionName));
    }
    
    if (ReputationValueText)
    {
        ReputationValueText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ReputationData.ReputationValue)));
    }
    
    if (ReputationProgressBar)
    {
        float NormalizedValue = (ReputationData.ReputationValue + 100.0f) / 200.0f; // -100 到 100 映射到 0 到 1
        ReputationProgressBar->SetPercent(FMath::Clamp(NormalizedValue, 0.0f, 1.0f));
    }
    
    if (RecentChangeText)
    {
        FString ChangeText = FString::Printf(TEXT("%+.1f"), ReputationData.RecentChange);
        RecentChangeText->SetText(FText::FromString(ChangeText));
        
        // 設置顏色（正數為綠色，負數為紅色）
        FSlateColor Color = ReputationData.RecentChange >= 0 ? FLinearColor::Green : FLinearColor::Red;
        RecentChangeText->SetColorAndOpacity(Color);
    }
    
    if (InfluenceLevelText)
    {
        InfluenceLevelText->SetText(FText::FromString(GetInfluenceLevelText(ReputationData.ReputationValue)));
    }
    
    if (PopulationText)
    {
        PopulationText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ReputationData.Population)));
    }
    
    if (EconomyText)
    {
        EconomyText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ReputationData.EconomyLevel)));
    }
    
    if (StabilityText)
    {
        StabilityText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ReputationData.StabilityLevel)));
    }
}

void UMingReputationPanel::UpdateStatistics(const TArray<FReputationData>& Reputations)
{
    int32 HighCount = 0;
    int32 MediumCount = 0;
    int32 LowCount = 0;
    float TotalValue = 0.0f;
    
    for (const FReputationData& Reputation : Reputations)
    {
        TotalValue += Reputation.ReputationValue;
        
        if (Reputation.ReputationValue > 75.0f)
        {
            HighCount++;
        }
        else if (Reputation.ReputationValue >= 25.0f)
        {
            MediumCount++;
        }
        else
        {
            LowCount++;
        }
    }
    
    // 更新統計文本
    if (HighCountText)
    {
        HighCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), HighCount)));
    }
    
    if (MediumCountText)
    {
        MediumCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), MediumCount)));
    }
    
    if (LowCountText)
    {
        LowCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), LowCount)));
    }
    
    if (AverageValueText)
    {
        float AverageValue = Reputations.Num() > 0 ? TotalValue / Reputations.Num() : 0.0f;
        AverageValueText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), AverageValue)));
    }
}

void UMingReputationPanel::UpdateReputationMap(const TArray<FReputationData>& Reputations)
{
    if (!ReputationMapImage)
    {
        return;
    }
    
    // 這裡應該更新地圖顯示，根據聲望值設置不同顏色
    // 暫時記錄日誌
    UE_LOG(LogTemp, Log, TEXT("更新聲望地圖，地區數量：%d"), Reputations.Num());
}

void UMingReputationPanel::UpdateEffectsList(const TArray<FReputationEffect>& Effects)
{
    if (!EffectsListScrollBox)
    {
        return;
    }
    
    // 清空現有效果列表
    EffectsListScrollBox->ClearChildren();
    
    // 創建效果項目
    for (const FReputationEffect& Effect : Effects)
    {
        UWidget* EffectItem = CreateEffectItem(Effect);
        if (EffectItem)
        {
            EffectsListScrollBox->AddChild(EffectItem);
        }
    }
}

UWidget* UMingReputationPanel::CreateEffectItem(const FReputationEffect& Effect)
{
    UTextBlock* TextBlock = NewObject<UTextBlock>(this);
    if (TextBlock)
    {
        FString EffectText = FString::Printf(TEXT("%s: %s"), *Effect.EffectName, *Effect.Description);
        TextBlock->SetText(FText::FromString(EffectText));
        
        FSlateFontInfo FontInfo = TextBlock->GetFont();
        FontInfo.Size = 12;
        TextBlock->SetFont(FontInfo);
        TextBlock->SetMargin(FMargin(5.0f, 2.0f));
        
        // 根據效果類型設置顏色
        FSlateColor Color = GetEffectColor(Effect.EffectType);
        TextBlock->SetColorAndOpacity(Color);
    }
    
    return TextBlock;
}

void UMingReputationPanel::UpdateFilterButtons()
{
    // 更新過濾器按鈕的視覺狀態
    if (FilterAllButton)
    {
        FilterAllButton->SetIsEnabled(CurrentFilter != EReputationFilter::All);
    }
    
    if (FilterHighButton)
    {
        FilterHighButton->SetIsEnabled(CurrentFilter != EReputationFilter::High);
    }
    
    if (FilterMediumButton)
    {
        FilterMediumButton->SetIsEnabled(CurrentFilter != EReputationFilter::Medium);
    }
    
    if (FilterLowButton)
    {
        FilterLowButton->SetIsEnabled(CurrentFilter != EReputationFilter::Low);
    }
}

void UMingReputationPanel::UpdateSortButtons()
{
    // 更新排序按鈕的視覺狀態
    if (SortByNameButton)
    {
        SortByNameButton->SetIsEnabled(CurrentSortType != EReputationSortType::ByName);
    }
    
    if (SortByValueButton)
    {
        SortByValueButton->SetIsEnabled(CurrentSortType != EReputationSortType::ByValue);
    }
    
    if (SortByChangeButton)
    {
        SortByChangeButton->SetIsEnabled(CurrentSortType != EReputationSortType::ByChange);
    }
}

void UMingReputationPanel::UpdateSelectionState()
{
    // 更新列表項目的選中狀態
    // 這裡需要遍歷所有聲望項目並設置選中狀態
    // 暫時留空，待實現
}

void UMingReputationPanel::PlayDetailsAnimation(bool bShow)
{
    // 播放詳細信息面板的顯示/隱藏動畫
    if (DetailsPanel)
    {
        // 這裡應該實現實際的動畫邏輯
        UE_LOG(LogTemp, Log, TEXT("播放詳細信息面板動畫：%s"), bShow ? TEXT("顯示") : TEXT("隱藏"));
    }
}

void UMingReputationPanel::PlayEffectsAnimation(bool bShow)
{
    // 播放效果面板的顯示/隱藏動畫
    if (EffectsPanel)
    {
        UE_LOG(LogTemp, Log, TEXT("播放效果面板動畫：%s"), bShow ? TEXT("顯示") : TEXT("隱藏"));
    }
}

void UMingReputationPanel::PlayRefreshAnimation()
{
    // 播放刷新動畫
    if (RefreshButton)
    {
        // 這裡應該實現實際的旋轉動畫
        UE_LOG(LogTemp, Log, TEXT("播放刷新動畫"));
    }
}

FString UMingReputationPanel::GetInfluenceLevelText(float Value)
{
    if (Value > 90.0f)
    {
        return TEXT("極高影響力");
    }
    else if (Value > 70.0f)
    {
        return TEXT("高影響力");
    }
    else if (Value > 50.0f)
    {
        return TEXT("中等影響力");
    }
    else if (Value > 30.0f)
    {
        return TEXT("低影響力");
    }
    else
    {
        return TEXT("極低影響力");
    }
}

FSlateColor UMingReputationPanel::GetReputationValueColor(float Value)
{
    if (Value > 75.0f)
    {
        return FLinearColor::Green;
    }
    else if (Value > 50.0f)
    {
        return FLinearColor(0.0f, 0.7f, 0.0f); // 深綠色
    }
    else if (Value > 25.0f)
    {
        return FLinearColor::Yellow;
    }
    else if (Value > 0.0f)
    {
        return FLinearColor::Orange;
    }
    else
    {
        return FLinearColor::Red;
    }
}

FSlateColor UMingReputationPanel::GetEffectColor(EReputationEffectType EffectType)
{
    switch (EffectType)
    {
        case EReputationEffectType::Positive:
            return FLinearColor::Green;
        case EReputationEffectType::Negative:
            return FLinearColor::Red;
        case EReputationEffectType::Neutral:
            return FLinearColor::Gray;
        default:
            return FLinearColor::White;
    }
}
