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
        TargetPath = TEXT("/Game/GeneratedAssets/"};
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
        Duration = TEXT("10 minutes"};
        Language = TEXT("Chinese"};
        CulturalContext = TEXT("Ming Dynasty"};
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameAssetGenerated, const FString&, AssetID, const FGeneratedGameAsset&, Asset};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameAssetGenerationProgress, const FString&, AssetID, int32, Progress};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameAssetGenerationCompleted, const FString&, RequestID, const TArray<FGeneratedGameAsset>&, GeneratedAssets};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUniversityGuideGenerated, const FString&, GuideID, const FGeneratedGameAsset&, GuideAsset);

/**
 * MingGoRTS ??戲資產???X * 負責???X?戲???????X??X??X?括??面??影??、音樂、音??、??檔??
 * 並支???X???導內容?X */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSGameAssetGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSGameAssetGenerator();

    // ???X??X??X??X?器
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void InitializeGameAssetGenerator(UWorld* World);

    // ???X?個?X??X    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameAsset(const FGameAssetRequirement& Requirement);

    // ???X??X?戲資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameAssetsBatch(const TArray<FGameAssetRequirement>& Requirements);

    // ???X?戲??景
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameScene(const FString& SceneName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // ????角色資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateCharacterAsset(const FString& CharacterName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // ????建??資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateBuildingAsset(const FString& BuildingName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // ???X???資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateEnvironmentAsset(const FString& EnvironmentName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // ????UI資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUIAsset(const FString& UIName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // ????影??資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateVideoAsset(const FString& VideoName, const FString& Description, float Duration = 60.0f);

    // ???X???資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateMusicAsset(const FString& MusicName, const FString& Description, float Duration = 180.0f);

    // ???X???資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateSoundEffectAsset(const FString& SoundName, const FString& Description);

    // ????紋??資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTextureAsset(const FString& TextureName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // ????3D模??資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateModelAsset(const FString& ModelName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // ???X?畫資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateAnimationAsset(const FString& AnimationName, const FString& Description, float Duration = 5.0f);

    // ???X?學資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTutorialAsset(const FString& TutorialName, const FString& Description, bool bInteractive = true);

    // ????高校引??資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUniversityGuide(const FUniversityGuideConfig& Config);

    // ???X??X??X    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FGeneratedGameAsset GetGeneratedAsset(const FString& AssetID) const;

    // ???X????X???資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TArray<FGeneratedGameAsset> GetAllGeneratedAssets() const;

    // ???X?獲???X    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TArray<FGeneratedGameAsset> GetAssetsByCategory(EGameAssetCategory Category) const;

    // ??用??獲???X    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TArray<FGeneratedGameAsset> GetAssetsByPurpose(EGameAssetPurpose Purpose) const;

    // 導出資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool ExportAsset(const FString& AssetID, const FString& ExportPath);

    // ????導出資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool ExportAssetsBatch(const TArray<FString>& AssetIDs, const FString& ExportPath);

    // ??除資產
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool DeleteAsset(const FString& AssetID);

    // 清?X????X    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void ClearAllAssets();

    // ????資產統??
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TMap<EGameAssetCategory, int32> GetAssetStatistics() const;

    // ???X???統??
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TMap<FString, float> GetGenerationStatistics() const;

public:
    // 事件委??
    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnGameAssetGenerated OnGameAssetGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnGameAssetGenerationProgress OnGameAssetGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnGameAssetGenerationCompleted OnGameAssetGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnUniversityGuideGenerated OnUniversityGuideGenerated;

protected:
    // ????資產????
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FGeneratedGameAsset ProcessAssetGeneration(const FGameAssetRequirement& Requirement);

    // ???X?戲??景??容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameSceneContent(const FGameAssetRequirement& Requirement);

    // ????角色??容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateCharacterContent(const FGameAssetRequirement& Requirement);

    // ????建?X?容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateBuildingContent(const FGameAssetRequirement& Requirement);

    // ???X??X?容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateEnvironmentContent(const FGameAssetRequirement& Requirement);

    // ????UI??容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUIContent(const FGameAssetRequirement& Requirement);

    // ????影?X?容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateVideoContent(const FGameAssetRequirement& Requirement);

    // ???X??X?容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateMusicContent(const FGameAssetRequirement& Requirement);

    // ???X??X?容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateSoundEffectContent(const FGameAssetRequirement& Requirement);

    // ????紋?X?容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTextureContent(const FGameAssetRequirement& Requirement);

    // ????3D模?X?容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateModelContent(const FGameAssetRequirement& Requirement);

    // ???X?畫??容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateAnimationContent(const FGameAssetRequirement& Requirement);

    // ???X?學??容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTutorialContent(const FGameAssetRequirement& Requirement);

    // ????高校引?X?容
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUniversityGuideContent(const FUniversityGuideConfig& Config);

    // 驗??資產????    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool ValidateAssetRequirement(const FGameAssetRequirement& Requirement) const;

    // ????資產完??
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void HandleAssetCompleted(const FString& AssetID, const FGeneratedGameAsset& Asset);

    // ????資產??誤
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void HandleAssetError(const FString& AssetID, const FString& ErrorMessage);

    // ??新???X?度
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void UpdateGenerationProgress(const FString& AssetID, int32 Progress);

    // ???X???資產ID
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUniqueAssetID() const;

    // ????資產類別??稱
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GetAssetCategoryName(EGameAssetCategory Category) const;

    // ????資產??途????    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GetAssetPurposeName(EGameAssetPurpose Purpose) const;

    // ????高校??容類?X?稱
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GetUniversityContentTypeName(EUniversityContentType ContentType) const;

    // ??建資產??數X    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TMap<FString, FString> CreateAssetMetadata(const FGameAssetRequirement& Requirement) const;

    // 計?X?估???X???
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    float CalculateEstimatedGenerationTime(const FGameAssetRequirement& Requirement) const;

protected:
    // ????世??引用
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // AI??容系統引用
    UPROPERTY()
    TObjectPtr<UMingAIGeneratedContentSystem> AIContentSystem;

    UPROPERTY()
    TObjectPtr<UMingAIContentRenderer> ContentRenderer;

    UPROPERTY()
    TObjectPtr<UMingAIContentManager> ContentManager;

    // ???X??X??X    UPROPERTY()
    TMap<FString, FGeneratedGameAsset> GeneratedAssets;

    // ???X???任??
    UPROPERTY()
    TMap<FString, FGameAssetRequirement> ActiveGenerations;

    // 系統??置
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

    // 統?X???
    UPROPERTY()
    TMap<EGameAssetCategory, int32> AssetCounts;

    UPROPERTY()
    TMap<EGameAssetCategory, float> TotalGenerationTimes;

    UPROPERTY()
    int32 TotalGeneratedAssets;

    UPROPERTY()
    float TotalGenerationTime;

private:
    // ???X??X???
    void ProcessBatchGeneration(const FString& RequestID, const TArray<FGameAssetRequirement>& Requirements);

    // 驗??資產ID
    bool IsValidAssetID(const FString& AssetID) const;

    // 記??資產????
    void LogAssetMessage(const FString& AssetID, const FString& Message, bool bIsError = false};

    // ??新統?X???
    void UpdateStatistics(EGameAssetCategory Category, float GenerationTime};

    // 清?X???資產
    void CleanupExpiredAssets();

    // ??份資產
    void BackupAsset(const FGeneratedGameAsset& Asset);

    // 壓縮資產????
    TArray<uint8> CompressAssetData(const TArray<uint8>& AssetData) const;

    // ?X縮?X?數X    TArray<uint8> DecompressAssetData(const TArray<uint8>& CompressedData) const;

    // ????系統??X    TMap<FString, FString> GetSystemStatus() const;
};

