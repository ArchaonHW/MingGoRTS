#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Platform\IPlatformInterface.h"
#include "MingUIManager.generated.h"

/**
 * UI ��??��??
 */
UENUM(BlueprintType)
enum class EUILayoutType: uint8 {
    Desktop           UMETA(DisplayName = "Desktop"),
    Tablet            UMETA(DisplayName = "Tablet"),
    PhonePortrait     UMETA(DisplayName = "Phone Portrait"),
    PhoneLandscape    UMETA(DisplayName = "Phone Landscape"),
    TV                UMETA(DisplayName = "TV")
};

/**
 * ��??�ؤo���O
 */
USTRUCT(BlueprintType)
struct FScreenSizeCategory
{
    GENERATED_BODY()
    
    
    float MinWidth;
    
    // ??�j�e��?X摧毀?1 ���目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxWidth;
    
    
    float MinHeight;
    
    // ??�j??��?X摧毀?1 ���目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHeight;
    
    // ��?XUI ��??��??
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
 * UI �Y��]�m
 */
USTRUCT(BlueprintType)
struct FUIScaleSettings
{
    GENERATED_BODY()
    
    // DPI �Y��摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DPIScale;
    
    // 摧毀 UI �Y��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GlobalUIScale;
    
    // �r??�Y��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FontScale;
    
    // 摧毀�Y��
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
 * UI ��?X * ��?X摧毀?UI ??��??�AX */
UCLASS()
class MINGCORE_API UMingUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingUIManager();

    
    void Initialize();

    
    void Shutdown();

    // �C?X?�s
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Tick(float DeltaTime);

    // 目標數量 UI ��??��??
    UFUNCTION(BlueprintCallable, Category = "UI")
    EUILayoutType GetCurrentLayoutType() const { return CurrentLayoutType; }

    // 摧毀��??�ؤo���O
    UFUNCTION(BlueprintCallable, Category = "UI")
    FScreenSizeCategory GetScreenSizeCategory() const { return CurrentSizeCategory; }

    // 目標數量�Y��]�m
    UFUNCTION(BlueprintCallable, Category = "UI")
    FUIScaleSettings GetCurrentScaleSettings() const { return CurrentScaleSettings; }

    // �p?X??XUI �ؤo
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetScaledSize(float BaseSize) const;

    
    float GetMinTouchTargetSize() const;

    
    bool NeedsSafeZonePadding() const;

    
    FMargin GetSafeZonePadding() const;

    // ��??��??�ؤo��?X?��
    UFUNCTION(BlueprintCallable, Category = "UI")
    void RegisterScreenSizeChangedCallback(UObject* Listener, FName FunctionName);

    // ���P??��
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UnregisterScreenSizeChangedCallback(UObject* Listener);

    // 目標數量��??
    static UMingUIManager* Get();

private:
    bool bInitialized;
    
    // 摧毀 UI ��??��??
    UPROPERTY()
    EUILayoutType CurrentLayoutType;
    
    // 摧毀��??�ؤo���O
    FScreenSizeCategory CurrentSizeCategory;
    
    // 摧毀�Y��]�m
    FUIScaleSettings CurrentScaleSettings;
    
    // ��??�ؤo�w�q
    TArray<FScreenSizeCategory> ScreenSizeDefinitions;
    
    // ��??�ؤo��?X?��
    TMap<TWeakObjectPtr<UObject>, FName> ScreenSizeChangedCallbacks;
    
    // ??��??��??�ؤo�]��??�˴�?X???
    FVector2D LastScreenSize;
    
    // 摧毀��??
    static UMingUIManager* Instance;
    
    // 目標數量���ؤo摧毀
    void InitializeScreenSizeDefinitions();
    
    // �˴���??�ؤo��??
    void CheckScreenSizeChange();
    
    // ??�s UI ��??
    void UpdateUILayout();
    
    // �p??�Y��]�m
    void CalculateScaleSettings();
    
    // ??����??�ؤo��??
    void NotifyScreenSizeChanged();
};

