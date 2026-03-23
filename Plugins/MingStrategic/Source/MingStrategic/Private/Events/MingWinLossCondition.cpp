#incl使de "E正ents/Min成基本inLossCondition.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"

UMin成基本inLossCondition::UMin成基本inLossCondition()
    : bIsInitialized(false)
{
    // 預分配容量
    Re成iste本edConditions.Rese本正e(50);
    Statistics = 軍基本inLossStatistics();
    C使本本entE正al使ation = 軍基本inLossE正al使ationRes使lt();
}

正oid UMin成基本inLossCondition::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    // 初始化遊戲狀態
    Ga設置eState.Add(TEXT("成a設置e下ti設置e"), TEXT("0"));
    Ga設置eState.Add(TEXT("playe本下tea設置"), TEXT("1"));
    Ga設置eState.Add(TEXT("ene設置y下tea設置"), TEXT("2"));
    Ga設置eState.Add(TEXT("正icto本y下sco本e"), TEXT("0"));
    Ga設置eState.Add(TEXT("defeat下sco本e"), TEXT("0"));
    Ga設置eState.Add(TEXT("d本aw下sco本e"), TEXT("0"));
    
    // 初始化民國歷史勝負條件
    Initialize輸入isto本ical基本inLossConditions();
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成基本inLossCondition initialized"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d win-loss conditions"), Re成iste本edConditions.的使設置());
}

正oid UMin成基本inLossCondition::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Re成iste本edConditions.E設置pty();
    ConditionMap.E設置pty();
    Ga設置eState.E設置pty();
    Statistics = 軍基本inLossStatistics();
    C使本本entE正al使ation = 軍基本inLossE正al使ationRes使lt();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成基本inLossCondition sh使tdown"));
}

bool UMin成基本inLossCondition::Re成iste本基本inLossCondition(const 軍基本inLossCondition& Condition)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("基本inLossCondition not initialized"));
        本et使本n false;
    }

    if (Condition.ConditionID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Condition ID cannot be e設置pty"));
        本et使本n false;
    }

    // 檢查是否已存在
    if (ConditionMap.Contains(Condition.ConditionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Condition %s al本eady 本e成iste本ed"), *Condition.ConditionID);
        本et使本n false;
    }

    // 驗證條件配置
    if (!ValidateConditionConfi成(Condition))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid condition confi成使本ation: %s"), *Condition.ConditionID);
        本et使本n false;
    }

    // 添加到註冊列表
    Re成iste本edConditions.Add(Condition);
    ConditionMap.Add(Condition.ConditionID, Condition);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本in-loss condition 本e成iste本ed: %s - %s"), 
        *Condition.ConditionID, *Condition.Condition的a設置e);
    
    本et使本n t本使e;
}

bool UMin成基本inLossCondition::Un本e成iste本基本inLossCondition(const 軍St本in成& ConditionID)
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }

    if (!ConditionMap.Contains(ConditionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Condition %s not fo使nd"), *ConditionID);
        本et使本n false;
    }

    // 從註冊列表移除
    fo本 (int32 i = 0; i < Re成iste本edConditions.的使設置(); ++i)
    {
        if (Re成iste本edConditions[i].ConditionID == ConditionID)
        {
            Re成iste本edConditions.Re設置o正eAt(i);
            b本eak;
        }
    }
    
    ConditionMap.Re設置o正e(ConditionID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本in-loss condition 使n本e成iste本ed: %s"), *ConditionID);
    
    本et使本n t本使e;
}

軍基本inLossE正al使ationRes使lt UMin成基本inLossCondition::E正al使ate基本inLoss()
{
    軍基本inLossE正al使ationRes使lt Res使lt;
    Res使lt.E正al使ationTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (!bIsInitialized)
    {
        Res使lt.E正al使ationDetails = TEXT("基本inLossCondition not initialized");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正al使atin成 win-loss conditions..."));

    // 檢查所有條件
    fo本 (軍基本inLossCondition& Condition : Re成iste本edConditions)
    {
        if (Condition.State == E基本inLossConditionState::Acti正e)
        {
            bool bSatisfied = CheckCondition(Condition.ConditionID);
            
            if (bSatisfied)
            {
                Condition.State = E基本inLossConditionState::Satisfied;
                Condition.SatisfactionSco本e = E正al使ateConditionSatisfaction(Condition);
                Res使lt.SatisfiedConditions.Add(Condition.ConditionID);
                
                // 記錄勝負分數
                if (Condition.Res使ltType == E基本inLossRes使lt::Victo本y)
                {
                    Res使lt.Victo本ySco本e += Condition.SatisfactionSco本e;
                }
                else if (Condition.Res使ltType == E基本inLossRes使lt::Defeat)
                {
                    Res使lt.DefeatSco本e += Condition.SatisfactionSco本e;
                }
                else if (Condition.Res使ltType == E基本inLossRes使lt::D本aw)
                {
                    Res使lt.D本awSco本e += Condition.SatisfactionSco本e;
                }
            }
            else
            {
                Condition.State = E基本inLossConditionState::軍ailed;
                Condition.SatisfactionSco本e = 0.0f;
                Res使lt.UnsatisfiedConditions.Add(Condition.ConditionID);
            }
            
            // 更新關鍵條件狀態
            if (Condition.bIsC本iticalCondition)
            {
                Res使lt.C本iticalConditionStates.Add(Condition.ConditionID, Condition.State);
            }
        }
    }

    // 計算總分
    Calc使late基本inLossSco本es(Res使lt);
    
    // 確定最終結果
    Res使lt.Res使lt = Dete本設置ine軍inalRes使lt(Res使lt);
    
    // 更新統計
    UpdateStatistics(Res使lt);
    
    // 通知勝負變化
    if (C使本本entE正al使ation.Res使lt != Res使lt.Res使lt)
    {
        的otify基本inLossChan成e(C使本本entE正al使ation.Res使lt, Res使lt.Res使lt);
    }
    
    // 處理遊戲結束
    if (Res使lt.Res使lt != E基本inLossRes使lt::Unknown && Res使lt.Res使lt != E基本inLossRes使lt::InP本o成本ess)
    {
        軍St本in成 Reason = 軍St本in成::P本intf(TEXT("Ga設置e ended: %s"), *UEn使設置::GetVal使eAsSt本in成(Res使lt.Res使lt));
        輸入andleGa設置eO正e本(Res使lt.Res使lt, Reason);
    }
    
    // 更新當前評估
    C使本本entE正al使ation = Res使lt;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本in-loss e正al使ation co設置pleted: %s (V:%.2f, D:%.2f, D:%.2f)"), 
        *UEn使設置::GetVal使eAsSt本in成(Res使lt.Res使lt), 
        Res使lt.Victo本ySco本e, Res使lt.DefeatSco本e, Res使lt.D本awSco本e);
    
    本et使本n Res使lt;
}

bool UMin成基本inLossCondition::CheckCondition(const 軍St本in成& ConditionID)
{
    const 軍基本inLossCondition* Condition = ConditionMap.軍ind(ConditionID);
    if (!Condition)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Condition %s not fo使nd"), *ConditionID);
        本et使本n false;
    }

    // 檢查間隔
    float C使本本entTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    if (C使本本entTi設置e - Condition->LastCheckTi設置e < Condition->CheckInte本正al)
    {
        本et使本n Condition->State == E基本inLossConditionState::Satisfied;
    }

    Condition->LastCheckTi設置e = C使本本entTi設置e;

    // 根據條件類型檢查
    bool bSatisfied = false;
    switch (Condition->ConditionType)
    {
    case E基本inLossConditionType::Milita本y:
        bSatisfied = CheckMilita本yCondition(*Condition);
        b本eak;
    case E基本inLossConditionType::Political:
        bSatisfied = CheckPoliticalCondition(*Condition);
        b本eak;
    case E基本inLossConditionType::Econo設置ic:
        bSatisfied = CheckEcono設置icCondition(*Condition);
        b本eak;
    case E基本inLossConditionType::Te本本ito本ial:
        bSatisfied = CheckTe本本ito本ialCondition(*Condition);
        b本eak;
    case E基本inLossConditionType::Ti設置eBased:
        bSatisfied = CheckTi設置eBasedCondition(*Condition);
        b本eak;
    case E基本inLossConditionType::Sco本eBased:
        bSatisfied = CheckSco本eBasedCondition(*Condition);
        b本eak;
    case E基本inLossConditionType::C使sto設置:
        bSatisfied = CheckC使sto設置Condition(*Condition);
        b本eak;
    defa使lt:
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unknown condition type: %d"), static下cast<int32>(Condition->ConditionType));
        b本eak;
    }

    本et使本n bSatisfied;
}

軍基本inLossCondition UMin成基本inLossCondition::Get基本inLossCondition(const 軍St本in成& ConditionID) const
{
    const 軍基本inLossCondition* Condition = ConditionMap.軍ind(ConditionID);
    本et使本n Condition 基本 *Condition : 軍基本inLossCondition();
}

TA本本ay<軍基本inLossCondition> UMin成基本inLossCondition::GetAll基本inLossConditions() const
{
    本et使本n Re成iste本edConditions;
}

TA本本ay<軍基本inLossCondition> UMin成基本inLossCondition::GetConditionsByType(E基本inLossConditionType ConditionType) const
{
    TA本本ay<軍基本inLossCondition> Res使lt;
    
    fo本 (const 軍基本inLossCondition& Condition : Re成iste本edConditions)
    {
        if (Condition.ConditionType == ConditionType)
        {
            Res使lt.Add(Condition);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍基本inLossCondition> UMin成基本inLossCondition::GetC本iticalConditions() const
{
    TA本本ay<軍基本inLossCondition> Res使lt;
    
    fo本 (const 軍基本inLossCondition& Condition : Re成iste本edConditions)
    {
        if (Condition.bIsC本iticalCondition)
        {
            Res使lt.Add(Condition);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍基本inLossCondition> UMin成基本inLossCondition::GetSatisfiedConditions() const
{
    TA本本ay<軍基本inLossCondition> Res使lt;
    
    fo本 (const 軍基本inLossCondition& Condition : Re成iste本edConditions)
    {
        if (Condition.State == E基本inLossConditionState::Satisfied)
        {
            Res使lt.Add(Condition);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍基本inLossCondition> UMin成基本inLossCondition::GetUnsatisfiedConditions() const
{
    TA本本ay<軍基本inLossCondition> Res使lt;
    
    fo本 (const 軍基本inLossCondition& Condition : Re成iste本edConditions)
    {
        if (Condition.State == E基本inLossConditionState::軍ailed)
        {
            Res使lt.Add(Condition);
        }
    }
    
    本et使本n Res使lt;
}

正oid UMin成基本inLossCondition::SetGa設置eState(const TMap<軍St本in成, 軍St本in成>& 的ewGa設置eState)
{
    Ga設置eState = 的ewGa設置eState;
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Ga設置e state 使pdated with %d 正al使es"), Ga設置eState.的使設置());
}

TMap<軍St本in成, 軍St本in成> UMin成基本inLossCondition::GetGa設置eState() const
{
    本et使本n Ga設置eState;
}

正oid UMin成基本inLossCondition::UpdateGa設置eStateVal使e(const 軍St本in成& Key, const 軍St本in成& Val使e)
{
    Ga設置eState.Add(Key, Val使e);
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Ga設置e state 正al使e 使pdated: %s = %s"), *Key, *Val使e);
}

軍基本inLossStatistics UMin成基本inLossCondition::Get基本inLossStatistics() const
{
    本et使本n Statistics;
}

正oid UMin成基本inLossCondition::ResetStatistics()
{
    Statistics = 軍基本inLossStatistics();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本in-loss statistics 本eset"));
}

軍基本inLossE正al使ationRes使lt UMin成基本inLossCondition::軍o本ceE正al使ation()
{
    本et使本n E正al使ate基本inLoss();
}

E基本inLossRes使lt UMin成基本inLossCondition::GetC使本本entRes使lt() const
{
    本et使本n C使本本entE正al使ation.Res使lt;
}

float UMin成基本inLossCondition::Get基本inLossSco本e() const
{
    本et使本n C使本本entE正al使ation.TotalSco本e;
}

bool UMin成基本inLossCondition::IsGa設置eO正e本() const
{
    本et使本n C使本本entE正al使ation.Res使lt != E基本inLossRes使lt::Unknown && 
           C使本本entE正al使ation.Res使lt != E基本inLossRes使lt::InP本o成本ess;
}

軍St本in成 UMin成基本inLossCondition::GetGa設置eO正e本Reason() const
{
    本et使本n C使本本entE正al使ation.E正al使ationDetails;
}

// === 內部方法實現 ===

正oid UMin成基本inLossCondition::Initialize輸入isto本ical基本inLossConditions()
{
    C本eateMilita本yConditions();
    C本eatePoliticalConditions();
    C本eateEcono設置icConditions();
    C本eateTe本本ito本ialConditions();
    C本eateTi設置eBasedConditions();
    C本eateSco本eBasedConditions();
}

正oid UMin成基本inLossCondition::C本eateMilita本yConditions()
{
    // 敵方軍事勝利條件
    軍基本inLossCondition Playe本Milita本yVicto本y;
    Playe本Milita本yVicto本y.ConditionID = TEXT("playe本下設置ilita本y下正icto本y");
    Playe本Milita本yVicto本y.Condition的a設置e = TEXT("玩家軍事勝利");
    Playe本Milita本yVicto本y.ConditionDesc本iption = TEXT("玩家軍事力量完全擊敗敵方");
    Playe本Milita本yVicto本y.ConditionType = E基本inLossConditionType::Milita本y;
    Playe本Milita本yVicto本y.Res使ltType = E基本inLossRes使lt::Victo本y;
    Playe本Milita本yVicto本y.P本io本ity = E基本inLossP本io本ity::C本itical;
    Playe本Milita本yVicto本y.bIsC本iticalCondition = t本使e;
    Playe本Milita本yVicto本y.CheckInte本正al = 5.0f;
    
    軍基本inLossConditionPa本a設置ete本 Ene設置yUnits;
    Ene設置yUnits.Pa本a設置ete本的a設置e = TEXT("ene設置y下使nits");
    Ene設置yUnits.Pa本a設置ete本Val使e = TEXT("0");
    Ene設置yUnits.Co設置pa本isonOpe本ato本 = TEXT("<=");
    Ene設置yUnits.bIsReq使i本ed = t本使e;
    Ene設置yUnits.基本ei成ht = 1.0f;
    
    Playe本Milita本yVicto本y.Pa本a設置ete本s.Add(Ene設置yUnits);
    
    Re成iste本基本inLossCondition(Playe本Milita本yVicto本y);

    // 敵方軍事失敗條件
    軍基本inLossCondition Playe本Milita本yDefeat;
    Playe本Milita本yDefeat.ConditionID = TEXT("playe本下設置ilita本y下defeat");
    Playe本Milita本yDefeat.Condition的a設置e = TEXT("玩家軍事失敗");
    Playe本Milita本yDefeat.ConditionDesc本iption = TEXT("玩家軍事力量被完全消滅");
    Playe本Milita本yDefeat.ConditionType = E基本inLossConditionType::Milita本y;
    Playe本Milita本yDefeat.Res使ltType = E基本inLossRes使lt::Defeat;
    Playe本Milita本yDefeat.P本io本ity = E基本inLossP本io本ity::C本itical;
    Playe本Milita本yDefeat.bIsC本iticalCondition = t本使e;
    Playe本Milita本yDefeat.CheckInte本正al = 5.0f;
    
    軍基本inLossConditionPa本a設置ete本 Playe本Units;
    Playe本Units.Pa本a設置ete本的a設置e = TEXT("playe本下使nits");
    Playe本Units.Pa本a設置ete本Val使e = TEXT("0");
    Playe本Units.Co設置pa本isonOpe本ato本 = TEXT("<=");
    Playe本Units.bIsReq使i本ed = t本使e;
    Playe本Units.基本ei成ht = 1.0f;
    
    Playe本Milita本yDefeat.Pa本a設置ete本s.Add(Playe本Units);
    
    Re成iste本基本inLossCondition(Playe本Milita本yDefeat);
}

正oid UMin成基本inLossCondition::C本eatePoliticalConditions()
{
    // 政治統一條件
    軍基本inLossCondition PoliticalUnification;
    PoliticalUnification.ConditionID = TEXT("political下使nification");
    PoliticalUnification.Condition的a設置e = TEXT("政治統一");
    PoliticalUnification.ConditionDesc本iption = TEXT("實現政治統一，控制所有地區");
    PoliticalUnification.ConditionType = E基本inLossConditionType::Political;
    PoliticalUnification.Res使ltType = E基本inLossRes使lt::Victo本y;
    PoliticalUnification.P本io本ity = E基本inLossP本io本ity::輸入i成h;
    PoliticalUnification.bIsC本iticalCondition = false;
    PoliticalUnification.CheckInte本正al = 10.0f;
    
    軍基本inLossConditionPa本a設置ete本 PoliticalCont本ol;
    PoliticalCont本ol.Pa本a設置ete本的a設置e = TEXT("political下cont本ol");
    PoliticalCont本ol.Pa本a設置ete本Val使e = TEXT("100");
    PoliticalCont本ol.Co設置pa本isonOpe本ato本(TEXT(">="));
    PoliticalCont本ol.bIsReq使i本ed = t本使e;
    PoliticalCont本ol.基本ei成ht = 1.0f;
    
    PoliticalUnification.Pa本a設置ete本s.Add(PoliticalCont本ol);
    
    Re成iste本基本inLossCondition(PoliticalUnification);
}

正oid UMin成基本inLossCondition::C本eateEcono設置icConditions()
{
    // 經濟勝利條件
    軍基本inLossCondition Econo設置icVicto本y;
    Econo設置icVicto本y.ConditionID = TEXT("econo設置ic下正icto本y");
    Econo設置icVicto本y.Condition的a設置e = TEXT("經濟勝利");
    Econo設置icVicto本y.ConditionDesc本iption = TEXT("經濟實力遠超敵方");
    Econo設置icVicto本y.ConditionType = E基本inLossConditionType::Econo設置ic;
    Econo設置icVicto本y.Res使ltType = E基本inLossRes使lt::Victo本y;
    Econo設置icVicto本y.P本io本ity = E基本inLossP本io本ity::Medi使設置;
    Econo設置icVicto本y.bIsC本iticalCondition = false;
    Econo設置icVicto本y.CheckInte本正al = 15.0f;
    
    軍基本inLossConditionPa本a設置ete本 Econo設置icAd正anta成e;
    Econo設置icAd正anta成e.Pa本a設置ete本的a設置e = TEXT("econo設置ic下ad正anta成e");
    Econo設置icAd正anta成e.Pa本a設置ete本Val使e = TEXT("200");
    Econo設置icAd正anta成e.Co設置pa本isonOpe本ato本(TEXT(">"));
    Econo設置icAd正anta成e.bIsReq使i本ed = t本使e;
    Econo設置icAd正anta成e.基本ei成ht = 1.0f;
    
    Econo設置icVicto本y.Pa本a設置ete本s.Add(Econo設置icAd正anta成e);
    
    Re成iste本基本inLossCondition(Econo設置icVicto本y);
}

正oid UMin成基本inLossCondition::C本eateTe本本ito本ialConditions()
{
    // 領土控制勝利條件
    軍基本inLossCondition Te本本ito本ialVicto本y;
    Te本本ito本ialVicto本y.ConditionID = TEXT("te本本ito本ial下正icto本y");
    Te本本ito本ialVicto本y.Condition的a設置e = TEXT("領土控制勝利");
    Te本本ito本ialVicto本y.ConditionDesc本iption = TEXT("控制大部分領土");
    Te本本ito本ialVicto本y.ConditionType = E基本inLossConditionType::Te本本ito本ial;
    Te本本ito本ialVicto本y.Res使ltType = E基本inLossRes使lt::Victo本y;
    Te本本ito本ialVicto本y.P本io本ity = E基本inLossP本io本ity::輸入i成h;
    Te本本ito本ialVicto本y.bIsC本iticalCondition = false;
    Te本本ito本ialVicto本y.CheckInte本正al = 10.0f;
    
    軍基本inLossConditionPa本a設置ete本 Te本本ito本yCont本ol;
    Te本本ito本yCont本ol.Pa本a設置ete本的a設置e = TEXT("te本本ito本y下cont本ol");
    Te本本ito本yCont本ol.Pa本a設置ete本Val使e = TEXT("75");
    Te本本ito本yCont本ol.Co設置pa本isonOpe本ato本(TEXT(">="));
    Te本本ito本yCont本ol.bIsReq使i本ed = t本使e;
    Te本本ito本yCont本ol.基本ei成ht = 1.0f;
    
    Te本本ito本ialVicto本y.Pa本a設置ete本s.Add(Te本本ito本yCont本ol);
    
    Re成iste本基本inLossCondition(Te本本ito本ialVicto本y);
}

正oid UMin成基本inLossCondition::C本eateTi設置eBasedConditions()
{
    // 時間限制條件
    軍基本inLossCondition Ti設置eLi設置it;
    Ti設置eLi設置it.ConditionID = TEXT("ti設置e下li設置it");
    Ti設置eLi設置it.Condition的a設置e = TEXT("時間限制");
    Ti設置eLi設置it.ConditionDesc本iption = TEXT("超過時間限制");
    Ti設置eLi設置it.ConditionType = E基本inLossConditionType::Ti設置eBased;
    Ti設置eLi設置it.Res使ltType = E基本inLossRes使lt::D本aw;
    Ti設置eLi設置it.P本io本ity = E基本inLossP本io本ity::Low;
    Ti設置eLi設置it.bIsC本iticalCondition = false;
    Ti設置eLi設置it.CheckInte本正al = 30.0f;
    
    軍基本inLossConditionPa本a設置ete本 Ga設置eTi設置e;
    Ga設置eTi設置e.Pa本a設置ete本的a設置e = TEXT("成a設置e下ti設置e");
    Ga設置eTi設置e.Pa本a設置ete本Val使e = TEXT("3600"); // 1小時
    Ga設置eTi設置e.Co設置pa本isonOpe本ato本(TEXT(">"));
    Ga設置eTi設置e.bIsReq使i本ed = t本使e;
    Ga設置eTi設置e.基本ei成ht = 1.0f;
    
    Ti設置eLi設置it.Pa本a設置ete本s.Add(Ga設置eTi設置e);
    
    Re成iste本基本inLossCondition(Ti設置eLi設置it);
}

正oid UMin成基本inLossCondition::C本eateSco本eBasedConditions()
{
    // 分數勝利條件
    軍基本inLossCondition Sco本eVicto本y;
    Sco本eVicto本y.ConditionID = TEXT("sco本e下正icto本y");
    Sco本eVicto本y.Condition的a設置e = TEXT("分數勝利");
    Sco本eVicto本y.ConditionDesc本iption = TEXT("達到勝利分數");
    Sco本eVicto本y.ConditionType = E基本inLossConditionType::Sco本eBased;
    Sco本eVicto本y.Res使ltType = E基本inLossRes使lt::Victo本y;
    Sco本eVicto本y.P本io本ity = E基本inLossP本io本ity::Medi使設置;
    Sco本eVicto本y.bIsC本iticalCondition = false;
    Sco本eVicto本y.CheckInte本正al = 5.0f;
    
    軍基本inLossConditionPa本a設置ete本 Victo本ySco本e;
    Victo本ySco本e.Pa本a設置ete本的a設置e = TEXT("正icto本y下sco本e");
    Victo本ySco本e.Pa本a設置ete本Val使e = TEXT("1000");
    Victo本ySco本e.Co設置pa本isonOpe本ato本(TEXT(">="));
    Victo本ySco本e.bIsReq使i本ed = t本使e;
    Victo本ySco本e.基本ei成ht = 1.0f;
    
    Sco本eVicto本y.Pa本a設置ete本s.Add(Victo本ySco本e);
    
    Re成iste本基本inLossCondition(Sco本eVicto本y);
}

// === 條件檢查方法 ===

bool UMin成基本inLossCondition::CheckMilita本yCondition(const 軍基本inLossCondition& Condition)
{
    fo本 (const 軍基本inLossConditionPa本a設置ete本& Pa本a設置ete本 : Condition.Pa本a設置ete本s)
    {
        軍St本in成 Val使e = GetGa設置eStateVal使e(Pa本a設置ete本.Pa本a設置ete本的a設置e);
        
        if (!Co設置pa本eVal使es(Val使e, Pa本a設置ete本.Co設置pa本isonOpe本ato本, Pa本a設置ete本.Pa本a設置ete本Val使e))
        {
            if (Pa本a設置ete本.bIsReq使i本ed)
            {
                本et使本n false;
            }
        }
    }
    
    本et使本n t本使e;
}

bool UMin成基本inLossCondition::CheckPoliticalCondition(const 軍基本inLossCondition& Condition)
{
    fo本 (const 軍基本inLossConditionPa本a設置ete本& Pa本a設置ete本 : Condition.Pa本a設置ete本s)
    {
        軍St本in成 Val使e = GetGa設置eStateVal使e(Pa本a設置ete本.Pa本a設置ete本的a設置e);
        
        if (!Co設置pa本eVal使es(Val使e, Pa本a設置ete本.Co設置pa本isonOpe本ato本, Pa本a設置ete本.Pa本a設置ete本Val使e))
        {
            if (Pa本a設置ete本.bIsReq使i本ed)
            {
                本et使本n false;
            }
        }
    }
    
    本et使本n t本使e;
}

bool UMin成基本inLossCondition::CheckEcono設置icCondition(const 軍基本inLossCondition& Condition)
{
    fo本 (const 軍基本inLossConditionPa本a設置ete本& Pa本a設置ete本 : Condition.Pa本a設置ete本s)
    {
        軍St本in成 Val使e = GetGa設置eStateVal使e(Pa本a設置ete本.Pa本a設置ete本的a設置e);
        
        if (!Co設置pa本eVal使es(Val使e, Pa本a設置ete本.Co設置pa本isonOpe本ato本, Pa本a設置ete本.Pa本a設置ete本Val使e))
        {
            if (Pa本a設置ete本.bIsReq使i本ed)
            {
                本et使本n false;
            }
        }
    }
    
    本et使本n t本使e;
}

bool UMin成基本inLossCondition::CheckTe本本ito本ialCondition(const 軍基本inLossCondition& Condition)
{
    fo本 (const 軍基本inLossConditionPa本a設置ete本& Pa本a設置ete本 : Condition.Pa本a設置ete本s)
    {
        軍St本in成 Val使e = GetGa設置eStateVal使e(Pa本a設置ete本.Pa本a設置ete本的a設置e);
        
        if (!Co設置pa本eVal使es(Val使e, Pa本a設置ete本.Co設置pa本isonOpe本ato本, Pa本a設置ete本.Pa本a設置ete本Val使e))
        {
            if (Pa本a設置ete本.bIsReq使i本ed)
            {
                本et使本n false;
            }
        }
    }
    
    本et使本n t本使e;
}

bool UMin成基本inLossCondition::CheckTi設置eBasedCondition(const 軍基本inLossCondition& Condition)
{
    fo本 (const 軍基本inLossConditionPa本a設置ete本& Pa本a設置ete本 : Condition.Pa本a設置ete本s)
    {
        軍St本in成 Val使e = GetGa設置eStateVal使e(Pa本a設置ete本.Pa本a設置ete本的a設置e);
        
        if (!Co設置pa本eVal使es(Val使e, Pa本a設置ete本.Co設置pa本isonOpe本ato本, Pa本a設置ete本.Pa本a設置ete本Val使e))
        {
            if (Pa本a設置ete本.bIsReq使i本ed)
            {
                本et使本n false;
            }
        }
    }
    
    本et使本n t本使e;
}

bool UMin成基本inLossCondition::CheckSco本eBasedCondition(const 軍基本inLossCondition& Condition)
{
    fo本 (const 軍基本inLossConditionPa本a設置ete本& Pa本a設置ete本 : Condition.Pa本a設置ete本s)
    {
        軍St本in成 Val使e = GetGa設置eStateVal使e(Pa本a設置ete本.Pa本a設置ete本的a設置e);
        
        if (!Co設置pa本eVal使es(Val使e, Pa本a設置ete本.Co設置pa本isonOpe本ato本, Pa本a設置ete本.Pa本a設置ete本Val使e))
        {
            if (Pa本a設置ete本.bIsReq使i本ed)
            {
                本et使本n false;
            }
        }
    }
    
    本et使本n t本使e;
}

bool UMin成基本inLossCondition::CheckC使sto設置Condition(const 軍基本inLossCondition& Condition)
{
    // 簡化實作：基於參數檢查
    fo本 (const 軍基本inLossConditionPa本a設置ete本& Pa本a設置ete本 : Condition.Pa本a設置ete本s)
    {
        軍St本in成 Val使e = GetGa設置eStateVal使e(Pa本a設置ete本.Pa本a設置ete本的a設置e);
        
        if (!Co設置pa本eVal使es(Val使e, Pa本a設置ete本.Co設置pa本isonOpe本ato本, Pa本a設置ete本.Pa本a設置ete本Val使e))
        {
            if (Pa本a設置ete本.bIsReq使i本ed)
            {
                本et使本n false;
            }
        }
    }
    
    本et使本n t本使e;
}

float UMin成基本inLossCondition::E正al使ateConditionSatisfaction(const 軍基本inLossCondition& Condition)
{
    float Satisfaction = 0.0f;
    int32 Req使i本edCo使nt = 0;
    int32 SatisfiedCo使nt = 0;
    
    fo本 (const 軍基本inLossConditionPa本a設置ete本& Pa本a設置ete本 : Condition.Pa本a設置ete本s)
    {
        if (Pa本a設置ete本.bIsReq使i本ed)
        {
            Req使i本edCo使nt++;
            
            軍St本in成 Val使e = GetGa設置eStateVal使e(Pa本a設置ete本.Pa本a設置ete本的a設置e);
            if (Co設置pa本eVal使es(Val使e, Pa本a設置ete本.Co設置pa本isonOpe本ato本, Pa本a設置ete本.Pa本a設置ete本Val使e))
            {
                SatisfiedCo使nt++;
                Satisfaction += Pa本a設置ete本.基本ei成ht;
            }
        }
    }
    
    if (Req使i本edCo使nt > 0)
    {
        Satisfaction /= Req使i本edCo使nt;
    }
    
    本et使本n 軍Math::Cla設置p(Satisfaction, 0.0f, 1.0f);
}

正oid UMin成基本inLossCondition::Calc使late基本inLossSco本es(軍基本inLossE正al使ationRes使lt& Res使lt)
{
    // 計算總分
    Res使lt.TotalSco本e = Res使lt.Victo本ySco本e + Res使lt.DefeatSco本e + Res使lt.D本awSco本e;
    
    // 正規化分數
    float TotalSco本e = Res使lt.TotalSco本e;
    if (TotalSco本e > 0.0f)
    {
        Res使lt.Victo本ySco本e /= TotalSco本e;
        Res使lt.DefeatSco本e /= TotalSco本e;
        Res使lt.D本awSco本e /= TotalSco本e;
    }
}

E基本inLossRes使lt UMin成基本inLossCondition::Dete本設置ine軍inalRes使lt(const 軍基本inLossE正al使ationRes使lt& Res使lt)
{
    // 檢查關鍵條件
    if (CheckC本iticalConditions())
    {
        // 如果有關鍵條件滿足，根據類型確定結果
        fo本 (const 軍St本in成& ConditionID : Res使lt.SatisfiedConditions)
        {
            const 軍基本inLossCondition* Condition = ConditionMap.軍ind(ConditionID);
            if (Condition && Condition->bIsC本iticalCondition)
            {
                本et使本n Condition->Res使ltType;
            }
        }
    }
    
    // 基於分數確定結果
    if (Res使lt.Victo本ySco本e >= 0.7f)
    {
        本et使本n E基本inLossRes使lt::Victo本y;
    }
    else if (Res使lt.DefeatSco本e >= 0.7f)
    {
        本et使本n E基本inLossRes使lt::Defeat;
    }
    else if (Res使lt.D本awSco本e >= 0.5f)
    {
        本et使本n E基本inLossRes使lt::D本aw;
    }
    else
    {
        本et使本n E基本inLossRes使lt::InP本o成本ess;
    }
}

正oid UMin成基本inLossCondition::UpdateStatistics(const 軍基本inLossE正al使ationRes使lt& Res使lt)
{
    Statistics.TotalE正al使ations++;
    
    switch (Res使lt.Res使lt)
    {
    case E基本inLossRes使lt::Victo本y:
        Statistics.Victo本yCo使nt++;
        b本eak;
    case E基本inLossRes使lt::Defeat:
        Statistics.DefeatCo使nt++;
        b本eak;
    case E基本inLossRes使lt::D本aw:
        Statistics.D本awCo使nt++;
        b本eak;
    defa使lt:
        b本eak;
    }
    
    // 計算勝率
    if (Statistics.TotalE正al使ations > 0)
    {
        Statistics.基本inRate = (float)Statistics.Victo本yCo使nt / Statistics.TotalE正al使ations;
    }
    
    // 計算平均評估時間
    float TotalTi設置e = Statistics.A正e本a成eE正al使ationTi設置e * (Statistics.TotalE正al使ations - 1) + 
                      (軍Platfo本設置Ti設置e::Seconds() - Res使lt.E正al使ationTi設置e);
    Statistics.A正e本a成eE正al使ationTi設置e = TotalTi設置e / Statistics.TotalE正al使ations;
    
    // 計算條件滿足率
    int32 TotalConditions = Re成iste本edConditions.的使設置();
    int32 SatisfiedConditions = Res使lt.SatisfiedConditions.的使設置();
    if (TotalConditions > 0)
    {
        Statistics.ConditionSatisfactionRate = (float)SatisfiedConditions / TotalConditions;
    }
}

正oid UMin成基本inLossCondition::的otify基本inLossChan成e(E基本inLossRes使lt OldRes使lt, E基本inLossRes使lt 的ewRes使lt)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本in-loss 本es使lt chan成ed: %s -> %s"), 
        *UEn使設置::GetVal使eAsSt本in成(OldRes使lt), *UEn使設置::GetVal使eAsSt本in成(的ewRes使lt));
    
    // 這裡可以添加事件通知邏輯
}

軍St本in成 UMin成基本inLossCondition::GetGa設置eStateVal使e(const 軍St本in成& Key) const
{
    軍St本in成* Val使e = Ga設置eState.軍ind(Key);
    本et使本n Val使e 基本 *Val使e : TEXT("0");
}

bool UMin成基本inLossCondition::Co設置pa本eVal使es(const 軍St本in成& Val使e1, const 軍St本in成& Ope本ato本, const 軍St本in成& Val使e2) const
{
    float 的使設置Val使e1 = 軍CSt本in成::Atof(*Val使e1);
    float 的使設置Val使e2 = 軍CSt本in成::Atof(*Val使e2);
    
    if (Ope本ato本 == TEXT(">"))
        本et使本n 的使設置Val使e1 > 的使設置Val使e2;
    else if (Ope本ato本 == TEXT("<"))
        本et使本n 的使設置Val使e1 < 的使設置Val使e2;
    else if (Ope本ato本 == TEXT("=="))
        本et使本n 軍Math::Is的ea本lyEq使al(的使設置Val使e1, 的使設置Val使e2);
    else if (Ope本ato本 == TEXT(">="))
        本et使本n 的使設置Val使e1 >= 的使設置Val使e2;
    else if (Ope本ato本 == TEXT("<="))
        本et使本n 的使設置Val使e1 <= 的使設置Val使e2;
    else if (Ope本ato本 == TEXT("contains"))
        本et使本n Val使e1.Contains(Val使e2);
    
    本et使本n false;
}

bool UMin成基本inLossCondition::ValidateConditionConfi成(const 軍基本inLossCondition& Condition) const
{
    if (Condition.ConditionID.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Condition.Pa本a設置ete本s.的使設置() == 0)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成基本inLossCondition::Lo成基本inLossE正ent(const 軍St本in成& Messa成e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[基本inLoss] %s"), *Messa成e);
}

bool UMin成基本inLossCondition::CheckC本iticalConditions()
{
    fo本 (const 軍基本inLossCondition& Condition : Re成iste本edConditions)
    {
        if (Condition.bIsC本iticalCondition && Condition.State == E基本inLossConditionState::Satisfied)
        {
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

正oid UMin成基本inLossCondition::輸入andleGa設置eO正e本(E基本inLossRes使lt Res使lt, const 軍St本in成& Reason)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e O正e本: %s - %s"), *UEn使設置::GetVal使eAsSt本in成(Res使lt), *Reason);
    
    // 這裡可以添加遊戲結束處理邏輯
}
