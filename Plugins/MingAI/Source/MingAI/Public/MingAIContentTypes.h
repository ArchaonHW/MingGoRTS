#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIContentTypes.generated.h"

UENUM(BlueprintType)
enum class EMingAIContentType: uint8 {
    None UMETA(DisplayName = "None"),
    Image UMETA(DisplayName = "Image"),
    Video UMETA(DisplayName = "Video"),
    Music UMETA(DisplayName = "Music"),
    SoundEffect UMETA(DisplayName = "Sound Effect"),
    Texture UMETA(DisplayName = "Texture"),
    Mesh UMETA(DisplayName = "3D Mesh"),
    Material UMETA(DisplayName = "Material"),
    Animation UMETA(DisplayName = "Animation"),
    Level UMETA(DisplayName = "Level"),
    UI UMETA(DisplayName = "UI"),
    Text UMETA(DisplayName = "Text"),
    Voice UMETA(DisplayName = "Voice"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingAIProvider: uint8 {
    None UMETA(DisplayName = "None"),
    StableDiffusion UMETA(DisplayName = "Stable Diffusion"),
    DALL_E UMETA(DisplayName = "DALL-E"),
    Midjourney UMETA(DisplayName = "Midjourney"),
    AIVA UMETA(DisplayName = "AIVA"),
    ElevenLabs UMETA(DisplayName = "ElevenLabs"),
    OpenAI UMETA(DisplayName = "OpenAI"),
    Custom UMETA(DisplayName = "Custom"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingAIQualityLevel: uint8 {
    Draft UMETA(DisplayName = "Draft"),
    Standard UMETA(DisplayName = "Standard"),
    High UMETA(DisplayName = "High"),
    Ultra UMETA(DisplayName = "Ultra"),
    Cinematic UMETA(DisplayName = "Cinematic"),
    Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMingAIGenerationStatus: uint8 {
    Idle UMETA(DisplayName = "Idle"),
    Queued UMETA(DisplayName = "Queued"),
    Generating UMETA(DisplayName = "Generating"),
    Processing UMETA(DisplayName = "Processing"),
    Completed UMETA(DisplayName = "Completed"),
    Failed UMETA(DisplayName = "Failed"),
    Cancelled UMETA(DisplayName = "Cancelled"),
    Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMingAIContentRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    EMingAIContentType ContentType;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    FString Prompt;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    FString NegativePrompt;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    EMingAIProvider Provider;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    EMingAIQualityLevel QualityLevel;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    int32 Width;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    int32 Height;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    int32 Seed;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    int32 Steps;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    float GuidanceScale;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    FString Style;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content")
    TMap<FString, FString> AdditionalParameters;

    FMingAIContentRequest()
        : ContentType(EMingAIContentType::None)
        , Provider(EMingAIProvider::StableDiffusion)
        , QualityLevel(EMingAIQualityLevel::Standard)
        , Width(1024)
        , Height(1024)
        , Seed(-1)
        , Steps(30)
        , GuidanceScale(7.5f)
    {}
};

USTRUCT(BlueprintType)
struct FMingAIContentResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Content")
    FGuid RequestID;

    UPROPERTY(BlueprintReadOnly, Category = "AI Content")
    FString FilePath;

    UPROPERTY(BlueprintReadOnly, Category = "AI Content")
    EMingAIContentType ContentType;

    UPROPERTY(BlueprintReadOnly, Category = "AI Content")
    EMingAIGenerationStatus Status;

    UPROPERTY(BlueprintReadOnly, Category = "AI Content")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "AI Content")
    float GenerationTime;

    UPROPERTY(BlueprintReadOnly, Category = "AI Content")
    int32 FileSize;

    UPROPERTY(BlueprintReadOnly, Category = "AI Content")
    FDateTime GenerationTimeStamp;

    FMingAIContentResult()
        : ContentType(EMingAIContentType::None)
        , Status(EMingAIGenerationStatus::Idle)
        , GenerationTime(0.0f)
        , FileSize(0)
    {}
};

USTRUCT(BlueprintType)
struct FMingAIProviderConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AI Provider")
    EMingAIProvider Provider;

    UPROPERTY(BlueprintReadWrite, Category = "AI Provider")
    FString ApiEndpoint;

    UPROPERTY(BlueprintReadWrite, Category = "AI Provider")
    FString ApiKey;

    UPROPERTY(BlueprintReadWrite, Category = "AI Provider")
    int32 MaxConcurrentRequests;

    UPROPERTY(BlueprintReadWrite, Category = "AI Provider")
    float TimeoutSeconds;

    UPROPERTY(BlueprintReadWrite, Category = "AI Provider")
    bool bEnabled;

    UPROPERTY(BlueprintReadWrite, Category = "AI Provider")
    TMap<FString, FString> CustomHeaders;

    FMingAIProviderConfig()
        : Provider(EMingAIProvider::None)
        , MaxConcurrentRequests(5)
        , TimeoutSeconds(120.0f)
        , bEnabled(false)
    {}
};

