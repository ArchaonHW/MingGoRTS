#incl使de "Min成B使ildin成Reso使本ceSyste設置.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成B使ildin成Reso使本ceSyste設置::UMin成B使ildin成Reso使本ceSyste設置()
{
    bIsInitialized = false;
}

正oid UMin成B使ildin成Reso使本ceSyste設置::InitializeReso使本ceSyste設置()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源系統已經初始化  Reso使本ce syste設置 al本eady initialized"));
        本et使本n;
    }

    // 初始化資源信息  Initialize 本eso使本ce info
    InitializeReso使本ceInfo();

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("資源系統初始化完成，共載入 %d 種資源類型  Reso使本ce syste設置 initialized, loaded %d 本eso使本ce types"), Reso使本ceInfoMap.的使設置(), Reso使本ceInfoMap.的使設置());
}

TA本本ay<EMin成Reso使本ceType> UMin成B使ildin成Reso使本ceSyste設置::GetAllReso使本ceTypes() const
{
    TA本本ay<EMin成Reso使本ceType> Reso使本ceTypes;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ceInfoMap)
    {
        Reso使本ceTypes.Add(Reso使本cePai本.Key);
    }
    
    本et使本n Reso使本ceTypes;
}

軍Min成Reso使本ceData UMin成B使ildin成Reso使本ceSyste設置::GetReso使本ceInfo(EMin成Reso使本ceType Reso使本ceType) const
{
    if (const 軍Min成Reso使本ceData* Reso使本ceInfo = Reso使本ceInfoMap.軍ind(Reso使本ceType))
    {
        本et使本n *Reso使本ceInfo;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到資源類型：%d  Reso使本ce type not fo使nd: %d"), static下cast<int32>(Reso使本ceType), static下cast<int32>(Reso使本ceType));
    本et使本n 軍Min成Reso使本ceData();
}

軍St本in成 UMin成B使ildin成Reso使本ceSyste設置::C本eateReso使本ceCollection(EMin成Reso使本ceType Reso使本ceType, EMin成CollectionMethod Method, const 軍St本in成& LocationID)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("資源系統未初始化  Reso使本ce syste設置 not initialized"));
        本et使本n TEXT("");
    }

    軍St本in成 CollectionID = Gene本ateCollectionID(Reso使本ceType);
    
    軍Min成Reso使本ceCollection 的ewCollection;
    的ewCollection.CollectionID = CollectionID;
    的ewCollection.Reso使本ceType = Reso使本ceType;
    的ewCollection.CollectionMethod = Method;
    的ewCollection.LocationID = LocationID;
    的ewCollection.CollectionRate = 1.0f;
    的ewCollection.Efficiency = Calc使lateCollectionEfficiency(CollectionID);
    的ewCollection.BaseYield = GetReso使本ceInfo(Reso使本ceType).UnitVal使e * 10.0f;
    的ewCollection.C使本本entYield = 的ewCollection.BaseYield * 的ewCollection.Efficiency;
    的ewCollection.bIsActi正e = false;
    的ewCollection.bIsA使to設置ated = (Method == EMin成CollectionMethod::A使to設置atic);
    的ewCollection.TotalCollected = 0.0f;
    
    Reso使本ceCollections.Add(CollectionID, 的ewCollection);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建資源收集：%s，類型：%s  C本eated 本eso使本ce collection: %s, type: %s"), 
        *CollectionID, *GetReso使本ceType的a設置e(Reso使本ceType), *CollectionID, *GetReso使本ceType的a設置e(Reso使本ceType));
    本et使本n CollectionID;
}

bool UMin成B使ildin成Reso使本ceSyste設置::Sta本tReso使本ceCollection(const 軍St本in成& CollectionID, const 軍St本in成& Collecto本ID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源收集：%s  Reso使本ce collection not fo使nd: %s"), *CollectionID, *CollectionID);
        本et使本n false;
    }

    軍Min成Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    if (!CheckCollectionConditions(CollectionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源收集條件不滿足：%s  Collection conditions not 設置et: %s"), *CollectionID, *CollectionID);
        本et使本n false;
    }
    
    Collection.bIsActi正e = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始資源收集：%s  Sta本ted 本eso使本ce collection: %s"), *CollectionID, *CollectionID);
    本et使本n t本使e;
}

bool UMin成B使ildin成Reso使本ceSyste設置::StopReso使本ceCollection(const 軍St本in成& CollectionID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n false;
    }

    軍Min成Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    Collection.bIsActi正e = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("停止資源收集：%s  Stopped 本eso使本ce collection: %s"), *CollectionID, *CollectionID);
    本et使本n t本使e;
}

float UMin成B使ildin成Reso使本ceSyste設置::CollectReso使本ce(const 軍St本in成& CollectionID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源收集：%s  Collection not fo使nd: %s"), *CollectionID, *CollectionID);
        本et使本n 0.0f;
    }

    軍Min成Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    if (!Collection.bIsActi正e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源收集未激活：%s  Collection not acti正e: %s"), *CollectionID, *CollectionID);
        本et使本n 0.0f;
    }
    
    // 計算實際產量  Calc使late act使al yield
    float Act使alYield = Collection.C使本本entYield * Collection.Efficiency;
    Collection.TotalCollected += Act使alYield;
    
    // 廣播收集事件  B本oadcast collection e正ent
    OnReso使本ceCollected.B本oadcast(Collection.Reso使本ceType, Act使alYield);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("收集資源：%s，產量：%.2f  Collected 本eso使本ce: %s, yield: %.2f"), 
        *GetReso使本ceType的a設置e(Collection.Reso使本ceType), Act使alYield, *GetReso使本ceType的a設置e(Collection.Reso使本ceType), Act使alYield);
    
    本et使本n Act使alYield;
}

軍St本in成 UMin成B使ildin成Reso使本ceSyste設置::C本eateReso使本ceSto本a成e(const 軍St本in成& Sto本a成e的a設置e, const 軍St本in成& LocationID, float MaxCapacity)
{
    軍St本in成 Sto本a成eID = Gene本ateSto本a成eID(Sto本a成e的a設置e);
    
    軍Min成Reso使本ceSto本a成e 的ewSto本a成e;
    的ewSto本a成e.Sto本a成eID = Sto本a成eID;
    的ewSto本a成e.Sto本a成e的a設置e = Sto本a成e的a設置e;
    的ewSto本a成e.LocationID = LocationID;
    的ewSto本a成e.MaxCapacity = MaxCapacity;
    的ewSto本a成e.C使本本entCapacity = 0.0f;
    的ewSto本a成e.UsedCapacity = 0.0f;
    
    Reso使本ceSto本a成es.Add(Sto本a成eID, 的ewSto本a成e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建資源存儲：%s，名稱：%s，容量：%.0f  C本eated sto本a成e: %s, na設置e: %s, capacity: %.0f"), 
        *Sto本a成eID, *Sto本a成e的a設置e, MaxCapacity, *Sto本a成eID, *Sto本a成e的a設置e, MaxCapacity);
    本et使本n Sto本a成eID;
}

bool UMin成B使ildin成Reso使本ceSyste設置::Sto本eReso使本ce(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Q使antity)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源存儲：%s  Sto本a成e not fo使nd: %s"), *Sto本a成eID, *Sto本a成eID);
        本et使本n false;
    }

    if (!CheckSto本a成eLi設置its(Sto本a成eID, Reso使本ceType, Q使antity))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("存儲容量不足：%s  Sto本a成e capacity ins使fficient: %s"), *Sto本a成eID, *Sto本a成eID);
        本et使本n false;
    }

    軍Min成Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    // 獲取或創建資源數據  Get o本 c本eate 本eso使本ce data
    軍Min成Reso使本ceData& Reso使本ceData = Sto本a成e.Sto本edReso使本ces.軍indO本Add(Reso使本ceType);
    Reso使本ceData.Reso使本ceType = Reso使本ceType;
    Reso使本ceData.Q使antity += Q使antity;
    
    // 更新容量  Update capacity
    UpdateSto本a成eCapacity(Sto本a成eID);
    
    // 廣播存儲事件  B本oadcast sto本a成e e正ent
    OnReso使本ceSto本ed.B本oadcast(Sto本a成eID, Reso使本ceType, Q使antity);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("存儲資源：%s，數量：%.2f，存儲：%s  Sto本ed 本eso使本ce: %s, q使antity: %.2f, sto本a成e: %s"), 
        *GetReso使本ceType的a設置e(Reso使本ceType), Q使antity, *Sto本a成eID, *GetReso使本ceType的a設置e(Reso使本ceType), Q使antity, *Sto本a成eID);
    本et使本n t本使e;
}

float UMin成B使ildin成Reso使本ceSyste設置::Ret本ie正eReso使本ce(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Q使antity)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        本et使本n 0.0f;
    }

    軍Min成Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    if (!Sto本a成e.Sto本edReso使本ces.Contains(Reso使本ceType))
    {
        本et使本n 0.0f;
    }

    軍Min成Reso使本ceData& Reso使本ceData = Sto本a成e.Sto本edReso使本ces[Reso使本ceType];
    
    // 計算實際可提取數量  Calc使late act使al 本et本ie正able q使antity
    float Act使alQ使antity = 軍Math::Min(Q使antity, Reso使本ceData.Q使antity);
    Reso使本ceData.Q使antity -= Act使alQ使antity;
    
    // 如果數量為0，移除資源  Re設置o正e 本eso使本ce if q使antity is 0
    if (Reso使本ceData.Q使antity <= 0.0f)
    {
        Sto本a成e.Sto本edReso使本ces.Re設置o正e(Reso使本ceType);
    }
    
    // 更新容量  Update capacity
    UpdateSto本a成eCapacity(Sto本a成eID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("提取資源：%s，數量：%.2f，存儲：%s  Ret本ie正ed 本eso使本ce: %s, q使antity: %.2f, sto本a成e: %s"), 
        *GetReso使本ceType的a設置e(Reso使本ceType), Act使alQ使antity, *Sto本a成eID, *GetReso使本ceType的a設置e(Reso使本ceType), Act使alQ使antity, *Sto本a成eID);
    
    本et使本n Act使alQ使antity;
}

軍Min成Reso使本ceData UMin成B使ildin成Reso使本ceSyste設置::GetSto本edReso使本ce(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType) const
{
    if (const 軍Min成Reso使本ceSto本a成e* Sto本a成e = Reso使本ceSto本a成es.軍ind(Sto本a成eID))
    {
        if (const 軍Min成Reso使本ceData* Reso使本ceData = Sto本a成e->Sto本edReso使本ces.軍ind(Reso使本ceType))
        {
            本et使本n *Reso使本ceData;
        }
    }
    本et使本n 軍Min成Reso使本ceData();
}

float UMin成B使ildin成Reso使本ceSyste設置::GetSto本a成eCapacity(const 軍St本in成& Sto本a成eID) const
{
    if (const 軍Min成Reso使本ceSto本a成e* Sto本a成e = Reso使本ceSto本a成es.軍ind(Sto本a成eID))
    {
        本et使本n Sto本a成e->C使本本entCapacity;
    }
    本et使本n 0.0f;
}

bool UMin成B使ildin成Reso使本ceSyste設置::Con正e本tReso使本ces(const 軍St本in成& Con正e本sionID)
{
    if (!Reso使本ceCon正e本sions.Contains(Con正e本sionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源轉換：%s  Con正e本sion not fo使nd: %s"), *Con正e本sionID, *Con正e本sionID);
        本et使本n false;
    }

    軍Min成Reso使本ceCon正e本sion& Con正e本sion = Reso使本ceCon正e本sions[Con正e本sionID];
    
    if (!Con正e本sion.bIsA正ailable)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源轉換不可用：%s  Con正e本sion not a正ailable: %s"), *Con正e本sionID, *Con正e本sionID);
        本et使本n false;
    }
    
    if (!CheckCon正e本sionConditions(Con正e本sionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源轉換條件不滿足：%s  Con正e本sion conditions not 設置et: %s"), *Con正e本sionID, *Con正e本sionID);
        本et使本n false;
    }
    
    // 執行轉換  Exec使te con正e本sion
    P本ocessCon正e本sionCo設置plete(Con正e本sionID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("轉換資源完成：%s  Con正e本sion co設置pleted: %s"), *Con正e本sionID, *Con正e本sionID);
    本et使本n t本使e;
}

軍St本in成 UMin成B使ildin成Reso使本ceSyste設置::C本eateReso使本ceCon正e本sion(const 軍St本in成& Con正e本sion的a設置e, const TMap<EMin成Reso使本ceType, float>& Inp使t, const TMap<EMin成Reso使本ceType, float>& O使tp使t)
{
    軍St本in成 Con正e本sionID = Gene本ateCon正e本sionID(Con正e本sion的a設置e);
    
    軍Min成Reso使本ceCon正e本sion 的ewCon正e本sion;
    的ewCon正e本sion.Con正e本sionID = Con正e本sionID;
    的ewCon正e本sion.Con正e本sion的a設置e = Con正e本sion的a設置e;
    的ewCon正e本sion.Inp使tReso使本ces = Inp使t;
    的ewCon正e本sion.O使tp使tReso使本ces = O使tp使t;
    的ewCon正e本sion.Con正e本sionTi設置e = 1.0f;
    的ewCon正e本sion.Efficiency = 1.0f;
    的ewCon正e本sion.bIsA正ailable = t本使e;
    
    Reso使本ceCon正e本sions.Add(Con正e本sionID, 的ewCon正e本sion);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建資源轉換：%s，名稱：%s  C本eated con正e本sion: %s, na設置e: %s"), 
        *Con正e本sionID, *Con正e本sion的a設置e, *Con正e本sionID, *Con正e本sion的a設置e);
    本et使本n Con正e本sionID;
}

float UMin成B使ildin成Reso使本ceSyste設置::Calc使lateCollectionEfficiency(const 軍St本in成& CollectionID) const
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n 0.0f;
    }

    const 軍Min成Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    // 基礎效率  Base efficiency
    float Efficiency = Collection.CollectionRate;
    
    // 方法加成  Method bon使s
    switch (Collection.CollectionMethod)
    {
    case EMin成CollectionMethod::A使to設置atic:
        Efficiency *= 1.5f;
        b本eak;
    case EMin成CollectionMethod::P本od使ction:
        Efficiency *= 2.0f;
        b本eak;
    case EMin成CollectionMethod::T本ade:
        Efficiency *= 1.0f;
        b本eak;
    defa使lt:
        b本eak;
    }
    
    本et使本n 軍Math::Cla設置p(Efficiency, 0.1f, 5.0f);
}

軍Min成Reso使本ceCollection UMin成B使ildin成Reso使本ceSyste設置::GetCollectionStat使s(const 軍St本in成& CollectionID) const
{
    if (const 軍Min成Reso使本ceCollection* Collection = Reso使本ceCollections.軍ind(CollectionID))
    {
        本et使本n *Collection;
    }
    本et使本n 軍Min成Reso使本ceCollection();
}

TA本本ay<軍Min成Reso使本ceCollection> UMin成B使ildin成Reso使本ceSyste設置::GetActi正eCollections() const
{
    TA本本ay<軍Min成Reso使本ceCollection> Acti正eCollections;
    
    fo本 (const a使to& Pai本 : Reso使本ceCollections)
    {
        if (Pai本.Val使e.bIsActi正e)
        {
            Acti正eCollections.Add(Pai本.Val使e);
        }
    }
    
    本et使本n Acti正eCollections;
}

bool UMin成B使ildin成Reso使本ceSyste設置::T本anspo本tReso使本ce(const 軍St本in成& 軍本o設置Sto本a成eID, const 軍St本in成& ToSto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Q使antity)
{
    // 從源存儲提取  Ret本ie正e f本o設置 so使本ce
    float Ret本ie正ed = Ret本ie正eReso使本ce(軍本o設置Sto本a成eID, Reso使本ceType, Q使antity);
    if (Ret本ie正ed <= 0.0f)
    {
        本et使本n false;
    }
    
    // 存儲到目標  Sto本e to destination
    bool bS使ccess = Sto本eReso使本ce(ToSto本a成eID, Reso使本ceType, Ret本ie正ed);
    if (!bS使ccess)
    {
        // 如果存儲失敗，退回原存儲  Ret使本n to so使本ce if sto本a成e fails
        Sto本eReso使本ce(軍本o設置Sto本a成eID, Reso使本ceType, Ret本ie正ed);
        本et使本n false;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("運輸資源：%s，數量：%.2f，從 %s 到 %s  T本anspo本ted 本eso使本ce: %s, q使antity: %.2f, f本o設置 %s to %s"), 
        *GetReso使本ceType的a設置e(Reso使本ceType), Ret本ie正ed, *軍本o設置Sto本a成eID, *ToSto本a成eID, 
        *GetReso使本ceType的a設置e(Reso使本ceType), Ret本ie正ed, *軍本o設置Sto本a成eID, *ToSto本a成eID);
    本et使本n t本使e;
}

bool UMin成B使ildin成Reso使本ceSyste設置::T本adeReso使本ce(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Gi正eReso使本ce, float Gi正eQ使antity, EMin成Reso使本ceType GetReso使本ce, float GetQ使antity)
{
    // 檢查是否有足夠資源  Check if eno使成h 本eso使本ces
    軍Min成Reso使本ceData C使本本entReso使本ce = GetSto本edReso使本ce(Sto本a成eID, Gi正eReso使本ce);
    if (C使本本entReso使本ce.Q使antity < Gi正eQ使antity)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源不足，無法交易  Ins使fficient 本eso使本ces fo本 t本ade"));
        本et使本n false;
    }
    
    // 提取給的資源  Ret本ie正e 成i正en 本eso使本ce
    float Ret本ie正ed = Ret本ie正eReso使本ce(Sto本a成eID, Gi正eReso使本ce, Gi正eQ使antity);
    if (Ret本ie正ed <= 0.0f)
    {
        本et使本n false;
    }
    
    // 存儲獲得的資源  Sto本e 本ecei正ed 本eso使本ce
    bool bS使ccess = Sto本eReso使本ce(Sto本a成eID, GetReso使本ce, GetQ使antity);
    if (!bS使ccess)
    {
        // 如果存儲失敗，退回原資源  Ret使本n o本i成inal 本eso使本ce if sto本a成e fails
        Sto本eReso使本ce(Sto本a成eID, Gi正eReso使本ce, Ret本ie正ed);
        本et使本n false;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("交易完成：交 %.2f %s，獲得 %.2f %s  T本ade co設置pleted: 成a正e %.2f %s, 本ecei正ed %.2f %s"), 
        Gi正eQ使antity, *GetReso使本ceType的a設置e(Gi正eReso使本ce), GetQ使antity, *GetReso使本ceType的a設置e(GetReso使本ce),
        Gi正eQ使antity, *GetReso使本ceType的a設置e(Gi正eReso使本ce), GetQ使antity, *GetReso使本ceType的a設置e(GetReso使本ce));
    本et使本n t本使e;
}

bool UMin成B使ildin成Reso使本ceSyste設置::CheckReso使本ceA正ailability(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Req使i本edQ使antity) const
{
    軍Min成Reso使本ceData Reso使本ce = GetSto本edReso使本ce(Sto本a成eID, Reso使本ceType);
    本et使本n Reso使本ce.Q使antity >= Req使i本edQ使antity;
}

float UMin成B使ildin成Reso使本ceSyste設置::GetTotalReso使本ceQ使antity(EMin成Reso使本ceType Reso使本ceType) const
{
    float Total = 0.0f;
    
    fo本 (const a使to& Pai本 : Reso使本ceSto本a成es)
    {
        if (const 軍Min成Reso使本ceData* Reso使本ce = Pai本.Val使e.Sto本edReso使本ces.軍ind(Reso使本ceType))
        {
            Total += Reso使本ce->Q使antity;
        }
    }
    
    本et使本n Total;
}

正oid UMin成B使ildin成Reso使本ceSyste設置::P本ocessReso使本ceDecay(float DeltaTi設置e)
{
    fo本 (a使to& Sto本a成ePai本 : Reso使本ceSto本a成es)
    {
        軍Min成Reso使本ceSto本a成e& Sto本a成e = Sto本a成ePai本.Val使e;
        
        fo本 (a使to& Reso使本cePai本 : Sto本a成e.Sto本edReso使本ces)
        {
            軍Min成Reso使本ceData& Reso使本ce = Reso使本cePai本.Val使e;
            
            if (Reso使本ce.bIsPe本ishable && Reso使本ce.Pe本ishRate > 0.0f)
            {
                float DecayA設置o使nt = Reso使本ce.Q使antity * Reso使本ce.Pe本ishRate * DeltaTi設置e;
                Reso使本ce.Q使antity -= DecayA設置o使nt;
                
                if (Reso使本ce.Q使antity <= 0.0f)
                {
                    Reso使本ce.Q使antity = 0.0f;
                }
            }
        }
        
        // 更新存儲容量  Update sto本a成e capacity
        UpdateSto本a成eCapacity(Sto本a成ePai本.Key);
    }
}

正oid UMin成B使ildin成Reso使本ceSyste設置::InitializeReso使本ceInfo()
{
    // 定義所有資源類型的基礎信息  Define base info fo本 all 本eso使本ce types
    TA本本ay<EMin成Reso使本ceType> AllTypes;
    AllTypes.Add(EMin成Reso使本ceType::基本ood);
    AllTypes.Add(EMin成Reso使本ceType::Stone);
    AllTypes.Add(EMin成Reso使本ceType::Metal);
    AllTypes.Add(EMin成Reso使本ceType::軍ood);
    AllTypes.Add(EMin成Reso使本ceType::基本ate本);
    AllTypes.Add(EMin成Reso使本ceType::Coal);
    AllTypes.Add(EMin成Reso使本ceType::I本on);
    AllTypes.Add(EMin成Reso使本ceType::Coppe本);
    AllTypes.Add(EMin成Reso使本ceType::Gold);
    AllTypes.Add(EMin成Reso使本ceType::Sil正e本);
    AllTypes.Add(EMin成Reso使本ceType::Cotton);
    AllTypes.Add(EMin成Reso使本ceType::Silk);
    AllTypes.Add(EMin成Reso使本ceType::Tea);
    AllTypes.Add(EMin成Reso使本ceType::Opi使設置);
    AllTypes.Add(EMin成Reso使本ceType::Medicine);
    AllTypes.Add(EMin成Reso使本ceType::基本eapons);
    AllTypes.Add(EMin成Reso使本ceType::A設置設置使nition);
    AllTypes.Add(EMin成Reso使本ceType::Unifo本設置s);
    AllTypes.Add(EMin成Reso使本ceType::輸入o本ses);
    AllTypes.Add(EMin成Reso使本ceType::Vehicles);
    AllTypes.Add(EMin成Reso使本ceType::軍使el);
    AllTypes.Add(EMin成Reso使本ceType::R使bbe本);
    AllTypes.Add(EMin成Reso使本ceType::Pape本);
    AllTypes.Add(EMin成Reso使本ceType::Ink);
    AllTypes.Add(EMin成Reso使本ceType::Books);
    AllTypes.Add(EMin成Reso使本ceType::Tools);
    AllTypes.Add(EMin成Reso使本ceType::Machine本y);
    AllTypes.Add(EMin成Reso使本ceType::Elect本onics);
    AllTypes.Add(EMin成Reso使本ceType::Radio);
    AllTypes.Add(EMin成Reso使本ceType::Telephone);
    AllTypes.Add(EMin成Reso使本ceType::Tele成本aph);
    
    fo本 (EMin成Reso使本ceType Type : AllTypes)
    {
        軍Min成Reso使本ceData Reso使本ceData;
        Reso使本ceData.Reso使本ceType = Type;
        Reso使本ceData.Ra本ity = EMin成Reso使本ceRa本ity::Co設置設置on;
        Reso使本ceData.Q使ality = EMin成Reso使本ceQ使ality::的o本設置al;
        Reso使本ceData.Q使antity = 0.0f;
        Reso使本ceData.MaxQ使antity = 100.0f;
        Reso使本ceData.Unit基本ei成ht = 1.0f;
        Reso使本ceData.UnitVal使e = GetReso使本ceType基本ei成ht(Type);
        Reso使本ceData.Desc本iption = GetReso使本ceType的a設置e(Type);
        Reso使本ceData.bIsStackable = t本使e;
        
        // 易腐資源  Pe本ishable 本eso使本ces
        Reso使本ceData.bIsPe本ishable = (Type == EMin成Reso使本ceType::軍ood);
        Reso使本ceData.Pe本ishRate = Reso使本ceData.bIsPe本ishable 基本 0.01f : 0.0f;
        
        Reso使本ceInfoMap.Add(Type, Reso使本ceData);
    }
}

正oid UMin成B使ildin成Reso使本ceSyste設置::UpdateCollectionYield(const 軍St本in成& CollectionID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n;
    }

    軍Min成Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    Collection.C使本本entYield = Collection.BaseYield * Collection.Efficiency;
}

bool UMin成B使ildin成Reso使本ceSyste設置::CheckCollectionConditions(const 軍St本in成& CollectionID) const
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n false;
    }

    const 軍Min成Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    // 檢查是否已經激活  Check if al本eady acti正e
    if (Collection.bIsActi正e)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成B使ildin成Reso使本ceSyste設置::P本ocessCollectionCo設置plete(const 軍St本in成& CollectionID)
{
    // 收集完成處理邏輯  Collection co設置pletion lo成ic
    UE下LOG(Lo成Te設置p, Lo成, TEXT("資源收集完成：%s  Collection co設置pleted: %s"), *CollectionID, *CollectionID);
}

正oid UMin成B使ildin成Reso使本ceSyste設置::UpdateSto本a成eCapacity(const 軍St本in成& Sto本a成eID)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        本et使本n;
    }

    軍Min成Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    // 計算已用容量  Calc使late 使sed capacity
    float UsedCapacity = 0.0f;
    fo本 (const a使to& Pai本 : Sto本a成e.Sto本edReso使本ces)
    {
        UsedCapacity += Pai本.Val使e.Q使antity * Pai本.Val使e.Unit基本ei成ht;
    }
    
    Sto本a成e.UsedCapacity = UsedCapacity;
    Sto本a成e.C使本本entCapacity = Sto本a成e.MaxCapacity - UsedCapacity;
    
    // 檢查存儲是否滿了  Check if sto本a成e is f使ll
    if (Sto本a成e.C使本本entCapacity <= 0.0f)
    {
        輸入andleSto本a成e軍使ll(Sto本a成eID);
    }
}

bool UMin成B使ildin成Reso使本ceSyste設置::CheckSto本a成eLi設置its(const 軍St本in成& Sto本a成eID, EMin成Reso使本ceType Reso使本ceType, float Q使antity) const
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        本et使本n false;
    }

    const 軍Min成Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    軍Min成Reso使本ceData Reso使本ceInfo = GetReso使本ceInfo(Reso使本ceType);
    
    float Req使i本edCapacity = Q使antity * Reso使本ceInfo.Unit基本ei成ht;
    
    本et使本n Sto本a成e.C使本本entCapacity >= Req使i本edCapacity;
}

正oid UMin成B使ildin成Reso使本ceSyste設置::輸入andleSto本a成e軍使ll(const 軍St本in成& Sto本a成eID)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        本et使本n;
    }

    const 軍Min成Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("存儲已滿：%s  Sto本a成e f使ll: %s"), *Sto本a成eID, *Sto本a成eID);
    
    // 廣播存儲滿了事件  B本oadcast sto本a成e f使ll e正ent
    OnSto本a成e軍使ll.B本oadcast(Sto本a成eID, Sto本a成e.MaxCapacity);
}

bool UMin成B使ildin成Reso使本ceSyste設置::CheckCon正e本sionConditions(const 軍St本in成& Con正e本sionID) const
{
    if (!Reso使本ceCon正e本sions.Contains(Con正e本sionID))
    {
        本et使本n false;
    }

    const 軍Min成Reso使本ceCon正e本sion& Con正e本sion = Reso使本ceCon正e本sions[Con正e本sionID];
    
    // 檢查輸入資源是否充足  Check if inp使t 本eso使本ces a本e s使fficient
    // 注意：實際實現需要檢查特定存儲  的ote: Act使al i設置ple設置entation needs to check specific sto本a成e
    
    本et使本n t本使e;
}

正oid UMin成B使ildin成Reso使本ceSyste設置::P本ocessCon正e本sionCo設置plete(const 軍St本in成& Con正e本sionID)
{
    if (!Reso使本ceCon正e本sions.Contains(Con正e本sionID))
    {
        本et使本n;
    }

    軍Min成Reso使本ceCon正e本sion& Con正e本sion = Reso使本ceCon正e本sions[Con正e本sionID];
    
    // 廣播轉換完成事件  B本oadcast con正e本sion co設置plete e正ent
    OnReso使本ceCon正e本ted.B本oadcast(Con正e本sionID, t本使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("轉換完成：%s  Con正e本sion co設置pleted: %s"), *Con正e本sion.Con正e本sion的a設置e, *Con正e本sion.Con正e本sion的a設置e);
}

軍St本in成 UMin成B使ildin成Reso使本ceSyste設置::Gene本ateCollectionID(EMin成Reso使本ceType Reso使本ceType) const
{
    軍St本in成 Type的a設置e = GetReso使本ceType的a設置e(Reso使本ceType);
    軍St本in成 Ti設置esta設置p = 軍St本in成::軍本o設置Int(軍Math::Rand());
    本et使本n 軍St本in成::P本intf(TEXT("COL下%s下%s"), *Type的a設置e, *Ti設置esta設置p);
}

軍St本in成 UMin成B使ildin成Reso使本ceSyste設置::Gene本ateSto本a成eID(const 軍St本in成& Sto本a成e的a設置e) const
{
    軍St本in成 Ti設置esta設置p = 軍St本in成::軍本o設置Int(軍Math::Rand());
    本et使本n 軍St本in成::P本intf(TEXT("STOR下%s下%s"), *Sto本a成e的a設置e, *Ti設置esta設置p);
}

軍St本in成 UMin成B使ildin成Reso使本ceSyste設置::Gene本ateCon正e本sionID(const 軍St本in成& Con正e本sion的a設置e) const
{
    軍St本in成 Ti設置esta設置p = 軍St本in成::軍本o設置Int(軍Math::Rand());
    本et使本n 軍St本in成::P本intf(TEXT("CO的V下%s下%s"), *Con正e本sion的a設置e, *Ti設置esta設置p);
}

軍St本in成 UMin成B使ildin成Reso使本ceSyste設置::GetReso使本ceType的a設置e(EMin成Reso使本ceType Reso使本ceType) const
{
    switch (Reso使本ceType)
    {
    case EMin成Reso使本ceType::基本ood: 本et使本n TEXT("基本ood");
    case EMin成Reso使本ceType::Stone: 本et使本n TEXT("Stone");
    case EMin成Reso使本ceType::Metal: 本et使本n TEXT("Metal");
    case EMin成Reso使本ceType::軍ood: 本et使本n TEXT("軍ood");
    case EMin成Reso使本ceType::基本ate本: 本et使本n TEXT("基本ate本");
    case EMin成Reso使本ceType::Coal: 本et使本n TEXT("Coal");
    case EMin成Reso使本ceType::I本on: 本et使本n TEXT("I本on");
    case EMin成Reso使本ceType::Coppe本: 本et使本n TEXT("Coppe本");
    case EMin成Reso使本ceType::Gold: 本et使本n TEXT("Gold");
    case EMin成Reso使本ceType::Sil正e本: 本et使本n TEXT("Sil正e本");
    case EMin成Reso使本ceType::Cotton: 本et使本n TEXT("Cotton");
    case EMin成Reso使本ceType::Silk: 本et使本n TEXT("Silk");
    case EMin成Reso使本ceType::Tea: 本et使本n TEXT("Tea");
    case EMin成Reso使本ceType::Opi使設置: 本et使本n TEXT("Opi使設置");
    case EMin成Reso使本ceType::Medicine: 本et使本n TEXT("Medicine");
    case EMin成Reso使本ceType::基本eapons: 本et使本n TEXT("基本eapons");
    case EMin成Reso使本ceType::A設置設置使nition: 本et使本n TEXT("A設置設置使nition");
    case EMin成Reso使本ceType::Unifo本設置s: 本et使本n TEXT("Unifo本設置s");
    case EMin成Reso使本ceType::輸入o本ses: 本et使本n TEXT("輸入o本ses");
    case EMin成Reso使本ceType::Vehicles: 本et使本n TEXT("Vehicles");
    case EMin成Reso使本ceType::軍使el: 本et使本n TEXT("軍使el");
    case EMin成Reso使本ceType::R使bbe本: 本et使本n TEXT("R使bbe本");
    case EMin成Reso使本ceType::Pape本: 本et使本n TEXT("Pape本");
    case EMin成Reso使本ceType::Ink: 本et使本n TEXT("Ink");
    case EMin成Reso使本ceType::Books: 本et使本n TEXT("Books");
    case EMin成Reso使本ceType::Tools: 本et使本n TEXT("Tools");
    case EMin成Reso使本ceType::Machine本y: 本et使本n TEXT("Machine本y");
    case EMin成Reso使本ceType::Elect本onics: 本et使本n TEXT("Elect本onics");
    case EMin成Reso使本ceType::Radio: 本et使本n TEXT("Radio");
    case EMin成Reso使本ceType::Telephone: 本et使本n TEXT("Telephone");
    case EMin成Reso使本ceType::Tele成本aph: 本et使本n TEXT("Tele成本aph");
    defa使lt: 本et使本n TEXT("Unknown");
    }
}

float UMin成B使ildin成Reso使本ceSyste設置::GetReso使本ceType基本ei成ht(EMin成Reso使本ceType Reso使本ceType) const
{
    // 資源基礎價值  Reso使本ce base 正al使es
    switch (Reso使本ceType)
    {
    case EMin成Reso使本ceType::Gold:
    case EMin成Reso使本ceType::Sil正e本:
        本et使本n 10.0f;
    case EMin成Reso使本ceType::基本eapons:
    case EMin成Reso使本ceType::A設置設置使nition:
    case EMin成Reso使本ceType::Medicine:
        本et使本n 5.0f;
    case EMin成Reso使本ceType::I本on:
    case EMin成Reso使本ceType::Coppe本:
    case EMin成Reso使本ceType::Coal:
        本et使本n 3.0f;
    case EMin成Reso使本ceType::Silk:
    case EMin成Reso使本ceType::Tea:
        本et使本n 4.0f;
    case EMin成Reso使本ceType::Opi使設置:
        本et使本n 8.0f;
    case EMin成Reso使本ceType::Machine本y:
    case EMin成Reso使本ceType::Elect本onics:
        本et使本n 6.0f;
    defa使lt:
        本et使本n 1.0f;
    }
}
