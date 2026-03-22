#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Internationalization/Text.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "MingLocalizationManager.generated.h"

// Forward declarations
class UMingPersonalManager;
class UMingLanguageSystem;
class UMingCultureAdapter;

// Supported languages
UENUM(BlueprintType)
enum class EMingLanguage : uint8
{
    ChineseSimplified  UMETA(DisplayName = "Chinese (Simplified)"),
    ChineseTraditional UMETA(DisplayName = "Chinese (Traditional)"),
    English            UMETA(DisplayName = "English"),
    Japanese           UMETA(DisplayName = "Japanese"),
    Korean             UMETA(DisplayName = "Korean"),
    French             UMETA(DisplayName = "French"),
    German             UMETA(DisplayName = "German"),
    Spanish            UMETA(DisplayName = "Spanish"),
    Russian            UMETA(DisplayName = "Russian"),
    Arabic             UMETA(DisplayName = "Arabic"),
    Portuguese         UMETA(DisplayName = "Portuguese"),
    Italian            UMETA(DisplayName = "Italian"),
    Dutch              UMETA(DisplayName = "Dutch"),
    Polish             UMETA(DisplayName = "Polish"),
    Turkish            UMETA(DisplayName = "Turkish"),
    Vietnamese         UMETA(DisplayName = "Vietnamese"),
    Thai               UMETA(DisplayName = "Thai"),
    Indonesian         UMETA(DisplayName = "Indonesian")
};

// Supported regions
UENUM(BlueprintType)
enum class EMingRegion : uint8
{
    MainlandChina      UMETA(DisplayName = "Mainland China"),
    Taiwan             UMETA(DisplayName = "Taiwan"),
    HongKong           UMETA(DisplayName = "Hong Kong"),
    Macau              UMETA(DisplayName = "Macau"),
    Japan              UMETA(DisplayName = "Japan"),
    SouthKorea         UMETA(DisplayName = "South Korea"),
    UnitedStates       UMETA(DisplayName = "United States"),
    UnitedKingdom      UMETA(DisplayName = "United Kingdom"),
    Germany            UMETA(DisplayName = "Germany"),
    France             UMETA(DisplayName = "France"),
    Spain              UMETA(DisplayName = "Spain"),
    Italy              UMETA(DisplayName = "Italy"),
    Russia             UMETA(DisplayName = "Russia"),
    Brazil             UMETA(DisplayName = "Brazil"),
    Mexico             UMETA(DisplayName = "Mexico"),
    Australia          UMETA(DisplayName = "Australia"),
    Canada             UMETA(DisplayName = "Canada"),
    SoutheastAsia      UMETA(DisplayName = "Southeast Asia")
};

// Language pack info
USTRUCT(BlueprintType)
struct FLanguagePackInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString PackID;

    UPROPERTY(BlueprintReadWrite)
    FString PackName;

    UPROPERTY(BlueprintReadWrite)
    EMingLanguage Language;

    UPROPERTY(BlueprintReadWrite)
    EMingRegion Region;

    UPROPERTY(BlueprintReadWrite)
    FString Version;

    UPROPERTY(BlueprintReadWrite)
    int32 TotalStrings;

    UPROPERTY(BlueprintReadWrite)
    int64 PackSize;

    UPROPERTY(BlueprintReadWrite)
    bool bIsInstalled;

    FLanguagePackInfo()
        : Language(EMingLanguage::ChineseTraditional)
        , Region(EMingRegion::Taiwan)
        , TotalStrings(0)
        , PackSize(0)
        , bIsInstalled(false)
    {}
};

// Localization text entry
USTRUCT(BlueprintType)
struct FLocalizedTextEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString Key;

    UPROPERTY(BlueprintReadWrite)
    FString Text;

    UPROPERTY(BlueprintReadWrite)
    EMingLanguage Language;

    UPROPERTY(BlueprintReadWrite)
    FString Context;

    FLocalizedTextEntry()
        : Language(EMingLanguage::ChineseTraditional)
    {}
};

/**
 * Localization Manager
 * Handles multi-language support and cultural adaptation
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingLocalizationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingLocalizationManager(};

    // Initialize localization system
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void InitializeLocalizationManager();

    // Set current language
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SetLanguage(EMingLanguage NewLanguage);

    // Get current language
    UFUNCTION(BlueprintPure, Category = "Localization")
    EMingLanguage GetCurrentLanguage() const { return CurrentLanguage; }

    // Set current region
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SetRegion(EMingRegion NewRegion);

    // Get current region
    UFUNCTION(BlueprintPure, Category = "Localization")
    EMingRegion GetCurrentRegion() const { return CurrentRegion; }

    // Get localized text
    UFUNCTION(BlueprintCallable, Category = "Localization")
    FString GetLocalizedText(const FString& Key, const FString& DefaultText = TEXT("")) const;

    // Get localized text with format
    UFUNCTION(BlueprintCallable, Category = "Localization")
    FString GetFormattedLocalizedText(const FString& Key, const TArray<FString>& Args) const;

    // Add localized text
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void AddLocalizedText(const FString& Key, const FString& Text, EMingLanguage Language);

    // Load language pack
    UFUNCTION(BlueprintCallable, Category = "Localization")
    bool LoadLanguagePack(const FLanguagePackInfo& PackInfo);

    // Unload language pack
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void UnloadLanguagePack(const FString& PackID);

    // Get loaded language packs
    UFUNCTION(BlueprintCallable, Category = "Localization")
    TArray<FLanguagePackInfo> GetLoadedLanguagePacks() const;

    // Check if language pack is available
    UFUNCTION(BlueprintCallable, Category = "Localization")
    bool IsLanguagePackAvailable(EMingLanguage Language) const;

    // Get supported languages
    UFUNCTION(BlueprintPure, Category = "Localization")
    TArray<EMingLanguage> GetSupportedLanguages() const;

    // Get supported regions
    UFUNCTION(BlueprintPure, Category = "Localization")
    TArray<EMingRegion> GetSupportedRegions() const;

    // Get language display name
    UFUNCTION(BlueprintPure, Category = "Localization")
    static FString GetLanguageDisplayName(EMingLanguage Language);

    // Get region display name
    UFUNCTION(BlueprintPure, Category = "Localization")
    static FString GetRegionDisplayName(EMingRegion Region);

    // Detect system language
    UFUNCTION(BlueprintCallable, Category = "Localization")
    EMingLanguage DetectSystemLanguage() const;

    // Detect system region
    UFUNCTION(BlueprintCallable, Category = "Localization")
    EMingRegion DetectSystemRegion() const;

    // Save localization settings
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SaveLocalizationSettings();

    // Load localization settings
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void LoadLocalizationSettings();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Localization Events")
    FOnLocalizationLanguageChanged OnLanguageChanged;

    UPROPERTY(BlueprintAssignable, Category = "Localization Events")
    FOnLocalizationRegionChanged OnRegionChanged;

private:
    UPROPERTY()
    EMingLanguage CurrentLanguage;

    UPROPERTY()
    EMingRegion CurrentRegion;

    UPROPERTY()
    TMap<FString, FLocalizedTextEntry> LocalizedTextMap;

    UPROPERTY()
    TArray<FLanguagePackInfo> LoadedPacks;

    UPROPERTY()
    TMap<EMingLanguage, bool> AvailableLanguages;

    void LoadDefaultLanguagePack(};
    void ClearLocalizedTextCache(};
    void NotifyLanguageChanged(};
    void NotifyRegionChanged(};
};

// Event delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLocalizationLanguageChanged, EMingLanguage, NewLanguage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLocalizationRegionChanged, EMingRegion, NewRegion);

