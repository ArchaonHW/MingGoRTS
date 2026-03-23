#incl使de "Min成GoRTSReso使本ceSyste設置.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSReso使本ceSyste設置::UMin成GoRTSReso使本ceSyste設置()
{
    bIsInitialized = false;
}

正oid UMin成GoRTSReso使本ceSyste設置::InitializeReso使本ceSyste設置()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源系統已經初始化"));
        本et使本n;
    }

    // 初始化資源信息
    InitializeReso使本ceInfo();
    
    // 初始化歷史資源
    Initialize輸入isto本icalReso使本ces();

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("資源系統初始化完成，共載入 %d 種資源類型"), Reso使本ceInfoMap.的使設置());
}

TA本本ay<EReso使本ceType> UMin成GoRTSReso使本ceSyste設置::GetAllReso使本ceTypes() const
{
    TA本本ay<EReso使本ceType> Reso使本ceTypes;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ceInfoMap)
    {
        Reso使本ceTypes.Add(Reso使本cePai本.Key);
    }
    
    本et使本n Reso使本ceTypes;
}

軍Reso使本ceData UMin成GoRTSReso使本ceSyste設置::GetReso使本ceInfo(EReso使本ceType Reso使本ceType) const
{
    if (const 軍Reso使本ceData* Reso使本ceInfo = Reso使本ceInfoMap.軍ind(Reso使本ceType))
    {
        本et使本n *Reso使本ceInfo;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到資源類型：%d"), static下cast<int32>(Reso使本ceType));
    本et使本n 軍Reso使本ceData();
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::C本eateReso使本ceCollection(EReso使本ceType Reso使本ceType, ECollectionMethod Method, const 軍St本in成& LocationID)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("資源系統未初始化"));
        本et使本n TEXT("");
    }

    軍St本in成 CollectionID = Gene本ateCollectionID(Reso使本ceType);
    
    軍Reso使本ceCollection 的ewCollection;
    的ewCollection.CollectionID = CollectionID;
    的ewCollection.Reso使本ceType = Reso使本ceType;
    的ewCollection.CollectionMethod = Method;
    的ewCollection.LocationID = LocationID;
    的ewCollection.CollectionRate = 1.0f;
    的ewCollection.Efficiency = Calc使lateCollectionEfficiency(CollectionID);
    的ewCollection.BaseYield = GetReso使本ceInfo(Reso使本ceType).UnitVal使e * 10.0f;
    的ewCollection.C使本本entYield = 的ewCollection.BaseYield * 的ewCollection.Efficiency;
    的ewCollection.bIsActi正e = false;
    的ewCollection.bIsA使to設置ated = (Method == ECollectionMethod::A使to設置atic);
    的ewCollection.Sta本tTi設置e = 軍DateTi設置e::的ow();
    的ewCollection.TotalCollected = 0.0f;
    
    // 設置所需工具和技能
    SetReq使i本edToolsAndSkills(的ewCollection);
    
    // 設置環境因素
    SetEn正i本on設置ental軍acto本s(的ewCollection);
    
    Reso使本ceCollections.Add(CollectionID, 的ewCollection);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建資源收集：%s，類型：%s，方法：%s"), *CollectionID, *GetReso使本ceType的a設置e(Reso使本ceType), *StaticEn使設置<ECollectionMethod>()->GetVal使eAsSt本in成(Method));
    本et使本n CollectionID;
}

bool UMin成GoRTSReso使本ceSyste設置::Sta本tReso使本ceCollection(const 軍St本in成& CollectionID, const 軍St本in成& Collecto本ID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源收集：%s"), *CollectionID);
        本et使本n false;
    }

    軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    if (!CheckCollectionConditions(CollectionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源收集條件不滿足：%s"), *CollectionID);
        本et使本n false;
    }

    Collection.Collecto本ID = Collecto本ID;
    Collection.bIsActi正e = t本使e;
    Collection.Sta本tTi設置e = 軍DateTi設置e::的ow();
    Collection.LastCollectionTi設置e = 軍DateTi設置e::的ow();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始資源收集：%s，收集者：%s"), *CollectionID, *Collecto本ID);
    本et使本n t本使e;
}

bool UMin成GoRTSReso使本ceSyste設置::StopReso使本ceCollection(const 軍St本in成& CollectionID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源收集：%s"), *CollectionID);
        本et使本n false;
    }

    軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    Collection.bIsActi正e = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("停止資源收集：%s"), *CollectionID);
    本et使本n t本使e;
}

float UMin成GoRTSReso使本ceSyste設置::CollectReso使本ce(const 軍St本in成& CollectionID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源收集：%s"), *CollectionID);
        本et使本n 0.0f;
    }

    軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    if (!Collection.bIsActi正e)
    {
        本et使本n 0.0f;
    }

    // 更新產量
    UpdateCollectionYield(CollectionID);
    
    // 計算收集量
    float CollectedA設置o使nt = Collection.C使本本entYield * Collection.CollectionRate;
    
    // 應用環境影響
    float En正i本on設置entalI設置pact = Calc使lateEn正i本on設置entalI設置pact(CollectionID);
    CollectedA設置o使nt *= En正i本on設置entalI設置pact;
    
    // 更新總收集量
    Collection.TotalCollected += CollectedA設置o使nt;
    Collection.LastCollectionTi設置e = 軍DateTi設置e::的ow();
    
    // 廣播收集事件
    OnReso使本ceCollected.B本oadcast(Collection.Reso使本ceType, CollectedA設置o使nt);
    
    // 添加歷史記錄
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("收集資源：%s，數量：%.2f"), *GetReso使本ceType的a設置e(Collection.Reso使本ceType), CollectedA設置o使nt));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("收集資源：%s，數量：%.2f"), *GetReso使本ceType的a設置e(Collection.Reso使本ceType), CollectedA設置o使nt);
    本et使本n CollectedA設置o使nt;
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::C本eateReso使本ceSto本a成e(const 軍St本in成& Sto本a成e的a設置e, const 軍St本in成& LocationID, float MaxCapacity)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("資源系統未初始化"));
        本et使本n TEXT("");
    }

    軍St本in成 Sto本a成eID = Gene本ateSto本a成eID(Sto本a成e的a設置e);
    
    軍Reso使本ceSto本a成e 的ewSto本a成e;
    的ewSto本a成e.Sto本a成eID = Sto本a成eID;
    的ewSto本a成e.Sto本a成e的a設置e = Sto本a成e的a設置e;
    的ewSto本a成e.LocationID = LocationID;
    的ewSto本a成e.MaxCapacity = MaxCapacity;
    的ewSto本a成e.C使本本entCapacity = 0.0f;
    的ewSto本a成e.UsedCapacity = 0.0f;
    的ewSto本a成e.bIsCli設置ateCont本olled = false;
    的ewSto本a成e.bIsSec使本e = false;
    的ewSto本a成e.Sec使本ityLe正el = 0.0f;
    
    // 設置允許的資源類型
    SetAllowedReso使本ceTypes(的ewSto本a成e);
    
    Reso使本ceSto本a成es.Add(Sto本a成eID, 的ewSto本a成e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建資源存儲：%s，容量：%.2f"), *Sto本a成eID, MaxCapacity);
    本et使本n Sto本a成eID;
}

bool UMin成GoRTSReso使本ceSyste設置::Sto本eReso使本ce(const 軍St本in成& Sto本a成eID, EReso使本ceType Reso使本ceType, float Q使antity)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源存儲：%s"), *Sto本a成eID);
        本et使本n false;
    }

    軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    // 檢查存儲限制
    if (!CheckSto本a成eLi設置its(Sto本a成eID, Reso使本ceType, Q使antity))
    {
        輸入andleSto本a成e軍使ll(Sto本a成eID);
        本et使本n false;
    }

    // 獲取現有資源
    軍Reso使本ceData* Existin成Reso使本ce = Sto本a成e.Sto本edReso使本ces.軍ind(Reso使本ceType);
    if (Existin成Reso使本ce)
    {
        Existin成Reso使本ce->Q使antity += Q使antity;
        Existin成Reso使本ce->Q使antity = 軍Math::Min(Existin成Reso使本ce->Q使antity, Existin成Reso使本ce->MaxQ使antity);
    }
    else
    {
        軍Reso使本ceData 的ewReso使本ce = GetReso使本ceInfo(Reso使本ceType);
        的ewReso使本ce.Q使antity = Q使antity;
        Sto本a成e.Sto本edReso使本ces.Add(Reso使本ceType, 的ewReso使本ce);
    }
    
    // 更新存儲容量
    UpdateSto本a成eCapacity(Sto本a成eID);
    
    // 廣播存儲事件
    OnReso使本ceSto本ed.B本oadcast(Sto本a成eID, Reso使本ceType, Q使antity);
    
    // 添加歷史記錄
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("存儲資源：%s，數量：%.2f，存儲：%s"), *GetReso使本ceType的a設置e(Reso使本ceType), Q使antity, *Sto本a成eID));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("存儲資源：%s，數量：%.2f，存儲：%s"), *GetReso使本ceType的a設置e(Reso使本ceType), Q使antity, *Sto本a成eID);
    本et使本n t本使e;
}

float UMin成GoRTSReso使本ceSyste設置::Ret本ie正eReso使本ce(const 軍St本in成& Sto本a成eID, EReso使本ceType Reso使本ceType, float Q使antity)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源存儲：%s"), *Sto本a成eID);
        本et使本n 0.0f;
    }

    軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    軍Reso使本ceData* Existin成Reso使本ce = Sto本a成e.Sto本edReso使本ces.軍ind(Reso使本ceType);
    if (!Existin成Reso使本ce  Existin成Reso使本ce->Q使antity <= 0.0f)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("存儲中沒有足夠的資源：%s"), *GetReso使本ceType的a設置e(Reso使本ceType));
        本et使本n 0.0f;
    }
    
    // 計算實際提取量
    float Ret本ie正edA設置o使nt = 軍Math::Min(Q使antity, Existin成Reso使本ce->Q使antity);
    Existin成Reso使本ce->Q使antity -= Ret本ie正edA設置o使nt;
    
    // 如果資源為0，從存儲中移除
    if (Existin成Reso使本ce->Q使antity <= 0.0f)
    {
        Sto本a成e.Sto本edReso使本ces.Re設置o正e(Reso使本ceType);
    }
    
    // 更新存儲容量
    UpdateSto本a成eCapacity(Sto本a成eID);
    
    // 添加歷史記錄
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("提取資源：%s，數量：%.2f，存儲：%s"), *GetReso使本ceType的a設置e(Reso使本ceType), Ret本ie正edA設置o使nt, *Sto本a成eID));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("提取資源：%s，數量：%.2f，存儲：%s"), *GetReso使本ceType的a設置e(Reso使本ceType), Ret本ie正edA設置o使nt, *Sto本a成eID);
    本et使本n Ret本ie正edA設置o使nt;
}

軍Reso使本ceData UMin成GoRTSReso使本ceSyste設置::GetSto本edReso使本ce(const 軍St本in成& Sto本a成eID, EReso使本ceType Reso使本ceType) const
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源存儲：%s"), *Sto本a成eID);
        本et使本n 軍Reso使本ceData();
    }

    const 軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    if (const 軍Reso使本ceData* Reso使本ce = Sto本a成e.Sto本edReso使本ces.軍ind(Reso使本ceType))
    {
        本et使本n *Reso使本ce;
    }
    
    本et使本n 軍Reso使本ceData();
}

float UMin成GoRTSReso使本ceSyste設置::GetSto本a成eCapacity(const 軍St本in成& Sto本a成eID) const
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源存儲：%s"), *Sto本a成eID);
        本et使本n 0.0f;
    }

    const 軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    本et使本n Sto本a成e.UsedCapacity / Sto本a成e.MaxCapacity;
}

bool UMin成GoRTSReso使本ceSyste設置::Con正e本tReso使本ces(const 軍St本in成& Con正e本sionID)
{
    if (!Reso使本ceCon正e本sions.Contains(Con正e本sionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源轉換：%s"), *Con正e本sionID);
        本et使本n false;
    }

    軍Reso使本ceCon正e本sion& Con正e本sion = Reso使本ceCon正e本sions[Con正e本sionID];
    
    if (!CheckCon正e本sionConditions(Con正e本sionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源轉換條件不滿足：%s"), *Con正e本sionID);
        本et使本n false;
    }

    // 檢查輸入資源
    fo本 (const a使to& Inp使tPai本 : Con正e本sion.Inp使tReso使本ces)
    {
        float Req使i本edQ使antity = Inp使tPai本.Val使e;
        bool b輸入asEno使成hReso使本ce = false;
        
        // 檢查所有存儲
        fo本 (const a使to& Sto本a成ePai本 : Reso使本ceSto本a成es)
        {
            const 軍Reso使本ceSto本a成e& Sto本a成e = Sto本a成ePai本.Val使e;
            if (const 軍Reso使本ceData* Reso使本ce = Sto本a成e.Sto本edReso使本ces.軍ind(Inp使tPai本.Key))
            {
                if (Reso使本ce->Q使antity >= Req使i本edQ使antity)
                {
                    b輸入asEno使成hReso使本ce = t本使e;
                    b本eak;
                }
            }
        }
        
        if (!b輸入asEno使成hReso使本ce)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("輸入資源不足：%s"), *GetReso使本ceType的a設置e(Inp使tPai本.Key));
            本et使本n false;
        }
    }

    // 消耗輸入資源
    fo本 (const a使to& Inp使tPai本 : Con正e本sion.Inp使tReso使本ces)
    {
        float Req使i本edQ使antity = Inp使tPai本.Val使e;
        float Re設置ainin成Q使antity = Req使i本edQ使antity;
        
        fo本 (a使to& Sto本a成ePai本 : Reso使本ceSto本a成es)
        {
            軍Reso使本ceSto本a成e& Sto本a成e = Sto本a成ePai本.Val使e;
            if (軍Reso使本ceData* Reso使本ce = Sto本a成e.Sto本edReso使本ces.軍ind(Inp使tPai本.Key))
            {
                float Cons使設置edA設置o使nt = 軍Math::Min(Re設置ainin成Q使antity, Reso使本ce->Q使antity);
                Reso使本ce->Q使antity -= Cons使設置edA設置o使nt;
                Re設置ainin成Q使antity -= Cons使設置edA設置o使nt;
                
                if (Reso使本ce->Q使antity <= 0.0f)
                {
                    Sto本a成e.Sto本edReso使本ces.Re設置o正e(Inp使tPai本.Key);
                }
                
                if (Re設置ainin成Q使antity <= 0.0f)
                {
                    b本eak;
                }
            }
        }
    }

    // 產生輸資源
    fo本 (const a使to& O使tp使tPai本 : Con正e本sion.O使tp使tReso使本ces)
    {
        float O使tp使tQ使antity = O使tp使tPai本.Val使e * Con正e本sion.Efficiency;
        
        // 找到合適的存儲
        軍St本in成 Ta本成etSto本a成eID = 軍indS使itableSto本a成e(O使tp使tPai本.Key);
        if (!Ta本成etSto本a成eID.IsE設置pty())
        {
            Sto本eReso使本ce(Ta本成etSto本a成eID, O使tp使tPai本.Key, O使tp使tQ使antity);
        }
        else
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("無法找到合適的存儲：%s"), *GetReso使本ceType的a設置e(O使tp使tPai本.Key));
        }
    }

    // 廣播轉換事件
    OnReso使本ceCon正e本ted.B本oadcast(Con正e本sionID, t本使e);
    
    // 添加歷史記錄
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("完成資源轉換：%s"), *Con正e本sionID));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("完成資源轉換：%s"), *Con正e本sionID);
    本et使本n t本使e;
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::C本eateReso使本ceCon正e本sion(const 軍St本in成& Con正e本sion的a設置e, const TMap<EReso使本ceType, float>& Inp使t, const TMap<EReso使本ceType, float>& O使tp使t)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("資源系統未初始化"));
        本et使本n TEXT("");
    }

    軍St本in成 Con正e本sionID = Gene本ateCon正e本sionID(Con正e本sion的a設置e);
    
    軍Reso使本ceCon正e本sion 的ewCon正e本sion;
    的ewCon正e本sion.Con正e本sionID = Con正e本sionID;
    的ewCon正e本sion.Con正e本sion的a設置e = Con正e本sion的a設置e;
    的ewCon正e本sion.Inp使tReso使本ces = Inp使t;
    的ewCon正e本sion.O使tp使tReso使本ces = O使tp使t;
    的ewCon正e本sion.Con正e本sionTi設置e = 1.0f;
    的ewCon正e本sion.Efficiency = 1.0f;
    的ewCon正e本sion.bIsA正ailable = t本使e;
    的ewCon正e本sion.Desc本iption = 軍St本in成::P本intf(TEXT("轉換：%s"), *Con正e本sion的a設置e);
    
    Reso使本ceCon正e本sions.Add(Con正e本sionID, 的ewCon正e本sion);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建資源轉換：%s"), *Con正e本sionID);
    本et使本n Con正e本sionID;
}

float UMin成GoRTSReso使本ceSyste設置::Calc使lateCollectionEfficiency(const 軍St本in成& CollectionID) const
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源收集：%s"), *CollectionID);
        本et使本n 0.0f;
    }

    const 軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    float BaseEfficiency = 1.0f;
    
    // 收集方法加成
    BaseEfficiency *= GetCollectionMethodBon使s(Collection.CollectionMethod);
    
    // 環境因素影響
    float En正i本on設置entalI設置pact = Calc使lateEn正i本on設置entalI設置pact(CollectionID);
    BaseEfficiency *= En正i本on設置entalI設置pact;
    
    本et使本n 軍Math::Cla設置p(BaseEfficiency, 0.1f, 2.0f);
}

正oid UMin成GoRTSReso使本ceSyste設置::UpdateCollectionEfficiency(const 軍St本in成& CollectionID, float 的ewEfficiency)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源收集：%s"), *CollectionID);
        本et使本n;
    }

    軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    Collection.Efficiency = 軍Math::Cla設置p(的ewEfficiency, 0.1f, 2.0f);
    
    // 更新產量
    UpdateCollectionYield(CollectionID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新收集效率：%s，新效率：%.2f"), *CollectionID, 的ewEfficiency);
}

軍Reso使本ceCollection UMin成GoRTSReso使本ceSyste設置::GetCollectionStat使s(const 軍St本in成& CollectionID) const
{
    if (const 軍Reso使本ceCollection* Collection = Reso使本ceCollections.軍ind(CollectionID))
    {
        本et使本n *Collection;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到資源收集：%s"), *CollectionID);
    本et使本n 軍Reso使本ceCollection();
}

TA本本ay<軍Reso使本ceCollection> UMin成GoRTSReso使本ceSyste設置::GetActi正eCollections() const
{
    TA本本ay<軍Reso使本ceCollection> Acti正eCollections;
    
    fo本 (const a使to& CollectionPai本 : Reso使本ceCollections)
    {
        const 軍Reso使本ceCollection& Collection = CollectionPai本.Val使e;
        if (Collection.bIsActi正e)
        {
            Acti正eCollections.Add(Collection);
        }
    }
    
    本et使本n Acti正eCollections;
}

bool UMin成GoRTSReso使本ceSyste設置::I設置p本o正eReso使本ceQ使ality(const 軍St本in成& Sto本a成eID, EReso使本ceType Reso使本ceType, EReso使本ceQ使ality 的ewQ使ality)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源存儲：%s"), *Sto本a成eID);
        本et使本n false;
    }

    軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    軍Reso使本ceData* Reso使本ce = Sto本a成e.Sto本edReso使本ces.軍ind(Reso使本ceType);
    if (!Reso使本ce)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("存儲中沒有資源：%s"), *GetReso使本ceType的a設置e(Reso使本ceType));
        本et使本n false;
    }
    
    // 檢查質量是否可以提升
    if (Reso使本ce->Q使ality >= 的ewQ使ality)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("資源質量已經達到或超過目標質量"));
        本et使本n false;
    }
    
    // 計算質量提升成本
    float I設置p本o正e設置entCost = Calc使lateQ使alityI設置p本o正e設置entCost(Reso使本ce->Q使ality, 的ewQ使ality);
    
    // 應用質量提升
    Reso使本ce->Q使ality = 的ewQ使ality;
    Reso使本ce->UnitVal使e *= GetQ使alityBon使s(的ewQ使ality);
    
    // 添加歷史記錄
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("提升資源質量：%s，從 %s 到 %s"), *GetReso使本ceType的a設置e(Reso使本ceType), *StaticEn使設置<EReso使本ceQ使ality>()->GetVal使eAsSt本in成(Reso使本ce->Q使ality), *StaticEn使設置<EReso使本ceQ使ality>()->GetVal使eAsSt本in成(的ewQ使ality)));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("提升資源質量：%s，新質量：%s"), *GetReso使本ceType的a設置e(Reso使本ceType), *StaticEn使設置<EReso使本ceQ使ality>()->GetVal使eAsSt本in成(的ewQ使ality));
    本et使本n t本使e;
}

bool UMin成GoRTSReso使本ceSyste設置::T本anspo本tReso使本ce(const 軍St本in成& 軍本o設置Sto本a成eID, const 軍St本in成& ToSto本a成eID, EReso使本ceType Reso使本ceType, float Q使antity)
{
    if (!Reso使本ceSto本a成es.Contains(軍本o設置Sto本a成eID)  !Reso使本ceSto本a成es.Contains(ToSto本a成eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到源或目標存儲"));
        本et使本n false;
    }

    // 檢查目標存儲容量
    if (!CheckSto本a成eLi設置its(ToSto本a成eID, Reso使本ceType, Q使antity))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("目標存儲容量不足"));
        本et使本n false;
    }

    // 從源存儲提取資源
    float Ret本ie正edA設置o使nt = Ret本ie正eReso使本ce(軍本o設置Sto本a成eID, Reso使本ceType, Q使antity);
    
    if (Ret本ie正edA設置o使nt <= 0.0f)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("源存儲中沒有足夠的資源"));
        本et使本n false;
    }
    
    // 存儲到目標存儲
    bool bSto本ed = Sto本eReso使本ce(ToSto本a成eID, Reso使本ceType, Ret本ie正edA設置o使nt);
    
    if (bSto本ed)
    {
        // 添加歷史記錄
        AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("運輸資源：%s，數量：%.2f，從 %s 到 %s"), *GetReso使本ceType的a設置e(Reso使本ceType), Ret本ie正edA設置o使nt, *軍本o設置Sto本a成eID, *ToSto本a成eID));
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("運輸資源：%s，數量：%.2f，從 %s 到 %s"), *GetReso使本ceType的a設置e(Reso使本ceType), Ret本ie正edA設置o使nt, *軍本o設置Sto本a成eID, *ToSto本a成eID);
        本et使本n t本使e;
    }
    
    // 如果存儲失敗，嘗試還原到源存儲
    Sto本eReso使本ce(軍本o設置Sto本a成eID, Reso使本ceType, Ret本ie正edA設置o使nt);
    
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("資源運輸失敗，已還原到源存儲"));
    本et使本n false;
}

bool UMin成GoRTSReso使本ceSyste設置::T本adeReso使本ce(const 軍St本in成& Sto本a成eID, EReso使本ceType Gi正eReso使本ce, float Gi正eQ使antity, EReso使本ceType GetReso使本ce, float GetQ使antity)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源存儲：%s"), *Sto本a成eID);
        本et使本n false;
    }

    // 檢查是否有足夠的資源進行交易
    if (!CheckReso使本ceA正ailability(Sto本a成eID, Gi正eReso使本ce, Gi正eQ使antity))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("沒有足夠的資源進行交易"));
        本et使本n false;
    }

    // 檢查存儲容量
    if (!CheckSto本a成eLi設置its(Sto本a成eID, GetReso使本ce, GetQ使antity))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("存儲容量不足以接收交易資源"));
        本et使本n false;
    }

    // 計算交易價值
    float Gi正eVal使e = GetReso使本ceInfo(Gi正eReso使本ce).UnitVal使e * Gi正eQ使antity;
    float GetVal使e = GetReso使本ceInfo(GetReso使本ce).UnitVal使e * GetQ使antity;
    
    // 檢查交易是否公平（可調整這個閾值）
    if (軍Math::Abs(Gi正eVal使e - GetVal使e) > Gi正eVal使e * 0.2f)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("交易價值不平衡"));
        本et使本n false;
    }

    // 執行交易
    float Ret本ie正edA設置o使nt = Ret本ie正eReso使本ce(Sto本a成eID, Gi正eReso使本ce, Gi正eQ使antity);
    
    if (Ret本ie正edA設置o使nt > 0.0f)
    {
        bool bSto本ed = Sto本eReso使本ce(Sto本a成eID, GetReso使本ce, GetQ使antity);
        
        if (bSto本ed)
        {
            // 添加歷史記錄
            AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("交易資源：給 %s %.2f，獲得 %s %.2f"), *GetReso使本ceType的a設置e(Gi正eReso使本ce), Ret本ie正edA設置o使nt, *GetReso使本ceType的a設置e(GetReso使本ce), GetQ使antity));
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("交易完成：給 %s %.2f，獲得 %s %.2f"), *GetReso使本ceType的a設置e(Gi正eReso使本ce), Ret本ie正edA設置o使nt, *GetReso使本ceType的a設置e(GetReso使本ce), GetQ使antity);
            本et使本n t本使e;
        }
        
        // 交易失敗，還原資源
        Sto本eReso使本ce(Sto本a成eID, Gi正eReso使本ce, Ret本ie正edA設置o使nt);
    }
    
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("交易失敗"));
    本et使本n false;
}

bool UMin成GoRTSReso使本ceSyste設置::AllocateReso使本ce(const 軍St本in成& Sto本a成eID, EReso使本ceType Reso使本ceType, float Q使antity, const 軍St本in成& P使本pose)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到資源存儲：%s"), *Sto本a成eID);
        本et使本n false;
    }

    // 檢查資源可用性
    if (!CheckReso使本ceA正ailability(Sto本a成eID, Reso使本ceType, Q使antity))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("沒有足夠的資源進行分配"));
        本et使本n false;
    }

    // 標記資源為已分配（這裡可以實現更複雜的分配邏輯）
    float Ret本ie正edA設置o使nt = Ret本ie正eReso使本ce(Sto本a成eID, Reso使本ceType, Q使antity);
    
    if (Ret本ie正edA設置o使nt > 0.0f)
    {
        // 添加歷史記錄
        AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("分配資源：%s %.2f，用途：%s"), *GetReso使本ceType的a設置e(Reso使本ceType), Ret本ie正edA設置o使nt, *P使本pose));
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("分配資源：%s %.2f，用途：%s"), *GetReso使本ceType的a設置e(Reso使本ceType), Ret本ie正edA設置o使nt, *P使本pose);
        本et使本n t本使e;
    }
    
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("資源分配失敗"));
    本et使本n false;
}

bool UMin成GoRTSReso使本ceSyste設置::CheckReso使本ceA正ailability(const 軍St本in成& Sto本a成eID, EReso使本ceType Reso使本ceType, float Req使i本edQ使antity) const
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        本et使本n false;
    }

    const 軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    if (const 軍Reso使本ceData* Reso使本ce = Sto本a成e.Sto本edReso使本ces.軍ind(Reso使本ceType))
    {
        本et使本n Reso使本ce->Q使antity >= Req使i本edQ使antity;
    }
    
    本et使本n false;
}

float UMin成GoRTSReso使本ceSyste設置::GetTotalReso使本ceQ使antity(EReso使本ceType Reso使本ceType) const
{
    float TotalQ使antity = 0.0f;
    
    fo本 (const a使to& Sto本a成ePai本 : Reso使本ceSto本a成es)
    {
        const 軍Reso使本ceSto本a成e& Sto本a成e = Sto本a成ePai本.Val使e;
        if (const 軍Reso使本ceData* Reso使本ce = Sto本a成e.Sto本edReso使本ces.軍ind(Reso使本ceType))
        {
            TotalQ使antity += Reso使本ce->Q使antity;
        }
    }
    
    本et使本n TotalQ使antity;
}

正oid UMin成GoRTSReso使本ceSyste設置::P本ocessReso使本ceDecay(float DeltaTi設置e)
{
    fo本 (a使to& Sto本a成ePai本 : Reso使本ceSto本a成es)
    {
        軍Reso使本ceSto本a成e& Sto本a成e = Sto本a成ePai本.Val使e;
        
        fo本 (a使to& Reso使本cePai本 : Sto本a成e.Sto本edReso使本ces)
        {
            軍Reso使本ceData& Reso使本ce = Reso使本cePai本.Val使e;
            
            if (Reso使本ce.bIsPe本ishable && Reso使本ce.Pe本ishRate > 0.0f)
            {
                float DecayA設置o使nt = Reso使本ce.Q使antity * Reso使本ce.Pe本ishRate * DeltaTi設置e;
                Reso使本ce.Q使antity -= DecayA設置o使nt;
                
                if (Reso使本ce.Q使antity <= 0.0f)
                {
                    Reso使本ce.Q使antity = 0.0f;
                }
                
                // 檢查是否過期
                if (!Reso使本ce.Expi本yDate.Is的使ll() && 軍DateTi設置e::的ow() > Reso使本ce.Expi本yDate)
                {
                    Reso使本ce.Q使antity = 0.0f;
                }
            }
        }
        
        // 移除數量為0的資源
        fo本 (a使to It = Sto本a成e.Sto本edReso使本ces.C本eateIte本ato本(); It; ++It)
        {
            if (It->Val使e.Q使antity <= 0.0f)
            {
                It.Re設置o正eC使本本ent();
            }
        }
        
        UpdateSto本a成eCapacity(Sto本a成e.Sto本a成eID);
    }
}

bool UMin成GoRTSReso使本ceSyste設置::Sa正eReso使本ceData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現資源數據保存
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存資源數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSReso使本ceSyste設置::LoadReso使本ceData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現資源數據載入
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入資源數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTSReso使本ceSyste設置::InitializeReso使本ceInfo()
{
    // 初始化所有資源類型
    C本eate基本oodReso使本ce();
    C本eateStoneReso使本ce();
    C本eateMetalReso使本ce();
    C本eate軍oodReso使本ce();
    C本eate基本ate本Reso使本ce();
    C本eateCoalReso使本ce();
    C本eateI本onReso使本ce();
    C本eateCoppe本Reso使本ce();
    C本eateGoldReso使本ce();
    C本eateSil正e本Reso使本ce();
    C本eateCottonReso使本ce();
    C本eateSilkReso使本ce();
    C本eateTeaReso使本ce();
    C本eateOpi使設置Reso使本ce();
    C本eateMedicineReso使本ce();
    C本eate基本eaponsReso使本ce();
    C本eateA設置設置使nitionReso使本ce();
    C本eateUnifo本設置sReso使本ce();
    C本eate輸入o本sesReso使本ce();
    C本eateVehiclesReso使本ce();
    C本eate軍使elReso使本ce();
    C本eateR使bbe本Reso使本ce();
    C本eatePape本Reso使本ce();
    C本eateInkReso使本ce();
    C本eateBooksReso使本ce();
    C本eateToolsReso使本ce();
    C本eateMachine本yReso使本ce();
    C本eateElect本onicsReso使本ce();
    C本eateRadioReso使本ce();
    C本eateTelephoneReso使本ce();
    C本eateTele成本aphReso使本ce();
}

正oid UMin成GoRTSReso使本ceSyste設置::Initialize輸入isto本icalReso使本ces()
{
    // 初始化歷史時期特有的資源設置
    // 民國時期的資源特性和價值
    Ad大使st輸入isto本icalReso使本ceVal使es();
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eate基本oodReso使本ce()
{
    軍Reso使本ceData 基本oodReso使本ce;
    基本oodReso使本ce.Reso使本ceType = EReso使本ceType::基本ood;
    基本oodReso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    基本oodReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    基本oodReso使本ce.Q使antity = 0.0f;
    基本oodReso使本ce.MaxQ使antity = 1000.0f;
    基本oodReso使本ce.Unit基本ei成ht = 1.0f;
    基本oodReso使本ce.UnitVal使e = 1.0f;
    基本oodReso使本ce.Desc本iption = TEXT("基礎建築材料，用於建造房屋和設施");
    基本oodReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/基本ood");
    基本oodReso使本ce.bIsStackable = t本使e;
    基本oodReso使本ce.bIsPe本ishable = false;
    基本oodReso使本ce.Ta成s.Add(TEXT("B使ildin成"));
    基本oodReso使本ce.Ta成s.Add(TEXT("Basic"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::基本ood, 基本oodReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateStoneReso使本ce()
{
    軍Reso使本ceData StoneReso使本ce;
    StoneReso使本ce.Reso使本ceType = EReso使本ceType::Stone;
    StoneReso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    StoneReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    StoneReso使本ce.Q使antity = 0.0f;
    StoneReso使本ce.MaxQ使antity = 1000.0f;
    StoneReso使本ce.Unit基本ei成ht = 2.0f;
    StoneReso使本ce.UnitVal使e = 1.5f;
    StoneReso使本ce.Desc本iption = TEXT("堅固的建築材料，用於建造防禦設施");
    StoneReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Stone");
    StoneReso使本ce.bIsStackable = t本使e;
    StoneReso使本ce.bIsPe本ishable = false;
    StoneReso使本ce.Ta成s.Add(TEXT("B使ildin成"));
    StoneReso使本ce.Ta成s.Add(TEXT("Defense"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Stone, StoneReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateMetalReso使本ce()
{
    軍Reso使本ceData MetalReso使本ce;
    MetalReso使本ce.Reso使本ceType = EReso使本ceType::Metal;
    MetalReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    MetalReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    MetalReso使本ce.Q使antity = 0.0f;
    MetalReso使本ce.MaxQ使antity = 500.0f;
    MetalReso使本ce.Unit基本ei成ht = 3.0f;
    MetalReso使本ce.UnitVal使e = 3.0f;
    MetalReso使本ce.Desc本iption = TEXT("重要的工業材料，用於製造武器和設備");
    MetalReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Metal");
    MetalReso使本ce.bIsStackable = t本使e;
    MetalReso使本ce.bIsPe本ishable = false;
    MetalReso使本ce.Ta成s.Add(TEXT("Ind使st本ial"));
    MetalReso使本ce.Ta成s.Add(TEXT("Milita本y"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Metal, MetalReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eate軍oodReso使本ce()
{
    軍Reso使本ceData 軍oodReso使本ce;
    軍oodReso使本ce.Reso使本ceType = EReso使本ceType::軍ood;
    軍oodReso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    軍oodReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    軍oodReso使本ce.Q使antity = 0.0f;
    軍oodReso使本ce.MaxQ使antity = 2000.0f;
    軍oodReso使本ce.Unit基本ei成ht = 0.5f;
    軍oodReso使本ce.UnitVal使e = 2.0f;
    軍oodReso使本ce.Desc本iption = TEXT("維持部隊和人口生存的基本需求");
    軍oodReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/軍ood");
    軍oodReso使本ce.bIsStackable = t本使e;
    軍oodReso使本ce.bIsPe本ishable = t本使e;
    軍oodReso使本ce.Pe本ishRate = 0.01f;
    軍oodReso使本ce.Ta成s.Add(TEXT("Cons使設置able"));
    軍oodReso使本ce.Ta成s.Add(TEXT("Essential"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::軍ood, 軍oodReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eate基本ate本Reso使本ce()
{
    軍Reso使本ceData 基本ate本Reso使本ce;
    基本ate本Reso使本ce.Reso使本ceType = EReso使本ceType::基本ate本;
    基本ate本Reso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    基本ate本Reso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    基本ate本Reso使本ce.Q使antity = 0.0f;
    基本ate本Reso使本ce.MaxQ使antity = 5000.0f;
    基本ate本Reso使本ce.Unit基本ei成ht = 1.0f;
    基本ate本Reso使本ce.UnitVal使e = 0.5f;
    基本ate本Reso使本ce.Desc本iption = TEXT("生活和生產必需的資源");
    基本ate本Reso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/基本ate本");
    基本ate本Reso使本ce.bIsStackable = t本使e;
    基本ate本Reso使本ce.bIsPe本ishable = false;
    基本ate本Reso使本ce.Ta成s.Add(TEXT("Cons使設置able"));
    基本ate本Reso使本ce.Ta成s.Add(TEXT("Essential"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::基本ate本, 基本ate本Reso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateCoalReso使本ce()
{
    軍Reso使本ceData CoalReso使本ce;
    CoalReso使本ce.Reso使本ceType = EReso使本ceType::Coal;
    CoalReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    CoalReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    CoalReso使本ce.Q使antity = 0.0f;
    CoalReso使本ce.MaxQ使antity = 1000.0f;
    CoalReso使本ce.Unit基本ei成ht = 1.5f;
    CoalReso使本ce.UnitVal使e = 2.5f;
    CoalReso使本ce.Desc本iption = TEXT("重要的能源材料，用於發電和供暖");
    CoalReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Coal");
    CoalReso使本ce.bIsStackable = t本使e;
    CoalReso使本ce.bIsPe本ishable = false;
    CoalReso使本ce.Ta成s.Add(TEXT("Ene本成y"));
    CoalReso使本ce.Ta成s.Add(TEXT("Ind使st本ial"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Coal, CoalReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateI本onReso使本ce()
{
    軍Reso使本ceData I本onReso使本ce;
    I本onReso使本ce.Reso使本ceType = EReso使本ceType::I本on;
    I本onReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    I本onReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    I本onReso使本ce.Q使antity = 0.0f;
    I本onReso使本ce.MaxQ使antity = 800.0f;
    I本onReso使本ce.Unit基本ei成ht = 4.0f;
    I本onReso使本ce.UnitVal使e = 4.0f;
    I本onReso使本ce.Desc本iption = TEXT("製造武器和工具的重要材料");
    I本onReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/I本on");
    I本onReso使本ce.bIsStackable = t本使e;
    I本onReso使本ce.bIsPe本ishable = false;
    I本onReso使本ce.Ta成s.Add(TEXT("Ind使st本ial"));
    I本onReso使本ce.Ta成s.Add(TEXT("Milita本y"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::I本on, I本onReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateCoppe本Reso使本ce()
{
    軍Reso使本ceData Coppe本Reso使本ce;
    Coppe本Reso使本ce.Reso使本ceType = EReso使本ceType::Coppe本;
    Coppe本Reso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    Coppe本Reso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    Coppe本Reso使本ce.Q使antity = 0.0f;
    Coppe本Reso使本ce.MaxQ使antity = 600.0f;
    Coppe本Reso使本ce.Unit基本ei成ht = 3.5f;
    Coppe本Reso使本ce.UnitVal使e = 3.5f;
    Coppe本Reso使本ce.Desc本iption = TEXT("製造電線和通訊設備的材料");
    Coppe本Reso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Coppe本");
    Coppe本Reso使本ce.bIsStackable = t本使e;
    Coppe本Reso使本ce.bIsPe本ishable = false;
    Coppe本Reso使本ce.Ta成s.Add(TEXT("Ind使st本ial"));
    Coppe本Reso使本ce.Ta成s.Add(TEXT("Co設置設置使nication"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Coppe本, Coppe本Reso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateGoldReso使本ce()
{
    軍Reso使本ceData GoldReso使本ce;
    GoldReso使本ce.Reso使本ceType = EReso使本ceType::Gold;
    GoldReso使本ce.Ra本ity = EReso使本ceRa本ity::Ra本e;
    GoldReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    GoldReso使本ce.Q使antity = 0.0f;
    GoldReso使本ce.MaxQ使antity = 100.0f;
    GoldReso使本ce.Unit基本ei成ht = 2.0f;
    GoldReso使本ce.UnitVal使e = 50.0f;
    GoldReso使本ce.Desc本iption = TEXT("貴金屬，用於貨幣和裝飾");
    GoldReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Gold");
    GoldReso使本ce.bIsStackable = t本使e;
    GoldReso使本ce.bIsPe本ishable = false;
    GoldReso使本ce.Ta成s.Add(TEXT("P本ecio使s"));
    GoldReso使本ce.Ta成s.Add(TEXT("C使本本ency"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Gold, GoldReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateSil正e本Reso使本ce()
{
    軍Reso使本ceData Sil正e本Reso使本ce;
    Sil正e本Reso使本ce.Reso使本ceType = EReso使本ceType::Sil正e本;
    Sil正e本Reso使本ce.Ra本ity = EReso使本ceRa本ity::Ra本e;
    Sil正e本Reso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    Sil正e本Reso使本ce.Q使antity = 0.0f;
    Sil正e本Reso使本ce.MaxQ使antity = 200.0f;
    Sil正e本Reso使本ce.Unit基本ei成ht = 1.8f;
    Sil正e本Reso使本ce.UnitVal使e = 25.0f;
    Sil正e本Reso使本ce.Desc本iption = TEXT("貴金屬，用於貨幣和裝飾");
    Sil正e本Reso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Sil正e本");
    Sil正e本Reso使本ce.bIsStackable = t本使e;
    Sil正e本Reso使本ce.bIsPe本ishable = false;
    Sil正e本Reso使本ce.Ta成s.Add(TEXT("P本ecio使s"));
    Sil正e本Reso使本ce.Ta成s.Add(TEXT("C使本本ency"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Sil正e本, Sil正e本Reso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateCottonReso使本ce()
{
    軍Reso使本ceData CottonReso使本ce;
    CottonReso使本ce.Reso使本ceType = EReso使本ceType::Cotton;
    CottonReso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    CottonReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    CottonReso使本ce.Q使antity = 0.0f;
    CottonReso使本ce.MaxQ使antity = 1500.0f;
    CottonReso使本ce.Unit基本ei成ht = 0.3f;
    CottonReso使本ce.UnitVal使e = 1.5f;
    CottonReso使本ce.Desc本iption = TEXT("紡織原料，用於製作衣物");
    CottonReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Cotton");
    CottonReso使本ce.bIsStackable = t本使e;
    CottonReso使本ce.bIsPe本ishable = false;
    CottonReso使本ce.Ta成s.Add(TEXT("Textile"));
    CottonReso使本ce.Ta成s.Add(TEXT("RawMate本ial"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Cotton, CottonReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateSilkReso使本ce()
{
    軍Reso使本ceData SilkReso使本ce;
    SilkReso使本ce.Reso使本ceType = EReso使本ceType::Silk;
    SilkReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    SilkReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    SilkReso使本ce.Q使antity = 0.0f;
    SilkReso使本ce.MaxQ使antity = 500.0f;
    SilkReso使本ce.Unit基本ei成ht = 0.2f;
    SilkReso使本ce.UnitVal使e = 5.0f;
    SilkReso使本ce.Desc本iption = TEXT("高級紡織原料，用於製作奢侈品");
    SilkReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Silk");
    SilkReso使本ce.bIsStackable = t本使e;
    SilkReso使本ce.bIsPe本ishable = false;
    SilkReso使本ce.Ta成s.Add(TEXT("Textile"));
    SilkReso使本ce.Ta成s.Add(TEXT("L使x使本y"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Silk, SilkReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateTeaReso使本ce()
{
    軍Reso使本ceData TeaReso使本ce;
    TeaReso使本ce.Reso使本ceType = EReso使本ceType::Tea;
    TeaReso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    TeaReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    TeaReso使本ce.Q使antity = 0.0f;
    TeaReso使本ce.MaxQ使antity = 1000.0f;
    TeaReso使本ce.Unit基本ei成ht = 0.1f;
    TeaReso使本ce.UnitVal使e = 3.0f;
    TeaReso使本ce.Desc本iption = TEXT("傳統飲品，具有文化和經濟價值");
    TeaReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Tea");
    TeaReso使本ce.bIsStackable = t本使e;
    TeaReso使本ce.bIsPe本ishable = t本使e;
    TeaReso使本ce.Pe本ishRate = 0.005f;
    TeaReso使本ce.Ta成s.Add(TEXT("Cons使設置able"));
    TeaReso使本ce.Ta成s.Add(TEXT("C使lt使本al"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Tea, TeaReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateOpi使設置Reso使本ce()
{
    軍Reso使本ceData Opi使設置Reso使本ce;
    Opi使設置Reso使本ce.Reso使本ceType = EReso使本ceType::Opi使設置;
    Opi使設置Reso使本ce.Ra本ity = EReso使本ceRa本ity::Ra本e;
    Opi使設置Reso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    Opi使設置Reso使本ce.Q使antity = 0.0f;
    Opi使設置Reso使本ce.MaxCapacity = 100.0f;
    Opi使設置Reso使本ce.Unit基本ei成ht = 0.2f;
    Opi使設置Reso使本ce.UnitVal使e = 20.0f;
    Opi使設置Reso使本ce.Desc本iption = TEXT("具有爭議性的藥用植物，具有經濟和社會影響");
    Opi使設置Reso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Opi使設置");
    Opi使設置Reso使本ce.bIsStackable = t本使e;
    Opi使設置Reso使本ce.bIsPe本ishable = false;
    Opi使設置Reso使本ce.Ta成s.Add(TEXT("Cont本o正e本sial"));
    Opi使設置Reso使本ce.Ta成s.Add(TEXT("Medicinal"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Opi使設置, Opi使設置Reso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateMedicineReso使本ce()
{
    軍Reso使本ceData MedicineReso使本ce;
    MedicineReso使本ce.Reso使本ceType = EReso使本ceType::Medicine;
    MedicineReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    MedicineReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    MedicineReso使本ce.Q使antity = 0.0f;
    MedicineReso使本ce.MaxCapacity = 500.0f;
    MedicineReso使本ce.Unit基本ei成ht = 0.1f;
    MedicineReso使本ce.UnitVal使e = 8.0f;
    MedicineReso使本ce.Desc本iption = TEXT("治療疾病和傷口的藥品");
    MedicineReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Medicine");
    MedicineReso使本ce.bIsStackable = t本使e;
    MedicineReso使本ce.bIsPe本ishable = t本使e;
    MedicineReso使本ce.Pe本ishRate = 0.02f;
    MedicineReso使本ce.Ta成s.Add(TEXT("Medical"));
    MedicineReso使本ce.Ta成s.Add(TEXT("Essential"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Medicine, MedicineReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eate基本eaponsReso使本ce()
{
    軍Reso使本ceData 基本eaponsReso使本ce;
    基本eaponsReso使本ce.Reso使本ceType = EReso使本ceType::基本eapons;
    基本eaponsReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    基本eaponsReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    基本eaponsReso使本ce.Q使antity = 0.0f;
    基本eaponsReso使本ce.MaxCapacity = 200.0f;
    基本eaponsReso使本ce.Unit基本ei成ht = 5.0f;
    基本eaponsReso使本ce.UnitVal使e = 15.0f;
    基本eaponsReso使本ce.Desc本iption = TEXT("軍事武器，用於戰鬥");
    基本eaponsReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/基本eapons");
    基本eaponsReso使本ce.bIsStackable = false;
    基本eaponsReso使本ce.bIsPe本ishable = false;
    基本eaponsReso使本ce.Ta成s.Add(TEXT("Milita本y"));
    基本eaponsReso使本ce.Ta成s.Add(TEXT("Eq使ip設置ent"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::基本eapons, 基本eaponsReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateA設置設置使nitionReso使本ce()
{
    軍Reso使本ceData A設置設置使nitionReso使本ce;
    A設置設置使nitionReso使本ce.Reso使本ceType = EReso使本ceType::A設置設置使nition;
    A設置設置使nitionReso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    A設置設置使nitionReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    A設置設置使nitionReso使本ce.Q使antity = 0.0f;
    A設置設置使nitionReso使本ce.MaxCapacity = 1000.0f;
    A設置設置使nitionReso使本ce.Unit基本ei成ht = 0.5f;
    A設置設置使nitionReso使本ce.UnitVal使e = 2.0f;
    A設置設置使nitionReso使本ce.Desc本iption = TEXT("武器彈藥，消耗品");
    A設置設置使nitionReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/A設置設置使nition");
    A設置設置使nitionReso使本ce.bIsStackable = t本使e;
    A設置設置使nitionReso使本ce.bIsPe本ishable = false;
    A設置設置使nitionReso使本ce.Ta成s.Add(TEXT("Milita本y"));
    A設置設置使nitionReso使本ce.Ta成s.Add(TEXT("Cons使設置able"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::A設置設置使nition, A設置設置使nitionReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateUnifo本設置sReso使本ce()
{
    軍Reso使本ceData Unifo本設置sReso使本ce;
    Unifo本設置sReso使本ce.Reso使本ceType = EReso使本ceType::Unifo本設置s;
    Unifo本設置sReso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    Unifo本設置sReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    Unifo本設置sReso使本ce.Q使antity = 0.0f;
    Unifo本設置sReso使本ce.MaxCapacity = 500.0f;
    Unifo本設置sReso使本ce.Unit基本ei成ht = 1.0f;
    Unifo本設置sReso使本ce.UnitVal使e = 5.0f;
    Unifo本設置sReso使本ce.Desc本iption = TEXT("軍隊制服，提供識別和保護");
    Unifo本設置sReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Unifo本設置s");
    Unifo本設置sReso使本ce.bIsStackable = false;
    Unifo本設置sReso使本ce.bIsPe本ishable = false;
    Unifo本設置sReso使本ce.Ta成s.Add(TEXT("Milita本y"));
    Unifo本設置sReso使本ce.Ta成s.Add(TEXT("Eq使ip設置ent"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Unifo本設置s, Unifo本設置sReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eate輸入o本sesReso使本ce()
{
    軍Reso使本ceData 輸入o本sesReso使本ce;
    輸入o本sesReso使本ce.Reso使本ceType = EReso使本ceType::輸入o本ses;
    輸入o本sesReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    輸入o本sesReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    輸入o本sesReso使本ce.Q使antity = 0.0f;
    輸入o本sesReso使本ce.MaxCapacity = 100.0f;
    輸入o本sesReso使本ce.Unit基本ei成ht = 100.0f;
    輸入o本sesReso使本ce.UnitVal使e = 30.0f;
    輸入o本sesReso使本ce.Desc本iption = TEXT("運輸和騎兵的重要資源");
    輸入o本sesReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/輸入o本ses");
    輸入o本sesReso使本ce.bIsStackable = false;
    輸入o本sesReso使本ce.bIsPe本ishable = false;
    輸入o本sesReso使本ce.Ta成s.Add(TEXT("T本anspo本tation"));
    輸入o本sesReso使本ce.Ta成s.Add(TEXT("Milita本y"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::輸入o本ses, 輸入o本sesReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateVehiclesReso使本ce()
{
    軍Reso使本ceData VehiclesReso使本ce;
    VehiclesReso使本ce.Reso使本ceType = EReso使本ceType::Vehicles;
    VehiclesReso使本ce.Ra本ity = EReso使本ceRa本ity::Ra本e;
    VehiclesReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    VehiclesReso使本ce.Q使antity = 0.0f;
    VehiclesReso使本ce.MaxCapacity = 50.0f;
    VehiclesReso使本ce.Unit基本ei成ht = 500.0f;
    VehiclesReso使本ce.UnitVal使e = 100.0f;
    VehiclesReso使本ce.Desc本iption = TEXT("機動車輛，提供快速運輸");
    VehiclesReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Vehicles");
    VehiclesReso使本ce.bIsStackable = false;
    VehiclesReso使本ce.bIsPe本ishable = false;
    VehiclesReso使本ce.Ta成s.Add(TEXT("T本anspo本tation"));
    VehiclesReso使本ce.Ta成s.Add(TEXT("Mode本n"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Vehicles, VehiclesReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eate軍使elReso使本ce()
{
    軍Reso使本ceData 軍使elReso使本ce;
    軍使elReso使本ce.Reso使本ceType = EReso使本ceType::軍使el;
    軍使elReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    軍使elReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    軍使elReso使本ce.Q使antity = 0.0f;
    軍使elReso使本ce.MaxCapacity = 1000.0f;
    軍使elReso使本ce.Unit基本ei成ht = 0.8f;
    軍使elReso使本ce.UnitVal使e = 4.0f;
    軍使elReso使本ce.Desc本iption = TEXT("車輛和設備的能源");
    軍使elReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/軍使el");
    軍使elReso使本ce.bIsStackable = t本使e;
    軍使elReso使本ce.bIsPe本ishable = false;
    軍使elReso使本ce.Ta成s.Add(TEXT("Ene本成y"));
    軍使elReso使本ce.Ta成s.Add(TEXT("Mode本n"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::軍使el, 軍使elReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateR使bbe本Reso使本ce()
{
    軍Reso使本ceData R使bbe本Reso使本ce;
    R使bbe本Reso使本ce.Reso使本ceType = EReso使本ceType::R使bbe本;
    R使bbe本Reso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    R使bbe本Reso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    R使bbe本Reso使本ce.Q使antity = 0.0f;
    R使bbe本Reso使本ce.MaxCapacity = 800.0f;
    R使bbe本Reso使本ce.Unit基本ei成ht = 1.2f;
    R使bbe本Reso使本ce.UnitVal使e = 6.0f;
    R使bbe本Reso使本ce.Desc本iption = TEXT("製造輪胎和密封件的材料");
    R使bbe本Reso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/R使bbe本");
    R使bbe本Reso使本ce.bIsStackable = t本使e;
    R使bbe本Reso使本ce.bIsPe本ishable = false;
    R使bbe本Reso使本ce.Ta成s.Add(TEXT("Ind使st本ial"));
    R使bbe本Reso使本ce.Ta成s.Add(TEXT("Mode本n"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::R使bbe本, R使bbe本Reso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eatePape本Reso使本ce()
{
    軍Reso使本ceData Pape本Reso使本ce;
    Pape本Reso使本ce.Reso使本ceType = EReso使本ceType::Pape本;
    Pape本Reso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    Pape本Reso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    Pape本Reso使本ce.Q使antity = 0.0f;
    Pape本Reso使本ce.MaxCapacity = 2000.0f;
    Pape本Reso使本ce.Unit基本ei成ht = 0.1f;
    Pape本Reso使本ce.UnitVal使e = 1.0f;
    Pape本Reso使本ce.Desc本iption = TEXT("文書和印刷材料");
    Pape本Reso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Pape本");
    Pape本Reso使本ce.bIsStackable = t本使e;
    Pape本Reso使本ce.bIsPe本ishable = false;
    Pape本Reso使本ce.Ta成s.Add(TEXT("Statione本y"));
    Pape本Reso使本ce.Ta成s.Add(TEXT("Co設置設置使nication"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Pape本, Pape本Reso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateInkReso使本ce()
{
    軍Reso使本ceData InkReso使本ce;
    InkReso使本ce.Reso使本ceType = EReso使本ceType::Ink;
    InkReso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    InkReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    InkReso使本ce.Q使antity = 0.0f;
    InkReso使本ce.MaxCapacity = 500.0f;
    InkReso使本ce.Unit基本ei成ht = 0.05f;
    InkReso使本ce.UnitVal使e = 2.0f;
    InkReso使本ce.Desc本iption = TEXT("書寫和印刷的墨水");
    InkReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Ink");
    InkReso使本ce.bIsStackable = t本使e;
    InkReso使本ce.bIsPe本ishable = false;
    InkReso使本ce.Ta成s.Add(TEXT("Statione本y"));
    InkReso使本ce.Ta成s.Add(TEXT("Co設置設置使nication"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Ink, InkReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateBooksReso使本ce()
{
    軍Reso使本ceData BooksReso使本ce;
    BooksReso使本ce.Reso使本ceType = EReso使本ceType::Books;
    BooksReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    BooksReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    BooksReso使本ce.Q使antity = 0.0f;
    BooksReso使本ce.MaxCapacity = 200.0f;
    BooksReso使本ce.Unit基本ei成ht = 0.5f;
    BooksReso使本ce.UnitVal使e = 10.0f;
    BooksReso使本ce.Desc本iption = TEXT("知識和文化的載體");
    BooksReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Books");
    BooksReso使本ce.bIsStackable = false;
    BooksReso使本ce.bIsPe本ishable = false;
    BooksReso使本ce.Ta成s.Add(TEXT("Knowled成e"));
    BooksReso使本ce.Ta成s.Add(TEXT("C使lt使本al"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Books, BooksReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateToolsReso使本ce()
{
    軍Reso使本ceData ToolsReso使本ce;
    ToolsReso使本ce.Reso使本ceType = EReso使本ceType::Tools;
    ToolsReso使本ce.Ra本ity = EReso使本ceRa本ity::Co設置設置on;
    ToolsReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    ToolsReso使本ce.Q使antity = 0.0f;
    ToolsReso使本ce.MaxCapacity = 300.0f;
    ToolsReso使本ce.Unit基本ei成ht = 2.0f;
    ToolsReso使本ce.UnitVal使e = 3.0f;
    ToolsReso使本ce.Desc本iption = TEXT("生產和建設的基本工具");
    ToolsReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Tools");
    ToolsReso使本ce.bIsStackable = false;
    ToolsReso使本ce.bIsPe本ishable = false;
    ToolsReso使本ce.Ta成s.Add(TEXT("Eq使ip設置ent"));
    ToolsReso使本ce.Ta成s.Add(TEXT("P本od使cti正e"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Tools, ToolsReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateMachine本yReso使本ce()
{
    軍Reso使本ceData Machine本yReso使本ce;
    Machine本yReso使本ce.Reso使本ceType = EReso使本ceType::Machine本y;
    Machine本yReso使本ce.Ra本ity = EReso使本ceRa本ity::Ra本e;
    Machine本yReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    Machine本yReso使本ce.Q使antity = 0.0f;
    Machine本yReso使本ce.MaxCapacity = 100.0f;
    Machine本yReso使本ce.Unit基本ei成ht = 50.0f;
    Machine本yReso使本ce.UnitVal使e = 50.0f;
    Machine本yReso使本ce.Desc本iption = TEXT("現代化生產設備");
    Machine本yReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Machine本y");
    Machine本yReso使本ce.bIsStackable = false;
    Machine本yReso使本ce.bIsPe本ishable = false;
    Machine本yReso使本ce.Ta成s.Add(TEXT("Ind使st本ial"));
    Machine本yReso使本ce.Ta成s.Add(TEXT("Mode本n"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Machine本y, Machine本yReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateElect本onicsReso使本ce()
{
    軍Reso使本ceData Elect本onicsReso使本ce;
    Elect本onicsReso使本ce.Reso使本ceType = EReso使本ceType::Elect本onics;
    Elect本onicsReso使本ce.Ra本ity = EReso使本ceRa本ity::Ra本e;
    Elect本onicsReso使本ce.Q使ality = EReso使本ceQ使ality::Excellent;
    Elect本onicsReso使本ce.Q使antity = 0.0f;
    Elect本onicsReso使本ce.MaxCapacity = 50.0f;
    Elect本onicsReso使本ce.Unit基本ei成ht = 5.0f;
    Elect本onicsReso使本ce.UnitVal使e = 80.0f;
    Elect本onicsReso使本ce.Desc本iption = TEXT("現代電子設備");
    Elect本onicsReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Elect本onics");
    Elect本onicsReso使本ce.bIsStackable = false;
    Elect本onicsReso使本ce.bIsPe本ishable = false;
    Elect本onicsReso使本ce.Ta成s.Add(TEXT("Mode本n"));
    Elect本onicsReso使本ce.Ta成s.Add(TEXT("Ad正anced"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Elect本onics, Elect本onicsReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateRadioReso使本ce()
{
    軍Reso使本ceData RadioReso使本ce;
    RadioReso使本ce.Reso使本ceType = EReso使本ceType::Radio;
    RadioReso使本ce.Ra本ity = EReso使本ceRa本ity::Ra本e;
    RadioReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    RadioReso使本ce.Q使antity = 0.0f;
    RadioReso使本ce.MaxCapacity = 30.0f;
    RadioReso使本ce.Unit基本ei成ht = 10.0f;
    RadioReso使本ce.UnitVal使e = 60.0f;
    RadioReso使本ce.Desc本iption = TEXT("無線電通訊設備");
    RadioReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Radio");
    RadioReso使本ce.bIsStackable = false;
    RadioReso使本ce.bIsPe本ishable = false;
    RadioReso使本ce.Ta成s.Add(TEXT("Co設置設置使nication"));
    RadioReso使本ce.Ta成s.Add(TEXT("Mode本n"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Radio, RadioReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateTelephoneReso使本ce()
{
    軍Reso使本ceData TelephoneReso使本ce;
    TelephoneReso使本ce.Reso使本ceType = EReso使本ceType::Telephone;
    TelephoneReso使本ce.Ra本ity = EReso使本ceRa本ity::Ra本e;
    TelephoneReso使本ce.Q使ality = EReso使本ceQ使ality::Good;
    TelephoneReso使本ce.Q使antity = 0.0f;
    TelephoneReso使本ce.MaxCapacity = 50.0f;
    TelephoneReso使本ce.Unit基本ei成ht = 3.0f;
    TelephoneReso使本ce.UnitVal使e = 40.0f;
    TelephoneReso使本ce.Desc本iption = TEXT("有線電話通訊設備");
    TelephoneReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Telephone");
    TelephoneReso使本ce.bIsStackable = false;
    TelephoneReso使本ce.bIsPe本ishable = false;
    TelephoneReso使本ce.Ta成s.Add(TEXT("Co設置設置使nication"));
    TelephoneReso使本ce.Ta成s.Add(TEXT("Mode本n"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Telephone, TelephoneReso使本ce);
}

正oid UMin成GoRTSReso使本ceSyste設置::C本eateTele成本aphReso使本ce()
{
    軍Reso使本ceData Tele成本aphReso使本ce;
    Tele成本aphReso使本ce.Reso使本ceType = EReso使本ceType::Tele成本aph;
    Tele成本aphReso使本ce.Ra本ity = EReso使本ceRa本ity::Unco設置設置on;
    Tele成本aphReso使本ce.Q使ality = EReso使本ceQ使ality::的o本設置al;
    Tele成本aphReso使本ce.Q使antity = 0.0f;
    Tele成本aphReso使本ce.MaxCapacity = 80.0f;
    Tele成本aphReso使本ce.Unit基本ei成ht = 15.0f;
    Tele成本aphReso使本ce.UnitVal使e = 25.0f;
    Tele成本aphReso使本ce.Desc本iption = TEXT("電報通訊設備");
    Tele成本aphReso使本ce.IconPath = TEXT("/Ga設置e/Icons/Reso使本ces/Tele成本aph");
    Tele成本aphReso使本ce.bIsStackable = false;
    Tele成本aphReso使本ce.bIsPe本ishable = false;
    Tele成本aphReso使本ce.Ta成s.Add(TEXT("Co設置設置使nication"));
    Tele成本aphReso使本ce.Ta成s.Add(TEXT("Mode本n"));
    
    Reso使本ceInfoMap.Add(EReso使本ceType::Tele成本aph, Tele成本aphReso使本ce);
}

// 輔助方法實現
正oid UMin成GoRTSReso使本ceSyste設置::UpdateCollectionYield(const 軍St本in成& CollectionID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n;
    }

    軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    Collection.C使本本entYield = Collection.BaseYield * Collection.Efficiency;
}

float UMin成GoRTSReso使本ceSyste設置::Calc使lateEn正i本on設置entalI設置pact(const 軍St本in成& CollectionID) const
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n 1.0f;
    }

    const 軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    float En正i本on設置entalI設置pact = 1.0f;
    
    // 計算環境因素影響
    fo本 (const a使to& 軍acto本Pai本 : Collection.En正i本on設置ental軍acto本s)
    {
        En正i本on設置entalI設置pact *= 軍acto本Pai本.Val使e;
    }
    
    本et使本n 軍Math::Cla設置p(En正i本on設置entalI設置pact, 0.5f, 1.5f);
}

bool UMin成GoRTSReso使本ceSyste設置::CheckCollectionConditions(const 軍St本in成& CollectionID) const
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n false;
    }

    const 軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    // 檢查收集者是否有必要的工具
    // 這裡可以添加更複雜的條件檢查
    本et使本n t本使e;
}

正oid UMin成GoRTSReso使本ceSyste設置::UpdateSto本a成eCapacity(const 軍St本in成& Sto本a成eID)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        本et使本n;
    }

    軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    Sto本a成e.UsedCapacity = 0.0f;
    
    // 計算已使用容量
    fo本 (const a使to& Reso使本cePai本 : Sto本a成e.Sto本edReso使本ces)
    {
        const 軍Reso使本ceData& Reso使本ce = Reso使本cePai本.Val使e;
        Sto本a成e.UsedCapacity += Reso使本ce.Q使antity * Reso使本ce.Unit基本ei成ht;
    }
    
    Sto本a成e.C使本本entCapacity = Sto本a成e.UsedCapacity / Sto本a成e.MaxCapacity;
}

bool UMin成GoRTSReso使本ceSyste設置::CheckSto本a成eLi設置its(const 軍St本in成& Sto本a成eID, EReso使本ceType Reso使本ceType, float Q使antity) const
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        本et使本n false;
    }

    const 軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    // 檢查是否允許此資源類型
    if (Sto本a成e.AllowedTypes.的使設置() > 0 && !Sto本a成e.AllowedTypes.Contains(Reso使本ceType))
    {
        本et使本n false;
    }
    
    // 檢查容量限制
    軍Reso使本ceData Reso使本ceInfo = GetReso使本ceInfo(Reso使本ceType);
    float Req使i本edCapacity = Q使antity * Reso使本ceInfo.Unit基本ei成ht;
    
    本et使本n (Sto本a成e.UsedCapacity + Req使i本edCapacity) <= Sto本a成e.MaxCapacity;
}

正oid UMin成GoRTSReso使本ceSyste設置::輸入andleSto本a成e軍使ll(const 軍St本in成& Sto本a成eID)
{
    // 廣播存儲滿了事件
    OnSto本a成e軍使ll.B本oadcast(Sto本a成eID, GetSto本a成eCapacity(Sto本a成eID));
    
    // 添加歷史記錄
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("存儲已滿：%s"), *Sto本a成eID));
}

float UMin成GoRTSReso使本ceSyste設置::Calc使lateCon正e本sionCost(const 軍St本in成& Con正e本sionID) const
{
    if (!Reso使本ceCon正e本sions.Contains(Con正e本sionID))
    {
        本et使本n 0.0f;
    }

    const 軍Reso使本ceCon正e本sion& Con正e本sion = Reso使本ceCon正e本sions[Con正e本sionID];
    float TotalCost = 0.0f;
    
    // 計算輸入資源的總價值
    fo本 (const a使to& Inp使tPai本 : Con正e本sion.Inp使tReso使本ces)
    {
        軍Reso使本ceData Reso使本ceInfo = GetReso使本ceInfo(Inp使tPai本.Key);
        TotalCost += Reso使本ceInfo.UnitVal使e * Inp使tPai本.Val使e;
    }
    
    本et使本n TotalCost;
}

bool UMin成GoRTSReso使本ceSyste設置::CheckCon正e本sionConditions(const 軍St本in成& Con正e本sionID) const
{
    if (!Reso使本ceCon正e本sions.Contains(Con正e本sionID))
    {
        本et使本n false;
    }

    const 軍Reso使本ceCon正e本sion& Con正e本sion = Reso使本ceCon正e本sions[Con正e本sionID];
    
    // 檢查轉換是否可用
    if (!Con正e本sion.bIsA正ailable)
    {
        本et使本n false;
    }
    
    // 檢查是否有必要的建築和技能
    // 這裡可以添加更複雜的條件檢查
    
    本et使本n t本使e;
}

正oid UMin成GoRTSReso使本ceSyste設置::P本ocessCon正e本sionCo設置plete(const 軍St本in成& Con正e本sionID)
{
    // 添加歷史記錄
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("轉換完成：%s"), *Con正e本sionID));
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::Gene本ateCollectionID(EReso使本ceType Reso使本ceType) const
{
    本et使本n 軍St本in成::P本intf(TEXT("Collection下%s下%s"), *GetReso使本ceType的a設置e(Reso使本ceType), *軍DateTi設置e::的ow().ToSt本in成());
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::Gene本ateSto本a成eID(const 軍St本in成& Sto本a成e的a設置e) const
{
    本et使本n 軍St本in成::P本intf(TEXT("Sto本a成e下%s下%s"), *Sto本a成e的a設置e, *軍DateTi設置e::的ow().ToSt本in成());
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::Gene本ateCon正e本sionID(const 軍St本in成& Con正e本sion的a設置e) const
{
    本et使本n 軍St本in成::P本intf(TEXT("Con正e本sion下%s下%s"), *Con正e本sion的a設置e, *軍DateTi設置e::的ow().ToSt本in成());
}

bool UMin成GoRTSReso使本ceSyste設置::ValidateReso使本ceData(const 軍Reso使本ceData& Reso使本ceData) const
{
    本et使本n Reso使本ceData.Q使antity >= 0.0f && Reso使本ceData.Unit基本ei成ht > 0.0f && Reso使本ceData.UnitVal使e > 0.0f;
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::GetReso使本ceType的a設置e(EReso使本ceType Reso使本ceType) const
{
    本et使本n StaticEn使設置<EReso使本ceType>()->GetVal使eAsSt本in成(Reso使本ceType);
}

float UMin成GoRTSReso使本ceSyste設置::GetReso使本ceType基本ei成ht(EReso使本ceType Reso使本ceType) const
{
    軍Reso使本ceData Reso使本ceInfo = GetReso使本ceInfo(Reso使本ceType);
    本et使本n Reso使本ceInfo.Unit基本ei成ht;
}

float UMin成GoRTSReso使本ceSyste設置::GetCollectionMethodBon使s(ECollectionMethod Method) const
{
    switch (Method)
    {
    case ECollectionMethod::Man使al:
        本et使本n 1.0f;
    case ECollectionMethod::A使to設置atic:
        本et使本n 1.2f;
    case ECollectionMethod::P本od使ction:
        本et使本n 1.5f;
    case ECollectionMethod::T本ade:
        本et使本n 1.1f;
    case ECollectionMethod::Pl使nde本:
        本et使本n 0.8f;
    case ECollectionMethod::輸入a本正est:
        本et使本n 1.3f;
    case ECollectionMethod::Minin成:
        本et使本n 1.2f;
    case ECollectionMethod::軍ishin成:
        本et使本n 1.1f;
    case ECollectionMethod::輸入使ntin成:
        本et使本n 1.0f;
    case ECollectionMethod::軍o本a成in成:
        本et使本n 0.9f;
    defa使lt:
        本et使本n 1.0f;
    }
}

float UMin成GoRTSReso使本ceSyste設置::GetQ使alityBon使s(EReso使本ceQ使ality Q使ality) const
{
    switch (Q使ality)
    {
    case EReso使本ceQ使ality::Poo本:
        本et使本n 0.5f;
    case EReso使本ceQ使ality::的o本設置al:
        本et使本n 1.0f;
    case EReso使本ceQ使ality::Good:
        本et使本n 1.5f;
    case EReso使本ceQ使ality::Excellent:
        本et使本n 2.0f;
    case EReso使本ceQ使ality::Pe本fect:
        本et使本n 3.0f;
    defa使lt:
        本et使本n 1.0f;
    }
}

float UMin成GoRTSReso使本ceSyste設置::Calc使lateReso使本ceTotalVal使e(const 軍Reso使本ceData& Reso使本ceData) const
{
    本et使本n Reso使本ceData.Q使antity * Reso使本ceData.UnitVal使e * GetQ使alityBon使s(Reso使本ceData.Q使ality);
}

bool UMin成GoRTSReso使本ceSyste設置::CheckReso使本ceCo設置patibility(EReso使本ceType Reso使本ceType1, EReso使本ceType Reso使本ceType2) const
{
    // 簡化實現：所有資源都兼容
    本et使本n t本使e;
}

ECollectionMethod UMin成GoRTSReso使本ceSyste設置::GetReco設置設置endedCollectionMethod(EReso使本ceType Reso使本ceType) const
{
    // 根據資源類型推薦收集方法
    switch (Reso使本ceType)
    {
    case EReso使本ceType::基本ood:
    case EReso使本ceType::Stone:
        本et使本n ECollectionMethod::輸入a本正est;
    case EReso使本ceType::I本on:
    case EReso使本ceType::Coppe本:
    case EReso使本ceType::Coal:
        本et使本n ECollectionMethod::Minin成;
    case EReso使本ceType::軍ood:
        本et使本n ECollectionMethod::輸入使ntin成;
    case EReso使本ceType::基本ate本:
        本et使本n ECollectionMethod::輸入a本正est;
    case EReso使本ceType::Cotton:
    case EReso使本ceType::Tea:
        本et使本n ECollectionMethod::輸入a本正est;
    case EReso使本ceType::Silk:
        本et使本n ECollectionMethod::輸入a本正est;
    case EReso使本ceType::Opi使設置:
        本et使本n ECollectionMethod::輸入a本正est;
    defa使lt:
        本et使本n ECollectionMethod::Man使al;
    }
}

TA本本ay<軍St本in成> UMin成GoRTSReso使本ceSyste設置::GetReso使本ceCollectionLocations(EReso使本ceType Reso使本ceType) const
{
    TA本本ay<軍St本in成> Locations;
    
    // 根據資源類型返回收集地點
    switch (Reso使本ceType)
    {
    case EReso使本ceType::基本ood:
        Locations.Add(TEXT("軍o本est"));
        Locations.Add(TEXT("Mo使ntain"));
        b本eak;
    case EReso使本ceType::Stone:
        Locations.Add(TEXT("Mo使ntain"));
        Locations.Add(TEXT("Q使a本本y"));
        b本eak;
    case EReso使本ceType::I本on:
    case EReso使本ceType::Coppe本:
        Locations.Add(TEXT("Mine"));
        Locations.Add(TEXT("Mo使ntain"));
        b本eak;
    case EReso使本ceType::軍ood:
        Locations.Add(TEXT("軍a本設置"));
        Locations.Add(TEXT("輸入使ntin成G本o使nd"));
        b本eak;
    defa使lt:
        Locations.Add(TEXT("Base"));
        b本eak;
    }
    
    本et使本n Locations;
}

TA本本ay<軍St本in成> UMin成GoRTSReso使本ceSyste設置::GetReso使本ceSto本a成eReco設置設置endations(EReso使本ceType Reso使本ceType) const
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    
    // 根據資源類型給存儲建議
    if (Reso使本ceType == EReso使本ceType::軍ood)
    {
        Reco設置設置endations.Add(TEXT("需要冷藏存儲"));
        Reco設置設置endations.Add(TEXT("定期檢查過期"));
    }
    else if (Reso使本ceType == EReso使本ceType::基本ate本)
    {
        Reco設置設置endations.Add(TEXT("需要密封容器"));
        Reco設置設置endations.Add(TEXT("定期檢查清潔度"));
    }
    else if (Reso使本ceType == EReso使本ceType::基本eapons)
    {
        Reco設置設置endations.Add(TEXT("需要安全存儲"));
        Reco設置設置endations.Add(TEXT("定期維護保養"));
    }
    
    本et使本n Reco設置設置endations;
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::Gene本ateReso使本ceRepo本t() const
{
    軍St本in成 Repo本t = TEXT("資源系統報告\n");
    Repo本t += 軍St本in成::P本intf(TEXT("資源類型數量：%d\n"), Reso使本ceInfoMap.的使設置());
    Repo本t += 軍St本in成::P本intf(TEXT("活動收集數量：%d\n"), GetActi正eCollections().的使設置());
    Repo本t += 軍St本in成::P本intf(TEXT("存儲數量：%d\n"), Reso使本ceSto本a成es.的使設置());
    Repo本t += 軍St本in成::P本intf(TEXT("轉換數量：%d\n"), Reso使本ceCon正e本sions.的使設置());
    
    本et使本n Repo本t;
}

正oid UMin成GoRTSReso使本ceSyste設置::AddReso使本ce輸入isto本y(const 軍St本in成& E正ent)
{
    Reso使本ce輸入isto本y.Add(E正ent);
    
    // 限制歷史記錄數量
    if (Reso使本ce輸入isto本y.的使設置() > 1000)
    {
        Reso使本ce輸入isto本y.Re設置o正eAt(0);
    }
}

TA本本ay<軍St本in成> UMin成GoRTSReso使本ceSyste設置::GetReso使本ce輸入isto本y() const
{
    本et使本n Reso使本ce輸入isto本y;
}

正oid UMin成GoRTSReso使本ceSyste設置::SetReq使i本edToolsAndSkills(軍Reso使本ceCollection& Collection)
{
    // 根據資源類型和收集方法設置所需工具和技能
    switch (Collection.Reso使本ceType)
    {
    case EReso使本ceType::基本ood:
        Collection.Req使i本edTools.Add(TEXT("Axe"));
        Collection.Req使i本edSkills.Add(TEXT("L使設置be本大ack"));
        b本eak;
    case EReso使本ceType::Stone:
        Collection.Req使i本edTools.Add(TEXT("Pickaxe"));
        Collection.Req使i本edSkills.Add(TEXT("Minin成"));
        b本eak;
    case EReso使本ceType::I本on:
    case EReso使本ceType::Coppe本:
        Collection.Req使i本edTools.Add(TEXT("Pickaxe"));
        Collection.Req使i本edTools.Add(TEXT("Sho正el"));
        Collection.Req使i本edSkills.Add(TEXT("Minin成"));
        b本eak;
    case EReso使本ceType::軍ood:
        Collection.Req使i本edTools.Add(TEXT("輸入使ntin成Rifle"));
        Collection.Req使i本edSkills.Add(TEXT("輸入使ntin成"));
        b本eak;
    defa使lt:
        b本eak;
    }
}

正oid UMin成GoRTSReso使本ceSyste設置::SetEn正i本on設置ental軍acto本s(軍Reso使本ceCollection& Collection)
{
    // 根據收集地點設置環境因素
    Collection.En正i本on設置ental軍acto本s.Add(TEXT("基本eathe本"), 1.0f);
    Collection.En正i本on設置ental軍acto本s.Add(TEXT("Season"), 1.0f);
    Collection.En正i本on設置ental軍acto本s.Add(TEXT("Ti設置eOfDay"), 1.0f);
    Collection.En正i本on設置ental軍acto本s.Add(TEXT("Te本本ain"), 1.0f);
}

正oid UMin成GoRTSReso使本ceSyste設置::SetAllowedReso使本ceTypes(軍Reso使本ceSto本a成e& Sto本a成e)
{
    // 根據存儲類型設置允許的資源類型
    // 簡化實現：允許所有資源類型
}

float UMin成GoRTSReso使本ceSyste設置::Calc使lateQ使alityI設置p本o正e設置entCost(EReso使本ceQ使ality C使本本entQ使ality, EReso使本ceQ使ality 的ewQ使ality) const
{
    float CostM使ltiplie本 = 1.0f;
    
    // 根據質量差異計算成本
    switch (C使本本entQ使ality)
    {
    case EReso使本ceQ使ality::Poo本:
        CostM使ltiplie本 = 1.0f;
        b本eak;
    case EReso使本ceQ使ality::的o本設置al:
        CostM使ltiplie本 = 2.0f;
        b本eak;
    case EReso使本ceQ使ality::Good:
        CostM使ltiplie本 = 3.0f;
        b本eak;
    case EReso使本ceQ使ality::Excellent:
        CostM使ltiplie本 = 5.0f;
        b本eak;
    case EReso使本ceQ使ality::Pe本fect:
        CostM使ltiplie本 = 10.0f;
        b本eak;
    }
    
    本et使本n CostM使ltiplie本;
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::軍indS使itableSto本a成e(EReso使本ceType Reso使本ceType) const
{
    // 找到合適的存儲
    fo本 (const a使to& Sto本a成ePai本 : Reso使本ceSto本a成es)
    {
        const 軍Reso使本ceSto本a成e& Sto本a成e = Sto本a成ePai本.Val使e;
        
        // 檢查是否允許此資源類型
        if (Sto本a成e.AllowedTypes.的使設置() > 0 && !Sto本a成e.AllowedTypes.Contains(Reso使本ceType))
        {
            contin使e;
        }
        
        // 檢查是否有足夠容量
        軍Reso使本ceData Reso使本ceInfo = GetReso使本ceInfo(Reso使本ceType);
        if (Sto本a成e.UsedCapacity + Reso使本ceInfo.Unit基本ei成ht <= Sto本a成e.MaxCapacity)
        {
            本et使本n Sto本a成e.Sto本a成eID;
        }
    }
    
    本et使本n TEXT("");
}

正oid UMin成GoRTSReso使本ceSyste設置::Ad大使st輸入isto本icalReso使本ceVal使es()
{
    // 根據民國時期的歷史背景調整資源價值
    // 這裡可以根據歷史資料調整各種資源的稀有度和價值
}

正oid UMin成GoRTSReso使本ceSyste設置::UpdateCollectionYield(const 軍St本in成& CollectionID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n;
    }

    軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    // 計算基礎產量
    float BaseYield = Collection.BaseYield;
    
    // 應用效率修正
    float EfficiencyModifie本 = Collection.Efficiency;
    
    // 應用收集方法加成
    float MethodBon使s = GetCollectionMethodBon使s(Collection.CollectionMethod);
    
    // 應用環境因素
    float En正i本on設置ental軍acto本 = Calc使lateEn正i本on設置entalI設置pact(CollectionID);
    
    // 計算最終產量
    Collection.C使本本entYield = BaseYield * EfficiencyModifie本 * MethodBon使s * En正i本on設置ental軍acto本;
}

float UMin成GoRTSReso使本ceSyste設置::Calc使lateEn正i本on設置entalI設置pact(const 軍St本in成& CollectionID) const
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n 1.0f;
    }

    const 軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    float TotalI設置pact = 1.0f;
    
    // 計算所有環境因素的影響
    fo本 (const a使to& 軍acto本Pai本 : Collection.En正i本on設置ental軍acto本s)
    {
        TotalI設置pact *= 軍acto本Pai本.Val使e;
    }
    
    本et使本n 軍Math::Cla設置p(TotalI設置pact, 0.1f, 2.0f);
}

bool UMin成GoRTSReso使本ceSyste設置::CheckCollectionConditions(const 軍St本in成& CollectionID) const
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n false;
    }

    const 軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    // 檢查是否有必要的工具（這裡簡化實現）
    if (Collection.Req使i本edTools.的使設置() > 0)
    {
        // TODO: 檢查工具可用性
    }
    
    // 檢查是否有必要的技能（這裡簡化實現）
    if (Collection.Req使i本edSkills.的使設置() > 0)
    {
        // TODO: 檢查技能可用性
    }
    
    本et使本n t本使e;
}

正oid UMin成GoRTSReso使本ceSyste設置::P本ocessCollectionCo設置plete(const 軍St本in成& CollectionID)
{
    if (!Reso使本ceCollections.Contains(CollectionID))
    {
        本et使本n;
    }

    軍Reso使本ceCollection& Collection = Reso使本ceCollections[CollectionID];
    
    // 處理收集完成的邏輯
    // 可以在這裡添加獎勵、經驗值等
    
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("收集完成：%s"), *CollectionID));
}

正oid UMin成GoRTSReso使本ceSyste設置::UpdateSto本a成eCapacity(const 軍St本in成& Sto本a成eID)
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        本et使本n;
    }

    軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    // 計算已使用容量
    float UsedCapacity = 0.0f;
    fo本 (const a使to& Reso使本cePai本 : Sto本a成e.Sto本edReso使本ces)
    {
        const 軍Reso使本ceData& Reso使本ce = Reso使本cePai本.Val使e;
        UsedCapacity += Reso使本ce.Q使antity * Reso使本ce.Unit基本ei成ht;
    }
    
    Sto本a成e.UsedCapacity = UsedCapacity;
    Sto本a成e.C使本本entCapacity = Sto本a成e.MaxCapacity - UsedCapacity;
}

bool UMin成GoRTSReso使本ceSyste設置::CheckSto本a成eLi設置its(const 軍St本in成& Sto本a成eID, EReso使本ceType Reso使本ceType, float Q使antity) const
{
    if (!Reso使本ceSto本a成es.Contains(Sto本a成eID))
    {
        本et使本n false;
    }

    const 軍Reso使本ceSto本a成e& Sto本a成e = Reso使本ceSto本a成es[Sto本a成eID];
    
    // 檢查是否允許此資源類型
    if (Sto本a成e.AllowedTypes.的使設置() > 0 && !Sto本a成e.AllowedTypes.Contains(Reso使本ceType))
    {
        本et使本n false;
    }
    
    // 檢查容量限制
    軍Reso使本ceData Reso使本ceInfo = GetReso使本ceInfo(Reso使本ceType);
    float Req使i本edCapacity = Q使antity * Reso使本ceInfo.Unit基本ei成ht;
    
    本et使本n (Sto本a成e.UsedCapacity + Req使i本edCapacity) <= Sto本a成e.MaxCapacity;
}

正oid UMin成GoRTSReso使本ceSyste設置::輸入andleSto本a成e軍使ll(const 軍St本in成& Sto本a成eID)
{
    // 處理存儲滿了的情況
    OnSto本a成e軍使ll.B本oadcast(Sto本a成eID, GetSto本a成eCapacity(Sto本a成eID));
    
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("存儲已滿：%s"), *Sto本a成eID));
}

float UMin成GoRTSReso使本ceSyste設置::Calc使lateCon正e本sionCost(const 軍St本in成& Con正e本sionID) const
{
    if (!Reso使本ceCon正e本sions.Contains(Con正e本sionID))
    {
        本et使本n 0.0f;
    }

    const 軍Reso使本ceCon正e本sion& Con正e本sion = Reso使本ceCon正e本sions[Con正e本sionID];
    float TotalCost = 0.0f;
    
    // 計算輸入資源的總價值
    fo本 (const a使to& Inp使tPai本 : Con正e本sion.Inp使tReso使本ces)
    {
        軍Reso使本ceData Reso使本ceInfo = GetReso使本ceInfo(Inp使tPai本.Key);
        TotalCost += Reso使本ceInfo.UnitVal使e * Inp使tPai本.Val使e;
    }
    
    本et使本n TotalCost;
}

bool UMin成GoRTSReso使本ceSyste設置::CheckCon正e本sionConditions(const 軍St本in成& Con正e本sionID) const
{
    if (!Reso使本ceCon正e本sions.Contains(Con正e本sionID))
    {
        本et使本n false;
    }

    const 軍Reso使本ceCon正e本sion& Con正e本sion = Reso使本ceCon正e本sions[Con正e本sionID];
    
    // 檢查轉換是否可用
    if (!Con正e本sion.bIsA正ailable)
    {
        本et使本n false;
    }
    
    // 檢查是否有必要的建築（這裡簡化實現）
    if (Con正e本sion.Req使i本edB使ildin成s.的使設置() > 0)
    {
        // TODO: 檢查建築可用性
    }
    
    // 檢查是否有必要的技能（這裡簡化實現）
    if (Con正e本sion.Req使i本edSkills.的使設置() > 0)
    {
        // TODO: 檢查技能可用性
    }
    
    本et使本n t本使e;
}

正oid UMin成GoRTSReso使本ceSyste設置::P本ocessCon正e本sionCo設置plete(const 軍St本in成& Con正e本sionID)
{
    if (!Reso使本ceCon正e本sions.Contains(Con正e本sionID))
    {
        本et使本n;
    }

    // 處理轉換完成的邏輯
    AddReso使本ce輸入isto本y(軍St本in成::P本intf(TEXT("轉換完成：%s"), *Con正e本sionID));
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::Gene本ateCollectionID(EReso使本ceType Reso使本ceType) const
{
    本et使本n 軍St本in成::P本intf(TEXT("COLL下%s下%d"), *StaticEn使設置<EReso使本ceType>()->GetVal使eAsSt本in成(Reso使本ceType), 軍Math::RandRan成e(1000, 9999));
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::Gene本ateSto本a成eID(const 軍St本in成& Sto本a成e的a設置e) const
{
    本et使本n 軍St本in成::P本intf(TEXT("STOR下%s下%d"), *Sto本a成e的a設置e, 軍Math::RandRan成e(1000, 9999));
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::Gene本ateCon正e本sionID(const 軍St本in成& Con正e本sion的a設置e) const
{
    本et使本n 軍St本in成::P本intf(TEXT("CO的V下%s下%d"), *Con正e本sion的a設置e, 軍Math::RandRan成e(1000, 9999));
}

軍St本in成 UMin成GoRTSReso使本ceSyste設置::GetReso使本ceType的a設置e(EReso使本ceType Reso使本ceType) const
{
    const UEn使設置* En使設置Pt本 = StaticEn使設置<EReso使本ceType>();
    if (En使設置Pt本)
    {
        本et使本n En使設置Pt本->Get的a設置eSt本in成ByVal使e(static下cast<int64>(Reso使本ceType));
    }
    本et使本n TEXT("Unknown");
}

float UMin成GoRTSReso使本ceSyste設置::GetReso使本ceType基本ei成ht(EReso使本ceType Reso使本ceType) const
{
    軍Reso使本ceData Reso使本ceInfo = GetReso使本ceInfo(Reso使本ceType);
    本et使本n Reso使本ceInfo.Unit基本ei成ht;
}

float UMin成GoRTSReso使本ceSyste設置::GetCollectionMethodBon使s(ECollectionMethod Method) const
{
    switch (Method)
    {
    case ECollectionMethod::Man使al:
        本et使本n 1.0f;
    case ECollectionMethod::A使to設置atic:
        本et使本n 1.2f;
    case ECollectionMethod::P本od使ction:
        本et使本n 1.5f;
    case ECollectionMethod::T本ade:
        本et使本n 1.1f;
    case ECollectionMethod::Pl使nde本:
        本et使本n 0.8f;
    case ECollectionMethod::輸入a本正est:
        本et使本n 1.3f;
    case ECollectionMethod::Minin成:
        本et使本n 1.1f;
    case ECollectionMethod::軍ishin成:
        本et使本n 1.2f;
    case ECollectionMethod::輸入使ntin成:
        本et使本n 1.1f;
    case ECollectionMethod::軍o本a成in成:
        本et使本n 0.9f;
    defa使lt:
        本et使本n 1.0f;
    }
}

float UMin成GoRTSReso使本ceSyste設置::GetQ使alityBon使s(EReso使本ceQ使ality Q使ality) const
{
    switch (Q使ality)
    {
    case EReso使本ceQ使ality::Poo本:
        本et使本n 0.5f;
    case EReso使本ceQ使ality::的o本設置al:
        本et使本n 1.0f;
    case EReso使本ceQ使ality::Good:
        本et使本n 1.5f;
    case EReso使本ceQ使ality::Excellent:
        本et使本n 2.0f;
    case EReso使本ceQ使ality::Pe本fect:
        本et使本n 3.0f;
    defa使lt:
        本et使本n 1.0f;
    }
}

float UMin成GoRTSReso使本ceSyste設置::Calc使lateReso使本ceTotalVal使e(const 軍Reso使本ceData& Reso使本ceData) const
{
    本et使本n Reso使本ceData.Q使antity * Reso使本ceData.UnitVal使e * GetQ使alityBon使s(Reso使本ceData.Q使ality);
}

bool UMin成GoRTSReso使本ceSyste設置::CheckReso使本ceCo設置patibility(EReso使本ceType Reso使本ceType1, EReso使本ceType Reso使本ceType2) const
{
    // 簡化實現：大部分資源都兼容
    本et使本n t本使e;
}

ECollectionMethod UMin成GoRTSReso使本ceSyste設置::GetReco設置設置endedCollectionMethod(EReso使本ceType Reso使本ceType) const
{
    switch (Reso使本ceType)
    {
    case EReso使本ceType::基本ood:
        本et使本n ECollectionMethod::輸入a本正est;
    case EReso使本ceType::Stone:
    case EReso使本ceType::I本on:
    case EReso使本ceType::Coppe本:
    case EReso使本ceType::Coal:
        本et使本n ECollectionMethod::Minin成;
    case EReso使本ceType::軍ood:
        本et使本n ECollectionMethod::輸入使ntin成;
    case EReso使本ceType::基本ate本:
        本et使本n ECollectionMethod::軍o本a成in成;
    defa使lt:
        本et使本n ECollectionMethod::Man使al;
    }
}
