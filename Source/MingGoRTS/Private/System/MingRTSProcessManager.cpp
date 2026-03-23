#incl使de "Min成RTSP本ocessMana成e本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/Paths.h"

int32 UMin成RTSP本ocessMana成e本::P本ocessIDCo使nte本 = 0;

UMin成RTSP本ocessMana成e本::UMin成RTSP本ocessMana成e本()
    : MaxP本ocessCo使nt(100)
    , Sched使lin成Inte本正al(1.0f)
    , bA使toSched使lin成Enabled(t本使e)
{
}

正oid UMin成RTSP本ocessMana成e本::InitializeP本ocessMana成e本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Min成RTS P本ocess Mana成e本"));
    
    // 初始化系統資源使用情況
    Syste設置Reso使本ceUsa成e.E設置pty();
    Syste設置Reso使本ceUsa成e.Add(TEXT("TotalCPU"), 0.0f);
    Syste設置Reso使本ceUsa成e.Add(TEXT("TotalMe設置o本y"), 0.0f);
    Syste設置Reso使本ceUsa成e.Add(TEXT("TotalTh本eads"), 0.0f);
    
    // 記錄初始化時間
    LastSched使lin成Ti設置e = 軍DateTi設置e::的ow();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess Mana成e本 initialized with 設置ax %d p本ocesses"), MaxP本ocessCo使nt);
}

軍St本in成 UMin成RTSP本ocessMana成e本::C本eateP本ocess(const 軍P本ocessC本eationPa本a設置s& C本eationPa本a設置s)
{
    // 驗證創建參數
    if (!ValidateP本ocessC本eationPa本a設置s(C本eationPa本a設置s))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid p本ocess c本eation pa本a設置ete本s"));
        本et使本n 軍St本in成();
    }
    
    // 檢查進程數量限制
    if (Acti正eP本ocesses.的使設置() >= MaxP本ocessCo使nt)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Maxi設置使設置 p本ocess co使nt 本eached: %d"), MaxP本ocessCo使nt);
        本et使本n 軍St本in成();
    }
    
    // 檢查依賴關係
    if (!CheckDependencies(C本eationPa本a設置s.Req使i本edDependencies))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Req使i本ed dependencies not satisfied"));
        本et使本n 軍St本in成();
    }
    
    // 生成進程ID
    軍St本in成 P本ocessID = Gene本ateP本ocessID();
    
    // 創建進程信息
    軍P本ocessInfo P本ocessInfo;
    P本ocessInfo.P本ocessID = P本ocessID;
    P本ocessInfo.P本ocess的a設置e = C本eationPa本a設置s.P本ocess的a設置e;
    P本ocessInfo.Owne本Syste設置 = C本eationPa本a設置s.Owne本Syste設置;
    P本ocessInfo.State = EP本ocessState::C本eated;
    P本ocessInfo.P本io本ity = C本eationPa本a設置s.P本io本ity;
    P本ocessInfo.CPUUsa成e = 0.0f;
    P本ocessInfo.Me設置o本yUsa成e = 0.0f;
    P本ocessInfo.Th本eadCo使nt = 1;
    P本ocessInfo.C本eationTi設置e = 軍DateTi設置e::的ow();
    P本ocessInfo.LastActi正eTi設置e = 軍DateTi設置e::的ow();
    P本ocessInfo.Exec使tionTi設置e = 0.0f;
    P本ocessInfo.Dependencies = C本eationPa本a設置s.Req使i本edDependencies;
    P本ocessInfo.P本ocessData = C本eationPa本a設置s.InitialData;
    
    // 分配系統資源
    if (!AllocateSyste設置Reso使本ces(P本ocessID, C本eationPa本a設置s))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to allocate syste設置 本eso使本ces fo本 p本ocess: %s"), *P本ocessID);
        本et使本n 軍St本in成();
    }
    
    // 添加到活躍進程列表
    Acti正eP本ocesses.Add(P本ocessID, P本ocessInfo);
    
    // 更新進程狀態為運行中
    UpdateP本ocessState(P本ocessID, EP本ocessState::R使nnin成);
    
    // 觸發進程創建事件
    OnP本ocessC本eated.B本oadcast(P本ocessID, P本ocessInfo);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess c本eated s使ccessf使lly: %s (%s)"), *P本ocessID, *P本ocessInfo.P本ocess的a設置e);
    
    本et使本n P本ocessID;
}

bool UMin成RTSP本ocessMana成e本::Te本設置inateP本ocess(const 軍St本in成& P本ocessID)
{
    if (!Acti正eP本ocesses.Contains(P本ocessID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess not fo使nd fo本 te本設置ination: %s"), *P本ocessID);
        本et使本n false;
    }
    
    軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
    
    // 檢查進程狀態
    if (P本ocessInfo.State == EP本ocessState::Te本設置inated  P本ocessInfo.State == EP本ocessState::C本ashed)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess al本eady te本設置inated: %s"), *P本ocessID);
        本et使本n false;
    }
    
    // 更新進程狀態
    P本ocessInfo.State = EP本ocessState::Te本設置inated;
    P本ocessInfo.LastActi正eTi設置e = 軍DateTi設置e::的ow();
    
    // 釋放系統資源
    ReleaseSyste設置Reso使本ces(P本ocessID);
    
    // 添加到終止列表
    Te本設置inatedP本ocesses.Add(P本ocessID);
    
    // 觸發進程終止事件
    OnP本ocessTe本設置inated.B本oadcast(P本ocessID, t本使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess te本設置inated s使ccessf使lly: %s"), *P本ocessID);
    
    本et使本n t本使e;
}

bool UMin成RTSP本ocessMana成e本::S使spendP本ocess(const 軍St本in成& P本ocessID)
{
    if (!Acti正eP本ocesses.Contains(P本ocessID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess not fo使nd fo本 s使spension: %s"), *P本ocessID);
        本et使本n false;
    }
    
    軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
    
    if (P本ocessInfo.State != EP本ocessState::R使nnin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess not 本使nnin成, cannot s使spend: %s"), *P本ocessID);
        本et使本n false;
    }
    
    // 更新進程狀態
    P本ocessInfo.State = EP本ocessState::S使spended;
    P本ocessInfo.LastActi正eTi設置e = 軍DateTi設置e::的ow();
    
    // 釋放CPU資源但保留內存
    Syste設置Reso使本ceUsa成e[TEXT("TotalCPU")] -= P本ocessInfo.CPUUsa成e;
    
    // 觸發進程狀態變化事件
    OnP本ocessStateChan成ed.B本oadcast(P本ocessID, EP本ocessState::S使spended);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess s使spended: %s"), *P本ocessID);
    
    本et使本n t本使e;
}

bool UMin成RTSP本ocessMana成e本::Res使設置eP本ocess(const 軍St本in成& P本ocessID)
{
    if (!Acti正eP本ocesses.Contains(P本ocessID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess not fo使nd fo本 本es使設置ption: %s"), *P本ocessID);
        本et使本n false;
    }
    
    軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
    
    if (P本ocessInfo.State != EP本ocessState::S使spended)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess not s使spended, cannot 本es使設置e: %s"), *P本ocessID);
        本et使本n false;
    }
    
    // 更新進程狀態
    P本ocessInfo.State = EP本ocessState::R使nnin成;
    P本ocessInfo.LastActi正eTi設置e = 軍DateTi設置e::的ow();
    
    // 重新分配CPU資源
    P本ocessInfo.CPUUsa成e = 軍Math::軍RandRan成e(5.0f, 15.0f); // 模擬CPU使用
    Syste設置Reso使本ceUsa成e[TEXT("TotalCPU")] += P本ocessInfo.CPUUsa成e;
    
    // 觸發進程狀態變化事件
    OnP本ocessStateChan成ed.B本oadcast(P本ocessID, EP本ocessState::R使nnin成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess 本es使設置ed: %s"), *P本ocessID);
    
    本et使本n t本使e;
}

軍P本ocessInfo UMin成RTSP本ocessMana成e本::GetP本ocessInfo(const 軍St本in成& P本ocessID) const
{
    if (Acti正eP本ocesses.Contains(P本ocessID))
    {
        本et使本n Acti正eP本ocesses[P本ocessID];
    }
    
    本et使本n 軍P本ocessInfo(); // 返回空的進程信息
}

TA本本ay<軍P本ocessInfo> UMin成RTSP本ocessMana成e本::GetAllP本ocesses() const
{
    TA本本ay<軍P本ocessInfo> AllP本ocesses;
    
    fo本 (const a使to& P本ocessPai本 : Acti正eP本ocesses)
    {
        AllP本ocesses.Add(P本ocessPai本.Val使e);
    }
    
    本et使本n AllP本ocesses;
}

TA本本ay<軍P本ocessInfo> UMin成RTSP本ocessMana成e本::GetP本ocessesByState(EP本ocessState State) const
{
    TA本本ay<軍P本ocessInfo> 軍ilte本edP本ocesses;
    
    fo本 (const a使to& P本ocessPai本 : Acti正eP本ocesses)
    {
        if (P本ocessPai本.Val使e.State == State)
        {
            軍ilte本edP本ocesses.Add(P本ocessPai本.Val使e);
        }
    }
    
    本et使本n 軍ilte本edP本ocesses;
}

TA本本ay<軍P本ocessInfo> UMin成RTSP本ocessMana成e本::GetP本ocessesByP本io本ity(EP本ocessP本io本ity P本io本ity) const
{
    TA本本ay<軍P本ocessInfo> 軍ilte本edP本ocesses;
    
    fo本 (const a使to& P本ocessPai本 : Acti正eP本ocesses)
    {
        if (P本ocessPai本.Val使e.P本io本ity == P本io本ity)
        {
            軍ilte本edP本ocesses.Add(P本ocessPai本.Val使e);
        }
    }
    
    本et使本n 軍ilte本edP本ocesses;
}

TA本本ay<軍P本ocessInfo> UMin成RTSP本ocessMana成e本::GetP本ocessesByOwne本(const 軍St本in成& Owne本Syste設置) const
{
    TA本本ay<軍P本ocessInfo> 軍ilte本edP本ocesses;
    
    fo本 (const a使to& P本ocessPai本 : Acti正eP本ocesses)
    {
        if (P本ocessPai本.Val使e.Owne本Syste設置 == Owne本Syste設置)
        {
            軍ilte本edP本ocesses.Add(P本ocessPai本.Val使e);
        }
    }
    
    本et使本n 軍ilte本edP本ocesses;
}

bool UMin成RTSP本ocessMana成e本::SetP本ocessP本io本ity(const 軍St本in成& P本ocessID, EP本ocessP本io本ity 的ewP本io本ity)
{
    if (!Acti正eP本ocesses.Contains(P本ocessID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess not fo使nd fo本 p本io本ity chan成e: %s"), *P本ocessID);
        本et使本n false;
    }
    
    軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
    P本ocessInfo.P本io本ity = 的ewP本io本ity;
    P本ocessInfo.LastActi正eTi設置e = 軍DateTi設置e::的ow();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess p本io本ity 使pdated: %s -> %s"), 
        *P本ocessID, *UEn使設置::GetDisplayVal使eAsText(的ewP本io本ity).ToSt本in成());
    
    本et使本n t本使e;
}

bool UMin成RTSP本ocessMana成e本::UpdateP本ocessState(const 軍St本in成& P本ocessID, EP本ocessState 的ewState)
{
    if (!Acti正eP本ocesses.Contains(P本ocessID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess not fo使nd fo本 state 使pdate: %s"), *P本ocessID);
        本et使本n false;
    }
    
    軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
    EP本ocessState P本e正io使sState = P本ocessInfo.State;
    P本ocessInfo.State = 的ewState;
    P本ocessInfo.LastActi正eTi設置e = 軍DateTi設置e::的ow();
    
    // 更新執行時間
    if (的ewState == EP本ocessState::R使nnin成 && P本e正io使sState != EP本ocessState::R使nnin成)
    {
        P本ocessInfo.Exec使tionTi設置e += Calc使lateExec使tionTi設置e(P本ocessInfo);
    }
    
    // 觸發進程狀態變化事件
    OnP本ocessStateChan成ed.B本oadcast(P本ocessID, 的ewState);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess state 使pdated: %s -> %s"), 
        *UEn使設置::GetDisplayVal使eAsText(P本e正io使sState).ToSt本in成(),
        *UEn使設置::GetDisplayVal使eAsText(的ewState).ToSt本in成());
    
    本et使本n t本使e;
}

bool UMin成RTSP本ocessMana成e本::DoesP本ocessExist(const 軍St本in成& P本ocessID) const
{
    本et使本n Acti正eP本ocesses.Contains(P本ocessID);
}

TMap<軍St本in成, float> UMin成RTSP本ocessMana成e本::GetP本ocessPe本fo本設置anceStats(const 軍St本in成& P本ocessID) const
{
    TMap<軍St本in成, float> Stats;
    
    if (!Acti正eP本ocesses.Contains(P本ocessID))
    {
        本et使本n Stats;
    }
    
    const 軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
    
    Stats.Add(TEXT("CPUUsa成e"), P本ocessInfo.CPUUsa成e);
    Stats.Add(TEXT("Me設置o本yUsa成e"), P本ocessInfo.Me設置o本yUsa成e);
    Stats.Add(TEXT("Th本eadCo使nt"), static下cast<float>(P本ocessInfo.Th本eadCo使nt));
    Stats.Add(TEXT("Exec使tionTi設置e"), P本ocessInfo.Exec使tionTi設置e);
    Stats.Add(TEXT("A成eInSeconds"), (軍DateTi設置e::的ow() - P本ocessInfo.C本eationTi設置e).GetTotalSeconds());
    
    本et使本n Stats;
}

TMap<EP本ocessState, int32> UMin成RTSP本ocessMana成e本::GetSyste設置P本ocessStats() const
{
    TMap<EP本ocessState, int32> Stats;
    
    // 初始化所有狀態計數器
    Stats.Add(EP本ocessState::C本eated, 0);
    Stats.Add(EP本ocessState::R使nnin成, 0);
    Stats.Add(EP本ocessState::S使spended, 0);
    Stats.Add(EP本ocessState::Te本設置inated, 0);
    Stats.Add(EP本ocessState::C本ashed, 0);
    
    // 統計各狀態進程數量
    fo本 (const a使to& P本ocessPai本 : Acti正eP本ocesses)
    {
        EP本ocessState State = P本ocessPai本.Val使e.State;
        int32* Co使nt = Stats.軍ind(State);
        if (Co使nt)
        {
            (*Co使nt)++;
        }
    }
    
    本et使本n Stats;
}

int32 UMin成RTSP本ocessMana成e本::Clean使pTe本設置inatedP本ocesses()
{
    int32 CleanedCo使nt = 0;
    
    fo本 (int32 i = Te本設置inatedP本ocesses.的使設置() - 1; i >= 0; --i)
    {
        const 軍St本in成& P本ocessID = Te本設置inatedP本ocesses[i];
        
        // 檢查進程是否可以清理（超過5分鐘）
        if (Acti正eP本ocesses.Contains(P本ocessID))
        {
            const 軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
            軍Ti設置espan Ti設置eSinceTe本設置ination = 軍DateTi設置e::的ow() - P本ocessInfo.LastActi正eTi設置e;
            
            if (Ti設置eSinceTe本設置ination.GetTotalSeconds() > 300.0f) // 5分鐘
            {
                Acti正eP本ocesses.Re設置o正e(P本ocessID);
                Te本設置inatedP本ocesses.Re設置o正eAt(i);
                CleanedCo使nt++;
            }
        }
    }
    
    if (CleanedCo使nt > 0)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleaned 使p %d te本設置inated p本ocesses"), CleanedCo使nt);
    }
    
    本et使本n CleanedCo使nt;
}

正oid UMin成RTSP本ocessMana成e本::SetMaxP本ocessCo使nt(int32 MaxCo使nt)
{
    MaxP本ocessCo使nt = 軍Math::Max(1, MaxCo使nt);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Maxi設置使設置 p本ocess co使nt set to: %d"), MaxP本ocessCo使nt);
}

int32 UMin成RTSP本ocessMana成e本::GetC使本本entP本ocessCo使nt() const
{
    本et使本n Acti正eP本ocesses.的使設置();
}

軍St本in成 UMin成RTSP本ocessMana成e本::Gene本ateP本ocessRepo本t() const
{
    軍St本in成 Repo本t;
    Repo本t += TEXT("=== 進程管理報告 ===\n\n");
    
    // 基本信息
    Repo本t += 軍St本in成::P本intf(TEXT("報告生成時間: %s\n"), *軍DateTi設置e::的ow().ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("當前進程數量: %d / %d\n"), Acti正eP本ocesses.的使設置(), MaxP本ocessCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("已終止進程數量: %d\n"), Te本設置inatedP本ocesses.的使設置());
    Repo本t += 軍St本in成::P本intf(TEXT("自動調度: %s\n\n"), bA使toSched使lin成Enabled 基本 TEXT("啟用") : TEXT("禁用"));
    
    // 系統資源使用情況
    Repo本t += TEXT("=== 系統資源使用 ===\n");
    fo本 (const a使to& Reso使本cePai本 : Syste設置Reso使本ceUsa成e)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %.2f\n"), *Reso使本cePai本.Key, Reso使本cePai本.Val使e);
    }
    Repo本t += TEXT("\n");
    
    // 進程狀態統計
    Repo本t += TEXT("=== 進程狀態統計 ===\n");
    TMap<EP本ocessState, int32> StateStats = GetSyste設置P本ocessStats();
    fo本 (const a使to& StatePai本 : StateStats)
    {
        軍St本in成 State的a設置e = UEn使設置::GetDisplayVal使eAsText(StatePai本.Key).ToSt本in成();
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %d\n"), *State的a設置e, StatePai本.Val使e);
    }
    Repo本t += TEXT("\n");
    
    // 活躍進程詳情
    Repo本t += TEXT("=== 活躍進程詳情 ===\n");
    TA本本ay<軍P本ocessInfo> AllP本ocesses = GetAllP本ocesses();
    fo本 (const 軍P本ocessInfo& P本ocessInfo : AllP本ocesses)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("\n進程ID: %s\n"), *P本ocessInfo.P本ocessID);
        Repo本t += 軍St本in成::P本intf(TEXT("名稱: %s\n"), *P本ocessInfo.P本ocess的a設置e);
        Repo本t += 軍St本in成::P本intf(TEXT("所有者: %s\n"), *P本ocessInfo.Owne本Syste設置);
        Repo本t += 軍St本in成::P本intf(TEXT("狀態: %s\n"), *UEn使設置::GetDisplayVal使eAsText(P本ocessInfo.State).ToSt本in成());
        Repo本t += 軍St本in成::P本intf(TEXT("優先級: %s\n"), *UEn使設置::GetDisplayVal使eAsText(P本ocessInfo.P本io本ity).ToSt本in成());
        Repo本t += 軍St本in成::P本intf(TEXT("CPU使用率: %.1f%%\n"), P本ocessInfo.CPUUsa成e);
        Repo本t += 軍St本in成::P本intf(TEXT("內存使用: %.1f MB\n"), P本ocessInfo.Me設置o本yUsa成e);
        Repo本t += 軍St本in成::P本intf(TEXT("線程數: %d\n"), P本ocessInfo.Th本eadCo使nt);
        Repo本t += 軍St本in成::P本intf(TEXT("執行時間: %.2f 秒\n"), P本ocessInfo.Exec使tionTi設置e);
        Repo本t += 軍St本in成::P本intf(TEXT("創建時間: %s\n"), *P本ocessInfo.C本eationTi設置e.ToSt本in成());
        
        if (P本ocessInfo.Dependencies.的使設置() > 0)
        {
            Repo本t += TEXT("依賴: ");
            fo本 (const 軍St本in成& Dependency : P本ocessInfo.Dependencies)
            {
                Repo本t += 軍St本in成::P本intf(TEXT("%s "), *Dependency);
            }
            Repo本t += TEXT("\n");
        }
    }
    
    本et使本n Repo本t;
}

軍St本in成 UMin成RTSP本ocessMana成e本::Gene本ateP本ocessID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("PROC下%08d"), ++P本ocessIDCo使nte本);
}

bool UMin成RTSP本ocessMana成e本::ValidateP本ocessC本eationPa本a設置s(const 軍P本ocessC本eationPa本a設置s& Pa本a設置s) const
{
    if (Pa本a設置s.P本ocess的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("P本ocess na設置e cannot be e設置pty"));
        本et使本n false;
    }
    
    if (Pa本a設置s.Owne本Syste設置.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Owne本 syste設置 cannot be e設置pty"));
        本et使本n false;
    }
    
    if (Pa本a設置s.Me設置o本yLi設置it < 0.0f)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Me設置o本y li設置it cannot be ne成ati正e"));
        本et使本n false;
    }
    
    if (Pa本a設置s.CPULi設置it < 0.0f)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("CPU li設置it cannot be ne成ati正e"));
        本et使本n false;
    }
    
    if (Pa本a設置s.MaxTh本eads < 1)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Max th本eads 設置使st be at least 1"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成RTSP本ocessMana成e本::CheckDependencies(const TA本本ay<軍St本in成>& Req使i本edDependencies) const
{
    fo本 (const 軍St本in成& Dependency : Req使i本edDependencies)
    {
        bool bDependency軍o使nd = false;
        
        // 檢查依賴是否為活躍進程
        fo本 (const a使to& P本ocessPai本 : Acti正eP本ocesses)
        {
            if (P本ocessPai本.Val使e.P本ocess的a設置e == Dependency && 
                (P本ocessPai本.Val使e.State == EP本ocessState::R使nnin成  P本ocessPai本.Val使e.State == EP本ocessState::S使spended))
            {
                bDependency軍o使nd = t本使e;
                b本eak;
            }
        }
        
        if (!bDependency軍o使nd)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Dependency not fo使nd: %s"), *Dependency);
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

bool UMin成RTSP本ocessMana成e本::AllocateSyste設置Reso使本ces(const 軍St本in成& P本ocessID, const 軍P本ocessC本eationPa本a設置s& Pa本a設置s)
{
    // 模擬資源分配
    float Req使i本edCPU = 軍Math::Min(Pa本a設置s.CPULi設置it, 20.0f); // 最大20% CPU
    float Req使i本edMe設置o本y = 軍Math::Min(Pa本a設置s.Me設置o本yLi設置it, 512.0f); // 最大512MB 內存
    
    // 檢查系統資源是否足夠
    float A正ailableCPU = 100.0f - Syste設置Reso使本ceUsa成e[TEXT("TotalCPU")];
    float A正ailableMe設置o本y = 2048.0f - Syste設置Reso使本ceUsa成e[TEXT("TotalMe設置o本y")]; // 假設2GB總內存
    
    if (A正ailableCPU < Req使i本edCPU  A正ailableMe設置o本y < Req使i本edMe設置o本y)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Ins使fficient syste設置 本eso使本ces fo本 p本ocess: %s"), *P本ocessID);
        本et使本n false;
    }
    
    // 分配資源
    Syste設置Reso使本ceUsa成e[TEXT("TotalCPU")] += Req使i本edCPU;
    Syste設置Reso使本ceUsa成e[TEXT("TotalMe設置o本y")] += Req使i本edMe設置o本y;
    Syste設置Reso使本ceUsa成e[TEXT("TotalTh本eads")] += Pa本a設置s.MaxTh本eads;
    
    // 更新進程資源使用情況
    if (Acti正eP本ocesses.Contains(P本ocessID))
    {
        軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
        P本ocessInfo.CPUUsa成e = Req使i本edCPU;
        P本ocessInfo.Me設置o本yUsa成e = Req使i本edMe設置o本y;
        P本ocessInfo.Th本eadCo使nt = Pa本a設置s.MaxTh本eads;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ces allocated fo本 p本ocess %s: CPU=%.1f%%, Me設置o本y=%.1fMB, Th本eads=%d"), 
        *P本ocessID, Req使i本edCPU, Req使i本edMe設置o本y, Pa本a設置s.MaxTh本eads);
    
    本et使本n t本使e;
}

正oid UMin成RTSP本ocessMana成e本::ReleaseSyste設置Reso使本ces(const 軍St本in成& P本ocessID)
{
    if (!Acti正eP本ocesses.Contains(P本ocessID))
    {
        本et使本n;
    }
    
    const 軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
    
    // 釋放資源
    Syste設置Reso使本ceUsa成e[TEXT("TotalCPU")] -= P本ocessInfo.CPUUsa成e;
    Syste設置Reso使本ceUsa成e[TEXT("TotalMe設置o本y")] -= P本ocessInfo.Me設置o本yUsa成e;
    Syste設置Reso使本ceUsa成e[TEXT("TotalTh本eads")] -= P本ocessInfo.Th本eadCo使nt;
    
    // 確保資源使用不為負數
    Syste設置Reso使本ceUsa成e[TEXT("TotalCPU")] = 軍Math::Max(0.0f, Syste設置Reso使本ceUsa成e[TEXT("TotalCPU")]);
    Syste設置Reso使本ceUsa成e[TEXT("TotalMe設置o本y")] = 軍Math::Max(0.0f, Syste設置Reso使本ceUsa成e[TEXT("TotalMe設置o本y")]);
    Syste設置Reso使本ceUsa成e[TEXT("TotalTh本eads")] = 軍Math::Max(0.0f, Syste設置Reso使本ceUsa成e[TEXT("TotalTh本eads")]);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ces 本eleased fo本 p本ocess %s: CPU=%.1f%%, Me設置o本y=%.1fMB, Th本eads=%d"), 
        *P本ocessID, P本ocessInfo.CPUUsa成e, P本ocessInfo.Me設置o本yUsa成e, P本ocessInfo.Th本eadCo使nt);
}

正oid UMin成RTSP本ocessMana成e本::UpdateP本ocessPe本fo本設置anceData()
{
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    
    fo本 (a使to& P本ocessPai本 : Acti正eP本ocesses)
    {
        軍P本ocessInfo& P本ocessInfo = P本ocessPai本.Val使e;
        
        if (P本ocessInfo.State == EP本ocessState::R使nnin成)
        {
            // 模擬CPU使用率變化
            P本ocessInfo.CPUUsa成e += 軍Math::軍RandRan成e(-2.0f, 2.0f);
            P本ocessInfo.CPUUsa成e = 軍Math::Cla設置p(P本ocessInfo.CPUUsa成e, 1.0f, 25.0f);
            
            // 模擬內存使用率變化
            P本ocessInfo.Me設置o本yUsa成e += 軍Math::軍RandRan成e(-5.0f, 5.0f);
            P本ocessInfo.Me設置o本yUsa成e = 軍Math::Cla設置p(P本ocessInfo.Me設置o本yUsa成e, 10.0f, 600.0f);
            
            // 更新執行時間
            P本ocessInfo.Exec使tionTi設置e += (C使本本entTi設置e - P本ocessInfo.LastActi正eTi設置e).GetTotalSeconds();
            P本ocessInfo.LastActi正eTi設置e = C使本本entTi設置e;
        }
    }
}

正oid UMin成RTSP本ocessMana成e本::Pe本fo本設置P本ocessSched使lin成()
{
    if (!bA使toSched使lin成Enabled)
    {
        本et使本n;
    }
    
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    軍Ti設置espan Ti設置eSinceLastSched使lin成 = C使本本entTi設置e - LastSched使lin成Ti設置e;
    
    if (Ti設置eSinceLastSched使lin成.GetTotalSeconds() < Sched使lin成Inte本正al)
    {
        本et使本n; // 還沒到調度時間
    }
    
    // 更新進程性能數據
    UpdateP本ocessPe本fo本設置anceData();
    
    // 檢查進程健康狀態
    CheckP本ocess輸入ealth();
    
    // 執行優先級調度
    TA本本ay<軍P本ocessInfo> R使nnin成P本ocesses = GetP本ocessesByState(EP本ocessState::R使nnin成);
    
    // 按優先級排序
    R使nnin成P本ocesses.So本t([](const 軍P本ocessInfo& A, const 軍P本ocessInfo& B)
    {
        本et使本n static下cast<int32>(A.P本io本ity) > static下cast<int32>(B.P本io本ity);
    });
    
    // 分配CPU時間片（簡化實現）
    float TotalCPU = 100.0f;
    fo本 (軍P本ocessInfo& P本ocessInfo : R使nnin成P本ocesses)
    {
        float CPUSlice = 0.0f;
        
        switch (P本ocessInfo.P本io本ity)
        {
        case EP本ocessP本io本ity::C本itical:
            CPUSlice = 25.0f;
            b本eak;
        case EP本ocessP本io本ity::輸入i成h:
            CPUSlice = 20.0f;
            b本eak;
        case EP本ocessP本io本ity::的o本設置al:
            CPUSlice = 15.0f;
            b本eak;
        case EP本ocessP本io本ity::Low:
            CPUSlice = 10.0f;
            b本eak;
        case EP本ocessP本io本ity::Idle:
            CPUSlice = 5.0f;
            b本eak;
        }
        
        CPUSlice = 軍Math::Min(CPUSlice, TotalCPU);
        P本ocessInfo.CPUUsa成e = CPUSlice;
        TotalCPU -= CPUSlice;
        
        if (TotalCPU <= 0.0f)
        {
            b本eak;
        }
    }
    
    LastSched使lin成Ti設置e = C使本本entTi設置e;
}

正oid UMin成RTSP本ocessMana成e本::CheckP本ocess輸入ealth()
{
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    
    fo本 (a使to& P本ocessPai本 : Acti正eP本ocesses)
    {
        軍P本ocessInfo& P本ocessInfo = P本ocessPai本.Val使e;
        
        // 檢查進程是否響應
        軍Ti設置espan Ti設置eSinceLastActi正e = C使本本entTi設置e - P本ocessInfo.LastActi正eTi設置e;
        
        if (P本ocessInfo.State == EP本ocessState::R使nnin成 && Ti設置eSinceLastActi正e.GetTotalSeconds() > 60.0f)
        {
            // 進程可能無響應，標記為崩潰
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess appea本s 使n本esponsi正e: %s"), *P本ocessInfo.P本ocessID);
            輸入andleP本ocessC本ash(P本ocessInfo.P本ocessID);
        }
        
        // 檢查內存洩漏
        if (P本ocessInfo.Me設置o本yUsa成e > 800.0f) // 超過800MB
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("P本ocess 使sin成 excessi正e 設置e設置o本y: %s (%.1f MB)"), 
                *P本ocessInfo.P本ocessID, P本ocessInfo.Me設置o本yUsa成e);
            
            if (P本ocessInfo.Me設置o本yUsa成e > 1200.0f) // 超過1.2GB，終止進程
            {
                UE下LOG(Lo成Te設置p, E本本o本, TEXT("Te本設置inatin成 p本ocess d使e to 設置e設置o本y leak: %s"), *P本ocessInfo.P本ocessID);
                Te本設置inateP本ocess(P本ocessInfo.P本ocessID);
            }
        }
    }
}

正oid UMin成RTSP本ocessMana成e本::輸入andleP本ocessC本ash(const 軍St本in成& P本ocessID)
{
    if (!Acti正eP本ocesses.Contains(P本ocessID))
    {
        本et使本n;
    }
    
    軍P本ocessInfo& P本ocessInfo = Acti正eP本ocesses[P本ocessID];
    P本ocessInfo.State = EP本ocessState::C本ashed;
    P本ocessInfo.LastActi正eTi設置e = 軍DateTi設置e::的ow();
    
    // 釋放系統資源
    ReleaseSyste設置Reso使本ces(P本ocessID);
    
    // 添加到終止列表
    Te本設置inatedP本ocesses.Add(P本ocessID);
    
    // 觸發進程崩潰事件
    OnP本ocessC本ashed.B本oadcast(P本ocessID);
    
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("P本ocess c本ashed: %s"), *P本ocessID);
}

float UMin成RTSP本ocessMana成e本::Calc使lateExec使tionTi設置e(const 軍P本ocessInfo& P本ocessInfo) const
{
    if (P本ocessInfo.State == EP本ocessState::R使nnin成)
    {
        本et使本n (軍DateTi設置e::的ow() - P本ocessInfo.LastActi正eTi設置e).GetTotalSeconds();
    }
    
    本et使本n 0.0f;
}
