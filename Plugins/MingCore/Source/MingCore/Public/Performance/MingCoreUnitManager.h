// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingRTSUnitManager.generated.h"

UENUM(BlueprintType)
enum class EUnitLODLevel : uint8
{
	LOD0,    // Full detail - close units
	LOD1,    // Medium detail
	LOD2,    // Low detail
	LOD3,    // Very low detail
	Impostor // Billboard impostor
};

UENUM(BlueprintType)
enum class EUnitPoolType : uint8
{
	Infantry,
	Cavalry,
	Archer,
	Siege,
	Hero,
	Structure,
	Custom
};

USTRUCT(BlueprintType)
struct FUnitLODConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
	EUnitLODLevel LODLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
	float DistanceThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
	float AnimationUpdateRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
	bool bEnableAI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
	bool bEnableCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
	bool bEnableShadows;

	FUnitLODConfig()
		: LODLevel(EUnitLODLevel::LOD0)
		, DistanceThreshold(0.0f)
		, AnimationUpdateRate(1.0f)
		, bEnableAI(true)
		, bEnableCollision(true)
		, bEnableShadows(true)
	{}
};

USTRUCT(BlueprintType)
struct FUnitPoolConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	EUnitPoolType PoolType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	TSubclassOf<AActor> UnitClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	int32 InitialPoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	int32 MaxPoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	bool bGrowable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	TArray<FUnitLODConfig> LODConfigs;

	FUnitPoolConfig()
		: PoolType(EUnitPoolType::Infantry)
		, InitialPoolSize(100)
		, MaxPoolSize(500)
		, bGrowable(true)
	{}
};

USTRUCT(BlueprintType)
struct FPooledUnit
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* UnitActor;

	UPROPERTY()
	EUnitPoolType PoolType;

	UPROPERTY()
	EUnitLODLevel CurrentLOD;

	UPROPERTY()
	bool bActive;

	UPROPERTY()
	float LastUsedTime;

	UPROPERTY()
	int32 PoolIndex;

	FPooledUnit()
		: UnitActor(nullptr)
		, PoolType(EUnitPoolType::Infantry)
		, CurrentLOD(EUnitLODLevel::LOD0)
		, bActive(false)
		, LastUsedTime(0.0f)
		, PoolIndex(-1)
	{}
};

USTRUCT(BlueprintType)
struct FUnitManagerStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 TotalActiveUnits;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 TotalPooledUnits;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 TotalUnitsByLOD[5];

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float AverageUnitsPerFrame;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 PoolHits;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 PoolMisses;

	FUnitManagerStats()
		: TotalActiveUnits(0)
		, TotalPooledUnits(0)
		, AverageUnitsPerFrame(0.0f)
		, PoolHits(0)
		, PoolMisses(0)
	{
		for (int32 i = 0; i < 5; ++i)
		{
			TotalUnitsByLOD[i] = 0;
		}
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitLODChanged, AActor*, Unit, EUnitLODLevel, NewLOD);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitSpawned, AActor*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDespawned, AActor*, Unit);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGCORE_API UMingRTSUnitManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMingRTSUnitManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Pool Management
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void RegisterPoolConfig(const FUnitPoolConfig& Config);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void InitializePools();

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void ClearAllPools();

	// Unit Spawning
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	AActor* SpawnUnit(EUnitPoolType PoolType, const FTransform& Transform, AActor* Owner = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void DespawnUnit(AActor* Unit);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void DespawnAllUnitsOfType(EUnitPoolType PoolType);

	// LOD Management
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void UpdateLODSystem();

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void SetUnitLOD(AActor* Unit, EUnitLODLevel NewLOD);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	EUnitLODLevel GetUnitLOD(AActor* Unit) const;

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void SetLODUpdateInterval(float Interval);

	// Batch Operations
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	TArray<AActor*> SpawnUnitsBatch(EUnitPoolType PoolType, const TArray<FTransform>& Transforms, AActor* Owner = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void DespawnUnitsBatch(const TArray<AActor*>& Units);

	// Spatial Management
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	TArray<AActor*> GetUnitsInRadius(const FVector& Center, float Radius, EUnitPoolType TypeFilter = EUnitPoolType::Custom);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	int32 GetUnitCountInRadius(const FVector& Center, float Radius) const;

	// Statistics
	UFUNCTION(BlueprintPure, Category = "Unit Manager")
	FUnitManagerStats GetStats() const;

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void ResetStats();

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	FString GetPerformanceReport() const;

	// Optimization Settings
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void SetMaxActiveUnits(int32 MaxUnits);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void EnableDistanceCulling(bool bEnable);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void SetCullingDistance(float Distance);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void EnableFrustumCulling(bool bEnable);

	// Memory Management
	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void CompactPools();

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	void TrimInactivePools(float InactiveTimeThreshold);

	UFUNCTION(BlueprintCallable, Category = "Unit Manager")
	int32 GetTotalMemoryUsage() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Unit Manager|Events")
	FOnUnitLODChanged OnUnitLODChanged;

	UPROPERTY(BlueprintAssignable, Category = "Unit Manager|Events")
	FOnUnitSpawned OnUnitSpawned;

	UPROPERTY(BlueprintAssignable, Category = "Unit Manager|Events")
	FOnUnitDespawned OnUnitDespawned;

protected:
	UPROPERTY()
	TMap<EUnitPoolType, FUnitPoolConfig> PoolConfigs;

	UPROPERTY()
	TMap<EUnitPoolType, TArray<FPooledUnit>> UnitPools;

	UPROPERTY()
	TMap<AActor*, FPooledUnit*> ActiveUnits;

	UPROPERTY()
	TMap<AActor*, EUnitLODLevel> UnitLODMap;

	UPROPERTY()
	FUnitManagerStats Stats;

	UPROPERTY()
	FTimerHandle LODUpdateTimerHandle;

	UPROPERTY()
	FTimerHandle CullingTimerHandle;

	float LODUpdateInterval;
	int32 MaxActiveUnits;
	bool bDistanceCullingEnabled;
	float CullingDistance;
	bool bFrustumCullingEnabled;
	
	mutable FCriticalSection UnitLock;

protected:
	AActor* AcquireUnitFromPool(EUnitPoolType PoolType);
	void ReleaseUnitToPool(AActor* Unit);
	void GrowPool(EUnitPoolType PoolType);
	void ApplyLODSettings(AActor* Unit, const FUnitLODConfig& LODConfig);
	FUnitLODLevel CalculateLODLevel(AActor* Unit);
	void PerformDistanceCulling();
	void PerformFrustumCulling();
	void OnLODUpdateTick();
	void OnCullingTick();
	bool IsInFrustum(const FVector& Location) const;
};
