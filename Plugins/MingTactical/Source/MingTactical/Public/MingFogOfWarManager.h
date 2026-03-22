#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFogOfWarManager.generated.h"

// Ë¶ñÈ?È°ûÂ?
UENUM(BlueprintType)
enum class EMingVisionType : uint8
{
    Normal,            // Ê≠?∏∏Ë¶ñÈ?
    Enhanced,          // Â¢ûÂº∑Ë¶ñÈ?
    NightVision,        // Â§úË?
    Thermal,           // ?±Ê???    Satellite,         // Ë°õÊ?Ë¶ñÈ?
    Aerial             // Á©∫‰∏≠?µÂ?
};

// Ëø∑ÈúßÁ≠âÁ?
UENUM(BlueprintType)
enum class EMingFogLevel : uint8
{
    Clear,             // Ê∏ÖÊô∞
    Light,             // ËºïÈúß
    Medium,            // ‰∏≠Èúß
    Heavy,             // ÊøÉÈúß
    Complete           // ÂÆåÂÖ®Ëø∑Èúß
};

// ?∞ÂΩ¢È°ûÂ?
UENUM(BlueprintType)
enum class EMingTerrainType : uint8
{
    Open,              // ?ãÈ??∞ÂΩ¢
    Forest,            // Ê£ÆÊ?
    Mountain,          // Â±±Âú∞
    Urban,             // ?éÂ?
    Desert,            // Ê≤ôÊ?
    Water,             // Ê∞¥Â?
    Swamp,             // Ê≤ºÊæ§
    Snow               // ?™Âú∞
};

// ?µÂ??Ä??UENUM(BlueprintType)
enum class EMingScoutingStatus : uint8
{
    Idle,              // Á©∫È?
    Scouting,          // ?µÂ?‰∏?    Observing,         // ËßÄÂØü‰∏≠
    Reporting,          // ?±Â?‰∏?    Hidden,            // ?±ËîΩ
    Detected           // Ë¢´Áôº??};

// Ë¶ñÈ??∏Ê?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingVisionData
{
    GENERATED_BODY()

    // ?Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // Ë¶ñÈ??äÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionRadius;

    // Ë¶ñÈ?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingVisionType VisionType;

    // Ë¶ñÈ?ËßíÂ∫¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionAngle;

    // Ë¶ñÈ??ÅÁ??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionDuration;

    // ?ØÂê¶?∫Ê?Á∫åË???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPersistent;

    // Ë¶ñÈ??¥Êñ∞?ìÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionUpdateInterval;

    // ?ØË??Æ‰??óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> VisibleUnits;

    // Â∑≤ÂÅµÂØüÂ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> ScoutedAreas;

    // ?ÄÂæåÊõ¥?∞Ê???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastUpdateTime;

    FMingVisionData()
    {
        UnitID = -1;
        VisionRadius = 0.0f;
        VisionType = EMingVisionType::Normal;
        VisionAngle = 360.0f;
        VisionDuration = 0.0f;
        bIsPersistent = false;
        VisionUpdateInterval = 0.1f;
        VisibleUnits.Empty(};
        ScoutedAreas.Empty(};
        LastUpdateTime = FDateTime::Now(};
    }
};

// Ëø∑ÈúßÁ∂≤Ê†º?∏Ê?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFogGridCell
{
    GENERATED_BODY()

    // Á∂≤Ê†ºÂ∫ßÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint GridPosition;

    // Ëø∑ÈúßÁ≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFogLevel FogLevel;

    // ?ØÂê¶Ë¢´ÂÅµÂØüÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsScouted;

    // ?µÂ??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ScoutedTime;

    // ?µÂ??Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoutingUnitID;

    // ?∞ÂΩ¢È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTerrainType TerrainType;

    // ?∞ÂΩ¢?ÆËîΩ??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrainOcclusion;

    // Âª∫Á??©ÈÅÆ?ΩÂÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StructureOcclusion;

    // ?ÄÂæåÂèØË¶ãÊ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastVisibleTime;

    FMingFogGridCell()
    {
        GridPosition = FIntPoint(0, 0};
        FogLevel = EMingFogLevel::Complete;
        bIsScouted = false;
        ScoutedTime = FDateTime::MinValue(};
        ScoutingUnitID = -1;
        TerrainType = EMingTerrainType::Open;
        TerrainOcclusion = 0.0f;
        StructureOcclusion = 0.0f;
        LastVisibleTime = FDateTime::MinValue(};
    }
};

// ?µÂ??±Â?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingScoutingReport
{
    GENERATED_BODY()

    // ?±Â?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ReportID;

    // ?µÂ??Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoutingUnitID;

    // ?µÂ??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ScoutingTime;

    // ?µÂ?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ScoutingLocation;

    // ?µÂ?ÁØÑÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ScoutingRadius;

    // ?ºÁèæ?ÑÂñÆ‰Ω?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> DiscoveredUnits;

    // ?ºÁèæ?ÑÂª∫ÁØâÁâ©
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> DiscoveredStructures;

    // ?∞ÂΩ¢‰ø°ÊÅØ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> TerrainInfo;

    // Â®ÅË?Ë©ï‰º∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreatAssessment;

    // ?±Â?Ë©≥Á¥∞Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReportDetail;

    // ?±Â??ØÈ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReportReliability;

    FMingScoutingReport()
    {
        ReportID = -1;
        ScoutingUnitID = -1;
        ScoutingTime = FDateTime::Now(};
        ScoutingLocation = FVector::ZeroVector;
        ScoutingRadius = 0.0f;
        DiscoveredUnits.Empty(};
        DiscoveredStructures.Empty(};
        TerrainInfo.Empty(};
        ThreatAssessment = TEXT(""};
        ReportDetail = 0.0f;
        ReportReliability = 1.0f;
    }
};

// ?∞Áà≠Ëø∑Èúß‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFogOfWarUpdated, const FVector&, Location, EMingFogLevel, NewFogLevel};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVisionUpdated, int32, UnitID, const TArray<int32>&, NewVisibleUnits};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoutingCompleted, const FMingScoutingReport&, Report};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitSpotted, int32, SpottingUnitID, int32, SpottedUnitID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitLostSight, int32, LosingUnitID, int32, LostUnitID};

/**
 * ?∞Áà≠Ëø∑ÈúßÁÆ°Á??? * Ë≤†Ë≤¨ÁÆ°Á??∞Áà≠Ëø∑Èúß?ÅË??éÂ??µÂ?Á≥ªÁµ±
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingFogOfWarManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFogOfWarManager(};

    // ?ùÂ??ñÊà∞?≠Ëø∑?ßÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool InitializeFogOfWarSystem(};

    // Ë®ªÂ??Æ‰?Ë¶ñÈ?
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool RegisterUnitVision(int32 UnitID, const FMingVisionData& VisionData};

    // ?¥Êñ∞?Æ‰?Ë¶ñÈ?
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool UpdateUnitVision(int32 UnitID, const FVector& NewPosition};

    // ?≤Â??Æ‰?Ë¶ñÈ?
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    FMingVisionData GetUnitVision(int32 UnitID) const;

    // ?≤Â??ØË??Æ‰?
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<int32> GetVisibleUnits(int32 UnitID) const;

    // Ê™¢Êü•?Æ‰??ØË???    UFUNCTION(BlueprintPure, Category = "Fog of War")
    bool IsUnitVisible(int32 ViewerID, int32 TargetID) const;

    // ?¥Êñ∞?∞Áà≠Ëø∑Èúß
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    void UpdateFogOfWar(};

    // ?≤Â?Ëø∑ÈúßÁ≠âÁ?
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    EMingFogLevel GetFogLevel(const FVector& Location) const;

    // Ë®≠ÁΩÆ?∞ÂΩ¢È°ûÂ?
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SetTerrainType(const FVector& Location, EMingTerrainType TerrainType};

    // Ê∑ªÂ?Âª∫Á??©ÈÅÆ??    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool AddStructureOcclusion(const FVector& Location, float OcclusionValue};

    // ?∑Ë??µÂ?‰ªªÂ?
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool ExecuteScoutingMission(int32 UnitID, const FVector& TargetLocation, float Radius};

    // ?≤Â??µÂ??±Â?
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FMingScoutingReport> GetScoutingReports() const;

    // Ë®≠ÁΩÆ?µÂ??Ä??    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SetScoutingStatus(int32 UnitID, EMingScoutingStatus Status};

    // Ë®àÁ?Ë¶ñÈ?ÁØÑÂ?
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FVector> CalculateVisionArea(int32 UnitID) const;

    // ?≤Â?Ëø∑ÈúßÁ∂≤Ê†º
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FMingFogGridCell> GetFogGrid() const;

    // ‰øùÂ?Ëø∑Èúß?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SaveFogOfWarData(};

    // ËºâÂÖ•Ëø∑Èúß?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool LoadFogOfWarData(};

    // Ê∏ÖÈô§?Ä?âËø∑?ßÊï∏??    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    void ClearAllFogOfWarData(};

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable)
    FOnFogOfWarUpdated OnFogOfWarUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnVisionUpdated OnVisionUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnScoutingCompleted OnScoutingCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnUnitSpotted OnUnitSpotted;

    UPROPERTY(BlueprintAssignable)
    FOnUnitLostSight OnUnitLostSight;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // ?Æ‰?Ë¶ñÈ??∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingVisionData> UnitVisionDatabase;

    // Ëø∑ÈúßÁ∂≤Ê†º
    UPROPERTY()
    TArray<FMingFogGridCell> FogGrid;

    // ?∞ÂΩ¢?∏Ê?
    UPROPERTY()
    TMap<FIntPoint, EMingTerrainType> TerrainGrid;

    // Âª∫Á??©ÈÅÆ?ΩÊï∏??    UPROPERTY()
    TMap<FIntPoint, float> StructureOcclusionGrid;

    // ?µÂ??±Â??∏Ê?Â∫?    UPROPERTY()
    TArray<FMingScoutingReport> ScoutingReports;

    // ?µÂ?‰ªªÂ??äÂ?
    UPROPERTY()
    TArray<int32> ActiveScoutingUnits;

    // Á∂≤Ê†ºË®≠ÁΩÆ
    UPROPERTY()
    int32 GridSizeX;

    UPROPERTY()
    int32 GridSizeY;

    UPROPERTY()
    float CellSize;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ?ùÂ??ñËø∑?ßÁ∂≤??    void InitializeFogGrid(};

    // ?¥Êñ∞?Æ‰?Ë¶ñÈ?
    void UpdateUnitVisionInternal(int32 UnitID};

    // Ë®àÁ?Ë¶ñÈ??ßÁ?Á∂≤Ê†º
    TArray<FIntPoint> CalculateVisibleGridCells(int32 UnitID};

    // ?¥Êñ∞Ëø∑ÈúßÁ∂≤Ê†º
    void UpdateFogGridCell(int32 GridX, int32 GridY, EMingFogLevel NewFogLevel, bool bIsScouted, int32 ScoutingUnitID};

    // Ê™¢Êü•?∞ÂΩ¢?ÆËîΩ
    bool CheckTerrainOcclusion(const FVector& From, const FVector& To};

    // Ê™¢Êü•Âª∫Á??©ÈÅÆ??    bool CheckStructureOcclusion(const FVector& From, const FVector& To};

    // Ë®àÁ?Ë¶ñÈ?Á∑?    TArray<FVector> CalculateVisionLines(int32 UnitID};

    // ?ïÁ??µÂ?‰ªªÂ?
    void ProcessScoutingMission(int32 UnitID, float DeltaTime};

    // ?üÊ??µÂ??±Â?
    FMingScoutingReport GenerateScoutingReport(int32 UnitID, const FVector& Location, float Radius};

    // ?¥Êñ∞?∞ÂΩ¢‰ø°ÊÅØ
    void UpdateTerrainInfo(const FVector& Location};

    // ‰∏ñÁ?Â∫ßÊ?ËΩâÊ??∫Á∂≤?ºÂ∫ßÊ®?    FIntPoint WorldToGrid(const FVector& WorldPosition) const;

    // Á∂≤Ê†ºÂ∫ßÊ?ËΩâÊ??∫‰??åÂ∫ßÊ®?    FVector GridToWorld(const FIntPoint& GridPosition) const;

    // Ê™¢Êü•Á∂≤Ê†ºÂ∫ßÊ??ØÂê¶?âÊ?
    bool IsValidGridPosition(const FIntPoint& GridPosition) const;

    // ?≤Â?Á∂≤Ê†ºÁ¥¢Â?
    int32 GetGridIndex(int32 GridX, int32 GridY) const;

    // Ë®àÁ??©È??ìË???    float CalculateDistance(const FVector& Point1, const FVector& Point2) const;

    // Ê™¢Êü•Ë¶ñÈ??ßÁ??Æ‰?
    void CheckUnitsInVision(int32 UnitID};

    // ?ïÁ??Æ‰??ºÁèæ/?∫Â§±
    void ProcessUnitDiscovery(int32 ViewerID, int32 DiscoveredUnitID};
    void ProcessUnitLossOfSight(int32 ViewerID, int32 LostUnitID};

    // È©óË?Ë¶ñÈ??∏Ê?
    bool ValidateVisionData(const FMingVisionData& VisionData) const;

    // Ê∏ÖÈô§?éÊ??ÑÂÅµÂØü‰ø°??    void CleanExpiredScoutingInfo(};

    // ?¥Êñ∞Ëø∑ÈúßÊ∏≤Ê?
    void UpdateFogRendering(};
};

