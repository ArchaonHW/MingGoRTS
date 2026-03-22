#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPlatformInterface.generated.h"

/**
 * å¹³å°é¡å??šè?
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
 * è¼¸å…¥é¡å??šè?
 */
UENUM(BlueprintType)
enum class EInputType : uint8
{
    MouseKeyboard UMETA(DisplayName = "Mouse & Keyboard"),
    Touch         UMETA(DisplayName = "Touch"),
    Gamepad       UMETA(DisplayName = "Gamepad")
};

/**
 * å¹³å°?½å?çµæ?
 */
USTRUCT(BlueprintType)
struct FPlatformCapabilities
{
    GENERATED_BODY()
    
    // ?¯å¦?¯æ?å¤šé?è§¸æ§
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsMultiTouch;
    
    // ?€å¤§å??‚è§¸?§é???    UPROPERTY(BlueprintReadOnly)
    int32 MaxTouchPoints;
    
    // ?¯å¦?¯æ??©ç??µç›¤
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsHardwareKeyboard;
    
    // ?¯å¦?¯æ?é¼ æ?
    UPROPERTY(BlueprintReadOnly)
    bool bSupportsMouse;
    
    // é»˜è?è¼¸å…¥é¡å?
    UPROPERTY(BlueprintReadOnly)
    EInputType DefaultInputType;
    
    // ?¯å¦?€è¦é›»æ± å„ª??    UPROPERTY(BlueprintReadOnly)
    bool bRequiresBatteryOptimization;
    
    // å»ºè­°?„æ?å¤§å??‚å–®ä½æ•¸
    UPROPERTY(BlueprintReadOnly)
    int32 RecommendedMaxUnits;
    
    // å»ºè­°?„ç•«è³ªç?ç´?    UPROPERTY(BlueprintReadOnly)
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
 * å¹³å°?½è±¡?¥å£
 * å®šç¾©?€?‰å¹³?°ç›¸?œå??½ç?çµ±ä??¥å£
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
    
    // ?²å??¶å?å¹³å°é¡å?
    virtual EPlatformType GetPlatformType() const = 0;
    
    // ?²å?å¹³å°?½å?
    virtual FPlatformCapabilities GetCapabilities() const = 0;
    
    // ?å??–å¹³??    virtual void Initialize() = 0;
    
    // ?œé?å¹³å°
    virtual void Shutdown() = 0;
    
    // ?²å?å±å?DPI
    virtual float GetScreenDPI() const = 0;
    
    // ?²å?å®‰å…¨?€??(?¨æ–¼ç§»å?ç«¯ç??‰æµ·å±ç?)
    virtual FMargin GetSafeZone() const = 0;
    
    // æª¢æŸ¥?¯å¦?ºè§¸?§è¨­??    virtual bool IsTouchDevice() const = 0;
    
    // è¨­ç½®?§èƒ½æ¨¡å?
    virtual void SetPerformanceMode(int32 Mode) = 0;
    
    // ?²å??¶å??»æ??»é? (0-1, -1è¡¨ç¤ºä¸æ”¯??
    virtual float GetBatteryLevel() const = 0;
    
    // ?¯å¦æ­?œ¨?…é›»
    virtual bool IsCharging() const = 0;
    
    // é¡¯ç¤ºå¹³å°?¹å??„å?è©±æ?
    virtual void ShowPlatformDialog(const FString& Title, const FString& Message) = 0;
    
    // ?†äº«?Ÿèƒ½
    virtual void ShareContent(const FString& Content) = 0;
    
    // è©•å?è«‹æ?
    virtual void RequestAppRating() = 0;
    
    // ä¿å??¸æ??°å¹³?°ç‰¹å®šå???    virtual bool SaveToPlatformStorage(const FString& Key, const FString& Value) = 0;
    
    // å¾å¹³?°ç‰¹å®šå??²è??–æ•¸??    virtual FString LoadFromPlatformStorage(const FString& Key) const = 0;
};
