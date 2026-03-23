// Copyright (c) 2026 MingGoRTS. All rights reserved.
// PowerShell to C++ API Conversion Example
// This file demonstrates how to use the converted C++ APIs

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "API/MingRTSAPISystem.h"
#include "API/MingRTSUE5LearningAPI.h"
#include "API/MingRTSPotatoAICodeGeneratorAPI.h"
#include "PowerShellAPIConversionExample.generated.h"

/**
 * Example class showing how to use the converted PowerShell APIs in C++
 */
UCLASS()
class MINGRTS_API APowerShellAPIConversionExample : public AActor
{
    GENERATED_BODY()

public:
    APowerShellAPIConversionExample();

    virtual void BeginPlay() override;

    // ===== UE5 Learning API Examples =====
    
    UFUNCTION(BlueprintCallable, Category = "API Examples|UE5 Learning")
    void Example_StartUE5Learning();

    UFUNCTION(BlueprintCallable, Category = "API Examples|UE5 Learning")
    void Example_FetchDocumentation();

    UFUNCTION(BlueprintCallable, Category = "API Examples|UE5 Learning")
    void Example_GetAPIPatterns();

    UFUNCTION(BlueprintCallable, Category = "API Examples|UE5 Learning")
    void Example_GenerateLearningReport();

    // ===== Potato AI Code Generator Examples =====
    
    UFUNCTION(BlueprintCallable, Category = "API Examples|Code Generation")
    void Example_GenerateUE5Class();

    UFUNCTION(BlueprintCallable, Category = "API Examples|Code Generation")
    void Example_GenerateUE5Function();

    UFUNCTION(BlueprintCallable, Category = "API Examples|Code Generation")
    void Example_GenerateUE5System();

    UFUNCTION(BlueprintCallable, Category = "API Examples|Code Generation")
    void Example_BatchCodeGeneration();

    UFUNCTION(BlueprintCallable, Category = "API Examples|Code Generation")
    void Example_CheckCodeQuality();

    // ===== API System Coordination Examples =====
    
    UFUNCTION(BlueprintCallable, Category = "API Examples|Coordination")
    void Example_CoordinateLearningAndGeneration();

    UFUNCTION(BlueprintCallable, Category = "API Examples|Coordination")
    void Example_ExecuteIntelligentWorkflow();

protected:
    UPROPERTY()
    TObjectPtr<UMingRTSAPISystem> APISystem;

    UPROPERTY()
    TObjectPtr<UMingRTSUE5LearningAPI> LearningAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSPotatoAICodeGeneratorAPI> CodeGeneratorAPI;

    // Event handlers
    UFUNCTION()
    void OnLearningCompleted(const FUE5LearningReport& Report);

    UFUNCTION()
    void OnCodeGenerated(const FGeneratedCodeResult& Result, int32 QualityScore);

    UFUNCTION()
    void OnLearningProgress(const FString& Section, float Progress);
};
