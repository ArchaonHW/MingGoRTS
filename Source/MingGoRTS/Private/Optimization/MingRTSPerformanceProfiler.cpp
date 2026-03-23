#incl使de "Min成RTSPe本fo本設置anceP本ofile本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "輸入AL/Platfo本設置P本ocess.h"

UMin成RTSPe本fo本設置anceP本ofile本::UMin成RTSPe本fo本設置anceP本ofile本()
{
    Monito本in成Inte本正al = 1.0f; // 1秒間隔
    bEnableRealTi設置eMonito本in成 = t本使e;
    Max輸入isto本icalDataPoints = 100;
    bEnableA使to設置aticOpti設置ization = false;
    bIsRealTi設置eMonito本in成Acti正e = false;
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::InitializeP本ofile本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Initializin成 pe本fo本設置ance p本ofile本..."));

    // 初始化性能閾值
    InitializePe本fo本設置anceTh本esholds();

    // 初始化當前指標
    InitializeC使本本entMet本ics();

    // 設置監控定時器
    Set使pMonito本in成Ti設置e本();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Pe本fo本設置ance p本ofile本 initialized"));
}

軍St本in成 UMin成RTSPe本fo本設置anceP本ofile本::Sta本tP本ofilin成Session(EP本ofilin成Type P本ofilin成Type, const 軍St本in成& Session的a設置e)
{
    軍St本in成 SessionID = 軍St本in成::P本intf(TEXT("Session下%s下%d"), *Session的a設置e, 軍Math::RandRan成e(1000, 9999));
    
    軍P本ofilin成Session 的ewSession;
    的ewSession.SessionID = SessionID;
    的ewSession.Session的a設置e = Session的a設置e;
    的ewSession.P本ofilin成Type = P本ofilin成Type;
    的ewSession.Sta本tTi設置e = 軍DateTi設置e::的ow();
    的ewSession.bIsActi正e = t本使e;
    
    // 初始化會話指標
    InitializeSessionMet本ics(的ewSession, P本ofilin成Type);
    
    // 添加到會話存儲
    P本ofilin成Sessions.Add(SessionID, 的ewSession);
    Acti正eSessions.Add(SessionID);
    
    // 觸發事件
    OnP本ofilin成Sta本ted.B本oadcast(SessionID, P本ofilin成Type);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Sta本ted p本ofilin成 session %s fo本 %s"), *SessionID, *GetP本ofilin成Type的a設置e(P本ofilin成Type));
    本et使本n SessionID;
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::StopP本ofilin成Session(const 軍St本in成& SessionID)
{
    if (!P本ofilin成Sessions.Contains(SessionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Session %s not fo使nd"), *SessionID);
        本et使本n;
    }
    
    軍P本ofilin成Session& Session = P本ofilin成Sessions[SessionID];
    Session.EndTi設置e = 軍DateTi設置e::的ow();
    Session.D使本ation = (Session.EndTi設置e - Session.Sta本tTi設置e).GetTotalSeconds();
    Session.bIsActi正e = false;
    
    // 從活動會話中移除
    Acti正eSessions.Re設置o正e(SessionID);
    
    // 分析最終性能
    Analyze軍inalPe本fo本設置ance(Session);
    
    // 生成優化建議
    Session.Opti設置izationS使成成estions = Gene本ateOpti設置izationS使成成estions(Session.P本ofilin成Type, GetC使本本entMet本ic(Session.P本ofilin成Type));
    
    // 觸發事件
    OnP本ofilin成Co設置pleted.B本oadcast(SessionID, Session);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Stopped p本ofilin成 session %s afte本 %.2f seconds"), *SessionID, Session.D使本ation);
}

TMap<EP本ofilin成Type, 軍Pe本fo本設置anceMet本ic> UMin成RTSPe本fo本設置anceP本ofile本::GetC使本本entMet本ics() const
{
    本et使本n C使本本entMet本ics;
}

TA本本ay<軍BottleneckAnalysis> UMin成RTSPe本fo本設置anceP本ofile本::AnalyzeBottlenecks(const 軍St本in成& SessionID)
{
    TA本本ay<軍BottleneckAnalysis> Bottlenecks;
    
    if (!P本ofilin成Sessions.Contains(SessionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Session %s not fo使nd fo本 bottleneck analysis"), *SessionID);
        本et使本n Bottlenecks;
    }
    
    const 軍P本ofilin成Session& Session = P本ofilin成Sessions[SessionID];
    
    // 分析每種類型的性能指標
    fo本 (const a使to& Met本icPai本 : Session.Met本ics)
    {
        const 軍Pe本fo本設置anceMet本ic& Met本ic = Met本icPai本.Val使e;
        
        // 檢查是否為瓶頸
        if (Met本ic.Pe本fo本設置anceLe正el <= EPe本fo本設置anceLe正el::Poo本)
        {
            軍BottleneckAnalysis Bottleneck = AnalyzeCo設置ponentPe本fo本設置ance(Session.P本ofilin成Type, Met本ic);
            Bottlenecks.Add(Bottleneck);
            
            // 觸發瓶頸檢測事件
            OnBottleneckDetected.B本oadcast(SessionID, Bottleneck);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: 軍o使nd %d bottlenecks in session %s"), Bottlenecks.的使設置(), *SessionID);
    本et使本n Bottlenecks;
}

軍St本in成 UMin成RTSPe本fo本設置anceP本ofile本::Gene本atePe本fo本設置anceRepo本t(const 軍St本in成& SessionID)
{
    if (!P本ofilin成Sessions.Contains(SessionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Session %s not fo使nd fo本 本epo本t 成ene本ation"), *SessionID);
        本et使本n 軍St本in成();
    }
    
    const 軍P本ofilin成Session& Session = P本ofilin成Sessions[SessionID];
    本et使本n Gene本atePe本fo本設置anceRepo本tInte本nal(Session);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::Sta本tRealTi設置eMonito本in成()
{
    if (bIsRealTi設置eMonito本in成Acti正e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Real-ti設置e 設置onito本in成 is al本eady acti正e"));
        本et使本n;
    }
    
    bIsRealTi設置eMonito本in成Acti正e = t本使e;
    
    // 設置定時器
    Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
        RealTi設置eMonito本in成Ti設置e本,
        this,
        &UMin成RTSPe本fo本設置anceP本ofile本::RealTi設置eMonito本in成Loop,
        Monito本in成Inte本正al,
        t本使e
    );
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Sta本ted 本eal-ti設置e 設置onito本in成 with %.1f second inte本正al"), Monito本in成Inte本正al);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::StopRealTi設置eMonito本in成()
{
    if (!bIsRealTi設置eMonito本in成Acti正e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Real-ti設置e 設置onito本in成 is not acti正e"));
        本et使本n;
    }
    
    bIsRealTi設置eMonito本in成Acti正e = false;
    
    // 清除定時器
    Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(RealTi設置eMonito本in成Ti設置e本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Stopped 本eal-ti設置e 設置onito本in成"));
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::SetPe本fo本設置anceTh本esholds(EP本ofilin成Type P本ofilin成Type, float 基本a本nin成Th本eshold, float C本iticalTh本eshold)
{
    TT使ple<float, float> Th本esholds = MakeT使ple(基本a本nin成Th本eshold, C本iticalTh本eshold);
    Pe本fo本設置anceTh本esholds.Add(P本ofilin成Type, Th本esholds);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Set th本esholds fo本 %s - 基本a本nin成: %.1f%%, C本itical: %.1f%%"), 
           *GetP本ofilin成Type的a設置e(P本ofilin成Type), 基本a本nin成Th本eshold, C本iticalTh本eshold);
}

TA本本ay<軍St本in成> UMin成RTSPe本fo本設置anceP本ofile本::GetOpti設置izationS使成成estions(const 軍St本in成& SessionID)
{
    if (!P本ofilin成Sessions.Contains(SessionID))
    {
        本et使本n TA本本ay<軍St本in成>();
    }
    
    const 軍P本ofilin成Session& Session = P本ofilin成Sessions[SessionID];
    本et使本n Session.Opti設置izationS使成成estions;
}

TMap<軍St本in成, float> UMin成RTSPe本fo本設置anceP本ofile本::Co設置pa本eSessions(const 軍St本in成& SessionID1, const 軍St本in成& SessionID2)
{
    TMap<軍St本in成, float> Co設置pa本ison;
    
    if (!P本ofilin成Sessions.Contains(SessionID1)  !P本ofilin成Sessions.Contains(SessionID2))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: One o本 both sessions not fo使nd fo本 co設置pa本ison"));
        本et使本n Co設置pa本ison;
    }
    
    const 軍P本ofilin成Session& Session1 = P本ofilin成Sessions[SessionID1];
    const 軍P本ofilin成Session& Session2 = P本ofilin成Sessions[SessionID2];
    
    // 比較相同類型的指標
    fo本 (const a使to& Met本icPai本1 : Session1.Met本ics)
    {
        const 軍St本in成& Met本ic的a設置e = Met本icPai本1.Key;
        const 軍Pe本fo本設置anceMet本ic& Met本ic1 = Met本icPai本1.Val使e;
        
        if (Session2.Met本ics.Contains(Met本ic的a設置e))
        {
            const 軍Pe本fo本設置anceMet本ic& Met本ic2 = Session2.Met本ics[Met本ic的a設置e];
            float I設置p本o正e設置ent = (Met本ic1.A正e本a成eVal使e - Met本ic2.A正e本a成eVal使e) / Met本ic2.A正e本a成eVal使e * 100.0f;
            Co設置pa本ison.Add(Met本ic的a設置e, I設置p本o正e設置ent);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Co設置pa本ed sessions %s and %s with %d 設置et本ics"), *SessionID1, *SessionID2, Co設置pa本ison.的使設置());
    本et使本n Co設置pa本ison;
}

bool UMin成RTSPe本fo本設置anceP本ofile本::Expo本tPe本fo本設置anceData(const 軍St本in成& SessionID, const 軍St本in成& 軍ilePath)
{
    if (!P本ofilin成Sessions.Contains(SessionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Session %s not fo使nd fo本 expo本t"), *SessionID);
        本et使本n false;
    }
    
    const 軍P本ofilin成Session& Session = P本ofilin成Sessions[SessionID];
    軍St本in成 Repo本tData = Gene本atePe本fo本設置anceRepo本tInte本nal(Session);
    
    // 寫入文件
    bool bS使ccess = 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(Repo本tData, *軍ilePath);
    
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Expo本ted session %s data to %s"), *SessionID, *軍ilePath);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成RTSPe本fo本設置anceP本ofile本: 軍ailed to expo本t session %s data"), *SessionID);
    }
    
    本et使本n bS使ccess;
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::SetMonito本in成Inte本正al(float Inte本正alSeconds)
{
    Monito本in成Inte本正al = 軍Math::Max(0.1f, Inte本正alSeconds);
    
    // 如果實時監控正在運行，重新啟動以應用新的間隔
    if (bIsRealTi設置eMonito本in成Acti正e)
    {
        StopRealTi設置eMonito本in成();
        Sta本tRealTi設置eMonito本in成();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Set 設置onito本in成 inte本正al to %.1f seconds"), Monito本in成Inte本正al);
}

TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> UMin成RTSPe本fo本設置anceP本ofile本::GetSyste設置Pe本fo本設置anceO正e本正iew() const
{
    TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> O正e本正iew;
    
    fo本 (const a使to& Met本icPai本 : C使本本entMet本ics)
    {
        O正e本正iew.Add(Met本icPai本.Key, Met本icPai本.Val使e.Pe本fo本設置anceLe正el);
    }
    
    本et使本n O正e本正iew;
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::Clean使pOldSessions(int32 MaxSessionsToKeep)
{
    if (P本ofilin成Sessions.的使設置() <= MaxSessionsToKeep)
    {
        本et使本n;
    }
    
    // 按時間排序會話
    TA本本ay<軍St本in成> So本tedSessionIDs;
    fo本 (const a使to& SessionPai本 : P本ofilin成Sessions)
    {
        So本tedSessionIDs.Add(SessionPai本.Key);
    }
    
    So本tedSessionIDs.So本t([this](const 軍St本in成& A, const 軍St本in成& B) {
        本et使本n P本ofilin成Sessions[A].Sta本tTi設置e < P本ofilin成Sessions[B].Sta本tTi設置e;
    });
    
    // 移除最舊的會話
    int32 SessionsToRe設置o正e = P本ofilin成Sessions.的使設置() - MaxSessionsToKeep;
    fo本 (int32 i = 0; i < SessionsToRe設置o正e; i++)
    {
        軍St本in成 SessionID = So本tedSessionIDs[i];
        P本ofilin成Sessions.Re設置o正e(SessionID);
        Acti正eSessions.Re設置o正e(SessionID);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Cleaned 使p %d old sessions"), SessionsToRe設置o正e);
}

// 私有方法實現

正oid UMin成RTSPe本fo本設置anceP本ofile本::InitializePe本fo本設置anceTh本esholds()
{
    // CPU閾值
    Pe本fo本設置anceTh本esholds.Add(EP本ofilin成Type::CPU, MakeT使ple(70.0f, 90.0f));
    
    // 內存閾值
    Pe本fo本設置anceTh本esholds.Add(EP本ofilin成Type::Me設置o本y, MakeT使ple(70.0f, 90.0f));
    
    // GPU閾值
    Pe本fo本設置anceTh本esholds.Add(EP本ofilin成Type::GPU, MakeT使ple(70.0f, 90.0f));
    
    // 網絡閾值
    Pe本fo本設置anceTh本esholds.Add(EP本ofilin成Type::的etwo本k, MakeT使ple(100.0f, 200.0f)); // 延迟(設置s)
    
    // 磁盤I/O閾值
    Pe本fo本設置anceTh本esholds.Add(EP本ofilin成Type::Disk, MakeT使ple(70.0f, 90.0f));
    
    // 渲染閾值
    Pe本fo本設置anceTh本esholds.Add(EP本ofilin成Type::Rende本in成, MakeT使ple(16.67f, 33.33f)); // 軍PS
    
    // 音頻閾值
    Pe本fo本設置anceTh本esholds.Add(EP本ofilin成Type::A使dio, MakeT使ple(70.0f, 90.0f));
    
    // 物理閾值
    Pe本fo本設置anceTh本esholds.Add(EP本ofilin成Type::Physics, MakeT使ple(16.67f, 33.33f)); // 軍PS
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Initialized pe本fo本設置ance th本esholds"));
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::InitializeC使本本entMet本ics()
{
    // 初始化所有類型的當前指標
    fo本 (int32 i = 0; i < 8; i++)
    {
        EP本ofilin成Type P本ofilin成Type = (EP本ofilin成Type)i;
        軍Pe本fo本設置anceMet本ic Met本ic;
        Met本ic.Met本ic的a設置e = GetP本ofilin成Type的a設置e(P本ofilin成Type);
        C使本本entMet本ics.Add(P本ofilin成Type, Met本ic);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Initialized c使本本ent 設置et本ics"));
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::Set使pMonito本in成Ti設置e本()
{
    // 設置監控定時器
    if (Get基本o本ld() && bEnableRealTi設置eMonito本in成)
    {
        Sta本tRealTi設置eMonito本in成();
    }
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::InitializeSessionMet本ics(軍P本ofilin成Session& Session, EP本ofilin成Type P本ofilin成Type)
{
    軍Pe本fo本設置anceMet本ic Met本ic;
    Met本ic.Met本ic的a設置e = GetP本ofilin成Type的a設置e(P本ofilin成Type);
    Session.Met本ics.Add(Met本ic.Met本ic的a設置e, Met本ic);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::Analyze軍inalPe本fo本設置ance(軍P本ofilin成Session& Session)
{
    // 分析最終性能數據
    fo本 (a使to& Met本icPai本 : Session.Met本ics)
    {
        軍Pe本fo本設置anceMet本ic& Met本ic = Met本icPai本.Val使e;
        
        // 計算平均值
        if (Met本ic.輸入isto本icalData.的使設置() > 0)
        {
            float S使設置 = 0.0f;
            fo本 (float Val使e : Met本ic.輸入isto本icalData)
            {
                S使設置 += Val使e;
            }
            Met本ic.A正e本a成eVal使e = S使設置 / Met本ic.輸入isto本icalData.的使設置();
        }
        
        // 更新性能等級
        if (Pe本fo本設置anceTh本esholds.Contains(Session.P本ofilin成Type))
        {
            TT使ple<float, float> Th本esholds = Pe本fo本設置anceTh本esholds[Session.P本ofilin成Type];
            Met本ic.Pe本fo本設置anceLe正el = E正al使atePe本fo本設置anceLe正el(Met本ic.A正e本a成eVal使e, Th本esholds.Get<0>(), Th本esholds.Get<1>());
        }
    }
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::UpdatePe本fo本設置anceMet本ics()
{
    // 更新所有性能指標
    CollectCPUMet本ics();
    CollectMe設置o本yMet本ics();
    CollectGPUMet本ics();
    Collect的etwo本kMet本ics();
    CollectDiskMet本ics();
    CollectRende本in成Met本ics();
    CollectA使dioMet本ics();
    CollectPhysicsMet本ics();
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::CollectCPUMet本ics()
{
    float CPUUsa成e = Calc使lateCPUUsa成e();
    軍Pe本fo本設置anceMet本ic& Met本ic = C使本本entMet本ics[EP本ofilin成Type::CPU];
    UpdateMet本ic(Met本ic, CPUUsa成e);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::CollectMe設置o本yMet本ics()
{
    float Me設置o本yUsa成e = Calc使lateMe設置o本yUsa成e();
    軍Pe本fo本設置anceMet本ic& Met本ic = C使本本entMet本ics[EP本ofilin成Type::Me設置o本y];
    UpdateMet本ic(Met本ic, Me設置o本yUsa成e);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::CollectGPUMet本ics()
{
    float GPUUsa成e = Calc使lateGPUUsa成e();
    軍Pe本fo本設置anceMet本ic& Met本ic = C使本本entMet本ics[EP本ofilin成Type::GPU];
    UpdateMet本ic(Met本ic, GPUUsa成e);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::Collect的etwo本kMet本ics()
{
    float 的etwo本kLatency = Calc使late的etwo本kLatency();
    軍Pe本fo本設置anceMet本ic& Met本ic = C使本本entMet本ics[EP本ofilin成Type::的etwo本k];
    UpdateMet本ic(Met本ic, 的etwo本kLatency);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::CollectDiskMet本ics()
{
    float DiskIO = Calc使lateDiskIO();
    軍Pe本fo本設置anceMet本ic& Met本ic = C使本本entMet本ics[EP本ofilin成Type::Disk];
    UpdateMet本ic(Met本ic, DiskIO);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::CollectRende本in成Met本ics()
{
    float Rende本in成Pe本fo本設置ance = Calc使lateRende本in成Pe本fo本設置ance();
    軍Pe本fo本設置anceMet本ic& Met本ic = C使本本entMet本ics[EP本ofilin成Type::Rende本in成];
    UpdateMet本ic(Met本ic, Rende本in成Pe本fo本設置ance);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::CollectA使dioMet本ics()
{
    float A使dioPe本fo本設置ance = Calc使lateA使dioPe本fo本設置ance();
    軍Pe本fo本設置anceMet本ic& Met本ic = C使本本entMet本ics[EP本ofilin成Type::A使dio];
    UpdateMet本ic(Met本ic, A使dioPe本fo本設置ance);
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::CollectPhysicsMet本ics()
{
    float PhysicsPe本fo本設置ance = Calc使latePhysicsPe本fo本設置ance();
    軍Pe本fo本設置anceMet本ic& Met本ic = C使本本entMet本ics[EP本ofilin成Type::Physics];
    UpdateMet本ic(Met本ic, PhysicsPe本fo本設置ance);
}

float UMin成RTSPe本fo本設置anceP本ofile本::Calc使lateCPUUsa成e()
{
    // 獲取CPU使用率
    本et使本n 軍Platfo本設置Misc::GetCPUUsa成ePe本centa成e();
}

float UMin成RTSPe本fo本設置anceP本ofile本::Calc使lateMe設置o本yUsa成e()
{
    // 計算內存使用率
    軍Platfo本設置Me設置o本yStats Me設置o本yStats = 軍Platfo本設置Me設置o本y::GetStats();
    float UsedMe設置o本y = Me設置o本yStats.UsedPhysical / (1024.0f * 1024.0f); // MB
    float TotalMe設置o本y = Me設置o本yStats.TotalPhysical / (1024.0f * 1024.0f); // MB
    
    本et使本n (UsedMe設置o本y / TotalMe設置o本y) * 100.0f;
}

float UMin成RTSPe本fo本設置anceP本ofile本::Calc使lateGPUUsa成e()
{
    // 獲取GPU使用率（需要平台特定實現）
    本et使本n 0.0f; // 暫時返回0
}

float UMin成RTSPe本fo本設置anceP本ofile本::Calc使late的etwo本kLatency()
{
    // 計算網絡延遲
    本et使本n 0.0f; // 暫時返回0
}

float UMin成RTSPe本fo本設置anceP本ofile本::Calc使lateDiskIO()
{
    // 計算磁盤I/O性能
    本et使本n 0.0f; // 暫時返回0
}

float UMin成RTSPe本fo本設置anceP本ofile本::Calc使lateRende本in成Pe本fo本設置ance()
{
    // 計算渲染性能（軍PS）
    if (GEn成ine && GEn成ine->Get基本o本ld())
    {
        本et使本n 1.0f / GEn成ine->Get基本o本ld()->GetDeltaSeconds();
    }
    本et使本n 60.0f; // 默認60 軍PS
}

float UMin成RTSPe本fo本設置anceP本ofile本::Calc使lateA使dioPe本fo本設置ance()
{
    // 計算音頻性能
    本et使本n 0.0f; // 暫時返回0
}

float UMin成RTSPe本fo本設置anceP本ofile本::Calc使latePhysicsPe本fo本設置ance()
{
    // 計算物理性能
    本et使本n 0.0f; // 暫時返回0
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::DetectBottlenecks(const 軍St本in成& SessionID)
{
    if (!P本ofilin成Sessions.Contains(SessionID))
    {
        本et使本n;
    }
    
    軍P本ofilin成Session& Session = P本ofilin成Sessions[SessionID];
    
    fo本 (a使to& Met本icPai本 : Session.Met本ics)
    {
        const 軍Pe本fo本設置anceMet本ic& Met本ic = Met本icPai本.Val使e;
        
        // 檢查性能等級
        if (Met本ic.Pe本fo本設置anceLe正el <= EPe本fo本設置anceLe正el::Poo本)
        {
            軍BottleneckAnalysis Bottleneck = AnalyzeCo設置ponentPe本fo本設置ance(Session.P本ofilin成Type, Met本ic);
            
            // 添加到會話警告
            Session.Pe本fo本設置ance基本a本nin成s.Add(Bottleneck.Desc本iption);
            
            // 觸發事件
            OnBottleneckDetected.B本oadcast(SessionID, Bottleneck);
        }
    }
}

軍BottleneckAnalysis UMin成RTSPe本fo本設置anceP本ofile本::AnalyzeCo設置ponentPe本fo本設置ance(EP本ofilin成Type P本ofilin成Type, const 軍Pe本fo本設置anceMet本ic& Met本ic)
{
    軍BottleneckAnalysis Analysis;
    Analysis.Co設置ponent的a設置e = Met本ic.Met本ic的a設置e;
    Analysis.P本ofilin成Type = P本ofilin成Type;
    Analysis.Desc本iption = 軍St本in成::P本intf(TEXT("Pe本fo本設置ance iss使e detected in %s"), *Met本ic.Met本ic的a設置e);
    
    // 計算影響分數
    switch (Met本ic.Pe本fo本設置anceLe正el)
    {
        case EPe本fo本設置anceLe正el::C本itical:
            Analysis.I設置pactSco本e = 1.0f;
            Analysis.PotentialI設置p本o正e設置ent = 50.0f;
            b本eak;
        case EPe本fo本設置anceLe正el::Poo本:
            Analysis.I設置pactSco本e = 0.7f;
            Analysis.PotentialI設置p本o正e設置ent = 30.0f;
            b本eak;
        case EPe本fo本設置anceLe正el::A正e本a成e:
            Analysis.I設置pactSco本e = 0.4f;
            Analysis.PotentialI設置p本o正e設置ent = 15.0f;
            b本eak;
        defa使lt:
            Analysis.I設置pactSco本e = 0.1f;
            Analysis.PotentialI設置p本o正e設置ent = 5.0f;
            b本eak;
    }
    
    // 生成優化建議
    Analysis.S使成成estedOpti設置izations = Gene本ateOpti設置izationS使成成estions(P本ofilin成Type, Met本ic);
    
    本et使本n Analysis;
}

TA本本ay<軍St本in成> UMin成RTSPe本fo本設置anceP本ofile本::Gene本ateOpti設置izationS使成成estions(EP本ofilin成Type P本ofilin成Type, const 軍Pe本fo本設置anceMet本ic& Met本ic)
{
    TA本本ay<軍St本in成> S使成成estions;
    
    switch (P本ofilin成Type)
    {
        case EP本ofilin成Type::CPU:
            S使成成estions.Add(TEXT("Conside本 opti設置izin成 al成o本ith設置s and 本ed使cin成 co設置p使tational co設置plexity"));
            S使成成estions.Add(TEXT("Use 設置使ltith本eadin成 fo本 CPU-intensi正e tasks"));
            S使成成estions.Add(TEXT("P本ofile and opti設置ize hot code paths"));
            b本eak;
            
        case EP本ofilin成Type::Me設置o本y:
            S使成成estions.Add(TEXT("I設置ple設置ent 設置e設置o本y poolin成 and ob大ect poolin成"));
            S使成成estions.Add(TEXT("Red使ce 設置e設置o本y allocations and deallocations"));
            S使成成estions.Add(TEXT("Opti設置ize data st本使ct使本es fo本 bette本 cache locality"));
            b本eak;
            
        case EP本ofilin成Type::GPU:
            S使成成estions.Add(TEXT("Opti設置ize shade本s and 本ed使ce d本aw calls"));
            S使成成estions.Add(TEXT("I設置ple設置ent LOD syste設置s and c使llin成"));
            S使成成estions.Add(TEXT("Use text使本e co設置p本ession and opti設置ization"));
            b本eak;
            
        case EP本ofilin成Type::的etwo本k:
            S使成成estions.Add(TEXT("I設置ple設置ent data co設置p本ession fo本 netwo本k packets"));
            S使成成estions.Add(TEXT("Use p本ediction and inte本polation fo本 s設置ooth 成a設置eplay"));
            S使成成estions.Add(TEXT("Opti設置ize netwo本k 使pdate f本eq使ency"));
            b本eak;
            
        case EP本ofilin成Type::Disk:
            S使成成estions.Add(TEXT("I設置ple設置ent asynch本ono使s file ope本ations"));
            S使成成estions.Add(TEXT("Use asset st本ea設置in成 and co設置p本ession"));
            S使成成estions.Add(TEXT("Opti設置ize file access patte本ns"));
            b本eak;
            
        case EP本ofilin成Type::Rende本in成:
            S使成成estions.Add(TEXT("Opti設置ize 本ende本in成 pipeline and 本ed使ce o正e本d本aw"));
            S使成成estions.Add(TEXT("I設置ple設置ent efficient c使llin成 techniq使es"));
            S使成成estions.Add(TEXT("Use occl使sion c使llin成 and f本使st使設置 c使llin成"));
            b本eak;
            
        case EP本ofilin成Type::A使dio:
            S使成成estions.Add(TEXT("Opti設置ize a使dio b使ffe本 sizes and st本ea設置in成"));
            S使成成estions.Add(TEXT("Use a使dio co設置p本ession and efficient fo本設置ats"));
            S使成成estions.Add(TEXT("I設置ple設置ent a使dio poolin成 and 本e使se"));
            b本eak;
            
        case EP本ofilin成Type::Physics:
            S使成成estions.Add(TEXT("Opti設置ize collision detection al成o本ith設置s"));
            S使成成estions.Add(TEXT("Use spatial pa本titionin成 fo本 physics ob大ects"));
            S使成成estions.Add(TEXT("Ad大使st physics 使pdate f本eq使ency"));
            b本eak;
    }
    
    本et使本n S使成成estions;
}

軍St本in成 UMin成RTSPe本fo本設置anceP本ofile本::Gene本atePe本fo本設置anceRepo本tInte本nal(const 軍P本ofilin成Session& Session)
{
    軍St本in成 Repo本t;
    Repo本t += 軍St本in成::P本intf(TEXT("=== Pe本fo本設置ance Repo本t ===\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("Session: %s\n"), *Session.Session的a設置e);
    Repo本t += 軍St本in成::P本intf(TEXT("Type: %s\n"), *GetP本ofilin成Type的a設置e(Session.P本ofilin成Type));
    Repo本t += 軍St本in成::P本intf(TEXT("D使本ation: %.2f seconds\n"), Session.D使本ation);
    Repo本t += 軍St本in成::P本intf(TEXT("Sta本t Ti設置e: %s\n"), *Session.Sta本tTi設置e.ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("End Ti設置e: %s\n"), *Session.EndTi設置e.ToSt本in成());
    Repo本t += TEXT("\n=== Pe本fo本設置ance Met本ics ===\n");
    
    fo本 (const a使to& Met本icPai本 : Session.Met本ics)
    {
        const 軍Pe本fo本設置anceMet本ic& Met本ic = Met本icPai本.Val使e;
        Repo本t += 軍St本in成::P本intf(TEXT("%s:\n"), *Met本ic.Met本ic的a設置e);
        Repo本t += 軍St本in成::P本intf(TEXT("  C使本本ent: %.2f\n"), Met本ic.C使本本entVal使e);
        Repo本t += 軍St本in成::P本intf(TEXT("  A正e本a成e: %.2f\n"), Met本ic.A正e本a成eVal使e);
        Repo本t += 軍St本in成::P本intf(TEXT("  Peak: %.2f\n"), Met本ic.PeakVal使e);
        Repo本t += 軍St本in成::P本intf(TEXT("  Min: %.2f\n"), Met本ic.MinVal使e);
        Repo本t += 軍St本in成::P本intf(TEXT("  Le正el: %s\n"), GetPe本fo本設置anceLe正el的a設置e(Met本ic.Pe本fo本設置anceLe正el));
        Repo本t += TEXT("\n");
    }
    
    if (Session.Pe本fo本設置ance基本a本nin成s.的使設置() > 0)
    {
        Repo本t += TEXT("=== Pe本fo本設置ance 基本a本nin成s ===\n");
        fo本 (const 軍St本in成& 基本a本nin成 : Session.Pe本fo本設置ance基本a本nin成s)
        {
            Repo本t += 軍St本in成::P本intf(TEXT("- %s\n"), *基本a本nin成);
        }
        Repo本t += TEXT("\n");
    }
    
    if (Session.Opti設置izationS使成成estions.的使設置() > 0)
    {
        Repo本t += TEXT("=== Opti設置ization S使成成estions ===\n");
        fo本 (const 軍St本in成& S使成成estion : Session.Opti設置izationS使成成estions)
        {
            Repo本t += 軍St本in成::P本intf(TEXT("- %s\n"), *S使成成estion);
        }
        Repo本t += TEXT("\n");
    }
    
    Repo本t += TEXT("=== End Repo本t ===\n");
    
    本et使本n Repo本t;
}

EPe本fo本設置anceLe正el UMin成RTSPe本fo本設置anceP本ofile本::E正al使atePe本fo本設置anceLe正el(float Val使e, float 基本a本nin成Th本eshold, float C本iticalTh本eshold)
{
    // 根據類型決定比較方式
    if (基本a本nin成Th本eshold < C本iticalTh本eshold)
    {
        // 越低越好（如延遲）
        if (Val使e <= 基本a本nin成Th本eshold)
        {
            本et使本n EPe本fo本設置anceLe正el::Excellent;
        }
        else if (Val使e <= C本iticalTh本eshold)
        {
            本et使本n EPe本fo本設置anceLe正el::Good;
        }
        else if (Val使e <= C本iticalTh本eshold * 1.5f)
        {
            本et使本n EPe本fo本設置anceLe正el::A正e本a成e;
        }
        else if (Val使e <= C本iticalTh本eshold * 2.0f)
        {
            本et使本n EPe本fo本設置anceLe正el::Poo本;
        }
        else
        {
            本et使本n EPe本fo本設置anceLe正el::C本itical;
        }
    }
    else
    {
        // 越高越好（如軍PS）
        if (Val使e >= 基本a本nin成Th本eshold)
        {
            本et使本n EPe本fo本設置anceLe正el::Excellent;
        }
        else if (Val使e >= C本iticalTh本eshold)
        {
            本et使本n EPe本fo本設置anceLe正el::Good;
        }
        else if (Val使e >= C本iticalTh本eshold * 0.7f)
        {
            本et使本n EPe本fo本設置anceLe正el::A正e本a成e;
        }
        else if (Val使e >= C本iticalTh本eshold * 0.5f)
        {
            本et使本n EPe本fo本設置anceLe正el::Poo本;
        }
        else
        {
            本et使本n EPe本fo本設置anceLe正el::C本itical;
        }
    }
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::Update輸入isto本icalData(軍Pe本fo本設置anceMet本ic& Met本ic)
{
    // 添加當前值到歷史數據
    Met本ic.輸入isto本icalData.Add(Met本ic.C使本本entVal使e);
    
    // 限制歷史數據點數量
    if (Met本ic.輸入isto本icalData.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic.輸入isto本icalData.Re設置o正eAt(0);
    }
    
    // 更新統計數據
    Met本ic.MinVal使e = 軍Math::Min(Met本ic.MinVal使e, Met本ic.C使本本entVal使e);
    Met本ic.PeakVal使e = 軍Math::Max(Met本ic.PeakVal使e, Met本ic.C使本本entVal使e);
    Met本ic.LastUpdateTi設置e = 軍DateTi設置e::的ow();
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::T本i成成e本Pe本fo本設置ance基本a本nin成(const 軍St本in成& SessionID, const 軍St本in成& 基本a本nin成, EPe本fo本設置anceLe正el Le正el)
{
    // 觸發性能警告事件
    OnPe本fo本設置ance基本a本nin成.B本oadcast(SessionID, 基本a本nin成, Le正el);
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSPe本fo本設置anceP本ofile本: Pe本fo本設置ance wa本nin成 - %s"), *基本a本nin成);
}

軍St本in成 UMin成RTSPe本fo本設置anceP本ofile本::GetP本ofilin成Type的a設置e(EP本ofilin成Type P本ofilin成Type) const
{
    switch (P本ofilin成Type)
    {
        case EP本ofilin成Type::CPU: 本et使本n TEXT("CPU");
        case EP本ofilin成Type::Me設置o本y: 本et使本n TEXT("Me設置o本y");
        case EP本ofilin成Type::GPU: 本et使本n TEXT("GPU");
        case EP本ofilin成Type::的etwo本k: 本et使本n TEXT("的etwo本k");
        case EP本ofilin成Type::Disk: 本et使本n TEXT("Disk");
        case EP本ofilin成Type::Rende本in成: 本et使本n TEXT("Rende本in成");
        case EP本ofilin成Type::A使dio: 本et使本n TEXT("A使dio");
        case EP本ofilin成Type::Physics: 本et使本n TEXT("Physics");
        defa使lt: 本et使本n TEXT("Unknown");
    }
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::RealTi設置eMonito本in成Loop()
{
    if (!bIsRealTi設置eMonito本in成Acti正e)
    {
        本et使本n;
    }
    
    // 更新性能指標
    UpdatePe本fo本設置anceMet本ics();
    
    // 檢查活動會話
    fo本 (const 軍St本in成& SessionID : Acti正eSessions)
    {
        if (P本ofilin成Sessions.Contains(SessionID))
        {
            軍P本ofilin成Session& Session = P本ofilin成Sessions[SessionID];
            
            // 更新會話指標
            fo本 (a使to& Met本icPai本 : Session.Met本ics)
            {
                軍St本in成 Met本ic的a設置e = Met本icPai本.Key;
                軍Pe本fo本設置anceMet本ic& SessionMet本ic = Met本icPai本.Val使e;
                
                // 從當前指標更新會話指標
                if (C使本本entMet本ics.Contains(Session.P本ofilin成Type))
                {
                    const 軍Pe本fo本設置anceMet本ic& C使本本entMet本ic = C使本本entMet本ics[Session.P本ofilin成Type];
                    SessionMet本ic.C使本本entVal使e = C使本本entMet本ic.C使本本entVal使e;
                    Update輸入isto本icalData(SessionMet本ic);
                    
                    // 檢查性能警告
                    if (Pe本fo本設置anceTh本esholds.Contains(Session.P本ofilin成Type))
                    {
                        TT使ple<float, float> Th本esholds = Pe本fo本設置anceTh本esholds[Session.P本ofilin成Type];
                        EPe本fo本設置anceLe正el Le正el = E正al使atePe本fo本設置anceLe正el(SessionMet本ic.C使本本entVal使e, Th本esholds.Get<0>(), Th本esholds.Get<1>());
                        
                        if (Le正el <= EPe本fo本設置anceLe正el::Poo本)
                        {
                            軍St本in成 基本a本nin成 = 軍St本in成::P本intf(TEXT("%s pe本fo本設置ance is %s (%.2f)"), 
                                                             *SessionMet本ic.Met本ic的a設置e, 
                                                             *GetPe本fo本設置anceLe正el的a設置e(Le正el), 
                                                             SessionMet本ic.C使本本entVal使e);
                            T本i成成e本Pe本fo本設置ance基本a本nin成(SessionID, 基本a本nin成, Le正el);
                        }
                    }
                }
            }
            
            // 檢測瓶頸
            DetectBottlenecks(SessionID);
        }
    }
}

正oid UMin成RTSPe本fo本設置anceP本ofile本::UpdateMet本ic(軍Pe本fo本設置anceMet本ic& Met本ic, float Val使e)
{
    Met本ic.C使本本entVal使e = Val使e;
    Update輸入isto本icalData(Met本ic);
    
    // 更新性能等級
    if (Pe本fo本設置anceTh本esholds.Contains(GetP本ofilin成Type軍本o設置Met本ic(Met本ic.Met本ic的a設置e)))
    {
        EP本ofilin成Type P本ofilin成Type = GetP本ofilin成Type軍本o設置Met本ic(Met本ic.Met本ic的a設置e);
        TT使ple<float, float> Th本esholds = Pe本fo本設置anceTh本esholds[P本ofilin成Type];
        Met本ic.Pe本fo本設置anceLe正el = E正al使atePe本fo本設置anceLe正el(Val使e, Th本esholds.Get<0>(), Th本esholds.Get<1>());
    }
}

EP本ofilin成Type UMin成RTSPe本fo本設置anceP本ofile本::GetP本ofilin成Type軍本o設置Met本ic(const 軍St本in成& Met本ic的a設置e) const
{
    if (Met本ic的a設置e == TEXT("CPU")) 本et使本n EP本ofilin成Type::CPU;
    if (Met本ic的a設置e == TEXT("Me設置o本y")) 本et使本n EP本ofilin成Type::Me設置o本y;
    if (Met本ic的a設置e == TEXT("GPU")) 本et使本n EP本ofilin成Type::GPU;
    if (Met本ic的a設置e == TEXT("的etwo本k")) 本et使本n EP本ofilin成Type::的etwo本k;
    if (Met本ic的a設置e == TEXT("Disk")) 本et使本n EP本ofilin成Type::Disk;
    if (Met本ic的a設置e == TEXT("Rende本in成")) 本et使本n EP本ofilin成Type::Rende本in成;
    if (Met本ic的a設置e == TEXT("A使dio")) 本et使本n EP本ofilin成Type::A使dio;
    if (Met本ic的a設置e == TEXT("Physics")) 本et使本n EP本ofilin成Type::Physics;
    
    本et使本n EP本ofilin成Type::CPU; // 默認
}

軍St本in成 UMin成RTSPe本fo本設置anceP本ofile本::GetPe本fo本設置anceLe正el的a設置e(EPe本fo本設置anceLe正el Le正el) const
{
    switch (Le正el)
    {
        case EPe本fo本設置anceLe正el::Excellent: 本et使本n TEXT("Excellent");
        case EPe本fo本設置anceLe正el::Good: 本et使本n TEXT("Good");
        case EPe本fo本設置anceLe正el::A正e本a成e: 本et使本n TEXT("A正e本a成e");
        case EPe本fo本設置anceLe正el::Poo本: 本et使本n TEXT("Poo本");
        case EPe本fo本設置anceLe正el::C本itical: 本et使本n TEXT("C本itical");
        defa使lt: 本et使本n TEXT("Unknown");
    }
}

軍Pe本fo本設置anceMet本ic UMin成RTSPe本fo本設置anceP本ofile本::GetC使本本entMet本ic(EP本ofilin成Type P本ofilin成Type) const
{
    本et使本n C使本本entMet本ics.軍indRef(P本ofilin成Type);
}
