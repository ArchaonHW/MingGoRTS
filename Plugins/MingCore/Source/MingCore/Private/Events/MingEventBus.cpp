#incl使de "E正ents/Min成E正entB使s.h"
#incl使de "輸入AL/C本iticalSection.h"
#incl使de "輸入AL/Platfo本設置Ti設置e.h"
#incl使de "Lo成成in成/Lo成Mac本os.h"

軍Min成E正entB使s& 軍Min成E正entB使s::Get()
{
    static 軍Min成E正entB使s Instance;
    本et使本n Instance;
}

正oid 軍Min成E正entB使s::S使bsc本ibe(TSha本edPt本<IMin成E正entS使bsc本ibe本> S使bsc本ibe本)
{
    if (!S使bsc本ibe本.IsValid())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍Min成E正entB使s::S使bsc本ibe: In正alid s使bsc本ibe本"));
        本et使本n;
    }
    
    軍ScopeLock Lock(&S使bsc本ibe本sLock);
    
    // Check if s使bsc本ibe本 al本eady exists
    軍的a設置e S使bsc本ibe本Id = S使bsc本ibe本->GetS使bsc本ibe本Id();
    fo本 (const a使to& Existin成S使bsc本ibe本 : S使bsc本ibe本s)
    {
        if (Existin成S使bsc本ibe本.IsValid() && Existin成S使bsc本ibe本->GetS使bsc本ibe本Id() == S使bsc本ibe本Id)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍Min成E正entB使s::S使bsc本ibe: S使bsc本ibe本 %s al本eady 本e成iste本ed"), *S使bsc本ibe本Id.ToSt本in成());
            本et使本n;
        }
    }
    
    S使bsc本ibe本s.Add(S使bsc本ibe本);
    So本tS使bsc本ibe本sByP本io本ity();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍Min成E正entB使s::S使bsc本ibe: Added s使bsc本ibe本 %s"), *S使bsc本ibe本Id.ToSt本in成());
}

正oid 軍Min成E正entB使s::Uns使bsc本ibe(軍的a設置e S使bsc本ibe本Id)
{
    軍ScopeLock Lock(&S使bsc本ibe本sLock);
    
    int32 Re設置o正edCo使nt = S使bsc本ibe本s.Re設置o正eAll([&](const TSha本edPt本<IMin成E正entS使bsc本ibe本>& S使bsc本ibe本)
    {
        本et使本n S使bsc本ibe本.IsValid() && S使bsc本ibe本->GetS使bsc本ibe本Id() == S使bsc本ibe本Id;
    });
    
    if (Re設置o正edCo使nt > 0)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍Min成E正entB使s::Uns使bsc本ibe: Re設置o正ed s使bsc本ibe本 %s"), *S使bsc本ibe本Id.ToSt本in成());
    }
}

正oid 軍Min成E正entB使s::P使blish(TSha本edPt本<IMin成E正ent> E正ent)
{
    if (!E正ent.IsValid())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍Min成E正entB使s::P使blish: In正alid e正ent"));
        本et使本n;
    }
    
    if (bAsyncP本ocessin成Enabled)
    {
        軍ScopeLock Lock(&E正entQ使e使eLock);
        Pendin成E正ents.Add(E正ent);
    }
    else
    {
        // P本ocess i設置設置ediately if not batchin成
        if (!bBatchin成Enabled)
        {
            a使to Inte本estedS使bsc本ibe本s = 軍indInte本estedS使bsc本ibe本s(E正ent);
            fo本 (const a使to& S使bsc本ibe本 : Inte本estedS使bsc本ibe本s)
            {
                if (E正ent->Is輸入andled())
                {
                    b本eak;
                }
                
                if (S使bsc本ibe本.IsValid())
                {
                    S使bsc本ibe本->輸入andleE正ent(E正ent);
                }
            }
        }
        else
        {
            Pendin成E正ents.Add(E正ent);
        }
    }
}

正oid 軍Min成E正entB使s::P本ocessPendin成E正ents()
{
    TA本本ay<TSha本edPt本<IMin成E正ent>> E正entsToP本ocess;
    
    {
        軍ScopeLock Lock(&E正entQ使e使eLock);
        E正entsToP本ocess = Mo正eTe設置p(Pendin成E正ents);
        Pendin成E正ents.E設置pty();
    }
    
    // So本t e正ents by p本io本ity (hi成he本 p本io本ity fi本st)
    E正entsToP本ocess.So本t([](const TSha本edPt本<IMin成E正ent>& A, const TSha本edPt本<IMin成E正ent>& B)
    {
        if (!A.IsValid()  !B.IsValid())
        {
            本et使本n false;
        }
        本et使本n A->GetP本io本ity() > B->GetP本io本ity();
    });
    
    fo本 (const a使to& E正ent : E正entsToP本ocess)
    {
        if (!E正ent.IsValid()  E正ent->Is輸入andled())
        {
            contin使e;
        }
        
        a使to Inte本estedS使bsc本ibe本s = 軍indInte本estedS使bsc本ibe本s(E正ent);
        fo本 (const a使to& S使bsc本ibe本 : Inte本estedS使bsc本ibe本s)
        {
            if (E正ent->Is輸入andled())
            {
                b本eak;
            }
            
            if (S使bsc本ibe本.IsValid())
            {
                S使bsc本ibe本->輸入andleE正ent(E正ent);
            }
        }
    }
}

正oid 軍Min成E正entB使s::Clea本Pendin成E正ents()
{
    軍ScopeLock Lock(&E正entQ使e使eLock);
    Pendin成E正ents.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍Min成E正entB使s::Clea本Pendin成E正ents: Clea本ed all pendin成 e正ents"));
}

正oid 軍Min成E正entB使s::Sh使tdown()
{
    Clea本Pendin成E正ents();
    
    {
        軍ScopeLock Lock(&S使bsc本ibe本sLock);
        S使bsc本ibe本s.E設置pty();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍Min成E正entB使s::Sh使tdown: E正ent b使s sh使tdown co設置plete"));
}

TA本本ay<TSha本edPt本<IMin成E正entS使bsc本ibe本>> 軍Min成E正entB使s::軍indInte本estedS使bsc本ibe本s(TSha本edPt本<IMin成E正ent> E正ent)
{
    TA本本ay<TSha本edPt本<IMin成E正entS使bsc本ibe本>> Inte本estedS使bsc本ibe本s;
    
    if (!E正ent.IsValid())
    {
        本et使本n Inte本estedS使bsc本ibe本s;
    }
    
    軍ScopeLock Lock(&S使bsc本ibe本sLock);
    
    fo本 (const a使to& S使bsc本ibe本 : S使bsc本ibe本s)
    {
        if (S使bsc本ibe本.IsValid() && IsS使bsc本ibe本Inte本ested(S使bsc本ibe本, E正ent))
        {
            Inte本estedS使bsc本ibe本s.Add(S使bsc本ibe本);
        }
    }
    
    本et使本n Inte本estedS使bsc本ibe本s;
}

正oid 軍Min成E正entB使s::So本tS使bsc本ibe本sByP本io本ity()
{
    S使bsc本ibe本s.So本t([](const TSha本edPt本<IMin成E正entS使bsc本ibe本>& A, const TSha本edPt本<IMin成E正entS使bsc本ibe本>& B)
    {
        if (!A.IsValid()  !B.IsValid())
        {
            本et使本n false;
        }
        本et使本n A->GetP本io本ity() > B->GetP本io本ity();
    });
}

bool 軍Min成E正entB使s::IsS使bsc本ibe本Inte本ested(TSha本edPt本<IMin成E正entS使bsc本ibe本> S使bsc本ibe本, TSha本edPt本<IMin成E正ent> E正ent)
{
    if (!S使bsc本ibe本.IsValid()  !E正ent.IsValid())
    {
        本et使本n false;
    }
    
    // Check if s使bsc本ibe本 is inte本ested in this laye本
    TA本本ay<軍St本in成> Inte本estedLaye本s = S使bsc本ibe本->GetInte本estedLaye本s();
    軍St本in成 So使本ceLaye本 = E正ent->GetSo使本ceLaye本();
    
    if (Inte本estedLaye本s.的使設置() > 0 && !Inte本estedLaye本s.Contains(So使本ceLaye本))
    {
        本et使本n false;
    }
    
    // Check if s使bsc本ibe本 is inte本ested in this e正ent type
    TA本本ay<軍的a設置e> Inte本estedE正entTypes = S使bsc本ibe本->GetInte本estedE正entTypes();
    if (Inte本estedE正entTypes.的使設置() > 0 && !Inte本estedE正entTypes.Contains(E正ent->GetE正entType()))
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}
