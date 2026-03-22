#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.generated.h"

// 編?X?令類??
UENUM(BlueprintType)
enum class EMingFormationCommand : uint8
{
    CreateFormation,    // ??建編??
    ChangeFormation,     // 變更編??
    DisbandFormation,   // ????編??
    MergeFormations,     // ??併編??
    SplitFormation,     // ????編??
    RotateFormation,     // ????編??
    ScaleFormation,      // 縮放編??
    MirrorFormation     // ????編??
};

// 編?X?XUENUM(BlueprintType)
enum class EMingFormationStatus : uint8
{
    Forming,           // 編????    Active,            // 活??
    Transitioning,      // 轉????    Disrupted,         // 被????    Disbanded          // 已解X};

// 編?X?????UENUM(BlueprintType)
enum class EMingFormationPriority : uint8
{
    Low,               // 低優????
    Normal,            // ??通優????
    High,              // 高優????
    Critical           // ??鍵??????};

// 編?X???角色
UENUM(BlueprintType)
enum class EMingUnitRole : uint8
{
    Leader,            // ????
    Vanguard,          // ????
    Flanker,           // ??翼
    Support,           // ??援
    Rearguard,         // 後??
    Reserve,           // ???X    Artillery,         // ??兵
    Scout,             // ????
    Medic              // ???X};

// 編?X???
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationData
{
    GENERATED_BODY()

    // 編??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // 編?X?稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationName;

    // 編??類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // 編?X?X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationStatus FormationStatus;

    // 編?X?????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationPriority Priority;

    // 編??中????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector FormationCenter;

    // 編?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator FormationRotation;

    // 編??規模
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationScale;

    // ???X?表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnitIDs;

    // ????角色????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, EMingUnitRole> UnitRoles;

    // 編??位置????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> FormationPositions;

    // 編?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationBonuses;

    // 編??減??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationPenalties;

    // ??建????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CreationTime;

    // ??後更???X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastUpdateTime;

    // 編?X?揮官ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommanderID;

    // ??用??揮官ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BackupCommanderID;

    // 編??經?X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationExperience;

    // 編??士氣
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationMorale;

    // 編??紀??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationDiscipline;

    // ??否???X?編X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsStatic;

    // ??否??自??編X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// 編?X?令????
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationCommand
{
    GENERATED_BODY()

    // ??令ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommandID;

    // ??令類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationCommand CommandType;

    // ????編??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetFormationID;

    // ??令??數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CommandParameters;

    // ??令??????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationPriority Priority;

    // ???X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // ??令??述
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

// 編??模板
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationTemplate
{
    GENERATED_BODY()

    // 模板ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TemplateID;

    // 模板??稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateName;

    // 模板??述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateDescription;

    // 編??類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // ??薦????類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMingUnitType> RecommendedUnitTypes;

    // ??小單位數X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinUnitCount;

    // ??大單位數X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxUnitCount;

    // 標?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardSpacing;

    // 標??深度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardDepth;

    // 標??寬度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardWidth;

    // 模板????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> TemplateBonuses;

    // 模板使用條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UsageConditions;

    // ??否??歷??編X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHistorical;

    // 歷史??景
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    FMingFormationTemplate()
    {
        TemplateID = -1;
        TemplateName = TEXT(""};
        TemplateDescription = TEXT(""};
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

// 編??管??事件委??
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFormationCreated, const FMingFormationData&, Formation};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFormationChanged, int32, FormationID, EMingFormationType, NewFormationType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFormationDisbanded, int32, FormationID, const TArray<int32>&, ReleasedUnits};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFormationUnitAdded, int32, FormationID, int32, UnitID, EMingUnitRole, Role};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFormationUnitRemoved, int32, FormationID, int32, UnitID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFormationCommandExecuted, const FMingFormationCommand&, Command};

/**
 * 編??管?X * 負責管?X???編?X?創建、?X???維護
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingFormationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFormationManager();

    // ???X?編??管??器
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool InitializeFormationManager();

    // ??建編??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    int32 CreateFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType, const FString& FormationName};

    // ????編??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool DisbandFormation(int32 FormationID};

    // 變更編??類??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ChangeFormationType(int32 FormationID, EMingFormationType NewFormationType};

    // 添?X??X?編X    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool AddUnitToFormation(int32 FormationID, int32 UnitID, EMingUnitRole Role};

    // 從編??移??單??    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RemoveUnitFromFormation(int32 FormationID, int32 UnitID};

    // 設置編?X?揮??    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool SetFormationCommander(int32 FormationID, int32 CommanderID};

    // ????編??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RotateFormation(int32 FormationID, const FRotator& NewRotation};

    // 縮放編??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ScaleFormation(int32 FormationID, float NewScale};

    // 移??編??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool MoveFormation(int32 FormationID, const FVector& TargetLocation};

    // ????編??信息
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    FMingFormationData GetFormationInfo(int32 FormationID) const;

    // ????編?X?表
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TArray<int32> GetFormationList() const;

    // ????編??模板
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TArray<FMingFormationTemplate> GetFormationTemplates() const;

    // ??用編??模板
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ApplyFormationTemplate(int32 FormationID, int32 TemplateID};

    // ????編??統??
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TMap<EMingFormationType, int32> GetFormationStatistics() const;

    // 計??編?X???
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    float CalculateFormationEfficiency(int32 FormationID) const;

    // 檢查編??完整X    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool CheckFormationIntegrity(int32 FormationID};

    // 修復編??
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RepairFormation(int32 FormationID};

    // 保??編?X???
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool SaveFormationData();

    // 載入編?X???
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool LoadFormationData();

    // 清除????編X    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    void ClearAllFormations();

    // 事件委??
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
    // 編?X?????    UPROPERTY()
    TMap<int32, FMingFormationData> FormationDatabase;

    // 編??模板??????    UPROPERTY()
    TMap<int32, FMingFormationTemplate> FormationTemplateDatabase;

    // 編?X?令????
    UPROPERTY()
    TArray<FMingFormationCommand> FormationCommandQueue;

    // ????活??編??
    UPROPERTY()
    TArray<int32> ActiveFormations;

    // ??否已??始??
    UPROPERTY()
    bool bInitialized;

private:
    // 載入??設編??模板
    void LoadDefaultFormationTemplates();

    // ????編??位置
    TArray<FVector> GenerateFormationPositions(EMingFormationType FormationType, int32 UnitCount, float Scale) const;

    // 計??編?X???
    TMap<FString, float> CalculateFormationBonuses(int32 FormationID) const;

    // 計??編??減??
    TMap<FString, float> CalculateFormationPenalties(int32 FormationID) const;

    // ??新編?X?X    void UpdateFormationStatus(int32 FormationID, EMingFormationStatus NewStatus};

    // ????編?X?令
    void ProcessFormationCommand(const FMingFormationCommand& Command};

    // 驗??編?X???
    bool ValidateFormationData(const FMingFormationData& Formation) const;

    // ??用編?X??X?單??    void ApplyFormationBonusesToUnits(int32 FormationID};

    // 移除編?X???從單??    void RemoveFormationBonusesFromUnits(int32 FormationID};

    // ????修復編??
    void AutoRepairFormation(int32 FormationID};

    // 計?X?佳編???X    EMingFormationType CalculateOptimalFormationType(const TArray<int32>& UnitIDs) const;

    // 檢查編??衝??
    bool CheckFormationConflict(int32 FormationID1, int32 FormationID2) const;

    // ????編??衝??
    void ResolveFormationConflict(int32 FormationID1, int32 FormationID2};
};

