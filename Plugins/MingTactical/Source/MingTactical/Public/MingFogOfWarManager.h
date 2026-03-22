#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFogOfWarManager.generated.h"

// ��??��??
UENUM(BlueprintType)
enum class EMingVisionType: uint8 {
    Normal,            // 摧毀��??
    Enhanced,          // �W�j��??
    NightVision,        // �]??
    Thermal,           // ???X
    Satellite,         // ��??��??
    Aerial             // �Ť�摧毀
};

// �g����??
UENUM(BlueprintType)
enum class EMingFogLevel: uint8 {
    Clear,             // �M��
    Light,             // ����
    Medium,            // ����
    Heavy,             // �@��
    Complete           // �����g��
};

// ??����??
UENUM(BlueprintType)
enum class EMingTerrainType: uint8 {
    Open,              // ???X?��
    Forest,            // ��??
    Mountain,          // �s�a
    Urban,             // 摧毀
    Desert,            // �F??
    Water,             // ��??
    Swamp,             // �h�A
    Snow               // ??�a
};

// ???X?XUENUM(BlueprintType)
enum class EMingScoutingStatus: uint8 {
    Idle,              // ��??
    Scouting,          // 摧毀??
    Observing,         // �[�
    Reporting,          // 摧毀??
    Hidden,            // ??��
    Detected           // �Q�oX};

// ��?X???
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingVisionData
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // ��?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionRadius;

    // ��??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingVisionType VisionType;

    // ��??����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionAngle;

    // ��?X??X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionDuration;

    // ??�_摧毀��?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPersistent;

    // ��?X?�s摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionUpdateInterval;

    // 故事重要性?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> VisibleUnits;

    // �w����?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> ScoutedAreas;

    // ??���???X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
        VisibleUnits.Empty();
        ScoutedAreas.Empty();
        LastUpdateTime = FDateTime::Now();
    }
};

// �g�����摧毀
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFogGridCell
{
    GENERATED_BODY()

    // ���y??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint GridPosition;

    // �g����??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFogLevel FogLevel;

    // ??�_�Q����??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsScouted;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ScoutedTime;

    // 目標數量ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoutingUnitID;

    // ??����??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTerrainType TerrainType;

    // ??��??��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrainOcclusion;

    // ��?X?�B摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StructureOcclusion;

    // ??��i��?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastVisibleTime;

    FMingFogGridCell()
    {
        GridPosition = FIntPoint(0, 0);
        FogLevel = EMingFogLevel::Complete;
        bIsScouted = false;
        ScoutedTime = FDateTime::MinValue();
        ScoutingUnitID = -1;
        TerrainType = EMingTerrainType::Open;
        TerrainOcclusion = 0.0f;
        StructureOcclusion = 0.0f;
        LastVisibleTime = FDateTime::MinValue();
    }
};

// 目標數量
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingScoutingReport
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ReportID;

    // 目標數量ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoutingUnitID;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ScoutingTime;

    // 摧毀��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ScoutingLocation;

    // 摧毀�d??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ScoutingRadius;

    // ??�{??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> DiscoveredUnits;

    // ??�{??�ؿv��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> DiscoveredStructures;

    // ??�ΫH��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> TerrainInfo;

    // ��??����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreatAssessment;

    // 摧毀�Բ�??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReportDetail;

    // 故事重要性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReportReliability;

    FMingScoutingReport()
    {
        ReportID = -1;
        ScoutingUnitID = -1;
        ScoutingTime = FDateTime::Now();
        ScoutingLocation = FVector::ZeroVector;
        ScoutingRadius = 0.0f;
        DiscoveredUnits.Empty();
        DiscoveredStructures.Empty();
        TerrainInfo.Empty();
        ThreatAssessment = TEXT(""};
        ReportDetail = 0.0f;
        ReportReliability = 1.0f;
    }
);

// ??���g���ƥ�e??






/**
 * ??���g����?X * �t�d��?X?���g��故事選項?�t��
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingFogOfWarManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFogOfWarManager();

    // ???X?��??�g??�t??
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool InitializeFogOfWarSystem();

    // ��?X???��??
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool RegisterUnitVision(int32 UnitID, const FMingVisionData& VisionData);

    // ??�s摧毀��??
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool UpdateUnitVision(int32 UnitID, const FVector& NewPosition);

    // 目標數量��??
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    FMingVisionData GetUnitVision(int32 UnitID) const;

    // 故事選項?
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<int32> GetVisibleUnits(int32 UnitID) const;

    // �ˬd故事重要性
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    bool IsUnitVisible(int32 ViewerID, int32 TargetID) const;

    // ??�s??���g��
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    void UpdateFogOfWar();

    // 摧毀�g����??
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    EMingFogLevel GetFogLevel(const FVector& Location) const;

    // �]�m??����??
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SetTerrainType(const FVector& Location, EMingTerrainType TerrainType);

    // �K??��?X?�BX
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool AddStructureOcclusion(const FVector& Location, float OcclusionValue);

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool ExecuteScoutingMission(int32 UnitID, const FVector& TargetLocation, float Radius);

    // 故事選項?
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FMingScoutingReport> GetScoutingReports() const;

    // �]�m???X?X
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SetScoutingStatus(int32 UnitID, EMingScoutingStatus Status);

    // �p??��??�d??
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FVector> CalculateVisionArea(int32 UnitID) const;

    // 摧毀�g�����
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FMingFogGridCell> GetFogGrid() const;

    // �O??�g��摧毀
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SaveFogOfWarData();

    // ���J�g��摧毀
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool LoadFogOfWarData();

    // �M��摧毀�g??��X
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    void ClearAllFogOfWarData();

    // �ƥ�e??
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

    // 摧毀��?X摧毀?
    UPROPERTY()
    TMap<int32, FMingVisionData> UnitVisionDatabase;

    // �g�����
    UPROPERTY()
    TArray<FMingFogGridCell> FogGrid;

    // ??��摧毀
    UPROPERTY()
    TMap<FIntPoint, EMingTerrainType> TerrainGrid;

    // ��?X?�B??��X
    UPROPERTY()
    TMap<FIntPoint, float> StructureOcclusionGrid;

    // 故事重要性摧毀?
    UPROPERTY()
    TArray<FMingScoutingReport> ScoutingReports;

    // 摧毀��?X???
    UPROPERTY()
    TArray<int32> ActiveScoutingUnits;

    // ���]�m
    UPROPERTY()
    int32 GridSizeX;

    UPROPERTY()
    int32 GridSizeY;

    UPROPERTY()
    float CellSize;

    // ??�_�w??�l??
    UPROPERTY()
    bool bInitialized;

private:
    // ???X?�g??��X
    void InitializeFogGrid();

    // ??�s摧毀��??
    void UpdateUnitVisionInternal(int32 UnitID);

    // �p??��?X???���
    TArray<FIntPoint> CalculateVisibleGridCells(int32 UnitID);

    // ??�s�g�����
    void UpdateFogGridCell(int32 GridX, int32 GridY, EMingFogLevel NewFogLevel, bool bIsScouted, int32 ScoutingUnitID);

    // �ˬd??��??��
    bool CheckTerrainOcclusion(const FVector& From, const FVector& To);

    // �ˬd��?X?�BX
    bool CheckStructureOcclusion(const FVector& From, const FVector& To);

    // �p??��摧毀
    TArray<FVector> CalculateVisionLines(int32 UnitID);

    // 目標數量��??
    void ProcessScoutingMission(int32 UnitID, float DeltaTime);

    // 故事選項?
    FMingScoutingReport GenerateScoutingReport(int32 UnitID, const FVector& Location, float Radius);

    // ??�s??�ΫH��
    void UpdateTerrainInfo(const FVector& Location);

    // �@??�y??��?X?��??�y??
    FIntPoint WorldToGrid(const FVector& WorldPosition) const;

    // ���y??��?X??X?�y??
    FVector GridToWorld(const FIntPoint& GridPosition) const;

    // �ˬd���y?X?�_摧毀
    bool IsValidGridPosition(const FIntPoint& GridPosition) const;

    // 摧毀����??
    int32 GetGridIndex(int32 GridX, int32 GridY) const;

    // �p?X??X??X
    float CalculateDistance(const FVector& Point1, const FVector& Point2) const;

    // �ˬd��?X??X???
    void CheckUnitsInVision(int32 UnitID);

    // 故事重要性?�{/??��
    void ProcessUnitDiscovery(int32 ViewerID, int32 DiscoveredUnitID);
    void ProcessUnitLossOfSight(int32 ViewerID, int32 LostUnitID);

    // ��??��?X???
    bool ValidateVisionData(const FMingVisionData& VisionData) const;

    // �M��???X?����HX
    void CleanExpiredScoutingInfo();

    // ??�s�g����??
    void UpdateFogRendering();
};

