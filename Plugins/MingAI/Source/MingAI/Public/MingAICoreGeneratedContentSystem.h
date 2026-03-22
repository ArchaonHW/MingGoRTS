#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAICoreGeneratedContentSystem.generated.h"

/**
 * AI Core Generated Content System
 * Manages AI-generated content for the MingGoRTS game
 */
UCLASS(BlueprintType, Blueprintable)
class MINGAI_API UMingAICoreGeneratedContentSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAICoreGeneratedContentSystem();

    /**
     * Initialize the AI content generation system
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content")
    void InitializeAIContentSystem();

    /**
     * Generate terrain content
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content")
    bool GenerateTerrainContent(const FString& TerrainType, const FVector& Location);

    /**
     * Generate unit content
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content")
    bool GenerateUnitContent(const FString& UnitType, const FVector& SpawnLocation);

    /**
     * Generate building content
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content")
    bool GenerateBuildingContent(const FString& BuildingType, const FVector& BuildLocation);

    /**
     * Generate audio content
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content")
    bool GenerateAudioContent(const FString& AudioType, const FString& Context);

    /**
     * Generate visual content
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content")
    bool GenerateVisualContent(const FString& VisualType, const FString& Style);

    /**
     * Get generated content status
     */
    UFUNCTION(BlueprintPure, Category = "AI Content")
    bool IsContentGenerationComplete() const;

    /**
     * Cancel content generation
     */
    UFUNCTION(BlueprintCallable, Category = "AI Content")
    void CancelContentGeneration();

protected:
    // Is content generation in progress
    UPROPERTY()
    bool bIsGeneratingContent;

    // Current generation task
    UPROPERTY()
    FString CurrentGenerationTask;

    // Generation progress
    UPROPERTY()
    float GenerationProgress;

    // Process terrain generation
    bool ProcessTerrainGeneration(const FString& TerrainType, const FVector& Location);

    // Process unit generation
    bool ProcessUnitGeneration(const FString& UnitType, const FVector& SpawnLocation);

    // Process building generation
    bool ProcessBuildingGeneration(const FString& BuildingType, const FVector& BuildLocation);

    // Process audio generation
    bool ProcessAudioGeneration(const FString& AudioType, const FString& Context);

    // Process visual generation
    bool ProcessVisualGeneration(const FString& VisualType, const FString& Style);

    // Validate generation parameters
    bool ValidateGenerationParameters(const FString& ContentType, const FString& Parameters);

    // Log generation progress
    void LogGenerationProgress(const FString& Task, float Progress);

    // Handle generation completion
    void OnGenerationComplete(const FString& Task, bool bSuccess);

    // Handle generation error
    void OnGenerationError(const FString& Task, const FString& ErrorMessage);
};
