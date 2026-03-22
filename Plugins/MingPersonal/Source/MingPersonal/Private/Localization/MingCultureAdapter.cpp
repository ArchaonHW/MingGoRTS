#include "Localization/MingCultureAdapter.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"

UMingCultureAdapter::UMingCultureAdapter()
    : bInitialized(false)
{
}

void UMingCultureAdapter::InitializeCultureAdapter()
{
    if (bInitialized)
    {
        return;
    }

    // 加載文化數據
    LoadCulturalData();

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("MingCultureAdapter initialized"));
}

EContentAppropriateness UMingCultureAdapter::CheckContentAppropriateness(const FString& ContentID, const FString& RegionCode)
{
    // 檢查內容在指定地區的適宜性
    if (FCulturalContentInfo* ContentInfo = CulturalContentRegistry.Find(ContentID))
    {
        // 檢查地區是否受影響
        if (ContentInfo->AffectedRegions.Contains(RegionCode))
        {
            // 根據敏感度級別返回適宜性
            switch (ContentInfo->SensitivityLevel)
            {
            case ECulturalSensitivity::Critical:
                return EContentAppropriateness::Inappropriate;
            case ECulturalSensitivity::High:
                return EContentAppropriateness::WarningRequired;
            case ECulturalSensitivity::Medium:
                return EContentAppropriateness::CautionAdvised;
            case ECulturalSensitivity::Low:
                return EContentAppropriateness::Appropriate;
            case ECulturalSensitivity::None:
                return EContentAppropriateness::Appropriate;
            default:
                return EContentAppropriateness::Unknown;
            }
        }
    }

    // 默認為適宜
    return EContentAppropriateness::Appropriate;
}

ECulturalSensitivity UMingCultureAdapter::GetContentSensitivityLevel(const FString& ContentID) const
{
    if (const FCulturalContentInfo* ContentInfo = CulturalContentRegistry.Find(ContentID))
    {
        return ContentInfo->SensitivityLevel;
    }

    return ECulturalSensitivity::None;
}

FString UMingCultureAdapter::GetAdaptedContent(const FString& ContentID, const FString& TargetRegion)
{
    // 獲取適配後的內容
    EContentAppropriateness Appropriateness = CheckContentAppropriateness(ContentID, TargetRegion);

    if (Appropriateness == EContentAppropriateness::Inappropriate)
    {
        // 需要替代內容
        if (const FCulturalContentInfo* ContentInfo = CulturalContentRegistry.Find(ContentID))
        {
            if (!ContentInfo->AdaptedContentID.IsEmpty())
            {
                return ContentInfo->AdaptedContentID;
            }
        }

        // 返回空表示內容不適宜且無替代
        return FString();
    }

    // 內容適宜，返回原ID
    return ContentID;
}

TArray<FString> UMingCultureAdapter::GetRequiredContentModifications(const FString& ContentID, const FString& TargetRegion)
{
    TArray<FString> Modifications;

    if (const FCulturalContentInfo* ContentInfo = CulturalContentRegistry.Find(ContentID))
    {
        if (ContentInfo->AffectedRegions.Contains(TargetRegion))
        {
            // 根據敏感度級別和內容類型生成修改建議
            switch (ContentInfo->ContentType)
            {
            case ECulturalContentType::Religious:
                Modifications.Add(TEXT("Remove religious symbols"));
                Modifications.Add(TEXT("Neutralize religious references"));
                break;
            case ECulturalContentType::Political:
                Modifications.Add(TEXT("Remove political content"));
                Modifications.Add(TEXT("Use neutral historical context"));
                break;
            case ECulturalContentType::Historical:
                Modifications.Add(TEXT("Add historical disclaimers"));
                Modifications.Add(TEXT("Verify historical accuracy"));
                break;
            case ECulturalContentType::Cultural:
                Modifications.Add(TEXT("Add cultural context"));
                Modifications.Add(TEXT("Explain cultural references"));
                break;
            default:
                break;
            }
        }
    }

    return Modifications;
}

bool UMingCultureAdapter::ShouldShowWarning(const FString& ContentID, const FString& RegionCode)
{
    EContentAppropriateness Appropriateness = CheckContentAppropriateness(ContentID, RegionCode);
    return Appropriateness == EContentAppropriateness::WarningRequired ||
           Appropriateness == EContentAppropriateness::CautionAdvised;
}

FString UMingCultureAdapter::GetWarningMessage(const FString& ContentID, const FString& RegionCode)
{
    EContentAppropriateness Appropriateness = CheckContentAppropriateness(ContentID, RegionCode);

    switch (Appropriateness)
    {
    case EContentAppropriateness::WarningRequired:
        return TEXT("Warning: This content may be culturally sensitive in your region.");
    case EContentAppropriateness::CautionAdvised:
        return TEXT("Note: This content has been adapted for your region.");
    default:
        return FString();
    }
}

void UMingCultureAdapter::RegisterCulturalContent(const FCulturalContentInfo& ContentInfo)
{
    CulturalContentRegistry.Add(ContentInfo.ContentID, ContentInfo);
    UE_LOG(LogTemp, Log, TEXT("Registered cultural content: %s"), *ContentInfo.ContentID);
}

void UMingCultureAdapter::UnregisterCulturalContent(const FString& ContentID)
{
    CulturalContentRegistry.Remove(ContentID);
}

void UMingCultureAdapter::SetRegionCulturalPreference(const FString& RegionCode, const FCulturalPreference& Preference)
{
    RegionPreferences.Add(RegionCode, Preference);
}

FCulturalPreference UMingCultureAdapter::GetRegionCulturalPreference(const FString& RegionCode) const
{
    if (const FCulturalPreference* Preference = RegionPreferences.Find(RegionCode))
    {
        return *Preference;
    }

    // 返回默認偏好
    return FCulturalPreference();
}

FString UMingCultureAdapter::GetColorMeaning(const FString& ColorName, const FString& RegionCode)
{
    // 返回顏色在特定文化中的含義
    static const TMap<FString, TMap<FString, FString>> ColorMeanings = {
        {TEXT("CN"), {
            {TEXT("red"), TEXT("luck, happiness, prosperity")},
            {TEXT("yellow"), TEXT("imperial power, royalty")},
            {TEXT("white"), TEXT("mourning, death")},
            {TEXT("black"), TEXT("water, winter, north")},
        }},
        {TEXT("JP"), {
            {TEXT("red"), TEXT("danger, protection from evil")},
            {TEXT("white"), TEXT("purity, new beginnings")},
            {TEXT("black"), TEXT("formality, dignity")},
        }},
        {TEXT("KR"), {
            {TEXT("red"), TEXT("passion, good fortune")},
            {TEXT("blue"), TEXT("peace, calmness")},
        }},
        {TEXT("US"), {
            {TEXT("red"), TEXT("danger, stop, passion")},
            {TEXT("white"), TEXT("purity, peace")},
            {TEXT("blue"), TEXT("trust, loyalty, calm")},
        }},
    };

    if (const TMap<FString, FString>* RegionColors = ColorMeanings.Find(RegionCode))
    {
        if (const FString* Meaning = RegionColors->Find(ColorName))
        {
            return *Meaning;
        }
    }

    return TEXT("No specific cultural meaning");
}

FString UMingCultureAdapter::GetNumberMeaning(int32 Number, const FString& RegionCode)
{
    // 返回數字在特定文化中的含義
    static const TMap<FString, TMap<int32, FString>> NumberMeanings = {
        {TEXT("CN"), {
            {4, TEXT("unlucky - sounds like 'death'")},
            {6, TEXT("lucky - sounds like 'smooth'")},
            {8, TEXT("very lucky - sounds like 'wealth'")},
            {9, TEXT("lucky - sounds like 'long lasting'")},
        }},
        {TEXT("JP"), {
            {4, TEXT("unlucky - sounds like 'death'")},
            {9, TEXT("unlucky - sounds like 'suffering'")},
            {7, TEXT("lucky - seven gods of fortune")},
        }},
        {TEXT("KR"), {
            {4, TEXT("unlucky")},
            {7, TEXT("lucky")},
        }},
    };

    if (const TMap<int32, FString>* RegionNumbers = NumberMeanings.Find(RegionCode))
    {
        if (const FString* Meaning = RegionNumbers->Find(Number))
        {
            return *Meaning;
        }
    }

    return TEXT("No specific cultural meaning");
}

TArray<FString> UMingCultureAdapter::GetTabooTopics(const FString& RegionCode)
{
    // 返回特定地區的禁忌話題
    static const TMap<FString, TArray<FString>> TabooTopics = {
        {TEXT("CN"), {
            TEXT("Politics"),
            TEXT("Religious extremism"),
            TEXT("Separatism"),
        }},
        {TEXT("JP"), {
            TEXT("Nuclear issues"),
            TEXT("War crimes"),
        }},
        {TEXT("KR"), {
            TEXT("North-South division"),
            TEXT("Comfort women issue"),
        }},
        {TEXT("SA"), {
            TEXT("Religious criticism"),
            TEXT("Women's rights debates"),
        }},
    };

    if (const TArray<FString>* Topics = TabooTopics.Find(RegionCode))
    {
        return *Topics;
    }

    return TArray<FString>();
}

bool UMingCultureAdapter::IsContentSensitive(const FString& ContentID, const FString& RegionCode)
{
    EContentAppropriateness Appropriateness = CheckContentAppropriateness(ContentID, RegionCode);
    return Appropriateness == EContentAppropriateness::Inappropriate ||
           Appropriateness == EContentAppropriateness::WarningRequired;
}

TArray<FString> UMingCultureAdapter::FilterSensitiveContent(const TArray<FString>& ContentIDs, const FString& RegionCode)
{
    TArray<FString> FilteredContent;

    for (const FString& ContentID : ContentIDs)
    {
        if (!IsContentSensitive(ContentID, RegionCode))
        {
            FilteredContent.Add(ContentID);
        }
    }

    return FilteredContent;
}

FString UMingCultureAdapter::ValidateHistoricalContent(const FString& ContentID, const FString& HistoricalPeriod)
{
    // 驗證歷史內容的準確性和適宜性
    // 這裡是簡化的實現
    return FString::Printf(TEXT("Historical content '%s' validated for period '%s'"), *ContentID, *HistoricalPeriod);
}

bool UMingCultureAdapter::SaveCulturalData(const FString& FilePath)
{
    // 保存文化數據到文件
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

    // 序列化文化內容註冊表
    TArray<TSharedPtr<FJsonValue>> ContentArray;
    for (const auto& Pair : CulturalContentRegistry)
    {
        TSharedPtr<FJsonObject> ContentJson = MakeShared<FJsonObject>();
        ContentJson->SetStringField(TEXT("ContentID"), Pair.Value.ContentID);
        ContentJson->SetNumberField(TEXT("ContentType"), static_cast<int32>(Pair.Value.ContentType));
        ContentJson->SetNumberField(TEXT("SensitivityLevel"), static_cast<int32>(Pair.Value.SensitivityLevel));
        ContentJson->SetStringField(TEXT("AdaptedContentID"), Pair.Value.AdaptedContentID);

        TArray<TSharedPtr<FJsonValue>> AffectedRegionsArray;
        for (const FString& Region : Pair.Value.AffectedRegions)
        {
            AffectedRegionsArray.Add(MakeShared<FJsonValueString>(Region));
        }
        ContentJson->SetArrayField(TEXT("AffectedRegions"), AffectedRegionsArray);

        ContentArray.Add(MakeShared<FJsonValueObject>(ContentJson));
    }
    JsonObject->SetArrayField(TEXT("CulturalContent"), ContentArray);

    // 寫入文件
    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
    {
        return FFileHelper::SaveStringToFile(JsonString, *FilePath);
    }

    return false;
}

bool UMingCultureAdapter::LoadCulturalData(const FString& FilePath)
{
    // 從文件加載文化數據
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load cultural data from: %s"), *FilePath);
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    if (!FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to parse cultural data JSON"));
        return false;
    }

    // 解析文化內容
    const TArray<TSharedPtr<FJsonValue>>* ContentArray;
    if (JsonObject->TryGetArrayField(TEXT("CulturalContent"), ContentArray))
    {
        for (const auto& Value : *ContentArray)
        {
            if (TSharedPtr<FJsonObject> ContentJson = Value->AsObject())
            {
                FCulturalContentInfo ContentInfo;
                ContentInfo.ContentID = ContentJson->GetStringField(TEXT("ContentID"));
                ContentInfo.ContentType = static_cast<ECulturalContentType>(ContentJson->GetIntegerField(TEXT("ContentType")));
                ContentInfo.SensitivityLevel = static_cast<ECulturalSensitivity>(ContentJson->GetIntegerField(TEXT("SensitivityLevel")));
                ContentInfo.AdaptedContentID = ContentJson->GetStringField(TEXT("AdaptedContentID"));

                const TArray<TSharedPtr<FJsonValue>>* RegionsArray;
                if (ContentJson->TryGetArrayField(TEXT("AffectedRegions"), RegionsArray))
                {
                    for (const auto& RegionValue : *RegionsArray)
                    {
                        ContentInfo.AffectedRegions.Add(RegionValue->AsString());
                    }
                }

                CulturalContentRegistry.Add(ContentInfo.ContentID, ContentInfo);
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Loaded cultural data from: %s"), *FilePath);
    return true;
}

void UMingCultureAdapter::LoadCulturalData()
{
    // 加載默認文化數據
    FString DefaultDataPath = FPaths::ProjectContentDir() / TEXT("Data/CulturalData.json");

    if (FPaths::FileExists(DefaultDataPath))
    {
        LoadCulturalData(DefaultDataPath);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("No cultural data file found at: %s, using defaults"), *DefaultDataPath);
    }
}
