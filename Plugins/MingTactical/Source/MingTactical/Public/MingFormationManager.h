#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.generated.h"

// 編隊命令類型
UENUM(BlueprintType)
enum class EMingFormationCommand : uint8
{
    CreateFormation,    // 創建編隊
    ChangeFormation,     // 變更編隊
    DisbandFormation,   // 解散編隊
    MergeFormations,     // 合併編隊
    SplitFormation,     // 分裂編隊
    RotateFormation,     // 旋轉編隊
    ScaleFormation,      // 縮放編隊
    MirrorFormation     // 鏡像編隊
};

// 編隊狀態
UENUM(BlueprintType)
enum class EMingFormationStatus : uint8
{
    Forming,           // 編隊中
    Active,            // 活躍
    Transitioning,      // 轉換中
    Disrupted,         // 被打亂
    Disbanded          // 已解散
};

// 編隊優先級
UENUM(BlueprintType)
enum class EMingFormationPriority : uint8
{
    Low,               // 低優先級
    Normal,            // 普通優先級
    High,              // 高優先級
    Critical           // 關鍵優先級
};

// 編隊單位角色
UENUM(BlueprintType)
enum class EMingUnitRole : uint8
{
    Leader,            // 領隊
    Vanguard,          // 先鋒
    Flanker,           // 側翼
    Support,           // 支援
    Rearguard,         // 後衛
    Reserve,           // 預備隊
    Artillery,         // 砲兵
    Scout,             // 偵察
    Medic              // 醫療兵
};

// 編隊數據
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationData
{
    GENERATED_BODY()

    // 編隊ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FormationID;

    // 編隊名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FormationName;

    // 編隊類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // 編隊狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationStatus FormationStatus;

    // 編隊優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationPriority Priority;

    // 編隊中心點
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector FormationCenter;

    // 編隊方向
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator FormationRotation;

    // 編隊規模
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationScale;

    // 單位列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnitIDs;

    // 單位角色映射
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, EMingUnitRole> UnitRoles;

    // 編隊位置數組
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> FormationPositions;

    // 編隊加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationBonuses;

    // 編隊減益
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> FormationPenalties;

    // 創建時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CreationTime;

    // 最後更新時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastUpdateTime;

    // 編隊指揮官ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommanderID;

    // 備用指揮官ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BackupCommanderID;

    // 編隊經驗值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationExperience;

    // 編隊士氣
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationMorale;

    // 編隊紀律
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FormationDiscipline;

    // 是否為靜態編隊
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsStatic;

    // 是否為自動編隊
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsAutoManaged;

    FMingFormationData()
    {
        FormationID = -1;
        FormationName = TEXT("");
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

// 編隊命令數據
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationCommand
{
    GENERATED_BODY()

    // 命令ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CommandID;

    // 命令類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationCommand CommandType;

    // 目標編隊ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetFormationID;

    // 命令參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CommandParameters;

    // 命令優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationPriority Priority;

    // 執行時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // 命令描述
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
        CommandDescription = TEXT("");
    }
};

// 編隊模板
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFormationTemplate
{
    GENERATED_BODY()

    // 模板ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TemplateID;

    // 模板名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateName;

    // 模板描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TemplateDescription;

    // 編隊類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFormationType FormationType;

    // 推薦單位類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EMingUnitType> RecommendedUnitTypes;

    // 最小單位數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinUnitCount;

    // 最大單位數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxUnitCount;

    // 標準間距
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardSpacing;

    // 標準深度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardDepth;

    // 標準寬度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StandardWidth;

    // 模板加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> TemplateBonuses;

    // 模板使用條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UsageConditions;

    // 是否為歷史編隊
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHistorical;

    // 歷史背景
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    FMingFormationTemplate()
    {
        TemplateID = -1;
        TemplateName = TEXT("");
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
        HistoricalBackground = TEXT("");
    }
};

// 編隊管理事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFormationCreated, const FMingFormationData&, Formation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFormationChanged, int32, FormationID, EMingFormationType, NewFormationType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFormationDisbanded, int32, FormationID, const TArray<int32>&, ReleasedUnits);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFormationUnitAdded, int32, FormationID, int32, UnitID, EMingUnitRole, Role);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFormationUnitRemoved, int32, FormationID, int32, UnitID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFormationCommandExecuted, const FMingFormationCommand&, Command);

/**
 * 編隊管理器
 * 負責管理戰術編隊的創建、變更和維護
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingFormationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFormationManager();

    // 初始化編隊管理器
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool InitializeFormationManager();

    // 創建編隊
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    int32 CreateFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType, const FString& FormationName);

    // 解散編隊
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool DisbandFormation(int32 FormationID);

    // 變更編隊類型
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ChangeFormationType(int32 FormationID, EMingFormationType NewFormationType);

    // 添加單位到編隊
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool AddUnitToFormation(int32 FormationID, int32 UnitID, EMingUnitRole Role);

    // 從編隊移除單位
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RemoveUnitFromFormation(int32 FormationID, int32 UnitID);

    // 設置編隊指揮官
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool SetFormationCommander(int32 FormationID, int32 CommanderID);

    // 旋轉編隊
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RotateFormation(int32 FormationID, const FRotator& NewRotation);

    // 縮放編隊
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ScaleFormation(int32 FormationID, float NewScale);

    // 移動編隊
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool MoveFormation(int32 FormationID, const FVector& TargetLocation);

    // 獲取編隊信息
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    FMingFormationData GetFormationInfo(int32 FormationID) const;

    // 獲取編隊列表
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TArray<int32> GetFormationList() const;

    // 獲取編隊模板
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TArray<FMingFormationTemplate> GetFormationTemplates() const;

    // 應用編隊模板
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool ApplyFormationTemplate(int32 FormationID, int32 TemplateID);

    // 獲取編隊統計
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    TMap<EMingFormationType, int32> GetFormationStatistics() const;

    // 計算編隊效率
    UFUNCTION(BlueprintPure, Category = "Formation Manager")
    float CalculateFormationEfficiency(int32 FormationID) const;

    // 檢查編隊完整性
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool CheckFormationIntegrity(int32 FormationID);

    // 修復編隊
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool RepairFormation(int32 FormationID);

    // 保存編隊數據
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool SaveFormationData();

    // 載入編隊數據
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    bool LoadFormationData();

    // 清除所有編隊
    UFUNCTION(BlueprintCallable, Category = "Formation Manager")
    void ClearAllFormations();

    // 事件委託
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
    // 編隊數據庫
    UPROPERTY()
    TMap<int32, FMingFormationData> FormationDatabase;

    // 編隊模板數據庫
    UPROPERTY()
    TMap<int32, FMingFormationTemplate> FormationTemplateDatabase;

    // 編隊命令隊列
    UPROPERTY()
    TArray<FMingFormationCommand> FormationCommandQueue;

    // 當前活躍編隊
    UPROPERTY()
    TArray<int32> ActiveFormations;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設編隊模板
    void LoadDefaultFormationTemplates();

    // 生成編隊位置
    TArray<FVector> GenerateFormationPositions(EMingFormationType FormationType, int32 UnitCount, float Scale) const;

    // 計算編隊加成
    TMap<FString, float> CalculateFormationBonuses(int32 FormationID) const;

    // 計算編隊減益
    TMap<FString, float> CalculateFormationPenalties(int32 FormationID) const;

    // 更新編隊狀態
    void UpdateFormationStatus(int32 FormationID, EMingFormationStatus NewStatus);

    // 處理編隊命令
    void ProcessFormationCommand(const FMingFormationCommand& Command);

    // 驗證編隊數據
    bool ValidateFormationData(const FMingFormationData& Formation) const;

    // 應用編隊加成到單位
    void ApplyFormationBonusesToUnits(int32 FormationID);

    // 移除編隊加成從單位
    void RemoveFormationBonusesFromUnits(int32 FormationID);

    // 自動修復編隊
    void AutoRepairFormation(int32 FormationID);

    // 計算最佳編隊類型
    EMingFormationType CalculateOptimalFormationType(const TArray<int32>& UnitIDs) const;

    // 檢查編隊衝突
    bool CheckFormationConflict(int32 FormationID1, int32 FormationID2) const;

    // 解決編隊衝突
    void ResolveFormationConflict(int32 FormationID1, int32 FormationID2);
};
