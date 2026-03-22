#include "MingAudioPanel.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMingAudioPanel::UMingAudioPanel(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CurrentThemeType(EAudioThemeType::Peaceful)
    , CurrentRegion(ERepublicEraRegion::Beijing)
    , bIsAudioSystemInitialized(false)
    , FadeDuration(1.0f)
    , bAutoFadeOnThemeChange(true)
    , VolumeControlsContainer(nullptr)
    , ThemeControlsContainer(nullptr)
    , RegionControlsContainer(nullptr)
    , CurrentThemeText(nullptr)
    , CurrentRegionText(nullptr)
    , MuteAllButton(nullptr)
    , TestAllButton(nullptr)
    , SaveSettingsButton(nullptr)
    , LoadSettingsButton(nullptr)
    , ResetButton(nullptr)
{
}

void UMingAudioPanel::NativeConstruct()
{
    Super::NativeConstruct();

    InitializeAudioControls();
    InitializeAudioThemes();
    SetupEventBindings();
    UpdateAudioUI();
}

void UMingAudioPanel::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMingAudioPanel::InitializeAudioPanel(UMingAudioRelationshipManager* InAudioManager, UMingRepublicEraAudioThemes* InThemeManager)
{
    AudioManager = InAudioManager;
    ThemeManager = InThemeManager;
    
    if (AudioManager && ThemeManager)
    {
        bIsAudioSystemInitialized = true;
        UE_LOG(LogTemp, Log, TEXT("Audio panel initialized successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Audio panel initialization failed - missing system references"));
    }
}

void UMingAudioPanel::SetVolume(EAudioControlType ControlType, float Volume)
{
    Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
    // 更新控制數據
    for (FAudioControlData& ControlData : AudioControls)
    {
        if (ControlData.ControlType == ControlType)
        {
            ControlData.Volume = Volume;
            break;
        }
    }
    
    // 應用音量變化
    ApplyVolumeChange(ControlType, Volume);
    
    // 觸發事件
    OnVolumeChanged.Broadcast(ControlType, Volume);
    
    UE_LOG(LogTemp, Log, TEXT("Set volume for control %d: %.2f"), (int32)ControlType, Volume);
}

void UMingAudioPanel::SetMasterVolume(float Volume)
{
    SetVolume(EAudioControlType::Master, Volume);
}

void UMingAudioPanel::SetRelationshipVolume(float Volume)
{
    SetVolume(EAudioControlType::Relationship, Volume);
}

void UMingAudioPanel::SetReputationVolume(float Volume)
{
    SetVolume(EAudioControlType::Reputation, Volume);
}

void UMingAudioPanel::SetDialogueVolume(float Volume)
{
    SetVolume(EAudioControlType::Dialogue, Volume);
}

void UMingAudioPanel::SetAmbientVolume(float Volume)
{
    SetVolume(EAudioControlType::Ambient, Volume);
}

void UMingAudioPanel::SetMusicVolume(float Volume)
{
    SetVolume(EAudioControlType::Music, Volume);
}

void UMingAudioPanel::SetSFXVolume(float Volume)
{
    SetVolume(EAudioControlType::SFX, Volume);
}

void UMingAudioPanel::MuteControl(EAudioControlType ControlType, bool bMuted)
{
    // 更新控制數據
    for (FAudioControlData& ControlData : AudioControls)
    {
        if (ControlData.ControlType == ControlType)
        {
            ControlData.bIsMuted = bMuted;
            break;
        }
    }
    
    // 應用靜音變化
    ApplyMuteChange(ControlType, bMuted);
    
    UE_LOG(LogTemp, Log, TEXT("Mute control %d: %s"), (int32)ControlType, bMuted ? TEXT("true") : TEXT("false"));
}

void UMingAudioPanel::MuteAll(bool bMuted)
{
    for (EAudioControlType ControlType : {EAudioControlType::Master, EAudioControlType::Relationship, 
                                           EAudioControlType::Reputation, EAudioControlType::Dialogue,
                                           EAudioControlType::Ambient, EAudioControlType::Music, EAudioControlType::SFX})
    {
        MuteControl(ControlType, bMuted);
    }
}

void UMingAudioPanel::ToggleMute(EAudioControlType ControlType)
{
    bool bCurrentMuted = IsMuted(ControlType);
    MuteControl(ControlType, !bCurrentMuted);
}

void UMingAudioPanel::PlayTheme(EAudioThemeType ThemeType, float Intensity)
{
    if (!ThemeManager)
    {
        return;
    }
    
    // 停止當前主題
    if (CurrentThemeType != ThemeType && bAutoFadeOnThemeChange)
    {
        FadeTheme(CurrentThemeType, 0.0f, FadeDuration);
    }
    
    // 播放新主題
    ERepublicEraTheme RepublicTheme = static_cast<ERepublicEraTheme>(ThemeType);
    ThemeManager->PlayTheme(RepublicTheme, Intensity);
    
    // 更新狀態
    CurrentThemeType = ThemeType;
    
    // 更新主題數據
    for (FAudioThemeData& ThemeData : AudioThemes)
    {
        if (ThemeData.ThemeType == ThemeType)
        {
            ThemeData.bIsPlaying = true;
            ThemeData.Intensity = Intensity;
        }
        else
        {
            ThemeData.bIsPlaying = false;
        }
    }
    
    UpdateThemeUI();
    
    // 觸發事件
    OnThemeChanged.Broadcast(ThemeType, true);
    
    UE_LOG(LogTemp, Log, TEXT("Playing theme %d with intensity %.2f"), (int32)ThemeType, Intensity);
}

void UMingAudioPanel::StopTheme(EAudioThemeType ThemeType)
{
    if (!ThemeManager)
    {
        return;
    }
    
    ERepublicEraTheme RepublicTheme = static_cast<ERepublicEraTheme>(ThemeType);
    ThemeManager->StopTheme();
    
    // 更新狀態
    for (FAudioThemeData& ThemeData : AudioThemes)
    {
        if (ThemeData.ThemeType == ThemeType)
        {
            ThemeData.bIsPlaying = false;
        }
    }
    
    UpdateThemeUI();
    
    // 觸發事件
    OnThemeChanged.Broadcast(ThemeType, false);
    
    UE_LOG(LogTemp, Log, TEXT("Stopped theme %d"), (int32)ThemeType);
}

void UMingAudioPanel::PauseTheme(EAudioThemeType ThemeType)
{
    UE_LOG(LogTemp, Log, TEXT("Paused theme %d"), (int32)ThemeType);
}

void UMingAudioPanel::ResumeTheme(EAudioThemeType ThemeType)
{
    UE_LOG(LogTemp, Log, TEXT("Resumed theme %d"), (int32)ThemeType);
}

void UMingAudioPanel::SetThemeIntensity(EAudioThemeType ThemeType, float Intensity)
{
    for (FAudioThemeData& ThemeData : AudioThemes)
    {
        if (ThemeData.ThemeType == ThemeType)
        {
            ThemeData.Intensity = Intensity;
            break;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Set theme intensity for %d: %.2f"), (int32)ThemeType, Intensity);
}

void UMingAudioPanel::FadeTheme(EAudioThemeType ThemeType, float TargetVolume, float FadeDuration)
{
    UE_LOG(LogTemp, Log, TEXT("Fading theme %d to volume %.2f over %.2f seconds"), 
           (int32)ThemeType, TargetVolume, FadeDuration);
}

void UMingAudioPanel::SetRegionalAudio(ERepublicEraRegion Region)
{
    CurrentRegion = Region;
    
    if (ThemeManager)
    {
        ThemeManager->SetRegionalAudio(Region);
    }
    
    UpdateRegionUI();
    
    UE_LOG(LogTemp, Log, TEXT("Set regional audio for region %d"), (int32)Region);
}

void UMingAudioPanel::PlayRegionalAmbient(ERepublicEraRegion Region, float Intensity)
{
    if (ThemeManager)
    {
        ThemeManager->PlayRegionalAmbient(Region, Intensity);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Playing regional ambient for region %d"), (int32)Region);
}

void UMingAudioPanel::PlayCulturalAudio(ERepublicEraRegion Region, float Intensity)
{
    if (ThemeManager)
    {
        ThemeManager->PlayCulturalAudio(Region, Intensity);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Playing cultural audio for region %d"), (int32)Region);
}

void UMingAudioPanel::StopRegionalAudio()
{
    UE_LOG(LogTemp, Log, TEXT("Stopped regional audio"));
}

void UMingAudioPanel::TestRelationshipAudio(ERelationshipAudioType AudioType)
{
    if (AudioManager)
    {
        FRelationshipAudioParams Params;
        Params.AudioType = AudioType;
        Params.Intensity = 1.0f;
        
        AudioManager->PlayRelationshipAudio(Params);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Testing relationship audio: %d"), (int32)AudioType);
}

void UMingAudioPanel::TestReputationAudio(EReputationAudioType AudioType)
{
    if (AudioManager)
    {
        FReputationAudioParams Params;
        Params.AudioType = AudioType;
        
        AudioManager->PlayReputationAudio(Params);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Testing reputation audio: %d"), (int32)AudioType);
}

void UMingAudioPanel::TestDialogueAudio(EDialogueAudioType DialogueType)
{
    if (AudioManager)
    {
        FDialogueAudioParams Params;
        Params.DialogueType = DialogueType;
        Params.EmotionalIntensity = 1.0f;
        
        AudioManager->PlayDialogueAudio(Params);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Testing dialogue audio: %d"), (int32)DialogueType);
}

void UMingAudioPanel::TestAllAudio()
{
    UE_LOG(LogTemp, Log, TEXT("Testing all audio types"));
    
    TestRelationshipAudio(ERelationshipAudioType::RelationshipIncrease);
    TestReputationAudio(EReputationAudioType::ReputationIncrease);
    TestDialogueAudio(EDialogueAudioType::Friendly);
}

void UMingAudioPanel::LoadAudioSettings()
{
    UE_LOG(LogTemp, Log, TEXT("Loading audio settings"));
}

void UMingAudioPanel::SaveAudioSettings()
{
    UE_LOG(LogTemp, Log, TEXT("Saving audio settings"));
}

void UMingAudioPanel::ResetToDefaults()
{
    // 重置所有音量到默認值
    SetMasterVolume(1.0f);
    SetRelationshipVolume(1.0f);
    SetReputationVolume(1.0f);
    SetDialogueVolume(1.0f);
    SetAmbientVolume(1.0f);
    SetMusicVolume(1.0f);
    SetSFXVolume(1.0f);
    
    // 取消所有靜音
    MuteAll(false);
    
    // 播放默認主題
    PlayTheme(EAudioThemeType::Peaceful, 1.0f);
    
    UE_LOG(LogTemp, Log, TEXT("Reset audio settings to defaults"));
}

void UMingAudioPanel::ApplyAudioProfile(const FString& ProfileName)
{
    UE_LOG(LogTemp, Log, TEXT("Applying audio profile: %s"), *ProfileName);
}

float UMingAudioPanel::GetVolume(EAudioControlType ControlType) const
{
    for (const FAudioControlData& ControlData : AudioControls)
    {
        if (ControlData.ControlType == ControlType)
        {
            return ControlData.Volume;
        }
    }
    return 0.0f;
}

bool UMingAudioPanel::IsMuted(EAudioControlType ControlType) const
{
    for (const FAudioControlData& ControlData : AudioControls)
    {
        if (ControlData.ControlType == ControlType)
        {
            return ControlData.bIsMuted;
        }
    }
    return false;
}

bool UMingAudioPanel::IsThemePlaying(EAudioThemeType ThemeType) const
{
    for (const FAudioThemeData& ThemeData : AudioThemes)
    {
        if (ThemeData.ThemeType == ThemeType)
        {
            return ThemeData.bIsPlaying;
        }
    }
    return false;
}

EAudioThemeType UMingAudioPanel::GetCurrentTheme() const
{
    return CurrentThemeType;
}

ERepublicEraRegion UMingAudioPanel::GetCurrentRegion() const
{
    return CurrentRegion;
}

float UMingAudioPanel::GetAverageVolume() const
{
    if (AudioControls.Num() == 0)
    {
        return 0.0f;
    }
    
    float Total = 0.0f;
    for (const FAudioControlData& ControlData : AudioControls)
    {
        if (ControlData.ControlType != EAudioControlType::Master)
        {
            Total += ControlData.Volume;
        }
    }
    
    return Total / (AudioControls.Num() - 1); // 排除主音量
}

int32 UMingAudioPanel::GetActiveThemeCount() const
{
    int32 Count = 0;
    for (const FAudioThemeData& ThemeData : AudioThemes)
    {
        if (ThemeData.bIsPlaying)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingAudioPanel::GetMutedControlCount() const
{
    int32 Count = 0;
    for (const FAudioControlData& ControlData : AudioControls)
    {
        if (ControlData.bIsMuted)
        {
            Count++;
        }
    }
    return Count;
}

void UMingAudioPanel::OnVolumeSliderChanged(EAudioControlType ControlType, float NewValue)
{
    SetVolume(ControlType, NewValue);
}

void UMingAudioPanel::OnMuteButtonClicked(EAudioControlType ControlType)
{
    ToggleMute(ControlType);
}

void UMingAudioPanel::OnThemeButtonClicked(EAudioThemeType ThemeType)
{
    if (IsThemePlaying(ThemeType))
    {
        StopTheme(ThemeType);
    }
    else
    {
        PlayTheme(ThemeType, 1.0f);
    }
}

void UMingAudioPanel::OnRegionButtonClicked(ERepublicEraRegion Region)
{
    SetRegionalAudio(Region);
}

void UMingAudioPanel::InitializeAudioControls()
{
    AudioControls.Empty();
    
    // 創建音頻控制數據
    AudioControls.Add(CreateAudioControlData(EAudioControlType::Master));
    AudioControls.Add(CreateAudioControlData(EAudioControlType::Relationship));
    AudioControls.Add(CreateAudioControlData(EAudioControlType::Reputation));
    AudioControls.Add(CreateAudioControlData(EAudioControlType::Dialogue));
    AudioControls.Add(CreateAudioControlData(EAudioControlType::Ambient));
    AudioControls.Add(CreateAudioControlData(EAudioControlType::Music));
    AudioControls.Add(CreateAudioControlData(EAudioControlType::SFX));
}

void UMingAudioPanel::InitializeAudioThemes()
{
    AudioThemes.Empty();
    
    // 創建音頻主題數據
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Battle));
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Peaceful));
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Tension));
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Victory));
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Defeat));
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Diplomatic));
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Economic));
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Cultural));
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Revolutionary));
    AudioThemes.Add(CreateAudioThemeData(EAudioThemeType::Nostalgic));
}

void UMingAudioPanel::UpdateAudioUI()
{
    UpdateVolumeUI();
    UpdateThemeUI();
    UpdateRegionUI();
}

void UMingAudioPanel::UpdateVolumeUI()
{
    // 更新當前主題和地區顯示
    if (CurrentThemeText)
    {
        CurrentThemeText->SetText(FText::FromString(GetAudioThemeName(CurrentThemeType)));
    }
    
    if (CurrentRegionText)
    {
        CurrentRegionText->SetText(FText::FromString(GetRegionDisplayName(CurrentRegion)));
    }
}

void UMingAudioPanel::UpdateThemeUI()
{
    // 這裡應該更新主題按鈕的狀態
    UE_LOG(LogTemp, Log, TEXT("Updated theme UI"));
}

void UMingAudioPanel::UpdateRegionUI()
{
    // 這裡應該更新地區按鈕的狀態
    UE_LOG(LogTemp, Log, TEXT("Updated region UI"));
}

FAudioControlData UMingAudioPanel::CreateAudioControlData(EAudioControlType ControlType)
{
    FAudioControlData ControlData;
    ControlData.ControlType = ControlType;
    ControlData.ControlName = GetAudioControlName(ControlType);
    ControlData.Volume = 1.0f;
    ControlData.bIsMuted = false;
    ControlData.bIsEnabled = true;
    ControlData.ControlColor = GetAudioControlColor(ControlType);
    
    return ControlData;
}

FAudioThemeData UMingAudioPanel::CreateAudioThemeData(EAudioThemeType ThemeType)
{
    FAudioThemeData ThemeData;
    ThemeData.ThemeType = ThemeType;
    ThemeData.ThemeName = GetAudioThemeName(ThemeType);
    ThemeData.bIsPlaying = false;
    ThemeData.bIsAvailable = true;
    ThemeData.Intensity = 1.0f;
    ThemeData.ThemeColor = GetAudioThemeColor(ThemeType);
    
    return ThemeData;
}

FLinearColor UMingAudioPanel::GetAudioControlColor(EAudioControlType ControlType) const
{
    switch (ControlType)
    {
        case EAudioControlType::Master: return FLinearColor(1.0f, 0.8f, 0.0f, 1.0f); // 金色
        case EAudioControlType::Relationship: return FLinearColor(0.0f, 0.8f, 0.0f, 1.0f); // 綠色
        case EAudioControlType::Reputation: return FLinearColor(0.8f, 0.4f, 0.0f, 1.0f); // 橙色
        case EAudioControlType::Dialogue: return FLinearColor(0.0f, 0.8f, 0.8f, 1.0f); // 青色
        case EAudioControlType::Ambient: return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // 灰色
        case EAudioControlType::Music: return FLinearColor(0.8f, 0.0f, 0.8f, 1.0f); // 紫色
        case EAudioControlType::SFX: return FLinearColor(0.8f, 0.8f, 0.0f, 1.0f); // 黃色
        default: return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
    }
}

FLinearColor UMingAudioPanel::GetAudioThemeColor(EAudioThemeType ThemeType) const
{
    switch (ThemeType)
    {
        case EAudioThemeType::Battle: return FLinearColor(0.8f, 0.0f, 0.0f, 1.0f); // 紅色
        case EAudioThemeType::Peaceful: return FLinearColor(0.0f, 0.8f, 0.0f, 1.0f); // 綠色
        case EAudioThemeType::Tension: return FLinearColor(0.8f, 0.4f, 0.0f, 1.0f); // 橙色
        case EAudioThemeType::Victory: return FLinearColor(1.0f, 0.8f, 0.0f, 1.0f); // 金色
        case EAudioThemeType::Defeat: return FLinearColor(0.3f, 0.3f, 0.3f, 1.0f); // 深灰色
        case EAudioThemeType::Diplomatic: return FLinearColor(0.0f, 0.4f, 0.8f, 1.0f); // 深藍色
        case EAudioThemeType::Economic: return FLinearColor(0.0f, 0.8f, 0.8f, 1.0f); // 青色
        case EAudioThemeType::Cultural: return FLinearColor(0.8f, 0.0f, 0.8f, 1.0f); // 紫色
        case EAudioThemeType::Revolutionary: return FLinearColor(0.8f, 0.0f, 0.0f, 1.0f); // 紅色
        case EAudioThemeType::Nostalgic: return FLinearColor(0.5f, 0.5f, 0.8f, 1.0f); // 淡藍色
        default: return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
    }
}

FString UMingAudioPanel::GetAudioControlName(EAudioControlType ControlType) const
{
    switch (ControlType)
    {
        case EAudioControlType::Master: return TEXT("主音量");
        case EAudioControlType::Relationship: return TEXT("關係音效");
        case EAudioControlType::Reputation: return TEXT("聲望音效");
        case EAudioControlType::Dialogue: return TEXT("對話音效");
        case EAudioControlType::Ambient: return TEXT("環境音效");
        case EAudioControlType::Music: return TEXT("背景音樂");
        case EAudioControlType::SFX: return TEXT("特效音效");
        default: return TEXT("未知");
    }
}

FString UMingAudioPanel::GetAudioThemeName(EAudioThemeType ThemeType) const
{
    switch (ThemeType)
    {
        case EAudioThemeType::Battle: return TEXT("戰爭主題");
        case EAudioThemeType::Peaceful: return TEXT("和平主題");
        case EAudioThemeType::Tension: return TEXT("緊張主題");
        case EAudioThemeType::Victory: return TEXT("勝利主題");
        case EAudioThemeType::Defeat: return TEXT("失敗主題");
        case EAudioThemeType::Diplomatic: return TEXT("外交主題");
        case EAudioThemeType::Economic: return TEXT("經濟主題");
        case EAudioThemeType::Cultural: return TEXT("文化主題");
        case EAudioThemeType::Revolutionary: return TEXT("革命主題");
        case EAudioThemeType::Nostalgic: return TEXT("懷舊主題");
        default: return TEXT("未知主題");
    }
}

FString UMingAudioPanel::GetRegionDisplayName(ERepublicEraRegion Region) const
{
    switch (Region)
    {
        case ERepublicEraRegion::Beijing: return TEXT("北京");
        case ERepublicEraRegion::Shanghai: return TEXT("上海");
        case ERepublicEraRegion::Nanjing: return TEXT("南京");
        case ERepublicEraRegion::Guangzhou: return TEXT("廣州");
        case ERepublicEraRegion::Tianjin: return TEXT("天津");
        case ERepublicEraRegion::Wuhan: return TEXT("武漢");
        case ERepublicEraRegion::Chongqing: return TEXT("重慶");
        case ERepublicEraRegion::Xian: return TEXT("西安");
        case ERepublicEraRegion::Chengdu: return TEXT("成都");
        case ERepublicEraRegion::Harbin: return TEXT("哈爾濱");
        default: return TEXT("未知地區");
    }
}

void UMingAudioPanel::SetupEventBindings()
{
    // 綁定按鈕事件
    if (MuteAllButton)
    {
        MuteAllButton->OnClicked.AddDynamic(this, &UMingAudioPanel::TestAllAudio);
    }
    
    if (TestAllButton)
    {
        TestAllButton->OnClicked.AddDynamic(this, &UMingAudioPanel::TestAllAudio);
    }
    
    if (SaveSettingsButton)
    {
        SaveSettingsButton->OnClicked.AddDynamic(this, &UMingAudioPanel::SaveAudioSettings);
    }
    
    if (LoadSettingsButton)
    {
        LoadSettingsButton->OnClicked.AddDynamic(this, &UMingAudioPanel::LoadAudioSettings);
    }
    
    if (ResetButton)
    {
        ResetButton->OnClicked.AddDynamic(this, &UMingAudioPanel::ResetToDefaults);
    }
    
    // 綁定音頻管理器事件
    if (AudioManager)
    {
        AudioManager->OnRelationshipAudioPlayed.AddDynamic(this, &UMingAudioPanel::OnAudioRelationshipPlayed);
        AudioManager->OnReputationAudioPlayed.AddDynamic(this, &UMingAudioPanel::OnAudioReputationPlayed);
        AudioManager->OnDialogueAudioPlayed.AddDynamic(this, &UMingAudioPanel::OnAudioDialoguePlayed);
    }
}

void UMingAudioPanel::OnAudioRelationshipPlayed(ERelationshipAudioType AudioType, USoundBase* Sound)
{
    UE_LOG(LogTemp, Log, TEXT("Relationship audio played: %d"), (int32)AudioType);
}

void UMingAudioPanel::OnAudioReputationPlayed(EReputationAudioType AudioType, USoundBase* Sound)
{
    UE_LOG(LogTemp, Log, TEXT("Reputation audio played: %d"), (int32)AudioType);
}

void UMingAudioPanel::OnAudioDialoguePlayed(EDialogueAudioType AudioType, USoundBase* Sound)
{
    UE_LOG(LogTemp, Log, TEXT("Dialogue audio played: %d"), (int32)AudioType);
}

void UMingAudioPanel::ApplyVolumeChange(EAudioControlType ControlType, float NewVolume)
{
    if (!AudioManager)
    {
        return;
    }
    
    switch (ControlType)
    {
        case EAudioControlType::Master:
            AudioManager->SetAudioIntensity(NewVolume);
            break;
        case EAudioControlType::Relationship:
            AudioManager->SetRelationshipAudioVolume(NewVolume);
            break;
        case EAudioControlType::Reputation:
            AudioManager->SetReputationAudioVolume(NewVolume);
            break;
        case EAudioControlType::Dialogue:
            AudioManager->SetDialogueAudioVolume(NewVolume);
            break;
        default:
            break;
    }
}

void UMingAudioPanel::ApplyMuteChange(EAudioControlType ControlType, bool bMuted)
{
    float TargetVolume = bMuted ? 0.0f : 1.0f;
    ApplyVolumeChange(ControlType, TargetVolume);
}

void UMingAudioPanel::ApplyThemeChange(EAudioThemeType ThemeType, bool bIsPlaying)
{
    UpdateThemeUI();
}

void UMingAudioPanel::CreateVolumeSliders()
{
    // 這裡應該創建音量滑塊UI組件
    UE_LOG(LogTemp, Log, TEXT("Creating volume sliders"));
}

void UMingAudioPanel::CreateThemeButtons()
{
    // 這裡應該創建主題按鈕UI組件
    UE_LOG(LogTemp, Log, TEXT("Creating theme buttons"));
}

void UMingAudioPanel::CreateRegionButtons()
{
    // 這裡應該創建地區按鈕UI組件
    UE_LOG(LogTemp, Log, TEXT("Creating region buttons"));
}

void UMingAudioPanel::UpdateControlStates()
{
    UpdateVolumeUI();
    UpdateThemeUI();
    UpdateRegionUI();
}
