// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "MingRTSBlueprintBindingSystem.generated.h"

/**
 * Blueprint asset types supported for binding
 */
UENUM(BlueprintType)
enum class EBlueprintBindingType : uint8
{
    Actor,
    ActorComponent,
    UserWidget,
    GameMode,
    GameState,
    PlayerController,
    PlayerState,
    Character,
    Pawn,
    AnimInstance,
    Custom
};

/**
 * Property binding information
 */
USTRUCT()
struct FBlueprintPropertyBinding
{
    GENERATED_BODY()

    UPROPERTY()
    FString PropertyName;

    UPROPERTY()
    FString PropertyType;

    UPROPERTY()
    FString Category;

    UPROPERTY()
    bool bBlueprintReadOnly = false;

    UPROPERTY()
    bool bBlueprintReadWrite = true;

    UPROPERTY()
    FString DefaultValue;

    UPROPERTY()
    FString Description;
};

/**
 * Function binding information
 */
USTRUCT()
struct FBlueprintFunctionBinding
{
    GENERATED_BODY()

    UPROPERTY()
    FString FunctionName;

    UPROPERTY()
    FString ReturnType;

    UPROPERTY()
    TArray<FString> ParameterTypes;

    UPROPERTY()
    TArray<FString> ParameterNames;

    UPROPERTY()
    FString Category;

    UPROPERTY()
    bool bBlueprintCallable = true;

    UPROPERTY()
    bool bBlueprintPure = false;

    UPROPERTY()
    FString Description;
};

/**
 * Event binding information
 */
USTRUCT()
struct FBlueprintEventBinding
{
    GENERATED_BODY()

    UPROPERTY()
    FString EventName;

    UPROPERTY()
    TArray<FString> ParameterTypes;

    UPROPERTY()
    TArray<FString> ParameterNames;

    UPROPERTY()
    FString Category;

    UPROPERTY()
    FString Description;
};

/**
 * Blueprint asset information for binding
 */
USTRUCT()
struct FBlueprintAssetInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString AssetPath;

    UPROPERTY()
    FString AssetName;

    UPROPERTY()
    FString PackagePath;

    UPROPERTY()
    EBlueprintBindingType BindingType;

    UPROPERTY()
    FString ParentClassName;

    UPROPERTY()
    TArray<FBlueprintPropertyBinding> Properties;

    UPROPERTY()
    TArray<FBlueprintFunctionBinding> Functions;

    UPROPERTY()
    TArray<FBlueprintEventBinding> Events;

    UPROPERTY()
    FDateTime LastModified;
};

/**
 * Generated binding code result
 */
USTRUCT()
struct FGeneratedBindingCode
{
    GENERATED_BODY()

    UPROPERTY()
    FString HeaderFileContent;

    UPROPERTY()
    FString SourceFileContent;

    UPROPERTY()
    FString BlueprintLibraryContent;

    UPROPERTY()
    FString FileName;

    UPROPERTY()
    FString HeaderFilePath;

    UPROPERTY()
    FString SourceFilePath;

    UPROPERTY()
    TArray<FString> GeneratedClasses;
};

/**
 * Blueprint binding configuration
 */
USTRUCT(BlueprintType)
struct FBlueprintBindingConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
    bool bGenerateHeaderFiles = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
    bool bGenerateSourceFiles = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
    bool bGenerateBlueprintLibrary = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
    bool bAutoRegisterEvents = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
    bool bGeneratePropertyGetters = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
    bool bGeneratePropertySetters = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paths")
    FString OutputHeaderPath = TEXT("Source/MingGoRTS/Public/Generated");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paths")
    FString OutputSourcePath = TEXT("Source/MingGoRTS/Private/Generated");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming")
    FString ClassPrefix = TEXT("MingRTS");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming")
    FString BlueprintLibraryPrefix = TEXT("MingRTSBPLib");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming")
    bool bUseOriginalNames = true;
};

/**
 * Automated Blueprint asset to C++ code binding system
 * This system provides:
 * - Automatic Blueprint asset scanning
 * - C++ header/source code generation
 * - Blueprint library generation
 * - Property/Function/Event binding
 * - Automatic code synchronization
 */
UCLASS(ClassGroup = (MingGoRTS), BlueprintType)
class MINGGORTS_API UMingRTSBlueprintBindingSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSBlueprintBindingSystem();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    void InitializeBlueprintBindingSystem(const FBlueprintBindingConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    void ShutdownBlueprintBindingSystem();

    // Asset Discovery
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    TArray<FBlueprintAssetInfo> ScanBlueprintAssets(const FString& ContentPath);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    TArray<FBlueprintAssetInfo> ScanBlueprintAssetsByType(EBlueprintBindingType Type);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    FBlueprintAssetInfo GetBlueprintAssetInfo(const FString& AssetPath);

    // Code Generation
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    FGeneratedBindingCode GenerateBindingCode(const FBlueprintAssetInfo& AssetInfo);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    TArray<FGeneratedBindingCode> GenerateBindingsForAssets(const TArray<FBlueprintAssetInfo>& Assets);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    bool WriteGeneratedCodeToFiles(const FGeneratedBindingCode& GeneratedCode);

    // Property Binding
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Properties")
    FString GeneratePropertyGetterCode(const FBlueprintPropertyBinding& Property);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Properties")
    FString GeneratePropertySetterCode(const FBlueprintPropertyBinding& Property);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Properties")
    FString GenerateUPropertyDeclaration(const FBlueprintPropertyBinding& Property);

    // Function Binding
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Functions")
    FString GenerateFunctionDeclaration(const FBlueprintFunctionBinding& Function);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Functions")
    FString GenerateFunctionImplementation(const FBlueprintFunctionBinding& Function, const FString& ClassName);

    // Event Binding
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Events")
    FString GenerateEventDelegateDeclaration(const FBlueprintEventBinding& Event);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Events")
    FString GenerateEventBindingCode(const FBlueprintEventBinding& Event, const FString& ClassName);

    // Blueprint Library Generation
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Library")
    FString GenerateBlueprintLibraryHeader(const TArray<FBlueprintAssetInfo>& Assets, const FString& LibraryName);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Library")
    FString GenerateBlueprintLibrarySource(const TArray<FBlueprintAssetInfo>& Assets, const FString& LibraryName);

    // Batch Operations
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Batch")
    bool ProcessAllBlueprintsInPath(const FString& ContentPath);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Batch")
    bool UpdateExistingBindings();

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Batch")
    bool GenerateCompleteBindingSolution();

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Utility")
    FString ConvertBlueprintNameToClassName(const FString& BlueprintName);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Utility")
    FString ConvertPropertyTypeToCppType(const FString& BlueprintType);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Utility")
    EBlueprintBindingType DetermineBindingType(UBlueprint* Blueprint);

    // Validation
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Validation")
    bool ValidateBlueprintForBinding(const FBlueprintAssetInfo& AssetInfo);

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding|Validation")
    TArray<FString> GetBindingValidationErrors(const FBlueprintAssetInfo& AssetInfo);

    // Delegates for progress
    UPROPERTY(BlueprintAssignable, Category = "Blueprint Binding")
    FOnBlueprintBindingProgress OnBindingProgress;

    UPROPERTY(BlueprintAssignable, Category = "Blueprint Binding")
    FOnBlueprintBindingComplete OnBindingComplete;

    UPROPERTY(BlueprintAssignable, Category = "Blueprint Binding")
    FOnBlueprintBindingError OnBindingError;

    // Statistics
    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    int32 GetTotalProcessedAssets() const { return TotalProcessedAssets; }

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    int32 GetSuccessfullyBoundAssets() const { return SuccessfullyBoundAssets; }

    UFUNCTION(BlueprintCallable, Category = "Blueprint Binding")
    int32 GetFailedBindings() const { return FailedBindings; }

private:
    UPROPERTY()
    FBlueprintBindingConfig CurrentConfig;

    UPROPERTY()
    TArray<FBlueprintAssetInfo> CachedAssetInfo;

    UPROPERTY()
    TMap<FString, FGeneratedBindingCode> GeneratedCodeCache;

    int32 TotalProcessedAssets = 0;
    int32 SuccessfullyBoundAssets = 0;
    int32 FailedBindings = 0;

    // Internal helper functions
    void ExtractBlueprintProperties(UBlueprint* Blueprint, FBlueprintAssetInfo& AssetInfo);
    void ExtractBlueprintFunctions(UBlueprint* Blueprint, FBlueprintAssetInfo& AssetInfo);
    void ExtractBlueprintEvents(UBlueprint* Blueprint, FBlueprintAssetInfo& AssetInfo);
    FString GenerateHeaderFileContent(const FBlueprintAssetInfo& AssetInfo);
    FString GenerateSourceFileContent(const FBlueprintAssetInfo& AssetInfo);
    FString GenerateIncludeGuards(const FString& FileName);
    FString GenerateClassForwardDeclarations(const FBlueprintAssetInfo& AssetInfo);
    bool EnsureOutputDirectoriesExist();
    FString GetBindingTypeInclude(EBlueprintBindingType Type);
    FString GeneratePropertyValidationCode(const FBlueprintPropertyBinding& Property);
    FString GenerateFunctionValidationCode(const FBlueprintFunctionBinding& Function);
};

// Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlueprintBindingProgress, const FString&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlueprintBindingComplete, const FBlueprintAssetInfo&, AssetInfo, const FGeneratedBindingCode&, GeneratedCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlueprintBindingError, const FBlueprintAssetInfo&, AssetInfo, const FString&, ErrorMessage);
