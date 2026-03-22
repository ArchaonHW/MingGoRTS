#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingPerformanceBlueprintLibrary.generated.h"

class UMingInstancedRenderingComponent;
class UMingSpatialPartitionComponent;
class UMingPerformanceMonitor;
class AMingTacticalUnit;

/**
 * ?�能?��X��X�數�? * ?��X��X�調?��X�能?��X�能
 */
UCLASS()
class MINGTACTICAL_API UMingPerformanceBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?��X�能X��?�實�?     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static UMingPerformanceMonitor* GetPerformanceMonitor(};

    /**
     * ?��X�性能X��
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void InitializePerformanceMonitor(};

    /**
     * ?��X��?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetCurrentFPS(};

    /**
     * ?��?平�?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetAverageFPS(};

    /**
     * ?��X�低FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMinFPS(};

    /**
     * ?��X�高FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMaxFPS(};

    /**
     * ?��?幀?��? (毫�?)
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetFrameTimeMs(};

    /**
     * ?��X�能?��X��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetPerformanceReport(};

    /**
     * ?��?簡短?�能?��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetShortPerformanceText(};

    /**
     * ?�否?�能?��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static bool IsPerformanceAcceptable(};

    /**
     * 設置?��?FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void SetTargetFPS(float TargetFPS};

    /**
     * ?��X�能記�?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void StartPerformanceRecording(};

    /**
     * ?�止?�能記�?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void StopPerformanceRecording(};

    /**
     * 導出?�能?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static bool ExportPerformanceReport(const FString& FilePath};

    /**
     * ?��?記憶體使X(MB)
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMemoryUsageMB(};

    /**
     * ?��?渲�?實�X��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetRenderedInstanceCount(};

    /**
     * ?��X�除實�X��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetCulledInstanceCount(};

    /**
     * ?��?活�?網格?��?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetActiveGridCellCount(};

    /**
     * 註�X��X�實例�?渲�?系統
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void RegisterUnitForInstancedRendering(AMingTacticalUnit* Unit};

    /**
     * 從實例�?渲�?系統移除?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void UnregisterUnitFromInstancedRendering(AMingTacticalUnit* Unit};

    /**
     * 設置?��X��X     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void SetUnitVisibility(AMingTacticalUnit* Unit, bool bVisible};

    /**
     * ?��X��?LOD級別
     */
    UFUNCTION(BlueprintPure, Category = "LOD")
    static int32 GetUnitLODLevel(AMingTacticalUnit* Unit};

    /**
     * 設置LOD距離?��?     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    static void SetLODDistanceThresholds(const TArray<float>& Thresholds};

    /**
     * 註�X��X�空?��?塊系�?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void RegisterUnitForSpatialPartition(AMingTacticalUnit* Unit};

    /**
     * 從空?��?塊系統移?�單�?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void UnregisterUnitFromSpatialPartition(AMingTacticalUnit* Unit};

    /**
     * ?��X��X��X��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static TArray<AMingTacticalUnit*> GetUnitsInRadius(const FVector& Center, float Radius};

    /**
     * 設置空�X��?網格大�?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void SetSpatialPartitionCellSize(float CellSize};

    /**
     * 設置空�X��X�新?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void SetSpatialPartitionUpdateRadius(float Radius};

    /**
     * ?��?網格調試資�?
     */
    UFUNCTION(BlueprintCallable, Category = "Debug")
    static void GetDebugGridInfo(TArray<FVector>& CellCenters, TArray<int32>& CellUnitCounts};

    /**
     * ?��X��?建議
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetOptimizationSuggestions(};
};

