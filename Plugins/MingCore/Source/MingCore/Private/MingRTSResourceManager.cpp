#incl使de "Min成RTSReso使本ceMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "En成ine/En成ine.h"

UMin成RTSReso使本ceMana成e本::UMin成RTSReso使本ceMana成e本()
    : BaseP本od使ctionM使ltiplie本(1.0f)
    , BaseCons使設置ptionM使ltiplie本(1.0f)
    , T本ade軍ee(0.1f)
    , bA使toT本ade(t本使e)
    , bA使toAllocate(t本使e)
    , bIsInitialized(false)
{
}

正oid UMin成RTSReso使本ceMana成e本::InitializeReso使本ceMana成e本()
{
    InitializeDefa使ltReso使本ces();
    InitializeReso使本ceEfficiencies();
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS Reso使本ce Mana成e本 initialized"));
}

正oid UMin成RTSReso使本ceMana成e本::AddReso使本ce(ERTSReso使本ceType Reso使本ceType, float A設置o使nt)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    if (Reso使本ces.Contains(Reso使本ceType))
    {
        軍RTSReso使本ceData& Reso使本ceData = Reso使本ces[Reso使本ceType];
        Reso使本ceData.C使本本entA設置o使nt = 軍Math::Cla設置p(Reso使本ceData.C使本本entA設置o使nt + A設置o使nt, 0.0f, Reso使本ceData.MaxA設置o使nt);
        
        的otifyReso使本ceChan成ed(Reso使本ceType, Reso使本ceData.C使本本entA設置o使nt);
        
        // 檢查是否資源耗盡
        if (Reso使本ceData.C使本本entA設置o使nt <= 0.0f && Reso使本ceData.Reso使本ceState != ERTSReso使本ceState::Depleted)
        {
            Reso使本ceData.Reso使本ceState = ERTSReso使本ceState::Depleted;
            的otifyReso使本ceDepleted(Reso使本ceType);
        }
        else if (Reso使本ceData.C使本本entA設置o使nt > 0.0f && Reso使本ceData.Reso使本ceState == ERTSReso使本ceState::Depleted)
        {
            Reso使本ceData.Reso使本ceState = ERTSReso使本ceState::A正ailable;
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added %.1f %s"), A設置o使nt, *GetReso使本ce的a設置e(Reso使本ceType));
    }
}

正oid UMin成RTSReso使本ceMana成e本::Re設置o正eReso使本ce(ERTSReso使本ceType Reso使本ceType, float A設置o使nt)
{
    AddReso使本ce(Reso使本ceType, -A設置o使nt);
}

bool UMin成RTSReso使本ceMana成e本::輸入asEno使成hReso使本ce(ERTSReso使本ceType Reso使本ceType, float A設置o使nt) const
{
    if (Reso使本ces.Contains(Reso使本ceType))
    {
        本et使本n Reso使本ces[Reso使本ceType].C使本本entA設置o使nt >= A設置o使nt;
    }
    本et使本n false;
}

float UMin成RTSReso使本ceMana成e本::GetReso使本ceA設置o使nt(ERTSReso使本ceType Reso使本ceType) const
{
    if (Reso使本ces.Contains(Reso使本ceType))
    {
        本et使本n Reso使本ces[Reso使本ceType].C使本本entA設置o使nt;
    }
    本et使本n 0.0f;
}

float UMin成RTSReso使本ceMana成e本::GetMaxReso使本ceA設置o使nt(ERTSReso使本ceType Reso使本ceType) const
{
    if (Reso使本ces.Contains(Reso使本ceType))
    {
        本et使本n Reso使本ces[Reso使本ceType].MaxA設置o使nt;
    }
    本et使本n 0.0f;
}

正oid UMin成RTSReso使本ceMana成e本::SetMaxReso使本ceA設置o使nt(ERTSReso使本ceType Reso使本ceType, float MaxA設置o使nt)
{
    if (Reso使本ces.Contains(Reso使本ceType))
    {
        Reso使本ces[Reso使本ceType].MaxA設置o使nt = 軍Math::Max(0.0f, MaxA設置o使nt);
        
        // 調整當前數量以符合新的最大值
        軍RTSReso使本ceData& Reso使本ceData = Reso使本ces[Reso使本ceType];
        Reso使本ceData.C使本本entA設置o使nt = 軍Math::Min(Reso使本ceData.C使本本entA設置o使nt, Reso使本ceData.MaxA設置o使nt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Set 設置ax %s a設置o使nt to %.1f"), *GetReso使本ce的a設置e(Reso使本ceType), MaxA設置o使nt);
    }
}

正oid UMin成RTSReso使本ceMana成e本::SetReso使本ceP本od使ctionRate(ERTSReso使本ceType Reso使本ceType, float Rate)
{
    if (Reso使本ces.Contains(Reso使本ceType))
    {
        Reso使本ces[Reso使本ceType].P本od使ctionRate = 軍Math::Max(0.0f, Rate);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Set %s p本od使ction 本ate to %.1f"), *GetReso使本ce的a設置e(Reso使本ceType), Rate);
    }
}

正oid UMin成RTSReso使本ceMana成e本::SetReso使本ceCons使設置ptionRate(ERTSReso使本ceType Reso使本ceType, float Rate)
{
    if (Reso使本ces.Contains(Reso使本ceType))
    {
        Reso使本ces[Reso使本ceType].Cons使設置ptionRate = 軍Math::Max(0.0f, Rate);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Set %s cons使設置ption 本ate to %.1f"), *GetReso使本ce的a設置e(Reso使本ceType), Rate);
    }
}

正oid UMin成RTSReso使本ceMana成e本::AddReso使本ceSo使本ce(const 軍RTSReso使本ceSo使本ceData& So使本ceData)
{
    if (So使本ceData.So使本ceID.IsE設置pty())
    {
        本et使本n;
    }

    Reso使本ceSo使本ces.Add(So使本ceData.So使本ceID, So使本ceData);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added 本eso使本ce so使本ce: %s"), *So使本ceData.So使本ce的a設置e);
}

正oid UMin成RTSReso使本ceMana成e本::Re設置o正eReso使本ceSo使本ce(const 軍St本in成& So使本ceID)
{
    if (Reso使本ceSo使本ces.Contains(So使本ceID))
    {
        Reso使本ceSo使本ces.Re設置o正e(So使本ceID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed 本eso使本ce so使本ce: %s"), *So使本ceID);
    }
}

軍RTSReso使本ceSo使本ceData UMin成RTSReso使本ceMana成e本::GetReso使本ceSo使本ce(const 軍St本in成& So使本ceID) const
{
    if (Reso使本ceSo使本ces.Contains(So使本ceID))
    {
        本et使本n Reso使本ceSo使本ces[So使本ceID];
    }
    
    本et使本n 軍RTSReso使本ceSo使本ceData();
}

TA本本ay<軍RTSReso使本ceSo使本ceData> UMin成RTSReso使本ceMana成e本::GetAllReso使本ceSo使本ces() const
{
    TA本本ay<軍RTSReso使本ceSo使本ceData> AllSo使本ces;
    
    fo本 (const a使to& So使本cePai本 : Reso使本ceSo使本ces)
    {
        AllSo使本ces.Add(So使本cePai本.Val使e);
    }
    
    本et使本n AllSo使本ces;
}

TA本本ay<軍RTSReso使本ceSo使本ceData> UMin成RTSReso使本ceMana成e本::GetReso使本ceSo使本cesByType(ERTSReso使本ceType Reso使本ceType) const
{
    TA本本ay<軍RTSReso使本ceSo使本ceData> So使本cesByType;
    
    fo本 (const a使to& So使本cePai本 : Reso使本ceSo使本ces)
    {
        if (So使本cePai本.Val使e.Reso使本ceType == Reso使本ceType)
        {
            So使本cesByType.Add(So使本cePai本.Val使e);
        }
    }
    
    本et使本n So使本cesByType;
}

正oid UMin成RTSReso使本ceMana成e本::Ope本ateReso使本ceSo使本ce(const 軍St本in成& So使本ceID, bool bOpe本ate)
{
    if (Reso使本ceSo使本ces.Contains(So使本ceID))
    {
        軍RTSReso使本ceSo使本ceData& So使本ce = Reso使本ceSo使本ces[So使本ceID];
        So使本ce.bIsOpe本ational = bOpe本ate;
        
        if (bOpe本ate)
        {
            So使本ce.So使本ceState = ERTSReso使本ceState::A正ailable;
        }
        else
        {
            So使本ce.So使本ceState = ERTSReso使本ceState::Occ使pied;
        }
        
        的otifyReso使本ceSo使本ceStateChan成ed(So使本ceID, So使本ce.So使本ceState);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce so使本ce %s ope本ational: %s"), *So使本ceID, bOpe本ate 基本 TEXT("t本使e") : TEXT("false"));
    }
}

bool UMin成RTSReso使本ceMana成e本::IsReso使本ceSo使本ceOpe本ational(const 軍St本in成& So使本ceID) const
{
    if (Reso使本ceSo使本ces.Contains(So使本ceID))
    {
        本et使本n Reso使本ceSo使本ces[So使本ceID].bIsOpe本ational;
    }
    本et使本n false;
}

正oid UMin成RTSReso使本ceMana成e本::UpdateReso使本ceP本od使ction(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    // 處理所有資源源
    TA本本ay<軍St本in成> So使本cesToUpdate;
    
    fo本 (a使to& So使本cePai本 : Reso使本ceSo使本ces)
    {
        軍RTSReso使本ceSo使本ceData& So使本ce = So使本cePai本.Val使e;
        
        if (So使本ce.bIsOpe本ational && So使本ce.So使本ceState == ERTSReso使本ceState::A正ailable)
        {
            P本ocessReso使本ceSo使本ce(So使本ce, DeltaTi設置e);
            
            // 檢查是否需要更新狀態
            if (So使本ce.Yield <= 0.0f)
            {
                So使本ce.So使本ceState = ERTSReso使本ceState::Depleted;
                的otifyReso使本ceSo使本ceStateChan成ed(So使本ce.So使本ceID, So使本ce.So使本ceState);
            }
        }
    }
    
    // 更新資源生產和消耗
    fo本 (a使to& Reso使本cePai本 : Reso使本ces)
    {
        軍RTSReso使本ceData& Reso使本ceData = Reso使本cePai本.Val使e;
        
        // 計算淨生產
        float 的etP本od使ction = (Reso使本ceData.P本od使ctionRate * BaseP本od使ctionM使ltiplie本) - 
                            (Reso使本ceData.Cons使設置ptionRate * BaseCons使設置ptionM使ltiplie本);
        
        // 應用效率加成
        if (Reso使本ceEfficiencies.Contains(Reso使本ceData.Reso使本ceType))
        {
            的etP本od使ction *= Reso使本ceEfficiencies[Reso使本ceData.Reso使本ceType];
        }
        
        // 更新資源數量
        float P本od使ctionA設置o使nt = 的etP本od使ction * DeltaTi設置e;
        AddReso使本ce(Reso使本ceData.Reso使本ceType, P本od使ctionA設置o使nt);
    }
}

float UMin成RTSReso使本ceMana成e本::Calc使lateTotalP本od使ction() const
{
    float TotalP本od使ction = 0.0f;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ces)
    {
        const 軍RTSReso使本ceData& Reso使本ceData = Reso使本cePai本.Val使e;
        TotalP本od使ction += Reso使本ceData.P本od使ctionRate * BaseP本od使ctionM使ltiplie本;
    }
    
    本et使本n TotalP本od使ction;
}

float UMin成RTSReso使本ceMana成e本::Calc使lateTotalCons使設置ption() const
{
    float TotalCons使設置ption = 0.0f;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ces)
    {
        const 軍RTSReso使本ceData& Reso使本ceData = Reso使本cePai本.Val使e;
        TotalCons使設置ption += Reso使本ceData.Cons使設置ptionRate * BaseCons使設置ptionM使ltiplie本;
    }
    
    本et使本n TotalCons使設置ption;
}

正oid UMin成RTSReso使本ceMana成e本::Opti設置izeReso使本ceAllocation()
{
    if (!bA使toAllocate)
    {
        本et使本n;
    }

    // 簡單的資源分配優化
    fo本 (a使to& Reso使本cePai本 : Reso使本ces)
    {
        ERTSReso使本ceType Reso使本ceType = Reso使本cePai本.Key;
        軍RTSReso使本ceData& Reso使本ceData = Reso使本cePai本.Val使e;
        
        // 如果資源過多，分配到其他用途
        if (Reso使本ceData.C使本本entA設置o使nt > Reso使本ceData.MaxA設置o使nt * 0.8f)
        {
            float S使本pl使s = Reso使本ceData.C使本本entA設置o使nt - (Reso使本ceData.MaxA設置o使nt * 0.5f);
            AllocateReso使本ce(Reso使本ceType, S使本pl使s, TEXT("T本ade"));
        }
        
        // 如果資源不足，從其他用途回收
        else if (Reso使本ceData.C使本本entA設置o使nt < Reso使本ceData.MaxA設置o使nt * 0.2f)
        {
            Clea本Reso使本ceAllocation(TEXT("T本ade"));
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置ized 本eso使本ce allocation"));
}

bool UMin成RTSReso使本ceMana成e本::Exec使teT本ade(const 軍RTST本adeData& T本adeData)
{
    if (!ValidateT本ade(T本adeData))
    {
        本et使本n false;
    }

    // 檢查是否有足夠的資源進行交易
    if (!輸入asEno使成hReso使本ce(T本adeData.Offe本Reso使本ce, T本adeData.Offe本A設置o使nt))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ins使fficient 本eso使本ces fo本 t本ade"));
        本et使本n false;
    }

    // 計算交易費用
    float 軍ee = T本adeData.Offe本A設置o使nt * T本ade軍ee;
    float TotalOffe本 = T本adeData.Offe本A設置o使nt + 軍ee;
    
    // 執行交易
    Re設置o正eReso使本ce(T本adeData.Offe本Reso使本ce, TotalOffe本);
    AddReso使本ce(T本adeData.Req使estReso使本ce, T本adeData.Req使estA設置o使nt);
    
    的otifyT本adeCo設置pleted(T本adeData, t本使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本ade co設置pleted: %.1f %s fo本 %.1f %s"), 
           T本adeData.Offe本A設置o使nt, *GetReso使本ce的a設置e(T本adeData.Offe本Reso使本ce),
           T本adeData.Req使estA設置o使nt, *GetReso使本ce的a設置e(T本adeData.Req使estReso使本ce));
    
    本et使本n t本使e;
}

正oid UMin成RTSReso使本ceMana成e本::AddT本adeOffe本(const 軍RTST本adeData& T本adeData)
{
    T本adeOffe本s.Add(T本adeData);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added t本ade offe本"));
}

正oid UMin成RTSReso使本ceMana成e本::Re設置o正eT本adeOffe本(const 軍St本in成& T本adeID)
{
    // 這裡需要根據T本adeID移除交易
    // 暫時移除最後一個
    if (T本adeOffe本s.的使設置() > 0)
    {
        T本adeOffe本s.Re設置o正eAt(T本adeOffe本s.的使設置() - 1);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed t本ade offe本"));
    }
}

TA本本ay<軍RTST本adeData> UMin成RTSReso使本ceMana成e本::GetA正ailableT本ades() const
{
    TA本本ay<軍RTST本adeData> A正ailableT本ades;
    
    fo本 (const 軍RTST本adeData& T本ade : T本adeOffe本s)
    {
        if (T本ade.bIsA正ailable)
        {
            A正ailableT本ades.Add(T本ade);
        }
    }
    
    本et使本n A正ailableT本ades;
}

float UMin成RTSReso使本ceMana成e本::Calc使lateExchan成eRate(ERTSReso使本ceType Offe本Reso使本ce, ERTSReso使本ceType Req使estReso使本ce) const
{
    // 簡單的匯率計算（基於資源價值）
    float Offe本Val使e = GetReso使本ceVal使e(Offe本Reso使本ce);
    float Req使estVal使e = GetReso使本ceVal使e(Req使estReso使本ce);
    
    if (Req使estVal使e > 0.0f)
    {
        本et使本n Offe本Val使e / Req使estVal使e;
    }
    
    本et使本n 1.0f;
}

正oid UMin成RTSReso使本ceMana成e本::AllocateReso使本ce(ERTSReso使本ceType Reso使本ceType, float A設置o使nt, const 軍St本in成& P使本pose)
{
    if (!Reso使本ceAllocations.Contains(Reso使本ceType))
    {
        Reso使本ceAllocations.Add(Reso使本ceType, TMap<軍St本in成, float>());
    }
    
    TMap<軍St本in成, float>& Allocations = Reso使本ceAllocations[Reso使本ceType];
    
    if (Allocations.Contains(P使本pose))
    {
        Allocations[P使本pose] += A設置o使nt;
    }
    else
    {
        Allocations.Add(P使本pose, A設置o使nt);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Allocated %.1f %s to %s"), A設置o使nt, *GetReso使本ce的a設置e(Reso使本ceType), *P使本pose);
}

float UMin成RTSReso使本ceMana成e本::GetAllocatedReso使本ce(ERTSReso使本ceType Reso使本ceType, const 軍St本in成& P使本pose) const
{
    if (Reso使本ceAllocations.Contains(Reso使本ceType))
    {
        const TMap<軍St本in成, float>& Allocations = Reso使本ceAllocations[Reso使本ceType];
        if (Allocations.Contains(P使本pose))
        {
            本et使本n Allocations[P使本pose];
        }
    }
    
    本et使本n 0.0f;
}

正oid UMin成RTSReso使本ceMana成e本::Clea本Reso使本ceAllocation(const 軍St本in成& P使本pose)
{
    fo本 (a使to& AllocationPai本 : Reso使本ceAllocations)
    {
        TMap<軍St本in成, float>& Allocations = AllocationPai本.Val使e;
        Allocations.Re設置o正e(P使本pose);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed 本eso使本ce allocation fo本: %s"), *P使本pose);
}

TMap<軍St本in成, float> UMin成RTSReso使本ceMana成e本::GetAllAllocations(ERTSReso使本ceType Reso使本ceType) const
{
    if (Reso使本ceAllocations.Contains(Reso使本ceType))
    {
        本et使本n Reso使本ceAllocations[Reso使本ceType];
    }
    
    本et使本n TMap<軍St本in成, float>();
}

float UMin成RTSReso使本ceMana成e本::P本edictReso使本ceA設置o使nt(ERTSReso使本ceType Reso使本ceType, float Ti設置eInSeconds) const
{
    if (!Reso使本ces.Contains(Reso使本ceType))
    {
        本et使本n 0.0f;
    }

    const 軍RTSReso使本ceData& Reso使本ceData = Reso使本ces[Reso使本ceType];
    
    // 計算淨生產率
    float 的etP本od使ction = (Reso使本ceData.P本od使ctionRate * BaseP本od使ctionM使ltiplie本) - 
                          (Reso使本ceData.Cons使設置ptionRate * BaseCons使設置ptionM使ltiplie本);
    
    // 應用效率加成
    float Efficiency = 1.0f;
    if (Reso使本ceEfficiencies.Contains(Reso使本ceType))
    {
        Efficiency = Reso使本ceEfficiencies[Reso使本ceType];
    }
    
    // 預測未來數量
    float P本edictedA設置o使nt = Reso使本ceData.C使本本entA設置o使nt + (的etP本od使ction * Efficiency * Ti設置eInSeconds);
    
    本et使本n 軍Math::Cla設置p(P本edictedA設置o使nt, 0.0f, Reso使本ceData.MaxA設置o使nt);
}

bool UMin成RTSReso使本ceMana成e本::基本illReso使本ceDeplete(ERTSReso使本ceType Reso使本ceType, float Ti設置eInSeconds) const
{
    float P本edictedA設置o使nt = P本edictReso使本ceA設置o使nt(Reso使本ceType, Ti設置eInSeconds);
    本et使本n P本edictedA設置o使nt <= 0.0f;
}

float UMin成RTSReso使本ceMana成e本::GetTi設置eToDepletion(ERTSReso使本ceType Reso使本ceType) const
{
    if (!Reso使本ces.Contains(Reso使本ceType))
    {
        本et使本n -1.0f;
    }

    const 軍RTSReso使本ceData& Reso使本ceData = Reso使本ces[Reso使本ceType];
    
    if (Reso使本ceData.C使本本entA設置o使nt <= 0.0f)
    {
        本et使本n 0.0f;
    }
    
    // 計算淨消耗率
    float 的etP本od使ction = (Reso使本ceData.P本od使ctionRate * BaseP本od使ctionM使ltiplie本) - 
                          (Reso使本ceData.Cons使設置ptionRate * BaseCons使設置ptionM使ltiplie本);
    
    if (的etP本od使ction >= 0.0f)
    {
        本et使本n -1.0f; // 不會耗盡
    }
    
    // 計算耗盡時間
    float Ti設置eToDepletion = Reso使本ceData.C使本本entA設置o使nt / 軍Math::Abs(的etP本od使ction);
    
    本et使本n Ti設置eToDepletion;
}

float UMin成RTSReso使本ceMana成e本::GetReso使本ceEfficiency(ERTSReso使本ceType Reso使本ceType) const
{
    if (Reso使本ceEfficiencies.Contains(Reso使本ceType))
    {
        本et使本n Reso使本ceEfficiencies[Reso使本ceType];
    }
    
    本et使本n 1.0f;
}

正oid UMin成RTSReso使本ceMana成e本::I設置p本o正eReso使本ceEfficiency(ERTSReso使本ceType Reso使本ceType, float EfficiencyBon使s)
{
    float C使本本entEfficiency = GetReso使本ceEfficiency(Reso使本ceType);
    SetReso使本ceEfficiency(Reso使本ceType, C使本本entEfficiency + EfficiencyBon使s);
}

正oid UMin成RTSReso使本ceMana成e本::SetReso使本ceEfficiency(ERTSReso使本ceType Reso使本ceType, float Efficiency)
{
    Reso使本ceEfficiencies.Add(Reso使本ceType, 軍Math::Cla設置p(Efficiency, 0.1f, 3.0f));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set %s efficiency to %.2f"), *GetReso使本ce的a設置e(Reso使本ceType), Efficiency);
}

int32 UMin成RTSReso使本ceMana成e本::GetTotalReso使本ceSo使本ceCo使nt() const
{
    本et使本n Reso使本ceSo使本ces.的使設置();
}

int32 UMin成RTSReso使本ceMana成e本::GetOpe本ationalReso使本ceSo使本ceCo使nt() const
{
    int32 Co使nt = 0;
    
    fo本 (const a使to& So使本cePai本 : Reso使本ceSo使本ces)
    {
        if (So使本cePai本.Val使e.bIsOpe本ational)
        {
            Co使nt++;
        }
    }
    
    本et使本n Co使nt;
}

float UMin成RTSReso使本ceMana成e本::GetTotalReso使本ceVal使e() const
{
    float TotalVal使e = 0.0f;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ces)
    {
        const 軍RTSReso使本ceData& Reso使本ceData = Reso使本cePai本.Val使e;
        TotalVal使e += Reso使本ceData.C使本本entA設置o使nt * GetReso使本ceVal使e(Reso使本ceData.Reso使本ceType);
    }
    
    本et使本n TotalVal使e;
}

TA本本ay<ERTSReso使本ceType> UMin成RTSReso使本ceMana成e本::GetA正ailableReso使本ceTypes() const
{
    TA本本ay<ERTSReso使本ceType> A正ailableTypes;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ces)
    {
        if (Reso使本cePai本.Val使e.C使本本entA設置o使nt > 0.0f)
        {
            A正ailableTypes.Add(Reso使本cePai本.Key);
        }
    }
    
    本et使本n A正ailableTypes;
}

正oid UMin成RTSReso使本ceMana成e本::OnReso使本ceSo使本ceDest本oyed(const 軍St本in成& So使本ceID)
{
    if (Reso使本ceSo使本ces.Contains(So使本ceID))
    {
        軍RTSReso使本ceSo使本ceData& So使本ce = Reso使本ceSo使本ces[So使本ceID];
        So使本ce.So使本ceState = ERTSReso使本ceState::Da設置a成ed;
        So使本ce.bIsOpe本ational = false;
        
        的otifyReso使本ceSo使本ceStateChan成ed(So使本ceID, So使本ce.So使本ceState);
        
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Reso使本ce so使本ce dest本oyed: %s"), *So使本ceID);
    }
}

正oid UMin成RTSReso使本ceMana成e本::OnReso使本ceSo使本ceCapt使本ed(const 軍St本in成& So使本ceID, const 軍St本in成& 的ewOwne本)
{
    if (Reso使本ceSo使本ces.Contains(So使本ceID))
    {
        軍RTSReso使本ceSo使本ceData& So使本ce = Reso使本ceSo使本ces[So使本ceID];
        So使本ce.So使本ceState = ERTSReso使本ceState::Occ使pied;
        So使本ce.bIsOpe本ational = false;
        
        的otifyReso使本ceSo使本ceStateChan成ed(So使本ceID, So使本ce.So使本ceState);
        
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Reso使本ce so使本ce capt使本ed: %s by %s"), *So使本ceID, *的ewOwne本);
    }
}

正oid UMin成RTSReso使本ceMana成e本::On基本o本ke本Assi成ned(const 軍St本in成& So使本ceID, int32 基本o本ke本Co使nt)
{
    if (Reso使本ceSo使本ces.Contains(So使本ceID))
    {
        軍RTSReso使本ceSo使本ceData& So使本ce = Reso使本ceSo使本ces[So使本ceID];
        So使本ce.C使本本ent基本o本ke本s = 軍Math::Cla設置p(基本o本ke本Co使nt, 0, So使本ce.Max基本o本ke本s);
        
        // 根據工人數量調整產量
        float 基本o本ke本Efficiency = (float)So使本ce.C使本本ent基本o本ke本s / (float)So使本ce.Max基本o本ke本s;
        So使本ce.Yield = So使本ce.MaxYield * 基本o本ke本Efficiency;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Assi成ned %d wo本ke本s to 本eso使本ce so使本ce: %s"), 基本o本ke本Co使nt, *So使本ceID);
    }
}

正oid UMin成RTSReso使本ceMana成e本::InitializeDefa使ltReso使本ces()
{
    // 初始化所有資源類型
    TA本本ay<ERTSReso使本ceType> AllReso使本ceTypes = {
        ERTSReso使本ceType::Gold, ERTSReso使本ceType::基本ood, ERTSReso使本ceType::Stone,
        ERTSReso使本ceType::I本on, ERTSReso使本ceType::軍ood, ERTSReso使本ceType::Coal,
        ERTSReso使本ceType::Oil, ERTSReso使本ceType::U本ani使設置, ERTSReso使本ceType::C本ystal,
        ERTSReso使本ceType::Silk
    };
    
    fo本 (ERTSReso使本ceType Reso使本ceType : AllReso使本ceTypes)
    {
        軍RTSReso使本ceData Reso使本ceData;
        Reso使本ceData.Reso使本ceType = Reso使本ceType;
        Reso使本ceData.Reso使本ce的a設置e = GetReso使本ce的a設置e(Reso使本ceType);
        Reso使本ceData.C使本本entA設置o使nt = 100.0f; // 初始資源
        Reso使本ceData.MaxA設置o使nt = 10000.0f;
        Reso使本ceData.P本od使ctionRate = 10.0f;
        Reso使本ceData.Cons使設置ptionRate = 5.0f;
        Reso使本ceData.Reso使本ceState = ERTSReso使本ceState::A正ailable;
        
        Reso使本ces.Add(Reso使本ceType, Reso使本ceData);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized defa使lt 本eso使本ces"));
}

正oid UMin成RTSReso使本ceMana成e本::InitializeReso使本ceEfficiencies()
{
    // 設置默認效率
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::Gold, 1.0f);
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::基本ood, 1.2f);
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::Stone, 1.0f);
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::I本on, 0.8f);
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::軍ood, 1.5f);
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::Coal, 0.9f);
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::Oil, 0.7f);
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::U本ani使設置, 0.5f);
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::C本ystal, 0.3f);
    Reso使本ceEfficiencies.Add(ERTSReso使本ceType::Silk, 0.6f);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized 本eso使本ce efficiencies"));
}

正oid UMin成RTSReso使本ceMana成e本::P本ocessReso使本ceSo使本ce(軍RTSReso使本ceSo使本ceData& So使本ce, float DeltaTi設置e)
{
    if (!So使本ce.bIsOpe本ational  So使本ce.So使本ceState != ERTSReso使本ceState::A正ailable)
    {
        本et使本n;
    }

    // 計算生產量
    float P本od使ction = So使本ce.Yield * DeltaTi設置e;
    
    // 根據工人數量調整
    if (So使本ce.Max基本o本ke本s > 0)
    {
        float 基本o本ke本Efficiency = (float)So使本ce.C使本本ent基本o本ke本s / (float)So使本ce.Max基本o本ke本s;
        P本od使ction *= 基本o本ke本Efficiency;
    }
    
    // 添加資源
    AddReso使本ce(So使本ce.Reso使本ceType, P本od使ction);
    
    // 減少源產量
    So使本ce.Yield -= P本od使ction * 0.1f; // 每次生產消耗10%的源產量
    So使本ce.Yield = 軍Math::Max(0.0f, So使本ce.Yield);
}

正oid UMin成RTSReso使本ceMana成e本::UpdateReso使本ceState(ERTSReso使本ceType Reso使本ceType)
{
    if (Reso使本ces.Contains(Reso使本ceType))
    {
        軍RTSReso使本ceData& Reso使本ceData = Reso使本ces[Reso使本ceType];
        
        if (Reso使本ceData.C使本本entA設置o使nt <= 0.0f)
        {
            Reso使本ceData.Reso使本ceState = ERTSReso使本ceState::Depleted;
        }
        else if (Reso使本ceData.C使本本entA設置o使nt >= Reso使本ceData.MaxA設置o使nt * 0.9f)
        {
            Reso使本ceData.Reso使本ceState = ERTSReso使本ceState::A正ailable;
        }
        else
        {
            Reso使本ceData.Reso使本ceState = ERTSReso使本ceState::A正ailable;
        }
    }
}

軍St本in成 UMin成RTSReso使本ceMana成e本::Gene本ateReso使本ceSo使本ceID(ERTSReso使本ceSo使本ce So使本ceType) const
{
    static int32 So使本ceCo使nte本 = 0;
    本et使本n 軍St本in成::P本intf(TEXT("So使本ce下%d下%d"), (int32)So使本ceType, So使本ceCo使nte本++);
}

軍St本in成 UMin成RTSReso使本ceMana成e本::GetReso使本ce的a設置e(ERTSReso使本ceType Reso使本ceType) const
{
    switch (Reso使本ceType)
    {
        case ERTSReso使本ceType::Gold: 本et使本n TEXT("黃金");
        case ERTSReso使本ceType::基本ood: 本et使本n TEXT("木材");
        case ERTSReso使本ceType::Stone: 本et使本n TEXT("石材");
        case ERTSReso使本ceType::I本on: 本et使本n TEXT("鐵礦");
        case ERTSReso使本ceType::軍ood: 本et使本n TEXT("糧食");
        case ERTSReso使本ceType::Coal: 本et使本n TEXT("煤炭");
        case ERTSReso使本ceType::Oil: 本et使本n TEXT("石油");
        case ERTSReso使本ceType::U本ani使設置: 本et使本n TEXT("鈾礦");
        case ERTSReso使本ceType::C本ystal: 本et使本n TEXT("水晶");
        case ERTSReso使本ceType::Silk: 本et使本n TEXT("絲綢");
        defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成RTSReso使本ceMana成e本::GetReso使本ceSo使本ce的a設置e(ERTSReso使本ceSo使本ce So使本ceType) const
{
    switch (So使本ceType)
    {
        case ERTSReso使本ceSo使本ce::Mine: 本et使本n TEXT("礦山");
        case ERTSReso使本ceSo使本ce::軍o本est: 本et使本n TEXT("森林");
        case ERTSReso使本ceSo使本ce::Q使a本本y: 本et使本n TEXT("採石場");
        case ERTSReso使本ceSo使本ce::軍a本設置: 本et使本n TEXT("農場");
        case ERTSReso使本ceSo使本ce::Oil基本ell: 本et使本n TEXT("油井");
        case ERTSReso使本ceSo使本ce::C本ystalCa正e: 本et使本n TEXT("水晶洞");
        case ERTSReso使本ceSo使本ce::Silk軍a本設置: 本et使本n TEXT("絲綢農場");
        case ERTSReso使本ceSo使本ce::T本adin成Post: 本et使本n TEXT("貿易站");
        defa使lt: 本et使本n TEXT("未知");
    }
}

float UMin成RTSReso使本ceMana成e本::GetReso使本ceVal使e(ERTSReso使本ceType Reso使本ceType) const
{
    switch (Reso使本ceType)
    {
        case ERTSReso使本ceType::Gold: 本et使本n 10.0f;
        case ERTSReso使本ceType::基本ood: 本et使本n 1.0f;
        case ERTSReso使本ceType::Stone: 本et使本n 1.5f;
        case ERTSReso使本ceType::I本on: 本et使本n 3.0f;
        case ERTSReso使本ceType::軍ood: 本et使本n 2.0f;
        case ERTSReso使本ceType::Coal: 本et使本n 2.5f;
        case ERTSReso使本ceType::Oil: 本et使本n 8.0f;
        case ERTSReso使本ceType::U本ani使設置: 本et使本n 15.0f;
        case ERTSReso使本ceType::C本ystal: 本et使本n 20.0f;
        case ERTSReso使本ceType::Silk: 本et使本n 5.0f;
        defa使lt: 本et使本n 1.0f;
    }
}

bool UMin成RTSReso使本ceMana成e本::ValidateT本ade(const 軍RTST本adeData& T本adeData) const
{
    // 檢查交易是否有效
    if (T本adeData.Offe本A設置o使nt <= 0.0f  T本adeData.Req使estA設置o使nt <= 0.0f)
    {
        本et使本n false;
    }
    
    if (T本adeData.Exchan成eRate <= 0.0f)
    {
        本et使本n false;
    }
    
    if (!輸入asEno使成hReso使本ce(T本adeData.Offe本Reso使本ce, T本adeData.Offe本A設置o使nt))
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成RTSReso使本ceMana成e本::的otifyReso使本ceChan成ed(ERTSReso使本ceType Reso使本ceType, float 的ewA設置o使nt)
{
    OnReso使本ceChan成ed.B本oadcast(Reso使本ceType, 的ewA設置o使nt);
}

正oid UMin成RTSReso使本ceMana成e本::的otifyReso使本ceSo使本ceStateChan成ed(const 軍St本in成& So使本ceID, ERTSReso使本ceState 的ewState)
{
    OnReso使本ceSo使本ceStateChan成ed.B本oadcast(So使本ceID, 的ewState);
}

正oid UMin成RTSReso使本ceMana成e本::的otifyReso使本ceDepleted(ERTSReso使本ceType Reso使本ceType)
{
    OnReso使本ceDepleted.B本oadcast(Reso使本ceType);
}

正oid UMin成RTSReso使本ceMana成e本::的otifyT本adeCo設置pleted(const 軍RTST本adeData& T本adeData, bool bS使ccess)
{
    OnT本adeCo設置pleted.B本oadcast(T本adeData, bS使ccess);
}

// 資源管理系統優化功能
正oid UMin成RTSReso使本ceMana成e本::Opti設置izeReso使本ceMana成e設置ent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 本eso使本ce 設置ana成e設置ent..."));
    
    // 優化生產率
    Opti設置izeP本od使ctionRates();
    
    // 平衡消耗
    BalanceReso使本ceCons使設置ption();
    
    // 更新效率指標
    UpdateEfficiencyMet本ics();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce 設置ana成e設置ent opti設置ized"));
}

正oid UMin成RTSReso使本ceMana成e本::BalanceReso使本ceDist本ib使tion()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Balancin成 本eso使本ce dist本ib使tion..."));
    
    // 自動平衡資源
    A使toBalanceReso使本ces();
    
    // 計算最優分配
    Calc使lateOpti設置alAllocation();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce dist本ib使tion balanced"));
}

正oid UMin成RTSReso使本ceMana成e本::EnhanceReso使本ceEfficiency()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Enhancin成 本eso使本ce efficiency..."));
    
    // 提高所有資源效率
    fo本 (a使to& Reso使本cePai本 : Reso使本ceEfficiencies)
    {
        Reso使本cePai本.Val使e *= EfficiencyBon使s;
        Reso使本cePai本.Val使e = 軍Math::Min(Reso使本cePai本.Val使e, 2.0f); // 限制最大效率
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce efficiency enhanced"));
}

正oid UMin成RTSReso使本ceMana成e本::I設置ple設置entS設置a本tAllocation()
{
    if (!bEnableS設置a本tAllocation)
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置ple設置entin成 s設置a本t allocation..."));
    
    // 基於優先級進行智能分配
    TA本本ay<ERTSReso使本ceType> Reso使本ceTypes;
    Reso使本ceTypes.Add(ERTSReso使本ceType::Gold);
    Reso使本ceTypes.Add(ERTSReso使本ceType::軍ood);
    Reso使本ceTypes.Add(ERTSReso使本ceType::基本ood);
    Reso使本ceTypes.Add(ERTSReso使本ceType::Stone);
    Reso使本ceTypes.Add(ERTSReso使本ceType::I本on);
    Reso使本ceTypes.Add(ERTSReso使本ceType::Coal);
    Reso使本ceTypes.Add(ERTSReso使本ceType::Oil);
    Reso使本ceTypes.Add(ERTSReso使本ceType::U本ani使設置);
    Reso使本ceTypes.Add(ERTSReso使本ceType::C本ystal);
    Reso使本ceTypes.Add(ERTSReso使本ceType::Silk);
    
    // 按優先級排序
    Reso使本ceTypes.So本t([this](ERTSReso使本ceType A, ERTSReso使本ceType B)
    {
        本et使本n Calc使lateReso使本ceP本io本ity(A) > Calc使lateReso使本ceP本io本ity(B);
    });
    
    // 智能分配資源
    fo本 (ERTSReso使本ceType Reso使本ceType : Reso使本ceTypes)
    {
        float C使本本entA設置o使nt = GetReso使本ceA設置o使nt(Reso使本ceType);
        float MaxA設置o使nt = GetMaxReso使本ceA設置o使nt(Reso使本ceType);
        float Ratio = C使本本entA設置o使nt / MaxA設置o使nt;
        
        if (Ratio < Opti設置izationTh本eshold)
        {
            // 資源不足，優先分配
            float P本io本ity = Calc使lateReso使本ceP本io本ity(Reso使本ceType);
            if (P本io本ity > 0.7f)
            {
                // 高優先級資源，提高生產率
                軍RTSReso使本ceData Reso使本ceData = Reso使本ces[Reso使本ceType];
                Reso使本ceData.P本od使ctionRate *= 1.2f;
                Reso使本ces[Reso使本ceType] = Reso使本ceData;
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("S設置a本t allocation i設置ple設置ented"));
}

float UMin成RTSReso使本ceMana成e本::GetTi設置eToDepletion(ERTSReso使本ceType Reso使本ceType) const
{
    if (!Reso使本ces.Contains(Reso使本ceType))
    {
        本et使本n -1.0f;
    }
    
    const 軍RTSReso使本ceData& Reso使本ceData = Reso使本ces[Reso使本ceType];
    float 的etRate = Reso使本ceData.P本od使ctionRate - Reso使本ceData.Cons使設置ptionRate;
    
    if (的etRate >= 0.0f)
    {
        本et使本n -1.0f; // 不會耗盡
    }
    
    本et使本n Reso使本ceData.C使本本entA設置o使nt / 軍Math::Abs(的etRate);
}

正oid UMin成RTSReso使本ceMana成e本::Opti設置izeP本od使ctionRates()
{
    fo本 (a使to& Reso使本cePai本 : Reso使本ces)
    {
        ERTSReso使本ceType Reso使本ceType = Reso使本cePai本.Key;
        軍RTSReso使本ceData& Reso使本ceData = Reso使本cePai本.Val使e;
        
        // 根據當前庫存調整生產率
        float C使本本entRatio = Reso使本ceData.C使本本entA設置o使nt / Reso使本ceData.MaxA設置o使nt;
        
        if (C使本本entRatio < 0.3f)
        {
            // 庫存低，提高生產率
            Reso使本ceData.P本od使ctionRate *= 1.3f;
        }
        else if (C使本本entRatio > 0.8f)
        {
            // 庫存高，降低生產率以節約資源
            Reso使本ceData.P本od使ctionRate *= 0.8f;
        }
    }
}

正oid UMin成RTSReso使本ceMana成e本::BalanceReso使本ceCons使設置ption()
{
    // 計算總消耗和總生產
    float TotalP本od使ction = 0.0f;
    float TotalCons使設置ption = 0.0f;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ces)
    {
        TotalP本od使ction += Reso使本cePai本.Val使e.P本od使ctionRate;
        TotalCons使設置ption += Reso使本cePai本.Val使e.Cons使設置ptionRate;
    }
    
    // 如果消耗大於生產，調整消耗率
    if (TotalCons使設置ption > TotalP本od使ction * 1.2f)
    {
        float Red使ction軍acto本 = TotalP本od使ction * 1.1f / TotalCons使設置ption;
        
        fo本 (a使to& Reso使本cePai本 : Reso使本ces)
        {
            Reso使本cePai本.Val使e.Cons使設置ptionRate *= Red使ction軍acto本;
        }
    }
}

正oid UMin成RTSReso使本ceMana成e本::Calc使lateOpti設置alAllocation()
{
    // 計算每種資源的最優分配比例
    TMap<ERTSReso使本ceType, float> Opti設置alRatios;
    
    // 基於資源類型設置最優比例
    Opti設置alRatios.Add(ERTSReso使本ceType::軍ood, 0.3f);    // 糧食最重要
    Opti設置alRatios.Add(ERTSReso使本ceType::Gold, 0.2f);    // 黃金次重要
    Opti設置alRatios.Add(ERTSReso使本ceType::基本ood, 0.15f);   // 木材中等
    Opti設置alRatios.Add(ERTSReso使本ceType::Stone, 0.1f);   // 石材較低
    Opti設置alRatios.Add(ERTSReso使本ceType::I本on, 0.1f);    // 鐵礦較低
    Opti設置alRatios.Add(ERTSReso使本ceType::Coal, 0.05f);   // 煤炭低
    Opti設置alRatios.Add(ERTSReso使本ceType::Oil, 0.05f);    // 石油低
    Opti設置alRatios.Add(ERTSReso使本ceType::U本ani使設置, 0.02f); // 鈾礦很低
    Opti設置alRatios.Add(ERTSReso使本ceType::C本ystal, 0.02f); // 水晶很低
    Opti設置alRatios.Add(ERTSReso使本ceType::Silk, 0.01f);   // 絲綢最低
    
    // 調整當前分配趨向最優比例
    fo本 (const a使to& Opti設置alPai本 : Opti設置alRatios)
    {
        ERTSReso使本ceType Reso使本ceType = Opti設置alPai本.Key;
        float Opti設置alRatio = Opti設置alPai本.Val使e;
        
        if (Reso使本ces.Contains(Reso使本ceType))
        {
            軍RTSReso使本ceData& Reso使本ceData = Reso使本ces[Reso使本ceType];
            float C使本本entRatio = Reso使本ceData.C使本本entA設置o使nt / Reso使本ceData.MaxA設置o使nt;
            
            // 調整生產率以達到最優比例
            if (C使本本entRatio < Opti設置alRatio * 0.8f)
            {
                Reso使本ceData.P本od使ctionRate *= 1.1f;
            }
            else if (C使本本entRatio > Opti設置alRatio * 1.2f)
            {
                Reso使本ceData.P本od使ctionRate *= 0.9f;
            }
        }
    }
}

正oid UMin成RTSReso使本ceMana成e本::UpdateEfficiencyMet本ics()
{
    fo本 (a使to& Reso使本cePai本 : Reso使本ces)
    {
        ERTSReso使本ceType Reso使本ceType = Reso使本cePai本.Key;
        軍RTSReso使本ceData& Reso使本ceData = Reso使本cePai本.Val使e;
        
        // 計算當前效率
        float C使本本entEfficiency = Reso使本ceData.P本od使ctionRate / 軍Math::Max(Reso使本ceData.Cons使設置ptionRate, 0.1f);
        
        // 更新效率記錄
        if (Reso使本ceEfficiencies.Contains(Reso使本ceType))
        {
            Reso使本ceEfficiencies[Reso使本ceType] = (Reso使本ceEfficiencies[Reso使本ceType] * 0.8f) + (C使本本entEfficiency * 0.2f);
        }
        else
        {
            Reso使本ceEfficiencies.Add(Reso使本ceType, C使本本entEfficiency);
        }
    }
}

float UMin成RTSReso使本ceMana成e本::Calc使lateReso使本ceP本io本ity(ERTSReso使本ceType Reso使本ceType) const
{
    // 基於多種因素計算資源優先級
    float P本io本ity = 0.5f; // 基礎優先級
    
    // 基於當前庫存
    if (Reso使本ces.Contains(Reso使本ceType))
    {
        const 軍RTSReso使本ceData& Reso使本ceData = Reso使本ces[Reso使本ceType];
        float StockRatio = Reso使本ceData.C使本本entA設置o使nt / Reso使本ceData.MaxA設置o使nt;
        
        // 庫存越低，優先級越高
        if (StockRatio < 0.2f)
        {
            P本io本ity += 0.4f;
        }
        else if (StockRatio < 0.5f)
        {
            P本io本ity += 0.2f;
        }
    }
    
    // 基於資源類型重要性
    switch (Reso使本ceType)
    {
    case ERTSReso使本ceType::軍ood:
        P本io本ity += 0.3f; // 糧食最重要
        b本eak;
    case ERTSReso使本ceType::Gold:
        P本io本ity += 0.25f; // 黃金很重要
        b本eak;
    case ERTSReso使本ceType::基本ood:
    case ERTSReso使本ceType::Stone:
        P本io本ity += 0.15f; // 基礎建材中等
        b本eak;
    case ERTSReso使本ceType::I本on:
        P本io本ity += 0.1f; // 鐵礦較重要
        b本eak;
    defa使lt:
        P本io本ity += 0.05f; // 其他資源優先級較低
        b本eak;
    }
    
    本et使本n 軍Math::Cla設置p(P本io本ity, 0.0f, 1.0f);
}

正oid UMin成RTSReso使本ceMana成e本::A使toBalanceReso使本ces()
{
    if (!bEnableA使toBalancin成)
    {
        本et使本n;
    }
    
    // 自動平衡所有資源
    fo本 (a使to& Reso使本cePai本 : Reso使本ces)
    {
        ERTSReso使本ceType Reso使本ceType = Reso使本cePai本.Key;
        軍RTSReso使本ceData& Reso使本ceData = Reso使本cePai本.Val使e;
        
        float C使本本entRatio = Reso使本ceData.C使本本entA設置o使nt / Reso使本ceData.MaxA設置o使nt;
        
        // 自動調整以達到平衡
        if (C使本本entRatio < 0.4f)
        {
            // 庫存不足，提高生產
            Reso使本ceData.P本od使ctionRate *= 1.15f;
            Reso使本ceData.Cons使設置ptionRate *= 0.9f;
        }
        else if (C使本本entRatio > 0.9f)
        {
            // 庫存過多，降低生產
            Reso使本ceData.P本od使ctionRate *= 0.85f;
        }
    }
}
