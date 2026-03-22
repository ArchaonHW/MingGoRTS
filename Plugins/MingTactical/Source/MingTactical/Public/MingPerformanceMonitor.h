#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPerformanceMonitor.generated.h"

/**
 * ?§èƒ½?‡æ?çµæ?
 */
USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()

    // ?¶å?FPS
    UPROPERTY(BlueprintReadOnly)
    float CurrentFPS;

    // å¹³å?FPS
    UPROPERTY(BlueprintReadOnly)
    float AverageFPS;

    // ?€ä½FPS
    UPROPERTY(BlueprintReadOnly)
    float MinFPS;

    // ?€é«˜FPS
    UPROPERTY(BlueprintReadOnly)
    float MaxFPS;

    // å¹€?‚é? (æ¯«ç?)
    UPROPERTY(BlueprintReadOnly)
    float FrameTimeMs;

    // å¹³å?å¹€?‚é?
    UPROPERTY(BlueprintReadOnly)
    float AverageFrameTimeMs;

    // æ¸²æ?ç·šç??‚é?
    UPROPERTY(BlueprintReadOnly)
    float RenderThreadTimeMs;

    // ?Šæˆ²ç·šç??‚é?
    UPROPERTY(BlueprintReadOnly)
    float GameThreadTimeMs;

    // GPU?‚é?
    UPROPERTY(BlueprintReadOnly)
    float GPUTimeMs;

    // ä½¿ç”¨?„è??¶é? (MB)
    UPROPERTY(BlueprintReadOnly)
    float UsedMemoryMB;

    // æ¸²æ??„å¯¦ä¾‹æ•¸??    UPROPERTY(BlueprintReadOnly)
    int32 RenderedInstanceCount;

    // ?”é™¤?„å¯¦ä¾‹æ•¸??    UPROPERTY(BlueprintReadOnly)
    int32 CulledInstanceCount;

    // æ´»è?ç¶²æ ¼?¸é?
    UPROPERTY(BlueprintReadOnly)
    int32 ActiveGridCellCount;

    // ç¸½ç¶²?¼æ•¸??    UPROPERTY(BlueprintReadOnly)
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
 * ?§èƒ½??§?? * å¯¦æ???§?Šæˆ²?§èƒ½?‡æ?
 */
UCLASS(BlueprintType)
class MINGTACTICAL_API UMingPerformanceMonitor : public UObject
{
    GENERATED_BODY()

public:
    UMingPerformanceMonitor(};

    virtual void BeginDestroy() override;

    /**
     * ?å??–æ€§èƒ½??§
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Initialize(};

    /**
     * ?œé??§èƒ½??§
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Shutdown(};

    /**
     * ?´æ–°?§èƒ½?¸æ? (æ¯å?èª¿ç”¨)
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void Update(float DeltaTime};

    /**
     * ?²å??¶å??§èƒ½?‡æ?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FPerformanceMetrics GetCurrentMetrics() const { return CurrentMetrics; }

    /**
     * ?²å??§èƒ½?±å??‡å?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FString GetPerformanceReport() const;

    /**
     * ?²å?ç°¡çŸ­?§èƒ½?‡å?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    FString GetShortPerformanceText() const;

    /**
     * ?¯å¦?§èƒ½?”æ?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    bool IsPerformanceAcceptable() const;

    /**
     * è¨­ç½®?®æ?FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTargetFPS(float TargetFPS};

    /**
     * ?²å??®æ?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetTargetFPS() const { return TargetFPS; }

    /**
     * ?‹å??§èƒ½è¨˜é?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StartRecording(};

    /**
     * ?œæ­¢?§èƒ½è¨˜é?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void StopRecording(};

    /**
     * ?²å?è¨˜é??„æ•¸??     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    TArray<FPerformanceMetrics> GetRecordedData() const;

    /**
     * å°å‡º?§èƒ½?±å??°æ?ä»?     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    bool ExportReport(const FString& FilePath) const;

    /**
     * æ·»å??ªå?ç¾©æ€§èƒ½æ¨™è?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void AddPerformanceMarker(const FString& MarkerName, float Value};

    /**
     * ?²å??ªå?ç¾©æ?è¨˜å€?     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    float GetPerformanceMarker(const FString& MarkerName) const;

    /**
     * ?œæ??²å?å¯¦ä?
     */
    static UMingPerformanceMonitor* Get(};

protected:
    // ?¶å??§èƒ½?‡æ?
    UPROPERTY()
    FPerformanceMetrics CurrentMetrics;

    // ?®æ?FPS
    UPROPERTY()
    float TargetFPS;

    // ?¯å¦æ­?œ¨è¨˜é?
    UPROPERTY()
    bool bIsRecording;

    // è¨˜é??„æ•¸??    UPROPERTY()
    TArray<FPerformanceMetrics> RecordedData;

    // ?€å¤§è??„å???    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MaxRecordedFrames;

    // ?ªå?ç¾©æ€§èƒ½æ¨™è?
    UPROPERTY()
    TMap<FString, float> CustomMarkers;

    // ?®ä?å¯¦ä?
    static UMingPerformanceMonitor* Instance;

    // è¨ˆç?FPS
    void CalculateFPS(float DeltaTime};

    // ?´æ–°è¨˜æ†¶é«”ä½¿??    void UpdateMemoryUsage(};

    // ?´æ–°ç·šç??‚é?
    void UpdateThreadTimes(};

    // è¨˜é??¸æ?é»?    void RecordDataPoint(};

    // ?²å?è¨˜æ†¶é«”ä½¿??    float GetMemoryUsageMB() const;

    // ?²å?æ¸²æ?ç·šç??‚é?
    float GetRenderThreadTime() const;

    // ?²å??Šæˆ²ç·šç??‚é?
    float GetGameThreadTime() const;

    // ?²å?GPU?‚é?
    float GetGPUTime() const;

    // ?²å?æ¸²æ?å¯¦ä??¸é?
    int32 GetRenderedInstanceCount() const;

    // ?²å??”é™¤å¯¦ä??¸é?
    int32 GetCulledInstanceCount() const;
};

