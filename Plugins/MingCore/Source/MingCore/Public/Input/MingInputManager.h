#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingInputManager.generated.h"

/**
 * ��J摧毀��??
 */
UENUM(BlueprintType)
enum class EInputAction: uint8 {
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
 * ��J??����??
 */
UENUM(BlueprintType)
enum class EInputGesture: uint8 {
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
 * ��J�ƥ�??
 */
USTRUCT(BlueprintType)
struct FInputEventData
{
    GENERATED_BODY()
    
    // ��J��m�]??���y��??
    UPROPERTY(BlueprintReadOnly)
    FVector2D ScreenPosition;
    
    
    FVector WorldPosition;
    
    // ��J摧毀��??
    UPROPERTY(BlueprintReadOnly)
    EInputAction Action;
    
    // ??����??�]Ĳ摧毀
    UPROPERTY(BlueprintReadOnly)
    EInputGesture Gesture;
    
    // Ĳ��ID�]??�IĲ摧毀
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
 * ��J��?X * ��??��??��?X?�L??Ĳ??��X */
UCLASS()
class MINGCORE_API UMingInputManager : public UObject
{
    GENERATED_BODY()

public:
    UMingInputManager();

    // ???X?��??��??��
    UFUNCTION(BlueprintCallable, Category = "Input")
    void Initialize();

    
    void Shutdown();

    // �C?X?�s
    UFUNCTION(BlueprintCallable, Category = "Input")
    void Tick(float DeltaTime);

    // �]�m摧毀��J��??
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetInputType(EInputType NewInputType);

    // 目標數量��J��??
    UFUNCTION(BlueprintCallable, Category = "Input")
    EInputType GetCurrentInputType() const { return CurrentInputType; }

    // �j??��J摧毀�]Blueprint??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Input")
    void BindAction(EInputAction Action, UObject* Listener, FName FunctionName);

    // 摧毀�j??
    UFUNCTION(BlueprintCallable, Category = "Input")
    void UnbindAction(EInputAction Action, UObject* Listener);

    // Ĳ�o��J�ƥ�
    UFUNCTION(BlueprintCallable, Category = "Input")
    void TriggerInputEvent(const FInputEventData& EventData);

    
    bool IsTouchInput() const;

    
    FVector2D GetLastInputPosition() const { return LastInputPosition; }

    
    void SetTouchSensitivity(float Sensitivity);

    
    float GetTouchSensitivity() const { return TouchSensitivity; }

    // 目標數量��??
    static UMingInputManager* Get();

private:
    bool bInitialized;
    
    // 摧毀��J��??
    UPROPERTY()
    EInputType CurrentInputType;
    
    // ??���摧毀??
    FVector2D LastInputPosition;
    
    // Ĳ��摧毀??
    float TouchSensitivity;
    
    // ��J�j?X???
    TMap<EInputAction, TMap<TWeakObjectPtr<UObject>, FName>> InputBindings;
    
    // Ĳ��摧毀�l??
    struct FTouchState
    {
        FVector2D StartPosition;
        FVector2D CurrentPosition;
        float StartTime;
        bool bIsActive;
    };
    
    TMap<int32, FTouchState> ActiveTouches;
    
    // ??���˴�??��
    float TapTimeThreshold;
    float TapDistanceThreshold;
    float LongPressTimeThreshold;
    
    // 摧毀��??
    static UMingInputManager* Instance;
    
    // ��J???X?��
    void ProcessTouchInput(float DeltaTime);
    void ProcessMouseInput(float DeltaTime);
    void DetectGestures(const FTouchState& TouchState, int32 TouchId);
    void ExecuteInputBinding(EInputAction Action, const FInputEventData& EventData);
};

