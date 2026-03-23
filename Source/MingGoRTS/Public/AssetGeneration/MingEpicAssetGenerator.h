#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "MingEpicAssetGenerator.generated.h"

UENUM(BlueprintType)
enum class EMingAssetType : uint8
{
    Images,
    Music,
    SFX,
    Video
};

UENUM(BlueprintType)
enum class EMingEpicId : uint8
{
    Epic1,
    Epic2,
    Epic3,
    Epic4,
    Epic5,
    Epic6
};

USTRUCT(BlueprintType)
struct FMingAssetConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    FString Prompt;

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    FString Category;

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    FString Resolution;

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    int32 Duration;

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    int32 Count;

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    int32 Variations;

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    bool bLoop;

    UPROPERTY(BlueprintReadWrite, Category = "Asset Config")
    FString Style;

    FMingAssetConfig()
    {
        Name = TEXT("");
        Description = TEXT("");
        Prompt = TEXT("");
        Category = TEXT("");
        Resolution = TEXT("512x512");
        Duration = 30;
        Count = 1;
        Variations = 1;
        bLoop = false;
        Style = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingEpicConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Epic Config")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "Epic Config")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Epic Config")
    TMap<EMingAssetType, TArray<FMingAssetConfig>> Assets;

    FMingEpicConfig()
    {
        Name = TEXT("");
        Description = TEXT("");
        Assets.Empty();
    }
};

USTRUCT(BlueprintType)
struct FMingGenerationProgress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Generation Progress")
    FString AssetKey;

    UPROPERTY(BlueprintReadWrite, Category = "Generation Progress")
    FString Status;

    UPROPERTY(BlueprintReadWrite, Category = "Generation Progress")
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadWrite, Category = "Generation Progress")
    FString Error;

    FMingGenerationProgress()
    {
        AssetKey = TEXT("");
        Status = TEXT("");
        Timestamp = FDateTime::Now();
        Error = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingGenerationResults
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Generation Results")
    TArray<FString> Success;

    UPROPERTY(BlueprintReadWrite, Category = "Generation Results")
    TArray<FString> Failed;

    UPROPERTY(BlueprintReadWrite, Category = "Generation Results")
    TArray<FString> Skipped;

    FMingGenerationResults()
    {
        Success.Empty();
        Failed.Empty();
        Skipped.Empty();
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssetGenerationCompleted, const FString&, AssetName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssetGenerationFailed, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGenerationProgress, const FString&, EpicId, float, Progress);

/**
 * MingGoRTS Epic Asset Generation System
 * 
 * This system provides comprehensive asset generation capabilities,
 * replacing the original Python-based epic asset generator.
 * 
 * Features:
 * - 6 Epic configurations with detailed asset specifications
 * - Multiple asset types (Images, Music, SFX, Video)
 * - Progress tracking and error handling
 * - Placeholder generation for development
 * - C++ native performance
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Asset Generation")
class MINGGORTS_API UMingEpicAssetGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingEpicAssetGenerator();

    // Initialize the epic asset generator
    UFUNCTION(BlueprintCallable, Category = "Asset Generation")
    void InitializeEpicAssetGenerator();

    // Generate assets for specific epic
    UFUNCTION(BlueprintCallable, Category = "Asset Generation")
    FMingGenerationResults GenerateAssetsForEpic(EMingEpicId EpicId, const TArray<EMingAssetType>& AssetTypes = TArray<EMingAssetType>(), bool bForce = false);

    // Generate all epics
    UFUNCTION(BlueprintCallable, Category = "Asset Generation")
    bool GenerateAllEpics(const TArray<EMingAssetType>& AssetTypes = TArray<EMingAssetType>(), bool bForce = false);

    // List all epics
    UFUNCTION(BlueprintCallable, Category = "Asset Generation")
    void ListEpics();

    // Clean generated assets
    UFUNCTION(BlueprintCallable, Category = "Asset Generation")
    void CleanGeneratedAssets();

    // Get epic configuration
    UFUNCTION(BlueprintCallable, Category = "Asset Generation")
    FMingEpicConfig GetEpicConfig(EMingEpicId EpicId);

    // Get generation progress
    UFUNCTION(BlueprintCallable, Category = "Asset Generation")
    TMap<FString, FMingGenerationProgress> GetGenerationProgress();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Asset Generation")
    FOnAssetGenerationCompleted OnAssetGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Asset Generation")
    FOnAssetGenerationFailed OnAssetGenerationFailed;

    UPROPERTY(BlueprintAssignable, Category = "Asset Generation")
    FOnGenerationProgress OnGenerationProgress;

protected:
    // Initialize epic configurations
    void InitializeEpicConfigs();

    // Generate placeholder image
    bool GeneratePlaceholderImage(const FString& Name, const FString& Category, const FString& Resolution, const FString& OutputDir);

    // Generate placeholder audio
    bool GeneratePlaceholderAudio(const FString& Name, int32 Duration, const FString& Category, const FString& OutputDir);

    // Generate placeholder video
    bool GeneratePlaceholderVideo(const FString& Name, int32 Duration, const FString& Style, const FString& OutputDir);

    // Save configuration
    void SaveConfig();

    // Load configuration
    void LoadConfig();

    // Save progress
    void SaveProgress();

    // Load progress
    void LoadProgress();

    // Create output structure
    void CreateOutputStructure();

    // Write log
    void WriteLog(const FString& Message, const FString& Level = TEXT("INFO"));

private:
    // Epic configurations
    UPROPERTY()
    TMap<EMingEpicId, FMingEpicConfig> EpicConfigs;

    // Generation progress
    UPROPERTY()
    TMap<FString, FMingGenerationProgress> GenerationProgress;

    // Project paths
    FString ProjectRoot;
    FString OutputBase;
    FString ConfigPath;
    FString ProgressFilePath;

    // System state
    bool bIsInitialized;
};
