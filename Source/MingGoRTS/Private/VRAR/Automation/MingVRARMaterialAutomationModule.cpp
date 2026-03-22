// Copyright Epic Games, Inc. All Rights Reserved.

#include "VRAR/Automation/MingVRARMaterialAutomationModule.h"
#include "VRAR/MingVRARMaterialFactory.h"
#include "Misc/Paths.h"
#include "Misc/ConfigCacheIni.h"
#include "Editor.h"
#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "MingVRARMaterialAutomation"

void FMingVRARMaterialAutomationModule::StartupModule()
{
    // Load auto-creation setting from config
    GConfig->GetBool(TEXT("VRARAutomation"), TEXT("bAutoCreateMaterials"), bAutoCreationEnabled, GEditorIni);
    
    // Bind to post engine init for auto-creation
    PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FMingVRARMaterialAutomationModule::OnPostEngineInit);
    
    // Bind to map opened event
    MapOpenedHandle = FEditorDelegates::OnMapOpened.AddRaw(this, &FMingVRARMaterialAutomationModule::OnMapOpened);
    
    // Register toolbar button
    RegisterToolbarButton();
    
    UE_LOG(LogTemp, Log, TEXT("MingVRARMaterialAutomationModule started"));
}

void FMingVRARMaterialAutomationModule::ShutdownModule()
{
    FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
    FEditorDelegates::OnMapOpened.Remove(MapOpenedHandle);
    
    UE_LOG(LogTemp, Log, TEXT("MingVRARMaterialAutomationModule shut down"));
}

void FMingVRARMaterialAutomationModule::OnPostEngineInit()
{
    UE_LOG(LogTemp, Log, TEXT("Post engine init - checking auto-create materials"));
    
    if (bAutoCreationEnabled && ShouldAutoCreateMaterials())
    {
        UE_LOG(LogTemp, Display, TEXT("Auto-creating VR/AR materials..."));
        AutoCreateMaterials();
    }
}

void FMingVRARMaterialAutomationModule::OnMapOpened(const FString& MapName)
{
    // Optional: Auto-create when specific maps are opened
    if (MapName.Contains(TEXT("VRAR")) || MapName.Contains(TEXT("MaterialTest")))
    {
        UE_LOG(LogTemp, Log, TEXT("VR/AR related map opened - checking materials"));
        if (ShouldAutoCreateMaterials())
        {
            AutoCreateMaterials();
        }
    }
}

void FMingVRARMaterialAutomationModule::OnProjectLoaded()
{
    UE_LOG(LogTemp, Log, TEXT("Project loaded - VR/AR automation ready"));
}

bool FMingVRARMaterialAutomationModule::ShouldAutoCreateMaterials()
{
    // Check if materials already exist
    if (DoMaterialsExist())
    {
        UE_LOG(LogTemp, Log, TEXT("Materials already exist - skipping auto-creation"));
        return false;
    }
    
    // Check if config files exist
    FString VRConfigPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/VR/Materials/MaterialConfigs.json"));
    FString ARConfigPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/AR/Materials/MaterialConfigs.json"));
    
    bool bVRConfigExists = FPaths::FileExists(VRConfigPath);
    bool bARConfigExists = FPaths::FileExists(ARConfigPath);
    
    if (!bVRConfigExists || !bARConfigExists)
    {
        UE_LOG(LogTemp, Warning, TEXT("Material config files missing - cannot auto-create"));
        return false;
    }
    
    return true;
}

void FMingVRARMaterialAutomationModule::AutoCreateMaterials()
{
    UE_LOG(LogTemp, Display, TEXT("=== Auto-Creating VR/AR Materials ==="));
    
    // Create factory
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(FPaths::ProjectDir());
    
    // Create materials
    bool VRResult = Factory->CreateVRMaterials();
    bool ARResult = Factory->CreateARMaterials();
    
    FString Report = Factory->GetCreationReport();
    UE_LOG(LogTemp, Display, TEXT("%s"), *Report);
    
    if (VRResult && ARResult)
    {
        UE_LOG(LogTemp, Display, TEXT("Auto-creation completed successfully!"));
        
        // Show notification
        FNotificationInfo Info(LOCTEXT("MaterialsCreated", "VR/AR Materials Created Successfully!"));
        Info.ExpireDuration = 5.0f;
        FSlateNotificationManager::Get().AddNotification(Info);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Auto-creation partially failed"));
    }
}

void FMingVRARMaterialAutomationModule::RegisterToolbarButton()
{
    // Register section in Level Editor toolbar
    UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
    FToolMenuSection& Section = ToolbarMenu->AddSection("VRARMaterials", LOCTEXT("VRARSection", "VR/AR Materials"));
    
    Section.AddEntry(FToolMenuEntry::InitToolBarButton(
        "CreateVRARMaterials",
        FUIAction(
            FExecuteAction::CreateRaw(this, &FMingVRARMaterialAutomationModule::OnToolbarButtonClicked),
            FCanExecuteAction()
        ),
        LOCTEXT("CreateMaterialsButton", "Create VR/AR Materials"),
        LOCTEXT("CreateMaterialsTooltip", "Create all VR/AR materials from JSON configuration"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Plus")
    ));
    
    ToolbarMenu->RefreshAllWidgets();
}

void FMingVRARMaterialAutomationModule::OnToolbarButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Toolbar button clicked - creating materials"));
    AutoCreateMaterials();
}

bool FMingVRARMaterialAutomationModule::DoMaterialsExist()
{
    // Check for existence of key materials
    FString VRMaterialPath = TEXT("/Game/VR/Materials/M_VRController_Body");
    FString ARMaterialPath = TEXT("/Game/AR/Materials/M_ARCursor_Default");
    
    // Note: In a real implementation, you'd use the Asset Registry to check
    // For now, we just check if the directories have content
    FString VRDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/VR/Materials"));
    FString ARDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Content/AR/Materials"));
    
    // Check if directories exist and have .uasset files
    if (FPaths::DirectoryExists(VRDir))
    {
        TArray<FString> VRFiles;
        IFileManager::Get().FindFiles(VRFiles, *VRDir, TEXT("*.uasset"));
        if (VRFiles.Num() >= 9) // All VR materials exist
        {
            if (FPaths::DirectoryExists(ARDir))
            {
                TArray<FString> ARFiles;
                IFileManager::Get().FindFiles(ARFiles, *ARDir, TEXT("*.uasset"));
                if (ARFiles.Num() >= 7) // All AR materials exist
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMingVRARMaterialAutomationModule, MingVRARMaterialAutomation)
