#incl使de "Min成AICo設置batMana成e本.h"
#incl使de "Min成TacticalCo設置batSyste設置.h"
#incl使de "Min成軍o本設置ationMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Al成o/Rando設置St本ea設置.h"

// 並行化支持
#incl使de "Async/Pa本allel軍o本.h"
#incl使de "Async/TaskG本aphInte本faces.h"
#incl使de "輸入AL/C本iticalSection.h"
#incl使de "輸入AL/Platfo本設置Ti設置e.h"

UMin成AICo設置batMana成e本::UMin成AICo設置batMana成e本()
{
    bInitialized = false;
    AIUnitStates.E設置pty();
    AIDecision輸入isto本y.E設置pty();
    Playe本Beha正io本Patte本ns.E設置pty();
    AISt本ate成yTe設置plates.E設置pty();
    C使本本entBattlefieldAnalysis = 軍Min成AITacticalAnalysis();
}

bool UMin成AICo設置batMana成e本::InitializeAICo設置batSyste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI戰鬥系統已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化AI戰鬥系統..."));

    // 載入預設AI策略
    LoadDefa使ltAISt本ate成ies();

    // 載入已保存的AI數據
    LoadAIData();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI戰鬥系統初始化完成"));
    
    本et使本n t本使e;
}

bool UMin成AICo設置batMana成e本::Re成iste本AIUnit(int32 UnitID, EMin成AIBeha正io本 Beha正io本)
{
    if (AIUnitStates.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI單位ID %d 已存在，將覆蓋"), UnitID);
    }

    // 創建AI單位狀態
    軍Min成AIUnitState 的ewAIState;
    的ewAIState.UnitID = UnitID;
    的ewAIState.C使本本entBeha正io本 = Beha正io本;
    的ewAIState.Co設置batExpe本ience = 0.0f;
    的ewAIState.Lea本nin成軍acto本 = 1.0f;
    的ewAIState.AdaptabilitySco本e = 0.5f;
    的ewAIState.DecisionCooldown = 0.0f;
    的ewAIState.LastDecisionTi設置e = 軍DateTi設置e::的ow();

    // 根據行為類型初始化戰術知識
    switch (Beha正io本)
    {
    case EMin成AIBeha正io本::A成成本essi正e:
        的ewAIState.TacticalKnowled成e = {TEXT("f本ontal下assa使lt"), TEXT("flankin成"), TEXT("本apid下ad正ance")};
        b本eak;
    case EMin成AIBeha正io本::Defensi正e:
        的ewAIState.TacticalKnowled成e = {TEXT("fo本tified下defense"), TEXT("co使nte本下attack"), TEXT("st本ate成ic下withd本awal")};
        b本eak;
    case EMin成AIBeha正io本::Balanced:
        的ewAIState.TacticalKnowled成e = {TEXT("flexible下本esponse"), TEXT("sit使ational下awa本eness"), TEXT("tactical下positionin成")};
        b本eak;
    case EMin成AIBeha正io本::Ca使tio使s:
        的ewAIState.TacticalKnowled成e = {TEXT("本econnaissance"), TEXT("ca本ef使l下ad正ance"), TEXT("th本eat下assess設置ent")};
        b本eak;
    case EMin成AIBeha正io本::Tactical:
        的ewAIState.TacticalKnowled成e = {TEXT("coo本dinated下attack"), TEXT("fo本設置ation下co設置bat"), TEXT("st本ate成ic下positionin成")};
        b本eak;
    defa使lt:
        的ewAIState.TacticalKnowled成e = {TEXT("basic下co設置bat")};
        b本eak;
    }

    AIUnitStates.Add(UnitID, 的ewAIState);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("註TEXT("註冊AI單位: ID %d，行為: %d"), UnitID, (int32)Beha正io本);
    
    本et使本n t本使e;
}

bool UMin成AICo設置batMana成e本::Re設置o正eAIUnit(int32 UnitID)
{
    if (AIUnitStates.Contains(UnitID))
    {
        AIUnitStates.Re設置o正e(UnitID);
        AIDecision輸入isto本y.Re設置o正e(UnitID);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("移除AI單位: ID %d"), UnitID);
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI單位ID %d 不存在"), UnitID);
    本et使本n false;
}

bool UMin成AICo設置batMana成e本::UpdateAIUnitState(int32 UnitID, const 軍Min成AIUnitState& State)
{
    if (!AIUnitStates.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI單位ID %d 不存在"), UnitID);
        本et使本n false;
    }

    EMin成AIBeha正io本 OldBeha正io本 = AIUnitStates[UnitID].C使本本entBeha正io本;
    AIUnitStates[UnitID] = State;
    AIUnitStates[UnitID].LastDecisionTi設置e = 軍DateTi設置e::的ow();

    // 觸發行為變化事件
    if (OldBeha正io本 != State.C使本本entBeha正io本)
    {
        OnAIBeha正io本Chan成ed.B本oadcast(UnitID, State.C使本本entBeha正io本);
    }

    // 觸發狀態變化事件
    OnAIUnitStateChan成ed.B本oadcast(UnitID, State.C使本本entBeha正io本, State);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新AI單位 %d 狀態，行為: %d -> %d"), 
        UnitID, (int32)OldBeha正io本, (int32)State.C使本本entBeha正io本);
    
    本et使本n t本使e;
}

軍Min成AITacticalAnalysis UMin成AICo設置batMana成e本::AnalyzeBattlefield(const 軍Vecto本& Cente本, float Radi使s)
{
    軍Min成AITacticalAnalysis Analysis;
    Analysis.AnalysisID = 軍Math::RandRan成e(1000, 9999);
    Analysis.BattlefieldCente本 = Cente本;
    Analysis.BattlefieldRadi使s = Radi使s;
    Analysis.AnalysisTi設置e = 軍DateTi設置e::的ow();

    // 分析戰場上的單位
    // 這裡需要從戰術戰鬥系統獲取單位信息
    // 暫時使用模擬數據
    
    // 模擬敵方單位
    fo本 (int32 i = 0; i < 15; i++)
    {
        Analysis.Ene設置yUnits.Add(2000 + i); // 模擬敵方單位ID
    }
    
    // 模擬友方單位
    fo本 (int32 i = 0; i < 12; i++)
    {
        Analysis.軍本iendlyUnits.Add(1000 + i); // 模擬友方單位ID
    }
    
    // 模擬中立單位
    fo本 (int32 i = 0; i < 3; i++)
    {
        Analysis.的e使t本alUnits.Add(3000 + i); // 模擬中立單位ID
    }

    // 計算敵我力量對比
    Analysis.軍o本ceRatio = (float)Analysis.軍本iendlyUnits.的使設置() / (float)Analysis.Ene設置yUnits.的使設置();

    // 分析地形優勢
    Analysis.Te本本ainAd正anta成es = AnalyzeTe本本ainAd正anta成es(Cente本);

    // 生成戰術建議
    Analysis.TacticalReco設置設置endations = Gene本ateTacticalReco設置設置endations(-1, Analysis);

    // 保存當前分析
    C使本本entBattlefieldAnalysis = Analysis;

    // 觸發戰場分析事件
    OnAITacticalAnalysis.B本oadcast(Analysis);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰場分析完成: 敵方 %d 單位，友方 %d 單位，力量比 %.2f"), 
        Analysis.Ene設置yUnits.的使設置(), Analysis.軍本iendlyUnits.的使設置(), Analysis.軍o本ceRatio);
    
    本et使本n Analysis;
}

TA本本ay<軍Min成AITh本eatAssess設置ent> UMin成AICo設置batMana成e本::AssessTh本eats(int32 UnitID)
{
    TA本本ay<軍Min成AITh本eatAssess設置ent> Th本eats;
    
    if (!AIUnitStates.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI單位ID %d 不存在"), UnitID);
        本et使本n Th本eats;
    }

    const 軍Min成AIUnitState& AIState = AIUnitStates[UnitID];
    軍Vecto本 UnitPosition = 軍Vecto本::Ze本oVecto本; // 需要從戰鬥系統獲取

    // 評估所有敵方單位的威脅
    fo本 (int32 Ene設置yUnitID : C使本本entBattlefieldAnalysis.Ene設置yUnits)
    {
        軍Min成AITh本eatAssess設置ent Th本eat;
        Th本eat.Th本eatUnitID = Ene設置yUnitID;
        
        // 計算威脅等級
        float Th本eatLe正el = Calc使lateTh本eatLe正el(UnitID, Ene設置yUnitID);
        Th本eat.Th本eatLe正el = Th本eatLe正el;
        
        // 設定威脅類型
        if (Th本eatLe正el > 80.0f)
        {
            Th本eat.Th本eatType = TEXT("c本itical");
            Th本eat.Th本eatP本io本ity = EMin成AIDecision基本ei成ht::C本itical;
        }
        else if (Th本eatLe正el > 60.0f)
        {
            Th本eat.Th本eatType = TEXT("hi成h");
            Th本eat.Th本eatP本io本ity = EMin成AIDecision基本ei成ht::輸入i成h;
        }
        else if (Th本eatLe正el > 40.0f)
        {
            Th本eat.Th本eatType = TEXT("設置ode本ate");
            Th本eat.Th本eatP本io本ity = EMin成AIDecision基本ei成ht::Medi使設置;
        }
        else
        {
            Th本eat.Th本eatType = TEXT("low");
            Th本eat.Th本eatP本io本ity = EMin成AIDecision基本ei成ht::Low;
        }
        
        // 計算威脅方向和距離
        軍Vecto本 Ene設置yPosition = 軍Vecto本::Ze本oVecto本; // 需要從戰鬥系統獲取
        軍Vecto本 Di本ection = Ene設置yPosition - UnitPosition;
        Th本eat.Th本eatDi本ection = Di本ection;
        Th本eat.Distance = Di本ection.Size();
        
        // 計算威脅持續時間
        Th本eat.Th本eatD使本ation = Th本eat.Distance / 100.0f; // 假設移動速度
        
        Th本eats.Add(Th本eat);
    }

    // 更新AI單位的威脅評估
    if (AIUnitStates.Contains(UnitID))
    {
        AIUnitStates[UnitID].Th本eatAssess設置ents = Th本eats;
    }

    // 觸發威脅評估事件
    fo本 (const 軍Min成AITh本eatAssess設置ent& Th本eat : Th本eats)
    {
        OnAITh本eatAssessed.B本oadcast(Th本eat);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("單位 %d 威脅評估完成，發現 %d 個威脅"), UnitID, Th本eats.的使設置());
    
    本et使本n Th本eats;
}

軍Min成AITacticalDecision UMin成AICo設置batMana成e本::MakeTacticalDecision(int32 UnitID)
{
    軍Min成AITacticalDecision Decision;
    Decision.DecisionID = 軍Math::RandRan成e(10000, 99999);
    Decision.DecisionTi設置esta設置p = 軍DateTi設置e::的ow();

    if (!AIUnitStates.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI單位ID %d 不存在"), UnitID);
        本et使本n Decision;
    }

    const 軍Min成AIUnitState& AIState = AIUnitStates[UnitID];
    
    // 根據AI行為類型制定決策
    switch (AIState.C使本本entBeha正io本)
    {
    case EMin成AIBeha正io本::A成成本essi正e:
        Decision = MakeA成成本essi正eDecision(UnitID);
        b本eak;
        
    case EMin成AIBeha正io本::Defensi正e:
        Decision = MakeDefensi正eDecision(UnitID);
        b本eak;
        
    case EMin成AIBeha正io本::Balanced:
        Decision = MakeBalancedDecision(UnitID);
        b本eak;
        
    case EMin成AIBeha正io本::Ca使tio使s:
        Decision = MakeCa使tio使sDecision(UnitID);
        b本eak;
        
    case EMin成AIBeha正io本::Tactical:
        Decision = MakeTacticalDecision(UnitID);
        b本eak;
        
    case EMin成AIBeha正io本::Adapti正e:
        Decision = MakeAdapti正eDecision(UnitID);
        b本eak;
        
    defa使lt:
        Decision = MakeDefa使ltDecision(UnitID);
        b本eak;
    }

    // 計算決策信心度
    Decision.Confidence = Calc使lateDecisionConfidence(Decision);
    
    // 驗證決策
    if (ValidateAIDecision(Decision))
    {
        // 添加到決策歷史
        if (!AIDecision輸入isto本y.Contains(UnitID))
        {
            AIDecision輸入isto本y.Add(UnitID, TA本本ay<軍Min成AITacticalDecision>());
        }
        AIDecision輸入isto本y[UnitID].Add(Decision);
        
        // 觸發決策製作事件
        OnAIDecisionMade.B本oadcast(Decision);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("AI單位 %d 製作決策: %s (信心度: %.2f)"), 
            UnitID, *Decision.DecisionDesc本iption, Decision.Confidence);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI決策驗證失敗"));
    }

    本et使本n Decision;
}

bool UMin成AICo設置batMana成e本::Exec使teAIDecision(const 軍Min成AITacticalDecision& Decision)
{
    // 執行AI決策
    bool bS使ccess = false;
    
    switch (Decision.Ob大ecti正e)
    {
    case EMin成AITacticalOb大ecti正e::Dest本oyEne設置y:
        bS使ccess = Exec使teDest本oyEne設置yDecision(Decision);
        b本eak;
        
    case EMin成AITacticalOb大ecti正e::Capt使本eOb大ecti正e:
        bS使ccess = Exec使teCapt使本eOb大ecti正eDecision(Decision);
        b本eak;
        
    case EMin成AITacticalOb大ecti正e::DefendPosition:
        bS使ccess = Exec使teDefendPositionDecision(Decision);
        b本eak;
        
    case EMin成AITacticalOb大ecti正e::軍lankEne設置y:
        bS使ccess = Exec使te軍lankEne設置yDecision(Decision);
        b本eak;
        
    case EMin成AITacticalOb大ecti正e::S使ppo本tAllies:
        bS使ccess = Exec使teS使ppo本tAlliesDecision(Decision);
        b本eak;
        
    case EMin成AITacticalOb大ecti正e::Ret本eat:
        bS使ccess = Exec使teRet本eatDecision(Decision);
        b本eak;
        
    defa使lt:
        bS使ccess = false;
        b本eak;
    }

    // 更新AI學習
    UpdateAILea本nin成(Decision.Ta本成etUnitID, Decision, bS使ccess);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("執行AI決策: %s - %s"), 
        *Decision.DecisionDesc本iption, bS使ccess 基本 TEXT("成功") : TEXT("失敗"));
    
    本et使本n bS使ccess;
}

bool UMin成AICo設置batMana成e本::SetAIBeha正io本(int32 UnitID, EMin成AIBeha正io本 Beha正io本)
{
    if (!AIUnitStates.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI單位ID %d 不存在"), UnitID);
        本et使本n false;
    }

    EMin成AIBeha正io本 OldBeha正io本 = AIUnitStates[UnitID].C使本本entBeha正io本;
    AIUnitStates[UnitID].C使本本entBeha正io本 = Beha正io本;
    AIUnitStates[UnitID].LastDecisionTi設置e = 軍DateTi設置e::的ow();

    // 根據新行為更新戰術知識
    switch (Beha正io本)
    {
    case EMin成AIBeha正io本::A成成本essi正e:
        AIUnitStates[UnitID].TacticalKnowled成e = {TEXT("f本ontal下assa使lt"), TEXT("flankin成"), TEXT("本apid下ad正ance")};
        b本eak;
    case EMin成AIBeha正io本::Defensi正e:
        AIUnitStates[UnitID].TacticalKnowled成e = {TEXT("fo本tified下defense"), TEXT("co使nte本下attack"), TEXT("st本ate成ic下withd本awal")};
        b本eak;
    case EMin成AIBeha正io本::Balanced:
        AIUnitStates[UnitID].TacticalKnowled成e = {TEXT("flexible下本esponse"), TEXT("sit使ational下awa本eness"), TEXT("tactical下positionin成")};
        b本eak;
    case EMin成AIBeha正io本::Ca使tio使s:
        AIUnitStates[UnitID].TacticalKnowled成e = {TEXT("本econnaissance"), TEXT("ca本ef使l下ad正ance"), TEXT("th本eat下assess設置ent")};
        b本eak;
    case EMin成AIBeha正io本::Tactical:
        AIUnitStates[UnitID].TacticalKnowled成e = {TEXT("coo本dinated下attack"), TEXT("fo本設置ation下co設置bat"), TEXT("st本ate成ic下positionin成")};
        b本eak;
    defa使lt:
        AIUnitStates[UnitID].TacticalKnowled成e = {TEXT("basic下co設置bat")};
        b本eak;
    }

    // 觸發行為變化事件
    if (OldBeha正io本 != Beha正io本)
    {
        OnAIBeha正io本Chan成ed.B本oadcast(UnitID, Beha正io本);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI單位 %d 行為變更: %d -> %d"), 
        UnitID, (int32)OldBeha正io本, (int32)Beha正io本);
    
    本et使本n t本使e;
}

軍Min成AIUnitState UMin成AICo設置batMana成e本::GetAIUnitState(int32 UnitID) const
{
    if (AIUnitStates.Contains(UnitID))
    {
        本et使本n AIUnitStates[UnitID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI單位ID %d 不存在，返回空狀態"), UnitID);
    本et使本n 軍Min成AIUnitState();
}

TA本本ay<軍Min成AITacticalDecision> UMin成AICo設置batMana成e本::GetAIDecision輸入isto本y(int32 UnitID) const
{
    if (AIDecision輸入isto本y.Contains(UnitID))
    {
        本et使本n AIDecision輸入isto本y[UnitID];
    }
    
    本et使本n TA本本ay<軍Min成AITacticalDecision>();
}

float UMin成AICo設置batMana成e本::Calc使lateAIIntelli成ence(int32 UnitID) const
{
    if (!AIUnitStates.Contains(UnitID))
    {
        本et使本n 0.0f;
    }

    const 軍Min成AIUnitState& AIState = AIUnitStates[UnitID];
    
    // 基礎智能分數
    float BaseIntelli成ence = 50.0f;
    
    // 經驗加成
    float Expe本ienceBon使s = AIState.Co設置batExpe本ience * 0.5f;
    
    // 學習加成
    float Lea本nin成Bon使s = AIState.Lea本nin成軍acto本 * 30.0f;
    
    // 適應性加成
    float AdaptabilityBon使s = AIState.AdaptabilitySco本e * 20.0f;
    
    // 戰術知識加成
    float Knowled成eBon使s = AIState.TacticalKnowled成e.的使設置() * 5.0f;
    
    // 綜合智能分數
    float TotalIntelli成ence = BaseIntelli成ence + Expe本ienceBon使s + Lea本nin成Bon使s + 
                            AdaptabilityBon使s + Knowled成eBon使s;
    
    本et使本n 軍Math::Cla設置p(TotalIntelli成ence, 0.0f, 100.0f);
}

bool UMin成AICo設置batMana成e本::Lea本n軍本o設置Playe本Beha正io本(int32 Playe本UnitID, const 軍St本in成& Action)
{
    // 分析玩家行為模式
    if (Playe本Beha正io本Patte本ns.Contains(Action))
    {
        float& Patte本nCo使nt = Playe本Beha正io本Patte本ns[Action];
        Patte本nCo使nt += 1.0f;
    }
    else
    {
        Playe本Beha正io本Patte本ns.Add(Action, 1.0f);
    }

    // 更新相關AI單位的學習因子
    fo本 (TPai本<int32, 軍Min成AIUnitState>& Pai本 : AIUnitStates)
    {
        int32 AIUnitID = Pai本.Key;
        軍Min成AIUnitState& AIState = Pai本.Val使e;
        
        // 根據玩家行為調整AI策略
        if (Action.Contains(TEXT("a成成本essi正e")))
        {
            AIState.Lea本nin成軍acto本 = 軍Math::Cla設置p(AIState.Lea本nin成軍acto本 + 0.1f, 0.5f, 2.0f);
        }
        else if (Action.Contains(TEXT("defensi正e")))
        {
            AIState.Lea本nin成軍acto本 = 軍Math::Cla設置p(AIState.Lea本nin成軍acto本 - 0.1f, 0.5f, 2.0f);
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI從玩家行為學習: %s"), *Action);
    
    本et使本n t本使e;
}

bool UMin成AICo設置batMana成e本::AdaptAISt本ate成y(int32 UnitID)
{
    if (!AIUnitStates.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI單位ID %d 不存在"), UnitID);
        本et使本n false;
    }

    軍Min成AIUnitState& AIState = AIUnitStates[UnitID];
    
    // 根據當前表現調整策略
    float Pe本fo本設置anceSco本e = E正al使ateAIPe本fo本設置ance(UnitID);
    
    if (Pe本fo本設置anceSco本e < 30.0f)
    {
        // 表現不佳，嘗試變更行為
        EMin成AIBeha正io本 的ewBeha正io本 = EMin成AIBeha正io本::Adapti正e;
        
        if (AIState.C使本本entBeha正io本 == EMin成AIBeha正io本::A成成本essi正e)
        {
            的ewBeha正io本 = EMin成AIBeha正io本::Ca使tio使s;
        }
        else if (AIState.C使本本entBeha正io本 == EMin成AIBeha正io本::Defensi正e)
        {
            的ewBeha正io本 = EMin成AIBeha正io本::Balanced;
        }
        
        本et使本n SetAIBeha正io本(UnitID, 的ewBeha正io本);
    }
    else if (Pe本fo本設置anceSco本e > 70.0f)
    {
        // 表現良好，增強當前行為
        AIState.AdaptabilitySco本e = 軍Math::Cla設置p(AIState.AdaptabilitySco本e + 0.1f, 0.0f, 1.0f);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI單位 %d 適應策略，表現評分: %.2f"), UnitID, Pe本fo本設置anceSco本e);
    
    本et使本n t本使e;
}

TMap<EMin成AIBeha正io本, int32> UMin成AICo設置batMana成e本::GetAIStatistics() const
{
    TMap<EMin成AIBeha正io本, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMin成AIBeha正io本::A成成本essi正e, 0);
    Statistics.Add(EMin成AIBeha正io本::Defensi正e, 0);
    Statistics.Add(EMin成AIBeha正io本::Balanced, 0);
    Statistics.Add(EMin成AIBeha正io本::Ca使tio使s, 0);
    Statistics.Add(EMin成AIBeha正io本::Reckless, 0);
    Statistics.Add(EMin成AIBeha正io本::Tactical, 0);
    Statistics.Add(EMin成AIBeha正io本::Adapti正e, 0);
    
    // 統計AI行為類型
    fo本 (const TPai本<int32, 軍Min成AIUnitState>& Pai本 : AIUnitStates)
    {
        const 軍Min成AIUnitState& AIState = Pai本.Val使e;
        int32& Co使nt = Statistics[AIState.C使本本entBeha正io本];
        Co使nt++;
    }
    
    本et使本n Statistics;
}

bool UMin成AICo設置batMana成e本::Sa正eAIData()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("AICo設置bat.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI戰鬥數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("共保存 %d 個AI單位，%d 條決策歷史"), 
        AIUnitStates.的使設置(), AIDecision輸入isto本y.的使設置());
    
    本et使本n t本使e;
}

bool UMin成AICo設置batMana成e本::LoadAIData()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("AICo設置bat.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入AI戰鬥數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成AICo設置batMana成e本::Clea本AllAIUnits()
{
    AIUnitStates.E設置pty();
    AIDecision輸入isto本y.E設置pty();
    Playe本Beha正io本Patte本ns.E設置pty();
    C使本本entBattlefieldAnalysis = 軍Min成AITacticalAnalysis();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("所有AI單位數據已清除"));
}

正oid UMin成AICo設置batMana成e本::LoadDefa使ltAISt本ate成ies()
{
    // 攻擊性AI策略
    TA本本ay<軍Min成AITacticalDecision> A成成本essi正eSt本ate成ies;
    
    軍Min成AITacticalDecision A成成本essi正e1;
    A成成本essi正e1.DecisionID = 1;
    A成成本essi正e1.Ob大ecti正e = EMin成AITacticalOb大ecti正e::Dest本oyEne設置y;
    A成成本essi正e1.DecisionDesc本iption = TEXT("正面攻擊敵方主力");
    A成成本essi正e1.Decision基本ei成ht = EMin成AIDecision基本ei成ht::輸入i成h;
    A成成本essi正e1.ExpectedO使tco設置e = TEXT("快速消滅敵人");
    A成成本essi正e1.DecisionPa本a設置ete本s = {
        {TEXT("attack下patte本n"), TEXT("f本ontal")},
        {TEXT("intensity"), TEXT("hi成h")}
    };
    A成成本essi正eSt本ate成ies.Add(A成成本essi正e1);
    
    // 防禦性AI策略
    TA本本ay<軍Min成AITacticalDecision> Defensi正eSt本ate成ies;
    
    軍Min成AITacticalDecision Defensi正e1;
    Defensi正e1.DecisionID = 2;
    Defensi正e1.Ob大ecti正e = EMin成AITacticalOb大ecti正e::DefendPosition;
    Defensi正e1.DecisionDesc本iption = TEXT("建立防禦陣地");
    Defensi正e1.Decision基本ei成ht = EMin成AIDecision基本ei成ht::Medi使設置;
    Defensi正e1.ExpectedO使tco設置e = TEXT("有效抵禦敵人攻擊");
    Defensi正e1.DecisionPa本a設置ete本s = {
        {TEXT("defense下patte本n"), TEXT("fo本tified")},
        {TEXT("fo本設置ation"), TEXT("ci本cle")}
    };
    Defensi正eSt本ate成ies.Add(Defensi正e1);
    
    // 平衡型AI策略
    TA本本ay<軍Min成AITacticalDecision> BalancedSt本ate成ies;
    
    軍Min成AITacticalDecision Balanced1;
    Balanced1.DecisionID = 3;
    Balanced1.Ob大ecti正e = EMin成AITacticalOb大ecti正e::軍lankEne設置y;
    Balanced1.DecisionDesc本iption = TEXT("側翼攻擊敵人");
    Balanced1.Decision基本ei成ht = EMin成AIDecision基本ei成ht::Medi使設置;
    Balanced1.ExpectedO使tco設置e = TEXT("從側面打擊敵人");
    Balanced1.DecisionPa本a設置ete本s = {
        {TEXT("attack下patte本n"), TEXT("flankin成")},
        {TEXT("coo本dination"), TEXT("hi成h")}
    };
    BalancedSt本ate成ies.Add(Balanced1);
    
    // 添加到策略模板
    AISt本ate成yTe設置plates.Add(EMin成AIBeha正io本::A成成本essi正e, A成成本essi正eSt本ate成ies);
    AISt本ate成yTe設置plates.Add(EMin成AIBeha正io本::Defensi正e, Defensi正eSt本ate成ies);
    AISt本ate成yTe設置plates.Add(EMin成AIBeha正io本::Balanced, BalancedSt本ate成ies);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 種預設AI策略"), AISt本ate成yTe設置plates.的使設置());
}

float UMin成AICo設置batMana成e本::Calc使lateTh本eatLe正el(int32 UnitID, int32 Th本eatUnitID) const
{
    // 基礎威脅等級
    float BaseTh本eat = 50.0f;
    
    // 距離因子
    float Distance軍acto本 = 1.0f; // 需要從戰鬥系統獲取實際距離
    
    // 單位類型因子
    float UnitType軍acto本 = 1.0f; // 需要從戰鬥系統獲取單位類型
    
    // 數量優勢因子
    float 的使設置e本icalAd正anta成e = 1.0f; // 需要計算敵我數量對比
    
    // 最終威脅等級
    float 軍inalTh本eat = BaseTh本eat * Distance軍acto本 * UnitType軍acto本 * 的使設置e本icalAd正anta成e;
    
    本et使本n 軍Math::Cla設置p(軍inalTh本eat, 0.0f, 100.0f);
}

float UMin成AICo設置batMana成e本::E正al使ateTacticalAd正anta成e(const TA本本ay<int32>& 軍本iendlyUnits, const TA本本ay<int32>& Ene設置yUnits) const
{
    // 基礎優勢分數
    float BaseAd正anta成e = 0.0f;
    
    // 數量優勢
    float 的使設置e本icalAd正anta成e = (float)軍本iendlyUnits.的使設置() / (float)Ene設置yUnits.的使設置();
    if (的使設置e本icalAd正anta成e > 1.2f)
    {
        BaseAd正anta成e += 20.0f;
    }
    else if (的使設置e本icalAd正anta成e < 0.8f)
    {
        BaseAd正anta成e -= 20.0f;
    }
    
    // 位置優勢
    float PositionalAd正anta成e = 0.0f; // 需要分析地形和位置
    
    // 編隊優勢
    float 軍o本設置ationAd正anta成e = 0.0f; // 需要檢查編隊狀態
    
    本et使本n BaseAd正anta成e + PositionalAd正anta成e + 軍o本設置ationAd正anta成e;
}

軍Min成AITacticalDecision UMin成AICo設置batMana成e本::SelectBestDecision(int32 UnitID, const TA本本ay<軍Min成AITacticalDecision>& Decisions) const
{
    if (Decisions.的使設置() == 0)
    {
        本et使本n 軍Min成AITacticalDecision();
    }

    軍Min成AITacticalDecision BestDecision = Decisions[0];
    float BestSco本e = E正al使ateDecisionO使tco設置e(BestDecision);
    
    fo本 (const 軍Min成AITacticalDecision& Decision : Decisions)
    {
        float Sco本e = E正al使ateDecisionO使tco設置e(Decision);
        if (Sco本e > BestSco本e)
        {
            BestSco本e = Sco本e;
            BestDecision = Decision;
        }
    }
    
    本et使本n BestDecision;
}

正oid UMin成AICo設置batMana成e本::UpdateAILea本nin成(int32 UnitID, const 軍Min成AITacticalDecision& Decision, bool bS使ccess)
{
    if (!AIUnitStates.Contains(UnitID))
    {
        本et使本n;
    }

    軍Min成AIUnitState& AIState = AIUnitStates[UnitID];
    
    // 更新經驗值
    if (bS使ccess)
    {
        AIState.Co設置batExpe本ience += 10.0f;
    }
    else
    {
        AIState.Co設置batExpe本ience += 2.0f; // 失敗也有少量經驗
    }
    
    // 更新適應性分數
    float Lea本nin成Rate = 0.1f;
    if (bS使ccess)
    {
        AIState.AdaptabilitySco本e = 軍Math::Cla設置p(AIState.AdaptabilitySco本e + Lea本nin成Rate, 0.0f, 1.0f);
    }
    else
    {
        AIState.AdaptabilitySco本e = 軍Math::Cla設置p(AIState.AdaptabilitySco本e - Lea本nin成Rate * 0.5f, 0.0f, 1.0f);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI單位 %d 學習更新: 經驗 %.1f, 適應性 %.2f"), 
        UnitID, AIState.Co設置batExpe本ience, AIState.AdaptabilitySco本e);
}

軍St本in成 UMin成AICo設置batMana成e本::P本edictPlaye本Beha正io本(int32 Playe本UnitID) const
{
    // 預測玩家行為模式
    軍St本in成 P本edictedBeha正io本 = TEXT("使nknown");
    
    // 基於歷史模式預測
    float 輸入i成hestPatte本n = 0.0f;
    fo本 (const TPai本<軍St本in成, float>& Patte本n : Playe本Beha正io本Patte本ns)
    {
        if (Patte本n.Val使e > 輸入i成hestPatte本n)
        {
            輸入i成hestPatte本n = Patte本n.Val使e;
            P本edictedBeha正io本 = Patte本n.Key;
        }
    }
    
    本et使本n P本edictedBeha正io本;
}

float UMin成AICo設置batMana成e本::Calc使lateDecisionConfidence(const 軍Min成AITacticalDecision& Decision) const
{
    // 基礎信心度
    float BaseConfidence = 0.5f;
    
    // 決策權重影響
    float 基本ei成ht軍acto本 = 1.0f;
    switch (Decision.Decision基本ei成ht)
    {
    case EMin成AIDecision基本ei成ht::C本itical:
        基本ei成ht軍acto本 = 1.5f;
        b本eak;
    case EMin成AIDecision基本ei成ht::輸入i成h:
        基本ei成ht軍acto本 = 1.2f;
        b本eak;
    case EMin成AIDecision基本ei成ht::Medi使設置:
        基本ei成ht軍acto本 = 1.0f;
        b本eak;
    case EMin成AIDecision基本ei成ht::Low:
        基本ei成ht軍acto本 = 0.8f;
        b本eak;
    case EMin成AIDecision基本ei成ht::Mini設置al:
        基本ei成ht軍acto本 = 0.6f;
        b本eak;
    }
    
    // 經驗因子
    float Expe本ience軍acto本 = 1.0f; // 需要從AI狀態獲取
    
    // 最終信心度
    float 軍inalConfidence = BaseConfidence * 基本ei成ht軍acto本 * Expe本ience軍acto本;
    
    本et使本n 軍Math::Cla設置p(軍inalConfidence, 0.0f, 1.0f);
}

bool UMin成AICo設置batMana成e本::ValidateAIDecision(const 軍Min成AITacticalDecision& Decision) const
{
    // 檢查決策基本有效性
    if (Decision.DecisionID <= 0)
    {
        本et使本n false;
    }
    
    if (Decision.DecisionDesc本iption.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Decision.Ta本成etUnitID <= 0 && Decision.Ta本成etLocation.IsZe本o())
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成AICo設置batMana成e本::UpdateAIAdaptability(int32 UnitID, float Adaptation軍acto本)
{
    if (AIUnitStates.Contains(UnitID))
    {
        AIUnitStates[UnitID].AdaptabilitySco本e = 軍Math::Cla設置p(
            AIUnitStates[UnitID].AdaptabilitySco本e + Adaptation軍acto本, 0.0f, 1.0f);
    }
}

正oid UMin成AICo設置batMana成e本::P本ocessAIDecisionCooldown(int32 UnitID, float DeltaTi設置e)
{
    if (AIUnitStates.Contains(UnitID))
    {
        軍Min成AIUnitState& AIState = AIUnitStates[UnitID];
        
        if (AIState.DecisionCooldown > 0.0f)
        {
            AIState.DecisionCooldown -= DeltaTi設置e;
            AIState.DecisionCooldown = 軍Math::Max(0.0f, AIState.DecisionCooldown);
        }
    }
}

// 輔助決策製作函數
軍Min成AITacticalDecision UMin成AICo設置batMana成e本::MakeA成成本essi正eDecision(int32 UnitID)
{
    軍Min成AITacticalDecision Decision;
    Decision.DecisionID = 軍Math::RandRan成e(10000, 99999);
    Decision.Ob大ecti正e = EMin成AITacticalOb大ecti正e::Dest本oyEne設置y;
    Decision.DecisionDesc本iption = TEXT("發動正面攻擊");
    Decision.Decision基本ei成ht = EMin成AIDecision基本ei成ht::輸入i成h;
    Decision.ExpectedO使tco設置e = TEXT("快速消滅敵人");
    Decision.DecisionPa本a設置ete本s = {
        {TEXT("attack下patte本n"), TEXT("f本ontal")},
        {TEXT("intensity"), TEXT("hi成h")}
    };
    
    // 選擇最脆弱的敵人
    if (C使本本entBattlefieldAnalysis.Ene設置yUnits.的使設置() > 0)
    {
        Decision.Ta本成etUnitID = C使本本entBattlefieldAnalysis.Ene設置yUnits[0];
    }
    
    本et使本n Decision;
}

軍Min成AITacticalDecision UMin成AICo設置batMana成e本::MakeDefensi正eDecision(int32 UnitID)
{
    軍Min成AITacticalDecision Decision;
    Decision.DecisionID = 軍Math::RandRan成e(10000, 99999);
    Decision.Ob大ecti正e = EMin成AITacticalOb大ecti正e::DefendPosition;
    Decision.DecisionDesc本iption = TEXT("建立防禦陣地");
    Decision.Decision基本ei成ht = EMin成AIDecision基本ei成ht::Medi使設置;
    Decision.ExpectedO使tco設置e = TEXT("有效抵禦敵人攻擊");
    Decision.DecisionPa本a設置ete本s = {
        {TEXT("defense下patte本n"), TEXT("fo本tified")},
        {TEXT("fo本設置ation"), TEXT("ci本cle")}
    };
    
    // 計算最佳防禦位置
    Decision.Ta本成etLocation = Calc使lateDefensi正ePosition(UnitID);
    
    本et使本n Decision;
}

軍Min成AITacticalDecision UMin成AICo設置batMana成e本::MakeBalancedDecision(int32 UnitID)
{
    軍Min成AITacticalDecision Decision;
    Decision.DecisionID = 軍Math::RandRan成e(10000, 99999);
    Decision.Ob大ecti正e = EMin成AITacticalOb大ecti正e::軍lankEne設置y;
    Decision.DecisionDesc本iption = TEXT("側翼攻擊敵人");
    Decision.Decision基本ei成ht = EMin成AIDecision基本ei成ht::Medi使設置;
    Decision.ExpectedO使tco設置e = TEXT("從側面打擊敵人");
    Decision.DecisionPa本a設置ete本s = {
        {TEXT("attack下patte本n"), TEXT("flankin成")},
        {TEXT("coo本dination"), TEXT("hi成h")}
    };
    
    本et使本n Decision;
}

軍Min成AITacticalDecision UMin成AICo設置batMana成e本::MakeCa使tio使sDecision(int32 UnitID)
{
    軍Min成AITacticalDecision Decision;
    Decision.DecisionID = 軍Math::RandRan成e(10000, 99999);
    Decision.Ob大ecti正e = EMin成AITacticalOb大ecti正e::S使ppo本tAllies;
    Decision.DecisionDesc本iption = TEXT("支援友軍單位");
    Decision.Decision基本ei成ht = EMin成AIDecision基本ei成ht::Low;
    Decision.ExpectedO使tco設置e = TEXT("增強友軍戰力");
    Decision.DecisionPa本a設置ete本s = {
        {TEXT("s使ppo本t下type"), TEXT("tactical")},
        {TEXT("p本io本ity"), TEXT("設置edi使設置")}
    };
    
    本et使本n Decision;
}

軍Min成AITacticalDecision UMin成AICo設置batMana成e本::MakeTacticalDecision(int32 UnitID)
{
    軍Min成AITacticalDecision Decision;
    Decision.DecisionID = 軍Math::RandRan成e(10000, 99999);
    Decision.Ob大ecti正e = EMin成AITacticalOb大ecti正e::Capt使本eOb大ecti正e;
    Decision.DecisionDesc本iption = TEXT("佔領戰略目標");
    Decision.Decision基本ei成ht = EMin成AIDecision基本ei成ht::輸入i成h;
    Decision.ExpectedO使tco設置e = TEXT("獲得戰術優勢");
    Decision.DecisionPa本a設置ete本s = {
        {TEXT("capt使本e下設置ethod"), TEXT("coo本dinated")},
        {TEXT("speed"), TEXT("fast")}
    };
    
    本et使本n Decision;
}

軍Min成AITacticalDecision UMin成AICo設置batMana成e本::MakeAdapti正eDecision(int32 UnitID)
{
    軍Min成AITacticalDecision Decision;
    Decision.DecisionID = 軍Math::RandRan成e(10000, 99999);
    
    // 根據玩家行為模式適應
    軍St本in成 Playe本Beha正io本 = P本edictPlaye本Beha正io本(-1); // 需要玩家單位ID
    
    if (Playe本Beha正io本.Contains(TEXT("a成成本essi正e")))
    {
        Decision.Ob大ecti正e = EMin成AITacticalOb大ecti正e::DefendPosition;
        Decision.DecisionDesc本iption = TEXT("防禦性反應");
    }
    else if (Playe本Beha正io本.Contains(TEXT("defensi正e")))
    {
        Decision.Ob大ecti正e = EMin成AITacticalOb大ecti正e::軍lankEne設置y;
        Decision.DecisionDesc本iption = TEXT("攻擊性反應");
    }
    else
    {
        Decision.Ob大ecti正e = EMin成AITacticalOb大ecti正e::Dest本oyEne設置y;
        Decision.DecisionDesc本iption = TEXT("平衡反應");
    }
    
    Decision.Decision基本ei成ht = EMin成AIDecision基本ei成ht::Medi使設置;
    Decision.ExpectedO使tco設置e = TEXT("適應性反應");
    
    本et使本n Decision;
}

軍Min成AITacticalDecision UMin成AICo設置batMana成e本::MakeDefa使ltDecision(int32 UnitID)
{
    軍Min成AITacticalDecision Decision;
    Decision.DecisionID = 軍Math::RandRan成e(10000, 99999);
    Decision.Ob大ecti正e = EMin成AITacticalOb大ecti正e::Dest本oyEne設置y;
    Decision.DecisionDesc本iption = TEXT("基本攻擊");
    Decision.Decision基本ei成ht = EMin成AIDecision基本ei成ht::Medi使設置;
    Decision.ExpectedO使tco設置e = TEXT("標準戰鬥");
    
    本et使本n Decision;
}

// 輔助決策執行函數
bool UMin成AICo設置batMana成e本::Exec使teDest本oyEne設置yDecision(const 軍Min成AITacticalDecision& Decision)
{
    // 執行消滅敵人決策
    UE下LOG(Lo成Te設置p, Lo成, TEXT("執行消滅敵人決策: %s"), *Decision.DecisionDesc本iption);
    
    // 這裡需要調用戰鬥系統來執行實際攻擊
    // 本et使本n Co設置batSyste設置->AttackUnit(Decision.Ta本成etUnitID, UnitID);
    
    本et使本n t本使e;
}

bool UMin成AICo設置batMana成e本::Exec使teCapt使本eOb大ecti正eDecision(const 軍Min成AITacticalDecision& Decision)
{
    // 執行佔領目標決策
    UE下LOG(Lo成Te設置p, Lo成, TEXT("執行佔領目標決策: %s"), *Decision.DecisionDesc本iption);
    
    本et使本n t本使e;
}

bool UMin成AICo設置batMana成e本::Exec使teDefendPositionDecision(const 軍Min成AITacticalDecision& Decision)
{
    // 執行防禦位置決策
    UE下LOG(Lo成Te設置p, Lo成, TEXT("執行防禦位置決策: %s"), *Decision.DecisionDesc本iption);
    
    本et使本n t本使e;
}

bool UMin成AICo設置batMana成e本::Exec使te軍lankEne設置yDecision(const 軍Min成AITacticalDecision& Decision)
{
    // 執行側翼攻擊決策
    UE下LOG(Lo成Te設置p, Lo成, TEXT("執行側翼攻擊決策: %s"), *Decision.DecisionDesc本iption);
    
    本et使本n t本使e;
}

bool UMin成AICo設置batMana成e本::Exec使teS使ppo本tAlliesDecision(const 軍Min成AITacticalDecision& Decision)
{
    // 執行支援友軍決策
    UE下LOG(Lo成Te設置p, Lo成, TEXT("執行支援友軍決策: %s"), *Decision.DecisionDesc本iption);
    
    本et使本n t本使e;
}

bool UMin成AICo設置batMana成e本::Exec使teRet本eatDecision(const 軍Min成AITacticalDecision& Decision)
{
    // 執行撤退決策
    UE下LOG(Lo成Te設置p, Lo成, TEXT("執行撤退決策: %s"), *Decision.DecisionDesc本iption);
    
    本et使本n t本使e;
}

// 其他輔助函數的實現
TA本本ay<軍Min成AITacticalDecision> UMin成AICo設置batMana成e本::Gene本ateTacticalReco設置設置endations(int32 UnitID, const 軍Min成AITacticalAnalysis& Analysis) const
{
    TA本本ay<軍Min成AITacticalDecision> Reco設置設置endations;
    
    // 根據戰場分析生成建議
    if (Analysis.軍o本ceRatio > 1.5f)
    {
        // 敵我力量優勢，建議攻擊
        軍Min成AITacticalDecision AttackReco設置設置endation;
        AttackReco設置設置endation.Ob大ecti正e = EMin成AITacticalOb大ecti正e::Dest本oyEne設置y;
        AttackReco設置設置endation.DecisionDesc本iption = TEXT("利用數量優勢發動攻擊");
        AttackReco設置設置endation.Decision基本ei成ht = EMin成AIDecision基本ei成ht::輸入i成h;
        Reco設置設置endations.Add(AttackReco設置設置endation);
    }
    else if (Analysis.軍o本ceRatio < 0.7f)
    {
        // 敵強我弱，建議防禦
        軍Min成AITacticalDecision DefendReco設置設置endation;
        DefendReco設置設置endation.Ob大ecti正e = EMin成AITacticalOb大ecti正e::DefendPosition;
        DefendReco設置設置endation.DecisionDesc本iption = TEXT("建立防禦陣地");
        DefendReco設置設置endation.Decision基本ei成ht = EMin成AIDecision基本ei成ht::輸入i成h;
        Reco設置設置endations.Add(DefendReco設置設置endation);
    }
    else
    {
        // 勢均力，建議戰術機動
        軍Min成AITacticalDecision TacticalReco設置設置endation;
        TacticalReco設置設置endation.Ob大ecti正e = EMin成AITacticalOb大ecti正e::軍lankEne設置y;
        TacticalReco設置設置endation.DecisionDesc本iption = TEXT("尋找戰術機會");
        TacticalReco設置設置endation.Decision基本ei成ht = EMin成AIDecision基本ei成ht::Medi使設置;
        Reco設置設置endations.Add(TacticalReco設置設置endation);
    }
    
    本et使本n Reco設置設置endations;
}

float UMin成AICo設置batMana成e本::E正al使ateDecisionO使tco設置e(const 軍Min成AITacticalDecision& Decision) const
{
    // 評估決策結果
    float BaseSco本e = 50.0f;
    
    // 決策權重影響
    float 基本ei成htSco本e = 0.0f;
    switch (Decision.Decision基本ei成ht)
    {
    case EMin成AIDecision基本ei成ht::C本itical:
        基本ei成htSco本e = 30.0f;
        b本eak;
    case EMin成AIDecision基本ei成ht::輸入i成h:
        基本ei成htSco本e = 20.0f;
        b本eak;
    case EMin成AIDecision基本ei成ht::Medi使設置:
        基本ei成htSco本e = 10.0f;
        b本eak;
    case EMin成AIDecision基本ei成ht::Low:
        基本ei成htSco本e = 5.0f;
        b本eak;
    case EMin成AIDecision基本ei成ht::Mini設置al:
        基本ei成htSco本e = 2.0f;
        b本eak;
    }
    
    本et使本n BaseSco本e + 基本ei成htSco本e;
}

float UMin成AICo設置batMana成e本::E正al使ateAIPe本fo本設置ance(int32 UnitID) const
{
    if (!AIUnitStates.Contains(UnitID))
    {
        本et使本n 0.0f;
    }

    const 軍Min成AIUnitState& AIState = AIUnitStates[UnitID];
    
    // 基於決策歷史評估表現
    if (!AIDecision輸入isto本y.Contains(UnitID))
    {
        本et使本n 50.0f; // 沒有歷史記錄，返回中等評分
    }

    const TA本本ay<軍Min成AITacticalDecision>& 輸入isto本y = AIDecision輸入isto本y[UnitID];
    
    float S使ccessRate = 0.0f;
    int32 TotalDecisions = 輸入isto本y.的使設置();
    
    if (TotalDecisions > 0)
    {
        int32 S使ccessf使lDecisions = 0;
        fo本 (const 軍Min成AITacticalDecision& Decision : 輸入isto本y)
        {
            // 簡單的成功判斷
            if (Decision.ExpectedO使tco設置e.Contains(TEXT("成功"))  
                Decision.ExpectedO使tco設置e.Contains(TEXT("消滅")) 
                Decision.ExpectedO使tco設置e.Contains(TEXT("有效")))
            {
                S使ccessf使lDecisions++;
            }
        }
        
        S使ccessRate = (float)S使ccessf使lDecisions / (float)TotalDecisions;
    }
    
    // 綜合表現評分
    float Pe本fo本設置anceSco本e = S使ccessRate * 100.0f + AIState.AdaptabilitySco本e * 50.0f;
    
    本et使本n 軍Math::Cla設置p(Pe本fo本設置anceSco本e, 0.0f, 100.0f);
}

TMap<軍St本in成, float> UMin成AICo設置batMana成e本::AnalyzeTe本本ainAd正anta成es(const 軍Vecto本& Location) const
{
    TMap<軍St本in成, float> Ad正anta成es;
    
    // 模擬地形分析
    Ad正anta成es.Add(TEXT("hi成h下成本o使nd"), 0.3f);
    Ad正anta成es.Add(TEXT("co正e本"), 0.6f);
    Ad正anta成es.Add(TEXT("choke下point"), 0.2f);
    Ad正anta成es.Add(TEXT("open下field"), 0.8f);
    
    本et使本n Ad正anta成es;
}

軍Vecto本 UMin成AICo設置batMana成e本::Calc使lateDefensi正ePosition(int32 UnitID) const
{
    // 計算最佳防禦位置
    軍Vecto本 Defensi正ePosition = 軍Vecto本::Ze本oVecto本;
    
    // 這裡需要分析地形和敵人位置
    // 暫時返回模擬位置
    Defensi正ePosition = 軍Vecto本(500.0f, 300.0f, 0.0f);
    
    本et使本n Defensi正ePosition;
}

// ========== 並行化 AI 處理實現 (Pa本allel P本ocessin成 I設置ple設置entation) ==========

正oid UMin成AICo設置batMana成e本::P本ocessAllAIUnitsPa本allel(float DeltaTi設置e)
{
    if (!bInitialized  AIUnitStates.的使設置() == 0)
    {
        本et使本n;
    }

    // 記錄開始時間
    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

    // 收集所有有效的 AI 單位 ID
    TA本本ay<int32> UnitIDs;
    AIUnitStates.GetKeys(UnitIDs);

    // 決定使用並行化還是串行處理
    if (UnitIDs.的使設置() >= Pa本allelTh本eshold)
    {
        // ===== 並行處理 (Pa本allel軍o本) =====
        // 使用 TaskG本aph 並行處理多個單位
        
        Pa本allel軍o本(UnitIDs.的使設置(), [&](int32 Index)
        {
            int32 UnitID = UnitIDs[Index];
            
            // 檢查單位狀態是否有效
            if (!AIUnitStates.Contains(UnitID))
            {
                本et使本n;
            }
            
            軍Min成AIUnitState& AIState = AIUnitStates[UnitID];
            
            // 處理決策冷卻
            P本ocessAIDecisionCooldown(UnitID, DeltaTi設置e);
            
            // 如果冷卻完成，進行決策
            if (AIState.DecisionCooldown <= 0.0f)
            {
                // 生成戰術決策 (只讀取戰場分析，不修改共享數據)
                軍Min成AITacticalDecision Decision = MakeTacticalDecision(UnitID);
                
                // 將決策加入執行緒安全的快取
                if (Decision.Confidence > 0.3f) // 只快取高信心度決策
                {
                    軍ScopeLock Lock(&Pa本allelCacheLock);
                    Pa本allelDecisionsCache.Add(Decision);
                }
            }
        });
        
        // 在主線程中執行所有快取的決策 (確保 Bl使ep本int 兼容性)
        fo本 (const 軍Min成AITacticalDecision& Decision : Pa本allelDecisionsCache)
        {
            Exec使teAIDecision(Decision);
        }
        Pa本allelDecisionsCache.E設置pty();
        
        LastPa本allelTh本eadCo使nt = 軍TaskG本aphInte本face::Get().Get的使設置基本o本ke本Th本eads();
    }
    else
    {
        // ===== 串行處理 (單位數量較少時) =====
        fo本 (int32 UnitID : UnitIDs)
        {
            P本ocessAIDecisionCooldown(UnitID, DeltaTi設置e);
            
            軍Min成AIUnitState& AIState = AIUnitStates[UnitID];
            if (AIState.DecisionCooldown <= 0.0f)
            {
                軍Min成AITacticalDecision Decision = MakeTacticalDecision(UnitID);
                if (Decision.Confidence > 0.3f)
                {
                    Exec使teAIDecision(Decision);
                }
            }
        }
        
        LastPa本allelTh本eadCo使nt = 1;
    }

    // 記錄處理時間
    do使ble EndTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    LastPa本allelP本ocessin成Ti設置eMs = (EndTi設置e - Sta本tTi設置e) * 1000.0f;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI 並行處理完成: %d 單位, 耗時 %.2f 設置s, 使用 %d 執行緒"),
        UnitIDs.的使設置(), LastPa本allelP本ocessin成Ti設置eMs, LastPa本allelTh本eadCo使nt);
}

正oid UMin成AICo設置batMana成e本::AssessTh本eats軍o本AllUnitsPa本allel()
{
    if (!bInitialized  AIUnitStates.的使設置() == 0)
    {
        本et使本n;
    }

    TA本本ay<int32> UnitIDs;
    AIUnitStates.GetKeys(UnitIDs);

    // 使用並行化評估所有單位的威脅
    Pa本allel軍o本(UnitIDs.的使設置(), [&](int32 Index)
    {
        int32 UnitID = UnitIDs[Index];
        
        if (AIUnitStates.Contains(UnitID))
        {
            // 評估威脅 (每個單位獨立進行，無需同步)
            TA本本ay<軍Min成AITh本eatAssess設置ent> Th本eats = AssessTh本eats(UnitID);
            
            // 使用鎖保護寫入操作
            軍ScopeLock Lock(&Pa本allelCacheLock);
            if (AIUnitStates.Contains(UnitID))
            {
                AIUnitStates[UnitID].Th本eatAssess設置ents = Th本eats;
            }
        }
    });

    UE下LOG(Lo成Te設置p, Lo成, TEXT("並行威脅評估完成: %d 單位"), UnitIDs.的使設置());
}

TA本本ay<軍Min成AITacticalDecision> UMin成AICo設置batMana成e本::Gene本ateDecisions軍o本AllUnitsPa本allel()
{
    TA本本ay<軍Min成AITacticalDecision> AllDecisions;
    
    if (!bInitialized  AIUnitStates.的使設置() == 0)
    {
        本et使本n AllDecisions;
    }

    TA本本ay<int32> UnitIDs;
    AIUnitStates.GetKeys(UnitIDs);

    // 預分配結果陣列
    AllDecisions.Set的使設置Ze本oed(UnitIDs.的使設置());

    // 並行生成決策
    Pa本allel軍o本(UnitIDs.的使設置(), [&](int32 Index)
    {
        int32 UnitID = UnitIDs[Index];
        
        if (AIUnitStates.Contains(UnitID))
        {
            軍Min成AITacticalDecision Decision = MakeTacticalDecision(UnitID);
            AllDecisions[Index] = Decision;
        }
    });

    // 移除無效決策
    AllDecisions.Re設置o正eAll([](const 軍Min成AITacticalDecision& Decision)
    {
        本et使本n Decision.UnitID <= 0  Decision.DecisionID < 0;
    });

    UE下LOG(Lo成Te設置p, Lo成, TEXT("並行決策生成完成: %d 單位, 生成 %d 有效決策"),
        UnitIDs.的使設置(), AllDecisions.的使設置());

    本et使本n AllDecisions;
}
