#include "Innovation/MingSocialDynamicsSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Algo/RandomShuffle.h"
#include "Misc/DateTime.h"

UMingSocialDynamicsSystem::UMingSocialDynamicsSystem()
{
    SimulationSpeed = 1.0f;
    RelationshipDecayRate = 0.01f;
    CulturalTransmissionRate = 0.1f;
    SocialMobilityProbability = 0.05f;
    GroupBehaviorThreshold = 0.7f;
    MaxIndividuals = 1000;
    MaxRelationshipsPerIndividual = 50;
    bEnableCulturalEvolution = true;
    bEnableSocialMobility = true;
    bEnableGroupBehaviors = true;
}

void UMingSocialDynamicsSystem::InitializeSocialDynamicsSystem()
{
    // Initialize system state
    bSystemInitialized = true;
    LastUpdateTime = FDateTime::Now();
    
    // Clear existing data
    Individuals.Empty();
    Relationships.Empty();
    SocialEvents.Empty();
    CulturalTraits.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Social Dynamics System initialized"));
}

void UMingSocialDynamicsSystem::ShutdownSocialDynamicsSystem()
{
    bSystemInitialized = false;
    
    // Clear all data
    Individuals.Empty();
    Relationships.Empty();
    SocialEvents.Empty();
    CulturalTraits.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Social Dynamics System shutdown"));
}

void UMingSocialDynamicsSystem::AddIndividual(const FSocialIndividual& Individual)
{
    if (Individuals.Num() >= MaxIndividuals)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum individuals limit reached"));
        return;
    }
    
    // Add individual with unique ID
    FSocialIndividual NewIndividual = Individual;
    if (NewIndividual.IndividualID.IsEmpty())
    {
        NewIndividual.IndividualID = FString::Printf(TEXT("Individual_%d"), Individuals.Num());
    }
    
    Individuals.Add(NewIndividual.IndividualID, NewIndividual);
    
    UE_LOG(LogTemp, Log, TEXT("Added individual: %s"), *NewIndividual.IndividualID);
}

void UMingSocialDynamicsSystem::RemoveIndividual(const FString& IndividualID)
{
    if (Individuals.Contains(IndividualID))
    {
        // Remove all relationships involving this individual
        Relationships.RemoveAll([&](const FSocialRelationship& Rel) {
            return Rel.IndividualA == IndividualID || Rel.IndividualB == IndividualID;
        });
        
        // Remove individual
        Individuals.Remove(IndividualID);
        
        UE_LOG(LogTemp, Log, TEXT("Removed individual: %s"), *IndividualID);
    }
}

FSocialIndividual UMingSocialDynamicsSystem::GetIndividual(const FString& IndividualID) const
{
    if (Individuals.Contains(IndividualID))
    {
        return Individuals[IndividualID];
    }
    
    return FSocialIndividual();
}

TArray<FSocialIndividual> UMingSocialDynamicsSystem::GetAllIndividuals() const
{
    TArray<FSocialIndividual> AllIndividuals;
    
    for (const auto& IndividualPair : Individuals)
    {
        AllIndividuals.Add(IndividualPair.Value);
    }
    
    return AllIndividuals;
}

void UMingSocialDynamicsSystem::CreateRelationship(const FSocialRelationship& Relationship)
{
    // Validate relationship
    FSocialRelationship ValidatedRelationship = Relationship;
    ValidateRelationship(ValidatedRelationship);
    
    // Check if relationship already exists
    bool bExists = false;
    for (const FSocialRelationship& ExistingRel : Relationships)
    {
        if ((ExistingRel.IndividualA == ValidatedRelationship.IndividualA && 
             ExistingRel.IndividualB == ValidatedRelationship.IndividualB) ||
            (ExistingRel.IndividualA == ValidatedRelationship.IndividualB && 
             ExistingRel.IndividualB == ValidatedRelationship.IndividualA))
        {
            bExists = true;
            break;
        }
    }
    
    if (!bExists)
    {
        Relationships.Add(ValidatedRelationship);
        
        // Update individual relationships
        if (Individuals.Contains(ValidatedRelationship.IndividualA))
        {
            Individuals[ValidatedRelationship.IndividualA].Relationships.Add(ValidatedRelationship);
        }
        if (Individuals.Contains(ValidatedRelationship.IndividualB))
        {
            Individuals[ValidatedRelationship.IndividualB].Relationships.Add(ValidatedRelationship);
        }
        
        // Broadcast event
        OnSocialRelationshipChanged.Broadcast(ValidatedRelationship);
        
        UE_LOG(LogTemp, Log, TEXT("Created relationship between %s and %s"), 
            *ValidatedRelationship.IndividualA, *ValidatedRelationship.IndividualB);
    }
}

void UMingSocialDynamicsSystem::UpdateRelationship(const FSocialRelationship& Relationship)
{
    for (int32 i = 0; i < Relationships.Num(); ++i)
    {
        FSocialRelationship& ExistingRel = Relationships[i];
        if ((ExistingRel.IndividualA == Relationship.IndividualA && 
             ExistingRel.IndividualB == Relationship.IndividualB) ||
            (ExistingRel.IndividualA == Relationship.IndividualB && 
             ExistingRel.IndividualB == Relationship.IndividualA))
        {
            ExistingRel = Relationship;
            CalculateRelationshipStrength(ExistingRel);
            
            // Broadcast event
            OnSocialRelationshipChanged.Broadcast(ExistingRel);
            
            UE_LOG(LogTemp, Log, TEXT("Updated relationship between %s and %s"), 
                *Relationship.IndividualA, *Relationship.IndividualB);
            break;
        }
    }
}

void UMingSocialDynamicsSystem::RemoveRelationship(const FString& IndividualA, const FString& IndividualB)
{
    for (int32 i = Relationships.Num() - 1; i >= 0; --i)
    {
        const FSocialRelationship& Rel = Relationships[i];
        if ((Rel.IndividualA == IndividualA && Rel.IndividualB == IndividualB) ||
            (Rel.IndividualA == IndividualB && Rel.IndividualB == IndividualA))
        {
            Relationships.RemoveAt(i);
            
            UE_LOG(LogTemp, Log, TEXT("Removed relationship between %s and %s"), 
                *IndividualA, *IndividualB);
            break;
        }
    }
}

TArray<FSocialRelationship> UMingSocialDynamicsSystem::GetIndividualRelationships(const FString& IndividualID) const
{
    TArray<FSocialRelationship> IndividualRelationships;
    
    for (const FSocialRelationship& Rel : Relationships)
    {
        if (Rel.IndividualA == IndividualID || Rel.IndividualB == IndividualID)
        {
            IndividualRelationships.Add(Rel);
        }
    }
    
    return IndividualRelationships;
}

void UMingSocialDynamicsSystem::CreateSocialEvent(const FSocialEvent& Event)
{
    FSocialEvent NewEvent = Event;
    if (NewEvent.EventID.IsEmpty())
    {
        NewEvent.EventID = FString::Printf(TEXT("Event_%d"), SocialEvents.Num());
    }
    
    SocialEvents.Add(NewEvent);
    
    // Process event impacts
    ProcessSocialImpact(NewEvent);
    
    // Broadcast event
    OnSocialEventOccurred.Broadcast(NewEvent);
    
    UE_LOG(LogTemp, Log, TEXT("Created social event: %s"), *NewEvent.EventID);
}

void UMingSocialDynamicsSystem::ProcessSocialEvent(const FString& EventID)
{
    for (const FSocialEvent& Event : SocialEvents)
    {
        if (Event.EventID == EventID)
        {
            ProcessSocialImpact(Event);
            break;
        }
    }
}

TArray<FSocialEvent> UMingSocialDynamicsSystem::GetRecentEvents(int32 Count) const
{
    TArray<FSocialEvent> RecentEvents;
    
    // Sort events by date
    TArray<FSocialEvent> SortedEvents = SocialEvents;
    SortedEvents.Sort([&](const FSocialEvent& A, const FSocialEvent& B) {
        return A.EventDate > B.EventDate;
    });
    
    // Get recent events
    for (int32 i = 0; i < FMath::Min(Count, SortedEvents.Num()); ++i)
    {
        RecentEvents.Add(SortedEvents[i]);
    }
    
    return RecentEvents;
}

void UMingSocialDynamicsSystem::AddCulturalTrait(const FCulturalTrait& Trait)
{
    FCulturalTrait NewTrait = Trait;
    if (NewTrait.TraitID.IsEmpty())
    {
        NewTrait.TraitID = FString::Printf(TEXT("Trait_%d"), CulturalTraits.Num());
    }
    
    CulturalTraits.Add(NewTrait);
    
    // Broadcast event
    OnCulturalTraitSpread.Broadcast(NewTrait);
    
    UE_LOG(LogTemp, Log, TEXT("Added cultural trait: %s"), *NewTrait.TraitID);
}

void UMingSocialDynamicsSystem::SpreadCulturalTrait(const FString& TraitID, ECulturalTransmissionType TransmissionType)
{
    for (FCulturalTrait& Trait : CulturalTraits)
    {
        if (Trait.TraitID == TraitID)
        {
            // Simulate cultural spread based on transmission type
            switch (TransmissionType)
            {
                case ECulturalTransmissionType::Vertical:
                    SpreadVerticalTransmission(Trait);
                    break;
                case ECulturalTransmissionType::Horizontal:
                    SpreadHorizontalTransmission(Trait);
                    break;
                case ECulturalTransmissionType::Oblique:
                    SpreadObliqueTransmission(Trait);
                    break;
                case ECulturalTransmissionType::MassMedia:
                    SpreadMassMediaTransmission(Trait);
                    break;
                case ECulturalTransmissionType::Education:
                    SpreadEducationTransmission(Trait);
                    break;
                case ECulturalTransmissionType::Religious:
                    SpreadReligiousTransmission(Trait);
                    break;
                case ECulturalTransmissionType::Technology:
                    SpreadTechnologyTransmission(Trait);
                    break;
            }
            
            // Broadcast event
            OnCulturalTraitSpread.Broadcast(Trait);
            
            UE_LOG(LogTemp, Log, TEXT("Spread cultural trait: %s via %s"), 
                *TraitID, *UEnum::GetValueAsString(TransmissionType));
            break;
        }
    }
}

TArray<FCulturalTrait> UMingSocialDynamicsSystem::GetCulturalTraits() const
{
    return CulturalTraits;
}

FSocialNetworkMetrics UMingSocialDynamicsSystem::AnalyzeSocialNetwork() const
{
    FSocialNetworkMetrics Metrics;
    
    // Calculate basic metrics
    Metrics.TotalNodes = Individuals.Num();
    Metrics.TotalEdges = Relationships.Num();
    
    if (Metrics.TotalNodes > 0)
    {
        // Calculate network density
        float MaxPossibleEdges = Metrics.TotalNodes * (Metrics.TotalNodes - 1) / 2.0f;
        Metrics.NetworkDensity = Metrics.TotalEdges / MaxPossibleEdges;
        
        // Calculate average path length (simplified)
        Metrics.AveragePathLength = CalculateAveragePathLength();
        
        // Calculate clustering coefficient
        Metrics.ClusteringCoefficient = CalculateClusteringCoefficient();
        
        // Calculate connected components
        Metrics.ConnectedComponents = CalculateConnectedComponents();
        
        // Calculate modularity
        Metrics.Modularity = CalculateModularity();
        
        // Calculate node centrality
        CalculateNodeCentrality(Metrics);
    }
    
    return Metrics;
}

float UMingSocialDynamicsSystem::CalculateSocialInfluence(const FString& IndividualID) const
{
    if (!Individuals.Contains(IndividualID))
    {
        return 0.0f;
    }
    
    const FSocialIndividual& Individual = Individuals[IndividualID];
    
    // Calculate influence based on multiple factors
    float RelationshipInfluence = Individual.Relationships.Num() * 0.1f;
    float ClassInfluence = GetClassInfluenceValue(Individual.SocialClass);
    float EconomicInfluence = Individual.EconomicStatus * 0.2f;
    float EducationInfluence = Individual.EducationLevel * 0.15f;
    
    return RelationshipInfluence + ClassInfluence + EconomicInfluence + EducationInfluence;
}

TArray<FString> UMingSocialDynamicsSystem::FindInfluentialIndividuals(int32 Count) const
{
    TArray<FString> InfluentialIndividuals;
    
    // Calculate influence for all individuals
    TArray<TPair<float, FString>> InfluenceScores;
    
    for (const auto& IndividualPair : Individuals)
    {
        float Influence = CalculateSocialInfluence(IndividualPair.Key);
        InfluenceScores.Add(TPair<float, FString>(Influence, IndividualPair.Key));
    }
    
    // Sort by influence (descending)
    InfluenceScores.Sort([&](const TPair<float, FString>& A, const TPair<float, FString>& B) {
        return A.Key > B.Key;
    });
    
    // Get top influential individuals
    for (int32 i = 0; i < FMath::Min(Count, InfluenceScores.Num()); ++i)
    {
        InfluentialIndividuals.Add(InfluenceScores[i].Value);
    }
    
    return InfluentialIndividuals;
}

void UMingSocialDynamicsSystem::ProcessSocialMobility()
{
    if (!bEnableSocialMobility)
    {
        return;
    }
    
    // Process social mobility for each individual
    for (auto& IndividualPair : Individuals)
    {
        FSocialIndividual& Individual = IndividualPair.Value;
        
        // Calculate mobility probability based on individual attributes
        float MobilityChance = SocialMobilityProbability;
        MobilityChance *= (1.0f + Individual.EconomicStatus * 0.1f);
        MobilityChance *= (1.0f + Individual.EducationLevel * 0.1f);
        
        if (FMath::FRand() < MobilityChance)
        {
            ESocialClass OldClass = Individual.SocialClass;
            ESocialClass NewClass = CalculateNewSocialClass(Individual);
            
            if (OldClass != NewClass)
            {
                HandleClassTransition(Individual.IndividualID, OldClass, NewClass);
                Individual.SocialClass = NewClass;
                
                UE_LOG(LogTemp, Log, TEXT("Individual %s moved from %s to %s"), 
                    *Individual.IndividualID, 
                    *UEnum::GetValueAsString(OldClass), 
                    *UEnum::GetValueAsString(NewClass));
            }
        }
    }
}

void UMingSocialDynamicsSystem::PromoteIndividual(const FString& IndividualID)
{
    if (Individuals.Contains(IndividualID))
    {
        FSocialIndividual& Individual = Individuals[IndividualID];
        ESocialClass OldClass = Individual.SocialClass;
        ESocialClass NewClass = PromoteSocialClass(OldClass);
        
        if (OldClass != NewClass)
        {
            HandleClassTransition(IndividualID, OldClass, NewClass);
            Individual.SocialClass = NewClass;
            
            UE_LOG(LogTemp, Log, TEXT("Promoted individual %s from %s to %s"), 
                *IndividualID, *UEnum::GetValueAsString(OldClass), *UEnum::GetValueAsString(NewClass));
        }
    }
}

void UMingSocialDynamicsSystem::DemoteIndividual(const FString& IndividualID)
{
    if (Individuals.Contains(IndividualID))
    {
        FSocialIndividual& Individual = Individuals[IndividualID];
        ESocialClass OldClass = Individual.SocialClass;
        ESocialClass NewClass = DemoteSocialClass(OldClass);
        
        if (OldClass != NewClass)
        {
            HandleClassTransition(IndividualID, OldClass, NewClass);
            Individual.SocialClass = NewClass;
            
            UE_LOG(LogTemp, Log, TEXT("Demoted individual %s from %s to %s"), 
                *IndividualID, *UEnum::GetValueAsString(OldClass), *UEnum::GetValueAsString(NewClass));
        }
    }
}

void UMingSocialDynamicsSystem::SimulateGroupBehavior()
{
    if (!bEnableGroupBehaviors)
    {
        return;
    }
    
    // Identify social groups based on relationships
    TArray<TArray<FString>> SocialGroups = IdentifySocialGroups();
    
    // Simulate group behaviors
    for (const TArray<FString>& Group : SocialGroups)
    {
        if (Group.Num() >= GroupBehaviorThreshold * MaxIndividuals)
        {
            SimulateGroupDecision(Group);
            SimulateGroupCohesion(Group);
        }
    }
}

void UMingSocialDynamicsSystem::ProcessSocialContagion(const FString& TraitID, float ContagionRate)
{
    if (!CulturalTraits.Contains(TraitID))
    {
        return;
    }
    
    const FCulturalTrait& Trait = CulturalTraits[TraitID];
    
    // Simulate contagion through social network
    TArray<FString> InfectedIndividuals = Trait.Adopters;
    TArray<FString> NewInfectedIndividuals;
    
    for (const FString& InfectedID : InfectedIndividuals)
    {
        TArray<FSocialRelationship> Relationships = GetIndividualRelationships(InfectedID);
        
        for (const FSocialRelationship& Rel : Relationships)
        {
            FString ContactID = (Rel.IndividualA == InfectedID) ? Rel.IndividualB : Rel.IndividualA;
            
            if (!InfectedIndividuals.Contains(ContactID) && !NewInfectedIndividuals.Contains(ContactID))
            {
                // Calculate infection probability
                float InfectionProb = ContagionRate * Rel.RelationshipStrength * Rel.TrustLevel;
                
                if (FMath::FRand() < InfectionProb)
                {
                    NewInfectedIndividuals.Add(ContactID);
                }
            }
        }
    }
    
    // Update trait adopters
    FCulturalTrait& MutableTrait = CulturalTraits[TraitID];
    MutableTrait.Adopters.Append(NewInfectedIndividuals);
    
    UE_LOG(LogTemp, Log, TEXT("Social contagion for trait %s: %d new adopters"), 
        *TraitID, NewInfectedIndividuals.Num());
}

TMap<ESocialClass, int32> UMingSocialDynamicsSystem::GetClassDistribution() const
{
    TMap<ESocialClass, int32> Distribution;
    
    // Initialize all classes to 0
    for (int32 i = 0; i <= static_cast<int32>(ESocialClass::Outcast); ++i)
    {
        ESocialClass Class = static_cast<ESocialClass>(i);
        Distribution.Add(Class, 0);
    }
    
    // Count individuals in each class
    for (const auto& IndividualPair : Individuals)
    {
        ESocialClass Class = IndividualPair.Value.SocialClass;
        Distribution[Class] = Distribution[Class] + 1;
    }
    
    return Distribution;
}

TMap<ESocialRelationType, int32> UMingSocialDynamicsSystem::GetRelationshipTypeDistribution() const
{
    TMap<ESocialRelationType, int32> Distribution;
    
    // Initialize all relationship types to 0
    for (int32 i = 0; i <= static_cast<int32>(ESocialRelationType::Rivalry); ++i)
    {
        ESocialRelationType Type = static_cast<ESocialRelationType>(i);
        Distribution.Add(Type, 0);
    }
    
    // Count relationships by type
    for (const FSocialRelationship& Rel : Relationships)
    {
        ESocialRelationType Type = Rel.RelationType;
        Distribution[Type] = Distribution[Type] + 1;
    }
    
    return Distribution;
}

float UMingSocialDynamicsSystem::CalculateSocialCohesion() const
{
    if (Individuals.Num() < 2)
    {
        return 1.0f;
    }
    
    float TotalCohesion = 0.0f;
    int32 Count = 0;
    
    for (const FSocialRelationship& Rel : Relationships)
    {
        TotalCohesion += Rel.RelationshipStrength * Rel.TrustLevel;
        Count++;
    }
    
    if (Count > 0)
    {
        return TotalCohesion / Count;
    }
    
    return 0.0f;
}

void UMingSocialDynamicsSystem::Tick(float DeltaTime)
{
    if (!bSystemInitialized)
    {
        return;
    }
    
    // Update simulation
    float AdjustedDeltaTime = DeltaTime * SimulationSpeed;
    
    // Update relationships
    UpdateRelationships();
    
    // Process cultural evolution
    if (bEnableCulturalEvolution)
    {
        ProcessCulturalEvolution();
    }
    
    // Update social network
    UpdateSocialNetwork();
    
    // Handle social conflicts
    HandleSocialConflict();
    
    // Update last update time
    LastUpdateTime = FDateTime::Now();
}

// Private helper functions

void UMingSocialDynamicsSystem::ValidateRelationship(FSocialRelationship& Relationship)
{
    // Ensure both individuals exist
    if (!Individuals.Contains(Relationship.IndividualA) || !Individuals.Contains(Relationship.IndividualB))
    {
        UE_LOG(LogTemp, Warning, TEXT("Relationship validation failed: individuals not found"));
        return;
    }
    
    // Calculate initial relationship strength
    CalculateRelationshipStrength(Relationship);
    
    // Clamp values
    Relationship.RelationshipStrength = FMath::Clamp(Relationship.RelationshipStrength, 0.0f, 1.0f);
    Relationship.TrustLevel = FMath::Clamp(Relationship.TrustLevel, 0.0f, 1.0f);
    Relationship.InfluenceLevel = FMath::Clamp(Relationship.InfluenceLevel, 0.0f, 1.0f);
}

void UMingSocialDynamicsSystem::CalculateRelationshipStrength(FSocialRelationship& Relationship)
{
    // Calculate strength based on shared interests and values
    float SharedInterestScore = 0.0f;
    float SharedValueScore = 0.0f;
    
    const FSocialIndividual& IndividualA = Individuals[Relationship.IndividualA];
    const FSocialIndividual& IndividualB = Individuals[Relationship.IndividualB];
    
    // Calculate shared interests
    for (const FString& InterestA : IndividualA.Interests)
    {
        if (IndividualB.Interests.Contains(InterestA))
        {
            SharedInterestScore += 1.0f;
        }
    }
    
    // Calculate shared values
    for (const FString& ValueA : IndividualA.Values)
    {
        if (IndividualB.Values.Contains(ValueA))
        {
            SharedValueScore += 1.0f;
        }
    }
    
    // Normalize scores
    float MaxSharedInterests = FMath::Min(IndividualA.Interests.Num(), IndividualB.Interests.Num());
    float MaxSharedValues = FMath::Min(IndividualA.Values.Num(), IndividualB.Values.Num());
    
    if (MaxSharedInterests > 0)
    {
        SharedInterestScore /= MaxSharedInterests;
    }
    
    if (MaxSharedValues > 0)
    {
        SharedValueScore /= MaxSharedValues;
    }
    
    // Calculate final relationship strength
    Relationship.RelationshipStrength = (SharedInterestScore + SharedValueScore) / 2.0f;
    
    // Update trust and influence based on relationship strength
    Relationship.TrustLevel = Relationship.RelationshipStrength * 0.8f;
    Relationship.InfluenceLevel = Relationship.RelationshipStrength * 0.6f;
}

void UMingSocialDynamicsSystem::ProcessSocialImpact(const FSocialEvent& Event)
{
    // Process impact on individuals
    for (const auto& ImpactPair : Event.IndividualImpacts)
    {
        const FString& IndividualID = ImpactPair.Key;
        float ImpactValue = ImpactPair.Value;
        
        if (Individuals.Contains(IndividualID))
        {
            FSocialIndividual& Individual = Individuals[IndividualID];
            
            // Update individual attributes based on impact
            Individual.SocialInfluence += ImpactValue * 0.1f;
            Individual.SocialInfluence = FMath::Clamp(Individual.SocialInfluence, 0.0f, 1.0f);
            
            UpdateIndividualInfluence(IndividualID);
        }
    }
    
    // Process relationship impacts
    for (FSocialRelationship& Rel : Relationships)
    {
        if (Event.Participants.Contains(Rel.IndividualA) && Event.Participants.Contains(Rel.IndividualB))
        {
            // Strengthen or weaken relationships based on event impact
            float ImpactModifier = Event.SocialImpact * 0.1f;
            Rel.RelationshipStrength += ImpactModifier;
            Rel.RelationshipStrength = FMath::Clamp(Rel.RelationshipStrength, 0.0f, 1.0f);
        }
    }
}

void UMingSocialDynamicsSystem::UpdateIndividualInfluence(const FString& IndividualID)
{
    if (!Individuals.Contains(IndividualID))
    {
        return;
    }
    
    FSocialIndividual& Individual = Individuals[IndividualID];
    
    // Recalculate influence based on current attributes
    float NewInfluence = CalculateSocialInfluence(IndividualID);
    Individual.SocialInfluence = NewInfluence;
}

void UMingSocialDynamicsSystem::HandleClassTransition(const FString& IndividualID, ESocialClass OldClass, ESocialClass NewClass)
{
    // Update relationships based on class change
    TArray<FSocialRelationship> IndividualRelationships = GetIndividualRelationships(IndividualID);
    
    for (FSocialRelationship& Rel : IndividualRelationships)
    {
        FString OtherIndividualID = (Rel.IndividualA == IndividualID) ? Rel.IndividualB : Rel.IndividualA;
        
        if (Individuals.Contains(OtherIndividualID))
        {
            ESocialClass OtherClass = Individuals[OtherIndividualID].SocialClass;
            
            // Adjust relationship strength based on class compatibility
            float ClassCompatibility = CalculateClassCompatibility(NewClass, OtherClass);
            Rel.RelationshipStrength *= ClassCompatibility;
            Rel.RelationshipStrength = FMath::Clamp(Rel.RelationshipStrength, 0.0f, 1.0f);
        }
    }
    
    // Broadcast class change event
    OnSocialClassChanged.Broadcast(IndividualID);
    
    UE_LOG(LogTemp, Log, TEXT("Handled class transition for %s: %s -> %s"), 
        *IndividualID, *UEnum::GetValueAsString(OldClass), *UEnum::GetValueAsString(NewClass));
}

void UMingSocialDynamicsSystem::UpdateRelationships()
{
    // Decay relationships over time
    for (FSocialRelationship& Rel : Relationships)
    {
        Rel.RelationshipStrength *= (1.0f - RelationshipDecayRate);
        Rel.RelationshipStrength = FMath::Clamp(Rel.RelationshipStrength, 0.0f, 1.0f);
    }
    
    // Remove very weak relationships
    Relationships.RemoveAll([&](const FSocialRelationship& Rel) {
        return Rel.RelationshipStrength < 0.01f;
    });
}

void UMingSocialDynamicsSystem::ProcessCulturalEvolution()
{
    // Simulate cultural trait evolution
    for (FCulturalTrait& Trait : CulturalTraits)
    {
        // Apply mutation
        if (FMath::FRand() < Trait.MutationRate)
        {
            // Mutate trait (simplified - in reality would be more complex)
            Trait.AdoptionRate *= FMath::RandRange(0.9f, 1.1f);
            Trait.AdoptionRate = FMath::Clamp(Trait.AdoptionRate, 0.0f, 1.0f);
        }
        
        // Apply regional variations
        for (auto& RegionalPair : Trait.RegionalVariations)
        {
            if (FMath::FRand() < 0.01f) // Small chance of regional change
            {
                RegionalPair.Value *= FMath::RandRange(0.95f, 1.05f);
                RegionalPair.Value = FMath::Clamp(RegionalPair.Value, 0.0f, 1.0f);
            }
        }
    }
}

void UMingSocialDynamicsSystem::UpdateSocialNetwork()
{
    // Update network metrics
    FSocialNetworkMetrics Metrics = AnalyzeSocialNetwork();
    
    // Log network statistics periodically
    static int32 UpdateCounter = 0;
    if (++UpdateCounter >= 100) // Every 100 ticks
    {
        UE_LOG(LogTemp, Log, TEXT("Social Network Metrics - Nodes: %d, Edges: %d, Density: %.3f"), 
            Metrics.TotalNodes, Metrics.TotalEdges, Metrics.NetworkDensity);
        UpdateCounter = 0;
    }
}

void UMingSocialDynamicsSystem::HandleSocialConflict()
{
    // Check for potential conflicts based on relationships and attributes
    for (const FSocialRelationship& Rel : Relationships)
    {
        if (Rel.RelationType == ESocialRelationType::Rivalry && Rel.RelationshipStrength > 0.5f)
        {
            // High rivalry could lead to conflict
            if (FMath::FRand() < 0.001f) // Small chance of conflict
            {
                // Create conflict event
                FSocialEvent ConflictEvent;
                ConflictEvent.EventType = ESocialEventType::SocialMovement;
                ConflictEvent.Description = FString::Printf(TEXT("Conflict between %s and %s"), 
                    *Rel.IndividualA, *Rel.IndividualB);
                ConflictEvent.Participants = {Rel.IndividualA, Rel.IndividualB};
                ConflictEvent.SocialImpact = -0.3f;
                
                CreateSocialEvent(ConflictEvent);
            }
        }
    }
}

// Additional helper functions (simplified implementations)

float UMingSocialDynamicsSystem::GetClassInfluenceValue(ESocialClass Class) const
{
    switch (Class)
    {
        case ESocialClass::Upper: return 1.0f;
        case ESocialClass::UpperMiddle: return 0.8f;
        case ESocialClass::Middle: return 0.6f;
        case ESocialClass::LowerMiddle: return 0.4f;
        case ESocialClass::Lower: return 0.2f;
        case ESocialClass::Outcast: return 0.1f;
        default: return 0.5f;
    }
}

ESocialClass UMingSocialDynamicsSystem::CalculateNewSocialClass(const FSocialIndividual& Individual) const
{
    float MobilityScore = Individual.EconomicStatus + Individual.EducationLevel + Individual.SocialInfluence;
    MobilityScore /= 3.0f;
    
    if (MobilityScore > 0.8f) return ESocialClass::Upper;
    if (MobilityScore > 0.6f) return ESocialClass::UpperMiddle;
    if (MobilityScore > 0.4f) return ESocialClass::Middle;
    if (MobilityScore > 0.2f) return ESocialClass::LowerMiddle;
    if (MobilityScore > 0.1f) return ESocialClass::Lower;
    return ESocialClass::Outcast;
}

ESocialClass UMingSocialDynamicsSystem::PromoteSocialClass(ESocialClass CurrentClass) const
{
    int32 ClassValue = static_cast<int32>(CurrentClass);
    if (ClassValue > 0)
    {
        return static_cast<ESocialClass>(ClassValue - 1);
    }
    return CurrentClass;
}

ESocialClass UMingSocialDynamicsSystem::DemoteSocialClass(ESocialClass CurrentClass) const
{
    int32 ClassValue = static_cast<int32>(CurrentClass);
    if (ClassValue < static_cast<int32>(ESocialClass::Outcast))
    {
        return static_cast<ESocialClass>(ClassValue + 1);
    }
    return CurrentClass;
}

float UMingSocialDynamicsSystem::CalculateClassCompatibility(ESocialClass ClassA, ESocialClass ClassB) const
{
    int32 Diff = FMath::Abs(static_cast<int32>(ClassA) - static_cast<int32>(ClassB));
    
    // Classes closer together have higher compatibility
    switch (Diff)
    {
        case 0: return 1.0f;  // Same class
        case 1: return 0.8f;  // Adjacent classes
        case 2: return 0.6f;
        case 3: return 0.4f;
        case 4: return 0.2f;
        default: return 0.1f;
    }
}

void UMingSocialDynamicsSystem::SpreadVerticalTransmission(FCulturalTrait& Trait)
{
    // Parent to child transmission (simplified)
    TArray<FString> NewAdopters;
    
    for (const FString& AdopterID : Trait.Adopters)
    {
        if (Individuals.Contains(AdopterID))
        {
            // Find potential "children" (younger individuals with relationships)
            const FSocialIndividual& Adopter = Individuals[AdopterID];
            
            for (const FSocialRelationship& Rel : Adopter.Relationships)
            {
                if (Rel.RelationType == ESocialRelationType::Family)
                {
                    FString ChildID = (Rel.IndividualA == AdopterID) ? Rel.IndividualB : Rel.IndividualA;
                    
                    if (!Trait.Adopters.Contains(ChildID) && FMath::FRand() < Trait.AdoptionRate)
                    {
                        NewAdopters.Add(ChildID);
                    }
                }
            }
        }
    }
    
    Trait.Adopters.Append(NewAdopters);
}

void UMingSocialDynamicsSystem::SpreadHorizontalTransmission(FCulturalTrait& Trait)
{
    // Peer to peer transmission
    TArray<FString> NewAdopters;
    
    for (const FString& AdopterID : Trait.Adopters)
    {
        TArray<FSocialRelationship> Relationships = GetIndividualRelationships(AdopterID);
        
        for (const FSocialRelationship& Rel : Relationships)
        {
            FString PeerID = (Rel.IndividualA == AdopterID) ? Rel.IndividualB : Rel.IndividualA;
            
            if (!Trait.Adopters.Contains(PeerID) && FMath::FRand() < Trait.AdoptionRate * Rel.RelationshipStrength)
            {
                NewAdopters.Add(PeerID);
            }
        }
    }
    
    Trait.Adopters.Append(NewAdopters);
}

void UMingSocialDynamicsSystem::SpreadObliqueTransmission(FCulturalTrait& Trait)
{
    // Non-parental adult transmission (simplified as similar to horizontal)
    SpreadHorizontalTransmission(Trait);
}

void UMingSocialDynamicsSystem::SpreadMassMediaTransmission(FCulturalTrait& Trait)
{
    // Mass media affects random individuals
    int32 TargetCount = FMath::Min(10, Individuals.Num() - Trait.Adopters.Num());
    
    for (int32 i = 0; i < TargetCount; ++i)
    {
        TArray<FString> NonAdopters;
        
        for (const auto& IndividualPair : Individuals)
        {
            if (!Trait.Adopters.Contains(IndividualPair.Key))
            {
                NonAdopters.Add(IndividualPair.Key);
            }
        }
        
        if (NonAdopters.Num() > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, NonAdopters.Num() - 1);
            if (FMath::FRand() < Trait.AdoptionRate * 0.5f) // Reduced rate for mass media
            {
                Trait.Adopters.Add(NonAdopters[RandomIndex]);
            }
        }
    }
}

void UMingSocialDynamicsSystem::SpreadEducationTransmission(FCulturalTrait& Trait)
{
    // Education-based transmission (similar to vertical but with teachers)
    SpreadVerticalTransmission(Trait);
}

void UMingSocialDynamicsSystem::SpreadReligiousTransmission(FCulturalTrait& Trait)
{
    // Religious transmission through religious relationships
    TArray<FString> NewAdopters;
    
    for (const FString& AdopterID : Trait.Adopters)
    {
        TArray<FSocialRelationship> Relationships = GetIndividualRelationships(AdopterID);
        
        for (const FSocialRelationship& Rel : Relationships)
        {
            if (Rel.RelationType == ESocialRelationType::Religious)
            {
                FString ConvertID = (Rel.IndividualA == AdopterID) ? Rel.IndividualB : Rel.IndividualA;
                
                if (!Trait.Adopters.Contains(ConvertID) && FMath::FRand() < Trait.AdoptionRate * Rel.TrustLevel)
                {
                    NewAdopters.Add(ConvertID);
                }
            }
        }
    }
    
    Trait.Adopters.Append(NewAdopters);
}

void UMingSocialDynamicsSystem::SpreadTechnologyTransmission(FCulturalTrait& Trait)
{
    // Technology transmission through professional relationships
    TArray<FString> NewAdopters;
    
    for (const FString& AdopterID : Trait.Adopters)
    {
        TArray<FSocialRelationship> Relationships = GetIndividualRelationships(AdopterID);
        
        for (const FSocialRelationship& Rel : Relationships)
        {
            if (Rel.RelationType == ESocialRelationType::Professional)
            {
                FString ColleagueID = (Rel.IndividualA == AdopterID) ? Rel.IndividualB : Rel.IndividualA;
                
                if (!Trait.Adopters.Contains(ColleagueID) && FMath::FRand() < Trait.AdoptionRate * Rel.InfluenceLevel)
                {
                    NewAdopters.Add(ColleagueID);
                }
            }
        }
    }
    
    Trait.Adopters.Append(NewAdopters);
}

// Simplified implementations for complex network analysis functions

float UMingSocialDynamicsSystem::CalculateAveragePathLength() const
{
    // Simplified calculation - in reality would use Floyd-Warshall or similar
    if (Individuals.Num() < 2) return 0.0f;
    
    return 2.5f; // Placeholder value
}

float UMingSocialDynamicsSystem::CalculateClusteringCoefficient() const
{
    // Simplified clustering coefficient calculation
    if (Relationships.Num() == 0) return 0.0f;
    
    return 0.3f; // Placeholder value
}

int32 UMingSocialDynamicsSystem::CalculateConnectedComponents() const
{
    // Simplified connected components calculation
    if (Individuals.Num() == 0) return 0;
    
    return 1; // Assume mostly connected for simplicity
}

float UMingSocialDynamicsSystem::CalculateModularity() const
{
    // Simplified modularity calculation
    return 0.4f; // Placeholder value
}

void UMingSocialDynamicsSystem::CalculateNodeCentrality(FSocialNetworkMetrics& Metrics) const
{
    // Simplified centrality calculation
    for (const auto& IndividualPair : Individuals)
    {
        float Centrality = CalculateSocialInfluence(IndividualPair.Key);
        Metrics.NodeCentrality.Add(IndividualPair.Key, Centrality);
    }
}

TArray<TArray<FString>> UMingSocialDynamicsSystem::IdentifySocialGroups() const
{
    // Simplified group identification using connected components
    TArray<TArray<FString>> Groups;
    
    // For simplicity, create one group with all individuals
    if (Individuals.Num() > 0)
    {
        TArray<FString> AllIndividuals;
        for (const auto& IndividualPair : Individuals)
        {
            AllIndividuals.Add(IndividualPair.Key);
        }
        Groups.Add(AllIndividuals);
    }
    
    return Groups;
}

void UMingSocialDynamicsSystem::SimulateGroupDecision(const TArray<FString>& Group)
{
    // Simplified group decision simulation
    if (Group.Num() == 0) return;
    
    // Calculate average influence
    float TotalInfluence = 0.0f;
    for (const FString& IndividualID : Group)
    {
        TotalInfluence += CalculateSocialInfluence(IndividualID);
    }
    
    float AverageInfluence = TotalInfluence / Group.Num();
    
    UE_LOG(LogTemp, Log, TEXT("Group decision simulated for %d individuals, average influence: %.2f"), 
        Group.Num(), AverageInfluence);
}

void UMingSocialDynamicsSystem::SimulateGroupCohesion(const TArray<FString>& Group)
{
    // Simplified group cohesion simulation
    float GroupCohesion = 0.0f;
    int32 RelationshipCount = 0;
    
    for (int32 i = 0; i < Group.Num(); ++i)
    {
        for (int32 j = i + 1; j < Group.Num(); ++j)
        {
            TArray<FSocialRelationship> Relationships = GetIndividualRelationships(Group[i]);
            
            for (const FSocialRelationship& Rel : Relationships)
            {
                if (Rel.IndividualB == Group[j])
                {
                    GroupCohesion += Rel.RelationshipStrength;
                    RelationshipCount++;
                    break;
                }
            }
        }
    }
    
    if (RelationshipCount > 0)
    {
        GroupCohesion /= RelationshipCount;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Group cohesion simulated: %.2f"), GroupCohesion);
}
