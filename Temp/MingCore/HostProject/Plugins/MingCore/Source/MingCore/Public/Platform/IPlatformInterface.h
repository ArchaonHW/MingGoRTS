#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPlatformInterface.generated.h"

/**
 * 平台類�X��?
 */
UENUM(BlueprintType)
enum class EPlatformType : uint8
{
    Windows       UMETA(DisplayName = "Windows"),
    Android       UMETA(DisplayName = "Android"),
    IOS           UMETA(DisplayName = "iOS"),
    Unknown       UMETA(DisplayName = "Unknown")
};

/**
 * 輸入類�X��?
 */
UENUM(BlueprintType)
enum class EInputType : uint8
{
    MouseKeyboard UMETA(DisplayName = "Mouse & Keyboard"),
    Touch         UMETA(DisplayName = "Touch"),
    Gamepad       UMETA(DisplayName = "Gamepad")
};

/**
 * 平台?��?結�?
 */
USTRUCT(BlueprintType)
struct FPlatformCapabilities
{
    GENERATED_BODY()
    
    // ?�否?��?多�?觸控
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsMultiTouch;
    
    // ?�大�X�觸?��X    UPROPERTY(BlueprintReadOnly)
    int32 MaxTouchPoints;
    
    // ?�否?��X��X�盤
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsHardwareKeyboard;
    
    // ?�否?��?鼠�?
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsMouse;
    
    // 默�?輸入類�?
    UPROPERTY(BlueprintReadOnly)
    EInputType DefaultInputType;
    
    // ?�否?�要電池優X    UPROPERTY(BlueprintReadOnly)
    bool bRequiresBatteryOptimization;
    
    // 建議?��?大�X�單位數
    UPROPERTY(BlueprintReadOnly)
    int32 RecommendedMaxUnits;
    
    // 建議?�畫質�?�?    UPROPERTY(BlueprintReadOnly)
    int32 RecommendedQualityLevel;
    
    FPlatformCapabilities()
        : bSupportsMultiTouch(false)
        , MaxTouchPoints(1)
        , bSupportsHardwareKeyboard(true)
        , bSupportsMouse(true)
        , DefaultInputType(EInputType::MouseKeyboard)
        , bRequiresBatteryOptimization(false)
        , RecommendedMaxUnits(1000)
        , RecommendedQualityLevel(3)
    {}
};

/**
 * 平台?�象?�口
 * 定義?�?�平?�相?��X��?統�X�口
 */
UINTERFACE(MinimalAPI)
class MINGCORE_API UPlatformInterface : public UInterface
{
    GENERATED_BODY()
};

class MINGCORE_API IPlatformInterface
{
    GENERATED_BODY()

public:
    virtual ~IPlatformInterface() {}
    
    // ?��X��?平台類�?
    virtual EPlatformType GetPlatformType() const = 0;
    
    // ?��?平台?��?
    virtual FPlatformCapabilities GetCapabilities() const = 0;
    
    // ?��X�平X    virtual void Initialize() = 0;
    
    // ?��?平台
    virtual void Shutdown() = 0;
    
    // ?��?屏�?DPI
    virtual float GetScreenDPI() const = 0;
    
    // ?��?安全?�X(?�於移�?端�X�海屏�?)
    virtual FMargin GetSafeZone() const = 0;
    
    // 檢查?�否?�觸?�設X    virtual bool IsTouchDevice() const = 0;
    
    // 設置?�能模�?
    virtual void SetPerformanceMode(int32 Mode) = 0;
    
    // ?��X��X��X��? (0-1, -1表示不支X
    virtual float GetBatteryLevel() const = 0;
    
    // ?�否�?��?�電
    virtual bool IsCharging() const = 0;
    
    // 顯示平台?��X��?話�?
    virtual void ShowPlatformDialog(const FString& Title, const FString& Message) = 0;
    
    // ?�享?�能
    virtual void ShareContent(const FString& Content) = 0;
    
    // 評�?請�?
    virtual void RequestAppRating() = 0;
    
    // 保�X��X�平?�特定�X    virtual bool SaveToPlatformStorage(const FString& Key, const FString& Value) = 0;
    
    // 從平?�特定�X��X�數X    virtual FString LoadFromPlatformStorage(const FString& Key) const = 0;
};

