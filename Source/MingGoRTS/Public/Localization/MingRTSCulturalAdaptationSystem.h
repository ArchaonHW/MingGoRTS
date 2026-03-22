#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCulturalAdaptationSystem.generated.h"

UENUM(BlueprintType)
enum class ECulturalRegion : uint8
{
    EastAsia, UMETA(DisplayName = "East Asia"),
    SoutheastAsia, UMETA(DisplayName = "Southeast Asia"),
    SouthAsia, UMETA(DisplayName = "South Asia"),
    MiddleEast, UMETA(DisplayName = "Middle East"),
    WesternEurope, UMETA(DisplayName = "Western Europe"),
    EasternEurope, UMETA(DisplayName = "Eastern Europe"),
    NorthAmerica, UMETA(DisplayName = "North America"),
    LatinAmerica, UMETA(DisplayName = "Latin America"),
    Oceania, UMETA(DisplayName = "Oceania"),
    Africa, UMETA(DisplayName = "Africa"),
    Global, UMETA(DisplayName = "Global")
};

UENUM(BlueprintType)
enum class EContentVariant : uint8
{
    Default, UMETA(DisplayName = "Default"),
    Regional, UMETA(DisplayName = "Regional"),
    Cultural, UMETA(DisplayName = "Cultural"),
    Religious, UMETA(DisplayName = "Religious"),
    Historical, UMETA(DisplayName = "Historical"),
    Custom, UMETA(DisplayName = "Custom")
};

// Content variant structure for regional adaptations
USTRUCT(BlueprintType)
struct FCulturalVariant
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    ECulturalRegion Region;

    UPROPERTY(BlueprintReadWrite)
    FString AdaptedContent;

    UPROPERTY(BlueprintReadWrite)
    bool bEnabled;

    UPROPERTY(BlueprintReadWrite)
    int32 MinAge;

    FCulturalVariant()
        : Region(ECulturalRegion::Global)
        , AdaptedContent(TEXT(""))
        , bEnabled(true)
        , MinAge(0)
    {}
};

// Regional gameplay parameters structure
USTRUCT(BlueprintType)
struct FRegionalGameplayParams
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    float DifficultyMultiplier;

    UPROPERTY(BlueprintReadWrite)
    float ResourceMultiplier;

    UPROPERTY(BlueprintReadWrite)
    float AIAggressiveness;

    UPROPERTY(BlueprintReadWrite)
    float TutorialPacing;

    FRegionalGameplayParams()
        : DifficultyMultiplier(1.0f)
        , ResourceMultiplier(1.0f)
        , AIAggressiveness(1.0f)
        , TutorialPacing(1.0f)
    {}
};

// Cultural preferences structure
USTRUCT(BlueprintType)
struct FRTSCulturalPreferences
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    ECulturalRegion PrimaryRegion;

    UPROPERTY(BlueprintReadWrite)
    int32 UserAge;

    UPROPERTY(BlueprintReadWrite)
    int32 ContentSensitivityLevel;

    UPROPERTY(BlueprintReadWrite)
    bool bEnableCulturalEvents;

    UPROPERTY(BlueprintReadWrite)
    bool bPreferHistoricalAccuracy;

    UPROPERTY(BlueprintReadWrite)
    bool bEnableRegionalArtVariants;

    FRTSCulturalPreferences()
        : PrimaryRegion(ECulturalRegion::EastAsia)
        , UserAge(18)
        , ContentSensitivityLevel(0)
        , bEnableCulturalEvents(true)
        , bPreferHistoricalAccuracy(true)
        , bEnableRegionalArtVariants(true)
    {}
};

// Cache statistics structure
USTRUCT(BlueprintType)
struct FCacheStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 TotalRequests;

    UPROPERTY(BlueprintReadWrite)
    int32 CacheHits;

    UPROPERTY(BlueprintReadWrite)
    int32 CacheMisses;

    UPROPERTY(BlueprintReadWrite)
    double HitRate;

    FCacheStatistics()
        : TotalRequests(0)
        , CacheHits(0)
        , CacheMisses(0)
        , HitRate(0.0)
    {}
};

/**
 * MingGoRTS Cultural Adaptation System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCulturalAdaptationSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCulturalAdaptationSystem();

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void InitializeCulturalSystem();

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void SetPlayerRegion(ECulturalRegion Region);

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    ECulturalRegion GetCurrentRegion() const;

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    FString GetAdaptedContent(const FString& ContentKey, EContentVariant Variant);

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    bool ApplyRegionalGameplaySettings();

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    FString GetAdaptedContentForCurrentRegion(const FString& ContentKey);

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    bool IsContentAllowed(const FString& ContentKey, int32 UserAge, ECulturalRegion Region);

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    TArray<FCulturalVariant> GetAvailableVariants(const FString& ContentKey);

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    bool LoadRegionalContentPack(ECulturalRegion Region);

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void UnloadRegionalContentPack(ECulturalRegion Region);

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    FRegionalGameplayParams GetRegionalGameplayParams(ECulturalRegion Region);

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void DetectRegionFromIP();

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void SaveCulturalPreferences();

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void LoadCulturalPreferences();

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    TArray<FString> GetRegionalHolidays(ECulturalRegion Region, int32 Year);

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    bool IsRegionalHoliday(ECulturalRegion Region);

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    FString GetRegionDisplayName(ECulturalRegion Region);

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    TArray<ECulturalRegion> GetAllRegions();

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void SetCulturalPreferences(const FRTSCulturalPreferences& NewPreferences);

    UFUNCTION(BlueprintCallable, Category = "Cultural Adaptation")
    void ClearContentCache();

    UFUNCTION(BlueprintPure, Category = "Cultural Adaptation")
    FRTSCulturalPreferences GetCulturalPreferences() const;

protected:
    UPROPERTY()
    ECulturalRegion CurrentRegion;

    UPROPERTY()
    TMap<FString, FString> AdaptedContent;

    // Cache management
    mutable int32 CacheAccessCounter;
    mutable FCriticalSection ContentCacheLock;
    mutable TMap<FString, FString> ContentCache;
    mutable TArray<FString> LRUOrder;
    mutable FCacheStatistics CacheStats;

    // Content and regional data
    TMap<FString, TArray<FCulturalVariant>> ContentVariants;
    TMap<ECulturalRegion, FRegionalGameplayParams> RegionalParams;
    TMap<ECulturalRegion, bool> LoadedContentPacks;
    FRTSCulturalPreferences Preferences;

    // Event delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionChanged, ECulturalRegion, NewRegion);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionDetectionCompleted, ECulturalRegion, DetectedRegion);
    
    UPROPERTY(BlueprintAssignable)
    FOnRegionChanged OnRegionChanged;
    
    UPROPERTY(BlueprintAssignable)
    FOnRegionDetectionCompleted OnRegionDetectionCompleted;

    void InitializeRegionalContent();
    FString AdaptContentForRegion(const FString& Content, ECulturalRegion Region);

private:
    void InitializeDefaultContent();
    void LoadContentVariants();
    void InitializeRegionalParams();
    ECulturalRegion DetectRegionFromSystem() const;
    const FCulturalVariant* FindBestVariant(const FString& ContentKey, ECulturalRegion Region) const;
    bool CheckContentRating(const FCulturalVariant& Variant, int32 UserAge) const;
    void OnIPRegionDetected(const FString& CountryCode);
    void UpdateLRUOrder(const FString& CacheKey) const;
    void CheckAndTrimCache();

    // Helper data
    TMap<FString, FString> DefaultContent;
};
