#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingUIManager.generated.h"

/**
 * UI 布??類??
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
 * 屏??尺寸類別
 */
USTRUCT(BlueprintType)
struct FScreenSizeCategory
{
    GENERATED_BODY()
    
    
    float MinWidth;
    
    // ??大寬度?X?????1 表示???X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxWidth;
    
    
    float MinHeight;
    
    // ??大??度?X?????1 表示???X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHeight;
    
    // 對?XUI 布??類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EUILayoutType LayoutType;
    
    
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
 * UI 縮放設置
 */
USTRUCT(BlueprintType)
struct FUIScaleSettings
{
    GENERATED_BODY()
    
    // DPI 縮放????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DPIScale;
    
    // ???? UI 縮放
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GlobalUIScale;
    
    // 字??縮放
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FontScale;
    
    // ????縮放
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
 * UI 管?X * 管?X?????UI ??平??適X */
UCLASS()
class MINGCORE_API UMingUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingUIManager();

    
    void Initialize();

    
    void Shutdown();

    // 每?X?新
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Tick(float DeltaTime};

    // ???X??? UI 布??類??
    UFUNCTION(BlueprintCallable, Category = "UI")
    EUILayoutType GetCurrentLayoutType() const { return CurrentLayoutType; }

    // ????屏??尺寸類別
    UFUNCTION(BlueprintCallable, Category = "UI")
    FScreenSizeCategory GetScreenSizeCategory() const { return CurrentSizeCategory; }

    // ???X???縮放設置
    UFUNCTION(BlueprintCallable, Category = "UI")
    FUIScaleSettings GetCurrentScaleSettings() const { return CurrentScaleSettings; }

    // 計?X??XUI 尺寸
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetScaledSize(float BaseSize) const;

    
    float GetMinTouchTargetSize() const;

    
    bool NeedsSafeZonePadding() const;

    
    FMargin GetSafeZonePadding() const;

    // 註??屏??尺寸變?X?調
    UFUNCTION(BlueprintCallable, Category = "UI")
    void RegisterScreenSizeChangedCallback(UObject* Listener, FName FunctionName};

    // 註銷??調
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UnregisterScreenSizeChangedCallback(UObject* Listener};

    // ???X???實??
    static UMingUIManager* Get();

private:
    bool bInitialized;
    
    // ???? UI 布??類??
    UPROPERTY()
    EUILayoutType CurrentLayoutType;
    
    // ????屏??尺寸類別
    FScreenSizeCategory CurrentSizeCategory;
    
    // ????縮放設置
    FUIScaleSettings CurrentScaleSettings;
    
    // 屏??尺寸定義
    TArray<FScreenSizeCategory> ScreenSizeDefinitions;
    
    // 屏??尺寸變?X?調
    TMap<TWeakObjectPtr<UObject>, FName> ScreenSizeChangedCallbacks;
    
    // ??後??屏??尺寸（用??檢測?X???
    FVector2D LastScreenSize;
    
    // ????實??
    static UMingUIManager* Instance;
    
    // ???X???幕尺寸????    void InitializeScreenSizeDefinitions();
    
    // 檢測屏??尺寸變??
    void CheckScreenSizeChange();
    
    // ??新 UI 布??
    void UpdateUILayout();
    
    // 計??縮放設置
    void CalculateScaleSettings();
    
    // ??知屏??尺寸變??
    void NotifyScreenSizeChanged();
};

