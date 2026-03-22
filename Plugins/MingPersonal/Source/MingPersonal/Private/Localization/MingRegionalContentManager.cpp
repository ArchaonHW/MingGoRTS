#include "Localization/MingRegionalContentManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"

UMingRegionalContentManager::UMingRegionalContentManager()
    : bInitialized(false)
{
}

void UMingRegionalContentManager::InitializeRegionalContentManager()
{
    if (bInitialized)
    {
        return;
    }

    // 加載地區內容數據
    LoadRegionalContentData();

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("MingRegionalContentManager initialized"));
}

bool UMingRegionalContentManager::IsContentAvailable(const FString& ContentID, const FString& RegionCode)
{
    if (const FRegionalContentInfo* ContentInfo = RegionalContentRegistry.Find(ContentID))
    {
        // 檢查內容是否被禁用
        if (ContentInfo->Status == ERegionalContentStatus::Disabled)
        {
            return false;
        }

        // 檢查地區限制
        if (ContentInfo->RestrictedRegions.Contains(RegionCode))
        {
            return false;
        }

        // 如果有指定可用地區，檢查是否包含
        if (ContentInfo->AvailableRegions.Num() > 0)
        {
            return ContentInfo->AvailableRegions.Contains(RegionCode);
        }

        // 默認為可用
        return true;
    }

    // 未註冊的內容默認為不可用
    return false;
}

ERegionalContentStatus UMingRegionalContentManager::GetContentStatus(const FString& ContentID, const FString& RegionCode)
{
    if (const FRegionalContentInfo* ContentInfo = RegionalContentRegistry.Find(ContentID))
    {
        // 檢查地區特定的狀態
        if (ContentInfo->RegionalVariants.Contains(RegionCode))
        {
            const FRegionalContentVariant& Variant = ContentInfo->RegionalVariants[RegionCode];
            return Variant.Status;
        }

        // 返回全局狀態
        return ContentInfo->Status;
    }

    return ERegionalContentStatus::Disabled;
}

FString UMingRegionalContentManager::GetAlternativeContentID(const FString& ContentID, const FString& RegionCode)
{
    if (const FRegionalContentInfo* ContentInfo = RegionalContentRegistry.Find(ContentID))
    {
        // 檢查是否有替代內容
        if (!ContentInfo->AlternativeContentID.IsEmpty())
        {
            // 檢查替代內容是否適用於該地區
            if (IsContentAvailable(ContentInfo->AlternativeContentID, RegionCode))
            {
                return ContentInfo->AlternativeContentID;
            }
        }

        // 檢查地區變體
        if (ContentInfo->RegionalVariants.Contains(RegionCode))
        {
            const FRegionalContentVariant& Variant = ContentInfo->RegionalVariants[RegionCode];
            if (!Variant.AlternativeContentID.IsEmpty())
            {
                return Variant.AlternativeContentID;
            }
        }
    }

    return FString();
}

void UMingRegionalContentManager::RegisterRegionalContent(const FRegionalContentInfo& ContentInfo)
{
    RegionalContentRegistry.Add(ContentInfo.ContentID, ContentInfo);
    UE_LOG(LogTemp, Log, TEXT("Registered regional content: %s"), *ContentInfo.ContentID);
}

void UMingRegionalContentManager::UnregisterRegionalContent(const FString& ContentID)
{
    RegionalContentRegistry.Remove(ContentID);
}

void UMingRegionalContentManager::UpdateContentAvailability(const FString& ContentID, const FString& RegionCode, bool bAvailable)
{
    if (FRegionalContentInfo* ContentInfo = RegionalContentRegistry.Find(ContentID))
    {
        if (bAvailable)
        {
            ContentInfo->AvailableRegions.Add(RegionCode);
            ContentInfo->RestrictedRegions.Remove(RegionCode);
        }
        else
        {
            ContentInfo->AvailableRegions.Remove(RegionCode);
            ContentInfo->RestrictedRegions.Add(RegionCode);
        }
    }
}

void UMingRegionalContentManager::CreateContentBundle(const FString& BundleName, const TArray<FString>& ContentIDs, const FString& TargetRegion)
{
    FContentBundle Bundle;
    Bundle.BundleName = BundleName;
    Bundle.ContentIDs = ContentIDs;
    Bundle.TargetRegion = TargetRegion;
    Bundle.CreationTime = FDateTime::Now();

    ContentBundles.Add(BundleName, Bundle);

    UE_LOG(LogTemp, Log, TEXT("Created content bundle: %s for region: %s"), *BundleName, *TargetRegion);
}

void UMingRegionalContentManager::DeleteContentBundle(const FString& BundleName)
{
    ContentBundles.Remove(BundleName);
}

TArray<FString> UMingRegionalContentManager::GetBundleContent(const FString& BundleName, const FString& RegionCode)
{
    TArray<FString> AvailableContent;

    if (const FContentBundle* Bundle = ContentBundles.Find(BundleName))
    {
        for (const FString& ContentID : Bundle->ContentIDs)
        {
            if (IsContentAvailable(ContentID, RegionCode))
            {
                AvailableContent.Add(ContentID);
            }
        }
    }

    return AvailableContent;
}

FRegionalAvailabilityReport UMingRegionalContentManager::GenerateAvailabilityReport(const FString& RegionCode)
{
    FRegionalAvailabilityReport Report;
    Report.RegionCode = RegionCode;
    Report.ReportDate = FDateTime::Now();

    for (const auto& Pair : RegionalContentRegistry)
    {
        if (IsContentAvailable(Pair.Key, RegionCode))
        {
            Report.AvailableContent.Add(Pair.Key);
        }
        else
        {
            Report.UnavailableContent.Add(Pair.Key);
        }
    }

    // 計算可用性百分比
    int32 TotalContent = RegionalContentRegistry.Num();
    if (TotalContent > 0)
    {
        Report.AvailabilityPercentage = (static_cast<float>(Report.AvailableContent.Num()) / TotalContent) * 100.0f;
    }

    return Report;
}

TMap<FString, float> UMingRegionalContentManager::CompareRegionalContent(const FString& RegionCode1, const FString& RegionCode2)
{
    TMap<FString, float> Comparison;

    TArray<FString> AvailableInRegion1;
    TArray<FString> AvailableInRegion2;

    // 獲取各地區可用的內容
    for (const auto& Pair : RegionalContentRegistry)
    {
        if (IsContentAvailable(Pair.Key, RegionCode1))
        {
            AvailableInRegion1.Add(Pair.Key);
        }
        if (IsContentAvailable(Pair.Key, RegionCode2))
        {
            AvailableInRegion2.Add(Pair.Key);
        }
    }

    // 計算共同內容
    TArray<FString> CommonContent;
    for (const FString& ContentID : AvailableInRegion1)
    {
        if (AvailableInRegion2.Contains(ContentID))
        {
            CommonContent.Add(ContentID);
        }
    }

    // 計算相似度
    int32 TotalUnique = AvailableInRegion1.Num() + AvailableInRegion2.Num() - CommonContent.Num();
    float Similarity = TotalUnique > 0 ? (static_cast<float>(CommonContent.Num()) / TotalUnique) * 100.0f : 100.0f;

    Comparison.Add(TEXT("Similarity"), Similarity);
    Comparison.Add(TEXT("Region1Count"), AvailableInRegion1.Num());
    Comparison.Add(TEXT("Region2Count"), AvailableInRegion2.Num());
    Comparison.Add(TEXT("CommonCount"), CommonContent.Num());

    return Comparison;
}

void UMingRegionalContentManager::UpdateContentForRegion(const FString& ContentID, const FString& RegionCode, const FRegionalContentVariant& Variant)
{
    if (FRegionalContentInfo* ContentInfo = RegionalContentRegistry.Find(ContentID))
    {
        ContentInfo->RegionalVariants.Add(RegionCode, Variant);
    }
}

TArray<FString> UMingRegionalContentManager::GetAvailableCharacters(const FString& RegionCode)
{
    TArray<FString> AvailableCharacters;

    for (const auto& Pair : RegionalContentRegistry)
    {
        if (Pair.Value.ContentType == ERegionalContentType::Character &&
            IsContentAvailable(Pair.Key, RegionCode))
        {
            AvailableCharacters.Add(Pair.Key);
        }
    }

    return AvailableCharacters;
}

TArray<FString> UMingRegionalContentManager::GetAvailableQuests(const FString& RegionCode)
{
    TArray<FString> AvailableQuests;

    for (const auto& Pair : RegionalContentRegistry)
    {
        if (Pair.Value.ContentType == ERegionalContentType::Quest &&
            IsContentAvailable(Pair.Key, RegionCode))
        {
            AvailableQuests.Add(Pair.Key);
        }
    }

    return AvailableQuests;
}

TArray<FString> UMingRegionalContentManager::GetAvailableItems(const FString& RegionCode)
{
    TArray<FString> AvailableItems;

    for (const auto& Pair : RegionalContentRegistry)
    {
        if (Pair.Value.ContentType == ERegionalContentType::Item &&
            IsContentAvailable(Pair.Key, RegionCode))
        {
            AvailableItems.Add(Pair.Key);
        }
    }

    return AvailableItems;
}

TArray<FString> UMingRegionalContentManager::GetAvailableBuildings(const FString& RegionCode)
{
    TArray<FString> AvailableBuildings;

    for (const auto& Pair : RegionalContentRegistry)
    {
        if (Pair.Value.ContentType == ERegionalContentType::Building &&
            IsContentAvailable(Pair.Key, RegionCode))
        {
            AvailableBuildings.Add(Pair.Key);
        }
    }

    return AvailableBuildings;
}

int32 UMingRegionalContentManager::GetTotalContentCount() const
{
    return RegionalContentRegistry.Num();
}

int32 UMingRegionalContentManager::GetAvailableContentCount(const FString& RegionCode)
{
    int32 Count = 0;
    for (const auto& Pair : RegionalContentRegistry)
    {
        if (IsContentAvailable(Pair.Key, RegionCode))
        {
            Count++;
        }
    }
    return Count;
}

bool UMingRegionalContentManager::IsContentBundleValid(const FString& BundleName, const FString& RegionCode)
{
    if (const FContentBundle* Bundle = ContentBundles.Find(BundleName))
    {
        // 檢查捆綁包是否適用於該地區
        if (Bundle->TargetRegion != RegionCode && !Bundle->TargetRegion.IsEmpty())
        {
            return false;
        }

        // 檢查捆綁包中的所有內容是否都可用
        for (const FString& ContentID : Bundle->ContentIDs)
        {
            if (!IsContentAvailable(ContentID, RegionCode))
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

FString UMingRegionalContentManager::GetLastError() const
{
    return LastErrorMessage;
}

bool UMingRegionalContentManager::SaveRegionalData(const FString& FilePath)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

    // 序列化地區內容註冊表
    TArray<TSharedPtr<FJsonValue>> ContentArray;
    for (const auto& Pair : RegionalContentRegistry)
    {
        TSharedPtr<FJsonObject> ContentJson = MakeShared<FJsonObject>();
        ContentJson->SetStringField(TEXT("ContentID"), Pair.Value.ContentID);
        ContentJson->SetNumberField(TEXT("ContentType"), static_cast<int32>(Pair.Value.ContentType));
        ContentJson->SetNumberField(TEXT("Status"), static_cast<int32>(Pair.Value.Status));
        ContentJson->SetStringField(TEXT("AlternativeContentID"), Pair.Value.AlternativeContentID);

        // 可用地區
        TArray<TSharedPtr<FJsonValue>> AvailableRegionsArray;
        for (const FString& Region : Pair.Value.AvailableRegions)
        {
            AvailableRegionsArray.Add(MakeShared<FJsonValueString>(Region));
        }
        ContentJson->SetArrayField(TEXT("AvailableRegions"), AvailableRegionsArray);

        // 限制地區
        TArray<TSharedPtr<FJsonValue>> RestrictedRegionsArray;
        for (const FString& Region : Pair.Value.RestrictedRegions)
        {
            RestrictedRegionsArray.Add(MakeShared<FJsonValueString>(Region));
        }
        ContentJson->SetArrayField(TEXT("RestrictedRegions"), RestrictedRegionsArray);

        ContentArray.Add(MakeShared<FJsonValueObject>(ContentJson));
    }
    JsonObject->SetArrayField(TEXT("RegionalContent"), ContentArray);

    // 序列化捆綁包
    TArray<TSharedPtr<FJsonValue>> BundleArray;
    for (const auto& Pair : ContentBundles)
    {
        TSharedPtr<FJsonObject> BundleJson = MakeShared<FJsonObject>();
        BundleJson->SetStringField(TEXT("BundleName"), Pair.Value.BundleName);
        BundleJson->SetStringField(TEXT("TargetRegion"), Pair.Value.TargetRegion);

        TArray<TSharedPtr<FJsonValue>> ContentIDsArray;
        for (const FString& ContentID : Pair.Value.ContentIDs)
        {
            ContentIDsArray.Add(MakeShared<FJsonValueString>(ContentID));
        }
        BundleJson->SetArrayField(TEXT("ContentIDs"), ContentIDsArray);

        BundleArray.Add(MakeShared<FJsonValueObject>(BundleJson));
    }
    JsonObject->SetArrayField(TEXT("ContentBundles"), BundleArray);

    // 寫入文件
    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
    {
        return FFileHelper::SaveStringToFile(JsonString, *FilePath);
    }

    return false;
}

bool UMingRegionalContentManager::LoadRegionalData(const FString& FilePath)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load regional data from: %s"), *FilePath);
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    if (!FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to parse regional data JSON"));
        return false;
    }

    // 解析地區內容
    const TArray<TSharedPtr<FJsonValue>>* ContentArray;
    if (JsonObject->TryGetArrayField(TEXT("RegionalContent"), ContentArray))
    {
        for (const auto& Value : *ContentArray)
        {
            if (TSharedPtr<FJsonObject> ContentJson = Value->AsObject())
            {
                FRegionalContentInfo ContentInfo;
                ContentInfo.ContentID = ContentJson->GetStringField(TEXT("ContentID"));
                ContentInfo.ContentType = static_cast<ERegionalContentType>(ContentJson->GetIntegerField(TEXT("ContentType")));
                ContentInfo.Status = static_cast<ERegionalContentStatus>(ContentJson->GetIntegerField(TEXT("Status")));
                ContentInfo.AlternativeContentID = ContentJson->GetStringField(TEXT("AlternativeContentID"));

                const TArray<TSharedPtr<FJsonValue>>* AvailableRegionsArray;
                if (ContentJson->TryGetArrayField(TEXT("AvailableRegions"), AvailableRegionsArray))
                {
                    for (const auto& RegionValue : *AvailableRegionsArray)
                    {
                        ContentInfo.AvailableRegions.Add(RegionValue->AsString());
                    }
                }

                const TArray<TSharedPtr<FJsonValue>>* RestrictedRegionsArray;
                if (ContentJson->TryGetArrayField(TEXT("RestrictedRegions"), RestrictedRegionsArray))
                {
                    for (const auto& RegionValue : *RestrictedRegionsArray)
                    {
                        ContentInfo.RestrictedRegions.Add(RegionValue->AsString());
                    }
                }

                RegionalContentRegistry.Add(ContentInfo.ContentID, ContentInfo);
            }
        }
    }

    // 解析捆綁包
    const TArray<TSharedPtr<FJsonValue>>* BundleArray;
    if (JsonObject->TryGetArrayField(TEXT("ContentBundles"), BundleArray))
    {
        for (const auto& Value : *BundleArray)
        {
            if (TSharedPtr<FJsonObject> BundleJson = Value->AsObject())
            {
                FContentBundle Bundle;
                Bundle.BundleName = BundleJson->GetStringField(TEXT("BundleName"));
                Bundle.TargetRegion = BundleJson->GetStringField(TEXT("TargetRegion"));

                const TArray<TSharedPtr<FJsonValue>>* ContentIDsArray;
                if (BundleJson->TryGetArrayField(TEXT("ContentIDs"), ContentIDsArray))
                {
                    for (const auto& ContentIDValue : *ContentIDsArray)
                    {
                        Bundle.ContentIDs.Add(ContentIDValue->AsString());
                    }
                }

                ContentBundles.Add(Bundle.BundleName, Bundle);
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Loaded regional data from: %s"), *FilePath);
    return true;
}

void UMingRegionalContentManager::LoadRegionalContentData()
{
    // 加載默認地區內容數據
    FString DefaultDataPath = FPaths::ProjectContentDir() / TEXT("Data/RegionalContentData.json");

    if (FPaths::FileExists(DefaultDataPath))
    {
        LoadRegionalData(DefaultDataPath);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("No regional content data file found at: %s, using defaults"), *DefaultDataPath);
    }
}
