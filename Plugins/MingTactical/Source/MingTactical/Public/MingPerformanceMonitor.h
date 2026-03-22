#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceMonitor.generated.h"

/**
 * ??��摧毀��??
 */
USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()

    // 摧毀FPS
    UPROPERTY(BlueprintReadOnly)
    float CurrentFPS;

    // ��??FPS
    UPROPERTY(BlueprintReadOnly)
    float AverageFPS;

    // ??�CFPS
    UPROPERTY(BlueprintReadOnly)
    float MinFPS;

    // ??��FPS
    UPROPERTY(BlueprintReadOnly)
    float MaxFPS;

    // �V摧毀 (�@??)
    UPROPERTY(BlueprintReadOnly)
    float FrameTimeMs;

    // ��??�V摧毀
    UPROPERTY(BlueprintReadOnly)
    float AverageFrameTimeMs;

    // ��??�u?X???
    UPROPERTY(BlueprintReadOnly)
    float RenderThreadTimeMs;

    // ??���u?X???
    UPROPERTY(BlueprintReadOnly)
    float GameThreadTimeMs;

    // GPU摧毀
    UPROPERTY(BlueprintReadOnly)
    float GPUTimeMs;

    // �ϥ�目標數量 (MB)
    UPROPERTY(BlueprintReadOnly)
    float UsedMemoryMB;

    // ��?X?��Ҽ�X
    UPROPERTY(BlueprintReadOnly)
    int32 RenderedInstanceCount;

    // ??��??��Ҽ�X
    UPROPERTY(BlueprintReadOnly)
    int32 CulledInstanceCount;

    // ��??���摧毀
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveGridCellCount;

    // �`��??��X
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
 * ??��X??X * ��?X摧毀��??��摧毀
 */
UCLASS(BlueprintType)
class MINGTACTICAL_API UMingPerformanceMonitor : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceMonitor();

    virtual void BeginDestroy() override;

    /**
     * ???X?�ʯ�X??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Initialize();

    /**
     * ???X?��X??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Shutdown();

    /**
     * ??�s??��摧毀 (�C??�ե�)
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Update(float DeltaTime);

    /**
     * 故事重要性?��摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FPerformanceMetrics GetCurrentMetrics() const { return CurrentMetrics; }

    /**
     * ???X?��目標數量
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FString GetPerformanceReport() const;

    /**
     * 摧毀²�u??��摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FString GetShortPerformanceText() const;

    /**
     * ??�_??��摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    bool IsPerformanceAcceptable() const;

    /**
     * �]�m摧毀FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTargetFPS(float TargetFPS);

    /**
     * 目標數量FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetTargetFPS() const { return TargetFPS; }

    /**
     * ???X?��O??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartRecording();

    /**
     * ??��??��O??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopRecording();

    /**
     * 摧毀�O?X?��X     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    TArray<FPerformanceMetrics> GetRecordedData() const;

    /**
     * �ɥX??��目標數量??     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    bool ExportReport(const FString& FilePath) const;

    /**
     * �K?X???�q�ʯ��??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void AddPerformanceMarker(const FString& MarkerName, float Value);

    /**
     * 目標數量�q??�O??     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetPerformanceMarker(const FString& MarkerName) const;

    /**
     * 目標數量��??
     */
    static UMingPerformanceMonitor* Get();

protected:
    // ???X?��摧毀
    UPROPERTY()
    FPerformanceMetrics CurrentMetrics;

    // 摧毀FPS
    UPROPERTY()
    float TargetFPS;

    // ??�_摧毀�O??
    UPROPERTY()
    bool bIsRecording;

    // �O?X?��X
    UPROPERTY()
    TArray<FPerformanceMetrics> RecordedData;

    // ??�j?X??X
    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MaxRecordedFrames;

    // 摧毀�q�ʯ��??
    UPROPERTY()
    TMap<FString, float> CustomMarkers;

    // 摧毀��??
    static UMingPerformanceMonitor* Instance;

    // �p??FPS
    void CalculateFPS(float DeltaTime);

    // ??�s�O�����X
    void UpdateMemoryUsage();

    // ??�s�u?X???
    void UpdateThreadTimes();

    // �O?X摧毀?
    void RecordDataPoint();

    // 摧毀�O�����X
    float GetMemoryUsageMB() const;

    // 摧毀��??�u?X???
    float GetRenderThreadTime() const;

    // ???X?���u?X???
    float GetGameThreadTime() const;

    // 摧毀GPU摧毀
    float GetGPUTime() const;

    // 摧毀��??��?X???
    int32 GetRenderedInstanceCount() const;

    // ???X?����?X???
    int32 GetCulledInstanceCount() const;
};

