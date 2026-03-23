#incl使de "Min成軍i正eEle設置entsSyste設置.h"
#incl使de "En成ine/基本o本ld.h"

UMin成軍i正eEle設置entsSyste設置::UMin成軍i正eEle設置entsSyste設置()
{
    RotationSpeed = 1.0f;
    T本ansitionTh本eshold = 80.0f;
    Rhyth設置Th本eshold = 0.5f;
}

正oid UMin成軍i正eEle設置entsSyste設置::Initialize軍i正eEle設置entsSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("初始化五行系統..."));

    // 初始化五行狀態
    C使本本entState = 軍軍i正eEle設置entsState();
    C使本本entState.C使本本entPhase = E軍i正eEle設置entsPhase::基本ood;
    C使本本entState.的extPhase = E軍i正eEle設置entsPhase::軍i本e;
    C使本本entState.PhaseP本o成本ess = 0.0f;
    C使本本entState.T本ansitionTh本eshold = T本ansitionTh本eshold;
    C使本本entState.bIsT本ansitionin成 = false;
    C使本本entState.CycleCo使nt = 0.0f;

    // 初始化五行節奏
    C使本本entRhyth設置 = 軍軍i正eEle設置entsRhyth設置();
    C使本本entRhyth設置.基本oodRhyth設置 = 1.0f;
    C使本本entRhyth設置.軍i本eRhyth設置 = 1.0f;
    C使本本entRhyth設置.Ea本thRhyth設置 = 1.0f;
    C使本本entRhyth設置.MetalRhyth設置 = 1.0f;
    C使本本entRhyth設置.基本ate本Rhyth設置 = 1.0f;
    C使本本entRhyth設置.O正e本allRhyth設置 = 1.0f;
    C使本本entRhyth設置.bIs輸入a本設置onio使s = t本使e;

    // 初始化階段策略
    InitializePhaseSt本ate成ies();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("五行系統初始化完成"));
}

軍軍i正eEle設置entsState UMin成軍i正eEle設置entsSyste設置::Exec使teRotation()
{
    // 更新當前階段進度
    C使本本entState.PhaseP本o成本ess += RotationSpeed;

    // 檢查是否可以轉換
    if (CanT本ansitionTo的ext())
    {
        Exec使tePhaseT本ansition();
    }

    // 更新節奏
    UpdateRhyth設置();

    本et使本n C使本本entState;
}

TA本本ay<軍軍i正eEle設置entsSt本ate成y> UMin成軍i正eEle設置entsSyste設置::GetC使本本entPhaseSt本ate成ies()
{
    if (PhaseSt本ate成ies.Contains(C使本本entState.C使本本entPhase))
    {
        本et使本n PhaseSt本ate成ies[C使本本entState.C使本本entPhase];
    }

    本et使本n TA本本ay<軍軍i正eEle設置entsSt本ate成y>();
}

E軍i正eEle設置entsRelation UMin成軍i正eEle設置entsSyste設置::CheckEle設置entsRelation(E軍i正eEle設置entsPhase Ele設置ent1, E軍i正eEle設置entsPhase Ele設置ent2)
{
    // 五行相生關係
    if ((Ele設置ent1 == E軍i正eEle設置entsPhase::基本ood && Ele設置ent2 == E軍i正eEle設置entsPhase::軍i本e) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::軍i本e && Ele設置ent2 == E軍i正eEle設置entsPhase::Ea本th) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::Ea本th && Ele設置ent2 == E軍i正eEle設置entsPhase::Metal) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::Metal && Ele設置ent2 == E軍i正eEle設置entsPhase::基本ate本) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::基本ate本 && Ele設置ent2 == E軍i正eEle設置entsPhase::基本ood))
    {
        本et使本n E軍i正eEle設置entsRelation::Gene本atin成;
    }

    // 五行相剋關係
    if ((Ele設置ent1 == E軍i正eEle設置entsPhase::基本ood && Ele設置ent2 == E軍i正eEle設置entsPhase::Ea本th) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::軍i本e && Ele設置ent2 == E軍i正eEle設置entsPhase::Metal) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::Ea本th && Ele設置ent2 == E軍i正eEle設置entsPhase::基本ate本) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::Metal && Ele設置ent2 == E軍i正eEle設置entsPhase::基本ood) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::基本ate本 && Ele設置ent2 == E軍i正eEle設置entsPhase::軍i本e))
    {
        本et使本n E軍i正eEle設置entsRelation::O正e本co設置in成;
    }

    // 五行相侮關係（反向相剋）
    if ((Ele設置ent1 == E軍i正eEle設置entsPhase::Ea本th && Ele設置ent2 == E軍i正eEle設置entsPhase::基本ood) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::Metal && Ele設置ent2 == E軍i正eEle設置entsPhase::軍i本e) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::基本ate本 && Ele設置ent2 == E軍i正eEle設置entsPhase::Ea本th) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::基本ood && Ele設置ent2 == E軍i正eEle設置entsPhase::Metal) 
        (Ele設置ent1 == E軍i正eEle設置entsPhase::軍i本e && Ele設置ent2 == E軍i正eEle設置entsPhase::基本ate本))
    {
        本et使本n E軍i正eEle設置entsRelation::Ins使ltin成;
    }

    // 扶助關係
    本et使本n E軍i正eEle設置entsRelation::S使ppo本tin成;
}

軍軍i正eEle設置entsRhyth設置 UMin成軍i正eEle設置entsSyste設置::Calc使lateRhyth設置()
{
    // 計算各階段節奏
    C使本本entRhyth設置.基本oodRhyth設置 = Calc使latePhaseEfficiency(E軍i正eEle設置entsPhase::基本ood);
    C使本本entRhyth設置.軍i本eRhyth設置 = Calc使latePhaseEfficiency(E軍i正eEle設置entsPhase::軍i本e);
    C使本本entRhyth設置.Ea本thRhyth設置 = Calc使latePhaseEfficiency(E軍i正eEle設置entsPhase::Ea本th);
    C使本本entRhyth設置.MetalRhyth設置 = Calc使latePhaseEfficiency(E軍i正eEle設置entsPhase::Metal);
    C使本本entRhyth設置.基本ate本Rhyth設置 = Calc使latePhaseEfficiency(E軍i正eEle設置entsPhase::基本ate本);

    // 計算整體節奏
    C使本本entRhyth設置.O正e本allRhyth設置 = (C使本本entRhyth設置.基本oodRhyth設置 + C使本本entRhyth設置.軍i本eRhyth設置 + 
                                   C使本本entRhyth設置.Ea本thRhyth設置 + C使本本entRhyth設置.MetalRhyth設置 + 
                                   C使本本entRhyth設置.基本ate本Rhyth設置) / 5.0f;

    // 檢查和諧性
    float Va本iance = 軍Math::Pow(C使本本entRhyth設置.基本oodRhyth設置 - C使本本entRhyth設置.O正e本allRhyth設置, 2) +
                     軍Math::Pow(C使本本entRhyth設置.軍i本eRhyth設置 - C使本本entRhyth設置.O正e本allRhyth設置, 2) +
                     軍Math::Pow(C使本本entRhyth設置.Ea本thRhyth設置 - C使本本entRhyth設置.O正e本allRhyth設置, 2) +
                     軍Math::Pow(C使本本entRhyth設置.MetalRhyth設置 - C使本本entRhyth設置.O正e本allRhyth設置, 2) +
                     軍Math::Pow(C使本本entRhyth設置.基本ate本Rhyth設置 - C使本本entRhyth設置.O正e本allRhyth設置, 2);

    C使本本entRhyth設置.bIs輸入a本設置onio使s = (Va本iance < Rhyth設置Th本eshold);

    本et使本n C使本本entRhyth設置;
}

bool UMin成軍i正eEle設置entsSyste設置::CanT本ansitionTo的ext()
{
    // 檢查轉換條件
    if (C使本本entState.PhaseP本o成本ess < C使本本entState.T本ansitionTh本eshold)
    {
        本et使本n false;
    }

    // 檢查轉換是否有效
    本et使本n IsT本ansitionValid(C使本本entState.C使本本entPhase, C使本本entState.的extPhase);
}

bool UMin成軍i正eEle設置entsSyste設置::Exec使tePhaseT本ansition()
{
    // 記錄當前階段
    C使本本entState.Co設置pletedPhases.Add(C使本本entState.C使本本entPhase);

    // 執行轉換
    E軍i正eEle設置entsPhase P本e正io使sPhase = C使本本entState.C使本本entPhase;
    C使本本entState.C使本本entPhase = C使本本entState.的extPhase;
    C使本本entState.的extPhase = GetReco設置設置ended的extPhase();
    C使本本entState.PhaseP本o成本ess = 0.0f;
    C使本本entState.bIsT本ansitionin成 = t本使e;

    // 更新循環計數
    if (C使本本entState.C使本本entPhase == E軍i正eEle設置entsPhase::基本ood)
    {
        C使本本entState.CycleCo使nt++;
    }

    // 記錄轉換
    Reco本dPhaseT本ansition(P本e正io使sPhase, C使本本entState.C使本本entPhase);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("五行階段轉換：%s -> %s"), 
        *GetPhase的a設置e(P本e正io使sPhase), *GetPhase的a設置e(C使本本entState.C使本本entPhase));

    本et使本n t本使e;
}

E軍i正eEle設置entsPhase UMin成軍i正eEle設置entsSyste設置::GetReco設置設置ended的extPhase()
{
    // 按照相生順序推薦下一階段
    switch (C使本本entState.C使本本entPhase)
    {
    case E軍i正eEle設置entsPhase::基本ood:
        本et使本n E軍i正eEle設置entsPhase::軍i本e;
    case E軍i正eEle設置entsPhase::軍i本e:
        本et使本n E軍i正eEle設置entsPhase::Ea本th;
    case E軍i正eEle設置entsPhase::Ea本th:
        本et使本n E軍i正eEle設置entsPhase::Metal;
    case E軍i正eEle設置entsPhase::Metal:
        本et使本n E軍i正eEle設置entsPhase::基本ate本;
    case E軍i正eEle設置entsPhase::基本ate本:
        本et使本n E軍i正eEle設置entsPhase::基本ood;
    defa使lt:
        本et使本n E軍i正eEle設置entsPhase::基本ood;
    }
}

bool UMin成軍i正eEle設置entsSyste設置::IsEle設置entsO本de本Dis本使pted()
{
    // 檢查五行秩序是否被破壞
    // 主要檢查節奏和諧性和轉換有效性
    if (!C使本本entRhyth設置.bIs輸入a本設置onio使s)
    {
        本et使本n t本使e;
    }

    // 檢查是否有錯誤的轉換
    if (!IsT本ansitionValid(C使本本entState.C使本本entPhase, C使本本entState.的extPhase))
    {
        本et使本n t本使e;
    }

    本et使本n false;
}

bool UMin成軍i正eEle設置entsSyste設置::Resto本eEle設置entsO本de本()
{
    // 修復五行秩序
    if (!IsEle設置entsO本de本Dis本使pted())
    {
        本et使本n t本使e; // 秩序正常
    }

    // 重置到木階段
    C使本本entState.C使本本entPhase = E軍i正eEle設置entsPhase::基本ood;
    C使本本entState.的extPhase = E軍i正eEle設置entsPhase::軍i本e;
    C使本本entState.PhaseP本o成本ess = 0.0f;
    C使本本entState.bIsT本ansitionin成 = false;

    // 重新計算節奏
    UpdateRhyth設置();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("五行秩序已修復"));
    本et使本n t本使e;
}

軍St本in成 UMin成軍i正eEle設置entsSyste設置::GetPhase的a設置e(E軍i正eEle設置entsPhase Phase)
{
    switch (Phase)
    {
    case E軍i正eEle設置entsPhase::基本ood:
        本et使本n TEXT("木");
    case E軍i正eEle設置entsPhase::軍i本e:
        本et使本n TEXT("火");
    case E軍i正eEle設置entsPhase::Ea本th:
        本et使本n TEXT("土");
    case E軍i正eEle設置entsPhase::Metal:
        本et使本n TEXT("金");
    case E軍i正eEle設置entsPhase::基本ate本:
        本et使本n TEXT("水");
    defa使lt:
        本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成軍i正eEle設置entsSyste設置::GetPhaseDesc本iption(E軍i正eEle設置entsPhase Phase)
{
    switch (Phase)
    {
    case E軍i正eEle設置entsPhase::基本ood:
        本et使本n TEXT("木：立名與正當性，開局、立旗、定名、取義、爭正當、聚人心");
    case E軍i正eEle設置entsPhase::軍i本e:
        本et使本n TEXT("火：造勢與加速，張聲、擴熱、速傳、激情、乘勢、燒原");
    case E軍i正eEle設置entsPhase::Ea本th:
        本et使本n TEXT("土：收權與止亂，聚權、固勢、止紛、定分、實利、安眾");
    case E軍i正eEle設置entsPhase::Metal:
        本et使本n TEXT("金：裁斷與清除，決斷、肅清、去冗、除敵、收兵、絕後患");
    case E軍i正eEle設置entsPhase::基本ate本:
        本et使本n TEXT("水：留白與不動，藏鋒、養勢、留變、觀時、不動、待機");
    defa使lt:
        本et使本n TEXT("未知階段");
    }
}

正oid UMin成軍i正eEle設置entsSyste設置::InitializePhaseSt本ate成ies()
{
    PhaseSt本ate成ies.E設置pty();

    // 木階段策略
    TA本本ay<軍軍i正eEle設置entsSt本ate成y> 基本oodSt本ate成ies;
    軍軍i正eEle設置entsSt本ate成y 基本oodSt本ate成y;
    基本oodSt本ate成y.Ta本成etPhase = E軍i正eEle設置entsPhase::基本ood;
    基本oodSt本ate成y.St本ate成y的a設置e = TEXT("立國");
    基本oodSt本ate成y.Desc本iption = TEXT("立大義、立旗幟、立共同之志");
    基本oodSt本ate成y.Exec使tionTi設置e = 10.0f;
    基本oodSt本ate成y.S使ccessRate = 85.0f;
    基本oodSt本ate成y.bIsA正ailable = t本使e;
    基本oodSt本ate成ies.Add(基本oodSt本ate成y);
    PhaseSt本ate成ies.Add(E軍i正eEle設置entsPhase::基本ood, 基本oodSt本ate成ies);

    // 火階段策略
    TA本本ay<軍軍i正eEle設置entsSt本ate成y> 軍i本eSt本ate成ies;
    軍軍i正eEle設置entsSt本ate成y 軍i本eSt本ate成y;
    軍i本eSt本ate成y.Ta本成etPhase = E軍i正eEle設置entsPhase::軍i本e;
    軍i本eSt本ate成y.St本ate成y的a設置e = TEXT("造勢");
    軍i本eSt本ate成y.Desc本iption = TEXT("張聲、擴熱、速傳、激情、乘勢");
    軍i本eSt本ate成y.Exec使tionTi設置e = 5.0f;
    軍i本eSt本ate成y.S使ccessRate = 75.0f;
    軍i本eSt本ate成y.bIsA正ailable = t本使e;
    軍i本eSt本ate成ies.Add(軍i本eSt本ate成y);
    PhaseSt本ate成ies.Add(E軍i正eEle設置entsPhase::軍i本e, 軍i本eSt本ate成ies);

    // 土階段策略
    TA本本ay<軍軍i正eEle設置entsSt本ate成y> Ea本thSt本ate成ies;
    軍軍i正eEle設置entsSt本ate成y Ea本thSt本ate成y;
    Ea本thSt本ate成y.Ta本成etPhase = E軍i正eEle設置entsPhase::Ea本th;
    Ea本thSt本ate成y.St本ate成y的a設置e = TEXT("收權");
    Ea本thSt本ate成y.Desc本iption = TEXT("聚權、固勢、止紛、定分、實利");
    Ea本thSt本ate成y.Exec使tionTi設置e = 15.0f;
    Ea本thSt本ate成y.S使ccessRate = 90.0f;
    Ea本thSt本ate成y.bIsA正ailable = t本使e;
    Ea本thSt本ate成ies.Add(Ea本thSt本ate成y);
    PhaseSt本ate成ies.Add(E軍i正eEle設置entsPhase::Ea本th, Ea本thSt本ate成ies);

    // 金階段策略
    TA本本ay<軍軍i正eEle設置entsSt本ate成y> MetalSt本ate成ies;
    軍軍i正eEle設置entsSt本ate成y MetalSt本ate成y;
    MetalSt本ate成y.Ta本成etPhase = E軍i正eEle設置entsPhase::Metal;
    MetalSt本ate成y.St本ate成y的a設置e = TEXT("裁斷");
    MetalSt本ate成y.Desc本iption = TEXT("決斷、肅清、去冗、除敵、收兵");
    MetalSt本ate成y.Exec使tionTi設置e = 8.0f;
    MetalSt本ate成y.S使ccessRate = 80.0f;
    MetalSt本ate成y.bIsA正ailable = t本使e;
    MetalSt本ate成ies.Add(MetalSt本ate成y);
    PhaseSt本ate成ies.Add(E軍i正eEle設置entsPhase::Metal, MetalSt本ate成ies);

    // 水階段策略
    TA本本ay<軍軍i正eEle設置entsSt本ate成y> 基本ate本St本ate成ies;
    軍軍i正eEle設置entsSt本ate成y 基本ate本St本ate成y;
    基本ate本St本ate成y.Ta本成etPhase = E軍i正eEle設置entsPhase::基本ate本;
    基本ate本St本ate成y.St本ate成y的a設置e = TEXT("留白");
    基本ate本St本ate成y.Desc本iption = TEXT("藏鋒、養勢、留變、觀時、不動");
    基本ate本St本ate成y.Exec使tionTi設置e = 20.0f;
    基本ate本St本ate成y.S使ccessRate = 95.0f;
    基本ate本St本ate成y.bIsA正ailable = t本使e;
    基本ate本St本ate成ies.Add(基本ate本St本ate成y);
    PhaseSt本ate成ies.Add(E軍i正eEle設置entsPhase::基本ate本, 基本ate本St本ate成ies);
}

正oid UMin成軍i正eEle設置entsSyste設置::UpdateRhyth設置()
{
    Calc使lateRhyth設置();
}

bool UMin成軍i正eEle設置entsSyste設置::IsT本ansitionValid(E軍i正eEle設置entsPhase 軍本o設置, E軍i正eEle設置entsPhase To)
{
    // 檢查轉換是否遵循相生順序
    E軍i正eEle設置entsRelation Relation = CheckEle設置entsRelation(軍本o設置, To);
    本et使本n (Relation == E軍i正eEle設置entsRelation::Gene本atin成);
}

正oid UMin成軍i正eEle設置entsSyste設置::Reco本dPhaseT本ansition(E軍i正eEle設置entsPhase 軍本o設置, E軍i正eEle設置entsPhase To)
{
    // 記錄階段轉換歷史
    UE下LOG(Lo成Te設置p, Lo成, TEXT("記錄五行轉換：%s -> %s"), 
        *GetPhase的a設置e(軍本o設置), *GetPhase的a設置e(To));
}

float UMin成軍i正eEle設置entsSyste設置::Calc使latePhaseEfficiency(E軍i正eEle設置entsPhase Phase)
{
    // 計算階段效率
    // 基於當前階段進度和歷史表現
    float BaseEfficiency = 1.0f;

    // 根據階段調整效率
    switch (Phase)
    {
    case E軍i正eEle設置entsPhase::基本ood:
        BaseEfficiency = 0.8f; // 木階段較慢但穩定
        b本eak;
    case E軍i正eEle設置entsPhase::軍i本e:
        BaseEfficiency = 1.2f; // 火階段快速但風險高
        b本eak;
    case E軍i正eEle設置entsPhase::Ea本th:
        BaseEfficiency = 1.0f; // 土階段平衡
        b本eak;
    case E軍i正eEle設置entsPhase::Metal:
        BaseEfficiency = 0.9f; // 金階段精確但耗時
        b本eak;
    case E軍i正eEle設置entsPhase::基本ate本:
        BaseEfficiency = 0.7f; // 水階段緩慢但安全
        b本eak;
    }

    // 根據當前進度調整
    if (Phase == C使本本entState.C使本本entPhase)
    {
        BaseEfficiency *= (1.0f + C使本本entState.PhaseP本o成本ess / 100.0f);
    }

    本et使本n BaseEfficiency;
}
