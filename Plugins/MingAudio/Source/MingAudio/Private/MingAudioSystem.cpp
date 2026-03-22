#include "MingAudioSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UMingAudioSystem::UMingAudioSystem()
{
    WorldContext = GetWorld();
    CurrentMusicTheme = EMingMusicTheme::MainMenu;
    
    // Initialize default settings
    AudioSettings.MasterVolume = 1.0f;
    AudioSettings.MusicVolume = 0.8f;
    AudioSettings.SFXVolume = 0.9f;
    AudioSettings.VoiceVolume = 1.0f;
    AudioSettings.AmbientVolume = 0.6f;
    AudioSettings.UIVolume = 0.7f;
    
    AudioSettings.bEnableMusic = true;
    AudioSettings.bEnableSFX = true;
    AudioSettings.bEnableVoice = true;
    AudioSettings.bEnableAmbient = true;
    AudioSettings.bEnableUI = true;
}

void UMingAudioSystem::InitializeAudio()
{
    InitializeDefaultTracks();
    ApplyVolumeSettings();
    
    UE_LOG(LogTemp, Log, TEXT("Audio system initialized with %d tracks"), AudioTracks.Num());
}

void UMingAudioSystem::InitializeDefaultTracks()
{
    AudioTracks.Empty();
    
    SetupMusicTracks();
    SetupSFXTracks();
    SetupVoiceTracks();
    SetupAmbientTracks();
    SetupUITracks();
}

void UMingAudioSystem::SetupMusicTracks()
{
    // Main Menu Theme
    FMingAudioTrack MainMenuMusic;
    MainMenuMusic.TrackID = TEXT("MUSIC_MAIN_MENU");
    MainMenuMusic.TrackName = TEXT("主選單音樂");
    MainMenuMusic.AudioType = EMingAudioType::Music;
    MainMenuMusic.MusicTheme = EMingMusicTheme::MainMenu;
    MainMenuMusic.bLoop = true;
    MainMenuMusic.bAutoPlay = true;
    MainMenuMusic.FadeInTime = 2.0f;
    MainMenuMusic.FadeOutTime = 1.5f;
    AudioTracks.Add(MainMenuMusic.TrackID, MainMenuMusic);
    
    // Campaign Map Theme
    FMingAudioTrack CampaignMapMusic;
    CampaignMapMusic.TrackID = TEXT("MUSIC_CAMPAIGN_MAP");
    CampaignMapMusic.TrackName = TEXT("戰役地圖音樂");
    CampaignMapMusic.AudioType = EMingAudioType::Music;
    CampaignMapMusic.MusicTheme = EMingMusicTheme::CampaignMap;
    CampaignMapMusic.bLoop = true;
    CampaignMapMusic.FadeInTime = 1.5f;
    CampaignMapMusic.FadeOutTime = 1.0f;
    AudioTracks.Add(CampaignMapMusic.TrackID, CampaignMapMusic);
    
    // Battle Theme
    FMingAudioTrack BattleMusic;
    BattleMusic.TrackID = TEXT("MUSIC_BATTLE");
    BattleMusic.TrackName = TEXT("戰鬥音樂");
    BattleMusic.AudioType = EMingAudioType::Music;
    BattleMusic.MusicTheme = EMingMusicTheme::Battle;
    BattleMusic.bLoop = true;
    BattleMusic.FadeInTime = 0.5f;
    BattleMusic.FadeOutTime = 0.5f;
    AudioTracks.Add(BattleMusic.TrackID, BattleMusic);
    
    // Victory Theme
    FMingAudioTrack VictoryMusic;
    VictoryMusic.TrackID = TEXT("MUSIC_VICTORY");
    VictoryMusic.TrackName = TEXT("勝利音樂");
    VictoryMusic.AudioType = EMingAudioType::Music;
    VictoryMusic.MusicTheme = EMingMusicTheme::Victory;
    VictoryMusic.bLoop = false;
    VictoryMusic.FadeInTime = 1.0f;
    VictoryMusic.FadeOutTime = 2.0f;
    AudioTracks.Add(VictoryMusic.TrackID, VictoryMusic);
    
    // Defeat Theme
    FMingAudioTrack DefeatMusic;
    DefeatMusic.TrackID = TEXT("MUSIC_DEFEAT");
    DefeatMusic.TrackName = TEXT("失敗音樂");
    DefeatMusic.AudioType = EMingAudioType::Music;
    DefeatMusic.MusicTheme = EMingMusicTheme::Defeat;
    DefeatMusic.bLoop = false;
    DefeatMusic.FadeInTime = 1.0f;
    DefeatMusic.FadeOutTime = 2.0f;
    AudioTracks.Add(DefeatMusic.TrackID, DefeatMusic);
    
    // Historical Theme
    FMingAudioTrack HistoricalMusic;
    HistoricalMusic.TrackID = TEXT("MUSIC_HISTORICAL");
    HistoricalMusic.TrackName = TEXT("歷史音樂");
    HistoricalMusic.AudioType = EMingAudioType::Music;
    HistoricalMusic.MusicTheme = EMingMusicTheme::Historical;
    HistoricalMusic.bLoop = true;
    HistoricalMusic.FadeInTime = 2.0f;
    HistoricalMusic.FadeOutTime = 1.5f;
    AudioTracks.Add(HistoricalMusic.TrackID, HistoricalMusic);
    
    // Republic Theme
    FMingAudioTrack RepublicMusic;
    RepublicMusic.TrackID = TEXT("MUSIC_REPUBLIC");
    RepublicMusic.TrackName = TEXT("共和音樂");
    RepublicMusic.AudioType = EMingAudioType::Music;
    RepublicMusic.MusicTheme = EMingMusicTheme::Republic;
    RepublicMusic.bLoop = true;
    RepublicMusic.FadeInTime = 1.5f;
    RepublicMusic.FadeOutTime = 1.0f;
    AudioTracks.Add(RepublicMusic.TrackID, RepublicMusic);
    
    // Imperial Theme
    FMingAudioTrack ImperialMusic;
    ImperialMusic.TrackID = TEXT("MUSIC_IMPERIAL");
    ImperialMusic.TrackName = TEXT("帝制音樂");
    ImperialMusic.AudioType = EMingAudioType::Music;
    ImperialMusic.MusicTheme = EMingMusicTheme::Imperial;
    ImperialMusic.bLoop = true;
    ImperialMusic.FadeInTime = 2.0f;
    ImperialMusic.FadeOutTime = 1.5f;
    AudioTracks.Add(ImperialMusic.TrackID, ImperialMusic);
    
    // Tense Theme
    FMingAudioTrack TenseMusic;
    TenseMusic.TrackID = TEXT("MUSIC_TENSE");
    TenseMusic.TrackName = TEXT("緊張音樂");
    TenseMusic.AudioType = EMingAudioType::Music;
    TenseMusic.MusicTheme = EMingMusicTheme::Tense;
    TenseMusic.bLoop = true;
    TenseMusic.FadeInTime = 0.3f;
    TenseMusic.FadeOutTime = 0.3f;
    AudioTracks.Add(TenseMusic.TrackID, TenseMusic);
    
    // Peaceful Theme
    FMingAudioTrack PeacefulMusic;
    PeacefulMusic.TrackID = TEXT("MUSIC_PEACEFUL");
    PeacefulMusic.TrackName = TEXT("和平音樂");
    PeacefulMusic.AudioType = EMingAudioType::Music;
    PeacefulMusic.MusicTheme = EMingMusicTheme::Peaceful;
    PeacefulMusic.bLoop = true;
    PeacefulMusic.FadeInTime = 2.0f;
    PeacefulMusic.FadeOutTime = 1.5f;
    AudioTracks.Add(PeacefulMusic.TrackID, PeacefulMusic);
}

void UMingAudioSystem::SetupSFXTracks()
{
    // Combat SFX
    FMingAudioTrack GunshotSFX;
    GunshotSFX.TrackID = TEXT("SFX_GUNSHOT");
    GunshotSFX.TrackName = TEXT("槍聲");
    GunshotSFX.AudioType = EMingAudioType::SFX;
    GunshotSFX.bLoop = false;
    GunshotSFX.FadeInTime = 0.0f;
    GunshotSFX.FadeOutTime = 0.1f;
    AudioTracks.Add(GunshotSFX.TrackID, GunshotSFX);
    
    FMingAudioTrack ExplosionSFX;
    ExplosionSFX.TrackID = TEXT("SFX_EXPLOSION");
    ExplosionSFX.TrackName = TEXT("爆炸聲");
    ExplosionSFX.AudioType = EMingAudioType::SFX;
    ExplosionSFX.bLoop = false;
    ExplosionSFX.FadeInTime = 0.0f;
    ExplosionSFX.FadeOutTime = 0.5f;
    AudioTracks.Add(ExplosionSFX.TrackID, ExplosionSFX);
    
    FMingAudioTrack SwordClashSFX;
    SwordClashSFX.TrackID = TEXT("SFX_SWORD_CLASH");
    SwordClashSFX.TrackName = TEXT("刀劍碰撞");
    SwordClashSFX.AudioType = EMingAudioType::SFX;
    SwordClashSFX.bLoop = false;
    SwordClashSFX.FadeInTime = 0.0f;
    SwordClashSFX.FadeOutTime = 0.2f;
    AudioTracks.Add(SwordClashSFX.TrackID, SwordClashSFX);
    
    // Building SFX
    FMingAudioTrack BuildSFX;
    BuildSFX.TrackID = TEXT("SFX_BUILD");
    BuildSFX.TrackName = TEXT("建造聲");
    BuildSFX.AudioType = EMingAudioType::SFX;
    BuildSFX.bLoop = false;
    BuildSFX.FadeInTime = 0.0f;
    BuildSFX.FadeOutTime = 0.3f;
    AudioTracks.Add(BuildSFX.TrackID, BuildSFX);
    
    FMingAudioTrack DestroySFX;
    DestroySFX.TrackID = TEXT("SFX_DESTROY");
    DestroySFX.TrackName = TEXT("破壞聲");
    DestroySFX.AudioType = EMingAudioType::SFX;
    DestroySFX.bLoop = false;
    DestroySFX.FadeInTime = 0.0f;
    DestroySFX.FadeOutTime = 0.5f;
    AudioTracks.Add(DestroySFX.TrackID, DestroySFX);
    
    // Unit SFX
    FMingAudioTrack UnitSelectSFX;
    UnitSelectSFX.TrackID = TEXT("SFX_UNIT_SELECT");
    UnitSelectSFX.TrackName = TEXT("單位選擇");
    UnitSelectSFX.AudioType = EMingAudioType::SFX;
    UnitSelectSFX.bLoop = false;
    UnitSelectSFX.FadeInTime = 0.0f;
    UnitSelectSFX.FadeOutTime = 0.1f;
    AudioTracks.Add(UnitSelectSFX.TrackID, UnitSelectSFX);
    
    FMingAudioTrack UnitMoveSFX;
    UnitMoveSFX.TrackID = TEXT("SFX_UNIT_MOVE");
    UnitMoveSFX.TrackName = TEXT("單位移動");
    UnitMoveSFX.AudioType = EMingAudioType::SFX;
    UnitMoveSFX.bLoop = false;
    UnitMoveSFX.FadeInTime = 0.0f;
    UnitMoveSFX.FadeOutTime = 0.1f;
    AudioTracks.Add(UnitMoveSFX.TrackID, UnitMoveSFX);
    
    FMingAudioTrack UnitAttackSFX;
    UnitAttackSFX.TrackID = TEXT("SFX_UNIT_ATTACK");
    UnitAttackSFX.TrackName = TEXT("單位攻擊");
    UnitAttackSFX.AudioType = EMingAudioType::SFX;
    UnitAttackSFX.bLoop = false;
    UnitAttackSFX.FadeInTime = 0.0f;
    UnitAttackSFX.FadeOutTime = 0.2f;
    AudioTracks.Add(UnitAttackSFX.TrackID, UnitAttackSFX);
}

void UMingAudioSystem::SetupVoiceTracks()
{
    // Character Voices
    FMingAudioTrack SunYatSenVoice;
    SunYatSenVoice.TrackID = TEXT("VOICE_SUN_YAT_SEN");
    SunYatSenVoice.TrackName = TEXT("孫中山語音");
    SunYatSenVoice.AudioType = EMingAudioType::Voice;
    SunYatSenVoice.bLoop = false;
    SunYatSenVoice.FadeInTime = 0.1f;
    SunYatSenVoice.FadeOutTime = 0.2f;
    AudioTracks.Add(SunYatSenVoice.TrackID, SunYatSenVoice);
    
    FMingAudioTrack ChiangKaiShekVoice;
    ChiangKaiShekVoice.TrackID = TEXT("VOICE_CHIANG_KAI_SHEK");
    ChiangKaiShekVoice.TrackName = TEXT("蔣介石語音");
    ChiangKaiShekVoice.AudioType = EMingAudioType::Voice;
    ChiangKaiShekVoice.bLoop = false;
    ChiangKaiShekVoice.FadeInTime = 0.1f;
    ChiangKaiShekVoice.FadeOutTime = 0.2f;
    AudioTracks.Add(ChiangKaiShekVoice.TrackID, ChiangKaiShekVoice);
    
    FMingAudioTrack LuXunVoice;
    LuXunVoice.TrackID = TEXT("VOICE_LU_XUN");
    LuXunVoice.TrackName = TEXT("魯迅語音");
    LuXunVoice.AudioType = EMingAudioType::Voice;
    LuXunVoice.bLoop = false;
    LuXunVoice.FadeInTime = 0.1f;
    LuXunVoice.FadeOutTime = 0.2f;
    AudioTracks.Add(LuXunVoice.TrackID, LuXunVoice);
    
    // Generic Voice Lines
    FMingAudioTrack VictoryVoice;
    VictoryVoice.TrackID = TEXT("VOICE_VICTORY");
    VictoryVoice.TrackName = TEXT("勝利語音");
    VictoryVoice.AudioType = EMingAudioType::Voice;
    VictoryVoice.bLoop = false;
    VictoryVoice.FadeInTime = 0.1f;
    VictoryVoice.FadeOutTime = 0.3f;
    AudioTracks.Add(VictoryVoice.TrackID, VictoryVoice);
    
    FMingAudioTrack DefeatVoice;
    DefeatVoice.TrackID = TEXT("VOICE_DEFEAT");
    DefeatVoice.TrackName = TEXT("失敗語音");
    DefeatVoice.AudioType = EMingAudioType::Voice;
    DefeatVoice.bLoop = false;
    DefeatVoice.FadeInTime = 0.1f;
    DefeatVoice.FadeOutTime = 0.3f;
    AudioTracks.Add(DefeatVoice.TrackID, DefeatVoice);
}

void UMingAudioSystem::SetupAmbientTracks()
{
    // Environmental Ambience
    FMingAudioTrack BattlefieldAmbient;
    BattlefieldAmbient.TrackID = TEXT("AMBIENT_BATTLEFIELD");
    BattlefieldAmbient.TrackName = TEXT("戰場環境音");
    BattlefieldAmbient.AudioType = EMingAudioType::Ambient;
    BattlefieldAmbient.bLoop = true;
    BattlefieldAmbient.FadeInTime = 3.0f;
    BattlefieldAmbient.FadeOutTime = 2.0f;
    AudioTracks.Add(BattlefieldAmbient.TrackID, BattlefieldAmbient);
    
    FMingAudioTrack CityAmbient;
    CityAmbient.TrackID = TEXT("AMBIENT_CITY");
    CityAmbient.TrackName = TEXT("城市環境音");
    CityAmbient.AudioType = EMingAudioType::Ambient;
    CityAmbient.bLoop = true;
    CityAmbient.FadeInTime = 2.0f;
    CityAmbient.FadeOutTime = 2.0f;
    AudioTracks.Add(CityAmbient.TrackID, CityAmbient);
    
    FMingAudioTrack ForestAmbient;
    ForestAmbient.TrackID = TEXT("AMBIENT_FOREST");
    ForestAmbient.TrackName = TEXT("森林環境音");
    ForestAmbient.AudioType = EMingAudioType::Ambient;
    ForestAmbient.bLoop = true;
    ForestAmbient.FadeInTime = 3.0f;
    ForestAmbient.FadeOutTime = 2.0f;
    AudioTracks.Add(ForestAmbient.TrackID, ForestAmbient);
    
    FMingAudioTrack WindAmbient;
    WindAmbient.TrackID = TEXT("AMBIENT_WIND");
    WindAmbient.TrackName = TEXT("風聲");
    WindAmbient.AudioType = EMingAudioType::Ambient;
    WindAmbient.bLoop = true;
    WindAmbient.FadeInTime = 2.0f;
    WindAmbient.FadeOutTime = 2.0f;
    AudioTracks.Add(WindAmbient.TrackID, WindAmbient);
}

void UMingAudioSystem::SetupUITracks()
{
    // UI Interaction Sounds
    FMingAudioTrack UISelectTrack;
    UISelectTrack.TrackID = TEXT("UI_SELECT");
    UISelectTrack.TrackName = TEXT("UI選擇");
    UISelectTrack.AudioType = EMingAudioType::UI;
    UISelectTrack.bLoop = false;
    UISelectTrack.FadeInTime = 0.0f;
    UISelectTrack.FadeOutTime = 0.05f;
    AudioTracks.Add(UISelectTrack.TrackID, UISelectTrack);
    
    FMingAudioTrack UIClickTrack;
    UIClickTrack.TrackID = TEXT("UI_CLICK");
    UIClickTrack.TrackName = TEXT("UI點擊");
    UIClickTrack.AudioType = EMingAudioType::UI;
    UIClickTrack.bLoop = false;
    UIClickTrack.FadeInTime = 0.0f;
    UIClickTrack.FadeOutTime = 0.05f;
    AudioTracks.Add(UIClickTrack.TrackID, UIClickTrack);
    
    FMingAudioTrack UIHoverTrack;
    UIHoverTrack.TrackID = TEXT("UI_HOVER");
    UIHoverTrack.TrackName = TEXT("UI懸停");
    UIHoverTrack.AudioType = EMingAudioType::UI;
    UIHoverTrack.bLoop = false;
    UIHoverTrack.FadeInTime = 0.0f;
    UIHoverTrack.FadeOutTime = 0.05f;
    AudioTracks.Add(UIHoverTrack.TrackID, UIHoverTrack);
    
    FMingAudioTrack UIErrorTrack;
    UIErrorTrack.TrackID = TEXT("UI_ERROR");
    UIErrorTrack.TrackName = TEXT("UI錯誤");
    UIErrorTrack.AudioType = EMingAudioType::UI;
    UIErrorTrack.bLoop = false;
    UIErrorTrack.FadeInTime = 0.0f;
    UIErrorTrack.FadeOutTime = 0.1f;
    AudioTracks.Add(UIErrorTrack.TrackID, UIErrorTrack);
    
    FMingAudioTrack UISuccessTrack;
    UISuccessTrack.TrackID = TEXT("UI_SUCCESS");
    UISuccessTrack.TrackName = TEXT("UI成功");
    UISuccessTrack.AudioType = EMingAudioType::UI;
    UISuccessTrack.bLoop = false;
    UISuccessTrack.FadeInTime = 0.0f;
    UISuccessTrack.FadeOutTime = 0.1f;
    AudioTracks.Add(UISuccessTrack.TrackID, UISuccessTrack);
}

void UMingAudioSystem::ShutdownAudio()
{
    StopMusic();
    StopAllSFX();
    StopVoice();
    StopAmbient();
    
    UE_LOG(LogTemp, Log, TEXT("Audio system shutdown"));
}

void UMingAudioSystem::UpdateAudioSettings(const FMingAudioSettings& NewSettings)
{
    AudioSettings = NewSettings;
    ApplyVolumeSettings();
    
    UE_LOG(LogTemp, Log, TEXT("Audio settings updated"));
}

FMingAudioSettings UMingAudioSystem::GetCurrentSettings() const
{
    return AudioSettings;
}

void UMingAudioSystem::PlayMusic(EMingMusicTheme Theme)
{
    TArray<FMingAudioTrack*> ThemeTracks = GetTracksByThemeInternal(Theme);
    if (ThemeTracks.Num() > 0)
    {
        PlayMusicTrack(ThemeTracks[0]->TrackID);
    }
}

void UMingAudioSystem::PlayMusicTrack(const FString& TrackID)
{
    if (!AudioSettings.bEnableMusic)
    {
        return;
    }
    
    FMingAudioTrack* Track = FindAudioTrack(TrackID);
    if (!Track || Track->AudioType != EMingAudioType::Music)
    {
        UE_LOG(LogTemp, Warning, TEXT("Music track not found: %s"), *TrackID);
        return;
    }
    
    // Stop current music
    StopMusic();
    
    // Play new music
    CurrentMusicTheme = Track->MusicTheme;
    CurrentMusicTrack = TrackID;
    
    PlayAudioTrackInternal(TrackID);
    
    OnMusicStarted.Broadcast(Track->MusicTheme, TrackID);
    
    UE_LOG(LogTemp, Log, TEXT("Music started: %s"), *Track->TrackName);
}

void UMingAudioSystem::StopMusic()
{
    if (!CurrentMusicTrack.IsEmpty())
    {
        StopAudioTrackInternal(CurrentMusicTrack);
        OnMusicStopped.Broadcast(CurrentMusicTrack);
        CurrentMusicTrack.Empty();
        CurrentMusicTheme = EMingMusicTheme::MainMenu;
        
        UE_LOG(LogTemp, Log, TEXT("Music stopped"));
    }
}

void UMingAudioSystem::PauseMusic()
{
    if (!CurrentMusicTrack.IsEmpty())
    {
        // This would pause the music
        UE_LOG(LogTemp, Log, TEXT("Music paused"));
    }
}

void UMingAudioSystem::ResumeMusic()
{
    if (!CurrentMusicTrack.IsEmpty())
    {
        // This would resume the music
        UE_LOG(LogTemp, Log, TEXT("Music resumed"));
    }
}

void UMingAudioSystem::FadeMusic(float TargetVolume, float Duration)
{
    if (!CurrentMusicTrack.IsEmpty())
    {
        // This would implement music fading
        UE_LOG(LogTemp, Log, TEXT("Fading music to %.2f over %.1f seconds"), TargetVolume, Duration);
    }
}

void UMingAudioSystem::CrossFadeMusic(EMingMusicTheme NewTheme, float Duration)
{
    TArray<FMingAudioTrack*> ThemeTracks = GetTracksByThemeInternal(NewTheme);
    if (ThemeTracks.Num() > 0)
    {
        // Fade out current music
        FadeMusic(0.0f, Duration * 0.5f);
        
        // Fade in new music
        FTimerHandle TimerHandle;
        if (WorldContext.IsValid())
        {
            WorldContext->GetTimerManager().SetTimer(
                TimerHandle,
                [this, NewTheme]()
                {
                    PlayMusic(NewTheme);
                },
                Duration * 0.5f,
                false
            );
        }
    }
}

bool UMingAudioSystem::IsMusicPlaying() const
{
    return !CurrentMusicTrack.IsEmpty();
}

EMingMusicTheme UMingAudioSystem::GetCurrentMusicTheme() const
{
    return CurrentMusicTheme;
}

void UMingAudioSystem::PlaySFX(const FString& SFXID)
{
    if (!AudioSettings.bEnableSFX)
    {
        return;
    }
    
    PlayAudioTrackInternal(SFXID);
    OnSFXPlayed.Broadcast(SFXID, FVector::ZeroVector);
}

void UMingAudioSystem::PlaySFXAtLocation(const FString& SFXID, FVector Location)
{
    if (!AudioSettings.bEnableSFX)
    {
        return;
    }
    
    PlayAudioTrackInternal(SFXID, Location);
    OnSFXPlayed.Broadcast(SFXID, Location);
}

void UMingAudioSystem::PlaySFX2D(const FString& SFXID)
{
    if (!AudioSettings.bEnableSFX)
    {
        return;
    }
    
    // Play as 2D sound (no attenuation)
    PlayAudioTrackInternal(SFXID);
    OnSFXPlayed.Broadcast(SFXID, FVector::ZeroVector);
}

void UMingAudioSystem::StopSFX(const FString& SFXID)
{
    StopAudioTrackInternal(SFXID);
}

void UMingAudioSystem::StopAllSFX()
{
    TArray<FMingAudioTrack*> SFXTracks = GetTracksByTypeInternal(EMingAudioType::SFX);
    for (FMingAudioTrack* Track : SFXTracks)
    {
        StopAudioTrackInternal(Track->TrackID);
    }
}

void UMingAudioSystem::PlayVoice(const FString& VoiceID)
{
    if (!AudioSettings.bEnableVoice)
    {
        return;
    }
    
    PlayAudioTrackInternal(VoiceID);
}

void UMingAudioSystem::PlayVoiceLine(const FString& CharacterID, const FString& LineID)
{
    FString VoiceID = FString::Printf(TEXT("%s_%s"), *CharacterID, *LineID);
    PlayVoice(VoiceID);
    OnVoicePlayed.Broadcast(CharacterID, LineID);
}

void UMingAudioSystem::StopVoice()
{
    TArray<FMingAudioTrack*> VoiceTracks = GetTracksByTypeInternal(EMingAudioType::Voice);
    for (FMingAudioTrack* Track : VoiceTracks)
    {
        StopAudioTrackInternal(Track->TrackID);
    }
}

void UMingAudioSystem::SetVoiceVolume(float Volume)
{
    AudioSettings.VoiceVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    ApplyVolumeSettings();
}

void UMingAudioSystem::PlayAmbient(const FString& AmbientID)
{
    if (!AudioSettings.bEnableAmbient)
    {
        return;
    }
    
    PlayAudioTrackInternal(AmbientID);
}

void UMingAudioSystem::StopAmbient()
{
    TArray<FMingAudioTrack*> AmbientTracks = GetTracksByTypeInternal(EMingAudioType::Ambient);
    for (FMingAudioTrack* Track : AmbientTracks)
    {
        StopAudioTrackInternal(Track->TrackID);
    }
}

void UMingAudioSystem::SetAmbientVolume(float Volume)
{
    AudioSettings.AmbientVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    ApplyVolumeSettings();
}

void UMingAudioSystem::PlayUISelect()
{
    PlaySFX(TEXT("UI_SELECT"));
}

void UMingAudioSystem::PlayUIClick()
{
    PlaySFX(TEXT("UI_CLICK"));
}

void UMingAudioSystem::PlayUIHover()
{
    PlaySFX(TEXT("UI_HOVER"));
}

void UMingAudioSystem::PlayUIError()
{
    PlaySFX(TEXT("UI_ERROR"));
}

void UMingAudioSystem::PlayUISuccess()
{
    PlaySFX(TEXT("UI_SUCCESS"));
}

void UMingAudioSystem::RegisterAudioTrack(const FMingAudioTrack& Track)
{
    AudioTracks.Add(Track.TrackID, Track);
    UE_LOG(LogTemp, Log, TEXT("Audio track registered: %s"), *Track.TrackName);
}

void UMingAudioSystem::UnregisterAudioTrack(const FString& TrackID)
{
    StopAudioTrackInternal(TrackID);
    AudioTracks.Remove(TrackID);
    UE_LOG(LogTemp, Log, TEXT("Audio track unregistered: %s"), *TrackID);
}

FMingAudioTrack UMingAudioSystem::GetAudioTrack(const FString& TrackID) const
{
    const FMingAudioTrack* Track = AudioTracks.Find(TrackID);
    return Track ? *Track : FMingAudioTrack();
}

TArray<FMingAudioTrack> UMingAudioSystem::GetTracksByType(EMingAudioType AudioType) const
{
    TArray<FMingAudioTrack> Result;
    for (const auto& Pair : AudioTracks)
    {
        if (Pair.Value.AudioType == AudioType)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FMingAudioTrack> UMingAudioSystem::GetTracksByTheme(EMingMusicTheme Theme) const
{
    TArray<FMingAudioTrack> Result;
    for (const auto& Pair : AudioTracks)
    {
        if (Pair.Value.MusicTheme == Theme)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

void UMingAudioSystem::SetMasterVolume(float Volume)
{
    AudioSettings.MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    ApplyVolumeSettings();
}

void UMingAudioSystem::SetMusicVolume(float Volume)
{
    AudioSettings.MusicVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    ApplyVolumeSettings();
}

void UMingAudioSystem::SetSFXVolume(float Volume)
{
    AudioSettings.SFXVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    ApplyVolumeSettings();
}

void UMingAudioSystem::SetVoiceVolume(float Volume)
{
    AudioSettings.VoiceVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    ApplyVolumeSettings();
}

void UMingAudioSystem::SetAmbientVolume(float Volume)
{
    AudioSettings.AmbientVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    ApplyVolumeSettings();
}

void UMingAudioSystem::SetUIVolume(float Volume)
{
    AudioSettings.UIVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    ApplyVolumeSettings();
}

float UMingAudioSystem::GetMasterVolume() const
{
    return AudioSettings.MasterVolume;
}

float UMingAudioSystem::GetMusicVolume() const
{
    return AudioSettings.MusicVolume;
}

float UMingAudioSystem::GetSFXVolume() const
{
    return AudioSettings.SFXVolume;
}

float UMingAudioSystem::GetVoiceVolume() const
{
    return AudioSettings.VoiceVolume;
}

float UMingAudioSystem::GetAmbientVolume() const
{
    return AudioSettings.AmbientVolume;
}

float UMingAudioSystem::GetUIVolume() const
{
    return AudioSettings.UIVolume;
}

void UMingAudioSystem::SetAudioState(EMingAudioType AudioType, bool bEnabled)
{
    switch (AudioType)
    {
    case EMingAudioType::Music:
        AudioSettings.bEnableMusic = bEnabled;
        if (!bEnabled) StopMusic();
        break;
    case EMingAudioType::SFX:
        AudioSettings.bEnableSFX = bEnabled;
        if (!bEnabled) StopAllSFX();
        break;
    case EMingAudioType::Voice:
        AudioSettings.bEnableVoice = bEnabled;
        if (!bEnabled) StopVoice();
        break;
    case EMingAudioType::Ambient:
        AudioSettings.bEnableAmbient = bEnabled;
        if (!bEnabled) StopAmbient();
        break;
    case EMingAudioType::UI:
        AudioSettings.bEnableUI = bEnabled;
        break;
    }
}

bool UMingAudioSystem::IsAudioTypeEnabled(EMingAudioType AudioType) const
{
    switch (AudioType)
    {
    case EMingAudioType::Music: return AudioSettings.bEnableMusic;
    case EMingAudioType::SFX: return AudioSettings.bEnableSFX;
    case EMingAudioType::Voice: return AudioSettings.bEnableVoice;
    case EMingAudioType::Ambient: return AudioSettings.bEnableAmbient;
    case EMingAudioType::UI: return AudioSettings.bEnableUI;
    default: return false;
    }
}

void UMingAudioSystem::MuteAllAudio()
{
    AudioSettings.MasterVolume = 0.0f;
    ApplyVolumeSettings();
}

void UMingAudioSystem::UnmuteAllAudio()
{
    AudioSettings.MasterVolume = 1.0f;
    ApplyVolumeSettings();
}

FString UMingAudioSystem::GetAudioTypeName(EMingAudioType AudioType)
{
    switch (AudioType)
    {
    case EMingAudioType::Music: return TEXT("音樂");
    case EMingAudioType::SFX: return TEXT("音效");
    case EMingAudioType::Voice: return TEXT("語音");
    case EMingAudioType::Ambient: return TEXT("環境音");
    case EMingAudioType::UI: return TEXT("UI音效");
    default: return TEXT("未知");
    }
}

FString UMingAudioSystem::GetMusicThemeName(EMingMusicTheme Theme)
{
    switch (Theme)
    {
    case EMingMusicTheme::MainMenu: return TEXT("主選單");
    case EMingMusicTheme::CampaignMap: return TEXT("戰役地圖");
    case EMingMusicTheme::Battle: return TEXT("戰鬥");
    case EMingMusicTheme::Victory: return TEXT("勝利");
    case EMingMusicTheme::Defeat: return TEXT("失敗");
    case EMingMusicTheme::Historical: return TEXT("歷史");
    case EMingMusicTheme::Republic: return TEXT("共和");
    case EMingMusicTheme::Imperial: return TEXT("帝制");
    case EMingMusicTheme::Tense: return TEXT("緊張");
    case EMingMusicTheme::Peaceful: return TEXT("和平");
    default: return TEXT("未知");
    }
}

FString UMingAudioSystem::SaveAudioSettings() const
{
    FString Result = TEXT("{\n");
    Result += FString::Printf(TEXT("  \"master_volume\": %.2f,\n"), AudioSettings.MasterVolume);
    Result += FString::Printf(TEXT("  \"music_volume\": %.2f,\n"), AudioSettings.MusicVolume);
    Result += FString::Printf(TEXT("  \"sfx_volume\": %.2f,\n"), AudioSettings.SFXVolume);
    Result += FString::Printf(TEXT("  \"voice_volume\": %.2f,\n"), AudioSettings.VoiceVolume);
    Result += FString::Printf(TEXT("  \"ambient_volume\": %.2f,\n"), AudioSettings.AmbientVolume);
    Result += FString::Printf(TEXT("  \"ui_volume\": %.2f,\n"), AudioSettings.UIVolume);
    Result += FString::Printf(TEXT("  \"enable_music\": %s,\n"), AudioSettings.bEnableMusic ? TEXT("true") : TEXT("false"));
    Result += FString::Printf(TEXT("  \"enable_sfx\": %s,\n"), AudioSettings.bEnableSFX ? TEXT("true") : TEXT("false"));
    Result += FString::Printf(TEXT("  \"enable_voice\": %s,\n"), AudioSettings.bEnableVoice ? TEXT("true") : TEXT("false"));
    Result += FString::Printf(TEXT("  \"enable_ambient\": %s,\n"), AudioSettings.bEnableAmbient ? TEXT("true") : TEXT("false"));
    Result += FString::Printf(TEXT("  \"enable_ui\": %s\n"), AudioSettings.bEnableUI ? TEXT("true") : TEXT("false"));
    Result += TEXT("}\n");
    
    return Result;
}

void UMingAudioSystem::LoadAudioSettings(const FString& JsonString)
{
    // Parse JSON and restore audio settings
    UE_LOG(LogTemp, Log, TEXT("Loading audio settings"));
}

void UMingAudioSystem::PlayAudioTrackInternal(const FString& TrackID, FVector Location)
{
    FMingAudioTrack* Track = FindAudioTrack(TrackID);
    if (!Track || !Track->AudioAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("Audio track not found or has no asset: %s"), *TrackID);
        return;
    }
    
    if (!WorldContext.IsValid())
    {
        return;
    }
    
    float Volume = GetEffectiveVolume(Track->AudioType);
    
    if (Location.IsNearlyZero())
    {
        // Play as 2D sound
        UGameplayStatics::PlaySound2D(WorldContext.Get(), Track->AudioAsset, Volume);
    }
    else
    {
        // Play as 3D sound
        UGameplayStatics::PlaySoundAtLocation(WorldContext.Get(), Track->AudioAsset, Location, Volume);
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Playing audio track: %s at volume %.2f"), *Track->TrackName, Volume);
}

void UMingAudioSystem::StopAudioTrackInternal(const FString& TrackID)
{
    // This would stop the specific audio track
    UE_LOG(LogTemp, Verbose, TEXT("Stopping audio track: %s"), *TrackID);
}

void UMingAudioSystem::ApplyVolumeSettings()
{
    // This would apply volume settings to all active audio
    UE_LOG(LogTemp, Log, TEXT("Applied audio volume settings"));
}

FMingAudioTrack* UMingAudioSystem::FindAudioTrack(const FString& TrackID)
{
    return AudioTracks.Find(TrackID);
}

TArray<FMingAudioTrack*> UMingAudioSystem::GetTracksByTypeInternal(EMingAudioType AudioType)
{
    TArray<FMingAudioTrack*> Result;
    for (auto& Pair : AudioTracks)
    {
        if (Pair.Value.AudioType == AudioType)
        {
            Result.Add(&Pair.Value);
        }
    }
    return Result;
}

TArray<FMingAudioTrack*> UMingAudioSystem::GetTracksByThemeInternal(EMingMusicTheme Theme)
{
    TArray<FMingAudioTrack*> Result;
    for (auto& Pair : AudioTracks)
    {
        if (Pair.Value.MusicTheme == Theme)
        {
            Result.Add(&Pair.Value);
        }
    }
    return Result;
}

float UMingAudioSystem::GetEffectiveVolume(EMingAudioType AudioType) const
{
    float TypeVolume = 1.0f;
    
    switch (AudioType)
    {
    case EMingAudioType::Music:
        TypeVolume = AudioSettings.MusicVolume;
        break;
    case EMingAudioType::SFX:
        TypeVolume = AudioSettings.SFXVolume;
        break;
    case EMingAudioType::Voice:
        TypeVolume = AudioSettings.VoiceVolume;
        break;
    case EMingAudioType::Ambient:
        TypeVolume = AudioSettings.AmbientVolume;
        break;
    case EMingAudioType::UI:
        TypeVolume = AudioSettings.UIVolume;
        break;
    }
    
    return AudioSettings.MasterVolume * TypeVolume;
}
