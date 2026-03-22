#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingRelationshipManager.generated.h"

UENUM(BlueprintType)
enum class ERelationshipType : uint8
{
    Friend,           // 朋友
    Ally,            // 盟友
    Rival,           // 競爭對手
    Enemy,           // 敵人
    Family,          // 家人
    Mentor,          // 導師
    Student,         // 學生
    Business,        // 商業夥伴
    Romantic,        // 戀愛對象
    Neutral          // 中立
};

UENUM(BlueprintType)
enum class EReputationLevel : uint8
{
    Unknown,         // 未知
    Disliked,        // 被厭惡
    Neutral,         // 中立
    Respected,       // 受尊敬
    Honored,         // 受敬重
    Legendary        // 傳奇
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
 * 角色關係和聲望管理系統
 * 管理玩家與NPC的關係以及在各個地區的聲望
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRelationshipManager();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void InitializeRelationshipSystem();

    // 獲取與特定角色的關係
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    FRelationshipData GetRelationship(const FString& CharacterID) const;

    // 更新角色關係
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void UpdateRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason);

    // 獲取地區聲望
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    FReputationData GetReputation(const FString& RegionID) const;

    // 更新地區聲望
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    void UpdateReputation(const FString& RegionID, float ChangeAmount, const FString& Reason);

    // 處理互動效果
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void ProcessInteraction(const FInteractionEffect& InteractionEffect);

    // 檢查是否可以接取特定任務（基於聲望）
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    bool CanAcceptQuest(const FString& QuestID, const FString& RegionID) const;

    // 獲取對話選項（基於關係）
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    TArray<FString> GetDialogueOptions(const FString& CharacterID) const;

    // 獲取價格修正（基於關係）
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    float GetPriceModifier(const FString& CharacterID) const;

    // 獲取關係等級
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    ERelationshipType GetRelationshipLevel(const FString& CharacterID) const;

    // 獲取聲望等級
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    EReputationLevel GetReputationLevel(const FString& RegionID) const;

    // 添加共同記憶
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    void AddSharedMemory(const FString& CharacterID, const FString& Memory);

    // 獲取所有關係數據
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    const TMap<FString, FRelationshipData>& GetAllRelationships() const { return Relationships; }

    // 獲取所有聲望數據
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    const TMap<FString, FReputationData>& GetAllReputations() const { return Reputations; }

protected:
    // 角色關係數據
    UPROPERTY(BlueprintReadOnly, Category = "Relationship System")
    TMap<FString, FRelationshipData> Relationships;

    // 地區聲望數據
    UPROPERTY(BlueprintReadOnly, Category = "Reputation System")
    TMap<FString, FReputationData> Reputations;

    // 當前遊戲日
    UPROPERTY(BlueprintReadOnly, Category = "Relationship System")
    int32 CurrentGameDay;

    // 關係變化回調
    UFUNCTION(BlueprintImplementableEvent, Category = "Relationship System")
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);

    // 聲望變化回調
    UFUNCTION(BlueprintImplementableEvent, Category = "Reputation System")
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

private:
    // 計算關係類型
    ERelationshipType CalculateRelationshipType(float RelationshipValue) const;

    // 計算聲望等級
    EReputationLevel CalculateReputationLevel(float ReputationScore) const;

    // 應用關係衰減（長時間無互動）
    void ApplyRelationshipDecay();

    // 保存系統數據
    void SaveRelationshipData();

    // 加載系統數據
    void LoadRelationshipData();
};
