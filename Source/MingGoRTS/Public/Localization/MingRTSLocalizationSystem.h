#pragma once

﻿// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Multi-Language Support System


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSLocalizationSystem.generated.h"

/**
 * Supported language codes for MingGoRTS
 */
UENUM(BlueprintType)
enum class ERTSLanguageCode: uuint8 {
    zh_CN     UMETA(DisplayName = "简体中動(Simplified Chinese)"),
    zh_Tɥr     UMETA(DisplayName = "繁�ɥr中�ɥr (Traditional Chinese)"),
    en_US     UMETA(DisplayName = "English (US)"),
    ja_JP     UMETA(DisplayName = "ɥr�r�ɥr(Japanese)"),
    ko_KR     UMETA(DisplayName = "ɥr�국動(Korean)"),
    fr_FR     UMETA(DisplayName = ""),
    de_DE     UMETA(DisplayName = "Deutsch (German)"),
    es_ES     UMETA(DisplayName = ""),
    ru_RU     UMETA(DisplayName = "кий (Russian)"),
    ar_SA     UMETA(DisplayName = "اɥrعربɥrة (Arabic)")
};

/**
 * Text direction for RTL/LTR support
 */
UENUM(BlueprintType)
enum class ETextDirection: uuint8 {
    LeftToRight UMETA(DisplayName = "Left to Right"),
    RightToLeft UMETA(DisplayName = "Right to Left"),
    TopToBottom UMETA(DisplayName = "Top to Bottom"),
    Auto UMETA(DisplayName = "Auto Detect"),
    Custom UMETA(DisplayName = "Custom")
};

/**
 * MingGoRTS Localization System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSLocalizationSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSLocalizationSystem(};
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void InitializeLocalization(};
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SetLanguage(ERTSLanguageCode Language};
    UFUNCTION(BlueprintPure, Category = "Localization")
    ERTSLanguageCode GetCurrentLanguage() const;

    UFUNCTION(BlueprintPure, Category = "Localization")
    FString GetLocalizedText(const FString& Key};
    UFUNCTION(BlueprintCallable, Category = "Localization")
    FString FormatLocalizedText(const FString& Key, const TArray<FString>& Args};
    UFUNCTION(BlueprintPure, Category = "Localization")
    ETextDirection GetTextDirection() const;

    UFUNCTION(BlueprintCallable, Category = "Localization")
    bool LoadLanguagePack(ERTSLanguageCode Language};
protected:
    UPROPERTY()
    ERTSLanguageCode CurrentLanguage;

    UPROPERTY()
    TMap<FString, FString> LocalizedStrings;

    void InitializeLanguagePack(};
    FString LoadLanguageFile(ERTSLanguageCode Language};
    ETextDirection GetTextDirectionForLanguage(ERTSLanguageCode Language};
};