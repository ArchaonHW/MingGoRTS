#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSDialogueSystem.generated.h"

UENUM(BlueprintType)
enum class EDialogueType : uint8
{
    Conversation     UMETA(DisplayName = "普通對話"),
    HistoricalEvent   UMETA(DisplayName = "歷史事件"),
    MilitaryBriefing  UMETA(DisplayName = "軍事簡報"),
    DiplomaticTalk   UMETA(DisplayName = "外交談話"),
    PersonalStory     UMETA(DisplayName = "個人故事"),
    QuestDialogue     UMETA(DisplayName = "任務對話")
};

UENUM(BlueprintType)
enum class EDialogueAttitude : uint8
{
    Friendly          UMETA(DisplayName = "友好"),
    Neutral           UMETA(DisplayName = "中立"),
    Suspicious        UMETA(DisplayName = "懷疑"),
    Hostile          UMETA(DisplayName = "敵對"),
    Respectful        UMETA(DisplayName = "尊敬"),
    Dismissive        UMETA(DisplayName = "輕視")
};

UENUM(BlueprintType)
enum class EDialogueOutcome : uint8
{
    None              UMETA(DisplayName = "無效果"),
    RelationshipChange UMETA(DisplayName = "關係變化"),
    QuestTrigger      UMETA(DisplayName = "觸發任務"),
    InformationGain   UMETA(DisplayName = "獲得情報"),
    SkillGain         UMETA(DisplayName = "技能提升"),
    EventTrigger      UMETA(DisplayName = "觸發事件")
};

USTRUCT(BlueprintType)
struct FDialogueCondition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Condition")
    FString ConditionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Condition")
    FString Parameter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Condition")
    FString Operator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Condition")
    FString Value;

    FDialogueCondition()
    {
        ConditionType = TEXT("");
        Parameter = TEXT("");
        Operator = TEXT("");
        Value = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FDialogueOutcome
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Outcome")
    EDialogueOutcome OutcomeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Outcome")
    FString TargetID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Outcome")
    float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Outcome")
    FString Description;

    FDialogueOutcome()
    {
        OutcomeType = EDialogueOutcome::None;
        TargetID = TEXT("");
        Value = 0.0f;
        Description = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingGameDialogueOption
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    EDialogueAttitude Attitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    FString NextDialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FDialogueCondition> Conditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FDialogueOutcome> Outcomes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Option")
    bool bIsAvailable;

    FDialogueOption()
    {
        OptionText = TEXT("");
        Attitude = EDialogueAttitude::Neutral;
        NextDialogueID = TEXT("");
        bIsAvailable = true;
    }
};

USTRUCT(BlueprintType)
struct FMingDialogue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString DialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString SpeakerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString DialogueText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    EDialogueType DialogueType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString SpeakerPortrait;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FDialogueOption> DialogueOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FDialogueCondition> TriggerConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FDialogueOutcome> AutoOutcomes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FString HistoricalContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bIsImportant;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bCanBeRepeated;

    FMingDialogue()
    {
        DialogueID = TEXT("");
        SpeakerID = TEXT("");
        DialogueText = TEXT("");
        DialogueType = EDialogueType::Conversation;
        SpeakerPortrait = TEXT("");
        BackgroundMusic = TEXT("");
        bIsImportant = false;
        bCanBeRepeated = true;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueStarted, const FString&, DialogueID, const FString&, SpeakerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueOptionSelected, const FString&, DialogueID, int32, OptionIndex, const FString&, OptionText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueCompleted, const FString&, DialogueID, const TArray<FDialogueOutcome>&, Outcomes);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSDialogueSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSDialogueSystem();

    // 對話系統事件
    UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
    FOnDialogueStarted OnDialogueStarted;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
    FOnDialogueOptionSelected OnDialogueOptionSelected;

    UPROPERTY(BlueprintAssignable, Category = "Dialogue System")
    FOnDialogueCompleted OnDialogueCompleted;

    // 初始化對話系統
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void InitializeDialogueSystem();

    // 獲取所有對話
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FMingDialogue> GetAllDialogues() const;

    // 獲取對話
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    FMingDialogue GetDialogue(const FString& DialogueID) const;

    // 開始對話
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool StartDialogue(const FString& DialogueID, const FMingCharacterData& PlayerCharacter);

    // 選擇對話選項
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool SelectDialogueOption(int32 OptionIndex);

    // 檢查對話是否可用
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    bool IsDialogueAvailable(const FString& DialogueID, const FMingCharacterData& PlayerCharacter) const;

    // 檢查對話選項是否可用
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    bool IsDialogueOptionAvailable(const FString& DialogueID, int32 OptionIndex, const FMingCharacterData& PlayerCharacter) const;

    // 獲取可用對話選項
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FDialogueOption> GetAvailableDialogueOptions(const FString& DialogueID, const FMingCharacterData& PlayerCharacter) const;

    // 獲取角色對話
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FMingDialogue> GetCharacterDialogues(const FString& CharacterID) const;

    // 獲取歷史事件對話
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FMingDialogue> GetHistoricalEventDialogues(const FString& EventID) const;

    // 獲取重要對話
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FMingDialogue> GetImportantDialogues() const;

    // 獲取對話歷史
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FString> GetDialogueHistory() const;

    // 添加對話到歷史
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void AddDialogueToHistory(const FString& DialogueID);

    // 清空對話歷史
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void ClearDialogueHistory();

    // 計算對話影響
    UFUNCTION(BlueprintPure, Category = "Dialogue System")
    TArray<FDialogueOutcome> CalculateDialogueOutcomes(const FString& DialogueID, int32 OptionIndex) const;

    // 應用對話結果
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    void ApplyDialogueOutcomes(const TArray<FDialogueOutcome>& Outcomes);

    // 保存對話數據
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool SaveDialogueData(const FString& SaveSlotName);

    // 載入對話數據
    UFUNCTION(BlueprintCallable, Category = "Dialogue System")
    bool LoadDialogueData(const FString& SaveSlotName);

protected:
    // 所有對話數據
    UPROPERTY()
    TArray<FMingDialogue> AllDialogues;

    // 對話ID到對話的映射
    UPROPERTY()
    TMap<FString, FMingDialogue> DialogueMap;

    // 角色ID到對話列表的映射 - 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<FString, TArray<FString>> CharacterDialogueMap;

    // 當前對話
    UPROPERTY()
    FMingDialogue CurrentDialogue;

    // 當前玩家角色
    UPROPERTY()
    FMingCharacterData CurrentPlayerCharacter;

    // 對話歷史
    UPROPERTY()
    TArray<FString> DialogueHistory;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化對話庫
    void InitializeDialogueLibrary();

    // 創建歷史人物對話
    void CreateHistoricalCharacterDialogues();

    // 創建軍事對話
    void CreateMilitaryDialogues();

    // 創建外交對話
    void CreateDiplomaticDialogues();

    // 創建個人故事對話
    void CreatePersonalStoryDialogues();

    // 創建任務對話
    void CreateQuestDialogues();

    // 驗證對話條件
    bool ValidateDialogueConditions(const TArray<FDialogueCondition>& Conditions, const FMingCharacterData& PlayerCharacter) const;

    // 驗證單個條件
    bool ValidateSingleCondition(const FDialogueCondition& Condition, const FMingCharacterData& PlayerCharacter) const;

    // 執行對話結果
    void ExecuteDialogueOutcomes(const TArray<FDialogueOutcome>& Outcomes);

    // 更新角色關係
    void UpdateCharacterRelationship(const FString& CharacterID, float RelationshipChange);

    // 觸發任務
    void TriggerQuest(const FString& QuestID);

    // 給予情報
    void GrantInformation(const FString& InformationID);

    // 提升技能
    void ImproveSkill(const FName& SkillID, float ImprovementAmount);

    // 觸發事件
    void TriggerEvent(const FString& EventID);

    // 生成對話ID
    FString GenerateDialogueID(const FString& BaseName, const FString& CharacterID) const;

    // 獲取角色關係值
    float GetCharacterRelationship(const FString& CharacterID) const;

    // 檢查角色是否已知
    bool IsCharacterKnown(const FString& CharacterID) const;

    // 獲取玩家屬性
    float GetPlayerAttribute(const FString& AttributeName) const;

    // 檢查任務狀態
    bool IsQuestCompleted(const FString& QuestID) const;

    // 檢查技能等級
    int32 GetSkillLevel(const FName& SkillID) const;
};
