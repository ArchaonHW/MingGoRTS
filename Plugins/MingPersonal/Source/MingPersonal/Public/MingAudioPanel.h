#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingAudioRelationshipManager.h"
#include "MingRepublicEraAudioThemes.h"
#include "MingAudioPanel.generated.h"

UENUM(BlueprintType)
enum class EAudioControlType: uint8 {
    Master,         // �D��X
    Relationship,  // 目標數量
    Reputation,     // 目標數量
    Dialogue,       // ���摧毀
    Ambient,        // 目標數量
    Music,          // ??��摧毀
    SFX             // 目標數量
};

UENUM(BlueprintType)
enum class EAudioThemeType: uint8 {
    Battle,         // ??���D??
    Peaceful,       // ??���D??
    Tension,        // ��i�D??
    Victory,        // ??�Q�D??
    Defeat,         // ��??�D??
    Diplomatic,     // �~��D??
    Economic,       // �g??�D??
    Cultural,       // 摧毀�D??
    Revolutionary,  // ??�R�D??
    Nostalgic       // 摧毀�D??
};

USTRUCT(BlueprintType)
struct FAudioControlData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Audio Control")
    EAudioControlType ControlType;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Control")
    FString ControlName;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Control")
    float Volume;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Control")
    bool bIsMuted;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Control")
    bool bIsEnabled;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Control")
    FLinearColor ControlColor;

    FAudioControlData()
    {
        ControlType = EAudioControlType::Master;
        ControlName = TEXT("");
        Volume = 1.0f;
        bIsMuted = false;
        bIsEnabled = true;
        ControlColor = FLinearColor::White;
    }
};

USTRUCT(BlueprintType)
struct FAudioThemeData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Audio Theme")
    EAudioThemeType ThemeType;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Theme")
    FString ThemeName;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Theme")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Theme")
    bool bIsPlaying;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Theme")
    bool bIsAvailable;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Theme")
    float Intensity;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Theme")
    FLinearColor ThemeColor;

    FAudioThemeData()
    {
        ThemeType = EAudioThemeType::Peaceful;
        ThemeName = TEXT("");
        Description = TEXT("");
        bIsPlaying = false;
        bIsAvailable = true;
        Intensity = 1.0f;
        ThemeColor = FLinearColor::White;
    }
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAudioSettingsChanged, const FString&, SettingName);

/**
 * ??�W??�OWidget
 * ��?X?�W�t��??��??��
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAudioPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingAudioPanel(const FObjectInitializer& ObjectInitializer);

    // ???X
    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void InitializeAudioPanel(UMingAudioRelationshipManager* InAudioManager, UMingRepublicEraAudioThemes* InThemeManager);

    // ???X?��
    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetVolume(EAudioControlType ControlType, float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetMasterVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetRelationshipVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetReputationVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetDialogueVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetAmbientVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetMusicVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetSFXVolume(float Volume);

    // ??��??��
    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void MuteControl(EAudioControlType ControlType, bool bMuted);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void MuteAll(bool bMuted);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void ToggleMute(EAudioControlType ControlType);

    // �D?X?��
    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void PlayTheme(EAudioThemeType ThemeType, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void StopTheme(EAudioThemeType ThemeType);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void PauseTheme(EAudioThemeType ThemeType);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void ResumeTheme(EAudioThemeType ThemeType);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetThemeIntensity(EAudioThemeType ThemeType, float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void FadeTheme(EAudioThemeType ThemeType, float TargetVolume, float FadeDuration);

    // ???X?�W
    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SetRegionalAudio(ERepublicEraRegion Region);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void PlayRegionalAmbient(ERepublicEraRegion Region, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void PlayCulturalAudio(ERepublicEraRegion Region, float Intensity = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void StopRegionalAudio();

    // ??�W����
    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void TestRelationshipAudio(ERelationshipAudioType AudioType);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void TestReputationAudio(EReputationAudioType AudioType);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void TestDialogueAudio(EDialogueAudioType DialogueType);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void TestAllAudio();

    // �]�m��??
    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void LoadAudioSettings();

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void SaveAudioSettings();

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void ResetToDefaults();

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void ApplyAudioProfile(const FString& ProfileName);

    // 摧毀�d??
    UFUNCTION(BlueprintPure, Category = "Audio Panel")
    float GetVolume(EAudioControlType ControlType) const;

    UFUNCTION(BlueprintPure, Category = "Audio Panel")
    bool IsMuted(EAudioControlType ControlType) const;

    UFUNCTION(BlueprintPure, Category = "Audio Panel")
    bool IsThemePlaying(EAudioThemeType ThemeType) const;

    UFUNCTION(BlueprintPure, Category = "Audio Panel")
    EAudioThemeType GetCurrentTheme() const;

    UFUNCTION(BlueprintPure, Category = "Audio Panel")
    ERepublicEraRegion GetCurrentRegion() const;

    // ??�W��??
    UFUNCTION(BlueprintPure, Category = "Audio Panel")
    float GetAverageVolume() const;

    UFUNCTION(BlueprintPure, Category = "Audio Panel")
    int32 GetActiveThemeCount() const;

    UFUNCTION(BlueprintPure, Category = "Audio Panel")
    int32 GetMutedControlCount() const;

    // ��?X???
    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void OnVolumeSliderChanged(EAudioControlType ControlType, float NewValue);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void OnMuteButtonClicked(EAudioControlType ControlType);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void OnThemeButtonClicked(EAudioThemeType ThemeType);

    UFUNCTION(BlueprintCallable, Category = "Audio Panel")
    void OnRegionButtonClicked(ERepublicEraRegion Region);

    // �ƥ�
    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnVolumeChanged OnVolumeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnThemeChanged OnThemeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Audio Events")
    FOnAudioSettingsChanged OnAudioSettingsChanged;

protected:
    // �t�Τޥ�
    UPROPERTY()
    TObjectPtr<UMingAudioRelationshipManager> AudioManager;

    UPROPERTY()
    TObjectPtr<UMingRepublicEraAudioThemes> ThemeManager;

    // ??�W??��摧毀
    UPROPERTY(BlueprintReadOnly, Category = "Audio Data")
    TArray<FAudioControlData> AudioControls;

    UPROPERTY(BlueprintReadOnly, Category = "Audio Data")
    TArray<FAudioThemeData> AudioThemes;

    // ???X?X
    UPROPERTY(BlueprintReadOnly, Category = "Audio State")
    EAudioThemeType CurrentThemeType;

    UPROPERTY(BlueprintReadOnly, Category = "Audio State")
    ERepublicEraRegion CurrentRegion;

    UPROPERTY(BlueprintReadOnly, Category = "Audio State")
    bool bIsAudioSystemInitialized;

    // �]�m
    UPROPERTY(BlueprintReadWrite, Category = "Audio Settings")
    float FadeDuration = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Audio Settings")
    bool bAutoFadeOnThemeChange = true;

    // UI�ե�ޥΡ]??�n�bBlueprint��??�w??
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* VolumeControlsContainer;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* ThemeControlsContainer;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* RegionControlsContainer;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentThemeText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentRegionText;

    UPROPERTY(meta = (BindWidget))
    class UButton* MuteAllButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* TestAllButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SaveSettingsButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* LoadSettingsButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ResetButton;

    // ??��??��
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ??��??��
    void InitializeAudioControls();
    void InitializeAudioThemes();
    void UpdateAudioUI();
    void UpdateVolumeUI();
    void UpdateThemeUI();
    void UpdateRegionUI();
    FAudioControlData CreateAudioControlData(EAudioControlType ControlType);
    FAudioThemeData CreateAudioThemeData(EAudioThemeType ThemeType);
    FLinearColor GetAudioControlColor(EAudioControlType ControlType) const;
    FLinearColor GetAudioThemeColor(EAudioThemeType ThemeType) const;
    FString GetAudioControlName(EAudioControlType ControlType) const;
    FString GetAudioThemeName(EAudioThemeType ThemeType) const;
    FString GetRegionDisplayName(ERepublicEraRegion Region) const;

    // �ƥ�j??
    void SetupEventBindings();
    void OnAudioRelationshipPlayed(ERelationshipAudioType AudioType, USoundBase* Sound);
    void OnAudioReputationPlayed(EReputationAudioType AudioType, USoundBase* Sound);
    void OnAudioDialoguePlayed(EDialogueAudioType AudioType, USoundBase* Sound);

private:
    // ���U??��
    void ApplyVolumeChange(EAudioControlType ControlType, float NewVolume);
    void ApplyMuteChange(EAudioControlType ControlType, bool bMuted);
    void ApplyThemeChange(EAudioThemeType ThemeType, bool bIsPlaying);
    void CreateVolumeSliders();
    void CreateThemeButtons();
    void CreateRegionButtons();
    void UpdateControlStates();
);

