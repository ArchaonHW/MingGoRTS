#include "MingReputationPanel.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Engine/Engine.h"

UMingReputationPanel::UMingReputationPanel(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bIsFiltered(false)
    , bSortByScore(false)
    , bSortAscending(true)
    , ReputationListView(nullptr)
    , TotalRegionText(nullptr)
    , GoodReputationText(nullptr)
    , BadReputationText(nullptr)
    , AverageScoreText(nullptr)
    , HighestLevelText(nullptr)
    , SearchBox(nullptr)
    , DetailsScrollBox(nullptr)
    , QuestScrollBox(nullptr)
{
}

void UMingReputationPanel::NativeConstruct()
{
    Super::NativeConstruct();

    // 初始化過濾器
    ActiveFilters.Add(EReputationLevel::Unknown, false);
    ActiveFilters.Add(EReputationLevel::Disliked, false);
    ActiveFilters.Add(EReputationLevel::Neutral, false);
    ActiveFilters.Add(EReputationLevel::Respected, false);
    ActiveFilters.Add(EReputationLevel::Honored, false);
    ActiveFilters.Add(EReputationLevel::Legendary, false);

    SetupEventBindings();
    UpdateUI();
}

void UMingReputationPanel::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMingReputationPanel::InitializeReputationPanel(UMingRelationshipManager* InRelationshipManager)
{
    RelationshipManager = InRelationshipManager;
    
    if (RelationshipManager)
    {
        UpdateReputationData();
        UE_LOG(LogTemp, Log, TEXT("Reputation panel initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Relationship manager is null - panel initialization failed"));
    }
}

void UMingReputationPanel::UpdateReputationData()
{
    if (!RelationshipManager)
    {
        return;
    }

    // 清空現有數據
    AllReputations.Empty();
    DisplayedReputations.Empty();

    // 獲取所有聲望數據
    const TMap<FString, FReputationData>& Reputations = RelationshipManager->GetAllReputations();
    
    for (const auto& ReputationPair : Reputations)
    {
        FReputationDisplayData DisplayData = CreateDisplayData(ReputationPair.Key, ReputationPair.Value);
        AllReputations.Add(DisplayData);
    }

    // 處理數據（過濾、排序）
    ProcessReputationData();
    
    // 更新UI
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Updated reputation data - Total: %d"), AllReputations.Num());
}

void UMingReputationPanel::RefreshReputationList()
{
    UpdateReputationData();
}

void UMingReputationPanel::UpdateSelectedReputation(const FString& RegionID)
{
    if (!RelationshipManager)
    {
        return;
    }

    // 查找選中的聲望
    for (const FReputationDisplayData& ReputationData : DisplayedReputations)
    {
        if (ReputationData.RegionID == RegionID)
        {
            SelectedReputation = ReputationData;
            ShowReputationDetails(RegionID);
            break;
        }
    }
}

void UMingReputationPanel::FilterByReputationLevel(EReputationLevel ReputationLevel, bool bActive)
{
    ActiveFilters.Add(ReputationLevel, bActive);
    
    // 檢查是否有任何活動過濾器
    bIsFiltered = false;
    for (const auto& FilterPair : ActiveFilters)
    {
        if (FilterPair.Value)
        {
            bIsFiltered = true;
            break;
        }
    }

    ProcessReputationData();
    UpdateUI();
    
    OnReputationFilterChanged.Broadcast(ReputationLevel, bActive);
    
    UE_LOG(LogTemp, Log, TEXT("Applied reputation filter: %d (Active: %s)"), 
           (int32)ReputationLevel, bActive ? TEXT("true") : TEXT("false"));
}

void UMingReputationPanel::SortByScore(bool bAscending)
{
    bSortByScore = true;
    bSortAscending = bAscending;
    
    ProcessReputationData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Sorted reputations by score (Ascending: %s)"), 
           bAscending ? TEXT("true") : TEXT("false"));
}

void UMingReputationPanel::SortByName(bool bAscending)
{
    bSortByScore = false;
    bSortAscending = bAscending;
    
    ProcessReputationData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Sorted reputations by name (Ascending: %s)"), 
           bAscending ? TEXT("true") : TEXT("false"));
}

void UMingReputationPanel::ClearFilters()
{
    // 清空所有過濾器
    for (auto& FilterPair : ActiveFilters)
    {
        FilterPair.Value = false;
    }
    
    bIsFiltered = false;
    CurrentSearchText.Empty();
    
    if (SearchBox)
    {
        SearchBox->SetText(FText::GetEmpty());
    }
    
    ProcessReputationData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Cleared all filters"));
}

void UMingReputationPanel::SearchReputations(const FString& SearchText)
{
    CurrentSearchText = SearchText;
    ProcessReputationData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Searched reputations: %s"), *SearchText);
}

void UMingReputationPanel::ClearSearch()
{
    SearchReputations(TEXT(""));
}

int32 UMingReputationPanel::GetTotalRegionCount() const
{
    return AllReputations.Num();
}

int32 UMingReputationPanel::GetGoodReputationCount() const
{
    int32 Count = 0;
    for (const FReputationDisplayData& ReputationData : AllReputations)
    {
        if (ReputationData.ReputationLevel == EReputationLevel::Respected ||
            ReputationData.ReputationLevel == EReputationLevel::Honored ||
            ReputationData.ReputationLevel == EReputationLevel::Legendary)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingReputationPanel::GetBadReputationCount() const
{
    int32 Count = 0;
    for (const FReputationDisplayData& ReputationData : AllReputations)
    {
        if (ReputationData.ReputationLevel == EReputationLevel::Unknown ||
            ReputationData.ReputationLevel == EReputationLevel::Disliked)
        {
            Count++;
        }
    }
    return Count;
}

float UMingReputationPanel::GetAverageReputationScore() const
{
    if (AllReputations.Num() == 0)
    {
        return 0.0f;
    }

    float Total = 0.0f;
    for (const FReputationDisplayData& ReputationData : AllReputations)
    {
        Total += ReputationData.ReputationScore;
    }
    
    return Total / AllReputations.Num();
}

EReputationLevel UMingReputationPanel::GetHighestReputationLevel() const
{
    EReputationLevel HighestLevel = EReputationLevel::Unknown;
    
    for (const FReputationDisplayData& ReputationData : AllReputations)
    {
        if (ReputationData.ReputationLevel > HighestLevel)
        {
            HighestLevel = ReputationData.ReputationLevel;
        }
    }
    
    return HighestLevel;
}

void UMingReputationPanel::ShowReputationDetails(const FString& RegionID)
{
    if (!RelationshipManager)
    {
        return;
    }

    // 獲取詳細聲望數據
    FReputationData ReputationData = RelationshipManager->GetReputation(RegionID);
    
    // 這裡應該更新詳情面板的UI
    UE_LOG(LogTemp, Log, TEXT("Showing reputation details for: %s"), *RegionID);
}

void UMingReputationPanel::HideReputationDetails()
{
    // 隱藏詳情面板
    UE_LOG(LogTemp, Log, TEXT("Hiding reputation details"));
}

void UMingReputationPanel::ShowQuestList(const FString& RegionID)
{
    // 顯示該地區的任務列表
    UE_LOG(LogTemp, Log, TEXT("Showing quest list for region: %s"), *RegionID);
}

void UMingReputationPanel::HideQuestList()
{
    // 隱藏任務列表
    UE_LOG(LogTemp, Log, TEXT("Hiding quest list"));
}

void UMingReputationPanel::OnReputationClicked(const FString& RegionID)
{
    UpdateSelectedReputation(RegionID);
    OnReputationSelected.Broadcast(RegionID);
}

void UMingReputationPanel::OnReputationDoubleClicked(const FString& RegionID)
{
    // 雙擊可能打開詳細對話或任務列表
    ShowQuestList(RegionID);
}

void UMingReputationPanel::OnReputationRightClicked(const FString& RegionID)
{
    // 右擊可能顯示上下文菜單
    UE_LOG(LogTemp, Log, TEXT("Right-clicked reputation: %s"), *RegionID);
}

bool UMingReputationPanel::CanAcceptQuestInRegion(const FString& RegionID) const
{
    if (!RelationshipManager)
    {
        return false;
    }

    return RelationshipManager->CanAcceptQuest(TEXT("DEFAULT_QUEST"), RegionID);
}

TArray<FString> UMingReputationPanel::GetAvailableQuestsInRegion(const FString& RegionID) const
{
    // 這裡應該從任務管理器獲取可用任務
    TArray<FString> AvailableQuests;
    
    // 暫時返回模擬數據
    if (CanAcceptQuestInRegion(RegionID))
    {
        AvailableQuests.Add(TEXT("地區任務1"));
        AvailableQuests.Add(TEXT("地區任務2"));
        AvailableQuests.Add(TEXT("地區任務3"));
    }
    
    return AvailableQuests;
}

void UMingReputationPanel::ExportReputationData()
{
    // 導出聲望數據到文件
    FString ExportData;
    
    for (const FReputationDisplayData& ReputationData : AllReputations)
    {
        ExportData += FString::Printf(TEXT("%s,%s,%.1f,%s\n"), 
                                      *ReputationData.RegionID,
                                      *ReputationData.RegionName,
                                      ReputationData.ReputationScore,
                                      *UEnum::GetValueAsString(ReputationData.ReputationLevel));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Exported reputation data (%d regions)"), ExportData.Len());
}

void UMingReputationPanel::ImportReputationData()
{
    // 從文件導入聲望數據
    UE_LOG(LogTemp, Log, TEXT("Import reputation data not yet implemented"));
}

void UMingReputationPanel::ProcessReputationData()
{
    DisplayedReputations.Empty();
    
    // 應用過濾器
    for (const FReputationDisplayData& ReputationData : AllReputations)
    {
        if (PassesFilter(ReputationData) && PassesSearch(ReputationData))
        {
            DisplayedReputations.Add(ReputationData);
        }
    }
    
    // 應用排序
    if (bSortByScore)
    {
        DisplayedReputations.Sort([this](const FReputationDisplayData& A, const FReputationDisplayData& B)
        {
            return bSortAscending ? (A.ReputationScore < B.ReputationScore) : (A.ReputationScore > B.ReputationScore);
        });
    }
    else
    {
        DisplayedReputations.Sort([this](const FReputationDisplayData& A, const FReputationDisplayData& B)
        {
            return bSortAscending ? (A.RegionName < B.RegionName) : (A.RegionName > B.RegionName);
        });
    }
}

void UMingReputationPanel::ApplyFilters()
{
    ProcessReputationData();
}

void UMingReputationPanel::ApplySorting()
{
    ProcessReputationData();
}

void UMingReputationPanel::UpdateStatistics()
{
    if (!TotalRegionText || !GoodReputationText || !BadReputationText || !AverageScoreText || !HighestLevelText)
    {
        return;
    }

    TotalRegionText->SetText(FText::AsNumber(GetTotalRegionCount()));
    GoodReputationText->SetText(FText::AsNumber(GetGoodReputationCount()));
    BadReputationText->SetText(FText::AsNumber(GetBadReputationCount()));
    
    FString AverageText = FString::Printf(TEXT("%.1f"), GetAverageReputationScore());
    AverageScoreText->SetText(FText::FromString(AverageText));
    
    FString HighestText = GetReputationDescription(GetHighestReputationLevel(), 0.0f);
    HighestLevelText->SetText(FText::FromString(HighestText));
}

void UMingReputationPanel::UpdateUI()
{
    UpdateStatistics();
    
    // 更新列表視圖
    if (ReputationListView)
    {
        ReputationListView->ClearListItems();
        
        for (const FReputationDisplayData& ReputationData : DisplayedReputations)
        {
            // 這裡應該創建並添加列表項
            // ReputationListView->AddItem(CreateListItem(ReputationData));
        }
    }
}

FReputationDisplayData UMingReputationPanel::CreateDisplayData(const FString& RegionID, const FReputationData& ReputationData)
{
    FReputationDisplayData DisplayData;
    
    DisplayData.RegionID = RegionID;
    DisplayData.RegionName = GetRegionDisplayName(RegionID);
    DisplayData.ReputationLevel = ReputationData.ReputationLevel;
    DisplayData.ReputationScore = ReputationData.ReputationScore;
    DisplayData.ReputationDescription = GetReputationDescription(ReputationData.ReputationLevel, ReputationData.ReputationScore);
    DisplayData.ReputationColor = GetReputationColor(ReputationData.ReputationLevel);
    
    // 模擬任務數據
    DisplayData.AvailableQuests = CanAcceptQuestInRegion(RegionID) ? 3 : 0;
    DisplayData.CompletedQuests = FMath::RandRange(0, 10);
    DisplayData.QuestSuccessRate = DisplayData.CompletedQuests > 0 ? FMath::RandRange(0.5f, 1.0f) : 0.0f;
    
    return DisplayData;
}

FLinearColor UMingReputationPanel::GetReputationColor(EReputationLevel ReputationLevel) const
{
    switch (ReputationLevel)
    {
        case EReputationLevel::Legendary:
            return FLinearColor(1.0f, 0.8f, 0.0f, 1.0f); // 金色
        case EReputationLevel::Honored:
            return FLinearColor(0.8f, 0.4f, 0.0f, 1.0f); // 橙色
        case EReputationLevel::Respected:
            return FLinearColor(0.0f, 0.8f, 0.0f, 1.0f); // 綠色
        case EReputationLevel::Neutral:
            return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // 灰色
        case EReputationLevel::Disliked:
            return FLinearColor(0.8f, 0.0f, 0.0f, 1.0f); // 紅色
        case EReputationLevel::Unknown:
        default:
            return FLinearColor(0.3f, 0.3f, 0.3f, 1.0f); // 深灰色
    }
}

FString UMingReputationPanel::GetReputationDescription(EReputationLevel ReputationLevel, float Score) const
{
    FString LevelString;
    switch (ReputationLevel)
    {
        case EReputationLevel::Legendary: LevelString = TEXT("傳奇"); break;
        case EReputationLevel::Honored: LevelString = TEXT("受敬重"); break;
        case EReputationLevel::Respected: LevelString = TEXT("受尊敬"); break;
        case EReputationLevel::Neutral: LevelString = TEXT("中立"); break;
        case EReputationLevel::Disliked: LevelString = TEXT("被厭惡"); break;
        case EReputationLevel::Unknown: 
        default: LevelString = TEXT("未知"); break;
    }
    
    return FString::Printf(TEXT("%s (%.0f)"), *LevelString, Score);
}

FString UMingReputationPanel::GetRegionDisplayName(const FString& RegionID) const
{
    // 地區ID到顯示名稱的映射
    if (RegionID == TEXT("BEIJING")) return TEXT("北京");
    if (RegionID == TEXT("SHANGHAI")) return TEXT("上海");
    if (RegionID == TEXT("NANJING")) return TEXT("南京");
    if (RegionID == TEXT("GUANGZHOU")) return TEXT("廣州");
    if (RegionID == TEXT("TIANJIN")) return TEXT("天津");
    if (RegionID == TEXT("WUHAN")) return TEXT("武漢");
    if (RegionID == TEXT("CHONGQING")) return TEXT("重慶");
    if (RegionID == TEXT("XIAN")) return TEXT("西安");
    if (RegionID == TEXT("CHENGDU")) return TEXT("成都");
    if (RegionID == TEXT("HARBIN")) return TEXT("哈爾濱");
    
    return RegionID; // 默認返回ID
}

void UMingReputationPanel::SetupEventBindings()
{
    // 綁定搜索框事件
    if (SearchBox)
    {
        SearchBox->OnTextChanged.AddDynamic(this, &UMingReputationPanel::SearchReputations);
    }
    
    // 綁定聲望管理器事件
    if (RelationshipManager)
    {
        // 這裡需要綁定聲望變化事件
        // RelationshipManager->OnReputationChanged.AddDynamic(this, &UMingReputationPanel::OnReputationDataChanged);
    }
}

void UMingReputationPanel::OnReputationDataChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason)
{
    UpdateReputationData();
}

bool UMingReputationPanel::PassesFilter(const FReputationDisplayData& ReputationData) const
{
    if (!bIsFiltered)
    {
        return true;
    }
    
    // 檢查是否匹配任何活動過濾器
    for (const auto& FilterPair : ActiveFilters)
    {
        if (FilterPair.Value && FilterPair.Key == ReputationData.ReputationLevel)
        {
            return true;
        }
    }
    
    return false;
}

bool UMingReputationPanel::PassesSearch(const FReputationDisplayData& ReputationData) const
{
    if (CurrentSearchText.IsEmpty())
    {
        return true;
    }
    
    // 搜索地區ID和名稱
    return ReputationData.RegionID.Contains(CurrentSearchText, ESearchCase::IgnoreCase) ||
           ReputationData.RegionName.Contains(CurrentSearchText, ESearchCase::IgnoreCase);
}

void UMingReputationPanel::RefreshDisplayList()
{
    ProcessReputationData();
    UpdateUI();
}

EReputationLevel UMingReputationPanel::CalculateReputationLevel(float Score) const
{
    if (Score >= 800.0f)
    {
        return EReputationLevel::Legendary;
    }
    else if (Score >= 600.0f)
    {
        return EReputationLevel::Honored;
    }
    else if (Score >= 400.0f)
    {
        return EReputationLevel::Respected;
    }
    else if (Score >= 200.0f)
    {
        return EReputationLevel::Neutral;
    }
    else if (Score >= 50.0f)
    {
        return EReputationLevel::Disliked;
    }
    else
    {
        return EReputationLevel::Unknown;
    }
}
