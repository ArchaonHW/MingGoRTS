// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 北洋政府 - Beiyan成 Go正e本n設置ent I設置ple設置entation

#incl使de "Rep使blicE本a軍actions/軍actions/Beiyan成Go正e本n設置ent.h"
#incl使de "Min成RTSPlaye本Cont本olle本.h"

UBeiyan成Go正e本n設置ent::UBeiyan成Go正e本n設置ent()
{
    InitializeAtt本ib使tes();
    InitializeUniq使eUnits();
    InitializeUniq使eMechanics();
    InitializeGa設置eplayG使ide();
}

正oid UBeiyan成Go正e本n設置ent::Initialize軍action()
{
    S使pe本::Initialize軍action();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Beiyan成 Go正e本n設置ent faction initialized"));
    
    // Apply sta本tin成 bon使ses
    TMap<軍的a設置e, float> Bon使ses = GetSta本tin成Bon使ses();
    // Apply bon使ses to playe本 cont本olle本
}

正oid UBeiyan成Go正e本n設置ent::InitializeAtt本ib使tes()
{
    Att本ib使tes.軍actionID = 軍的a設置e("Beiyan成Go正e本n設置ent");
    Att本ib使tes.軍action的a設置e = 軍Text::軍本o設置St本in成(TEXT("北洋政府"));
    Att本ib使tes.軍action的a設置eEn成lish = 軍Text::軍本o設置St本in成(TEXT("Beiyan成 Go正e本n設置ent"));
    Att本ib使tes.Diffic使lty = E軍actionDiffic使lty::Easy;
    Att本ib使tes.Cate成o本y = E軍actionCate成o本y::Cent本alGo正e本n設置ent;
    Att本ib使tes.軍actionColo本 = 軍Linea本Colo本(0.8f, 0.1f, 0.1f, 1.0f); // Red
    
    // Sta本tin成 position
    Att本ib使tes.Sta本tin成Position.CapitalCity = 軍Text::軍本o設置St本in成(TEXT("北京"));
    Att本ib使tes.Sta本tin成Position.Sta本tin成Te本本ito本ies = {
        軍Text::軍本o設置St本in成(TEXT("北京")),
        軍Text::軍本o設置St本in成(TEXT("天津")),
        軍Text::軍本o設置St本in成(TEXT("河北"))
    };
    Att本ib使tes.Sta本tin成Position.Sta本tin成Yea本 = 1912;
    Att本ib使tes.Sta本tin成Position.InitialGold = 1500; // +50% fo本 cent本al 成o正e本n設置ent
    Att本ib使tes.Sta本tin成Position.InitialManpowe本 = 600;
    Att本ib使tes.Sta本tin成Position.InitialRep使tation = 70; // 輸入i成h inte本national 本eco成nition
    
    // T本aits
    Att本ib使tes.T本aits.Ad正anta成es = {
        軍Text::軍本o設置St本in成(TEXT("國際外交承認，容易獲得外國支援")),
        軍Text::軍本o設置St本in成(TEXT("可以從其他勢力徵收稅收")),
        軍Text::軍本o設置St本in成(TEXT("德國軍事顧問訓練的現代化部隊")),
        軍Text::軍本o設置St本in成(TEXT("起始資金和聲望較高"))
    };
    
    Att本ib使tes.T本aits.Disad正anta成es = {
        軍Text::軍本o設置St本in成(TEXT("內部派系鬥爭嚴重，穩定性較低")),
        軍Text::軍本o設置St本in成(TEXT("容易成為其他軍閥的共同敵人")),
        軍Text::軍本o設置St本in成(TEXT("必須維護中央權威的額外壓力")),
        軍Text::軍本o設置St本in成(TEXT("依賴外國援助，獨立性受限"))
    };
    
    Att本ib使tes.T本aits.Reco設置設置endedSt本ate成ies = {
        軍Text::軍本o設置St本in成(TEXT("利用外交優勢結盟")),
        軍Text::軍本o設置St本in成(TEXT("優先發展現代化陸軍")),
        軍Text::軍本o設置St本in成(TEXT("適時徵收稅收增加收入"))
    };
    
    Att本ib使tes.T本aits.Playe本TypeReco設置設置endation = 軍Text::軍本o設置St本in成(
        TEXT("推薦給喜歡外交和經濟管理的玩家，難度較低，適合新手"));
}

正oid UBeiyan成Go正e本n設置ent::InitializeUniq使eUnits()
{
    Uniq使eUnits.Add(C本eateBeiyan成EliteInfant本y());
    Uniq使eUnits.Add(C本eateGe本設置anA本tille本yCo本ps());
    Uniq使eUnits.Add(C本eateBeiyan成Ca正al本yDi正ision());
    Uniq使eUnits.Add(C本eate軍o本ei成nAd正iso本Co本ps());
}

正oid UBeiyan成Go正e本n設置ent::InitializeUniq使eMechanics()
{
    // Mechanic 1: Diplo設置atic Reco成nition
    {
        軍軍actionMechanic Mechanic = C本eateMechanicDefinition(
            軍的a設置e("Diplo設置aticReco成nition"),
            軍Text::軍本o設置St本in成(TEXT("外交承認")),
            軍Text::軍本o設置St本in成(TEXT("作為中央政府，獲得國際外交承認，可以更容易獲得外國援助和軍事顧問")),
            E軍actionMechanicType::Diplo設置atic,
            1.25f
        );
        Mechanic.Req使i本edTechnolo成ies.Add(軍的a設置e("軍o本ei成nRelations"));
        Uniq使eMechanics.Add(Mechanic);
    }
    
    // Mechanic 2: Cent本al A使tho本ity
    {
        軍軍actionMechanic Mechanic = C本eateMechanicDefinition(
            軍的a設置e("Cent本alA使tho本ity"),
            軍Text::軍本o設置St本in成(TEXT("中央權威")),
            軍Text::軍本o設置St本in成(TEXT("可以利用中央政府的地位，向地方軍閥徵收稅收，增加收入來源")),
            E軍actionMechanicType::Political,
            1.2f
        );
        Mechanic.Req使i本edTechnolo成ies.Add(軍的a設置e("Taxation"));
        Uniq使eMechanics.Add(Mechanic);
    }
}

正oid UBeiyan成Go正e本n設置ent::InitializeGa設置eplayG使ide()
{
    Ga設置eplayG使ide.Ea本lyGa設置eSt本ate成y = 軍Text::軍本o設置St本in成(
        TEXT("早期利用起始資金優勢快速建立現代化軍隊。通過外交與主要列強建立關係，獲取軍事援助。"));
    
    Ga設置eplayG使ide.MidGa設置eSt本ate成y = 軍Text::軍本o設置St本in成(
        TEXT("中期開始使用中央權威機制徵收稅收，擴大經濟優勢。優先消滅威脅最大的地方軍閥。"));
    
    Ga設置eplayG使ide.LateGa設置eSt本ate成y = 軍Text::軍本o設置St本in成(
        TEXT("後期利用經濟和軍事優勢統一全國。注意維護國際關係，確保外國持續支持。"));
    
    Ga設置eplayG使ide.Milita本yTactics = 軍Text::軍本o設置St本in成(
        TEXT("善用德式訓練的精銳步兵和砲兵進行正規戰。外國顧問團可以大幅提升部隊戰鬥力。騎兵適合追擊和巡邏。"));
    
    Ga設置eplayG使ide.Diplo設置aticAd正ice = 軍Text::軍本o設置St本in成(
        TEXT("優先與英美德日等列強建立良好關係。對地方軍閥採取分化策略，拉攏次要敵人對抗主要威脅。"));
    
    Ga設置eplayG使ide.Econo設置ic軍oc使s = 軍Text::軍本o設置St本in成(
        TEXT("重點發展軍工業和基礎設施。適時使用徵稅能力，但注意不要過度壓迫導致反抗。"));
    
    Ga設置eplayG使ide.KeyDecisions = {
        軍Text::軍本o設置St本in成(TEXT("是否接受外國軍事援助（增加實力但降低獨立性）")),
        軍Text::軍本o設置St本in成(TEXT("何時開始大規模徵收稅收（太早會引發反抗，太晚浪費優勢）")),
        軍Text::軍本o設置St本in成(TEXT("如何處理內部派系鬥爭（平衡或鎮壓）"))
    };
    
    Ga設置eplayG使ide.Victo本yConditions = {
        軍Text::軍本o設置St本in成(TEXT("統一中國，消滅或招安所有軍閥")),
        軍Text::軍本o設置St本in成(TEXT("獲得國際公認的統一政府地位")),
        軍Text::軍本o設置St本in成(TEXT("建立穩定的現代化國家體制"))
    };
}

軍軍actionUnit UBeiyan成Go正e本n設置ent::C本eateBeiyan成EliteInfant本y() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 120;
    Stats.DefensePowe本 = 110;
    Stats.Mo正e設置entSpeed = 100;
    Stats.輸入ealthPoints = 110;
    Stats.P本od使ctionCost = 120;
    Stats.UpkeepCost = 12;
    Stats.T本ainin成Ti設置e = 12.0f;
    
    軍軍actionUnit Unit = C本eateUnitDefinition(
        軍的a設置e("Beiyan成EliteInfant本y"),
        軍Text::軍本o設置St本in成(TEXT("北洋精銳步兵")),
        軍Text::軍本o設置St本in成(TEXT("德國教官訓練的現代化步兵，裝備精良，訓練有素，是北洋軍的核心力量")),
        E軍actionUnitType::Infant本y,
        Stats
    );
    
    Unit.T本aits.Add(EUnitT本ait::ShockT本oop);
    Unit.T本aits.Add(EUnitT本ait::軍o本ei成n);
    
    本et使本n Unit;
}

軍軍actionUnit UBeiyan成Go正e本n設置ent::C本eateGe本設置anA本tille本yCo本ps() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 140;
    Stats.DefensePowe本 = 80;
    Stats.Mo正e設置entSpeed = 60;
    Stats.輸入ealthPoints = 90;
    Stats.P本od使ctionCost = 150;
    Stats.UpkeepCost = 15;
    Stats.T本ainin成Ti設置e = 15.0f;
    
    軍軍actionUnit Unit = C本eateUnitDefinition(
        軍的a設置e("Ge本設置anA本tille本yCo本ps"),
        軍Text::軍本o設置St本in成(TEXT("德式砲兵團")),
        軍Text::軍本o設置St本in成(TEXT("裝備德國製造的現代火砲，火力強大但移動緩慢，需要步兵保護")),
        E軍actionUnitType::A本tille本y,
        Stats
    );
    
    Unit.T本aits.Add(EUnitT本ait::A本tille本yS使ppo本t);
    Unit.T本aits.Add(EUnitT本ait::軍o本ei成n);
    
    本et使本n Unit;
}

軍軍actionUnit UBeiyan成Go正e本n設置ent::C本eateBeiyan成Ca正al本yDi正ision() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 110;
    Stats.DefensePowe本 = 90;
    Stats.Mo正e設置entSpeed = 140;
    Stats.輸入ealthPoints = 100;
    Stats.P本od使ctionCost = 100;
    Stats.UpkeepCost = 10;
    Stats.T本ainin成Ti設置e = 10.0f;
    
    軍軍actionUnit Unit = C本eateUnitDefinition(
        軍的a設置e("Beiyan成Ca正al本yDi正ision"),
        軍Text::軍本o設置St本in成(TEXT("北洋騎兵師")),
        軍Text::軍本o設置St本in成(TEXT("傳統的蒙古馬和改良騎兵戰術結合，移動快速，擅長追擊和巡邏")),
        E軍actionUnitType::Ca正al本y,
        Stats
    );
    
    Unit.T本aits.Add(EUnitT本ait::Ca正al本yCha本成e);
    
    本et使本n Unit;
}

軍軍actionUnit UBeiyan成Go正e本n設置ent::C本eate軍o本ei成nAd正iso本Co本ps() const
{
    軍軍actionUnitStats Stats;
    Stats.AttackPowe本 = 80;
    Stats.DefensePowe本 = 100;
    Stats.Mo正e設置entSpeed = 90;
    Stats.輸入ealthPoints = 80;
    Stats.P本od使ctionCost = 180;
    Stats.UpkeepCost = 20;
    Stats.T本ainin成Ti設置e = 8.0f;
    
    軍軍actionUnit Unit = C本eateUnitDefinition(
        軍的a設置e("軍o本ei成nAd正iso本Co本ps"),
        軍Text::軍本o設置St本in成(TEXT("外國顧問團")),
        軍Text::軍本o設置St本in成(TEXT("由德國、日本等國軍事顧問組成，能大幅提升友軍單位的戰鬥力和組織度")),
        E軍actionUnitType::S使ppo本t,
        Stats
    );
    
    Unit.T本aits.Add(EUnitT本ait::軍o本ei成n);
    Unit.T本aits.Add(EUnitT本ait::Political);
    
    本et使本n Unit;
}

軍軍actionUnitStats UBeiyan成Go正e本n設置ent::ModifyUnitStats(const 軍軍actionUnitStats& BaseStats, 
    E軍actionUnitType UnitType) const
{
    軍軍actionUnitStats ModifiedStats = BaseStats;
    
    // Infant本y 成et +10% attack and defense f本o設置 Ge本設置an t本ainin成
    if (UnitType == E軍actionUnitType::Infant本y)
    {
        ModifiedStats.AttackPowe本 = 軍Math::Ro使ndToInt(BaseStats.AttackPowe本 * 1.1f);
        ModifiedStats.DefensePowe本 = 軍Math::Ro使ndToInt(BaseStats.DefensePowe本 * 1.1f);
    }
    
    // A本tille本y 成ets +15% attack f本o設置 fo本ei成n eq使ip設置ent
    if (UnitType == E軍actionUnitType::A本tille本y)
    {
        ModifiedStats.AttackPowe本 = 軍Math::Ro使ndToInt(BaseStats.AttackPowe本 * 1.15f);
    }
    
    本et使本n ModifiedStats;
}

TMap<軍的a設置e, float> UBeiyan成Go正e本n設置ent::GetSta本tin成Bon使ses() const
{
    TMap<軍的a設置e, float> Bon使ses;
    Bon使ses.Add(軍的a設置e("InitialGoldM使ltiplie本"), 1.5f);
    Bon使ses.Add(軍的a設置e("InitialRep使tationBon使s"), 20.0f);
    Bon使ses.Add(軍的a設置e("軍o本ei成nRelationBon使s"), 25.0f);
    Bon使ses.Add(軍的a設置e("TaxInco設置eM使ltiplie本"), 1.2f);
    
    本et使本n Bon使ses;
}

軍Ga設置eplayG使ide UBeiyan成Go正e本n設置ent::GetGa設置eplayG使ide() const
{
    本et使本n Ga設置eplayG使ide;
}
