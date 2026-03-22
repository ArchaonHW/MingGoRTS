#include "MingRelationshipPanel.h"
#include "MingRelationshipManager.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Engine/Engine.h"

UMingRelationshipPanel::UMingRelationshipPanel()
{
    bIsUpdating = false;
    CurrentFilter = ERelationshipFilter::All;
    CurrentSortType = ERelationshipSortType::ByName;
    SelectedCharacterID = TEXT("");
}

void UMingRelationshipPanel::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 綁定按鈕事件
    if (FilterAllButton)
    {
        FilterAllButton->OnClicked.AddDynamic(this, &UMingRelationshipPanel::OnFilterAllClicked);
    }
    
    if (FilterFriendsButton)
    {
        FilterFriendsButton->OnClicked.AddDynamic(this, &UMingRelationshipPanel::OnFilterFriendsClicked);
    }
    
    if (FilterEnemiesButton)
    {
        FilterEnemiesButton->OnClicked.AddDynamic(this, &UMingRelationshipPanel::OnFilterEnemiesClicked);
    }
    
    if (FilterNeutralButton)
    {
        FilterNeutralButton->OnClicked.AddDynamic(this, &UMingRelationshipPanel::OnFilterNeutralClicked);
    }
    
    if (SortByNameButton)
    {
        SortByNameButton->OnClicked.AddDynamic(this, &UMingRelationshipPanel::OnSortByNameClicked);
    }
    
    if (SortByValueButton)
    {
        SortByValueButton->OnClicked.AddDynamic(this, &UMingRelationshipPanel::OnSortByValueClicked);
    }
    
    if (SortByChangeButton)
    {
        SortByChangeButton->OnClicked.AddDynamic(this, &UMingRelationshipPanel::OnSortByChangeClicked);
    }
    
    if (RefreshButton)
    {
        RefreshButton->OnClicked.AddDynamic(this, &UMingRelationshipPanel::OnRefreshClicked);
    }
    
    // 設置默認過濾器和排序
    UpdateFilterButtons();
    UpdateSortButtons();
}

void UMingRelationshipPanel::UpdateRelationshipData(UMingRelationshipManager* InRelationshipManager)
{
    if (bIsUpdating || !InRelationshipManager)
    {
        return;
    }
    
    bIsUpdating = true;
    RelationshipManager = InRelationshipManager;
    
    // 獲取所有關係數據
    TArray<FRelationshipData> AllRelationships = RelationshipManager->GetAllRelationships();
    
    // 應用過濾器
    TArray<FRelationshipData> FilteredRelationships = ApplyFilter(AllRelationships, CurrentFilter);
    
    // 應用排序
    TArray<FRelationshipData> SortedRelationships = ApplySort(FilteredRelationships, CurrentSortType);
    
    // 更新UI顯示
    UpdateRelationshipList(SortedRelationships);
    
    // 更新統計信息
    UpdateStatistics(AllRelationships);
    
    bIsUpdating = false;
    
    UE_LOG(LogTemp, Log, TEXT("關係面板數據已更新，顯示 %d 個關係"), SortedRelationships.Num());
}

void UMingRelationshipPanel::SetRelationshipFilter(ERelationshipFilter Filter)
{
    if (CurrentFilter != Filter)
    {
        CurrentFilter = Filter;
        UpdateFilterButtons();
        
        if (RelationshipManager)
        {
            UpdateRelationshipData(RelationshipManager);
        }
    }
}

void UMingRelationshipPanel::SetRelationshipSort(ERelationshipSortType SortType)
{
    if (CurrentSortType != SortType)
    {
        CurrentSortType = SortType;
        UpdateSortButtons();
        
        if (RelationshipManager)
        {
            UpdateRelationshipData(RelationshipManager);
        }
    }
}

void UMingRelationshipPanel::SelectCharacter(const FString& CharacterID)
{
    if (SelectedCharacterID == CharacterID)
    {
        return;
    }
    
    SelectedCharacterID = CharacterID;
    
    // 更新詳細信息面板
    UpdateCharacterDetails(CharacterID);
    
    // 更新選中狀態
    UpdateSelectionState();
    
    UE_LOG(LogTemp, Log, TEXT("已選中角色：%s"), *CharacterID);
}

void UMingRelationshipPanel::ShowCharacterDetails(const FString& CharacterID)
{
    SelectCharacter(CharacterID);
    
    // 顯示詳細信息面板
    if (DetailsPanel)
    {
        DetailsPanel->SetVisibility(ESlateVisibility::Visible);
        PlayDetailsAnimation(true);
    }
}

void UMingRelationshipPanel::HideCharacterDetails()
{
    SelectedCharacterID = TEXT("");
    
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

void UMingRelationshipPanel::RefreshRelationships()
{
    if (RelationshipManager)
    {
        UpdateRelationshipData(RelationshipManager);
        PlayRefreshAnimation();
    }
}

// 按鈕事件處理
void UMingRelationshipPanel::OnFilterAllClicked()
{
    SetRelationshipFilter(ERelationshipFilter::All);
}

void UMingRelationshipPanel::OnFilterFriendsClicked()
{
    SetRelationshipFilter(ERelationshipFilter::Friends);
}

void UMingRelationshipPanel::OnFilterEnemiesClicked()
{
    SetRelationshipFilter(ERelationshipFilter::Enemies);
}

void UMingRelationshipPanel::OnFilterNeutralClicked()
{
    SetRelationshipFilter(ERelationshipFilter::Neutral);
}

void UMingRelationshipPanel::OnSortByNameClicked()
{
    SetRelationshipSort(ERelationshipSortType::ByName);
}

void UMingRelationshipPanel::OnSortByValueClicked()
{
    SetRelationshipSort(ERelationshipSortType::ByValue);
}

void UMingRelationshipPanel::OnSortByChangeClicked()
{
    SetRelationshipSort(ERelationshipSortType::ByChange);
}

void UMingRelationshipPanel::OnRefreshClicked()
{
    RefreshRelationships();
}

void UMingRelationshipPanel::OnCharacterClicked(const FString& CharacterID)
{
    ShowCharacterDetails(CharacterID);
}

void UMingRelationshipPanel::OnRelationshipValueChanged(const FString& CharacterID, float OldValue, float NewValue)
{
    // 如果當前顯示的關係數據發生變化，更新UI
    if (RelationshipManager && !bIsUpdating)
    {
        UpdateRelationshipData(RelationshipManager);
    }
}

// 私有輔助函數
TArray<FRelationshipData> UMingRelationshipPanel::ApplyFilter(const TArray<FRelationshipData>& Relationships, ERelationshipFilter Filter)
{
    TArray<FRelationshipData> FilteredRelationships;
    
    for (const FRelationshipData& Relationship : Relationships)
    {
        bool bShouldInclude = false;
        
        switch (Filter)
        {
            case ERelationshipFilter::All:
                bShouldInclude = true;
                break;
                
            case ERelationshipFilter::Friends:
                bShouldInclude = Relationship.RelationshipValue > 50.0f;
                break;
                
            case ERelationshipFilter::Enemies:
                bShouldInclude = Relationship.RelationshipValue < -50.0f;
                break;
                
            case ERelationshipFilter::Neutral:
                bShouldInclude = Relationship.RelationshipValue >= -50.0f && Relationship.RelationshipValue <= 50.0f;
                break;
        }
        
        if (bShouldInclude)
        {
            FilteredRelationships.Add(Relationship);
        }
    }
    
    return FilteredRelationships;
}

TArray<FRelationshipData> UMingRelationshipPanel::ApplySort(const TArray<FRelationshipData>& Relationships, ERelationshipSortType SortType)
{
    TArray<FRelationshipData> SortedRelationships = Relationships;
    
    switch (SortType)
    {
        case ERelationshipSortType::ByName:
            SortedRelationships.Sort([](const FRelationshipData& A, const FRelationshipData& B)
            {
                return A.CharacterName < B.CharacterName;
            });
            break;
            
        case ERelationshipSortType::ByValue:
            SortedRelationships.Sort([](const FRelationshipData& A, const FRelationshipData& B)
            {
                return A.RelationshipValue > B.RelationshipValue;
            });
            break;
            
        case ERelationshipSortType::ByChange:
            SortedRelationships.Sort([](const FRelationshipData& A, const FRelationshipData& B)
            {
                return A.RecentChange > B.RecentChange;
            });
            break;
    }
    
    return SortedRelationships;
}

void UMingRelationshipPanel::UpdateRelationshipList(const TArray<FRelationshipData>& Relationships)
{
    if (!RelationshipListScrollBox)
    {
        return;
    }
    
    // 清空現有列表
    RelationshipListScrollBox->ClearChildren();
    
    // 創建關係項目
    for (const FRelationshipData& Relationship : Relationships)
    {
        UWidget* RelationshipItem = CreateRelationshipItem(Relationship);
        if (RelationshipItem)
        {
            RelationshipListScrollBox->AddChild(RelationshipItem);
        }
    }
    
    // 更新計數
    if (RelationshipCountText)
    {
        RelationshipCountText->SetText(FText::FromString(FString::Printf(TEXT("共 %d 個關係"), Relationships.Num())));
    }
}

UWidget* UMingRelationshipPanel::CreateRelationshipItem(const FRelationshipData& Relationship)
{
    // 這裡應該創建一個自定義的關係項目Widget
    // 暫時返回一個簡單的文本塊作為示例
    UTextBlock* TextBlock = NewObject<UTextBlock>(this);
    if (TextBlock)
    {
        FString ItemText = FString::Printf(TEXT("%s: %.1f"), *Relationship.CharacterName, Relationship.RelationshipValue);
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

void UMingRelationshipPanel::UpdateCharacterDetails(const FString& CharacterID)
{
    if (!RelationshipManager || CharacterID.IsEmpty())
    {
        return;
    }
    
    // 獲取角色詳細信息
    FRelationshipData RelationshipData = RelationshipManager->GetRelationship(CharacterID);
    
    // 更新詳細信息面板
    if (CharacterNameText)
    {
        CharacterNameText->SetText(FText::FromString(RelationshipData.CharacterName));
    }
    
    if (RelationshipValueText)
    {
        RelationshipValueText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), RelationshipData.RelationshipValue)));
    }
    
    if (RelationshipProgressBar)
    {
        float NormalizedValue = (RelationshipData.RelationshipValue + 100.0f) / 200.0f; // -100 到 100 映射到 0 到 1
        RelationshipProgressBar->SetPercent(FMath::Clamp(NormalizedValue, 0.0f, 1.0f));
    }
    
    if (RecentChangeText)
    {
        FString ChangeText = FString::Printf(TEXT("%+.1f"), RelationshipData.RecentChange);
        RecentChangeText->SetText(FText::FromString(ChangeText));
        
        // 設置顏色（正數為綠色，負數為紅色）
        FSlateColor Color = RelationshipData.RecentChange >= 0 ? FLinearColor::Green : FLinearColor::Red;
        RecentChangeText->SetColorAndOpacity(Color);
    }
    
    if (LastInteractionText)
    {
        LastInteractionText->SetText(FText::FromString(RelationshipData.LastInteraction));
    }
    
    if (RelationshipHistoryText)
    {
        FString HistoryText;
        for (const FString& HistoryItem : RelationshipData.InteractionHistory)
        {
            HistoryText += HistoryItem + TEXT("\n");
        }
        RelationshipHistoryText->SetText(FText::FromString(HistoryText));
    }
}

void UMingRelationshipPanel::UpdateStatistics(const TArray<FRelationshipData>& Relationships)
{
    int32 FriendCount = 0;
    int32 EnemyCount = 0;
    int32 NeutralCount = 0;
    float TotalValue = 0.0f;
    
    for (const FRelationshipData& Relationship : Relationships)
    {
        TotalValue += Relationship.RelationshipValue;
        
        if (Relationship.RelationshipValue > 50.0f)
        {
            FriendCount++;
        }
        else if (Relationship.RelationshipValue < -50.0f)
        {
            EnemyCount++;
        }
        else
        {
            NeutralCount++;
        }
    }
    
    // 更新統計文本
    if (FriendCountText)
    {
        FriendCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), FriendCount)));
    }
    
    if (EnemyCountText)
    {
        EnemyCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), EnemyCount)));
    }
    
    if (NeutralCountText)
    {
        NeutralCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), NeutralCount)));
    }
    
    if (AverageValueText)
    {
        float AverageValue = Relationships.Num() > 0 ? TotalValue / Relationships.Num() : 0.0f;
        AverageValueText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), AverageValue)));
    }
}

void UMingRelationshipPanel::UpdateFilterButtons()
{
    // 更新過濾器按鈕的視覺狀態
    if (FilterAllButton)
    {
        FilterAllButton->SetIsEnabled(CurrentFilter != ERelationshipFilter::All);
    }
    
    if (FilterFriendsButton)
    {
        FilterFriendsButton->SetIsEnabled(CurrentFilter != ERelationshipFilter::Friends);
    }
    
    if (FilterEnemiesButton)
    {
        FilterEnemiesButton->SetIsEnabled(CurrentFilter != ERelationshipFilter::Enemies);
    }
    
    if (FilterNeutralButton)
    {
        FilterNeutralButton->SetIsEnabled(CurrentFilter != ERelationshipFilter::Neutral);
    }
}

void UMingRelationshipPanel::UpdateSortButtons()
{
    // 更新排序按鈕的視覺狀態
    if (SortByNameButton)
    {
        SortByNameButton->SetIsEnabled(CurrentSortType != ERelationshipSortType::ByName);
    }
    
    if (SortByValueButton)
    {
        SortByValueButton->SetIsEnabled(CurrentSortType != ERelationshipSortType::ByValue);
    }
    
    if (SortByChangeButton)
    {
        SortByChangeButton->SetIsEnabled(CurrentSortType != ERelationshipSortType::ByChange);
    }
}

void UMingRelationshipPanel::UpdateSelectionState()
{
    // 更新列表項目的選中狀態
    // 這裡需要遍歷所有關係項目並設置選中狀態
    // 暫時留空，待實現
}

void UMingRelationshipPanel::PlayDetailsAnimation(bool bShow)
{
    // 播放詳細信息面板的顯示/隱藏動畫
    if (DetailsPanel)
    {
        // 這裡應該實現實際的動畫邏輯
        UE_LOG(LogTemp, Log, TEXT("播放詳細信息面板動畫：%s"), bShow ? TEXT("顯示") : TEXT("隱藏"));
    }
}

void UMingRelationshipPanel::PlayRefreshAnimation()
{
    // 播放刷新動畫
    if (RefreshButton)
    {
        // 這裡應該實現實際的旋轉動畫
        UE_LOG(LogTemp, Log, TEXT("播放刷新動畫"));
    }
}

FString UMingRelationshipPanel::GetRelationshipStatusText(float Value)
{
    if (Value > 80.0f)
    {
        return TEXT("摯友");
    }
    else if (Value > 50.0f)
    {
        return TEXT("朋友");
    }
    else if (Value > 20.0f)
    {
        return TEXT友善");
    }
    else if (Value > -20.0f)
    {
        return TEXT("中立");
    }
    else if (Value > -50.0f)
    {
        return TEXT("敵對");
    }
    else if (Value > -80.0f)
    {
        return TEXT("仇敵");
    }
    else
    {
        return TEXT("死敵");
    }
}

FSlateColor UMingRelationshipPanel::GetRelationshipValueColor(float Value)
{
    if (Value > 50.0f)
    {
        return FLinearColor::Green;
    }
    else if (Value > 0.0f)
    {
        return FLinearColor(0.0f, 0.5f, 0.0f); // 深綠色
    }
    else if (Value > -50.0f)
    {
        return FLinearColor::Yellow;
    }
    else
    {
        return FLinearColor::Red;
    }
}
