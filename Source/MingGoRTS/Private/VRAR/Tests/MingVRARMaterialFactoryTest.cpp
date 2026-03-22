// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

/**
 * Automation test for VR/AR Material Factory.
 * Run this test from Session Frontend or via command line.
 * 
 * Command line:
 *   UnrealEditor-Cmd.exe C:\HW\MingGoRTS\MingGoRTS.uproject -run=AutomationTest -TestFilter=VRAR -log
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMingVRARMaterialFactoryTest, 
    "MingGoRTS.VRAR.MaterialFactory.BasicTest",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMingVRARMaterialFactoryTest::RunTest(const FString& Parameters)
{
    // Import the factory
    #include "VRAR/MingVRARMaterialFactory.h"
    
    // Create factory
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    TestNotNull(TEXT("Factory should be created"), Factory);
    
    // Initialize
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    
    // Create VR materials
    bool VRResult = Factory->CreateVRMaterials();
    TestTrue(TEXT("VR materials should be created successfully"), VRResult);
    
    // Create AR materials  
    bool ARResult = Factory->CreateARMaterials();
    TestTrue(TEXT("AR materials should be created successfully"), ARResult);
    
    // Get report
    FString Report = Factory->GetCreationReport();
    TestFalse(TEXT("Report should not be empty"), Report.IsEmpty());
    
    UE_LOG(LogTemp, Log, TEXT("Material Factory Test Report:\n%s"), *Report);
    
    return true;
}
