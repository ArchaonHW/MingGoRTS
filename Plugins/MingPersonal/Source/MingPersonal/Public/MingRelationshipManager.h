#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingRelationshipManager.generated.h"

UENUM(BlueprintType)
enum class ERelationshipType : uint8
{
    Friend,           // ?ãÂ?
    Ally,            // ?üÂ?
    Rival,           // Á´∂Áà≠Â∞çÊ?
    Enemy,           // ?µ‰∫∫
    Family,          // ÂÆ∂‰∫∫
    Mentor,          // Â∞éÂ∏´
    Student,         // Â≠∏Á?
    Business,        // ?ÜÊ•≠Â§•‰º¥
    Romantic,        // ?Ä?õÂ?Ë±?    Neutral          // ‰∏≠Á?
};

UENUM(BlueprintType)
enum class EReputationLevel : uint8
{
    Unknown,         // ?™Áü•
    Disliked,        // Ë¢´Âé≠??    Neutral,         // ‰∏≠Á?
    Respected,       // ?óÂ???    Honored,         // ?óÊï¨??    Legendary        // ?≥Â?
};

USTRUCT(BlueprintType)
struct FRelationshipData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERelationshipType RelationshipType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RelationshipValue; // -100 to 100

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LastInteractionDay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SharedMemories;

    FRelationshipData()
    {
        CharacterID = TEXT("");
        RelationshipType = ERelationshipType::Neutral;
        RelationshipValue = 0.0f;
        LastInteractionDay = 0;
    }
};

USTRUCT(BlueprintType)
struct FReputationData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EReputationLevel ReputationLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationScore; // 0 to 1000

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FactionReputation; // Faction -> Score

    FReputationData()
    {
        RegionID = TEXT("");
        ReputationLevel = EReputationLevel::Unknown;
        ReputationScore = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FInteractionEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RelationshipChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Reason;

    FInteractionEffect()
    {
        CharacterID = TEXT("");
        RelationshipChange = 0.0f;
        ReputationChange = 0.0f;
        RegionID = TEXT("");
        Reason = TEXT("");
    }
};

/**
 * ËßíËâ≤?ú‰??åËÅ≤?õÁÆ°?ÜÁ≥ªÁµ? * ÁÆ°Á??©ÂÆ∂?áNPC?ÑÈ?‰øÇ‰ª•?äÂú®?ÑÂÄãÂú∞?Ä?ÑËÅ≤?? */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRelationshipManager();

    // ?ùÂ??ñÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void InitializeRelationshipSystem();

    // ?≤Â??áÁâπÂÆöË??≤Á??ú‰?
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    FRelationshipData GetRelationship(const FString& CharacterID) const;

    // ?¥Êñ∞ËßíËâ≤?ú‰?
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void UpdateRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason);

    // ?≤Â??∞Â??≤Ê?
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    FReputationData GetReputation(const FString& RegionID) const;

    // ?¥Êñ∞?∞Â??≤Ê?
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    void UpdateReputation(const FString& RegionID, float ChangeAmount, const FString& Reason);

    // ?ïÁ?‰∫íÂ??àÊ?
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void ProcessInteraction(const FInteractionEffect& InteractionEffect);

    // Ê™¢Êü•?ØÂê¶?Ø‰ª•?•Â??πÂ?‰ªªÂ?ÔºàÂü∫?ºËÅ≤?õÔ?
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    bool CanAcceptQuest(const FString& QuestID, const FString& RegionID) const;

    // ?≤Â?Â∞çË©±?∏È?ÔºàÂü∫?ºÈ?‰øÇÔ?
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    TArray<FString> GetDialogueOptions(const FString& CharacterID) const;

    // ?≤Â??πÊ†º‰øÆÊ≠£ÔºàÂü∫?ºÈ?‰øÇÔ?
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    float GetPriceModifier(const FString& CharacterID) const;

    // ?≤Â??ú‰?Á≠âÁ?
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    ERelationshipType GetRelationshipLevel(const FString& CharacterID) const;

    // ?≤Â??≤Ê?Á≠âÁ?
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    EReputationLevel GetReputationLevel(const FString& RegionID) const;

    // Ê∑ªÂ??±Â?Ë®òÊÜ∂
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void AddSharedMemory(const FString& CharacterID, const FString& Memory);

    // ?≤Â??Ä?âÈ?‰øÇÊï∏??    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    const TMap<FString, FRelationshipData>& GetAllRelationships() const { return Relationships; }

    // ?≤Â??Ä?âËÅ≤?õÊï∏??    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    const TMap<FString, FReputationData>& GetAllReputations() const { return Reputations; }

protected:
    // ËßíËâ≤?ú‰??∏Ê?
    UPROPERTY(BlueprintReadOnly, Category = "Relationship System")
    TMap<FString, FRelationshipData> Relationships;

    // ?∞Â??≤Ê??∏Ê?
    UPROPERTY(BlueprintReadOnly, Category = "Reputation System")
    TMap<FString, FReputationData> Reputations;

    // ?∂Â??äÊà≤??    UPROPERTY(BlueprintReadOnly, Category = "Relationship System")
    int32 CurrentGameDay;

    // ?ú‰?ËÆäÂ??ûË™ø
    UFUNCTION(BlueprintImplementableEvent, Category = "Relationship System")
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);

    // ?≤Ê?ËÆäÂ??ûË™ø
    UFUNCTION(BlueprintImplementableEvent, Category = "Reputation System")
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

private:
    // Ë®àÁ??ú‰?È°ûÂ?
    ERelationshipType CalculateRelationshipType(float RelationshipValue) const;

    // Ë®àÁ??≤Ê?Á≠âÁ?
    EReputationLevel CalculateReputationLevel(float ReputationScore) const;

    // ?âÁî®?ú‰?Ë°∞Ê?ÔºàÈï∑?ÇÈ??°‰??ïÔ?
    void ApplyRelationshipDecay();

    // ‰øùÂ?Á≥ªÁµ±?∏Ê?
    void SaveRelationshipData();

    // ?†Ë?Á≥ªÁµ±?∏Ê?
    void LoadRelationshipData();
};
