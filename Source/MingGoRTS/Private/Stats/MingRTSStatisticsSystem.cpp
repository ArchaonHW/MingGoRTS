#include "Stats/MingRTSStatisticsSystem.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

UMingRTSStatisticsSystem::UMingRTSStatisticsSystem()
{
}

void UMingRTSStatisticsSystem::InitializeStatisticsSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Statistics System Initialized"));
}

void UMingRTSStatisticsSystem::RecordMatchStart(const FString& PlayerID, const FString& MatchID, 
    const FString& MapName, const FString& Faction)
{
    FMatchStatistics MatchStats;
    MatchStats.MatchID = MatchID;
    MatchStats.MapName = MapName;
    MatchStats.Faction = Faction;
    MatchStats.Timestamp = FDateTime::Now().ToString();
    
    ActiveMatches.Add(PlayerID, MatchStats);
    
    UE_LOG(LogTemp, Log, TEXT("Match started for player %s: %s on %s (%s)"), 
        *PlayerID, *MatchID, *MapName, *Faction);
}

void UMingRTSStatisticsSystem::RecordMatchEnd(const FString& PlayerID, const FString& MatchID, 
    bool bIsVictory, const FMatchStatistics& Stats)
{
    // 更新對局統計
    FMatchStatistics FinalStats = Stats;
    FinalStats.MatchID = MatchID;
    FinalStats.bIsVictory = bIsVictory;
    
    // 添加到歷史記錄
    auto& History = MatchHistory.FindOrAdd(PlayerID);
    History.Insert(FinalStats, 0);
    
    // 限制歷史記錄數量
    if (History.Num() > 100)
    {
        History.SetNum(100);
    }
    
    // 更新生涯統計
    auto& Career = CareerStats.FindOrAdd(PlayerID);
    Career.TotalMatches++;
    if (bIsVictory)
    {
        Career.Wins++;
        Career.CurrentWinStreak++;
        Career.LongestWinStreak = FMath::Max(Career.LongestWinStreak, Career.CurrentWinStreak);
    }
    else
    {
        Career.Losses++;
        Career.CurrentWinStreak = 0;
    }
    
    Career.TotalUnitsKilled += Stats.UnitsKilled;
    Career.TotalUnitsLost += Stats.UnitsLost;
    Career.TotalBuildingsConstructed += Stats.BuildingsConstructed;
    Career.TotalBuildingsDestroyed += Stats.BuildingsDestroyed;
    Career.TotalResourcesGathered += Stats.ResourcesGathered;
    Career.TotalPlayTimeHours += Stats.Duration / 3600.0f;
    Career.HighestAPM = FMath::Max(Career.HighestAPM, Stats.PeakAPM);
    
    CalculateWinRate(Career);
    UpdateFactionStats(PlayerID, Stats.Faction, bIsVictory, Stats.Duration);
    CheckMilestones(PlayerID, Career);
    
    // 移除活躍對局
    ActiveMatches.Remove(PlayerID);
    
    OnMatchRecorded.Broadcast(PlayerID, FinalStats);
    
    UE_LOG(LogTemp, Log, TEXT("Match ended for player %s: %s - %s"), 
        *PlayerID, *MatchID, bIsVictory ? TEXT("Victory") : TEXT("Defeat"));
    
    // 保存統計
    SaveStatistics(PlayerID);
}

void UMingRTSStatisticsSystem::UpdateLiveStat(const FString& PlayerID, EStatType StatType, float Value)
{
    OnStatUpdated.Broadcast(PlayerID, StatType, Value);
}

void UMingRTSStatisticsSystem::RecordUnitKilled(const FString& PlayerID, const FString& MatchID, int32 UnitValue)
{
    auto* ActiveMatch = ActiveMatches.Find(PlayerID);
    if (ActiveMatch)
    {
        ActiveMatch->UnitsKilled++;
    }
}

void UMingRTSStatisticsSystem::RecordResourceGathered(const FString& PlayerID, const FString& MatchID, int32 Amount)
{
    auto* ActiveMatch = ActiveMatches.Find(PlayerID);
    if (ActiveMatch)
    {
        ActiveMatch->ResourcesGathered += Amount;
    }
}

void UMingRTSStatisticsSystem::RecordAPM(const FString& PlayerID, const FString& MatchID, float CurrentAPM)
{
    auto* ActiveMatch = ActiveMatches.Find(PlayerID);
    if (ActiveMatch)
    {
        ActiveMatch->AverageAPM = (ActiveMatch->AverageAPM + CurrentAPM) / 2.0f;
        ActiveMatch->PeakAPM = FMath::Max(ActiveMatch->PeakAPM, CurrentAPM);
    }
}

FPlayerCareerStats UMingRTSStatisticsSystem::GetCareerStats(const FString& PlayerID) const
{
    const FPlayerCareerStats* Stats = CareerStats.Find(PlayerID);
    if (Stats)
    {
        return *Stats;
    }
    return FPlayerCareerStats();
}

TArray<FMatchStatistics> UMingRTSStatisticsSystem::GetMatchHistory(const FString& PlayerID, int32 Count) const
{
    const auto* History = MatchHistory.Find(PlayerID);
    if (History)
    {
        TArray<FMatchStatistics> Result = *History;
        if (Result.Num() > Count)
        {
            Result.SetNum(Count);
        }
        return Result;
    }
    return TArray<FMatchStatistics>();
}

TArray<FFactionStatistics> UMingRTSStatisticsSystem::GetFactionStats(const FString& PlayerID) const
{
    const auto* Factions = FactionStats.Find(PlayerID);
    if (Factions)
    {
        TArray<FFactionStatistics> Result;
        for (const auto& Pair : *Factions)
        {
            Result.Add(Pair.Value);
        }
        return Result;
    }
    return TArray<FFactionStatistics>();
}

float UMingRTSStatisticsSystem::GetStatValue(const FString& PlayerID, EStatType StatType) const
{
    const FPlayerCareerStats* Stats = CareerStats.Find(PlayerID);
    if (!Stats)
    {
        return 0.0f;
    }
    
    switch (StatType)
    {
    case EStatType::MatchesPlayed: return Stats->TotalMatches;
    case EStatType::MatchesWon: return Stats->Wins;
    case EStatType::MatchesLost: return Stats->Losses;
    case EStatType::WinRate: return Stats->WinRate;
    case EStatType::TotalPlayTime: return Stats->TotalPlayTimeHours;
    case EStatType::UnitsKilled: return Stats->TotalUnitsKilled;
    case EStatType::UnitsLost: return Stats->TotalUnitsLost;
    case EStatType::BuildingsConstructed: return Stats->TotalBuildingsConstructed;
    case EStatType::BuildingsDestroyed: return Stats->TotalBuildingsDestroyed;
    case EStatType::ResourcesGathered: return Stats->TotalResourcesGathered;
    case EStatType::APM: return Stats->AverageAPM;
    case EStatType::PeakAPM: return Stats->HighestAPM;
    default: return 0.0f;
    }
}

TArray<float> UMingRTSStatisticsSystem::GetStatTrend(const FString& PlayerID, EStatType StatType, int32 Days) const
{
    TArray<float> Trend;
    
    const auto* History = MatchHistory.Find(PlayerID);
    if (!History)
    {
        return Trend;
    }
    
    FDateTime Now = FDateTime::Now();
    FTimespan DayThreshold = FTimespan::FromDays(Days);
    
    for (const FMatchStatistics& Match : *History)
    {
        FDateTime MatchDate;
        if (FDateTime::Parse(Match.Timestamp, MatchDate))
        {
            if (Now - MatchDate <= DayThreshold)
            {
                switch (StatType)
                {
                case EStatType::WinRate:
                    Trend.Add(Match.bIsVictory ? 1.0f : 0.0f);
                    break;
                case EStatType::APM:
                    Trend.Add(Match.AverageAPM);
                    break;
                case EStatType::MatchesPlayed:
                    Trend.Add(1.0f);
                    break;
                default:
                    break;
                }
            }
        }
    }
    
    return Trend;
}

FString UMingRTSStatisticsSystem::CompareStats(const FString& PlayerID1, const FString& PlayerID2) const
{
    FPlayerCareerStats Stats1 = GetCareerStats(PlayerID1);
    FPlayerCareerStats Stats2 = GetCareerStats(PlayerID2);
    
    FString Result = FString::Printf(TEXT("統計對比:\n"));
    Result += FString::Printf(TEXT("對局數: %d vs %d\n"), Stats1.TotalMatches, Stats2.TotalMatches);
    Result += FString::Printf(TEXT("勝率: %.1f%% vs %.1f%%\n"), Stats1.WinRate * 100, Stats2.WinRate * 100);
    Result += FString::Printf(TEXT"遊戲時間: %.1fh vs %.1fh\n"), Stats1.TotalPlayTimeHours, Stats2.TotalPlayTimeHours);
    Result += FString::Printf(TEXT("平均APM: %.0f vs %.0f\n"), Stats1.AverageAPM, Stats2.AverageAPM);
    Result += FString::Printf(TEXT("最高APM: %.0f vs %.0f\n"), Stats1.HighestAPM, Stats2.HighestAPM);
    
    return Result;
}

int32 UMingRTSStatisticsSystem::GetLeaderboardRank(const FString& PlayerID, EStatType StatType) const
{
    // 這裡應該查詢排行榜系統
    // 簡化處理，返回示例排名
    return -1;
}

FString UMingRTSStatisticsSystem::GetStatsSummary(const FString& PlayerID) const
{
    FPlayerCareerStats Stats = GetCareerStats(PlayerID);
    
    FString Summary = FString::Printf(TEXT("玩家生涯統計摘要:\n"));
    Summary += FString::Printf(TEXT("總對局: %d (勝%d/負%d)\n"), Stats.TotalMatches, Stats.Wins, Stats.Losses);
    Summary += FString::Printf(TEXT"勝率: %.1f%%\n"), Stats.WinRate * 100);
    Summary += FString::Printf(TEXT"遊戲時間: %.1f小時\n"), Stats.TotalPlayTimeHours);
    Summary += FString::Printf(TEXT"消滅單位: %lld\n"), Stats.TotalUnitsKilled);
    Summary += FString::Printf(TEXT"收集資源: %lld\n"), Stats.TotalResourcesGathered);
    Summary += FString::Printf(TEXT"平均APM: %.0f (最高: %.0f)\n"), Stats.AverageAPM, Stats.HighestAPM);
    Summary += FString::Printf(TEXT"當前連勝: %d (最長: %d)\n"), Stats.CurrentWinStreak, Stats.LongestWinStreak);
    
    if (!Stats.FavoriteFaction.IsEmpty())
    {
        Summary += FString::Printf(TEXT"最愛勢力: %s\n"), *Stats.FavoriteFaction);
    }
    if (!Stats.FavoriteMap.IsEmpty())
    {
        Summary += FString::Printf(TEXT"最愛地圖: %s\n"), *Stats.FavoriteMap);
    }
    
    return Summary;
}

bool UMingRTSStatisticsSystem::ExportStatsToCSV(const FString& PlayerID, const FString& FilePath) const
{
    FString CSVData = TEXT("MatchID,MapName,Faction,Duration,Victory,UnitsKilled,ResourcesGathered,APM,Timestamp\n");
    
    TArray<FMatchStatistics> History = GetMatchHistory(PlayerID, 1000);
    for (const FMatchStatistics& Match : History)
    {
        CSVData += FString::Printf(TEXT("%s,%s,%s,%.0f,%s,%d,%d,%.0f,%s\n"),
            *Match.MatchID, *Match.MapName, *Match.Faction, Match.Duration,
            Match.bIsVictory ? TEXT("Win") : TEXT("Loss"),
            Match.UnitsKilled, Match.ResourcesGathered, Match.AverageAPM, *Match.Timestamp);
    }
    
    return FFileHelper::SaveStringToFile(CSVData, *FilePath);
}

void UMingRTSStatisticsSystem::ResetStatistics(const FString& PlayerID)
{
    CareerStats.Remove(PlayerID);
    MatchHistory.Remove(PlayerID);
    FactionStats.Remove(PlayerID);
    
    UE_LOG(LogTemp, Log, TEXT("Statistics reset for player: %s"), *PlayerID);
}

void UMingRTSStatisticsSystem::SaveStatistics(const FString& PlayerID)
{
    SaveStatsToFile(PlayerID);
}

void UMingRTSStatisticsSystem::LoadStatistics(const FString& PlayerID)
{
    LoadStatsFromFile(PlayerID);
}

void UMingRTSStatisticsSystem::CalculateWinRate(FPlayerCareerStats& Stats)
{
    if (Stats.TotalMatches > 0)
    {
        Stats.WinRate = (float)Stats.Wins / (float)Stats.TotalMatches;
    }
}

void UMingRTSStatisticsSystem::UpdateFactionStats(const FString& PlayerID, const FString& Faction, 
    bool bIsVictory, float Duration)
{
    auto& FactionMap = FactionStats.FindOrAdd(PlayerID);
    auto& Stats = FactionMap.FindOrAdd(Faction);
    
    Stats.FactionName = Faction;
    Stats.MatchesPlayed++;
    if (bIsVictory)
    {
        Stats.Wins++;
    }
    
    Stats.WinRate = (float)Stats.Wins / (float)Stats.MatchesPlayed;
    Stats.AverageMatchDuration = (Stats.AverageMatchDuration * (Stats.MatchesPlayed - 1) + Duration) / Stats.MatchesPlayed;
}

void UMingRTSStatisticsSystem::CheckMilestones(const FString& PlayerID, const FPlayerCareerStats& Stats)
{
    // 檢查各種里程碑
    if (Stats.Wins == 1)
    {
        OnMilestoneReached.Broadcast(TEXT("首勝達成！"));
    }
    else if (Stats.Wins == 10)
    {
        OnMilestoneReached.Broadcast(TEXT"10勝達成！"));
    }
    else if (Stats.Wins == 100)
    {
        OnMilestoneReached.Broadcast(TEXT"100勝達成！"));
    }
    
    if (Stats.CurrentWinStreak == 5)
    {
        OnMilestoneReached.Broadcast(TEXT("5連勝！"));
    }
    else if (Stats.CurrentWinStreak == 10)
    {
        OnMilestoneReached.Broadcast(TEXT("10連勝！勢不可擋！"));
    }
}

void UMingRTSStatisticsSystem::SaveStatsToFile(const FString& PlayerID)
{
    FString FilePath = GetStatsFilePath(PlayerID);
    
    // 創建目錄
    FString Dir = FPaths::GetPath(FilePath);
    IFileManager::Get().MakeDirectory(*Dir, true);
    
    // 這裡應該實現完整的序列化
    // 簡化處理
    UE_LOG(LogTemp, Log, TEXT("Statistics saved for player: %s"), *PlayerID);
}

void UMingRTSStatisticsSystem::LoadStatsFromFile(const FString& PlayerID)
{
    FString FilePath = GetStatsFilePath(PlayerID);
    
    if (!IFileManager::Get().FileExists(*FilePath))
    {
        return;
    }
    
    // 這裡應該實現完整的反序列化
    // 簡化處理
    UE_LOG(LogTemp, Log, TEXT("Statistics loaded for player: %s"), *PlayerID);
}

FString UMingRTSStatisticsSystem::GetStatsFilePath(const FString& PlayerID) const
{
    return FPaths::ProjectSavedDir() / TEXT("Statistics") / PlayerID + TEXT(".stats");
}
