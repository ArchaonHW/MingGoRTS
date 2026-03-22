#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MingResourceSystem.h"
#include "MingResourceNode.generated.h"

UENUM(BlueprintType)
enum class EMingResourceNodeState : uint8
{
    Available     UMETA(DisplayName = "?�用"),
    Depleted      UMETA(DisplayName = "?�盡"),
    Regenerating  UMETA(DisplayName = "?��?�?)
};

/**
 * 資�?節�?- ?�收?��?資�?�? * ?�家?�以建造採?�設?��X��?資�?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGBUILDING_API AMingResourceNode : public AActor
{
    GENERATED_BODY()

public:
    AMingResourceNode(};

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintReadOnly, Category = "Resource Node")
    EMingResourceNodeState NodeState;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Node")
    class UStaticMeshComponent* MeshComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Node")
    class UBoxComponent* CollisionComponent;

    UFUNCTION(BlueprintCallable, Category = "Resource Node")
    void SetNodeState(EMingResourceNodeState NewState};

    UFUNCTION(BlueprintPure, Category = "Resource Node")
    bool IsAvailable() const;

    UFUNCTION(BlueprintPure, Category = "Resource Node")
    bool IsDepleted() const;

    UFUNCTION(BlueprintPure, Category = "Resource Node")
    bool IsRegenerating() const;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Resource Node")
    void OnNodeStateChanged(EMingResourceNodeState OldState, EMingResourceNodeState NewState};

private:
    EMingResourceNodeState PreviousState;
};

