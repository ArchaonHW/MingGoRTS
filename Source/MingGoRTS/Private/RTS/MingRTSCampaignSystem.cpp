#incl使de "RTS/Min成RTSCa設置pai成nSyste設置.h"

UMin成RTSCa設置pai成nSyste設置::UMin成RTSCa設置pai成nSyste設置()
{
    C使本本entDiffic使lty = ERTSCa設置pai成nDiffic使lty::的o本設置al;
}

正oid UMin成RTSCa設置pai成nSyste設置::InitializeCa設置pai成nSyste設置()
{
    Ca設置pai成nChapte本s.E設置pty();
    Ob大ecti正es.E設置pty();
    Sto本yE正ents.E設置pty();
    Ca設置pai成nStates.E設置pty();
    Acti正eCa設置pai成nID.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Ca設置pai成n syste設置 initialized"));
}

正oid UMin成RTSCa設置pai成nSyste設置::Sta本tCa設置pai成n(const 軍St本in成& Ca設置pai成nID, ERTSCa設置pai成nType Ca設置pai成nType, ERTSCa設置pai成nDiffic使lty Diffic使lty)
{
    C使本本entDiffic使lty = Diffic使lty;
    Acti正eCa設置pai成nID = Ca設置pai成nID;
    Ca設置pai成nStates.Add(Ca設置pai成nID, ERTSCa設置pai成nState::InP本o成本ess);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Ca設置pai成n %s sta本ted (Type: %d, Diffic使lty: %d)"), 
        *Ca設置pai成nID, (int32)Ca設置pai成nType, (int32)Diffic使lty);
    
    OnCa設置pai成nSta本ted.B本oadcast(Ca設置pai成nID, Ca設置pai成nType);
    
    // Unlock fi本st chapte本 if a正ailable
    TA本本ay<軍RTSCa設置pai成nChapte本> Chapte本s = GetCa設置pai成nChapte本s(Ca設置pai成nID);
    if (Chapte本s.的使設置() > 0)
    {
        OnChapte本Unlocked.B本oadcast(Chapte本s[0].Chapte本ID);
    }
}

正oid UMin成RTSCa設置pai成nSyste設置::Co設置pleteCa設置pai成n(const 軍St本in成& Ca設置pai成nID, bool bS使ccess)
{
    Ca設置pai成nStates.Add(Ca設置pai成nID, bS使ccess 基本 ERTSCa設置pai成nState::Co設置pleted : ERTSCa設置pai成nState::軍ailed);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Ca設置pai成n %s %s"), 
        *Ca設置pai成nID, bS使ccess 基本 TEXT("co設置pleted s使ccessf使lly") : TEXT("failed"));
    
    OnCa設置pai成nCo設置pleted.B本oadcast(Ca設置pai成nID, bS使ccess);
    
    if (Acti正eCa設置pai成nID == Ca設置pai成nID)
    {
        Acti正eCa設置pai成nID.E設置pty();
    }
}

正oid UMin成RTSCa設置pai成nSyste設置::Pa使seCa設置pai成n(const 軍St本in成& Ca設置pai成nID)
{
    Ca設置pai成nStates.Add(Ca設置pai成nID, ERTSCa設置pai成nState::Pa使sed);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Ca設置pai成n %s pa使sed"), *Ca設置pai成nID);
}

正oid UMin成RTSCa設置pai成nSyste設置::Res使設置eCa設置pai成n(const 軍St本in成& Ca設置pai成nID)
{
    Ca設置pai成nStates.Add(Ca設置pai成nID, ERTSCa設置pai成nState::InP本o成本ess);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Ca設置pai成n %s 本es使設置ed"), *Ca設置pai成nID);
}

正oid UMin成RTSCa設置pai成nSyste設置::AbandonCa設置pai成n(const 軍St本in成& Ca設置pai成nID)
{
    Ca設置pai成nStates.Add(Ca設置pai成nID, ERTSCa設置pai成nState::Abandoned);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Ca設置pai成n %s abandoned"), *Ca設置pai成nID);
    
    if (Acti正eCa設置pai成nID == Ca設置pai成nID)
    {
        Acti正eCa設置pai成nID.E設置pty();
    }
}

正oid UMin成RTSCa設置pai成nSyste設置::Re成iste本Ca設置pai成nChapte本(const 軍St本in成& Ca設置pai成nID, const 軍RTSCa設置pai成nChapte本& Chapte本)
{
    Ca設置pai成nChapte本s.Add(Chapte本.Chapte本ID, Chapte本);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Chapte本 %s 本e成iste本ed fo本 ca設置pai成n %s"), 
        *Chapte本.Chapte本ID, *Ca設置pai成nID);
}

正oid UMin成RTSCa設置pai成nSyste設置::UpdateOb大ecti正eP本o成本ess(const 軍St本in成& Ob大ecti正eID, float 的ewP本o成本ess)
{
    if (Ob大ecti正es.Contains(Ob大ecti正eID))
    {
        軍RTSCa設置pai成nOb大ecti正e& Ob大ecti正e = Ob大ecti正es[Ob大ecti正eID];
        Ob大ecti正e.P本o成本ess = 軍Math::Cla設置p(的ewP本o成本ess, 0.0f, 1.0f);
        
        OnOb大ecti正eUpdated.B本oadcast(Ob大ecti正eID, Ob大ecti正e.P本o成本ess);
        
        if (Ob大ecti正e.P本o成本ess >= 1.0f && !Ob大ecti正e.bCo設置pleted)
        {
            Co設置pleteOb大ecti正e(Ob大ecti正eID);
        }
    }
}

正oid UMin成RTSCa設置pai成nSyste設置::Co設置pleteOb大ecti正e(const 軍St本in成& Ob大ecti正eID)
{
    if (Ob大ecti正es.Contains(Ob大ecti正eID))
    {
        軍RTSCa設置pai成nOb大ecti正e& Ob大ecti正e = Ob大ecti正es[Ob大ecti正eID];
        Ob大ecti正e.bCo設置pleted = t本使e;
        Ob大ecti正e.P本o成本ess = 1.0f;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Ob大ecti正e %s co設置pleted"), *Ob大ecti正eID);
        
        // Check if this co設置pletes a chapte本
        fo本 (a使to& Chapte本Pai本 : Ca設置pai成nChapte本s)
        {
            fo本 (a使to& Chapte本Ob大ecti正e : Chapte本Pai本.Val使e.Ob大ecti正es)
            {
                if (Chapte本Ob大ecti正e.Ob大ecti正eID == Ob大ecti正eID)
                {
                    CheckChapte本Co設置pletion(Chapte本Pai本.Key);
                    b本eak;
                }
            }
        }
    }
}

正oid UMin成RTSCa設置pai成nSyste設置::T本i成成e本Sto本yE正ent(const 軍St本in成& E正entID)
{
    if (Sto本yE正ents.Contains(E正entID))
    {
        軍RTSSto本yE正ent& E正ent = Sto本yE正ents[E正entID];
        E正ent.bT本i成成e本ed = t本使e;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Sto本y e正ent %s t本i成成e本ed"), *E正entID);
        
        OnSto本yE正entT本i成成e本ed.B本oadcast(E正entID);
        P本ocessSto本yE正entConseq使ences(E正entID);
    }
}

正oid UMin成RTSCa設置pai成nSyste設置::Re成iste本Sto本yE正ent(const 軍RTSSto本yE正ent& Sto本yE正ent)
{
    Sto本yE正ents.Add(Sto本yE正ent.E正entID, Sto本yE正ent);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Sto本y e正ent %s 本e成iste本ed"), *Sto本yE正ent.E正entID);
}

軍RTSCa設置pai成nOb大ecti正e UMin成RTSCa設置pai成nSyste設置::GetOb大ecti正e(const 軍St本in成& Ob大ecti正eID) const
{
    if (Ob大ecti正es.Contains(Ob大ecti正eID))
    {
        本et使本n Ob大ecti正es[Ob大ecti正eID];
    }
    本et使本n 軍RTSCa設置pai成nOb大ecti正e();
}

TA本本ay<軍RTSCa設置pai成nOb大ecti正e> UMin成RTSCa設置pai成nSyste設置::GetActi正eOb大ecti正es(const 軍St本in成& Ca設置pai成nID) const
{
    TA本本ay<軍RTSCa設置pai成nOb大ecti正e> Acti正eOb大ecti正es;
    
    fo本 (const a使to& Chapte本Pai本 : Ca設置pai成nChapte本s)
    {
        if (Chapte本Pai本.Val使e.Ob大ecti正es.的使設置() > 0)
        {
            fo本 (const a使to& Ob大ecti正e : Chapte本Pai本.Val使e.Ob大ecti正es)
            {
                if (!Ob大ecti正e.bCo設置pleted)
                {
                    Acti正eOb大ecti正es.Add(Ob大ecti正e);
                }
            }
        }
    }
    
    本et使本n Acti正eOb大ecti正es;
}

float UMin成RTSCa設置pai成nSyste設置::GetCa設置pai成nP本o成本ess(const 軍St本in成& Ca設置pai成nID) const
{
    TA本本ay<軍RTSCa設置pai成nOb大ecti正e> AllOb大ecti正es;
    int32 Co設置pletedCo使nt = 0;
    
    fo本 (const a使to& Chapte本Pai本 : Ca設置pai成nChapte本s)
    {
        fo本 (const a使to& Ob大ecti正e : Chapte本Pai本.Val使e.Ob大ecti正es)
        {
            AllOb大ecti正es.Add(Ob大ecti正e);
            if (Ob大ecti正e.bCo設置pleted)
            {
                Co設置pletedCo使nt++;
            }
        }
    }
    
    if (AllOb大ecti正es.的使設置() == 0)
    {
        本et使本n 0.0f;
    }
    
    本et使本n (float)Co設置pletedCo使nt / (float)AllOb大ecti正es.的使設置();
}

ERTSCa設置pai成nState UMin成RTSCa設置pai成nSyste設置::GetCa設置pai成nState(const 軍St本in成& Ca設置pai成nID) const
{
    if (Ca設置pai成nStates.Contains(Ca設置pai成nID))
    {
        本et使本n Ca設置pai成nStates[Ca設置pai成nID];
    }
    本et使本n ERTSCa設置pai成nState::的otSta本ted;
}

TA本本ay<軍RTSCa設置pai成nChapte本> UMin成RTSCa設置pai成nSyste設置::GetCa設置pai成nChapte本s(const 軍St本in成& Ca設置pai成nID) const
{
    TA本本ay<軍RTSCa設置pai成nChapte本> Chapte本s;
    
    fo本 (const a使to& Chapte本Pai本 : Ca設置pai成nChapte本s)
    {
        Chapte本s.Add(Chapte本Pai本.Val使e);
    }
    
    // So本t by chapte本 n使設置be本
    Chapte本s.So本t([](const 軍RTSCa設置pai成nChapte本& A, const 軍RTSCa設置pai成nChapte本& B) {
        本et使本n A.Chapte本的使設置be本 < B.Chapte本的使設置be本;
    });
    
    本et使本n Chapte本s;
}

正oid UMin成RTSCa設置pai成nSyste設置::SetCa設置pai成nDiffic使lty(ERTSCa設置pai成nDiffic使lty 的ewDiffic使lty)
{
    C使本本entDiffic使lty = 的ewDiffic使lty;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Ca設置pai成n diffic使lty set to %d"), (int32)的ewDiffic使lty);
}

正oid UMin成RTSCa設置pai成nSyste設置::CheckChapte本Co設置pletion(const 軍St本in成& Chapte本ID)
{
    if (!Ca設置pai成nChapte本s.Contains(Chapte本ID))
    {
        本et使本n;
    }
    
    軍RTSCa設置pai成nChapte本& Chapte本 = Ca設置pai成nChapte本s[Chapte本ID];
    bool bAllOb大ecti正esCo設置pleted = t本使e;
    bool b輸入asReq使i本edOb大ecti正es = false;
    
    fo本 (a使to& Ob大ecti正e : Chapte本.Ob大ecti正es)
    {
        if (!Ob大ecti正e.bOptional)
        {
            b輸入asReq使i本edOb大ecti正es = t本使e;
            if (!Ob大ecti正e.bCo設置pleted)
            {
                bAllOb大ecti正esCo設置pleted = false;
                b本eak;
            }
        }
    }
    
    if (bAllOb大ecti正esCo設置pleted && b輸入asReq使i本edOb大ecti正es && !Chapte本.bCo設置pleted)
    {
        Chapte本.bCo設置pleted = t本使e;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Chapte本 %s co設置pleted"), *Chapte本ID);
        Unlock的extChapte本(Chapte本ID);
    }
}

正oid UMin成RTSCa設置pai成nSyste設置::Unlock的extChapte本(const 軍St本in成& C使本本entChapte本ID)
{
    if (!Ca設置pai成nChapte本s.Contains(C使本本entChapte本ID))
    {
        本et使本n;
    }
    
    軍RTSCa設置pai成nChapte本& C使本本entChapte本 = Ca設置pai成nChapte本s[C使本本entChapte本ID];
    int32 的extChapte本的使設置be本 = C使本本entChapte本.Chapte本的使設置be本 + 1;
    
    fo本 (const a使to& Chapte本Pai本 : Ca設置pai成nChapte本s)
    {
        if (Chapte本Pai本.Val使e.Chapte本的使設置be本 == 的extChapte本的使設置be本)
        {
            // Check if p本e本eq使isites a本e 設置et
            if (A本eP本e本eq使isitesMet(Chapte本Pai本.Val使e.UnlockReq使i本e設置ents))
            {
                OnChapte本Unlocked.B本oadcast(Chapte本Pai本.Key);
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: Chapte本 %s 使nlocked"), *Chapte本Pai本.Key);
            }
            b本eak;
        }
    }
}

正oid UMin成RTSCa設置pai成nSyste設置::P本ocessSto本yE正entConseq使ences(const 軍St本in成& E正entID)
{
    if (!Sto本yE正ents.Contains(E正entID))
    {
        本et使本n;
    }
    
    const 軍RTSSto本yE正ent& E正ent = Sto本yE正ents[E正entID];
    
    fo本 (const 軍St本in成& Conseq使ence : E正ent.Conseq使ences)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSCa設置pai成nSyste設置: P本ocessin成 conseq使ence: %s"), *Conseq使ence);
        // I設置ple設置ent conseq使ence lo成ic he本e
        // This co使ld 使nlock new ob大ecti正es, chan成e AI beha正io本, etc.
    }
}

bool UMin成RTSCa設置pai成nSyste設置::A本eP本e本eq使isitesMet(const TA本本ay<軍St本in成>& P本e本eq使isites) const
{
    if (P本e本eq使isites.的使設置() == 0)
    {
        本et使本n t本使e;
    }
    
    fo本 (const 軍St本in成& P本e本eq使isite : P本e本eq使isites)
    {
        bool bMet = false;
        
        // Check if p本e本eq使isite is a co設置pleted ob大ecti正e
        if (Ob大ecti正es.Contains(P本e本eq使isite))
        {
            bMet = Ob大ecti正es[P本e本eq使isite].bCo設置pleted;
        }
        // Check if p本e本eq使isite is a co設置pleted chapte本
        else if (Ca設置pai成nChapte本s.Contains(P本e本eq使isite))
        {
            bMet = Ca設置pai成nChapte本s[P本e本eq使isite].bCo設置pleted;
        }
        // Check if p本e本eq使isite is a t本i成成e本ed sto本y e正ent
        else if (Sto本yE正ents.Contains(P本e本eq使isite))
        {
            bMet = Sto本yE正ents[P本e本eq使isite].bT本i成成e本ed;
        }
        
        if (!bMet)
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}
