#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLevelDesigner.generated.h"

UENUM(BlueprintType)
enum class EMingTerrainType : uint8
{
    Grass UMETA(DisplayName = "Grass"),
    Desert UMETA(DisplayName = "Desert"),
    Forest UMETA(DisplayName = "Forest"),
    Mountain UMETA(DisplayName = "Mountain"),
    Water UMETA(DisplayName = "Water"),
    Urban UMETA(DisplayName = "Urban"),
    Snow UMETA(DisplayName = "Snow"),
    Swamp UMETA(DisplayName = "Swamp")
};

UENUM(BlueprintType)
enum class EMingMapSize : uint8
{
    Small UMETA(DisplayName = "Small (2x2 km)"),
    Medium UMETA(DisplayName = "Medium (4x4 km)"),
    Large UMETA(DisplayName = "Large (8x8 km)"),
    Huge UMETA(DisplayName = "Huge (16x16 km)")
};

UENUM(BlueprintType)
enum class EMingWeatherType : uint8
{
    Clear UMETA(DisplayName = "Clear"),
    Cloudy UMETA(DisplayName = "Cloudy"),
    Rain UMETA(DisplayName = "Rain"),
    Storm UMETA(DisplayName = "Storm"),
    Snow UMETA(DisplayName = "Snow"),
    Fog UMETA(DisplayName = "Fog")
};

USTRUCT(BlueprintType)
struct FMingTerrainData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTerrainType TerrainType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Size;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Height;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TexturePath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeedModifier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisibilityModifier;

    FMingTerrainData()
        : TerrainType(EMingTerrainType::Grass)
        , Position(FVector2D::ZeroVector)
        , Size(100.0f)
        , Height(0.0f)
        , Color(FLinearColor::Green)
        , MovementSpeedModifier(1.0f)
        , VisibilityModifier(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingMapObject
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ObjectID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ObjectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Scale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPlayerStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsObjective;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ObjectName;

    FMingMapObject()
        : Location(FVector::ZeroVector)
        , Rotation(FRotator::ZeroRotator)
        , Scale(FVector::OneVector)
        , ActorClass(nullptr)
        , bIsPlayerStart(false)
        , bIsObjective(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingLevelData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingMapSize MapSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTerrainType DefaultTerrain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingWeatherType WeatherType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingTerrainData> TerrainData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingMapObject> MapObjects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D MapBounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeOfDay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHasTimeLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RequiredObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> OptionalObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AmbientSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;

    FMingLevelData()
        : MapSize(EMingMapSize::Medium)
        , DefaultTerrain(EMingTerrainType::Grass)
        , WeatherType(EMingWeatherType::Clear)
        , MapBounds(FVector2D(4000.0f, 4000.0f))
        , TimeOfDay(12.0f)
        , bHasTimeLimit(false)
        , TimeLimit(0.0f)
        , bIsCompleted(false)
    {}
};

/**
 * Level Designer System for MingGoRTS
 * Provides tools for creating and editing game levels
 */
UCLASS(ClassGroup = (Level Design), Blueprintable)
class MINGLEVELDESIGNER_API UMingLevelDesigner : public UObject
{
    GENERATED_BODY()

public:
    UMingLevelDesigner();

    // Level Management
    UFUNCTION(BlueprintCallable, Category = "Level")
    void InitializeLevelDesigner();

    UFUNCTION(BlueprintCallable, Category = "Level")
    void CreateNewLevel(const FString& LevelID, EMingMapSize MapSize);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void LoadLevel(const FString& LevelID);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void SaveLevel(const FString& LevelID);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void DeleteLevel(const FString& LevelID);

    UFUNCTION(BlueprintPure, Category = "Level")
    TArray<FMingLevelData> GetAllLevels() const;

    UFUNCTION(BlueprintPure, Category = "Level")
    FMingLevelData GetCurrentLevel() const;

    // Terrain Editing
    UFUNCTION(BlueprintCallable, Category = "Terrain")
    void AddTerrainPatch(const FMingTerrainData& TerrainData);

    UFUNCTION(BlueprintCallable, Category = "Terrain")
    void RemoveTerrainPatch(const FVector2D& Position);

    UFUNCTION(BlueprintCallable, Category = "Terrain")
    void ModifyTerrainHeight(const FVector2D& Position, float HeightDelta, float Radius);

    UFUNCTION(BlueprintCallable, Category = "Terrain")
    void SetTerrainType(const FVector2D& Position, EMingTerrainType TerrainType, float Radius);

    UFUNCTION(BlueprintCallable, Category = "Terrain")
    void SmoothTerrain(const FVector2D& Position, float Radius, float Strength);

    UFUNCTION(BlueprintPure, Category = "Terrain")
    TArray<FMingTerrainData> GetTerrainData() const;

    UFUNCTION(BlueprintPure, Category = "Terrain")
    EMingTerrainType GetTerrainAt(const FVector2D& Position) const;

    // Object Placement
    UFUNCTION(BlueprintCallable, Category = "Objects")
    void PlaceObject(const FMingMapObject& MapObject);

    UFUNCTION(BlueprintCallable, Category = "Objects")
    void RemoveObject(const FString& ObjectID);

    UFUNCTION(BlueprintCallable, Category = "Objects")
    void MoveObject(const FString& ObjectID, FVector NewLocation);

    UFUNCTION(BlueprintCallable, Category = "Objects")
    void RotateObject(const FString& ObjectID, FRotator NewRotation);

    UFUNCTION(BlueprintCallable, Category = "Objects")
    void ScaleObject(const FString& ObjectID, FVector NewScale);

    UFUNCTION(BlueprintPure, Category = "Objects")
    TArray<FMingMapObject> GetMapObjects() const;

    UFUNCTION(BlueprintPure, Category = "Objects")
    FMingMapObject GetObject(const FString& ObjectID) const;

    UFUNCTION(BlueprintPure, Category = "Objects")
    TArray<FMingMapObject> GetObjectsInArea(FVector Center, float Radius) const;

    // Level Properties
    UFUNCTION(BlueprintCallable, Category = "Properties")
    void SetMapSize(EMingMapSize NewSize);

    UFUNCTION(BlueprintCallable, Category = "Properties")
    void SetWeatherType(EMingWeatherType Weather);

    UFUNCTION(BlueprintCallable, Category = "Properties")
    void SetTimeOfDay(float Time);

    UFUNCTION(BlueprintCallable, Category = "Properties")
    void SetTimeLimit(float Limit);

    UFUNCTION(BlueprintCallable, Category = "Properties")
    void AddObjective(const FString& ObjectiveID, bool bIsRequired);

    UFUNCTION(BlueprintCallable, Category = "Properties")
    void RemoveObjective(const FString& ObjectiveID);

    UFUNCTION(BlueprintPure, Category = "Properties")
    FVector2D GetMapBounds() const;

    UFUNCTION(BlueprintPure, Category = "Properties")
    bool IsPositionInBounds(const FVector2D& Position) const;

    // Level Testing
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void TestLevel();

    UFUNCTION(BlueprintCallable, Category = "Testing")
    void StopTest();

    UFUNCTION(BlueprintCallable, Category = "Testing")
    void ValidateLevel();

    UFUNCTION(BlueprintPure, Category = "Testing")
    TArray<FString> GetValidationErrors() const;

    // Level Templates
    UFUNCTION(BlueprintCallable, Category = "Templates")
    void LoadTemplate(const FString& TemplateID);

    UFUNCTION(BlueprintCallable, Category = "Templates")
    void SaveAsTemplate(const FString& TemplateID);

    UFUNCTION(BlueprintPure, Category = "Templates")
    TArray<FString> GetAvailableTemplates() const;

    // Import/Export
    UFUNCTION(BlueprintCallable, Category = "Import/Export")
    void ExportLevel(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Import/Export")
    void ImportLevel(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Import/Export")
    void ExportToJSON(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Import/Export")
    void ImportFromJSON(const FString& FilePath);

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelCreated, const FString&, LevelID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelLoaded, const FString&, LevelID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelSaved, const FString&, LevelID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTerrainModified, const FVector2D&, Position);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectPlaced, const FString&, ObjectID, const FVector&, Location);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLevelCreated OnLevelCreated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLevelLoaded OnLevelLoaded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLevelSaved OnLevelSaved;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTerrainModified OnTerrainModified;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnObjectPlaced OnObjectPlaced;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetTerrainTypeName(EMingTerrainType TerrainType);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetMapSizeName(EMingMapSize MapSize);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetWeatherTypeName(EMingWeatherType WeatherType);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveLevelData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadLevelData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingLevelData> LevelDatabase;

    UPROPERTY()
    FString CurrentLevelID;

    UPROPERTY()
    TMap<FString, FMingLevelData> LevelTemplates;

    UPROPERTY()
    TArray<FString> ValidationErrors;

    UPROPERTY()
    bool bIsTesting;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeDefaultTemplates();
    void SetupDefaultLevels();
    void ValidateLevelIntegrity();

    // Terrain Helpers
    void GenerateTerrain(EMingMapSize MapSize, EMingTerrainType DefaultTerrain);
    void ApplyTerrainModifiers();
    void OptimizeTerrainData();

    // Object Helpers
    FString GenerateUniqueObjectID();
    bool IsValidObjectPlacement(const FMingMapObject& Object) const;
    void UpdateObjectReferences();

    // Validation Helpers
    bool ValidatePlayerStarts();
    bool ValidateObjectives();
    bool ValidateTerrainIntegrity();
    bool ValidateMapBounds();

    // Template Helpers
    void CreateDefaultTemplates();
    void ApplyTemplate(const FMingLevelData& Template);
};
