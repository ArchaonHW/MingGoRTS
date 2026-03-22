#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/TextureRenderTarget2D.h"
#include "MingAIGameUIRenderer.generated.h"

UENUM(BlueprintType)
enum class EAIGameUIRenderMode : uint8
{
    ScreenSpace,        // 屏幕空間
    WorldSpace,         // 世界空間
    RenderTarget,       // 渲染目標
    VR,                 // VR模式
    MixedReality        // 混合現實
};

UENUM(BlueprintType)
enum class EAIGameUILayer : uint8
{
    Background,         // 背景層
    MainUI,             // 主UI層
    HUD,                // HUD層
    Popup,              // 彈窗層
    Overlay,            // 覆蓋層
    TopMost             // 最頂層
};

USTRUCT(BlueprintType)
struct FAIGameUIRenderConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    EAIGameUIRenderMode RenderMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    EAIGameUILayer UILayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    FVector2D ScreenPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    FVector2D ScreenSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    float Opacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    bool bAutoScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    bool bEnableInteraction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    float FadeInDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    float FadeOutDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Game UI Render")
    UTextureRenderTarget2D* TargetRenderTarget;

    FAIGameUIRenderConfig()
    {
        RenderMode = EAIGameUIRenderMode::ScreenSpace;
        UILayer = EAIGameUILayer::MainUI;
        ScreenPosition = FVector2D(0.5f, 0.5f);
        ScreenSize = FVector2D(1.0f, 1.0f);
        Opacity = 1.0f;
        bAutoScale = true;
        bEnableInteraction = true;
        FadeInDuration = 0.5f;
        FadeOutDuration = 0.3f;
        TargetRenderTarget = nullptr;
    }
};

USTRUCT(BlueprintType)
struct FAIRenderedGameUI
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Rendered UI")
    FString RenderID;

    UPROPERTY(BlueprintReadOnly, Category = "AI Rendered UI")
    UUserWidget* WidgetInstance;

    UPROPERTY(BlueprintReadOnly, Category = "AI Rendered UI")
    UTextureRenderTarget2D* RenderTexture;

    UPROPERTY(BlueprintReadOnly, Category = "AI Rendered UI")
    FAIGameUIRenderConfig RenderConfig;

    UPROPERTY(BlueprintReadOnly, Category = "AI Rendered UI")
    float CurrentOpacity;

    UPROPERTY(BlueprintReadOnly, Category = "AI Rendered UI")
    bool bIsVisible;

    UPROPERTY(BlueprintReadOnly, Category = "AI Rendered UI")
    float TimeSinceCreated;

    FAIRenderedGameUI()
    {
        RenderID = TEXT("");
        WidgetInstance = nullptr;
        RenderTexture = nullptr;
        CurrentOpacity = 1.0f;
        bIsVisible = true;
        TimeSinceCreated = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIRenderedToGame, const FAIRenderedGameUI&, RenderedUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUIOpacityChanged, const FString&, RenderID, float, NewOpacity);

/**
 * AI生成UI遊戲畫面渲染器
 * 將AI生成的UI渲染到遊戲畫面中
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAIGameUIRenderer : public UObject
{
    GENERATED_BODY()

public:
    UMingAIGameUIRenderer();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void InitializeRenderer(APlayerController* PlayerController);

    // 渲染UI到遊戲畫面
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    FAIRenderedGameUI RenderAIToGame(const FAIUIGeneratedWidget& AIWidget, const FAIGameUIRenderConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    FAIRenderedGameUI RenderAIToGameAsync(const FAIUIGeneratedWidget& AIWidget, const FAIGameUIRenderConfig& Config);

    // 多層次UI渲染
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void RenderMultiLayerUI(const TArray<FAIUIGeneratedWidget>& Widgets, const TArray<FAIGameUIRenderConfig>& Configs);

    // 更新渲染
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void UpdateRenderedUI(const FString& RenderID, const FAIGameUIRenderConfig& NewConfig);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void UpdateUIOpacity(const FString& RenderID, float NewOpacity);

    // 顯示/隱藏
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void ShowUI(const FString& RenderID, bool bFadeIn = true);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void HideUI(const FString& RenderID, bool bFadeOut = true);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void ToggleUI(const FString& RenderID);

    // 移除渲染
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void RemoveRenderedUI(const FString& RenderID);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void RemoveAllRenderedUI();

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void RemoveUILayer(EAIGameUILayer Layer);

    // 獲取渲染狀態
    UFUNCTION(BlueprintPure, Category = "AI Game UI Render")
    bool IsUIRendered(const FString& RenderID) const;

    UFUNCTION(BlueprintPure, Category = "AI Game UI Render")
    FAIRenderedGameUI GetRenderedUI(const FString& RenderID) const;

    UFUNCTION(BlueprintPure, Category = "AI Game UI Render")
    TArray<FAIRenderedGameUI> GetAllRenderedUI() const;

    UFUNCTION(BlueprintPure, Category = "AI Game UI Render")
    TArray<FAIRenderedGameUI> GetRenderedUIByLayer(EAIGameUILayer Layer) const;

    // 層級管理
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetUILayerOrder(EAIGameUILayer Layer, int32 Order);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void BringUIToFront(const FString& RenderID);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SendUIToBack(const FString& RenderID);

    // 動畫效果
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void AnimateUIEntrance(const FString& RenderID, TEnumAsByte<EUMGSequencePlayMode::Type> PlayMode);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void AnimateUIExit(const FString& RenderID, TEnumAsByte<EUMGSequencePlayMode::Type> PlayMode);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void AnimateUIHover(const FString& RenderID, bool bIsHovering);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void AnimateUIPulse(const FString& RenderID);

    // 渲染目標操作
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    UTextureRenderTarget2D* CreateRenderTarget(int32 Width, int32 Height);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void CaptureWidgetToRenderTarget(UUserWidget* Widget, UTextureRenderTarget2D* RenderTarget);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void ApplyRenderTargetToMaterial(UTextureRenderTarget2D* RenderTarget, UMaterialInstanceDynamic* Material);

    // 屏幕空間操作
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetScreenSpacePosition(const FString& RenderID, FVector2D Position);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetScreenSpaceSize(const FString& RenderID, FVector2D Size);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void AnchorUIToScreenPosition(const FString& RenderID, FVector2D AnchorPoint);

    // 世界空間操作
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void AttachUIToWorldObject(const FString& RenderID, AActor* TargetActor, FVector Offset);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetUIWorldPosition(const FString& RenderID, FVector WorldPosition);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetUIWorldRotation(const FString& RenderID, FRotator WorldRotation);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void MakeUIBillboard(const FString& RenderID, bool bBillboard);

    // 交互控制
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void EnableUIInteraction(const FString& RenderID, bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetUIInputMode(const FString& RenderID, TEnumAsByte<EUMGInputMode::Type> InputMode);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void FocusUI(const FString& RenderID);

    // 性能優化
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetRenderQuality(const FString& RenderID, float Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetRenderDistance(const FString& RenderID, float MaxDistance);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void EnableOcclusionCulling(const FString& RenderID, bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void PauseRendering(const FString& RenderID);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void ResumeRendering(const FString& RenderID);

    // 實時更新
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void EnableRealTimeUpdate(const FString& RenderID, bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetUpdateInterval(const FString& RenderID, float Interval);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void ForceRedraw(const FString& RenderID);

    // 批量操作
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void ShowAllUILayer(EAIGameUILayer Layer);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void HideAllUILayer(EAIGameUILayer Layer);

    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void SetAllUIOpacity(EAIGameUILayer Layer, float Opacity);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "AI Game UI Events")
    FOnUIRenderedToGame OnUIRenderedToGame;

    UPROPERTY(BlueprintAssignable, Category = "AI Game UI Events")
    FOnUIOpacityChanged OnUIOpacityChanged;

    // 每幀更新
    UFUNCTION(BlueprintCallable, Category = "AI Game UI Render")
    void TickRenderer(float DeltaTime);

protected:
    UPROPERTY()
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY()
    TMap<FString, FAIRenderedGameUI> RenderedWidgets;

    UPROPERTY()
    TMap<EAIGameUILayer, int32> LayerOrders;

    UPROPERTY()
    TMap<FString, float> UpdateTimers;

    UPROPERTY()
    TMap<FString, bool> RealTimeUpdateEnabled;

    // 內部函數
    FAIRenderedGameUI CreateRenderedUIInternal(const FAIUIGeneratedWidget& AIWidget, const FAIGameUIRenderConfig& Config);
    void SetupScreenSpaceUI(FAIRenderedGameUI& RenderedUI, const FAIGameUIRenderConfig& Config);
    void SetupWorldSpaceUI(FAIRenderedGameUI& RenderedUI, const FAIGameUIRenderConfig& Config);
    void SetupRenderTargetUI(FAIRenderedGameUI& RenderedUI, const FAIGameUIRenderConfig& Config);
    void UpdateOpacityInternal(FAIRenderedGameUI& RenderedUI, float TargetOpacity, float DeltaTime);
    void CleanupRenderedUI(const FString& RenderID);
    void SortUIByLayer();
    void ApplyLayerOrder(EAIGameUILayer Layer);
    bool ShouldUpdateUI(const FString& RenderID, float DeltaTime);

private:
    void InitializeLayerOrders();
    FString GenerateRenderID();
    void LogRenderEvent(const FString& Event, const FString& RenderID);
};
