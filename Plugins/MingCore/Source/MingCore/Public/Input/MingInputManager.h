#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingInputManager.generated.h"

/**
 * è¼¸å…¥?ä?é¡å?
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
 * è¼¸å…¥?‹å‹¢é¡å?
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
 * è¼¸å…¥äº‹ä»¶çµæ?
 */
USTRUCT(BlueprintType)
struct FInputEventData
{
    GENERATED_BODY()
    
    // è¼¸å…¥ä½ç½®ï¼ˆå?å¹•åº§æ¨™ï?
    UPROPERTY(BlueprintReadOnly)
    FVector2D ScreenPosition;
    
    // ä¸–ç?ä½ç½®ï¼ˆå??œæ?ï¼?    UPROPERTY(BlueprintReadOnly)
    FVector WorldPosition;
    
    // è¼¸å…¥?ä?é¡å?
    UPROPERTY(BlueprintReadOnly)
    EInputAction Action;
    
    // ?‹å‹¢é¡å?ï¼ˆè§¸?§ï?
    UPROPERTY(BlueprintReadOnly)
    EInputGesture Gesture;
    
    // è§¸æ§IDï¼ˆå?é»è§¸?§ï?
    UPROPERTY(BlueprintReadOnly)
    int32 TouchId;
    
    // ?¯å¦å·²è???    UPROPERTY(BlueprintReadOnly)
    bool bHandled;
    
    // ?‰éµä¿®é£¾ç¬¦ï?Shift, Ctrl, Altï¼?    UPROPERTY(BlueprintReadOnly)
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
 * è¼¸å…¥ç®¡ç??? * çµ±ä?ç®¡ç?é¼ æ??µç›¤?Œè§¸?§è¼¸?? */
UCLASS()
class MINGCORE_API UMingInputManager : public UObject
{
    GENERATED_BODY()

public:
    UMingInputManager();

    // ?å??–è¼¸?¥ç®¡?†å™¨
    UFUNCTION(BlueprintCallable, Category = "Input")
    void Initialize();

    // ?œé?è¼¸å…¥ç®¡ç???    UFUNCTION(BlueprintCallable, Category = "Input")
    void Shutdown();

    // æ¯å??´æ–°
    UFUNCTION(BlueprintCallable, Category = "Input")
    void Tick(float DeltaTime);

    // è¨­ç½®?¶å?è¼¸å…¥é¡å?
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetInputType(EInputType NewInputType);

    // ?²å??¶å?è¼¸å…¥é¡å?
    UFUNCTION(BlueprintCallable, Category = "Input")
    EInputType GetCurrentInputType() const { return CurrentInputType; }

    // ç¶å?è¼¸å…¥?ä?ï¼ˆBlueprint?¯èª¿?¨ï?
    UFUNCTION(BlueprintCallable, Category = "Input")
    void BindAction(EInputAction Action, UObject* Listener, FName FunctionName);

    // è§?™¤ç¶å?
    UFUNCTION(BlueprintCallable, Category = "Input")
    void UnbindAction(EInputAction Action, UObject* Listener);

    // è§¸ç™¼è¼¸å…¥äº‹ä»¶
    UFUNCTION(BlueprintCallable, Category = "Input")
    void TriggerInputEvent(const FInputEventData& EventData);

    // ?¤æ–·?¯å¦?ºè§¸?§è¨­??    UFUNCTION(BlueprintCallable, Category = "Input")
    bool IsTouchInput() const;

    // ?²å??€å¾Œè¼¸?¥ä?ç½?    UFUNCTION(BlueprintCallable, Category = "Input")
    FVector2D GetLastInputPosition() const { return LastInputPosition; }

    // è¨­ç½®è§¸æ§ç¸®æ”¾?ˆæ?åº?    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetTouchSensitivity(float Sensitivity);

    // ?²å?è§¸æ§ç¸®æ”¾?ˆæ?åº?    UFUNCTION(BlueprintCallable, Category = "Input")
    float GetTouchSensitivity() const { return TouchSensitivity; }

    // ?œæ??²å?å¯¦ä?
    static UMingInputManager* Get();

private:
    bool bInitialized;
    
    // ?¶å?è¼¸å…¥é¡å?
    UPROPERTY()
    EInputType CurrentInputType;
    
    // ?€å¾Œè¼¸?¥ä?ç½?    FVector2D LastInputPosition;
    
    // è§¸æ§?ˆæ?åº?    float TouchSensitivity;
    
    // è¼¸å…¥ç¶å?? å?
    TMap<EInputAction, TMap<TWeakObjectPtr<UObject>, FName>> InputBindings;
    
    // è§¸æ§?€?‹è¿½è¸?    struct FTouchState
    {
        FVector2D StartPosition;
        FVector2D CurrentPosition;
        float StartTime;
        bool bIsActive;
    };
    
    TMap<int32, FTouchState> ActiveTouches;
    
    // ?‹å‹¢æª¢æ¸¬?ƒæ•¸
    float TapTimeThreshold;
    float TapDistanceThreshold;
    float LongPressTimeThreshold;
    
    // ?®ä?å¯¦ä?
    static UMingInputManager* Instance;
    
    // è¼¸å…¥?•ç??½æ•¸
    void ProcessTouchInput(float DeltaTime);
    void ProcessMouseInput(float DeltaTime);
    void DetectGestures(const FTouchState& TouchState, int32 TouchId);
    void ExecuteInputBinding(EInputAction Action, const FInputEventData& EventData);
};
