#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceMonitor.generated.h"

/**
 * ?�能?��?結�?
 */
USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()

    // ?��?FPS
    UPROPERTY(BlueprintReadOnly)
    float CurrentFPS;

    // 平�?FPS
    UPROPERTY(BlueprintReadOnly)
    float AverageFPS;

    // ?�低FPS
    UPROPERTY(BlueprintReadOnly)
    float MinFPS;

    // ?�高FPS
    UPROPERTY(BlueprintReadOnly)
    float MaxFPS;

    // 幀?��? (毫�?)
    UPROPERTY(BlueprintReadOnly)
    float FrameTimeMs;

    // 平�?幀?��?
    UPROPERTY(BlueprintReadOnly)
    float AverageFrameTimeMs;

    // 渲�?線�X��?
    UPROPERTY(BlueprintReadOnly)
    float RenderThreadTimeMs;

    // ?�戲線�X��?
    UPROPERTY(BlueprintReadOnly)
    float GameThreadTimeMs;

    // GPU?��?
    UPROPERTY(BlueprintReadOnly)
    float GPUTimeMs;

    // 使用?��X��? (MB)
    UPROPERTY(BlueprintReadOnly)
    float UsedMemoryMB;

    // 渲�X�實例數X    UPROPERTY(BlueprintReadOnly)
    int32 RenderedInstanceCount;

    // ?�除?�實例數X    UPROPERTY(BlueprintReadOnly)
    int32 CulledInstanceCount;

    // 活�?網格?��?
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveGridCellCount;

    // 總網?�數X    UPROPERTY(BlueprintReadOnly)
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
 * ?�能X��X * 實�X��?�戲?�能?��?
 */
UCLASS(BlueprintType)
class MINGTACTICAL_API UMingPerformanceMonitor : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceMonitor(};

    virtual void BeginDestroy() override;

    /**
     * ?��X�性能X��
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Initialize(};

    /**
     * ?��X�能X��
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Shutdown(};

    /**
     * ?�新?�能?��? (每�?調用)
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Update(float DeltaTime};

    /**
     * ?��X��X�能?��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FPerformanceMetrics GetCurrentMetrics() const { return CurrentMetrics; }

    /**
     * ?��X�能?��X��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FString GetPerformanceReport() const;

    /**
     * ?��?簡短?�能?��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FString GetShortPerformanceText() const;

    /**
     * ?�否?�能?��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    bool IsPerformanceAcceptable() const;

    /**
     * 設置?��?FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTargetFPS(float TargetFPS};

    /**
     * ?��X��?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetTargetFPS() const { return TargetFPS; }

    /**
     * ?��X�能記�?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartRecording(};

    /**
     * ?�止?�能記�?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopRecording(};

    /**
     * ?��?記�X�數X     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    TArray<FPerformanceMetrics> GetRecordedData() const;

    /**
     * 導出?�能?��X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    bool ExportReport(const FString& FilePath) const;

    /**
     * 添�X��?義性能標�?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void AddPerformanceMarker(const FString& MarkerName, float Value};

    /**
     * ?��X��?義�?記�?     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetPerformanceMarker(const FString& MarkerName) const;

    /**
     * ?��X��?實�?
     */
    static UMingPerformanceMonitor* Get(};

protected:
    // ?��X�能?��?
    UPROPERTY()
    FPerformanceMetrics CurrentMetrics;

    // ?��?FPS
    UPROPERTY()
    float TargetFPS;

    // ?�否�?��記�?
    UPROPERTY()
    bool bIsRecording;

    // 記�X�數X    UPROPERTY()
    TArray<FPerformanceMetrics> RecordedData;

    // ?�大�X��X    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MaxRecordedFrames;

    // ?��?義性能標�?
    UPROPERTY()
    TMap<FString, float> CustomMarkers;

    // ?��?實�?
    static UMingPerformanceMonitor* Instance;

    // 計�?FPS
    void CalculateFPS(float DeltaTime};

    // ?�新記憶體使X    void UpdateMemoryUsage(};

    // ?�新線�X��?
    void UpdateThreadTimes(};

    // 記�X��?�?    void RecordDataPoint(};

    // ?��?記憶體使X    float GetMemoryUsageMB() const;

    // ?��?渲�?線�X��?
    float GetRenderThreadTime() const;

    // ?��X�戲線�X��?
    float GetGameThreadTime() const;

    // ?��?GPU?��?
    float GetGPUTime() const;

    // ?��?渲�?實�X��?
    int32 GetRenderedInstanceCount() const;

    // ?��X�除實�X��?
    int32 GetCulledInstanceCount() const;
};

