#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingUIManager.generated.h"

/**
 * UI å¸ƒå?é¡å?
 */
UENUM(BlueprintType)
enum class EUILayoutType : uint8
{
    Desktop           UMETA(DisplayName = "Desktop"),
    Tablet            UMETA(DisplayName = "Tablet"),
    PhonePortrait     UMETA(DisplayName = "Phone Portrait"),
    PhoneLandscape    UMETA(DisplayName = "Phone Landscape"),
    TV                UMETA(DisplayName = "TV")
};

/**
 * å±å?å°ºå¯¸é¡åˆ¥
 */
USTRUCT(BlueprintType)
struct FScreenSizeCategory
{
    GENERATED_BODY()
    
    // ?€å°å¯¬åº¦ï??ç?ï¼?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinWidth;
    
    // ?€å¤§å¯¬åº¦ï??ç?ï¼?1 è¡¨ç¤º?¡é??¶ï?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxWidth;
    
    // ?€å°é?åº¦ï??ç?ï¼?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinHeight;
    
    // ?€å¤§é?åº¦ï??ç?ï¼?1 è¡¨ç¤º?¡é??¶ï?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHeight;
    
    // å°æ???UI å¸ƒå?é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EUILayoutType LayoutType;
    
    // è§¸æ§?®æ??€å°å°ºå¯¸ï??ç?ï¼?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinTouchTargetSize;
    
    FScreenSizeCategory()
        : MinWidth(0)
        , MaxWidth(-1)
        , MinHeight(0)
        , MaxHeight(-1)
        , LayoutType(EUILayoutType::Desktop)
        , MinTouchTargetSize(44.0f)
    {}
};

/**
 * UI ç¸®æ”¾è¨­ç½®
 */
USTRUCT(BlueprintType)
struct FUIScaleSettings
{
    GENERATED_BODY()
    
    // DPI ç¸®æ”¾? å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DPIScale;
    
    // ?¨å? UI ç¸®æ”¾
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GlobalUIScale;
    
    // å­—é?ç¸®æ”¾
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FontScale;
    
    // ?–æ?ç¸®æ”¾
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float IconScale;
    
    FUIScaleSettings()
        : DPIScale(1.0f)
        , GlobalUIScale(1.0f)
        , FontScale(1.0f)
        , IconScale(1.0f)
    {}
};

/**
 * UI ç®¡ç??? * ç®¡ç??¿æ?å¼?UI ?Œå¹³?°é©?? */
UCLASS()
class MINGCORE_API UMingUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingUIManager();

    // ?å???UI ç®¡ç???    UFUNCTION(BlueprintCallable, Category = "UI")
    void Initialize();

    // ?œé? UI ç®¡ç???    UFUNCTION(BlueprintCallable, Category = "UI")
    void Shutdown();

    // æ¯å??´æ–°
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Tick(float DeltaTime);

    // ?²å??¶å? UI å¸ƒå?é¡å?
    UFUNCTION(BlueprintCallable, Category = "UI")
    EUILayoutType GetCurrentLayoutType() const { return CurrentLayoutType; }

    // ?²å?å±å?å°ºå¯¸é¡åˆ¥
    UFUNCTION(BlueprintCallable, Category = "UI")
    FScreenSizeCategory GetScreenSizeCategory() const { return CurrentSizeCategory; }

    // ?²å??¶å?ç¸®æ”¾è¨­ç½®
    UFUNCTION(BlueprintCallable, Category = "UI")
    FUIScaleSettings GetCurrentScaleSettings() const { return CurrentScaleSettings; }

    // è¨ˆç??©é???UI å°ºå¯¸
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetScaledSize(float BaseSize) const;

    // ?²å?è§¸æ§?®æ??€å°å°ºå¯?    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetMinTouchTargetSize() const;

    // æª¢æŸ¥?¯å¦?€è¦é©?å??¨å???    UFUNCTION(BlueprintCallable, Category = "UI")
    bool NeedsSafeZonePadding() const;

    // ?²å?å®‰å…¨?€?Ÿé?è·?    UFUNCTION(BlueprintCallable, Category = "UI")
    FMargin GetSafeZonePadding() const;

    // è¨»å?å±å?å°ºå¯¸è®Šå??èª¿
    UFUNCTION(BlueprintCallable, Category = "UI")
    void RegisterScreenSizeChangedCallback(UObject* Listener, FName FunctionName);

    // è¨»éŠ·?èª¿
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UnregisterScreenSizeChangedCallback(UObject* Listener);

    // ?œæ??²å?å¯¦ä?
    static UMingUIManager* Get();

private:
    bool bInitialized;
    
    // ?¶å? UI å¸ƒå?é¡å?
    UPROPERTY()
    EUILayoutType CurrentLayoutType;
    
    // ?¶å?å±å?å°ºå¯¸é¡åˆ¥
    FScreenSizeCategory CurrentSizeCategory;
    
    // ?¶å?ç¸®æ”¾è¨­ç½®
    FUIScaleSettings CurrentScaleSettings;
    
    // å±å?å°ºå¯¸å®šç¾©
    TArray<FScreenSizeCategory> ScreenSizeDefinitions;
    
    // å±å?å°ºå¯¸è®Šå??èª¿
    TMap<TWeakObjectPtr<UObject>, FName> ScreenSizeChangedCallbacks;
    
    // ?€å¾Œç?å±å?å°ºå¯¸ï¼ˆç”¨?¼æª¢æ¸¬è??–ï?
    FVector2D LastScreenSize;
    
    // ?®ä?å¯¦ä?
    static UMingUIManager* Instance;
    
    // ?å??–å?å¹•å°ºå¯¸å?ç¾?    void InitializeScreenSizeDefinitions();
    
    // æª¢æ¸¬å±å?å°ºå¯¸è®Šå?
    void CheckScreenSizeChange();
    
    // ?´æ–° UI å¸ƒå?
    void UpdateUILayout();
    
    // è¨ˆç?ç¸®æ”¾è¨­ç½®
    void CalculateScaleSettings();
    
    // ?šçŸ¥å±å?å°ºå¯¸è®Šå?
    void NotifyScreenSizeChanged();
};
