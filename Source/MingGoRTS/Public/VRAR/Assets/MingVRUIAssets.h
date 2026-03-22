// VR UI Asset Definitions for MingGoRTS
// Epic 9.1: VR/AR Support System - VR User Interface Assets

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MingVRUIAssets.generated.h"

/**
 * VR UI Panel Types
 */
UENUM(BlueprintType)
enum class EVRPanelType: uint8 {
    MainMenu        UMETA(DisplayName = "Main Menu"),
    HUD             UMETA(DisplayName = "HUD"),
    Settings        UMETA(DisplayName = "Settings"),
    Inventory       UMETA(DisplayName = "Inventory"),
    Dialogue        UMETA(DisplayName = "Dialogue"),
    Map             UMETA(DisplayName = "Map"),
    QuestLog        UMETA(DisplayName = "Quest Log"),
    Character       UMETA(DisplayName = "Character"),
    Crafting        UMETA(DisplayName = "Crafting"),
    Trade           UMETA(DisplayName = "Trade"),
    Notification    UMETA(DisplayName = "Notification"),
    RadialMenu      UMETA(DisplayName = "Radial Menu"),
    VirtualKeyboard UMETA(DisplayName = "Virtual Keyboard"),
    Custom          UMETA(DisplayName = "Custom")
};

/**
 * VR UI Element Types
 */
UENUM(BlueprintType)
enum class EVRUIElementType: uint8 {
    Button          UMETA(DisplayName = "Button"),
    Toggle          UMETA(DisplayName = "Toggle"),
    Slider          UMETA(DisplayName = "Slider"),
    Text            UMETA(DisplayName = "Text"),
    Image           UMETA(DisplayName = "Image"),
    ScrollBox       UMETA(DisplayName = "Scroll Box"),
    Dropdown        UMETA(DisplayName = "Dropdown"),
    InputField      UMETA(DisplayName = "Input Field"),
    ProgressBar     UMETA(DisplayName = "Progress Bar"),
    Icon            UMETA(DisplayName = "Icon"),
    Panel           UMETA(DisplayName = "Panel"),
    Tooltip         UMETA(DisplayName = "Tooltip"),
    Notification    UMETA(DisplayName = "Notification")
};

/**
 * VR Panel Visual Configuration
 */
USTRUCT(BlueprintType)
struct FVRPanelVisualConfig
{
    GENERATED_BODY()

    // Panel Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Panel Type")
    EVRPanelType PanelType = EVRPanelType::MainMenu;

    // Mesh Assets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> PanelMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> BorderMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> CornerMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> BackplateMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> PanelMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> BorderMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> BackplateMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> HighlightMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> DisabledMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor PanelColor = FLinearColor(0.1f, 0.1f, 0.15f, 0.9f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor BorderColor = FLinearColor(0.0f, 0.6f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor BackplateColor = FLinearColor(0.05f, 0.05f, 0.1f, 0.95f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor HighlightColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor DisabledColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.5f);

    // Dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    FVector2D PanelSize = FVector2D(400.0f, 300.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float PanelThickness = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float BorderWidth = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float CornerRadius = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float BackplateOffset = 5.0f;

    // Curvature (for curved VR panels)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curvature")
    bool bUseCurvedPanel = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curvature")
    float CurveRadius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curvature")
    float CurveAngle = 30.0f;

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float OpenAnimationDuration = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float CloseAnimationDuration = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TSoftObjectPtr<UAnimBlueprint> PanelAnimBlueprint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnableFloatingAnimation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FloatingAmplitude = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FloatingSpeed = 1.0f;
};

/**
 * VR Button Visual Configuration
 */
USTRUCT(BlueprintType)
struct FVRButtonVisualConfig
{
    GENERATED_BODY()

    // Mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> ButtonMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> IconMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> NormalMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> HoverMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> PressedMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> DisabledMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor NormalColor = FLinearColor(0.2f, 0.2f, 0.25f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor HoverColor = FLinearColor(0.0f, 0.6f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor PressedColor = FLinearColor(0.0f, 0.4f, 0.8f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor DisabledColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.5f);

    // Dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    FVector2D ButtonSize = FVector2D(120.0f, 40.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float ButtonDepth = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float PressDepth = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float CornerRadius = 5.0f;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> HoverSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> ClickSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> ReleaseSound;
};

/**
 * VR Radial Menu Configuration
 */
USTRUCT(BlueprintType)
struct FVRRadialMenuConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
    int32 SegmentCount = 8;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
    float InnerRadius = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
    float OuterRadius = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
    float Height = 5.0f;

    // Mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> SegmentMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> CenterMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> SegmentMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> SelectedSegmentMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> HoverSegmentMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> DisabledSegmentMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor SegmentColor = FLinearColor(0.2f, 0.2f, 0.25f, 0.9f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor SelectedColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor HoverColor = FLinearColor(0.0f, 0.6f, 0.8f, 0.8f);

    // Icons
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    TArray<TSoftObjectPtr<UTexture2D>> SegmentIcons;

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float OpenAnimationDuration = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float SelectionAnimationDuration = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float RotationSpeed = 180.0f;
};

/**
 * Virtual Keyboard Configuration
 */
USTRUCT(BlueprintType)
struct FVRVirtualKeyboardConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
    int32 RowCount = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
    int32 KeysPerRow = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
    float KeySpacing = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
    float RowSpacing = 5.0f;

    // Key Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keys")
    TSoftObjectPtr<UStaticMesh> KeyMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keys")
    FVector2D KeySize = FVector2D(20.0f, 20.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keys")
    float KeyDepth = 3.0f;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> KeyNormalMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> KeyPressedMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> KeySpecialMaterial;

    // Font
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Font")
    TSoftObjectPtr<UFont> KeyFont;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Font")
    float FontSize = 14.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Font")
    FLinearColor TextColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> KeyPressSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> KeyReleaseSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> BackspaceSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> EnterSound;
};

/**
 * VR UI Asset Definition
 */
UCLASS(BlueprintType)
class MINGRTS_API UMingVRUIAssets : public UDataAsset
{
    GENERATED_BODY()

public:
    // Panel Configurations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Panels")
    FVRPanelVisualConfig MainMenuConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Panels")
    FVRPanelVisualConfig HUDConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Panels")
    FVRPanelVisualConfig SettingsConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Panels")
    FVRPanelVisualConfig DialogueConfig;

    // Button Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons")
    FVRButtonVisualConfig DefaultButtonConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons")
    FVRButtonVisualConfig PrimaryButtonConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons")
    FVRButtonVisualConfig DangerButtonConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons")
    FVRButtonVisualConfig IconButtonConfig;

    // Radial Menu
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Menu")
    FVRRadialMenuConfig RadialMenuConfig;

    // Virtual Keyboard
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Keyboard")
    FVRVirtualKeyboardConfig KeyboardConfig;

    // Font Assets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
    TSoftObjectPtr<UFont> PrimaryFont;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
    TSoftObjectPtr<UFont> SecondaryFont;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fonts")
    TSoftObjectPtr<UFont> MonospaceFont;

    // Icon Textures
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    TSoftObjectPtr<UTexture2D> MenuIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    TSoftObjectPtr<UTexture2D> SettingsIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    TSoftObjectPtr<UTexture2D> InventoryIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    TSoftObjectPtr<UTexture2D> MapIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    TSoftObjectPtr<UTexture2D> QuestIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    TSoftObjectPtr<UTexture2D> CharacterIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    TSoftObjectPtr<UTexture2D> CraftingIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icons")
    TSoftObjectPtr<UTexture2D> TradeIcon;

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "VR UI Assets")
    FVRPanelVisualConfig GetPanelConfig(EVRPanelType PanelType) const;

    UFUNCTION(BlueprintCallable, Category = "VR UI Assets")
    FVRButtonVisualConfig GetButtonConfig(const FString& ButtonStyle) const;

    UFUNCTION(BlueprintCallable, Category = "VR UI Assets")
    TSoftObjectPtr<UTexture2D> GetIconTexture(const FString& IconName) const;
};
