#include "MingRelationshipPanel.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Engine/Engine.h"

UMingRelationshipPanel::UMingRelationshipPanel(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bIsFiltered(false)
    , bSortByValue(false)
    , bSortAscending(true)
    , RelationshipListView(nullptr)
    , TotalCountText(nullptr)
    , FriendCountText(nullptr)
    , EnemyCountText(nullptr)
    , AverageValueText(nullptr)
    , SearchBox(nullptr)
    , DetailsScrollBox(nullptr)
{
}

void UMingRelationshipPanel::NativeConstruct()
{
    Super::NativeConstruct();

    // 初始化過濾器
    ActiveFilters.Add(ERelationshipType::Friend, false);
    ActiveFilters.Add(ERelationshipType::Ally, false);
    ActiveFilters.Add(ERelationshipType::Rival, false);
    ActiveFilters.Add(ERelationshipType::Enemy, false);
    ActiveFilters.Add(ERelationshipType::Family, false);
    ActiveFilters.Add(ERelationshipType::Mentor, false);
    ActiveFilters.Add(ERelationshipType::Student, false);
    ActiveFilters.Add(ERelationshipType::Business, false);
    ActiveFilters.Add(ERelationshipType::Romantic, false);
    ActiveFilters.Add(ERelationshipType::Neutral, false);

    SetupEventBindings();
    UpdateUI();
}

void UMingRelationshipPanel::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMingRelationshipPanel::InitializeRelationshipPanel(UMingRelationshipManager* InRelationshipManager)
{
    RelationshipManager = InRelationshipManager;
    
    if (RelationshipManager)
    {
        UpdateRelationshipData();
        UE_LOG(LogTemp, Log, TEXT("Relationship panel initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Relationship manager is null - panel initialization failed"));
    }
}

void UMingRelationshipPanel::UpdateRelationshipData()
{
    if (!RelationshipManager)
    {
        return;
    }

    // 清空現有數據
    AllRelationships.Empty();
    DisplayedRelationships.Empty();

    // 獲取所有關係數據
    const TMap<FString, FRelationshipData>& Relationships = RelationshipManager->GetAllRelationships();
    
    for (const auto& RelationshipPair : Relationships)
    {
        FRelationshipDisplayData DisplayData = CreateDisplayData(RelationshipPair.Key, RelationshipPair.Value);
        AllRelationships.Add(DisplayData);
    }

    // 處理數據（過濾、排序）
    ProcessRelationshipData();
    
    // 更新UI
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Updated relationship data - Total: %d"), AllRelationships.Num());
}

void UMingRelationshipPanel::RefreshRelationshipList()
{
    UpdateRelationshipData();
}

void UMingRelationshipPanel::UpdateSelectedRelationship(const FString& CharacterID)
{
    if (!RelationshipManager)
    {
        return;
    }

    // 查找選中的關係
    for (const FRelationshipDisplayData& RelationshipData : DisplayedRelationships)
    {
        if (RelationshipData.CharacterID == CharacterID)
        {
            SelectedRelationship = RelationshipData;
            ShowRelationshipDetails(CharacterID);
            break;
        }
    }
}

void UMingRelationshipPanel::FilterByRelationshipType(ERelationshipType RelationshipType, bool bActive)
{
    ActiveFilters.Add(RelationshipType, bActive);
    
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

    ProcessRelationshipData();
    UpdateUI();
    
    OnRelationshipFilterChanged.Broadcast(RelationshipType, bActive);
    
    UE_LOG(LogTemp, Log, TEXT("Applied relationship filter: %d (Active: %s)"), 
           (int32)RelationshipType, bActive ? TEXT("true") : TEXT("false"));
}

void UMingRelationshipPanel::SortByValue(bool bAscending)
{
    bSortByValue = true;
    bSortAscending = bAscending;
    
    ProcessRelationshipData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Sorted relationships by value (Ascending: %s)"), 
           bAscending ? TEXT("true") : TEXT("false"));
}

void UMingRelationshipPanel::SortByName(bool bAscending)
{
    bSortByValue = false;
    bSortAscending = bAscending;
    
    ProcessRelationshipData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Sorted relationships by name (Ascending: %s)"), 
           bAscending ? TEXT("true") : TEXT("false"));
}

void UMingRelationshipPanel::ClearFilters()
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
    
    ProcessRelationshipData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Cleared all filters"));
}

void UMingRelationshipPanel::SearchRelationships(const FString& SearchText)
{
    CurrentSearchText = SearchText;
    ProcessRelationshipData();
    UpdateUI();
    
    UE_LOG(LogTemp, Log, TEXT("Searched relationships: %s"), *SearchText);
}

void UMingRelationshipPanel::ClearSearch()
{
    SearchRelationships(TEXT(""));
}

int32 UMingRelationshipPanel::GetTotalRelationshipCount() const
{
    return AllRelationships.Num();
}

int32 UMingRelationshipPanel::GetFriendCount() const
{
    int32 Count = 0;
    for (const FRelationshipDisplayData& RelationshipData : AllRelationships)
    {
        if (RelationshipData.RelationshipType == ERelationshipType::Friend || 
            RelationshipData.RelationshipType == ERelationshipType::Ally)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRelationshipPanel::GetEnemyCount() const
{
    int32 Count = 0;
    for (const FRelationshipDisplayData& RelationshipData : AllRelationships)
    {
        if (RelationshipData.RelationshipType == ERelationshipType::Enemy || 
            RelationshipData.RelationshipType == ERelationshipType::Rival)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRelationshipPanel::GetNeutralCount() const
{
    int32 Count = 0;
    for (const FRelationshipDisplayData& RelationshipData : AllRelationships)
    {
        if (RelationshipData.RelationshipType == ERelationshipType::Neutral)
        {
            Count++;
        }
    }
    return Count;
}

float UMingRelationshipPanel::GetAverageRelationshipValue() const
{
    if (AllRelationships.Num() == 0)
    {
        return 0.0f;
    }

    float Total = 0.0f;
    for (const FRelationshipDisplayData& RelationshipData : AllRelationships)
    {
        Total += RelationshipData.RelationshipValue;
    }
    
    return Total / AllRelationships.Num();
}

void UMingRelationshipPanel::ShowRelationshipDetails(const FString& CharacterID)
{
    if (!RelationshipManager)
    {
        return;
    }

    // 獲取詳細關係數據
    FRelationshipData RelationshipData = RelationshipManager->GetRelationship(CharacterID);
    
    // 這裡應該更新詳情面板的UI
    UE_LOG(LogTemp, Log, TEXT("Showing relationship details for: %s"), *CharacterID);
}

void UMingRelationshipPanel::HideRelationshipDetails()
{
    // 隱藏詳情面板
    UE_LOG(LogTemp, Log, TEXT("Hiding relationship details"));
}

void UMingRelationshipPanel::AddSharedMemory(const FString& CharacterID, const FString& Memory)
{
    if (RelationshipManager)
    {
        RelationshipManager->AddSharedMemory(CharacterID, Memory);
        UpdateRelationshipData();
    }
}

void UMingRelationshipPanel::RemoveSharedMemory(const FString& CharacterID, const FString& Memory)
{
    // 這需要在RelationshipManager中實現移除功能
    UE_LOG(LogTemp, Log, TEXT("Remove shared memory not yet implemented"));
}

void UMingRelationshipPanel::OnRelationshipClicked(const FString& CharacterID)
{
    UpdateSelectedRelationship(CharacterID);
    OnRelationshipSelected.Broadcast(CharacterID);
}

void UMingRelationshipPanel::OnRelationshipDoubleClicked(const FString& CharacterID)
{
    // 雙擊可能打開詳細對話或其他操作
    ShowRelationshipDetails(CharacterID);
}

void UMingRelationshipPanel::OnRelationshipRightClicked(const FString& CharacterID)
{
    // 右擊可能顯示上下文菜單
    UE_LOG(LogTemp, Log, TEXT("Right-clicked relationship: %s"), *CharacterID);
}

void UMingRelationshipPanel::ExportRelationshipData()
{
    // 導出關係數據到文件
    FString ExportData;
    
    for (const FRelationshipDisplayData& RelationshipData : AllRelationships)
    {
        ExportData += FString::Printf(TEXT("%s,%s,%.1f,%s\n"), 
                                      *RelationshipData.CharacterID,
                                      *RelationshipData.CharacterName,
                                      RelationshipData.RelationshipValue,
                                      *UEnum::GetValueAsString(RelationshipData.RelationshipType));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Exported relationship data (%d characters)"), ExportData.Len());
}

void UMingRelationshipPanel::ImportRelationshipData()
{
    // 從文件導入關係數據
    UE_LOG(LogTemp, Log, TEXT("Import relationship data not yet implemented"));
}

void UMingRelationshipPanel::ProcessRelationshipData()
{
    DisplayedRelationships.Empty();
    
    // 應用過濾器
    for (const FRelationshipDisplayData& RelationshipData : AllRelationships)
    {
        if (PassesFilter(RelationshipData) && PassesSearch(RelationshipData))
        {
            DisplayedRelationships.Add(RelationshipData);
        }
    }
    
    // 應用排序
    if (bSortByValue)
    {
        DisplayedRelationships.Sort([this](const FRelationshipDisplayData& A, const FRelationshipDisplayData& B)
        {
            return bSortAscending ? (A.RelationshipValue < B.RelationshipValue) : (A.RelationshipValue > B.RelationshipValue);
        });
    }
    else
    {
        DisplayedRelationships.Sort([this](const FRelationshipDisplayData& A, const FRelationshipDisplayData& B)
        {
            return bSortAscending ? (A.CharacterName < B.CharacterName) : (A.CharacterName > B.CharacterName);
        });
    }
}

void UMingRelationshipPanel::ApplyFilters()
{
    ProcessRelationshipData();
}

void UMingRelationshipPanel::ApplySorting()
{
    ProcessRelationshipData();
}

void UMingRelationshipPanel::UpdateStatistics()
{
    if (!TotalCountText || !FriendCountText || !EnemyCountText || !AverageValueText)
    {
        return;
    }

    TotalCountText->SetText(FText::AsNumber(GetTotalRelationshipCount()));
    FriendCountText->SetText(FText::AsNumber(GetFriendCount()));
    EnemyCountText->SetText(FText::AsNumber(GetEnemyCount()));
    
    FString AverageText = FString::Printf(TEXT("%.1f"), GetAverageRelationshipValue());
    AverageValueText->SetText(FText::FromString(AverageText));
}

void UMingRelationshipPanel::UpdateUI()
{
    UpdateStatistics();
    
    // 更新列表視圖
    if (RelationshipListView)
    {
        RelationshipListView->ClearListItems();
        
        for (const FRelationshipDisplayData& RelationshipData : DisplayedRelationships)
        {
            // 這裡應該創建並添加列表項
            // RelationshipListView->AddItem(CreateListItem(RelationshipData));
        }
    }
}

FRelationshipDisplayData UMingRelationshipPanel::CreateDisplayData(const FString& CharacterID, const FRelationshipData& RelationshipData)
{
    FRelationshipDisplayData DisplayData;
    
    DisplayData.CharacterID = CharacterID;
    DisplayData.CharacterName = CharacterID; // 實際實現時應該從角色數據獲取名稱
    DisplayData.RelationshipType = RelationshipData.RelationshipType;
    DisplayData.RelationshipValue = RelationshipData.RelationshipValue;
    DisplayData.RelationshipDescription = GetRelationshipDescription(RelationshipData.RelationshipType, RelationshipData.RelationshipValue);
    DisplayData.RelationshipColor = GetRelationshipColor(RelationshipData.RelationshipType, RelationshipData.RelationshipValue);
    DisplayData.SharedMemories = RelationshipData.SharedMemories;
    
    return DisplayData;
}

FLinearColor UMingRelationshipPanel::GetRelationshipColor(ERelationshipType RelationshipType, float Value) const
{
    switch (RelationshipType)
    {
        case ERelationshipType::Friend:
        case ERelationshipType::Ally:
            return FLinearColor(0.0f, 0.8f, 0.0f, 1.0f); // 綠色
        case ERelationshipType::Enemy:
        case ERelationshipType::Rival:
            return FLinearColor(0.8f, 0.0f, 0.0f, 1.0f); // 紅色
        case ERelationshipType::Romantic:
            return FLinearColor(0.8f, 0.0f, 0.8f, 1.0f); // 粉色
        case ERelationshipType::Family:
            return FLinearColor(0.0f, 0.0f, 0.8f, 1.0f); // 藍色
        case ERelationshipType::Mentor:
        case ERelationshipType::Student:
            return FLinearColor(0.8f, 0.8f, 0.0f, 1.0f); // 黃色
        case ERelationshipType::Business:
            return FLinearColor(0.0f, 0.8f, 0.8f, 1.0f); // 青色
        case ERelationshipType::Neutral:
        default:
            return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // 灰色
    }
}

FString UMingRelationshipPanel::GetRelationshipDescription(ERelationshipType RelationshipType, float Value) const
{
    FString TypeString;
    switch (RelationshipType)
    {
        case ERelationshipType::Friend: TypeString = TEXT("朋友"); break;
        case ERelationshipType::Ally: TypeString = TEXT("盟友"); break;
        case ERelationshipType::Rival: TypeString = TEXT("競爭對手"); break;
        case ERelationshipType::Enemy: TypeString = TEXT("敵人"); break;
        case ERelationshipType::Family: TypeString = TEXT("家人"); break;
        case ERelationshipType::Mentor: TypeString = TEXT("導師"); break;
        case ERelationshipType::Student: TypeString = TEXT("學生"); break;
        case ERelationshipType::Business: TypeString = TEXT("商業夥伴"); break;
        case ERelationshipType::Romantic: TypeString = TEXT("戀愛對象"); break;
        case ERelationshipType::Neutral: 
        default: TypeString = TEXT("中立"); break;
    }
    
    return FString::Printf(TEXT("%s (%.1f)"), *TypeString, Value);
}

void UMingRelationshipPanel::SetupEventBindings()
{
    // 綁定搜索框事件
    if (SearchBox)
    {
        SearchBox->OnTextChanged.AddDynamic(this, &UMingRelationshipPanel::SearchRelationships);
    }
    
    // 綁定關係管理器事件
    if (RelationshipManager)
    {
        // 這裡需要綁定關係變化事件
        // RelationshipManager->OnRelationshipChanged.AddDynamic(this, &UMingRelationshipPanel::OnRelationshipDataChanged);
    }
}

void UMingRelationshipPanel::OnRelationshipDataChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason)
{
    UpdateRelationshipData();
}

bool UMingRelationshipPanel::PassesFilter(const FRelationshipDisplayData& RelationshipData) const
{
    if (!bIsFiltered)
    {
        return true;
    }
    
    // 檢查是否匹配任何活動過濾器
    for (const auto& FilterPair : ActiveFilters)
    {
        if (FilterPair.Value && FilterPair.Key == RelationshipData.RelationshipType)
        {
            return true;
        }
    }
    
    return false;
}

bool UMingRelationshipPanel::PassesSearch(const FRelationshipDisplayData& RelationshipData) const
{
    if (CurrentSearchText.IsEmpty())
    {
        return true;
    }
    
    // 搜索字符ID和名稱
    return RelationshipData.CharacterID.Contains(CurrentSearchText, ESearchCase::IgnoreCase) ||
           RelationshipData.CharacterName.Contains(CurrentSearchText, ESearchCase::IgnoreCase);
}

void UMingRelationshipPanel::RefreshDisplayList()
{
    ProcessRelationshipData();
    UpdateUI();
}
