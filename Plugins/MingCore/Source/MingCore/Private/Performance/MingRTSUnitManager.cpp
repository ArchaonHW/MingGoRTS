// Copyright Epic Games, Inc. All Rights Reserved.

#include "Performance/MingRTSUnitManager.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"

UMingRTSUnitManager::UMingRTSUnitManager()
	: LODUpdateInterval(0.5f)
	, MaxActiveUnits(2000)
	, bDistanceCullingEnabled(true)
	, CullingDistance(10000.0f)
	, bFrustumCullingEnabled(true)
{
}

void UMingRTSUnitManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Start LOD update timer
	if (GetGameInstance())
	{
		FTimerDelegate LODDelegate;
		LODDelegate.BindUObject(this, &UMingRTSUnitManager::OnLODUpdateTick);
		GetGameInstance()->GetTimerManager().SetTimer(LODUpdateTimerHandle, LODDelegate, LODUpdateInterval, true);

		FTimerDelegate CullingDelegate;
		CullingDelegate.BindUObject(this, &UMingRTSUnitManager::OnCullingTick);
		GetGameInstance()->GetTimerManager().SetTimer(CullingTimerHandle, CullingDelegate, 1.0f, true);
	}

	UE_LOG(LogTemp, Log, TEXT("RTS Unit Manager initialized"));
}

void UMingRTSUnitManager::Deinitialize()
{
	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(LODUpdateTimerHandle);
		GetGameInstance()->GetTimerManager().ClearTimer(CullingTimerHandle);
	}

	ClearAllPools();

	Super::Deinitialize();
}

void UMingRTSUnitManager::RegisterPoolConfig(const FUnitPoolConfig& Config)
{
	PoolConfigs.Add(Config.PoolType, Config);
}

void UMingRTSUnitManager::InitializePools()
{
	for (const auto& Pair : PoolConfigs)
	{
		const FUnitPoolConfig& Config = Pair.Value;
		TArray<FPooledUnit>& Pool = UnitPools.FindOrAdd(Pair.Key);

		// Pre-populate pool
		for (int32 i = 0; i < Config.InitialPoolSize; ++i)
		{
			if (Config.UnitClass && GetWorld())
			{
				AActor* Unit = GetWorld()->SpawnActor<AActor>(Config.UnitClass);
				if (Unit)
				{
					FPooledUnit PooledUnit;
					PooledUnit.UnitActor = Unit;
					PooledUnit.PoolType = Config.PoolType;
	ooledUnit.PoolIndex = i;
					PooledUnit.bActive = false;
					Unit->SetActorHiddenInGame(true);
					Unit->SetActorEnableCollision(false);
					Pool.Add(PooledUnit);
				}
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Initialized %d unit pools"), PoolConfigs.Num());
}

void UMingRTSUnitManager::ClearAllPools()
{
	for (auto& Pair : ActiveUnits)
	{
		if (Pair.Key && IsValid(Pair.Key))
		{
			Pair.Key->Destroy();
		}
	}
	ActiveUnits.Empty();
	UnitLODMap.Empty();

	for (auto& Pair : UnitPools)
	{
		for (FPooledUnit& PooledUnit : Pair.Value)
		{
			if (PooledUnit.UnitActor && IsValid(PooledUnit.UnitActor))
			{
				PooledUnit.UnitActor->Destroy();
			}
		}
	}
	UnitPools.Empty();
}

AActor* UMingRTSUnitManager::SpawnUnit(EUnitPoolType PoolType, const FTransform& Transform, AActor* Owner)
{
	FScopeLock Lock(&UnitLock);

	// Check max active units limit
	if (ActiveUnits.Num() >= MaxActiveUnits)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max active units limit reached: %d"), MaxActiveUnits);
		return nullptr;
	}

	AActor* Unit = AcquireUnitFromPool(PoolType);
	if (Unit)
	{
		Unit->SetActorTransform(Transform);
		Unit->SetActorHiddenInGame(false);
		Unit->SetActorEnableCollision(true);

		if (Owner)
		{
			Unit->SetOwner(Owner);
		}

		// Set initial LOD
		EUnitLODLevel InitialLOD = CalculateLODLevel(Unit);
		SetUnitLOD(Unit, InitialLOD);

		OnUnitSpawned.Broadcast(Unit);

		UE_LOG(LogTemp, Verbose, TEXT("Spawned unit of type %s"), *UEnum::GetValueAsString(PoolType));
	}

	return Unit;
}

void UMingRTSUnitManager::DespawnUnit(AActor* Unit)
{
	if (!Unit)
	{
		return;
	}

	FScopeLock Lock(&UnitLock);

	ReleaseUnitToPool(Unit);

	OnUnitDespawned.Broadcast(Unit);
}

void UMingRTSUnitManager::DespawnAllUnitsOfType(EUnitPoolType PoolType)
{
	TArray<AActor*> UnitsToDespawn;
	
	for (const auto& Pair : ActiveUnits)
	{
		if (Pair.Value && Pair.Value->PoolType == PoolType)
		{
			UnitsToDespawn.Add(Pair.Key);
		}
	}

	for (AActor* Unit : UnitsToDespawn)
	{
		DespawnUnit(Unit);
	}
}

void UMingRTSUnitManager::UpdateLODSystem()
{
	FScopeLock Lock(&UnitLock);

	for (const auto& Pair : ActiveUnits)
	{
		AActor* Unit = Pair.Key;
		if (Unit && IsValid(Unit))
		{
			EUnitLODLevel NewLOD = CalculateLODLevel(Unit);
			EUnitLODLevel CurrentLOD = UnitLODMap.FindRef(Unit);

			if (NewLOD != CurrentLOD)
			{
				SetUnitLOD(Unit, NewLOD);
			}
		}
	}
}

void UMingRTSUnitManager::SetUnitLOD(AActor* Unit, EUnitLODLevel NewLOD)
{
	if (!Unit)
	{
		return;
	}

	EUnitLODLevel CurrentLOD = UnitLODMap.FindRef(Unit);
	if (CurrentLOD == NewLOD)
	{
		return;
	}

	UnitLODMap.Add(Unit, NewLOD);

	FPooledUnit* PooledUnit = ActiveUnits.FindRef(Unit);
	if (PooledUnit)
	{
		PooledUnit->CurrentLOD = NewLOD;

		FUnitPoolConfig* Config = PoolConfigs.Find(PooledUnit->PoolType);
		if (Config && Config->LODConfigs.IsValidIndex(static_cast<int32>(NewLOD)))
		{
			ApplyLODSettings(Unit, Config->LODConfigs[static_cast<int32>(NewLOD)]);
		}
	}

	OnUnitLODChanged.Broadcast(Unit, NewLOD);
}

EUnitLODLevel UMingRTSUnitManager::GetUnitLOD(AActor* Unit) const
{
	return UnitLODMap.FindRef(Unit);
}

void UMingRTSUnitManager::SetLODUpdateInterval(float Interval)
{
	LODUpdateInterval = FMath::Max(0.1f, Interval);

	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(LODUpdateTimerHandle);
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UMingRTSUnitManager::OnLODUpdateTick);
		GetGameInstance()->GetTimerManager().SetTimer(LODUpdateTimerHandle, Delegate, LODUpdateInterval, true);
	}
}

TArray<AActor*> UMingRTSUnitManager::SpawnUnitsBatch(EUnitPoolType PoolType, const TArray<FTransform>& Transforms, AActor* Owner)
{
	TArray<AActor*> SpawnedUnits;
	SpawnedUnits.Reserve(Transforms.Num());

	for (const FTransform& Transform : Transforms)
	{
		AActor* Unit = SpawnUnit(PoolType, Transform, Owner);
		if (Unit)
		{
			SpawnedUnits.Add(Unit);
		}
	}

	return SpawnedUnits;
}

void UMingRTSUnitManager::DespawnUnitsBatch(const TArray<AActor*>& Units)
{
	for (AActor* Unit : Units)
	{
		DespawnUnit(Unit);
	}
}

TArray<AActor*> UMingRTSUnitManager::GetUnitsInRadius(const FVector& Center, float Radius, EUnitPoolType TypeFilter)
{
	TArray<AActor*> UnitsInRadius;
	float RadiusSquared = Radius * Radius;

	for (const auto& Pair : ActiveUnits)
	{
		AActor* Unit = Pair.Key;
		if (Unit && IsValid(Unit))
		{
			if (TypeFilter != EUnitPoolType::Custom && Pair.Value->PoolType != TypeFilter)
			{
				continue;
			}

			float DistanceSquared = FVector::DistSquared(Unit->GetActorLocation(), Center);
			if (DistanceSquared <= RadiusSquared)
			{
				UnitsInRadius.Add(Unit);
			}
		}
	}

	return UnitsInRadius;
}

int32 UMingRTSUnitManager::GetUnitCountInRadius(const FVector& Center, float Radius) const
{
	int32 Count = 0;
	float RadiusSquared = Radius * Radius;

	for (const auto& Pair : ActiveUnits)
	{
		AActor* Unit = Pair.Key;
		if (Unit && IsValid(Unit))
		{
			float DistanceSquared = FVector::DistSquared(Unit->GetActorLocation(), Center);
			if (DistanceSquared <= RadiusSquared)
			{
				Count++;
			}
		}
	}

	return Count;
}

FUnitManagerStats UMingRTSUnitManager::GetStats() const
{
	FScopeLock Lock(&UnitLock);

	FUnitManagerStats CurrentStats = Stats;
	CurrentStats.TotalActiveUnits = ActiveUnits.Num();
	CurrentStats.TotalPooledUnits = 0;

	for (const auto& Pair : UnitPools)
	{
		CurrentStats.TotalPooledUnits += Pair.Value.Num();
	}

	// Count units by LOD
	for (int32 i = 0; i < 5; ++i)
	{
		CurrentStats.TotalUnitsByLOD[i] = 0;
	}

	for (const auto& Pair : UnitLODMap)
	{
		int32 LODIndex = static_cast<int32>(Pair.Value);
		if (LODIndex >= 0 && LODIndex < 5)
		{
			CurrentStats.TotalUnitsByLOD[LODIndex]++;
		}
	}

	return CurrentStats;
}

void UMingRTSUnitManager::ResetStats()
{
	Stats = FUnitManagerStats();
}

FString UMingRTSUnitManager::GetPerformanceReport() const
{
	FUnitManagerStats CurrentStats = GetStats();

	FString Report;
	Report += TEXT("=== RTS Unit Manager Performance Report ===\n\n");
	Report += FString::Printf(TEXT("Active Units: %d/%d\n"), CurrentStats.TotalActiveUnits, MaxActiveUnits);
	Report += FString::Printf(TEXT("Pooled Units: %d\n"), CurrentStats.TotalPooledUnits);
	Report += FString::Printf(TEXT("Pool Efficiency: %.2f%%\n"), 
		CurrentStats.PoolHits / (float)FMath::Max(1, CurrentStats.PoolHits + CurrentStats.PoolMisses) * 100.0f);
	
	Report += TEXT("\nUnits by LOD Level:\n");
	Report += FString::Printf(TEXT("  LOD0 (Full): %d\n"), CurrentStats.TotalUnitsByLOD[0]);
	Report += FString::Printf(TEXT("  LOD1 (Medium): %d\n"), CurrentStats.TotalUnitsByLOD[1]);
	Report += FString::Printf(TEXT("  LOD2 (Low): %d\n"), CurrentStats.TotalUnitsByLOD[2]);
	Report += FString::Printf(TEXT("  LOD3 (Very Low): %d\n"), CurrentStats.TotalUnitsByLOD[3]);
	Report += FString::Printf(TEXT("  Impostor: %d\n"), CurrentStats.TotalUnitsByLOD[4]);

	return Report;
}

void UMingRTSUnitManager::SetMaxActiveUnits(int32 MaxUnits)
{
	MaxActiveUnits = FMath::Max(100, MaxUnits);
}

void UMingRTSUnitManager::EnableDistanceCulling(bool bEnable)
{
	bDistanceCullingEnabled = bEnable;
}

void UMingRTSUnitManager::SetCullingDistance(float Distance)
{
	CullingDistance = FMath::Max(1000.0f, Distance);
}

void UMingRTSUnitManager::EnableFrustumCulling(bool bEnable)
{
	bFrustumCullingEnabled = bEnable;
}

void UMingRTSUnitManager::CompactPools()
{
	for (auto& Pair : UnitPools)
	{
		TArray<FPooledUnit>& Pool = Pair.Value;
		for (int32 i = Pool.Num() - 1; i >= 0; --i)
		{
			if (!Pool[i].bActive && Pool[i].UnitActor)
			{
				// Keep inactive units in pool but hidden
				Pool[i].UnitActor->SetActorHiddenInGame(true);
			}
		}
	}
}

void UMingRTSUnitManager::TrimInactivePools(float InactiveTimeThreshold)
{
	float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

	for (auto& Pair : UnitPools)
	{
		TArray<FPooledUnit>& Pool = Pair.Value;
		for (int32 i = Pool.Num() - 1; i >= 0; --i)
		{
			if (!Pool[i].bActive && (CurrentTime - Pool[i].LastUsedTime) > InactiveTimeThreshold)
			{
				if (Pool[i].UnitActor && IsValid(Pool[i].UnitActor))
				{
					Pool[i].UnitActor->Destroy();
				}
				Pool.RemoveAt(i);
			}
		}
	}
}

int32 UMingRTSUnitManager::GetTotalMemoryUsage() const
{
	int32 MemoryUsage = 0;
	MemoryUsage += ActiveUnits.Num() * sizeof(FPooledUnit);
	MemoryUsage += UnitLODMap.Num() * (sizeof(AActor*) + sizeof(EUnitLODLevel));

	for (const auto& Pair : UnitPools)
	{
		MemoryUsage += Pair.Value.Num() * sizeof(FPooledUnit);
	}

	return MemoryUsage;
}

AActor* UMingRTSUnitManager::AcquireUnitFromPool(EUnitPoolType PoolType)
{
	TArray<FPooledUnit>* Pool = UnitPools.Find(PoolType);
	if (!Pool)
	{
		// Try to grow pool
		GrowPool(PoolType);
		Pool = UnitPools.Find(PoolType);
	}

	if (Pool)
	{
		for (FPooledUnit& PooledUnit : *Pool)
		{
			if (!PooledUnit.bActive)
			{
				PooledUnit.bActive = true;
				PooledUnit.LastUsedTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
				ActiveUnits.Add(PooledUnit.UnitActor, &PooledUnit);
				Stats.PoolHits++;
				return PooledUnit.UnitActor;
			}
		}

		// Pool exhausted, try to grow
		GrowPool(PoolType);
		
		// Try again after growing
		for (FPooledUnit& PooledUnit : *Pool)
		{
			if (!PooledUnit.bActive)
			{
				PooledUnit.bActive = true;
				PooledUnit.LastUsedTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
				ActiveUnits.Add(PooledUnit.UnitActor, &PooledUnit);
				Stats.PoolHits++;
				return PooledUnit.UnitActor;
			}
		}
	}

	Stats.PoolMisses++;
	return nullptr;
}

void UMingRTSUnitManager::ReleaseUnitToPool(AActor* Unit)
{
	FPooledUnit* PooledUnit = ActiveUnits.FindRef(Unit);
	if (PooledUnit)
	{
		PooledUnit->bActive = false;
		PooledUnit->LastUsedTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
		
		Unit->SetActorHiddenInGame(true);
		Unit->SetActorEnableCollision(false);
		Unit->SetActorLocation(FVector(0, 0, -10000)); // Move out of view

		ActiveUnits.Remove(Unit);
		UnitLODMap.Remove(Unit);
	}
}

void UMingRTSUnitManager::GrowPool(EUnitPoolType PoolType)
{
	FUnitPoolConfig* Config = PoolConfigs.Find(PoolType);
	if (!Config || !Config->bGrowable)
	{
		return;
	}

	TArray<FPooledUnit>& Pool = UnitPools.FindOrAdd(PoolType);
	int32 CurrentSize = Pool.Num();
	int32 GrowAmount = FMath::Min(50, Config->MaxPoolSize - CurrentSize);

	if (GrowAmount <= 0)
	{
		return;
	}

	for (int32 i = 0; i < GrowAmount; ++i)
	{
		if (Config->UnitClass && GetWorld())
		{
			AActor* Unit = GetWorld()->SpawnActor<AActor>(Config->UnitClass);
			if (Unit)
			{
				FPooledUnit PooledUnit;
				PooledUnit.UnitActor = Unit;
				PooledUnit.PoolType = Config->PoolType;
				PooledUnit.PoolIndex = CurrentSize + i;
				PooledUnit.bActive = false;
				Unit->SetActorHiddenInGame(true);
				Unit->SetActorEnableCollision(false);
				Pool.Add(PooledUnit);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Grew pool %s from %d to %d units"), 
		*UEnum::GetValueAsString(PoolType), CurrentSize, Pool.Num());
}

void UMingRTSUnitManager::ApplyLODSettings(AActor* Unit, const FUnitLODConfig& LODConfig)
{
	if (!Unit)
	{
		return;
	}

	// Apply animation update rate
	USkeletalMeshComponent* SkeletalMesh = Unit->FindComponentByClass<USkeletalMeshComponent>();
	if (SkeletalMesh)
	{
		SkeletalMesh->bEnableUpdateRateOptimizations = true;
		SkeletalMesh->SkinnedMeshComponent::bPerBoneMotionBlur = LODConfig.LODLevel == EUnitLODLevel::LOD0;
	}

	// Enable/disable collision
	Unit->SetActorEnableCollision(LODConfig.bEnableCollision);

	// Note: Shadow and AI settings would be applied through the unit's controller/component
}

EUnitLODLevel UMingRTSUnitManager::CalculateLODLevel(AActor* Unit)
{
	if (!Unit || !GetWorld())
	{
		return EUnitLODLevel::LOD0;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		return EUnitLODLevel::LOD0;
	}

	FVector CameraLocation = PC->PlayerCameraManager ? PC->PlayerCameraManager->GetCameraLocation() : PC->GetPawn() ? PC->GetPawn()->GetActorLocation() : FVector::ZeroVector;
	float Distance = FVector::Dist(Unit->GetActorLocation(), CameraLocation);

	FUnitPoolConfig* Config = nullptr;
	FPooledUnit* PooledUnit = ActiveUnits.FindRef(Unit);
	if (PooledUnit)
	{
		Config = PoolConfigs.Find(PooledUnit->PoolType);
	}

	if (Config)
	{
		for (int32 i = Config->LODConfigs.Num() - 1; i >= 0; --i)
		{
			if (Distance >= Config->LODConfigs[i].DistanceThreshold)
			{
				return static_cast<EUnitLODLevel>(i);
			}
		}
	}

	// Default LOD based on distance
	if (Distance > 8000.0f) return EUnitLODLevel::Impostor;
	if (Distance > 5000.0f) return EUnitLODLevel::LOD3;
	if (Distance > 2500.0f) return EUnitLODLevel::LOD2;
	if (Distance > 1000.0f) return EUnitLODLevel::LOD1;
	return EUnitLODLevel::LOD0;
}

void UMingRTSUnitManager::PerformDistanceCulling()
{
	if (!bDistanceCullingEnabled || !GetWorld())
	{
		return;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		return;
	}

	FVector CameraLocation = PC->PlayerCameraManager ? PC->PlayerCameraManager->GetCameraLocation() : PC->GetPawn() ? PC->GetPawn()->GetActorLocation() : FVector::ZeroVector;
	float CullDistanceSquared = CullingDistance * CullingDistance;

	TArray<AActor*> UnitsToCull;

	for (const auto& Pair : ActiveUnits)
	{
		AActor* Unit = Pair.Key;
		if (Unit && IsValid(Unit))
		{
			float DistanceSquared = FVector::DistSquared(Unit->GetActorLocation(), CameraLocation);
			if (DistanceSquared > CullDistanceSquared)
			{
				UnitsToCull.Add(Unit);
			}
		}
	}

	// Hide culled units instead of despawning
	for (AActor* Unit : UnitsToCull)
	{
		if (Unit)
		{
			Unit->SetActorHiddenInGame(true);
		}
	}
}

void UMingRTSUnitManager::PerformFrustumCulling()
{
	if (!bFrustumCullingEnabled || !GetWorld())
	{
		return;
	}

	// Implementation would check if units are within camera frustum
	// For now, simplified version
	for (const auto& Pair : ActiveUnits)
	{
		AActor* Unit = Pair.Key;
		if (Unit && IsValid(Unit) && Unit->IsHidden())
		{
			// Check if unit should be visible
			if (IsInFrustum(Unit->GetActorLocation()))
			{
				Unit->SetActorHiddenInGame(false);
			}
		}
	}
}

void UMingRTSUnitManager::OnLODUpdateTick()
{
	UpdateLODSystem();
}

void UMingRTSUnitManager::OnCullingTick()
{
	PerformDistanceCulling();
	PerformFrustumCulling();
}

bool UMingRTSUnitManager::IsInFrustum(const FVector& Location) const
{
	if (!GetWorld())
	{
		return true;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !PC->PlayerCameraManager)
	{
		return true;
	}

	// Simplified frustum check - check if within reasonable angle of camera forward
	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
	FVector CameraForward = CameraRotation.Vector();

	FVector ToLocation = (Location - CameraLocation).GetSafeNormal();
	float DotProduct = FVector::DotProduct(CameraForward, ToLocation);

	// If within 90 degree cone (cos(90) = 0)
	return DotProduct > 0.0f;
}
