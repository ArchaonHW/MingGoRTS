#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIAssetGenerator.generated.h"

/**
 * AI資產類型枚舉
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
 * 民國時期風格枚舉
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
 * AI資產生成參數結構
 */
USTRUCT(BlueprintType)
struct FAIAssetGenerationParams
{
    GENERATED_BODY()

    // 資產類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    EAIAssetType AssetType;

    // 民國時期風格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    ERepublicEraStyle EraStyle;

    // 基礎描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString BaseDescription;

    // 詳細提示詞
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString DetailedPrompt;

    // 負面提示詞 (用於圖像生成)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString NegativePrompt;

    // 圖像尺寸 (用於圖像生成)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    int32 ImageWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    int32 ImageHeight;

    // 音樂時長 (用於音樂生成)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    float MusicDuration;

    // 音樂風格 (用於音樂生成)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    FString MusicStyle;

    // 質量設置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Asset")
    float Quality;

    // 種子值 (用於可重複生成)
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
 * AI資產生成結果結構
 */
USTRUCT(BlueprintType)
struct FAIAssetGenerationResult
{
    GENERATED_BODY()

    // 是否成功
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 資產類型
    UPROPERTY(BlueprintReadOnly)
    EAIAssetType AssetType;

    // 生成的資產路徑
    UPROPERTY(BlueprintReadOnly)
    FString AssetPath;

    // 縮略圖路徑
    UPROPERTY(BlueprintReadOnly)
    FString ThumbnailPath;

    // 生成時間
    UPROPERTY(BlueprintReadOnly)
    float GenerationTime;

    // 錯誤信息
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    // 資產ID
    UPROPERTY(BlueprintReadOnly)
    FString AssetID;

    // 資產標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Tags;

    // 資產元數據
    UPROPERTY(BlueprintReadOnly)
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
 * AI資產生成器
 * 集成Stable Diffusion和AIVA進行資產生成
 */
UCLASS(BlueprintType)
class MINGCORE_API UMingAIAssetGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingAIAssetGenerator();

    /**
     * 初始化AI資產生成器
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void Initialize();

    /**
     * 關閉AI資產生成器
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void Shutdown();

    /**
     * 生成AI資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateAsset(const FAIAssetGenerationParams& Params);

    /**
     * 異步生成AI資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void GenerateAssetAsync(const FAIAssetGenerationParams& Params);

    /**
     * 生成角色肖像
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateCharacterPortrait(
        const FString& CharacterDescription, 
        ERepublicEraStyle EraStyle,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * 生成建築物
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateBuilding(
        const FString& BuildingDescription,
        ERepublicEraStyle EraStyle,
        int32 ImageWidth = 512,
        int32 ImageHeight = 512
    );

    /**
     * 生成背景音樂
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    FAIAssetGenerationResult GenerateBackgroundMusic(
        const FString& MusicDescription,
        ERepublicEraStyle EraStyle,
        float Duration = 30.0f,
        const FString& MusicStyle = TEXT("Classical")
    );

    /**
     * 獲取生成的資產
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Generator")
    FAIAssetGenerationResult GetGeneratedAsset(const FString& AssetID);

    /**
     * 獲取所有生成的資產
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Generator")
    TArray<FAIAssetGenerationResult> GetAllGeneratedAssets();

    /**
     * 刪除資產
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    bool DeleteAsset(const FString& AssetID);

    /**
     * 設置Stable Diffusion API端點
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void SetStableDiffusionEndpoint(const FString& Endpoint);

    /**
     * 設置AIVA API端點
     */
    UFUNCTION(BlueprintCallable, Category = "AI Asset Generator")
    void SetAIVAEndpoint(const FString& Endpoint);

    /**
     * 獲取民國時期提示詞
     */
    UFUNCTION(BlueprintPure, Category = "AI Asset Generator")
    FString GetRepublicEraPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType);

    /**
     * 靜態獲取實例
     */
    static UMingAIAssetGenerator* Get();

protected:
    // Stable Diffusion API端點
    FString StableDiffusionEndpoint;

    // AIVA API端點
    FString AIVAEndpoint;

    // 已生成的資產
    UPROPERTY()
    TArray<FAIAssetGenerationResult> GeneratedAssets;

    // 是否已初始化
    bool bIsInitialized;

    // 單例實例
    static UMingAIAssetGenerator* Instance;

    // 生成唯一資產ID
    FString GenerateAssetID();

    // 生成Stable Diffusion提示詞
    FString GenerateStableDiffusionPrompt(const FAIAssetGenerationParams& Params);

    // 調用Stable Diffusion API
    FAIAssetGenerationResult CallStableDiffusionAPI(const FString& Prompt, const FString& NegativePrompt, int32 Width, int32 Height);

    // 調用AIVA API
    FAIAssetGenerationResult CallAIVAAPI(const FString& MusicDescription, float Duration, const FString& Style);

    // 保存生成的資產
    bool SaveGeneratedAsset(const FAIAssetGenerationResult& Result);

    // 生成縮略圖
    FString GenerateThumbnail(const FString& AssetPath, EAIAssetType AssetType);

    // 創建資產目錄
    bool CreateAssetDirectory(EAIAssetType AssetType);

    // 驗證API響應
    bool ValidateAPIResponse(const FString& Response);

    // 處理生成錯誤
    FAIAssetGenerationResult HandleGenerationError(const FString& Error);
};
