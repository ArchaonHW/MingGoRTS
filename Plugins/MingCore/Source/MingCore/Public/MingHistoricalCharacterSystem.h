#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalCharacterSystem.generated.h"

// 人物類?X???
UENUM(BlueprintType)
enum class EMingCharacterType : uint8
{
    MilitaryLeader,    // 軍?X???
    Politician,        // ??治??    Diplomat,          // 外交??    Businessman,       // ??人
    Scholar,           // 學??    Revolutionary,     // ??命??    Warlord,           // 軍閥
    ForeignAdvisor     // 外??顧??
};

// 人物????類??
UENUM(BlueprintType)
enum class EMingRelationshipType : uint8
{
    Ally,              // ????
    Enemy,             // ????
    Neutral,           // 中??
    Subordinate,       // 下屬
    Superior,          // 上??
    Family,            // 家??
    Mentor,            // 導師
    Rival              // 競爭對??
};

// 對話類??
UENUM(BlueprintType)
enum class EMingDialogueType : uint8
{
    Greeting,          // ????    Political,         // ??治討??
    Military,          // 軍?X?議
    Economic,          // 經??談判
    Personal,          // ??人交??
    Diplomatic,        // 外交交??
    Threat,            // 威??
    Persuasion,        // 說??
    Request,           // 請??
    Information        // 信息????
};

/**
 * ??治????記??結?? (??代 TTuple)
 */
USTRUCT(BlueprintType)
struct FMingPoliticalAlliance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID2;

    FMingPoliticalAlliance()
        : CharacterID1(0)
        , CharacterID2(0)
    {}
};

// 對話????結??
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDialogueOption
{
    GENERATED_BODY()

    // ????ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // ???X?本
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionText;

    // ???X?述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // ????類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDialogueType DialogueType;

    // ???X?置條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Prerequisites;

    // ????後??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Consequences;

    // ????影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> RelationshipChanges;

    // ?X?新對話
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedDialogues;

    FMingDialogueOption()
    {
        OptionID = -1;
        OptionText = TEXT(""};
        OptionDescription = TEXT(""};
        DialogueType = EMingDialogueType::Greeting;
        Prerequisites.Empty();
        Consequences.Empty();
        RelationshipChanges.Empty();
        UnlockedDialogues.Empty();
    }
};

// 對話節點????USTRUCT(BlueprintType)
struct FINGCORE_API FMingDialogueNode
{
    GENERATED_BODY()

    // 節點ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NodeID;

    // 節點?X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeText;

    // 說話X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SpeakerName;

    // 對話類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDialogueType DialogueType;

    // 對話????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDialogueOption> Options;

    // 條件檢查
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Conditions;

    // 對話????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DialogueSound;

    // 對話??畫
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DialogueAnimation;

    FMingDialogueNode()
    {
        NodeID = -1;
        NodeText = TEXT(""};
        SpeakerName = TEXT(""};
        DialogueType = EMingDialogueType::Greeting;
        Options.Empty();
        Conditions.Empty();
        DialogueSound = TEXT(""};
        DialogueAnimation = TEXT(""};
    }
};

// 歷史人物????
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalCharacter
{
    GENERATED_BODY()

    // 人物ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID;

    // 人物姓??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterName;

    // 人物字??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CourtesyName;

    // 人物類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingCharacterType CharacterType;

    // 人物??述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterDescription;

    // 歷史??景
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    // 人物屬??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> CharacterAttributes;

    // 人物??X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Skills;

    // 人物????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> Relationships;

    // ????位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CurrentLocation;

    // ??屬勢X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Faction;

    // 人物??X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CurrentStatus;

    // ??否??用??????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAvailableForDialogue;

    // 對話樹根節??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RootDialogueNode;

    FMingHistoricalCharacter()
    {
        CharacterID = -1;
        CharacterName = TEXT(""};
        CourtesyName = TEXT(""};
        CharacterType = EMingCharacterType::MilitaryLeader;
        CharacterDescription = TEXT(""};
        HistoricalBackground = TEXT(""};
        CharacterAttributes.Empty();
        Skills.Empty();
        Relationships.Empty();
        CurrentLocation = TEXT(""};
        Faction = TEXT(""};
        CurrentStatus = TEXT(""};
        bAvailableForDialogue = true;
        RootDialogueNode = -1;
    }
};

// 人物互??記??
USTRUCT(BlueprintType)
struct FINGCORE_API FMingCharacterInteraction
{
    GENERATED_BODY()

    // 互??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InteractionID;

    // 人物ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID;

    // 互?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime InteractionTime;

    // 互?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString InteractionLocation;

    // 對話節點ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DialogueNodeID;

    // ???X?選??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // 互??結??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> InteractionResults;

    // ????變??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> RelationshipChanges;

    // ?X?新??容
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockedContent;

    FMingCharacterInteraction()
    {
        InteractionID = -1;
        CharacterID = -1;
        InteractionTime = FDateTime::Now();
        InteractionLocation = TEXT(""};
        DialogueNodeID = -1;
        ChosenOptionID = -1;
        InteractionResults.Empty();
        RelationshipChanges.Empty();
        UnlockedContent.Empty();
    }
};

// 人物事件委??
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterEncountered, int32, CharacterID, const FMingHistoricalCharacter&, Character};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueStarted, int32, CharacterID, int32, NodeID, const FMingDialogueNode&, DialogueNode};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueOptionChosen, int32, CharacterID, int32, OptionID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipChanged, const FString&, CharacterName, int32, NewRelationshipValue};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionCompleted, const FMingCharacterInteraction&, Interaction};

/**
 * 歷史人物系統
 * 負責管??歷史人物????話??互??
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalCharacterSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalCharacterSystem();

    // ???X?人??系??    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool InitializeCharacterSystem();

    // 註??歷史人物
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character) {};

    // ????人物信息
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingHistoricalCharacter GetCharacterInfo(int32 CharacterID) const;

    // ???X???位置??人X    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<int32> GetCharactersAtLocation(const FString& Location) {};

    // ????對話
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool StartDialogue(int32 CharacterID};

    // ????對話節??    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingDialogueNode GetDialogueNode(int32 NodeID) const;

    // ????對話????
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool ChooseDialogueOption(int32 CharacterID, int32 NodeID, int32 OptionID};

    // 檢查對話條件
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool CheckDialogueConditions(int32 NodeID) const;

    // ????人物????
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    int32 GetRelationshipValue(int32 CharacterID, const FString& TargetCharacterName) const;

    // ??新人物????
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpdateRelationship(int32 CharacterID, const FString& TargetCharacterName, int32 ChangeValue};

    // ????互??歷史
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<FMingCharacterInteraction> GetInteractionHistory(int32 CharacterID) const;

    // 建?X?治????
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool CreatePoliticalAlliance(int32 CharacterID1, int32 CharacterID2};

    // ???X?治????
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool BreakPoliticalAlliance(int32 CharacterID1, int32 CharacterID2};

    // ????人物統??
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TMap<EMingCharacterType, int32> GetCharacterStatistics() const;

    // 保??人物????
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool SaveCharacterData();

    // 載入人物????
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool LoadCharacterData();

    // 清除????人??數X    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void ClearAllCharacterData();

    // 事件委??
    UPROPERTY(BlueprintAssignable)
    FOnCharacterEncountered OnCharacterEncountered;

    UPROPERTY(BlueprintAssignable)
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable)
    FOnDialogueOptionChosen OnDialogueOptionChosen;

    UPROPERTY(BlueprintAssignable)
    FOnRelationshipChanged OnRelationshipChanged;

    UPROPERTY(BlueprintAssignable)
    FOnInteractionCompleted OnInteractionCompleted;

protected:
    // 人物??????    UPROPERTY()
    TMap<int32, FMingHistoricalCharacter> CharacterDatabase;

    // 對話節點數??庫
    UPROPERTY()
    TMap<int32, FMingDialogueNode> DialogueNodeDatabase;

    // 互??歷史記??
    UPROPERTY()
    TArray<FMingCharacterInteraction> InteractionHistory;

    // ????對話中??人物
    UPROPERTY()
    TArray<int32> ActiveDialogueCharacters;

    // ??治????記?? - 使用????結?X?代 TTuple
    UPROPERTY()
    TArray<FMingPoliticalAlliance> PoliticalAlliances;

    // ??否已??始??
    UPROPERTY()
    bool bInitialized;

private:
    // 載入??設人物????
    void LoadDefaultCharacters();

    // 載入??設對話????
    void LoadDefaultDialogues();

    // 驗??人物????
    bool ValidateCharacterData(const FMingHistoricalCharacter& Character) const;

    // 驗??對話????
    bool ValidateDialogueData(const FMingDialogueNode& DialogueNode) const;

    // ????對話????後??
    void ProcessDialogueConsequences(int32 CharacterID, int32 NodeID, int32 OptionID};

    // ??新人物??X    void UpdateCharacterStatus(int32 CharacterID, const FString& NewStatus) {};

    // 記??互??
    void RecordInteraction(const FMingCharacterInteraction& Interaction) {};

    // 觸發人物事件
    void TriggerCharacterEvent(int32 CharacterID, const FString& EventType) {};

    // ???X???對話
    FMingDialogueNode GenerateDynamicDialogue(int32 CharacterID, const FString& Context) const;
};

