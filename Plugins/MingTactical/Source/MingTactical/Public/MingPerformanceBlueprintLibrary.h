#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingPerformanceBlueprintLibrary.generated.h"

class UMingInstancedRenderingComponent;
class UMingSpatialPartitionComponent;
class UMingPerformanceMonitor;
class AMingTacticalUnit;

/**
 * ?ßËÉΩ?™Â??çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á??ßËÉΩ?™Â??üËÉΩ
 */
UCLASS()
class MINGTACTICAL_API UMingPerformanceBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â??ßËÉΩ??éß?®ÂØ¶‰æ?     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static UMingPerformanceMonitor* GetPerformanceMonitor();

    /**
     * ?ùÂ??ñÊÄßËÉΩ??éß
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void InitializePerformanceMonitor();

    /**
     * ?≤Â??∂Â?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetCurrentFPS();

    /**
     * ?≤Â?Âπ≥Â?FPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetAverageFPS();

    /**
     * ?≤Â??Ä‰ΩéFPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMinFPS();

    /**
     * ?≤Â??ÄÈ´òFPS
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMaxFPS();

    /**
     * ?≤Â?ÂπÄ?ÇÈ? (ÊØ´Á?)
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetFrameTimeMs();

    /**
     * ?≤Â??ßËÉΩ?±Â??áÂ?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetPerformanceReport();

    /**
     * ?≤Â?Á∞°Áü≠?ßËÉΩ?áÂ?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetShortPerformanceText();

    /**
     * ?ØÂê¶?ßËÉΩ?îÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static bool IsPerformanceAcceptable();

    /**
     * Ë®≠ÁΩÆ?ÆÊ?FPS
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void SetTargetFPS(float TargetFPS);

    /**
     * ?ãÂ??ßËÉΩË®òÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void StartPerformanceRecording();

    /**
     * ?úÊ≠¢?ßËÉΩË®òÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static void StopPerformanceRecording();

    /**
     * Â∞éÂá∫?ßËÉΩ?±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    static bool ExportPerformanceReport(const FString& FilePath);

    /**
     * ?≤Â?Ë®òÊÜ∂È´î‰Ωø??(MB)
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static float GetMemoryUsageMB();

    /**
     * ?≤Â?Ê∏≤Ê?ÂØ¶‰??∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetRenderedInstanceCount();

    /**
     * ?≤Â??îÈô§ÂØ¶‰??∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetCulledInstanceCount();

    /**
     * ?≤Â?Ê¥ªË?Á∂≤Ê†º?∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static int32 GetActiveGridCellCount();

    /**
     * Ë®ªÂ??Æ‰??∞ÂØ¶‰æãÂ?Ê∏≤Ê?Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void RegisterUnitForInstancedRendering(AMingTacticalUnit* Unit);

    /**
     * ÂæûÂØ¶‰æãÂ?Ê∏≤Ê?Á≥ªÁµ±ÁßªÈô§?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void UnregisterUnitFromInstancedRendering(AMingTacticalUnit* Unit);

    /**
     * Ë®≠ÁΩÆ?Æ‰??ØË???     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    static void SetUnitVisibility(AMingTacticalUnit* Unit, bool bVisible);

    /**
     * ?≤Â??Æ‰?LODÁ¥öÂà•
     */
    UFUNCTION(BlueprintPure, Category = "LOD")
    static int32 GetUnitLODLevel(AMingTacticalUnit* Unit);

    /**
     * Ë®≠ÁΩÆLODË∑ùÈõ¢?æÂÄ?     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    static void SetLODDistanceThresholds(const TArray<float>& Thresholds);

    /**
     * Ë®ªÂ??Æ‰??∞Á©∫?ìÂ?Â°äÁ≥ªÁµ?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void RegisterUnitForSpatialPartition(AMingTacticalUnit* Unit);

    /**
     * ÂæûÁ©∫?ìÂ?Â°äÁ≥ªÁµ±Áßª?§ÂñÆ‰Ω?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void UnregisterUnitFromSpatialPartition(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??áÂ??äÂ??ßÁ??Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static TArray<AMingTacticalUnit*> GetUnitsInRadius(const FVector& Center, float Radius);

    /**
     * Ë®≠ÁΩÆÁ©∫È??ÜÂ?Á∂≤Ê†ºÂ§ßÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void SetSpatialPartitionCellSize(float CellSize);

    /**
     * Ë®≠ÁΩÆÁ©∫È??ÜÂ??¥Êñ∞?äÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    static void SetSpatialPartitionUpdateRadius(float Radius);

    /**
     * ?≤Â?Á∂≤Ê†ºË™øË©¶Ë≥áË?
     */
    UFUNCTION(BlueprintCallable, Category = "Debug")
    static void GetDebugGridInfo(TArray<FVector>& CellCenters, TArray<int32>& CellUnitCounts);

    /**
     * ?≤Â??™Â?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    static FString GetOptimizationSuggestions();
};
