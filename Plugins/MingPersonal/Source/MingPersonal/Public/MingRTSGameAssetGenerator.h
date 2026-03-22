#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Misc/DateTime.h"
#include "MingAIGeneratedContentSystem.h"
#include "MingAIContentRenderer.h"
#include "MingAIContentManager.h"
#include "MingRTSGameAssetGenerator.generated.h"

UENUM(BlueprintType)
enum class EGameAssetCategory : uint8
{
    GameScene          UMETA(DisplayName = "Game Scene"),
    Character          UMETA(DisplayName = "Character"),
    Building           UMETA(DisplayName = "Building"),
    Environment        UMETA(DisplayName = "Environment"),
    UI                 UMETA(DisplayName = "UI"),
    Video              UMETA(DisplayName = "Video"),
    Music              UMETA(DisplayName = "Music"),
    SoundEffect        UMETA(DisplayName = "Sound Effect"),
    Texture            UMETA(DisplayName = "Texture"),
    Model              UMETA(DisplayName = "3D Model"),
    Animation          UMETA(DisplayName = "Animation"),
    Tutorial           UMETA(DisplayName = "Tutorial"),
    UniversityGuide    UMETA(DisplayName = "University Guide"),
    Custom             UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EQualityLevel : uint8
{
    Low      UMETA(DisplayName = "Low"),
    Medium   UMETA(DisplayName = "Medium"),
    High     UMETA(DisplayName = "High"),
    Ultra    UMETA(DisplayName = "Ultra"),
    Custom   UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EGameAssetPurpose : uint8
{
    Gameplay           UMETA(DisplayName = "Gameplay"),
    Story              UMETA(DisplayName = "Story"),
    Tutorial           UMETA(DisplayName = "Tutorial"),
    Marketing          UMETA(DisplayName = "Marketing"),
    Education          UMETA(DisplayName = "Education"),
    University         UMETA(DisplayName = "University"),
    Presentation       UMETA(DisplayName = "Presentation"),
    Documentation      UMETA(DisplayName = "Documentation"),
    Custom             UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EUniversityContentType : uint8
{
    CampusTour         UMETA(DisplayName = "Campus Tour"),
    AcademicIntro      UMETA(DisplayName = "Academic Introduction"),
    StudentLife        UMETA(DisplayName = "Student Life"),
    ResearchShowcase   UMETA(DisplayName = "Research Showcase"),
    CareerGuidance     UMETA(DisplayName = "Career Guidance"),
    HistoricalContent  UMETA(DisplayName = "Historical Content"),
    CulturalContent    UMETA(DisplayName = "Cultural Content"),
    TechnicalDemo      UMETA(DisplayName = "Technical Demo"),
    InteractiveGuide   UMETA(DisplayName = "Interactive Guide"),
    Custom             UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FGameAssetRequirement
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    EGameAssetCategory AssetCategory;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    EGameAssetPurpose AssetPurpose;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    FString AssetName;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    EQualityLevel Quality;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    EArtStyle Style;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    FString Theme;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    FString Context;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    TMap<FString, FString> CustomParameters;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    bool bIsUrgent;

    UPROPERTY(BlueprintReadWrite, Category = "Game Asset Requirement")
    FString TargetPath;

    FGameAssetRequirement()
    {
        AssetCategory = EGameAssetCategory::GameScene;
        AssetPurpose = EGameAssetPurpose::Gameplay;
        AssetName = TEXT("");
        Description = TEXT("");
        Quality = EQualityLevel::Standard;
        Style = EArtStyle::Realistic;
        Theme = TEXT("");
        Context = TEXT("");
        bIsUrgent = false;
        TargetPath = TEXT("/Game/GeneratedAssets/");
    }
};

USTRUCT(BlueprintType)
struct FUniversityGuideConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    EUniversityContentType ContentType;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    FString UniversityName;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    FString Department;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    FString TargetAudience;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    TArray<FString> KeyTopics;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    FString Duration;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    FString Language;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    FString CulturalContext;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    bool bInteractive;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    bool bIncludeHistoricalContent;

    UPROPERTY(BlueprintReadWrite, Category = "University Guide Config")
    bool bIncludeStudentTestimonials;

    FUniversityGuideConfig()
    {
        ContentType = EUniversityContentType::CampusTour;
        UniversityName = TEXT("");
        Department = TEXT("");
        TargetAudience = TEXT("");
        Duration = TEXT("10 minutes");
        Language = TEXT("Chinese");
        CulturalContext = TEXT("Ming Dynasty");
        bInteractive = true;
        bIncludeHistoricalContent = true;
        bIncludeStudentTestimonials = false;
    }
};

USTRUCT(BlueprintType)
struct FGeneratedGameAsset
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    FString AssetID;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    FString AssetName;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    EGameAssetCategory AssetCategory;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    EGameAssetPurpose AssetPurpose;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    FString AssetPath;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    EQualityLevel Quality;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    EArtStyle Style;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    TArray<uint8> AssetData;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    FString GeneratedAt;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    float GenerationTime;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    TArray<FString> Tags;

    UPROPERTY(BlueprintReadOnly, Category = "Generated Game Asset")
    TMap<FString, FString> Metadata;

    FGeneratedGameAsset()
    {
        AssetID = TEXT("");
        AssetName = TEXT("");
        AssetCategory = EGameAssetCategory::Custom;
        AssetPurpose = EGameAssetPurpose::Custom;
        AssetPath = TEXT("");
        Description = TEXT("");
        Quality = EQualityLevel::Standard;
        Style = EArtStyle::Realistic;
        GeneratedAt = TEXT("");
        GenerationTime = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameAssetGenerated, const FString&, AssetID, const FGeneratedGameAsset&, Asset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameAssetGenerationProgress, const FString&, AssetID, int32, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameAssetGenerationCompleted, const FString&, RequestID, const TArray<FGeneratedGameAsset>&, GeneratedAssets);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUniversityGuideGenerated, const FString&, GuideID, const FGeneratedGameAsset&, GuideAsset);

/**
 * MingGoRTS 遊戲資產生成器
 * 負責生成遊戲所需的所有資產，包括畫面、影片、音樂、音效、圖檔等
 * 並支持高校引導內容生成
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSGameAssetGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSGameAssetGenerator();

    // 初始化遊戲資產生成器
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void InitializeGameAssetGenerator(UWorld* World);

    // 生成單個遊戲資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameAsset(const FGameAssetRequirement& Requirement);

    // 批量生成遊戲資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameAssetsBatch(const TArray<FGameAssetRequirement>& Requirements);

    // 生成遊戲場景
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameScene(const FString& SceneName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 生成角色資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateCharacterAsset(const FString& CharacterName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 生成建築資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateBuildingAsset(const FString& BuildingName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 生成環境資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateEnvironmentAsset(const FString& EnvironmentName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 生成UI資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUIAsset(const FString& UIName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 生成影片資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateVideoAsset(const FString& VideoName, const FString& Description, float Duration = 60.0f);

    // 生成音樂資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateMusicAsset(const FString& MusicName, const FString& Description, float Duration = 180.0f);

    // 生成音效資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateSoundEffectAsset(const FString& SoundName, const FString& Description);

    // 生成紋理資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTextureAsset(const FString& TextureName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 生成3D模型資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateModelAsset(const FString& ModelName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 生成動畫資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateAnimationAsset(const FString& AnimationName, const FString& Description, float Duration = 5.0f);

    // 生成教學資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTutorialAsset(const FString& TutorialName, const FString& Description, bool bInteractive = true);

    // 生成高校引導資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUniversityGuide(const FUniversityGuideConfig& Config);

    // 獲取生成的資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FGeneratedGameAsset GetGeneratedAsset(const FString& AssetID) const;

    // 獲取所有生成的資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TArray<FGeneratedGameAsset> GetAllGeneratedAssets() const;

    // 按類別獲取資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TArray<FGeneratedGameAsset> GetAssetsByCategory(EGameAssetCategory Category) const;

    // 按用途獲取資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TArray<FGeneratedGameAsset> GetAssetsByPurpose(EGameAssetPurpose Purpose) const;

    // 導出資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool ExportAsset(const FString& AssetID, const FString& ExportPath);

    // 批量導出資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool ExportAssetsBatch(const TArray<FString>& AssetIDs, const FString& ExportPath);

    // 刪除資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool DeleteAsset(const FString& AssetID);

    // 清理所有資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void ClearAllAssets();

    // 獲取資產統計
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TMap<EGameAssetCategory, int32> GetAssetStatistics() const;

    // 獲取生成統計
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TMap<FString, float> GetGenerationStatistics() const;

public:
    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnGameAssetGenerated OnGameAssetGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnGameAssetGenerationProgress OnGameAssetGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnGameAssetGenerationCompleted OnGameAssetGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnUniversityGuideGenerated OnUniversityGuideGenerated;

protected:
    // 處理資產生成
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FGeneratedGameAsset ProcessAssetGeneration(const FGameAssetRequirement& Requirement);

    // 生成遊戲場景內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameSceneContent(const FGameAssetRequirement& Requirement);

    // 生成角色內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateCharacterContent(const FGameAssetRequirement& Requirement);

    // 生成建築內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateBuildingContent(const FGameAssetRequirement& Requirement);

    // 生成環境內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateEnvironmentContent(const FGameAssetRequirement& Requirement);

    // 生成UI內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUIContent(const FGameAssetRequirement& Requirement);

    // 生成影片內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateVideoContent(const FGameAssetRequirement& Requirement);

    // 生成音樂內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateMusicContent(const FGameAssetRequirement& Requirement);

    // 生成音效內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateSoundEffectContent(const FGameAssetRequirement& Requirement);

    // 生成紋理內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTextureContent(const FGameAssetRequirement& Requirement);

    // 生成3D模型內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateModelContent(const FGameAssetRequirement& Requirement);

    // 生成動畫內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateAnimationContent(const FGameAssetRequirement& Requirement);

    // 生成教學內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTutorialContent(const FGameAssetRequirement& Requirement);

    // 生成高校引導內容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUniversityGuideContent(const FUniversityGuideConfig& Config);

    // 驗證資產需求
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool ValidateAssetRequirement(const FGameAssetRequirement& Requirement) const;

    // 處理資產完成
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void HandleAssetCompleted(const FString& AssetID, const FGeneratedGameAsset& Asset);

    // 處理資產錯誤
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void HandleAssetError(const FString& AssetID, const FString& ErrorMessage);

    // 更新生成進度
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void UpdateGenerationProgress(const FString& AssetID, int32 Progress);

    // 生成唯一資產ID
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUniqueAssetID() const;

    // 獲取資產類別名稱
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GetAssetCategoryName(EGameAssetCategory Category) const;

    // 獲取資產用途名稱
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GetAssetPurposeName(EGameAssetPurpose Purpose) const;

    // 獲取高校內容類型名稱
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GetUniversityContentTypeName(EUniversityContentType ContentType) const;

    // 創建資產元數據
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TMap<FString, FString> CreateAssetMetadata(const FGameAssetRequirement& Requirement) const;

    // 計算預估生成時間
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    float CalculateEstimatedGenerationTime(const FGameAssetRequirement& Requirement) const;

protected:
    // 當前世界引用
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // AI內容系統引用
    UPROPERTY()
    TObjectPtr<UMingAIGeneratedContentSystem> AIContentSystem;

    UPROPERTY()
    TObjectPtr<UMingAIContentRenderer> ContentRenderer;

    UPROPERTY()
    TObjectPtr<UMingAIContentManager> ContentManager;

    // 生成的資產存儲
    UPROPERTY()
    TMap<FString, FGeneratedGameAsset> GeneratedAssets;

    // 當前生成任務
    UPROPERTY()
    TMap<FString, FGameAssetRequirement> ActiveGenerations;

    // 系統配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Asset Generator")
    int32 MaxConcurrentGenerations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Asset Generator")
    float GenerationTimeout;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Asset Generator")
    bool bEnableAutoBackup;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Asset Generator")
    bool bEnableCompression;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Asset Generator")
    FString DefaultAssetPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Asset Generator")
    TArray<FString> SupportedFormats;

    // 統計數據
    UPROPERTY()
    TMap<EGameAssetCategory, int32> AssetCounts;

    UPROPERTY()
    TMap<EGameAssetCategory, float> TotalGenerationTimes;

    UPROPERTY()
    int32 TotalGeneratedAssets;

    UPROPERTY()
    float TotalGenerationTime;

private:
    // 處理批量生成
    void ProcessBatchGeneration(const FString& RequestID, const TArray<FGameAssetRequirement>& Requirements);

    // 驗證資產ID
    bool IsValidAssetID(const FString& AssetID) const;

    // 記錄資產日誌
    void LogAssetMessage(const FString& AssetID, const FString& Message, bool bIsError = false);

    // 更新統計數據
    void UpdateStatistics(EGameAssetCategory Category, float GenerationTime);

    // 清理過期資產
    void CleanupExpiredAssets();

    // 備份資產
    void BackupAsset(const FGeneratedGameAsset& Asset);

    // 壓縮資產數據
    TArray<uint8> CompressAssetData(const TArray<uint8>& AssetData) const;

    // 解壓縮資產數據
    TArray<uint8> DecompressAssetData(const TArray<uint8>& CompressedData) const;

    // 獲取系統狀態
    TMap<FString, FString> GetSystemStatus() const;
};
