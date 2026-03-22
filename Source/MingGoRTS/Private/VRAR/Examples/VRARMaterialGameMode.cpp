// Copyright Epic Games, Inc. All Rights Reserved.

#include "VRAR/Examples/VRARMaterialGameMode.h"
#include "VRAR/MingVRARMaterialFactory.h"

AVRARMaterialGameMode::AVRARMaterialGameMode()
{
    LastReport = TEXT("Not executed yet");
    MaterialsCreatedCount = 0;
    bCreationSuccessful = false;
}

void AVRARMaterialGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Automatically create materials when game starts (Editor or Runtime)
    UE_LOG(LogTemp, Log, TEXT("VRARMaterialGameMode::BeginPlay - Starting material creation..."));
    
    // Create all materials
    CreateAllVRARMaterials();
    
    // Log the results
    UE_LOG(LogTemp, Display, TEXT("Material Creation Report: %s"), *LastReport);
}

bool AVRARMaterialGameMode::CreateAllVRARMaterials()
{
    UE_LOG(LogTemp, Log, TEXT("=== Creating VR/AR Materials via C++ ==="));
    
    // Step 1: Create the Material Factory
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>(this);
    if (!Factory)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create MaterialFactory!"));
        LastReport = TEXT("ERROR: Failed to create factory");
        bCreationSuccessful = false;
        return false;
    }
    
    // Step 2: Initialize with project path
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    UE_LOG(LogTemp, Log, TEXT("MaterialFactory initialized successfully"));
    
    // Step 3: Create VR Materials (9 total)
    UE_LOG(LogTemp, Log, TEXT("Creating VR materials..."));
    bool VRResult = Factory->CreateVRMaterials();
    
    if (VRResult)
    {
        UE_LOG(LogTemp, Log, TEXT("VR materials created successfully"));
        MaterialsCreatedCount += 9;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Some VR materials may have failed"));
    }
    
    // Step 4: Create AR Materials (7 total)
    UE_LOG(LogTemp, Log, TEXT("Creating AR materials..."));
    bool ARResult = Factory->CreateARMaterials();
    
    if (ARResult)
    {
        UE_LOG(LogTemp, Log, TEXT("AR materials created successfully"));
        MaterialsCreatedCount += 7;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Some AR materials may have failed"));
    }
    
    // Step 5: Get the creation report
    LastReport = Factory->GetCreationReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *LastReport);
    
    // Step 6: Determine overall success
    bCreationSuccessful = VRResult && ARResult;
    
    if (bCreationSuccessful)
    {
        UE_LOG(LogTemp, Display, TEXT("=== All 16 materials created successfully! ==="));
        UE_LOG(LogTemp, Display, TEXT("Check Content Browser at:"));
        UE_LOG(LogTemp, Display, TEXT("  - /Game/VR/Materials/ (9 materials)"));
        UE_LOG(LogTemp, Display, TEXT("  - /Game/AR/Materials/ (7 materials)"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("=== Material creation partially failed ==="));
    }
    
    return bCreationSuccessful;
}

bool AVRARMaterialGameMode::CreateVRMaterialsOnly()
{
    UE_LOG(LogTemp, Log, TEXT("=== Creating VR Materials Only ==="));
    
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>(this);
    if (!Factory)
    {
        LastReport = TEXT("ERROR: Failed to create factory");
        return false;
    }
    
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    bool Result = Factory->CreateVRMaterials();
    
    LastReport = Factory->GetCreationReport();
    MaterialsCreatedCount = Result ? 9 : 0;
    bCreationSuccessful = Result;
    
    return Result;
}

bool AVRARMaterialGameMode::CreateARMaterialsOnly()
{
    UE_LOG(LogTemp, Log, TEXT("=== Creating AR Materials Only ==="));
    
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>(this);
    if (!Factory)
    {
        LastReport = TEXT("ERROR: Failed to create factory");
        return false;
    }
    
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    bool Result = Factory->CreateARMaterials();
    
    LastReport = Factory->GetCreationReport();
    MaterialsCreatedCount = Result ? 7 : 0;
    bCreationSuccessful = Result;
    
    return Result;
}
