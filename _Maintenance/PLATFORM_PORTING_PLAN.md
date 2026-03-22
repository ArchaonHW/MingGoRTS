# 平台移植規劃

## 跨平台支持：Windows, Android, iOS

### 1. 平台特定優化

#### 1.1 Android 平台適配
```cpp
// Android 平台檢測和優化
#if PLATFORM_ANDROID
class MINGGORTS_API UMingGoRTSAndroidManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Android")
    void InitializeAndroidSpecific();
    
    UFUNCTION(BlueprintCallable, Category = "Android")
    void HandleTouchInput(const FVector2D& TouchPosition);
    
    UFUNCTION(BlueprintCallable, Category = "Android")
    void OptimizeForMobile();
    
    UFUNCTION(BlueprintCallable, Category = "Android")
    void AdjustUIForMobile();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Android")
    bool bUseTouchControls = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Android")
    float MobileUIScale = 1.5f;
};
#endif
```

#### 1.2 iOS 平台適配
```cpp
#if PLATFORM_IOS
class MINGGORTS_API UMingGoRTSiOSManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "iOS")
    void InitializeiOSSpecific();
    
    UFUNCTION(BlueprintCallable, Category = "iOS")
    void HandleiOSNotifications();
    
    UFUNCTION(BlueprintCallable, Category = "iOS")
    void OptimizeForiOS();
    
    UFUNCTION(BlueprintCallable, Category = "iOS")
    void IntegrateWithiCloud();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "iOS")
    bool bUseGameCenter = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "iOS")
    bool biCloudSyncEnabled = true;
};
#endif
```

### 2. 輸入系統適配

#### 2.1 觸摸控制支持
```cpp
USTRUCT(BlueprintType)
struct FTouchInputData
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D TouchPosition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TouchDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsTap;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSwipe;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D SwipeDirection;
};

class MINGGORTS_API UTouchInputManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Touch Input")
    void ProcessTouchInput(const FTouchInputData& TouchData);
    
    UFUNCTION(BlueprintCallable, Category = "Touch Input")
    void SimulateMouseFromTouch(const FVector2D& TouchPosition);
    
    UFUNCTION(BlueprintCallable, Category = "Touch Input")
    void EnableTouchGestures();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Touch Input")
    float TapThreshold = 0.1f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Touch Input")
    float SwipeThreshold = 50.0f;
};
```

#### 2.2 控制器適配
```cpp
// 移動平台控制器支持
class MINGGORTS_API UMobileControllerManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Mobile Controller")
    bool IsMobileControllerConnected();
    
    UFUNCTION(BlueprintCallable, Category = "Mobile Controller")
    void MapMobileControlsToRTS();
    
    UFUNCTION(BlueprintCallable, Category = "Mobile Controller")
    void ShowMobileControlsUI();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mobile Controller")
    TMap<FName, FKey> ControllerMappings;
};
```

### 3. 性能優化

#### 3.1 移動平台性能優化
```cpp
// 移動平台性能設置
USTRUCT(BlueprintType)
struct FMobilePerformanceSettings
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxUnitCount = 200; // 移動平台限制單位數量
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetFrameRate = 30.0f; // 移動平台目標幀率
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TextureQuality = 1; // 0=High, 1=Medium, 2=Low
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LODDistance = 1000.0f; // LOD距離
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableMobileShadows = false; // 移動平台關閉陰影
};

class MINGGORTS_API UMobilePerformanceManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Mobile Performance")
    void ApplyMobileSettings();
    
    UFUNCTION(BlueprintCallable, Category = "Mobile Performance")
    void OptimizeForDevice();
    
    UFUNCTION(BlueprintCallable, Category = "Mobile Performance")
    void AdjustQualityBasedOnPerformance();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mobile Performance")
    FMobilePerformanceSettings PerformanceSettings;
};
```

### 4. 平台特定功能

#### 4.1 Android 特定功能
```cpp
#if PLATFORM_ANDROID
class MINGGORTS_API UAndroidSpecificFeatures : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Android")
    void ShareScreenshot();
    
    UFUNCTION(BlueprintCallable, Category = "Android")
    void RateGame();
    
    UFUNCTION(BlueprintCallable, Category = "Android")
    void CheckPermissions();
    
    UFUNCTION(BlueprintCallable, Category = "Android")
    void ShowAndroidAchievements();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Android")
    FString GooglePlayGamesID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Android")
    FString AndroidMarketURL;
};
#endif
```

#### 4.2 iOS 特定功能
```cpp
#if PLATFORM_IOS
class MINGGORTS_API UiOSSpecificFeatures : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "iOS")
    void ShareToSocialMedia();
    
    UFUNCTION(BlueprintCallable, Category = "iOS")
    void ShowGameCenter();
    
    UFUNCTION(BlueprintCallable, Category = "iOS")
    void RequestReview();
    
    UFUNCTION(BlueprintCallable, Category = "iOS")
    void ShowiOSAchievements();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "iOS")
    FString AppStoreID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "iOS")
    bool biCloudEnabled = true;
};
#endif
```

### 5. 內容管理

#### 5.1 平台特定資源
```cpp
// 平台資源管理器
class MINGGORTS_API UPlatformResourceManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Platform Resources")
    void LoadPlatformSpecificAssets();
    
    UFUNCTION(BlueprintCallable, Category = "Platform Resources")
    void OptimizeAssetLoading();
    
    UFUNCTION(BlueprintCallable, Category = "Platform Resources")
    void HandleAssetStreaming();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Resources")
    TMap<FName, FString> PlatformAssetPaths;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Resources")
    bool bUseAssetStreaming = true;
};
```

### 6. 測試策略

#### 6.1 平台測試
```cpp
// 平台測試工具
class MINGGORTS_API UPlatformTestingTools : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void RunPlatformSpecificTests();
    
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void TestMobilePerformance();
    
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void ValidateTouchControls();
    
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void TestNetworkConnectivity();
    
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void GeneratePlatformReport();
};
```

### 7. 實現時間表

#### 階段 1: 基礎適配 (4週)
- Android 平台檢測
- iOS 平台檢測
- 基礎觸摸控制
- 性能優化框架

#### 階段 2: 移動優化 (4週)
- 完整觸摸控制系統
- 移動控制器支持
- UI適配
- 資源優化

#### 階段 3: 平台功能 (4週)
- Android 特定功能
- iOS 特定功能
- 社交集成
- 應用商店集成

#### 階段 4: 優化和測試 (2週)
- 性能調優
- 平台測試
- 內容優化
- 發布準備

### 8. 技術要求

#### 8.1 Android 要求
- Android API Level: 24+
- OpenGL ES 3.2+
- RAM: 最低 4GB
- 存儲空間: 最低 2GB

#### 8.2 iOS 要求
- iOS 12.0+
- Metal 支持
- iPhone 6s 及以上
- iPad Air 2 及以上

#### 8.3 性能目標
- Android: 30 FPS 穩定
- iOS: 60 FPS 目標
- 記憶使用: < 1GB
- 啟動時間: < 10秒
