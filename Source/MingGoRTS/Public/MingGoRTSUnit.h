#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "MingGoRTSUnit.generated.h"

UCLASS()
class MINGGORTS_API AMingGoRTSUnit : public ACharacter
{
    GENERATED_BODY()

public:
    AMingGoRTSUnit();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    UDecalComponent* SelectionDecal;

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void SetSelected(bool bIsSelected);

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void MoveToLocation(const FVector& TargetLocation);

    UFUNCTION(BlueprintPure, Category = "Unit")
    bool IsSelected() const { return bIsSelected; }

protected:
    UPROPERTY()
    bool bIsSelected;

    UPROPERTY()
    FVector TargetMoveLocation;

    UPROPERTY()
    bool bIsMoving;
};



#endif // MINGGORTSUNIT_H
