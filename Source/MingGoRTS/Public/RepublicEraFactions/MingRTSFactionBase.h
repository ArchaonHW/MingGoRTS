// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Republic Era Factions - Base Faction Class

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Factions/MingRTSFactionTypes.h"
#include "MingRTSFactionBase.generated.h"

class AMingRTSUnit;
class UMingRTSPlayerController;

/**
 * Base class for all Republic Era factions
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class MINGRTS_API UMingRTSFactionBase : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSFactionBase();
    
    /** Initialize the faction */
    UFUNCTION(BlueprintCallable, Category = "Faction")
    virtual void InitializeFaction();
    
    /** Called when the faction is selected by a player */
    UFUNCTION(BlueprintCallable, Category = "Faction")
    virtual void OnFactionSelected(UMingRTSPlayerController* PlayerController);
    
    /** Called at the start of the game */
    UFUNCTION(BlueprintCallable, Category = "Faction")
    virtual void OnGameStart();
    
    /** Get faction attributes */
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual FFactionAttributes GetFactionAttributes() const { return Attributes; }
    
    /** Get faction unique units */
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual TArray<FFactionUnit> GetUniqueUnits() const { return UniqueUnits; }
    
    /** Get faction unique mechanics */
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual TArray<FFactionMechanic> GetUniqueMechanics() const { return UniqueMechanics; }
    
    /** Get gameplay guide for this faction */
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual FGameplayGuide GetGameplayGuide() const;
    
    /** Check if this faction can use a specific unit type */
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual bool CanUseUnitType(EFactionUnitType UnitType) const { return true; }
    
    /** Apply faction-specific modifiers to unit stats */
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual FFactionUnitStats ModifyUnitStats(const FFactionUnitStats& BaseStats, EFactionUnitType UnitType) const;
    
    /** Get faction-specific starting bonuses */
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual TMap<FName, float> GetStartingBonuses() const;
    
    /** Apply unique mechanic effects */
    UFUNCTION(BlueprintCallable, Category = "Faction")
    virtual void ApplyMechanicEffects(const FName& MechanicID);
    
    /** Check if a mechanic is active */
    UFUNCTION(BlueprintPure, Category = "Faction")
    virtual bool IsMechanicActive(const FName& MechanicID) const;
    
    /** Event: When a unique unit is produced */
    UPROPERTY(BlueprintAssignable, Category = "Faction|Events")
    FOnUniqueUnitProduced OnUniqueUnitProduced;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUniqueUnitProduced, 
        const FFactionUnit&, ProducedUnit);
    
    /** Event: When a mechanic is activated */
    UPROPERTY(BlueprintAssignable, Category = "Faction|Events")
    FOnMechanicActivated OnMechanicActivated;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMechanicActivated,
        const FName&, MechanicID);

protected:
    /** Faction attributes */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction Data")
    FFactionAttributes Attributes;
    
    /** Unique units for this faction */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction Data")
    TArray<FFactionUnit> UniqueUnits;
    
    /** Unique mechanics for this faction */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction Data")
    TArray<FFactionMechanic> UniqueMechanics;
    
    /** Gameplay guide */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction Data")
    FGameplayGuide GameplayGuide;
    
    /** Active mechanic states */
    UPROPERTY()
    TMap<FName, bool> ActiveMechanics;
    
    /** Owning player controller */
    UPROPERTY()
    TObjectPtr<UMingRTSPlayerController> OwnerController;
    
    /** Initialize default attributes - override in subclasses */
    virtual void InitializeAttributes();
    
    /** Initialize unique units - override in subclasses */
    virtual void InitializeUniqueUnits();
    
    /** Initialize unique mechanics - override in subclasses */
    virtual void InitializeUniqueMechanics();
    
    /** Initialize gameplay guide - override in subclasses */
    virtual void InitializeGameplayGuide();
    
    /** Helper: Create a basic unit definition */
    FFactionUnit CreateUnitDefinition(
        const FName& UnitID,
        const FText& UnitName,
        const FText& Description,
        EFactionUnitType UnitType,
        const FFactionUnitStats& Stats) const;
    
    /** Helper: Create a mechanic definition */
    FFactionMechanic CreateMechanicDefinition(
        const FName& MechanicID,
        const FText& MechanicName,
        const FText& Description,
        EFactionMechanicType MechanicType,
        float EffectMultiplier = 1.0f) const;
};
