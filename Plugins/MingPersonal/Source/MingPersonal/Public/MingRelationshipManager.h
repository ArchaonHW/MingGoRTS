#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingRelationshipManager.generated.h"

UENUM(BlueprintType)
enum class ERelationshipType : uint8
{
    Friend,           // ????
    Ally,            // ????
    Rival,           // 競爭對??
    Enemy,           // ??人
    Family,          // 家人
    Mentor,          // 導師
    Student,         // 學??
    Business,        // ??業夥伴
    Romantic,        // ????????    Neutral          // 中??
};

UENUM(BlueprintType)
enum class EReputationLevel : uint8
{
    Unknown,         // ??知
    Disliked,        // 被厭X    Neutral,         // 中??
    Respected,       // ???X    Honored,         // ??敬X    Legendary        // ????
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
        CharacterID = TEXT(""};
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
        RegionID = TEXT(""};
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
        CharacterID = TEXT(""};
        RelationshipChange = 0.0f;
        ReputationChange = 0.0f;
        RegionID = TEXT(""};
        Reason = TEXT(""};
    }
};

/**
 * 角色???X?聲??管??系?? * 管?X?家??NPC????係以??在??個地????聲X */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRelationshipManager(};

    // ???X?系??    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void InitializeRelationshipSystem();

    // ???X?特定?X??X???
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    FRelationshipData GetRelationship(const FString& CharacterID) const;

    // ??新角色????
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void UpdateRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason);

    // ???X??X???
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    FReputationData GetReputation(const FString& RegionID) const;

    // ??新???X???
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    void UpdateReputation(const FString& RegionID, float ChangeAmount, const FString& Reason);

    // ????互?X???
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void ProcessInteraction(const FInteractionEffect& InteractionEffect);

    // 檢查??否??以???X???任??（基??聲????
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    bool CanAcceptQuest(const FString& QuestID, const FString& RegionID) const;

    // ????對話????（基????係??
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    TArray<FString> GetDialogueOptions(const FString& CharacterID) const;

    // ???X?格修正（基????係??
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    float GetPriceModifier(const FString& CharacterID) const;

    // ???X???等??
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    ERelationshipType GetRelationshipLevel(const FString& CharacterID) const;

    // ???X???等??
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    EReputationLevel GetReputationLevel(const FString& RegionID) const;

    // 添?X???記憶
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void AddSharedMemory(const FString& CharacterID, const FString& Memory);

    // ???X?????係數X    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    const TMap<FString, FRelationshipData>& GetAllRelationships() const { return Relationships; }

    // ???X???聲??數X    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    const TMap<FString, FReputationData>& GetAllReputations() const { return Reputations; }

protected:
    // 角色???X???
    UPROPERTY(BlueprintReadOnly, Category = "Relationship System")
    TMap<FString, FRelationshipData> Relationships;

    // ???X??X???
    UPROPERTY(BlueprintReadOnly, Category = "Reputation System")
    TMap<FString, FReputationData> Reputations;

    // ???X?戲X    UPROPERTY(BlueprintReadOnly, Category = "Relationship System")
    int32 CurrentGameDay;

    // ????變?X?調
    UFUNCTION(BlueprintImplementableEvent, Category = "Relationship System")
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);

    // ????變?X?調
    UFUNCTION(BlueprintImplementableEvent, Category = "Reputation System")
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

private:
    // 計?X???類??
    ERelationshipType CalculateRelationshipType(float RelationshipValue) const;

    // 計?X???等??
    EReputationLevel CalculateReputationLevel(float ReputationScore) const;

    // ??用????衰??（長???X??X???
    void ApplyRelationshipDecay(};

    // 保??系統????
    void SaveRelationshipData(};

    // ????系統????
    void LoadRelationshipData(};
);

