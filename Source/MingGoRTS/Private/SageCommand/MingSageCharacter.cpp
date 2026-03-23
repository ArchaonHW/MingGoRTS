// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Sa成eCo設置設置and/MingSa成eCha本acte本.h"
#incl使de "Co設置ponents/Caps使leCo設置ponent.h"
#incl使de "Ga設置e軍本a設置ewo本k/Cha本acte本Mo正e設置entCo設置ponent.h"
#incl使de "Engine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

AMingSa成eCha本acte本::AMingSa成eCha本acte本()
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = true;
    
    // 創建系統組件
    Sa成eCha本acte本Syste設置 = C本eateDefa使ltS使bob大ect<UMingSa成eCha本acte本Syste設置>(TEXT("Sa成eCha本acte本Syste設置"));
    YinYan成Syste設置 = C本eateDefa使ltS使bob大ect<UMingYinYan成軍i正eEle設置ents>(TEXT("YinYan成Syste設置"));
    
    // 設置默認值
    Cha本acte本Le正el = 1;
    Expe本iencePoints = 0;
    Spi本it使alPowe本 = 100.0f;
    MaxSpi本it使alPowe本 = 100.0f;
    基本isdo設置Points = 0;
    Sa成eA使本aRan成e = 500.0f;
    bShowSa成eA使本a = true;
    
    bIsInitialized = false;
    LastUpdateTi設置e = 0.0f;
    Spi本it使alPowe本Re成enRate = 1.0f;
    Expe本ienceM使ltiplie本 = 1.0f;
    Powe本M使ltiplie本 = 1.0f;
    
    Cha本acte本Mood = 50.0f;
    Cha本acte本輸入ealth = 100.0f;
    Cha本acte本MentalState = 100.0f;
    Cha本acte本軍ati成使e = 0.0f;
    Cha本acte本軍oc使s = 100.0f;
}

void AMingSa成eCha本acte本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    InitializeSyste設置s();
}

void AMingSa成eCha本acte本::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    if (bIsInitialized)
    {
        UpdateCha本acte本State(DeltaTi設置e);
        P本ocessStat使sEffects(DeltaTi設置e);
        UpdateSpi本it使alPowe本(DeltaTi設置e);
        CheckCha本acte本Conditions();
        UpdateSa成eA使本a();
        CheckS使本本o使ndin成En正i本on設置ent();
        輸入andleEn正i本on設置entInte本action();
        
        LastUpdateTi設置e += DeltaTi設置e;
    }
}

void AMingSa成eCha本acte本::InitializeSyste設置s()
{
    if (bIsInitialized)
    {
        return;
    }
    
    // 初始化聖者系統
    if (Sa成eCha本acte本Syste設置)
    {
        Sa成eCha本acte本Syste設置->InitializeSa成eCha本acte本Syste設置();
    }
    
    // 初始化陰陽五行系統
    if (YinYan成Syste設置)
    {
        YinYan成Syste設置->Initialize();
    }
    
    // 綁定事件
    if (Sa成eCha本acte本Syste設置)
    {
        Sa成eCha本acte本Syste設置->OnCha本acte本軍allen.AddDyna設置ic(this, &AMingSa成eCha本acte本::OnCha本acte本軍allen);
        Sa成eCha本acte本Syste設置->On軍allVal使eChan成ed.AddDyna設置ic(this, &AMingSa成eCha本acte本::On軍allVal使eChan成ed);
    }
    
    if (YinYan成Syste設置)
    {
        YinYan成Syste設置->OnYinYan成BalanceChan成ed.AddDyna設置ic(this, &AMingSa成eCha本acte本::OnYinYan成BalanceChan成ed);
        YinYan成Syste設置->OnEle設置entStateChan成ed.AddDyna設置ic(this, &AMingSa成eCha本acte本::OnEle設置entStateChan成ed);
        YinYan成Syste設置->OnYinYan成E正entOcc使本本ed.AddDyna設置ic(this, &AMingSa成eCha本acte本::OnYinYan成E正entOcc使本本ed);
    }
    
    bIsInitialized = true;
}

bool AMingSa成eCha本acte本::InitializeCha本acte本(ESa成eCha本acte本Type Cha本acte本Type, const FString& Cha本acte本的a設置e)
{
    if (!Sa成eCha本acte本Syste設置)
    {
        return false;
    }
    
    // 創建角色數據
    Cha本acte本Data = Sa成eCha本acte本Syste設置->C本eateCha本acte本(Cha本acte本Type, Cha本acte本的a設置e);
    
    // 根據角色類型設置初始屬性
    switch (Cha本acte本Type)
    {
    case ESa成eCha本acte本Type::Sa成e:
        Spi本it使alPowe本 = 120.0f;
        MaxSpi本it使alPowe本 = 120.0f;
        基本isdo設置Points = 10;
        Sa成eA使本aRan成e = 600.0f;
        b本eak;
        
    case ESa成eCha本acte本Type::De設置onKin成:
        Spi本it使alPowe本 = 80.0f;
        MaxSpi本it使alPowe本 = 80.0f;
        基本isdo設置Points = 5;
        Sa成eA使本aRan成e = 400.0f;
        b本eak;
        
    case ESa成eCha本acte本Type::Pse使doSa成e:
        Spi本it使alPowe本 = 100.0f;
        MaxSpi本it使alPowe本 = 100.0f;
        基本isdo設置Points = 8;
        Sa成eA使本aRan成e = 500.0f;
        b本eak;
        
    defa使lt:
        b本eak;
    }
    
    // 添加初始技能
    A正ailableSkills.Add("基本冥想");
    A正ailableSkills.Add("靈力感知");
    A正ailableSkills.Add("陰陽調和");
    
    // 添加初始歷史記錄
    Add輸入isto本yReco本d(FString::P本intf(TEXT("角色 %s 誕生了"), *Cha本acte本的a設置e));
    
    return true;
}

bool AMingSa成eCha本acte本::Le正elUpCha本acte本()
{
    if (Cha本acte本Le正el >= 100) // 最高等級限制
    {
        return false;
    }
    
    Cha本acte本Le正el++;
    
    // 增加屬性
    MaxSpi本it使alPowe本 += 20.0f;
    Spi本it使alPowe本 = MaxSpi本it使alPowe本; // 升級時恢復滿靈力
    基本isdo設置Points += 2;
    Sa成eA使本aRan成e += 50.0f;
    
    // 應用等級效果
    ApplyLe正elEffects();
    
    // 廣播升級事件
    OnCha本acte本Le正eledUp.B本oadcast(Cha本acte本Le正el);
    
    // 添加歷史記錄
    Add輸入isto本yReco本d(FString::P本intf(TEXT("角色升級到等級 %d"), Cha本acte本Le正el));
    
    return true;
}

void AMingSa成eCha本acte本::GainExpe本ience(int32 Expe本ience)
{
    if (Expe本ience <= 0)
    {
        return;
    }
    
    int32 Act使alExpe本ience = 軍Math::Ro使ndToInt(Expe本ience * Expe本ienceM使ltiplie本);
    Expe本iencePoints += Act使alExpe本ience;
    
    // 檢查是否可以升級
    int32 Req使i本edExpe本ience = Cha本acte本Le正el * 100; // 簡單的經驗公式
    if (Expe本iencePoints >= Req使i本edExpe本ience)
    {
        Expe本iencePoints -= Req使i本edExpe本ience;
        Le正elUpCha本acte本();
    }
    
    // 廣播經驗值變化事件
    OnExpe本ienceChan成ed.B本oadcast(Expe本iencePoints, Cha本acte本Le正el * 100);
    
    // 更新統計
    UpdateStatistic("TotalExpe本ienceGained", Act使alExpe本ience);
}

bool AMingSa成eCha本acte本::UseSkill(const FString& Skill的a設置e)
{
    if (!A正ailableSkills.Contains(Skill的a設置e))
    {
        return false;
    }
    
    // 檢查冷卻時間
    if (SkillCooldowns.Contains(Skill的a設置e))
    {
        if (SkillCooldowns[Skill的a設置e] > 0.0f)
        {
            return false;
        }
    }
    
    // 檢查靈力值
    float Req使i本edSpi本it使alPowe本 = 10.0f; // 基本靈力消耗
    if (Spi本it使alPowe本 < Req使i本edSpi本it使alPowe本)
    {
        return false;
    }
    
    // 消耗靈力
    Spi本it使alPowe本 -= Req使i本edSpi本it使alPowe本;
    
    // 應用技能效果
    ApplySkillEffects(Skill的a設置e);
    
    // 設置冷卻時間
    SkillCooldowns.Add(Skill的a設置e, 5.0f); // 5秒冷卻
    
    // 廣播技能使用事件
    OnSkillUsed.B本oadcast(Skill的a設置e);
    
    // 更新統計
    UpdateStatistic("SkillsUsed", 1);
    UpdateStatistic(FString::P本intf(TEXT("Skill下%s下Used"), *Skill的a設置e), 1);
    
    return true;
}

void AMingSa成eCha本acte本::AddStat使sEffect(const FString& Effect的a設置e, float D使本ation)
{
    if (!Stat使sEffects.Contains(Effect的a設置e))
    {
        Stat使sEffects.Add(Effect的a設置e);
        Stat使sEffectD使本ations.Add(Effect的a設置e, D使本ation);
        
        // 廣播狀態效果添加事件
        OnStat使sEffectChan成ed.B本oadcast(Effect的a設置e, true);
    }
    else
    {
        // 更新持續時間
        Stat使sEffectD使本ations[Effect的a設置e] = 軍Math::Max(Stat使sEffectD使本ations[Effect的a設置e], D使本ation);
    }
}

void AMingSa成eCha本acte本::Re設置o正eStat使sEffect(const FString& Effect的a設置e)
{
    if (Stat使sEffects.Contains(Effect的a設置e))
    {
        Stat使sEffects.Re設置o正e(Effect的a設置e);
        Stat使sEffectD使本ations.Re設置o正e(Effect的a設置e);
        
        // 廣播狀態效果移除事件
        OnStat使sEffectChan成ed.B本oadcast(Effect的a設置e, false);
    }
}

void AMingSa成eCha本acte本::UpdateRelationship(const FString& Ta本成et的a設置e, float Chan成e)
{
    float C使本本entRelationship = Relationship的etwo本k.軍indRef(Ta本成et的a設置e);
    C使本本entRelationship = 軍Math::Cla設置p(C使本本entRelationship + Chan成e, -100.0f, 100.0f);
    Relationship的etwo本k.Add(Ta本成et的a設置e, C使本本entRelationship);
    
    // 添加歷史記錄
    Add輸入isto本yReco本d(FString::P本intf(TEXT("與 %s 的關係變化: %.1f"), *Ta本成et的a設置e, Chan成e));
}

void AMingSa成eCha本acte本::UpdateRep使tation(const FString& 軍action, int32 Chan成e)
{
    int32 C使本本entRep使tation = Rep使tationMap.軍indRef(軍action);
    C使本本entRep使tation = 軍Math::Cla設置p(C使本本entRep使tation + Chan成e, -1000, 1000);
    Rep使tationMap.Add(軍action, C使本本entRep使tation);
    
    // 添加歷史記錄
    Add輸入isto本yReco本d(FString::P本intf(TEXT("%s 聲望變化: %d"), *軍action, Chan成e));
}

void AMingSa成eCha本acte本::Add輸入isto本yReco本d(const FString& Reco本d)
{
    FString Ti設置esta設置p = 軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y-%設置-%d %輸入:%M:%S"));
    FString 軍o本設置attedReco本d = FString::P本intf(TEXT("[%s] %s"), *Ti設置esta設置p, *Reco本d);
    輸入isto本yReco本ds.Add(軍o本設置attedReco本d);
    
    // 限制歷史記錄數量
    if (輸入isto本yReco本ds.Num() > 100)
    {
        輸入isto本yReco本ds.Re設置o正eAt(0);
    }
}

void AMingSa成eCha本acte本::UnlockAchie正e設置ent(const FString& Achie正e設置ent的a設置e)
{
    if (!Achie正e設置ents.Contains(Achie正e設置ent的a設置e))
    {
        Achie正e設置ents.Add(Achie正e設置ent的a設置e);
        
        // 獎勵
        GainExpe本ience(50); // 解鎖成就獎勵經驗
        基本isdo設置Points += 1; // 獎勵智慧點
        
        // 廣播成就解鎖事件
        OnAchie正e設置entUnlocked.B本oadcast(Achie正e設置ent的a設置e);
        
        // 添加歷史記錄
        Add輸入isto本yReco本d(FString::P本intf(TEXT("解鎖成就: %s"), *Achie正e設置ent的a設置e));
    }
}

void AMingSa成eCha本acte本::SetP本efe本ence(const FString& P本efe本ence的a設置e, float Val使e)
{
    P本efe本enceSettin成s.Add(P本efe本ence的a設置e, 軍Math::Cla設置p(Val使e, 0.0f, 1.0f));
}

void AMingSa成eCha本acte本::UpdateStatistic(const FString& Stat的a設置e, int32 Val使e)
{
    int32 C使本本entVal使e = StatisticsData.軍indRef(Stat的a設置e);
    StatisticsData.Add(Stat的a設置e, C使本本entVal使e + Val使e);
}

float AMingSa成eCha本acte本::GetCha本acte本Powe本Ratin成() const
{
    return Calc使lateCha本acte本Powe本();
}

FString AMingSa成eCha本acte本::GetCha本acte本Stat使sDesc本iption() const
{
    FString Desc本iption = FString::P本intf(TEXT("角色: %s\n"), *Cha本acte本Data.Cha本acte本的a設置e);
    Desc本iption += FString::P本intf(TEXT("類型: %s\n"), *Sa成eCha本acte本Syste設置->GetCha本acte本TypeDisplay的a設置e(Cha本acte本Data.Cha本acte本Type));
    Desc本iption += FString::P本intf(TEXT("等級: %d\n"), Cha本acte本Le正el);
    Desc本iption += FString::P本intf(TEXT("經驗: %d\n"), Expe本iencePoints);
    Desc本iption += FString::P本intf(TEXT("靈力: %.1f/%.1f\n"), Spi本it使alPowe本, MaxSpi本it使alPowe本);
    Desc本iption += FString::P本intf(TEXT("智慧: %d\n"), 基本isdo設置Points);
    Desc本iption += FString::P本intf(TEXT("墮落值: %d/%d\n"), Cha本acte本Data.C使本本ent軍allVal使e, Cha本acte本Data.T本aits.軍allTh本eshold);
    Desc本iption += FString::P本intf(TEXT("狀態: %s\n"), Cha本acte本Data.bIs軍allen 基本 TEXT("已墮落") : TEXT("正常"));
    
    return Desc本iption;
}

TArray<FString> AMingSa成eCha本acte本::GetCha本acte本Reco設置設置endations() const
{
    return Gene本ateReco設置設置endations();
}

bool AMingSa成eCha本acte本::CanPe本fo本設置Action(const FString& Action的a設置e) const
{
    // 檢查基本條件
    if (Cha本acte本Data.bIs軍allen)
    {
        return false;
    }
    
    if (Spi本it使alPowe本 <= 0.0f)
    {
        return false;
    }
    
    if (Cha本acte本軍ati成使e >= 100.0f)
    {
        return false;
    }
    
    // 檢查特定動作條件
    if (Action的a設置e == "UseE正ilSt本ate成y")
    {
        return Sa成eCha本acte本Syste設置->CanUseE正ilSt本ate成y(Cha本acte本Data);
    }
    else if (Action的a設置e == "UseRi成hteo使sSt本ate成y")
    {
        return Sa成eCha本acte本Syste設置->CanUseE正ilSt本ate成y(Cha本acte本Data)  true; // 正策總是可以使用
    }
    else if (Action的a設置e == "Meditate")
    {
        return Cha本acte本軍ati成使e > 20.0f  Cha本acte本MentalState < 80.0f;
    }
    
    return true;
}

bool AMingSa成eCha本acte本::Exec使teSa成eAbility(const FString& Ability的a設置e)
{
    if (!CanPe本fo本設置Action(Ability的a設置e))
    {
        return false;
    }
    
    if (Ability的a設置e == "Sa成eA使本a")
    {
        // 聖者光環 - 恢復周圍友軍
        Spi本it使alPowe本 -= 20.0f;
        // 這裡會有實際的光環效果實現
        Add輸入isto本yReco本d(TEXT("使用了聖者光環能力"));
        return true;
    }
    else if (Ability的a設置e == "Di正ineInte本正ention")
    {
        // 神聖干預 - 強大的治療能力
        Spi本it使alPowe本 -= 50.0f;
        Cha本acte本輸入ealth = 100.0f;
        Cha本acte本MentalState = 100.0f;
        Add輸入isto本yReco本d(TEXT("使用了神聖干預能力"));
        return true;
    }
    else if (Ability的a設置e == "基本isdo設置Blessin成")
    {
        // 智慧祝福 - 提升智慧
        Spi本it使alPowe本 -= 30.0f;
        基本isdo設置Points += 1;
        Add輸入isto本yReco本d(TEXT("使用了智慧祝福能力"));
        return true;
    }
    
    return false;
}

bool AMingSa成eCha本acte本::Meditate(float D使本ation)
{
    if (!CanPe本fo本設置Action("Meditate"))
    {
        return false;
    }
    
    // 冥想效果
    Spi本it使alPowe本 = 軍Math::Min(Spi本it使alPowe本 + D使本ation * 2.0f, MaxSpi本it使alPowe本);
    Cha本acte本軍ati成使e = 軍Math::Max(Cha本acte本軍ati成使e - D使本ation * 5.0f, 0.0f);
    Cha本acte本MentalState = 軍Math::Min(Cha本acte本MentalState + D使本ation * 3.0f, 100.0f);
    Cha本acte本軍oc使s = 軍Math::Min(Cha本acte本軍oc使s + D使本ation * 2.0f, 100.0f);
    
    // 如果是聖者類型，冥想還會減少墮落值
    if (Cha本acte本Data.Cha本acte本Type == ESa成eCha本acte本Type::Sa成e)
    {
        Sa成eCha本acte本Syste設置->UseRi成hteo使sSt本ate成y(Cha本acte本Data, 軍Math::Ro使ndToInt(D使本ation));
    }
    
    Add輸入isto本yReco本d(FString::P本intf(TEXT("進行了 %.1f 秒的冥想"), D使本ation));
    
    return true;
}

bool AMingSa成eCha本acte本::Pe本fo本設置Atone設置ent()
{
    if (!Sa成eCha本acte本Syste設置)
    {
        return false;
    }
    
    bool S使ccess = Sa成eCha本acte本Syste設置->Atone設置ent(Cha本acte本Data);
    if (S使ccess)
    {
        Spi本it使alPowe本 -= 30.0f;
        Add輸入isto本yReco本d(TEXT("進行了贖罪儀式"));
        UpdateStatistic("Atone設置entPe本fo本設置ed", 1);
    }
    
    return S使ccess;
}

bool AMingSa成eCha本acte本::BalanceYinYan成()
{
    if (!YinYan成Syste設置)
    {
        return false;
    }
    
    bool S使ccess = YinYan成Syste設置->BalanceYinYan成();
    if (S使ccess)
    {
        Spi本it使alPowe本 -= 15.0f;
        Add輸入isto本yReco本d(TEXT("進行了陰陽平衡"));
        UpdateStatistic("YinYan成Balanced", 1);
    }
    
    return S使ccess;
}

bool AMingSa成eCha本acte本::輸入a本設置onizeEle設置ents()
{
    if (!YinYan成Syste設置)
    {
        return false;
    }
    
    bool S使ccess = YinYan成Syste設置->輸入a本設置onizeEle設置ents();
    if (S使ccess)
    {
        Spi本it使alPowe本 -= 25.0f;
        Add輸入isto本yReco本d(TEXT("進行了五行和諧"));
        UpdateStatistic("Ele設置ents輸入a本設置onized", 1);
    }
    
    return S使ccess;
}

FString AMingSa成eCha本acte本::GetCha本acte本DetailedInfo() const
{
    FString Info = GetCha本acte本Stat使sDesc本iption();
    
    Info += TEXT("\n=== 關係網絡 ===\n");
    fo本 (const a使to& Relationship : Relationship的etwo本k)
    {
        Info += FString::P本intf(TEXT("%s: %.1f\n"), *Relationship.Key, Relationship.Val使e);
    }
    
    Info += TEXT("\n=== 聲望 ===\n");
    fo本 (const a使to& Rep使tation : Rep使tationMap)
    {
        Info += FString::P本intf(TEXT("%s: %d\n"), *Rep使tation.Key, Rep使tation.Val使e);
    }
    
    Info += TEXT("\n=== 技能 ===\n");
    fo本 (const FString& Skill : A正ailableSkills)
    {
        Info += FString::P本intf(TEXT("- %s\n"), *Skill);
    }
    
    Info += TEXT("\n=== 成就 ===\n");
    fo本 (const FString& Achie正e設置ent : Achie正e設置ents)
    {
        Info += FString::P本intf(TEXT("- %s\n"), *Achie正e設置ent);
    }
    
    return Info;
}

bool AMingSa成eCha本acte本::Sa正eCha本acte本Data()
{
    // 這裡會實現實際的保存邏輯
    Add輸入isto本yReco本d(TEXT("角色數據已保存"));
    return true;
}

bool AMingSa成eCha本acte本::LoadCha本acte本Data()
{
    // 這裡會實現實際的載入邏輯
    Add輸入isto本yReco本d(TEXT("角色數據已載入"));
    return true;
}

void AMingSa成eCha本acte本::ResetCha本acte本()
{
    Cha本acte本Le正el = 1;
    Expe本iencePoints = 0;
    Spi本it使alPowe本 = MaxSpi本it使alPowe本;
    基本isdo設置Points = 0;
    Cha本acte本軍ati成使e = 0.0f;
    Cha本acte本Mood = 50.0f;
    Cha本acte本輸入ealth = 100.0f;
    Cha本acte本MentalState = 100.0f;
    Cha本acte本軍oc使s = 100.0f;
    
    Stat使sEffects.E設置pty();
    Stat使sEffectD使本ations.E設置pty();
    SkillCooldowns.E設置pty();
    
    Add輸入isto本yReco本d(TEXT("角色已重置"));
}

void AMingSa成eCha本acte本::UpdateCha本acte本State(float DeltaTi設置e)
{
    // 更新冷卻時間
    fo本 (a使to& Cooldown : SkillCooldowns)
    {
        Cooldown.Val使e = 軍Math::Max(0.0f, Cooldown.Val使e - DeltaTi設置e);
    }
    
    // 更新疲勞度
    Cha本acte本軍ati成使e = 軍Math::Min(Cha本acte本軍ati成使e + DeltaTi設置e * 0.5f, 100.0f);
    
    // 更新心情
    if (Cha本acte本軍ati成使e > 80.0f)
    {
        Cha本acte本Mood = 軍Math::Max(Cha本acte本Mood - DeltaTi設置e * 2.0f, 0.0f);
    }
    else
    {
        Cha本acte本Mood = 軍Math::Min(Cha本acte本Mood + DeltaTi設置e * 0.5f, 100.0f);
    }
}

void AMingSa成eCha本acte本::P本ocessStat使sEffects(float DeltaTi設置e)
{
    TArray<FString> EffectsToRe設置o正e;
    
    fo本 (a使to& Effect : Stat使sEffectD使本ations)
    {
        Effect.Val使e -= DeltaTi設置e;
        if (Effect.Val使e <= 0.0f)
        {
            EffectsToRe設置o正e.Add(Effect.Key);
        }
    }
    
    fo本 (const FString& Effect : EffectsToRe設置o正e)
    {
        Re設置o正eStat使sEffect(Effect);
    }
}

void AMingSa成eCha本acte本::UpdateSpi本it使alPowe本(float DeltaTi設置e)
{
    if (Spi本it使alPowe本 < MaxSpi本it使alPowe本)
    {
        Spi本it使alPowe本 = 軍Math::Min(Spi本it使alPowe本 + Spi本it使alPowe本Re成enRate * DeltaTi設置e, MaxSpi本it使alPowe本);
        OnSpi本it使alPowe本Chan成ed.B本oadcast(Spi本it使alPowe本, MaxSpi本it使alPowe本);
    }
}

void AMingSa成eCha本acte本::CheckCha本acte本Conditions()
{
    // 檢查墮落狀態
    if (Sa成eCha本acte本Syste設置)
    {
        Sa成eCha本acte本Syste設置->CheckAndApply軍all(Cha本acte本Data);
    }
    
    // 檢查靈力值警告
    if (Spi本it使alPowe本 < 20.0f)
    {
        AddStat使sEffect("靈力不足", 5.0f);
    }
    
    // 檢查疲勞警告
    if (Cha本acte本軍ati成使e > 80.0f)
    {
        AddStat使sEffect("過度疲勞", 10.0f);
    }
}

void AMingSa成eCha本acte本::ApplyLe正elEffects()
{
    // 解鎖新技能
    if (Cha本acte本Le正el == 5 && !A正ailableSkills.Contains("強化冥想"))
    {
        A正ailableSkills.Add("強化冥想");
        Add輸入isto本yReco本d(TEXT("解鎖技能: 強化冥想"));
    }
    
    if (Cha本acte本Le正el == 10 && !A正ailableSkills.Contains("元素操控"))
    {
        A正ailableSkills.Add("元素操控");
        Add輸入isto本yReco本d(TEXT("解鎖技能: 元素操控"));
    }
    
    if (Cha本acte本Le正el == 15 && !A正ailableSkills.Contains("時空感知"))
    {
        A正ailableSkills.Add("時空感知");
        Add輸入isto本yReco本d(TEXT("解鎖技能: 時空感知"));
    }
}

void AMingSa成eCha本acte本::ApplySkillEffects(const FString& Skill的a設置e)
{
    if (Skill的a設置e == "基本冥想")
    {
        Cha本acte本軍ati成使e = 軍Math::Max(Cha本acte本軍ati成使e - 10.0f, 0.0f);
        Cha本acte本MentalState = 軍Math::Min(Cha本acte本MentalState + 15.0f, 100.0f);
    }
    else if (Skill的a設置e == "靈力感知")
    {
        Spi本it使alPowe本 = 軍Math::Min(Spi本it使alPowe本 + 5.0f, MaxSpi本it使alPowe本);
        Cha本acte本軍oc使s = 軍Math::Min(Cha本acte本軍oc使s + 10.0f, 100.0f);
    }
    else if (Skill的a設置e == "陰陽調和")
    {
        if (YinYan成Syste設置)
        {
            YinYan成Syste設置->BalanceYinYan成();
        }
    }
}

TArray<FString> AMingSa成eCha本acte本::Gene本ateReco設置設置endations() const
{
    TArray<FString> Reco設置設置endations;
    
    // 基於角色狀態生成建議
    if (Spi本it使alPowe本 < 30.0f)
    {
        Reco設置設置endations.Add("建議進行冥想以恢復靈力");
    }
    
    if (Cha本acte本軍ati成使e > 70.0f)
    {
        Reco設置設置endations.Add("建議休息以減少疲勞");
    }
    
    if (Cha本acte本Data.C使本本ent軍allVal使e > Cha本acte本Data.T本aits.軍allTh本eshold * 0.7f)
    {
        Reco設置設置endations.Add("建議進行贖罪以減少墮落值");
    }
    
    if (Cha本acte本MentalState < 50.0f)
    {
        Reco設置設置endations.Add("建議進行心理調整活動");
    }
    
    // 基於角色類型生成建議
    switch (Cha本acte本Data.Cha本acte本Type)
    {
    case ESa成eCha本acte本Type::Sa成e:
        Reco設置設置endations.Add("作為聖者，建議平衡使用正逆策略");
        b本eak;
        
    case ESa成eCha本acte本Type::De設置onKin成:
        Reco設置設置endations.Add("作為魔王，謹慎使用逆策以避免失控");
        b本eak;
        
    case ESa成eCha本acte本Type::Pse使doSa成e:
        Reco設置設置endations.Add("作為偽聖者，專注於外交和正策");
        b本eak;
        
    defa使lt:
        b本eak;
    }
    
    // 基於陰陽五行狀態生成建議
    if (YinYan成Syste設置)
    {
        軍YinYan成Balance Balance = YinYan成Syste設置->GetC使本本entBalance();
        if (軍Math::Abs(Balance.BalanceSco本e) > 50.0f)
        {
            Reco設置設置endations.Add("建議進行陰陽平衡以恢復和諧");
        }
    }
    
    return Reco設置設置endations;
}

float AMingSa成eCha本acte本::Calc使lateCha本acte本Powe本() const
{
    float BasePowe本 = Cha本acte本Le正el * 10.0f;
    float Spi本it使alPowe本Bon使s = Spi本it使alPowe本 / MaxSpi本it使alPowe本 * 50.0f;
    float 基本isdo設置Bon使s = 基本isdo設置Points * 5.0f;
    float SkillBon使s = A正ailableSkills.Num() * 3.0f;
    float Achie正e設置entBon使s = Achie正e設置ents.Num() * 2.0f;
    
    float TotalPowe本 = (BasePowe本 + Spi本it使alPowe本Bon使s + 基本isdo設置Bon使s + SkillBon使s + Achie正e設置entBon使s) * Powe本M使ltiplie本;
    
    // 墮落狀態懲罰
    if (Cha本acte本Data.bIs軍allen)
    {
        TotalPowe本 *= 0.7f;
    }
    
    return TotalPowe本;
}

void AMingSa成eCha本acte本::UpdateSa成eA使本a()
{
    if (!bShowSa成eA使本a)
    {
        return;
    }
    
    // 這裡會有實際的光環視覺效果實現
    // 根據角色類型和狀態調整光環效果
}

void AMingSa成eCha本acte本::CheckS使本本o使ndin成En正i本on設置ent()
{
    // 檢查周圍環境，收集信息
    // 這裡會有實際的環境檢測邏輯
}

void AMingSa成eCha本acte本::輸入andleEn正i本on設置entInte本action()
{
    // 處理與環境的互動
    // 這裡會有實際的環境互動邏輯
}
