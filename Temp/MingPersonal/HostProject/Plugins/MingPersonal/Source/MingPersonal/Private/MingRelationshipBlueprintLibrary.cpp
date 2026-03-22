#include "MingRelationshipBlueprintLibrary.h"
#include "MingPersonalManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

UMingRelationshipManager* UMingRelationshipBlueprintLibrary::GetRelationshipManager()
{
    // 嘗試從遊戲實例獲取 PersonalManager
    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            // 這裡可以通過 GameInstance 或其他方式獲取 PersonalManager
            // 暫時返回 nullptr，實際使用時需要根據具體架構調整
            return nullptr;
        }
    }
    
    // 如果無法獲取，創建一個臨時實例
    return NewObject<UMingRelationshipManager>();
}

void UMingRelationshipBlueprintLibrary::UpdateCharacterRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        Manager->UpdateRelationship(CharacterID, ChangeAmount, Reason);
    }
}

FRelationshipData UMingRelationshipBlueprintLibrary::GetCharacterRelationship(const FString& CharacterID)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        return Manager->GetRelationship(CharacterID);
    }
    return FRelationshipData();
}

ERelationshipType UMingRelationshipBlueprintLibrary::GetRelationshipType(const FString& CharacterID)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        return Manager->GetRelationshipLevel(CharacterID);
    }
    return ERelationshipType::Neutral;
}

float UMingRelationshipBlueprintLibrary::GetRelationshipValue(const FString& CharacterID)
{
    FRelationshipData Relationship = GetCharacterRelationship(CharacterID);
    return Relationship.RelationshipValue;
}

void UMingRelationshipBlueprintLibrary::AddSharedMemory(const FString& CharacterID, const FString& Memory)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        Manager->AddSharedMemory(CharacterID, Memory);
    }
}

TArray<FString> UMingRelationshipBlueprintLibrary::GetSharedMemories(const FString& CharacterID)
{
    FRelationshipData Relationship = GetCharacterRelationship(CharacterID);
    return Relationship.SharedMemories;
}

void UMingRelationshipBlueprintLibrary::UpdateRegionReputation(const FString& RegionID, float ChangeAmount, const FString& Reason)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        Manager->UpdateReputation(RegionID, ChangeAmount, Reason);
    }
}

FReputationData UMingRelationshipBlueprintLibrary::GetRegionReputation(const FString& RegionID)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        return Manager->GetReputation(RegionID);
    }
    return FReputationData();
}

EReputationLevel UMingRelationshipBlueprintLibrary::GetReputationLevel(const FString& RegionID)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        return Manager->GetReputationLevel(RegionID);
    }
    return EReputationLevel::Unknown;
}

float UMingRelationshipBlueprintLibrary::GetReputationScore(const FString& RegionID)
{
    FReputationData Reputation = GetRegionReputation(RegionID);
    return Reputation.ReputationScore;
}

TArray<FString> UMingRelationshipBlueprintLibrary::GetDialogueOptions(const FString& CharacterID)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        return Manager->GetDialogueOptions(CharacterID);
    }
    return TArray<FString>();
}

float UMingRelationshipBlueprintLibrary::GetPriceModifier(const FString& CharacterID)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        return Manager->GetPriceModifier(CharacterID);
    }
    return 1.0f;
}

bool UMingRelationshipBlueprintLibrary::CanAcceptQuest(const FString& QuestID, const FString& RegionID)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        return Manager->CanAcceptQuest(QuestID, RegionID);
    }
    return false;
}

void UMingRelationshipBlueprintLibrary::ProcessInteractionEffect(const FString& CharacterID, const FString& RegionID, 
                                                                float RelationshipChange, float ReputationChange, const FString& Reason)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        FInteractionEffect Effect;
        Effect.CharacterID = CharacterID;
        Effect.RegionID = RegionID;
        Effect.RelationshipChange = RelationshipChange;
        Effect.ReputationChange = ReputationChange;
        Effect.Reason = Reason;
        
        Manager->ProcessInteraction(Effect);
    }
}

void UMingRelationshipBlueprintLibrary::ProcessBatchInteractions(const TArray<FInteractionEffect>& Interactions)
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        for (const FInteractionEffect& Effect : Interactions)
        {
            Manager->ProcessInteraction(Effect);
        }
    }
}

bool UMingRelationshipBlueprintLibrary::IsFriend(const FString& CharacterID)
{
    ERelationshipType Type = GetRelationshipType(CharacterID);
    return Type == ERelationshipType::Friend || Type == ERelationshipType::Ally;
}

bool UMingRelationshipBlueprintLibrary::IsEnemy(const FString& CharacterID)
{
    ERelationshipType Type = GetRelationshipType(CharacterID);
    return Type == ERelationshipType::Enemy || Type == ERelationshipType::Rival;
}

bool UMingRelationshipBlueprintLibrary::IsNeutral(const FString& CharacterID)
{
    ERelationshipType Type = GetRelationshipType(CharacterID);
    return Type == ERelationshipType::Neutral;
}

bool UMingRelationshipBlueprintLibrary::HasGoodReputation(const FString& RegionID)
{
    EReputationLevel Level = GetReputationLevel(RegionID);
    return Level == EReputationLevel::Respected || Level == EReputationLevel::Honored || Level == EReputationLevel::Legendary;
}

bool UMingRelationshipBlueprintLibrary::HasBadReputation(const FString& RegionID)
{
    EReputationLevel Level = GetReputationLevel(RegionID);
    return Level == EReputationLevel::Disliked || Level == EReputationLevel::Unknown;
}

FString UMingRelationshipBlueprintLibrary::GetRelationshipTypeString(ERelationshipType RelationshipType)
{
    switch (RelationshipType)
    {
        case ERelationshipType::Friend: return TEXT("朋友");
        case ERelationshipType::Ally: return TEXT("盟友");
        case ERelationshipType::Rival: return TEXT("競爭對手");
        case ERelationshipType::Enemy: return TEXT("敵人");
        case ERelationshipType::Family: return TEXT("家人");
        case ERelationshipType::Mentor: return TEXT("導師");
        case ERelationshipType::Student: return TEXT("學生");
        case ERelationshipType::Business: return TEXT("商業夥伴");
        case ERelationshipType::Romantic: return TEXT("戀愛對象");
        case ERelationshipType::Neutral: 
        default: return TEXT("中立");
    }
}

FString UMingRelationshipBlueprintLibrary::GetReputationLevelString(EReputationLevel ReputationLevel)
{
    switch (ReputationLevel)
    {
        case EReputationLevel::Unknown: return TEXT("未知");
        case EReputationLevel::Disliked: return TEXT("被厭惡");
        case EReputationLevel::Neutral: return TEXT("中立");
        case EReputationLevel::Respected: return TEXT("受尊敬");
        case EReputationLevel::Honored: return TEXT("受敬重");
        case EReputationLevel::Legendary: return TEXT("傳奇");
        default: return TEXT("未知");
    }
}

FString UMingRelationshipBlueprintLibrary::GetRelationshipDescription(const FString& CharacterID)
{
    FRelationshipData Relationship = GetCharacterRelationship(CharacterID);
    FString TypeString = GetRelationshipTypeString(Relationship.RelationshipType);
    return FString::Printf(TEXT("%s (%.1f)"), *TypeString, Relationship.RelationshipValue);
}

FString UMingRelationshipBlueprintLibrary::GetReputationDescription(const FString& RegionID)
{
    FReputationData Reputation = GetRegionReputation(RegionID);
    FString LevelString = GetReputationLevelString(Reputation.ReputationLevel);
    return FString::Printf(TEXT("%s (%.0f)"), *LevelString, Reputation.ReputationScore);
}

int32 UMingRelationshipBlueprintLibrary::GetTotalRelationshipCount()
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        return Manager->GetAllRelationships().Num();
    }
    return 0;
}

int32 UMingRelationshipBlueprintLibrary::GetFriendCount()
{
    return GetCharactersByRelationshipType(ERelationshipType::Friend).Num() + 
           GetCharactersByRelationshipType(ERelationshipType::Ally).Num();
}

int32 UMingRelationshipBlueprintLibrary::GetEnemyCount()
{
    return GetCharactersByRelationshipType(ERelationshipType::Enemy).Num() + 
           GetCharactersByRelationshipType(ERelationshipType::Rival).Num();
}

int32 UMingRelationshipBlueprintLibrary::GetKnownRegionCount()
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        return Manager->GetAllReputations().Num();
    }
    return 0;
}

TArray<FString> UMingRelationshipBlueprintLibrary::GetAllKnownRegions()
{
    TArray<FString> Regions;
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        const TMap<FString, FReputationData>& Reputations = Manager->GetAllReputations();
        for (const auto& ReputationPair : Reputations)
        {
            Regions.Add(ReputationPair.Key);
        }
    }
    return Regions;
}

TArray<FString> UMingRelationshipBlueprintLibrary::GetCharactersByRelationshipType(ERelationshipType RelationshipType)
{
    TArray<FString> Characters;
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        const TMap<FString, FRelationshipData>& Relationships = Manager->GetAllRelationships();
        for (const auto& RelationshipPair : Relationships)
        {
            if (RelationshipPair.Value.RelationshipType == RelationshipType)
            {
                Characters.Add(RelationshipPair.Key);
            }
        }
    }
    return Characters;
}

TArray<FString> UMingRelationshipBlueprintLibrary::GetCharactersByRelationshipRange(float MinValue, float MaxValue)
{
    TArray<FString> Characters;
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        const TMap<FString, FRelationshipData>& Relationships = Manager->GetAllRelationships();
        for (const auto& RelationshipPair : Relationships)
        {
            float Value = RelationshipPair.Value.RelationshipValue;
            if (Value >= MinValue && Value <= MaxValue)
            {
                Characters.Add(RelationshipPair.Key);
            }
        }
    }
    return Characters;
}

TArray<FString> UMingRelationshipBlueprintLibrary::GetRegionsByReputationLevel(EReputationLevel ReputationLevel)
{
    TArray<FString> Regions;
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        const TMap<FString, FReputationData>& Reputations = Manager->GetAllReputations();
        for (const auto& ReputationPair : Reputations)
        {
            if (ReputationPair.Value.ReputationLevel == ReputationLevel)
            {
                Regions.Add(ReputationPair.Key);
            }
        }
    }
    return Regions;
}

TArray<FString> UMingRelationshipBlueprintLibrary::GetRegionsByReputationRange(float MinScore, float MaxScore)
{
    TArray<FString> Regions;
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        const TMap<FString, FReputationData>& Reputations = Manager->GetAllReputations();
        for (const auto& ReputationPair : Reputations)
        {
            float Score = ReputationPair.Value.ReputationScore;
            if (Score >= MinScore && Score <= MaxScore)
            {
                Regions.Add(ReputationPair.Key);
            }
        }
    }
    return Regions;
}

bool UMingRelationshipBlueprintLibrary::IsValidCharacterID(const FString& CharacterID)
{
    return !CharacterID.IsEmpty() && CharacterID.Len() > 0;
}

bool UMingRelationshipBlueprintLibrary::IsValidRegionID(const FString& RegionID)
{
    return !RegionID.IsEmpty() && RegionID.Len() > 0;
}

void UMingRelationshipBlueprintLibrary::LogAllRelationships()
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        UE_LOG(LogTemp, Warning, TEXT("=== 所有角色關係 ==="));
        const TMap<FString, FRelationshipData>& Relationships = Manager->GetAllRelationships();
        for (const auto& RelationshipPair : Relationships)
        {
            FString Description = GetRelationshipDescription(RelationshipPair.Key);
            UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *RelationshipPair.Key, *Description);
        }
        UE_LOG(LogTemp, Warning, TEXT("=== 關係列表結束 ==="));
    }
}

void UMingRelationshipBlueprintLibrary::LogAllReputations()
{
    if (UMingRelationshipManager* Manager = GetRelationshipManager())
    {
        UE_LOG(LogTemp, Warning, TEXT("=== 所有地區聲望 ==="));
        const TMap<FString, FReputationData>& Reputations = Manager->GetAllReputations();
        for (const auto& ReputationPair : Reputations)
        {
            FString Description = GetReputationDescription(ReputationPair.Key);
            UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *ReputationPair.Key, *Description);
        }
        UE_LOG(LogTemp, Warning, TEXT("=== 聲望列表結束 ==="));
    }
}

void UMingRelationshipBlueprintLibrary::LogRelationshipStatus(const FString& CharacterID)
{
    FString Description = GetRelationshipDescription(CharacterID);
    UE_LOG(LogTemp, Warning, TEXT("角色關係狀態: %s - %s"), *CharacterID, *Description);
}

void UMingRelationshipBlueprintLibrary::LogReputationStatus(const FString& RegionID)
{
    FString Description = GetReputationDescription(RegionID);
    UE_LOG(LogTemp, Warning, TEXT("地區聲望狀態: %s - %s"), *RegionID, *Description);
}
