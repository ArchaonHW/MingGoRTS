#include "MingQuestPanel.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMingQuestPanel::UMingQuestPanel(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bIsFiltered(false)
    , bSortByDifficulty(false)
    , bSortByLevel(false)
    , bSortByTime(false)
    , bSortAscending(true)
    , QuestListView(nullptr)
    , TotalQuestText(nullptr)
    , AvailableQuestText(nullptr)
    , ActiveQuestText(nullptr)
    , CompletedQuestText(nullptr)
    , CompletionRateText(nullptr)
    , SearchBox(nullptr)
    , DetailsScrollBox(nullptr)
{
}

void UMingQuestPanel::NativeConstruct()
{
    Super::NativeConstruct();

    // 初始化過濾器
    StatusFilters.Add(EQuestStatus::Available, false);
    StatusFilters.Add(EQuestStatus::Active, false);
    StatusFilters.Add(EQuestStatus::Completed, false);
    StatusFilters.Add(EQuestStatus::Failed, false);
    StatusFilters.Add(EQuestStatus::Abandoned, false);

    TypeFilters.Add(EQuestType::Main, false);
    TypeFilters.Add(EQuestType::Side, false);
    TypeFilters.Add(EQuestType::Daily, false);
    TypeFilters.Add(EQuestType::Weekly, false);
    TypeFilters.Add(EQuestType::Special, false);
    TypeFilters.Add(EQuestType::Relationship, false);
    TypeFilters.Add(EQuestType::Reputation, false);
    TypeFilters.Add(EQuestType::Exploration, false);

    SetupEventBindings();
    GenerateRandomQuests(); // 生成一些測試任務
    UpdateUI();
}

void UMingQuestPanel::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMingQuestPanel::InitializeQuestPanel(UMingRelationshipManager* InRelationshipManager)
{
    RelationshipManager = InRelationshipManager;
    
    if (RelationshipManager)
    {
        UE_LOG(LogTemp, Log, TEXT("Quest panel initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Relationship manager is null - panel initialization failed"));
    }
}

void UMingQuestPanel::AddQuest(const FQuestData& QuestData)
{
    AllQuests.Add(QuestData);
    ProcessQuestData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Added quest: %s"), *QuestData.QuestTitle);
}

void UMingQuestPanel::RemoveQuest(const FString& QuestID)
{
    for (int32 i = 0; i < AllQuests.Num(); ++i)
    {
        if (AllQuests[i].QuestID == QuestID)
        {
            AllQuests.RemoveAt(i);
            ProcessQuestData();
            UpdateUI();
            
            UE_LOG(LogTemp, Log, TEXT("Removed quest: %s"), *QuestID);
            break;
        }
    }
}

void UMingQuestPanel::UpdateQuest(const FString& QuestID, const FQuestData& QuestData)
{
    for (int32 i = 0; i < AllQuests.Num(); ++i)
    {
        if (AllQuests[i].QuestID == QuestID)
        {
            AllQuests[i] = QuestData;
            ProcessQuestData();
            UpdateUI();
            
            UE_LOG(LogTemp, Log, TEXT("Updated quest: %s"), *QuestData.QuestTitle);
            break;
        }
    }
}

void UMingQuestPanel::AcceptQuest(const FString& QuestID)
{
    for (FQuestData& QuestData : AllQuests)
    {
        if (QuestData.QuestID == QuestID && QuestData.QuestStatus == EQuestStatus::Available)
        {
            QuestData.QuestStatus = EQuestStatus::Active;
            QuestData.StartTime = FDateTime::Now();
            
            ProcessQuestData();
            UpdateUI();
            
            OnQuestAccepted.Broadcast(QuestID);
            
            UE_LOG(LogTemp, Log, TEXT("Accepted quest: %s"), *QuestID);
            break;
        }
    }
}

void UMingQuestPanel::AbandonQuest(const FString& QuestID)
{
    for (FQuestData& QuestData : AllQuests)
    {
        if (QuestData.QuestID == QuestID && QuestData.QuestStatus == EQuestStatus::Active)
        {
            QuestData.QuestStatus = EQuestStatus::Abandoned;
            
            ProcessQuestData();
            UpdateUI();
            
            UE_LOG(LogTemp, Log, TEXT("Abandoned quest: %s"), *QuestID);
            break;
        }
    }
}

void UMingQuestPanel::CompleteQuest(const FString& QuestID)
{
    for (FQuestData& QuestData : AllQuests)
    {
        if (QuestData.QuestID == QuestID && QuestData.QuestStatus == EQuestStatus::Active)
        {
            QuestData.QuestStatus = EQuestStatus::Completed;
            QuestData.EndTime = FDateTime::Now();
            
            ProcessQuestData();
            UpdateUI();
            
            OnQuestCompleted.Broadcast(QuestID);
            
            UE_LOG(LogTemp, Log, TEXT("Completed quest: %s"), *QuestID);
            break;
        }
    }
}

void UMingQuestPanel::FailQuest(const FString& QuestID)
{
    for (FQuestData& QuestData : AllQuests)
    {
        if (QuestData.QuestID == QuestID && QuestData.QuestStatus == EQuestStatus::Active)
        {
            QuestData.QuestStatus = EQuestStatus::Failed;
            QuestData.EndTime = FDateTime::Now();
            
            ProcessQuestData();
            UpdateUI();
            
            OnQuestFailed.Broadcast(QuestID);
            
            UE_LOG(LogTemp, Log, TEXT("Failed quest: %s"), *QuestID);
            break;
        }
    }
}

FQuestData UMingQuestPanel::GetQuest(const FString& QuestID) const
{
    for (const FQuestData& QuestData : AllQuests)
    {
        if (QuestData.QuestID == QuestID)
        {
            return QuestData;
        }
    }
    return FQuestData();
}

TArray<FQuestData> UMingQuestPanel::GetQuestsByStatus(EQuestStatus Status) const
{
    TArray<FQuestData> Result;
    for (const FQuestData& QuestData : AllQuests)
    {
        if (QuestData.QuestStatus == Status)
        {
            Result.Add(QuestData);
        }
    }
    return Result;
}

TArray<FQuestData> UMingQuestPanel::GetQuestsByType(EQuestType Type) const
{
    TArray<FQuestData> Result;
    for (const FQuestData& QuestData : AllQuests)
    {
        if (QuestData.QuestType == Type)
        {
            Result.Add(QuestData);
        }
    }
    return Result;
}

TArray<FQuestData> UMingQuestPanel::GetQuestsByRegion(const FString& RegionID) const
{
    TArray<FQuestData> Result;
    for (const FQuestData& QuestData : AllQuests)
    {
        if (QuestData.RegionID == RegionID)
        {
            Result.Add(QuestData);
        }
    }
    return Result;
}

bool UMingQuestPanel::CanAcceptQuest(const FString& QuestID) const
{
    FQuestData QuestData = GetQuest(QuestID);
    
    if (QuestData.QuestID.IsEmpty())
    {
        return false;
    }
    
    // 檢查狀態
    if (QuestData.QuestStatus != EQuestStatus::Available)
    {
        return false;
    }
    
    // 檢查等級要求
    // 這裡可以添加玩家等級檢查
    
    // 檢查地區聲望要求
    if (RelationshipManager && !QuestData.RegionID.IsEmpty())
    {
        return RelationshipManager->CanAcceptQuest(QuestID, QuestData.RegionID);
    }
    
    return true;
}

bool UMingQuestPanel::HasActiveQuests() const
{
    for (const FQuestData& QuestData : AllQuests)
    {
        if (QuestData.QuestStatus == EQuestStatus::Active)
        {
            return true;
        }
    }
    return false;
}

void UMingQuestPanel::FilterByStatus(EQuestStatus Status, bool bActive)
{
    StatusFilters.Add(Status, bActive);
    ProcessQuestData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Applied status filter: %d (Active: %s)"), 
           (int32)Status, bActive ? TEXT("true") : TEXT("false"));
}

void UMingQuestPanel::FilterByType(EQuestType Type, bool bActive)
{
    TypeFilters.Add(Type, bActive);
    ProcessQuestData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Applied type filter: %d (Active: %s)"), 
           (int32)Type, bActive ? TEXT("true") : TEXT("false"));
}

void UMingQuestPanel::FilterByRegion(const FString& RegionID, bool bActive)
{
    if (bActive)
    {
        RegionFilters.Add(RegionID);
    }
    else
    {
        RegionFilters.Remove(RegionID);
    }
    
    ProcessQuestData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Applied region filter: %s (Active: %s)"), 
           *RegionID, bActive ? TEXT("true") : TEXT("false"));
}

void UMingQuestPanel::SortByDifficulty(bool bAscending)
{
    bSortByDifficulty = true;
    bSortAscending = bAscending;
    
    ProcessQuestData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Sorted quests by difficulty (Ascending: %s)"), 
           bAscending ? TEXT("true") : TEXT("false"));
}

void UMingQuestPanel::SortByLevel(bool bAscending)
{
    bSortByLevel = true;
    bSortAscending = bAscending;
    
    ProcessQuestData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Sorted quests by level (Ascending: %s)"), 
           bAscending ? TEXT("true") : TEXT("false"));
}

void UMingQuestPanel::SortByTime(bool bAscending)
{
    bSortByTime = true;
    bSortAscending = bAscending;
    
    ProcessQuestData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Sorted quests by time (Ascending: %s)"), 
           bAscending ? TEXT("true") : TEXT("false"));
}

void UMingQuestPanel::ClearFilters()
{
    // 清空所有過濾器
    for (auto& FilterPair : StatusFilters)
    {
        FilterPair.Value = false;
    }
    
    for (auto& FilterPair : TypeFilters)
    {
        FilterPair.Value = false;
    }
    
    RegionFilters.Empty();
    CurrentSearchText.Empty();
    
    if (SearchBox)
    {
        SearchBox->SetText(FText::GetEmpty());
    }
    
    ProcessQuestData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Cleared all filters"));
}

void UMingQuestPanel::SearchQuests(const FString& SearchText)
{
    CurrentSearchText = SearchText;
    ProcessQuestData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Searched quests: %s"), *SearchText);
}

void UMingQuestPanel::ClearSearch()
{
    SearchQuests(TEXT(""));
}

int32 UMingQuestPanel::GetTotalQuestCount() const
{
    return AllQuests.Num();
}

int32 UMingQuestPanel::GetAvailableQuestCount() const
{
    return GetQuestsByStatus(EQuestStatus::Available).Num();
}

int32 UMingQuestPanel::GetActiveQuestCount() const
{
    return GetQuestsByStatus(EQuestStatus::Active).Num();
}

int32 UMingQuestPanel::GetCompletedQuestCount() const
{
    return GetQuestsByStatus(EQuestStatus::Completed).Num();
}

float UMingQuestPanel::GetQuestCompletionRate() const
{
    if (AllQuests.Num() == 0)
    {
        return 0.0f;
    }
    
    return (float)GetCompletedQuestCount() / AllQuests.Num() * 100.0f;
}

void UMingQuestPanel::ShowQuestDetails(const FString& QuestID)
{
    FQuestData QuestData = GetQuest(QuestID);
    if (!QuestData.QuestID.IsEmpty())
    {
        SelectedQuest = QuestData;
        UE_LOG(LogTemp, Log, TEXT("Showing quest details for: %s"), *QuestData.QuestTitle);
    }
}

void UMingQuestPanel::HideQuestDetails()
{
    UE_LOG(LogTemp, Log, TEXT("Hiding quest details"));
}

void UMingQuestPanel::UpdateQuestProgress(const FString& QuestID, const FString& RequirementText, float Progress)
{
    for (FQuestData& QuestData : AllQuests)
    {
        if (QuestData.QuestID == QuestID)
        {
            // 更新對應的需求進度
            for (FQuestRequirement& Requirement : QuestData.Requirements)
            {
                if (Requirement.RequirementText == RequirementText)
                {
                    Requirement.Progress = Progress;
                    Requirement.bIsCompleted = (Progress >= Requirement.TargetValue);
                    break;
                }
            }
            
            ProcessQuestData();
            UpdateUI();
            
            UE_LOG(LogTemp, Log, TEXT("Updated quest progress: %s - %s (%.1f%%)"), 
                   *QuestID, *RequirementText, Progress);
            break;
        }
    }
}

void UMingQuestPanel::OnQuestClicked(const FString& QuestID)
{
    ShowQuestDetails(QuestID);
    OnQuestSelected.Broadcast(QuestID, GetQuest(QuestID).QuestStatus);
}

void UMingQuestPanel::OnQuestDoubleClicked(const FString& QuestID)
{
    if (CanAcceptQuest(QuestID))
    {
        AcceptQuest(QuestID);
    }
    else
    {
        ShowQuestDetails(QuestID);
    }
}

void UMingQuestPanel::OnQuestRightClicked(const FString& QuestID)
{
    UE_LOG(LogTemp, Log, TEXT("Right-clicked quest: %s"), *QuestID);
}

void UMingQuestPanel::ExportQuestData()
{
    FString ExportData;
    
    for (const FQuestData& QuestData : AllQuests)
    {
        ExportData += FString::Printf(TEXT("%s,%s,%s,%s\n"), 
                                      *QuestData.QuestID,
                                      *QuestData.QuestTitle,
                                      *GetQuestTypeDisplayName(QuestData.QuestType),
                                      *GetQuestStatusDisplayName(QuestData.QuestStatus));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Exported quest data (%d quests)"), ExportData.Len());
}

void UMingQuestPanel::ImportQuestData()
{
    UE_LOG(LogTemp, Log, TEXT("Import quest data not yet implemented"));
}

void UMingQuestPanel::ProcessQuestData()
{
    DisplayedQuests.Empty();
    
    // 應用過濾器
    for (const FQuestData& QuestData : AllQuests)
    {
        if (PassesFilter(QuestData) && PassesSearch(QuestData))
        {
            DisplayedQuests.Add(QuestData);
        }
    }
    
    // 應用排序
    if (bSortByDifficulty)
    {
        DisplayedQuests.Sort([this](const FQuestData& A, const FQuestData& B)
        {
            return bSortAscending ? (A.Difficulty < B.Difficulty) : (A.Difficulty > B.Difficulty);
        });
    }
    else if (bSortByLevel)
    {
        DisplayedQuests.Sort([this](const FQuestData& A, const FQuestData& B)
        {
            return bSortAscending ? (A.SuggestedLevel < B.SuggestedLevel) : (A.SuggestedLevel > B.SuggestedLevel);
        });
    }
    else if (bSortByTime)
    {
        DisplayedQuests.Sort([this](const FQuestData& A, const FQuestData& B)
        {
            return bSortAscending ? (A.StartTime < B.StartTime) : (A.StartTime > B.StartTime);
        });
    }
}

void UMingQuestPanel::ApplyFilters()
{
    ProcessQuestData();
}

void UMingQuestPanel::ApplySorting()
{
    ProcessQuestData();
}

void UMingQuestPanel::UpdateStatistics()
{
    if (!TotalQuestText || !AvailableQuestText || !ActiveQuestText || !CompletedQuestText || !CompletionRateText)
    {
        return;
    }

    TotalQuestText->SetText(FText::AsNumber(GetTotalQuestCount()));
    AvailableQuestText->SetText(FText::AsNumber(GetAvailableQuestCount()));
    ActiveQuestText->SetText(FText::AsNumber(GetActiveQuestCount()));
    CompletedQuestText->SetText(FText::AsNumber(GetCompletedQuestCount()));
    
    FString RateText = FString::Printf(TEXT("%.1f%%"), GetQuestCompletionRate());
    CompletionRateText->SetText(FText::FromString(RateText));
}

void UMingQuestPanel::UpdateUI()
{
    UpdateStatistics();
    
    // 更新列表視圖
    if (QuestListView)
    {
        QuestListView->ClearListItems();
        
        for (const FQuestData& QuestData : DisplayedQuests)
        {
            // 這裡應該創建並添加列表項
            // QuestListView->AddItem(CreateListItem(QuestData));
        }
    }
}

FLinearColor UMingQuestPanel::GetQuestStatusColor(EQuestStatus Status) const
{
    switch (Status)
    {
        case EQuestStatus::Available:
            return FLinearColor(0.0f, 0.8f, 0.0f, 1.0f); // 綠色
        case EQuestStatus::Active:
            return FLinearColor(0.0f, 0.4f, 0.8f, 1.0f); // 藍色
        case EQuestStatus::Completed:
            return FLinearColor(0.8f, 0.8f, 0.0f, 1.0f); // 金色
        case EQuestStatus::Failed:
            return FLinearColor(0.8f, 0.0f, 0.0f, 1.0f); // 紅色
        case EQuestStatus::Abandoned:
            return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // 灰色
        default:
            return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
    }
}

FString UMingQuestPanel::GetQuestTypeDisplayName(EQuestType Type) const
{
    switch (Type)
    {
        case EQuestType::Main: return TEXT("主線");
        case EQuestType::Side: return TEXT("支線");
        case EQuestType::Daily: return TEXT("每日");
        case EQuestType::Weekly: return TEXT("每週");
        case EQuestType::Special: return TEXT("特殊");
        case EQuestType::Relationship: return TEXT("關係");
        case EQuestType::Reputation: return TEXT("聲望");
        case EQuestType::Exploration: return TEXT("探索");
        default: return TEXT("未知");
    }
}

FString UMingQuestPanel::GetQuestStatusDisplayName(EQuestStatus Status) const
{
    switch (Status)
    {
        case EQuestStatus::Available: return TEXT("可接取");
        case EQuestStatus::Active: return TEXT("進行中");
        case EQuestStatus::Completed: return TEXT("已完成");
        case EQuestStatus::Failed: return TEXT("失敗");
        case EQuestStatus::Abandoned: return TEXT("放棄");
        default: return TEXT("未知");
    }
}

void UMingQuestPanel::SetupEventBindings()
{
    // 綁定搜索框事件
    if (SearchBox)
    {
        SearchBox->OnTextChanged.AddDynamic(this, &UMingQuestPanel::SearchQuests);
    }
    
    // 綁定聲望管理器事件
    if (RelationshipManager)
    {
        // 這裡需要綁定聲望變化事件
        // RelationshipManager->OnReputationChanged.AddDynamic(this, &UMingQuestPanel::OnReputationChanged);
    }
}

void UMingQuestPanel::OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason)
{
    // 聲望變化可能影響任務可用性
    UpdateUI();
}

bool UMingQuestPanel::PassesFilter(const FQuestData& QuestData) const
{
    // 檢查狀態過濾器
    bool bStatusFilterActive = false;
    for (const auto& FilterPair : StatusFilters)
    {
        if (FilterPair.Value)
        {
            bStatusFilterActive = true;
            if (FilterPair.Key == QuestData.QuestStatus)
            {
                break; // 匹配狀態過濾器
            }
        }
    }
    
    if (bStatusFilterActive)
    {
        bool bMatches = false;
        for (const auto& FilterPair : StatusFilters)
        {
            if (FilterPair.Value && FilterPair.Key == QuestData.QuestStatus)
            {
                bMatches = true;
                break;
            }
        }
        if (!bMatches) return false;
    }
    
    // 檢查類型過濾器
    bool bTypeFilterActive = false;
    for (const auto& FilterPair : TypeFilters)
    {
        if (FilterPair.Value)
        {
            bTypeFilterActive = true;
            if (FilterPair.Key == QuestData.QuestType)
            {
                break; // 匹配類型過濾器
            }
        }
    }
    
    if (bTypeFilterActive)
    {
        bool bMatches = false;
        for (const auto& FilterPair : TypeFilters)
        {
            if (FilterPair.Value && FilterPair.Key == QuestData.QuestType)
            {
                bMatches = true;
                break;
            }
        }
        if (!bMatches) return false;
    }
    
    // 檢查地區過濾器
    if (RegionFilters.Num() > 0 && !RegionFilters.Contains(QuestData.RegionID))
    {
        return false;
    }
    
    return true;
}

bool UMingQuestPanel::PassesSearch(const FQuestData& QuestData) const
{
    if (CurrentSearchText.IsEmpty())
    {
        return true;
    }
    
    return QuestData.QuestTitle.Contains(CurrentSearchText, ESearchCase::IgnoreCase) ||
           QuestData.QuestDescription.Contains(CurrentSearchText, ESearchCase::IgnoreCase) ||
           QuestData.QuestID.Contains(CurrentSearchText, ESearchCase::IgnoreCase);
}

void UMingQuestPanel::RefreshDisplayList()
{
    ProcessQuestData();
    UpdateUI();
}

void UMingQuestPanel::CheckQuestTimeLimits()
{
    // 檢查任務時間限制
    for (FQuestData& QuestData : AllQuests)
    {
        if (QuestData.bHasTimeLimit && QuestData.QuestStatus == EQuestStatus::Active)
        {
            FTimespan ElapsedTime = FDateTime::Now() - QuestData.StartTime;
            if (ElapsedTime.GetTotalSeconds() >= QuestData.TimeLimit)
            {
                FailQuest(QuestData.QuestID);
            }
        }
    }
}

void UMingQuestPanel::GenerateRandomQuests()
{
    // 生成一些測試任務
    FQuestData TestQuest1;
    TestQuest1.QuestID = TEXT("QUEST_001");
    TestQuest1.QuestTitle = TEXT("初次見面");
    TestQuest1.QuestDescription = TEXT("與北京的地區官員建立聯繫");
    TestQuest1.QuestType = EQuestType::Relationship;
    TestQuest1.QuestStatus = EQuestStatus::Available;
    TestQuest1.RegionID = TEXT("BEIJING");
    TestQuest1.QuestGiverID = TEXT("OFFICER_001");
    TestQuest1.SuggestedLevel = 1;
    TestQuest1.Difficulty = 1.0f;
    
    FQuestRequirement Requirement1;
    Requirement1.RequirementText = TEXT("與官員對話");
    Requirement1.TargetValue = 1.0f;
    TestQuest1.Requirements.Add(Requirement1);
    
    FQuestReward Reward1;
    Reward1.RewardType = TEXT("聲望");
    Reward1.Amount = 50;
    Reward1.Description = TEXT("北京地區聲望 +50");
    TestQuest1.Rewards.Add(Reward1);
    
    AddQuest(TestQuest1);
    
    FQuestData TestQuest2;
    TestQuest2.QuestID = TEXT("QUEST_002");
    TestQuest2.QuestTitle = TEXT("商業合作");
    TestQuest2.QuestDescription = TEXT("與上海的商人建立商業關係");
    TestQuest2.QuestType = EQuestType::Business;
    TestQuest2.QuestStatus = EQuestStatus::Active;
    TestQuest2.RegionID = TEXT("SHANGHAI");
    TestQuest2.QuestGiverID = TEXT("MERCHANT_001");
    TestQuest2.SuggestedLevel = 2;
    TestQuest2.Difficulty = 1.5f;
    TestQuest2.StartTime = FDateTime::Now();
    
    FQuestRequirement Requirement2;
    Requirement2.RequirementText = TEXT("完成商業談判");
    Requirement2.TargetValue = 1.0f;
    Requirement2.Progress = 0.5f;
    TestQuest2.Requirements.Add(Requirement2);
    
    FQuestReward Reward2;
    Reward2.RewardType = TEXT("金錢");
    Reward2.Amount = 1000;
    Reward2.Description = TEXT("獲得 1000 元");
    TestQuest2.Rewards.Add(Reward2);
    
    AddQuest(TestQuest2);
}
