#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" // Changed to ActorComponent
#include "IdeaManager.generated.h"

// Forward declaration to avoid circular dependencies
class UModifierManager;

USTRUCT(BlueprintType)
struct FIdea
{
    GENERATED_BODY()

    FIdea() : Cost(0) {} // Added default constructor
// ... rest of the struct ...

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName IdeaID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Cost;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString Description;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<FName, float> Modifiers;
};

USTRUCT(BlueprintType)
struct FIdeaGroup
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName GroupID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString GroupName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FIdea> Ideas;
};

// A delegate that will be broadcasted when an idea is successfully unlocked
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIdeaUnlocked, FName, IdeaID);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGWARRTS_API UIdeaManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UIdeaManager();

    // All available idea groups loaded from JSON
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ideas")
    TArray<FIdeaGroup> AllIdeaGroups;

    // The idea group chosen by the player
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ideas")
    FIdeaGroup PlayerChosenGroup;

    // The IDs of ideas the player has unlocked
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ideas")
    TSet<FName> UnlockedIdeas;

    // The delegate that other systems (like TriggerManager) can subscribe to
    UPROPERTY(BlueprintAssignable, Category = "Ideas")
    FOnIdeaUnlocked OnIdeaUnlockedEvent;

    // Function to load all idea groups from the JSON file
    UFUNCTION(BlueprintCallable, Category = "Ideas")
    void LoadAllIdeaGroups();

    // Function for the player to choose an idea group
    UFUNCTION(BlueprintCallable, Category = "Ideas")
    void ChooseIdeaGroup(FName GroupID);

    // Function for the player to unlock an idea
    UFUNCTION(BlueprintCallable, Category = "Ideas")
    bool UnlockIdea(FName IdeaID);
};
