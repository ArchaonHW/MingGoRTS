#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" // Changed to ActorComponent
#include "ModifierManager.generated.h"

// A delegate that will be broadcasted when a new modifier should be applied
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifierApplied, FName, ModifierID, float, ModifierValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGWARRTS_API UModifierManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UModifierManager();

    // A map to store all active modifiers in the game
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifiers")
    TMap<FName, float> ActiveModifiers;

    // The delegate that other systems can subscribe to
    UPROPERTY(BlueprintAssignable, Category = "Modifiers")
    FOnModifierApplied OnModifierApplied;

    // Function to add or update a modifier and broadcast the change
    UFUNCTION(BlueprintCallable, Category = "Modifiers")
    void ApplyModifier(FName ModifierID, float Value);
};
