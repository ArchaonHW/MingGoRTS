#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalCharacterSystem.generated.h"

// 人物類型枚舉
UENUM(BlueprintType)
enum class EMingCharacterType : uint8
{
    MilitaryLeader,    // 軍事領袖
    Politician,        // 政治家
    Diplomat,          // 外交官
    Businessman,       // 商人
    Scholar,           // 學者
    Revolutionary,     // 革命家
    Warlord,           // 軍閥
    ForeignAdvisor     // 外國顧問
};

// 人物關係類型
UENUM(BlueprintType)
enum class EMingRelationshipType : uint8
{
    Ally,              // 同盟
    Enemy,             // 敵對
    Neutral,           // 中立
    Subordinate,       // 下屬
    Superior,          // 上級
    Family,            // 家族
    Mentor,            // 導師
    Rival              // 競爭對手
};

// 對話類型
UENUM(BlueprintType)
enum class EMingDialogueType : uint8
{
    Greeting,          // 問候
    Political,         // 政治討論
    Military,          // 軍事商議
    Economic,          // 經濟談判
    Personal,          // 個人交談
    Diplomatic,        // 外交交涉
    Threat,            // 威脅
    Persuasion,        // 說服
    Request,           // 請求
    Information        // 信息收集
};

/**
 * 政治聯盟記錄結構 (替代 TTuple)
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

// 對話選項結構
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDialogueOption
{
    GENERATED_BODY()

    // 選項ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // 選項文本
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionText;

    // 選項描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // 選項類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDialogueType DialogueType;

    // 選項前置條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Prerequisites;

    // 選項後果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Consequences;

    // 關係影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> RelationshipChanges;

    // 解鎖的新對話
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedDialogues;

    FMingDialogueOption()
    {
        OptionID = -1;
        OptionText = TEXT("");
        OptionDescription = TEXT("");
        DialogueType = EMingDialogueType::Greeting;
        Prerequisites.Empty();
        Consequences.Empty();
        RelationshipChanges.Empty();
        UnlockedDialogues.Empty();
    }
};

// 對話節點結構
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDialogueNode
{
    GENERATED_BODY()

    // 節點ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NodeID;

    // 節點文本
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NodeText;

    // 說話者
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SpeakerName;

    // 對話類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDialogueType DialogueType;

    // 對話選項
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDialogueOption> Options;

    // 條件檢查
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Conditions;

    // 對話音效
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DialogueSound;

    // 對話動畫
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DialogueAnimation;

    FMingDialogueNode()
    {
        NodeID = -1;
        NodeText = TEXT("");
        SpeakerName = TEXT("");
        DialogueType = EMingDialogueType::Greeting;
        Options.Empty();
        Conditions.Empty();
        DialogueSound = TEXT("");
        DialogueAnimation = TEXT("");
    }
};

// 歷史人物數據
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalCharacter
{
    GENERATED_BODY()

    // 人物ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID;

    // 人物姓名
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterName;

    // 人物字號
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CourtesyName;

    // 人物類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingCharacterType CharacterType;

    // 人物描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterDescription;

    // 歷史背景
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    // 人物屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> CharacterAttributes;

    // 人物技能
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Skills;

    // 人物關係
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> Relationships;

    // 當前位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CurrentLocation;

    // 所屬勢力
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Faction;

    // 人物狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CurrentStatus;

    // 是否可用於對話
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAvailableForDialogue;

    // 對話樹根節點
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RootDialogueNode;

    FMingHistoricalCharacter()
    {
        CharacterID = -1;
        CharacterName = TEXT("");
        CourtesyName = TEXT("");
        CharacterType = EMingCharacterType::MilitaryLeader;
        CharacterDescription = TEXT("");
        HistoricalBackground = TEXT("");
        CharacterAttributes.Empty();
        Skills.Empty();
        Relationships.Empty();
        CurrentLocation = TEXT("");
        Faction = TEXT("");
        CurrentStatus = TEXT("");
        bAvailableForDialogue = true;
        RootDialogueNode = -1;
    }
};

// 人物互動記錄
USTRUCT(BlueprintType)
struct FINGCORE_API FMingCharacterInteraction
{
    GENERATED_BODY()

    // 互動ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InteractionID;

    // 人物ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CharacterID;

    // 互動時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime InteractionTime;

    // 互動地點
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString InteractionLocation;

    // 對話節點ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DialogueNodeID;

    // 選擇的選項ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // 互動結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> InteractionResults;

    // 關係變化
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> RelationshipChanges;

    // 解鎖的新內容
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockedContent;

    FMingCharacterInteraction()
    {
        InteractionID = -1;
        CharacterID = -1;
        InteractionTime = FDateTime::Now();
        InteractionLocation = TEXT("");
        DialogueNodeID = -1;
        ChosenOptionID = -1;
        InteractionResults.Empty();
        RelationshipChanges.Empty();
        UnlockedContent.Empty();
    }
};

// 人物事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterEncountered, int32, CharacterID, const FMingHistoricalCharacter&, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueStarted, int32, CharacterID, int32, NodeID, const FMingDialogueNode&, DialogueNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueOptionChosen, int32, CharacterID, int32, OptionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipChanged, const FString&, CharacterName, int32, NewRelationshipValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionCompleted, const FMingCharacterInteraction&, Interaction);

/**
 * 歷史人物系統
 * 負責管理歷史人物的對話和互動
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalCharacterSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalCharacterSystem();

    // 初始化人物系統
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool InitializeCharacterSystem();

    // 註冊歷史人物
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character);

    // 獲取人物信息
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingHistoricalCharacter GetCharacterInfo(int32 CharacterID) const;

    // 獲取當前位置的人物
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<int32> GetCharactersAtLocation(const FString& Location);

    // 開始對話
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool StartDialogue(int32 CharacterID);

    // 獲取對話節點
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingDialogueNode GetDialogueNode(int32 NodeID) const;

    // 選擇對話選項
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool ChooseDialogueOption(int32 CharacterID, int32 NodeID, int32 OptionID);

    // 檢查對話條件
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool CheckDialogueConditions(int32 NodeID) const;

    // 獲取人物關係
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    int32 GetRelationshipValue(int32 CharacterID, const FString& TargetCharacterName) const;

    // 更新人物關係
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpdateRelationship(int32 CharacterID, const FString& TargetCharacterName, int32 ChangeValue);

    // 獲取互動歷史
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<FMingCharacterInteraction> GetInteractionHistory(int32 CharacterID) const;

    // 建立政治聯盟
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool CreatePoliticalAlliance(int32 CharacterID1, int32 CharacterID2);

    // 破壞政治聯盟
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool BreakPoliticalAlliance(int32 CharacterID1, int32 CharacterID2);

    // 獲取人物統計
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TMap<EMingCharacterType, int32> GetCharacterStatistics() const;

    // 保存人物數據
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool SaveCharacterData();

    // 載入人物數據
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool LoadCharacterData();

    // 清除所有人物數據
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void ClearAllCharacterData();

    // 事件委託
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
    // 人物數據庫
    UPROPERTY()
    TMap<int32, FMingHistoricalCharacter> CharacterDatabase;

    // 對話節點數據庫
    UPROPERTY()
    TMap<int32, FMingDialogueNode> DialogueNodeDatabase;

    // 互動歷史記錄
    UPROPERTY()
    TArray<FMingCharacterInteraction> InteractionHistory;

    // 當前對話中的人物
    UPROPERTY()
    TArray<int32> ActiveDialogueCharacters;

    // 政治聯盟記錄 - 使用包裝結構替代 TTuple
    UPROPERTY()
    TArray<FMingPoliticalAlliance> PoliticalAlliances;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設人物數據
    void LoadDefaultCharacters();

    // 載入預設對話數據
    void LoadDefaultDialogues();

    // 驗證人物數據
    bool ValidateCharacterData(const FMingHistoricalCharacter& Character) const;

    // 驗證對話數據
    bool ValidateDialogueData(const FMingDialogueNode& DialogueNode) const;

    // 處理對話選項後果
    void ProcessDialogueConsequences(int32 CharacterID, int32 NodeID, int32 OptionID);

    // 更新人物狀態
    void UpdateCharacterStatus(int32 CharacterID, const FString& NewStatus);

    // 記錄互動
    void RecordInteraction(const FMingCharacterInteraction& Interaction);

    // 觸發人物事件
    void TriggerCharacterEvent(int32 CharacterID, const FString& EventType);

    // 生成動態對話
    FMingDialogueNode GenerateDynamicDialogue(int32 CharacterID, const FString& Context) const;
};
