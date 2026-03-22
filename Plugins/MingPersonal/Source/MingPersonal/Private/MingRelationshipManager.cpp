#include "MingRelationshipManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

UMingRelationshipManager::UMingRelationshipManager()
{
    CurrentGameDay = 1;
}

void UMingRelationshipManager::InitializeRelationshipSystem()
{
    // 初始化系統
    Relationships.Empty();
    Reputations.Empty();
    
    // 加載保存的數據
    LoadRelationshipData();
    
    UE_LOG(LogTemp, Warning, TEXT("Relationship and Reputation System Initialized"));
}

FRelationshipData UMingRelationshipManager::GetRelationship(const FString& CharacterID) const
{
    if (const FRelationshipData* Relationship = Relationships.Find(CharacterID))
    {
        return *Relationship;
    }
    
    // 返回默認關係數據
    FRelationshipData DefaultRelationship;
    DefaultRelationship.CharacterID = CharacterID;
    return DefaultRelationship;
}

void UMingRelationshipManager::UpdateRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason)
{
    float OldValue = 0.0f;
    
    if (FRelationshipData* ExistingRelationship = Relationships.Find(CharacterID))
    {
        OldValue = ExistingRelationship->RelationshipValue;
        ExistingRelationship->RelationshipValue = FMath::Clamp(ExistingRelationship->RelationshipValue + ChangeAmount, -100.0f, 100.0f);
        ExistingRelationship->RelationshipType = CalculateRelationshipType(ExistingRelationship->RelationshipValue);
        ExistingRelationship->LastInteractionDay = CurrentGameDay;
    }
    else
    {
        // 創建新的關係記錄
        FRelationshipData NewRelationship;
        NewRelationship.CharacterID = CharacterID;
        NewRelationship.RelationshipValue = FMath::Clamp(ChangeAmount, -100.0f, 100.0f);
        NewRelationship.RelationshipType = CalculateRelationshipType(NewRelationship.RelationshipValue);
        NewRelationship.LastInteractionDay = CurrentGameDay;
        Relationships.Add(CharacterID, NewRelationship);
        
        OldValue = 0.0f;
    }
    
    // 觸發回調
    OnRelationshipChanged(CharacterID, OldValue, Relationships[CharacterID].RelationshipValue, Reason);
    
    // 保存數據
    SaveRelationshipData();
    
    UE_LOG(LogTemp, Warning, TEXT("Relationship updated for %s: %.1f (%s)"), 
           *CharacterID, Relationships[CharacterID].RelationshipValue, *Reason);
}

FReputationData UMingRelationshipManager::GetReputation(const FString& RegionID) const
{
    if (const FReputationData* Reputation = Reputations.Find(RegionID))
    {
        return *Reputation;
    }
    
    // 返回默認聲望數據
    FReputationData DefaultReputation;
    DefaultReputation.RegionID = RegionID;
    return DefaultReputation;
}

void UMingRelationshipManager::UpdateReputation(const FString& RegionID, float ChangeAmount, const FString& Reason)
{
    float OldValue = 0.0f;
    
    if (FReputationData* ExistingReputation = Reputations.Find(RegionID))
    {
        OldValue = ExistingReputation->ReputationScore;
        ExistingReputation->ReputationScore = FMath::Clamp(ExistingReputation->ReputationScore + ChangeAmount, 0.0f, 1000.0f);
        ExistingReputation->ReputationLevel = CalculateReputationLevel(ExistingReputation->ReputationScore);
    }
    else
    {
        // 創建新的聲望記錄
        FReputationData NewReputation;
        NewReputation.RegionID = RegionID;
        NewReputation.ReputationScore = FMath::Clamp(ChangeAmount, 0.0f, 1000.0f);
        NewReputation.ReputationLevel = CalculateReputationLevel(NewReputation.ReputationScore);
        Reputations.Add(RegionID, NewReputation);
        
        OldValue = 0.0f;
    }
    
    // 觸發回調
    OnReputationChanged(RegionID, OldValue, Reputations[RegionID].ReputationScore, Reason);
    
    // 保存數據
    SaveRelationshipData();
    
    UE_LOG(LogTemp, Warning, TEXT("Reputation updated for %s: %.1f (%s)"), 
           *RegionID, Reputations[RegionID].ReputationScore, *Reason);
}

void UMingRelationshipManager::ProcessInteraction(const FInteractionEffect& InteractionEffect)
{
    // 更新角色關係
    if (!InteractionEffect.CharacterID.IsEmpty())
    {
        UpdateRelationship(InteractionEffect.CharacterID, InteractionEffect.RelationshipChange, InteractionEffect.Reason);
    }
    
    // 更新地區聲望
    if (!InteractionEffect.RegionID.IsEmpty())
    {
        UpdateReputation(InteractionEffect.RegionID, InteractionEffect.ReputationChange, InteractionEffect.Reason);
    }
}

bool UMingRelationshipManager::CanAcceptQuest(const FString& QuestID, const FString& RegionID) const
{
    FReputationData RegionReputation = GetReputation(RegionID);
    
    // 根據聲望等級判斷是否可以接取任務
    switch (RegionReputation.ReputationLevel)
    {
        case EReputationLevel::Legendary:
            return true; // 傳奇聲望可以接取所有任務
        case EReputationLevel::Honored:
            return true; // 受敬重可以接取高級任務
        case EReputationLevel::Respected:
            return true; // 受尊敬可以接取中級任務
        case EReputationLevel::Neutral:
            return true; // 中立可以接取基礎任務
        case EReputationLevel::Disliked:
            return false; // 被厭惡無法接取任務
        case EReputationLevel::Unknown:
        default:
            return false; // 未知聲望無法接取任務
    }
}

TArray<FString> UMingRelationshipManager::GetDialogueOptions(const FString& CharacterID) const
{
    TArray<FString> DialogueOptions;
    FRelationshipData Relationship = GetRelationship(CharacterID);
    
    // 根據關係類型和數值提供不同的對話選項
    switch (Relationship.RelationshipType)
    {
        case ERelationshipType::Friend:
        case ERelationshipType::Ally:
            DialogueOptions.Add(TEXT("友好問候"));
            DialogueOptions.Add(TEXT("分享情報"));
            DialogueOptions.Add(TEXT("請求幫助"));
            DialogueOptions.Add(TEXT("私人對話"));
            break;
            
        case ERelationshipType::Family:
            DialogueOptions.Add(TEXT("家庭關懷"));
            DialogueOptions.Add(TEXT("家族事務"));
            DialogueOptions.Add(TEXT("親密對話"));
            break;
            
        case ERelationshipType::Mentor:
            DialogueOptions.Add(TEXT("請教指導"));
            DialogueOptions.Add(TEXT("學習技能"));
            DialogueOptions.Add(TEXT("尋求建議"));
            break;
            
        case ERelationshipType::Business:
            DialogueOptions.Add(TEXT("商業談判"));
            DialogueOptions.Add(TEXT("交易提議"));
            DialogueOptions.Add(TEXT("合作機會"));
            break;
            
        case ERelationshipType::Romantic:
            DialogueOptions.Add(TEXT("浪漫問候"));
            DialogueOptions.Add(TEXT("約會邀請"));
            DialogueOptions.Add(TEXT("情感表達"));
            break;
            
        case ERelationshipType::Rival:
            DialogueOptions.Add(TEXT("競爭對話"));
            DialogueOptions.Add(TEXT("挑戰宣言"));
            DialogueOptions.Add(TEXT("相互尊重"));
            break;
            
        case ERelationshipType::Enemy:
            DialogueOptions.Add(TEXT("威脅對話"));
            DialogueOptions.Add(TEXT("敵對宣言"));
            DialogueOptions.Add(TEXT("谈判可能"));
            break;
            
        case ERelationshipType::Neutral:
        default:
            DialogueOptions.Add(TEXT("基本問候"));
            DialogueOptions.Add(TEXT("一般對話"));
            DialogueOptions.Add(TEXT("詢問信息"));
            break;
    }
    
    return DialogueOptions;
}

float UMingRelationshipManager::GetPriceModifier(const FString& CharacterID) const
{
    FRelationshipData Relationship = GetRelationship(CharacterID);
    
    // 根據關係數值計算價格修正
    if (Relationship.RelationshipValue >= 80.0f)
    {
        return 0.8f; // 20% 折扣
    }
    else if (Relationship.RelationshipValue >= 50.0f)
    {
        return 0.9f; // 10% 折扣
    }
    else if (Relationship.RelationshipValue >= 20.0f)
    {
        return 0.95f; // 5% 折扣
    }
    else if (Relationship.RelationshipValue <= -50.0f)
    {
        return 1.2f; // 20% 加價
    }
    else if (Relationship.RelationshipValue <= -20.0f)
    {
        return 1.1f; // 10% 加價
    }
    
    return 1.0f; // 無修正
}

ERelationshipType UMingRelationshipManager::GetRelationshipLevel(const FString& CharacterID) const
{
    FRelationshipData Relationship = GetRelationship(CharacterID);
    return Relationship.RelationshipType;
}

EReputationLevel UMingRelationshipManager::GetReputationLevel(const FString& RegionID) const
{
    FReputationData Reputation = GetReputation(RegionID);
    return Reputation.ReputationLevel;
}

void UMingRelationshipManager::AddSharedMemory(const FString& CharacterID, const FString& Memory)
{
    if (FRelationshipData* Relationship = Relationships.Find(CharacterID))
    {
        Relationship->SharedMemories.Add(Memory);
        Relationship->LastInteractionDay = CurrentGameDay;
        
        // 共同記憶會略微提升關係
        UpdateRelationship(CharacterID, 2.0f, FString::Printf(TEXT("共同記憶: %s"), *Memory));
    }
}

ERelationshipType UMingRelationshipManager::CalculateRelationshipType(float RelationshipValue) const
{
    if (RelationshipValue >= 75.0f)
    {
        return ERelationshipType::Friend;
    }
    else if (RelationshipValue >= 50.0f)
    {
        return ERelationshipType::Ally;
    }
    else if (RelationshipValue >= 25.0f)
    {
        return ERelationshipType::Neutral;
    }
    else if (RelationshipValue >= -25.0f)
    {
        return ERelationshipType::Neutral;
    }
    else if (RelationshipValue >= -50.0f)
    {
        return ERelationshipType::Rival;
    }
    else
    {
        return ERelationshipType::Enemy;
    }
}

EReputationLevel UMingRelationshipManager::CalculateReputationLevel(float ReputationScore) const
{
    if (ReputationScore >= 800.0f)
    {
        return EReputationLevel::Legendary;
    }
    else if (ReputationScore >= 600.0f)
    {
        return EReputationLevel::Honored;
    }
    else if (ReputationScore >= 400.0f)
    {
        return EReputationLevel::Respected;
    }
    else if (ReputationScore >= 200.0f)
    {
        return EReputationLevel::Neutral;
    }
    else if (ReputationScore >= 50.0f)
    {
        return EReputationLevel::Disliked;
    }
    else
    {
        return EReputationLevel::Unknown;
    }
}

void UMingRelationshipManager::ApplyRelationshipDecay()
{
    // 長時間無互動的關係會逐漸衰減
    for (auto& RelationshipPair : Relationships)
    {
        FRelationshipData& Relationship = RelationshipPair.Value;
        int32 DaysSinceInteraction = CurrentGameDay - Relationship.LastInteractionDay;
        
        if (DaysSinceInteraction > 30) // 30天無互動開始衰減
        {
            float DecayAmount = (DaysSinceInteraction - 30) * 0.1f; // 每10天衰減1點
            Relationship.RelationshipValue = FMath::Max(Relationship.RelationshipValue - DecayAmount, -100.0f);
            Relationship.RelationshipType = CalculateRelationshipType(Relationship.RelationshipValue);
        }
    }
}

void UMingRelationshipManager::SaveRelationshipData()
{
    // TODO: 實現數據保存邏輯
    // 可以保存到遊戲存檔或專用文件
}

void UMingRelationshipManager::LoadRelationshipData()
{
    // TODO: 實現數據加載邏輯
    // 從遊戲存檔或專用文件加載
}
