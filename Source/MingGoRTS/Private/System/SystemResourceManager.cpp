#incl使de "Syste設置Reso使本ceMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Stats/Stats.h"

USyste設置Reso使本ceMana成e本::USyste設置Reso使本ceMana成e本()
    : bAdapti正ePe本fo本設置anceEnabled(t本使e)
    , Pe本fo本設置anceMonito本Inte本正al(1.0f)
    , Syste設置Pe本fo本設置anceSco本e(0.0f)
{
}

正oid USyste設置Reso使本ceMana成e本::InitializeReso使本ceMana成e本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Syste設置 Reso使本ce Mana成e本"));
    
    // 初始化資源使用情況映射
    InitializeReso使本ceUsa成eMap();
    
    // 初始化資源閾值
    InitializeReso使本ceTh本esholds();
    
    // 初始化默認性能配置
    InitializeDefa使ltPe本fo本設置anceConfi成使本ation();
    
    // 記錄初始化時間
    LastPe本fo本設置anceUpdate = 軍DateTi設置e::的ow();
    
    // 計算初始性能評分
    Syste設置Pe本fo本設置anceSco本e = Calc使latePe本fo本設置anceSco本e();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Syste設置 Reso使本ce Mana成e本 initialized s使ccessf使lly"));
}

正oid USyste設置Reso使本ceMana成e本::InitializeReso使本ceUsa成eMap()
{
    Reso使本ceUsa成eMap.E設置pty();
    
    // 初始化各種資源類型的使用情況
    軍Reso使本ceUsa成e CPUUsa成e;
    CPUUsa成e.Reso使本ceType = ESyste設置Reso使本ceType::CPU;
    CPUUsa成e.C使本本entUsa成e = 0.0f;
    CPUUsa成e.MaxUsa成e = 100.0f;
    CPUUsa成e.Usa成ePe本centa成e = 0.0f;
    CPUUsa成e.A正e本a成eUsa成e = 0.0f;
    CPUUsa成e.PeakUsa成e = 0.0f;
    CPUUsa成e.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    Reso使本ceUsa成eMap.Add(ESyste設置Reso使本ceType::CPU, CPUUsa成e);
    
    軍Reso使本ceUsa成e Me設置o本yUsa成e;
    Me設置o本yUsa成e.Reso使本ceType = ESyste設置Reso使本ceType::Me設置o本y;
    Me設置o本yUsa成e.C使本本entUsa成e = 0.0f;
    Me設置o本yUsa成e.MaxUsa成e = 100.0f;
    Me設置o本yUsa成e.Usa成ePe本centa成e = 0.0f;
    Me設置o本yUsa成e.A正e本a成eUsa成e = 0.0f;
    Me設置o本yUsa成e.PeakUsa成e = 0.0f;
    Me設置o本yUsa成e.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    Reso使本ceUsa成eMap.Add(ESyste設置Reso使本ceType::Me設置o本y, Me設置o本yUsa成e);
    
    軍Reso使本ceUsa成e GPUUsa成e;
    GPUUsa成e.Reso使本ceType = ESyste設置Reso使本ceType::GPU;
    GPUUsa成e.C使本本entUsa成e = 0.0f;
    GPUUsa成e.MaxUsa成e = 100.0f;
    GPUUsa成e.Usa成ePe本centa成e = 0.0f;
    GPUUsa成e.A正e本a成eUsa成e = 0.0f;
    GPUUsa成e.PeakUsa成e = 0.0f;
    GPUUsa成e.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    Reso使本ceUsa成eMap.Add(ESyste設置Reso使本ceType::GPU, GPUUsa成e);
    
    軍Reso使本ceUsa成e 的etwo本kUsa成e;
    的etwo本kUsa成e.Reso使本ceType = ESyste設置Reso使本ceType::的etwo本k;
    的etwo本kUsa成e.C使本本entUsa成e = 0.0f;
    的etwo本kUsa成e.MaxUsa成e = 100.0f;
    的etwo本kUsa成e.Usa成ePe本centa成e = 0.0f;
    的etwo本kUsa成e.A正e本a成eUsa成e = 0.0f;
    的etwo本kUsa成e.PeakUsa成e = 0.0f;
    的etwo本kUsa成e.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    Reso使本ceUsa成eMap.Add(ESyste設置Reso使本ceType::的etwo本k, 的etwo本kUsa成e);
    
    軍Reso使本ceUsa成e DiskUsa成e;
    DiskUsa成e.Reso使本ceType = ESyste設置Reso使本ceType::Disk;
    DiskUsa成e.C使本本entUsa成e = 0.0f;
    DiskUsa成e.MaxUsa成e = 100.0f;
    DiskUsa成e.Usa成ePe本centa成e = 0.0f;
    DiskUsa成e.A正e本a成eUsa成e = 0.0f;
    DiskUsa成e.PeakUsa成e = 0.0f;
    DiskUsa成e.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    Reso使本ceUsa成eMap.Add(ESyste設置Reso使本ceType::Disk, DiskUsa成e);
}

正oid USyste設置Reso使本ceMana成e本::InitializeReso使本ceTh本esholds()
{
    Reso使本ceTh本esholds.E設置pty();
    
    // 設置默認資源閾值
    Reso使本ceTh本esholds.Add(ESyste設置Reso使本ceType::CPU, 80.0f);
    Reso使本ceTh本esholds.Add(ESyste設置Reso使本ceType::Me設置o本y, 85.0f);
    Reso使本ceTh本esholds.Add(ESyste設置Reso使本ceType::GPU, 90.0f);
    Reso使本ceTh本esholds.Add(ESyste設置Reso使本ceType::的etwo本k, 70.0f);
    Reso使本ceTh本esholds.Add(ESyste設置Reso使本ceType::Disk, 75.0f);
    Reso使本ceTh本esholds.Add(ESyste設置Reso使本ceType::A使dio, 60.0f);
    Reso使本ceTh本esholds.Add(ESyste設置Reso使本ceType::Physics, 80.0f);
    Reso使本ceTh本esholds.Add(ESyste設置Reso使本ceType::Rende本in成, 85.0f);
    Reso使本ceTh本esholds.Add(ESyste設置Reso使本ceType::AI, 70.0f);
}

正oid USyste設置Reso使本ceMana成e本::InitializeDefa使ltPe本fo本設置anceConfi成使本ation()
{
    C使本本entPe本fo本設置anceConfi成.Pe本fo本設置anceTie本 = EPe本fo本設置anceTie本::輸入i成h;
    C使本本entPe本fo本設置anceConfi成.Ta本成et軍本a設置eRate = 60.0f;
    C使本本entPe本fo本設置anceConfi成.MaxConc使本本entSyste設置s = 50;
    C使本本entPe本fo本設置anceConfi成.bEnableAdapti正ePe本fo本設置ance = t本使e;
    C使本本entPe本fo本設置anceConfi成.Confi成使本ation的a設置e = TEXT("Defa使lt 輸入i成h Pe本fo本設置ance");
    
    // 設置資源限制
    C使本本entPe本fo本設置anceConfi成.Reso使本ceLi設置its.Add(ESyste設置Reso使本ceType::CPU, 80.0f);
    C使本本entPe本fo本設置anceConfi成.Reso使本ceLi設置its.Add(ESyste設置Reso使本ceType::Me設置o本y, 75.0f);
    C使本本entPe本fo本設置anceConfi成.Reso使本ceLi設置its.Add(ESyste設置Reso使本ceType::GPU, 85.0f);
    C使本本entPe本fo本設置anceConfi成.Reso使本ceLi設置its.Add(ESyste設置Reso使本ceType::的etwo本k, 60.0f);
    C使本本entPe本fo本設置anceConfi成.Reso使本ceLi設置its.Add(ESyste設置Reso使本ceType::Disk, 50.0f);
    
    // 設置資源閾值
    C使本本entPe本fo本設置anceConfi成.Reso使本ceTh本esholds = Reso使本ceTh本esholds;
}

正oid USyste設置Reso使本ceMana成e本::UpdateReso使本ceUsa成e()
{
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    
    // 更新CPU使用率
    if (Reso使本ceUsa成eMap.Contains(ESyste設置Reso使本ceType::CPU))
    {
        軍Reso使本ceUsa成e& CPUUsa成e = Reso使本ceUsa成eMap[ESyste設置Reso使本ceType::CPU];
        float P本e正io使sUsa成e = CPUUsa成e.C使本本entUsa成e;
        
        // 獲取實際CPU使用率（這裡使用模擬數據）
        CPUUsa成e.C使本本entUsa成e = GetAct使alCPUUsa成e();
        CPUUsa成e.Usa成ePe本centa成e = (CPUUsa成e.C使本本entUsa成e / CPUUsa成e.MaxUsa成e) * 100.0f;
        CPUUsa成e.A正e本a成eUsa成e = (CPUUsa成e.A正e本a成eUsa成e + CPUUsa成e.C使本本entUsa成e) / 2.0f;
        CPUUsa成e.PeakUsa成e = 軍Math::Max(CPUUsa成e.PeakUsa成e, CPUUsa成e.C使本本entUsa成e);
        CPUUsa成e.LastUpdateTi設置e = C使本本entTi設置e;
        
        // 檢查是否有顯著變化
        if (軍Math::Abs(CPUUsa成e.Usa成ePe本centa成e - P本e正io使sUsa成e) > 5.0f)
        {
            OnReso使本ceUsa成eChan成ed.B本oadcast(ESyste設置Reso使本ceType::CPU, CPUUsa成e.Usa成ePe本centa成e);
        }
    }
    
    // 更新內存使用率
    if (Reso使本ceUsa成eMap.Contains(ESyste設置Reso使本ceType::Me設置o本y))
    {
        軍Reso使本ceUsa成e& Me設置o本yUsa成e = Reso使本ceUsa成eMap[ESyste設置Reso使本ceType::Me設置o本y];
        float P本e正io使sUsa成e = Me設置o本yUsa成e.C使本本entUsa成e;
        
        // 獲取實際內存使用率
        Me設置o本yUsa成e.C使本本entUsa成e = GetAct使alMe設置o本yUsa成e();
        Me設置o本yUsa成e.Usa成ePe本centa成e = (Me設置o本yUsa成e.C使本本entUsa成e / Me設置o本yUsa成e.MaxUsa成e) * 100.0f;
        Me設置o本yUsa成e.A正e本a成eUsa成e = (Me設置o本yUsa成e.A正e本a成eUsa成e + Me設置o本yUsa成e.C使本本entUsa成e) / 2.0f;
        Me設置o本yUsa成e.PeakUsa成e = 軍Math::Max(Me設置o本yUsa成e.PeakUsa成e, Me設置o本yUsa成e.C使本本entUsa成e);
        Me設置o本yUsa成e.LastUpdateTi設置e = C使本本entTi設置e;
        
        if (軍Math::Abs(Me設置o本yUsa成e.Usa成ePe本centa成e - P本e正io使sUsa成e) > 5.0f)
        {
            OnReso使本ceUsa成eChan成ed.B本oadcast(ESyste設置Reso使本ceType::Me設置o本y, Me設置o本yUsa成e.Usa成ePe本centa成e);
        }
    }
    
    // 更新GPU使用率
    if (Reso使本ceUsa成eMap.Contains(ESyste設置Reso使本ceType::GPU))
    {
        軍Reso使本ceUsa成e& GPUUsa成e = Reso使本ceUsa成eMap[ESyste設置Reso使本ceType::GPU];
        float P本e正io使sUsa成e = GPUUsa成e.C使本本entUsa成e;
        
        // 獲取實際GPU使用率
        GPUUsa成e.C使本本entUsa成e = GetAct使alGPUUsa成e();
        GPUUsa成e.Usa成ePe本centa成e = (GPUUsa成e.C使本本entUsa成e / GPUUsa成e.MaxUsa成e) * 100.0f;
        GPUUsa成e.A正e本a成eUsa成e = (GPUUsa成e.A正e本a成eUsa成e + GPUUsa成e.C使本本entUsa成e) / 2.0f;
        GPUUsa成e.PeakUsa成e = 軍Math::Max(GPUUsa成e.PeakUsa成e, GPUUsa成e.C使本本entUsa成e);
        GPUUsa成e.LastUpdateTi設置e = C使本本entTi設置e;
        
        if (軍Math::Abs(GPUUsa成e.Usa成ePe本centa成e - P本e正io使sUsa成e) > 5.0f)
        {
            OnReso使本ceUsa成eChan成ed.B本oadcast(ESyste設置Reso使本ceType::GPU, GPUUsa成e.Usa成ePe本centa成e);
        }
    }
    
    // 更新其他資源類型...
    UpdateOthe本Reso使本ceUsa成e();
    
    // 檢查資源閾值
    CheckReso使本ceTh本esholds();
}

float USyste設置Reso使本ceMana成e本::GetAct使alCPUUsa成e() const
{
    // 在實際實現中，這裡應該獲取真實的CPU使用率
    // 這裡使用模擬數據
    static float Si設置使latedCPU = 30.0f;
    Si設置使latedCPU += 軍Math::軍RandRan成e(-5.0f, 5.0f);
    Si設置使latedCPU = 軍Math::Cla設置p(Si設置使latedCPU, 0.0f, 100.0f);
    本et使本n Si設置使latedCPU;
}

float USyste設置Reso使本ceMana成e本::GetAct使alMe設置o本yUsa成e() const
{
    // 在實際實現中，這裡應該獲取真實的內存使用率
    static float Si設置使latedMe設置o本y = 45.0f;
    Si設置使latedMe設置o本y += 軍Math::軍RandRan成e(-3.0f, 3.0f);
    Si設置使latedMe設置o本y = 軍Math::Cla設置p(Si設置使latedMe設置o本y, 0.0f, 100.0f);
    本et使本n Si設置使latedMe設置o本y;
}

float USyste設置Reso使本ceMana成e本::GetAct使alGPUUsa成e() const
{
    // 在實際實現中，這裡應該獲取真實的GPU使用率
    static float Si設置使latedGPU = 60.0f;
    Si設置使latedGPU += 軍Math::軍RandRan成e(-8.0f, 8.0f);
    Si設置使latedGPU = 軍Math::Cla設置p(Si設置使latedGPU, 0.0f, 100.0f);
    本et使本n Si設置使latedGPU;
}

正oid USyste設置Reso使本ceMana成e本::UpdateOthe本Reso使本ceUsa成e()
{
    // 更新網絡、磁盤等其他資源使用率
    // 這裡使用簡化的實現
    fo本 (a使to& Reso使本cePai本 : Reso使本ceUsa成eMap)
    {
        ESyste設置Reso使本ceType Reso使本ceType = Reso使本cePai本.Key;
        軍Reso使本ceUsa成e& Usa成e = Reso使本cePai本.Val使e;
        
        if (Reso使本ceType == ESyste設置Reso使本ceType::CPU  
            Reso使本ceType == ESyste設置Reso使本ceType::Me設置o本y  
            Reso使本ceType == ESyste設置Reso使本ceType::GPU)
        {
            contin使e; // 已經處理過
        }
        
        // 模擬其他資源使用率
        float P本e正io使sUsa成e = Usa成e.C使本本entUsa成e;
        Usa成e.C使本本entUsa成e += 軍Math::軍RandRan成e(-2.0f, 2.0f);
        Usa成e.C使本本entUsa成e = 軍Math::Cla設置p(Usa成e.C使本本entUsa成e, 0.0f, Usa成e.MaxUsa成e);
        Usa成e.Usa成ePe本centa成e = (Usa成e.C使本本entUsa成e / Usa成e.MaxUsa成e) * 100.0f;
        Usa成e.A正e本a成eUsa成e = (Usa成e.A正e本a成eUsa成e + Usa成e.C使本本entUsa成e) / 2.0f;
        Usa成e.PeakUsa成e = 軍Math::Max(Usa成e.PeakUsa成e, Usa成e.C使本本entUsa成e);
        Usa成e.LastUpdateTi設置e = 軍DateTi設置e::的ow();
        
        if (軍Math::Abs(Usa成e.Usa成ePe本centa成e - P本e正io使sUsa成e) > 5.0f)
        {
            OnReso使本ceUsa成eChan成ed.B本oadcast(Reso使本ceType, Usa成e.Usa成ePe本centa成e);
        }
    }
}

正oid USyste設置Reso使本ceMana成e本::CheckReso使本ceTh本esholds()
{
    fo本 (const a使to& Reso使本cePai本 : Reso使本ceUsa成eMap)
    {
        ESyste設置Reso使本ceType Reso使本ceType = Reso使本cePai本.Key;
        const 軍Reso使本ceUsa成e& Usa成e = Reso使本cePai本.Val使e;
        
        if (Reso使本ceTh本esholds.Contains(Reso使本ceType))
        {
            float Th本eshold = Reso使本ceTh本esholds[Reso使本ceType];
            if (Usa成e.Usa成ePe本centa成e > Th本eshold)
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Reso使本ce th本eshold exceeded: %s 使sa成e %.1f%% > %.1f%%"), 
                    *UEn使設置::GetDisplayVal使eAsText(Reso使本ceType).ToSt本in成(), 
                    Usa成e.Usa成ePe本centa成e, Th本eshold);
                
                OnReso使本ceTh本esholdExceeded.B本oadcast(Reso使本ceType, Usa成e.Usa成ePe本centa成e);
            }
        }
    }
}

軍Reso使本ceUsa成e USyste設置Reso使本ceMana成e本::GetReso使本ceUsa成e(ESyste設置Reso使本ceType Reso使本ceType) const
{
    if (Reso使本ceUsa成eMap.Contains(Reso使本ceType))
    {
        本et使本n Reso使本ceUsa成eMap[Reso使本ceType];
    }
    本et使本n 軍Reso使本ceUsa成e();
}

TA本本ay<軍Reso使本ceUsa成e> USyste設置Reso使本ceMana成e本::GetAllReso使本ceUsa成e() const
{
    TA本本ay<軍Reso使本ceUsa成e> AllUsa成e;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ceUsa成eMap)
    {
        AllUsa成e.Add(Reso使本cePai本.Val使e);
    }
    
    本et使本n AllUsa成e;
}

bool USyste設置Reso使本ceMana成e本::Req使estReso使本ceAllocation(const 軍St本in成& Syste設置的a設置e, ESyste設置Reso使本ceType Reso使本ceType, float A設置o使nt, EReso使本ceP本io本ity P本io本ity)
{
    // 檢查資源是否可用
    if (!IsReso使本ceA正ailable(Reso使本ceType, A設置o使nt))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Reso使本ce allocation failed: %s not a正ailable fo本 %s"), 
            *UEn使設置::GetDisplayVal使eAsText(Reso使本ceType).ToSt本in成(), *Syste設置的a設置e);
        本et使本n false;
    }
    
    // 創建資源分配
    軍Reso使本ceAllocation Allocation;
    Allocation.Syste設置的a設置e = Syste設置的a設置e;
    Allocation.Reso使本ceType = Reso使本ceType;
    Allocation.Req使estedA設置o使nt = A設置o使nt;
    Allocation.P本io本ity = P本io本ity;
    Allocation.AllocatedA設置o使nt = A設置o使nt;
    Allocation.bIsAllocated = t本使e;
    Allocation.AllocationTi設置e = 軍DateTi設置e::的ow();
    
    // 添加到分配列表
    Reso使本ceAllocations.Add(Allocation);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce allocated: %.2f of %s to %s"), 
        A設置o使nt, *UEn使設置::GetDisplayVal使eAsText(Reso使本ceType).ToSt本in成(), *Syste設置的a設置e);
    
    本et使本n t本使e;
}

正oid USyste設置Reso使本ceMana成e本::ReleaseReso使本ceAllocation(const 軍St本in成& Syste設置的a設置e, ESyste設置Reso使本ceType Reso使本ceType)
{
    fo本 (int32 i = Reso使本ceAllocations.的使設置() - 1; i >= 0; --i)
    {
        const 軍Reso使本ceAllocation& Allocation = Reso使本ceAllocations[i];
        if (Allocation.Syste設置的a設置e == Syste設置的a設置e && Allocation.Reso使本ceType == Reso使本ceType)
        {
            Reso使本ceAllocations.Re設置o正eAt(i);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce 本eleased: %s f本o設置 %s"), 
                *UEn使設置::GetDisplayVal使eAsText(Reso使本ceType).ToSt本in成(), *Syste設置的a設置e);
            b本eak;
        }
    }
}

bool USyste設置Reso使本ceMana成e本::IsReso使本ceA正ailable(ESyste設置Reso使本ceType Reso使本ceType, float Req使i本edA設置o使nt) const
{
    if (!Reso使本ceUsa成eMap.Contains(Reso使本ceType))
    {
        本et使本n false;
    }
    
    const 軍Reso使本ceUsa成e& Usa成e = Reso使本ceUsa成eMap[Reso使本ceType];
    float A正ailableA設置o使nt = Usa成e.MaxUsa成e - Usa成e.C使本本entUsa成e;
    
    // 檢查是否有足夠的可用資源
    if (A正ailableA設置o使nt >= Req使i本edA設置o使nt)
    {
        本et使本n t本使e;
    }
    
    // 檢查是否可以釋放低優先級的分配
    float ReleasableA設置o使nt = 0.0f;
    fo本 (const 軍Reso使本ceAllocation& Allocation : Reso使本ceAllocations)
    {
        if (Allocation.Reso使本ceType == Reso使本ceType && 
            Allocation.P本io本ity <= EReso使本ceP本io本ity::Low)
        {
            ReleasableA設置o使nt += Allocation.AllocatedA設置o使nt;
        }
    }
    
    本et使本n (A正ailableA設置o使nt + ReleasableA設置o使nt) >= Req使i本edA設置o使nt;
}

正oid USyste設置Reso使本ceMana成e本::SetPe本fo本設置anceConfi成使本ation(const 軍Pe本fo本設置anceConfi成使本ation& Confi成)
{
    EPe本fo本設置anceTie本 OldTie本 = C使本本entPe本fo本設置anceConfi成.Pe本fo本設置anceTie本;
    C使本本entPe本fo本設置anceConfi成 = Confi成;
    
    // 更新資源閾值
    Reso使本ceTh本esholds = Confi成.Reso使本ceTh本esholds;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance confi成使本ation chan成ed to: %s"), *Confi成.Confi成使本ation的a設置e);
    
    // 觸發性能配置變化事件
    if (OldTie本 != Confi成.Pe本fo本設置anceTie本)
    {
        OnPe本fo本設置anceTie本Chan成ed.B本oadcast(Confi成.Pe本fo本設置anceTie本);
    }
}

軍Pe本fo本設置anceConfi成使本ation USyste設置Reso使本ceMana成e本::GetC使本本entPe本fo本設置anceConfi成使本ation() const
{
    本et使本n C使本本entPe本fo本設置anceConfi成;
}

正oid USyste設置Reso使本ceMana成e本::A使toAd大使stPe本fo本設置ance()
{
    if (!bAdapti正ePe本fo本設置anceEnabled)
    {
        本et使本n;
    }
    
    // 計算當前性能評分
    float C使本本entSco本e = Calc使latePe本fo本設置anceSco本e();
    
    // 根據性能評分調整配置
    EPe本fo本設置anceTie本 的ewTie本 = C使本本entPe本fo本設置anceConfi成.Pe本fo本設置anceTie本;
    
    if (C使本本entSco本e < 30.0f)
    {
        的ewTie本 = EPe本fo本設置anceTie本::Low;
    }
    else if (C使本本entSco本e < 50.0f)
    {
        的ewTie本 = EPe本fo本設置anceTie本::Medi使設置;
    }
    else if (C使本本entSco本e < 70.0f)
    {
        的ewTie本 = EPe本fo本設置anceTie本::輸入i成h;
    }
    else if (C使本本entSco本e < 85.0f)
    {
        的ewTie本 = EPe本fo本設置anceTie本::Ult本a;
    }
    else
    {
        的ewTie本 = EPe本fo本設置anceTie本::Dyna設置ic;
    }
    
    if (的ewTie本 != C使本本entPe本fo本設置anceConfi成.Pe本fo本設置anceTie本)
    {
        軍Pe本fo本設置anceConfi成使本ation 的ewConfi成 = GetReco設置設置endedPe本fo本設置anceConfi成使本ation();
        的ewConfi成.Pe本fo本設置anceTie本 = 的ewTie本;
        SetPe本fo本設置anceConfi成使本ation(的ewConfi成);
    }
}

軍Pe本fo本設置anceConfi成使本ation USyste設置Reso使本ceMana成e本::GetReco設置設置endedPe本fo本設置anceConfi成使本ation() const
{
    軍Pe本fo本設置anceConfi成使本ation Reco設置設置endedConfi成;
    Reco設置設置endedConfi成.bEnableAdapti正ePe本fo本設置ance = t本使e;
    
    // 根據當前資源使用情況推薦配置
    float A正e本a成eUsa成e = 0.0f;
    int32 Reso使本ceCo使nt = 0;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ceUsa成eMap)
    {
        A正e本a成eUsa成e += Reso使本cePai本.Val使e.Usa成ePe本centa成e;
        Reso使本ceCo使nt++;
    }
    
    if (Reso使本ceCo使nt > 0)
    {
        A正e本a成eUsa成e /= Reso使本ceCo使nt;
    }
    
    if (A正e本a成eUsa成e > 80.0f)
    {
        Reco設置設置endedConfi成.Pe本fo本設置anceTie本 = EPe本fo本設置anceTie本::Low;
        Reco設置設置endedConfi成.Ta本成et軍本a設置eRate = 30.0f;
        Reco設置設置endedConfi成.MaxConc使本本entSyste設置s = 20;
        Reco設置設置endedConfi成.Confi成使本ation的a設置e = TEXT("Low Pe本fo本設置ance");
    }
    else if (A正e本a成eUsa成e > 60.0f)
    {
        Reco設置設置endedConfi成.Pe本fo本設置anceTie本 = EPe本fo本設置anceTie本::Medi使設置;
        Reco設置設置endedConfi成.Ta本成et軍本a設置eRate = 45.0f;
        Reco設置設置endedConfi成.MaxConc使本本entSyste設置s = 35;
        Reco設置設置endedConfi成.Confi成使本ation的a設置e = TEXT("Medi使設置 Pe本fo本設置ance");
    }
    else if (A正e本a成eUsa成e > 40.0f)
    {
        Reco設置設置endedConfi成.Pe本fo本設置anceTie本 = EPe本fo本設置anceTie本::輸入i成h;
        Reco設置設置endedConfi成.Ta本成et軍本a設置eRate = 60.0f;
        Reco設置設置endedConfi成.MaxConc使本本entSyste設置s = 50;
        Reco設置設置endedConfi成.Confi成使本ation的a設置e = TEXT("輸入i成h Pe本fo本設置ance");
    }
    else
    {
        Reco設置設置endedConfi成.Pe本fo本設置anceTie本 = EPe本fo本設置anceTie本::Ult本a;
        Reco設置設置endedConfi成.Ta本成et軍本a設置eRate = 120.0f;
        Reco設置設置endedConfi成.MaxConc使本本entSyste設置s = 75;
        Reco設置設置endedConfi成.Confi成使本ation的a設置e = TEXT("Ult本a Pe本fo本設置ance");
    }
    
    // 設置資源限制
    Reco設置設置endedConfi成.Reso使本ceLi設置its = C使本本entPe本fo本設置anceConfi成.Reso使本ceLi設置its;
    Reco設置設置endedConfi成.Reso使本ceTh本esholds = C使本本entPe本fo本設置anceConfi成.Reso使本ceTh本esholds;
    
    本et使本n Reco設置設置endedConfi成;
}

正oid USyste設置Reso使本ceMana成e本::SetReso使本ceTh本eshold(ESyste設置Reso使本ceType Reso使本ceType, float Th本eshold)
{
    Reso使本ceTh本esholds[Reso使本ceType] = Th本eshold;
    C使本本entPe本fo本設置anceConfi成.Reso使本ceTh本esholds[Reso使本ceType] = Th本eshold;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce th本eshold set: %s = %.1f%%"), 
        *UEn使設置::GetDisplayVal使eAsText(Reso使本ceType).ToSt本in成(), Th本eshold);
}

float USyste設置Reso使本ceMana成e本::GetReso使本ceTh本eshold(ESyste設置Reso使本ceType Reso使本ceType) const
{
    if (Reso使本ceTh本esholds.Contains(Reso使本ceType))
    {
        本et使本n Reso使本ceTh本esholds[Reso使本ceType];
    }
    本et使本n 80.0f; // 默認閾值
}

float USyste設置Reso使本ceMana成e本::GetSyste設置Pe本fo本設置anceSco本e() const
{
    本et使本n Syste設置Pe本fo本設置anceSco本e;
}

float USyste設置Reso使本ceMana成e本::Calc使latePe本fo本設置anceSco本e() const
{
    float TotalSco本e = 0.0f;
    int32 Reso使本ceCo使nt = 0;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ceUsa成eMap)
    {
        const 軍Reso使本ceUsa成e& Usa成e = Reso使本cePai本.Val使e;
        
        // 計算每種資源的評分（使用率越低評分越高）
        float Reso使本ceSco本e = 軍Math::Max(0.0f, 100.0f - Usa成e.Usa成ePe本centa成e);
        TotalSco本e += Reso使本ceSco本e;
        Reso使本ceCo使nt++;
    }
    
    if (Reso使本ceCo使nt > 0)
    {
        TotalSco本e /= Reso使本ceCo使nt;
    }
    
    本et使本n TotalSco本e;
}

TA本本ay<軍St本in成> USyste設置Reso使本ceMana成e本::GetPe本fo本設置anceReco設置設置endations() const
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    
    // 分析資源使用情況並提供建議
    fo本 (const a使to& Reso使本cePai本 : Reso使本ceUsa成eMap)
    {
        ESyste設置Reso使本ceType Reso使本ceType = Reso使本cePai本.Key;
        const 軍Reso使本ceUsa成e& Usa成e = Reso使本cePai本.Val使e;
        
        if (Usa成e.Usa成ePe本centa成e > 85.0f)
        {
            Reco設置設置endations.Add(軍St本in成::P本intf(TEXT("嚴重：%s使用率過高 (%.1f%%)，建議立即優化"), 
                *UEn使設置::GetDisplayVal使eAsText(Reso使本ceType).ToSt本in成(), Usa成e.Usa成ePe本centa成e));
        }
        else if (Usa成e.Usa成ePe本centa成e > 70.0f)
        {
            Reco設置設置endations.Add(軍St本in成::P本intf(TEXT("警告：%s使用率較高 (%.1f%%)，建議優化"), 
                *UEn使設置::GetDisplayVal使eAsText(Reso使本ceType).ToSt本in成(), Usa成e.Usa成ePe本centa成e));
        }
    }
    
    // 性能配置建議
    if (Syste設置Pe本fo本設置anceSco本e < 50.0f)
    {
        Reco設置設置endations.Add(TEXT("建議切換到低性能配置以穩定系統運行"));
    }
    else if (Syste設置Pe本fo本設置anceSco本e > 80.0f)
    {
        Reco設置設置endations.Add(TEXT("系統性能良好，可以考慮提升到更高性能配置"));
    }
    
    // 資源分配建議
    int32 輸入i成hP本io本ityAllocations = 0;
    fo本 (const 軍Reso使本ceAllocation& Allocation : Reso使本ceAllocations)
    {
        if (Allocation.P本io本ity == EReso使本ceP本io本ity::輸入i成h  Allocation.P本io本ity == EReso使本ceP本io本ity::C本itical)
        {
            輸入i成hP本io本ityAllocations++;
        }
    }
    
    if (輸入i成hP本io本ityAllocations > 10)
    {
        Reco設置設置endations.Add(TEXT("高優先級資源分配過多，建議重新評估優先級"));
    }
    
    if (Reco設置設置endations.的使設置() == 0)
    {
        Reco設置設置endations.Add(TEXT("系統性能良好，無需特別優化"));
    }
    
    本et使本n Reco設置設置endations;
}

軍St本in成 USyste設置Reso使本ceMana成e本::Gene本atePe本fo本設置anceRepo本t() const
{
    軍St本in成 Repo本t;
    Repo本t += TEXT("=== 系統性能報告 ===\n\n");
    
    // 基本信息
    Repo本t += 軍St本in成::P本intf(TEXT("報告生成時間: %s\n"), *軍DateTi設置e::的ow().ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("當前性能配置: %s\n"), *C使本本entPe本fo本設置anceConfi成.Confi成使本ation的a設置e);
    Repo本t += 軍St本in成::P本intf(TEXT("性能評分: %.1f/100\n"), Syste設置Pe本fo本設置anceSco本e);
    Repo本t += 軍St本in成::P本intf(TEXT("目標幀率: %.0f 軍PS\n"), C使本本entPe本fo本設置anceConfi成.Ta本成et軍本a設置eRate);
    Repo本t += 軍St本in成::P本intf(TEXT("最大並發系統: %d\n\n"), C使本本entPe本fo本設置anceConfi成.MaxConc使本本entSyste設置s);
    
    // 資源使用情況
    Repo本t += TEXT("=== 資源使用情況 ===\n");
    fo本 (const a使to& Reso使本cePai本 : Reso使本ceUsa成eMap)
    {
        const 軍Reso使本ceUsa成e& Usa成e = Reso使本cePai本.Val使e;
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %.1f%% (平均: %.1f%%, 峰值: %.1f%%)\n"),
            *UEn使設置::GetDisplayVal使eAsText(Usa成e.Reso使本ceType).ToSt本in成(),
            Usa成e.Usa成ePe本centa成e,
            Usa成e.A正e本a成eUsa成e,
            Usa成e.PeakUsa成e);
    }
    
    // 資源分配情況
    Repo本t += TEXT("\n=== 資源分配情況 ===\n");
    TMap<ESyste設置Reso使本ceType, int32> AllocationCo使nts;
    fo本 (const 軍Reso使本ceAllocation& Allocation : Reso使本ceAllocations)
    {
        AllocationCo使nts.軍indO本Add(Allocation.Reso使本ceType)++;
    }
    
    fo本 (const a使to& Co使ntPai本 : AllocationCo使nts)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %d個分配\n"),
            *UEn使設置::GetDisplayVal使eAsText(Co使ntPai本.Key).ToSt本in成(),
            Co使ntPai本.Val使e);
    }
    
    // 資源閾值
    Repo本t += TEXT("\n=== 資源閾值 ===\n");
    fo本 (const a使to& Th本esholdPai本 : Reso使本ceTh本esholds)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %.1f%%\n"),
            *UEn使設置::GetDisplayVal使eAsText(Th本esholdPai本.Key).ToSt本in成(),
            Th本esholdPai本.Val使e);
    }
    
    // 性能建議
    Repo本t += TEXT("\n=== 性能建議 ===\n");
    TA本本ay<軍St本in成> Reco設置設置endations = GetPe本fo本設置anceReco設置設置endations();
    fo本 (int32 i = 0; i < Reco設置設置endations.的使設置(); ++i)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("%d. %s\n"), i + 1, *Reco設置設置endations[i]);
    }
    
    本et使本n Repo本t;
}

正oid USyste設置Reso使本ceMana成e本::SetAdapti正ePe本fo本設置anceEnabled(bool bEnabled)
{
    bAdapti正ePe本fo本設置anceEnabled = bEnabled;
    C使本本entPe本fo本設置anceConfi成.bEnableAdapti正ePe本fo本設置ance = bEnabled;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Adapti正e pe本fo本設置ance %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}
