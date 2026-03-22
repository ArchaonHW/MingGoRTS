#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Sound/SoundBase.h"
#include "MingGoRTSAIFilmGenerator.generated.h"

UENUM(BlueprintType)
enum class EFilmGenerationStatus: uint8 {
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




UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSAIFilmGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSAIFilmGenerator();

    // �D??�v?X??X?��
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void GenerateFilmSequence(const TArray<FFilmSequenceData>& FilmData);

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void StartGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void StopGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    EFilmGenerationStatus GetGenerationStatus() const { return CurrentStatus; }

    // ???X???
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void GenerateSingleFrame(const FString& Prompt, const FString& Style, int32 FrameIndex);

    // Stable Diffusion ????
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void SetStableDiffusionAPI(const FString& APIEndpoint, const FString& APIKey);

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    bool TestAPIConnection();

    // �v??��??��??
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void AddFilmSequence(const FFilmSequenceData& SequenceData);

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void ClearFilmSequences();

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    TArray<FFilmSequenceData> GetFilmSequences() const { return FilmSequences; }

    // ??��??��
    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void PreviewFrame(int32 FrameIndex);

    UFUNCTION(BlueprintCallable, Category = "AI Film")
    void PlayGeneratedFilm();

    // �e??�ƥ�
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

    int32 CurrentFrameIndex;

    bool bIsGenerating;

    // �`??�G??UObject��??����??UPROPERTY
    FTickerDelegate GenerationTicker;
    FDelegateHandle GenerationTickerHandle;

private:
    void ProcessNextFrame();
    bool OnGenerationTick(float DeltaTime);
    void SendStableDiffusionRequest(const FString& Prompt, int32 FrameIndex);
    void HandleGenerationResponse(bool bSuccess, const FString& ResponseData, int32 FrameIndex);
    FString BuildEnhancedPrompt(const FString& BasePrompt, const FString& Style);
    void NotifyGenerationCompleted(bool bSuccess, const FString& ErrorMessage = FString()};
};

