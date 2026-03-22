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
enum class EGameAssetCategory: uint8 {
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
enum class EQualityLevel: uint8 {
    Low      UMETA(DisplayName = "Low"),
    Medium   UMETA(DisplayName = "Medium"),
    High     UMETA(DisplayName = "High"),
    Ultra    UMETA(DisplayName = "Ultra"),
    Custom   UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EGameAssetPurpose: uint8 {
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
enum class EUniversityContentType: uint8 {
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
);

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




DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUniversityGuideGenerated, const FString&, GuideID, const FGeneratedGameAsset&, GuideAsset);

/**
 * MingGoRTS ??���겣???X * �t�d???X?��摧毀故事選項X?�A??��??�v??�B���֡B��??�B??��??
 * �ä�目標數量�ɤ��e?X */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSGameAssetGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSGameAssetGenerator();

    // 故事選項X??X?��
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void InitializeGameAssetGenerator(UWorld* World);

    // ???X?��?X??X
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameAsset(const FGameAssetRequirement& Requirement);

    // 故事重要性?���겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameAssetsBatch(const TArray<FGameAssetRequirement>& Requirements);

    // ???X?��??��
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameScene(const FString& SceneName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 摧毀����겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateCharacterAsset(const FString& CharacterName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 摧毀��??�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateBuildingAsset(const FString& BuildingName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 目標數量�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateEnvironmentAsset(const FString& EnvironmentName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 摧毀UI�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUIAsset(const FString& UIName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 摧毀�v??�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateVideoAsset(const FString& VideoName, const FString& Description, float Duration = 60.0f);

    // 目標數量�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateMusicAsset(const FString& MusicName, const FString& Description, float Duration = 180.0f);

    // 目標數量�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateSoundEffectAsset(const FString& SoundName, const FString& Description);

    // 摧毀��??�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTextureAsset(const FString& TextureName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // 摧毀3D��??�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateModelAsset(const FString& ModelName, const FString& Description, EQualityLevel Quality = EQualityLevel::High);

    // ???X?�e�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateAnimationAsset(const FString& AnimationName, const FString& Description, float Duration = 5.0f);

    // ???X?�Ǹ겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTutorialAsset(const FString& TutorialName, const FString& Description, bool bInteractive = true);

    // 摧毀���դ�??�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUniversityGuide(const FUniversityGuideConfig& Config);

    // 故事選項X
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FGeneratedGameAsset GetGeneratedAsset(const FString& AssetID) const;

    // 目標數量?X???�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TArray<FGeneratedGameAsset> GetAllGeneratedAssets() const;

    // ???X?��???X
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TArray<FGeneratedGameAsset> GetAssetsByCategory(EGameAssetCategory Category) const;

    // ??��??��???X
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TArray<FGeneratedGameAsset> GetAssetsByPurpose(EGameAssetPurpose Purpose) const;

    // �ɥX�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool ExportAsset(const FString& AssetID, const FString& ExportPath);

    // 摧毀�ɥX�겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool ExportAssetsBatch(const TArray<FString>& AssetIDs, const FString& ExportPath);

    // ??���겣
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool DeleteAsset(const FString& AssetID);

    // �M?X摧毀X
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void ClearAllAssets();

    // 摧毀�겣��??
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TMap<EGameAssetCategory, int32> GetAssetStatistics() const;

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TMap<FString, float> GetGenerationStatistics() const;

public:
    // �ƥ�e??
    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnGameAssetGenerated OnGameAssetGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnGameAssetGenerationProgress OnGameAssetGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnGameAssetGenerationCompleted OnGameAssetGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Game Asset Generator Events")
    FOnUniversityGuideGenerated OnUniversityGuideGenerated;

protected:
    // 摧毀�겣摧毀
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FGeneratedGameAsset ProcessAssetGeneration(const FGameAssetRequirement& Requirement);

    // ???X?��??��??�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateGameSceneContent(const FGameAssetRequirement& Requirement);

    // 摧毀����??�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateCharacterContent(const FGameAssetRequirement& Requirement);

    // 摧毀��?X?�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateBuildingContent(const FGameAssetRequirement& Requirement);

    // 故事重要性?�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateEnvironmentContent(const FGameAssetRequirement& Requirement);

    // 摧毀UI??�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUIContent(const FGameAssetRequirement& Requirement);

    // 摧毀�v?X?�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateVideoContent(const FGameAssetRequirement& Requirement);

    // 故事重要性?�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateMusicContent(const FGameAssetRequirement& Requirement);

    // 故事重要性?�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateSoundEffectContent(const FGameAssetRequirement& Requirement);

    // 摧毀��?X?�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTextureContent(const FGameAssetRequirement& Requirement);

    // 摧毀3D��?X?�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateModelContent(const FGameAssetRequirement& Requirement);

    // ???X?�e??�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateAnimationContent(const FGameAssetRequirement& Requirement);

    // ???X?��??�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateTutorialContent(const FGameAssetRequirement& Requirement);

    // 摧毀���դ�?X?�e
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUniversityGuideContent(const FUniversityGuideConfig& Config);

    // ��??�겣摧毀
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    bool ValidateAssetRequirement(const FGameAssetRequirement& Requirement) const;

    // 摧毀�겣��??
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void HandleAssetCompleted(const FString& AssetID, const FGeneratedGameAsset& Asset);

    // 摧毀�겣??�~
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void HandleAssetError(const FString& AssetID, const FString& ErrorMessage);

    // ??�s???X?��
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    void UpdateGenerationProgress(const FString& AssetID, int32 Progress);

    // 目標數量�겣ID
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GenerateUniqueAssetID() const;

    // 摧毀�겣���O??��
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GetAssetCategoryName(EGameAssetCategory Category) const;

    // 摧毀�겣??�~摧毀
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GetAssetPurposeName(EGameAssetPurpose Purpose) const;

    // 摧毀����??�e��?X?��
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    FString GetUniversityContentTypeName(EUniversityContentType ContentType) const;

    // ??�ظ겣??��X
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    TMap<FString, FString> CreateAssetMetadata(const FGameAssetRequirement& Requirement) const;

    // �p?X?��目標數量
    UFUNCTION(BlueprintCallable, Category = "Game Asset Generator")
    float CalculateEstimatedGenerationTime(const FGameAssetRequirement& Requirement) const;

protected:
    // 摧毀�@??�ޥ�
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // AI??�e�t�Τޥ�
    UPROPERTY()
    TObjectPtr<UMingAIGeneratedContentSystem> AIContentSystem;

    UPROPERTY()
    TObjectPtr<UMingAIContentRenderer> ContentRenderer;

    UPROPERTY()
    TObjectPtr<UMingAIContentManager> ContentManager;

    // 故事選項X
    UPROPERTY()
    TMap<FString, FGeneratedGameAsset> GeneratedAssets;

    // 目標數量��??
    UPROPERTY()
    TMap<FString, FGameAssetRequirement> ActiveGenerations;

    // �t��??�m
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

    // ��?X???
    UPROPERTY()
    TMap<EGameAssetCategory, int32> AssetCounts;

    UPROPERTY()
    TMap<EGameAssetCategory, float> TotalGenerationTimes;

    UPROPERTY()
    int32 TotalGeneratedAssets;

    UPROPERTY()
    float TotalGenerationTime;

private:
    // 故事選項?
    void ProcessBatchGeneration(const FString& RequestID, const TArray<FGameAssetRequirement>& Requirements);

    // ��??�겣ID
    bool IsValidAssetID(const FString& AssetID) const;

    // �O??�겣摧毀
    void LogAssetMessage(const FString& AssetID, const FString& Message, bool bIsError = false);

    // ??�s��?X???
    void UpdateStatistics(EGameAssetCategory Category, float GenerationTime);

    // �M?X???�겣
    void CleanupExpiredAssets();

    // ??���겣
    void BackupAsset(const FGeneratedGameAsset& Asset);

    // ���Y�겣摧毀
    TArray<uint8> CompressAssetData(const TArray<uint8>& AssetData) const;

    // ?X�Y?X?��X
    TArray<uint8> DecompressAssetData(const TArray<uint8>& CompressedData) const;

    // 摧毀�t��??X
    TMap<FString, FString> GetSystemStatus() const;
};

