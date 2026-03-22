#include "Input/MingInputManager.h"
#include "Platform/MingPlatformManager.h"
#include "Kismet/GameplayStatics.h"

// 靜態實例初始化
UMingInputManager* UMingInputManager::Instance = nullptr;

UMingInputManager::UMingInputManager()
    : bInitialized(false)
    , CurrentInputType(EInputType::MouseKeyboard)
    , LastInputPosition(FVector2D::ZeroVector)
    , TouchSensitivity(1.0f)
    , TapTimeThreshold(0.2f)
    , TapDistanceThreshold(20.0f)
    , LongPressTimeThreshold(0.5f)
{
}

void UMingInputManager::Initialize()
{
    if (bInitialized)
    {
        return;
    }
    
    // 根據平台設置默認輸入類型
    UMingPlatformManager* PlatformManager = UMingPlatformManager::Get();
    if (PlatformManager)
    {
        FPlatformCapabilities Capabilities = PlatformManager->GetPlatformCapabilities();
        CurrentInputType = Capabilities.DefaultInputType;
        UE_LOG(LogTemp, Log, TEXT("InputManager initialized with input type: %s"),
            *UEnum::GetValueAsString(CurrentInputType));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlatformManager not available, using default Mouse&Keyboard"));
        CurrentInputType = EInputType::MouseKeyboard;
    }
    
    // 初始化觸控狀態
    ActiveTouches.Empty();
    
    Instance = this;
    bInitialized = true;
}

void UMingInputManager::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    
    // 清理輸入綁定
    InputBindings.Empty();
    ActiveTouches.Empty();
    
    Instance = nullptr;
    bInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("InputManager shutdown"));
}

void UMingInputManager::Tick(float DeltaTime)
{
    if (!bInitialized)
    {
        return;
    }
    
    // 根據輸入類型處理輸入
    if (CurrentInputType == EInputType::Touch)
    {
        ProcessTouchInput(DeltaTime);
    }
    else
    {
        ProcessMouseInput(DeltaTime);
    }
}

void UMingInputManager::SetInputType(EInputType NewInputType)
{
    if (CurrentInputType != NewInputType)
    {
        CurrentInputType = NewInputType;
        UE_LOG(LogTemp, Log, TEXT("Input type changed to: %s"),
            *UEnum::GetValueAsString(CurrentInputType));
    }
}

void UMingInputManager::BindAction(EInputAction Action, UObject* Listener, FName FunctionName)
{
    if (!Listener || FunctionName.IsNone())
    {
        return;
    }
    
    // 添加到綁定映射
    if (!InputBindings.Contains(Action))
    {
        InputBindings.Add(Action, TMap<TWeakObjectPtr<UObject>, FName>());
    }
    
    InputBindings[Action].Add(Listener, FunctionName);
    
    UE_LOG(LogTemp, Verbose, TEXT("Bound action %s to %s::%s"),
        *UEnum::GetValueAsString(Action),
        *Listener->GetName(),
        *FunctionName.ToString());
}

void UMingInputManager::UnbindAction(EInputAction Action, UObject* Listener)
{
    if (!Listener)
    {
        return;
    }
    
    if (InputBindings.Contains(Action))
    {
        InputBindings[Action].Remove(Listener);
        
        UE_LOG(LogTemp, Verbose, TEXT("Unbound action %s from %s"),
            *UEnum::GetValueAsString(Action),
            *Listener->GetName());
    }
}

void UMingInputManager::TriggerInputEvent(const FInputEventData& EventData)
{
    if (EventData.bHandled)
    {
        return;
    }
    
    // 更新最後輸入位置
    LastInputPosition = EventData.ScreenPosition;
    
    // 執行綁定的回調
    ExecuteInputBinding(EventData.Action, EventData);
    
    UE_LOG(LogTemp, Verbose, TEXT("Input event triggered: %s at (%f, %f)"),
        *UEnum::GetValueAsString(EventData.Action),
        EventData.ScreenPosition.X,
        EventData.ScreenPosition.Y);
}

bool UMingInputManager::IsTouchInput() const
{
    return CurrentInputType == EInputType::Touch;
}

void UMingInputManager::SetTouchSensitivity(float Sensitivity)
{
    TouchSensitivity = FMath::Clamp(Sensitivity, 0.1f, 3.0f);
    UE_LOG(LogTemp, Log, TEXT("Touch sensitivity set to: %f"), TouchSensitivity);
}

void UMingInputManager::ProcessTouchInput(float DeltaTime)
{
    // 這裡應該從引擎獲取觸控輸入
    // 暫時使用模擬邏輯
    
    // 檢測手勢
    for (auto& TouchPair : ActiveTouches)
    {
        DetectGestures(TouchPair.Value, TouchPair.Key);
    }
}

void UMingInputManager::ProcessMouseInput(float DeltaTime)
{
    // 鼠標輸入通常通過 PlayerController 的 InputComponent 處理
    // 這裡可以添加額外的鼠標手勢檢測（如框選）
}

void UMingInputManager::DetectGestures(const FTouchState& TouchState, int32 TouchId)
{
    if (!TouchState.bIsActive)
    {
        return;
    }
    
    // 計算觸控時間和移動距離
    float TouchDuration = FPlatformTime::Seconds() - TouchState.StartTime;
    float MoveDistance = FVector2D::Distance(TouchState.StartPosition, TouchState.CurrentPosition);
    
    // 檢測點擊 (短時間 + 小移動)
    if (TouchDuration < TapTimeThreshold && MoveDistance < TapDistanceThreshold)
    {
        FInputEventData EventData;
        EventData.ScreenPosition = TouchState.CurrentPosition;
        EventData.Action = EInputAction::Select;
        EventData.Gesture = EInputGesture::Tap;
        EventData.TouchId = TouchId;
        
        TriggerInputEvent(EventData);
    }
    // 檢測長按
    else if (TouchDuration >= LongPressTimeThreshold && MoveDistance < TapDistanceThreshold)
    {
        FInputEventData EventData;
        EventData.ScreenPosition = TouchState.CurrentPosition;
        EventData.Action = EInputAction::OpenMenu;
        EventData.Gesture = EInputGesture::LongPress;
        EventData.TouchId = TouchId;
        
        TriggerInputEvent(EventData);
    }
}

void UMingInputManager::ExecuteInputBinding(EInputAction Action, const FInputEventData& EventData)
{
    if (!InputBindings.Contains(Action))
    {
        return;
    }
    
    auto& Bindings = InputBindings[Action];
    
    // 執行所有綁定的回調
    for (auto It = Bindings.CreateIterator(); It; ++It)
    {
        if (It->Key.IsValid())
        {
            UObject* Listener = It->Key.Get();
            FName FunctionName = It->Value;
            
            // 通過反射調用函數
            UFunction* Function = Listener->FindFunction(FunctionName);
            if (Function)
            {
                // 準備參數
                struct FInputCallbackParams
                {
                    FInputEventData EventData;
                };
                
                FInputCallbackParams Params;
                Params.EventData = EventData;
                
                Listener->ProcessEvent(Function, &Params);
            }
        }
        else
        {
            // 移除無效的綁定
            It.RemoveCurrent();
        }
    }
}

UMingInputManager* UMingInputManager::Get()
{
    return Instance;
}
