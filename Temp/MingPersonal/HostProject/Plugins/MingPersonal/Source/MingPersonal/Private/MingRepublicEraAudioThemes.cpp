#include "MingRepublicEraAudioThemes.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UMingRepublicEraAudioThemes::UMingRepublicEraAudioThemes()
    : MasterVolume(1.0f)
    , ThemeVolume(1.0f)
    , RegionalVolume(1.0f)
    , CurrentTheme(ERepublicEraTheme::PeacefulTheme)
    , bIsThemePlaying(false)
{
}

void UMingRepublicEraAudioThemes::InitializeRepublicEraThemes()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Republic Era Audio Themes"));
    
    InitializeInstrumentLists();
    InitializeDefaultThemes();
    InitializeRegionalAudio();
    
    UE_LOG(LogTemp, Log, TEXT("Republic Era Audio Themes initialized successfully"));
}

void UMingRepublicEraAudioThemes::PlayTheme(ERepublicEraTheme ThemeType, float Intensity)
{
    if (ThemeMap.Contains(ThemeType))
    {
        const FRepublicEraAudioTheme& Theme = ThemeMap[ThemeType];
        PlayThemeByName(Theme.ThemeName, Intensity);
        
        CurrentTheme = ThemeType;
        bIsThemePlaying = true;
        
        UE_LOG(LogTemp, Log, TEXT("Playing Republic Era theme: %s (Intensity: %.2f)"), *Theme.ThemeName, Intensity);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Theme not found: %d"), (int32)ThemeType);
    }
}

void UMingRepublicEraAudioThemes::PlayThemeByName(const FString& ThemeName, float Intensity)
{
    if (ThemeNameMap.Contains(ThemeName))
    {
        const FRepublicEraAudioTheme& Theme = ThemeNameMap[ThemeName];
        
        // 這裡應該播放實際的音頻資產
        // 暫時使用日誌記錄
        UE_LOG(LogTemp, Log, TEXT("Playing theme: %s (Asset: %s)"), *ThemeName, *Theme.AudioAssetPath);
        
        // 實際實現時，這裡會調用音頻系統播放音效
        // UGameplayStatics::PlaySound2D(GetWorld(), LoadObject<USoundBase>(nullptr, *Theme.AudioAssetPath), ThemeVolume * MasterVolume * Intensity);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Theme name not found: %s"), *ThemeName);
    }
}

void UMingRepublicEraAudioThemes::StopTheme()
{
    if (bIsThemePlaying)
    {
        // 停止當前播放的主題
        bIsThemePlaying = false;
        UE_LOG(LogTemp, Log, TEXT("Stopped Republic Era theme"));
    }
}

void UMingRepublicEraAudioThemes::FadeTheme(float TargetVolume, float FadeDuration)
{
    if (bIsThemePlaying)
    {
        // 實現淡入淡出效果
        UE_LOG(LogTemp, Log, TEXT("Fading theme to volume %.2f over %.2f seconds"), TargetVolume, FadeDuration);
        
        // 實際實現時會使用音頻組件的淡入淡出功能
    }
}

void UMingRepublicEraAudioThemes::PlayRegionalAmbient(ERepublicEraRegion Region, float Intensity)
{
    if (RegionalAudioMap.Contains(Region))
    {
        const FRepublicEraRegionalAudio& RegionalAudio = RegionalAudioMap[Region];
        
        UE_LOG(LogTemp, Log, TEXT("Playing regional ambient for %s (Intensity: %.2f)"), *RegionalAudio.RegionName, Intensity);
        
        // 實際實現時播放地區環境音效
        // UGameplayStatics::PlaySound2D(GetWorld(), LoadObject<USoundBase>(nullptr, *RegionalAudio.AmbientAudioPath), RegionalVolume * MasterVolume * Intensity);
    }
}

void UMingRepublicEraAudioThemes::PlayCulturalAudio(ERepublicEraRegion Region, float Intensity)
{
    if (RegionalAudioMap.Contains(Region))
    {
        const FRepublicEraRegionalAudio& RegionalAudio = RegionalAudioMap[Region];
        
        UE_LOG(LogTemp, Log, TEXT("Playing cultural audio for %s (Intensity: %.2f)"), *RegionalAudio.RegionName, Intensity);
        
        // 實際實現時播放文化音效
        // UGameplayStatics::PlaySound2D(GetWorld(), LoadObject<USoundBase>(nullptr, *RegionalAudio.CulturalAudioPath), RegionalVolume * MasterVolume * Intensity);
    }
}

void UMingRepublicEraAudioThemes::SetRegionalAudio(ERepublicEraRegion Region)
{
    // 設置當前地區的音頻環境
    PlayRegionalAmbient(Region, 1.0f);
    PlayCulturalAudio(Region, 0.7f);
    
    UE_LOG(LogTemp, Log, TEXT("Set regional audio for: %d"), (int32)Region);
}

void UMingRepublicEraAudioThemes::AddTheme(const FRepublicEraAudioTheme& Theme)
{
    ThemeMap.Add(Theme.ThemeType, Theme);
    ThemeNameMap.Add(Theme.ThemeName, Theme);
    
    UE_LOG(LogTemp, Log, TEXT("Added theme: %s"), *Theme.ThemeName);
}

void UMingRepublicEraAudioThemes::RemoveTheme(ERepublicEraTheme ThemeType)
{
    if (ThemeMap.Contains(ThemeType))
    {
        FString ThemeName = ThemeMap[ThemeType].ThemeName;
        ThemeMap.Remove(ThemeType);
        ThemeNameMap.Remove(ThemeName);
        
        UE_LOG(LogTemp, Log, TEXT("Removed theme: %s"), *ThemeName);
    }
}

FRepublicEraAudioTheme UMingRepublicEraAudioThemes::GetTheme(ERepublicEraTheme ThemeType) const
{
    if (ThemeMap.Contains(ThemeType))
    {
        return ThemeMap[ThemeType];
    }
    return FRepublicEraAudioTheme();
}

TArray<FRepublicEraAudioTheme> UMingRepublicEraAudioThemes::GetAllThemes() const
{
    TArray<FRepublicEraAudioTheme> Themes;
    for (const auto& ThemePair : ThemeMap)
    {
        Themes.Add(ThemePair.Value);
    }
    return Themes;
}

void UMingRepublicEraAudioThemes::AddRegionalAudio(const FRepublicEraRegionalAudio& RegionalAudio)
{
    RegionalAudioMap.Add(RegionalAudio.Region, RegionalAudio);
    
    UE_LOG(LogTemp, Log, TEXT("Added regional audio for: %s"), *RegionalAudio.RegionName);
}

FRepublicEraRegionalAudio UMingRepublicEraAudioThemes::GetRegionalAudio(ERepublicEraRegion Region) const
{
    if (RegionalAudioMap.Contains(Region))
    {
        return RegionalAudioMap[Region];
    }
    return FRepublicEraRegionalAudio();
}

void UMingRepublicEraAudioThemes::SetMasterVolume(float Volume)
{
    MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    UE_LOG(LogTemp, Log, TEXT("Set master volume to: %.2f"), MasterVolume);
}

void UMingRepublicEraAudioThemes::SetThemeVolume(float Volume)
{
    ThemeVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    UE_LOG(LogTemp, Log, TEXT("Set theme volume to: %.2f"), ThemeVolume);
}

void UMingRepublicEraAudioThemes::SetRegionalVolume(float Volume)
{
    RegionalVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    UE_LOG(LogTemp, Log, TEXT("Set regional volume to: %.2f"), RegionalVolume);
}

bool UMingRepublicEraAudioThemes::IsThemePlaying() const
{
    return bIsThemePlaying;
}

void UMingRepublicEraAudioThemes::InitializeDefaultThemes()
{
    // 戰爭主題
    FRepublicEraAudioTheme BattleTheme;
    BattleTheme.ThemeType = ERepublicEraTheme::BattleTheme;
    BattleTheme.ThemeName = TEXT("Republic_Battle");
    BattleTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/BattleTheme.BattleTheme");
    BattleTheme.BaseIntensity = 1.5f;
    BattleTheme.Duration = 120.0f;
    BattleTheme.bIsLooping = true;
    BattleTheme.InstrumentLayers.Add(TEXT("Military_Drums"));
    BattleTheme.InstrumentLayers.Add(TEXT("Trumpet"));
    BattleTheme.InstrumentLayers.Add(TEXT("Violin"));
    AddTheme(BattleTheme);

    // 和平主題
    FRepublicEraAudioTheme PeacefulTheme;
    PeacefulTheme.ThemeType = ERepublicEraTheme::PeacefulTheme;
    PeacefulTheme.ThemeName = TEXT("Republic_Peaceful");
    PeacefulTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/PeacefulTheme.PeacefulTheme");
    PeacefulTheme.BaseIntensity = 0.8f;
    PeacefulTheme.Duration = 180.0f;
    PeacefulTheme.bIsLooping = true;
    PeacefulTheme.InstrumentLayers.Add(TEXT("Pipa"));
    PeacefulTheme.InstrumentLayers.Add(TEXT("Guzheng"));
    PeacefulTheme.InstrumentLayers.Add(TEXT("Dizi"));
    AddTheme(PeacefulTheme);

    // 緊張主題
    FRepublicEraAudioTheme TensionTheme;
    TensionTheme.ThemeType = ERepublicEraTheme::TensionTheme;
    TensionTheme.ThemeName = TEXT("Republic_Tension");
    TensionTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/TensionTheme.TensionTheme");
    TensionTheme.BaseIntensity = 1.2f;
    TensionTheme.Duration = 90.0f;
    TensionTheme.bIsLooping = true;
    TensionTheme.InstrumentLayers.Add(TEXT("Low_Strings"));
    TensionTheme.InstrumentLayers.Add(TEXT("Percussion"));
    AddTheme(TensionTheme);

    // 勝利主題
    FRepublicEraAudioTheme VictoryTheme;
    VictoryTheme.ThemeType = ERepublicEraTheme::VictoryTheme;
    VictoryTheme.ThemeName = TEXT("Republic_Victory");
    VictoryTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/VictoryTheme.VictoryTheme");
    VictoryTheme.BaseIntensity = 1.3f;
    VictoryTheme.Duration = 60.0f;
    VictoryTheme.bIsLooping = false;
    VictoryTheme.InstrumentLayers.Add(TEXT("Brass"));
    VictoryTheme.InstrumentLayers.Add(TEXT("Drums"));
    VictoryTheme.InstrumentLayers.Add(TEXT("Cymbals"));
    AddTheme(VictoryTheme);

    // 失敗主題
    FRepublicEraAudioTheme DefeatTheme;
    DefeatTheme.ThemeType = ERepublicEraTheme::DefeatTheme;
    DefeatTheme.ThemeName = TEXT("Republic_Defeat");
    DefeatTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/DefeatTheme.DefeatTheme");
    DefeatTheme.BaseIntensity = 0.7f;
    DefeatTheme.Duration = 45.0f;
    DefeatTheme.bIsLooping = false;
    DefeatTheme.InstrumentLayers.Add(TEXT("Somber_Strings"));
    DefeatTheme.InstrumentLayers.Add(TEXT("Low_Brass"));
    AddTheme(DefeatTheme);

    // 外交主題
    FRepublicEraAudioTheme DiplomaticTheme;
    DiplomaticTheme.ThemeType = ERepublicEraTheme::DiplomaticTheme;
    DiplomaticTheme.ThemeName = TEXT("Republic_Diplomatic");
    DiplomaticTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/DiplomaticTheme.DiplomaticTheme");
    DiplomaticTheme.BaseIntensity = 1.0f;
    DiplomaticTheme.Duration = 150.0f;
    DiplomaticTheme.bIsLooping = true;
    DiplomaticTheme.InstrumentLayers.Add(TEXT("Piano"));
    DiplomaticTheme.InstrumentLayers.Add(TEXT("Strings"));
    AddTheme(DiplomaticTheme);

    // 經濟主題
    FRepublicEraAudioTheme EconomicTheme;
    EconomicTheme.ThemeType = ERepublicEraTheme::EconomicTheme;
    EconomicTheme.ThemeName = TEXT("Republic_Economic");
    EconomicTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/EconomicTheme.EconomicTheme");
    EconomicTheme.BaseIntensity = 0.9f;
    EconomicTheme.Duration = 120.0f;
    EconomicTheme.bIsLooping = true;
    EconomicTheme.InstrumentLayers.Add(TEXT("Light_Percussion"));
    EconomicTheme.InstrumentLayers.Add(TEXT("Woodwinds"));
    AddTheme(EconomicTheme);

    // 文化主題
    FRepublicEraAudioTheme CulturalTheme;
    CulturalTheme.ThemeType = ERepublicEraTheme::CulturalTheme;
    CulturalTheme.ThemeName = TEXT("Republic_Cultural");
    CulturalTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/CulturalTheme.CulturalTheme");
    CulturalTheme.BaseIntensity = 1.0f;
    CulturalTheme.Duration = 180.0f;
    CulturalTheme.bIsLooping = true;
    CulturalTheme.InstrumentLayers.Add(TEXT("Erhu"));
    CulturalTheme.InstrumentLayers.Add(TEXT("Pipa"));
    CulturalTheme.InstrumentLayers.Add(TEXT("Flute"));
    AddTheme(CulturalTheme);

    // 革命主題
    FRepublicEraAudioTheme RevolutionaryTheme;
    RevolutionaryTheme.ThemeType = ERepublicEraTheme::RevolutionaryTheme;
    RevolutionaryTheme.ThemeName = TEXT("Republic_Revolutionary");
    RevolutionaryTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/RevolutionaryTheme.RevolutionaryTheme");
    RevolutionaryTheme.BaseIntensity = 1.4f;
    RevolutionaryTheme.Duration = 100.0f;
    RevolutionaryTheme.bIsLooping = true;
    RevolutionaryTheme.InstrumentLayers.Add(TEXT("Drums"));
    RevolutionaryTheme.InstrumentLayers.Add(TEXT("Trumpet"));
    RevolutionaryTheme.InstrumentLayers.Add(TEXT("Choir"));
    AddTheme(RevolutionaryTheme);

    // 懷舊主題
    FRepublicEraAudioTheme NostalgicTheme;
    NostalgicTheme.ThemeType = ERepublicEraTheme::NostalgicTheme;
    NostalgicTheme.ThemeName = TEXT("Republic_Nostalgic");
    NostalgicTheme.AudioAssetPath = TEXT("/Game/Audio/RepublicEra/NostalgicTheme.NostalgicTheme");
    NostalgicTheme.BaseIntensity = 0.8f;
    NostalgicTheme.Duration = 200.0f;
    NostalgicTheme.bIsLooping = true;
    NostalgicTheme.InstrumentLayers.Add(TEXT("Piano"));
    NostalgicTheme.InstrumentLayers.Add(TEXT("Violin"));
    NostalgicTheme.InstrumentLayers.Add(TEXT("Cello"));
    AddTheme(NostalgicTheme);
}

void UMingRepublicEraAudioThemes::InitializeRegionalAudio()
{
    // 北京
    FRepublicEraRegionalAudio BeijingAudio;
    BeijingAudio.Region = ERepublicEraRegion::Beijing;
    BeijingAudio.RegionName = TEXT("北京");
    BeijingAudio.AmbientAudioPath = TEXT("/Game/Audio/RepublicEra/Regions/Beijing_Ambient.Beijing_Ambient");
    BeijingAudio.CulturalAudioPath = TEXT("/Game/Audio/RepublicEra/Regions/Beijing_Cultural.Beijing_Cultural");
    BeijingAudio.RegionalInstruments.Add(TEXT("Peking_Opera"));
    BeijingAudio.RegionalInstruments.Add(TEXT("Traditional_Drums"));
    BeijingAudio.CulturalIntensity = 1.0f;
    AddRegionalAudio(BeijingAudio);

    // 上海
    FRepublicEraRegionalAudio ShanghaiAudio;
    ShanghaiAudio.Region = ERepublicEraRegion::Shanghai;
    ShanghaiAudio.RegionName = TEXT("上海");
    ShanghaiAudio.AmbientAudioPath = TEXT("/Game/Audio/RepublicEra/Regions/Shanghai_Ambient.Shanghai_Ambient");
    ShanghaiAudio.CulturalAudioPath = TEXT("/Game/Audio/RepublicEra/Regions/Shanghai_Cultural.Shanghai_Cultural");
    ShanghaiAudio.RegionalInstruments.Add(TEXT("Jazz_Piano"));
    ShanghaiAudio.RegionalInstruments.Add(TEXT("Saxophone"));
    ShanghaiAudio.CulturalIntensity = 1.2f;
    AddRegionalAudio(ShanghaiAudio);

    // 南京
    FRepublicEraRegionalAudio NanjingAudio;
    NanjingAudio.Region = ERepublicEraRegion::Nanjing;
    NanjingAudio.RegionName = TEXT("南京");
    NanjingAudio.AmbientAudioPath = TEXT("/Game/Audio/RepublicEra/Regions/Nanjing_Ambient.Nanjing_Ambient");
    NanjingAudio.CulturalAudioPath = TEXT("/Game/Audio/RepublicEra/Regions/Nanjing_Cultural.Nanjing_Cultural");
    NanjingAudio.RegionalInstruments.Add(TEXT("Classical_Guitar"));
    NanjingAudio.RegionalInstruments.Add(TEXT("Flute"));
    NanjingAudio.CulturalIntensity = 0.9f;
    AddRegionalAudio(NanjingAudio);

    // 廣州
    FRepublicEraRegionalAudio GuangzhouAudio;
    GuangzhouAudio.Region = ERepublicEraRegion::Guangzhou;
    GuangzhouAudio.RegionName = TEXT("廣州");
    GuangzhouAudio.AmbientAudioPath = TEXT("/Game/Audio/RepublicEra/Regions/Guangzhou_Ambient.Guangzhou_Ambient");
    GuangzhouAudio.CulturalAudioPath = TEXT("/Game/Audio/RepublicEra/Regions/Guangzhou_Cultural.Guangzhou_Cultural");
    GuangzhouAudio.RegionalInstruments.Add(TEXT("Cantonese_Opera"));
    GuangzhouAudio.RegionalInstruments.Add(TEXT("Guzheng"));
    GuangzhouAudio.CulturalIntensity = 1.1f;
    AddRegionalAudio(GuangzhouAudio);

    // 其他地區可以繼續添加...
}

void UMingRepublicEraAudioThemes::InitializeInstrumentLists()
{
    // 傳統樂器
    TraditionalInstruments.Add(TEXT("Pipa"));
    TraditionalInstruments.Add(TEXT("Guzheng"));
    TraditionalInstruments.Add(TEXT("Erhu"));
    TraditionalInstruments.Add(TEXT("Dizi"));
    TraditionalInstruments.Add(TEXT("Xiao"));
    TraditionalInstruments.Add(TEXT("Gong"));
    TraditionalInstruments.Add(TEXT("Cymbals"));

    // 軍事樂器
    MilitaryInstruments.Add(TEXT("Military_Drums"));
    MilitaryInstruments.Add(TEXT("Trumpet"));
    MilitaryInstruments.Add(TEXT("Bugle"));
    MilitaryInstruments.Add(TEXT("Snare_Drum"));
    MilitaryInstruments.Add(TEXT("Bass_Drum"));

    // 都市樂器
    UrbanInstruments.Add(TEXT("Piano"));
    UrbanInstruments.Add(TEXT("Violin"));
    UrbanInstruments.Add(TEXT("Cello"));
    UrbanInstruments.Add(TEXT("Jazz_Piano"));
    UrbanInstruments.Add(TEXT("Saxophone"));
    UrbanInstruments.Add(TEXT("Clarinet"));
}

FString UMingRepublicEraAudioThemes::GetThemeAssetPath(ERepublicEraTheme ThemeType) const
{
    if (ThemeMap.Contains(ThemeType))
    {
        return ThemeMap[ThemeType].AudioAssetPath;
    }
    return TEXT("");
}

FString UMingRepublicEraAudioThemes::GetRegionalAssetPath(ERepublicEraRegion Region, bool bCultural) const
{
    if (RegionalAudioMap.Contains(Region))
    {
        const FRepublicEraRegionalAudio& RegionalAudio = RegionalAudioMap[Region];
        return bCultural ? RegionalAudio.CulturalAudioPath : RegionalAudio.AmbientAudioPath;
    }
    return TEXT("");
}
