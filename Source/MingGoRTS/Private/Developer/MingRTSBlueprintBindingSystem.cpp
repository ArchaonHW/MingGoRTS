// Copyright Epic Games, Inc. All Rights Reserved.

#include "Developer/MingRTSBlueprintBindingSystem.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSBlueprintBindingSystem::UMingRTSBlueprintBindingSystem()
{
    CurrentConfig = FBlueprintBindingConfig();
    TotalProcessedAssets = 0;
    SuccessfullyBoundAssets = 0;
    FailedBindings = 0;
}

void UMingRTSBlueprintBindingSystem::InitializeBlueprintBindingSystem(const FBlueprintBindingConfig& Config)
{
    CurrentConfig = Config;
    CachedAssetInfo.Empty();
    GeneratedCodeCache.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSBlueprintBindingSystem initialized"));
    UE_LOG(LogTemp, Log, TEXT("Output paths: Header=%s, Source=%s"), 
        *CurrentConfig.OutputHeaderPath, *CurrentConfig.OutputSourcePath);
    
    EnsureOutputDirectoriesExist();
}

void UMingRTSBlueprintBindingSystem::ShutdownBlueprintBindingSystem()
{
    CachedAssetInfo.Empty();
    GeneratedCodeCache.Empty();
    UE_LOG(LogTemp, Log, TEXT("MingRTSBlueprintBindingSystem shutdown"));
}

TArray<FBlueprintAssetInfo> UMingRTSBlueprintBindingSystem::ScanBlueprintAssets(const FString& ContentPath)
{
    TArray<FBlueprintAssetInfo> FoundAssets;
    
    // Scan for blueprint assets in the specified path
    TArray<FAssetData> AssetDataArray;
    FARFilter Filter;
    Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
    Filter.PackagePaths.Add(*ContentPath);
    Filter.bRecursivePaths = true;
    
    // Note: In actual implementation, use AssetRegistry to get assets
    // This is a simplified version for demonstration
    
    UE_LOG(LogTemp, Log, TEXT("Scanning blueprints in path: %s"), *ContentPath);
    
    OnBindingProgress.Broadcast(FString::Printf(TEXT("Scanning blueprints in: %s"), *ContentPath));
    
    return FoundAssets;
}

TArray<FBlueprintAssetInfo> UMingRTSBlueprintBindingSystem::ScanBlueprintAssetsByType(EBlueprintBindingType Type)
{
    TArray<FBlueprintAssetInfo> FilteredAssets;
    
    for (const FBlueprintAssetInfo& AssetInfo : CachedAssetInfo)
    {
        if (AssetInfo.BindingType == Type)
        {
            FilteredAssets.Add(AssetInfo);
        }
    }
    
    return FilteredAssets;
}

FBlueprintAssetInfo UMingRTSBlueprintBindingSystem::GetBlueprintAssetInfo(const FString& AssetPath)
{
    // Try to find in cache first
    for (const FBlueprintAssetInfo& Info : CachedAssetInfo)
    {
        if (Info.AssetPath == AssetPath)
        {
            return Info;
        }
    }
    
    FBlueprintAssetInfo NewInfo;
    NewInfo.AssetPath = AssetPath;
    
    // Extract asset name from path
    int32 LastSlashIndex;
    if (AssetPath.FindLastChar('/', LastSlashIndex))
    {
        NewInfo.AssetName = AssetPath.Mid(LastSlashIndex + 1);
        NewInfo.PackagePath = AssetPath.Left(LastSlashIndex);
    }
    
    return NewInfo;
}

FGeneratedBindingCode UMingRTSBlueprintBindingSystem::GenerateBindingCode(const FBlueprintAssetInfo& AssetInfo)
{
    FGeneratedBindingCode Result;
    Result.FileName = ConvertBlueprintNameToClassName(AssetInfo.AssetName);
    
    UE_LOG(LogTemp, Log, TEXT("Generating binding code for: %s"), *AssetInfo.AssetName);
    OnBindingProgress.Broadcast(FString::Printf(TEXT("Generating code for: %s"), *AssetInfo.AssetName));
    
    // Generate header file content
    Result.HeaderFileContent = GenerateHeaderFileContent(AssetInfo);
    
    // Generate source file content
    Result.SourceFileContent = GenerateSourceFileContent(AssetInfo);
    
    // Generate blueprint library if enabled
    if (CurrentConfig.bGenerateBlueprintLibrary)
    {
        TArray<FBlueprintAssetInfo> SingleAsset;
        SingleAsset.Add(AssetInfo);
        Result.BlueprintLibraryContent = GenerateBlueprintLibraryHeader(SingleAsset, 
            CurrentConfig.BlueprintLibraryPrefix + Result.FileName);
    }
    
    // Set file paths
    FString BaseFileName = CurrentConfig.ClassPrefix + Result.FileName;
    Result.HeaderFilePath = FPaths::Combine(CurrentConfig.OutputHeaderPath, BaseFileName + ".h");
    Result.SourceFilePath = FPaths::Combine(CurrentConfig.OutputSourcePath, BaseFileName + ".cpp");
    
    Result.GeneratedClasses.Add(BaseFileName);
    
    // Cache the result
    GeneratedCodeCache.Add(AssetInfo.AssetPath, Result);
    
    OnBindingComplete.Broadcast(AssetInfo, Result);
    
    return Result;
}

TArray<FGeneratedBindingCode> UMingRTSBlueprintBindingSystem::GenerateBindingsForAssets(const TArray<FBlueprintAssetInfo>& Assets)
{
    TArray<FGeneratedBindingCode> Results;
    
    TotalProcessedAssets = Assets.Num();
    SuccessfullyBoundAssets = 0;
    FailedBindings = 0;
    
    for (const FBlueprintAssetInfo& Asset : Assets)
    {
        if (ValidateBlueprintForBinding(Asset))
        {
            FGeneratedBindingCode Code = GenerateBindingCode(Asset);
            Results.Add(Code);
            SuccessfullyBoundAssets++;
        }
        else
        {
            TArray<FString> Errors = GetBindingValidationErrors(Asset);
            for (const FString& Error : Errors)
            {
                UE_LOG(LogTemp, Warning, TEXT("Binding validation error for %s: %s"), *Asset.AssetName, *Error);
                OnBindingError.Broadcast(Asset, Error);
            }
            FailedBindings++;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Binding generation complete. Success: %d, Failed: %d"), 
        SuccessfullyBoundAssets, FailedBindings);
    
    return Results;
}

bool UMingRTSBlueprintBindingSystem::WriteGeneratedCodeToFiles(const FGeneratedBindingCode& GeneratedCode)
{
    if (!EnsureOutputDirectoriesExist())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create output directories"));
        return false;
    }
    
    // Write header file
    if (!GeneratedCode.HeaderFileContent.IsEmpty())
    {
        if (!FFileHelper::SaveStringToFile(GeneratedCode.HeaderFileContent, *GeneratedCode.HeaderFilePath))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to write header file: %s"), *GeneratedCode.HeaderFilePath);
            return false;
        }
        UE_LOG(LogTemp, Log, TEXT("Generated header file: %s"), *GeneratedCode.HeaderFilePath);
    }
    
    // Write source file
    if (!GeneratedCode.SourceFileContent.IsEmpty())
    {
        if (!FFileHelper::SaveStringToFile(GeneratedCode.SourceFileContent, *GeneratedCode.SourceFilePath))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to write source file: %s"), *GeneratedCode.SourceFilePath);
            return false;
        }
        UE_LOG(LogTemp, Log, TEXT("Generated source file: %s"), *GeneratedCode.SourceFilePath);
    }
    
    return true;
}

FString UMingRTSBlueprintBindingSystem::GeneratePropertyGetterCode(const FBlueprintPropertyBinding& Property)
{
    FString CppType = ConvertPropertyTypeToCppType(Property.PropertyType);
    
    FString Code = FString::Printf(TEXT("\tUFUNCTION(BlueprintCallable, Category = \"%s\")\n"), *Property.Category);
    Code += FString::Printf(TEXT("\t%s Get%s() const;\n"), *CppType, *Property.PropertyName);
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GeneratePropertySetterCode(const FBlueprintPropertyBinding& Property)
{
    if (Property.bBlueprintReadOnly)
    {
        return TEXT("");
    }
    
    FString CppType = ConvertPropertyTypeToCppType(Property.PropertyType);
    
    FString Code = FString::Printf(TEXT("\tUFUNCTION(BlueprintCallable, Category = \"%s\")\n"), *Property.Category);
    Code += FString::Printf(TEXT("\tvoid Set%s(%s Value);\n"), *Property.PropertyName, *CppType);
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateUPropertyDeclaration(const FBlueprintPropertyBinding& Property)
{
    FString Specifiers = TEXT("EditAnywhere, BlueprintReadWrite");
    if (Property.bBlueprintReadOnly)
    {
        Specifiers = TEXT("VisibleAnywhere, BlueprintReadOnly");
    }
    else if (Property.bBlueprintReadWrite)
    {
        Specifiers = TEXT("EditAnywhere, BlueprintReadWrite");
    }
    
    FString CppType = ConvertPropertyTypeToCppType(Property.PropertyType);
    
    FString Code = FString::Printf(TEXT("\tUPROPERTY(%s, Category = \"%s\")\n"), *Specifiers, *Property.Category);
    Code += FString::Printf(TEXT("\t%s %s;\n"), *CppType, *Property.PropertyName);
    
    if (!Property.Description.IsEmpty())
    {
        Code += FString::Printf(TEXT("\t// %s\n"), *Property.Description);
    }
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateFunctionDeclaration(const FBlueprintFunctionBinding& Function)
{
    FString Code = TEXT("\tUFUNCTION(");
    
    if (Function.bBlueprintCallable)
    {
        Code += TEXT("BlueprintCallable, ");
    }
    if (Function.bBlueprintPure)
    {
        Code += TEXT("BlueprintPure, ");
    }
    
    Code += FString::Printf(TEXT("Category = \"%s\")\n"), *Function.Category);
    
    // Return type
    FString ReturnType = Function.ReturnType.IsEmpty() ? TEXT("void") : ConvertPropertyTypeToCppType(Function.ReturnType);
    Code += FString::Printf(TEXT("\t%s %s("), *ReturnType, *Function.FunctionName);
    
    // Parameters
    for (int32 i = 0; i < Function.ParameterTypes.Num(); ++i)
    {
        if (i > 0)
        {
            Code += TEXT(", ");
        }
        FString ParamType = ConvertPropertyTypeToCppType(Function.ParameterTypes[i]);
        FString ParamName = i < Function.ParameterNames.Num() ? Function.ParameterNames[i] : FString::Printf(TEXT("Param%d"), i);
        Code += FString::Printf(TEXT("%s %s"), *ParamType, *ParamName);
    }
    
    Code += TEXT(");\n");
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateFunctionImplementation(const FBlueprintFunctionBinding& Function, const FString& ClassName)
{
    FString ReturnType = Function.ReturnType.IsEmpty() ? TEXT("void") : ConvertPropertyTypeToCppType(Function.ReturnType);
    
    FString Code = FString::Printf(TEXT("%s %s::%s("), *ReturnType, *ClassName, *Function.FunctionName);
    
    // Parameters
    for (int32 i = 0; i < Function.ParameterTypes.Num(); ++i)
    {
        if (i > 0)
        {
            Code += TEXT(", ");
        }
        FString ParamType = ConvertPropertyTypeToCppType(Function.ParameterTypes[i]);
        FString ParamName = i < Function.ParameterNames.Num() ? Function.ParameterNames[i] : FString::Printf(TEXT("Param%d"), i);
        Code += FString::Printf(TEXT("%s %s"), *ParamType, *ParamName);
    }
    
    Code += TEXT(")\n");
    Code += TEXT("{\n");
    
    // Implementation placeholder
    if (!Function.ReturnType.IsEmpty() && Function.ReturnType != TEXT("void"))
    {
        FString CppReturnType = ConvertPropertyTypeToCppType(Function.ReturnType);
        if (CppReturnType == TEXT("bool"))
        {
            Code += TEXT("\treturn false;\n");
        }
        else if (CppReturnType == TEXT("int32") || CppReturnType == TEXT("float") || CppReturnType == TEXT("double"))
        {
            Code += TEXT("\treturn 0;\n");
        }
        else if (CppReturnType == TEXT("FString") || CppReturnType == TEXT("FText"))
        {
            Code += TEXT("\treturn TEXT(\"\");\n");
        }
        else
        {
            Code += FString::Printf(TEXT("\treturn %s();\n"), *CppReturnType);
        }
    }
    
    Code += TEXT("}\n");
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateEventDelegateDeclaration(const FBlueprintEventBinding& Event)
{
    FString ParamList;
    for (int32 i = 0; i < Event.ParameterTypes.Num(); ++i)
    {
        if (i > 0)
        {
            ParamList += TEXT(", ");
        }
        FString ParamType = ConvertPropertyTypeToCppType(Event.ParameterTypes[i]);
        FString ParamName = i < Event.ParameterNames.Num() ? Event.ParameterNames[i] : FString::Printf(TEXT("Param%d"), i);
        ParamList += FString::Printf(TEXT("%s %s"), *ParamType, *ParamName);
    }
    
    FString Code = FString::Printf(TEXT("\tDECLARE_DYNAMIC_MULTICAST_DELEGATE%s(%sDelegate"), 
        Event.ParameterTypes.Num() > 0 ? TEXT("_") : TEXT(""), *Event.EventName);
    
    if (!ParamList.IsEmpty())
    {
        Code += TEXT("_") + ParamList;
    }
    
    Code += TEXT(");\n");
    
    // Event declaration
    Code += FString::Printf(TEXT("\tUPROPERTY(BlueprintAssignable, Category = \"%s\")\n"), *Event.Category);
    Code += FString::Printf(TEXT("\t%sDelegate On%s;\n"), *Event.EventName, *Event.EventName);
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateEventBindingCode(const FBlueprintEventBinding& Event, const FString& ClassName)
{
    // This generates code to bind to the event
    FString Code = FString::Printf(TEXT("\t// Bind to %s event\n"), *Event.EventName);
    Code += FString::Printf(TEXT("\tOn%s.AddDynamic(this, &%s::Handle%s);\n"), *Event.EventName, *ClassName, *Event.EventName);
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateBlueprintLibraryHeader(const TArray<FBlueprintAssetInfo>& Assets, const FString& LibraryName)
{
    FString Code = TEXT("// Copyright Epic Games, Inc. All Rights Reserved.\n\n");
    Code += TEXT("#pragma once\n\n");
    Code += TEXT("#include \"CoreMinimal.h\"\n");
    Code += TEXT("#include \"Kismet/BlueprintFunctionLibrary.h\"\n");
    Code += FString::Printf(TEXT("#include \"%s.generated.h\"\n\n"), *LibraryName);
    
    // Class declaration
    Code += FString::Printf(TEXT("UCLASS()\nclass MINGGORTS_API U%s : public UBlueprintFunctionLibrary\n{\n"), *LibraryName);
    Code += TEXT("\tGENERATED_BODY()\n\n");
    Code += TEXT("public:\n");
    
    // Generate static functions for each asset
    for (const FBlueprintAssetInfo& Asset : Assets)
    {
        FString ClassName = CurrentConfig.ClassPrefix + ConvertBlueprintNameToClassName(Asset.AssetName);
        
        Code += FString::Printf(TEXT("\t/** Create instance of %s */\n"), *ClassName);
        Code += TEXT("\tUFUNCTION(BlueprintCallable, Category = \"Blueprint Binding\")\n");
        Code += FString::Printf(TEXT("\tstatic %s* Create%s(UObject* Outer);\n\n"), *ClassName, *ConvertBlueprintNameToClassName(Asset.AssetName));
        
        // Property accessors
        for (const FBlueprintPropertyBinding& Prop : Asset.Properties)
        {
            Code += FString::Printf(TEXT("\t/** Get %s property */\n"), *Prop.PropertyName);
            Code += TEXT("\tUFUNCTION(BlueprintCallable, Category = \"Blueprint Binding\")\n");
            FString CppType = ConvertPropertyTypeToCppType(Prop.PropertyType);
            Code += FString::Printf(TEXT("\tstatic %s Get%s(%s* Target);\n"), *CppType, *Prop.PropertyName, *ClassName);
            
            if (!Prop.bBlueprintReadOnly)
            {
                Code += FString::Printf(TEXT("\t/** Set %s property */\n"), *Prop.PropertyName);
                Code += TEXT("\tUFUNCTION(BlueprintCallable, Category = \"Blueprint Binding\")\n");
                Code += FString::Printf(TEXT("\tstatic void Set%s(%s* Target, %s Value);\n"), *Prop.PropertyName, *ClassName, *CppType);
            }
            Code += TEXT("\n");
        }
    }
    
    Code += TEXT("};\n");
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateBlueprintLibrarySource(const TArray<FBlueprintAssetInfo>& Assets, const FString& LibraryName)
{
    FString Code = TEXT("// Copyright Epic Games, Inc. All Rights Reserved.\n\n");
    Code += FString::Printf(TEXT("#include \"Developer/%s.h\"\n\n"), *LibraryName);
    
    for (const FBlueprintAssetInfo& Asset : Assets)
    {
        FString ClassName = CurrentConfig.ClassPrefix + ConvertBlueprintNameToClassName(Asset.AssetName);
        
        // Constructor implementation
        Code += FString::Printf(TEXT("%s* U%s::Create%s(UObject* Outer)\n"), *ClassName, *LibraryName, *ConvertBlueprintNameToClassName(Asset.AssetName));
        Code += TEXT("{\n");
        Code += FString::Printf(TEXT("\treturn NewObject<%s>(Outer);\n"), *ClassName);
        Code += TEXT("}\n\n");
        
        // Property implementations
        for (const FBlueprintPropertyBinding& Prop : Asset.Properties)
        {
            FString CppType = ConvertPropertyTypeToCppType(Prop.PropertyType);
            
            // Getter
            Code += FString::Printf(TEXT("%s U%s::Get%s(%s* Target)\n"), *CppType, *LibraryName, *Prop.PropertyName, *ClassName);
            Code += TEXT("{\n");
            Code += FString::Printf(TEXT("\treturn Target ? Target->%s : %s();\n"), *Prop.PropertyName, *CppType);
            Code += TEXT("}\n\n");
            
            // Setter
            if (!Prop.bBlueprintReadOnly)
            {
                Code += FString::Printf(TEXT("void U%s::Set%s(%s* Target, %s Value)\n"), *LibraryName, *Prop.PropertyName, *ClassName, *CppType);
                Code += TEXT("{\n");
                Code += FString::Printf(TEXT("\tif (Target)\n\t{\n\t\tTarget->%s = Value;\n\t}\n"), *Prop.PropertyName);
                Code += TEXT("}\n\n");
            }
        }
    }
    
    return Code;
}

bool UMingRTSBlueprintBindingSystem::ProcessAllBlueprintsInPath(const FString& ContentPath)
{
    TArray<FBlueprintAssetInfo> Assets = ScanBlueprintAssets(ContentPath);
    
    if (Assets.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No blueprints found in path: %s"), *ContentPath);
        return false;
    }
    
    TArray<FGeneratedBindingCode> GeneratedCodes = GenerateBindingsForAssets(Assets);
    
    bool bAllSuccess = true;
    for (const FGeneratedBindingCode& Code : GeneratedCodes)
    {
        if (!WriteGeneratedCodeToFiles(Code))
        {
            bAllSuccess = false;
        }
    }
    
    return bAllSuccess;
}

bool UMingRTSBlueprintBindingSystem::UpdateExistingBindings()
{
    UE_LOG(LogTemp, Log, TEXT("Updating existing bindings..."));
    
    // Scan all cached assets and regenerate if modified
    bool bAnyUpdated = false;
    
    for (FBlueprintAssetInfo& AssetInfo : CachedAssetInfo)
    {
        // Check if asset has been modified since last binding
        if (FGeneratedBindingCode* CachedCode = GeneratedCodeCache.Find(AssetInfo.AssetPath))
        {
            if (AssetInfo.LastModified > FDateTime::Now())
            {
                UE_LOG(LogTemp, Log, TEXT("Asset %s has been modified, regenerating binding..."), *AssetInfo.AssetName);
                FGeneratedBindingCode NewCode = GenerateBindingCode(AssetInfo);
                WriteGeneratedCodeToFiles(NewCode);
                bAnyUpdated = true;
            }
        }
    }
    
    return bAnyUpdated;
}

bool UMingRTSBlueprintBindingSystem::GenerateCompleteBindingSolution()
{
    UE_LOG(LogTemp, Log, TEXT("Generating complete binding solution..."));
    OnBindingProgress.Broadcast(TEXT("Starting complete binding solution generation"));
    
    // Scan all blueprints in the project
    TArray<FBlueprintAssetInfo> AllBlueprints = ScanBlueprintAssets(TEXT("/Game"));
    
    if (AllBlueprints.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No blueprints found in project"));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Found %d blueprints to process"), AllBlueprints.Num());
    
    // Generate bindings for all blueprints
    TArray<FGeneratedBindingCode> GeneratedCodes = GenerateBindingsForAssets(AllBlueprints);
    
    // Write all generated code to files
    bool bAllSuccess = true;
    for (const FGeneratedBindingCode& Code : GeneratedCodes)
    {
        if (!WriteGeneratedCodeToFiles(Code))
        {
            bAllSuccess = false;
        }
    }
    
    // Generate a master blueprint library
    if (CurrentConfig.bGenerateBlueprintLibrary && bAllSuccess)
    {
        FString MasterLibraryName = CurrentConfig.BlueprintLibraryPrefix + TEXT("Master");
        FString MasterHeader = GenerateBlueprintLibraryHeader(AllBlueprints, MasterLibraryName);
        FString MasterSource = GenerateBlueprintLibrarySource(AllBlueprints, MasterLibraryName);
        
        FString MasterHeaderPath = FPaths::Combine(CurrentConfig.OutputHeaderPath, MasterLibraryName + ".h");
        FString MasterSourcePath = FPaths::Combine(CurrentConfig.OutputSourcePath, MasterLibraryName + ".cpp");
        
        FFileHelper::SaveStringToFile(MasterHeader, *MasterHeaderPath);
        FFileHelper::SaveStringToFile(MasterSource, *MasterSourcePath);
        
        UE_LOG(LogTemp, Log, TEXT("Generated master blueprint library: %s"), *MasterLibraryName);
    }
    
    OnBindingProgress.Broadcast(FString::Printf(TEXT("Complete binding solution generated. Success: %d, Failed: %d"),
        SuccessfullyBoundAssets, FailedBindings));
    
    return bAllSuccess;
}

FString UMingRTSBlueprintBindingSystem::ConvertBlueprintNameToClassName(const FString& BlueprintName)
{
    if (CurrentConfig.bUseOriginalNames)
    {
        return BlueprintName;
    }
    
    // Remove BP_ prefix if present
    FString Result = BlueprintName;
    if (Result.StartsWith(TEXT("BP_")))
    {
        Result = Result.Mid(3);
    }
    
    // Remove spaces and special characters
    Result = Result.Replace(TEXT(" "), TEXT(""));
    Result = Result.Replace(TEXT("-"), TEXT(""));
    Result = Result.Replace(TEXT("_"), TEXT(""));
    
    return Result;
}

FString UMingRTSBlueprintBindingSystem::ConvertPropertyTypeToCppType(const FString& BlueprintType)
{
    static TMap<FString, FString> TypeMapping;
    if (TypeMapping.Num() == 0)
    {
        TypeMapping.Add(TEXT("Boolean"), TEXT("bool"));
        TypeMapping.Add(TEXT("Byte"), TEXT("uint8"));
        TypeMapping.Add(TEXT("Integer"), TEXT("int32"));
        TypeMapping.Add(TEXT("Integer64"), TEXT("int64"));
        TypeMapping.Add(TEXT("Float"), TEXT("float"));
        TypeMapping.Add(TEXT("String"), TEXT("FString"));
        TypeMapping.Add(TEXT("Text"), TEXT("FText"));
        TypeMapping.Add(TEXT("Name"), TEXT("FName"));
        TypeMapping.Add(TEXT("Vector"), TEXT("FVector"));
        TypeMapping.Add(TEXT("Rotator"), TEXT("FRotator"));
        TypeMapping.Add(TEXT("Transform"), TEXT("FTransform"));
        TypeMapping.Add(TEXT("Color"), TEXT("FColor"));
        TypeMapping.Add(TEXT("LinearColor"), TEXT("FLinearColor"));
    }
    
    if (const FString* CppType = TypeMapping.Find(BlueprintType))
    {
        return *CppType;
    }
    
    // If not found, assume it's a custom UObject type
    return BlueprintType + TEXT("*");
}

EBlueprintBindingType UMingRTSBlueprintBindingSystem::DetermineBindingType(UBlueprint* Blueprint)
{
    if (!Blueprint || !Blueprint->GeneratedClass)
    {
        return EBlueprintBindingType::Custom;
    }
    
    UClass* ParentClass = Blueprint->GeneratedClass->GetSuperClass();
    
    if (ParentClass->IsChildOf(AActor::StaticClass()))
    {
        return EBlueprintBindingType::Actor;
    }
    if (ParentClass->IsChildOf(UActorComponent::StaticClass()))
    {
        return EBlueprintBindingType::ActorComponent;
    }
    if (ParentClass->IsChildOf(UUserWidget::StaticClass()))
    {
        return EBlueprintBindingType::UserWidget;
    }
    if (ParentClass->IsChildOf(AGameModeBase::StaticClass()))
    {
        return EBlueprintBindingType::GameMode;
    }
    if (ParentClass->IsChildOf(AGameStateBase::StaticClass()))
    {
        return EBlueprintBindingType::GameState;
    }
    if (ParentClass->IsChildOf(APlayerController::StaticClass()))
    {
        return EBlueprintBindingType::PlayerController;
    }
    if (ParentClass->IsChildOf(APlayerState::StaticClass()))
    {
        return EBlueprintBindingType::PlayerState;
    }
    if (ParentClass->IsChildOf(ACharacter::StaticClass()))
    {
        return EBlueprintBindingType::Character;
    }
    if (ParentClass->IsChildOf(APawn::StaticClass()))
    {
        return EBlueprintBindingType::Pawn;
    }
    if (ParentClass->IsChildOf(UAnimInstance::StaticClass()))
    {
        return EBlueprintBindingType::AnimInstance;
    }
    
    return EBlueprintBindingType::Custom;
}

bool UMingRTSBlueprintBindingSystem::ValidateBlueprintForBinding(const FBlueprintAssetInfo& AssetInfo)
{
    if (AssetInfo.AssetName.IsEmpty())
    {
        return false;
    }
    
    if (AssetInfo.AssetPath.IsEmpty())
    {
        return false;
    }
    
    // Check for invalid characters in name
    if (AssetInfo.AssetName.Contains(TEXT(" ")) || AssetInfo.AssetName.Contains(TEXT("-")))
    {
        return false;
    }
    
    return true;
}

TArray<FString> UMingRTSBlueprintBindingSystem::GetBindingValidationErrors(const FBlueprintAssetInfo& AssetInfo)
{
    TArray<FString> Errors;
    
    if (AssetInfo.AssetName.IsEmpty())
    {
        Errors.Add(TEXT("Asset name is empty"));
    }
    
    if (AssetInfo.AssetPath.IsEmpty())
    {
        Errors.Add(TEXT("Asset path is empty"));
    }
    
    if (AssetInfo.AssetName.Contains(TEXT(" ")))
    {
        Errors.Add(TEXT("Asset name contains spaces"));
    }
    
    if (AssetInfo.AssetName.Contains(TEXT("-")))
    {
        Errors.Add(TEXT("Asset name contains hyphens"));
    }
    
    return Errors;
}

FString UMingRTSBlueprintBindingSystem::GenerateHeaderFileContent(const FBlueprintAssetInfo& AssetInfo)
{
    FString ClassName = CurrentConfig.ClassPrefix + ConvertBlueprintNameToClassName(AssetInfo.AssetName);
    
    FString Code = TEXT("// Copyright Epic Games, Inc. All Rights Reserved.\n\n");
    Code += TEXT("#pragma once\n\n");
    
    // Include guards and basic includes
    Code += TEXT("#include \"CoreMinimal.h\"\n");
    Code += GetBindingTypeInclude(AssetInfo.BindingType);
    Code += FString::Printf(TEXT("#include \"%s.generated.h\"\n\n"), *ClassName);
    
    // Forward declarations
    Code += GenerateClassForwardDeclarations(AssetInfo);
    
    // Class declaration
    FString ParentClass = TEXT("UObject");
    switch (AssetInfo.BindingType)
    {
        case EBlueprintBindingType::Actor: ParentClass = TEXT("AActor"); break;
        case EBlueprintBindingType::ActorComponent: ParentClass = TEXT("UActorComponent"); break;
        case EBlueprintBindingType::UserWidget: ParentClass = TEXT("UUserWidget"); break;
        case EBlueprintBindingType::GameMode: ParentClass = TEXT("AGameModeBase"); break;
        case EBlueprintBindingType::GameState: ParentClass = TEXT("AGameStateBase"); break;
        case EBlueprintBindingType::PlayerController: ParentClass = TEXT("APlayerController"); break;
        case EBlueprintBindingType::PlayerState: ParentClass = TEXT("APlayerState"); break;
        case EBlueprintBindingType::Character: ParentClass = TEXT("ACharacter"); break;
        case EBlueprintBindingType::Pawn: ParentClass = TEXT("APawn"); break;
        case EBlueprintBindingType::AnimInstance: ParentClass = TEXT("UAnimInstance"); break;
        default: ParentClass = TEXT("UObject"); break;
    }
    
    Code += FString::Printf(TEXT("UCLASS(ClassGroup = (MingGoRTS))\n"));
    Code += FString::Printf(TEXT("class MINGGORTS_API A%s : public %s\n{\n"), *ClassName, *ParentClass);
    Code += TEXT("\tGENERATED_BODY()\n\n");
    
    // Public section
    Code += TEXT("public:\n");
    Code += TEXT("\tA" + ClassName + TEXT("();\n\n"));
    
    // Properties
    if (AssetInfo.Properties.Num() > 0)
    {
        Code += TEXT("\t// Properties\n");
        for (const FBlueprintPropertyBinding& Prop : AssetInfo.Properties)
        {
            Code += GenerateUPropertyDeclaration(Prop);
            Code += TEXT("\n");
        }
    }
    
    // Events
    if (AssetInfo.Events.Num() > 0 && CurrentConfig.bAutoRegisterEvents)
    {
        Code += TEXT("\t// Events\n");
        for (const FBlueprintEventBinding& Event : AssetInfo.Events)
        {
            Code += GenerateEventDelegateDeclaration(Event);
            Code += TEXT("\n");
        }
    }
    
    // Functions
    if (AssetInfo.Functions.Num() > 0)
    {
        Code += TEXT("\t// Functions\n");
        for (const FBlueprintFunctionBinding& Function : AssetInfo.Functions)
        {
            Code += GenerateFunctionDeclaration(Function);
            Code += TEXT("\n");
        }
    }
    
    Code += TEXT("};\n");
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateSourceFileContent(const FBlueprintAssetInfo& AssetInfo)
{
    FString ClassName = CurrentConfig.ClassPrefix + ConvertBlueprintNameToClassName(AssetInfo.AssetName);
    
    FString Code = TEXT("// Copyright Epic Games, Inc. All Rights Reserved.\n\n");
    Code += FString::Printf(TEXT("#include \"Developer/%s.h\"\n\n"), *ClassName);
    
    // Constructor
    Code += FString::Printf(TEXT("A%s::A%s()\n"), *ClassName, *ClassName);
    Code += TEXT("{\n");
    Code += TEXT("\t// Set default values for properties\n");
    
    for (const FBlueprintPropertyBinding& Prop : AssetInfo.Properties)
    {
        if (!Prop.DefaultValue.IsEmpty())
        {
            Code += FString::Printf(TEXT("\t%s = %s;\n"), *Prop.PropertyName, *Prop.DefaultValue);
        }
    }
    
    Code += TEXT("}\n\n");
    
    // Function implementations
    for (const FBlueprintFunctionBinding& Function : AssetInfo.Functions)
    {
        Code += GenerateFunctionImplementation(Function, FString::Printf(TEXT("A%s"), *ClassName));
        Code += TEXT("\n");
    }
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateIncludeGuards(const FString& FileName)
{
    FString Guard = FileName.ToUpper();
    Guard = Guard.Replace(TEXT("."), TEXT("_"));
    Guard = Guard.Replace(TEXT("/"), TEXT("_"));
    Guard = Guard.Replace(TEXT("\\"), TEXT("_"));
    
    return FString::Printf(TEXT("#ifndef %s\n#define %s\n"), *Guard, *Guard);
}

FString UMingRTSBlueprintBindingSystem::GenerateClassForwardDeclarations(const FBlueprintAssetInfo& AssetInfo)
{
    FString Code;
    
    // Add forward declarations for referenced types
    TSet<FString> ForwardDeclTypes;
    
    for (const FBlueprintPropertyBinding& Prop : AssetInfo.Properties)
    {
        if (!Prop.PropertyType.StartsWith(TEXT("E")) && 
            !Prop.PropertyType.StartsWith(TEXT("F")) &&
            !Prop.PropertyType.StartsWith(TEXT("U")) &&
            !Prop.PropertyType.StartsWith(TEXT("A")) &&
            Prop.PropertyType != TEXT("bool") &&
            Prop.PropertyType != TEXT("int32") &&
            Prop.PropertyType != TEXT("float") &&
            Prop.PropertyType != TEXT("FString") &&
            Prop.PropertyType != TEXT("FText") &&
            Prop.PropertyType != TEXT("FName") &&
            Prop.PropertyType != TEXT("FVector") &&
            Prop.PropertyType != TEXT("FRotator") &&
            Prop.PropertyType != TEXT("FTransform"))
        {
            ForwardDeclTypes.Add(Prop.PropertyType);
        }
    }
    
    for (const FString& Type : ForwardDeclTypes)
    {
        Code += FString::Printf(TEXT("class %s;\n"), *Type);
    }
    
    if (!Code.IsEmpty())
    {
        Code += TEXT("\n");
    }
    
    return Code;
}

bool UMingRTSBlueprintBindingSystem::EnsureOutputDirectoriesExist()
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    bool bHeaderDir = PlatformFile.CreateDirectoryTree(*CurrentConfig.OutputHeaderPath);
    bool bSourceDir = PlatformFile.CreateDirectoryTree(*CurrentConfig.OutputSourcePath);
    
    return bHeaderDir && bSourceDir;
}

FString UMingRTSBlueprintBindingSystem::GetBindingTypeInclude(EBlueprintBindingType Type)
{
    switch (Type)
    {
        case EBlueprintBindingType::Actor:
        case EBlueprintBindingType::Character:
        case EBlueprintBindingType::Pawn:
            return TEXT("#include \"GameFramework/Actor.h\"\n");
        case EBlueprintBindingType::ActorComponent:
            return TEXT("#include \"Components/ActorComponent.h\"\n");
        case EBlueprintBindingType::UserWidget:
            return TEXT("#include \"Blueprint/UserWidget.h\"\n");
        case EBlueprintBindingType::GameMode:
            return TEXT("#include \"GameFramework/GameModeBase.h\"\n");
        case EBlueprintBindingType::GameState:
            return TEXT("#include \"GameFramework/GameStateBase.h\"\n");
        case EBlueprintBindingType::PlayerController:
            return TEXT("#include \"GameFramework/PlayerController.h\"\n");
        case EBlueprintBindingType::PlayerState:
            return TEXT("#include \"GameFramework/PlayerState.h\"\n");
        case EBlueprintBindingType::AnimInstance:
            return TEXT("#include \"Animation/AnimInstance.h\"\n");
        default:
            return TEXT("#include \"UObject/NoExportTypes.h\"\n");
    }
}

FString UMingRTSBlueprintBindingSystem::GeneratePropertyValidationCode(const FBlueprintPropertyBinding& Property)
{
    FString Code = FString::Printf(TEXT("\t// Validate %s\n"), *Property.PropertyName);
    
    if (Property.PropertyType == TEXT("FString") || Property.PropertyType == TEXT("FText"))
    {
        Code += FString::Printf(TEXT("\tif (%s.IsEmpty())\n\t{\n\t\tUE_LOG(LogTemp, Warning, TEXT(\"%s is empty\"));\n\t}\n"),
            *Property.PropertyName, *Property.PropertyName);
    }
    else if (Property.PropertyType == TEXT("int32") || Property.PropertyType == TEXT("float"))
    {
        Code += FString::Printf(TEXT("\t// Numeric validation for %s\n"), *Property.PropertyName);
    }
    
    return Code;
}

FString UMingRTSBlueprintBindingSystem::GenerateFunctionValidationCode(const FBlueprintFunctionBinding& Function)
{
    FString Code = TEXT("\t// Function validation\n");
    
    for (int32 i = 0; i < Function.ParameterTypes.Num(); ++i)
    {
        FString ParamName = i < Function.ParameterNames.Num() ? Function.ParameterNames[i] : FString::Printf(TEXT("Param%d"), i);
        Code += FString::Printf(TEXT("\t// Validate %s parameter\n"), *ParamName);
    }
    
    return Code;
}

void UMingRTSBlueprintBindingSystem::ExtractBlueprintProperties(UBlueprint* Blueprint, FBlueprintAssetInfo& AssetInfo)
{
    if (!Blueprint || !Blueprint->GeneratedClass)
    {
        return;
    }
    
    // Iterate through all properties in the generated class
    for (TFieldIterator<FProperty> PropIt(Blueprint->GeneratedClass); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;
        
        FBlueprintPropertyBinding Binding;
        Binding.PropertyName = Property->GetName();
        Binding.PropertyType = Property->GetClass()->GetName();
        Binding.Category = TEXT("Default");
        Binding.Description = Property->GetMetaData(TEXT("Tooltip"));
        
        AssetInfo.Properties.Add(Binding);
    }
}

void UMingRTSBlueprintBindingSystem::ExtractBlueprintFunctions(UBlueprint* Blueprint, FBlueprintAssetInfo& AssetInfo)
{
    if (!Blueprint || !Blueprint->GeneratedClass)
    {
        return;
    }
    
    // This would extract functions from the blueprint
    // Implementation depends on how functions are defined in the blueprint
}

void UMingRTSBlueprintBindingSystem::ExtractBlueprintEvents(UBlueprint* Blueprint, FBlueprintAssetInfo& AssetInfo)
{
    if (!Blueprint || !Blueprint->GeneratedClass)
    {
        return;
    }
    
    // Extract event graphs and custom events
    for (UEdGraph* Graph : Blueprint->UbergraphPages)
    {
        if (Graph)
        {
            for (UEdGraphNode* Node : Graph->Nodes)
            {
                if (Node && Node->IsA(UK2Node_Event::StaticClass()))
                {
                    FBlueprintEventBinding Event;
                    Event.EventName = Node->GetNodeTitle(ENodeTitleType::ListView).ToString();
                    Event.Category = TEXT("Events");
                    
                    AssetInfo.Events.Add(Event);
                }
            }
        }
    }
}
