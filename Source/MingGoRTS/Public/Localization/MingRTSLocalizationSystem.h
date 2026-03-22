// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Multi-Language Support System

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSLocalizationSystem.generated.h"

/**
 * Supported language codes for MingGoRTS
 */
UENUM(BlueprintType)
enum class ELanguageCode : uint8
{
    zh_CN     UMETA(DisplayName = "简体中文 (Simplified Chinese)"),
    zh_TW     UMETA(DisplayName = "繁體中文 (Traditional Chinese)"),
    en_US     UMETA(DisplayName = "English (US)"),
    ja_JP     UMETA(DisplayName = "日本語 (Japanese)"),
    ko_KR     UMETA(DisplayName = "한국어 (Korean)"),
    fr_FR     UMETA(DisplayName = "Français (French)"),
    de_DE     UMETA(DisplayName = "Deutsch (German)"),
    es_ES     UMETA(DisplayName = "Español (Spanish)"),
    ru_RU     UMETA(DisplayName = "Русский (Russian)"),
    ar_SA     UMETA(DisplayName = "العربية (Arabic)")
};

/**
 * Text direction for RTL/LTR support
 */
UENUM(BlueprintType)
enum class ETextDirection : uint8
{
    LTR       UMETA(DisplayName = "Left to Right"),
    RTL       UMETA(DisplayName = "Right to Left"),
    Auto      UMETA(DisplayName = "Auto Detect")
};

/**
 * Language pack data structure
 */
USTRUCT()
struct FLanguagePack
{
    GENERATED_BODY()
    
    UPROPERTY()
    ELanguageCode LanguageCode;
    
    UPROPERTY()
    FString LanguageName;
    
    UPROPERTY()
    FString NativeName;
    
    UPROPERTY()
    ETextDirection TextDirection;
    
    UPROPERTY()
    TMap<FString, FString> TextEntries;
    
    UPROPERTY()
    bool bIsLoaded = false;
    
    UPROPERTY()
    int32 EntryCount = 0;
    
    FLanguagePack()
        : LanguageCode(ELanguageCode::en_US)
        , TextDirection(ETextDirection::LTR)
        , bIsLoaded(false)
        , EntryCount(0)
    {}
};

/**
 * Main localization system for MingGoRTS
 * Manages multiple languages, text lookup, and language switching
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UMingRTSLocalizationSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSLocalizationSystem();
    
    /** Initialize the localization system */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    void InitializeLocalizationSystem();
    
    /** Shutdown and cleanup */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    void ShutdownLocalizationSystem();
    
    /** Set current language */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    void SetLanguage(ELanguageCode LanguageCode);
    
    /** Get current language */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    ELanguageCode GetCurrentLanguage() const { return CurrentLanguage; }
    
    /** Get localized text by key */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    FString GetLocalizedText(const FString& Key) const;
    
    /** Get localized text with namespace */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    FString GetLocalizedTextByNamespace(const FString& Namespace, const FString& Key) const;
    
    /** Get text direction for current language */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    ETextDirection GetTextDirection() const;
    
    /** Check if language is loaded */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    bool IsLanguageLoaded(ELanguageCode LanguageCode) const;
    
    /** Load language pack */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    bool LoadLanguagePack(ELanguageCode LanguageCode);
    
    /** Unload language pack to free memory */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    void UnloadLanguagePack(ELanguageCode LanguageCode);
    
    /** Get all supported languages */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    TArray<ELanguageCode> GetSupportedLanguages() const;
    
    /** Get language info */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    FLanguagePack GetLanguageInfo(ELanguageCode LanguageCode) const;
    
    /** Reload current language (for hot-reload during development) */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    void ReloadCurrentLanguage();
    
    /** Check if key exists */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    bool HasKey(const FString& Key) const;
    
    /** Get translation with format arguments */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization", 
             meta = (AutoCreateRefTerm = "Args"))
    FString FormatLocalizedText(const FString& Key, const TArray<FString>& Args) const;

    /** Event: Language changed */
    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Localization|Events")
    FOnLanguageChanged OnLanguageChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLanguageChanged, ELanguageCode, NewLanguage);

private:
    /** Current active language */
    UPROPERTY()
    ELanguageCode CurrentLanguage;
    
    /** Language packs storage */
    UPROPERTY()
    TMap<ELanguageCode, FLanguagePack> LoadedLanguages;
    
    /** Text cache for current language */
    mutable TMap<FString, FString> TextCache;
    
    /** Configuration path */
    FString LocalizationConfigPath;
    
    /** Load language pack from file */
    bool LoadLanguagePackFromFile(ELanguageCode LanguageCode, const FString& FilePath);
    
    /** Parse locres file format */
    bool ParseLocresFile(const FString& FilePath, FLanguagePack& OutPack);
    
    /** Get default language file path */
    FString GetLanguageFilePath(ELanguageCode LanguageCode) const;
    
    /** Detect text direction from language code */
    ETextDirection DetectTextDirection(ELanguageCode LanguageCode) const;
    
    /** Clear text cache */
    void ClearCache();
    
    /** Preload common texts to cache */
    void PreloadCommonTexts();
    
    /** Get fallback key if translation missing */
    FString GetFallbackText(const FString& Key) const;
};
