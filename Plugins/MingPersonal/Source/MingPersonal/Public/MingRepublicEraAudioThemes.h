#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRepublicEraAudioThemes.generated.h"

UENUM(BlueprintType)
enum class ERepublicEraTheme : uint8
{
    BattleTheme,        // ??主?
    PeacefulTheme,      // ?平主?
    TensionTheme,       // 緊張?
    VictoryTheme,       // ??主?
    DefeatTheme,        // ??
    DiplomaticTheme,    // 外交?
    EconomicTheme,      // ??
    CulturalTheme,      // ???
    RevolutionaryTheme, // ??主?
    NostalgicTheme      // ???
};

UENUM(BlueprintType)
enum class EPersonalRepublicEraRegion : uint8
{
    Beijing,            // ??
    Shanghai,           // 上海
    Nanjing,            // ??
    Guangzhou,          // ??
    Tianjin,            // 天津
    Wuhan,              // 武漢
    Chongqing,          // ???
    Xian,               // ?
    Chengdu,            // ???
    Harbin              // ???};

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
        ThemeName = TEXT(""};
        AudioAssetPath = TEXT(""};
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
    EPersonalRepublicEraRegion Region;

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
        Region = EPersonalRepublicEraRegion::Beijing;
        RegionName = TEXT(""};
        AmbientAudioPath = TEXT(""};
        CulturalAudioPath = TEXT(""};
        CulturalIntensity = 1.0f;
    }
};

/**
 * ?????主???? * ???????主??????
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRepublicEraAudioThemes : public UObject
{
    GENERATED_BODY()

public:
    UMingRepublicEraAudioThemes(};

    // ??????????    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void InitializeRepublicEraThemes(};

    // ??主???
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayTheme(ERepublicEraTheme ThemeType, float Intensity = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayThemeByName(const FString& ThemeName, float Intensity = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void StopTheme(};

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void FadeTheme(float TargetVolume, float FadeDuration};

    // ????
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayRegionalAmbient(EPersonalRepublicEraRegion Region, float Intensity = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayCulturalAudio(EPersonalRepublicEraRegion Region, float Intensity = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetRegionalAudio(EPersonalRepublicEraRegion Region};

    // ??
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void AddTheme(const FRepublicEraAudioTheme& Theme};

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void RemoveTheme(ERepublicEraTheme ThemeType};

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Republic Era Audio")
    FRepublicEraAudioTheme GetTheme(ERepublicEraTheme ThemeType) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Republic Era Audio")
    TArray<FRepublicEraAudioTheme> GetAllThemes() const;

    // ???
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void AddRegionalAudio(const FRepublicEraRegionalAudio& RegionalAudio};

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Republic Era Audio")
    FRepublicEraRegionalAudio GetRegionalAudio(EPersonalRepublicEraRegion Region) const;

    // ??設?
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetMasterVolume(float Volume};

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetThemeVolume(float Volume};

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetRegionalVolume(float Volume};

    // ????    UFUNCTION(BlueprintPure, Category = "Republic Era Audio")
    bool IsThemePlaying() const;

    UFUNCTION(BlueprintPure, Category = "Republic Era Audio")
    ERepublicEraTheme GetCurrentTheme() const { return CurrentTheme; }

    UFUNCTION(BlueprintPure, Category = "Republic Era Audio")
    float GetMasterVolume() const { return MasterVolume; }

protected:
    // ???
    UPROPERTY()
    TMap<ERepublicEraTheme, FRepublicEraAudioTheme> ThemeMap;

    UPROPERTY()
    TMap<FString, FRepublicEraAudioTheme> ThemeNameMap;

    // ????
    UPROPERTY()
    TMap<EPersonalRepublicEraRegion, FRepublicEraRegionalAudio> RegionalAudioMap;

    // ??設?
    UPROPERTY()
    float MasterVolume = 1.0f;

    UPROPERTY()
    float ThemeVolume = 1.0f;

    UPROPERTY()
    float RegionalVolume = 1.0f;

    // ???    UPROPERTY()
    ERepublicEraTheme CurrentTheme = ERepublicEraTheme::PeacefulTheme;

    UPROPERTY()
    bool bIsThemePlaying = false;

    // ?????
    void InitializeDefaultThemes(};
    void InitializeRegionalAudio(};
    FString GetThemeAssetPath(ERepublicEraTheme ThemeType) const;
    FString GetRegionalAssetPath(EPersonalRepublicEraRegion Region, bool bCultural = false) const;

private:
    // ????????
    TArray<FString> TraditionalInstruments;
    TArray<FString> MilitaryInstruments;
    TArray<FString> UrbanInstruments;

    // ???????    void InitializeInstrumentLists(};
};

