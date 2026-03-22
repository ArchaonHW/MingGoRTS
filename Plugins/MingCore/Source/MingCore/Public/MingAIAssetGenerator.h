#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIAssetGenerator.generated.h"

/**
 * AIè³‡ç”¢é¡å??šè?
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
 * æ°‘å??‚æ?é¢¨æ ¼?šè?
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
 * AIè³‡ç”¢?Ÿæ??ƒæ•¸çµæ?
 */
USTRUCT(BlueprintType)
struct FAIAssetGenerationParams
{
    GENERATED_BODY()

    // è³‡ç”¢é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    EAIAssetType AssetType;

    // æ°‘å??‚æ?é¢¨æ ¼
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    ERepublicEraStyle EraStyle;

    // ?ºç??è¿°
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString BaseDescription;

    // è©³ç´°?ç¤ºè©?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString DetailedPrompt;

    // è² é¢?ç¤ºè©?(?¨æ–¼?–å??Ÿæ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString NegativePrompt;

    // ?–å?å°ºå¯¸ (?¨æ–¼?–å??Ÿæ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    int32 ImageWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    int32 ImageHeight;

    // ?³æ??‚é•· (?¨æ–¼?³æ??Ÿæ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    float MusicDuration;

    // ?³æ?é¢¨æ ¼ (?¨æ–¼?³æ??Ÿæ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString MusicStyle;

    // è³ªé?è¨­ç½®
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    float Quality;

    // ç¨®å???(?¨æ–¼?¯é?è¤‡ç???
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
 * AIè³‡ç”¢?Ÿæ?çµæ?çµæ?
 */
USTRUCT(BlueprintType)
struct FAIAssetGenerationResult
{
    GENERATED_BODY()

    // ?¯å¦?å?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // è³‡ç”¢é¡å?
    UPROPERTY(BlueprintReadOnly)
    EAIAssetType AssetType;

    // ?Ÿæ??„è??¢è·¯å¾?    UPROPERTY(BlueprintReadOnly)
    FString AssetPath;

    // ç¸®ç•¥?–è·¯å¾?    UPROPERTY(BlueprintReadOnly)
    FString ThumbnailPath;

    // ?Ÿæ??‚é?
    UPROPERTY(BlueprintReadOnly)
    float GenerationTime;

    // ?¯èª¤ä¿¡æ¯
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    // è³‡ç”¢ID
    UPROPERTY(BlueprintReadOnly)
    FString AssetID;

    // è³‡ç”¢æ¨™ç±¤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Tags;

    // è³‡ç”¢?ƒæ•¸??    UPROPERTY(BlueprintReadOnly)
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
 * AIè³‡ç”¢?Ÿæ??? * ?†æ?Stable Diffusion?ŒAIVA?²è?è³‡ç”¢?Ÿæ?
 */
UCLASS(BlueprintType)
class MINGCORE_API UMingAIAssetGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingAIAssetGenerator();

    /**
     * ?å??–AIè³‡ç”¢?Ÿæ???     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void Initialize();

    /**
     * ?œé?AIè³‡ç”¢?Ÿæ???     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void Shutdown();

    /**
     * ?Ÿæ?AIè³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateAsset(const FAIAssetGenerationParams& Params);

    /**
     * ?°æ­¥?Ÿæ?AIè³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void GenerateAssetAsync(const FAIAssetGenerationParams& Params);

    /**
     * ?Ÿæ?è§’è‰²?–å?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateCharacterPortrait(
        const FString& CharacterDescription, 
        ERepublicEraStyle EraStyle,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * ?Ÿæ?å»ºç???     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateBuilding(
        const FString& BuildingDescription,
        ERepublicEraStyle EraStyle,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * ?Ÿæ??Œæ™¯?³æ?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateBackgroundMusic(
        const FString& MusicDescription,
        ERepublicEraStyle EraStyle,
        float Duration = 30.0f,
        const FString& MusicStyle = TEXT("Classical")
    );

    /**
     * ?²å??Ÿæ??„è???     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Generator")
    FAIAssetGenerationResult GetGeneratedAsset(const FString& AssetID);

    /**
     * ?²å??€?‰ç??ç?è³‡ç”¢
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Generator")
    TArray<FAIAssetGenerationResult> GetAllGeneratedAssets();

    /**
     * ?ªé™¤è³‡ç”¢
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    bool DeleteAsset(const FString& AssetID);

    /**
     * è¨­ç½®Stable Diffusion APIç«¯é?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void SetStableDiffusionEndpoint(const FString& Endpoint);

    /**
     * è¨­ç½®AIVA APIç«¯é?
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void SetAIVAEndpoint(const FString& Endpoint);

    /**
     * ?²å?æ°‘å??‚æ??ç¤ºè©?     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Generator")
    FString GetRepublicEraPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType);

    /**
     * ?œæ??²å?å¯¦ä?
     */
    static UMingAIAssetGenerator* Get();

protected:
    // Stable Diffusion APIç«¯é?
    FString StableDiffusionEndpoint;

    // AIVA APIç«¯é?
    FString AIVAEndpoint;

    // å·²ç??ç?è³‡ç”¢
    UPROPERTY()
    TArray<FAIAssetGenerationResult> GeneratedAssets;

    // ?¯å¦å·²å?å§‹å?
    bool bIsInitialized;

    // ?®ä?å¯¦ä?
    static UMingAIAssetGenerator* Instance;

    // ?Ÿæ??¯ä?è³‡ç”¢ID
    FString GenerateAssetID();

    // ?Ÿæ?Stable Diffusion?ç¤ºè©?    FString GenerateStableDiffusionPrompt(const FAIAssetGenerationParams& Params);

    // èª¿ç”¨Stable Diffusion API
    FAIAssetGenerationResult CallStableDiffusionAPI(const FString& Prompt, const FString& NegativePrompt, int32 Width, int32 Height);

    // èª¿ç”¨AIVA API
    FAIAssetGenerationResult CallAIVAAPI(const FString& MusicDescription, float Duration, const FString& Style);

    // ä¿å??Ÿæ??„è???    bool SaveGeneratedAsset(const FAIAssetGenerationResult& Result);

    // ?Ÿæ?ç¸®ç•¥??    FString GenerateThumbnail(const FString& AssetPath, EAIAssetType AssetType);

    // ?µå»ºè³‡ç”¢?®é?
    bool CreateAssetDirectory(EAIAssetType AssetType);

    // é©—è?API?¿æ?
    bool ValidateAPIResponse(const FString& Response);

    // ?•ç??Ÿæ??¯èª¤
    FAIAssetGenerationResult HandleGenerationError(const FString& Error);
};
