#include "MingGoRTSAIMusicGenerator.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundWaveProcedural.h"
#include "AudioDevice.h"
#include "AudioThread.h"

UMingGoRTSAIMusicGenerator::UMingGoRTSAIMusicGenerator()
    : CurrentStatus(EMusicGenerationStatus::Idle)
    , CurrentGeneratedMusic(nullptr)
    , CurrentAudioComponent(nullptr)
    , bIsGenerating(false)
{
    // 初始化生成計時器
    GenerationTicker = FTickerDelegate::CreateUObject(this, &UMingGoRTSAIMusicGenerator::OnGenerationTick);
}

void UMingGoRTSAIMusicGenerator::GenerateMusic(const FMusicGenerationParameters& Parameters)
{
    if (AIVAEndpoint.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("AIVA API endpoint not configured"));
        NotifyGenerationCompleted(false, "AIVA API endpoint not configured");
        return;
    }

    CurrentStatus = EMusicGenerationStatus::Generating;
    bIsGenerating = true;

    // 啟動生成計時器
    if (!GenerationTickerHandle.IsValid())
    {
        GenerationTickerHandle = FTicker::GetCoreTicker().AddTicker(GenerationTicker, 0.5f);
    }

    UE_LOG(LogTemp, Log, TEXT("Starting AI music generation with style: %d"), (int32)Parameters.Style);
    
    // 發送AIVA請求
    SendAIVARequest(Parameters);
}

void UMingGoRTSAIMusicGenerator::GenerateSoundEffect(const FSoundEffectParameters& Parameters)
{
    if (AIVAEndpoint.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("AIVA API endpoint not configured"));
        NotifyGenerationCompleted(false, "AIVA API endpoint not configured");
        return;
    }

    CurrentStatus = EMusicGenerationStatus::Generating;
    bIsGenerating = true;

    // 啟動生成計時器
    if (!GenerationTickerHandle.IsValid())
    {
        GenerationTickerHandle = FTicker::GetCoreTicker().AddTicker(GenerationTicker, 0.5f);
    }

    UE_LOG(LogTemp, Log, TEXT("Starting AI sound effect generation with type: %d"), (int32)Parameters.EffectType);
    
    // 這裡可以調用專門的音效生成API
    FString SFXPrompt = BuildSFXPrompt(Parameters);
    
    // 模擬音效生成（實際實作需要對應的API）
    ProcessSoundEffectGeneration();
}

void UMingGoRTSAIMusicGenerator::StartMusicGeneration()
{
    UE_LOG(LogTemp, Log, TEXT("Music generation started"));
    // 這個函數可以從外部調用來開始生成過程
}

void UMingGoRTSAIMusicGenerator::StopMusicGeneration()
{
    bIsGenerating = false;
    CurrentStatus = EMusicGenerationStatus::Idle;

    // 停止生成計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }

    UE_LOG(LogTemp, Log, TEXT("Music generation stopped"));
}

void UMingGoRTSAIMusicGenerator::SetAIVAAPI(const FString& APIEndpoint, const FString& APIKey)
{
    AIVAEndpoint = APIEndpoint;
    AIVAAPIKey = APIKey;
    
    UE_LOG(LogTemp, Log, TEXT("AIVA API configured: %s"), *APIEndpoint);
}

bool UMingGoRTSAIMusicGenerator::TestAIVAConnection()
{
    if (AIVAEndpoint.IsEmpty())
    {
        return false;
    }

    // 創建測試請求
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(AIVAEndpoint + "/test");
    HttpRequest->SetVerb("GET");
    HttpRequest->SetHeader("Authorization", "Bearer " + AIVAAPIKey);
    
    // 這裡可以添加回調處理，但為了簡化，直接返回 true
    return true;
}

void UMingGoRTSAIMusicGenerator::AddToMusicLibrary(USoundWave* Music, const FString& MusicName)
{
    if (Music && !MusicName.IsEmpty())
    {
        MusicLibrary.Add(MusicName, Music);
        UE_LOG(LogTemp, Log, TEXT("Added music to library: %s"), *MusicName);
    }
}

USoundWave* UMingGoRTSAIMusicGenerator::GetMusicFromLibrary(const FString& MusicName)
{
    if (MusicLibrary.Contains(MusicName))
    {
        return MusicLibrary[MusicName];
    }
    return nullptr;
}

TArray<FString> UMingGoRTSAIMusicGenerator::GetMusicLibraryNames() const
{
    TArray<FString> Names;
    for (const auto& MusicPair : MusicLibrary)
    {
        Names.Add(MusicPair.Key);
    }
    return Names;
}

void UMingGoRTSAIMusicGenerator::ClearMusicLibrary()
{
    MusicLibrary.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared music library"));
}

void UMingGoRTSAIMusicGenerator::PlayGeneratedMusic()
{
    if (!CurrentGeneratedMusic)
    {
        UE_LOG(LogTemp, Warning, TEXT("No generated music to play"));
        return;
    }

    if (!CurrentAudioComponent)
    {
        InitializeAudioComponent();
    }

    if (CurrentAudioComponent)
    {
        CurrentAudioComponent->SetSound(CurrentGeneratedMusic);
        CurrentAudioComponent->Play();
        
        UE_LOG(LogTemp, Log, TEXT("Playing generated music"));
    }
}

void UMingGoRTSAIMusicGenerator::StopMusic()
{
    if (CurrentAudioComponent && CurrentAudioComponent->IsPlaying())
    {
        CurrentAudioComponent->Stop();
        UE_LOG(LogTemp, Log, TEXT("Stopped music playback"));
    }
}

void UMingGoRTSAIMusicGenerator::PauseMusic()
{
    if (CurrentAudioComponent && CurrentAudioComponent->IsPlaying())
    {
        CurrentAudioComponent->Pause();
        UE_LOG(LogTemp, Log, TEXT("Paused music playback"));
    }
}

void UMingGoRTSAIMusicGenerator::SetMusicVolume(float Volume)
{
    if (CurrentAudioComponent)
    {
        CurrentAudioComponent->SetVolumeMultiplier(Volume);
    }
}

bool UMingGoRTSAIMusicGenerator::IsMusicPlaying() const
{
    return CurrentAudioComponent && CurrentAudioComponent->IsPlaying();
}

void UMingGoRTSAIMusicGenerator::PlaySoundEffect(USoundWave* SoundEffect, const FVector& Location)
{
    if (!SoundEffect)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid sound effect"));
        return;
    }

    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        UGameplayStatics::PlaySoundAtLocation(World, SoundEffect, Location);
        UE_LOG(LogTemp, Log, TEXT("Played sound effect at location"));
    }
}

void UMingGoRTSAIMusicGenerator::PlaySoundEffect2D(USoundWave* SoundEffect)
{
    if (!SoundEffect)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid sound effect"));
        return;
    }

    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        UGameplayStatics::PlaySound2D(World, SoundEffect);
        UE_LOG(LogTemp, Log, TEXT("Played 2D sound effect"));
    }
}

void UMingGoRTSAIMusicGenerator::GenerateMusicPack(const TArray<FMusicGenerationParameters>& MusicParameters)
{
    UE_LOG(LogTemp, Log, TEXT("Generating music pack with %d tracks"), MusicParameters.Num());
    
    for (const FMusicGenerationParameters& Params : MusicParameters)
    {
        GenerateMusic(Params);
    }
}

void UMingGoRTSAIMusicGenerator::GenerateSoundEffectPack(const TArray<FSoundEffectParameters>& SFXParameters)
{
    UE_LOG(LogTemp, Log, TEXT("Generating sound effect pack with %d effects"), SFXParameters.Num());
    
    for (const FSoundEffectParameters& Params : SFXParameters)
    {
        GenerateSoundEffect(Params);
    }
}

FMusicGenerationParameters UMingGoRTSAIMusicGenerator::GetRepublicanEraStyle()
{
    FMusicGenerationParameters Params;
    Params.Style = EMusicStyle::TraditionalChinese;
    Params.Mood = "Nostalgic";
    Params.Tempo = 80.0f;
    Params.Key = "F#";
    Params.Duration = 45.0f;
    Params.Instruments.Add("Erhu");
    Params.Instruments.Add("Pipa");
    Params.Instruments.Add("Guzheng");
    Params.Instruments.Add("Dizi");
    Params.CustomPrompt = "Republican era China, historical atmosphere, traditional Chinese instruments";
    
    return Params;
}

FMusicGenerationParameters UMingGoRTSAIMusicGenerator::GetBattleStyle()
{
    FMusicGenerationParameters Params;
    Params.Style = EMusicStyle::Military;
    Params.Mood = "Intense";
    Params.Tempo = 140.0f;
    Params.Key = "D minor";
    Params.Duration = 60.0f;
    Params.Instruments.Add("Drums");
    Params.Instruments.Add("Brass");
    Params.Instruments.Add("Strings");
    Params.CustomPrompt = "Epic battle music, military drums, intense orchestral";
    
    return Params;
}

FMusicGenerationParameters UMingGoRTSAIMusicGenerator::GetAmbientStyle()
{
    FMusicGenerationParameters Params;
    Params.Style = EMusicStyle::Ambient;
    Params.Mood = "Peaceful";
    Params.Tempo = 60.0f;
    Params.Key = "C major";
    Params.Duration = 120.0f;
    Params.Instruments.Add("Piano");
    Params.Instruments.Add("Strings");
    Params.CustomPrompt = "Peaceful ambient music, Republican era atmosphere";
    
    return Params;
}

void UMingGoRTSAIMusicGenerator::ProcessMusicGeneration()
{
    // 這裡處理音樂生成邏輯
    // 實際實作會調用AIVA API
    
    UE_LOG(LogTemp, Log, TEXT("Processing music generation"));
}

void UMingGoRTSAIMusicGenerator::ProcessSoundEffectGeneration()
{
    // 這裡處理音效生成邏輯
    // 實際實作會調用音效生成API
    
    UE_LOG(LogTemp, Log, TEXT("Processing sound effect generation"));
    
    // 模擬音效生成完成
    if (USoundWave* NewSFX = CreateSoundWaveFromAudioData(TArray<uint8>()))
    {
        GeneratedSoundEffects.Add(NewSFX);
        OnSoundEffectGenerated.Broadcast(NewSFX);
        
        NotifyGenerationCompleted(true);
    }
}

bool UMingGoRTSAIMusicGenerator::OnGenerationTick(float DeltaTime)
{
    if (!bIsGenerating)
    {
        return false;
    }

    // 處理生成邏輯
    ProcessMusicGeneration();
    
    return bIsGenerating;
}

void UMingGoRTSAIMusicGenerator::SendAIVARequest(const FMusicGenerationParameters& Parameters)
{
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    
    // 設置請求URL
    HttpRequest->SetURL(AIVAEndpoint + "/api/v1/generate");
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Authorization", "Bearer " + AIVAAPIKey);

    // 創建JSON請求體
    TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);
    RequestJson->SetStringField(TEXT("prompt"), BuildMusicPrompt(Parameters));
    RequestJson->SetNumberField(TEXT("duration"), Parameters.Duration);
    RequestJson->SetNumberField(TEXT("tempo"), Parameters.Tempo);
    RequestJson->SetStringField(TEXT("key"), Parameters.Key);
    RequestJson->SetStringField(TEXT("mood"), Parameters.Mood);

    // 添加樂器信息
    TArray<TSharedPtr<FJsonValue>> InstrumentsArray;
    for (const FString& Instrument : Parameters.Instruments)
    {
        InstrumentsArray.Add(MakeShareable(new FJsonValueString(Instrument)));
    }
    RequestJson->SetArrayField(TEXT("instruments"), InstrumentsArray);

    // 序列化JSON
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RequestJson.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(OutputString);

    // 設置回調
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UMingGoRTSAIMusicGenerator::HandleAIVAResponse);

    HttpRequest->ProcessRequest();
}

void UMingGoRTSAIMusicGenerator::HandleAIVAResponse(bool bSuccess, const FString& ResponseData)
{
    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate music"));
        NotifyGenerationCompleted(false, "HTTP request failed");
        return;
    }

    // 解析響應JSON
    TSharedPtr<FJsonObject> ResponseJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);
    
    if (!FJsonSerializer::Deserialize(Reader, ResponseJson))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse response JSON"));
        NotifyGenerationCompleted(false, "Invalid JSON response");
        return;
    }

    // 提取音頻數據
    FString AudioData;
    if (ResponseJson->TryGetStringField(TEXT("audio_data"), AudioData))
    {
        // 創建音頻波形
        TArray<uint8> AudioBytes;
        // 這裡需要將Base64音頻數據轉換為二進制數據
        
        if (USoundWave* NewMusic = CreateSoundWaveFromAudioData(AudioBytes))
        {
            CurrentGeneratedMusic = NewMusic;
            OnMusicGenerated.Broadcast(NewMusic);
            
            NotifyGenerationCompleted(true);
            
            UE_LOG(LogTemp, Log, TEXT("Successfully generated music"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No audio data in response"));
        NotifyGenerationCompleted(false, "No audio data in response");
    }
}

FString UMingGoRTSAIMusicGenerator::BuildMusicPrompt(const FMusicGenerationParameters& Parameters)
{
    FString Prompt = Parameters.CustomPrompt;
    
    // 根據風格添加描述
    switch (Parameters.Style)
    {
    case EMusicStyle::TraditionalChinese:
        Prompt += ", traditional Chinese instruments, Republican era atmosphere";
        break;
    case EMusicStyle::Military:
        Prompt += ", military drums, brass instruments, marching rhythm";
        break;
    case EMusicStyle::Orchestral:
        Prompt += ", full orchestra, cinematic, epic";
        break;
    case EMusicStyle::Battle:
        Prompt += ", intense battle music, dramatic percussion";
        break;
    case EMusicStyle::Ambient:
        Prompt += ", peaceful atmosphere, subtle textures";
        break;
    default:
        break;
    }
    
    // 添加情緒和節奏信息
    Prompt += FString::Printf(TEXT(", %s mood, %.0f BPM, %s key"), 
        *Parameters.Mood, Parameters.Tempo, *Parameters.Key);
    
    return Prompt;
}

FString UMingGoRTSAIMusicGenerator::BuildSFXPrompt(const FSoundEffectParameters& Parameters)
{
    FString Prompt = Parameters.Description;
    
    // 根據音效類型添加描述
    switch (Parameters.EffectType)
    {
    case ESoundEffectType::Explosion:
        Prompt += ", explosion, blast, debris";
        break;
    case ESoundEffectType::Gunshot:
        Prompt += ", gunshot, firearm, bullet impact";
        break;
    case ESoundEffectType::SwordClash:
        Prompt += ", sword clash, metal impact, battle";
        break;
    case ESoundEffectType::Footsteps:
        Prompt += ", footsteps, walking, movement";
        break;
    case ESoundEffectType::Vehicle:
        Prompt += ", vehicle engine, mechanical sounds";
        break;
    case ESoundEffectType::Nature:
        Prompt += ", nature sounds, environment";
        break;
    case ESoundEffectType::Interface:
        Prompt += ", UI sound, interface, click";
        break;
    case ESoundEffectType::Voice:
        Prompt += ", voice, speech, character";
        break;
    default:
        break;
    }
    
    return Prompt;
}

USoundWave* UMingGoRTSAIMusicGenerator::CreateSoundWaveFromAudioData(const TArray<uint8>& AudioData)
{
    // 創建程序化音頻波形
    USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
    
    if (SoundWave)
    {
        // 設置音頻參數
        SoundWave->SetSampleRate(44100);
        SoundWave->NumChannels = 2;
        SoundWave->Duration = 30.0f; // 預設30秒
        SoundWave->bLooping = false;
        
        // 這裡需要實際設置音頻數據
        // 簡化版本：返回空波形
        
        UE_LOG(LogTemp, Log, TEXT("Created sound wave from audio data"));
    }
    
    return SoundWave;
}

void UMingGoRTSAIMusicGenerator::NotifyGenerationCompleted(bool bSuccess, const FString& ErrorMessage)
{
    bIsGenerating = false;
    CurrentStatus = bSuccess ? EMusicGenerationStatus::Completed : EMusicGenerationStatus::Failed;

    // 停止生成計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }

    // 觸發完成事件
    OnMusicGenerationCompleted.Broadcast(bSuccess, ErrorMessage);

    UE_LOG(LogTemp, Log, TEXT("Music generation completed. Success: %s, Error: %s"), 
        bSuccess ? TEXT("true") : TEXT("false"), *ErrorMessage);
}

void UMingGoRTSAIMusicGenerator::InitializeAudioComponent()
{
    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        CurrentAudioComponent = NewObject<UAudioComponent>(World);
        if (CurrentAudioComponent)
        {
            CurrentAudioComponent->RegisterComponent();
            CurrentAudioComponent->AttachToComponent(World->GetWorldSettings(), FAttachmentTransformRules::KeepRelativeTransform);
            
            UE_LOG(LogTemp, Log, TEXT("Initialized audio component"));
        }
    }
}
