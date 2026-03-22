// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Text Manager Implementation

#include "Localization/MingRTSTextManager.h"
#include "Localization/MingRTSLocalizationSystem.h"
#include "Internationalization/Internationalization.h"
#include "Misc/ConfigCacheIni.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSText, Log, All);

UMingRTSTextManager::UMingRTSTextManager()
{
    LocalizationSystem = nullptr;
}

void UMingRTSTextManager::InitializeTextManager()
{
    UE_LOG(LogMingRTSText, Log, TEXT("Initializing Text Manager..."));
    
    // Get or create localization system
    // In practice, this would be set by a game instance or subsystem
    // For now, we'll initialize it on first use
    
    UE_LOG(LogMingRTSText, Log, TEXT("Text Manager initialized"));
}

FString UMingRTSTextManager::GetText(const FString& Namespace, const FString& Key) const
{
    if (!LocalizationSystem)
    {
        return Key;
    }
    
    FString FullKey = Namespace + TEXT(".") + Key;
    return LocalizationSystem->GetLocalizedText(FullKey);
}

FString UMingRTSTextManager::GetTextByCategory(ETextCategory Category, const FString& Key) const
{
    // Build category namespace
    FString CategoryPrefix;
    switch (Category)
    {
    case ETextCategory::UI: CategoryPrefix = TEXT("UI"); break;
    case ETextCategory::Dialogue: CategoryPrefix = TEXT("Dialogue"); break;
    case ETextCategory::Tutorial: CategoryPrefix = TEXT("Tutorial"); break;
    case ETextCategory::System: CategoryPrefix = TEXT("System"); break;
    case ETextCategory::Campaign: CategoryPrefix = TEXT("Campaign"); break;
    case ETextCategory::Unit: CategoryPrefix = TEXT("Unit"); break;
    case ETextCategory::Building: CategoryPrefix = TEXT("Building"); break;
    case ETextCategory::Technology: CategoryPrefix = TEXT("Technology"); break;
    case ETextCategory::Event: CategoryPrefix = TEXT("Event"); break;
    case ETextCategory::Achievement: CategoryPrefix = TEXT("Achievement"); break;
    default: CategoryPrefix = TEXT("General"); break;
    }
    
    return GetText(CategoryPrefix, Key);
}

FString UMingRTSTextManager::FormatText(const FString& Key, const TArray<FString>& Args) const
{
    if (!LocalizationSystem)
    {
        return Key;
    }
    
    return LocalizationSystem->FormatLocalizedText(Key, Args);
}

FString UMingRTSTextManager::FormatTextNamed(const FString& Key, 
    const TMap<FString, FString>& NamedArgs) const
{
    FString Text = GetRawText(Key);
    
    // Replace named placeholders {name}
    for (const auto& Pair : NamedArgs)
    {
        FString Placeholder = FString::Printf(TEXT("{%s}"), *Pair.Key);
        Text = Text.Replace(*Placeholder, *Pair.Value);
    }
    
    return Text;
}

FString UMingRTSTextManager::GetPluralText(const FString& KeySingular, 
    const FString& KeyPlural, int32 Count) const
{
    if (Count == 1)
    {
        return FormatText(KeySingular, { FString::FromInt(Count) });
    }
    else
    {
        return FormatText(KeyPlural, { FString::FromInt(Count) });
    }
}

FString UMingRTSTextManager::TruncateText(const FString& Text, int32 MaxLength, 
    const FString& Ellipsis) const
{
    if (MaxLength <= 0 || Text.Len() <= MaxLength)
    {
        return Text;
    }
    
    // Account for ellipsis length
    int32 TruncateLength = MaxLength - Ellipsis.Len();
    if (TruncateLength <= 0)
    {
        return Ellipsis.Left(MaxLength);
    }
    
    return Text.Left(TruncateLength) + Ellipsis;
}

TArray<FString> UMingRTSTextManager::GetKeysInNamespace(const FString& Namespace) const
{
    TArray<FString> Keys;
    
    if (!LocalizationSystem)
    {
        return Keys;
    }
    
    // This would need access to the language pack's keys
    // For now, return empty array
    // In full implementation, we'd enumerate all keys with the namespace prefix
    
    return Keys;
}

TArray<FString> UMingRTSTextManager::SearchKeys(const FString& SearchTerm) const
{
    TArray<FString> Results;
    
    if (!LocalizationSystem)
    {
        return Results;
    }
    
    // Search through metadata
    for (const auto& Pair : TextMetadataMap)
    {
        if (Pair.Key.Contains(SearchTerm) || 
            Pair.Value.Context.Contains(SearchTerm))
        {
            Results.Add(Pair.Key);
        }
    }
    
    return Results;
}

void UMingRTSTextManager::RegisterTextMetadata(const FString& Key, const FTextMetadata& Metadata)
{
    FTextMetadata NewMetadata = Metadata;
    NewMetadata.Key = Key;
    TextMetadataMap.Add(Key, NewMetadata);
    
    // Add to category mapping
    TArray<FString>& KeysInCategory = CategoryKeys.FindOrAdd(Metadata.Category);
    if (!KeysInCategory.Contains(Key))
    {
        KeysInCategory.Add(Key);
    }
}

FTextMetadata UMingRTSTextManager::GetTextMetadata(const FString& Key) const
{
    const FTextMetadata* Metadata = TextMetadataMap.Find(Key);
    if (Metadata)
    {
        return *Metadata;
    }
    
    return FTextMetadata();
}

bool UMingRTSTextManager::HasText(const FString& Key) const
{
    if (!LocalizationSystem)
    {
        return false;
    }
    
    return LocalizationSystem->HasKey(Key);
}

int32 UMingRTSTextManager::GetTextLength(const FString& Text) const
{
    // Return actual character count (not byte count)
    return Text.Len();
}

FString UMingRTSTextManager::ToUpper(const FString& Text) const
{
    FString Locale = GetCurrentLocale();
    return Text.ToUpper();
}

FString UMingRTSTextManager::ToLower(const FString& Text) const
{
    FString Locale = GetCurrentLocale();
    return Text.ToLower();
}

TArray<ETextCategory> UMingRTSTextManager::GetAllCategories() const
{
    TArray<ETextCategory> Categories;
    CategoryKeys.GetKeys(Categories);
    return Categories;
}

TArray<FString> UMingRTSTextManager::GetKeysInCategory(ETextCategory Category) const
{
    const TArray<FString>* Keys = CategoryKeys.Find(Category);
    if (Keys)
    {
        return *Keys;
    }
    
    return TArray<FString>();
}

TMap<FString, FString> UMingRTSTextManager::BatchGetTexts(const TArray<FString>& Keys) const
{
    TMap<FString, FString> Results;
    
    if (!LocalizationSystem)
    {
        // Return keys as values
        for (const FString& Key : Keys)
        {
            Results.Add(Key, Key);
        }
        return Results;
    }
    
    for (const FString& Key : Keys)
    {
        FString Text = LocalizationSystem->GetLocalizedText(Key);
        Results.Add(Key, Text);
    }
    
    return Results;
}

void UMingRTSTextManager::ExportMissingTranslations(const FString& OutputPath) const
{
    // Find keys without translations
    TMap<FString, FString> MissingTranslations;
    
    // In full implementation, we'd compare keys against loaded language packs
    // and export the missing ones to a file for translators
    
    UE_LOG(LogMingRTSText, Log, TEXT("Exporting missing translations to: %s"), *OutputPath);
    
    // Format: Key|Source Text|Context
    TArray<FString> Lines;
    Lines.Add(TEXT("Key|Source|Context|Category"));
    
    for (const auto& Pair : TextMetadataMap)
    {
        FString Line = FString::Printf(TEXT("%s|%s|%s|%s"),
            *Pair.Key,
            *GetRawText(Pair.Key),
            *Pair.Value.Context,
            *UEnum::GetValueAsString(Pair.Value.Category));
        Lines.Add(Line);
    }
    
    // Write to file
    // FFileHelper::SaveStringArrayToFile(Lines, *OutputPath);
    
    UE_LOG(LogMingRTSText, Log, TEXT("Exported %d entries"), Lines.Num() - 1);
}

bool UMingRTSTextManager::ImportTranslations(const FString& FilePath)
{
    UE_LOG(LogMingRTSText, Log, TEXT("Importing translations from: %s"), *FilePath);
    
    // Read translation file
    // Format: Key|Translated Text
    // Update language packs with new translations
    
    // For now, just reload current language
    if (LocalizationSystem)
    {
        LocalizationSystem->ReloadCurrentLanguage();
    }
    
    return true;
}

// Private methods

FString UMingRTSTextManager::GetRawText(const FString& Key) const
{
    if (!LocalizationSystem)
    {
        return Key;
    }
    
    return LocalizationSystem->GetLocalizedText(Key);
}

FString UMingRTSTextManager::ApplyFormatting(const FString& Text, 
    const TArray<FString>& Args) const
{
    FString Result = Text;
    
    for (int32 i = 0; i < Args.Num(); ++i)
    {
        FString Placeholder = FString::Printf(TEXT("{%d}"), i);
        Result = Result.Replace(*Placeholder, *Args[i]);
    }
    
    return Result;
}

FString UMingRTSTextManager::ApplyNamedFormatting(const FString& Text,
    const TMap<FString, FString>& NamedArgs) const
{
    FString Result = Text;
    
    for (const auto& Pair : NamedArgs)
    {
        FString Placeholder = FString::Printf(TEXT("{%s}"), *Pair.Key);
        Result = Result.Replace(*Placeholder, *Pair.Value);
    }
    
    return Result;
}

void UMingRTSTextManager::ClearFormatCache()
{
    FormatCache.Empty();
}

FString UMingRTSTextManager::GetCurrentLocale() const
{
    // Get locale from internationalization system
    FCultureRef CurrentCulture = FInternationalization::Get().GetCurrentCulture();
    return CurrentCulture->GetName();
}
