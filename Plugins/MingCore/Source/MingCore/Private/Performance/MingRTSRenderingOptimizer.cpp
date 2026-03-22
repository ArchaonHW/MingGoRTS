// Copyright Epic Games, Inc. All Rights Reserved.

#include "Performance/MingRTSRenderingOptimizer.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

UMingRTSRenderingOptimizer::UMingRTSRenderingOptimizer()
	: MaxDrawCalls(1000)
	, TargetFrameTime(16.67f)
	, bInstancingEnabled(true)
	, GlobalLODScale(1.0f)
	, bForceLowestLODMode(false)
{
	CullingSettings = FCullingSettings();
	InstancingSettings = FInstancingSettings();
}

void UMingRTSRenderingOptimizer::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GetGameInstance())
	{
		// Start culling timer
		FTimerDelegate CullingDelegate;
		CullingDelegate.BindUObject(this, &UMingRTSRenderingOptimizer::OnCullingTick);
		GetGameInstance()->GetTimerManager().SetTimer(CullingTimerHandle, CullingDelegate, 0.1f, true);

		// Start stats timer
		FTimerDelegate StatsDelegate;
		StatsDelegate.BindUObject(this, &UMingRTSRenderingOptimizer::OnStatsTick);
		GetGameInstance()->GetTimerManager().SetTimer(StatsTimerHandle, StatsDelegate, 1.0f, true);
	}

	UE_LOG(LogTemp, Log, TEXT("RTS Rendering Optimizer initialized"));
}

void UMingRTSRenderingOptimizer::Deinitialize()
{
	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(CullingTimerHandle);
		GetGameInstance()->GetTimerManager().ClearTimer(StatsTimerHandle);
	}

	ClearInstancedComponents();
	RegisteredObjects.Empty();

	Super::Deinitialize();
}

void UMingRTSRenderingOptimizer::SetCullingSettings(const FCullingSettings& Settings)
{
	CullingSettings = Settings;
}

FCullingSettings UMingRTSRenderingOptimizer::GetCullingSettings() const
{
	return CullingSettings;
}

void UMingRTSRenderingOptimizer::PerformCulling()
{
	double StartTime = FPlatformTime::Seconds();

	if (CullingSettings.bEnableFrustumCulling)
	{
		PerformFrustumCulling();
	}

	if (CullingSettings.bEnableOcclusionCulling)
	{
		PerformOcclusionCulling();
	}

	if (CullingSettings.bEnableDistanceCulling)
	{
		PerformDistanceCulling();
	}

	Stats.CullingTime = (FPlatformTime::Seconds() - StartTime) * 1000.0f;

	OnCullingCompleted.Broadcast();
}

bool UMingRTSRenderingOptimizer::IsObjectVisible(AActor* Object) const
{
	if (!Object)
	{
		return false;
	}

	FVector Location = Object->GetActorLocation();
	FVector Extent = Object->GetRootComponent() ? Object->GetRootComponent()->Bounds.BoxExtent : FVector::ZeroVector;

	if (CullingSettings.bEnableFrustumCulling && !IsInFrustum(Location, Extent))
	{
		return false;
	}

	if (CullingSettings.bEnableOcclusionCulling && IsOccluded(Location))
	{
		return false;
	}

	if (CullingSettings.bEnableDistanceCulling)
	{
		APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
		if (PC)
		{
			FVector CameraLocation = PC->PlayerCameraManager ? PC->PlayerCameraManager->GetCameraLocation() : PC->GetPawn() ? PC->GetPawn()->GetActorLocation() : FVector::ZeroVector;
			float Distance = FVector::Dist(Location, CameraLocation);
			
			if (Distance < CullingSettings.NearCullDistance || Distance > CullingSettings.FarCullDistance)
			{
				return false;
			}
		}
	}

	return true;
}

void UMingRTSRenderingOptimizer::RegisterCullingObject(AActor* Object)
{
	if (Object)
	{
		RegisteredObjects.AddUnique(Object);
	}
}

void UMingRTSRenderingOptimizer::UnregisterCullingObject(AActor* Object)
{
	RegisteredObjects.Remove(Object);
}

void UMingRTSRenderingOptimizer::SetObjectCullingEnabled(AActor* Object, bool bEnabled)
{
	if (Object)
	{
		Object->SetActorHiddenInGame(!bEnabled);
	}
}

void UMingRTSRenderingOptimizer::SetInstancingSettings(const FInstancingSettings& Settings)
{
	InstancingSettings = Settings;
}

FInstancingSettings UMingRTSRenderingOptimizer::GetInstancingSettings() const
{
	return InstancingSettings;
}

void UMingRTSRenderingOptimizer::BatchStaticMeshes(const TArray<AActor*>& MeshActors)
{
	if (!bInstancingEnabled || MeshActors.Num() < InstancingSettings.MinInstancesToBatch)
	{
		return;
	}

	TMap<FString, FInstancedMeshGroup> Groups;
	GroupMeshesForInstancing(MeshActors, Groups);

	for (auto& Pair : Groups)
	{
		FInstancedMeshGroup& Group = Pair.Value;
		if (Group.Instances.Num() >= InstancingSettings.MinInstancesToBatch)
		{
			USceneComponent* Parent = nullptr;
			if (MeshActors.Num() > 0 && MeshActors[0])
			{
				Parent = MeshActors[0]->GetRootComponent();
			}

			if (InstancingSettings.Method == EInstancingMethod::HISM)
			{
				CreateHISMComponent(Group.Mesh, Group.Instances, Parent);
			}
			else
			{
				CreateISMComponent(Group, Parent);
			}

			// Hide original actors
			for (AActor* Actor : MeshActors)
			{
				if (Actor && Actor->GetRootComponent())
				{
					UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
					if (MeshComp && MeshComp->GetStaticMesh() == Group.Mesh)
					{
						Actor->SetActorHiddenInGame(true);
					}
				}
			}
		}
	}

	Stats.InstancedObjects += MeshActors.Num();
	OnInstancingCompleted.Broadcast();
}

void UMingRTSRenderingOptimizer::CreateHISMComponent(UStaticMesh* Mesh, const TArray<FTransform>& Instances, USceneComponent* Parent)
{
	if (!Mesh || !GetWorld())
	{
		return;
	}

	UHierarchicalInstancedStaticMeshComponent* HISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(GetWorld()->GetCurrentLevel());
	if (HISM)
	{
		HISM->SetStaticMesh(Mesh);
		HISM->SetCollisionEnabled(InstancingSettings.bEnableCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
		HISM->bCastShadow = true;
		HISM->bAffectDistanceFieldLighting = true;

		if (Parent)
		{
			HISM->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);
		}

		for (const FTransform& InstanceTransform : Instances)
		{
			HISM->AddInstance(InstanceTransform);
		}

		HISM->RegisterComponent();
		InstancedComponents.Add(HISM);

		UE_LOG(LogTemp, Log, TEXT("Created HISM with %d instances of %s"), Instances.Num(), *Mesh->GetName());
	}
}

void UMingRTSRenderingOptimizer::ClearInstancedComponents()
{
	for (UInstancedStaticMeshComponent* Component : InstancedComponents)
	{
		if (Component && IsValid(Component))
		{
			Component->DestroyComponent();
		}
	}
	InstancedComponents.Empty();
	Stats.InstancedObjects = 0;
}

FRenderingStats UMingRTSRenderingOptimizer::GetRenderingStats() const
{
	return Stats;
}

void UMingRTSRenderingOptimizer::ResetRenderingStats()
{
	Stats = FRenderingStats();
}

void UMingRTSRenderingOptimizer::UpdateRenderingStats()
{
	UpdateDrawCallStats();

	// Count visible vs culled objects
	int32 VisibleCount = 0;
	int32 CulledCount = 0;

	for (const TWeakObjectPtr<AActor>& WeakActor : RegisteredObjects)
	{
		AActor* Actor = WeakActor.Get();
		if (Actor && IsValid(Actor))
		{
			if (IsObjectVisible(Actor))
			{
				VisibleCount++;
			}
			else
			{
				CulledCount++;
			}
		}
	}

	Stats.VisibleObjects = VisibleCount;
	Stats.CulledObjects = CulledCount;
}

FString UMingRTSRenderingOptimizer::GetRenderingReport() const
{
	FString Report;
	Report += TEXT("=== RTS Rendering Optimizer Report ===\n\n");
	
	Report += FString::Printf(TEXT("Visible Objects: %d\n"), Stats.VisibleObjects);
	Report += FString::Printf(TEXT("Culled Objects: %d\n"), Stats.CulledObjects);
	Report += FString::Printf(TEXT("Instanced Objects: %d\n"), Stats.InstancedObjects);
	Report += FString::Printf(TEXT("Draw Calls: %d\n"), Stats.DrawCalls);
	Report += FString::Printf(TEXT("Frame Time: %.2f ms\n"), Stats.FrameTime);
	Report += FString::Printf(TEXT("Culling Time: %.2f ms\n\n"), Stats.CullingTime);

	Report += TEXT("Culling Settings:\n");
	Report += FString::Printf(TEXT("  Frustum Culling: %s\n"), CullingSettings.bEnableFrustumCulling ? TEXT("Enabled") : TEXT("Disabled"));
	Report += FString::Printf(TEXT("  Occlusion Culling: %s\n"), CullingSettings.bEnableOcclusionCulling ? TEXT("Enabled") : TEXT("Disabled"));
	Report += FString::Printf(TEXT("  Distance Culling: %s\n\n"), CullingSettings.bEnableDistanceCulling ? TEXT("Enabled") : TEXT("Disabled"));

	Report += TEXT("Instancing Settings:\n");
	Report += FString::Printf(TEXT("  Method: %s\n"), *UEnum::GetValueAsString(InstancingSettings.Method));
	Report += FString::Printf(TEXT("  Min Instances: %d\n"), InstancingSettings.MinInstancesToBatch);
	Report += FString::Printf(TEXT("  Enabled: %s\n"), bInstancingEnabled ? TEXT("Yes") : TEXT("No"));

	return Report;
}

void UMingRTSRenderingOptimizer::EnableOcclusionCulling(bool bEnable)
{
	CullingSettings.bEnableOcclusionCulling = bEnable;
}

void UMingRTSRenderingOptimizer::EnableInstancing(bool bEnable)
{
	bInstancingEnabled = bEnable;
}

void UMingRTSRenderingOptimizer::SetMaxDrawCallsPerFrame(int32 MaxDraws)
{
	MaxDrawCalls = FMath::Max(100, MaxDraws);
}

void UMingRTSRenderingOptimizer::SetTargetFrameTime(float TargetMS)
{
	TargetFrameTime = FMath::Max(8.33f, TargetMS);
}

void UMingRTSRenderingOptimizer::SetGlobalLODScale(float Scale)
{
	GlobalLODScale = FMath::Clamp(Scale, 0.1f, 2.0f);
	
	// Apply to console variable
	if (IConsoleVariable* LODVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ViewDistanceScale")))
	{
		LODVar->Set(GlobalLODScale);
	}
}

void UMingRTSRenderingOptimizer::ForceLowestLOD(bool bForce)
{
	bForceLowestLODMode = bForce;
	
	if (IConsoleVariable* LODVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ForceLOD")))
	{
		LODVar->Set(bForce ? -1 : 0);
	}
}

void UMingRTSRenderingOptimizer::OptimizeShadowSettings()
{
	// Reduce shadow distance for performance
	if (IConsoleVariable* ShadowDistVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Shadow.DistanceScale")))
	{
		ShadowDistVar->Set(0.5f);
	}

	// Reduce shadow quality
	if (IConsoleVariable* ShadowResVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Shadow.MaxResolution")))
	{
		ShadowResVar->Set(1024);
	}

	UE_LOG(LogTemp, Log, TEXT("Shadow settings optimized for performance"));
}

void UMingRTSRenderingOptimizer::SetShadowDistance(float Distance)
{
	if (IConsoleVariable* ShadowDistVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Shadow.DistanceScale")))
	{
		float Scale = Distance / 10000.0f; // Normalize
		ShadowDistVar->Set(Scale);
	}
}

void UMingRTSRenderingOptimizer::ShowCullingDebug(bool bShow)
{
	if (IConsoleVariable* DebugVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Culling.Show")))
	{
		DebugVar->Set(bShow ? 1 : 0);
	}
}

void UMingRTSRenderingOptimizer::ShowInstancingDebug(bool bShow)
{
	if (IConsoleVariable* DebugVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.InstanceDebug.Draw")))
	{
		DebugVar->Set(bShow ? 1 : 0);
	}
}

void UMingRTSRenderingOptimizer::ToggleWireframeMode()
{
	if (UWorld* World = GetWorld())
	{
		static bool bWireframe = false;
		bWireframe = !bWireframe;

		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PC = Iterator->Get();
			if (PC && PC->PlayerCameraManager)
			{
				PC->ConsoleCommand(bWireframe ? TEXT("show wireframe") : TEXT("show lit"));
			}
		}
	}
}

void UMingRTSRenderingOptimizer::OnCullingTick()
{
	PerformCulling();
}

void UMingRTSRenderingOptimizer::OnStatsTick()
{
	UpdateRenderingStats();
	AutoAdjustQuality();
}

void UMingRTSRenderingOptimizer::PerformFrustumCulling()
{
	// This is handled by the engine automatically
	// We can add custom logic here if needed
}

void UMingRTSRenderingOptimizer::PerformOcclusionCulling()
{
	// This is handled by the engine automatically
	// We can add custom logic here if needed
}

void UMingRTSRenderingOptimizer::PerformDistanceCulling()
{
	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PC)
	{
		return;
	}

	FVector CameraLocation = PC->PlayerCameraManager ? PC->PlayerCameraManager->GetCameraLocation() : PC->GetPawn() ? PC->GetPawn()->GetActorLocation() : FVector::ZeroVector;

	for (const TWeakObjectPtr<AActor>& WeakActor : RegisteredObjects)
	{
		AActor* Actor = WeakActor.Get();
		if (Actor && IsValid(Actor))
		{
			float Distance = FVector::Dist(Actor->GetActorLocation(), CameraLocation);
			
			bool bShouldBeVisible = (Distance >= CullingSettings.NearCullDistance && 
			                         Distance <= CullingSettings.FarCullDistance);
			
			if (Actor->IsHidden() != !bShouldBeVisible)
			{
				SetObjectCullingEnabled(Actor, bShouldBeVisible);
			}
		}
	}
}

bool UMingRTSRenderingOptimizer::IsInFrustum(const FVector& Location, const FVector& Extent) const
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

	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	FRotator CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
	FVector CameraForward = CameraRotation.Vector();

	FVector ToLocation = (Location - CameraLocation).GetSafeNormal();
	float DotProduct = FVector::DotProduct(CameraForward, ToLocation);

	// Simple frustum check - within 90 degree cone
	return DotProduct > 0.0f;
}

bool UMingRTSRenderingOptimizer::IsOccluded(const FVector& Location) const
{
	// Simplified occlusion check
	// Full implementation would use hardware occlusion queries
	return false;
}

void UMingRTSRenderingOptimizer::GroupMeshesForInstancing(const TArray<AActor*>& MeshActors, TMap<FString, FInstancedMeshGroup>& OutGroups)
{
	for (AActor* Actor : MeshActors)
	{
		if (!Actor)
		{
			continue;
		}

		UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
		if (!MeshComp || !MeshComp->GetStaticMesh())
		{
			continue;
		}

		UStaticMesh* Mesh = MeshComp->GetStaticMesh();
		UMaterialInterface* Material = MeshComp->GetMaterial(0);

		// Create a key based on mesh and material
		FString Key = FString::Printf(TEXT("%s_%s"), 
			*Mesh->GetPathName(), 
			Material ? *Material->GetPathName() : TEXT("Default"));

		FInstancedMeshGroup& Group = OutGroups.FindOrAdd(Key);
		Group.Mesh = Mesh;
		Group.Material = Material;
		Group.Instances.Add(Actor->GetActorTransform());
		Group.InstanceCount++;
	}
}

UInstancedStaticMeshComponent* UMingRTSRenderingOptimizer::CreateISMComponent(const FInstancedMeshGroup& Group, USceneComponent* Parent)
{
	if (!Group.Mesh || !GetWorld())
	{
		return nullptr;
	}

	UInstancedStaticMeshComponent* ISM = NewObject<UInstancedStaticMeshComponent>(GetWorld()->GetCurrentLevel());
	if (ISM)
	{
		ISM->SetStaticMesh(Group.Mesh);
		ISM->SetMaterial(0, Group.Material);
		ISM->SetCollisionEnabled(InstancingSettings.bEnableCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
		ISM->bCastShadow = true;

		if (Parent)
		{
			ISM->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);
		}

		for (const FTransform& InstanceTransform : Group.Instances)
		{
			ISM->AddInstance(InstanceTransform);
		}

		ISM->RegisterComponent();
		InstancedComponents.Add(ISM);

		UE_LOG(LogTemp, Log, TEXT("Created ISM with %d instances of %s"), Group.Instances.Num(), *Group.Mesh->GetName());
	}

	return ISM;
}

void UMingRTSRenderingOptimizer::UpdateDrawCallStats()
{
	// Estimate draw calls based on visible objects
	// In a real implementation, this would query the renderer
	Stats.DrawCalls = Stats.VisibleObjects * 2; // Approximate
}

void UMingRTSRenderingOptimizer::AutoAdjustQuality()
{
	if (Stats.FrameTime > TargetFrameTime * 1.2f)
	{
		// Frame time too high, reduce quality
		if (GlobalLODScale > 0.5f)
		{
			SetGlobalLODScale(GlobalLODScale * 0.9f);
		}
	}
	else if (Stats.FrameTime < TargetFrameTime * 0.8f)
	{
		// Frame time good, can increase quality
		if (GlobalLODScale < 1.5f)
		{
			SetGlobalLODScale(GlobalLODScale * 1.05f);
		}
	}
}
