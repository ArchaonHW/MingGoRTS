// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Cultural Adapter System for Localization

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCultureAdapter.generated.h"

// Cultural content appropriateness level
UENUM(BlueprintType)
enum class ECulturalAppropriateness: uint8 {
    Appropriate        UMETA(DisplayName = "Appropriate"),
    Caution            UMETA(DisplayName = "Caution"),
    Inappropriate      UMETA(DisplayName = "Inappropriate"),
    Blocked            UMETA(DisplayName = "Blocked"),
    NeedsReview        UMETA(DisplayName = "Needs Review")
};

// Cultural content type
UENUM(BlueprintType)
enum class ECulturalContentType: uint8 {
    Religious          UMETA(DisplayName = "Religious"),
    Political          UMETA(DisplayName = "Political"),
    Historical         UMETA(DisplayName = "Historical"),
    Social             UMETA(DisplayName = "Social"),
    Visual             UMETA(DisplayName = "Visual"),
    Audio              UMETA(DisplayName = "Audio"),
    Text               UMETA(DisplayName = "Text"),
    Symbol             UMETA(DisplayName = "Symbol")
};

// Cultural adaptation rule
USTRUCT(BlueprintType)
struct FCulturalAdaptationRule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule")
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule")
    FString TargetRegion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule")
    ECulturalAppropriateness AppropriatenessLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rule")
    FString AdaptationDescription;
};

// Cultural content info
USTRUCT(BlueprintType)
struct FCulturalContentInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    ECulturalContentType ContentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    TArray<FString> RegionalVariants;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    TMap<FString, ECulturalAppropriateness> RegionalAppropriateness;
};

// Cultural preferences
USTRUCT(BlueprintType)
struct FCulturalPreferences
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preferences")
    TMap<FString, FString> PreferredContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preferences")
    TArray<FString> RestrictedContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preferences")
    bool bEnableCulturalFiltering;
};

/**
 * Cultural Adapter System
 * Manages cultural content adaptation and regional preferences
 */
UCLASS(ClassGroup = (Localization), Blueprintable)
class MINGPERSONAL_API UMingCultureAdapter : public UObject
{
    GENERATED_BODY()

public:
    UMingCultureAdapter();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    void InitializeCulturalAdapter();

    // Cultural content management
    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    bool IsContentAppropriate(const FString& ContentID, const FString& RegionCode) const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    FCulturalContentInfo GetContentInfo(const FString& ContentID) const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    void SetContentAppropriateness(const FString& ContentID, const FString& RegionCode, ECulturalAppropriateness Appropriateness);

    // Regional preferences
    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    FCulturalPreferences GetRegionalPreferences(const FString& RegionCode) const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    void SetRegionalPreferences(const FString& RegionCode, const FCulturalPreferences& Preferences);

    // Adaptation rules
    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    void AddAdaptationRule(const FCulturalAdaptationRule& Rule);

    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    void RemoveAdaptationRule(const FString& RuleID);

    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    TArray<FCulturalAdaptationRule> GetAdaptationRules() const;

    // Content filtering
    UFUNCTION(BlueprintCallable, Category = "Cultural Adapter")
    TArray<FString> GetFilteredContent(const FString& RegionCode, const TArray<FString>& ContentIDs) const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCulturalContentChanged OnCulturalContentChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCulturalRuleViolated OnCulturalRuleViolated;

protected:
    // Cultural content registry
    UPROPERTY()
    TMap<FString, FCulturalContentInfo> CulturalContentRegistry;

    // Regional preferences
    UPROPERTY()
    TMap<FString, FCulturalPreferences> RegionalPreferences;

    // Adaptation rules
    UPROPERTY()
    TArray<FCulturalAdaptationRule> AdaptationRules;

    // Initialize default cultural data
    void InitializeDefaultCulturalData();

    // Load preset rules
    void LoadPresetRules();

    // Find adaptation rule
    const FCulturalAdaptationRule* FindAdaptationRule(const FString& ContentID, const FString& RegionCode) const;
};

// Event delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCulturalContentChanged, const FString&, const FCulturalContentInfo&);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCulturalRuleViolated, const FString&, const FString&, const FCulturalAdaptationRule&);
