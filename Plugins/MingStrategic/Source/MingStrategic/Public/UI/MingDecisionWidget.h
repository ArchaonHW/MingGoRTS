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
 * Ê±∫Á??∏È??âÈà™?ãÂ?Ë®? */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDecisionOptionSelected, const FString&, DecisionId, const FString&, OptionId);

/**
 * Ê±∫Á??åÈù¢Widget
 * È°ØÁ§∫Ê±∫Á?Â∞çË©±Ê°ÜÂ??∏È?
 */
UCLASS()
class MINGSTRATEGIC_API UMingDecisionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingDecisionWidget(const FObjectInitializer& ObjectInitializer);

    // ?ùÂ??ñÊ±∫Á≠ñÈ°ØÁ§?    UFUNCTION(BlueprintCallable, Category = "Decision UI")
    void SetupDecision(const FMingDecision& Decision);

    // È°ØÁ§∫Ê±∫Á??åÈù¢
    UFUNCTION(BlueprintCallable, Category = "Decision UI")
    void ShowDecision();

    // ?±Ë?Ê±∫Á??åÈù¢
    UFUNCTION(BlueprintCallable, Category = "Decision UI")
    void HideDecision();

    // ?∏È??∏Ê?‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Decision UI|Events")
    FOnDecisionOptionSelected OnOptionSelected;

protected:
    // WidgetÁµÑ‰ª∂ÂºïÁî®
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

    // ?∂Â?È°ØÁ§∫?ÑÊ±∫Á≠ñID
    UPROPERTY()
    FString CurrentDecisionId;

    // ?∏È??âÈà™?ãÂ?Ë°?    UPROPERTY()
    TArray<TObjectPtr<UButton>> OptionButtons;

    // ?ùÂ???    virtual void NativeConstruct() override;

    // ?µÂª∫?∏È??âÈà™??    UFUNCTION()
    void CreateOptionButtons(const TArray<FMingDecisionOption>& Options);

    // ?∏È??âÈà™?ãÈ??äË???    UFUNCTION()
    void OnOptionButtonClicked(const FString& OptionId);

    // Âª∂Â??âÈà™?ãÈ??äË???    UFUNCTION()
    void OnPostponeClicked();

    // ?ïÁï´?≠Êîæ
    UFUNCTION(BlueprintImplementableEvent, Category = "Decision UI|Animation")
    void PlayShowAnimation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Decision UI|Animation")
    void PlayHideAnimation();

    // Âπ≥Âè∞?©È?Ë®≠ÁΩÆ
    void SetupForPlatform();
};
