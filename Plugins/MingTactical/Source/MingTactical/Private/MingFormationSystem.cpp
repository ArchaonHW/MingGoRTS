#include "MingFormationSystem.h"
#include "MingTacticalUnit.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UMingFormationSystem::UMingFormationSystem()
{
}

void UMingFormationSystem::InitializeFormation(EMingFormationType Type, FVector Center, FVector Direction, int32 UnitCount)
{
    FormationData.FormationType = Type;
    FormationData.CenterPosition = Center;
    FormationData.FormationDirection = Direction.GetSafeNormal();
    FormationData.MaxUnits = UnitCount;
    FormationData.UnitSpacing = 100.0f;

    GenerateFormationSlots();

    UE_LOG(LogTemp, Log, TEXT("Formation initialized: Type=%d, Units=%d, Center=%s"),
           (int32)Type, UnitCount, *Center.ToString());
}

void UMingFormationSystem::UpdateFormationCenter(FVector NewCenter)
{
    FormationData.CenterPosition = NewCenter;
    UpdateSlotPositions();

    // Update all assigned units
    for (const auto& Pair : UnitSlotMap)
    {
        if (Pair.Value.IsValid())
        {
            AMingTacticalUnit* Unit = Pair.Value.Get();
            FVector TargetPosition = GetSlotPosition(Pair.Key);
            
            // Set formation position for the unit
            if (AMingAIController* AIController = Cast<AMingAIController>(Unit->GetController()))
            {
                FVector FormationOffset = TargetPosition - Unit->GetActorLocation();
                AIController->SetFormationPosition(FormationOffset);
            }
        }
    }
}

void UMingFormationSystem::UpdateFormationDirection(FVector NewDirection)
{
    FormationData.FormationDirection = NewDirection.GetSafeNormal();
    
    // Regenerate slots with new direction
    GenerateFormationSlots();
    UpdateSlotPositions();
}

void UMingFormationSystem::AssignUnitToSlot(AMingTacticalUnit* Unit, int32 SlotIndex)
{
    if (!Unit || SlotIndex < 0 || SlotIndex >= FormationData.FormationSlots.Num())
    {
        return;
    }

    // Remove unit from current slot if assigned
    RemoveUnitFromFormation(Unit);

    // Check if slot is available
    if (FormationData.FormationSlots[SlotIndex].bIsOccupied)
    {
        // Find alternative slot
        SlotIndex = FindBestSlotForUnit(Unit);
        if (SlotIndex < 0)
        {
            return; // No available slots
        }
    }

    // Assign unit to slot
    FormationData.FormationSlots[SlotIndex].AssignedUnit = Unit;
    FormationData.FormationSlots[SlotIndex].bIsOccupied = true;
    UnitSlotMap.Add(SlotIndex, Unit);

    // Bind to unit destruction
    Unit->OnDestroyed.AddUniqueDynamic(this, &UMingFormationSystem::OnUnitDestroyed);

    // Set formation position on AI controller
    if (AMingAIController* AIController = Cast<AMingAIController>(Unit->GetController()))
    {
        FVector TargetPosition = GetSlotPosition(SlotIndex);
        FVector FormationOffset = TargetPosition - FormationData.CenterPosition;
        AIController->SetFormationPosition(FormationOffset);
    }

    UE_LOG(LogTemp, Log, TEXT("Unit %d assigned to formation slot %d"), Unit->UnitId, SlotIndex);
}

void UMingFormationSystem::RemoveUnitFromFormation(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return;
    }

    // Find and remove unit from its slot
    for (auto& Pair : UnitSlotMap)
    {
        if (Pair.Value == Unit)
        {
            int32 SlotIndex = Pair.Key;
            
            if (FormationData.FormationSlots.IsValidIndex(SlotIndex))
            {
                FormationData.FormationSlots[SlotIndex].AssignedUnit.Reset();
                FormationData.FormationSlots[SlotIndex].bIsOccupied = false;
            }
            
            UnitSlotMap.Remove(SlotIndex);
            
            // Clear formation position on AI controller
            if (AMingAIController* AIController = Cast<AMingAIController>(Unit->GetController()))
            {
                AIController->ClearFormationPosition();
            }

            UE_LOG(LogTemp, Log, TEXT("Unit %d removed from formation"), Unit->UnitId);
            break;
        }
    }
}

void UMingFormationSystem::DisbandFormation()
{
    // Clear all formation positions
    for (const auto& Pair : UnitSlotMap)
    {
        if (Pair.Value.IsValid())
        {
            if (AMingAIController* AIController = Cast<AMingAIController>(Pair.Value->GetController()))
            {
                AIController->ClearFormationPosition();
            }
        }
    }

    UnitSlotMap.Empty();
    FormationData.FormationSlots.Empty();

    UE_LOG(LogTemp, Log, TEXT("Formation disbanded"));
}

FVector UMingFormationSystem::GetSlotPosition(int32 SlotIndex) const
{
    if (!FormationData.FormationSlots.IsValidIndex(SlotIndex))
    {
        return FormationData.CenterPosition;
    }

    // Calculate world position from relative offset
    FVector RelativeOffset = FormationData.FormationSlots[SlotIndex].RelativeOffset;
    
    // Transform relative offset to world space using formation direction
    FQuat Rotation = FQuat::FindBetweenNormals(FVector::ForwardVector, FormationData.FormationDirection);
    FVector WorldOffset = Rotation.RotateVector(RelativeOffset);
    
    return FormationData.CenterPosition + WorldOffset;
}

int32 UMingFormationSystem::FindBestSlotForUnit(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return -1;
    }

    FVector UnitLocation = Unit->GetActorLocation();
    int32 BestSlot = -1;
    float MinDistance = MAX_flt;

    for (int32 i = 0; i < FormationData.FormationSlots.Num(); ++i)
    {
        if (!FormationData.FormationSlots[i].bIsOccupied)
        {
            FVector SlotPosition = GetSlotPosition(i);
            float Distance = FVector::Dist(UnitLocation, SlotPosition);
            
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                BestSlot = i;
            }
        }
    }

    return BestSlot;
}

bool UMingFormationSystem::HasAvailableSlots() const
{
    for (const auto& Slot : FormationData.FormationSlots)
    {
        if (!Slot.bIsOccupied)
        {
            return true;
        }
    }
    return false;
}

int32 UMingFormationSystem::GetOccupiedSlotCount() const
{
    int32 Count = 0;
    for (const auto& Slot : FormationData.FormationSlots)
    {
        if (Slot.bIsOccupied)
        {
            Count++;
        }
    }
    return Count;
}

void UMingFormationSystem::GenerateFormationSlots()
{
    FormationData.FormationSlots.Empty();

    TArray<FVector> Offsets;
    
    switch (FormationData.FormationType)
    {
    case EMingFormationType::Line:
        Offsets = GenerateLineFormation(FormationData.MaxUnits, FormationData.UnitSpacing);
        break;
    case EMingFormationType::Column:
        Offsets = GenerateColumnFormation(FormationData.MaxUnits, FormationData.UnitSpacing);
        break;
    case EMingFormationType::Wedge:
        Offsets = GenerateWedgeFormation(FormationData.MaxUnits, FormationData.UnitSpacing);
        break;
    case EMingFormationType::Square:
        Offsets = GenerateSquareFormation(FormationData.MaxUnits, FormationData.UnitSpacing);
        break;
    case EMingFormationType::Circle:
        Offsets = GenerateCircleFormation(FormationData.MaxUnits, FormationData.UnitSpacing * 2.0f);
        break;
    case EMingFormationType::Scattered:
        // For scattered formation, just create empty slots
        for (int32 i = 0; i < FormationData.MaxUnits; ++i)
        {
            Offsets.Add(FVector::ZeroVector);
        }
        break;
    default:
        Offsets = GenerateLineFormation(FormationData.MaxUnits, FormationData.UnitSpacing);
        break;
    }

    // Create formation slots
    for (int32 i = 0; i < Offsets.Num(); ++i)
    {
        FMingFormationSlot Slot;
        Slot.SlotIndex = i;
        Slot.RelativeOffset = Offsets[i];
        Slot.bIsOccupied = false;
        
        FormationData.FormationSlots.Add(Slot);
    }
}

void UMingFormationSystem::UpdateSlotPositions()
{
    // Update any existing assignments with new positions
    for (const auto& Pair : UnitSlotMap)
    {
        if (Pair.Value.IsValid())
        {
            if (AMingAIController* AIController = Cast<AMingAIController>(Pair.Value->GetController()))
            {
                FVector TargetPosition = GetSlotPosition(Pair.Key);
                FVector FormationOffset = TargetPosition - FormationData.CenterPosition;
                AIController->SetFormationPosition(FormationOffset);
            }
        }
    }
}

void UMingFormationSystem::RegenerateFormation()
{
    GenerateFormationSlots();
    UpdateSlotPositions();
}

void UMingFormationSystem::OnUnitDestroyed(AActor* DestroyedActor)
{
    AMingTacticalUnit* Unit = Cast<AMingTacticalUnit>(DestroyedActor);
    if (Unit)
    {
        RemoveUnitFromFormation(Unit);
    }
}

// Static formation generators
TArray<FVector> UMingFormationSystem::GenerateLineFormation(int32 UnitCount, float Spacing)
{
    TArray<FVector> Offsets;
    
    float HalfWidth = (UnitCount - 1) * Spacing * 0.5f;
    
    for (int32 i = 0; i < UnitCount; ++i)
    {
        FVector Offset;
        Offset.X = 0.0f;
        Offset.Y = (i * Spacing) - HalfWidth;
        Offset.Z = 0.0f;
        
        Offsets.Add(Offset);
    }
    
    return Offsets;
}

TArray<FVector> UMingFormationSystem::GenerateColumnFormation(int32 UnitCount, float Spacing)
{
    TArray<FVector> Offsets;
    
    float HalfDepth = (UnitCount - 1) * Spacing * 0.5f;
    
    for (int32 i = 0; i < UnitCount; ++i)
    {
        FVector Offset;
        Offset.X = (i * Spacing) - HalfDepth;
        Offset.Y = 0.0f;
        Offset.Z = 0.0f;
        
        Offsets.Add(Offset);
    }
    
    return Offsets;
}

TArray<FVector> UMingFormationSystem::GenerateWedgeFormation(int32 UnitCount, float Spacing)
{
    TArray<FVector> Offsets;
    
    int32 Row = 0;
    int32 UnitsInRow = 1;
    int32 UnitsPlaced = 0;
    
    while (UnitsPlaced < UnitCount)
    {
        float RowOffset = Row * Spacing;
        float HalfWidth = (UnitsInRow - 1) * Spacing * 0.5f;
        
        for (int32 i = 0; i < UnitsInRow && UnitsPlaced < UnitCount; ++i)
        {
            FVector Offset;
            Offset.X = -RowOffset; // Wedge points forward (negative X)
            Offset.Y = (i * Spacing) - HalfWidth;
            Offset.Z = 0.0f;
            
            Offsets.Add(Offset);
            UnitsPlaced++;
        }
        
        Row++;
        UnitsInRow += 2; // Each row adds 2 more units
    }
    
    return Offsets;
}

TArray<FVector> UMingFormationSystem::GenerateSquareFormation(int32 UnitCount, float Spacing)
{
    TArray<FVector> Offsets;
    
    int32 GridSize = FMath::CeilToInt(FMath::Sqrt((float)UnitCount));
    float HalfSize = (GridSize - 1) * Spacing * 0.5f;
    
    int32 UnitsPlaced = 0;
    for (int32 Row = 0; Row < GridSize && UnitsPlaced < UnitCount; ++Row)
    {
        for (int32 Col = 0; Col < GridSize && UnitsPlaced < UnitCount; ++Col)
        {
            FVector Offset;
            Offset.X = (Row * Spacing) - HalfSize;
            Offset.Y = (Col * Spacing) - HalfSize;
            Offset.Z = 0.0f;
            
            Offsets.Add(Offset);
            UnitsPlaced++;
        }
    }
    
    return Offsets;
}

TArray<FVector> UMingFormationSystem::GenerateCircleFormation(int32 UnitCount, float Radius)
{
    TArray<FVector> Offsets;
    
    for (int32 i = 0; i < UnitCount; ++i)
    {
        float Angle = (2.0f * PI * i) / UnitCount;
        
        FVector Offset;
        Offset.X = FMath::Cos(Angle) * Radius;
        Offset.Y = FMath::Sin(Angle) * Radius;
        Offset.Z = 0.0f;
        
        Offsets.Add(Offset);
    }
    
    return Offsets;
}
