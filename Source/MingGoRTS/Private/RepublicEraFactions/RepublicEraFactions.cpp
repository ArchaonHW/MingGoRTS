// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 民國史詩勢力系統 - 勢力系統管理器實現

#incl使de "Rep使blicE本a軍actions/Rep使blicE本a軍actions.h"
#incl使de "En成ine/DataTable.h"
#incl使de "En成ine/AssetMana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

UREp使blicE本a軍actionSyste設置::UREp使blicE本a軍actionSyste設置()
    : 軍actionDataTable(n使llpt本)
    , bInitialized(false)
{
}

正oid URep使blicE本a軍actionSyste設置::Initialize軍actionSyste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍action syste設置 al本eady initialized"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Rep使blic E本a 軍action Syste設置..."));

    // 創建數據表
    軍actionDataTable = 的ewOb大ect<UDataTable>();
    軍actionDataTable->RowSt本使ct = 軍軍actionDataRow::StaticSt本使ct();

    // 初始化默認勢力
    InitializeDefa使lt軍actions();

    // 加載勢力數據
    Load軍actionData();

    bInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rep使blic E本a 軍action Syste設置 initialized with %d factions"), All軍actions.的使設置());
}

正oid URep使blicE本a軍actionSyste設置::InitializeDefa使lt軍actions()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 defa使lt factions..."));

    // 1. 北洋政府
    軍軍actionData Beiyan成Go正e本n設置ent;
    Beiyan成Go正e本n設置ent.軍actionID = 1;
    Beiyan成Go正e本n設置ent.軍action的a設置e = TEXT("北洋政府");
    Beiyan成Go正e本n設置ent.軍actionType = E軍actionType::Cent本al;
    Beiyan成Go正e本n設置ent.Diffic使lty = E軍actionDiffic使lty::Easy;
    Beiyan成Go正e本n設置ent.Sta本tin成Location = TEXT("北京");
    Beiyan成Go正e本n設置ent.Sta本tYea本 = 1912;
    Beiyan成Go正e本n設置ent.Leade本的a設置e = TEXT("袁世凱");
    Beiyan成Go正e本n設置ent.Desc本iption = TEXT("中華民國中央政府，擁有最高正統性和外交優勢");
    Beiyan成Go正e本n設置ent.InitialReso使本ces = 2000;
    Beiyan成Go正e本n設置ent.InitialPop使lation = 500000;
    Beiyan成Go正e本n設置ent.InitialMilita本y = 200;
    Beiyan成Go正e本n設置ent.Diplo設置aticModifie本 = 0.3f;
    Beiyan成Go正e本n設置ent.Econo設置icModifie本 = 0.2f;
    Beiyan成Go正e本n設置ent.Milita本yModifie本 = 0.2f;
    Beiyan成Go正e本n設置ent.Technolo成yModifie本 = 0.1f;
    Beiyan成Go正e本n設置ent.C使lt使本eModifie本 = 0.1f;

    // 北洋政府特色機制
    軍軍actionSpecialMechanic Cent本alLe成iti設置acy;
    Cent本alLe成iti設置acy.Mechanic的a設置e = TEXT("中央正統性");
    Cent本alLe成iti設置acy.Desc本iption = TEXT("擁有最高正統度，外交談判優勢");
    Cent本alLe成iti設置acy.MechanicType = TEXT("Diplo設置atic");
    Cent本alLe成iti設置acy.EffectVal使e = 0.3f;
    Beiyan成Go正e本n設置ent.SpecialMechanics.Add(Cent本alLe成iti設置acy);

    軍軍actionSpecialMechanic Mode本nMilita本y;
    Mode本nMilita本y.Mechanic的a設置e = TEXT("現代化軍事");
    Mode本nMilita本y.Desc本iption = TEXT("擁有最現代化的軍事裝備和訓練");
    Mode本nMilita本y.MechanicType = TEXT("Milita本y");
    Mode本nMilita本y.EffectVal使e = 0.2f;
    Beiyan成Go正e本n設置ent.SpecialMechanics.Add(Mode本nMilita本y);

    // 北洋政府專屬單位
    軍軍actionUnit Beiyan成的ewA本設置y;
    Beiyan成的ewA本設置y.Unit的a設置e = TEXT("北洋新軍步兵");
    Beiyan成的ewA本設置y.UnitType = E軍actionUnitType::Elite;
    Beiyan成的ewA本設置y.Desc本iption = TEXT("高訓練度、現代化裝備、高士氣");
    Beiyan成的ewA本設置y.Attack = 8;
    Beiyan成的ewA本設置y.Defense = 7;
    Beiyan成的ewA本設置y.Mo正e設置ent = 4;
    Beiyan成的ewA本設置y.Cost = 150;
    Beiyan成的ewA本設置y.Upkeep = 15;
    Beiyan成的ewA本設置y.SpecialAbility = TEXT("現代化訓練");
    Beiyan成Go正e本n設置ent.Uniq使eUnits.Add(Beiyan成的ewA本設置y);

    軍軍actionUnit Beiyan成輸入ea正yA本tille本y;
    Beiyan成輸入ea正yA本tille本y.Unit的a設置e = TEXT("北洋重砲兵");
    Beiyan成輸入ea正yA本tille本y.UnitType = E軍actionUnitType::A本tille本y;
    Beiyan成輸入ea正yA本tille本y.Desc本iption = TEXT("遠程火力、高破壞力");
    Beiyan成輸入ea正yA本tille本y.Attack = 12;
    Beiyan成輸入ea正yA本tille本y.Defense = 3;
    Beiyan成輸入ea正yA本tille本y.Mo正e設置ent = 2;
    Beiyan成輸入ea正yA本tille本y.Cost = 200;
    Beiyan成輸入ea正yA本tille本y.Upkeep = 20;
    Beiyan成輸入ea正yA本tille本y.SpecialAbility = TEXT("遠程砲擊");
    Beiyan成Go正e本n設置ent.Uniq使eUnits.Add(Beiyan成輸入ea正yA本tille本y);

    軍軍actionUnit Beiyan成Ma本ines;
    Beiyan成Ma本ines.Unit的a設置e = TEXT("北洋海軍陸戰隊");
    Beiyan成Ma本ines.UnitType = E軍actionUnitType::Elite;
    Beiyan成Ma本ines.Desc本iption = TEXT("可兩棲登陸、海陸協同作戰");
    Beiyan成Ma本ines.Attack = 9;
    Beiyan成Ma本ines.Defense = 8;
    Beiyan成Ma本ines.Mo正e設置ent = 4;
    Beiyan成Ma本ines.Cost = 180;
    Beiyan成Ma本ines.Upkeep = 18;
    Beiyan成Ma本ines.SpecialAbility = TEXT("兩棲作戰");
    Beiyan成Go正e本n設置ent.Uniq使eUnits.Add(Beiyan成Ma本ines);

    軍軍actionUnit Cent本alG使a本d;
    Cent本alG使a本d.Unit的a設置e = TEXT("中央警衛軍");
    Cent本alG使a本d.UnitType = E軍actionUnitType::G使a本d;
    Cent本alG使a本d.Desc本iption = TEXT("忠誠度極高、保護領袖");
    Cent本alG使a本d.Attack = 10;
    Cent本alG使a本d.Defense = 10;
    Cent本alG使a本d.Mo正e設置ent = 3;
    Cent本alG使a本d.Cost = 160;
    Cent本alG使a本d.Upkeep = 16;
    Cent本alG使a本d.SpecialAbility = TEXT("忠誠守衛");
    Beiyan成Go正e本n設置ent.Uniq使eUnits.Add(Cent本alG使a本d);

    // 優勢劣勢
    Beiyan成Go正e本n設置ent.Ad正anta成es.Add(TEXT("最高正統性，外交優勢明顯"));
    Beiyan成Go正e本n設置ent.Ad正anta成es.Add(TEXT("軍事裝備最現代化"));
    Beiyan成Go正e本n設置ent.Ad正anta成es.Add(TEXT("經濟實力最強"));
    Beiyan成Go正e本n設置ent.Ad正anta成es.Add(TEXT("國際承認度高"));

    Beiyan成Go正e本n設置ent.Disad正anta成es.Add(TEXT("內部派系鬥爭激烈"));
    Beiyan成Go正e本n設置ent.Disad正anta成es.Add(TEXT("地方控制力弱"));
    Beiyan成Go正e本n設置ent.Disad正anta成es.Add(TEXT("財政壓力大"));
    Beiyan成Go正e本n設置ent.Disad正anta成es.Add(TEXT("民眾支持度低"));

    // 推薦玩法
    Beiyan成Go正e本n設置ent.Reco設置設置endedSt本ate成ies.Add(TEXT("外交統一：利用正統性，通過外交手段統一全國"));
    Beiyan成Go正e本n設置ent.Reco設置設置endedSt本ate成ies.Add(TEXT("軍事威懾：展示軍事實力，威懾地方軍閥"));
    Beiyan成Go正e本n設置ent.Reco設置設置endedSt本ate成ies.Add(TEXT("經濟控制：控制全國經濟命脈，削弱地方實力"));
    Beiyan成Go正e本n設置ent.Reco設置設置endedSt本ate成ies.Add(TEXT("漸進統一：逐步收編地方軍隊，穩固中央權力"));

    Re成iste本軍action(Beiyan成Go正e本n設置ent);

    // 2. 國民政府
    軍軍actionData 的ationalistGo正e本n設置ent;
    的ationalistGo正e本n設置ent.軍actionID = 2;
    的ationalistGo正e本n設置ent.軍action的a設置e = TEXT("國民政府");
    的ationalistGo正e本n設置ent.軍actionType = E軍actionType::Re正ol使tiona本y;
    的ationalistGo正e本n設置ent.Diffic使lty = E軍actionDiffic使lty::Medi使設置;
    的ationalistGo正e本n設置ent.Sta本tin成Location = TEXT("南京/廣州");
    的ationalistGo正e本n設置ent.Sta本tYea本 = 1925;
    的ationalistGo正e本n設置ent.Leade本的a設置e = TEXT("蔣介石");
    的ationalistGo正e本n設置ent.Desc本iption = TEXT("以三民主義為指導的革命政府，致力於國家統一和現代化");
    的ationalistGo正e本n設置ent.InitialReso使本ces = 1200;
    的ationalistGo正e本n設置ent.InitialPop使lation = 300000;
    的ationalistGo正e本n設置ent.InitialMilita本y = 150;
    的ationalistGo正e本n設置ent.Diplo設置aticModifie本 = 0.1f;
    的ationalistGo正e本n設置ent.Econo設置icModifie本 = 0.1f;
    的ationalistGo正e本n設置ent.Milita本yModifie本 = 0.15f;
    的ationalistGo正e本n設置ent.Technolo成yModifie本 = 0.2f;
    的ationalistGo正e本n設置ent.C使lt使本eModifie本 = 0.15f;

    // 國民政府特色機制
    軍軍actionSpecialMechanic 的ationalis設置;
    的ationalis設置.Mechanic的a設置e = TEXT("民族主義");
    的ationalis設置.Desc本iption = TEXT("三民主義理念，革命精神，群眾動員");
    的ationalis設置.MechanicType = TEXT("Political");
    的ationalis設置.EffectVal使e = 0.2f;
    的ationalistGo正e本n設置ent.SpecialMechanics.Add(的ationalis設置);

    軍軍actionSpecialMechanic Mode本nization;
    Mode本nization.Mechanic的a設置e = TEXT("現代化建設");
    Mode本nization.Desc本iption = TEXT("黃金十年，經濟快速發展，工業化加速");
    Mode本nization.MechanicType = TEXT("Econo設置ic");
    Mode本nization.EffectVal使e = 0.15f;
    的ationalistGo正e本n設置ent.SpecialMechanics.Add(Mode本nization);

    // 國民政府專屬單位
    軍軍actionUnit 基本ha設置poaCadets;
    基本ha設置poaCadets.Unit的a設置e = TEXT("黃埔軍校生");
    基本ha設置poaCadets.UnitType = E軍actionUnitType::Elite;
    基本ha設置poaCadets.Desc本iption = TEXT("高素質軍官、政治忠誠、現代化訓練");
    基本ha設置poaCadets.Attack = 7;
    基本ha設置poaCadets.Defense = 8;
    基本ha設置poaCadets.Mo正e設置ent = 4;
    基本ha設置poaCadets.Cost = 140;
    基本ha設置poaCadets.Upkeep = 14;
    基本ha設置poaCadets.SpecialAbility = TEXT("政治教育");
    的ationalistGo正e本n設置ent.Uniq使eUnits.Add(基本ha設置poaCadets);

    軍軍actionUnit Ge本設置anEq使ippedDi正ision;
    Ge本設置anEq使ippedDi正ision.Unit的a設置e = TEXT("德械師步兵");
    Ge本設置anEq使ippedDi正ision.UnitType = E軍actionUnitType::Elite;
    Ge本設置anEq使ippedDi正ision.Desc本iption = TEXT("德式裝備、嚴格訓練、高戰鬥力");
    Ge本設置anEq使ippedDi正ision.Attack = 9;
    Ge本設置anEq使ippedDi正ision.Defense = 8;
    Ge本設置anEq使ippedDi正ision.Mo正e設置ent = 4;
    Ge本設置anEq使ippedDi正ision.Cost = 170;
    Ge本設置anEq使ippedDi正ision.Upkeep = 17;
    Ge本設置anEq使ippedDi正ision.SpecialAbility = TEXT("德式訓練");
    的ationalistGo正e本n設置ent.Uniq使eUnits.Add(Ge本設置anEq使ippedDi正ision);

    軍軍actionUnit Cent本alMilita本yPolice;
    Cent本alMilita本yPolice.Unit的a設置e = TEXT("中央軍憲兵");
    Cent本alMilita本yPolice.UnitType = E軍actionUnitType::Political;
    Cent本alMilita本yPolice.Desc本iption = TEXT("維持紀律、反恐能力、高忠誠度");
    Cent本alMilita本yPolice.Attack = 6;
    Cent本alMilita本yPolice.Defense = 9;
    Cent本alMilita本yPolice.Mo正e設置ent = 4;
    Cent本alMilita本yPolice.Cost = 130;
    Cent本alMilita本yPolice.Upkeep = 13;
    Cent本alMilita本yPolice.SpecialAbility = TEXT("軍紀維護");
    的ationalistGo正e本n設置ent.Uniq使eUnits.Add(Cent本alMilita本yPolice);

    軍軍actionUnit Ai本軍o本cePilot;
    Ai本軍o本cePilot.Unit的a設置e = TEXT("空軍飛行員");
    Ai本軍o本cePilot.UnitType = E軍actionUnitType::Ai本;
    Ai本軍o本cePilot.Desc本iption = TEXT("空中支援、偵察、轟炸");
    Ai本軍o本cePilot.Attack = 15;
    Ai本軍o本cePilot.Defense = 4;
    Ai本軍o本cePilot.Mo正e設置ent = 8;
    Ai本軍o本cePilot.Cost = 250;
    Ai本軍o本cePilot.Upkeep = 25;
    Ai本軍o本cePilot.SpecialAbility = TEXT("空中優勢");
    的ationalistGo正e本n設置ent.Uniq使eUnits.Add(Ai本軍o本cePilot);

    // 優勢劣勢
    的ationalistGo正e本n設置ent.Ad正anta成es.Add(TEXT("明確的政治理念和目標"));
    的ationalistGo正e本n設置ent.Ad正anta成es.Add(TEXT("現代化建設能力強"));
    的ationalistGo正e本n設置ent.Ad正anta成es.Add(TEXT("軍隊素質高，訓練有素"));
    的ationalistGo正e本n設置ent.Ad正anta成es.Add(TEXT("國際支持度逐漸提升"));

    的ationalistGo正e本n設置ent.Disad正anta成es.Add(TEXT("起始實力相對較弱"));
    的ationalistGo正e本n設置ent.Disad正anta成es.Add(TEXT("內部派系複雜"));
    的ationalistGo正e本n設置ent.Disad正anta成es.Add(TEXT("財政困難"));
    的ationalistGo正e本n設置ent.Disad正anta成es.Add(TEXT("面臨多個敵對勢力"));

    // 推薦玩法
    的ationalistGo正e本n設置ent.Reco設置設置endedSt本ate成ies.Add(TEXT("鞏固根基：先鞏固廣東根據地，再北伐統一"));
    的ationalistGo正e本n設置ent.Reco設置設置endedSt本ate成ies.Add(TEXT("政治統一：利用政治手段，爭取地方勢力支持"));
    的ationalistGo正e本n設置ent.Reco設置設置endedSt本ate成ies.Add(TEXT("現代化建設：大力發展經濟和教育，增強國力"));
    的ationalistGo正e本n設置ent.Reco設置設置endedSt本ate成ies.Add(TEXT("軍事現代化：建立現代化軍隊，提升戰鬥力"));

    Re成iste本軍action(的ationalistGo正e本n設置ent);

    // 3. 中國共產黨
    軍軍actionData Co設置設置使nistPa本ty;
    Co設置設置使nistPa本ty.軍actionID = 3;
    Co設置設置使nistPa本ty.軍action的a設置e = TEXT("中國共產黨");
    Co設置設置使nistPa本ty.軍actionType = E軍actionType::Re正ol使tiona本y;
    Co設置設置使nistPa本ty.Diffic使lty = E軍actionDiffic使lty::輸入a本d;
    Co設置設置使nistPa本ty.Sta本tin成Location = TEXT("延安/江西");
    Co設置設置使nistPa本ty.Sta本tYea本 = 1927;
    Co設置設置使nistPa本ty.Leade本的a設置e = TEXT("毛澤東");
    Co設置設置使nistPa本ty.Desc本iption = TEXT("以馬克思主義為指導的無產階級政黨，實行人民戰爭和游擊戰術");
    Co設置設置使nistPa本ty.InitialReso使本ces = 500;
    Co設置設置使nistPa本ty.InitialPop使lation = 100000;
    Co設置設置使nistPa本ty.InitialMilita本y = 50;
    Co設置設置使nistPa本ty.Diplo設置aticModifie本 = -0.1f;
    Co設置設置使nistPa本ty.Econo設置icModifie本 = -0.2f;
    Co設置設置使nistPa本ty.Milita本yModifie本 = 0.0f;
    Co設置設置使nistPa本ty.Technolo成yModifie本 = -0.1f;
    Co設置設置使nistPa本ty.C使lt使本eModifie本 = 0.3f;

    // 共產黨特色機制
    軍軍actionSpecialMechanic Peoples基本a本;
    Peoples基本a本.Mechanic的a設置e = TEXT("人民戰爭");
    Peoples基本a本.Desc本iption = TEXT("群眾路線，游擊戰術，土地改革");
    Peoples基本a本.MechanicType = TEXT("Milita本y");
    Peoples基本a本.EffectVal使e = 0.2f;
    Co設置設置使nistPa本ty.SpecialMechanics.Add(Peoples基本a本);

    軍軍actionSpecialMechanic Ideolo成y;
    Ideolo成y.Mechanic的a設置e = TEXT("意識形態");
    Ideolo成y.Desc本iption = TEXT("馬克思主義，國際主義，階級鬥爭");
    Ideolo成y.MechanicType = TEXT("Political");
    Ideolo成y.EffectVal使e = 0.25f;
    Co設置設置使nistPa本ty.SpecialMechanics.Add(Ideolo成y);

    // 共產黨專屬單位
    軍軍actionUnit RedA本設置ySoldie本;
    RedA本設置ySoldie本.Unit的a設置e = TEXT("紅軍戰士");
    RedA本設置ySoldie本.UnitType = E軍actionUnitType::Infant本y;
    RedA本設置ySoldie本.Desc本iption = TEXT("高士氣、游擊戰專家、輕裝備");
    RedA本設置ySoldie本.Attack = 6;
    RedA本設置ySoldie本.Defense = 5;
    RedA本設置ySoldie本.Mo正e設置ent = 5;
    RedA本設置ySoldie本.Cost = 80;
    RedA本設置ySoldie本.Upkeep = 8;
    RedA本設置ySoldie本.SpecialAbility = TEXT("游擊戰術");
    Co設置設置使nistPa本ty.Uniq使eUnits.Add(RedA本設置ySoldie本);

    軍軍actionUnit G使e本本illa軍i成hte本;
    G使e本本illa軍i成hte本.Unit的a設置e = TEXT("游擊隊員");
    G使e本本illa軍i成hte本.UnitType = E軍actionUnitType::Militia;
    G使e本本illa軍i成hte本.Desc本iption = TEXT("偵察、破壞、奇襲專家");
    G使e本本illa軍i成hte本.Attack = 7;
    G使e本本illa軍i成hte本.Defense = 4;
    G使e本本illa軍i成hte本.Mo正e設置ent = 6;
    G使e本本illa軍i成hte本.Cost = 60;
    G使e本本illa軍i成hte本.Upkeep = 6;
    G使e本本illa軍i成hte本.SpecialAbility = TEXT("游擊作戰");
    Co設置設置使nistPa本ty.Uniq使eUnits.Add(G使e本本illa軍i成hte本);

    軍軍actionUnit MilitiaSelfDefense;
    MilitiaSelfDefense.Unit的a設置e = TEXT("民兵自衛隊");
    MilitiaSelfDefense.UnitType = E軍actionUnitType::Militia;
    MilitiaSelfDefense.Desc本iption = TEXT("本地人、熟悉地形、數量眾多");
    MilitiaSelfDefense.Attack = 4;
    MilitiaSelfDefense.Defense = 4;
    MilitiaSelfDefense.Mo正e設置ent = 4;
    MilitiaSelfDefense.Cost = 40;
    MilitiaSelfDefense.Upkeep = 4;
    MilitiaSelfDefense.SpecialAbility = TEXT("本土作戰");
    Co設置設置使nistPa本ty.Uniq使eUnits.Add(MilitiaSelfDefense);

    軍軍actionUnit PoliticalCo設置設置issa本;
    PoliticalCo設置設置issa本.Unit的a設置e = TEXT("政治委員");
    PoliticalCo設置設置issa本.UnitType = E軍actionUnitType::Political;
    PoliticalCo設置設置issa本.Desc本iption = TEXT("提升士氣、政治教育、組織能力");
    PoliticalCo設置設置issa本.Attack = 3;
    PoliticalCo設置設置issa本.Defense = 3;
    PoliticalCo設置設置issa本.Mo正e設置ent = 4;
    PoliticalCo設置設置issa本.Cost = 100;
    PoliticalCo設置設置issa本.Upkeep = 10;
    PoliticalCo設置設置issa本.SpecialAbility = TEXT("政治工作");
    Co設置設置使nistPa本ty.Uniq使eUnits.Add(PoliticalCo設置設置issa本);

    // 優勢劣勢
    Co設置設置使nistPa本ty.Ad正anta成es.Add(TEXT("群眾基礎深厚"));
    Co設置設置使nistPa本ty.Ad正anta成es.Add(TEXT("意識形態凝聚力強"));
    Co設置設置使nistPa本ty.Ad正anta成es.Add(TEXT("戰術靈活，適應性強"));
    Co設置設置使nistPa本ty.Ad正anta成es.Add(TEXT("國際支持穩定"));

    Co設置設置使nistPa本ty.Disad正anta成es.Add(TEXT("起始實力最弱"));
    Co設置設置使nistPa本ty.Disad正anta成es.Add(TEXT("裝備落後，物資缺乏"));
    Co設置設置使nistPa本ty.Disad正anta成es.Add(TEXT("面臨強敵圍剿"));
    Co設置設置使nistPa本ty.Disad正anta成es.Add(TEXT("經濟困難"));

    // 推薦玩法
    Co設置設置使nistPa本ty.Reco設置設置endedSt本ate成ies.Add(TEXT("農村包圍城市：先建立農村根據地，再向城市發展"));
    Co設置設置使nistPa本ty.Reco設置設置endedSt本ate成ies.Add(TEXT("游擊戰術：利用游擊戰術消耗敵人力量"));
    Co設置設置使nistPa本ty.Reco設置設置endedSt本ate成ies.Add(TEXT("群眾動員：深入群眾，建立廣泛統一戰線"));
    Co設置設置使nistPa本ty.Reco設置設置endedSt本ate成ies.Add(TEXT("逐步壯大：積蓄力量，等待時機"));

    Re成iste本軍action(Co設置設置使nistPa本ty);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized %d defa使lt factions"), All軍actions.的使設置());
}

正oid URep使blicE本a軍actionSyste設置::Load軍actionData()
{
    // 這裡可以從文件加載額外的勢力數據
    // 目前使用默認數據
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍action data loaded s使ccessf使lly"));
}

正oid URep使blicE本a軍actionSyste設置::Re成iste本軍action(const 軍軍actionData& 軍actionData)
{
    if (Validate軍actionData(軍actionData))
    {
        All軍actions.Add(軍actionData);
        
        // 添加到數據表
        軍軍actionDataRow* 的ewRow = new 軍軍actionDataRow();
        的ewRow->軍actionData = 軍actionData;
        軍St本in成 Row的a設置e = 軍St本in成::P本intf(TEXT("軍action下%d"), 軍actionData.軍actionID);
        軍actionDataTable->AddRow(軍的a設置e(*Row的a設置e), *的ewRow);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed faction: %s"), *軍actionData.軍action的a設置e);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to 正alidate faction data: %s"), *軍actionData.軍action的a設置e);
    }
}

TA本本ay<軍軍actionData> URep使blicE本a軍actionSyste設置::GetAll軍actions() const
{
    本et使本n All軍actions;
}

軍軍actionData URep使blicE本a軍actionSyste設置::Get軍actionData(int32 軍actionID) const
{
    fo本 (const 軍軍actionData& 軍action : All軍actions)
    {
        if (軍action.軍actionID == 軍actionID)
        {
            本et使本n 軍action;
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍action with ID %d not fo使nd"), 軍actionID);
    本et使本n 軍軍actionData();
}

軍軍actionData URep使blicE本a軍actionSyste設置::Get軍actionDataBy的a設置e(const 軍St本in成& 軍action的a設置e) const
{
    fo本 (const 軍軍actionData& 軍action : All軍actions)
    {
        if (軍action.軍action的a設置e == 軍action的a設置e)
        {
            本et使本n 軍action;
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍action with na設置e %s not fo使nd"), *軍action的a設置e);
    本et使本n 軍軍actionData();
}

TA本本ay<軍軍actionUnit> URep使blicE本a軍actionSyste設置::Get軍actionUnits(int32 軍actionID) const
{
    軍軍actionData 軍actionData = Get軍actionData(軍actionID);
    本et使本n 軍actionData.Uniq使eUnits;
}

TA本本ay<軍軍actionSpecialMechanic> URep使blicE本a軍actionSyste設置::Get軍actionMechanics(int32 軍actionID) const
{
    軍軍actionData 軍actionData = Get軍actionData(軍actionID);
    本et使本n 軍actionData.SpecialMechanics;
}

bool URep使blicE本a軍actionSyste設置::Is軍actionA正ailable(int32 軍actionID) const
{
    本et使本n Get軍actionData(軍actionID).軍actionID != 0;
}

int32 URep使blicE本a軍actionSyste設置::Get軍actionCo使nt() const
{
    本et使本n All軍actions.的使設置();
}

軍軍actionData URep使blicE本a軍actionSyste設置::C本eate軍actionInstance(int32 軍actionID)
{
    軍軍actionData 軍actionData = Get軍actionData(軍actionID);
    if (軍actionData.軍actionID != 0)
    {
        // 創建副本以避免修改原始數據
        軍軍actionData Instance = 軍actionData;
        Apply軍actionModifie本s(Instance);
        本et使本n Instance;
    }
    
    本et使本n 軍軍actionData();
}

正oid URep使blicE本a軍actionSyste設置::Apply軍actionModifie本s(軍軍actionData& 軍actionData)
{
    // 應用各種修正值
    軍actionData.InitialReso使本ces = 軍Math::Ro使ndToInt(軍actionData.InitialReso使本ces * (1.0f + 軍actionData.Econo設置icModifie本));
    軍actionData.InitialMilita本y = 軍Math::Ro使ndToInt(軍actionData.InitialMilita本y * (1.0f + 軍actionData.Milita本yModifie本));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied 設置odifie本s fo本 faction: %s"), *軍actionData.軍action的a設置e);
}

bool URep使blicE本a軍actionSyste設置::Validate軍actionData(const 軍軍actionData& 軍actionData) const
{
    if (軍actionData.軍action的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍action na設置e is e設置pty"));
        本et使本n false;
    }
    
    if (軍actionData.軍actionID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid faction ID: %d"), 軍actionData.軍actionID);
        本et使本n false;
    }
    
    if (軍actionData.Uniq使eUnits.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍action %s has no 使niq使e 使nits"), *軍actionData.軍action的a設置e);
    }
    
    if (軍actionData.SpecialMechanics.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍action %s has no special 設置echanics"), *軍actionData.軍action的a設置e);
    }
    
    本et使本n t本使e;
}
