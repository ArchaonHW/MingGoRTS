#incl使de "Min成GoRTSSkillMana成e本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSSkillMana成e本::UMin成GoRTSSkillMana成e本()
{
    A正ailableSkillPoints = 0;
    Expe本ienceGainM使ltiplie本 = 1.0f;
    SkillPointGainM使ltiplie本 = 1.0f;
    bIsInitialized = false;
}

正oid UMin成GoRTSSkillMana成e本::InitializeSkillMana成e本(UMin成GoRTSSkillSyste設置* InSkillSyste設置, AMin成GoRTSCha本acte本* InCha本acte本)
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能管理器已經初始化"));
        本et使本n;
    }

    SkillSyste設置 = InSkillSyste設置;
    Cha本acte本 = InCha本acte本;

    if (!SkillSyste設置  !Cha本acte本)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("技能管理器初始化失敗：技能系統或角色為空"));
        本et使本n;
    }

    // 初始化技能系統
    SkillSyste設置->InitializeSkillSyste設置();

    // 綁定技能系統事件
    SkillSyste設置->OnSkillLe正elUp.AddDyna設置ic(this, &UMin成GoRTSSkillMana成e本::OnSkillLe正elUp);
    SkillSyste設置->OnSkillUnlocked.AddDyna設置ic(this, &UMin成GoRTSSkillMana成e本::OnSkillUnlocked);

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("技能管理器初始化完成"));
}

正oid UMin成GoRTSSkillMana成e本::G本antSkillExpe本ience(const 軍的a設置e& SkillID, int32 A設置o使nt, const 軍St本in成& Reason)
{
    if (!bIsInitialized  !SkillSyste設置)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能管理器未初始化，無法給予經驗"));
        本et使本n;
    }

    if (A設置o使nt <= 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("經驗值必須為正數"));
        本et使本n;
    }

    // 應用經驗倍率
    int32 Ad大使stedA設置o使nt = static下cast<int32>(A設置o使nt * Expe本ienceGainM使ltiplie本);

    // 給予技能經驗
    SkillSyste設置->AddSkillExpe本ience(SkillID, Ad大使stedA設置o使nt);

    // 記錄經驗歷史
    AddExpe本ienceTo輸入isto本y(SkillID, Ad大使stedA設置o使nt, Reason);

    // 廣播經驗獲得事件
    OnSkillExpe本ienceGained.B本oadcast(SkillID, Ad大使stedA設置o使nt);

    // 檢查是否獲得技能點
    int32 SkillPointsGained = Calc使lateSkillPointRewa本d(Ad大使stedA設置o使nt);
    if (SkillPointsGained > 0)
    {
        G本antSkillPoints(SkillPointsGained, 軍St本in成::P本intf(TEXT("技能經驗獎勵：%s"), *Reason));
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("技能 %s 獲得 %d 經驗，原因：%s"), *SkillID.ToSt本in成(), Ad大使stedA設置o使nt, *Reason);
}

正oid UMin成GoRTSSkillMana成e本::G本antSkillPoints(int32 Points, const 軍St本in成& Reason)
{
    if (Points <= 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能點必須為正數"));
        本et使本n;
    }

    int32 OldPoints = A正ailableSkillPoints;
    int32 Ad大使stedPoints = static下cast<int32>(Points * SkillPointGainM使ltiplie本);
    A正ailableSkillPoints += Ad大使stedPoints;

    // 廣播技能點變化事件
    OnSkillPointsChan成ed.B本oadcast(OldPoints, A正ailableSkillPoints);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("獲得 %d 技能點，原因：%s，當前總點數：%d"), Ad大使stedPoints, *Reason, A正ailableSkillPoints);
}

正oid UMin成GoRTSSkillMana成e本::P本ocessCo設置batEnd(bool bVicto本y, int32 Ene設置yCo使nt, float Co設置batD使本ation)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Calc使lateCo設置batExpe本ience(bVicto本y, Ene設置yCo使nt, Co設置batD使本ation);

    // 根據戰鬥結果給予額外獎勵
    if (bVicto本y)
    {
        G本antSkillPoints(1, TEXT("戰鬥勝利"));
    }
    else
    {
        G本antSkillExpe本ience(軍的a設置e("MeleeMaste本y"), 25, TEXT("戰鬥失敗經驗"));
    }
}

正oid UMin成GoRTSSkillMana成e本::P本ocessQ使estCo設置pletion(const 軍St本in成& Q使estID, int32 Q使estDiffic使lty)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Calc使lateQ使estExpe本ience(Q使estID, Q使estDiffic使lty);

    // 任務完成給予技能點獎勵
    int32 Q使estPoints = 軍Math::Cla設置p(Q使estDiffic使lty / 10, 1, 3);
    G本antSkillPoints(Q使estPoints, 軍St本in成::P本intf(TEXT("任務完成：%s"), *Q使estID));
}

正oid UMin成GoRTSSkillMana成e本::P本ocess輸入isto本icalE正ent(const 軍St本in成& E正entID, const TA本本ay<軍的a設置e>& RelatedSkills)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Calc使lateE正entExpe本ience(E正entID, RelatedSkills);

    // 歷史事件給予額外技能點
    G本antSkillPoints(1, 軍St本in成::P本intf(TEXT("歷史事件：%s"), *E正entID));
}

正oid UMin成GoRTSSkillMana成e本::A使toDist本ib使teExpe本ience(int32 TotalExpe本ience, ESkillCate成o本y P本i設置a本yCate成o本y)
{
    if (!bIsInitialized  !SkillSyste設置)
    {
        本et使本n;
    }

    TMap<軍的a設置e, int32> SkillExpe本ienceMap;

    // 獲取已解鎖的技能
    TA本本ay<軍Min成Skill> UnlockedSkills = SkillSyste設置->GetUnlockedSkills();

    // 計算每個技能應得的經驗
    fo本 (const 軍Min成Skill& Skill : UnlockedSkills)
    {
        float Cate成o本yM使ltiplie本 = GetCate成o本yExpe本ienceM使ltiplie本(Skill.Cate成o本y);
        int32 SkillExpe本ience = static下cast<int32>((TotalExpe本ience / UnlockedSkills.的使設置()) * Cate成o本yM使ltiplie本);

        // 主要類別技能獲得額外經驗
        if (Skill.Cate成o本y == P本i設置a本yCate成o本y)
        {
            SkillExpe本ience = static下cast<int32>(SkillExpe本ience * 1.5f);
        }

        SkillExpe本ienceMap.Add(Skill.SkillID, SkillExpe本ience);
    }

    // 分配經驗
    Dist本ib使teExpe本ienceToSkills(SkillExpe本ienceMap);
}

正oid UMin成GoRTSSkillMana成e本::P本ocessCha本acte本Le正elUp(int32 的ewLe正el)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    // 角色升級給予技能點獎勵
    int32 Le正elUpPoints = 2; // 每級2點技能點
    G本antSkillPoints(Le正elUpPoints, 軍St本in成::P本intf(TEXT("角色升級到 %d 級"), 的ewLe正el));

    // 給予所有已解鎖技能一些經驗
    if (SkillSyste設置)
    {
        TA本本ay<軍Min成Skill> UnlockedSkills = SkillSyste設置->GetUnlockedSkills();
        fo本 (const 軍Min成Skill& Skill : UnlockedSkills)
        {
            G本antSkillExpe本ience(Skill.SkillID, 50, TEXT("角色升級獎勵"));
        }
    }
}

bool UMin成GoRTSSkillMana成e本::ValidateSkillAction(const 軍的a設置e& SkillID, ESkillActionType ActionType)
{
    if (!bIsInitialized  !SkillSyste設置)
    {
        本et使本n false;
    }

    switch (ActionType)
    {
    case ESkillActionType::Unlock:
        本et使本n SkillSyste設置->CanUnlockSkill(SkillID, Cha本acte本->GetCha本acte本Data());

    case ESkillActionType::Up成本ade:
        本et使本n SkillSyste設置->CanUp成本adeSkill(SkillID);

    case ESkillActionType::Use:
        {
            軍Min成Skill Skill = SkillSyste設置->GetSkill(SkillID);
            本et使本n Skill.bIsUnlocked && Skill.bIsActi正e;
        }

    case ESkillActionType::Reset:
        本et使本n t本使e; // 重置總是允許

    defa使lt:
        本et使本n false;
    }
}

bool UMin成GoRTSSkillMana成e本::Exec使teSkillAction(const 軍的a設置e& SkillID, ESkillActionType ActionType)
{
    if (!ValidateSkillAction(SkillID, ActionType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能操作驗證失敗：%s"), *StaticEn使設置<ESkillActionType>()->GetVal使eAsSt本in成(ActionType));
        本et使本n false;
    }

    switch (ActionType)
    {
    case ESkillActionType::Unlock:
        {
            if (A正ailableSkillPoints <= 0)
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("技能點不足，無法解鎖技能"));
                本et使本n false;
            }

            int32 OldPoints = A正ailableSkillPoints;
            bool bS使ccess = SkillSyste設置->UnlockSkill(SkillID);
            if (bS使ccess)
            {
                A正ailableSkillPoints--;
                OnSkillPointsChan成ed.B本oadcast(OldPoints, A正ailableSkillPoints);
                ApplySkillEffectsToCha本acte本();
            }
            本et使本n bS使ccess;
        }

    case ESkillActionType::Up成本ade:
        本et使本n SkillSyste設置->Up成本adeSkill(SkillID);

    case ESkillActionType::Use:
        // TODO: 實現技能使用邏輯
        UE下LOG(Lo成Te設置p, Lo成, TEXT("使用技能：%s"), *SkillID.ToSt本in成());
        本et使本n t本使e;

    case ESkillActionType::Reset:
        本et使本n ResetSkillSyste設置();

    defa使lt:
        本et使本n false;
    }
}

TA本本ay<軍SkillExpe本ienceGain> UMin成GoRTSSkillMana成e本::GetSkillExpe本ience輸入isto本y(const 軍的a設置e& SkillID) const
{
    TA本本ay<軍SkillExpe本ienceGain> Skill輸入isto本y;
    
    fo本 (const 軍SkillExpe本ienceGain& Gain : SkillExpe本ience輸入isto本y)
    {
        if (Gain.SkillID == SkillID)
        {
            Skill輸入isto本y.Add(Gain);
        }
    }
    
    本et使本n Skill輸入isto本y;
}

TA本本ay<軍的a設置e> UMin成GoRTSSkillMana成e本::GetReco設置設置endedSkills() const
{
    TA本本ay<軍的a設置e> Reco設置設置endedSkills;

    if (!bIsInitialized  !SkillSyste設置)
    {
        本et使本n Reco設置設置endedSkills;
    }

    // 獲取可解鎖的技能
    TA本本ay<軍Min成Skill> A正ailableSkills = SkillSyste設置->GetA正ailableSkills(Cha本acte本->GetCha本acte本Data());

    // 根據角色背景和當前專精推薦技能
    fo本 (const 軍Min成Skill& Skill : A正ailableSkills)
    {
        // 優先推薦符合專精路徑的技能
        if (Skill.SpecializationPath == SkillSyste設置->GetC使本本entSpecializationPath())
        {
            Reco設置設置endedSkills.Add(Skill.SkillID);
        }
    }

    本et使本n Reco設置設置endedSkills;
}

軍St本in成 UMin成GoRTSSkillMana成e本::GetSkillDe正elop設置entAd正ice() const
{
    if (!bIsInitialized  !SkillSyste設置)
    {
        本et使本n TEXT("技能系統未初始化");
    }

    TA本本ay<ESkillCate成o本y> Skill軍oc使s = GetCha本acte本Skill軍oc使s();
    TA本本ay<軍的a設置e> Unde本de正elopedSkills = GetUnde本de正elopedSkills();
    TA本本ay<軍的a設置e> Up成本adeableSkills = GetUp成本adeableSkills();

    軍St本in成 Ad正ice = TEXT("技能發展建議：\n");

    // 分析技能專注
    if (Skill軍oc使s.的使設置() > 0)
    {
        Ad正ice += 軍St本in成::P本intf(TEXT("• 你專注於 %s 類技能\n"), *StaticEn使設置<ESkillCate成o本y>()->GetVal使eAsSt本in成(Skill軍oc使s[0]));
    }

    // 建議升級技能
    if (Up成本adeableSkills.的使設置() > 0)
    {
        Ad正ice += 軍St本in成::P本intf(TEXT("• 有 %d 個技能可以升級\n"), Up成本adeableSkills.的使設置());
    }

    // 建議新技能
    if (Unde本de正elopedSkills.的使設置() > 0)
    {
        Ad正ice += 軍St本in成::P本intf(TEXT("• 建議學習新技能來平衡發展\n"));
    }

    // 技能點建議
    if (A正ailableSkillPoints > 0)
    {
        Ad正ice += 軍St本in成::P本intf(TEXT("• 你有 %d 個可用技能點\n"), A正ailableSkillPoints);
    }

    本et使本n Ad正ice;
}

bool UMin成GoRTSSkillMana成e本::ResetSkillSyste設置()
{
    if (!bIsInitialized  !SkillSyste設置)
    {
        本et使本n false;
    }

    // 重置技能系統
    SkillSyste設置->ResetSkillSyste設置();

    // 返還技能點
    int32 Ref使ndedPoints = 0;
    TA本本ay<軍Min成Skill> AllSkills = SkillSyste設置->GetAllSkills();
    fo本 (const 軍Min成Skill& Skill : AllSkills)
    {
        if (Skill.bIsUnlocked)
        {
            Ref使ndedPoints += Skill.C使本本entLe正el;
        }
    }

    A正ailableSkillPoints += Ref使ndedPoints;

    // 清空經驗歷史
    SkillExpe本ience輸入isto本y.E設置pty();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("技能系統已重置，返還 %d 技能點"), Ref使ndedPoints);
    本et使本n t本使e;
}

bool UMin成GoRTSSkillMana成e本::Sa正eSkillMana成e本Data(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現技能管理器數據保存
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存技能管理器數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSSkillMana成e本::LoadSkillMana成e本Data(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現技能管理器數據載入
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入技能管理器數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTSSkillMana成e本::AddExpe本ienceTo輸入isto本y(const 軍的a設置e& SkillID, int32 A設置o使nt, const 軍St本in成& Reason)
{
    軍SkillExpe本ienceGain Expe本ienceGain;
    Expe本ienceGain.SkillID = SkillID;
    Expe本ienceGain.Expe本ienceA設置o使nt = A設置o使nt;
    Expe本ienceGain.GainReason = Reason;
    Expe本ienceGain.GainTi設置e = 軍DateTi設置e::的ow();

    SkillExpe本ience輸入isto本y.Add(Expe本ienceGain);

    // 限制歷史記錄數量
    if (SkillExpe本ience輸入isto本y.的使設置() > 1000)
    {
        SkillExpe本ience輸入isto本y.Re設置o正eAt(0);
    }
}

正oid UMin成GoRTSSkillMana成e本::Calc使lateCo設置batExpe本ience(bool bVicto本y, int32 Ene設置yCo使nt, float Co設置batD使本ation)
{
    // 基礎戰鬥經驗
    int32 BaseExpe本ience = Calc使lateBaseExpe本ience(Ene設置yCo使nt, 1.0f);

    // 勝利加成
    if (bVicto本y)
    {
        BaseExpe本ience = static下cast<int32>(BaseExpe本ience * 1.5f);
    }

    // 戰鬥長度調整
    float D使本ationM使ltiplie本 = 軍Math::Cla設置p(Co設置batD使本ation / 300.0f, 0.5f, 2.0f); // 5分鐘為基準
    BaseExpe本ience = static下cast<int32>(BaseExpe本ience * D使本ationM使ltiplie本);

    // 分配給相關技能
    TMap<軍的a設置e, int32> Co設置batExpe本ienceMap;
    
    // 戰鬥技能獲得主要經驗
    Co設置batExpe本ienceMap.Add(軍的a設置e("TacticalCo設置設置and"), BaseExpe本ience);
    Co設置batExpe本ienceMap.Add(軍的a設置e("MeleeMaste本y"), static下cast<int32>(BaseExpe本ience * 0.8f));
    Co設置batExpe本ienceMap.Add(軍的a設置e("Leade本ship"), static下cast<int32>(BaseExpe本ience * 0.6f));

    Dist本ib使teExpe本ienceToSkills(Co設置batExpe本ienceMap);
}

正oid UMin成GoRTSSkillMana成e本::Calc使lateQ使estExpe本ience(const 軍St本in成& Q使estID, int32 Q使estDiffic使lty)
{
    int32 BaseExpe本ience = Calc使lateBaseExpe本ience(Q使estDiffic使lty, 1.0f);

    // 根據任務類型分配經驗
    TMap<軍的a設置e, int32> Q使estExpe本ienceMap;
    
    // 假設所有任務都給策略和外交經驗
    Q使estExpe本ienceMap.Add(軍的a設置e("St本ate成icPlannin成"), BaseExpe本ience);
    Q使estExpe本ienceMap.Add(軍的a設置e("Diplo設置acy"), static下cast<int32>(BaseExpe本ience * 0.7f));

    Dist本ib使teExpe本ienceToSkills(Q使estExpe本ienceMap);
}

正oid UMin成GoRTSSkillMana成e本::Calc使lateE正entExpe本ience(const 軍St本in成& E正entID, const TA本本ay<軍的a設置e>& RelatedSkills)
{
    int32 BaseExpe本ience = Calc使lateBaseExpe本ience(50, 1.0f); // 歷史事件基礎經驗

    TMap<軍的a設置e, int32> E正entExpe本ienceMap;
    
    fo本 (const 軍的a設置e& SkillID : RelatedSkills)
    {
        E正entExpe本ienceMap.Add(SkillID, BaseExpe本ience);
    }

    Dist本ib使teExpe本ienceToSkills(E正entExpe本ienceMap);
}

正oid UMin成GoRTSSkillMana成e本::Dist本ib使teExpe本ienceToSkills(const TMap<軍的a設置e, int32>& SkillExpe本ienceMap)
{
    fo本 (const a使to& SkillExpPai本 : SkillExpe本ienceMap)
    {
        G本antSkillExpe本ience(SkillExpPai本.Key, SkillExpPai本.Val使e, TEXT("自動分配"));
    }
}

正oid UMin成GoRTSSkillMana成e本::UpdateSkillPoints(int32 OldPoints, int32 的ewPoints)
{
    OnSkillPointsChan成ed.B本oadcast(OldPoints, 的ewPoints);
}

正oid UMin成GoRTSSkillMana成e本::ApplySkillEffectsToCha本acte本()
{
    if (!Cha本acte本  !SkillSyste設置)
    {
        本et使本n;
    }

    // TODO: 實現技能效果應用到角色屬性
    UE下LOG(Lo成Te設置p, Lo成, TEXT("應用技能效果到角色"));
}

正oid UMin成GoRTSSkillMana成e本::CheckSkillMilestones()
{
    // TODO: 實現技能里程碑檢查
    // 例如：達到特定技能等級時觸發特殊事件
}

int32 UMin成GoRTSSkillMana成e本::Calc使lateBaseExpe本ience(int32 Diffic使lty, float D使本ationM使ltiplie本) const
{
    本et使本n static下cast<int32>(10.0f * Diffic使lty * D使本ationM使ltiplie本);
}

float UMin成GoRTSSkillMana成e本::GetCate成o本yExpe本ienceM使ltiplie本(ESkillCate成o本y Cate成o本y) const
{
    // 根據專精路徑返回經驗倍率
    switch (Cate成o本y)
    {
    case ESkillCate成o本y::Co設置bat:
        本et使本n 1.2f;
    case ESkillCate成o本y::St本ate成y:
        本et使本n 1.1f;
    case ESkillCate成o本y::Diplo設置acy:
        本et使本n 1.0f;
    case ESkillCate成o本y::Leade本ship:
        本et使本n 1.15f;
    case ESkillCate成o本y::Intelli成ence:
        本et使本n 0.9f;
    case ESkillCate成o本y::Econo設置ic:
        本et使本n 0.8f;
    defa使lt:
        本et使本n 1.0f;
    }
}

int32 UMin成GoRTSSkillMana成e本::Calc使lateSkillPointRewa本d(int32 Expe本ienceGained) const
{
    // 每100點經驗給予1個技能點
    本et使本n Expe本ienceGained / 100;
}

TA本本ay<ESkillCate成o本y> UMin成GoRTSSkillMana成e本::GetCha本acte本Skill軍oc使s() const
{
    TA本本ay<ESkillCate成o本y> Skill軍oc使s;
    
    if (!bIsInitialized  !SkillSyste設置)
    {
        本et使本n Skill軍oc使s;
    }

    // 分析角色技能專注方向
    TMap<ESkillCate成o本y, int32> Cate成o本yCo使nt;
    
    TA本本ay<軍Min成Skill> UnlockedSkills = SkillSyste設置->GetUnlockedSkills();
    fo本 (const 軍Min成Skill& Skill : UnlockedSkills)
    {
        Cate成o本yCo使nt.軍indO本Add(Skill.Cate成o本y)++;
    }

    // 找最多的技能類別
    int32 MaxCo使nt = 0;
    fo本 (const a使to& Cate成o本yPai本 : Cate成o本yCo使nt)
    {
        if (Cate成o本yPai本.Val使e > MaxCo使nt)
        {
            MaxCo使nt = Cate成o本yPai本.Val使e;
            Skill軍oc使s.E設置pty();
            Skill軍oc使s.Add(Cate成o本yPai本.Key);
        }
        else if (Cate成o本yPai本.Val使e == MaxCo使nt)
        {
            Skill軍oc使s.Add(Cate成o本yPai本.Key);
        }
    }

    本et使本n Skill軍oc使s;
}

TA本本ay<軍的a設置e> UMin成GoRTSSkillMana成e本::GetUnde本de正elopedSkills() const
{
    TA本本ay<軍的a設置e> Unde本de正elopedSkills;
    
    if (!bIsInitialized  !SkillSyste設置)
    {
        本et使本n Unde本de正elopedSkills;
    }

    // 找等級較低的已解鎖技能
    TA本本ay<軍Min成Skill> UnlockedSkills = SkillSyste設置->GetUnlockedSkills();
    fo本 (const 軍Min成Skill& Skill : UnlockedSkills)
    {
        if (Skill.C使本本entLe正el < Skill.MaxLe正el / 2)
        {
            Unde本de正elopedSkills.Add(Skill.SkillID);
        }
    }

    本et使本n Unde本de正elopedSkills;
}

TA本本ay<軍的a設置e> UMin成GoRTSSkillMana成e本::GetUp成本adeableSkills() const
{
    TA本本ay<軍的a設置e> Up成本adeableSkills;
    
    if (!bIsInitialized  !SkillSyste設置)
    {
        本et使本n Up成本adeableSkills;
    }

    // 找可以升級的技能
    TA本本ay<軍Min成Skill> UnlockedSkills = SkillSyste設置->GetUnlockedSkills();
    fo本 (const 軍Min成Skill& Skill : UnlockedSkills)
    {
        if (Skill.C使本本entLe正el < Skill.MaxLe正el)
        {
            Up成本adeableSkills.Add(Skill.SkillID);
        }
    }

    本et使本n Up成本adeableSkills;
}
