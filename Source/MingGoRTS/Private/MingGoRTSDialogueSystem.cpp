#incl使de "Min成GoRTSDialo成使eSyste設置.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSDialo成使eSyste設置::UMin成GoRTSDialo成使eSyste設置()
{
    bIsInitialized = false;
}

正oid UMin成GoRTSDialo成使eSyste設置::InitializeDialo成使eSyste設置()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("對話系統已經初始化"));
        本et使本n;
    }

    // 初始化對話庫
    InitializeDialo成使eLib本a本y();

    // 構建對話映射
    Dialo成使eMap.E設置pty();
    fo本 (const 軍Min成Dialo成使e& Dialo成使e : AllDialo成使es)
    {
        Dialo成使eMap.Add(Dialo成使e.Dialo成使eID, Dialo成使e);
        
        // 構建角色對話映射
        if (!Cha本acte本Dialo成使eMap.Contains(Dialo成使e.Speake本ID))
        {
            Cha本acte本Dialo成使eMap.Add(Dialo成使e.Speake本ID, TA本本ay<軍St本in成>());
        }
        Cha本acte本Dialo成使eMap[Dialo成使e.Speake本ID].Add(Dialo成使e.Dialo成使eID);
    }

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("對話系統初始化完成，共載入 %d 個對話"), AllDialo成使es.的使設置());
}

TA本本ay<軍Min成Dialo成使e> UMin成GoRTSDialo成使eSyste設置::GetAllDialo成使es() const
{
    本et使本n AllDialo成使es;
}

軍Min成Dialo成使e UMin成GoRTSDialo成使eSyste設置::GetDialo成使e(const 軍St本in成& Dialo成使eID) const
{
    if (const 軍Min成Dialo成使e* Dialo成使e = Dialo成使eMap.軍ind(Dialo成使eID))
    {
        本et使本n *Dialo成使e;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到對話 ID：%s"), *Dialo成使eID);
    本et使本n 軍Min成Dialo成使e();
}

bool UMin成GoRTSDialo成使eSyste設置::Sta本tDialo成使e(const 軍St本in成& Dialo成使eID, const 軍Min成Cha本acte本Data& Playe本Cha本acte本)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("對話系統未初始化"));
        本et使本n false;
    }

    if (!Dialo成使eMap.Contains(Dialo成使eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("對話不存在：%s"), *Dialo成使eID);
        本et使本n false;
    }

    // 檢查對話是否可用
    if (!IsDialo成使eA正ailable(Dialo成使eID, Playe本Cha本acte本))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("對話不可用：%s"), *Dialo成使eID);
        本et使本n false;
    }

    // 設置當前對話和玩家
    C使本本entDialo成使e = Dialo成使eMap[Dialo成使eID];
    C使本本entPlaye本Cha本acte本 = Playe本Cha本acte本;

    // 添加到對話歷史
    AddDialo成使eTo輸入isto本y(Dialo成使eID);

    // 廣播對話開始事件
    OnDialo成使eSta本ted.B本oadcast(Dialo成使eID, C使本本entDialo成使e.Speake本ID);

    // 執行自動結果
    if (C使本本entDialo成使e.A使toO使tco設置es.的使設置() > 0)
    {
        Exec使teDialo成使eO使tco設置es(C使本本entDialo成使e.A使toO使tco設置es);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始對話：%s，講者：%s"), *Dialo成使eID, *C使本本entDialo成使e.Speake本ID);
    本et使本n t本使e;
}

bool UMin成GoRTSDialo成使eSyste設置::SelectDialo成使eOption(int32 OptionIndex)
{
    if (C使本本entDialo成使e.Dialo成使eID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("沒有正在進行的對話"));
        本et使本n false;
    }

    if (OptionIndex < 0  OptionIndex >= C使本本entDialo成使e.Dialo成使eOptions.的使設置())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("無效的對話選項索引：%d"), OptionIndex);
        本et使本n false;
    }

    const 軍Dialo成使eOption& SelectedOption = C使本本entDialo成使e.Dialo成使eOptions[OptionIndex];
    
    // 檢查選項是否可用
    if (!SelectedOption.bIsA正ailable)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("對話選項不可用：%d"), OptionIndex);
        本et使本n false;
    }

    // 廣播選項選擇事件
    OnDialo成使eOptionSelected.B本oadcast(C使本本entDialo成使e.Dialo成使eID, OptionIndex, SelectedOption.OptionText);

    // 執行選項結果
    if (SelectedOption.O使tco設置es.的使設置() > 0)
    {
        Exec使teDialo成使eO使tco設置es(SelectedOption.O使tco設置es);
    }

    // 檢查是否有下一個對話
    if (!SelectedOption.的extDialo成使eID.IsE設置pty())
    {
        本et使本n Sta本tDialo成使e(SelectedOption.的extDialo成使eID, C使本本entPlaye本Cha本acte本);
    }
    else
    {
        // 對話結束
        TA本本ay<軍Dialo成使eO使tco設置e> AllO使tco設置es = C使本本entDialo成使e.A使toO使tco設置es;
        AllO使tco設置es.Append(SelectedOption.O使tco設置es);
        OnDialo成使eCo設置pleted.B本oadcast(C使本本entDialo成使e.Dialo成使eID, AllO使tco設置es);
        
        // 清空當前對話
        C使本本entDialo成使e = 軍Min成Dialo成使e();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("對話結束"));
        本et使本n t本使e;
    }
}

bool UMin成GoRTSDialo成使eSyste設置::IsDialo成使eA正ailable(const 軍St本in成& Dialo成使eID, const 軍Min成Cha本acte本Data& Playe本Cha本acte本) const
{
    if (!Dialo成使eMap.Contains(Dialo成使eID))
    {
        本et使本n false;
    }

    const 軍Min成Dialo成使e& Dialo成使e = Dialo成使eMap[Dialo成使eID];

    // 檢查是否重複對話
    if (!Dialo成使e.bCanBeRepeated && Dialo成使e輸入isto本y.Contains(Dialo成使eID))
    {
        本et使本n false;
    }

    // 檢查觸發條件
    本et使本n ValidateDialo成使eConditions(Dialo成使e.T本i成成e本Conditions, Playe本Cha本acte本);
}

bool UMin成GoRTSDialo成使eSyste設置::IsDialo成使eOptionA正ailable(const 軍St本in成& Dialo成使eID, int32 OptionIndex, const 軍Min成Cha本acte本Data& Playe本Cha本acte本) const
{
    if (!Dialo成使eMap.Contains(Dialo成使eID))
    {
        本et使本n false;
    }

    const 軍Min成Dialo成使e& Dialo成使e = Dialo成使eMap[Dialo成使eID];
    if (OptionIndex < 0  OptionIndex >= Dialo成使e.Dialo成使eOptions.的使設置())
    {
        本et使本n false;
    }

    const 軍Dialo成使eOption& Option = Dialo成使e.Dialo成使eOptions[OptionIndex];
    
    // 檢查選項可用性標記
    if (!Option.bIsA正ailable)
    {
        本et使本n false;
    }

    // 檢查選項條件
    本et使本n ValidateDialo成使eConditions(Option.Conditions, Playe本Cha本acte本);
}

TA本本ay<軍Dialo成使eOption> UMin成GoRTSDialo成使eSyste設置::GetA正ailableDialo成使eOptions(const 軍St本in成& Dialo成使eID, const 軍Min成Cha本acte本Data& Playe本Cha本acte本) const
{
    TA本本ay<軍Dialo成使eOption> A正ailableOptions;
    
    if (Dialo成使eMap.Contains(Dialo成使eID))
    {
        const 軍Min成Dialo成使e& Dialo成使e = Dialo成使eMap[Dialo成使eID];
        fo本 (int32 i = 0; i < Dialo成使e.Dialo成使eOptions.的使設置(); ++i)
        {
            if (IsDialo成使eOptionA正ailable(Dialo成使eID, i, Playe本Cha本acte本))
            {
                A正ailableOptions.Add(Dialo成使e.Dialo成使eOptions[i]);
            }
        }
    }
    
    本et使本n A正ailableOptions;
}

TA本本ay<軍Min成Dialo成使e> UMin成GoRTSDialo成使eSyste設置::GetCha本acte本Dialo成使es(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍Min成Dialo成使e> Cha本acte本Dialo成使es;
    
    if (Cha本acte本Dialo成使eMap.Contains(Cha本acte本ID))
    {
        const TA本本ay<軍St本in成>& Dialo成使eIDs = Cha本acte本Dialo成使eMap[Cha本acte本ID];
        fo本 (const 軍St本in成& Dialo成使eID : Dialo成使eIDs)
        {
            if (Dialo成使eMap.Contains(Dialo成使eID))
            {
                Cha本acte本Dialo成使es.Add(Dialo成使eMap[Dialo成使eID]);
            }
        }
    }
    
    本et使本n Cha本acte本Dialo成使es;
}

TA本本ay<軍Min成Dialo成使e> UMin成GoRTSDialo成使eSyste設置::Get輸入isto本icalE正entDialo成使es(const 軍St本in成& E正entID) const
{
    TA本本ay<軍Min成Dialo成使e> E正entDialo成使es;
    
    fo本 (const 軍Min成Dialo成使e& Dialo成使e : AllDialo成使es)
    {
        if (Dialo成使e.Dialo成使eType == EDialo成使eType::輸入isto本icalE正ent && 
            Dialo成使e.輸入isto本icalContext.Contains(E正entID))
        {
            E正entDialo成使es.Add(Dialo成使e);
        }
    }
    
    本et使本n E正entDialo成使es;
}

TA本本ay<軍Min成Dialo成使e> UMin成GoRTSDialo成使eSyste設置::GetI設置po本tantDialo成使es() const
{
    TA本本ay<軍Min成Dialo成使e> I設置po本tantDialo成使es;
    
    fo本 (const 軍Min成Dialo成使e& Dialo成使e : AllDialo成使es)
    {
        if (Dialo成使e.bIsI設置po本tant)
        {
            I設置po本tantDialo成使es.Add(Dialo成使e);
        }
    }
    
    本et使本n I設置po本tantDialo成使es;
}

TA本本ay<軍St本in成> UMin成GoRTSDialo成使eSyste設置::GetDialo成使e輸入isto本y() const
{
    本et使本n Dialo成使e輸入isto本y;
}

正oid UMin成GoRTSDialo成使eSyste設置::AddDialo成使eTo輸入isto本y(const 軍St本in成& Dialo成使eID)
{
    if (!Dialo成使e輸入isto本y.Contains(Dialo成使eID))
    {
        Dialo成使e輸入isto本y.Add(Dialo成使eID);
        
        // 限制歷史記錄數量
        if (Dialo成使e輸入isto本y.的使設置() > 100)
        {
            Dialo成使e輸入isto本y.Re設置o正eAt(0);
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("對話已添加到歷史：%s"), *Dialo成使eID);
    }
}

正oid UMin成GoRTSDialo成使eSyste設置::Clea本Dialo成使e輸入isto本y()
{
    Dialo成使e輸入isto本y.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("對話歷史已清空"));
}

TA本本ay<軍Dialo成使eO使tco設置e> UMin成GoRTSDialo成使eSyste設置::Calc使lateDialo成使eO使tco設置es(const 軍St本in成& Dialo成使eID, int32 OptionIndex) const
{
    TA本本ay<軍Dialo成使eO使tco設置e> O使tco設置es;
    
    if (Dialo成使eMap.Contains(Dialo成使eID))
    {
        const 軍Min成Dialo成使e& Dialo成使e = Dialo成使eMap[Dialo成使eID];
        
        // 添加自動結果
        O使tco設置es.Append(Dialo成使e.A使toO使tco設置es);
        
        // 添加選項結果
        if (OptionIndex >= 0 && OptionIndex < Dialo成使e.Dialo成使eOptions.的使設置())
        {
            O使tco設置es.Append(Dialo成使e.Dialo成使eOptions[OptionIndex].O使tco設置es);
        }
    }
    
    本et使本n O使tco設置es;
}

正oid UMin成GoRTSDialo成使eSyste設置::ApplyDialo成使eO使tco設置es(const TA本本ay<軍Dialo成使eO使tco設置e>& O使tco設置es)
{
    fo本 (const 軍Dialo成使eO使tco設置e& O使tco設置e : O使tco設置es)
    {
        switch (O使tco設置e.O使tco設置eType)
        {
        case EDialo成使eO使tco設置e::RelationshipChan成e:
            UpdateCha本acte本Relationship(O使tco設置e.Ta本成etID, O使tco設置e.Val使e);
            b本eak;
        case EDialo成使eO使tco設置e::Q使estT本i成成e本:
            T本i成成e本Q使est(O使tco設置e.Ta本成etID);
            b本eak;
        case EDialo成使eO使tco設置e::Info本設置ationGain:
            G本antInfo本設置ation(O使tco設置e.Ta本成etID);
            b本eak;
        case EDialo成使eO使tco設置e::SkillGain:
            I設置p本o正eSkill(軍的a設置e(*O使tco設置e.Ta本成etID), O使tco設置e.Val使e);
            b本eak;
        case EDialo成使eO使tco設置e::E正entT本i成成e本:
            T本i成成e本E正ent(O使tco設置e.Ta本成etID);
            b本eak;
        defa使lt:
            b本eak;
        }
    }
}

bool UMin成GoRTSDialo成使eSyste設置::Sa正eDialo成使eData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現對話數據保存
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存對話數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSDialo成使eSyste設置::LoadDialo成使eData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現對話數據載入
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入對話數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTSDialo成使eSyste設置::InitializeDialo成使eLib本a本y()
{
    C本eate輸入isto本icalCha本acte本Dialo成使es();
    C本eateMilita本yDialo成使es();
    C本eateDiplo設置aticDialo成使es();
    C本eatePe本sonalSto本yDialo成使es();
    C本eateQ使estDialo成使es();
}

正oid UMin成GoRTSDialo成使eSyste設置::C本eate輸入isto本icalCha本acte本Dialo成使es()
{
    // 蔣介石對話
    軍Min成Dialo成使e Chian成Dialo成使e;
    Chian成Dialo成使e.Dialo成使eID = Gene本ateDialo成使eID(TEXT("Chian成KaiShek下軍i本stMeetin成"), TEXT("Chian成KaiShek"));
    Chian成Dialo成使e.Speake本ID = TEXT("Chian成KaiShek");
    Chian成Dialo成使e.Dialo成使eText = TEXT("年輕人，我看你氣宇不凡，想必是黃埔軍校的精英。當前國家多難，正需要你這樣的人才為國效力。");
    Chian成Dialo成使e.Dialo成使eType = EDialo成使eType::輸入isto本icalE正ent;
    Chian成Dialo成使e.Speake本Po本t本ait = TEXT("/Ga設置e/Po本t本aits/Chian成KaiShek.Chian成KaiShek");
    Chian成Dialo成使e.輸入isto本icalContext = TEXT("的o本the本nExpedition");
    Chian成Dialo成使e.bIsI設置po本tant = t本使e;
    Chian成Dialo成使e.bCanBeRepeated = false;

    // 添加對話選項
    軍Dialo成使eOption Option1;
    Option1.OptionText = TEXT("願為國家統一奉獻力量！");
    Option1.Attit使de = EDialo成使eAttit使de::Respectf使l;
    Option1.的extDialo成使eID = Gene本ateDialo成使eID(TEXT("Chian成KaiShek下LoyalResponse"), TEXT("Chian成KaiShek"));
    
    軍Dialo成使eO使tco設置e O使tco設置e1;
    O使tco設置e1.O使tco設置eType = EDialo成使eO使tco設置e::RelationshipChan成e;
    O使tco設置e1.Ta本成etID = TEXT("Chian成KaiShek");
    O使tco設置e1.Val使e = 20.0f;
    O使tco設置e1.Desc本iption = TEXT("獲得蔣介石信任");
    Option1.O使tco設置es.Add(O使tco設置e1);
    
    Chian成Dialo成使e.Dialo成使eOptions.Add(Option1);

    軍Dialo成使eOption Option2;
    Option2.OptionText = TEXT("我需要考慮一下您的提議。");
    Option2.Attit使de = EDialo成使eAttit使de::的e使t本al;
    Option2.的extDialo成使eID = Gene本ateDialo成使eID(TEXT("Chian成KaiShek下的e使t本alResponse"), TEXT("Chian成KaiShek"));
    
    軍Dialo成使eO使tco設置e O使tco設置e2;
    O使tco設置e2.O使tco設置eType = EDialo成使eO使tco設置e::RelationshipChan成e;
    O使tco設置e2.Ta本成etID = TEXT("Chian成KaiShek");
    O使tco設置e2.Val使e = 5.0f;
    O使tco設置e2.Desc本iption = TEXT("保持中立關係");
    Option2.O使tco設置es.Add(O使tco設置e2);
    
    Chian成Dialo成使e.Dialo成使eOptions.Add(Option2);

    AllDialo成使es.Add(Chian成Dialo成使e);

    // 毛澤東對話
    軍Min成Dialo成使e MaoDialo成使e;
    MaoDialo成使e.Dialo成使eID = Gene本ateDialo成使eID(TEXT("MaoZedon成下軍i本stMeetin成"), TEXT("MaoZedon成"));
    MaoDialo成使e.Speake本ID = TEXT("MaoZedon成");
    MaoDialo成使e.Dialo成使eText = TEXT("同志，我看你心懷天下，關心勞苦大眾。在這個動盪的年代，我們應該為人民的解放事業而奮鬥。");
    MaoDialo成使e.Dialo成使eType = EDialo成使eType::輸入isto本icalE正ent;
    MaoDialo成使e.Speake本Po本t本ait = TEXT("/Ga設置e/Po本t本aits/MaoZedon成.MaoZedon成");
    MaoDialo成使e.輸入isto本icalContext = TEXT("Re正ol使tiona本yMo正e設置ent");
    MaoDialo成使e.bIsI設置po本tant = t本使e;
    MaoDialo成使e.bCanBeRepeated = false;

    軍Dialo成使eOption MaoOption1;
    MaoOption1.OptionText = TEXT("我願意加入革命事業！");
    MaoOption1.Attit使de = EDialo成使eAttit使de::軍本iendly;
    MaoOption1.的extDialo成使eID = Gene本ateDialo成使eID(TEXT("MaoZedon成下Re正ol使tiona本yResponse"), TEXT("MaoZedon成"));
    
    軍Dialo成使eO使tco設置e MaoO使tco設置e1;
    MaoO使tco設置e1.O使tco設置eType = EDialo成使eO使tco設置e::Q使estT本i成成e本;
    MaoO使tco設置e1.Ta本成etID = TEXT("Re正ol使tiona本yQ使est");
    MaoO使tco設置e1.Val使e = 1.0f;
    MaoO使tco設置e1.Desc本iption = TEXT("觸發革命任務線");
    MaoOption1.O使tco設置es.Add(MaoO使tco設置e1);
    
    MaoDialo成使e.Dialo成使eOptions.Add(MaoOption1);

    AllDialo成使es.Add(MaoDialo成使e);
}

正oid UMin成GoRTSDialo成使eSyste設置::C本eateMilita本yDialo成使es()
{
    // 軍事簡報對話
    軍Min成Dialo成使e Milita本yB本iefin成;
    Milita本yB本iefin成.Dialo成使eID = Gene本ateDialo成使eID(TEXT("Milita本yB本iefin成下BattlePlan"), TEXT("Co設置設置ande本"));
    Milita本yB本iefin成.Speake本ID = TEXT("Co設置設置ande本");
    Milita本yB本iefin成.Dialo成使eText = TEXT("將軍，根據最新情報，敵軍在前方10公里處集結。我們必須立即制定作戰計劃。你認為我們應該如何應對？");
    Milita本yB本iefin成.Dialo成使eType = EDialo成使eType::Milita本yB本iefin成;
    Milita本yB本iefin成.Speake本Po本t本ait = TEXT("/Ga設置e/Po本t本aits/Milita本yCo設置設置ande本.Milita本yCo設置設置ande本");

    軍Dialo成使eOption Milita本yOption1;
    Milita本yOption1.OptionText = TEXT("主動擊，先發制人！");
    Milita本yOption1.Attit使de = EDialo成使eAttit使de::輸入ostile;
    Milita本yOption1.的extDialo成使eID = Gene本ateDialo成使eID(TEXT("Milita本yB本iefin成下A成成本essi正ePlan"), TEXT("Co設置設置ande本"));
    
    軍Dialo成使eO使tco設置e Milita本yO使tco設置e1;
    Milita本yO使tco設置e1.O使tco設置eType = EDialo成使eO使tco設置e::SkillGain;
    Milita本yO使tco設置e1.Ta本成etID = TEXT("TacticalCo設置設置and");
    Milita本yO使tco設置e1.Val使e = 10.0f;
    Milita本yO使tco設置e1.Desc本iption = TEXT("戰術指揮技能提升");
    Milita本yOption1.O使tco設置es.Add(Milita本yO使tco設置e1);
    
    Milita本yB本iefin成.Dialo成使eOptions.Add(Milita本yOption1);

    軍Dialo成使eOption Milita本yOption2;
    Milita本yOption2.OptionText = TEXT("固守防線，等待時機。");
    Milita本yOption2.Attit使de = EDialo成使eAttit使de::的e使t本al;
    Milita本yOption2.的extDialo成使eID = Gene本ateDialo成使eID(TEXT("Milita本yB本iefin成下Defensi正ePlan"), TEXT("Co設置設置ande本"));
    
    軍Dialo成使eO使tco設置e Milita本yO使tco設置e2;
    Milita本yO使tco設置e2.O使tco設置eType = EDialo成使eO使tco設置e::SkillGain;
    Milita本yO使tco設置e2.Ta本成etID = TEXT("Leade本ship");
    Milita本yO使tco設置e2.Val使e = 8.0f;
    Milita本yO使tco設置e2.Desc本iption = TEXT("領導力技能提升");
    Milita本yOption2.O使tco設置es.Add(Milita本yO使tco設置e2);
    
    Milita本yB本iefin成.Dialo成使eOptions.Add(Milita本yOption2);

    AllDialo成使es.Add(Milita本yB本iefin成);
}

正oid UMin成GoRTSDialo成使eSyste設置::C本eateDiplo設置aticDialo成使es()
{
    // 外交談話
    軍Min成Dialo成使e Diplo設置aticTalk;
    Diplo設置aticTalk.Dialo成使eID = Gene本ateDialo成使eID(TEXT("Diplo設置aticTalk下Alliance"), TEXT("Diplo設置at"));
    Diplo設置aticTalk.Speake本ID = TEXT("Diplo設置at");
    Diplo設置aticTalk.Dialo成使eText = TEXT("將軍，我們的代表團前來商討結盟事宜。面對共同的威脅，我們應該團一致，共同對敵。");
    Diplo設置aticTalk.Dialo成使eType = EDialo成使eType::Diplo設置aticTalk;
    Diplo設置aticTalk.Speake本Po本t本ait = TEXT("/Ga設置e/Po本t本aits/Diplo設置at.Diplo設置at");

    軍Dialo成使eOption Diplo設置aticOption1;
    Diplo設置aticOption1.OptionText = TEXT("我們很樂意與貴方結盟。");
    Diplo設置aticOption1.Attit使de = EDialo成使eAttit使de::軍本iendly;
    Diplo設置aticOption1.的extDialo成使eID = Gene本ateDialo成使eID(TEXT("Diplo設置aticTalk下AllianceAccepted"), TEXT("Diplo設置at"));
    
    軍Dialo成使eO使tco設置e Diplo設置aticO使tco設置e1;
    Diplo設置aticO使tco設置e1.O使tco設置eType = EDialo成使eO使tco設置e::E正entT本i成成e本;
    Diplo設置aticO使tco設置e1.Ta本成etID = TEXT("Alliance軍o本設置ed");
    Diplo設置aticO使tco設置e1.Val使e = 1.0f;
    Diplo設置aticO使tco設置e1.Desc本iption = TEXT("形成聯盟");
    Diplo設置aticOption1.O使tco設置es.Add(Diplo設置aticO使tco設置e1);
    
    Diplo設置aticTalk.Dialo成使eOptions.Add(Diplo設置aticOption1);

    AllDialo成使es.Add(Diplo設置aticTalk);
}

正oid UMin成GoRTSDialo成使eSyste設置::C本eatePe本sonalSto本yDialo成使es()
{
    // 個人故事對話
    軍Min成Dialo成使e Pe本sonalSto本y;
    Pe本sonalSto本y.Dialo成使eID = Gene本ateDialo成使eID(TEXT("Pe本sonalSto本y下軍a設置ily"), TEXT("軍a設置ilyMe設置be本"));
    Pe本sonalSto本y.Speake本ID = TEXT("軍a設置ilyMe設置be本");
    Pe本sonalSto本y.Dialo成使eText = TEXT("孩子，你在軍中的生活還好嗎？家鄉的親人們都很想念你。希望你平安歸來。");
    Pe本sonalSto本y.Dialo成使eType = EDialo成使eType::Pe本sonalSto本y;
    Pe本sonalSto本y.Speake本Po本t本ait = TEXT("/Ga設置e/Po本t本aits/軍a設置ilyMe設置be本.軍a設置ilyMe設置be本");

    軍Dialo成使eOption Pe本sonalOption1;
    Pe本sonalOption1.OptionText = TEXT("我很好，請家人放心。");
    Pe本sonalOption1.Attit使de = EDialo成使eAttit使de::軍本iendly;
    Pe本sonalOption1.的extDialo成使eID = Gene本ateDialo成使eID(TEXT("Pe本sonalSto本y下Reass使本in成"), TEXT("軍a設置ilyMe設置be本"));
    
    軍Dialo成使eO使tco設置e Pe本sonalO使tco設置e1;
    Pe本sonalO使tco設置e1.O使tco設置eType = EDialo成使eO使tco設置e::RelationshipChan成e;
    Pe本sonalO使tco設置e1.Ta本成etID = TEXT("軍a設置ily");
    Pe本sonalO使tco設置e1.Val使e = 15.0f;
    Pe本sonalO使tco設置e1.Desc本iption = TEXT("家庭關係改善");
    Pe本sonalOption1.O使tco設置es.Add(Pe本sonalO使tco設置e1);
    
    Pe本sonalSto本y.Dialo成使eOptions.Add(Pe本sonalOption1);

    AllDialo成使es.Add(Pe本sonalSto本y);
}

正oid UMin成GoRTSDialo成使eSyste設置::C本eateQ使estDialo成使es()
{
    // 任務對話
    軍Min成Dialo成使e Q使estDialo成使e;
    Q使estDialo成使e.Dialo成使eID = Gene本ateDialo成使eID(TEXT("Q使est下Resc使eMission"), TEXT("Q使estGi正e本"));
    Q使estDialo成使e.Speake本ID = TEXT("Q使estGi正e本");
    Q使estDialo成使e.Dialo成使eText = TEXT("將軍，我們的村莊遭到了土匪的襲擊，許多村民被挾為人質。懇請您手相救！");
    Q使estDialo成使e.Dialo成使eType = EDialo成使eType::Q使estDialo成使e;
    Q使estDialo成使e.Speake本Po本t本ait = TEXT("/Ga設置e/Po本t本aits/Villa成e本.Villa成e本");

    軍Dialo成使eOption Q使estOption1;
    Q使estOption1.OptionText = TEXT("我會立即派人營救！");
    Q使estOption1.Attit使de = EDialo成使eAttit使de::軍本iendly;
    Q使estOption1.的extDialo成使eID = Gene本ateDialo成使eID(TEXT("Q使est下Resc使eMission下Accepted"), TEXT("Q使estGi正e本"));
    
    軍Dialo成使eO使tco設置e Q使estO使tco設置e1;
    Q使estO使tco設置e1.O使tco設置eType = EDialo成使eO使tco設置e::Q使estT本i成成e本;
    Q使estO使tco設置e1.Ta本成etID = TEXT("Resc使eMission");
    Q使estO使tco設置e1.Val使e = 1.0f;
    Q使estO使tco設置e1.Desc本iption = TEXT("觸發營救任務");
    Q使estOption1.O使tco設置es.Add(Q使estO使tco設置e1);
    
    Q使estDialo成使e.Dialo成使eOptions.Add(Q使estOption1);

    AllDialo成使es.Add(Q使estDialo成使e);
}

bool UMin成GoRTSDialo成使eSyste設置::ValidateDialo成使eConditions(const TA本本ay<軍Dialo成使eCondition>& Conditions, const 軍Min成Cha本acte本Data& Playe本Cha本acte本) const
{
    fo本 (const 軍Dialo成使eCondition& Condition : Conditions)
    {
        if (!ValidateSin成leCondition(Condition, Playe本Cha本acte本))
        {
            本et使本n false;
        }
    }
    本et使本n t本使e;
}

bool UMin成GoRTSDialo成使eSyste設置::ValidateSin成leCondition(const 軍Dialo成使eCondition& Condition, const 軍Min成Cha本acte本Data& Playe本Cha本acte本) const
{
    if (Condition.ConditionType == TEXT("Att本ib使te"))
    {
        float Att本ib使teVal使e = GetPlaye本Att本ib使te(Condition.Pa本a設置ete本);
        float Req使i本edVal使e = 軍CSt本in成::Atof(*Condition.Val使e);
        
        if (Condition.Ope本ato本 == TEXT(">="))
        {
            本et使本n Att本ib使teVal使e >= Req使i本edVal使e;
        }
        else if (Condition.Ope本ato本 == TEXT("<="))
        {
            本et使本n Att本ib使teVal使e <= Req使i本edVal使e;
        }
        else if (Condition.Ope本ato本 == TEXT("=="))
        {
            本et使本n Att本ib使teVal使e == Req使i本edVal使e;
        }
        else if (Condition.Ope本ato本 == TEXT(">"))
        {
            本et使本n Att本ib使teVal使e > Req使i本edVal使e;
        }
        else if (Condition.Ope本ato本 == TEXT("<"))
        {
            本et使本n Att本ib使teVal使e < Req使i本edVal使e;
        }
    }
    else if (Condition.ConditionType == TEXT("Relationship"))
    {
        float RelationshipVal使e = GetCha本acte本Relationship(Condition.Pa本a設置ete本);
        float Req使i本edVal使e = 軍CSt本in成::Atof(*Condition.Val使e);
        
        if (Condition.Ope本ato本 == TEXT(">="))
        {
            本et使本n RelationshipVal使e >= Req使i本edVal使e;
        }
    }
    else if (Condition.ConditionType == TEXT("SkillLe正el"))
    {
        int32 SkillLe正el = GetSkillLe正el(軍的a設置e(*Condition.Pa本a設置ete本));
        int32 Req使i本edLe正el = 軍CSt本in成::Atoi(*Condition.Val使e);
        
        if (Condition.Ope本ato本 == TEXT(">="))
        {
            本et使本n SkillLe正el >= Req使i本edLe正el;
        }
    }
    else if (Condition.ConditionType == TEXT("Q使estCo設置pleted"))
    {
        bool bQ使estCo設置pleted = IsQ使estCo設置pleted(Condition.Pa本a設置ete本);
        bool bReq使i本ed = Condition.Val使e.ToBool();
        
        本et使本n bQ使estCo設置pleted == bReq使i本ed;
    }
    else if (Condition.ConditionType == TEXT("Cha本acte本Known"))
    {
        bool bCha本acte本Known = IsCha本acte本Known(Condition.Pa本a設置ete本);
        bool bReq使i本ed = Condition.Val使e.ToBool();
        
        本et使本n bCha本acte本Known == bReq使i本ed;
    }

    本et使本n t本使e; // 默認通過未知條件
}

正oid UMin成GoRTSDialo成使eSyste設置::Exec使teDialo成使eO使tco設置es(const TA本本ay<軍Dialo成使eO使tco設置e>& O使tco設置es)
{
    ApplyDialo成使eO使tco設置es(O使tco設置es);
}

正oid UMin成GoRTSDialo成使eSyste設置::UpdateCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, float RelationshipChan成e)
{
    // TODO: 實現角色關係更新
    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新角色 %s 關係：%f"), *Cha本acte本ID, RelationshipChan成e);
}

正oid UMin成GoRTSDialo成使eSyste設置::T本i成成e本Q使est(const 軍St本in成& Q使estID)
{
    // TODO: 實現任務觸發
    UE下LOG(Lo成Te設置p, Lo成, TEXT("觸發任務：%s"), *Q使estID);
}

正oid UMin成GoRTSDialo成使eSyste設置::G本antInfo本設置ation(const 軍St本in成& Info本設置ationID)
{
    // TODO: 實現情報給予
    UE下LOG(Lo成Te設置p, Lo成, TEXT("給予情報：%s"), *Info本設置ationID);
}

正oid UMin成GoRTSDialo成使eSyste設置::I設置p本o正eSkill(const 軍的a設置e& SkillID, float I設置p本o正e設置entA設置o使nt)
{
    // TODO: 實現技能提升
    UE下LOG(Lo成Te設置p, Lo成, TEXT("提升技能 %s：%f"), *SkillID.ToSt本in成(), I設置p本o正e設置entA設置o使nt);
}

正oid UMin成GoRTSDialo成使eSyste設置::T本i成成e本E正ent(const 軍St本in成& E正entID)
{
    // TODO: 實現事件觸發
    UE下LOG(Lo成Te設置p, Lo成, TEXT("觸發事件：%s"), *E正entID);
}

軍St本in成 UMin成GoRTSDialo成使eSyste設置::Gene本ateDialo成使eID(const 軍St本in成& Base的a設置e, const 軍St本in成& Cha本acte本ID) const
{
    本et使本n 軍St本in成::P本intf(TEXT("%s下%s"), *Cha本acte本ID, *Base的a設置e);
}

float UMin成GoRTSDialo成使eSyste設置::GetCha本acte本Relationship(const 軍St本in成& Cha本acte本ID) const
{
    // TODO: 實現角色關係獲取
    本et使本n 50.0f; // 默認中立關係
}

bool UMin成GoRTSDialo成使eSyste設置::IsCha本acte本Known(const 軍St本in成& Cha本acte本ID) const
{
    // TODO: 實現角色已知狀態檢查
    本et使本n Dialo成使e輸入isto本y.的使設置() > 0; // 簡化實現
}

float UMin成GoRTSDialo成使eSyste設置::GetPlaye本Att本ib使te(const 軍St本in成& Att本ib使te的a設置e) const
{
    if (Att本ib使te的a設置e == TEXT("Leade本ship"))
    {
        本et使本n C使本本entPlaye本Cha本acte本.Att本ib使tes.Leade本ship;
    }
    else if (Att本ib使te的a設置e == TEXT("Intelli成ence"))
    {
        本et使本n C使本本entPlaye本Cha本acte本.Att本ib使tes.Intelli成ence;
    }
    else if (Att本ib使te的a設置e == TEXT("Co使本a成e"))
    {
        本et使本n C使本本entPlaye本Cha本acte本.Att本ib使tes.Co使本a成e;
    }
    else if (Att本ib使te的a設置e == TEXT("Cha本is設置a"))
    {
        本et使本n C使本本entPlaye本Cha本acte本.Att本ib使tes.Cha本is設置a;
    }
    else if (Att本ib使te的a設置e == TEXT("Constit使tion"))
    {
        本et使本n C使本本entPlaye本Cha本acte本.Att本ib使tes.Constit使tion;
    }
    
    本et使本n 50.0f; // 默認值
}

bool UMin成GoRTSDialo成使eSyste設置::IsQ使estCo設置pleted(const 軍St本in成& Q使estID) const
{
    // TODO: 實現任務狀態檢查
    本et使本n false; // 默認未完成
}

int32 UMin成GoRTSDialo成使eSyste設置::GetSkillLe正el(const 軍的a設置e& SkillID) const
{
    // TODO: 實現技能等級獲取
    本et使本n 1; // 默認等級
}
