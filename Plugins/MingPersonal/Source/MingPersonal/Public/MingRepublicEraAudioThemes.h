#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRepublicEraAudioThemes.generated.h"

UENUM(BlueprintType)
enum class ERepublicEraTheme : uint8
{
    BattleTheme,        // 戰爭主題
    PeacefulTheme,      // 和平主題
    TensionTheme,       // 緊張主題
    VictoryTheme,       // 勝利主題
    DefeatTheme,        // 失敗主題
    DiplomaticTheme,    // 外交主題
    EconomicTheme,      // 經濟主題
    CulturalTheme,      // 文化主題
    RevolutionaryTheme, // 革命主題
    NostalgicTheme      // 懷舊主題
};

UENUM(BlueprintType)
enum class ERepublicEraRegion : uint8
{
    Beijing,            // 北京
    Shanghai,           // 上海
    Nanjing,            // 南京
    Guangzhou,          // 廣州
    Tianjin,            // 天津
    Wuhan,              // 武漢
    Chongqing,          // 重慶
    Xian,               // 西安
    Chengdu,            // 成都
    Harbin              // 哈爾濱
};

USTRUCT(BlueprintType)
struct FRepublicEraAudioTheme
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    ERepublicEraTheme ThemeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    FString ThemeName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    FString AudioAssetPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    float BaseIntensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    float Duration = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    bool bIsLooping = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    TArray<FString> InstrumentLayers;

    FRepublicEraAudioTheme()
    {
        ThemeType = ERepublicEraTheme::PeacefulTheme;
        ThemeName = TEXT("");
        AudioAssetPath = TEXT("");
        BaseIntensity = 1.0f;
        Duration = 60.0f;
        bIsLooping = true;
    }
};

USTRUCT(BlueprintType)
struct FRepublicEraRegionalAudio
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    ERepublicEraRegion Region;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    FString RegionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    FString AmbientAudioPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    FString CulturalAudioPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    TArray<FString> RegionalInstruments;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Republic Era Audio")
    float CulturalIntensity = 1.0f;

    FRepublicEraRegionalAudio()
    {
        Region = ERepublicEraRegion::Beijing;
        RegionName = TEXT("");
        AmbientAudioPath = TEXT("");
        CulturalAudioPath = TEXT("");
        CulturalIntensity = 1.0f;
    }
};

/**
 * 民國時期音頻主題管理器
 * 管理民國時期的音頻主題和地區音效
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRepublicEraAudioThemes : public UObject
{
    GENERATED_BODY()

public:
    UMingRepublicEraAudioThemes();

    // 初始化民國時期音頻主題
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void InitializeRepublicEraThemes();

    // 播放主題音樂
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayTheme(ERepublicEraTheme ThemeType, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayThemeByName(const FString& ThemeName, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void StopTheme();

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void FadeTheme(float TargetVolume, float FadeDuration);

    // 地區音效
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayRegionalAmbient(ERepublicEraRegion Region, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayCulturalAudio(ERepublicEraRegion Region, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetRegionalAudio(ERepublicEraRegion Region);

    // 主題管理
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void AddTheme(const FRepublicEraAudioTheme& Theme);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void RemoveTheme(ERepublicEraTheme ThemeType);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Republic Era Audio")
    FRepublicEraAudioTheme GetTheme(ERepublicEraTheme ThemeType) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Republic Era Audio")
    TArray<FRepublicEraAudioTheme> GetAllThemes() const;

    // 地區管理
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void AddRegionalAudio(const FRepublicEraRegionalAudio& RegionalAudio);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Republic Era Audio")
    FRepublicEraRegionalAudio GetRegionalAudio(ERepublicEraRegion Region) const;

    // 音頻設置
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetMasterVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetThemeVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetRegionalVolume(float Volume);

    // 狀態查詢
    UFUNCTION(BlueprintPure, Category = "Republic Era Audio")
    bool IsThemePlaying() const;

    UFUNCTION(BlueprintPure, Category = "Republic Era Audio")
    ERepublicEraTheme GetCurrentTheme() const { return CurrentTheme; }

    UFUNCTION(BlueprintPure, Category = "Republic Era Audio")
    float GetMasterVolume() const { return MasterVolume; }

protected:
    // 主題數據
    UPROPERTY()
    TMap<ERepublicEraTheme, FRepublicEraAudioTheme> ThemeMap;

    UPROPERTY()
    TMap<FString, FRepublicEraAudioTheme> ThemeNameMap;

    // 地區數據
    UPROPERTY()
    TMap<ERepublicEraRegion, FRepublicEraRegionalAudio> RegionalAudioMap;

    // 音頻設置
    UPROPERTY()
    float MasterVolume = 1.0f;

    UPROPERTY()
    float ThemeVolume = 1.0f;

    UPROPERTY()
    float RegionalVolume = 1.0f;

    // 狀態
    UPROPERTY()
    ERepublicEraTheme CurrentTheme = ERepublicEraTheme::PeacefulTheme;

    UPROPERTY()
    bool bIsThemePlaying = false;

    // 內部函數
    void InitializeDefaultThemes();
    void InitializeRegionalAudio();
    FString GetThemeAssetPath(ERepublicEraTheme ThemeType) const;
    FString GetRegionalAssetPath(ERepublicEraRegion Region, bool bCultural = false) const;

private:
    // 民國時期特色樂器
    TArray<FString> TraditionalInstruments;
    TArray<FString> MilitaryInstruments;
    TArray<FString> UrbanInstruments;

    // 初始化樂器列表
    void InitializeInstrumentLists();
};
