#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIAssetGenerator.generated.h"

/**
 * AI資產類�X��?
 */
UENUM(BlueprintType)
enum class EAIAssetType : uint8
{
    CharacterPortrait    UMETA(DisplayName = "Character Portrait"),
    Building             UMETA(DisplayName = "Building"),  
    Landscape            UMETA(DisplayName = "Landscape"),
    Weapon               UMETA(DisplayName = "Weapon"),
    Uniform              UMETA(DisplayName = "Uniform"),
    Prop                 UMETA(DisplayName = "Prop"),
    BackgroundMusic      UMETA(DisplayName = "Background Music"),
    SoundEffect          UMETA(DisplayName = "Sound Effect"),
    VoiceLine            UMETA(DisplayName = "Voice Line")
};

/**
 * 民�X��?風格?��?
 */
UENUM(BlueprintType)
enum class ERepublicEraStyle : uint8
{
    EarlyRepublic      UMETA(DisplayName = "Early Republic (1912-1928)"),
    NanjingDecade      UMETA(DisplayName = "Nanjing Decade (1928-1937)"),
    WarOfResistance    UMETA(DisplayName = "War of Resistance (1937-1945)"),
    CivilWar           UMETA(DisplayName = "Civil War (1945-1949)")
};

/**
 * AI資產?��X�數結�?
 */
USTRUCT(BlueprintType)
struct FAIAssetGenerationParams
{
    GENERATED_BODY()

    // 資產類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    EAIAssetType AssetType;

    // 民�X��?風格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    ERepublicEraStyle EraStyle;

    // ?��X�述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString BaseDescription;

    // 詳細?�示�?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString DetailedPrompt;

    // 負面?�示�?(?�於?��X��?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString NegativePrompt;

    // ?��?尺寸 (?�於?��X��?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    int32 ImageWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    int32 ImageHeight;

    // ?��X�長 (?�於?��X��?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    float MusicDuration;

    // ?��?風格 (?�於?��X��?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString MusicStyle;

    // 質�?設置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    float Quality;

    // 種�X(?�於?��?複�X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    int32 Seed;

    FAIAssetGenerationParams()
        : AssetType(EAIAssetType::CharacterPortrait)
        , EraStyle(ERepublicEraStyle::NanjingDecade)
        , BaseDescription(TEXT(""))
        , DetailedPrompt(TEXT(""))
        , NegativePrompt(TEXT(""))
        , ImageWidth(512)
        , ImageHeight(512)
        , MusicDuration(30.0f)
        , MusicStyle(TEXT("Classical"))
        , Quality(0.8f)
        , Seed(-1)
    {}
};

/**
 * AI資產?��?結�?結�?
 */
USTRUCT(BlueprintType)
struct FAIAssetGenerationResult
{
    GENERATED_BODY()

    // ?�否?��?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 資產類�?
    UPROPERTY(BlueprintReadOnly)
    EAIAssetType AssetType;

    // ?��X��X�路�?    UPROPERTY(BlueprintReadOnly)
    FString AssetPath;

    // 縮略?�路�?    UPROPERTY(BlueprintReadOnly)
    FString ThumbnailPath;

    // ?��X��?
    UPROPERTY(BlueprintReadOnly)
    float GenerationTime;

    // ?�誤信息
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    // 資產ID
    UPROPERTY(BlueprintReadOnly)
    FString AssetID;

    // 資產標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Tags;

    // 資產?�數X    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Metadata;

    FAIAssetGenerationResult()
        : bSuccess(false)
        , AssetType(EAIAssetType::CharacterPortrait)
        , GenerationTime(0.0f)
        , ErrorMessage(TEXT(""))
        , AssetID(TEXT(""))
    {}
};

/**
 * AI資產?��X * ?��?Stable Diffusion?�AIVA?��?資產?��?
 */
UCLASS(BlueprintType)
class MINGCORE_API UMingAIAssetGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingAIAssetGenerator(};

    /**
     * ?��X�AI資產?��X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void Initialize(};

    /**
     * ?��?AI資產?��X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void Shutdown(};

    /**
     * ?��?AI資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateAsset(const FAIAssetGenerationParams& Params) {};

    /**
     * ?�步?��?AI資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void GenerateAssetAsync(const FAIAssetGenerationParams& Params) {};

    /**
     * ?��?角色?��?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateCharacterPortrait(
        const FString& CharacterDescription, 
        ERepublicEraStyle EraStyle,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    };

    /**
     * ?��?建�X     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateBuilding(
        const FString& BuildingDescription,
        ERepublicEraStyle EraStyle,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    };

    /**
     * ?��X�景?��?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateBackgroundMusic(
        const FString& MusicDescription,
        ERepublicEraStyle EraStyle,
        float Duration = 30.0f,
        const FString& MusicStyle = TEXT("Classical")
    };

    /**
     * ?��X��X��X     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Generator")
    FAIAssetGenerationResult GetGeneratedAsset(const FString& AssetID) {};

    /**
     * ?��X�?��X��?資產
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Generator")
    TArray<FAIAssetGenerationResult> GetAllGeneratedAssets(};

    /**
     * ?�除資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    bool DeleteAsset(const FString& AssetID) {};

    /**
     * 設置Stable Diffusion API端�?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void SetStableDiffusionEndpoint(const FString& Endpoint) {};

    /**
     * 設置AIVA API端�?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void SetAIVAEndpoint(const FString& Endpoint) {};

    /**
     * ?��?民�X��X�示�?     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Generator")
    FString GetRepublicEraPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType};

    /**
     * ?��X��?實�?
     */
    static UMingAIAssetGenerator* Get(};

protected:
    // Stable Diffusion API端�?
    FString StableDiffusionEndpoint;

    // AIVA API端�?
    FString AIVAEndpoint;

    // 已�X��?資產
    UPROPERTY()
    TArray<FAIAssetGenerationResult> GeneratedAssets;

    // ?�否已�?始�?
    bool bIsInitialized;

    // ?��?實�?
    static UMingAIAssetGenerator* Instance;

    // 生成唯一資產ID
    FString GenerateAssetID() {};

    // ?��?Stable Diffusion?�示�?    FString GenerateStableDiffusionPrompt(const FAIAssetGenerationParams& Params) {};

    // 調用Stable Diffusion API
    FAIAssetGenerationResult CallStableDiffusionAPI(const FString& Prompt, const FString& NegativePrompt, int32 Width, int32 Height) {};

    // 調用AIVA API
    FAIAssetGenerationResult CallAIVAAPI(const FString& MusicDescription, float Duration, const FString& Style) {};

    // 保�X��X��X    bool SaveGeneratedAsset(const FAIAssetGenerationResult& Result) {};

    // 生成資產縮略圖
    FString GenerateThumbnail(const FString& AssetPath, EAIAssetType AssetType) {};

    // 創建資產目錄
    bool CreateAssetDirectory(EAIAssetType AssetType) {};

    // 驗�?API?��?
    bool ValidateAPIResponse(const FString& Response) {};

    // 處理生成錯誤
    FAIAssetGenerationResult HandleGenerationError(const FString& Error) {};
};

