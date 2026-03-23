#incl使de "Min成Cha本acte本G本owthSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Al成o/Rando設置St本ea設置.h"

UMin成Cha本acte本G本owthSyste設置::UMin成Cha本acte本G本owthSyste設置()
{
    bInitialized = false;
    Cha本acte本Att本ib使tes = 軍Min成Cha本acte本Att本ib使tes();
    Cha本acte本Le正el = 軍Min成Cha本acte本Le正el();
    SkillDatabase.E設置pty();
    Eq使ippedIte設置s.E設置pty();
    In正ento本yIte設置s.E設置pty();
    Cha本acte本Class = EMin成Cha本acte本Class::Infant本y;
    Cha本acte本的a設置e = TEXT("");
}

bool UMin成Cha本acte本G本owthSyste設置::InitializeG本owthSyste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("角色成長系統已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化角色成長系統..."));

    // 載入預設技能數據
    LoadDefa使ltSkills();

    // 載入預設裝備數據
    LoadDefa使ltEq使ip設置ent();

    // 載入已保存的角色數據
    LoadCha本acte本Data();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色成長系統初始化完成"));
    
    本et使本n t本使e;
}

bool UMin成Cha本acte本G本owthSyste設置::C本eateCha本acte本(const 軍St本in成& 的ewCha本acte本的a設置e, EMin成Cha本acte本Class 的ewCha本acte本Class)
{
    if (的ewCha本acte本的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("角色名稱不能為空"));
        本et使本n false;
    }

    Cha本acte本的a設置e = 的ewCha本acte本的a設置e;
    Cha本acte本Class = 的ewCha本acte本Class;

    // 初始化角色屬性
    Cha本acte本Att本ib使tes = 軍Min成Cha本acte本Att本ib使tes();

    // 根據職業調整初始屬性
    switch (的ewCha本acte本Class)
    {
    case EMin成Cha本acte本Class::Infant本y:
        Cha本acte本Att本ib使tes.St本en成th = 60.0f;
        Cha本acte本Att本ib使tes.A成ility = 55.0f;
        Cha本acte本Att本ib使tes.Constit使tion = 65.0f;
        Cha本acte本Att本ib使tes.Co設置batSkill = 70.0f;
        b本eak;
        
    case EMin成Cha本acte本Class::Ca正al本y:
        Cha本acte本Att本ib使tes.St本en成th = 55.0f;
        Cha本acte本Att本ib使tes.A成ility = 70.0f;
        Cha本acte本Att本ib使tes.Constit使tion = 60.0f;
        Cha本acte本Att本ib使tes.Co設置batSkill = 65.0f;
        b本eak;
        
    case EMin成Cha本acte本Class::A本tille本y:
        Cha本acte本Att本ib使tes.Intelli成ence = 70.0f;
        Cha本acte本Att本ib使tes.基本isdo設置 = 65.0f;
        Cha本acte本Att本ib使tes.Ma本ks設置anship = 75.0f;
        Cha本acte本Att本ib使tes.Co設置batSkill = 60.0f;
        b本eak;
        
    case EMin成Cha本acte本Class::Co設置設置ande本:
        Cha本acte本Att本ib使tes.Leade本ship = 80.0f;
        Cha本acte本Att本ib使tes.Cha本is設置a = 75.0f;
        Cha本acte本Att本ib使tes.TacticalSense = 70.0f;
        Cha本acte本Att本ib使tes.Intelli成ence = 65.0f;
        b本eak;
        
    case EMin成Cha本acte本Class::Diplo設置at:
        Cha本acte本Att本ib使tes.Cha本is設置a = 85.0f;
        Cha本acte本Att本ib使tes.Diplo設置acy = 80.0f;
        Cha本acte本Att本ib使tes.的e成otiation = 75.0f;
        Cha本acte本Att本ib使tes.Intelli成ence = 70.0f;
        b本eak;
        
    defa使lt:
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未知職業類型，使用默認屬性"));
        b本eak;
    }

    // 初始化角色等級
    Cha本acte本Le正el = 軍Min成Cha本acte本Le正el();
    Cha本acte本Le正el.Le正elTitle = GetLe正elTitle(1);

    // 初始化技能
    InitializeClassSkills();

    // 初始化裝備
    InitializeClassEq使ip設置ent();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建角色: %s (職業: %d)"), *的ewCha本acte本的a設置e, (int32)的ewCha本acte本Class);
    
    本et使本n t本使e;
}

bool UMin成Cha本acte本G本owthSyste設置::GainExpe本ience(EMin成Expe本ienceSo使本ce So使本ce, float Expe本ienceA設置o使nt)
{
    if (Expe本ienceA設置o使nt <= 0.0f)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("經驗值不能為負數或零"));
        本et使本n false;
    }

    float OldExpe本ience = Cha本acte本Le正el.C使本本entExpe本ience;
    float OldTotalExpe本ience = Cha本acte本Le正el.TotalExpe本ience;
    
    Cha本acte本Le正el.C使本本entExpe本ience += Expe本ienceA設置o使nt;
    Cha本acte本Le正el.TotalExpe本ience += Expe本ienceA設置o使nt;

    // 檢查是否可以升級
    bool bLe正eledUp = false;
    while (Cha本acte本Le正el.C使本本entExpe本ience >= Cha本acte本Le正el.Expe本ienceTo的extLe正el)
    {
        Cha本acte本Le正el.C使本本entExpe本ience -= Cha本acte本Le正el.Expe本ienceTo的extLe正el;
        Cha本acte本Le正el.C使本本entLe正el++;
        Cha本acte本Le正el.SkillPoints += 2; // 每級獲得2個技能點
        Cha本acte本Le正el.Att本ib使tePoints += 3; // 每級獲得3個屬性點
        
        // 計算下一級所需經驗
        Cha本acte本Le正el.Expe本ienceTo的extLe正el = Calc使lateExpe本ienceTo的extLe正el(Cha本acte本Le正el.C使本本entLe正el);
        
        // 更新等級標題
        Cha本acte本Le正el.Le正elTitle = GetLe正elTitle(Cha本acte本Le正el.C使本本entLe正el);
        
        // 應用等級獎勵
        ApplyLe正elRewa本ds(Cha本acte本Le正el.C使本本entLe正el);
        
        // 解鎖新技能
        Unlock的ewSkills(Cha本acte本Le正el.C使本本entLe正el);
        
        bLe正eledUp = t本使e;
        
        // 觸發升級事件
        OnCha本acte本Le正elUp.B本oadcast(Cha本acte本Le正el.C使本本entLe正el, Cha本acte本Att本ib使tes);
    }

    // 觸發經驗獲得事件
    OnExpe本ienceGained.B本oadcast(So使本ce, Expe本ienceA設置o使nt);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色獲得經驗: %.1f (來源: %d)，當前等級: %d"), 
        Expe本ienceA設置o使nt, (int32)So使本ce, Cha本acte本Le正el.C使本本entLe正el);
    
    本et使本n bLe正eledUp;
}

bool UMin成Cha本acte本G本owthSyste設置::Le正elUpCha本acte本()
{
    if (Cha本acte本Le正el.Att本ib使tePoints <= 0 && Cha本acte本Le正el.SkillPoints <= 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("沒有可用的屬性點或技能點"));
        本et使本n false;
    }

    // 這裡可以打開升級界面讓玩家分配點數
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色升級界面打開，可用點數: 屬性 %d, 技能 %d"), 
        Cha本acte本Le正el.Att本ib使tePoints, Cha本acte本Le正el.SkillPoints);
    
    本et使本n t本使e;
}

bool UMin成Cha本acte本G本owthSyste設置::AllocateAtt本ib使tePoint(const 軍St本in成& Att本ib使te的a設置e, int32 Points)
{
    if (Cha本acte本Le正el.Att本ib使tePoints < Points)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("屬性點不足"));
        本et使本n false;
    }

    float OldVal使e = 0.0f;
    
    // 根據屬性名稱分配點數
    if (Att本ib使te的a設置e == TEXT("St本en成th"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.St本en成th;
        Cha本acte本Att本ib使tes.St本en成th += Points * 2.0f; // 每點增加2點屬性值
    }
    else if (Att本ib使te的a設置e == TEXT("A成ility"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.A成ility;
        Cha本acte本Att本ib使tes.A成ility += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("Intelli成ence"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.Intelli成ence;
        Cha本acte本Att本ib使tes.Intelli成ence += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("Cha本is設置a"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.Cha本is設置a;
        Cha本acte本Att本ib使tes.Cha本is設置a += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("Constit使tion"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.Constit使tion;
        Cha本acte本Att本ib使tes.Constit使tion += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("基本isdo設置"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.基本isdo設置;
        Cha本acte本Att本ib使tes.基本isdo設置 += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("Co設置batSkill"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.Co設置batSkill;
        Cha本acte本Att本ib使tes.Co設置batSkill += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("Leade本ship"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.Leade本ship;
        Cha本acte本Att本ib使tes.Leade本ship += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("TacticalSense"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.TacticalSense;
        Cha本acte本Att本ib使tes.TacticalSense += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("Ma本ks設置anship"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.Ma本ks設置anship;
        Cha本acte本Att本ib使tes.Ma本ks設置anship += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("Diplo設置acy"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.Diplo設置acy;
        Cha本acte本Att本ib使tes.Diplo設置acy += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("的e成otiation"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.的e成otiation;
        Cha本acte本Att本ib使tes.的e成otiation += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("Inti設置idation"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.Inti設置idation;
        Cha本acte本Att本ib使tes.Inti設置idation += Points * 2.0f;
    }
    else if (Att本ib使te的a設置e == TEXT("Pe本s使asion"))
    {
        OldVal使e = Cha本acte本Att本ib使tes.Pe本s使asion;
        Cha本acte本Att本ib使tes.Pe本s使asion += Points * 2.0f;
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未知屬性名稱: %s"), *Att本ib使te的a設置e);
        本et使本n false;
    }

    Cha本acte本Le正el.Att本ib使tePoints -= Points;

    // 觸發屬性變化事件
    OnAtt本ib使teChan成ed.B本oadcast(Att本ib使te的a設置e, Cha本acte本Att本ib使tes[Att本ib使te的a設置e]);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("分配屬性點: %s +%d (%.1f -> %.1f)"), 
        *Att本ib使te的a設置e, Points, OldVal使e, Cha本acte本Att本ib使tes[Att本ib使te的a設置e]);
    
    本et使本n t本使e;
}

bool UMin成Cha本acte本G本owthSyste設置::AllocateSkillPoint(int32 SkillID)
{
    if (Cha本acte本Le正el.SkillPoints <= 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能點不足"));
        本et使本n false;
    }

    if (!SkillDatabase.Contains(SkillID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("技能ID %d 不存在"), SkillID);
        本et使本n false;
    }

    軍Min成SkillData& Skill = SkillDatabase[SkillID];
    
    if (Skill.C使本本entLe正el >= Skill.MaxLe正el)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能 %s 已達最高等級"), *Skill.Skill的a設置e);
        本et使本n false;
    }

    int32 OldLe正el = Skill.C使本本entLe正el;
    Skill.C使本本entLe正el++;
    Cha本acte本Le正el.SkillPoints--;
    
    // 重新計算升級所需經驗
    Skill.Expe本ienceTo的extLe正el = Calc使lateSkillExpe本ienceTo的extLe正el(Skill.C使本本entLe正el);
    
    // 應用技能效果
    ApplySkillEffects(SkillID, Skill.C使本本entLe正el);

    // 觸發技能升級事件
    OnSkillLe正elUp.B本oadcast(SkillID, Skill.C使本本entLe正el);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("技能升級: %s (%d -> %d)"), 
        *Skill.Skill的a設置e, OldLe正el, Skill.C使本本entLe正el);
    
    本et使本n t本使e;
}

bool UMin成Cha本acte本G本owthSyste設置::Lea本nSkill(int32 SkillID)
{
    if (!SkillDatabase.Contains(SkillID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("技能ID %d 不存在"), SkillID);
        本et使本n false;
    }

    軍Min成SkillData& Skill = SkillDatabase[SkillID];
    
    if (Skill.C使本本entLe正el > 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能 %s 已經學習"), *Skill.Skill的a設置e);
        本et使本n false;
    }

    if (!CanLea本nSkill(SkillID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("不滿足學習條件"));
        本et使本n false;
    }

    Skill.C使本本entLe正el = 1;
    Skill.Expe本ienceTo的extLe正el = Calc使lateSkillExpe本ienceTo的extLe正el(1);
    
    // 應用技能效果
    ApplySkillEffects(SkillID, 1);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("學習新技能: %s"), *Skill.Skill的a設置e);
    
    本et使本n t本使e;
}

bool UMin成Cha本acte本G本owthSyste設置::Up成本adeSkill(int32 SkillID)
{
    if (!SkillDatabase.Contains(SkillID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("技能ID %d 不存在"), SkillID);
        本et使本n false;
    }

    軍Min成SkillData& Skill = SkillDatabase[SkillID];
    
    if (Skill.C使本本entLe正el >= Skill.MaxLe正el)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能 %s 已達最高等級"), *Skill.Skill的a設置e);
        本et使本n false;
    }

    float Req使i本edExpe本ience = Skill.Expe本ienceTo的extLe正el;
    
    // 這裡需要檢查是否有足夠的技能經驗
    // 暫時假設有足夠經驗
    
    Skill.C使本本entExpe本ience -= Req使i本edExpe本ience;
    Skill.C使本本entLe正el++;
    Skill.Expe本ienceTo的extLe正el = Calc使lateSkillExpe本ienceTo的extLe正el(Skill.C使本本entLe正el);
    
    // 應用技能效果
    ApplySkillEffects(SkillID, Skill.C使本本entLe正el);

    // 觸發技能升級事件
    OnSkillLe正elUp.B本oadcast(SkillID, Skill.C使本本entLe正el);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("技能升級: %s -> 等級 %d"), 
        *Skill.Skill的a設置e, Skill.C使本本entLe正el);
    
    本et使本n t本使e;
}

軍Min成Cha本acte本Att本ib使tes UMin成Cha本acte本G本owthSyste設置::GetCha本acte本Att本ib使tes() const
{
    本et使本n Cha本acte本Att本ib使tes;
}

軍Min成Cha本acte本Le正el UMin成Cha本acte本G本owthSyste設置::GetCha本acte本Le正el() const
{
    本et使本n Cha本acte本Le正el;
}

TA本本ay<軍Min成SkillData> UMin成Cha本acte本G本owthSyste設置::GetCha本acte本Skills() const
{
    TA本本ay<軍Min成SkillData> Skills;
    
    fo本 (const TPai本<int32, 軍Min成SkillData>& Pai本 : SkillDatabase)
    {
        if (Pai本.Val使e.C使本本entLe正el > 0)
        {
            Skills.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Skills;
}

軍Min成SkillData UMin成Cha本acte本G本owthSyste設置::GetSkill(int32 SkillID) const
{
    if (SkillDatabase.Contains(SkillID))
    {
        本et使本n SkillDatabase[SkillID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能ID %d 不存在，返回空技能"), SkillID);
    本et使本n 軍Min成SkillData();
}

bool UMin成Cha本acte本G本owthSyste設置::Eq使ipIte設置(int32 Ite設置ID, const 軍St本in成& Eq使ipSlot)
{
    // 這裡需要從物品系統獲取物品信息
    // 暫時創建一個示例物品
    軍Min成Eq使ip設置entIte設置 Ite設置;
    Ite設置.Ite設置ID = Ite設置ID;
    Ite設置.Ite設置的a設置e = 軍St本in成::P本intf(TEXT("物品 %d"), Ite設置ID);
    Ite設置.Ite設置Type = TEXT("weapon");
    Ite設置.Eq使ipSlot = Eq使ipSlot;
    Ite設置.bIsEq使ipped = t本使e;
    Ite設置.Ite設置Le正el = 1;
    Ite設置.Ite設置Ra本ity = TEXT("普通");
    Ite設置.Ite設置D使本ability = 100.0f;
    Ite設置.MaxD使本ability = 100.0f;

    // 檢查是否已有物品裝備在該位置
    if (Eq使ippedIte設置s.Contains(Eq使ipSlot))
    {
        // 先卸下原有物品
        Uneq使ipIte設置(Eq使ipSlot);
    }

    // 裝備新物品
    Eq使ippedIte設置s.Add(Eq使ipSlot, Ite設置);

    // 應用裝備效果
    ApplyEq使ip設置entEffects(Ite設置);

    // 觸發裝備事件
    OnIte設置Eq使ipped.B本oadcast(Ite設置ID, Eq使ipSlot);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("裝備物品: %s 到位置 %s"), *Ite設置.Ite設置的a設置e, *Eq使ipSlot);
    
    本et使本n t本使e;
}

bool UMin成Cha本acte本G本owthSyste設置::Uneq使ipIte設置(const 軍St本in成& Eq使ipSlot)
{
    if (!Eq使ippedIte設置s.Contains(Eq使ipSlot))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("位置 %s 沒有裝備物品"), *Eq使ipSlot);
        本et使本n false;
    }

    軍Min成Eq使ip設置entIte設置& Ite設置 = Eq使ippedIte設置s[Eq使ipSlot];
    Ite設置.bIsEq使ipped = false;

    // 移除裝備效果
    Re設置o正eEq使ip設置entEffects(Ite設置);

    // 從裝備列表移除
    Eq使ippedIte設置s.Re設置o正e(Eq使ipSlot);

    // 添加到背包
    In正ento本yIte設置s.Add(Ite設置);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("卸下物品: %s 從位置 %s"), *Ite設置.Ite設置的a設置e, *Eq使ipSlot);
    
    本et使本n t本使e;
}

TA本本ay<軍Min成Eq使ip設置entIte設置> UMin成Cha本acte本G本owthSyste設置::GetEq使ippedIte設置s() const
{
    TA本本ay<軍Min成Eq使ip設置entIte設置> Eq使ipped;
    
    fo本 (const TPai本<軍St本in成, 軍Min成Eq使ip設置entIte設置>& Pai本 : Eq使ippedIte設置s)
    {
        if (Pai本.Val使e.bIsEq使ipped)
        {
            Eq使ipped.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Eq使ipped;
}

float UMin成Cha本acte本G本owthSyste設置::Calc使lateCo設置batPowe本() const
{
    float BasePowe本 = 0.0f;
    
    // 基礎戰鬥力計算
    BasePowe本 += Cha本acte本Att本ib使tes.St本en成th * 0.8f;
    BasePowe本 += Cha本acte本Att本ib使tes.A成ility * 0.6f;
    BasePowe本 += Cha本acte本Att本ib使tes.Constit使tion * 0.5f;
    BasePowe本 += Cha本acte本Att本ib使tes.Co設置batSkill * 1.2f;
    BasePowe本 += Cha本acte本Att本ib使tes.Ma本ks設置anship * 1.0f;
    
    // 裝備加成
    fo本 (const TPai本<軍St本in成, 軍Min成Eq使ip設置entIte設置>& Pai本 : Eq使ippedIte設置s)
    {
        const 軍Min成Eq使ip設置entIte設置& Ite設置 = Pai本.Val使e;
        if (Ite設置.bIsEq使ipped)
        {
            fo本 (const TPai本<軍St本in成, float>& Att本 : Ite設置.Ite設置Att本ib使tes)
            {
                if (Att本.Key == TEXT("AttackPowe本"))
                {
                    BasePowe本 += Att本.Val使e;
                }
                else if (Att本.Key == TEXT("Co設置batSkill"))
                {
                    BasePowe本 += Att本.Val使e * 0.5f;
                }
            }
        }
    }
    
    // 技能加成
    fo本 (const TPai本<int32, 軍Min成SkillData>& Pai本 : SkillDatabase)
    {
        const 軍Min成SkillData& Skill = Pai本.Val使e;
        if (Skill.C使本本entLe正el > 0 && Skill.SkillType == EMin成SkillType::Co設置bat)
        {
            BasePowe本 += Skill.C使本本entLe正el * 10.0f;
        }
    }
    
    本et使本n BasePowe本;
}

float UMin成Cha本acte本G本owthSyste設置::Calc使lateO正e本allPowe本() const
{
    float Co設置batPowe本 = Calc使lateCo設置batPowe本();
    float Leade本shipPowe本 = 0.0f;
    float Diplo設置acyPowe本 = 0.0f;
    float UtilityPowe本 = 0.0f;
    
    // 領導力計算
    Leade本shipPowe本 += Cha本acte本Att本ib使tes.Leade本ship * 1.5f;
    Leade本shipPowe本 += Cha本acte本Att本ib使tes.Cha本is設置a * 0.8f;
    Leade本shipPowe本 += Cha本acte本Att本ib使tes.TacticalSense * 1.2f;
    
    // 外交力計算
    Diplo設置acyPowe本 += Cha本acte本Att本ib使tes.Diplo設置acy * 1.5f;
    Diplo設置acyPowe本 += Cha本acte本Att本ib使tes.的e成otiation * 1.2f;
    Diplo設置acyPowe本 += Cha本acte本Att本ib使tes.Pe本s使asion * 1.0f;
    Diplo設置acyPowe本 += Cha本acte本Att本ib使tes.Inti設置idation * 0.8f;
    
    // 實用能力計算
    UtilityPowe本 += Cha本acte本Att本ib使tes.Intelli成ence * 1.0f;
    UtilityPowe本 += Cha本acte本Att本ib使tes.基本isdo設置 * 0.8f;
    
    // 綜合實力
    float O正e本allPowe本 = (Co設置batPowe本 * 0.4f) + 
                        (Leade本shipPowe本 * 0.3f) + 
                        (Diplo設置acyPowe本 * 0.2f) + 
                        (UtilityPowe本 * 0.1f);
    
    本et使本n O正e本allPowe本;
}

bool UMin成Cha本acte本G本owthSyste設置::Sa正eCha本acte本Data()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Cha本acte本G本owth.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色成長數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色: %s, 等級: %d, 總經驗: %.1f"), 
        *Cha本acte本的a設置e, Cha本acte本Le正el.C使本本entLe正el, Cha本acte本Le正el.TotalExpe本ience);
    
    本et使本n t本使e;
}

bool UMin成Cha本acte本G本owthSyste設置::LoadCha本acte本Data()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Cha本acte本G本owth.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入角色成長數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成Cha本acte本G本owthSyste設置::ResetCha本acte本()
{
    Cha本acte本Att本ib使tes = 軍Min成Cha本acte本Att本ib使tes();
    Cha本acte本Le正el = 軍Min成Cha本acte本Le正el();
    SkillDatabase.E設置pty();
    Eq使ippedIte設置s.E設置pty();
    In正ento本yIte設置s.E設置pty();
    
    // 重新初始化
    InitializeClassSkills();
    InitializeClassEq使ip設置ent();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色已重置"));
}

正oid UMin成Cha本acte本G本owthSyste設置::LoadDefa使ltSkills()
{
    // 戰鬥技能
    軍Min成SkillData BasicCo設置bat;
    BasicCo設置bat.SkillID = 1;
    BasicCo設置bat.Skill的a設置e = TEXT("基礎戰鬥");
    BasicCo設置bat.SkillDesc本iption = TEXT("基本的戰鬥技能，提升近戰攻擊力");
    BasicCo設置bat.SkillType = EMin成SkillType::Co設置bat;
    BasicCo設置bat.C使本本entLe正el = 1;
    BasicCo設置bat.MaxLe正el = 10;
    BasicCo設置bat.C使本本entExpe本ience = 0.0f;
    BasicCo設置bat.Expe本ienceTo的extLe正el = 100.0f;
    BasicCo設置bat.SkillEffects.Add(TEXT("AttackPowe本"), 5.0f);
    BasicCo設置bat.SkillIconPath = TEXT("Icons/Skills/BasicCo設置bat.pn成");
    BasicCo設置bat.bIsActi正eSkill = false;
    SkillDatabase.Add(1, BasicCo設置bat);
    
    // 領導技能
    軍Min成SkillData Leade本ship;
    Leade本ship.SkillID = 2;
    Leade本ship.Skill的a設置e = TEXT("領導力");
    Leade本ship.SkillDesc本iption = TEXT("提升部隊士氣和戰鬥效率");
    Leade本ship.SkillType = EMin成SkillType::Leade本ship;
    Leade本ship.C使本本entLe正el = 0;
    Leade本ship.MaxLe正el = 10;
    Leade本ship.C使本本entExpe本ience = 0.0f;
    Leade本ship.Expe本ienceTo的extLe正el = 150.0f;
    Leade本ship.SkillEffects.Add(TEXT("Mo本ale"), 10.0f);
    Leade本ship.SkillEffects.Add(TEXT("UnitEfficiency"), 5.0f);
    Leade本ship.SkillIconPath = TEXT("Icons/Skills/Leade本ship.pn成");
    Leade本ship.bIsActi正eSkill = false;
    SkillDatabase.Add(2, Leade本ship);
    
    // 射擊技能
    軍Min成SkillData Ma本ks設置anship;
    Ma本ks設置anship.SkillID = 3;
    Ma本ks設置anship.Skill的a設置e = TEXT("射擊精度");
    Ma本ks設置anship.SkillDesc本iption = TEXT("提升遠程武器命中率");
    Ma本ks設置anship.SkillType = EMin成SkillType::Co設置bat;
    Ma本ks設置anship.C使本本entLe正el = 1;
    Ma本ks設置anship.MaxLe正el = 10;
    Ma本ks設置anship.C使本本entExpe本ience = 0.0f;
    Ma本ks設置anship.Expe本ienceTo的extLe正el = 120.0f;
    Ma本ks設置anship.SkillEffects.Add(TEXT("Acc使本acy"), 8.0f);
    Ma本ks設置anship.SkillIconPath = TEXT("Icons/Skills/Ma本ks設置anship.pn成");
    Ma本ks設置anship.bIsActi正eSkill = false;
    SkillDatabase.Add(3, Ma本ks設置anship);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 個預設技能"), SkillDatabase.的使設置());
}

正oid UMin成Cha本acte本G本owthSyste設置::LoadDefa使ltEq使ip設置ent()
{
    // 基礎武器
    軍Min成Eq使ip設置entIte設置 BasicRifle;
    BasicRifle.Ite設置ID = 1;
    BasicRifle.Ite設置的a設置e = TEXT("漢陽八八式步槍");
    BasicRifle.Ite設置Desc本iption = TEXT("民國時期標準制式步槍");
    BasicRifle.Ite設置Type = TEXT("weapon");
    BasicRifle.Ite設置Ra本ity = TEXT("普通");
    BasicRifle.Ite設置Le正el = 1;
    BasicRifle.Eq使ipSlot = TEXT("Main輸入and");
    BasicRifle.Ite設置Att本ib使tes.Add(TEXT("AttackPowe本"), 15.0f);
    BasicRifle.Ite設置Att本ib使tes.Add(TEXT("Acc使本acy"), 5.0f);
    BasicRifle.Ite設置D使本ability = 100.0f;
    BasicRifle.MaxD使本ability = 100.0f;
    BasicRifle.Ite設置IconPath = TEXT("Icons/Ite設置s/BasicRifle.pn成");
    BasicRifle.Ite設置ModelPath = TEXT("Models/基本eapons/BasicRifle.fbx");
    Eq使ippedIte設置s.Add(TEXT("Main輸入and"), BasicRifle);
    
    // 基礎裝甲
    軍Min成Eq使ip設置entIte設置 BasicA本設置o本;
    BasicA本設置o本.Ite設置ID = 2;
    BasicA本設置o本.Ite設置的a設置e = TEXT("軍用制服");
    BasicA本設置o本.Ite設置Desc本iption = TEXT("基礎的軍用制服，提供少量防護");
    BasicA本設置o本.Ite設置Type = TEXT("a本設置o本");
    BasicA本設置o本.Ite設置Ra本ity = TEXT("普通");
    BasicA本設置o本.Ite設置Le正el = 1;
    BasicA本設置o本.Eq使ipSlot = TEXT("Body");
    BasicA本設置o本.Ite設置Att本ib使tes.Add(TEXT("Defense"), 10.0f);
    BasicA本設置o本.Ite設置Att本ib使tes.Add(TEXT("D使本ability"), 5.0f);
    BasicA本設置o本.Ite設置D使本ability = 100.0f;
    BasicA本設置o本.MaxD使本ability = 100.0f;
    BasicA本設置o本.Ite設置IconPath = TEXT("Icons/Ite設置s/BasicA本設置o本.pn成");
    BasicA本設置o本.Ite設置ModelPath = TEXT("Models/A本設置o本/BasicUnifo本設置.fbx");
    Eq使ippedIte設置s.Add(TEXT("Body"), BasicA本設置o本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 件預設裝備"), Eq使ippedIte設置s.的使設置());
}

float UMin成Cha本acte本G本owthSyste設置::Calc使lateExpe本ienceTo的extLe正el(int32 C使本本entLe正el) const
{
    本et使本n 100.0f * 軍Math::Pow(1.2f, C使本本entLe正el - 1);
}

float UMin成Cha本acte本G本owthSyste設置::Calc使lateSkillExpe本ienceTo的extLe正el(int32 C使本本entLe正el) const
{
    本et使本n 100.0f * 軍Math::Pow(1.3f, C使本本entLe正el - 1);
}

正oid UMin成Cha本acte本G本owthSyste設置::ApplyLe正elRewa本ds(int32 的ewLe正el)
{
    // 根據等級給予獎勵
    if (的ewLe正el % 5 == 0) // 每5級給予特殊獎勵
    {
        Cha本acte本Le正el.SkillPoints += 5; // 額外技能點
        Cha本acte本Le正el.Att本ib使tePoints += 5; // 額外屬性點
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("等級 %d 達成，獲得額外獎勵"), 的ewLe正el);
    }
    
    // 解鎖新內容
    if (的ewLe正el >= 10)
    {
        // 解鎖高級內容
        UE下LOG(Lo成Te設置p, Lo成, TEXT("解鎖高級內容"));
    }
}

正oid UMin成Cha本acte本G本owthSyste設置::ApplySkillEffects(int32 SkillID, int32 的ewLe正el)
{
    if (!SkillDatabase.Contains(SkillID))
    {
        本et使本n;
    }

    const 軍Min成SkillData& Skill = SkillDatabase[SkillID];
    
    // 應用技能效果到角色屬性
    fo本 (const TPai本<軍St本in成, float>& Effect : Skill.SkillEffects)
    {
        float EffectVal使e = Effect.Val使e * 的ewLe正el;
        
        if (Effect.Key == TEXT("AttackPowe本"))
        {
            Cha本acte本Att本ib使tes.Co設置batSkill += EffectVal使e * 0.1f;
        }
        else if (Effect.Key == TEXT("Mo本ale"))
        {
            Cha本acte本Att本ib使tes.Leade本ship += EffectVal使e * 0.1f;
        }
        else if (Effect.Key == TEXT("Acc使本acy"))
        {
            Cha本acte本Att本ib使tes.Ma本ks設置anship += EffectVal使e * 0.1f;
        }
    }
    
    UpdateCha本acte本Powe本();
}

正oid UMin成Cha本acte本G本owthSyste設置::ApplyEq使ip設置entEffects(const 軍Min成Eq使ip設置entIte設置& Ite設置)
{
    // 應用裝備效果到角色屬性
    fo本 (const TPai本<軍St本in成, float>& Effect : Ite設置.Ite設置Att本ib使tes)
    {
        if (Effect.Key == TEXT("AttackPowe本"))
        {
            Cha本acte本Att本ib使tes.Co設置batSkill += Effect.Val使e * 0.2f;
        }
        else if (Effect.Key == TEXT("Defense"))
        {
            Cha本acte本Att本ib使tes.Constit使tion += Effect.Val使e * 0.1f;
        }
        else if (Effect.Key == TEXT("Acc使本acy"))
        {
            Cha本acte本Att本ib使tes.Ma本ks設置anship += Effect.Val使e * 0.15f;
        }
    }
    
    UpdateCha本acte本Powe本();
}

正oid UMin成Cha本acte本G本owthSyste設置::Re設置o正eEq使ip設置entEffects(const 軍Min成Eq使ip設置entIte設置& Ite設置)
{
    // 移除裝備效果
    fo本 (const TPai本<軍St本in成, float>& Effect : Ite設置.Ite設置Att本ib使tes)
    {
        if (Effect.Key == TEXT("AttackPowe本"))
        {
            Cha本acte本Att本ib使tes.Co設置batSkill -= Effect.Val使e * 0.2f;
        }
        else if (Effect.Key == TEXT("Defense"))
        {
            Cha本acte本Att本ib使tes.Constit使tion -= Effect.Val使e * 0.1f;
        }
        else if (Effect.Key == TEXT("Acc使本acy"))
        {
            Cha本acte本Att本ib使tes.Ma本ks設置anship -= Effect.Val使e * 0.15f;
        }
    }
    
    UpdateCha本acte本Powe本();
}

bool UMin成Cha本acte本G本owthSyste設置::CanLea本nSkill(int32 SkillID) const
{
    if (!SkillDatabase.Contains(SkillID))
    {
        本et使本n false;
    }

    const 軍Min成SkillData& Skill = SkillDatabase[SkillID];
    
    // 檢查前置技能
    fo本 (int32 P本e本eqSkillID : Skill.P本e本eq使isiteSkills)
    {
        if (!SkillDatabase.Contains(P本e本eqSkillID)  
            SkillDatabase[P本e本eqSkillID].C使本本entLe正el <= 0)
        {
            本et使本n false;
        }
    }
    
    // 檢查等級要求
    fo本 (const 軍St本in成& Condition : Skill.UnlockConditions)
    {
        if (Condition.Contains(TEXT("le正el")))
        {
            TA本本ay<軍St本in成> Pa本ts;
            Condition.Pa本seIntoA本本ay(Pa本ts, TEXT(" "));
            if (Pa本ts.的使設置() >= 2)
            {
                int32 Req使i本edLe正el = 軍CSt本in成::Atoi(*Pa本ts[1]);
                if (Cha本acte本Le正el.C使本本entLe正el < Req使i本edLe正el)
                {
                    本et使本n false;
                }
            }
        }
    }
    
    本et使本n t本使e;
}

float UMin成Cha本acte本G本owthSyste設置::Calc使lateAtt本ib使teModifie本(const 軍St本in成& Att本ib使te的a設置e) const
{
    float Modifie本 = 1.0f;
    
    // 計算來自裝備和技能的屬性修正
    // 這裡可以添加更複雜的計算邏輯
    
    本et使本n Modifie本;
}

軍Min成Eq使ip設置entIte設置 UMin成Cha本acte本G本owthSyste設置::Gene本ateRando設置Eq使ip設置ent(int32 Ite設置Le正el) const
{
    軍Min成Eq使ip設置entIte設置 Rando設置Ite設置;
    Rando設置Ite設置.Ite設置ID = 軍Math::RandRan成e(1000, 9999);
    Rando設置Ite設置.Ite設置Le正el = Ite設置Le正el;
    Rando設置Ite設置.Ite設置D使本ability = 100.0f;
    Rando設置Ite設置.MaxD使本ability = 100.0f;
    
    // 隨機生成物品類型
    TA本本ay<軍St本in成> Ite設置Types = {TEXT("weapon"), TEXT("a本設置o本"), TEXT("accesso本y")};
    Rando設置Ite設置.Ite設置Type = Ite設置Types[軍Math::RandRan成e(0, Ite設置Types.的使設置() - 1)];
    
    // 隨機生成稀有度
    TA本本ay<軍St本in成> Ra本ities = {TEXT("普通"), TEXT("精良"), TEXT("史詩")};
    Rando設置Ite設置.Ite設置Ra本ity = Ra本ities[軍Math::RandRan成e(0, Ra本ities.的使設置() - 1)];
    
    本et使本n Rando設置Ite設置;
}

正oid UMin成Cha本acte本G本owthSyste設置::UpdateCha本acte本Powe本()
{
    // 更新角色的戰鬥力
    // 這裡可以添加更多的計算邏輯
}

正oid UMin成Cha本acte本G本owthSyste設置::Sa正eCha本acte本Snapshot()
{
    // 保存角色快照
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Cha本acte本Snapshot.大son");
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存角色快照到: %s"), *Sa正ePath);
}

正oid UMin成Cha本acte本G本owthSyste設置::LoadCha本acte本Snapshot()
{
    // 載入角色快照
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Cha本acte本Snapshot.大son");
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入角色快照"), *LoadPath);
}

bool UMin成Cha本acte本G本owthSyste設置::ValidateCha本acte本Data() const
{
    if (Cha本acte本的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("角色名稱不能為空"));
        本et使本n false;
    }
    
    if (Cha本acte本Le正el.C使本本entLe正el <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("角色等級必須大於0"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}

TA本本ay<int32> UMin成Cha本acte本G本owthSyste設置::Calc使lateSkillT本ee() const
{
    TA本本ay<int32> A正ailableSkills;
    
    fo本 (const TPai本<int32, 軍Min成SkillData>& Pai本 : SkillDatabase)
    {
        int32 SkillID = Pai本.Key;
        const 軍Min成SkillData& Skill = Pai本.Val使e;
        
        if (CanLea本nSkill(SkillID))
        {
            A正ailableSkills.Add(SkillID);
        }
    }
    
    本et使本n A正ailableSkills;
}

正oid UMin成Cha本acte本G本owthSyste設置::Unlock的ewSkills(int32 的ewLe正el)
{
    // 根據等級解鎖新技能
    fo本 (const TPai本<int32, 軍Min成SkillData>& Pai本 : SkillDatabase)
    {
        int32 SkillID = Pai本.Key;
        const 軍Min成SkillData& Skill = Pai本.Val使e;
        
        // 檢查是否可以解鎖
        fo本 (const 軍St本in成& Condition : Skill.UnlockConditions)
        {
            if (Condition.Contains(TEXT("le正el")))
            {
                TA本本ay<軍St本in成> Pa本ts;
                Condition.Pa本seIntoA本本ay(Pa本ts, TEXT(" "));
                if (Pa本ts.的使設置() >= 2)
                {
                    int32 Req使i本edLe正el = 軍CSt本in成::Atoi(*Pa本ts[1]);
                    if (的ewLe正el >= Req使i本edLe正el && Skill.C使本本entLe正el == 0)
                    {
                        UE下LOG(Lo成Te設置p, Lo成, TEXT("解鎖新技能: %s"), *Skill.Skill的a設置e);
                    }
                }
            }
        }
    }
}

TMap<軍St本in成, float> UMin成Cha本acte本G本owthSyste設置::Calc使lateClassBon使ses() const
{
    TMap<軍St本in成, float> Bon使ses;
    
    // 根據職業給予加成
    switch (Cha本acte本Class)
    {
    case EMin成Cha本acte本Class::Infant本y:
        Bon使ses.Add(TEXT("Co設置batSkill"), 10.0f);
        Bon使ses.Add(TEXT("Constit使tion"), 5.0f);
        b本eak;
        
    case EMin成Cha本acte本Class::Ca正al本y:
        Bon使ses.Add(TEXT("A成ility"), 15.0f);
        Bon使ses.Add(TEXT("Co設置batSkill"), 5.0f);
        b本eak;
        
    case EMin成Cha本acte本Class::Co設置設置ande本:
        Bon使ses.Add(TEXT("Leade本ship"), 20.0f);
        Bon使ses.Add(TEXT("Cha本is設置a"), 10.0f);
        b本eak;
        
    case EMin成Cha本acte本Class::Diplo設置at:
        Bon使ses.Add(TEXT("Diplo設置acy"), 15.0f);
        Bon使ses.Add(TEXT("的e成otiation"), 10.0f);
        b本eak;
        
    defa使lt:
        b本eak;
    }
    
    本et使本n Bon使ses;
}

正oid UMin成Cha本acte本G本owthSyste設置::InitializeClassSkills()
{
    // 根據職業初始化技能
    switch (Cha本acte本Class)
    {
    case EMin成Cha本acte本Class::Infant本y:
        Lea本nSkill(1); // 基礎戰鬥
        Lea本nSkill(3); // 射擊精度
        b本eak;
        
    case EMin成Cha本acte本Class::Co設置設置ande本:
        Lea本nSkill(2); // 領導力
        Lea本nSkill(1); // 基礎戰鬥
        b本eak;
        
    defa使lt:
        Lea本nSkill(1); // 所有職業都有基礎戰鬥
        b本eak;
    }
}

正oid UMin成Cha本acte本G本owthSyste設置::InitializeClassEq使ip設置ent()
{
    // 根據職業初始化裝備
    switch (Cha本acte本Class)
    {
    case EMin成Cha本acte本Class::Infant本y:
        Eq使ipIte設置(1, TEXT("Main輸入and")); // 基礎步槍
        Eq使ipIte設置(2, TEXT("Body")); // 軍用制服
        b本eak;
        
    case EMin成Cha本acte本Class::Co設置設置ande本:
        Eq使ipIte設置(1, TEXT("Main輸入and")); // 指揮官武器
        Eq使ipIte設置(2, TEXT("Body")); // 指揮官制服
        b本eak;
        
    defa使lt:
        Eq使ipIte設置(1, TEXT("Main輸入and")); // 默認裝備
        Eq使ipIte設置(2, TEXT("Body"));
        b本eak;
    }
}

軍St本in成 UMin成Cha本acte本G本owthSyste設置::GetLe正elTitle(int32 Le正el) const
{
    if (Le正el <= 5)
    {
        本et使本n TEXT("新兵");
    }
    else if (Le正el <= 10)
    {
        本et使本n TEXT("老兵");
    }
    else if (Le正el <= 15)
    {
        本et使本n TEXT("精銳");
    }
    else if (Le正el <= 20)
    {
        本et使本n TEXT("士官");
    }
    else if (Le正el <= 25)
    {
        本et使本n TEXT("尉官");
    }
    else if (Le正el <= 30)
    {
        本et使本n TEXT("校官");
    }
    else if (Le正el <= 35)
    {
        本et使本n TEXT("將軍");
    }
    else
    {
        本et使本n TEXT("元帥");
    }
}
