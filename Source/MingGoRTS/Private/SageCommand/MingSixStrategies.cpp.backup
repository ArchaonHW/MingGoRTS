#incl使de "Sa成eCo設置設置and/Min成SixSt本ate成ies.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成SixSt本ate成ies::UMin成SixSt本ate成ies()
{
    // 初始化策略狀態
    St本ate成yStates.Add(ESixSt本ate成yType::輸入ea正enSt本ate成y, ESt本ate成yState::Plannin成);
    St本ate成yStates.Add(ESixSt本ate成yType::Ea本thSt本ate成y, ESt本ate成yState::Plannin成);
    St本ate成yStates.Add(ESixSt本ate成yType::輸入使設置anSt本ate成y, ESt本ate成yState::Plannin成);
    St本ate成yStates.Add(ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y, ESt本ate成yState::Plannin成);
    St本ate成yStates.Add(ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y, ESt本ate成yState::Plannin成);
    St本ate成yStates.Add(ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y, ESt本ate成yState::Plannin成);
}

bool UMin成SixSt本ate成ies::Initialize()
{
    if (bSyste設置Acti正e)
    {
        本et使本n t本使e;
    }

    // 初始化系統狀態
    bSyste設置Acti正e = t本使e;
    Syste設置Stability = 100.0f;

    // 設置執行更新定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Exec使tionUpdateTi設置e本,
            this,
            &UMin成SixSt本ate成ies::UpdateSt本ate成yExec使tion,
            0.1f,
            t本使e
        );
    }

    本et使本n t本使e;
}

正oid UMin成SixSt本ate成ies::Clean使p()
{
    bSyste設置Acti正e = false;
    
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Exec使tionUpdateTi設置e本);
    }
    
    St本ate成yStates.E設置pty();
    Exec使tionPlans.E設置pty();
    St本ate成y輸入isto本y.E設置pty();
    Acti正eSt本ate成ies.E設置pty();
}

軍St本ate成yE正al使ation UMin成SixSt本ate成ies::E正al使ateSt本ate成y(ESixSt本ate成yType St本ate成yType, const 軍St本in成& Context)
{
    軍St本ate成yE正al使ation E正al使ation;
    E正al使ation.St本ate成yType = St本ate成yType;
    
    // 評估策略的各個維度
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::輸入ea正enSt本ate成y:
        E正al使ation.S使ccessP本obability = E正al使ate輸入ea正enSt本ate成y(Context);
        b本eak;
    case ESixSt本ate成yType::Ea本thSt本ate成y:
        E正al使ation.S使ccessP本obability = E正al使ateEa本thSt本ate成y(Context);
        b本eak;
    case ESixSt本ate成yType::輸入使設置anSt本ate成y:
        E正al使ation.S使ccessP本obability = E正al使ate輸入使設置anSt本ate成y(Context);
        b本eak;
    case ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y:
        E正al使ation.S使ccessP本obability = E正al使ate輸入ea正enEa本thSt本ate成y(Context);
        b本eak;
    case ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y:
        E正al使ation.S使ccessP本obability = E正al使ate輸入ea正en輸入使設置anSt本ate成y(Context);
        b本eak;
    case ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y:
        E正al使ation.S使ccessP本obability = E正al使ateEa本th輸入使設置anSt本ate成y(Context);
        b本eak;
    }
    
    // 計算其他評估指標
    E正al使ation.RiskLe正el = Calc使lateSt本ate成yRisk(St本ate成yType, Context);
    E正al使ation.Reso使本ceCost = Calc使lateReso使本ceReq使i本e設置ents(St本ate成yType);
    E正al使ation.Ti設置eReq使i本ed = Calc使lateTi設置eReq使i本e設置ents(St本ate成yType);
    E正al使ation.St本ate成icVal使e = E正al使ation.S使ccessP本obability * (100.0f - E正al使ation.RiskLe正el) / 100.0f;
    
    // 生成建議
    if (E正al使ation.S使ccessP本obability > 70.0f && E正al使ation.RiskLe正el < 30.0f)
    {
        E正al使ation.Reco設置設置endation = TEXT("推薦執行：成功概率高，風險較低");
    }
    else if (E正al使ation.S使ccessP本obability > 50.0f)
    {
        E正al使ation.Reco設置設置endation = TEXT("可考慮執行：需要謹慎評估風險");
    }
    else
    {
        E正al使ation.Reco設置設置endation = TEXT("不推薦執行：成功概率較低，風險較高");
    }
    
    // 觸發事件
    OnSt本ate成yE正al使ated.B本oadcast(E正al使ation);
    
    本et使本n E正al使ation;
}

TA本本ay<軍St本ate成yE正al使ation> UMin成SixSt本ate成ies::E正al使ateAllSt本ate成ies(const 軍St本in成& Context)
{
    TA本本ay<軍St本ate成yE正al使ation> E正al使ations;
    
    // 評估所有六種策略
    E正al使ations.Add(E正al使ateSt本ate成y(ESixSt本ate成yType::輸入ea正enSt本ate成y, Context));
    E正al使ations.Add(E正al使ateSt本ate成y(ESixSt本ate成yType::Ea本thSt本ate成y, Context));
    E正al使ations.Add(E正al使ateSt本ate成y(ESixSt本ate成yType::輸入使設置anSt本ate成y, Context));
    E正al使ations.Add(E正al使ateSt本ate成y(ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y, Context));
    E正al使ations.Add(E正al使ateSt本ate成y(ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y, Context));
    E正al使ations.Add(E正al使ateSt本ate成y(ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y, Context));
    
    本et使本n E正al使ations;
}

ESixSt本ate成yType UMin成SixSt本ate成ies::GetOpti設置alSt本ate成y(const 軍St本in成& Context)
{
    TA本本ay<軍St本ate成yE正al使ation> E正al使ations = E正al使ateAllSt本ate成ies(Context);
    
    ESixSt本ate成yType Opti設置alSt本ate成y = ESixSt本ate成yType::輸入ea正enSt本ate成y;
    float MaxSt本ate成icVal使e = 0.0f;
    
    fo本 (const 軍St本ate成yE正al使ation& E正al使ation : E正al使ations)
    {
        if (E正al使ation.St本ate成icVal使e > MaxSt本ate成icVal使e)
        {
            MaxSt本ate成icVal使e = E正al使ation.St本ate成icVal使e;
            Opti設置alSt本ate成y = E正al使ation.St本ate成yType;
        }
    }
    
    本et使本n Opti設置alSt本ate成y;
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::GetSt本ate成yReco設置設置endations(const 軍St本in成& Context)
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    TA本本ay<軍St本ate成yE正al使ation> E正al使ations = E正al使ateAllSt本ate成ies(Context);
    
    // 按戰略價值排序
    E正al使ations.So本t([](const 軍St本ate成yE正al使ation& A, const 軍St本ate成yE正al使ation& B)
    {
        本et使本n A.St本ate成icVal使e > B.St本ate成icVal使e;
    });
    
    // 生成建議
    fo本 (int32 i = 0; i < E正al使ations.的使設置() && i < 3; ++i)
    {
        const 軍St本ate成yE正al使ation& E正al使ation = E正al使ations[i];
        Reco設置設置endations.Add(軍St本in成::P本intf(TEXT("%d. %s - %s"), 
            i + 1, *GetSt本ate成y的a設置e(E正al使ation.St本ate成yType), *E正al使ation.Reco設置設置endation));
    }
    
    本et使本n Reco設置設置endations;
}

bool UMin成SixSt本ate成ies::Exec使teSt本ate成y(ESixSt本ate成yType St本ate成yType, const 軍St本in成& Context)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    // 檢查策略是否已經在執行中
    if (Acti正eSt本ate成ies.Contains(St本ate成yType))
    {
        本et使本n false;
    }
    
    // 評估策略
    軍St本ate成yE正al使ation E正al使ation = E正al使ateSt本ate成y(St本ate成yType, Context);
    
    // 如果成功概率太低，拒絕執行
    if (E正al使ation.S使ccessP本obability < 30.0f)
    {
        本et使本n false;
    }
    
    // 創建執行計劃
    軍St本ate成yExec使tionPlan Plan = C本eateExec使tionPlan(St本ate成yType, Context);
    
    // 開始執行
    本et使本n Sta本tSt本ate成yExec使tion(Plan);
}

軍St本ate成yExec使tionPlan UMin成SixSt本ate成ies::C本eateExec使tionPlan(ESixSt本ate成yType St本ate成yType, const 軍St本in成& Context)
{
    軍St本ate成yExec使tionPlan Plan;
    Plan.St本ate成yType = St本ate成yType;
    Plan.C使本本entState = ESt本ate成yState::P本epa本in成;
    Plan.P本o成本essPe本centa成e = 0.0f;
    
    // 根據策略類型生成執行步驟
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::輸入ea正enSt本ate成y:
        Plan.Exec使tionSteps.Add(TEXT("分析天時變化"));
        Plan.Exec使tionSteps.Add(TEXT("選擇最佳時機"));
        Plan.Exec使tionSteps.Add(TEXT("利用自然現象"));
        Plan.Exec使tionSteps.Add(TEXT("執行天道策略"));
        b本eak;
    case ESixSt本ate成yType::Ea本thSt本ate成y:
        Plan.Exec使tionSteps.Add(TEXT("勘察地形地貌"));
        Plan.Exec使tionSteps.Add(TEXT("佔據有利位置"));
        Plan.Exec使tionSteps.Add(TEXT("設置防禦工事"));
        Plan.Exec使tionSteps.Add(TEXT("執行地道策略"));
        b本eak;
    case ESixSt本ate成yType::輸入使設置anSt本ate成y:
        Plan.Exec使tionSteps.Add(TEXT("分析敵我人心"));
        Plan.Exec使tionSteps.Add(TEXT("制定心理戰術"));
        Plan.Exec使tionSteps.Add(TEXT("分化敵方內部"));
        Plan.Exec使tionSteps.Add(TEXT("執行人道策略"));
        b本eak;
    case ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y:
        Plan.Exec使tionSteps.Add(TEXT("分析天時地利"));
        Plan.Exec使tionSteps.Add(TEXT("協調天地因素"));
        Plan.Exec使tionSteps.Add(TEXT("制定綜合方案"));
        Plan.Exec使tionSteps.Add(TEXT("執行天地策略"));
        b本eak;
    case ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y:
        Plan.Exec使tionSteps.Add(TEXT("分析天時人心"));
        Plan.Exec使tionSteps.Add(TEXT("協調天人關係"));
        Plan.Exec使tionSteps.Add(TEXT("制定精神戰術"));
        Plan.Exec使tionSteps.Add(TEXT("執行天人策略"));
        b本eak;
    case ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y:
        Plan.Exec使tionSteps.Add(TEXT("分析地利人心"));
        Plan.Exec使tionSteps.Add(TEXT("協調地人關係"));
        Plan.Exec使tionSteps.Add(TEXT("制定持久戰術"));
        Plan.Exec使tionSteps.Add(TEXT("執行地人策略"));
        b本eak;
    }
    
    // 計算所需資源
    Plan.Req使i本edReso使本ces.Add(TEXT("兵力"));
    Plan.Req使i本edReso使本ces.Add(TEXT("物資"));
    Plan.Req使i本edReso使本ces.Add(TEXT("時間"));
    
    // 識別風險因素
    Plan.Risk軍acto本s.Add(TEXT("敵方反制"));
    Plan.Risk軍acto本s.Add(TEXT("環境變化"));
    Plan.Risk軍acto本s.Add(TEXT("內部不穩"));
    
    // 計算預計持續時間
    Plan.Esti設置atedD使本ation = Calc使lateTi設置eReq使i本e設置ents(St本ate成yType);
    
    本et使本n Plan;
}

bool UMin成SixSt本ate成ies::Sta本tSt本ate成yExec使tion(const 軍St本ate成yExec使tionPlan& Plan)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n false;
    }
    
    ESixSt本ate成yType St本ate成yType = Plan.St本ate成yType;
    
    // 保存執行計劃
    Exec使tionPlans.Add(St本ate成yType, Plan);
    
    // 設置策略狀態
    St本ate成yStates[St本ate成yType] = ESt本ate成yState::Exec使tin成;
    
    // 添加到活動策略列表
    Acti正eSt本ate成ies.Add(St本ate成yType);
    
    // 記錄事件
    Reco本dSt本ate成yE正ent(軍St本in成::P本intf(TEXT("開始執行策略：%s"), *GetSt本ate成y的a設置e(St本ate成yType)), 
                       St本ate成yType, ESt本ate成yState::Exec使tin成, 50.0f);
    
    // 觸發事件
    OnSt本ate成yExec使tionSta本ted.B本oadcast(Plan);
    
    本et使本n t本使e;
}

bool UMin成SixSt本ate成ies::Pa使seSt本ate成yExec使tion(ESixSt本ate成yType St本ate成yType)
{
    if (!Acti正eSt本ate成ies.Contains(St本ate成yType))
    {
        本et使本n false;
    }
    
    St本ate成yStates[St本ate成yType] = ESt本ate成yState::Plannin成;
    
    // 記錄事件
    Reco本dSt本ate成yE正ent(軍St本in成::P本intf(TEXT("暫停執行策略：%s"), *GetSt本ate成y的a設置e(St本ate成yType)), 
                       St本ate成yType, ESt本ate成yState::Plannin成, 25.0f);
    
    本et使本n t本使e;
}

bool UMin成SixSt本ate成ies::Res使設置eSt本ate成yExec使tion(ESixSt本ate成yType St本ate成yType)
{
    if (!Acti正eSt本ate成ies.Contains(St本ate成yType))
    {
        本et使本n false;
    }
    
    St本ate成yStates[St本ate成yType] = ESt本ate成yState::Exec使tin成;
    
    // 記錄事件
    Reco本dSt本ate成yE正ent(軍St本in成::P本intf(TEXT("恢復執行策略：%s"), *GetSt本ate成y的a設置e(St本ate成yType)), 
                       St本ate成yType, ESt本ate成yState::Exec使tin成, 35.0f);
    
    本et使本n t本使e;
}

bool UMin成SixSt本ate成ies::CancelSt本ate成yExec使tion(ESixSt本ate成yType St本ate成yType)
{
    if (!Acti正eSt本ate成ies.Contains(St本ate成yType))
    {
        本et使本n false;
    }
    
    St本ate成yStates[St本ate成yType] = ESt本ate成yState::軍ailed;
    Acti正eSt本ate成ies.Re設置o正e(St本ate成yType);
    Exec使tionPlans.Re設置o正e(St本ate成yType);
    
    // 記錄事件
    Reco本dSt本ate成yE正ent(軍St本in成::P本intf(TEXT("取消執行策略：%s"), *GetSt本ate成y的a設置e(St本ate成yType)), 
                       St本ate成yType, ESt本ate成yState::軍ailed, 20.0f);
    
    本et使本n t本使e;
}

ESt本ate成yState UMin成SixSt本ate成ies::GetSt本ate成yState(ESixSt本ate成yType St本ate成yType) const
{
    if (St本ate成yStates.Contains(St本ate成yType))
    {
        本et使本n St本ate成yStates[St本ate成yType];
    }
    本et使本n ESt本ate成yState::Plannin成;
}

float UMin成SixSt本ate成ies::GetSt本ate成yP本o成本ess(ESixSt本ate成yType St本ate成yType) const
{
    if (Exec使tionPlans.Contains(St本ate成yType))
    {
        本et使本n Exec使tionPlans[St本ate成yType].P本o成本essPe本centa成e;
    }
    本et使本n 0.0f;
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::GetActi正eSt本ate成ies() const
{
    TA本本ay<軍St本in成> Acti正eSt本ate成y的a設置es;
    
    fo本 (ESixSt本ate成yType St本ate成yType : Acti正eSt本ate成ies)
    {
        Acti正eSt本ate成y的a設置es.Add(GetSt本ate成y的a設置e(St本ate成yType));
    }
    
    本et使本n Acti正eSt本ate成y的a設置es;
}

bool UMin成SixSt本ate成ies::IsSt本ate成yExec使tin成(ESixSt本ate成yType St本ate成yType) const
{
    本et使本n Acti正eSt本ate成ies.Contains(St本ate成yType) && 
           St本ate成yStates.Contains(St本ate成yType) && 
           St本ate成yStates[St本ate成yType] == ESt本ate成yState::Exec使tin成;
}

TA本本ay<ESixSt本ate成yType> UMin成SixSt本ate成ies::GetCo設置patibleSt本ate成ies(ESixSt本ate成yType St本ate成yType) const
{
    本et使本n GetSyne本成isticSt本ate成ies(St本ate成yType);
}

TA本本ay<ESixSt本ate成yType> UMin成SixSt本ate成ies::GetConflictin成St本ate成ies(ESixSt本ate成yType St本ate成yType) const
{
    本et使本n GetAnta成onisticSt本ate成ies(St本ate成yType);
}

float UMin成SixSt本ate成ies::Calc使lateSt本ate成ySyne本成y(ESixSt本ate成yType St本ate成y1, ESixSt本ate成yType St本ate成y2) const
{
    本et使本n Calc使lateCo設置patibilitySco本e(St本ate成y1, St本ate成y2);
}

ESt本ate成yCo設置plexity UMin成SixSt本ate成ies::GetSt本ate成yCo設置plexity(ESixSt本ate成yType St本ate成yType) const
{
    本et使本n Dete本設置ineSt本ate成yCo設置plexity(St本ate成yType);
}

float UMin成SixSt本ate成ies::Calc使lateSt本ate成icAd正anta成e(ESixSt本ate成yType St本ate成yType) const
{
    // 基於策略複雜度和當前狀態計算戰略優勢
    ESt本ate成yCo設置plexity Co設置plexity = GetSt本ate成yCo設置plexity(St本ate成yType);
    float Co設置plexityBon使s = 0.0f;
    
    switch (Co設置plexity)
    {
    case ESt本ate成yCo設置plexity::Si設置ple:
        Co設置plexityBon使s = 10.0f;
        b本eak;
    case ESt本ate成yCo設置plexity::Mode本ate:
        Co設置plexityBon使s = 25.0f;
        b本eak;
    case ESt本ate成yCo設置plexity::Co設置plex:
        Co設置plexityBon使s = 40.0f;
        b本eak;
    case ESt本ate成yCo設置plexity::Maste本:
        Co設置plexityBon使s = 60.0f;
        b本eak;
    }
    
    本et使本n 50.0f + Co設置plexityBon使s; // 基礎50分 + 複雜度加成
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::GetSt本ate成yEffects(ESixSt本ate成yType St本ate成yType) const
{
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::輸入ea正enSt本ate成y:
        本et使本n Get輸入ea正enSt本ate成yEffects();
    case ESixSt本ate成yType::Ea本thSt本ate成y:
        本et使本n GetEa本thSt本ate成yEffects();
    case ESixSt本ate成yType::輸入使設置anSt本ate成y:
        本et使本n Get輸入使設置anSt本ate成yEffects();
    case ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y:
        本et使本n Get輸入ea正enEa本thSt本ate成yEffects();
    case ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y:
        本et使本n Get輸入ea正en輸入使設置anSt本ate成yEffects();
    case ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y:
        本et使本n GetEa本th輸入使設置anSt本ate成yEffects();
    defa使lt:
        本et使本n TA本本ay<軍St本in成>();
    }
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::GetSt本ate成yReq使i本e設置ents(ESixSt本ate成yType St本ate成yType) const
{
    TA本本ay<軍St本in成> Req使i本e設置ents;
    
    Req使i本e設置ents.Add(TEXT("足夠的兵力"));
    Req使i本e設置ents.Add(TEXT("充分的物資"));
    Req使i本e設置ents.Add(TEXT("適當的時機"));
    
    // 根據策略類型添加特殊要求
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::輸入ea正enSt本ate成y:
        Req使i本e設置ents.Add(TEXT("有利的天時"));
        b本eak;
    case ESixSt本ate成yType::Ea本thSt本ate成y:
        Req使i本e設置ents.Add(TEXT("有利的地形"));
        b本eak;
    case ESixSt本ate成yType::輸入使設置anSt本ate成y:
        Req使i本e設置ents.Add(TEXT("深入了解敵我人心"));
        b本eak;
    case ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y:
        Req使i本e設置ents.Add(TEXT("天時地利的協調"));
        b本eak;
    case ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y:
        Req使i本e設置ents.Add(TEXT("天人合一的境界"));
        b本eak;
    case ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y:
        Req使i本e設置ents.Add(TEXT("地人協調的能力"));
        b本eak;
    }
    
    本et使本n Req使i本e設置ents;
}

TA本本ay<軍SixSt本ate成yE正ent> UMin成SixSt本ate成ies::GetSt本ate成y輸入isto本y() const
{
    本et使本n St本ate成y輸入isto本y;
}

軍SixSt本ate成yE正ent UMin成SixSt本ate成ies::GetLastSt本ate成yE正ent() const
{
    if (St本ate成y輸入isto本y.的使設置() > 0)
    {
        本et使本n St本ate成y輸入isto本y.Last();
    }
    本et使本n 軍SixSt本ate成yE正ent();
}

正oid UMin成SixSt本ate成ies::Clea本St本ate成y輸入isto本y()
{
    St本ate成y輸入isto本y.E設置pty();
}

// 私有方法實現

float UMin成SixSt本ate成ies::E正al使ate輸入ea正enSt本ate成y(const 軍St本in成& Context)
{
    float BaseSco本e = 50.0f;
    
    // 檢查上下文中的天時因素
    if (Context.Contains("天時")  Context.Contains("天氣")  Context.Contains("季節"))
    {
        BaseSco本e += 20.0f;
    }
    
    if (Context.Contains("夜晚")  Context.Contains("月光"))
    {
        BaseSco本e += 15.0f;
    }
    
    if (Context.Contains("風雨")  Context.Contains("雷電"))
    {
        BaseSco本e += 10.0f;
    }
    
    本et使本n 軍Math::Cla設置p(BaseSco本e, 0.0f, 100.0f);
}

float UMin成SixSt本ate成ies::E正al使ateEa本thSt本ate成y(const 軍St本in成& Context)
{
    float BaseSco本e = 50.0f;
    
    // 檢查上下文中的地利因素
    if (Context.Contains("地形")  Context.Contains("地貌")  Context.Contains("位置"))
    {
        BaseSco本e += 20.0f;
    }
    
    if (Context.Contains("高地")  Context.Contains("要塞"))
    {
        BaseSco本e += 15.0f;
    }
    
    if (Context.Contains("防禦")  Context.Contains("守護"))
    {
        BaseSco本e += 10.0f;
    }
    
    本et使本n 軍Math::Cla設置p(BaseSco本e, 0.0f, 100.0f);
}

float UMin成SixSt本ate成ies::E正al使ate輸入使設置anSt本ate成y(const 軍St本in成& Context)
{
    float BaseSco本e = 50.0f;
    
    // 檢查上下文中的人和因素
    if (Context.Contains("人心")  Context.Contains("士氣")  Context.Contains("忠誠"))
    {
        BaseSco本e += 20.0f;
    }
    
    if (Context.Contains("心理")  Context.Contains("情感"))
    {
        BaseSco本e += 15.0f;
    }
    
    if (Context.Contains("分化")  Context.Contains("離間"))
    {
        BaseSco本e += 10.0f;
    }
    
    本et使本n 軍Math::Cla設置p(BaseSco本e, 0.0f, 100.0f);
}

float UMin成SixSt本ate成ies::E正al使ate輸入ea正enEa本thSt本ate成y(const 軍St本in成& Context)
{
    // 天地策略是天道和地道策略的結合
    float 輸入ea正enSco本e = E正al使ate輸入ea正enSt本ate成y(Context);
    float Ea本thSco本e = E正al使ateEa本thSt本ate成y(Context);
    
    本et使本n (輸入ea正enSco本e + Ea本thSco本e) / 2.0f + 10.0f; // 結合策略有額外加成
}

float UMin成SixSt本ate成ies::E正al使ate輸入ea正en輸入使設置anSt本ate成y(const 軍St本in成& Context)
{
    // 天人策略是天道和人道策略的結合
    float 輸入ea正enSco本e = E正al使ate輸入ea正enSt本ate成y(Context);
    float 輸入使設置anSco本e = E正al使ate輸入使設置anSt本ate成y(Context);
    
    本et使本n (輸入ea正enSco本e + 輸入使設置anSco本e) / 2.0f + 10.0f; // 結合策略有額外加成
}

float UMin成SixSt本ate成ies::E正al使ateEa本th輸入使設置anSt本ate成y(const 軍St本in成& Context)
{
    // 地人策略是地道和人道策略的結合
    float Ea本thSco本e = E正al使ateEa本thSt本ate成y(Context);
    float 輸入使設置anSco本e = E正al使ate輸入使設置anSt本ate成y(Context);
    
    本et使本n (Ea本thSco本e + 輸入使設置anSco本e) / 2.0f + 10.0f; // 結合策略有額外加成
}

float UMin成SixSt本ate成ies::Calc使lateSt本ate成yRisk(ESixSt本ate成yType St本ate成yType, const 軍St本in成& Context)
{
    float BaseRisk = 30.0f; // 基礎風險
    
    // 根據策略複雜度調整風險
    ESt本ate成yCo設置plexity Co設置plexity = Dete本設置ineSt本ate成yCo設置plexity(St本ate成yType);
    switch (Co設置plexity)
    {
    case ESt本ate成yCo設置plexity::Si設置ple:
        BaseRisk -= 10.0f;
        b本eak;
    case ESt本ate成yCo設置plexity::Mode本ate:
        BaseRisk += 0.0f;
        b本eak;
    case ESt本ate成yCo設置plexity::Co設置plex:
        BaseRisk += 15.0f;
        b本eak;
    case ESt本ate成yCo設置plexity::Maste本:
        BaseRisk += 30.0f;
        b本eak;
    }
    
    // 根據上下文調整風險
    if (Context.Contains("危險")  Context.Contains("風險"))
    {
        BaseRisk += 20.0f;
    }
    
    if (Context.Contains("安全")  Context.Contains("穩定"))
    {
        BaseRisk -= 15.0f;
    }
    
    本et使本n 軍Math::Cla設置p(BaseRisk, 0.0f, 100.0f);
}

float UMin成SixSt本ate成ies::Calc使lateReso使本ceReq使i本e設置ents(ESixSt本ate成yType St本ate成yType)
{
    // 根據策略類型計算資源需求
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::輸入ea正enSt本ate成y:
        本et使本n 40.0f; // 主要依賴時機，資源需求較低
    case ESixSt本ate成yType::Ea本thSt本ate成y:
        本et使本n 60.0f; // 需要佔據地利，資源需求中等
    case ESixSt本ate成yType::輸入使設置anSt本ate成y:
        本et使本n 50.0f; // 主要依賴心理戰，資源需求中等
    case ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y:
        本et使本n 70.0f; // 天地結合，資源需求較高
    case ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y:
        本et使本n 65.0f; // 天人結合，資源需求較高
    case ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y:
        本et使本n 75.0f; // 地人結合，資源需求最高
    defa使lt:
        本et使本n 50.0f;
    }
}

float UMin成SixSt本ate成ies::Calc使lateTi設置eReq使i本e設置ents(ESixSt本ate成yType St本ate成yType)
{
    // 根據策略類型計算時間需求（返回分鐘數）
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::輸入ea正enSt本ate成y:
        本et使本n 30.0f; // 依賴時機，時間較短
    case ESixSt本ate成yType::Ea本thSt本ate成y:
        本et使本n 60.0f; // 需要佔據地利，時間中等
    case ESixSt本ate成yType::輸入使設置anSt本ate成y:
        本et使本n 45.0f; // 心理戰需要時間，時間中等
    case ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y:
        本et使本n 75.0f; // 天地結合，時間較長
    case ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y:
        本et使本n 90.0f; // 天人結合，時間較長
    case ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y:
        本et使本n 120.0f; // 地人結合，時間最長
    defa使lt:
        本et使本n 60.0f;
    }
}

正oid UMin成SixSt本ate成ies::UpdateSt本ate成yExec使tion(float DeltaTi設置e)
{
    if (!bSyste設置Acti正e)
    {
        本et使本n;
    }
    
    // 更新所有執行中的策略
    fo本 (ESixSt本ate成yType St本ate成yType : Acti正eSt本ate成ies)
    {
        if (St本ate成yStates[St本ate成yType] == ESt本ate成yState::Exec使tin成)
        {
            UpdateSt本ate成yP本o成本ess(St本ate成yType, DeltaTi設置e);
            CheckSt本ate成yCo設置pletion(St本ate成yType);
        }
    }
}

正oid UMin成SixSt本ate成ies::UpdateSt本ate成yP本o成本ess(ESixSt本ate成yType St本ate成yType, float DeltaTi設置e)
{
    if (!Exec使tionPlans.Contains(St本ate成yType))
    {
        本et使本n;
    }
    
    軍St本ate成yExec使tionPlan& Plan = Exec使tionPlans[St本ate成yType];
    
    // 更新進度
    float P本o成本essInc本e設置ent = (St本ate成yExec使tionSpeed * DeltaTi設置e / Plan.Esti設置atedD使本ation) * 100.0f;
    Plan.P本o成本essPe本centa成e = 軍Math::Cla設置p(Plan.P本o成本essPe本centa成e + P本o成本essInc本e設置ent, 0.0f, 100.0f);
}

正oid UMin成SixSt本ate成ies::CheckSt本ate成yCo設置pletion(ESixSt本ate成yType St本ate成yType)
{
    if (!Exec使tionPlans.Contains(St本ate成yType))
    {
        本et使本n;
    }
    
    const 軍St本ate成yExec使tionPlan& Plan = Exec使tionPlans[St本ate成yType];
    
    if (Plan.P本o成本essPe本centa成e >= 100.0f)
    {
        // 策略執行完成
        St本ate成yStates[St本ate成yType] = ESt本ate成yState::Co設置pleted;
        Acti正eSt本ate成ies.Re設置o正e(St本ate成yType);
        
        // 記錄事件
        Reco本dSt本ate成yE正ent(軍St本in成::P本intf(TEXT("策略執行完成：%s"), *GetSt本ate成y的a設置e(St本ate成yType)), 
                           St本ate成yType, ESt本ate成yState::Co設置pleted, 80.0f);
    }
}

TA本本ay<ESixSt本ate成yType> UMin成SixSt本ate成ies::GetSyne本成isticSt本ate成ies(ESixSt本ate成yType St本ate成yType) const
{
    TA本本ay<ESixSt本ate成yType> Syne本成isticSt本ate成ies;
    
    // 定義策略間的協同關係
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::輸入ea正enSt本ate成y:
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y);
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y);
        b本eak;
    case ESixSt本ate成yType::Ea本thSt本ate成y:
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y);
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y);
        b本eak;
    case ESixSt本ate成yType::輸入使設置anSt本ate成y:
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y);
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y);
        b本eak;
    case ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y:
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::輸入ea正enSt本ate成y);
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::Ea本thSt本ate成y);
        b本eak;
    case ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y:
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::輸入ea正enSt本ate成y);
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::輸入使設置anSt本ate成y);
        b本eak;
    case ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y:
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::Ea本thSt本ate成y);
        Syne本成isticSt本ate成ies.Add(ESixSt本ate成yType::輸入使設置anSt本ate成y);
        b本eak;
    }
    
    本et使本n Syne本成isticSt本ate成ies;
}

TA本本ay<ESixSt本ate成yType> UMin成SixSt本ate成ies::GetAnta成onisticSt本ate成ies(ESixSt本ate成yType St本ate成yType) const
{
    // 六策之間沒有直接的對抗關係，更多是互補
    本et使本n TA本本ay<ESixSt本ate成yType>();
}

float UMin成SixSt本ate成ies::Calc使lateCo設置patibilitySco本e(ESixSt本ate成yType St本ate成y1, ESixSt本ate成yType St本ate成y2) const
{
    // 如果是協同策略，返回高分數
    TA本本ay<ESixSt本ate成yType> Syne本成isticSt本ate成ies = GetSyne本成isticSt本ate成ies(St本ate成y1);
    if (Syne本成isticSt本ate成ies.Contains(St本ate成y2))
    {
        本et使本n 0.8f;
    }
    
    // 如果是相同策略，返回中等分數
    if (St本ate成y1 == St本ate成y2)
    {
        本et使本n 0.5f;
    }
    
    // 其他情況返回低分數
    本et使本n 0.2f;
}

正oid UMin成SixSt本ate成ies::Reco本dSt本ate成yE正ent(const 軍St本in成& Desc本iption, ESixSt本ate成yType St本ate成yType, ESt本ate成yState State, float I設置pact)
{
    軍SixSt本ate成yE正ent E正ent;
    E正ent.E正entID = 軍St本in成::P本intf(TEXT("STRATEGY下%lld"), 軍DateTi設置e::的ow().GetTicks());
    E正ent.Desc本iption = Desc本iption;
    E正ent.St本ate成yType = St本ate成yType;
    E正ent.E正entState = State;
    E正ent.I設置pactLe正el = I設置pact;
    E正ent.Ti設置esta設置p = 軍DateTi設置e::的ow();
    
    St本ate成y輸入isto本y.Add(E正ent);
    
    // 限制歷史記錄數量
    if (St本ate成y輸入isto本y.的使設置() > 1000)
    {
        St本ate成y輸入isto本y.Re設置o正eAt(0);
    }
}

軍St本in成 UMin成SixSt本ate成ies::GetSt本ate成y的a設置e(ESixSt本ate成yType St本ate成yType) const
{
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::輸入ea正enSt本ate成y: 本et使本n TEXT("天道策略");
    case ESixSt本ate成yType::Ea本thSt本ate成y: 本et使本n TEXT("地道策略");
    case ESixSt本ate成yType::輸入使設置anSt本ate成y: 本et使本n TEXT("人道策略");
    case ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y: 本et使本n TEXT("天地策略");
    case ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y: 本et使本n TEXT("天人策略");
    case ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y: 本et使本n TEXT("地人策略");
    defa使lt: 本et使本n TEXT("未知策略");
    }
}

軍St本in成 UMin成SixSt本ate成ies::GetState的a設置e(ESt本ate成yState State) const
{
    switch (State)
    {
    case ESt本ate成yState::Plannin成: 本et使本n TEXT("計劃中");
    case ESt本ate成yState::P本epa本in成: 本et使本n TEXT("準備中");
    case ESt本ate成yState::Exec使tin成: 本et使本n TEXT("執行中");
    case ESt本ate成yState::Monito本in成: 本et使本n TEXT("監控中");
    case ESt本ate成yState::Co設置pleted: 本et使本n TEXT("已完成");
    case ESt本ate成yState::軍ailed: 本et使本n TEXT("失敗");
    defa使lt: 本et使本n TEXT("未知狀態");
    }
}

軍St本in成 UMin成SixSt本ate成ies::GetCo設置plexity的a設置e(ESt本ate成yCo設置plexity Co設置plexity) const
{
    switch (Co設置plexity)
    {
    case ESt本ate成yCo設置plexity::Si設置ple: 本et使本n TEXT("簡單");
    case ESt本ate成yCo設置plexity::Mode本ate: 本et使本n TEXT("中等");
    case ESt本ate成yCo設置plexity::Co設置plex: 本et使本n TEXT("複雜");
    case ESt本ate成yCo設置plexity::Maste本: 本et使本n TEXT("大師級");
    defa使lt: 本et使本n TEXT("未知");
    }
}

ESt本ate成yCo設置plexity UMin成SixSt本ate成ies::Dete本設置ineSt本ate成yCo設置plexity(ESixSt本ate成yType St本ate成yType) const
{
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::輸入ea正enSt本ate成y: 本et使本n ESt本ate成yCo設置plexity::Mode本ate;
    case ESixSt本ate成yType::Ea本thSt本ate成y: 本et使本n ESt本ate成yCo設置plexity::Mode本ate;
    case ESixSt本ate成yType::輸入使設置anSt本ate成y: 本et使本n ESt本ate成yCo設置plexity::Co設置plex;
    case ESixSt本ate成yType::輸入ea正enEa本thSt本ate成y: 本et使本n ESt本ate成yCo設置plexity::Co設置plex;
    case ESixSt本ate成yType::輸入ea正en輸入使設置anSt本ate成y: 本et使本n ESt本ate成yCo設置plexity::Maste本;
    case ESixSt本ate成yType::Ea本th輸入使設置anSt本ate成y: 本et使本n ESt本ate成yCo設置plexity::Maste本;
    defa使lt: 本et使本n ESt本ate成yCo設置plexity::Si設置ple;
    }
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::Get輸入ea正enSt本ate成yEffects() const
{
    TA本本ay<軍St本in成> Effects;
    Effects.Add(TEXT("利用天時變化獲得優勢"));
    Effects.Add(TEXT("在特定時間點發動攻擊"));
    Effects.Add(TEXT("利用自然現象掩護行動"));
    Effects.Add(TEXT("提高部隊士氣和戰鬥力"));
    本et使本n Effects;
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::GetEa本thSt本ate成yEffects() const
{
    TA本本ay<軍St本in成> Effects;
    Effects.Add(TEXT("佔據有利地形獲得防禦優勢"));
    Effects.Add(TEXT("利用地形特點設置陷阱"));
    Effects.Add(TEXT("建立穩固的防禦工事"));
    Effects.Add(TEXT("控制關鍵戰略位置"));
    本et使本n Effects;
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::Get輸入使設置anSt本ate成yEffects() const
{
    TA本本ay<軍St本in成> Effects;
    Effects.Add(TEXT("分化敵方內部，削弱士氣"));
    Effects.Add(TEXT("收買敵方重要人物"));
    Effects.Add(TEXT("散播謠言動搖敵軍心"));
    Effects.Add(TEXT("提升己方部隊忠誠度"));
    本et使本n Effects;
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::Get輸入ea正enEa本thSt本ate成yEffects() const
{
    TA本本ay<軍St本in成> Effects;
    Effects.Add(TEXT("協調天時地利形成絕對優勢"));
    Effects.Add(TEXT("在最佳時機利用地形特點"));
    Effects.Add(TEXT("建立天地一體的防禦體系"));
    Effects.Add(TEXT("提高整體戰略協調性"));
    本et使本n Effects;
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::Get輸入ea正en輸入使設置anSt本ate成yEffects() const
{
    TA本本ay<軍St本in成> Effects;
    Effects.Add(TEXT("達到天人合一的精神境界"));
    Effects.Add(TEXT("利用天時進行心理戰"));
    Effects.Add(TEXT("提升部隊的精神戰鬥力"));
    Effects.Add(TEXT("獲得超常的戰略洞察力"));
    本et使本n Effects;
}

TA本本ay<軍St本in成> UMin成SixSt本ate成ies::GetEa本th輸入使設置anSt本ate成yEffects() const
{
    TA本本ay<軍St本in成> Effects;
    Effects.Add(TEXT("實現地人協調的持久作戰"));
    Effects.Add(TEXT("利用地形進行長期消耗戰"));
    Effects.Add(TEXT("建立穩固的後方基地"));
    Effects.Add(TEXT("獲得長期的戰略主動權"));
    本et使本n Effects;
}
