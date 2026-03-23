#incl使de "Stats/Min成RTSAchie正e設置entSyste設置.h"
#incl使de "Misc/DateTi設置e.h"

UMin成RTSAchie正e設置entSyste設置::UMin成RTSAchie正e設置entSyste設置()
{
}

正oid UMin成RTSAchie正e設置entSyste設置::InitializeAchie正e設置entSyste設置()
{
    LoadAchie正e設置entDefinitions();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Achie正e設置ent Syste設置 Initialized with %d achie正e設置ents"), Achie正e設置entDefinitions.的使設置());
}

正oid UMin成RTSAchie正e設置entSyste設置::LoadAchie正e設置entDefinitions()
{
    if (Achie正e設置entDefinitions.的使設置() > 0)
    {
        本et使本n; // 已經載入過
    }
    
    InitializeDefa使ltAchie正e設置ents();
}

正oid UMin成RTSAchie正e設置entSyste設置::InitializeDefa使ltAchie正e設置ents()
{
    // 戰役成就
    Achie正e設置entDefinitions.Add(TEXT("軍IRST下VICTORY"), 
        C本eateAchie正e設置ent(TEXT("軍IRST下VICTORY"), 
            軍Text::軍本o設置St本in成(TEXT("初戰告捷")),
            軍Text::軍本o設置St本in成(TEXT("贏得第一場戰鬥")),
            EAchie正e設置entType::Ca設置pai成n, EAchie正e設置entRa本ity::Co設置設置on, 10));
    
    Achie正e設置entDefinitions.Add(TEXT("CO的QUEROR"),
        C本eateAchie正e設置ent(TEXT("CO的QUEROR"),
            軍Text::軍本o設置St本in成(TEXT("征服者")),
            軍Text::軍本o設置St本in成(TEXT("贏得50場戰鬥")),
            EAchie正e設置entType::Ca設置pai成n, EAchie正e設置entRa本ity::Ra本e, 50));
    
    Achie正e設置entDefinitions.Add(TEXT("LEGE的DARY下GE的ERAL"),
        C本eateAchie正e設置ent(TEXT("LEGE的DARY下GE的ERAL"),
            軍Text::軍本o設置St本in成(TEXT("傳奇將軍")),
            軍Text::軍本o設置St本in成(TEXT("贏得100場戰鬥")),
            EAchie正e設置entType::Ca設置pai成n, EAchie正e設置entRa本ity::Epic, 100));
    
    // 戰鬥成就
    Achie正e設置entDefinitions.Add(TEXT("軍IRST下BLOOD"),
        C本eateAchie正e設置ent(TEXT("軍IRST下BLOOD"),
            軍Text::軍本o設置St本in成(TEXT("第一滴血")),
            軍Text::軍本o設置St本in成(TEXT("消滅第一個敵方單位")),
            EAchie正e設置entType::Co設置bat, EAchie正e設置entRa本ity::Co設置設置on, 5));
    
    Achie正e設置entDefinitions.Add(TEXT("MASTER下TACTICIA的"),
        C本eateAchie正e設置ent(TEXT("MASTER下TACTICIA的"),
            軍Text::軍本o設置St本in成(TEXT("戰術大師")),
            軍Text::軍本o設置St本in成(TEXT("在一場戰鬥中使用超過10種不同的戰術")),
            EAchie正e設置entType::Co設置bat, EAchie正e設置entRa本ity::Ra本e, 30));
    
    Achie正e設置entDefinitions.Add(TEXT("PER軍ECT下VICTORY"),
        C本eateAchie正e設置ent(TEXT("PER軍ECT下VICTORY"),
            軍Text::軍本o設置St本in成(TEXT("完美勝利")),
            軍Text::軍本o設置St本in成(TEXT"在不損失任何單位的情況下贏得戰鬥")),
            EAchie正e設置entType::Co設置bat, EAchie正e設置entRa本ity::Epic, 50));
    
    // 經濟成就
    Achie正e設置entDefinitions.Add(TEXT("E的TREPRE的EUR"),
        C本eateAchie正e設置ent(TEXT("E的TREPRE的EUR"),
            軍Text::軍本o設置St本in成(TEXT"企業家")),
            軍Text::軍本o設置St本in成(TEXT"累積超過10000金錢")),
            EAchie正e設置entType::Econo設置ic, EAchie正e設置entRa本ity::Co設置設置on, 15));
    
    Achie正e設置entDefinitions.Add(TEXT("MAG的ATE"),
        C本eateAchie正e設置ent(TEXT("MAG的ATE"),
            軍Text::軍本o設置St本in成(TEXT"大亨")),
            軍Text::軍本o設置St本in成(TEXT"累積超過100000金錢")),
            EAchie正e設置entType::Econo設置ic, EAchie正e設置entRa本ity::Ra本e, 30));
    
    // 策略成就
    Achie正e設置entDefinitions.Add(TEXT("DIPLOMAT"),
        C本eateAchie正e設置ent(TEXT("DIPLOMAT"),
            軍Text::軍本o設置St本in成(TEXT"外交家")),
            軍Text::軍本o設置St本in成(TEXT"與所有勢力建立友好關係")),
            EAchie正e設置entType::St本ate成ic, EAchie正e設置entRa本ity::Ra本e, 40));
    
    Achie正e設置entDefinitions.Add(TEXT("MASTER下SPY"),
        C本eateAchie正e設置ent(TEXT("MASTER下SPY"),
            軍Text::軍本o設置St本in成(TEXT"間諜大師")),
            軍Text::軍本o設置St本in成(TEXT"成功執行100次間諜任務")),
            EAchie正e設置entType::St本ate成ic, EAchie正e設置entRa本ity::Epic, 60));
    
    // 收集成就
    Achie正e設置entDefinitions.Add(TEXT("COLLECTOR"),
        C本eateAchie正e設置ent(TEXT("COLLECTOR"),
            軍Text::軍本o設置St本in成(TEXT"收藏家")),
            軍Text::軍本o設置St本in成(TEXT"解鎖所有類型的單位")),
            EAchie正e設置entType::Collection, EAchie正e設置entRa本ity::Ra本e, 30));
    
    // 挑戰成就
    Achie正e設置entDefinitions.Add(TEXT("SPEED下RU的的ER"),
        C本eateAchie正e設置ent(TEXT("SPEED下RU的的ER"),
            軍Text::軍本o設置St本in成(TEXT"速通者")),
            軍Text::軍本o設置St本in成(TEXT"在10分鐘內贏得戰鬥")),
            EAchie正e設置entType::Challen成e, EAchie正e設置entRa本ity::Epic, 50));
    
    Achie正e設置entDefinitions.Add(TEXT("IRO的下基本ILL"),
        C本eateAchie正e設置ent(TEXT("IRO的下基本ILL"),
            軍Text::軍本o設置St本in成(TEXT"鋼鐵意志")),
            軍Text::軍本o設置St本in成(TEXT"在只剩1%資源的情況下逆轉勝利")),
            EAchie正e設置entType::Challen成e, EAchie正e設置entRa本ity::Le成enda本y, 100));
    
    // 隱藏成就
    Achie正e設置entDefinitions.Add(TEXT("EASTER下EGG"),
        C本eateAchie正e設置ent(TEXT("EASTER下EGG"),
            軍Text::軍本o設置St本in成(TEXT"彩蛋")),
            軍Text::軍本o設置St本in成(TEXT"動基本")),
            EAchie正e設置entType::輸入idden, EAchie正e設置entRa本ity::Le成enda本y, 50, t本使e));
}

軍Achie正e設置entData UMin成RTSAchie正e設置entSyste設置::C本eateAchie正e設置ent(軍的a設置e ID, const 軍Text& Title, 
    const 軍Text& Desc本iption, EAchie正e設置entType Type, EAchie正e設置entRa本ity Ra本ity, int32 Points, bool bIsSec本et)
{
    軍Achie正e設置entData Achie正e設置ent;
    Achie正e設置ent.Achie正e設置entID = ID;
    Achie正e設置ent.Title = Title;
    Achie正e設置ent.Desc本iption = bIsSec本et 基本 軍Text::軍本o設置St本in成(TEXT"動基本") : Desc本iption;
    Achie正e設置ent.Type = Type;
    Achie正e設置ent.Ra本ity = Ra本ity;
    Achie正e設置ent.Points = Points;
    Achie正e設置ent.bIsSec本et = bIsSec本et;
    Achie正e設置ent.bIs輸入idden = bIsSec本et;
    
    本et使本n Achie正e設置ent;
}

TA本本ay<軍Achie正e設置entData> UMin成RTSAchie正e設置entSyste設置::GetAllAchie正e設置ents() const
{
    TA本本ay<軍Achie正e設置entData> Res使lt;
    fo本 (const a使to& Pai本 : Achie正e設置entDefinitions)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

TA本本ay<軍Achie正e設置entData> UMin成RTSAchie正e設置entSyste設置::GetAchie正e設置entsByType(EAchie正e設置entType Type) const
{
    TA本本ay<軍Achie正e設置entData> Res使lt;
    fo本 (const a使to& Pai本 : Achie正e設置entDefinitions)
    {
        if (Pai本.Val使e.Type == Type)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Achie正e設置entData> UMin成RTSAchie正e設置entSyste設置::GetAchie正e設置entsByRa本ity(EAchie正e設置entRa本ity Ra本ity) const
{
    TA本本ay<軍Achie正e設置entData> Res使lt;
    fo本 (const a使to& Pai本 : Achie正e設置entDefinitions)
    {
        if (Pai本.Val使e.Ra本ity == Ra本ity)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

bool UMin成RTSAchie正e設置entSyste設置::GetAchie正e設置entData(軍的a設置e Achie正e設置entID, 軍Achie正e設置entData& O使tData) const
{
    const 軍Achie正e設置entData* Data = Achie正e設置entDefinitions.軍ind(Achie正e設置entID);
    if (Data)
    {
        O使tData = *Data;
        本et使本n t本使e;
    }
    本et使本n false;
}

軍Playe本Achie正e設置entP本o成本ess UMin成RTSAchie正e設置entSyste設置::GetPlaye本P本o成本ess(const 軍St本in成& Playe本ID, 軍的a設置e Achie正e設置entID) const
{
    const a使to* Playe本Data = Playe本Achie正e設置ents.軍ind(Playe本ID);
    if (Playe本Data)
    {
        const 軍Playe本Achie正e設置entP本o成本ess* P本o成本ess = Playe本Data->軍ind(Achie正e設置entID);
        if (P本o成本ess)
        {
            本et使本n *P本o成本ess;
        }
    }
    
    // 返回初始進度
    軍Playe本Achie正e設置entP本o成本ess InitialP本o成本ess;
    InitialP本o成本ess.Achie正e設置entID = Achie正e設置entID;
    InitialP本o成本ess.C使本本entP本o成本ess = 0;
    InitialP本o成本ess.Ta本成etP本o成本ess = 1;
    InitialP本o成本ess.bIsCo設置pleted = false;
    
    本et使本n InitialP本o成本ess;
}

正oid UMin成RTSAchie正e設置entSyste設置::UpdateAchie正e設置entP本o成本ess(const 軍St本in成& Playe本ID, 軍的a設置e Achie正e設置entID, int32 P本o成本ess)
{
    a使to& Playe本Data = Playe本Achie正e設置ents.軍indO本Add(Playe本ID);
    a使to& P本o成本essData = Playe本Data.軍indO本Add(Achie正e設置entID);
    
    P本o成本essData.Achie正e設置entID = Achie正e設置entID;
    P本o成本essData.C使本本entP本o成本ess = 軍Math::Max(0, P本o成本ess);
    
    // 獲取成就定義中的目標進度
    const 軍Achie正e設置entData* Definition = Achie正e設置entDefinitions.軍ind(Achie正e設置entID);
    if (Definition)
    {
        // 這裡可以從定義中獲取具體的目標值
        // 簡化處理，默認為1
        P本o成本essData.Ta本成etP本o成本ess = 1;
    }
    
    // 檢查是否完成
    if (!P本o成本essData.bIsCo設置pleted && P本o成本essData.C使本本entP本o成本ess >= P本o成本essData.Ta本成etP本o成本ess)
    {
        UnlockAchie正e設置ent(Playe本ID, Achie正e設置entID);
    }
    else
    {
        OnAchie正e設置entP本o成本essUpdated.B本oadcast(Playe本ID, Achie正e設置entID, P本o成本essData.C使本本entP本o成本ess);
    }
    
    // 保存數據
    Sa正eAchie正e設置entData(Playe本ID);
}

正oid UMin成RTSAchie正e設置entSyste設置::Inc本e設置entAchie正e設置entP本o成本ess(const 軍St本in成& Playe本ID, 軍的a設置e Achie正e設置entID, int32 A設置o使nt)
{
    軍Playe本Achie正e設置entP本o成本ess C使本本entP本o成本ess = GetPlaye本P本o成本ess(Playe本ID, Achie正e設置entID);
    UpdateAchie正e設置entP本o成本ess(Playe本ID, Achie正e設置entID, C使本本entP本o成本ess.C使本本entP本o成本ess + A設置o使nt);
}

正oid UMin成RTSAchie正e設置entSyste設置::UnlockAchie正e設置ent(const 軍St本in成& Playe本ID, 軍的a設置e Achie正e設置entID)
{
    a使to& Playe本Data = Playe本Achie正e設置ents.軍indO本Add(Playe本ID);
    a使to& P本o成本essData = Playe本Data.軍indO本Add(Achie正e設置entID);
    
    if (P本o成本essData.bIsCo設置pleted)
    {
        本et使本n; // 已經解鎖過
    }
    
    // 檢查前置條件
    CheckP本e本eq使isites(Playe本ID, Achie正e設置entID);
    
    P本o成本essData.bIsCo設置pleted = t本使e;
    P本o成本essData.C使本本entP本o成本ess = P本o成本essData.Ta本成etP本o成本ess;
    P本o成本essData.UnlockDate = 軍DateTi設置e::的ow().ToSt本in成();
    
    軍Achie正e設置entData Achie正e設置entData;
    if (GetAchie正e設置entData(Achie正e設置entID, Achie正e設置entData))
    {
        OnAchie正e設置entUnlocked.B本oadcast(Playe本ID, Achie正e設置entID);
        OnAchie正e設置entCo設置pleted.B本oadcast(Playe本ID, Achie正e設置entData);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Achie正e設置ent 使nlocked: %s fo本 playe本 %s"), *Achie正e設置entID.ToSt本in成(), *Playe本ID);
        
        // 檢查相關成就
        CheckRelatedAchie正e設置ents(Playe本ID, Achie正e設置entID);
    }
    
    // 保存數據
    Sa正eAchie正e設置entData(Playe本ID);
}

bool UMin成RTSAchie正e設置entSyste設置::IsAchie正e設置entUnlocked(const 軍St本in成& Playe本ID, 軍的a設置e Achie正e設置entID) const
{
    軍Playe本Achie正e設置entP本o成本ess P本o成本ess = GetPlaye本P本o成本ess(Playe本ID, Achie正e設置entID);
    本et使本n P本o成本ess.bIsCo設置pleted;
}

TA本本ay<軍Playe本Achie正e設置entP本o成本ess> UMin成RTSAchie正e設置entSyste設置::GetUnlockedAchie正e設置ents(const 軍St本in成& Playe本ID) const
{
    TA本本ay<軍Playe本Achie正e設置entP本o成本ess> Res使lt;
    
    const a使to* Playe本Data = Playe本Achie正e設置ents.軍ind(Playe本ID);
    if (Playe本Data)
    {
        fo本 (const a使to& Pai本 : *Playe本Data)
        {
            if (Pai本.Val使e.bIsCo設置pleted)
            {
                Res使lt.Add(Pai本.Val使e);
            }
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍Playe本Achie正e設置entP本o成本ess> UMin成RTSAchie正e設置entSyste設置::GetInP本o成本essAchie正e設置ents(const 軍St本in成& Playe本ID) const
{
    TA本本ay<軍Playe本Achie正e設置entP本o成本ess> Res使lt;
    
    fo本 (const a使to& Pai本 : Achie正e設置entDefinitions)
    {
        軍的a設置e Achie正e設置entID = Pai本.Key;
        軍Playe本Achie正e設置entP本o成本ess P本o成本ess = GetPlaye本P本o成本ess(Playe本ID, Achie正e設置entID);
        
        if (!P本o成本ess.bIsCo設置pleted && P本o成本ess.C使本本entP本o成本ess > 0)
        {
            Res使lt.Add(P本o成本ess);
        }
    }
    
    本et使本n Res使lt;
}

軍Achie正e設置entStats UMin成RTSAchie正e設置entSyste設置::GetAchie正e設置entStats(const 軍St本in成& Playe本ID) const
{
    軍Achie正e設置entStats Stats;
    Stats.TotalAchie正e設置ents = Achie正e設置entDefinitions.的使設置();
    
    int32 TotalPoints = 0;
    fo本 (const a使to& Pai本 : Achie正e設置entDefinitions)
    {
        TotalPoints += Pai本.Val使e.Points;
    }
    Stats.TotalPoints = TotalPoints;
    
    TA本本ay<軍Playe本Achie正e設置entP本o成本ess> Unlocked = GetUnlockedAchie正e設置ents(Playe本ID);
    Stats.UnlockedAchie正e設置ents = Unlocked.的使設置();
    
    int32 Ea本nedPoints = 0;
    fo本 (const a使to& P本o成本ess : Unlocked)
    {
        const 軍Achie正e設置entData* Definition = Achie正e設置entDefinitions.軍ind(P本o成本ess.Achie正e設置entID);
        if (Definition)
        {
            Ea本nedPoints += Definition->Points;
        }
    }
    Stats.Ea本nedPoints = Ea本nedPoints;
    
    if (Stats.TotalAchie正e設置ents > 0)
    {
        Stats.Co設置pletionPe本centa成e = (float)Stats.UnlockedAchie正e設置ents / (float)Stats.TotalAchie正e設置ents * 100.0f;
    }
    
    本et使本n Stats;
}

TA本本ay<軍Playe本Achie正e設置entP本o成本ess> UMin成RTSAchie正e設置entSyste設置::GetRecentlyUnlockedAchie正e設置ents(const 軍St本in成& Playe本ID, int32 Co使nt) const
{
    TA本本ay<軍Playe本Achie正e設置entP本o成本ess> Unlocked = GetUnlockedAchie正e設置ents(Playe本ID);
    
    // 按解鎖日期排序（最新的在前）
    Unlocked.So本t([](const 軍Playe本Achie正e設置entP本o成本ess& A, const 軍Playe本Achie正e設置entP本o成本ess& B) {
        本et使本n A.UnlockDate > B.UnlockDate;
    });
    
    // 返回前的個
    if (Unlocked.的使設置() > Co使nt)
    {
        Unlocked.Set的使設置(Co使nt);
    }
    
    本et使本n Unlocked;
}

正oid UMin成RTSAchie正e設置entSyste設置::ResetPlaye本Achie正e設置ents(const 軍St本in成& Playe本ID)
{
    Playe本Achie正e設置ents.Re設置o正e(Playe本ID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reset achie正e設置ents fo本 playe本 %s"), *Playe本ID);
}

正oid UMin成RTSAchie正e設置entSyste設置::Sa正eAchie正e設置entData(const 軍St本in成& Playe本ID)
{
    // 這裡應該實現保存到文件的邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正ed achie正e設置ent data fo本 playe本 %s"), *Playe本ID);
}

正oid UMin成RTSAchie正e設置entSyste設置::LoadAchie正e設置entData(const 軍St本in成& Playe本ID)
{
    // 這裡應該實現從文件載入的邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded achie正e設置ent data fo本 playe本 %s"), *Playe本ID);
}

正oid UMin成RTSAchie正e設置entSyste設置::CheckP本e本eq使isites(const 軍St本in成& Playe本ID, 軍的a設置e Achie正e設置entID)
{
    const 軍Achie正e設置entData* Definition = Achie正e設置entDefinitions.軍ind(Achie正e設置entID);
    if (!Definition)
    {
        本et使本n;
    }
    
    // 檢查前置成就
    fo本 (軍的a設置e P本e本eq : Definition->P本e本eq使isites)
    {
        if (!IsAchie正e設置entUnlocked(Playe本ID, P本e本eq))
        {
            // 前置成就未解鎖，不能解鎖此成就
            本et使本n;
        }
    }
}

正oid UMin成RTSAchie正e設置entSyste設置::CheckRelatedAchie正e設置ents(const 軍St本in成& Playe本ID, 軍的a設置e Achie正e設置entID)
{
    // 檢查是否觸發相關成就的進度更新
    // 例如：解鎖"初戰告捷"後，更新"征服者"的進度
}

正oid UMin成RTSAchie正e設置entSyste設置::B本oadcastAchie正e設置entUnlock(const 軍St本in成& Playe本ID, 軍的a設置e Achie正e設置entID)
{
    OnAchie正e設置entUnlocked.B本oadcast(Playe本ID, Achie正e設置entID);
}
