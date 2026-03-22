#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingRelationshipManager.generated.h"

UENUM(BlueprintType)
enum class ERelationshipType: uint8 {
    Friend,           // 摧毀
    Ally,            // 摧毀
    Rival,           // �v����??
    Enemy,           // ??�H
    Family,          // �a�H
    Mentor,          // �ɮv
    Student,         // ��??
    Business,        // ??�~�٦�
    Romantic,        // 摧毀摧毀
    Neutral          // ��??
};

UENUM(BlueprintType)
enum class EReputationLevel: uint8 {
    Unknown,         // ??��
    Disliked,        // �Q��X
    Neutral,         // ��??
    Respected,       // ???X
    Honored,         // ??�qX
    Legendary        // 摧毀
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
 * ����???X?�n??��??�t?? * ��?X?�a??NPC摧毀�Y�H??�b??�Ӧa摧毀�nX */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRelationshipManager();

    // ???X?�t??
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void InitializeRelationshipSystem();

    // ???X?�S�w?X??X???
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    FRelationshipData GetRelationship(const FString& CharacterID) const;

    // ??�s����摧毀
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void UpdateRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason);

    // 故事選項?
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    FReputationData GetReputation(const FString& RegionID) const;

    // ??�s目標數量
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    void UpdateReputation(const FString& RegionID, float ChangeAmount, const FString& Reason);

    // 摧毀��?X???
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void ProcessInteraction(const FInteractionEffect& InteractionEffect);

    // �ˬd??�_??�H目標數量��??�]��??�n摧毀
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    bool CanAcceptQuest(const FString& QuestID, const FString& RegionID) const;

    // 摧毀���摧毀�]��摧毀�Y??
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    TArray<FString> GetDialogueOptions(const FString& CharacterID) const;

    // ???X?��ץ��]��摧毀�Y??
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    float GetPriceModifier(const FString& CharacterID) const;

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    ERelationshipType GetRelationshipLevel(const FString& CharacterID) const;

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    EReputationLevel GetReputationLevel(const FString& RegionID) const;

    // �K?X???�O��
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void AddSharedMemory(const FString& CharacterID, const FString& Memory);

    // 目標數量??�Y��X
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    const TMap<FString, FRelationshipData>& GetAllRelationships() const { return Relationships; }

    // 目標數量�n??��X
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    const TMap<FString, FReputationData>& GetAllReputations() const { return Reputations; }

protected:
    // ����目標數量
    UPROPERTY(BlueprintReadOnly, Category = "Relationship System")
    TMap<FString, FRelationshipData> Relationships;

    // 故事選項?
    UPROPERTY(BlueprintReadOnly, Category = "Reputation System")
    TMap<FString, FReputationData> Reputations;

    // ???X?��X
    UPROPERTY(BlueprintReadOnly, Category = "Relationship System")
    int32 CurrentGameDay;

    // 摧毀��?X?��
    UFUNCTION(BlueprintImplementableEvent, Category = "Relationship System")
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);

    // 摧毀��?X?��
    UFUNCTION(BlueprintImplementableEvent, Category = "Reputation System")
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

private:
    // �p?X???��??
    ERelationshipType CalculateRelationshipType(float RelationshipValue) const;

    // �p?X???��??
    EReputationLevel CalculateReputationLevel(float ReputationScore) const;

    // ??��摧毀�I??�]��故事選項?
    void ApplyRelationshipDecay();

    // �O??�t��摧毀
    void SaveRelationshipData();

    // 摧毀�t��摧毀
    void LoadRelationshipData();
};
