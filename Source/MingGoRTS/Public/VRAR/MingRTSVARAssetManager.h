// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR/AR Asset Management

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingRTSVARAssetManager.generated.h"

/**
 * VR/AR Asset Types
 */
UENUM(BlueprintType)
enum class EVARAssetType : uint8
{
    VRControllerMesh     UMETA(DisplayName = "VR Controller Mesh"),
    VRHandMesh          UMETA(DisplayName = "VR Hand Mesh"),
    VRPointer           UMETA(DisplayName = "VR Pointer"),
    VRTeleportMarker    UMETA(DisplayName = "VR Teleport Marker"),
    VRUIMesh            UMETA(DisplayName = "VR UI Mesh"),
    VRSkybox            UMETA(DisplayName = "VR Skybox"),
    ARCursor            UMETA(DisplayName = "AR Cursor"),
    ARPlaneMaterial     UMETA(DisplayName = "AR Plane Material"),
    ARContentOverlay    UMETA(DisplayName = "AR Content Overlay"),
    ARAnchor            UMETA(DisplayName = "AR Anchor"),
    HapticFeedback      UMETA(DisplayName = "Haptic Feedback"),
    SpatialAudio        UMETA(DisplayName = "Spatial Audio"),
    ComfortVignette     UMETA(DisplayName = "Comfort Vignette"),
    DefaultEnvironment  UMETA(DisplayName = "Default Environment")
};

/**
 * VR/AR Asset Info Structure
 */
USTRUCT(BlueprintType)
struct FVARAssetInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Asset")
    FString AssetID;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Asset")
    FString AssetName;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Asset")
    EVARAssetType AssetType = EVARAssetType::VRControllerMesh;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Asset")
    FString AssetPath;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Asset")
    int32 MemorySize = 0;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Asset")
    bool bIsLoaded = false;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Asset")
    bool bIsDefault = false;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Asset")
    int32 ReferenceCount = 0;
};

/**
 * Asset Loading Priority
 */
UENUM(BlueprintType)
enum class EVARAssetPriority : uint8
{
    Low         UMETA(DisplayName = "Low"),
    Medium      UMETA(DisplayName = "Medium"),
    High        UMETA(DisplayName = "High"),
    Critical    UMETA(DisplayName = "Critical")
};

/**
 * Asset Pool Configuration
 */
USTRUCT(BlueprintType)
struct FVARAssetPoolConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Pool")
    EVARAssetType AssetType = EVARAssetType::VRControllerMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Pool")
    int32 PoolSize = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Pool")
    bool bPreloadAssets = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Pool")
    float UnloadTimeout = 30.0f;
};

/**
 * MingGoRTS VR/AR Asset Manager
 * Manages loading, caching, and pooling of VR/AR specific assets
 */
UCLASS()
class MINGGORTS_API UMingRTSVARAssetManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets")
    void InitializeAssetManager();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets")
    void ShutdownAssetManager();

    // Asset Registration
    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Registration")
    void RegisterAsset(const FString& AssetID, EVARAssetType Type, const FString& AssetPath, 
                       bool bIsDefault = false);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Registration")
    void UnregisterAsset(const FString& AssetID);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Registration")
    TArray<FVARAssetInfo> GetRegisteredAssets() const;

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Registration")
    TArray<FVARAssetInfo> GetAssetsByType(EVARAssetType Type) const;

    // Asset Loading
    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Loading")
    UObject* LoadAsset(const FString& AssetID, EVARAssetPriority Priority = EVARAssetPriority::Medium);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Loading")
    UObject* LoadAssetAsync(const FString& AssetID, EVARAssetPriority Priority = EVARAssetPriority::Medium);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Loading")
    void UnloadAsset(const FString& AssetID);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Loading")
    void PreloadAssets(EVARAssetType Type);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Loading")
    bool IsAssetLoaded(const FString& AssetID) const;

    // Asset Pools
    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Pooling")
    void ConfigureAssetPool(const FVARAssetPoolConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Pooling")
    UObject* AcquireAssetFromPool(EVARAssetType Type);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Pooling")
    void ReturnAssetToPool(UObject* Asset, EVARAssetType Type);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Pooling")
    void ClearAssetPool(EVARAssetType Type);

    // Default Assets
    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Defaults")
    void SetDefaultAsset(EVARAssetType Type, const FString& AssetID);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Defaults")
    UObject* GetDefaultAsset(EVARAssetType Type);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Defaults")
    void LoadAllDefaultAssets();

    // Memory Management
    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Memory")
    int32 GetTotalMemoryUsage() const;

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Memory")
    int32 GetMemoryUsageByType(EVARAssetType Type) const;

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Memory")
    void SetMemoryBudget(int32 BudgetMB);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Memory")
    void UnloadUnusedAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Memory")
    void ClearAssetCache();

    // VR Specific Assets
    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|VR")
    void LoadVRControllerAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|VR")
    void LoadVRHandAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|VR")
    void LoadVRUIAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|VR")
    void LoadVREnvironmentAssets();

    // AR Specific Assets
    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|AR")
    void LoadARPlaneMaterials();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|AR")
    void LoadAROverlayAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|AR")
    void LoadARCursorAssets();

    // Platform Specific
    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Platform")
    void LoadPlatformSpecificAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Platform")
    void LoadQuestAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Platform")
    void LoadPSVRAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Platform")
    void LoadSteamVRAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Platform")
    void LoadARCoreAssets();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Assets|Platform")
    void LoadARKitAssets();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "VR/AR Assets|Events")
    FOnAssetLoaded OnAssetLoaded;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Assets|Events")
    FOnAssetUnloaded OnAssetUnloaded;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Assets|Events")
    FOnAssetLoadFailed OnAssetLoadFailed;

    // Override
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    void CreateDefaultAssetRegistry();
    void InitializeAssetPools();
    UObject* InternalLoadAsset(const FString& AssetPath);
    void InternalUnloadAsset(const FString& AssetID);
    bool IsAssetInUse(const FString& AssetID) const;
    void UpdateAssetReferenceCount(const FString& AssetID, int32 Delta);

    UPROPERTY()
    TMap<FString, FVARAssetInfo> AssetRegistry;

    UPROPERTY()
    TMap<FString, UObject*> LoadedAssets;

    UPROPERTY()
    TMap<EVARAssetType, FString> DefaultAssets;

    UPROPERTY()
    TMap<EVARAssetType, TArray<UObject*>> AssetPools;

    UPROPERTY()
    TMap<EVARAssetType, FVARAssetPoolConfig> PoolConfigs;

    UPROPERTY()
    int32 MemoryBudgetMB = 512;

    UPROPERTY()
    int32 CurrentMemoryUsage = 0;

    UPROPERTY()
    bool bIsInitialized = false;
};

// Event delegate declarations
declare dynamic_multicast_delegate FOnAssetLoaded(const FString& AssetID, EVARAssetType Type);
declare dynamic_multicast_delegate FOnAssetUnloaded(const FString& AssetID);
declare dynamic_multicast_delegate FOnAssetLoadFailed(const FString& AssetID, const FString& Error);
