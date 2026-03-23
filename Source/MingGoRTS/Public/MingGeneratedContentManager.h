#pragma once


#include "CoreMinimal.h"
#include "MingGoRTS.h"

// Generated Content Integration
USTRUCT(BlueprintType)
struct FGeneratedAsset
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConfigPath;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Quality;
};

UCLASS(BlueprintType)
class MINGRTS_API UMingGeneratedContentManager : public UObject
{
    GENERATED_BODY()

public:
    // Load generated content
    UFUNCTION(BlueprintCallable)
    static bool LoadGeneratedContent(const FString& ContentPath};
    
    // Get asset list
    UFUNCTION(BlueprintCallable)
    static TArray<FGeneratedAsset> GetGeneratedAssets(const FString& AssetType};
    
    // Initialize content system
    UFUNCTION(BlueprintCallable)
    static void InitializeGeneratedContent(};
};
