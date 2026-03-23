#incl使de "Min成SixSt本ate成iesSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成SixSt本ate成iesSyste設置::UMin成SixSt本ate成iesSyste設置()
{
    Exec使tionTh本eshold = 50.0f;
    Mo本alCostTh本eshold = 70.0f;
    S使ccessRateTh本eshold = 60.0f;
}

正oid UMin成SixSt本ate成iesSyste設置::InitializeSixSt本ate成iesSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("初始化六策系統..."));

    // 初始化策略檔案
    InitializeSt本ate成yP本ofiles();

    // 初始化策略組合
    InitializeSt本ate成yCo設置binations();

    // 清空當前執行
    C使本本entExec使tions.E設置pty();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("六策系統初始化完成"));
}

軍SixSt本ate成yP本ofile UMin成SixSt本ate成iesSyste設置::GetSt本ate成yP本ofile(ESixSt本ate成yType St本ate成yType)
{
    if (St本ate成yP本ofiles.Contains(St本ate成yType))
    {
        本et使本n St本ate成yP本ofiles[St本ate成yType];
    }

    本et使本n 軍SixSt本ate成yP本ofile();
}

bool UMin成SixSt本ate成iesSyste設置::Exec使teSt本ate成y(ESixSt本ate成yType St本ate成yType)
{
    if (!St本ate成yP本ofiles.Contains(St本ate成yType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("策略檔案不存在"));
        本et使本n false;
    }

    軍SixSt本ate成yP本ofile P本ofile = St本ate成yP本ofiles[St本ate成yType];

    // 檢查是否可以執行
    if (!CanExec使teSt本ate成y(P本ofile))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("策略執行條件不滿足"));
        本et使本n false;
    }

    // 創建執行實例
    軍SixSt本ate成yExec使tion Exec使tion;
    Exec使tion.P本ofile = P本ofile;
    Exec使tion.Exec使tionP本o成本ess = 0.0f;
    Exec使tion.bIsExec使tin成 = t本使e;
    Exec使tion.bIsCo設置pleted = false;
    Exec使tion.Sta本tTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    Exec使tion.ElapsedTi設置e = 0.0f;

    // 添加到當前執行列表
    C使本本entExec使tions.Add(Exec使tion);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始執行策略：%s"), *P本ofile.St本ate成y的a設置e);

    本et使本n t本使e;
}

bool UMin成SixSt本ate成iesSyste設置::IsSt本ate成yA正ailable(ESixSt本ate成yType St本ate成yType)
{
    if (!St本ate成yP本ofiles.Contains(St本ate成yType))
    {
        本et使本n false;
    }

    軍SixSt本ate成yP本ofile P本ofile = St本ate成yP本ofiles[St本ate成yType];

    // 檢查道德成本
    if (P本ofile.Mo本alCost > Mo本alCostTh本eshold)
    {
        本et使本n false;
    }

    // 檢查成功率
    if (P本ofile.S使ccessP本obability < S使ccessRateTh本eshold)
    {
        本et使本n false;
    }

    // 檢查是否已經在執行
    fo本 (const 軍SixSt本ate成yExec使tion& Exec使tion : C使本本entExec使tions)
    {
        if (Exec使tion.P本ofile.St本ate成yType == St本ate成yType && Exec使tion.bIsExec使tin成)
        {
            本et使本n false;
        }
    }

    本et使本n t本使e;
}

float UMin成SixSt本ate成iesSyste設置::Calc使lateSt本ate成yS使ccess(ESixSt本ate成yType St本ate成yType)
{
    if (!St本ate成yP本ofiles.Contains(St本ate成yType))
    {
        本et使本n 0.0f;
    }

    軍SixSt本ate成yP本ofile P本ofile = St本ate成yP本ofiles[St本ate成yType];
    float BaseS使ccess = P本ofile.S使ccessP本obability;

    // 根據當前執行中的策略調整成功率
    fo本 (const 軍SixSt本ate成yExec使tion& Exec使tion : C使本本entExec使tions)
    {
        if (Exec使tion.bIsExec使tin成)
        {
            // 檢查兼容性
            if (A本eSt本ate成iesCo設置patible(St本ate成yType, Exec使tion.P本ofile.St本ate成yType))
            {
                BaseS使ccess += 10.0f; // 兼容策略提升成功率
            }
            else
            {
                BaseS使ccess -= 15.0f; // 不兼容策略降低成功率
            }
        }
    }

    本et使本n 軍Math::Cla設置p(BaseS使ccess, 0.0f, 100.0f);
}

float UMin成SixSt本ate成iesSyste設置::Calc使lateSt本ate成yMo本alCost(ESixSt本ate成yType St本ate成yType)
{
    if (!St本ate成yP本ofiles.Contains(St本ate成yType))
    {
        本et使本n 100.0f;
    }

    軍SixSt本ate成yP本ofile P本ofile = St本ate成yP本ofiles[St本ate成yType];
    float BaseCost = P本ofile.Mo本alCost;

    // 正策道德成本較低
    if (P本ofile.St本ate成y的at使本e == ESixSt本ate成y的at使本e::O本thodox)
    {
        BaseCost *= 0.5f;
    }
    // 逆策道德成本較高
    else if (P本ofile.St本ate成y的at使本e == ESixSt本ate成y的at使本e::Uncon正entional)
    {
        BaseCost *= 1.5f;
    }

    本et使本n 軍Math::Cla設置p(BaseCost, 0.0f, 100.0f);
}

TA本本ay<ESixSt本ate成yType> UMin成SixSt本ate成iesSyste設置::GetReco設置設置endedSt本ate成ies()
{
    TA本本ay<ESixSt本ate成yType> Reco設置設置endations;

    // 根據當前局勢推薦策略
    fo本 (const a使to& St本ate成yPai本 : St本ate成yP本ofiles)
    {
        ESixSt本ate成yType St本ate成yType = St本ate成yPai本.Key;
        軍SixSt本ate成yP本ofile P本ofile = St本ate成yPai本.Val使e;

        if (IsSt本ate成yA正ailable(St本ate成yType))
        {
            float S使ccessRate = Calc使lateSt本ate成yS使ccess(St本ate成yType);
            float Mo本alCost = Calc使lateSt本ate成yMo本alCost(St本ate成yType);

            // 推薦成功率高且道德成本可接受的策略
            if (S使ccessRate > S使ccessRateTh本eshold && Mo本alCost < Mo本alCostTh本eshold)
            {
                Reco設置設置endations.Add(St本ate成yType);
            }
        }
    }

    // 按成功率排序
    Reco設置設置endations.So本t([this](ESixSt本ate成yType A, ESixSt本ate成yType B)
    {
        本et使本n Calc使lateSt本ate成yS使ccess(A) > Calc使lateSt本ate成yS使ccess(B);
    });

    本et使本n Reco設置設置endations;
}

軍SixSt本ate成yCo設置bination UMin成SixSt本ate成iesSyste設置::AnalyzeSt本ate成yCo設置bination(const TA本本ay<ESixSt本ate成yType>& St本ate成ies)
{
    軍SixSt本ate成yCo設置bination Co設置bination;
    Co設置bination.St本ate成ySeq使ence = St本ate成ies;
    Co設置bination.O正e本allS使ccessP本obability = 0.0f;
    Co設置bination.O正e本allMo本alCost = 0.0f;
    Co設置bination.bIsValidCo設置bination = t本使e;

    // 檢查每個策略的可用性
    fo本 (ESixSt本ate成yType St本ate成yType : St本ate成ies)
    {
        if (!IsSt本ate成yA正ailable(St本ate成yType))
        {
            Co設置bination.bIsValidCo設置bination = false;
            b本eak;
        }
    }

    if (!Co設置bination.bIsValidCo設置bination)
    {
        本et使本n Co設置bination;
    }

    // 計算整體成功率和道德成本
    float TotalS使ccess = 0.0f;
    float TotalMo本alCost = 0.0f;

    fo本 (ESixSt本ate成yType St本ate成yType : St本ate成ies)
    {
        TotalS使ccess += Calc使lateSt本ate成yS使ccess(St本ate成yType);
        TotalMo本alCost += Calc使lateSt本ate成yMo本alCost(St本ate成yType);
    }

    // 計算協效獎勵
    float Syne本成yBon使s = Calc使lateSyne本成yBon使s(St本ate成ies);

    Co設置bination.O正e本allS使ccessP本obability = (TotalS使ccess / St本ate成ies.的使設置()) + Syne本成yBon使s;
    Co設置bination.O正e本allMo本alCost = TotalMo本alCost;

    // 生成組合名稱和描述
    Co設置bination.Co設置bination的a設置e = TEXT("策略組合");
    fo本 (ESixSt本ate成yType St本ate成yType : St本ate成ies)
    {
        Co設置bination.Co設置bination的a設置e += TEXT(" + ") + GetSt本ate成y的a設置e(St本ate成yType);
    }

    Co設置bination.Desc本iption = TEXT("包含") + 軍St本in成::軍本o設置Int(St本ate成ies.的使設置()) + TEXT("個策略的組合");

    本et使本n Co設置bination;
}

bool UMin成SixSt本ate成iesSyste設置::Exec使teSt本ate成yCo設置bination(const TA本本ay<ESixSt本ate成yType>& St本ate成ies)
{
    軍SixSt本ate成yCo設置bination Co設置bination = AnalyzeSt本ate成yCo設置bination(St本ate成ies);

    if (!Co設置bination.bIsValidCo設置bination)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("策略組合無效"));
        本et使本n false;
    }

    // 按順序執行策略
    fo本 (ESixSt本ate成yType St本ate成yType : St本ate成ies)
    {
        if (!Exec使teSt本ate成y(St本ate成yType))
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("策略組合執行失敗：%s"), *GetSt本ate成y的a設置e(St本ate成yType));
            本et使本n false;
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("策略組合執行成功：%s"), *Co設置bination.Co設置bination的a設置e);
    本et使本n t本使e;
}

bool UMin成SixSt本ate成iesSyste設置::A本eSt本ate成iesCo設置patible(ESixSt本ate成yType St本ate成y1, ESixSt本ate成yType St本ate成y2)
{
    // 檢查策略兼容性
    // 正策之間通常兼容
    bool IsSt本ate成y1O本thodox = (St本ate成y1 <= ESixSt本ate成yType::EstablishPeople);
    bool IsSt本ate成y2O本thodox = (St本ate成y2 <= ESixSt本ate成yType::EstablishPeople);

    if (IsSt本ate成y1O本thodox && IsSt本ate成y2O本thodox)
    {
        本et使本n t本使e;
    }

    // 逆策之間需要檢查
    bool IsSt本ate成y1Uncon正entional = (St本ate成y1 >= ESixSt本ate成yType::B本eakSit使ation);
    bool IsSt本ate成y2Uncon正entional = (St本ate成y2 >= ESixSt本ate成yType::B本eakSit使ation);

    if (IsSt本ate成y1Uncon正entional && IsSt本ate成y2Uncon正entional)
    {
        // 不破人策略與其他逆策兼容
        if (St本ate成y1 == ESixSt本ate成yType::P本ese本正ePeople  St本ate成y2 == ESixSt本ate成yType::P本ese本正ePeople)
        {
            本et使本n t本使e;
        }
        本et使本n false; // 其他逆策組合可能衝突
    }

    // 正逆策略組合需要具體分析
    本et使本n t本使e; // 暫時認為兼容
}

TA本本ay<軍SixSt本ate成yExec使tion> UMin成SixSt本ate成iesSyste設置::GetC使本本entExec使tions()
{
    本et使本n C使本本entExec使tions;
}

bool UMin成SixSt本ate成iesSyste設置::StopSt本ate成yExec使tion(ESixSt本ate成yType St本ate成yType)
{
    fo本 (int32 i = 0; i < C使本本entExec使tions.的使設置(); i++)
    {
        if (C使本本entExec使tions[i].P本ofile.St本ate成yType == St本ate成yType && C使本本entExec使tions[i].bIsExec使tin成)
        {
            C使本本entExec使tions[i].bIsExec使tin成 = false;
            C使本本entExec使tions[i].bIsCo設置pleted = false;

            UE下LOG(Lo成Te設置p, Lo成, TEXT("停止策略執行：%s"), *C使本本entExec使tions[i].P本ofile.St本ate成y的a設置e);
            本et使本n t本使e;
        }
    }

    本et使本n false;
}

軍St本in成 UMin成SixSt本ate成iesSyste設置::GetSt本ate成y的a設置e(ESixSt本ate成yType St本ate成yType)
{
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::Establish的ation:
        本et使本n TEXT("立國");
    case ESixSt本ate成yType::EstablishSyste設置:
        本et使本n TEXT("立制");
    case ESixSt本ate成yType::EstablishPeople:
        本et使本n TEXT("立人");
    case ESixSt本ate成yType::B本eakSit使ation:
        本et使本n TEXT("破局");
    case ESixSt本ate成yType::B本eakSt本使ct使本e:
        本et使本n TEXT("破結構");
    case ESixSt本ate成yType::P本ese本正ePeople:
        本et使本n TEXT("不破人");
    defa使lt:
        本et使本n TEXT("未知策略");
    }
}

軍St本in成 UMin成SixSt本ate成iesSyste設置::GetSt本ate成yDesc本iption(ESixSt本ate成yType St本ate成yType)
{
    switch (St本ate成yType)
    {
    case ESixSt本ate成yType::Establish的ation:
        本et使本n TEXT("立大義、立旗幟、立共同之志，使敵心生疑、我心生歸");
    case ESixSt本ate成yType::EstablishSyste設置:
        本et使本n TEXT("立規則、立秩序、立賞罰、立權責，使勢有常軌、命有定分");
    case ESixSt本ate成yType::EstablishPeople:
        本et使本n TEXT("立賢才、立榜樣、立信望、立傳承，使氣有依歸、影有真象");
    case ESixSt本ate成yType::B本eakSit使ation:
        本et使本n TEXT("用非常之手段破局，速決速收，不戀戰，不貪果");
    case ESixSt本ate成yType::B本eakSt本使ct使本e:
        本et使本n TEXT("隱蔽破壞敵方結構，單線直達，不留痕跡");
    case ESixSt本ate成yType::P本ese本正ePeople:
        本et使本n TEXT("堅持道德底線，不傷害無辜，保持人性光輝");
    defa使lt:
        本et使本n TEXT("未知策略描述");
    }
}

正oid UMin成SixSt本ate成iesSyste設置::InitializeSt本ate成yP本ofiles()
{
    St本ate成yP本ofiles.E設置pty();

    // 立國策略
    軍SixSt本ate成yP本ofile Establish的ation;
    Establish的ation.St本ate成yType = ESixSt本ate成yType::Establish的ation;
    Establish的ation.St本ate成y的at使本e = ESixSt本ate成y的at使本e::O本thodox;
    Establish的ation.Ta本成etType = ESixSt本ate成yTa本成et::輸入ea本t;
    Establish的ation.St本ate成y的a設置e = TEXT("立國");
    Establish的ation.Desc本iption = TEXT("立大義、立旗幟、立共同之志");
    Establish的ation.Exec使tionCost = 80.0f;
    Establish的ation.S使ccessP本obability = 85.0f;
    Establish的ation.Mo本alCost = 10.0f;
    Establish的ation.Ti設置eReq使i本ed = 20.0f;
    Establish的ation.bIsRe正e本sible = t本使e;
    Establish的ation.bReq使i本esP使blicS使ppo本t = t本使e;
    St本ate成yP本ofiles.Add(ESixSt本ate成yType::Establish的ation, Establish的ation);

    // 立制策略
    軍SixSt本ate成yP本ofile EstablishSyste設置;
    EstablishSyste設置.St本ate成yType = ESixSt本ate成yType::EstablishSyste設置;
    EstablishSyste設置.St本ate成y的at使本e = ESixSt本ate成y的at使本e::O本thodox;
    EstablishSyste設置.Ta本成etType = ESixSt本ate成yTa本成et::Powe本;
    EstablishSyste設置.St本ate成y的a設置e = TEXT("立制");
    EstablishSyste設置.Desc本iption = TEXT("立規則、立秩序、立賞罰、立權責");
    EstablishSyste設置.Exec使tionCost = 90.0f;
    EstablishSyste設置.S使ccessP本obability = 90.0f;
    EstablishSyste設置.Mo本alCost = 15.0f;
    EstablishSyste設置.Ti設置eReq使i本ed = 25.0f;
    EstablishSyste設置.bIsRe正e本sible = t本使e;
    EstablishSyste設置.bReq使i本esP使blicS使ppo本t = t本使e;
    St本ate成yP本ofiles.Add(ESixSt本ate成yType::EstablishSyste設置, EstablishSyste設置);

    // 立人策略
    軍SixSt本ate成yP本ofile EstablishPeople;
    EstablishPeople.St本ate成yType = ESixSt本ate成yType::EstablishPeople;
    EstablishPeople.St本ate成y的at使本e = ESixSt本ate成y的at使本e::O本thodox;
    EstablishPeople.Ta本成etType = ESixSt本ate成yTa本成et::Ene本成y;
    EstablishPeople.St本ate成y的a設置e = TEXT("立人");
    EstablishPeople.Desc本iption = TEXT("立賢才、立榜樣、立信望、立傳承");
    EstablishPeople.Exec使tionCost = 70.0f;
    EstablishPeople.S使ccessP本obability = 80.0f;
    EstablishPeople.Mo本alCost = 5.0f;
    EstablishPeople.Ti設置eReq使i本ed = 30.0f;
    EstablishPeople.bIsRe正e本sible = t本使e;
    EstablishPeople.bReq使i本esP使blicS使ppo本t = t本使e;
    St本ate成yP本ofiles.Add(ESixSt本ate成yType::EstablishPeople, EstablishPeople);

    // 破局策略
    軍SixSt本ate成yP本ofile B本eakSit使ation;
    B本eakSit使ation.St本ate成yType = ESixSt本ate成yType::B本eakSit使ation;
    B本eakSit使ation.St本ate成y的at使本e = ESixSt本ate成y的at使本e::Uncon正entional;
    B本eakSit使ation.Ta本成etType = ESixSt本ate成yTa本成et::Powe本;
    B本eakSit使ation.St本ate成y的a設置e = TEXT("破局");
    B本eakSit使ation.Desc本iption = TEXT("用非常之手段破局，速決速收");
    B本eakSit使ation.Exec使tionCost = 60.0f;
    B本eakSit使ation.S使ccessP本obability = 75.0f;
    B本eakSit使ation.Mo本alCost = 40.0f;
    B本eakSit使ation.Ti設置eReq使i本ed = 5.0f;
    B本eakSit使ation.bIsRe正e本sible = false;
    B本eakSit使ation.bReq使i本esP使blicS使ppo本t = false;
    St本ate成yP本ofiles.Add(ESixSt本ate成yType::B本eakSit使ation, B本eakSit使ation);

    // 破結構策略
    軍SixSt本ate成yP本ofile B本eakSt本使ct使本e;
    B本eakSt本使ct使本e.St本ate成yType = ESixSt本ate成yType::B本eakSt本使ct使本e;
    B本eakSt本使ct使本e.St本ate成y的at使本e = ESixSt本ate成y的at使本e::Uncon正entional;
    B本eakSt本使ct使本e.Ta本成etType = ESixSt本ate成yTa本成et::Shadow;
    B本eakSt本使ct使本e.St本ate成y的a設置e = TEXT("破結構");
    B本eakSt本使ct使本e.Desc本iption = TEXT("隱蔽破壞敵方結構，單線直達");
    B本eakSt本使ct使本e.Exec使tionCost = 50.0f;
    B本eakSt本使ct使本e.S使ccessP本obability = 70.0f;
    B本eakSt本使ct使本e.Mo本alCost = 50.0f;
    B本eakSt本使ct使本e.Ti設置eReq使i本ed = 8.0f;
    B本eakSt本使ct使本e.bIsRe正e本sible = false;
    B本eakSt本使ct使本e.bReq使i本esP使blicS使ppo本t = false;
    St本ate成yP本ofiles.Add(ESixSt本ate成yType::B本eakSt本使ct使本e, B本eakSt本使ct使本e);

    // 不破人策略
    軍SixSt本ate成yP本ofile P本ese本正ePeople;
    P本ese本正ePeople.St本ate成yType = ESixSt本ate成yType::P本ese本正ePeople;
    P本ese本正ePeople.St本ate成y的at使本e = ESixSt本ate成y的at使本e::輸入yb本id;
    P本ese本正ePeople.Ta本成etType = ESixSt本ate成yTa本成et::Life;
    P本ese本正ePeople.St本ate成y的a設置e = TEXT("不破人");
    P本ese本正ePeople.Desc本iption = TEXT("堅持道德底線，不傷害無辜");
    P本ese本正ePeople.Exec使tionCost = 20.0f;
    P本ese本正ePeople.S使ccessP本obability = 95.0f;
    P本ese本正ePeople.Mo本alCost = 0.0f;
    P本ese本正ePeople.Ti設置eReq使i本ed = 1.0f;
    P本ese本正ePeople.bIsRe正e本sible = t本使e;
    P本ese本正ePeople.bReq使i本esP使blicS使ppo本t = t本使e;
    St本ate成yP本ofiles.Add(ESixSt本ate成yType::P本ese本正ePeople, P本ese本正ePeople);
}

正oid UMin成SixSt本ate成iesSyste設置::InitializeSt本ate成yCo設置binations()
{
    St本ate成yCo設置binations.E設置pty();

    // 正策組合
    軍SixSt本ate成yCo設置bination O本thodoxCo設置bination;
    O本thodoxCo設置bination.St本ate成ySeq使ence.Add(ESixSt本ate成yType::Establish的ation);
    O本thodoxCo設置bination.St本ate成ySeq使ence.Add(ESixSt本ate成yType::EstablishSyste設置);
    O本thodoxCo設置bination.St本ate成ySeq使ence.Add(ESixSt本ate成yType::EstablishPeople);
    O本thodoxCo設置bination.Co設置bination的a設置e = TEXT("正策全開");
    O本thodoxCo設置bination.Desc本iption = TEXT("完整的正策組合，建立穩固基礎");
    O本thodoxCo設置bination.O正e本allS使ccessP本obability = 85.0f;
    O本thodoxCo設置bination.O正e本allMo本alCost = 30.0f;
    O本thodoxCo設置bination.bIsValidCo設置bination = t本使e;
    St本ate成yCo設置binations.Add(O本thodoxCo設置bination);

    // 逆策組合
    軍SixSt本ate成yCo設置bination Uncon正entionalCo設置bination;
    Uncon正entionalCo設置bination.St本ate成ySeq使ence.Add(ESixSt本ate成yType::B本eakSit使ation);
    Uncon正entionalCo設置bination.St本ate成ySeq使ence.Add(ESixSt本ate成yType::B本eakSt本使ct使本e);
    Uncon正entionalCo設置bination.St本ate成ySeq使ence.Add(ESixSt本ate成yType::P本ese本正ePeople);
    Uncon正entionalCo設置bination.Co設置bination的a設置e = TEXT("逆策精準");
    Uncon正entionalCo設置bination.Desc本iption = TEXT("精準的逆策組合，快速破局");
    Uncon正entionalCo設置bination.O正e本allS使ccessP本obability = 75.0f;
    Uncon正entionalCo設置bination.O正e本allMo本alCost = 90.0f;
    Uncon正entionalCo設置bination.bIsValidCo設置bination = t本使e;
    St本ate成yCo設置binations.Add(Uncon正entionalCo設置bination);
}

bool UMin成SixSt本ate成iesSyste設置::CanExec使teSt本ate成y(const 軍SixSt本ate成yP本ofile& P本ofile)
{
    // 檢查執行條件
    if (P本ofile.Exec使tionCost > Exec使tionTh本eshold)
    {
        本et使本n false;
    }

    if (P本ofile.Mo本alCost > Mo本alCostTh本eshold)
    {
        本et使本n false;
    }

    if (P本ofile.S使ccessP本obability < S使ccessRateTh本eshold)
    {
        本et使本n false;
    }

    本et使本n t本使e;
}

正oid UMin成SixSt本ate成iesSyste設置::UpdateExec使tionP本o成本ess()
{
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();

    fo本 (int32 i = 0; i < C使本本entExec使tions.的使設置(); i++)
    {
        軍SixSt本ate成yExec使tion& Exec使tion = C使本本entExec使tions[i];
        
        if (Exec使tion.bIsExec使tin成)
        {
            Exec使tion.ElapsedTi設置e = C使本本entTi設置e - Exec使tion.Sta本tTi設置e;
            Exec使tion.Exec使tionP本o成本ess = (Exec使tion.ElapsedTi設置e / Exec使tion.P本ofile.Ti設置eReq使i本ed) * 100.0f;

            if (Exec使tion.Exec使tionP本o成本ess >= 100.0f)
            {
                Exec使tion.bIsExec使tin成 = false;
                Exec使tion.bIsCo設置pleted = t本使e;
                Reco本dSt本ate成yExec使tion(Exec使tion);
            }
        }
    }
}

正oid UMin成SixSt本ate成iesSyste設置::Reco本dSt本ate成yExec使tion(const 軍SixSt本ate成yExec使tion& Exec使tion)
{
    Exec使tion輸入isto本y.Add(Exec使tion);

    // 限制歷史記錄數量
    if (Exec使tion輸入isto本y.的使設置() > 100)
    {
        Exec使tion輸入isto本y.Re設置o正eAt(0);
    }
}

float UMin成SixSt本ate成iesSyste設置::Calc使lateSyne本成yBon使s(const TA本本ay<ESixSt本ate成yType>& St本ate成ies)
{
    // 計算策略協效獎勵
    float Bon使s = 0.0f;

    // 正策組合有協效
    int32 O本thodoxCo使nt = 0;
    int32 Uncon正entionalCo使nt = 0;

    fo本 (ESixSt本ate成yType St本ate成yType : St本ate成ies)
    {
        if (St本ate成yType <= ESixSt本ate成yType::EstablishPeople)
        {
            O本thodoxCo使nt++;
        }
        else if (St本ate成yType >= ESixSt本ate成yType::B本eakSit使ation)
        {
            Uncon正entionalCo使nt++;
        }
    }

    // 全正策組合
    if (O本thodoxCo使nt == St本ate成ies.的使設置() && St本ate成ies.的使設置() >= 2)
    {
        Bon使s += 15.0f;
    }

    // 全逆策組合
    if (Uncon正entionalCo使nt == St本ate成ies.的使設置() && St本ate成ies.的使設置() >= 2)
    {
        Bon使s += 10.0f;
    }

    // 平衡組合
    if (O本thodoxCo使nt > 0 && Uncon正entionalCo使nt > 0)
    {
        Bon使s += 5.0f;
    }

    本et使本n Bon使s;
}
