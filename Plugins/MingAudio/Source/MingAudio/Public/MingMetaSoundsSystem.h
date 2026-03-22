#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Sound/SoundBase.h"
#include "MingMetaSoundsSystem.generated.h"

UENUM(BlueprintType)
enum class EMingProceduralSFXType : uint8
{
    Gunshot UMETA(DisplayName = "Gunshot"),
    Footstep UMETA(DisplayName = "Footstep"),
    Explosion UMETA(DisplayName = "Explosion"),
    SwordClash UMETA(DisplayName = "Sword Clash"),
    Impact UMETA(DisplayName = "Impact"),
    Ambient UMETA(DisplayName = "Ambient"),
    UI UMETA(DisplayName = "UI Sound"),
    Vehicle UMETA(DisplayName = "Vehicle")
};

UENUM(BlueprintType)
enum class EMingSurfaceType : uint8
{
    Concrete UMETA(DisplayName = "Concrete"),
    Wood UMETA(DisplayName = "Wood"),
    Metal UMETA(DisplayName = "Metal"),
    Grass UMETA(DisplayName = "Grass"),
    Dirt UMETA(DisplayName = "Dirt"),
    Snow UMETA(DisplayName = "Snow"),
    Water UMETA(DisplayName = "Water"),
    Sand UMETA(DisplayName = "Sand")
};

USTRUCT(BlueprintType)
struct FMingGunshotParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gunshot")
    float Caliber = 7.62f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gunshot")
    float BarrelLength = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gunshot")
    bool bSuppressed = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gunshot")
    float Distance = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gunshot")
    float EnvironmentReverb = 0.5f;

    FMingGunshotParams()
        : Caliber(7.62f)
        , BarrelLength(50.0f)
        , bSuppressed(false)
        , Distance(0.0f)
        , EnvironmentReverb(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FMingFootstepParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
    EMingSurfaceType SurfaceType = EMingSurfaceType::Concrete;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
    float Weight = 75.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
    float Speed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
    bool bSneaking = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
    bool bRunning = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
    float ArmorWeight = 0.0f;

    FMingFootstepParams()
        : SurfaceType(EMingSurfaceType::Concrete)
        , Weight(75.0f)
        , Speed(1.0f)
        , bSneaking(false)
        , bRunning(false)
        , ArmorWeight(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingExplosionParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float ExplosiveAmount = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float Distance = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    bool bUnderwater = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float DebrisAmount = 0.5f;

    FMingExplosionParams()
        : ExplosiveAmount(1.0f)
        , Distance(100.0f)
        , bUnderwater(false)
        , DebrisAmount(0.5f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProceduralSFXGenerated, EMingProceduralSFXType, SFXType, USoundBase*, GeneratedSound};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMetaSoundPatchCreated, const FString&, PatchName};

/**
 * MetaSounds-based Procedural Audio System
 * Generates sound effects in real-time using MetaSounds patches
 */
UCLASS(ClassGroup = (Audio), Blueprintable)
class MINGAUDIO_API UMingMetaSoundsSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingMetaSoundsSystem(};

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "MetaSounds")
    void InitializeMetaSounds(};

    UFUNCTION(BlueprintCallable, Category = "MetaSounds")
    void ShutdownMetaSounds(};

    // Gunshot Generation
    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Gunshot")
    USoundBase* GenerateGunshot(const FMingGunshotParams& Params};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Gunshot")
    void PlayGunshot(const FMingGunshotParams& Params, FVector Location = FVector::ZeroVector};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Gunshot")
    USoundBase* GenerateRifleShot(float Caliber = 7.62f, float Distance = 0.0f};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Gunshot")
    USoundBase* GeneratePistolShot(float Caliber = 9.0f, float Distance = 0.0f};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Gunshot")
    USoundBase* GenerateMachineGunBurst(int32 ShotCount = 3, float Caliber = 7.62f};

    // Footstep Generation
    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Footstep")
    USoundBase* GenerateFootstep(const FMingFootstepParams& Params};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Footstep")
    void PlayFootstep(const FMingFootstepParams& Params, FVector Location = FVector::ZeroVector};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Footstep")
    USoundBase* GenerateFootstepOnSurface(EMingSurfaceType Surface, float Weight = 75.0f, bool bRunning = false};

    // Explosion Generation
    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Explosion")
    USoundBase* GenerateExplosion(const FMingExplosionParams& Params};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Explosion")
    void PlayExplosion(const FMingExplosionParams& Params, FVector Location = FVector::ZeroVector};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Explosion")
    USoundBase* GenerateGrenadeExplosion(float Distance = 50.0f};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Explosion")
    USoundBase* GenerateArtilleryExplosion(float ExplosiveAmount = 10.0f};

    // Impact Sounds
    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Impact")
    USoundBase* GenerateImpactSound(EMingSurfaceType Surface, float ImpactForce = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Impact")
    void PlayBulletImpact(EMingSurfaceType Surface, FVector Location, float BulletCaliber = 7.62f};

    // Ambient Sounds
    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Ambient")
    USoundBase* GenerateWindAmbient(float Intensity = 0.5f, bool bGusty = false};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Ambient")
    USoundBase* GenerateRainAmbient(float Intensity = 0.5f};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Ambient")
    USoundBase* GenerateBattleAmbient(int32 UnitCount = 100, float Distance = 500.0f};

    // UI Sounds
    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|UI")
    USoundBase* GenerateUIClick(bool bConfirm = true};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|UI")
    USoundBase* GenerateUIHover(};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|UI")
    USoundBase* GenerateUIError(};

    // Vehicle Sounds
    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Vehicle")
    USoundBase* GenerateEngineSound(float RPM, float Load = 0.5f, int32 Cylinders = 4};

    UFUNCTION(BlueprintCallable, Category = "Procedural SFX|Vehicle")
    USoundBase* GenerateTankTrackSound(float Speed, EMingSurfaceType Surface};

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "MetaSounds|Utility")
    bool IsMetaSoundsEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "MetaSounds|Utility")
    void SetMasterPitch(float Pitch};

    UFUNCTION(BlueprintCallable, Category = "MetaSounds|Utility")
    void SetMasterVolume(float Volume};

    UFUNCTION(BlueprintPure, Category = "MetaSounds|Utility")
    float GetRandomVariation(float BaseValue, float VariationRange = 0.1f) const;

    // Patch Management
    UFUNCTION(BlueprintCallable, Category = "MetaSounds|Patches")
    void LoadMetaSoundPatch(const FString& PatchName, const FString& PatchPath};

    UFUNCTION(BlueprintCallable, Category = "MetaSounds|Patches")
    void UnloadMetaSoundPatch(const FString& PatchName};

    UFUNCTION(BlueprintCallable, Category = "MetaSounds|Patches")
    void CreateRuntimePatch(const FString& PatchName, EMingProceduralSFXType Type};

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnProceduralSFXGenerated OnProceduralSFXGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMetaSoundPatchCreated OnMetaSoundPatchCreated;

protected:
    UPROPERTY()
    float MasterPitch = 1.0f;

    UPROPERTY()
    float MasterVolume = 1.0f;

    UPROPERTY()
    TMap<FString, USoundBase*> LoadedPatches;

    UPROPERTY()
    TMap<EMingProceduralSFXType, FString> DefaultPatchPaths;

    // Internal generation functions
    USoundBase* CreateGunshotWave(const FMingGunshotParams& Params};
    USoundBase* CreateFootstepWave(const FMingFootstepParams& Params};
    USoundBase* CreateExplosionWave(const FMingExplosionParams& Params};
    USoundBase* CreateImpactWave(EMingSurfaceType Surface, float Force};
    USoundBase* CreateAmbientWave(float Intensity, const FString& Type};
    USoundBase* CreateUIWave(bool bConfirm};
    USoundBase* CreateEngineWave(float RPM, float Load, int32 Cylinders};

    // Helper functions
    FString GetSurfaceMaterialName(EMingSurfaceType Surface) const;
    float CalculateDecayTime(float Distance, float EnvironmentScale) const;
    float CalculateDopplerEffect(float Velocity, float SoundSpeed = 343.0f) const;
    void ApplyRandomization(USoundBase* Sound, float PitchRange = 0.05f, float VolumeRange = 0.1f};

    // Initialization
    void InitializeDefaultPatches(};
    void SetupGunshotPatch(};
    void SetupFootstepPatch(};
    void SetupExplosionPatch(};
    void SetupImpactPatch(};
    void SetupAmbientPatch(};
    void SetupUIPatch(};
    void SetupVehiclePatch(};
};

