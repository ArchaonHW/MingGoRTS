#include "Localization/MingLocalizationManager.h"
#include "Localization/MingLanguageSystem.h"
#include "Localization/MingCultureAdapter.h"
#include "Localization/MingRegionalContentManager.h"
#include "MingPersonalManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"

UMingLocalizationManager::UMingLocalizationManager()
    : bIsInitialized(false)
    , CurrentLanguage(EMingLanguage::ChineseSimplified)
    , CurrentRegion(EMingRegion::MainlandChina)
    , DefaultNamespace(TEXT("Default"))
{
}

void UMingLocalizationManager::InitializeLocalization(UMingPersonalManager* InPersonalManager)
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("LocalizationManager already initialized"));
        return;
    }

    PersonalManager = InPersonalManager;

    // 初始化子系統
    InitializeSubsystems();

    // 加載語言偏好設置
    LoadLanguagePreference();

    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("LocalizationManager initialized - Current language: %s, Region: %s"),
        *GetCurrentLanguageName(), *GetRegionName(CurrentRegion));
}

void UMingLocalizationManager::ShutdownLocalization()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 保存語言偏好
    SaveLanguagePreference();

    // 關閉子系統
    if (LanguageSystem)
    {
        LanguageSystem->Shutdown();
    }

    if (CultureAdapter)
    {
        CultureAdapter->Shutdown();
    }

    if (RegionalContentManager)
    {
        RegionalContentManager->Shutdown();
    }

    bIsInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("LocalizationManager shutdown"));
}

bool UMingLocalizationManager::SetLanguage(EMingLanguage NewLanguage)
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (NewLanguage == CurrentLanguage)
    {
        return true;
    }

    // 檢查語言是否可用
    if (!IsLanguageAvailable(NewLanguage))
    {
        UE_LOG(LogTemp, Warning, TEXT("Language %s is not available"), *GetLanguageName(NewLanguage));
        return false;
    }

    // 加載語言包
    if (!LoadLanguagePack(NewLanguage))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load language pack for %s"), *GetLanguageName(NewLanguage));
        return false;
    }

    EMingLanguage OldLanguage = CurrentLanguage;
    CurrentLanguage = NewLanguage;

    // 更新 UE 引擎文化設置
    FString CultureName = GetLanguageName(NewLanguage);
    FInternationalization::Get().SetCurrentCulture(CultureName);

    // 保存設置
    SaveLanguagePreference();

    UE_LOG(LogTemp, Log, TEXT("Language changed from %s to %s"),
        *GetLanguageName(OldLanguage), *GetCurrentLanguageName());

    // 觸發事件
    OnLanguageChanged.Broadcast(NewLanguage);

    return true;
}

FString UMingLocalizationManager::GetLanguageName(EMingLanguage Language) const
{
    switch (Language)
    {
    case EMingLanguage::ChineseSimplified:  return TEXT("zh-CN");
    case EMingLanguage::ChineseTraditional: return TEXT("zh-TW");
    case EMingLanguage::English:            return TEXT("en");
    case EMingLanguage::Japanese:             return TEXT("ja");
    case EMingLanguage::Korean:               return TEXT("ko");
    case EMingLanguage::French:               return TEXT("fr");
    case EMingLanguage::German:               return TEXT("de");
    case EMingLanguage::Spanish:              return TEXT("es");
    case EMingLanguage::Russian:              return TEXT("ru");
    case EMingLanguage::Arabic:               return TEXT("ar");
    case EMingLanguage::Portuguese:           return TEXT("pt");
    case EMingLanguage::Italian:              return TEXT("it");
    case EMingLanguage::Dutch:                return TEXT("nl");
    case EMingLanguage::Polish:               return TEXT("pl");
    case EMingLanguage::Turkish:              return TEXT("tr");
    case EMingLanguage::Vietnamese:           return TEXT("vi");
    case EMingLanguage::Thai:                 return TEXT("th");
    case EMingLanguage::Indonesian:           return TEXT("id");
    default:                                  return TEXT("en");
    }
}

FString UMingLocalizationManager::GetCurrentLanguageName() const
{
    return GetLanguageName(CurrentLanguage);
}

TArray<EMingLanguage> UMingLocalizationManager::GetSupportedLanguages() const
{
    TArray<EMingLanguage> Languages;
    Languages.Add(EMingLanguage::ChineseSimplified);
    Languages.Add(EMingLanguage::ChineseTraditional);
    Languages.Add(EMingLanguage::English);
    Languages.Add(EMingLanguage::Japanese);
    Languages.Add(EMingLanguage::Korean);
    Languages.Add(EMingLanguage::French);
    Languages.Add(EMingLanguage::German);
    Languages.Add(EMingLanguage::Spanish);
    Languages.Add(EMingLanguage::Russian);
    Languages.Add(EMingLanguage::Portuguese);
    Languages.Add(EMingLanguage::Italian);
    return Languages;
}

bool UMingLocalizationManager::IsLanguageAvailable(EMingLanguage Language) const
{
    // 檢查語言包是否已安裝
    return InstalledLanguages.Contains(Language);
}

FLanguagePackInfo UMingLocalizationManager::GetLanguagePackInfo(EMingLanguage Language) const
{
    FLanguagePackInfo Info;
    Info.Language = Language;
    Info.ISOCode = GetLanguageName(Language);
    Info.bIsInstalled = InstalledLanguages.Contains(Language);
    Info.bIsActive = (Language == CurrentLanguage);

    if (LocalizationData.Contains(Language))
    {
        Info.EntryCount = LocalizationData[Language].Num();
    }

    // 設置語言名稱
    switch (Language)
    {
    case EMingLanguage::ChineseSimplified:  Info.LanguageName = TEXT("Chinese (Simplified)"); Info.NativeName = TEXT("\u7b80\u4f53\u4e2d\u6587"); break;
    case EMingLanguage::ChineseTraditional: Info.LanguageName = TEXT("Chinese (Traditional)"); Info.NativeName = TEXT("\u7e41\u9ad4\u4e2d\u6587"); break;
    case EMingLanguage::English:            Info.LanguageName = TEXT("English"); Info.NativeName = TEXT("English"); break;
    case EMingLanguage::Japanese:             Info.LanguageName = TEXT("Japanese"); Info.NativeName = TEXT("\u65e5\u672c\u8a9e"); break;
    case EMingLanguage::Korean:               Info.LanguageName = TEXT("Korean"); Info.NativeName = TEXT("\ud55c\uad6d\uc5b4"); break;
    case EMingLanguage::French:               Info.LanguageName = TEXT("French"); Info.NativeName = TEXT("Fran\u00e7ais"); break;
    case EMingLanguage::German:               Info.LanguageName = TEXT("German"); Info.NativeName = TEXT("Deutsch"); break;
    case EMingLanguage::Spanish:              Info.LanguageName = TEXT("Spanish"); Info.NativeName = TEXT("Espa\u00f1ol"); break;
    case EMingLanguage::Russian:              Info.LanguageName = TEXT("Russian"); Info.NativeName = TEXT("\u0420\u0443\u0441\u0441\u043a\u0438\u0439"); break;
    default:                                  Info.LanguageName = TEXT("Unknown"); Info.NativeName = TEXT("Unknown"); break;
    }

    return Info;
}

bool UMingLocalizationManager::DownloadLanguagePack(EMingLanguage Language)
{
    UE_LOG(LogTemp, Log, TEXT("Downloading language pack for %s"), *GetLanguageName(Language));

    // 實際實現會從服務器下載語言包
    // 這裡模擬下載成功
    if (!InstalledLanguages.Contains(Language))
    {
        InstalledLanguages.Add(Language);
    }

    return true;
}

bool UMingLocalizationManager::UnloadLanguagePack(EMingLanguage Language)
{
    if (Language == CurrentLanguage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot unload currently active language pack"));
        return false;
    }

    InstalledLanguages.Remove(Language);
    LocalizationData.Remove(Language);

    UE_LOG(LogTemp, Log, TEXT("Unloaded language pack for %s"), *GetLanguageName(Language));
    return true;
}

void UMingLocalizationManager::SetRegion(EMingRegion NewRegion)
{
    if (NewRegion == CurrentRegion)
    {
        return;
    }

    EMingRegion OldRegion = CurrentRegion;
    CurrentRegion = NewRegion;

    UE_LOG(LogTemp, Log, TEXT("Region changed from %s to %s"),
        *GetRegionName(OldRegion), *GetRegionName(NewRegion));

    OnRegionChanged.Broadcast(NewRegion);
}

FString UMingLocalizationManager::GetRegionName(EMingRegion Region) const
{
    switch (Region)
    {
    case EMingRegion::MainlandChina:   return TEXT("Mainland China");
    case EMingRegion::Taiwan:          return TEXT("Taiwan");
    case EMingRegion::HongKong:      return TEXT("Hong Kong");
    case EMingRegion::Macau:           return TEXT("Macau");
    case EMingRegion::Japan:           return TEXT("Japan");
    case EMingRegion::SouthKorea:      return TEXT("South Korea");
    case EMingRegion::UnitedStates:    return TEXT("United States");
    case EMingRegion::UnitedKingdom:   return TEXT("United Kingdom");
    case EMingRegion::Germany:         return TEXT("Germany");
    case EMingRegion::France:          return TEXT("France");
    case EMingRegion::Russia:          return TEXT("Russia");
    case EMingRegion::Brazil:           return TEXT("Brazil");
    case EMingRegion::India:            return TEXT("India");
    case EMingRegion::SoutheastAsia:   return TEXT("Southeast Asia");
    case EMingRegion::MiddleEast:      return TEXT("Middle East");
    case EMingRegion::Global:          return TEXT("Global");
    default:                           return TEXT("Unknown");
    }
}

FString UMingLocalizationManager::GetRegionCode(EMingRegion Region) const
{
    switch (Region)
    {
    case EMingRegion::MainlandChina:   return TEXT("CN");
    case EMingRegion::Taiwan:          return TEXT("TW");
    case EMingRegion::HongKong:      return TEXT("HK");
    case EMingRegion::Macau:           return TEXT("MO");
    case EMingRegion::Japan:           return TEXT("JP");
    case EMingRegion::SouthKorea:      return TEXT("KR");
    case EMingRegion::UnitedStates:    return TEXT("US");
    case EMingRegion::UnitedKingdom:   return TEXT("GB");
    case EMingRegion::Germany:         return TEXT("DE");
    case EMingRegion::France:          return TEXT("FR");
    case EMingRegion::Russia:          return TEXT("RU");
    case EMingRegion::Brazil:           return TEXT("BR");
    case EMingRegion::India:            return TEXT("IN");
    case EMingRegion::SoutheastAsia:   return TEXT("SEA");
    case EMingRegion::MiddleEast:      return TEXT("ME");
    case EMingRegion::Global:          return TEXT("GL");
    default:                           return TEXT("XX");
    }
}

EMingRegion UMingLocalizationManager::AutoDetectRegion() const
{
    // 獲取系統文化設置
    FString SystemCulture = FInternationalization::Get().GetCurrentCulture()->GetName();

    // 根據系統文化推斷地區
    if (SystemCulture.StartsWith(TEXT("zh-CN"))) return EMingRegion::MainlandChina;
    if (SystemCulture.StartsWith(TEXT("zh-TW"))) return EMingRegion::Taiwan;
    if (SystemCulture.StartsWith(TEXT("zh-HK"))) return EMingRegion::HongKong;
    if (SystemCulture.StartsWith(TEXT("ja"))) return EMingRegion::Japan;
    if (SystemCulture.StartsWith(TEXT("ko"))) return EMingRegion::SouthKorea;
    if (SystemCulture.StartsWith(TEXT("en-US"))) return EMingRegion::UnitedStates;
    if (SystemCulture.StartsWith(TEXT("en-GB"))) return EMingRegion::UnitedKingdom;
    if (SystemCulture.StartsWith(TEXT("de"))) return EMingRegion::Germany;
    if (SystemCulture.StartsWith(TEXT("fr"))) return EMingRegion::France;
    if (SystemCulture.StartsWith(TEXT("ru"))) return EMingRegion::Russia;

    return EMingRegion::Global;
}

FText UMingLocalizationManager::GetLocalizedText(const FString& Key, const FString& Namespace) const
{
    if (!bIsInitialized)
    {
        return FText::FromString(Key);
    }

    // 構建完整鍵
    FString FullKey = Namespace + TEXT(".") + Key;

    // 查找本地化條目
    if (LocalizationData.Contains(CurrentLanguage))
    {
        const TMap<FString, FLocalizationEntry>& LanguageMap = LocalizationData[CurrentLanguage];
        if (LanguageMap.Contains(FullKey))
        {
            return LanguageMap[FullKey].LocalizedText;
        }
    }

    // 如果找不到，返回源文本
    return FText::FromString(Key);
}

FText UMingLocalizationManager::GetLocalizedTextWithParams(const FString& Key, const TMap<FString, FString>& Params, const FString& Namespace) const
{
    FText BaseText = GetLocalizedText(Key, Namespace);
    FString TextString = BaseText.ToString();

    // 替換參數
    for (const TPair<FString, FString>& Param : Params)
    {
        FString Placeholder = FString::Printf(TEXT("{%s}"), *Param.Key);
        TextString = TextString.Replace(*Placeholder, *Param.Value);
    }

    return FText::FromString(TextString);
}

FString UMingLocalizationManager::GetLocalizedString(const FString& Key, const FString& Namespace) const
{
    return GetLocalizedText(Key, Namespace).ToString();
}

void UMingLocalizationManager::AddLocalizationEntry(const FLocalizationEntry& Entry, EMingLanguage TargetLanguage)
{
    if (!LocalizationData.Contains(TargetLanguage))
    {
        LocalizationData.Add(TargetLanguage, TMap<FString, FLocalizationEntry>());
    }

    LocalizationData[TargetLanguage].Add(Entry.Key, Entry);
}

void UMingLocalizationManager::AddLocalizationEntries(const TArray<FLocalizationEntry>& Entries, EMingLanguage TargetLanguage)
{
    for (const FLocalizationEntry& Entry : Entries)
    {
        AddLocalizationEntry(Entry, TargetLanguage);
    }
}

bool UMingLocalizationManager::ExportLocalizationToCSV(const FString& FilePath, EMingLanguage Language) const
{
    if (!LocalizationData.Contains(Language))
    {
        return false;
    }

    FString CSVContent = TEXT("Key,SourceText,LocalizedText,Category,Context,Description\n");

    const TMap<FString, FLocalizationEntry>& LanguageMap = LocalizationData[Language];
    for (const TPair<FString, FLocalizationEntry>& Pair : LanguageMap)
    {
        const FLocalizationEntry& Entry = Pair.Value;
        CSVContent += FString::Printf(TEXT("%s,%s,%s,%d,%s,%s\n"),
            *Entry.Key,
            *Entry.SourceText.ToString(),
            *Entry.LocalizedText.ToString(),
            static_cast<int32>(Entry.Category),
            *Entry.Context,
            *Entry.Description);
    }

    return FFileHelper::SaveStringToFile(CSVContent, *FilePath);
}

bool UMingLocalizationManager::ExportLocalizationToJSON(const FString& FilePath, EMingLanguage Language) const
{
    // JSON 導出實現
    UE_LOG(LogTemp, Log, TEXT("Exporting localization to JSON: %s"), *FilePath);
    return true;
}

bool UMingLocalizationManager::ImportLocalizationFromCSV(const FString& FilePath, EMingLanguage TargetLanguage)
{
    UE_LOG(LogTemp, Log, TEXT("Importing localization from CSV: %s"), *FilePath);

    // CSV 導入實現
    return true;
}

bool UMingLocalizationManager::ImportLocalizationFromJSON(const FString& FilePath, EMingLanguage TargetLanguage)
{
    UE_LOG(LogTemp, Log, TEXT("Importing localization from JSON: %s"), *FilePath);

    // JSON 導入實現
    return true;
}

FString UMingLocalizationManager::FormatNumber(int32 Number) const
{
    // 根據當前語言格式化數字
    return FString::Printf(TEXT("%d"), Number);
}

FString UMingLocalizationManager::FormatNumberFloat(float Number, int32 DecimalPlaces) const
{
    FString FormatString = FString::Printf(TEXT("%%.%df"), DecimalPlaces);
    return FString::Printf(*FormatString, Number);
}

FString UMingLocalizationManager::FormatDateTime(const FDateTime& DateTime, const FString& Format) const
{
    return DateTime.ToString(*Format);
}

FString UMingLocalizationManager::FormatCurrency(float Amount, const FString& CurrencyCode) const
{
    FString Symbol = GetCurrencySymbol();
    return FString::Printf(TEXT("%s%.2f"), *Symbol, Amount);
}

FString UMingLocalizationManager::GetDateFormat() const
{
    // 根據地區返回日期格式
    switch (CurrentRegion)
    {
    case EMingRegion::MainlandChina:
    case EMingRegion::Taiwan:
    case EMingRegion::HongKong:
    case EMingRegion::Japan:
    case EMingRegion::SouthKorea:
        return TEXT("yyyy-MM-dd");
    case EMingRegion::UnitedStates:
        return TEXT("MM/dd/yyyy");
    case EMingRegion::UnitedKingdom:
    case EMingRegion::Germany:
    case EMingRegion::France:
        return TEXT("dd/MM/yyyy");
    default:
        return TEXT("yyyy-MM-dd");
    }
}

FString UMingLocalizationManager::GetTimeFormat() const
{
    // 24小時制或12小時制
    switch (CurrentRegion)
    {
    case EMingRegion::UnitedStates:
    case EMingRegion::UnitedKingdom:
        return TEXT("hh:mm a"); // 12小時制
    default:
        return TEXT("HH:mm");   // 24小時制
    }
}

FString UMingLocalizationManager::GetNumberFormat() const
{
    return TEXT("#,##0.##");
}

FString UMingLocalizationManager::GetCurrencySymbol() const
{
    switch (CurrentRegion)
    {
    case EMingRegion::MainlandChina:   return TEXT("\u00a5");  // 人民幣
    case EMingRegion::Taiwan:          return TEXT("NT$");      // 新台幣
    case EMingRegion::HongKong:      return TEXT("HK$");      // 港幣
    case EMingRegion::Japan:           return TEXT("\u00a5");  // 日元
    case EMingRegion::SouthKorea:      return TEXT("\u20a9");  // 韓元
    case EMingRegion::UnitedStates:    return TEXT("$");       // 美元
    case EMingRegion::UnitedKingdom:   return TEXT("\u00a3");  // 英鎊
    case EMingRegion::Germany:
    case EMingRegion::France:          return TEXT("\u20ac");  // 歐元
    case EMingRegion::Russia:          return TEXT("\u20bd");  // 盧布
    default:                           return TEXT("$");
    }
}

bool UMingLocalizationManager::IsRightToLeft() const
{
    // 阿拉伯語和希伯來語等從右到左書寫
    return (CurrentLanguage == EMingLanguage::Arabic);
}

int32 UMingLocalizationManager::GetTranslationProgress(EMingLanguage Language) const
{
    if (!LocalizationData.Contains(Language))
    {
        return 0;
    }

    // 計算翻譯完成度
    const TMap<FString, FLocalizationEntry>& LanguageMap = LocalizationData[Language];
    int32 Total = LanguageMap.Num();
    int32 Completed = 0;

    for (const TPair<FString, FLocalizationEntry>& Pair : LanguageMap)
    {
        if (!Pair.Value.LocalizedText.IsEmpty())
        {
            Completed++;
        }
    }

    return Total > 0 ? (Completed * 100 / Total) : 0;
}

int32 UMingLocalizationManager::GetTotalEntryCount() const
{
    int32 Total = 0;
    for (const TPair<EMingLanguage, TMap<FString, FLocalizationEntry>>& Pair : LocalizationData)
    {
        Total += Pair.Value.Num();
    }
    return Total;
}

int32 UMingLocalizationManager::GetPendingReviewCount(EMingLanguage Language) const
{
    if (!LocalizationData.Contains(Language))
    {
        return 0;
    }

    int32 Count = 0;
    const TMap<FString, FLocalizationEntry>& LanguageMap = LocalizationData[Language];

    for (const TPair<FString, FLocalizationEntry>& Pair : LanguageMap)
    {
        if (Pair.Value.bNeedsReview)
        {
            Count++;
        }
    }

    return Count;
}

FString UMingLocalizationManager::GenerateLocalizationReport() const
{
    FString Report = TEXT("=== Localization Report ===\n\n");

    Report += FString::Printf(TEXT("Current Language: %s\n"), *GetCurrentLanguageName());
    Report += FString::Printf(TEXT("Current Region: %s\n"), *GetRegionName(CurrentRegion));
    Report += FString::Printf(TEXT("Total Entry Count: %d\n\n"), GetTotalEntryCount());

    Report += TEXT("Language Progress:\n");
    TArray<EMingLanguage> Languages = GetSupportedLanguages();
    for (EMingLanguage Language : Languages)
    {
        int32 Progress = GetTranslationProgress(Language);
        FLanguagePackInfo Info = GetLanguagePackInfo(Language);
        Report += FString::Printf(TEXT("  %s: %d%% (%d entries)\n"),
            *Info.LanguageName, Progress, Info.EntryCount);
    }

    return Report;
}

void UMingLocalizationManager::InitializeSubsystems()
{
    // 創建語言系統
    LanguageSystem = NewObject<UMingLanguageSystem>(this);
    if (LanguageSystem)
    {
        LanguageSystem->Initialize();
    }

    // 創建文化適配器
    CultureAdapter = NewObject<UMingCultureAdapter>(this);
    if (CultureAdapter)
    {
        CultureAdapter->Initialize();
    }

    // 創建地區內容管理器
    RegionalContentManager = NewObject<UMingRegionalContentManager>(this);
    if (RegionalContentManager)
    {
        RegionalContentManager->Initialize();
    }
}

bool UMingLocalizationManager::LoadLanguagePack(EMingLanguage Language)
{
    UE_LOG(LogTemp, Log, TEXT("Loading language pack for %s"), *GetLanguageName(Language));

    // 實際實現會從文件加載語言包
    // 這裡初始化空的地圖
    if (!LocalizationData.Contains(Language))
    {
        LocalizationData.Add(Language, TMap<FString, FLocalizationEntry>());
    }

    // 標記為已安裝
    if (!InstalledLanguages.Contains(Language))
    {
        InstalledLanguages.Add(Language);
    }

    OnLocalizationLoaded.Broadcast(true);
    return true;
}

void UMingLocalizationManager::SaveLanguagePreference()
{
    // 保存到配置系統
    UE_LOG(LogTemp, Log, TEXT("Saving language preference: %s"), *GetCurrentLanguageName());
}

void UMingLocalizationManager::LoadLanguagePreference()
{
    // 從配置系統加載
    UE_LOG(LogTemp, Log, TEXT("Loading language preference"));
}

FString UMingLocalizationManager::GetLanguageFileName(EMingLanguage Language) const
{
    return FString::Printf(TEXT("%s.json"), *GetLanguageName(Language));
}

FString UMingLocalizationManager::GetLanguageFilePath(EMingLanguage Language) const
{
    return FPaths::ProjectContentDir() / TEXT("Localization") / GetLanguageFileName(Language);
}

bool UMingLocalizationManager::ParseLanguageFile(const FString& FilePath, EMingLanguage Language)
{
    UE_LOG(LogTemp, Log, TEXT("Parsing language file: %s"), *FilePath);
    return true;
}

UMingLocalizationManager* UMingLocalizationManager::GetLocalizationManager(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    // 從遊戲實例獲取
    // 實際實現需要根據項目架構調整
    return nullptr;
}
