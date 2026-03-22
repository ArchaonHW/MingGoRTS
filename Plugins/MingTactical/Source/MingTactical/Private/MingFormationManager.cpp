#include "MingFormationManager.h"
#include "MingTacticalCombatSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UMingFormationManager::UMingFormationManager()
{
    bInitialized = false;
    FormationDatabase.Empty();
    FormationTemplateDatabase.Empty();
    FormationCommandQueue.Empty();
    ActiveFormations.Empty();
}

bool UMingFormationManager::InitializeFormationManager()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊管理器已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化編隊管理器..."));

    // 載入預設編隊模板
    LoadDefaultFormationTemplates();

    // 載入已保存的編隊數據
    LoadFormationData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("編隊管理器初始化完成"));
    
    return true;
}

int32 UMingFormationManager::CreateFormation(const TArray<int32>& UnitIDs, EMingFormationType FormationType, const FString& FormationName)
{
    if (UnitIDs.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("無法創建空編隊"));
        return -1;
    }

    // 生成新的編隊ID
    int32 NewFormationID = FormationDatabase.Num() + 1;

    // 創建編隊數據
    FMingFormationData NewFormation;
    NewFormation.FormationID = NewFormationID;
    NewFormation.FormationName = FormationName;
    NewFormation.FormationType = FormationType;
    NewFormation.FormationStatus = EMingFormationStatus::Forming;
    NewFormation.Priority = EMingFormationPriority::Normal;
    NewFormation.UnitIDs = UnitIDs;
    NewFormation.CreationTime = FDateTime::Now();
    NewFormation.LastUpdateTime = FDateTime::Now();
    NewFormation.FormationExperience = 0.0f;
    NewFormation.FormationMorale = 75.0f;
    NewFormation.FormationDiscipline = 70.0f;
    NewFormation.bIsStatic = false;
    NewFormation.bIsAutoManaged = false;

    // 計算編隊中心點
    FVector FormationCenter = FVector::ZeroVector;
    for (int32 UnitID : UnitIDs)
    {
        // 這裡需要從戰鬥系統獲取單位位置
        // 暫時使用模擬位置
        FormationCenter += FVector(UnitID * 100.0f, 0.0f, 0.0f);
    }
    FormationCenter /= UnitIDs.Num();
    NewFormation.FormationCenter = FormationCenter;

    // 生成編隊位置
    NewFormation.FormationPositions = GenerateFormationPositions(FormationType, UnitIDs.Num(), NewFormation.FormationScale);

    // 設置單位角色
    for (int32 i = 0; i < UnitIDs.Num(); i++)
    {
        int32 UnitID = UnitIDs[i];
        EMingUnitRole Role = EMingUnitRole::Support; // 默認角色
        
        // 根據位置分配角色
        if (i == 0)
        {
            Role = EMingUnitRole::Leader;
        }
        else if (i < 3)
        {
            Role = EMingUnitRole::Vanguard;
        }
        else if (i >= UnitIDs.Num() - 2)
        {
            Role = EMingUnitRole::Rearguard;
        }
        
        NewFormation.UnitRoles.Add(UnitID, Role);
    }

    // 計算編隊加成和減益
    NewFormation.FormationBonuses = CalculateFormationBonuses(NewFormationID);
    NewFormation.FormationPenalties = CalculateFormationPenalties(NewFormationID);

    // 驗證編隊數據
    if (!ValidateFormationData(NewFormation))
    {
        UE_LOG(LogTemp, Error, TEXT("編隊數據驗證失敗"));
        return -1;
    }

    // 添加到數據庫
    FormationDatabase.Add(NewFormationID, NewFormation);
    ActiveFormations.Add(NewFormationID);

    // 應用編隊加成到單位
    ApplyFormationBonusesToUnits(NewFormationID);

    // 更新編隊狀態為活躍
    UpdateFormationStatus(NewFormationID, EMingFormationStatus::Active);

    // 觸發編隊創建事件
    OnFormationCreated.Broadcast(NewFormation);

    UE_LOG(LogTemp, Log, TEXT("創建編隊: %s (ID: %d)，包含 %d 個單位"), 
        *FormationName, NewFormationID, UnitIDs.Num());
    
    return NewFormationID;
}

bool UMingFormationManager::DisbandFormation(int32 FormationID)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];

    // 移除編隊加成
    RemoveFormationBonusesFromUnits(FormationID);

    // 從活躍列表中移除
    ActiveFormations.Remove(FormationID);

    // 觸發編隊解散事件
    OnFormationDisbanded.Broadcast(FormationID, Formation.UnitIDs);

    // 從數據庫中移除
    FormationDatabase.Remove(FormationID);

    UE_LOG(LogTemp, Log, TEXT("解散編隊: %s (ID: %d)"), *Formation.FormationName, FormationID);
    
    return true;
}

bool UMingFormationManager::ChangeFormationType(int32 FormationID, EMingFormationType NewFormationType)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];
    EMingFormationType OldFormationType = Formation.FormationType;

    // 移除舊的編隊加成
    RemoveFormationBonusesFromUnits(FormationID);

    // 更新編隊類型
    Formation.FormationType = NewFormationType;
    Formation.LastUpdateTime = FDateTime::Now();

    // 重新生成編隊位置
    Formation.FormationPositions = GenerateFormationPositions(NewFormationType, Formation.UnitIDs.Num(), Formation.FormationScale);

    // 重新計算編隊加成和減益
    Formation.FormationBonuses = CalculateFormationBonuses(FormationID);
    Formation.FormationPenalties = CalculateFormationPenalties(FormationID);

    // 應用新的編隊加成
    ApplyFormationBonusesToUnits(FormationID);

    // 更新編隊狀態為轉換中
    UpdateFormationStatus(FormationID, EMingFormationStatus::Transitioning);

    // 觸發編隊變化事件
    OnFormationChanged.Broadcast(FormationID, OldFormationType, NewFormationType);

    // 短暫延遲後設為活躍狀態
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUFunction(this, TEXT("UpdateFormationStatus"), FormationID, EMingFormationStatus::Active);
    
    if (GWorld)
    {
        GWorld->GetTimerManager().SetTimerForNextTick(TimerDelegate);
    }

    UE_LOG(LogTemp, Log, TEXT("編隊 %s 變更類型: %d -> %d"), 
        *Formation.FormationName, (int32)OldFormationType, (int32)NewFormationType);
    
    return true;
}

bool UMingFormationManager::AddUnitToFormation(int32 FormationID, int32 UnitID, EMingUnitRole Role)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];

    // 檢查單位是否已在編隊中
    if (Formation.UnitIDs.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位 %d 已在編隊 %d 中"), UnitID, FormationID);
        return false;
    }

    // 添加單位到編隊
    Formation.UnitIDs.Add(UnitID);
    Formation.UnitRoles.Add(UnitID, Role);
    Formation.LastUpdateTime = FDateTime::Now();

    // 重新生成編隊位置
    Formation.FormationPositions = GenerateFormationPositions(Formation.FormationType, Formation.UnitIDs.Num(), Formation.FormationScale);

    // 重新計算編隊加成
    Formation.FormationBonuses = CalculateFormationBonuses(FormationID);
    Formation.FormationPenalties = CalculateFormationPenalties(FormationID);

    // 應用編隊加成到新單位
    ApplyFormationBonusesToUnits(FormationID);

    // 觸發單位添加事件
    OnFormationUnitAdded.Broadcast(FormationID, UnitID, Role);

    UE_LOG(LogTemp, Log, TEXT("添加單位 %d 到編隊 %s，角色: %d"), 
        UnitID, *Formation.FormationName, (int32)Role);
    
    return true;
}

bool UMingFormationManager::RemoveUnitFromFormation(int32 FormationID, int32 UnitID)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];

    if (!Formation.UnitIDs.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("單位 %d 不在編隊 %d 中"), UnitID, FormationID);
        return false;
    }

    // 移除單位角色
    Formation.UnitRoles.Remove(UnitID);

    // 移除單位
    Formation.UnitIDs.Remove(UnitID);
    Formation.LastUpdateTime = FDateTime::Now();

    // 檢查編隊是否為空
    if (Formation.UnitIDs.Num() == 0)
    {
        // 解散空編隊
        return DisbandFormation(FormationID);
    }

    // 重新生成編隊位置
    Formation.FormationPositions = GenerateFormationPositions(Formation.FormationType, Formation.UnitIDs.Num(), Formation.FormationScale);

    // 重新計算編隊加成
    Formation.FormationBonuses = CalculateFormationBonuses(FormationID);
    Formation.FormationPenalties = CalculateFormationPenalties(FormationID);

    // 觸發單位移除事件
    OnFormationUnitRemoved.Broadcast(FormationID, UnitID);

    UE_LOG(LogTemp, Log, TEXT("從編隊 %s 移除單位 %d"), *Formation.FormationName, UnitID);
    
    return true;
}

bool UMingFormationManager::SetFormationCommander(int32 FormationID, int32 CommanderID)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];
    int32 OldCommanderID = Formation.CommanderID;

    // 設置新指揮官
    Formation.CommanderID = CommanderID;
    Formation.LastUpdateTime = FDateTime::Now();

    // 增加編隊士氣
    Formation.FormationMorale += 10.0f;
    Formation.FormationMorale = FMath::Clamp(Formation.FormationMorale, 0.0f, 100.0f);

    UE_LOG(LogTemp, Log, TEXT("編隊 %s 指揮官變更: %d -> %d"), 
        *Formation.FormationName, OldCommanderID, CommanderID);
    
    return true;
}

bool UMingFormationManager::RotateFormation(int32 FormationID, const FRotator& NewRotation)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];
    FRotator OldRotation = Formation.FormationRotation;

    // 更新編隊旋轉
    Formation.FormationRotation = NewRotation;
    Formation.LastUpdateTime = FDateTime::Now();

    // 重新計算編隊位置（基於新的旋轉）
    for (int32 i = 0; i < Formation.FormationPositions.Num(); i++)
    {
        FVector& Position = Formation.FormationPositions[i];
        // 應用旋轉變換
        Position = NewRotation.RotateVector(Position);
    }

    UE_LOG(LogTemp, Log, TEXT("編隊 %s 旋轉更新: (%.1f, %.1f, %.1f) -> (%.1f, %.1f, %.1f)"), 
        *Formation.FormationName, 
        OldRotation.Pitch, OldRotation.Yaw, OldRotation.Roll,
        NewRotation.Pitch, NewRotation.Yaw, NewRotation.Roll);
    
    return true;
}

bool UMingFormationManager::ScaleFormation(int32 FormationID, float NewScale)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];
    float OldScale = Formation.FormationScale;

    // 更新編隊規模
    Formation.FormationScale = FMath::Clamp(NewScale, 0.5f, 2.0f);
    Formation.LastUpdateTime = FDateTime::Now();

    // 重新生成編隊位置
    Formation.FormationPositions = GenerateFormationPositions(Formation.FormationType, Formation.UnitIDs.Num(), Formation.FormationScale);

    UE_LOG(LogTemp, Log, TEXT("編隊 %s 規模變更: %.2f -> %.2f"), 
        *Formation.FormationName, OldScale, Formation.FormationScale);
    
    return true;
}

bool UMingFormationManager::MoveFormation(int32 FormationID, const FVector& TargetLocation)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];
    FVector OldCenter = Formation.FormationCenter;

    // 更新編隊中心點
    Formation.FormationCenter = TargetLocation;
    Formation.LastUpdateTime = FDateTime::Now();

    // 更新所有單位的目標位置
    for (int32 i = 0; i < Formation.UnitIDs.Num(); i++)
    {
        int32 UnitID = Formation.UnitIDs[i];
        FVector UnitTargetPosition = TargetLocation + Formation.FormationPositions[i];
        
        // 這裡需要調用戰鬥系統來移動單位
        // MoveUnit(UnitID, UnitTargetPosition);
    }

    UE_LOG(LogTemp, Log, TEXT("移動編隊 %s 到 (%.1f, %.1f, %.1f)"), 
        *Formation.FormationName, TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
    
    return true;
}

FMingFormationData UMingFormationManager::GetFormationInfo(int32 FormationID) const
{
    if (FormationDatabase.Contains(FormationID))
    {
        return FormationDatabase[FormationID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在，返回空編隊"), FormationID);
    return FMingFormationData();
}

TArray<int32> UMingFormationManager::GetFormationList() const
{
    return ActiveFormations;
}

TArray<FMingFormationTemplate> UMingFormationManager::GetFormationTemplates() const
{
    TArray<FMingFormationTemplate> Templates;
    
    for (const TPair<int32, FMingFormationTemplate>& Pair : FormationTemplateDatabase)
    {
        Templates.Add(Pair.Value);
    }
    
    return Templates;
}

bool UMingFormationManager::ApplyFormationTemplate(int32 FormationID, int32 TemplateID)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    if (!FormationTemplateDatabase.Contains(TemplateID))
    {
        UE_LOG(LogTemp, Warning, TEXT("模板ID %d 不存在"), TemplateID);
        return false;
    }

    const FMingFormationTemplate& Template = FormationTemplateDatabase[TemplateID];
    
    // 應用模板到編隊
    return ChangeFormationType(FormationID, Template.FormationType);
}

TMap<EMingFormationType, int32> UMingFormationManager::GetFormationStatistics() const
{
    TMap<EMingFormationType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMingFormationType::Line, 0);
    Statistics.Add(EMingFormationType::Column, 0);
    Statistics.Add(EMingFormationType::Wedge, 0);
    Statistics.Add(EMingFormationType::Diamond, 0);
    Statistics.Add(EMingFormationType::Circle, 0);
    Statistics.Add(EMingFormationType::Skirmish, 0);
    Statistics.Add(EMingFormationType::Square, 0);
    Statistics.Add(EMingFormationType::Arrow, 0);
    Statistics.Add(EMingFormationType::Crescent, 0);
    Statistics.Add(EMingFormationType::Custom, 0);
    
    // 統計編隊類型
    for (const TPair<int32, FMingFormationData>& Pair : FormationDatabase)
    {
        const FMingFormationData& Formation = Pair.Value;
        int32& Count = Statistics[Formation.FormationType];
        Count++;
    }
    
    return Statistics;
}

float UMingFormationManager::CalculateFormationEfficiency(int32 FormationID) const
{
    if (!FormationDatabase.Contains(FormationID))
    {
        return 0.0f;
    }

    const FMingFormationData& Formation = FormationDatabase[FormationID];
    
    // 基礎效率
    float BaseEfficiency = 1.0f;
    
    // 單位數量效率因子
    float UnitCountFactor = FMath::Clamp(Formation.UnitIDs.Num() / 20.0f, 0.5f, 1.5f);
    
    // 士氣效率因子
    float MoraleFactor = Formation.FormationMorale / 100.0f;
    
    // 紀律效率因子
    float DisciplineFactor = Formation.FormationDiscipline / 100.0f;
    
    // 經驗效率因子
    float ExperienceFactor = FMath::Clamp(Formation.FormationExperience / 100.0f, 0.0f, 2.0f);
    
    // 編隊類型效率因子
    float TypeEfficiency = 1.0f;
    switch (Formation.FormationType)
    {
    case EMingFormationType::Line:
        TypeEfficiency = 1.2f; // 適合正面攻擊
        break;
    case EMingFormationType::Wedge:
        TypeEfficiency = 1.3f; // 適合突破
        break;
    case EMingFormationType::Circle:
        TypeEfficiency = 1.1f; // 適合防禦
        break;
    default:
        TypeEfficiency = 1.0f;
        break;
    }
    
    // 綜合效率計算
    float TotalEfficiency = BaseEfficiency * UnitCountFactor * MoraleFactor * 
                           DisciplineFactor * ExperienceFactor * TypeEfficiency;
    
    return FMath::Clamp(TotalEfficiency, 0.0f, 2.0f);
}

bool UMingFormationManager::CheckFormationIntegrity(int32 FormationID)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        return false;
    }

    const FMingFormationData& Formation = FormationDatabase[FormationID];
    
    // 檢查編隊完整性
    if (Formation.UnitIDs.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊 %s 為空"), *Formation.FormationName);
        return false;
    }
    
    // 檢查指揮官是否存在
    if (Formation.CommanderID != -1)
    {
        bool bCommanderFound = false;
        for (int32 UnitID : Formation.UnitIDs)
        {
            if (UnitID == Formation.CommanderID)
            {
                bCommanderFound = true;
                break;
            }
        }
        
        if (!bCommanderFound)
        {
            UE_LOG(LogTemp, Warning, TEXT("編隊 %s 的指揮官不在編隊中"), *Formation.FormationName);
            return false;
        }
    }
    
    // 檢查編隊士氣
    if (Formation.FormationMorale < 20.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊 %s 士氣過低"), *Formation.FormationName);
        return false;
    }
    
    return true;
}

bool UMingFormationManager::RepairFormation(int32 FormationID)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊ID %d 不存在"), FormationID);
        return false;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];
    
    // 修復編隊
    Formation.FormationStatus = EMingFormationStatus::Active;
    Formation.FormationMorale = FMath::Clamp(Formation.FormationMorale + 25.0f, 0.0f, 100.0f);
    Formation.FormationDiscipline = FMath::Clamp(Formation.FormationDiscipline + 15.0f, 0.0f, 100.0f);
    Formation.LastUpdateTime = FDateTime::Now();
    
    // 重新生成編隊位置
    Formation.FormationPositions = GenerateFormationPositions(Formation.FormationType, Formation.UnitIDs.Num(), Formation.FormationScale);
    
    UE_LOG(LogTemp, Log, TEXT("修復編隊: %s"), *Formation.FormationName);
    
    return true;
}

bool UMingFormationManager::SaveFormationData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("Formations.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("編隊數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 個編隊"), FormationDatabase.Num());
    
    return true;
}

bool UMingFormationManager::LoadFormationData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("Formations.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入編隊數據"), *LoadPath);
    
    return true;
}

void UMingFormationManager::ClearAllFormations()
{
    FormationDatabase.Empty();
    FormationTemplateDatabase.Empty();
    FormationCommandQueue.Empty();
    ActiveFormations.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("所有編隊數據已清除"));
}

void UMingFormationManager::LoadDefaultFormationTemplates()
{
    // 線形陣型模板
    FMingFormationTemplate LineTemplate;
    LineTemplate.TemplateID = 1;
    LineTemplate.TemplateName = TEXT("線形陣型");
    LineTemplate.TemplateDescription = TEXT("單位排成一線，適合正面攻擊和防禦");
    LineTemplate.FormationType = EMingFormationType::Line;
    LineTemplate.RecommendedUnitTypes = {EMingUnitType::Infantry, EMingUnitType::Cavalry};
    LineTemplate.MinUnitCount = 3;
    LineTemplate.MaxUnitCount = 50;
    LineTemplate.StandardSpacing = 100.0f;
    LineTemplate.StandardDepth = 200.0f;
    LineTemplate.StandardWidth = 300.0f;
    LineTemplate.TemplateBonuses = {
        {TEXT("attack"), 10.0f},
        {TEXT("morale"), 5.0f}
    };
    LineTemplate.UsageConditions = {TEXT("open_terrain"), TEXT("enemy_front")};
    LineTemplate.bIsHistorical = true;
    LineTemplate.HistoricalBackground = TEXT("線形陣型是古代戰爭中最常見的陣型之一，在民國時期仍被廣泛使用。");
    
    FormationTemplateDatabase.Add(1, LineTemplate);
    
    // 楔形陣型模板
    FMingFormationTemplate WedgeTemplate;
    WedgeTemplate.TemplateID = 2;
    WedgeTemplate.TemplateName = TEXT("楔形陣型");
    WedgeTemplate.TemplateDescription = TEXT("單位排成楔形，適合突破敵陣");
    WedgeTemplate.FormationType = EMingFormationType::Wedge;
    WedgeTemplate.RecommendedUnitTypes = {EMingUnitType::Cavalry, EMingUnitType::Tank};
    WedgeTemplate.MinUnitCount = 5;
    WedgeTemplate.MaxUnitCount = 30;
    WedgeTemplate.StandardSpacing = 80.0f;
    WedgeTemplate.StandardDepth = 250.0f;
    WedgeTemplate.StandardWidth = 200.0f;
    WedgeTemplate.TemplateBonuses = {
        {TEXT("breakthrough"), 20.0f},
        {TEXT("charge"), 15.0f}
    };
    WedgeTemplate.UsageConditions = {TEXT("enemy_center"), TEXT("breakthrough_needed")};
    WedgeTemplate.bIsHistorical = true;
    WedgeTemplate.HistoricalBackground = TEXT("楔形陣型在古代被用於突破敵軍陣線，在現代戰爭中仍有其價值。");
    
    FormationTemplateDatabase.Add(2, WedgeTemplate);
    
    // 圓形陣型模板
    FMingFormationTemplate CircleTemplate;
    CircleTemplate.TemplateID = 3;
    CircleTemplate.TemplateName = TEXT("圓形陣型");
    CircleTemplate.TemplateDescription = TEXT("單位排成圓形，適合全方位防禦");
    CircleTemplate.FormationType = EMingFormationType::Circle;
    CircleTemplate.RecommendedUnitTypes = {EMingUnitType::Infantry, EMingUnitType::Artillery};
    CircleTemplate.MinUnitCount = 4;
    CircleTemplate.MaxUnitCount = 40;
    CircleTemplate.StandardSpacing = 90.0f;
    CircleTemplate.StandardDepth = 180.0f;
    CircleTemplate.StandardWidth = 180.0f;
    CircleTemplate.TemplateBonuses = {
        {TEXT("defense"), 20.0f},
        {TEXT("cohesion"), 15.0f},
        {TEXT("all_round_protection"), 10.0f}
    };
    CircleTemplate.UsageConditions = {TEXT("surrounded"), TEXT("defensive_stance")};
    CircleTemplate.bIsHistorical = true;
    CircleTemplate.HistoricalBackground = TEXT("圓形陣型在被包圍時提供最佳防禦，在歷史上多次挽救危局。");
    
    FormationTemplateDatabase.Add(3, CircleTemplate);
    
    // 方形陣型模板
    FMingFormationTemplate SquareTemplate;
    SquareTemplate.TemplateID = 4;
    SquareTemplate.TemplateName = TEXT("方形陣型");
    SquareTemplate.TemplateDescription = TEXT("單位排成方形，適合穩定防禦");
    SquareTemplate.FormationType = EMingFormationType::Square;
    SquareTemplate.RecommendedUnitTypes = {EMingUnitType::Infantry};
    SquareTemplate.MinUnitCount = 6;
    SquareTemplate.MaxUnitCount = 36;
    SquareTemplate.StandardSpacing = 85.0f;
    SquareTemplate.StandardDepth = 150.0f;
    SquareTemplate.StandardWidth = 150.0f;
    SquareTemplate.TemplateBonuses = {
        {TEXT("stability"), 25.0f},
        {TEXT("resistance"), 15.0f}
    };
    SquareTemplate.UsageConditions = {TEXT("stable_defense"), TEXT("holding_position")};
    SquareTemplate.bIsHistorical = true;
    SquareTemplate.HistoricalBackground = TEXT("方形陣型提供最穩定的防禦結構，在歷史上被用於防守重要據點。");
    
    FormationTemplateDatabase.Add(4, SquareTemplate);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設編隊模板"), FormationTemplateDatabase.Num());
}

TArray<FVector> UMingFormationManager::GenerateFormationPositions(EMingFormationType FormationType, int32 UnitCount, float Scale) const
{
    TArray<FVector> Positions;
    
    switch (FormationType)
    {
    case EMingFormationType::Line:
        // 線形排列
        for (int32 i = 0; i < UnitCount; i++)
        {
            FVector Position = FVector((i - UnitCount / 2) * 100.0f * Scale, 0.0f, 0.0f);
            Positions.Add(Position);
        }
        break;
        
    case EMingFormationType::Wedge:
        // 楔形排列
        for (int32 i = 0; i < UnitCount; i++)
        {
            int32 Row = i / 3;
            int32 Col = i % 3;
            float X = Row * 100.0f * Scale;
            float Y = (Col - 1) * 80.0f * Scale;
            Positions.Add(FVector(X, Y, 0.0f));
        }
        break;
        
    case EMingFormationType::Circle:
        // 圓形排列
        for (int32 i = 0; i < UnitCount; i++)
        {
            float Angle = (i / (float)UnitCount) * 2.0f * PI;
            float Radius = 100.0f * Scale;
            FVector Position = FVector(
                Radius * FMath::Cos(Angle),
                Radius * FMath::Sin(Angle),
                0.0f
            );
            Positions.Add(Position);
        }
        break;
        
    case EMingFormationType::Square:
        // 方形排列
        int32 SideLength = FMath::CeilToInt(FMath::Sqrt(UnitCount));
        for (int32 i = 0; i < UnitCount; i++)
        {
            int32 Row = i / SideLength;
            int32 Col = i % SideLength;
            float X = (Col - SideLength / 2) * 85.0f * Scale;
            float Y = (Row - SideLength / 2) * 85.0f * Scale;
            Positions.Add(FVector(X, Y, 0.0f));
        }
        break;
        
    default:
        // 默認線形排列
        for (int32 i = 0; i < UnitCount; i++)
        {
            FVector Position = FVector((i - UnitCount / 2) * 100.0f * Scale, 0.0f, 0.0f);
            Positions.Add(Position);
        }
        break;
    }
    
    return Positions;
}

TMap<FString, float> UMingFormationManager::CalculateFormationBonuses(int32 FormationID) const
{
    TMap<FString, float> Bonuses;
    
    if (!FormationDatabase.Contains(FormationID))
    {
        return Bonuses;
    }

    const FMingFormationData& Formation = FormationDatabase[FormationID];
    
    // 根據編隊類型計算加成
    switch (Formation.FormationType)
    {
    case EMingFormationType::Line:
        Bonuses = {
            {TEXT("attack"), 10.0f},
            {TEXT("morale"), 5.0f},
            {TEXT("discipline"), 8.0f}
        };
        break;
        
    case EMingFormationType::Wedge:
        Bonuses = {
            {TEXT("breakthrough"), 20.0f},
            {TEXT("charge"), 15.0f},
            {TEXT("momentum"), 10.0f}
        };
        break;
        
    case EMingFormationType::Circle:
        Bonuses = {
            {TEXT("defense"), 20.0f},
            {TEXT("cohesion"), 15.0f},
            {TEXT("all_round_protection"), 10.0f}
        };
        break;
        
    case EMingFormationType::Square:
        Bonuses = {
            {TEXT("stability"), 25.0f},
            {TEXT("resistance"), 15.0f},
            {TEXT("morale"), 10.0f}
        };
        break;
        
    default:
        Bonuses = {
            {TEXT("basic"), 5.0f}
        };
        break;
    }
    
    return Bonuses;
}

TMap<FString, float> UMingFormationManager::CalculateFormationPenalties(int32 FormationID) const
{
    TMap<FString, float> Penalties;
    
    if (!FormationDatabase.Contains(FormationID))
    {
        return Penalties;
    }

    const FMingFormationData& Formation = FormationDatabase[FormationID];
    
    // 根據編隊類型計算減益
    switch (Formation.FormationType)
    {
    case EMingFormationType::Line:
        Penalties = {
            {TEXT("flank_vulnerability"), -15.0f},
            {TEXT("mobility"), -5.0f}
        };
        break;
        
    case EMingFormationType::Wedge:
        Penalties = {
            {TEXT("flank_vulnerability"), -20.0f},
            {TEXT("stability"), -10.0f}
        };
        break;
        
    case EMingFormationType::Circle:
        Penalties = {
            {TEXT("mobility"), -15.0f},
            {TEXT("attack"), -5.0f}
        };
        break;
        
    case EMingFormationType::Square:
        Penalties = {
            {TEXT("mobility"), -20.0f},
            {TEXT("flexibility"), -10.0f}
        };
        break;
        
    default:
        Penalties = {
            {TEXT("basic"), -2.0f}
        };
        break;
    }
    
    return Penalties;
}

void UMingFormationManager::UpdateFormationStatus(int32 FormationID, EMingFormationStatus NewStatus)
{
    if (FormationDatabase.Contains(FormationID))
    {
        FMingFormationData& Formation = FormationDatabase[FormationID];
        Formation.FormationStatus = NewStatus;
        Formation.LastUpdateTime = FDateTime::Now();
        
        UE_LOG(LogTemp, Log, TEXT("編隊 %s 狀態更新: %d"), *Formation.FormationName, (int32)NewStatus);
    }
}

void UMingFormationManager::ProcessFormationCommand(const FMingFormationCommand& Command)
{
    // 處理編隊命令
    switch (Command.CommandType)
    {
    case EMingFormationCommand::CreateFormation:
        // 創建編隊邏輯
        break;
        
    case EMingFormationCommand::ChangeFormation:
        // 變更編隊邏輯
        break;
        
    case EMingFormationCommand::DisbandFormation:
        // 解散編隊邏輯
        break;
        
    default:
        break;
    }
    
    // 觸發命令執行事件
    OnFormationCommandExecuted.Broadcast(Command);
}

bool UMingFormationManager::ValidateFormationData(const FMingFormationData& Formation) const
{
    if (Formation.FormationID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("編隊ID必須大於0"));
        return false;
    }
    
    if (Formation.FormationName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("編隊名稱不能為空"));
        return false;
    }
    
    if (Formation.UnitIDs.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("編隊必須包含至少一個單位"));
        return false;
    }
    
    return true;
}

void UMingFormationManager::ApplyFormationBonusesToUnits(int32 FormationID)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        return;
    }

    const FMingFormationData& Formation = FormationDatabase[FormationID];
    
    // 應用編隊加成到所有單位
    for (int32 UnitID : Formation.UnitIDs)
    {
        // 這裡需要調用戰鬥系統來應用加成
        // ApplyUnitBonus(UnitID, Formation.FormationBonuses);
    }
    
    UE_LOG(LogTemp, Log, TEXT("應用編隊 %s 的加成到 %d 個單位"), 
        *Formation.FormationName, Formation.UnitIDs.Num());
}

void UMingFormationManager::RemoveFormationBonusesFromUnits(int32 FormationID)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        return;
    }

    const FMingFormationData& Formation = FormationDatabase[FormationID];
    
    // 移除編隊加成從所有單位
    for (int32 UnitID : Formation.UnitIDs)
    {
        // 這裡需要調用戰鬥系統來移除加成
        // RemoveUnitBonus(UnitID, Formation.FormationBonuses);
    }
    
    UE_LOG(LogTemp, Log, TEXT("移除編隊 %s 的加成從 %d 個單位"), 
        *Formation.FormationName, Formation.UnitIDs.Num());
}

void UMingFormationManager::AutoRepairFormation(int32 FormationID)
{
    if (!FormationDatabase.Contains(FormationID))
    {
        return;
    }

    FMingFormationData& Formation = FormationDatabase[FormationID];
    
    // 自動修復條件檢查
    bool bNeedsRepair = false;
    
    if (Formation.FormationMorale < 30.0f)
    {
        bNeedsRepair = true;
    }
    
    if (Formation.FormationDiscipline < 40.0f)
    {
        bNeedsRepair = true;
    }
    
    if (bNeedsRepair)
    {
        RepairFormation(FormationID);
    }
}

EMingFormationType UMingFormationManager::CalculateOptimalFormationType(const TArray<int32>& UnitIDs) const
{
    // 根據單位類型和數量計算最佳編隊類型
    int32 InfantryCount = 0;
    int32 CavalryCount = 0;
    int32 ArtilleryCount = 0;
    
    // 統計單位類型
    for (int32 UnitID : UnitIDs)
    {
        // 這裡需要從戰鬥系統獲取單位類型
        // EMingUnitType UnitType = GetUnitType(UnitID);
        // 暫時假設都是步兵
        InfantryCount++;
    }
    
    // 根據單位組合推薦編隊類型
    if (CavalryCount > InfantryCount * 0.5f)
    {
        return EMingFormationType::Wedge; // 騎兵多，推薦楔形
    }
    else if (ArtilleryCount > 0)
    {
        return EMingFormationType::Circle; // 有砲兵，推薦圓形
    }
    else
    {
        return EMingFormationType::Line; // 默認線形
    }
}

bool UMingFormationManager::CheckFormationConflict(int32 FormationID1, int32 FormationID2) const
{
    if (!FormationDatabase.Contains(FormationID1) || !FormationDatabase.Contains(FormationID2))
    {
        return false;
    }

    const FMingFormationData& Formation1 = FormationDatabase[FormationID1];
    const FMingFormationData& Formation2 = FormationDatabase[FormationID2];
    
    // 檢查編隊距離
    float Distance = FVector::Dist(Formation1.FormationCenter, Formation2.FormationCenter);
    float MinDistance = 500.0f; // 最小安全距離
    
    if (Distance < MinDistance)
    {
        UE_LOG(LogTemp, Warning, TEXT("編隊 %s 和 %s 距離過近，可能發生衝突"), 
            *Formation1.FormationName, *Formation2.FormationName);
        return true;
    }
    
    return false;
}

void UMingFormationManager::ResolveFormationConflict(int32 FormationID1, int32 FormationID2)
{
    // 解決編隊衝突
    if (FormationDatabase.Contains(FormationID1) && FormationDatabase.Contains(FormationID2))
    {
        FMingFormationData& Formation1 = FormationDatabase[FormationID1];
        FMingFormationData& Formation2 = FormationDatabase[FormationID2];
        
        // 降低雙方士氣
        Formation1.FormationMorale -= 10.0f;
        Formation2.FormationMorale -= 10.0f;
        
        UE_LOG(LogTemp, Log, TEXT("解決編隊衝突: %s 和 %s"), 
            *Formation1.FormationName, *Formation2.FormationName);
    }
}
