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
    Brush,            // ?´Á?
    Eraser,           // Ê©°ÁöÆ??    Clone,            // ?ãÈ?Â∑•ÂÖ∑
    Heal,             // ‰øÆÂæ©Â∑•ÂÖ∑
    Blur,             // Ê®°Á?Â∑•ÂÖ∑
    Sharpen,          // ?≥Â?Â∑•ÂÖ∑
    Smudge,           // Ê∂ÇÊäπÂ∑•ÂÖ∑
    Dodge,            // Ê∏õÊ∑°Â∑•ÂÖ∑
    Burn,             // ?†Ê∑±Â∑•ÂÖ∑
    ColorPicker       // È°èËâ≤?∏Ê???};

UENUM(BlueprintType)
enum class EBrushMode : uint8
{
    Normal,           // Ê≠?∏∏
    Multiply,         // Ê≠???äÂ?
    Screen,           // ÊøæËâ≤
    Overlay,          // ?†Â?
    SoftLight,        // ?îÂ?
    HardLight,        // Âº∑Â?
    ColorDodge,       // È°èËâ≤Ê∏õÊ∑°
    ColorBurn,        // È°èËâ≤?†Ê∑±
    Darken,           // ËÆäÊ?
    Lighten,          // ËÆä‰∫Æ
    Difference        // Â∑ÆÂÄ?};

USTRUCT(BlueprintType)
struct FBrushSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush Settings")
    float Size = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush Settings")
    float Hardness = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush Settings")
    float Opacity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush Settings")
    float Flow = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush Settings")
    FLinearColor Color = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush Settings")
    EBrushMode Mode = EBrushMode::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush Settings")
    bool bPressureSensitive = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush Settings")
    int32 Spacing = 25;
};

USTRUCT(BlueprintType)
struct FLayerInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer")
    FString LayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer")
    UTexture2D* LayerTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer")
    float Opacity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer")
    EBrushMode BlendMode = EBrushMode::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer")
    bool bVisible = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer")
    bool bLocked = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer")
    int32 LayerIndex = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArtEdited, class UTexture2D*, EditedArt);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLayerChanged, const FString&, LayerName, int32, LayerIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToolChanged, EEditingTool, NewTool);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSArtEditor : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSArtEditor();

    // ?ñÂ?Á∑®ËºØ?∫Êú¨?üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void LoadImage(UTexture2D* Image);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SaveImage(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    UTexture2D* GetCurrentImage() const { return CurrentImage; }

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void CreateNewImage(int32 Width, int32 Height, FLinearColor BackgroundColor = FLinearColor::White);

    // Á∑®ËºØÂ∑•ÂÖ∑
    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SetEditingTool(EEditingTool Tool);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    EEditingTool GetCurrentTool() const { return CurrentTool; }

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SetBrushSettings(const FBrushSettings& Settings);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    FBrushSettings GetBrushSettings() const { return BrushSettings; }

    // Áπ™Ë£Ω?üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void StartStroke(const FVector2D& Position);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ContinueStroke(const FVector2D& Position);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void EndStroke();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void DrawLine(const FVector2D& StartPos, const FVector2D& EndPos);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void DrawRectangle(const FVector2D& TopLeft, const FVector2D& BottomRight);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void DrawCircle(const FVector2D& Center, float Radius);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void FillArea(const FVector2D& Position, FLinearColor FillColor);

    // ?ñÂ±§Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void AddLayer(const FString& LayerName);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void RemoveLayer(const FString& LayerName);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SelectLayer(const FString& LayerName);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void MoveLayerUp(const FString& LayerName);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void MoveLayerDown(const FString& LayerName);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SetLayerOpacity(const FString& LayerName, float Opacity);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SetLayerBlendMode(const FString& LayerName, EBrushMode BlendMode);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ToggleLayerVisibility(const FString& LayerName);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ToggleLayerLock(const FString& LayerName);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    TArray<FLayerInfo> GetLayers() const { return Layers; }

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    FLayerInfo GetActiveLayer() const;

    // ?ñÂ?Ë™øÊï¥
    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void AdjustBrightness(float Brightness);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void AdjustContrast(float Contrast);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void AdjustSaturation(float Saturation);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void AdjustHue(float Hue);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void AdjustGamma(float Gamma);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void InvertColors();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void Desaturate();

    // ÊøæÈè°?àÊ?
    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ApplyBlurFilter(float Radius);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ApplySharpenFilter(float Strength);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ApplyGaussianBlur(float Sigma);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ApplyEdgeDetection();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ApplyEmbossFilter();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ApplyNoiseFilter(float Strength);

    // ?∏Ê?Â∑•ÂÖ∑
    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SelectRectangular(const FVector2D& TopLeft, const FVector2D& BottomRight);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SelectElliptical(const FVector2D& Center, float RadiusX, float RadiusY);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SelectLasso(const TArray<FVector2D>& Points);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void SelectByColor(FLinearColor Color, float Tolerance = 0.1f);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ClearSelection();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void InvertSelection();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void CopySelection();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void PasteSelection();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void DeleteSelection();

    // ËÆäÊ?Â∑•ÂÖ∑
    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ScaleImage(float ScaleX, float ScaleY);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void RotateImage(float Angle);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void FlipImage(bool bHorizontal, bool bVertical);

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void CropImage(const FVector2D& TopLeft, const FVector2D& BottomRight);

    // Ê≠∑Âè≤Ë®òÈ?
    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void Undo();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void Redo();

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    bool CanUndo() const;

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    bool CanRedo() const;

    UFUNCTION(BlueprintCallable, Category = "Art Editor")
    void ClearHistory();

    // ÂßîÊ?‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Art Editor")
    FOnArtEdited OnArtEdited;

    UPROPERTY(BlueprintAssignable, Category = "Art Editor")
    FOnLayerChanged OnLayerChanged;

    UPROPERTY(BlueprintAssignable, Category = "Art Editor")
    FOnToolChanged OnToolChanged;

protected:
    UPROPERTY()
    UTexture2D* CurrentImage;

    UPROPERTY()
    EEditingTool CurrentTool;

    UPROPERTY()
    FBrushSettings BrushSettings;

    UPROPERTY()
    TArray<FLayerInfo> Layers;

    UPROPERTY()
    int32 ActiveLayerIndex;

    UPROPERTY()
    bool bIsDrawing;

    UPROPERTY()
    FVector2D LastDrawPosition;

    UPROPERTY()
    TArray<UTexture2D*> UndoHistory;

    UPROPERTY()
    TArray<UTexture2D*> RedoHistory;

    UPROPERTY()
    int32 MaxHistorySize;

private:
    void InitializeLayers();
    void UpdateActiveLayer();
    void SaveToHistory();
    void MergeLayers();
    void ApplyBrushStroke(const FVector2D& Position);
    void BlendLayers();
    void NotifyArtEdited();
    void NotifyLayerChanged(const FString& LayerName, int32 LayerIndex);
    void NotifyToolChanged(EEditingTool NewTool);
    UTexture2D* CreateLayerTexture(int32 Width, int32 Height);
    void ProcessSelection();
};
