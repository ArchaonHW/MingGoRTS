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
 * 決策選項按鈪態委託
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDecisionOptionSelected, const FString&, DecisionId, const FString&, OptionId);

/**
 * 決策界面Widget
 * 顯示決策對話框和選項
 */
UCLASS()
class MINGSTRATEGIC_API UMingDecisionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingDecisionWidget(const FObjectInitializer& ObjectInitializer);

    // 初始化決策顯示
    UFUNCTION(BlueprintCallable, Category = "Decision UI")
    void SetupDecision(const FMingDecision& Decision);

    // 顯示決策界面
    UFUNCTION(BlueprintCallable, Category = "Decision UI")
    void ShowDecision();

    // 隱藏決策界面
    UFUNCTION(BlueprintCallable, Category = "Decision UI")
    void HideDecision();

    // 選項選擇事件
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

    // 當前顯示的決策ID
    UPROPERTY()
    FString CurrentDecisionId;

    // 選項按鈪態列表
    UPROPERTY()
    TArray<TObjectPtr<UButton>> OptionButtons;

    // 初始化
    virtual void NativeConstruct() override;

    // 創建選項按鈪態
    UFUNCTION()
    void CreateOptionButtons(const TArray<FMingDecisionOption>& Options);

    // 選項按鈪態點擊處理
    UFUNCTION()
    void OnOptionButtonClicked(const FString& OptionId);

    // 延後按鈪態點擊處理
    UFUNCTION()
    void OnPostponeClicked();

    // 動畫播放
    UFUNCTION(BlueprintImplementableEvent, Category = "Decision UI|Animation")
    void PlayShowAnimation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Decision UI|Animation")
    void PlayHideAnimation();

    // 平台適配設置
    void SetupForPlatform();
};
