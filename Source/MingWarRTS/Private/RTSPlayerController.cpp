#include "RTSPlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerInput.h" // For IsShiftKeyDown()
#include "Framework/Application/SlateApplication.h" // For GetInputModifierState()
#include "RTSUnit.h"
#include "Engine/HitResult.h"
#include "ModifierManager.h" // Include to instantiate it
#include "IdeaManager.h"
#include "NewEventManager.h"
#include "TriggerManager.h"

ARTSPlayerController::ARTSPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    bShowMouseCursor = true;
    bIsSelecting = false;

    // Create and initialize Components
    SupplyManagerComponent = CreateDefaultSubobject<USupplyManager>(TEXT("SupplyManagerComponent"));
    ModifierManagerComponent = CreateDefaultSubobject<UModifierManager>(TEXT("ModifierManagerComponent"));
    IdeaManagerComponent = CreateDefaultSubobject<UIdeaManager>(TEXT("IdeaManagerComponent"));
    EventManagerComponent = CreateDefaultSubobject<UNewEventManager>(TEXT("EventManagerComponent"));
    TriggerManagerComponent = CreateDefaultSubobject<UTriggerManager>(TEXT("TriggerManagerComponent"));
    ResourceManagerComponent = CreateDefaultSubobject<UResourceManager>(TEXT("ResourceManagerComponent"));
}

void ARTSPlayerController::BeginPlay()
{
    Super::BeginPlay();

    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);

    // Initialize systems that rely on loading data
    if (IdeaManagerComponent)
    {
        IdeaManagerComponent->LoadAllIdeaGroups();
    }
    if (EventManagerComponent)
    {
        // EventManagerComponent->LoadAllGameEvents();
    }
    
    // Initialize triggers AFTER the other managers are ready
    if (TriggerManagerComponent)
    {
        TriggerManagerComponent->InitializeTriggers(IdeaManagerComponent, EventManagerComponent);
    }
}

void ARTSPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsSelecting)
    {
        UpdateMarqueeSelection();
    }

    // Update supply levels (for now, just for selected units as a placeholder)
    if (SupplyManagerComponent && SelectedUnits.Num() > 0)
    {
        SupplyManagerComponent->UpdateSupplyLevels(SelectedUnits);
    }
}

void ARTSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Select", IE_Pressed, this, &ARTSPlayerController::OnSelectPressed);
    InputComponent->BindAction("Select", IE_Released, this, &ARTSPlayerController::OnSelectReleased);
    
    // 將會綁定給右鍵 (下一個階段會用到)
    InputComponent->BindAction("Command", IE_Pressed, this, &ARTSPlayerController::OnCommandPressed);

    // Bind Possess action
    InputComponent->BindAction("Possess", IE_Pressed, this, &ARTSPlayerController::PossessSelectedUnit);
}

void ARTSPlayerController::OnSelectPressed()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("C++: Left Mouse PRESSED!"));

    // [1] 開始框選
    StartMarqueeSelection();

    // [2] 單點選取
    FHitResult HitResult;
    bool bHit = GetHitResultUnderCursor(ECC_Pawn, false, HitResult);

    // Clear previous selection unless Shift is held
    if (!FSlateApplication::Get().GetModifierKeys().IsShiftDown())
    {
        ClearSelection();
    }

    if (bHit && HitResult.GetActor())
    {
        ARTSUnit* HitUnit = Cast<ARTSUnit>(HitResult.GetActor());
        if (HitUnit)
        {
            HitUnit->SetSelected(true);
            SelectedUnits.Add(HitUnit);
        }
    }
}

void ARTSPlayerController::OnSelectReleased()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("C++: Left Mouse RELEASED!"));

    // [1] 結束框選
    EndMarqueeSelection();
}

void ARTSPlayerController::StartMarqueeSelection()
{
    float MouseX, MouseY;
    if (GetMousePosition(MouseX, MouseY))
    {
        SelectionStartPos = FVector2D(MouseX, MouseY);
        SelectionEndPos = SelectionStartPos;
        bIsSelecting = true;
    }
}

void ARTSPlayerController::UpdateMarqueeSelection()
{
    float MouseX, MouseY;
    if (GetMousePosition(MouseX, MouseY))
    {
        SelectionEndPos = FVector2D(MouseX, MouseY);
    }
}

void ARTSPlayerController::EndMarqueeSelection()
{
    bIsSelecting = false;
    // 這裡通常會加上從 HUD 取得「被框選到」的單位的邏輯
    // 下一步我們會在藍圖 HUD 中實作
}

void ARTSPlayerController::OnCommandPressed()
{
    // 當玩家按下右鍵，我們會在這裡讓選取的單位走向滑鼠的位置
    // 準備進入 Phase 2
}

void ARTSPlayerController::ClearSelection()
{
    for (ARTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.Empty();
}

void ARTSPlayerController::PossessSelectedUnit()
{
    // We can only possess if exactly one unit is selected
    if (SelectedUnits.Num() == 1 && SelectedUnits[0] != nullptr)
    {
        ARTSUnit* UnitToPossess = SelectedUnits[0];
        
        // UnPossess the current Pawn (which is likely the RTS Camera Pawn)
        AController* CurrentController = GetPawn()->GetController();
        if(CurrentController)
        {
             // Before possessing, we need to decide what happens to the old pawn.
             // For an RTS controller, we'd typically want to return to it later.
             // For now, let's just possess the new unit.
        }

        Possess(UnitToPossess);

        // After possessing, you might want to change input mode, camera, etc.
        // This will be handled in the unit's BeginPlay or on being possessed.
        UE_LOG(LogTemp, Log, TEXT("Possessing unit: %s"), *UnitToPossess->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot possess unit. Select exactly one unit. Currently selected: %d"), SelectedUnits.Num());
    }
}