#include "MingMetaSoundsSystem.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundWaveProcedural.h"
#include "AudioDevice.h"
#include "AudioMixer.h"

UMingMetaSoundsSystem::UMingMetaSoundsSystem()
    : MasterPitch(1.0f)
    , MasterVolume(1.0f)
{
}

void UMingMetaSoundsSystem::InitializeMetaSounds()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing MetaSounds Procedural Audio System"));
    
    InitializeDefaultPatches();
    
    UE_LOG(LogTemp, Log, TEXT("MetaSounds System initialized successfully"));
}

void UMingMetaSoundsSystem::ShutdownMetaSounds()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down MetaSounds System"));
    
    // Unload all patches
    for (auto& Pair : LoadedPatches)
    {
        if (Pair.Value)
        {
            Pair.Value->RemoveFromRoot();
        }
    }
    LoadedPatches.Empty();
}

void UMingMetaSoundsSystem::InitializeDefaultPatches()
{
    // Setup default patch paths (these would reference MetaSound Patch assets)
    DefaultPatchPaths.Add(EMingProceduralSFXType::Gunshot, TEXT("/Game/Audio/MetaSounds/Patches/GunshotPatch"));
    DefaultPatchPaths.Add(EMingProceduralSFXType::Footstep, TEXT("/Game/Audio/MetaSounds/Patches/FootstepPatch"));
    DefaultPatchPaths.Add(EMingProceduralSFXType::Explosion, TEXT("/Game/Audio/MetaSounds/Patches/ExplosionPatch"));
    DefaultPatchPaths.Add(EMingProceduralSFXType::Impact, TEXT("/Game/Audio/MetaSounds/Patches/ImpactPatch"));
    DefaultPatchPaths.Add(EMingProceduralSFXType::Ambient, TEXT("/Game/Audio/MetaSounds/Patches/AmbientPatch"));
    DefaultPatchPaths.Add(EMingProceduralSFXType::UI, TEXT("/Game/Audio/MetaSounds/Patches/UIPatch"));
    DefaultPatchPaths.Add(EMingProceduralSFXType::Vehicle, TEXT("/Game/Audio/MetaSounds/Patches/VehiclePatch"));
}

bool UMingMetaSoundsSystem::IsMetaSoundsEnabled() const
{
    // Check if MetaSounds are available in the engine
    if (GEngine && GEngine->GetAudioDeviceManager())
    {
        return true;
    }
    return false;
}

// Gunshot Generation
USoundBase* UMingMetaSoundsSystem::GenerateGunshot(const FMingGunshotParams& Params)
{
    USoundBase* GeneratedSound = CreateGunshotWave(Params);
    
    if (GeneratedSound)
    {
        ApplyRandomization(GeneratedSound, 0.08f, 0.15f);
        OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::Gunshot, GeneratedSound);
    }
    
    return GeneratedSound;
}

void UMingMetaSoundsSystem::PlayGunshot(const FMingGunshotParams& Params, FVector Location)
{
    USoundBase* GunshotSound = GenerateGunshot(Params);
    
    if (GunshotSound)
    {
        if (Location.IsZero())
        {
            UGameplayStatics::PlaySound2D(GetWorld(), GunshotSound, MasterVolume, MasterPitch);
        }
        else
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunshotSound, Location, MasterVolume, MasterPitch);
        }
    }
}

USoundBase* UMingMetaSoundsSystem::CreateGunshotWave(const FMingGunshotParams& Params)
{
    // Create procedural sound wave for gunshot
    USoundWaveProcedural* GunshotWave = NewObject<USoundWaveProcedural>();
    
    if (!GunshotWave)
    {
        return nullptr;
    }
    
    // Calculate gunshot characteristics based on parameters
    float BaseFrequency = 2000.0f - (Params.Caliber * 50.0f); // Larger caliber = lower pitch
    float DecayTime = 0.3f + (Params.Caliber * 0.02f); // Larger caliber = longer decay
    float DistanceAttenuation = FMath::Clamp(1.0f - (Params.Distance / 5000.0f), 0.1f, 1.0f);
    
    if (Params.bSuppressed)
    {
        BaseFrequency *= 0.7f;
        DecayTime *= 0.5f;
        DistanceAttenuation *= 0.6f;
    }
    
    // Apply environment reverb
    DecayTime *= (1.0f + Params.EnvironmentReverb);
    
    // Generate audio data (simplified implementation)
    // In a full implementation, this would generate actual PCM data
    // For now, we create a placeholder that references a base gunshot sound
    GunshotWave->Duration = DecayTime;
    GunshotWave->SetPitch(MasterPitch * (1.0f + FMath::RandRange(-0.05f, 0.05f)));
    
    return GunshotWave;
}

USoundBase* UMingMetaSoundsSystem::GenerateRifleShot(float Caliber, float Distance)
{
    FMingGunshotParams Params;
    Params.Caliber = Caliber;
    Params.Distance = Distance;
    Params.BarrelLength = 60.0f;
    
    return GenerateGunshot(Params);
}

USoundBase* UMingMetaSoundsSystem::GeneratePistolShot(float Caliber, float Distance)
{
    FMingGunshotParams Params;
    Params.Caliber = Caliber;
    Params.Distance = Distance;
    Params.BarrelLength = 15.0f;
    
    return GenerateGunshot(Params);
}

USoundBase* UMingMetaSoundsSystem::GenerateMachineGunBurst(int32 ShotCount, float Caliber)
{
    // Generate multiple shots with slight timing variations
    // This would be implemented as a composite sound or sequence
    USoundBase* FirstShot = GenerateRifleShot(Caliber, 0.0f);
    return FirstShot;
}

// Footstep Generation
USoundBase* UMingMetaSoundsSystem::GenerateFootstep(const FMingFootstepParams& Params)
{
    USoundBase* GeneratedSound = CreateFootstepWave(Params);
    
    if (GeneratedSound)
    {
        ApplyRandomization(GeneratedSound, 0.1f, 0.1f);
        OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::Footstep, GeneratedSound);
    }
    
    return GeneratedSound;
}

void UMingMetaSoundsSystem::PlayFootstep(const FMingFootstepParams& Params, FVector Location)
{
    USoundBase* FootstepSound = GenerateFootstep(Params);
    
    if (FootstepSound)
    {
        float VolumeMultiplier = 1.0f;
        
        // Reduce volume for sneaking
        if (Params.bSneaking)
        {
            VolumeMultiplier *= 0.3f;
        }
        
        // Increase volume for running
        if (Params.bRunning)
        {
            VolumeMultiplier *= 1.3f;
        }
        
        // Add armor weight factor
        VolumeMultiplier *= (1.0f + (Params.ArmorWeight * 0.01f));
        
        if (Location.IsZero())
        {
            UGameplayStatics::PlaySound2D(GetWorld(), FootstepSound, MasterVolume * VolumeMultiplier, MasterPitch * Params.Speed);
        }
        else
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSound, Location, MasterVolume * VolumeMultiplier, MasterPitch * Params.Speed);
        }
    }
}

USoundBase* UMingMetaSoundsSystem::CreateFootstepWave(const FMingFootstepParams& Params)
{
    USoundWaveProcedural* FootstepWave = NewObject<USoundWaveProcedural>();
    
    if (!FootstepWave)
    {
        return nullptr;
    }
    
    // Calculate footstep characteristics
    float Duration = 0.15f;
    float Frequency = 500.0f;
    
    // Surface type affects sound characteristics
    switch (Params.SurfaceType)
    {
    case EMingSurfaceType::Concrete:
        Frequency = 800.0f;
        Duration = 0.12f;
        break;
    case EMingSurfaceType::Wood:
        Frequency = 400.0f;
        Duration = 0.18f;
        break;
    case EMingSurfaceType::Metal:
        Frequency = 1200.0f;
        Duration = 0.25f;
        break;
    case EMingSurfaceType::Grass:
        Frequency = 200.0f;
        Duration = 0.1f;
        break;
    case EMingSurfaceType::Dirt:
        Frequency = 300.0f;
        Duration = 0.15f;
        break;
    case EMingSurfaceType::Snow:
        Frequency = 150.0f;
        Duration = 0.2f;
        break;
    case EMingSurfaceType::Water:
        Frequency = 600.0f;
        Duration = 0.22f;
        break;
    case EMingSurfaceType::Sand:
        Frequency = 250.0f;
        Duration = 0.16f;
        break;
    }
    
    // Weight affects volume and low frequencies
    float WeightFactor = Params.Weight / 75.0f; // Normalize to 75kg average
    Frequency *= FMath::Lerp(0.8f, 1.2f, WeightFactor);
    
    FootstepWave->Duration = Duration;
    FootstepWave->SetPitch(MasterPitch * (1.0f + FMath::RandRange(-0.05f, 0.05f)));
    
    return FootstepWave;
}

USoundBase* UMingMetaSoundsSystem::GenerateFootstepOnSurface(EMingSurfaceType Surface, float Weight, bool bRunning)
{
    FMingFootstepParams Params;
    Params.SurfaceType = Surface;
    Params.Weight = Weight;
    Params.bRunning = bRunning;
    Params.Speed = bRunning ? 1.5f : 1.0f;
    
    return GenerateFootstep(Params);
}

// Explosion Generation
USoundBase* UMingMetaSoundsSystem::GenerateExplosion(const FMingExplosionParams& Params)
{
    USoundBase* GeneratedSound = CreateExplosionWave(Params);
    
    if (GeneratedSound)
    {
        OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::Explosion, GeneratedSound);
    }
    
    return GeneratedSound;
}

void UMingMetaSoundsSystem::PlayExplosion(const FMingExplosionParams& Params, FVector Location)
{
    USoundBase* ExplosionSound = GenerateExplosion(Params);
    
    if (ExplosionSound)
    {
        float DistanceAttenuation = FMath::Clamp(1.0f - (Params.Distance / 10000.0f), 0.1f, 1.0f);
        
        if (Location.IsZero())
        {
            UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound, MasterVolume * DistanceAttenuation, MasterPitch);
        }
        else
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, Location, MasterVolume * DistanceAttenuation, MasterPitch);
        }
    }
}

USoundBase* UMingMetaSoundsSystem::CreateExplosionWave(const FMingExplosionParams& Params)
{
    USoundWaveProcedural* ExplosionWave = NewObject<USoundWaveProcedural>();
    
    if (!ExplosionWave)
    {
        return nullptr;
    }
    
    // Calculate explosion characteristics
    float Duration = 2.0f + (Params.ExplosiveAmount * 0.5f);
    float DistanceAttenuation = FMath::Clamp(1.0f - (Params.Distance / 5000.0f), 0.1f, 1.0f);
    
    if (Params.bUnderwater)
    {
        Duration *= 1.5f;
    }
    
    ExplosionWave->Duration = Duration * DistanceAttenuation;
    ExplosionWave->SetPitch(MasterPitch);
    
    return ExplosionWave;
}

USoundBase* UMingMetaSoundsSystem::GenerateGrenadeExplosion(float Distance)
{
    FMingExplosionParams Params;
    Params.ExplosiveAmount = 0.5f;
    Params.Distance = Distance;
    Params.DebrisAmount = 0.3f;
    
    return GenerateExplosion(Params);
}

USoundBase* UMingMetaSoundsSystem::GenerateArtilleryExplosion(float ExplosiveAmount)
{
    FMingExplosionParams Params;
    Params.ExplosiveAmount = ExplosiveAmount;
    Params.Distance = 0.0f;
    Params.DebrisAmount = 0.8f;
    
    return GenerateExplosion(Params);
}

// Impact Sounds
USoundBase* UMingMetaSoundsSystem::GenerateImpactSound(EMingSurfaceType Surface, float ImpactForce)
{
    USoundBase* GeneratedSound = CreateImpactWave(Surface, ImpactForce);
    
    if (GeneratedSound)
    {
        OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::Impact, GeneratedSound);
    }
    
    return GeneratedSound;
}

void UMingMetaSoundsSystem::PlayBulletImpact(EMingSurfaceType Surface, FVector Location, float BulletCaliber)
{
    float ImpactForce = BulletCaliber / 10.0f;
    USoundBase* ImpactSound = GenerateImpactSound(Surface, ImpactForce);
    
    if (ImpactSound && !Location.IsZero())
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location, MasterVolume, MasterPitch);
    }
}

USoundBase* UMingMetaSoundsSystem::CreateImpactWave(EMingSurfaceType Surface, float Force)
{
    USoundWaveProcedural* ImpactWave = NewObject<USoundWaveProcedural>();
    
    if (!ImpactWave)
    {
        return nullptr;
    }
    
    float Duration = 0.1f + (Force * 0.05f);
    ImpactWave->Duration = Duration;
    ImpactWave->SetPitch(MasterPitch * (1.0f + FMath::RandRange(-0.1f, 0.1f)));
    
    return ImpactWave;
}

// Ambient Sounds
USoundBase* UMingMetaSoundsSystem::GenerateWindAmbient(float Intensity, bool bGusty)
{
    USoundBase* GeneratedSound = CreateAmbientWave(Intensity, TEXT("Wind"));
    
    if (GeneratedSound)
    {
        OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::Ambient, GeneratedSound);
    }
    
    return GeneratedSound;
}

USoundBase* UMingMetaSoundsSystem::GenerateRainAmbient(float Intensity)
{
    USoundBase* GeneratedSound = CreateAmbientWave(Intensity, TEXT("Rain"));
    
    if (GeneratedSound)
    {
        OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::Ambient, GeneratedSound);
    }
    
    return GeneratedSound;
}

USoundBase* UMingMetaSoundsSystem::GenerateBattleAmbient(int32 UnitCount, float Distance)
{
    // Calculate battle ambient intensity based on unit count and distance
    float Intensity = FMath::Clamp(UnitCount / 1000.0f, 0.0f, 1.0f);
    Intensity *= FMath::Clamp(1.0f - (Distance / 2000.0f), 0.1f, 1.0f);
    
    USoundBase* GeneratedSound = CreateAmbientWave(Intensity, TEXT("Battle"));
    
    if (GeneratedSound)
    {
        OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::Ambient, GeneratedSound);
    }
    
    return GeneratedSound;
}

USoundBase* UMingMetaSoundsSystem::CreateAmbientWave(float Intensity, const FString& Type)
{
    USoundWaveProcedural* AmbientWave = NewObject<USoundWaveProcedural>();
    
    if (!AmbientWave)
    {
        return nullptr;
    }
    
    // Long duration for ambient sounds (loopable)
    AmbientWave->Duration = 10.0f;
    AmbientWave->SetPitch(MasterPitch);
    
    return AmbientWave;
}

// UI Sounds
USoundBase* UMingMetaSoundsSystem::GenerateUIClick(bool bConfirm)
{
    USoundBase* GeneratedSound = CreateUIWave(bConfirm);
    
    if (GeneratedSound)
    {
        OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::UI, GeneratedSound);
    }
    
    return GeneratedSound;
}

void UMingMetaSoundsSystem::PlayUISelect()
{
    USoundBase* ClickSound = GenerateUIClick(true);
    if (ClickSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), ClickSound, MasterVolume * 0.8f, MasterPitch);
    }
}

void UMingMetaSoundsSystem::PlayUIClick()
{
    USoundBase* ClickSound = GenerateUIClick(true);
    if (ClickSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), ClickSound, MasterVolume, MasterPitch);
    }
}

void UMingMetaSoundsSystem::PlayUIHover()
{
    USoundBase* HoverSound = GenerateUIHover();
    if (HoverSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), HoverSound, MasterVolume * 0.5f, MasterPitch * 1.2f);
    }
}

void UMingMetaSoundsSystem::PlayUIError()
{
    USoundBase* ErrorSound = GenerateUIError();
    if (ErrorSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), ErrorSound, MasterVolume, MasterPitch * 0.8f);
    }
}

void UMingMetaSoundsSystem::PlayUISuccess()
{
    USoundBase* SuccessSound = GenerateUIClick(true);
    if (SuccessSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), SuccessSound, MasterVolume, MasterPitch * 1.1f);
    }
}

USoundBase* UMingMetaSoundsSystem::GenerateUIHover()
{
    USoundWaveProcedural* UIWave = NewObject<USoundWaveProcedural>();
    
    if (!UIWave)
    {
        return nullptr;
    }
    
    UIWave->Duration = 0.05f;
    UIWave->SetPitch(MasterPitch * 1.5f);
    
    return UIWave;
}

USoundBase* UMingMetaSoundsSystem::GenerateUIError()
{
    USoundWaveProcedural* UIWave = NewObject<USoundWaveProcedural>();
    
    if (!UIWave)
    {
        return nullptr;
    }
    
    UIWave->Duration = 0.3f;
    UIWave->SetPitch(MasterPitch * 0.7f);
    
    return UIWave;
}

USoundBase* UMingMetaSoundsSystem::CreateUIWave(bool bConfirm)
{
    USoundWaveProcedural* UIWave = NewObject<USoundWaveProcedural>();
    
    if (!UIWave)
    {
        return nullptr;
    }
    
    UIWave->Duration = 0.1f;
    UIWave->SetPitch(MasterPitch * (bConfirm ? 1.0f : 0.8f));
    
    return UIWave;
}

// Vehicle Sounds
USoundBase* UMingMetaSoundsSystem::GenerateEngineSound(float RPM, float Load, int32 Cylinders)
{
    USoundBase* GeneratedSound = CreateEngineWave(RPM, Load, Cylinders);
    
    if (GeneratedSound)
    {
        OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::Vehicle, GeneratedSound);
    }
    
    return GeneratedSound;
}

USoundBase* UMingMetaSoundsSystem::GenerateTankTrackSound(float Speed, EMingSurfaceType Surface)
{
    USoundWaveProcedural* TrackWave = NewObject<USoundWaveProcedural>();
    
    if (!TrackWave)
    {
        return nullptr;
    }
    
    float Duration = 2.0f;
    float PitchMultiplier = Speed / 30.0f; // Normalize to 30 km/h
    
    TrackWave->Duration = Duration;
    TrackWave->SetPitch(MasterPitch * PitchMultiplier);
    
    OnProceduralSFXGenerated.Broadcast(EMingProceduralSFXType::Vehicle, TrackWave);
    
    return TrackWave;
}

USoundBase* UMingMetaSoundsSystem::CreateEngineWave(float RPM, float Load, int32 Cylinders)
{
    USoundWaveProcedural* EngineWave = NewObject<USoundWaveProcedural>();
    
    if (!EngineWave)
    {
        return nullptr;
    }
    
    // Calculate engine sound characteristics
    float BaseFrequency = (RPM / 60.0f) * Cylinders / 2.0f; // Firing frequency
    float Duration = 2.0f;
    float PitchMultiplier = RPM / 1000.0f; // Normalize to 1000 RPM
    
    EngineWave->Duration = Duration;
    EngineWave->SetPitch(MasterPitch * PitchMultiplier);
    
    return EngineWave;
}

// Utility Functions
void UMingMetaSoundsSystem::SetMasterPitch(float Pitch)
{
    MasterPitch = FMath::Clamp(Pitch, 0.5f, 2.0f);
}

void UMingMetaSoundsSystem::SetMasterVolume(float Volume)
{
    MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

float UMingMetaSoundsSystem::GetRandomVariation(float BaseValue, float VariationRange) const
{
    return BaseValue * (1.0f + FMath::RandRange(-VariationRange, VariationRange));
}

void UMingMetaSoundsSystem::ApplyRandomization(USoundBase* Sound, float PitchRange, float VolumeRange)
{
    if (!Sound)
    {
        return;
    }
    
    // Apply random pitch variation
    float RandomPitch = 1.0f + FMath::RandRange(-PitchRange, PitchRange);
    Sound->SetPitch(RandomPitch * MasterPitch);
}

FString UMingMetaSoundsSystem::GetSurfaceMaterialName(EMingSurfaceType Surface) const
{
    switch (Surface)
    {
    case EMingSurfaceType::Concrete: return TEXT("Concrete");
    case EMingSurfaceType::Wood: return TEXT("Wood");
    case EMingSurfaceType::Metal: return TEXT("Metal");
    case EMingSurfaceType::Grass: return TEXT("Grass");
    case EMingSurfaceType::Dirt: return TEXT("Dirt");
    case EMingSurfaceType::Snow: return TEXT("Snow");
    case EMingSurfaceType::Water: return TEXT("Water");
    case EMingSurfaceType::Sand: return TEXT("Sand");
    default: return TEXT("Unknown");
    }
}

float UMingMetaSoundsSystem::CalculateDecayTime(float Distance, float EnvironmentScale) const
{
    float BaseDecay = 1.0f;
    float DistanceFactor = FMath::Clamp(Distance / 1000.0f, 0.0f, 5.0f);
    return BaseDecay + (DistanceFactor * EnvironmentScale);
}

float UMingMetaSoundsSystem::CalculateDopplerEffect(float Velocity, float SoundSpeed) const
{
    if (SoundSpeed <= 0.0f)
    {
        return 1.0f;
    }
    
    float MachNumber = Velocity / SoundSpeed;
    return 1.0f / (1.0f + MachNumber);
}

// Patch Management
void UMingMetaSoundsSystem::LoadMetaSoundPatch(const FString& PatchName, const FString& PatchPath)
{
    // In a full implementation, this would load actual MetaSound Patch assets
    // For now, we store the path for reference
    UE_LOG(LogTemp, Log, TEXT("Loading MetaSound Patch: %s from %s"), *PatchName, *PatchPath);
    
    // Placeholder - actual implementation would use LoadObject<> to load the patch
    OnMetaSoundPatchCreated.Broadcast(PatchName);
}

void UMingMetaSoundsSystem::UnloadMetaSoundPatch(const FString& PatchName)
{
    if (LoadedPatches.Contains(PatchName))
    {
        LoadedPatches.Remove(PatchName);
        UE_LOG(LogTemp, Log, TEXT("Unloaded MetaSound Patch: %s"), *PatchName);
    }
}

void UMingMetaSoundsSystem::CreateRuntimePatch(const FString& PatchName, EMingProceduralSFXType Type)
{
    // Create a runtime MetaSound patch based on the procedural type
    UE_LOG(LogTemp, Log, TEXT("Creating runtime MetaSound Patch: %s for type %d"), *PatchName, (int32)Type);
    
    OnMetaSoundPatchCreated.Broadcast(PatchName);
}

// Helper to get world context
UWorld* UMingMetaSoundsSystem::GetWorld() const
{
    if (GEngine)
    {
        // Return the first game world
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
