#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingPersonalUIManager.generated.h"

class UMingRelationshipManager;
class UMingAudioRelationshipManager;
class UMingRepublicEraAudioThemes;

UENUM(BlueprintType)
enum class EPersonalUIType: uint8 {
    RelationshipPanel,     // ???X?�O
    ReputationPanel,       // ???X?�O
    DialoguePanel,         // ���??�O
    QuestPanel,            // ��?X?�O
    AudioPanel,            // ??�W??�O
    MainDashboard,         // �D??��O
    CharacterDetails,      // �����??
    RegionDetails,         // 摧毀��??
    SystemSettings         // �t�γ]�m
};

UENUM(BlueprintType)
enum class EUIAnimationType: uint8 {
    None,                  // ???X
    Fade,                  // �H�J�H�X
    Slide,                 // ��??
    Scale,                 // �Y��
    Rotate,                // 摧毀
    Bounce                 // �u��
};

USTRUCT(BlueprintType)
struct FPersonalUIConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    EPersonalUIType UIType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    FString WidgetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    FString WidgetPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    bool bIsVisible = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    bool bIsInteractive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    EUIAnimationType OpenAnimation = EUIAnimationType::Fade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    EUIAnimationType CloseAnimation = EUIAnimationType::Fade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    float AnimationDuration = 0.3f;

    FPersonalUIConfig()
    {
        UIType = EPersonalUIType::RelationshipPanel;
        WidgetName = TEXT("");
        WidgetPath = TEXT("");
        bIsVisible = false;
        bIsInteractive = true;
        OpenAnimation = EUIAnimationType::Fade;
        CloseAnimation = EUIAnimationType::Fade;
        AnimationDuration = 0.3f;
    }
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIStateChanged, bool, bIsAnyPanelOpen);

/**
 * ??�H�t��UI��?X * ��?X摧毀?�Y�B�n??�B??��??UI??��
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingPersonalUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPersonalUIManager();

    // ???X?UI�t��
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void InitializeUIManager(UMingRelationshipManager* RelationshipManager, 
                            UMingAudioRelationshipManager* AudioManager,
                            UMingRepublicEraAudioThemes* ThemeManager);

    // UI??�O��??
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void OpenUIPanel(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void CloseUIPanel(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ToggleUIPanel(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void CloseAllPanels();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ShowMainDashboard();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void HideMainDashboard();

    // Widget��??
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    UUserWidget* GetWidget(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    bool IsWidgetVisible(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void SetWidgetVisibility(EPersonalUIType UIType, bool bVisible);

    // UI??�m
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void AddUIConfig(const FPersonalUIConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void RemoveUIConfig(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal UI")
    FPersonalUIConfig GetUIConfig(EPersonalUIType UIType);

    // ??�e�t��
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void PlayOpenAnimation(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void PlayCloseAnimation(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void SetAnimationSpeed(float Speed);

    // ???X?�s
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateRelationshipUI();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateReputationUI();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateDialogueUI(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateQuestUI();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateAudioUI();

    // �D??��O
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void RefreshDashboard();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ShowNotification(const FString& Message, float Duration = 3.0f);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ShowConfirmationDialog(const FString& Title, const FString& Message, const FString& OnConfirm);

    // 摧毀�d??
    UFUNCTION(BlueprintPure, Category = "Personal UI")
    bool IsAnyPanelOpen() const;

    UFUNCTION(BlueprintPure, Category = "Personal UI")
    EPersonalUIType GetActivePanelType() const { return ActivePanelType; }

    UFUNCTION(BlueprintPure, Category = "Personal UI")
    TArray<EPersonalUIType> GetVisiblePanels() const;

    // �ƥ�
    UPROPERTY(BlueprintAssignable, Category = "UI Events")
    FOnUIPanelOpened OnUIPanelOpened;

    UPROPERTY(BlueprintAssignable, Category = "UI Events")
    FOnUIPanelClosed OnUIPanelClosed;

    UPROPERTY(BlueprintAssignable, Category = "UI Events")
    FOnUIStateChanged OnUIStateChanged;

protected:
    // �t�Τޥ�
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    UPROPERTY()
    TObjectPtr<UMingAudioRelationshipManager> AudioManager;

    UPROPERTY()
    TObjectPtr<UMingRepublicEraAudioThemes> ThemeManager;

    // Widget�s�x
    UPROPERTY()
    TMap<EPersonalUIType, UUserWidget*> WidgetMap;

    // UI??�m
    UPROPERTY()
    TMap<EPersonalUIType, FPersonalUIConfig> UIConfigMap;

    // ??X
    UPROPERTY()
    EPersonalUIType ActivePanelType = EPersonalUIType::MainDashboard;

    UPROPERTY()
    bool bIsMainDashboardVisible = false;

    UPROPERTY()
    float AnimationSpeed = 1.0f;

    // ??��??��
    UUserWidget* CreateWidget(EPersonalUIType UIType);
    void InitializeDefaultConfigs();
    void SetupEventBindings();
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);
    void OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType);

private:
    // Widget??�ػ��U??��
    UUserWidget* CreateRelationshipPanel();
    UUserWidget* CreateReputationPanel();
    UUserWidget* CreateDialoguePanel();
    UUserWidget* CreateQuestPanel();
    UUserWidget* CreateAudioPanel();
    UUserWidget* CreateMainDashboard();
    UUserWidget* CreateCharacterDetails();
    UUserWidget* CreateRegionDetails();
    UUserWidget* CreateSystemSettings();

    // ??�e���U??��
    void PlayAnimation(UUserWidget* Widget, EUIAnimationType AnimationType, float Duration);
    void StopAnimation(UUserWidget* Widget);
);

