// Copyright Epic Games, Inc. All Rights Reserved.

#include "VRAR/MingVRARMaterialFactory.h"
#include "AssetToolsModule.h"
#include "Factories/MaterialFactoryNew.h"
#include "Materials/Material.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionTime.h"
#include "Materials/MaterialExpressionSine.h"
#include "Materials/MaterialExpressionMultiply.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"
#include "Materials/MaterialExpressionFrac.h"
#include "Materials/MaterialExpressionIf.h"
#include "MaterialEditingLibrary.h"
#include "EditorAssetLibrary.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Misc/FileHelper.h"

UMingVRARMaterialFactory::UMingVRARMaterialFactory()
    : ProjectPath(TEXT("C:/HW/MingGoRTS"))
    , VRMaterialConfigPath(TEXT("C:/HW/MingGoRTS/Content/VR/Materials/MaterialConfigs.json"))
    , ARMaterialConfigPath(TEXT("C:/HW/MingGoRTS/Content/AR/Materials/MaterialConfigs.json"))
    , VRMaterialOutputPath(TEXT("/Game/VR/Materials"))
    , ARMaterialOutputPath(TEXT("/Game/AR/Materials"))
{
    // Initialize asset tools
    FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
    AssetTools = AssetToolsModule.Get();
    
    MaterialFactory = NewObject<UMaterialFactoryNew>();
}

void UMingVRARMaterialFactory::Initialize(const FString& InProjectPath)
{
    ProjectPath = InProjectPath;
    VRMaterialConfigPath = FPaths::Combine(ProjectPath, TEXT("Content/VR/Materials/MaterialConfigs.json"));
    ARMaterialConfigPath = FPaths::Combine(ProjectPath, TEXT("Content/AR/Materials/MaterialConfigs.json"));
}

FLinearColor UMingVRARMaterialFactory::ParseColorString(const FString& ColorStr) const
{
    TArray<FString> Parts;
    ColorStr.ParseIntoArray(Parts, TEXT(","));
    
    if (Parts.Num() == 3)
    {
        return FLinearColor(
            FCString::Atof(*Parts[0]),
            FCString::Atof(*Parts[1]),
            FCString::Atof(*Parts[2]),
            1.0f
        );
    }
    else if (Parts.Num() == 4)
    {
        return FLinearColor(
            FCString::Atof(*Parts[0]),
            FCString::Atof(*Parts[1]),
            FCString::Atof(*Parts[2]),
            FCString::Atof(*Parts[3])
        );
    }
    
    return FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

UMaterialExpressionVectorParameter* UMingVRARMaterialFactory::CreateVectorParameter(
    UMaterial* Material, const FString& ParamName, const FLinearColor& DefaultValue, int32 X, int32 Y)
{
    UMaterialExpressionVectorParameter* Param = NewObject<UMaterialExpressionVectorParameter>(Material);
    Param->ParameterName = FName(*ParamName);
    Param->DefaultValue = DefaultValue;
    Param->Material = Material;
    Param->X = X;
    Param->Y = Y;
    
    Material->Expressions.Add(Param);
    return Param;
}

UMaterialExpressionScalarParameter* UMingVRARMaterialFactory::CreateScalarParameter(
    UMaterial* Material, const FString& ParamName, float DefaultValue, int32 X, int32 Y)
{
    UMaterialExpressionScalarParameter* Param = NewObject<UMaterialExpressionScalarParameter>(Material);
    Param->ParameterName = FName(*ParamName);
    Param->DefaultValue = DefaultValue;
    Param->Material = Material;
    Param->X = X;
    Param->Y = Y;
    
    Material->Expressions.Add(Param);
    return Param;
}

void UMingVRARMaterialFactory::SetupMaterialProperties(UMaterial* Material, const TMap<FString, FString>& Config)
{
    // Base Color
    if (Config.Contains(TEXT("BaseColor")))
    {
        FLinearColor Color = ParseColorString(Config[TEXT("BaseColor")]);
        UMaterialExpressionVectorParameter* Param = CreateVectorParameter(Material, TEXT("BaseColor"), Color, -300, 0);
        Material->BaseColor.Expression = Param;
    }
    
    // Roughness
    if (Config.Contains(TEXT("Roughness")))
    {
        float Value = FCString::Atof(*Config[TEXT("Roughness")]);
        UMaterialExpressionScalarParameter* Param = CreateScalarParameter(Material, TEXT("Roughness"), Value, -300, 100);
        Material->Roughness.Expression = Param;
    }
    
    // Metallic
    if (Config.Contains(TEXT("Metallic")))
    {
        float Value = FCString::Atof(*Config[TEXT("Metallic")]);
        UMaterialExpressionScalarParameter* Param = CreateScalarParameter(Material, TEXT("Metallic"), Value, -300, 150);
        Material->Metallic.Expression = Param;
    }
    
    // Emissive
    if (Config.Contains(TEXT("Emissive")))
    {
        FLinearColor Color = ParseColorString(Config[TEXT("Emissive")]);
        UMaterialExpressionVectorParameter* Param = CreateVectorParameter(Material, TEXT("EmissiveColor"), Color, -300, 200);
        Material->EmissiveColor.Expression = Param;
        
        // Set unlit shading model for emissive materials
        Material->ShadingModel = MSM_Unlit;
    }
    
    // Translucent / Opacity
    if (Config.Contains(TEXT("Translucent")) && Config[TEXT("Translucent")].ToLower() == TEXT("true"))
    {
        Material->BlendMode = BLEND_Translucent;
        
        if (Config.Contains(TEXT("Opacity")))
        {
            float Value = FCString::Atof(*Config[TEXT("Opacity")]);
            UMaterialExpressionScalarParameter* Param = CreateScalarParameter(Material, TEXT("Opacity"), Value, -300, 250);
            Material->Opacity.Expression = Param;
        }
    }
    
    // Special effects
    if (Config.Contains(TEXT("Hologram")) && Config[TEXT("Hologram")].ToLower() == TEXT("true"))
    {
        AddHologramEffect(Material);
    }
    
    if (Config.Contains(TEXT("GridTexture")) && Config[TEXT("GridTexture")].ToLower() == TEXT("true"))
    {
        AddGridTexture(Material);
    }
}

void UMingVRARMaterialFactory::AddHologramEffect(UMaterial* Material)
{
    // Create time node
    UMaterialExpressionTime* TimeNode = NewObject<UMaterialExpressionTime>(Material);
    TimeNode->Material = Material;
    TimeNode->X = -500;
    TimeNode->Y = 300;
    Material->Expressions.Add(TimeNode);
    
    // Create sine node
    UMaterialExpressionSine* SineNode = NewObject<UMaterialExpressionSine>(Material);
    SineNode->Period = 2.0f;
    SineNode->Material = Material;
    SineNode->X = -300;
    SineNode->Y = 300;
    Material->Expressions.Add(SineNode);
    
    // Connect time to sine
    SineNode->Input.Expression = TimeNode;
    
    // Create multiply node
    UMaterialExpressionMultiply* MultiplyNode = NewObject<UMaterialExpressionMultiply>(Material);
    MultiplyNode->Material = Material;
    MultiplyNode->X = -100;
    MultiplyNode->Y = 300;
    Material->Expressions.Add(MultiplyNode);
    
    MultiplyNode->A.Expression = SineNode;
    
    // Connect to emissive
    if (Material->EmissiveColor.Expression)
    {
        MultiplyNode->B.Expression = Material->EmissiveColor.Expression;
    }
    Material->EmissiveColor.Expression = MultiplyNode;
}

void UMingVRARMaterialFactory::AddGridTexture(UMaterial* Material)
{
    // Create texture coordinate
    UMaterialExpressionTextureCoordinate* TexCoord = NewObject<UMaterialExpressionTextureCoordinate>(Material);
    TexCoord->Material = Material;
    TexCoord->X = -500;
    TexCoord->Y = 400;
    Material->Expressions.Add(TexCoord);
    
    // Create frac node
    UMaterialExpressionFrac* FracNode = NewObject<UMaterialExpressionFrac>(Material);
    FracNode->Material = Material;
    FracNode->X = -300;
    FracNode->Y = 400;
    Material->Expressions.Add(FracNode);
    
    FracNode->Input.Expression = TexCoord;
    
    // Create if node for threshold
    UMaterialExpressionIf* IfNode = NewObject<UMaterialExpressionIf>(Material);
    IfNode->EqualsThreshold = 0.95f;
    IfNode->Material = Material;
    IfNode->X = -100;
    IfNode->Y = 400;
    Material->Expressions.Add(IfNode);
    
    IfNode->A.Expression = FracNode;
    
    // Connect to opacity mask
    Material->OpacityMask.Expression = IfNode;
}

UMaterial* UMingVRARMaterialFactory::CreateMaterialFromConfig(const FString& MaterialName, 
    const TMap<FString, FString>& Config, const FString& OutputPath)
{
    FString AssetPath = FPaths::Combine(OutputPath, MaterialName);
    
    // Check if asset already exists
    if (UEditorAssetLibrary::DoesAssetExist(AssetPath))
    {
        UE_LOG(LogTemp, Log, TEXT("Material already exists: %s"), *MaterialName);
        return Cast<UMaterial>(UEditorAssetLibrary::LoadAsset(AssetPath));
    }
    
    // Create new material
    UMaterial* Material = Cast<UMaterial>(AssetTools->CreateAsset(
        MaterialName,
        OutputPath,
        UMaterial::StaticClass(),
        MaterialFactory
    ));
    
    if (!Material)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create material: %s"), *MaterialName);
        FailedAssets.Add(MaterialName);
        return nullptr;
    }
    
    // Setup properties
    SetupMaterialProperties(Material, Config);
    
    // Recompile material
    FMaterialUpdateContext UpdateContext;
    UpdateContext.AddMaterial(Material);
    Material->PostEditChange();
    
    // Save asset
    UEditorAssetLibrary::SaveAsset(AssetPath);
    
    CreatedAssets.Add(MaterialName);
    UE_LOG(LogTemp, Log, TEXT("Created material: %s"), *MaterialName);
    
    return Material;
}

int32 UMingVRARMaterialFactory::CreateMaterialsFromJSON(const FString& JSONPath, const FString& OutputPath)
{
    FString JSONContent;
    if (!FFileHelper::LoadFileToString(JSONContent, *JSONPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *JSONPath);
        return 0;
    }
    
    TSharedPtr<FJsonObject> JSONObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONContent);
    
    if (!FJsonSerializer::Deserialize(Reader, JSONObject) || !JSONObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON: %s"), *JSONPath);
        return 0;
    }
    
    int32 CreatedCount = 0;
    
    for (const auto& Pair : JSONObject->Values)
    {
        FString MaterialName = Pair.Key;
        TSharedPtr<FJsonObject> MaterialConfig = Pair.Value->AsObject();
        
        if (!MaterialConfig.IsValid())
        {
            continue;
        }
        
        TMap<FString, FString> Config;
        for (const auto& ConfigPair : MaterialConfig->Values)
        {
            FString Key = ConfigPair.Key;
            FString Value = ConfigPair.Value->AsString();
            Config.Add(Key, Value);
        }
        
        UMaterial* CreatedMaterial = CreateMaterialFromConfig(MaterialName, Config, OutputPath);
        if (CreatedMaterial)
        {
            CreatedCount++;
        }
    }
    
    return CreatedCount;
}

bool UMingVRARMaterialFactory::CreateVRMaterials()
{
    UE_LOG(LogTemp, Log, TEXT("Creating VR materials from: %s"), *VRMaterialConfigPath);
    int32 Count = CreateMaterialsFromJSON(VRMaterialConfigPath, VRMaterialOutputPath);
    UE_LOG(LogTemp, Log, TEXT("Created %d VR materials"), Count);
    return Count > 0;
}

bool UMingVRARMaterialFactory::CreateARMaterials()
{
    UE_LOG(LogTemp, Log, TEXT("Creating AR materials from: %s"), *ARMaterialConfigPath);
    int32 Count = CreateMaterialsFromJSON(ARMaterialConfigPath, ARMaterialOutputPath);
    UE_LOG(LogTemp, Log, TEXT("Created %d AR materials"), Count);
    return Count > 0;
}

FString UMingVRARMaterialFactory::GetCreationReport() const
{
    FString Report = TEXT("=== Material Creation Report ===\n");
    Report += FString::Printf(TEXT("Successfully created: %d materials\n"), CreatedAssets.Num());
    
    for (const FString& Asset : CreatedAssets)
    {
        Report += FString::Printf(TEXT("  ✓ %s\n"), *Asset);
    }
    
    if (FailedAssets.Num() > 0)
    {
        Report += FString::Printf(TEXT("\nFailed to create: %d materials\n"), FailedAssets.Num());
        for (const FString& Asset : FailedAssets)
        {
            Report += FString::Printf(TEXT("  ✗ %s\n"), *Asset);
        }
    }
    
    Report += TEXT("================================\n");
    return Report;
}
