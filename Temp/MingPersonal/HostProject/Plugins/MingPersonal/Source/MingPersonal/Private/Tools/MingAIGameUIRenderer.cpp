#include "Tools/MingAIGameUIRenderer.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

UMingAIGameUIRenderer::UMingAIGameUIRenderer()
    : PlayerController(nullptr)
{
    InitializeLayerOrders();
}

void UMingAIGameUIRenderer::InitializeRenderer(APlayerController* InPlayerController)
{
    PlayerController = InPlayerController;
    UE_LOG(LogTemp, Log, TEXT("AI Game UI Renderer initialized"));
}

void UMingAIGameUIRenderer::InitializeLayerOrders()
{
    LayerOrders.Add(EAIGameUILayer::Background, 0);
    LayerOrders.Add(EAIGameUILayer::MainUI, 100);
    LayerOrders.Add(EAIGameUILayer::HUD, 200);
    LayerOrders.Add(EAIGameUILayer::Popup, 300);
    LayerOrders.Add(EAIGameUILayer::Overlay, 400);
    LayerOrders.Add(EAIGameUILayer::TopMost, 500);
}

FAIRenderedGameUI UMingAIGameUIRenderer::RenderAIToGame(const FAIUIGeneratedWidget& AIWidget, const FAIGameUIRenderConfig& Config)
{
    return CreateRenderedUIInternal(AIWidget, Config);
}

FAIRenderedGameUI UMingAIGameUIRenderer::RenderAIToGameAsync(const FAIUIGeneratedWidget& AIWidget, const FAIGameUIRenderConfig& Config)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, AIWidget, Config]()
    {
        FAIRenderedGameUI RenderedUI = CreateRenderedUIInternal(AIWidget, Config);
        
        AsyncTask(ENamedThreads::GameThread, [this, RenderedUI]()
        {
            OnUIRenderedToGame.Broadcast(RenderedUI);
        });
    });
    
    return FAIRenderedGameUI();
}

void UMingAIGameUIRenderer::RenderMultiLayerUI(const TArray<FAIUIGeneratedWidget>& Widgets, const TArray<FAIGameUIRenderConfig>& Configs)
{
    UE_LOG(LogTemp, Log, TEXT("Rendering %d UI layers"), Widgets.Num());
    
    for (int32 i = 0; i < Widgets.Num() && i < Configs.Num(); ++i)
    {
        FAIRenderedGameUI RenderedUI = CreateRenderedUIInternal(Widgets[i], Configs[i]);
        OnUIRenderedToGame.Broadcast(RenderedUI);
    }
}

void UMingAIGameUIRenderer::UpdateRenderedUI(const FString& RenderID, const FAIGameUIRenderConfig& NewConfig)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (!RenderedUI)
    {
        UE_LOG(LogTemp, Warning, TEXT("UI not found for update: %s"), *RenderID);
        return;
    }
    
    RenderedUI->RenderConfig = NewConfig;
    
    // Update position
    if (RenderedUI->WidgetInstance)
    {
        UWidgetComponent* WidgetComp = Cast<UWidgetComponent>(RenderedUI->WidgetInstance->GetOuter());
        if (WidgetComp)
        {
            switch (NewConfig.RenderMode)
            {
            case EAIGameUIRenderMode::ScreenSpace:
                SetupScreenSpaceUI(*RenderedUI, NewConfig);
                break;
            case EAIGameUIRenderMode::WorldSpace:
                SetupWorldSpaceUI(*RenderedUI, NewConfig);
                break;
            case EAIGameUIRenderMode::RenderTarget:
                SetupRenderTargetUI(*RenderedUI, NewConfig);
                break;
            default:
                break;
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Updated rendered UI: %s"), *RenderID);
}

void UMingAIGameUIRenderer::UpdateUIOpacity(const FString& RenderID, float NewOpacity)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI)
    {
        RenderedUI->CurrentOpacity = FMath::Clamp(NewOpacity, 0.0f, 1.0f);
        
        if (RenderedUI->WidgetInstance)
        {
            RenderedUI->WidgetInstance->SetRenderOpacity(RenderedUI->CurrentOpacity);
        }
        
        OnUIOpacityChanged.Broadcast(RenderID, RenderedUI->CurrentOpacity);
        UE_LOG(LogTemp, Log, TEXT("UI %s opacity updated to %.2f"), *RenderID, RenderedUI->CurrentOpacity);
    }
}

void UMingAIGameUIRenderer::ShowUI(const FString& RenderID, bool bFadeIn)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (!RenderedUI)
    {
        return;
    }
    
    RenderedUI->bIsVisible = true;
    
    if (RenderedUI->WidgetInstance)
    {
        RenderedUI->WidgetInstance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    
    if (bFadeIn)
    {
        // Start fade in animation
        UpdateUIOpacity(RenderID, 0.0f);
    }
    else
    {
        UpdateUIOpacity(RenderID, 1.0f);
    }
    
    UE_LOG(LogTemp, Log, TEXT("UI shown: %s"), *RenderID);
}

void UMingAIGameUIRenderer::HideUI(const FString& RenderID, bool bFadeOut)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (!RenderedUI)
    {
        return;
    }
    
    if (bFadeOut)
    {
        // Start fade out animation
        UpdateUIOpacity(RenderID, 0.0f);
        
        // Hide after fade completes
        FTimerHandle HideTimer;
        FTimerDelegate HideDelegate;
        HideDelegate.BindLambda([this, RenderID]()
        {
            FAIRenderedGameUI* UI = RenderedWidgets.Find(RenderID);
            if (UI)
            {
                UI->bIsVisible = false;
                if (UI->WidgetInstance)
                {
                    UI->WidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
                }
            }
        });
    }
    else
    {
        RenderedUI->bIsVisible = false;
        if (RenderedUI->WidgetInstance)
        {
            RenderedUI->WidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("UI hidden: %s"), *RenderID);
}

void UMingAIGameUIRenderer::ToggleUI(const FString& RenderID)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI)
    {
        if (RenderedUI->bIsVisible)
        {
            HideUI(RenderID, true);
        }
        else
        {
            ShowUI(RenderID, true);
        }
    }
}

void UMingAIGameUIRenderer::RemoveRenderedUI(const FString& RenderID)
{
    CleanupRenderedUI(RenderID);
    UE_LOG(LogTemp, Log, TEXT("Removed rendered UI: %s"), *RenderID);
}

void UMingAIGameUIRenderer::RemoveAllRenderedUI()
{
    TArray<FString> IDs;
    RenderedWidgets.GenerateKeyArray(IDs);
    
    for (const FString& ID : IDs)
    {
        CleanupRenderedUI(ID);
    }
    
    RenderedWidgets.Empty();
    UE_LOG(LogTemp, Log, TEXT("All rendered UI removed"));
}

void UMingAIGameUIRenderer::RemoveUILayer(EAIGameUILayer Layer)
{
    TArray<FString> IDsToRemove;
    
    for (const auto& Pair : RenderedWidgets)
    {
        if (Pair.Value.RenderConfig.UILayer == Layer)
        {
            IDsToRemove.Add(Pair.Key);
        }
    }
    
    for (const FString& ID : IDsToRemove)
    {
        CleanupRenderedUI(ID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Removed %d widgets from layer %d"), IDsToRemove.Num(), static_cast<int32>(Layer));
}

bool UMingAIGameUIRenderer::IsUIRendered(const FString& RenderID) const
{
    return RenderedWidgets.Contains(RenderID);
}

FAIRenderedGameUI UMingAIGameUIRenderer::GetRenderedUI(const FString& RenderID) const
{
    const FAIRenderedGameUI* Found = RenderedWidgets.Find(RenderID);
    if (Found)
    {
        return *Found;
    }
    return FAIRenderedGameUI();
}

TArray<FAIRenderedGameUI> UMingAIGameUIRenderer::GetAllRenderedUI() const
{
    TArray<FAIRenderedGameUI> Result;
    RenderedWidgets.GenerateValueArray(Result);
    return Result;
}

TArray<FAIRenderedGameUI> UMingAIGameUIRenderer::GetRenderedUIByLayer(EAIGameUILayer Layer) const
{
    TArray<FAIRenderedGameUI> Result;
    
    for (const auto& Pair : RenderedWidgets)
    {
        if (Pair.Value.RenderConfig.UILayer == Layer)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

void UMingAIGameUIRenderer::SetUILayerOrder(EAIGameUILayer Layer, int32 Order)
{
    LayerOrders.Add(Layer, Order);
    ApplyLayerOrder(Layer);
    UE_LOG(LogTemp, Log, TEXT("Layer %d order set to %d"), static_cast<int32>(Layer), Order);
}

void UMingAIGameUIRenderer::BringUIToFront(const FString& RenderID)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        UPanelWidget* Parent = Cast<UPanelWidget>(RenderedUI->WidgetInstance->GetParent());
        if (Parent)
        {
            UWidget* Widget = RenderedUI->WidgetInstance;
            Parent->RemoveChild(Widget);
            Parent->AddChild(Widget);
        }
    }
}

void UMingAIGameUIRenderer::SendUIToBack(const FString& RenderID)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        UPanelWidget* Parent = Cast<UPanelWidget>(RenderedUI->WidgetInstance->GetParent());
        if (Parent)
        {
            UWidget* Widget = RenderedUI->WidgetInstance;
            Parent->RemoveChild(Widget);
            Parent->InsertChildAt(0, Widget);
        }
    }
}

void UMingAIGameUIRenderer::AnimateUIEntrance(const FString& RenderID, TEnumAsByte<EUMGSequencePlayMode::Type> PlayMode)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        UWidgetAnimation* Animation = RenderedUI->WidgetInstance->GetAnimationByName(TEXT("Entrance"));
        if (Animation)
        {
            RenderedUI->WidgetInstance->PlayAnimation(Animation, 0.0f, 1, PlayMode);
        }
    }
}

void UMingAIGameUIRenderer::AnimateUIExit(const FString& RenderID, TEnumAsByte<EUMGSequencePlayMode::Type> PlayMode)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        UWidgetAnimation* Animation = RenderedUI->WidgetInstance->GetAnimationByName(TEXT("Exit"));
        if (Animation)
        {
            RenderedUI->WidgetInstance->PlayAnimation(Animation, 0.0f, 1, PlayMode);
        }
    }
}

void UMingAIGameUIRenderer::AnimateUIHover(const FString& RenderID, bool bIsHovering)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        FName AnimName = bIsHovering ? TEXT("HoverIn") : TEXT("HoverOut");
        UWidgetAnimation* Animation = RenderedUI->WidgetInstance->GetAnimationByName(AnimName);
        if (Animation)
        {
            RenderedUI->WidgetInstance->PlayAnimation(Animation, 0.0f, 1, EUMGSequencePlayMode::Forward);
        }
    }
}

void UMingAIGameUIRenderer::AnimateUIPulse(const FString& RenderID)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        UWidgetAnimation* Animation = RenderedUI->WidgetInstance->GetAnimationByName(TEXT("Pulse"));
        if (Animation)
        {
            RenderedUI->WidgetInstance->PlayAnimation(Animation, 0.0f, 0, EUMGSequencePlayMode::Forward);
        }
    }
}

UTextureRenderTarget2D* UMingAIGameUIRenderer::CreateRenderTarget(int32 Width, int32 Height)
{
    UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>(this);
    RenderTarget->InitAutoFormat(Width, Height);
    RenderTarget->ClearColor = FLinearColor::Transparent;
    return RenderTarget;
}

void UMingAIGameUIRenderer::CaptureWidgetToRenderTarget(UUserWidget* Widget, UTextureRenderTarget2D* RenderTarget)
{
    if (!Widget || !RenderTarget)
    {
        return;
    }
    
    FWidgetRenderer WidgetRenderer;
    WidgetRenderer.RenderWidget(Widget, RenderTarget, FVector2D(RenderTarget->SizeX, RenderTarget->SizeY), 1.0f);
}

void UMingAIGameUIRenderer::ApplyRenderTargetToMaterial(UTextureRenderTarget2D* RenderTarget, UMaterialInstanceDynamic* Material)
{
    if (Material && RenderTarget)
    {
        Material->SetTextureParameterValue(TEXT("UI_RenderTarget"), RenderTarget);
    }
}

void UMingAIGameUIRenderer::SetScreenSpacePosition(const FString& RenderID, FVector2D Position)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(RenderedUI->WidgetInstance->Slot))
        {
            Slot->SetPosition(Position);
        }
    }
}

void UMingAIGameUIRenderer::SetScreenSpaceSize(const FString& RenderID, FVector2D Size)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(RenderedUI->WidgetInstance->Slot))
        {
            Slot->SetSize(Size);
        }
    }
}

void UMingAIGameUIRenderer::AnchorUIToScreenPosition(const FString& RenderID, FVector2D AnchorPoint)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(RenderedUI->WidgetInstance->Slot))
        {
            FAnchors Anchors;
            Anchors.Minimum = AnchorPoint;
            Anchors.Maximum = AnchorPoint;
            Slot->SetAnchors(Anchors);
        }
    }
}

void UMingAIGameUIRenderer::AttachUIToWorldObject(const FString& RenderID, AActor* TargetActor, FVector Offset)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (!RenderedUI || !TargetActor)
    {
        return;
    }
    
    // Find or create widget component
    UWidgetComponent* WidgetComp = TargetActor->FindComponentByClass<UWidgetComponent>();
    if (!WidgetComp)
    {
        WidgetComp = NewObject<UWidgetComponent>(TargetActor);
        WidgetComp->RegisterComponent();
    }
    
    if (WidgetComp && RenderedUI->WidgetInstance)
    {
        WidgetComp->SetWidget(RenderedUI->WidgetInstance);
        WidgetComp->SetRelativeLocation(Offset);
        WidgetComp->SetWidgetSpace(EWidgetSpace::World);
    }
}

void UMingAIGameUIRenderer::SetUIWorldPosition(const FString& RenderID, FVector WorldPosition)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        UWidgetComponent* WidgetComp = Cast<UWidgetComponent>(RenderedUI->WidgetInstance->GetOuter());
        if (WidgetComp)
        {
            WidgetComp->SetWorldLocation(WorldPosition);
        }
    }
}

void UMingAIGameUIRenderer::SetUIWorldRotation(const FString& RenderID, FRotator WorldRotation)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        UWidgetComponent* WidgetComp = Cast<UWidgetComponent>(RenderedUI->WidgetInstance->GetOuter());
        if (WidgetComp)
        {
            WidgetComp->SetWorldRotation(WorldRotation);
        }
    }
}

void UMingAIGameUIRenderer::MakeUIBillboard(const FString& RenderID, bool bBillboard)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        UWidgetComponent* WidgetComp = Cast<UWidgetComponent>(RenderedUI->WidgetInstance->GetOuter());
        if (WidgetComp)
        {
            WidgetComp->SetTwoSided(bBillboard);
        }
    }
}

void UMingAIGameUIRenderer::EnableUIInteraction(const FString& RenderID, bool bEnable)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        ESlateVisibility Visibility = bEnable 
            ? ESlateVisibility::Visible 
            : ESlateVisibility::SelfHitTestInvisible;
        RenderedUI->WidgetInstance->SetVisibility(Visibility);
    }
}

void UMingAIGameUIRenderer::SetUIInputMode(const FString& RenderID, TEnumAsByte<EUMGInputMode::Type> InputMode)
{
    if (PlayerController)
    {
        switch (InputMode)
        {
        case EUMGInputMode::UIOnly:
            PlayerController->SetInputMode(FInputModeUIOnly());
            break;
        case EUMGInputMode::GameAndUI:
            PlayerController->SetInputMode(FInputModeGameAndUI());
            break;
        case EUMGInputMode::GameOnly:
            PlayerController->SetInputMode(FInputModeGameOnly());
            break;
        default:
            break;
        }
    }
}

void UMingAIGameUIRenderer::FocusUI(const FString& RenderID)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        RenderedUI->WidgetInstance->SetUserFocus(PlayerController);
    }
}

void UMingAIGameUIRenderer::SetRenderQuality(const FString& RenderID, float Quality)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        UWidgetComponent* WidgetComp = Cast<UWidgetComponent>(RenderedUI->WidgetInstance->GetOuter());
        if (WidgetComp)
        {
            WidgetComp->SetDrawSize(FVector2D(1920 * Quality, 1080 * Quality));
        }
    }
}

void UMingAIGameUIRenderer::SetRenderDistance(const FString& RenderID, float MaxDistance)
{
    // Implementation would set LOD or visibility based on distance
    UE_LOG(LogTemp, Log, TEXT("Set render distance for %s: %.2f"), *RenderID, MaxDistance);
}

void UMingAIGameUIRenderer::EnableOcclusionCulling(const FString& RenderID, bool bEnable)
{
    // Implementation would enable/disable occlusion queries
    UE_LOG(LogTemp, Log, TEXT("Occlusion culling %s for %s"), 
        bEnable ? TEXT("enabled") : TEXT("disabled"), *RenderID);
}

void UMingAIGameUIRenderer::PauseRendering(const FString& RenderID)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        RenderedUI->WidgetInstance->SetIsEnabled(false);
    }
}

void UMingAIGameUIRenderer::ResumeRendering(const FString& RenderID)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        RenderedUI->WidgetInstance->SetIsEnabled(true);
    }
}

void UMingAIGameUIRenderer::EnableRealTimeUpdate(const FString& RenderID, bool bEnable)
{
    RealTimeUpdateEnabled.Add(RenderID, bEnable);
}

void UMingAIGameUIRenderer::SetUpdateInterval(const FString& RenderID, float Interval)
{
    UpdateTimers.Add(RenderID, Interval);
}

void UMingAIGameUIRenderer::ForceRedraw(const FString& RenderID)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI && RenderedUI->WidgetInstance)
    {
        RenderedUI->WidgetInstance->ForceLayoutPrepass();
    }
}

void UMingAIGameUIRenderer::ShowAllUILayer(EAIGameUILayer Layer)
{
    for (const auto& Pair : RenderedWidgets)
    {
        if (Pair.Value.RenderConfig.UILayer == Layer)
        {
            ShowUI(Pair.Key, true);
        }
    }
}

void UMingAIGameUIRenderer::HideAllUILayer(EAIGameUILayer Layer)
{
    for (const auto& Pair : RenderedWidgets)
    {
        if (Pair.Value.RenderConfig.UILayer == Layer)
        {
            HideUI(Pair.Key, true);
        }
    }
}

void UMingAIGameUIRenderer::SetAllUIOpacity(EAIGameUILayer Layer, float Opacity)
{
    for (const auto& Pair : RenderedWidgets)
    {
        if (Pair.Value.RenderConfig.UILayer == Layer)
        {
            UpdateUIOpacity(Pair.Key, Opacity);
        }
    }
}

void UMingAIGameUIRenderer::TickRenderer(float DeltaTime)
{
    // Update opacity animations
    for (auto& Pair : RenderedWidgets)
    {
        FAIRenderedGameUI& UI = Pair.Value;
        
        if (UI.bIsVisible)
        {
            // Fade in
            float TargetOpacity = UI.RenderConfig.Opacity;
            if (UI.CurrentOpacity < TargetOpacity)
            {
                float NewOpacity = FMath::Min(UI.CurrentOpacity + DeltaTime / UI.RenderConfig.FadeInDuration, TargetOpacity);
                UpdateUIOpacity(Pair.Key, NewOpacity);
            }
        }
        else
        {
            // Fade out
            if (UI.CurrentOpacity > 0.0f)
            {
                float NewOpacity = FMath::Max(UI.CurrentOpacity - DeltaTime / UI.RenderConfig.FadeOutDuration, 0.0f);
                UpdateUIOpacity(Pair.Key, NewOpacity);
            }
        }
        
        // Update timers
        UI.TimeSinceCreated += DeltaTime;
        
        // Handle real-time updates
        if (RealTimeUpdateEnabled.FindRef(Pair.Key) && ShouldUpdateUI(Pair.Key, DeltaTime))
        {
            ForceRedraw(Pair.Key);
        }
    }
}

// Internal implementation
FAIRenderedGameUI UMingAIGameUIRenderer::CreateRenderedUIInternal(const FAIUIGeneratedWidget& AIWidget, const FAIGameUIRenderConfig& Config)
{
    FAIRenderedGameUI RenderedUI;
    RenderedUI.RenderID = GenerateRenderID();
    RenderedUI.RenderConfig = Config;
    
    // Create widget instance
    if (AIWidget.WidgetClass.IsValid())
    {
        UClass* WidgetClass = AIWidget.WidgetClass.LoadSynchronous();
        if (WidgetClass && PlayerController)
        {
            RenderedUI.WidgetInstance = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
            
            if (RenderedUI.WidgetInstance)
            {
                // Setup based on render mode
                switch (Config.RenderMode)
                {
                case EAIGameUIRenderMode::ScreenSpace:
                    SetupScreenSpaceUI(RenderedUI, Config);
                    break;
                case EAIGameUIRenderMode::WorldSpace:
                    SetupWorldSpaceUI(RenderedUI, Config);
                    break;
                case EAIGameUIRenderMode::RenderTarget:
                    SetupRenderTargetUI(RenderedUI, Config);
                    break;
                default:
                    break;
                }
                
                // Set initial state
                RenderedUI.WidgetInstance->SetRenderOpacity(Config.Opacity);
                RenderedUI.CurrentOpacity = Config.Opacity;
                RenderedUI.bIsVisible = true;
            }
        }
    }
    
    // Store rendered UI
    RenderedWidgets.Add(RenderedUI.RenderID, RenderedUI);
    
    UE_LOG(LogTemp, Log, TEXT("Created rendered UI: %s"), *RenderedUI.RenderID);
    return RenderedUI;
}

void UMingAIGameUIRenderer::SetupScreenSpaceUI(FAIRenderedGameUI& RenderedUI, const FAIGameUIRenderConfig& Config)
{
    if (RenderedUI.WidgetInstance && PlayerController)
    {
        // Add to viewport with layer order
        int32 ZOrder = LayerOrders.FindRef(Config.UILayer);
        RenderedUI.WidgetInstance->AddToPlayerScreen(ZOrder);
        
        UE_LOG(LogTemp, Log, TEXT("Setup screen space UI with ZOrder: %d"), ZOrder);
    }
}

void UMingAIGameUIRenderer::SetupWorldSpaceUI(FAIRenderedGameUI& RenderedUI, const FAIGameUIRenderConfig& Config)
{
    // World space setup would create a WidgetComponent
    UE_LOG(LogTemp, Log, TEXT("Setup world space UI"));
}

void UMingAIGameUIRenderer::SetupRenderTargetUI(FAIRenderedGameUI& RenderedUI, const FAIGameUIRenderConfig& Config)
{
    if (Config.TargetRenderTarget)
    {
        RenderedUI.RenderTexture = Config.TargetRenderTarget;
        
        // Capture widget to render target
        if (RenderedUI.WidgetInstance)
        {
            CaptureWidgetToRenderTarget(RenderedUI.WidgetInstance, RenderedUI.RenderTexture);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Setup render target UI"));
    }
}

void UMingAIGameUIRenderer::UpdateOpacityInternal(FAIRenderedGameUI& RenderedUI, float TargetOpacity, float DeltaTime)
{
    RenderedUI.CurrentOpacity = TargetOpacity;
    
    if (RenderedUI.WidgetInstance)
    {
        RenderedUI.WidgetInstance->SetRenderOpacity(TargetOpacity);
    }
}

void UMingAIGameUIRenderer::CleanupRenderedUI(const FString& RenderID)
{
    FAIRenderedGameUI* RenderedUI = RenderedWidgets.Find(RenderID);
    if (RenderedUI)
    {
        if (RenderedUI->WidgetInstance)
        {
            RenderedUI->WidgetInstance->RemoveFromParent();
            RenderedUI->WidgetInstance = nullptr;
        }
        
        // Cleanup render texture
        if (RenderedUI->RenderTexture)
        {
            RenderedUI->RenderTexture = nullptr;
        }
        
        RenderedWidgets.Remove(RenderID);
        UpdateTimers.Remove(RenderID);
        RealTimeUpdateEnabled.Remove(RenderID);
    }
}

void UMingAIGameUIRenderer::SortUIByLayer()
{
    // Sort by layer order
    TArray<FAIRenderedGameUI> SortedUI;
    RenderedWidgets.GenerateValueArray(SortedUI);
    
    SortedUI.Sort([this](const FAIRenderedGameUI& A, const FAIRenderedGameUI& B)
    {
        int32 OrderA = LayerOrders.FindRef(A.RenderConfig.UILayer);
        int32 OrderB = LayerOrders.FindRef(B.RenderConfig.UILayer);
        return OrderA < OrderB;
    });
}

void UMingAIGameUIRenderer::ApplyLayerOrder(EAIGameUILayer Layer)
{
    // Update ZOrder for all widgets in layer
    int32 ZOrder = LayerOrders.FindRef(Layer);
    
    for (const auto& Pair : RenderedWidgets)
    {
        if (Pair.Value.RenderConfig.UILayer == Layer && Pair.Value.WidgetInstance)
        {
            // Remove and re-add with new ZOrder
            Pair.Value.WidgetInstance->RemoveFromParent();
            Pair.Value.WidgetInstance->AddToPlayerScreen(ZOrder);
        }
    }
}

bool UMingAIGameUIRenderer::ShouldUpdateUI(const FString& RenderID, float DeltaTime)
{
    float* Interval = UpdateTimers.Find(RenderID);
    if (Interval)
    {
        *Interval -= DeltaTime;
        if (*Interval <= 0.0f)
        {
            // Reset timer
            FAIRenderedGameUI* UI = RenderedWidgets.Find(RenderID);
            if (UI)
            {
                *Interval = UI->RenderConfig.FadeInDuration;
            }
            return true;
        }
    }
    return false;
}

FString UMingAIGameUIRenderer::GenerateRenderID()
{
    return FString::Printf(TEXT("AIUI_Render_%s"), *FGuid::NewGuid().ToString());
}

void UMingAIGameUIRenderer::LogRenderEvent(const FString& Event, const FString& RenderID)
{
    UE_LOG(LogTemp, Log, TEXT("[AI UI Render] %s: %s"), *Event, *RenderID);
}
