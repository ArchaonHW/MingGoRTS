#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "MingRTSAssetGenerationAPI.generated.h"

/**
 * 資產生成API系統
 * 專門負責Epic資產和遊戲資產生成功能 (從PythonAPI分配而來)
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSAssetGenerationAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAssetGenerationAPI();

    // 初始化資產生成系統
    UFUNCTION(BlueprintCallable, Category = "Asset Generation API")
    bool InitializeAssetGeneration();

    // Epic資產生成器功能
    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Epic")
    bool GenerateAssetsForEpic(const FString& EpicID, const TArray<FString>& AssetTypes = TArray<FString>(), bool bForce = false);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Epic")
    bool GenerateAllEpicAssets(const TArray<FString>& AssetTypes = TArray<FString>(), bool bForce = false);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Epic")
    void ListEpicAssets();

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Epic")
    TArray<FString> GetAvailableEpics();

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Epic")
    bool AddEpicConfiguration(const FString& EpicID, const FString& Name, const FString& Description);

    // 遊戲資產生成功能
    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Game")
    FString GenerateGameScene(const FString& SceneName, const FString& Description, EAssetQuality Quality = EAssetQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Game")
    FString GenerateCharacterAsset(const FString& CharacterName, const FString& Description, EAssetQuality Quality = EAssetQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Game")
    FString GenerateBuildingAsset(const FString& BuildingName, const FString& Description, EAssetQuality Quality = EAssetQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Game")
    FString GenerateEnvironmentAsset(const FString& EnvironmentName, const FString& Description, EAssetQuality Quality = EAssetQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Game")
    FString GenerateUIAsset(const FString& UIName, const FString& Description, EAssetQuality Quality = EAssetQuality::Standard);

    // 批量資產生成
    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Batch")
    FString GenerateGameAssetsBatch(const TArray<FAssetRequirement>& Requirements);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Batch")
    bool GenerateAssetPackage(const FString& PackageName, const TArray<FString>& AssetIDs);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Batch")
    TArray<FString> GetBatchGenerationProgress(const FString& BatchID);

    // 資產管理功能
    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Management")
    void CleanGeneratedAssets();

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Management")
    bool ExportAssetPackage(const FString& PackageID, const FString& ExportPath);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Management")
    bool ImportAssetPackage(const FString& ImportPath);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Management")
    TArray<FString> GetAssetLibrary();

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Management")
    bool ValidateAssetIntegrity(const FString& AssetID);

    // 資產優化功能
    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Optimization")
    FString OptimizeAsset(const FString& AssetID);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Optimization")
    bool CompressAssetPackage(const FString& PackageID);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Optimization")
    TArray<FString> GetOptimizationSuggestions(const FString& AssetID);

    // 資產分析功能
    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Analysis")
    TMap<FString, FString> AnalyzeAssetMetadata(const FString& AssetID);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Analysis")
    float CalculateAssetQuality(const FString& AssetID);

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Analysis")
    TArray<FString> FindSimilarAssets(const FString& AssetID);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnAssetGenerated, const FString&, AssetID, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnBatchGenerationStarted, const FString&, BatchID, int32, AssetCount);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParam(FOnBatchGenerationProgress, const FString&, BatchID, int32, Completed, int32, Total);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnBatchGenerationCompleted, const FString&, BatchID, bool, bSuccess);

    UPROPERTY(BlueprintAssignable, Category = "Asset Generation|Events")
    FOnAssetGenerated OnAssetGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Asset Generation|Events")
    FOnBatchGenerationStarted OnBatchGenerationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Asset Generation|Events")
    FOnBatchGenerationProgress OnBatchGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "Asset Generation|Events")
    FOnBatchGenerationCompleted OnBatchGenerationCompleted;

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Status")
    bool IsInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Status")
    FString GetSystemStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Status")
    TArray<FString> GetAvailableFunctions() const;

    UFUNCTION(BlueprintCallable, Category = "Asset Generation|Status")
    TMap<FString, int32> GetAssetStatistics() const;

private:
    bool bIsInitialized;
    
    // Epic資產配置
    TMap<FString, TSharedPtr<FJsonObject>> EpicAssetsConfig;
    
    // 資產生成模板
    TMap<FString, TSharedPtr<FJsonObject>> AssetTemplates;
    
    // 批量生成追蹤
    TMap<FString, TArray<FString>> BatchGenerationProgress;
    
    // 資產庫索引
    TMap<FString, TSharedPtr<FJsonObject>> AssetLibrary;

    // 內部輔助函數
    bool LoadEpicAssetsConfig();
    bool LoadAssetTemplates();
    bool LoadAssetLibrary();
    
    FString GeneratePlaceholderAsset(const FString& AssetType, const FString& Name, const FString& Category);
    bool ProcessAssetGeneration(const FString& AssetID, const FString& AssetType);
    
    FString GetAssetOutputDirectory(const FString& AssetType) const;
    bool EnsureAssetDirectoryExists(const FString& DirectoryPath) const;
    FString GenerateUniqueAssetID(const FString& Prefix) const;
    
    // 資產處理輔助函數
    bool ValidateAssetFile(const FString& AssetPath);
    TSharedPtr<FJsonObject> LoadJsonFile(const FString& FilePath) const;
    bool SaveJsonFile(const FString& FilePath, TSharedPtr<FJsonObject> JsonObject) const;
};

// 資產質量級別枚舉
UENUM(BlueprintType)
enum class EAssetQuality : uint8
{
    Draft      UMETA(DisplayName = "Draft"),
    Standard   UMETA(DisplayName = "Standard"),
    High       UMETA(DisplayName = "High"),
    Ultra      UMETA(DisplayName = "Ultra"),
    Cinematic  UMETA(DisplayName = "Cinematic")
};

// 資產類型枚舉
UENUM(BlueprintType)
enum class EAssetType : uint8
{
    Scene        UMETA(DisplayName = "Scene"),
    Character    UMETA(DisplayName = "Character"),
    Building     UMETA(DisplayName = "Building"),
    Environment  UMETA(DisplayName = "Environment"),
    UI           UMETA(DisplayName = "UI"),
    Prop         UMETA(DisplayName = "Prop"),
    Effect       UMETA(DisplayName = "Effect"),
    Custom       UMETA(DisplayName = "Custom")
};

// 資產用途枚舉
UENUM(BlueprintType)
enum class EAssetPurpose : uint8
{
    Gameplay     UMETA(DisplayName = "Gameplay"),
    Story        UMETA(DisplayName = "Story"),
    Tutorial     UMETA(DisplayName = "Tutorial"),
    Marketing    UMETA(DisplayName = "Marketing"),
    Education    UMETA(DisplayName = "Education"),
    Demo         UMETA(DisplayName = "Demo"),
    Documentation UMETA(DisplayName = "Documentation"),
    Custom       UMETA(DisplayName = "Custom")
};

// 資產需求結構
USTRUCT(BlueprintType)
struct FAssetRequirement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Requirement")
    FString AssetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Requirement")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Requirement")
    EAssetType AssetType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Requirement")
    EAssetQuality Quality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Requirement")
    EAssetPurpose Purpose;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Requirement")
    TMap<FString, FString> AdditionalParameters;

    FAssetRequirement()
    {
        AssetName = TEXT("");
        Description = TEXT("");
        AssetType = EAssetType::Scene;
        Quality = EAssetQuality::Standard;
        Purpose = EAssetPurpose::Gameplay;
    }
};
