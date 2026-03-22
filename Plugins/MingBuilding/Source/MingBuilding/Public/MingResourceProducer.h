#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingResourceSystem.h"
#include "MingResourceProducer.generated.h"

UCLASS(ClassGroup = (Gameplay), meta = (BlueprintSpawnableComponent))
class MINGBUILDING_API UMingResourceProducer : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingResourceProducer();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    EMingResourceType ProducedResource;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    float ProductionRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    float ProductionInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    bool bAutoStartProduction;

    UPROPERTY(BlueprintReadOnly, Category = "Production")
    bool bIsProducing;

    UPROPERTY(BlueprintReadOnly, Category = "Production")
    float TimeSinceLastProduction;

    UPROPERTY(BlueprintReadOnly, Category = "Production")
    int32 TotalProduced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
    TArray<FMingResourceCost> OperatingCosts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
    bool bConsumeResourcesToOperate;

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Production")
    void StartProduction();

    UFUNCTION(BlueprintCallable, Category = "Production")
    void StopProduction();

    UFUNCTION(BlueprintCallable, Category = "Production")
    void PauseProduction();

    UFUNCTION(BlueprintCallable, Category = "Production")
    void ResumeProduction();

    UFUNCTION(BlueprintCallable, Category = "Production")
    void SetProductionRate(float NewRate};

    UFUNCTION(BlueprintPure, Category = "Production")
    bool IsOperational() const;

    UFUNCTION(BlueprintPure, Category = "Production")
    float GetProductionProgress() const;

    UFUNCTION(BlueprintCallable, Category = "Production")
    void UpdateProduction(float DeltaTime};

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionTick, EMingResourceType, ResourceType, int32, Amount};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProductionStarted};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProductionStopped};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInsufficientResources};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnProductionTick OnProductionTick;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnProductionStarted OnProductionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnProductionStopped OnProductionStopped;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnInsufficientResources OnInsufficientResources;

protected:
    UPROPERTY()
    TWeakObjectPtr<class UMingResourceSystem> ResourceSystem;

    void ProduceResource();
    bool CheckOperatingCosts();
    void ConsumeOperatingCosts();

    UFUNCTION()
    void OnResourceSystemAvailable();
};

