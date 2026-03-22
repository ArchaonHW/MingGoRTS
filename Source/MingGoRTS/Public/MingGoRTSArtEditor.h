#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Materials/MaterialInterface.h"
#include "MingGoRTSArtEditor.generated.h"

UENUM(BlueprintType)
enum class EEditingTool : uint8
{
    Brush UMETA(DisplayName = "Brush"),
    Eraser UMETA(DisplayName = "Eraser"),
    Clone UMETA(DisplayName = "Clone"),
    Heal UMETA(DisplayName = "Heal"),
    Blur UMETA(DisplayName = "Blur"),
    Sharpen UMETA(DisplayName = "Sharpen"),
    Smudge UMETA(DisplayName = "Smudge"),
    Dodge UMETA(DisplayName = "Dodge"),
    Burn UMETA(DisplayName = "Burn"),
    ColorPicker UMETA(DisplayName = "Color Picker"),
    MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EBrushMode : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Multiply UMETA(DisplayName = "Multiply"),
    Screen UMETA(DisplayName = "Screen"),
    Overlay UMETA(DisplayName = "Overlay"),
    SoftLight UMETA(DisplayName = "Soft Light"),
    HardLight UMETA(DisplayName = "Hard Light"),
    ColorDodge UMETA(DisplayName = "Color Dodge"),
    ColorBurn UMETA(DisplayName = "Color Burn"),
    Darken UMETA(DisplayName = "Darken"),
    Lighten UMETA(DisplayName = "Lighten"),
    MAX UMETA(Hidden)
};

/**
 * MingGoRTS Art Editor
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSArtEditor : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSArtEditor();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void InitializeArtEditor();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    bool SetEditingTool(EEditingTool Tool);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    bool SetBrushMode(EBrushMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    bool EditTexture(UTexture2D* Texture, const FVector2D& Position);

    UFUNCTION(BlueprintPure, Category = "Art Editor")
    EEditingTool GetCurrentTool() const;

    UFUNCTION(BlueprintPure, Category = "Art Editor")
    EBrushMode GetCurrentBrushMode() const;

protected:
    UPROPERTY()
    EEditingTool CurrentTool;

    UPROPERTY()
    EBrushMode CurrentBrushMode;

    UPROPERTY()
    UTexture2D* EditingTexture;

    void InitializeEditingTools();
};
