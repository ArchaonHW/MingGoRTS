// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "RTS/Min成RTSReso使本ceGathe本in成Syste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成RTSReso使本ceGathe本in成Syste設置::UMin成RTSReso使本ceGathe本in成Syste設置()
{
    InitializeDefa使ltVal使es();
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::InitializeReso使本ceSyste設置()
{
    Reso使本ce的odes.E設置pty();
    Acti正eGathe本in成Tasks.E設置pty();
    Playe本In正ento本ies.E設置pty();
    Gathe本in成EfficiencyBon使ses.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS Reso使本ce Gathe本in成 Syste設置 initialized"));
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::InitializeDefa使ltVal使es()
{
    // 基础采集速度 (单位/秒)
    BaseGathe本Rates.Add(ERTSReso使本ceType::Gold, 10.0f);
    BaseGathe本Rates.Add(ERTSReso使本ceType::軍ood, 15.0f);
    BaseGathe本Rates.Add(ERTSReso使本ceType::基本ood, 12.0f);
    BaseGathe本Rates.Add(ERTSReso使本ceType::Stone, 8.0f);
    BaseGathe本Rates.Add(ERTSReso使本ceType::I本on, 6.0f);
    BaseGathe本Rates.Add(ERTSReso使本ceType::Coal, 6.0f);
    BaseGathe本Rates.Add(ERTSReso使本ceType::Silk, 4.0f);
    BaseGathe本Rates.Add(ERTSReso使本ceType::Tea, 4.0f);
    BaseGathe本Rates.Add(ERTSReso使本ceType::Po本celain, 2.0f);
    BaseGathe本Rates.Add(ERTSReso使本ceType::Spices, 2.0f);

    // 资源基础价值
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::Gold, 1.0f);
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::軍ood, 0.5f);
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::基本ood, 0.4f);
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::Stone, 0.6f);
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::I本on, 0.8f);
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::Coal, 0.5f);
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::Silk, 2.0f);
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::Tea, 1.5f);
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::Po本celain, 3.0f);
    Reso使本ceBaseVal使es.Add(ERTSReso使本ceType::Spices, 4.0f);
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::Re成iste本Reso使本ce的ode(const 軍RTSReso使本ce的ode& 的ode)
{
    Reso使本ce的odes.Add(的ode);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed 本eso使本ce node: %s at location %s"),
        *UEn使設置::GetVal使eAsSt本in成(的ode.Reso使本ceType),
        *的ode.Location.ToSt本in成());
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::Un本e成iste本Reso使本ce的ode(AActo本* 的odeActo本)
{
    fo本 (int32 i = Reso使本ce的odes.的使設置() - 1; i >= 0; --i)
    {
        if (Reso使本ce的odes[i].的odeActo本 == 的odeActo本)
        {
            Reso使本ce的odes.Re設置o正eAt(i);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Un本e成iste本ed 本eso使本ce node"));
            b本eak;
        }
    }
}

TA本本ay<軍RTSReso使本ce的ode> UMin成RTSReso使本ceGathe本in成Syste設置::GetReso使本ce的odesOfType(ERTSReso使本ceType Reso使本ceType) const
{
    TA本本ay<軍RTSReso使本ce的ode> Res使lt;
    fo本 (const 軍RTSReso使本ce的ode& 的ode : Reso使本ce的odes)
    {
        if (的ode.Reso使本ceType == Reso使本ceType && !的ode.bIsDepleted)
        {
            Res使lt.Add(的ode);
        }
    }
    本et使本n Res使lt;
}

軍RTSReso使本ce的ode UMin成RTSReso使本ceGathe本in成Syste設置::軍ind的ea本estReso使本ce的ode(const 軍Vecto本& Location, ERTSReso使本ceType Reso使本ceType) const
{
    軍RTSReso使本ce的ode 的ea本est的ode;
    float MinDistance = 軍LT下MAX;
    
    fo本 (const 軍RTSReso使本ce的ode& 的ode : Reso使本ce的odes)
    {
        if (的ode.Reso使本ceType == Reso使本ceType && !的ode.bIsDepleted)
        {
            float Distance = 軍Vecto本::Distance(Location, 的ode.Location);
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                的ea本est的ode = 的ode;
            }
        }
    }
    
    本et使本n 的ea本est的ode;
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::Sta本tGathe本in成(AActo本* Gathe本e本, const 軍RTSReso使本ce的ode& Ta本成et的ode)
{
    if (!Gathe本e本  Ta本成et的ode.bIsDepleted)
    {
        本et使本n;
    }
    
    // 检查是否已在采集
    fo本 (軍RTSGathe本in成Task& Task : Acti正eGathe本in成Tasks)
    {
        if (Task.Gathe本e本 == Gathe本e本)
        {
            Task.Ta本成et的ode = Ta本成et的ode;
            本et使本n;
        }
    }
    
    // 创建新的采集任务
    軍RTSGathe本in成Task 的ewTask;
    的ewTask.Gathe本e本 = Gathe本e本;
    的ewTask.Ta本成et的ode = Ta本成et的ode;
    的ewTask.Gathe本A設置o使nt = 0.0f;
    的ewTask.P本o成本ess = 0.0f;
    的ewTask.bIsCo設置plete = false;
    
    Acti正eGathe本in成Tasks.Add(的ewTask);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted 成athe本in成 %s"), *UEn使設置::GetVal使eAsSt本in成(Ta本成et的ode.Reso使本ceType));
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::StopGathe本in成(AActo本* Gathe本e本)
{
    fo本 (int32 i = Acti正eGathe本in成Tasks.的使設置() - 1; i >= 0; --i)
    {
        if (Acti正eGathe本in成Tasks[i].Gathe本e本 == Gathe本e本)
        {
            Acti正eGathe本in成Tasks.Re設置o正eAt(i);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped 成athe本in成"));
            b本eak;
        }
    }
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::P本ocessGathe本in成(float DeltaTi設置e)
{
    fo本 (軍RTSGathe本in成Task& Task : Acti正eGathe本in成Tasks)
    {
        if (Task.bIsCo設置plete  Task.Ta本成et的ode.bIsDepleted)
        {
            contin使e;
        }
        
        // 计算采集速度
        float BaseRate = BaseGathe本Rates.軍indRef(Task.Ta本成et的ode.Reso使本ceType);
        float Efficiency = GetGathe本in成Efficiency(0, Task.Ta本成et的ode.Reso使本ceType); // TODO: 获取正确的Playe本ID
        float Gathe本Rate = BaseRate * Efficiency * Task.Ta本成et的ode.Gathe本Rate;
        
        // 更新进度
        float Gathe本A設置o使nt = Gathe本Rate * DeltaTi設置e;
        Task.Gathe本A設置o使nt += Gathe本A設置o使nt;
        Task.P本o成本ess += Gathe本A設置o使nt / 100.0f; // 假设每100单位为一个采集周期
        
        // 检查是否完成一个采集周期
        if (Task.P本o成本ess >= 1.0f)
        {
            Co設置pleteGathe本in成(Task);
            Task.P本o成本ess = 0.0f;
            Task.Gathe本A設置o使nt = 0.0f;
        }
    }
    
    // 清理无效任务
    fo本 (int32 i = Acti正eGathe本in成Tasks.的使設置() - 1; i >= 0; --i)
    {
        if (!Acti正eGathe本in成Tasks[i].Gathe本e本  Acti正eGathe本in成Tasks[i].Ta本成et的ode.bIsDepleted)
        {
            Acti正eGathe本in成Tasks.Re設置o正eAt(i);
        }
    }
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::Co設置pleteGathe本in成(const 軍RTSGathe本in成Task& Task)
{
    // 添加资源到玩家库存
    int32 Playe本ID = 0; // TODO: 从Gathe本e本获取Playe本ID
    AddReso使本ce(Task.Ta本成et的ode.Reso使本ceType, 100.0f, Playe本ID);
    
    // 减少资源节点数量
    fo本 (軍RTSReso使本ce的ode& 的ode : Reso使本ce的odes)
    {
        if (的ode.的odeActo本 == Task.Ta本成et的ode.的odeActo本)
        {
            的ode.A設置o使nt -= 100.0f;
            if (的ode.A設置o使nt <= 0.0f)
            {
                的ode.bIsDepleted = t本使e;
                OnReso使本ceDepleted.B本oadcast(的ode.的odeActo本, 的ode.Reso使本ceType);
            }
            b本eak;
        }
    }
    
    OnReso使本ceGathe本ed.B本oadcast(Task.Ta本成et的ode.Reso使本ceType, 100.0f, Playe本ID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gathe本ed 100 使nits of %s"), *UEn使設置::GetVal使eAsSt本in成(Task.Ta本成et的ode.Reso使本ceType));
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::AddReso使本ce(ERTSReso使本ceType Reso使本ceType, float A設置o使nt, int32 Playe本ID)
{
    軍RTSPlaye本In正ento本y& In正ento本y = Playe本In正ento本ies.軍indO本Add(Playe本ID);
    
    float C使本本entA設置o使nt = In正ento本y.Reso使本ces.軍indRef(Reso使本ceType);
    float Capacity = In正ento本y.Reso使本ceCapacity.軍indRef(Reso使本ceType);
    
    float 的ewA設置o使nt = 軍Math::Min(C使本本entA設置o使nt + A設置o使nt, Capacity);
    In正ento本y.Reso使本ces.Add(Reso使本ceType, 的ewA設置o使nt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 %d now has %.1f %s"), Playe本ID, 的ewA設置o使nt, *UEn使設置::GetVal使eAsSt本in成(Reso使本ceType));
}

bool UMin成RTSReso使本ceGathe本in成Syste設置::Cons使設置eReso使本ce(ERTSReso使本ceType Reso使本ceType, float A設置o使nt, int32 Playe本ID)
{
    軍RTSPlaye本In正ento本y& In正ento本y = Playe本In正ento本ies.軍indO本Add(Playe本ID);
    
    float C使本本entA設置o使nt = In正ento本y.Reso使本ces.軍indRef(Reso使本ceType);
    
    if (C使本本entA設置o使nt >= A設置o使nt)
    {
        In正ento本y.Reso使本ces.Add(Reso使本ceType, C使本本entA設置o使nt - A設置o使nt);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

float UMin成RTSReso使本ceGathe本in成Syste設置::GetReso使本ceA設置o使nt(ERTSReso使本ceType Reso使本ceType, int32 Playe本ID) const
{
    const 軍RTSPlaye本In正ento本y* In正ento本y = Playe本In正ento本ies.軍ind(Playe本ID);
    if (In正ento本y)
    {
        本et使本n In正ento本y->Reso使本ces.軍indRef(Reso使本ceType);
    }
    本et使本n 0.0f;
}

float UMin成RTSReso使本ceGathe本in成Syste設置::GetReso使本ceCapacity(ERTSReso使本ceType Reso使本ceType, int32 Playe本ID) const
{
    const 軍RTSPlaye本In正ento本y* In正ento本y = Playe本In正ento本ies.軍ind(Playe本ID);
    if (In正ento本y)
    {
        本et使本n In正ento本y->Reso使本ceCapacity.軍indRef(Reso使本ceType);
    }
    本et使本n 0.0f;
}

bool UMin成RTSReso使本ceGathe本in成Syste設置::輸入asEno使成hReso使本ce(ERTSReso使本ceType Reso使本ceType, float A設置o使nt, int32 Playe本ID) const
{
    本et使本n GetReso使本ceA設置o使nt(Reso使本ceType, Playe本ID) >= A設置o使nt;
}

軍RTSPlaye本In正ento本y UMin成RTSReso使本ceGathe本in成Syste設置::GetPlaye本In正ento本y(int32 Playe本ID) const
{
    const 軍RTSPlaye本In正ento本y* In正ento本y = Playe本In正ento本ies.軍ind(Playe本ID);
    if (In正ento本y)
    {
        本et使本n *In正ento本y;
    }
    本et使本n 軍RTSPlaye本In正ento本y();
}

bool UMin成RTSReso使本ceGathe本in成Syste設置::T本adeReso使本ces(int32 軍本o設置Playe本ID, int32 ToPlaye本ID, ERTSReso使本ceType Reso使本ceType, float A設置o使nt)
{
    if (Cons使設置eReso使本ce(Reso使本ceType, A設置o使nt, 軍本o設置Playe本ID))
    {
        AddReso使本ce(Reso使本ceType, A設置o使nt, ToPlaye本ID);
        OnReso使本cesT本aded.B本oadcast(Reso使本ceType, A設置o使nt, ToPlaye本ID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("T本aded %.1f %s f本o設置 Playe本 %d to Playe本 %d"),
            A設置o使nt, *UEn使設置::GetVal使eAsSt本in成(Reso使本ceType), 軍本o設置Playe本ID, ToPlaye本ID);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

float UMin成RTSReso使本ceGathe本in成Syste設置::Calc使lateReso使本ceVal使e(ERTSReso使本ceType Reso使本ceType) const
{
    本et使本n Reso使本ceBaseVal使es.軍indRef(Reso使本ceType);
}

正oid UMin成RTSReso使本ceGathe本in成Syste設置::ApplyGathe本in成EfficiencyBon使s(int32 Playe本ID, ERTSReso使本ceType Reso使本ceType, float Bon使sM使ltiplie本)
{
    TMap<ERTSReso使本ceType, float>& Bon使ses = Gathe本in成EfficiencyBon使ses.軍indO本Add(Playe本ID);
    Bon使ses.Add(Reso使本ceType, Bon使sM使ltiplie本);
}

float UMin成RTSReso使本ceGathe本in成Syste設置::GetGathe本in成Efficiency(int32 Playe本ID, ERTSReso使本ceType Reso使本ceType) const
{
    const TMap<ERTSReso使本ceType, float>* Bon使ses = Gathe本in成EfficiencyBon使ses.軍ind(Playe本ID);
    if (Bon使ses)
    {
        本et使本n 1.0f + Bon使ses->軍indRef(Reso使本ceType);
    }
    本et使本n 1.0f;
}
