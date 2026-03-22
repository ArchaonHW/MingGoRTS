#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingPerformanceBlueprintLibrary.generated.h"

class UMingInstancedRenderingComponent;
class UMingSpatialPartitionComponent;
class UMingPerformanceMonitor;
class AMingTacticalUnit;

/**
 * 性能優化藍圖函數庫
 * 提供藍圖可調用的性能優化功能
 */
UCLASS()
class MINGTACTICAL_API UMingPerformanceBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取性能監控器實例
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static UMingPerformanceMonitor* GetPerformanceMonitor();

    /**
     * 初始化性能監控
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void InitializePerformanceMonitor();

    /**
     * 獲取當前FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetCurrentFPS();

    /**
     * 獲取平均FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetAverageFPS();

    /**
     * 獲取最低FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMinFPS();

    /**
     * 獲取最高FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMaxFPS();

    /**
     * 獲取幀時間 (毫秒)
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetFrameTimeMs();

    /**
     * 獲取性能報告文字
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetPerformanceReport();

    /**
     * 獲取簡短性能文字
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetShortPerformanceText();

    /**
     * 是否性能達標
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static bool IsPerformanceAcceptable();

    /**
     * 設置目標FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void SetTargetFPS(float TargetFPS);

    /**
     * 開始性能記錄
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void StartPerformanceRecording();

    /**
     * 停止性能記錄
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void StopPerformanceRecording();

    /**
     * 導出性能報告
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static bool ExportPerformanceReport(const FString& FilePath);

    /**
     * 獲取記憶體使用 (MB)
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMemoryUsageMB();

    /**
     * 獲取渲染實例數量
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetRenderedInstanceCount();

    /**
     * 獲取剔除實例數量
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetCulledInstanceCount();

    /**
     * 獲取活躍網格數量
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetActiveGridCellCount();

    /**
     * 註冊單位到實例化渲染系統
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void RegisterUnitForInstancedRendering(AMingTacticalUnit* Unit);

    /**
     * 從實例化渲染系統移除單位
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void UnregisterUnitFromInstancedRendering(AMingTacticalUnit* Unit);

    /**
     * 設置單位可見性
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void SetUnitVisibility(AMingTacticalUnit* Unit, bool bVisible);

    /**
     * 獲取單位LOD級別
     */
    UFUNCTION(BlueprintPure, Category = "LOD")
    static int32 GetUnitLODLevel(AMingTacticalUnit* Unit);

    /**
     * 設置LOD距離閾值
     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    static void SetLODDistanceThresholds(const TArray<float>& Thresholds);

    /**
     * 註冊單位到空間分塊系統
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void RegisterUnitForSpatialPartition(AMingTacticalUnit* Unit);

    /**
     * 從空間分塊系統移除單位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void UnregisterUnitFromSpatialPartition(AMingTacticalUnit* Unit);

    /**
     * 獲取指定半徑內的單位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static TArray<AMingTacticalUnit*> GetUnitsInRadius(const FVector& Center, float Radius);

    /**
     * 設置空間分塊網格大小
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void SetSpatialPartitionCellSize(float CellSize);

    /**
     * 設置空間分塊更新半徑
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void SetSpatialPartitionUpdateRadius(float Radius);

    /**
     * 獲取網格調試資訊
     */
    UFUNCTION(BlueprintCallable, Category = "Debug")
    static void GetDebugGridInfo(TArray<FVector>& CellCenters, TArray<int32>& CellUnitCounts);

    /**
     * 獲取優化建議
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetOptimizationSuggestions();
};
