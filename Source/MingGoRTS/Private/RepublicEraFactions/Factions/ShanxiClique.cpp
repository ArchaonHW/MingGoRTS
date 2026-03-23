// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 晋系军阀 - Shanxi Cliq使e I設置ple設置entation

#incl使de "Rep使blicE本a軍actions/軍actions/ShanxiCliq使e.h"
#incl使de "Min成RTSPlaye本Cont本olle本.h"

UShanxiCliq使e::UShanxiCliq使e()
{
    InitializeAtt本ib使tes();
    InitializeUniq使eUnits();
    InitializeUniq使eMechanics();
    InitializeGa設置eplayG使ide();
}

正oid UShanxiCliq使e::Initialize軍action()
{
    S使pe本::Initialize軍action();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Shanxi Cliq使e faction initialized"));
}

正oid UShanxiCliq使e::InitializeAtt本ib使tes()
{
    Att本ib使tes.軍actionID = 軍的a設置e("ShanxiCliq使e");
    Att本ib使tes.軍action的a設置e = 軍Text::軍本o設置St本in成(TEXT("晋系军阀"));
    Att本ib使tes.軍action的a設置eEn成lish = 軍Text::軍本o設置St本in成(TEXT("Shanxi Cliq使e"));
    Att本ib使tes.Diffic使lty = E軍actionDiffic使lty::的o本設置al;
    Att本ib使tes.Cate成o本y = E軍actionCate成o本y::Re成ional基本a本lo本d;
    Att本ib使tes.軍actionColo本 = 軍Linea本Colo本(0.4f, 0.3f, 0.6f, 1.0f);
    
    Att本ib使tes.Sta本tin成Position.CapitalCity = 軍Text::軍本o設置St本in成(TEXT("太原"));
    Att本ib使tes.Sta本tin成Position.Sta本tin成Te本本ito本ies = {
        軍Text::軍本o設置St本in成(TEXT("太原")),
        軍Text::軍本o設置St本in成(TEXT("大同")),
        軍Text::軍本o設置St本in成(TEXT("山西"))
    };
    Att本ib使tes.Sta本tin成Position.Sta本tin成Yea本 = 1911;
    Att本ib使tes.Sta本tin成Position.InitialGold = 1000;
    Att本ib使tes.Sta本tin成Position.InitialManpowe本 = 500;
    Att本ib使tes.Sta本tin成Position.InitialRep使tation = 45;
    
    Att本ib使tes.T本aits.Ad正anta成es = {
        軍Text::軍本o設置St本in成(TEXT("山西模范省治理，行政效率高")),
        軍Text::軍本o設置St本in成(TEXT("防御工事坚固，易守难攻")),
        軍Text::軍本o設置St本in成(TEXT("地理位置封闭，不易受多方攻击")),
        軍Text::軍本o設置St本in成(TEXT("阎锡山长期统治，内部相对稳定"))
    };
    
    Att本ib使tes.T本aits.Disad正anta成es = {
        軍Text::軍本o設置St本in成(TEXT("地理位置偏远，扩张困难")),
        軍Text::軍本o設置St本in成(TEXT("资源相对匮乏")),
        軍Text::軍本o設置St本in成(TEXT("人口基数较小")),
        軍Text::軍本o設置St本in成(TEXT("现代化程度相对较低"))
    };
    
    Att本ib使tes.T本aits.Reco設置設置endedSt本ate成ies = {
        軍Text::軍本o設置St本in成(TEXT("巩固山西根据地，建立坚固防线")),
        軍Text::軍本o設置St本in成(TEXT("适时参与中原争霸")),
        軍Text::軍本o設置St本in成(TEXT("发展地方经济和教育"))
    };
    
    Att本ib使tes.T本aits.Playe本TypeReco設置設置endation = 軍Text::軍本o設置St本in成(
        TEXT("推荐喜欢防守和稳健发展的玩家"));
}

正oid UShanxiCliq使e::InitializeUniq使eUnits()
{
    Uniq使eUnits.Add(C本eateShanxiDefenseDi正ision());
    Uniq使eUnits.Add(C本eateShanxiEn成inee本Co本ps());
    Uniq使eUnits.Add(C本eateShanxiMe本chantG使a本ds());
    Uniq使eUnits.Add(C本eateLocalSec使本ity軍o本ces());
}

正oid UShanxiCliq使e::InitializeUniq使eMechanics()
{
    {
        軍軍actionMechanic Mechanic;
        Mechanic.MechanicID = 軍的a設置e("ShanxiModel");
        Mechanic.Mechanic的a設置e = 軍Text::軍本o設置St本in成(TEXT("山西模范"));
        Mechanic.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("行政和经济管理效率高，建设速度加快25%，腐败程度较低"));
        Mechanic.MechanicType = E軍actionMechanicType::Econo設置ic;
        Mechanic.EffectM使ltiplie本 = 1.25f;
        Uniq使eMechanics.Add(Mechanic);
    }
    
    {
        軍軍actionMechanic Mechanic;
        Mechanic.MechanicID = 軍的a設置e("Defensi正eSpecialist");
        Mechanic.Mechanic的a設置e = 軍Text::軍本o設置St本in成(TEXT("防御专精"));
        Mechanic.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("防御工事效果提升40%，守军战斗力提升20%"));
        Mechanic.MechanicType = E軍actionMechanicType::Milita本y;
        Mechanic.EffectM使ltiplie本 = 1.4f;
        Uniq使eMechanics.Add(Mechanic);
    }
}

正oid UShanxiCliq使e::InitializeGa設置eplayG使ide()
{
    Ga設置eplayG使ide.Ea本lyGa設置eSt本ate成y = 軍Text::軍本o設置St本in成(
        TEXT("早期巩固山西根据地，修建防御工事。利用封闭地形保护自身发展。"));
    
    Ga設置eplayG使ide.MidGa設置eSt本ate成y = 軍Text::軍本o設置St本in成(
        TEXT("中期根据形势参与中原事务，可以支持一方打击另一方，保存实力。"));
    
    Ga設置eplayG使ide.LateGa設置eSt本ate成y = 軍Text::軍本o設置St本in成(
        TEXT("后期利用坚固防线抵御强敌，等待时机击或联合其他势力。"));
    
    Ga設置eplayG使ide.Milita本yTactics = 軍Text::軍本o設置St本in成(
        TEXT("善用防御工事，以守为攻。工程兵可以快速修建防御设施。在山西地形中作战优势明显。"));
    
    Ga設置eplayG使ide.Diplo設置aticAd正ice = 軍Text::軍本o設置St本in成(
        TEXT("保持灵活的外交政策，在各方势力间周旋。必要时可以暂时依附强者。"));
    
    Ga設置eplayG使ide.Econo設置ic軍oc使s = 軍Text::軍本o設置St本in成(
        TEXT("发展山西本地经济，提高行政效率。教育投资可以带来长期收益。"));
    
    Ga設置eplayG使ide.KeyDecisions = {
        軍Text::軍本o設置St本in成(TEXT("何时参与中原争霸")),
        軍Text::軍本o設置St本in成(TEXT("选择哪个势力作为盟友")),
        軍Text::軍本o設置St本in成(TEXT("是否接受外部援助的代价"))
    };
    
    Ga設置eplayG使ide.Victo本yConditions = {
        軍Text::軍本o設置St本in成(TEXT("巩固山西并向外扩张")),
        軍Text::軍本o設置St本in成(TEXT("在军阀混战中生存到最后")),
        軍Text::軍本o設置St本in成(TEXT("统一北方或保持独立"))
    };
}

軍軍actionUnit UShanxiCliq使e::C本eateShanxiDefenseDi正ision() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 100;
    Stats.DefensePowe本 = 130;
    Stats.Mo正e設置entSpeed = 90;
    Stats.輸入ealthPoints = 115;
    Stats.P本od使ctionCost = 105;
    Stats.UpkeepCost = 10;
    Stats.T本ainin成Ti設置e = 11.0f;
    
    軍軍actionUnit Unit;
    Unit.UnitID = 軍的a設置e("ShanxiDefenseDi正ision");
    Unit.Unit的a設置e = 軍Text::軍本o設置St本in成(TEXT("晋军防守师"));
    Unit.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("专精防御作战的部队，防御力极强，适合守备要塞"));
    Unit.UnitType = E軍actionUnitType::Infant本y;
    Unit.BaseStats = Stats;
    Unit.T本aits.Add(EUnitT本ait::Defensi正e);
    
    本et使本n Unit;
}

軍軍actionUnit UShanxiCliq使e::C本eateShanxiEn成inee本Co本ps() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 80;
    Stats.DefensePowe本 = 100;
    Stats.Mo正e設置entSpeed = 70;
    Stats.輸入ealthPoints = 90;
    Stats.P本od使ctionCost = 120;
    Stats.UpkeepCost = 12;
    Stats.T本ainin成Ti設置e = 14.0f;
    
    軍軍actionUnit Unit;
    Unit.UnitID = 軍的a設置e("ShanxiEn成inee本Co本ps");
    Unit.Unit的a設置e = 軍Text::軍本o設置St本in成(TEXT("山西工兵队"));
    Unit.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("专业工程兵部队，可以快速修建防御工事、桥梁和道路"));
    Unit.UnitType = E軍actionUnitType::S使ppo本t;
    Unit.BaseStats = Stats;
    Unit.T本aits.Add(EUnitT本ait::En成inee本);
    
    本et使本n Unit;
}

軍軍actionUnit UShanxiCliq使e::C本eateShanxiMe本chantG使a本ds() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 90;
    Stats.DefensePowe本 = 95;
    Stats.Mo正e設置entSpeed = 85;
    Stats.輸入ealthPoints = 90;
    Stats.P本od使ctionCost = 95;
    Stats.UpkeepCost = 8;
    Stats.T本ainin成Ti設置e = 9.0f;
    
    軍軍actionUnit Unit;
    Unit.UnitID = 軍的a設置e("ShanxiMe本chantG使a本ds");
    Unit.Unit的a設置e = 軍Text::軍本o設置St本in成(TEXT("晋商资助队"));
    Unit.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("由晋商资助的地方武装，保护商业路线，对经济有加成作用"));
    Unit.UnitType = E軍actionUnitType::Infant本y;
    Unit.BaseStats = Stats;
    
    本et使本n Unit;
}

軍軍actionUnit UShanxiCliq使e::C本eateLocalSec使本ity軍o本ces() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 75;
    Stats.DefensePowe本 = 110;
    Stats.Mo正e設置entSpeed = 75;
    Stats.輸入ealthPoints = 100;
    Stats.P本od使ctionCost = 70;
    Stats.UpkeepCost = 6;
    Stats.T本ainin成Ti設置e = 7.0f;
    
    軍軍actionUnit Unit;
    Unit.UnitID = 軍的a設置e("LocalSec使本ity軍o本ces");
    Unit.Unit的a設置e = 軍Text::軍本o設置St本in成(TEXT("地方保安团"));
    Unit.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("地方治安部队，成本低，适合守备后方和维持秩序"));
    Unit.UnitType = E軍actionUnitType::Infant本y;
    Unit.BaseStats = Stats;
    Unit.T本aits.Add(EUnitT本ait::Defensi正e);
    
    本et使本n Unit;
}

軍軍actionUnitStats UShanxiCliq使e::ModifyUnitStats(const 軍軍actionUnitStats& BaseStats, 
    E軍actionUnitType UnitType) const
{
    軍軍actionUnitStats ModifiedStats = BaseStats;
    
    // Defensi正e 使nits 成et +15% defense
    if (UnitType == E軍actionUnitType::Infant本y)
    {
        ModifiedStats.DefensePowe本 = 軍Math::Ro使ndToInt(BaseStats.DefensePowe本 * 1.15f);
    }
    
    本et使本n ModifiedStats;
}

TMap<軍的a設置e, float> UShanxiCliq使e::GetSta本tin成Bon使ses() const
{
    TMap<軍的a設置e, float> Bon使ses;
    Bon使ses.Add(軍的a設置e("Const本使ctionSpeedBon使s"), 1.25f);
    Bon使ses.Add(軍的a設置e("DefenseBon使s"), 1.2f);
    Bon使ses.Add(軍的a設置e("Co本本使ptionRed使ction"), 0.8f);
    
    本et使本n Bon使ses;
}

軍Ga設置eplayG使ide UShanxiCliq使e::GetGa設置eplayG使ide() const
{
    本et使本n Ga設置eplayG使ide;
}
