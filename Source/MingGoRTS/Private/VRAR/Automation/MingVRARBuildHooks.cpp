// Copyright Epic Games, Inc. All Rights Reserved.

#include "VRAR/Automation/MingVRARBuildHooks.h"
#include "VRAR/MingVRARMaterialFactory.h"
#include "Misc/Paths.h"

int32 UMingVRARBuildHooks::MaterialsCreatedCount = 0;
int32 UMingVRARBuildHooks::MaterialsVerifiedCount = 0;
bool UMingVRARBuildHooks::bLastBuildCheckPassed = false;

bool UMingVRARBuildHooks::PreBuildMaterialCheck()
{
    UE_LOG(LogTemp, Log, TEXT("=== Pre-Build Material Check ==="));
    
    // Check if materials exist
    if (VerifyAllMaterialsPresent())
    {
        UE_LOG(LogTemp, Log, TEXT("All materials present - build can proceed"));
        bLastBuildCheckPassed = true;
        return true;
    }
    
    // Materials missing - try to create them
    UE_LOG(LogTemp, Warning, TEXT("Materials missing - attempting auto-creation before build"));
    bool bCreated = EnsureMaterialsExist();
    
    bLastBuildCheckPassed = bCreated;
    return bCreated;
}

bool UMingVRARBuildHooks::PostBuildMaterialVerification()
{
    UE_LOG(LogTemp, Log, TEXT("=== Post-Build Material Verification ==="));
    
    bool bVerified = VerifyAllMaterialsPresent();
    
    if (bVerified)
    {
        UE_LOG(LogTemp, Display, TEXT("✓ All VR/AR materials verified after build"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("✗ Some materials missing after build!"));
    }
    
    return bVerified;
}

bool UMingVRARBuildHooks::EnsureMaterialsExist()
{
    UE_LOG(LogTemp, Display, TEXT("Ensuring VR/AR materials exist..."));
    
    // Create factory
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(FPaths::ProjectDir());
    
    // Create materials
    bool VRResult = Factory->CreateVRMaterials();
    bool ARResult = Factory->CreateARMaterials();
    
    FString Report = Factory->GetCreationReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
    
    MaterialsCreatedCount = VRResult ? 9 : 0;
    MaterialsCreatedCount += ARResult ? 7 : 0;
    
    return VRResult && ARResult;
}

bool UMingVRARBuildHooks::VerifyAllMaterialsPresent()
{
    UE_LOG(LogTemp, Log, TEXT("Verifying all materials present..."));
    
    // Define expected materials
    TArray<FString> ExpectedVRMaterials = {
        TEXT("M_VRController_Body"),
        TEXT("M_VRController_Button"),
        TEXT("M_VRPointer_Laser"),
        TEXT("M_VRPointer_Cursor"),
        TEXT("M_VRUI_Panel"),
        TEXT("M_VRUI_Border"),
        TEXT("M_VRTeleport_Valid"),
        TEXT("M_VRTeleport_Invalid"),
        TEXT("M_VRComfort_Vignette")
    };
    
    TArray<FString> ExpectedARMaterials = {
        TEXT("M_ARPlane_Floor"),
        TEXT("M_ARPlane_Wall"),
        TEXT("M_ARPlane_Boundary"),
        TEXT("M_ARCursor_Default"),
        TEXT("M_ARAnchor_Default"),
        TEXT("M_AROverlay_Content"),
        TEXT("M_AROverlay_Hologram")
    };
    
    // Check directories
    FString VRDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/VR/Materials"));
    FString ARDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/AR/Materials"));
    
    bool bAllPresent = true;
    MaterialsVerifiedCount = 0;
    
    // Check VR materials
    if (FPaths::DirectoryExists(VRDir))
    {
        TArray<FString> VRFiles;
        IFileManager::Get().FindFiles(VRFiles, *VRDir, TEXT("*.uasset"));
        
        for (const FString& Material : ExpectedVRMaterials)
        {
            bool bFound = false;
            for (const FString& File : VRFiles)
            {
                if (File.Contains(Material))
                {
                    bFound = true;
                    break;
                }
            }
            
            if (bFound)
            {
                MaterialsVerifiedCount++;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Missing VR material: %s"), *Material);
                bAllPresent = false;
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("VR Materials directory not found!"));
        bAllPresent = false;
    }
    
    // Check AR materials
    if (FPaths::DirectoryExists(ARDir))
    {
        TArray<FString> ARFiles;
        IFileManager::Get().FindFiles(ARFiles, *ARDir, TEXT("*.uasset"));
        
        for (const FString& Material : ExpectedARMaterials)
        {
            bool bFound = false;
            for (const FString& File : ARFiles)
            {
                if (File.Contains(Material))
                {
                    bFound = true;
                    break;
                }
            }
            
            if (bFound)
            {
                MaterialsVerifiedCount++;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Missing AR material: %s"), *Material);
                bAllPresent = false;
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AR Materials directory not found!"));
        bAllPresent = false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Verified %d/16 materials"), MaterialsVerifiedCount);
    
    return bAllPresent;
}

FString UMingVRARBuildHooks::GetBuildAutomationStatus()
{
    return FString::Printf(
        TEXT("Build Automation Status:\n")
        TEXT("  Materials Created: %d\n")
        TEXT("  Materials Verified: %d\n")
        TEXT("  Last Check Passed: %s"),
        MaterialsCreatedCount,
        MaterialsVerifiedCount,
        bLastBuildCheckPassed ? TEXT("Yes") : TEXT("No")
    );
}
