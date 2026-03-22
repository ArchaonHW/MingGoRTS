// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingRTSMemoryOptimizer.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Texture,
	Mesh,
	Animation,
	Audio,
	Blueprint,
	Particle,
	Material,
	Custom
};

UENUM(BlueprintType)
enum class ELoadingPriority : uint8
{
	Immediate,
	High,
	Normal,
	Low,
	Background
};

UENUM(BlueprintType)
enum class EMemoryPressureLevel : uint8
{
	Normal,
	Elevated,
	Critical,
	Emergency
};

USTRUCT(BlueprintType)
struct FStreamingAsset
{
	GENERATED_BODY()

	UPROPERTY()
	FString AssetPath;

	UPROPERTY()
	EResourceType ResourceType;

	UPROPERTY()
	ELoadingPriority Priority;

	UPROPERTY()
	bool bIsLoaded;

	UPROPERTY()
	bool bIsStreamed;

	UPROPERTY()
	float LastAccessTime;

	UPROPERTY()
	float LastUsedTime;

	UPROPERTY()
	int32 ReferenceCount;

	UPROPERTY()
	int32 SizeKB;

	UPROPERTY()
	bool bKeepInMemory;

	FStreamingAsset()
		: ResourceType(EResourceType::Texture)
		, Priority(ELoadingPriority::Normal)
		, bIsLoaded(false)
		, bIsStreamed(false)
		, LastAccessTime(0.0f)
		, LastUsedTime(0.0f)
		, ReferenceCount(0)
		, SizeKB(0)
		, bKeepInMemory(false)
	{}
};

USTRUCT(BlueprintType)
struct FMemoryBudget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	int32 TotalBudgetMB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	int32 TextureBudgetMB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	int32 MeshBudgetMB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	int32 AnimationBudgetMB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	int32 AudioBudgetMB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	int32 ReservedBudgetMB;

	FMemoryBudget()
		: TotalBudgetMB(1024)
		, TextureBudgetMB(512)
		, MeshBudgetMB(256)
		, AnimationBudgetMB(128)
		, AudioBudgetMB(64)
		, ReservedBudgetMB(64)
	{}

	int32 GetUsedBudgetMB() const
	{
		return TextureBudgetMB + MeshBudgetMB + AnimationBudgetMB + AudioBudgetMB + ReservedBudgetMB;
	}
};

USTRUCT(BlueprintType)
struct FMemoryStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 CurrentMemoryUsageMB;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 PeakMemoryUsageMB;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 AvailableMemoryMB;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 TextureMemoryMB;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 MeshMemoryMB;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 LoadedAssetCount;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 StreamedAssetCount;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	EMemoryPressureLevel PressureLevel;

	FMemoryStats()
		: CurrentMemoryUsageMB(0)
		, PeakMemoryUsageMB(0)
		, AvailableMemoryMB(0)
		, TextureMemoryMB(0)
		, MeshMemoryMB(0)
		, LoadedAssetCount(0)
		, StreamedAssetCount(0)
		, PressureLevel(EMemoryPressureLevel::Normal)
	{}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMemoryPressureChanged, EMemoryPressureLevel, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMemoryWarning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMemoryCritical);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGCORE_API UMingRTSMemoryOptimizer : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMingRTSMemoryOptimizer();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Asset Streaming
	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void RegisterStreamingAsset(const FString& AssetPath, EResourceType Type, ELoadingPriority Priority);

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	bool RequestAssetLoad(const FString& AssetPath);

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void RequestAssetUnload(const FString& AssetPath);

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void PreloadAssetsForLevel(const FString& LevelName);

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void StreamOutUnusedAssets(float UnusedTimeThreshold);

	// Memory Budget Management
	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void SetMemoryBudget(const FMemoryBudget& Budget);

	UFUNCTION(BlueprintPure, Category = "Memory Optimizer")
	FMemoryBudget GetMemoryBudget() const;

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void AdjustBudgetForPlatform();

	// Memory Monitoring
	UFUNCTION(BlueprintPure, Category = "Memory Optimizer")
	FMemoryStats GetMemoryStats() const;

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void UpdateMemoryStats();

	UFUNCTION(BlueprintPure, Category = "Memory Optimizer")
	EMemoryPressureLevel GetCurrentMemoryPressure() const;

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void SetMemoryPressureThresholds(int32 ElevatedMB, int32 CriticalMB, int32 EmergencyMB);

	// Garbage Collection Control
	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void ForceGarbageCollection();

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void SetGCInterval(float IntervalSeconds);

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void TriggerIncrementalGC();

	// Texture Streaming
	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void SetTextureStreamingPoolSize(int32 SizeMB);

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void RefreshTextureStreaming();

	// Memory Optimization
	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void OptimizeMemoryUsage();

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void CompactMemory();

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void ClearUnusedAssets();

	// Resource Management
	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void SetAssetKeepInMemory(const FString& AssetPath, bool bKeep);

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void ReferenceAsset(const FString& AssetPath);

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void DereferenceAsset(const FString& AssetPath);

	// Reporting
	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	FString GetMemoryReport() const;

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	TArray<FString> GetTopMemoryConsumers(int32 Count) const;

	UFUNCTION(BlueprintCallable, Category = "Memory Optimizer")
	void DumpMemoryUsageToLog();

public:
	UPROPERTY(BlueprintAssignable, Category = "Memory Optimizer|Events")
	FOnMemoryPressureChanged OnMemoryPressureChanged;

	UPROPERTY(BlueprintAssignable, Category = "Memory Optimizer|Events")
	FOnMemoryWarning OnMemoryWarning;

	UPROPERTY(BlueprintAssignable, Category = "Memory Optimizer|Events")
	FOnMemoryCritical OnMemoryCritical;

protected:
	UPROPERTY()
	TMap<FString, FStreamingAsset> StreamingAssets;

	UPROPERTY()
	FMemoryBudget CurrentBudget;

	UPROPERTY()
	FMemoryStats CurrentStats;

	UPROPERTY()
	FTimerHandle MemoryMonitorTimerHandle;

	UPROPERTY()
	FTimerHandle GCTimerHandle;

	int32 ElevatedThresholdMB;
	int32 CriticalThresholdMB;
	int32 EmergencyThresholdMB;

	mutable FCriticalSection MemoryLock;

protected:
	void OnMemoryMonitorTick();
	void OnGCTick();
	void UpdateMemoryPressure();
	void HandleMemoryPressure();
	void UnloadLowPriorityAssets(int32 TargetMemoryMB);
	int32 CalculateAssetMemoryUsage(const FStreamingAsset& Asset) const;
	bool IsAssetInUse(const FStreamingAsset& Asset) const;
	void LoadAssetInternal(const FString& AssetPath);
	void UnloadAssetInternal(const FString& AssetPath);
};
