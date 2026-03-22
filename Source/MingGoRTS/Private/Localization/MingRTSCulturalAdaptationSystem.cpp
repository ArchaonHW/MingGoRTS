// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.2: Cultural Adaptation System Implementation
// 
// This system manages cultural adaptation and localization for different regions,
// providing content variants, regional gameplay parameters, and cultural preferences.
// It supports 11 cultural regions with automatic detection and manual configuration.

#include "Localization/MingRTSCulturalAdaptationSystem.h"
#include "Misc/ConfigCacheIni.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

// Constants namespace for better maintainability and to avoid magic numbers
// All configuration values are centralized here for easy modification
namespace CulturalAdaptationConstants
{
    // Cache management constants
    constexpr int32 CacheTrimInterval = 100;        // Check cache size every N accesses
    constexpr int32 MaxContentCacheSize = 1000;     // Maximum number of cached entries
    constexpr float CacheTrimRatio = 0.25f;         // Remove 25% of oldest entries when full
    
    // Default gameplay parameters for regional adaptation
    constexpr float DefaultDifficultyMultiplier = 1.0f;   // Base difficulty multiplier
    constexpr float DefaultResourceMultiplier = 1.0f;     // Base resource multiplier
    constexpr float DefaultAIAggressiveness = 1.0f;       // Base AI aggressiveness
    constexpr float DefaultTutorialPacing = 1.0f;        // Base tutorial pacing
    
    // Content rating constants for age-based content filtering
    constexpr int32 MinAgeForContent = 0;     // Minimum age for unrestricted content
    constexpr int32 ChildAgeLimit = 7;        // Age limit for child-appropriate content
    constexpr int32 TeenAgeLimit = 13;        // Age limit for teen-appropriate content
    constexpr int32 AdultAgeLimit = 18;        // Age limit for adult-only content
}

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSCultural, Log, All);

// Constructor: Initialize with default East Asian region
UMingRTSCulturalAdaptationSystem::UMingRTSCulturalAdaptationSystem()
    : CurrentRegion(ECulturalRegion::EastAsia)
{
    // Initialize member variables to their default states
    CacheAccessCounter = 0;
}

// Initialize the cultural adaptation system with all necessary components
// This method sets up the foundation for cultural content management
void UMingRTSCulturalAdaptationSystem::InitializeCulturalSystem()
{
    UE_LOG(LogMingRTSCultural, Log, TEXT("Initializing Cultural Adaptation System..."));
    
    // Initialize core system components in order
    InitializeDefaultContent();      // Set up base content that all regions share
    LoadContentVariants();          // Load regional content variants from data files
    InitializeRegionalParams();      // Set up region-specific gameplay parameters
    LoadCulturalPreferences();       // Load user's saved cultural preferences
    
    // Auto-detect region if no user preference is saved
    // This provides a good default experience for first-time users
    if (Preferences.PrimaryRegion == ECulturalRegion::Global)
    {
        ECulturalRegion SystemRegion = DetectRegionFromSystem();
        SetPlayerRegion(SystemRegion);
    }
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Cultural Adaptation System initialized for region: %s"),
        *GetRegionDisplayName(CurrentRegion));
}

// Set the player's cultural region and update all related systems
// This triggers content loading, preference updates, and event broadcasting
void UMingRTSCulturalAdaptationSystem::SetPlayerRegion(ECulturalRegion Region)
{
    // Early return if the region is already set to avoid unnecessary work
    if (Region == CurrentRegion)
    {
        return;
    }
    
    // Store the previous region for logging and event purposes
    ECulturalRegion PreviousRegion = CurrentRegion;
    CurrentRegion = Region;
    Preferences.PrimaryRegion = Region;
    
    // Load regional content pack if not already loaded
    // This ensures region-specific assets are available
    if (!LoadedContentPacks.Contains(Region) || !LoadedContentPacks[Region])
    {
        LoadRegionalContentPack(Region);
    }
    
    // Notify all listeners that the region has changed
    // This allows UI and other systems to update accordingly
    OnRegionChanged.Broadcast(CurrentRegion);
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Region changed from %s to %s"),
        *GetRegionDisplayName(PreviousRegion),
        *GetRegionDisplayName(CurrentRegion));
}

// Get culturally adapted content for a specific content key and region
// This method implements thread-safe caching for performance optimization
FString UMingRTSCulturalAdaptationSystem::GetAdaptedContent(const FString& ContentKey, 
    ECulturalRegion Region) const
{
    // Update cache statistics and access counter for performance monitoring
    FScopeLock StatsLock(&ContentCacheLock);
    CacheStats.TotalRequests++;
    CacheAccessCounter++;
    
    // Periodic cache maintenance: Check if cache needs trimming
    // This prevents memory bloat while maintaining good performance
    if (CacheAccessCounter >= CulturalAdaptationConstants::CacheTrimInterval)
    {
        StatsLock.Unlock();  // Unlock before calling CheckAndTrimCache to avoid deadlock
        CheckAndTrimCache();
        StatsLock.Lock();    // Re-lock to update counter
        CacheAccessCounter = 0;
    }
    StatsLock.Unlock();
    
    // Generate cache key for this specific content and region combination
    const FString CacheKey = FString::Printf(TEXT("%s_%d"), *ContentKey, static_cast<int32>(Region));
    
    // First, check if content is already cached (fast path)
    {
        FScopeLock Lock(&ContentCacheLock);
        const FString* CachedContent = ContentCache.Find(CacheKey);
        if (CachedContent)
        {
            // Cache hit: Update statistics and LRU order
            CacheStats.CacheHits++;
            UpdateLRUOrder(CacheKey);
            
            // Recalculate hit rate for performance monitoring
            if (CacheStats.TotalRequests > 0)
            {
                CacheStats.HitRate = (static_cast<double>(CacheStats.CacheHits) / static_cast<double>(CacheStats.TotalRequests)) * 100.0;
            }
            
            return *CachedContent;  // Return cached content immediately
        }
        // Cache miss: Update statistics
        CacheStats.CacheMisses++;
    }
    
    // Cache miss: Find the best content variant for this region
    const FCulturalVariant* Variant = FindBestVariant(ContentKey, Region);
    if (!Variant)
    {
        // No variant found: Return default content or empty string
        UE_LOG(LogMingRTSCultural, Warning, TEXT("No cultural variant found for content key: %s"), *ContentKey);
        return TEXT("");
    }
    
    // Check if content is appropriate for the user's age
    // This implements content rating and parental controls
    if (!CheckContentRating(*Variant, Preferences.UserAge))
    {
        UE_LOG(LogMingRTSCultural, Warning, TEXT("Content not appropriate for user age %d: %s"), 
            Preferences.UserAge, *ContentKey);
        return TEXT("");
    }
    
    // Cache the content for future use (thread-safe)
    {
        FScopeLock Lock(&ContentCacheLock);
        ContentCache.Add(CacheKey, Variant->AdaptedContent);
        UpdateLRUOrder(CacheKey);
    }
    
    return Variant->AdaptedContent;
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

// Detect the user's cultural region from system locale settings
// This provides automatic region detection for first-time users
ECulturalRegion UMingRTSCulturalAdaptationSystem::DetectRegionFromSystem() const
{
    // Get the current system locale from UE's internationalization system
    const FString SystemLocale = FInternationalization::Get().GetCurrentLocale().GetName();
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Detected system locale: %s"), *SystemLocale);
    
    // Map common locale codes to cultural regions
    // This handles the most common locales explicitly
    if (SystemLocale.StartsWith(TEXT("zh")))          // Chinese locales
    {
        return ECulturalRegion::EastAsia;
    }
    else if (SystemLocale.StartsWith(TEXT("ja")))      // Japanese
    {
        return ECulturalRegion::EastAsia;
    }
    else if (SystemLocale.StartsWith(TEXT("ko")))      // Korean
    {
        return ECulturalRegion::EastAsia;
    }
    else if (SystemLocale.StartsWith(TEXT("en")))      // English (default to North America)
    {
        return ECulturalRegion::NorthAmerica;
    }
    else if (SystemLocale.StartsWith(TEXT("fr")))      // French
    {
        return ECulturalRegion::WesternEurope;
    }
    else if (SystemLocale.StartsWith(TEXT("de")))      // German
    {
        return ECulturalRegion::WesternEurope;
    }
    else if (SystemLocale.StartsWith(TEXT("es")))      // Spanish
    {
        return ECulturalRegion::LatinAmerica;
    }
    else if (SystemLocale.StartsWith(TEXT("pt")))      // Portuguese
    {
        return ECulturalRegion::LatinAmerica;
    }
    else if (SystemLocale.StartsWith(TEXT("ru")))      // Russian
    {
        return ECulturalRegion::EasternEurope;
    }
    else if (SystemLocale.StartsWith(TEXT("ar")))      // Arabic
    {
        return ECulturalRegion::MiddleEast;
    }
    else if (SystemLocale.StartsWith(TEXT("hi")))      // Hindi
    {
        return ECulturalRegion::SouthAsia;
    }
    else if (SystemLocale.StartsWith(TEXT("th")))      // Thai
    {
        return ECulturalRegion::SoutheastAsia;
    }
    else if (SystemLocale.StartsWith(TEXT("vi")))      // Vietnamese
    {
        return ECulturalRegion::SoutheastAsia;
    }
    
    // Default to Global region for unsupported locales
    UE_LOG(LogMingRTSCultural, Warning, TEXT("Unsupported locale '%s', defaulting to Global"), *SystemLocale);
    return ECulturalRegion::Global;
}

void UMingRTSCulturalAdaptationSystem::DetectRegionFromIP()
{
    // Async IP-based detection would be implemented here
    // For now, fallback to system detection
    ECulturalRegion Region = DetectRegionFromSystem(); // Fallback
    
    // Broadcast detection completed
    OnRegionDetectionCompleted.Broadcast(Region);
}

// Save cultural preferences to persistent storage
// This ensures user's settings are retained across sessions
void UMingRTSCulturalAdaptationSystem::SaveCulturalPreferences()
{
    // Save to config
    FString RegionString = UEnum::GetValueAsString(Preferences.PrimaryRegion);
    RegionString.RemoveFromStart(TEXT("ECulturalRegion::"));
    
    GConfig->SetString(TEXT("Cultural"), TEXT("PrimaryRegion"), *RegionString, GGameIni);
    GConfig->SetInt(TEXT("Cultural"), TEXT("UserAge"), Preferences.UserAge, GGameIni);
    GConfig->SetInt(TEXT("Cultural"), TEXT("ContentSensitivityLevel"), Preferences.ContentSensitivityLevel, GGameIni);
    GConfig->SetBool(TEXT("Cultural"), TEXT("EnableCulturalEvents"), Preferences.bEnableCulturalEvents, GGameIni);
    GConfig->SetBool(TEXT("Cultural"), TEXT("PreferHistoricalAccuracy"), Preferences.bPreferHistoricalAccuracy, GGameIni);
    GConfig->SetBool(TEXT("Cultural"), TEXT("EnableRegionalArtVariants"), Preferences.bEnableRegionalArtVariants, GGameIni);
    
    GConfig->Flush(false, GGameIni);
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Cultural preferences saved"));
}

// Load cultural preferences from persistent storage
// This restores user's previous settings for consistent experience
void UMingRTSCulturalAdaptationSystem::LoadCulturalPreferences()
{
    // Read primary region from game configuration file
    FString RegionString;
    if (GConfig->GetString(TEXT("Cultural"), TEXT("PrimaryRegion"), RegionString, GGameIni))
    {
        // Convert string back to enum value safely
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
    
    // Read user age for content filtering
    int32 UserAge = 0;
    if (GConfig->GetInt(TEXT("Cultural"), TEXT("UserAge"), UserAge, GGameIni))
    {
        Preferences.UserAge = FMath::Clamp(UserAge, 0, 100);  // Clamp to reasonable range
    }
    
    // Read content sensitivity level
    int32 SensitivityLevel = 0;
    if (GConfig->GetInt(TEXT("Cultural"), TEXT("ContentSensitivityLevel"), SensitivityLevel, GGameIni))
    {
        Preferences.SensitivityLevel = FMath::Clamp(SensitivityLevel, 0, 3);  // Clamp to valid range
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
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Loaded cultural preferences: Region=%s, Age=%d, Sensitivity=%d"),
        *GetRegionDisplayName(Preferences.PrimaryRegion),
        Preferences.UserAge,
        Preferences.SensitivityLevel);
}

// Get region-specific holidays and cultural events for a given year
// This provides cultural context for special events and content
TArray<FString> UMingRTSCulturalAdaptationSystem::GetRegionalHolidays(ECulturalRegion Region, int32 Year) const
{
    TArray<FString> Holidays;
    
    // Add region-specific holidays based on cultural traditions
    // This helps provide authentic cultural experiences for each region
    switch (Region)
    {
    case ECulturalRegion::EastAsia:
        // Traditional East Asian holidays
        Holidays.Add(TEXT("LunarNewYear"));        // Chinese New Year
        Holidays.Add(TEXT("DragonBoatFestival"));   // Dragon Boat Festival
        Holidays.Add(TEXT("MidAutumnFestival"));    // Mid-Autumn Festival
        break;
        
    case ECulturalRegion::MiddleEast:
        // Islamic holidays
        Holidays.Add(TEXT("EidAlFitr"));            // End of Ramadan
        Holidays.Add(TEXT("EidAlAdha"));            // Feast of Sacrifice
        break;
        
    case ECulturalRegion::WesternEurope:
    case ECulturalRegion::NorthAmerica:
        // Western holidays
        Holidays.Add(TEXT("Christmas"));            // Christmas
        Holidays.Add(TEXT("NewYear"));              // New Year's Day
        Holidays.Add(TEXT("Thanksgiving"));         // Thanksgiving (North America)
        break;
        
    case ECulturalRegion::SouthAsia:
        // South Asian holidays
        Holidays.Add(TEXT("Diwali"));               // Festival of Lights
        Holidays.Add(TEXT("Holi"));                 // Festival of Colors
        break;
        
    case ECulturalRegion::SoutheastAsia:
        // Southeast Asian holidays
        Holidays.Add(TEXT("Songkran"));             // Thai New Year
        Holidays.Add(TEXT("Vesak"));                // Buddha's Birthday
        break;
        
    case ECulturalRegion::LatinAmerica:
        // Latin American holidays
        Holidays.Add(TEXT("DiaDeLosMuertos"));      // Day of the Dead
        Holidays.Add(TEXT("Carnival"));             // Carnival
        break;
        
    case ECulturalRegion::EasternEurope:
        // Eastern European holidays
        Holidays.Add(TEXT("OrthodoxChristmas"));    // Orthodox Christmas
        Holidays.Add(TEXT("Maslenitsa"));           // Butter Week
        break;
        
    case ECulturalRegion::Africa:
        // African holidays (simplified representation)
        Holidays.Add(TEXT("Kwanzaa"));              // Kwanzaa
        Holidays.Add(TEXT("AfricaDay"));            // Africa Day
        break;
        
    case ECulturalRegion::Oceania:
        // Oceanian holidays
        Holidays.Add(TEXT("ANZACDay"));             // ANZAC Day
        Holidays.Add(TEXT("WaitangiDay"));          // Waitangi Day (New Zealand)
        break;
        
    case ECulturalRegion::Global:
    default:
        // Global/International holidays
        Holidays.Add(TEXT("NewYear"));              // New Year's Day
        Holidays.Add(TEXT("InternationalWorkersDay")); // May Day
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
              CulturalAdaptationConstants::DefaultTutorialPacing);
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

// Check if content is appropriate for the user's age based on sensitivity levels
// This implements parental controls and content rating systems
bool UMingRTSCulturalAdaptationSystem::CheckContentRating(const FCulturalVariant& Variant, 
    int32 UserAge) const
{
    // Simple age-based content rating system
    // 0 = no restrictions (all ages)
    // 1 = 7+ (child appropriate)
    // 2 = 13+ (teen appropriate)
    // 3 = 18+ (adult only)
    
    switch (Variant.SensitivityLevel)
    {
    case CulturalAdaptationConstants::MinAgeForContent:
        return true;  // No age restrictions
        
    case 1:
        return UserAge >= CulturalAdaptationConstants::ChildAgeLimit;  // Age 7+
        
    case 2:
        return UserAge >= CulturalAdaptationConstants::TeenAgeLimit;  // Age 13+
        
    case 3:
        return UserAge >= CulturalAdaptationConstants::AdultAgeLimit;  // Age 18+
        
    default:
        // Unknown sensitivity level: default to allowing content
        // This is safer than blocking content due to system errors
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

// Clear all cached content and reset cache statistics
// This is useful for memory management or when cultural data changes
void UMingRTSCulturalAdaptationSystem::ClearContentCache()
{
    FScopeLock Lock(&ContentCacheLock);
    
    // Clear all cache data
    ContentCache.Empty();
    LRUCacheOrder.Empty();
    
    // Reset cache statistics
    CacheStats.TotalRequests = 0;
    CacheStats.CacheHits = 0;
    CacheStats.CacheMisses = 0;
    CacheStats.HitRate = 0.0;
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Content cache cleared"));
}

// Update the Least Recently Used (LRU) cache order
// This tracks which cached items are most recently accessed for efficient cache management
void UMingRTSCulturalAdaptationSystem::UpdateLRUOrder(const FString& CacheKey) const
{
    // Move the accessed key to the end of the LRU list (most recently used)
    // This ensures that recently accessed items are kept in cache longer
    LRUCacheOrder.Remove(CacheKey);
    LRUCacheOrder.Add(CacheKey);
}

// Check and trim the content cache if it exceeds maximum size
// This implements LRU cache eviction to prevent memory bloat
void UMingRTSCulturalAdaptationSystem::CheckAndTrimCache()
{
    FScopeLock Lock(&ContentCacheLock);
    
    // Check if cache size exceeds the maximum allowed
    if (ContentCache.Num() <= CulturalAdaptationConstants::MaxContentCacheSize)
    {
        return;  // Cache is within acceptable size limits
    }
    
    // Calculate how many entries to remove (remove oldest 25%)
    // This aggressive trimming ensures the cache stays well below the limit
    const int32 EntriesToRemove = FMath::Max(1, static_cast<int32>(ContentCache.Num() * CulturalAdaptationConstants::CacheTrimRatio));
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Cache size %d exceeds limit %d, removing %d oldest entries"),
        ContentCache.Num(), CulturalAdaptationConstants::MaxContentCacheSize, EntriesToRemove);
    
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
    
    // Rebuild LRU order with remaining entries to maintain consistency
    LRUCacheOrder.Empty();
    for (const auto& CachePair : ContentCache)
    {
        LRUCacheOrder.Add(CachePair.Key);
    }
    
    UE_LOG(LogMingRTSCultural, Log, TEXT("Cache trimmed: removed %d entries, new size %d"),
        RemovedCount, ContentCache.Num());
}
