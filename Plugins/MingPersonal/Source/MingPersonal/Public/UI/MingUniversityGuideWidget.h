#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingUniversityGuideManager.h"
#include "MingUniversityGuideWidget.generated.h"

/**
 * 高校引�?UI Widget
 */
UCLASS()
class MINGPERSONAL_API UMingUniversityGuideWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    /**
     * 設置引�?管�X     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void SetGuideManager(UMingUniversityGuideManager* Manager};

    /**
     * 顯示引�X�容
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void DisplayContent(const FUniversityGuideContent& Content};

    /**
     * ?�新?�度�?     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void UpdateProgressBar(float Progress};

    /**
     * 顯示/?��?導航?��?
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void SetNavigationButtonsEnabled(bool bPreviousEnabled, bool bNextEnabled};

    /**
     * ?�放?�容?�畫
     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void PlayContentAnimation(};

    /**
     * 顯示完�?對話�?     */
    UFUNCTION(BlueprintCallable, Category = "University Guide UI")
    void ShowCompletionDialog(};

    // UI?��?綁�?（在?��?中實?��?
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

    // ?��?事件?��?
    UFUNCTION()
    void OnPreviousButtonClicked(};

    UFUNCTION()
    void OnNextButtonClicked(};

    UFUNCTION()
    void OnCloseButtonClicked(};

protected:
    UPROPERTY()
    TObjectPtr<UMingUniversityGuideManager> GuideManager;

    UFUNCTION()
    void HandleContentChanged(const FUniversityGuideContent& Content};

    UFUNCTION()
    void HandleProgressUpdated(const FString& ContentID, float Progress};

    UFUNCTION()
    void HandleGuideStarted(const FString& GuideID};

    UFUNCTION()
    void HandleGuideCompleted(const FString& GuideID};

    void SetupEventBindings(};
    void CleanupEventBindings(};
};

