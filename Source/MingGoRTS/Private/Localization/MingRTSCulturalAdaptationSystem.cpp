// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.2: Cultural Adaptation System Implementation

#include "Localization/MingRTSCulturalAdaptationSystem.h"
#include "Misc/ConfigCacheIni.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

// Constants for better maintainability
namespace CulturalAdaptationConstants
{
    constexpr int32 CacheTrimInterval = 100;
    constexpr int32 MaxContentCacheSize = 1000;
    constexpr float DefaultDifficultyMultiplier = 1.0f;
    constexpr float DefaultResourceMultiplier = 1.0f;
    constexpr float DefaultAIAggressiveness = 1.0f;
    constexpr float DefaultTutorialPacing = 1.0f;
    constexpr int32 MinAgeForContent = 0;
    constexpr int32 ChildAgeLimit = 7;
    constexpr int32 TeenAgeLimit = 13;
    constexpr int32 AdultAgeLimit = 18;
    constexpr float CacheTrimRatio = 0.25f;
}

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSCultural, Log, All);

UMingRTSCulturalAdaptationSystem::UMingRTSCulturalAdaptationSystem()
    : CurrentRegion(ECulturalRegion::EastAsia)
{
}

void UMingRTSCulturalAdaptationSystem::InitializeCulturalSystem()
{
    UE_LOG(LogMingRTSCultural, Log, TEXT("Initializing Cultural Adaptation System..."));
    
    // Initialize default content
    InitializeDefaultContent();
    
    // Load content variants
    LoadContentVariants();
    
    // Initialize regional parameters
    InitializeRegionalParams();
    
    // Load saved preferences
    LoadCulturalPreferences();
    
    // If no preference saved, detect from system
    if (Preferences.PrimaryRegion == ECulturalRegion::Global)
    {
        ECulturalRegion SystemRegion = DetectRegionFromSystem();
        SetPlayerRegion(SystemRegion);
    }
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Cultural Adaptation System initialized for region: %s"),
        *GetRegionDisplayName(CurrentRegion));
}

void UMingRTSCulturalAdaptationSystem::SetPlayerRegion(ECulturalRegion Region)
{
    if (Region == CurrentRegion)
    {
        return;
    }
    
    ECulturalRegion PreviousRegion = CurrentRegion;
    CurrentRegion = Region;
    Preferences.PrimaryRegion = Region;
    
    // Load content pack for new region
    if (!LoadedContentPacks.Contains(Region) || !LoadedContentPacks[Region])
    {
        LoadRegionalContentPack(Region);
    }
    
    // Broadcast change event
    OnRegionChanged.Broadcast(CurrentRegion);
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Region changed from %s to %s"),
        *GetRegionDisplayName(PreviousRegion),
        *GetRegionDisplayName(CurrentRegion));
}

FString UMingRTSCulturalAdaptationSystem::GetAdaptedContent(const FString& ContentKey, 
    ECulturalRegion Region) const
{
    // Update statistics and access counter
    FScopeLock StatsLock(&ContentCacheLock);
    CacheStats.TotalRequests++;
    CacheAccessCounter++;
    
    // Periodic cache trimming check (every 100 accesses)
    if (CacheAccessCounter >= CulturalAdaptationConstants::CacheTrimInterval)
    {
        StatsLock.Unlock();
        CheckAndTrimCache();
        StatsLock.Lock();
        CacheAccessCounter = 0;
    }
    StatsLock.Unlock();
    
    // Check cache first for performance - thread safe read
    const FString CacheKey = FString::Printf(TEXT("%s_%d"), *ContentKey, static_cast<int32>(Region));
    {
        FScopeLock Lock(&ContentCacheLock);
        const FString* CachedContent = ContentCache.Find(CacheKey);
        if (CachedContent)
        {
            // Cache hit - update statistics and LRU order
            CacheStats.CacheHits++;
            UpdateLRUOrder(CacheKey);
            
            // Recalculate hit rate
            if (CacheStats.TotalRequests > 0)
            {
                CacheStats.HitRate = (static_cast<double>(CacheStats.CacheHits) / static_cast<double>(CacheStats.TotalRequests)) * 100.0;
            }
            
            return *CachedContent;
        }
        // Cache miss
        CacheStats.CacheMisses++;
    }
    
    // Find best variant
    const FCulturalVariant* Variant = FindBestVariant(ContentKey, Region);
    if (Variant && !Variant->AdaptedText.IsEmpty())
    {
        // Cache the result - thread safe write with LRU update
        FScopeLock Lock(&ContentCacheLock);
        ContentCache.Add(CacheKey, Variant->AdaptedText);
        UpdateLRUOrder(CacheKey);
        
        // Update current size statistic
        CacheStats.CurrentSize = ContentCache.Num();
        if (CacheStats.CurrentSize > CacheStats.MaxSize)
        {
            CacheStats.MaxSize = CacheStats.CurrentSize;
        }
        
        return Variant->AdaptedText;
    }
    
    // Fallback to default content
    const FString* Default = DefaultContent.Find(ContentKey);
    if (Default)
    {
        // Cache the result - thread safe write with LRU update
        FScopeLock Lock(&ContentCacheLock);
        ContentCache.Add(CacheKey, *Default);
        UpdateLRUOrder(CacheKey);
        
        CacheStats.CurrentSize = ContentCache.Num();
        if (CacheStats.CurrentSize > CacheStats.MaxSize)
        {
            CacheStats.MaxSize = CacheStats.CurrentSize;
        }
        
        return *Default;
    }
    
    // Last resort: return key and cache it - thread safe write with LRU update
    FScopeLock Lock(&ContentCacheLock);
    ContentCache.Add(CacheKey, ContentKey);
    UpdateLRUOrder(CacheKey);
    
    CacheStats.CurrentSize = ContentCache.Num();
    if (CacheStats.CurrentSize > CacheStats.MaxSize)
    {
        CacheStats.MaxSize = CacheStats.CurrentSize;
    }
    
    return ContentKey;
}

FString UMingRTSCulturalAdaptationSystem::GetAdaptedContentForCurrentRegion(const FString& ContentKey) const
{
    return GetAdaptedContent(ContentKey, CurrentRegion);
}

bool UMingRTSCulturalAdaptationSystem::IsContentAllowed(const FString& ContentKey, 
    int32 UserAge, ECulturalRegion Region) const
{
    const FCulturalVariant* Variant = FindBestVariant(ContentKey, Region);
    if (Variant)
    {
        return CheckContentRating(*Variant, UserAge);
    }
    
    // If no variant found, allow by default
    return true;
}

TArray<FCulturalVariant> UMingRTSCulturalAdaptationSystem::GetAvailableVariants(const FString& ContentKey) const
{
    TArray<FCulturalVariant> Variants;
    
    const TArray<FCulturalVariant>* FoundVariants = ContentVariants.Find(ContentKey);
    if (FoundVariants)
    {
        for (const FCulturalVariant& Variant : *FoundVariants)
        {
            if (Variant.bEnabled)
            {
                Variants.Add(Variant);
            }
        }
    }
    
    return Variants;
}

bool UMingRTSCulturalAdaptationSystem::LoadRegionalContentPack(ECulturalRegion Region)
{
    UE_LOG(LogMingRTSCultural, Log, TEXT("Loading regional content pack for: %s"), 
        *GetRegionDisplayName(Region));
    
    // Mark as loaded
    LoadedContentPacks.Add(Region, true);
    
    // In full implementation, this would:
    // 1. Load regional content from asset bundle
    // 2. Load regional texture variants
    // 3. Load regional audio variants
    // 4. Apply regional gameplay parameters
    
    return true;
}

void UMingRTSCulturalAdaptationSystem::UnloadRegionalContentPack(ECulturalRegion Region)
{
    UE_LOG(LogMingRTSCultural, Log, TEXT("Unloading regional content pack for: %s"),
        *GetRegionDisplayName(Region));
    
    LoadedContentPacks.Add(Region, false);
}

FRegionalGameplayParams UMingRTSCulturalAdaptationSystem::GetRegionalGameplayParams(ECulturalRegion Region) const
{
    const FRegionalGameplayParams* Params = RegionalParams.Find(Region);
    if (Params)
    {
        return *Params;
    }
    
    // Return default params
    return FRegionalGameplayParams();
}

ECulturalRegion UMingRTSCulturalAdaptationSystem::DetectRegionFromSystem() const
{
    // Get system culture
    FCultureRef SystemCulture = FInternationalization::Get().GetDefaultCulture();
    FString Locale = SystemCulture->GetName();
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Detecting region from system locale: %s"), *Locale);
    
    // Map locale to region
    if (Locale.StartsWith(TEXT("zh")))
    {
        return ECulturalRegion::EastAsia;
    }
    else if (Locale.StartsWith(TEXT("ja")))
    {
        return ECulturalRegion::EastAsia;
    }
    else if (Locale.StartsWith(TEXT("ko")))
    {
        return ECulturalRegion::EastAsia;
    }
    else if (Locale.StartsWith(TEXT("en-US")) || Locale.StartsWith(TEXT("en-CA")))
    {
        return ECulturalRegion::NorthAmerica;
    }
    else if (Locale.StartsWith(TEXT("en-GB")) || Locale.StartsWith(TEXT("en-AU")) || 
             Locale.StartsWith(TEXT("en-NZ")))
    {
        return ECulturalRegion::Oceania;
    }
    else if (Locale.StartsWith(TEXT("pl")) || Locale.StartsWith(TEXT("cs")) || 
             Locale.StartsWith(TEXT("hu")) || Locale.StartsWith(TEXT("ro")) ||
             Locale.StartsWith(TEXT("bg")) || Locale.StartsWith(TEXT("hr")) ||
             Locale.StartsWith(TEXT("sr")) || Locale.StartsWith(TEXT("sk")) ||
             Locale.StartsWith(TEXT("sl")) || Locale.StartsWith(TEXT("et")) ||
             Locale.StartsWith(TEXT("lv")) || Locale.StartsWith(TEXT("lt")) ||
             Locale.StartsWith(TEXT("ru")) || Locale.StartsWith(TEXT("uk")) ||
             Locale.StartsWith(TEXT("be")) || Locale.StartsWith(TEXT("mk")) ||
             Locale.StartsWith(TEXT("sq")) || Locale.StartsWith(TEXT("ka")) ||
             Locale.StartsWith(TEXT("hy")) || Locale.StartsWith(TEXT("az")))
    {
        return ECulturalRegion::EasternEurope;
    }
    else if (Locale.StartsWith(TEXT("de")) || Locale.StartsWith(TEXT("fr")) || 
             Locale.StartsWith(TEXT("es")) || Locale.StartsWith(TEXT("it")) ||
             Locale.StartsWith(TEXT("nl")) || Locale.StartsWith(TEXT("pt")) ||
             Locale.StartsWith(TEXT("sv")) || Locale.StartsWith(TEXT("da")) ||
             Locale.StartsWith(TEXT("fi")) || Locale.StartsWith(TEXT("nb")) ||
             Locale.StartsWith(TEXT("is")) || Locale.StartsWith(TEXT("ga")) ||
             Locale.StartsWith(TEXT("mt")) || Locale.StartsWith(TEXT("ca")) ||
             Locale.StartsWith(TEXT("eu")) || Locale.StartsWith(TEXT("gl")) ||
             Locale.StartsWith(TEXT("wa")) || Locale.StartsWith(TEXT("br")) ||
             Locale.StartsWith(TEXT("co")) || Locale.StartsWith(TEXT("oc")) ||
             Locale.StartsWith(TEXT("lb")) || Locale.StartsWith(TEXT("li")) ||
             Locale.StartsWith(TEXT("rm")) || Locale.StartsWith(TEXT("sc")) ||
             Locale.StartsWith(TEXT("vo")) || Locale.StartsWith(TEXT("wo")) ||
             Locale.StartsWith(TEXT("ji")) || Locale.StartsWith(TEXT("yi")) ||
             Locale.StartsWith(TEXT("kw")) || Locale.StartsWith(TEXT("gv")) ||
             Locale.StartsWith(TEXT("tr")) || Locale.StartsWith(TEXT("el")))
    {
        return ECulturalRegion::WesternEurope;
    }
    else if (Locale.StartsWith(TEXT("ar")) || Locale.StartsWith(TEXT("he")) || 
             Locale.StartsWith(TEXT("fa")))
    {
        return ECulturalRegion::MiddleEast;
    }
    else if (Locale.StartsWith(TEXT("hi")) || Locale.StartsWith(TEXT("bn")) || 
             Locale.StartsWith(TEXT("ta")) || Locale.StartsWith(TEXT("te")) ||
             Locale.StartsWith(TEXT("mr")) || Locale.StartsWith(TEXT("ur")))
    {
        return ECulturalRegion::SouthAsia;
    }
    else if (Locale.StartsWith(TEXT("th")) || Locale.StartsWith(TEXT("vi")) || 
             Locale.StartsWith(TEXT("ms")) || Locale.StartsWith(TEXT("id")) ||
             Locale.StartsWith(TEXT("tl")) || Locale.StartsWith(TEXT("my")) ||
             Locale.StartsWith(TEXT("km")) || Locale.StartsWith(TEXT("lo")))
    {
        return ECulturalRegion::SoutheastAsia;
    }
    else if (Locale.StartsWith(TEXT("af")) || Locale.StartsWith(TEXT("sw")) || 
             Locale.StartsWith(TEXT("zu")) || Locale.StartsWith(TEXT("xh")) ||
             Locale.StartsWith(TEXT("am")) || Locale.StartsWith(TEXT("ar-EG")))
    {
        return ECulturalRegion::Africa;
    }
    else if (Locale.StartsWith(TEXT("es-MX")) || Locale.StartsWith(TEXT("es-AR")) || 
             Locale.StartsWith(TEXT("es-CO")) || Locale.StartsWith(TEXT("es-PE")) ||
             Locale.StartsWith(TEXT("es-CL")) || Locale.StartsWith(TEXT("es-VE")) ||
             Locale.StartsWith(TEXT("pt-BR")))
    {
        return ECulturalRegion::LatinAmerica;
    }
    
    // Default
    return ECulturalRegion::EastAsia;
}

void UMingRTSCulturalAdaptationSystem::DetectRegionFromIP()
{
    // Async IP-based detection would be implemented here
    // For now, fallback to system detection
    ECulturalRegion Region = DetectRegionFromSystem();
    OnIPRegionDetected(TEXT(""));
}

void UMingRTSCulturalAdaptationSystem::SetCulturalPreferences(const FRTSCulturalPreferences& NewPreferences)
{
    Preferences = NewPreferences;
    
    // Update current region if changed
    if (Preferences.PrimaryRegion != CurrentRegion)
    {
        SetPlayerRegion(Preferences.PrimaryRegion);
    }
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Cultural preferences updated"));
}

FRTSCulturalPreferences UMingRTSCulturalAdaptationSystem::GetCulturalPreferences() const
{
    return Preferences;
}

void UMingRTSCulturalAdaptationSystem::SaveCulturalPreferences()
{
    // Save to config
    FString RegionString = UEnum::GetValueAsString(Preferences.PrimaryRegion);
    RegionString.RemoveFromStart(TEXT("ECulturalRegion::"));
    
    GConfig->SetString(TEXT("Cultural"), TEXT("PrimaryRegion"), *RegionString, GGameIni);
    GConfig->SetInt(TEXT("Cultural"), TEXT("ContentSensitivityLevel"), Preferences.ContentSensitivityLevel, GGameIni);
    GConfig->SetBool(TEXT("Cultural"), TEXT("EnableCulturalEvents"), Preferences.bEnableCulturalEvents, GGameIni);
    GConfig->SetBool(TEXT("Cultural"), TEXT("PreferHistoricalAccuracy"), Preferences.bPreferHistoricalAccuracy, GGameIni);
    GConfig->SetBool(TEXT("Cultural"), TEXT("EnableRegionalArtVariants"), Preferences.bEnableRegionalArtVariants, GGameIni);
    
    GConfig->Flush(false, GGameIni);
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Cultural preferences saved"));
}

void UMingRTSCulturalAdaptationSystem::LoadCulturalPreferences()
{
    FString RegionString;
    if (GConfig->GetString(TEXT("Cultural"), TEXT("PrimaryRegion"), RegionString, GGameIni))
    {
        UEnum* EnumPtr = StaticEnum<ECulturalRegion>();
        if (EnumPtr)
        {
            int64 Value = EnumPtr->GetValueByNameString(RegionString);
            // Validate enum value is within valid range to prevent undefined behavior
            if (Value != INDEX_NONE && Value >= 0 && Value <= static_cast<int64>(ECulturalRegion::Global))
            {
                Preferences.PrimaryRegion = static_cast<ECulturalRegion>(Value);
                CurrentRegion = Preferences.PrimaryRegion;
            }
            else
            {
                UE_LOG(LogMingRTSCultural, Warning, TEXT("Invalid region value '%s' in config, using default"), *RegionString);
            }
        }
    }
    
    int32 SensitivityLevel;
    if (GConfig->GetInt(TEXT("Cultural"), TEXT("ContentSensitivityLevel"), SensitivityLevel, GGameIni))
    {
        Preferences.ContentSensitivityLevel = SensitivityLevel;
    }
    
    bool bEnabled;
    if (GConfig->GetBool(TEXT("Cultural"), TEXT("EnableCulturalEvents"), bEnabled, GGameIni))
    {
        Preferences.bEnableCulturalEvents = bEnabled;
    }
    
    if (GConfig->GetBool(TEXT("Cultural"), TEXT("PreferHistoricalAccuracy"), bEnabled, GGameIni))
    {
        Preferences.bPreferHistoricalAccuracy = bEnabled;
    }
    
    if (GConfig->GetBool(TEXT("Cultural"), TEXT("EnableRegionalArtVariants"), bEnabled, GGameIni))
    {
        Preferences.bEnableRegionalArtVariants = bEnabled;
    }
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Cultural preferences loaded for region: %s"),
        *GetRegionDisplayName(CurrentRegion));
}

TArray<FString> UMingRTSCulturalAdaptationSystem::GetRegionalHolidays(ECulturalRegion Region, int32 Year) const
{
    TArray<FString> Holidays;
    
    // Add region-specific holidays
    switch (Region)
    {
    case ECulturalRegion::EastAsia:
        Holidays.Add(TEXT("LunarNewYear"));
        Holidays.Add(TEXT("DragonBoatFestival"));
        Holidays.Add(TEXT("MidAutumnFestival"));
        break;
    case ECulturalRegion::MiddleEast:
        Holidays.Add(TEXT("EidAlFitr"));
        Holidays.Add(TEXT("EidAlAdha"));
        break;
    case ECulturalRegion::WesternEurope:
    case ECulturalRegion::NorthAmerica:
        Holidays.Add(TEXT("Christmas"));
        Holidays.Add(TEXT("NewYear"));
        Holidays.Add(TEXT("Thanksgiving"));
        break;
    case ECulturalRegion::SouthAsia:
        Holidays.Add(TEXT("Diwali"));
        Holidays.Add(TEXT("Holi"));
        break;
    default:
        break;
    }
    
    return Holidays;
}

bool UMingRTSCulturalAdaptationSystem::IsRegionalHoliday(ECulturalRegion Region) const
{
    FDateTime Today = FDateTime::Now();
    TArray<FString> Holidays = GetRegionalHolidays(Region, Today.GetYear());
    
    // Check if today matches any holiday (simplified)
    // Full implementation would check actual dates
    return Holidays.Num() > 0;
}

FString UMingRTSCulturalAdaptationSystem::GetRegionDisplayName(ECulturalRegion Region)
{
    UEnum* EnumPtr = StaticEnum<ECulturalRegion>();
    if (EnumPtr)
    {
        return EnumPtr->GetDisplayNameTextByValue((int64)Region).ToString();
    }
    return TEXT("Unknown");
}

TArray<ECulturalRegion> UMingRTSCulturalAdaptationSystem::GetAllRegions()
{
    TArray<ECulturalRegion> Regions;
    
    UEnum* EnumPtr = StaticEnum<ECulturalRegion>();
    if (EnumPtr)
    {
        for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
        {
            ECulturalRegion Region = static_cast<ECulturalRegion>(EnumPtr->GetValueByIndex(i));
            if (Region != ECulturalRegion::Global)
            {
                Regions.Add(Region);
            }
        }
    }
    
    return Regions;
}

// Private methods

void UMingRTSCulturalAdaptationSystem::InitializeDefaultContent()
{
    // Initialize with some default content
    DefaultContent.Add(TEXT("Game.Title"), TEXT("MingGoRTS"));
    DefaultContent.Add(TEXT("Unit.Worker.Name"), TEXT("Worker"));
    DefaultContent.Add(TEXT("Building.HQ.Name"), TEXT("Headquarters"));
}

void UMingRTSCulturalAdaptationSystem::LoadContentVariants()
{
    // In full implementation, load from config/data table
    // For now, leave empty - content would be populated at runtime or from asset
}

void UMingRTSCulturalAdaptationSystem::InitializeRegionalParams()
{
    // Initialize default regional parameters
    const auto AddParams = [this](ECulturalRegion Region, float Difficulty, float Resources, 
                                  float AI, float Tutorial)
    {
        FRegionalGameplayParams Params;
        Params.DifficultyMultiplier = Difficulty;
        Params.ResourceMultiplier = Resources;
        Params.AIAggressiveness = AI;
        Params.TutorialPacing = Tutorial;
        RegionalParams.Add(Region, Params);
    };
    
    AddParams(ECulturalRegion::EastAsia, 
              CulturalAdaptationConstants::DefaultDifficultyMultiplier, 
              CulturalAdaptationConstants::DefaultResourceMultiplier, 
              0.9f, 0.9f);
    AddParams(ECulturalRegion::WesternEurope, 
              CulturalAdaptationConstants::DefaultDifficultyMultiplier, 
              CulturalAdaptationConstants::DefaultResourceMultiplier, 
              CulturalAdaptationConstants::DefaultAIAggressiveness, 
              CulturalAdaptationConstants::DefaultTutorialPacing);
    AddParams(ECulturalRegion::NorthAmerica, 0.9f, 1.1f, 
              CulturalAdaptationConstants::DefaultAIAggressiveness, 1.1f);
    AddParams(ECulturalRegion::LatinAmerica, 1.1f, 0.9f, 1.1f, 0.9f);
    AddParams(ECulturalRegion::SoutheastAsia, 
              CulturalAdaptationConstants::DefaultDifficultyMultiplier, 
              CulturalAdaptationConstants::DefaultResourceMultiplier, 
              CulturalAdaptationConstants::DefaultAIAggressiveness, 
              CulturalAdptationConstants::DefaultTutorialPacing);
    AddParams(ECulturalRegion::MiddleEast, 1.1f, 0.9f, 1.2f, 0.8f);
}

const FCulturalVariant* UMingRTSCulturalAdaptationSystem::FindBestVariant(
    const FString& ContentKey, ECulturalRegion Region) const
{
    const TArray<FCulturalVariant>* Variants = ContentVariants.Find(ContentKey);
    if (!Variants)
    {
        return nullptr;
    }
    
    // Use const reference for better performance
    for (const FCulturalVariant& Variant : *Variants)
    {
        if (Variant.Region == Region && Variant.bEnabled)
        {
            return &Variant;
        }
    }
    
    // Fallback to global/default
    for (const FCulturalVariant& Variant : *Variants)
    {
        if (Variant.Region == ECulturalRegion::Global && Variant.bEnabled)
        {
            return &Variant;
        }
    }
    
    return nullptr;
}

bool UMingRTSCulturalAdaptationSystem::CheckContentRating(const FCulturalVariant& Variant, 
    int32 UserAge) const
{
    // Simple age-based content rating
    // 0 = no restrictions
    // 1 = 7+
    // 2 = 13+
    // 3 = 18+
    
    switch (Variant.SensitivityLevel)
    {
    case CulturalAdaptationConstants::MinAgeForContent:
        return true;
    case 1:
        return UserAge >= CulturalAdaptationConstants::ChildAgeLimit;
    case 2:
        return UserAge >= CulturalAdaptationConstants::TeenAgeLimit;
    case 3:
        return UserAge >= CulturalAdaptationConstants::AdultAgeLimit;
    default:
        return true;
    }
}

void UMingRTSCulturalAdaptationSystem::OnIPRegionDetected(const FString& CountryCode)
{
    // Parse country code and map to region
    ECulturalRegion DetectedRegion = DetectRegionFromSystem(); // Fallback
    
    // Broadcast detection completed
    OnRegionDetectionCompleted.Broadcast(DetectedRegion);
}

void UMingRTSCulturalAdaptationSystem::ClearContentCache()
{
    FScopeLock Lock(&ContentCacheLock);
    int32 PreviousSize = ContentCache.Num();
    ContentCache.Empty();
    LRUCacheOrder.Empty();
    
    // Reset statistics
    CacheStats.CurrentSize = 0;
    CacheStats.HitRate = 0.0;
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Content cache cleared. Previous size: %d, Current size: %d"), 
        PreviousSize, ContentCache.Num());
}

void UMingRTSCulturalAdaptationSystem::UpdateLRUOrder(const FString& CacheKey) const
{
    // Move the accessed key to the end (most recently used)
    LRUCacheOrder.Remove(CacheKey);
    LRUCacheOrder.Add(CacheKey);
}

void UMingRTSCulturalAdaptationSystem::CheckAndTrimCache()
{
    FScopeLock Lock(&ContentCacheLock);
    if (ContentCache.Num() <= CulturalAdaptationConstants::MaxContentCacheSize)
    {
        return;
    }
    
    // Calculate how many entries to remove (remove oldest 25%)
    const int32 EntriesToRemove = FMath::Max(1, static_cast<int32>(ContentCache.Num() * CulturalAdaptationConstants::CacheTrimRatio));
    
    // Remove oldest entries based on LRU order (from the beginning of array)
    int32 RemovedCount = 0;
    for (int32 i = 0; i < LRUCacheOrder.Num() && RemovedCount < EntriesToRemove; ++i)
    {
        const FString& Key = LRUCacheOrder[i];
        if (ContentCache.Remove(Key) > 0)
        {
            RemovedCount++;
        }
    }
    
    // Rebuild LRU order to remove deleted keys
    TArray<FString> NewLRUOrder;
    for (const FString& Key : LRUCacheOrder)
    {
        if (ContentCache.Contains(Key))
        {
            NewLRUOrder.Add(Key);
        }
    }
    LRUCacheOrder = MoveTemp(NewLRUOrder);
    
    // Update statistics
    CacheStats.CurrentSize = ContentCache.Num();
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Cache trimmed using LRU: removed %d oldest entries, new size: %d, max size was: %d"),
        RemovedCount, ContentCache.Num(), CacheStats.MaxSize);
}
