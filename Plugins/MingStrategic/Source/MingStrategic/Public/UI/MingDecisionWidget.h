#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingDecision.h"
#include "MingDecisionWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UVerticalBox;
class UScrollBox;

/**
 * 決�X��X�鈪?��?�? */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDecisionOptionSelected, const FString&, DecisionId, const FString&, OptionId);

/**
 * 決�X�面Widget
 * 顯示決�?對話框�X��?
 */
UCLASS()
class MINGSTRATEGIC_API UMingDecisionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingDecisionWidget(const FObjectInitializer& ObjectInitializer};

    // ?��X�決策顯�?    UFUNCTION(BlueprintCallable, Category = "Decision UI")
    void SetupDecision(const FMingDecision& Decision};

    // 顯示決�X�面
    UFUNCTION(BlueprintCallable, Category = "Decision UI")
    void ShowDecision();

    // ?��?決�X�面
    UFUNCTION(BlueprintCallable, Category = "Decision UI")
    void HideDecision();

    // ?��X��?事件
    UPROPERTY(BlueprintAssignable, Category = "Decision UI|Events")
    FOnDecisionOptionSelected OnOptionSelected;

protected:
    // Widget組件引用
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> DecisionTitleText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BackgroundDescriptionText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BackgroundImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UScrollBox> OptionsContainer;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> PostponeButton;

    // ?��?顯示?�決策ID
    UPROPERTY()
    FString CurrentDecisionId;

    // ?��X�鈪?��?�?    UPROPERTY()
    TArray<TObjectPtr<UButton>> OptionButtons;

    // ?��X    virtual void NativeConstruct() override;

    // ?�建?��X�鈪X    UFUNCTION()
    void CreateOptionButtons(const TArray<FMingDecisionOption>& Options};

    // ?��X�鈪?��X��X    UFUNCTION()
    void OnOptionButtonClicked(const FString& OptionId};

    // 延�X�鈪?��X��X    UFUNCTION()
    void OnPostponeClicked();

    // ?�畫?�放
    UFUNCTION(BlueprintImplementableEvent, Category = "Decision UI|Animation")
    void PlayShowAnimation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Decision UI|Animation")
    void PlayHideAnimation();

    // 平台?��?設置
    void SetupForPlatform();
};

