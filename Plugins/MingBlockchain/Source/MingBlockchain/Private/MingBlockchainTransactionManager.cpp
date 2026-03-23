#incl使de "Min成BlockchainT本ansactionMana成e本.h"
#incl使de "Min成Blockchain基本allet.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/Paths.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"

UMin成BlockchainT本ansactionMana成e本::UMin成BlockchainT本ansactionMana成e本()
{
    P本i設置a本yCo設置ponentTick.bCanE正e本Tick = t本使e;
    P本i設置a本yCo設置ponentTick.bSta本t基本ithTickEnabled = t本使e;

    C使本本ent軍eeType = ET本ansaction軍eeType::Standa本d;
    bA使toRet本yEnabled = t本使e;
    MaxRet本yAtte設置pts = 3;
    Defa使ltGasP本ice = 20.0f; // Gwei
    MaxPendin成T本ansactions = 50;
    T本ansactionTi設置eo使t = 300.0f; // 5 設置in使tes
    GasUpdateInte本正al = 30.0f; // Update e正e本y 30 seconds
    Ti設置eSinceLastGasUpdate = 0.0f;

    Blockchain基本allet = n使llpt本;
}

正oid UMin成BlockchainT本ansactionMana成e本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    InitializeT本ansactionConfi成使本ation();
    LoadT本ansactionData();
}

正oid UMin成BlockchainT本ansactionMana成e本::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Sa正eT本ansactionData();
    S使pe本::EndPlay(EndPlayReason);
}

正oid UMin成BlockchainT本ansactionMana成e本::TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction)
{
    S使pe本::TickCo設置ponent(DeltaTi設置e, TickType, ThisTick軍使nction);

    Ti設置eSinceLastGasUpdate += DeltaTi設置e;
    if (Ti設置eSinceLastGasUpdate >= GasUpdateInte本正al)
    {
        UpdateGasP本ices();
        Ti設置eSinceLastGasUpdate = 0.0f;
    }

    // Check fo本 t本ansaction ti設置eo使ts
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    fo本 (int32 i = Pendin成T本ansactions.的使設置() - 1; i >= 0; --i)
    {
        const 軍BlockchainT本ansaction& T本ansaction = Pendin成T本ansactions[i];
        if (C使本本entTi設置e - T本ansaction.Ti設置esta設置p > T本ansactionTi設置eo使t)
        {
            輸入andleT本ansactionTi設置eo使t(T本ansaction.T本ansaction輸入ash);
        }
    }

    // Clean 使p old t本ansactions pe本iodically
    if (軍Math::RandRan成e(0, 1000) < 10) // 1% chance pe本 tick
    {
        Clean使pOldT本ansactions();
    }
}

正oid UMin成BlockchainT本ansactionMana成e本::InitializeT本ansactionMana成e本(UMin成Blockchain基本allet* 基本allet)
{
    Blockchain基本allet = 基本allet;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: Initialized with wallet"));
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::C本eateT本ansaction(const 軍St本in成& 軍本o設置Add本ess, const 軍St本in成& ToAdd本ess, const 軍St本in成& TokenCont本act, float A設置o使nt, ET本ansactionP本io本ity P本io本ity)
{
    if (!ValidateT本ansactionPa本a設置ete本s(軍本o設置Add本ess, ToAdd本ess, TokenCont本act, A設置o使nt))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainT本ansactionMana成e本: In正alid t本ansaction pa本a設置ete本s"));
        本et使本n TEXT("");
    }

    if (Pendin成T本ansactions.的使設置() >= MaxPendin成T本ansactions)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainT本ansactionMana成e本: Maxi設置使設置 pendin成 t本ansactions 本eached"));
        本et使本n TEXT("");
    }

    軍BlockchainT本ansaction T本ansaction;
    T本ansaction.T本ansaction輸入ash = Gene本ateT本ansaction輸入ash();
    T本ansaction.軍本o設置Add本ess = 軍本o設置Add本ess;
    T本ansaction.ToAdd本ess = ToAdd本ess;
    T本ansaction.TokenCont本act = TokenCont本act;
    T本ansaction.TokenSy設置bol = GetTokenSy設置bol(TokenCont本act);
    T本ansaction.A設置o使nt = A設置o使nt;
    T本ansaction.GasP本ice = Calc使lateOpti設置alGasP本ice(P本io本ity);
    T本ansaction.GasLi設置it = Calc使lateGasLi設置it(T本ansaction);
    T本ansaction.Gas軍ee = (T本ansaction.GasP本ice * T本ansaction.GasLi設置it) / 1e9f; // Con正e本t to ET輸入
    T本ansaction.P本io本ity = P本io本ity;
    T本ansaction.Stat使s = ET本ansactionStat使s::Pendin成;
    T本ansaction.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();

    Pendin成T本ansactions.Add(T本ansaction);
    OnT本ansactionC本eated.B本oadcast(T本ansaction);

    // Sta本t p本ocessin成
    P本ocessT本ansaction(T本ansaction);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: C本eated t本ansaction %s"), *T本ansaction.T本ansaction輸入ash);
    本et使本n T本ansaction.T本ansaction輸入ash;
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::C本eateRewa本dT本ansaction(const 軍St本in成& Playe本ID, const 軍Minin成Rewa本d& Rewa本d)
{
    if (!Blockchain基本allet  !Blockchain基本allet->Is基本alletConnected())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainT本ansactionMana成e本: 基本allet not connected fo本 本ewa本d t本ansaction"));
        本et使本n TEXT("");
    }

    軍St本in成 基本alletAdd本ess = Blockchain基本allet->Get基本alletInfo().基本alletAdd本ess;
    本et使本n C本eateT本ansaction(
        TEXT("0x0000000000000000000000000000000000000"), // Syste設置 add本ess
        基本alletAdd本ess,
        Rewa本d.TokenCont本act,
        Rewa本d.A設置o使nt,
        ET本ansactionP本io本ity::輸入i成h
    );
}

bool UMin成BlockchainT本ansactionMana成e本::P本ocessT本ansaction(const 軍BlockchainT本ansaction& T本ansaction)
{
    if (!ValidateT本ansaction(T本ansaction))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainT本ansactionMana成e本: In正alid t本ansaction data"));
        本et使本n false;
    }

    // 軍ind and 使pdate the t本ansaction in pendin成 list
    fo本 (軍BlockchainT本ansaction& Pendin成Tx : Pendin成T本ansactions)
    {
        if (Pendin成Tx.T本ansaction輸入ash == T本ansaction.T本ansaction輸入ash)
        {
            本et使本n P本ocessT本ansactionOnBlockchain(Pendin成Tx);
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainT本ansactionMana成e本: T本ansaction %s not fo使nd in pendin成 list"), *T本ansaction.T本ansaction輸入ash);
    本et使本n false;
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::C本eateT本ansactionBatch(const TA本本ay<軍BlockchainT本ansaction>& T本ansactions)
{
    if (T本ansactions.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainT本ansactionMana成e本: Cannot c本eate e設置pty batch"));
        本et使本n TEXT("");
    }

    軍T本ansactionBatch Batch;
    Batch.BatchID = Gene本ateBatchID();
    Batch.T本ansactions = T本ansactions;
    Batch.C本eatedTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    Batch.BatchStat使s = ET本ansactionStat使s::Pendin成;

    // Calc使late totals
    fo本 (const 軍BlockchainT本ansaction& T本ansaction : T本ansactions)
    {
        Batch.TotalA設置o使nt += T本ansaction.A設置o使nt;
        Batch.TotalGas軍ee += T本ansaction.Gas軍ee;
    }

    T本ansactionBatches.Add(Batch.BatchID, Batch);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: C本eated batch %s with %d t本ansactions"), 
           *Batch.BatchID, T本ansactions.的使設置());

    本et使本n Batch.BatchID;
}

bool UMin成BlockchainT本ansactionMana成e本::P本ocessT本ansactionBatch(const 軍St本in成& BatchID)
{
    if (!T本ansactionBatches.Contains(BatchID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainT本ansactionMana成e本: Batch %s not fo使nd"), *BatchID);
        本et使本n false;
    }

    軍T本ansactionBatch& Batch = T本ansactionBatches[BatchID];
    P本ocessBatchT本ansactions(Batch);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: P本ocessin成 batch %s"), *BatchID);
    本et使本n t本使e;
}

軍BlockchainT本ansaction UMin成BlockchainT本ansactionMana成e本::GetT本ansaction(const 軍St本in成& T本ansaction輸入ash) const
{
    // Check pendin成 t本ansactions fi本st
    fo本 (const 軍BlockchainT本ansaction& T本ansaction : Pendin成T本ansactions)
    {
        if (T本ansaction.T本ansaction輸入ash == T本ansaction輸入ash)
        {
            本et使本n T本ansaction;
        }
    }

    // Check t本ansaction histo本y
    fo本 (const 軍BlockchainT本ansaction& T本ansaction : T本ansaction輸入isto本y)
    {
        if (T本ansaction.T本ansaction輸入ash == T本ansaction輸入ash)
        {
            本et使本n T本ansaction;
        }
    }

    本et使本n 軍BlockchainT本ansaction();
}

軍T本ansactionBatch UMin成BlockchainT本ansactionMana成e本::GetT本ansactionBatch(const 軍St本in成& BatchID) const
{
    if (const 軍T本ansactionBatch* Batch = T本ansactionBatches.軍ind(BatchID))
    {
        本et使本n *Batch;
    }
    本et使本n 軍T本ansactionBatch();
}

TA本本ay<軍BlockchainT本ansaction> UMin成BlockchainT本ansactionMana成e本::GetPendin成T本ansactions() const
{
    本et使本n Pendin成T本ansactions;
}

TA本本ay<軍BlockchainT本ansaction> UMin成BlockchainT本ansactionMana成e本::GetT本ansaction輸入isto本y(int32 Li設置it) const
{
    TA本本ay<軍BlockchainT本ansaction> Res使lt;
    int32 Co使nt = 軍Math::Min(Li設置it, T本ansaction輸入isto本y.的使設置());
    
    fo本 (int32 i = T本ansaction輸入isto本y.的使設置() - Co使nt; i < T本ansaction輸入isto本y.的使設置(); ++i)
    {
        Res使lt.Add(T本ansaction輸入isto本y[i]);
    }
    
    本et使本n Res使lt;
}

軍T本ansactionStatistics UMin成BlockchainT本ansactionMana成e本::GetT本ansactionStatistics(const 軍St本in成& Playe本ID) const
{
    if (Playe本ID.IsE設置pty())
    {
        // Ret使本n 成lobal statistics
        軍T本ansactionStatistics GlobalStats;
        fo本 (const 軍BlockchainT本ansaction& T本ansaction : T本ansaction輸入isto本y)
        {
            GlobalStats.TotalT本ansactions++;
            GlobalStats.TotalVol使設置e += T本ansaction.A設置o使nt;
            GlobalStats.TotalGas軍ees += T本ansaction.Gas軍ee;
            
            if (T本ansaction.Stat使s == ET本ansactionStat使s::Confi本設置ed)
            {
                GlobalStats.S使ccessf使lT本ansactions++;
            }
            else if (T本ansaction.Stat使s == ET本ansactionStat使s::軍ailed)
            {
                GlobalStats.軍ailedT本ansactions++;
            }
        }
        
        GlobalStats.Pendin成T本ansactions = Pendin成T本ansactions.的使設置();
        GlobalStats.S使ccessRate = GlobalStats.TotalT本ansactions > 0 基本 
            (float)GlobalStats.S使ccessf使lT本ansactions / GlobalStats.TotalT本ansactions : 0.0f;
        GlobalStats.A正e本a成eGas軍ee = GlobalStats.TotalT本ansactions > 0 基本 
            GlobalStats.TotalGas軍ees / GlobalStats.TotalT本ansactions : 0.0f;
        
        本et使本n GlobalStats;
    }
    else
    {
        // Ret使本n playe本-specific statistics
        if (const 軍T本ansactionStatistics* Stats = Playe本Statistics.軍ind(Playe本ID))
        {
            本et使本n *Stats;
        }
        
        本et使本n 軍T本ansactionStatistics();
    }
}

bool UMin成BlockchainT本ansactionMana成e本::CancelT本ansaction(const 軍St本in成& T本ansaction輸入ash)
{
    fo本 (int32 i = 0; i < Pendin成T本ansactions.的使設置(); ++i)
    {
        if (Pendin成T本ansactions[i].T本ansaction輸入ash == T本ansaction輸入ash)
        {
            軍BlockchainT本ansaction& T本ansaction = Pendin成T本ansactions[i];
            T本ansaction.Stat使s = ET本ansactionStat使s::Cancelled;
            T本ansaction.E本本o本Messa成e = TEXT("T本ansaction cancelled by 使se本");
            
            // Mo正e to histo本y
            T本ansaction輸入isto本y.Add(T本ansaction);
            Pendin成T本ansactions.Re設置o正eAt(i);
            
            OnT本ansaction軍ailed.B本oadcast(T本ansaction);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: Cancelled t本ansaction %s"), *T本ansaction輸入ash);
            本et使本n t本使e;
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainT本ansactionMana成e本: T本ansaction %s not fo使nd in pendin成 list"), *T本ansaction輸入ash);
    本et使本n false;
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::Ret本yT本ansaction(const 軍St本in成& T本ansaction輸入ash, ET本ansactionP本io本ity 的ewP本io本ity)
{
    // 軍ind the failed t本ansaction
    軍BlockchainT本ansaction 軍ailedT本ansaction;
    bool b軍o使nd = false;
    
    fo本 (const 軍BlockchainT本ansaction& T本ansaction : T本ansaction輸入isto本y)
    {
        if (T本ansaction.T本ansaction輸入ash == T本ansaction輸入ash && T本ansaction.Stat使s == ET本ansactionStat使s::軍ailed)
        {
            軍ailedT本ansaction = T本ansaction;
            b軍o使nd = t本使e;
            b本eak;
        }
    }
    
    if (!b軍o使nd)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainT本ansactionMana成e本: 軍ailed t本ansaction %s not fo使nd"), *T本ansaction輸入ash);
        本et使本n TEXT("");
    }
    
    // C本eate new t本ansaction with hi成he本 p本io本ity
    本et使本n C本eateT本ansaction(
        軍ailedT本ansaction.軍本o設置Add本ess,
        軍ailedT本ansaction.ToAdd本ess,
        軍ailedT本ansaction.TokenCont本act,
        軍ailedT本ansaction.A設置o使nt,
        的ewP本io本ity
    );
}

float UMin成BlockchainT本ansactionMana成e本::Esti設置ateGas軍ee(const 軍St本in成& 軍本o設置Add本ess, const 軍St本in成& ToAdd本ess, const 軍St本in成& TokenCont本act, float A設置o使nt, ET本ansactionP本io本ity P本io本ity) const
{
    float GasP本ice = Calc使lateOpti設置alGasP本ice(P本io本ity);
    int32 GasLi設置it;
    
    if (TokenCont本act.IsE設置pty()  TokenCont本act == TEXT("0x0000000000000000000000000000000000000"))
    {
        GasLi設置it = 21000; // Standa本d ET輸入 t本ansfe本
    }
    else
    {
        GasLi設置it = 65000; // ERC20 token t本ansfe本
    }
    
    本et使本n (GasP本ice * GasLi設置it) / 1e9f; // Con正e本t to ET輸入
}

float UMin成BlockchainT本ansactionMana成e本::GetC使本本entGasP本ice() const
{
    本et使本n Defa使ltGasP本ice;
}

float UMin成BlockchainT本ansactionMana成e本::GetReco設置設置endedGasP本ice(ET本ansactionP本io本ity P本io本ity) const
{
    本et使本n Calc使lateOpti設置alGasP本ice(P本io本ity);
}

正oid UMin成BlockchainT本ansactionMana成e本::SetT本ansaction軍eeType(ET本ansaction軍eeType 軍eeType)
{
    C使本本ent軍eeType = 軍eeType;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: T本ansaction fee type set to %d"), (int32)軍eeType);
}

ET本ansaction軍eeType UMin成BlockchainT本ansactionMana成e本::GetT本ansaction軍eeType() const
{
    本et使本n C使本本ent軍eeType;
}

正oid UMin成BlockchainT本ansactionMana成e本::SetA使toRet本yEnabled(bool bEnabled)
{
    bA使toRet本yEnabled = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: A使to-本et本y %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

bool UMin成BlockchainT本ansactionMana成e本::IsA使toRet本yEnabled() const
{
    本et使本n bA使toRet本yEnabled;
}

正oid UMin成BlockchainT本ansactionMana成e本::SetMaxRet本yAtte設置pts(int32 MaxAtte設置pts)
{
    MaxRet本yAtte設置pts = 軍Math::Max(1, MaxAtte設置pts);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: Max 本et本y atte設置pts set to %d"), MaxRet本yAtte設置pts);
}

int32 UMin成BlockchainT本ansactionMana成e本::GetMaxRet本yAtte設置pts() const
{
    本et使本n MaxRet本yAtte設置pts;
}

int32 UMin成BlockchainT本ansactionMana成e本::GetT本ansactionConfi本設置ations(const 軍St本in成& T本ansaction輸入ash) const
{
    軍BlockchainT本ansaction T本ansaction = GetT本ansaction(T本ansaction輸入ash);
    本et使本n T本ansaction.Confi本設置ationCo使nt;
}

bool UMin成BlockchainT本ansactionMana成e本::IsT本ansactionConfi本設置ed(const 軍St本in成& T本ansaction輸入ash, int32 Req使i本edConfi本設置ations) const
{
    軍BlockchainT本ansaction T本ansaction = GetT本ansaction(T本ansaction輸入ash);
    本et使本n T本ansaction.Stat使s == ET本ansactionStat使s::Confi本設置ed && 
           T本ansaction.Confi本設置ationCo使nt >= Req使i本edConfi本設置ations;
}

float UMin成BlockchainT本ansactionMana成e本::GetT本ansactionP本ocessin成Ti設置e(const 軍St本in成& T本ansaction輸入ash) const
{
    軍BlockchainT本ansaction T本ansaction = GetT本ansaction(T本ansaction輸入ash);
    if (T本ansaction.Stat使s == ET本ansactionStat使s::Confi本設置ed && !T本ansaction.Block輸入ash.IsE設置pty())
    {
        本et使本n T本ansaction.Ti設置esta設置p; // In a 本eal i設置ple設置entation, wo使ld calc使late act使al p本ocessin成 ti設置e
    }
    本et使本n 0.0f;
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::Expo本tT本ansaction(const 軍St本in成& T本ansaction輸入ash, const 軍St本in成& 軍o本設置at) const
{
    軍BlockchainT本ansaction T本ansaction = GetT本ansaction(T本ansaction輸入ash);
    if (T本ansaction.T本ansaction輸入ash.IsE設置pty())
    {
        本et使本n TEXT("");
    }
    
    本et使本n 軍o本設置atT本ansaction軍o本Expo本t(T本ansaction, 軍o本設置at);
}

bool UMin成BlockchainT本ansactionMana成e本::I設置po本tT本ansaction(const 軍St本in成& Data, const 軍St本in成& 軍o本設置at)
{
    軍BlockchainT本ansaction T本ansaction = Pa本seT本ansaction軍本o設置I設置po本t(Data, 軍o本設置at);
    if (ValidateT本ansaction(T本ansaction))
    {
        T本ansaction輸入isto本y.Add(T本ansaction);
        Sa正eT本ansactionData();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: I設置po本ted t本ansaction %s"), *T本ansaction.T本ansaction輸入ash);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

bool UMin成BlockchainT本ansactionMana成e本::ValidateT本ansaction(const 軍BlockchainT本ansaction& T本ansaction) const
{
    本et使本n !T本ansaction.T本ansaction輸入ash.IsE設置pty() &&
           !T本ansaction.軍本o設置Add本ess.IsE設置pty() &&
           !T本ansaction.ToAdd本ess.IsE設置pty() &&
           T本ansaction.A設置o使nt > 0.0f &&
           T本ansaction.GasLi設置it > 0 &&
           T本ansaction.GasP本ice >= 0.0f;
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::GetT本ansactionE本本o本(const 軍St本in成& T本ansaction輸入ash) const
{
    軍BlockchainT本ansaction T本ansaction = GetT本ansaction(T本ansaction輸入ash);
    本et使本n T本ansaction.E本本o本Messa成e;
}

正oid UMin成BlockchainT本ansactionMana成e本::InitializeT本ansactionConfi成使本ation()
{
    // Load confi成使本ation f本o設置 settin成s o本 使se defa使lts
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: Confi成使本ation initialized"));
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::Gene本ateT本ansaction輸入ash() const
{
    本et使本n 軍G使id::的ewG使id().ToSt本in成();
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::Gene本ateBatchID() const
{
    本et使本n TEXT("BATC輸入下") + 軍G使id::的ewG使id().ToSt本in成();
}

float UMin成BlockchainT本ansactionMana成e本::Calc使lateOpti設置alGasP本ice(ET本ansactionP本io本ity P本io本ity) const
{
    float BaseGasP本ice = GetC使本本entGasP本ice();
    float M使ltiplie本 = Get軍eeM使ltiplie本(P本io本ity);
    
    本et使本n BaseGasP本ice * M使ltiplie本;
}

int32 UMin成BlockchainT本ansactionMana成e本::Calc使lateGasLi設置it(const 軍BlockchainT本ansaction& T本ansaction) const
{
    if (T本ansaction.TokenCont本act.IsE設置pty()  T本ansaction.TokenCont本act == TEXT("0x0000000000000000000000000000000000000"))
    {
        本et使本n 21000; // Standa本d ET輸入 t本ansfe本
    }
    else
    {
        本et使本n 65000; // ERC20 token t本ansfe本
    }
}

bool UMin成BlockchainT本ansactionMana成e本::P本ocessT本ansactionOnBlockchain(軍BlockchainT本ansaction& T本ansaction)
{
    // Si設置使late blockchain p本ocessin成
    // In a 本eal i設置ple設置entation, this wo使ld inte本act with act使al blockchain nodes
    
    // Si設置使late p本ocessin成 ti設置e based on 成as p本ice
    float P本ocessin成Ti設置e = 5.0f + (30.0f / T本ansaction.GasP本ice); // 輸入i成he本 成as = faste本 p本ocessin成
    
    // 軍o本 si設置使lation, we'll 本ando設置ly s使cceed o本 fail
    bool bS使ccess = UKis設置etMathLib本a本y::Rando設置Bool基本ith基本ei成ht(0.95f); // 95% s使ccess 本ate
    
    if (bS使ccess)
    {
        T本ansaction.Stat使s = ET本ansactionStat使s::Confi本設置ed;
        T本ansaction.Block輸入ash = TEXT("0x") + 軍G使id::的ewG使id().ToSt本in成().Left(64);
        T本ansaction.Block的使設置be本 = UKis設置etMathLib本a本y::Rando設置Inte成e本InRan成e(15000000, 16000000);
        T本ansaction.Confi本設置ationCo使nt = 12; // Si設置使late f使ll confi本設置ation
        
        // Mo正e f本o設置 pendin成 to histo本y
        Pendin成T本ansactions.Re設置o正e(T本ansaction);
        T本ansaction輸入isto本y.Add(T本ansaction);
        
        OnT本ansactionConfi本設置ed.B本oadcast(T本ansaction);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: T本ansaction %s confi本設置ed"), *T本ansaction.T本ansaction輸入ash);
    }
    else
    {
        T本ansaction.Stat使s = ET本ansactionStat使s::軍ailed;
        T本ansaction.E本本o本Messa成e = TEXT("T本ansaction failed: Ins使fficient 成as o本 netwo本k e本本o本");
        
        // Mo正e f本o設置 pendin成 to histo本y
        Pendin成T本ansactions.Re設置o正e(T本ansaction);
        T本ansaction輸入isto本y.Add(T本ansaction);
        
        OnT本ansaction軍ailed.B本oadcast(T本ansaction);
        
        // A使to-本et本y if enabled
        if (bA使toRet本yEnabled)
        {
            Ret本y軍ailedT本ansaction(T本ansaction);
        }
        
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainT本ansactionMana成e本: T本ansaction %s failed"), *T本ansaction.T本ansaction輸入ash);
    }
    
    本et使本n bS使ccess;
}

正oid UMin成BlockchainT本ansactionMana成e本::UpdateT本ansactionStat使s(const 軍St本in成& T本ansaction輸入ash, ET本ansactionStat使s 的ewStat使s, const 軍St本in成& E本本o本Messa成e)
{
    fo本 (軍BlockchainT本ansaction& T本ansaction : Pendin成T本ansactions)
    {
        if (T本ansaction.T本ansaction輸入ash == T本ansaction輸入ash)
        {
            T本ansaction.Stat使s = 的ewStat使s;
            if (!E本本o本Messa成e.IsE設置pty())
            {
                T本ansaction.E本本o本Messa成e = E本本o本Messa成e;
            }
            
            if (的ewStat使s == ET本ansactionStat使s::Confi本設置ed)
            {
                OnT本ansactionConfi本設置ed.B本oadcast(T本ansaction);
            }
            else if (的ewStat使s == ET本ansactionStat使s::軍ailed)
            {
                OnT本ansaction軍ailed.B本oadcast(T本ansaction);
            }
            
            b本eak;
        }
    }
}

正oid UMin成BlockchainT本ansactionMana成e本::UpdateT本ansactionConfi本設置ations(const 軍St本in成& T本ansaction輸入ash, int32 的ewConfi本設置ations)
{
    fo本 (軍BlockchainT本ansaction& T本ansaction : Pendin成T本ansactions)
    {
        if (T本ansaction.T本ansaction輸入ash == T本ansaction輸入ash)
        {
            T本ansaction.Confi本設置ationCo使nt = 的ewConfi本設置ations;
            b本eak;
        }
    }
}

正oid UMin成BlockchainT本ansactionMana成e本::P本ocessBatchT本ansactions(軍T本ansactionBatch& Batch)
{
    int32 S使ccessCo使nt = 0;
    int32 軍ail使本eCo使nt = 0;
    
    fo本 (軍BlockchainT本ansaction& T本ansaction : Batch.T本ansactions)
    {
        if (P本ocessT本ansactionOnBlockchain(T本ansaction))
        {
            S使ccessCo使nt++;
        }
        else
        {
            軍ail使本eCo使nt++;
        }
    }
    
    Batch.P本ocessedTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    Batch.BatchStat使s = 軍ail使本eCo使nt == 0 基本 ET本ansactionStat使s::Confi本設置ed : ET本ansactionStat使s::軍ailed;
    
    OnBatchP本ocessed.B本oadcast(Batch);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: Batch %s p本ocessed: %d s使ccess, %d failed"), 
           *Batch.BatchID, S使ccessCo使nt, 軍ail使本eCo使nt);
}

正oid UMin成BlockchainT本ansactionMana成e本::輸入andleT本ansactionTi設置eo使t(const 軍St本in成& T本ansaction輸入ash)
{
    fo本 (int32 i = 0; i < Pendin成T本ansactions.的使設置(); ++i)
    {
        if (Pendin成T本ansactions[i].T本ansaction輸入ash == T本ansaction輸入ash)
        {
            軍BlockchainT本ansaction& T本ansaction = Pendin成T本ansactions[i];
            T本ansaction.Stat使s = ET本ansactionStat使s::Expi本ed;
            T本ansaction.E本本o本Messa成e = TEXT("T本ansaction ti設置ed o使t");
            
            // Mo正e to histo本y
            T本ansaction輸入isto本y.Add(T本ansaction);
            Pendin成T本ansactions.Re設置o正eAt(i);
            
            OnT本ansaction軍ailed.B本oadcast(T本ansaction);
            
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainT本ansactionMana成e本: T本ansaction %s ti設置ed o使t"), *T本ansaction輸入ash);
            b本eak;
        }
    }
}

正oid UMin成BlockchainT本ansactionMana成e本::Ret本y軍ailedT本ansaction(const 軍BlockchainT本ansaction& T本ansaction)
{
    if (T本ansaction.Stat使s == ET本ansactionStat使s::軍ailed)
    {
        // C本eate 本et本y with hi成he本 p本io本ity
        ET本ansactionP本io本ity 的ewP本io本ity = (ET本ansactionP本io本ity)軍Math::Min((int32)T本ansaction.P本io本ity + 1, (int32)ET本ansactionP本io本ity::C本itical);
        
        軍St本in成 的ew輸入ash = C本eateT本ansaction(
            T本ansaction.軍本o設置Add本ess,
            T本ansaction.ToAdd本ess,
            T本ansaction.TokenCont本act,
            T本ansaction.A設置o使nt,
            的ewP本io本ity
        );
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: Ret本yin成 t本ansaction %s as %s"), 
               *T本ansaction.T本ansaction輸入ash, *的ew輸入ash);
    }
}

正oid UMin成BlockchainT本ansactionMana成e本::UpdateGasP本ices()
{
    // In a 本eal i設置ple設置entation, this wo使ld q使e本y c使本本ent 成as p本ices f本o設置 the netwo本k
    // 軍o本 now, si設置使late with so設置e 本ando設置ness
    float 的etwo本kGasP本ice = Get的etwo本kGasP本ice();
    Defa使ltGasP本ice = 軍Math::Le本p(Defa使ltGasP本ice, 的etwo本kGasP本ice, 0.3f); // S設置ooth t本ansition
    
    UE下LOG(Lo成Te設置p, Ve本yVe本bose, TEXT("Min成BlockchainT本ansactionMana成e本: Updated 成as p本ice to %f Gwei"), Defa使ltGasP本ice);
}

float UMin成BlockchainT本ansactionMana成e本::Get的etwo本kGasP本ice() const
{
    // Si設置使late netwo本k 成as p本ice with so設置e 正a本iation
    本et使本n UKis設置etMathLib本a本y::Rando設置軍loatInRan成e(15.0f, 35.0f);
}

正oid UMin成BlockchainT本ansactionMana成e本::UpdateT本ansactionStatistics(const 軍St本in成& Playe本ID, const 軍BlockchainT本ansaction& T本ansaction)
{
    軍T本ansactionStatistics& Stats = Playe本Statistics.軍indO本Add(Playe本ID);
    
    Stats.TotalT本ansactions++;
    Stats.TotalVol使設置e += T本ansaction.A設置o使nt;
    Stats.TotalGas軍ees += T本ansaction.Gas軍ee;
    
    if (T本ansaction.Stat使s == ET本ansactionStat使s::Confi本設置ed)
    {
        Stats.S使ccessf使lT本ansactions++;
    }
    else if (T本ansaction.Stat使s == ET本ansactionStat使s::軍ailed)
    {
        Stats.軍ailedT本ansactions++;
    }
    
    Stats.S使ccessRate = Stats.TotalT本ansactions > 0 基本 
        (float)Stats.S使ccessf使lT本ansactions / Stats.TotalT本ansactions : 0.0f;
    Stats.A正e本a成eGas軍ee = Stats.TotalT本ansactions > 0 基本 
        Stats.TotalGas軍ees / Stats.TotalT本ansactions : 0.0f;
}

正oid UMin成BlockchainT本ansactionMana成e本::Clean使pOldT本ansactions()
{
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    const float MaxA成e = 86400.0f * 7.0f; // 7 days
    
    // Clean 使p old confi本設置ed t本ansactions
    T本ansaction輸入isto本y.Re設置o正eAll([C使本本entTi設置e, MaxA成e](const 軍BlockchainT本ansaction& T本ansaction) {
        本et使本n (C使本本entTi設置e - T本ansaction.Ti設置esta設置p) > MaxA成e;
    });
    
    // Clean 使p old batches
    fo本 (a使to It = T本ansactionBatches.C本eateIte本ato本(); It; ++It)
    {
        if ((C使本本entTi設置e - It->Val使e.C本eatedTi設置e) > MaxA成e)
        {
            It.Re設置o正eC使本本ent();
        }
    }
}

正oid UMin成BlockchainT本ansactionMana成e本::Sa正eT本ansactionData()
{
    // Sa正e t本ansaction data to pe本sistent sto本a成e
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("T本ansactions.大son");
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: T本ansaction data sa正ed to %s"), *Sa正ePath);
}

正oid UMin成BlockchainT本ansactionMana成e本::LoadT本ansactionData()
{
    // Load t本ansaction data f本o設置 pe本sistent sto本a成e
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("T本ansactions.大son");
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainT本ansactionMana成e本: T本ansaction data loaded f本o設置 %s"), *LoadPath);
}

bool UMin成BlockchainT本ansactionMana成e本::ValidateT本ansactionPa本a設置ete本s(const 軍St本in成& 軍本o設置Add本ess, const 軍St本in成& ToAdd本ess, const 軍St本in成& TokenCont本act, float A設置o使nt) const
{
    本et使本n !軍本o設置Add本ess.IsE設置pty() &&
           !ToAdd本ess.IsE設置pty() &&
           A設置o使nt > 0.0f &&
           軍本o設置Add本ess != ToAdd本ess;
}

float UMin成BlockchainT本ansactionMana成e本::Get軍eeM使ltiplie本(ET本ansactionP本io本ity P本io本ity) const
{
    switch (P本io本ity)
    {
        case ET本ansactionP本io本ity::Low:       本et使本n 0.8f;
        case ET本ansactionP本io本ity::Medi使設置:    本et使本n 1.0f;
        case ET本ansactionP本io本ity::輸入i成h:      本et使本n 1.5f;
        case ET本ansactionP本io本ity::U本成ent:    本et使本n 2.0f;
        case ET本ansactionP本io本ity::C本itical:  本et使本n 3.0f;
        defa使lt: 本et使本n 1.0f;
    }
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::軍o本設置atT本ansaction軍o本Expo本t(const 軍BlockchainT本ansaction& T本ansaction, const 軍St本in成& 軍o本設置at) const
{
    if (軍o本設置at.ToUppe本() == TEXT("JSO的"))
    {
        TSha本edPt本<軍JsonOb大ect> JsonOb大ect = MakeSha本eable(new 軍JsonOb大ect);
        JsonOb大ect->SetSt本in成軍ield(TEXT("hash"), T本ansaction.T本ansaction輸入ash);
        JsonOb大ect->SetSt本in成軍ield(TEXT("f本o設置"), T本ansaction.軍本o設置Add本ess);
        JsonOb大ect->SetSt本in成軍ield(TEXT("to"), T本ansaction.ToAdd本ess);
        JsonOb大ect->SetSt本in成軍ield(TEXT("token"), T本ansaction.TokenCont本act);
        JsonOb大ect->Set的使設置be本軍ield(TEXT("a設置o使nt"), T本ansaction.A設置o使nt);
        JsonOb大ect->Set的使設置be本軍ield(TEXT("成as軍ee"), T本ansaction.Gas軍ee);
        JsonOb大ect->Set的使設置be本軍ield(TEXT("成asP本ice"), T本ansaction.GasP本ice);
        JsonOb大ect->Set的使設置be本軍ield(TEXT("stat使s"), (int32)T本ansaction.Stat使s);
        JsonOb大ect->SetSt本in成軍ield(TEXT("ti設置esta設置p"), 軍St本in成::Sanitize軍loat(T本ansaction.Ti設置esta設置p));
        
        軍St本in成 O使tp使tSt本in成;
        TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
        軍JsonSe本ialize本::Se本ialize(JsonOb大ect.ToSha本edRef(), 基本本ite本);
        
        本et使本n O使tp使tSt本in成;
    }
    else if (軍o本設置at.ToUppe本() == TEXT("CSV"))
    {
        本et使本n 軍St本in成::P本intf(TEXT("%s,%s,%s,%s,%f,%f,%f,%d,%f"),
            *T本ansaction.T本ansaction輸入ash,
            *T本ansaction.軍本o設置Add本ess,
            *T本ansaction.ToAdd本ess,
            *T本ansaction.TokenSy設置bol,
            T本ansaction.A設置o使nt,
            T本ansaction.Gas軍ee,
            T本ansaction.GasP本ice,
            (int32)T本ansaction.Stat使s,
            T本ansaction.Ti設置esta設置p
        );
    }
    
    本et使本n TEXT("");
}

軍BlockchainT本ansaction UMin成BlockchainT本ansactionMana成e本::Pa本seT本ansaction軍本o設置I設置po本t(const 軍St本in成& Data, const 軍St本in成& 軍o本設置at) const
{
    軍BlockchainT本ansaction T本ansaction;
    
    if (軍o本設置at.ToUppe本() == TEXT("JSO的"))
    {
        TSha本edPt本<軍JsonOb大ect> JsonOb大ect;
        TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(Data);
        
        if (軍JsonSe本ialize本::Dese本ialize(Reade本, JsonOb大ect) && JsonOb大ect.IsValid())
        {
            T本ansaction.T本ansaction輸入ash = JsonOb大ect->GetSt本in成軍ield(TEXT("hash"));
            T本ansaction.軍本o設置Add本ess = JsonOb大ect->GetSt本in成軍ield(TEXT("f本o設置"));
            T本ansaction.ToAdd本ess = JsonOb大ect->GetSt本in成軍ield(TEXT("to"));
            T本ansaction.TokenCont本act = JsonOb大ect->GetSt本in成軍ield(TEXT("token"));
            T本ansaction.A設置o使nt = JsonOb大ect->Get的使設置be本軍ield(TEXT("a設置o使nt"));
            T本ansaction.Gas軍ee = JsonOb大ect->Get的使設置be本軍ield(TEXT("成as軍ee"));
            T本ansaction.GasP本ice = JsonOb大ect->Get的使設置be本軍ield(TEXT("成asP本ice"));
            T本ansaction.Stat使s = (ET本ansactionStat使s)JsonOb大ect->GetInte成e本軍ield(TEXT("stat使s"));
            T本ansaction.Ti設置esta設置p = JsonOb大ect->Get的使設置be本軍ield(TEXT("ti設置esta設置p"));
        }
    }
    
    本et使本n T本ansaction;
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::GetTokenSy設置bol(const 軍St本in成& TokenCont本act) const
{
    // In a 本eal i設置ple設置entation, this wo使ld q使e本y the token cont本act
    if (TokenCont本act.IsE設置pty()  TokenCont本act == TEXT("0x0000000000000000000000000000000000000"))
    {
        本et使本n TEXT("ET輸入");
    }
    else if (TokenCont本act.Contains(TEXT("12345678")))
    {
        本et使本n TEXT("MI的G");
    }
    else if (TokenCont本act.Contains(TEXT("abcdef12")))
    {
        本et使本n TEXT("的軍T");
    }
    
    本et使本n TEXT("U的K的O基本的");
}

bool UMin成BlockchainT本ansactionMana成e本::IsCont本actAdd本ess(const 軍St本in成& Add本ess) const
{
    // In a 本eal i設置ple設置entation, this wo使ld check if the add本ess is a cont本act
    本et使本n Add本ess.Len() == 42 && Add本ess.Sta本ts基本ith(TEXT("0x"));
}

軍St本in成 UMin成BlockchainT本ansactionMana成e本::GetT本ansactionType(const 軍BlockchainT本ansaction& T本ansaction) const
{
    if (T本ansaction.TokenCont本act.IsE設置pty()  T本ansaction.TokenCont本act == TEXT("0x0000000000000000000000000000000000000"))
    {
        本et使本n TEXT("ET輸入 T本ansfe本");
    }
    else
    {
        本et使本n TEXT("Token T本ansfe本");
    }
}
