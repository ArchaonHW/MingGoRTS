// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Localization Subsystem Implementation

#include "Localization/MingRTSLocalizationSubsystem.h"
#include "Localization/MingRTSLocalizationSystem.h"
#include "Localization/MingRTSTextManager.h"
#include "Kismet/GameplayStatics.h"
#include "Internationalization/Internationalization.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSLocalizationSubsystem, Log, All);

const FString UMingRTSLocalizationSubsystem::LanguageSettingsSlot = TEXT("MingRTS_LanguageSettings");

void UMingRTSLocalizationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("Initializing Localization Subsystem..."));
    
    // Create localization system
    LocalizationSystem = NewObject<UMingRTSLocalizationSystem>(this);
    
    // Create text manager
    TextManager = NewObject<UMingRTSTextManager>(this);
    
    // Bind to language change events
    if (LocalizationSystem)
    {
        LocalizationSystem->OnLanguageChanged.AddDynamic(this, &UMingRTSLocalizationSubsystem::HandleLanguageChanged);
    }
}

void UMingRTSLocalizationSubsystem::Deinitialize()
{
    UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("Deinitializing Localization Subsystem..."));
    
    // Unbind events
    if (LocalizationSystem)
    {
        LocalizationSystem->OnLanguageChanged.RemoveDynamic(this, &UMingRTSLocalizationSubsystem::HandleLanguageChanged);
        LocalizationSystem->ShutdownLocalizationSystem();
    }
    
    LocalizationSystem = nullptr;
    TextManager = nullptr;
    CulturalSystem = nullptr;
    
    Super::Deinitialize();
}

bool UMingRTSLocalizationSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UMingRTSLocalizationSubsystem::InitializeLocalization()
{
    UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("Initializing Localization..."));
    
    if (LocalizationSystem)
    {
        LocalizationSystem->InitializeLocalizationSystem();
    }
    
    if (TextManager)
    {
        TextManager->InitializeTextManager();
    }
    
    if (CulturalSystem)
    {
        CulturalSystem->InitializeCulturalSystem();
    }
    
    // Load saved language preference or use system default
    InitializeFromSettings();
    
    UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("Localization initialized successfully"));
}

FString UMingRTSLocalizationSubsystem::GetCulturalContent(const FString& ContentKey) const
{
    if (CulturalSystem)
    {
        return CulturalSystem->GetAdaptedContentForCurrentRegion(ContentKey);
    }
    return ContentKey;
}

void UMingRTSLocalizationSubsystem::SetCulturalRegion(ECulturalRegion Region)
{
    if (CulturalSystem)
    {
        CulturalSystem->SetPlayerRegion(Region);
    }
}

ECulturalRegion UMingRTSLocalizationSubsystem::GetCurrentCulturalRegion() const
{
    if (CulturalSystem)
    {
        return CulturalSystem->GetCurrentRegion();
    }
    return ECulturalRegion::EastAsia;
}

FString UMingRTSLocalizationSubsystem::GetText(const FString& Key) const
{
    if (LocalizationSystem)
    {
        return LocalizationSystem->GetLocalizedText(Key);
    }
    return Key;
}

FString UMingRTSLocalizationSubsystem::GetTextEx(const FString& Namespace, const FString& Key) const
{
    if (LocalizationSystem)
    {
        return LocalizationSystem->GetLocalizedTextByNamespace(Namespace, Key);
    }
    return Key;
}

void UMingRTSLocalizationSubsystem::SetLanguage(TEnumAsByte<enum class ELanguageCode> LanguageCode)
{
    if (LocalizationSystem)
    {
        ELanguageCode PreviousLanguage = LocalizationSystem->GetCurrentLanguage();
        LocalizationSystem->SetLanguage(LanguageCode);
        
        // Save preference
        SaveUserLanguagePreference(LanguageCode);
        
        UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("Language set to %s"),
            *UEnum::GetValueAsString(LanguageCode));
    }
}

TEnumAsByte<enum class ELanguageCode> UMingRTSLocalizationSubsystem::GetCurrentLanguage() const
{
    if (LocalizationSystem)
    {
        return LocalizationSystem->GetCurrentLanguage();
    }
    return ELanguageCode::zh_CN;
}

ELanguageCode UMingRTSLocalizationSubsystem::GetSystemPreferredLanguage() const
{
    // Get system locale
    FCultureRef SystemCulture = FInternationalization::Get().GetDefaultCulture();
    FString SystemLocale = SystemCulture->GetName();
    
    UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("System locale: %s"), *SystemLocale);
    
    // Map system locale to our language codes
    if (SystemLocale.StartsWith(TEXT("zh-Hans")) || SystemLocale == TEXT("zh-CN"))
    {
        return ELanguageCode::zh_CN;
    }
    else if (SystemLocale.StartsWith(TEXT("zh-Hant")) || SystemLocale == TEXT("zh-TW"))
    {
        return ELanguageCode::zh_TW;
    }
    else if (SystemLocale.StartsWith(TEXT("ja")))
    {
        return ELanguageCode::ja_JP;
    }
    else if (SystemLocale.StartsWith(TEXT("ko")))
    {
        return ELanguageCode::ko_KR;
    }
    else if (SystemLocale.StartsWith(TEXT("fr")))
    {
        return ELanguageCode::fr_FR;
    }
    else if (SystemLocale.StartsWith(TEXT("de")))
    {
        return ELanguageCode::de_DE;
    }
    else if (SystemLocale.StartsWith(TEXT("es")))
    {
        return ELanguageCode::es_ES;
    }
    else if (SystemLocale.StartsWith(TEXT("ru")))
    {
        return ELanguageCode::ru_RU;
    }
    else if (SystemLocale.StartsWith(TEXT("ar")))
    {
        return ELanguageCode::ar_SA;
    }
    
    // Default to English for all other locales
    return ELanguageCode::en_US;
}

ELanguageCode UMingRTSLocalizationSubsystem::LoadUserLanguagePreference() const
{
    // Load from save game or config
    // For now, use a simple config approach
    FString SavedLanguage;
    if (GConfig->GetString(TEXT("Localization"), TEXT("PreferredLanguage"), 
        SavedLanguage, GGameIni))
    {
        UEnum* EnumPtr = StaticEnum<ELanguageCode>();
        if (EnumPtr)
        {
            int64 Value = EnumPtr->GetValueByNameString(SavedLanguage);
            if (Value != INDEX_NONE)
            {
                return static_cast<ELanguageCode>(Value);
            }
        }
    }
    
    // Return system preferred language if no saved preference
    return GetSystemPreferredLanguage();
}

void UMingRTSLocalizationSubsystem::SaveUserLanguagePreference(ELanguageCode LanguageCode)
{
    FString LanguageString = UEnum::GetValueAsString(LanguageCode);
    LanguageString.RemoveFromStart(TEXT("ELanguageCode::"));
    
    // Save to config
    GConfig->SetString(TEXT("Localization"), TEXT("PreferredLanguage"), 
        *LanguageString, GGameIni);
    GConfig->Flush(false, GGameIni);
    
    UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("Saved language preference: %s"), *LanguageString);
}

bool UMingRTSLocalizationSubsystem::IsLanguageAvailable(ELanguageCode LanguageCode) const
{
    if (LocalizationSystem)
    {
        // Check if language pack exists
        FString FilePath = LocalizationSystem->GetLanguageInfo(LanguageCode).LanguageName;
        // In full implementation, check if file exists
        return true; // Assume all languages are available for now
    }
    return false;
}

TArray<FLanguageOption> UMingRTSLocalizationSubsystem::GetLanguageOptions() const
{
    TArray<FLanguageOption> Options;
    
    if (!LocalizationSystem)
    {
        return Options;
    }
    
    ELanguageCode CurrentLang = GetCurrentLanguage();
    TArray<ELanguageCode> SupportedLanguages = LocalizationSystem->GetSupportedLanguages();
    
    for (ELanguageCode LangCode : SupportedLanguages)
    {
        FLanguageOption Option;
        Option.LanguageCode = LangCode;
        
        FLanguagePack Info = LocalizationSystem->GetLanguageInfo(LangCode);
        Option.DisplayName = Info.LanguageName;
        Option.NativeName = Info.NativeName;
        Option.bIsAvailable = IsLanguageAvailable(LangCode);
        Option.bIsSelected = (LangCode == CurrentLang);
        
        Options.Add(Option);
    }
    
    return Options;
}

void UMingRTSLocalizationSubsystem::InitializeFromSettings()
{
    // Load saved preference
    ELanguageCode PreferredLanguage = LoadUserLanguagePreference();
    
    // Check if language is available
    if (IsLanguageAvailable(PreferredLanguage))
    {
        SetLanguage(PreferredLanguage);
        UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("Initialized with saved language: %s"),
            *UEnum::GetValueAsString(PreferredLanguage));
    }
    else
    {
        // Fall back to Chinese
        SetLanguage(ELanguageCode::zh_CN);
        UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("Saved language not available, using default: zh_CN"));
    }
}

void UMingRTSLocalizationSubsystem::HandleLanguageChanged(ELanguageCode NewLanguage)
{
    // Re-broadcast to subsystem listeners
    OnLanguageChanged.Broadcast(NewLanguage);
    
    UE_LOG(LogMingRTSLocalizationSubsystem, Log, TEXT("Language changed event broadcast: %s"),
        *UEnum::GetValueAsString(NewLanguage));
}
