#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.generated.h"

// �s?X?�O��??
UENUM(BlueprintType)
enum class EMingFormationCommand: uint8 {
    CreateFormation,    // ??�ؽs??
    ChangeFormation,     // �ܧ�s??
    DisbandFormation,   // 摧毀�s??
    MergeFormations,     // ??�ֽs??
    SplitFormation,     // 摧毀�s??
    RotateFormation,     // 摧毀�s??
    ScaleFormation,      // �Y��s??
    MirrorFormation     // 摧毀�s??
};

// �s?X?XUENUM(BlueprintType)
enum class EMingFormationStatus: uint8 {
    Forming,           // �s摧毀
    Active,            // ��??
    Transitioning,      // ��摧毀
    Disrupted,         // �Q摧毀
    Disbanded          // �w��X};

// �s?X摧毀?UENUM(BlueprintType)
enum class EMingFormationPriority: uint8 {
    Low,               // �C�u摧毀
    Normal,            // ??�q�u摧毀
    High,              // ���u摧毀
    Critical           // ??��摧毀??};

// �s?X???����
UENUM(BlueprintType)
enum class EMingUnitRole: uint8 {
    Leader,            // 摧毀
    Vanguard,          // 摧毀
    Flanker,           // ??�l
    Support,           // ??��
    Rearguard,         // ��??
    Reserve,           // ???X
    Artillery,         // ??�L
    Scout,             // 摧毀
    Medic              // ???X};

// �s?X???
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationData
{
    GENERATED_BODY()

    // �s??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // �s?X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationName;

    // �s??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // �s?X?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationStatus FormationStatus;

    // �s?X摧毀?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationPriority Priority;

    // �s??��摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector FormationCenter;

    // �s?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator FormationRotation;

    // �s??�W��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationScale;

    // ???X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnitIDs;

    // 摧毀����摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, EMingUnitRole> UnitRoles;

    // �s??��m摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> FormationPositions;

    // �s?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationBonuses;

    // �s??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationPenalties;

    // ??��摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CreationTime;

    // ??���???X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastUpdateTime;

    // �s?X?���xID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommanderID;

    // ??��??���xID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BackupCommanderID;

    // �s??�g?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationExperience;

    // �s??�h��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationMorale;

    // �s??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationDiscipline;

    // ??�_???X?�sX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsStatic;

    // ??�_??��??�sX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsAutoManaged;

    FMingFormationData()
    {
        FormationID = -1;
        FormationName = TEXT(""};
        FormationType = EMingFormationType::Line;
        FormationStatus = EMingFormationStatus::Forming;
        Priority = EMingFormationPriority::Normal;
        FormationCenter = FVector::ZeroVector;
        FormationRotation = FRotator::ZeroRotator;
        FormationScale = 1.0f;
        UnitIDs.Empty();
        UnitRoles.Empty();
        FormationPositions.Empty();
        FormationBonuses.Empty();
        FormationPenalties.Empty();
        CreationTime = FDateTime::Now();
        LastUpdateTime = FDateTime::Now();
        CommanderID = -1;
        BackupCommanderID = -1;
        FormationExperience = 0.0f;
        FormationMorale = 75.0f;
        FormationDiscipline = 70.0f;
        bIsStatic = false;
        bIsAutoManaged = false;
    }
};

// �s?X?�O摧毀
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationCommand
{
    GENERATED_BODY()

    // ??�OID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommandID;

    // ??�O��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationCommand CommandType;

    // 摧毀�s??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetFormationID;

    // ??�O??��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CommandParameters;

    // ??�O摧毀??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationPriority Priority;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // ??�O??�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CommandDescription;

    FMingFormationCommand()
    {
        CommandID = -1;
        CommandType = EMingFormationCommand::CreateFormation;
        TargetFormationID = -1;
        CommandParameters.Empty();
        Priority = EMingFormationPriority::Normal;
        ExecutionTime = 0.0f;
        CommandDescription = TEXT(""};
    }
};

// �s??�ҪO
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationTemplate
{
    GENERATED_BODY()

    // �ҪOID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TemplateID;

    // �ҪO??��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateName;

    // �ҪO??�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateDescription;

    // �s??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // ??��摧毀��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMingUnitType> RecommendedUnitTypes;

    // ??�p����X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinUnitCount;

    // ??�j����X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxUnitCount;

    // ��?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardSpacing;

    // ��??�`��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardDepth;

    // ��??�e��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardWidth;

    // �ҪO摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> TemplateBonuses;

    // �ҪO�ϥα���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UsageConditions;

    // ??�_??��??�sX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHistorical;

    // ���v??��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    FMingFormationTemplate()
    {
        TemplateID = -1;
        TemplateName = TEXT(""};
        TemplateDescription = TEXT("");
        FormationType = EMingFormationType::Line;
        RecommendedUnitTypes.Empty();
        MinUnitCount = 1;
        MaxUnitCount = 100;
        StandardSpacing = 100.0f;
        StandardDepth = 200.0f;
        StandardWidth = 300.0f;
        TemplateBonuses.Empty();
        UsageConditions.Empty();
        bIsHistorical = false;
        HistoricalBackground = TEXT(""};
    }
};

// �s??��??�ƥ�e??







/**
 * �s??��?X * �t�d��?X???�s?X?�ЫءB?X???���@
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingFormationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFormationManager();

    // ???X?�s??��??��
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool InitializeFormationManager();

    // ??�ؽs??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    int32 CreateFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType, const FString& FormationName);

    // 摧毀�s??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool DisbandFormation(int32 FormationID);

    // �ܧ�s??��??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ChangeFormationType(int32 FormationID, EMingFormationType NewFormationType);

    // �K?X??X?�sX
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool AddUnitToFormation(int32 FormationID, int32 UnitID, EMingUnitRole Role);

    // �q�s??��??��??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RemoveUnitFromFormation(int32 FormationID, int32 UnitID);

    // �]�m�s?X?��??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool SetFormationCommander(int32 FormationID, int32 CommanderID);

    // 摧毀�s??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RotateFormation(int32 FormationID, const FRotator& NewRotation);

    // �Y��s??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ScaleFormation(int32 FormationID, float NewScale);

    // ��??�s??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool MoveFormation(int32 FormationID, const FVector& TargetLocation);

    // 摧毀�s??�H��
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    FMingFormationData GetFormationInfo(int32 FormationID) const;

    // 摧毀�s?X?��
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TArray<int32> GetFormationList() const;

    // 摧毀�s??�ҪO
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TArray<FMingFormationTemplate> GetFormationTemplates() const;

    // ??�νs??�ҪO
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ApplyFormationTemplate(int32 FormationID, int32 TemplateID);

    // 摧毀�s??��??
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TMap<EMingFormationType, int32> GetFormationStatistics() const;

    // �p??�s?X???
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    float CalculateFormationEfficiency(int32 FormationID) const;

    // �ˬd�s??����X
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool CheckFormationIntegrity(int32 FormationID);

    // �״_�s??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RepairFormation(int32 FormationID);

    // �O??�s?X???
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool SaveFormationData();

    // ���J�s?X???
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool LoadFormationData();

    // �M��摧毀�sX
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    void ClearAllFormations();

    // �ƥ�e??
    UPROPERTY(BlueprintAssignable)
    FOnFormationCreated OnFormationCreated;

    UPROPERTY(BlueprintAssignable)
    FOnFormationChanged OnFormationChanged;

    UPROPERTY(BlueprintAssignable)
    FOnFormationDisbanded OnFormationDisbanded;

    UPROPERTY(BlueprintAssignable)
    FOnFormationUnitAdded OnFormationUnitAdded;

    UPROPERTY(BlueprintAssignable)
    FOnFormationUnitRemoved OnFormationUnitRemoved;

    UPROPERTY(BlueprintAssignable)
    FOnFormationCommandExecuted OnFormationCommandExecuted;

protected:
    // �s?X摧毀?
    UPROPERTY()
    TMap<int32, FMingFormationData> FormationDatabase;

    // �s??�ҪO摧毀??
    UPROPERTY()
    TMap<int32, FMingFormationTemplate> FormationTemplateDatabase;

    // �s?X?�O摧毀
    UPROPERTY()
    TArray<FMingFormationCommand> FormationCommandQueue;

    // 摧毀��??�s??
    UPROPERTY()
    TArray<int32> ActiveFormations;

    // ??�_�w??�l??
    UPROPERTY()
    bool bInitialized;

private:
    // ���J??�]�s??�ҪO
    void LoadDefaultFormationTemplates();

    // 摧毀�s??��m
    TArray<FVector> GenerateFormationPositions(EMingFormationType FormationType, int32 UnitCount, float Scale) const;

    // �p??�s?X???
    TMap<FString, float> CalculateFormationBonuses(int32 FormationID) const;

    // �p??�s??��??
    TMap<FString, float> CalculateFormationPenalties(int32 FormationID) const;

    // ??�s�s?X?X
    void UpdateFormationStatus(int32 FormationID, EMingFormationStatus NewStatus);

    // 摧毀�s?X?�O
    void ProcessFormationCommand(const FMingFormationCommand& Command);

    // ��??�s?X???
    bool ValidateFormationData(const FMingFormationData& Formation) const;

    // ??�νs?X??X?��??
    void ApplyFormationBonusesToUnits(int32 FormationID);

    // �����s?X???�q��??
    void RemoveFormationBonusesFromUnits(int32 FormationID);

    // 摧毀�״_�s??
    void AutoRepairFormation(int32 FormationID);

    // �p?X?�νs???X
    EMingFormationType CalculateOptimalFormationType(const TArray<int32>& UnitIDs) const;

    // �ˬd�s??��??
    bool CheckFormationConflict(int32 FormationID1, int32 FormationID2) const;

    // 摧毀�s??��??
    void ResolveFormationConflict(int32 FormationID1, int32 FormationID2);
);

