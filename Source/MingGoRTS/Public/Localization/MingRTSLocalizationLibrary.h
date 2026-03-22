// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Localization Blueprint Function Library

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Localization/MingRTSLocalizationSystem.h"
#include "MingRTSLocalizationLibrary.generated.h"

/**
 * Blueprint function library for localization operations
 * Provides easy access to localization from Blueprints
 */
UCLASS()
class MINGRTS_API UMingRTSLocalizationLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
    
public:
    /** Get localized text by key */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject"))
    static FString GetLocalizedText(const FString& Key);
    
    /** Get localized text with namespace */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject"))
    static FString GetLocalizedTextEx(const FString& Namespace, const FString& Key);
    
    /** Set current language */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject"))
    static void SetGameLanguage(ERTSLanguageCode LanguageCode);
    
    /** Get current language */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject"))
    static ERTSLanguageCode GetCurrentGameLanguage();
    
    /** Format text with arguments */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "Args"))
    static FString FormatLocalizedText(const FString& Key, const TArray<FString>& Args);
    
    /** Check if key exists */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject"))
    static bool HasLocalizedKey(const FString& Key);
    
    /** Get text direction for current language */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject"))
    static ETextDirection GetTextDirection();
    
    /** Is current language RTL? */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject"))
    static bool IsRightToLeft();
    
    /** Get language display name */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization")
    static FString GetLanguageDisplayName(ERTSLanguageCode LanguageCode);
    
    /** Get language native name */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization")
    static FString GetLanguageNativeName(ERTSLanguageCode LanguageCode);
    
    /** Get all supported languages */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization")
    static TArray<TEnumAsByte<ERTSLanguageCode>> GetSupportedLanguages();
    
    /** Truncate text to max length */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    static FString TruncateText(const FString& Text, int32 MaxLength, 
                                const FString& Ellipsis);
    
    /** Convert to uppercase */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    static FString TextToUpper(const FString& Text);
    
    /** Convert to lowercase */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    static FString TextToLower(const FString& Text);
    
    /** Get pluralized text (singular/plural based on count) */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject"))
    static FString GetPluralizedText(const FString& KeySingular, 
                                     const FString& KeyPlural, 
                                     int32 Count);
    
    /** Reload current language (development only) */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|Development",
             meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
    static void ReloadLanguage();
    
    /** Format currency value for current locale */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    static FString FormatCurrency(int32 Amount, const FString& CurrencyCode);
    
    /** Format number with locale-specific separators */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    static FString FormatNumber(int32 Number);
    
    /** Format date for current locale */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization")
    static FString FormatDate(const FDateTime& Date);
    
    /** Get localized game title */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization",
             meta = (WorldContext = "WorldContextObject"))
    static FString GetGameTitle();
};


