#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIContentTypes.h"
#include "MingAIGeneratedContentSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIContentGenerationCompleted, const FGuid&, RequestID, const FMingAIContentResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIContentGenerationFailed, const FGuid&, RequestID, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAIContentGenerationProgress, const FGuid&, RequestID, float, Progress, const FString&, Status);

/**
 * AI Content Generation System
 * Core system for generating game content using AI services
 */
UCLASS(ClassGroup = (AI, Content), Blueprintable)
class MINGAI_API UMingAIGeneratedContentSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAIGeneratedContentSystem();

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void InitializeSystem();

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void ShutdownSystem();

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SetProviderConfig(EMingAIProvider Provider, const FMingAIProviderConfig& Config);

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    FMingAIProviderConfig GetProviderConfig(EMingAIProvider Provider) const;

    // Content Generation
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FGuid GenerateContent(const FMingAIContentRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    bool CancelGeneration(const FGuid& RequestID);

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    EMingAIGenerationStatus GetGenerationStatus(const FGuid& RequestID) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    TArray<FGuid> GetActiveGenerations() const;

    // Convenience Methods
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FGuid GenerateImage(const FString& Prompt, EMingAIQualityLevel Quality, int32 Width, int32 Height);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FGuid GenerateMusic(const FString& Prompt, float Duration, EMingAIQualityLevel Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FGuid GenerateSoundEffect(const FString& Prompt, float Duration, EMingAIQualityLevel Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FGuid GenerateVoice(const FString& Text, const FString& VoiceID, EMingAIQualityLevel Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FGuid GenerateTexture(const FString& Prompt, EMingAIQualityLevel Quality, int32 Width, int32 Height);

    // Results
    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    FMingAIContentResult GetGenerationResult(const FGuid& RequestID) const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void ClearCompletedGenerations();

    // Statistics
    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    int32 GetTotalGenerationsCount() const { return TotalGenerationsCount; }

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    int32 GetSuccessfulGenerationsCount() const { return SuccessfulGenerationsCount; }

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    int32 GetFailedGenerationsCount() const { return FailedGenerationsCount; }

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    float GetAverageGenerationTime() const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "AI Content Events")
    FOnAIContentGenerationCompleted OnGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Content Events")
    FOnAIContentGenerationFailed OnGenerationFailed;

    UPROPERTY(BlueprintAssignable, Category = "AI Content Events")
    FOnAIContentGenerationProgress OnGenerationProgress;

protected:
    UPROPERTY()
    TMap<EMingAIProvider, FMingAIProviderConfig> ProviderConfigs;

    UPROPERTY()
    TMap<FGuid, FMingAIContentRequest> PendingRequests;

    UPROPERTY()
    TMap<FGuid, FMingAIContentResult> CompletedResults;

    UPROPERTY()
    TSet<FGuid> ActiveRequests;

    UPROPERTY()
    int32 TotalGenerationsCount;

    UPROPERTY()
    int32 SuccessfulGenerationsCount;

    UPROPERTY()
    int32 FailedGenerationsCount;

    UPROPERTY()
    TArray<float> GenerationTimes;

    UPROPERTY()
    bool bInitialized;

    // Internal methods
    void ProcessNextInQueue();
    void OnGenerationRequestCompleted(FGuid RequestID, bool bSuccess, const FString& FilePath, const FString& Error);
    void UpdateGenerationProgress(FGuid RequestID, float Progress, const FString& Status);
    void ProcessGenerationResult(FGuid RequestID, bool bSuccess, const FString& FilePath, const FString& Error);
    
    FString GetProviderEndpoint(EMingAIProvider Provider) const;
    FString BuildRequestPayload(const FMingAIContentRequest& Request) const;
    bool ValidateRequest(const FMingAIContentRequest& Request) const;
    FString GetOutputDirectory(EMingAIContentType ContentType) const;
    FString GenerateUniqueFileName(EMingAIContentType ContentType) const;
    
    void SendHttpRequest(const FGuid& RequestID, const FMingAIContentRequest& Request);
    void HandleHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FGuid RequestID);
};
