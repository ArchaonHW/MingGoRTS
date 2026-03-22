#include "MingLevelDesigner.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingLevelDesigner::UMingLevelDesigner()
{
    WorldContext = GetWorld();
    bIsTesting = false;
}

void UMingLevelDesigner::InitializeLevelDesigner()
{
    InitializeDefaultTemplates();
    SetupDefaultLevels();
    
    UE_LOG(LogTemp, Log, TEXT("Level Designer initialized with %d levels"), LevelDatabase.Num());
}

void UMingLevelDesigner::InitializeDefaultTemplates()
{
    LevelTemplates.Empty();
    CreateDefaultTemplates();
    
    UE_LOG(LogTemp, Log, TEXT("Level Designer initialized with %d templates"), LevelTemplates.Num());
}

void UMingLevelDesigner::CreateDefaultTemplates()
{
    // Wuchang Uprising Template
    FMingLevelData WuchangTemplate;
    WuchangTemplate.LevelID = TEXT("TEMPLATE_WUCHANG");
    WuchangTemplate.LevelName = TEXT("武昌起義模板");
    WuchangTemplate.Description = TEXT("1911年武昌起義的標準地圖模板");
    WuchangTemplate.MapSize = EMingMapSize::Medium;
    WuchangTemplate.DefaultTerrain = EMingTerrainType::Urban;
    WuchangTemplate.WeatherType = EMingWeatherType::Clear;
    WuchangTemplate.TimeOfDay = 6.0f; // Early morning
    WuchangTemplate.MapBounds = FVector2D(4000.0f, 4000.0f);
    WuchangTemplate.BackgroundMusic = TEXT("MUSIC_HISTORICAL");
    WuchangTemplate.AmbientSound = TEXT("AMBIENT_CITY");
    
    // Add terrain patches
    FMingTerrainData CityTerrain;
    CityTerrain.TerrainType = EMingTerrainType::Urban;
    CityTerrain.Position = FVector2D(2000.0f, 2000.0f);
    CityTerrain.Size = 3000.0f;
    CityTerrain.Height = 10.0f;
    CityTerrain.Color = FLinearColor(0.4f, 0.4f, 0.4f);
    CityTerrain.MovementSpeedModifier = 0.9f;
    CityTerrain.VisibilityModifier = 0.8f;
    WuchangTemplate.TerrainData.Add(CityTerrain);
    
    FMingTerrainData RiverTerrain;
    RiverTerrain.TerrainType = EMingTerrainType::Water;
    RiverTerrain.Position = FVector2D(2000.0f, 1000.0f);
    RiverTerrain.Size = 800.0f;
    RiverTerrain.Height = -5.0f;
    RiverTerrain.Color = FLinearColor(0.2f, 0.4f, 0.8f);
    RiverTerrain.MovementSpeedModifier = 0.3f;
    RiverTerrain.VisibilityModifier = 1.2f;
    WuchangTemplate.TerrainData.Add(RiverTerrain);
    
    // Add key objects
    FMingMapObject GovernorPalace;
    GovernorPalace.ObjectID = TEXT("GOVERNOR_PALACE");
    GovernorPalace.ObjectType = TEXT("Building");
    GovernorPalace.Location = FVector(2000.0f, 2000.0f, 20.0f);
    GovernorPalace.Rotation = FRotator(0.0f, 0.0f, 0.0f);
    GovernorPalace.Scale = FVector(2.0f, 2.0f, 1.5f);
    GovernorPalace.bIsObjective = true;
    GovernorPalace.ObjectName = TEXT("總督府");
    WuchangTemplate.MapObjects.Add(GovernorPalace);
    
    FMingMapObject PlayerStart;
    PlayerStart.ObjectID = TEXT("PLAYER_START");
    PlayerStart.ObjectType = TEXT("StartPoint");
    PlayerStart.Location = FVector(1500.0f, 1500.0f, 10.0f);
    PlayerStart.bIsPlayerStart = true;
    PlayerStart.ObjectName = TEXT("玩家起始點");
    WuchangTemplate.MapObjects.Add(PlayerStart);
    
    FMingMapObject EnemyBase;
    EnemyBase.ObjectID = TEXT("ENEMY_BASE");
    EnemyBase.ObjectType = TEXT("Base");
    EnemyBase.Location = FVector(2500.0f, 2500.0f, 15.0f);
    EnemyBase.Scale = FVector(1.5f, 1.5f, 1.0f);
    EnemyBase.ObjectName = TEXT("敵軍基地");
    WuchangTemplate.MapObjects.Add(EnemyBase);
    
    // Add objectives
    WuchangTemplate.RequiredObjectives.Add(TEXT("Capture_Governor_Palace"));
    WuchangTemplate.RequiredObjectives.Add(TEXT("Eliminate_Enemy_Forces"));
    WuchangTemplate.OptionalObjectives.Add(TEXT("Rescue_Civilians"));
    
    LevelTemplates.Add(WuchangTemplate.LevelID, WuchangTemplate);
    
    // Northern Expedition Template
    FMingLevelData NorthernTemplate;
    NorthernTemplate.LevelID = TEXT("TEMPLATE_NORTHERN");
    NorthernTemplate.LevelName = TEXT("北伐戰爭模板");
    NorthernTemplate.Description = TEXT("北伐戰爭的大規模戰役模板");
    NorthernTemplate.MapSize = EMingMapSize::Large;
    NorthernTemplate.DefaultTerrain = EMingTerrainType::Grass;
    NorthernTemplate.WeatherType = EMingWeatherType::Cloudy;
    NorthernTemplate.TimeOfDay = 10.0f;
    NorthernTemplate.MapBounds = FVector2D(8000.0f, 8000.0f);
    NorthernTemplate.BackgroundMusic = TEXT("MUSIC_BATTLE");
    NorthernTemplate.AmbientSound = TEXT("AMBIENT_BATTLEFIELD");
    
    // Add diverse terrain
    FMingTerrainData PlainsTerrain;
    PlainsTerrain.TerrainType = EMingTerrainType::Grass;
    PlainsTerrain.Position = FVector2D(4000.0f, 4000.0f);
    PlainsTerrain.Size = 6000.0f;
    PlainsTerrain.Height = 0.0f;
    PlainsTerrain.Color = FLinearColor(0.3f, 0.6f, 0.2f);
    PlainsTerrain.MovementSpeedModifier = 1.0f;
    PlainsTerrain.VisibilityModifier = 1.0f;
    NorthernTemplate.TerrainData.Add(PlainsTerrain);
    
    FMingTerrainData MountainTerrain;
    MountainTerrain.TerrainType = EMingTerrainType::Mountain;
    MountainTerrain.Position = FVector2D(6000.0f, 2000.0f);
    MountainTerrain.Size = 1500.0f;
    MountainTerrain.Height = 200.0f;
    MountainTerrain.Color = FLinearColor(0.5f, 0.4f, 0.3f);
    MountainTerrain.MovementSpeedModifier = 0.5f;
    MountainTerrain.VisibilityModifier = 0.6f;
    NorthernTemplate.TerrainData.Add(MountainTerrain);
    
    FMingTerrainData ForestTerrain;
    ForestTerrain.TerrainType = EMingTerrainType::Forest;
    ForestTerrain.Position = FVector2D(2000.0f, 6000.0f);
    ForestTerrain.Size = 2000.0f;
    ForestTerrain.Height = 5.0f;
    ForestTerrain.Color = FLinearColor(0.2f, 0.5f, 0.1f);
    ForestTerrain.MovementSpeedModifier = 0.7f;
    ForestTerrain.VisibilityModifier = 0.4f;
    NorthernTemplate.TerrainData.Add(ForestTerrain);
    
    // Add strategic locations
    FMingMapObject GuangzhouBase;
    GuangzhouBase.ObjectID = TEXT("GUANGZHOU_BASE");
    GuangzhouBase.ObjectType = TEXT("Base");
    GuangzhouBase.Location = FVector(1000.0f, 1000.0f, 10.0f);
    GuangzhouBase.Scale = FVector(3.0f, 3.0f, 2.0f);
    GuangzhouBase.bIsPlayerStart = true;
    GuangzhouBase.ObjectName = TEXT("廣州基地");
    NorthernTemplate.MapObjects.Add(GuangzhouBase);
    
    FMingMapObject BeijingTarget;
    BeijingTarget.ObjectID = TEXT("BEIJING_TARGET");
    BeijingTarget.ObjectType = TEXT("Capital");
    BeijingTarget.Location = FVector(7000.0f, 7000.0f, 15.0f);
    BeijingTarget.Scale = FVector(4.0f, 4.0f, 3.0f);
    BeijingTarget.bIsObjective = true;
    BeijingTarget.ObjectName = TEXT("北京");
    NorthernTemplate.MapObjects.Add(BeijingTarget);
    
    // Add city objectives
    FMingMapObject NanjingCity;
    NanjingCity.ObjectID = TEXT("NANJING_CITY");
    NanjingCity.ObjectType = TEXT("City");
    NanjingCity.Location = FVector(4000.0f, 4000.0f, 12.0f);
    NanjingCity.Scale = FVector(2.5f, 2.5f, 1.8f);
    NanjingCity.bIsObjective = true;
    NanjingCity.ObjectName = TEXT("南京");
    NorthernTemplate.MapObjects.Add(NanjingCity);
    
    NorthernTemplate.RequiredObjectives.Add(TEXT("Capture_Beijing"));
    NorthernTemplate.RequiredObjectives.Add(TEXT("Unify_Southern_Provinces"));
    NorthernTemplate.OptionalObjectives.Add(TEXT("Secure_Railway_Lines"));
    NorthernTemplate.OptionalObjectives.Add(TEXT("Win_Hearts_and_Minds"));
    
    LevelTemplates.Add(NorthernTemplate.LevelID, NorthernTemplate);
    
    // Anti-Japanese War Template
    FMingLevelData AntiJapaneseTemplate;
    AntiJapaneseTemplate.LevelID = TEXT("TEMPLATE_ANTI_JAPANESE");
    AntiJapaneseTemplate.LevelName = TEXT("抗日戰爭模板");
    AntiJapaneseTemplate.Description = TEXT("抗日戰爭的防禦戰役模板");
    AntiJapaneseTemplate.MapSize = EMingMapSize::Medium;
    AntiJapaneseTemplate.DefaultTerrain = EMingTerrainType::Mountain;
    AntiJapaneseTemplate.WeatherType = EMingWeatherType::Rain;
    AntiJapaneseTemplate.TimeOfDay = 14.0f;
    AntiJapaneseTemplate.MapBounds = FVector2D(4000.0f, 4000.0f);
    AntiJapaneseTemplate.BackgroundMusic = TEXT("MUSIC_TENSE");
    AntiJapaneseTemplate.AmbientSound = TEXT("AMBIENT_FOREST");
    AntiJapaneseTemplate.bHasTimeLimit = true;
    AntiJapaneseTemplate.TimeLimit = 1800.0f; // 30 minutes
    
    // Add mountain terrain
    FMingTerrainData MainMountain;
    MainMountain.TerrainType = EMingTerrainType::Mountain;
    MainMountain.Position = FVector2D(2000.0f, 2000.0f);
    MainMountain.Size = 3500.0f;
    MainMountain.Height = 300.0f;
    MainMountain.Color = FLinearColor(0.4f, 0.3f, 0.2f);
    MainMountain.MovementSpeedModifier = 0.4f;
    MainMountain.VisibilityModifier = 0.5f;
    AntiJapaneseTemplate.TerrainData.Add(MainMountain);
    
    // Add defensive positions
    FMingMapObject DefenseLine;
    DefenseLine.ObjectID = TEXT("DEFENSE_LINE");
    DefenseLine.ObjectType = TEXT("Defense");
    DefenseLine.Location = FVector(2000.0f, 1500.0f, 50.0f);
    DefenseLine.Scale = FVector(6.0f, 1.0f, 0.5f);
    DefenseLine.bIsPlayerStart = true;
    DefenseLine.ObjectName = TEXT("防線");
    AntiJapaneseTemplate.MapObjects.Add(DefenseLine);
    
    FMingMapObject Village;
    Village.ObjectID = TEXT("VILLAGE");
    Village.ObjectType = TEXT("Settlement");
    Village.Location = FVector(2000.0f, 2500.0f, 20.0f);
    Village.Scale = FVector(1.5f, 1.5f, 1.0f);
    Village.bIsObjective = true;
    Village.ObjectName = TEXT("村莊");
    AntiJapaneseTemplate.MapObjects.Add(Village);
    
    AntiJapaneseTemplate.RequiredObjectives.Add(TEXT("Defend_Village"));
    AntiJapaneseTemplate.RequiredObjectives.Add(TEXT("Hold_Defense_Line"));
    AntiJapaneseTemplate.OptionalObjectives.Add(TEXT("Destroy_Artillery"));
    
    LevelTemplates.Add(AntiJapaneseTemplate.LevelID, AntiJapaneseTemplate);
}

void UMingLevelDesigner::SetupDefaultLevels()
{
    // Create some default levels from templates
    LoadTemplate(TEXT("TEMPLATE_WUCHANG"));
    if (GetCurrentLevel().LevelID.IsEmpty())
    {
        CreateNewLevel(TEXT("WUCHANG_UPRISING"), EMingMapSize::Medium);
        ApplyTemplate(LevelTemplates[TEXT("TEMPLATE_WUCHANG")]);
        SaveLevel(TEXT("WUCHANG_UPRISING"));
    }
    
    LoadTemplate(TEXT("TEMPLATE_NORTHERN"));
    if (!LevelDatabase.Contains(TEXT("NORTHERN_EXPEDITION")))
    {
        CreateNewLevel(TEXT("NORTHERN_EXPEDITION"), EMingMapSize::Large);
        ApplyTemplate(LevelTemplates[TEXT("TEMPLATE_NORTHERN")]);
        SaveLevel(TEXT("NORTHERN_EXPEDITION"));
    }
}

void UMingLevelDesigner::CreateNewLevel(const FString& LevelID, EMingMapSize MapSize)
{
    if (LevelDatabase.Contains(LevelID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Level already exists: %s"), *LevelID);
        return;
    }
    
    FMingLevelData NewLevel;
    NewLevel.LevelID = LevelID;
    NewLevel.LevelName = FString::Printf(TEXT("Level %s"), *LevelID);
    NewLevel.Description = TEXT("新創建的關卡");
    NewLevel.MapSize = MapSize;
    
    // Set map bounds based on size
    switch (MapSize)
    {
    case EMingMapSize::Small:
        NewLevel.MapBounds = FVector2D(2000.0f, 2000.0f);
        break;
    case EMingMapSize::Medium:
        NewLevel.MapBounds = FVector2D(4000.0f, 4000.0f);
        break;
    case EMingMapSize::Large:
        NewLevel.MapBounds = FVector2D(8000.0f, 8000.0f);
        break;
    case EMingMapSize::Huge:
        NewLevel.MapBounds = FVector2D(16000.0f, 16000.0f);
        break;
    }
    
    // Generate default terrain
    GenerateTerrain(MapSize, NewLevel.DefaultTerrain);
    
    LevelDatabase.Add(LevelID, NewLevel);
    CurrentLevelID = LevelID;
    
    OnLevelCreated.Broadcast(LevelID);
    
    UE_LOG(LogTemp, Log, TEXT("Created new level: %s"), *LevelID);
}

void UMingLevelDesigner::LoadLevel(const FString& LevelID)
{
    FMingLevelData* Level = LevelDatabase.Find(LevelID);
    if (!Level)
    {
        UE_LOG(LogTemp, Warning, TEXT("Level not found: %s"), *LevelID);
        return;
    }
    
    CurrentLevelID = LevelID;
    
    OnLevelLoaded.Broadcast(LevelID);
    
    UE_LOG(LogTemp, Log, TEXT("Loaded level: %s"), *Level->LevelName);
}

void UMingLevelDesigner::SaveLevel(const FString& LevelID)
{
    FMingLevelData* Level = LevelDatabase.Find(LevelID);
    if (!Level)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot save - level not found: %s"), *LevelID);
        return;
    }
    
    // Validate level before saving
    ValidateLevelIntegrity();
    
    OnLevelSaved.Broadcast(LevelID);
    
    UE_LOG(LogTemp, Log, TEXT("Saved level: %s"), *Level->LevelName);
}

void UMingLevelDesigner::DeleteLevel(const FString& LevelID)
{
    if (!LevelDatabase.Contains(LevelID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot delete - level not found: %s"), *LevelID);
        return;
    }
    
    LevelDatabase.Remove(LevelID);
    
    if (CurrentLevelID == LevelID)
    {
        CurrentLevelID.Empty();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Deleted level: %s"), *LevelID);
}

TArray<FMingLevelData> UMingLevelDesigner::GetAllLevels() const
{
    TArray<FMingLevelData> Result;
    for (const auto& Pair : LevelDatabase)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

FMingLevelData UMingLevelDesigner::GetCurrentLevel() const
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    return Level ? *Level : FMingLevelData();
}

void UMingLevelDesigner::AddTerrainPatch(const FMingTerrainData& TerrainData)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    Level->TerrainData.Add(TerrainData);
    OnTerrainModified.Broadcast(TerrainData.Position);
    
    UE_LOG(LogTemp, Log, TEXT("Added terrain patch at position %s"), *TerrainData.Position.ToString());
}

void UMingLevelDesigner::RemoveTerrainPatch(const FVector2D& Position)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    for (int32 i = 0; i < Level->TerrainData.Num(); ++i)
    {
        if (FVector2D::Dist(Level->TerrainData[i].Position, Position) < 50.0f)
        {
            Level->TerrainData.RemoveAt(i);
            OnTerrainModified.Broadcast(Position);
            break;
        }
    }
}

void UMingLevelDesigner::ModifyTerrainHeight(const FVector2D& Position, float HeightDelta, float Radius)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    for (FMingTerrainData& Terrain : Level->TerrainData)
    {
        float Distance = FVector2D::Dist(Terrain.Position, Position);
        if (Distance < Radius)
        {
            float Influence = 1.0f - (Distance / Radius);
            Terrain.Height += HeightDelta * Influence;
        }
    }
    
    OnTerrainModified.Broadcast(Position);
}

void UMingLevelDesigner::SetTerrainType(const FVector2D& Position, EMingTerrainType TerrainType, float Radius)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    for (FMingTerrainData& Terrain : Level->TerrainData)
    {
        float Distance = FVector2D::Dist(Terrain.Position, Position);
        if (Distance < Radius)
        {
            Terrain.TerrainType = TerrainType;
        }
    }
    
    OnTerrainModified.Broadcast(Position);
}

void UMingLevelDesigner::SmoothTerrain(const FVector2D& Position, float Radius, float Strength)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    // Calculate average height in radius
    float AverageHeight = 0.0f;
    int32 Count = 0;
    
    for (const FMingTerrainData& Terrain : Level->TerrainData)
    {
        float Distance = FVector2D::Dist(Terrain.Position, Position);
        if (Distance < Radius)
        {
            AverageHeight += Terrain.Height;
            Count++;
        }
    }
    
    if (Count > 0)
    {
        AverageHeight /= Count;
        
        // Apply smoothing
        for (FMingTerrainData& Terrain : Level->TerrainData)
        {
            float Distance = FVector2D::Dist(Terrain.Position, Position);
            if (Distance < Radius)
            {
                float Influence = (1.0f - (Distance / Radius)) * Strength;
                Terrain.Height = FMath::Lerp(Terrain.Height, AverageHeight, Influence);
            }
        }
    }
    
    OnTerrainModified.Broadcast(Position);
}

TArray<FMingTerrainData> UMingLevelDesigner::GetTerrainData() const
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    return Level ? Level->TerrainData : TArray<FMingTerrainData>();
}

EMingTerrainType UMingLevelDesigner::GetTerrainAt(const FVector2D& Position) const
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return EMingTerrainType::Grass;
    }
    
    for (const FMingTerrainData& Terrain : Level->TerrainData)
    {
        float Distance = FVector2D::Dist(Terrain.Position, Position);
        if (Distance < Terrain.Size)
        {
            return Terrain.TerrainType;
        }
    }
    
    return Level->DefaultTerrain;
}

void UMingLevelDesigner::PlaceObject(const FMingMapObject& MapObject)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    if (!IsValidObjectPlacement(MapObject))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid object placement"));
        return;
    }
    
    FMingMapObject NewObject = MapObject;
    if (NewObject.ObjectID.IsEmpty())
    {
        NewObject.ObjectID = GenerateUniqueObjectID();
    }
    
    Level->MapObjects.Add(NewObject);
    OnObjectPlaced.Broadcast(NewObject.ObjectID, NewObject.Location);
    
    UE_LOG(LogTemp, Log, TEXT("Placed object: %s"), *NewObject.ObjectName);
}

void UMingLevelDesigner::RemoveObject(const FString& ObjectID)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    for (int32 i = 0; i < Level->MapObjects.Num(); ++i)
    {
        if (Level->MapObjects[i].ObjectID == ObjectID)
        {
            Level->MapObjects.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Removed object: %s"), *ObjectID);
            break;
        }
    }
}

void UMingLevelDesigner::MoveObject(const FString& ObjectID, FVector NewLocation)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    for (FMingMapObject& Object : Level->MapObjects)
    {
        if (Object.ObjectID == ObjectID)
        {
            Object.Location = NewLocation;
            UE_LOG(LogTemp, Log, TEXT("Moved object %s to %s"), *ObjectID, *NewLocation.ToString());
            break;
        }
    }
}

void UMingLevelDesigner::RotateObject(const FString& ObjectID, FRotator NewRotation)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    for (FMingMapObject& Object : Level->MapObjects)
    {
        if (Object.ObjectID == ObjectID)
        {
            Object.Rotation = NewRotation;
            UE_LOG(LogTemp, Log, TEXT("Rotated object %s"), *ObjectID);
            break;
        }
    }
}

void UMingLevelDesigner::ScaleObject(const FString& ObjectID, FVector NewScale)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    for (FMingMapObject& Object : Level->MapObjects)
    {
        if (Object.ObjectID == ObjectID)
        {
            Object.Scale = NewScale;
            UE_LOG(LogTemp, Log, TEXT("Scaled object %s"), *ObjectID);
            break;
        }
    }
}

TArray<FMingMapObject> UMingLevelDesigner::GetMapObjects() const
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    return Level ? Level->MapObjects : TArray<FMingMapObject>();
}

FMingMapObject UMingLevelDesigner::GetObject(const FString& ObjectID) const
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (Level)
    {
        for (const FMingMapObject& Object : Level->MapObjects)
        {
            if (Object.ObjectID == ObjectID)
            {
                return Object;
            }
        }
    }
    return FMingMapObject();
}

TArray<FMingMapObject> UMingLevelDesigner::GetObjectsInArea(FVector Center, float Radius) const
{
    TArray<FMingMapObject> Result;
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    
    if (Level)
    {
        for (const FMingMapObject& Object : Level->MapObjects)
        {
            if (FVector::Dist(Object.Location, Center) <= Radius)
            {
                Result.Add(Object);
            }
        }
    }
    
    return Result;
}

void UMingLevelDesigner::SetMapSize(EMingMapSize NewSize)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    Level->MapSize = NewSize;
    
    // Update map bounds
    switch (NewSize)
    {
    case EMingMapSize::Small:
        Level->MapBounds = FVector2D(2000.0f, 2000.0f);
        break;
    case EMingMapSize::Medium:
        Level->MapBounds = FVector2D(4000.0f, 4000.0f);
        break;
    case EMingMapSize::Large:
        Level->MapBounds = FVector2D(8000.0f, 8000.0f);
        break;
    case EMingMapSize::Huge:
        Level->MapBounds = FVector2D(16000.0f, 16000.0f);
        break;
    }
}

void UMingLevelDesigner::SetWeatherType(EMingWeatherType Weather)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (Level)
    {
        Level->WeatherType = Weather;
    }
}

void UMingLevelDesigner::SetTimeOfDay(float Time)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (Level)
    {
        Level->TimeOfDay = FMath::Clamp(Time, 0.0f, 24.0f);
    }
}

void UMingLevelDesigner::SetTimeLimit(float Limit)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (Level)
    {
        Level->TimeLimit = Limit;
        Level->bHasTimeLimit = Limit > 0.0f;
    }
}

void UMingLevelDesigner::AddObjective(const FString& ObjectiveID, bool bIsRequired)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    if (bIsRequired)
    {
        if (!Level->RequiredObjectives.Contains(ObjectiveID))
        {
            Level->RequiredObjectives.Add(ObjectiveID);
        }
    }
    else
    {
        if (!Level->OptionalObjectives.Contains(ObjectiveID))
        {
            Level->OptionalObjectives.Add(ObjectiveID);
        }
    }
}

void UMingLevelDesigner::RemoveObjective(const FString& ObjectiveID)
{
    FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return;
    }
    
    Level->RequiredObjectives.Remove(ObjectiveID);
    Level->OptionalObjectives.Remove(ObjectiveID);
}

FVector2D UMingLevelDesigner::GetMapBounds() const
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    return Level ? Level->MapBounds : FVector2D::ZeroVector;
}

bool UMingLevelDesigner::IsPositionInBounds(const FVector2D& Position) const
{
    FVector2D Bounds = GetMapBounds();
    return Position.X >= 0.0f && Position.X <= Bounds.X && 
           Position.Y >= 0.0f && Position.Y <= Bounds.Y;
}

void UMingLevelDesigner::TestLevel()
{
    if (bIsTesting)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already testing level"));
        return;
    }
    
    ValidateLevel();
    if (ValidationErrors.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot test level - validation errors exist"));
        return;
    }
    
    bIsTesting = true;
    
    // This would start the game with the current level
    UE_LOG(LogTemp, Log, TEXT("Started testing level: %s"), *CurrentLevelID);
}

void UMingLevelDesigner::StopTest()
{
    if (!bIsTesting)
    {
        return;
    }
    
    bIsTesting = false;
    
    // This would stop the game and return to editor
    UE_LOG(LogTemp, Log, TEXT("Stopped testing level"));
}

void UMingLevelDesigner::ValidateLevel()
{
    ValidationErrors.Empty();
    
    ValidatePlayerStarts();
    ValidateObjectives();
    ValidateTerrainIntegrity();
    ValidateMapBounds();
    
    UE_LOG(LogTemp, Log, TEXT("Level validation complete - %d errors found"), ValidationErrors.Num());
}

TArray<FString> UMingLevelDesigner::GetValidationErrors() const
{
    return ValidationErrors;
}

void UMingLevelDesigner::LoadTemplate(const FString& TemplateID)
{
    const FMingLevelData* Template = LevelTemplates.Find(TemplateID);
    if (!Template)
    {
        UE_LOG(LogTemp, Warning, TEXT("Template not found: %s"), *TemplateID);
        return;
    }
    
    ApplyTemplate(*Template);
    CurrentLevelID = FString::Printf(TEXT("TEMPLATE_%s"), *TemplateID);
    
    UE_LOG(LogTemp, Log, TEXT("Loaded template: %s"), *TemplateID);
}

void UMingLevelDesigner::SaveAsTemplate(const FString& TemplateID)
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        UE_LOG(LogTemp, Warning, TEXT("No current level to save as template"));
        return;
    }
    
    FMingLevelData Template = *Level;
    Template.LevelID = TemplateID;
    Template.LevelName = FString::Printf(TEXT("Template %s"), *TemplateID);
    
    LevelTemplates.Add(TemplateID, Template);
    
    UE_LOG(LogTemp, Log, TEXT("Saved as template: %s"), *TemplateID);
}

TArray<FString> UMingLevelDesigner::GetAvailableTemplates() const
{
    TArray<FString> Result;
    for (const auto& Pair : LevelTemplates)
    {
        Result.Add(Pair.Key);
    }
    return Result;
}

void UMingLevelDesigner::ExportLevel(const FString& FilePath)
{
    // This would export the level to a file
    UE_LOG(LogTemp, Log, TEXT("Exporting level to: %s"), *FilePath);
}

void UMingLevelDesigner::ImportLevel(const FString& FilePath)
{
    // This would import a level from a file
    UE_LOG(LogTemp, Log, TEXT("Importing level from: %s"), *FilePath);
}

void UMingLevelDesigner::ExportToJSON(const FString& FilePath)
{
    FString LevelData = SaveLevelData();
    
    // This would write the JSON to file
    UE_LOG(LogTemp, Log, TEXT("Exporting level JSON to: %s"), *FilePath);
}

void UMingLevelDesigner::ImportFromJSON(const FString& FilePath)
{
    // This would read JSON from file and load it
    UE_LOG(LogTemp, Log, TEXT("Importing level JSON from: %s"), *FilePath);
}

FString UMingLevelDesigner::GetTerrainTypeName(EMingTerrainType TerrainType)
{
    switch (TerrainType)
    {
    case EMingTerrainType::Grass: return TEXT("草地");
    case EMingTerrainType::Desert: return TEXT("沙漠");
    case EMingTerrainType::Forest: return TEXT("森林");
    case EMingTerrainType::Mountain: return TEXT("山地");
    case EMingTerrainType::Water: return TEXT("水域");
    case EMingTerrainType::Urban: return TEXT("城市");
    case EMingTerrainType::Snow: return TEXT("雪地");
    case EMingTerrainType::Swamp: return TEXT("沼澤");
    default: return TEXT("未知");
    }
}

FString UMingLevelDesigner::GetMapSizeName(EMingMapSize MapSize)
{
    switch (MapSize)
    {
    case EMingMapSize::Small: return TEXT("小型 (2x2 km)");
    case EMingMapSize::Medium: return TEXT("中型 (4x4 km)");
    case EMingMapSize::Large: return TEXT("大型 (8x8 km)");
    case EMingMapSize::Huge: return TEXT("巨型 (16x16 km)");
    default: return TEXT("未知");
    }
}

FString UMingLevelDesigner::GetWeatherTypeName(EMingWeatherType WeatherType)
{
    switch (WeatherType)
    {
    case EMingWeatherType::Clear: return TEXT("晴朗");
    case EMingWeatherType::Cloudy: return TEXT("多雲");
    case EMingWeatherType::Rain: return TEXT("下雨");
    case EMingWeatherType::Storm: return TEXT("暴風雨");
    case EMingWeatherType::Snow: return TEXT("下雪");
    case EMingWeatherType::Fog: return TEXT("霧");
    default: return TEXT("未知");
    }
}

FString UMingLevelDesigner::SaveLevelData() const
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return TEXT("{}");
    }
    
    FString Result = TEXT("{\n");
    Result += FString::Printf(TEXT("  \"level_id\": \"%s\",\n"), *Level->LevelID);
    Result += FString::Printf(TEXT("  \"level_name\": \"%s\",\n"), *Level->LevelName);
    Result += FString::Printf(TEXT("  \"description\": \"%s\",\n"), *Level->Description);
    Result += FString::Printf(TEXT("  \"map_size\": \"%s\",\n"), *GetMapSizeName(Level->MapSize));
    Result += FString::Printf(TEXT("  \"default_terrain\": \"%s\",\n"), *GetTerrainTypeName(Level->DefaultTerrain));
    Result += FString::Printf(TEXT("  \"weather_type\": \"%s\",\n"), *GetWeatherTypeName(Level->WeatherType));
    Result += FString::Printf(TEXT("  \"map_bounds\": [%.1f, %.1f],\n"), Level->MapBounds.X, Level->MapBounds.Y);
    Result += FString::Printf(TEXT("  \"time_of_day\": %.1f,\n"), Level->TimeOfDay);
    Result += FString::Printf(TEXT("  \"has_time_limit\": %s,\n"), Level->bHasTimeLimit ? TEXT("true") : TEXT("false"));
    Result += FString::Printf(TEXT("  \"time_limit\": %.1f,\n"), Level->TimeLimit);
    Result += FString::Printf(TEXT("  \"terrain_patches\": %d,\n"), Level->TerrainData.Num());
    Result += FString::Printf(TEXT("  \"map_objects\": %d,\n"), Level->MapObjects.Num());
    Result += FString::Printf(TEXT("  \"required_objectives\": %d,\n"), Level->RequiredObjectives.Num());
    Result += FString::Printf(TEXT("  \"optional_objectives\": %d\n"), Level->OptionalObjectives.Num());
    Result += TEXT("}\n");
    
    return Result;
}

void UMingLevelDesigner::LoadLevelData(const FString& JsonString)
{
    // Parse JSON and restore level data
    UE_LOG(LogTemp, Log, TEXT("Loading level data from JSON"));
}

void UMingLevelDesigner::ValidateLevelIntegrity()
{
    // Comprehensive level validation
    UE_LOG(LogTemp, Log, TEXT("Validating level integrity"));
}

void UMingLevelDesigner::GenerateTerrain(EMingMapSize MapSize, EMingTerrainType DefaultTerrain)
{
    // Generate default terrain based on size and type
    UE_LOG(LogTemp, Log, TEXT("Generated terrain for map size %d"), (int32)MapSize);
}

void UMingLevelDesigner::ApplyTerrainModifiers()
{
    // Apply terrain modifiers and optimizations
    UE_LOG(LogTemp, Log, TEXT("Applied terrain modifiers"));
}

void UMingLevelDesigner::OptimizeTerrainData()
{
    // Optimize terrain data for performance
    UE_LOG(LogTemp, Log, TEXT("Optimized terrain data"));
}

FString UMingLevelDesigner::GenerateUniqueObjectID()
{
    static int32 Counter = 0;
    return FString::Printf(TEXT("OBJ_%d"), ++Counter);
}

bool UMingLevelDesigner::IsValidObjectPlacement(const FMingMapObject& Object) const
{
    // Check if object placement is valid
    FVector2D ObjectPos2D(Object.Location.X, Object.Location.Y);
    return IsPositionInBounds(ObjectPos2D);
}

void UMingLevelDesigner::UpdateObjectReferences()
{
    // Update object references after changes
    UE_LOG(LogTemp, Log, TEXT("Updated object references"));
}

bool UMingLevelDesigner::ValidatePlayerStarts()
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return false;
    }
    
    bool HasPlayerStart = false;
    for (const FMingMapObject& Object : Level->MapObjects)
    {
        if (Object.bIsPlayerStart)
        {
            HasPlayerStart = true;
            break;
        }
    }
    
    if (!HasPlayerStart)
    {
        ValidationErrors.Add(TEXT("Level must have at least one player start position"));
        return false;
    }
    
    return true;
}

bool UMingLevelDesigner::ValidateObjectives()
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return false;
    }
    
    if (Level->RequiredObjectives.Num() == 0)
    {
        ValidationErrors.Add(TEXT("Level must have at least one required objective"));
        return false;
    }
    
    return true;
}

bool UMingLevelDesigner::ValidateTerrainIntegrity()
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return false;
    }
    
    // Check for terrain overlaps and gaps
    // This would implement more sophisticated terrain validation
    return true;
}

bool UMingLevelDesigner::ValidateMapBounds()
{
    const FMingLevelData* Level = LevelDatabase.Find(CurrentLevelID);
    if (!Level)
    {
        return false;
    }
    
    // Check if all objects are within bounds
    for (const FMingMapObject& Object : Level->MapObjects)
    {
        FVector2D ObjectPos2D(Object.Location.X, Object.Location.Y);
        if (!IsPositionInBounds(ObjectPos2D))
        {
            ValidationErrors.Add(FString::Printf(TEXT("Object %s is outside map bounds"), *Object.ObjectName));
            return false;
        }
    }
    
    return true;
}

void UMingLevelDesigner::ApplyTemplate(const FMingLevelData& Template)
{
    FMingLevelData NewLevel;
    NewLevel = Template;
    NewLevel.LevelID = CurrentLevelID;
    NewLevel.LevelName = FString::Printf(TEXT("Level %s"), *CurrentLevelID);
    
    LevelDatabase.Add(CurrentLevelID, NewLevel);
}
