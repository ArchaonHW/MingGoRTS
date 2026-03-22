// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Multi-Language Support System Implementation

#include "Localization/MingRTSLocalizationSystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSLocalization, Log, All);

UMingRTSLocalizationSystem::UMingRTSLocalizationSystem()
    : CurrentLanguage(ELanguageCode::zh_CN)
{
    LocalizationConfigPath = FPaths::ProjectContentDir() / TEXT("Localization");
}

void UMingRTSLocalizationSystem::InitializeLocalizationSystem()
{
    UE_LOG(LogMingRTSLocalization, Log, TEXT("Initializing Localization System..."));
    
    // Load default language
    LoadLanguagePack(CurrentLanguage);
    
    // Preload common texts for performance
    PreloadCommonTexts();
    
    UE_LOG(LogMingRTSLocalization, Log, TEXT("Localization System initialized with language: %s"),
        *GetLanguageInfo(CurrentLanguage).LanguageName);
}

void UMingRTSLocalizationSystem::ShutdownLocalizationSystem()
{
    UE_LOG(LogMingRTSLocalization, Log, TEXT("Shutting down Localization System..."));
    
    // Clear all loaded languages
    for (auto& Pair : LoadedLanguages)
    {
        if (Pair.Value.bIsLoaded)
        {
            UnloadLanguagePack(Pair.Key);
        }
    }
    
    LoadedLanguages.Empty();
    ClearCache();
    
    UE_LOG(LogMingRTSLocalization, Log, TEXT("Localization System shutdown complete"));
}

void UMingRTSLocalizationSystem::SetLanguage(ELanguageCode LanguageCode)
{
    if (LanguageCode == CurrentLanguage)
    {
        return;
    }
    
    // Load new language if not loaded
    if (!IsLanguageLoaded(LanguageCode))
    {
        if (!LoadLanguagePack(LanguageCode))
        {
            UE_LOG(LogMingRTSLocalization, Warning, TEXT("Failed to load language pack for %s"),
                *GetLanguageInfo(LanguageCode).LanguageName);
            return;
        }
    }
    
    // Clear cache for old language
    ClearCache();
    
    // Update current language
    ELanguageCode PreviousLanguage = CurrentLanguage;
    CurrentLanguage = LanguageCode;
    
    // Preload common texts
    PreloadCommonTexts();
    
    // Broadcast language changed event
    OnLanguageChanged.Broadcast(CurrentLanguage);
    
    UE_LOG(LogMingRTSLocalization, Log, TEXT("Language changed from %s to %s"),
        *GetLanguageInfo(PreviousLanguage).LanguageName,
        *GetLanguageInfo(CurrentLanguage).LanguageName);
}

FString UMingRTSLocalizationSystem::GetLocalizedText(const FString& Key) const
{
    if (Key.IsEmpty())
    {
        return FString();
    }
    
    // Check cache first
    const FString* CachedText = TextCache.Find(Key);
    if (CachedText != nullptr)
    {
        return *CachedText;
    }
    
    // Get from current language pack
    const FLanguagePack* Pack = LoadedLanguages.Find(CurrentLanguage);
    if (Pack && Pack->bIsLoaded)
    {
        const FString* Text = Pack->TextEntries.Find(Key);
        if (Text != nullptr && !Text->IsEmpty())
        {
            // Add to cache
            TextCache.Add(Key, *Text);
            return *Text;
        }
    }
    
    // Return fallback text
    return GetFallbackText(Key);
}

FString UMingRTSLocalizationSystem::GetLocalizedTextByNamespace(const FString& Namespace, const FString& Key) const
{
    FString FullKey = Namespace + TEXT(".") + Key;
    return GetLocalizedText(FullKey);
}

ETextDirection UMingRTSLocalizationSystem::GetTextDirection() const
{
    const FLanguagePack* Pack = LoadedLanguages.Find(CurrentLanguage);
    if (Pack)
    {
        return Pack->TextDirection;
    }
    
    return ETextDirection::LTR;
}

bool UMingRTSLocalizationSystem::IsLanguageLoaded(ELanguageCode LanguageCode) const
{
    const FLanguagePack* Pack = LoadedLanguages.Find(LanguageCode);
    return Pack != nullptr && Pack->bIsLoaded;
}

bool UMingRTSLocalizationSystem::LoadLanguagePack(ELanguageCode LanguageCode)
{
    if (IsLanguageLoaded(LanguageCode))
    {
        return true;
    }
    
    FString FilePath = GetLanguageFilePath(LanguageCode);
    
    FLanguagePack NewPack;
    NewPack.LanguageCode = LanguageCode;
    NewPack.LanguageName = UEnum::GetDisplayValueAsText(LanguageCode).ToString();
    NewPack.NativeName = GetLanguageInfo(LanguageCode).NativeName;
    NewPack.TextDirection = DetectTextDirection(LanguageCode);
    
    if (LoadLanguagePackFromFile(LanguageCode, FilePath))
    {
        UE_LOG(LogMingRTSLocalization, Log, TEXT("Loaded language pack: %s (%d entries)"),
            *NewPack.LanguageName, NewPack.EntryCount);
        return true;
    }
    
    UE_LOG(LogMingRTSLocalization, Warning, TEXT("Failed to load language pack from: %s"), *FilePath);
    return false;
}

void UMingRTSLocalizationSystem::UnloadLanguagePack(ELanguageCode LanguageCode)
{
    FLanguagePack* Pack = LoadedLanguages.Find(LanguageCode);
    if (Pack && Pack->bIsLoaded)
    {
        Pack->TextEntries.Empty();
        Pack->bIsLoaded = false;
        Pack->EntryCount = 0;
        
        UE_LOG(LogMingRTSLocalization, Log, TEXT("Unloaded language pack: %s"), 
            *Pack->LanguageName);
    }
}

TArray<ELanguageCode> UMingRTSLocalizationSystem::GetSupportedLanguages() const
{
    TArray<ELanguageCode> Languages;
    
    // Add all enum values
    UEnum* EnumPtr = StaticEnum<ELanguageCode>();
    if (EnumPtr)
    {
        for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
        {
            Languages.Add(static_cast<ELanguageCode>(EnumPtr->GetValueByIndex(i)));
        }
    }
    
    return Languages;
}

FLanguagePack UMingRTSLocalizationSystem::GetLanguageInfo(ELanguageCode LanguageCode) const
{
    static const TMap<ELanguageCode, FLanguagePack> DefaultInfo = []
    {
        TMap<ELanguageCode, FLanguagePack> Info;
        
        auto AddInfo = [&Info](ELanguageCode Code, const FString& Name, 
                              const FString& Native, ETextDirection Dir)
        {
            FLanguagePack Pack;
            Pack.LanguageCode = Code;
            Pack.LanguageName = Name;
            Pack.NativeName = Native;
            Pack.TextDirection = Dir;
            Info.Add(Code, Pack);
        };
        
        AddInfo(ELanguageCode::zh_CN, TEXT("简体中文"), TEXT("简体中文"), ETextDirection::LTR);
        AddInfo(ELanguageCode::zh_TW, TEXT("繁體中文"), TEXT("繁體中文"), ETextDirection::LTR);
        AddInfo(ELanguageCode::en_US, TEXT("English"), TEXT("English"), ETextDirection::LTR);
        AddInfo(ELanguageCode::ja_JP, TEXT("日本語"), TEXT("日本語"), ETextDirection::LTR);
        AddInfo(ELanguageCode::ko_KR, TEXT("한국어"), TEXT("한국어"), ETextDirection::LTR);
        AddInfo(ELanguageCode::fr_FR, TEXT("Français"), TEXT("Français"), ETextDirection::LTR);
        AddInfo(ELanguageCode::de_DE, TEXT("Deutsch"), TEXT("Deutsch"), ETextDirection::LTR);
        AddInfo(ELanguageCode::es_ES, TEXT("Español"), TEXT("Español"), ETextDirection::LTR);
        AddInfo(ELanguageCode::ru_RU, TEXT("Русский"), TEXT("Русский"), ETextDirection::LTR);
        AddInfo(ELanguageCode::ar_SA, TEXT("العربية"), TEXT("العربية"), ETextDirection::RTL);
        
        return Info;
    }();
    
    const FLanguagePack* Info = DefaultInfo.Find(LanguageCode);
    if (Info)
    {
        return *Info;
    }
    
    return FLanguagePack();
}

void UMingRTSLocalizationSystem::ReloadCurrentLanguage()
{
    if (IsLanguageLoaded(CurrentLanguage))
    {
        UnloadLanguagePack(CurrentLanguage);
    }
    
    ClearCache();
    LoadLanguagePack(CurrentLanguage);
    PreloadCommonTexts();
    
    UE_LOG(LogMingRTSLocalization, Log, TEXT("Reloaded current language: %s"),
        *GetLanguageInfo(CurrentLanguage).LanguageName);
}

bool UMingRTSLocalizationSystem::HasKey(const FString& Key) const
{
    const FLanguagePack* Pack = LoadedLanguages.Find(CurrentLanguage);
    if (Pack && Pack->bIsLoaded)
    {
        return Pack->TextEntries.Contains(Key);
    }
    return false;
}

FString UMingRTSLocalizationSystem::FormatLocalizedText(const FString& Key, const TArray<FString>& Args) const
{
    FString Text = GetLocalizedText(Key);
    
    for (int32 i = 0; i < Args.Num(); ++i)
    {
        FString Placeholder = FString::Printf(TEXT("{%d}"), i);
        Text = Text.Replace(*Placeholder, *Args[i]);
    }
    
    return Text;
}

// Private methods

bool UMingRTSLocalizationSystem::LoadLanguagePackFromFile(ELanguageCode LanguageCode, const FString& FilePath)
{
    // Check if file exists
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
    {
        UE_LOG(LogMingRTSLocalization, Warning, TEXT("Language file not found: %s"), *FilePath);
        return false;
    }
    
    FLanguagePack& Pack = LoadedLanguages.FindOrAdd(LanguageCode);
    
    // Parse the file
    if (ParseLocresFile(FilePath, Pack))
    {
        Pack.bIsLoaded = true;
        Pack.EntryCount = Pack.TextEntries.Num();
        return true;
    }
    
    return false;
}

bool UMingRTSLocalizationSystem::ParseLocresFile(const FString& FilePath, FLanguagePack& OutPack)
{
    // Read file content
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        UE_LOG(LogMingRTSLocalization, Error, TEXT("Failed to read language file: %s"), *FilePath);
        return false;
    }
    
    // Simple JSON parsing for now (can be enhanced to use UE's JSON parser)
    // Expected format: {"Key1": "Value1", "Key2": "Value2"}
    
    // Clear existing entries
    OutPack.TextEntries.Empty();
    
    // Parse key-value pairs
    // This is a simplified implementation - in production, use proper JSON parsing
    TArray<FString> Lines;
    FileContent.ParseIntoArrayLines(Lines, true);
    
    for (const FString& Line : Lines)
    {
        // Skip empty lines and comments
        if (Line.TrimStartAndEnd().IsEmpty() || Line.TrimStart().StartsWith(TEXT("//")))
        {
            continue;
        }
        
        // Look for "Key": "Value" pattern
        int32 ColonIndex;
        if (Line.Find(TEXT("\":\""), ColonIndex) || Line.Find(TEXT("\": \""), ColonIndex))
        {
            FString Key, Value;
            
            // Extract key
            int32 KeyStart = Line.Find(TEXT("\""));
            if (KeyStart != INDEX_NONE)
            {
                int32 KeyEnd = Line.Find(TEXT("\""), ESearchCase::CaseSensitive, ESearchDir::FromStart, KeyStart + 1);
                if (KeyEnd != INDEX_NONE)
                {
                    Key = Line.Mid(KeyStart + 1, KeyEnd - KeyStart - 1);
                }
            }
            
            // Extract value
            int32 ValueStart = Line.Find(TEXT("\""), ESearchCase::CaseSensitive, ESearchDir::FromStart, ColonIndex + 1);
            if (ValueStart != INDEX_NONE)
            {
                int32 ValueEnd = Line.Find(TEXT("\""), ESearchCase::CaseSensitive, ESearchDir::FromStart, ValueStart + 1);
                if (ValueEnd == INDEX_NONE)
                {
                    // Handle case where value extends to end of line
                    ValueEnd = Line.Len();
                    if (Line.EndsWith(TEXT("\"")")))
                    {
                        ValueEnd--;
                    }
                }
                Value = Line.Mid(ValueStart + 1, ValueEnd - ValueStart - 1);
            }
            
            // Escape sequences
            Value = Value.Replace(TEXT("\\n"), TEXT("\n"));
            Value = Value.Replace(TEXT("\\t"), TEXT("\t"));
            Value = Value.Replace(TEXT("\\\""), TEXT("\""));
            Value = Value.Replace(TEXT("\\\\"), TEXT("\\"));
            
            if (!Key.IsEmpty())
            {
                OutPack.TextEntries.Add(Key, Value);
            }
        }
    }
    
    UE_LOG(LogMingRTSLocalization, Log, TEXT("Parsed %d text entries from %s"),
        OutPack.TextEntries.Num(), *FilePath);
    
    return OutPack.TextEntries.Num() > 0;
}

FString UMingRTSLocalizationSystem::GetLanguageFilePath(ELanguageCode LanguageCode) const
{
    FString LanguageCodeStr = UEnum::GetValueAsString(LanguageCode);
    LanguageCodeStr.RemoveFromStart(TEXT("ELanguageCode::"));
    
    return LocalizationConfigPath / TEXT("Core") / LanguageCodeStr / TEXT("Game.locres");
}

ETextDirection UMingRTSLocalizationSystem::DetectTextDirection(ELanguageCode LanguageCode) const
{
    switch (LanguageCode)
    {
    case ELanguageCode::ar_SA:
        return ETextDirection::RTL;
    default:
        return ETextDirection::LTR;
    }
}

void UMingRTSLocalizationSystem::ClearCache()
{
    TextCache.Empty();
}

void UMingRTSLocalizationSystem::PreloadCommonTexts()
{
    // Preload frequently used UI texts
    TArray<FString> CommonKeys = {
        TEXT("UI.OK"),
        TEXT("UI.Cancel"),
        TEXT("UI.Yes"),
        TEXT("UI.No"),
        TEXT("UI.Close"),
        TEXT("UI.Back"),
        TEXT("UI.Next"),
        TEXT("UI.Previous"),
        TEXT("UI.Confirm"),
        TEXT("UI.Loading"),
        TEXT("UI.Save"),
        TEXT("UI.Load"),
        TEXT("UI.Settings"),
        TEXT("UI.Menu"),
        TEXT("UI.Pause")
    };
    
    for (const FString& Key : CommonKeys)
    {
        GetLocalizedText(Key);
    }
}

FString UMingRTSLocalizationSystem::GetFallbackText(const FString& Key) const
{
    // Try English as fallback
    if (CurrentLanguage != ELanguageCode::en_US)
    {
        const FLanguagePack* EnglishPack = LoadedLanguages.Find(ELanguageCode::en_US);
        if (EnglishPack && EnglishPack->bIsLoaded)
        {
            const FString* Text = EnglishPack->TextEntries.Find(Key);
            if (Text != nullptr && !Text->IsEmpty())
            {
                return *Text;
            }
        }
    }
    
    // Return key as last resort
    return Key;
}
