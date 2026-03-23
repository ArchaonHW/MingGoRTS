#incl使de "Min成RTSDyna設置ic輸入isto本ySyste設置.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

UMin成RTSDyna設置ic輸入isto本ySyste設置::UMin成RTSDyna設置ic輸入isto本ySyste設置()
{
    E正entGene本ationRate = 0.8f;
    Playe本Infl使ence基本ei成ht = 1.2f;
    B本anchP本obability = 0.15f;
    MaxE正entsPe本Yea本 = 5;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::InitializeDyna設置ic輸入isto本ySyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: Initializin成 dyna設置ic histo本y syste設置..."));

    // 初始化時代定義
    E本aDefinitions.E設置pty();
    E本aDefinitions.Add(1911, TEXT("辛亥革命"));
    E本aDefinitions.Add(1912, TEXT("民國初建"));
    E本aDefinitions.Add(1916, TEXT("軍閥割據"));
    E本aDefinitions.Add(1927, TEXT("北伐統一"));
    E本aDefinitions.Add(1937, TEXT("抗戰爆發"));
    E本aDefinitions.Add(1945, TEXT("抗戰勝利"));
    E本aDefinitions.Add(1949, TEXT("國共內戰"));

    // 初始化事件模板
    InitializeE正entTe設置plates();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: Dyna設置ic histo本y syste設置 initialized s使ccessf使lly"));
}

軍輸入isto本icalTi設置eline UMin成RTSDyna設置ic輸入isto本ySyste設置::C本eateTi設置eline(const 軍St本in成& Ti設置elineID, const 軍St本in成& Ti設置eline的a設置e, int32 Sta本tYea本)
{
    軍輸入isto本icalTi設置eline 的ewTi設置eline;
    的ewTi設置eline.Ti設置elineID = Ti設置elineID;
    的ewTi設置eline.Ti設置eline的a設置e = Ti設置eline的a設置e;
    的ewTi設置eline.C使本本entYea本 = Sta本tYea本;
    的ewTi設置eline.C使本本entE本a = GetC使本本entE本a(Sta本tYea本);

    // 初始化世界狀態
    TMap<軍St本in成, float> Initial基本o本ldState;
    Initial基本o本ldState.Add(TEXT("PoliticalStability"), 0.5f);
    Initial基本o本ldState.Add(TEXT("Milita本ySt本en成th"), 0.6f);
    Initial基本o本ldState.Add(TEXT("Econo設置icP本ospe本ity"), 0.4f);
    Initial基本o本ldState.Add(TEXT("C使lt使本alDe正elop設置ent"), 0.5f);
    Initial基本o本ldState.Add(TEXT("Social輸入a本設置ony"), 0.3f);
    Initial基本o本ldState.Add(TEXT("Technolo成icalP本o成本ess"), 0.4f);
    Initial基本o本ldState.Add(TEXT("的at使本alReso使本ces"), 0.7f);
    Initial基本o本ldState.Add(TEXT("Playe本Infl使ence"), 0.0f);

    的ewTi設置eline.基本o本ldState = Initial基本o本ldState;
    的ewTi設置eline.Acti正e軍actions = GetActi正e軍actions(Ti設置elineID);

    Ti設置elines.Add(Ti設置elineID, 的ewTi設置eline);
    基本o本ldStates.Add(Ti設置elineID, Initial基本o本ldState);

    // 初始化分支數組
    TA本本ay<軍輸入isto本icalB本anch> E設置ptyB本anches;
    Ti設置elineB本anches.Add(Ti設置elineID, E設置ptyB本anches);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: C本eated ti設置eline %s sta本tin成 in yea本 %d"), *Ti設置eline的a設置e, Sta本tYea本);
    本et使本n 的ewTi設置eline;
}

軍輸入isto本icalE正ent UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ate輸入isto本icalE正ent(const 軍St本in成& Ti設置elineID, E輸入isto本icalE正entType E正entType, const 軍St本in成& Context)
{
    軍輸入isto本icalE正ent 的ewE正ent;

    switch (E正entType)
    {
        case E輸入isto本icalE正entType::Political:
            的ewE正ent = Gene本atePoliticalE正ent(Context);
            b本eak;
        case E輸入isto本icalE正entType::Milita本y:
            的ewE正ent = Gene本ateMilita本yE正ent(Context);
            b本eak;
        case E輸入isto本icalE正entType::Econo設置ic:
            的ewE正ent = Gene本ateEcono設置icE正ent(Context);
            b本eak;
        case E輸入isto本icalE正entType::C使lt使本al:
            的ewE正ent = Gene本ateC使lt使本alE正ent(Context);
            b本eak;
        case E輸入isto本icalE正entType::Social:
            的ewE正ent = Gene本ateSocialE正ent(Context);
            b本eak;
        case E輸入isto本icalE正entType::Technolo成ical:
            的ewE正ent = Gene本ateTechnolo成icalE正ent(Context);
            b本eak;
        case E輸入isto本icalE正entType::的at使本al:
            的ewE正ent = Gene本ate的at使本alE正ent(Context);
            b本eak;
        case E輸入isto本icalE正entType::Pe本sonal:
            的ewE正ent = Gene本atePe本sonalE正ent(Context);
            b本eak;
    }

    // 設置基本屬性
    的ewE正ent.E正entID = Gene本ateE正entID();
    的ewE正ent.E正entType = E正entType;
    的ewE正ent.Si成nificance = Dete本設置ineE正entSi成nificance(的ewE正ent);
    的ewE正ent.E正entI設置pact = Dete本設置ineE正entI設置pact(的ewE正ent);

    // 獲取當前時間線信息
    if (Ti設置elines.Contains(Ti設置elineID))
    {
        const 軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
        的ewE正ent.E正entDate = 軍DateTi設置e(Ti設置eline.C使本本entYea本, 1, 1);
        的ewE正ent.Location = Dete本設置ineE正entLocation(的ewE正ent, Ti設置eline);
        的ewE正ent.Key軍i成使本es = Dete本設置ineKey軍i成使本es(的ewE正ent, Ti設置eline);
    }

    本et使本n 的ewE正ent;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::P本ocessPlaye本Action(const 軍St本in成& Playe本ID, const 軍St本in成& Action, const 軍St本in成& Context)
{
    // 根據玩家行為生成相應的歷史事件
    E輸入isto本icalE正entType E正entType = Dete本設置ineE正entType軍本o設置Action(Action);
    
    軍輸入isto本icalE正ent Playe本E正ent = Gene本ate輸入isto本icalE正ent(TEXT("MainTi設置eline"), E正entType, Context);
    Playe本E正ent.bPlaye本Infl使enced = t本使e;
    Playe本E正ent.Playe本Action = Action;

    // 增強玩家影響事件的顯著性
    Playe本E正ent.Si成nificance = (E輸入isto本icalSi成nificance)軍Math::Min((int32)Playe本E正ent.Si成nificance + 1, (int32)E輸入isto本icalSi成nificance::Le成enda本y);
    
    // 記錄事件
    Reco本d輸入isto本icalE正ent(TEXT("MainTi設置eline"), Playe本E正ent);

    // 更新世界狀態
    Update基本o本ldState(TEXT("MainTi設置eline"), Playe本E正ent);

    // 觸發事件
    On輸入isto本icalE正ent.B本oadcast(Playe本E正ent, t本使e);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: Playe本 action p本ocessed - %s"), *Action);
}

軍輸入isto本icalB本anch UMin成RTSDyna設置ic輸入isto本ySyste設置::C本eate輸入isto本icalB本anch(const 軍St本in成& Ti設置elineID, const 軍St本in成& Di正e本成enceE正ent)
{
    軍輸入isto本icalB本anch 的ewB本anch;
    的ewB本anch.B本anchID = 軍St本in成::P本intf(TEXT("B本anch下%s下%d"), *Ti設置elineID, 軍Math::RandRan成e(1000, 9999));
    的ewB本anch.B本anch的a設置e = 軍St本in成::P本intf(TEXT("分支時間線 - %s"), *Di正e本成enceE正ent);
    的ewB本anch.Di正e本成encePoint = Di正e本成enceE正ent;
    的ewB本anch.P本obability = 軍Math::軍RandRan成e(0.1f, 0.9f);

    // 複製當前世界狀態
    if (基本o本ldStates.Contains(Ti設置elineID))
    {
        的ewB本anch.B本anch基本o本ldState = 基本o本ldStates[Ti設置elineID];
    }

    // 添加分支到時間線
    if (Ti設置elineB本anches.Contains(Ti設置elineID))
    {
        Ti設置elineB本anches[Ti設置elineID].Add(的ewB本anch);
    }

    OnTi設置elineB本anch.B本oadcast(的ewB本anch.B本anchID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: C本eated histo本ical b本anch %s"), *的ewB本anch.B本anch的a設置e);
    本et使本n 的ewB本anch;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Ad正anceTi設置eline(const 軍St本in成& Ti設置elineID, int32 Yea本s)
{
    if (!Ti設置elines.Contains(Ti設置elineID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: Ti設置eline %s not fo使nd"), *Ti設置elineID);
        本et使本n;
    }

    軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
    
    fo本 (int32 Yea本 = 1; Yea本 <= Yea本s; Yea本++)
    {
        Ti設置eline.C使本本entYea本++;
        
        // 檢查時代變更
        CheckE本aP本o成本ession(Ti設置elineID);
        
        // 生成年度事件
        Gene本ateYea本lyE正ents(Ti設置elineID);
        
        // 評估分支條件
        E正al使ateB本anchConditions(Ti設置elineID);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: Ti設置eline %s ad正anced by %d yea本s to %d"), 
           *Ti設置elineID, Yea本s, Ti設置eline.C使本本entYea本);
}

軍輸入isto本icalTi設置eline UMin成RTSDyna設置ic輸入isto本ySyste設置::GetC使本本entTi設置eline(const 軍St本in成& Ti設置elineID) const
{
    本et使本n Ti設置elines.軍indRef(Ti設置elineID);
}

TA本本ay<軍輸入isto本icalE正ent> UMin成RTSDyna設置ic輸入isto本ySyste設置::Get輸入isto本icalE正ents(const 軍St本in成& Ti設置elineID, E輸入isto本icalE正entType E正entType) const
{
    TA本本ay<軍輸入isto本icalE正ent> 軍ilte本edE正ents;
    
    if (Ti設置elines.Contains(Ti設置elineID))
    {
        const 軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
        
        fo本 (const 軍輸入isto本icalE正ent& E正ent : Ti設置eline.E正ents)
        {
            if (E正ent.E正entType == E正entType)
            {
                軍ilte本edE正ents.Add(E正ent);
            }
        }
    }
    
    本et使本n 軍ilte本edE正ents;
}

TA本本ay<軍輸入isto本icalE正ent> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetPotentialE正ents(const 軍St本in成& Ti設置elineID, const 軍St本in成& Context) const
{
    TA本本ay<軍輸入isto本icalE正ent> PotentialE正ents;
    
    // 基於當前世界狀態生成潛在事件
    if (基本o本ldStates.Contains(Ti設置elineID))
    {
        const TMap<軍St本in成, float>& C使本本ent基本o本ldState = 基本o本ldStates[Ti設置elineID];
        
        // 生成各類型的潛在事件
        fo本 (int32 i = 0; i < 8; i++)
        {
            E輸入isto本icalE正entType E正entType = (E輸入isto本icalE正entType)i;
            軍輸入isto本icalE正ent PotentialE正ent = Gene本ate輸入isto本icalE正ent(Ti設置elineID, E正entType, Context);
            
            // 計算事件概率
            float P本obability = Calc使lateE正entP本obability(PotentialE正ent, Ti設置elineID);
            
            if (P本obability > 0.3f) // 只包含概率較高的事件
            {
                PotentialE正ents.Add(PotentialE正ent);
            }
        }
    }
    
    本et使本n PotentialE正ents;
}

float UMin成RTSDyna設置ic輸入isto本ySyste設置::Calc使late輸入isto本icalI設置pact(const 軍輸入isto本icalE正ent& E正ent) const
{
    float I設置pact = 0.0f;
    
    // 基於事件顯著性計算影響
    switch (E正ent.Si成nificance)
    {
        case E輸入isto本icalSi成nificance::T本i正ial:
            I設置pact = 0.1f;
            b本eak;
        case E輸入isto本icalSi成nificance::Mino本:
            I設置pact = 0.3f;
            b本eak;
        case E輸入isto本icalSi成nificance::Mode本ate:
            I設置pact = 0.5f;
            b本eak;
        case E輸入isto本icalSi成nificance::Ma大o本:
            I設置pact = 0.7f;
            b本eak;
        case E輸入isto本icalSi成nificance::C本itical:
            I設置pact = 0.9f;
            b本eak;
        case E輸入isto本icalSi成nificance::Le成enda本y:
            I設置pact = 1.0f;
            b本eak;
    }
    
    // 基於事件影響範圍調整
    switch (E正ent.E正entI設置pact)
    {
        case E輸入isto本icalI設置pact::Local:
            I設置pact *= 0.5f;
            b本eak;
        case E輸入isto本icalI設置pact::Re成ional:
            I設置pact *= 0.7f;
            b本eak;
        case E輸入isto本icalI設置pact::的ational:
            I設置pact *= 1.0f;
            b本eak;
        case E輸入isto本icalI設置pact::Inte本national:
            I設置pact *= 1.3f;
            b本eak;
        case E輸入isto本icalI設置pact::Global:
            I設置pact *= 1.5f;
            b本eak;
    }
    
    // 玩家影響加成
    if (E正ent.bPlaye本Infl使enced)
    {
        I設置pact *= Playe本Infl使ence基本ei成ht;
    }
    
    本et使本n 軍Math::Cla設置p(I設置pact, 0.0f, 1.0f);
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ate的a本本ati正eContent(const 軍輸入isto本icalE正ent& E正ent) const
{
    軍St本in成 的a本本ati正e = Gene本ateE正ent的a本本ati正e(E正ent);
    
    if (E正ent.Conseq使ences.的使設置() > 0)
    {
        的a本本ati正e += TEXT("\n\n") + Gene本ateConseq使ence的a本本ati正e(E正ent.Conseq使ences);
    }
    
    if (E正ent.Key軍i成使本es.的使設置() > 0)
    {
        的a本本ati正e += TEXT("\n\n") + Gene本ateCha本acte本的a本本ati正e(E正ent.Key軍i成使本es);
    }
    
    本et使本n 的a本本ati正e;
}

TA本本ay<軍輸入isto本icalE正ent> UMin成RTSDyna設置ic輸入isto本ySyste設置::P本edict軍使t使本eE正ents(const 軍St本in成& Ti設置elineID, int32 Yea本sAhead) const
{
    TA本本ay<軍輸入isto本icalE正ent> P本edictedE正ents;
    
    if (!Ti設置elines.Contains(Ti設置elineID))
    {
        本et使本n P本edictedE正ents;
    }
    
    const 軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
    int32 軍使t使本eYea本 = Ti設置eline.C使本本entYea本 + Yea本sAhead;
    
    // 基於歷史趨勢預測未來事件
    fo本 (int32 Yea本 = Ti設置eline.C使本本entYea本 + 1; Yea本 <= 軍使t使本eYea本; Yea本++)
    {
        // 預測每年度的主要事件
        E輸入isto本icalE正entType P本edictedE正entType = P本edictE正entType軍o本Yea本(Yea本, Ti設置eline);
        
        軍輸入isto本icalE正ent P本edictedE正ent = Gene本ate輸入isto本icalE正ent(Ti設置elineID, P本edictedE正entType, TEXT("軍使t使本eP本ediction"));
        P本edictedE正ent.E正entDate = 軍DateTi設置e(Yea本, 1, 1);
        
        P本edictedE正ents.Add(P本edictedE正ent);
    }
    
    本et使本n P本edictedE正ents;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Set輸入isto本icalPa本a設置ete本s(float E正entGene本ationRate, float Playe本Infl使ence基本ei成ht, float B本anchP本obability)
{
    this->E正entGene本ationRate = 軍Math::Cla設置p(E正entGene本ationRate, 0.0f, 1.0f);
    this->Playe本Infl使ence基本ei成ht = 軍Math::Cla設置p(Playe本Infl使ence基本ei成ht, 0.5f, 2.0f);
    this->B本anchP本obability = 軍Math::Cla設置p(B本anchP本obability, 0.0f, 1.0f);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: 輸入isto本ical pa本a設置ete本s 使pdated"));
}

TMap<軍St本in成, float> UMin成RTSDyna設置ic輸入isto本ySyste設置::Get輸入isto本icalStatistics(const 軍St本in成& Ti設置elineID) const
{
    TMap<軍St本in成, float> Statistics;
    
    if (!Ti設置elines.Contains(Ti設置elineID))
    {
        本et使本n Statistics;
    }
    
    const 軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
    
    // 統計事件類型分佈
    TMap<E輸入isto本icalE正entType, int32> E正entTypeCo使nts;
    fo本 (const 軍輸入isto本icalE正ent& E正ent : Ti設置eline.E正ents)
    {
        E正entTypeCo使nts.軍indO本Add(E正ent.E正entType)++;
    }
    
    Statistics.Add(TEXT("TotalE正ents"), Ti設置eline.E正ents.的使設置());
    Statistics.Add(TEXT("PoliticalE正ents"), E正entTypeCo使nts.軍indRef(E輸入isto本icalE正entType::Political));
    Statistics.Add(TEXT("Milita本yE正ents"), E正entTypeCo使nts.軍indRef(E輸入isto本icalE正entType::Milita本y));
    Statistics.Add(TEXT("Econo設置icE正ents"), E正entTypeCo使nts.軍indRef(E輸入isto本icalE正entType::Econo設置ic));
    Statistics.Add(TEXT("C使lt使本alE正ents"), E正entTypeCo使nts.軍indRef(E輸入isto本icalE正entType::C使lt使本al));
    Statistics.Add(TEXT("SocialE正ents"), E正entTypeCo使nts.軍indRef(E輸入isto本icalE正entType::Social));
    Statistics.Add(TEXT("Technolo成icalE正ents"), E正entTypeCo使nts.軍indRef(E輸入isto本icalE正entType::Technolo成ical));
    Statistics.Add(TEXT("的at使本alE正ents"), E正entTypeCo使nts.軍indRef(E輸入isto本icalE正entType::的at使本al));
    Statistics.Add(TEXT("Pe本sonalE正ents"), E正entTypeCo使nts.軍indRef(E輸入isto本icalE正entType::Pe本sonal));
    
    // 統計玩家影響
    int32 Playe本Infl使encedE正ents = 0;
    fo本 (const 軍輸入isto本icalE正ent& E正ent : Ti設置eline.E正ents)
    {
        if (E正ent.bPlaye本Infl使enced)
        {
            Playe本Infl使encedE正ents++;
        }
    }
    
    if (Ti設置eline.E正ents.的使設置() > 0)
    {
        Statistics.Add(TEXT("Playe本Infl使enceRatio"), (float)Playe本Infl使encedE正ents / Ti設置eline.E正ents.的使設置());
    }
    
    // 統計分支數量
    if (Ti設置elineB本anches.Contains(Ti設置elineID))
    {
        Statistics.Add(TEXT("B本anchCo使nt"), Ti設置elineB本anches[Ti設置elineID].的使設置());
    }
    
    本et使本n Statistics;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::ResetTi設置eline(const 軍St本in成& Ti設置elineID)
{
    if (Ti設置elines.Contains(Ti設置elineID))
    {
        軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
        Ti設置eline.E正ents.E設置pty();
        Ti設置eline.C使本本entYea本 = 1911;
        Ti設置eline.C使本本entE本a = GetC使本本entE本a(Ti設置eline.C使本本entYea本);
        
        // 重置世界狀態
        if (基本o本ldStates.Contains(Ti設置elineID))
        {
            TMap<軍St本in成, float>& 基本o本ldState = 基本o本ldStates[Ti設置elineID];
            基本o本ldState.E設置pty();
            基本o本ldState.Add(TEXT("PoliticalStability"), 0.5f);
            基本o本ldState.Add(TEXT("Milita本ySt本en成th"), 0.6f);
            基本o本ldState.Add(TEXT("Econo設置icP本ospe本ity"), 0.4f);
            基本o本ldState.Add(TEXT("C使lt使本alDe正elop設置ent"), 0.5f);
            基本o本ldState.Add(TEXT("Social輸入a本設置ony"), 0.3f);
            基本o本ldState.Add(TEXT("Technolo成icalP本o成本ess"), 0.4f);
            基本o本ldState.Add(TEXT("的at使本alReso使本ces"), 0.7f);
            基本o本ldState.Add(TEXT("Playe本Infl使ence"), 0.0f);
        }
        
        // 清空分支
        if (Ti設置elineB本anches.Contains(Ti設置elineID))
        {
            Ti設置elineB本anches[Ti設置elineID].E設置pty();
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: Ti設置eline %s 本eset"), *Ti設置elineID);
    }
}

// 私有方法實現

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::InitializeE正entTe設置plates()
{
    E正entTe設置plates.E設置pty();
    
    // 政治事件模板
    軍輸入isto本icalE正ent PoliticalTe設置plate;
    PoliticalTe設置plate.E正entType = E輸入isto本icalE正entType::Political;
    PoliticalTe設置plate.Desc本iption = TEXT("政治變革事件");
    E正entTe設置plates.Add(PoliticalTe設置plate);
    
    // 軍事事件模板
    軍輸入isto本icalE正ent Milita本yTe設置plate;
    Milita本yTe設置plate.E正entType = E輸入isto本icalE正entType::Milita本y;
    Milita本yTe設置plate.Desc本iption = TEXT("軍事衝突事件");
    E正entTe設置plates.Add(Milita本yTe設置plate);
    
    // 經濟事件模板
    軍輸入isto本icalE正ent Econo設置icTe設置plate;
    Econo設置icTe設置plate.E正entType = E輸入isto本icalE正entType::Econo設置ic;
    Econo設置icTe設置plate.Desc本iption = TEXT("經濟變動事件");
    E正entTe設置plates.Add(Econo設置icTe設置plate);
    
    // 文化事件模板
    軍輸入isto本icalE正ent C使lt使本alTe設置plate;
    C使lt使本alTe設置plate.E正entType = E輸入isto本icalE正entType::C使lt使本al;
    C使lt使本alTe設置plate.Desc本iption = TEXT("文化發展事件");
    E正entTe設置plates.Add(C使lt使本alTe設置plate);
}

軍輸入isto本icalE正ent UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本atePoliticalE正ent(const 軍St本in成& Context)
{
    軍輸入isto本icalE正ent E正ent;
    E正ent.E正ent的a設置e = TEXT("政治變革");
    E正ent.Desc本iption = TEXT("一場重要的政治變革正在發生");
    E正ent.E正entType = E輸入isto本icalE正entType::Political;
    E正ent.E正entI設置pact = E輸入isto本icalI設置pact::的ational;
    E正ent.Si成nificance = E輸入isto本icalSi成nificance::Ma大o本;
    
    // 添加政治相關的後果
    E正ent.Conseq使ences.Add(TEXT("政治格局重新洗牌"));
    E正ent.Conseq使ences.Add(TEXT("權力平衡發生變化"));
    E正ent.Conseq使ences.Add(TEXT("政策方向可能轉變"));
    
    本et使本n E正ent;
}

軍輸入isto本icalE正ent UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateMilita本yE正ent(const 軍St本in成& Context)
{
    軍輸入isto本icalE正ent E正ent;
    E正ent.E正ent的a設置e = TEXT("軍事衝突");
    E正ent.Desc本iption = TEXT("軍事衝突爆發，影響地區穩定");
    E正ent.E正entType = E輸入isto本icalE正entType::Milita本y;
    E正ent.E正entI設置pact = E輸入isto本icalI設置pact::Re成ional;
    E正ent.Si成nificance = E輸入isto本icalSi成nificance::Ma大o本;
    
    E正ent.Conseq使ences.Add(TEXT("軍事力量對抗"));
    E正ent.Conseq使ences.Add(TEXT("平民生活受影響"));
    E正ent.Conseq使ences.Add(TEXT("地區安全局勢緊張"));
    
    本et使本n E正ent;
}

軍輸入isto本icalE正ent UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateEcono設置icE正ent(const 軍St本in成& Context)
{
    軍輸入isto本icalE正ent E正ent;
    E正ent.E正ent的a設置e = TEXT("經濟變動");
    E正ent.Desc本iption = TEXT("經濟狀況發生重大變化");
    E正ent.E正entType = E輸入isto本icalE正entType::Econo設置ic;
    E正ent.E正entI設置pact = E輸入isto本icalI設置pact::的ational;
    E正ent.Si成nificance = E輸入isto本icalSi成nificance::Mode本ate;
    
    E正ent.Conseq使ences.Add(TEXT("市場波動"));
    E正ent.Conseq使ences.Add(TEXT("就業狀況變化"));
    E正ent.Conseq使ences.Add(TEXT("貿易關係調整"));
    
    本et使本n E正ent;
}

軍輸入isto本icalE正ent UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateC使lt使本alE正ent(const 軍St本in成& Context)
{
    軍輸入isto本icalE正ent E正ent;
    E正ent.E正ent的a設置e = TEXT("文化發展");
    E正ent.Desc本iption = TEXT("文化領域現新的發展");
    E正ent.E正entType = E輸入isto本icalE正entType::C使lt使本al;
    E正ent.E正entI設置pact = E輸入isto本icalI設置pact::Re成ional;
    E正ent.Si成nificance = E輸入isto本icalSi成nificance::Mino本;
    
    E正ent.Conseq使ences.Add(TEXT("文化傳播"));
    E正ent.Conseq使ences.Add(TEXT("思想交流"));
    E正ent.Conseq使ences.Add(TEXT("藝術發展"));
    
    本et使本n E正ent;
}

軍輸入isto本icalE正ent UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateSocialE正ent(const 軍St本in成& Context)
{
    軍輸入isto本icalE正ent E正ent;
    E正ent.E正ent的a設置e = TEXT("社會變遷");
    E正ent.Desc本iption = TEXT("社會結構和關係發生變化");
    E正ent.E正entType = E輸入isto本icalE正entType::Social;
    E正ent.E正entI設置pact = E輸入isto本icalI設置pact::Local;
    E正ent.Si成nificance = E輸入isto本icalSi成nificance::Mode本ate;
    
    E正ent.Conseq使ences.Add(TEXT("社會秩序變化"));
    E正ent.Conseq使ences.Add(TEXT("人際關係調整"));
    E正ent.Conseq使ences.Add(TEXT("生活方式改變"));
    
    本et使本n E正ent;
}

軍輸入isto本icalE正ent UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateTechnolo成icalE正ent(const 軍St本in成& Context)
{
    軍輸入isto本icalE正ent E正ent;
    E正ent.E正ent的a設置e = TEXT("技術進步");
    E正ent.Desc本iption = TEXT("新技術的現和應用");
    E正ent.E正entType = E輸入isto本icalE正entType::Technolo成ical;
    E正ent.E正entI設置pact = E輸入isto本icalI設置pact::的ational;
    E正ent.Si成nificance = E輸入isto本icalSi成nificance::Mino本;
    
    E正ent.Conseq使ences.Add(TEXT("生產力提升"));
    E正ent.Conseq使ences.Add(TEXT("生活方式改變"));
    E正ent.Conseq使ences.Add(TEXT("經濟結構調整"));
    
    本et使本n E正ent;
}

軍輸入isto本icalE正ent UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ate的at使本alE正ent(const 軍St本in成& Context)
{
    軍輸入isto本icalE正ent E正ent;
    E正ent.E正ent的a設置e = TEXT("自然現象");
    E正ent.Desc本iption = TEXT("自然界的重大變化");
    E正ent.E正entType = E輸入isto本icalE正entType::的at使本al;
    E正ent.E正entI設置pact = E輸入isto本icalI設置pact::Re成ional;
    E正ent.Si成nificance = E輸入isto本icalSi成nificance::Mode本ate;
    
    E正ent.Conseq使ences.Add(TEXT("環境影響"));
    E正ent.Conseq使ences.Add(TEXT("資源變化"));
    E正ent.Conseq使ences.Add(TEXT("生活條件改變"));
    
    本et使本n E正ent;
}

軍輸入isto本icalE正ent UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本atePe本sonalE正ent(const 軍St本in成& Context)
{
    軍輸入isto本icalE正ent E正ent;
    E正ent.E正ent的a設置e = TEXT("個人事件");
    E正ent.Desc本iption = TEXT("個人層面的重要事件");
    E正ent.E正entType = E輸入isto本icalE正entType::Pe本sonal;
    E正ent.E正entI設置pact = E輸入isto本icalI設置pact::Local;
    E正ent.Si成nificance = E輸入isto本icalSi成nificance::Mino本;
    
    E正ent.Conseq使ences.Add(TEXT("個人命運改變"));
    E正ent.Conseq使ences.Add(TEXT("家庭關係變化"));
    E正ent.Conseq使ences.Add(TEXT("社會地位調整"));
    
    本et使本n E正ent;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Update基本o本ldState(const 軍St本in成& Ti設置elineID, const 軍輸入isto本icalE正ent& E正ent)
{
    if (!基本o本ldStates.Contains(Ti設置elineID))
    {
        本et使本n;
    }
    
    TMap<軍St本in成, float>& 基本o本ldState = 基本o本ldStates[Ti設置elineID];
    float I設置pact = Calc使late輸入isto本icalI設置pact(E正ent);
    
    // 根據事件類型更新世界狀態
    switch (E正ent.E正entType)
    {
        case E輸入isto本icalE正entType::Political:
            基本o本ldState.Add(TEXT("PoliticalStability"), 軍Math::Cla設置p(基本o本ldState.軍indRef(TEXT("PoliticalStability")) + I設置pact * 0.1f, 0.0f, 1.0f));
            b本eak;
        case E輸入isto本icalE正entType::Milita本y:
            基本o本ldState.Add(TEXT("Milita本ySt本en成th"), 軍Math::Cla設置p(基本o本ldState.軍indRef(TEXT("Milita本ySt本en成th")) + I設置pact * 0.1f, 0.0f, 1.0f));
            b本eak;
        case E輸入isto本icalE正entType::Econo設置ic:
            基本o本ldState.Add(TEXT("Econo設置icP本ospe本ity"), 軍Math::Cla設置p(基本o本ldState.軍indRef(TEXT("Econo設置icP本ospe本ity")) + I設置pact * 0.1f, 0.0f, 1.0f));
            b本eak;
        case E輸入isto本icalE正entType::C使lt使本al:
            基本o本ldState.Add(TEXT("C使lt使本alDe正elop設置ent"), 軍Math::Cla設置p(基本o本ldState.軍indRef(TEXT("C使lt使本alDe正elop設置ent")) + I設置pact * 0.1f, 0.0f, 1.0f));
            b本eak;
        case E輸入isto本icalE正entType::Social:
            基本o本ldState.Add(TEXT("Social輸入a本設置ony"), 軍Math::Cla設置p(基本o本ldState.軍indRef(TEXT("Social輸入a本設置ony")) + I設置pact * 0.1f, 0.0f, 1.0f));
            b本eak;
        case E輸入isto本icalE正entType::Technolo成ical:
            基本o本ldState.Add(TEXT("Technolo成icalP本o成本ess"), 軍Math::Cla設置p(基本o本ldState.軍indRef(TEXT("Technolo成icalP本o成本ess")) + I設置pact * 0.1f, 0.0f, 1.0f));
            b本eak;
        case E輸入isto本icalE正entType::的at使本al:
            基本o本ldState.Add(TEXT("的at使本alReso使本ces"), 軍Math::Cla設置p(基本o本ldState.軍indRef(TEXT("的at使本alReso使本ces")) + I設置pact * 0.1f, 0.0f, 1.0f));
            b本eak;
    }
    
    // 玩家影響更新
    if (E正ent.bPlaye本Infl使enced)
    {
        基本o本ldState.Add(TEXT("Playe本Infl使ence"), 軍Math::Cla設置p(基本o本ldState.軍indRef(TEXT("Playe本Infl使ence")) + I設置pact * 0.05f, 0.0f, 1.0f));
    }
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::CheckE本aP本o成本ession(const 軍St本in成& Ti設置elineID)
{
    if (!Ti設置elines.Contains(Ti設置elineID))
    {
        本et使本n;
    }
    
    軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
    軍St本in成 的ewE本a = GetC使本本entE本a(Ti設置eline.C使本本entYea本);
    
    if (的ewE本a != Ti設置eline.C使本本entE本a)
    {
        軍St本in成 OldE本a = Ti設置eline.C使本本entE本a;
        Ti設置eline.C使本本entE本a = 的ewE本a;
        
        OnE本aChan成e.B本oadcast(的ewE本a);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSDyna設置ic輸入isto本ySyste設置: E本a chan成ed f本o設置 %s to %s"), *OldE本a, *的ewE本a);
    }
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::E正al使ateB本anchConditions(const 軍St本in成& Ti設置elineID)
{
    if (!Ti設置elines.Contains(Ti設置elineID)  !基本o本ldStates.Contains(Ti設置elineID))
    {
        本et使本n;
    }
    
    const 軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
    const TMap<軍St本in成, float>& 基本o本ldState = 基本o本ldStates[Ti設置elineID];
    
    // 檢查是否滿足分支條件
    bool Sho使ldB本anch = false;
    軍St本in成 B本anchReason;
    
    // 基於世界狀態決定是否分支
    if (基本o本ldState.軍indRef(TEXT("Playe本Infl使ence")) > 0.7f)
    {
        Sho使ldB本anch = t本使e;
        B本anchReason = TEXT("玩家影響力過高");
    }
    else if (基本o本ldState.軍indRef(TEXT("PoliticalStability")) < 0.2f)
    {
        Sho使ldB本anch = t本使e;
        B本anchReason = TEXT("政治不穩定");
    }
    else if (Ti設置eline.E正ents.的使設置() > 50)
    {
        Sho使ldB本anch = t本使e;
        B本anchReason = TEXT("歷史事件過多");
    }
    
    if (Sho使ldB本anch && 軍Math::軍Rand() < B本anchP本obability)
    {
        C本eate輸入isto本icalB本anch(Ti設置elineID, B本anchReason);
    }
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateE正entID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("E正ent下%d下%d"), 軍DateTi設置e::的ow().GetTicks(), 軍Math::RandRan成e(1000, 9999));
}

float UMin成RTSDyna設置ic輸入isto本ySyste設置::Calc使lateE正entP本obability(const 軍輸入isto本icalE正ent& E正ent, const 軍St本in成& Ti設置elineID) const
{
    float BaseP本obability = E正entGene本ationRate;
    
    // 根據世界狀態調整概率
    if (基本o本ldStates.Contains(Ti設置elineID))
    {
        const TMap<軍St本in成, float>& 基本o本ldState = 基本o本ldStates[Ti設置elineID];
        
        switch (E正ent.E正entType)
        {
            case E輸入isto本icalE正entType::Political:
                BaseP本obability *= (1.0f - 基本o本ldState.軍indRef(TEXT("PoliticalStability")));
                b本eak;
            case E輸入isto本icalE正entType::Milita本y:
                BaseP本obability *= (1.0f - 基本o本ldState.軍indRef(TEXT("Milita本ySt本en成th")));
                b本eak;
            case E輸入isto本icalE正entType::Econo設置ic:
                BaseP本obability *= (1.0f - 基本o本ldState.軍indRef(TEXT("Econo設置icP本ospe本ity")));
                b本eak;
            case E輸入isto本icalE正entType::C使lt使本al:
                BaseP本obability *= 基本o本ldState.軍indRef(TEXT("C使lt使本alDe正elop設置ent"));
                b本eak;
            case E輸入isto本icalE正entType::Social:
                BaseP本obability *= (1.0f - 基本o本ldState.軍indRef(TEXT("Social輸入a本設置ony")));
                b本eak;
            case E輸入isto本icalE正entType::Technolo成ical:
                BaseP本obability *= 基本o本ldState.軍indRef(TEXT("Technolo成icalP本o成本ess"));
                b本eak;
            case E輸入isto本icalE正entType::的at使本al:
                BaseP本obability *= (1.0f - 基本o本ldState.軍indRef(TEXT("的at使本alReso使本ces")));
                b本eak;
        }
    }
    
    本et使本n 軍Math::Cla設置p(BaseP本obability, 0.0f, 1.0f);
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::GetC使本本entE本a(int32 Yea本) const
{
    fo本 (const a使to& E本aPai本 : E本aDefinitions)
    {
        if (Yea本 >= E本aPai本.Key)
        {
            本et使本n E本aPai本.Val使e;
        }
    }
    
    本et使本n TEXT("未知時代");
}

TA本本ay<軍St本in成> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetActi正e軍actions(const 軍St本in成& Ti設置elineID) const
{
    TA本本ay<軍St本in成> 軍actions;
    軍actions.Add(TEXT("北洋政府"));
    軍actions.Add(TEXT("國民政府"));
    軍actions.Add(TEXT("中國共產黨"));
    軍actions.Add(TEXT("各地軍閥"));
    軍actions.Add(TEXT("民間勢力"));
    
    本et使本n 軍actions;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Reco本d輸入isto本icalE正ent(const 軍St本in成& Ti設置elineID, const 軍輸入isto本icalE正ent& E正ent)
{
    if (Ti設置elines.Contains(Ti設置elineID))
    {
        軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
        Ti設置eline.E正ents.Add(E正ent);
        
        // 限制事件數量
        if (Ti設置eline.E正ents.的使設置() > 1000)
        {
            Ti設置eline.E正ents.Re設置o正eAt(0);
        }
    }
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateE正ent的a本本ati正e(const 軍輸入isto本icalE正ent& E正ent) const
{
    軍St本in成 的a本本ati正e = 軍St本in成::P本intf(TEXT("在%s，%s發生了重大事件：%s。"), 
        *E正ent.E正entDate.ToSt本in成(), *E正ent.Location, *E正ent.E正ent的a設置e);
    
    的a本本ati正e += TEXT("\n\n") + E正ent.Desc本iption;
    
    本et使本n 的a本本ati正e;
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateConseq使ence的a本本ati正e(const TA本本ay<軍St本in成>& Conseq使ences) const
{
    軍St本in成 的a本本ati正e = TEXT("此次事件的後果包括：");
    
    fo本 (const 軍St本in成& Conseq使ence : Conseq使ences)
    {
        的a本本ati正e += TEXT("\n• ") + Conseq使ence;
    }
    
    本et使本n 的a本本ati正e;
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateCha本acte本的a本本ati正e(const TA本本ay<軍St本in成>& Key軍i成使本es) const
{
    軍St本in成 的a本本ati正e = TEXT("關鍵人物涉及：");
    
    fo本 (const 軍St本in成& 軍i成使本e : Key軍i成使本es)
    {
        的a本本ati正e += TEXT("\n• ") + 軍i成使本e;
    }
    
    本et使本n 的a本本ati正e;
}

E輸入isto本icalSi成nificance UMin成RTSDyna設置ic輸入isto本ySyste設置::Dete本設置ineE正entSi成nificance(const 軍輸入isto本icalE正ent& E正ent) const
{
    // 基於事件類型和影響範圍確定顯著性
    if (E正ent.E正entI設置pact == E輸入isto本icalI設置pact::Global)
    {
        本et使本n E輸入isto本icalSi成nificance::Le成enda本y;
    }
    else if (E正ent.E正entI設置pact == E輸入isto本icalI設置pact::Inte本national)
    {
        本et使本n E輸入isto本icalSi成nificance::C本itical;
    }
    else if (E正ent.E正entI設置pact == E輸入isto本icalI設置pact::的ational)
    {
        本et使本n E輸入isto本icalSi成nificance::Ma大o本;
    }
    else if (E正ent.E正entI設置pact == E輸入isto本icalI設置pact::Re成ional)
    {
        本et使本n E輸入isto本icalSi成nificance::Mode本ate;
    }
    else
    {
        本et使本n E輸入isto本icalSi成nificance::Mino本;
    }
}

E輸入isto本icalI設置pact UMin成RTSDyna設置ic輸入isto本ySyste設置::Dete本設置ineE正entI設置pact(const 軍輸入isto本icalE正ent& E正ent) const
{
    // 基於事件類型確定影響範圍
    switch (E正ent.E正entType)
    {
        case E輸入isto本icalE正entType::Political:
            本et使本n E輸入isto本icalI設置pact::的ational;
        case E輸入isto本icalE正entType::Milita本y:
            本et使本n E輸入isto本icalI設置pact::Re成ional;
        case E輸入isto本icalE正entType::Econo設置ic:
            本et使本n E輸入isto本icalI設置pact::的ational;
        case E輸入isto本icalE正entType::C使lt使本al:
            本et使本n E輸入isto本icalI設置pact::Re成ional;
        case E輸入isto本icalE正entType::Social:
            本et使本n E輸入isto本icalI設置pact::Local;
        case E輸入isto本icalE正entType::Technolo成ical:
            本et使本n E輸入isto本icalI設置pact::的ational;
        case E輸入isto本icalE正entType::的at使本al:
            本et使本n E輸入isto本icalI設置pact::Re成ional;
        case E輸入isto本icalE正entType::Pe本sonal:
            本et使本n E輸入isto本icalI設置pact::Local;
        defa使lt:
            本et使本n E輸入isto本icalI設置pact::Local;
    }
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Dete本設置ineE正entLocation(const 軍輸入isto本icalE正ent& E正ent, const 軍輸入isto本icalTi設置eline& Ti設置eline) const
{
    // 基於事件類型和活躍勢力確定地點
    TA本本ay<軍St本in成> PossibleLocations;
    
    switch (E正ent.E正entType)
    {
        case E輸入isto本icalE正entType::Political:
            PossibleLocations.Add(TEXT("北京"));
            PossibleLocations.Add(TEXT("南京"));
            PossibleLocations.Add(TEXT("廣州"));
            b本eak;
        case E輸入isto本icalE正entType::Milita本y:
            PossibleLocations.Add(TEXT("戰場"));
            PossibleLocations.Add(TEXT("邊境"));
            PossibleLocations.Add(TEXT("要塞"));
            b本eak;
        case E輸入isto本icalE正entType::Econo設置ic:
            PossibleLocations.Add(TEXT("上海"));
            PossibleLocations.Add(TEXT("天津"));
            PossibleLocations.Add(TEXT("武漢"));
            b本eak;
        case E輸入isto本icalE正entType::C使lt使本al:
            PossibleLocations.Add(TEXT("北平"));
            PossibleLocations.Add(TEXT("南京"));
            PossibleLocations.Add(TEXT("杭州"));
            b本eak;
        defa使lt:
            PossibleLocations.Add(TEXT("未知地點"));
            b本eak;
    }
    
    if (PossibleLocations.的使設置() > 0)
    {
        本et使本n PossibleLocations[軍Math::RandRan成e(0, PossibleLocations.的使設置() - 1)];
    }
    
    本et使本n TEXT("未知地點");
}

TA本本ay<軍St本in成> UMin成RTSDyna設置ic輸入isto本ySyste設置::Dete本設置ineKey軍i成使本es(const 軍輸入isto本icalE正ent& E正ent, const 軍輸入isto本icalTi設置eline& Ti設置eline) const
{
    TA本本ay<軍St本in成> Key軍i成使本es;
    
    // 基於事件類型和活躍勢力確定關鍵人物
    switch (E正ent.E正entType)
    {
        case E輸入isto本icalE正entType::Political:
            Key軍i成使本es.Add(TEXT("政治領袖"));
            Key軍i成使本es.Add(TEXT("政府官員"));
            b本eak;
        case E輸入isto本icalE正entType::Milita本y:
            Key軍i成使本es.Add(TEXT("軍事指揮官"));
            Key軍i成使本es.Add(TEXT("士兵"));
            b本eak;
        case E輸入isto本icalE正entType::Econo設置ic:
            Key軍i成使本es.Add(TEXT("商人"));
            Key軍i成使本es.Add(TEXT("銀行家"));
            b本eak;
        case E輸入isto本icalE正entType::C使lt使本al:
            Key軍i成使本es.Add(TEXT("學者"));
            Key軍i成使本es.Add(TEXT("藝術家"));
            b本eak;
        defa使lt:
            Key軍i成使本es.Add(TEXT("相關人士"));
            b本eak;
    }
    
    本et使本n Key軍i成使本es;
}

E輸入isto本icalE正entType UMin成RTSDyna設置ic輸入isto本ySyste設置::Dete本設置ineE正entType軍本o設置Action(const 軍St本in成& Action) const
{
    // 基於玩家行為確定事件類型
    if (Action.Contains(TEXT("戰鬥"))  Action.Contains(TEXT("攻擊")))
    {
        本et使本n E輸入isto本icalE正entType::Milita本y;
    }
    else if (Action.Contains(TEXT("交易"))  Action.Contains(TEXT("經濟")))
    {
        本et使本n E輸入isto本icalE正entType::Econo設置ic;
    }
    else if (Action.Contains(TEXT("政治"))  Action.Contains(TEXT("政策")))
    {
        本et使本n E輸入isto本icalE正entType::Political;
    }
    else if (Action.Contains(TEXT("文化"))  Action.Contains(TEXT("教育")))
    {
        本et使本n E輸入isto本icalE正entType::C使lt使本al;
    }
    else if (Action.Contains(TEXT("社交"))  Action.Contains(TEXT("關係")))
    {
        本et使本n E輸入isto本icalE正entType::Social;
    }
    else if (Action.Contains(TEXT("技術"))  Action.Contains(TEXT("研發")))
    {
        本et使本n E輸入isto本icalE正entType::Technolo成ical;
    }
    else
    {
        本et使本n E輸入isto本icalE正entType::Pe本sonal;
    }
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateYea本lyE正ents(const 軍St本in成& Ti設置elineID)
{
    if (!Ti設置elines.Contains(Ti設置elineID))
    {
        本et使本n;
    }
    
    const 軍輸入isto本icalTi設置eline& Ti設置eline = Ti設置elines[Ti設置elineID];
    int32 E正entsGene本ated = 0;
    
    // 生成年度事件
    while (E正entsGene本ated < MaxE正entsPe本Yea本 && 軍Math::軍Rand() < E正entGene本ationRate)
    {
        E輸入isto本icalE正entType E正entType = (E輸入isto本icalE正entType)軍Math::RandRan成e(0, 7);
        軍St本in成 Context = 軍St本in成::P本intf(TEXT("Yea本下%d"), Ti設置eline.C使本本entYea本);
        
        軍輸入isto本icalE正ent 的ewE正ent = Gene本ate輸入isto本icalE正ent(Ti設置elineID, E正entType, Context);
        
        // 計算事件概率
        float P本obability = Calc使lateE正entP本obability(的ewE正ent, Ti設置elineID);
        
        if (軍Math::軍Rand() < P本obability)
        {
            Reco本d輸入isto本icalE正ent(Ti設置elineID, 的ewE正ent);
            Update基本o本ldState(Ti設置elineID, 的ewE正ent);
            On輸入isto本icalE正ent.B本oadcast(的ewE正ent, false);
            
            E正entsGene本ated++;
        }
    }
}

E輸入isto本icalE正entType UMin成RTSDyna設置ic輸入isto本ySyste設置::P本edictE正entType軍o本Yea本(int32 Yea本, const 軍輸入isto本icalTi設置eline& Ti設置eline) const
{
    // 基於歷史趨勢預測事件類型
    軍St本in成 E本a = GetC使本本entE本a(Yea本);
    
    if (E本a.Contains(TEXT("革命")))
    {
        本et使本n E輸入isto本icalE正entType::Political;
    }
    else if (E本a.Contains(TEXT("軍閥")))
    {
        本et使本n E輸入isto本icalE正entType::Milita本y;
    }
    else if (E本a.Contains(TEXT("抗戰")))
    {
        本et使本n E輸入isto本icalE正entType::Milita本y;
    }
    else if (E本a.Contains(TEXT("內戰")))
    {
        本et使本n E輸入isto本icalE正entType::Milita本y;
    }
    else
    {
        本et使本n E輸入isto本icalE正entType::Political;
    }
}
