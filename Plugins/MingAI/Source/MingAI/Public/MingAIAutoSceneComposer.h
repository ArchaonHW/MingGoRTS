#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIContentTypes.h"
#include "MingAIAutoSceneComposer.generated.h"

UENUM(BlueprintType)
enum class EMingSceneType : uint8
{
    None UMETA(DisplayName = "None"),
    Battlefield UMETA(DisplayName = "Battlefield"),
    Village UMETA(DisplayName = "Village"),
    Palace UMETA(DisplayName = "Palace"),
    Forest UMETA(DisplayName = "Forest"),
    Mountain UMETA(DisplayName = "Mountain"),
    River UMETA(DisplayName = "River"),
    Desert UMETA(DisplayName = "Desert"),
    City UMETA(DisplayName = "City"),
    Temple UMETA(DisplayName = "Temple"),
    Custom UMETA(DisplayName = "Custom"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingSceneComplexity : uint8
{
    Simple UMETA(DisplayName = "Simple"),
    Medium UMETA(DisplayName = "Medium"),
    Complex UMETA(DisplayName = "Complex"),
    Epic UMETA(DisplayName = "Epic"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingSceneMood : uint8
{
    Peaceful UMETA(DisplayName = "Peaceful"),
    Tense UMETA(DisplayName = "Tense"),
    Epic UMETA(DisplayName = "Epic"),
    Mysterious UMETA(DisplayName = "Mysterious"),
    Dramatic UMETA(DisplayName = "Dramatic"),
    Joyful UMETA(DisplayName = "Joyful"),
    Somber UMETA(DisplayName = "Somber"),
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMingSceneCompositionRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    EMingSceneType SceneType;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    EMingSceneComplexity Complexity;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    EMingSceneMood Mood;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    FString TimeOfDay;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    FString Weather;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    bool bGenerateCharacters;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    bool bGenerateEnvironment;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    bool bGenerateLighting;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    bool bGenerateMusic;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Composition")
    EMingAIQualityLevel QualityLevel;

    FMingSceneCompositionRequest()
        : SceneType(EMingSceneType::None)
        , Complexity(EMingSceneComplexity::Medium)
        , Mood(EMingSceneMood::Peaceful)
        , TimeOfDay(TEXT("Day"))
        , Weather(TEXT("Clear"))
        , bGenerateCharacters(true)
        , bGenerateEnvironment(true)
        , bGenerateLighting(true)
        , bGenerateMusic(true)
        , QualityLevel(EMingAIQualityLevel::High)
    {}
};

USTRUCT(BlueprintType)
struct FMingSceneCompositionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Scene Composition")
    FGuid CompositionID;

    UPROPERTY(BlueprintReadOnly, Category = "Scene Composition")
    FString SceneName;

    UPROPERTY(BlueprintReadOnly, Category = "Scene Composition")
    TArray<FGuid> GeneratedImageIDs;

    UPROPERTY(BlueprintReadOnly, Category = "Scene Composition")
    TArray<FGuid> GeneratedAudioIDs;

    UPROPERTY(BlueprintReadOnly, Category = "Scene Composition")
    FString LevelPath;

    UPROPERTY(BlueprintReadOnly, Category = "Scene Composition")
    float TotalGenerationTime;

    UPROPERTY(BlueprintReadOnly, Category = "Scene Composition")
    bool bSuccess;

    FMingSceneCompositionResult()
        : TotalGenerationTime(0.0f)
        , bSuccess(false)
    {}
};

/**
 * AI Auto Scene Composer
 * Automatically composes complete game scenes using AI-generated content
 */
UCLASS(ClassGroup = (AI, Content), Blueprintable)
class MINGAI_API UMingAIAutoSceneComposer : public UObject
{
    GENERATED_BODY()

public:
    UMingAIAutoSceneComposer();

    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    void InitializeComposer();

    // Scene Templates
    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    FMingSceneCompositionRequest GetBattlefieldTemplate(const FString& Description};

    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    FMingSceneCompositionRequest GetVillageTemplate(const FString& Description};

    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    FMingSceneCompositionRequest GetPalaceTemplate(const FString& Description};

    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    FMingSceneCompositionRequest GetForestTemplate(const FString& Description};

    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    FMingSceneCompositionRequest GetCustomTemplate(EMingSceneType SceneType, const FString& Description};

    // Scene Composition
    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    FGuid ComposeScene(const FMingSceneCompositionRequest& Request};

    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    TArray<FGuid> ComposeSceneBatch(const TArray<FMingSceneCompositionRequest>& Requests};

    // Scene Variations
    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    TArray<FGuid> GenerateSceneVariations(const FMingSceneCompositionRequest& BaseRequest, int32 VariationCount};

    // Smart Recommendations
    UFUNCTION(BlueprintPure, Category = "AI Scene Composition")
    TArray<FMingSceneCompositionRequest> GetRecommendedScenes(const FString& GameContext) const;

    // Scene Components
    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    FGuid GenerateSceneBackground(const FMingSceneCompositionRequest& Request};

    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    TArray<FGuid> GenerateSceneCharacters(const FMingSceneCompositionRequest& Request};

    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    FGuid GenerateSceneLighting(const FMingSceneCompositionRequest& Request};

    UFUNCTION(BlueprintCallable, Category = "AI Scene Composition")
    FGuid GenerateSceneAudio(const FMingSceneCompositionRequest& Request};

    // Results
    UFUNCTION(BlueprintPure, Category = "AI Scene Composition")
    FMingSceneCompositionResult GetCompositionResult(const FGuid& CompositionID) const;

protected:
    UPROPERTY()
    TMap<FGuid, FMingSceneCompositionResult> CompositionResults;

    void ProcessSceneComposition(FGuid CompositionID, const FMingSceneCompositionRequest& Request};
    FString BuildScenePrompt(const FMingSceneCompositionRequest& Request) const;
    FString BuildCharacterPrompt(const FMingSceneCompositionRequest& Request, int32 CharacterIndex) const;
    FString BuildAudioPrompt(const FMingSceneCompositionRequest& Request) const;
    TArray<FMingAIContentRequest> DecomposeSceneRequest(const FMingSceneCompositionRequest& Request) const;
    void OnComponentGenerated(FGuid CompositionID, FGuid ComponentID, bool bSuccess};
    
    FString GetSceneTypeKeywords(EMingSceneType SceneType) const;
    FString GetComplexityKeywords(EMingSceneComplexity Complexity) const;
    FString GetMoodKeywords(EMingSceneMood Mood) const;
    int32 GetExpectedAssetCount(EMingSceneComplexity Complexity) const;
};

