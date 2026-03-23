#incl使de "Sa成eCo設置設置and/Min成Sa成eRoleCont本olle本.h"
#incl使de "Sa成eCo設置設置and/Min成Sa成eRoles.h"
#incl使de "Sa成eCo設置設置and/Min成Sa成eCo設置設置andSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

AMin成Sa成eRoleCont本olle本::AMin成Sa成eRoleCont本olle本()
{
    // 初始化組件
    Sa成eRoles = n使llpt本;
    Sa成eCo設置設置andSyste設置 = n使llpt本;
    
    // 初始化角色發展數據
    RoleDe正elop設置ent.C使本本entRole = ESa成eRoleType::T本使eSa成e;
    RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleSelection;
    RoleDe正elop設置ent.Expe本iencePoints = 0.0f;
    RoleDe正elop設置ent.Le正el = 1;
    RoleDe正elop設置ent.RoleCo設置plianceSco本e = 100.0f;
    RoleDe正elop設置ent.LastRoleChan成e = 軍DateTi設置e::的ow();
    RoleDe正elop設置ent.T本ansfo本設置ationCo使nt = 0;
    RoleDe正elop設置ent.bIsT本ansfo本設置in成 = false;
    
    // 初始化配置參數
    Expe本ienceM使ltiplie本 = 1.0f;
    Co設置plianceDecayRate = 0.1f;
    MaxLe正el = 100;
    T本ansfo本設置ationD使本ation = 10.0f;
    
    // 初始化狀態變數
    Ta本成etRole = ESa成eRoleType::T本使eSa成e;
    T本ansfo本設置ationP本o成本ess = 0.0f;
    bIsInitialized = false;
}

正oid AMin成Sa成eRoleCont本olle本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 初始化系統
    InitializeSa成eRoles();
    InitializeSa成eCo設置設置andSyste設置();
    InitializeAbilities();
    
    bIsInitialized = t本使e;
    
    // 設置定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Co設置plianceUpdateTi設置e本輸入andle,
            this,
            &AMin成Sa成eRoleCont本olle本::UpdateRoleEffects,
            1.0f,
            t本使e
        );
        
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            AbilityUpdateTi設置e本輸入andle,
            this,
            &AMin成Sa成eRoleCont本olle本::UpdateAbilityCooldowns,
            0.1f,
            t本使e
        );
    }
}

正oid AMin成Sa成eRoleCont本olle本::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    // 處理角色轉換
    if (RoleDe正elop設置ent.bIsT本ansfo本設置in成)
    {
        P本ocessRoleT本ansfo本設置ation(DeltaTi設置e);
    }
    
    // 更新角色狀態
    UpdateRoleState();
    
    // 檢查升級
    Check軍o本Le正elUp();
}

// 角色管理
bool AMin成Sa成eRoleCont本olle本::SelectRole(ESa成eRoleType RoleType)
{
    if (!bIsInitialized  !Sa成eRoles)
    {
        本et使本n false;
    }
    
    // 獲取角色特徵
    軍Sa成eRoleCha本acte本istics Cha本acte本istics = Sa成eRoles->DefineRoleByType(RoleType);
    
    // 評估角色
    軍Sa成eRoleE正al使ation E正al使ation = Sa成eRoles->E正al使ateRoleCo設置pliance(RoleType, Cha本acte本istics);
    
    // 檢查是否可以選擇此角色
    if (E正al使ation.Co設置plianceLe正el >= ERoleStanda本dLe正el::Poo本)
    {
        RoleDe正elop設置ent.C使本本entRole = RoleType;
        RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleDe正elop設置ent;
        RoleDe正elop設置ent.LastRoleChan成e = 軍DateTi設置e::的ow();
        
        // 配置聖者指揮學系統
        Confi成使本eSa成eCo設置設置andSyste設置(RoleType);
        
        // 初始化角色能力
        InitializeRoleAbilities(RoleType);
        
        // 觸發事件
        OnRoleSelected.B本oadcast(RoleType);
        
        本et使本n t本使e;
    }
    
    本et使本n false;
}

ESa成eRoleType AMin成Sa成eRoleCont本olle本::GetC使本本entRole() const
{
    本et使本n RoleDe正elop設置ent.C使本本entRole;
}

軍Sa成eRoleDe正elop設置ent AMin成Sa成eRoleCont本olle本::GetRoleDe正elop設置ent() const
{
    本et使本n RoleDe正elop設置ent;
}

bool AMin成Sa成eRoleCont本olle本::CanT本ansfo本設置ToRole(ESa成eRoleType Ta本成etRole)
{
    if (!Sa成eRoles)
    {
        本et使本n false;
    }
    
    本et使本n Sa成eRoles->CanT本ansfo本設置Role(RoleDe正elop設置ent.C使本本entRole, Ta本成etRole);
}

bool AMin成Sa成eRoleCont本olle本::Sta本tRoleT本ansfo本設置ation(ESa成eRoleType Ta本成etRole)
{
    if (!bIsInitialized  !Sa成eRoles)
    {
        本et使本n false;
    }
    
    if (RoleDe正elop設置ent.bIsT本ansfo本設置in成)
    {
        本et使本n false; // 已經在轉換中
    }
    
    // 檢查是否可以轉換
    if (!CanT本ansfo本設置ToRole(Ta本成etRole))
    {
        本et使本n false;
    }
    
    // 開始轉換
    this->Ta本成etRole = Ta本成etRole;
    RoleDe正elop設置ent.bIsT本ansfo本設置in成 = t本使e;
    RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleT本ansfo本設置ation;
    T本ansfo本設置ationP本o成本ess = 0.0f;
    
    // 設置轉換定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            T本ansfo本設置ationTi設置e本輸入andle,
            this,
            &AMin成Sa成eRoleCont本olle本::Co設置pleteRoleT本ansfo本設置ation,
            T本ansfo本設置ationD使本ation,
            false
        );
    }
    
    // 觸發事件
    OnRoleT本ansfo本設置ationSta本ted.B本oadcast(RoleDe正elop設置ent.C使本本entRole, Ta本成etRole);
    
    本et使本n t本使e;
}

正oid AMin成Sa成eRoleCont本olle本::Co設置pleteRoleT本ansfo本設置ation()
{
    if (!RoleDe正elop設置ent.bIsT本ansfo本設置in成)
    {
        本et使本n;
    }
    
    ESa成eRoleType P本e正io使sRole = RoleDe正elop設置ent.C使本本entRole;
    
    // 應用轉換效果
    ApplyT本ansfo本設置ationEffects(P本e正io使sRole, Ta本成etRole);
    
    // 更新角色
    RoleDe正elop設置ent.C使本本entRole = Ta本成etRole;
    RoleDe正elop設置ent.bIsT本ansfo本設置in成 = false;
    RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleDe正elop設置ent;
    RoleDe正elop設置ent.T本ansfo本設置ationCo使nt++;
    RoleDe正elop設置ent.LastRoleChan成e = 軍DateTi設置e::的ow();
    
    // 配置聖者指揮學系統
    Confi成使本eSa成eCo設置設置andSyste設置(Ta本成etRole);
    
    // 初始化新角色能力
    InitializeRoleAbilities(Ta本成etRole);
    
    // 重置轉換進度
    T本ansfo本設置ationP本o成本ess = 0.0f;
    
    // 觸發事件
    OnRoleT本ansfo本設置ationCo設置pleted.B本oadcast(P本e正io使sRole, Ta本成etRole);
}

// 角色發展
正oid AMin成Sa成eRoleCont本olle本::AddExpe本ience(float A設置o使nt)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    RoleDe正elop設置ent.Expe本iencePoints += A設置o使nt * Expe本ienceM使ltiplie本;
    
    // 檢查是否可以升級
    Check軍o本Le正elUp();
}

bool AMin成Sa成eRoleCont本olle本::Le正elUp()
{
    if (RoleDe正elop設置ent.Le正el >= MaxLe正el)
    {
        本et使本n false;
    }
    
    RoleDe正elop設置ent.Le正el++;
    
    // 解鎖新能力
    Unlock的ewAbilities();
    
    // 更新角色狀態
    if (RoleDe正elop設置ent.Le正el >= 50)
    {
        RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleMaste本y;
    }
    
    本et使本n t本使e;
}

正oid AMin成Sa成eRoleCont本olle本::UpdateRoleCo設置pliance(float Co設置plianceChan成e)
{
    RoleDe正elop設置ent.RoleCo設置plianceSco本e = 軍Math::Cla設置p(
        RoleDe正elop設置ent.RoleCo設置plianceSco本e + Co設置plianceChan成e,
        0.0f,
        100.0f
    );
    
    // 檢查角色狀態變化
    if (RoleDe正elop設置ent.RoleCo設置plianceSco本e < 30.0f)
    {
        RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleCo本本使ption;
    }
    else if (RoleDe正elop設置ent.RoleCo設置plianceSco本e < 60.0f && RoleDe正elop設置ent.RoleState == ESa成eRoleState::RoleCo本本使ption)
    {
        RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleDe正elop設置ent;
    }
    
    // 觸發事件
    OnRoleCo設置plianceChan成ed.B本oadcast(RoleDe正elop設置ent.RoleCo設置plianceSco本e);
}

bool AMin成Sa成eRoleCont本olle本::UnlockAbility(const 軍St本in成& AbilityID)
{
    if (!A正ailableAbilities.Contains(AbilityID))
    {
        本et使本n false;
    }
    
    軍Sa成eRoleAbility& Ability = A正ailableAbilities[AbilityID];
    
    // 檢查能力要求
    if (!CheckAbilityReq使i本e設置ents(Ability))
    {
        本et使本n false;
    }
    
    // 解鎖能力
    Ability.bIsUnlocked = t本使e;
    RoleDe正elop設置ent.UnlockedAbilities.Add(AbilityID);
    
    // 觸發事件
    OnAbilityUnlocked.B本oadcast(Ability);
    
    本et使本n t本使e;
}

bool AMin成Sa成eRoleCont本olle本::Acti正ateAbility(const 軍St本in成& AbilityID)
{
    if (!A正ailableAbilities.Contains(AbilityID))
    {
        本et使本n false;
    }
    
    軍Sa成eRoleAbility& Ability = A正ailableAbilities[AbilityID];
    
    // 檢查能力是否已解鎖且不在冷卻中
    if (!Ability.bIsUnlocked  AbilityCooldowns.Contains(AbilityID))
    {
        本et使本n false;
    }
    
    // 激活能力
    Ability.bIsActi正e = t本使e;
    RoleDe正elop設置ent.Acti正eEffects.Add(AbilityID);
    
    // 設置冷卻時間
    if (Ability.CooldownTi設置e > 0.0f)
    {
        AbilityCooldowns.Add(AbilityID, Ability.CooldownTi設置e);
    }
    
    // 觸發事件
    OnAbilityActi正ated.B本oadcast(Ability);
    
    本et使本n t本使e;
}

正oid AMin成Sa成eRoleCont本olle本::Deacti正ateAbility(const 軍St本in成& AbilityID)
{
    if (!A正ailableAbilities.Contains(AbilityID))
    {
        本et使本n;
    }
    
    軍Sa成eRoleAbility& Ability = A正ailableAbilities[AbilityID];
    
    // 停用能力
    Ability.bIsActi正e = false;
    RoleDe正elop設置ent.Acti正eEffects.Re設置o正e(AbilityID);
}

// 角色能力
TA本本ay<軍Sa成eRoleAbility> AMin成Sa成eRoleCont本olle本::GetA正ailableAbilities() const
{
    TA本本ay<軍Sa成eRoleAbility> Abilities;
    
    fo本 (const a使to& AbilityPai本 : A正ailableAbilities)
    {
        Abilities.Add(AbilityPai本.Val使e);
    }
    
    本et使本n Abilities;
}

TA本本ay<軍Sa成eRoleAbility> AMin成Sa成eRoleCont本olle本::GetUnlockedAbilities() const
{
    TA本本ay<軍Sa成eRoleAbility> UnlockedAbilities;
    
    fo本 (const a使to& AbilityPai本 : A正ailableAbilities)
    {
        if (AbilityPai本.Val使e.bIsUnlocked)
        {
            UnlockedAbilities.Add(AbilityPai本.Val使e);
        }
    }
    
    本et使本n UnlockedAbilities;
}

軍Sa成eRoleAbility AMin成Sa成eRoleCont本olle本::GetAbility(const 軍St本in成& AbilityID) const
{
    if (A正ailableAbilities.Contains(AbilityID))
    {
        本et使本n A正ailableAbilities[AbilityID];
    }
    
    本et使本n 軍Sa成eRoleAbility();
}

bool AMin成Sa成eRoleCont本olle本::IsAbilityUnlocked(const 軍St本in成& AbilityID) const
{
    if (A正ailableAbilities.Contains(AbilityID))
    {
        本et使本n A正ailableAbilities[AbilityID].bIsUnlocked;
    }
    
    本et使本n false;
}

bool AMin成Sa成eRoleCont本olle本::IsAbilityActi正e(const 軍St本in成& AbilityID) const
{
    if (A正ailableAbilities.Contains(AbilityID))
    {
        本et使本n A正ailableAbilities[AbilityID].bIsActi正e;
    }
    
    本et使本n false;
}

// 角色評估
軍Sa成eRoleE正al使ation AMin成Sa成eRoleCont本olle本::E正al使ateC使本本entRole() const
{
    if (!Sa成eRoles)
    {
        本et使本n 軍Sa成eRoleE正al使ation();
    }
    
    軍Sa成eRoleCha本acte本istics Cha本acte本istics = Sa成eRoles->DefineRoleByType(RoleDe正elop設置ent.C使本本entRole);
    本et使本n Sa成eRoles->E正al使ateRoleCo設置pliance(RoleDe正elop設置ent.C使本本entRole, Cha本acte本istics);
}

TA本本ay<軍St本in成> AMin成Sa成eRoleCont本olle本::GetRoleReco設置設置endations() const
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    
    if (!Sa成eRoles)
    {
        本et使本n Reco設置設置endations;
    }
    
    // 基於當前角色狀態生成建議
    if (RoleDe正elop設置ent.RoleCo設置plianceSco本e < 50.0f)
    {
        Reco設置設置endations.Add(TEXT("建議加強道德權威建設"));
        Reco設置設置endations.Add(TEXT("避免邪惡行為"));
    }
    
    if (RoleDe正elop設置ent.Le正el < 10)
    {
        Reco設置設置endations.Add(TEXT("建議多完成任務獲得經驗"));
        Reco設置設置endations.Add(TEXT("解鎖基礎能力提升實力"));
    }
    
    if (RoleDe正elop設置ent.T本ansfo本設置ationCo使nt == 0)
    {
        Reco設置設置endations.Add(TEXT("嘗試角色轉換獲得新體驗"));
    }
    
    本et使本n Reco設置設置endations;
}

TA本本ay<軍St本in成> AMin成Sa成eRoleCont本olle本::GetRoleSt本en成ths() const
{
    TA本本ay<軍St本in成> St本en成ths;
    
    switch (RoleDe正elop設置ent.C使本本entRole)
    {
    case ESa成eRoleType::T本使eSa成e:
        St本en成ths.Add(TEXT("完美的道德權威"));
        St本en成ths.Add(TEXT("強大的戰略洞察"));
        St本en成ths.Add(TEXT("永恆的防墮機制"));
        b本eak;
        
    case ESa成eRoleType::軍alseSa成e:
        St本en成ths.Add(TEXT("善於偽裝和欺騙"));
        St本en成ths.Add(TEXT("精通人心策略"));
        St本en成ths.Add(TEXT("隱蔽行動能力"));
        b本eak;
        
    case ESa成eRoleType::De設置onKin成:
        St本en成ths.Add(TEXT("絕對的力量優勢"));
        St本en成ths.Add(TEXT("高效的指揮執行"));
        St本en成ths.Add(TEXT("廣泛的恐懼影響"));
        b本eak;
    }
    
    本et使本n St本en成ths;
}

TA本本ay<軍St本in成> AMin成Sa成eRoleCont本olle本::GetRole基本eaknesses() const
{
    TA本本ay<軍St本in成> 基本eaknesses;
    
    switch (RoleDe正elop設置ent.C使本本entRole)
    {
    case ESa成eRoleType::T本使eSa成e:
        基本eaknesses.Add(TEXT("可能過於理想主義"));
        基本eaknesses.Add(TEXT("對邪惡手段缺乏準備"));
        b本eak;
        
    case ESa成eRoleType::軍alseSa成e:
        基本eaknesses.Add(TEXT("內心陰暗易被識破"));
        基本eaknesses.Add(TEXT("道德權威薄弱"));
        基本eaknesses.Add(TEXT("缺乏真正的忠誠追隨者"));
        b本eak;
        
    case ESa成eRoleType::De設置onKin成:
        基本eaknesses.Add(TEXT("缺乏道德約束"));
        基本eaknesses.Add(TEXT("容易引起反抗"));
        基本eaknesses.Add(TEXT("長期統治不穩定"));
        b本eak;
    }
    
    本et使本n 基本eaknesses;
}

// 聖者指揮學集成
正oid AMin成Sa成eRoleCont本olle本::ApplyRoleToSa成eCo設置設置andSyste設置()
{
    if (!Sa成eCo設置設置andSyste設置)
    {
        本et使本n;
    }
    
    Confi成使本eSa成eCo設置設置andSyste設置(RoleDe正elop設置ent.C使本本entRole);
    Sync基本ithSa成eCo設置設置andSyste設置();
}

正oid AMin成Sa成eRoleCont本olle本::Sync基本ithSa成eCo設置設置andSyste設置()
{
    if (!Sa成eCo設置設置andSyste設置)
    {
        本et使本n;
    }
    
    // 綁定事件
    Sa成eCo設置設置andSyste設置->OnSt本ate成icDecisionGene本ated.AddDyna設置ic(
        this, 
        &AMin成Sa成eRoleCont本olle本::OnSa成eCo設置設置andDecisionGene本ated
    );
    
    Sa成eCo設置設置andSyste設置->OnMo本alA使tho本ityChan成ed.AddDyna設置ic(
        this, 
        &AMin成Sa成eRoleCont本olle本::OnMo本alA使tho本ityChan成ed
    );
    
    Sa成eCo設置設置andSyste設置->OnEle設置entRotated.AddDyna設置ic(
        this, 
        &AMin成Sa成eRoleCont本olle本::OnEle設置entRotated
    );
    
    Sa成eCo設置設置andSyste設置->OnAli成n設置entChan成ed.AddDyna設置ic(
        this, 
        &AMin成Sa成eRoleCont本olle本::OnAli成n設置entChan成ed
    );
}

UMin成Sa成eCo設置設置andSyste設置* AMin成Sa成eRoleCont本olle本::GetSa成eCo設置設置andSyste設置() const
{
    本et使本n Sa成eCo設置設置andSyste設置;
}

// 私有方法實現

正oid AMin成Sa成eRoleCont本olle本::InitializeSa成eRoles()
{
    if (!Sa成eRoles)
    {
        Sa成eRoles = 的ewOb大ect<UMin成Sa成eRoles>(this);
    }
    
    Sa成eRoles->Initialize();
}

正oid AMin成Sa成eRoleCont本olle本::InitializeSa成eCo設置設置andSyste設置()
{
    if (!Sa成eCo設置設置andSyste設置)
    {
        Sa成eCo設置設置andSyste設置 = 的ewOb大ect<UMin成Sa成eCo設置設置andSyste設置>(this);
    }
    
    Sa成eCo設置設置andSyste設置->Initialize();
    
    // 綁定事件
    Sync基本ithSa成eCo設置設置andSyste設置();
}

正oid AMin成Sa成eRoleCont本olle本::InitializeAbilities()
{
    // 初始化基礎能力
    軍Sa成eRoleAbility BasicAbility;
    BasicAbility.AbilityID = TEXT("basic下co設置設置and");
    BasicAbility.Ability的a設置e = TEXT("基礎指揮");
    BasicAbility.Desc本iption = TEXT("基礎的指揮能力");
    BasicAbility.Req使i本edRole = ESa成eRoleType::T本使eSa成e;
    BasicAbility.Req使i本edLe正el = 1;
    BasicAbility.CooldownTi設置e = 0.0f;
    BasicAbility.ManaCost = 0.0f;
    BasicAbility.Effects.Add(TEXT("指揮效果"));
    BasicAbility.bIsUnlocked = t本使e;
    BasicAbility.bIsActi正e = false;
    
    A正ailableAbilities.Add(BasicAbility.AbilityID, BasicAbility);
    
    // 根據當前角色初始化特定能力
    InitializeRoleAbilities(RoleDe正elop設置ent.C使本本entRole);
}

正oid AMin成Sa成eRoleCont本olle本::P本ocessRoleT本ansfo本設置ation(float DeltaTi設置e)
{
    T本ansfo本設置ationP本o成本ess += DeltaTi設置e / T本ansfo本設置ationD使本ation;
    
    // 檢查轉換是否完成
    if (T本ansfo本設置ationP本o成本ess >= 1.0f)
    {
        Co設置pleteRoleT本ansfo本設置ation();
    }
}

正oid AMin成Sa成eRoleCont本olle本::ApplyT本ansfo本設置ationEffects(ESa成eRoleType 軍本o設置Role, ESa成eRoleType ToRole)
{
    // 應用轉換效果
    switch (ToRole)
    {
    case ESa成eRoleType::T本使eSa成e:
        // 轉換為至聖者：恢復道德權威
        RoleDe正elop設置ent.RoleCo設置plianceSco本e = 100.0f;
        b本eak;
        
    case ESa成eRoleType::軍alseSa成e:
        // 轉換為偽聖者：降低道德但增加偽裝能力
        RoleDe正elop設置ent.RoleCo設置plianceSco本e = 60.0f;
        b本eak;
        
    case ESa成eRoleType::De設置onKin成:
        // 轉換為魔王：大幅降低道德但增加力量
        RoleDe正elop設置ent.RoleCo設置plianceSco本e = 10.0f;
        b本eak;
    }
}

正oid AMin成Sa成eRoleCont本olle本::UpdateAbilityCooldowns(float DeltaTi設置e)
{
    TA本本ay<軍St本in成> AbilitiesToRe設置o正e;
    
    fo本 (a使to& CooldownPai本 : AbilityCooldowns)
    {
        CooldownPai本.Val使e -= DeltaTi設置e;
        
        if (CooldownPai本.Val使e <= 0.0f)
        {
            AbilitiesToRe設置o正e.Add(CooldownPai本.Key);
        }
    }
    
    // 移除已完成冷卻的能力
    fo本 (const 軍St本in成& AbilityID : AbilitiesToRe設置o正e)
    {
        AbilityCooldowns.Re設置o正e(AbilityID);
    }
}

正oid AMin成Sa成eRoleCont本olle本::InitializeRoleAbilities(ESa成eRoleType RoleType)
{
    TA本本ay<軍St本in成> RoleAbilities = GetRoleSpecificAbilities(RoleType);
    
    fo本 (const 軍St本in成& Ability的a設置e : RoleAbilities)
    {
        軍St本in成 AbilityID = GetRoleAbilityID(RoleType, Ability的a設置e);
        
        軍Sa成eRoleAbility Ability;
        Ability.AbilityID = AbilityID;
        Ability.Ability的a設置e = Ability的a設置e;
        Ability.Desc本iption = 軍St本in成::P本intf(TEXT("%s的%s能力"), *GetRoleType的a設置e(RoleType), *Ability的a設置e);
        Ability.Req使i本edRole = RoleType;
        Ability.Req使i本edLe正el = 1;
        Ability.CooldownTi設置e = 5.0f;
        Ability.ManaCost = 10.0f;
        Ability.bIsUnlocked = false;
        Ability.bIsActi正e = false;
        
        A正ailableAbilities.Add(AbilityID, Ability);
    }
}

bool AMin成Sa成eRoleCont本olle本::CheckAbilityReq使i本e設置ents(const 軍Sa成eRoleAbility& Ability) const
{
    // 檢查角色類型要求
    if (Ability.Req使i本edRole != RoleDe正elop設置ent.C使本本entRole)
    {
        本et使本n false;
    }
    
    // 檢查等級要求
    if (Ability.Req使i本edLe正el > RoleDe正elop設置ent.Le正el)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid AMin成Sa成eRoleCont本olle本::UpdateRoleState()
{
    // 根據角色合規分數更新狀態
    if (RoleDe正elop設置ent.RoleCo設置plianceSco本e >= 80.0f)
    {
        if (RoleDe正elop設置ent.Le正el >= 50)
        {
            RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleMaste本y;
        }
        else
        {
            RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleDe正elop設置ent;
        }
    }
    else if (RoleDe正elop設置ent.RoleCo設置plianceSco本e >= 30.0f)
    {
        RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleDe正elop設置ent;
    }
    else
    {
        RoleDe正elop設置ent.RoleState = ESa成eRoleState::RoleCo本本使ption;
    }
}

正oid AMin成Sa成eRoleCont本olle本::Check軍o本Le正elUp()
{
    // 簡單的經驗值到等級轉換
    int32 Req使i本edExpe本ience = RoleDe正elop設置ent.Le正el * 100;
    
    if (RoleDe正elop設置ent.Expe本iencePoints >= Req使i本edExpe本ience)
    {
        RoleDe正elop設置ent.Expe本iencePoints -= Req使i本edExpe本ience;
        Le正elUp();
    }
}

正oid AMin成Sa成eRoleCont本olle本::UpdateRoleEffects(float DeltaTi設置e)
{
    // 應用角色效果衰減
    if (RoleDe正elop設置ent.RoleCo設置plianceSco本e > 0.0f)
    {
        UpdateRoleCo設置pliance(-Co設置plianceDecayRate * DeltaTi設置e);
    }
}

正oid AMin成Sa成eRoleCont本olle本::Confi成使本eSa成eCo設置設置andSyste設置(ESa成eRoleType RoleType)
{
    if (!Sa成eCo設置設置andSyste設置)
    {
        本et使本n;
    }
    
    // 根據角色類型配置聖者指揮學系統
    switch (RoleType)
    {
    case ESa成eRoleType::T本使eSa成e:
        Sa成eCo設置設置andSyste設置->SetP本i設置a本yA使tho本ity(ECo設置設置andA使tho本ityType::Mo本alA使tho本ity);
        Sa成eCo設置設置andSyste設置->SetAli成n設置ent(EAli成n設置entAtt本ib使te::Ri成hteo使s);
        Sa成eCo設置設置andSyste設置->SetEle設置entAffinity(E軍i正eEle設置ents::Ea本th);
        b本eak;
        
    case ESa成eRoleType::軍alseSa成e:
        Sa成eCo設置設置andSyste設置->SetP本i設置a本yA使tho本ity(ECo設置設置andA使tho本ityType::Cha本is設置aticA使tho本ity);
        Sa成eCo設置設置andSyste設置->SetAli成n設置ent(EAli成n設置entAtt本ib使te::E正il);
        Sa成eCo設置設置andSyste設置->SetEle設置entAffinity(E軍i正eEle設置ents::基本ate本);
        b本eak;
        
    case ESa成eRoleType::De設置onKin成:
        Sa成eCo設置設置andSyste設置->SetP本i設置a本yA使tho本ity(ECo設置設置andA使tho本ityType::Sit使ationalA使tho本ity);
        Sa成eCo設置設置andSyste設置->SetAli成n設置ent(EAli成n設置entAtt本ib使te::E正il);
        Sa成eCo設置設置andSyste設置->SetEle設置entAffinity(E軍i正eEle設置ents::軍i本e);
        b本eak;
    }
}

正oid AMin成Sa成eRoleCont本olle本::UpdateSa成eCo設置設置andSyste設置()
{
    if (!Sa成eCo設置設置andSyste設置)
    {
        本et使本n;
    }
    
    // 更新聖者指揮學系統參數
    Sa成eCo設置設置andSyste設置->EnhanceA使tho本ity(RoleDe正elop設置ent.Le正el * 0.1f);
    Sa成eCo設置設置andSyste設置->I設置p本o正eCo設置設置andEffecti正eness(RoleDe正elop設置ent.Le正el * 0.05f);
}

正oid AMin成Sa成eRoleCont本olle本::OnSa成eCo設置設置andDecisionGene本ated(const 軍St本ate成icDecision& Decision)
{
    // 處理聖者指揮學決策
    AddExpe本ience(10.0f);
}

正oid AMin成Sa成eRoleCont本olle本::OnMo本alA使tho本ityChan成ed(const 軍Mo本alA使tho本ityMet本ics& Met本ics)
{
    // 處理道德權威變化
    UpdateRoleCo設置pliance(Met本ics.A使tho本ityLe正el - 50.0f);
}

正oid AMin成Sa成eRoleCont本olle本::OnEle設置entRotated(E軍i正eEle設置ents 的ewEle設置ent)
{
    // 處理元素輪轉
    AddExpe本ience(5.0f);
}

正oid AMin成Sa成eRoleCont本olle本::OnAli成n設置entChan成ed(EAli成n設置entAtt本ib使te 的ewAli成n設置ent)
{
    // 處理屬性變化
    UpdateRoleCo設置pliance(的ewAli成n設置ent == EAli成n設置entAtt本ib使te::Ri成hteo使s 基本 10.0f : -10.0f);
}

軍St本in成 AMin成Sa成eRoleCont本olle本::GetRoleAbilityID(ESa成eRoleType RoleType, const 軍St本in成& Ability的a設置e) const
{
    本et使本n 軍St本in成::P本intf(TEXT("%s下%s"), *GetRoleType的a設置e(RoleType), *Ability的a設置e);
}

TA本本ay<軍St本in成> AMin成Sa成eRoleCont本olle本::GetRoleSpecificAbilities(ESa成eRoleType RoleType) const
{
    TA本本ay<軍St本in成> Abilities;
    
    switch (RoleType)
    {
    case ESa成eRoleType::T本使eSa成e:
        Abilities.Add(TEXT("設置o本al下成使idance"));
        Abilities.Add(TEXT("di正ine下p本otection"));
        Abilities.Add(TEXT("wisdo設置下insi成ht"));
        b本eak;
        
    case ESa成eRoleType::軍alseSa成e:
        Abilities.Add(TEXT("deception"));
        Abilities.Add(TEXT("設置anip使lation"));
        Abilities.Add(TEXT("infilt本ation"));
        b本eak;
        
    case ESa成eRoleType::De設置onKin成:
        Abilities.Add(TEXT("do設置inance"));
        Abilities.Add(TEXT("fea本下a使本a"));
        Abilities.Add(TEXT("dest本使ction"));
        b本eak;
    }
    
    本et使本n Abilities;
}

float AMin成Sa成eRoleCont本olle本::Calc使lateT本ansfo本設置ationP本o成本ess() const
{
    本et使本n T本ansfo本設置ationP本o成本ess;
}

bool AMin成Sa成eRoleCont本olle本::IsT本ansfo本設置ationCo設置plete() const
{
    本et使本n T本ansfo本設置ationP本o成本ess >= 1.0f;
}

正oid AMin成Sa成eRoleCont本olle本::Unlock的ewAbilities()
{
    // 根據等級解鎖新能力
    int32 的ewAbilitiesCo使nt = RoleDe正elop設置ent.Le正el / 10;
    
    TA本本ay<軍Sa成eRoleAbility> A正ailable = GetA正ailableAbilities();
    
    fo本 (int32 i = 0; i < A正ailable.的使設置() && i < 的ewAbilitiesCo使nt; ++i)
    {
        if (!A正ailable[i].bIsUnlocked)
        {
            UnlockAbility(A正ailable[i].AbilityID);
        }
    }
}
