#include "MingCombatVisualizer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "FXSystemComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h"

UMingCombatVisualizer::UMingCombatVisualizer()
{
    bBatchModeEnabled = false;
    MaxEffectsPerFrame = 50;
    bEffectsEnabled = true;
}

UMingCombatVisualizer* UMingCombatVisualizer::Get(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    static TMap<TWeakObjectPtr<UWorld>, UMingCombatVisualizer*> WorldVisualizers;

    TWeakObjectPtr<UWorld> WorldKey = World;
    UMingCombatVisualizer** Existing = WorldVisualizers.Find(WorldKey);

    if (Existing && *Existing)
    {
        return *Existing;
    }

    UMingCombatVisualizer* NewVisualizer = NewObject<UMingCombatVisualizer>(World);
    NewVisualizer->CachedWorld = World;
    WorldVisualizers.Add(WorldKey, NewVisualizer);

    return NewVisualizer;
}

void UMingCombatVisualizer::SpawnCombatEffect(const FMingCombatEffect& Effect)
{
    if (!bEffectsEnabled)
    {
        return;
    }

    if (bBatchModeEnabled)
    {
        PendingEffects.Add(Effect);
    }
    else
    {
        SpawnEffectInternal(Effect);
    }
}

void UMingCombatVisualizer::SpawnMuzzleFlash(AActor* Source, FVector Location, FVector Direction)
{
    FMingCombatEffect Effect;
    Effect.EffectType = EMingCombatEffectType::MuzzleFlash;
    Effect.Location = Location;
    Effect.Direction = Direction;
    Effect.Scale = 1.0f;
    Effect.Duration = 0.1f;
    Effect.Color = FLinearColor(1.0f, 0.8f, 0.2f);
    Effect.SourceActor = Source;
    Effect.bAttachToSource = true;

    SpawnCombatEffect(Effect);

    // Play sound
    if (CachedWorld.IsValid())
    {
        UGameplayStatics::PlaySoundAtLocation(CachedWorld.Get(), nullptr, Location, 1.0f, 1.0f, 0.0f);
    }
}

void UMingCombatVisualizer::SpawnProjectileTrail(FVector Start, FVector End, float Speed)
{
    FMingCombatEffect Effect;
    Effect.EffectType = EMingCombatEffectType::ProjectileTrail;
    Effect.Location = Start;
    Effect.Direction = (End - Start).GetSafeNormal();
    Effect.Scale = 1.0f;
    Effect.Duration = FVector::Distance(Start, End) / Speed;
    Effect.Color = FLinearColor(0.8f, 0.8f, 0.9f);

    SpawnCombatEffect(Effect);
}

void UMingCombatVisualizer::SpawnImpactEffect(FVector Location, FVector Normal)
{
    FMingCombatEffect Effect;
    Effect.EffectType = EMingCombatEffectType::Impact;
    Effect.Location = Location;
    Effect.Direction = Normal;
    Effect.Scale = 1.0f;
    Effect.Duration = 0.5f;
    Effect.Color = FLinearColor(0.5f, 0.5f, 0.5f);

    SpawnCombatEffect(Effect);

    // Spawn decals, sparks, dust
    // This would spawn actual particle systems in a full implementation
}

void UMingCombatVisualizer::SpawnExplosion(FVector Location, float Radius, float Damage)
{
    FMingCombatEffect Effect;
    Effect.EffectType = EMingCombatEffectType::Explosion;
    Effect.Location = Location;
    Effect.Scale = Radius / 100.0f;
    Effect.Duration = 2.0f;
    Effect.Color = FLinearColor(1.0f, 0.3f, 0.0f);

    SpawnCombatEffect(Effect);

    // Screen shake for nearby players
    if (CachedWorld.IsValid())
    {
        APlayerController* PC = CachedWorld->GetFirstPlayerController();
        if (PC)
        {
            float ShakeIntensity = FMath::Clamp(Damage / 100.0f, 0.1f, 1.0f);
            PlayScreenShake(ShakeIntensity, 0.5f);
        }
    }

    // Play explosion sound
    if (CachedWorld.IsValid())
    {
        UGameplayStatics::PlaySoundAtLocation(CachedWorld.Get(), nullptr, Location, 2.0f, 1.0f, 0.0f);
    }
}

void UMingCombatVisualizer::SpawnHealingEffect(AActor* Target, float Amount)
{
    if (!Target)
    {
        return;
    }

    FMingCombatEffect Effect;
    Effect.EffectType = EMingCombatEffectType::Healing;
    Effect.Location = Target->GetActorLocation() + FVector(0, 0, 100);
    Effect.Scale = FMath::Clamp(Amount / 50.0f, 0.5f, 2.0f);
    Effect.Duration = 1.5f;
    Effect.Color = FLinearColor(0.2f, 1.0f, 0.2f);
    Effect.SourceActor = Target;
    Effect.bAttachToSource = true;

    SpawnCombatEffect(Effect);

    // Show healing number
    ShowHealingNumber(FMath::FloorToInt(Amount), Effect.Location);
}

void UMingCombatVisualizer::SpawnShieldEffect(AActor* Target, float Duration)
{
    if (!Target)
    {
        return;
    }

    FMingCombatEffect Effect;
    Effect.EffectType = EMingCombatEffectType::Shield;
    Effect.Location = Target->GetActorLocation();
    Effect.Scale = 1.5f;
    Effect.Duration = Duration;
    Effect.Color = FLinearColor(0.3f, 0.5f, 1.0f);
    Effect.SourceActor = Target;
    Effect.bAttachToSource = true;

    SpawnCombatEffect(Effect);
}

void UMingCombatVisualizer::ShowDamageNumber(int32 Damage, FVector Location, EMingDamageIndicatorType Type)
{
    if (!bEffectsEnabled || !CachedWorld.IsValid())
    {
        return;
    }

    // Determine color based on type
    FLinearColor Color;
    switch (Type)
    {
    case EMingDamageIndicatorType::Critical:
        Color = FLinearColor(1.0f, 0.2f, 0.2f); // Red
        break;
    case EMingDamageIndicatorType::Miss:
        Color = FLinearColor(0.7f, 0.7f, 0.7f); // Gray
        break;
    case EMingDamageIndicatorType::Blocked:
        Color = FLinearColor(0.5f, 0.5f, 0.5f); // Dark Gray
        break;
    case EMingDamageIndicatorType::Immune:
        Color = FLinearColor(1.0f, 1.0f, 0.0f); // Yellow
        break;
    default:
        Color = FLinearColor(1.0f, 1.0f, 1.0f); // White
        break;
    }

    FString Text = FString::Printf(TEXT("%d"), Damage);
    if (Type == EMingDamageIndicatorType::Critical)
    {
        Text = TEXT("!") + Text + TEXT("!");
    }
    else if (Type == EMingDamageIndicatorType::Miss)
    {
        Text = TEXT("MISS");
    }
    else if (Type == EMingDamageIndicatorType::Blocked)
    {
        Text = TEXT("BLOCKED");
    }

    // In a real implementation, this would spawn a 3D text widget
    UE_LOG(LogTemp, Verbose, TEXT("Damage: %s at location %s"), *Text, *Location.ToString());
}

void UMingCombatVisualizer::ShowHealingNumber(int32 Amount, FVector Location)
{
    if (!bEffectsEnabled)
    {
        return;
    }

    FString Text = FString::Printf(TEXT("+%d"), Amount);
    UE_LOG(LogTemp, Verbose, TEXT("Healing: %s"), *Text);
}

void UMingCombatVisualizer::ShowMissIndicator(FVector Location)
{
    ShowDamageNumber(0, Location, EMingDamageIndicatorType::Miss);
}

void UMingCombatVisualizer::ShowCriticalHitIndicator(FVector Location)
{
    // Spawn special critical hit effect
    FMingCombatEffect Effect;
    Effect.EffectType = EMingCombatEffectType::Impact;
    Effect.Location = Location;
    Effect.Scale = 1.5f;
    Effect.Duration = 0.3f;
    Effect.Color = FLinearColor(1.0f, 0.0f, 0.0f);

    SpawnCombatEffect(Effect);
}

void UMingCombatVisualizer::PlayScreenShake(float Intensity, float Duration)
{
    if (!CachedWorld.IsValid())
    {
        return;
    }

    APlayerController* PC = CachedWorld->GetFirstPlayerController();
    if (PC)
    {
        // This would play actual camera shake
        UE_LOG(LogTemp, Verbose, TEXT("Screen shake: Intensity=%.2f, Duration=%.2f"), Intensity, Duration);
    }
}

void UMingCombatVisualizer::PlayHitBlur(float Duration)
{
    UE_LOG(LogTemp, Verbose, TEXT("Hit blur: Duration=%.2f"), Duration);
}

void UMingCombatVisualizer::FlashScreenColor(FLinearColor Color, float Duration)
{
    UE_LOG(LogTemp, Verbose, TEXT("Screen flash: Color=%s, Duration=%.2f"), *Color.ToString(), Duration);
}

void UMingCombatVisualizer::PlayAttackAnimation(AActor* Unit, FVector Target)
{
    if (!Unit)
    {
        return;
    }

    // Get rotation to target
    FRotator TargetRotation = CalculateRotationToTarget(Unit->GetActorLocation(), Target);

    // Play attack animation through the unit's anim instance
    // This would call PlayAnimMontage or similar
    UE_LOG(LogTemp, Verbose, TEXT("Attack animation: Unit=%s, Target=%s"), 
           *Unit->GetName(), *Target.ToString());
}

void UMingCombatVisualizer::PlayHitReaction(AActor* Unit, FVector HitDirection)
{
    if (!Unit)
    {
        return;
    }

    // Play hit reaction animation
    UE_LOG(LogTemp, Verbose, TEXT("Hit reaction: Unit=%s"), *Unit->GetName());

    // Spawn blood/decal effect at hit location
    FVector HitLocation = Unit->GetActorLocation() + HitDirection * 50.0f;
    SpawnImpactEffect(HitLocation, -HitDirection);
}

void UMingCombatVisualizer::PlayDeathAnimation(AActor* Unit)
{
    if (!Unit)
    {
        return;
    }

    // Play death animation
    UE_LOG(LogTemp, Log, TEXT("Death animation: Unit=%s"), *Unit->GetName());

    // Spawn death effect (explosion of particles, ragdoll, etc.)
    FMingCombatEffect Effect;
    Effect.EffectType = EMingCombatEffectType::Explosion;
    Effect.Location = Unit->GetActorLocation();
    Effect.Scale = 1.0f;
    Effect.Duration = 3.0f;
    Effect.Color = FLinearColor(0.1f, 0.1f, 0.1f);

    SpawnCombatEffect(Effect);
}

void UMingCombatVisualizer::PlayVictoryAnimation(AActor* Unit)
{
    if (!Unit)
    {
        return;
    }

    UE_LOG(LogTemp, Verbose, TEXT("Victory animation: Unit=%s"), *Unit->GetName());
}

void UMingCombatVisualizer::PlayReloadAnimation(AActor* Unit)
{
    if (!Unit)
    {
        return;
    }

    UE_LOG(LogTemp, Verbose, TEXT("Reload animation: Unit=%s"), *Unit->GetName());
}

void UMingCombatVisualizer::ShowAttackTargetIndicator(AActor* Attacker, AActor* Target)
{
    if (!Attacker || !Target)
    {
        return;
    }

    // Draw line or arrow from attacker to target
    FVector Start = Attacker->GetActorLocation();
    FVector End = Target->GetActorLocation();

    UE_LOG(LogTemp, Verbose, TEXT("Attack indicator: %s -> %s"), 
           *Attacker->GetName(), *Target->GetName());
}

void UMingCombatVisualizer::ShowMovementDestinationIndicator(FVector Location)
{
    // Spawn destination marker
    UE_LOG(LogTemp, Verbose, TEXT("Movement destination: %s"), *Location.ToString());
}

void UMingCombatVisualizer::ShowAreaEffectIndicator(FVector Center, float Radius, float Duration)
{
    // Spawn circular area indicator
    UE_LOG(LogTemp, Verbose, TEXT("Area effect: Center=%s, Radius=%.1f, Duration=%.1f"),
           *Center.ToString(), Radius, Duration);
}

void UMingCombatVisualizer::EnableBatchMode(bool bEnable)
{
    if (bBatchModeEnabled && !bEnable)
    {
        FlushEffects();
    }
    bBatchModeEnabled = bEnable;
}

void UMingCombatVisualizer::FlushEffects()
{
    if (PendingEffects.Num() == 0)
    {
        return;
    }

    // Sort by priority/type
    PendingEffects.Sort([](const FMingCombatEffect& A, const FMingCombatEffect& B)
    {
        return (int32)A.EffectType < (int32)B.EffectType;
    });

    // Spawn up to MaxEffectsPerFrame
    int32 CountToSpawn = FMath::Min(PendingEffects.Num(), MaxEffectsPerFrame);
    for (int32 i = 0; i < CountToSpawn; ++i)
    {
        SpawnEffectInternal(PendingEffects[i]);
    }

    // Clear processed effects
    PendingEffects.RemoveAt(0, CountToSpawn);
}

void UMingCombatVisualizer::SetMaxEffectsPerFrame(int32 MaxCount)
{
    MaxEffectsPerFrame = FMath::Max(1, MaxCount);
}

void UMingCombatVisualizer::ClearAllEffects()
{
    PendingEffects.Empty();
    EffectPools.Empty();
}

void UMingCombatVisualizer::ProcessPendingEffects()
{
    if (bBatchModeEnabled)
    {
        FlushEffects();
    }
}

void UMingCombatVisualizer::SpawnEffectInternal(const FMingCombatEffect& Effect)
{
    if (!CachedWorld.IsValid())
    {
        return;
    }

    // In a real implementation, this would:
    // 1. Get or spawn particle system
    // 2. Set location, rotation, scale
    // 3. Set color, duration parameters
    // 4. Attach to actor if needed
    // 5. Activate

    UE_LOG(LogTemp, Verbose, TEXT("Spawned effect: Type=%d at %s"), 
           (int32)Effect.EffectType, *Effect.Location.ToString());
}

UFXSystemComponent* UMingCombatVisualizer::GetEffectFromPool(EMingCombatEffectType Type)
{
    if (EffectPools.Contains(Type))
    {
        auto& Pool = EffectPools[Type];
        for (auto& Effect : Pool)
        {
            if (Effect.IsValid())
            {
                return Effect.Get();
            }
        }
    }
    return nullptr;
}

void UMingCombatVisualizer::ReturnEffectToPool(EMingCombatEffectType Type, UFXSystemComponent* Effect)
{
    if (!EffectPools.Contains(Type))
    {
        EffectPools.Add(Type, TArray<TWeakObjectPtr<UFXSystemComponent>>());
    }
    EffectPools[Type].Add(Effect);
}

FVector UMingCombatVisualizer::GetRandomOffsetInRadius(float Radius)
{
    float Angle = FMath::RandRange(0.0f, 2.0f * PI);
    float Distance = FMath::RandRange(0.0f, Radius);
    return FVector(FMath::Cos(Angle) * Distance, FMath::Sin(Angle) * Distance, 0.0f);
}

FRotator UMingCombatVisualizer::CalculateRotationToTarget(FVector Source, FVector Target)
{
    FVector Direction = (Target - Source).GetSafeNormal();
    return Direction.Rotation();
}

FLinearColor UMingCombatVisualizer::GetEffectColor(EMingCombatEffectType Type)
{
    switch (Type)
    {
    case EMingCombatEffectType::MuzzleFlash:
        return FLinearColor(1.0f, 0.8f, 0.2f);
    case EMingCombatEffectType::ProjectileTrail:
        return FLinearColor(0.8f, 0.8f, 0.9f);
    case EMingCombatEffectType::Impact:
        return FLinearColor(0.5f, 0.5f, 0.5f);
    case EMingCombatEffectType::Blood:
        return FLinearColor(0.8f, 0.1f, 0.1f);
    case EMingCombatEffectType::Explosion:
        return FLinearColor(1.0f, 0.3f, 0.0f);
    case EMingCombatEffectType::Shield:
        return FLinearColor(0.3f, 0.5f, 1.0f);
    case EMingCombatEffectType::Healing:
        return FLinearColor(0.2f, 1.0f, 0.2f);
    case EMingCombatEffectType::Buff:
        return FLinearColor(1.0f, 0.8f, 0.0f);
    case EMingCombatEffectType::Debuff:
        return FLinearColor(0.5f, 0.0f, 0.5f);
    default:
        return FLinearColor::White;
    }
}

float UMingCombatVisualizer::GetEffectDuration(EMingCombatEffectType Type)
{
    switch (Type)
    {
    case EMingCombatEffectType::MuzzleFlash:
        return 0.1f;
    case EMingCombatEffectType::Impact:
        return 0.5f;
    case EMingCombatEffectType::Blood:
        return 2.0f;
    case EMingCombatEffectType::Explosion:
        return 3.0f;
    case EMingCombatEffectType::Shield:
        return 5.0f;
    case EMingCombatEffectType::Healing:
        return 1.5f;
    default:
        return 1.0f;
    }
}

void UMingCombatVisualizer::SetEffectsEnabled(bool bEnabled)
{
    bEffectsEnabled = bEnabled;
    if (!bEnabled)
    {
        ClearAllEffects();
    }
}

bool UMingCombatVisualizer::AreEffectsEnabled() const
{
    return bEffectsEnabled;
}
