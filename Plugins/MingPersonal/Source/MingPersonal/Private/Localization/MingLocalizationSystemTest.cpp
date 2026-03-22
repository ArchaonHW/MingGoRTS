#include "Localization/MingLocalizationSystemTest.h"
#include "Localization/MingLocalizationManager.h"
#include "Localization/MingLanguageSystem.h"
#include "Localization/MingCultureAdapter.h"
#include "Localization/MingRegionalContentManager.h"

UMingLocalizationSystemTest::UMingLocalizationSystemTest()
    : PassedTests(0)
    , FailedTests(0)
{
}

bool UMingLocalizationSystemTest::RunAllTests()
{
    ResetTestCounts();
    TestCases.Empty();
    TestResults = TEXT("=== MingGoRTS Localization System Test Results ===\n\n");

    // 運行所有測試
    TestLocalizationManagerCreation();
    TestLanguageSwitching();
    TestTextLocalization();
    TestParameterizedText();
    TestLanguagePackInfo();
    TestRegionSetting();
    TestNumberFormatting();
    TestDateFormatting();
    TestCurrencyFormatting();
    TestLanguageDetection();
    TestChineseConversion();
    TestCulturalAdaptation();
    TestColorMeanings();
    TestNumberMeanings();
    TestRegionalContentAvailability();
    TestContentAdaptation();
    TestRightToLeftDetection();
    TestTranslationQuality();
    TestExportImport();

    // 生成總結報告
    TestResults += FString::Printf(TEXT("\n=== Test Summary ===\n"));
    TestResults += FString::Printf(TEXT("Passed: %d\n"), PassedTests);
    TestResults += FString::Printf(TEXT("Failed: %d\n"), FailedTests);
    TestResults += FString::Printf(TEXT("Pass Rate: %.1f%%\n"), GetTestPassRate());

    if (FailedTests > 0)
    {
        TestResults += TEXT("\n=== Errors ===\n");
        for (const FString& Error : ErrorMessages)
        {
            TestResults += Error + TEXT("\n");
        }
    }

    return FailedTests == 0;
}

float UMingLocalizationSystemTest::GetTestPassRate() const
{
    int32 TotalTests = PassedTests + FailedTests;
    if (TotalTests == 0)
    {
        return 0.0f;
    }
    return (static_cast<float>(PassedTests) / TotalTests) * 100.0f;
}

void UMingLocalizationSystemTest::LogTestResult(const FString& TestName, bool bPassed, const FString& Message)
{
    if (bPassed)
    {
        PassedTests++;
        TestResults += FString::Printf(TEXT("[PASS] %s\n"), *TestName);
    }
    else
    {
        FailedTests++;
        FString ErrorMsg = FString::Printf(TEXT("[FAIL] %s: %s"), *TestName, *Message);
        TestResults += ErrorMsg + TEXT("\n");
        ErrorMessages.Add(ErrorMsg);
    }
}

void UMingLocalizationSystemTest::ResetTestCounts()
{
    PassedTests = 0;
    FailedTests = 0;
    ErrorMessages.Empty();
    TestResults.Empty();
}

void UMingLocalizationSystemTest::AddTestCase(const FString& TestName, const FString& Description, bool bPassed, const FString& Error)
{
    FLocalizationTestCase TestCase;
    TestCase.TestName = TestName;
    TestCase.Description = Description;
    TestCase.Result = bPassed ? ELocalizationTestResult::Passed : ELocalizationTestResult::Failed;
    TestCase.ErrorMessage = Error;
    TestCases.Add(TestCase);
}

UMingLocalizationManager* UMingLocalizationSystemTest::CreateTestLocalizationManager()
{
    UMingLocalizationManager* Manager = NewObject<UMingLocalizationManager>(this);
    if (Manager)
    {
        // 創建一個簡單的測試用的 PersonalManager
        // 實際實現可能需要調整
    }
    return Manager;
}

bool UMingLocalizationSystemTest::TestLocalizationManagerCreation()
{
    FString TestName = TEXT("Localization Manager Creation");
    FString Description = TEXT("Tests creating and initializing the localization manager");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    bool bSuccess = (LocManager != nullptr);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to create localization manager"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Creation failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestLanguageSwitching()
{
    FString TestName = TEXT("Language Switching");
    FString Description = TEXT("Tests switching between different languages");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 測試切換到英文
    bool bSuccess = LocManager->SetLanguage(EMingLanguage::English);
    if (bSuccess)
    {
        bSuccess = (LocManager->GetCurrentLanguage() == EMingLanguage::English);
    }

    // 測試切換回中文
    if (bSuccess)
    {
        bSuccess = LocManager->SetLanguage(EMingLanguage::ChineseSimplified);
        if (bSuccess)
        {
            bSuccess = (LocManager->GetCurrentLanguage() == EMingLanguage::ChineseSimplified);
        }
    }

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Language switching failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Switching failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestTextLocalization()
{
    FString TestName = TEXT("Text Localization");
    FString Description = TEXT("Tests retrieving localized text by key");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 添加一個測試用的本地化條目
    FLocalizationEntry Entry;
    Entry.Key = TEXT("TEST.HELLO");
    Entry.SourceText = FText::FromString(TEXT("Hello"));
    Entry.LocalizedText = FText::FromString(TEXT("\u4f60\u597d"));
    Entry.Category = EMingTextCategory::General;
    LocManager->AddLocalizationEntry(Entry, EMingLanguage::ChineseSimplified);

    // 切換到中文並獲取文本
    LocManager->SetLanguage(EMingLanguage::ChineseSimplified);
    FText LocalizedText = LocManager->GetLocalizedText(TEXT("HELLO"), TEXT("TEST"));

    bool bSuccess = !LocalizedText.IsEmpty();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Text localization failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Localization failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestParameterizedText()
{
    FString TestName = TEXT("Parameterized Text");
    FString Description = TEXT("Tests text with parameter substitution");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 添加帶參數的本地化條目
    FLocalizationEntry Entry;
    Entry.Key = TEXT("TEST.WELCOME_USER");
    Entry.SourceText = FText::FromString(TEXT("Welcome, {Username}!"));
    Entry.LocalizedText = FText::FromString(TEXT("\u6b22\u8fce, {Username}!"));
    LocManager->AddLocalizationEntry(Entry, EMingLanguage::ChineseSimplified);

    TMap<FString, FString> Params;
    Params.Add(TEXT("Username"), TEXT("Player1"));

    LocManager->SetLanguage(EMingLanguage::ChineseSimplified);
    FText LocalizedText = LocManager->GetLocalizedTextWithParams(TEXT("WELCOME_USER"), Params, TEXT("TEST"));

    FString TextString = LocalizedText.ToString();
    bool bSuccess = TextString.Contains(TEXT("Player1"));

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Parameterized text failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Parameter substitution failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestLanguagePackInfo()
{
    FString TestName = TEXT("Language Pack Info");
    FString Description = TEXT("Tests retrieving language pack information");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    FLanguagePackInfo Info = LocManager->GetLanguagePackInfo(EMingLanguage::ChineseSimplified);
    bool bSuccess = !Info.LanguageName.IsEmpty();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Language pack info retrieval failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Info retrieval failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestRegionSetting()
{
    FString TestName = TEXT("Region Setting");
    FString Description = TEXT("Tests setting and getting current region");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 設置地區
    LocManager->SetRegion(EMingRegion::Taiwan);
    bool bSuccess = (LocManager->GetCurrentRegion() == EMingRegion::Taiwan);

    // 檢查地區名稱
    if (bSuccess)
    {
        FString RegionName = LocManager->GetRegionName(EMingRegion::Taiwan);
        bSuccess = !RegionName.IsEmpty();
    }

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Region setting failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Region setting failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestNumberFormatting()
{
    FString TestName = TEXT("Number Formatting");
    FString Description = TEXT("Tests formatting numbers according to locale");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    FString FormattedNumber = LocManager->FormatNumber(1234567);
    bool bSuccess = !FormattedNumber.IsEmpty();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Number formatting failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Formatting failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestDateFormatting()
{
    FString TestName = TEXT("Date Formatting");
    FString Description = TEXT("Tests formatting dates according to locale");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    FDateTime TestDate(2024, 3, 22);
    FString FormattedDate = LocManager->FormatDateTime(TestDate);
    bool bSuccess = !FormattedDate.IsEmpty();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Date formatting failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Formatting failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestCurrencyFormatting()
{
    FString TestName = TEXT("Currency Formatting");
    FString Description = TEXT("Tests formatting currency according to locale");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    FString FormattedCurrency = LocManager->FormatCurrency(99.99f);
    bool bSuccess = !FormattedCurrency.IsEmpty();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Currency formatting failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Formatting failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestLanguageDetection()
{
    FString TestName = TEXT("Language Detection");
    FString Description = TEXT("Tests automatic language detection");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    UMingLanguageSystem* LangSystem = LocManager->GetLanguageSystem();
    if (!LangSystem)
    {
        LogTestResult(TestName, false, TEXT("Language system not available"));
        AddTestCase(TestName, Description, false, TEXT("Language system not available"));
        return false;
    }

    // 測試中文檢測
    FString DetectedLang = LangSystem->DetectLanguage(TEXT("\u4f60\u597d\u4e16\u754c"));
    bool bSuccess = (DetectedLang == TEXT("zh"));

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Language detection failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Detection failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestChineseConversion()
{
    FString TestName = TEXT("Chinese Conversion");
    FString Description = TEXT("Tests simplified/traditional Chinese conversion");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    UMingLanguageSystem* LangSystem = LocManager->GetLanguageSystem();
    if (!LangSystem)
    {
        LogTestResult(TestName, false, TEXT("Language system not available"));
        AddTestCase(TestName, Description, false, TEXT("Language system not available"));
        return false;
    }

    // 測試簡體轉繁體
    FString Simplified = TEXT("\u56fd");
    FString Traditional = LangSystem->SimplifiedToTraditional(Simplified);
    bool bSuccess = !Traditional.IsEmpty() && Traditional != Simplified;

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Chinese conversion failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Conversion failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestCulturalAdaptation()
{
    FString TestName = TEXT("Cultural Adaptation");
    FString Description = TEXT("Tests cultural content adaptation");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    UMingCultureAdapter* CultureAdapter = LocManager->GetCultureAdapter();
    if (!CultureAdapter)
    {
        LogTestResult(TestName, false, TEXT("Culture adapter not available"));
        AddTestCase(TestName, Description, false, TEXT("Culture adapter not available"));
        return false;
    }

    // 註冊文化內容
    FCulturalContentInfo ContentInfo;
    ContentInfo.ContentID = TEXT("TEST_CONTENT");
    ContentInfo.ContentType = ECulturalContentType::Religious;
    ContentInfo.SensitivityLevel = ECulturalSensitivity::High;
    ContentInfo.AffectedRegions.Add(TEXT("CN"));
    CultureAdapter->RegisterCulturalContent(ContentInfo);

    // 檢查內容適宜性
    EContentAppropriateness Appropriateness = CultureAdapter->CheckContentAppropriateness(TEXT("TEST_CONTENT"), TEXT("CN"));
    bool bSuccess = (Appropriateness != EContentAppropriateness::Appropriate);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Cultural adaptation check failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Adaptation failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestColorMeanings()
{
    FString TestName = TEXT("Color Meanings");
    FString Description = TEXT("Tests retrieving color meanings in different cultures");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    UMingCultureAdapter* CultureAdapter = LocManager->GetCultureAdapter();
    if (!CultureAdapter)
    {
        LogTestResult(TestName, false, TEXT("Culture adapter not available"));
        AddTestCase(TestName, Description, false, TEXT("Culture adapter not available"));
        return false;
    }

    FString ColorMeaning = CultureAdapter->GetColorMeaning(TEXT("red"), TEXT("CN"));
    bool bSuccess = !ColorMeaning.IsEmpty();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Color meaning retrieval failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Retrieval failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestNumberMeanings()
{
    FString TestName = TEXT("Number Meanings");
    FString Description = TEXT("Tests retrieving number meanings in different cultures");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    UMingCultureAdapter* CultureAdapter = LocManager->GetCultureAdapter();
    if (!CultureAdapter)
    {
        LogTestResult(TestName, false, TEXT("Culture adapter not available"));
        AddTestCase(TestName, Description, false, TEXT("Culture adapter not available"));
        return false;
    }

    // 在中國，4被認為是不吉利的
    FString NumberMeaning = CultureAdapter->GetNumberMeaning(4, TEXT("CN"));
    bool bSuccess = NumberMeaning.Contains(TEXT("unlucky"));

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Number meaning retrieval failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Retrieval failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestRegionalContentAvailability()
{
    FString TestName = TEXT("Regional Content Availability");
    FString Description = TEXT("Tests checking content availability in different regions");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    UMingRegionalContentManager* RegionalManager = LocManager->GetRegionalContentManager();
    if (!RegionalManager)
    {
        LogTestResult(TestName, false, TEXT("Regional content manager not available"));
        AddTestCase(TestName, Description, false, TEXT("Regional manager not available"));
        return false;
    }

    // 註冊地區內容
    FRegionalContentInfo ContentInfo;
    ContentInfo.ContentID = TEXT("TEST_CHAR");
    ContentInfo.ContentType = ERegionalContentType::Character;
    ContentInfo.AvailableRegions.Add(TEXT("CN"));
    RegionalManager->RegisterRegionalContent(ContentInfo);

    // 檢查在中國是否可用
    bool bAvailable = RegionalManager->IsContentAvailable(TEXT("TEST_CHAR"), TEXT("CN"));
    bool bSuccess = bAvailable;

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Content availability check failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Check failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestContentAdaptation()
{
    FString TestName = TEXT("Content Adaptation");
    FString Description = TEXT("Tests content adaptation for different regions");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    UMingRegionalContentManager* RegionalManager = LocManager->GetRegionalContentManager();
    if (!RegionalManager)
    {
        LogTestResult(TestName, false, TEXT("Regional content manager not available"));
        AddTestCase(TestName, Description, false, TEXT("Regional manager not available"));
        return false;
    }

    // 註冊需要適配的內容
    FRegionalContentInfo ContentInfo;
    ContentInfo.ContentID = TEXT("SENSITIVE_QUEST");
    ContentInfo.AlternativeContentID = TEXT("ALTERNATIVE_QUEST");
    ContentInfo.Status = ERegionalContentStatus::Modified;
    RegionalManager->RegisterRegionalContent(ContentInfo);

    FString AdaptedID = RegionalManager->GetAlternativeContentID(TEXT("SENSITIVE_QUEST"), TEXT("CN"));
    bool bSuccess = (AdaptedID == TEXT("ALTERNATIVE_QUEST"));

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Content adaptation failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Adaptation failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestRightToLeftDetection()
{
    FString TestName = TEXT("Right-to-Left Detection");
    FString Description = TEXT("Tests detecting RTL languages");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    bool bIsRTL = LocManager->IsRightToLeft();
    bool bSuccess = !bIsRTL; // 當前語言（中文）不應該是RTL

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("RTL detection failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Detection failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestTranslationQuality()
{
    FString TestName = TEXT("Translation Quality");
    FString Description = TEXT("Tests translation quality validation");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    UMingLanguageSystem* LangSystem = LocManager->GetLanguageSystem();
    if (!LangSystem)
    {
        LogTestResult(TestName, false, TEXT("Language system not available"));
        AddTestCase(TestName, Description, false, TEXT("Language system not available"));
        return false;
    }

    // 測試翻譯質量驗證
    ETranslationQuality Quality = LangSystem->ValidateTranslation(TEXT("Hello World"), TEXT("\u4f60\u597d\u4e16\u754c"));
    bool bSuccess = (Quality != ETranslationQuality::Draft);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Translation quality validation failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Validation failed"));

    return bSuccess;
}

bool UMingLocalizationSystemTest::TestExportImport()
{
    FString TestName = TEXT("Export/Import");
    FString Description = TEXT("Tests localization data export and import");

    UMingLocalizationManager* LocManager = CreateTestLocalizationManager();
    if (!LocManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create localization manager"));
        AddTestCase(TestName, Description, false, TEXT("Manager creation failed"));
        return false;
    }

    // 導出測試
    FString TempPath = FPaths::ProjectSavedDir() / TEXT("TestLocalization.csv");
    bool bExportSuccess = LocManager->ExportLocalizationToCSV(TempPath, EMingLanguage::ChineseSimplified);

    // 導入測試
    bool bImportSuccess = LocManager->ImportLocalizationFromCSV(TempPath, EMingLanguage::ChineseSimplified);

    bool bSuccess = bExportSuccess && bImportSuccess;

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Export/import failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Export/import failed"));

    return bSuccess;
}
