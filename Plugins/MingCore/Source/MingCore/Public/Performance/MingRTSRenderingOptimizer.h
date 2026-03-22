// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingRTSRenderingOptimizer.generated.h"

UENUM(BlueprintType)
enum class ECullingMethod: uint8 {
	None,
	Frustum,
	Occlusion,
	Distance,
	Combined
};

UENUM(BlueprintType)
enum class EInstancingMethod: uint8 {
	None,
	StaticMesh,
	HISM,
	GPUInstancing
};

USTRUCT(BlueprintType)
struct FCullingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Culling")
	bool bEnableFrustumCulling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Culling")
	bool bEnableOcclusionCulling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Culling")
	bool bEnableDistanceCulling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Culling")
	float FrustumMargin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Culling")
	float OcclusionSlop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Culling")
	float NearCullDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Culling")
	float FarCullDistance;

	FCullingSettings()
		: bEnableFrustumCulling(true)
		, bEnableOcclusionCulling(true)
		, bEnableDistanceCulling(true)
		, FrustumMargin(100.0f)
		, OcclusionSlop(0.8f)
		, NearCullDistance(100.0f)
		, FarCullDistance(20000.0f)
	{}
};

USTRUCT(BlueprintType)
struct FInstancingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instancing")
	EInstancingMethod Method;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instancing")
	int32 MinInstancesToBatch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instancing")
	int32 MaxInstancesPerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instancing")
	bool bEnableLODSupport;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instancing")
	bool bEnableCollision;

	FInstancingSettings()
		: Method(EInstancingMethod::HISM)
		, MinInstancesToBatch(10)
		, MaxInstancesPerComponent(1000)
		, bEnableLODSupport(true)
		, bEnableCollision(false)
	{}
};

USTRUCT(BlueprintType)
struct FRenderingStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 VisibleObjects;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 CulledObjects;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 InstancedObjects;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 DrawCalls;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float FrameTime;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float CullingTime;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 OcclusionQueries;

	FRenderingStats()
		: VisibleObjects(0)
		, CulledObjects(0)
		, InstancedObjects(0)
		, DrawCalls(0)
		, FrameTime(0.0f)
		, CullingTime(0.0f)
		, OcclusionQueries(0)
	{}
};

USTRUCT(BlueprintType)
struct FInstancedMeshGroup
{
	GENERATED_BODY()

	UPROPERTY()
	UStaticMesh* Mesh;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	TArray<FTransform> Instances;

	UPROPERTY()
	int32 InstanceCount;

	FInstancedMeshGroup()
		: Mesh(nullptr)
		, Material(nullptr)
		, InstanceCount(0)
	{}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCullingCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInstancingCompleted);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGCORE_API UMingRTSRenderingOptimizer : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMingRTSRenderingOptimizer();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Culling System
	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void SetCullingSettings(const FCullingSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "Rendering Optimizer")
	FCullingSettings GetCullingSettings() const;

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void PerformCulling();

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	bool IsObjectVisible(AActor* Object) const;

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void RegisterCullingObject(AActor* Object);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void UnregisterCullingObject(AActor* Object);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void SetObjectCullingEnabled(AActor* Object, bool bEnabled);

	// Instancing System
	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void SetInstancingSettings(const FInstancingSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "Rendering Optimizer")
	FInstancingSettings GetInstancingSettings() const;

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void BatchStaticMeshes(const TArray<AActor*>& MeshActors);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void CreateHISMComponent(UStaticMesh* Mesh, const TArray<FTransform>& Instances, USceneComponent* Parent);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void ClearInstancedComponents();

	// Performance Monitoring
	UFUNCTION(BlueprintPure, Category = "Rendering Optimizer")
	FRenderingStats GetRenderingStats() const;

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void ResetRenderingStats();

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void UpdateRenderingStats();

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	FString GetRenderingReport() const;

	// Optimization Controls
	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void EnableOcclusionCulling(bool bEnable);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void EnableInstancing(bool bEnable);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void SetMaxDrawCallsPerFrame(int32 MaxDrawCalls);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void SetTargetFrameTime(float TargetMS);

	// Level of Detail
	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void SetGlobalLODScale(float Scale);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void ForceLowestLOD(bool bForce);

	// Shadow Optimization
	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void OptimizeShadowSettings();

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void SetShadowDistance(float Distance);

	// Debug
	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void ShowCullingDebug(bool bShow);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void ShowInstancingDebug(bool bShow);

	UFUNCTION(BlueprintCallable, Category = "Rendering Optimizer")
	void ToggleWireframeMode();

public:
	UPROPERTY(BlueprintAssignable, Category = "Rendering Optimizer|Events")
	FOnCullingCompleted OnCullingCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Rendering Optimizer|Events")
	FOnInstancingCompleted OnInstancingCompleted;

protected:
	UPROPERTY()
	FCullingSettings CullingSettings;

	UPROPERTY()
	FInstancingSettings InstancingSettings;

	UPROPERTY()
	FRenderingStats Stats;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> RegisteredObjects;

	UPROPERTY()
	TArray<UInstancedStaticMeshComponent*> InstancedComponents;

	UPROPERTY()
	FTimerHandle CullingTimerHandle;

	UPROPERTY()
	FTimerHandle StatsTimerHandle;

	int32 MaxDrawCalls;
	float TargetFrameTime;
	bool bInstancingEnabled;
	float GlobalLODScale;
	bool bForceLowestLODMode;

	mutable FCriticalSection RenderingLock;

protected:
	void OnCullingTick();
	void OnStatsTick();
	void PerformFrustumCulling();
	void PerformOcclusionCulling();
	void PerformDistanceCulling();
	bool IsInFrustum(const FVector& Location, const FVector& Extent) const;
	bool IsOccluded(const FVector& Location) const;
	void GroupMeshesForInstancing(const TArray<AActor*>& MeshActors, TMap<FString, FInstancedMeshGroup>& OutGroups);
	UInstancedStaticMeshComponent* CreateISMComponent(const FInstancedMeshGroup& Group, USceneComponent* Parent);
	void UpdateDrawCallStats();
	void AutoAdjustQuality();
};
