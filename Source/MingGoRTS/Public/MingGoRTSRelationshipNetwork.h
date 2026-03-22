#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSHistoricalCharacters.h"
#include "MingGoRTSRelationshipNetwork.generated.h"

UENUM(BlueprintType)
enum class ERelationshipType : uint8
{
    Family            UMETA(DisplayName = "家庭關係"),
    Friend            UMETA(DisplayName = "朋友關係"),
    Mentor            UMETA(DisplayName = "師徒關係"),
    Rival             UMETA(DisplayName = "競爭關係"),
    Ally              UMETA(DisplayName = "盟友關係"),
    Enemy             UMETA(DisplayName = "敵對關係"),
    Colleague         UMETA(DisplayName = "同事關係"),
    Subordinate       UMETA(DisplayName = "下屬關係"),
    Superior          UMETA(DisplayName = "上司關係"),
    Business          UMETA(DisplayName = "商業關係"),
    Political         UMETA(DisplayName = "政治關係"),
    Military          UMETA(DisplayName = "軍事關係"),
    Romantic          UMETA(DisplayName = "愛情關係"),
    Acquaintance      UMETA(DisplayName = "熟人關係"),
    Stranger          UMETA(DisplayName = "陌生人")
};

UENUM(BlueprintType)
enum class ERelationshipStrength : uint8
{
    VeryWeak          UMETA(DisplayName = "非常弱"),
    Weak              UMETA(DisplayName = "弱"),
    Moderate          UMETA(DisplayName = "中等"),
    Strong            UMETA(DisplayName = "強"),
    VeryStrong        UMETA(DisplayName = "非常強")
};

UENUM(BlueprintType)
enum class ERelationshipStatus : uint8
{
    Active            UMETA(DisplayName = "活躍"),
    Dormant           UMETA(DisplayName = "休眠"),
    Strained          UMETA(DisplayName = "緊張"),
    Broken            UMETA(DisplayName = "破裂"),
    Developing        UMETA(DisplayName = "發展中"),
    Improving         UMETA(DisplayName = "改善中")
};

USTRUCT(BlueprintType)
struct FRelationshipConnection
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    FString SourceCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    FString TargetCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    ERelationshipType RelationshipType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    ERelationshipStrength Strength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    ERelationshipStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    float RelationshipValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    FString RelationshipDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    TArray<FString> SharedMemories;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    TArray<FString> MutualConnections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    FString LastInteractionDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    float InteractionFrequency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    float TrustLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    float InfluenceLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    bool bIsReciprocal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    bool bCanBeInfluenced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Connection")
    bool bIsSecret;

    FRelationshipConnection()
    {
        SourceCharacterID = TEXT("");
        TargetCharacterID = TEXT("");
        RelationshipType = ERelationshipType::Acquaintance;
        Strength = ERelationshipStrength::Weak;
        Status = ERelationshipStatus::Active;
        RelationshipValue = 50.0f;
        RelationshipDescription = TEXT("");
        LastInteractionDate = TEXT("");
        InteractionFrequency = 0.0f;
        TrustLevel = 50.0f;
        InfluenceLevel = 50.0f;
        bIsReciprocal = false;
        bCanBeInfluenced = true;
        bIsSecret = false;
    }
};

USTRUCT(BlueprintType)
struct FRelationshipInfluence
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Influence")
    FString SourceCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Influence")
    FString TargetCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Influence")
    FString InfluenceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Influence")
    float InfluenceValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Influence")
    FString InfluenceDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Influence")
    FString Context;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Influence")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Influence")
    bool bIsPermanent;

    FRelationshipInfluence()
    {
        SourceCharacterID = TEXT("");
        TargetCharacterID = TEXT("");
        InfluenceType = TEXT("");
        InfluenceValue = 0.0f;
        InfluenceDescription = TEXT("");
        Context = TEXT("");
        Duration = 0.0f;
        bIsPermanent = false;
    }
};

USTRUCT(BlueprintType)
struct FRelationshipNetwork
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    FString NetworkID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    FString NetworkName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    TArray<FString> CharacterIDs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    TArray<FRelationshipConnection> Connections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    TArray<FRelationshipInfluence> Influences;

    // 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<FString, TArray<FString>> DirectConnections;

    TMap<FString, TArray<FString>> IndirectConnections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    float NetworkDensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    float NetworkCohesion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    FString CentralCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    TArray<FString> KeyInfluencers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    TArray<FString> BridgingCharacters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    bool bIsStable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    bool bIsGrowing;

    FRelationshipNetwork()
    {
        NetworkID = TEXT("");
        NetworkName = TEXT("");
        Description = TEXT("");
        NetworkDensity = 0.0f;
        NetworkCohesion = 0.0f;
        CentralCharacterID = TEXT("");
        bIsStable = false;
        bIsGrowing = false;
    }
};

USTRUCT(BlueprintType)
struct FRelationshipEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    FString EventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    FString EventName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    TArray<FString> InvolvedCharacters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    TArray<FRelationshipConnection> ChangedRelationships;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    FString EventType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    FString EventDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    FString Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    float EventImpact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    bool bIsPositive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Event")
    bool bIsHistorical;

    FRelationshipEvent()
    {
        EventID = TEXT("");
        EventName = TEXT("");
        Description = TEXT("");
        EventType = TEXT("");
        EventDate = TEXT("");
        Location = TEXT("");
        EventImpact = 0.0f;
        bIsPositive = false;
        bIsHistorical = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipChanged, const FString&, SourceID, const FString&, TargetID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRelationshipEvent, const FString&, EventID, const FString&, EventName, const TArray<FString>&, InvolvedCharacters);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNetworkAnalysisComplete, const FString&, NetworkID, const FRelationshipNetwork&, NetworkData);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSRelationshipNetwork : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSRelationshipNetwork();

    // 關係網絡事件
    UPROPERTY(BlueprintAssignable, Category = "Relationship Network")
    FOnRelationshipChanged OnRelationshipChanged;

    UPROPERTY(BlueprintAssignable, Category = "Relationship Network")
    FOnRelationshipEvent OnRelationshipEvent;

    UPROPERTY(BlueprintAssignable, Category = "Relationship Network")
    FOnNetworkAnalysisComplete OnNetworkAnalysisComplete;

    // 初始化關係網絡系統
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void InitializeRelationshipNetwork();

    // 獲取所有關係網絡
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FRelationshipNetwork> GetAllNetworks() const;

    // 獲取關係網絡
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    FRelationshipNetwork GetNetwork(const FString& NetworkID) const;

    // 創建關係網絡
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    FString CreateNetwork(const FString& NetworkName, const TArray<FString>& CharacterIDs);

    // 添加角色到網絡
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool AddCharacterToNetwork(const FString& NetworkID, const FString& CharacterID);

    // 移除角色從網絡
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool RemoveCharacterFromNetwork(const FString& NetworkID, const FString& CharacterID);

    // 創建關係連接
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool CreateRelationship(const FString& SourceID, const FString& TargetID, ERelationshipType Type, ERelationshipStrength Strength);

    // 更新關係
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool UpdateRelationship(const FString& SourceID, const FString& TargetID, float NewValue, const FString& Reason);

    // 獲取關係連接
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    FRelationshipConnection GetRelationship(const FString& SourceID, const FString& TargetID) const;

    // 獲取角色所有關係
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FRelationshipConnection> GetCharacterRelationships(const FString& CharacterID) const;

    // 獲取直接連接
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> GetDirectConnections(const FString& CharacterID) const;

    // 獲取間接連接
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> GetIndirectConnections(const FString& CharacterID) const;

    // 計算關係強度
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float CalculateRelationshipStrength(const FString& SourceID, const FString& TargetID) const;

    // 應用關係影響
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool ApplyRelationshipInfluence(const FString& SourceID, const FString& TargetID, const FString& InfluenceType, float InfluenceValue);

    // 傳播關係影響
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void PropagateRelationshipInfluence(const FString& SourceID, const FString& TargetID, const FString& InfluenceType, float InfluenceValue, int32 MaxDepth);

    // 分析網絡
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void AnalyzeNetwork(const FString& NetworkID);

    // 計算網絡密度
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float CalculateNetworkDensity(const FString& NetworkID) const;

    // 計算網絡凝聚力
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float CalculateNetworkCohesion(const FString& NetworkID) const;

    // 找到中心角色
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    FString FindCentralCharacter(const FString& NetworkID) const;

    // 找到關鍵影響者
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> FindKeyInfluencers(const FString& NetworkID) const;

    // 找到橋接角色
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> FindBridgingCharacters(const FString& NetworkID) const;

    // 計算最短路徑
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> FindShortestPath(const FString& SourceID, const FString& TargetID) const;

    // 計算影響力
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float CalculateInfluence(const FString& CharacterID) const;

    // 獲取關係建議
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> GetRelationshipSuggestions(const FString& CharacterID) const;

    // 記錄關係事件
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void RecordRelationshipEvent(const FString& EventName, const TArray<FString>& InvolvedCharacters, const FString& Description);

    // 獲取關係歷史
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FRelationshipEvent> GetRelationshipHistory(const FString& CharacterID) const;

    // 保存關係數據
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool SaveRelationshipData(const FString& SaveSlotName);

    // 載入關係數據
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool LoadRelationshipData(const FString& SaveSlotName);

protected:
    // 所有關係網絡
    UPROPERTY()
    TArray<FRelationshipNetwork> AllNetworks;

    // 網絡ID到網絡的映射
    UPROPERTY()
    TMap<FString, FRelationshipNetwork> NetworkMap;

    // 角色ID到網絡ID的映射
    UPROPERTY()
    TMap<FString, FString> CharacterNetworkMap;

    // 關係連接映射
    UPROPERTY()
    TMap<FString, FRelationshipConnection> RelationshipMap;

    // 關係歷史
    UPROPERTY()
    TArray<FRelationshipEvent> RelationshipHistory;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化歷史關係網絡
    void InitializeHistoricalNetworks();

    // 創建國民黨網絡
    void CreateNationalistNetwork();

    // 創建共產黨網絡
    void CreateCommunistNetwork();

    // 創建軍閥網絡
    void CreateWarlordNetwork();

    // 創建外交網絡
    void CreateDiplomaticNetwork();

    // 創建家庭網絡
    void CreateFamilyNetwork();

    // 創建商業網絡
    void CreateBusinessNetwork();

    // 更新網絡分析
    void UpdateNetworkAnalysis(const FString& NetworkID);

    // 計算直接連接
    void CalculateDirectConnections(const FString& NetworkID);

    // 計算間接連接
    void CalculateIndirectConnections(const FString& NetworkID);

    // 計算網絡統計
    void CalculateNetworkStatistics(const FString& NetworkID);

    // 應用關係變化
    void ApplyRelationshipChange(const FString& SourceID, const FString& TargetID, float ChangeValue);

    // 傳播影響到網絡
    void PropagateInfluenceToNetwork(const FString& NetworkID, const FString& SourceID, const FString& InfluenceType, float InfluenceValue);

    // 生成關係ID
    FString GenerateRelationshipID(const FString& SourceID, const FString& TargetID) const;

    // 生成網絡ID
    FString GenerateNetworkID(const FString& BaseName) const;

    // 驗證關係連接
    bool ValidateRelationshipConnection(const FRelationshipConnection& Connection) const;

    // 計算關係權重
    float CalculateRelationshipWeight(const FRelationshipConnection& Connection) const;

    // 檢查關係衝突
    bool CheckRelationshipConflict(const FString& SourceID, const FString& TargetID, ERelationshipType NewType) const;

    // 解決關係衝突
    void ResolveRelationshipConflict(const FString& SourceID, const FString& TargetID, ERelationshipType NewType);

    // 更新關係狀態
    void UpdateRelationshipStatus(const FString& SourceID, const FString& TargetID);

    // 計算信任度
    float CalculateTrustLevel(const FString& SourceID, const FString& TargetID) const;

    // 計算影響度
    float CalculateInfluenceLevel(const FString& SourceID, const FString& TargetID) const;

    // 獲取關係類型權重
    float GetRelationshipTypeWeight(ERelationshipType Type) const;

    // 獲取關係強度權重
    float GetRelationshipStrengthWeight(ERelationshipStrength Strength) const;

    // 檢查關係是否對稱
    bool IsRelationshipSymmetric(ERelationshipType Type) const;

    // 創建對稱關係
    void CreateSymmetricRelationship(const FString& SourceID, const FString& TargetID, ERelationshipType Type, ERelationshipStrength Strength);

    // 記錄關係變化
    void RecordRelationshipChange(const FString& SourceID, const FString& TargetID, const FString& ChangeType, float OldValue, float NewValue);

    // 分析關係模式
    void AnalyzeRelationshipPatterns(const FString& NetworkID);

    // 預測關係發展
    void PredictRelationshipDevelopment(const FString& SourceID, const FString& TargetID);

    // 生成關係報告
    FString GenerateRelationshipReport(const FString& NetworkID) const;
};
