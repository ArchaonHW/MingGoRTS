// MingRTSCppReferenceIntegration.h - C++ Reference Integration for Coding API
#pragma once
#include "CoreMinimal.h"
#include "MingRTSCodingAPI.h"

USTRUCT()
struct FCppReferenceCategory
{
    GENERATED_BODY()
    UPROPERTY() FString CategoryName;
    UPROPERTY() FString ChineseTitle;
    UPROPERTY() FString Description;
    UPROPERTY() FString BaseUrl;
    UPROPERTY() TArray<FString> Topics;
};

UCLASS()
class MINGGORTS_API UMingRTSCppReferenceIntegration : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable) bool LoadCppReferenceDocs();
    UFUNCTION(BlueprintCallable) TArray<FCppReferenceCategory> GetAllCategories() const;
    UFUNCTION(BlueprintCallable) FString GenerateCodeFromReference(const FString& Category, const FString& Topic);
    UFUNCTION(BlueprintCallable) TArray<FString> GetRelevantTopics(ECodingTaskType TaskType) const;
    UFUNCTION(BlueprintCallable) FString CreateCodeTemplate(ECodingAPIDomain Domain, const FString& Topic);
private:
    UPROPERTY() TMap<FString, FCppReferenceCategory> CategoryMap;
    UPROPERTY() FString DocsBasePath;
    void InitializeDefaultCategories();
    FString ReadDocFile(const FString& CategoryName);
    TArray<FString> ParseTopics(const FString& Content);
};
