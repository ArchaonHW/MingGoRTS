#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingInputManager.generated.h"

/**
 * 輸入????類??
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
 * 輸入??勢類??
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
 * 輸入事件結??
 */
USTRUCT(BlueprintType)
struct FInputEventData
{
    GENERATED_BODY()
    
    // 輸入位置（??幕座標??
    UPROPERTY(BlueprintReadOnly)
    FVector2D ScreenPosition;
    
    
    FVector WorldPosition;
    
    // 輸入????類??
    UPROPERTY(BlueprintReadOnly)
    EInputAction Action;
    
    // ??勢類??（觸????
    UPROPERTY(BlueprintReadOnly)
    EInputGesture Gesture;
    
    // 觸控ID（??點觸????
    UPROPERTY(BlueprintReadOnly)
    int32 TouchId;
    
    
    bool bHandled;
    
    
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
 * 輸入管?X * 統??管??鼠?X?盤??觸??輸X */
UCLASS()
class MINGCORE_API UMingInputManager : public UObject
{
    GENERATED_BODY()

public:
    UMingInputManager();

    // ???X?輸??管??器
    UFUNCTION(BlueprintCallable, Category = "Input")
    void Initialize();

    
    void Shutdown();

    // 每?X?新
    UFUNCTION(BlueprintCallable, Category = "Input")
    void Tick(float DeltaTime};

    // 設置????輸入類??
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetInputType(EInputType NewInputType};

    // ???X???輸入類??
    UFUNCTION(BlueprintCallable, Category = "Input")
    EInputType GetCurrentInputType() const { return CurrentInputType; }

    // 綁??輸入????（Blueprint??調????
    UFUNCTION(BlueprintCallable, Category = "Input")
    void BindAction(EInputAction Action, UObject* Listener, FName FunctionName};

    // ????綁??
    UFUNCTION(BlueprintCallable, Category = "Input")
    void UnbindAction(EInputAction Action, UObject* Listener};

    // 觸發輸入事件
    UFUNCTION(BlueprintCallable, Category = "Input")
    void TriggerInputEvent(const FInputEventData& EventData};

    
    bool IsTouchInput() const;

    
    FVector2D GetLastInputPosition() const { return LastInputPosition; }

    
    void SetTouchSensitivity(float Sensitivity};

    
    float GetTouchSensitivity() const { return TouchSensitivity; }

    // ???X???實??
    static UMingInputManager* Get();

private:
    bool bInitialized;
    
    // ????輸入類??
    UPROPERTY()
    EInputType CurrentInputType;
    
    // ??後輸??????    FVector2D LastInputPosition;
    
    // 觸控??????    float TouchSensitivity;
    
    // 輸入綁?X???
    TMap<EInputAction, TMap<TWeakObjectPtr<UObject>, FName>> InputBindings;
    
    // 觸控????追??    struct FTouchState
    {
        FVector2D StartPosition;
        FVector2D CurrentPosition;
        float StartTime;
        bool bIsActive;
    };
    
    TMap<int32, FTouchState> ActiveTouches;
    
    // ??勢檢測??數
    float TapTimeThreshold;
    float TapDistanceThreshold;
    float LongPressTimeThreshold;
    
    // ????實??
    static UMingInputManager* Instance;
    
    // 輸入???X?數
    void ProcessTouchInput(float DeltaTime};
    void ProcessMouseInput(float DeltaTime};
    void DetectGestures(const FTouchState& TouchState, int32 TouchId};
    void ExecuteInputBinding(EInputAction Action, const FInputEventData& EventData};
};

