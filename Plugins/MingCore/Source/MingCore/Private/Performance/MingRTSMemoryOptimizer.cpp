// Copyright Epic Games, Inc. All Rights Reserved.

#include "Performance/MingRTSMemoryOptimizer.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"
#include "Engine/TextureStreaming.h"
#include "Misc/App.h"

UMingRTSMemoryOptimizer::UMingRTSMemoryOptimizer()
	: ElevatedThresholdMB(768)
	, CriticalThresholdMB(896)
	, EmergencyThresholdMB(960)
{
	// Set default budget
	CurrentBudget = FMemoryBudget();
}

void UMingRTSMemoryOptimizer::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GetGameInstance())
	{
		// Start memory monitoring
		FTimerDelegate MonitorDelegate;
		MonitorDelegate.BindUObject(this, &UMingRTSMemoryOptimizer::OnMemoryMonitorTick);
		GetGameInstance()->GetTimerManager().SetTimer(MemoryMonitorTimerHandle, MonitorDelegate, 2.0f, true);

		// Start GC timer
		FTimerDelegate GCDelegate;
		GCDelegate.BindUObject(this, &UMingRTSMemoryOptimizer::OnGCTick);
		GetGameInstance()->GetTimerManager().SetTimer(GCTimerHandle, GCDelegate, 60.0f, true);
	}

	AdjustBudgetForPlatform();

	UE_LOG(LogTemp, Log, TEXT("Memory Optimizer initialized with %d MB budget"), CurrentBudget.TotalBudgetMB);
}

void UMingRTSMemoryOptimizer::Deinitialize()
{
	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(MemoryMonitorTimerHandle);
		GetGameInstance()->GetTimerManager().ClearTimer(GCTimerHandle);
	}

	StreamingAssets.Empty();

	Super::Deinitialize();
}

void UMingRTSMemoryOptimizer::RegisterStreamingAsset(const FString& AssetPath, EResourceType Type, ELoadingPriority Priority)
{
	FScopeLock Lock(&MemoryLock);

	FStreamingAsset Asset;
	Asset.AssetPath = AssetPath;
	Asset.ResourceType = Type;
	Asset.Priority = Priority;
	Asset.bIsLoaded = false;
	Asset.LastAccessTime = FPlatformTime::Seconds();

	StreamingAssets.Add(AssetPath, Asset);
}

bool UMingRTSMemoryOptimizer::RequestAssetLoad(const FString& AssetPath)
{
	FScopeLock Lock(&MemoryLock);

	FStreamingAsset* Asset = StreamingAssets.Find(AssetPath);
	if (!Asset)
	{
		return false;
	}

	Asset->LastAccessTime = FPlatformTime::Seconds();
	Asset->ReferenceCount++;

	if (!Asset->bIsLoaded)
	{
		// Check memory budget before loading
		UpdateMemoryStats();
		if (CurrentStats.AvailableMemoryMB > 50) // Need at least 50MB free
		{
			LoadAssetInternal(AssetPath);
			return true;
		}
		else
		{
			// Try to free memory
			UnloadLowPriorityAssets(100);
			
			// Try again
			UpdateMemoryStats();
			if (CurrentStats.AvailableMemoryMB > 50)
			{
				LoadAssetInternal(AssetPath);
				return true;
			}
			
			return false; // Cannot load due to memory constraints
		}
	}

	return true; // Already loaded
}

void UMingRTSMemoryOptimizer::RequestAssetUnload(const FString& AssetPath)
{
	FScopeLock Lock(&MemoryLock);

	FStreamingAsset* Asset = StreamingAssets.Find(AssetPath);
	if (!Asset)
	{
		return;
	}

	Asset->ReferenceCount = FMath::Max(0, Asset->ReferenceCount - 1);

	if (Asset->ReferenceCount == 0 && !Asset->bKeepInMemory)
	{
		UnloadAssetInternal(AssetPath);
	}
}

void UMingRTSMemoryOptimizer::PreloadAssetsForLevel(const FString& LevelName)
{
	// Mark all assets for this level as high priority
	FString LevelPrefix = FString::Printf(TEXT("/Game/Maps/%s/"), *LevelName);

	for (auto& Pair : StreamingAssets)
	{
		if (Pair.Value.AssetPath.StartsWith(LevelPrefix))
		{
			Pair.Value.Priority = ELoadingPriority::High;
			RequestAssetLoad(Pair.Value.AssetPath);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Preloaded assets for level: %s"), *LevelName);
}

void UMingRTSMemoryOptimizer::StreamOutUnusedAssets(float UnusedTimeThreshold)
{
	FScopeLock Lock(&MemoryLock);

	float CurrentTime = FPlatformTime::Seconds();
	TArray<FString> AssetsToUnload;

	for (auto& Pair : StreamingAssets)
	{
		FStreamingAsset& Asset = Pair.Value;
		if (Asset.bIsLoaded && !Asset.bKeepInMemory && Asset.ReferenceCount == 0)
		{
			float UnusedTime = CurrentTime - Asset.LastAccessTime;
			if (UnusedTime > UnusedTimeThreshold)
			{
				AssetsToUnload.Add(Pair.Key);
			}
		}
	}

	for (const FString& AssetPath : AssetsToUnload)
	{
		UnloadAssetInternal(AssetPath);
	}

	UE_LOG(LogTemp, Log, TEXT("Streamed out %d unused assets"), AssetsToUnload.Num());
}

void UMingRTSMemoryOptimizer::SetMemoryBudget(const FMemoryBudget& Budget)
{
	CurrentBudget = Budget;
	
	// Apply texture streaming budget
	SetTextureStreamingPoolSize(CurrentBudget.TextureBudgetMB);
}

FMemoryBudget UMingRTSMemoryOptimizer::GetMemoryBudget() const
{
	return CurrentBudget;
}

void UMingRTSMemoryOptimizer::AdjustBudgetForPlatform()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	// Mobile platforms - more conservative
	CurrentBudget.TotalBudgetMB = 512;
	CurrentBudget.TextureBudgetMB = 256;
	CurrentBudget.MeshBudgetMB = 128;
	CurrentBudget.AnimationBudgetMB = 64;
	CurrentBudget.AudioBudgetMB = 32;
	CurrentBudget.ReservedBudgetMB = 32;
#elif PLATFORM_XBOXONE || PLATFORM_PS4 || PLATFORM_XBOXSERIESX || PLATFORM_PS5
	// Consoles - generous budget
	CurrentBudget.TotalBudgetMB = 2048;
	CurrentBudget.TextureBudgetMB = 1024;
	CurrentBudget.MeshBudgetMB = 512;
	CurrentBudget.AnimationBudgetMB = 256;
	CurrentBudget.AudioBudgetMB = 128;
	CurrentBudget.ReservedBudgetMB = 128;
#else
	// PC - dynamic based on available RAM
	int32 TotalRAM = FPlatformMemory::GetPhysicalGBRam();
	if (TotalRAM >= 16)
	{
		CurrentBudget.TotalBudgetMB = 4096;
		CurrentBudget.TextureBudgetMB = 2048;
		CurrentBudget.MeshBudgetMB = 1024;
		CurrentBudget.AnimationBudgetMB = 512;
		CurrentBudget.AudioBudgetMB = 256;
		CurrentBudget.ReservedBudgetMB = 256;
	}
	else if (TotalRAM >= 8)
	{
		CurrentBudget.TotalBudgetMB = 2048;
		CurrentBudget.TextureBudgetMB = 1024;
		CurrentBudget.MeshBudgetMB = 512;
		CurrentBudget.AnimationBudgetMB = 256;
		CurrentBudget.AudioBudgetMB = 128;
		CurrentBudget.ReservedBudgetMB = 128;
	}
	else
	{
		CurrentBudget.TotalBudgetMB = 1024;
		CurrentBudget.TextureBudgetMB = 512;
		CurrentBudget.MeshBudgetMB = 256;
		CurrentBudget.AnimationBudgetMB = 128;
		CurrentBudget.AudioBudgetMB = 64;
		CurrentBudget.ReservedBudgetMB = 64;
	}
#endif

	SetTextureStreamingPoolSize(CurrentBudget.TextureBudgetMB);
}

FMemoryStats UMingRTSMemoryOptimizer::GetMemoryStats() const
{
	FScopeLock Lock(&MemoryLock);
	return CurrentStats;
}

void UMingRTSMemoryOptimizer::UpdateMemoryStats()
{
	FScopeLock Lock(&MemoryLock);

	// Get current memory usage
	CurrentStats.CurrentMemoryUsageMB = FPlatformMemory::GetUsedVirtualMemory() / (1024 * 1024);
	CurrentStats.AvailableMemoryMB = CurrentBudget.TotalBudgetMB - CurrentStats.CurrentMemoryUsageMB;

	// Calculate type-specific memory
	CurrentStats.TextureMemoryMB = 0;
	CurrentStats.MeshMemoryMB = 0;
	CurrentStats.LoadedAssetCount = 0;
	CurrentStats.StreamedAssetCount = 0;

	for (const auto& Pair : StreamingAssets)
	{
		const FStreamingAsset& Asset = Pair.Value;
		if (Asset.bIsLoaded)
		{
			CurrentStats.LoadedAssetCount++;
			int32 AssetSize = CalculateAssetMemoryUsage(Asset);
			
			switch (Asset.ResourceType)
			{
			case EResourceType::Texture:
				CurrentStats.TextureMemoryMB += AssetSize / 1024;
				break;
			case EResourceType::Mesh:
				CurrentStats.MeshMemoryMB += AssetSize / 1024;
				break;
			default:
				break;
			}
		}

		if (Asset.bIsStreamed)
		{
			CurrentStats.StreamedAssetCount++;
		}
	}

	// Update peak
	if (CurrentStats.CurrentMemoryUsageMB > CurrentStats.PeakMemoryUsageMB)
	{
		CurrentStats.PeakMemoryUsageMB = CurrentStats.CurrentMemoryUsageMB;
	}

	UpdateMemoryPressure();
}

EMemoryPressureLevel UMingRTSMemoryOptimizer::GetCurrentMemoryPressure() const
{
	return CurrentStats.PressureLevel;
}

void UMingRTSMemoryOptimizer::SetMemoryPressureThresholds(int32 ElevatedMB, int32 CriticalMB, int32 EmergencyMB)
{
	ElevatedThresholdMB = ElevatedMB;
	CriticalThresholdMB = CriticalMB;
	EmergencyThresholdMB = EmergencyMB;
}

void UMingRTSMemoryOptimizer::ForceGarbageCollection()
{
	GEngine->ForceGarbageCollection(true);
	UE_LOG(LogTemp, Log, TEXT("Forced garbage collection"));
}

void UMingRTSMemoryOptimizer::SetGCInterval(float IntervalSeconds)
{
	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(GCTimerHandle);
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UMingRTSMemoryOptimizer::OnGCTick);
		GetGameInstance()->GetTimerManager().SetTimer(GCTimerHandle, Delegate, IntervalSeconds, true);
	}
}

void UMingRTSMemoryOptimizer::TriggerIncrementalGC()
{
	GEngine->PerformGarbageCollection(EForceGarbageCollectionFlags::IncrementalReachability);
}

void UMingRTSMemoryOptimizer::SetTextureStreamingPoolSize(int32 SizeMB)
{
	if (GConfig)
	{
		GConfig->SetInt(TEXT("TextureStreaming"), TEXT("PoolSize"), SizeMB, GEngineIni);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.Streaming.PoolSize"))->Set(SizeMB);
	}
}

void UMingRTSMemoryOptimizer::RefreshTextureStreaming()
{
	IConsoleManager::Get().FindConsoleVariable(TEXT("r.Streaming.ForceAllMiplevelsToBeResident"))->Set(0);
	GEngine->GetTextureStreamingManager()->UpdateResourceStreaming(0.0f);
}

void UMingRTSMemoryOptimizer::OptimizeMemoryUsage()
{
	UpdateMemoryStats();

	if (CurrentStats.PressureLevel >= EMemoryPressureLevel::Critical)
	{
		// Aggressive cleanup
		StreamOutUnusedAssets(0.0f); // Immediate
		ClearUnusedAssets();
		ForceGarbageCollection();
	}
	else if (CurrentStats.PressureLevel == EMemoryPressureLevel::Elevated)
	{
		// Moderate cleanup
		StreamOutUnusedAssets(30.0f); // 30 seconds
		TriggerIncrementalGC();
	}

	CompactMemory();
}

void UMingRTSMemoryOptimizer::CompactMemory()
{
	// Compact texture streaming
	RefreshTextureStreaming();

	// Request memory compaction from OS
	FPlatformMemory::Trim();

	UE_LOG(LogTemp, Log, TEXT("Memory compaction performed"));
}

void UMingRTSMemoryOptimizer::ClearUnusedAssets()
{
	TArray<FString> AssetsToClear;
	
	for (auto& Pair : StreamingAssets)
	{
		FStreamingAsset& Asset = Pair.Value;
		if (Asset.bIsLoaded && Asset.ReferenceCount == 0 && !Asset.bKeepInMemory)
		{
			AssetsToClear.Add(Pair.Key);
		}
	}

	for (const FString& AssetPath : AssetsToClear)
	{
		UnloadAssetInternal(AssetPath);
	}
}

void UMingRTSMemoryOptimizer::SetAssetKeepInMemory(const FString& AssetPath, bool bKeep)
{
	FScopeLock Lock(&MemoryLock);

	FStreamingAsset* Asset = StreamingAssets.Find(AssetPath);
	if (Asset)
	{
		Asset->bKeepInMemory = bKeep;
	}
}

void UMingRTSMemoryOptimizer::ReferenceAsset(const FString& AssetPath)
{
	FScopeLock Lock(&MemoryLock);

	FStreamingAsset* Asset = StreamingAssets.Find(AssetPath);
	if (Asset)
	{
		Asset->ReferenceCount++;
		Asset->LastAccessTime = FPlatformTime::Seconds();
	}
}

void UMingRTSMemoryOptimizer::DereferenceAsset(const FString& AssetPath)
{
	RequestAssetUnload(AssetPath);
}

FString UMingRTSMemoryOptimizer::GetMemoryReport() const
{
	FScopeLock Lock(&MemoryLock);

	FString Report;
	Report += TEXT("=== Memory Optimizer Report ===\n\n");
	
	Report += FString::Printf(TEXT("Memory Budget: %d MB\n"), CurrentBudget.TotalBudgetMB);
	Report += FString::Printf(TEXT("Current Usage: %d MB\n"), CurrentStats.CurrentMemoryUsageMB);
	Report += FString::Printf(TEXT("Peak Usage: %d MB\n"), CurrentStats.PeakMemoryUsageMB);
	Report += FString::Printf(TEXT("Available: %d MB\n"), CurrentStats.AvailableMemoryMB);
	Report += FString::Printf(TEXT("Pressure Level: %s\n\n"), 
		*UEnum::GetValueAsString(CurrentStats.PressureLevel));
	
	Report += TEXT("Budget Breakdown:\n");
	Report += FString::Printf(TEXT("  Textures: %d MB (Used: %d MB)\n"), 
		CurrentBudget.TextureBudgetMB, CurrentStats.TextureMemoryMB);
	Report += FString::Printf(TEXT("  Meshes: %d MB (Used: %d MB)\n"), 
		CurrentBudget.MeshBudgetMB, CurrentStats.MeshMemoryMB);
	Report += FString::Printf(TEXT("  Animations: %d MB\n"), CurrentBudget.AnimationBudgetMB);
	Report += FString::Printf(TEXT("  Audio: %d MB\n"), CurrentBudget.AudioBudgetMB);
	Report += FString::Printf(TEXT("  Reserved: %d MB\n\n"), CurrentBudget.ReservedBudgetMB);
	
	Report += FString::Printf(TEXT("Streaming Assets: %d loaded, %d streamed\n"),
		CurrentStats.LoadedAssetCount, CurrentStats.StreamedAssetCount);

	return Report;
}

TArray<FString> UMingRTSMemoryOptimizer::GetTopMemoryConsumers(int32 Count) const
{
	FScopeLock Lock(&MemoryLock);

	TArray<TPair<FString, int32>> AssetSizes;
	
	for (const auto& Pair : StreamingAssets)
	{
		if (Pair.Value.bIsLoaded)
		{
			int32 Size = CalculateAssetMemoryUsage(Pair.Value);
			AssetSizes.Add(TPair<FString, int32>(Pair.Key, Size));
		}
	}

	// Sort by size descending
	AssetSizes.Sort([](const TPair<FString, int32>& A, const TPair<FString, int32>& B) {
		return A.Value > B.Value;
	});

	TArray<FString> Result;
	for (int32 i = 0; i < FMath::Min(Count, AssetSizes.Num()); ++i)
	{
		Result.Add(FString::Printf(TEXT("%s (%d KB)"), *AssetSizes[i].Key, AssetSizes[i].Value));
	}

	return Result;
}

void UMingRTSMemoryOptimizer::DumpMemoryUsageToLog()
{
	FString Report = GetMemoryReport();
	UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
	
	TArray<FString> TopConsumers = GetTopMemoryConsumers(10);
	UE_LOG(LogTemp, Log, TEXT("Top 10 Memory Consumers:"));
	for (const FString& Consumer : TopConsumers)
	{
		UE_LOG(LogTemp, Log, TEXT("  %s"), *Consumer);
	}
}

void UMingRTSMemoryOptimizer::OnMemoryMonitorTick()
{
	UpdateMemoryStats();
}

void UMingRTSMemoryOptimizer::OnGCTick()
{
	TriggerIncrementalGC();
}

void UMingRTSMemoryOptimizer::UpdateMemoryPressure()
{
	EMemoryPressureLevel OldLevel = CurrentStats.PressureLevel;

	if (CurrentStats.CurrentMemoryUsageMB >= EmergencyThresholdMB)
	{
		CurrentStats.PressureLevel = EMemoryPressureLevel::Emergency;
	}
	else if (CurrentStats.CurrentMemoryUsageMB >= CriticalThresholdMB)
	{
		CurrentStats.PressureLevel = EMemoryPressureLevel::Critical;
	}
	else if (CurrentStats.CurrentMemoryUsageMB >= ElevatedThresholdMB)
	{
		CurrentStats.PressureLevel = EMemoryPressureLevel::Elevated;
	}
	else
	{
		CurrentStats.PressureLevel = EMemoryPressureLevel::Normal;
	}

	if (CurrentStats.PressureLevel != OldLevel)
	{
		OnMemoryPressureChanged.Broadcast(CurrentStats.PressureLevel);
		
		if (CurrentStats.PressureLevel == EMemoryPressureLevel::Critical)
		{
			OnMemoryWarning.Broadcast();
		}
		else if (CurrentStats.PressureLevel == EMemoryPressureLevel::Emergency)
		{
			OnMemoryCritical.Broadcast();
		}

		// Auto-optimize on pressure change
		HandleMemoryPressure();
	}
}

void UMingRTSMemoryOptimizer::HandleMemoryPressure()
{
	switch (CurrentStats.PressureLevel)
	{
	case EMemoryPressureLevel::Elevated:
		StreamOutUnusedAssets(60.0f);
		break;
		
	case EMemoryPressureLevel::Critical:
		StreamOutUnusedAssets(10.0f);
		TriggerIncrementalGC();
		break;
		
	case EMemoryPressureLevel::Emergency:
		StreamOutUnusedAssets(0.0f);
		ClearUnusedAssets();
		ForceGarbageCollection();
		CompactMemory();
		break;
		
	default:
		break;
	}
}

void UMingRTSMemoryOptimizer::UnloadLowPriorityAssets(int32 TargetMemoryMB)
{
	FScopeLock Lock(&MemoryLock);

	TArray<FString> Candidates;
	
	// Find low priority loaded assets
	for (auto& Pair : StreamingAssets)
	{
		FStreamingAsset& Asset = Pair.Value;
		if (Asset.bIsLoaded && !Asset.bKeepInMemory && Asset.ReferenceCount == 0)
		{
			if (Asset.Priority == ELoadingPriority::Low || 
			    Asset.Priority == ELoadingPriority::Background)
			{
				Candidates.Add(Pair.Key);
			}
		}
	}

	// Sort by last access time (oldest first)
	Candidates.Sort([this](const FString& A, const FString& B) {
		const FStreamingAsset* AssetA = StreamingAssets.Find(A);
		const FStreamingAsset* AssetB = StreamingAssets.Find(B);
		if (AssetA && AssetB)
		{
			return AssetA->LastAccessTime < AssetB->LastAccessTime;
		}
		return false;
	});

	int32 FreedMemoryMB = 0;
	for (const FString& AssetPath : Candidates)
	{
		if (FreedMemoryMB >= TargetMemoryMB)
		{
			break;
		}

		FStreamingAsset* Asset = StreamingAssets.Find(AssetPath);
		if (Asset)
		{
			FreedMemoryMB += CalculateAssetMemoryUsage(*Asset) / 1024;
			UnloadAssetInternal(AssetPath);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Unloaded low priority assets, freed %d MB"), FreedMemoryMB);
}

int32 UMingRTSMemoryOptimizer::CalculateAssetMemoryUsage(const FStreamingAsset& Asset) const
{
	// Estimate based on type
	switch (Asset.ResourceType)
	{
	case EResourceType::Texture:
		return 1024; // 1MB average
	case EResourceType::Mesh:
		return 512; // 512KB average
	case EResourceType::Animation:
		return 256; // 256KB average
	case EResourceType::Audio:
		return 128; // 128KB average
	default:
		return Asset.SizeKB;
	}
}

bool UMingRTSMemoryOptimizer::IsAssetInUse(const FStreamingAsset& Asset) const
{
	return Asset.ReferenceCount > 0 || Asset.bKeepInMemory;
}

void UMingRTSMemoryOptimizer::LoadAssetInternal(const FString& AssetPath)
{
	FStreamingAsset* Asset = StreamingAssets.Find(AssetPath);
	if (!Asset)
	{
		return;
	}

	Asset->bIsLoaded = true;
	Asset->LastUsedTime = FPlatformTime::Seconds();

	// Actual asset loading would happen here via SoftObjectPath or Streaming
	UE_LOG(LogTemp, Verbose, TEXT("Loading asset: %s"), *AssetPath);
}

void UMingRTSMemoryOptimizer::UnloadAssetInternal(const FString& AssetPath)
{
	FStreamingAsset* Asset = StreamingAssets.Find(AssetPath);
	if (!Asset)
	{
		return;
	}

	Asset->bIsLoaded = false;
	Asset->ReferenceCount = 0;

	UE_LOG(LogTemp, Verbose, TEXT("Unloading asset: %s"), *AssetPath);
}
