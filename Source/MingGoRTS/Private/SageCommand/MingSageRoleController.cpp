#include "SageCommand/MingSageRoleController.h"
#include "SageCommand/MingSageRoles.h"
#include "SageCommand/MingSageCommandSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AMingSageRoleController::AMingSageRoleController()
{
    // 初始化組件
    SageRoles = nullptr;
    SageCommandSystem = nullptr;
    
    // 初始化角色發展數據
    RoleDevelopment.CurrentRole = ESageRoleType::TrueSage;
    RoleDevelopment.RoleState = ESageRoleState::RoleSelection;
    RoleDevelopment.ExperiencePoints = 0.0f;
    RoleDevelopment.Level = 1;
    RoleDevelopment.RoleComplianceScore = 100.0f;
    RoleDevelopment.LastRoleChange = FDateTime::Now();
    RoleDevelopment.TransformationCount = 0;
    RoleDevelopment.bIsTransforming = false;
    
    // 初始化配置參數
    ExperienceMultiplier = 1.0f;
    ComplianceDecayRate = 0.1f;
    MaxLevel = 100;
    TransformationDuration = 10.0f;
    
    // 初始化狀態變數
    TargetRole = ESageRoleType::TrueSage;
    TransformationProgress = 0.0f;
    bIsInitialized = false;
}

void AMingSageRoleController::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化系統
    InitializeSageRoles();
    InitializeSageCommandSystem();
    InitializeAbilities();
    
    bIsInitialized = true;
    
    // 設置定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            ComplianceUpdateTimerHandle,
            this,
            &AMingSageRoleController::UpdateRoleEffects,
            1.0f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            AbilityUpdateTimerHandle,
            this,
            &AMingSageRoleController::UpdateAbilityCooldowns,
            0.1f,
            true
        );
    }
}

void AMingSageRoleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bIsInitialized)
    {
        return;
    }
    
    // 處理角色轉換
    if (RoleDevelopment.bIsTransforming)
    {
        ProcessRoleTransformation(DeltaTime);
    }
    
    // 更新角色狀態
    UpdateRoleState();
    
    // 檢查升級
    CheckForLevelUp();
}

// 角色管理
bool AMingSageRoleController::SelectRole(ESageRoleType RoleType)
{
    if (!bIsInitialized || !SageRoles)
    {
        return false;
    }
    
    // 獲取角色特徵
    FSageRoleCharacteristics Characteristics = SageRoles->DefineRoleByType(RoleType);
    
    // 評估角色
    FSageRoleEvaluation Evaluation = SageRoles->EvaluateRoleCompliance(RoleType, Characteristics);
    
    // 檢查是否可以選擇此角色
    if (Evaluation.ComplianceLevel >= ERoleStandardLevel::Poor)
    {
        RoleDevelopment.CurrentRole = RoleType;
        RoleDevelopment.RoleState = ESageRoleState::RoleDevelopment;
        RoleDevelopment.LastRoleChange = FDateTime::Now();
        
        // 配置聖者指揮學系統
        ConfigureSageCommandSystem(RoleType);
        
        // 初始化角色能力
        InitializeRoleAbilities(RoleType);
        
        // 觸發事件
        OnRoleSelected.Broadcast(RoleType);
        
        return true;
    }
    
    return false;
}

ESageRoleType AMingSageRoleController::GetCurrentRole() const
{
    return RoleDevelopment.CurrentRole;
}

FSageRoleDevelopment AMingSageRoleController::GetRoleDevelopment() const
{
    return RoleDevelopment;
}

bool AMingSageRoleController::CanTransformToRole(ESageRoleType TargetRole)
{
    if (!SageRoles)
    {
        return false;
    }
    
    return SageRoles->CanTransformRole(RoleDevelopment.CurrentRole, TargetRole);
}

bool AMingSageRoleController::StartRoleTransformation(ESageRoleType TargetRole)
{
    if (!bIsInitialized || !SageRoles)
    {
        return false;
    }
    
    if (RoleDevelopment.bIsTransforming)
    {
        return false; // 已經在轉換中
    }
    
    // 檢查是否可以轉換
    if (!CanTransformToRole(TargetRole))
    {
        return false;
    }
    
    // 開始轉換
    this->TargetRole = TargetRole;
    RoleDevelopment.bIsTransforming = true;
    RoleDevelopment.RoleState = ESageRoleState::RoleTransformation;
    TransformationProgress = 0.0f;
    
    // 設置轉換定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            TransformationTimerHandle,
            this,
            &AMingSageRoleController::CompleteRoleTransformation,
            TransformationDuration,
            false
        );
    }
    
    // 觸發事件
    OnRoleTransformationStarted.Broadcast(RoleDevelopment.CurrentRole, TargetRole);
    
    return true;
}

void AMingSageRoleController::CompleteRoleTransformation()
{
    if (!RoleDevelopment.bIsTransforming)
    {
        return;
    }
    
    ESageRoleType PreviousRole = RoleDevelopment.CurrentRole;
    
    // 應用轉換效果
    ApplyTransformationEffects(PreviousRole, TargetRole);
    
    // 更新角色
    RoleDevelopment.CurrentRole = TargetRole;
    RoleDevelopment.bIsTransforming = false;
    RoleDevelopment.RoleState = ESageRoleState::RoleDevelopment;
    RoleDevelopment.TransformationCount++;
    RoleDevelopment.LastRoleChange = FDateTime::Now();
    
    // 配置聖者指揮學系統
    ConfigureSageCommandSystem(TargetRole);
    
    // 初始化新角色能力
    InitializeRoleAbilities(TargetRole);
    
    // 重置轉換進度
    TransformationProgress = 0.0f;
    
    // 觸發事件
    OnRoleTransformationCompleted.Broadcast(PreviousRole, TargetRole);
}

// 角色發展
void AMingSageRoleController::AddExperience(float Amount)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    RoleDevelopment.ExperiencePoints += Amount * ExperienceMultiplier;
    
    // 檢查是否可以升級
    CheckForLevelUp();
}

bool AMingSageRoleController::LevelUp()
{
    if (RoleDevelopment.Level >= MaxLevel)
    {
        return false;
    }
    
    RoleDevelopment.Level++;
    
    // 解鎖新能力
    UnlockNewAbilities();
    
    // 更新角色狀態
    if (RoleDevelopment.Level >= 50)
    {
        RoleDevelopment.RoleState = ESageRoleState::RoleMastery;
    }
    
    return true;
}

void AMingSageRoleController::UpdateRoleCompliance(float ComplianceChange)
{
    RoleDevelopment.RoleComplianceScore = FMath::Clamp(
        RoleDevelopment.RoleComplianceScore + ComplianceChange,
        0.0f,
        100.0f
    );
    
    // 檢查角色狀態變化
    if (RoleDevelopment.RoleComplianceScore < 30.0f)
    {
        RoleDevelopment.RoleState = ESageRoleState::RoleCorruption;
    }
    else if (RoleDevelopment.RoleComplianceScore < 60.0f && RoleDevelopment.RoleState == ESageRoleState::RoleCorruption)
    {
        RoleDevelopment.RoleState = ESageRoleState::RoleDevelopment;
    }
    
    // 觸發事件
    OnRoleComplianceChanged.Broadcast(RoleDevelopment.RoleComplianceScore);
}

bool AMingSageRoleController::UnlockAbility(const FString& AbilityID)
{
    if (!AvailableAbilities.Contains(AbilityID))
    {
        return false;
    }
    
    FSageRoleAbility& Ability = AvailableAbilities[AbilityID];
    
    // 檢查能力要求
    if (!CheckAbilityRequirements(Ability))
    {
        return false;
    }
    
    // 解鎖能力
    Ability.bIsUnlocked = true;
    RoleDevelopment.UnlockedAbilities.Add(AbilityID);
    
    // 觸發事件
    OnAbilityUnlocked.Broadcast(Ability);
    
    return true;
}

bool AMingSageRoleController::ActivateAbility(const FString& AbilityID)
{
    if (!AvailableAbilities.Contains(AbilityID))
    {
        return false;
    }
    
    FSageRoleAbility& Ability = AvailableAbilities[AbilityID];
    
    // 檢查能力是否已解鎖且不在冷卻中
    if (!Ability.bIsUnlocked || AbilityCooldowns.Contains(AbilityID))
    {
        return false;
    }
    
    // 激活能力
    Ability.bIsActive = true;
    RoleDevelopment.ActiveEffects.Add(AbilityID);
    
    // 設置冷卻時間
    if (Ability.CooldownTime > 0.0f)
    {
        AbilityCooldowns.Add(AbilityID, Ability.CooldownTime);
    }
    
    // 觸發事件
    OnAbilityActivated.Broadcast(Ability);
    
    return true;
}

void AMingSageRoleController::DeactivateAbility(const FString& AbilityID)
{
    if (!AvailableAbilities.Contains(AbilityID))
    {
        return;
    }
    
    FSageRoleAbility& Ability = AvailableAbilities[AbilityID];
    
    // 停用能力
    Ability.bIsActive = false;
    RoleDevelopment.ActiveEffects.Remove(AbilityID);
}

// 角色能力
TArray<FSageRoleAbility> AMingSageRoleController::GetAvailableAbilities() const
{
    TArray<FSageRoleAbility> Abilities;
    
    for (const auto& AbilityPair : AvailableAbilities)
    {
        Abilities.Add(AbilityPair.Value);
    }
    
    return Abilities;
}

TArray<FSageRoleAbility> AMingSageRoleController::GetUnlockedAbilities() const
{
    TArray<FSageRoleAbility> UnlockedAbilities;
    
    for (const auto& AbilityPair : AvailableAbilities)
    {
        if (AbilityPair.Value.bIsUnlocked)
        {
            UnlockedAbilities.Add(AbilityPair.Value);
        }
    }
    
    return UnlockedAbilities;
}

FSageRoleAbility AMingSageRoleController::GetAbility(const FString& AbilityID) const
{
    if (AvailableAbilities.Contains(AbilityID))
    {
        return AvailableAbilities[AbilityID];
    }
    
    return FSageRoleAbility();
}

bool AMingSageRoleController::IsAbilityUnlocked(const FString& AbilityID) const
{
    if (AvailableAbilities.Contains(AbilityID))
    {
        return AvailableAbilities[AbilityID].bIsUnlocked;
    }
    
    return false;
}

bool AMingSageRoleController::IsAbilityActive(const FString& AbilityID) const
{
    if (AvailableAbilities.Contains(AbilityID))
    {
        return AvailableAbilities[AbilityID].bIsActive;
    }
    
    return false;
}

// 角色評估
FSageRoleEvaluation AMingSageRoleController::EvaluateCurrentRole() const
{
    if (!SageRoles)
    {
        return FSageRoleEvaluation();
    }
    
    FSageRoleCharacteristics Characteristics = SageRoles->DefineRoleByType(RoleDevelopment.CurrentRole);
    return SageRoles->EvaluateRoleCompliance(RoleDevelopment.CurrentRole, Characteristics);
}

TArray<FString> AMingSageRoleController::GetRoleRecommendations() const
{
    TArray<FString> Recommendations;
    
    if (!SageRoles)
    {
        return Recommendations;
    }
    
    // 基於當前角色狀態生成建議
    if (RoleDevelopment.RoleComplianceScore < 50.0f)
    {
        Recommendations.Add(TEXT("建議加強道德權威建設"));
        Recommendations.Add(TEXT("避免邪惡行為"));
    }
    
    if (RoleDevelopment.Level < 10)
    {
        Recommendations.Add(TEXT("建議多完成任務獲得經驗"));
        Recommendations.Add(TEXT("解鎖基礎能力提升實力"));
    }
    
    if (RoleDevelopment.TransformationCount == 0)
    {
        Recommendations.Add(TEXT("嘗試角色轉換獲得新體驗"));
    }
    
    return Recommendations;
}

TArray<FString> AMingSageRoleController::GetRoleStrengths() const
{
    TArray<FString> Strengths;
    
    switch (RoleDevelopment.CurrentRole)
    {
    case ESageRoleType::TrueSage:
        Strengths.Add(TEXT("完美的道德權威"));
        Strengths.Add(TEXT("強大的戰略洞察"));
        Strengths.Add(TEXT("永恆的防墮機制"));
        break;
        
    case ESageRoleType::FalseSage:
        Strengths.Add(TEXT("善於偽裝和欺騙"));
        Strengths.Add(TEXT("精通人心策略"));
        Strengths.Add(TEXT("隱蔽行動能力"));
        break;
        
    case ESageRoleType::DemonKing:
        Strengths.Add(TEXT("絕對的力量優勢"));
        Strengths.Add(TEXT("高效的指揮執行"));
        Strengths.Add(TEXT("廣泛的恐懼影響"));
        break;
    }
    
    return Strengths;
}

TArray<FString> AMingSageRoleController::GetRoleWeaknesses() const
{
    TArray<FString> Weaknesses;
    
    switch (RoleDevelopment.CurrentRole)
    {
    case ESageRoleType::TrueSage:
        Weaknesses.Add(TEXT("可能過於理想主義"));
        Weaknesses.Add(TEXT("對邪惡手段缺乏準備"));
        break;
        
    case ESageRoleType::FalseSage:
        Weaknesses.Add(TEXT("內心陰暗易被識破"));
        Weaknesses.Add(TEXT("道德權威薄弱"));
        Weaknesses.Add(TEXT("缺乏真正的忠誠追隨者"));
        break;
        
    case ESageRoleType::DemonKing:
        Weaknesses.Add(TEXT("缺乏道德約束"));
        Weaknesses.Add(TEXT("容易引起反抗"));
        Weaknesses.Add(TEXT("長期統治不穩定"));
        break;
    }
    
    return Weaknesses;
}

// 聖者指揮學集成
void AMingSageRoleController::ApplyRoleToSageCommandSystem()
{
    if (!SageCommandSystem)
    {
        return;
    }
    
    ConfigureSageCommandSystem(RoleDevelopment.CurrentRole);
    SyncWithSageCommandSystem();
}

void AMingSageRoleController::SyncWithSageCommandSystem()
{
    if (!SageCommandSystem)
    {
        return;
    }
    
    // 綁定事件
    SageCommandSystem->OnStrategicDecisionGenerated.AddDynamic(
        this, 
        &AMingSageRoleController::OnSageCommandDecisionGenerated
    );
    
    SageCommandSystem->OnMoralAuthorityChanged.AddDynamic(
        this, 
        &AMingSageRoleController::OnMoralAuthorityChanged
    );
    
    SageCommandSystem->OnElementRotated.AddDynamic(
        this, 
        &AMingSageRoleController::OnElementRotated
    );
    
    SageCommandSystem->OnAlignmentChanged.AddDynamic(
        this, 
        &AMingSageRoleController::OnAlignmentChanged
    );
}

UMingSageCommandSystem* AMingSageRoleController::GetSageCommandSystem() const
{
    return SageCommandSystem;
}

// 私有方法實現

void AMingSageRoleController::InitializeSageRoles()
{
    if (!SageRoles)
    {
        SageRoles = NewObject<UMingSageRoles>(this);
    }
    
    SageRoles->Initialize();
}

void AMingSageRoleController::InitializeSageCommandSystem()
{
    if (!SageCommandSystem)
    {
        SageCommandSystem = NewObject<UMingSageCommandSystem>(this);
    }
    
    SageCommandSystem->Initialize();
    
    // 綁定事件
    SyncWithSageCommandSystem();
}

void AMingSageRoleController::InitializeAbilities()
{
    // 初始化基礎能力
    FSageRoleAbility BasicAbility;
    BasicAbility.AbilityID = TEXT("basic_command");
    BasicAbility.AbilityName = TEXT("基礎指揮");
    BasicAbility.Description = TEXT("基礎的指揮能力");
    BasicAbility.RequiredRole = ESageRoleType::TrueSage;
    BasicAbility.RequiredLevel = 1;
    BasicAbility.CooldownTime = 0.0f;
    BasicAbility.ManaCost = 0.0f;
    BasicAbility.Effects.Add(TEXT("指揮效果"));
    BasicAbility.bIsUnlocked = true;
    BasicAbility.bIsActive = false;
    
    AvailableAbilities.Add(BasicAbility.AbilityID, BasicAbility);
    
    // 根據當前角色初始化特定能力
    InitializeRoleAbilities(RoleDevelopment.CurrentRole);
}

void AMingSageRoleController::ProcessRoleTransformation(float DeltaTime)
{
    TransformationProgress += DeltaTime / TransformationDuration;
    
    // 檢查轉換是否完成
    if (TransformationProgress >= 1.0f)
    {
        CompleteRoleTransformation();
    }
}

void AMingSageRoleController::ApplyTransformationEffects(ESageRoleType FromRole, ESageRoleType ToRole)
{
    // 應用轉換效果
    switch (ToRole)
    {
    case ESageRoleType::TrueSage:
        // 轉換為至聖者：恢復道德權威
        RoleDevelopment.RoleComplianceScore = 100.0f;
        break;
        
    case ESageRoleType::FalseSage:
        // 轉換為偽聖者：降低道德但增加偽裝能力
        RoleDevelopment.RoleComplianceScore = 60.0f;
        break;
        
    case ESageRoleType::DemonKing:
        // 轉換為魔王：大幅降低道德但增加力量
        RoleDevelopment.RoleComplianceScore = 10.0f;
        break;
    }
}

void AMingSageRoleController::UpdateAbilityCooldowns(float DeltaTime)
{
    TArray<FString> AbilitiesToRemove;
    
    for (auto& CooldownPair : AbilityCooldowns)
    {
        CooldownPair.Value -= DeltaTime;
        
        if (CooldownPair.Value <= 0.0f)
        {
            AbilitiesToRemove.Add(CooldownPair.Key);
        }
    }
    
    // 移除已完成冷卻的能力
    for (const FString& AbilityID : AbilitiesToRemove)
    {
        AbilityCooldowns.Remove(AbilityID);
    }
}

void AMingSageRoleController::InitializeRoleAbilities(ESageRoleType RoleType)
{
    TArray<FString> RoleAbilities = GetRoleSpecificAbilities(RoleType);
    
    for (const FString& AbilityName : RoleAbilities)
    {
        FString AbilityID = GetRoleAbilityID(RoleType, AbilityName);
        
        FSageRoleAbility Ability;
        Ability.AbilityID = AbilityID;
        Ability.AbilityName = AbilityName;
        Ability.Description = FString::Printf(TEXT("%s的%s能力"), *GetRoleTypeName(RoleType), *AbilityName);
        Ability.RequiredRole = RoleType;
        Ability.RequiredLevel = 1;
        Ability.CooldownTime = 5.0f;
        Ability.ManaCost = 10.0f;
        Ability.bIsUnlocked = false;
        Ability.bIsActive = false;
        
        AvailableAbilities.Add(AbilityID, Ability);
    }
}

bool AMingSageRoleController::CheckAbilityRequirements(const FSageRoleAbility& Ability) const
{
    // 檢查角色類型要求
    if (Ability.RequiredRole != RoleDevelopment.CurrentRole)
    {
        return false;
    }
    
    // 檢查等級要求
    if (Ability.RequiredLevel > RoleDevelopment.Level)
    {
        return false;
    }
    
    return true;
}

void AMingSageRoleController::UpdateRoleState()
{
    // 根據角色合規分數更新狀態
    if (RoleDevelopment.RoleComplianceScore >= 80.0f)
    {
        if (RoleDevelopment.Level >= 50)
        {
            RoleDevelopment.RoleState = ESageRoleState::RoleMastery;
        }
        else
        {
            RoleDevelopment.RoleState = ESageRoleState::RoleDevelopment;
        }
    }
    else if (RoleDevelopment.RoleComplianceScore >= 30.0f)
    {
        RoleDevelopment.RoleState = ESageRoleState::RoleDevelopment;
    }
    else
    {
        RoleDevelopment.RoleState = ESageRoleState::RoleCorruption;
    }
}

void AMingSageRoleController::CheckForLevelUp()
{
    // 簡單的經驗值到等級轉換
    int32 RequiredExperience = RoleDevelopment.Level * 100;
    
    if (RoleDevelopment.ExperiencePoints >= RequiredExperience)
    {
        RoleDevelopment.ExperiencePoints -= RequiredExperience;
        LevelUp();
    }
}

void AMingSageRoleController::UpdateRoleEffects(float DeltaTime)
{
    // 應用角色效果衰減
    if (RoleDevelopment.RoleComplianceScore > 0.0f)
    {
        UpdateRoleCompliance(-ComplianceDecayRate * DeltaTime);
    }
}

void AMingSageRoleController::ConfigureSageCommandSystem(ESageRoleType RoleType)
{
    if (!SageCommandSystem)
    {
        return;
    }
    
    // 根據角色類型配置聖者指揮學系統
    switch (RoleType)
    {
    case ESageRoleType::TrueSage:
        SageCommandSystem->SetPrimaryAuthority(ECommandAuthorityType::MoralAuthority);
        SageCommandSystem->SetAlignment(EAlignmentAttribute::Righteous);
        SageCommandSystem->SetElementAffinity(EFiveElements::Earth);
        break;
        
    case ESageRoleType::FalseSage:
        SageCommandSystem->SetPrimaryAuthority(ECommandAuthorityType::CharismaticAuthority);
        SageCommandSystem->SetAlignment(EAlignmentAttribute::Evil);
        SageCommandSystem->SetElementAffinity(EFiveElements::Water);
        break;
        
    case ESageRoleType::DemonKing:
        SageCommandSystem->SetPrimaryAuthority(ECommandAuthorityType::SituationalAuthority);
        SageCommandSystem->SetAlignment(EAlignmentAttribute::Evil);
        SageCommandSystem->SetElementAffinity(EFiveElements::Fire);
        break;
    }
}

void AMingSageRoleController::UpdateSageCommandSystem()
{
    if (!SageCommandSystem)
    {
        return;
    }
    
    // 更新聖者指揮學系統參數
    SageCommandSystem->EnhanceAuthority(RoleDevelopment.Level * 0.1f);
    SageCommandSystem->ImproveCommandEffectiveness(RoleDevelopment.Level * 0.05f);
}

void AMingSageRoleController::OnSageCommandDecisionGenerated(const FStrategicDecision& Decision)
{
    // 處理聖者指揮學決策
    AddExperience(10.0f);
}

void AMingSageRoleController::OnMoralAuthorityChanged(const FMoralAuthorityMetrics& Metrics)
{
    // 處理道德權威變化
    UpdateRoleCompliance(Metrics.AuthorityLevel - 50.0f);
}

void AMingSageRoleController::OnElementRotated(EFiveElements NewElement)
{
    // 處理元素輪轉
    AddExperience(5.0f);
}

void AMingSageRoleController::OnAlignmentChanged(EAlignmentAttribute NewAlignment)
{
    // 處理屬性變化
    UpdateRoleCompliance(NewAlignment == EAlignmentAttribute::Righteous ? 10.0f : -10.0f);
}

FString AMingSageRoleController::GetRoleAbilityID(ESageRoleType RoleType, const FString& AbilityName) const
{
    return FString::Printf(TEXT("%s_%s"), *GetRoleTypeName(RoleType), *AbilityName);
}

TArray<FString> AMingSageRoleController::GetRoleSpecificAbilities(ESageRoleType RoleType) const
{
    TArray<FString> Abilities;
    
    switch (RoleType)
    {
    case ESageRoleType::TrueSage:
        Abilities.Add(TEXT("moral_guidance"));
        Abilities.Add(TEXT("divine_protection"));
        Abilities.Add(TEXT("wisdom_insight"));
        break;
        
    case ESageRoleType::FalseSage:
        Abilities.Add(TEXT("deception"));
        Abilities.Add(TEXT("manipulation"));
        Abilities.Add(TEXT("infiltration"));
        break;
        
    case ESageRoleType::DemonKing:
        Abilities.Add(TEXT("dominance"));
        Abilities.Add(TEXT("fear_aura"));
        Abilities.Add(TEXT("destruction"));
        break;
    }
    
    return Abilities;
}

float AMingSageRoleController::CalculateTransformationProgress() const
{
    return TransformationProgress;
}

bool AMingSageRoleController::IsTransformationComplete() const
{
    return TransformationProgress >= 1.0f;
}

void AMingSageRoleController::UnlockNewAbilities()
{
    // 根據等級解鎖新能力
    int32 NewAbilitiesCount = RoleDevelopment.Level / 10;
    
    TArray<FSageRoleAbility> Available = GetAvailableAbilities();
    
    for (int32 i = 0; i < Available.Num() && i < NewAbilitiesCount; ++i)
    {
        if (!Available[i].bIsUnlocked)
        {
            UnlockAbility(Available[i].AbilityID);
        }
    }
}
