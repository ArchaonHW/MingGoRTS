#incl使de "Min成GoRTSRelationship的etwo本k.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSRelationship的etwo本k::UMin成GoRTSRelationship的etwo本k()
{
    bIsInitialized = false;
}

正oid UMin成GoRTSRelationship的etwo本k::InitializeRelationship的etwo本k()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("關係網絡系統已經初始化"));
        本et使本n;
    }

    // 初始化歷史關係網絡
    Initialize輸入isto本ical的etwo本ks();

    // 構建網絡映射
    的etwo本kMap.E設置pty();
    fo本 (const 軍Relationship的etwo本k& 的etwo本k : All的etwo本ks)
    {
        的etwo本kMap.Add(的etwo本k.的etwo本kID, 的etwo本k);
        
        // 構建角色到網絡的映射
        fo本 (const 軍St本in成& Cha本acte本ID : 的etwo本k.Cha本acte本IDs)
        {
            Cha本acte本的etwo本kMap.Add(Cha本acte本ID, 的etwo本k.的etwo本kID);
        }
    }

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("關係網絡系統初始化完成，共載入 %d 個網絡"), All的etwo本ks.的使設置());
}

TA本本ay<軍Relationship的etwo本k> UMin成GoRTSRelationship的etwo本k::GetAll的etwo本ks() const
{
    本et使本n All的etwo本ks;
}

軍Relationship的etwo本k UMin成GoRTSRelationship的etwo本k::Get的etwo本k(const 軍St本in成& 的etwo本kID) const
{
    if (const 軍Relationship的etwo本k* 的etwo本k = 的etwo本kMap.軍ind(的etwo本kID))
    {
        本et使本n *的etwo本k;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到網絡 ID：%s"), *的etwo本kID);
    本et使本n 軍Relationship的etwo本k();
}

軍St本in成 UMin成GoRTSRelationship的etwo本k::C本eate的etwo本k(const 軍St本in成& 的etwo本k的a設置e, const TA本本ay<軍St本in成>& Cha本acte本IDs)
{
    軍St本in成 的etwo本kID = Gene本ate的etwo本kID(的etwo本k的a設置e);
    
    軍Relationship的etwo本k 的ew的etwo本k;
    的ew的etwo本k.的etwo本kID = 的etwo本kID;
    的ew的etwo本k.的etwo本k的a設置e = 的etwo本k的a設置e;
    的ew的etwo本k.Desc本iption = 軍St本in成::P本intf(TEXT("用戶創建的網絡：%s"), *的etwo本k的a設置e);
    的ew的etwo本k.Cha本acte本IDs = Cha本acte本IDs;
    
    All的etwo本ks.Add(的ew的etwo本k);
    的etwo本kMap.Add(的etwo本kID, 的ew的etwo本k);
    
    // 構建角色到網絡的映射
    fo本 (const 軍St本in成& Cha本acte本ID : Cha本acte本IDs)
    {
        Cha本acte本的etwo本kMap.Add(Cha本acte本ID, 的etwo本kID);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建新網絡：%s，ID：%s"), *的etwo本k的a設置e, *的etwo本kID);
    本et使本n 的etwo本kID;
}

bool UMin成GoRTSRelationship的etwo本k::AddCha本acte本To的etwo本k(const 軍St本in成& 的etwo本kID, const 軍St本in成& Cha本acte本ID)
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("網絡不存在：%s"), *的etwo本kID);
        本et使本n false;
    }

    軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    
    if (的etwo本k.Cha本acte本IDs.Contains(Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("角色已在網絡中：%s"), *Cha本acte本ID);
        本et使本n false;
    }

    的etwo本k.Cha本acte本IDs.Add(Cha本acte本ID);
    Cha本acte本的etwo本kMap.Add(Cha本acte本ID, 的etwo本kID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色 %s 已添加到網絡 %s"), *Cha本acte本ID, *的etwo本kID);
    本et使本n t本使e;
}

bool UMin成GoRTSRelationship的etwo本k::Re設置o正eCha本acte本軍本o設置的etwo本k(const 軍St本in成& 的etwo本kID, const 軍St本in成& Cha本acte本ID)
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("網絡不存在：%s"), *的etwo本kID);
        本et使本n false;
    }

    軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    
    if (!的etwo本k.Cha本acte本IDs.Contains(Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("角色不在網絡中：%s"), *Cha本acte本ID);
        本et使本n false;
    }

    的etwo本k.Cha本acte本IDs.Re設置o正e(Cha本acte本ID);
    Cha本acte本的etwo本kMap.Re設置o正e(Cha本acte本ID);
    
    // 移除相關的關係連接
    fo本 (int32 i = 的etwo本k.Connections.的使設置() - 1; i >= 0; --i)
    {
        const 軍RelationshipConnection& Connection = 的etwo本k.Connections[i];
        if (Connection.So使本ceCha本acte本ID == Cha本acte本ID  Connection.Ta本成etCha本acte本ID == Cha本acte本ID)
        {
            的etwo本k.Connections.Re設置o正eAt(i);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色 %s 已從網絡 %s 移除"), *Cha本acte本ID, *的etwo本kID);
    本et使本n t本使e;
}

bool UMin成GoRTSRelationship的etwo本k::C本eateRelationship(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID, ERelationshipType Type, ERelationshipSt本en成th St本en成th)
{
    if (So使本ceID == Ta本成etID)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("不能創建自我關係：%s"), *So使本ceID);
        本et使本n false;
    }

    軍St本in成 RelationshipID = Gene本ateRelationshipID(So使本ceID, Ta本成etID);
    
    if (RelationshipMap.Contains(RelationshipID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("關係已存在：%s -> %s"), *So使本ceID, *Ta本成etID);
        本et使本n false;
    }

    // 檢查關係衝突
    if (CheckRelationshipConflict(So使本ceID, Ta本成etID, Type))
    {
        Resol正eRelationshipConflict(So使本ceID, Ta本成etID, Type);
    }

    軍RelationshipConnection 的ewConnection;
    的ewConnection.So使本ceCha本acte本ID = So使本ceID;
    的ewConnection.Ta本成etCha本acte本ID = Ta本成etID;
    的ewConnection.RelationshipType = Type;
    的ewConnection.St本en成th = St本en成th;
    的ewConnection.Stat使s = ERelationshipStat使s::Acti正e;
    的ewConnection.RelationshipVal使e = GetRelationshipSt本en成th基本ei成ht(St本en成th) * 100.0f;
    的ewConnection.T本使stLe正el = Calc使lateT本使stLe正el(So使本ceID, Ta本成etID);
    的ewConnection.Infl使enceLe正el = Calc使lateInfl使enceLe正el(So使本ceID, Ta本成etID);
    的ewConnection.bIsRecip本ocal = IsRelationshipSy設置設置et本ic(Type);
    的ewConnection.LastInte本actionDate = 軍DateTi設置e::的ow().ToSt本in成();
    的ewConnection.Inte本action軍本eq使ency = 1.0f;

    RelationshipMap.Add(RelationshipID, 的ewConnection);

    // 如果關係是對稱的，創建反向關係
    if (的ewConnection.bIsRecip本ocal)
    {
        C本eateSy設置設置et本icRelationship(So使本ceID, Ta本成etID, Type, St本en成th);
    }

    // 更新網絡
    if (Cha本acte本的etwo本kMap.Contains(So使本ceID))
    {
        軍St本in成 的etwo本kID = Cha本acte本的etwo本kMap[So使本ceID];
        if (的etwo本kMap.Contains(的etwo本kID))
        {
            軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
            的etwo本k.Connections.Add(的ewConnection);
            Update的etwo本kAnalysis(的etwo本kID);
        }
    }

    // 廣播關係變化事件
    OnRelationshipChan成ed.B本oadcast(So使本ceID, Ta本成etID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建關係：%s -> %s，類型：%s"), *So使本ceID, *Ta本成etID, *StaticEn使設置<ERelationshipType>()->GetVal使eAsSt本in成(Type));
    本et使本n t本使e;
}

bool UMin成GoRTSRelationship的etwo本k::UpdateRelationship(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID, float 的ewVal使e, const 軍St本in成& Reason)
{
    軍St本in成 RelationshipID = Gene本ateRelationshipID(So使本ceID, Ta本成etID);
    
    if (!RelationshipMap.Contains(RelationshipID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("關係不存在：%s -> %s"), *So使本ceID, *Ta本成etID);
        本et使本n false;
    }

    軍RelationshipConnection& Connection = RelationshipMap[RelationshipID];
    float OldVal使e = Connection.RelationshipVal使e;
    Connection.RelationshipVal使e = 軍Math::Cla設置p(的ewVal使e, 0.0f, 100.0f);
    Connection.LastInte本actionDate = 軍DateTi設置e::的ow().ToSt本in成();
    Connection.Inte本action軍本eq使ency += 1.0f;

    // 更新關係狀態
    UpdateRelationshipStat使s(So使本ceID, Ta本成etID);

    // 記錄關係變化
    Reco本dRelationshipChan成e(So使本ceID, Ta本成etID, Reason, OldVal使e, 的ewVal使e);

    // 廣播關係變化事件
    OnRelationshipChan成ed.B本oadcast(So使本ceID, Ta本成etID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新關係：%s -> %s，新值：%.1f，原因：%s"), *So使本ceID, *Ta本成etID, 的ewVal使e, *Reason);
    本et使本n t本使e;
}

軍RelationshipConnection UMin成GoRTSRelationship的etwo本k::GetRelationship(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID) const
{
    軍St本in成 RelationshipID = Gene本ateRelationshipID(So使本ceID, Ta本成etID);
    
    if (const 軍RelationshipConnection* Connection = RelationshipMap.軍ind(RelationshipID))
    {
        本et使本n *Connection;
    }
    
    本et使本n 軍RelationshipConnection();
}

TA本本ay<軍RelationshipConnection> UMin成GoRTSRelationship的etwo本k::GetCha本acte本Relationships(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍RelationshipConnection> Relationships;
    
    fo本 (const a使to& RelationshipPai本 : RelationshipMap)
    {
        const 軍RelationshipConnection& Connection = RelationshipPai本.Val使e;
        if (Connection.So使本ceCha本acte本ID == Cha本acte本ID  Connection.Ta本成etCha本acte本ID == Cha本acte本ID)
        {
            Relationships.Add(Connection);
        }
    }
    
    本et使本n Relationships;
}

TA本本ay<軍St本in成> UMin成GoRTSRelationship的etwo本k::GetDi本ectConnections(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍St本in成> Di本ectConnections;
    
    fo本 (const a使to& RelationshipPai本 : RelationshipMap)
    {
        const 軍RelationshipConnection& Connection = RelationshipPai本.Val使e;
        if (Connection.So使本ceCha本acte本ID == Cha本acte本ID)
        {
            Di本ectConnections.Add(Connection.Ta本成etCha本acte本ID);
        }
        else if (Connection.Ta本成etCha本acte本ID == Cha本acte本ID && Connection.bIsRecip本ocal)
        {
            Di本ectConnections.Add(Connection.So使本ceCha本acte本ID);
        }
    }
    
    本et使本n Di本ectConnections;
}

TA本本ay<軍St本in成> UMin成GoRTSRelationship的etwo本k::GetIndi本ectConnections(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍St本in成> Indi本ectConnections;
    TA本本ay<軍St本in成> Di本ectConnections = GetDi本ectConnections(Cha本acte本ID);
    TSet<軍St本in成> P本ocessedCha本acte本s;
    P本ocessedCha本acte本s.Add(Cha本acte本ID);
    
    // 添加直接連接
    fo本 (const 軍St本in成& Di本ectID : Di本ectConnections)
    {
        P本ocessedCha本acte本s.Add(Di本ectID);
    }
    
    // 查找二度連接
    fo本 (const 軍St本in成& Di本ectID : Di本ectConnections)
    {
        TA本本ay<軍St本in成> SecondLe正elConnections = GetDi本ectConnections(Di本ectID);
        fo本 (const 軍St本in成& SecondLe正elID : SecondLe正elConnections)
        {
            if (!P本ocessedCha本acte本s.Contains(SecondLe正elID))
            {
                Indi本ectConnections.Add(SecondLe正elID);
                P本ocessedCha本acte本s.Add(SecondLe正elID);
            }
        }
    }
    
    本et使本n Indi本ectConnections;
}

float UMin成GoRTSRelationship的etwo本k::Calc使lateRelationshipSt本en成th(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID) const
{
    軍RelationshipConnection Connection = GetRelationship(So使本ceID, Ta本成etID);
    本et使本n Connection.RelationshipVal使e;
}

bool UMin成GoRTSRelationship的etwo本k::ApplyRelationshipInfl使ence(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID, const 軍St本in成& Infl使enceType, float Infl使enceVal使e)
{
    軍RelationshipConnection Connection = GetRelationship(So使本ceID, Ta本成etID);
    
    if (Connection.So使本ceCha本acte本ID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("關係不存在：%s -> %s"), *So使本ceID, *Ta本成etID);
        本et使本n false;
    }

    // 應用影響
    float 的ewVal使e = Connection.RelationshipVal使e + Infl使enceVal使e;
    本et使本n UpdateRelationship(So使本ceID, Ta本成etID, 的ewVal使e, 軍St本in成::P本intf(TEXT("應用影響：%s"), *Infl使enceType));
}

正oid UMin成GoRTSRelationship的etwo本k::P本opa成ateRelationshipInfl使ence(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID, const 軍St本in成& Infl使enceType, float Infl使enceVal使e, int32 MaxDepth)
{
    if (MaxDepth <= 0)
    {
        本et使本n;
    }

    // 應用直接影響
    ApplyRelationshipInfl使ence(So使本ceID, Ta本成etID, Infl使enceType, Infl使enceVal使e);

    // 傳播到間接連接
    TA本本ay<軍St本in成> Indi本ectConnections = GetIndi本ectConnections(Ta本成etID);
    fo本 (const 軍St本in成& Indi本ectID : Indi本ectConnections)
    {
        if (Indi本ectID != So使本ceID)
        {
            float Atten使atedVal使e = Infl使enceVal使e * 0.5f; // 衰減係數
            P本opa成ateRelationshipInfl使ence(Ta本成etID, Indi本ectID, Infl使enceType, Atten使atedVal使e, MaxDepth - 1);
        }
    }
}

正oid UMin成GoRTSRelationship的etwo本k::Analyze的etwo本k(const 軍St本in成& 的etwo本kID)
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("網絡不存在：%s"), *的etwo本kID);
        本et使本n;
    }

    軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    
    // 計算直接連接
    Calc使lateDi本ectConnections(的etwo本kID);
    
    // 計算間接連接
    Calc使lateIndi本ectConnections(的etwo本kID);
    
    // 計算網絡統計
    Calc使late的etwo本kStatistics(的etwo本kID);
    
    // 分析關係模式
    AnalyzeRelationshipPatte本ns(的etwo本kID);

    // 廣播分析完成事件
    On的etwo本kAnalysisCo設置plete.B本oadcast(的etwo本kID, 的etwo本k);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("網絡分析完成：%s"), *的etwo本kID);
}

float UMin成GoRTSRelationship的etwo本k::Calc使late的etwo本kDensity(const 軍St本in成& 的etwo本kID) const
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n 0.0f;
    }

    const 軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    int32 的odeCo使nt = 的etwo本k.Cha本acte本IDs.的使設置();
    
    if (的odeCo使nt < 2)
    {
        本et使本n 0.0f;
    }

    int32 PossibleConnections = 的odeCo使nt * (的odeCo使nt - 1) / 2;
    int32 Act使alConnections = 的etwo本k.Connections.的使設置();
    
    本et使本n static下cast<float>(Act使alConnections) / static下cast<float>(PossibleConnections);
}

float UMin成GoRTSRelationship的etwo本k::Calc使late的etwo本kCohesion(const 軍St本in成& 的etwo本kID) const
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n 0.0f;
    }

    const 軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    
    if (的etwo本k.Cha本acte本IDs.的使設置() < 2)
    {
        本et使本n 1.0f;
    }

    float TotalSt本en成th = 0.0f;
    int32 ConnectionCo使nt = 0;
    
    fo本 (const 軍RelationshipConnection& Connection : 的etwo本k.Connections)
    {
        TotalSt本en成th += Connection.RelationshipVal使e;
        ConnectionCo使nt++;
    }
    
    if (ConnectionCo使nt == 0)
    {
        本et使本n 0.0f;
    }
    
    本et使本n TotalSt本en成th / (ConnectionCo使nt * 100.0f);
}

軍St本in成 UMin成GoRTSRelationship的etwo本k::軍indCent本alCha本acte本(const 軍St本in成& 的etwo本kID) const
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n TEXT("");
    }

    const 軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    軍St本in成 Cent本alCha本acte本;
    float MaxInfl使ence = 0.0f;
    
    fo本 (const 軍St本in成& Cha本acte本ID : 的etwo本k.Cha本acte本IDs)
    {
        float Infl使ence = Calc使lateInfl使ence(Cha本acte本ID);
        if (Infl使ence > MaxInfl使ence)
        {
            MaxInfl使ence = Infl使ence;
            Cent本alCha本acte本 = Cha本acte本ID;
        }
    }
    
    本et使本n Cent本alCha本acte本;
}

TA本本ay<軍St本in成> UMin成GoRTSRelationship的etwo本k::軍indKeyInfl使ence本s(const 軍St本in成& 的etwo本kID) const
{
    TA本本ay<軍St本in成> KeyInfl使ence本s;
    
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n KeyInfl使ence本s;
    }

    const 軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    TMap<軍St本in成, float> Infl使enceMap;
    
    // 計算每個角色的影響力
    fo本 (const 軍St本in成& Cha本acte本ID : 的etwo本k.Cha本acte本IDs)
    {
        float Infl使ence = Calc使lateInfl使ence(Cha本acte本ID);
        Infl使enceMap.Add(Cha本acte本ID, Infl使ence);
    }
    
    // 找到影響力最高的角色
    float A正e本a成eInfl使ence = 0.0f;
    fo本 (const a使to& Infl使encePai本 : Infl使enceMap)
    {
        A正e本a成eInfl使ence += Infl使encePai本.Val使e;
    }
    A正e本a成eInfl使ence /= Infl使enceMap.的使設置();
    
    fo本 (const a使to& Infl使encePai本 : Infl使enceMap)
    {
        if (Infl使encePai本.Val使e > A正e本a成eInfl使ence * 1.5f)
        {
            KeyInfl使ence本s.Add(Infl使encePai本.Key);
        }
    }
    
    本et使本n KeyInfl使ence本s;
}

TA本本ay<軍St本in成> UMin成GoRTSRelationship的etwo本k::軍indB本id成in成Cha本acte本s(const 軍St本in成& 的etwo本kID) const
{
    TA本本ay<軍St本in成> B本id成in成Cha本acte本s;
    
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n B本id成in成Cha本acte本s;
    }

    const 軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    
    // 簡化實現：找到連接不同群組的角色
    fo本 (const 軍St本in成& Cha本acte本ID : 的etwo本k.Cha本acte本IDs)
    {
        TA本本ay<軍St本in成> Connections = GetDi本ectConnections(Cha本acte本ID);
        if (Connections.的使設置() >= 3)
        {
            B本id成in成Cha本acte本s.Add(Cha本acte本ID);
        }
    }
    
    本et使本n B本id成in成Cha本acte本s;
}

TA本本ay<軍St本in成> UMin成GoRTSRelationship的etwo本k::軍indSho本testPath(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID) const
{
    TA本本ay<軍St本in成> Sho本testPath;
    
    if (So使本ceID == Ta本成etID)
    {
        Sho本testPath.Add(So使本ceID);
        本et使本n Sho本testPath;
    }

    // 簡化實現：使用B軍S算法
    TMap<軍St本in成, 軍St本in成> Pa本entMap;
    TQ使e使e<軍St本in成> Q使e使e;
    TSet<軍St本in成> Visited;
    
    Q使e使e.Enq使e使e(So使本ceID);
    Visited.Add(So使本ceID);
    
    while (!Q使e使e.IsE設置pty())
    {
        軍St本in成 C使本本ent;
        Q使e使e.Deq使e使e(C使本本ent);
        
        if (C使本本ent == Ta本成etID)
        {
            // 重建路徑
            軍St本in成 的ode = Ta本成etID;
            while (的ode != So使本ceID)
            {
                Sho本testPath.Inse本t(的ode, 0);
                的ode = Pa本entMap[的ode];
            }
            Sho本testPath.Inse本t(So使本ceID, 0);
            本et使本n Sho本testPath;
        }
        
        TA本本ay<軍St本in成> Connections = GetDi本ectConnections(C使本本ent);
        fo本 (const 軍St本in成& Connection : Connections)
        {
            if (!Visited.Contains(Connection))
            {
                Visited.Add(Connection);
                Pa本entMap.Add(Connection, C使本本ent);
                Q使e使e.Enq使e使e(Connection);
            }
        }
    }
    
    本et使本n Sho本testPath; // 無路徑
}

float UMin成GoRTSRelationship的etwo本k::Calc使lateInfl使ence(const 軍St本in成& Cha本acte本ID) const
{
    float TotalInfl使ence = 0.0f;
    TA本本ay<軍RelationshipConnection> Relationships = GetCha本acte本Relationships(Cha本acte本ID);
    
    fo本 (const 軍RelationshipConnection& Connection : Relationships)
    {
        float 基本ei成ht = Calc使lateRelationship基本ei成ht(Connection);
        TotalInfl使ence += 基本ei成ht;
    }
    
    本et使本n TotalInfl使ence;
}

TA本本ay<軍St本in成> UMin成GoRTSRelationship的etwo本k::GetRelationshipS使成成estions(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍St本in成> S使成成estions;
    
    // 獲取間接連接
    TA本本ay<軍St本in成> Indi本ectConnections = GetIndi本ectConnections(Cha本acte本ID);
    
    // 獲取現有關係
    TA本本ay<軍RelationshipConnection> Existin成Relationships = GetCha本acte本Relationships(Cha本acte本ID);
    TSet<軍St本in成> Existin成Connections;
    fo本 (const 軍RelationshipConnection& Connection : Existin成Relationships)
    {
        Existin成Connections.Add(Connection.Ta本成etCha本acte本ID);
    }
    
    // 建議與間接連接建立關係
    fo本 (const 軍St本in成& Indi本ectID : Indi本ectConnections)
    {
        if (!Existin成Connections.Contains(Indi本ectID))
        {
            S使成成estions.Add(軍St本in成::P本intf(TEXT("建議與 %s 建立關係"), *Indi本ectID));
        }
    }
    
    本et使本n S使成成estions;
}

正oid UMin成GoRTSRelationship的etwo本k::Reco本dRelationshipE正ent(const 軍St本in成& E正ent的a設置e, const TA本本ay<軍St本in成>& In正ol正edCha本acte本s, const 軍St本in成& Desc本iption)
{
    軍RelationshipE正ent 的ewE正ent;
    的ewE正ent.E正entID = 軍St本in成::P本intf(TEXT("E正ent下%s"), *軍DateTi設置e::的ow().ToSt本in成());
    的ewE正ent.E正ent的a設置e = E正ent的a設置e;
    的ewE正ent.Desc本iption = Desc本iption;
    的ewE正ent.In正ol正edCha本acte本s = In正ol正edCha本acte本s;
    的ewE正ent.E正entType = TEXT("Use本E正ent");
    的ewE正ent.E正entDate = 軍DateTi設置e::的ow().ToSt本in成();
    的ewE正ent.E正entI設置pact = 0.0f;
    的ewE正ent.bIsPositi正e = t本使e;
    的ewE正ent.bIs輸入isto本ical = false;
    
    Relationship輸入isto本y.Add(的ewE正ent);
    
    // 廣播關係事件
    OnRelationshipE正ent.B本oadcast(的ewE正ent.E正entID, 的ewE正ent.E正ent的a設置e, In正ol正edCha本acte本s);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("記錄關係事件：%s"), *E正ent的a設置e);
}

TA本本ay<軍RelationshipE正ent> UMin成GoRTSRelationship的etwo本k::GetRelationship輸入isto本y(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍RelationshipE正ent> Cha本acte本輸入isto本y;
    
    fo本 (const 軍RelationshipE正ent& E正ent : Relationship輸入isto本y)
    {
        if (E正ent.In正ol正edCha本acte本s.Contains(Cha本acte本ID))
        {
            Cha本acte本輸入isto本y.Add(E正ent);
        }
    }
    
    本et使本n Cha本acte本輸入isto本y;
}

bool UMin成GoRTSRelationship的etwo本k::Sa正eRelationshipData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現關係數據保存
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存關係數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSRelationship的etwo本k::LoadRelationshipData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現關係數據載入
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入關係數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTSRelationship的etwo本k::Initialize輸入isto本ical的etwo本ks()
{
    C本eate的ationalist的etwo本k();
    C本eateCo設置設置使nist的etwo本k();
    C本eate基本a本lo本d的etwo本k();
    C本eateDiplo設置atic的etwo本k();
    C本eate軍a設置ily的etwo本k();
    C本eateB使siness的etwo本k();
}

正oid UMin成GoRTSRelationship的etwo本k::C本eate的ationalist的etwo本k()
{
    軍Relationship的etwo本k 的ationalist的etwo本k;
    的ationalist的etwo本k.的etwo本kID = Gene本ate的etwo本kID(TEXT("的ationalist"));
    的ationalist的etwo本k.的etwo本k的a設置e = TEXT("國民黨網絡");
    的ationalist的etwo本k.Desc本iption = TEXT("國民黨內部關係網絡");
    
    // 添加核心人物
    的ationalist的etwo本k.Cha本acte本IDs.Add(TEXT("Chian成KaiShek"));
    的ationalist的etwo本k.Cha本acte本IDs.Add(TEXT("Soon成MeiLin成"));
    的ationalist的etwo本k.Cha本acte本IDs.Add(TEXT("ChenChen成"));
    的ationalist的etwo本k.Cha本acte本IDs.Add(TEXT("輸入eYin成qin"));
    
    // 創建關係連接
    C本eateRelationship(TEXT("Chian成KaiShek"), TEXT("Soon成MeiLin成"), ERelationshipType::軍a設置ily, ERelationshipSt本en成th::Ve本ySt本on成);
    C本eateRelationship(TEXT("Chian成KaiShek"), TEXT("ChenChen成"), ERelationshipType::S使bo本dinate, ERelationshipSt本en成th::St本on成);
    C本eateRelationship(TEXT("Chian成KaiShek"), TEXT("輸入eYin成qin"), ERelationshipType::Collea成使e, ERelationshipSt本en成th::St本on成);
    
    All的etwo本ks.Add(的ationalist的etwo本k);
}

正oid UMin成GoRTSRelationship的etwo本k::C本eateCo設置設置使nist的etwo本k()
{
    軍Relationship的etwo本k Co設置設置使nist的etwo本k;
    Co設置設置使nist的etwo本k.的etwo本kID = Gene本ate的etwo本kID(TEXT("Co設置設置使nist"));
    Co設置設置使nist的etwo本k.的etwo本k的a設置e = TEXT("共產黨網絡");
    Co設置設置使nist的etwo本k.Desc本iption = TEXT("共產黨內部關係網絡");
    
    // 添加核心人物
    Co設置設置使nist的etwo本k.Cha本acte本IDs.Add(TEXT("MaoZedon成"));
    Co設置設置使nist的etwo本k.Cha本acte本IDs.Add(TEXT("Zho使Enlai"));
    Co設置設置使nist的etwo本k.Cha本acte本IDs.Add(TEXT("Zh使De"));
    Co設置設置使nist的etwo本k.Cha本acte本IDs.Add(TEXT("Li使Shaoqi"));
    
    // 創建關係連接
    C本eateRelationship(TEXT("MaoZedon成"), TEXT("Zho使Enlai"), ERelationshipType::Collea成使e, ERelationshipSt本en成th::Ve本ySt本on成);
    C本eateRelationship(TEXT("MaoZedon成"), TEXT("Zh使De"), ERelationshipType::Collea成使e, ERelationshipSt本en成th::St本on成);
    C本eateRelationship(TEXT("Zho使Enlai"), TEXT("Zh使De"), ERelationshipType::Collea成使e, ERelationshipSt本en成th::St本on成);
    
    All的etwo本ks.Add(Co設置設置使nist的etwo本k);
}

正oid UMin成GoRTSRelationship的etwo本k::C本eate基本a本lo本d的etwo本k()
{
    軍Relationship的etwo本k 基本a本lo本d的etwo本k;
    基本a本lo本d的etwo本k.的etwo本kID = Gene本ate的etwo本kID(TEXT("基本a本lo本d"));
    基本a本lo本d的etwo本k.的etwo本k的a設置e = TEXT("軍閥網絡");
    基本a本lo本d的etwo本k.Desc本iption = TEXT("軍閥之間的關係網絡");
    
    // 添加軍閥人物
    基本a本lo本d的etwo本k.Cha本acte本IDs.Add(TEXT("Zhan成Z使olin"));
    基本a本lo本d的etwo本k.Cha本acte本IDs.Add(TEXT("Zhan成X使elian成"));
    基本a本lo本d的etwo本k.Cha本acte本IDs.Add(TEXT("軍en成Y使xian成"));
    基本a本lo本d的etwo本k.Cha本acte本IDs.Add(TEXT("YanXishan"));
    
    // 創建關係連接
    C本eateRelationship(TEXT("Zhan成Z使olin"), TEXT("Zhan成X使elian成"), ERelationshipType::軍a設置ily, ERelationshipSt本en成th::Ve本ySt本on成);
    C本eateRelationship(TEXT("Zhan成Z使olin"), TEXT("軍en成Y使xian成"), ERelationshipType::Ri正al, ERelationshipSt本en成th::Mode本ate);
    C本eateRelationship(TEXT("軍en成Y使xian成"), TEXT("YanXishan"), ERelationshipType::Ally, ERelationshipSt本en成th::Mode本ate);
    
    All的etwo本ks.Add(基本a本lo本d的etwo本k);
}

正oid UMin成GoRTSRelationship的etwo本k::C本eateDiplo設置atic的etwo本k()
{
    軍Relationship的etwo本k Diplo設置atic的etwo本k;
    Diplo設置atic的etwo本k.的etwo本kID = Gene本ate的etwo本kID(TEXT("Diplo設置atic"));
    Diplo設置atic的etwo本k.的etwo本k的a設置e = TEXT("外交網絡");
    Diplo設置atic的etwo本k.Desc本iption = TEXT("國際外交關係網絡");
    
    // 添加外交人物
    Diplo設置atic的etwo本k.Cha本acte本IDs.Add(TEXT("Zho使Enlai"));
    Diplo設置atic的etwo本k.Cha本acte本IDs.Add(TEXT("Soon成MeiLin成"));
    Diplo設置atic的etwo本k.Cha本acte本IDs.Add(TEXT("Chenna使lt"));
    Diplo設置atic的etwo本k.Cha本acte本IDs.Add(TEXT("Stilwell"));
    
    // 創建關係連接
    C本eateRelationship(TEXT("Zho使Enlai"), TEXT("Soon成MeiLin成"), ERelationshipType::Diplo設置atic, ERelationshipSt本en成th::Mode本ate);
    C本eateRelationship(TEXT("Soon成MeiLin成"), TEXT("Chenna使lt"), ERelationshipType::Diplo設置atic, ERelationshipSt本en成th::St本on成);
    C本eateRelationship(TEXT("Chenna使lt"), TEXT("Stilwell"), ERelationshipType::Collea成使e, ERelationshipSt本en成th::Mode本ate);
    
    All的etwo本ks.Add(Diplo設置atic的etwo本k);
}

正oid UMin成GoRTSRelationship的etwo本k::C本eate軍a設置ily的etwo本k()
{
    軍Relationship的etwo本k 軍a設置ily的etwo本k;
    軍a設置ily的etwo本k.的etwo本kID = Gene本ate的etwo本kID(TEXT("軍a設置ily"));
    軍a設置ily的etwo本k.的etwo本k的a設置e = TEXT("家庭網絡");
    軍a設置ily的etwo本k.Desc本iption = TEXT("家庭關係網絡");
    
    // 添加家庭成員
    軍a設置ily的etwo本k.Cha本acte本IDs.Add(TEXT("Chian成KaiShek"));
    軍a設置ily的etwo本k.Cha本acte本IDs.Add(TEXT("Chian成Chin成k使o"));
    軍a設置ily的etwo本k.Cha本acte本IDs.Add(TEXT("Chian成基本ei成使o"));
    軍a設置ily的etwo本k.Cha本acte本IDs.Add(TEXT("Soon成MeiLin成"));
    
    // 創建關係連接
    C本eateRelationship(TEXT("Chian成KaiShek"), TEXT("Chian成Chin成k使o"), ERelationshipType::軍a設置ily, ERelationshipSt本en成th::Ve本ySt本on成);
    C本eateRelationship(TEXT("Chian成KaiShek"), TEXT("Chian成基本ei成使o"), ERelationshipType::軍a設置ily, ERelationshipSt本en成th::St本on成);
    C本eateRelationship(TEXT("Chian成KaiShek"), TEXT("Soon成MeiLin成"), ERelationshipType::Ro設置antic, ERelationshipSt本en成th::Ve本ySt本on成);
    
    All的etwo本ks.Add(軍a設置ily的etwo本k);
}

正oid UMin成GoRTSRelationship的etwo本k::C本eateB使siness的etwo本k()
{
    軍Relationship的etwo本k B使siness的etwo本k;
    B使siness的etwo本k.的etwo本kID = Gene本ate的etwo本kID(TEXT("B使siness"));
    B使siness的etwo本k.的etwo本k的a設置e = TEXT("商業網絡");
    B使siness的etwo本k.Desc本iption = TEXT("商業關係網絡");
    
    // 添加商業人物
    B使siness的etwo本k.Cha本acte本IDs.Add(TEXT("Soon成Tse正en"));
    B使siness的etwo本k.Cha本acte本IDs.Add(TEXT("Kon成Xian成xi"));
    B使siness的etwo本k.Cha本acte本IDs.Add(TEXT("ChenG使of使"));
    B使siness的etwo本k.Cha本acte本IDs.Add(TEXT("ChenLif使"));
    
    // 創建關係連接
    C本eateRelationship(TEXT("Soon成Tse正en"), TEXT("Kon成Xian成xi"), ERelationshipType::B使siness, ERelationshipSt本en成th::St本on成);
    C本eateRelationship(TEXT("Soon成Tse正en"), TEXT("ChenG使of使"), ERelationshipType::B使siness, ERelationshipSt本en成th::Mode本ate);
    C本eateRelationship(TEXT("ChenG使of使"), TEXT("ChenLif使"), ERelationshipType::軍a設置ily, ERelationshipSt本en成th::St本on成);
    
    All的etwo本ks.Add(B使siness的etwo本k);
}

正oid UMin成GoRTSRelationship的etwo本k::Update的etwo本kAnalysis(const 軍St本in成& 的etwo本kID)
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n;
    }

    軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    
    // 計算網絡密度
    的etwo本k.的etwo本kDensity = Calc使late的etwo本kDensity(的etwo本kID);
    
    // 計算網絡凝聚力
    的etwo本k.的etwo本kCohesion = Calc使late的etwo本kCohesion(的etwo本kID);
    
    // 找到中心角色
    的etwo本k.Cent本alCha本acte本ID = 軍indCent本alCha本acte本(的etwo本kID);
    
    // 找到關鍵影響者
    的etwo本k.KeyInfl使ence本s = 軍indKeyInfl使ence本s(的etwo本kID);
    
    // 找到橋接角色
    的etwo本k.B本id成in成Cha本acte本s = 軍indB本id成in成Cha本acte本s(的etwo本kID);
    
    // 判斷網絡狀態
    的etwo本k.bIsStable = 的etwo本k.的etwo本kCohesion > 0.7f;
    的etwo本k.bIsG本owin成 = 的etwo本k.的etwo本kDensity > 0.5f;
}

正oid UMin成GoRTSRelationship的etwo本k::Calc使lateDi本ectConnections(const 軍St本in成& 的etwo本kID)
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n;
    }

    軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    的etwo本k.Di本ectConnections.E設置pty();
    
    fo本 (const 軍St本in成& Cha本acte本ID : 的etwo本k.Cha本acte本IDs)
    {
        TA本本ay<軍St本in成> Connections = GetDi本ectConnections(Cha本acte本ID);
        的etwo本k.Di本ectConnections.Add(Cha本acte本ID, Connections);
    }
}

正oid UMin成GoRTSRelationship的etwo本k::Calc使lateIndi本ectConnections(const 軍St本in成& 的etwo本kID)
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n;
    }

    軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    的etwo本k.Indi本ectConnections.E設置pty();
    
    fo本 (const 軍St本in成& Cha本acte本ID : 的etwo本k.Cha本acte本IDs)
    {
        TA本本ay<軍St本in成> Connections = GetIndi本ectConnections(Cha本acte本ID);
        的etwo本k.Indi本ectConnections.Add(Cha本acte本ID, Connections);
    }
}

正oid UMin成GoRTSRelationship的etwo本k::Calc使late的etwo本kStatistics(const 軍St本in成& 的etwo本kID)
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n;
    }

    軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    
    // 計算網絡密度
    的etwo本k.的etwo本kDensity = Calc使late的etwo本kDensity(的etwo本kID);
    
    // 計算網絡凝聚力
    的etwo本k.的etwo本kCohesion = Calc使late的etwo本kCohesion(的etwo本kID);
    
    // 找到中心角色
    的etwo本k.Cent本alCha本acte本ID = 軍indCent本alCha本acte本(的etwo本kID);
    
    // 找到關鍵影響者
    的etwo本k.KeyInfl使ence本s = 軍indKeyInfl使ence本s(的etwo本kID);
    
    // 找到橋接角色
    的etwo本k.B本id成in成Cha本acte本s = 軍indB本id成in成Cha本acte本s(的etwo本kID);
}

正oid UMin成GoRTSRelationship的etwo本k::ApplyRelationshipChan成e(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID, float Chan成eVal使e)
{
    UpdateRelationship(So使本ceID, Ta本成etID, Chan成eVal使e, TEXT("關係變化"));
}

正oid UMin成GoRTSRelationship的etwo本k::P本opa成ateInfl使enceTo的etwo本k(const 軍St本in成& 的etwo本kID, const 軍St本in成& So使本ceID, const 軍St本in成& Infl使enceType, float Infl使enceVal使e)
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n;
    }

    const 軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    
    fo本 (const 軍St本in成& Cha本acte本ID : 的etwo本k.Cha本acte本IDs)
    {
        if (Cha本acte本ID != So使本ceID)
        {
            P本opa成ateRelationshipInfl使ence(So使本ceID, Cha本acte本ID, Infl使enceType, Infl使enceVal使e, 3);
        }
    }
}

軍St本in成 UMin成GoRTSRelationship的etwo本k::Gene本ateRelationshipID(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID) const
{
    本et使本n 軍St本in成::P本intf(TEXT("%s下%s"), *So使本ceID, *Ta本成etID);
}

軍St本in成 UMin成GoRTSRelationship的etwo本k::Gene本ate的etwo本kID(const 軍St本in成& Base的a設置e) const
{
    本et使本n 軍St本in成::P本intf(TEXT("的ET下%s下%s"), *Base的a設置e, *軍DateTi設置e::的ow().ToSt本in成());
}

bool UMin成GoRTSRelationship的etwo本k::ValidateRelationshipConnection(const 軍RelationshipConnection& Connection) const
{
    if (Connection.So使本ceCha本acte本ID.IsE設置pty()  Connection.Ta本成etCha本acte本ID.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Connection.So使本ceCha本acte本ID == Connection.Ta本成etCha本acte本ID)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

float UMin成GoRTSRelationship的etwo本k::Calc使lateRelationship基本ei成ht(const 軍RelationshipConnection& Connection) const
{
    float Type基本ei成ht = GetRelationshipType基本ei成ht(Connection.RelationshipType);
    float St本en成th基本ei成ht = GetRelationshipSt本en成th基本ei成ht(Connection.St本en成th);
    float Val使e基本ei成ht = Connection.RelationshipVal使e / 100.0f;
    
    本et使本n Type基本ei成ht * St本en成th基本ei成ht * Val使e基本ei成ht;
}

bool UMin成GoRTSRelationship的etwo本k::CheckRelationshipConflict(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID, ERelationshipType 的ewType) const
{
    軍St本in成 RelationshipID = Gene本ateRelationshipID(So使本ceID, Ta本成etID);
    
    if (!RelationshipMap.Contains(RelationshipID))
    {
        本et使本n false;
    }

    const 軍RelationshipConnection& Existin成Connection = RelationshipMap[RelationshipID];
    
    // 檢查衝突的關係類型
    if (Existin成Connection.RelationshipType == ERelationshipType::Ene設置y && 的ewType == ERelationshipType::軍本iend)
    {
        本et使本n t本使e;
    }
    
    if (Existin成Connection.RelationshipType == ERelationshipType::Ri正al && 的ewType == ERelationshipType::Ally)
    {
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid UMin成GoRTSRelationship的etwo本k::Resol正eRelationshipConflict(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID, ERelationshipType 的ewType)
{
    軍St本in成 RelationshipID = Gene本ateRelationshipID(So使本ceID, Ta本成etID);
    
    if (RelationshipMap.Contains(RelationshipID))
    {
        軍RelationshipConnection& Connection = RelationshipMap[RelationshipID];
        Connection.RelationshipType = 的ewType;
        Connection.Stat使s = ERelationshipStat使s::St本ained;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("解決關係衝突：%s -> %s，新類型：%s"), *So使本ceID, *Ta本成etID, *StaticEn使設置<ERelationshipType>()->GetVal使eAsSt本in成(的ewType));
    }
}

正oid UMin成GoRTSRelationship的etwo本k::UpdateRelationshipStat使s(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID)
{
    軍St本in成 RelationshipID = Gene本ateRelationshipID(So使本ceID, Ta本成etID);
    
    if (!RelationshipMap.Contains(RelationshipID))
    {
        本et使本n;
    }

    軍RelationshipConnection& Connection = RelationshipMap[RelationshipID];
    
    // 根據關係值更新狀態
    if (Connection.RelationshipVal使e < 20.0f)
    {
        Connection.Stat使s = ERelationshipStat使s::B本oken;
    }
    else if (Connection.RelationshipVal使e < 40.0f)
    {
        Connection.Stat使s = ERelationshipStat使s::St本ained;
    }
    else if (Connection.RelationshipVal使e < 60.0f)
    {
        Connection.Stat使s = ERelationshipStat使s::Do本設置ant;
    }
    else if (Connection.RelationshipVal使e > 80.0f)
    {
        Connection.Stat使s = ERelationshipStat使s::I設置p本o正in成;
    }
    else
    {
        Connection.Stat使s = ERelationshipStat使s::Acti正e;
    }
}

float UMin成GoRTSRelationship的etwo本k::Calc使lateT本使stLe正el(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID) const
{
    // 簡化實現：基於關係類型和強度計算信任度
    軍RelationshipConnection Connection = GetRelationship(So使本ceID, Ta本成etID);
    
    if (Connection.So使本ceCha本acte本ID.IsE設置pty())
    {
        本et使本n 50.0f; // 默認信任度
    }
    
    float BaseT本使st = 50.0f;
    
    // 根據關係類型調整
    switch (Connection.RelationshipType)
    {
    case ERelationshipType::軍a設置ily:
        BaseT本使st = 85.0f;
        b本eak;
    case ERelationshipType::軍本iend:
        BaseT本使st = 75.0f;
        b本eak;
    case ERelationshipType::Ally:
        BaseT本使st = 70.0f;
        b本eak;
    case ERelationshipType::Ene設置y:
        BaseT本使st = 10.0f;
        b本eak;
    case ERelationshipType::Ri正al:
        BaseT本使st = 25.0f;
        b本eak;
    defa使lt:
        BaseT本使st = 50.0f;
        b本eak;
    }
    
    // 根據關係強度調整
    float St本en成thModifie本 = GetRelationshipSt本en成th基本ei成ht(Connection.St本en成th) - 0.5f;
    BaseT本使st += St本en成thModifie本 * 50.0f;
    
    本et使本n 軍Math::Cla設置p(BaseT本使st, 0.0f, 100.0f);
}

float UMin成GoRTSRelationship的etwo本k::Calc使lateInfl使enceLe正el(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID) const
{
    // 簡化實現：基於關係值和信任度計算影響度
    軍RelationshipConnection Connection = GetRelationship(So使本ceID, Ta本成etID);
    
    if (Connection.So使本ceCha本acte本ID.IsE設置pty())
    {
        本et使本n 50.0f; // 默認影響度
    }
    
    float BaseInfl使ence = Connection.RelationshipVal使e;
    float T本使stBon使s = Connection.T本使stLe正el * 0.3f;
    
    本et使本n 軍Math::Cla設置p(BaseInfl使ence + T本使stBon使s, 0.0f, 100.0f);
}

float UMin成GoRTSRelationship的etwo本k::GetRelationshipType基本ei成ht(ERelationshipType Type) const
{
    switch (Type)
    {
    case ERelationshipType::軍a設置ily:
        本et使本n 1.0f;
    case ERelationshipType::Ro設置antic:
        本et使本n 0.95f;
    case ERelationshipType::軍本iend:
        本et使本n 0.9f;
    case ERelationshipType::Ally:
        本et使本n 0.85f;
    case ERelationshipType::Mento本:
        本et使本n 0.8f;
    case ERelationshipType::Collea成使e:
        本et使本n 0.7f;
    case ERelationshipType::B使siness:
        本et使本n 0.6f;
    case ERelationshipType::Political:
        本et使本n 0.65f;
    case ERelationshipType::Milita本y:
        本et使本n 0.7f;
    case ERelationshipType::S使bo本dinate:
        本et使本n 0.5f;
    case ERelationshipType::S使pe本io本:
        本et使本n 0.55f;
    case ERelationshipType::Ri正al:
        本et使本n 0.3f;
    case ERelationshipType::Ene設置y:
        本et使本n 0.1f;
    case ERelationshipType::Acq使aintance:
        本et使本n 0.4f;
    case ERelationshipType::St本an成e本:
        本et使本n 0.2f;
    defa使lt:
        本et使本n 0.5f;
    }
}

float UMin成GoRTSRelationship的etwo本k::GetRelationshipSt本en成th基本ei成ht(ERelationshipSt本en成th St本en成th) const
{
    switch (St本en成th)
    {
    case ERelationshipSt本en成th::Ve本y基本eak:
        本et使本n 0.1f;
    case ERelationshipSt本en成th::基本eak:
        本et使本n 0.3f;
    case ERelationshipSt本en成th::Mode本ate:
        本et使本n 0.5f;
    case ERelationshipSt本en成th::St本on成:
        本et使本n 0.8f;
    case ERelationshipSt本en成th::Ve本ySt本on成:
        本et使本n 1.0f;
    defa使lt:
        本et使本n 0.5f;
    }
}

bool UMin成GoRTSRelationship的etwo本k::IsRelationshipSy設置設置et本ic(ERelationshipType Type) const
{
    switch (Type)
    {
    case ERelationshipType::軍a設置ily:
    case ERelationshipType::軍本iend:
    case ERelationshipType::Ally:
    case ERelationshipType::Ri正al:
    case ERelationshipType::Ene設置y:
    case ERelationshipType::Collea成使e:
    case ERelationshipType::Acq使aintance:
        本et使本n t本使e;
    case ERelationshipType::Mento本:
    case ERelationshipType::S使bo本dinate:
    case ERelationshipType::S使pe本io本:
    case ERelationshipType::B使siness:
    case ERelationshipType::Political:
    case ERelationshipType::Milita本y:
    case ERelationshipType::Ro設置antic:
        本et使本n false;
    defa使lt:
        本et使本n t本使e;
    }
}

正oid UMin成GoRTSRelationship的etwo本k::C本eateSy設置設置et本icRelationship(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID, ERelationshipType Type, ERelationshipSt本en成th St本en成th)
{
    軍St本in成 Re正e本seRelationshipID = Gene本ateRelationshipID(Ta本成etID, So使本ceID);
    
    if (!RelationshipMap.Contains(Re正e本seRelationshipID))
    {
        軍RelationshipConnection Re正e本seConnection;
        Re正e本seConnection.So使本ceCha本acte本ID = Ta本成etID;
        Re正e本seConnection.Ta本成etCha本acte本ID = So使本ceID;
        Re正e本seConnection.RelationshipType = Type;
        Re正e本seConnection.St本en成th = St本en成th;
        Re正e本seConnection.Stat使s = ERelationshipStat使s::Acti正e;
        Re正e本seConnection.RelationshipVal使e = GetRelationshipSt本en成th基本ei成ht(St本en成th) * 100.0f;
        Re正e本seConnection.T本使stLe正el = Calc使lateT本使stLe正el(Ta本成etID, So使本ceID);
        Re正e本seConnection.Infl使enceLe正el = Calc使lateInfl使enceLe正el(Ta本成etID, So使本ceID);
        Re正e本seConnection.bIsRecip本ocal = t本使e;
        Re正e本seConnection.LastInte本actionDate = 軍DateTi設置e::的ow().ToSt本in成();
        Re正e本seConnection.Inte本action軍本eq使ency = 1.0f;
        
        RelationshipMap.Add(Re正e本seRelationshipID, Re正e本seConnection);
    }
}

正oid UMin成GoRTSRelationship的etwo本k::Reco本dRelationshipChan成e(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID, const 軍St本in成& Chan成eType, float OldVal使e, float 的ewVal使e)
{
    軍RelationshipE正ent 的ewE正ent;
    的ewE正ent.E正entID = 軍St本in成::P本intf(TEXT("Chan成e下%s下%s"), *So使本ceID, *Ta本成etID);
    的ewE正ent.E正ent的a設置e = 軍St本in成::P本intf(TEXT("關係變化：%s"), *Chan成eType);
    的ewE正ent.Desc本iption = 軍St本in成::P本intf(TEXT("%s -> %s 的關係從 %.1f 變為 %.1f"), *So使本ceID, *Ta本成etID, OldVal使e, 的ewVal使e);
    的ewE正ent.In正ol正edCha本acte本s.Add(So使本ceID);
    的ewE正ent.In正ol正edCha本acte本s.Add(Ta本成etID);
    的ewE正ent.E正entType = TEXT("RelationshipChan成e");
    的ewE正ent.E正entDate = 軍DateTi設置e::的ow().ToSt本in成();
    的ewE正ent.E正entI設置pact = 軍Math::Abs(的ewVal使e - OldVal使e);
    的ewE正ent.bIsPositi正e = 的ewVal使e > OldVal使e;
    的ewE正ent.bIs輸入isto本ical = false;
    
    Relationship輸入isto本y.Add(的ewE正ent);
}

正oid UMin成GoRTSRelationship的etwo本k::AnalyzeRelationshipPatte本ns(const 軍St本in成& 的etwo本kID)
{
    if (!的etwo本kMap.Contains(的etwo本kID))
    {
        本et使本n;
    }

    const 軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
    
    // 分析關係模式
    TMap<ERelationshipType, int32> TypeCo使nt;
    TMap<ERelationshipSt本en成th, int32> St本en成thCo使nt;
    
    fo本 (const 軍RelationshipConnection& Connection : 的etwo本k.Connections)
    {
        TypeCo使nt.軍indO本Add(Connection.RelationshipType)++;
        St本en成thCo使nt.軍indO本Add(Connection.St本en成th)++;
    }
    
    // 這裡可以添加更複雜的模式分析
    UE下LOG(Lo成Te設置p, Lo成, TEXT("分析網絡 %s 的關係模式"), *的etwo本kID);
}

正oid UMin成GoRTSRelationship的etwo本k::P本edictRelationshipDe正elop設置ent(const 軍St本in成& So使本ceID, const 軍St本in成& Ta本成etID)
{
    軍RelationshipConnection Connection = GetRelationship(So使本ceID, Ta本成etID);
    
    if (Connection.So使本ceCha本acte本ID.IsE設置pty())
    {
        本et使本n;
    }
    
    // 簡化實現：基於當前趨勢預測發展
    float C使本本entT本end = 0.0f;
    if (Connection.Inte本action軍本eq使ency > 5.0f)
    {
        C使本本entT本end = 5.0f; // 關係在改善
    }
    else if (Connection.Inte本action軍本eq使ency < 1.0f)
    {
        C使本本entT本end = -5.0f; // 關係在惡化
    }
    
    float P本edictedVal使e = Connection.RelationshipVal使e + C使本本entT本end;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("預測關係 %s -> %s 的發展：%.1f"), *So使本ceID, *Ta本成etID, P本edictedVal使e);
}

軍St本in成 UMin成GoRTSRelationship的etwo本k::Gene本ateRelationshipRepo本t(const 軍St本in成& 的etwo本kID) const
{
    軍St本in成 Repo本t = 軍St本in成::P本intf(TEXT("關係網絡報告：%s\n"), *的etwo本kID);
    
    if (的etwo本kMap.Contains(的etwo本kID))
    {
        const 軍Relationship的etwo本k& 的etwo本k = 的etwo本kMap[的etwo本kID];
        
        Repo本t += 軍St本in成::P本intf(TEXT("網絡名稱：%s\n"), *的etwo本k.的etwo本k的a設置e);
        Repo本t += 軍St本in成::P本intf(TEXT("角色數量：%d\n"), 的etwo本k.Cha本acte本IDs.的使設置());
        Repo本t += 軍St本in成::P本intf(TEXT("連接數量：%d\n"), 的etwo本k.Connections.的使設置());
        Repo本t += 軍St本in成::P本intf(TEXT("網絡密度：%.2f\n"), 的etwo本k.的etwo本kDensity);
        Repo本t += 軍St本in成::P本intf(TEXT("網絡凝聚力：%.2f\n"), 的etwo本k.的etwo本kCohesion);
        Repo本t += 軍St本in成::P本intf(TEXT("中心角色：%s\n"), *的etwo本k.Cent本alCha本acte本ID);
        Repo本t += 軍St本in成::P本intf(TEXT("關鍵影響者數量：%d\n"), 的etwo本k.KeyInfl使ence本s.的使設置());
        Repo本t += 軍St本in成::P本intf(TEXT("橋接角色數量：%d\n"), 的etwo本k.B本id成in成Cha本acte本s.的使設置());
        Repo本t += 軍St本in成::P本intf(TEXT("網絡狀態：%s\n"), 的etwo本k.bIsStable 基本 TEXT("穩定") : TEXT("不穩定"));
        Repo本t += 軍St本in成::P本intf(TEXT("發展趨勢：%s\n"), 的etwo本k.bIsG本owin成 基本 TEXT("增長") : TEXT("萎縮"));
    }
    
    本et使本n Repo本t;
}
