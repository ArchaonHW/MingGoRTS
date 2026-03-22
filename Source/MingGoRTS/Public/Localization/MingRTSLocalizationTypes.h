#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSLocalizationTypes.generated.h"

UENUM(BlueprintType)
enum class EMingLanguage : uint8
{
    English UMETA(DisplayName = "English"),
    SimplifiedChinese UMETA(DisplayName = "简体中文"),
    TraditionalChinese UMETA(DisplayName = "繁體中文"),
    Japanese UMETA(DisplayName = "日本語"),
    Korean UMETA(DisplayName = "한국어"),
    French UMETA(DisplayName = "Français"),
    German UMETA(DisplayName = "Deutsch"),
    Spanish UMETA(DisplayName = "Español"),
    Italian UMETA(DisplayName = "Italiano"),
    Russian UMETA(DisplayName = "Русский"),
    Arabic UMETA(DisplayName = "العربية"),
    Portuguese UMETA(DisplayName = "Português"),
    Hindi UMETA(DisplayName = "हिन्दी"),
    Thai UMETA(DisplayName = "ไทย"),
    Vietnamese UMETA(DisplayName = "Tiếng Việt"),
    Indonesian UMETA(DisplayName = "Bahasa Indonesia"),
    Turkish UMETA(DisplayName = "Türkçe"),
    Polish UMETA(DisplayName = "Polski"),
    Dutch UMETA(DisplayName = "Nederlands"),
    Custom UMETA(DisplayName = "Custom"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingRegion : uint8
{
    Global UMETA(DisplayName = "Global"),
    Asia UMETA(DisplayName = "Asia"),
    Europe UMETA(DisplayName = "Europe"),
    Americas UMETA(DisplayName = "Americas"),
    Africa UMETA(DisplayName = "Africa"),
    Oceania UMETA(DisplayName = "Oceania"),
    MiddleEast UMETA(DisplayName = "Middle East"),
    China UMETA(DisplayName = "China"),
    Japan UMETA(DisplayName = "Japan"),
    Korea UMETA(DisplayName = "Korea"),
    SoutheastAsia UMETA(DisplayName = "Southeast Asia"),
    India UMETA(DisplayName = "India"),
    WesternEurope UMETA(DisplayName = "Western Europe"),
    EasternEurope UMETA(DisplayName = "Eastern Europe"),
    NorthAmerica UMETA(DisplayName = "North America"),
    SouthAmerica UMETA(DisplayName = "South America"),
    Custom UMETA(DisplayName = "Custom"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingTextDirection : uint8
{
    LeftToRight UMETA(DisplayName = "Left to Right"),
    RightToLeft UMETA(DisplayName = "Right to Left"),
    TopToBottom UMETA(DisplayName = "Top to Bottom"),
    AutoDetect UMETA(DisplayName = "Auto Detect"),
    Custom UMETA(DisplayName = "Custom"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingDateFormat : uint8
{
    MDY UMETA(DisplayName = "MM/DD/YYYY"),
    DMY UMETA(DisplayName = "DD/MM/YYYY"),
    YMD UMETA(DisplayName = "YYYY/MM/DD"),
    ISO UMETA(DisplayName = "ISO 8601"),
    Custom UMETA(DisplayName = "Custom"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingNumberFormat : uint8
{
    DotDecimal UMETA(DisplayName = "1,234.56"),
    CommaDecimal UMETA(DisplayName = "1.234,56"),
    SpaceDecimal UMETA(DisplayName = "1 234,56"),
    ArabicNumerals UMETA(DisplayName = "Arabic Numerals"),
    HindiNumerals UMETA(DisplayName = "Hindi Numerals"),
    Custom UMETA(DisplayName = "Custom"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingCurrencySymbol : uint8
{
    USD UMETA(DisplayName = "$"),
    EUR UMETA(DisplayName = "€"),
    GBP UMETA(DisplayName = "£"),
    JPY UMETA(DisplayName = "¥"),
    CNY UMETA(DisplayName = "¥"),
    KRW UMETA(DisplayName = "₩"),
    INR UMETA(DisplayName = "₹"),
    Custom UMETA(DisplayName = "Custom"),
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMingCultureConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Culture")
    EMingLanguage Language;

    UPROPERTY(BlueprintReadWrite, Category = "Culture")
    EMingRegion Region;

    UPROPERTY(BlueprintReadWrite, Category = "Culture")
    EMingTextDirection TextDirection;

    UPROPERTY(BlueprintReadWrite, Category = "Culture")
    EMingDateFormat DateFormat;

    UPROPERTY(BlueprintReadWrite, Category = "Culture")
    EMingNumberFormat NumberFormat;

    UPROPERTY(BlueprintReadWrite, Category = "Culture")
    EMingCurrencySymbol CurrencySymbol;

    UPROPERTY(BlueprintReadWrite, Category = "Culture")
    FString TimeFormat;

    UPROPERTY(BlueprintReadWrite, Category = "Culture")
    FString CustomDecimalSeparator;

    UPROPERTY(BlueprintReadWrite, Category = "Culture")
    FString CustomThousandsSeparator;

    FMingCultureConfiguration()
        : Language(EMingLanguage::English)
        , Region(EMingRegion::Global)
        , TextDirection(EMingTextDirection::LeftToRight)
        , DateFormat(EMingDateFormat::MDY)
        , NumberFormat(EMingNumberFormat::DotDecimal)
        , CurrencySymbol(EMingCurrencySymbol::USD)
        , TimeFormat(TEXT("12H"))
    {}
};

USTRUCT(BlueprintType)
struct FMingLocalizationEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Localization")
    FString Key;

    UPROPERTY(BlueprintReadWrite, Category = "Localization")
    TMap<EMingLanguage, FString> Translations;

    UPROPERTY(BlueprintReadWrite, Category = "Localization")
    FString Context;

    UPROPERTY(BlueprintReadWrite, Category = "Localization")
    FString Category;

    UPROPERTY(BlueprintReadWrite, Category = "Localization")
    int32 MaxLength;

    UPROPERTY(BlueprintReadWrite, Category = "Localization")
    bool bRequiresReview;

    FMingLocalizationEntry()
        : MaxLength(0)
        , bRequiresReview(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingLocalizedText
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Localization")
    FString Key;

    UPROPERTY(BlueprintReadOnly, Category = "Localization")
    FString Text;

    UPROPERTY(BlueprintReadOnly, Category = "Localization")
    EMingLanguage Language;

    UPROPERTY(BlueprintReadOnly, Category = "Localization")
    bool bIsTranslated;

    FMingLocalizedText()
        : Language(EMingLanguage::English)
        , bIsTranslated(false)
    {}
};
