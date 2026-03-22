#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MingResourceSystem.h"
#include "MingResourceNode.generated.h"

UENUM(BlueprintType)
enum class EMingResourceNodeState: uint8 {
    Available     UMETA(DisplayName = "??"),
    Depleted      UMETA(DisplayName = "??"),
    Regenerating  UMETA(DisplayName = "摧毀??)
};

/**
 * ??`??- 摧毀摧毀?? * ??a??Hسy??]目標數量??
 */
UCLASS(BlueprintType, Blueprintable)
class MINGBUILDING_API AMingResourceNode : public AActor
{
    GENERATED_BODY()

public:
    AMingResourceNode();

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
    void SetNodeState(EMingResourceNodeState NewState);

    UFUNCTION(BlueprintPure, Category = "Resource Node")
    bool IsAvailable() const;

    UFUNCTION(BlueprintPure, Category = "Resource Node")
    bool IsDepleted() const;

    UFUNCTION(BlueprintPure, Category = "Resource Node")
    bool IsRegenerating() const;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Resource Node")
    void OnNodeStateChanged(EMingResourceNodeState OldState, EMingResourceNodeState NewState);

private:
    EMingResourceNodeState PreviousState;
);

