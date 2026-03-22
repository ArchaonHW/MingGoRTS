#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceMonitor.generated.h"

/**
 * 性能指標結構
 */
USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()

    // 當前FPS
    UPROPERTY(BlueprintReadOnly)
    float CurrentFPS;

    // 平均FPS
    UPROPERTY(BlueprintReadOnly)
    float AverageFPS;

    // 最低FPS
    UPROPERTY(BlueprintReadOnly)
    float MinFPS;

    // 最高FPS
    UPROPERTY(BlueprintReadOnly)
    float MaxFPS;

    // 幀時間 (毫秒)
    UPROPERTY(BlueprintReadOnly)
    float FrameTimeMs;

    // 平均幀時間
    UPROPERTY(BlueprintReadOnly)
    float AverageFrameTimeMs;

    // 渲染線程時間
    UPROPERTY(BlueprintReadOnly)
    float RenderThreadTimeMs;

    // 遊戲線程時間
    UPROPERTY(BlueprintReadOnly)
    float GameThreadTimeMs;

    // GPU時間
    UPROPERTY(BlueprintReadOnly)
    float GPUTimeMs;

    // 使用的記憶體 (MB)
    UPROPERTY(BlueprintReadOnly)
    float UsedMemoryMB;

    // 渲染的實例數量
    UPROPERTY(BlueprintReadOnly)
    int32 RenderedInstanceCount;

    // 剔除的實例數量
    UPROPERTY(BlueprintReadOnly)
    int32 CulledInstanceCount;

    // 活躍網格數量
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveGridCellCount;

    // 總網格數量
    UPROPERTY(BlueprintReadOnly)
    int32 TotalGridCellCount;

    FPerformanceMetrics()
        : CurrentFPS(60.0f)
        , AverageFPS(60.0f)
        , MinFPS(60.0f)
        , MaxFPS(60.0f)
        , FrameTimeMs(16.67f)
        , AverageFrameTimeMs(16.67f)
        , RenderThreadTimeMs(0.0f)
        , GameThreadTimeMs(0.0f)
        , GPUTimeMs(0.0f)
        , UsedMemoryMB(0.0f)
        , RenderedInstanceCount(0)
        , CulledInstanceCount(0)
        , ActiveGridCellCount(0)
        , TotalGridCellCount(0)
    {}
};

/**
 * 性能監控器
 * 實時監控遊戲性能指標
 */
UCLASS(BlueprintType)
class MINGTACTICAL_API UMingPerformanceMonitor : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceMonitor();

    virtual void BeginDestroy() override;

    /**
     * 初始化性能監控
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Initialize();

    /**
     * 關閉性能監控
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Shutdown();

    /**
     * 更新性能數據 (每幀調用)
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Update(float DeltaTime);

    /**
     * 獲取當前性能指標
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FPerformanceMetrics GetCurrentMetrics() const { return CurrentMetrics; }

    /**
     * 獲取性能報告文字
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FString GetPerformanceReport() const;

    /**
     * 獲取簡短性能文字
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FString GetShortPerformanceText() const;

    /**
     * 是否性能達標
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    bool IsPerformanceAcceptable() const;

    /**
     * 設置目標FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTargetFPS(float TargetFPS);

    /**
     * 獲取目標FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetTargetFPS() const { return TargetFPS; }

    /**
     * 開始性能記錄
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartRecording();

    /**
     * 停止性能記錄
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopRecording();

    /**
     * 獲取記錄的數據
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    TArray<FPerformanceMetrics> GetRecordedData() const;

    /**
     * 導出性能報告到文件
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    bool ExportReport(const FString& FilePath) const;

    /**
     * 添加自定義性能標記
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void AddPerformanceMarker(const FString& MarkerName, float Value);

    /**
     * 獲取自定義標記值
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetPerformanceMarker(const FString& MarkerName) const;

    /**
     * 靜態獲取實例
     */
    static UMingPerformanceMonitor* Get();

protected:
    // 當前性能指標
    UPROPERTY()
    FPerformanceMetrics CurrentMetrics;

    // 目標FPS
    UPROPERTY()
    float TargetFPS;

    // 是否正在記錄
    UPROPERTY()
    bool bIsRecording;

    // 記錄的數據
    UPROPERTY()
    TArray<FPerformanceMetrics> RecordedData;

    // 最大記錄幀數
    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MaxRecordedFrames;

    // 自定義性能標記
    UPROPERTY()
    TMap<FString, float> CustomMarkers;

    // 單例實例
    static UMingPerformanceMonitor* Instance;

    // 計算FPS
    void CalculateFPS(float DeltaTime);

    // 更新記憶體使用
    void UpdateMemoryUsage();

    // 更新線程時間
    void UpdateThreadTimes();

    // 記錄數據點
    void RecordDataPoint();

    // 獲取記憶體使用
    float GetMemoryUsageMB() const;

    // 獲取渲染線程時間
    float GetRenderThreadTime() const;

    // 獲取遊戲線程時間
    float GetGameThreadTime() const;

    // 獲取GPU時間
    float GetGPUTime() const;

    // 獲取渲染實例數量
    int32 GetRenderedInstanceCount() const;

    // 獲取剔除實例數量
    int32 GetCulledInstanceCount() const;
};
