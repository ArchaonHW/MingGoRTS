// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR/AR Asset Manager Implementation

#include "VRAR/MingRTSVARAssetManager.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "UObject/SoftObjectPtr.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingVARAssets, Log, All);

void UMingRTSVARAssetManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Initializing VR/AR Asset Manager..."));
    
    CreateDefaultAssetRegistry();
    InitializeAssetPools();
    
    UE_LOG(LogMingVARAssets, Log, TEXT("VR/AR Asset Manager initialized"));
}

void UMingRTSVARAssetManager::Deinitialize()
{
    UE_LOG(LogMingVARAssets, Log, TEXT("Shutting down VR/AR Asset Manager..."));
    
    ClearAssetCache();
    
    for (auto& Pool : AssetPools)
    {
        Pool.Value.Empty();
    }
    AssetPools.Empty();
    
    Super::Deinitialize();
}

void UMingRTSVARAssetManager::InitializeAssetManager()
{
    bIsInitialized = true;
    CurrentMemoryUsage = 0;
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Asset Manager initialized with %d MB budget"), MemoryBudgetMB);
}

void UMingRTSVARAssetManager::ShutdownAssetManager()
{
    bIsInitialized = false;
    UnloadUnusedAssets();
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Asset Manager shutdown"));
}

void UMingRTSVARAssetManager::RegisterAsset(const FString& AssetID, EVARAssetType Type, 
                                              const FString& AssetPath, bool bIsDefault)
{
    FVARAssetInfo Info;
    Info.AssetID = AssetID;
    Info.AssetName = FPaths::GetCleanFilename(AssetPath);
    Info.AssetType = Type;
    Info.AssetPath = AssetPath;
    Info.bIsDefault = bIsDefault;
    
    AssetRegistry.Add(AssetID, Info);
    
    if (bIsDefault)
    {
        DefaultAssets.Add(Type, AssetID);
    }
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Registered asset: %s (%s)"), 
           *AssetID, *UEnum::GetValueAsString(Type));
}

void UMingRTSVARAssetManager::UnregisterAsset(const FString& AssetID)
{
    if (LoadedAssets.Contains(AssetID))
    {
        InternalUnloadAsset(AssetID);
    }
    
    if (AssetRegistry.Contains(AssetID))
    {
        FVARAssetInfo& Info = AssetRegistry[AssetID];
        
        if (Info.bIsDefault)
        {
            DefaultAssets.Remove(Info.AssetType);
        }
        
        AssetRegistry.Remove(AssetID);
        
        UE_LOG(LogMingVARAssets, Log, TEXT("Unregistered asset: %s"), *AssetID);
    }
}

TArray<FVARAssetInfo> UMingRTSVARAssetManager::GetRegisteredAssets() const
{
    TArray<FVARAssetInfo> Result;
    AssetRegistry.GenerateValueArray(Result);
    return Result;
}

TArray<FVARAssetInfo> UMingRTSVARAssetManager::GetAssetsByType(EVARAssetType Type) const
{
    TArray<FVARAssetInfo> Result;
    
    for (const auto& Pair : AssetRegistry)
    {
        if (Pair.Value.AssetType == Type)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

UObject* UMingRTSVARAssetManager::LoadAsset(const FString& AssetID, EVARAssetPriority Priority)
{
    if (!AssetRegistry.Contains(AssetID))
    {
        UE_LOG(LogMingVARAssets, Warning, TEXT("Asset not found in registry: %s"), *AssetID);
        return nullptr;
    }
    
    if (LoadedAssets.Contains(AssetID))
    {
        UpdateAssetReferenceCount(AssetID, 1);
        return LoadedAssets[AssetID];
    }
    
    FVARAssetInfo& Info = AssetRegistry[AssetID];
    UObject* Asset = InternalLoadAsset(Info.AssetPath);
    
    if (Asset)
    {
        LoadedAssets.Add(AssetID, Asset);
        Info.bIsLoaded = true;
        Info.ReferenceCount = 1;
        
        // Estimate memory size (simplified)
        Info.MemorySize = sizeof(UObject) * 2; // Placeholder
        CurrentMemoryUsage += Info.MemorySize;
        
        OnAssetLoaded.Broadcast(AssetID, Info.AssetType);
        
        UE_LOG(LogMingVARAssets, Log, TEXT("Loaded asset: %s"), *AssetID);
    }
    else
    {
        OnAssetLoadFailed.Broadcast(AssetID, TEXT("Failed to load from path"));
        
        UE_LOG(LogMingVARAssets, Error, TEXT("Failed to load asset: %s"), *AssetID);
    }
    
    return Asset;
}

UObject* UMingRTSVARAssetManager::LoadAssetAsync(const FString& AssetID, EVARAssetPriority Priority)
{
    // For async loading, we would use FStreamableManager
    // For now, fall back to synchronous loading
    return LoadAsset(AssetID, Priority);
}

void UMingRTSVARAssetManager::UnloadAsset(const FString& AssetID)
{
    if (!AssetRegistry.Contains(AssetID))
    {
        return;
    }
    
    FVARAssetInfo& Info = AssetRegistry[AssetID];
    
    if (Info.ReferenceCount > 1)
    {
        UpdateAssetReferenceCount(AssetID, -1);
        UE_LOG(LogMingVARAssets, Verbose, TEXT("Decremented reference count for: %s (%d remaining)"),
               *AssetID, Info.ReferenceCount);
        return;
    }
    
    InternalUnloadAsset(AssetID);
}

void UMingRTSVARAssetManager::PreloadAssets(EVARAssetType Type)
{
    TArray<FVARAssetInfo> Assets = GetAssetsByType(Type);
    
    for (const FVARAssetInfo& Info : Assets)
    {
        if (!Info.bIsLoaded)
        {
            LoadAsset(Info.AssetID, EVARAssetPriority::Low);
        }
    }
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Preloaded %d assets of type %s"),
           Assets.Num(), *UEnum::GetValueAsString(Type));
}

bool UMingRTSVARAssetManager::IsAssetLoaded(const FString& AssetID) const
{
    if (!AssetRegistry.Contains(AssetID))
    {
        return false;
    }
    
    return AssetRegistry[AssetID].bIsLoaded;
}

void UMingRTSVARAssetManager::ConfigureAssetPool(const FVARAssetPoolConfig& Config)
{
    PoolConfigs.Add(Config.AssetType, Config);
    
    if (Config.bPreloadAssets)
    {
        PreloadAssets(Config.AssetType);
    }
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Configured asset pool for type: %s (size: %d)"),
           *UEnum::GetValueAsString(Config.AssetType), Config.PoolSize);
}

UObject* UMingRTSVARAssetManager::AcquireAssetFromPool(EVARAssetType Type)
{
    if (!AssetPools.Contains(Type) || AssetPools[Type].Num() == 0)
    {
        // Pool is empty, create a new instance
        FString DefaultAssetID = DefaultAssets.Contains(Type) ? DefaultAssets[Type] : FString();
        
        if (!DefaultAssetID.IsEmpty())
        {
            return LoadAsset(DefaultAssetID, EVARAssetPriority::Medium);
        }
        
        return nullptr;
    }
    
    // Get asset from pool
    TArray<UObject*>& Pool = AssetPools[Type];
    UObject* Asset = Pool.Last();
    Pool.RemoveAt(Pool.Num() - 1);
    
    UE_LOG(LogMingVARAssets, Verbose, TEXT("Acquired asset from pool: %s"), 
           *UEnum::GetValueAsString(Type));
    
    return Asset;
}

void UMingRTSVARAssetManager::ReturnAssetToPool(UObject* Asset, EVARAssetType Type)
{
    if (!Asset)
    {
        return;
    }
    
    if (!AssetPools.Contains(Type))
    {
        AssetPools.Add(Type, TArray<UObject*>());
    }
    
    FVARAssetPoolConfig* Config = PoolConfigs.Find(Type);
    int32 MaxPoolSize = Config ? Config->PoolSize : 10;
    
    TArray<UObject*>& Pool = AssetPools[Type];
    
    if (Pool.Num() < MaxPoolSize)
    {
        Pool.Add(Asset);
        
        UE_LOG(LogMingVARAssets, Verbose, TEXT("Returned asset to pool: %s (pool size: %d)"),
               *UEnum::GetValueAsString(Type), Pool.Num());
    }
    else
    {
        // Pool is full, destroy the asset
        // In production, this would properly destroy the asset
        UE_LOG(LogMingVARAssets, Verbose, TEXT("Pool full, destroying asset: %s"),
               *UEnum::GetValueAsString(Type));
    }
}

void UMingRTSVARAssetManager::ClearAssetPool(EVARAssetType Type)
{
    if (AssetPools.Contains(Type))
    {
        AssetPools[Type].Empty();
        
        UE_LOG(LogMingVARAssets, Log, TEXT("Cleared asset pool: %s"),
               *UEnum::GetValueAsString(Type));
    }
}

void UMingRTSVARAssetManager::SetDefaultAsset(EVARAssetType Type, const FString& AssetID)
{
    if (AssetRegistry.Contains(AssetID))
    {
        // Remove old default
        for (auto& Pair : AssetRegistry)
        {
            if (Pair.Value.AssetType == Type && Pair.Value.bIsDefault)
            {
                Pair.Value.bIsDefault = false;
            }
        }
        
        // Set new default
        AssetRegistry[AssetID].bIsDefault = true;
        DefaultAssets.Add(Type, AssetID);
        
        UE_LOG(LogMingVARAssets, Log, TEXT("Set default asset for %s: %s"),
               *UEnum::GetValueAsString(Type), *AssetID);
    }
}

UObject* UMingRTSVARAssetManager::GetDefaultAsset(EVARAssetType Type)
{
    if (DefaultAssets.Contains(Type))
    {
        return LoadAsset(DefaultAssets[Type], EVARAssetPriority::High);
    }
    
    // Try to find any asset of this type
    TArray<FVARAssetInfo> Assets = GetAssetsByType(Type);
    if (Assets.Num() > 0)
    {
        return LoadAsset(Assets[0].AssetID, EVARAssetPriority::High);
    }
    
    return nullptr;
}

void UMingRTSVARAssetManager::LoadAllDefaultAssets()
{
    for (const auto& Pair : DefaultAssets)
    {
        LoadAsset(Pair.Value, EVARAssetPriority::High);
    }
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded all default assets (%d types)"), 
           DefaultAssets.Num());
}

int32 UMingRTSVARAssetManager::GetTotalMemoryUsage() const
{
    return CurrentMemoryUsage / (1024 * 1024); // Convert to MB
}

int32 UMingRTSVARAssetManager::GetMemoryUsageByType(EVARAssetType Type) const
{
    int32 TotalSize = 0;
    
    for (const auto& Pair : AssetRegistry)
    {
        if (Pair.Value.AssetType == Type && Pair.Value.bIsLoaded)
        {
            TotalSize += Pair.Value.MemorySize;
        }
    }
    
    return TotalSize / (1024 * 1024); // Convert to MB
}

void UMingRTSVARAssetManager::SetMemoryBudget(int32 BudgetMB)
{
    MemoryBudgetMB = FMath::Max(128, BudgetMB);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Memory budget set to: %d MB"), MemoryBudgetMB);
    
    // Check if we need to unload assets
    if (GetTotalMemoryUsage() > MemoryBudgetMB * 0.9f)
    {
        UnloadUnusedAssets();
    }
}

void UMingRTSVARAssetManager::UnloadUnusedAssets()
{
    TArray<FString> AssetsToUnload;
    
    for (const auto& Pair : AssetRegistry)
    {
        if (Pair.Value.bIsLoaded && Pair.Value.ReferenceCount == 0)
        {
            AssetsToUnload.Add(Pair.Key);
        }
    }
    
    for (const FString& AssetID : AssetsToUnload)
    {
        InternalUnloadAsset(AssetID);
    }
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Unloaded %d unused assets"), AssetsToUnload.Num());
}

void UMingRTSVARAssetManager::ClearAssetCache()
{
    TArray<FString> LoadedAssetIDs;
    LoadedAssets.GetKeys(LoadedAssetIDs);
    
    for (const FString& AssetID : LoadedAssetIDs)
    {
        InternalUnloadAsset(AssetID);
    }
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Cleared asset cache"));
}

void UMingRTSVARAssetManager::LoadVRControllerAssets()
{
    PreloadAssets(EVARAssetType::VRControllerMesh);
    PreloadAssets(EVARAssetType::VRPointer);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded VR controller assets"));
}

void UMingRTSVARAssetManager::LoadVRHandAssets()
{
    PreloadAssets(EVARAssetType::VRHandMesh);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded VR hand assets"));
}

void UMingRTSVARAssetManager::LoadVRUIAssets()
{
    PreloadAssets(EVARAssetType::VRUIMesh);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded VR UI assets"));
}

void UMingRTSVARAssetManager::LoadVREnvironmentAssets()
{
    PreloadAssets(EVARAssetType::VRSkybox);
    PreloadAssets(EVARAssetType::ComfortVignette);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded VR environment assets"));
}

void UMingRTSVARAssetManager::LoadARPlaneMaterials()
{
    PreloadAssets(EVARAssetType::ARPlaneMaterial);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded AR plane materials"));
}

void UMingRTSVARAssetManager::LoadAROverlayAssets()
{
    PreloadAssets(EVARAssetType::ARContentOverlay);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded AR overlay assets"));
}

void UMingRTSVARAssetManager::LoadARCursorAssets()
{
    PreloadAssets(EVARAssetType::ARCursor);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded AR cursor assets"));
}

void UMingRTSVARAssetManager::LoadPlatformSpecificAssets()
{
#if PLATFORM_ANDROID
    LoadARCoreAssets();
#elif PLATFORM_IOS
    LoadARKitAssets();
#elif PLATFORM_QUEST
    LoadQuestAssets();
#elif PLATFORM_PS5
    LoadPSVRAssets();
#endif
}

void UMingRTSVARAssetManager::LoadQuestAssets()
{
    // Load Meta Quest specific assets
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded Quest specific assets"));
}

void UMingRTSVARAssetManager::LoadPSVRAssets()
{
    // Load PlayStation VR specific assets
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded PSVR specific assets"));
}

void UMingRTSVARAssetManager::LoadSteamVRAssets()
{
    // Load SteamVR specific assets
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded SteamVR specific assets"));
}

void UMingRTSVARAssetManager::LoadARCoreAssets()
{
    // Load ARCore specific assets
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded ARCore specific assets"));
}

void UMingRTSVARAssetManager::LoadARKitAssets()
{
    // Load ARKit specific assets
    UE_LOG(LogMingVARAssets, Log, TEXT("Loaded ARKit specific assets"));
}

void UMingRTSVARAssetManager::CreateDefaultAssetRegistry()
{
    // Register default VR controller meshes
    RegisterAsset(TEXT("VR_LeftController"), EVARAssetType::VRControllerMesh, 
                  TEXT("/Game/VR/Controllers/LeftController.LeftController"), true);
    RegisterAsset(TEXT("VR_RightController"), EVARAssetType::VRControllerMesh, 
                  TEXT("/Game/VR/Controllers/RightController.RightController"), true);
    
    // Register default VR hand meshes
    RegisterAsset(TEXT("VR_LeftHand"), EVARAssetType::VRHandMesh, 
                  TEXT("/Game/VR/Hands/LeftHand.LeftHand"), true);
    RegisterAsset(TEXT("VR_RightHand"), EVARAssetType::VRHandMesh, 
                  TEXT("/Game/VR/Hands/RightHand.RightHand"), true);
    
    // Register default VR pointer
    RegisterAsset(TEXT("VR_Pointer"), EVARAssetType::VRPointer, 
                  TEXT("/Game/VR/Pointer.Pointer"), true);
    
    // Register default teleport marker
    RegisterAsset(TEXT("VR_TeleportMarker"), EVARAssetType::VRTeleportMarker, 
                  TEXT("/Game/VR/TeleportMarker.TeleportMarker"), true);
    
    // Register default AR cursor
    RegisterAsset(TEXT("AR_Cursor"), EVARAssetType::ARCursor, 
                  TEXT("/Game/AR/Cursor.Cursor"), true);
    
    // Register default AR plane material
    RegisterAsset(TEXT("AR_PlaneMaterial"), EVARAssetType::ARPlaneMaterial, 
                  TEXT("/Game/AR/PlaneMaterial.PlaneMaterial"), true);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Created default asset registry with %d assets"),
           AssetRegistry.Num());
}

void UMingRTSVARAssetManager::InitializeAssetPools()
{
    // Configure default pools
    FVARAssetPoolConfig PointerPool;
    PointerPool.AssetType = EVARAssetType::VRPointer;
    PointerPool.PoolSize = 5;
    PointerPool.bPreloadAssets = false;
    PoolConfigs.Add(EVARAssetType::VRPointer, PointerPool);
    
    FVARAssetPoolConfig CursorPool;
    CursorPool.AssetType = EVARAssetType::ARCursor;
    CursorPool.PoolSize = 3;
    CursorPool.bPreloadAssets = false;
    PoolConfigs.Add(EVARAssetType::ARCursor, CursorPool);
    
    UE_LOG(LogMingVARAssets, Log, TEXT("Initialized asset pools"));
}

UObject* UMingRTSVARAssetManager::InternalLoadAsset(const FString& AssetPath)
{
    // In production, this would use UAssetManager or FStreamableManager
    // For now, return a placeholder
    return nullptr;
}

void UMingRTSVARAssetManager::InternalUnloadAsset(const FString& AssetID)
{
    if (!AssetRegistry.Contains(AssetID))
    {
        return;
    }
    
    FVARAssetInfo& Info = AssetRegistry[AssetID];
    
    if (LoadedAssets.Contains(AssetID))
    {
        // Update memory tracking
        CurrentMemoryUsage -= Info.MemorySize;
        
        // Remove from loaded assets
        LoadedAssets.Remove(AssetID);
        
        // Update asset info
        Info.bIsLoaded = false;
        Info.ReferenceCount = 0;
        
        OnAssetUnloaded.Broadcast(AssetID);
        
        UE_LOG(LogMingVARAssets, Log, TEXT("Unloaded asset: %s"), *AssetID);
    }
}

bool UMingRTSVARAssetManager::IsAssetInUse(const FString& AssetID) const
{
    if (!AssetRegistry.Contains(AssetID))
    {
        return false;
    }
    
    return AssetRegistry[AssetID].ReferenceCount > 0;
}

void UMingRTSVARAssetManager::UpdateAssetReferenceCount(const FString& AssetID, int32 Delta)
{
    if (AssetRegistry.Contains(AssetID))
    {
        FVARAssetInfo& Info = AssetRegistry[AssetID];
        Info.ReferenceCount = FMath::Max(0, Info.ReferenceCount + Delta);
    }
}
