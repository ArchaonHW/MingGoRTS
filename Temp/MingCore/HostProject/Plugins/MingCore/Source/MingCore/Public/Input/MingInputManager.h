#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingInputManager.generated.h"

/**
 * 輸入操作類型
 */
UENUM(BlueprintType)
enum class EInputAction : uint8
{
    Select            UMETA(DisplayName = "Select"),
    Move              UMETA(DisplayName = "Move"),
    Attack            UMETA(DisplayName = "Attack"),
    Build             UMETA(DisplayName = "Build"),
    Cancel            UMETA(DisplayName = "Cancel"),
    ZoomIn            UMETA(DisplayName = "Zoom In"),
    ZoomOut           UMETA(DisplayName = "Zoom Out"),
    PanCamera         UMETA(DisplayName = "Pan Camera"),
    OpenMenu          UMETA(DisplayName = "Open Menu"),
    QuickSave         UMETA(DisplayName = "Quick Save"),
    QuickLoad         UMETA(DisplayName = "Quick Load"),
    UnitSelectAll     UMETA(DisplayName = "Select All Units"),
    UnitFormation     UMETA(DisplayName = "Formation"),
    None              UMETA(DisplayName = "None")
};

/**
 * 輸入手勢類型
 */
UENUM(BlueprintType)
enum class EInputGesture : uint8
{
    None              UMETA(DisplayName = "None"),
    Tap               UMETA(DisplayName = "Tap"),
    DoubleTap         UMETA(DisplayName = "Double Tap"),
    LongPress         UMETA(DisplayName = "Long Press"),
    Pinch             UMETA(DisplayName = "Pinch"),
    Spread            UMETA(DisplayName = "Spread"),
    Pan               UMETA(DisplayName = "Pan"),
    SwipeLeft         UMETA(DisplayName = "Swipe Left"),
    SwipeRight        UMETA(DisplayName = "Swipe Right"),
    SwipeUp           UMETA(DisplayName = "Swipe Up"),
    SwipeDown         UMETA(DisplayName = "Swipe Down"),
    Rotate            UMETA(DisplayName = "Rotate")
};

/**
 * 輸入事件結構
 */
USTRUCT(BlueprintType)
struct FInputEventData
{
    GENERATED_BODY()
    
    // 輸入位置（屏幕座標）
    UPROPERTY(BlueprintReadOnly)
    FVector2D ScreenPosition;
    
    // 世界位置（如果有）
    UPROPERTY(BlueprintReadOnly)
    FVector WorldPosition;
    
    // 輸入操作類型
    UPROPERTY(BlueprintReadOnly)
    EInputAction Action;
    
    // 手勢類型（觸控）
    UPROPERTY(BlueprintReadOnly)
    EInputGesture Gesture;
    
    // 觸控ID（多點觸控）
    UPROPERTY(BlueprintReadOnly)
    int32 TouchId;
    
    // 是否已處理
    UPROPERTY(BlueprintReadOnly)
    bool bHandled;
    
    // 按鍵修飾符（Shift, Ctrl, Alt）
    UPROPERTY(BlueprintReadOnly)
    bool bShiftPressed;
    
    UPROPERTY(BlueprintReadOnly)
    bool bCtrlPressed;
    
    UPROPERTY(BlueprintReadOnly)
    bool bAltPressed;
    
    FInputEventData()
        : ScreenPosition(FVector2D::ZeroVector)
        , WorldPosition(FVector::ZeroVector)
        , Action(EInputAction::None)
        , Gesture(EInputGesture::None)
        , TouchId(0)
        , bHandled(false)
        , bShiftPressed(false)
        , bCtrlPressed(false)
        , bAltPressed(false)
    {}
};

/**
 * 輸入管理器
 * 統一管理鼠標鍵盤和觸控輸入
 */
UCLASS()
class MINGCORE_API UMingInputManager : public UObject
{
    GENERATED_BODY()

public:
    UMingInputManager();

    // 初始化輸入管理器
    UFUNCTION(BlueprintCallable, Category = "Input")
    void Initialize();

    // 關閉輸入管理器
    UFUNCTION(BlueprintCallable, Category = "Input")
    void Shutdown();

    // 每幀更新
    UFUNCTION(BlueprintCallable, Category = "Input")
    void Tick(float DeltaTime);

    // 設置當前輸入類型
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetInputType(EInputType NewInputType);

    // 獲取當前輸入類型
    UFUNCTION(BlueprintCallable, Category = "Input")
    EInputType GetCurrentInputType() const { return CurrentInputType; }

    // 綁定輸入操作（Blueprint可調用）
    UFUNCTION(BlueprintCallable, Category = "Input")
    void BindAction(EInputAction Action, UObject* Listener, FName FunctionName);

    // 解除綁定
    UFUNCTION(BlueprintCallable, Category = "Input")
    void UnbindAction(EInputAction Action, UObject* Listener);

    // 觸發輸入事件
    UFUNCTION(BlueprintCallable, Category = "Input")
    void TriggerInputEvent(const FInputEventData& EventData);

    // 判斷是否為觸控設備
    UFUNCTION(BlueprintCallable, Category = "Input")
    bool IsTouchInput() const;

    // 獲取最後輸入位置
    UFUNCTION(BlueprintCallable, Category = "Input")
    FVector2D GetLastInputPosition() const { return LastInputPosition; }

    // 設置觸控縮放靈敏度
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetTouchSensitivity(float Sensitivity);

    // 獲取觸控縮放靈敏度
    UFUNCTION(BlueprintCallable, Category = "Input")
    float GetTouchSensitivity() const { return TouchSensitivity; }

    // 靜態獲取實例
    static UMingInputManager* Get();

private:
    bool bInitialized;
    
    // 當前輸入類型
    UPROPERTY()
    EInputType CurrentInputType;
    
    // 最後輸入位置
    FVector2D LastInputPosition;
    
    // 觸控靈敏度
    float TouchSensitivity;
    
    // 輸入綁定映射
    TMap<EInputAction, TMap<TWeakObjectPtr<UObject>, FName>> InputBindings;
    
    // 觸控狀態追踪
    struct FTouchState
    {
        FVector2D StartPosition;
        FVector2D CurrentPosition;
        float StartTime;
        bool bIsActive;
    };
    
    TMap<int32, FTouchState> ActiveTouches;
    
    // 手勢檢測參數
    float TapTimeThreshold;
    float TapDistanceThreshold;
    float LongPressTimeThreshold;
    
    // 單例實例
    static UMingInputManager* Instance;
    
    // 輸入處理函數
    void ProcessTouchInput(float DeltaTime);
    void ProcessMouseInput(float DeltaTime);
    void DetectGestures(const FTouchState& TouchState, int32 TouchId);
    void ExecuteInputBinding(EInputAction Action, const FInputEventData& EventData);
};
