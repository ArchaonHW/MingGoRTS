#include "MingAudioPanel.h"
#include "MingAudioRelationshipManager.h"
#include "MingRelationshipManager.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Engine/Engine.h"

UMingAudioPanel::UMingAudioPanel()
{
    bIsUpdating = false;
    AudioRelationshipManager = nullptr;
    RelationshipManager = nullptr;
    CurrentTheme = TEXT("");
    bIsMuted = false;
    MasterVolume = 1.0f;
    MusicVolume = 0.8f;
    SFXVolume = 0.8f;
    VoiceVolume = 0.8f;
    AmbientVolume = 0.6f;
}

void UMingAudioPanel::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 綁定按鈕事件
    if (PlayButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMingAudioPanel::OnPlayClicked);
    }
    
    if (PauseButton)
    {
        PauseButton->OnClicked.AddDynamic(this, &UMingAudioPanel::OnPauseClicked);
    }
    
    if (StopButton)
    {
        StopButton->OnClicked.AddDynamic(this, &UMingAudioPanel::OnStopClicked);
    }
    
    if (MuteButton)
    {
        MuteButton->OnClicked.AddDynamic(this, &UMingAudioPanel::OnMuteClicked);
    }
    
    if (TestRelationshipButton)
    {
        TestRelationshipButton->OnClicked.AddDynamic(this, &UMingAudioPanel::OnTestRelationshipClicked);
    }
    
    if (TestReputationButton)
    {
        TestReputationButton->OnClicked.AddDynamic(this, &UMingAudioPanel::OnTestReputationClicked);
    }
    
    if (ResetSettingsButton)
    {
        ResetSettingsButton->OnClicked.AddDynamic(this, &UMingAudioPanel::OnResetSettingsClicked);
    }
    
    // 綁定滑塊事件
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UMingAudioPanel::OnMasterVolumeChanged);
    }
    
    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UMingAudioPanel::OnMusicVolumeChanged);
    }
    
    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UMingAudioPanel::OnSFXVolumeChanged);
    }
    
    if (VoiceVolumeSlider)
    {
        VoiceVolumeSlider->OnValueChanged.AddDynamic(this, &UMingAudioPanel::OnVoiceVolumeChanged);
    }
    
    if (AmbientVolumeSlider)
    {
        AmbientVolumeSlider->OnValueChanged.AddDynamic(this, &UMingAudioPanel::OnAmbientVolumeChanged);
    }
    
    // 綁定下拉框事件
    if (ThemeComboBox)
    {
        ThemeComboBox->OnSelectionChanged.AddDynamic(this, &UMingAudioPanel::OnThemeSelectionChanged);
    }
    
    // 綁定複選框事件
    if (DynamicAudioCheckBox)
    {
        DynamicAudioCheckBox->OnCheckStateChanged.AddDynamic(this, &UMingAudioPanel::OnDynamicAudioChanged);
    }
    
    if (RelationshipAudioCheckBox)
    {
        RelationshipAudioCheckBox->OnCheckStateChanged.AddDynamic(this, &UMingAudioPanel::OnRelationshipAudioChanged);
    }
    
    if (ReputationAudioCheckBox)
    {
        ReputationAudioCheckBox->OnCheckStateChanged.AddDynamic(this, &UMingAudioPanel::OnReputationAudioChanged);
    }
    
    // 初始化UI
    InitializeUI();
}

void UMingAudioPanel::InitializeAudioPanel(UMingAudioRelationshipManager* InAudioManager, UMingRelationshipManager* InRelationshipManager)
{
    AudioRelationshipManager = InAudioManager;
    RelationshipManager = InRelationshipManager;
    
    if (AudioRelationshipManager)
    {
        // 獲取當前音頻設置
        UpdateAudioSettings();
        
        // 更新主題列表
        UpdateThemeList();
        
        // 更新音頻狀態
        UpdateAudioStatus();
    }
}

void UMingAudioPanel::UpdateAudioSettings()
{
    if (!AudioRelationshipManager)
    {
        return;
    }
    
    bIsUpdating = true;
    
    // 獲取當前音頻設置
    MasterVolume = AudioRelationshipManager->GetMasterVolume();
    MusicVolume = AudioRelationshipManager->GetMusicVolume();
    SFXVolume = AudioRelationshipManager->GetSFXVolume();
    VoiceVolume = AudioRelationshipManager->GetVoiceVolume();
    AmbientVolume = AudioRelationshipManager->GetAmbientVolume();
    bIsMuted = AudioRelationshipManager->IsMuted();
    CurrentTheme = AudioRelationshipManager->GetCurrentTheme();
    
    // 更新UI控件
    UpdateVolumeSliders();
    UpdateMuteButton();
    UpdateThemeSelection();
    UpdateAudioOptions();
    
    bIsUpdating = false;
}

void UMingAudioPanel::SetMasterVolume(float Volume)
{
    MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetMasterVolume(MasterVolume);
    }
    
    UpdateVolumeDisplay();
}

void UMingAudioPanel::SetMusicVolume(float Volume)
{
    MusicVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetMusicVolume(MusicVolume);
    }
    
    UpdateVolumeDisplay();
}

void UMingAudioPanel::SetSFXVolume(float Volume)
{
    SFXVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetSFXVolume(SFXVolume);
    }
    
    UpdateVolumeDisplay();
}

void UMingAudioPanel::SetVoiceVolume(float Volume)
{
    VoiceVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetVoiceVolume(VoiceVolume);
    }
    
    UpdateVolumeDisplay();
}

void UMingAudioPanel::SetAmbientVolume(float Volume)
{
    AmbientVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetAmbientVolume(AmbientVolume);
    }
    
    UpdateVolumeDisplay();
}

void UMingAudioPanel::SetMuted(bool bMuted)
{
    bIsMuted = bMuted;
    
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetMuted(bIsMuted);
    }
    
    UpdateMuteButton();
}

void UMingAudioPanel::SetTheme(const FString& ThemeName)
{
    if (CurrentTheme == ThemeName)
    {
        return;
    }
    
    CurrentTheme = ThemeName;
    
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetTheme(ThemeName);
    }
    
    UpdateThemeSelection();
    
    UE_LOG(LogTemp, Log, TEXT("切換音頻主題：%s"), *ThemeName);
}

void UMingAudioPanel::PlayAudio(const FString& AudioID)
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->PlayAudio(AudioID);
    }
}

void UMingAudioPanel::PauseAudio()
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->PauseAudio();
    }
    
    UpdatePlaybackButtons();
}

void UMingAudioPanel::StopAudio()
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->StopAudio();
    }
    
    UpdatePlaybackButtons();
}

void UMingAudioPanel::ResumeAudio()
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->ResumeAudio();
    }
    
    UpdatePlaybackButtons();
}

void UMingAudioPanel::TestRelationshipAudio(const FString& CharacterID, float RelationshipValue)
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->PlayRelationshipAudio(CharacterID, RelationshipValue);
    }
}

void UMingAudioPanel::TestReputationAudio(const FString& RegionID, float ReputationValue)
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->PlayReputationAudio(RegionID, ReputationValue);
    }
}

void UMingAudioPanel::ResetToDefaults()
{
    // 重置為默認設置
    MasterVolume = 1.0f;
    MusicVolume = 0.8f;
    SFXVolume = 0.8f;
    VoiceVolume = 0.8f;
    AmbientVolume = 0.6f;
    bIsMuted = false;
    
    // 應用設置
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetMasterVolume(MasterVolume);
        AudioRelationshipManager->SetMusicVolume(MusicVolume);
        AudioRelationshipManager->SetSFXVolume(SFXVolume);
        AudioRelationshipManager->SetVoiceVolume(VoiceVolume);
        AudioRelationshipManager->SetAmbientVolume(AmbientVolume);
        AudioRelationshipManager->SetMuted(bIsMuted);
    }
    
    // 更新UI
    UpdateAudioSettings();
    
    UE_LOG(LogTemp, Log, TEXT("音頻設置已重置為默認值"));
}

// 按鈕事件處理
void UMingAudioPanel::OnPlayClicked()
{
    ResumeAudio();
}

void UMingAudioPanel::OnPauseClicked()
{
    PauseAudio();
}

void UMingAudioPanel::OnStopClicked()
{
    StopAudio();
}

void UMingAudioPanel::OnMuteClicked()
{
    SetMuted(!bIsMuted);
}

void UMingAudioPanel::OnTestRelationshipClicked()
{
    // 測試關係音頻
    TestRelationshipAudio(TEXT("TestCharacter"), 75.0f);
}

void UMingAudioPanel::OnTestReputationClicked()
{
    // 測試聲望音頻
    TestReputationAudio(TEXT("TestRegion"), 60.0f);
}

void UMingAudioPanel::OnResetSettingsClicked()
{
    ResetToDefaults();
}

void UMingAudioPanel::OnMasterVolumeChanged(float Value)
{
    if (bIsUpdating)
    {
        return;
    }
    
    SetMasterVolume(Value);
}

void UMingAudioPanel::OnMusicVolumeChanged(float Value)
{
    if (bIsUpdating)
    {
        return;
    }
    
    SetMusicVolume(Value);
}

void UMingAudioPanel::OnSFXVolumeChanged(float Value)
{
    if (bIsUpdating)
    {
        return;
    }
    
    SetSFXVolume(Value);
}

void UMingAudioPanel::OnVoiceVolumeChanged(float Value)
{
    if (bIsUpdating)
    {
        return;
    }
    
    SetVoiceVolume(Value);
}

void UMingAudioPanel::OnAmbientVolumeChanged(float Value)
{
    if (bIsUpdating)
    {
        return;
    }
    
    SetAmbientVolume(Value);
}

void UMingAudioPanel::OnThemeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (bIsUpdating || SelectionType == ESelectInfo::Type::OnMouseClick)
    {
        return;
    }
    
    SetTheme(SelectedItem);
}

void UMingAudioPanel::OnDynamicAudioChanged(bool bIsChecked)
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetDynamicAudioEnabled(bIsChecked);
    }
}

void UMingAudioPanel::OnRelationshipAudioChanged(bool bIsChecked)
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetRelationshipAudioEnabled(bIsChecked);
    }
}

void UMingAudioPanel::OnReputationAudioChanged(bool bIsChecked)
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetReputationAudioEnabled(bIsChecked);
    }
}

// 私有輔助函數
void UMingAudioPanel::InitializeUI()
{
    // 初始化滑塊值
    UpdateVolumeSliders();
    
    // 初始化按鈕狀態
    UpdatePlaybackButtons();
    UpdateMuteButton();
    
    // 初始化複選框
    UpdateAudioOptions();
}

void UMingAudioPanel::UpdateVolumeSliders()
{
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->SetValue(MasterVolume);
    }
    
    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->SetValue(MusicVolume);
    }
    
    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->SetValue(SFXVolume);
    }
    
    if (VoiceVolumeSlider)
    {
        VoiceVolumeSlider->SetValue(VoiceVolume);
    }
    
    if (AmbientVolumeSlider)
    {
        AmbientVolumeSlider->SetValue(AmbientVolume);
    }
    
    UpdateVolumeDisplay();
}

void UMingAudioPanel::UpdateVolumeDisplay()
{
    if (MasterVolumeText)
    {
        MasterVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), MasterVolume * 100.0f)));
    }
    
    if (MusicVolumeText)
    {
        MusicVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), MusicVolume * 100.0f)));
    }
    
    if (SFXVolumeText)
    {
        SFXVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), SFXVolume * 100.0f)));
    }
    
    if (VoiceVolumeText)
    {
        VoiceVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), VoiceVolume * 100.0f)));
    }
    
    if (AmbientVolumeText)
    {
        AmbientVolumeText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), AmbientVolume * 100.0f)));
    }
}

void UMingAudioPanel::UpdatePlaybackButtons()
{
    bool bIsPlaying = AudioRelationshipManager ? AudioRelationshipManager->IsPlaying() : false;
    
    if (PlayButton)
    {
        PlayButton->SetIsEnabled(!bIsPlaying);
    }
    
    if (PauseButton)
    {
        PauseButton->SetIsEnabled(bIsPlaying);
    }
    
    if (StopButton)
    {
        StopButton->SetIsEnabled(bIsPlaying);
    }
}

void UMingAudioPanel::UpdateMuteButton()
{
    if (MuteButton)
    {
        if (bIsMuted)
        {
            MuteButton->SetText(FText::FromString(TEXT("取消靜音")));
        }
        else
        {
            MuteButton->SetText(FText::FromString(TEXT("靜音")));
        }
    }
}

void UMingAudioPanel::UpdateThemeList()
{
    if (!ThemeComboBox || !AudioRelationshipManager)
    {
        return;
    }
    
    // 清空現有選項
    ThemeComboBox->ClearOptions();
    
    // 獲取可用主題列表
    TArray<FString> AvailableThemes = AudioRelationshipManager->GetAvailableThemes();
    
    // 添加選項到下拉框
    for (const FString& Theme : AvailableThemes)
    {
        ThemeComboBox->AddOption(Theme);
    }
    
    // 選擇當前主題
    if (!CurrentTheme.IsEmpty())
    {
        ThemeComboBox->SetSelectedOption(CurrentTheme);
    }
}

void UMingAudioPanel::UpdateThemeSelection()
{
    if (ThemeComboBox && !CurrentTheme.IsEmpty())
    {
        ThemeComboBox->SetSelectedOption(CurrentTheme);
    }
}

void UMingAudioPanel::UpdateAudioOptions()
{
    if (!AudioRelationshipManager)
    {
        return;
    }
    
    bIsUpdating = true;
    
    if (DynamicAudioCheckBox)
    {
        DynamicAudioCheckBox->SetIsChecked(AudioRelationshipManager->IsDynamicAudioEnabled());
    }
    
    if (RelationshipAudioCheckBox)
    {
        RelationshipAudioCheckBox->SetIsChecked(AudioRelationshipManager->IsRelationshipAudioEnabled());
    }
    
    if (ReputationAudioCheckBox)
    {
        ReputationAudioCheckBox->SetIsChecked(AudioRelationshipManager->IsReputationAudioEnabled());
    }
    
    bIsUpdating = false;
}

void UMingAudioPanel::UpdateAudioStatus()
{
    if (!AudioRelationshipManager)
    {
        return;
    }
    
    // 更新當前播放狀態
    UpdatePlaybackButtons();
    
    // 更新當前主題信息
    if (CurrentThemeText)
    {
        CurrentThemeText->SetText(FText::FromString(CurrentTheme));
    }
    
    // 更新音頻統計信息
    UpdateAudioStatistics();
}

void UMingAudioPanel::UpdateAudioStatistics()
{
    if (!AudioRelationshipManager)
    {
        return;
    }
    
    // 獲取音頻統計信息
    int32 ActiveSounds = AudioRelationshipManager->GetActiveSoundCount();
    float MemoryUsage = AudioRelationshipManager->GetAudioMemoryUsage();
    
    // 更新統計顯示
    if (ActiveSoundsText)
    {
        ActiveSoundsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ActiveSounds)));
    }
    
    if (MemoryUsageText)
    {
        MemoryUsageText->SetText(FText::FromString(FString::Printf(TEXT("%.1f MB"), MemoryUsage)));
    }
}

void UMingAudioPanel::ShowAudioTestPanel()
{
    if (AudioTestPanel)
    {
        AudioTestPanel->SetVisibility(ESlateVisibility::Visible);
        PlayShowAnimation();
    }
}

void UMingAudioPanel::HideAudioTestPanel()
{
    if (AudioTestPanel)
    {
        PlayHideAnimation();
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (AudioTestPanel)
            {
                AudioTestPanel->SetVisibility(ESlateVisibility::Hidden);
            }
        }, 0.3f, false);
    }
}

void UMingAudioPanel::PlayShowAnimation()
{
    // 播放顯示動畫
    UE_LOG(LogTemp, Log, TEXT("播放音頻面板顯示動畫"));
}

void UMingAudioPanel::PlayHideAnimation()
{
    // 播放隱藏動畫
    UE_LOG(LogTemp, Log, TEXT("播放音頻面板隱藏動畫"));
}
