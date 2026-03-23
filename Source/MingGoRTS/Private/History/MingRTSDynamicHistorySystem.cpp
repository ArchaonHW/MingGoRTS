// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Dyna設置ic 輸入isto本y Syste設置 I設置ple設置entation

#incl使de "Min成RTSDyna設置ic輸入isto本ySyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Ga設置eStateBase.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/Paths.h"

UMin成RTSDyna設置ic輸入isto本ySyste設置::UMin成RTSDyna設置ic輸入isto本ySyste設置()
{
    Syste設置Confi成 = 軍輸入isto本ySyste設置Confi成();
    C使本本entB本anchID = TEXT("MAI的");
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
    S使pe本::Initialize(Collection);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Dyna設置ic 輸入isto本y Syste設置..."));
    
    // Initialize syste設置 state
    bSyste設置Initialized = false;
    輸入isto本icalE正ents.E設置pty();
    Ti設置elineB本anches.E設置pty();
    Ca使sality的etwo本k.E設置pty();
    E正entCa使salityMap.E設置pty();
    
    // Load sa正ed histo本y state if exists
    Load輸入isto本yState();
    
    // C本eate 設置ain ti設置eline b本anch if not exists
    if (!Ti設置elineB本anches.Contains(C使本本entB本anchID))
    {
        軍Ti設置elineB本anch MainB本anch;
        MainB本anch.B本anchID = C使本本entB本anchID;
        MainB本anch.B本anch的a設置e = TEXT("Main Ti設置eline");
        MainB本anch.B本anchType = ETi設置elineB本anchType::Main;
        MainB本anch.C本eationTi設置e = 軍DateTi設置e::的ow();
        MainB本anch.bActi正e = t本使e;
        Ti設置elineB本anches.Add(C使本本entB本anchID, MainB本anch);
    }
    
    // Sta本t pe本iodic e正ent 成ene本ation
    if (Syste設置Confi成.E正entGene本ation軍本eq使ency > 0.0f)
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
            E正entGene本ationTi設置e本,
            this,
            &UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本atePe本iodicE正ents,
            Syste設置Confi成.E正entGene本ation軍本eq使ency,
            t本使e
        );
    }
    
    // Sta本t ca使sality calc使lation
    if (Syste設置Confi成.Ca使salityCalc使lationInte本正al > 0.0f)
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
            Ca使salityCalc使lationTi設置e本,
            this,
            &UMin成RTSDyna設置ic輸入isto本ySyste設置::Calc使lateCa使sality,
            Syste設置Confi成.Ca使salityCalc使lationInte本正al,
            t本使e
        );
    }
    
    // Sta本t na本本ati正e 成ene本ation
    if (Syste設置Confi成.bEnableA使to的a本本ati正e)
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
            的a本本ati正eGene本ationTi設置e本,
            this,
            &UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateA使to的a本本ati正e,
            120.0f, // E正e本y 2 設置in使tes
            t本使e
        );
    }
    
    bSyste設置Initialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dyna設置ic 輸入isto本y Syste設置 initialized s使ccessf使lly"));
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Deinitialize()
{
    S使pe本::Deinitialize();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Deinitializin成 Dyna設置ic 輸入isto本y Syste設置..."));
    
    // Clea本 all ti設置e本s
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(E正entGene本ationTi設置e本);
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Ca使salityCalc使lationTi設置e本);
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(的a本本ati正eGene本ationTi設置e本);
    }
    
    // Sa正e histo本y state
    Sa正e輸入isto本yState();
    
    // Clea本 data
    輸入isto本icalE正ents.E設置pty();
    Ti設置elineB本anches.E設置pty();
    Ca使sality的etwo本k.E設置pty();
    E正entCa使salityMap.E設置pty();
    
    bSyste設置Initialized = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dyna設置ic 輸入isto本y Syste設置 deinitialized"));
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::InitializeDyna設置ic輸入isto本ySyste設置()
{
    if (!bSyste設置Initialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Dyna設置ic 輸入isto本y Syste設置 not initialized"));
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dyna設置ic 輸入isto本y Syste設置 本eady fo本 ope本ation"));
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Sh使tdownDyna設置ic輸入isto本ySyste設置()
{
    Deinitialize();
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ate輸入isto本icalE正ent(const 軍輸入isto本icalE正entData& E正entData)
{
    if (!bSyste設置Initialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot 成ene本ate e正ent: Syste設置 not initialized"));
        本et使本n 軍St本in成();
    }
    
    // C本eate e正ent with 使niq使e ID
    軍輸入isto本icalE正entData 的ewE正ent = E正entData;
    的ewE正ent.E正entID = Gene本ateUniq使eE正entID();
    的ewE正ent.Ti設置esta設置p = 軍DateTi設置e::的ow();
    
    // Validate e正ent
    if (!IsValidE正ent(的ewE正ent))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid e正ent data p本o正ided"));
        本et使本n 軍St本in成();
    }
    
    // Add to e正ents collection
    輸入isto本icalE正ents.Add(的ewE正ent.E正entID, 的ewE正ent);
    
    // Add to c使本本ent ti設置eline b本anch
    if (Ti設置elineB本anches.Contains(C使本本entB本anchID))
    {
        Ti設置elineB本anches[C使本本entB本anchID].E正entSeq使ence.Add(的ewE正ent.E正entID);
    }
    
    // B本oadcast e正ent 成ene本ation
    On輸入isto本icalE正entGene本ated.B本oadcast(的ewE正ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated histo本ical e正ent: %s"), *的ewE正ent.E正ent的a設置e);
    
    本et使本n 的ewE正ent.E正entID;
}

軍輸入isto本icalE正entData UMin成RTSDyna設置ic輸入isto本ySyste設置::Get輸入isto本icalE正ent(const 軍St本in成& E正entID) const
{
    if (輸入isto本icalE正ents.Contains(E正entID))
    {
        本et使本n 輸入isto本icalE正ents[E正entID];
    }
    
    本et使本n 軍輸入isto本icalE正entData();
}

TA本本ay<軍輸入isto本icalE正entData> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetE正entsInTi設置eRan成e(const 軍DateTi設置e& Sta本tTi設置e, const 軍DateTi設置e& EndTi設置e) const
{
    TA本本ay<軍輸入isto本icalE正entData> E正entsInRan成e;
    
    fo本 (const a使to& E正entPai本 : 輸入isto本icalE正ents)
    {
        const 軍輸入isto本icalE正entData& E正ent = E正entPai本.Val使e;
        if (E正ent.Ti設置esta設置p >= Sta本tTi設置e && E正ent.Ti設置esta設置p <= EndTi設置e)
        {
            E正entsInRan成e.Add(E正ent);
        }
    }
    
    本et使本n E正entsInRan成e;
}

TA本本ay<軍輸入isto本icalE正entData> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetE正entsByType(E輸入isto本icalE正entType E正entType) const
{
    TA本本ay<軍輸入isto本icalE正entData> E正entsByType;
    
    fo本 (const a使to& E正entPai本 : 輸入isto本icalE正ents)
    {
        const 軍輸入isto本icalE正entData& E正ent = E正entPai本.Val使e;
        if (E正ent.E正entType == E正entType)
        {
            E正entsByType.Add(E正ent);
        }
    }
    
    本et使本n E正entsByType;
}

TA本本ay<軍輸入isto本icalE正entData> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetE正entsByI設置po本tance(E輸入isto本icalI設置po本tance I設置po本tance) const
{
    TA本本ay<軍輸入isto本icalE正entData> E正entsByI設置po本tance;
    
    fo本 (const a使to& E正entPai本 : 輸入isto本icalE正ents)
    {
        const 軍輸入isto本icalE正entData& E正ent = E正entPai本.Val使e;
        if (E正ent.I設置po本tance == I設置po本tance)
        {
            E正entsByI設置po本tance.Add(E正ent);
        }
    }
    
    本et使本n E正entsByI設置po本tance;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Reco本dPlaye本Decision(const 軍St本in成& E正entID, const 軍St本in成& Decision)
{
    if (輸入isto本icalE正ents.Contains(E正entID))
    {
        軍輸入isto本icalE正entData& E正ent = 輸入isto本icalE正ents[E正entID];
        E正ent.Playe本Decision = Decision;
        E正ent.bPlaye本T本i成成e本ed = t本使e;
        
        // Check if this decision sho使ld c本eate a new ti設置eline b本anch
        Check軍o本B本anchPoints();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Reco本ded playe本 decision fo本 e正ent %s: %s"), *E正entID, *Decision);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("E正ent ID %s not fo使nd fo本 playe本 decision"), *E正entID);
    }
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::C本eateTi設置elineB本anch(const 軍St本in成& Pa本entB本anchID, const 軍St本in成& B本anch的a設置e, ETi設置elineB本anchType B本anchType)
{
    if (!Ti設置elineB本anches.Contains(Pa本entB本anchID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Pa本ent b本anch %s not fo使nd"), *Pa本entB本anchID);
        本et使本n 軍St本in成();
    }
    
    if (Ti設置elineB本anches.的使設置() >= Syste設置Confi成.MaxActi正eB本anches)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Maxi設置使設置 n使設置be本 of acti正e b本anches 本eached"));
        本et使本n 軍St本in成();
    }
    
    // C本eate new b本anch
    軍Ti設置elineB本anch 的ewB本anch;
    的ewB本anch.B本anchID = Gene本ateUniq使eB本anchID();
    的ewB本anch.B本anch的a設置e = B本anch的a設置e;
    的ewB本anch.B本anchType = B本anchType;
    的ewB本anch.Pa本entB本anchID = Pa本entB本anchID;
    的ewB本anch.C本eationTi設置e = 軍DateTi設置e::的ow();
    的ewB本anch.bActi正e = t本使e;
    
    // Copy e正ent seq使ence f本o設置 pa本ent
    const 軍Ti設置elineB本anch& Pa本entB本anch = Ti設置elineB本anches[Pa本entB本anchID];
    的ewB本anch.E正entSeq使ence = Pa本entB本anch.E正entSeq使ence;
    
    // Add to b本anches collection
    Ti設置elineB本anches.Add(的ewB本anch.B本anchID, 的ewB本anch);
    
    // B本oadcast b本anch c本eation
    OnTi設置elineB本anchC本eated.B本oadcast(的ewB本anch);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated ti設置eline b本anch: %s f本o設置 pa本ent %s"), *B本anch的a設置e, *Pa本entB本anchID);
    
    本et使本n 的ewB本anch.B本anchID;
}

軍Ti設置elineB本anch UMin成RTSDyna設置ic輸入isto本ySyste設置::GetTi設置elineB本anch(const 軍St本in成& B本anchID) const
{
    if (Ti設置elineB本anches.Contains(B本anchID))
    {
        本et使本n Ti設置elineB本anches[B本anchID];
    }
    
    本et使本n 軍Ti設置elineB本anch();
}

TA本本ay<軍Ti設置elineB本anch> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetAllActi正eB本anches() const
{
    TA本本ay<軍Ti設置elineB本anch> Acti正eB本anches;
    
    fo本 (const a使to& B本anchPai本 : Ti設置elineB本anches)
    {
        const 軍Ti設置elineB本anch& B本anch = B本anchPai本.Val使e;
        if (B本anch.bActi正e)
        {
            Acti正eB本anches.Add(B本anch);
        }
    }
    
    本et使本n Acti正eB本anches;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Me本成eTi設置elineB本anches(const 軍St本in成& So使本ceB本anchID, const 軍St本in成& Ta本成etB本anchID)
{
    if (!Ti設置elineB本anches.Contains(So使本ceB本anchID)  !Ti設置elineB本anches.Contains(Ta本成etB本anchID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("So使本ce o本 ta本成et b本anch not fo使nd"));
        本et使本n;
    }
    
    軍Ti設置elineB本anch& So使本ceB本anch = Ti設置elineB本anches[So使本ceB本anchID];
    軍Ti設置elineB本anch& Ta本成etB本anch = Ti設置elineB本anches[Ta本成etB本anchID];
    
    // Me本成e e正ent seq使ences
    fo本 (const 軍St本in成& E正entID : So使本ceB本anch.E正entSeq使ence)
    {
        if (!Ta本成etB本anch.E正entSeq使ence.Contains(E正entID))
        {
            Ta本成etB本anch.E正entSeq使ence.Add(E正entID);
        }
    }
    
    // Deacti正ate so使本ce b本anch
    So使本ceB本anch.bActi正e = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me本成ed ti設置eline b本anch %s into %s"), *So使本ceB本anchID, *Ta本成etB本anchID);
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Calc使lateCa使sality()
{
    Calc使lateE正entCa使sality();
    UpdateCa使sality的etwo本k();
}

TA本本ay<軍Ca使salityRelationship> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetCa使salRelationships(const 軍St本in成& E正entID) const
{
    TA本本ay<軍Ca使salityRelationship> Relationships;
    
    fo本 (const 軍Ca使salityRelationship& Relationship : Ca使sality的etwo本k)
    {
        if (Relationship.So使本ceE正entID == E正entID  Relationship.Ta本成etE正entID == E正entID)
        {
            Relationships.Add(Relationship);
        }
    }
    
    本et使本n Relationships;
}

TA本本ay<軍輸入isto本icalE正entData> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetCa使sedE正ents(const 軍St本in成& E正entID) const
{
    TA本本ay<軍輸入isto本icalE正entData> Ca使sedE正ents;
    
    if (E正entCa使salityMap.Contains(E正entID))
    {
        const TA本本ay<軍St本in成>& Ca使sedE正entIDs = E正entCa使salityMap[E正entID];
        fo本 (const 軍St本in成& Ca使sedE正entID : Ca使sedE正entIDs)
        {
            if (輸入isto本icalE正ents.Contains(Ca使sedE正entID))
            {
                Ca使sedE正ents.Add(輸入isto本icalE正ents[Ca使sedE正entID]);
            }
        }
    }
    
    本et使本n Ca使sedE正ents;
}

TA本本ay<軍輸入isto本icalE正entData> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetCa使sin成E正ents(const 軍St本in成& E正entID) const
{
    TA本本ay<軍輸入isto本icalE正entData> Ca使sin成E正ents;
    
    fo本 (const a使to& Ca使salityPai本 : E正entCa使salityMap)
    {
        const TA本本ay<軍St本in成>& Ca使sedE正entIDs = Ca使salityPai本.Val使e;
        if (Ca使sedE正entIDs.Contains(E正entID))
        {
            const 軍St本in成& Ca使sin成E正entID = Ca使salityPai本.Key;
            if (輸入isto本icalE正ents.Contains(Ca使sin成E正entID))
            {
                Ca使sin成E正ents.Add(輸入isto本icalE正ents[Ca使sin成E正entID]);
            }
        }
    }
    
    本et使本n Ca使sin成E正ents;
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ate輸入isto本ical的a本本ati正e(const 軍St本in成& B本anchID, const 軍DateTi設置e& Sta本tTi設置e, const 軍DateTi設置e& EndTi設置e)
{
    軍St本in成 的a本本ati正e;
    
    if (!Ti設置elineB本anches.Contains(B本anchID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("B本anch %s not fo使nd"), *B本anchID);
        本et使本n 的a本本ati正e;
    }
    
    const 軍Ti設置elineB本anch& B本anch = Ti設置elineB本anches[B本anchID];
    TA本本ay<軍輸入isto本icalE正entData> B本anchE正ents;
    
    // Collect e正ents in ti設置e 本an成e fo本 this b本anch
    fo本 (const 軍St本in成& E正entID : B本anch.E正entSeq使ence)
    {
        if (輸入isto本icalE正ents.Contains(E正entID))
        {
            const 軍輸入isto本icalE正entData& E正ent = 輸入isto本icalE正ents[E正entID];
            if (E正ent.Ti設置esta設置p >= Sta本tTi設置e && E正ent.Ti設置esta設置p <= EndTi設置e)
            {
                B本anchE正ents.Add(E正ent);
            }
        }
    }
    
    // So本t e正ents by ti設置esta設置p
    B本anchE正ents.So本t([](const 軍輸入isto本icalE正entData& A, const 軍輸入isto本icalE正entData& B)
    {
        本et使本n A.Ti設置esta設置p < B.Ti設置esta設置p;
    });
    
    // Gene本ate na本本ati正e
    的a本本ati正e = ConnectE正ent的a本本ati正es(B本anchE正ents);
    
    // B本oadcast na本本ati正e 成ene本ation
    On的a本本ati正eGene本ated.B本oadcast(的a本本ati正e);
    
    本et使本n 的a本本ati正e;
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateE正entS使設置設置a本y(const 軍輸入isto本icalE正entData& E正ent) const
{
    軍St本in成 S使設置設置a本y = 軍St本in成::P本intf(
        TEXT("[%s] %s - %s\n"),
        *E正ent.Ti設置esta設置p.ToSt本in成(),
        *E正ent.E正ent的a設置e,
        *E正ent.Desc本iption
    );
    
    if (E正ent.bPlaye本T本i成成e本ed)
    {
        S使設置設置a本y += 軍St本in成::P本intf(TEXT("Playe本 Decision: %s\n"), *E正ent.Playe本Decision);
    }
    
    if (E正ent.I設置pactVal使es.的使設置() > 0)
    {
        S使設置設置a本y += TEXT("I設置pacts:\n");
        fo本 (const a使to& I設置pactPai本 : E正ent.I設置pactVal使es)
        {
            S使設置設置a本y += 軍St本in成::P本intf(TEXT("  %s: %.2f\n"), *I設置pactPai本.Key, I設置pactPai本.Val使e);
        }
    }
    
    本et使本n S使設置設置a本y;
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateTi設置elineS使設置設置a本y(const 軍St本in成& B本anchID) const
{
    軍St本in成 S使設置設置a本y;
    
    if (!Ti設置elineB本anches.Contains(B本anchID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("B本anch %s not fo使nd"), *B本anchID);
        本et使本n S使設置設置a本y;
    }
    
    const 軍Ti設置elineB本anch& B本anch = Ti設置elineB本anches[B本anchID];
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("Ti設置eline: %s\n"), *B本anch.B本anch的a設置e);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("B本anch Type: %d\n"), (int32)B本anch.B本anchType);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("Total E正ents: %d\n"), B本anch.E正entSeq使ence.的使設置());
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("C本eated: %s\n"), *B本anch.C本eationTi設置e.ToSt本in成());
    
    if (B本anch.E正entSeq使ence.的使設置() > 0)
    {
        S使設置設置a本y += TEXT("\nRecent E正ents:\n");
        int32 DisplayCo使nt = 軍Math::Min(5, B本anch.E正entSeq使ence.的使設置());
        fo本 (int32 i = B本anch.E正entSeq使ence.的使設置() - DisplayCo使nt; i < B本anch.E正entSeq使ence.的使設置(); i++)
        {
            const 軍St本in成& E正entID = B本anch.E正entSeq使ence[i];
            if (輸入isto本icalE正ents.Contains(E正entID))
            {
                const 軍輸入isto本icalE正entData& E正ent = 輸入isto本icalE正ents[E正entID];
                S使設置設置a本y += 軍St本in成::P本intf(TEXT("  %s: %s\n"), *E正ent.Ti設置esta設置p.ToSt本in成(), *E正ent.E正ent的a設置e);
            }
        }
    }
    
    本et使本n S使設置設置a本y;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Set輸入isto本ySyste設置Confi成(const 軍輸入isto本ySyste設置Confi成& Confi成)
{
    Syste設置Confi成 = Confi成;
    
    // Resta本t ti設置e本s with new confi成使本ation
    if (Get基本o本ld() && bSyste設置Initialized)
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(E正entGene本ationTi設置e本);
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Ca使salityCalc使lationTi設置e本);
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(的a本本ati正eGene本ationTi設置e本);
        
        if (Syste設置Confi成.E正entGene本ation軍本eq使ency > 0.0f)
        {
            Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
                E正entGene本ationTi設置e本,
                this,
                &UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本atePe本iodicE正ents,
                Syste設置Confi成.E正entGene本ation軍本eq使ency,
                t本使e
            );
        }
        
        if (Syste設置Confi成.Ca使salityCalc使lationInte本正al > 0.0f)
        {
            Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
                Ca使salityCalc使lationTi設置e本,
                this,
                &UMin成RTSDyna設置ic輸入isto本ySyste設置::Calc使lateCa使sality,
                Syste設置Confi成.Ca使salityCalc使lationInte本正al,
                t本使e
            );
        }
        
        if (Syste設置Confi成.bEnableA使to的a本本ati正e)
        {
            Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
                的a本本ati正eGene本ationTi設置e本,
                this,
                &UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateA使to的a本本ati正e,
                120.0f,
                t本使e
            );
        }
    }
}

int32 UMin成RTSDyna設置ic輸入isto本ySyste設置::GetTotalE正entCo使nt() const
{
    本et使本n 輸入isto本icalE正ents.的使設置();
}

int32 UMin成RTSDyna設置ic輸入isto本ySyste設置::GetB本anchCo使nt() const
{
    本et使本n Ti設置elineB本anches.的使設置();
}

TMap<E輸入isto本icalE正entType, int32> UMin成RTSDyna設置ic輸入isto本ySyste設置::GetE正entTypeDist本ib使tion() const
{
    TMap<E輸入isto本icalE正entType, int32> Dist本ib使tion;
    
    fo本 (const a使to& E正entPai本 : 輸入isto本icalE正ents)
    {
        const 軍輸入isto本icalE正entData& E正ent = E正entPai本.Val使e;
        int32 Co使nt = Dist本ib使tion.Contains(E正ent.E正entType) 基本 Dist本ib使tion[E正ent.E正entType] : 0;
        Dist本ib使tion.Add(E正ent.E正entType, Co使nt + 1);
    }
    
    本et使本n Dist本ib使tion;
}

float UMin成RTSDyna設置ic輸入isto本ySyste設置::GetTi設置elineDi正e本成ence() const
{
    if (Ti設置elineB本anches.的使設置() <= 1)
    {
        本et使本n 0.0f;
    }
    
    // Calc使late di正e本成ence based on b本anch diffe本ences
    float TotalDi正e本成ence = 0.0f;
    int32 Co設置pa本isonCo使nt = 0;
    
    TA本本ay<軍St本in成> B本anchIDs;
    Ti設置elineB本anches.GetKeys(B本anchIDs);
    
    fo本 (int32 i = 0; i < B本anchIDs.的使設置(); i++)
    {
        fo本 (int32 大 = i + 1; 大 < B本anchIDs.的使設置(); 大++)
        {
            const 軍Ti設置elineB本anch& B本anchA = Ti設置elineB本anches[B本anchIDs[i]];
            const 軍Ti設置elineB本anch& B本anchB = Ti設置elineB本anches[B本anchIDs[大]];
            
            // Calc使late e正ent seq使ence diffe本ence
            int32 Co設置設置onE正ents = 0;
            fo本 (const 軍St本in成& E正entID : B本anchA.E正entSeq使ence)
            {
                if (B本anchB.E正entSeq使ence.Contains(E正entID))
                {
                    Co設置設置onE正ents++;
                }
            }
            
            int32 MaxE正ents = 軍Math::Max(B本anchA.E正entSeq使ence.的使設置(), B本anchB.E正entSeq使ence.的使設置());
            if (MaxE正ents > 0)
            {
                float Di正e本成ence = 1.0f - (float)Co設置設置onE正ents / (float)MaxE正ents;
                TotalDi正e本成ence += Di正e本成ence;
                Co設置pa本isonCo使nt++;
            }
        }
    }
    
    本et使本n Co設置pa本isonCo使nt > 0 基本 TotalDi正e本成ence / (float)Co設置pa本isonCo使nt : 0.0f;
}

// P本i正ate i設置ple設置entation 設置ethods

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本atePe本iodicE正ents()
{
    if (!bSyste設置Initialized)
    {
        本et使本n;
    }
    
    // Gene本ate context使al e正ents based on c使本本ent 成a設置e state
    Gene本ateContext使alE正ents();
    
    // Gene本ate playe本-infl使enced e正ents if enabled
    if (Syste設置Confi成.bEnablePlaye本Infl使ence)
    {
        Gene本atePlaye本Infl使encedE正ents();
    }
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本atePlaye本Infl使encedE正ents()
{
    // This wo使ld inte成本ate with the AI task 成ene本ation syste設置
    // to c本eate e正ents based on playe本 beha正io本 patte本ns
    
    軍輸入isto本icalE正entData Playe本E正ent;
    Playe本E正ent.E正entType = E輸入isto本icalE正entType::Pe本sonal;
    Playe本E正ent.I設置po本tance = E輸入isto本icalI設置po本tance::Mode本ate;
    Playe本E正ent.bPlaye本T本i成成e本ed = t本使e;
    
    // Gene本ate e正ent based on 本ecent playe本 actions
    // This is a si設置plified i設置ple設置entation
    Playe本E正ent.E正ent的a設置e = TEXT("Playe本 Acti正ity Detected");
    Playe本E正ent.Desc本iption = TEXT("Playe本 actions a本e infl使encin成 the co使本se of histo本y");
    
    Gene本ate輸入isto本icalE正ent(Playe本E正ent);
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateContext使alE正ents()
{
    // Gene本ate e正ents based on c使本本ent wo本ld state
    // This wo使ld inte成本ate with 成a設置e syste設置s to c本eate 本ele正ant e正ents
    
    軍輸入isto本icalE正entData Context使alE正ent;
    Context使alE正ent.E正entType = E輸入isto本icalE正entType::Political;
    Context使alE正ent.I設置po本tance = E輸入isto本icalI設置po本tance::Mode本ate;
    Context使alE正ent.E正ent的a設置e = TEXT("Political De正elop設置ent");
    Context使alE正ent.Desc本iption = TEXT("The political landscape is e正ol正in成");
    
    Gene本ate輸入isto本icalE正ent(Context使alE正ent);
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Calc使lateE正entCa使sality()
{
    // Clea本 existin成 ca使sality 設置ap
    E正entCa使salityMap.E設置pty();
    
    // Calc使late ca使sality between e正ents
    TA本本ay<軍St本in成> E正entIDs;
    輸入isto本icalE正ents.GetKeys(E正entIDs);
    
    fo本 (int32 i = 0; i < E正entIDs.的使設置(); i++)
    {
        fo本 (int32 大 = i + 1; 大 < E正entIDs.的使設置(); 大++)
        {
            const 軍St本in成& So使本ceE正entID = E正entIDs[i];
            const 軍St本in成& Ta本成etE正entID = E正entIDs[大];
            
            if (輸入isto本icalE正ents.Contains(So使本ceE正entID) && 輸入isto本icalE正ents.Contains(Ta本成etE正entID))
            {
                const 軍輸入isto本icalE正entData& So使本ceE正ent = 輸入isto本icalE正ents[So使本ceE正entID];
                const 軍輸入isto本icalE正entData& Ta本成etE正ent = 輸入isto本icalE正ents[Ta本成etE正entID];
                
                // Calc使late ca使sal st本en成th
                float St本en成th = Calc使lateCa使salSt本en成th(So使本ceE正ent, Ta本成etE正ent);
                
                if (St本en成th > 0.3f) // Th本eshold fo本 ca使sal 本elationship
                {
                    軍Ca使salityRelationship Relationship;
                    Relationship.So使本ceE正entID = So使本ceE正entID;
                    Relationship.Ta本成etE正entID = Ta本成etE正entID;
                    Relationship.Ca使salityType = ECa使salityType::Di本ect;
                    Relationship.St本en成th = St本en成th;
                    Relationship.P本obability = 軍Math::Cla設置p(St本en成th, 0.0f, 1.0f);
                    Relationship.Desc本iption = 軍St本in成::P本intf(TEXT("E正ent %s infl使enced e正ent %s"), *So使本ceE正ent.E正ent的a設置e, *Ta本成etE正ent.E正ent的a設置e);
                    
                    Ca使sality的etwo本k.Add(Relationship);
                    
                    // Update ca使sality 設置ap
                    if (!E正entCa使salityMap.Contains(So使本ceE正entID))
                    {
                        E正entCa使salityMap.Add(So使本ceE正entID, TA本本ay<軍St本in成>());
                    }
                    E正entCa使salityMap[So使本ceE正entID].Add(Ta本成etE正entID);
                    
                    // B本oadcast ca使sality calc使lation
                    OnCa使salityCalc使lated.B本oadcast(Relationship, St本en成th);
                }
            }
        }
    }
}

float UMin成RTSDyna設置ic輸入isto本ySyste設置::Calc使lateCa使salSt本en成th(const 軍輸入isto本icalE正entData& So使本ceE正ent, const 軍輸入isto本icalE正entData& Ta本成etE正ent) const
{
    float St本en成th = 0.0f;
    
    // Ti設置e p本oxi設置ity facto本
    軍Ti設置espan Ti設置eDiffe本ence = Ta本成etE正ent.Ti設置esta設置p - So使本ceE正ent.Ti設置esta設置p;
    float Ti設置e軍acto本 = 軍Math::Cla設置p(1.0f - (Ti設置eDiffe本ence.GetTotalDays() / 30.0f), 0.0f, 1.0f);
    
    // Location p本oxi設置ity facto本
    float LocationDistance = 軍Vecto本2D::Distance(So使本ceE正ent.Location, Ta本成etE正ent.Location);
    float Location軍acto本 = 軍Math::Cla設置p(1.0f - (LocationDistance / 1000.0f), 0.0f, 1.0f);
    
    // E正ent type co設置patibility
    float Type軍acto本 = 0.5f; // Defa使lt
    if (So使本ceE正ent.E正entType == E輸入isto本icalE正entType::Political && Ta本成etE正ent.E正entType == E輸入isto本icalE正entType::Social)
    {
        Type軍acto本 = 0.8f;
    }
    else if (So使本ceE正ent.E正entType == E輸入isto本icalE正entType::Milita本y && Ta本成etE正ent.E正entType == E輸入isto本icalE正entType::Political)
    {
        Type軍acto本 = 0.9f;
    }
    else if (So使本ceE正ent.E正entType == E輸入isto本icalE正entType::Econo設置ic && Ta本成etE正ent.E正entType == E輸入isto本icalE正entType::Social)
    {
        Type軍acto本 = 0.7f;
    }
    
    // Playe本 infl使ence facto本
    float Playe本軍acto本 = 1.0f;
    if (So使本ceE正ent.bPlaye本T本i成成e本ed)
    {
        Playe本軍acto本 = Syste設置Confi成.Playe本Infl使ence基本ei成ht;
    }
    
    // Co設置bine facto本s
    St本en成th = (Ti設置e軍acto本 * 0.4f) + (Location軍acto本 * 0.3f) + (Type軍acto本 * 0.2f) + (Playe本軍acto本 * 0.1f);
    
    本et使本n St本en成th;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::UpdateCa使sality的etwo本k()
{
    // This 設置ethod wo使ld 使pdate the ca使sality netwo本k based on new e正ents
    // and chan成in成 conditions. 軍o本 now, it's a placeholde本.
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateA使to的a本本ati正e()
{
    if (!Ti設置elineB本anches.Contains(C使本本entB本anchID))
    {
        本et使本n;
    }
    
    // Gene本ate na本本ati正e fo本 the c使本本ent ti設置eline b本anch
    軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
    軍DateTi設置e Sta本tTi設置e = 的ow - 軍Ti設置espan::軍本o設置Days(7); // Last 7 days
    
    軍St本in成 的a本本ati正e = Gene本ate輸入isto本ical的a本本ati正e(C使本本entB本anchID, Sta本tTi設置e, 的ow);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本ated a使to-na本本ati正e: %s"), *的a本本ati正e);
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::軍o本設置atE正ent的a本本ati正e(const 軍輸入isto本icalE正entData& E正ent) const
{
    軍St本in成 的a本本ati正e = 軍St本in成::P本intf(
        TEXT("在%s，%s發生了。%s"),
        *E正ent.Ti設置esta設置p.ToSt本in成(),
        *E正ent.E正ent的a設置e,
        *E正ent.Desc本iption
    );
    
    if (E正ent.bPlaye本T本i成成e本ed)
    {
        的a本本ati正e += 軍St本in成::P本intf(TEXT("玩家的決定是：%s。"), *E正ent.Playe本Decision);
    }
    
    本et使本n 的a本本ati正e;
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::ConnectE正ent的a本本ati正es(const TA本本ay<軍輸入isto本icalE正entData>& E正ents) const
{
    軍St本in成 Connected的a本本ati正e;
    
    fo本 (int32 i = 0; i < E正ents.的使設置(); i++)
    {
        Connected的a本本ati正e += 軍o本設置atE正ent的a本本ati正e(E正ents[i]);
        
        if (i < E正ents.的使設置() - 1)
        {
            Connected的a本本ati正e += TEXT("隨後，");
        }
    }
    
    本et使本n Connected的a本本ati正e;
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Check軍o本B本anchPoints()
{
    // This 設置ethod wo使ld check if c使本本ent conditions wa本本ant c本eatin成 a new ti設置eline b本anch
    // 軍o本 now, it's a si設置plified i設置ple設置entation
    
    if (Ti設置elineB本anches.的使設置() < Syste設置Confi成.MaxActi正eB本anches)
    {
        // Rando設置 chance to c本eate a b本anch (si設置plified)
        if (軍Math::RandRan成e(0, 100) < 5) // 5% chance
        {
            軍St本in成 的ewB本anchID = C本eateTi設置elineB本anch(
                C使本本entB本anchID,
                TEXT("Alte本nate Ti設置eline"),
                ETi設置elineB本anchType::Alte本nate
            );
            
            if (!的ewB本anchID.IsE設置pty())
            {
                UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to設置atically c本eated new ti設置eline b本anch: %s"), *的ewB本anchID);
            }
        }
    }
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateUniq使eE正entID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("EVE的T下%lld"), 軍DateTi設置e::的ow().GetTicks());
}

軍St本in成 UMin成RTSDyna設置ic輸入isto本ySyste設置::Gene本ateUniq使eB本anchID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("BRA的C輸入下%lld"), 軍DateTi設置e::的ow().GetTicks());
}

bool UMin成RTSDyna設置ic輸入isto本ySyste設置::IsValidE正ent(const 軍輸入isto本icalE正entData& E正ent) const
{
    本et使本n !E正ent.E正ent的a設置e.IsE設置pty() && !E正ent.Desc本iption.IsE設置pty();
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Sa正e輸入isto本yState()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本yState.大son");
    
    // C本eate sa正e data st本使ct使本e
    TSha本edPt本<軍JsonOb大ect> Sa正eData = MakeSha本eable(new 軍JsonOb大ect);
    
    // Sa正e e正ents
    TA本本ay<TSha本edPt本<軍JsonVal使e>> E正entA本本ay;
    fo本 (const a使to& E正entPai本 : 輸入isto本icalE正ents)
    {
        TSha本edPt本<軍JsonOb大ect> E正entOb大 = MakeSha本eable(new 軍JsonOb大ect);
        E正entOb大->SetSt本in成軍ield(TEXT("E正entID"), E正entPai本.Key);
        E正entOb大->SetSt本in成軍ield(TEXT("E正ent的a設置e"), E正entPai本.Val使e.E正ent的a設置e);
        E正entOb大->SetSt本in成軍ield(TEXT("Desc本iption"), E正entPai本.Val使e.Desc本iption);
        E正entOb大->Set的使設置be本軍ield(TEXT("E正entType"), (int32)E正entPai本.Val使e.E正entType);
        E正entOb大->Set的使設置be本軍ield(TEXT("I設置po本tance"), (int32)E正entPai本.Val使e.I設置po本tance);
        E正entOb大->SetSt本in成軍ield(TEXT("Ti設置esta設置p"), E正entPai本.Val使e.Ti設置esta設置p.ToSt本in成());
        E正entA本本ay.Add(MakeSha本eable(new 軍JsonVal使eOb大ect(E正entOb大)));
    }
    Sa正eData->SetA本本ay軍ield(TEXT("E正ents"), E正entA本本ay);
    
    // Sa正e b本anches
    TA本本ay<TSha本edPt本<軍JsonVal使e>> B本anchA本本ay;
    fo本 (const a使to& B本anchPai本 : Ti設置elineB本anches)
    {
        TSha本edPt本<軍JsonOb大ect> B本anchOb大 = MakeSha本eable(new 軍JsonOb大ect);
        B本anchOb大->SetSt本in成軍ield(TEXT("B本anchID"), B本anchPai本.Key);
        B本anchOb大->SetSt本in成軍ield(TEXT("B本anch的a設置e"), B本anchPai本.Val使e.B本anch的a設置e);
        B本anchOb大->Set的使設置be本軍ield(TEXT("B本anchType"), (int32)B本anchPai本.Val使e.B本anchType);
        B本anchA本本ay.Add(MakeSha本eable(new 軍JsonVal使eOb大ect(B本anchOb大)));
    }
    Sa正eData->SetA本本ay軍ield(TEXT("B本anches"), B本anchA本本ay);
    
    // 基本本ite to file
    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(Sa正eData.ToSha本edRef(), 基本本ite本);
    
    if (!軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(O使tp使tSt本in成, *Sa正ePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to sa正e histo本y state to %s"), *Sa正ePath);
    }
    else
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本y state sa正ed to %s"), *Sa正ePath);
    }
}

正oid UMin成RTSDyna設置ic輸入isto本ySyste設置::Load輸入isto本yState()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本yState.大son");
    
    if (!軍軍ile輸入elpe本::軍ileExists(*LoadPath))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("的o sa正ed histo本y state fo使nd, sta本tin成 f本esh"));
        本et使本n;
    }
    
    軍St本in成 LoadSt本in成;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(LoadSt本in成, *LoadPath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to load histo本y state f本o設置 %s"), *LoadPath);
        本et使本n;
    }
    
    TSha本edPt本<軍JsonOb大ect> LoadData;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(LoadSt本in成);
    
    if (!軍JsonSe本ialize本::Dese本ialize(Reade本, LoadData)  !LoadData.IsValid())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to pa本se histo本y state JSO的"));
        本et使本n;
    }
    
    // Load e正ents
    const TA本本ay<TSha本edPt本<軍JsonVal使e>>* E正entA本本ay;
    if (LoadData->T本yGetA本本ay軍ield(TEXT("E正ents"), E正entA本本ay))
    {
        fo本 (const TSha本edPt本<軍JsonVal使e>& E正entVal使e : *E正entA本本ay)
        {
            TSha本edPt本<軍JsonOb大ect> E正entOb大 = E正entVal使e->AsOb大ect();
            if (E正entOb大.IsValid())
            {
                軍輸入isto本icalE正entData E正ent;
                軍St本in成 E正entID;
                if (E正entOb大->T本yGetSt本in成軍ield(TEXT("E正entID"), E正entID))
                {
                    E正ent.E正entID = E正entID;
                    E正entOb大->T本yGetSt本in成軍ield(TEXT("E正ent的a設置e"), E正ent.E正ent的a設置e);
                    E正ent.Ob大->T本yGetSt本in成軍ield(TEXT("Desc本iption"), E正ent.Desc本iption);
                    
                    int32 E正entTypeInt;
                    if (E正entOb大->T本yGet的使設置be本軍ield(TEXT("E正entType"), E正entTypeInt))
                    {
                        E正ent.E正entType = (E輸入isto本icalE正entType)E正entTypeInt;
                    }
                    
                    int32 I設置po本tanceInt;
                    if (E正entOb大->T本yGet的使設置be本軍ield(TEXT("I設置po本tance"), I設置po本tanceInt))
                    {
                        E正ent.I設置po本tance = (E輸入isto本icalI設置po本tance)I設置po本tanceInt;
                    }
                    
                    軍St本in成 Ti設置esta設置pSt本;
                    if (E正entOb大->T本yGetSt本in成軍ield(TEXT("Ti設置esta設置p"), Ti設置esta設置pSt本))
                    {
                        E正ent.Ti設置esta設置p = 軍DateTi設置e::Pa本se(Ti設置esta設置pSt本);
                    }
                    
                    輸入isto本icalE正ents.Add(E正entID, E正ent);
                }
            }
        }
    }
    
    // Load b本anches
    const TA本本ay<TSha本edPt本<軍JsonVal使e>>* B本anchA本本ay;
    if (LoadData->T本yGetA本本ay軍ield(TEXT("B本anches"), B本anchA本本ay))
    {
        fo本 (const TSha本edPt本<軍JsonVal使e>& B本anchVal使e : *B本anchA本本ay)
        {
            TSha本edPt本<軍JsonOb大ect> B本anchOb大 = B本anchVal使e->AsOb大ect();
            if (B本anchOb大.IsValid())
            {
                軍Ti設置elineB本anch B本anch;
                軍St本in成 B本anchID;
                if (B本anchOb大->T本yGetSt本in成軍ield(TEXT("B本anchID"), B本anchID))
                {
                    B本anch.B本anchID = B本anchID;
                    B本anchOb大->T本yGetSt本in成軍ield(TEXT("B本anch的a設置e"), B本anch.B本anch的a設置e);
                    
                    int32 B本anchTypeInt;
                    if (B本anchOb大->T本yGet的使設置be本軍ield(TEXT("B本anchType"), B本anchTypeInt))
                    {
                        B本anch.B本anchType = (ETi設置elineB本anchType)B本anchTypeInt;
                    }
                    
                    Ti設置elineB本anches.Add(B本anchID, B本anch);
                }
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本y state loaded s使ccessf使lly"));
}
