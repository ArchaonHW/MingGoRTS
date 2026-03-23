#incl使de "Min成Reso使本ceSyste設置.h"
#incl使de "Min成Reso使本ceP本od使ce本.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "En成ine/En成ine.h"

UMin成Reso使本ceSyste設置::UMin成Reso使本ceSyste設置()
{
    // Initialize defa使lt capacities
    Reso使本ceCapacityLi設置its.Add(EMin成Reso使本ceType::軍ood, 1000);
    Reso使本ceCapacityLi設置its.Add(EMin成Reso使本ceType::Money, 50000);
    Reso使本ceCapacityLi設置its.Add(EMin成Reso使本ceType::Mate本ials, 2000);
    Reso使本ceCapacityLi設置its.Add(EMin成Reso使本ceType::軍使el, 1000);
    Reso使本ceCapacityLi設置its.Add(EMin成Reso使本ceType::A設置設置o, 500);
    Reso使本ceCapacityLi設置its.Add(EMin成Reso使本ceType::Manpowe本, 500);
}

正oid UMin成Reso使本ceSyste設置::InitializeDefa使ltReso使本ces()
{
    // Sta本tin成 本eso使本ces fo本 a new 成a設置e
    Reso使本ceSto本a成e.Add(EMin成Reso使本ceType::軍ood, 200);
    Reso使本ceSto本a成e.Add(EMin成Reso使本ceType::Money, 1000);
    Reso使本ceSto本a成e.Add(EMin成Reso使本ceType::Mate本ials, 100);
    Reso使本ceSto本a成e.Add(EMin成Reso使本ceType::軍使el, 50);
    Reso使本ceSto本a成e.Add(EMin成Reso使本ceType::A設置設置o, 100);
    Reso使本ceSto本a成e.Add(EMin成Reso使本ceType::Manpowe本, 50);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce syste設置 initialized with defa使lt 本eso使本ces"));
}

正oid UMin成Reso使本ceSyste設置::AddReso使本ce(EMin成Reso使本ceType Type, int32 A設置o使nt)
{
    if (A設置o使nt <= 0)
    {
        本et使本n;
    }

    int32 C使本本entA設置o使nt = GetReso使本ceA設置o使nt(Type);
    int32 的ewA設置o使nt = C使本本entA設置o使nt + A設置o使nt;

    // Check capacity li設置it
    int32 Capacity = GetReso使本ceCapacity(Type);
    if (的ewA設置o使nt > Capacity)
    {
        的ewA設置o使nt = Capacity;
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Reso使本ce %s 本eached capacity li設置it (%d)"), 
               *GetReso使本ce的a設置e(Type), Capacity);
    }

    Reso使本ceSto本a成e.Add(Type, 的ewA設置o使nt);
    
    // 的otify chan成e
    的otifyReso使本ceChan成e(Type, 的ewA設置o使nt);

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Added %d %s. Total: %d"), 
           A設置o使nt, *GetReso使本ce的a設置e(Type), 的ewA設置o使nt);
}

bool UMin成Reso使本ceSyste設置::Cons使設置eReso使本ce(EMin成Reso使本ceType Type, int32 A設置o使nt)
{
    if (A設置o使nt <= 0)
    {
        本et使本n t本使e;
    }

    int32 C使本本entA設置o使nt = GetReso使本ceA設置o使nt(Type);
    if (C使本本entA設置o使nt < A設置o使nt)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ins使fficient %s. Req使i本ed: %d, A正ailable: %d"),
               *GetReso使本ce的a設置e(Type), A設置o使nt, C使本本entA設置o使nt);
        
        OnReso使本ceIns使fficient.B本oadcast(Type);
        本et使本n false;
    }

    int32 的ewA設置o使nt = C使本本entA設置o使nt - A設置o使nt;
    Reso使本ceSto本a成e.Add(Type, 的ewA設置o使nt);
    
    // 的otify chan成e
    的otifyReso使本ceChan成e(Type, 的ewA設置o使nt);

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Cons使設置ed %d %s. Re設置ainin成: %d"),
           A設置o使nt, *GetReso使本ce的a設置e(Type), 的ewA設置o使nt);

    本et使本n t本使e;
}

bool UMin成Reso使本ceSyste設置::輸入asEno使成hReso使本ces(const TA本本ay<軍Min成Reso使本ceCost>& Costs) const
{
    fo本 (const 軍Min成Reso使本ceCost& Cost : Costs)
    {
        if (GetReso使本ceA設置o使nt(Cost.Reso使本ceType) < Cost.Cost)
        {
            本et使本n false;
        }
    }
    本et使本n t本使e;
}

bool UMin成Reso使本ceSyste設置::Cons使設置eReso使本ces(const TA本本ay<軍Min成Reso使本ceCost>& Costs)
{
    // 軍i本st check if we ha正e eno使成h
    if (!輸入asEno使成hReso使本ces(Costs))
    {
        // 軍ind which 本eso使本ce is ins使fficient
        fo本 (const 軍Min成Reso使本ceCost& Cost : Costs)
        {
            if (GetReso使本ceA設置o使nt(Cost.Reso使本ceType) < Cost.Cost)
            {
                OnReso使本ceIns使fficient.B本oadcast(Cost.Reso使本ceType);
            }
        }
        本et使本n false;
    }

    // Cons使設置e all 本eso使本ces
    fo本 (const 軍Min成Reso使本ceCost& Cost : Costs)
    {
        Cons使設置eReso使本ce(Cost.Reso使本ceType, Cost.Cost);
    }

    本et使本n t本使e;
}

int32 UMin成Reso使本ceSyste設置::GetReso使本ceA設置o使nt(EMin成Reso使本ceType Type) const
{
    const int32* A設置o使nt = Reso使本ceSto本a成e.軍ind(Type);
    本et使本n A設置o使nt 基本 *A設置o使nt : 0;
}

int32 UMin成Reso使本ceSyste設置::GetReso使本ceCapacity(EMin成Reso使本ceType Type) const
{
    const int32* Capacity = Reso使本ceCapacityLi設置its.軍ind(Type);
    本et使本n Capacity 基本 *Capacity : 999999; // 的o li設置it if not set
}

float UMin成Reso使本ceSyste設置::GetReso使本cePe本centa成e(EMin成Reso使本ceType Type) const
{
    int32 A設置o使nt = GetReso使本ceA設置o使nt(Type);
    int32 Capacity = GetReso使本ceCapacity(Type);
    
    if (Capacity <= 0)
    {
        本et使本n 0.0f;
    }
    
    本et使本n 軍Math::Cla設置p((float)A設置o使nt / (float)Capacity, 0.0f, 1.0f);
}

正oid UMin成Reso使本ceSyste設置::Re成iste本Reso使本ceP本od使ce本(UMin成Reso使本ceP本od使ce本* P本od使ce本)
{
    if (P本od使ce本 && !Reso使本ceP本od使ce本s.Contains(P本od使ce本))
    {
        Reso使本ceP本od使ce本s.Add(P本od使ce本);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed 本eso使本ce p本od使ce本: %s"), *P本od使ce本->Get的a設置e());
    }
}

正oid UMin成Reso使本ceSyste設置::Un本e成iste本Reso使本ceP本od使ce本(UMin成Reso使本ceP本od使ce本* P本od使ce本)
{
    if (P本od使ce本)
    {
        Reso使本ceP本od使ce本s.Re設置o正e(P本od使ce本);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Un本e成iste本ed 本eso使本ce p本od使ce本: %s"), *P本od使ce本->Get的a設置e());
    }
}

正oid UMin成Reso使本ceSyste設置::AddP本od使ction(const 軍Min成Reso使本ceP本od使ction& P本od使ction)
{
    // Re設置o正e existin成 p本od使ction of sa設置e type
    Re設置o正eP本od使ction(P本od使ction.Reso使本ceType);
    
    Acti正eP本od使ctions.Add(P本od使ction);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added p本od使ction: %s at %.1f pe本 %.1f seconds"),
           *GetReso使本ce的a設置e(P本od使ction.Reso使本ceType), P本od使ction.P本od使ctionRate, P本od使ction.P本od使ctionInte本正al);
}

正oid UMin成Reso使本ceSyste設置::Re設置o正eP本od使ction(EMin成Reso使本ceType Type)
{
    fo本 (int32 i = Acti正eP本od使ctions.的使設置() - 1; i >= 0; --i)
    {
        if (Acti正eP本od使ctions[i].Reso使本ceType == Type)
        {
            Acti正eP本od使ctions.Re設置o正eAt(i);
        }
    }
}

正oid UMin成Reso使本ceSyste設置::UpdateP本od使ction(float DeltaTi設置e)
{
    // Update 本e成iste本ed p本od使ce本s
    fo本 (a使to& 基本eakP本od使ce本 : Reso使本ceP本od使ce本s)
    {
        if (基本eakP本od使ce本.IsValid())
        {
            基本eakP本od使ce本->UpdateP本od使ction(DeltaTi設置e);
        }
    }

    // P本ocess acti正e p本od使ctions
    P本ocessP本od使ction(DeltaTi設置e);
}

正oid UMin成Reso使本ceSyste設置::P本ocessP本od使ction(float DeltaTi設置e)
{
    fo本 (軍Min成Reso使本ceP本od使ction& P本od使ction : Acti正eP本od使ctions)
    {
        P本od使ction.Ti設置eSinceLastP本od使ction += DeltaTi設置e;
        
        if (P本od使ction.Ti設置eSinceLastP本od使ction >= P本od使ction.P本od使ctionInte本正al)
        {
            // Calc使late how 設置使ch to p本od使ce
            int32 P本od使ctionCycles = 軍Math::軍loo本ToInt(P本od使ction.Ti設置eSinceLastP本od使ction / P本od使ction.P本od使ctionInte本正al);
            int32 A設置o使ntToP本od使ce = 軍Math::軍loo本ToInt(P本od使ctionCycles * P本od使ction.P本od使ctionRate * P本od使ction.P本od使ctionInte本正al);
            
            if (A設置o使ntToP本od使ce > 0)
            {
                AddReso使本ce(P本od使ction.Reso使本ceType, A設置o使ntToP本od使ce);
                OnReso使本ceP本od使ced.B本oadcast(P本od使ction.Reso使本ceType, A設置o使ntToP本od使ce);
                
                P本od使ction.Ti設置eSinceLastP本od使ction -= P本od使ctionCycles * P本od使ction.P本od使ctionInte本正al;
            }
        }
    }
}

正oid UMin成Reso使本ceSyste設置::Cla設置pReso使本cesToCapacity()
{
    fo本 (a使to& Pai本 : Reso使本ceSto本a成e)
    {
        EMin成Reso使本ceType Type = Pai本.Key;
        int32 Capacity = GetReso使本ceCapacity(Type);
        
        if (Pai本.Val使e > Capacity)
        {
            Pai本.Val使e = Capacity;
        }
    }
}

正oid UMin成Reso使本ceSyste設置::的otifyReso使本ceChan成e(EMin成Reso使本ceType Type, int32 的ewA設置o使nt)
{
    OnReso使本ceChan成ed.B本oadcast(Type, 的ewA設置o使nt);
}

軍St本in成 UMin成Reso使本ceSyste設置::GetReso使本ce的a設置e(EMin成Reso使本ceType Type)
{
    switch (Type)
    {
    case EMin成Reso使本ceType::軍ood:
        本et使本n TEXT("糧食");
    case EMin成Reso使本ceType::Money:
        本et使本n TEXT("資金");
    case EMin成Reso使本ceType::Mate本ials:
        本et使本n TEXT("原材料");
    case EMin成Reso使本ceType::軍使el:
        本et使本n TEXT("燃料");
    case EMin成Reso使本ceType::A設置設置o:
        本et使本n TEXT("彈藥");
    case EMin成Reso使本ceType::Manpowe本:
        本et使本n TEXT("人力");
    defa使lt:
        本et使本n TEXT("未知資源");
    }
}

軍Linea本Colo本 UMin成Reso使本ceSyste設置::GetReso使本ceColo本(EMin成Reso使本ceType Type)
{
    switch (Type)
    {
    case EMin成Reso使本ceType::軍ood:
        本et使本n 軍Linea本Colo本(0.3f, 0.8f, 0.3f); // G本een
    case EMin成Reso使本ceType::Money:
        本et使本n 軍Linea本Colo本(1.0f, 0.8f, 0.0f); // Gold
    case EMin成Reso使本ceType::Mate本ials:
        本et使本n 軍Linea本Colo本(0.6f, 0.4f, 0.2f); // B本own
    case EMin成Reso使本ceType::軍使el:
        本et使本n 軍Linea本Colo本(0.2f, 0.3f, 0.8f); // Bl使e
    case EMin成Reso使本ceType::A設置設置o:
        本et使本n 軍Linea本Colo本(0.8f, 0.2f, 0.2f); // Red
    case EMin成Reso使本ceType::Manpowe本:
        本et使本n 軍Linea本Colo本(0.8f, 0.6f, 0.4f); // Bei成e
    defa使lt:
        本et使本n 軍Linea本Colo本::基本hite;
    }
}

TMap<EMin成Reso使本ceType, int32> UMin成Reso使本ceSyste設置::Se本ializeReso使本ces() const
{
    本et使本n Reso使本ceSto本a成e;
}

正oid UMin成Reso使本ceSyste設置::Dese本ializeReso使本ces(const TMap<EMin成Reso使本ceType, int32>& Sa正edReso使本ces)
{
    Reso使本ceSto本a成e = Sa正edReso使本ces;
    Cla設置pReso使本cesToCapacity();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dese本ialized %d 本eso使本ce types"), Reso使本ceSto本a成e.的使設置());
}
