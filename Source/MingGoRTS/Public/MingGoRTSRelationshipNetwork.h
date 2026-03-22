#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSHistoricalCharacters.h"
#include "MingGoRTSRelationshipNetwork.generated.h"

UENUM(BlueprintType)
enum class EMingGameRelationshipType : uint8
{
    Family            UMETA(DisplayName = "ÂÆ∂Â∫≠?ú‰?"),
    Friend            UMETA(DisplayName = "?ãÂ??ú‰?"),
    Mentor            UMETA(DisplayName = "Â∏´Â??ú‰?"),
    Rival             UMETA(DisplayName = "Á´∂Áà≠?ú‰?"),
    Ally              UMETA(DisplayName = "?üÂ??ú‰?"),
    Enemy             UMETA(DisplayName = "?µÂ??ú‰?"),
    Colleague         UMETA(DisplayName = "?å‰??ú‰?"),
    Subordinate       UMETA(DisplayName = "‰∏ãÂ±¨?ú‰?"),
    Superior          UMETA(DisplayName = "‰∏äÂè∏?ú‰?"),
    Business          UMETA(DisplayName = "?ÜÊ•≠?ú‰?"),
    Political         UMETA(DisplayName = "?øÊ≤ª?ú‰?"),
    Military          UMETA(DisplayName = "Ëªç‰??ú‰?"),
    Romantic          UMETA(DisplayName = "?õÊ??ú‰?"),
    Acquaintance      UMETA(DisplayName = "?ü‰∫∫?ú‰?"),
    Stranger          UMETA(DisplayName = "?åÁ?‰∫?)
};

UENUM(BlueprintType)
enum class ERelationshipStrength : uint8
{
    VeryWeak          UMETA(DisplayName = "?ûÂ∏∏Âº?),
    Weak              UMETA(DisplayName = "Âº?),
    Moderate          UMETA(DisplayName = "‰∏≠Á?"),
    Strong            UMETA(DisplayName = "Âº?),
    VeryStrong        UMETA(DisplayName = "?ûÂ∏∏Âº?)
};

UENUM(BlueprintType)
enum class ERelationshipStatus : uint8
{
    Active            UMETA(DisplayName = "Ê¥ªË?"),
    Dormant           UMETA(DisplayName = "‰ºëÁ?"),
    Strained          UMETA(DisplayName = "Á∑äÂºµ"),
    Broken            UMETA(DisplayName = "?¥Ë?"),
    Developing        UMETA(DisplayName = "?ºÂ?‰∏?),
    Improving         UMETA(DisplayName = "?πÂ?‰∏?)
};

/**
 * FString ?∏Á??ÖË?ÁµêÊ? (?®Êñº TMap<TArray> ÂµåÂ?)
 */
USTRUCT(BlueprintType)
struct FStringArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Items;

    FStringArrayWrapper() {}
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    TMap<FString, FStringArrayWrapper> DirectConnections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Network")
    TMap<FString, FStringArrayWrapper> IndirectConnections;

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

    // ?ú‰?Á∂≤Áµ°‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Relationship Network")
    FOnRelationshipChanged OnRelationshipChanged;

    UPROPERTY(BlueprintAssignable, Category = "Relationship Network")
    FOnRelationshipEvent OnRelationshipEvent;

    UPROPERTY(BlueprintAssignable, Category = "Relationship Network")
    FOnNetworkAnalysisComplete OnNetworkAnalysisComplete;

    // ?ùÂ??ñÈ?‰øÇÁ∂≤Áµ°Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void InitializeRelationshipNetwork();

    // ?≤Â??Ä?âÈ?‰øÇÁ∂≤Áµ?    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FRelationshipNetwork> GetAllNetworks() const;

    // ?≤Â??ú‰?Á∂≤Áµ°
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    FRelationshipNetwork GetNetwork(const FString& NetworkID) const;

    // ?µÂª∫?ú‰?Á∂≤Áµ°
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    FString CreateNetwork(const FString& NetworkName, const TArray<FString>& CharacterIDs);

    // Ê∑ªÂ?ËßíËâ≤?∞Á∂≤Áµ?    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool AddCharacterToNetwork(const FString& NetworkID, const FString& CharacterID);

    // ÁßªÈô§ËßíËâ≤ÂæûÁ∂≤Áµ?    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool RemoveCharacterFromNetwork(const FString& NetworkID, const FString& CharacterID);

    // ?µÂª∫?ú‰???é•
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool CreateRelationship(const FString& SourceID, const FString& TargetID, ERelationshipType Type, ERelationshipStrength Strength);

    // ?¥Êñ∞?ú‰?
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool UpdateRelationship(const FString& SourceID, const FString& TargetID, float NewValue, const FString& Reason);

    // ?≤Â??ú‰???é•
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    FRelationshipConnection GetRelationship(const FString& SourceID, const FString& TargetID) const;

    // ?≤Â?ËßíËâ≤?Ä?âÈ?‰ø?    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FRelationshipConnection> GetCharacterRelationships(const FString& CharacterID) const;

    // ?≤Â??¥Êé•??é•
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> GetDirectConnections(const FString& CharacterID) const;

    // ?≤Â??ìÊé•??é•
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> GetIndirectConnections(const FString& CharacterID) const;

    // Ë®àÁ??ú‰?Âº∑Â∫¶
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float CalculateRelationshipStrength(const FString& SourceID, const FString& TargetID) const;

    // ?âÁî®?ú‰?ÂΩ±Èüø
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool ApplyRelationshipInfluence(const FString& SourceID, const FString& TargetID, const FString& InfluenceType, float InfluenceValue);

    // ?≥Êí≠?ú‰?ÂΩ±Èüø
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void PropagateRelationshipInfluence(const FString& SourceID, const FString& TargetID, const FString& InfluenceType, float InfluenceValue, int32 MaxDepth);

    // ?ÜÊ?Á∂≤Áµ°
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void AnalyzeNetwork(const FString& NetworkID);

    // Ë®àÁ?Á∂≤Áµ°ÂØÜÂ∫¶
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float CalculateNetworkDensity(const FString& NetworkID) const;

    // Ë®àÁ?Á∂≤Áµ°?ùË???    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float CalculateNetworkCohesion(const FString& NetworkID) const;

    // ?æÂà∞‰∏≠Â?ËßíËâ≤
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    FString FindCentralCharacter(const FString& NetworkID) const;

    // ?æÂà∞?úÈçµÂΩ±Èüø??    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> FindKeyInfluencers(const FString& NetworkID) const;

    // ?æÂà∞Ê©ãÊé•ËßíËâ≤
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> FindBridgingCharacters(const FString& NetworkID) const;

    // Ë®àÁ??Ä?≠Ë∑ØÂæ?    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> FindShortestPath(const FString& SourceID, const FString& TargetID) const;

    // Ë®àÁ?ÂΩ±Èüø??    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float CalculateInfluence(const FString& CharacterID) const;

    // ?≤Â??ú‰?Âª∫Ë≠∞
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> GetRelationshipSuggestions(const FString& CharacterID) const;

    // Ë®òÈ??ú‰?‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void RecordRelationshipEvent(const FString& EventName, const TArray<FString>& InvolvedCharacters, const FString& Description);

    // ?≤Â??ú‰?Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FRelationshipEvent> GetRelationshipHistory(const FString& CharacterID) const;

    // ‰øùÂ??ú‰??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool SaveRelationshipData(const FString& SaveSlotName);

    // ËºâÂÖ•?ú‰??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool LoadRelationshipData(const FString& SaveSlotName);

protected:
    // ?Ä?âÈ?‰øÇÁ∂≤Áµ?    UPROPERTY()
    TArray<FRelationshipNetwork> AllNetworks;

    // Á∂≤Áµ°ID?∞Á∂≤Áµ°Á??†Â?
    UPROPERTY()
    TMap<FString, FRelationshipNetwork> NetworkMap;

    // ËßíËâ≤ID?∞Á∂≤Áµ°ID?ÑÊ?Â∞?    UPROPERTY()
    TMap<FString, FString> CharacterNetworkMap;

    // ?ú‰???é•?†Â?
    UPROPERTY()
    TMap<FString, FRelationshipConnection> RelationshipMap;

    // ?ú‰?Ê≠∑Âè≤
    UPROPERTY()
    TArray<FRelationshipEvent> RelationshipHistory;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ≠∑?≤È?‰øÇÁ∂≤Áµ?    void InitializeHistoricalNetworks();

    // ?µÂª∫?ãÊ?Èª®Á∂≤Áµ?    void CreateNationalistNetwork();

    // ?µÂª∫?±Áî¢Èª®Á∂≤Áµ?    void CreateCommunistNetwork();

    // ?µÂª∫ËªçÈñ•Á∂≤Áµ°
    void CreateWarlordNetwork();

    // ?µÂª∫Â§ñ‰∫§Á∂≤Áµ°
    void CreateDiplomaticNetwork();

    // ?µÂª∫ÂÆ∂Â∫≠Á∂≤Áµ°
    void CreateFamilyNetwork();

    // ?µÂª∫?ÜÊ•≠Á∂≤Áµ°
    void CreateBusinessNetwork();

    // ?¥Êñ∞Á∂≤Áµ°?ÜÊ?
    void UpdateNetworkAnalysis(const FString& NetworkID);

    // Ë®àÁ??¥Êé•??é•
    void CalculateDirectConnections(const FString& NetworkID);

    // Ë®àÁ??ìÊé•??é•
    void CalculateIndirectConnections(const FString& NetworkID);

    // Ë®àÁ?Á∂≤Áµ°Áµ±Ë?
    void CalculateNetworkStatistics(const FString& NetworkID);

    // ?âÁî®?ú‰?ËÆäÂ?
    void ApplyRelationshipChange(const FString& SourceID, const FString& TargetID, float ChangeValue);

    // ?≥Êí≠ÂΩ±Èüø?∞Á∂≤Áµ?    void PropagateInfluenceToNetwork(const FString& NetworkID, const FString& SourceID, const FString& InfluenceType, float InfluenceValue);

    // ?üÊ??ú‰?ID
    FString GenerateRelationshipID(const FString& SourceID, const FString& TargetID) const;

    // ?üÊ?Á∂≤Áµ°ID
    FString GenerateNetworkID(const FString& BaseName) const;

    // È©óË??ú‰???é•
    bool ValidateRelationshipConnection(const FRelationshipConnection& Connection) const;

    // Ë®àÁ??ú‰?Ê¨äÈ?
    float CalculateRelationshipWeight(const FRelationshipConnection& Connection) const;

    // Ê™¢Êü•?ú‰?Ë°ùÁ?
    bool CheckRelationshipConflict(const FString& SourceID, const FString& TargetID, ERelationshipType NewType) const;

    // Ëß?±∫?ú‰?Ë°ùÁ?
    void ResolveRelationshipConflict(const FString& SourceID, const FString& TargetID, ERelationshipType NewType);

    // ?¥Êñ∞?ú‰??Ä??    void UpdateRelationshipStatus(const FString& SourceID, const FString& TargetID);

    // Ë®àÁ?‰ø°‰ªªÂ∫?    float CalculateTrustLevel(const FString& SourceID, const FString& TargetID) const;

    // Ë®àÁ?ÂΩ±ÈüøÂ∫?    float CalculateInfluenceLevel(const FString& SourceID, const FString& TargetID) const;

    // ?≤Â??ú‰?È°ûÂ?Ê¨äÈ?
    float GetRelationshipTypeWeight(ERelationshipType Type) const;

    // ?≤Â??ú‰?Âº∑Â∫¶Ê¨äÈ?
    float GetRelationshipStrengthWeight(ERelationshipStrength Strength) const;

    // Ê™¢Êü•?ú‰??ØÂê¶Â∞çÁ®±
    bool IsRelationshipSymmetric(ERelationshipType Type) const;

    // ?µÂª∫Â∞çÁ®±?ú‰?
    void CreateSymmetricRelationship(const FString& SourceID, const FString& TargetID, ERelationshipType Type, ERelationshipStrength Strength);

    // Ë®òÈ??ú‰?ËÆäÂ?
    void RecordRelationshipChange(const FString& SourceID, const FString& TargetID, const FString& ChangeType, float OldValue, float NewValue);

    // ?ÜÊ??ú‰?Ê®°Â?
    void AnalyzeRelationshipPatterns(const FString& NetworkID);

    // ?êÊ∏¨?ú‰??ºÂ?
    void PredictRelationshipDevelopment(const FString& SourceID, const FString& TargetID);

    // ?üÊ??ú‰??±Â?
    FString GenerateRelationshipReport(const FString& NetworkID) const;
};
