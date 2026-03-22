#include "Tutorial/MingRTSCodexSystem.h"

UMingRTSCodexSystem::UMingRTSCodexSystem()
{
}

void UMingRTSCodexSystem::InitializeCodexSystem()
{
    LoadCodexData();
    LoadViewHistory();
    
    UE_LOG(LogTemp, Log, TEXT("Codex System Initialized with %d sections"), Sections.Num());
}

TArray<FCodexSection> UMingRTSCodexSystem::GetAllSections() const
{
    return Sections;
}

TArray<FCodexEntry> UMingRTSCodexSystem::GetEntriesByType(ECodexEntryType Type) const
{
    TArray<FCodexEntry> Result;
    
    for (const FCodexSection& Section : Sections)
    {
        if (Section.SectionType == Type)
        {
            for (const FCodexEntry& Entry : Section.Entries)
            {
                if (!Entry.bIsLocked)
                {
                    Result.Add(Entry);
                }
            }
        }
    }
    
    return Result;
}

bool UMingRTSCodexSystem::GetEntry(FName EntryID, FCodexEntry& OutEntry) const
{
    for (const FCodexSection& Section : Sections)
    {
        for (const FCodexEntry& Entry : Section.Entries)
        {
            if (Entry.EntryID == EntryID)
            {
                OutEntry = Entry;
                return true;
            }
        }
    }
    
    return false;
}

TArray<FCodexEntry> UMingRTSCodexSystem::SearchEntries(const FString& SearchQuery) const
{
    TArray<FCodexEntry> Results;
    FString LowerQuery = SearchQuery.ToLower();
    
    for (const FCodexSection& Section : Sections)
    {
        for (const FCodexEntry& Entry : Section.Entries)
        {
            if (Entry.bIsLocked)
            {
                continue;
            }
            
            bool bMatch = false;
            
            // 檢查標題
            if (Entry.Title.ToString().ToLower().Contains(LowerQuery))
            {
                bMatch = true;
            }
            // 檢查描述
            else if (Entry.Description.ToString().ToLower().Contains(LowerQuery))
            {
                bMatch = true;
            }
            // 檢查標籤
            else
            {
                for (const FString& Tag : Entry.Tags)
                {
                    if (Tag.ToLower().Contains(LowerQuery))
                    {
                        bMatch = true;
                        break;
                    }
                }
            }
            
            if (bMatch)
            {
                Results.Add(Entry);
            }
        }
    }
    
    return Results;
}

void UMingRTSCodexSystem::ViewEntry(FName EntryID)
{
    for (FCodexSection& Section : Sections)
    {
        for (FCodexEntry& Entry : Section.Entries)
        {
            if (Entry.EntryID == EntryID)
            {
                Entry.ViewCount++;
                Entry.LastViewedTime = FDateTime::Now().ToString();
                
                ViewHistory.Add(EntryID, Entry.ViewCount);
                ViewTimestamps.Add(EntryID, Entry.LastViewedTime);
                
                OnEntryViewed.Broadcast(EntryID);
                
                UE_LOG(LogTemp, Log, TEXT("Viewed codex entry: %s"), *EntryID.ToString());
                return;
            }
        }
    }
}

bool UMingRTSCodexSystem::IsEntryUnlocked(FName EntryID) const
{
    for (const FCodexSection& Section : Sections)
    {
        for (const FCodexEntry& Entry : Section.Entries)
        {
            if (Entry.EntryID == EntryID)
            {
                return !Entry.bIsLocked;
            }
        }
    }
    
    return false;
}

void UMingRTSCodexSystem::UnlockEntry(FName EntryID)
{
    for (FCodexSection& Section : Sections)
    {
        for (FCodexEntry& Entry : Section.Entries)
        {
            if (Entry.EntryID == EntryID)
            {
                if (Entry.bIsLocked)
                {
                    Entry.bIsLocked = false;
                    OnEntryUnlocked.Broadcast(EntryID);
                    UE_LOG(LogTemp, Log, TEXT("Unlocked codex entry: %s"), *EntryID.ToString());
                }
                return;
            }
        }
    }
}

TArray<FCodexEntry> UMingRTSCodexSystem::GetRecommendedEntries() const
{
    TArray<FCodexEntry> Results;
    
    // 基於玩家進度推薦未閱讀或冷門但重要的條目
    // 這裡提供基礎實現
    
    TArray<FCodexEntry> AllUnlocked;
    for (const FCodexSection& Section : Sections)
    {
        for (const FCodexEntry& Entry : Section.Entries)
        {
            if (!Entry.bIsLocked)
            {
                AllUnlocked.Add(Entry);
            }
        }
    }
    
    // 按查看次數排序，推薦查看少的
    AllUnlocked.Sort([](const FCodexEntry& A, const FCodexEntry& B) {
        return A.ViewCount < B.ViewCount;
    });
    
    // 返回前5個
    for (int32 i = 0; i < FMath::Min(5, AllUnlocked.Num()); ++i)
    {
        Results.Add(AllUnlocked[i]);
    }
    
    return Results;
}

TArray<FCodexEntry> UMingRTSCodexSystem::GetRecentlyViewed(int32 Count) const
{
    TArray<FCodexEntry> Results;
    TArray<FCodexEntry> AllEntries;
    
    // 收集所有有查看記錄的條目
    for (const FCodexSection& Section : Sections)
    {
        for (const FCodexEntry& Entry : Section.Entries)
        {
            if (Entry.ViewCount > 0)
            {
                AllEntries.Add(Entry);
            }
        }
    }
    
    // 按最後查看時間排序
    AllEntries.Sort([](const FCodexEntry& A, const FCodexEntry& B) {
        return A.LastViewedTime > B.LastViewedTime;
    });
    
    // 返回前N個
    for (int32 i = 0; i < FMath::Min(Count, AllEntries.Num()); ++i)
    {
        Results.Add(AllEntries[i]);
    }
    
    return Results;
}

TArray<FCodexEntry> UMingRTSCodexSystem::GetPopularEntries(int32 Count) const
{
    TArray<FCodexEntry> Results;
    TArray<FCodexEntry> AllEntries;
    
    for (const FCodexSection& Section : Sections)
    {
        for (const FCodexEntry& Entry : Section.Entries)
        {
            if (!Entry.bIsLocked)
            {
                AllEntries.Add(Entry);
            }
        }
    }
    
    // 按查看次數排序
    AllEntries.Sort([](const FCodexEntry& A, const FCodexEntry& B) {
        return A.ViewCount > B.ViewCount;
    });
    
    for (int32 i = 0; i < FMath::Min(Count, AllEntries.Num()); ++i)
    {
        Results.Add(AllEntries[i]);
    }
    
    return Results;
}

TArray<FCodexEntry> UMingRTSCodexSystem::GetEntriesByTag(const FString& Tag) const
{
    TArray<FCodexEntry> Results;
    FString LowerTag = Tag.ToLower();
    
    for (const FCodexSection& Section : Sections)
    {
        for (const FCodexEntry& Entry : Section.Entries)
        {
            if (Entry.bIsLocked)
            {
                continue;
            }
            
            for (const FString& EntryTag : Entry.Tags)
            {
                if (EntryTag.ToLower() == LowerTag)
                {
                    Results.Add(Entry);
                    break;
                }
            }
        }
    }
    
    return Results;
}

TArray<FCodexEntry> UMingRTSCodexSystem::GetRelatedEntries(FName EntryID) const
{
    TArray<FCodexEntry> Results;
    FCodexEntry SourceEntry;
    
    if (!GetEntry(EntryID, SourceEntry))
    {
        return Results;
    }
    
    for (FName RelatedID : SourceEntry.RelatedEntries)
    {
        FCodexEntry RelatedEntry;
        if (GetEntry(RelatedID, RelatedEntry) && !RelatedEntry.bIsLocked)
        {
            Results.Add(RelatedEntry);
        }
    }
    
    return Results;
}

void UMingRTSCodexSystem::SaveViewHistory()
{
    // 保存到遊戲存檔系統
    UE_LOG(LogTemp, Log, TEXT("Codex view history saved"));
}

void UMingRTSCodexSystem::LoadViewHistory()
{
    // 從遊戲存檔系統載入
    UE_LOG(LogTemp, Log, TEXT("Codex view history loaded"));
}

void UMingRTSCodexSystem::LoadCodexData()
{
    Sections.Empty();
    
    InitializeGameBasicsSection();
    InitializeFactionSection();
    InitializeUnitSection();
    InitializeBuildingSection();
    InitializeTechnologySection();
    InitializeHistoricalSection();
    InitializeStrategySection();
    InitializeLoreSection();
    
    // 按排序順序排序章節
    Sections.Sort([](const FCodexSection& A, const FCodexSection& B) {
        return A.SortOrder < B.SortOrder;
    });
}

void UMingRTSCodexSystem::InitializeGameBasicsSection()
{
    FCodexSection Section;
    Section.SectionType = ECodexEntryType::GameBasics;
    Section.SectionName = FText::FromString(TEXT("遊戲基礎"));
    Section.SectionDescription = FText::FromString(TEXT("學習遊戲的基本操作和界面"));
    Section.SortOrder = 1;
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::GameBasics, TEXT("Controls"),
        FText::FromString(TEXT("基本操作")),
        FText::FromString(TEXT("WASD移動視角，滑鼠左鍵選擇，右鍵下達命令。滾輪縮放，按住右鍵旋轉視角。")),
        {TEXT("操作"), TEXT("基礎")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::GameBasics, TEXT("Resources"),
        FText::FromString(TEXT("資源系統")),
        FText::FromString(TEXT("遊戲中主要有三種資源：糧食(維持部隊)、金錢(建造和訓練)、鋼鐵(高級單位)。資源來源於建築生產和貿易。")),
        {TEXT("資源"), TEXT("經濟")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::GameBasics, TEXT("UI"),
        FText::FromString(TEXT("界面說明")),
        FText::FromString(TEXT("左上角顯示資源，右上角是小地圖，底部是選中單位的信息和命令面板。")),
        {TEXT("界面"), TEXT("UI")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::GameBasics, TEXT("Hotkeys"),
        FText::FromString(TEXT("快捷鍵")),
        FText::FromString(TEXT("常用快捷鍵：1-9選擇編隊，Ctrl+數字創建編隊，A鍵攻擊移動，H鍵駐守，M鍵巡邏。")),
        {TEXT("快捷鍵"), TEXT("操作")}));
    
    Sections.Add(Section);
}

void UMingRTSCodexSystem::InitializeFactionSection()
{
    FCodexSection Section;
    Section.SectionType = ECodexEntryType::FactionInfo;
    Section.SectionName = FText::FromString(TEXT("勢力介紹"));
    Section.SectionDescription = FText::FromString(TEXT("了解各個可玩勢力的特色和玩法"));
    Section.SortOrder = 2;
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::FactionInfo, TEXT("Faction_Beiyang"),
        FText::FromString(TEXT("北洋政府")),
        FText::FromString(TEXT("北洋政府擁有中央正統地位，外交關係較好，可以獲得外國援助。適合新手玩家。")),
        {TEXT("勢力"), TEXT("北洋")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::FactionInfo, TEXT("Faction_Guomindang"),
        FText::FromString(TEXT("國民政府")),
        FText::FromString(TEXT("國民政府代表民族主義力量，現代化進程快，科技研發速度較快。")),
        {TEXT("勢力"), TEXT("國民政府")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::FactionInfo, TEXT("Faction_CCP"),
        FText::FromString(TEXT("中國共產黨")),
        FText::FromString(TEXT("共產黨擅長人民戰爭和游擊戰術，民眾支持度高，游擊單位成本低。")),
        {TEXT("勢力"), TEXT("共產黨")}));
    
    Sections.Add(Section);
}

void UMingRTSCodexSystem::InitializeUnitSection()
{
    FCodexSection Section;
    Section.SectionType = ECodexEntryType::UnitInfo;
    Section.SectionName = FText::FromString(TEXT("單位圖鑑"));
    Section.SectionDescription = FText::FromString(TEXT("查看所有單位的詳細屬性和能力"));
    Section.SortOrder = 3;
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::UnitInfo, TEXT("Unit_Militia"),
        FText::FromString(TEXT("民兵")),
        FText::FromString(TEXT("基礎步兵單位，訓練快速成本低，但戰鬥力較弱。適合早期防禦和騷擾。")),
        {TEXT("單位"), TEXT("步兵")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::UnitInfo, TEXT("Unit_Regular"),
        FText::FromString(TEXT("正規軍")),
        FText::FromString(TEXT("標準步兵單位，攻防平衡，是軍隊的主力。")),
        {TEXT("單位"), TEXT("步兵")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::UnitInfo, TEXT("Unit_Elite"),
        FText::FromString(TEXT("精銳部隊")),
        FText::FromString(TEXT("高級步兵單位，訓練成本高但戰鬥力強，擁有特殊技能。")),
        {TEXT("單位"), TEXT("步兵")}));
    
    Sections.Add(Section);
}

void UMingRTSCodexSystem::InitializeBuildingSection()
{
    FCodexSection Section;
    Section.SectionType = ECodexEntryType::BuildingInfo;
    Section.SectionName = FText::FromString(TEXT("建築說明"));
    Section.SectionDescription = FText::FromString(TEXT("了解各種建築的功能和建造順序"));
    Section.SortOrder = 4;
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::BuildingInfo, TEXT("Building_House"),
        FText::FromString(TEXT("房屋")),
        FText::FromString(TEXT("提供人口上限，每個房屋增加10人口。是擴充軍隊的基礎。")),
        {TEXT("建築"), TEXT("基礎")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::BuildingInfo, TEXT("Building_Farm"),
        FText::FromString(TEXT("農場")),
        FText::FromString(TEXT("生產糧食資源，維持部隊需要。可以升級提高產量。")),
        {TEXT("建築"), TEXT("經濟")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::BuildingInfo, TEXT("Building_Barracks"),
        FText::FromString(TEXT("兵營")),
        FText::FromString(TEXT("訓練步兵單位，可以升級解鎖更高級的兵種。")),
        {TEXT("建築"), TEXT("軍事")}));
    
    Sections.Add(Section);
}

void UMingRTSCodexSystem::InitializeTechnologySection()
{
    FCodexSection Section;
    Section.SectionType = ECodexEntryType::Technology;
    Section.SectionName = FText::FromString(TEXT("科技樹"));
    Section.SectionDescription = FText::FromString(TEXT("了解各項科技的效果和前置條件"));
    Section.SortOrder = 5;
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::Technology, TEXT("Tech_InfantryWeapons"),
        FText::FromString(TEXT("步兵武器升級")),
        FText::FromString(TEXT("提升所有步兵單位的攻擊力。分為三個等級。")),
        {TEXT("科技"), TEXT("軍事")}));
    
    Sections.Add(Section);
}

void UMingRTSCodexSystem::InitializeHistoricalSection()
{
    FCodexSection Section;
    Section.SectionType = ECodexEntryType::Historical;
    Section.SectionName = FText::FromString(TEXT("歷史背景"));
    Section.SectionDescription = FText::FromString(TEXT("了解民國時期的歷史背景"));
    Section.SortOrder = 6;
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::Historical, TEXT("History_1912"),
        FText::FromString(TEXT("中華民國成立")),
        FText::FromString(TEXT("1912年1月1日，孫中山在南京就任臨時大總統，中華民國正式成立。")),
        {TEXT("歷史"), TEXT("1912")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::Historical, TEXT("History_Warlords"),
        FText::FromString(TEXT("軍閥割據時代")),
        FText::FromString(TEXT("1916年袁世凱死後，中國陷入軍閥混戰。各地軍閥割據一方，互相征戰。")),
        {TEXT("歷史"), TEXT("軍閥")}));
    
    Sections.Add(Section);
}

void UMingRTSCodexSystem::InitializeStrategySection()
{
    FCodexSection Section;
    Section.SectionType = ECodexEntryType::Strategy;
    Section.SectionName = FText::FromString(TEXT("戰術策略"));
    Section.SectionDescription = FText::FromString(TEXT("學習進階的戰術和策略"));
    Section.SortOrder = 7;
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::Strategy, TEXT("Strategy_Rush"),
        FText::FromString(TEXT("快攻戰術")),
        FText::FromString(TEXT("早期快速生產單位，在對手發展完成前發動攻擊。適合激進型玩家。")),
        {TEXT("策略"), TEXT("進攻")}));
    
    Section.Entries.Add(CreateEntry(ECodexEntryType::Strategy, TEXT("Strategy_Turtle"),
        FText::FromString(TEXT("防守發展")),
        FText::FromString(TEXT("前期專注防守和經濟建設，後期用強大的經濟優勢壓倒對手。")),
        {TEXT("策略"), TEXT("防守")}));
    
    Sections.Add(Section);
}

void UMingRTSCodexSystem::InitializeLoreSection()
{
    FCodexSection Section;
    Section.SectionType = ECodexEntryType::Lore;
    Section.SectionName = FText::FromString(TEXT("世界觀"));
    Section.SectionDescription = FText::FromString(TEXT("深入了解遊戲世界的背景故事"));
    Section.SortOrder = 8;
    
    // 初始鎖定，完成特定章節後解鎖
    FCodexEntry Entry = CreateEntry(ECodexEntryType::Lore, TEXT("Lore_World"),
        FText::FromString(TEXT("世界觀概覽")),
        FText::FromString(TEXT("民國史詩的世界觀建立在真實歷史基礎上...")),
        {TEXT("背景"), TEXT("故事")});
    Entry.bIsLocked = true;
    Entry.UnlockCondition = TEXT("CompleteCampaign_Chapter1");
    Section.Entries.Add(Entry);
    
    Sections.Add(Section);
}

FCodexEntry UMingRTSCodexSystem::CreateEntry(ECodexEntryType Type, const FString& ID, 
    const FText& Title, const FText& Description, const TArray<FString>& Tags)
{
    FCodexEntry Entry;
    Entry.EntryID = FName(*ID);
    Entry.EntryType = Type;
    Entry.Title = Title;
    Entry.Subtitle = FText::GetEmpty();
    Entry.Description = Description;
    Entry.Tags = Tags;
    Entry.bIsLocked = false;
    Entry.ViewCount = 0;
    
    return Entry;
}
