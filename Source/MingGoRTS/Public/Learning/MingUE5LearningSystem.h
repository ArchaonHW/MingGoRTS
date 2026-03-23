// MingGoRTS UE5.7 API Learning System
// Generated: 2026-03-23 14:01:33

#pragma once

#include 'CoreMinimal.h'
#include 'UObject/NoExportTypes.h'

/**
 * UE5.7 API Learning System
 * ?其?摮虫????｜nreal Engine 5.7?PI
 */
UCLASS(Blueprintable, BlueprintType)
class MINGGORTS_API UMingUE5LearningSystem : public UObject
{
    GENERATED_BODY()

public:
    // Constructor
    UMingUE5LearningSystem();

    // Initialize learning system
    UFUNCTION(BlueprintCallable, Category = 'UE5 Learning')
    bool InitializeLearningSystem();

    // Learn specific API
    UFUNCTION(BlueprintCallable, Category = 'UE5 Learning')
    bool LearnAPI(const FString& APIName);

    // Get learning progress
    UFUNCTION(BlueprintCallable, Category = 'UE5 Learning')
    float GetLearningProgress() const;

    // Generate learning report
    UFUNCTION(BlueprintCallable, Category = 'UE5 Learning')
    bool GenerateLearningReport(const FString& ReportPath);

protected:
    // API knowledge base
    UPROPERTY(BlueprintReadOnly, Category = 'UE5 Learning')
    TMap<FString, FString> APIKnowledgeBase;

    // Learning progress
    UPROPERTY(BlueprintReadOnly, Category = 'UE5 Learning')
    int32 LearnedAPIs;

    // Total APIs to learn
    UPROPERTY(BlueprintReadOnly, Category = 'UE5 Learning')
    int32 TotalAPIs;

private:
    // Initialize API knowledge base
    void InitializeAPIKnowledgeBase();

    // Update learning progress
    void UpdateLearningProgress();

    // Validate API name
    bool IsValidAPIName(const FString& APIName) const;
};

