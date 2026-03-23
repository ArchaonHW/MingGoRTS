#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "MingRTSAudioGenerationAPI.generated.h"

/**
 * 音頻生成API系統
 * 專門負責音樂和音效生成功能 (從PythonAPI分配而來)
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSAudioGenerationAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAudioGenerationAPI();

    // 初始化音頻生成系統
    UFUNCTION(BlueprintCallable, Category = "Audio Generation API")
    bool InitializeAudioGeneration();

    // 音樂生成器功能
    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Music")
    FString GenerateMusic(const FString& Prompt, int32 Duration = 10, const FString& OutputName = TEXT("generated"), const FString& Style = TEXT("default"));

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Music")
    FString GeneratePresetMusic(const FString& PresetName);

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Music")
    void GenerateAllPresetMusic();

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Music")
    TArray<FString> GetAvailableMusicPresets();

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Music")
    bool AddCustomMusicPreset(const FString& PresetName, const FString& Prompt, int32 Duration, const FString& Style);

    // 音效生成功能
    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Sound Effects")
    FString GenerateSoundEffect(const FString& Description, const FString& OutputName = TEXT("sfx"), const FString& Category = TEXT("general"));

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Sound Effects")
    TArray<FString> GenerateBattleSoundEffects();

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Sound Effects")
    TArray<FString> GenerateEnvironmentSounds(const FString& EnvironmentType);

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Sound Effects")
    TArray<FString> GenerateUISounds();

    // 音頻管理功能
    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Management")
    void ListGeneratedAudio();

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Management")
    void CleanGeneratedAudio();

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Management")
    bool ExportAudioLibrary(const FString& ExportPath);

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Management")
    bool ImportAudioLibrary(const FString& ImportPath);

    // 音頻質量控制
    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Quality")
    FString GenerateHighQualityMusic(const FString& Prompt, int32 Duration = 60);

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Quality")
    FString GenerateCinematicMusic(const FString& SceneDescription);

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Quality")
    FString OptimizeAudioFile(const FString& AudioPath);

    // 音頻分析和處理
    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Analysis")
    TMap<FString, float> AnalyzeAudioCharacteristics(const FString& AudioPath);

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Analysis")
    FString GenerateAudioMetadata(const FString& AudioPath);

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Analysis")
    bool ValidateAudioQuality(const FString& AudioPath);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnMusicGenerated, const FString&, OutputPath, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSoundEffectGenerated, const FString&, OutputPath, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAudioGenerationCompleted, const FString&, Summary);

    UPROPERTY(BlueprintAssignable, Category = "Audio Generation|Events")
    FOnMusicGenerated OnMusicGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Audio Generation|Events")
    FOnSoundEffectGenerated OnSoundEffectGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Audio Generation|Events")
    FOnAudioGenerationCompleted OnAudioGenerationCompleted;

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Status")
    bool IsInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Status")
    FString GetSystemStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Audio Generation|Status")
    TArray<FString> GetAvailableFunctions() const;

private:
    bool bIsInitialized;
    
    // 音樂預設數據
    TMap<FString, TSharedPtr<FJsonObject>> MusicPresets;
    
    // 音效模板
    TMap<FString, TArray<FString>> SoundEffectTemplates;
    
    // 音頻生成配置
    TSharedPtr<FJsonObject> AudioConfig;

    // 內部輔助函數
    bool LoadMusicPresets();
    bool LoadSoundEffectTemplates();
    bool LoadAudioConfiguration();
    
    FString GeneratePlaceholderMusic(int32 Duration, const FString& OutputName, const FString& Style);
    FString GeneratePlaceholderSoundEffect(const FString& Description, const FString& OutputName, const FString& Category);
    
    FString GetAudioOutputDirectory() const;
    bool EnsureAudioDirectoryExists() const;
    FString GenerateUniqueAudioID(const FString& Prefix) const;
    
    // 音頻處理輔助函數
    bool ProcessAudioFile(const FString& InputPath, const FString& OutputPath);
    TSharedPtr<FJsonObject> LoadJsonFile(const FString& FilePath) const;
    bool SaveJsonFile(const FString& FilePath, TSharedPtr<FJsonObject> JsonObject) const;
};

// 音頻質量級別枚舉
UENUM(BlueprintType)
enum class EAudioQuality : uint8
{
    Draft      UMETA(DisplayName = "Draft"),
    Standard   UMETA(DisplayName = "Standard"),
    High       UMETA(DisplayName = "High"),
    Ultra      UMETA(DisplayName = "Ultra"),
    Cinematic  UMETA(DisplayName = "Cinematic")
};

// 音樂類型枚舉
UENUM(BlueprintType)
enum class EMusicType : uint8
{
    Background    UMETA(DisplayName = "Background"),
    Battle        UMETA(DisplayName = "Battle"),
    Menu          UMETA(DisplayName = "Menu"),
    Victory       UMETA(DisplayName = "Victory"),
    Building      UMETA(DisplayName = "Building"),
    Exploration   UMETA(DisplayName = "Exploration"),
    Cinematic     UMETA(DisplayName = "Cinematic"),
    Custom        UMETA(DisplayName = "Custom")
};

// 音效類型枚舉
UENUM(BlueprintType)
enum class ESoundEffectType : uint8
{
    Combat        UMETA(DisplayName = "Combat"),
    Environment   UMETA(DisplayName = "Environment"),
    UI            UMETA(DisplayName = "UI"),
    Interface     UMETA(DisplayName = "Interface"),
    Ambient       UMETA(DisplayName = "Ambient"),
    Action        UMETA(DisplayName = "Action"),
    Magic         UMETA(DisplayName = "Magic"),
    Custom        UMETA(DisplayName = "Custom")
};
