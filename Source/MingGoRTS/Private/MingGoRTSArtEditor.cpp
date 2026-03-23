#include "MingGoRTSArtEditor.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "TextureResource.h"
#include "RenderingThread.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UMingGoRTSArtEditor::UMingGoRTSArtEditor()
    : CurrentImage(nullptr)
    , CurrentTool(EEditingTool::Brush)
    , ActiveLayerIndex(0)
    , bIsDrawing(false)
    , MaxHistorySize(50)
{
    InitializeLayers();
}

void UMingGoRTSArtEditor::LoadImage(UTexture2D* Image)
{
    if (!Image)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid image to load"));
        return;
    }

    CurrentImage = Image;
    
    // ?µÂª∫?åÊôØ?ñÂ±§
    if (Layers.Num() > 0)
    {
        Layers[0].LayerTexture = Image;
        Layers[0].LayerName = TEXT("Background");
    }
    
    SaveToHistory();
    
    UE_LOG(LogTemp, Log, TEXT("Loaded image: %dx%d"), Image->GetSizeX(), Image->GetSizeY());
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::SaveImage(const FString& FilePath)
{
    if (!CurrentImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("No image to save"));
        return;
    }

    // ?à‰Ωµ?Ä?âÂ?Â±?
    MergeLayers();
    
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??ñÂ?‰øùÂ??èËºØ
    // Á∞°Â??àÔ??™ÊòØË®òÈ??á‰ª∂Ë∑ØÂ?
    UE_LOG(LogTemp, Log, TEXT("Saving image to: %s"), *FilePath);
    
    // ÂØ¶È?ÂØ¶‰??ÄË¶ÅÂ?Á¥ãÁ??∏Ê?‰øùÂ??∫PNG?ñJPG?á‰ª∂
}

UTexture2D* UMingGoRTSArtEditor::GetCurrentImage() const
{
    return CurrentImage;
}

void UMingGoRTSArtEditor::CreateNewImage(int32 Width, int32 Height, FLinearColor BackgroundColor)
{
    // ?µÂª∫?∞Â???
    CurrentImage = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
    
    if (CurrentImage)
    {
        // ?ùÂ??ñÂ?Â±?
        InitializeLayers();
        
        // Ë®≠ÁΩÆ?åÊôØ?ñÂ±§
        if (Layers.Num() > 0)
        {
            Layers[0].LayerTexture = CreateLayerTexture(Width, Height);
            Layers[0].LayerName = TEXT("Background");
        }
        
        SaveToHistory();
        
        UE_LOG(LogTemp, Log, TEXT("Created new image: %dx%d"), Width, Height);
        NotifyArtEdited();
    }
}

void UMingGoRTSArtEditor::SetEditingTool(EEditingTool Tool)
{
    CurrentTool = Tool;
    NotifyToolChanged(Tool);
    
    UE_LOG(LogTemp, Log, TEXT("Set editing tool: %d"), (int32)Tool);
}

EEditingTool UMingGoRTSArtEditor::GetCurrentTool() const
{
    return CurrentTool;
}

void UMingGoRTSArtEditor::SetBrushSettings(const FBrushSettings& Settings)
{
    BrushSettings = Settings;
    
    UE_LOG(LogTemp, Log, TEXT("Updated brush settings - Size: %.1f, Opacity: %.2f"), 
        Settings.Size, Settings.Opacity);
}

FBrushSettings UMingGoRTSArtEditor::GetBrushSettings() const
{
    return BrushSettings;
}

void UMingGoRTSArtEditor::StartStroke(const FVector2D& Position)
{
    bIsDrawing = true;
    LastDrawPosition = Position;
    
    ApplyBrushStroke(Position);
    
    UE_LOG(LogTemp, Log, TEXT("Started stroke at position: (%.1f, %.1f)"), Position.X, Position.Y);
}

void UMingGoRTSArtEditor::ContinueStroke(const FVector2D& Position)
{
    if (!bIsDrawing)
    {
        return;
    }
    
    // Áπ™Ë£ΩÂæû‰?‰∏Ä‰ΩçÁΩÆ?∞Áï∂?ç‰?ÁΩÆÁ?Á∑öÊ?
    DrawLine(LastDrawPosition, Position);
    
    LastDrawPosition = Position;
}

void UMingGoRTSArtEditor::EndStroke()
{
    if (bIsDrawing)
    {
        bIsDrawing = false;
        SaveToHistory();
        NotifyArtEdited();
        
        UE_LOG(LogTemp, Log, TEXT("Ended stroke"));
    }
}

void UMingGoRTSArtEditor::DrawLine(const FVector2D& StartPos, const FVector2D& EndPos)
{
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Á∑öÊ?Áπ™Ë£Ω?èËºØ
    // Á∞°Â??àÔ??™ÊòØË®òÈ??ç‰?
    
    UE_LOG(LogTemp, Log, TEXT("Drawing line from (%.1f, %.1f) to (%.1f, %.1f)"), 
        StartPos.X, StartPos.Y, EndPos.X, EndPos.Y);
    
    ApplyBrushStroke(EndPos);
}

void UMingGoRTSArtEditor::DrawRectangle(const FVector2D& TopLeft, const FVector2D& BottomRight)
{
    UE_LOG(LogTemp, Log, TEXT("Drawing rectangle from (%.1f, %.1f) to (%.1f, %.1f)"), 
        TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y);
    
    // Áπ™Ë£Ω?©ÂΩ¢?õÊ???
    DrawLine(TopLeft, FVector2D(BottomRight.X, TopLeft.Y));
    DrawLine(FVector2D(BottomRight.X, TopLeft.Y), BottomRight);
    DrawLine(BottomRight, FVector2D(TopLeft.X, BottomRight.Y));
    DrawLine(FVector2D(TopLeft.X, BottomRight.Y), TopLeft);
}

void UMingGoRTSArtEditor::DrawCircle(const FVector2D& Center, float Radius)
{
    UE_LOG(LogTemp, Log, TEXT("Drawing circle at (%.1f, %.1f) with radius %.1f"), 
        Center.X, Center.Y, Radius);
    
    // Á∞°Â??ÑÂ?ÂΩ¢Áπ™Ë£?- ‰ΩøÁî®Â§öÈ?ÂΩ¢Ë?‰º?
    const int32 NumSegments = 32;
    TArray<FVector2D> Points;
    
    for (int32 i = 0; i <= NumSegments; ++i)
    {
        float Angle = 2.0f * PI * i / NumSegments;
        FVector2D Point = Center + FVector2D(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius);
        Points.Add(Point);
    }
    
    // ??é•?Ä?âÈ?ÂΩ¢Ê??ìÂΩ¢
    for (int32 i = 0; i < Points.Num() - 1; ++i)
    {
        DrawLine(Points[i], Points[i + 1]);
    }
}

void UMingGoRTSArtEditor::FillArea(const FVector2D& Position, FLinearColor FillColor)
{
    UE_LOG(LogTemp, Log, TEXT("Filling area at (%.1f, %.1f) with color (%.2f, %.2f, %.2f)"), 
        Position.X, Position.Y, FillColor.R, FillColor.G, FillColor.B);
    
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Â°´Â?ÁÆóÊ?ÔºàÊ¥™Ê∞¥Â°´?ÖÔ?
    // Á∞°Â??àÔ??™ÊòØË®òÈ??ç‰?
}

void UMingGoRTSArtEditor::AddLayer(const FString& LayerName)
{
    if (!CurrentImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("No image loaded, cannot add layer"));
        return;
    }
    
    FLayerInfo NewLayer;
    NewLayer.LayerName = LayerName;
    NewLayer.LayerTexture = CreateLayerTexture(CurrentImage->GetSizeX(), CurrentImage->GetSizeY());
    NewLayer.LayerIndex = Layers.Num();
    NewLayer.Opacity = 1.0f;
    NewLayer.BlendMode = EBrushMode::Normal;
    NewLayer.bVisible = true;
    NewLayer.bLocked = false;
    
    Layers.Add(NewLayer);
    ActiveLayerIndex = Layers.Num() - 1;
    
    NotifyLayerChanged(LayerName, ActiveLayerIndex);
    
    UE_LOG(LogTemp, Log, TEXT("Added layer: %s"), *LayerName);
}

void UMingGoRTSArtEditor::RemoveLayer(const FString& LayerName)
{
    for (int32 i = 0; i < Layers.Num(); ++i)
    {
        if (Layers[i].LayerName == LayerName)
        {
            Layers.RemoveAt(i);
            
            // Ë™øÊï¥Ê¥ªÂ??ñÂ±§Á¥¢Â?
            if (ActiveLayerIndex >= Layers.Num())
            {
                ActiveLayerIndex = Layers.Num() - 1;
            }
            
            NotifyLayerChanged(LayerName, -1);
            
            UE_LOG(LogTemp, Log, TEXT("Removed layer: %s"), *LayerName);
            return;
        }
    }
}

void UMingGoRTSArtEditor::SelectLayer(const FString& LayerName)
{
    for (int32 i = 0; i < Layers.Num(); ++i)
    {
        if (Layers[i].LayerName == LayerName)
        {
            ActiveLayerIndex = i;
            NotifyLayerChanged(LayerName, i);
            
            UE_LOG(LogTemp, Log, TEXT("Selected layer: %s"), *LayerName);
            return;
        }
    }
}

void UMingGoRTSArtEditor::MoveLayerUp(const FString& LayerName)
{
    for (int32 i = 0; i < Layers.Num() - 1; ++i)
    {
        if (Layers[i].LayerName == LayerName)
        {
            // ‰∫§Ê??ñÂ±§
            Layers.Swap(i, i + 1);
            
            // ?¥Êñ∞Á¥¢Â?
            Layers[i].LayerIndex = i;
            Layers[i + 1].LayerIndex = i + 1;
            
            // ?¥Êñ∞Ê¥ªÂ??ñÂ±§
            if (ActiveLayerIndex == i)
            {
                ActiveLayerIndex = i + 1;
            }
            else if (ActiveLayerIndex == i + 1)
            {
                ActiveLayerIndex = i;
            }
            
            NotifyLayerChanged(LayerName, i + 1);
            
            UE_LOG(LogTemp, Log, TEXT("Moved layer up: %s"), *LayerName);
            return;
        }
    }
}

void UMingGoRTSArtEditor::MoveLayerDown(const FString& LayerName)
{
    for (int32 i = 1; i < Layers.Num(); ++i)
    {
        if (Layers[i].LayerName == LayerName)
        {
            // ‰∫§Ê??ñÂ±§
            Layers.Swap(i, i - 1);
            
            // ?¥Êñ∞Á¥¢Â?
            Layers[i].LayerIndex = i;
            Layers[i - 1].LayerIndex = i - 1;
            
            // ?¥Êñ∞Ê¥ªÂ??ñÂ±§
            if (ActiveLayerIndex == i)
            {
                ActiveLayerIndex = i - 1;
            }
            else if (ActiveLayerIndex == i - 1)
            {
                ActiveLayerIndex = i;
            }
            
            NotifyLayerChanged(LayerName, i - 1);
            
            UE_LOG(LogTemp, Log, TEXT("Moved layer down: %s"), *LayerName);
            return;
        }
    }
}

void UMingGoRTSArtEditor::SetLayerOpacity(const FString& LayerName, float Opacity)
{
    for (FLayerInfo& Layer : Layers)
    {
        if (Layer.LayerName == LayerName)
        {
            Layer.Opacity = FMath::Clamp(Opacity, 0.0f, 1.0f);
            
            UE_LOG(LogTemp, Log, TEXT("Set layer %s opacity to %.2f"), *LayerName, Opacity);
            NotifyArtEdited();
            return;
        }
    }
}

void UMingGoRTSArtEditor::SetLayerBlendMode(const FString& LayerName, EBrushMode BlendMode)
{
    for (FLayerInfo& Layer : Layers)
    {
        if (Layer.LayerName == LayerName)
        {
            Layer.BlendMode = BlendMode;
            
            UE_LOG(LogTemp, Log, TEXT("Set layer %s blend mode to %d"), *LayerName, (int32)BlendMode);
            NotifyArtEdited();
            return;
        }
    }
}

void UMingGoRTSArtEditor::ToggleLayerVisibility(const FString& LayerName)
{
    for (FLayerInfo& Layer : Layers)
    {
        if (Layer.LayerName == LayerName)
        {
            Layer.bVisible = !Layer.bVisible;
            
            UE_LOG(LogTemp, Log, TEXT("Toggled layer %s visibility to %s"), 
                *LayerName, Layer.bVisible ? TEXT("visible") : TEXT("hidden"));
            NotifyArtEdited();
            return;
        }
    }
}

void UMingGoRTSArtEditor::ToggleLayerLock(const FString& LayerName)
{
    for (FLayerInfo& Layer : Layers)
    {
        if (Layer.LayerName == LayerName)
        {
            Layer.bLocked = !Layer.bLocked;
            
            UE_LOG(LogTemp, Log, TEXT("Toggled layer %s lock to %s"), 
                *LayerName, Layer.bLocked ? TEXT("locked") : TEXT("unlocked"));
            return;
        }
    }
}

TArray<FLayerInfo> UMingGoRTSArtEditor::GetLayers() const
{
    return Layers;
}

FLayerInfo* UMingGoRTSArtEditor::GetActiveLayer()
{
    if (ActiveLayerIndex >= 0 && ActiveLayerIndex < Layers.Num())
    {
        return &Layers[ActiveLayerIndex];
    }
    return nullptr;
}

void UMingGoRTSArtEditor::AdjustBrightness(float Brightness)
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting brightness by %.2f"), Brightness);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?‰∫ÆÂ∫¶Ë™øÊï¥?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::AdjustContrast(float Contrast)
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting contrast by %.2f"), Contrast);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Â∞çÊ?Â∫¶Ë™ø?¥È?Ëº?
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::AdjustSaturation(float Saturation)
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting saturation by %.2f"), Saturation);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?È£ΩÂ?Â∫¶Ë™ø?¥È?Ëº?
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::AdjustHue(float Hue)
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting hue by %.2f"), Hue);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??≤Áõ∏Ë™øÊï¥?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::AdjustGamma(float Gamma)
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting gamma by %.2f"), Gamma);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?‰ºΩÈ¶¨Ë™øÊï¥?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::InvertColors()
{
    UE_LOG(LogTemp, Log, TEXT("Inverting colors"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?È°èËâ≤?çË??èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::Desaturate()
{
    UE_LOG(LogTemp, Log, TEXT("Desaturating image"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??ªÈ£Ω?åÈ?Ëº?
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyBlurFilter(float Radius)
{
    UE_LOG(LogTemp, Log, TEXT("Applying blur filter with radius %.2f"), Radius);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Ê®°Á?ÊøæÈè°?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplySharpenFilter(float Strength)
{
    UE_LOG(LogTemp, Log, TEXT("Applying sharpen filter with strength %.2f"), Strength);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??≥Â?ÊøæÈè°?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyGaussianBlur(float Sigma)
{
    UE_LOG(LogTemp, Log, TEXT("Applying Gaussian blur with sigma %.2f"), Sigma);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?È´òÊñØÊ®°Á??èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyEdgeDetection()
{
    UE_LOG(LogTemp, Log, TEXT("Applying edge detection filter"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??äÁ∑£Ê™¢Ê∏¨?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyEmbossFilter()
{
    UE_LOG(LogTemp, Log, TEXT("Applying emboss filter"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?ÊµÆÈ?ÊøæÈè°?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyNoiseFilter(float Strength)
{
    UE_LOG(LogTemp, Log, TEXT("Applying noise filter with strength %.2f"), Strength);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??™È?ÊøæÈè°?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::SelectRectangular(const FVector2D& TopLeft, const FVector2D& BottomRight)
{
    UE_LOG(LogTemp, Log, TEXT("Rectangular selection from (%.1f, %.1f) to (%.1f, %.1f)"), 
        TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??©ÂΩ¢?∏Ê??èËºØ
}

void UMingGoRTSArtEditor::SelectElliptical(const FVector2D& Center, float RadiusX, float RadiusY)
{
    UE_LOG(LogTemp, Log, TEXT("Elliptical selection at (%.1f, %.1f) with radii %.1f, %.1f"), 
        Center.X, Center.Y, RadiusX, RadiusY);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Ê©¢Â??∏Ê??èËºØ
}

void UMingGoRTSArtEditor::SelectLasso(const TArray<FVector2D>& Points)
{
    UE_LOG(LogTemp, Log, TEXT("Lasso selection with %d points"), Points.Num());
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Â•óÁ¥¢?∏Ê??èËºØ
}

void UMingGoRTSArtEditor::SelectByColor(FLinearColor Color, float Tolerance)
{
    UE_LOG(LogTemp, Log, TEXT("Color selection with tolerance %.2f"), Tolerance);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?È°èËâ≤?∏Ê??èËºØ
}

void UMingGoRTSArtEditor::ClearSelection()
{
    UE_LOG(LogTemp, Log, TEXT("Clearing selection"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Ê∏ÖÈô§?∏Ê??èËºØ
}

void UMingGoRTSArtEditor::InvertSelection()
{
    UE_LOG(LogTemp, Log, TEXT("Inverting selection"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??çË??∏Ê??èËºØ
}

void UMingGoRTSArtEditor::CopySelection()
{
    UE_LOG(LogTemp, Log, TEXT("Copying selection"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Ë§áË£Ω?∏Ê??èËºØ
}

void UMingGoRTSArtEditor::PasteSelection()
{
    UE_LOG(LogTemp, Log, TEXT("Pasting selection"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Ë≤º‰??∏Ê??èËºØ
}

void UMingGoRTSArtEditor::DeleteSelection()
{
    UE_LOG(LogTemp, Log, TEXT("Deleting selection"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??™Èô§?∏Ê??èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ScaleImage(float ScaleX, float ScaleY)
{
    UE_LOG(LogTemp, Log, TEXT("Scaling image by (%.2f, %.2f)"), ScaleX, ScaleY);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Á∏ÆÊîæ?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::RotateImage(float Angle)
{
    UE_LOG(LogTemp, Log, TEXT("Rotating image by %.2f degrees"), Angle);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??ãË??èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::FlipImage(bool bHorizontal, bool bVertical)
{
    UE_LOG(LogTemp, Log, TEXT("Flipping image (H:%s, V:%s)"), 
        bHorizontal ? TEXT("true") : TEXT("false"), bVertical ? TEXT("true") : TEXT("false"));
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?ÁøªË??èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::CropImage(const FVector2D& TopLeft, const FVector2D& BottomRight)
{
    UE_LOG(LogTemp, Log, TEXT("Cropping image from (%.1f, %.1f) to (%.1f, %.1f)"), 
        TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y);
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ?Ë£ÅÂâ™?èËºØ
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::Undo()
{
    if (CanUndo())
    {
        UE_LOG(LogTemp, Log, TEXT("Undoing last operation"));
        // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??§Èä∑?èËºØ
        NotifyArtEdited();
    }
}

void UMingGoRTSArtEditor::Redo()
{
    if (CanRedo())
    {
        UE_LOG(LogTemp, Log, TEXT("Redoing last operation"));
        // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??çÂ??èËºØ
        NotifyArtEdited();
    }
}

bool UMingGoRTSArtEditor::CanUndo() const
{
    return UndoHistory.Num() > 0;
}

bool UMingGoRTSArtEditor::CanRedo() const
{
    return RedoHistory.Num() > 0;
}

void UMingGoRTSArtEditor::ClearHistory()
{
    UndoHistory.Empty();
    RedoHistory.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Cleared editing history"));
}

void UMingGoRTSArtEditor::InitializeLayers()
{
    Layers.Empty();
    
    // ?µÂª∫?åÊôØ?ñÂ±§
    FLayerInfo BackgroundLayer;
    BackgroundLayer.LayerName = TEXT("Background");
    BackgroundLayer.LayerTexture = nullptr;
    BackgroundLayer.LayerIndex = 0;
    BackgroundLayer.Opacity = 1.0f;
    BackgroundLayer.BlendMode = EBrushMode::Normal;
    BackgroundLayer.bVisible = true;
    BackgroundLayer.bLocked = false;
    
    Layers.Add(BackgroundLayer);
    ActiveLayerIndex = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Initialized layers system"));
}

void UMingGoRTSArtEditor::UpdateActiveLayer()
{
    // Á¢∫‰?Ê¥ªÂ??ñÂ±§Á¥¢Â??âÊ?
    if (ActiveLayerIndex < 0 || ActiveLayerIndex >= Layers.Num())
    {
        ActiveLayerIndex = 0;
    }
}

void UMingGoRTSArtEditor::SaveToHistory()
{
    if (!CurrentImage)
    {
        return;
    }
    
    // Ê∑ªÂ??∞Êí§?∑Ê≠∑??
    UndoHistory.Add(CurrentImage);
    
    // ?êÂà∂Ê≠∑Âè≤Â§ßÂ?
    if (UndoHistory.Num() > MaxHistorySize)
    {
        UndoHistory.RemoveAt(0);
    }
    
    // Ê∏ÖÈô§?çÂ?Ê≠∑Âè≤
    RedoHistory.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Saved to history (Undo count: %d)"), UndoHistory.Num());
}

void UMingGoRTSArtEditor::MergeLayers()
{
    if (Layers.Num() <= 1)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Merging %d layers"), Layers.Num());
    
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??ñÂ±§?à‰Ωµ?èËºØ
    // Á∞°Â??àÔ??™ÊòØË®òÈ??ç‰?
}

void UMingGoRTSArtEditor::ApplyBrushStroke(const FVector2D& Position)
{
    FLayerInfo* ActiveLayer = GetActiveLayer();
    if (!ActiveLayer || ActiveLayer->bLocked)
    {
        return;
    }
    
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??´Á??âÁî®?èËºØ
    // Á∞°Â??àÔ??™ÊòØË®òÈ??ç‰?
    
    UE_LOG(LogTemp, Log, TEXT("Applied brush stroke at (%.1f, %.1f)"), Position.X, Position.Y);
}

void UMingGoRTSArtEditor::BlendLayers()
{
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??ñÂ±§Ê∑∑Â??èËºØ
    UE_LOG(LogTemp, Log, TEXT("Blending layers"));
}

void UMingGoRTSArtEditor::NotifyArtEdited()
{
    OnArtEdited.Broadcast(CurrentImage);
}

void UMingGoRTSArtEditor::NotifyLayerChanged(const FString& LayerName, int32 LayerIndex)
{
    OnLayerChanged.Broadcast(LayerName, LayerIndex);
}

void UMingGoRTSArtEditor::NotifyToolChanged(EEditingTool NewTool)
{
    OnToolChanged.Broadcast(NewTool);
}

UTexture2D* UMingGoRTSArtEditor::CreateLayerTexture(int32 Width, int32 Height)
{
    return UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
}

void UMingGoRTSArtEditor::ProcessSelection()
{
    // ?ôË£°?ÄË¶ÅÂØ¶?õÁ??∏Ê??ïÁ??èËºØ
    UE_LOG(LogTemp, Log, TEXT("Processing selection"));
}
