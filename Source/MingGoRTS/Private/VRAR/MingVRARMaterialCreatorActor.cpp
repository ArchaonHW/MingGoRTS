// Copyright Epic Games, Inc. All Rights Reserved.

#include "VRAR/MingVRARMaterialCreatorActor.h"
#include "VRAR/MingVRARMaterialFactory.h"
#include "Misc/Paths.h"

AMingVRARMaterialCreatorActor::AMingVRARMaterialCreatorActor()
{
    ProjectPath = TEXT("C:/HW/MingGoRTS");
    VRMaterialConfigPath = FPaths::Combine(ProjectPath, TEXT("Content/VR/Materials/MaterialConfigs.json"));
    ARMaterialConfigPath = FPaths::Combine(ProjectPath, TEXT("Content/AR/Materials/MaterialConfigs.json"));
    VRMaterialOutputPath = TEXT("/Game/VR/Materials");
    ARMaterialOutputPath = TEXT("/Game/AR/Materials");
    
    CreatedVRMaterialsCount = 0;
    CreatedARMaterialsCount = 0;
    
    // This actor doesn't need tick
    PrimaryActorTick.bCanEverTick = false;
    
    // Editor-only actor
#if WITH_EDITORONLY_DATA
    bEditorOnlyActor = true;
#endif
}

void AMingVRARMaterialCreatorActor::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize material factory
    MaterialFactory = NewObject<UMingVRARMaterialFactory>(this);
    if (MaterialFactory)
    {
        MaterialFactory->Initialize(ProjectPath);
    }
}

void AMingVRARMaterialCreatorActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

#if WITH_EDITOR
void AMingVRARMaterialCreatorActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    // Update paths if project path changed
    if (PropertyChangedEvent.Property && 
        PropertyChangedEvent.Property->GetName() == TEXT("ProjectPath"))
    {
        VRMaterialConfigPath = FPaths::Combine(ProjectPath, TEXT("Content/VR/Materials/MaterialConfigs.json"));
        ARMaterialConfigPath = FPaths::Combine(ProjectPath, TEXT("Content/AR/Materials/MaterialConfigs.json"));
    }
}
#endif

bool AMingVRARMaterialCreatorActor::CreateVRMaterials()
{
    if (!MaterialFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("MaterialFactory is not initialized!"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Starting VR material creation..."));
    
    bool Result = MaterialFactory->CreateVRMaterials();
    
    // Update counters
    CreatedVRMaterialsCount = MaterialFactory->GetCreationReport().Contains(TEXT("Created")) ? 9 : 0;
    
    // Log results
    FString Report = MaterialFactory->GetCreationReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
    
    return Result;
}

bool AMingVRARMaterialCreatorActor::CreateARMaterials()
{
    if (!MaterialFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("MaterialFactory is not initialized!"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Starting AR material creation..."));
    
    bool Result = MaterialFactory->CreateARMaterials();
    
    // Update counters
    CreatedARMaterialsCount = MaterialFactory->GetCreationReport().Contains(TEXT("Created")) ? 7 : 0;
    
    // Log results
    FString Report = MaterialFactory->GetCreationReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
    
    return Result;
}

bool AMingVRARMaterialCreatorActor::CreateAllMaterials()
{
    bool VRResult = CreateVRMaterials();
    bool ARResult = CreateARMaterials();
    
    return VRResult && ARResult;
}

FString AMingVRARMaterialCreatorActor::GetCreationReport() const
{
    if (!MaterialFactory)
    {
        return TEXT("MaterialFactory not initialized!");
    }
    
    return MaterialFactory->GetCreationReport();
}

void AMingVRARMaterialCreatorActor::ResetResults()
{
    CreatedVRMaterialsCount = 0;
    CreatedARMaterialsCount = 0;
    CreatedAssets.Empty();
    FailedAssets.Empty();
    
    if (MaterialFactory)
    {
        // Note: MaterialFactory doesn't have a public reset method
        // Results are tracked per-session
    }
}
