// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Localization Blueprint Function Library Implementation

#include "Localization/MingRTSLocalizationLibrary.h"
#include "Localization/MingRTSLocalizationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/FInternationalization.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSLocalizationLib, Log, All);

// Singleton accessor for localization system
UMingRTSLocalizationSystem* GetLocalizationSystem()
{
    // In production, this would be managed by a game instance or subsystem
    // For now, use a static instance
    static UMingRTSLocalizationSystem* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingRTSLocalizationSystem>();
        Instance->InitializeLocalizationSystem();
    }
    return Instance;
}

FString UMingRTSLocalizationLibrary::GetLocalizedText(const FString& Key)
{
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        return LocSystem->GetLocalizedText(Key);
    }
    return Key;
}

FString UMingRTSLocalizationLibrary::GetLocalizedTextEx(const FString& Namespace, const FString& Key)
{
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        return LocSystem->GetLocalizedTextByNamespace(Namespace, Key);
    }
    return Key;
}

void UMingRTSLocalizationLibrary::SetGameLanguage(TEnumAsByte<enum class ELanguageCode> LanguageCode)
{
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        LocSystem->SetLanguage(LanguageCode);
    }
}

TEnumAsByte<enum class ELanguageCode> UMingRTSLocalizationLibrary::GetCurrentGameLanguage()
{
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        return LocSystem->GetCurrentLanguage();
    }
    return ELanguageCode::zh_CN;
}

FString UMingRTSLocalizationLibrary::FormatLocalizedText(const FString& Key, const TArray<FString>& Args)
{
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        return LocSystem->FormatLocalizedText(Key, Args);
    }
    return Key;
}

bool UMingRTSLocalizationLibrary::HasLocalizedKey(const FString& Key)
{
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        return LocSystem->HasKey(Key);
    }
    return false;
}

TEnumAsByte<enum class ETextDirection> UMingRTSLocalizationLibrary::GetTextDirection()
{
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        return LocSystem->GetTextDirection();
    }
    return ETextDirection::LTR;
}

bool UMingRTSLocalizationLibrary::IsRightToLeft()
{
    return GetTextDirection() == ETextDirection::RTL;
}

FString UMingRTSLocalizationLibrary::GetLanguageDisplayName(TEnumAsByte<enum class ELanguageCode> LanguageCode)
{
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        FLanguagePack Info = LocSystem->GetLanguageInfo(LanguageCode);
        return Info.LanguageName;
    }
    
    // Fallback to enum display name
    UEnum* EnumPtr = StaticEnum<ELanguageCode>();
    if (EnumPtr)
    {
        return EnumPtr->GetDisplayNameTextByValue((int64)LanguageCode).ToString();
    }
    
    return TEXT("Unknown");
}

FString UMingRTSLocalizationLibrary::GetLanguageNativeName(TEnumAsByte<enum class ELanguageCode> LanguageCode)
{
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        FLanguagePack Info = LocSystem->GetLanguageInfo(LanguageCode);
        return Info.NativeName;
    }
    return GetLanguageDisplayName(LanguageCode);
}

TArray<TEnumAsByte<enum class ELanguageCode>> UMingRTSLocalizationLibrary::GetSupportedLanguages()
{
    TArray<TEnumAsByte<enum class ELanguageCode>> Languages;
    
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

FString UMingRTSLocalizationLibrary::TruncateText(const FString& Text, int32 MaxLength, 
    const FString& Ellipsis)
{
    if (MaxLength <= 0 || Text.Len() <= MaxLength)
    {
        return Text;
    }
    
    int32 TruncateLength = MaxLength - Ellipsis.Len();
    if (TruncateLength <= 0)
    {
        return Ellipsis.Left(MaxLength);
    }
    
    return Text.Left(TruncateLength) + Ellipsis;
}

FString UMingRTSLocalizationLibrary::TextToUpper(const FString& Text)
{
    return Text.ToUpper();
}

FString UMingRTSLocalizationLibrary::TextToLower(const FString& Text)
{
    return Text.ToLower();
}

FString UMingRTSLocalizationLibrary::GetPluralizedText(const FString& KeySingular, 
    const FString& KeyPlural, int32 Count)
{
    if (Count == 1)
    {
        return FormatLocalizedText(KeySingular, { FString::FromInt(Count) });
    }
    else
    {
        return FormatLocalizedText(KeyPlural, { FString::FromInt(Count) });
    }
}

void UMingRTSLocalizationLibrary::ReloadLanguage()
{
#if WITH_EDITOR
    if (UMingRTSLocalizationSystem* LocSystem = GetLocalizationSystem())
    {
        LocSystem->ReloadCurrentLanguage();
        UE_LOG(LogMingRTSLocalizationLib, Log, TEXT("Language reloaded for development"));
    }
#else
    UE_LOG(LogMingRTSLocalizationLib, Warning, TEXT("Language reload only available in editor"));
#endif
}

FString UMingRTSLocalizationLibrary::FormatCurrency(int32 Amount, const FString& CurrencyCode)
{
    // Format currency based on currency code
    if (CurrencyCode == TEXT("USD"))
    {
        return FString::Printf(TEXT("$%s"), *FormatNumber(Amount));
    }
    else if (CurrencyCode == TEXT("EUR"))
    {
        return FString::Printf(TEXT("€%s"), *FormatNumber(Amount));
    }
    else if (CurrencyCode == TEXT("GBP"))
    {
        return FString::Printf(TEXT("£%s"), *FormatNumber(Amount));
    }
    else if (CurrencyCode == TEXT("CNY"))
    {
        return FString::Printf(TEXT("¥%s"), *FormatNumber(Amount));
    }
    else if (CurrencyCode == TEXT("JPY"))
    {
        return FString::Printf(TEXT("¥%s"), *FormatNumber(Amount));
    }
    else if (CurrencyCode == TEXT("KRW"))
    {
        return FString::Printf(TEXT("₩%s"), *FormatNumber(Amount));
    }
    else if (CurrencyCode == TEXT("RUB"))
    {
        return FString::Printf(TEXT("₽%s"), *FormatNumber(Amount));
    }
    
    // Default: just format the number
    return FormatNumber(Amount);
}

FString UMingRTSLocalizationLibrary::FormatNumber(int32 Number)
{
    // Use UE's internationalization for number formatting
    FInternationalization& I18N = FInternationalization::Get();
    FCultureRef CurrentCulture = I18N.GetCurrentCulture();
    
    // Format with thousands separator
    FString Result = FString::FromInt(Number);
    
    // Add thousands separators
    int32 Length = Result.Len();
    if (Length > 3 && Number >= 0)
    {
        for (int32 i = Length - 3; i > 0; i -= 3)
        {
            Result.InsertAt(i, TEXT(","));
        }
    }
    
    return Result;
}

FString UMingRTSLocalizationLibrary::FormatDate(const FDateTime& Date)
{
    // Format: YYYY-MM-DD
    return FString::Printf(TEXT("%04d-%02d-%02d"),
        Date.GetYear(), Date.GetMonth(), Date.GetDay());
}

FString UMingRTSLocalizationLibrary::GetGameTitle()
{
    return GetLocalizedText(TEXT("Game.Title"));
}
