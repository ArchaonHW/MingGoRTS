#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCampaignSystem.generated.h"

UENUM(BlueprintType)
enum class EMingCampaignType : uint8
{
    Historical UMETA(DisplayName = "Historical"),
    WhatIf UMETA(DisplayName = "What If"),
    Tutorial UMETA(DisplayName = "Tutorial"),
    Custom UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EMingMissionStatus : uint8
{
    NotStarted UMETA(DisplayName = "Not Started"),
    InProgress UMETA(DisplayName = "In Progress"),
    Completed UMETA(DisplayName = "Completed"),
    Failed UMETA(DisplayName = "Failed"),
    Skipped UMETA(DisplayName = "Skipped")
};

UENUM(BlueprintType)
enum class EMingMissionType : uint8
{
    Combat UMETA(DisplayName = "Combat"),
    Defense UMETA(DisplayName = "Defense"),
    Escort UMETA(DisplayName = "Escort"),
    Capture UMETA(DisplayName = "Capture"),
    Rescue UMETA(DisplayName = "Rescue"),
    Stealth UMETA(DisplayName = "Stealth"),
    Puzzle UMETA(DisplayName = "Puzzle"),
    Dialogue UMETA(DisplayName = "Dialogue")
};

USTRUCT(BlueprintType)
struct FMingMissionObjective
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ObjectiveID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsOptional;

    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;

    UPROPERTY(BlueprintReadOnly)
    float Progress;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString TriggerCondition;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SuccessCondition;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString FailureCondition;

    FMingMissionObjective()
        : bIsOptional(false)
        , bIsCompleted(false)
        , Progress(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingMission
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString MissionID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString MissionName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString BriefingText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingMissionType MissionType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString MapName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMingMissionObjective> Objectives;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> PrerequisiteMissions;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TimeLimit;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bHasTimeLimit;

    UPROPERTY(BlueprintReadOnly)
    EMingMissionStatus Status;

    UPROPERTY(BlueprintReadOnly)
    float StartTime;

    UPROPERTY(BlueprintReadOnly)
    float CompletionTime;

    FMingMission()
        : MissionType(EMingMissionType::Combat)
        , TimeLimit(0.0f)
        , bHasTimeLimit(false)
        , Status(EMingMissionStatus::NotStarted)
        , StartTime(0.0f)
        , CompletionTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingCampaign
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString CampaignID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString CampaignName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingCampaignType CampaignType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMingMission> Missions;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString StartMissionID;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    UPROPERTY(BlueprintReadOnly)
    FString CurrentMissionID;

    FMingCampaign()
        : CampaignType(EMingCampaignType::Historical)
        , bIsActive(false)
    {}
};

/**
 * Campaign System for MingGoRTS
 * Manages campaigns, missions, and progression
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGSTRATEGIC_API UMingCampaignSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingCampaignSystem();

    // Campaign Management
    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void LoadCampaign(const FString& CampaignID);

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void StartCampaign(const FString& CampaignID);

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void SaveCampaign(const FString& CampaignID);

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void CompleteCampaign(const FString& CampaignID);

    UFUNCTION(BlueprintPure, Category = "Campaign")
    FMingCampaign GetCurrentCampaign() const;

    UFUNCTION(BlueprintPure, Category = "Campaign")
    TArray<FMingCampaign> GetAllCampaigns() const;

    UFUNCTION(BlueprintPure, Category = "Campaign")
    TArray<FMingCampaign> GetAvailableCampaigns() const;

    // Mission Management
    UFUNCTION(BlueprintCallable, Category = "Mission")
    void StartMission(const FString& MissionID);

    UFUNCTION(BlueprintCallable, Category = "Mission")
    void CompleteMission(const FString& MissionID);

    UFUNCTION(BlueprintCallable, Category = "Mission")
    void FailMission(const FString& MissionID);

    UFUNCTION(BlueprintCallable, Category = "Mission")
    void SkipMission(const FString& MissionID);

    UFUNCTION(BlueprintPure, Category = "Mission")
    FMingMission GetCurrentMission() const;

    UFUNCTION(BlueprintPure, Category = "Mission")
    TArray<FMingMission> GetAvailableMissions() const;

    UFUNCTION(BlueprintPure, Category = "Mission")
    TArray<FMingMission> GetCompletedMissions() const;

    // Objective Management
    UFUNCTION(BlueprintCallable, Category = "Objectives")
    void UpdateObjectiveProgress(const FString& ObjectiveID, float Progress);

    UFUNCTION(BlueprintCallable, Category = "Objectives")
    void CompleteObjective(const FString& ObjectiveID);

    UFUNCTION(BlueprintCallable, Category = "Objectives")
    void FailObjective(const FString& ObjectiveID);

    UFUNCTION(BlueprintPure, Category = "Objectives")
    TArray<FMingMissionObjective> GetCurrentObjectives() const;

    UFUNCTION(BlueprintPure, Category = "Objectives")
    float GetMissionProgress() const;

    UFUNCTION(BlueprintPure, Category = "Objectives")
    bool IsMissionComplete() const;

    // Progression
    UFUNCTION(BlueprintCallable, Category = "Progression")
    void UnlockNextMission();

    UFUNCTION(BlueprintCallable, Category = "Progression")
    void SetMissionPrerequisite(const FString& MissionID, const FString& PrerequisiteID);

    UFUNCTION(BlueprintPure, Category = "Progression")
    TArray<FString> GetMissionChain(const FString& MissionID) const;

    // Validation
    UFUNCTION(BlueprintPure, Category = "Validation")
    bool CanStartMission(const FString& MissionID) const;

    UFUNCTION(BlueprintPure, Category = "Validation")
    TArray<FString> GetMissingPrerequisites(const FString& MissionID) const;

    UFUNCTION(BlueprintPure, Category = "Validation")
    bool IsCampaignComplete(const FString& CampaignID) const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionStarted, const FMingMission&, Mission, const FString&, CampaignID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionCompleted, const FMingMission&, Mission, const FString&, CampaignID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionFailed, const FMingMission&, Mission, const FString&, CampaignID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveUpdated, const FMingMissionObjective&, Objective, float, Progress);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMissionStarted OnMissionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMissionCompleted OnMissionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMissionFailed OnMissionFailed;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnObjectiveUpdated OnObjectiveUpdated;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetMissionTypeName(EMingMissionType Type);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetCampaignTypeName(EMingCampaignType Type);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveCampaignData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadCampaignData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingCampaign> Campaigns;

    UPROPERTY()
    FString CurrentCampaignID;

    UPROPERTY()
    TSet<FString> CompletedCampaigns;

    UPROPERTY()
    TSet<FString> CompletedMissions;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal functions
    void InitializeDefaultCampaigns();
    void SetupHistoricalCampaigns();
    void SetupWhatIfCampaigns();
    void SetupTutorialCampaign();
    void CheckMissionCompletion();
    void UnlockDependentMissions(const FString& CompletedMissionID);
    void ValidateMissionProgression();

    // Helpers
    FMingMission* FindMission(const FString& MissionID);
    FMingCampaign* FindCampaign(const FString& CampaignID);
    bool ArePrerequisitesMet(const FString& MissionID) const;
};
