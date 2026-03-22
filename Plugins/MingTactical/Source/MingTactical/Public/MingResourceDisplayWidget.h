#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingResourceSystem.h"
#include "MingResourceDisplayWidget.generated.h"

/**
 * 資源顯示 Widget
 * 顯示6種資源的數量和進度條
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingResourceDisplayWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 資源顯示容器
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UHorizontalBox> ResourceContainer;

    // 資源項目結構
    USTRUCT(BlueprintType)
    struct FResourceDisplayItem
    {
        GENERATED_BODY()

        UPROPERTY()
        TObjectPtr<class UBorder> ItemBorder;

        UPROPERTY()
        TObjectPtr<class UTextBlock> ResourceNameText;

        UPROPERTY()
        TObjectPtr<class UTextBlock> ResourceAmountText;

        UPROPERTY()
        TObjectPtr<class UProgressBar> ResourceProgressBar;

        UPROPERTY()
        TObjectPtr<class UImage> ResourceIcon;

        EMingResourceType ResourceType;
    };

    // 6種資源的顯示項目
    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    FResourceDisplayItem FoodDisplay;

    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    FResourceDisplayItem MoneyDisplay;

    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    FResourceDisplayItem MaterialsDisplay;

    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    FResourceDisplayItem FuelDisplay;

    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    FResourceDisplayItem AmmoDisplay;

    UPROPERTY(BlueprintReadOnly, Category = "Resources")
    FResourceDisplayItem ManpowerDisplay;

    // 更新函數
    UFUNCTION(BlueprintCallable, Category = "Resources")
    void UpdateResourceDisplay(EMingResourceType ResourceType, int32 Amount, int32 Capacity);

    UFUNCTION(BlueprintCallable, Category = "Resources")
    void UpdateAllResources(const TMap<EMingResourceType, int32>& Resources, const TMap<EMingResourceType, int32>& Capacities);

    UFUNCTION(BlueprintCallable, Category = "Resources")
    void InitializeResourceDisplays();

    // 視覺效果
    UFUNCTION(BlueprintCallable, Category = "Visual")
    void SetResourceHighlighted(EMingResourceType ResourceType, bool bHighlighted);

    UFUNCTION(BlueprintCallable, Category = "Visual")
    void ShowResourceChangeAnimation(EMingResourceType ResourceType, int32 ChangeAmount);

protected:
    UPROPERTY()
    TWeakObjectPtr<class UMingResourceSystem> ResourceSystem;

    UPROPERTY()
    TMap<EMingResourceType, FResourceDisplayItem*> ResourceDisplayMap;

    // 初始化
    virtual bool Initialize() override;
    void SetupResourceDisplays();
    void BindResourceEvents();

    // 輔助函數
    FResourceDisplayItem* GetResourceDisplayItem(EMingResourceType ResourceType);
    void UpdateSingleResourceItem(FResourceDisplayItem* Item, EMingResourceType ResourceType, int32 Amount, int32 Capacity);
    FString GetResourceDisplayName(EMingResourceType ResourceType) const;
    FLinearColor GetResourceDisplayColor(EMingResourceType ResourceType) const;

    // 事件處理
    UFUNCTION()
    void OnResourceChanged(EMingResourceType ResourceType, int32 NewAmount);

    UFUNCTION()
    void OnResourceProduced(EMingResourceType ResourceType, int32 Amount);

    UFUNCTION()
    void OnResourceInsufficient(EMingResourceType ResourceType);

private:
    // 動畫計時器
    TMap<EMingResourceType, float> AnimationTimers;
    TMap<EMingResourceType, int32> LastResourceAmounts;
    
    // 視覺效果
    void PlayResourceChangeEffect(FResourceDisplayItem* Item, int32 ChangeAmount);
    void UpdateResourceColor(FResourceDisplayItem* Item, float Percentage);
};
