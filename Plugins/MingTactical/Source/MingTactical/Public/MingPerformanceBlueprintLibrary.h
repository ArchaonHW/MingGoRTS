#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingPerformanceBlueprintLibrary.generated.h"

class UMingInstancedRenderingComponent;
class UMingSpatialPartitionComponent;
class UMingPerformanceMonitor;
class AMingTacticalUnit;

/**
 * 摧毀?X??X??? * 故事重要性?目標數量?X?
 */
UCLASS()
class MINGTACTICAL_API UMingPerformanceBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ???X?X摧毀??     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static UMingPerformanceMonitor* GetPerformanceMonitor();

    /**
     * ???X?ʯX??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void InitializePerformanceMonitor();

    /**
     * 目標數量FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetCurrentFPS();

    /**
     * 摧毀??FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetAverageFPS();

    /**
     * ???X?CFPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMinFPS();

    /**
     * ???X?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMaxFPS();

    /**
     * 摧毀V摧毀 (@??)
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetFrameTimeMs();

    /**
     * 目標數量?X???
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetPerformanceReport();

    /**
     * 摧毀²u摧毀??
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetShortPerformanceText();

    /**
     * ??_摧毀??
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static bool IsPerformanceAcceptable();

    /**
     * ]m摧毀FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void SetTargetFPS(float TargetFPS);

    /**
     * ???X?O??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void StartPerformanceRecording();

    /**
     * 摧毀O??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void StopPerformanceRecording();

    /**
     * ɥX摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static bool ExportPerformanceReport(const FString& FilePath);

    /**
     * 摧毀OX(MB)
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMemoryUsageMB();

    /**
     * 摧毀目標數量
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetRenderedInstanceCount();

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetCulledInstanceCount();

    /**
     * 摧毀摧毀??
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetActiveGridCellCount();

    /**
     * ?X??X摧毀?t
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void RegisterUnitForInstancedRendering(AMingTacticalUnit* Unit);

    /**
     * q摧毀tβ摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void UnregisterUnitFromInstancedRendering(AMingTacticalUnit* Unit);

    /**
     * ]m故事重要性     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void SetUnitVisibility(AMingTacticalUnit* Unit, bool bVisible);

    /**
     * 目標數量LODŧO
     */
    UFUNCTION(BlueprintPure, Category = "LOD")
    static int32 GetUnitLODLevel(AMingTacticalUnit* Unit);

    /**
     * ]mLODZ摧毀     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    static void SetLODDistanceThresholds(const TArray<float>& Thresholds);

    /**
     * ?X??X摧毀?t??     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void RegisterUnitForSpatialPartition(AMingTacticalUnit* Unit);

    /**
     * q摧毀tβ摧毀     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void UnregisterUnitFromSpatialPartition(AMingTacticalUnit* Unit);

    /**
     * 故事選項X??X???
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static TArray<AMingTacticalUnit*> GetUnitsInRadius(const FVector& Center, float Radius);

    /**
     * ]m?X???j??
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void SetSpatialPartitionCellSize(float CellSize);

    /**
     * ]m?X??X?s摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void SetSpatialPartitionUpdateRadius(float Radius);

    /**
     * 摧毀ոո??
     */
    UFUNCTION(BlueprintCallable, Category = "Debug")
    static void GetDebugGridInfo(TArray<FVector>& CellCenters, TArray<int32>& CellUnitCounts);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetOptimizationSuggestions();
};

