#incl使de "Min成GoRTS輸入isto本icalCha本acte本s.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTS輸入isto本icalCha本acte本s::UMin成GoRTS輸入isto本icalCha本acte本s()
{
    bIsInitialized = false;
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::InitializeCha本acte本Syste設置()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("歷史人物系統已經初始化"));
        本et使本n;
    }

    // 初始化歷史人物庫
    Initialize輸入isto本icalCha本acte本Lib本a本y();

    // 構建角色映射
    Cha本acte本Map.E設置pty();
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : AllCha本acte本s)
    {
        Cha本acte本Map.Add(Cha本acte本.Cha本acte本ID, Cha本acte本);
        
        // 初始化角色關係
        if (!Cha本acte本Relationships.Contains(Cha本acte本.Cha本acte本ID))
        {
            Cha本acte本Relationships.Add(Cha本acte本.Cha本acte本ID, 50.0f); // 默認中立
        }
        
        // 初始化角色可用性
        if (!Cha本acte本A正ailability.Contains(Cha本acte本.Cha本acte本ID))
        {
            Cha本acte本A正ailability.Add(Cha本acte本.Cha本acte本ID, Cha本acte本.bIsA正ailable);
        }
    }

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("歷史人物系統初始化完成，共載入 %d 個人物"), AllCha本acte本s.的使設置());
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成GoRTS輸入isto本icalCha本acte本s::GetAllCha本acte本s() const
{
    本et使本n AllCha本acte本s;
}

軍Min成輸入isto本icalCha本acte本 UMin成GoRTS輸入isto本icalCha本acte本s::GetCha本acte本(const 軍St本in成& Cha本acte本ID) const
{
    if (const 軍Min成輸入isto本icalCha本acte本* Cha本acte本 = Cha本acte本Map.軍ind(Cha本acte本ID))
    {
        本et使本n *Cha本acte本;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到歷史人物 ID：%s"), *Cha本acte本ID);
    本et使本n 軍Min成輸入isto本icalCha本acte本();
}

float UMin成GoRTS輸入isto本icalCha本acte本s::GetCha本acte本Relationship(const 軍St本in成& Cha本acte本ID) const
{
    if (const float* Relationship = Cha本acte本Relationships.軍ind(Cha本acte本ID))
    {
        本et使本n *Relationship;
    }
    
    本et使本n 50.0f; // 默認中立關係
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::UpdateCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, float RelationshipChan成e)
{
    float C使本本entRelationship = GetCha本acte本Relationship(Cha本acte本ID);
    float 的ewRelationship = 軍Math::Cla設置p(C使本本entRelationship + RelationshipChan成e, 0.0f, 100.0f);
    
    Cha本acte本Relationships.Add(Cha本acte本ID, 的ewRelationship);
    
    // 廣播關係變化事件
    OnCha本acte本RelationshipChan成ed.B本oadcast(Cha本acte本ID, 的ewRelationship);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色 %s 關係更新：%.1f -> %.1f"), *Cha本acte本ID, C使本本entRelationship, 的ewRelationship);
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成GoRTS輸入isto本icalCha本acte本s::GetA正ailableCha本acte本s() const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> A正ailableCha本acte本s;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : AllCha本acte本s)
    {
        if (Cha本acte本.bIsA正ailable && Cha本acte本.bIsAli正e)
        {
            A正ailableCha本acte本s.Add(Cha本acte本);
        }
    }
    
    本et使本n A正ailableCha本acte本s;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成GoRTS輸入isto本icalCha本acte本s::GetCha本acte本sBy軍action(E輸入isto本ical軍action 軍action) const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> 軍actionCha本acte本s;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : AllCha本acte本s)
    {
        if (Cha本acte本.軍action == 軍action && Cha本acte本.bIsA正ailable)
        {
            軍actionCha本acte本s.Add(Cha本acte本);
        }
    }
    
    本et使本n 軍actionCha本acte本s;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成GoRTS輸入isto本icalCha本acte本s::GetCha本acte本sByRole(ECha本acte本Role Role) const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> RoleCha本acte本s;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : AllCha本acte本s)
    {
        if (Cha本acte本.Role == Role && Cha本acte本.bIsA正ailable)
        {
            RoleCha本acte本s.Add(Cha本acte本);
        }
    }
    
    本et使本n RoleCha本acte本s;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成GoRTS輸入isto本icalCha本acte本s::GetCha本acte本sAtLocation(const 軍St本in成& Location) const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> LocationCha本acte本s;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : AllCha本acte本s)
    {
        if (Cha本acte本.KnownLocations.Contains(Location) && Cha本acte本.bIsA正ailable)
        {
            LocationCha本acte本s.Add(Cha本acte本);
        }
    }
    
    本et使本n LocationCha本acte本s;
}

TA本本ay<軍Min成輸入isto本icalCha本acte本> UMin成GoRTS輸入isto本icalCha本acte本s::GetCha本acte本sInE正ent(const 軍St本in成& E正entID) const
{
    TA本本ay<軍Min成輸入isto本icalCha本acte本> E正entCha本acte本s;
    
    fo本 (const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 : AllCha本acte本s)
    {
        if (Cha本acte本.KeyE正ents.Contains(E正entID) && Cha本acte本.bIsA正ailable)
        {
            E正entCha本acte本s.Add(Cha本acte本);
        }
    }
    
    本et使本n E正entCha本acte本s;
}

bool UMin成GoRTS輸入isto本icalCha本acte本s::IsCha本acte本A正ailable(const 軍St本in成& Cha本acte本ID) const
{
    if (const bool* bA正ailable = Cha本acte本A正ailability.軍ind(Cha本acte本ID))
    {
        本et使本n *bA正ailable;
    }
    
    本et使本n false;
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::SetCha本acte本A正ailability(const 軍St本in成& Cha本acte本ID, bool bIsA正ailable)
{
    Cha本acte本A正ailability.Add(Cha本acte本ID, bIsA正ailable);
    
    // 廣播可用性變化事件
    OnCha本acte本A正ailabilityChan成ed.B本oadcast(Cha本acte本ID, bIsA正ailable);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色 %s 可用性設置為：%s"), *Cha本acte本ID, bIsA正ailable 基本 TEXT("可用") : TEXT("不可用"));
}

TA本本ay<軍St本in成> UMin成GoRTS輸入isto本icalCha本acte本s::GetCha本acte本Dialo成使eOptions(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍St本in成> Dialo成使eOptions;
    
    if (Cha本acte本Map.Contains(Cha本acte本ID))
    {
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Map[Cha本acte本ID];
        Dialo成使eOptions = Cha本acte本.Dialo成使eIDs;
    }
    
    本et使本n Dialo成使eOptions;
}

軍St本in成 UMin成GoRTS輸入isto本icalCha本acte本s::Gene本ateCha本acte本Response(const 軍St本in成& Cha本acte本ID, EDialo成使eAttit使de Playe本Attit使de, const 軍St本in成& Context) const
{
    if (!Cha本acte本Map.Contains(Cha本acte本ID))
    {
        本et使本n TEXT("我無法理解你的意思。");
    }
    
    const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Map[Cha本acte本ID];
    本et使本n Gene本ateResponseText(Cha本acte本, Playe本Attit使de, Context);
}

float UMin成GoRTS輸入isto本icalCha本acte本s::Calc使lateCha本acte本Infl使ence(const 軍St本in成& Cha本acte本ID) const
{
    if (!Cha本acte本Map.Contains(Cha本acte本ID))
    {
        本et使本n 0.0f;
    }
    
    const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Map[Cha本acte本ID];
    
    // 基礎影響力
    float BaseInfl使ence = Cha本acte本.Infl使ence;
    
    // 角色關係加成
    float RelationshipBon使s = (GetCha本acte本Relationship(Cha本acte本ID) - 50.0f) * 0.2f;
    
    // 角色重要性加成
    float RoleBon使s = 0.0f;
    switch (Cha本acte本.Role)
    {
    case ECha本acte本Role::Leade本:
        RoleBon使s = 20.0f;
        b本eak;
    case ECha本acte本Role::Milita本y:
        RoleBon使s = 15.0f;
        b本eak;
    case ECha本acte本Role::Political:
        RoleBon使s = 12.0f;
        b本eak;
    case ECha本acte本Role::Diplo設置atic:
        RoleBon使s = 10.0f;
        b本eak;
    defa使lt:
        RoleBon使s = 5.0f;
        b本eak;
    }
    
    本et使本n 軍Math::Cla設置p(BaseInfl使ence + RelationshipBon使s + RoleBon使s, 0.0f, 100.0f);
}

TA本本ay<軍St本in成> UMin成GoRTS輸入isto本icalCha本acte本s::GetReco設置設置endedDialo成使es(const 軍St本in成& Cha本acte本ID, const 軍Min成Cha本acte本Data& Playe本Cha本acte本) const
{
    TA本本ay<軍St本in成> Reco設置設置endedDialo成使es;
    
    if (!Cha本acte本Map.Contains(Cha本acte本ID))
    {
        本et使本n Reco設置設置endedDialo成使es;
    }
    
    const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Map[Cha本acte本ID];
    
    // 根據角色關係和玩家屬性推薦對話
    float Relationship = GetCha本acte本Relationship(Cha本acte本ID);
    
    // 高關係值推薦友好對話
    if (Relationship > 70.0f)
    {
        fo本 (const 軍St本in成& Dialo成使eID : Cha本acte本.Dialo成使eIDs)
        {
            if (Dialo成使eID.Contains(TEXT("軍本iendly"))  Dialo成使eID.Contains(TEXT("Loyal")))
            {
                Reco設置設置endedDialo成使es.Add(Dialo成使eID);
            }
        }
    }
    // 低關係值推薦中立的對話
    else if (Relationship < 30.0f)
    {
        fo本 (const 軍St本in成& Dialo成使eID : Cha本acte本.Dialo成使eIDs)
        {
            if (Dialo成使eID.Contains(TEXT("的e使t本al"))  Dialo成使eID.Contains(TEXT("軍o本設置al")))
            {
                Reco設置設置endedDialo成使es.Add(Dialo成使eID);
            }
        }
    }
    // 中等關係值推薦常規對話
    else
    {
        Reco設置設置endedDialo成使es = Cha本acte本.Dialo成使eIDs;
    }
    
    本et使本n Reco設置設置endedDialo成使es;
}

bool UMin成GoRTS輸入isto本icalCha本acte本s::Sa正eCha本acte本Data(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現角色數據保存
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存角色數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTS輸入isto本icalCha本acte本s::LoadCha本acte本Data(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現角色數據載入
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入角色數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::Initialize輸入isto本icalCha本acte本Lib本a本y()
{
    C本eate的ationalistCha本acte本s();
    C本eateCo設置設置使nistCha本acte本s();
    C本eate基本a本lo本dCha本acte本s();
    C本eateIndependentCha本acte本s();
    C本eate軍o本ei成nCha本acte本s();
    C本eateCi正ilianCha本acte本s();
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::C本eate的ationalistCha本acte本s()
{
    // 蔣介石
    軍Min成輸入isto本icalCha本acte本 Chian成KaiShek;
    Chian成KaiShek.Cha本acte本ID = Gene本ateCha本acte本ID(TEXT("Chian成KaiShek"));
    Chian成KaiShek.Cha本acte本的a設置e = TEXT("蔣介石");
    Chian成KaiShek.Co使本tesy的a設置e = TEXT("介石");
    Chian成KaiShek.Bi本thDate = TEXT("1887-10-31");
    Chian成KaiShek.DeathDate = TEXT("1975-04-05");
    Chian成KaiShek.Role = ECha本acte本Role::Leade本;
    Chian成KaiShek.軍action = E輸入isto本ical軍action::的ationalist;
    Chian成KaiShek.Title = TEXT("國民政府主席");
    Chian成KaiShek.Desc本iption = TEXT("中華民國國民黨總裁，領導北伐和抗戰的重要人物");
    Chian成KaiShek.Po本t本aitPath = TEXT("/Ga設置e/Po本t本aits/Chian成KaiShek.Chian成KaiShek");
    Chian成KaiShek.VoiceType = TEXT("A使tho本itati正e");
    Chian成KaiShek.bIsAli正e = t本使e;
    Chian成KaiShek.bIsA正ailable = t本使e;
    Chian成KaiShek.Infl使ence = 95.0f;
    
    // 設置性格特徵
    Chian成KaiShek.Pe本sonality.Pat本iotis設置 = 0.9f;
    Chian成KaiShek.Pe本sonality.A設置bition = 0.8f;
    Chian成KaiShek.Pe本sonality.Conscientio使sness = 0.7f;
    Chian成KaiShek.Pe本sonality.Ext本a正e本sion = 0.6f;
    
    // 設置已知地點
    Chian成KaiShek.KnownLocations.Add(TEXT("的an大in成"));
    Chian成KaiShek.KnownLocations.Add(TEXT("Chon成qin成"));
    Chian成KaiShek.KnownLocations.Add(TEXT("Taipei"));
    
    // 設置關鍵事件
    Chian成KaiShek.KeyE正ents.Add(TEXT("的o本the本nExpedition"));
    Chian成KaiShek.KeyE正ents.Add(TEXT("SecondSinoJapanese基本a本"));
    Chian成KaiShek.KeyE正ents.Add(TEXT("ChineseCi正il基本a本"));
    
    // 設置對話ID
    Chian成KaiShek.Dialo成使eIDs.Add(TEXT("Chian成KaiShek下軍i本stMeetin成"));
    Chian成KaiShek.Dialo成使eIDs.Add(TEXT("Chian成KaiShek下Milita本yB本iefin成"));
    Chian成KaiShek.Dialo成使eIDs.Add(TEXT("Chian成KaiShek下PoliticalDisc使ssion"));
    
    AllCha本acte本s.Add(Chian成KaiShek);

    // 宋美齡
    軍Min成輸入isto本icalCha本acte本 Soon成MeiLin成;
    Soon成MeiLin成.Cha本acte本ID = Gene本ateCha本acte本ID(TEXT("Soon成MeiLin成"));
    Soon成MeiLin成.Cha本acte本的a設置e = TEXT("宋美齡");
    Soon成MeiLin成.Co使本tesy的a設置e = TEXT("美齡");
    Soon成MeiLin成.Bi本thDate = TEXT("1898-03-05");
    Soon成MeiLin成.DeathDate = TEXT("2003-10-23");
    Soon成MeiLin成.Role = ECha本acte本Role::Diplo設置atic;
    Soon成MeiLin成.軍action = E輸入isto本ical軍action::的ationalist;
    Soon成MeiLin成.Title = TEXT("第一夫人");
    Soon成MeiLin成.Desc本iption = TEXT("蔣介石夫人，著名外交家，在國際舞台上為中國發聲");
    Soon成MeiLin成.Po本t本aitPath = TEXT("/Ga設置e/Po本t本aits/Soon成MeiLin成.Soon成MeiLin成");
    Soon成MeiLin成.VoiceType = TEXT("Ele成ant");
    Soon成MeiLin成.bIsAli正e = t本使e;
    Soon成MeiLin成.bIsA正ailable = t本使e;
    Soon成MeiLin成.Infl使ence = 80.0f;
    
    Soon成MeiLin成.Pe本sonality.Ext本a正e本sion = 0.8f;
    Soon成MeiLin成.Pe本sonality.A成本eeableness = 0.7f;
    Soon成MeiLin成.Pe本sonality.Openness = 0.9f;
    
    Soon成MeiLin成.KnownLocations.Add(TEXT("的an大in成"));
    Soon成MeiLin成.KnownLocations.Add(TEXT("基本ashin成ton"));
    Soon成MeiLin成.KnownLocations.Add(TEXT("的ewYo本k"));
    
    Soon成MeiLin成.KeyE正ents.Add(TEXT("Diplo設置aticMission"));
    Soon成MeiLin成.KeyE正ents.Add(TEXT("Inte本nationalConfe本ence"));
    
    Soon成MeiLin成.Dialo成使eIDs.Add(TEXT("Soon成MeiLin成下Diplo設置aticTalk"));
    Soon成MeiLin成.Dialo成使eIDs.Add(TEXT("Soon成MeiLin成下C使lt使本alExchan成e"));
    
    AllCha本acte本s.Add(Soon成MeiLin成);
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::C本eateCo設置設置使nistCha本acte本s()
{
    // 毛澤東
    軍Min成輸入isto本icalCha本acte本 MaoZedon成;
    MaoZedon成.Cha本acte本ID = Gene本ateCha本acte本ID(TEXT("MaoZedon成"));
    MaoZedon成.Cha本acte本的a設置e = TEXT("毛澤東");
    MaoZedon成.Co使本tesy的a設置e = TEXT("潤之");
    MaoZedon成.Bi本thDate = TEXT("1893-12-26");
    MaoZedon成.DeathDate = TEXT("1976-09-09");
    MaoZedon成.Role = ECha本acte本Role::Leade本;
    MaoZedon成.軍action = E輸入isto本ical軍action::Co設置設置使nist;
    MaoZedon成.Title = TEXT("中國共產黨主席");
    MaoZedon成.Desc本iption = TEXT("中國共產黨創始人之一，領導中國革命的重要人物");
    MaoZedon成.Po本t本aitPath = TEXT("/Ga設置e/Po本t本aits/MaoZedon成.MaoZedon成");
    MaoZedon成.VoiceType = TEXT("Re正ol使tiona本y");
    MaoZedon成.bIsAli正e = t本使e;
    MaoZedon成.bIsA正ailable = t本使e;
    MaoZedon成.Infl使ence = 90.0f;
    
    MaoZedon成.Pe本sonality.Pat本iotis設置 = 0.9f;
    MaoZedon成.Pe本sonality.A設置bition = 0.8f;
    MaoZedon成.Pe本sonality.Openness = 0.7f;
    MaoZedon成.Pe本sonality.Ext本a正e本sion = 0.6f;
    
    MaoZedon成.KnownLocations.Add(TEXT("Yan'an"));
    MaoZedon成.KnownLocations.Add(TEXT("Bei大in成"));
    MaoZedon成.KnownLocations.Add(TEXT("Shan成hai"));
    
    MaoZedon成.KeyE正ents.Add(TEXT("Lon成Ma本ch"));
    MaoZedon成.KeyE正ents.Add(TEXT("ChineseCi正il基本a本"));
    MaoZedon成.KeyE正ents.Add(TEXT("C使lt使本alRe正ol使tion"));
    
    MaoZedon成.Dialo成使eIDs.Add(TEXT("MaoZedon成下軍i本stMeetin成"));
    MaoZedon成.Dialo成使eIDs.Add(TEXT("MaoZedon成下Re正ol使tiona本yTalk"));
    MaoZedon成.Dialo成使eIDs.Add(TEXT("MaoZedon成下PhilosophicalDisc使ssion"));
    
    AllCha本acte本s.Add(MaoZedon成);

    // 周恩來
    軍Min成輸入isto本icalCha本acte本 Zho使Enlai;
    Zho使Enlai.Cha本acte本ID = Gene本ateCha本acte本ID(TEXT("Zho使Enlai"));
    Zho使Enlai.Cha本acte本的a設置e = TEXT("周恩來");
    Zho使Enlai.Co使本tesy的a設置e = TEXT("翔宇");
    Zho使Enlai.Bi本thDate = TEXT("1898-03-05");
    Zho使Enlai.DeathDate = TEXT("1976-01-08");
    Zho使Enlai.Role = ECha本acte本Role::Diplo設置atic;
    Zho使Enlai.軍action = E輸入isto本ical軍action::Co設置設置使nist;
    Zho使Enlai.Title = TEXT("國務院總理");
    Zho使Enlai.Desc本iption = TEXT("中國共產黨重要領導人，著名外交家");
    Zho使Enlai.Po本t本aitPath = TEXT("/Ga設置e/Po本t本aits/Zho使Enlai.Zho使Enlai");
    Zho使Enlai.VoiceType = TEXT("Diplo設置atic");
    Zho使Enlai.bIsAli正e = t本使e;
    Zho使Enlai.bIsA正ailable = t本使e;
    Zho使Enlai.Infl使ence = 85.0f;
    
    Zho使Enlai.Pe本sonality.A成本eeableness = 0.8f;
    Zho使Enlai.Pe本sonality.Conscientio使sness = 0.9f;
    Zho使Enlai.Pe本sonality.Ext本a正e本sion = 0.7f;
    
    Zho使Enlai.KnownLocations.Add(TEXT("Bei大in成"));
    Zho使Enlai.KnownLocations.Add(TEXT("Gene正a"));
    Zho使Enlai.KnownLocations.Add(TEXT("Band使n成"));
    
    Zho使Enlai.KeyE正ents.Add(TEXT("Gene正aConfe本ence"));
    Zho使Enlai.KeyE正ents.Add(TEXT("Band使n成Confe本ence"));
    
    Zho使Enlai.Dialo成使eIDs.Add(TEXT("Zho使Enlai下Diplo設置aticTalk"));
    Zho使Enlai.Dialo成使eIDs.Add(TEXT("Zho使Enlai下PolicyDisc使ssion"));
    
    AllCha本acte本s.Add(Zho使Enlai);
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::C本eate基本a本lo本dCha本acte本s()
{
    // 張作霖
    軍Min成輸入isto本icalCha本acte本 Zhan成Z使olin;
    Zhan成Z使olin.Cha本acte本ID = Gene本ateCha本acte本ID(TEXT("Zhan成Z使olin"));
    Zhan成Z使olin.Cha本acte本的a設置e = TEXT("張作霖");
    Zhan成Z使olin.Co使本tesy的a設置e = TEXT("雨亭");
    Zhan成Z使olin.Bi本thDate = TEXT("1875-03-19");
    Zhan成Z使olin.DeathDate = TEXT("1928-06-04");
    Zhan成Z使olin.Role = ECha本acte本Role::Milita本y;
    Zhan成Z使olin.軍action = E輸入isto本ical軍action::基本a本lo本d;
    Zhan成Z使olin.Title = TEXT("東三省巡閱使");
    Zhan成Z使olin.Desc本iption = TEXT("奉系軍閥首領，控制東北地區");
    Zhan成Z使olin.Po本t本aitPath = TEXT("/Ga設置e/Po本t本aits/Zhan成Z使olin.Zhan成Z使olin");
    Zhan成Z使olin.VoiceType = TEXT("A使tho本itati正e");
    Zhan成Z使olin.bIsAli正e = false;
    Zhan成Z使olin.bIsA正ailable = false; // 已去世
    Zhan成Z使olin.Infl使ence = 75.0f;
    
    Zhan成Z使olin.Pe本sonality.A設置bition = 0.8f;
    Zhan成Z使olin.Pe本sonality.Ext本a正e本sion = 0.7f;
    Zhan成Z使olin.Pe本sonality.Conscientio使sness = 0.6f;
    
    Zhan成Z使olin.KnownLocations.Add(TEXT("Shenyan成"));
    Zhan成Z使olin.KnownLocations.Add(TEXT("Bei大in成"));
    
    Zhan成Z使olin.KeyE正ents.Add(TEXT("軍i本stZhili軍en成tian基本a本"));
    Zhan成Z使olin.KeyE正ents.Add(TEXT("輸入使an成成使t使nIncident"));
    
    AllCha本acte本s.Add(Zhan成Z使olin);

    // 張學良
    軍Min成輸入isto本icalCha本acte本 Zhan成X使elian成;
    Zhan成X使elian成.Cha本acte本ID = Gene本ateCha本acte本ID(TEXT("Zhan成X使elian成"));
    Zhan成X使elian成.Cha本acte本的a設置e = TEXT("張學良");
    Zhan成X使elian成.Co使本tesy的a設置e = TEXT("漢卿");
    Zhan成X使elian成.Bi本thDate = TEXT("1901-06-03");
    Zhan成X使elian成.DeathDate = TEXT("2001-10-14");
    Zhan成X使elian成.Role = ECha本acte本Role::Milita本y;
    Zhan成X使elian成.軍action = E輸入isto本ical軍action::基本a本lo本d;
    Zhan成X使elian成.Title = TEXT("東三省邊防司令長官");
    Zhan成X使elian成.Desc本iption = TEXT("張作霖之子，西安事變的重要人物");
    Zhan成X使elian成.Po本t本aitPath = TEXT("/Ga設置e/Po本t本aits/Zhan成X使elian成.Zhan成X使elian成");
    Zhan成X使elian成.VoiceType = TEXT("Yo使n成Milita本y");
    Zhan成X使elian成.bIsAli正e = t本使e;
    Zhan成X使elian成.bIsA正ailable = t本使e;
    Zhan成X使elian成.Infl使ence = 70.0f;
    
    Zhan成X使elian成.Pe本sonality.Pat本iotis設置 = 0.8f;
    Zhan成X使elian成.Pe本sonality.Conscientio使sness = 0.7f;
    Zhan成X使elian成.Pe本sonality.Openness = 0.6f;
    
    Zhan成X使elian成.KnownLocations.Add(TEXT("Shenyan成"));
    Zhan成X使elian成.KnownLocations.Add(TEXT("Xi'an"));
    
    Zhan成X使elian成.KeyE正ents.Add(TEXT("Xi'anIncident"));
    Zhan成X使elian成.KeyE正ents.Add(TEXT("的o本theastIncident"));
    
    Zhan成X使elian成.Dialo成使eIDs.Add(TEXT("Zhan成X使elian成下軍i本stMeetin成"));
    Zhan成X使elian成.Dialo成使eIDs.Add(TEXT("Zhan成X使elian成下Xi'anTalk"));
    
    AllCha本acte本s.Add(Zhan成X使elian成);
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::C本eateIndependentCha本acte本s()
{
    // 魯迅
    軍Min成輸入isto本icalCha本acte本 L使X使n;
    L使X使n.Cha本acte本ID = Gene本ateCha本acte本ID(TEXT("L使X使n"));
    L使X使n.Cha本acte本的a設置e = TEXT("魯迅");
    L使X使n.Co使本tesy的a設置e = TEXT("豫才");
    L使X使n.Bi本thDate = TEXT("1881-09-25");
    L使X使n.DeathDate = TEXT("1936-10-19");
    L使X使n.Role = ECha本acte本Role::C使lt使本al;
    L使X使n.軍action = E輸入isto本ical軍action::Independent;
    L使X使n.Title = TEXT("文學家、思想家");
    L使X使n.Desc本iption = TEXT("中國現代文學奠基人，新文化運動重要人物");
    L使X使n.Po本t本aitPath = TEXT("/Ga設置e/Po本t本aits/L使X使n.L使X使n");
    L使X使n.VoiceType = TEXT("Intellect使al");
    L使X使n.bIsAli正e = false;
    L使X使n.bIsA正ailable = false; // 已去世
    L使X使n.Infl使ence = 65.0f;
    
    L使X使n.Pe本sonality.Openness = 0.9f;
    L使X使n.Pe本sonality.Conscientio使sness = 0.8f;
    L使X使n.Pe本sonality.的e使本oticis設置 = 0.6f;
    
    L使X使n.KnownLocations.Add(TEXT("Bei大in成"));
    L使X使n.KnownLocations.Add(TEXT("Shan成hai"));
    L使X使n.KnownLocations.Add(TEXT("G使an成zho使"));
    
    L使X使n.KeyE正ents.Add(TEXT("的ewC使lt使本eMo正e設置ent"));
    L使X使n.KeyE正ents.Add(TEXT("May軍o使本thMo正e設置ent"));
    
    AllCha本acte本s.Add(L使X使n);
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::C本eate軍o本ei成nCha本acte本s()
{
    // 陳納德
    軍Min成輸入isto本icalCha本acte本 Chenna使lt;
    Chenna使lt.Cha本acte本ID = Gene本ateCha本acte本ID(TEXT("Chenna使lt"));
    Chenna使lt.Cha本acte本的a設置e = TEXT("陳納德");
    Chenna使lt.Bi本thDate = TEXT("1893-09-06");
    Chenna使lt.DeathDate = TEXT("1958-07-27");
    Chenna使lt.Role = ECha本acte本Role::Milita本y;
    Chenna使lt.軍action = E輸入isto本ical軍action::軍o本ei成n;
    Chenna使lt.Title = TEXT("飛虎隊指揮官");
    Chenna使lt.Desc本iption = TEXT("美國飛行員，飛虎隊創始人，協助中國抗戰");
    Chenna使lt.Po本t本aitPath = TEXT("/Ga設置e/Po本t本aits/Chenna使lt.Chenna使lt");
    Chenna使lt.VoiceType = TEXT("A設置e本icanMilita本y");
    Chenna使lt.bIsAli正e = t本使e;
    Chenna使lt.bIsA正ailable = t本使e;
    Chenna使lt.Infl使ence = 60.0f;
    
    Chenna使lt.Pe本sonality.Conscientio使sness = 0.8f;
    Chenna使lt.Pe本sonality.A成本eeableness = 0.7f;
    Chenna使lt.Pe本sonality.Ext本a正e本sion = 0.6f;
    
    Chenna使lt.KnownLocations.Add(TEXT("K使n設置in成"));
    Chenna使lt.KnownLocations.Add(TEXT("Chon成qin成"));
    Chenna使lt.KnownLocations.Add(TEXT("基本ashin成ton"));
    
    Chenna使lt.KeyE正ents.Add(TEXT("軍lyin成Ti成e本s軍o本設置ation"));
    Chenna使lt.KeyE正ents.Add(TEXT("SecondSinoJapanese基本a本"));
    
    Chenna使lt.Dialo成使eIDs.Add(TEXT("Chenna使lt下Milita本yB本iefin成"));
    Chenna使lt.Dialo成使eIDs.Add(TEXT("Chenna使lt下A正iationTalk"));
    
    AllCha本acte本s.Add(Chenna使lt);
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::C本eateCi正ilianCha本acte本s()
{
    // 普通村民
    軍Min成輸入isto本icalCha本acte本 Villa成e本;
    Villa成e本.Cha本acte本ID = Gene本ateCha本acte本ID(TEXT("Villa成e本"));
    Villa成e本.Cha本acte本的a設置e = TEXT("村民");
    Villa成e本.Role = ECha本acte本Role::Ci正ilian;
    Villa成e本.軍action = E輸入isto本ical軍action::Independent;
    Villa成e本.Title = TEXT("普通農民");
    Villa成e本.Desc本iption = TEXT("生活在農村的普通百姓，關心家園和收成");
    Villa成e本.Po本t本aitPath = TEXT("/Ga設置e/Po本t本aits/Villa成e本.Villa成e本");
    Villa成e本.VoiceType = TEXT("R使本al");
    Villa成e本.bIsAli正e = t本使e;
    Villa成e本.bIsA正ailable = t本使e;
    Villa成e本.Infl使ence = 20.0f;
    
    Villa成e本.Pe本sonality.A成本eeableness = 0.7f;
    Villa成e本.Pe本sonality.Conscientio使sness = 0.6f;
    Villa成e本.Pe本sonality.Ext本a正e本sion = 0.5f;
    
    Villa成e本.KnownLocations.Add(TEXT("R使本alVilla成e"));
    Villa成e本.KnownLocations.Add(TEXT("Town"));
    
    Villa成e本.KeyE正ents.Add(TEXT("LocalE正ents"));
    
    Villa成e本.Dialo成使eIDs.Add(TEXT("Villa成e本下DailyTalk"));
    Villa成e本.Dialo成使eIDs.Add(TEXT("Villa成e本下Q使estReq使est"));
    
    AllCha本acte本s.Add(Villa成e本);
}

軍St本in成 UMin成GoRTS輸入isto本icalCha本acte本s::Gene本ateResponseText(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本, EDialo成使eAttit使de Playe本Attit使de, const 軍St本in成& Context) const
{
    軍St本in成 ResponseTe設置plate = GetResponseTe設置plate(Cha本acte本, Playe本Attit使de);
    
    // 根據角色性格和玩家態度生成回應
    if (Playe本Attit使de == EDialo成使eAttit使de::軍本iendly)
    {
        if (Cha本acte本.Pe本sonality.A成本eeableness > 0.7f)
        {
            本et使本n ResponseTe設置plate + TEXT("我很高興能與你交流。");
        }
        else if (Cha本acte本.Pe本sonality.Ext本a正e本sion > 0.6f)
        {
            本et使本n ResponseTe設置plate + TEXT("讓我們暢談一番！");
        }
    }
    else if (Playe本Attit使de == EDialo成使eAttit使de::輸入ostile)
    {
        if (Cha本acte本.Pe本sonality.的e使本oticis設置 > 0.6f)
        {
            本et使本n ResponseTe設置plate + TEXT("你的態度讓我感到不安。");
        }
        else if (Cha本acte本.Pe本sonality.Conscientio使sness > 0.7f)
        {
            本et使本n ResponseTe設置plate + TEXT("我認為我們應該冷靜地討論問題。");
        }
    }
    else if (Playe本Attit使de == EDialo成使eAttit使de::Respectf使l)
    {
        if (Cha本acte本.Role == ECha本acte本Role::Leade本)
        {
            本et使本n ResponseTe設置plate + TEXT("你的敬意我感受到了。");
        }
        else if (Cha本acte本.Pe本sonality.Ext本a正e本sion > 0.5f)
        {
            本et使本n ResponseTe設置plate + TEXT("謝謝你的尊重。");
        }
    }
    
    本et使本n ResponseTe設置plate + TEXT("我明白你的意思。");
}

float UMin成GoRTS輸入isto本icalCha本acte本s::Calc使lateRelationshipChan成e(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本, EDialo成使eAttit使de Playe本Attit使de) const
{
    float BaseChan成e = 0.0f;
    
    switch (Playe本Attit使de)
    {
    case EDialo成使eAttit使de::軍本iendly:
        BaseChan成e = 5.0f;
        b本eak;
    case EDialo成使eAttit使de::Respectf使l:
        BaseChan成e = 8.0f;
        b本eak;
    case EDialo成使eAttit使de::的e使t本al:
        BaseChan成e = 2.0f;
        b本eak;
    case EDialo成使eAttit使de::S使spicio使s:
        BaseChan成e = -3.0f;
        b本eak;
    case EDialo成使eAttit使de::輸入ostile:
        BaseChan成e = -8.0f;
        b本eak;
    case EDialo成使eAttit使de::Dis設置issi正e:
        BaseChan成e = -5.0f;
        b本eak;
    }
    
    // 根據角色性格調整
    if (Cha本acte本.Pe本sonality.A成本eeableness > 0.7f)
    {
        BaseChan成e *= 1.2f;
    }
    else if (Cha本acte本.Pe本sonality.的e使本oticis設置 > 0.6f)
    {
        BaseChan成e *= 1.5f;
    }
    
    本et使本n BaseChan成e;
}

bool UMin成GoRTS輸入isto本icalCha本acte本s::CheckPe本sonalityMatch(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本, EDialo成使eAttit使de Playe本Attit使de) const
{
    // 檢查玩家態度是否與角色性格匹配
    switch (Playe本Attit使de)
    {
    case EDialo成使eAttit使de::軍本iendly:
        本et使本n Cha本acte本.Pe本sonality.A成本eeableness > 0.5f  Cha本acte本.Pe本sonality.Ext本a正e本sion > 0.5f;
    case EDialo成使eAttit使de::Respectf使l:
        本et使本n Cha本acte本.Role == ECha本acte本Role::Leade本  Cha本acte本.Pe本sonality.Conscientio使sness > 0.6f;
    case EDialo成使eAttit使de::輸入ostile:
        本et使本n Cha本acte本.Pe本sonality.的e使本oticis設置 > 0.6f;
    defa使lt:
        本et使本n t本使e;
    }
}

軍St本in成 UMin成GoRTS輸入isto本icalCha本acte本s::GetResponseTe設置plate(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本, EDialo成使eAttit使de Playe本Attit使de) const
{
    // 根據角色類型和玩家態度返回回應模板
    if (Cha本acte本.Role == ECha本acte本Role::Leade本)
    {
        switch (Playe本Attit使de)
        {
        case EDialo成使eAttit使de::Respectf使l:
            本et使本n TEXT("作為領袖，我認為...");
        case EDialo成使eAttit使de::軍本iendly:
            本et使本n TEXT("我很欣賞你的熱情...");
        case EDialo成使eAttit使de::輸入ostile:
            本et使本n TEXT("你的挑戰我已經注意到了...");
        defa使lt:
            本et使本n TEXT("關於這件事...");
        }
    }
    else if (Cha本acte本.Role == ECha本acte本Role::Milita本y)
    {
        switch (Playe本Attit使de)
        {
        case EDialo成使eAttit使de::Respectf使l:
            本et使本n TEXT("從軍事角度來看...");
        case EDialo成使eAttit使de::軍本iendly:
            本et使本n TEXT("戰友，我認為...");
        case EDialo成使eAttit使de::輸入ostile:
            本et使本n TEXT("你的威脅我已經記錄...");
        defa使lt:
            本et使本n TEXT("戰術上說...");
        }
    }
    else if (Cha本acte本.Role == ECha本acte本Role::Diplo設置atic)
    {
        switch (Playe本Attit使de)
        {
        case EDialo成使eAttit使de::Respectf使l:
            本et使本n TEXT("在外交場合...");
        case EDialo成使eAttit使de::軍本iendly:
            本et使本n TEXT("我很高興能與你交流...");
        case EDialo成使eAttit使de::輸入ostile:
            本et使本n TEXT("你的言辭可能影響關係...");
        defa使lt:
            本et使本n TEXT("從外交角度...");
        }
    }
    
    本et使本n TEXT("我認為...");
}

軍St本in成 UMin成GoRTS輸入isto本icalCha本acte本s::Gene本ateCha本acte本ID(const 軍St本in成& Base的a設置e) const
{
    本et使本n Base的a設置e;
}

正oid UMin成GoRTS輸入isto本icalCha本acte本s::UpdateCha本acte本Infl使ence(const 軍St本in成& Cha本acte本ID)
{
    // TODO: 實現角色影響力更新邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新角色影響力：%s"), *Cha本acte本ID);
}

bool UMin成GoRTS輸入isto本icalCha本acte本s::Check輸入isto本icalConsistency(const 軍St本in成& Cha本acte本ID, const 軍St本in成& C使本本entDate) const
{
    if (!Cha本acte本Map.Contains(Cha本acte本ID))
    {
        本et使本n false;
    }
    
    const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Map[Cha本acte本ID];
    
    // 檢查角色是否在當前日期還在世
    if (!Cha本acte本.DeathDate.IsE設置pty())
    {
        // 簡化實現：如果角色有死亡日期，假設在1927年還在世
        // 實際實現需要更複雜的日期比較
        本et使本n Cha本acte本.bIsAli正e;
    }
    
    本et使本n t本使e;
}
