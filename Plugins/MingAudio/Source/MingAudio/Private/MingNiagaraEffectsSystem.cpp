#include "MingNiagaraEffectsSystem.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

UMingNiagaraEffectsSystem::UMingNiagaraEffectsSystem()
    : MaxConcurrentEffects(100)
    , ActiveEffectCount(0)
    , CurrentWeatherEffect(nullptr)
{
}

void UMingNiagaraEffectsSystem::InitializeNiagaraSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Niagara Effects System"));
    
    InitializeEffectTemplates();
    
    UE_LOG(LogTemp, Log, TEXT("Niagara Effects System initialized with max %d concurrent effects"), MaxConcurrentEffects);
}

void UMingNiagaraEffectsSystem::ShutdownNiagaraSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Niagara Effects System"));
    
    // Stop all active effects
    for (auto* Effect : ActiveEffects)
    {
        if (Effect)
        {
            Effect->Deactivate();
        }
    }
    
    ActiveEffects.Empty();
    ActiveEffectCount = 0;
    
    // Clear weather effect
    if (CurrentWeatherEffect)
    {
        CurrentWeatherEffect->Deactivate();
        CurrentWeatherEffect = nullptr;
    }
}

void UMingNiagaraEffectsSystem::InitializeEffectTemplates()
{
    // Set up default Niagara system paths
    NiagaraSystemPaths.Add(TEXT("Explosion"), TEXT("/Game/Effects/Niagara/NS_Explosion"));
    NiagaraSystemPaths.Add(TEXT("MuzzleFlash"), TEXT("/Game/Effects/Niagara/NS_MuzzleFlash"));
    NiagaraSystemPaths.Add(TEXT("BulletTrail"), TEXT("/Game/Effects/Niagara/NS_BulletTrail"));
    NiagaraSystemPaths.Add(TEXT("Impact"), TEXT("/Game/Effects/Niagara/NS_Impact"));
    NiagaraSystemPaths.Add(TEXT("Fire"), TEXT("/Game/Effects/Niagara/NS_Fire"));
    NiagaraSystemPaths.Add(TEXT("Smoke"), TEXT("/Game/Effects/Niagara/NS_Smoke"));
    NiagaraSystemPaths.Add(TEXT("Dust"), TEXT("/Game/Effects/Niagara/NS_Dust"));
    NiagaraSystemPaths.Add(TEXT("Spark"), TEXT("/Game/Effects/Niagara/NS_Spark"));
    NiagaraSystemPaths.Add(TEXT("Rain"), TEXT("/Game/Effects/Niagara/NS_Rain"));
    NiagaraSystemPaths.Add(TEXT("Snow"), TEXT("/Game/Effects/Niagara/NS_Snow"));
    NiagaraSystemPaths.Add(TEXT("Fog"), TEXT("/Game/Effects/Niagara/NS_Fog"));
    NiagaraSystemPaths.Add(TEXT("Selection"), TEXT("/Game/Effects/Niagara/NS_Selection"));
}

// Explosion Effects
UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnExplosion(const FVector& Location, const FMingExplosionParams& Params)
{
    UNiagaraComponent* Explosion = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Explosion, Location);
    
    if (Explosion)
    {
        // Set explosion parameters
        Explosion->SetFloatParameter(TEXT("Scale"), Params.Scale);
        Explosion->SetFloatParameter(TEXT("Duration"), Params.Duration);
        Explosion->SetFloatParameter(TEXT("ParticleCount"), static_cast<float>(Params.ParticleCount));
        Explosion->SetColorParameter(TEXT("FireColor"), Params.FireColor);
        Explosion->SetColorParameter(TEXT("SmokeColor"), Params.SmokeColor);
        Explosion->SetBoolParameter(TEXT("bSparks"), Params.bSparks);
        Explosion->SetBoolParameter(TEXT("bShockwave"), Params.bShockwave);
        Explosion->SetBoolParameter(TEXT("bDebris"), Params.bDebris);
        
        Explosion->Activate(true);
        OnNiagaraEffectSpawned.Broadcast(EMingNiagaraEffectType::Explosion, Explosion);
    }
    
    return Explosion;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnGrenadeExplosion(const FVector& Location, float Scale)
{
    FMingExplosionParams Params;
    Params.ExplosionType = EMingExplosionType::Small;
    Params.Scale = Scale;
    Params.ParticleCount = 50;
    Params.Duration = 1.5f;
    Params.bShockwave = false;
    
    return SpawnExplosion(Location, Params);
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnArtilleryExplosion(const FVector& Location, float Scale)
{
    FMingExplosionParams Params;
    Params.ExplosionType = EMingExplosionType::Artillery;
    Params.Scale = Scale;
    Params.ParticleCount = 300;
    Params.Duration = 4.0f;
    Params.bShockwave = true;
    Params.bDebris = true;
    Params.FireColor = FLinearColor(1.0f, 0.4f, 0.05f);
    
    return SpawnExplosion(Location, Params);
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnBarrelExplosion(const FVector& Location)
{
    FMingExplosionParams Params;
    Params.ExplosionType = EMingExplosionType::Medium;
    Params.Scale = 1.5f;
    Params.ParticleCount = 150;
    Params.Duration = 3.0f;
    Params.bShockwave = true;
    
    return SpawnExplosion(Location, Params);
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnVehicleExplosion(const FVector& Location)
{
    FMingExplosionParams Params;
    Params.ExplosionType = EMingExplosionType::Large;
    Params.Scale = 3.0f;
    Params.ParticleCount = 500;
    Params.Duration = 5.0f;
    Params.bShockwave = true;
    Params.bDebris = true;
    Params.SmokeColor = FLinearColor(0.1f, 0.1f, 0.1f);
    
    return SpawnExplosion(Location, Params);
}

// Muzzle Flash Effects
UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnMuzzleFlash(const FVector& Location, const FVector& Direction, const FMingMuzzleFlashParams& Params)
{
    FQuat Rotation = FQuat::FindBetweenNormals(FVector::ForwardVector, Direction.GetSafeNormal());
    UNiagaraComponent* MuzzleFlash = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::MuzzleFlash, Location, Rotation);
    
    if (MuzzleFlash)
    {
        MuzzleFlash->SetFloatParameter(TEXT("Caliber"), Params.Caliber);
        MuzzleFlash->SetFloatParameter(TEXT("Scale"), Params.Scale);
        MuzzleFlash->SetFloatParameter(TEXT("Duration"), Params.Duration);
        MuzzleFlash->SetColorParameter(TEXT("FlashColor"), Params.FlashColor);
        MuzzleFlash->SetBoolParameter(TEXT("bSmoke"), Params.bSmoke);
        
        MuzzleFlash->Activate(true);
        OnNiagaraEffectSpawned.Broadcast(EMingNiagaraEffectType::MuzzleFlash, MuzzleFlash);
    }
    
    return MuzzleFlash;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnRifleMuzzleFlash(const FVector& Location, const FVector& Direction)
{
    FMingMuzzleFlashParams Params;
    Params.Caliber = 7.62f;
    Params.Scale = 1.0f;
    Params.Duration = 0.05f;
    Params.bSmoke = true;
    
    return SpawnMuzzleFlash(Location, Direction, Params);
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnPistolMuzzleFlash(const FVector& Location, const FVector& Direction)
{
    FMingMuzzleFlashParams Params;
    Params.Caliber = 9.0f;
    Params.Scale = 0.7f;
    Params.Duration = 0.04f;
    Params.bSmoke = true;
    Params.FlashColor = FLinearColor(1.0f, 0.8f, 0.3f);
    
    return SpawnMuzzleFlash(Location, Direction, Params);
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnMachineGunMuzzleFlash(const FVector& Location, const FVector& Direction)
{
    FMingMuzzleFlashParams Params;
    Params.Caliber = 7.62f;
    Params.Scale = 1.2f;
    Params.Duration = 0.08f;
    Params.bSmoke = true;
    Params.FlashColor = FLinearColor(1.0f, 0.7f, 0.2f);
    
    return SpawnMuzzleFlash(Location, Direction, Params);
}

// Bullet Trail Effects
UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnBulletTrail(const FVector& StartLocation, const FVector& EndLocation, const FMingTrailParams& Params)
{
    FVector Direction = EndLocation - StartLocation;
    float Distance = Direction.Size();
    FQuat Rotation = FQuat::FindBetweenNormals(FVector::ForwardVector, Direction.GetSafeNormal());
    
    // Spawn at midpoint
    FVector Midpoint = (StartLocation + EndLocation) * 0.5f;
    UNiagaraComponent* Trail = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::BulletTrail, Midpoint, Rotation);
    
    if (Trail)
    {
        Trail->SetFloatParameter(TEXT("Thickness"), Params.Thickness);
        Trail->SetFloatParameter(TEXT("Length"), Distance);
        Trail->SetFloatParameter(TEXT("Duration"), Params.Duration);
        Trail->SetFloatParameter(TEXT("FadeTime"), Params.FadeTime);
        Trail->SetColorParameter(TEXT("TrailColor"), Params.Color);
        
        Trail->Activate(true);
        OnNiagaraEffectSpawned.Broadcast(EMingNiagaraEffectType::BulletTrail, Trail);
    }
    
    return Trail;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnTracerRound(const FVector& StartLocation, const FVector& EndLocation, FLinearColor Color)
{
    FMingTrailParams Params;
    Params.Thickness = 4.0f;
    Params.Color = Color;
    Params.Duration = 0.3f;
    Params.FadeTime = 0.2f;
    
    return SpawnBulletTrail(StartLocation, EndLocation, Params);
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnProjectileTrail(const FVector& StartLocation, const FVector& EndLocation, float Thickness)
{
    FMingTrailParams Params;
    Params.Thickness = Thickness;
    Params.Color = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);
    Params.Duration = 1.0f;
    Params.FadeTime = 0.5f;
    
    return SpawnBulletTrail(StartLocation, EndLocation, Params);
}

// Impact Effects
UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnBulletImpact(const FVector& Location, const FVector& Normal, float Caliber)
{
    FQuat Rotation = FQuat::FindBetweenNormals(FVector::UpVector, Normal.GetSafeNormal());
    UNiagaraComponent* Impact = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Impact, Location, Rotation);
    
    if (Impact)
    {
        Impact->SetFloatParameter(TEXT("Caliber"), Caliber);
        Impact->SetFloatParameter(TEXT("ImpactForce"), Caliber / 10.0f);
        Impact->Activate(true);
        OnNiagaraEffectSpawned.Broadcast(EMingNiagaraEffectType::Impact, Impact);
    }
    
    return Impact;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnGroundImpact(const FVector& Location, float Scale)
{
    UNiagaraComponent* Impact = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Impact, Location);
    
    if (Impact)
    {
        Impact->SetFloatParameter(TEXT("Scale"), Scale);
        Impact->SetColorParameter(TEXT("DustColor"), FLinearColor(0.4f, 0.35f, 0.3f));
        Impact->Activate(true);
    }
    
    return Impact;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnBloodImpact(const FVector& Location, float Scale)
{
    UNiagaraComponent* Impact = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Impact, Location);
    
    if (Impact)
    {
        Impact->SetFloatParameter(TEXT("Scale"), Scale);
        Impact->SetColorParameter(TEXT("BloodColor"), FLinearColor(0.6f, 0.05f, 0.05f));
        Impact->SetBoolParameter(TEXT("bBlood"), true);
        Impact->Activate(true);
    }
    
    return Impact;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnSparks(const FVector& Location, int32 Count)
{
    UNiagaraComponent* Sparks = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Spark, Location);
    
    if (Sparks)
    {
        Sparks->SetIntParameter(TEXT("SparkCount"), Count);
        Sparks->Activate(true);
        OnNiagaraEffectSpawned.Broadcast(EMingNiagaraEffectType::Spark, Sparks);
    }
    
    return Sparks;
}

// Fire and Smoke Effects
UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnFire(const FVector& Location, float Scale, float Duration)
{
    UNiagaraComponent* Fire = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Fire, Location);
    
    if (Fire)
    {
        Fire->SetFloatParameter(TEXT("Scale"), Scale);
        Fire->SetFloatParameter(TEXT("Duration"), Duration);
        Fire->SetColorParameter(TEXT("FireColor"), FLinearColor(1.0f, 0.5f, 0.1f));
        Fire->Activate(true);
        OnNiagaraEffectSpawned.Broadcast(EMingNiagaraEffectType::Fire, Fire);
    }
    
    return Fire;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnSmoke(const FVector& Location, float Scale, FLinearColor Color)
{
    UNiagaraComponent* Smoke = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Smoke, Location);
    
    if (Smoke)
    {
        Smoke->SetFloatParameter(TEXT("Scale"), Scale);
        Smoke->SetColorParameter(TEXT("SmokeColor"), Color);
        Smoke->SetFloatParameter(TEXT("Duration"), 10.0f);
        Smoke->Activate(true);
        OnNiagaraEffectSpawned.Broadcast(EMingNiagaraEffectType::Smoke, Smoke);
    }
    
    return Smoke;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnDust(const FVector& Location, float Scale)
{
    UNiagaraComponent* Dust = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Dust, Location);
    
    if (Dust)
    {
        Dust->SetFloatParameter(TEXT("Scale"), Scale);
        Dust->SetColorParameter(TEXT("DustColor"), FLinearColor(0.5f, 0.45f, 0.4f));
        Dust->Activate(true);
        OnNiagaraEffectSpawned.Broadcast(EMingNiagaraEffectType::Dust, Dust);
    }
    
    return Dust;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnBurningObject(const FVector& Location, float Scale)
{
    UNiagaraComponent* Fire = SpawnFire(Location, Scale, 60.0f);
    
    // Also spawn smoke
    FVector SmokeLocation = Location + FVector(0, 0, 100 * Scale);
    SpawnSmoke(SmokeLocation, Scale * 2.0f);
    
    return Fire;
}

// Weather Effects
UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnWeatherEffect(const FMingWeatherParams& Params)
{
    // Stop existing weather effect
    StopWeatherEffect();
    
    FVector Location = FVector::ZeroVector;
    UNiagaraComponent* Weather = nullptr;
    
    switch (Params.WeatherType)
    {
    case EMingWeatherType::Rain:
        Weather = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Weather, Location);
        break;
    case EMingWeatherType::Snow:
        Weather = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Weather, Location);
        break;
    case EMingWeatherType::Fog:
        Weather = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Weather, Location);
        break;
    default:
        Weather = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Weather, Location);
        break;
    }
    
    if (Weather)
    {
        Weather->SetFloatParameter(TEXT("Intensity"), Params.Intensity);
        Weather->SetFloatParameter(TEXT("WindSpeed"), Params.WindSpeed);
        Weather->SetVectorParameter(TEXT("WindDirection"), Params.WindDirection);
        Weather->SetColorParameter(TEXT("WeatherColor"), Params.Color);
        Weather->SetFloatParameter(TEXT("Coverage"), Params.Coverage);
        Weather->Activate(true);
        
        CurrentWeatherEffect = Weather;
    }
    
    return Weather;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnRain(float Intensity)
{
    FMingWeatherParams Params;
    Params.WeatherType = EMingWeatherType::Rain;
    Params.Intensity = Intensity;
    Params.Color = FLinearColor(0.6f, 0.65f, 0.7f);
    
    return SpawnWeatherEffect(Params);
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnSnow(float Intensity)
{
    FMingWeatherParams Params;
    Params.WeatherType = EMingWeatherType::Snow;
    Params.Intensity = Intensity;
    Params.Color = FLinearColor(0.95f, 0.95f, 0.98f);
    
    return SpawnWeatherEffect(Params);
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnFog(float Density)
{
    FMingWeatherParams Params;
    Params.WeatherType = EMingWeatherType::Fog;
    Params.Intensity = Density;
    Params.Color = FLinearColor(0.7f, 0.72f, 0.75f);
    
    return SpawnWeatherEffect(Params);
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnDustStorm(float Intensity)
{
    FMingWeatherParams Params;
    Params.WeatherType = EMingWeatherType::DustStorm;
    Params.Intensity = Intensity;
    Params.Color = FLinearColor(0.6f, 0.55f, 0.45f);
    Params.WindSpeed = 50.0f;
    
    return SpawnWeatherEffect(Params);
}

void UMingNiagaraEffectsSystem::StopWeatherEffect()
{
    if (CurrentWeatherEffect)
    {
        CurrentWeatherEffect->Deactivate();
        CurrentWeatherEffect = nullptr;
    }
}

// Environment Effects
UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnLeaves(const FVector& Location, float Count)
{
    UNiagaraComponent* Leaves = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Environment, Location);
    
    if (Leaves)
    {
        Leaves->SetFloatParameter(TEXT("ParticleCount"), Count);
        Leaves->SetColorParameter(TEXT("ParticleColor"), FLinearColor(0.3f, 0.5f, 0.2f));
        Leaves->Activate(true);
    }
    
    return Leaves;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnButterflies(const FVector& Location, float Count)
{
    UNiagaraComponent* Butterflies = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Environment, Location);
    
    if (Butterflies)
    {
        Butterflies->SetFloatParameter(TEXT("ParticleCount"), Count);
        Butterflies->SetColorParameter(TEXT("ParticleColor"), FLinearColor(0.8f, 0.6f, 0.2f));
        Butterflies->SetFloatParameter(TEXT("Speed"), 2.0f);
        Butterflies->Activate(true);
    }
    
    return Butterflies;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnFireflies(const FVector& Location, float Count)
{
    UNiagaraComponent* Fireflies = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Environment, Location);
    
    if (Fireflies)
    {
        Fireflies->SetFloatParameter(TEXT("ParticleCount"), Count);
        Fireflies->SetColorParameter(TEXT("ParticleColor"), FLinearColor(0.9f, 1.0f, 0.3f));
        Fireflies->SetFloatParameter(TEXT("Glow"), 1.0f);
        Fireflies->Activate(true);
    }
    
    return Fireflies;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnPollen(const FVector& Location, float Density)
{
    UNiagaraComponent* Pollen = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Environment, Location);
    
    if (Pollen)
    {
        Pollen->SetFloatParameter(TEXT("ParticleCount"), Density * 100.0f);
        Pollen->SetColorParameter(TEXT("ParticleColor"), FLinearColor(0.95f, 0.9f, 0.7f));
        Pollen->Activate(true);
    }
    
    return Pollen;
}

// UI/Selection Effects
UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnSelectionRing(const FVector& Location, float Radius, FLinearColor Color)
{
    UNiagaraComponent* Selection = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::Selection, Location);
    
    if (Selection)
    {
        Selection->SetFloatParameter(TEXT("Radius"), Radius);
        Selection->SetColorParameter(TEXT("RingColor"), Color);
        Selection->SetFloatParameter(TEXT("Thickness"), 5.0f);
        Selection->Activate(true);
        OnNiagaraEffectSpawned.Broadcast(EMingNiagaraEffectType::Selection, Selection);
    }
    
    return Selection;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnTargetMarker(const FVector& Location, float Scale)
{
    UNiagaraComponent* Marker = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::UI, Location);
    
    if (Marker)
    {
        Marker->SetFloatParameter(TEXT("Scale"), Scale);
        Marker->SetColorParameter(TEXT("MarkerColor"), FLinearColor(1.0f, 0.0f, 0.0f));
        Marker->Activate(true);
    }
    
    return Marker;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnCommandMarker(const FVector& Location, FLinearColor Color)
{
    UNiagaraComponent* Marker = SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType::UI, Location);
    
    if (Marker)
    {
        Marker->SetFloatParameter(TEXT("Scale"), 1.5f);
        Marker->SetColorParameter(TEXT("MarkerColor"), Color);
        Marker->SetFloatParameter(TEXT("Duration"), 2.0f);
        Marker->Activate(true);
    }
    
    return Marker;
}

// Utility Functions
void UMingNiagaraEffectsSystem::SetEffectScale(UNiagaraComponent* NiagaraComponent, float Scale)
{
    if (NiagaraComponent)
    {
        NiagaraComponent->SetFloatParameter(TEXT("Scale"), Scale);
    }
}

void UMingNiagaraEffectsSystem::SetEffectColor(UNiagaraComponent* NiagaraComponent, FLinearColor Color)
{
    if (NiagaraComponent)
    {
        NiagaraComponent->SetColorParameter(TEXT("Color"), Color);
    }
}

void UMingNiagaraEffectsSystem::SetEffectDuration(UNiagaraComponent* NiagaraComponent, float Duration)
{
    if (NiagaraComponent)
    {
        NiagaraComponent->SetFloatParameter(TEXT("Duration"), Duration);
    }
}

void UMingNiagaraEffectsSystem::StopEffect(UNiagaraComponent* NiagaraComponent)
{
    if (NiagaraComponent)
    {
        NiagaraComponent->Deactivate();
    }
}

void UMingNiagaraEffectsSystem::DestroyEffect(UNiagaraComponent* NiagaraComponent)
{
    if (NiagaraComponent)
    {
        NiagaraComponent->DestroyComponent();
        ActiveEffects.Remove(NiagaraComponent);
        ActiveEffectCount--;
    }
}

void UMingNiagaraEffectsSystem::AttachEffectToComponent(UNiagaraComponent* NiagaraComponent, USceneComponent* ParentComponent, FName SocketName)
{
    if (NiagaraComponent && ParentComponent)
    {
        if (SocketName != NAME_None)
        {
            NiagaraComponent->AttachToComponent(ParentComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
        }
        else
        {
            NiagaraComponent->AttachToComponent(ParentComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        }
    }
}

// System Management
void UMingNiagaraEffectsSystem::PreloadEffectSystems()
{
    UE_LOG(LogTemp, Log, TEXT("Preloading Niagara effect systems..."));
    
    // Load all effect templates
    for (const auto& Pair : NiagaraSystemPaths)
    {
        // Load the Niagara system asset
        // This would use LoadObject<UNiagaraSystem>() in actual implementation
        UE_LOG(LogTemp, Log, TEXT("Preloading: %s from %s"), *Pair.Key, *Pair.Value);
    }
}

void UMingNiagaraEffectsSystem::ClearEffectPool()
{
    for (auto* Effect : ActiveEffects)
    {
        if (Effect)
        {
            Effect->Deactivate();
            Effect->DestroyComponent();
        }
    }
    
    ActiveEffects.Empty();
    ActiveEffectCount = 0;
}

void UMingNiagaraEffectsSystem::SetMaxConcurrentEffects(int32 MaxCount)
{
    MaxConcurrentEffects = MaxCount;
    
    // Clean up excess effects if needed
    while (ActiveEffects.Num() > MaxConcurrentEffects)
    {
        if (ActiveEffects[0])
        {
            ActiveEffects[0]->Deactivate();
            ActiveEffects[0]->DestroyComponent();
        }
        ActiveEffects.RemoveAt(0);
        ActiveEffectCount--;
    }
}

int32 UMingNiagaraEffectsSystem::GetActiveEffectCount() const
{
    return ActiveEffectCount;
}

bool UMingNiagaraEffectsSystem::IsEffectSystemReady() const
{
    return EffectTemplates.Num() > 0;
}

// Batch Operations
void UMingNiagaraEffectsSystem::SpawnExplosionField(const TArray<FVector>& Locations, float ExplosionScale, float DelayBetween)
{
    // This would use a timer to spawn explosions sequentially
    for (int32 i = 0; i < Locations.Num(); i++)
    {
        FMingExplosionParams Params;
        Params.Scale = ExplosionScale;
        
        // In actual implementation, use FTimerManager for delays
        SpawnExplosion(Locations[i], Params);
    }
}

void UMingNiagaraEffectsSystem::SpawnArtilleryBarrage(const FVector& StartLocation, const FVector& EndLocation, int32 ShotCount, float Interval)
{
    FVector Direction = (EndLocation - StartLocation) / FMath::Max(ShotCount - 1, 1);
    
    for (int32 i = 0; i < ShotCount; i++)
    {
        FVector TargetLocation = StartLocation + Direction * i;
        
        // Add some randomness to impact location
        TargetLocation += FVector(FMath::RandRange(-100.0f, 100.0f), FMath::RandRange(-100.0f, 100.0f), 0);
        
        // In actual implementation, use FTimerManager for intervals
        SpawnArtilleryExplosion(TargetLocation, 2.0f);
    }
}

// Internal functions
UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType Type, const FVector& Location, const FQuat& Rotation)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }
    
    // Check concurrent limit
    if (ActiveEffectCount >= MaxConcurrentEffects)
    {
        CleanupFinishedEffects();
        
        if (ActiveEffectCount >= MaxConcurrentEffects)
        {
            UE_LOG(LogTemp, Warning, TEXT("Max concurrent effects reached (%d)"), MaxConcurrentEffects);
            return nullptr;
        }
    }
    
    UNiagaraSystem* Template = GetEffectTemplate(Type);
    if (!Template)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Niagara template found for effect type: %d"), static_cast<int32>(Type));
        return nullptr;
    }
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        World,
        Template,
        Location,
        Rotation.Rotator(),
        FVector(1.0f),
        true,
        true,
        ENCPoolMethod::None,
        true
    );
    
    if (NiagaraComponent)
    {
        ActiveEffects.Add(NiagaraComponent);
        ActiveEffectCount++;
        
        // Bind to finish event for cleanup
        // NiagaraComponent->OnSystemFinished.AddDynamic(this, &UMingNiagaraEffectsSystem::OnEffectFinished);
    }
    
    return NiagaraComponent;
}

UNiagaraComponent* UMingNiagaraEffectsSystem::SpawnNiagaraEffectAttached(EMingNiagaraEffectType Type, USceneComponent* AttachComponent, FName SocketName)
{
    if (!AttachComponent)
    {
        return nullptr;
    }
    
    UNiagaraSystem* Template = GetEffectTemplate(Type);
    if (!Template)
    {
        return nullptr;
    }
    
    UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
        Template,
        AttachComponent,
        SocketName,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget,
        true,
        true,
        ENCPoolMethod::None,
        true
    );
    
    if (NiagaraComponent)
    {
        ActiveEffects.Add(NiagaraComponent);
        ActiveEffectCount++;
    }
    
    return NiagaraComponent;
}

UNiagaraSystem* UMingNiagaraEffectsSystem::GetEffectTemplate(EMingNiagaraEffectType Type)
{
    UNiagaraSystem** FoundTemplate = EffectTemplates.Find(Type);
    if (FoundTemplate && *FoundTemplate)
    {
        return *FoundTemplate;
    }
    
    // Try to load from path
    FString PathKey;
    switch (Type)
    {
    case EMingNiagaraEffectType::Explosion: PathKey = TEXT("Explosion"); break;
    case EMingNiagaraEffectType::MuzzleFlash: PathKey = TEXT("MuzzleFlash"); break;
    case EMingNiagaraEffectType::BulletTrail: PathKey = TEXT("BulletTrail"); break;
    case EMingNiagaraEffectType::Impact: PathKey = TEXT("Impact"); break;
    case EMingNiagaraEffectType::Fire: PathKey = TEXT("Fire"); break;
    case EMingNiagaraEffectType::Smoke: PathKey = TEXT("Smoke"); break;
    case EMingNiagaraEffectType::Dust: PathKey = TEXT("Dust"); break;
    case EMingNiagaraEffectType::Spark: PathKey = TEXT("Spark"); break;
    case EMingNiagaraEffectType::Weather: PathKey = TEXT("Rain"); break;
    case EMingNiagaraEffectType::Environment: PathKey = TEXT("Leaves"); break;
    case EMingNiagaraEffectType::UI: PathKey = TEXT("Selection"); break;
    case EMingNiagaraEffectType::Selection: PathKey = TEXT("Selection"); break;
    default: PathKey = TEXT("Explosion"); break;
    }
    
    FString* AssetPath = NiagaraSystemPaths.Find(PathKey);
    if (AssetPath)
    {
        // In actual implementation:
        // UNiagaraSystem* LoadedSystem = LoadObject<UNiagaraSystem>(nullptr, **AssetPath);
        // if (LoadedSystem)
        // {
        //     EffectTemplates.Add(Type, LoadedSystem);
        //     return LoadedSystem;
        // }
    }
    
    return nullptr;
}

void UMingNiagaraEffectsSystem::OnEffectFinished(UNiagaraComponent* NiagaraComponent)
{
    if (NiagaraComponent)
    {
        ActiveEffects.Remove(NiagaraComponent);
        ActiveEffectCount = FMath::Max(0, ActiveEffectCount - 1);
    }
}

void UMingNiagaraEffectsSystem::CleanupFinishedEffects()
{
    for (int32 i = ActiveEffects.Num() - 1; i >= 0; i--)
    {
        if (!ActiveEffects[i] || !ActiveEffects[i]->IsActive())
        {
            ActiveEffects.RemoveAt(i);
            ActiveEffectCount--;
        }
    }
    
    ActiveEffectCount = FMath::Max(0, ActiveEffectCount);
}

UWorld* UMingNiagaraEffectsSystem::GetWorld() const
{
    if (WorldContext.IsValid())
    {
        return WorldContext.Get();
    }
    
    if (GEngine)
    {
        for (const FWorldContext& Context : GEngine->GetWorldContexts())
        {
            if (Context.World() && (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE))
            {
                return Context.World();
            }
        }
    }
    
    return nullptr;
}
