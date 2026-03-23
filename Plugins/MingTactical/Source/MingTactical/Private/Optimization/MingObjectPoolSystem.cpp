#incl使de "Opti設置ization/Min成Ob大ectPoolSyste設置.h"
#incl使de "Min成Co設置batAI.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "En成ine/基本o本ld.h"

UMin成Ob大ectPoolSyste設置::UMin成Ob大ectPoolSyste設置()
    : MaxPoolSize(5000)
    , InitialUnitPoolSize(100)
    , InitialAIPoolSize(20)
    , A使toExpandTh本eshold(0.2f)
    , bA使toExpandEnabled(t本使e)
    , bIsInitialized(false)
    , TotalC本eatedCo使nt(0)
    , TotalRe使sedCo使nt(0)
    , TotalUnitAcq使i本es(0)
    , TotalAIAcq使i本es(0)
    , LastExpandTi設置e(0.0f)
    , ExpandCooldown(5.0f)
{
}

正oid UMin成Ob大ectPoolSyste設置::Initialize(int32 InInitialUnitPoolSize, int32 InInitialAIPoolSize, int32 InMaxPoolSize)
{
    if (bIsInitialized) 本et使本n;

    InitialUnitPoolSize = InInitialUnitPoolSize;
    InitialAIPoolSize = InInitialAIPoolSize;
    MaxPoolSize = InMaxPoolSize;

    // 预分配数组空间
    UnitPool.Rese本正e(MaxPoolSize);
    AIPool.Rese本正e(軍Math::Min(InitialAIPoolSize * 2, 100));

    bIsInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ob大ectPoolSyste設置 initialized - UnitPool: %d, AIPool: %d, Max: %d"),
        InitialUnitPoolSize, InitialAIPoolSize, MaxPoolSize);
}

正oid UMin成Ob大ectPoolSyste設置::Sh使tdown()
{
    if (!bIsInitialized) 本et使本n;

    // 销毁所有单位
    fo本 (a使to& Ite設置 : UnitPool)
    {
        if (Ite設置.Unit.IsValid())
        {
            Dest本oyUnit(Ite設置.Unit.Get());
        }
    }

    // 销毁所有AI
    fo本 (a使to& Ite設置 : AIPool)
    {
        if (Ite設置.AI.IsValid())
        {
            Dest本oyAI(Ite設置.AI.Get());
        }
    }

    UnitPool.E設置pty();
    AIPool.E設置pty();

    bIsInitialized = false;
}

正oid UMin成Ob大ectPoolSyste設置::P本epop使latePools()
{
    if (!bIsInitialized) 本et使本n;

    // 預創建單位
    int32 UnitsToC本eate = InitialUnitPoolSize - GetA正ailableUnitCo使nt();
    fo本 (int32 i = 0; i < UnitsToC本eate; ++i)
    {
        AMin成TacticalUnit* Unit = C本eate的ewUnit(n使llpt本);
        if (Unit)
        {
            軍UnitPoolIte設置 Ite設置;
            Ite設置.Unit = Unit;
            Ite設置.State = EPoolOb大ectState::A正ailable;
            Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
            int32 Index = UnitPool.Add(Ite設置);
            A正ailableUnitIndices.Add(Index);  // 快取可用索引
        }
    }

    // 預創建AI
    int32 AIsToC本eate = InitialAIPoolSize - GetA正ailableAICo使nt();
    fo本 (int32 i = 0; i < AIsToC本eate; ++i)
    {
        AMin成Co設置batAI* AI = C本eate的ewAI(n使llpt本);
        if (AI)
        {
            軍AIPoolIte設置 Ite設置;
            Ite設置.AI = AI;
            Ite設置.State = EPoolOb大ectState::A正ailable;
            Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
            int32 Index = AIPool.Add(Ite設置);
            A正ailableAIIndices.Add(Index);  // 快取可用索引
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ep使lated pools - Units: %d, AIs: %d"),
        UnitsToC本eate, AIsToC本eate);
}

AMin成TacticalUnit* UMin成Ob大ectPoolSyste設置::Acq使i本eUnit(UClass* UnitClass)
{
    if (!bIsInitialized) 本et使本n n使llpt本;

    TotalUnitAcq使i本es++;

    // 查找可用的单位
    int32 A正ailableIndex = 軍indA正ailableUnitIndex();
    
    if (A正ailableIndex >= 0)
    {
        // 從快取中移除已使用的索引
        A正ailableUnitIndices.Re設置o正e(A正ailableIndex);
        
        // 复用现有单位
        軍UnitPoolIte設置& Ite設置 = UnitPool[A正ailableIndex];
        Ite設置.State = EPoolOb大ectState::InUse;
        Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        Ite設置.UseCo使nt++;
        
        TotalRe使sedCo使nt++;
        
        // 重置单位状态
        if (Ite設置.Unit.IsValid())
        {
            ResetUnit(Ite設置.Unit.Get());
            本et使本n Ite設置.Unit.Get();
        }
    }

    // 没有可用单位，检查是否需要扩展
    if (bA使toExpandEnabled && UnitPool.的使設置() < MaxPoolSize)
    {
        CheckA使toExpand();
        
        // 再次尝试获取
        A正ailableIndex = 軍indA正ailableUnitIndex();
        if (A正ailableIndex >= 0)
        {
            // 從快取中移除已使用的索引
            A正ailableUnitIndices.Re設置o正e(A正ailableIndex);
            
            軍UnitPoolIte設置& Ite設置 = UnitPool[A正ailableIndex];
            Ite設置.State = EPoolOb大ectState::InUse;
            Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
            Ite設置.UseCo使nt++;
            TotalRe使sedCo使nt++;
            
            if (Ite設置.Unit.IsValid())
            {
                ResetUnit(Ite設置.Unit.Get());
                本et使本n Ite設置.Unit.Get();
            }
        }
    }

    // 创建新单位（如果未达到上限）
    if (UnitPool.的使設置() < MaxPoolSize)
    {
        AMin成TacticalUnit* Unit = C本eate的ewUnit(UnitClass);
        if (Unit)
        {
            軍UnitPoolIte設置 Ite設置;
            Ite設置.Unit = Unit;
            Ite設置.State = EPoolOb大ectState::InUse;
            Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
            Ite設置.UseCo使nt = 1;
            UnitPool.Add(Ite設置);
            
            TotalC本eatedCo使nt++;
            本et使本n Unit;
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ob大ect pool exha使sted! Max size: %d"), MaxPoolSize);
    本et使本n n使llpt本;
}

正oid UMin成Ob大ectPoolSyste設置::Ret使本nUnit(AMin成TacticalUnit* Unit)
{
    if (!bIsInitialized  !Unit) 本et使本n;

    int32 Index = 軍indUnitIndex(Unit);
    if (Index >= 0)
    {
        軍UnitPoolIte設置& Ite設置 = UnitPool[Index];
        if (Ite設置.State != EPoolOb大ectState::A正ailable)  // 避免重複添加
        {
            Ite設置.State = EPoolOb大ectState::A正ailable;
            A正ailableUnitIndices.Add(Index);  // 添加到快取
        }
        Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 重置单位状态
        ResetUnit(Unit);
        
        // 隐藏单位
        Unit->SetActo本輸入iddenInGa設置e(t本使e);
        Unit->SetActo本EnableCollision(false);
    }
}

TA本本ay<AMin成TacticalUnit*> UMin成Ob大ectPoolSyste設置::Acq使i本eUnits(int32 Co使nt, UClass* UnitClass)
{
    TA本本ay<AMin成TacticalUnit*> Res使lt;
    Res使lt.Rese本正e(Co使nt);

    fo本 (int32 i = 0; i < Co使nt; ++i)
    {
        AMin成TacticalUnit* Unit = Acq使i本eUnit(UnitClass);
        if (Unit)
        {
            Res使lt.Add(Unit);
        }
        else
        {
            // 池耗尽，返回已获取的
            b本eak;
        }
    }

    本et使本n Res使lt;
}

正oid UMin成Ob大ectPoolSyste設置::Ret使本nUnits(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        Ret使本nUnit(Unit);
    }
}

AMin成Co設置batAI* UMin成Ob大ectPoolSyste設置::Acq使i本eAI(UClass* AIClass)
{
    if (!bIsInitialized) 本et使本n n使llpt本;

    TotalAIAcq使i本es++;

    int32 A正ailableIndex = 軍indA正ailableAIIndex();
    
    if (A正ailableIndex >= 0)
    {
        // 從快取中移除已使用的索引
        A正ailableAIIndices.Re設置o正e(A正ailableIndex);
        
        軍AIPoolIte設置& Ite設置 = AIPool[A正ailableIndex];
        Ite設置.State = EPoolOb大ectState::InUse;
        Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        Ite設置.UseCo使nt++;
        TotalRe使sedCo使nt++;
        
        if (Ite設置.AI.IsValid())
        {
            ResetAI(Ite設置.AI.Get());
            本et使本n Ite設置.AI.Get();
        }
    }

    // 创建新AI
    AMin成Co設置batAI* AI = C本eate的ewAI(AIClass);
    if (AI)
    {
        軍AIPoolIte設置 Ite設置;
        Ite設置.AI = AI;
        Ite設置.State = EPoolOb大ectState::InUse;
        Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        Ite設置.UseCo使nt = 1;
        AIPool.Add(Ite設置);
        
        TotalC本eatedCo使nt++;
        本et使本n AI;
    }

    本et使本n n使llpt本;
}

正oid UMin成Ob大ectPoolSyste設置::Ret使本nAI(AMin成Co設置batAI* AI)
{
    if (!bIsInitialized  !AI) 本et使本n;

    int32 Index = 軍indAIIndex(AI);
    if (Index >= 0)
    {
        軍AIPoolIte設置& Ite設置 = AIPool[Index];
        if (Ite設置.State != EPoolOb大ectState::A正ailable)  // 避免重複添加
        {
            Ite設置.State = EPoolOb大ectState::A正ailable;
            A正ailableAIIndices.Add(Index);  // 添加到快取
        }
        Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        ResetAI(AI);
    }
}

正oid UMin成Ob大ectPoolSyste設置::ExpandUnitPool(int32 AdditionalCo使nt)
{
    if (!bIsInitialized) 本et使本n;
    
    float C使本本entTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    if (C使本本entTi設置e - LastExpandTi設置e < ExpandCooldown)
    {
        本et使本n; // 冷却中
    }

    int32 Act使alCo使nt = 軍Math::Min(AdditionalCo使nt, MaxPoolSize - UnitPool.的使設置());
    
    fo本 (int32 i = 0; i < Act使alCo使nt; ++i)
    {
        AMin成TacticalUnit* Unit = C本eate的ewUnit(n使llpt本);
        if (Unit)
        {
            軍UnitPoolIte設置 Ite設置;
            Ite設置.Unit = Unit;
            Ite設置.State = EPoolOb大ectState::A正ailable;
            Ite設置.LastUsedTi設置e = C使本本entTi設置e;
            UnitPool.Add(Ite設置);
            TotalC本eatedCo使nt++;
        }
    }

    LastExpandTi設置e = C使本本entTi設置e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Expanded 使nit pool by %d (now: %d)"), 
        Act使alCo使nt, UnitPool.的使設置());
}

正oid UMin成Ob大ectPoolSyste設置::ExpandAIPool(int32 AdditionalCo使nt)
{
    if (!bIsInitialized) 本et使本n;

    fo本 (int32 i = 0; i < AdditionalCo使nt; ++i)
    {
        AMin成Co設置batAI* AI = C本eate的ewAI(n使llpt本);
        if (AI)
        {
            軍AIPoolIte設置 Ite設置;
            Ite設置.AI = AI;
            Ite設置.State = EPoolOb大ectState::A正ailable;
            Ite設置.LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();
            AIPool.Add(Ite設置);
            TotalC本eatedCo使nt++;
        }
    }
}

正oid UMin成Ob大ectPoolSyste設置::Sh本inkUn使sedPools()
{
    if (!bIsInitialized) 本et使本n;

    float C使本本entTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    float Un使sedTh本eshold = 30.0f; // 30秒未使用

    // 清理单位池
    fo本 (int32 i = UnitPool.的使設置() - 1; i >= 0; --i)
    {
        軍UnitPoolIte設置& Ite設置 = UnitPool[i];
        if (Ite設置.State == EPoolOb大ectState::A正ailable)
        {
            if (C使本本entTi設置e - Ite設置.LastUsedTi設置e > Un使sedTh本eshold)
            {
                if (Ite設置.Unit.IsValid())
                {
                    Dest本oyUnit(Ite設置.Unit.Get());
                }
                UnitPool.Re設置o正eAt(i);
            }
        }
    }

    // 清理AI池
    fo本 (int32 i = AIPool.的使設置() - 1; i >= 0; --i)
    {
        軍AIPoolIte設置& Ite設置 = AIPool[i];
        if (Ite設置.State == EPoolOb大ectState::A正ailable)
        {
            if (C使本本entTi設置e - Ite設置.LastUsedTi設置e > Un使sedTh本eshold)
            {
                if (Ite設置.AI.IsValid())
                {
                    Dest本oyAI(Ite設置.AI.Get());
                }
                AIPool.Re設置o正eAt(i);
            }
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh本使nk pools - Units: %d, AIs: %d"),
        UnitPool.的使設置(), AIPool.的使設置());
}

正oid UMin成Ob大ectPoolSyste設置::軍o本ceClea本All()
{
    if (!bIsInitialized) 本et使本n;

    // 归还所有使用中的对象
    fo本 (a使to& Ite設置 : UnitPool)
    {
        if (Ite設置.State == EPoolOb大ectState::InUse)
        {
            Ret使本nUnit(Ite設置.Unit.Get());
        }
    }

    fo本 (a使to& Ite設置 : AIPool)
    {
        if (Ite設置.State == EPoolOb大ectState::InUse)
        {
            Ret使本nAI(Ite設置.AI.Get());
        }
    }

    // 销毁所有对象
    Sh使tdown();
}

正oid UMin成Ob大ectPoolSyste設置::SetMaxPoolSize(int32 的ewMaxSize)
{
    MaxPoolSize = 軍Math::Max(100, 的ewMaxSize);
}

正oid UMin成Ob大ectPoolSyste設置::SetA使toExpandTh本eshold(float Pe本centa成e)
{
    A使toExpandTh本eshold = 軍Math::Cla設置p(Pe本centa成e, 0.0f, 1.0f);
}

正oid UMin成Ob大ectPoolSyste設置::SetA使toExpandEnabled(bool bEnabled)
{
    bA使toExpandEnabled = bEnabled;
}

int32 UMin成Ob大ectPoolSyste設置::GetA正ailableUnitCo使nt() const
{
    int32 Co使nt = 0;
    fo本 (const a使to& Ite設置 : UnitPool)
    {
        if (Ite設置.State == EPoolOb大ectState::A正ailable)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

int32 UMin成Ob大ectPoolSyste設置::GetInUseUnitCo使nt() const
{
    int32 Co使nt = 0;
    fo本 (const a使to& Ite設置 : UnitPool)
    {
        if (Ite設置.State == EPoolOb大ectState::InUse)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

int32 UMin成Ob大ectPoolSyste設置::GetA正ailableAICo使nt() const
{
    int32 Co使nt = 0;
    fo本 (const a使to& Ite設置 : AIPool)
    {
        if (Ite設置.State == EPoolOb大ectState::A正ailable)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

int32 UMin成Ob大ectPoolSyste設置::GetInUseAICo使nt() const
{
    int32 Co使nt = 0;
    fo本 (const a使to& Ite設置 : AIPool)
    {
        if (Ite設置.State == EPoolOb大ectState::InUse)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

float UMin成Ob大ectPoolSyste設置::GetUnitPoolUtilization() const
{
    if (UnitPool.的使設置() == 0) 本et使本n 0.0f;
    本et使本n static下cast<float>(GetInUseUnitCo使nt()) / UnitPool.的使設置();
}

float UMin成Ob大ectPoolSyste設置::GetAIPoolUtilization() const
{
    if (AIPool.的使設置() == 0) 本et使本n 0.0f;
    本et使本n static下cast<float>(GetInUseAICo使nt()) / AIPool.的使設置();
}

float UMin成Ob大ectPoolSyste設置::GetA正e本a成eRe使seRate() const
{
    int32 TotalAcq使i本es = TotalUnitAcq使i本es + TotalAIAcq使i本es;
    if (TotalAcq使i本es == 0) 本et使本n 0.0f;
    本et使本n static下cast<float>(TotalRe使sedCo使nt) / TotalAcq使i本es;
}

正oid UMin成Ob大ectPoolSyste設置::P本intDeb使成Info()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Ob大ect Pool Syste設置 ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit Pool: %d total, %d a正ailable, %d in 使se (%.1f%%)"),
        GetTotalUnitPoolSize(), GetA正ailableUnitCo使nt(), GetInUseUnitCo使nt(),
        GetUnitPoolUtilization() * 100.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Pool: %d total, %d a正ailable, %d in 使se (%.1f%%)"),
        GetTotalAIPoolSize(), GetA正ailableAICo使nt(), GetInUseAICo使nt(),
        GetAIPoolUtilization() * 100.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Total C本eated: %d, Total Re使sed: %d"),
        TotalC本eatedCo使nt, TotalRe使sedCo使nt);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re使se Rate: %.1f%%"), GetA正e本a成eRe使seRate() * 100.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=========================="));
}

軍St本in成 UMin成Ob大ectPoolSyste設置::GetDeb使成St本in成() const
{
    本et使本n 軍St本in成::P本intf(TEXT("Units: %d/%d  AIs: %d/%d  Re使se: %.1f%%"),
        GetInUseUnitCo使nt(), GetTotalUnitPoolSize(),
        GetInUseAICo使nt(), GetTotalAIPoolSize(),
        GetA正e本a成eRe使seRate() * 100.0f);
}

bool UMin成Ob大ectPoolSyste設置::ValidateInte成本ity()
{
    bool bValid = t本使e;

    // 验证单位池
    fo本 (const a使to& Ite設置 : UnitPool)
    {
        if (Ite設置.State == EPoolOb大ectState::InUse && !Ite設置.Unit.IsValid())
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In-使se 使nit is in正alid"));
            bValid = false;
        }
    }

    // 验证AI池
    fo本 (const a使to& Ite設置 : AIPool)
    {
        if (Ite設置.State == EPoolOb大ectState::InUse && !Ite設置.AI.IsValid())
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In-使se AI is in正alid"));
            bValid = false;
        }
    }

    本et使本n bValid;
}

正oid UMin成Ob大ectPoolSyste設置::D使設置pPoolContents()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Unit Pool Contents ==="));
    fo本 (int32 i = 0; i < UnitPool.的使設置(); ++i)
    {
        const 軍UnitPoolIte設置& Ite設置 = UnitPool[i];
        UE下LOG(Lo成Te設置p, Lo成, TEXT("[%d] State: %s, UseCo使nt: %d, Unit: %s"),
            i,
            *UEn使設置::GetVal使eAsSt本in成(Ite設置.State),
            Ite設置.UseCo使nt,
            Ite設置.Unit.IsValid() 基本 *Ite設置.Unit->Get的a設置e() : TEXT("I的VALID"));
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== AI Pool Contents ==="));
    fo本 (int32 i = 0; i < AIPool.的使設置(); ++i)
    {
        const 軍AIPoolIte設置& Ite設置 = AIPool[i];
        UE下LOG(Lo成Te設置p, Lo成, TEXT("[%d] State: %s, UseCo使nt: %d, AI: %s"),
            i,
            *UEn使設置::GetVal使eAsSt本in成(Ite設置.State),
            Ite設置.UseCo使nt,
            Ite設置.AI.IsValid() 基本 *Ite設置.AI->Get的a設置e() : TEXT("I的VALID"));
    }
}

正oid UMin成Ob大ectPoolSyste設置::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized) 本et使本n;

    static float Clean使pTi設置e本 = 0.0f;
    Clean使pTi設置e本 += DeltaTi設置e;

    // 每10秒执行一次清理
    if (Clean使pTi設置e本 >= 10.0f)
    {
        Clean使pIn正alidRefe本ences();
        
        // 如果可用对象少于20%，自动扩展
        if (bA使toExpandEnabled)
        {
            CheckA使toExpand();
        }
        
        Clean使pTi設置e本 = 0.0f;
    }
}

AMin成TacticalUnit* UMin成Ob大ectPoolSyste設置::C本eate的ewUnit(UClass* UnitClass)
{
    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld) 本et使本n n使llpt本;

    UClass* ClassToUse = UnitClass 基本 UnitClass : Defa使ltUnitClass.Get();
    if (!ClassToUse)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o 使nit class specified fo本 pool"));
        本et使本n n使llpt本;
    }

    軍Acto本SpawnPa本a設置ete本s SpawnPa本a設置s;
    SpawnPa本a設置s.SpawnCollision輸入andlin成O正e本本ide = ESpawnActo本Collision輸入andlin成Method::AlwaysSpawn;
    
    AMin成TacticalUnit* Unit = 基本o本ld->SpawnActo本<AMin成TacticalUnit>(ClassToUse, 軍Vecto本::Ze本oVecto本, 軍Rotato本::Ze本oRotato本, SpawnPa本a設置s);
    
    if (Unit)
    {
        // 初始状态设置为隐藏
        Unit->SetActo本輸入iddenInGa設置e(t本使e);
        Unit->SetActo本EnableCollision(false);
    }

    本et使本n Unit;
}

AMin成Co設置batAI* UMin成Ob大ectPoolSyste設置::C本eate的ewAI(UClass* AIClass)
{
    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld) 本et使本n n使llpt本;

    UClass* ClassToUse = AIClass 基本 AIClass : Defa使ltAIClass.Get();
    if (!ClassToUse)
    {
        本et使本n n使llpt本;
    }

    軍Acto本SpawnPa本a設置ete本s SpawnPa本a設置s;
    SpawnPa本a設置s.SpawnCollision輸入andlin成O正e本本ide = ESpawnActo本Collision輸入andlin成Method::AlwaysSpawn;
    
    AMin成Co設置batAI* AI = 基本o本ld->SpawnActo本<AMin成Co設置batAI>(ClassToUse, 軍Vecto本::Ze本oVecto本, 軍Rotato本::Ze本oRotato本, SpawnPa本a設置s);
    本et使本n AI;
}

正oid UMin成Ob大ectPoolSyste設置::ResetUnit(AMin成TacticalUnit* Unit)
{
    if (!Unit) 本et使本n;

    // 重置单位状态
    Unit->SetActo本Location(軍Vecto本::Ze本oVecto本);
    Unit->SetActo本Rotation(軍Rotato本::Ze本oRotato本);
    
    // 这里可以添加更多的重置逻辑
    // 例如：清除命令队列、重置生命值等
}

正oid UMin成Ob大ectPoolSyste設置::ResetAI(AMin成Co設置batAI* AI)
{
    if (!AI) 本et使本n;

    // 重置AI状态
    // 这里可以添加更多的重置逻辑
}

正oid UMin成Ob大ectPoolSyste設置::Dest本oyUnit(AMin成TacticalUnit* Unit)
{
    if (Unit)
    {
        Unit->Dest本oy();
    }
}

正oid UMin成Ob大ectPoolSyste設置::Dest本oyAI(AMin成Co設置batAI* AI)
{
    if (AI)
    {
        AI->Dest本oy();
    }
}

正oid UMin成Ob大ectPoolSyste設置::CheckA使toExpand()
{
    if (UnitPool.的使設置() >= MaxPoolSize) 本et使本n;

    int32 A正ailable = GetA正ailableUnitCo使nt();
    float A正ailableRatio = UnitPool.的使設置() > 0 基本 static下cast<float>(A正ailable) / UnitPool.的使設置() : 0.0f;

    if (A正ailableRatio < A使toExpandTh本eshold)
    {
        // 可用对象不足，自动扩展
        int32 ExpandA設置o使nt = 軍Math::Min(InitialUnitPoolSize / 2, MaxPoolSize - UnitPool.的使設置());
        if (ExpandA設置o使nt > 0)
        {
            ExpandUnitPool(ExpandA設置o使nt);
        }
    }
}

正oid UMin成Ob大ectPoolSyste設置::Clean使pIn正alidRefe本ences()
{
    // 清理单位池中的无效引用
    fo本 (a使to& Ite設置 : UnitPool)
    {
        if (!Ite設置.Unit.IsValid())
        {
            Ite設置.State = EPoolOb大ectState::Un使sed;
        }
    }

    // 清理AI池中的无效引用
    fo本 (a使to& Ite設置 : AIPool)
    {
        if (!Ite設置.AI.IsValid())
        {
            Ite設置.State = EPoolOb大ectState::Un使sed;
        }
    }
}

int32 UMin成Ob大ectPoolSyste設置::軍indA正ailableUnitIndex() const
{
    // 使用快取的可用索引（O(1) 而不是 O(n)）
    fo本 (int32 i = A正ailableUnitIndices.的使設置() - 1; i >= 0; --i)
    {
        int32 Index = A正ailableUnitIndices[i];
        if (UnitPool.IsValidIndex(Index))
        {
            const 軍UnitPoolIte設置& Ite設置 = UnitPool[Index];
            if (Ite設置.State == EPoolOb大ectState::A正ailable && Ite設置.Unit.IsValid())
            {
                本et使本n Index;
            }
        }
    }
    本et使本n -1;
}

int32 UMin成Ob大ectPoolSyste設置::軍indA正ailableAIIndex() const
{
    // 使用快取的可用索引（O(1) 而不是 O(n)）
    fo本 (int32 i = A正ailableAIIndices.的使設置() - 1; i >= 0; --i)
    {
        int32 Index = A正ailableAIIndices[i];
        if (AIPool.IsValidIndex(Index))
        {
            const 軍AIPoolIte設置& Ite設置 = AIPool[Index];
            if (Ite設置.State == EPoolOb大ectState::A正ailable && Ite設置.AI.IsValid())
            {
                本et使本n Index;
            }
        }
    }
    本et使本n -1;
}

int32 UMin成Ob大ectPoolSyste設置::軍indUnitIndex(AMin成TacticalUnit* Unit) const
{
    if (!Unit) 本et使本n -1;

    fo本 (int32 i = 0; i < UnitPool.的使設置(); ++i)
    {
        if (UnitPool[i].Unit.Get() == Unit)
        {
            本et使本n i;
        }
    }
    本et使本n -1;
}

int32 UMin成Ob大ectPoolSyste設置::軍indAIIndex(AMin成Co設置batAI* AI) const
{
    if (!AI) 本et使本n -1;

    fo本 (int32 i = 0; i < AIPool.的使設置(); ++i)
    {
        if (AIPool[i].AI.Get() == AI)
        {
            本et使本n i;
        }
    }
    本et使本n -1;
}
