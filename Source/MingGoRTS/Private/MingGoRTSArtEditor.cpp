#include "MingGoRTSArtEditor.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
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
    
    // 創建背景圖層
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

    // 合併所有圖層
    MergeLayers();
    
    // 這裡需要實際的圖像保存邏輯
    // 簡化版本：只是記錄文件路徑
    UE_LOG(LogTemp, Log, TEXT("Saving image to: %s"), *FilePath);
    
    // 實際實作需要將紋理數據保存為PNG或JPG文件
}

UTexture2D* UMingGoRTSArtEditor::GetCurrentImage() const
{
    return CurrentImage;
}

void UMingGoRTSArtEditor::CreateNewImage(int32 Width, int32 Height, FLinearColor BackgroundColor)
{
    // 創建新圖像
    CurrentImage = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
    
    if (CurrentImage)
    {
        // 初始化圖層
        InitializeLayers();
        
        // 設置背景圖層
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
    
    // 繪製從上一位置到當前位置的線條
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
    // 這裡需要實際的線條繪製邏輯
    // 簡化版本：只是記錄操作
    
    UE_LOG(LogTemp, Log, TEXT("Drawing line from (%.1f, %.1f) to (%.1f, %.1f)"), 
        StartPos.X, StartPos.Y, EndPos.X, EndPos.Y);
    
    ApplyBrushStroke(EndPos);
}

void UMingGoRTSArtEditor::DrawRectangle(const FVector2D& TopLeft, const FVector2D& BottomRight)
{
    UE_LOG(LogTemp, Log, TEXT("Drawing rectangle from (%.1f, %.1f) to (%.1f, %.1f)"), 
        TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y);
    
    // 繪製矩形四條邊
    DrawLine(TopLeft, FVector2D(BottomRight.X, TopLeft.Y));
    DrawLine(FVector2D(BottomRight.X, TopLeft.Y), BottomRight);
    DrawLine(BottomRight, FVector2D(TopLeft.X, BottomRight.Y));
    DrawLine(FVector2D(TopLeft.X, BottomRight.Y), TopLeft);
}

void UMingGoRTSArtEditor::DrawCircle(const FVector2D& Center, float Radius)
{
    UE_LOG(LogTemp, Log, TEXT("Drawing circle at (%.1f, %.1f) with radius %.1f"), 
        Center.X, Center.Y, Radius);
    
    // 簡化的圓形繪製 - 使用多邊形近似
    const int32 NumSegments = 32;
    TArray<FVector2D> Points;
    
    for (int32 i = 0; i <= NumSegments; ++i)
    {
        float Angle = 2.0f * PI * i / NumSegments;
        FVector2D Point = Center + FVector2D(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius);
        Points.Add(Point);
    }
    
    // 連接所有點形成圓形
    for (int32 i = 0; i < Points.Num() - 1; ++i)
    {
        DrawLine(Points[i], Points[i + 1]);
    }
}

void UMingGoRTSArtEditor::FillArea(const FVector2D& Position, FLinearColor FillColor)
{
    UE_LOG(LogTemp, Log, TEXT("Filling area at (%.1f, %.1f) with color (%.2f, %.2f, %.2f)"), 
        Position.X, Position.Y, FillColor.R, FillColor.G, FillColor.B);
    
    // 這裡需要實際的填充算法（洪水填充）
    // 簡化版本：只是記錄操作
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
            
            // 調整活動圖層索引
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
            // 交換圖層
            Layers.Swap(i, i + 1);
            
            // 更新索引
            Layers[i].LayerIndex = i;
            Layers[i + 1].LayerIndex = i + 1;
            
            // 更新活動圖層
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
            // 交換圖層
            Layers.Swap(i, i - 1);
            
            // 更新索引
            Layers[i].LayerIndex = i;
            Layers[i - 1].LayerIndex = i - 1;
            
            // 更新活動圖層
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
    // 這裡需要實際的亮度調整邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::AdjustContrast(float Contrast)
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting contrast by %.2f"), Contrast);
    // 這裡需要實際的對比度調整邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::AdjustSaturation(float Saturation)
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting saturation by %.2f"), Saturation);
    // 這裡需要實際的飽和度調整邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::AdjustHue(float Hue)
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting hue by %.2f"), Hue);
    // 這裡需要實際的色相調整邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::AdjustGamma(float Gamma)
{
    UE_LOG(LogTemp, Log, TEXT("Adjusting gamma by %.2f"), Gamma);
    // 這裡需要實際的伽馬調整邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::InvertColors()
{
    UE_LOG(LogTemp, Log, TEXT("Inverting colors"));
    // 這裡需要實際的顏色反轉邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::Desaturate()
{
    UE_LOG(LogTemp, Log, TEXT("Desaturating image"));
    // 這裡需要實際的去飽和邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyBlurFilter(float Radius)
{
    UE_LOG(LogTemp, Log, TEXT("Applying blur filter with radius %.2f"), Radius);
    // 這裡需要實際的模糊濾鏡邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplySharpenFilter(float Strength)
{
    UE_LOG(LogTemp, Log, TEXT("Applying sharpen filter with strength %.2f"), Strength);
    // 這裡需要實際的銳化濾鏡邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyGaussianBlur(float Sigma)
{
    UE_LOG(LogTemp, Log, TEXT("Applying Gaussian blur with sigma %.2f"), Sigma);
    // 這裡需要實際的高斯模糊邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyEdgeDetection()
{
    UE_LOG(LogTemp, Log, TEXT("Applying edge detection filter"));
    // 這裡需要實際的邊緣檢測邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyEmbossFilter()
{
    UE_LOG(LogTemp, Log, TEXT("Applying emboss filter"));
    // 這裡需要實際的浮雕濾鏡邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ApplyNoiseFilter(float Strength)
{
    UE_LOG(LogTemp, Log, TEXT("Applying noise filter with strength %.2f"), Strength);
    // 這裡需要實際的噪點濾鏡邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::SelectRectangular(const FVector2D& TopLeft, const FVector2D& BottomRight)
{
    UE_LOG(LogTemp, Log, TEXT("Rectangular selection from (%.1f, %.1f) to (%.1f, %.1f)"), 
        TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y);
    // 這裡需要實際的矩形選擇邏輯
}

void UMingGoRTSArtEditor::SelectElliptical(const FVector2D& Center, float RadiusX, float RadiusY)
{
    UE_LOG(LogTemp, Log, TEXT("Elliptical selection at (%.1f, %.1f) with radii %.1f, %.1f"), 
        Center.X, Center.Y, RadiusX, RadiusY);
    // 這裡需要實際的橢圓選擇邏輯
}

void UMingGoRTSArtEditor::SelectLasso(const TArray<FVector2D>& Points)
{
    UE_LOG(LogTemp, Log, TEXT("Lasso selection with %d points"), Points.Num());
    // 這裡需要實際的套索選擇邏輯
}

void UMingGoRTSArtEditor::SelectByColor(FLinearColor Color, float Tolerance)
{
    UE_LOG(LogTemp, Log, TEXT("Color selection with tolerance %.2f"), Tolerance);
    // 這裡需要實際的顏色選擇邏輯
}

void UMingGoRTSArtEditor::ClearSelection()
{
    UE_LOG(LogTemp, Log, TEXT("Clearing selection"));
    // 這裡需要實際的清除選擇邏輯
}

void UMingGoRTSArtEditor::InvertSelection()
{
    UE_LOG(LogTemp, Log, TEXT("Inverting selection"));
    // 這裡需要實際的反轉選擇邏輯
}

void UMingGoRTSArtEditor::CopySelection()
{
    UE_LOG(LogTemp, Log, TEXT("Copying selection"));
    // 這裡需要實際的複製選擇邏輯
}

void UMingGoRTSArtEditor::PasteSelection()
{
    UE_LOG(LogTemp, Log, TEXT("Pasting selection"));
    // 這裡需要實際的貼上選擇邏輯
}

void UMingGoRTSArtEditor::DeleteSelection()
{
    UE_LOG(LogTemp, Log, TEXT("Deleting selection"));
    // 這裡需要實際的刪除選擇邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::ScaleImage(float ScaleX, float ScaleY)
{
    UE_LOG(LogTemp, Log, TEXT("Scaling image by (%.2f, %.2f)"), ScaleX, ScaleY);
    // 這裡需要實際的縮放邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::RotateImage(float Angle)
{
    UE_LOG(LogTemp, Log, TEXT("Rotating image by %.2f degrees"), Angle);
    // 這裡需要實際的旋轉邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::FlipImage(bool bHorizontal, bool bVertical)
{
    UE_LOG(LogTemp, Log, TEXT("Flipping image (H:%s, V:%s)"), 
        bHorizontal ? TEXT("true") : TEXT("false"), bVertical ? TEXT("true") : TEXT("false"));
    // 這裡需要實際的翻轉邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::CropImage(const FVector2D& TopLeft, const FVector2D& BottomRight)
{
    UE_LOG(LogTemp, Log, TEXT("Cropping image from (%.1f, %.1f) to (%.1f, %.1f)"), 
        TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y);
    // 這裡需要實際的裁剪邏輯
    NotifyArtEdited();
}

void UMingGoRTSArtEditor::Undo()
{
    if (CanUndo())
    {
        UE_LOG(LogTemp, Log, TEXT("Undoing last operation"));
        // 這裡需要實際的撤銷邏輯
        NotifyArtEdited();
    }
}

void UMingGoRTSArtEditor::Redo()
{
    if (CanRedo())
    {
        UE_LOG(LogTemp, Log, TEXT("Redoing last operation"));
        // 這裡需要實際的重做邏輯
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
    
    // 創建背景圖層
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
    // 確保活動圖層索引有效
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
    
    // 添加到撤銷歷史
    UndoHistory.Add(CurrentImage);
    
    // 限制歷史大小
    if (UndoHistory.Num() > MaxHistorySize)
    {
        UndoHistory.RemoveAt(0);
    }
    
    // 清除重做歷史
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
    
    // 這裡需要實際的圖層合併邏輯
    // 簡化版本：只是記錄操作
}

void UMingGoRTSArtEditor::ApplyBrushStroke(const FVector2D& Position)
{
    FLayerInfo* ActiveLayer = GetActiveLayer();
    if (!ActiveLayer || ActiveLayer->bLocked)
    {
        return;
    }
    
    // 這裡需要實際的畫筆應用邏輯
    // 簡化版本：只是記錄操作
    
    UE_LOG(LogTemp, Log, TEXT("Applied brush stroke at (%.1f, %.1f)"), Position.X, Position.Y);
}

void UMingGoRTSArtEditor::BlendLayers()
{
    // 這裡需要實際的圖層混合邏輯
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
    // 這裡需要實際的選擇處理邏輯
    UE_LOG(LogTemp, Log, TEXT("Processing selection"));
}
