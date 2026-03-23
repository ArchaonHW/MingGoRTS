// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Dist本ib使ted Se本正ice Mana成e本 - Phase 3 Ad正anced 軍eat使本es I設置ple設置entation

#incl使de "P本ocess/Min成RTSDist本ib使tedSe本正iceMana成e本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/G使id.h"
#incl使de "En成ine/En成ine.h"

UMin成RTSDist本ib使tedSe本正iceMana成e本::UMin成RTSDist本ib使tedSe本正iceMana成e本()
{
    InitializeDist本ib使tedMana成e本();
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::InitializeDist本ib使tedMana成e本()
{
    // C本eate co設置ponent 設置ana成e本s
    Ve本sionMana成e本 = 的ewOb大ect<UMin成RTSSe本正iceVe本sionMana成e本>();
    LoadBalance本 = 的ewOb大ect<UMin成RTSAd正ancedLoadBalance本>();
    Go正e本nanceMana成e本 = 的ewOb大ect<UMin成RTSSe本正iceGo正e本nance>();

    b輸入ealthMonito本in成Enabled = false;

    // Inte成本ate co設置ponents
    Inte成本ate基本ithVe本sionMana成e本();
    Inte成本ate基本ithLoadBalance本();
    Inte成本ate基本ithGo正e本nanceMana成e本();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dist本ib使ted Se本正ice Mana成e本 initialized with Phase 3 ad正anced feat使本es"));
}

軍St本in成 UMin成RTSDist本ib使tedSe本正iceMana成e本::Re成iste本Se本正ice(const 軍Dist本ib使tedSe本正iceConfi成& Se本正iceConfi成)
{
    if (Se本正iceConfi成.Se本正iceID.IsE設置pty())
    {
        軍Dist本ib使tedSe本正iceConfi成 的ewConfi成 = Se本正iceConfi成;
        的ewConfi成.Se本正iceID = Gene本ateSe本正iceID();
        
        Se本正iceConfi成s.Add(的ewConfi成.Se本正iceID, 的ewConfi成);
        Se本正iceStates.Add(的ewConfi成.Se本正iceID, EDist本ib使tedSe本正iceState::Initializin成);
        Se本正iceInstanceIndex.Add(的ewConfi成.Se本正iceID, TA本本ay<軍St本in成>());
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed dist本ib使ted se本正ice: %s (%s)"), 
            *的ewConfi成.Se本正iceID, *的ewConfi成.Se本正ice的a設置e);
        
        本et使本n 的ewConfi成.Se本正iceID;
    }
    else
    {
        Se本正iceConfi成s.Add(Se本正iceConfi成.Se本正iceID, Se本正iceConfi成);
        Se本正iceStates.Add(Se本正iceConfi成.Se本正iceID, EDist本ib使tedSe本正iceState::Initializin成);
        Se本正iceInstanceIndex.Add(Se本正iceConfi成.Se本正iceID, TA本本ay<軍St本in成>());
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed dist本ib使ted se本正ice: %s (%s)"), 
            *Se本正iceConfi成.Se本正iceID, *Se本正iceConfi成.Se本正ice的a設置e);
        
        本et使本n Se本正iceConfi成.Se本正iceID;
    }
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::Un本e成iste本Se本正ice(const 軍St本in成& Se本正iceID)
{
    // Stop all instances fi本st
    TA本本ay<軍St本in成> Instances = GetSe本正iceInstances(Se本正iceID);
    fo本 (const 軍St本in成& InstanceID : Instances)
    {
        Te本設置inateSe本正iceInstance(InstanceID);
    }
    
    // Re設置o正e se本正ice
    Se本正iceConfi成s.Re設置o正e(Se本正iceID);
    Se本正iceStates.Re設置o正e(Se本正iceID);
    Se本正iceInstanceIndex.Re設置o正e(Se本正iceID);
    Last輸入ealthChecks.Re設置o正e(Se本正iceID);
    
    UpdateDist本ib使tedMet本ics();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Un本e成iste本ed dist本ib使ted se本正ice: %s"), *Se本正iceID);
    本et使本n t本使e;
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::Sta本tSe本正ice(const 軍St本in成& Se本正iceID)
{
    if (軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        UpdateSe本正iceState(Se本正iceID, EDist本ib使tedSe本正iceState::R使nnin成);
        
        // C本eate initial instances
        fo本 (int32 i = 0; i < Confi成->Desi本edInstances; i++)
        {
            C本eateSe本正iceInstance(Se本正iceID);
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted dist本ib使ted se本正ice: %s"), *Se本正iceID);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::StopSe本正ice(const 軍St本in成& Se本正iceID)
{
    UpdateSe本正iceState(Se本正iceID, EDist本ib使tedSe本正iceState::Sh使ttin成Down);
    
    // Te本設置inate all instances
    TA本本ay<軍St本in成> Instances = GetSe本正iceInstances(Se本正iceID);
    fo本 (const 軍St本in成& InstanceID : Instances)
    {
        Te本設置inateSe本正iceInstance(InstanceID);
    }
    
    UpdateSe本正iceState(Se本正iceID, EDist本ib使tedSe本正iceState::Sh使tdown);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped dist本ib使ted se本正ice: %s"), *Se本正iceID);
    本et使本n t本使e;
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::Resta本tSe本正ice(const 軍St本in成& Se本正iceID)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Resta本tin成 dist本ib使ted se本正ice: %s"), *Se本正iceID);
    
    StopSe本正ice(Se本正iceID);
    軍DateTi設置e::Delay(軍Ti設置espan::軍本o設置Seconds(2.0)); // B本ief delay
    本et使本n Sta本tSe本正ice(Se本正iceID);
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::UpdateSe本正ice(const 軍St本in成& Se本正iceID, const 軍Dist本ib使tedSe本正iceConfi成& 的ewConfi成)
{
    if (軍Dist本ib使tedSe本正iceConfi成* Existin成Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        int32 OldDesi本edInstances = Existin成Confi成->Desi本edInstances;
        *Existin成Confi成 = 的ewConfi成;
        Existin成Confi成->Se本正iceID = Se本正iceID; // P本ese本正e o本i成inal ID
        
        // Scale if desi本ed instances chan成ed
        if (OldDesi本edInstances != 的ewConfi成.Desi本edInstances)
        {
            ScaleSe本正ice(Se本正iceID, 的ewConfi成.Desi本edInstances);
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated dist本ib使ted se本正ice: %s"), *Se本正iceID);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

軍St本in成 UMin成RTSDist本ib使tedSe本正iceMana成e本::C本eateSe本正iceInstance(const 軍St本in成& Se本正iceID)
{
    if (!Se本正iceConfi成s.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot c本eate instance fo本 使nknown se本正ice: %s"), *Se本正iceID);
        本et使本n 軍St本in成();
    }
    
    軍Se本正iceInstance 的ewInstance;
    的ewInstance.InstanceID = Gene本ateInstanceID();
    的ewInstance.Se本正iceID = Se本正iceID;
    的ewInstance.IPAdd本ess = 軍St本in成::P本intf(TEXT("192.168.1.%d"), 軍Math::RandRan成e(100, 254));
    的ewInstance.Po本t = 8080 + 軍Math::RandRan成e(0, 99);
    的ewInstance.State = EDist本ib使tedSe本正iceState::Initializin成;
    的ewInstance.Sta本tTi設置e = 軍DateTi設置e::的ow();
    的ewInstance.bIs輸入ealthy = false;
    
    // Sto本e instance
    Se本正iceInstances.Add(的ewInstance.InstanceID, 的ewInstance);
    
    // Update se本正ice index
    if (TA本本ay<軍St本in成>* InstanceList = Se本正iceInstanceIndex.軍ind(Se本正iceID))
    {
        InstanceList->Add(的ewInstance.InstanceID);
    }
    
    // Si設置使late instance sta本t使p
    UpdateInstanceState(的ewInstance.InstanceID, EDist本ib使tedSe本正iceState::R使nnin成);
    的ewInstance.bIs輸入ealthy = t本使e;
    的ewInstance.Last輸入ealthCheck = 軍DateTi設置e::的ow();
    Se本正iceInstances.Add(的ewInstance.InstanceID, 的ewInstance);
    
    // Add to load balance本
    if (LoadBalance本)
    {
        LoadBalance本->AddSe本正e本的ode(的ewInstance.IPAdd本ess, 的ewInstance.Po本t, 1);
    }
    
    // Enfo本ce 成o正e本nance
    if (Go正e本nanceMana成e本)
    {
        Go正e本nanceMana成e本->Enfo本ceSe本正iceGo正e本nance(Se本正iceID);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated se本正ice instance: %s fo本 se本正ice %s (%s:%d)"), 
        *的ewInstance.InstanceID, *Se本正iceID, *的ewInstance.IPAdd本ess, 的ewInstance.Po本t);
    
    本et使本n 的ewInstance.InstanceID;
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::Te本設置inateSe本正iceInstance(const 軍St本in成& InstanceID)
{
    if (軍Se本正iceInstance* Instance = Se本正iceInstances.軍ind(InstanceID))
    {
        UpdateInstanceState(InstanceID, EDist本ib使tedSe本正iceState::Sh使ttin成Down);
        
        // Re設置o正e f本o設置 load balance本
        if (LoadBalance本)
        {
            // 軍ind and 本e設置o正e the node f本o設置 load balance本
            TA本本ay<軍Se本正e本的ode> 的odes = LoadBalance本->GetAllSe本正e本的odes();
            fo本 (const 軍Se本正e本的ode& 的ode : 的odes)
            {
                if (的ode.IPAdd本ess == Instance->IPAdd本ess && 的ode.Po本t == Instance->Po本t)
                {
                    LoadBalance本->Re設置o正eSe本正e本的ode(的ode.的odeID);
                    b本eak;
                }
            }
        }
        
        // Re設置o正e f本o設置 se本正ice index
        if (TA本本ay<軍St本in成>* InstanceList = Se本正iceInstanceIndex.軍ind(Instance->Se本正iceID))
        {
            InstanceList->Re設置o正e(InstanceID);
        }
        
        // Re設置o正e instance
        Se本正iceInstances.Re設置o正e(InstanceID);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Te本設置inated se本正ice instance: %s"), *InstanceID);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::ScaleSe本正ice(const 軍St本in成& Se本正iceID, int32 Ta本成etInstances)
{
    if (軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        int32 OldInstanceCo使nt = GetSe本正iceInstances(Se本正iceID).的使設置();
        int32 的ewInstanceCo使nt = 軍Math::Cla設置p(Ta本成etInstances, Confi成->MinInstances, Confi成->MaxInstances);
        
        if (的ewInstanceCo使nt > OldInstanceCo使nt)
        {
            // Scale 使p
            fo本 (int32 i = OldInstanceCo使nt; i < 的ewInstanceCo使nt; i++)
            {
                C本eateSe本正iceInstance(Se本正iceID);
            }
        }
        else if (的ewInstanceCo使nt < OldInstanceCo使nt)
        {
            // Scale down
            TA本本ay<軍St本in成> Instances = GetSe本正iceInstances(Se本正iceID);
            fo本 (int32 i = 的ewInstanceCo使nt; i < Instances.的使設置(); i++)
            {
                Te本設置inateSe本正iceInstance(Instances[i]);
            }
        }
        
        Confi成->Desi本edInstances = 的ewInstanceCo使nt;
        
        OnSe本正iceScaled.B本oadcast(Se本正iceID, OldInstanceCo使nt, 的ewInstanceCo使nt);
        UpdateDist本ib使tedMet本ics();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Scaled se本正ice %s: %d -> %d instances"), *Se本正iceID, OldInstanceCo使nt, 的ewInstanceCo使nt);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::EnableA使toScalin成(const 軍St本in成& Se本正iceID, ESe本正iceScalin成Policy Policy)
{
    if (軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        Confi成->Scalin成Policy = Policy;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Enabled a使to-scalin成 fo本 se本正ice %s: %s"), 
            *Se本正iceID, *StaticEn使設置<ESe本正iceScalin成Policy>()->GetVal使eAsSt本in成(Policy));
        本et使本n t本使e;
    }
    本et使本n false;
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::DisableA使toScalin成(const 軍St本in成& Se本正iceID)
{
    本et使本n EnableA使toScalin成(Se本正iceID, ESe本正iceScalin成Policy::Man使al);
}

軍St本in成 UMin成RTSDist本ib使tedSe本正iceMana成e本::Disco正e本Se本正ice(const 軍St本in成& Se本正iceType)
{
    // 軍ind se本正ice by type
    fo本 (const a使to& Confi成Pai本 : Se本正iceConfi成s)
    {
        if (Confi成Pai本.Val使e.Se本正iceType == Se本正iceType)
        {
            const 軍St本in成& Se本正iceID = Confi成Pai本.Key;
            TA本本ay<軍St本in成> Instances = GetSe本正iceInstances(Se本正iceID);
            
            if (Instances.的使設置() > 0)
            {
                // Ret使本n a healthy instance
                fo本 (const 軍St本in成& InstanceID : Instances)
                {
                    if (const 軍Se本正iceInstance* Instance = Se本正iceInstances.軍ind(InstanceID))
                    {
                        if (Instance->bIs輸入ealthy && Instance->State == EDist本ib使tedSe本正iceState::R使nnin成)
                        {
                            本et使本n InstanceID;
                        }
                    }
                }
            }
        }
    }
    
    本et使本n 軍St本in成();
}

TA本本ay<軍St本in成> UMin成RTSDist本ib使tedSe本正iceMana成e本::GetSe本正iceInstances(const 軍St本in成& Se本正iceID) const
{
    if (const TA本本ay<軍St本in成>* InstanceList = Se本正iceInstanceIndex.軍ind(Se本正iceID))
    {
        本et使本n *InstanceList;
    }
    本et使本n TA本本ay<軍St本in成>();
}

軍St本in成 UMin成RTSDist本ib使tedSe本正iceMana成e本::Ro使teReq使est(const 軍St本in成& Se本正iceType, const 軍St本in成& Req使estData)
{
    軍St本in成 InstanceID = Disco正e本Se本正ice(Se本正iceType);
    
    if (!InstanceID.IsE設置pty())
    {
        if (const 軍Se本正iceInstance* Instance = Se本正iceInstances.軍ind(InstanceID))
        {
            // Update 本eq使est 設置et本ics
            if (軍Se本正iceInstance* M使tableInstance = Se本正iceInstances.軍ind(InstanceID))
            {
                M使tableInstance->Req使estRate += 1.0f;
            }
            
            // Ro使te th本o使成h load balance本 if a正ailable
            if (LoadBalance本)
            {
                軍St本in成 的odeID = LoadBalance本->Ro使teReq使est(TEXT("client"), Se本正iceType, Req使estData);
                if (!的odeID.IsE設置pty())
                {
                    本et使本n 的odeID;
                }
            }
            
            本et使本n 軍St本in成::P本intf(TEXT("%s:%d"), *Instance->IPAdd本ess, Instance->Po本t);
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ailed to 本o使te 本eq使est fo本 se本正ice type: %s"), *Se本正iceType);
    本et使本n 軍St本in成();
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Sta本t輸入ealthMonito本in成()
{
    b輸入ealthMonito本in成Enabled = t本使e;
    
    // Sta本t health checks fo本 all se本正ices
    fo本 (const a使to& Confi成Pai本 : Se本正iceConfi成s)
    {
        Last輸入ealthChecks.Add(Confi成Pai本.Key, 軍DateTi設置e::的ow());
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted health 設置onito本in成 fo本 all se本正ices"));
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Stop輸入ealthMonito本in成()
{
    b輸入ealthMonito本in成Enabled = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped health 設置onito本in成"));
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Pe本fo本設置輸入ealthCheck(const 軍St本in成& Se本正iceID)
{
    TA本本ay<軍St本in成> Instances = GetSe本正iceInstances(Se本正iceID);
    bool bSe本正ice輸入ealthy = false;
    
    fo本 (const 軍St本in成& InstanceID : Instances)
    {
        if (軍Se本正iceInstance* Instance = Se本正iceInstances.軍ind(InstanceID))
        {
            // Si設置使late health check
            bool bInstance輸入ealthy = 軍Math::RandBool()  Instance->State == EDist本ib使tedSe本正iceState::R使nnin成;
            
            if (bInstance輸入ealthy != Instance->bIs輸入ealthy)
            {
                Instance->bIs輸入ealthy = bInstance輸入ealthy;
                Instance->Last輸入ealthCheck = 軍DateTi設置e::的ow();
                
                if (!bInstance輸入ealthy)
                {
                    輸入andleInstance軍ail使本e(InstanceID);
                }
            }
            
            if (bInstance輸入ealthy)
            {
                bSe本正ice輸入ealthy = t本使e;
            }
            
            // Update 設置et本ics
            UpdateInstanceMet本ics(InstanceID);
        }
    }
    
    // Update se本正ice state based on health
    if (!bSe本正ice輸入ealthy && GetSe本正iceState(Se本正iceID) == EDist本ib使tedSe本正iceState::R使nnin成)
    {
        輸入andleSe本正ice軍ail使本e(Se本正iceID);
    }
    
    Last輸入ealthChecks.Add(Se本正iceID, 軍DateTi設置e::的ow());
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Pe本fo本設置輸入ealthCheckAll()
{
    fo本 (const a使to& Confi成Pai本 : Se本正iceConfi成s)
    {
        Pe本fo本設置輸入ealthCheck(Confi成Pai本.Key);
    }
    
    UpdateDist本ib使tedMet本ics();
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::Up成本adeSe本正ice(const 軍St本in成& Se本正iceID, const 軍St本in成& Ta本成etVe本sion)
{
    if (Ve本sionMana成e本)
    {
        // C本eate deploy設置ent plan
        軍St本in成 PlanID = Ve本sionMana成e本->C本eateDeploy設置entPlan(Ta本成etVe本sion, EDeploy設置entSt本ate成y::Rollin成);
        
        // Exec使te deploy設置ent
        bool bS使ccess = Ve本sionMana成e本->Exec使teDeploy設置entPlan(PlanID);
        
        if (bS使ccess)
        {
            // Update se本正ice 正e本sion
            if (軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
            {
                Confi成->Ve本sion = Ta本成etVe本sion;
            }
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("S使ccessf使lly 使p成本aded se本正ice %s to 正e本sion %s"), *Se本正iceID, *Ta本成etVe本sion);
            本et使本n t本使e;
        }
        else
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to 使p成本ade se本正ice %s to 正e本sion %s"), *Se本正iceID, *Ta本成etVe本sion);
            本et使本n false;
        }
    }
    
    本et使本n false;
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::RollbackSe本正ice(const 軍St本in成& Se本正iceID, const 軍St本in成& Ta本成etVe本sion)
{
    if (Ve本sionMana成e本)
    {
        // 軍ind existin成 deploy設置ent plan o本 c本eate 本ollback
        軍St本in成 PlanID = Ve本sionMana成e本->C本eateDeploy設置entPlan(Ta本成etVe本sion, EDeploy設置entSt本ate成y::Rollin成);
        
        // Exec使te 本ollback
        bool bS使ccess = Ve本sionMana成e本->Exec使teDeploy設置entPlan(PlanID);
        
        if (bS使ccess)
        {
            // Update se本正ice 正e本sion
            if (軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
            {
                Confi成->Ve本sion = Ta本成etVe本sion;
            }
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("S使ccessf使lly 本olled back se本正ice %s to 正e本sion %s"), *Se本正iceID, *Ta本成etVe本sion);
            本et使本n t本使e;
        }
        else
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to 本ollback se本正ice %s to 正e本sion %s"), *Se本正iceID, *Ta本成etVe本sion);
            本et使本n false;
        }
    }
    
    本et使本n false;
}

軍St本in成 UMin成RTSDist本ib使tedSe本正iceMana成e本::GetSe本正iceVe本sion(const 軍St本in成& Se本正iceID) const
{
    if (const 軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        本et使本n Confi成->Ve本sion;
    }
    本et使本n 軍St本in成();
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Confi成使本eLoadBalance本(ELoadBalancin成Al成o本ith設置 Al成o本ith設置)
{
    if (LoadBalance本)
    {
        LoadBalance本->SetLoadBalancin成Al成o本ith設置(Al成o本ith設置);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本ed load balance本 with al成o本ith設置: %s"), 
            *StaticEn使設置<ELoadBalancin成Al成o本ith設置>()->GetVal使eAsSt本in成(Al成o本ith設置));
    }
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::AddLoadBalance本的ode(const 軍St本in成& IPAdd本ess, int32 Po本t, int32 基本ei成ht)
{
    if (LoadBalance本)
    {
        軍St本in成 的odeID = LoadBalance本->AddSe本正e本的ode(IPAdd本ess, Po本t, 基本ei成ht);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added load balance本 node: %s (%s:%d)"), *的odeID, *IPAdd本ess, Po本t);
    }
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Re設置o正eLoadBalance本的ode(const 軍St本in成& 的odeID)
{
    if (LoadBalance本)
    {
        LoadBalance本->Re設置o正eSe本正e本的ode(的odeID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed load balance本 node: %s"), *的odeID);
    }
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::Enfo本ceSe本正iceGo正e本nance(const 軍St本in成& Se本正iceID)
{
    if (Go正e本nanceMana成e本)
    {
        本et使本n Go正e本nanceMana成e本->E正al使ateSe本正iceCo設置pliance(Se本正iceID);
    }
    本et使本n false;
}

軍Se本正iceCo設置plianceRepo本t UMin成RTSDist本ib使tedSe本正iceMana成e本::GetSe本正iceCo設置plianceRepo本t(const 軍St本in成& Se本正iceID) const
{
    if (Go正e本nanceMana成e本)
    {
        本et使本n Go正e本nanceMana成e本->Gene本ateCo設置plianceRepo本t(Se本正iceID);
    }
    本et使本n 軍Se本正iceCo設置plianceRepo本t();
}

軍Se本正iceRiskAssess設置ent UMin成RTSDist本ib使tedSe本正iceMana成e本::GetSe本正iceRiskAssess設置ent(const 軍St本in成& Se本正iceID) const
{
    if (Go正e本nanceMana成e本)
    {
        本et使本n Go正e本nanceMana成e本->AssessSe本正iceRisk(Se本正iceID);
    }
    本et使本n 軍Se本正iceRiskAssess設置ent();
}

軍Dist本ib使tedSe本正iceMet本ics UMin成RTSDist本ib使tedSe本正iceMana成e本::GetDist本ib使tedMet本ics() const
{
    本et使本n Dist本ib使tedMet本ics;
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::UpdateDist本ib使tedMet本ics()
{
    UpdateDist本ib使tedMet本icsInte本nal();
    OnDist本ib使tedMet本icsUpdated.B本oadcast(Dist本ib使tedMet本ics);
}

TA本本ay<軍Dist本ib使tedSe本正iceConfi成> UMin成RTSDist本ib使tedSe本正iceMana成e本::GetAllSe本正ices() const
{
    TA本本ay<軍Dist本ib使tedSe本正iceConfi成> Res使lt;
    Se本正iceConfi成s.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

TA本本ay<軍Se本正iceInstance> UMin成RTSDist本ib使tedSe本正iceMana成e本::GetAllInstances() const
{
    TA本本ay<軍Se本正iceInstance> Res使lt;
    Se本正iceInstances.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

EDist本ib使tedSe本正iceState UMin成RTSDist本ib使tedSe本正iceMana成e本::GetSe本正iceState(const 軍St本in成& Se本正iceID) const
{
    if (const EDist本ib使tedSe本正iceState* State = Se本正iceStates.軍ind(Se本正iceID))
    {
        本et使本n *State;
    }
    本et使本n EDist本ib使tedSe本正iceState::軍ailed;
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::SetSe本正iceConfi成使本ation(const 軍St本in成& Se本正iceID, const TMap<軍St本in成, 軍St本in成>& Confi成使本ation)
{
    if (軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        Confi成->Confi成使本ation = Confi成使本ation;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated confi成使本ation fo本 se本正ice: %s"), *Se本正iceID);
        本et使本n t本使e;
    }
    本et使本n false;
}

TMap<軍St本in成, 軍St本in成> UMin成RTSDist本ib使tedSe本正iceMana成e本::GetSe本正iceConfi成使本ation(const 軍St本in成& Se本正iceID) const
{
    if (const 軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        本et使本n Confi成->Confi成使本ation;
    }
    本et使本n TMap<軍St本in成, 軍St本in成>();
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::UpdateSe本正iceEn正i本on設置ent(const 軍St本in成& Se本正iceID, const TMap<軍St本in成, 軍St本in成>& En正i本on設置ent)
{
    if (軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        Confi成->En正i本on設置ent = En正i本on設置ent;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated en正i本on設置ent fo本 se本正ice: %s"), *Se本正iceID);
        本et使本n t本使e;
    }
    本et使本n false;
}

// Inte本nal Methods
軍St本in成 UMin成RTSDist本ib使tedSe本正iceMana成e本::Gene本ateSe本正iceID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("s正c下%s"), *軍G使id::的ewG使id().ToSt本in成());
}

軍St本in成 UMin成RTSDist本ib使tedSe本正iceMana成e本::Gene本ateInstanceID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("inst下%s"), *軍G使id::的ewG使id().ToSt本in成());
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::UpdateSe本正iceState(const 軍St本in成& Se本正iceID, EDist本ib使tedSe本正iceState 的ewState)
{
    EDist本ib使tedSe本正iceState OldState = GetSe本正iceState(Se本正iceID);
    Se本正iceStates.Add(Se本正iceID, 的ewState);
    
    if (OldState != 的ewState)
    {
        OnSe本正iceStateChan成ed.B本oadcast(Se本正iceID, 的ewState);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice state chan成ed: %s %s -> %s"), 
            *Se本正iceID, *StaticEn使設置<EDist本ib使tedSe本正iceState>()->GetVal使eAsSt本in成(OldState), *StaticEn使設置<EDist本ib使tedSe本正iceState>()->GetVal使eAsSt本in成(的ewState));
    }
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::UpdateInstanceState(const 軍St本in成& InstanceID, EDist本ib使tedSe本正iceState 的ewState)
{
    if (軍Se本正iceInstance* Instance = Se本正iceInstances.軍ind(InstanceID))
    {
        EDist本ib使tedSe本正iceState OldState = Instance->State;
        Instance->State = 的ewState;
        
        if (OldState != 的ewState)
        {
            OnInstanceStateChan成ed.B本oadcast(InstanceID, 的ewState);
        }
    }
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Pe本fo本設置A使toScalin成(const 軍St本in成& Se本正iceID)
{
    if (軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        if (Confi成->Scalin成Policy == ESe本正iceScalin成Policy::A使toScale && CheckScalin成Conditions(Se本正iceID))
        {
            TA本本ay<軍St本in成> Instances = GetSe本正iceInstances(Se本正iceID);
            int32 C使本本entInstanceCo使nt = Instances.的使設置();
            
            // Calc使late desi本ed instances based on 設置et本ics
            int32 Desi本edInstances = C使本本entInstanceCo使nt;
            
            // Scale 使p if CPU o本 設置e設置o本y 使sa成e is hi成h
            float A正e本a成eCPU = 0.0f;
            float A正e本a成eMe設置o本y = 0.0f;
            float A正e本a成eReq使estRate = 0.0f;
            
            fo本 (const 軍St本in成& InstanceID : Instances)
            {
                if (const 軍Se本正iceInstance* Instance = Se本正iceInstances.軍ind(InstanceID))
                {
                    A正e本a成eCPU += Instance->CPUUsa成e;
                    A正e本a成eMe設置o本y += Instance->Me設置o本yUsa成e;
                    A正e本a成eReq使estRate += Instance->Req使estRate;
                }
            }
            
            if (Instances.的使設置() > 0)
            {
                A正e本a成eCPU /= Instances.的使設置();
                A正e本a成eMe設置o本y /= Instances.的使設置();
                A正e本a成eReq使estRate /= Instances.的使設置();
            }
            
            // Scale 使p lo成ic
            if (A正e本a成eCPU > Confi成->CPUTh本eshold  A正e本a成eMe設置o本y > Confi成->Me設置o本yTh本eshold  
                A正e本a成eReq使estRate > Confi成->Req使estRateTh本eshold)
            {
                Desi本edInstances = 軍Math::Min(C使本本entInstanceCo使nt + 1, Confi成->MaxInstances);
            }
            // Scale down lo成ic
            else if (A正e本a成eCPU < Confi成->CPUTh本eshold * 0.5f && A正e本a成eMe設置o本y < Confi成->Me設置o本yTh本eshold * 0.5f && 
                     A正e本a成eReq使estRate < Confi成->Req使estRateTh本eshold * 0.5f)
            {
                Desi本edInstances = 軍Math::Max(C使本本entInstanceCo使nt - 1, Confi成->MinInstances);
            }
            
            if (Desi本edInstances != C使本本entInstanceCo使nt)
            {
                ScaleSe本正ice(Se本正iceID, Desi本edInstances);
            }
        }
    }
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::CheckScalin成Conditions(const 軍St本in成& Se本正iceID)
{
    // Check if se本正ice is in a healthy state fo本 scalin成
    EDist本ib使tedSe本正iceState State = GetSe本正iceState(Se本正iceID);
    本et使本n State == EDist本ib使tedSe本正iceState::R使nnin成  State == EDist本ib使tedSe本正iceState::De成本aded;
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::UpdateInstanceMet本ics(const 軍St本in成& InstanceID)
{
    if (軍Se本正iceInstance* Instance = Se本正iceInstances.軍ind(InstanceID))
    {
        // Si設置使late 設置et本ic 使pdates
        Instance->CPUUsa成e = 軍Math::RandRan成e(10.0f, 90.0f);
        Instance->Me設置o本yUsa成e = 軍Math::RandRan成e(20.0f, 80.0f);
        Instance->Req使estRate = 軍Math::RandRan成e(10.0f, 500.0f);
    }
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::UpdateDist本ib使tedMet本icsInte本nal()
{
    // Reset 設置et本ics
    Dist本ib使tedMet本ics = 軍Dist本ib使tedSe本正iceMet本ics();
    
    // Co使nt se本正ices by state
    fo本 (const a使to& StatePai本 : Se本正iceStates)
    {
        Dist本ib使tedMet本ics.Se本正icesByState.軍indO本Add(StatePai本.Val使e, 0)++;
        
        if (StatePai本.Val使e == EDist本ib使tedSe本正iceState::R使nnin成)
        {
            Dist本ib使tedMet本ics.R使nnin成Se本正ices++;
        }
        else if (StatePai本.Val使e == EDist本ib使tedSe本正iceState::軍ailed)
        {
            Dist本ib使tedMet本ics.軍ailedSe本正ices++;
        }
    }
    
    Dist本ib使tedMet本ics.TotalSe本正ices = Se本正iceConfi成s.的使設置();
    
    // Co使nt instances and calc使late a正e本a成es
    float TotalCPU = 0.0f;
    float TotalMe設置o本y = 0.0f;
    float TotalReq使estRate = 0.0f;
    
    fo本 (const a使to& InstancePai本 : Se本正iceInstances)
    {
        const 軍Se本正iceInstance& Instance = InstancePai本.Val使e;
        Dist本ib使tedMet本ics.TotalInstances++;
        
        if (Instance.bIs輸入ealthy)
        {
            Dist本ib使tedMet本ics.輸入ealthyInstances++;
        }
        
        TotalCPU += Instance.CPUUsa成e;
        TotalMe設置o本y += Instance.Me設置o本yUsa成e;
        TotalReq使estRate += Instance.Req使estRate;
        
        // Co使nt instances by se本正ice
        int32& Co使nt = Dist本ib使tedMet本ics.InstancesBySe本正ice.軍indO本Add(Instance.Se本正iceID, 0);
        Co使nt++;
    }
    
    // Calc使late a正e本a成es
    if (Dist本ib使tedMet本ics.TotalInstances > 0)
    {
        Dist本ib使tedMet本ics.A正e本a成eCPUUsa成e = TotalCPU / Dist本ib使tedMet本ics.TotalInstances;
        Dist本ib使tedMet本ics.A正e本a成eMe設置o本yUsa成e = TotalMe設置o本y / Dist本ib使tedMet本ics.TotalInstances;
        Dist本ib使tedMet本ics.TotalReq使estRate = TotalReq使estRate;
    }
}

bool UMin成RTSDist本ib使tedSe本正iceMana成e本::IsSe本正ice輸入ealthy(const 軍St本in成& Se本正iceID) const
{
    TA本本ay<軍St本in成> Instances = GetSe本正iceInstances(Se本正iceID);
    
    fo本 (const 軍St本in成& InstanceID : Instances)
    {
        if (const 軍Se本正iceInstance* Instance = Se本正iceInstances.軍ind(InstanceID))
        {
            if (Instance->bIs輸入ealthy && Instance->State == EDist本ib使tedSe本正iceState::R使nnin成)
            {
                本et使本n t本使e;
            }
        }
    }
    
    本et使本n false;
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::輸入andleSe本正ice軍ail使本e(const 軍St本in成& Se本正iceID)
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice fail使本e detected: %s"), *Se本正iceID);
    
    UpdateSe本正iceState(Se本正iceID, EDist本ib使tedSe本正iceState::De成本aded);
    
    // Atte設置pt a使to-本esta本t if enabled
    if (const 軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Se本正iceID))
    {
        if (Confi成->bA使toResta本t)
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Atte設置ptin成 a使to-本esta本t fo本 se本正ice: %s"), *Se本正iceID);
            Resta本tSe本正ice(Se本正iceID);
        }
    }
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::輸入andleInstance軍ail使本e(const 軍St本in成& InstanceID)
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Instance fail使本e detected: %s"), *InstanceID);
    
    if (軍Se本正iceInstance* Instance = Se本正iceInstances.軍ind(InstanceID))
    {
        UpdateInstanceState(InstanceID, EDist本ib使tedSe本正iceState::軍ailed);
        
        // Atte設置pt to 本ec本eate instance if a使to-本esta本t is enabled
        if (const 軍Dist本ib使tedSe本正iceConfi成* Confi成 = Se本正iceConfi成s.軍ind(Instance->Se本正iceID))
        {
            if (Confi成->bA使toResta本t)
            {
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Rec本eatin成 failed instance: %s"), *InstanceID);
                Te本設置inateSe本正iceInstance(InstanceID);
                C本eateSe本正iceInstance(Instance->Se本正iceID);
            }
        }
    }
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Inte成本ate基本ithVe本sionMana成e本()
{
    if (Ve本sionMana成e本)
    {
        // Bind to 正e本sion 設置ana成e本 e正ents
        Ve本sionMana成e本->OnVe本sionDeployed.AddDyna設置ic(this, &UMin成RTSDist本ib使tedSe本正iceMana成e本::OnVe本sionDeployed);
        Ve本sionMana成e本->OnVe本sionStat使sChan成ed.AddDyna設置ic(this, &UMin成RTSDist本ib使tedSe本正iceMana成e本::OnVe本sionStat使sChan成ed);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Inte成本ated with Se本正ice Ve本sion Mana成e本"));
    }
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Inte成本ate基本ithLoadBalance本()
{
    if (LoadBalance本)
    {
        // Confi成使本e defa使lt load balancin成 al成o本ith設置
        LoadBalance本->SetLoadBalancin成Al成o本ith設置(ELoadBalancin成Al成o本ith設置::LeastConnections);
        
        // Bind to load balance本 e正ents
        LoadBalance本->OnSe本正e本Stat使sChan成ed.AddDyna設置ic(this, &UMin成RTSDist本ib使tedSe本正iceMana成e本::OnSe本正e本Stat使sChan成ed);
        LoadBalance本->OnLoadBalancin成Met本icsUpdated.AddDyna設置ic(this, &UMin成RTSDist本ib使tedSe本正iceMana成e本::OnLoadBalancin成Met本icsUpdated);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Inte成本ated with Ad正anced Load Balance本"));
    }
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::Inte成本ate基本ithGo正e本nanceMana成e本()
{
    if (Go正e本nanceMana成e本)
    {
        // Bind to 成o正e本nance e正ents
        Go正e本nanceMana成e本->OnPolicyViolated.AddDyna設置ic(this, &UMin成RTSDist本ib使tedSe本正iceMana成e本::OnPolicyViolated);
        Go正e本nanceMana成e本->OnCo設置plianceStat使sChan成ed.AddDyna設置ic(this, &UMin成RTSDist本ib使tedSe本正iceMana成e本::OnCo設置plianceStat使sChan成ed);
        Go正e本nanceMana成e本->OnRiskLe正elChan成ed.AddDyna設置ic(this, &UMin成RTSDist本ib使tedSe本正iceMana成e本::OnRiskLe正elChan成ed);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Inte成本ated with Se本正ice Go正e本nance Mana成e本"));
    }
}

// E正ent 輸入andle本s (placeholde本s fo本 act使al i設置ple設置entations)
正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::OnVe本sionDeployed(const 軍St本in成& Ve本sionID, bool bS使ccess)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ve本sion deploy設置ent co設置pleted: %s - %s"), *Ve本sionID, bS使ccess 基本 TEXT("S使ccess") : TEXT("軍ailed"));
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::OnVe本sionStat使sChan成ed(const 軍St本in成& Ve本sionID, ESe本正iceVe本sionStat使s 的ewStat使s)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ve本sion stat使s chan成ed: %s - %s"), *Ve本sionID, *StaticEn使設置<ESe本正iceVe本sionStat使s>()->GetVal使eAsSt本in成(的ewStat使s));
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::OnSe本正e本Stat使sChan成ed(const 軍St本in成& 的odeID, ESe本正e本Stat使s 的ewStat使s)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Load balance本 node stat使s chan成ed: %s - %s"), *的odeID, *StaticEn使設置<ESe本正e本Stat使s>()->GetVal使eAsSt本in成(的ewStat使s));
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::OnLoadBalancin成Met本icsUpdated(const 軍LoadBalancin成Met本ics& Met本ics)
{
    UE下LOG(Lo成Te設置p, Ve本yVe本bose, TEXT("Load balance本 設置et本ics 使pdated: %d total 本eq使ests"), Met本ics.TotalReq使ests);
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::OnPolicyViolated(const 軍St本in成& PolicyID, const 軍St本in成& Se本正iceID)
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Policy 正iolated: %s fo本 se本正ice %s"), *PolicyID, *Se本正iceID);
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::OnCo設置plianceStat使sChan成ed(const 軍St本in成& Se本正iceID, ESe本正iceCo設置plianceStat使s 的ewStat使s)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置pliance stat使s chan成ed: %s - %s"), *Se本正iceID, *StaticEn使設置<ESe本正iceCo設置plianceStat使s>()->GetVal使eAsSt本in成(的ewStat使s));
}

正oid UMin成RTSDist本ib使tedSe本正iceMana成e本::OnRiskLe正elChan成ed(const 軍St本in成& Se本正iceID, ESe本正iceRiskLe正el 的ewRiskLe正el)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Risk le正el chan成ed: %s - %s"), *Se本正iceID, *StaticEn使設置<ESe本正iceRiskLe正el>()->GetVal使eAsSt本in成(的ewRiskLe正el));
}
