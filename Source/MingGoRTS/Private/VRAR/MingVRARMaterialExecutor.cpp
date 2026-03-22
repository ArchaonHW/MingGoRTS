// Copyright Epic Games, Inc. All Rights Reserved.

#include "VRAR/MingVRARMaterialExecutor.h"
#include "VRAR/MingVRARMaterialFactory.h"
#include "Misc/Paths.h"

AMingVRARMaterialExecutor::AMingVRARMaterialExecutor()
{
    PrimaryActorTick.bCanEverTick = false;
    
    TotalMaterialsToCreate = 16; // 9 VR + 7 AR
    MaterialsCreated = 0;
    bIsCreatingMaterials = false;
    LastExecutionResult = TEXT("Ready to execute");
}

void AMingVRARMaterialExecutor::BeginPlay()
{
    Super::BeginPlay();
}

bool AMingVRARMaterialExecutor::ExecuteMaterialCreation()
{
    if (bIsCreatingMaterials)
    {
        UE_LOG(LogTemp, Warning, TEXT("Material creation already in progress!"));
        LastExecutionResult = TEXT("Already in progress");
        return false;
    }

    bIsCreatingMaterials = true;
    MaterialsCreated = 0;
    
    UE_LOG(LogTemp, Log, TEXT("=== Starting VR/AR Material Creation ==="));
    
    // Create the factory
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>(this);
    if (!Factory)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create MaterialFactory!"));
        LastExecutionResult = TEXT("Failed to create factory");
        bIsCreatingMaterials = false;
        return false;
    }
    
    // Initialize with project path
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    UE_LOG(LogTemp, Log, TEXT("MaterialFactory initialized"));
    
    // Create VR materials
    UE_LOG(LogTemp, Log, TEXT("Creating VR materials..."));
    bool VRResult = Factory->CreateVRMaterials();
    
    // Create AR materials
    UE_LOG(LogTemp, Log, TEXT("Creating AR materials..."));
    bool ARResult = Factory->CreateARMaterials();
    
    // Get report
    FString Report = Factory->GetCreationReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
    
    // Update counters
    MaterialsCreated = 9 + 7; // VR + AR
    bIsCreatingMaterials = false;
    
    if (VRResult && ARResult)
    {
        LastExecutionResult = TEXT("SUCCESS: All materials created!");
        UE_LOG(LogTemp, Display, TEXT("=== Material Creation Complete ==="));
        return true;
    }
    else
    {
        LastExecutionResult = TEXT("PARTIAL: Some materials may have failed");
        UE_LOG(LogTemp, Warning, TEXT("=== Material Creation Partially Complete ==="));
        return false;
    }
}

bool AMingVRARMaterialExecutor::CreateVRMaterialsOnly()
{
    if (bIsCreatingMaterials)
    {
        UE_LOG(LogTemp, Warning, TEXT("Material creation already in progress!"));
        return false;
    }

    bIsCreatingMaterials = true;
    
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>(this);
    if (!Factory)
    {
        LastExecutionResult = TEXT("Failed to create factory");
        bIsCreatingMaterials = false;
        return false;
    }
    
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    bool Result = Factory->CreateVRMaterials();
    
    MaterialsCreated += 9;
    bIsCreatingMaterials = false;
    LastExecutionResult = Result ? TEXT("VR Materials created") : TEXT("VR Materials failed");
    
    return Result;
}

bool AMingVRARMaterialExecutor::CreateARMaterialsOnly()
{
    if (bIsCreatingMaterials)
    {
        UE_LOG(LogTemp, Warning, TEXT("Material creation already in progress!"));
        return false;
    }

    bIsCreatingMaterials = true;
    
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>(this);
    if (!Factory)
    {
        LastExecutionResult = TEXT("Failed to create factory");
        bIsCreatingMaterials = false;
        return false;
    }
    
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    bool Result = Factory->CreateARMaterials();
    
    MaterialsCreated += 7;
    bIsCreatingMaterials = false;
    LastExecutionResult = Result ? TEXT("AR Materials created") : TEXT("AR Materials failed");
    
    return Result;
}
