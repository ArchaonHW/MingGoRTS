// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Text Manager Component

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSTextManager.generated.h"

/**
 * Text category for organization
 */
UENUM(BlueprintType)
enum class ETextCategory: uint8 {
    UI                UMETA(DisplayName = "UI Text"),
    Dialogue          UMETA(DisplayName = "Dialogue"),
    Tutorial          UMETA(DisplayName = "Tutorial"),
    System            UMETA(DisplayName = "System Messages"),
    Campaign          UMETA(DisplayName = "Campaign"),
    Unit              UMETA(DisplayName = "Unit Descriptions"),
    Building          UMETA(DisplayName = "Building Descriptions"),
    Technology        UMETA(DisplayName = "Technology"),
    Event             UMETA(DisplayName = "Events"),
    Achievement       UMETA(DisplayName = "Achievements")
};

/**
 * Text metadata
 */
USTRUCT()
struct FTextMetadata
{
    GENERATED_BODY()
    
    UPROPERTY()
    FString Key;
    
    UPROPERTY()
    ETextCategory Category;
    
    UPROPERTY()
    FString Context; // Context for translators
    
    UPROPERTY()
    int32 MaxLength; // Maximum allowed length
    
    UPROPERTY()
    bool bAllowTruncation;
    
    FTextMetadata()
        : Category(ETextCategory::UI)
        , MaxLength(0)
        , bAllowTruncation(true)
    {}
};

/**
 * Text manager for advanced text operations
 * Handles formatting, pluralization, and text organization
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UMingRTSTextManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSTextManager();
    
    /** Initialize text manager */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    void InitializeTextManager();
    
    /** Get text from specific namespace and key */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    FString GetText(const FString& Namespace, const FString& Key) const;
    
    /** Get text with category filter */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    FString GetTextByCategory(ETextCategory Category, const FString& Key) const;
    
    /** Format text with numbered arguments {0}, {1}, etc. */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text",
             meta = (AutoCreateRefTerm = "Args"))
    FString FormatText(const FString& Key, const TArray<FString>& Args) const;
    
    /** Format text with named arguments {name}, {value}, etc. */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    FString FormatTextNamed(const FString& Key, 
                           const TMap<FString, FString>& NamedArgs) const;
    
    /** Get pluralized text based on count */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    FString GetPluralText(const FString& KeySingular, 
                         const FString& KeyPlural, 
                         int32 Count) const;
    
    /** Truncate text to specified length with ellipsis */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    FString TruncateText(const FString& Text, int32 MaxLength, 
                        const FString& Ellipsis = TEXT("...")) const;
    
    /** Get all keys in a namespace */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    TArray<FString> GetKeysInNamespace(const FString& Namespace) const;
    
    /** Search text keys by partial match */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    TArray<FString> SearchKeys(const FString& SearchTerm) const;
    
    /** Register text metadata for organization */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    void RegisterTextMetadata(const FString& Key, const FTextMetadata& Metadata);
    
    /** Get text metadata */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    FTextMetadata GetTextMetadata(const FString& Key) const;
    
    /** Check if text exists */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    bool HasText(const FString& Key) const;
    
    /** Get text length in characters (handles multi-byte) */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    int32 GetTextLength(const FString& Text) const;
    
    /** Convert text to uppercase (locale-aware) */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    FString ToUpper(const FString& Text) const;
    
    /** Convert text to lowercase (locale-aware) */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    FString ToLower(const FString& Text) const;
    
    /** Get all categories */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    TArray<ETextCategory> GetAllCategories() const;
    
    /** Get keys in category */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text")
    TArray<FString> GetKeysInCategory(ETextCategory Category) const;
    
    /** Batch get texts for performance */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text",
             meta = (AutoCreateRefTerm = "Keys"))
    TMap<FString, FString> BatchGetTexts(const TArray<FString>& Keys) const;
    
    /** Export missing translations for translators */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text|Development")
    void ExportMissingTranslations(const FString& OutputPath) const;
    
    /** Import translations from file */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Text|Development")
    bool ImportTranslations(const FString& FilePath);

private:
    /** Reference to localization system */
    UPROPERTY()
    TObjectPtr<class UMingRTSLocalizationSystem> LocalizationSystem;
    
    /** Text metadata storage */
    UPROPERTY()
    TMap<FString, FTextMetadata> TextMetadataMap;
    
    /** Category to keys mapping */
    TMap<ETextCategory, TArray<FString>> CategoryKeys;
    
    /** Get raw text from localization system */
    FString GetRawText(const FString& Key) const;
    
    /** Apply formatting arguments */
    FString ApplyFormatting(const FString& Text, 
                           const TArray<FString>& Args) const;
    
    /** Apply named formatting arguments */
    FString ApplyNamedFormatting(const FString& Text,
                                const TMap<FString, FString>& NamedArgs) const;
    
    /** Cache for formatted texts */
    mutable TMap<FString, FString> FormatCache;
    
    /** Clear format cache */
    void ClearFormatCache();
    
    /** Get current locale for string operations */
    FString GetCurrentLocale() const;
};


