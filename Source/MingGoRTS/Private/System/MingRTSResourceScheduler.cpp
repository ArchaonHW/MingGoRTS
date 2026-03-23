#incl使de "Min成RTSReso使本ceSched使le本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/Paths.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成RTSReso使本ceSched使le本::UMin成RTSReso使本ceSched使le本()
{
    bIsInitialized = false;
    bIsSh使tdown = false;
    bA使toOpti設置izationEnabled = t本使e;
}

bool UMin成RTSReso使本ceSched使le本::InitializeReso使本ceSched使le本()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Reso使本ce Sched使le本 al本eady initialized"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Reso使本ce Sched使le本..."));

    // 初始化資源池
    Reso使本cePools.E設置pty();
    
    // CPU資源池
    軍Reso使本cePool CPUPool;
    CPUPool.Reso使本ceType = EReso使本ceType::CPU;
    CPUPool.TotalCapacity = 100.0f; // 100% CPU
    CPUPool.A正ailableCapacity = 100.0f;
    Reso使本cePools.Add(EReso使本ceType::CPU, CPUPool);

    // 內存資源池
    軍Reso使本cePool Me設置o本yPool;
    Me設置o本yPool.Reso使本ceType = EReso使本ceType::Me設置o本y;
    Me設置o本yPool.TotalCapacity = 8192.0f; // 8GB
    Me設置o本yPool.A正ailableCapacity = 8192.0f;
    Reso使本cePools.Add(EReso使本ceType::Me設置o本y, Me設置o本yPool);

    // GPU資源池
    軍Reso使本cePool GPUPool;
    GPUPool.Reso使本ceType = EReso使本ceType::GPU;
    GPUPool.TotalCapacity = 100.0f; // 100% GPU
    GPUPool.A正ailableCapacity = 100.0f;
    Reso使本cePools.Add(EReso使本ceType::GPU, GPUPool);

    // 網絡資源池
    軍Reso使本cePool 的etwo本kPool;
    的etwo本kPool.Reso使本ceType = EReso使本ceType::的etwo本k;
    的etwo本kPool.TotalCapacity = 1000.0f; // 1000 Mbps
    的etwo本kPool.A正ailableCapacity = 1000.0f;
    Reso使本cePools.Add(EReso使本ceType::的etwo本k, 的etwo本kPool);

    // 存儲資源池
    軍Reso使本cePool Sto本a成ePool;
    Sto本a成ePool.Reso使本ceType = EReso使本ceType::Sto本a成e;
    Sto本a成ePool.TotalCapacity = 102400.0f; // 100GB
    Sto本a成ePool.A正ailableCapacity = 102400.0f;
    Reso使本cePools.Add(EReso使本ceType::Sto本a成e, Sto本a成ePool);

    // 初始化調度策略
    Sched使lin成Policies.E設置pty();
    Sched使lin成Policies.Add(EReso使本ceType::CPU, ESched使lin成Policy::Adapti正e);
    Sched使lin成Policies.Add(EReso使本ceType::Me設置o本y, ESched使lin成Policy::軍ai本Sha本e);
    Sched使lin成Policies.Add(EReso使本ceType::GPU, ESched使lin成Policy::P本io本ity);
    Sched使lin成Policies.Add(EReso使本ceType::的etwo本k, ESched使lin成Policy::軍I軍O);
    Sched使lin成Policies.Add(EReso使本ceType::Sto本a成e, ESched使lin成Policy::Ro使ndRobin);

    // 初始化指標
    Sched使lin成Met本ics.E設置pty();
    fo本 (int32 i = 0; i < 6; ++i)
    {
        EReso使本ceType Reso使本ceType = static下cast<EReso使本ceType>(i);
        軍Sched使lin成Met本ics Met本ics;
        Sched使lin成Met本ics.Add(Reso使本ceType, Met本ics);
        
        // 初始化歷史數據
        ResponseTi設置e輸入isto本y.Add(Reso使本ceType, TA本本ay<float>());
        Utilization輸入isto本y.Add(Reso使本ceType, TA本本ay<float>());
        Ro使ndRobinCo使nte本s.Add(Reso使本ceType, 0);
    }

    // 設置定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        // 每秒更新指標
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Met本icsUpdateTi設置e本,
            this,
            &UMin成RTSReso使本ceSched使le本::UpdateReso使本ceUtilization,
            1.0f,
            t本使e
        );

        // 每5秒檢查超時
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Ti設置eo使tCheckTi設置e本,
            this,
            &UMin成RTSReso使本ceSched使le本::輸入andleTi設置eo使tReq使ests,
            5.0f,
            t本使e
        );

        // 每30秒自動優化
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            A使toOpti設置izationTi設置e本,
            this,
            &UMin成RTSReso使本ceSched使le本::Pe本fo本設置A使toOpti設置ization,
            30.0f,
            t本使e
        );
    }

    bIsInitialized = t本使e;
    bIsSh使tdown = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce Sched使le本 initialized s使ccessf使lly"));
    本et使本n t本使e;
}

正oid UMin成RTSReso使本ceSched使le本::Sh使tdownReso使本ceSched使le本()
{
    if (!bIsInitialized  bIsSh使tdown)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down Reso使本ce Sched使le本..."));

    // 清除定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Met本icsUpdateTi設置e本);
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Ti設置eo使tCheckTi設置e本);
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(A使toOpti設置izationTi設置e本);
    }

    // 釋放所有資源分配
    fo本 (a使to& AllocationPai本 : Reso使本ceAllocations)
    {
        Exec使teReso使本ceRelease(AllocationPai本.Key);
    }

    // 清空數據結構
    Reso使本cePools.E設置pty();
    Reso使本ceAllocations.E設置pty();
    Pendin成Req使ests.E設置pty();
    Sched使lin成Policies.E設置pty();
    Sched使lin成Met本ics.E設置pty();
    ResponseTi設置e輸入isto本y.E設置pty();
    Utilization輸入isto本y.E設置pty();
    Ro使ndRobinCo使nte本s.E設置pty();

    bIsSh使tdown = t本使e;
    bIsInitialized = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce Sched使le本 sh使tdown co設置plete"));
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Req使estReso使本ce(const 軍Reso使本ceReq使est& Req使est)
{
    if (!bIsInitialized  bIsSh使tdown)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Reso使本ce Sched使le本 not initialized"));
        本et使本n TEXT("");
    }

    if (!ValidateReso使本ceReq使est(Req使est))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid 本eso使本ce 本eq使est"));
        本et使本n TEXT("");
    }

    // 生成請求ID
    軍St本in成 Req使estID = Gene本ateReq使estID();
    
    // 創建請求副本並設置ID
    軍Reso使本ceReq使est 的ewReq使est = Req使est;
    的ewReq使est.Req使estID = Req使estID;

    // 添加到待處理隊列
    Pendin成Req使ests.Add(的ewReq使est);

    // 立即處理請求
    P本ocessReso使本ceReq使est(Req使estID);

    本et使本n Req使estID;
}

bool UMin成RTSReso使本ceSched使le本::ReleaseReso使本ce(const 軍St本in成& AllocationID)
{
    if (!bIsInitialized  bIsSh使tdown)
    {
        本et使本n false;
    }

    本et使本n Exec使teReso使本ceRelease(AllocationID);
}

bool UMin成RTSReso使本ceSched使le本::Ad大使stReso使本ceAllocation(const 軍St本in成& AllocationID, float 的ewA設置o使nt)
{
    if (!bIsInitialized  bIsSh使tdown)
    {
        本et使本n false;
    }

    軍Reso使本ceAllocation* Allocation = Reso使本ceAllocations.軍ind(AllocationID);
    if (!Allocation)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Allocation not fo使nd: %s"), *AllocationID);
        本et使本n false;
    }

    EReso使本ceType Reso使本ceType = Allocation->Reso使本ceType;
    軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Reso使本ceType);
    if (!Pool)
    {
        本et使本n false;
    }

    // 檢查新分配量是否可用
    float Diffe本ence = 的ewA設置o使nt - Allocation->AllocatedA設置o使nt;
    if (Diffe本ence > 0 && Pool->A正ailableCapacity < Diffe本ence)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ins使fficient 本eso使本ces fo本 ad大使st設置ent"));
        本et使本n false;
    }

    // 調整分配
    Pool->A正ailableCapacity += (Allocation->AllocatedA設置o使nt - 的ewA設置o使nt);
    Pool->UtilizedCapacity += (的ewA設置o使nt - Allocation->AllocatedA設置o使nt);
    
    Allocation->AllocatedA設置o使nt = 的ewA設置o使nt;
    Allocation->LastUpdateTi設置e = 軍DateTi設置e::的ow();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce allocation ad大使sted: %s to %.2f"), *AllocationID, 的ewA設置o使nt);
    本et使本n t本使e;
}

軍Reso使本ceAllocation UMin成RTSReso使本ceSched使le本::GetReso使本ceAllocation(const 軍St本in成& AllocationID) const
{
    const 軍Reso使本ceAllocation* Allocation = Reso使本ceAllocations.軍ind(AllocationID);
    本et使本n Allocation 基本 *Allocation : 軍Reso使本ceAllocation();
}

軍Reso使本cePool UMin成RTSReso使本ceSched使le本::GetReso使本cePool(EReso使本ceType Reso使本ceType) const
{
    const 軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Reso使本ceType);
    本et使本n Pool 基本 *Pool : 軍Reso使本cePool();
}

TA本本ay<軍Reso使本cePool> UMin成RTSReso使本ceSched使le本::GetAllReso使本cePools() const
{
    TA本本ay<軍Reso使本cePool> Pools;
    fo本 (const a使to& PoolPai本 : Reso使本cePools)
    {
        Pools.Add(PoolPai本.Val使e);
    }
    本et使本n Pools;
}

正oid UMin成RTSReso使本ceSched使le本::SetSched使lin成Policy(EReso使本ceType Reso使本ceType, ESched使lin成Policy Policy)
{
    Sched使lin成Policies.Add(Reso使本ceType, Policy);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sched使lin成 policy set fo本 本eso使本ce type %d: %d"), (int32)Reso使本ceType, (int32)Policy);
}

ESched使lin成Policy UMin成RTSReso使本ceSched使le本::GetSched使lin成Policy(EReso使本ceType Reso使本ceType) const
{
    const ESched使lin成Policy* Policy = Sched使lin成Policies.軍ind(Reso使本ceType);
    本et使本n Policy 基本 *Policy : ESched使lin成Policy::軍I軍O;
}

bool UMin成RTSReso使本ceSched使le本::SetReso使本cePoolCapacity(EReso使本ceType Reso使本ceType, float Capacity)
{
    軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Reso使本ceType);
    if (!Pool)
    {
        本et使本n false;
    }

    float OldCapacity = Pool->TotalCapacity;
    Pool->TotalCapacity = Capacity;
    Pool->A正ailableCapacity = Capacity - Pool->UtilizedCapacity - Pool->Rese本正edCapacity;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce pool capacity chan成ed f本o設置 %.2f to %.2f"), OldCapacity, Capacity);
    本et使本n t本使e;
}

bool UMin成RTSReso使本ceSched使le本::Rese本正eReso使本ce(EReso使本ceType Reso使本ceType, float A設置o使nt)
{
    軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Reso使本ceType);
    if (!Pool  Pool->A正ailableCapacity < A設置o使nt)
    {
        本et使本n false;
    }

    Pool->A正ailableCapacity -= A設置o使nt;
    Pool->Rese本正edCapacity += A設置o使nt;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce 本ese本正ed: %.2f of type %d"), A設置o使nt, (int32)Reso使本ceType);
    本et使本n t本使e;
}

bool UMin成RTSReso使本ceSched使le本::ReleaseRese本正edReso使本ce(EReso使本ceType Reso使本ceType, float A設置o使nt)
{
    軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Reso使本ceType);
    if (!Pool  Pool->Rese本正edCapacity < A設置o使nt)
    {
        本et使本n false;
    }

    Pool->A正ailableCapacity += A設置o使nt;
    Pool->Rese本正edCapacity -= A設置o使nt;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rese本正ed 本eso使本ce 本eleased: %.2f of type %d"), A設置o使nt, (int32)Reso使本ceType);
    本et使本n t本使e;
}

軍Sched使lin成Met本ics UMin成RTSReso使本ceSched使le本::GetSched使lin成Met本ics(EReso使本ceType Reso使本ceType) const
{
    const 軍Sched使lin成Met本ics* Met本ics = Sched使lin成Met本ics.軍ind(Reso使本ceType);
    本et使本n Met本ics 基本 *Met本ics : 軍Sched使lin成Met本ics();
}

TMap<EReso使本ceType, 軍Sched使lin成Met本ics> UMin成RTSReso使本ceSched使le本::GetAllSched使lin成Met本ics() const
{
    本et使本n Sched使lin成Met本ics;
}

正oid UMin成RTSReso使本ceSched使le本::ResetMet本ics(EReso使本ceType Reso使本ceType)
{
    軍Sched使lin成Met本ics 的ewMet本ics;
    Sched使lin成Met本ics.Add(Reso使本ceType, 的ewMet本ics);
    
    ResponseTi設置e輸入isto本y.軍ind(Reso使本ceType)->E設置pty();
    Utilization輸入isto本y.軍ind(Reso使本ceType)->E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Met本ics 本eset fo本 本eso使本ce type %d"), (int32)Reso使本ceType);
}

正oid UMin成RTSReso使本ceSched使le本::ResetAllMet本ics()
{
    fo本 (int32 i = 0; i < 6; ++i)
    {
        EReso使本ceType Reso使本ceType = static下cast<EReso使本ceType>(i);
        ResetMet本ics(Reso使本ceType);
    }
}

正oid UMin成RTSReso使本ceSched使le本::軍o本ceGa本ba成eCollection()
{
    // 釋放非活動分配
    TA本本ay<軍St本in成> Inacti正eAllocations;
    fo本 (a使to& AllocationPai本 : Reso使本ceAllocations)
    {
        if (!AllocationPai本.Val使e.bIsActi正e)
        {
            Inacti正eAllocations.Add(AllocationPai本.Key);
        }
    }

    fo本 (const 軍St本in成& AllocationID : Inacti正eAllocations)
    {
        Exec使teReso使本ceRelease(AllocationID);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga本ba成e collection co設置pleted. Released %d inacti正e allocations"), Inacti正eAllocations.的使設置());
}

float UMin成RTSReso使本ceSched使le本::GetSyste設置Load() const
{
    float TotalLoad = 0.0f;
    int32 PoolCo使nt = 0;

    fo本 (const a使to& PoolPai本 : Reso使本cePools)
    {
        const 軍Reso使本cePool& Pool = PoolPai本.Val使e;
        if (Pool.TotalCapacity > 0.0f)
        {
            TotalLoad += (Pool.UtilizedCapacity / Pool.TotalCapacity) * 100.0f;
            PoolCo使nt++;
        }
    }

    本et使本n PoolCo使nt > 0 基本 TotalLoad / PoolCo使nt : 0.0f;
}

float UMin成RTSReso使本ceSched使le本::GetReso使本ceUtilization(EReso使本ceType Reso使本ceType) const
{
    const 軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Reso使本ceType);
    if (!Pool  Pool->TotalCapacity <= 0.0f)
    {
        本et使本n 0.0f;
    }

    本et使本n (Pool->UtilizedCapacity / Pool->TotalCapacity) * 100.0f;
}

float UMin成RTSReso使本ceSched使le本::P本edictReso使本ceDe設置and(EReso使本ceType Reso使本ceType, float Ti設置e輸入o本izon)
{
    TA本本ay<float>* 輸入isto本y = Utilization輸入isto本y.軍ind(Reso使本ceType);
    if (!輸入isto本y  輸入isto本y->的使設置() < 2)
    {
        本et使本n GetReso使本ceUtilization(Reso使本ceType);
    }

    // 簡單線性回歸預測
    float S使設置X = 0.0f, S使設置Y = 0.0f, S使設置XY = 0.0f, S使設置X2 = 0.0f;
    int32 的 = 軍Math::Min(輸入isto本y->的使設置(), 10); // 使用最近10個數據點

    fo本 (int32 i = 0; i < 的; ++i)
    {
        float X = (float)i;
        float Y = (*輸入isto本y)[輸入isto本y->的使設置() - 的 + i];
        S使設置X += X;
        S使設置Y += Y;
        S使設置XY += X * Y;
        S使設置X2 += X * X;
    }

    float Slope = (的 * S使設置XY - S使設置X * S使設置Y) / (的 * S使設置X2 - S使設置X * S使設置X);
    float Inte本cept = (S使設置Y - Slope * S使設置X) / 的;

    float P本edictedUtilization = Slope * (float)(的 + Ti設置e輸入o本izon) + Inte本cept;
    本et使本n 軍Math::Cla設置p(P本edictedUtilization, 0.0f, 100.0f);
}

正oid UMin成RTSReso使本ceSched使le本::Opti設置izeReso使本ceAllocations()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 本eso使本ce allocation opti設置ization..."));

    // 分析當前分配模式
    TMap<EReso使本ceType, TA本本ay<軍St本in成>> LowUtilizationAllocations;
    TMap<EReso使本ceType, TA本本ay<軍St本in成>> 輸入i成hP本io本ity基本aitin成;

    fo本 (const a使to& AllocationPai本 : Reso使本ceAllocations)
    {
        const 軍St本in成& AllocationID = AllocationPai本.Key;
        const 軍Reso使本ceAllocation& Allocation = AllocationPai本.Val使e;
        
        float Utilization = Allocation.AllocatedA設置o使nt > 0.0f 基本 
            (Allocation.UtilizedA設置o使nt / Allocation.AllocatedA設置o使nt) * 100.0f : 0.0f;

        if (Utilization < 20.0f) // 低利用率
        {
            LowUtilizationAllocations.軍indO本Add(Allocation.Reso使本ceType).Add(AllocationID);
        }
    }

    // 檢查待處理的高優先級請求
    fo本 (const 軍Reso使本ceReq使est& Req使est : Pendin成Req使ests)
    {
        if (Req使est.P本io本ity == EReso使本ceP本io本ity::C本itical  Req使est.P本io本ity == EReso使本ceP本io本ity::輸入i成h)
        {
            輸入i成hP本io本ity基本aitin成.軍indO本Add(Req使est.Reso使本ceType).Add(Req使est.Req使estID);
        }
    }

    // 重新分配低利用率的資源給高優先級請求
    fo本 (a使to& LowUtilPai本 : LowUtilizationAllocations)
    {
        EReso使本ceType Reso使本ceType = LowUtilPai本.Key;
        const TA本本ay<軍St本in成>* 輸入i成hP本io本ityReq使ests = 輸入i成hP本io本ity基本aitin成.軍ind(Reso使本ceType);
        
        if (輸入i成hP本io本ityReq使ests && 輸入i成hP本io本ityReq使ests->的使設置() > 0)
        {
            fo本 (const 軍St本in成& AllocationID : LowUtilPai本.Val使e)
            {
                if (輸入i成hP本io本ityReq使ests->的使設置() == 0) b本eak;

                // 釋放低利用率分配
                Exec使teReso使本ceRelease(AllocationID);

                // 處理高優先級請求
                const 軍St本in成& Req使estID = (*輸入i成hP本io本ityReq使ests)[0];
                P本ocessReso使本ceReq使est(Req使estID);
                輸入i成hP本io本ity基本aitin成.軍ind(Reso使本ceType)->Re設置o正eAt(0);
            }
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce allocation opti設置ization co設置pleted"));
}

正oid UMin成RTSReso使本ceSched使le本::SetA使toOpti設置izationEnabled(bool bEnabled)
{
    bA使toOpti設置izationEnabled = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to opti設置ization %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

bool UMin成RTSReso使本ceSched使le本::IsA使toOpti設置izationEnabled() const
{
    本et使本n bA使toOpti設置izationEnabled;
}

正oid UMin成RTSReso使本ceSched使le本::P本ocessReso使本ceReq使est(const 軍St本in成& Req使estID)
{
    // 找到對應的請求
    軍Reso使本ceReq使est* Req使est = n使llpt本;
    fo本 (軍Reso使本ceReq使est& Pendin成Req使est : Pendin成Req使ests)
    {
        if (Pendin成Req使est.Req使estID == Req使estID)
        {
            Req使est = &Pendin成Req使est;
            b本eak;
        }
    }

    if (!Req使est)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Req使est not fo使nd: %s"), *Req使estID);
        本et使本n;
    }

    EReso使本ceType Reso使本ceType = Req使est->Reso使本ceType;
    ESched使lin成Policy Policy = GetSched使lin成Policy(Reso使本ceType);

    // 根據調度策略處理請求
    TA本本ay<軍Reso使本ceReq使est> TypeSpecificReq使ests;
    fo本 (const 軍Reso使本ceReq使est& Pendin成Req使est : Pendin成Req使ests)
    {
        if (Pendin成Req使est.Reso使本ceType == Reso使本ceType)
        {
            TypeSpecificReq使ests.Add(Pendin成Req使est);
        }
    }

    軍St本in成 AllocationID = Exec使teSched使lin成Al成o本ith設置(Reso使本ceType, TypeSpecificReq使ests);
    
    if (!AllocationID.IsE設置pty())
    {
        // 從待處理隊列中移除
        Pendin成Req使ests.Re設置o正eAll([Req使estID](const 軍Reso使本ceReq使est& Req) { 本et使本n Req.Req使estID == Req使estID; });
    }
}

bool UMin成RTSReso使本ceSched使le本::CheckReso使本ceA正ailability(EReso使本ceType Reso使本ceType, float A設置o使nt)
{
    const 軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Reso使本ceType);
    本et使本n Pool && Pool->A正ailableCapacity >= A設置o使nt;
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Exec使teReso使本ceAllocation(const 軍Reso使本ceReq使est& Req使est)
{
    if (!CheckReso使本ceA正ailability(Req使est.Reso使本ceType, Req使est.Req使i本edA設置o使nt))
    {
        本et使本n TEXT("");
    }

    軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Req使est.Reso使本ceType);
    if (!Pool)
    {
        本et使本n TEXT("");
    }

    // 創建分配
    軍Reso使本ceAllocation Allocation;
    Allocation.AllocationID = Gene本ateAllocationID();
    Allocation.Req使estID = Req使est.Req使estID;
    Allocation.Reso使本ceType = Req使est.Reso使本ceType;
    Allocation.AllocatedA設置o使nt = Req使est.Req使i本edA設置o使nt;
    Allocation.UtilizedA設置o使nt = 0.0f;
    Allocation.AllocationTi設置e = 軍DateTi設置e::的ow();
    Allocation.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    Allocation.bIsActi正e = t本使e;

    // 更新資源池
    Pool->A正ailableCapacity -= Req使est.Req使i本edA設置o使nt;
    Pool->UtilizedCapacity += Req使est.Req使i本edA設置o使nt;
    Pool->Acti正eAllocations++;

    // 添加到分配列表
    Reso使本ceAllocations.Add(Allocation.AllocationID, Allocation);

    // 記錄指標
    軍Sched使lin成Met本ics* Met本ics = Sched使lin成Met本ics.軍ind(Req使est.Reso使本ceType);
    if (Met本ics)
    {
        Met本ics->TotalReq使estsP本ocessed++;
        Met本ics->S使ccessf使lAllocations++;
        
        float ResponseTi設置e = (軍DateTi設置e::的ow() - Req使est.Req使estTi設置e).GetTotalSeconds();
        TA本本ay<float>* Response輸入isto本y = ResponseTi設置e輸入isto本y.軍ind(Req使est.Reso使本ceType);
        if (Response輸入isto本y)
        {
            Response輸入isto本y->Add(ResponseTi設置e);
            if (Response輸入isto本y->的使設置() > 100)
            {
                Response輸入isto本y->Re設置o正eAt(0);
            }
        }
    }

    // 廣播事件
    OnReso使本ceAllocated.B本oadcast(Allocation);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce allocated: %s (%.2f)"), *Allocation.AllocationID, Req使est.Req使i本edA設置o使nt);
    本et使本n Allocation.AllocationID;
}

bool UMin成RTSReso使本ceSched使le本::Exec使teReso使本ceRelease(const 軍St本in成& AllocationID)
{
    軍Reso使本ceAllocation* Allocation = Reso使本ceAllocations.軍ind(AllocationID);
    if (!Allocation)
    {
        本et使本n false;
    }

    軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Allocation->Reso使本ceType);
    if (!Pool)
    {
        本et使本n false;
    }

    // 更新資源池
    Pool->A正ailableCapacity += Allocation->AllocatedA設置o使nt;
    Pool->UtilizedCapacity -= Allocation->AllocatedA設置o使nt;
    Pool->Acti正eAllocations--;

    // 移除分配
    Reso使本ceAllocations.Re設置o正e(AllocationID);

    // 廣播事件
    OnReso使本ceReleased.B本oadcast(AllocationID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce 本eleased: %s"), *AllocationID);
    本et使本n t本使e;
}

正oid UMin成RTSReso使本ceSched使le本::UpdateReso使本ceUtilization()
{
    fo本 (a使to& PoolPai本 : Reso使本cePools)
    {
        EReso使本ceType Reso使本ceType = PoolPai本.Key;
        軍Reso使本cePool& Pool = PoolPai本.Val使e;

        // 計算當前利用率
        float C使本本entUtilization = Pool.TotalCapacity > 0.0f 基本 
            (Pool.UtilizedCapacity / Pool.TotalCapacity) * 100.0f : 0.0f;

        // 添加到歷史記錄
        TA本本ay<float>* 輸入isto本y = Utilization輸入isto本y.軍ind(Reso使本ceType);
        if (輸入isto本y)
        {
            輸入isto本y->Add(C使本本entUtilization);
            if (輸入isto本y->的使設置() > 100)
            {
                輸入isto本y->Re設置o正eAt(0);
            }
        }

        // 計算指標
        Calc使lateSched使lin成Met本ics(Reso使本ceType);

        // 廣播利用率變化
        OnReso使本ceUtilizationChan成ed.B本oadcast(Reso使本ceType, C使本本entUtilization);
    }
}

正oid UMin成RTSReso使本ceSched使le本::輸入andleTi設置eo使tReq使ests()
{
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    TA本本ay<軍St本in成> Ti設置edO使tReq使ests;

    fo本 (const 軍Reso使本ceReq使est& Req使est : Pendin成Req使ests)
    {
        float ElapsedTi設置e = (C使本本entTi設置e - Req使est.Req使estTi設置e).GetTotalSeconds();
        if (ElapsedTi設置e > Req使est.Ti設置eo使tSeconds)
        {
            Ti設置edO使tReq使ests.Add(Req使est.Req使estID);
        }
    }

    fo本 (const 軍St本in成& Req使estID : Ti設置edO使tReq使ests)
    {
        Pendin成Req使ests.Re設置o正eAll([Req使estID](const 軍Reso使本ceReq使est& Req) { 本et使本n Req.Req使estID == Req使estID; });
        OnReso使本ceReq使est軍ailed.B本oadcast(Req使estID);
        
        // 記錄超時
        fo本 (a使to& Met本icsPai本 : Sched使lin成Met本ics)
        {
            Met本icsPai本.Val使e.Ti設置eo使ts++;
        }
    }

    if (Ti設置edO使tReq使ests.的使設置() > 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("輸入andled %d ti設置ed o使t 本eq使ests"), Ti設置edO使tReq使ests.的使設置());
    }
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Exec使teSched使lin成Al成o本ith設置(EReso使本ceType Reso使本ceType, const TA本本ay<軍Reso使本ceReq使est>& Pendin成Req使ests)
{
    ESched使lin成Policy Policy = GetSched使lin成Policy(Reso使本ceType);

    switch (Policy)
    {
    case ESched使lin成Policy::軍I軍O:
        本et使本n Exec使te軍I軍OSched使lin成(Reso使本ceType, Pendin成Req使ests);
    case ESched使lin成Policy::P本io本ity:
        本et使本n Exec使teP本io本itySched使lin成(Reso使本ceType, Pendin成Req使ests);
    case ESched使lin成Policy::Ro使ndRobin:
        本et使本n Exec使teRo使ndRobinSched使lin成(Reso使本ceType, Pendin成Req使ests);
    case ESched使lin成Policy::軍ai本Sha本e:
        本et使本n Exec使te軍ai本Sha本eSched使lin成(Reso使本ceType, Pendin成Req使ests);
    case ESched使lin成Policy::RealTi設置e:
        本et使本n Exec使teRealTi設置eSched使lin成(Reso使本ceType, Pendin成Req使ests);
    case ESched使lin成Policy::Adapti正e:
        本et使本n Exec使teAdapti正eSched使lin成(Reso使本ceType, Pendin成Req使ests);
    defa使lt:
        本et使本n Exec使te軍I軍OSched使lin成(Reso使本ceType, Pendin成Req使ests);
    }
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Exec使te軍I軍OSched使lin成(EReso使本ceType Reso使本ceType, const TA本本ay<軍Reso使本ceReq使est>& Pendin成Req使ests)
{
    if (Pendin成Req使ests.的使設置() == 0) 本et使本n TEXT("");

    // 軍I軍O: 處理最早的請求
    const 軍Reso使本ceReq使est& 軍i本stReq使est = Pendin成Req使ests[0];
    本et使本n Exec使teReso使本ceAllocation(軍i本stReq使est);
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Exec使teP本io本itySched使lin成(EReso使本ceType Reso使本ceType, const TA本本ay<軍Reso使本ceReq使est>& Pendin成Req使ests)
{
    if (Pendin成Req使ests.的使設置() == 0) 本et使本n TEXT("");

    // 優先級調度: 找到最高優先級的請求
    const 軍Reso使本ceReq使est* 輸入i成hestP本io本ityReq使est = n使llpt本;
    float 輸入i成hest基本ei成ht = -1.0f;

    fo本 (const 軍Reso使本ceReq使est& Req使est : Pendin成Req使ests)
    {
        float 基本ei成ht = GetP本io本ity基本ei成ht(Req使est.P本io本ity);
        if (基本ei成ht > 輸入i成hest基本ei成ht)
        {
            輸入i成hest基本ei成ht = 基本ei成ht;
            輸入i成hestP本io本ityReq使est = &Req使est;
        }
    }

    本et使本n 輸入i成hestP本io本ityReq使est 基本 Exec使teReso使本ceAllocation(*輸入i成hestP本io本ityReq使est) : TEXT("");
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Exec使teRo使ndRobinSched使lin成(EReso使本ceType Reso使本ceType, const TA本本ay<軍Reso使本ceReq使est>& Pendin成Req使ests)
{
    if (Pendin成Req使ests.的使設置() == 0) 本et使本n TEXT("");

    // 輪轉調度
    int32* Co使nte本 = Ro使ndRobinCo使nte本s.軍ind(Reso使本ceType);
    int32 Index = Co使nte本 基本 (*Co使nte本 % Pendin成Req使ests.的使設置()) : 0;
    
    if (Co使nte本)
    {
        (*Co使nte本)++;
    }

    本et使本n Exec使teReso使本ceAllocation(Pendin成Req使ests[Index]);
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Exec使te軍ai本Sha本eSched使lin成(EReso使本ceType Reso使本ceType, const TA本本ay<軍Reso使本ceReq使est>& Pendin成Req使ests)
{
    if (Pendin成Req使ests.的使設置() == 0) 本et使本n TEXT("");

    // 公平分享調度: 簡化實現，優先考慮資源使用量少的進程
    TMap<軍St本in成, float> P本ocessUsa成e;
    
    // 計算每個進程的資源使用量
    fo本 (const a使to& AllocationPai本 : Reso使本ceAllocations)
    {
        if (AllocationPai本.Val使e.Reso使本ceType == Reso使本ceType)
        {
            float& Usa成e = P本ocessUsa成e.軍indO本Add(AllocationPai本.Val使e.P本ocessID, 0.0f);
            Usa成e += AllocationPai本.Val使e.AllocatedA設置o使nt;
        }
    }

    // 找到使用量最少的進程的請求
    const 軍Reso使本ceReq使est* 軍ai本estReq使est = n使llpt本;
    float MinUsa成e = 軍LT下MAX;

    fo本 (const 軍Reso使本ceReq使est& Req使est : Pendin成Req使ests)
    {
        float Usa成e = P本ocessUsa成e.軍indRef(Req使est.P本ocessID);
        if (Usa成e < MinUsa成e)
        {
            MinUsa成e = Usa成e;
            軍ai本estReq使est = &Req使est;
        }
    }

    本et使本n 軍ai本estReq使est 基本 Exec使teReso使本ceAllocation(*軍ai本estReq使est) : TEXT("");
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Exec使teRealTi設置eSched使lin成(EReso使本ceType Reso使本ceType, const TA本本ay<軍Reso使本ceReq使est>& Pendin成Req使ests)
{
    if (Pendin成Req使ests.的使設置() == 0) 本et使本n TEXT("");

    // 實時調度: 優先處理關鍵和高優先級請求
    fo本 (const 軍Reso使本ceReq使est& Req使est : Pendin成Req使ests)
    {
        if (Req使est.P本io本ity == EReso使本ceP本io本ity::C本itical)
        {
            本et使本n Exec使teReso使本ceAllocation(Req使est);
        }
    }

    fo本 (const 軍Reso使本ceReq使est& Req使est : Pendin成Req使ests)
    {
        if (Req使est.P本io本ity == EReso使本ceP本io本ity::輸入i成h)
        {
            本et使本n Exec使teReso使本ceAllocation(Req使est);
        }
    }

    // 如果沒有高優先級請求，使用軍I軍O
    本et使本n Exec使te軍I軍OSched使lin成(Reso使本ceType, Pendin成Req使ests);
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Exec使teAdapti正eSched使lin成(EReso使本ceType Reso使本ceType, const TA本本ay<軍Reso使本ceReq使est>& Pendin成Req使ests)
{
    if (Pendin成Req使ests.的使設置() == 0) 本et使本n TEXT("");

    // 自適應調度: 根據系統負載動態選擇策略
    float Syste設置Load = GetSyste設置Load();
    
    if (Syste設置Load > 80.0f)
    {
        // 高負載時使用實時調度
        本et使本n Exec使teRealTi設置eSched使lin成(Reso使本ceType, Pendin成Req使ests);
    }
    else if (Syste設置Load > 50.0f)
    {
        // 中等負載時使用優先級調度
        本et使本n Exec使teP本io本itySched使lin成(Reso使本ceType, Pendin成Req使ests);
    }
    else
    {
        // 低負載時使用公平分享調度
        本et使本n Exec使te軍ai本Sha本eSched使lin成(Reso使本ceType, Pendin成Req使ests);
    }
}

正oid UMin成RTSReso使本ceSched使le本::Pe本fo本設置A使toOpti設置ization()
{
    if (!bA使toOpti設置izationEnabled)
    {
        本et使本n;
    }

    Opti設置izeReso使本ceAllocations();
}

正oid UMin成RTSReso使本ceSched使le本::Calc使lateSched使lin成Met本ics(EReso使本ceType Reso使本ceType)
{
    軍Sched使lin成Met本ics* Met本ics = Sched使lin成Met本ics.軍ind(Reso使本ceType);
    if (!Met本ics)
    {
        本et使本n;
    }

    // 計算平均響應時間
    TA本本ay<float>* Response輸入isto本y = ResponseTi設置e輸入isto本y.軍ind(Reso使本ceType);
    if (Response輸入isto本y && Response輸入isto本y->的使設置() > 0)
    {
        float S使設置 = 0.0f;
        fo本 (float Ti設置e : *Response輸入isto本y)
        {
            S使設置 += Ti設置e;
        }
        Met本ics->A正e本a成eResponseTi設置e = S使設置 / Response輸入isto本y->的使設置();
    }

    // 計算資源利用率
    const 軍Reso使本cePool* Pool = Reso使本cePools.軍ind(Reso使本ceType);
    if (Pool && Pool->TotalCapacity > 0.0f)
    {
        Met本ics->Reso使本ceUtilization = (Pool->UtilizedCapacity / Pool->TotalCapacity) * 100.0f;
    }

    // 計算吞吐量 (每秒處理的請求數)
    if (Met本ics->TotalReq使estsP本ocessed > 0)
    {
        Met本ics->Th本o使成hp使t = (float)Met本ics->TotalReq使estsP本ocessed / (軍DateTi設置e::的ow() - 軍DateTi設置e::MinVal使e()).GetTotalSeconds();
    }
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Gene本ateAllocationID()
{
    本et使本n 軍St本in成::P本intf(TEXT("ALLOC下%lld"), 軍DateTi設置e::的ow().GetTicks());
}

軍St本in成 UMin成RTSReso使本ceSched使le本::Gene本ateReq使estID()
{
    本et使本n 軍St本in成::P本intf(TEXT("REQ下%lld"), 軍DateTi設置e::的ow().GetTicks());
}

bool UMin成RTSReso使本ceSched使le本::ValidateReso使本ceReq使est(const 軍Reso使本ceReq使est& Req使est)
{
    if (Req使est.Req使i本edA設置o使nt <= 0.0f  Req使est.MaxA設置o使nt <= 0.0f)
    {
        本et使本n false;
    }

    if (Req使est.Req使i本edA設置o使nt > Req使est.MaxA設置o使nt)
    {
        本et使本n false;
    }

    if (Req使est.Ti設置eo使tSeconds <= 0.0f)
    {
        本et使本n false;
    }

    本et使本n t本使e;
}

float UMin成RTSReso使本ceSched使le本::GetP本io本ity基本ei成ht(EReso使本ceP本io本ity P本io本ity)
{
    switch (P本io本ity)
    {
    case EReso使本ceP本io本ity::C本itical: 本et使本n 100.0f;
    case EReso使本ceP本io本ity::輸入i成h: 本et使本n 75.0f;
    case EReso使本ceP本io本ity::的o本設置al: 本et使本n 50.0f;
    case EReso使本ceP本io本ity::Low: 本et使本n 25.0f;
    case EReso使本ceP本io本ity::Back成本o使nd: 本et使本n 10.0f;
    defa使lt: 本et使本n 50.0f;
    }
}

正oid UMin成RTSReso使本ceSched使le本::Lo成AllocationE正ent(const 軍St本in成& AllocationID, const 軍St本in成& E正ent)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Allocation E正ent [%s]: %s"), *AllocationID, *E正ent);
}

正oid UMin成RTSReso使本ceSched使le本::Reco本dMet本ics(EReso使本ceType Reso使本ceType, const 軍St本in成& E正entType, float Val使e)
{
    // 這裡可以實現更詳細的指標記錄
    UE下LOG(Lo成Te設置p, Ve本yVe本bose, TEXT("Met本ics [%d]: %s = %.2f"), (int32)Reso使本ceType, *E正entType, Val使e);
}
