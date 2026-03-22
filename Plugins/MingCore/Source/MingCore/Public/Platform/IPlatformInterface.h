#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPlatformInterface.generated.h"

/**
 * ���x��?X???
 */
UENUM(BlueprintType)
enum class EPlatformType: uint8 {
    Windows       UMETA(DisplayName = "Windows"),
    Android       UMETA(DisplayName = "Android"),
    IOS           UMETA(DisplayName = "iOS"),
    Unknown       UMETA(DisplayName = "Unknown")
};

/**
 * ��J��?X???
 */
UENUM(BlueprintType)
enum class EInputType: uint8 {
    MouseKeyboard UMETA(DisplayName = "Mouse & Keyboard"),
    Touch         UMETA(DisplayName = "Touch"),
    Gamepad       UMETA(DisplayName = "Gamepad")
};

/**
 * ���x摧毀��??
 */
USTRUCT(BlueprintType)
struct FPlatformCapabilities
{
    GENERATED_BODY()
    
    // ??�_摧毀�h??Ĳ��
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsMultiTouch;
    
    
    int32 MaxTouchPoints;
    
    // ??�_故事重要性?�L
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsHardwareKeyboard;
    
    // ??�_摧毀��??
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsMouse;
    
    // �q??��J��??
    UPROPERTY(BlueprintReadOnly)
    EInputType DefaultInputType;
    
    
    bool bRequiresBatteryOptimization;
    
    // ��ĳ摧毀�j?X?����
    UPROPERTY(BlueprintReadOnly)
    int32 RecommendedMaxUnits;
    
    
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
 * ���x??�H??�f
 * �w�q摧毀��??��目標數量��?X?�f
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
    
    // 目標數量���x��??
    virtual EPlatformType GetPlatformType() const = 0;
    
    // 摧毀���x摧毀
    virtual FPlatformCapabilities GetCapabilities() const = 0;
    
    // ???X?��X
    virtual void Initialize() = 0;
    
    // 摧毀���x
    virtual void Shutdown() = 0;
    
    // 摧毀��??DPI
    virtual float GetScreenDPI() const = 0;
    
    // 摧毀�w��??X(??��??��?X?����??)
    virtual FMargin GetSafeZone() const = 0;
    
    // �ˬd??�_??Ĳ??�]X
    virtual bool IsTouchDevice() const = 0;
    
    // �]�m??���??
    virtual void SetPerformanceMode(int32 Mode) = 0;
    
    // 故事選項X??? (0-1, -1��ܤ���X
    virtual float GetBatteryLevel() const = 0;
    
    // ??�_摧毀??�q
    virtual bool IsCharging() const = 0;
    
    // ��ܥ��x目標數量��??
    virtual void ShowPlatformDialog(const FString& Title, const FString& Message) = 0;
    
    // ??��??��
    virtual void ShareContent(const FString& Content) = 0;
    
    // ��??��??
    virtual void RequestAppRating() = 0;
    
    // �O?X??X?��??�S�w?X
    virtual bool SaveToPlatformStorage(const FString& Key, const FString& Value) = 0;
    
    // �q��??�S�w?X??X?��X
    virtual FString LoadFromPlatformStorage(const FString& Key) const = 0;
};

