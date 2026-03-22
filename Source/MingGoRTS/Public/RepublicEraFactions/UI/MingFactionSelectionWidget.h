// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 势力选择UI界面 - Faction Selection UI Widget

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateTypes.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/GridPanel.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Styling/SlateBrush.h"
#include "MingFactionSelectionWidget.generated.h"

class UMingRTSFactionBase;

/**
 * 单个势力卡片UI
 */
UCLASS()
class MINGGORTS_API UMingFactionCardWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UBorder* CardBorder;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UImage* FactionIcon;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* FactionNameText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* FactionEnglishNameText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* DifficultyText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UImage* DifficultyStars;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UProgressBar* MilitaryPowerBar;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UProgressBar* EconomicPowerBar;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UProgressBar* DiplomaticPowerBar;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* SelectButton;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UBorder* HighlightBorder;
    
    /** 设置势力数据 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void SetFactionData(UMingRTSFactionBase* Faction);
    
    /** 设置选中状态 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void SetSelected(bool bSelected);
    
    /** 获取势力ID */
    UFUNCTION(BlueprintPure, Category = "Faction|UI")
    FName GetFactionID() const { return AssociatedFactionID; }

    UPROPERTY(BlueprintAssignable, Category = "Faction|UI")
    FOnFactionCardClicked OnFactionCardClicked;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFactionCardClicked, FName, FactionID);

protected:
    virtual void NativeConstruct() override;
    
    UFUNCTION()
    void OnCardClicked();
    
    UFUNCTION()
    void OnCardHovered();
    
    UFUNCTION()
    void OnCardUnhovered();

private:
    UPROPERTY()
    FName AssociatedFactionID;
    
    UPROPERTY()
    TObjectPtr<UMingRTSFactionBase> AssociatedFaction;
};

/**
 * 势力详情面板UI
 */
UCLASS()
class MINGGORTS_API UMingFactionDetailPanel : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* DetailFactionName;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* DetailDescription;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* CapitalCityText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* StartingYearText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* InitialResourcesText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UVerticalBox* AdvantagesList;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UVerticalBox* DisadvantagesList;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UVerticalBox* UniqueUnitsList;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UVerticalBox* UniqueMechanicsList;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* GameplayGuideText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* ConfirmSelectionButton;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* BackButton;
    
    /** 显示势力详情 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void ShowFactionDetails(UMingRTSFactionBase* Faction);
    
    /** 清空详情显示 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void ClearDetails();

    UPROPERTY(BlueprintAssignable, Category = "Faction|UI")
    FOnConfirmSelection OnConfirmSelection;
    
    UPROPERTY(BlueprintAssignable, Category = "Faction|UI")
    FOnBackClicked OnBackClicked;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmSelection);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackClicked);

protected:
    virtual void NativeConstruct() override;
    
    UFUNCTION()
    void OnConfirmClicked();
    
    UFUNCTION()
    void OnBackClicked();

private:
    UPROPERTY()
    TObjectPtr<UMingRTSFactionBase> CurrentDisplayedFaction;
};

/**
 * 主势力选择界面
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UMingFactionSelectionWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UMingFactionSelectionWidget(const FObjectInitializer& ObjectInitializer);
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* TitleText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* SubtitleText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UScrollBox* FactionScrollBox;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UHorizontalBox* FactionCardsContainer;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UMingFactionDetailPanel* DetailPanel;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UBorder* FilterPanel;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* FilterEasyButton;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* FilterNormalButton;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* FilterHardButton;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* FilterAllButton;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* SelectedFactionText;
    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* RandomSelectButton;
    
    /** 初始化势力选择界面 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void InitializeFactionSelection();
    
    /** 刷新势力列表 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void RefreshFactionList();
    
    /** 按难度筛选 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void FilterByDifficulty(EFactionDifficulty Difficulty);
    
    /** 显示所有势力 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void ShowAllFactions();
    
    /** 随机选择势力 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void RandomSelectFaction();
    
    /** 获取当前选中势力 */
    UFUNCTION(BlueprintPure, Category = "Faction|UI")
    UMingRTSFactionBase* GetSelectedFaction() const;
    
    /** 确认选择势力 */
    UFUNCTION(BlueprintCallable, Category = "Faction|UI")
    void ConfirmFactionSelection();

    UPROPERTY(BlueprintAssignable, Category = "Faction|UI")
    FOnFactionSelected OnFactionSelected;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFactionSelected, UMingRTSFactionBase*, SelectedFaction);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    
    UFUNCTION()
    void OnFactionCardClicked(FName FactionID);
    
    UFUNCTION()
    void OnFilterEasyClicked();
    
    UFUNCTION()
    void OnFilterNormalClicked();
    
    UFUNCTION()
    void OnFilterHardClicked();
    
    UFUNCTION()
    void OnFilterAllClicked();
    
    UFUNCTION()
    void OnRandomSelectClicked();
    
    UFUNCTION()
    void OnDetailConfirmClicked();

private:
    UPROPERTY()
    TObjectPtr<UMingRTSFactionManager> FactionManager;
    
    UPROPERTY()
    TArray<TObjectPtr<UMingFactionCardWidget>> FactionCards;
    
    UPROPERTY()
    TObjectPtr<UMingRTSFactionBase> CurrentlySelectedFaction;
    
    UPROPERTY()
    EFactionDifficulty CurrentFilter = EFactionDifficulty::Normal;
    
    /** 创建势力卡片 */
    void CreateFactionCard(UMingRTSFactionBase* Faction);
    
    /** 更新选中状态显示 */
    void UpdateSelectionVisuals();
    
    /** 获取势力难度颜色 */
    FSlateColor GetDifficultyColor(EFactionDifficulty Difficulty) const;
};
