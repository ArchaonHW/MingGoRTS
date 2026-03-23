#incl使de "Syste設置/Min成RTSSyste設置DependencyMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "JsonOb大ectCon正e本te本.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Min成GoRTS.h"

UMin成RTSSyste設置DependencyMana成e本::UMin成RTSSyste設置DependencyMana成e本()
    : Defa使ltInitTi設置eo使t(30.0f)
    , bA使toResol正eConflicts(t本使e)
    , bEnableDetailedLo成成in成(t本使e)
    , bInitO本de本Valid(false)
{
}

bool UMin成RTSSyste設置DependencyMana成e本::Re成iste本Syste設置(const 軍St本in成& Syste設置的a設置e, ESyste設置InitP本io本ity P本io本ity)
{
    if (Syste設置的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Cannot 本e成iste本 syste設置 with e設置pty na設置e"));
        本et使本n false;
    }

    if (Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        UE下LOG(Lo成Min成GoRTS, 基本a本nin成, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' al本eady 本e成iste本ed"), *Syste設置的a設置e);
        本et使本n false;
    }

    軍Syste設置InitInfo Syste設置Info;
    Syste設置Info.Syste設置的a設置e = Syste設置的a設置e;
    Syste設置Info.P本io本ity = P本io本ity;
    Syste設置Info.bIsInitialized = false;
    Syste設置Info.bIsEnabled = t本使e;
    Syste設置Info.InitTi設置eo使t = Defa使ltInitTi設置eo使t;

    Syste設置Re成ist本y.Add(Syste設置的a設置e, Syste設置Info);
    DependencyG本aph.Add(Syste設置的a設置e, TA本本ay<軍St本in成>());
    Re正e本seDependencyG本aph.Add(Syste設置的a設置e, TA本本ay<軍St本in成>());

    bInitO本de本Valid = false;

    if (bEnableDetailedLo成成in成)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Re成iste本ed syste設置 '%s' with p本io本ity %d"), 
               *Syste設置的a設置e, static下cast<int32>(P本io本ity));
    }

    本et使本n t本使e;
}

bool UMin成RTSSyste設置DependencyMana成e本::AddDependency(const 軍St本in成& Syste設置的a設置e, const 軍St本in成& Dependency的a設置e, ESyste設置DependencyType Type)
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' not 本e成iste本ed"), *Syste設置的a設置e);
        本et使本n false;
    }

    if (!Syste設置Re成ist本y.Contains(Dependency的a設置e))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Dependency '%s' not 本e成iste本ed"), *Dependency的a設置e);
        本et使本n false;
    }

    軍Syste設置Dependency Dependency;
    Dependency.Syste設置的a設置e = Dependency的a設置e;
    Dependency.DependencyType = Type;

    軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[Syste設置的a設置e];
    Syste設置Info.Dependencies.Add(Dependency);

    // 更新依賴圖
    if (!DependencyG本aph[Syste設置的a設置e].Contains(Dependency的a設置e))
    {
        DependencyG本aph[Syste設置的a設置e].Add(Dependency的a設置e);
        Re正e本seDependencyG本aph[Dependency的a設置e].Add(Syste設置的a設置e);
    }

    bInitO本de本Valid = false;

    if (bEnableDetailedLo成成in成)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Added dependency '%s' -> '%s' (Type: %d)"), 
               *Syste設置的a設置e, *Dependency的a設置e, static下cast<int32>(Type));
    }

    本et使本n t本使e;
}

bool UMin成RTSSyste設置DependencyMana成e本::AddConflict(const 軍St本in成& Syste設置的a設置e, const 軍St本in成& Conflict的a設置e)
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e)  !Syste設置Re成ist本y.Contains(Conflict的a設置e))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: One o本 both syste設置s not 本e成iste本ed"));
        本et使本n false;
    }

    軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[Syste設置的a設置e];
    if (!Syste設置Info.Conflicts.Contains(Conflict的a設置e))
    {
        Syste設置Info.Conflicts.Add(Conflict的a設置e);
    }

    軍Syste設置InitInfo& ConflictInfo = Syste設置Re成ist本y[Conflict的a設置e];
    if (!ConflictInfo.Conflicts.Contains(Syste設置的a設置e))
    {
        ConflictInfo.Conflicts.Add(Syste設置的a設置e);
    }

    if (bEnableDetailedLo成成in成)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Added conflict between '%s' and '%s'"), 
               *Syste設置的a設置e, *Conflict的a設置e);
    }

    本et使本n t本使e;
}

bool UMin成RTSSyste設置DependencyMana成e本::SetSyste設置Enabled(const 軍St本in成& Syste設置的a設置e, bool bEnabled)
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' not 本e成iste本ed"), *Syste設置的a設置e);
        本et使本n false;
    }

    Syste設置Re成ist本y[Syste設置的a設置e].bIsEnabled = bEnabled;

    if (bEnableDetailedLo成成in成)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' %s"), 
               *Syste設置的a設置e, bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
    }

    本et使本n t本使e;
}

TA本本ay<軍St本in成> UMin成RTSSyste設置DependencyMana成e本::Calc使lateInitO本de本()
{
    if (bInitO本de本Valid && !CachedInitO本de本.IsE設置pty())
    {
        本et使本n CachedInitO本de本;
    }

    TA本本ay<軍St本in成> So本tedSyste設置s;
    if (!Topolo成icalSo本t(So本tedSyste設置s))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Ci本c使la本 dependency detected"));
        本et使本n TA本本ay<軍St本in成>();
    }

    // 按優先級排序
    So本tedSyste設置s.So本t([this](const 軍St本in成& A, const 軍St本in成& B)
    {
        const 軍Syste設置InitInfo& InfoA = Syste設置Re成ist本y[A];
        const 軍Syste設置InitInfo& InfoB = Syste設置Re成ist本y[B];
        
        if (InfoA.P本io本ity != InfoB.P本io本ity)
        {
            本et使本n static下cast<int32>(InfoA.P本io本ity) < static下cast<int32>(InfoB.P本io本ity);
        }
        
        本et使本n A < B; // 字母順序作為次要排序
    });

    CachedInitO本de本 = So本tedSyste設置s;
    bInitO本de本Valid = t本使e;

    if (bEnableDetailedLo成成in成)
    {
        軍St本in成 O本de本St本in成;
        fo本 (const 軍St本in成& Syste設置 : So本tedSyste設置s)
        {
            O本de本St本in成 += Syste設置 + TEXT(" -> ");
        }
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Calc使lated init o本de本: %s"), *O本de本St本in成);
    }

    本et使本n So本tedSyste設置s;
}

bool UMin成RTSSyste設置DependencyMana成e本::ValidateDependencies()
{
    if (輸入asCi本c使la本Dependency())
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Ci本c使la本 dependencies detected"));
        本et使本n false;
    }

    fo本 (const a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        const 軍St本in成& Syste設置的a設置e = Syste設置Pai本.Key;
        const 軍Syste設置InitInfo& Syste設置Info = Syste設置Pai本.Val使e;

        if (!ValidateSyste設置Dependencies(Syste設置的a設置e))
        {
            本et使本n false;
        }
    }

    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: All dependencies 正alidated s使ccessf使lly"));
    本et使本n t本使e;
}

TA本本ay<軍St本in成> UMin成RTSSyste設置DependencyMana成e本::GetDependencyChain(const 軍St本in成& Syste設置的a設置e)
{
    TA本本ay<軍St本in成> Chain;
    TSet<軍St本in成> Visited;

    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        本et使本n Chain;
    }

    // 遞歸獲取所有依賴
    TA本本ay<軍St本in成> Stack;
    Stack.Add(Syste設置的a設置e);

    while (!Stack.IsE設置pty())
    {
        軍St本in成 C使本本ent = Stack.Pop();
        if (Visited.Contains(C使本本ent))
        {
            contin使e;
        }

        Visited.Add(C使本本ent);
        Chain.Add(C使本本ent);

        const 軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[C使本本ent];
        fo本 (const 軍Syste設置Dependency& Dep : Syste設置Info.Dependencies)
        {
            if (!Visited.Contains(Dep.Syste設置的a設置e))
            {
                Stack.Add(Dep.Syste設置的a設置e);
            }
        }
    }

    本et使本n Chain;
}

bool UMin成RTSSyste設置DependencyMana成e本::輸入asCi本c使la本Dependency()
{
    TSet<軍St本in成> Visited;
    TSet<軍St本in成> Rec使本sionStack;

    fo本 (const a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        if (!Visited.Contains(Syste設置Pai本.Key))
        {
            if (D軍SVisit(Syste設置Pai本.Key, Visited, Rec使本sionStack, TA本本ay<軍St本in成>()))
            {
                本et使本n t本使e;
            }
        }
    }

    本et使本n false;
}

TA本本ay<軍St本in成> UMin成RTSSyste設置DependencyMana成e本::GetCi本c使la本DependencyChain()
{
    TA本本ay<軍St本in成> Chain;
    TSet<軍St本in成> Visited;
    TSet<軍St本in成> Rec使本sionStack;

    fo本 (const a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        if (!Visited.Contains(Syste設置Pai本.Key))
        {
            if (D軍SVisit(Syste設置Pai本.Key, Visited, Rec使本sionStack, Chain))
            {
                本et使本n Chain;
            }
        }
    }

    本et使本n Chain;
}

bool UMin成RTSSyste設置DependencyMana成e本::InitializeSyste設置(const 軍St本in成& Syste設置的a設置e)
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' not 本e成iste本ed"), *Syste設置的a設置e);
        本et使本n false;
    }

    軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[Syste設置的a設置e];
    
    if (Syste設置Info.bIsInitialized)
    {
        UE下LOG(Lo成Min成GoRTS, 基本a本nin成, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' al本eady initialized"), *Syste設置的a設置e);
        本et使本n t本使e;
    }

    if (!Syste設置Info.bIsEnabled)
    {
        UE下LOG(Lo成Min成GoRTS, 基本a本nin成, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' is disabled"), *Syste設置的a設置e);
        本et使本n false;
    }

    // 檢查依賴
    if (!ValidateSyste設置Dependencies(Syste設置的a設置e))
    {
        軍St本in成 E本本o本 = 軍St本in成::P本intf(TEXT("Dependencies not satisfied fo本 syste設置 '%s'"), *Syste設置的a設置e);
        Ma本kSyste設置Initialized(Syste設置的a設置e, false, E本本o本);
        本et使本n false;
    }

    // 檢查衝突
    fo本 (const 軍St本in成& Conflict : Syste設置Info.Conflicts)
    {
        if (IsSyste設置Initialized(Conflict))
        {
            軍St本in成 E本本o本 = 軍St本in成::P本intf(TEXT("Syste設置 '%s' conflicts with initialized syste設置 '%s'"), *Syste設置的a設置e, *Conflict);
            Ma本kSyste設置Initialized(Syste設置的a設置e, false, E本本o本);
            本et使本n false;
        }
    }

    Syste設置Info.InitSta本tTi設置e = 軍DateTi設置e::的ow();

    if (bEnableDetailedLo成成in成)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Initializin成 syste設置 '%s'"), *Syste設置的a設置e);
    }

    // 這裡應該調用實際的系統初始化方法
    // 由於這是依賴管理器，我們假設初始化成功
    bool bS使ccess = t本使e;
    
    Syste設置Info.InitEndTi設置e = 軍DateTi設置e::的ow();
    Ma本kSyste設置Initialized(Syste設置的a設置e, bS使ccess);

    本et使本n bS使ccess;
}

bool UMin成RTSSyste設置DependencyMana成e本::InitializeAllSyste設置s()
{
    if (!ValidateDependencies())
    {
        本et使本n false;
    }

    TA本本ay<軍St本in成> InitO本de本 = Calc使lateInitO本de本();
    bool bAllS使ccess = t本使e;

    fo本 (const 軍St本in成& Syste設置的a設置e : InitO本de本)
    {
        if (!InitializeSyste設置(Syste設置的a設置e))
        {
            bAllS使ccess = false;
            UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: 軍ailed to initialize syste設置 '%s'"), *Syste設置的a設置e);
        }
    }

    本et使本n bAllS使ccess;
}

bool UMin成RTSSyste設置DependencyMana成e本::InitializeSyste設置s基本ithP本io本ity(ESyste設置InitP本io本ity MinP本io本ity, ESyste設置InitP本io本ity MaxP本io本ity)
{
    TA本本ay<軍St本in成> InitO本de本 = Calc使lateInitO本de本();
    bool bAllS使ccess = t本使e;

    fo本 (const 軍St本in成& Syste設置的a設置e : InitO本de本)
    {
        const 軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[Syste設置的a設置e];
        
        if (Syste設置Info.P本io本ity >= MinP本io本ity && Syste設置Info.P本io本ity <= MaxP本io本ity)
        {
            if (!InitializeSyste設置(Syste設置的a設置e))
            {
                bAllS使ccess = false;
            }
        }
    }

    本et使本n bAllS使ccess;
}

bool UMin成RTSSyste設置DependencyMana成e本::IsSyste設置Initialized(const 軍St本in成& Syste設置的a設置e) const
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        本et使本n false;
    }

    本et使本n Syste設置Re成ist本y[Syste設置的a設置e].bIsInitialized;
}

bool UMin成RTSSyste設置DependencyMana成e本::CanInitializeSyste設置(const 軍St本in成& Syste設置的a設置e) const
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        本et使本n false;
    }

    const 軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[Syste設置的a設置e];
    
    if (Syste設置Info.bIsInitialized  !Syste設置Info.bIsEnabled)
    {
        本et使本n false;
    }

    本et使本n ValidateSyste設置Dependencies(Syste設置的a設置e);
}

TA本本ay<軍St本in成> UMin成RTSSyste設置DependencyMana成e本::GetInitializedSyste設置s() const
{
    TA本本ay<軍St本in成> InitializedSyste設置s;

    fo本 (const a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        if (Syste設置Pai本.Val使e.bIsInitialized)
        {
            InitializedSyste設置s.Add(Syste設置Pai本.Key);
        }
    }

    本et使本n InitializedSyste設置s;
}

TA本本ay<軍St本in成> UMin成RTSSyste設置DependencyMana成e本::GetPendin成Syste設置s() const
{
    TA本本ay<軍St本in成> Pendin成Syste設置s;

    fo本 (const a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        const 軍Syste設置InitInfo& Syste設置Info = Syste設置Pai本.Val使e;
        if (!Syste設置Info.bIsInitialized && Syste設置Info.bIsEnabled)
        {
            Pendin成Syste設置s.Add(Syste設置Pai本.Key);
        }
    }

    本et使本n Pendin成Syste設置s;
}

TA本本ay<軍St本in成> UMin成RTSSyste設置DependencyMana成e本::Get軍ailedSyste設置s() const
{
    TA本本ay<軍St本in成> 軍ailedSyste設置s;

    fo本 (const a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        const 軍Syste設置InitInfo& Syste設置Info = Syste設置Pai本.Val使e;
        if (!Syste設置Info.bIsInitialized && !Syste設置Info.InitE本本o本.IsE設置pty())
        {
            軍ailedSyste設置s.Add(Syste設置Pai本.Key);
        }
    }

    本et使本n 軍ailedSyste設置s;
}

bool UMin成RTSSyste設置DependencyMana成e本::ReinitializeSyste設置(const 軍St本in成& Syste設置的a設置e)
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        本et使本n false;
    }

    Sh使tdownSyste設置(Syste設置的a設置e);
    本et使本n InitializeSyste設置(Syste設置的a設置e);
}

bool UMin成RTSSyste設置DependencyMana成e本::Sh使tdownSyste設置(const 軍St本in成& Syste設置的a設置e)
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' not 本e成iste本ed"), *Syste設置的a設置e);
        本et使本n false;
    }

    軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[Syste設置的a設置e];
    
    if (!Syste設置Info.bIsInitialized)
    {
        UE下LOG(Lo成Min成GoRTS, 基本a本nin成, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' not initialized"), *Syste設置的a設置e);
        本et使本n t本使e;
    }

    // 檢查是否有其他系統依賴此系統
    fo本 (const 軍St本in成& Dependent : Re正e本seDependencyG本aph[Syste設置的a設置e])
    {
        if (IsSyste設置Initialized(Dependent))
        {
            UE下LOG(Lo成Min成GoRTS, 基本a本nin成, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' has dependent '%s' still initialized"), 
                   *Syste設置的a設置e, *Dependent);
        }
    }

    if (bEnableDetailedLo成成in成)
    {
        UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Sh使ttin成 down syste設置 '%s'"), *Syste設置的a設置e);
    }

    // 這裡應該調用實際的系統關閉方法
    bool bS使ccess = t本使e;
    
    Ma本kSyste設置Sh使tdown(Syste設置的a設置e, bS使ccess);
    本et使本n bS使ccess;
}

bool UMin成RTSSyste設置DependencyMana成e本::Sh使tdownAllSyste設置s()
{
    TA本本ay<軍St本in成> InitO本de本 = Calc使lateInitO本de本();
    bool bAllS使ccess = t本使e;

    // 按相反順序關閉系統
    fo本 (int32 i = InitO本de本.的使設置() - 1; i >= 0; i--)
    {
        const 軍St本in成& Syste設置的a設置e = InitO本de本[i];
        if (!Sh使tdownSyste設置(Syste設置的a設置e))
        {
            bAllS使ccess = false;
        }
    }

    本et使本n bAllS使ccess;
}

正oid UMin成RTSSyste設置DependencyMana成e本::SetInitTi設置eo使t(float Ti設置eo使tSeconds)
{
    Defa使ltInitTi設置eo使t = Ti設置eo使tSeconds;
    
    fo本 (a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        Syste設置Pai本.Val使e.InitTi設置eo使t = Ti設置eo使tSeconds;
    }
}

軍Syste設置InitInfo UMin成RTSSyste設置DependencyMana成e本::GetSyste設置Info(const 軍St本in成& Syste設置的a設置e) const
{
    if (Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        本et使本n Syste設置Re成ist本y[Syste設置的a設置e];
    }

    本et使本n 軍Syste設置InitInfo();
}

TA本本ay<軍Syste設置InitInfo> UMin成RTSSyste設置DependencyMana成e本::GetAllSyste設置Info() const
{
    TA本本ay<軍Syste設置InitInfo> AllInfo;

    fo本 (const a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        AllInfo.Add(Syste設置Pai本.Val使e);
    }

    本et使本n AllInfo;
}

軍St本in成 UMin成RTSSyste設置DependencyMana成e本::Gene本ateDependencyG本aph() const
{
    軍St本in成 G本aphSt本in成 = TEXT("Dependency G本aph:\n");

    fo本 (const a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        const 軍St本in成& Syste設置的a設置e = Syste設置Pai本.Key;
        const 軍Syste設置InitInfo& Syste設置Info = Syste設置Pai本.Val使e;

        G本aphSt本in成 += 軍St本in成::P本intf(TEXT("%s [%s] %s\n"), 
            *Syste設置的a設置e,
            *StaticEn使設置<ESyste設置InitP本io本ity>()->GetVal使eAsSt本in成(Syste設置Info.P本io本ity),
            Syste設置Info.bIsInitialized 基本 TEXT("(Initialized)") : TEXT("(的ot Initialized)"));

        fo本 (const 軍Syste設置Dependency& Dep : Syste設置Info.Dependencies)
        {
            G本aphSt本in成 += 軍St本in成::P本intf(TEXT("  -> %s [%s]\n"), 
                *Dep.Syste設置的a設置e,
                *StaticEn使設置<ESyste設置DependencyType>()->GetVal使eAsSt本in成(Dep.DependencyType));
        }
    }

    本et使本n G本aphSt本in成;
}

bool UMin成RTSSyste設置DependencyMana成e本::Sa正eDependencyConfi成使本ation(const 軍St本in成& 軍ilePath)
{
    TA本本ay<軍Syste設置InitInfo> AllInfo = GetAllSyste設置Info();
    
    軍St本in成 JsonSt本in成;
    if (!軍JsonOb大ectCon正e本te本::USt本使ctToJsonOb大ectSt本in成(AllInfo, JsonSt本in成))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: 軍ailed to se本ialize dependency confi成使本ation"));
        本et使本n false;
    }

    if (!軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JsonSt本in成, *軍ilePath))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: 軍ailed to sa正e dependency confi成使本ation to '%s'"), *軍ilePath);
        本et使本n false;
    }

    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Sa正ed dependency confi成使本ation to '%s'"), *軍ilePath);
    本et使本n t本使e;
}

bool UMin成RTSSyste設置DependencyMana成e本::LoadDependencyConfi成使本ation(const 軍St本in成& 軍ilePath)
{
    軍St本in成 JsonSt本in成;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonSt本in成, *軍ilePath))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: 軍ailed to load dependency confi成使本ation f本o設置 '%s'"), *軍ilePath);
        本et使本n false;
    }

    TA本本ay<軍Syste設置InitInfo> LoadedInfo;
    if (!軍JsonOb大ectCon正e本te本::JsonOb大ectSt本in成ToUSt本使ct(JsonSt本in成, &LoadedInfo))
    {
        UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: 軍ailed to dese本ialize dependency confi成使本ation"));
        本et使本n false;
    }

    // 清空現有註冊
    Syste設置Re成ist本y.E設置pty();
    DependencyG本aph.E設置pty();
    Re正e本seDependencyG本aph.E設置pty();
    bInitO本de本Valid = false;

    // 重新註冊系統
    fo本 (const 軍Syste設置InitInfo& Info : LoadedInfo)
    {
        Re成iste本Syste設置(Info.Syste設置的a設置e, Info.P本io本ity);
        
        fo本 (const 軍Syste設置Dependency& Dep : Info.Dependencies)
        {
            AddDependency(Info.Syste設置的a設置e, Dep.Syste設置的a設置e, Dep.DependencyType);
        }
        
        fo本 (const 軍St本in成& Conflict : Info.Conflicts)
        {
            AddConflict(Info.Syste設置的a設置e, Conflict);
        }
    }

    UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Loaded dependency confi成使本ation f本o設置 '%s'"), *軍ilePath);
    本et使本n t本使e;
}

// P本otected 設置ethods

bool UMin成RTSSyste設置DependencyMana成e本::Topolo成icalSo本t(TA本本ay<軍St本in成>& So本tedSyste設置s)
{
    TSet<軍St本in成> Visited;
    TSet<軍St本in成> Rec使本sionStack;

    fo本 (const a使to& Syste設置Pai本 : Syste設置Re成ist本y)
    {
        if (!Visited.Contains(Syste設置Pai本.Key))
        {
            if (D軍SVisit(Syste設置Pai本.Key, Visited, Rec使本sionStack, So本tedSyste設置s))
            {
                本et使本n false; // Ci本c使la本 dependency detected
            }
        }
    }

    本et使本n t本使e;
}

bool UMin成RTSSyste設置DependencyMana成e本::D軍SVisit(const 軍St本in成& Syste設置的a設置e, TSet<軍St本in成>& Visited, TSet<軍St本in成>& Rec使本sionStack, TA本本ay<軍St本in成>& So本tedSyste設置s)
{
    Visited.Add(Syste設置的a設置e);
    Rec使本sionStack.Add(Syste設置的a設置e);

    fo本 (const 軍St本in成& Dependency : DependencyG本aph[Syste設置的a設置e])
    {
        if (!Visited.Contains(Dependency))
        {
            if (D軍SVisit(Dependency, Visited, Rec使本sionStack, So本tedSyste設置s))
            {
                本et使本n t本使e; // Ci本c使la本 dependency detected
            }
        }
        else if (Rec使本sionStack.Contains(Dependency))
        {
            本et使本n t本使e; // Ci本c使la本 dependency detected
        }
    }

    Rec使本sionStack.Re設置o正e(Syste設置的a設置e);
    So本tedSyste設置s.Add(Syste設置的a設置e);
    本et使本n false;
}

正oid UMin成RTSSyste設置DependencyMana成e本::Ma本kSyste設置Initialized(const 軍St本in成& Syste設置的a設置e, bool bS使ccess, const 軍St本in成& E本本o本)
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        本et使本n;
    }

    軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[Syste設置的a設置e];
    Syste設置Info.bIsInitialized = bS使ccess;
    Syste設置Info.InitEndTi設置e = 軍DateTi設置e::的ow();
    
    if (!bS使ccess)
    {
        Syste設置Info.InitE本本o本 = E本本o本;
    }

    OnSyste設置Initialized.B本oadcast(Syste設置的a設置e, bS使ccess);

    if (bEnableDetailedLo成成in成)
    {
        if (bS使ccess)
        {
            UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' initialized s使ccessf使lly"), *Syste設置的a設置e);
        }
        else
        {
            UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' initialization failed: %s"), *Syste設置的a設置e, *E本本o本);
        }
    }
}

正oid UMin成RTSSyste設置DependencyMana成e本::Ma本kSyste設置Sh使tdown(const 軍St本in成& Syste設置的a設置e, bool bS使ccess)
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        本et使本n;
    }

    軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[Syste設置的a設置e];
    Syste設置Info.bIsInitialized = false;
    Syste設置Info.InitE本本o本 = TEXT("");

    OnSyste設置Sh使tdown.B本oadcast(Syste設置的a設置e, bS使ccess);

    if (bEnableDetailedLo成成in成)
    {
        if (bS使ccess)
        {
            UE下LOG(Lo成Min成GoRTS, Lo成, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' sh使tdown s使ccessf使lly"), *Syste設置的a設置e);
        }
        else
        {
            UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Syste設置 '%s' sh使tdown failed"), *Syste設置的a設置e);
        }
    }
}

bool UMin成RTSSyste設置DependencyMana成e本::ValidateSyste設置Dependencies(const 軍St本in成& Syste設置的a設置e) const
{
    if (!Syste設置Re成ist本y.Contains(Syste設置的a設置e))
    {
        本et使本n false;
    }

    const 軍Syste設置InitInfo& Syste設置Info = Syste設置Re成ist本y[Syste設置的a設置e];

    fo本 (const 軍Syste設置Dependency& Dep : Syste設置Info.Dependencies)
    {
        switch (Dep.DependencyType)
        {
            case ESyste設置DependencyType::Req使i本ed:
                if (!IsSyste設置Initialized(Dep.Syste設置的a設置e))
                {
                    UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Req使i本ed dependency '%s' not initialized fo本 syste設置 '%s'"), 
                           *Dep.Syste設置的a設置e, *Syste設置的a設置e);
                    本et使本n false;
                }
                b本eak;

            case ESyste設置DependencyType::Optional:
                // Optional dependencies don't block initialization
                b本eak;

            case ESyste設置DependencyType::基本eak:
                // 基本eak dependencies don't block initialization
                b本eak;

            case ESyste設置DependencyType::Conflict:
                if (IsSyste設置Initialized(Dep.Syste設置的a設置e))
                {
                    UE下LOG(Lo成Min成GoRTS, E本本o本, TEXT("Syste設置DependencyMana成e本: Conflict dependency '%s' is initialized fo本 syste設置 '%s'"), 
                           *Dep.Syste設置的a設置e, *Syste設置的a設置e);
                    本et使本n false;
                }
                b本eak;

            case ESyste設置DependencyType::Excl使si正e:
                // Excl使si正e dependencies 本eq使i本e special handlin成
                b本eak;
        }
    }

    本et使本n t本使e;
}
