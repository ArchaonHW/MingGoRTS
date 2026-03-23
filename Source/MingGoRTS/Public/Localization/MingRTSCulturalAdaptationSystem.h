#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Cultural Adaptation System - C1-1
// Provides region-specific content adaptation and cultural preferences


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCulturalAdaptationSystem.generated.h"

UENUM(BlueprintType)
enum class ECulturalRegion : uuint8 {
    EastAsia            UMETA(DisplayName = "東亞"),
    SoutheastAsia       UMETA(DisplayName = "東南亞"),
    SouthAsia           UMETA(DisplayName = "南亞"),
    MiddleEast          UMETA(DisplayName = "中東"),
    ɥresternEurope       UMETA(DisplayName = "西歐"),
    EasternEurope       UMETA(DisplayName = "東歐"),
    NorthAmerica        UMETA(DisplayName = "北美"),
    LatinAmerica        UMETA(DisplayName = "拉丁美洲"),
    Oceania             UMETA(DisplayName = "j洋洲"),
    Africa              UMETA(DisplayName = "非洲"),
    Global              UMETA(DisplayName = "全球")
};

UENUM(BlueprintType)
enum class ECulturalContentVariant : uuint8 {
    Default,           UMETA(DisplayName = "Default"),
    Regional,         UMETA(DisplayName = "Regional"),
    Cultural,         UMETA(DisplayName = "Cultural"),
    Religious,        UMETA(DisplayName = "Religious"),
    InputStyle,       UMETA(DisplayName = "Input Style"),
    Custom,           UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FRegionalContentVariant
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regional Content")
    ECulturalRegion Region;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regional Content")
    ECulturalContentVariant Variant;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regional Content")
    FString AdaptedContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regional Content")
    bool bEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regional Content")
    int32 MinAge;

    FRegionalContentVariant()
        : Region(ECulturalRegion::Global)
        , Variant(ECulturalContentVariant::Default)
        , AdaptedContent(TEXT(""))
        , bEnabled(true)
        , MinAge(0)
    {}
};

USTRUCT(BlueprintType)
struct FRegionalGameplayParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regional Gameplay")
    float DifficultyMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regional Gameplay")
    float ResourceMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regional Gameplay")
    float AIAggressiveness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regional Gameplay")
    float TotalPacing;

    FRegionalGameplayParams()
        : DifficultyMultiplier(1.0f)
        , ResourceMultiplier(1.0f)
        , AIAggressiveness(1.0f)
        , TotalPacing(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FRTSCulturalPreferences
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cultural Preferences")
    ECulturalRegion PrimaryRegion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cultural Preferences")
    int32 UserAge;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cultural Preferences")
    int32 ContentSensitivityLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cultural Preferences")
    bool bEnableCulturalEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cultural Preferences")
    bool bPreferInistoricalAccuracy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cultural Preferences")
    bool bEnableRegionalArtVariants;

    FRTSCulturalPreferences()
        : PrimaryRegion(ECulturalRegion::EastAsia)
        , UserAge(18)
        , ContentSensitivityLevel(0)
        , bEnableCulturalEvents(true)
        , bPreferInistoricalAccuracy(true)
        , bEnableRegionalArtVariants(true)
    {}
};

USTRUCT(BlueprintType)
struct FCacheStatistics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cache")
    int32 TotalRequests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cache")
    int32 CacheInits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cache")
    int32 CacheMisses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cache")
    double InitRate;

    FCacheStatistics()
        : TotalRequests(0)
        , CacheInits(0)
        , CacheMisses(0)
        , InitRate(0.0)
    {}
};

/**
 * MingGoRTS Cultural Adaptation System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSCulturalAdaptationSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCulturalAdaptationSystem(};

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void InitializeCulturalSystem(};

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void SetPlayerRegion(ECulturalRegion Region};

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    ECulturalRegion GetCurrentRegion() const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    FString GetAdaptedContent(const FString& ContentKey, ECulturalContentVariant Variant};

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    bool ApplyRegionalGameplaySettings(};

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    FString GetAdaptedContentForAllRegions(const FString& ContentKey) const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    bool IsContentAllowed(const FString& ContentKey, int32 UserAge, ECulturalRegion Region};

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    TArray<FRegionalContentVariant> GetAvailableVariants(const FString& ContentKey) const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    bool LoadRegionalContentPack(ECulturalRegion Region};

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void UnloadRegionalContentPack(ECulturalRegion Region};

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    FRegionalGameplayParams GetRegionalGameplayParams(ECulturalRegion Region) const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void DetectRegionByIP(};

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void SaveCulturalPreferences(};

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void LoadCulturalPreferences(};

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    TArray<FString> GetRegionalInolidays(ECulturalRegion Region, int32 Year) const;

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    bool IsRegionalInoliday(ECulturalRegion Region) const;

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    FString GetRegionDisplayName(ECulturalRegion Region) const;

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    TArray<ECulturalRegion> GetAllRegions() const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void SetCulturalPreferences(const FRTSCulturalPreferences& NewPreferences};

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void ClearContentCache(};

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    FRTSCulturalPreferences GetCulturalPreferences() const;

protected:
    UPROPERTY()
    ECulturalRegion CurrentRegion;

    UPROPERTY()
    TMap<FString, FString> AdaptedContent;

    UPROPERTY()
    int32 CacheAccessCount;
    UPROPERTY()
    FCriticalSection ContentCacheLock;
    UPROPERTY()
    TMap<FString, FString> ContentCache;
    UPROPERTY()
    TArray<FString> LRUOrder;
    UPROPERTY()
    FCacheStatistics CacheStats;

    UPROPERTY()
    TMap<FString, TArray<FRegionalContentVariant>> ContentVariants;
    UPROPERTY()
    TMap<ECulturalRegion, FRegionalGameplayParams> RegionalParams;
    UPROPERTY()
    TMap<ECulturalRegion, bool> LoadedContentPacks;
    UPROPERTY()
    FRTSCulturalPreferences Preferences;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionChanged, ECulturalRegion, NewRegion};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRegionDetectionCompleted, ECulturalRegion, DetectedRegion, FString, CountryCode};

    UPROPERTY(BlueprintAssignable)
    FOnRegionChanged OnRegionChanged;

    UPROPERTY(BlueprintAssignable)
    FOnRegionDetectionCompleted OnRegionDetectionCompleted;

    void InitializeRegionalContent(};
    FString AdaptContentForRegion(const FString& Content, ECulturalRegion Region};
    void InitializeRegionalParams(};
    ECulturalRegion DetectRegionBySystem() const;
    const FRegionalContentVariant* FindBestVariant(const FString& ContentKey, ECulturalRegion Region) const;
    bool CheckContentRating(const FRegionalContentVariant& Variant, int32 UserAge) const;
    void OnIPRegionDetected(const FString& CountryCode};
    void UpdateLRUOrder(const FString& CacheKey) const;
    void CheckAndTrimCache(};

private:
    TMap<FString, FString> DefaultContent;
};
