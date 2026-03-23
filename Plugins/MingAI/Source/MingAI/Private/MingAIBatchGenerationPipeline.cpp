#incl使de "Min成AIBatchGene本ationPipeline.h"
#incl使de "Min成AIGene本atedContentSyste設置.h"

UMin成AIBatchGene本ationPipeline::UMin成AIBatchGene本ationPipeline()
    : MaxConc使本本entTasks(5)
    , bInitialized(false)
{
}

正oid UMin成AIBatchGene本ationPipeline::InitializePipeline()
{
    if (bInitialized)
    {
        本et使本n;
    }

    MaxConc使本本entTasks = 5;
    bInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Batch Gene本ation Pipeline initialized"));
}

正oid UMin成AIBatchGene本ationPipeline::Sh使tdownPipeline()
{
    // Cancel all acti正e batches
    fo本 (const 軍G使id& BatchID : Acti正eBatches)
    {
        CancelBatch(BatchID);
    }

    BatchConfi成s.E設置pty();
    BatchIte設置s.E設置pty();
    Ite設置Data.E設置pty();
    BatchStat使ses.E設置pty();
    Acti正eBatches.E設置pty();

    bInitialized = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Batch Gene本ation Pipeline sh使tdown"));
}

軍G使id UMin成AIBatchGene本ationPipeline::C本eateBatch(const 軍St本in成& Batch的a設置e, const 軍Min成BatchConfi成使本ation& Confi成)
{
    軍G使id BatchID = 軍G使id::的ewG使id();
    
    軍Min成BatchConfi成使本ation 的ewConfi成 = Confi成;
    的ewConfi成.Batch的a設置e = Batch的a設置e;
    
    BatchConfi成s.Add(BatchID, 的ewConfi成);
    BatchIte設置s.Add(BatchID, TA本本ay<軍G使id>());
    BatchStat使ses.Add(BatchID, EMin成BatchStat使s::Pendin成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated batch %s with ID %s"), *Batch的a設置e, *BatchID.ToSt本in成());
    
    if (Confi成.bA使toSta本t)
    {
        Sta本tBatch(BatchID);
    }
    
    本et使本n BatchID;
}

bool UMin成AIBatchGene本ationPipeline::AddIte設置ToBatch(const 軍G使id& BatchID, const 軍Min成AIContentReq使est& Req使est)
{
    if (!BatchConfi成s.Contains(BatchID))
    {
        本et使本n false;
    }
    
    軍G使id Ite設置ID = 軍G使id::的ewG使id();
    
    軍Min成BatchIte設置 的ewIte設置;
    的ewIte設置.Ite設置ID = Ite設置ID;
    的ewIte設置.Req使est = Req使est;
    的ewIte設置.Stat使s = EMin成BatchStat使s::Pendin成;
    的ewIte設置.P本o成本ess = 0.0f;
    
    Ite設置Data.Add(Ite設置ID, 的ewIte設置);
    BatchIte設置s[BatchID].Add(Ite設置ID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added ite設置 %s to batch %s"), *Ite設置ID.ToSt本in成(), *BatchID.ToSt本in成());
    本et使本n t本使e;
}

bool UMin成AIBatchGene本ationPipeline::AddIte設置sToBatch(const 軍G使id& BatchID, const TA本本ay<軍Min成AIContentReq使est>& Req使ests)
{
    bool bAllAdded = t本使e;
    fo本 (const 軍Min成AIContentReq使est& Req使est : Req使ests)
    {
        if (!AddIte設置ToBatch(BatchID, Req使est))
        {
            bAllAdded = false;
        }
    }
    本et使本n bAllAdded;
}

bool UMin成AIBatchGene本ationPipeline::Re設置o正eIte設置軍本o設置Batch(const 軍G使id& BatchID, const 軍G使id& Ite設置ID)
{
    if (!BatchIte設置s.Contains(BatchID)  !Ite設置Data.Contains(Ite設置ID))
    {
        本et使本n false;
    }
    
    BatchIte設置s[BatchID].Re設置o正e(Ite設置ID);
    Ite設置Data.Re設置o正e(Ite設置ID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed ite設置 %s f本o設置 batch %s"), *Ite設置ID.ToSt本in成(), *BatchID.ToSt本in成());
    本et使本n t本使e;
}

bool UMin成AIBatchGene本ationPipeline::Sta本tBatch(const 軍G使id& BatchID)
{
    if (!BatchConfi成s.Contains(BatchID))
    {
        本et使本n false;
    }
    
    if (BatchStat使ses[BatchID] != EMin成BatchStat使s::Pendin成 && 
        BatchStat使ses[BatchID] != EMin成BatchStat使s::Pa使sed)
    {
        本et使本n false;
    }
    
    BatchStat使ses[BatchID] = EMin成BatchStat使s::P本ocessin成;
    Acti正eBatches.Add(BatchID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted batch %s"), *BatchID.ToSt本in成());
    
    P本ocessBatch(BatchID);
    本et使本n t本使e;
}

bool UMin成AIBatchGene本ationPipeline::Pa使seBatch(const 軍G使id& BatchID)
{
    if (!BatchConfi成s.Contains(BatchID)  BatchStat使ses[BatchID] != EMin成BatchStat使s::P本ocessin成)
    {
        本et使本n false;
    }
    
    BatchStat使ses[BatchID] = EMin成BatchStat使s::Pa使sed;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pa使sed batch %s"), *BatchID.ToSt本in成());
    本et使本n t本使e;
}

bool UMin成AIBatchGene本ationPipeline::Res使設置eBatch(const 軍G使id& BatchID)
{
    if (!BatchConfi成s.Contains(BatchID)  BatchStat使ses[BatchID] != EMin成BatchStat使s::Pa使sed)
    {
        本et使本n false;
    }
    
    BatchStat使ses[BatchID] = EMin成BatchStat使s::P本ocessin成;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Res使設置ed batch %s"), *BatchID.ToSt本in成());
    
    P本ocessBatch(BatchID);
    本et使本n t本使e;
}

bool UMin成AIBatchGene本ationPipeline::CancelBatch(const 軍G使id& BatchID)
{
    if (!BatchConfi成s.Contains(BatchID))
    {
        本et使本n false;
    }
    
    BatchStat使ses[BatchID] = EMin成BatchStat使s::Cancelled;
    Acti正eBatches.Re設置o正e(BatchID);
    
    // Ma本k all pendin成 ite設置s as cancelled
    if (BatchIte設置s.Contains(BatchID))
    {
        fo本 (const 軍G使id& Ite設置ID : BatchIte設置s[BatchID])
        {
            if (Ite設置Data.Contains(Ite設置ID) && Ite設置Data[Ite設置ID].Stat使s == EMin成BatchStat使s::Pendin成)
            {
                Ite設置Data[Ite設置ID].Stat使s = EMin成BatchStat使s::Cancelled;
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancelled batch %s"), *BatchID.ToSt本in成());
    本et使本n t本使e;
}

bool UMin成AIBatchGene本ationPipeline::Ret本y軍ailedIte設置s(const 軍G使id& BatchID)
{
    if (!BatchConfi成s.Contains(BatchID))
    {
        本et使本n false;
    }
    
    bool b輸入as軍ailedIte設置s = false;
    
    if (BatchIte設置s.Contains(BatchID))
    {
        fo本 (const 軍G使id& Ite設置ID : BatchIte設置s[BatchID])
        {
            if (Ite設置Data.Contains(Ite設置ID) && Ite設置Data[Ite設置ID].Stat使s == EMin成BatchStat使s::軍ailed)
            {
                Ite設置Data[Ite設置ID].Stat使s = EMin成BatchStat使s::Pendin成;
                Ite設置Data[Ite設置ID].P本o成本ess = 0.0f;
                Ite設置Data[Ite設置ID].E本本o本Messa成e.E設置pty();
                b輸入as軍ailedIte設置s = t本使e;
            }
        }
    }
    
    if (b輸入as軍ailedIte設置s)
    {
        Sta本tBatch(BatchID);
    }
    
    本et使本n b輸入as軍ailedIte設置s;
}

EMin成BatchStat使s UMin成AIBatchGene本ationPipeline::GetBatchStat使s(const 軍G使id& BatchID) const
{
    if (BatchStat使ses.Contains(BatchID))
    {
        本et使本n BatchStat使ses[BatchID];
    }
    本et使本n EMin成BatchStat使s::軍ailed;
}

軍Min成BatchStatistics UMin成AIBatchGene本ationPipeline::GetBatchStatistics(const 軍G使id& BatchID) const
{
    本et使本n Calc使lateStatistics(BatchID);
}

TA本本ay<軍G使id> UMin成AIBatchGene本ationPipeline::GetBatchIte設置IDs(const 軍G使id& BatchID) const
{
    if (BatchIte設置s.Contains(BatchID))
    {
        本et使本n BatchIte設置s[BatchID];
    }
    本et使本n TA本本ay<軍G使id>();
}

軍Min成BatchIte設置 UMin成AIBatchGene本ationPipeline::GetBatchIte設置(const 軍G使id& BatchID, const 軍G使id& Ite設置ID) const
{
    if (Ite設置Data.Contains(Ite設置ID))
    {
        本et使本n Ite設置Data[Ite設置ID];
    }
    本et使本n 軍Min成BatchIte設置();
}

TA本本ay<軍G使id> UMin成AIBatchGene本ationPipeline::GetActi正eBatchIDs() const
{
    TA本本ay<軍G使id> Res使lt;
    fo本 (const 軍G使id& BatchID : Acti正eBatches)
    {
        Res使lt.Add(BatchID);
    }
    本et使本n Res使lt;
}

軍G使id UMin成AIBatchGene本ationPipeline::C本eateCha本acte本Batch(const TA本本ay<軍St本in成>& Cha本acte本Desc本iptions, EMin成AIQ使alityLe正el Q使ality)
{
    軍Min成BatchConfi成使本ation Confi成;
    Confi成.Batch的a設置e = TEXT("Cha本acte本 Gene本ation Batch");
    Confi成.P本io本ity = EMin成BatchP本io本ity::輸入i成h;
    Confi成.MaxConc使本本entTasks = 3;
    Confi成.bA使toSta本t = t本使e;
    
    軍G使id BatchID = C本eateBatch(Confi成.Batch的a設置e, Confi成);
    
    fo本 (const 軍St本in成& Desc本iption : Cha本acte本Desc本iptions)
    {
        軍Min成AIContentReq使est Req使est;
        Req使est.ContentType = EMin成AIContentType::I設置a成e;
        Req使est.P本o設置pt = Desc本iption;
        Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
        Req使est.Q使alityLe正el = Q使ality;
        Req使est.基本idth = 1024;
        Req使est.輸入ei成ht = 1024;
        
        AddIte設置ToBatch(BatchID, Req使est);
    }
    
    本et使本n BatchID;
}

軍G使id UMin成AIBatchGene本ationPipeline::C本eateEn正i本on設置entBatch(const TA本本ay<軍St本in成>& En正i本on設置entDesc本iptions, EMin成AIQ使alityLe正el Q使ality)
{
    軍Min成BatchConfi成使本ation Confi成;
    Confi成.Batch的a設置e = TEXT("En正i本on設置ent Gene本ation Batch");
    Confi成.P本io本ity = EMin成BatchP本io本ity::的o本設置al;
    Confi成.MaxConc使本本entTasks = 2;
    Confi成.bA使toSta本t = t本使e;
    
    軍G使id BatchID = C本eateBatch(Confi成.Batch的a設置e, Confi成);
    
    fo本 (const 軍St本in成& Desc本iption : En正i本on設置entDesc本iptions)
    {
        軍Min成AIContentReq使est Req使est;
        Req使est.ContentType = EMin成AIContentType::I設置a成e;
        Req使est.P本o設置pt = Desc本iption;
        Req使est.P本o正ide本 = EMin成AIP本o正ide本::StableDiff使sion;
        Req使est.Q使alityLe正el = Q使ality;
        Req使est.基本idth = 1920;
        Req使est.輸入ei成ht = 1080;
        
        AddIte設置ToBatch(BatchID, Req使est);
    }
    
    本et使本n BatchID;
}

軍G使id UMin成AIBatchGene本ationPipeline::C本eateS軍XBatch(const TA本本ay<軍St本in成>& So使ndDesc本iptions, EMin成AIQ使alityLe正el Q使ality)
{
    軍Min成BatchConfi成使本ation Confi成;
    Confi成.Batch的a設置e = TEXT("So使nd Effects Gene本ation Batch");
    Confi成.P本io本ity = EMin成BatchP本io本ity::的o本設置al;
    Confi成.MaxConc使本本entTasks = 5;
    Confi成.bA使toSta本t = t本使e;
    
    軍G使id BatchID = C本eateBatch(Confi成.Batch的a設置e, Confi成);
    
    fo本 (const 軍St本in成& Desc本iption : So使ndDesc本iptions)
    {
        軍Min成AIContentReq使est Req使est;
        Req使est.ContentType = EMin成AIContentType::So使ndEffect;
        Req使est.P本o設置pt = Desc本iption;
        Req使est.P本o正ide本 = EMin成AIP本o正ide本::C使sto設置;
        Req使est.Q使alityLe正el = Q使ality;
        
        AddIte設置ToBatch(BatchID, Req使est);
    }
    
    本et使本n BatchID;
}

軍G使id UMin成AIBatchGene本ationPipeline::C本eateM使sicBatch(const TA本本ay<軍St本in成>& M使sicDesc本iptions, EMin成AIQ使alityLe正el Q使ality)
{
    軍Min成BatchConfi成使本ation Confi成;
    Confi成.Batch的a設置e = TEXT("M使sic Gene本ation Batch");
    Confi成.P本io本ity = EMin成BatchP本io本ity::輸入i成h;
    Confi成.MaxConc使本本entTasks = 2;
    Confi成.bA使toSta本t = t本使e;
    
    軍G使id BatchID = C本eateBatch(Confi成.Batch的a設置e, Confi成);
    
    fo本 (const 軍St本in成& Desc本iption : M使sicDesc本iptions)
    {
        軍Min成AIContentReq使est Req使est;
        Req使est.ContentType = EMin成AIContentType::M使sic;
        Req使est.P本o設置pt = Desc本iption;
        Req使est.P本o正ide本 = EMin成AIP本o正ide本::AIVA;
        Req使est.Q使alityLe正el = Q使ality;
        Req使est.AdditionalPa本a設置ete本s.Add(TEXT("D使本ation"), TEXT("120.0"));
        
        AddIte設置ToBatch(BatchID, Req使est);
    }
    
    本et使本n BatchID;
}

軍G使id UMin成AIBatchGene本ationPipeline::C本eateCo設置pleteP本o大ectBatch(const 軍St本in成& P本o大ect的a設置e, const TA本本ay<軍Min成AIContentReq使est>& AllReq使i本e設置ents)
{
    軍Min成BatchConfi成使本ation Confi成;
    Confi成.Batch的a設置e = P本o大ect的a設置e;
    Confi成.P本io本ity = EMin成BatchP本io本ity::C本itical;
    Confi成.MaxConc使本本entTasks = 5;
    Confi成.bA使toSta本t = t本使e;
    
    軍G使id BatchID = C本eateBatch(P本o大ect的a設置e, Confi成);
    AddIte設置sToBatch(BatchID, AllReq使i本e設置ents);
    
    本et使本n BatchID;
}

正oid UMin成AIBatchGene本ationPipeline::P本ocessBatch(軍G使id BatchID)
{
    if (!BatchConfi成s.Contains(BatchID)  BatchStat使ses[BatchID] != EMin成BatchStat使s::P本ocessin成)
    {
        本et使本n;
    }
    
    // P本ocess ite設置s 使ntil we hit the conc使本本ent li設置it
    while (CanP本ocessMo本eIte設置s(BatchID))
    {
        P本ocess的extIte設置(BatchID);
    }
    
    // Check if batch is co設置plete
    軍Min成BatchStatistics Stats = Calc使lateStatistics(BatchID);
    if (Stats.Co設置pletedIte設置s + Stats.軍ailedIte設置s >= Stats.TotalIte設置s)
    {
        Co設置pleteBatch(BatchID);
    }
}

正oid UMin成AIBatchGene本ationPipeline::P本ocess的extIte設置(軍G使id BatchID)
{
    if (!BatchIte設置s.Contains(BatchID))
    {
        本et使本n;
    }
    
    // 軍ind next pendin成 ite設置
    fo本 (const 軍G使id& Ite設置ID : BatchIte設置s[BatchID])
    {
        if (Ite設置Data.Contains(Ite設置ID) && Ite設置Data[Ite設置ID].Stat使s == EMin成BatchStat使s::Pendin成)
        {
            Ite設置Data[Ite設置ID].Stat使s = EMin成BatchStat使s::P本ocessin成;
            
            // Sta本t 成ene本ation
            UMin成AIGene本atedContentSyste設置* ContentSyste設置 = 的ewOb大ect<UMin成AIGene本atedContentSyste設置>();
            ContentSyste設置->InitializeSyste設置();
            軍G使id Req使estID = ContentSyste設置->Gene本ateContent(Ite設置Data[Ite設置ID].Req使est);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocessin成 ite設置 %s in batch %s"), *Ite設置ID.ToSt本in成(), *BatchID.ToSt本in成());
            b本eak;
        }
    }
}

正oid UMin成AIBatchGene本ationPipeline::UpdateBatchP本o成本ess(軍G使id BatchID)
{
    if (!BatchConfi成s.Contains(BatchID))
    {
        本et使本n;
    }
    
    軍Min成BatchStatistics Stats = Calc使lateStatistics(BatchID);
    
    OnBatchP本o成本ess.B本oadcast(BatchID, Stats.O正e本allP本o成本ess, Stats.Co設置pletedIte設置s);
}

正oid UMin成AIBatchGene本ationPipeline::OnIte設置Gene本ationCo設置pleted(軍G使id BatchID, 軍G使id Ite設置ID, bool bS使ccess, const 軍St本in成& Res使ltPath)
{
    if (!Ite設置Data.Contains(Ite設置ID))
    {
        本et使本n;
    }
    
    if (bS使ccess)
    {
        Ite設置Data[Ite設置ID].Stat使s = EMin成BatchStat使s::Co設置pleted;
        Ite設置Data[Ite設置ID].Res使ltPath = Res使ltPath;
        Ite設置Data[Ite設置ID].P本o成本ess = 1.0f;
        
        OnIte設置Co設置pleted.B本oadcast(BatchID, Ite設置ID);
    }
    else
    {
        Ite設置Data[Ite設置ID].Stat使s = EMin成BatchStat使s::軍ailed;
        Ite設置Data[Ite設置ID].E本本o本Messa成e = TEXT("Gene本ation failed");
        
        OnIte設置軍ailed.B本oadcast(BatchID, Ite設置ID);
    }
    
    UpdateBatchP本o成本ess(BatchID);
    
    // Contin使e p本ocessin成 if batch is still acti正e
    if (BatchStat使ses.Contains(BatchID) && BatchStat使ses[BatchID] == EMin成BatchStat使s::P本ocessin成)
    {
        P本ocessBatch(BatchID);
    }
}

正oid UMin成AIBatchGene本ationPipeline::Co設置pleteBatch(軍G使id BatchID)
{
    if (!BatchConfi成s.Contains(BatchID))
    {
        本et使本n;
    }
    
    BatchStat使ses[BatchID] = EMin成BatchStat使s::Co設置pleted;
    Acti正eBatches.Re設置o正e(BatchID);
    
    軍Min成BatchStatistics Stats = Calc使lateStatistics(BatchID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Batch %s co設置pleted: %d/%d ite設置s s使ccessf使l"), 
        *BatchID.ToSt本in成(), Stats.Co設置pletedIte設置s, Stats.TotalIte設置s);
    
    OnBatchCo設置pleted.B本oadcast(BatchID);
}

bool UMin成AIBatchGene本ationPipeline::CanP本ocessMo本eIte設置s(軍G使id BatchID) const
{
    if (!BatchConfi成s.Contains(BatchID))
    {
        本et使本n false;
    }
    
    int32 MaxConc使本本ent = BatchConfi成s[BatchID].MaxConc使本本entTasks;
    int32 Acti正eCo使nt = GetActi正eIte設置Co使nt(BatchID);
    
    本et使本n Acti正eCo使nt < MaxConc使本本ent;
}

int32 UMin成AIBatchGene本ationPipeline::GetActi正eIte設置Co使nt(軍G使id BatchID) const
{
    if (!BatchIte設置s.Contains(BatchID))
    {
        本et使本n 0;
    }
    
    int32 Co使nt = 0;
    fo本 (const 軍G使id& Ite設置ID : BatchIte設置s[BatchID])
    {
        if (Ite設置Data.Contains(Ite設置ID) && Ite設置Data[Ite設置ID].Stat使s == EMin成BatchStat使s::P本ocessin成)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

軍Min成BatchStatistics UMin成AIBatchGene本ationPipeline::Calc使lateStatistics(軍G使id BatchID) const
{
    軍Min成BatchStatistics Stats;
    
    if (!BatchIte設置s.Contains(BatchID))
    {
        本et使本n Stats;
    }
    
    Stats.TotalIte設置s = BatchIte設置s[BatchID].的使設置();
    
    fo本 (const 軍G使id& Ite設置ID : BatchIte設置s[BatchID])
    {
        if (Ite設置Data.Contains(Ite設置ID))
        {
            const 軍Min成BatchIte設置& Ite設置 = Ite設置Data[Ite設置ID];
            
            switch (Ite設置.Stat使s)
            {
            case EMin成BatchStat使s::Co設置pleted:
                Stats.Co設置pletedIte設置s++;
                b本eak;
            case EMin成BatchStat使s::軍ailed:
                Stats.軍ailedIte設置s++;
                b本eak;
            case EMin成BatchStat使s::Pendin成:
                Stats.Pendin成Ite設置s++;
                b本eak;
            defa使lt:
                b本eak;
            }
        }
    }
    
    if (Stats.TotalIte設置s > 0)
    {
        Stats.O正e本allP本o成本ess = static下cast<float>(Stats.Co設置pletedIte設置s + Stats.軍ailedIte設置s) / Stats.TotalIte設置s;
    }
    
    本et使本n Stats;
}
