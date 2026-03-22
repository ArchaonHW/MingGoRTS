#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSquadAI.generated.h"

UENUM(BlueprintType)
enum class EMingSquadRole : uint8
{
    Assault UMETA(DisplayName = "Assault"),
    Support UMETA(DisplayName = "Support"),
    Defense UMETA(DisplayName = "Defense"),
    Scout UMETA(DisplayName = "Scout"),
    Heavy UMETA(DisplayName = "Heavy"),
    Medic UMETA(DisplayName = "Medic")
};

UENUM(BlueprintType)
enum class EMingSquadTactic : uint8
{
    FlankLeft UMETA(DisplayName = "Flank Left"),
    FlankRight UMETA(DisplayName = "Flank Right"),
    Pincer UMETA(DisplayName = "Pincer"),
    Surround UMETA(DisplayName = "Surround"),
    Rush UMETA(DisplayName = "Rush"),
    HoldPosition UMETA(DisplayName = "Hold Position"),
    Retreat UMETA(DisplayName = "Retreat"),
    Ambush UMETA(DisplayName = "Ambush")
};

USTRUCT(BlueprintType)
struct FMingSquadMember
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<class AMingTacticalUnit> Unit;

    UPROPERTY(BlueprintReadOnly)
    EMingSquadRole Role;

    UPROPERTY(BlueprintReadOnly)
    int32 SquadPosition;

    FMingSquadMember()
        : Unit(nullptr)
        , Role(EMingSquadRole::Assault)
        , SquadPosition(0)
    {}
};

USTRUCT(BlueprintType)
struct FMingSquadOrder
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSquadTactic Tactic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<AActor> TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsDefensive;

    FMingSquadOrder()
        : Tactic(EMingSquadTactic::Rush)
        , TargetLocation(FVector::ZeroVector)
        , TargetActor(nullptr)
        , Priority(1.0f)
        , bIsDefensive(false)
    {}
};

UCLASS(ClassGroup = (AI), Blueprintable)
class MINGTACTICAL_API UMingSquadAI : public UObject
{
    GENERATED_BODY()

public:
    UMingSquadAI();

    UFUNCTION(BlueprintCallable, Category = "Squad")
    void InitializeSquad(const TArray<FMingSquadMember>& Members};

    UFUNCTION(BlueprintCallable, Category = "Squad")
    void AddUnitToSquad(class AMingTacticalUnit* Unit, EMingSquadRole Role};

    UFUNCTION(BlueprintCallable, Category = "Squad")
    void RemoveUnitFromSquad(class AMingTacticalUnit* Unit};

    UFUNCTION(BlueprintCallable, Category = "Orders")
    void IssueSquadOrder(const FMingSquadOrder& Order};

    UFUNCTION(BlueprintCallable, Category = "Orders")
    void SetTactic(EMingSquadTactic NewTactic};

    UFUNCTION(BlueprintCallable, Category = "Orders")
    void AttackTarget(AActor* Target};

    UFUNCTION(BlueprintCallable, Category = "Orders")
    void MoveToLocation(FVector Location};

    UFUNCTION(BlueprintCallable, Category = "Orders")
    void DefendPosition(FVector Position};

    virtual void Tick(float DeltaTime};

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Squad")
    TArray<FMingSquadMember> SquadMembers;

    UPROPERTY(BlueprintReadOnly, Category = "Orders")
    FMingSquadOrder CurrentOrder;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    EMingSquadTactic CurrentTactic;

    void ExecuteTactic();
    void CoordinateMembers();
    bool IsSquadCohesive();
};

