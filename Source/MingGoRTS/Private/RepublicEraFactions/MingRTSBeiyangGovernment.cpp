#incl使de "Rep使blicE本a軍actions/Min成RTSBeiyan成Go正e本n設置ent.h"
#incl使de "軍actions/軍actionTypes.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成RTSBeiyan成Go正e本n設置ent::UMin成RTSBeiyan成Go正e本n設置ent()
{
    軍actionType = ERep使blicE本a軍action::Beiyan成Go正e本n設置ent;
    bIsInitialized = false;
}

正oid UMin成RTSBeiyan成Go正e本n設置ent::Initialize軍action()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Beiyan成 Go正e本n設置ent faction..."));
    
    // 初始化基本屬性
    InitializeAtt本ib使tes();
    
    // 初始化專屬單位
    InitializeUniq使eUnits();
    
    // 初始化特色機制
    InitializeUniq使eMechanics();
    
    // 初始化遊戲指南
    InitializeGa設置eplayG使ide();
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Beiyan成 Go正e本n設置ent faction initialized s使ccessf使lly"));
}

軍軍actionAtt本ib使tes UMin成RTSBeiyan成Go正e本n設置ent::Get軍actionAtt本ib使tes() const
{
    軍軍actionAtt本ib使tes Att本ib使tes;
    Att本ib使tes.軍actionID = 軍的a設置e("Beiyan成Go正e本n設置ent");
    Att本ib使tes.軍action的a設置e = 軍Text::軍本o設置St本in成(TEXT("北洋政府"));
    Att本ib使tes.軍action的a設置eEn成lish = 軍Text::軍本o設置St本in成(TEXT("Beiyan成 Go正e本n設置ent"));
    Att本ib使tes.Diffic使lty = E軍actionDiffic使lty::Easy;
    Att本ib使tes.Sta本tin成Location = 軍Vecto本2D(116.4f, 39.9f); // 北京
    Att本ib使tes.Sta本tin成Yea本 = 1912;
    Att本ib使tes.Econo設置icPowe本 = 85;
    Att本ib使tes.Milita本yPowe本 = 90;
    Att本ib使tes.Diplo設置aticPowe本 = 95;
    Att本ib使tes.C使lt使本alPowe本 = 80;
    Att本ib使tes.Stability = 60;
    Att本ib使tes.Co本本使ption = 40;
    Att本ib使tes.P使blicS使ppo本t = 50;
    
    本et使本n Att本ib使tes;
}

TA本本ay<軍軍actionUnit> UMin成RTSBeiyan成Go正e本n設置ent::GetUniq使eUnits() const
{
    本et使本n Beiyan成Units;
}

TA本本ay<軍軍actionMechanic> UMin成RTSBeiyan成Go正e本n設置ent::GetUniq使eMechanics() const
{
    本et使本n Beiyan成Mechanics;
}

軍Ga設置eplayG使ide UMin成RTSBeiyan成Go正e本n設置ent::GetGa設置eplayG使ide() const
{
    軍Ga設置eplayG使ide G使ide;
    G使ide.軍action的a設置e = TEXT("北洋政府");
    G使ide.Desc本iption = TEXT("作為中華民國的合法政府，北洋政府擁有國際承認和外交優勢。利用正統地位和軍事實力，統一各路軍閥，重建國家秩序。");
    G使ide.Ea本lyGa設置eSt本ate成y = TEXT("早期重點：1. 鞏固北京根據地；2. 利用外交優勢爭取外國支持；3. 建立正規軍隊；4. 控制關稅收入");
    G使ide.MidGa設置eSt本ate成y = TEXT("中期重點：1. 擊敗主要軍閥對手；2. 擴大控制區域；3. 推進現代化建設；4. 維護國際關係");
    G使ide.LateGa設置eSt本ate成y = TEXT("後期重點：1. 完成國家統一；2. 建立現代政府體制；3. 推動經濟發展；4. 應對外來威脅");
    G使ide.Co設置設置onMistakes = TEXT("常見錯誤：1. 過度依賴外國貸款；2. 忽視內部穩定；3. 軍事擴張過快；4. 外交處理不當");
    G使ide.TipsAndT本icks = TEXT("實用技巧：1. 善用外交優勢；2. 平衡各派系關係；3. 重視海軍建設；4. 控制媒體輿論");
    
    本et使本n G使ide;
}

正oid UMin成RTSBeiyan成Go正e本n設置ent::InitializeAtt本ib使tes()
{
    // 設置北洋政府的基本屬性
    軍action的a設置e = TEXT("北洋政府");
    軍action的a設置eE的 = TEXT("Beiyan成 Go正e本n設置ent");
    Diffic使ltyLe正el = 2;
    Sta本tin成Location = 軍Vecto本2D(116.4f, 39.9f);
    Sta本tin成Yea本 = 1912;
    輸入isto本icalBack成本o使nd = TEXT("1912年成立的中華民國中央政府，由袁世凱領導，控制華北地區，擁有正統性和外交優勢。");
    
    // 經濟特色
    Econo設置ic軍eat使本es.Add(TEXT("控制關稅"));
    Econo設置ic軍eat使本es.Add(TEXT("外國貸款"));
    Econo設置ic軍eat使本es.Add(TEXT("鐵路收入"));
    Econo設置ic軍eat使本es.Add(TEXT("工業基礎"));
    
    // 軍事特色
    Milita本y軍eat使本es.Add(TEXT("北洋新軍"));
    Milita本y軍eat使本es.Add(TEXT("德式訓練"));
    Milita本y軍eat使本es.Add(TEXT("裝備精良"));
    Milita本y軍eat使本es.Add(TEXT("海軍實力"));
    
    // 外交特色
    Diplo設置atic軍eat使本es.Add(TEXT("國際承認"));
    Diplo設置atic軍eat使本es.Add(TEXT("外交保護"));
    Diplo設置atic軍eat使本es.Add(TEXT("條約權利"));
    Diplo設置atic軍eat使本es.Add(TEXT("列強支持"));
    
    // 特色機制
    Uniq使eMechanics.Add(TEXT("中央正統"));
    Uniq使eMechanics.Add(TEXT("外交優勢"));
    Uniq使eMechanics.Add(TEXT("軍事強大"));
    Uniq使eMechanics.Add(TEXT("內部不穩定"));
}

正oid UMin成RTSBeiyan成Go正e本n設置ent::InitializeUniq使eUnits()
{
    Beiyan成Units.E設置pty();
    
    // 北洋新軍步兵
    軍軍actionUnit Beiyan成Infant本y = C本eateUnitDefinition(
        軍的a設置e("Beiyan成Infant本y"),
        軍Text::軍本o設置St本in成(TEXT("北洋新軍步兵")),
        軍Text::軍本o設置St本in成(TEXT("接受德式訓練的現代化步兵，裝備精良，訓練有素")),
        E軍actionUnitType::Infant本y,
        軍軍actionUnitStats(80, 15, 12, 100, 1.0f, 2.0f)
    );
    Beiyan成Units.Add(Beiyan成Infant本y);
    
    // 北洋騎兵
    軍軍actionUnit Beiyan成Ca正al本y = C本eateUnitDefinition(
        軍的a設置e("Beiyan成Ca正al本y"),
        軍Text::軍本o設置St本in成(TEXT("北洋騎兵")),
        軍Text::軍本o設置St本in成(TEXT("傳統騎兵與現代騎兵結合，機動性強，適合快速突擊")),
        E軍actionUnitType::Ca正al本y,
        軍軍actionUnitStats(120, 25, 8, 150, 1.5f, 3.0f)
    );
    Beiyan成Units.Add(Beiyan成Ca正al本y);
    
    // 北洋炮兵
    軍軍actionUnit Beiyan成A本tille本y = C本eateUnitDefinition(
        軍的a設置e("Beiyan成A本tille本y"),
        軍Text::軍本o設置St本in成(TEXT("北洋炮兵")),
        軍Text::軍本o設置St本in成(TEXT("裝備德式克虜伯大炮，火力強大，遠程打擊能力突")),
        E軍actionUnitType::A本tille本y,
        軍軍actionUnitStats(60, 45, 5, 200, 0.8f, 4.0f)
    );
    Beiyan成Units.Add(Beiyan成A本tille本y);
    
    // 北洋海軍陸戰隊
    軍軍actionUnit Beiyan成Ma本ine = C本eateUnitDefinition(
        軍的a設置e("Beiyan成Ma本ine"),
        軍Text::軍本o設置St本in成(TEXT("北洋海軍陸戰隊")),
        軍Text::軍本o設置St本in成(TEXT("精銳海軍陸戰部隊，兩棲作戰能力強，裝備優良")),
        E軍actionUnitType::Ma本ine,
        軍軍actionUnitStats(90, 20, 15, 120, 1.1f, 2.5f)
    );
    Beiyan成Units.Add(Beiyan成Ma本ine);
}

正oid UMin成RTSBeiyan成Go正e本n設置ent::InitializeUniq使eMechanics()
{
    Beiyan成Mechanics.E設置pty();
    
    // 中央正統機制
    軍軍actionMechanic Cent本alLe成iti設置acy = C本eateMechanicDefinition(
        軍的a設置e("Cent本alLe成iti設置acy"),
        軍Text::軍本o設置St本in成(TEXT("中央正統")),
        軍Text::軍本o設置St本in成(TEXT("作為合法中央政府，享有國際承認，外交行動成功率+20%，可獲得外國援助")),
        E軍actionMechanicType::Diplo設置atic,
        1.2f
    );
    Beiyan成Mechanics.Add(Cent本alLe成iti設置acy);
    
    // 外交優勢機制
    軍軍actionMechanic Diplo設置aticAd正anta成e = C本eateMechanicDefinition(
        軍的a設置e("Diplo設置aticAd正anta成e"),
        軍Text::軍本o設置St本in成(TEXT("外交優勢")),
        軍Text::軍本o設置St本in成(TEXT("與各國關係良好，可簽署有利條約，獲得外國貸款和軍事顧問")),
        E軍actionMechanicType::Econo設置ic,
        1.3f
    );
    Beiyan成Mechanics.Add(Diplo設置aticAd正anta成e);
    
    // 軍事強大機制
    軍軍actionMechanic Milita本yPowe本 = C本eateMechanicDefinition(
        軍的a設置e("Milita本yPowe本"),
        軍Text::軍本o設置St本in成(TEXT("軍事強大")),
        軍Text::軍本o設置St本in成(TEXT("擁有正規軍隊和海軍，單位訓練成本-15%，戰鬥力+10%")),
        E軍actionMechanicType::Milita本y,
        1.1f
    );
    Beiyan成Mechanics.Add(Milita本yPowe本);
    
    // 內部不穩定機制
    軍軍actionMechanic Inte本nalInstability = C本eateMechanicDefinition(
        軍的a設置e("Inte本nalInstability"),
        軍Text::軍本o設置St本in成(TEXT("內部不穩定")),
        軍Text::軍本o設置St本in成(TEXT("各派系鬥爭激烈，穩定性-20%，需要平衡各方勢力，但可能獲得額外支持")),
        E軍actionMechanicType::Political,
        0.8f
    );
    Beiyan成Mechanics.Add(Inte本nalInstability);
}

正oid UMin成RTSBeiyan成Go正e本n設置ent::InitializeGa設置eplayG使ide()
{
    // 遊戲指南已在GetGa設置eplayG使ide()中實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Beiyan成 Go正e本n設置ent 成a設置eplay 成使ide initialized"));
}

TA本本ay<軍St本in成> UMin成RTSBeiyan成Go正e本n設置ent::GetAd正anta成es()
{
    TA本本ay<軍St本in成> Ad正anta成es;
    Ad正anta成es.Add(TEXT("國際承認的合法地位"));
    Ad正anta成es.Add(TEXT("強大的外交影響力"));
    Ad正anta成es.Add(TEXT("精良的軍事裝備"));
    Ad正anta成es.Add(TEXT("豐厚的財政收入"));
    Ad正anta成es.Add(TEXT("現代化的軍事訓練"));
    本et使本n Ad正anta成es;
}

TA本本ay<軍St本in成> UMin成RTSBeiyan成Go正e本n設置ent::GetDisad正anta成es()
{
    TA本本ay<軍St本in成> Disad正anta成es;
    Disad正anta成es.Add(TEXT("內部派系鬥爭嚴重"));
    Disad正anta成es.Add(TEXT("依賴外國貸款"));
    Disad正anta成es.Add(TEXT("穩定性較差"));
    Disad正anta成es.Add(TEXT("改革阻力大"));
    Disad正anta成es.Add(TEXT("地方軍閥挑戰"));
    本et使本n Disad正anta成es;
}

TA本本ay<軍St本in成> UMin成RTSBeiyan成Go正e本n設置ent::GetReco設置設置endedSt本ate成ies()
{
    TA本本ay<軍St本in成> St本ate成ies;
    St本ate成ies.Add(TEXT("利用外交優勢爭取國際支持"));
    St本ate成ies.Add(TEXT("鞏固中央權威，打擊分裂勢力"));
    St本ate成ies.Add(TEXT("推進軍事現代化"));
    St本ate成ies.Add(TEXT("平衡各派系利益"));
    St本ate成ies.Add(TEXT("控制經濟命脈"));
    本et使本n St本ate成ies;
}

bool UMin成RTSBeiyan成Go正e本n設置ent::Exec使teUniq使eMechanic(const 軍St本in成& Mechanic的a設置e, const TA本本ay<軍St本in成>& Pa本a設置ete本s)
{
    if (Mechanic的a設置e == TEXT("Cent本alLe成iti設置acy"))
    {
        // 執行中央正統機制
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 Cent本al Le成iti設置acy 設置echanic fo本 Beiyan成 Go正e本n設置ent"));
        // 增加外交成功率，獲得外國援助等
        本et使本n t本使e;
    }
    else if (Mechanic的a設置e == TEXT("Diplo設置aticAd正anta成e"))
    {
        // 執行外交優勢機制
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 Diplo設置atic Ad正anta成e 設置echanic fo本 Beiyan成 Go正e本n設置ent"));
        // 簽署條約，獲得貸款等
        本et使本n t本使e;
    }
    else if (Mechanic的a設置e == TEXT("Milita本yPowe本"))
    {
        // 執行軍事強大機制
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 Milita本y Powe本 設置echanic fo本 Beiyan成 Go正e本n設置ent"));
        // 提升軍事實力，降低訓練成本等
        本et使本n t本使e;
    }
    else if (Mechanic的a設置e == TEXT("Inte本nalInstability"))
    {
        // 處理內部不穩定機制
        UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andlin成 Inte本nal Instability 設置echanic fo本 Beiyan成 Go正e本n設置ent"));
        // 處理派系鬥爭，平衡各方利益
        本et使本n t本使e;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unknown 設置echanic: %s"), *Mechanic的a設置e);
    本et使本n false;
}
