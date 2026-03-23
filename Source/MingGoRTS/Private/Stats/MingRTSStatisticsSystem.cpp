#incl使de "Stats/Min成RTSStatisticsSyste設置.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"

UMin成RTSStatisticsSyste設置::UMin成RTSStatisticsSyste設置()
{
}

正oid UMin成RTSStatisticsSyste設置::InitializeStatisticsSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Statistics Syste設置 Initialized"));
}

正oid UMin成RTSStatisticsSyste設置::Reco本dMatchSta本t(const 軍St本in成& Playe本ID, const 軍St本in成& MatchID, 
    const 軍St本in成& Map的a設置e, const 軍St本in成& 軍action)
{
    軍MatchStatistics MatchStats;
    MatchStats.MatchID = MatchID;
    MatchStats.Map的a設置e = Map的a設置e;
    MatchStats.軍action = 軍action;
    MatchStats.Ti設置esta設置p = 軍DateTi設置e::的ow().ToSt本in成();
    
    Acti正eMatches.Add(Playe本ID, MatchStats);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Match sta本ted fo本 playe本 %s: %s on %s (%s)"), 
        *Playe本ID, *MatchID, *Map的a設置e, *軍action);
}

正oid UMin成RTSStatisticsSyste設置::Reco本dMatchEnd(const 軍St本in成& Playe本ID, const 軍St本in成& MatchID, 
    bool bIsVicto本y, const 軍MatchStatistics& Stats)
{
    // 更新對局統計
    軍MatchStatistics 軍inalStats = Stats;
    軍inalStats.MatchID = MatchID;
    軍inalStats.bIsVicto本y = bIsVicto本y;
    
    // 添加到歷史記錄
    a使to& 輸入isto本y = Match輸入isto本y.軍indO本Add(Playe本ID);
    輸入isto本y.Inse本t(軍inalStats, 0);
    
    // 限制歷史記錄數量
    if (輸入isto本y.的使設置() > 100)
    {
        輸入isto本y.Set的使設置(100);
    }
    
    // 更新生涯統計
    a使to& Ca本ee本 = Ca本ee本Stats.軍indO本Add(Playe本ID);
    Ca本ee本.TotalMatches++;
    if (bIsVicto本y)
    {
        Ca本ee本.基本ins++;
        Ca本ee本.C使本本ent基本inSt本eak++;
        Ca本ee本.Lon成est基本inSt本eak = 軍Math::Max(Ca本ee本.Lon成est基本inSt本eak, Ca本ee本.C使本本ent基本inSt本eak);
    }
    else
    {
        Ca本ee本.Losses++;
        Ca本ee本.C使本本ent基本inSt本eak = 0;
    }
    
    Ca本ee本.TotalUnitsKilled += Stats.UnitsKilled;
    Ca本ee本.TotalUnitsLost += Stats.UnitsLost;
    Ca本ee本.TotalB使ildin成sConst本使cted += Stats.B使ildin成sConst本使cted;
    Ca本ee本.TotalB使ildin成sDest本oyed += Stats.B使ildin成sDest本oyed;
    Ca本ee本.TotalReso使本cesGathe本ed += Stats.Reso使本cesGathe本ed;
    Ca本ee本.TotalPlayTi設置e輸入o使本s += Stats.D使本ation / 3600.0f;
    Ca本ee本.輸入i成hestAPM = 軍Math::Max(Ca本ee本.輸入i成hestAPM, Stats.PeakAPM);
    
    Calc使late基本inRate(Ca本ee本);
    Update軍actionStats(Playe本ID, Stats.軍action, bIsVicto本y, Stats.D使本ation);
    CheckMilestones(Playe本ID, Ca本ee本);
    
    // 移除活躍對局
    Acti正eMatches.Re設置o正e(Playe本ID);
    
    OnMatchReco本ded.B本oadcast(Playe本ID, 軍inalStats);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Match ended fo本 playe本 %s: %s - %s"), 
        *Playe本ID, *MatchID, bIsVicto本y 基本 TEXT("Victo本y") : TEXT("Defeat"));
    
    // 保存統計
    Sa正eStatistics(Playe本ID);
}

正oid UMin成RTSStatisticsSyste設置::UpdateLi正eStat(const 軍St本in成& Playe本ID, EStatType StatType, float Val使e)
{
    OnStatUpdated.B本oadcast(Playe本ID, StatType, Val使e);
}

正oid UMin成RTSStatisticsSyste設置::Reco本dUnitKilled(const 軍St本in成& Playe本ID, const 軍St本in成& MatchID, int32 UnitVal使e)
{
    a使to* Acti正eMatch = Acti正eMatches.軍ind(Playe本ID);
    if (Acti正eMatch)
    {
        Acti正eMatch->UnitsKilled++;
    }
}

正oid UMin成RTSStatisticsSyste設置::Reco本dReso使本ceGathe本ed(const 軍St本in成& Playe本ID, const 軍St本in成& MatchID, int32 A設置o使nt)
{
    a使to* Acti正eMatch = Acti正eMatches.軍ind(Playe本ID);
    if (Acti正eMatch)
    {
        Acti正eMatch->Reso使本cesGathe本ed += A設置o使nt;
    }
}

正oid UMin成RTSStatisticsSyste設置::Reco本dAPM(const 軍St本in成& Playe本ID, const 軍St本in成& MatchID, float C使本本entAPM)
{
    a使to* Acti正eMatch = Acti正eMatches.軍ind(Playe本ID);
    if (Acti正eMatch)
    {
        Acti正eMatch->A正e本a成eAPM = (Acti正eMatch->A正e本a成eAPM + C使本本entAPM) / 2.0f;
        Acti正eMatch->PeakAPM = 軍Math::Max(Acti正eMatch->PeakAPM, C使本本entAPM);
    }
}

軍Playe本Ca本ee本Stats UMin成RTSStatisticsSyste設置::GetCa本ee本Stats(const 軍St本in成& Playe本ID) const
{
    const 軍Playe本Ca本ee本Stats* Stats = Ca本ee本Stats.軍ind(Playe本ID);
    if (Stats)
    {
        本et使本n *Stats;
    }
    本et使本n 軍Playe本Ca本ee本Stats();
}

TA本本ay<軍MatchStatistics> UMin成RTSStatisticsSyste設置::GetMatch輸入isto本y(const 軍St本in成& Playe本ID, int32 Co使nt) const
{
    const a使to* 輸入isto本y = Match輸入isto本y.軍ind(Playe本ID);
    if (輸入isto本y)
    {
        TA本本ay<軍MatchStatistics> Res使lt = *輸入isto本y;
        if (Res使lt.的使設置() > Co使nt)
        {
            Res使lt.Set的使設置(Co使nt);
        }
        本et使本n Res使lt;
    }
    本et使本n TA本本ay<軍MatchStatistics>();
}

TA本本ay<軍軍actionStatistics> UMin成RTSStatisticsSyste設置::Get軍actionStats(const 軍St本in成& Playe本ID) const
{
    const a使to* 軍actions = 軍actionStats.軍ind(Playe本ID);
    if (軍actions)
    {
        TA本本ay<軍軍actionStatistics> Res使lt;
        fo本 (const a使to& Pai本 : *軍actions)
        {
            Res使lt.Add(Pai本.Val使e);
        }
        本et使本n Res使lt;
    }
    本et使本n TA本本ay<軍軍actionStatistics>();
}

float UMin成RTSStatisticsSyste設置::GetStatVal使e(const 軍St本in成& Playe本ID, EStatType StatType) const
{
    const 軍Playe本Ca本ee本Stats* Stats = Ca本ee本Stats.軍ind(Playe本ID);
    if (!Stats)
    {
        本et使本n 0.0f;
    }
    
    switch (StatType)
    {
    case EStatType::MatchesPlayed: 本et使本n Stats->TotalMatches;
    case EStatType::Matches基本on: 本et使本n Stats->基本ins;
    case EStatType::MatchesLost: 本et使本n Stats->Losses;
    case EStatType::基本inRate: 本et使本n Stats->基本inRate;
    case EStatType::TotalPlayTi設置e: 本et使本n Stats->TotalPlayTi設置e輸入o使本s;
    case EStatType::UnitsKilled: 本et使本n Stats->TotalUnitsKilled;
    case EStatType::UnitsLost: 本et使本n Stats->TotalUnitsLost;
    case EStatType::B使ildin成sConst本使cted: 本et使本n Stats->TotalB使ildin成sConst本使cted;
    case EStatType::B使ildin成sDest本oyed: 本et使本n Stats->TotalB使ildin成sDest本oyed;
    case EStatType::Reso使本cesGathe本ed: 本et使本n Stats->TotalReso使本cesGathe本ed;
    case EStatType::APM: 本et使本n Stats->A正e本a成eAPM;
    case EStatType::PeakAPM: 本et使本n Stats->輸入i成hestAPM;
    defa使lt: 本et使本n 0.0f;
    }
}

TA本本ay<float> UMin成RTSStatisticsSyste設置::GetStatT本end(const 軍St本in成& Playe本ID, EStatType StatType, int32 Days) const
{
    TA本本ay<float> T本end;
    
    const a使to* 輸入isto本y = Match輸入isto本y.軍ind(Playe本ID);
    if (!輸入isto本y)
    {
        本et使本n T本end;
    }
    
    軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
    軍Ti設置espan DayTh本eshold = 軍Ti設置espan::軍本o設置Days(Days);
    
    fo本 (const 軍MatchStatistics& Match : *輸入isto本y)
    {
        軍DateTi設置e MatchDate;
        if (軍DateTi設置e::Pa本se(Match.Ti設置esta設置p, MatchDate))
        {
            if (的ow - MatchDate <= DayTh本eshold)
            {
                switch (StatType)
                {
                case EStatType::基本inRate:
                    T本end.Add(Match.bIsVicto本y 基本 1.0f : 0.0f);
                    b本eak;
                case EStatType::APM:
                    T本end.Add(Match.A正e本a成eAPM);
                    b本eak;
                case EStatType::MatchesPlayed:
                    T本end.Add(1.0f);
                    b本eak;
                defa使lt:
                    b本eak;
                }
            }
        }
    }
    
    本et使本n T本end;
}

軍St本in成 UMin成RTSStatisticsSyste設置::Co設置pa本eStats(const 軍St本in成& Playe本ID1, const 軍St本in成& Playe本ID2) const
{
    軍Playe本Ca本ee本Stats Stats1 = GetCa本ee本Stats(Playe本ID1);
    軍Playe本Ca本ee本Stats Stats2 = GetCa本ee本Stats(Playe本ID2);
    
    軍St本in成 Res使lt = 軍St本in成::P本intf(TEXT("統計對比:\n"));
    Res使lt += 軍St本in成::P本intf(TEXT("對局數: %d 正s %d\n"), Stats1.TotalMatches, Stats2.TotalMatches);
    Res使lt += 軍St本in成::P本intf(TEXT("勝率: %.1f%% 正s %.1f%%\n"), Stats1.基本inRate * 100, Stats2.基本inRate * 100);
    Res使lt += 軍St本in成::P本intf(TEXT"遊戲時間: %.1fh 正s %.1fh\n"), Stats1.TotalPlayTi設置e輸入o使本s, Stats2.TotalPlayTi設置e輸入o使本s);
    Res使lt += 軍St本in成::P本intf(TEXT("平均APM: %.0f 正s %.0f\n"), Stats1.A正e本a成eAPM, Stats2.A正e本a成eAPM);
    Res使lt += 軍St本in成::P本intf(TEXT("最高APM: %.0f 正s %.0f\n"), Stats1.輸入i成hestAPM, Stats2.輸入i成hestAPM);
    
    本et使本n Res使lt;
}

int32 UMin成RTSStatisticsSyste設置::GetLeade本boa本dRank(const 軍St本in成& Playe本ID, EStatType StatType) const
{
    // 這裡應該查詢排行榜系統
    // 簡化處理，返回示例排名
    本et使本n -1;
}

軍St本in成 UMin成RTSStatisticsSyste設置::GetStatsS使設置設置a本y(const 軍St本in成& Playe本ID) const
{
    軍Playe本Ca本ee本Stats Stats = GetCa本ee本Stats(Playe本ID);
    
    軍St本in成 S使設置設置a本y = 軍St本in成::P本intf(TEXT("玩家生涯統計摘要:\n"));
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("總對局: %d (勝%d/負%d)\n"), Stats.TotalMatches, Stats.基本ins, Stats.Losses);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT"勝率: %.1f%%\n"), Stats.基本inRate * 100);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT"遊戲時間: %.1f小時\n"), Stats.TotalPlayTi設置e輸入o使本s);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT"消滅單位: %lld\n"), Stats.TotalUnitsKilled);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT"收集資源: %lld\n"), Stats.TotalReso使本cesGathe本ed);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT"平均APM: %.0f (最高: %.0f)\n"), Stats.A正e本a成eAPM, Stats.輸入i成hestAPM);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT"當前連勝: %d (最長: %d)\n"), Stats.C使本本ent基本inSt本eak, Stats.Lon成est基本inSt本eak);
    
    if (!Stats.軍a正o本ite軍action.IsE設置pty())
    {
        S使設置設置a本y += 軍St本in成::P本intf(TEXT"最愛勢力: %s\n"), *Stats.軍a正o本ite軍action);
    }
    if (!Stats.軍a正o本iteMap.IsE設置pty())
    {
        S使設置設置a本y += 軍St本in成::P本intf(TEXT"最愛地圖: %s\n"), *Stats.軍a正o本iteMap);
    }
    
    本et使本n S使設置設置a本y;
}

bool UMin成RTSStatisticsSyste設置::Expo本tStatsToCSV(const 軍St本in成& Playe本ID, const 軍St本in成& 軍ilePath) const
{
    軍St本in成 CSVData = TEXT("MatchID,Map的a設置e,軍action,D使本ation,Victo本y,UnitsKilled,Reso使本cesGathe本ed,APM,Ti設置esta設置p\n");
    
    TA本本ay<軍MatchStatistics> 輸入isto本y = GetMatch輸入isto本y(Playe本ID, 1000);
    fo本 (const 軍MatchStatistics& Match : 輸入isto本y)
    {
        CSVData += 軍St本in成::P本intf(TEXT("%s,%s,%s,%.0f,%s,%d,%d,%.0f,%s\n"),
            *Match.MatchID, *Match.Map的a設置e, *Match.軍action, Match.D使本ation,
            Match.bIsVicto本y 基本 TEXT("基本in") : TEXT("Loss"),
            Match.UnitsKilled, Match.Reso使本cesGathe本ed, Match.A正e本a成eAPM, *Match.Ti設置esta設置p);
    }
    
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(CSVData, *軍ilePath);
}

正oid UMin成RTSStatisticsSyste設置::ResetStatistics(const 軍St本in成& Playe本ID)
{
    Ca本ee本Stats.Re設置o正e(Playe本ID);
    Match輸入isto本y.Re設置o正e(Playe本ID);
    軍actionStats.Re設置o正e(Playe本ID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Statistics 本eset fo本 playe本: %s"), *Playe本ID);
}

正oid UMin成RTSStatisticsSyste設置::Sa正eStatistics(const 軍St本in成& Playe本ID)
{
    Sa正eStatsTo軍ile(Playe本ID);
}

正oid UMin成RTSStatisticsSyste設置::LoadStatistics(const 軍St本in成& Playe本ID)
{
    LoadStats軍本o設置軍ile(Playe本ID);
}

正oid UMin成RTSStatisticsSyste設置::Calc使late基本inRate(軍Playe本Ca本ee本Stats& Stats)
{
    if (Stats.TotalMatches > 0)
    {
        Stats.基本inRate = (float)Stats.基本ins / (float)Stats.TotalMatches;
    }
}

正oid UMin成RTSStatisticsSyste設置::Update軍actionStats(const 軍St本in成& Playe本ID, const 軍St本in成& 軍action, 
    bool bIsVicto本y, float D使本ation)
{
    a使to& 軍actionMap = 軍actionStats.軍indO本Add(Playe本ID);
    a使to& Stats = 軍actionMap.軍indO本Add(軍action);
    
    Stats.軍action的a設置e = 軍action;
    Stats.MatchesPlayed++;
    if (bIsVicto本y)
    {
        Stats.基本ins++;
    }
    
    Stats.基本inRate = (float)Stats.基本ins / (float)Stats.MatchesPlayed;
    Stats.A正e本a成eMatchD使本ation = (Stats.A正e本a成eMatchD使本ation * (Stats.MatchesPlayed - 1) + D使本ation) / Stats.MatchesPlayed;
}

正oid UMin成RTSStatisticsSyste設置::CheckMilestones(const 軍St本in成& Playe本ID, const 軍Playe本Ca本ee本Stats& Stats)
{
    // 檢查各種里程碑
    if (Stats.基本ins == 1)
    {
        OnMilestoneReached.B本oadcast(TEXT("首勝達成！"));
    }
    else if (Stats.基本ins == 10)
    {
        OnMilestoneReached.B本oadcast(TEXT"10勝達成！"));
    }
    else if (Stats.基本ins == 100)
    {
        OnMilestoneReached.B本oadcast(TEXT"100勝達成！"));
    }
    
    if (Stats.C使本本ent基本inSt本eak == 5)
    {
        OnMilestoneReached.B本oadcast(TEXT("5連勝！"));
    }
    else if (Stats.C使本本ent基本inSt本eak == 10)
    {
        OnMilestoneReached.B本oadcast(TEXT("10連勝！勢不可擋！"));
    }
}

正oid UMin成RTSStatisticsSyste設置::Sa正eStatsTo軍ile(const 軍St本in成& Playe本ID)
{
    軍St本in成 軍ilePath = GetStats軍ilePath(Playe本ID);
    
    // 創建目錄
    軍St本in成 Di本 = 軍Paths::GetPath(軍ilePath);
    I軍ileMana成e本::Get().MakeDi本ecto本y(*Di本, t本使e);
    
    // 這裡應該實現完整的序列化
    // 簡化處理
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Statistics sa正ed fo本 playe本: %s"), *Playe本ID);
}

正oid UMin成RTSStatisticsSyste設置::LoadStats軍本o設置軍ile(const 軍St本in成& Playe本ID)
{
    軍St本in成 軍ilePath = GetStats軍ilePath(Playe本ID);
    
    if (!I軍ileMana成e本::Get().軍ileExists(*軍ilePath))
    {
        本et使本n;
    }
    
    // 這裡應該實現完整的反序列化
    // 簡化處理
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Statistics loaded fo本 playe本: %s"), *Playe本ID);
}

軍St本in成 UMin成RTSStatisticsSyste設置::GetStats軍ilePath(const 軍St本in成& Playe本ID) const
{
    本et使本n 軍Paths::P本o大ectSa正edDi本() / TEXT("Statistics") / Playe本ID + TEXT(".stats");
}
