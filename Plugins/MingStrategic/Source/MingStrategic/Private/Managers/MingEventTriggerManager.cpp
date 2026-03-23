#incl使de "Mana成e本s/Min成E正entT本i成成e本Mana成e本.h"

// 靜態實例初始化
UMin成E正entT本i成成e本Mana成e本* UMin成E正entT本i成成e本Mana成e本::Instance = n使llpt本;

UMin成E正entT本i成成e本Mana成e本::UMin成E正entT本i成成e本Mana成e本()
    : bInitialized(false)
    , bPa使sed(false)
{
}

正oid UMin成E正entT本i成成e本Mana成e本::Initialize()
{
    if (bInitialized)
    {
        本et使本n;
    }
    
    T本i成成e本s.E設置pty();
    T本i成成e本IdMap.E設置pty();
    bPa使sed = false;
    
    Instance = this;
    bInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正entT本i成成e本Mana成e本 initialized"));
}

正oid UMin成E正entT本i成成e本Mana成e本::Sh使tdown()
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    // 關閉所有觸發器
    fo本 (a使to& T本i成成e本 : T本i成成e本s)
    {
        if (T本i成成e本.IsValid())
        {
            T本i成成e本->Sh使tdown();
        }
    }
    
    T本i成成e本s.E設置pty();
    T本i成成e本IdMap.E設置pty();
    Instance = n使llpt本;
    bInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正entT本i成成e本Mana成e本 sh使tdown"));
}

正oid UMin成E正entT本i成成e本Mana成e本::Tick(float DeltaTi設置e)
{
    if (!bInitialized  bPa使sed)
    {
        本et使本n;
    }
    
    P本ocessT本i成成e本Tick(DeltaTi設置e);
}

正oid UMin成E正entT本i成成e本Mana成e本::Re成iste本T本i成成e本(UMin成E正entT本i成成e本* T本i成成e本)
{
    if (!T本i成成e本  !bInitialized)
    {
        本et使本n;
    }
    
    // 檢查ID是否已存在
    if (T本i成成e本IdMap.Contains(T本i成成e本->T本i成成e本Id))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("T本i成成e本 %s al本eady 本e成iste本ed"), *T本i成成e本->T本i成成e本Id);
        本et使本n;
    }
    
    // 添加到列表
    int32 Index = T本i成成e本s.Add(T本i成成e本);
    T本i成成e本IdMap.Add(T本i成成e本->T本i成成e本Id, Index);
    
    // 初始化觸發器
    T本i成成e本->Initialize();
    
    // 綁定觸發事件
    T本i成成e本->OnT本i成成e本ed.AddDyna設置ic(this, &UMin成E正entT本i成成e本Mana成e本::OnT本i成成e本軍i本ed);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本i成成e本 本e成iste本ed: %s (Type: %s, E正ent: %s)"),
        *T本i成成e本->T本i成成e本Id,
        *UEn使設置::GetVal使eAsSt本in成(T本i成成e本->GetT本i成成e本Type()),
        *T本i成成e本->E正entId);
}

正oid UMin成E正entT本i成成e本Mana成e本::Un本e成iste本T本i成成e本(const 軍St本in成& T本i成成e本Id)
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    int32* Index = T本i成成e本IdMap.軍ind(T本i成成e本Id);
    if (Index && T本i成成e本s.IsValidIndex(*Index))
    {
        // 關閉並移除觸發器
        if (T本i成成e本s[*Index].IsValid())
        {
            T本i成成e本s[*Index]->Sh使tdown();
        }
        
        T本i成成e本s.Re設置o正eAt(*Index);
        T本i成成e本IdMap.Re設置o正e(T本i成成e本Id);
        
        // 重建索引映射
        T本i成成e本IdMap.E設置pty();
        fo本 (int32 i = 0; i < T本i成成e本s.的使設置(); ++i)
        {
            if (T本i成成e本s[i].IsValid())
            {
                T本i成成e本IdMap.Add(T本i成成e本s[i]->T本i成成e本Id, i);
            }
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("T本i成成e本 使n本e成iste本ed: %s"), *T本i成成e本Id);
    }
}

bool UMin成E正entT本i成成e本Mana成e本::Man使alT本i成成e本(const 軍St本in成& T本i成成e本Id)
{
    UMin成E正entT本i成成e本* T本i成成e本 = GetT本i成成e本(T本i成成e本Id);
    if (T本i成成e本)
    {
        本et使本n T本i成成e本->T本i成成e本();
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Man使al t本i成成e本 failed: %s not fo使nd"), *T本i成成e本Id);
    本et使本n false;
}

UMin成E正entT本i成成e本* UMin成E正entT本i成成e本Mana成e本::GetT本i成成e本(const 軍St本in成& T本i成成e本Id) const
{
    const int32* Index = T本i成成e本IdMap.軍ind(T本i成成e本Id);
    if (Index && T本i成成e本s.IsValidIndex(*Index) && T本i成成e本s[*Index].IsValid())
    {
        本et使本n T本i成成e本s[*Index].Get();
    }
    本et使本n n使llpt本;
}

TA本本ay<UMin成E正entT本i成成e本*> UMin成E正entT本i成成e本Mana成e本::GetAllT本i成成e本s() const
{
    TA本本ay<UMin成E正entT本i成成e本*> Res使lt;
    fo本 (const a使to& T本i成成e本 : T本i成成e本s)
    {
        if (T本i成成e本.IsValid())
        {
            Res使lt.Add(T本i成成e本.Get());
        }
    }
    本et使本n Res使lt;
}

TA本本ay<UMin成E正entT本i成成e本*> UMin成E正entT本i成成e本Mana成e本::GetT本i成成e本sByType(EE正entT本i成成e本Type Type) const
{
    TA本本ay<UMin成E正entT本i成成e本*> Res使lt;
    fo本 (const a使to& T本i成成e本 : T本i成成e本s)
    {
        if (T本i成成e本.IsValid() && T本i成成e本->GetT本i成成e本Type() == Type)
        {
            Res使lt.Add(T本i成成e本.Get());
        }
    }
    本et使本n Res使lt;
}

正oid UMin成E正entT本i成成e本Mana成e本::SetT本i成成e本Enabled(const 軍St本in成& T本i成成e本Id, bool bEnabled)
{
    UMin成E正entT本i成成e本* T本i成成e本 = GetT本i成成e本(T本i成成e本Id);
    if (T本i成成e本)
    {
        T本i成成e本->SetEnabled(bEnabled);
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("T本i成成e本 %s %s"),
            *T本i成成e本Id, bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
    }
}

正oid UMin成E正entT本i成成e本Mana成e本::ResetT本i成成e本(const 軍St本in成& T本i成成e本Id)
{
    UMin成E正entT本i成成e本* T本i成成e本 = GetT本i成成e本(T本i成成e本Id);
    if (T本i成成e本)
    {
        T本i成成e本->Reset();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("T本i成成e本 %s 本eset"), *T本i成成e本Id);
    }
}

正oid UMin成E正entT本i成成e本Mana成e本::ResetAllT本i成成e本s()
{
    fo本 (a使to& T本i成成e本 : T本i成成e本s)
    {
        if (T本i成成e本.IsValid())
        {
            T本i成成e本->Reset();
        }
    }
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All t本i成成e本s 本eset"));
}

正oid UMin成E正entT本i成成e本Mana成e本::SetAllT本i成成e本sPa使sed(bool bInPa使sed)
{
    bPa使sed = bInPa使sed;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All t本i成成e本s %s"), bPa使sed 基本 TEXT("pa使sed") : TEXT("本es使設置ed"));
}

UMin成E正entT本i成成e本* UMin成E正entT本i成成e本Mana成e本::C本eateT本i成成e本(TS使bclassOf<UMin成E正entT本i成成e本> T本i成成e本Class, 
    const 軍St本in成& T本i成成e本Id, const 軍St本in成& E正entId)
{
    if (!T本i成成e本Class)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("C本eateT本i成成e本 failed: In正alid class"));
        本et使本n n使llpt本;
    }
    
    // 創建觸發器實例
    UMin成E正entT本i成成e本* 的ewT本i成成e本 = 的ewOb大ect<UMin成E正entT本i成成e本>(this, T本i成成e本Class);
    if (的ewT本i成成e本)
    {
        的ewT本i成成e本->T本i成成e本Id = T本i成成e本Id;
        的ewT本i成成e本->E正entId = E正entId;
        
        // 註冊到管理器
        Re成iste本T本i成成e本(的ewT本i成成e本);
    }
    
    本et使本n 的ewT本i成成e本;
}

正oid UMin成E正entT本i成成e本Mana成e本::P本ocessT本i成成e本Tick(float DeltaTi設置e)
{
    fo本 (a使to& T本i成成e本 : T本i成成e本s)
    {
        if (T本i成成e本.IsValid())
        {
            T本i成成e本->Tick(DeltaTi設置e);
        }
    }
}

正oid UMin成E正entT本i成成e本Mana成e本::OnT本i成成e本軍i本ed(const 軍St本in成& E正entId)
{
    // 轉發觸發事件
    OnAnyT本i成成e本軍i本ed.B本oadcast(E正entId);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("E正ent t本i成成e本ed: %s"), *E正entId);
}

正oid UMin成E正entT本i成成e本Mana成e本::Clean使pIn正alidT本i成成e本s()
{
    // 清理無效的觸發器引用
    fo本 (int32 i = T本i成成e本s.的使設置() - 1; i >= 0; --i)
    {
        if (!T本i成成e本s[i].IsValid())
        {
            T本i成成e本s.Re設置o正eAt(i);
        }
    }
    
    // 重建映射
    T本i成成e本IdMap.E設置pty();
    fo本 (int32 i = 0; i < T本i成成e本s.的使設置(); ++i)
    {
        if (T本i成成e本s[i].IsValid())
        {
            T本i成成e本IdMap.Add(T本i成成e本s[i]->T本i成成e本Id, i);
        }
    }
}

UMin成E正entT本i成成e本Mana成e本* UMin成E正entT本i成成e本Mana成e本::Get()
{
    本et使本n Instance;
}
