#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionConsequenceCalculator.generated.h"

/**
 * 決策後果類型
 */
UENUM(BlueprintType)
enum class EDecisionConsequenceType : uint8
{
    Immediate           UMETA(DisplayName = "Immediate"),
    Delayed             UMETA(DisplayName = "Delayed"),
    Cumulative          UMETA(DisplayName = "Cumulative"),
    Conditional         UMETA(DisplayName = "Conditional"),
    Random              UMETA(DisplayName = "Random"),
    Hidden              UMETA(DisplayName = "Hidden"),
    Permanent           UMETA(DisplayName = "Permanent"),
    Temporary           UMETA(DisplayName = "Temporary")
};

/**
 * 後果影響範圍
 */
UENUM(BlueprintType)
enum class EConsequenceImpactScope : uint8
{
    Personal            UMETA(DisplayName = "Personal"),
    Local               UMETA(DisplayName = "Local"),
    Regional            UMETA(DisplayName = "Regional"),
    National            UMETA(DisplayName = "National"),
    International       UMETA(DisplayName = "International"),
    Global              UMETA(DisplayName = "Global")
};

/**
 * 後果嚴重程度
 */
UENUM(BlueprintType)
enum class EConsequenceSeverity : uint8
{
    Trivial             UMETA(DisplayName = "Trivial"),
    Minor               UMETA(DisplayName = "Minor"),
    Moderate            UMETA(DisplayName = "Moderate"),
    Major               UMETA(DisplayName = "Major"),
    Critical            UMETA(DisplayName = "Critical"),
    Catastrophic        UMETA(DisplayName = "Catastrophic")
};

/**
 * 後果持續時間
 */
UENUM(BlueprintType)
enum class EConsequenceDuration : uint8
{
    Instant             UMETA(DisplayName = "Instant"),
    Short               UMETA(DisplayName = "Short"),
    Medium              UMETA(DisplayName = "Medium"),
    Long                UMETA(DisplayName = "Long"),
    Permanent           UMETA(DisplayName = "Permanent"),
    Indefinite          UMETA(DisplayName = "Indefinite")
};

/**
 * 後果影響類型
 */
UENUM(BlueprintType)
enum class EConsequenceImpactType : uint8
{
    Positive            UMETA(DisplayName = "Positive"),
    Negative            UMETA(DisplayName = "Negative"),
    Mixed               UMETA(DisplayName = "Mixed"),
    Neutral             UMETA(DisplayName = "Neutral"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * 後果參數
 */
USTRUCT(BlueprintType)
struct FConsequenceParameter
{
    GENERATED_BODY()

    // 參數名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString ParameterName;

    // 參數值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString ParameterValue;

    // 參數類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString ParameterType; // "int", "float", "bool", "string", "array"

    // 是否必需
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    bool bIsRequired;

    // 默認值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString DefaultValue;

    // 描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString Description;

    FConsequenceParameter()
        : ParameterName(TEXT(""))
        , ParameterValue(TEXT(""))
        , ParameterType(TEXT("string"))
        , bIsRequired(false)
        , DefaultValue(TEXT(""))
        , Description(TEXT(""))
    {}
};

/**
 * 決策後果定義
 */
USTRUCT(BlueprintType)
struct FDecisionConsequence
{
    GENERATED_BODY()

    // 後果ID
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    // 後果名稱
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceName;

    // 後果描述
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceDescription;

    // 後果類型
    UPROPERTY(BlueprintReadOnly)
    EDecisionConsequenceType ConsequenceType;

    // 影響範圍
    UPROPERTY(BlueprintReadOnly)
    EConsequenceImpactScope ImpactScope;

    // 嚴重程度
    UPROPERTY(BlueprintReadOnly)
    EConsequenceSeverity Severity;

    // 持續時間
    UPROPERTY(BlueprintReadOnly)
    EConsequenceDuration Duration;

    // 影響類型
    UPROPERTY(BlueprintReadOnly)
    EConsequenceImpactType ImpactType;

    // 後果參數
    UPROPERTY(BlueprintReadOnly)
    TArray<FConsequenceParameter> Parameters;

    // 觸發條件
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TriggerConditions;

    // 後果效果
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Effects;

    // 後果標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConsequenceTags;

    // 相關決策ID
    UPROPERTY(BlueprintReadOnly)
    FString RelatedDecisionID;

    // 相關人物ID
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedCharacterIDs;

    // 相關事件ID
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedEventIDs;

    // 發生概率 (0-1)
    UPROPERTY(BlueprintReadOnly)
    float Probability;

    // 延迟時間 (秒)
    UPROPERTY(BlueprintReadOnly)
    float DelayTime;

    // 冷卻時間 (秒)
    UPROPERTY(BlueprintReadOnly)
    float CooldownTime;

    // 是否已觸發
    UPROPERTY(BlueprintReadOnly)
    bool bIsTriggered;

    // 觸發時間
    UPROPERTY(BlueprintReadOnly)
    float TriggerTime;

    // 是否可逆轉
    UPROPERTY(BlueprintReadOnly)
    bool bIsReversible;

    // 可逆轉條件
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ReversalConditions;

    FDecisionConsequence()
        : ConsequenceID(TEXT(""))
        , ConsequenceName(TEXT(""))
        , ConsequenceDescription(TEXT(""))
        , ConsequenceType(EDecisionConsequenceType::Immediate)
        , ImpactScope(EConsequenceImpactScope::Local)
        , Severity(EConsequenceSeverity::Moderate)
        , Duration(EConsequenceDuration::Medium)
        , ImpactType(EConsequenceImpactType::Neutral)
        , RelatedDecisionID(TEXT(""))
        , Probability(1.0f)
        , DelayTime(0.0f)
        , CooldownTime(0.0f)
        , bIsTriggered(false)
        , TriggerTime(0.0f)
        , bIsReversible(false)
    {}
};

/**
 * 後果計算結果
 */
USTRUCT(BlueprintType)
struct FConsequenceCalculationResult
{
    GENERATED_BODY()

    // 是否成功計算
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 後果ID
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    // 計算時間
    UPROPERTY(BlueprintReadOnly)
    float CalculationTime;

    // 計算出的後果
    UPROPERTY(BlueprintReadOnly)
    TArray<FDecisionConsequence> CalculatedConsequences;

    // 影響的遊戲狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> AffectedGameState;

    // 影響的人物
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedCharacters;

    // 影響的事件
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedEvents;

    // 影響的資源
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> AffectedResources;

    // 影響的建築
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedBuildings;

    // 影響的單位
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedUnits;

    // 總體影響分數
    UPROPERTY(BlueprintReadOnly)
    float OverallImpactScore;

    // 正面影響分數
    UPROPERTY(BlueprintReadOnly)
    float PositiveImpactScore;

    // 負面影響分數
    UPROPERTY(BlueprintReadOnly)
    float NegativeImpactScore;

    // 計算詳情
    UPROPERTY(BlueprintReadOnly)
    FString CalculationDetails;

    // 錯誤信息
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    FConsequenceCalculationResult()
        : bSuccess(false)
        , ConsequenceID(TEXT(""))
        , CalculationTime(0.0f)
        , OverallImpactScore(0.0f)
        , PositiveImpactScore(0.0f)
        , NegativeImpactScore(0.0f)
        , CalculationDetails(TEXT(""))
        , ErrorMessage(TEXT(""))
    {}
};

/**
 * 決策上下文
 */
USTRUCT(BlueprintType)
struct FDecisionContext
{
    GENERATED_BODY()

    // 決策ID
    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    // 決策時間
    UPROPERTY(BlueprintReadOnly)
    float DecisionTime;

    // 決策地點
    UPROPERTY(BlueprintReadOnly)
    FString DecisionLocation;

    // 決策者ID
    UPROPERTY(BlueprintReadOnly)
    FString DecisionMakerID;

    // 決策類型
    UPROPERTY(BlueprintReadOnly)
    FString DecisionType;

    // 決策選項
    UPROPERTY(BlueprintReadOnly)
    FString DecisionOption;

    // 當前遊戲狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CurrentGameState;

    // 相關人物狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CharacterStates;

    // 相關事件狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> EventStates;

    // 資源狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceStates;

    // 建築狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> BuildingStates;

    // 單位狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> UnitStates;

    // 民國時期
    UPROPERTY(BlueprintReadOnly)
    FString RepublicEra;

    // 當前年份
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentYear;

    // 玩家陣營
    UPROPERTY(BlueprintReadOnly)
    FString PlayerFaction;

    // 難度等級
    UPROPERTY(BlueprintReadOnly)
    int32 DifficultyLevel;

    FDecisionContext()
        : DecisionID(TEXT(""))
        , DecisionTime(0.0f)
        , DecisionLocation(TEXT(""))
        , DecisionMakerID(TEXT(""))
        , DecisionType(TEXT(""))
        , DecisionOption(TEXT(""))
        , RepublicEra(TEXT(""))
        , CurrentYear(1912)
        , PlayerFaction(TEXT(""))
        , DifficultyLevel(1)
    {}
};

/**
 * 決策後果計算器
 * 計算決策的各種後果和影響
 */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingDecisionConsequenceCalculator : public UObject
{
    GENERATED_BODY()

public:
    UMingDecisionConsequenceCalculator();

    /**
     * 初始化後果計算器
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    void Initialize();

    /**
     * 關閉後果計算器
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    void Shutdown();

    /**
     * 註冊決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool RegisterDecisionConsequence(const FDecisionConsequence& Consequence);

    /**
     * 取消註冊決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool UnregisterDecisionConsequence(const FString& ConsequenceID);

    /**
     * 計算決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    FConsequenceCalculationResult CalculateDecisionConsequences(const FDecisionContext& Context);

    /**
     * 獲取決策後果
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    FDecisionConsequence GetDecisionConsequence(const FString& ConsequenceID) const;

    /**
     * 獲取所有決策後果
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetAllDecisionConsequences() const;

    /**
     * 獲取指定類型的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesByType(EDecisionConsequenceType ConsequenceType) const;

    /**
     * 獲取指定影響範圍的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesByScope(EConsequenceImpactScope ImpactScope) const;

    /**
     * 獲取指定嚴重程度的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesBySeverity(EConsequenceSeverity Severity) const;

    /**
     * 獲取指定影響類型的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesByImpactType(EConsequenceImpactType ImpactType) const;

    /**
     * 獲取相關決策的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesForDecision(const FString& DecisionID) const;

    /**
     * 獲取相關人物的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesForCharacter(const FString& CharacterID) const;

    /**
     * 獲取相關事件的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesForEvent(const FString& EventID) const;

    /**
     * 觸發後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool TriggerConsequence(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * 應用後果效果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool ApplyConsequenceEffects(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * 逆轉後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool ReverseConsequence(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * 獲取後果統計
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    TMap<FString, int32> GetConsequenceStatistics() const;

    /**
     * 獲取後果影響分析
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TMap<FString, float> GetConsequenceImpactAnalysis(const FString& DecisionID) const;

    /**
     * 預測決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> PredictDecisionConsequences(const FDecisionContext& Context, int32 PredictionCount);

    /**
     * 模擬決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FConsequenceCalculationResult> SimulateDecisionConsequences(const FDecisionContext& Context, int32 SimulationCount);

protected:
    // 已註冊的決策後果
    UPROPERTY()
    TArray<FDecisionConsequence> RegisteredConsequences;

    // 後果ID到後果的映射
    UPROPERTY()
    TMap<FString, FDecisionConsequence> ConsequenceMap;

    // 後果統計
    UPROPERTY()
    TMap<FString, int32> ConsequenceStatistics;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化民國歷史決策後果庫
    void InitializeHistoricalDecisionConsequences();

    // 創建政治決策後果
    void CreatePoliticalDecisionConsequences();

    // 創建軍事決策後果
    void CreateMilitaryDecisionConsequences();

    // 創建經濟決策後果
    void CreateEconomicDecisionConsequences();

    // 創建社會決策後果
    void CreateSocialDecisionConsequences();

    // 創建文化決策後果
    void CreateCulturalDecisionConsequences();

    // 創建外交決策後果
    void CreateDiplomaticDecisionConsequences();

    // 計算即時後果
    void CalculateImmediateConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result);

    // 計算延迟後果
    void CalculateDelayedConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result);

    // 計算累積後果
    void CalculateCumulativeConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result);

    // 計算條件後果
    void CalculateConditionalConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result);

    // 計算隨機後果
    void CalculateRandomConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result);

    // 計算隱藏後果
    void CalculateHiddenConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result);

    // 計算永久後果
    void CalculatePermanentConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result);

    // 計算臨時後果
    void CalculateTemporaryConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result);

    // 計算後果影響分數
    float CalculateConsequenceImpact(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 計算總體影響分數
    void CalculateOverallImpact(FConsequenceCalculationResult& Result) const;

    // 檢查後果條件
    bool CheckConsequenceConditions(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 應用後果到遊戲狀態
    void ApplyConsequenceToGameState(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TMap<FString, FString>& AffectedStates) const;

    // 應用後果到人物
    void ApplyConsequenceToCharacters(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedCharacters) const;

    // 應用後果到事件
    void ApplyConsequenceToEvents(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedEvents) const;

    // 應用後果到資源
    void ApplyConsequenceToResources(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TMap<FString, int32>& AffectedResources) const;

    // 應用後果到建築
    void ApplyConsequenceToBuildings(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedBuildings) const;

    // 應用後果到單位
    void ApplyConsequenceToUnits(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedUnits) const;

    // 更新後果統計
    void UpdateConsequenceStatistics(const FString& ConsequenceID);

    // 獲取後果描述
    FString GetConsequenceDescription(const FString& ConsequenceID) const;

    // 驗證後果配置
    bool ValidateConsequenceConfig(const FDecisionConsequence& Consequence) const;

    // 記錄後果日誌
    void LogConsequenceEvent(const FString& ConsequenceID, const FString& Message);

    // 生成後果報告
    FString GenerateConsequenceReport(const FString& ConsequenceID, const FConsequenceCalculationResult& Result) const;

    // 解析後果參數
    TArray<FString> ParseConsequenceParameters(const FString& ParameterString) const;

    // 計算後果概率
    float CalculateConsequenceProbability(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 計算後果延迟
    float CalculateConsequenceDelay(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 計算後果持續時間
    float CalculateConsequenceDuration(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 檢查後果是否可逆轉
    bool IsConsequenceReversible(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 獲取後果優先級
    int32 GetConsequencePriority(const FDecisionConsequence& Consequence) const;

    // 排序後果
    void SortConsequencesByPriority(TArray<FDecisionConsequence>& Consequences) const;
};
