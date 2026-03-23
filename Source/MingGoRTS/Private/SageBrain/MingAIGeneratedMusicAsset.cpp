#include "MingAIGeneratedMusicAsset.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Sound/SoundWave.h"
#include "UObject/UObjectGlobals.h"
#include "Async/Async.h"
#include "TimerManager.h"

UMingAIGeneratedMusicAsset::UMingAIGeneratedMusicAsset()
    : CurrentStatus(EMusicAssetGenerationStatus::Idle)
    , GenerationProgress(0.0f)
    , CurrentProvider(EMusicAIProvider::Mock)
    , bSageBrainEnhancementEnabled(true)
    , CurrentBatchIndex(0)
    , AudioComponent(nullptr)
{
}

bool UMingAIGeneratedMusicAsset::Initialize()
{
    if (!AudioComponent)
    {
        InitializeAudioComponent();
    }
    
    UpdateStatus(EMusicAssetGenerationStatus::Idle, TEXT("系統初始化完成"));
    LogGenerationEvent(TEXT("Initialize"), TEXT("AI音樂生成系統已初始化"));
    return true;
}

void UMingAIGeneratedMusicAsset::Shutdown()
{
    CancelGeneration();
    CleanupAudioComponent();
    UpdateStatus(EMusicAssetGenerationStatus::Idle, TEXT("系統已關閉"));
}

void UMingAIGeneratedMusicAsset::GenerateMusicAsset(const FMusicAssetGenerationParams& Params)
{
    if (IsGenerating())
    {
        LastErrorMessage = TEXT("已有生成任務正在進行");
        UE_LOG(LogTemp, Warning, TEXT("%s"), *LastErrorMessage);
        return;
    }

    GenerateMusicAssetAsync(Params);
}

void UMingAIGeneratedMusicAsset::GenerateMusicAssetAsync(const FMusicAssetGenerationParams& Params)
{
    if (IsGenerating())
    {
        LastErrorMessage = TEXT("已有生成任務正在進行");
        OnGenerationComplete.Broadcast(false, FGeneratedMusicAsset());
        return;
    }

    CurrentStatus = EMusicAssetGenerationStatus::Generating;
    GenerationProgress = 0.0f;
    LastErrorMessage.Empty();

    // 使用 SageBrain 增強提示詞
    FMusicAssetGenerationParams EnhancedParams = Params;
    if (bSageBrainEnhancementEnabled)
    {
        EnhancedParams.Prompt = EnhancePromptWithSageBrain(Params.Prompt, Params.PhilosophyMode);
    }

    UpdateStatus(EMusicAssetGenerationStatus::Generating, 
        FString::Printf(TEXT("開始生成音樂: %s"), *EnhancedParams.OutputName));

    // 異步發送請求
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, EnhancedParams]()
    {
        SendGenerationRequest(EnhancedParams);
    });
}

void UMingAIGeneratedMusicAsset::SendGenerationRequest(const FMusicAssetGenerationParams& Params)
{
    FString RequestBody;
    
    // 根據提供商構建請求
    switch (Params.Provider)
    {
    case EMusicAIProvider::AIVA:
        RequestBody = BuildAIVARequest(Params);
        break;
    case EMusicAIProvider::Soundraw:
        RequestBody = BuildSoundrawRequest(Params);
        break;
    case EMusicAIProvider::OpenAI:
        RequestBody = BuildOpenAIRequest(Params);
        break;
    case EMusicAIProvider::Mock:
        RequestBody = BuildMockRequest(Params);
        break;
    default:
        RequestBody = BuildMockRequest(Params);
        break;
    }

    // 創建 HTTP 請求
    FString Endpoint = Params.CustomAPIEndpoint.IsEmpty() ? 
        TEXT("https://api.mock-music-service.com/generate") : Params.CustomAPIEndpoint;
    
    CurrentRequest = FHttpModule::Get().CreateRequest();
    CurrentRequest->SetURL(Endpoint);
    CurrentRequest->SetVerb(TEXT("POST"));
    CurrentRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    CurrentRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *CurrentAPIKey));
    CurrentRequest->SetContentAsString(RequestBody);

    // 綁定回調
    CurrentRequest->OnProcessRequestComplete().BindUObject(this, 
        &UMingAIGeneratedMusicAsset::HandleGenerationResponse);

    UpdateStatus(EMusicAssetGenerationStatus::Generating, TEXT("發送API請求..."));
    CurrentRequest->ProcessRequest();
}

void UMingAIGeneratedMusicAsset::HandleGenerationResponse(FHttpRequestPtr Request, 
    FHttpResponsePtr Response, bool bSuccess)
{
    if (!bSuccess || !Response.IsValid())
    {
        FinalizeGeneration(false, TEXT("HTTP請求失敗"));
        return;
    }

    int32 ResponseCode = Response->GetResponseCode();
    FString ResponseContent = Response->GetContentAsString();

    if (ResponseCode != 200)
    {
        FinalizeGeneration(false, FString::Printf(TEXT("API返回錯誤: %d - %s"), ResponseCode, *ResponseContent));
        return;
    }

    // 解析 JSON 響應
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);
    
    if (!FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        FinalizeGeneration(false, TEXT("無法解析API響應"));
        return;
    }

    // 提取音頻下載 URL
    FString DownloadUrl;
    if (!JsonObject->TryGetStringField(TEXT("audio_url"), DownloadUrl) ||
        !JsonObject->TryGetStringField(TEXT("url"), DownloadUrl))
    {
        // 如果是 Mock 模式，創建本地模擬音頻
        if (CurrentProvider == EMusicAIProvider::Mock)
        {
            // 在實際實現中，這裡會調用本地音頻生成邏輯
            FString MockPath = CreateMockAudioFile();
            if (!MockPath.IsEmpty())
            {
                ProcessDownloadedAudio(MockPath);
                return;
            }
        }
        
        FinalizeGeneration(false, TEXT("響應中未找到音頻URL"));
        return;
    }

    // 準備保存路徑
    FString OutputDir = FPaths::ProjectSavedDir() / TEXT("GeneratedAudio") / TEXT("Music");
    EnsureDirectoryExists(OutputDir);
    
    FString FileName = FString::Printf(TEXT("%s_%s.wav"), 
        *Params.OutputName, 
        *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    
    FString SavePath = OutputDir / FileName;
    PendingDownloadPath = SavePath;

    // 開始下載
    UpdateStatus(EMusicAssetGenerationStatus::Downloading, TEXT("下載生成的音頻..."));
    DownloadGeneratedAudio(DownloadUrl, SavePath);
}

void UMingAIGeneratedMusicAsset::DownloadGeneratedAudio(const FString& DownloadUrl, const FString& SavePath)
{
    FHttpRequestPtr DownloadRequest = FHttpModule::Get().CreateRequest();
    DownloadRequest->SetURL(DownloadUrl);
    DownloadRequest->SetVerb(TEXT("GET"));
    
    DownloadRequest->OnProcessRequestComplete().BindUObject(this,
        &UMingAIGeneratedMusicAsset::HandleDownloadComplete);

    DownloadRequest->ProcessRequest();
}

void UMingAIGeneratedMusicAsset::HandleDownloadComplete(FHttpRequestPtr Request, 
    FHttpResponsePtr Response, bool bSuccess)
{
    if (!bSuccess || !Response.IsValid())
    {
        FinalizeGeneration(false, TEXT("下載音頻失敗"));
        return;
    }

    // 保存文件
    const TArray<uint8>& AudioData = Response->GetContent();
    if (AudioData.Num() == 0)
    {
        FinalizeGeneration(false, TEXT("下載的音頻數據為空"));
        return;
    }

    if (!FFileHelper::SaveArrayToFile(AudioData, *PendingDownloadPath))
    {
        FinalizeGeneration(false, TEXT("保存音頻文件失敗"));
        return;
    }

    UpdateStatus(EMusicAssetGenerationStatus::Processing, TEXT("處理音頻文件..."));
    ProcessDownloadedAudio(PendingDownloadPath);
}

void UMingAIGeneratedMusicAsset::ProcessDownloadedAudio(const FString& FilePath)
{
    // 加載音頻文件為 SoundWave
    USoundWave* SoundWave = LoadSoundWaveFromFile(FilePath);
    if (!SoundWave)
    {
        FinalizeGeneration(false, TEXT("無法加載音頻文件"));
        return;
    }

    // 創建資產記錄
    FGeneratedMusicAsset Asset;
    Asset.AssetName = FPaths::GetBaseFilename(FilePath);
    Asset.FilePath = FilePath;
    Asset.SoundWave = SoundWave;
    Asset.Duration = SoundWave->Duration;
    Asset.SampleRate = SoundWave->GetSampleRateForCurrentPlatform();
    Asset.NumChannels = SoundWave->NumChannels;
    Asset.GenerationTime = FDateTime::Now();
    Asset.AIProvider = GetProviderAPIName(CurrentProvider);
    
    // 添加到資產庫
    GeneratedAssets.Add(Asset);

    UpdateStatus(EMusicAssetGenerationStatus::Completed, 
        FString::Printf(TEXT("音樂生成完成: %s"), *Asset.AssetName));
    
    FinalizeGeneration(true);
    OnGenerationComplete.Broadcast(true, Asset);
}

void UMingAIGeneratedMusicAsset::FinalizeGeneration(bool bSuccess, const FString& ErrorMessage)
{
    if (!bSuccess)
    {
        LastErrorMessage = ErrorMessage;
        CurrentStatus = EMusicAssetGenerationStatus::Failed;
        UE_LOG(LogTemp, Error, TEXT("音樂生成失敗: %s"), *ErrorMessage);
    }
    else
    {
        CurrentStatus = EMusicAssetGenerationStatus::Completed;
        GenerationProgress = 100.0f;
    }

    CurrentRequest.Reset();
    PendingDownloadPath.Empty();
}

void UMingAIGeneratedMusicAsset::CancelGeneration()
{
    if (CurrentRequest.IsValid())
    {
        CurrentRequest->CancelRequest();
    }
    
    CurrentStatus = EMusicAssetGenerationStatus::Cancelled;
    UpdateStatus(EMusicAssetGenerationStatus::Cancelled, TEXT("生成已取消"));
}

// API 請求構建函數
FString UMingAIGeneratedMusicAsset::BuildAIVARequest(const FMusicAssetGenerationParams& Params)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    
    JsonObject->SetStringField(TEXT("prompt"), Params.Prompt);
    JsonObject->SetNumberField(TEXT("duration"), Params.Duration);
    JsonObject->SetStringField(TEXT("style"), GetStyleDescription(Params.Style));
    JsonObject->SetNumberField(TEXT("tempo"), Params.Tempo);
    JsonObject->SetStringField(TEXT("key"), Params.Key);
    JsonObject->SetStringField(TEXT("mood"), Params.Mood);
    
    if (Params.Instruments.Num() > 0)
    {
        TArray<TSharedPtr<FJsonValue>> InstrumentsArray;
        for (const FString& Instrument : Params.Instruments)
        {
            InstrumentsArray.Add(MakeShared<FJsonValueString>(Instrument));
        }
        JsonObject->SetArrayField(TEXT("instruments"), InstrumentsArray);
    }

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    return OutputString;
}

FString UMingAIGeneratedMusicAsset::BuildSoundrawRequest(const FMusicAssetGenerationParams& Params)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    
    JsonObject->SetStringField(TEXT("description"), Params.Prompt);
    JsonObject->SetNumberField(TEXT("length"), Params.Duration);
    JsonObject->SetStringField(TEXT("genre"), GetStyleDescription(Params.Style));
    JsonObject->SetStringField(TEXT("energy"), Params.Mood);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    return OutputString;
}

FString UMingAIGeneratedMusicAsset::BuildOpenAIRequest(const FMusicAssetGenerationParams& Params)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    
    JsonObject->SetStringField(TEXT("model"), TEXT("music-gen-001"));
    JsonObject->SetStringField(TEXT("prompt"), Params.Prompt);
    JsonObject->SetNumberField(TEXT("duration_seconds"), Params.Duration);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    return OutputString;
}

FString UMingAIGeneratedMusicAsset::BuildMockRequest(const FMusicAssetGenerationParams& Params)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    
    JsonObject->SetStringField(TEXT("action"), TEXT("generate"));
    JsonObject->SetStringField(TEXT("prompt"), Params.Prompt);
    JsonObject->SetNumberField(TEXT("duration"), Params.Duration);
    
    // Mock 模式下直接返回本地測試音頻路徑
    JsonObject->SetStringField(TEXT("audio_url"), TEXT("mock://local/generate"));
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    return OutputString;
}

// 提示詞增強
FString UMingAIGeneratedMusicAsset::EnhancePromptWithSageBrain(const FString& BasePrompt, const FString& Philosophy)
{
    FString EnhancedPrompt = BasePrompt;
    
    // 根據哲學模式添加風格元素
    if (Philosophy == TEXT("Confucianism"))
    {
        EnhancedPrompt += TEXT(" with harmonious melodies, balanced orchestration, and dignified atmosphere");
    }
    else if (Philosophy == TEXT("Taoism"))
    {
        EnhancedPrompt += TEXT(" with natural flowing rhythms, peaceful ambiance, and subtle instrumentation");
    }
    else if (Philosophy == TEXT("Buddhism"))
    {
        EnhancedPrompt += TEXT(" with meditative qualities, serene tones, and contemplative mood");
    }
    else if (Philosophy == TEXT("Legalism"))
    {
        EnhancedPrompt += TEXT(" with strict rhythmic structure, powerful percussion, and commanding presence");
    }
    else if (Philosophy == TEXT("MilitaryStrategy"))
    {
        EnhancedPrompt += TEXT(" with strategic crescendos, tactical rhythm shifts, and intense battle themes");
    }
    
    return EnhancedPrompt;
}

FString UMingAIGeneratedMusicAsset::GetStyleDescription(EMusicAssetStyle Style)
{
    switch (Style)
    {
    case EMusicAssetStyle::Orchestral: return TEXT("orchestral");
    case EMusicAssetStyle::Traditional: return TEXT("traditional chinese");
    case EMusicAssetStyle::Electronic: return TEXT("electronic");
    case EMusicAssetStyle::Ambient: return TEXT("ambient");
    case EMusicAssetStyle::Rock: return TEXT("rock");
    case EMusicAssetStyle::Cinematic: return TEXT("cinematic");
    case EMusicAssetStyle::Battle: return TEXT("battle");
    case EMusicAssetStyle::Victory: return TEXT("victory");
    case EMusicAssetStyle::Suspense: return TEXT("suspense");
    case EMusicAssetStyle::Peaceful: return TEXT("peaceful");
    default: return TEXT("custom");
    }
}

TArray<FString> UMingAIGeneratedMusicAsset::GetInstrumentsForStyle(EMusicAssetStyle Style)
{
    TArray<FString> Instruments;
    
    switch (Style)
    {
    case EMusicAssetStyle::Traditional:
        Instruments.Add(TEXT("guqin"));
        Instruments.Add(TEXT("erhu"));
        Instruments.Add(TEXT("pipa"));
        Instruments.Add(TEXT("dizi"));
        Instruments.Add(TEXT("guzheng"));
        break;
    case EMusicAssetStyle::Orchestral:
        Instruments.Add(TEXT("strings"));
        Instruments.Add(TEXT("brass"));
        Instruments.Add(TEXT("woodwinds"));
        Instruments.Add(TEXT("percussion"));
        break;
    case EMusicAssetStyle::Battle:
        Instruments.Add(TEXT("drums"));
        Instruments.Add(TEXT("brass"));
        Instruments.Add(TEXT("percussion"));
        break;
    default:
        break;
    }
    
    return Instruments;
}

// 預設模板
FMusicAssetGenerationParams UMingAIGeneratedMusicAsset::GetBattleMusicPreset()
{
    FMusicAssetGenerationParams Params;
    Params.Prompt = TEXT("Epic battle music with traditional Chinese instruments, intense drums and powerful orchestral elements");
    Params.Style = EMusicAssetStyle::Battle;
    Params.Duration = 60.0f;
    Params.Tempo = 140;
    Params.Mood = TEXT("Intense");
    Params.Instruments = GetInstrumentsForStyle(EMusicAssetStyle::Battle);
    Params.bLoop = true;
    return Params;
}

FMusicAssetGenerationParams UMingAIGeneratedMusicAsset::GetAmbientMusicPreset()
{
    FMusicAssetGenerationParams Params;
    Params.Prompt = TEXT("Peaceful ambient background music with subtle traditional Chinese elements, atmospheric and relaxing");
    Params.Style = EMusicAssetStyle::Ambient;
    Params.Duration = 120.0f;
    Params.Tempo = 80;
    Params.Mood = TEXT("Peaceful");
    Params.bLoop = true;
    return Params;
}

FMusicAssetGenerationParams UMingAIGeneratedMusicAsset::GetVictoryMusicPreset()
{
    FMusicAssetGenerationParams Params;
    Params.Prompt = TEXT("Triumphant victory music with celebratory Chinese instruments and uplifting orchestral arrangement");
    Params.Style = EMusicAssetStyle::Victory;
    Params.Duration = 30.0f;
    Params.Tempo = 130;
    Params.Mood = TEXT("Triumphant");
    return Params;
}

FMusicAssetGenerationParams UMingAIGeneratedMusicAsset::GetTraditionalChinesePreset()
{
    FMusicAssetGenerationParams Params;
    Params.Prompt = TEXT("Authentic traditional Chinese music featuring guqin, erhu, and bamboo flute");
    Params.Style = EMusicAssetStyle::Traditional;
    Params.Duration = 90.0f;
    Params.Tempo = 100;
    Params.Mood = TEXT("Traditional");
    Params.Instruments = GetInstrumentsForStyle(EMusicAssetStyle::Traditional);
    return Params;
}

FMusicAssetGenerationParams UMingAIGeneratedMusicAsset::GetRepublicanEraPreset()
{
    FMusicAssetGenerationParams Params;
    Params.Prompt = TEXT("1920s-1940s Chinese Republican era music, blend of traditional and Western influences, nostalgic atmosphere");
    Params.Style = EMusicAssetStyle::Orchestral;
    Params.Duration = 60.0f;
    Params.Tempo = 110;
    Params.Mood = TEXT("Nostalgic");
    return Params;
}

// 批量生成
void UMingAIGeneratedMusicAsset::GenerateMusicPack(const TArray<FMusicAssetGenerationParams>& ParamsArray)
{
    BatchQueue = ParamsArray;
    CurrentBatchIndex = 0;
    BatchResults.Empty();
    
    if (BatchQueue.Num() > 0)
    {
        ProcessNextBatchItem();
    }
}

void UMingAIGeneratedMusicAsset::ProcessNextBatchItem()
{
    if (CurrentBatchIndex >= BatchQueue.Num())
    {
        CompleteBatchGeneration();
        return;
    }

    FMusicAssetGenerationParams Params = BatchQueue[CurrentBatchIndex];
    
    // 設置批次索引
    Params.OutputName = FString::Printf(TEXT("%s_%d"), *Params.OutputName, CurrentBatchIndex + 1);
    
    // 綁定單個完成的回調
    OnGenerationComplete.Clear();
    OnGenerationComplete.AddDynamic(this, &UMingAIGeneratedMusicAsset::OnBatchItemComplete);
    
    GenerateMusicAssetAsync(Params);
}

void UMingAIGeneratedMusicAsset::OnBatchItemComplete(bool bSuccess, const FGeneratedMusicAsset& GeneratedAsset)
{
    if (bSuccess)
    {
        BatchResults.Add(GeneratedAsset);
    }
    
    CurrentBatchIndex++;
    
    // 更新進度
    GenerationProgress = (float)CurrentBatchIndex / BatchQueue.Num() * 100.0f;
    OnGenerationProgress.Broadcast(TEXT("Batch"), GenerationProgress, 
        FString::Printf(TEXT("已完成 %d/%d"), CurrentBatchIndex, BatchQueue.Num()));
    
    // 處理下一個
    ProcessNextBatchItem();
}

void UMingAIGeneratedMusicAsset::CompleteBatchGeneration()
{
    bool bAllSuccess = (BatchResults.Num() == BatchQueue.Num());
    
    OnBatchGenerationComplete.Broadcast(bAllSuccess, BatchResults);
    
    BatchQueue.Empty();
    CurrentBatchIndex = 0;
}

void UMingAIGeneratedMusicAsset::GeneratePresetMusicPack(EMusicAssetStyle Style, int32 Count)
{
    TArray<FMusicAssetGenerationParams> ParamsArray;
    
    for (int32 i = 0; i < Count; i++)
    {
        FMusicAssetGenerationParams Params;
        Params.Style = Style;
        Params.OutputName = FString::Printf(TEXT("%s_Music_%d"), 
            *GetStyleDescription(Style), i + 1);
        ParamsArray.Add(Params);
    }
    
    GenerateMusicPack(ParamsArray);
}

// 資產管理
TArray<FGeneratedMusicAsset> UMingAIGeneratedMusicAsset::GetGeneratedAssets() const
{
    return GeneratedAssets;
}

FGeneratedMusicAsset UMingAIGeneratedMusicAsset::GetAssetByName(const FString& AssetName) const
{
    for (const FGeneratedMusicAsset& Asset : GeneratedAssets)
    {
        if (Asset.AssetName == AssetName)
        {
            return Asset;
        }
    }
    return FGeneratedMusicAsset();
}

bool UMingAIGeneratedMusicAsset::DeleteAsset(const FString& AssetName)
{
    for (int32 i = 0; i < GeneratedAssets.Num(); i++)
    {
        if (GeneratedAssets[i].AssetName == AssetName)
        {
            // 刪除文件
            if (FPaths::FileExists(GeneratedAssets[i].FilePath))
            {
                FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*GeneratedAssets[i].FilePath);
            }
            
            GeneratedAssets.RemoveAt(i);
            return true;
        }
    }
    return false;
}

void UMingAIGeneratedMusicAsset::ClearAllAssets()
{
    for (const FGeneratedMusicAsset& Asset : GeneratedAssets)
    {
        if (FPaths::FileExists(Asset.FilePath))
        {
            FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*Asset.FilePath);
        }
    }
    
    GeneratedAssets.Empty();
}

// 播放控制
void UMingAIGeneratedMusicAsset::PlayAsset(const FString& AssetName, float FadeInDuration)
{
    FGeneratedMusicAsset Asset = GetAssetByName(AssetName);
    if (!Asset.SoundWave)
    {
        UE_LOG(LogTemp, Warning, TEXT("無法播放資產: %s"), *AssetName);
        return;
    }

    if (!AudioComponent)
    {
        InitializeAudioComponent();
    }

    AudioComponent->SetSound(Asset.SoundWave);
    AudioComponent->FadeIn(FadeInDuration);
}

void UMingAIGeneratedMusicAsset::StopPlayback(float FadeOutDuration)
{
    if (AudioComponent)
    {
        AudioComponent->FadeOut(FadeOutDuration, 0.0f);
    }
}

void UMingAIGeneratedMusicAsset::PausePlayback()
{
    if (AudioComponent)
    {
        AudioComponent->SetPaused(true);
    }
}

void UMingAIGeneratedMusicAsset::ResumePlayback()
{
    if (AudioComponent)
    {
        AudioComponent->SetPaused(false);
    }
}

void UMingAIGeneratedMusicAsset::SetVolume(float Volume)
{
    if (AudioComponent)
    {
        AudioComponent->SetVolumeMultiplier(Volume);
    }
}

bool UMingAIGeneratedMusicAsset::IsPlaying() const
{
    return AudioComponent && AudioComponent->IsPlaying();
}

// API 配置
void UMingAIGeneratedMusicAsset::SetAPIKey(const FString& APIKey)
{
    CurrentAPIKey = APIKey;
}

void UMingAIGeneratedMusicAsset::SetProvider(EMusicAIProvider Provider)
{
    CurrentProvider = Provider;
}

void UMingAIGeneratedMusicAsset::SetCustomEndpoint(const FString& Endpoint)
{
    CustomAPIEndpoint = Endpoint;
}

bool UMingAIGeneratedMusicAsset::TestAPIConnection()
{
    // 實際實現中會發送測試請求到 API
    UE_LOG(LogTemp, Log, TEXT("測試 API 連接: %s"), *GetProviderAPIName(CurrentProvider));
    return true;
}

// SageBrain 整合
void UMingAIGeneratedMusicAsset::EnableSageBrainEnhancement(bool bEnable)
{
    bSageBrainEnhancementEnabled = bEnable;
}

// 輔助函數
FString UMingAIGeneratedMusicAsset::GetProviderAPIName(EMusicAIProvider Provider)
{
    switch (Provider)
    {
    case EMusicAIProvider::AIVA: return TEXT("AIVA");
    case EMusicAIProvider::Soundraw: return TEXT("Soundraw");
    case EMusicAIProvider::Boomy: return TEXT("Boomy");
    case EMusicAIProvider::Amper: return TEXT("Amper");
    case EMusicAIProvider::OpenAI: return TEXT("OpenAI");
    case EMusicAIProvider::LocalModel: return TEXT("Local");
    case EMusicAIProvider::Mock: return TEXT("Mock");
    default: return TEXT("Unknown");
    }
}

FString UMingAIGeneratedMusicAsset::GetQualityString(EMusicAssetQuality Quality)
{
    switch (Quality)
    {
    case EMusicAssetQuality::Low: return TEXT("128k");
    case EMusicAssetQuality::Medium: return TEXT("192k");
    case EMusicAssetQuality::High: return TEXT("320k");
    case EMusicAssetQuality::Lossless: return TEXT("lossless");
    default: return TEXT("standard");
    }
}

void UMingAIGeneratedMusicAsset::UpdateStatus(EMusicAssetGenerationStatus NewStatus, const FString& Message)
{
    CurrentStatus = NewStatus;
    LogGenerationEvent(GetProviderAPIName(CurrentProvider), Message);
}

void UMingAIGeneratedMusicAsset::LogGenerationEvent(const FString& Event, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[AIGeneratedMusic] %s: %s"), *Event, *Details);
}

void UMingAIGeneratedMusicAsset::InitializeAudioComponent()
{
    // 音頻組件應該在世界中創建，這裡只是準備
    // 實際實現中應該在擁有者的 BeginPlay 中創建
}

void UMingAIGeneratedMusicAsset::CleanupAudioComponent()
{
    if (AudioComponent)
    {
        AudioComponent->Stop();
        AudioComponent = nullptr;
    }
}

bool UMingAIGeneratedMusicAsset::EnsureDirectoryExists(const FString& Directory)
{
    return FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*Directory);
}

USoundWave* UMingAIGeneratedMusicAsset::LoadSoundWaveFromFile(const FString& FilePath)
{
    // 使用 Unreal 的方式加載音頻文件
    // 實際實現需要使用 RuntimeAudioImporter 或其他音頻加載庫
    
    TArray<uint8> AudioData;
    if (!FFileHelper::LoadFileToArray(AudioData, *FilePath))
    {
        return nullptr;
    }

    // 創建 SoundWave
    USoundWave* SoundWave = NewObject<USoundWave>(USoundWave::StaticClass());
    if (SoundWave)
    {
        // 設置音頻數據
        // 注意：這裡需要根據實際的音頻格式進行處理
        // 可能需要使用 RuntimeAudioImporter 插件
        SoundWave->RawData.Lock(LOCK_READ_WRITE);
        FMemory::Memcpy(SoundWave->RawData.Lock(LOCK_READ_WRITE), AudioData.GetData(), AudioData.Num());
        SoundWave->RawData.Unlock();
        
        SoundWave->Duration = 30.0f; // 需要從實際音頻數據計算
        SoundWave->SoundGroup = SOUNDGROUP_Default;
    }
    
    return SoundWave;
}

FString UMingAIGeneratedMusicAsset::CreateMockAudioFile()
{
    // 在 Mock 模式下創建測試用的音頻文件
    // 這裡可以生成簡單的 WAV 文件或使用預設的測試音頻
    
    FString OutputDir = FPaths::ProjectSavedDir() / TEXT("GeneratedAudio") / TEXT("Music");
    EnsureDirectoryExists(OutputDir);
    
    FString FileName = FString::Printf(TEXT("MockMusic_%s.wav"), 
        *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    
    FString FullPath = OutputDir / FileName;
    
    // 創建一個空的 WAV 文件頭
    // 實際實現中應該生成真正的音頻數據
    TArray<uint8> MockWavData;
    
    // 簡單的 WAV 文件頭
    MockWavData.Append((const uint8*)TEXT("RIFF"), 4);
    uint32 FileSize = 36 + 0; // Header + data
    MockWavData.Append((const uint8*)&FileSize, 4);
    MockWavData.Append((const uint8*)TEXT("WAVE"), 4);
    MockWavData.Append((const uint8*)TEXT("fmt "), 4);
    
    uint32 Subchunk1Size = 16;
    uint16 AudioFormat = 1; // PCM
    uint16 NumChannels = 2;
    uint32 SampleRate = 44100;
    uint32 ByteRate = SampleRate * NumChannels * 2;
    uint16 BlockAlign = NumChannels * 2;
    uint16 BitsPerSample = 16;
    
    MockWavData.Append((const uint8*)&Subchunk1Size, 4);
    MockWavData.Append((const uint8*)&AudioFormat, 2);
    MockWavData.Append((const uint8*)&NumChannels, 2);
    MockWavData.Append((const uint8*)&SampleRate, 4);
    MockWavData.Append((const uint8*)&ByteRate, 4);
    MockWavData.Append((const uint8*)&BlockAlign, 2);
    MockWavData.Append((const uint8*)&BitsPerSample, 2);
    MockWavData.Append((const uint8*)TEXT("data"), 4);
    
    uint32 DataSize = 0;
    MockWavData.Append((const uint8*)&DataSize, 4);
    
    FFileHelper::SaveArrayToFile(MockWavData, *FullPath);
    
    return FullPath;
}

bool UMingAIGeneratedMusicAsset::ExportAsset(const FString& AssetName, const FString& ExportPath)
{
    FGeneratedMusicAsset Asset = GetAssetByName(AssetName);
    if (Asset.FilePath.IsEmpty())
    {
        return false;
    }

    if (!FPaths::FileExists(Asset.FilePath))
    {
        return false;
    }

    return FPlatformFileManager::Get().GetPlatformFile().CopyFile(*ExportPath, *Asset.FilePath);
}

bool UMingAIGeneratedMusicAsset::ImportAsset(const FString& ImportPath, const FString& AssetName)
{
    if (!FPaths::FileExists(ImportPath))
    {
        return false;
    }

    FString OutputDir = FPaths::ProjectSavedDir() / TEXT("GeneratedAudio") / TEXT("Music");
    EnsureDirectoryExists(OutputDir);
    
    FString DestinationPath = OutputDir / AssetName + TEXT(".wav");
    
    if (FPlatformFileManager::Get().GetPlatformFile().CopyFile(*DestinationPath, *ImportPath))
    {
        // 加載並添加到資產庫
        USoundWave* SoundWave = LoadSoundWaveFromFile(DestinationPath);
        if (SoundWave)
        {
            FGeneratedMusicAsset Asset;
            Asset.AssetName = AssetName;
            Asset.FilePath = DestinationPath;
            Asset.SoundWave = SoundWave;
            Asset.GenerationTime = FDateTime::Now();
            Asset.AIProvider = TEXT("Imported");
            
            GeneratedAssets.Add(Asset);
            return true;
        }
    }
    
    return false;
}

void UMingAIGeneratedMusicAsset::OnGenerationProgressTick()
{
    // 更新進度
    if (IsGenerating())
    {
        GenerationProgress += 1.0f;
        if (GenerationProgress > 99.0f)
        {
            GenerationProgress = 99.0f;
        }
        
        OnGenerationProgress.Broadcast(TEXT("Current"), GenerationProgress, TEXT("生成中..."));
    }
}
