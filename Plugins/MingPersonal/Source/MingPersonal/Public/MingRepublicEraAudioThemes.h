#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRepublicEraAudioThemes.generated.h"

UENUM(BlueprintType)
enum class ERepublicEraTheme : uint8
{
    BattleTheme,        // ?∞Áà≠‰∏ªÈ?
    PeacefulTheme,      // ?åÂπ≥‰∏ªÈ?
    TensionTheme,       // Á∑äÂºµ‰∏ªÈ?
    VictoryTheme,       // ?ùÂà©‰∏ªÈ?
    DefeatTheme,        // Â§±Ê?‰∏ªÈ?
    DiplomaticTheme,    // Â§ñ‰∫§‰∏ªÈ?
    EconomicTheme,      // Á∂ìÊ?‰∏ªÈ?
    CulturalTheme,      // ?áÂ?‰∏ªÈ?
    RevolutionaryTheme, // ?©ÂëΩ‰∏ªÈ?
    NostalgicTheme      // ?∑Ë?‰∏ªÈ?
};

UENUM(BlueprintType)
enum class ERepublicEraRegion : uint8
{
    Beijing,            // ?ó‰∫¨
    Shanghai,           // ‰∏äÊµ∑
    Nanjing,            // ?ó‰∫¨
    Guangzhou,          // Âª??
    Tianjin,            // Â§©Ê¥•
    Wuhan,              // Ê≠¶Êº¢
    Chongqing,          // ?çÊÖ∂
    Xian,               // Ë•øÂ?
    Chengdu,            // ?êÈÉΩ
    Harbin              // ?àÁàæÊø?};

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
 * Ê∞ëÂ??ÇÊ??≥È†ª‰∏ªÈ?ÁÆ°Á??? * ÁÆ°Á?Ê∞ëÂ??ÇÊ??ÑÈü≥?ª‰∏ªÈ°åÂ??∞Â??≥Ê?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRepublicEraAudioThemes : public UObject
{
    GENERATED_BODY()

public:
    UMingRepublicEraAudioThemes();

    // ?ùÂ??ñÊ??ãÊ??üÈü≥?ª‰∏ªÈ°?    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void InitializeRepublicEraThemes();

    // ?≠Êîæ‰∏ªÈ??≥Ê?
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayTheme(ERepublicEraTheme ThemeType, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayThemeByName(const FString& ThemeName, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void StopTheme();

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void FadeTheme(float TargetVolume, float FadeDuration);

    // ?∞Â??≥Ê?
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayRegionalAmbient(ERepublicEraRegion Region, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void PlayCulturalAudio(ERepublicEraRegion Region, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetRegionalAudio(ERepublicEraRegion Region);

    // ‰∏ªÈ?ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void AddTheme(const FRepublicEraAudioTheme& Theme);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void RemoveTheme(ERepublicEraTheme ThemeType);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Republic Era Audio")
    FRepublicEraAudioTheme GetTheme(ERepublicEraTheme ThemeType) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Republic Era Audio")
    TArray<FRepublicEraAudioTheme> GetAllThemes() const;

    // ?∞Â?ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void AddRegionalAudio(const FRepublicEraRegionalAudio& RegionalAudio);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Republic Era Audio")
    FRepublicEraRegionalAudio GetRegionalAudio(ERepublicEraRegion Region) const;

    // ?≥È†ªË®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetMasterVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetThemeVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Republic Era Audio")
    void SetRegionalVolume(float Volume);

    // ?Ä?ãÊü•Ë©?    UFUNCTION(BlueprintPure, Category = "Republic Era Audio")
    bool IsThemePlaying() const;

    UFUNCTION(BlueprintPure, Category = "Republic Era Audio")
    ERepublicEraTheme GetCurrentTheme() const { return CurrentTheme; }

    UFUNCTION(BlueprintPure, Category = "Republic Era Audio")
    float GetMasterVolume() const { return MasterVolume; }

protected:
    // ‰∏ªÈ??∏Ê?
    UPROPERTY()
    TMap<ERepublicEraTheme, FRepublicEraAudioTheme> ThemeMap;

    UPROPERTY()
    TMap<FString, FRepublicEraAudioTheme> ThemeNameMap;

    // ?∞Â??∏Ê?
    UPROPERTY()
    TMap<ERepublicEraRegion, FRepublicEraRegionalAudio> RegionalAudioMap;

    // ?≥È†ªË®≠ÁΩÆ
    UPROPERTY()
    float MasterVolume = 1.0f;

    UPROPERTY()
    float ThemeVolume = 1.0f;

    UPROPERTY()
    float RegionalVolume = 1.0f;

    // ?Ä??    UPROPERTY()
    ERepublicEraTheme CurrentTheme = ERepublicEraTheme::PeacefulTheme;

    UPROPERTY()
    bool bIsThemePlaying = false;

    // ?ßÈÉ®?ΩÊï∏
    void InitializeDefaultThemes();
    void InitializeRegionalAudio();
    FString GetThemeAssetPath(ERepublicEraTheme ThemeType) const;
    FString GetRegionalAssetPath(ERepublicEraRegion Region, bool bCultural = false) const;

private:
    // Ê∞ëÂ??ÇÊ??πËâ≤Ê®ÇÂô®
    TArray<FString> TraditionalInstruments;
    TArray<FString> MilitaryInstruments;
    TArray<FString> UrbanInstruments;

    // ?ùÂ??ñÊ??®Â?Ë°?    void InitializeInstrumentLists();
};
