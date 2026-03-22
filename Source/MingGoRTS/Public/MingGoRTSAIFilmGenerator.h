#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Sound/SoundBase.h"
#include "MingGoRTSAIFilmGenerator.generated.h"

UENUM(BlueprintType)
enum class EFilmGenerationStatus : uint8
{
    Idle,
    Generating,
    Completed,
    Failed
};

USTRUCT(BlueprintType)
struct FFilmSequenceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Film")
    FString SceneDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Film")
    FString StylePrompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Film")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Film")
    class UTexture2D* GeneratedFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Film")
    class USoundBase* BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Film")
    TArray<FString> AdditionalPrompts;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFilmFrameGenerated, int32, FrameIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFilmGenerationCompleted, bool, bSuccess, const FString&, ErrorMessage);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSAIFilmGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSAIFilmGenerator();

    // 主要影片生成功能
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void GenerateFilmSequence(const TArray<FFilmSequenceData>& FilmData);

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void StartGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void StopGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    EFilmGenerationStatus GetGenerationStatus() const { return CurrentStatus; }

    // 單幀生成
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void GenerateSingleFrame(const FString& Prompt, const FString& Style, int32 FrameIndex);

    // Stable Diffusion 整合
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void SetStableDiffusionAPI(const FString& APIEndpoint, const FString& APIKey);

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    bool TestAPIConnection();

    // 影片序列管理
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void AddFilmSequence(const FFilmSequenceData& SequenceData);

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void ClearFilmSequences();

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    TArray<FFilmSequenceData> GetFilmSequences() const { return FilmSequences; }

    // 預覽功能
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void PreviewFrame(int32 FrameIndex);

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void PlayGeneratedFilm();

    // 委托事件
    UPROPERTY(BlueprintAssignable, Category = "AI Film")
    FOnFilmFrameGenerated OnFilmFrameGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AI Film")
    FOnFilmGenerationCompleted OnFilmGenerationCompleted;

protected:
    UPROPERTY()
    EFilmGenerationStatus CurrentStatus;

    UPROPERTY()
    TArray<FFilmSequenceData> FilmSequences;

    UPROPERTY()
    FString StableDiffusionEndpoint;

    UPROPERTY()
    FString StableDiffusionAPIKey;

    UPROPERTY()
    int32 CurrentFrameIndex;

    UPROPERTY()
    bool bIsGenerating;

    UPROPERTY()
    FTickerDelegate GenerationTicker;

    UPROPERTY()
    FDelegateHandle GenerationTickerHandle;

private:
    void ProcessNextFrame();
    bool OnGenerationTick(float DeltaTime);
    void SendStableDiffusionRequest(const FString& Prompt, int32 FrameIndex);
    void HandleGenerationResponse(bool bSuccess, const FString& ResponseData, int32 FrameIndex);
    FString BuildEnhancedPrompt(const FString& BasePrompt, const FString& Style);
    void NotifyGenerationCompleted(bool bSuccess, const FString& ErrorMessage = FString());
};
