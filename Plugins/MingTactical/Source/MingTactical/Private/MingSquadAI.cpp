#include "MingSquadAI.h"
#include "MingTacticalUnit.h"
#include "MingAIController.h"
#include "Engine/World.h"

UMingSquadAI::UMingSquadAI()
{
    CurrentTactic = EMingSquadTactic::Rush;
}

void UMingSquadAI::InitializeSquad(const TArray<FMingSquadMember>& Members)
{
    SquadMembers = Members;
    
    // Assign positions based on roles
    for (int32 i = 0; i < SquadMembers.Num(); ++i)
    {
        SquadMembers[i].SquadPosition = i;
        
        if (SquadMembers[i].Unit)
        {
            SquadMembers[i].Unit->SetSquadAI(this);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Squad initialized with %d members"), SquadMembers.Num());
}

void UMingSquadAI::AddUnitToSquad(AMingTacticalUnit* Unit, EMingSquadRole Role)
{
    if (!Unit)
    {
        return;
    }

    FMingSquadMember NewMember;
    NewMember.Unit = Unit;
    NewMember.Role = Role;
    NewMember.SquadPosition = SquadMembers.Num();
    
    SquadMembers.Add(NewMember);
    Unit->SetSquadAI(this);
    
    UE_LOG(LogTemp, Log, TEXT("Unit %s added to squad as %s"), 
           *Unit->GetName(), *UEnum::GetValueAsString(Role));
}

void UMingSquadAI::RemoveUnitFromSquad(AMingTacticalUnit* Unit)
{
    for (int32 i = 0; i < SquadMembers.Num(); ++i)
    {
        if (SquadMembers[i].Unit == Unit)
        {
            SquadMembers.RemoveAt(i);
            
            // Reassign positions
            for (int32 j = 0; j < SquadMembers.Num(); ++j)
            {
                SquadMembers[j].SquadPosition = j;
            }
            
            UE_LOG(LogTemp, Log, TEXT("Unit %s removed from squad"), *Unit->GetName());
            return;
        }
    }
}

void UMingSquadAI::IssueSquadOrder(const FMingSquadOrder& Order)
{
    CurrentOrder = Order;
    CurrentTactic = Order.Tactic;
    
    ExecuteTactic();
    
    UE_LOG(LogTemp, Log, TEXT("Squad order issued: %s"), *UEnum::GetValueAsString(Order.Tactic));
}

void UMingSquadAI::SetTactic(EMingSquadTactic NewTactic)
{
    CurrentTactic = NewTactic;
    CurrentOrder.Tactic = NewTactic;
    
    ExecuteTactic();
}

void UMingSquadAI::AttackTarget(AActor* Target)
{
    if (!Target)
    {
        return;
    }

    FMingSquadOrder Order;
    Order.Tactic = EMingSquadTactic::Surround;
    Order.TargetActor = Target;
    Order.TargetLocation = Target->GetActorLocation();
    Order.Priority = 2.0f;
    Order.bIsDefensive = false;
    
    IssueSquadOrder(Order);
}

void UMingSquadAI::MoveToLocation(FVector Location)
{
    FMingSquadOrder Order;
    Order.Tactic = EMingSquadTactic::Rush;
    Order.TargetLocation = Location;
    Order.Priority = 1.0f;
    Order.bIsDefensive = false;
    
    IssueSquadOrder(Order);
}

void UMingSquadAI::DefendPosition(FVector Position)
{
    FMingSquadOrder Order;
    Order.Tactic = EMingSquadTactic::HoldPosition;
    Order.TargetLocation = Position;
    Order.Priority = 1.5f;
    Order.bIsDefensive = true;
    
    IssueSquadOrder(Order);
}

void UMingSquadAI::Tick(float DeltaTime)
{
    // Update squad cohesion and coordination
    CoordinateMembers();
    
    // Check if current tactic needs adjustment
    if (!IsSquadCohesive())
    {
        // May need to regroup or adjust tactics
    }
}

void UMingSquadAI::ExecuteTactic()
{
    switch (CurrentTactic)
    {
    case EMingSquadTactic::FlankLeft:
        ExecuteFlankLeft();
        break;
        
    case EMingSquadTactic::FlankRight:
        ExecuteFlankRight();
        break;
        
    case EMingSquadTactic::Pincer:
        ExecutePincer();
        break;
        
    case EMingSquadTactic::Surround:
        ExecuteSurround();
        break;
        
    case EMingSquadTactic::Rush:
        ExecuteRush();
        break;
        
    case EMingSquadTactic::HoldPosition:
        ExecuteHoldPosition();
        break;
        
    case EMingSquadTactic::Retreat:
        ExecuteRetreat();
        break;
        
    case EMingSquadTactic::Ambush:
        ExecuteAmbush();
        break;
    }
}

void UMingSquadAI::ExecuteFlankLeft()
{
    // Left flank maneuver
    FVector Target = CurrentOrder.TargetLocation;
    FVector FlankOffset = FVector(0, -300, 0); // Left offset
    
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (!Member.Unit || !Member.Unit->GetController())
        {
            continue;
        }
        
        AMingAIController* AI = Cast<AMingAIController>(Member.Unit->GetController());
        if (AI)
        {
            FVector MoveTarget = Target + FlankOffset;
            
            // Adjust based on role
            if (Member.Role == EMingSquadRole::Assault)
            {
                MoveTarget += FVector(100, 0, 0); // Move forward
            }
            else if (Member.Role == EMingSquadRole::Support)
            {
                MoveTarget += FVector(-100, 0, 0); // Stay back
            }
            
            AI->IssueMoveCommand(MoveTarget);
            
            if (CurrentOrder.TargetActor)
            {
                AI->IssueAttackCommand(CurrentOrder.TargetActor);
            }
        }
    }
}

void UMingSquadAI::ExecuteFlankRight()
{
    // Right flank maneuver
    FVector Target = CurrentOrder.TargetLocation;
    FVector FlankOffset = FVector(0, 300, 0); // Right offset
    
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (!Member.Unit || !Member.Unit->GetController())
        {
            continue;
        }
        
        AMingAIController* AI = Cast<AMingAIController>(Member.Unit->GetController());
        if (AI)
        {
            FVector MoveTarget = Target + FlankOffset;
            
            if (Member.Role == EMingSquadRole::Assault)
            {
                MoveTarget += FVector(100, 0, 0);
            }
            else if (Member.Role == EMingSquadRole::Support)
            {
                MoveTarget += FVector(-100, 0, 0);
            }
            
            AI->IssueMoveCommand(MoveTarget);
            
            if (CurrentOrder.TargetActor)
            {
                AI->IssueAttackCommand(CurrentOrder.TargetActor);
            }
        }
    }
}

void UMingSquadAI::ExecutePincer()
{
    // Pincer movement - split squad into two groups
    int32 HalfSize = SquadMembers.Num() / 2;
    FVector Target = CurrentOrder.TargetLocation;
    
    for (int32 i = 0; i < SquadMembers.Num(); ++i)
    {
        const FMingSquadMember& Member = SquadMembers[i];
        if (!Member.Unit || !Member.Unit->GetController())
        {
            continue;
        }
        
        AMingAIController* AI = Cast<AMingAIController>(Member.Unit->GetController());
        if (AI)
        {
            FVector MoveTarget;
            
            if (i < HalfSize)
            {
                // Left pincer
                MoveTarget = Target + FVector(0, -200, 0);
            }
            else
            {
                // Right pincer
                MoveTarget = Target + FVector(0, 200, 0);
            }
            
            AI->IssueMoveCommand(MoveTarget);
            
            if (CurrentOrder.TargetActor)
            {
                AI->IssueAttackCommand(CurrentOrder.TargetActor);
            }
        }
    }
}

void UMingSquadAI::ExecuteSurround()
{
    // Surround target from all directions
    if (!CurrentOrder.TargetActor)
    {
        return;
    }
    
    FVector TargetLocation = CurrentOrder.TargetActor->GetActorLocation();
    float Radius = 400.0f;
    
    for (int32 i = 0; i < SquadMembers.Num(); ++i)
    {
        const FMingSquadMember& Member = SquadMembers[i];
        if (!Member.Unit || !Member.Unit->GetController())
        {
            continue;
        }
        
        AMingAIController* AI = Cast<AMingAIController>(Member.Unit->GetController());
        if (AI)
        {
            // Calculate position around target
            float Angle = (2.0f * PI * i) / SquadMembers.Num();
            FVector Offset = FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0);
            FVector MoveTarget = TargetLocation + Offset;
            
            AI->IssueMoveCommand(MoveTarget);
            AI->IssueAttackCommand(CurrentOrder.TargetActor);
        }
    }
}

void UMingSquadAI::ExecuteRush()
{
    // Direct rush to target
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (!Member.Unit || !Member.Unit->GetController())
        {
            continue;
        }
        
        AMingAIController* AI = Cast<AMingAIController>(Member.Unit->GetController());
        if (AI)
        {
            // Assault units lead the charge
            FVector MoveTarget = CurrentOrder.TargetLocation;
            
            if (Member.Role == EMingSquadRole::Assault)
            {
                MoveTarget += FVector(200, 0, 0); // Move forward
            }
            else if (Member.Role == EMingSquadRole::Support)
            {
                MoveTarget += FVector(-100, 0, 0); // Stay back
            }
            
            AI->IssueMoveCommand(MoveTarget);
            
            if (CurrentOrder.TargetActor)
            {
                AI->IssueAttackCommand(CurrentOrder.TargetActor);
            }
        }
    }
}

void UMingSquadAI::ExecuteHoldPosition()
{
    // Hold defensive position
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (!Member.Unit || !Member.Unit->GetController())
        {
            continue;
        }
        
        AMingAIController* AI = Cast<AMingAIController>(Member.Unit->GetController());
        if (AI)
        {
            // Spread out around the position
            FVector BasePosition = CurrentOrder.TargetLocation;
            FVector Offset = GetRandomOffsetInRadius(150.0f);
            FVector HoldPosition = BasePosition + Offset;
            
            AI->IssueMoveCommand(HoldPosition);
            
            // Set to defensive stance
            AI->SetDefensiveStance(true);
        }
    }
}

void UMingSquadAI::ExecuteRetreat()
{
    // Retreat away from target
    FVector RetreatDirection = FVector::ZeroVector;
    
    if (CurrentOrder.TargetActor)
    {
        FVector CurrentLocation = GetSquadCenter();
        RetreatDirection = (CurrentLocation - CurrentOrder.TargetActor->GetActorLocation()).GetSafeNormal();
    }
    else
    {
        RetreatDirection = FVector(-1, 0, 0); // Default retreat direction
    }
    
    FVector RetreatTarget = GetSquadCenter() + RetreatDirection * 500.0f;
    
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (!Member.Unit || !Member.Unit->GetController())
        {
            continue;
        }
        
        AMingAIController* AI = Cast<AMingAIController>(Member.Unit->GetController());
        if (AI)
        {
            // Add some randomness to avoid bunching up
            FVector RandomOffset = GetRandomOffsetInRadius(100.0f);
            FVector UnitRetreatTarget = RetreatTarget + RandomOffset;
            
            AI->IssueMoveCommand(UnitRetreatTarget);
            
            // Set to retreat mode
            AI->SetRetreatMode(true);
        }
    }
}

void UMingSquadAI::ExecuteAmbush()
{
    // Set up ambush - hide units and wait for target
    if (!CurrentOrder.TargetActor)
    {
        return;
    }
    
    FVector TargetLocation = CurrentOrder.TargetActor->GetActorLocation();
    FVector PredictedPath = TargetLocation + FVector(300, 0, 0); // Predict target movement
    
    for (int32 i = 0; i < SquadMembers.Num(); ++i)
    {
        const FMingSquadMember& Member = SquadMembers[i];
        if (!Member.Unit || !Member.Unit->GetController())
        {
            continue;
        }
        
        AMingAIController* AI = Cast<AMingAIController>(Member.Unit->GetController());
        if (AI)
        {
            // Position units in ambush locations
            FVector AmbushPosition;
            
            if (i % 2 == 0)
            {
                AmbushPosition = PredictedPath + FVector(0, -200, 0);
            }
            else
            {
                AmbushPosition = PredictedPath + FVector(0, 200, 0);
            }
            
            AI->IssueMoveCommand(AmbushPosition);
            AI->SetAmbushMode(true);
            
            // Hold fire until target is in range
            AI->SetHoldFire(true);
        }
    }
}

void UMingSquadAI::CoordinateMembers()
{
    // Coordinate squad members based on their roles
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (!Member.Unit)
        {
            continue;
        }
        
        switch (Member.Role)
        {
        case EMingSquadRole::Support:
            // Support units should stay near assault units
            CoordinateSupportUnit(Member);
            break;
            
        case EMingSquadRole::Medic:
            // Medics should stay near injured units
            CoordinateMedicUnit(Member);
            break;
            
        case EMingSquadRole::Scout:
            // Scouts should move ahead
            CoordinateScoutUnit(Member);
            break;
            
        default:
            break;
        }
    }
}

void UMingSquadAI::CoordinateSupportUnit(const FMingSquadMember& SupportMember)
{
    // Find nearest assault unit and stay nearby
    AMingTacticalUnit* NearestAssault = nullptr;
    float MinDistance = FLT_MAX;
    
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (Member.Role == EMingSquadRole::Assault && Member.Unit)
        {
            float Distance = FVector::Dist(SupportMember.Unit->GetActorLocation(), Member.Unit->GetActorLocation());
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                NearestAssault = Member.Unit;
            }
        }
    }
    
    if (NearestAssault && MinDistance > 200.0f)
    {
        // Move support unit closer to assault unit
        AMingAIController* AI = Cast<AMingAIController>(SupportMember.Unit->GetController());
        if (AI)
        {
            FVector MoveTarget = NearestAssault->GetActorLocation() + FVector(-100, 0, 0);
            AI->IssueMoveCommand(MoveTarget);
        }
    }
}

void UMingSquadAI::CoordinateMedicUnit(const FMingSquadMember& MedicMember)
{
    // Find most injured squad member
    AMingTacticalUnit* MostInjured = nullptr;
    float LowestHealthPercentage = 1.0f;
    
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (Member.Unit && Member.Unit != MedicMember.Unit)
        {
            float HealthPercentage = Member.Unit->GetHealthPercentage();
            if (HealthPercentage < LowestHealthPercentage && HealthPercentage < 0.7f)
            {
                LowestHealthPercentage = HealthPercentage;
                MostInjured = Member.Unit;
            }
        }
    }
    
    if (MostInjured)
    {
        // Move medic to injured unit
        AMingAIController* AI = Cast<AMingAIController>(MedicMember.Unit->GetController());
        if (AI)
        {
            FVector MoveTarget = MostInjured->GetActorLocation() + FVector(-50, 0, 0);
            AI->IssueMoveCommand(MoveTarget);
        }
    }
}

void UMingSquadAI::CoordinateScoutUnit(const FMingSquadMember& ScoutMember)
{
    // Scouts should move ahead of the squad
    FVector SquadCenter = GetSquadCenter();
    FVector ScoutDirection = CurrentOrder.TargetLocation - SquadCenter;
    ScoutDirection = ScoutDirection.GetSafeNormal();
    
    FVector ScoutTarget = SquadCenter + ScoutDirection * 300.0f;
    
    AMingAIController* AI = Cast<AMingAIController>(ScoutMember.Unit->GetController());
    if (AI)
    {
        AI->IssueMoveCommand(ScoutTarget);
        AI->SetScoutMode(true);
    }
}

bool UMingSquadAI::IsSquadCohesive()
{
    if (SquadMembers.Num() <= 1)
    {
        return true;
    }
    
    FVector SquadCenter = GetSquadCenter();
    float MaxDistance = 0.0f;
    
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (Member.Unit)
        {
            float Distance = FVector::Dist(Member.Unit->GetActorLocation(), SquadCenter);
            MaxDistance = FMath::Max(MaxDistance, Distance);
        }
    }
    
    // Squad is cohesive if all members are within 400 units of center
    return MaxDistance <= 400.0f;
}

FVector UMingSquadAI::GetSquadCenter() const
{
    FVector Center = FVector::ZeroVector;
    int32 ValidMembers = 0;
    
    for (const FMingSquadMember& Member : SquadMembers)
    {
        if (Member.Unit)
        {
            Center += Member.Unit->GetActorLocation();
            ValidMembers++;
        }
    }
    
    if (ValidMembers > 0)
    {
        Center /= ValidMembers;
    }
    
    return Center;
}

FVector UMingSquadAI::GetRandomOffsetInRadius(float Radius)
{
    float Angle = FMath::RandRange(0.0f, 2.0f * PI);
    float Distance = FMath::RandRange(0.0f, Radius);
    return FVector(FMath::Cos(Angle) * Distance, FMath::Sin(Angle) * Distance, 0.0f);
}
