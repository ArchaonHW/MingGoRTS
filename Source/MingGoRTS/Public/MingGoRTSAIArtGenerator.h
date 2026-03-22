#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "Components/SceneCaptureComponent2D.h"
#include "MingGoRTSAIArtGenerator.generated.h"

UENUM(BlueprintType)
enum class EArtGenerationStatus: uint8 {
    Idle,
    Generating,
    Processing,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class EArtStyle: uint8 {
    Realistic,         // �g�ꭷ��
    Anime,            // ??������
    OilPainting,      // �o�e����
    Watercolor,       // ��m����
    Sketch,           // ��??����
    TraditionalChinese, // ��?X?��??��
    Military,         // �x??����
    Historical,       // ���v����
    ConceptArt,      // ����????
    PixelArt         // ???X???
};

UENUM(BlueprintType)
enum class EArtCategory: uint8 {
    Character,         // ����]??
    Environment,       // ??���]??
    Prop,             // ??��]??
    Weapon,           // �Z���]??
    Vehicle,          // ����]??
    Architecture,     // ��??�]??
    UI,               // UI�]??
    Icon,             // ????�]??
    Texture,          // ��??�]??
    Concept           // �����]??
};

USTRUCT(BlueprintType)
struct FArtGenerationParameters
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    EArtStyle Style = EArtStyle::Realistic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    EArtCategory Category = EArtCategory::Concept;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    FString Prompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    FString NegativePrompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    int32 Width = 1024;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    int32 Height = 1024;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    int32 Steps = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    float CFGScale = 7.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    FString Sampler = "DPM++ 2M Karras";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    int32 Seed = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    bool bEnableControlNet = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    class UTexture2D* ControlImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    FString ControlNetModel = "canny";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    TArray<FString> AdditionalPrompts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Art Parameters")
    bool bEnhanceRepublicanEra = false;
};

USTRUCT(BlueprintType)
struct FArtPostProcessSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float Brightness = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float Contrast = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float Saturation = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float Gamma = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    bool bEnableSharpen = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float SharpenStrength = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    bool bEnableBlur = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float BlurRadius = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    bool bEnableNoise = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    float NoiseStrength = 0.1f;
};





UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSAIArtGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSAIArtGenerator();

    // �D??ø?X??X?��
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void GenerateArt(const FArtGenerationParameters& Parameters);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void StartArtGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void StopArtGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    EArtGenerationStatus GetGenerationStatus() const { return CurrentStatus; }

    // Stable Diffusion ????
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void SetStableDiffusionAPI(const FString& APIEndpoint, const FString& APIKey);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    bool TestAPIConnection();

    // ControlNet ??��
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void SetControlNetImage(UTexture2D* ControlImage, const FString& Model);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void ClearControlNetImage();

    // ???X???
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void GenerateArtBatch(const TArray<FArtGenerationParameters>& BatchParameters);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void GenerateVariations(UTexture2D* SourceImage, int32 VariationCount = 4);

    // ????��?X
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* PostProcessImage(UTexture2D* SourceImage, const FArtPostProcessSettings& Settings);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* ResizeImage(UTexture2D* SourceImage, int32 NewWidth, int32 NewHeight);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* CropImage(UTexture2D* SourceImage, int32 X, int32 Y, int32 Width, int32 Height);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* RotateImage(UTexture2D* SourceImage, float Angle);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* FlipImage(UTexture2D* SourceImage, bool bHorizontal, bool bVertical);

    // ????�겣��??
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void AddToArtLibrary(UTexture2D* Art, const FString& ArtName);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UTexture2D* GetArtFromLibrary(const FString& ArtName);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    TArray<FString> GetArtLibraryNames() const;

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void ClearArtLibrary();

    // ??��????
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    UMaterialInterface* GenerateMaterial(UTexture2D* BaseTexture, const FString& MaterialName);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void GenerateTextureSet(const FArtGenerationParameters& Parameters, TArray<UTexture2D*>& OutTextures);

    // ??�]����
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    FArtGenerationParameters GetRepublicanEraCharacterStyle();

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    FArtGenerationParameters GetRepublicanEraEnvironmentStyle();

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    FArtGenerationParameters GetMilitaryWeaponStyle();

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    FArtGenerationParameters GetTraditionalChineseStyle();

    // ????�ɥX
    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void ExportArt(UTexture2D* Art, const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "AI Art")
    void ExportArtBatch(const TArray<UTexture2D*>& Arts, const FString& DirectoryPath);

    // �e??�ƥ�
    UPROPERTY(BlueprintAssignable, Category = "AI Art")
    FOnArtGenerated OnArtGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AI Art")
    FOnArtGenerationCompleted OnArtGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Art")
    FOnArtGenerationProgress OnArtGenerationProgress;

protected:
    UPROPERTY()
    EArtGenerationStatus CurrentStatus;

    UPROPERTY()
    FString StableDiffusionEndpoint;

    UPROPERTY()
    FString StableDiffusionAPIKey;

    UPROPERTY()
    UTexture2D* CurrentGeneratedArt;

    UPROPERTY()
    UTexture2D* ControlNetImage;

    UPROPERTY()
    FString ControlNetModel;

    UPROPERTY()
    TMap<FString, UTexture2D*> ArtLibrary;

    UPROPERTY()
    TArray<UTexture2D*> GeneratedArts;

    UPROPERTY()
    bool bIsGenerating;

    UPROPERTY()
    float GenerationProgress;

    // Ticker delegates - not UPROPERTY as FTickerDelegate is not supported by UHT
    FTickerDelegate GenerationTicker;
    FDelegateHandle GenerationTickerHandle;

private:
    void ProcessArtGeneration();
    bool OnGenerationTick(float DeltaTime);
    void SendStableDiffusionRequest(const FArtGenerationParameters& Parameters);
    void HandleGenerationResponse(bool bSuccess, const FString& ResponseData);
    FString BuildEnhancedPrompt(const FArtGenerationParameters& Parameters);
    FString GetStylePrompt(EArtStyle Style);
    FString GetCategoryPrompt(EArtCategory Category);
    UTexture2D* CreateTextureFromImageData(const TArray<uint8>& ImageData);
    void NotifyGenerationCompleted(bool bSuccess, const FString& ErrorMessage = FString()};
    void NotifyGenerationProgress(float Progress);
    void ApplyPostProcessing(UTexture2D* Texture, const FArtPostProcessSettings& Settings);
    void InitializeDefaultStyles();
};

