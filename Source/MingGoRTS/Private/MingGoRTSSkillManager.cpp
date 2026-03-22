#include "MingGoRTSSkillManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSSkillManager::UMingGoRTSSkillManager()
{
    AvailableSkillPoints = 0;
    ExperienceGainMultiplier = 1.0f;
    SkillPointGainMultiplier = 1.0f;
    bIsInitialized = false;
}

void UMingGoRTSSkillManager::InitializeSkillManager(UMingGoRTSSkillSystem* InSkillSystem, AMingGoRTSCharacter* InCharacter)
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能管理器已經初始化"));
        return;
    }

    SkillSystem = InSkillSystem;
    Character = InCharacter;

    if (!SkillSystem || !Character)
    {
        UE_LOG(LogTemp, Error, TEXT("技能管理器初始化失敗：技能系統或角色為空"));
        return;
    }

    // 初始化技能系統
    SkillSystem->InitializeSkillSystem();

    // 綁定技能系統事件
    SkillSystem->OnSkillLevelUp.AddDynamic(this, &UMingGoRTSSkillManager::OnSkillLevelUp);
    SkillSystem->OnSkillUnlocked.AddDynamic(this, &UMingGoRTSSkillManager::OnSkillUnlocked);

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("技能管理器初始化完成"));
}

void UMingGoRTSSkillManager::GrantSkillExperience(const FName& SkillID, int32 Amount, const FString& Reason)
{
    if (!bIsInitialized || !SkillSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能管理器未初始化，無法給予經驗"));
        return;
    }

    if (Amount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("經驗值必須為正數"));
        return;
    }

    // 應用經驗倍率
    int32 AdjustedAmount = static_cast<int32>(Amount * ExperienceGainMultiplier);

    // 給予技能經驗
    SkillSystem->AddSkillExperience(SkillID, AdjustedAmount);

    // 記錄經驗歷史
    AddExperienceToHistory(SkillID, AdjustedAmount, Reason);

    // 廣播經驗獲得事件
    OnSkillExperienceGained.Broadcast(SkillID, AdjustedAmount);

    // 檢查是否獲得技能點
    int32 SkillPointsGained = CalculateSkillPointReward(AdjustedAmount);
    if (SkillPointsGained > 0)
    {
        GrantSkillPoints(SkillPointsGained, FString::Printf(TEXT("技能經驗獎勵：%s"), *Reason));
    }

    UE_LOG(LogTemp, Log, TEXT("技能 %s 獲得 %d 經驗，原因：%s"), *SkillID.ToString(), AdjustedAmount, *Reason);
}

void UMingGoRTSSkillManager::GrantSkillPoints(int32 Points, const FString& Reason)
{
    if (Points <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能點必須為正數"));
        return;
    }

    int32 OldPoints = AvailableSkillPoints;
    int32 AdjustedPoints = static_cast<int32>(Points * SkillPointGainMultiplier);
    AvailableSkillPoints += AdjustedPoints;

    // 廣播技能點變化事件
    OnSkillPointsChanged.Broadcast(OldPoints, AvailableSkillPoints);

    UE_LOG(LogTemp, Log, TEXT("獲得 %d 技能點，原因：%s，當前總點數：%d"), AdjustedPoints, *Reason, AvailableSkillPoints);
}

void UMingGoRTSSkillManager::ProcessCombatEnd(bool bVictory, int32 EnemyCount, float CombatDuration)
{
    if (!bIsInitialized)
    {
        return;
    }

    CalculateCombatExperience(bVictory, EnemyCount, CombatDuration);

    // 根據戰鬥結果給予額外獎勵
    if (bVictory)
    {
        GrantSkillPoints(1, TEXT("戰鬥勝利"));
    }
    else
    {
        GrantSkillExperience(FName("MeleeMastery"), 25, TEXT("戰鬥失敗經驗"));
    }
}

void UMingGoRTSSkillManager::ProcessQuestCompletion(const FString& QuestID, int32 QuestDifficulty)
{
    if (!bIsInitialized)
    {
        return;
    }

    CalculateQuestExperience(QuestID, QuestDifficulty);

    // 任務完成給予技能點獎勵
    int32 QuestPoints = FMath::Clamp(QuestDifficulty / 10, 1, 3);
    GrantSkillPoints(QuestPoints, FString::Printf(TEXT("任務完成：%s"), *QuestID));
}

void UMingGoRTSSkillManager::ProcessHistoricalEvent(const FString& EventID, const TArray<FName>& RelatedSkills)
{
    if (!bIsInitialized)
    {
        return;
    }

    CalculateEventExperience(EventID, RelatedSkills);

    // 歷史事件給予額外技能點
    GrantSkillPoints(1, FString::Printf(TEXT("歷史事件：%s"), *EventID));
}

void UMingGoRTSSkillManager::AutoDistributeExperience(int32 TotalExperience, ESkillCategory PrimaryCategory)
{
    if (!bIsInitialized || !SkillSystem)
    {
        return;
    }

    TMap<FName, int32> SkillExperienceMap;

    // 獲取已解鎖的技能
    TArray<FMingSkill> UnlockedSkills = SkillSystem->GetUnlockedSkills();

    // 計算每個技能應得的經驗
    for (const FMingSkill& Skill : UnlockedSkills)
    {
        float CategoryMultiplier = GetCategoryExperienceMultiplier(Skill.Category);
        int32 SkillExperience = static_cast<int32>((TotalExperience / UnlockedSkills.Num()) * CategoryMultiplier);

        // 主要類別技能獲得額外經驗
        if (Skill.Category == PrimaryCategory)
        {
            SkillExperience = static_cast<int32>(SkillExperience * 1.5f);
        }

        SkillExperienceMap.Add(Skill.SkillID, SkillExperience);
    }

    // 分配經驗
    DistributeExperienceToSkills(SkillExperienceMap);
}

void UMingGoRTSSkillManager::ProcessCharacterLevelUp(int32 NewLevel)
{
    if (!bIsInitialized)
    {
        return;
    }

    // 角色升級給予技能點獎勵
    int32 LevelUpPoints = 2; // 每級2點技能點
    GrantSkillPoints(LevelUpPoints, FString::Printf(TEXT("角色升級到 %d 級"), NewLevel));

    // 給予所有已解鎖技能一些經驗
    if (SkillSystem)
    {
        TArray<FMingSkill> UnlockedSkills = SkillSystem->GetUnlockedSkills();
        for (const FMingSkill& Skill : UnlockedSkills)
        {
            GrantSkillExperience(Skill.SkillID, 50, TEXT("角色升級獎勵"));
        }
    }
}

bool UMingGoRTSSkillManager::ValidateSkillAction(const FName& SkillID, ESkillActionType ActionType)
{
    if (!bIsInitialized || !SkillSystem)
    {
        return false;
    }

    switch (ActionType)
    {
    case ESkillActionType::Unlock:
        return SkillSystem->CanUnlockSkill(SkillID, Character->GetCharacterData());

    case ESkillActionType::Upgrade:
        return SkillSystem->CanUpgradeSkill(SkillID);

    case ESkillActionType::Use:
        {
            FMingSkill Skill = SkillSystem->GetSkill(SkillID);
            return Skill.bIsUnlocked && Skill.bIsActive;
        }

    case ESkillActionType::Reset:
        return true; // 重置總是允許

    default:
        return false;
    }
}

bool UMingGoRTSSkillManager::ExecuteSkillAction(const FName& SkillID, ESkillActionType ActionType)
{
    if (!ValidateSkillAction(SkillID, ActionType))
    {
        UE_LOG(LogTemp, Warning, TEXT("技能操作驗證失敗：%s"), *StaticEnum<ESkillActionType>()->GetValueAsString(ActionType));
        return false;
    }

    switch (ActionType)
    {
    case ESkillActionType::Unlock:
        {
            if (AvailableSkillPoints <= 0)
            {
                UE_LOG(LogTemp, Warning, TEXT("技能點不足，無法解鎖技能"));
                return false;
            }

            int32 OldPoints = AvailableSkillPoints;
            bool bSuccess = SkillSystem->UnlockSkill(SkillID);
            if (bSuccess)
            {
                AvailableSkillPoints--;
                OnSkillPointsChanged.Broadcast(OldPoints, AvailableSkillPoints);
                ApplySkillEffectsToCharacter();
            }
            return bSuccess;
        }

    case ESkillActionType::Upgrade:
        return SkillSystem->UpgradeSkill(SkillID);

    case ESkillActionType::Use:
        // TODO: 實現技能使用邏輯
        UE_LOG(LogTemp, Log, TEXT("使用技能：%s"), *SkillID.ToString());
        return true;

    case ESkillActionType::Reset:
        return ResetSkillSystem();

    default:
        return false;
    }
}

TArray<FSkillExperienceGain> UMingGoRTSSkillManager::GetSkillExperienceHistory(const FName& SkillID) const
{
    TArray<FSkillExperienceGain> SkillHistory;
    
    for (const FSkillExperienceGain& Gain : SkillExperienceHistory)
    {
        if (Gain.SkillID == SkillID)
        {
            SkillHistory.Add(Gain);
        }
    }
    
    return SkillHistory;
}

TArray<FName> UMingGoRTSSkillManager::GetRecommendedSkills() const
{
    TArray<FName> RecommendedSkills;

    if (!bIsInitialized || !SkillSystem)
    {
        return RecommendedSkills;
    }

    // 獲取可解鎖的技能
    TArray<FMingSkill> AvailableSkills = SkillSystem->GetAvailableSkills(Character->GetCharacterData());

    // 根據角色背景和當前專精推薦技能
    for (const FMingSkill& Skill : AvailableSkills)
    {
        // 優先推薦符合專精路徑的技能
        if (Skill.SpecializationPath == SkillSystem->GetCurrentSpecializationPath())
        {
            RecommendedSkills.Add(Skill.SkillID);
        }
    }

    return RecommendedSkills;
}

FString UMingGoRTSSkillManager::GetSkillDevelopmentAdvice() const
{
    if (!bIsInitialized || !SkillSystem)
    {
        return TEXT("技能系統未初始化");
    }

    TArray<ESkillCategory> SkillFocus = GetCharacterSkillFocus();
    TArray<FName> UnderdevelopedSkills = GetUnderdevelopedSkills();
    TArray<FName> UpgradeableSkills = GetUpgradeableSkills();

    FString Advice = TEXT("技能發展建議：\n");

    // 分析技能專注
    if (SkillFocus.Num() > 0)
    {
        Advice += FString::Printf(TEXT("• 你專注於 %s 類技能\n"), *StaticEnum<ESkillCategory>()->GetValueAsString(SkillFocus[0]));
    }

    // 建議升級技能
    if (UpgradeableSkills.Num() > 0)
    {
        Advice += FString::Printf(TEXT("• 有 %d 個技能可以升級\n"), UpgradeableSkills.Num());
    }

    // 建議新技能
    if (UnderdevelopedSkills.Num() > 0)
    {
        Advice += FString::Printf(TEXT("• 建議學習新技能來平衡發展\n"));
    }

    // 技能點建議
    if (AvailableSkillPoints > 0)
    {
        Advice += FString::Printf(TEXT("• 你有 %d 個可用技能點\n"), AvailableSkillPoints);
    }

    return Advice;
}

bool UMingGoRTSSkillManager::ResetSkillSystem()
{
    if (!bIsInitialized || !SkillSystem)
    {
        return false;
    }

    // 重置技能系統
    SkillSystem->ResetSkillSystem();

    // 返還技能點
    int32 RefundedPoints = 0;
    TArray<FMingSkill> AllSkills = SkillSystem->GetAllSkills();
    for (const FMingSkill& Skill : AllSkills)
    {
        if (Skill.bIsUnlocked)
        {
            RefundedPoints += Skill.CurrentLevel;
        }
    }

    AvailableSkillPoints += RefundedPoints;

    // 清空經驗歷史
    SkillExperienceHistory.Empty();

    UE_LOG(LogTemp, Log, TEXT("技能系統已重置，返還 %d 技能點"), RefundedPoints);
    return true;
}

bool UMingGoRTSSkillManager::SaveSkillManagerData(const FString& SaveSlotName)
{
    // TODO: 實現技能管理器數據保存
    UE_LOG(LogTemp, Log, TEXT("保存技能管理器數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSSkillManager::LoadSkillManagerData(const FString& SaveSlotName)
{
    // TODO: 實現技能管理器數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入技能管理器數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSSkillManager::AddExperienceToHistory(const FName& SkillID, int32 Amount, const FString& Reason)
{
    FSkillExperienceGain ExperienceGain;
    ExperienceGain.SkillID = SkillID;
    ExperienceGain.ExperienceAmount = Amount;
    ExperienceGain.GainReason = Reason;
    ExperienceGain.GainTime = FDateTime::Now();

    SkillExperienceHistory.Add(ExperienceGain);

    // 限制歷史記錄數量
    if (SkillExperienceHistory.Num() > 1000)
    {
        SkillExperienceHistory.RemoveAt(0);
    }
}

void UMingGoRTSSkillManager::CalculateCombatExperience(bool bVictory, int32 EnemyCount, float CombatDuration)
{
    // 基礎戰鬥經驗
    int32 BaseExperience = CalculateBaseExperience(EnemyCount, 1.0f);

    // 勝利加成
    if (bVictory)
    {
        BaseExperience = static_cast<int32>(BaseExperience * 1.5f);
    }

    // 戰鬥長度調整
    float DurationMultiplier = FMath::Clamp(CombatDuration / 300.0f, 0.5f, 2.0f); // 5分鐘為基準
    BaseExperience = static_cast<int32>(BaseExperience * DurationMultiplier);

    // 分配給相關技能
    TMap<FName, int32> CombatExperienceMap;
    
    // 戰鬥技能獲得主要經驗
    CombatExperienceMap.Add(FName("TacticalCommand"), BaseExperience);
    CombatExperienceMap.Add(FName("MeleeMastery"), static_cast<int32>(BaseExperience * 0.8f));
    CombatExperienceMap.Add(FName("Leadership"), static_cast<int32>(BaseExperience * 0.6f));

    DistributeExperienceToSkills(CombatExperienceMap);
}

void UMingGoRTSSkillManager::CalculateQuestExperience(const FString& QuestID, int32 QuestDifficulty)
{
    int32 BaseExperience = CalculateBaseExperience(QuestDifficulty, 1.0f);

    // 根據任務類型分配經驗
    TMap<FName, int32> QuestExperienceMap;
    
    // 假設所有任務都給策略和外交經驗
    QuestExperienceMap.Add(FName("StrategicPlanning"), BaseExperience);
    QuestExperienceMap.Add(FName("Diplomacy"), static_cast<int32>(BaseExperience * 0.7f));

    DistributeExperienceToSkills(QuestExperienceMap);
}

void UMingGoRTSSkillManager::CalculateEventExperience(const FString& EventID, const TArray<FName>& RelatedSkills)
{
    int32 BaseExperience = CalculateBaseExperience(50, 1.0f); // 歷史事件基礎經驗

    TMap<FName, int32> EventExperienceMap;
    
    for (const FName& SkillID : RelatedSkills)
    {
        EventExperienceMap.Add(SkillID, BaseExperience);
    }

    DistributeExperienceToSkills(EventExperienceMap);
}

void UMingGoRTSSkillManager::DistributeExperienceToSkills(const TMap<FName, int32>& SkillExperienceMap)
{
    for (const auto& SkillExpPair : SkillExperienceMap)
    {
        GrantSkillExperience(SkillExpPair.Key, SkillExpPair.Value, TEXT("自動分配"));
    }
}

void UMingGoRTSSkillManager::UpdateSkillPoints(int32 OldPoints, int32 NewPoints)
{
    OnSkillPointsChanged.Broadcast(OldPoints, NewPoints);
}

void UMingGoRTSSkillManager::ApplySkillEffectsToCharacter()
{
    if (!Character || !SkillSystem)
    {
        return;
    }

    // TODO: 實現技能效果應用到角色屬性
    UE_LOG(LogTemp, Log, TEXT("應用技能效果到角色"));
}

void UMingGoRTSSkillManager::CheckSkillMilestones()
{
    // TODO: 實現技能里程碑檢查
    // 例如：達到特定技能等級時觸發特殊事件
}

int32 UMingGoRTSSkillManager::CalculateBaseExperience(int32 Difficulty, float DurationMultiplier) const
{
    return static_cast<int32>(10.0f * Difficulty * DurationMultiplier);
}

float UMingGoRTSSkillManager::GetCategoryExperienceMultiplier(ESkillCategory Category) const
{
    // 根據專精路徑返回經驗倍率
    switch (Category)
    {
    case ESkillCategory::Combat:
        return 1.2f;
    case ESkillCategory::Strategy:
        return 1.1f;
    case ESkillCategory::Diplomacy:
        return 1.0f;
    case ESkillCategory::Leadership:
        return 1.15f;
    case ESkillCategory::Intelligence:
        return 0.9f;
    case ESkillCategory::Economic:
        return 0.8f;
    default:
        return 1.0f;
    }
}

int32 UMingGoRTSSkillManager::CalculateSkillPointReward(int32 ExperienceGained) const
{
    // 每100點經驗給予1個技能點
    return ExperienceGained / 100;
}

TArray<ESkillCategory> UMingGoRTSSkillManager::GetCharacterSkillFocus() const
{
    TArray<ESkillCategory> SkillFocus;
    
    if (!bIsInitialized || !SkillSystem)
    {
        return SkillFocus;
    }

    // 分析角色技能專注方向
    TMap<ESkillCategory, int32> CategoryCount;
    
    TArray<FMingSkill> UnlockedSkills = SkillSystem->GetUnlockedSkills();
    for (const FMingSkill& Skill : UnlockedSkills)
    {
        CategoryCount.FindOrAdd(Skill.Category)++;
    }

    // 找出最多的技能類別
    int32 MaxCount = 0;
    for (const auto& CategoryPair : CategoryCount)
    {
        if (CategoryPair.Value > MaxCount)
        {
            MaxCount = CategoryPair.Value;
            SkillFocus.Empty();
            SkillFocus.Add(CategoryPair.Key);
        }
        else if (CategoryPair.Value == MaxCount)
        {
            SkillFocus.Add(CategoryPair.Key);
        }
    }

    return SkillFocus;
}

TArray<FName> UMingGoRTSSkillManager::GetUnderdevelopedSkills() const
{
    TArray<FName> UnderdevelopedSkills;
    
    if (!bIsInitialized || !SkillSystem)
    {
        return UnderdevelopedSkills;
    }

    // 找出等級較低的已解鎖技能
    TArray<FMingSkill> UnlockedSkills = SkillSystem->GetUnlockedSkills();
    for (const FMingSkill& Skill : UnlockedSkills)
    {
        if (Skill.CurrentLevel < Skill.MaxLevel / 2)
        {
            UnderdevelopedSkills.Add(Skill.SkillID);
        }
    }

    return UnderdevelopedSkills;
}

TArray<FName> UMingGoRTSSkillManager::GetUpgradeableSkills() const
{
    TArray<FName> UpgradeableSkills;
    
    if (!bIsInitialized || !SkillSystem)
    {
        return UpgradeableSkills;
    }

    // 找出可以升級的技能
    TArray<FMingSkill> UnlockedSkills = SkillSystem->GetUnlockedSkills();
    for (const FMingSkill& Skill : UnlockedSkills)
    {
        if (Skill.CurrentLevel < Skill.MaxLevel)
        {
            UpgradeableSkills.Add(Skill.SkillID);
        }
    }

    return UpgradeableSkills;
}
