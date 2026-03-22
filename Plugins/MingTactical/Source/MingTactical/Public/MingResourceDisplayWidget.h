#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingResourceSystem.h"
#include "MingResourceDisplayWidget.generated.h"

// Ë≥áÊ??ÖÁõÆÁµêÊ?
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

/**
 * Ë≥áÊ?È°ØÁ§∫ Widget
 * È°ØÁ§∫6Á®ÆË?Ê∫êÁ??∏È??åÈÄ≤Â∫¶Ê¢? */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingResourceDisplayWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Ë≥áÊ?È°ØÁ§∫ÂÆπÂô®
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UHorizontalBox> ResourceContainer;

    // 6Á®ÆË?Ê∫êÁ?È°ØÁ§∫?ÖÁõÆ
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

    // ?¥Êñ∞?ΩÊï∏
    UFUNCTION(BlueprintCallable, Category = "Resources")
    void UpdateResourceDisplay(EMingResourceType ResourceType, int32 Amount, int32 Capacity};

    UFUNCTION(BlueprintCallable, Category = "Resources")
    void UpdateAllResources(const TMap<EMingResourceType, int32>& Resources, const TMap<EMingResourceType, int32>& Capacities};

    UFUNCTION(BlueprintCallable, Category = "Resources")
    void InitializeResourceDisplays(};

    // Ë¶ñË¶∫?àÊ?
    UFUNCTION(BlueprintCallable, Category = "Visual")
    void SetResourceHighlighted(EMingResourceType ResourceType, bool bHighlighted};

    UFUNCTION(BlueprintCallable, Category = "Visual")
    void ShowResourceChangeAnimation(EMingResourceType ResourceType, int32 ChangeAmount};

protected:
    UPROPERTY()
    TWeakObjectPtr<class UMingResourceSystem> ResourceSystem;

    // Ê≥®Ê?ÔºöÈ?UObject?áÈ?‰∏çËÉΩ?®ÊñºUPROPERTY
    TMap<EMingResourceType, FResourceDisplayItem*> ResourceDisplayMap;

    // ?ùÂ???    virtual bool Initialize() override;
    void SetupResourceDisplays(};
    void BindResourceEvents(};

    // ËºîÂä©?ΩÊï∏
    FResourceDisplayItem* GetResourceDisplayItem(EMingResourceType ResourceType};
    void UpdateSingleResourceItem(FResourceDisplayItem* Item, EMingResourceType ResourceType, int32 Amount, int32 Capacity};
    FString GetResourceDisplayName(EMingResourceType ResourceType) const;
    FLinearColor GetResourceDisplayColor(EMingResourceType ResourceType) const;

    // ‰∫ã‰ª∂?ïÁ?
    UFUNCTION()
    void OnResourceChanged(EMingResourceType ResourceType, int32 NewAmount};

    UFUNCTION()
    void OnResourceProduced(EMingResourceType ResourceType, int32 Amount};

    UFUNCTION()
    void OnResourceInsufficient(EMingResourceType ResourceType};

private:
    // ?ïÁï´Ë®àÊ???    TMap<EMingResourceType, float> AnimationTimers;
    TMap<EMingResourceType, int32> LastResourceAmounts;
    
    // Ë¶ñË¶∫?àÊ?
    void PlayResourceChangeEffect(FResourceDisplayItem* Item, int32 ChangeAmount};
    void UpdateResourceColor(FResourceDisplayItem* Item, float Percentage};
};

