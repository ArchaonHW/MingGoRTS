#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MingNiagaraEffectsSystem.generated.h"

UENUM(BlueprintType)
enum class EMingNiagaraEffectType : uint8
{
    Explosion UMETA(DisplayName = "Explosion"),
    MuzzleFlash UMETA(DisplayName = "Muzzle Flash"),
    BulletTrail UMETA(DisplayName = "Bullet Trail"),
    Impact UMETA(DisplayName = "Impact"),
    Fire UMETA(DisplayName = "Fire"),
    Smoke UMETA(DisplayName = "Smoke"),
    Dust UMETA(DisplayName = "Dust"),
    Spark UMETA(DisplayName = "Spark"),
    Weather UMETA(DisplayName = "Weather"),
    Environment UMETA(DisplayName = "Environment"),
    UI UMETA(DisplayName = "UI Effect"),
    Selection UMETA(DisplayName = "Selection")
};

UENUM(BlueprintType)
enum class EMingExplosionType : uint8
{
    Small UMETA(DisplayName = "Small"),
    Medium UMETA(DisplayName = "Medium"),
    Large UMETA(DisplayName = "Large"),
    Artillery UMETA(DisplayName = "Artillery"),
    Nuclear UMETA(DisplayName = "Nuclear"),
    Chemical UMETA(DisplayName = "Chemical")
};

UENUM(BlueprintType)
enum class EMingWeatherType : uint8
{
    Rain UMETA(DisplayName = "Rain"),
    Snow UMETA(DisplayName = "Snow"),
    Fog UMETA(DisplayName = "Fog"),
    DustStorm UMETA(DisplayName = "Dust Storm"),
    Ash UMETA(DisplayName = "Ash Fall")
};

USTRUCT(BlueprintType)
struct FMingNiagaraExplosionParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    EMingExplosionType ExplosionType = EMingExplosionType::Medium;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float Scale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    FLinearColor FireColor = FLinearColor(1.0f, 0.6f, 0.1f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    FLinearColor SmokeColor = FLinearColor(0.3f, 0.3f, 0.3f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float Duration = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    int32 ParticleCount = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    bool bSparks = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    bool bShockwave = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    bool bDebris = true;

    FMingExplosionParams()
        : ExplosionType(EMingExplosionType::Medium)
        , Scale(1.0f)
        , FireColor(1.0f, 0.6f, 0.1f)
        , SmokeColor(0.3f, 0.3f, 0.3f)
        , Duration(2.0f)
        , ParticleCount(100)
        , bSparks(true)
        , bShockwave(true)
        , bDebris(true)
    {}
};

USTRUCT(BlueprintType)
struct FMingMuzzleFlashParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Flash")
    float Caliber = 7.62f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Flash")
    FLinearColor FlashColor = FLinearColor(1.0f, 0.9f, 0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Flash")
    float Duration = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Flash")
    float Scale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Flash")
    bool bSmoke = true;

    FMingMuzzleFlashParams()
        : Caliber(7.62f)
        , FlashColor(1.0f, 0.9f, 0.5f)
        , Duration(0.05f)
        , Scale(1.0f)
        , bSmoke(true)
    {}
};

USTRUCT(BlueprintType)
struct FMingTrailParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    float Thickness = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    FLinearColor Color = FLinearColor(0.8f, 0.8f, 0.7f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    float Duration = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    float FadeTime = 0.3f;

    FMingTrailParams()
        : Thickness(2.0f)
        , Color(0.8f, 0.8f, 0.7f)
        , Duration(0.5f)
        , FadeTime(0.3f)
    {}
};

USTRUCT(BlueprintType)
struct FMingWeatherParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    EMingWeatherType WeatherType = EMingWeatherType::Rain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    float Intensity = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    float WindSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    FVector WindDirection = FVector(1.0f, 0.0f, 0.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    FLinearColor Color = FLinearColor(0.7f, 0.75f, 0.8f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    float Coverage = 10000.0f;

    FMingWeatherParams()
        : WeatherType(EMingWeatherType::Rain)
        , Intensity(0.5f)
        , WindSpeed(10.0f)
        , WindDirection(1.0f, 0.0f, 0.0f)
        , Color(0.7f, 0.75f, 0.8f)
        , Coverage(10000.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNiagaraEffectSpawned, EMingNiagaraEffectType, EffectType, UNiagaraComponent*, NiagaraComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNiagaraSystemCreated, const FString&, SystemName);

/**
 * Niagara-based Procedural Visual Effects System
 * Generates VFX in real-time using Niagara particle systems
 */
UCLASS(ClassGroup = (Effects), Blueprintable)
class MINGAUDIO_API UMingNiagaraEffectsSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingNiagaraEffectsSystem(};

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects")
    void InitializeNiagaraSystem(};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects")
    void ShutdownNiagaraSystem(};

    // Explosion Effects
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Explosions")
    UNiagaraComponent* SpawnExplosion(const FVector& Location, const FMingExplosionParams& Params};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Explosions")
    UNiagaraComponent* SpawnGrenadeExplosion(const FVector& Location, float Scale = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Explosions")
    UNiagaraComponent* SpawnArtilleryExplosion(const FVector& Location, float Scale = 2.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Explosions")
    UNiagaraComponent* SpawnBarrelExplosion(const FVector& Location};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Explosions")
    UNiagaraComponent* SpawnVehicleExplosion(const FVector& Location};

    // Muzzle Flash Effects
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Muzzle Flash")
    UNiagaraComponent* SpawnMuzzleFlash(const FVector& Location, const FVector& Direction, const FMingMuzzleFlashParams& Params};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Muzzle Flash")
    UNiagaraComponent* SpawnRifleMuzzleFlash(const FVector& Location, const FVector& Direction};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Muzzle Flash")
    UNiagaraComponent* SpawnPistolMuzzleFlash(const FVector& Location, const FVector& Direction};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Muzzle Flash")
    UNiagaraComponent* SpawnMachineGunMuzzleFlash(const FVector& Location, const FVector& Direction};

    // Bullet Trail Effects
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Trails")
    UNiagaraComponent* SpawnBulletTrail(const FVector& StartLocation, const FVector& EndLocation, const FMingTrailParams& Params};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Trails")
    UNiagaraComponent* SpawnTracerRound(const FVector& StartLocation, const FVector& EndLocation, FLinearColor Color = FLinearColor(1.0f, 0.3f, 0.0f)};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Trails")
    UNiagaraComponent* SpawnProjectileTrail(const FVector& StartLocation, const FVector& EndLocation, float Thickness = 5.0f};

    // Impact Effects
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Impacts")
    UNiagaraComponent* SpawnBulletImpact(const FVector& Location, const FVector& Normal, float Caliber = 7.62f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Impacts")
    UNiagaraComponent* SpawnGroundImpact(const FVector& Location, float Scale = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Impacts")
    UNiagaraComponent* SpawnBloodImpact(const FVector& Location, float Scale = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Impacts")
    UNiagaraComponent* SpawnSparks(const FVector& Location, int32 Count = 20};

    // Fire and Smoke Effects
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Fire/Smoke")
    UNiagaraComponent* SpawnFire(const FVector& Location, float Scale = 1.0f, float Duration = 5.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Fire/Smoke")
    UNiagaraComponent* SpawnSmoke(const FVector& Location, float Scale = 1.0f, FLinearColor Color = FLinearColor(0.3f, 0.3f, 0.3f)};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Fire/Smoke")
    UNiagaraComponent* SpawnDust(const FVector& Location, float Scale = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Fire/Smoke")
    UNiagaraComponent* SpawnBurningObject(const FVector& Location, float Scale = 1.0f};

    // Weather Effects
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Weather")
    UNiagaraComponent* SpawnWeatherEffect(const FMingWeatherParams& Params};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Weather")
    UNiagaraComponent* SpawnRain(float Intensity = 0.5f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Weather")
    UNiagaraComponent* SpawnSnow(float Intensity = 0.5f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Weather")
    UNiagaraComponent* SpawnFog(float Density = 0.3f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Weather")
    UNiagaraComponent* SpawnDustStorm(float Intensity = 0.7f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Weather")
    void StopWeatherEffect(};

    // Environment Effects
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Environment")
    UNiagaraComponent* SpawnLeaves(const FVector& Location, float Count = 50.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Environment")
    UNiagaraComponent* SpawnButterflies(const FVector& Location, float Count = 20.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Environment")
    UNiagaraComponent* SpawnFireflies(const FVector& Location, float Count = 30.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Environment")
    UNiagaraComponent* SpawnPollen(const FVector& Location, float Density = 0.5f};

    // UI/Selection Effects
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|UI")
    UNiagaraComponent* SpawnSelectionRing(const FVector& Location, float Radius = 100.0f, FLinearColor Color = FLinearColor(1.0f, 1.0f, 0.0f)};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|UI")
    UNiagaraComponent* SpawnTargetMarker(const FVector& Location, float Scale = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|UI")
    UNiagaraComponent* SpawnCommandMarker(const FVector& Location, FLinearColor Color = FLinearColor(0.0f, 1.0f, 0.0f)};

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Utility")
    void SetEffectScale(UNiagaraComponent* NiagaraComponent, float Scale};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Utility")
    void SetEffectColor(UNiagaraComponent* NiagaraComponent, FLinearColor Color};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Utility")
    void SetEffectDuration(UNiagaraComponent* NiagaraComponent, float Duration};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Utility")
    void StopEffect(UNiagaraComponent* NiagaraComponent};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Utility")
    void DestroyEffect(UNiagaraComponent* NiagaraComponent};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Utility")
    void AttachEffectToComponent(UNiagaraComponent* NiagaraComponent, USceneComponent* ParentComponent, FName SocketName = NAME_None};

    // System Management
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|System")
    void PreloadEffectSystems(};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|System")
    void ClearEffectPool(};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|System")
    void SetMaxConcurrentEffects(int32 MaxCount};

    UFUNCTION(BlueprintPure, Category = "Niagara Effects|System")
    int32 GetActiveEffectCount() const;

    UFUNCTION(BlueprintPure, Category = "Niagara Effects|System")
    bool IsEffectSystemReady() const;

    // Batch Operations
    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Batch")
    void SpawnExplosionField(const TArray<FVector>& Locations, float ExplosionScale = 1.0f, float DelayBetween = 0.1f};

    UFUNCTION(BlueprintCallable, Category = "Niagara Effects|Batch")
    void SpawnArtilleryBarrage(const FVector& StartLocation, const FVector& EndLocation, int32 ShotCount = 10, float Interval = 0.5f};

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnNiagaraEffectSpawned OnNiagaraEffectSpawned;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnNiagaraSystemCreated OnNiagaraSystemCreated;

protected:
    UPROPERTY()
    int32 MaxConcurrentEffects = 100;

    UPROPERTY()
    int32 ActiveEffectCount = 0;

    UPROPERTY()
    TMap<EMingNiagaraEffectType, UNiagaraSystem*> EffectTemplates;

    UPROPERTY()
    TArray<UNiagaraComponent*> ActiveEffects;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    UPROPERTY()
    UNiagaraComponent* CurrentWeatherEffect;

    // Template paths
    UPROPERTY()
    TMap<FString, FString> NiagaraSystemPaths;

    // Internal functions
    UNiagaraComponent* SpawnNiagaraEffectAtLocation(EMingNiagaraEffectType Type, const FVector& Location, const FQuat& Rotation = FQuat::Identity);
    UNiagaraComponent* SpawnNiagaraEffectAttached(EMingNiagaraEffectType Type, USceneComponent* AttachComponent, FName SocketName);
    
    void InitializeEffectTemplates();
    void LoadEffectTemplate(EMingNiagaraEffectType Type, const FString& AssetPath);
    
    UNiagaraSystem* GetEffectTemplate(EMingNiagaraEffectType Type);
    
    void OnEffectFinished(UNiagaraComponent* NiagaraComponent);
    void CleanupFinishedEffects();
    
    void UpdateEffectParameters(UNiagaraComponent* NiagaraComponent, const TMap<FName, float>& FloatParams, const TMap<FName, FLinearColor>& ColorParams);
    
    UWorld* GetWorld() const;
};



#endif // MINGNIAGARAEFFECTSSYSTEM_H
