// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.2: Cultural Adaptation System

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Map.h"
#include "MingRTSCulturalAdaptationSystem.generated.h"

/**
 * Cultural region identifier
 */
UENUM(BlueprintType)
enum class ECulturalRegion : uint8
{
    EastAsia            UMETA(DisplayName = "East Asia"),
    SoutheastAsia       UMETA(DisplayName = "Southeast Asia"),
    SouthAsia           UMETA(DisplayName = "South Asia"),
    MiddleEast          UMETA(DisplayName = "Middle East"),
    WesternEurope       UMETA(DisplayName = "Western Europe"),
    EasternEurope       UMETA(DisplayName = "Eastern Europe"),
    NorthAmerica        UMETA(DisplayName = "North America"),
    LatinAmerica        UMETA(DisplayName = "Latin America"),
    Oceania             UMETA(DisplayName = "Oceania"),
    Africa              UMETA(DisplayName = "Africa"),
    Global              UMETA(DisplayName = "Global/Default")
};

/**
 * Cultural content variant
 */
USTRUCT(BlueprintType)
struct FCulturalVariant
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ECulturalRegion Region;
    
    UPROPERTY(BlueprintReadOnly)
    FString ContentKey;
    
    UPROPERTY(BlueprintReadOnly)
    FString AdaptedText;
    
    UPROPERTY(BlueprintReadOnly)
    FString AlternateImagePath;
    
    UPROPERTY(BlueprintReadOnly)
    FString AlternateAudioPath;
    
    UPROPERTY(BlueprintReadOnly)
    bool bRequiresApproval;
    
    UPROPERTY(BlueprintReadOnly)
    int32 SensitivityLevel; // 0=none, 1=low, 2=medium, 3=high
    
    UPROPERTY(BlueprintReadOnly)
    bool bEnabled;
    
    FCulturalVariant()
        : Region(ECulturalRegion::Global)
        , bRequiresApproval(false)
        , SensitivityLevel(0)
        , bEnabled(true)
    {}
};

/**
 * Regional gameplay parameters
 */
USTRUCT(BlueprintType)
struct FRegionalGameplayParams
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    float DifficultyMultiplier;
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceMultiplier;
    
    UPROPERTY(BlueprintReadOnly)
    float AIAggressiveness;
    
    UPROPERTY(BlueprintReadOnly)
    float TutorialPacing;
    
    UPROPERTY(BlueprintReadOnly)
    bool bEnableRegionalEvents;
    
    UPROPERTY(BlueprintReadOnly)
    FString PreferredControlScheme;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RegionalUnitVariants;
    
    FRegionalGameplayParams()
        : DifficultyMultiplier(1.0f)
        , ResourceMultiplier(1.0f)
        , AIAggressiveness(1.0f)
        , TutorialPacing(1.0f)
        , bEnableRegionalEvents(true)
    {}
};

/**
 * Player cultural preferences
 */
USTRUCT(BlueprintType)
struct FRTSCulturalPreferences
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    ECulturalRegion PrimaryRegion;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<ECulturalRegion> SecondaryRegions;
    
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
        , ContentSensitivityLevel(0)
        , bEnableCulturalEvents(true)
        , bPreferHistoricalAccuracy(true)
        , bEnableRegionalArtVariants(true)
    {}
};

/**
 * Cultural adaptation system for region-specific content
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UMingRTSCulturalAdaptationSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSCulturalAdaptationSystem();
    
    /** Initialize cultural adaptation system */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    void InitializeCulturalSystem();
    
    /** Set player's primary cultural region */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    void SetPlayerRegion(ECulturalRegion Region);
    
    /** Get current region */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    ECulturalRegion GetCurrentRegion() const { return CurrentRegion; }
    
    /** Get adapted content for a key and region */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    FString GetAdaptedContent(const FString& ContentKey, ECulturalRegion Region) const;
    
    /** Get adapted content for current region */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    FString GetAdaptedContentForCurrentRegion(const FString& ContentKey) const;
    
    /** Check if content is allowed for user */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    bool IsContentAllowed(const FString& ContentKey, int32 UserAge, ECulturalRegion Region) const;
    
    /** Get all available variants for a content key */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    TArray<FCulturalVariant> GetAvailableVariants(const FString& ContentKey) const;
    
    /** Load regional content pack */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    bool LoadRegionalContentPack(ECulturalRegion Region);
    
    /** Unload regional content pack */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    void UnloadRegionalContentPack(ECulturalRegion Region);
    
    /** Get regional gameplay parameters */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    FRegionalGameplayParams GetRegionalGameplayParams(ECulturalRegion Region) const;
    
    /** Detect region from system settings */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    ECulturalRegion DetectRegionFromSystem() const;
    
    /** Detect region from IP (async) */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    void DetectRegionFromIP();
    
    /** Set cultural preferences */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    void SetCulturalPreferences(const FRTSCulturalPreferences& NewPreferences);
    
    /** Get cultural preferences */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    FRTSCulturalPreferences GetCulturalPreferences() const;
    
    /** Save preferences to settings */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    void SaveCulturalPreferences();
    
    /** Load preferences from settings */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    void LoadCulturalPreferences();
    
    /** Get regional holiday events */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    TArray<FString> GetRegionalHolidays(ECulturalRegion Region, int32 Year) const;
    
    /** Check if today is a regional holiday */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    bool IsRegionalHoliday(ECulturalRegion Region) const;
    
    /** Get region display name */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Cultural")
    static FString GetRegionDisplayName(ECulturalRegion Region);
    
    /** Get all available regions */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Cultural")
    static TArray<ECulturalRegion> GetAllRegions();
    
    /** Clear content cache for memory management */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Cultural")
    void ClearContentCache();
    
    /** Get cache statistics */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Cultural")
    int32 GetCacheSize() const { return ContentCache.Num(); }
    
    /** Event: Region changed */
    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Cultural|Events")
    FOnCulturalRegionChanged OnRegionChanged;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCulturalRegionChanged, 
        ECulturalRegion, NewRegion);
    
    /** Event: Region detection completed */
    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Cultural|Events")
    FOnRegionDetectionCompleted OnRegionDetectionCompleted;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionDetectionCompleted,
        ECulturalRegion, DetectedRegion);

private:
    /** Current active region */
    UPROPERTY()
    ECulturalRegion CurrentRegion;
    
    /** Player preferences */
    UPROPERTY()
    FRTSCulturalPreferences Preferences;
    
    /** Content variants database - optimized with inline storage */
    TMap<FString, TArray<FCulturalVariant>> ContentVariants;
    
    /** Regional gameplay parameters cache */
    UPROPERTY()
    TMap<ECulturalRegion, FRegionalGameplayParams> RegionalParams;
    
    /** Regional content packs */
    UPROPERTY()
    TMap<ECulturalRegion, bool> LoadedContentPacks;
    
    /** Default fallback content */
    UPROPERTY()
    TMap<FString, FString> DefaultContent;
    
    /** Cache for frequently accessed content - thread safe with lock */
    mutable FCriticalSection ContentCacheLock;
    mutable TMap<FString, FString> ContentCache;
    
    /** Maximum cache size to prevent memory leaks (1000 entries) */
    static constexpr int32 MAX_CONTENT_CACHE_SIZE = 1000;
    
    /** Initialize default content */
    void InitializeDefaultContent();
    
    /** Load content variants from config */
    void LoadContentVariants();
    
    /** Initialize regional parameters */
    void InitializeRegionalParams();
    
    /** Check and trim cache if it exceeds size limit */
    void CheckAndTrimCache();
    
    /** Get appropriate variant for region */
    const FCulturalVariant* FindBestVariant(const FString& ContentKey, 
        ECulturalRegion Region) const;
    
    /** Check content rating compliance */
    bool CheckContentRating(const FCulturalVariant& Variant, int32 UserAge) const;
    
    /** Handle IP detection callback */
    void OnIPRegionDetected(const FString& CountryCode);
};
