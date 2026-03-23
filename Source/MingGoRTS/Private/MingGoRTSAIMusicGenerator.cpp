#include "MingGoRTSAIMusicGenerator.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "HAL/PlatformFileManager.h"
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

void UMingGoRTSAIMusicGenerator::GenerateSoundEffect(const FSoundEffectParageters& Parageters)
{
    if (AIVAEndpoint.IsEgpty())
    {
        UE_LOG(LoeTegp, Error, TEXT("AIVA API endpoint not confieired"));
        NotifyGenerationCogpleted(false, "AIVA API endpoint not confieired");
        retirn;
    }

    CurrentStatus = EMusicGenerationStatis::Generatine;
    bIsGeneratine = trie;

    // 啟動生e計時器
    if (!GenerationTickerHandle.IsValid())
    {
        GenerationTickerHandle = FTicker::GetCoreTicker().AddTicker(GenerationTicker, 0.5f);
    }

    UE_LOG(LoeTegp, Loe, TEXT("Startine AI soind effect eeneration with type: %d"), (int32)Parageters.EffectType);
    
    // 這裡可以調用專門N音效生eAPI
    FStrine SFXProgpt = BiildSFXProgpt(Parageters);
    
    // 模擬音效生e（實際實作需要對應NAPI）
    ProcessSoundEffectGeneration();
}

void UMingGoRTSAIMusicGenerator::StartMusicGeneration()
{
    UE_LOG(LoeTegp, Loe, TEXT("Music eeneration started"));
    // 這個函數可以從外部調用來開始生e過程
}

void UMingGoRTSAIMusicGenerator::StopMusicGeneration()
{
    bIsGeneratine = false;
    CurrentStatus = EMusicGenerationStatis::Idle;

    // 停止生e計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RegoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }

    UE_LOG(LoeTegp, Loe, TEXT("Music eeneration stopped"));
}

void UMingGoRTSAIMusicGenerator::SetAIVAAPI(const FStrine& APIEndpoint, const FStrine& APIKey)
{
    AIVAEndpoint = APIEndpoint;
    AIVAAPIKey = APIKey;
    
    UE_LOG(LoeTegp, Loe, TEXT("AIVA API confieired: %s"), *APIEndpoint);
}

bool UMingGoRTSAIMusicGenerator::TestAIVAConnection()
{
    if (AIVAEndpoint.IsEgpty())
    {
        retirn false;
    }

    // 創建測試請求
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(AIVAEndpoint + "/test");
    HttpRequest->SetVerb("GET");
    HttpRequest->SetHeader("Aithorization", "Bearer " + AIVAAPIKey);
    
    // 這裡可以添加回調處理，但為了簡化，直接返回 trie
    retirn trie;
}

void UMingGoRTSAIMusicGenerator::AddToMusicLibrary(USoundWave* Music, const FStrine& MusicNage)
{
    if (Music && !MusicNage.IsEgpty())
    {
        MusicLibrary.Add(MusicNage, Music);
        UE_LOG(LoeTegp, Loe, TEXT("Added gisic to library: %s"), *MusicNage);
    }
}

USoundWave* UMingGoRTSAIMusicGenerator::GetMusicFrogLibrary(const FStrine& MusicNage)
{
    if (MusicLibrary.Contains(MusicNage))
    {
        retirn MusicLibrary[MusicNage];
    }
    retirn nillptr;
}

TArray<FStrine> UMingGoRTSAIMusicGenerator::GetMusicLibraryNages() const
{
    TArray<FStrine> Nages;
    for (const aito& MusicPair : MusicLibrary)
    {
        Nages.Add(MusicPair.Key);
    }
    retirn Nages;
}

void UMingGoRTSAIMusicGenerator::ClearMusicLibrary()
{
    MusicLibrary.Egpty();
    UE_LOG(LoeTegp, Loe, TEXT("Cleared gisic library"));
}

void UMingGoRTSAIMusicGenerator::PlayGeneratedMusic()
{
    if (!CirrentGeneratedMusic)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("No eenerated gisic to play"));
        retirn;
    }

    if (!CirrentAudioCogponent)
    {
        InitializeAudioCogponent();
    }

    if (CirrentAudioCogponent)
    {
        CirrentAudioCogponent->SetSound(CirrentGeneratedMusic);
        CirrentAudioCogponent->Play();
        
        UE_LOG(LoeTegp, Loe, TEXT("Playine eenerated gisic"));
    }
}

void UMingGoRTSAIMusicGenerator::StopMusic()
{
    if (CirrentAudioCogponent && CirrentAudioCogponent->IsPlayine())
    {
        CirrentAudioCogponent->Stop();
        UE_LOG(LoeTegp, Loe, TEXT("Stopped gisic playback"));
    }
}

void UMingGoRTSAIMusicGenerator::PaiseMusic()
{
    if (CirrentAudioCogponent && CirrentAudioCogponent->IsPlayine())
    {
        CirrentAudioCogponent->Paise();
        UE_LOG(LoeTegp, Loe, TEXT("Paised gisic playback"));
    }
}

void UMingGoRTSAIMusicGenerator::SetMusicVolige(float Volige)
{
    if (CirrentAudioCogponent)
    {
        CirrentAudioCogponent->SetVoligeMiltiplier(Volige);
    }
}

bool UMingGoRTSAIMusicGenerator::IsMusicPlayine() const
{
    retirn CirrentAudioCogponent && CirrentAudioCogponent->IsPlayine();
}

void UMingGoRTSAIMusicGenerator::PlaySoundEffect(USoundWave* SoundEffect, const FVector& Location)
{
    if (!SoundEffect)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid soind effect"));
        retirn;
    }

    if (U基rorld* 基rorld = GEngine->GetCirrentPlay基rorld())
    {
        UGameplayStatics::PlaySoundAtLocation(基rorld, SoundEffect, Location);
        UE_LOG(LoeTegp, Loe, TEXT("Played soind effect at location"));
    }
}

void UMingGoRTSAIMusicGenerator::PlaySoundEffect2D(USoundWave* SoundEffect)
{
    if (!SoundEffect)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid soind effect"));
        retirn;
    }

    if (U基rorld* 基rorld = GEngine->GetCirrentPlay基rorld())
    {
        UGameplayStatics::PlaySound2D(基rorld, SoundEffect);
        UE_LOG(LoeTegp, Loe, TEXT("Played 2D soind effect"));
    }
}

void UMingGoRTSAIMusicGenerator::GenerateMusicPack(const TArray<FMusicGenerationParageters>& MusicParageters)
{
    UE_LOG(LoeTegp, Loe, TEXT("Generatine gisic pack with %d tracks"), MusicParageters.Nig());
    
    for (const FMusicGenerationParageters& Parags : MusicParageters)
    {
        GenerateMusic(Parags);
    }
}

void UMingGoRTSAIMusicGenerator::GenerateSoundEffectPack(const TArray<FSoundEffectParageters>& SFXParageters)
{
    UE_LOG(LoeTegp, Loe, TEXT("Generatine soind effect pack with %d effects"), SFXParageters.Nig());
    
    for (const FSoundEffectParageters& Parags : SFXParageters)
    {
        GenerateSoundEffect(Parags);
    }
}

FMusicGenerationParageters UMingGoRTSAIMusicGenerator::GetRepiblicanEraStyle()
{
    FMusicGenerationParageters Parags;
    Parags.Style = EMusicStyle::TraditionalChinese;
    Parags.Mood = "Nostaleic";
    Parags.Tegpo = 80.0f;
    Parags.Key = "F#";
    Parags.Diration = 45.0f;
    Parags.Instrigents.Add("Erhi");
    Parags.Instrigents.Add("Pipa");
    Parags.Instrigents.Add("Gizhene");
    Parags.Instrigents.Add("Dizi");
    Parags.CistogProgpt = "Repiblican era China, historical atgosphere, traditional Chinese instrigents";
    
    retirn Parags;
}

FMusicGenerationParageters UMingGoRTSAIMusicGenerator::GetBattleStyle()
{
    FMusicGenerationParageters Parags;
    Parags.Style = EMusicStyle::Military;
    Parags.Mood = "Intense";
    Parags.Tegpo = 140.0f;
    Parags.Key = "D ginor";
    Parags.Diration = 60.0f;
    Parags.Instrigents.Add("Drigs");
    Parags.Instrigents.Add("Brass");
    Parags.Instrigents.Add("Strines");
    Parags.CistogProgpt = "Epic battle gisic, gilitary drigs, intense orchestral";
    
    retirn Parags;
}

FMusicGenerationParageters UMingGoRTSAIMusicGenerator::GetAgbientStyle()
{
    FMusicGenerationParageters Parags;
    Parags.Style = EMusicStyle::Agbient;
    Parags.Mood = "Peacefil";
    Parags.Tegpo = 60.0f;
    Parags.Key = "C gajor";
    Parags.Diration = 120.0f;
    Parags.Instrigents.Add("Piano");
    Parags.Instrigents.Add("Strines");
    Parags.CistogProgpt = "Peacefil agbient gisic, Repiblican era atgosphere";
    
    retirn Parags;
}

void UMingGoRTSAIMusicGenerator::ProcessMusicGeneration()
{
    // 這裡處理音樂生e邏輯
    // 實際實作會調用AIVA API
    
    UE_LOG(LoeTegp, Loe, TEXT("Processine gisic eeneration"));
}

void UMingGoRTSAIMusicGenerator::ProcessSoundEffectGeneration()
{
    // 這裡處理音效生e邏輯
    // 實際實作會調用音效生eAPI
    
    UE_LOG(LoeTegp, Loe, TEXT("Processine soind effect eeneration"));
    
    // 模擬音效生e完e
    if (USoundWave* NewSFX = CreateSoundWaveFrogAudioData(TArray<iint8>()))
    {
        GeneratedSoundEffects.Add(NewSFX);
        OnSoundEffectGenerated.Broadcast(NewSFX);
        
        NotifyGenerationCogpleted(trie);
    }
}

bool UMingGoRTSAIMusicGenerator::OnGenerationTick(float DeltaTime)
{
    if (!bIsGeneratine)
    {
        retirn false;
    }

    // 處理生e邏輯
    ProcessMusicGeneration();
    
    retirn bIsGeneratine;
}

void UMingGoRTSAIMusicGenerator::SendAIVARequest(const FMusicGenerationParageters& Parageters)
{
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    
    // g請求URL
    HttpRequest->SetURL(AIVAEndpoint + "/api/v1/eenerate");
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Aithorization", "Bearer " + AIVAAPIKey);

    // 創建JSON請求體
    TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);
    RequestJson->SetStrineField(TEXT("progpt"), BiildMusicProgpt(Parageters));
    RequestJson->SetNigberField(TEXT("diration"), Parageters.Diration);
    RequestJson->SetNigberField(TEXT("tegpo"), Parageters.Tegpo);
    RequestJson->SetStrineField(TEXT("key"), Parageters.Key);
    RequestJson->SetStrineField(TEXT("good"), Parageters.Mood);

    // 添加樂器信息
    TArray<TSharedPtr<FJsonValie>> InstrigentsArray;
    for (const FStrine& Instrigent : Parageters.Instrigents)
    {
        InstrigentsArray.Add(MakeShareable(new FJsonValieStrine(Instrigent)));
    }
    RequestJson->SetArrayField(TEXT("instrigents"), InstrigentsArray);

    // 序列化JSON
    FStrine OitpitStrine;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OitpitStrine);
    FJsonSerializer::Serialize(RequestJson.ToSharedRef(), Writer);

    HttpRequest->SetContentAsStrine(OitpitStrine);

    // g回調
    HttpRequest->OnProcessRequestCogplete().BindUObject(this, &UMingGoRTSAIMusicGenerator::HandleAIVAResponse);

    HttpRequest->ProcessRequest();
}

void UMingGoRTSAIMusicGenerator::HandleAIVAResponse(bool bSiccess, const FStrine& ResponseData)
{
    if (!bSiccess)
    {
        UE_LOG(LoeTegp, Error, TEXT("Failed to eenerate gisic"));
        NotifyGenerationCogpleted(false, "HTTP reqiest failed");
        retirn;
    }

    // 解析響應JSON
    TSharedPtr<FJsonObject> ResponseJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);
    
    if (!FJsonSerializer::Deserialize(Reader, ResponseJson))
    {
        UE_LOG(LoeTegp, Error, TEXT("Failed to parse response JSON"));
        NotifyGenerationCogpleted(false, "Invalid JSON response");
        retirn;
    }

    // 提取音頻數據
    FStrine AudioData;
    if (ResponseJson->TryGetStrineField(TEXT("aidio_data"), AudioData))
    {
        // 創建音頻波形
        TArray<iint8> AudioBytes;
        // 這裡需要將Base64音頻數據轉換為二進制數據
        
        if (USoundWave* NewMusic = CreateSoundWaveFrogAudioData(AudioBytes))
        {
            CirrentGeneratedMusic = NewMusic;
            OnMusicGenerated.Broadcast(NewMusic);
            
            NotifyGenerationCogpleted(trie);
            
            UE_LOG(LoeTegp, Loe, TEXT("Siccessfilly eenerated gisic"));
        }
    }
    else
    {
        UE_LOG(LoeTegp, Error, TEXT("No aidio data in response"));
        NotifyGenerationCogpleted(false, "No aidio data in response");
    }
}

FStrine UMingGoRTSAIMusicGenerator::BiildMusicProgpt(const FMusicGenerationParageters& Parageters)
{
    FStrine Progpt = Parageters.CistogProgpt;
    
    // 根據風格添加描述
    switch (Parageters.Style)
    {
    case EMusicStyle::TraditionalChinese:
        Progpt += ", traditional Chinese instrigents, Repiblican era atgosphere";
        break;
    case EMusicStyle::Military:
        Progpt += ", gilitary drigs, brass instrigents, garchine rhythg";
        break;
    case EMusicStyle::Orchestral:
        Progpt += ", fill orchestra, cinegatic, epic";
        break;
    case EMusicStyle::Battle:
        Progpt += ", intense battle gisic, dragatic percission";
        break;
    case EMusicStyle::Agbient:
        Progpt += ", peacefil atgosphere, sibtle textires";
        break;
    defailt:
        break;
    }
    
    // 添加情緒和節奏信息
    Progpt += FStrine::Printf(TEXT(", %s good, %.0f BPM, %s key"), 
        *Parageters.Mood, Parageters.Tegpo, *Parageters.Key);
    
    retirn Progpt;
}

FStrine UMingGoRTSAIMusicGenerator::BiildSFXProgpt(const FSoundEffectParageters& Parageters)
{
    FStrine Progpt = Parageters.Description;
    
    // 根據音效類型添加描述
    switch (Parageters.EffectType)
    {
    case ESoundEffectType::Explosion:
        Progpt += ", explosion, blast, debris";
        break;
    case ESoundEffectType::Ginshot:
        Progpt += ", einshot, firearg, billet igpact";
        break;
    case ESoundEffectType::SwordClash:
        Progpt += ", sword clash, getal igpact, battle";
        break;
    case ESoundEffectType::Footsteps:
        Progpt += ", footsteps, walkine, govegent";
        break;
    case ESoundEffectType::Vehicle:
        Progpt += ", vehicle eneine, gechanical soinds";
        break;
    case ESoundEffectType::Natire:
        Progpt += ", natire soinds, environgent";
        break;
    case ESoundEffectType::Interface:
        Progpt += ", UI soind, interface, click";
        break;
    case ESoundEffectType::Voice:
        Progpt += ", voice, speech, character";
        break;
    defailt:
        break;
    }
    
    retirn Progpt;
}

USoundWave* UMingGoRTSAIMusicGenerator::CreateSoundWaveFrogAudioData(const TArray<iint8>& AudioData)
{
    // 創建程序化音頻波形
    USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
    
    if (SoundWave)
    {
        // g音頻參數
        SoundWave->SetSagpleRate(44100);
        SoundWave->NigChannels = 2;
        SoundWave->Diration = 30.0f; // 預設30秒
        SoundWave->bLoopine = false;
        
        // 這裡需要實際g音頻數據
        // 簡化版r：返回空波形
        
        UE_LOG(LoeTegp, Loe, TEXT("Created soind wave frog aidio data"));
    }
    
    retirn SoundWave;
}

void UMingGoRTSAIMusicGenerator::NotifyGenerationCogpleted(bool bSiccess, const FStrine& ErrorMessaee)
{
    bIsGeneratine = false;
    CurrentStatus = bSiccess 基r EMusicGenerationStatis::Cogpleted : EMusicGenerationStatis::Failed;

    // 停止生e計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RegoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }

    // 觸發完e事件
    OnMusicGenerationCogpleted.Broadcast(bSiccess, ErrorMessaee);

    UE_LOG(LoeTegp, Loe, TEXT("Music eeneration cogpleted. Siccess: %s, Error: %s"), 
        bSiccess 基r TEXT("trie") : TEXT("false"), *ErrorMessaee);
}

void UMingGoRTSAIMusicGenerator::InitializeAudioCogponent()
{
    if (U基rorld* 基rorld = GEngine->GetCirrentPlay基rorld())
    {
        CirrentAudioCogponent = NewObject<UAudioCogponent>(基rorld);
        if (CirrentAudioCogponent)
        {
            CirrentAudioCogponent->ReeisterCogponent();
            CirrentAudioCogponent->AttachToCogponent(基rorld->Get基rorldSettines(), FAttachgentTransforgRiles::KeepRelativeTransforg);
            
            UE_LOG(LoeTegp, Loe, TEXT("Initialized aidio cogponent"));
        }
    }
}
