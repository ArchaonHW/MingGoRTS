#include "MingQuestPanel.h"
#include "MingRelationshipManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Engine/Engine.h"

UMingQuestPanel::UMingQuestPanel()
{
    bIsUpdating = false;
    CurrentFilter = EQuestFilter::All;
    CurrentSortType = EQuestSortType::ByName;
    SelectedQuestID = TEXT("");
    bShowCompleted = false;
}

void UMingQuestPanel::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 綁定按鈕事件
    if (FilterAllButton)
    {
        FilterAllButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnFilterAllClicked);
    }
    
    if (FilterActiveButton)
    {
        FilterActiveButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnFilterActiveClicked);
    }
    
    if (FilterCompletedButton)
    {
        FilterCompletedButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnFilterCompletedClicked);
    }
    
    if (SortByNameButton)
    {
        SortByNameButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnSortByNameClicked);
    }
    
    if (SortByPriorityButton)
    {
        SortByPriorityButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnSortByPriorityClicked);
    }
    
    if (SortByProgressButton)
    {
        SortByProgressButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnSortByProgressClicked);
    }
    
    if (RefreshButton)
    {
        RefreshButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnRefreshClicked);
    }
    
    if (AcceptQuestButton)
    {
        AcceptQuestButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnAcceptQuestClicked);
    }
    
    if (AbandonQuestButton)
    {
        AbandonQuestButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnAbandonQuestClicked);
    }
    
    if (TrackQuestButton)
    {
        TrackQuestButton->OnClicked.AddDynamic(this, &UMingQuestPanel::OnTrackQuestClicked);
    }
    
    // 設置默認過濾器和排序
    UpdateFilterButtons();
    UpdateSortButtons();
}

void UMingQuestPanel::UpdateQuestData(UMingRelationshipManager* InRelationshipManager)
{
    if (bIsUpdating || !InRelationshipManager)
    {
        return;
    }
    
    bIsUpdating = true;
    RelationshipManager = InRelationshipManager;
    
    // 獲取所有任務數據
    TArray<FQuestData> AllQuests = RelationshipManager->GetAllQuests();
    
    // 應用過濾器
    TArray<FQuestData> FilteredQuests = ApplyFilter(AllQuests, CurrentFilter);
    
    // 應用排序
    TArray<FQuestData> SortedQuests = ApplySort(FilteredQuests, CurrentSortType);
    
    // 更新UI顯示
    UpdateQuestList(SortedQuests);
    
    // 更新統計信息
    UpdateStatistics(AllQuests);
    
    bIsUpdating = false;
    
    UE_LOG(LogTemp, Log, TEXT("任務面板數據已更新，顯示 %d 個任務"), SortedQuests.Num());
}

void UMingQuestPanel::SetQuestFilter(EQuestFilter Filter)
{
    if (CurrentFilter != Filter)
    {
        CurrentFilter = Filter;
        UpdateFilterButtons();
        
        if (RelationshipManager)
        {
            UpdateQuestData(RelationshipManager);
        }
    }
}

void UMingQuestPanel::SetQuestSort(EQuestSortType SortType)
{
    if (CurrentSortType != SortType)
    {
        CurrentSortType = SortType;
        UpdateSortButtons();
        
        if (RelationshipManager)
        {
            UpdateQuestData(RelationshipManager);
        }
    }
}

void UMingQuestPanel::SelectQuest(const FString& QuestID)
{
    if (SelectedQuestID == QuestID)
    {
        return;
    }
    
    SelectedQuestID = QuestID;
    
    // 更新詳細信息面板
    UpdateQuestDetails(QuestID);
    
    // 更新選中狀態
    UpdateSelectionState();
    
    UE_LOG(LogTemp, Log, TEXT("已選中任務：%s"), *QuestID);
}

void UMingQuestPanel::ShowQuestDetails(const FString& QuestID)
{
    SelectQuest(QuestID);
    
    // 顯示詳細信息面板
    if (DetailsPanel)
    {
        DetailsPanel->SetVisibility(ESlateVisibility::Visible);
        PlayDetailsAnimation(true);
    }
}

void UMingQuestPanel::HideQuestDetails()
{
    SelectedQuestID = TEXT("");
    
    // 隱藏詳細信息面板
    if (DetailsPanel)
    {
        PlayDetailsAnimation(false);
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

void UMingQuestPanel::RefreshQuests()
{
    if (RelationshipManager)
    {
        UpdateQuestData(RelationshipManager);
        PlayRefreshAnimation();
    }
}

void UMingQuestPanel::AcceptQuest(const FString& QuestID)
{
    if (!RelationshipManager || QuestID.IsEmpty())
    {
        return;
    }
    
    // 獲取任務數據
    FQuestData QuestData = RelationshipManager->GetQuest(QuestID);
    
    // 檢查是否可以接受
    if (!CanAcceptQuest(QuestData))
    {
        UE_LOG(LogTemp, Warning, TEXT("無法接受任務：%s - 條件不滿足"), *QuestID);
        return;
    }
    
    // 接受任務
    bool bSuccess = RelationshipManager->AcceptQuest(QuestID);
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("成功接受任務：%s"), *QuestID);
        
        // 更新UI
        UpdateQuestData(RelationshipManager);
        
        // 觸發事件
        OnQuestAccepted.Broadcast(QuestID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("接受任務失敗：%s"), *QuestID);
    }
}

void UMingQuestPanel::AbandonQuest(const FString& QuestID)
{
    if (!RelationshipManager || QuestID.IsEmpty())
    {
        return;
    }
    
    // 獲取任務數據
    FQuestData QuestData = RelationshipManager->GetQuest(QuestID);
    
    // 檢查是否可以放棄
    if (!CanAbandonQuest(QuestData))
    {
        UE_LOG(LogTemp, Warning, TEXT("無法放棄任務：%s - 不允許放棄"), *QuestID);
        return;
    }
    
    // 放棄任務
    bool bSuccess = RelationshipManager->AbandonQuest(QuestID);
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("成功放棄任務：%s"), *QuestID);
        
        // 更新UI
        UpdateQuestData(RelationshipManager);
        
        // 觸發事件
        OnQuestAbandoned.Broadcast(QuestID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("放棄任務失敗：%s"), *QuestID);
    }
}

void UMingQuestPanel::TrackQuest(const FString& QuestID)
{
    if (!RelationshipManager || QuestID.IsEmpty())
    {
        return;
    }
    
    // 設置追蹤任務
    bool bSuccess = RelationshipManager->TrackQuest(QuestID);
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("開始追蹤任務：%s"), *QuestID);
        
        // 更新UI
        UpdateQuestData(RelationshipManager);
        
        // 觸發事件
        OnQuestTracked.Broadcast(QuestID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("追蹤任務失敗：%s"), *QuestID);
    }
}

void UMingQuestPanel::ShowCompletedQuests(bool bShow)
{
    if (bShowCompleted != bShow)
    {
        bShowCompleted = bShow;
        
        if (RelationshipManager)
        {
            UpdateQuestData(RelationshipManager);
        }
    }
}

// 按鈕事件處理
void UMingQuestPanel::OnFilterAllClicked()
{
    SetQuestFilter(EQuestFilter::All);
}

void UMingQuestPanel::OnFilterActiveClicked()
{
    SetQuestFilter(EQuestFilter::Active);
}

void UMingQuestPanel::OnFilterCompletedClicked()
{
    SetQuestFilter(EQuestFilter::Completed);
}

void UMingQuestPanel::OnSortByNameClicked()
{
    SetQuestSort(EQuestSortType::ByName);
}

void UMingQuestPanel::OnSortByPriorityClicked()
{
    SetQuestSort(EQuestSortType::ByPriority);
}

void UMingQuestPanel::OnSortByProgressClicked()
{
    SetQuestSort(EQuestSortType::ByProgress);
}

void UMingQuestPanel::OnRefreshClicked()
{
    RefreshQuests();
}

void UMingQuestPanel::OnAcceptQuestClicked()
{
    if (!SelectedQuestID.IsEmpty())
    {
        AcceptQuest(SelectedQuestID);
    }
}

void UMingQuestPanel::OnAbandonQuestClicked()
{
    if (!SelectedQuestID.IsEmpty())
    {
        AbandonQuest(SelectedQuestID);
    }
}

void UMingQuestPanel::OnTrackQuestClicked()
{
    if (!SelectedQuestID.IsEmpty())
    {
        TrackQuest(SelectedQuestID);
    }
}

void UMingQuestPanel::OnQuestClicked(const FString& QuestID)
{
    ShowQuestDetails(QuestID);
}

void UMingQuestPanel::OnQuestProgressUpdated(const FString& QuestID, float OldProgress, float NewProgress)
{
    // 如果當前顯示的任務數據發生變化，更新UI
    if (RelationshipManager && !bIsUpdating)
    {
        UpdateQuestData(RelationshipManager);
    }
}

// 私有輔助函數
TArray<FQuestData> UMingQuestPanel::ApplyFilter(const TArray<FQuestData>& Quests, EQuestFilter Filter)
{
    TArray<FQuestData> FilteredQuests;
    
    for (const FQuestData& Quest : Quests)
    {
        bool bShouldInclude = false;
        
        switch (Filter)
        {
            case EQuestFilter::All:
                bShouldInclude = true;
                break;
                
            case EQuestFilter::Active:
                bShouldInclude = Quest.bIsActive && !Quest.bIsCompleted;
                break;
                
            case EQuestFilter::Completed:
                bShouldInclude = Quest.bIsCompleted;
                break;
        }
        
        // 檢查是否顯示已完成任務
        if (bShouldInclude && Quest.bIsCompleted && !bShowCompleted)
        {
            bShouldInclude = false;
        }
        
        if (bShouldInclude)
        {
            FilteredQuests.Add(Quest);
        }
    }
    
    return FilteredQuests;
}

TArray<FQuestData> UMingQuestPanel::ApplySort(const TArray<FQuestData>& Quests, EQuestSortType SortType)
{
    TArray<FQuestData> SortedQuests = Quests;
    
    switch (SortType)
    {
        case EQuestSortType::ByName:
            SortedQuests.Sort([](const FQuestData& A, const FQuestData& B)
            {
                return A.QuestName < B.QuestName;
            });
            break;
            
        case EQuestSortType::ByPriority:
            SortedQuests.Sort([](const FQuestData& A, const FQuestData& B)
            {
                return static_cast<int32>(A.Priority) > static_cast<int32>(B.Priority);
            });
            break;
            
        case EQuestSortType::ByProgress:
            SortedQuests.Sort([](const FQuestData& A, const FQuestData& B)
            {
                return A.Progress > B.Progress;
            });
            break;
    }
    
    return SortedQuests;
}

void UMingQuestPanel::UpdateQuestList(const TArray<FQuestData>& Quests)
{
    if (!QuestListScrollBox)
    {
        return;
    }
    
    // 清空現有列表
    QuestListScrollBox->ClearChildren();
    
    // 創建任務項目
    for (const FQuestData& Quest : Quests)
    {
        UWidget* QuestItem = CreateQuestItem(Quest);
        if (QuestItem)
        {
            QuestListScrollBox->AddChild(QuestItem);
        }
    }
    
    // 更新計數
    if (QuestCountText)
    {
        QuestCountText->SetText(FText::FromString(FString::Printf(TEXT("共 %d 個任務"), Quests.Num())));
    }
}

UWidget* UMingQuestPanel::CreateQuestItem(const FQuestData& Quest)
{
    // 這裡應該創建一個自定義的任務項目Widget
    // 暫時返回一個簡單的文本塊作為示例
    UTextBlock* TextBlock = NewObject<UTextBlock>(this);
    if (TextBlock)
    {
        FString ItemText = FString::Printf(TEXT("%s (%.0f%%)"), *Quest.QuestName, Quest.Progress);
        TextBlock->SetText(FText::FromString(ItemText));
        
        // 設置字體和樣式
        FSlateFontInfo FontInfo = TextBlock->GetFont();
        FontInfo.Size = 14;
        TextBlock->SetFont(FontInfo);
        
        // 設置邊距
        TextBlock->SetMargin(FMargin(10.0f, 5.0f));
        
        // 根據優先級設置顏色
        FSlateColor Color = GetPriorityColor(Quest.Priority);
        TextBlock->SetColorAndOpacity(Color);
    }
    
    return TextBlock;
}

void UMingQuestPanel::UpdateQuestDetails(const FString& QuestID)
{
    if (!RelationshipManager || QuestID.IsEmpty())
    {
        return;
    }
    
    // 獲取任務詳細信息
    FQuestData QuestData = RelationshipManager->GetQuest(QuestID);
    
    // 更新詳細信息面板
    if (QuestNameText)
    {
        QuestNameText->SetText(FText::FromString(QuestData.QuestName));
    }
    
    if (QuestDescriptionText)
    {
        QuestDescriptionText->SetText(FText::FromString(QuestData.Description));
    }
    
    if (QuestTypeText)
    {
        QuestTypeText->SetText(FText::FromString(GetQuestTypeText(QuestData.QuestType)));
    }
    
    if (QuestPriorityText)
    {
        QuestPriorityText->SetText(FText::FromString(GetPriorityText(QuestData.Priority)));
    }
    
    if (QuestProgressBar)
    {
        QuestProgressBar->SetPercent(QuestData.Progress / 100.0f);
    }
    
    if (QuestProgressText)
    {
        QuestProgressText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), QuestData.Progress)));
    }
    
    if (QuestRewardText)
    {
        FString RewardText;
        for (const FQuestReward& Reward : QuestData.Rewards)
        {
            RewardText += FString::Printf(TEXT("%s x%d\n"), *Reward.ItemName, Reward.Quantity);
        }
        QuestRewardText->SetText(FText::FromString(RewardText));
    }
    
    if (QuestObjectivesText)
    {
        FString ObjectivesText;
        for (const FQuestObjective& Objective : QuestData.Objectives)
        {
            FString Status = Objective.bCompleted ? TEXT("✓") : TEXT("○");
            ObjectivesText += FString::Printf(TEXT("%s %s\n"), *Status, *Objective.Description);
        }
        QuestObjectivesText->SetText(FText::FromString(ObjectivesText));
    }
    
    // 更新按鈕狀態
    UpdateActionButtons(QuestData);
}

void UMingQuestPanel::UpdateStatistics(const TArray<FQuestData>& Quests)
{
    int32 ActiveCount = 0;
    int32 CompletedCount = 0;
    int32 TrackedCount = 0;
    float TotalProgress = 0.0f;
    
    for (const FQuestData& Quest : Quests)
    {
        TotalProgress += Quest.Progress;
        
        if (Quest.bIsActive && !Quest.bIsCompleted)
        {
            ActiveCount++;
        }
        
        if (Quest.bIsCompleted)
        {
            CompletedCount++;
        }
        
        if (Quest.bIsTracked)
        {
            TrackedCount++;
        }
    }
    
    // 更新統計文本
    if (ActiveCountText)
    {
        ActiveCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ActiveCount)));
    }
    
    if (CompletedCountText)
    {
        CompletedCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CompletedCount)));
    }
    
    if (TrackedCountText)
    {
        TrackedCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TrackedCount)));
    }
    
    if (AverageProgressText)
    {
        float AverageProgress = Quests.Num() > 0 ? TotalProgress / Quests.Num() : 0.0f;
        AverageProgressText->SetText(FText::FromString(FString::Printf(TEXT("%.1f%%"), AverageProgress)));
    }
}

void UMingQuestPanel::UpdateFilterButtons()
{
    // 更新過濾器按鈕的視覺狀態
    if (FilterAllButton)
    {
        FilterAllButton->SetIsEnabled(CurrentFilter != EQuestFilter::All);
    }
    
    if (FilterActiveButton)
    {
        FilterActiveButton->SetIsEnabled(CurrentFilter != EQuestFilter::Active);
    }
    
    if (FilterCompletedButton)
    {
        FilterCompletedButton->SetIsEnabled(CurrentFilter != EQuestFilter::Completed);
    }
}

void UMingQuestPanel::UpdateSortButtons()
{
    // 更新排序按鈕的視覺狀態
    if (SortByNameButton)
    {
        SortByNameButton->SetIsEnabled(CurrentSortType != EQuestSortType::ByName);
    }
    
    if (SortByPriorityButton)
    {
        SortByPriorityButton->SetIsEnabled(CurrentSortType != EQuestSortType::ByPriority);
    }
    
    if (SortByProgressButton)
    {
        SortByProgressButton->SetIsEnabled(CurrentSortType != EQuestSortType::ByProgress);
    }
}

void UMingQuestPanel::UpdateActionButtons(const FQuestData& QuestData)
{
    if (AcceptQuestButton)
    {
        bool bCanAccept = CanAcceptQuest(QuestData);
        AcceptQuestButton->SetIsEnabled(bCanAccept);
        AcceptQuestButton->SetVisibility(bCanAccept ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
    
    if (AbandonQuestButton)
    {
        bool bCanAbandon = CanAbandonQuest(QuestData);
        AbandonQuestButton->SetIsEnabled(bCanAbandon);
        AbandonQuestButton->SetVisibility(bCanAbandon ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
    
    if (TrackQuestButton)
    {
        bool bCanTrack = QuestData.bIsActive && !QuestData.bIsTracked;
        TrackQuestButton->SetIsEnabled(bCanTrack);
        TrackQuestButton->SetVisibility(bCanTrack ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

void UMingQuestPanel::UpdateSelectionState()
{
    // 更新列表項目的選中狀態
    // 這裡需要遍歷所有任務項目並設置選中狀態
    // 暫時留空，待實現
}

bool UMingQuestPanel::CanAcceptQuest(const FQuestData& QuestData)
{
    // 檢查任務是否可以被接受
    if (QuestData.bIsActive || QuestData.bIsCompleted)
    {
        return false;
    }
    
    // 檢查前置條件
    for (const FString& Prerequisite : QuestData.Prerequisites)
    {
        // 這裡應該檢查前置任務是否完成
        // 暫時返回true
    }
    
    // 檢查玩家等級
    // 這裡應該檢查玩家等級是否足夠
    // 暫時返回true
    
    return true;
}

bool UMingQuestPanel::CanAbandonQuest(const FQuestData& QuestData)
{
    // 檢查任務是否可以被放棄
    return QuestData.bIsActive && !QuestData.bIsCompleted && !QuestData.bIsMandatory;
}

void UMingQuestPanel::PlayDetailsAnimation(bool bShow)
{
    // 播放詳細信息面板的顯示/隱藏動畫
    if (DetailsPanel)
    {
        UE_LOG(LogTemp, Log, TEXT("播放詳細信息面板動畫：%s"), bShow ? TEXT("顯示") : TEXT("隱藏"));
    }
}

void UMingQuestPanel::PlayRefreshAnimation()
{
    // 播放刷新動畫
    if (RefreshButton)
    {
        UE_LOG(LogTemp, Log, TEXT("播放刷新動畫"));
    }
}

FString UMingQuestPanel::GetQuestTypeText(EQuestType QuestType)
{
    switch (QuestType)
    {
        case EQuestType::Main:
            return TEXT("主線任務");
        case EQuestType::Side:
            return TEXT("支線任務");
        case EQuestType::Daily:
            return TEXT("每日任務");
        case EQuestType::Weekly:
            return TEXT("每週任務");
        case EQuestType::Event:
            return TEXT("活動任務");
        case EQuestType::Hidden:
            return TEXT("隱藏任務");
        default:
            return TEXT("未知類型");
    }
}

FString UMingQuestPanel::GetPriorityText(EQuestPriority Priority)
{
    switch (Priority)
    {
        case EQuestPriority::Low:
            return TEXT("低優先級");
        case EQuestPriority::Medium:
            return TEXT("中優先級");
        case EQuestPriority::High:
            return TEXT("高優先級");
        case EQuestPriority::Urgent:
            return TEXT("緊急任務");
        default:
            return TEXT("未知優先級");
    }
}

FSlateColor UMingQuestPanel::GetPriorityColor(EQuestPriority Priority)
{
    switch (Priority)
    {
        case EQuestPriority::Low:
            return FLinearColor::Gray;
        case EQuestPriority::Medium:
            return FLinearColor::White;
        case EQuestPriority::High:
            return FLinearColor::Yellow;
        case EQuestPriority::Urgent:
            return FLinearColor::Red;
        default:
            return FLinearColor::White;
    }
}
