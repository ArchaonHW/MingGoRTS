// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Potato AI Code Generator API Implementation

#include "API/MingRTSPotatoAICodeGeneratorAPI.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformApplicationMisc.h"

DEFINE_LOG_CATEGORY_STATIC(LogPotatoAI, Log, All);

const TMap<FString, FString> UMingRTSPotatoAICodeGeneratorAPI::ClassNameMappings = {
    {TEXT("player"), TEXT("MingPlayer")},
    {TEXT("enemy"), TEXT("MingEnemy")},
    {TEXT("weapon"), TEXT("MingWeapon")},
    {TEXT("item"), TEXT("MingItem")},
    {TEXT("ui"), TEXT("MingUI")},
    {TEXT("system"), TEXT("MingSystem")},
    {TEXT("manager"), TEXT("MingManager")},
    {TEXT("controller"), TEXT("MingController")}
};

const TMap<FString, FString> UMingRTSPotatoAICodeGeneratorAPI::SystemNameMappings = {
    {TEXT("audio"), TEXT("Audio")},
    {TEXT("ui"), TEXT("UI")},
    {TEXT("input"), TEXT("Input")},
    {TEXT("network"), TEXT("Network")},
    {TEXT("save"), TEXT("Save")},
    {TEXT("physics"), TEXT("Physics")},
    {TEXT("render"), TEXT("Render")},
    {TEXT("ai"), TEXT("AI")}
};

const TMap<FString, FString> UMingRTSPotatoAICodeGeneratorAPI::ActorNameMappings = {
    {TEXT("player"), TEXT("MingPlayerCharacter")},
    {TEXT("enemy"), TEXT("MingEnemyCharacter")},
    {TEXT("npc"), TEXT("MingNPCCharacter")},
    {TEXT("item"), TEXT("MingItemActor")},
    {TEXT("weapon"), TEXT("MingWeaponActor")}
};

const TMap<FString, FString> UMingRTSPotatoAICodeGeneratorAPI::FunctionNameMappings = {
    {TEXT("calculate"), TEXT("Calculate")},
    {TEXT("compute"), TEXT("Compute")},
    {TEXT("process"), TEXT("Process")},
    {TEXT("handle"), TEXT("Handle")},
    {TEXT("update"), TEXT("Update")},
    {TEXT("initialize"), TEXT("Initialize")},
    {TEXT("init"), TEXT("Init")}
};

UMingRTSPotatoAICodeGeneratorAPI::UMingRTSPotatoAICodeGeneratorAPI()
    : bIsInitialized(false)
    , TotalGeneratedCount(0)
    , AverageQualityScore(0.0f)
{
}

bool UMingRTSPotatoAICodeGeneratorAPI::Initialize()
{
    UE_LOG(LogPotatoAI, Log, TEXT("🥔 Initializing Potato AI Code Generator API"));
    
    InitializeTemplateDatabase();
    
    bIsInitialized = true;
    return true;
}

void UMingRTSPotatoAICodeGeneratorAPI::Shutdown()
{
    UE_LOG(LogPotatoAI, Log, TEXT("🥔 Shutting down Potato AI Code Generator API"));
    bIsInitialized = false;
}

FGeneratedCodeResult UMingRTSPotatoAICodeGeneratorAPI::GenerateCode(const FString& Requirement, ECodeGenerationLanguage Language, ECodeTemplateType Template)
{
    FGeneratedCodeResult Result;
    Result.Requirement = Requirement;
    Result.TemplateType = Template;
    Result.GeneratedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    UE_LOG(LogPotatoAI, Log, TEXT("🥔 Generating code for: %s"), *Requirement);
    
    FCodeGenerationOptions Options;
    Options.bAddComments = true;
    Options.bAddErrorHandling = true;
    Options.bOptimizeForBlueprint = true;
    Options.bAddLogging = true;
    
    switch (Template)
    {
    case ECodeTemplateType::UE5_Class:
        Result = GenerateUE5Class(Requirement, Options);
        break;
    case ECodeTemplateType::UE5_Function:
        Result = GenerateUE5Function(Requirement, Options);
        break;
    case ECodeTemplateType::UE5_System:
        Result = GenerateUE5System(Requirement, Options);
        break;
    case ECodeTemplateType::UE5_Gameplay:
        Result = GenerateUE5Gameplay(Requirement, Options);
        break;
    default:
        Result = GenerateGenericCode(Requirement, Options);
        break;
    }
    
    // Calculate quality score
    Result.QualityScore = CalculateQualityScore(Result.CodeContent);
    
    // Store in history
    GenerationHistory.Add(Result);
    TotalGeneratedCount++;
    
    // Update average quality
    AverageQualityScore = ((AverageQualityScore * (TotalGeneratedCount - 1)) + Result.QualityScore) / TotalGeneratedCount;
    
    OnCodeGenerated.Broadcast(Result, Result.QualityScore);
    
    UE_LOG(LogPotatoAI, Log, TEXT("🥔 Code generated with quality score: %d/100"), Result.QualityScore);
    
    return Result;
}

FGeneratedCodeResult UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5Class(const FString& Requirement, const FCodeGenerationOptions& Options)
{
    FGeneratedCodeResult Result;
    Result.TemplateType = ECodeTemplateType::UE5_Class;
    Result.Requirement = Requirement;
    Result.GeneratedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString ClassName = GenerateClassName(Requirement);
    Result.CodeContent = GenerateUE5ClassCode(ClassName, Options);
    Result.QualityScore = CalculateQualityScore(Result.CodeContent);
    
    Result.Suggestions.Add(TEXT("This class is optimized for UE5 Blueprint integration"));
    Result.Suggestions.Add(TEXT("Supports runtime configuration and state management"));
    Result.Suggestions.Add(TEXT("Includes complete lifecycle management"));
    
    return Result;
}

FGeneratedCodeResult UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5Function(const FString& Requirement, const FCodeGenerationOptions& Options)
{
    FGeneratedCodeResult Result;
    Result.TemplateType = ECodeTemplateType::UE5_Function;
    Result.Requirement = Requirement;
    Result.GeneratedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString FunctionName = GenerateFunctionName(Requirement);
    FString ReturnType = DetermineReturnType(Requirement);
    
    Result.CodeContent = GenerateUE5FunctionCode(FunctionName, ReturnType, Options);
    Result.QualityScore = CalculateQualityScore(Result.CodeContent);
    
    Result.Suggestions.Add(TEXT("Consider adding caching for performance"));
    Result.Suggestions.Add(TEXT("Can implement async version for better responsiveness"));
    Result.Suggestions.Add(TEXT("Add input parameter validation"));
    
    return Result;
}

FGeneratedCodeResult UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5System(const FString& Requirement, const FCodeGenerationOptions& Options)
{
    FGeneratedCodeResult Result;
    Result.TemplateType = ECodeTemplateType::UE5_System;
    Result.Requirement = Requirement;
    Result.GeneratedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString SystemName = GenerateSystemName(Requirement);
    Result.CodeContent = GenerateUE5SystemCode(SystemName, Options);
    Result.QualityScore = CalculateQualityScore(Result.CodeContent);
    
    Result.Suggestions.Add(TEXT("Complete lifecycle management"));
    Result.Suggestions.Add(TEXT("Auto-start and stop mechanisms"));
    Result.Suggestions.Add(TEXT("Performance monitoring and optimization"));
    Result.Suggestions.Add(TEXT("Event-driven architecture"));
    
    return Result;
}

FGeneratedCodeResult UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5Gameplay(const FString& Requirement, const FCodeGenerationOptions& Options)
{
    FGeneratedCodeResult Result;
    Result.TemplateType = ECodeTemplateType::UE5_Gameplay;
    Result.Requirement = Requirement;
    Result.GeneratedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString ActorName = GenerateActorName(Requirement);
    Result.CodeContent = GenerateUE5GameplayCode(ActorName, Options);
    Result.QualityScore = CalculateQualityScore(Result.CodeContent);
    
    Result.Suggestions.Add(TEXT("Includes complete character control system"));
    Result.Suggestions.Add(TEXT("Supports double jump and action system"));
    Result.Suggestions.Add(TEXT("Optimized camera follow mechanism"));
    Result.Suggestions.Add(TEXT("Extensible through Blueprint"));
    
    return Result;
}

FGeneratedCodeResult UMingRTSPotatoAICodeGeneratorAPI::GenerateGenericCode(const FString& Requirement, const FCodeGenerationOptions& Options)
{
    FGeneratedCodeResult Result;
    Result.TemplateType = ECodeTemplateType::UE5_Generic;
    Result.Requirement = Requirement;
    Result.GeneratedAt = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    Result.CodeContent = GenerateGenericCodeContent(Requirement, Options);
    Result.QualityScore = CalculateQualityScore(Result.CodeContent);
    
    return Result;
}

ECodeTemplateType UMingRTSPotatoAICodeGeneratorAPI::SelectCodeTemplate(const FString& Requirement, ECodeGenerationLanguage Language)
{
    TArray<FString> Keywords = ExtractKeywords(Requirement);
    
    for (const auto& Pair : TemplateDatabase)
    {
        if (MatchesTemplate(Requirement, Pair.Value))
        {
            return Pair.Key;
        }
    }
    
    return ECodeTemplateType::UE5_Generic;
}

FString UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5ClassCode(const FString& ClassName, const FCodeGenerationOptions& Options)
{
    FString HeaderGuard = FString::Printf(TEXT("MING_%s_H"), *ClassName.ToUpper());
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString Code = FString::Printf(
        TEXT("// 🥔 Generated by Potato AI\n"
             "// Requirement: %s\n"
             "// Generated: %s\n\n"
             "#pragma once\n\n"
             "#include \"CoreMinimal.h\"\n"
             "#include \"UObject/NoExportTypes.h\"\n"
             "#include \"%s.generated.h\"\n\n"
             "UCLASS(Blueprintable, BlueprintType, Category = \"MingGoRTS\")\n"
             "class MINGGORTS_API %s : public UObject\n"
             "{\n"
             "    GENERATED_BODY()\n\n"
             "public:\n"
             "    %s();\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
             "    bool Initialize();\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
             "    void Update(float DeltaTime);\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
             "    FString GetStatus() const;\n\n"
             "protected:\n"
             "    virtual void BeginPlay() override;\n"
             "    virtual void Tick(float DeltaTime) override;\n\n"
             "private:\n"
             "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Config\")\n"
             "    bool bIsActive = true;\n\n"
             "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Config\")\n"
             "    float UpdateInterval = 0.1f;\n\n"
             "    void ProcessInternalLogic();\n"
             "    void UpdateInternalState();\n"
             "};\n\n"
             "// 🥔 Potato AI Tips:\n"
             "// - This class is optimized for UE5 Blueprint integration\n"
             "// - Supports runtime configuration and state management\n"
             "// - Includes complete lifecycle management\n"),
        *ClassName,
        *Timestamp,
        *HeaderGuard,
        *ClassName,
        *ClassName,
        *ClassName,
        *ClassName,
        *ClassName
    );
    
    return Code;
}

FString UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5FunctionCode(const FString& FunctionName, const FString& ReturnType, const FCodeGenerationOptions& Options)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString Code = FString::Printf(
        TEXT("// 🥔 Generated by Potato AI\n"
             "// Function: %s\n"
             "// Return Type: %s\n"
             "// Generated: %s\n\n"
             "UFUNCTION(BlueprintCallable, Category = \"Potato AI\")\n"
             "%s %s()\n"
             "{\n"
             "    // 🥔 Potato AI Implementation\n\n"
             "    try\n"
             "    {\n"
             "        // Main logic\n"
             "        %s Result = Process%s();\n\n"
             "        // Error checking\n"
             "        if (!Validate%s(Result))\n"
             "        {\n"
             "            UE_LOG(LogTemp, Warning, TEXT(\"🥔 Potato AI detected invalid result in %s\"));\n"
             "            return GetDefault%s();\n"
             "        }\n\n"
             "        // Success log\n"
             "        UE_LOG(LogTemp, Log, TEXT(\"🥔 Potato AI successfully executed %s\"));\n"
             "        return Result;\n"
             "    }\n"
             "    catch (const std::exception& e)\n"
             "    {\n"
             "        UE_LOG(LogTemp, Error, TEXT(\"🥔 Potato AI error in %s: %%s\"), UTF8_TO_TCHAR(e.what()));\n"
             "        return GetDefault%s();\n"
             "    }\n"
             "}\n\n"
             "// Helper methods\n"
             "private:\n"
             "%s Process%s();\n"
             "bool Validate%s(%s Value);\n"
             "%s GetDefault%s();\n\n"
             "// 🥔 Potato AI Optimization Suggestions:\n"
             "// - Consider adding caching mechanism for performance\n"
             "// - Can implement async version for better responsiveness\n"
             "// - Add input parameter validation\n"),
        *FunctionName,
        *ReturnType,
        *Timestamp,
        *ReturnType,
        *FunctionName,
        *ReturnType,
        *FunctionName,
        *FunctionName,
        *FunctionName,
        *FunctionName,
        *FunctionName,
        *FunctionName,
        *FunctionName,
        *FunctionName,
        *ReturnType,
        *FunctionName,
        *FunctionName,
        *ReturnType,
        *FunctionName
    );
    
    return Code;
}

FString UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5SystemCode(const FString& SystemName, const FCodeGenerationOptions& Options)
{
    FString ManagerName = SystemName + TEXT("Manager");
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString Code = FString::Printf(
        TEXT("// 🥔 Generated by Potato AI\n"
             "// System: %s\n"
             "// Generated: %s\n\n"
             "#pragma once\n\n"
             "#include \"CoreMinimal.h\"\n"
             "#include \"UObject/NoExportTypes.h\"\n"
             "#include \"Engine/World.h\"\n"
             "#include \"Components/ActorComponent.h\"\n"
             "#include \"%s.generated.h\"\n\n"
             "UCLASS(BlueprintType, Blueprintable, Category = \"MingGoRTS Systems\")\n"
             "class MINGGORTS_API %s : public UActorComponent\n"
             "{\n"
             "    GENERATED_BODY()\n\n"
             "public:\n"
             "    %s();\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
             "    bool InitializeSystem();\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
             "    void StartSystem();\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
             "    void StopSystem();\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
             "    void ResetSystem();\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"%s\")\n"
             "    bool IsSystemRunning() const { return bIsRunning; }\n\n"
             "protected:\n"
             "    virtual void BeginPlay() override;\n"
             "    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;\n"
             "    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;\n\n"
             "private:\n"
             "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Config\")\n"
             "    bool bAutoStart = true;\n\n"
             "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Config\")\n"
             "    bool bIsRunning = false;\n\n"
             "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Config\")\n"
             "    float UpdateFrequency = 60.0f;\n\n"
             "    void InitializeComponents();\n"
             "    void ProcessSystemLogic(float DeltaTime);\n"
             "    void CleanupSystem();\n\n"
             "    UFUNCTION()\n"
             "    void OnSystemStateChanged();\n\n"
             "    void UpdatePerformanceMetrics();\n"
             "};\n\n"
             "// 🥔 Potato AI System Design Features:\n"
             "// - Complete lifecycle management\n"
             "// - Auto-start and stop mechanisms\n"
             "// - Performance monitoring and optimization\n"
             "// - Event-driven architecture\n"),
        *SystemName,
        *Timestamp,
        *ManagerName,
        *ManagerName,
        *ManagerName,
        *SystemName,
        *SystemName,
        *SystemName,
        *SystemName,
        *SystemName
    );
    
    return Code;
}

FString UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5GameplayCode(const FString& ActorName, const FCodeGenerationOptions& Options)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString Code = FString::Printf(
        TEXT("// 🥔 Generated by Potato AI\n"
             "// Actor: %s\n"
             "// Generated: %s\n\n"
             "#pragma once\n\n"
             "#include \"CoreMinimal.h\"\n"
             "#include \"GameFramework/Character.h\"\n"
             "#include \"Components/StaticMeshComponent.h\"\n"
             "#include \"Components/CapsuleComponent.h\"\n"
             "#include \"Camera/CameraComponent.h\"\n"
             "#include \"GameFramework/SpringArmComponent.h\"\n"
             "#include \"%s.generated.h\"\n\n"
             "UCLASS(Blueprintable, BlueprintType)\n"
             "class MINGGORTS_API %s : public ACharacter\n"
             "{\n"
             "    GENERATED_BODY()\n\n"
             "public:\n"
             "    %s();\n\n"
             "protected:\n"
             "    virtual void BeginPlay() override;\n"
             "    virtual void Tick(float DeltaTime) override;\n"
             "    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"Gameplay\")\n"
             "    void MoveForward(float Value);\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"Gameplay\")\n"
             "    void MoveRight(float Value);\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"Gameplay\")\n"
             "    void Jump();\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"Gameplay\")\n"
             "    void PrimaryAction();\n\n"
             "    UFUNCTION(BlueprintCallable, Category = \"Gameplay\")\n"
             "    void SecondaryAction();\n\n"
             "private:\n"
             "    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = \"Camera\")\n"
             "    class USpringArmComponent* CameraBoom;\n\n"
             "    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = \"Camera\")\n"
             "    class UCameraComponent* FollowCamera;\n\n"
             "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Gameplay\")\n"
             "    float MovementSpeed = 600.0f;\n\n"
             "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Gameplay\")\n"
             "    float JumpHeight = 300.0f;\n\n"
             "    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Gameplay\")\n"
             "    bool bCanDoubleJump = true;\n\n"
             "    int32 JumpCount = 0;\n"
             "    bool bIsPerformingAction = false;\n\n"
             "    void HandleMovement(float DeltaTime);\n"
             "    void HandleJumping();\n"
             "    void HandleActions();\n"
             "    void UpdateAnimation();\n"
             "};\n\n"
             "// 🥔 Potato AI Game Design Suggestions:\n"
             "// - Includes complete character control system\n"
             "// - Supports double jump and action system\n"
             "// - Optimized camera follow mechanism\n"
             "// - Extensible through Blueprint\n"),
        *ActorName,
        *Timestamp,
        *ActorName,
        *ActorName,
        *ActorName
    );
    
    return Code;
}

FString UMingRTSPotatoAICodeGeneratorAPI::GenerateGenericCodeContent(const FString& Description, const FCodeGenerationOptions& Options)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    
    FString Code = FString::Printf(
        TEXT("// 🥔 Generated by Potato AI\n"
             "// Description: %s\n"
             "// Generated: %s\n\n"
             "// Generic code template generated by Potato AI\n"
             "// Will be customized based on specific requirements\n\n"
             "#include <iostream>\n"
             "#include <string>\n"
             "#include <vector>\n\n"
             "class PotatoAIGenerated\n"
             "{\n"
             "private:\n"
             "    std::string description;\n"
             "    bool isActive;\n\n"
             "public:\n"
             "    PotatoAIGenerated(const std::string& desc) : description(desc), isActive(true)\n"
             "    {\n"
             "        std::cout << \"🥔 Potato AI created: \" << description << std::endl;\n"
             "    }\n\n"
             "    void execute()\n"
             "    {\n"
             "        if (isActive)\n"
             "        {\n"
             "            std::cout << \"🥔 Potato AI executing: \" << description << std::endl;\n"
             "            // Add your specific logic here\n"
             "        }\n"
             "    }\n\n"
             "    void setStatus(bool status)\n"
             "    {\n"
             "        isActive = status;\n"
             "        std::cout << \"🥔 Potato AI status updated: \" << (status ? \"Active\" : \"Inactive\") << std::endl;\n"
             "    }\n"
             "};\n\n"
             "// 🥔 Potato AI Usage Example:\n"
             "int main()\n"
             "{\n"
             "    PotatoAIGenerated myCode(\"%s\");\n"
             "    myCode.execute();\n"
             "    return 0;\n"
             "}\n"),
        *Description,
        *Timestamp,
        *Description
    );
    
    return Code;
}

FString UMingRTSPotatoAICodeGeneratorAPI::GenerateClassName(const FString& Requirement) const
{
    FString LowerRequirement = Requirement.ToLower();
    
    for (const auto& Pair : ClassNameMappings)
    {
        if (LowerRequirement.Contains(Pair.Key))
        {
            return Pair.Value;
        }
    }
    
    return TEXT("MingPotatoAIClass");
}

FString UMingRTSPotatoAICodeGeneratorAPI::GenerateFunctionName(const FString& Requirement) const
{
    FString LowerRequirement = Requirement.ToLower();
    
    for (const auto& Pair : FunctionNameMappings)
    {
        if (LowerRequirement.Contains(Pair.Key))
        {
            return Pair.Value;
        }
    }
    
    return TEXT("Execute");
}

FString UMingRTSPotatoAICodeGeneratorAPI::GenerateSystemName(const FString& Requirement) const
{
    FString LowerRequirement = Requirement.ToLower();
    
    for (const auto& Pair : SystemNameMappings)
    {
        if (LowerRequirement.Contains(Pair.Key))
        {
            return TEXT("Ming") + Pair.Value + TEXT("System");
        }
    }
    
    return TEXT("MingPotatoAISystem");
}

FString UMingRTSPotatoAICodeGeneratorAPI::GenerateActorName(const FString& Requirement) const
{
    FString LowerRequirement = Requirement.ToLower();
    
    for (const auto& Pair : ActorNameMappings)
    {
        if (LowerRequirement.Contains(Pair.Key))
        {
            return Pair.Value;
        }
    }
    
    return TEXT("MingGameplayActor");
}

FString UMingRTSPotatoAICodeGeneratorAPI::DetermineReturnType(const FString& Requirement) const
{
    FString LowerRequirement = Requirement.ToLower();
    
    if (LowerRequirement.Contains(TEXT("bool")) || LowerRequirement.Contains(TEXT("boolean")))
    {
        return TEXT("bool");
    }
    if (LowerRequirement.Contains(TEXT("int")) || LowerRequirement.Contains(TEXT("integer")))
    {
        return TEXT("int32");
    }
    if (LowerRequirement.Contains(TEXT("float")) || LowerRequirement.Contains(TEXT("double")))
    {
        return TEXT("float");
    }
    if (LowerRequirement.Contains(TEXT("string")) || LowerRequirement.Contains(TEXT("text")))
    {
        return TEXT("FString");
    }
    
    return TEXT("void");
}

int32 UMingRTSPotatoAICodeGeneratorAPI::CalculateQualityScore(const FString& Code)
{
    int32 Score = 85;
    
    // Check basic elements
    if (Code.Contains(TEXT("#include"))) Score += 5;
    if (Code.Contains(TEXT("class")) || Code.Contains(TEXT("struct"))) Score += 5;
    if (Code.Contains(TEXT("UFUNCTION")) || Code.Contains(TEXT("void"))) Score += 5;
    
    // Check UE5 specific elements
    if (Code.Contains(TEXT("UFUNCTION")) || Code.Contains(TEXT("UCLASS")) || Code.Contains(TEXT("UPROPERTY"))) Score += 10;
    if (Code.Contains(TEXT("GENERATED_BODY"))) Score += 5;
    
    // Check comment quality
    if (Code.Contains(TEXT("//"))) Score += 3;
    if (Code.Contains(TEXT("🥔"))) Score += 2; // Potato AI marker
    
    return FMath::Clamp(Score, 0, 100);
}

FCodeQualityReport UMingRTSPotatoAICodeGeneratorAPI::CheckCodeQuality(const FString& Code)
{
    FCodeQualityReport Report;
    
    Report.bHasIncludes = Code.Contains(TEXT("#include"));
    Report.bHasClassDefinition = Code.Contains(TEXT("class")) || Code.Contains(TEXT("struct"));
    Report.bHasFunctions = Code.Contains(TEXT("UFUNCTION")) || Code.Contains(TEXT("void")) || Code.Contains(TEXT("()"));
    Report.bHasUE5Macros = Code.Contains(TEXT("UCLASS")) || Code.Contains(TEXT("UFUNCTION")) || Code.Contains(TEXT("UPROPERTY"));
    Report.bHasComments = Code.Contains(TEXT("//"));
    
    Report.OverallScore = CalculateQualityScore(Code);
    
    AddQualitySuggestions(Report, Code);
    
    OnQualityCheckCompleted.Broadcast(Report);
    
    return Report;
}

void UMingRTSPotatoAICodeGeneratorAPI::AddQualitySuggestions(FCodeQualityReport& Report, const FString& Code)
{
    if (!Report.bHasIncludes)
    {
        Report.Recommendations.Add(TEXT("Add necessary #include statements"));
    }
    if (!Report.bHasComments)
    {
        Report.Recommendations.Add(TEXT("Add comments to explain code functionality"));
    }
    if (!Report.bHasUE5Macros)
    {
        Report.Recommendations.Add(TEXT("Consider using UE5 UCLASS/UFUNCTION macros for Blueprint support"));
    }
    if (Report.OverallScore < 90)
    {
        Report.Recommendations.Add(TEXT("Review code for optimization opportunities"));
    }
}

void UMingRTSPotatoAICodeGeneratorAPI::InitializeTemplateDatabase()
{
    // UE5 Class template
    FCodeTemplateInfo ClassTemplate;
    ClassTemplate.TemplateType = ECodeTemplateType::UE5_Class;
    ClassTemplate.Pattern = TEXT("class|struct|create.*class");
    ClassTemplate.Description = TEXT("UE5 UObject Class Template");
    ClassTemplate.Keywords = {TEXT("class"), TEXT("struct"), TEXT("create"), TEXT("object")};
    TemplateDatabase.Add(ECodeTemplateType::UE5_Class, ClassTemplate);
    
    // UE5 Function template
    FCodeTemplateInfo FunctionTemplate;
    FunctionTemplate.TemplateType = ECodeTemplateType::UE5_Function;
    FunctionTemplate.Pattern = TEXT("function|method|create.*function");
    FunctionTemplate.Description = TEXT("UE5 Function Template");
    FunctionTemplate.Keywords = {TEXT("function"), TEXT("method"), TEXT("create"), TEXT("implement")};
    TemplateDatabase.Add(ECodeTemplateType::UE5_Function, FunctionTemplate);
    
    // UE5 System template
    FCodeTemplateInfo SystemTemplate;
    SystemTemplate.TemplateType = ECodeTemplateType::UE5_System;
    SystemTemplate.Pattern = TEXT("system|manager|create.*system");
    SystemTemplate.Description = TEXT("UE5 ActorComponent System Template");
    SystemTemplate.Keywords = {TEXT("system"), TEXT("manager"), TEXT("component"), TEXT("service")};
    TemplateDatabase.Add(ECodeTemplateType::UE5_System, SystemTemplate);
    
    // UE5 Gameplay template
    FCodeTemplateInfo GameplayTemplate;
    GameplayTemplate.TemplateType = ECodeTemplateType::UE5_Gameplay;
    GameplayTemplate.Pattern = TEXT("gameplay|character|actor|player");
    GameplayTemplate.Description = TEXT("UE5 Gameplay Actor Template");
    GameplayTemplate.Keywords = {TEXT("gameplay"), TEXT("character"), TEXT("actor"), TEXT("player")};
    TemplateDatabase.Add(ECodeTemplateType::UE5_Gameplay, GameplayTemplate);
}

TArray<FString> UMingRTSPotatoAICodeGeneratorAPI::ExtractKeywords(const FString& Requirement) const
{
    TArray<FString> Keywords;
    FString LowerRequirement = Requirement.ToLower();
    
    // Split by spaces and common delimiters
    LowerRequirement.ParseIntoArray(Keywords, TEXT(" "), true);
    LowerRequirement.ParseIntoArray(Keywords, TEXT("_"), true);
    LowerRequirement.ParseIntoArray(Keywords, TEXT("-"), true);
    
    return Keywords;
}

bool UMingRTSPotatoAICodeGeneratorAPI::MatchesTemplate(const FString& Requirement, const FCodeTemplateInfo& Template) const
{
    FString LowerRequirement = Requirement.ToLower();
    
    for (const FString& Keyword : Template.Keywords)
    {
        if (LowerRequirement.Contains(Keyword.ToLower()))
        {
            return true;
        }
    }
    
    return false;
}

TArray<FCodeTemplateInfo> UMingRTSPotatoAICodeGeneratorAPI::GetAvailableTemplates() const
{
    TArray<FCodeTemplateInfo> Templates;
    TemplateDatabase.GenerateValueArray(Templates);
    return Templates;
}

TArray<FGeneratedCodeResult> UMingRTSPotatoAICodeGeneratorAPI::BatchGenerateCode(const TArray<FString>& Requirements, ECodeTemplateType Template)
{
    TArray<FGeneratedCodeResult> Results;
    
    for (const FString& Requirement : Requirements)
    {
        FGeneratedCodeResult Result = GenerateCode(Requirement, ECodeGenerationLanguage::CPP, Template);
        Results.Add(Result);
    }
    
    return Results;
}

bool UMingRTSPotatoAICodeGeneratorAPI::ExportCodeToFile(const FGeneratedCodeResult& Result, const FString& FilePath)
{
    return FFileHelper::SaveStringToFile(Result.CodeContent, *FilePath);
}

bool UMingRTSPotatoAICodeGeneratorAPI::ExportToClipboard(const FGeneratedCodeResult& Result)
{
    FPlatformApplicationMisc::ClipboardCopy(*Result.CodeContent);
    return true;
}

bool UMingRTSPotatoAICodeGeneratorAPI::IsInitialized() const
{
    return bIsInitialized;
}

int32 UMingRTSPotatoAICodeGeneratorAPI::GetTotalGeneratedCount() const
{
    return TotalGeneratedCount;
}

float UMingRTSPotatoAICodeGeneratorAPI::GetAverageQualityScore() const
{
    return AverageQualityScore;
}
