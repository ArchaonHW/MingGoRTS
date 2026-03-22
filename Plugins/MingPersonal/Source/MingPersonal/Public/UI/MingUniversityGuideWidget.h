#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingUniversityGuideManager.h"
#include "MingUniversityGuideWidget.generated.h"

/**
 * È´òÊ†°ÂºïÂ?UI Widget
 */
UCLASS()
class MINGPERSONAL_API UMingUniversityGuideWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    /**
     * Ë®≠ÁΩÆÂºïÂ?ÁÆ°Á???     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void SetGuideManager(UMingUniversityGuideManager* Manager);

    /**
     * È°ØÁ§∫ÂºïÂ??ßÂÆπ
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void DisplayContent(const FUniversityGuideContent& Content);

    /**
     * ?¥Êñ∞?≤Â∫¶Ê¢?     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void UpdateProgressBar(float Progress);

    /**
     * È°ØÁ§∫/?±Ë?Â∞éËà™?âÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void SetNavigationButtonsEnabled(bool bPreviousEnabled, bool bNextEnabled);

    /**
     * ?≠Êîæ?ßÂÆπ?ïÁï´
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void PlayContentAnimation();

    /**
     * È°ØÁ§∫ÂÆåÊ?Â∞çË©±Ê°?     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void ShowCompletionDialog();

    // UI?ÉÁ?Á∂ÅÂ?ÔºàÂú®?çÂ?‰∏≠ÂØ¶?æÔ?
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TitleText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DescriptionText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UButton* PreviousButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* NextButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CloseButton;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* NavigationBox;

    // ?âÈ?‰∫ã‰ª∂?ïÁ?
    UFUNCTION()
    void OnPreviousButtonClicked();

    UFUNCTION()
    void OnNextButtonClicked();

    UFUNCTION()
    void OnCloseButtonClicked();

protected:
    UPROPERTY()
    TObjectPtr<UMingUniversityGuideManager> GuideManager;

    UFUNCTION()
    void HandleContentChanged(const FUniversityGuideContent& Content);

    UFUNCTION()
    void HandleProgressUpdated(const FString& ContentID, float Progress);

    UFUNCTION()
    void HandleGuideStarted(const FString& GuideID);

    UFUNCTION()
    void HandleGuideCompleted(const FString& GuideID);

    void SetupEventBindings();
    void CleanupEventBindings();
};
