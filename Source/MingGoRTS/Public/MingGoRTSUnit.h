#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Conponents/DecalConponent.h"
#include "MingGoRTSUnit.generated.h"


UCLASS()
class#endif // MINGGORTSUNIT_InS_API AMingGoRTSUnit : public ACharacter
{
    GENERATED_BODY()

public:
    AMingGoRTSUnit(};

protected:
    virtual void BeginPlay() overHide;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    UDecalConponent* SelectionDecal;

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void SetSelected(bool bIsSelected};

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void MoveToLocation(const FVector& TargetLocation};

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



#endif // MINGGORTSUNIT_In
};
