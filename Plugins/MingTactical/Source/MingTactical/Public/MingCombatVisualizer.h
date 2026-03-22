#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCombatVisualizer.generated.h"

UENUM(BlueprintType)
enum class EMingCombatEffectType : uint8
{
    MuzzleFlash UMETA(DisplayName = "Muzzle Flash"),
    ProjectileTrail UMETA(DisplayName = "Projectile Trail"),
    Impact UMETA(DisplayName = "Impact"),
    Blood UMETA(DisplayName = "Blood"),
    Explosion UMETA(DisplayName = "Explosion"),
    Shield UMETA(DisplayName = "Shield"),
    Healing UMETA(DisplayName = "Healing"),
    Buff UMETA(DisplayName = "Buff"),
    Debuff UMETA(DisplayName = "Debuff")
};

UENUM(BlueprintType)
enum class EMingDamageIndicatorType : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Critical UMETA(DisplayName = "Critical"),
    Miss UMETA(DisplayName = "Miss"),
    Blocked UMETA(DisplayName = "Blocked"),
    Immune UMETA(DisplayName = "Immune")
};

USTRUCT(BlueprintType)
struct FMingEffectPoolEntry
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<TWeakObjectPtr<class UFXSystemComponent>> Effects;
};

USTRUCT(BlueprintType)
struct FMingCombatEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingCombatEffectType EffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Direction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Scale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor Color;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UFXSystemComponent> ParticleSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundBase* Sound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAttachToSource;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TWeakObjectPtr<class AActor> SourceActor;

    FMingCombatEffect()
        : EffectType(EMingCombatEffectType::Impact)
        , Location(FVector::ZeroVector)
        , Direction(FVector::ForwardVector)
        , Scale(1.0f)
        , Duration(1.0f)
        , Color(FLinearColor::White)
        , bAttachToSource(false)
    {}
};

/**
 * Combat Visualizer for MingGoRTS
 * Manages combat animations, effects, and visual feedback
 */
UCLASS(ClassGroup = (VisualEffects), Blueprintable)
class MINGTACTICAL_API UMingCombatVisualizer : public UObject
{
    GENERATED_BODY()

public:
    UMingCombatVisualizer();

    // Singleton access
    UFUNCTION(BlueprintPure, Category = "Combat Visualizer", meta = (WorldContext = "WorldContextObject"))
    static UMingCombatVisualizer* Get(UObject* WorldContextObject};

    // Effect Spawning
    UFUNCTION(BlueprintCallable, Category = "Effects")
    void SpawnCombatEffect(const FMingCombatEffect& Effect};

    UFUNCTION(BlueprintCallable, Category = "Effects")
    void SpawnMuzzleFlash(AActor* Source, FVector Location, FVector Direction};

    UFUNCTION(BlueprintCallable, Category = "Effects")
    void SpawnProjectileTrail(FVector Start, FVector End, float Speed};

    UFUNCTION(BlueprintCallable, Category = "Effects")
    void SpawnImpactEffect(FVector Location, FVector Normal};

    UFUNCTION(BlueprintCallable, Category = "Effects")
    void SpawnExplosion(FVector Location, float Radius, float Damage};

    UFUNCTION(BlueprintCallable, Category = "Effects")
    void SpawnHealingEffect(AActor* Target, float Amount};

    UFUNCTION(BlueprintCallable, Category = "Effects")
    void SpawnShieldEffect(AActor* Target, float Duration};

    // Damage Indicators
    UFUNCTION(BlueprintCallable, Category = "UI Feedback")
    void ShowDamageNumber(int32 Damage, FVector Location, EMingDamageIndicatorType Type};

    UFUNCTION(BlueprintCallable, Category = "UI Feedback")
    void ShowHealingNumber(int32 Amount, FVector Location};

    UFUNCTION(BlueprintCallable, Category = "UI Feedback")
    void ShowMissIndicator(FVector Location};

    UFUNCTION(BlueprintCallable, Category = "UI Feedback")
    void ShowCriticalHitIndicator(FVector Location};

    // Screen Effects
    UFUNCTION(BlueprintCallable, Category = "Screen Effects")
    void PlayScreenShake(float Intensity, float Duration};

    UFUNCTION(BlueprintCallable, Category = "Screen Effects")
    void PlayHitBlur(float Duration};

    UFUNCTION(BlueprintCallable, Category = "Screen Effects")
    void FlashScreenColor(FLinearColor Color, float Duration};

    // Unit Animations
    UFUNCTION(BlueprintCallable, Category = "Animations")
    void PlayAttackAnimation(AActor* Unit, FVector Target};

    UFUNCTION(BlueprintCallable, Category = "Animations")
    void PlayHitReaction(AActor* Unit, FVector HitDirection};

    UFUNCTION(BlueprintCallable, Category = "Animations")
    void PlayDeathAnimation(AActor* Unit};

    UFUNCTION(BlueprintCallable, Category = "Animations")
    void PlayVictoryAnimation(AActor* Unit};

    UFUNCTION(BlueprintCallable, Category = "Animations")
    void PlayReloadAnimation(AActor* Unit};

    // Selection Feedback
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void ShowAttackTargetIndicator(AActor* Attacker, AActor* Target};

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void ShowMovementDestinationIndicator(FVector Location};

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void ShowAreaEffectIndicator(FVector Center, float Radius, float Duration};

    // Batch Processing
    UFUNCTION(BlueprintCallable, Category = "Batch")
    void EnableBatchMode(bool bEnable};

    UFUNCTION(BlueprintCallable, Category = "Batch")
    void FlushEffects();

    // Performance
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetMaxEffectsPerFrame(int32 MaxCount};

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void ClearAllEffects();

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FLinearColor GetEffectColor(EMingCombatEffectType Type};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static float GetEffectDuration(EMingCombatEffectType Type};

    UFUNCTION(BlueprintCallable, Category = "Utility")
    void SetEffectsEnabled(bool bEnabled};

    UFUNCTION(BlueprintPure, Category = "Utility")
    bool AreEffectsEnabled() const;

protected:
    UPROPERTY()
    TWeakObjectPtr<UWorld> CachedWorld;

    UPROPERTY()
    TArray<FMingCombatEffect> PendingEffects;

    UPROPERTY()
    bool bBatchModeEnabled;

    UPROPERTY()
    int32 MaxEffectsPerFrame;

    UPROPERTY()
    bool bEffectsEnabled;

    // Effect pools
    UPROPERTY()
    TMap<EMingCombatEffectType, FMingEffectPoolEntry> EffectPools;

    // Internal functions
    void ProcessPendingEffects();
    void SpawnEffectInternal(const FMingCombatEffect& Effect};
    UFXSystemComponent* GetEffectFromPool(EMingCombatEffectType Type};
    void ReturnEffectToPool(EMingCombatEffectType Type, UFXSystemComponent* Effect};

    // Helper functions
    FVector GetRandomOffsetInRadius(float Radius};
    FRotator CalculateRotationToTarget(FVector Source, FVector Target};
};

