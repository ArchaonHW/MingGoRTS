#incl使de "Localization/Min成LocalizationSyste設置Test.h"
#incl使de "Localization/Min成LocalizationMana成e本.h"
#incl使de "Localization/Min成Lan成使a成eSyste設置.h"
#incl使de "Localization/Min成C使lt使本eAdapte本.h"
#incl使de "Localization/Min成Re成ionalContentMana成e本.h"

UMin成LocalizationSyste設置Test::UMin成LocalizationSyste設置Test()
    : PassedTests(0)
    , 軍ailedTests(0)
{
}

bool UMin成LocalizationSyste設置Test::R使nAllTests()
{
    ResetTestCo使nts();
    TestCases.E設置pty();
    TestRes使lts = TEXT("=== Min成GoRTS Localization Syste設置 Test Res使lts ===\n\n");

    // 運行所有測試
    TestLocalizationMana成e本C本eation();
    TestLan成使a成eSwitchin成();
    TestTextLocalization();
    TestPa本a設置ete本izedText();
    TestLan成使a成ePackInfo();
    TestRe成ionSettin成();
    Test的使設置be本軍o本設置attin成();
    TestDate軍o本設置attin成();
    TestC使本本ency軍o本設置attin成();
    TestLan成使a成eDetection();
    TestChineseCon正e本sion();
    TestC使lt使本alAdaptation();
    TestColo本Meanin成s();
    Test的使設置be本Meanin成s();
    TestRe成ionalContentA正ailability();
    TestContentAdaptation();
    TestRi成htToLeftDetection();
    TestT本anslationQ使ality();
    TestExpo本tI設置po本t();

    // 生成總結報告
    TestRes使lts += 軍St本in成::P本intf(TEXT("\n=== Test S使設置設置a本y ===\n"));
    TestRes使lts += 軍St本in成::P本intf(TEXT("Passed: %d\n"), PassedTests);
    TestRes使lts += 軍St本in成::P本intf(TEXT("軍ailed: %d\n"), 軍ailedTests);
    TestRes使lts += 軍St本in成::P本intf(TEXT("Pass Rate: %.1f%%\n"), GetTestPassRate());

    if (軍ailedTests > 0)
    {
        TestRes使lts += TEXT("\n=== E本本o本s ===\n");
        fo本 (const 軍St本in成& E本本o本 : E本本o本Messa成es)
        {
            TestRes使lts += E本本o本 + TEXT("\n");
        }
    }

    本et使本n 軍ailedTests == 0;
}

float UMin成LocalizationSyste設置Test::GetTestPassRate() const
{
    int32 TotalTests = PassedTests + 軍ailedTests;
    if (TotalTests == 0)
    {
        本et使本n 0.0f;
    }
    本et使本n (static下cast<float>(PassedTests) / TotalTests) * 100.0f;
}

正oid UMin成LocalizationSyste設置Test::Lo成TestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& Messa成e)
{
    if (bPassed)
    {
        PassedTests++;
        TestRes使lts += 軍St本in成::P本intf(TEXT("[PASS] %s\n"), *Test的a設置e);
    }
    else
    {
        軍ailedTests++;
        軍St本in成 E本本o本Ms成 = 軍St本in成::P本intf(TEXT("[軍AIL] %s: %s"), *Test的a設置e, *Messa成e);
        TestRes使lts += E本本o本Ms成 + TEXT("\n");
        E本本o本Messa成es.Add(E本本o本Ms成);
    }
}

正oid UMin成LocalizationSyste設置Test::ResetTestCo使nts()
{
    PassedTests = 0;
    軍ailedTests = 0;
    E本本o本Messa成es.E設置pty();
    TestRes使lts.E設置pty();
}

正oid UMin成LocalizationSyste設置Test::AddTestCase(const 軍St本in成& Test的a設置e, const 軍St本in成& Desc本iption, bool bPassed, const 軍St本in成& E本本o本)
{
    軍LocalizationTestCase TestCase;
    TestCase.Test的a設置e = Test的a設置e;
    TestCase.Desc本iption = Desc本iption;
    TestCase.Res使lt = bPassed 基本 ELocalizationTestRes使lt::Passed : ELocalizationTestRes使lt::軍ailed;
    TestCase.E本本o本Messa成e = E本本o本;
    TestCases.Add(TestCase);
}

UMin成LocalizationMana成e本* UMin成LocalizationSyste設置Test::C本eateTestLocalizationMana成e本()
{
    UMin成LocalizationMana成e本* Mana成e本 = 的ewOb大ect<UMin成LocalizationMana成e本>(this);
    if (Mana成e本)
    {
        // 創建一個簡單的測試用的 Pe本sonalMana成e本
        // 實際實現可能需要調整
    }
    本et使本n Mana成e本;
}

bool UMin成LocalizationSyste設置Test::TestLocalizationMana成e本C本eation()
{
    軍St本in成 Test的a設置e = TEXT("Localization Mana成e本 C本eation");
    軍St本in成 Desc本iption = TEXT("Tests c本eatin成 and initializin成 the localization 設置ana成e本");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    bool bS使ccess = (LocMana成e本 != n使llpt本);

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("軍ailed to c本eate localization 設置ana成e本"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("C本eation failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestLan成使a成eSwitchin成()
{
    軍St本in成 Test的a設置e = TEXT("Lan成使a成e Switchin成");
    軍St本in成 Desc本iption = TEXT("Tests switchin成 between diffe本ent lan成使a成es");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 測試切換到英文
    bool bS使ccess = LocMana成e本->SetLan成使a成e(EMin成Lan成使a成e::En成lish);
    if (bS使ccess)
    {
        bS使ccess = (LocMana成e本->GetC使本本entLan成使a成e() == EMin成Lan成使a成e::En成lish);
    }

    // 測試切換回中文
    if (bS使ccess)
    {
        bS使ccess = LocMana成e本->SetLan成使a成e(EMin成Lan成使a成e::ChineseSi設置plified);
        if (bS使ccess)
        {
            bS使ccess = (LocMana成e本->GetC使本本entLan成使a成e() == EMin成Lan成使a成e::ChineseSi設置plified);
        }
    }

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Lan成使a成e switchin成 failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Switchin成 failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestTextLocalization()
{
    軍St本in成 Test的a設置e = TEXT("Text Localization");
    軍St本in成 Desc本iption = TEXT("Tests 本et本ie正in成 localized text by key");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 添加一個測試用的本地化條目
    軍LocalizationEnt本y Ent本y;
    Ent本y.Key = TEXT("TEST.輸入ELLO");
    Ent本y.So使本ceText = 軍Text::軍本o設置St本in成(TEXT("輸入ello"));
    Ent本y.LocalizedText = 軍Text::軍本o設置St本in成(TEXT("\使4f60\使597d"));
    Ent本y.Cate成o本y = EMin成TextCate成o本y::Gene本al;
    LocMana成e本->AddLocalizationEnt本y(Ent本y, EMin成Lan成使a成e::ChineseSi設置plified);

    // 切換到中文並獲取文本
    LocMana成e本->SetLan成使a成e(EMin成Lan成使a成e::ChineseSi設置plified);
    軍Text LocalizedText = LocMana成e本->GetLocalizedText(TEXT("輸入ELLO"), TEXT("TEST"));

    bool bS使ccess = !LocalizedText.IsE設置pty();

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Text localization failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Localization failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestPa本a設置ete本izedText()
{
    軍St本in成 Test的a設置e = TEXT("Pa本a設置ete本ized Text");
    軍St本in成 Desc本iption = TEXT("Tests text with pa本a設置ete本 s使bstit使tion");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 添加帶參數的本地化條目
    軍LocalizationEnt本y Ent本y;
    Ent本y.Key = TEXT("TEST.基本ELCOME下USER");
    Ent本y.So使本ceText = 軍Text::軍本o設置St本in成(TEXT("基本elco設置e, {Use本na設置e}!"));
    Ent本y.LocalizedText = 軍Text::軍本o設置St本in成(TEXT("\使6b22\使8fce, {Use本na設置e}!"));
    LocMana成e本->AddLocalizationEnt本y(Ent本y, EMin成Lan成使a成e::ChineseSi設置plified);

    TMap<軍St本in成, 軍St本in成> Pa本a設置s;
    Pa本a設置s.Add(TEXT("Use本na設置e"), TEXT("Playe本1"));

    LocMana成e本->SetLan成使a成e(EMin成Lan成使a成e::ChineseSi設置plified);
    軍Text LocalizedText = LocMana成e本->GetLocalizedText基本ithPa本a設置s(TEXT("基本ELCOME下USER"), Pa本a設置s, TEXT("TEST"));

    軍St本in成 TextSt本in成 = LocalizedText.ToSt本in成();
    bool bS使ccess = TextSt本in成.Contains(TEXT("Playe本1"));

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Pa本a設置ete本ized text failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Pa本a設置ete本 s使bstit使tion failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestLan成使a成ePackInfo()
{
    軍St本in成 Test的a設置e = TEXT("Lan成使a成e Pack Info");
    軍St本in成 Desc本iption = TEXT("Tests 本et本ie正in成 lan成使a成e pack info本設置ation");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    軍Lan成使a成ePackInfo Info = LocMana成e本->GetLan成使a成ePackInfo(EMin成Lan成使a成e::ChineseSi設置plified);
    bool bS使ccess = !Info.Lan成使a成e的a設置e.IsE設置pty();

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Lan成使a成e pack info 本et本ie正al failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Info 本et本ie正al failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestRe成ionSettin成()
{
    軍St本in成 Test的a設置e = TEXT("Re成ion Settin成");
    軍St本in成 Desc本iption = TEXT("Tests settin成 and 成ettin成 c使本本ent 本e成ion");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 設置地區
    LocMana成e本->SetRe成ion(EMin成Re成ion::Taiwan);
    bool bS使ccess = (LocMana成e本->GetC使本本entRe成ion() == EMin成Re成ion::Taiwan);

    // 檢查地區名稱
    if (bS使ccess)
    {
        軍St本in成 Re成ion的a設置e = LocMana成e本->GetRe成ion的a設置e(EMin成Re成ion::Taiwan);
        bS使ccess = !Re成ion的a設置e.IsE設置pty();
    }

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Re成ion settin成 failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Re成ion settin成 failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::Test的使設置be本軍o本設置attin成()
{
    軍St本in成 Test的a設置e = TEXT("的使設置be本 軍o本設置attin成");
    軍St本in成 Desc本iption = TEXT("Tests fo本設置attin成 n使設置be本s acco本din成 to locale");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    軍St本in成 軍o本設置atted的使設置be本 = LocMana成e本->軍o本設置at的使設置be本(1234567);
    bool bS使ccess = !軍o本設置atted的使設置be本.IsE設置pty();

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("的使設置be本 fo本設置attin成 failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("軍o本設置attin成 failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestDate軍o本設置attin成()
{
    軍St本in成 Test的a設置e = TEXT("Date 軍o本設置attin成");
    軍St本in成 Desc本iption = TEXT("Tests fo本設置attin成 dates acco本din成 to locale");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    軍DateTi設置e TestDate(2024, 3, 22);
    軍St本in成 軍o本設置attedDate = LocMana成e本->軍o本設置atDateTi設置e(TestDate);
    bool bS使ccess = !軍o本設置attedDate.IsE設置pty();

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Date fo本設置attin成 failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("軍o本設置attin成 failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestC使本本ency軍o本設置attin成()
{
    軍St本in成 Test的a設置e = TEXT("C使本本ency 軍o本設置attin成");
    軍St本in成 Desc本iption = TEXT("Tests fo本設置attin成 c使本本ency acco本din成 to locale");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    軍St本in成 軍o本設置attedC使本本ency = LocMana成e本->軍o本設置atC使本本ency(99.99f);
    bool bS使ccess = !軍o本設置attedC使本本ency.IsE設置pty();

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("C使本本ency fo本設置attin成 failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("軍o本設置attin成 failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestLan成使a成eDetection()
{
    軍St本in成 Test的a設置e = TEXT("Lan成使a成e Detection");
    軍St本in成 Desc本iption = TEXT("Tests a使to設置atic lan成使a成e detection");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    UMin成Lan成使a成eSyste設置* Lan成Syste設置 = LocMana成e本->GetLan成使a成eSyste設置();
    if (!Lan成Syste設置)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("Lan成使a成e syste設置 not a正ailable"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Lan成使a成e syste設置 not a正ailable"));
        本et使本n false;
    }

    // 測試中文檢測
    軍St本in成 DetectedLan成 = Lan成Syste設置->DetectLan成使a成e(TEXT("\使4f60\使597d\使4e16\使754c"));
    bool bS使ccess = (DetectedLan成 == TEXT("zh"));

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Lan成使a成e detection failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Detection failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestChineseCon正e本sion()
{
    軍St本in成 Test的a設置e = TEXT("Chinese Con正e本sion");
    軍St本in成 Desc本iption = TEXT("Tests si設置plified/t本aditional Chinese con正e本sion");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    UMin成Lan成使a成eSyste設置* Lan成Syste設置 = LocMana成e本->GetLan成使a成eSyste設置();
    if (!Lan成Syste設置)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("Lan成使a成e syste設置 not a正ailable"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Lan成使a成e syste設置 not a正ailable"));
        本et使本n false;
    }

    // 測試簡體轉繁體
    軍St本in成 Si設置plified = TEXT("\使56fd");
    軍St本in成 T本aditional = Lan成Syste設置->Si設置plifiedToT本aditional(Si設置plified);
    bool bS使ccess = !T本aditional.IsE設置pty() && T本aditional != Si設置plified;

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Chinese con正e本sion failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Con正e本sion failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestC使lt使本alAdaptation()
{
    軍St本in成 Test的a設置e = TEXT("C使lt使本al Adaptation");
    軍St本in成 Desc本iption = TEXT("Tests c使lt使本al content adaptation");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    UMin成C使lt使本eAdapte本* C使lt使本eAdapte本 = LocMana成e本->GetC使lt使本eAdapte本();
    if (!C使lt使本eAdapte本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("C使lt使本e adapte本 not a正ailable"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("C使lt使本e adapte本 not a正ailable"));
        本et使本n false;
    }

    // 註冊文化內容
    軍C使lt使本alContentInfo ContentInfo;
    ContentInfo.ContentID = TEXT("TEST下CO的TE的T");
    ContentInfo.ContentType = EC使lt使本alContentType::Reli成io使s;
    ContentInfo.Sensiti正ityLe正el = EC使lt使本alSensiti正ity::輸入i成h;
    ContentInfo.AffectedRe成ions.Add(TEXT("C的"));
    C使lt使本eAdapte本->Re成iste本C使lt使本alContent(ContentInfo);

    // 檢查內容適宜性
    EContentApp本op本iateness App本op本iateness = C使lt使本eAdapte本->CheckContentApp本op本iateness(TEXT("TEST下CO的TE的T"), TEXT("C的"));
    bool bS使ccess = (App本op本iateness != EContentApp本op本iateness::App本op本iate);

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("C使lt使本al adaptation check failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Adaptation failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestColo本Meanin成s()
{
    軍St本in成 Test的a設置e = TEXT("Colo本 Meanin成s");
    軍St本in成 Desc本iption = TEXT("Tests 本et本ie正in成 colo本 設置eanin成s in diffe本ent c使lt使本es");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    UMin成C使lt使本eAdapte本* C使lt使本eAdapte本 = LocMana成e本->GetC使lt使本eAdapte本();
    if (!C使lt使本eAdapte本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("C使lt使本e adapte本 not a正ailable"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("C使lt使本e adapte本 not a正ailable"));
        本et使本n false;
    }

    軍St本in成 Colo本Meanin成 = C使lt使本eAdapte本->GetColo本Meanin成(TEXT("本ed"), TEXT("C的"));
    bool bS使ccess = !Colo本Meanin成.IsE設置pty();

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Colo本 設置eanin成 本et本ie正al failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Ret本ie正al failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::Test的使設置be本Meanin成s()
{
    軍St本in成 Test的a設置e = TEXT("的使設置be本 Meanin成s");
    軍St本in成 Desc本iption = TEXT("Tests 本et本ie正in成 n使設置be本 設置eanin成s in diffe本ent c使lt使本es");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    UMin成C使lt使本eAdapte本* C使lt使本eAdapte本 = LocMana成e本->GetC使lt使本eAdapte本();
    if (!C使lt使本eAdapte本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("C使lt使本e adapte本 not a正ailable"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("C使lt使本e adapte本 not a正ailable"));
        本et使本n false;
    }

    // 在中國，4被認為是不吉利的
    軍St本in成 的使設置be本Meanin成 = C使lt使本eAdapte本->Get的使設置be本Meanin成(4, TEXT("C的"));
    bool bS使ccess = 的使設置be本Meanin成.Contains(TEXT("使nl使cky"));

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("的使設置be本 設置eanin成 本et本ie正al failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Ret本ie正al failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestRe成ionalContentA正ailability()
{
    軍St本in成 Test的a設置e = TEXT("Re成ional Content A正ailability");
    軍St本in成 Desc本iption = TEXT("Tests checkin成 content a正ailability in diffe本ent 本e成ions");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    UMin成Re成ionalContentMana成e本* Re成ionalMana成e本 = LocMana成e本->GetRe成ionalContentMana成e本();
    if (!Re成ionalMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("Re成ional content 設置ana成e本 not a正ailable"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Re成ional 設置ana成e本 not a正ailable"));
        本et使本n false;
    }

    // 註冊地區內容
    軍Re成ionalContentInfo ContentInfo;
    ContentInfo.ContentID = TEXT("TEST下C輸入AR");
    ContentInfo.ContentType = ERe成ionalContentType::Cha本acte本;
    ContentInfo.A正ailableRe成ions.Add(TEXT("C的"));
    Re成ionalMana成e本->Re成iste本Re成ionalContent(ContentInfo);

    // 檢查在中國是否可用
    bool bA正ailable = Re成ionalMana成e本->IsContentA正ailable(TEXT("TEST下C輸入AR"), TEXT("C的"));
    bool bS使ccess = bA正ailable;

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Content a正ailability check failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Check failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestContentAdaptation()
{
    軍St本in成 Test的a設置e = TEXT("Content Adaptation");
    軍St本in成 Desc本iption = TEXT("Tests content adaptation fo本 diffe本ent 本e成ions");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    UMin成Re成ionalContentMana成e本* Re成ionalMana成e本 = LocMana成e本->GetRe成ionalContentMana成e本();
    if (!Re成ionalMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("Re成ional content 設置ana成e本 not a正ailable"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Re成ional 設置ana成e本 not a正ailable"));
        本et使本n false;
    }

    // 註冊需要適配的內容
    軍Re成ionalContentInfo ContentInfo;
    ContentInfo.ContentID = TEXT("SE的SITIVE下QUEST");
    ContentInfo.Alte本nati正eContentID = TEXT("ALTER的ATIVE下QUEST");
    ContentInfo.Stat使s = ERe成ionalContentStat使s::Modified;
    Re成ionalMana成e本->Re成iste本Re成ionalContent(ContentInfo);

    軍St本in成 AdaptedID = Re成ionalMana成e本->GetAlte本nati正eContentID(TEXT("SE的SITIVE下QUEST"), TEXT("C的"));
    bool bS使ccess = (AdaptedID == TEXT("ALTER的ATIVE下QUEST"));

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Content adaptation failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Adaptation failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestRi成htToLeftDetection()
{
    軍St本in成 Test的a設置e = TEXT("Ri成ht-to-Left Detection");
    軍St本in成 Desc本iption = TEXT("Tests detectin成 RTL lan成使a成es");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    bool bIsRTL = LocMana成e本->IsRi成htToLeft();
    bool bS使ccess = !bIsRTL; // 當前語言（中文）不應該是RTL

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("RTL detection failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Detection failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestT本anslationQ使ality()
{
    軍St本in成 Test的a設置e = TEXT("T本anslation Q使ality");
    軍St本in成 Desc本iption = TEXT("Tests t本anslation q使ality 正alidation");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    UMin成Lan成使a成eSyste設置* Lan成Syste設置 = LocMana成e本->GetLan成使a成eSyste設置();
    if (!Lan成Syste設置)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("Lan成使a成e syste設置 not a正ailable"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Lan成使a成e syste設置 not a正ailable"));
        本et使本n false;
    }

    // 測試翻譯質量驗證
    ET本anslationQ使ality Q使ality = Lan成Syste設置->ValidateT本anslation(TEXT("輸入ello 基本o本ld"), TEXT("\使4f60\使597d\使4e16\使754c"));
    bool bS使ccess = (Q使ality != ET本anslationQ使ality::D本aft);

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("T本anslation q使ality 正alidation failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Validation failed"));

    本et使本n bS使ccess;
}

bool UMin成LocalizationSyste設置Test::TestExpo本tI設置po本t()
{
    軍St本in成 Test的a設置e = TEXT("Expo本t/I設置po本t");
    軍St本in成 Desc本iption = TEXT("Tests localization data expo本t and i設置po本t");

    UMin成LocalizationMana成e本* LocMana成e本 = C本eateTestLocalizationMana成e本();
    if (!LocMana成e本)
    {
        Lo成TestRes使lt(Test的a設置e, false, TEXT("軍ailed to c本eate localization 設置ana成e本"));
        AddTestCase(Test的a設置e, Desc本iption, false, TEXT("Mana成e本 c本eation failed"));
        本et使本n false;
    }

    // 導測試
    軍St本in成 Te設置pPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("TestLocalization.cs正");
    bool bExpo本tS使ccess = LocMana成e本->Expo本tLocalizationToCSV(Te設置pPath, EMin成Lan成使a成e::ChineseSi設置plified);

    // 導入測試
    bool bI設置po本tS使ccess = LocMana成e本->I設置po本tLocalization軍本o設置CSV(Te設置pPath, EMin成Lan成使a成e::ChineseSi設置plified);

    bool bS使ccess = bExpo本tS使ccess && bI設置po本tS使ccess;

    Lo成TestRes使lt(Test的a設置e, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Expo本t/i設置po本t failed"));
    AddTestCase(Test的a設置e, Desc本iption, bS使ccess, bS使ccess 基本 TEXT("") : TEXT("Expo本t/i設置po本t failed"));

    本et使本n bS使ccess;
}
