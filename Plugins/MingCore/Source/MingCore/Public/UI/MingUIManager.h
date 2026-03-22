#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingUIManager.generated.h"

/**
 * UI 布局類型
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
 * 屏幕尺寸類別
 */
USTRUCT(BlueprintType)
struct FScreenSizeCategory
{
    GENERATED_BODY()
    
    // 最小寬度（像素）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinWidth;
    
    // 最大寬度（像素，-1 表示無限制）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxWidth;
    
    // 最小高度（像素）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinHeight;
    
    // 最大高度（像素，-1 表示無限制）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHeight;
    
    // 對應的 UI 布局類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EUILayoutType LayoutType;
    
    // 觸控目標最小尺寸（像素）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
    
    // DPI 縮放因子
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DPIScale;
    
    // 全局 UI 縮放
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GlobalUIScale;
    
    // 字體縮放
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FontScale;
    
    // 圖標縮放
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
 * UI 管理器
 * 管理響應式 UI 和平台適配
 */
UCLASS()
class MINGCORE_API UMingUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingUIManager();

    // 初始化 UI 管理器
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Initialize();

    // 關閉 UI 管理器
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Shutdown();

    // 每幀更新
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Tick(float DeltaTime);

    // 獲取當前 UI 布局類型
    UFUNCTION(BlueprintCallable, Category = "UI")
    EUILayoutType GetCurrentLayoutType() const { return CurrentLayoutType; }

    // 獲取屏幕尺寸類別
    UFUNCTION(BlueprintCallable, Category = "UI")
    FScreenSizeCategory GetScreenSizeCategory() const { return CurrentSizeCategory; }

    // 獲取當前縮放設置
    UFUNCTION(BlueprintCallable, Category = "UI")
    FUIScaleSettings GetCurrentScaleSettings() const { return CurrentScaleSettings; }

    // 計算適配的 UI 尺寸
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetScaledSize(float BaseSize) const;

    // 獲取觸控目標最小尺寸
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetMinTouchTargetSize() const;

    // 檢查是否需要適配安全區域
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool NeedsSafeZonePadding() const;

    // 獲取安全區域邊距
    UFUNCTION(BlueprintCallable, Category = "UI")
    FMargin GetSafeZonePadding() const;

    // 註冊屏幕尺寸變化回調
    UFUNCTION(BlueprintCallable, Category = "UI")
    void RegisterScreenSizeChangedCallback(UObject* Listener, FName FunctionName);

    // 註銷回調
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UnregisterScreenSizeChangedCallback(UObject* Listener);

    // 靜態獲取實例
    static UMingUIManager* Get();

private:
    bool bInitialized;
    
    // 當前 UI 布局類型
    UPROPERTY()
    EUILayoutType CurrentLayoutType;
    
    // 當前屏幕尺寸類別
    FScreenSizeCategory CurrentSizeCategory;
    
    // 當前縮放設置
    FUIScaleSettings CurrentScaleSettings;
    
    // 屏幕尺寸定義
    TArray<FScreenSizeCategory> ScreenSizeDefinitions;
    
    // 屏幕尺寸變化回調
    TMap<TWeakObjectPtr<UObject>, FName> ScreenSizeChangedCallbacks;
    
    // 最後的屏幕尺寸（用於檢測變化）
    FVector2D LastScreenSize;
    
    // 單例實例
    static UMingUIManager* Instance;
    
    // 初始化屏幕尺寸定義
    void InitializeScreenSizeDefinitions();
    
    // 檢測屏幕尺寸變化
    void CheckScreenSizeChange();
    
    // 更新 UI 布局
    void UpdateUILayout();
    
    // 計算縮放設置
    void CalculateScaleSettings();
    
    // 通知屏幕尺寸變化
    void NotifyScreenSizeChanged();
};
