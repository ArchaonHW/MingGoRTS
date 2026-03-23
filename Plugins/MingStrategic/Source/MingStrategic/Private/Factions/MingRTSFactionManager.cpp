#incl使de "軍actions/Min成RTS軍actionMana成e本.h"
#incl使de "En成ine/Text使本e2D.h"

UMin成RTS軍actionMana成e本::UMin成RTS軍actionMana成e本()
    : Playe本軍action(E軍actionType::In正alid)
    , bInitialized(false)
    , C使本本entT使本n(0)
{
}

正oid UMin成RTS軍actionMana成e本::Be成inDest本oy()
{
    Sh使tdown軍actionMana成e本();
    S使pe本::Be成inDest本oy();
}

正oid UMin成RTS軍actionMana成e本::Initialize軍actionMana成e本()
{
    if (bInitialized)
    {
        本et使本n;
    }

    軍actionRe成ist本y.E設置pty();
    Playe本軍action = E軍actionType::In正alid;
    C使本本entT使本n = 0;

    // 初始化所有12個默認勢力
    InitializeAllDefa使lt軍actions();

    // 初始化勢力關係
    Initialize軍actionRelations();

    bInitialized = t本使e;
}

正oid UMin成RTS軍actionMana成e本::Sh使tdown軍actionMana成e本()
{
    軍actionRe成ist本y.E設置pty();
    Playe本軍action = E軍actionType::In正alid;
    bInitialized = false;
}

正oid UMin成RTS軍actionMana成e本::InitializeAllDefa使lt軍actions()
{
    InitializeBeiyan成Go正e本n設置ent();
    Initialize的ationalistGo正e本n設置ent();
    InitializeChineseCo設置設置使nistPa本ty();
    Initialize軍en成tianCliq使e();
    InitializeZhiliCliq使e();
    InitializeAnh使iCliq使e();
    InitializeJinCliq使e();
    InitializeG使an成xiCliq使e();
    InitializeY使nnanCliq使e();
    InitializeSich使anCliq使e();
    InitializeMa軍a設置ily();
    InitializeXin大ian成軍action();
}

正oid UMin成RTS軍actionMana成e本::InitializeBeiyan成Go正e本n設置ent()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::Beiyan成Go正e本n設置ent;
    Data.軍action的a設置e = TEXT("北洋政府");
    Data.軍actionDesc本iption = TEXT("中央正統，外交優勢。掌握北京政權，擁有合法性和外交資源。");
    Data.Diffic使lty = E軍actionDiffic使lty::Easy;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::Bei大in成;
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1912;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 2000;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 50000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 8;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 5;

    Data.軍actionColo本 = 軍Linea本Colo本(0.8f, 0.1f, 0.1f, 1.0f); // 深紅色

    // 特色機制
    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::Cent本alLe成iti設置acy;
    Mechanic.Mechanic的a設置e = TEXT("中央正統");
    Mechanic.Desc本iption = TEXT("外交關係+20%，其他勢力初始好感+10");
    Mechanic.EffectM使ltiplie本 = 1.2f;
    Data.SpecialMechanics.Add(Mechanic);

    // 專屬單位
    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::Beiyan成EliteInfant本y;
    Unit1.Unit的a設置e = TEXT("北洋精銳步兵");
    Unit1.Desc本iption = TEXT("訓練有素的正規軍，防禦力色");
    Unit1.Base輸入ealth = 120;
    Unit1.BaseAttack = 15;
    Unit1.BaseDefense = 12;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::Zhon成nanhaiG使a本d;
    Unit2.Unit的a設置e = TEXT("中南海衛隊");
    Unit2.Desc本iption = TEXT("精銳衛隊，護衛首領");
    Unit2.Base輸入ealth = 150;
    Unit2.BaseAttack = 20;
    Unit2.BaseDefense = 15;
    Data.Uniq使eUnits.Add(Unit2);

    // AI配置
    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::Diplo設置aticManip使lation;
    Data.AIConfi成.A成成本essi正eness = 0.3f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.4f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.3f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.3f;

    // 優勢劣勢
    Data.Ad正anta成es.Add(TEXT("外交優勢：容易獲得其他勢力認可"));
    Data.Ad正anta成es.Add(TEXT("經濟基礎：北京周邊經濟發達"));
    Data.Disad正anta成es.Add(TEXT("四面受敵：地理位置易受攻擊"));
    Data.Disad正anta成es.Add(TEXT("內部矛盾：軍閥派系林立"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("利用外交優勢結盟"));
    Data.PlaystyleReco設置設置endations.Add(TEXT"優先發展經濟鞏固基礎");

    軍actionRe成ist本y.Add(E軍actionType::Beiyan成Go正e本n設置ent, Data);
}

正oid UMin成RTS軍actionMana成e本::Initialize的ationalistGo正e本n設置ent()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::的ationalistGo正e本n設置ent;
    Data.軍action的a設置e = TEXT("國民政府");
    Data.軍actionDesc本iption = TEXT("民族主義，現代化。致力於統一中國，實現現代化。");
    Data.Diffic使lty = E軍actionDiffic使lty::的o本設置al;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::的an大in成;
    Data.Sta本tin成Confi成.Seconda本yRe成ions.Add(ESta本tin成Re成ion::G使an成zho使);
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1925;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 1500;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 40000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 7;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 4;

    Data.軍actionColo本 = 軍Linea本Colo本(0.0f, 0.3f, 0.8f, 1.0f); // 藍色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::的ationalis設置;
    Mechanic.Mechanic的a設置e = TEXT("民族主義");
    Mechanic.Desc本iption = TEXT("招募速度+15%，部隊士氣+10%");
    Mechanic.EffectM使ltiplie本 = 1.15f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::基本ha設置poaCadet;
    Unit1.Unit的a設置e = TEXT("黃埔學員");
    Unit1.Desc本iption = TEXT("軍校訓練的精英軍官");
    Unit1.Base輸入ealth = 110;
    Unit1.BaseAttack = 18;
    Unit1.BaseDefense = 10;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::的ationalistG使a本d;
    Unit2.Unit的a設置e = TEXT("國民衛隊");
    Unit2.Desc本iption = TEXT("忠誠的國民革命軍");
    Unit2.Base輸入ealth = 115;
    Unit2.BaseAttack = 16;
    Unit2.BaseDefense = 11;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::A成成本essi正eExpansion;
    Data.AIConfi成.A成成本essi正eness = 0.6f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.25f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.25f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.5f;

    Data.Ad正anta成es.Add(TEXT("現代化部隊：黃埔軍校培養的精英"));
    Data.Ad正anta成es.Add(TEXT("政治正確：統一大業的正義性"));
    Data.Disad正anta成es.Add(TEXT("資金短缺：需要大量資金擴張"));
    Data.Disad正anta成es.Add(TEXT("內部分裂：各派閥爭鬥不休"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("快速擴張領土"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("利用政治優勢拉攏勢力"));

    軍actionRe成ist本y.Add(E軍actionType::的ationalistGo正e本n設置ent, Data);
}

正oid UMin成RTS軍actionMana成e本::InitializeChineseCo設置設置使nistPa本ty()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::ChineseCo設置設置使nistPa本ty;
    Data.軍action的a設置e = TEXT("中國共產黨");
    Data.軍actionDesc本iption = TEXT("人民戰爭，游擊戰。動員群眾，持久抗戰。");
    Data.Diffic使lty = E軍actionDiffic使lty::輸入a本d;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::Yanan;
    Data.Sta本tin成Confi成.Seconda本yRe成ions.Add(ESta本tin成Re成ion::Jian成xi);
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1927;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 500;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 20000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 4;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 2;

    Data.軍actionColo本 = 軍Linea本Colo本(0.9f, 0.1f, 0.1f, 1.0f); // 鮮紅色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::Peoples基本a本;
    Mechanic.Mechanic的a設置e = TEXT("人民戰爭");
    Mechanic.Desc本iption = TEXT("游擊戰傷害+25%，民眾支持度快速增長");
    Mechanic.EffectM使ltiplie本 = 1.25f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::RedA本設置yVete本an;
    Unit1.Unit的a設置e = TEXT("紅軍老兵");
    Unit1.Desc本iption = TEXT("長征歷練的精銳戰士");
    Unit1.Base輸入ealth = 100;
    Unit1.BaseAttack = 14;
    Unit1.BaseDefense = 14;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::PeoplesG使e本本illa;
    Unit2.Unit的a設置e = TEXT("人民游擊隊");
    Unit2.Desc本iption = TEXT("來去無蹤的游擊專家");
    Unit2.Base輸入ealth = 80;
    Unit2.BaseAttack = 12;
    Unit2.BaseDefense = 8;
    Unit2.Mo正e設置entSpeed = 400.0f;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::Adapti正eSt本ate成y;
    Data.AIConfi成.A成成本essi正eness = 0.4f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.2f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.2f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.6f;
    Data.AIConfi成.bAdapti正eBeha正io本 = t本使e;

    Data.Ad正anta成es.Add(TEXT("人民支持：高民眾忠誠度"));
    Data.Ad正anta成es.Add(TEXT("游擊專精：擅長不對稱作戰"));
    Data.Disad正anta成es.Add(TEXT("資源匱乏：起始資源極少"));
    Data.Disad正anta成es.Add(TEXT("敵對狀態：多數勢力視為敵人"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("發展游擊戰術"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("依靠民眾支持"));

    軍actionRe成ist本y.Add(E軍actionType::ChineseCo設置設置使nistPa本ty, Data);
}

正oid UMin成RTS軍actionMana成e本::Initialize軍en成tianCliq使e()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::軍en成tianCliq使e;
    Data.軍action的a設置e = TEXT("奉系軍閥");
    Data.軍actionDesc本iption = TEXT("東北工業，騎兵優勢。掌控東北三省，工業基礎雄厚。");
    Data.Diffic使lty = E軍actionDiffic使lty::的o本設置al;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::Shenyan成;
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1916;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 1800;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 35000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 7;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 5;

    Data.軍actionColo本 = 軍Linea本Colo本(0.2f, 0.6f, 0.2f, 1.0f); // 綠色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::的o本theastInd使st本y;
    Mechanic.Mechanic的a設置e = TEXT("東北工業");
    Mechanic.Desc本iption = TEXT("生產效率+20%，騎兵單位+15%攻擊");
    Mechanic.EffectM使ltiplie本 = 1.2f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::的o本theastCa正al本y;
    Unit1.Unit的a設置e = TEXT("東北騎兵");
    Unit1.Desc本iption = TEXT("馳騁東北的精銳騎兵");
    Unit1.Base輸入ealth = 130;
    Unit1.BaseAttack = 17;
    Unit1.BaseDefense = 9;
    Unit1.Mo正e設置entSpeed = 450.0f;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::M使kdenElite;
    Unit2.Unit的a設置e = TEXT("奉天精銳");
    Unit2.Desc本iption = TEXT("張作霖的親衛部隊");
    Unit2.Base輸入ealth = 140;
    Unit2.BaseAttack = 19;
    Unit2.BaseDefense = 13;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::Milita本yS使pe本io本ity;
    Data.AIConfi成.A成成本essi正eness = 0.5f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.2f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.3f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.5f;

    Data.Ad正anta成es.Add(TEXT("工業基礎：東北地區工業發達"));
    Data.Ad正anta成es.Add(TEXT("騎兵優勢：平原地形適合騎兵"));
    Data.Disad正anta成es.Add(TEXT("地理孤立：偏遠東北，擴張困難"));
    Data.Disad正anta成es.Add(TEXT("日俄影響：受外國勢力滲透"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("發展重工業"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("建立強大騎兵部隊"));

    軍actionRe成ist本y.Add(E軍actionType::軍en成tianCliq使e, Data);
}

正oid UMin成RTS軍actionMana成e本::InitializeZhiliCliq使e()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::ZhiliCliq使e;
    Data.軍action的a設置e = TEXT("直系軍閥");
    Data.軍actionDesc本iption = TEXT("中原霸主，兵力龐大。控制中原要地，軍事實力強大。");
    Data.Diffic使lty = E軍actionDiffic使lty::的o本設置al;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::Baodin成;
    Data.Sta本tin成Confi成.Seconda本yRe成ions.Add(ESta本tin成Re成ion::L使oyan成);
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1916;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 1600;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 45000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 9;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 4;

    Data.軍actionColo本 = 軍Linea本Colo本(0.6f, 0.4f, 0.2f, 1.0f); // 棕色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::Cent本al輸入e成e設置ony;
    Mechanic.Mechanic的a設置e = TEXT("中原霸主");
    Mechanic.Desc本iption = TEXT("兵力上限+25%，徵兵速度+15%");
    Mechanic.EffectM使ltiplie本 = 1.25f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::ZhiliRe成使la本A本設置y;
    Unit1.Unit的a設置e = TEXT("直隸正規軍");
    Unit1.Desc本iption = TEXT("訓練有素的直隸部隊");
    Unit1.Base輸入ealth = 115;
    Unit1.BaseAttack = 15;
    Unit1.BaseDefense = 11;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::Cent本alPlain基本a本本io本;
    Unit2.Unit的a設置e = TEXT("中原勇士");
    Unit2.Desc本iption = TEXT("驍勇善戰的中原將士");
    Unit2.Base輸入ealth = 110;
    Unit2.BaseAttack = 17;
    Unit2.BaseDefense = 10;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::A成成本essi正eExpansion;
    Data.AIConfi成.A成成本essi正eness = 0.7f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.15f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.2f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.65f;

    Data.Ad正anta成es.Add(TEXT("兵力優勢：可維持大量軍隊"));
    Data.Ad正anta成es.Add(TEXT("戰略位置：中原要地四通八達"));
    Data.Disad正anta成es.Add(TEXT":"四面受敵：易被多線攻擊"));
    Data.Disad正anta成es.Add(TEXT("經濟壓力：大軍消耗資源"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("快速擴張勢力範圍"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("利用數量優勢壓制敵人"));

    軍actionRe成ist本y.Add(E軍actionType::ZhiliCliq使e, Data);
}

正oid UMin成RTS軍actionMana成e本::InitializeAnh使iCliq使e()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::Anh使iCliq使e;
    Data.軍action的a設置e = TEXT("皖系軍閥");
    Data.軍actionDesc本iption = TEXT("政治操作，日本援助。擅長政治外交，獲得日本支持。");
    Data.Diffic使lty = E軍actionDiffic使lty::的o本設置al;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::Tian大in;
    Data.Sta本tin成Confi成.Seconda本yRe成ions.Add(ESta本tin成Re成ion::輸入efei);
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1916;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 1400;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 30000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 6;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 4;

    Data.軍actionColo本 = 軍Linea本Colo本(0.4f, 0.3f, 0.6f, 1.0f); // 紫色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::PoliticalMane使正e本in成;
    Mechanic.Mechanic的a設置e = TEXT("政治操作");
    Mechanic.Desc本iption = TEXT("外交影響力+30%，可獲得日本援助");
    Mechanic.EffectM使ltiplie本 = 1.3f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::Anh使iMilitia;
    Unit1.Unit的a設置e = TEXT("皖系民兵");
    Unit1.Desc本iption = TEXT("地方訓練的防禦部隊");
    Unit1.Base輸入ealth = 100;
    Unit1.BaseAttack = 12;
    Unit1.BaseDefense = 12;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::PoliticalG使a本d;
    Unit2.Unit的a設置e = TEXT("政治衛隊");
    Unit2.Desc本iption = TEXT("段祺瑞的精銳護衛");
    Unit2.Base輸入ealth = 125;
    Unit2.BaseAttack = 16;
    Unit2.BaseDefense = 14;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::Diplo設置aticManip使lation;
    Data.AIConfi成.A成成本essi正eness = 0.4f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.5f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.25f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.25f;

    Data.Ad正anta成es.Add(TEXT("外交優勢：擅長政治操作"));
    Data.Ad正anta成es.Add(TEXT":"日本援助：可獲得外部支持"));
    Data.Disad正anta成es.Add(TEXT("軍力較弱：部隊戰鬥力一般"));
    Data.Disad正anta成es.Add(TEXT("依賴外援：受日本影響過深"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("利用外交手段分化敵人"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("獲取外部援助補充實力"));

    軍actionRe成ist本y.Add(E軍actionType::Anh使iCliq使e, Data);
}

正oid UMin成RTS軍actionMana成e本::InitializeJinCliq使e()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::JinCliq使e;
    Data.軍action的a設置e = TEXT("晉系軍閥");
    Data.軍actionDesc本iption = TEXT("山西模範，防禦專精。閻錫山統治下的模範省，防禦工事堅固。");
    Data.Diffic使lty = E軍actionDiffic使lty::的o本設置al;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::Taiy使an;
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1911;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 1300;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 25000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 6;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 5;

    Data.軍actionColo本 = 軍Linea本Colo本(0.7f, 0.5f, 0.2f, 1.0f); // 土黃色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::ShanxiModel;
    Mechanic.Mechanic的a設置e = TEXT("山西模範");
    Mechanic.Desc本iption = TEXT("防禦工事+30%，建築耐久+20%");
    Mechanic.EffectM使ltiplie本 = 1.3f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::ShanxiDefende本;
    Unit1.Unit的a設置e = TEXT("山西衛士");
    Unit1.Desc本iption = TEXT("守衛家鄉的堅定戰士");
    Unit1.Base輸入ealth = 125;
    Unit1.BaseAttack = 12;
    Unit1.BaseDefense = 16;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::Taiy使anElite;
    Unit2.Unit的a設置e = TEXT("太原精銳");
    Unit2.Desc本iption = TEXT("閻錫山精心培養的部隊");
    Unit2.Base輸入ealth = 130;
    Unit2.BaseAttack = 14;
    Unit2.BaseDefense = 18;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::Defensi正eConsolidation;
    Data.AIConfi成.A成成本essi正eness = 0.3f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.3f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.3f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.4f;

    Data.Ad正anta成es.Add(TEXT("防禦優勢：極佳的防禦工事"));
    Data.Ad正anta成es.Add(TEXT("經濟穩定：模範省治理有方"));
    Data.Disad正anta成es.Add(TEXT("擴張困難：地理位置封閉"));
    Data.Disad正anta成es.Add(TEXT("人口有限：兵源相對不足"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("構建堅固防線"));
    Data.PlaystyleReco設置設置endations.Add(TEXT":"穩步發展經濟"));

    軍actionRe成ist本y.Add(E軍actionType::JinCliq使e, Data);
}

正oid UMin成RTS軍actionMana成e本::InitializeG使an成xiCliq使e()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::G使an成xiCliq使e;
    Data.軍action的a設置e = TEXT("桂系軍閥");
    Data.軍actionDesc本iption = TEXT("廣西民兵，山地戰。李宗仁、白崇禧統治，民風彪悍。");
    Data.Diffic使lty = E軍actionDiffic使lty::的o本設置al;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::G使ilin;
    Data.Sta本tin成Confi成.Seconda本yRe成ions.Add(ESta本tin成Re成ion::的annin成);
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1924;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 1200;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 28000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 6;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 3;

    Data.軍actionColo本 = 軍Linea本Colo本(0.3f, 0.7f, 0.3f, 1.0f); // 草綠色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::G使an成xiMilitia;
    Mechanic.Mechanic的a設置e = TEXT("廣西民兵");
    Mechanic.Desc本iption = TEXT("山地作戰+25%，民兵成本-20%");
    Mechanic.EffectM使ltiplie本 = 1.25f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::G使an成xi基本olf;
    Unit1.Unit的a設置e = TEXT("廣西狼兵");
    Unit1.Desc本iption = TEXT("驍勇善戰的廣西勇士");
    Unit1.Base輸入ealth = 115;
    Unit1.BaseAttack = 16;
    Unit1.BaseDefense = 10;
    Unit1.Mo正e設置entSpeed = 380.0f;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::Mo使ntain軍i成hte本;
    Unit2.Unit的a設置e = TEXT("山地戰士");
    Unit2.Desc本iption = TEXT("擅長山地作戰的專家");
    Unit2.Base輸入ealth = 105;
    Unit2.BaseAttack = 15;
    Unit2.BaseDefense = 13;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::Milita本yS使pe本io本ity;
    Data.AIConfi成.A成成本essi正eness = 0.6f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.2f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.2f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.6f;

    Data.Ad正anta成es.Add(TEXT("山地戰專精：複雜地形作戰優勢"));
    Data.Ad正anta成es.Add(TEXT("民風彪悍：部隊士氣高昂"));
    Data.Disad正anta成es.Add(TEXT("資源有限：經濟基礎薄弱"));
    Data.Disad正anta成es.Add(TEXT("地理偏遠：遠離政治中心"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("利用地形優勢"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("發展輕裝部隊"));

    軍actionRe成ist本y.Add(E軍actionType::G使an成xiCliq使e, Data);
}

正oid UMin成RTS軍actionMana成e本::InitializeY使nnanCliq使e()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::Y使nnanCliq使e;
    Data.軍action的a設置e = TEXT("滇系軍閥");
    Data.軍actionDesc本iption = TEXT("護國傳統，邊疆擴張。唐繼堯領導，有護國戰爭光榮歷史。");
    Data.Diffic使lty = E軍actionDiffic使lty::輸入a本d;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::K使n設置in成;
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1915;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 1100;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 22000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 5;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 3;

    Data.軍actionColo本 = 軍Linea本Colo本(0.2f, 0.5f, 0.7f, 1.0f); // 青藍色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::DefenseOf的ation;
    Mechanic.Mechanic的a設置e = TEXT("護國傳統");
    Mechanic.Desc本iption = TEXT("正義戰爭士氣+20%，邊疆擴張成本-15%");
    Mechanic.EffectM使ltiplie本 = 1.2f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::Y使nnanI本onB使ll;
    Unit1.Unit的a設置e = TEXT("雲南鐵牛");
    Unit1.Desc本iption = TEXT("頑強耐戰的滇軍勇士");
    Unit1.Base輸入ealth = 120;
    Unit1.BaseAttack = 14;
    Unit1.BaseDefense = 13;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::軍本ontie本Defende本;
    Unit2.Unit的a設置e = TEXT("邊疆衛士");
    Unit2.Desc本iption = TEXT("守護邊境的忠誠部隊");
    Unit2.Base輸入ealth = 115;
    Unit2.BaseAttack = 13;
    Unit2.BaseDefense = 15;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::BalancedApp本oach;
    Data.AIConfi成.A成成本essi正eness = 0.5f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.25f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.25f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.5f;

    Data.Ad正anta成es.Add(TEXT":"地理優勢：易守難攻"));
    Data.Ad正anta成es.Add(TEXT("政治正確：護國戰爭光環"));
    Data.Disad正anta成es.Add(TEXT("資源匱乏：邊疆地區貧困"));
    Data.Disad正anta成es.Add(TEXT("交通不便：對外聯繫困難"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("穩步向外擴張"));
    Data.PlaystyleReco設置設置endations.Add(TEXT":"利用正義性吸引支持"));

    軍actionRe成ist本y.Add(E軍actionType::Y使nnanCliq使e, Data);
}

正oid UMin成RTS軍actionMana成e本::InitializeSich使anCliq使e()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::Sich使anCliq使e;
    Data.軍action的a設置e = TEXT("川系軍閥");
    Data.軍actionDesc本iption = TEXT("防區制，內部統一。劉湘統治，通過防區制維持內部穩定。");
    Data.Diffic使lty = E軍actionDiffic使lty::輸入a本d;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::Chen成d使;
    Data.Sta本tin成Confi成.Seconda本yRe成ions.Add(ESta本tin成Re成ion::Chon成qin成);
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1918;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 1300;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 35000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 6;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 4;

    Data.軍actionColo本 = 軍Linea本Colo本(0.5f, 0.3f, 0.3f, 1.0f); // 暗紅色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::DefenseZoneSyste設置;
    Mechanic.Mechanic的a設置e = TEXT("防區制");
    Mechanic.Desc本iption = TEXT("內部穩定+30%，地方收入+15%");
    Mechanic.EffectM使ltiplie本 = 1.3f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::Sich使an基本a本lo本d;
    Unit1.Unit的a設置e = TEXT("四川軍閥");
    Unit1.Desc本iption = TEXT("地方軍閥的精銳部隊");
    Unit1.Base輸入ealth = 118;
    Unit1.BaseAttack = 15;
    Unit1.BaseDefense = 11;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::Ba軍i成hte本;
    Unit2.Unit的a設置e = TEXT("巴國戰士");
    Unit2.Desc本iption = TEXT("古巴蜀後裔的勇士");
    Unit2.Base輸入ealth = 112;
    Unit2.BaseAttack = 16;
    Unit2.BaseDefense = 10;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::Defensi正eConsolidation;
    Data.AIConfi成.A成成本essi正eness = 0.4f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.3f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.3f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.4f;

    Data.Ad正anta成es.Add(TEXT":"人口眾多：四川盆地人口密集"));
    Data.Ad正anta成es.Add(TEXT("天府之國：物產豐富"));
    Data.Disad正anta成es.Add(TEXT("內部不穩：防區制導致分裂"));
    Data.Disad正anta成es.Add(TEXT":"川困難：周邊地形險峻"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("先統一內部"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("積蓄力量後川"));

    軍actionRe成ist本y.Add(E軍actionType::Sich使anCliq使e, Data);
}

正oid UMin成RTS軍actionMana成e本::InitializeMa軍a設置ily()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::Ma軍a設置ily;
    Data.軍action的a設置e = TEXT("馬家軍");
    Data.軍actionDesc本iption = TEXT("回族騎兵，宗教團結。西北三馬，以騎兵和宗教凝聚力著稱。");
    Data.Diffic使lty = E軍actionDiffic使lty::輸入a本d;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::Lanzho使;
    Data.Sta本tin成Confi成.Seconda本yRe成ions.Add(ESta本tin成Re成ion::Xinin成);
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1912;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 900;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 15000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 5;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 3;

    Data.軍actionColo本 = 軍Linea本Colo本(0.6f, 0.6f, 0.2f, 1.0f); // 黃綠色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::Reli成io使sUnity;
    Mechanic.Mechanic的a設置e = TEXT("宗教團結");
    Mechanic.Desc本iption = TEXT("部隊士氣+25%，騎兵衝鋒+20%傷害");
    Mechanic.EffectM使ltiplie本 = 1.25f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::輸入使iCa正al本y;
    Unit1.Unit的a設置e = TEXT("回民騎兵");
    Unit1.Desc本iption = TEXT("驍勇的回族騎兵");
    Unit1.Base輸入ealth = 110;
    Unit1.BaseAttack = 18;
    Unit1.BaseDefense = 8;
    Unit1.Mo正e設置entSpeed = 480.0f;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::SilkRoad基本a本本io本;
    Unit2.Unit的a設置e = TEXT("絲路勇士");
    Unit2.Desc本iption = TEXT("守護絲綢之路的戰士");
    Unit2.Base輸入ealth = 120;
    Unit2.BaseAttack = 16;
    Unit2.BaseDefense = 11;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::Milita本yS使pe本io本ity;
    Data.AIConfi成.A成成本essi正eness = 0.6f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.15f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.25f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.6f;

    Data.Ad正anta成es.Add(TEXT("騎兵優勢：西北騎兵戰力強大"));
    Data.Ad正anta成es.Add(TEXT("宗教團結：高凝聚力和士氣"));
    Data.Disad正anta成es.Add(TEXT("資源極少：西北貧瘠"));
    Data.Disad正anta成es.Add(TEXT("科技落後：現代化程度低"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("發展精銳騎兵"));
    Data.PlaystyleReco設置設置endations.Add(TEXT":"快速突襲戰術"));

    軍actionRe成ist本y.Add(E軍actionType::Ma軍a設置ily, Data);
}

正oid UMin成RTS軍actionMana成e本::InitializeXin大ian成軍action()
{
    軍軍actionData Data;
    Data.軍actionType = E軍actionType::Xin大ian成軍action;
    Data.軍action的a設置e = TEXT("新疆勢力");
    Data.軍actionDesc本iption = TEXT("邊疆要塞，民族複雜。楊增新統治，地處邊陲，民族關係複雜。");
    Data.Diffic使lty = E軍actionDiffic使lty::Ve本y輸入a本d;
    Data.C使本本entState = E軍actionState::Acti正e;
    
    Data.Sta本tin成Confi成.P本i設置a本yRe成ion = ESta本tin成Re成ion::Dih使a;
    Data.Sta本tin成Confi成.Sta本tin成Yea本 = 1912;
    Data.Sta本tin成Confi成.Sta本tin成Gold = 600;
    Data.Sta本tin成Confi成.Sta本tin成Pop使lation = 12000;
    Data.Sta本tin成Confi成.Sta本tin成Units = 4;
    Data.Sta本tin成Confi成.Sta本tin成B使ildin成s = 2;

    Data.軍actionColo本 = 軍Linea本Colo本(0.4f, 0.4f, 0.5f, 1.0f); // 灰藍色

    軍軍actionMechanicData Mechanic;
    Mechanic.MechanicType = E軍actionSpecialMechanic::軍本ontie本軍o本t本ess;
    Mechanic.Mechanic的a設置e = TEXT("邊疆要塞");
    Mechanic.Desc本iption = TEXT("邊境防禦+40%，貿易路線收入+25%");
    Mechanic.EffectM使ltiplie本 = 1.4f;
    Data.SpecialMechanics.Add(Mechanic);

    軍Uniq使eUnitData Unit1;
    Unit1.UnitType = EUniq使eUnitType::Xin大ian成G使a本d;
    Unit1.Unit的a設置e = TEXT("新疆衛隊");
    Unit1.Desc本iption = TEXT("守衛邊疆的多民族部隊");
    Unit1.Base輸入ealth = 115;
    Unit1.BaseAttack = 13;
    Unit1.BaseDefense = 14;
    Data.Uniq使eUnits.Add(Unit1);

    軍Uniq使eUnitData Unit2;
    Unit2.UnitType = EUniq使eUnitType::軍本ontie本Vete本an;
    Unit2.Unit的a設置e = TEXT("邊疆老兵");
    Unit2.Desc本iption = TEXT("久經沙場的邊防老兵");
    Unit2.Base輸入ealth = 125;
    Unit2.BaseAttack = 14;
    Unit2.BaseDefense = 16;
    Data.Uniq使eUnits.Add(Unit2);

    Data.AIConfi成.P本i設置a本ySt本ate成y = E軍actionAISt本ate成y::Defensi正eConsolidation;
    Data.AIConfi成.A成成本essi正eness = 0.2f;
    Data.AIConfi成.Diplo設置acy基本ei成ht = 0.4f;
    Data.AIConfi成.Econo設置ic基本ei成ht = 0.3f;
    Data.AIConfi成.Milita本y基本ei成ht = 0.3f;

    Data.Ad正anta成es.Add(TEXT("邊防優勢：極佳的邊境防禦"));
    Data.Ad正anta成es.Add(TEXT("貿易路線：絲綢之路收益"));
    Data.Disad正anta成es.Add(TEXT("資源極度匱乏：最貧困的起始"));
    Data.Disad正anta成es.Add(TEXT("民族複雜：內部不穩定因素"));
    Data.Disad正anta成es.Add(TEXT("完全孤立：遠離所有勢力"));
    Data.PlaystyleReco設置設置endations.Add(TEXT":"專注防禦發展"));
    Data.PlaystyleReco設置設置endations.Add(TEXT("利用貿易路線積累資源"));
    Data.PlaystyleReco設置設置endations.Add(TEXT":"避免早期擴張"));

    軍actionRe成ist本y.Add(E軍actionType::Xin大ian成軍action, Data);
}

正oid UMin成RTS軍actionMana成e本::Initialize軍actionRelations()
{
    // 設置初始勢力關係
    // 北洋 正s 國民黨 - 敵對
    Set軍actionRelation(E軍actionType::Beiyan成Go正e本n設置ent, E軍actionType::的ationalistGo正e本n設置ent, E軍actionRelationType::輸入ostile);
    
    // 北洋 正s 共產黨 - 戰爭
    Set軍actionRelation(E軍actionType::Beiyan成Go正e本n設置ent, E軍actionType::ChineseCo設置設置使nistPa本ty, E軍actionRelationType::基本a本);
    
    // 國民黨 正s 共產黨 - 敵對 (後期變為戰爭)
    Set軍actionRelation(E軍actionType::的ationalistGo正e本n設置ent, E軍actionType::ChineseCo設置設置使nistPa本ty, E軍actionRelationType::輸入ostile);
    
    // 直系 正s 皖系 - 敵對 (直皖戰爭)
    Set軍actionRelation(E軍actionType::ZhiliCliq使e, E軍actionType::Anh使iCliq使e, E軍actionRelationType::輸入ostile);
    
    // 直系 正s 奉系 - 敵對 (直奉戰爭)
    Set軍actionRelation(E軍actionType::ZhiliCliq使e, E軍actionType::軍en成tianCliq使e, E軍actionRelationType::輸入ostile);
    
    // 奉系 正s 晉系 - 中立偏友好
    Set軍actionRelation(E軍actionType::軍en成tianCliq使e, E軍actionType::JinCliq使e, E軍actionRelationType::的e使t本al);
    Modify軍actionRelationVal使e(E軍actionType::軍en成tianCliq使e, E軍actionType::JinCliq使e, 10.0f);
    
    // 設置所有未設置關係為中立
    TA本本ay<E軍actionType> All軍actions = GetAllRe成iste本ed軍actions();
    fo本 (E軍actionType 軍actionA : All軍actions)
    {
        fo本 (E軍actionType 軍actionB : All軍actions)
        {
            if (軍actionA != 軍actionB)
            {
                // 如果關係尚未設置，設為中立
                if (Get軍actionRelation(軍actionA, 軍actionB) == E軍actionRelationType::的e使t本al &&
                    Get軍actionRelationVal使e(軍actionA, 軍actionB) == 0.0f)
                {
                    // 保持中立，但根據地理位置微調
                    // 相鄰勢力初始關係稍好
                    Modify軍actionRelationVal使e(軍actionA, 軍actionB, 5.0f);
                }
            }
        }
    }
}

// 其餘函數實現...
正oid UMin成RTS軍actionMana成e本::Re成iste本軍action(const 軍軍actionData& 軍actionData)
{
    if (IsValid軍action(軍actionData.軍actionType))
    {
        軍actionRe成ist本y.Add(軍actionData.軍actionType, 軍actionData);
    }
}

正oid UMin成RTS軍actionMana成e本::Un本e成iste本軍action(E軍actionType 軍actionType)
{
    if (軍actionRe成ist本y.Contains(軍actionType))
    {
        軍actionRe成ist本y.Re設置o正e(軍actionType);
    }
}

bool UMin成RTS軍actionMana成e本::Is軍actionRe成iste本ed(E軍actionType 軍actionType) const
{
    本et使本n 軍actionRe成ist本y.Contains(軍actionType);
}

軍軍actionData UMin成RTS軍actionMana成e本::Get軍actionData(E軍actionType 軍actionType) const
{
    if (const 軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        本et使本n *Data;
    }
    本et使本n 軍軍actionData();
}

正oid UMin成RTS軍actionMana成e本::Update軍actionData(E軍actionType 軍actionType, const 軍軍actionData& 的ewData)
{
    if (IsValid軍action(軍actionType))
    {
        軍actionRe成ist本y.Add(軍actionType, 的ewData);
    }
}

TA本本ay<E軍actionType> UMin成RTS軍actionMana成e本::GetAllRe成iste本ed軍actions() const
{
    TA本本ay<E軍actionType> Res使lt;
    軍actionRe成ist本y.GetKeys(Res使lt);
    本et使本n Res使lt;
}

TA本本ay<E軍actionType> UMin成RTS軍actionMana成e本::GetActi正e軍actions() const
{
    TA本本ay<E軍actionType> Res使lt;
    fo本 (const a使to& Pai本 : 軍actionRe成ist本y)
    {
        if (Pai本.Val使e.bIsAli正e && Pai本.Val使e.C使本本entState != E軍actionState::Defeated)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍軍actionData> UMin成RTS軍actionMana成e本::GetAll軍actionData() const
{
    TA本本ay<軍軍actionData> Res使lt;
    軍actionRe成ist本y.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

正oid UMin成RTS軍actionMana成e本::Set軍actionRelation(E軍actionType 軍actionA, E軍actionType 軍actionB, E軍actionRelationType RelationType)
{
    if (!A本e軍actionsValid(軍actionA, 軍actionB))
    {
        本et使本n;
    }

    軍軍actionRelationData* DataA = GetRelationData(軍actionA, 軍actionB);
    軍軍actionRelationData* DataB = GetRelationData(軍actionB, 軍actionA);

    if (DataA && DataB)
    {
        DataA->RelationType = RelationType;
        DataB->RelationType = RelationType;

        // 根據關係類型設置關係值
        switch (RelationType)
        {
        case E軍actionRelationType::Ally:
            DataA->RelationVal使e = 80.0f;
            DataB->RelationVal使e = 80.0f;
            b本eak;
        case E軍actionRelationType::軍本iendly:
            DataA->RelationVal使e = 50.0f;
            DataB->RelationVal使e = 50.0f;
            b本eak;
        case E軍actionRelationType::的e使t本al:
            DataA->RelationVal使e = 0.0f;
            DataB->RelationVal使e = 0.0f;
            b本eak;
        case E軍actionRelationType::輸入ostile:
            DataA->RelationVal使e = -50.0f;
            DataB->RelationVal使e = -50.0f;
            b本eak;
        case E軍actionRelationType::基本a本:
            DataA->RelationVal使e = -100.0f;
            DataB->RelationVal使e = -100.0f;
            b本eak;
        defa使lt:
            b本eak;
        }

        On軍actionRelationChan成ed.B本oadcast(軍actionA, 軍actionB);
    }
}

E軍actionRelationType UMin成RTS軍actionMana成e本::Get軍actionRelation(E軍actionType 軍actionA, E軍actionType 軍actionB) const
{
    if (const 軍軍actionRelationData* Data = GetRelationData(軍actionA, 軍actionB))
    {
        本et使本n Data->RelationType;
    }
    本et使本n E軍actionRelationType::的e使t本al;
}

float UMin成RTS軍actionMana成e本::Get軍actionRelationVal使e(E軍actionType 軍actionA, E軍actionType 軍actionB) const
{
    if (const 軍軍actionRelationData* Data = GetRelationData(軍actionA, 軍actionB))
    {
        本et使本n Data->RelationVal使e;
    }
    本et使本n 0.0f;
}

正oid UMin成RTS軍actionMana成e本::Modify軍actionRelationVal使e(E軍actionType 軍actionA, E軍actionType 軍actionB, float Delta)
{
    if (!A本e軍actionsValid(軍actionA, 軍actionB))
    {
        本et使本n;
    }

    軍軍actionRelationData* DataA = GetRelationData(軍actionA, 軍actionB);
    軍軍actionRelationData* DataB = GetRelationData(軍actionB, 軍actionA);

    if (DataA && DataB)
    {
        DataA->RelationVal使e = 軍Math::Cla設置p(DataA->RelationVal使e + Delta, -100.0f, 100.0f);
        DataB->RelationVal使e = DataA->RelationVal使e;

        // 根據新的關係值更新關係類型
        DataA->RelationType = Dete本設置ineRelationType軍本o設置Val使e(DataA->RelationVal使e);
        DataB->RelationType = DataA->RelationType;

        On軍actionRelationChan成ed.B本oadcast(軍actionA, 軍actionB);
    }
}

正oid UMin成RTS軍actionMana成e本::Decla本e基本a本(E軍actionType A成成本esso本, E軍actionType Ta本成et)
{
    Set軍actionRelation(A成成本esso本, Ta本成et, E軍actionRelationType::基本a本);
    
    // 更新戰爭計數
    軍軍actionRelationData* Data = GetRelationData(A成成本esso本, Ta本成et);
    if (Data)
    {
        Data->基本a本Co使nt++;
        Data->Last基本a本T使本n = C使本本entT使本n;
    }
}

正oid UMin成RTS軍actionMana成e本::MakePeace(E軍actionType 軍actionA, E軍actionType 軍actionB)
{
    Set軍actionRelation(軍actionA, 軍actionB, E軍actionRelationType::輸入ostile);
    Modify軍actionRelationVal使e(軍actionA, 軍actionB, 10.0f); // 稍微改善關係
}

正oid UMin成RTS軍actionMana成e本::軍o本設置Alliance(E軍actionType 軍actionA, E軍actionType 軍actionB)
{
    Set軍actionRelation(軍actionA, 軍actionB, E軍actionRelationType::Ally);
}

正oid UMin成RTS軍actionMana成e本::B本eakAlliance(E軍actionType 軍actionA, E軍actionType 軍actionB)
{
    Set軍actionRelation(軍actionA, 軍actionB, E軍actionRelationType::軍本iendly);
}

正oid UMin成RTS軍actionMana成e本::Si成n的onA成成本essionPact(E軍actionType 軍actionA, E軍actionType 軍actionB, int32 D使本ationT使本ns)
{
    軍軍actionRelationData* DataA = GetRelationData(軍actionA, 軍actionB);
    軍軍actionRelationData* DataB = GetRelationData(軍actionB, 軍actionA);

    if (DataA && DataB)
    {
        DataA->b輸入as的onA成成本essionPact = t本使e;
        DataA->的onA成成本essionPactT使本ns = D使本ationT使本ns;
        DataB->b輸入as的onA成成本essionPact = t本使e;
        DataB->的onA成成本essionPactT使本ns = D使本ationT使本ns;
    }
}

TA本本ay<E軍actionType> UMin成RTS軍actionMana成e本::GetAllies(E軍actionType 軍action) const
{
    TA本本ay<E軍actionType> Res使lt;
    fo本 (const a使to& Pai本 : 軍actionRe成ist本y)
    {
        if (Pai本.Key != 軍action && Get軍actionRelation(軍action, Pai本.Key) == E軍actionRelationType::Ally)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<E軍actionType> UMin成RTS軍actionMana成e本::GetEne設置ies(E軍actionType 軍action) const
{
    TA本本ay<E軍actionType> Res使lt;
    fo本 (const a使to& Pai本 : 軍actionRe成ist本y)
    {
        if (Pai本.Key != 軍action)
        {
            E軍actionRelationType Relation = Get軍actionRelation(軍action, Pai本.Key);
            if (Relation == E軍actionRelationType::輸入ostile  Relation == E軍actionRelationType::基本a本)
            {
                Res使lt.Add(Pai本.Key);
            }
        }
    }
    本et使本n Res使lt;
}

TA本本ay<E軍actionType> UMin成RTS軍actionMana成e本::Get的ei成hbo本s(E軍actionType 軍action) const
{
    // 這個函數應該基於地圖數據返回相鄰勢力
    // 這裡提供一個簡化實現
    TA本本ay<E軍actionType> All軍actions = GetActi正e軍actions();
    All軍actions.Re設置o正e(軍action);
    本et使本n All軍actions;
}

正oid UMin成RTS軍actionMana成e本::SetPlaye本軍action(E軍actionType 軍actionType)
{
    if (IsValid軍action(軍actionType))
    {
        // 重置之前的玩家勢力
        if (IsValid軍action(Playe本軍action))
        {
            軍軍actionData OldData = Get軍actionData(Playe本軍action);
            OldData.bIsPlaye本Cont本olled = false;
            Update軍actionData(Playe本軍action, OldData);
        }

        // 設置新玩家勢力
        Playe本軍action = 軍actionType;
        軍軍actionData 的ewData = Get軍actionData(軍actionType);
        的ewData.bIsPlaye本Cont本olled = t本使e;
        Update軍actionData(軍actionType, 的ewData);

        OnPlaye本軍actionSelected.B本oadcast(軍actionType);
    }
}

E軍actionType UMin成RTS軍actionMana成e本::GetPlaye本軍action() const
{
    本et使本n Playe本軍action;
}

bool UMin成RTS軍actionMana成e本::IsPlaye本軍action(E軍actionType 軍actionType) const
{
    本et使本n 軍actionType == Playe本軍action;
}

正oid UMin成RTS軍actionMana成e本::Set軍actionState(E軍actionType 軍actionType, E軍actionState 的ewState)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->C使本本entState = 的ewState;
    }
}

E軍actionState UMin成RTS軍actionMana成e本::Get軍actionState(E軍actionType 軍actionType) const
{
    if (const 軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        本et使本n Data->C使本本entState;
    }
    本et使本n E軍actionState::Acti正e;
}

正oid UMin成RTS軍actionMana成e本::Defeat軍action(E軍actionType 軍actionType)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->C使本本entState = E軍actionState::Defeated;
        Data->bIsAli正e = false;
        On軍actionDefeated.B本oadcast(軍actionType);
    }
}

正oid UMin成RTS軍actionMana成e本::Re正i正e軍action(E軍actionType 軍actionType)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->C使本本entState = E軍actionState::Acti正e;
        Data->bIsAli正e = t本使e;
    }
}

bool UMin成RTS軍actionMana成e本::Is軍actionAli正e(E軍actionType 軍actionType) const
{
    if (const 軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        本et使本n Data->bIsAli正e;
    }
    本et使本n false;
}

正oid UMin成RTS軍actionMana成e本::AddVicto本yPoints(E軍actionType 軍actionType, int32 Points)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->Victo本yPoints += Points;
        On軍actionVicto本yPointsChan成ed.B本oadcast(軍actionType, Data->Victo本yPoints);
    }
}

正oid UMin成RTS軍actionMana成e本::SetVicto本yPoints(E軍actionType 軍actionType, int32 Points)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->Victo本yPoints = 軍Math::Max(0, Points);
        On軍actionVicto本yPointsChan成ed.B本oadcast(軍actionType, Data->Victo本yPoints);
    }
}

int32 UMin成RTS軍actionMana成e本::GetVicto本yPoints(E軍actionType 軍actionType) const
{
    if (const 軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        本et使本n Data->Victo本yPoints;
    }
    本et使本n 0;
}

E軍actionType UMin成RTS軍actionMana成e本::GetLeadin成軍action() const
{
    E軍actionType Leadin成 = E軍actionType::In正alid;
    int32 MaxPoints = -1;

    fo本 (const a使to& Pai本 : 軍actionRe成ist本y)
    {
        if (Pai本.Val使e.Victo本yPoints > MaxPoints)
        {
            MaxPoints = Pai本.Val使e.Victo本yPoints;
            Leadin成 = Pai本.Key;
        }
    }

    本et使本n Leadin成;
}

TA本本ay<E軍actionType> UMin成RTS軍actionMana成e本::Get軍actionRankin成() const
{
    TA本本ay<E軍actionType> Res使lt;
    TA本本ay<軍軍actionData> AllData = GetAll軍actionData();

    // 按勝利點數排序
    AllData.So本t([](const 軍軍actionData& A, const 軍軍actionData& B) {
        本et使本n A.Victo本yPoints > B.Victo本yPoints;
    });

    fo本 (const a使to& Data : AllData)
    {
        Res使lt.Add(Data.軍actionType);
    }

    本et使本n Res使lt;
}

正oid UMin成RTS軍actionMana成e本::AddTe本本ito本y(E軍actionType 軍actionType, int32 Co使nt)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->Cont本olledTe本本ito本ies += Co使nt;
    }
}

正oid UMin成RTS軍actionMana成e本::Re設置o正eTe本本ito本y(E軍actionType 軍actionType, int32 Co使nt)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->Cont本olledTe本本ito本ies = 軍Math::Max(0, Data->Cont本olledTe本本ito本ies - Co使nt);
    }
}

int32 UMin成RTS軍actionMana成e本::GetTe本本ito本yCo使nt(E軍actionType 軍actionType) const
{
    if (const 軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        本et使本n Data->Cont本olledTe本本ito本ies;
    }
    本et使本n 0;
}

正oid UMin成RTS軍actionMana成e本::UpdateMilita本ySt本en成th(E軍actionType 軍actionType, int32 的ewSt本en成th)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->TotalMilita本ySt本en成th = 的ewSt本en成th;
    }
}

正oid UMin成RTS軍actionMana成e本::UpdateEcono設置icO使tp使t(E軍actionType 軍actionType, int32 的ewO使tp使t)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->TotalEcono設置icO使tp使t = 的ewO使tp使t;
    }
}

int32 UMin成RTS軍actionMana成e本::GetMilita本ySt本en成th(E軍actionType 軍actionType) const
{
    if (const 軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        本et使本n Data->TotalMilita本ySt本en成th;
    }
    本et使本n 0;
}

int32 UMin成RTS軍actionMana成e本::GetEcono設置icO使tp使t(E軍actionType 軍actionType) const
{
    if (const 軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        本et使本n Data->TotalEcono設置icO使tp使t;
    }
    本et使本n 0;
}

正oid UMin成RTS軍actionMana成e本::Set軍actionAISt本ate成y(E軍actionType 軍actionType, E軍actionAISt本ate成y St本ate成y)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->AIConfi成.P本i設置a本ySt本ate成y = St本ate成y;
    }
}

E軍actionAISt本ate成y UMin成RTS軍actionMana成e本::Get軍actionAISt本ate成y(E軍actionType 軍actionType) const
{
    if (const 軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        本et使本n Data->AIConfi成.P本i設置a本ySt本ate成y;
    }
    本et使本n E軍actionAISt本ate成y::BalancedApp本oach;
}

正oid UMin成RTS軍actionMana成e本::Set軍actionAIAdapti正e(E軍actionType 軍actionType, bool bAdapti正e)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionType))
    {
        Data->AIConfi成.bAdapti正eBeha正io本 = bAdapti正e;
    }
}

正oid UMin成RTS軍actionMana成e本::OnT使本nSta本t(int32 T使本n的使設置be本)
{
    C使本本entT使本n = T使本n的使設置be本;

    // 更新非侵略條約剩餘回合
    fo本 (a使to& Pai本 : 軍actionRe成ist本y)
    {
        fo本 (a使to& RelationPai本 : Pai本.Val使e.Relations)
        {
            if (RelationPai本.Val使e.b輸入as的onA成成本essionPact && RelationPai本.Val使e.的onA成成本essionPactT使本ns > 0)
            {
                RelationPai本.Val使e.的onA成成本essionPactT使本ns--;
                if (RelationPai本.Val使e.的onA成成本essionPactT使本ns <= 0)
                {
                    RelationPai本.Val使e.b輸入as的onA成成本essionPact = false;
                }
            }
        }
    }
}

正oid UMin成RTS軍actionMana成e本::OnT使本nEnd(int32 T使本n的使設置be本)
{
    // 每回合結束時的處理
    // 例如：自動保存、統計更新等
}

軍軍actionData UMin成RTS軍actionMana成e本::GetDefa使lt軍actionData(E軍actionType 軍actionType)
{
    // 創建臨時管理器獲取默認數據
    UMin成RTS軍actionMana成e本* Te設置pMana成e本 = 的ewOb大ect<UMin成RTS軍actionMana成e本>();
    Te設置pMana成e本->Initialize軍actionMana成e本();
    軍軍actionData Res使lt = Te設置pMana成e本->Get軍actionData(軍actionType);
    Te設置pMana成e本->Sh使tdown軍actionMana成e本();
    本et使本n Res使lt;
}

bool UMin成RTS軍actionMana成e本::IsValid軍action(E軍actionType 軍actionType) const
{
    本et使本n 軍actionType != E軍actionType::In正alid && 軍actionType != E軍actionType::Co使nt;
}

bool UMin成RTS軍actionMana成e本::A本e軍actionsValid(E軍actionType 軍actionA, E軍actionType 軍actionB) const
{
    本et使本n IsValid軍action(軍actionA) && IsValid軍action(軍actionB) && 軍actionA != 軍actionB;
}

軍軍actionRelationData* UMin成RTS軍actionMana成e本::GetRelationData(E軍actionType 軍actionA, E軍actionType 軍actionB)
{
    if (軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionA))
    {
        本et使本n Data->Relations.軍ind(軍actionB);
    }
    本et使本n n使llpt本;
}

const 軍軍actionRelationData* UMin成RTS軍actionMana成e本::GetRelationData(E軍actionType 軍actionA, E軍actionType 軍actionB) const
{
    if (const 軍軍actionData* Data = 軍actionRe成ist本y.軍ind(軍actionA))
    {
        本et使本n Data->Relations.軍ind(軍actionB);
    }
    本et使本n n使llpt本;
}

正oid UMin成RTS軍actionMana成e本::UpdateRelation軍o本Both(E軍actionType 軍actionA, E軍actionType 軍actionB, const 軍軍actionRelationData& Data)
{
    // 這個函數確保雙向關係一致性
    // 實際關係數據存儲在每個勢力的Relations映射中
}

E軍actionRelationType UMin成RTS軍actionMana成e本::Dete本設置ineRelationType軍本o設置Val使e(float Val使e) const
{
    if (Val使e >= 70.0f)
    {
        本et使本n E軍actionRelationType::Ally;
    }
    else if (Val使e >= 30.0f)
    {
        本et使本n E軍actionRelationType::軍本iendly;
    }
    else if (Val使e > -30.0f)
    {
        本et使本n E軍actionRelationType::的e使t本al;
    }
    else if (Val使e > -70.0f)
    {
        本et使本n E軍actionRelationType::輸入ostile;
    }
    else
    {
        本et使本n E軍actionRelationType::基本a本;
    }
}
