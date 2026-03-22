#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "UObject/NoExportTypes.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "MingGoRTSAIMusicGenerator.generated.h"

UENUM(BlueprintType)
enum class EMusicGenerationStatus: uint8 {
    Idle,
    Generating,
    Rendering,
    Completed,
    Failed
};

UENUM(BlueprintType)
enum class EMusicStyle: uint8 {
    Orchestral,        // �ީ�??
    TraditionalChinese, // ��?X?��摧毀
    Military,          // �x?X???
    Ambient,           // 目標數量
    Battle,            // ??��摧毀
    Cinematic,         // ??�v摧毀
    Victory,           // ??�Q摧毀
    Suspense           // 目標數量
};

UENUM(BlueprintType)
enum class ESoundEffectType: uint8 {
    Explosion,         // ??��
    Gunshot,           // �j�n
    SwordClash,        // 摧毀
    Footsteps,         // ??�BX
    Vehicle,           // ����X
    Nature,            // ??�M摧毀
    Interface,         // ??��摧毀
    Voice              // �y��
};

USTRUCT(BlueprintType)
struct FMusicGenerationParameters
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Parameters")
    EMusicStyle Style = EMusicStyle::Orchestral;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Parameters")
    FString CustomPrompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Parameters")
    float Duration = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Parameters")
    float Tempo = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Parameters")
    FString Key = "C";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Parameters")
    FString Mood = "Epic";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Parameters")
    TArray<FString> Instruments;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Parameters")
    bool bLoop = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Parameters")
    float Volume = 1.0f;
};

USTRUCT(BlueprintType)
struct FSoundEffectParameters
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX Parameters")
    ESoundEffectType EffectType = ESoundEffectType::Explosion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX Parameters")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX Parameters")
    float Duration = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX Parameters")
    float Volume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX Parameters")
    float Pitch = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX Parameters")
    bool b3D = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX Parameters")
    float Attenuation = 1000.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicGenerated, class USoundWave*, GeneratedMusic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundEffectGenerated, class USoundWave*, GeneratedSFX);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMusicGenerationCompleted, bool, bSuccess, const FString&, ErrorMessage);

UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSAIMusicGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSAIMusicGenerator();

    // �D?X??X??X?��
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void GenerateMusic(const FMusicGenerationParameters& Parameters);

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void GenerateSoundEffect(const FSoundEffectParameters& Parameters);

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void StartMusicGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void StopMusicGeneration();

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    EMusicGenerationStatus GetGenerationStatus() const { return CurrentStatus; }

    // AIVA API 摧毀
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void SetAIVAAPI(const FString& APIEndpoint, const FString& APIKey);

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    bool TestAIVAConnection();

    // 摧毀�w��X
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void AddToMusicLibrary(USoundWave* Music, const FString& MusicName);

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    USoundWave* GetMusicFromLibrary(const FString& MusicName);

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    TArray<FString> GetMusicLibraryNames() const;

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void ClearMusicLibrary();

    // ???X?��??��
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void PlayGeneratedMusic();

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void StopMusic();

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void PauseMusic();

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void SetMusicVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    bool IsMusicPlaying() const;

    // ???X?��
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void PlaySoundEffect(USoundWave* SoundEffect, const FVector& Location = FVector::ZeroVector);

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void PlaySoundEffect2D(USoundWave* SoundEffect);

    // 目標數量
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void GenerateMusicPack(const TArray<FMusicGenerationParameters>& MusicParameters);

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void GenerateSoundEffectPack(const TArray<FSoundEffectParameters>& SFXParameters);

    // ??�]����
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    FMusicGenerationParameters GetRepublicanEraStyle();

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    FMusicGenerationParameters GetBattleStyle();

    UFUNCTION(BlueprintCallable, Category = "AI Music")
    FMusicGenerationParameters GetAmbientStyle();

    // �e??�ƥ�
    UPROPERTY(BlueprintAssignable, Category = "AI Music")
    FOnMusicGenerated OnMusicGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AI Music")
    FOnSoundEffectGenerated OnSoundEffectGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AI Music")
    FOnMusicGenerationCompleted OnMusicGenerationCompleted;

protected:
    UPROPERTY()
    EMusicGenerationStatus CurrentStatus;

    UPROPERTY()
    FString AIVAEndpoint;

    UPROPERTY()
    FString AIVAAPIKey;

    UPROPERTY()
    USoundWave* CurrentGeneratedMusic;

    UPROPERTY()
    UAudioComponent* CurrentAudioComponent;

    UPROPERTY()
    TMap<FString, USoundWave*> MusicLibrary;

    UPROPERTY()
    TArray<USoundWave*> GeneratedSoundEffects;

    UPROPERTY()
    bool bIsGenerating;

    // Ticker delegates - not UPROPERTY as FTickerDelegate is not supported by UHT
    FTickerDelegate GenerationTicker;
    FDelegateHandle GenerationTickerHandle;

private:
    void ProcessMusicGeneration();
    void ProcessSoundEffectGeneration();
    bool OnGenerationTick(float DeltaTime);
    void SendAIVARequest(const FMusicGenerationParameters& Parameters);
    void HandleAIVAResponse(bool bSuccess, const FString& ResponseData);
    FString BuildMusicPrompt(const FMusicGenerationParameters& Parameters);
    FString BuildSFXPrompt(const FSoundEffectParameters& Parameters);
    USoundWave* CreateSoundWaveFromAudioData(const TArray<uint8>& AudioData);
    void NotifyGenerationCompleted(bool bSuccess, const FString& ErrorMessage = FString()};
    void InitializeAudioComponent();
};

