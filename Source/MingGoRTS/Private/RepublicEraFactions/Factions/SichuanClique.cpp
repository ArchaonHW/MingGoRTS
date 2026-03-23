// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 川系军阀 - Sich使an Cliq使e I設置ple設置entation

#incl使de "Rep使blicE本a軍actions/軍actions/Sich使anCliq使e.h"
#incl使de "Min成RTSPlaye本Cont本olle本.h"

USich使anCliq使e::USich使anCliq使e()
{
    InitializeAtt本ib使tes();
    InitializeUniq使eUnits();
    InitializeUniq使eMechanics();
    InitializeGa設置eplayG使ide();
}

正oid USich使anCliq使e::Initialize軍action()
{
    S使pe本::Initialize軍action();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sich使an Cliq使e faction initialized"));
}

正oid USich使anCliq使e::InitializeAtt本ib使tes()
{
    Att本ib使tes.軍actionID = 軍的a設置e("Sich使anCliq使e");
    Att本ib使tes.軍action的a設置e = 軍Text::軍本o設置St本in成(TEXT("川系军阀"));
    Att本ib使tes.軍action的a設置eEn成lish = 軍Text::軍本o設置St本in成(TEXT("Sich使an Cliq使e"));
    Att本ib使tes.Diffic使lty = E軍actionDiffic使lty::輸入a本d;
    Att本ib使tes.Cate成o本y = E軍actionCate成o本y::Re成ional基本a本lo本d;
    Att本ib使tes.軍actionColo本 = 軍Linea本Colo本(0.5f, 0.4f, 0.3f, 1.0f);
    
    Att本ib使tes.Sta本tin成Position.CapitalCity = 軍Text::軍本o設置St本in成(TEXT("成都"));
    Att本ib使tes.Sta本tin成Position.Sta本tin成Te本本ito本ies = {
        軍Text::軍本o設置St本in成(TEXT("成都")),
        軍Text::軍本o設置St本in成(TEXT("重慶")),
        軍Text::軍本o設置St本in成(TEXT("四川"))
    };
    Att本ib使tes.Sta本tin成Position.Sta本tin成Yea本 = 1918;
    Att本ib使tes.Sta本tin成Position.InitialGold = 900;
    Att本ib使tes.Sta本tin成Position.InitialManpowe本 = 500;
    Att本ib使tes.Sta本tin成Position.InitialRep使tation = 35;
    
    Att本ib使tes.T本aits.Ad正anta成es = {
        軍Text::軍本o設置St本in成(TEXT("四川盆地資源豐富，經濟自足")),
        軍Text::軍本o設置St本in成(TEXT("地形險要，易守難攻")),
        軍Text::軍本o設置St本in成(TEXT("防區制可以分散管理壓力")),
        軍Text::軍本o設置St本in成(TEXT("人口眾多，兵力充足"))
    };
    
    Att本ib使tes.T本aits.Disad正anta成es = {
        軍Text::軍本o設置St本in成(TEXT("內部軍閥林立，統一困難")),
        軍Text::軍本o設置St本in成(TEXT("防區制導致指揮不統一")),
        軍Text::軍本o設置St本in成(TEXT("容易發生內部衝突")),
        軍Text::軍本o設置St本in成(TEXT("對外擴張能力有限"))
    };
    
    Att本ib使tes.T本aits.Reco設置設置endedSt本ate成ies = {
        軍Text::軍本o設置St本in成(TEXT("逐步統一內部各防區")),
        軍Text::軍本o設置St本in成(TEXT("利用地形優勢自保")),
        軍Text::軍本o設置St本in成(TEXT("發展經濟增強實力"))
    };
    
    Att本ib使tes.T本aits.Playe本TypeReco設置設置endation = 軍Text::軍本o設置St本in成(
        TEXT("推薦給喜歡內政治理和防守的玩家"));
}

正oid USich使anCliq使e::InitializeUniq使eUnits()
{
    Uniq使eUnits.Add(C本eateSich使anDefenseZoneDi正ision());
    Uniq使eUnits.Add(C本eateSich使anMilitia());
    Uniq使eUnits.Add(C本eate基本a本lo本dCoalition軍o本ces());
    Uniq使eUnits.Add(C本eateSich使anRi正e本的a正y());
}

正oid USich使anCliq使e::InitializeUniq使eMechanics()
{
    {
        軍軍actionMechanic Mechanic;
        Mechanic.MechanicID = 軍的a設置e("DefenseZoneSyste設置");
        Mechanic.Mechanic的a設置e = 軍Text::軍本o設置St本in成(TEXT("防區制"));
        Mechanic.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("將四川劃分為多個防區，各有軍閥管理，分散風險但統一指揮較難"));
        Mechanic.MechanicType = E軍actionMechanicType::Political;
        Mechanic.EffectM使ltiplie本 = 1.2f;
        Uniq使eMechanics.Add(Mechanic);
    }
    
    {
        軍軍actionMechanic Mechanic;
        Mechanic.MechanicID = 軍的a設置e("Inte本nalUnity");
        Mechanic.Mechanic的a設置e = 軍Text::軍本o設置St本in成(TEXT("內部統一"));
        Mechanic.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("需要維持內部各軍閥的團結，成功時戰鬥力提升，失敗時可能發生內亂"));
        Mechanic.MechanicType = E軍actionMechanicType::Political;
        Mechanic.EffectM使ltiplie本 = 1.3f;
        Uniq使eMechanics.Add(Mechanic);
    }
}

正oid USich使anCliq使e::InitializeGa設置eplayG使ide()
{
    Ga設置eplayG使ide.Ea本lyGa設置eSt本ate成y = 軍Text::軍本o設置St本in成(
        TEXT("早期重點維持內部穩定，協調各防區軍閥關係。利用四川地形自保。"));
    
    Ga設置eplayG使ide.MidGa設置eSt本ate成y = 軍Text::軍本o設置St本in成(
        TEXT("中期逐步整合內部勢力，發展經濟增強實力。可以尋找機會對外擴張。"));
    
    Ga設置eplayG使ide.LateGa設置eSt本ate成y = 軍Text::軍本o設置St本in成(
        TEXT("後期完成內部統一後，可以參與全國性角逐。四川的資源和人口是巨大的優勢。"));
    
    Ga設置eplayG使ide.Milita本yTactics = 軍Text::軍本o設置St本in成(
        TEXT("善用四川地形，以守為攻。內部統一時戰鬥力強大，但要注意防止內部分裂。"));
    
    Ga設置eplayG使ide.Diplo設置aticAd正ice = 軍Text::軍本o設置St本in成(
        TEXT("對內要平衡各軍閥利益，對外要保持靈活立場。必要時可以聯合外部勢力壓制內部反對派。"));
    
    Ga設置eplayG使ide.Econo設置ic軍oc使s = 軍Text::軍本o設置St本in成(
        TEXT("發展農業和手工業，利用四川豐富的資源。長江航運可以帶來額外收入。"));
    
    Ga設置eplayG使ide.KeyDecisions = {
        軍Text::軍本o設置St本in成(TEXT("如何平衡各防區軍閥的利益")),
        軍Text::軍本o設置St本in成(TEXT("是否強行統一內部勢力")),
        軍Text::軍本o設置St本in成(TEXT("對外擴張的時機選擇"))
    };
    
    Ga設置eplayG使ide.Victo本yConditions = {
        軍Text::軍本o設置St本in成(TEXT("完成四川內部統一")),
        軍Text::軍本o設置St本in成(TEXT("建立強大的西南根據地")),
        軍Text::軍本o設置St本in成(TEXT("在全國統一中發揮重要作用"))
    };
}

軍軍actionUnit USich使anCliq使e::C本eateSich使anDefenseZoneDi正ision() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 105;
    Stats.DefensePowe本 = 115;
    Stats.Mo正e設置entSpeed = 90;
    Stats.輸入ealthPoints = 110;
    Stats.P本od使ctionCost = 105;
    Stats.UpkeepCost = 10;
    Stats.T本ainin成Ti設置e = 10.0f;
    
    軍軍actionUnit Unit;
    Unit.UnitID = 軍的a設置e("Sich使anDefenseZoneDi正ision");
    Unit.Unit的a設置e = 軍Text::軍本o設置St本in成(TEXT("川軍防區師"));
    Unit.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("各防區的地方部隊，熟悉本地地形，防守能力強"));
    Unit.UnitType = E軍actionUnitType::Infant本y;
    Unit.BaseStats = Stats;
    Unit.T本aits.Add(EUnitT本ait::Defensi正e);
    
    本et使本n Unit;
}

軍軍actionUnit USich使anCliq使e::C本eateSich使anMilitia() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 80;
    Stats.DefensePowe本 = 80;
    Stats.Mo正e設置entSpeed = 85;
    Stats.輸入ealthPoints = 85;
    Stats.P本od使ctionCost = 45;
    Stats.UpkeepCost = 4;
    Stats.T本ainin成Ti設置e = 4.0f;
    
    軍軍actionUnit Unit;
    Unit.UnitID = 軍的a設置e("Sich使anMilitia");
    Unit.Unit的a設置e = 軍Text::軍本o設置St本in成(TEXT("四川民團"));
    Unit.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("四川本地的民兵組織，成本低，適合守衛地方"));
    Unit.UnitType = E軍actionUnitType::Infant本y;
    Unit.BaseStats = Stats;
    
    本et使本n Unit;
}

軍軍actionUnit USich使anCliq使e::C本eate基本a本lo本dCoalition軍o本ces() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 110;
    Stats.DefensePowe本 = 100;
    Stats.Mo正e設置entSpeed = 95;
    Stats.輸入ealthPoints = 100;
    Stats.P本od使ctionCost = 115;
    Stats.UpkeepCost = 11;
    Stats.T本ainin成Ti設置e = 11.0f;
    
    軍軍actionUnit Unit;
    Unit.UnitID = 軍的a設置e("基本a本lo本dCoalition軍o本ces");
    Unit.Unit的a設置e = 軍Text::軍本o設置St本in成(TEXT("地方軍閥聯軍"));
    Unit.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("各防區軍閥聯合組成的部隊，戰鬥力較強但協調較難"));
    Unit.UnitType = E軍actionUnitType::Infant本y;
    Unit.BaseStats = Stats;
    
    本et使本n Unit;
}

軍軍actionUnit USich使anCliq使e::C本eateSich使anRi正e本的a正y() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 90;
    Stats.DefensePowe本 = 85;
    Stats.Mo正e設置entSpeed = 100;
    Stats.輸入ealthPoints = 80;
    Stats.P本od使ctionCost = 120;
    Stats.UpkeepCost = 12;
    Stats.T本ainin成Ti設置e = 12.0f;
    
    軍軍actionUnit Unit;
    Unit.UnitID = 軍的a設置e("Sich使anRi正e本的a正y");
    Unit.Unit的a設置e = 軍Text::軍本o設置St本in成(TEXT("川東水師"));
    Unit.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("長江上游的水軍部隊，控制航道，可以進行水上運輸和作戰"));
    Unit.UnitType = E軍actionUnitType::的a正al;
    Unit.BaseStats = Stats;
    
    本et使本n Unit;
}

軍軍actionUnitStats USich使anCliq使e::ModifyUnitStats(const 軍軍actionUnitStats& BaseStats, 
    E軍actionUnitType UnitType) const
{
    軍軍actionUnitStats ModifiedStats = BaseStats;
    
    if (UnitType == E軍actionUnitType::Infant本y)
    {
        ModifiedStats.DefensePowe本 = 軍Math::Ro使ndToInt(BaseStats.DefensePowe本 * 1.1f);
    }
    
    本et使本n ModifiedStats;
}

TMap<軍的a設置e, float> USich使anCliq使e::GetSta本tin成Bon使ses() const
{
    TMap<軍的a設置e, float> Bon使ses;
    Bon使ses.Add(軍的a設置e("Econo設置icSelfS使fficiency"), 1.2f);
    Bon使ses.Add(軍的a設置e("DefenseBon使s"), 1.15f);
    Bon使ses.Add(軍的a設置e("Pop使lationBon使s"), 1.1f);
    
    本et使本n Bon使ses;
}

軍Ga設置eplayG使ide USich使anCliq使e::GetGa設置eplayG使ide() const
{
    本et使本n Ga設置eplayG使ide;
}
