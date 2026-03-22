#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MingResourceSystem.h"
#include "MingResourceNode.generated.h"

UENUM(BlueprintType)
enum class EMingResourceNodeState : uint8
{
    Available     UMETA(DisplayName = "?ØÊé°??),
    Depleted      UMETA(DisplayName = "Â∑≤ÊûØÁ´?),
    Regenerating  UMETA(DisplayName = "?çÁ?‰∏?)
};

/**
 * Ë≥áÊ?ÁØÄÈª?- ?∞Â?‰∏äÁ??ØÊé°?ÜË?Ê∫? * Â¶ÇÔ?Á≥ßÈ??Ü„ÄÅÁ§¶?¥„ÄÅÁ??ôÊ°∂Á≠? */
UCLASS(BlueprintType, Blueprintable)
class MINGBUILDING_API AMingResourceNode : public AActor
{
    GENERATED_BODY()

public:
    AMingResourceNode();

    // Ë≥áÊ?Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    EMingResourceType ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    int32 TotalAmount;

    UPROPERTY(BlueprintReadOnly, Category = "Resource")
    int32 CurrentAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    int32 GatherAmountPerTrip; // ÊØèÊ¨°?°È???
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    float GatherTime; // ?°È??Ä?Ä?ÇÈ?ÔºàÁ?Ôº?
    // ?çÁ?Ë®≠ÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
    bool bCanRegenerate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
    float RegenerationRate; // ÊØèÁ??çÁ???
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
    float RegenerationInterval; // ?çÁ??ìÈ?

    // ?Ä??    UPROPERTY(BlueprintReadOnly, Category = "State")
    EMingResourceNodeState NodeState;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    int32 CurrentGathererCount; // ?∂Â??°È??ÖÊï∏??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    int32 MaxGathererCount; // ?ÄÂ§ßÂ??ÇÊé°?ÜËÄÖÊï∏??
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // ?°È??•Âè£
    UFUNCTION(BlueprintCallable, Category = "Gathering")
    bool CanBeGathered() const;

    UFUNCTION(BlueprintCallable, Category = "Gathering")
    int32 GatherResource(int32 RequestedAmount);

    UFUNCTION(BlueprintCallable, Category = "Gathering")
    void RegisterGatherer();

    UFUNCTION(BlueprintCallable, Category = "Gathering")
    void UnregisterGatherer();

    UFUNCTION(BlueprintCallable, Category = "Gathering")
    float GetGatherProgress() const;

    // ?çÁ?
    UFUNCTION(BlueprintCallable, Category = "Regeneration")
    void StartRegeneration();

    UFUNCTION(BlueprintCallable, Category = "Regeneration")
    void StopRegeneration();

    // Ë¶ñË¶∫?çÈ?
    UFUNCTION(BlueprintCallable, Category = "Visual")
    void UpdateVisualState();

    UFUNCTION(BlueprintCallable, Category = "Visual")
    void SetHighlighted(bool bHighlighted);

    // ‰∫ã‰ª∂
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceGathered, EMingResourceType, ResourceType, int32, Amount);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceDepleted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceRegenerated, int32, NewAmount);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResourceGathered OnResourceGathered;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResourceDepleted OnResourceDepleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResourceRegenerated OnResourceRegenerated;

    // Â∑•ÂÖ∑?ΩÊï∏
    UFUNCTION(BlueprintPure, Category = "Utility")
    FLinearColor GetResourceColor() const;

    UFUNCTION(BlueprintPure, Category = "Utility")
    FString GetResourceName() const;

    UFUNCTION(BlueprintPure, Category = "Utility")
    float GetResourcePercentage() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UStaticMeshComponent> ResourceMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UBillboardComponent> ResourceIcon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UTextRenderComponent> AmountText;

    // ?çÁ?Ë®àÊ?
    float TimeSinceLastRegeneration;

    void ProcessRegeneration(float DeltaTime);
    void UpdateAmountDisplay();
    void DepleteNode();

    UFUNCTION()
    void OnGatheringComplete();
};
