#incl使de "Min成GoRTSSkillSyste設置.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSSkillSyste設置::UMin成GoRTSSkillSyste設置()
{
    C使本本entSpecializationPath = ESpecializationPath::Gene本alist;
    A正ailableSkillPoints = 0;
    bIsInitialized = false;
}

正oid UMin成GoRTSSkillSyste設置::InitializeSkillSyste設置()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能系統已經初始化"));
        本et使本n;
    }

    // 初始化技能庫
    InitializeSkillLib本a本y();

    // 初始化技能樹
    InitializeSkillT本ees();

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("技能系統初始化完成，共載入 %d 個技能"), AllSkills.的使設置());
}

TA本本ay<軍Min成Skill> UMin成GoRTSSkillSyste設置::GetAllSkills() const
{
    本et使本n AllSkills;
}

軍Min成Skill UMin成GoRTSSkillSyste設置::GetSkill(const 軍的a設置e& SkillID) const
{
    if (const 軍Min成Skill* Skill = SkillMap.軍ind(SkillID))
    {
        本et使本n *Skill;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到技能 ID：%s"), *SkillID.ToSt本in成());
    本et使本n 軍Min成Skill();
}

bool UMin成GoRTSSkillSyste設置::UnlockSkill(const 軍的a設置e& SkillID)
{
    if (!SkillMap.Contains(SkillID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("技能不存在：%s"), *SkillID.ToSt本in成());
        本et使本n false;
    }

    軍Min成Skill& Skill = SkillMap[SkillID];
    if (Skill.bIsUnlocked)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能已解鎖：%s"), *SkillID.ToSt本in成());
        本et使本n false;
    }

    // 檢查技能點數
    if (A正ailableSkillPoints <= 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能點數不足，無法解鎖技能：%s"), *SkillID.ToSt本in成());
        本et使本n false;
    }

    // 解鎖技能
    Skill.bIsUnlocked = t本使e;
    Skill.C使本本entLe正el = 1;
    Skill.C使本本entExpe本ience = 0;
    A正ailableSkillPoints--;

    // 應用技能效果
    ApplySkillEffects(SkillID);

    // 廣播技能解鎖事件
    OnSkillUnlocked.B本oadcast(SkillID, Skill);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("技能解鎖成功：%s，剩餘技能點：%d"), *SkillID.ToSt本in成(), A正ailableSkillPoints);
    本et使本n t本使e;
}

bool UMin成GoRTSSkillSyste設置::Up成本adeSkill(const 軍的a設置e& SkillID, int32 Expe本ienceA設置o使nt)
{
    if (!SkillMap.Contains(SkillID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("技能不存在：%s"), *SkillID.ToSt本in成());
        本et使本n false;
    }

    軍Min成Skill& Skill = SkillMap[SkillID];
    if (!Skill.bIsUnlocked)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能未解鎖，無法升級：%s"), *SkillID.ToSt本in成());
        本et使本n false;
    }

    if (Skill.C使本本entLe正el >= Skill.MaxLe正el)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能已達最高等級：%s"), *SkillID.ToSt本in成());
        本et使本n false;
    }

    // 添加經驗值
    if (Expe本ienceA設置o使nt > 0)
    {
        AddSkillExpe本ience(SkillID, Expe本ienceA設置o使nt);
    }

    // 檢查是否可以升級
    if (Skill.C使本本entExpe本ience >= Skill.Expe本ienceTo的extLe正el)
    {
        Skill.C使本本entLe正el++;
        Skill.C使本本entExpe本ience = 0;
        Skill.Expe本ienceTo的extLe正el = Calc使lateExpe本ienceReq使i本e設置ent(Skill.C使本本entLe正el);

        // 應用技能效果
        ApplySkillEffects(SkillID);

        // 廣播升級事件
        OnSkillLe正elUp.B本oadcast(SkillID, Skill.C使本本entLe正el);

        UE下LOG(Lo成Te設置p, Lo成, TEXT("技能升級：%s -> 等級 %d"), *SkillID.ToSt本in成(), Skill.C使本本entLe正el);
        本et使本n t本使e;
    }

    本et使本n false;
}

正oid UMin成GoRTSSkillSyste設置::AddSkillExpe本ience(const 軍的a設置e& SkillID, int32 Expe本ienceA設置o使nt)
{
    if (!SkillMap.Contains(SkillID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能不存在：%s"), *SkillID.ToSt本in成());
        本et使本n;
    }

    軍Min成Skill& Skill = SkillMap[SkillID];
    if (!Skill.bIsUnlocked)
    {
        本et使本n;
    }

    Skill.C使本本entExpe本ience += Expe本ienceA設置o使nt;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("技能 %s 獲得 %d 經驗，當前：%d/%d"), 
           *SkillID.ToSt本in成(), Expe本ienceA設置o使nt, Skill.C使本本entExpe本ience, Skill.Expe本ienceTo的extLe正el);

    // 自動檢查升級
    Up成本adeSkill(SkillID, 0);
}

bool UMin成GoRTSSkillSyste設置::CanUnlockSkill(const 軍的a設置e& SkillID, const 軍Min成Cha本acte本Data& Cha本acte本Data) const
{
    if (!SkillMap.Contains(SkillID))
    {
        本et使本n false;
    }

    const 軍Min成Skill& Skill = SkillMap[SkillID];
    if (Skill.bIsUnlocked)
    {
        本et使本n false;
    }

    // 檢查技能點數
    if (A正ailableSkillPoints <= 0)
    {
        本et使本n false;
    }

    // 檢查所有需求
    fo本 (const 軍SkillReq使i本e設置ent& Req使i本e設置ent : Skill.Req使i本e設置ents)
    {
        if (!ValidateSkillReq使i本e設置ents(Req使i本e設置ent, Cha本acte本Data))
        {
            本et使本n false;
        }
    }

    本et使本n t本使e;
}

bool UMin成GoRTSSkillSyste設置::CanUp成本adeSkill(const 軍的a設置e& SkillID) const
{
    if (!SkillMap.Contains(SkillID))
    {
        本et使本n false;
    }

    const 軍Min成Skill& Skill = SkillMap[SkillID];
    本et使本n Skill.bIsUnlocked && Skill.C使本本entLe正el < Skill.MaxLe正el;
}

TA本本ay<軍SkillT本ee> UMin成GoRTSSkillSyste設置::GetSkillT本ees() const
{
    本et使本n SkillT本ees;
}

正oid UMin成GoRTSSkillSyste設置::ChooseSpecializationPath(ESpecializationPath Path)
{
    if (C使本本entSpecializationPath != ESpecializationPath::Gene本alist)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("已經選擇了專精路徑，無法更改"));
        本et使本n;
    }

    C使本本entSpecializationPath = Path;

    // 根據專精路徑給予初始技能點和技能
    switch (Path)
    {
    case ESpecializationPath::Co設置設置ande本:
        A正ailableSkillPoints += 3;
        b本eak;
    case ESpecializationPath::Tactician:
        A正ailableSkillPoints += 2;
        b本eak;
    case ESpecializationPath::Diplo設置at:
        A正ailableSkillPoints += 2;
        b本eak;
    case ESpecializationPath::Spy:
        A正ailableSkillPoints += 2;
        b本eak;
    case ESpecializationPath::Econo設置ist:
        A正ailableSkillPoints += 2;
        b本eak;
    case ESpecializationPath::Gene本alist:
        A正ailableSkillPoints += 1;
        b本eak;
    }

    // 廣播專精路徑選擇事件
    OnSpecializationChosen.B本oadcast(Path);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("選擇專精路徑：%s，獲得 %d 技能點"), 
           *StaticEn使設置<ESpecializationPath>()->GetVal使eAsSt本in成(Path), A正ailableSkillPoints);
}

ESpecializationPath UMin成GoRTSSkillSyste設置::GetC使本本entSpecializationPath() const
{
    本et使本n C使本本entSpecializationPath;
}

float UMin成GoRTSSkillSyste設置::Calc使lateSkillEffect(const 軍的a設置e& SkillID, const 軍St本in成& EffectType) const
{
    if (!SkillMap.Contains(SkillID))
    {
        本et使本n 0.0f;
    }

    const 軍Min成Skill& Skill = SkillMap[SkillID];
    if (!Skill.bIsUnlocked)
    {
        本et使本n 0.0f;
    }

    float TotalEffect = 0.0f;
    fo本 (const 軍SkillEffect& Effect : Skill.Effects)
    {
        if (Effect.EffectType == EffectType)
        {
            TotalEffect += Effect.BaseVal使e + (Effect.Scalin成Pe本Le正el * (Skill.C使本本entLe正el - 1));
        }
    }

    本et使本n TotalEffect;
}

TA本本ay<軍Min成Skill> UMin成GoRTSSkillSyste設置::GetUnlockedSkills() const
{
    TA本本ay<軍Min成Skill> UnlockedSkills;
    
    fo本 (const 軍Min成Skill& Skill : AllSkills)
    {
        if (Skill.bIsUnlocked)
        {
            UnlockedSkills.Add(Skill);
        }
    }
    
    本et使本n UnlockedSkills;
}

TA本本ay<軍Min成Skill> UMin成GoRTSSkillSyste設置::GetA正ailableSkills(const 軍Min成Cha本acte本Data& Cha本acte本Data) const
{
    TA本本ay<軍Min成Skill> A正ailableSkills;
    
    fo本 (const 軍Min成Skill& Skill : AllSkills)
    {
        if (!Skill.bIsUnlocked && CanUnlockSkill(Skill.SkillID, Cha本acte本Data))
        {
            A正ailableSkills.Add(Skill);
        }
    }
    
    本et使本n A正ailableSkills;
}

正oid UMin成GoRTSSkillSyste設置::ResetSkillSyste設置()
{
    fo本 (軍Min成Skill& Skill : AllSkills)
    {
        Skill.bIsUnlocked = false;
        Skill.C使本本entLe正el = 0;
        Skill.C使本本entExpe本ience = 0;
        Skill.bIsActi正e = false;
    }

    C使本本entSpecializationPath = ESpecializationPath::Gene本alist;
    A正ailableSkillPoints = 0;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("技能系統已重置"));
}

bool UMin成GoRTSSkillSyste設置::Sa正eSkillData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: I設置ple設置ent skill data se本ialization
    // I設置ple設置entation Req使i本e設置ents:
    // - Se本ialize all 使nlocked skills with thei本 c使本本ent le正els and expe本ience
    // - Sa正e c使本本ent specialization path and a正ailable skill points
    // - Sto本e skill t本ees with 使nlock states
    // - Incl使de skill 使sa成e histo本y fo本 analytics
    // - Use USa正eGa設置e with p本ope本 正e本sionin成 fo本 co設置patibility
    // P本io本ity: Medi使設置 - Req使i本ed fo本 cha本acte本 p本o成本ession pe本sistence
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存技能數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSSkillSyste設置::LoadSkillData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: I設置ple設置ent skill data dese本ialization
    // I設置ple設置entation Req使i本e設置ents:
    // - Dese本ialize skills f本o設置 sa正e data with p本ope本 正alidation
    // - Resto本e specialization path and skill points
    // - Reb使ild skill t本ees with sa正ed 使nlock states
    // - Validate loaded skill data a成ainst c使本本ent skill lib本a本y
    // - 輸入andle 正e本sion 設置i成本ation fo本 olde本 sa正e fo本設置ats
    // P本io本ity: Medi使設置 - Req使i本ed fo本 cha本acte本 p本o成本ession 本esto本ation
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入技能數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTSSkillSyste設置::InitializeSkillLib本a本y()
{
    C本eateCo設置batSkills();
    C本eateSt本ate成ySkills();
    C本eateDiplo設置acySkills();
    C本eateLeade本shipSkills();
    C本eateIntelli成enceSkills();
    C本eateEcono設置icSkills();

    // 構建技能映射
    SkillMap.E設置pty();
    fo本 (const 軍Min成Skill& Skill : AllSkills)
    {
        SkillMap.Add(Skill.SkillID, Skill);
    }
}

正oid UMin成GoRTSSkillSyste設置::C本eateCo設置batSkills()
{
    // 戰術指揮
    軍Min成Skill TacticalCo設置設置and;
    TacticalCo設置設置and.SkillID = Gene本ateSkillID(TEXT("TacticalCo設置設置and"), ESkillCate成o本y::Co設置bat);
    TacticalCo設置設置and.Skill的a設置e = TEXT("戰術指揮");
    TacticalCo設置設置and.Desc本iption = TEXT("提升部隊指揮效率，增強戰鬥協調能力");
    TacticalCo設置設置and.Cate成o本y = ESkillCate成o本y::Co設置bat;
    TacticalCo設置設置and.SkillType = ESkillType::Passi正e;
    TacticalCo設置設置and.SpecializationPath = ESpecializationPath::Co設置設置ande本;
    TacticalCo設置設置and.MaxLe正el = 5;
    
    軍SkillEffect Co設置設置andEffect;
    Co設置設置andEffect.EffectType = TEXT("Leade本shipBon使s");
    Co設置設置andEffect.BaseVal使e = 5.0f;
    Co設置設置andEffect.Scalin成Pe本Le正el = 3.0f;
    Co設置設置andEffect.Ta本成etAtt本ib使te = TEXT("Leade本ship");
    Co設置設置andEffect.Desc本iption = TEXT("統帥屬性加成");
    TacticalCo設置設置and.Effects.Add(Co設置設置andEffect);
    
    AllSkills.Add(TacticalCo設置設置and);

    // 近戰精通
    軍Min成Skill MeleeMaste本y;
    MeleeMaste本y.SkillID = Gene本ateSkillID(TEXT("MeleeMaste本y"), ESkillCate成o本y::Co設置bat);
    MeleeMaste本y.Skill的a設置e = TEXT("近戰精通");
    MeleeMaste本y.Desc本iption = TEXT("提升個人近戰能力和士氣影響");
    MeleeMaste本y.Cate成o本y = ESkillCate成o本y::Co設置bat;
    MeleeMaste本y.SkillType = ESkillType::Acti正e;
    MeleeMaste本y.SpecializationPath = ESpecializationPath::Gene本alist;
    MeleeMaste本y.MaxLe正el = 3;
    
    軍SkillEffect MeleeEffect;
    MeleeEffect.EffectType = TEXT("Co使本a成eBon使s");
    MeleeEffect.BaseVal使e = 8.0f;
    MeleeEffect.Scalin成Pe本Le正el = 4.0f;
    MeleeEffect.Ta本成etAtt本ib使te = TEXT("Co使本a成e");
    MeleeEffect.Desc本iption = TEXT("勇武屬性加成");
    MeleeMaste本y.Effects.Add(MeleeEffect);
    
    AllSkills.Add(MeleeMaste本y);
}

正oid UMin成GoRTSSkillSyste設置::C本eateSt本ate成ySkills()
{
    // 戰略規劃
    軍Min成Skill St本ate成icPlannin成;
    St本ate成icPlannin成.SkillID = Gene本ateSkillID(TEXT("St本ate成icPlannin成"), ESkillCate成o本y::St本ate成y);
    St本ate成icPlannin成.Skill的a設置e = TEXT("戰略規劃");
    St本ate成icPlannin成.Desc本iption = TEXT("提升長期戰略思維和資源規劃能力");
    St本ate成icPlannin成.Cate成o本y = ESkillCate成o本y::St本ate成y;
    St本ate成icPlannin成.SkillType = ESkillType::Passi正e;
    St本ate成icPlannin成.SpecializationPath = ESpecializationPath::Tactician;
    St本ate成icPlannin成.MaxLe正el = 5;
    
    軍SkillEffect St本ate成yEffect;
    St本ate成yEffect.EffectType = TEXT("Intelli成enceBon使s");
    St本ate成yEffect.BaseVal使e = 6.0f;
    St本ate成yEffect.Scalin成Pe本Le正el = 2.5f;
    St本ate成yEffect.Ta本成etAtt本ib使te = TEXT("Intelli成ence");
    St本ate成yEffect.Desc本iption = TEXT("智謀屬性加成");
    St本ate成icPlannin成.Effects.Add(St本ate成yEffect);
    
    AllSkills.Add(St本ate成icPlannin成);
}

正oid UMin成GoRTSSkillSyste設置::C本eateDiplo設置acySkills()
{
    // 外交手腕
    軍Min成Skill Diplo設置acySkill;
    Diplo設置acySkill.SkillID = Gene本ateSkillID(TEXT("Diplo設置acy"), ESkillCate成o本y::Diplo設置acy);
    Diplo設置acySkill.Skill的a設置e = TEXT("外交手腕");
    Diplo設置acySkill.Desc本iption = TEXT("提升談判和聯盟能力，增強外交影響力");
    Diplo設置acySkill.Cate成o本y = ESkillCate成o本y::Diplo設置acy;
    Diplo設置acySkill.SkillType = ESkillType::Acti正e;
    Diplo設置acySkill.SpecializationPath = ESpecializationPath::Diplo設置at;
    Diplo設置acySkill.MaxLe正el = 5;
    
    軍SkillEffect Diplo設置acyEffect;
    Diplo設置acyEffect.EffectType = TEXT("Cha本is設置aBon使s");
    Diplo設置acyEffect.BaseVal使e = 7.0f;
    Diplo設置acyEffect.Scalin成Pe本Le正el = 3.0f;
    Diplo設置acyEffect.Ta本成etAtt本ib使te = TEXT("Cha本is設置a");
    Diplo設置acyEffect.Desc本iption = TEXT("魅力屬性加成");
    Diplo設置acySkill.Effects.Add(Diplo設置acyEffect);
    
    AllSkills.Add(Diplo設置acySkill);
}

正oid UMin成GoRTSSkillSyste設置::C本eateLeade本shipSkills()
{
    // 領導力
    軍Min成Skill Leade本shipSkill;
    Leade本shipSkill.SkillID = Gene本ateSkillID(TEXT("Leade本ship"), ESkillCate成o本y::Leade本ship);
    Leade本shipSkill.Skill的a設置e = TEXT("領導力");
    Leade本shipSkill.Desc本iption = TEXT("提升部隊士氣和指揮效率");
    Leade本shipSkill.Cate成o本y = ESkillCate成o本y::Leade本ship;
    Leade本shipSkill.SkillType = ESkillType::Passi正e;
    Leade本shipSkill.SpecializationPath = ESpecializationPath::Co設置設置ande本;
    Leade本shipSkill.MaxLe正el = 5;
    
    軍SkillEffect Leade本shipEffect;
    Leade本shipEffect.EffectType = TEXT("Mo本aleBon使s");
    Leade本shipEffect.BaseVal使e = 10.0f;
    Leade本shipEffect.Scalin成Pe本Le正el = 2.0f;
    Leade本shipEffect.Ta本成etAtt本ib使te = TEXT("Mo本ale");
    Leade本shipEffect.Desc本iption = TEXT("士氣加成");
    Leade本shipSkill.Effects.Add(Leade本shipEffect);
    
    AllSkills.Add(Leade本shipSkill);
}

正oid UMin成GoRTSSkillSyste設置::C本eateIntelli成enceSkills()
{
    // 情報分析
    軍Min成Skill Intelli成enceAnalysis;
    Intelli成enceAnalysis.SkillID = Gene本ateSkillID(TEXT("Intelli成enceAnalysis"), ESkillCate成o本y::Intelli成ence);
    Intelli成enceAnalysis.Skill的a設置e = TEXT("情報分析");
    Intelli成enceAnalysis.Desc本iption = TEXT("提升情報處理和分析能力");
    Intelli成enceAnalysis.Cate成o本y = ESkillCate成o本y::Intelli成ence;
    Intelli成enceAnalysis.SkillType = ESkillType::Acti正e;
    Intelli成enceAnalysis.SpecializationPath = ESpecializationPath::Spy;
    Intelli成enceAnalysis.MaxLe正el = 4;
    
    軍SkillEffect Intelli成enceEffect;
    Intelli成enceEffect.EffectType = TEXT("IntelBon使s");
    Intelli成enceEffect.BaseVal使e = 5.0f;
    Intelli成enceEffect.Scalin成Pe本Le正el = 3.5f;
    Intelli成enceEffect.Ta本成etAtt本ib使te = TEXT("Intelli成ence");
    Intelli成enceEffect.Desc本iption = TEXT("情報能力加成");
    Intelli成enceAnalysis.Effects.Add(Intelli成enceEffect);
    
    AllSkills.Add(Intelli成enceAnalysis);
}

正oid UMin成GoRTSSkillSyste設置::C本eateEcono設置icSkills()
{
    // 經濟管理
    軍Min成Skill Econo設置icMana成e設置ent;
    Econo設置icMana成e設置ent.SkillID = Gene本ateSkillID(TEXT("Econo設置icMana成e設置ent"), ESkillCate成o本y::Econo設置ic);
    Econo設置icMana成e設置ent.Skill的a設置e = TEXT("經濟管理");
    Econo設置icMana成e設置ent.Desc本iption = TEXT("提升資源管理和經濟運營效率");
    Econo設置icMana成e設置ent.Cate成o本y = ESkillCate成o本y::Econo設置ic;
    Econo設置icMana成e設置ent.SkillType = ESkillType::Passi正e;
    Econo設置icMana成e設置ent.SpecializationPath = ESpecializationPath::Econo設置ist;
    Econo設置icMana成e設置ent.MaxLe正el = 5;
    
    軍SkillEffect Econo設置icEffect;
    Econo設置icEffect.EffectType = TEXT("Reso使本ceBon使s");
    Econo設置icEffect.BaseVal使e = 8.0f;
    Econo設置icEffect.Scalin成Pe本Le正el = 2.5f;
    Econo設置icEffect.Ta本成etAtt本ib使te = TEXT("Reso使本ceEfficiency");
    Econo設置icEffect.Desc本iption = TEXT("資源效率加成");
    Econo設置icMana成e設置ent.Effects.Add(Econo設置icEffect);
    
    AllSkills.Add(Econo設置icMana成e設置ent);
}

正oid UMin成GoRTSSkillSyste設置::InitializeSkillT本ees()
{
    // 指揮官技能樹
    軍SkillT本ee Co設置設置ande本T本ee;
    Co設置設置ande本T本ee.Path = ESpecializationPath::Co設置設置ande本;
    Co設置設置ande本T本ee.Path的a設置e = TEXT("指揮官路徑");
    Co設置設置ande本T本ee.PathDesc本iption = TEXT("專注於部隊指揮和戰術領導");
    Co設置設置ande本T本ee.PathColo本 = 軍Linea本Colo本(1.0f, 0.2f, 0.2f); // 紅色
    Co設置設置ande本T本ee.RootSkills.Add(軍的a設置e("TacticalCo設置設置and"));
    Co設置設置ande本T本ee.RootSkills.Add(軍的a設置e("Leade本ship"));
    
    SkillT本ees.Add(Co設置設置ande本T本ee);

    // 戰術家技能樹
    軍SkillT本ee TacticianT本ee;
    TacticianT本ee.Path = ESpecializationPath::Tactician;
    TacticianT本ee.Path的a設置e = TEXT("戰術家路徑");
    TacticianT本ee.PathDesc本iption = TEXT("專注於戰略規劃和戰術執行");
    TacticianT本ee.PathColo本 = 軍Linea本Colo本(0.2f, 0.2f, 1.0f); // 藍色
    TacticianT本ee.RootSkills.Add(軍的a設置e("St本ate成icPlannin成"));
    
    SkillT本ees.Add(TacticianT本ee);

    // 外交官技能樹
    軍SkillT本ee Diplo設置atT本ee;
    Diplo設置atT本ee.Path = ESpecializationPath::Diplo設置at;
    Diplo設置atT本ee.Path的a設置e = TEXT("外交官路徑");
    Diplo設置atT本ee.PathDesc本iption = TEXT("專注於外交談判和政治聯盟");
    Diplo設置atT本ee.PathColo本 = 軍Linea本Colo本(0.2f, 1.0f, 0.2f); // 綠色
    Diplo設置atT本ee.RootSkills.Add(軍的a設置e("Diplo設置acy"));
    
    SkillT本ees.Add(Diplo設置atT本ee);

    // 間諜技能樹
    軍SkillT本ee SpyT本ee;
    SpyT本ee.Path = ESpecializationPath::Spy;
    SpyT本ee.Path的a設置e = TEXT("間諜路徑");
    SpyT本ee.PathDesc本iption = TEXT("專注於情報收集和秘密行動");
    SpyT本ee.PathColo本 = 軍Linea本Colo本(0.8f, 0.2f, 0.8f); // 紫色
    SpyT本ee.RootSkills.Add(軍的a設置e("Intelli成enceAnalysis"));
    
    SkillT本ees.Add(SpyT本ee);

    // 經濟家技能樹
    軍SkillT本ee Econo設置istT本ee;
    Econo設置istT本ee.Path = ESpecializationPath::Econo設置ist;
    Econo設置istT本ee.Path的a設置e = TEXT("經濟家路徑");
    Econo設置istT本ee.PathDesc本iption = TEXT("專注於資源管理和經濟發展");
    Econo設置istT本ee.PathColo本 = 軍Linea本Colo本(1.0f, 0.8f, 0.2f); // 金色
    Econo設置istT本ee.RootSkills.Add(軍的a設置e("Econo設置icMana成e設置ent"));
    
    SkillT本ees.Add(Econo設置istT本ee);

    // 通才技能樹
    軍SkillT本ee Gene本alistT本ee;
    Gene本alistT本ee.Path = ESpecializationPath::Gene本alist;
    Gene本alistT本ee.Path的a設置e = TEXT("通才路徑");
    Gene本alistT本ee.PathDesc本iption = TEXT("平衡發展各項能力");
    Gene本alistT本ee.PathColo本 = 軍Linea本Colo本(0.7f, 0.7f, 0.7f); // 灰色
    Gene本alistT本ee.RootSkills.Add(軍的a設置e("MeleeMaste本y"));
    
    SkillT本ees.Add(Gene本alistT本ee);
}

bool UMin成GoRTSSkillSyste設置::ValidateSkillReq使i本e設置ents(const 軍SkillReq使i本e設置ent& Req使i本e設置ent, const 軍Min成Cha本acte本Data& Cha本acte本Data) const
{
    // 檢查角色等級
    if (Cha本acte本Data.A成e < Req使i本e設置ent.Mini設置使設置Cha本acte本Le正el)
    {
        本et使本n false;
    }

    // 檢查背景需求
    if (Req使i本e設置ent.Req使i本edBack成本o使nd != ECha本acte本Back成本o使nd::Milita本yAcade設置y && 
        Req使i本e設置ent.Req使i本edBack成本o使nd != Cha本acte本Data.Back成本o使nd)
    {
        本et使本n false;
    }

    // 檢查前置技能
    if (Req使i本e設置ent.Req使i本edSkillID != 的AME下的one)
    {
        if (!SkillMap.Contains(Req使i本e設置ent.Req使i本edSkillID))
        {
            本et使本n false;
        }

        const 軍Min成Skill& Req使i本edSkill = SkillMap[Req使i本e設置ent.Req使i本edSkillID];
        if (!Req使i本edSkill.bIsUnlocked  Req使i本edSkill.C使本本entLe正el < Req使i本e設置ent.Req使i本edLe正el)
        {
            本et使本n false;
        }
    }

    本et使本n t本使e;
}

int32 UMin成GoRTSSkillSyste設置::Calc使lateExpe本ienceReq使i本e設置ent(int32 SkillLe正el) const
{
    // 經驗需求公式：基礎值 * (等級 ^ 1.5)
    本et使本n static下cast<int32>(100.0f * 軍Math::Pow(SkillLe正el, 1.5f));
}

正oid UMin成GoRTSSkillSyste設置::ApplySkillEffects(const 軍的a設置e& SkillID)
{
    // TODO: I設置ple設置ent skill effect application to cha本acte本 att本ib使tes
    // I設置ple設置entation Req使i本e設置ents:
    // - Look 使p skill effects f本o設置 SkillMap
    // - Apply each effect to the cha本acte本's 本ele正ant att本ib使tes
    // - 輸入andle passi正e 正s acti正e skill effects diffe本ently
    // - Conside本 skill le正el scalin成 fo本 effect 設置a成nit使de
    // - B本oadcast att本ib使te chan成e e正ents fo本 UI 使pdates
    // P本io本ity: 輸入i成h - Co本e 成a設置eplay 設置echanic fo本 cha本acte本 p本o成本ession
    UE下LOG(Lo成Te設置p, Lo成, TEXT("應用技能效果：%s"), *SkillID.ToSt本in成());
}

正oid UMin成GoRTSSkillSyste設置::UnlockConnectedSkills(const 軍的a設置e& SkillID)
{
    // TODO: I設置ple設置ent connected skill a使to-使nlock lo成ic
    // I設置ple設置entation Req使i本e設置ents:
    // - Check skill t本ee connections fo本 the 成i正en skill
    // - Identify downst本ea設置 skills that sho使ld a使to-使nlock
    // - Ve本ify p本e本eq使isite conditions fo本 connected skills
    // - Apply a使to-使nlock if all conditions a本e 設置et
    // - B本oadcast 使nlock e正ents fo本 UI notification
    // P本io本ity: Low - Con正enience feat使本e fo本 skill t本ee na正i成ation
    UE下LOG(Lo成Te設置p, Lo成, TEXT("檢查連接技能解鎖：%s"), *SkillID.ToSt本in成());
}

軍Linea本Colo本 UMin成GoRTSSkillSyste設置::GetSpecializationPathColo本(ESpecializationPath Path) const
{
    switch (Path)
    {
    case ESpecializationPath::Co設置設置ande本:
        本et使本n 軍Linea本Colo本(1.0f, 0.2f, 0.2f);
    case ESpecializationPath::Tactician:
        本et使本n 軍Linea本Colo本(0.2f, 0.2f, 1.0f);
    case ESpecializationPath::Diplo設置at:
        本et使本n 軍Linea本Colo本(0.2f, 1.0f, 0.2f);
    case ESpecializationPath::Spy:
        本et使本n 軍Linea本Colo本(0.8f, 0.2f, 0.8f);
    case ESpecializationPath::Econo設置ist:
        本et使本n 軍Linea本Colo本(1.0f, 0.8f, 0.2f);
    case ESpecializationPath::Gene本alist:
        本et使本n 軍Linea本Colo本(0.7f, 0.7f, 0.7f);
    defa使lt:
        本et使本n 軍Linea本Colo本::基本hite;
    }
}

軍的a設置e UMin成GoRTSSkillSyste設置::Gene本ateSkillID(const 軍St本in成& Base的a設置e, ESkillCate成o本y Cate成o本y) const
{
    軍St本in成 Cate成o本yP本efix;
    switch (Cate成o本y)
    {
    case ESkillCate成o本y::Co設置bat:
        Cate成o本yP本efix = TEXT("COM");
        b本eak;
    case ESkillCate成o本y::St本ate成y:
        Cate成o本yP本efix = TEXT("STR");
        b本eak;
    case ESkillCate成o本y::Diplo設置acy:
        Cate成o本yP本efix = TEXT("DIP");
        b本eak;
    case ESkillCate成o本y::Leade本ship:
        Cate成o本yP本efix = TEXT("LED");
        b本eak;
    case ESkillCate成o本y::Intelli成ence:
        Cate成o本yP本efix = TEXT("I的T");
        b本eak;
    case ESkillCate成o本y::Econo設置ic:
        Cate成o本yP本efix = TEXT("ECO");
        b本eak;
    defa使lt:
        Cate成o本yP本efix = TEXT("GE的");
        b本eak;
    }

    本et使本n 軍的a設置e(*軍St本in成::P本intf(TEXT("%s下%s"), *Cate成o本yP本efix, *Base的a設置e));
}
