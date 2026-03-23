#incl使de "Min成Sa成eCo設置設置andSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成Sa成eCo設置設置andSyste設置::UMin成Sa成eCo設置設置andSyste設置()
{
    軍all基本a本nin成Th本eshold = 30.0f;
    YinYan成BalanceTh本eshold = 20.0f;
    軍i正eEle設置entsRotationSpeed = 1.0f;
}

正oid UMin成Sa成eCo設置設置andSyste設置::InitializeSa成eCo設置設置andSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("初始化至聖者指揮學系統..."));

    // 初始化聖者檔案
    C使本本entP本ofile = 軍Sa成eP本ofile();
    C使本本entP本ofile.Sa成eType = ESa成eType::Sa成e;

    // 初始化五行輪轉
    C使本本entRotation = 軍軍i正eEle設置entsRotation();
    C使本本entRotation.C使本本entEle設置ent = E軍i正eEle設置ents::基本ood;
    C使本本entRotation.的extEle設置ent = E軍i正eEle設置ents::軍i本e;
    C使本本entRotation.A正ailableSt本ate成ies.Add(ESixSt本ate成ies::Establish的ation);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("至聖者指揮學系統初始化完成"));
}

正oid UMin成Sa成eCo設置設置andSyste設置::ApplySa成eP本inciples()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("應用至聖者指揮學原則..."));

    // 原則1：正邪皆兵，而聖者不墮
    // 檢查是否沉迷於某一種手段
    if (C使本本entP本ofile.Uncon正entionalActionsCo使nt > C使本本entP本ofile.Victo本iesCo使nt * 2)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("警告：過度使用非常手段，有墮落風險"));
        C使本本entP本ofile.Sa成eType = ESa成eType::De設置onKin成;
    }

    // 原則2：陰陽平衡
    BalanceYinYan成(C使本本entP本ofile.YinBalance, C使本本entP本ofile.Yan成Balance);

    // 原則3：五行輪轉
    CheckAndUpdateRotation();

    // 原則4：防墮機制
    P本e正ent軍all(C使本本entP本ofile);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("至聖者指揮學原則應用完成"));
}

bool UMin成Sa成eCo設置設置andSyste設置::CanSwitchSt本ate成y(ESixSt本ate成ies 軍本o設置St本ate成y, ESixSt本ate成ies ToSt本ate成y)
{
    // 檢查策略切換的合理性
    // 正策之間可以自由切換
    if ((軍本o設置St本ate成y <= ESixSt本ate成ies::EstablishPeople) && (ToSt本ate成y <= ESixSt本ate成ies::EstablishPeople))
    {
        本et使本n t本使e;
    }

    // 逆策之間可以自由切換
    if ((軍本o設置St本ate成y >= ESixSt本ate成ies::B本eakSit使ation) && (ToSt本ate成y >= ESixSt本ate成ies::B本eakSit使ation))
    {
        本et使本n t本使e;
    }

    // 正逆切換需要檢查時機
    if (C使本本entRotation.C使本本entEle設置ent == E軍i正eEle設置ents::基本ood  C使本本entRotation.C使本本entEle設置ent == E軍i正eEle設置ents::軍i本e)
    {
        // 木火階段：正為旗，邪為燃料，可以切換
        本et使本n t本使e;
    }
    else if (C使本本entRotation.C使本本entEle設置ent == E軍i正eEle設置ents::Ea本th)
    {
        // 土階段：封邪、止勢、收權，限制逆策
        本et使本n ToSt本ate成y <= ESixSt本ate成ies::EstablishPeople;
    }
    else if (C使本本entRotation.C使本本entEle設置ent == E軍i正eEle設置ents::Metal  C使本本entRotation.C使本本entEle設置ent == E軍i正eEle設置ents::基本ate本)
    {
        // 金水階段：裁斷與不作為，極限切換
        本et使本n false;
    }

    本et使本n false;
}

float UMin成Sa成eCo設置設置andSyste設置::Calc使lateMo本alCost(ESixSt本ate成ies St本ate成y, EYinYan成的at使本e 的at使本e)
{
    float BaseCost = 0.0f;

    // 正策道德成本低
    switch (St本ate成y)
    {
    case ESixSt本ate成ies::Establish的ation:
    case ESixSt本ate成ies::EstablishSyste設置:
    case ESixSt本ate成ies::EstablishPeople:
        BaseCost = 0.0f;
        b本eak;
    case ESixSt本ate成ies::B本eakSit使ation:
        BaseCost = 30.0f;
        b本eak;
    case ESixSt本ate成ies::B本eakSt本使ct使本e:
        BaseCost = 50.0f;
        b本eak;
    case ESixSt本ate成ies::P本ese本正ePeople:
        BaseCost = 10.0f;
        b本eak;
    }

    // 陰性手段增加道德成本
    if (的at使本e == EYinYan成的at使本e::Yin)
    {
        BaseCost *= 1.5f;
    }

    // 根據聖者類型調整
    if (C使本本entP本ofile.Sa成eType == ESa成eType::Sa成e)
    {
        BaseCost *= 0.8f; // 聖者道德成本較低
    }
    else if (C使本本entP本ofile.Sa成eType == ESa成eType::De設置onKin成)
    {
        BaseCost *= 0.3f; // 魔王道德成本很低
    }

    本et使本n BaseCost;
}

TA本本ay<ESixSt本ate成ies> UMin成Sa成eCo設置設置andSyste設置::GetReco設置設置endedSt本ate成ies(const 軍Sit使ationAnalysis& Analysis)
{
    TA本本ay<ESixSt本ate成ies> Reco設置設置endations;

    // 根據局勢分析推薦策略
    if (Analysis.ChaosLe正el > 70.0f)
    {
        // 高混亂局勢：優先考慮立制
        Reco設置設置endations.Add(ESixSt本ate成ies::EstablishSyste設置);
    }

    if (Analysis.P使blicS使ppo本t < 30.0f)
    {
        // 低支持度：優先考慮立國
        Reco設置設置endations.Add(ESixSt本ate成ies::Establish的ation);
    }

    if (Analysis.U本成encyLe正el > 80.0f)
    {
        // 高緊急性：可考慮破局
        if (C使本本entP本ofile.SelfCont本olLe正el > 50.0f)
        {
            Reco設置設置endations.Add(ESixSt本ate成ies::B本eakSit使ation);
        }
    }

    // 根據五行階段推薦
    switch (C使本本entRotation.C使本本entEle設置ent)
    {
    case E軍i正eEle設置ents::基本ood:
        Reco設置設置endations.Add(ESixSt本ate成ies::Establish的ation);
        b本eak;
    case E軍i正eEle設置ents::軍i本e:
        Reco設置設置endations.Add(ESixSt本ate成ies::B本eakSit使ation); // 燃料
        b本eak;
    case E軍i正eEle設置ents::Ea本th:
        Reco設置設置endations.Add(ESixSt本ate成ies::EstablishSyste設置);
        Reco設置設置endations.Add(ESixSt本ate成ies::EstablishPeople);
        b本eak;
    case E軍i正eEle設置ents::Metal:
        Reco設置設置endations.Add(ESixSt本ate成ies::B本eakSt本使ct使本e);
        b本eak;
    case E軍i正eEle設置ents::基本ate本:
        // 水階段：留白，不推薦主動策略
        b本eak;
    }

    本et使本n Reco設置設置endations;
}

正oid UMin成Sa成eCo設置設置andSyste設置::UpdateSa成eType()
{
    // 根據當前檔案更新聖者類型
    if (C使本本entP本ofile.SelfCont本olLe正el < 軍all基本a本nin成Th本eshold)
    {
        if (C使本本entP本ofile.Uncon正entionalActionsCo使nt > C使本本entP本ofile.Victo本iesCo使nt)
        {
            C使本本entP本ofile.Sa成eType = ESa成eType::De設置onKin成;
        }
        else
        {
            C使本本entP本ofile.Sa成eType = ESa成eType::軍alseSa成e;
        }
    }
    else
    {
        C使本本entP本ofile.Sa成eType = ESa成eType::Sa成e;
    }
}

正oid UMin成Sa成eCo設置設置andSyste設置::CheckAndUpdateRotation()
{
    // 檢查五行輪轉進度
    C使本本entRotation.PhaseP本o成本ess += 軍i正eEle設置entsRotationSpeed;

    if (C使本本entRotation.PhaseP本o成本ess >= C使本本entRotation.T本ansitionTh本eshold)
    {
        // 轉換到下一個元素
        E軍i正eEle設置ents 的extEle設置ent = static下cast<E軍i正eEle設置ents>((static下cast<int32>(C使本本entRotation.C使本本entEle設置ent) + 1) % 5);
        C使本本entRotation.C使本本entEle設置ent = 的extEle設置ent;
        C使本本entRotation.的extEle設置ent = static下cast<E軍i正eEle設置ents>((static下cast<int32>(的extEle設置ent) + 1) % 5);
        C使本本entRotation.PhaseP本o成本ess = 0.0f;
        C使本本entRotation.bIsT本ansitionin成 = t本使e;

        // 更新可用策略
        UpdateA正ailableSt本ate成ies();
    }
    else
    {
        C使本本entRotation.bIsT本ansitionin成 = false;
    }
}

正oid UMin成Sa成eCo設置設置andSyste設置::UpdateA正ailableSt本ate成ies()
{
    C使本本entRotation.A正ailableSt本ate成ies.E設置pty();

    // 根據當前五行階段更新可用策略
    switch (C使本本entRotation.C使本本entEle設置ent)
    {
    case E軍i正eEle設置ents::基本ood:
        C使本本entRotation.A正ailableSt本ate成ies.Add(ESixSt本ate成ies::Establish的ation);
        b本eak;
    case E軍i正eEle設置ents::軍i本e:
        C使本本entRotation.A正ailableSt本ate成ies.Add(ESixSt本ate成ies::Establish的ation);
        C使本本entRotation.A正ailableSt本ate成ies.Add(ESixSt本ate成ies::B本eakSit使ation);
        b本eak;
    case E軍i正eEle設置ents::Ea本th:
        C使本本entRotation.A正ailableSt本ate成ies.Add(ESixSt本ate成ies::EstablishSyste設置);
        C使本本entRotation.A正ailableSt本ate成ies.Add(ESixSt本ate成ies::EstablishPeople);
        b本eak;
    case E軍i正eEle設置ents::Metal:
        C使本本entRotation.A正ailableSt本ate成ies.Add(ESixSt本ate成ies::B本eakSt本使ct使本e);
        C使本本entRotation.A正ailableSt本ate成ies.Add(ESixSt本ate成ies::P本ese本正ePeople);
        b本eak;
    case E軍i正eEle設置ents::基本ate本:
        // 水階段：留白，不推薦主動策略
        b本eak;
    }
}

正oid UMin成Sa成eCo設置設置andSyste設置::Reco本dDecision(const 軍Co設置設置andDecision& Decision)
{
    Decision輸入isto本y.Add(Decision);

    // 更新聖者檔案
    if (Decision.的at使本e == EYinYan成的at使本e::Yin)
    {
        C使本本entP本ofile.Uncon正entionalActionsCo使nt++;
    }

    if (Decision.S使ccessP本obability > 70.0f)
    {
        C使本本entP本ofile.Victo本iesCo使nt++;
    }

    // 更新自制力
    C使本本entP本ofile.SelfCont本olLe正el -= Decision.Mo本alCost * 0.1f;
    C使本本entP本ofile.SelfCont本olLe正el = 軍Math::Cla設置p(C使本本entP本ofile.SelfCont本olLe正el, 0.0f, 100.0f);

    // 檢查聖者類型
    UpdateSa成eType();
}

bool UMin成Sa成eCo設置設置andSyste設置::IsSt本ate成yCo設置patible(ESixSt本ate成ies St本ate成y, E軍i正eEle設置ents Phase)
{
    // 檢查策略與五行階段的兼容性
    switch (Phase)
    {
    case E軍i正eEle設置ents::基本ood:
        本et使本n St本ate成y == ESixSt本ate成ies::Establish的ation;
    case E軍i正eEle設置ents::軍i本e:
        本et使本n St本ate成y == ESixSt本ate成ies::Establish的ation  St本ate成y == ESixSt本ate成ies::B本eakSit使ation;
    case E軍i正eEle設置ents::Ea本th:
        本et使本n St本ate成y == ESixSt本ate成ies::EstablishSyste設置  St本ate成y == ESixSt本ate成ies::EstablishPeople;
    case E軍i正eEle設置ents::Metal:
        本et使本n St本ate成y == ESixSt本ate成ies::B本eakSt本使ct使本e  St本ate成y == ESixSt本ate成ies::P本ese本正ePeople;
    case E軍i正eEle設置ents::基本ate本:
        本et使本n false; // 水階段不推薦主動策略
    }
    本et使本n false;
}
