#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MingInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FMINGTACTICAL_API FMingInputActionConfig
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<class UInputAction> InputAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName InputTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bShowInSettings = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText SettingsDisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText SettingsDescription;
};

/**
 * Input Configuration Data Asset
 * Contains all input action mappings for MingGoRTS
 */
UCLASS()
class MINGTACTICAL_API UMingInputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    UMingInputConfig(const FObjectInitializer& ObjectInitializer);

    // Core Selection Actions
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selection")
    TSoftObjectPtr<UInputAction> SelectAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selection")
    TSoftObjectPtr<UInputAction> SelectCompleteAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selection")
    TSoftObjectPtr<UInputAction> AddToSelectionAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selection")
    TSoftObjectPtr<UInputAction> RemoveFromSelectionAction;

    // Command Actions
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Commands")
    TSoftObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Commands")
    TSoftObjectPtr<UInputAction> AttackAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Commands")
    TSoftObjectPtr<UInputAction> StopAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Commands")
    TSoftObjectPtr<UInputAction> HoldPositionAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Commands")
    TSoftObjectPtr<UInputAction> PatrolAction;

    // Camera Actions
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    TSoftObjectPtr<UInputAction> CameraPanAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    TSoftObjectPtr<UInputAction> CameraRotateAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
    TSoftObjectPtr<UInputAction> CameraZoomAction;

    // Utility Actions
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Utility")
    TSoftObjectPtr<UInputAction> SelectAllAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Utility")
    TSoftObjectPtr<UInputAction> CancelAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Utility")
    TSoftObjectPtr<UInputAction> OpenMenuAction;

    // Input Mapping Contexts
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Contexts")
    TSoftObjectPtr<class UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Contexts")
    TSoftObjectPtr<UInputMappingContext> SelectionMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Contexts")
    TSoftObjectPtr<UInputMappingContext> CommandMappingContext;

    // Helper function to get input action by tag
    UFUNCTION(BlueprintCallable, Category = "Input")
    const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

    // Input configuration data for settings menu
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
    TArray<FMingInputActionConfig> InputActionConfigs;
};

