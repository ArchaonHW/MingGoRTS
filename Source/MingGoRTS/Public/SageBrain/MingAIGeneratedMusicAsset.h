#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "MingAIGeneratedMusicAsset.generated.h"

// 音樂生成狀態
UENUM(BlueprintType)
enum class EMusicAssetGenerationStatus : uint8
{
    Idle           UMETA(DisplayName = "閒置"),
    Generating     UMETA(DisplayName = "生成中"),
    Downloading    UMETA(DisplayName = "下載中"),
    Processing     UMETA(DisplayName = "處理中"),
    Completed      UMETA(DisplayName = "完成"),
    Failed         UMETA(DisplayName = "失敗"),
    Cancelled      UMETA(DisplayName = "已取消")
};

// AI 音樂提供商
UENUM(BlueprintType)
enum class EMusicAIProvider : uint8
{
    AIVA           UMETA(DisplayName = "AIVA"),
    Soundraw       UMETA(DisplayName = "Soundraw"),
    Boomy          UMETA(DisplayName = "Boomy"),
    Amper          UMETA(DisplayName = "Amper"),
    OpenAI         UMETA(DisplayName = "OpenAI"),
    LocalModel     UMETA(DisplayName = "本地模型"),
    Mock           UMETA(DisplayName = "模擬")  // 用於測試
};

// 音樂風格
UENUM(BlueprintType)
enum class EMusicAssetStyle : uint8
{
    Orchestral     UMETA(DisplayName = "管弦樂"),
    Traditional    UMETA(DisplayName = "傳統中國"),
    Electronic     UMETA(DisplayName = "電子音樂"),
    Ambient        UMETA(DisplayName = "環境音樂"),
    Rock           UMETA(DisplayName = "搖滾"),
    Cinematic      UMETA(DisplayName = "電影配樂"),
    Battle         UMETA(DisplayName = "戰鬥音樂"),
    Victory        UMETA(DisplayName = "勝利音樂"),
    Suspense       UMETA(DisplayName = "懸疑音樂"),
    Peaceful       UMETA(DisplayName = "平和音樂"),
    Custom         UMETA(DisplayName = "自定義")
};

// 音質等級
UENUM(BlueprintType)
enum class EMusicAssetQuality : uint8
{
    Low      UMETA(DisplayName = "低質量 - 128kbps"),
    Medium   UMETA(DisplayName = "中質量 - 192kbps"),
    High     UMETA(DisplayName = "高質量 - 320kbps"),
    Lossless UMETA(DisplayName = "無損 - FLAC")
};

// 音樂生成參數
USTRUCT(BlueprintType)
struct FMusicAssetGenerationParams
{
    GENERATED_BODY()

    // 基本參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Generation")
    FString Prompt = TEXT("Epic battle music with traditional Chinese instruments");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Generation")
    EMusicAssetStyle Style = EMusicAssetStyle::Orchestral;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Generation")
    float Duration = 30.0f;  // 秒

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Generation")
    EMusicAssetQuality Quality = EMusicAssetQuality::High;

    // 音樂屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Properties")
    int32 Tempo = 120;  // BPM

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Properties")
    FString Key = TEXT("C");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Properties")
    FString Mood = TEXT("Epic");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Properties")
    TArray<FString> Instruments;

    // 輸出設置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
    FString OutputName = TEXT("GeneratedMusic");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
    FString OutputDirectory = TEXT("GeneratedAudio/Music");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
    bool bLoop = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
    float FadeInDuration = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
    float FadeOutDuration = 2.0f;

    // 遊戲特定參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Context")
    bool bUseSageBrainEnhancement = true;  // 使用聖者大腦增強

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Context")
    FString GameContext = TEXT("RTS Battle");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Context")
    FString PhilosophyMode = TEXT("Confucianism");  // 哲學模式

    // 提供商設置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider")
    EMusicAIProvider Provider = EMusicAIProvider::AIVA;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider")
    FString CustomAPIEndpoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Provider")
    FString APIKey;
};

// 生成的音樂資產數據
USTRUCT(BlueprintType)
struct FGeneratedMusicAsset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    FString AssetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    FString FilePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    USoundWave* SoundWave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    int32 SampleRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
    int32 NumChannels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
    FString GenerationPrompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
    FString Style;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
    FDateTime GenerationTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
    FString AIProvider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
    TMap<FString, FString> Tags;
};

// 生成進度回調
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMusicAssetGenerationProgress, 
    const FString&, AssetName, 
    float, ProgressPercent, 
    const FString&, StatusMessage);

// 生成完成回調
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMusicAssetGenerationComplete, 
    bool, bSuccess, 
    const FGeneratedMusicAsset&, GeneratedAsset);

// 批量生成回調
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatchMusicGenerationComplete, 
    bool, bAllSuccess, 
    const TArray<FGeneratedMusicAsset>&, GeneratedAssets);

/**
 * AI 生成音樂資產系統
 * 負責調用外部 AI 音樂生成 API，下載並處理生成的音樂文件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingAIGeneratedMusicAsset : public UObject
{
    GENERATED_BODY()

public:
    UMingAIGeneratedMusicAsset();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset")
    bool Initialize();

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset")
    void Shutdown();

    // 單首音樂生成
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Generation")
    void GenerateMusicAsset(const FMusicAssetGenerationParams& Params);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Generation")
    void GenerateMusicAssetAsync(const FMusicAssetGenerationParams& Params);

    // 批量生成
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Batch")
    void GenerateMusicPack(const TArray<FMusicAssetGenerationParams>& ParamsArray);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Batch")
    void GeneratePresetMusicPack(EMusicAssetStyle Style, int32 Count = 5);

    // 取消生成
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Control")
    void CancelGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Control")
    bool IsGenerating() const { return CurrentStatus == EMusicAssetGenerationStatus::Generating || 
                                       CurrentStatus == EMusicAssetGenerationStatus::Downloading ||
                                       CurrentStatus == EMusicAssetGenerationStatus::Processing; }

    // 預設模板
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Presets")
    FMusicAssetGenerationParams GetBattleMusicPreset();

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Presets")
    FMusicAssetGenerationParams GetAmbientMusicPreset();

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Presets")
    FMusicAssetGenerationParams GetVictoryMusicPreset();

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Presets")
    FMusicAssetGenerationParams GetTraditionalChinesePreset();

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Presets")
    FMusicAssetGenerationParams GetRepublicanEraPreset();

    // 資產管理
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Management")
    TArray<FGeneratedMusicAsset> GetGeneratedAssets() const;

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Management")
    FGeneratedMusicAsset GetAssetByName(const FString& AssetName) const;

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Management")
    bool DeleteAsset(const FString& AssetName);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Management")
    void ClearAllAssets();

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Management")
    bool ExportAsset(const FString& AssetName, const FString& ExportPath);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Management")
    bool ImportAsset(const FString& ImportPath, const FString& AssetName);

    // 播放控制
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Playback")
    void PlayAsset(const FString& AssetName, float FadeInDuration = 2.0f);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Playback")
    void StopPlayback(float FadeOutDuration = 2.0f);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Playback")
    void PausePlayback();

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Playback")
    void ResumePlayback();

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Playback")
    void SetVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Playback")
    bool IsPlaying() const;

    // API 配置
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|API")
    void SetAPIKey(const FString& APIKey);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|API")
    void SetProvider(EMusicAIProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|API")
    void SetCustomEndpoint(const FString& Endpoint);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|API")
    bool TestAPIConnection();

    // 聖者大腦整合
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|SageBrain")
    void EnableSageBrainEnhancement(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|SageBrain")
    FString EnhancePromptWithSageBrain(const FString& BasePrompt, const FString& Philosophy);

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Status")
    EMusicAssetGenerationStatus GetStatus() const { return CurrentStatus; }

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Status")
    FString GetLastError() const { return LastErrorMessage; }

    UFUNCTION(BlueprintCallable, Category = "AI Music Asset|Status")
    float GetGenerationProgress() const { return GenerationProgress; }

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "AI Music Asset|Events")
    FOnMusicAssetGenerationProgress OnGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "AI Music Asset|Events")
    FOnMusicAssetGenerationComplete OnGenerationComplete;

    UPROPERTY(BlueprintAssignable, Category = "AI Music Asset|Events")
    FOnBatchMusicGenerationComplete OnBatchGenerationComplete;

private:
    // 當前狀態
    EMusicAssetGenerationStatus CurrentStatus;
    float GenerationProgress;
    FString LastErrorMessage;

    // API 設置
    FString CurrentAPIKey;
    EMusicAIProvider CurrentProvider;
    FString CustomAPIEndpoint;
    bool bSageBrainEnhancementEnabled;

    // HTTP 請求
    FHttpRequestPtr CurrentRequest;
    FString PendingDownloadPath;

    // 音頻組件
    UPROPERTY()
    UAudioComponent* AudioComponent;

    // 生成的資產庫
    UPROPERTY()
    TArray<FGeneratedMusicAsset> GeneratedAssets;

    // 批量生成隊列
    TArray<FMusicAssetGenerationParams> BatchQueue;
    int32 CurrentBatchIndex;
    TArray<FGeneratedMusicAsset> BatchResults;

    // 內部函數
    void UpdateStatus(EMusicAssetGenerationStatus NewStatus, const FString& Message = TEXT(""));
    void SendGenerationRequest(const FMusicAssetGenerationParams& Params);
    void HandleGenerationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
    void DownloadGeneratedAudio(const FString& DownloadUrl, const FString& SavePath);
    void HandleDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
    void ProcessDownloadedAudio(const FString& FilePath);
    void FinalizeGeneration(bool bSuccess, const FString& ErrorMessage = TEXT(""));

    // API 請求構建
    FString BuildAIVARequest(const FMusicAssetGenerationParams& Params);
    FString BuildSoundrawRequest(const FMusicAssetGenerationParams& Params);
    FString BuildOpenAIRequest(const FMusicAssetGenerationParams& Params);
    FString BuildMockRequest(const FMusicAssetGenerationParams& Params);

    // 提示詞構建
    FString BuildEnhancedPrompt(const FMusicAssetGenerationParams& Params);
    FString GetStyleDescription(EMusicAssetStyle Style);
    TArray<FString> GetInstrumentsForStyle(EMusicAssetStyle Style);

    // 文件處理
    FString GetUniqueFilePath(const FString& Directory, const FString& BaseName, const FString& Extension);
    bool EnsureDirectoryExists(const FString& Directory);
    USoundWave* LoadSoundWaveFromFile(const FString& FilePath);
    bool ConvertAudioFormat(const FString& InputPath, const FString& OutputPath, EMusicAssetQuality Quality);

    // 批量處理
    void ProcessNextBatchItem();
    void CompleteBatchGeneration();

    // 輔助函數
    FString GetProviderAPIName(EMusicAIProvider Provider);
    FString GetQualityString(EMusicAssetQuality Quality);
    void LogGenerationEvent(const FString& Event, const FString& Details);
    
    // 回調函數
    void OnGenerationProgressTick();
    void InitializeAudioComponent();
    void CleanupAudioComponent();
};
