#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingMercenaryMode.generated.h"

UENUM(BlueprintType)
enum class EMingMercenaryRank : uint8
{
    Rookie UMETA(DisplayName = "Rookie"),
    Veteran UMETA(DisplayName = "Veteran"),
    Elite UMETA(DisplayName = "Elite"),
    Legend UMETA(DisplayName = "Legend")
};

UENUM(BlueprintType)
enum class EMingMercenaryType : uint8
{
    Infantry UMETA(DisplayName = "Infantry"),
    Cavalry UMETA(DisplayName = "Cavalry"),
    Artillery UMETA(DisplayName = "Artillery"),
    Sniper UMETA(DisplayName = "Sniper"),
    Medic UMETA(DisplayName = "Medic"),
    Engineer UMETA(DisplayName = "Engineer"),
    Scout UMETA(DisplayName = "Scout"),
    Heavy UMETA(DisplayName = "Heavy")
};

UENUM(BlueprintType)
enum class EMingContractType : uint8
{
    Assassination UMETA(DisplayName = "Assassination"),
    Escort UMETA(DisplayName = "Escort"),
    Sabotage UMETA(DisplayName = "Sabotage"),
    Rescue UMETA(DisplayName = "Rescue"),
    Capture UMETA(DisplayName = "Capture"),
    Defense UMETA(DisplayName = "Defense"),
    Reconnaissance UMETA(DisplayName = "Reconnaissance"),
    Extraction UMETA(DisplayName = "Extraction")
};

USTRUCT(BlueprintType)
struct FMingMercenarySkills
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StealthSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MedicalSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EngineeringSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LeadershipSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MarksmanshipSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SurvivalSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DiplomacySkill;

    FMingMercenarySkills()
        : CombatSkill(50.0f)
        , StealthSkill(50.0f)
        , MedicalSkill(50.0f)
        , EngineeringSkill(50.0f)
        , LeadershipSkill(50.0f)
        , MarksmanshipSkill(50.0f)
        , SurvivalSkill(50.0f)
        , DiplomacySkill(50.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingMercenary
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString MercenaryID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Nickname;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingMercenaryType MercType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingMercenaryRank Rank;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Age;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Nationality;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Background;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FMingMercenarySkills Skills;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Experience;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Reputation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DailyRate;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsAvailable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsHired;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Specializations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Equipment;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Portrait;

    FMingMercenary()
        : MercType(EMingMercenaryType::Infantry)
        , Rank(EMingMercenaryRank::Rookie)
        , Age(25)
        , Experience(0.0f)
        , Reputation(50.0f)
        , DailyRate(100.0f)
        , bIsAvailable(true)
        , bIsHired(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingContract
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ContractID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingContractType ContractType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Client;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Target;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Location;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Reward;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TimeLimit;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> RequiredSkills;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> RequiredEquipment;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Objectives;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Restrictions;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;

    UPROPERTY(BlueprintReadOnly)
    float StartTime;

    UPROPERTY(BlueprintReadOnly)
    float CompletionTime;

    FMingContract()
        : ContractType(EMingContractType::Assassination)
        , Difficulty(50.0f)
        , Reward(1000.0f)
        , TimeLimit(3600.0f)
        , bIsActive(false)
        , bIsCompleted(false)
        , StartTime(0.0f)
        , CompletionTime(0.0f)
    {}
};

/**
 * Mercenary Mode System for MingGoRTS
 * Manages mercenaries, contracts, and mercenary gameplay
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGMERCENARY_API UMingMercenaryMode : public UObject
{
    GENERATED_BODY()

public:
    UMingMercenaryMode();

    // Mercenary Management
    UFUNCTION(BlueprintCallable, Category = "Mercenary")
    void InitializeMercenaryMode();

    UFUNCTION(BlueprintCallable, Category = "Mercenary")
    void HireMercenary(const FString& MercenaryID);

    UFUNCTION(BlueprintCallable, Category = "Mercenary")
    void FireMercenary(const FString& MercenaryID);

    UFUNCTION(BlueprintCallable, Category = "Mercenary")
    void TrainMercenary(const FString& MercenaryID, const FString& SkillType, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Mercenary")
    void UpgradeMercenary(const FString& MercenaryID);

    UFUNCTION(BlueprintPure, Category = "Mercenary")
    TArray<FMingMercenary> GetAvailableMercenaries() const;

    UFUNCTION(BlueprintPure, Category = "Mercenary")
    TArray<FMingMercenary> GetHiredMercenaries() const;

    UFUNCTION(BlueprintPure, Category = "Mercenary")
    FMingMercenary GetMercenary(const FString& MercenaryID) const;

    UFUNCTION(BlueprintPure, Category = "Mercenary")
    float GetMercenaryCost(const FString& MercenaryID) const;

    // Contract Management
    UFUNCTION(BlueprintCallable, Category = "Contracts")
    void AcceptContract(const FString& ContractID);

    UFUNCTION(BlueprintCallable, Category = "Contracts")
    void CompleteContract(const FString& ContractID);

    UFUNCTION(BlueprintCallable, Category = "Contracts")
    void FailContract(const FString& ContractID);

    UFUNCTION(BlueprintCallable, Category = "Contracts")
    void AbandonContract(const FString& ContractID);

    UFUNCTION(BlueprintPure, Category = "Contracts")
    TArray<FMingContract> GetAvailableContracts() const;

    UFUNCTION(BlueprintPure, Category = "Contracts")
    TArray<FMingContract> GetActiveContracts() const;

    UFUNCTION(BlueprintPure, Category = "Contracts")
    TArray<FMingContract> GetCompletedContracts() const;

    UFUNCTION(BlueprintPure, Category = "Contracts")
    FMingContract GetContract(const FString& ContractID) const;

    UFUNCTION(BlueprintPure, Category = "Contracts")
    bool CanAcceptContract(const FString& ContractID) const;

    // Mercenary Skills
    UFUNCTION(BlueprintCallable, Category = "Skills")
    void ImproveSkill(const FString& MercenaryID, const FString& SkillType, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void AddSpecialization(const FString& MercenaryID, const FString& Specialization);

    UFUNCTION(BlueprintPure, Category = "Skills")
    float GetSkillLevel(const FString& MercenaryID, const FString& SkillType) const;

    UFUNCTION(BlueprintPure, Category = "Skills")
    TArray<FString> GetMercenarySpecializations(const FString& MercenaryID) const;

    // Reputation System
    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void AddReputation(const FString& MercenaryID, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void RemoveReputation(const FString& MercenaryID, float Amount);

    UFUNCTION(BlueprintPure, Category = "Reputation")
    float GetReputation(const FString& MercenaryID) const;

    UFUNCTION(BlueprintPure, Category = "Reputation")
    EMingMercenaryRank GetRank(const FString& MercenaryID) const;

    UFUNCTION(BlueprintCallable, Category = "Reputation")
    void PromoteMercenary(const FString& MercenaryID);

    // Equipment Management
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void EquipItem(const FString& MercenaryID, const FString& ItemID);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void UnequipItem(const FString& MercenaryID, const FString& ItemID);

    UFUNCTION(BlueprintPure, Category = "Equipment")
    TArray<FString> GetMercenaryEquipment(const FString& MercenaryID) const;

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void PurchaseEquipment(const FString& ItemID);

    // Financial Management
    UFUNCTION(BlueprintCallable, Category = "Finance")
    void PayMercenaries();

    UFUNCTION(BlueprintCallable, Category = "Finance")
    void CollectContractReward(const FString& ContractID);

    UFUNCTION(BlueprintPure, Category = "Finance")
    float GetDailyCosts() const;

    UFUNCTION(BlueprintPure, Category = "Finance")
    float GetTotalEarnings() const;

    UFUNCTION(BlueprintPure, Category = "Finance")
    float GetCurrentBalance() const;

    // Mercenary AI
    UFUNCTION(BlueprintCallable, Category = "AI")
    void AssignMercenaryToContract(const FString& MercenaryID, const FString& ContractID);

    UFUNCTION(BlueprintCallable, Category = "AI")
    void RemoveMercenaryFromContract(const FString& MercenaryID);

    UFUNCTION(BlueprintPure, Category = "AI")
    TArray<FString> GetAssignedMercenaries(const FString& ContractID) const;

    UFUNCTION(BlueprintPure, Category = "AI")
    FString GetCurrentContract(const FString& MercenaryID) const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMercenaryHired, const FMingMercenary&, Mercenary, float, Cost);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMercenaryFired, const FString&, MercenaryID, float, Severance);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnContractAccepted, const FMingContract&, Contract, const FString&, AssignedMercenary);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnContractCompleted, const FString&, ContractID, float, Reward);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMercenaryPromoted, const FString&, MercenaryID, EMingMercenaryRank, NewRank);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMercenaryHired OnMercenaryHired;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMercenaryFired OnMercenaryFired;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnContractAccepted OnContractAccepted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnContractCompleted OnContractCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMercenaryPromoted OnMercenaryPromoted;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetMercenaryTypeName(EMingMercenaryType MercType);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetRankName(EMingMercenaryRank Rank);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetContractTypeName(EMingContractType ContractType);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveMercenaryData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadMercenaryData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingMercenary> Mercenaries;

    UPROPERTY()
    TMap<FString, FMingContract> Contracts;

    UPROPERTY()
    TMap<FString, FString> MercenaryContractAssignments;

    UPROPERTY()
    float CurrentBalance;

    UPROPERTY()
    float TotalEarnings;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeDefaultMercenaries();
    void InitializeDefaultContracts();
    void SetupMercenarySkills();

    // Mercenary Management
    void UpdateMercenaryStats(const FString& MercenaryID);
    void CalculateMercenaryCost(FMingMercenary& Mercenary);
    bool CanAffordMercenary(const FString& MercenaryID) const;

    // Contract Management
    void GenerateNewContracts();
    void UpdateContractDifficulty();
    void EvaluateContractPerformance(const FString& ContractID);

    // Reputation System
    void UpdateReputationEffects(const FString& MercenaryID);
    void CheckRankPromotion(const FString& MercenaryID);

    // Financial System
    void ProcessDailyExpenses();
    void CalculateContractReward(FMingContract& Contract);
    void UpdateFinancialRecords();

    // AI System
    void AssignOptimalMercenaries(const FString& ContractID);
    void EvaluateMercenaryPerformance(const FString& MercenaryID, const FString& ContractID);

    // Helpers
    FMingMercenary* FindMercenary(const FString& MercenaryID);
    FMingContract* FindContract(const FString& ContractID);
    bool HasRequiredSkills(const FString& MercenaryID, const TArray<FString>& RequiredSkills) const;
    float CalculateContractSuccessChance(const FString& ContractID) const;
};
