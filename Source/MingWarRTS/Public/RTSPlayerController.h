#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NewEventManager.h"
#include "RTSPlayerController.generated.h"

class USupplyManager;
class UModifierManager;
class UIdeaManager;
class UTriggerManager;
class UCombatManager;
class UBuildingManager;
class UResourceManager;
class ARTSUnit;

UCLASS()
class MINGWARRTS_API ARTSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ARTSPlayerController();
    void PossessSelectedUnit();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    // Component properties
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RTS Components")
    USupplyManager* SupplyManagerComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RTS Components")
    UModifierManager* ModifierManagerComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RTS Components")
    UIdeaManager* IdeaManagerComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RTS Components")
    UNewEventManager* EventManagerComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RTS Components")
    UTriggerManager* TriggerManagerComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RTS Components")
    UCombatManager* CombatManagerComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RTS Components")
    UBuildingManager* BuildingManagerComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RTS Components")
    UResourceManager* ResourceManagerComponent;

private:
    // Selection Logic
    void OnSelectPressed();
    void OnSelectReleased();
    void OnCommandPressed();

    void StartMarqueeSelection();
    void UpdateMarqueeSelection();
    void EndMarqueeSelection();
    void ClearSelection();

    bool bIsSelecting;
    FVector2D SelectionStartPos;
    FVector2D SelectionEndPos;
    
    TArray<ARTSUnit*> SelectedUnits;
};
