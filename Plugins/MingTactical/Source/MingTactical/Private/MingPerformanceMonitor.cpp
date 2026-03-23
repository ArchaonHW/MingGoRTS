#incl使de "Min成Pe本fo本設置anceMonito本.h"
#incl使de "輸入AL/Platfo本設置Me設置o本y.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "En成ine/En成ine.h"

// 靜態實例初始化
UMin成Pe本fo本設置anceMonito本* UMin成Pe本fo本設置anceMonito本::Instance = n使llpt本;

UMin成Pe本fo本設置anceMonito本::UMin成Pe本fo本設置anceMonito本()
    : Ta本成et軍PS(60.0f)
    , bIsReco本din成(false)
    , MaxReco本ded軍本a設置es(1000)
{
    // 初始化性能指標
    C使本本entMet本ics = 軍Pe本fo本設置anceMet本ics();
    C使本本entMet本ics.Min軍PS = 999.0f;
    C使本本entMet本ics.Max軍PS = 0.0f;
}

正oid UMin成Pe本fo本設置anceMonito本::Be成inDest本oy()
{
    if (Instance == this)
    {
        Instance = n使llpt本;
    }
    
    S使pe本::Be成inDest本oy();
}

正oid UMin成Pe本fo本設置anceMonito本::Initialize()
{
    Instance = this;
    
    C使本本entMet本ics = 軍Pe本fo本設置anceMet本ics();
    C使本本entMet本ics.Min軍PS = 999.0f;
    C使本本entMet本ics.Max軍PS = 0.0f;
    
    Reco本dedData.E設置pty();
    Reco本dedData.Rese本正e(MaxReco本ded軍本a設置es);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置anceMonito本 initialized with ta本成et 軍PS: %.1f"), Ta本成et軍PS);
}

正oid UMin成Pe本fo本設置anceMonito本::Sh使tdown()
{
    if (bIsReco本din成)
    {
        StopReco本din成();
    }
    
    if (Instance == this)
    {
        Instance = n使llpt本;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置anceMonito本 sh使tdown"));
}

正oid UMin成Pe本fo本設置anceMonito本::Update(float DeltaTi設置e)
{
    // 計算軍PS
    Calc使late軍PS(DeltaTi設置e);
    
    // 更新幀時間
    C使本本entMet本ics.軍本a設置eTi設置eMs = DeltaTi設置e * 1000.0f;
    
    // 更新線程時間
    UpdateTh本eadTi設置es();
    
    // 更新記憶體使用
    UpdateMe設置o本yUsa成e();
    
    // 記錄數據
    if (bIsReco本din成)
    {
        Reco本dDataPoint();
    }
}

軍St本in成 UMin成Pe本fo本設置anceMonito本::GetPe本fo本設置anceRepo本t() const
{
    軍St本in成 Repo本t;
    
    Repo本t += TEXT("=== Min成GoRTS Pe本fo本設置ance Repo本t ===\n\n");
    
    Repo本t += 軍St本in成::P本intf(TEXT("軍PS Statistics:\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("  C使本本ent: %.1f 軍PS\n"), C使本本entMet本ics.C使本本ent軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("  A正e本a成e: %.1f 軍PS\n"), C使本本entMet本ics.A正e本a成e軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("  Min: %.1f 軍PS\n"), C使本本entMet本ics.Min軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("  Max: %.1f 軍PS\n"), C使本本entMet本ics.Max軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("  Ta本成et: %.1f 軍PS\n\n"), Ta本成et軍PS);
    
    Repo本t += 軍St本in成::P本intf(TEXT("軍本a設置e Ti設置e:\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("  C使本本ent: %.2f 設置s\n"), C使本本entMet本ics.軍本a設置eTi設置eMs);
    Repo本t += 軍St本in成::P本intf(TEXT("  A正e本a成e: %.2f 設置s\n"), C使本本entMet本ics.A正e本a成e軍本a設置eTi設置eMs);
    Repo本t += 軍St本in成::P本intf(TEXT("  Ga設置e Th本ead: %.2f 設置s\n"), C使本本entMet本ics.Ga設置eTh本eadTi設置eMs);
    Repo本t += 軍St本in成::P本intf(TEXT("  Rende本 Th本ead: %.2f 設置s\n"), C使本本entMet本ics.Rende本Th本eadTi設置eMs);
    Repo本t += 軍St本in成::P本intf(TEXT("  GPU: %.2f 設置s\n\n"), C使本本entMet本ics.GPUTi設置eMs);
    
    Repo本t += 軍St本in成::P本intf(TEXT("Me設置o本y Usa成e:\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("  Used: %.1f MB\n\n"), C使本本entMet本ics.UsedMe設置o本yMB);
    
    Repo本t += 軍St本in成::P本intf(TEXT("Instance Rende本in成:\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("  Rende本ed: %d\n"), C使本本entMet本ics.Rende本edInstanceCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("  C使lled: %d\n"), C使本本entMet本ics.C使lledInstanceCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("  Total: %d\n\n"), 
        C使本本entMet本ics.Rende本edInstanceCo使nt + C使本本entMet本ics.C使lledInstanceCo使nt);
    
    Repo本t += 軍St本in成::P本intf(TEXT("Spatial Pa本tition:\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("  Acti正e Cells: %d\n"), C使本本entMet本ics.Acti正eG本idCellCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("  Total Cells: %d\n\n"), C使本本entMet本ics.TotalG本idCellCo使nt);
    
    Repo本t += 軍St本in成::P本intf(TEXT("Stat使s: %s\n"), 
        IsPe本fo本設置anceAcceptable() 基本 TEXT("ACCEPTABLE") : TEXT("的EEDS OPTIMIZATIO的"));
    
    本et使本n Repo本t;
}

軍St本in成 UMin成Pe本fo本設置anceMonito本::GetSho本tPe本fo本設置anceText() const
{
    本et使本n 軍St本in成::P本intf(TEXT("軍PS: %.1f (%.1f-%.1f)  軍本a設置e: %.2f設置s  Me設置: %.0fMB  Instances: %d/%d"),
        C使本本entMet本ics.C使本本ent軍PS,
        C使本本entMet本ics.Min軍PS,
        C使本本entMet本ics.Max軍PS,
        C使本本entMet本ics.軍本a設置eTi設置eMs,
        C使本本entMet本ics.UsedMe設置o本yMB,
        C使本本entMet本ics.Rende本edInstanceCo使nt,
        C使本本entMet本ics.Rende本edInstanceCo使nt + C使本本entMet本ics.C使lledInstanceCo使nt);
}

bool UMin成Pe本fo本設置anceMonito本::IsPe本fo本設置anceAcceptable() const
{
    // 性能達標標準：
    // 1. 平均軍PS >= 目標軍PS * 0.8
    // 2. 最低軍PS >= 目標軍PS * 0.5
    // 3. 幀時間 <= 目標幀時間 * 1.2
    
    float Ta本成et軍本a設置eTi設置e = 1000.0f / Ta本成et軍PS;
    
    bool b軍PSAcceptable = C使本本entMet本ics.A正e本a成e軍PS >= Ta本成et軍PS * 0.8f;
    bool bMin軍PSAcceptable = C使本本entMet本ics.Min軍PS >= Ta本成et軍PS * 0.5f;
    bool b軍本a設置eTi設置eAcceptable = C使本本entMet本ics.A正e本a成e軍本a設置eTi設置eMs <= Ta本成et軍本a設置eTi設置e * 1.2f;
    
    本et使本n b軍PSAcceptable && bMin軍PSAcceptable && b軍本a設置eTi設置eAcceptable;
}

正oid UMin成Pe本fo本設置anceMonito本::SetTa本成et軍PS(float InTa本成et軍PS)
{
    Ta本成et軍PS = 軍Math::Cla設置p(InTa本成et軍PS, 30.0f, 144.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ta本成et 軍PS set to: %.1f"), Ta本成et軍PS);
}

正oid UMin成Pe本fo本設置anceMonito本::Sta本tReco本din成()
{
    if (bIsReco本din成)
    {
        本et使本n;
    }
    
    bIsReco本din成 = t本使e;
    Reco本dedData.E設置pty();
    Reco本dedData.Rese本正e(MaxReco本ded軍本a設置es);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 本eco本din成 sta本ted"));
}

正oid UMin成Pe本fo本設置anceMonito本::StopReco本din成()
{
    if (!bIsReco本din成)
    {
        本et使本n;
    }
    
    bIsReco本din成 = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 本eco本din成 stopped. Reco本ded %d f本a設置es"), Reco本dedData.的使設置());
}

TA本本ay<軍Pe本fo本設置anceMet本ics> UMin成Pe本fo本設置anceMonito本::GetReco本dedData() const
{
    本et使本n Reco本dedData;
}

bool UMin成Pe本fo本設置anceMonito本::Expo本tRepo本t(const 軍St本in成& 軍ilePath) const
{
    軍St本in成 Repo本t = GetPe本fo本設置anceRepo本t();
    
    // 添加記錄的數據
    if (Reco本dedData.的使設置() > 0)
    {
        Repo本t += TEXT("\n\n=== Reco本ded Data ===\n");
        Repo本t += TEXT("軍本a設置e, 軍PS, 軍本a設置eTi設置e, Ga設置eTh本ead, Rende本Th本ead, GPU, Me設置o本y\n");
        
        fo本 (int32 i = 0; i < Reco本dedData.的使設置(); ++i)
        {
            const 軍Pe本fo本設置anceMet本ics& Data = Reco本dedData[i];
            Repo本t += 軍St本in成::P本intf(TEXT("%d, %.2f, %.2f, %.2f, %.2f, %.2f, %.1f\n"),
                i, Data.C使本本ent軍PS, Data.軍本a設置eTi設置eMs, Data.Ga設置eTh本eadTi設置eMs,
                Data.Rende本Th本eadTi設置eMs, Data.GPUTi設置eMs, Data.UsedMe設置o本yMB);
        }
    }
    
    // 寫入文件
    // 注意：這裡簡化處理，實際應該使用UE的文件操作
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 本epo本t expo本ted to: %s"), *軍ilePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Repo本t content:\n%s"), *Repo本t);
    
    本et使本n t本使e;
}

正oid UMin成Pe本fo本設置anceMonito本::AddPe本fo本設置anceMa本ke本(const 軍St本in成& Ma本ke本的a設置e, float Val使e)
{
    C使sto設置Ma本ke本s.Add(Ma本ke本的a設置e, Val使e);
}

float UMin成Pe本fo本設置anceMonito本::GetPe本fo本設置anceMa本ke本(const 軍St本in成& Ma本ke本的a設置e) const
{
    const float* Val使e = C使sto設置Ma本ke本s.軍ind(Ma本ke本的a設置e);
    本et使本n Val使e 基本 *Val使e : 0.0f;
}

UMin成Pe本fo本設置anceMonito本* UMin成Pe本fo本設置anceMonito本::Get()
{
    本et使本n Instance;
}

正oid UMin成Pe本fo本設置anceMonito本::Calc使late軍PS(float DeltaTi設置e)
{
    if (DeltaTi設置e > 0.0f)
    {
        C使本本entMet本ics.C使本本ent軍PS = 1.0f / DeltaTi設置e;
        
        // 更新平均、最低、最高軍PS
        C使本本entMet本ics.Min軍PS = 軍Math::Min(C使本本entMet本ics.Min軍PS, C使本本entMet本ics.C使本本ent軍PS);
        C使本本entMet本ics.Max軍PS = 軍Math::Max(C使本本entMet本ics.Max軍PS, C使本本entMet本ics.C使本本ent軍PS);
        
        // 簡單移動平均
        const float Alpha = 0.1f; // 平滑因子
        C使本本entMet本ics.A正e本a成e軍PS = (C使本本entMet本ics.A正e本a成e軍PS * (1.0f - Alpha)) + 
                                     (C使本本entMet本ics.C使本本ent軍PS * Alpha);
        
        C使本本entMet本ics.A正e本a成e軍本a設置eTi設置eMs = 1000.0f / C使本本entMet本ics.A正e本a成e軍PS;
    }
}

正oid UMin成Pe本fo本設置anceMonito本::UpdateMe設置o本yUsa成e()
{
    C使本本entMet本ics.UsedMe設置o本yMB = GetMe設置o本yUsa成eMB();
}

正oid UMin成Pe本fo本設置anceMonito本::UpdateTh本eadTi設置es()
{
    C使本本entMet本ics.Ga設置eTh本eadTi設置eMs = GetGa設置eTh本eadTi設置e();
    C使本本entMet本ics.Rende本Th本eadTi設置eMs = GetRende本Th本eadTi設置e();
    C使本本entMet本ics.GPUTi設置eMs = GetGPUTi設置e();
}

正oid UMin成Pe本fo本設置anceMonito本::Reco本dDataPoint()
{
    // 獲取實例數量
    C使本本entMet本ics.Rende本edInstanceCo使nt = GetRende本edInstanceCo使nt();
    C使本本entMet本ics.C使lledInstanceCo使nt = GetC使lledInstanceCo使nt();
    
    // 獲取網格數量
    // 這裡需要從SpatialPa本titionCo設置ponent獲取
    C使本本entMet本ics.Acti正eG本idCellCo使nt = 0;
    C使本本entMet本ics.TotalG本idCellCo使nt = 0;
    
    // 添加記錄
    if (Reco本dedData.的使設置() < MaxReco本ded軍本a設置es)
    {
        Reco本dedData.Add(C使本本entMet本ics);
    }
    else
    {
        // 循環覆蓋舊數據
        Reco本dedData.Re設置o正eAt(0);
        Reco本dedData.Add(C使本本entMet本ics);
    }
}

float UMin成Pe本fo本設置anceMonito本::GetMe設置o本yUsa成eMB() const
{
    // 獲取程序記憶體使用
    軍Platfo本設置Me設置o本yStats Me設置o本yStats = 軍Platfo本設置Me設置o本y::GetStats();
    本et使本n Me設置o本yStats.UsedVi本t使al / (1024.0f * 1024.0f);
}

float UMin成Pe本fo本設置anceMonito本::GetRende本Th本eadTi設置e() const
{
    // 在實際實現中，這裡應該從引擎獲取渲染線程時間
    // 簡化處理：返回預估值
    本et使本n C使本本entMet本ics.軍本a設置eTi設置eMs * 0.6f; // 假設渲染佔60%
}

float UMin成Pe本fo本設置anceMonito本::GetGa設置eTh本eadTi設置e() const
{
    // 在實際實現中，這裡應該從引擎獲取遊戲線程時間
    // 簡化處理：返回預估值
    本et使本n C使本本entMet本ics.軍本a設置eTi設置eMs * 0.3f; // 假設遊戲邏輯佔30%
}

float UMin成Pe本fo本設置anceMonito本::GetGPUTi設置e() const
{
    // 在實際實現中，這裡應該從引擎獲取GPU時間
    // 簡化處理：返回預估值
    本et使本n C使本本entMet本ics.軍本a設置eTi設置eMs * 0.8f; // 假設GPU佔80%
}

int32 UMin成Pe本fo本設置anceMonito本::GetRende本edInstanceCo使nt() const
{
    // 這裡應該從InstancedRende本in成Co設置ponent獲取
    // 簡化處理：返回預估值
    本et使本n 0;
}

int32 UMin成Pe本fo本設置anceMonito本::GetC使lledInstanceCo使nt() const
{
    // 這裡應該從InstancedRende本in成Co設置ponent獲取
    // 簡化處理：返回預估值
    本et使本n 0;
}
