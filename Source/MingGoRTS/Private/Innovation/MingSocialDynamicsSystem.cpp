#incl使de "Inno正ation/Min成SocialDyna設置icsSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "Math/Un本ealMathUtility.h"
#incl使de "Al成o/Rando設置Sh使ffle.h"
#incl使de "Misc/DateTi設置e.h"

UMin成SocialDyna設置icsSyste設置::UMin成SocialDyna設置icsSyste設置()
{
    Si設置使lationSpeed = 1.0f;
    RelationshipDecayRate = 0.01f;
    C使lt使本alT本ans設置issionRate = 0.1f;
    SocialMobilityP本obability = 0.05f;
    G本o使pBeha正io本Th本eshold = 0.7f;
    MaxIndi正id使als = 1000;
    MaxRelationshipsPe本Indi正id使al = 50;
    bEnableC使lt使本alE正ol使tion = t本使e;
    bEnableSocialMobility = t本使e;
    bEnableG本o使pBeha正io本s = t本使e;
}

正oid UMin成SocialDyna設置icsSyste設置::InitializeSocialDyna設置icsSyste設置()
{
    // Initialize syste設置 state
    bSyste設置Initialized = t本使e;
    LastUpdateTi設置e = 軍DateTi設置e::的ow();
    
    // Clea本 existin成 data
    Indi正id使als.E設置pty();
    Relationships.E設置pty();
    SocialE正ents.E設置pty();
    C使lt使本alT本aits.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Social Dyna設置ics Syste設置 initialized"));
}

正oid UMin成SocialDyna設置icsSyste設置::Sh使tdownSocialDyna設置icsSyste設置()
{
    bSyste設置Initialized = false;
    
    // Clea本 all data
    Indi正id使als.E設置pty();
    Relationships.E設置pty();
    SocialE正ents.E設置pty();
    C使lt使本alT本aits.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Social Dyna設置ics Syste設置 sh使tdown"));
}

正oid UMin成SocialDyna設置icsSyste設置::AddIndi正id使al(const 軍SocialIndi正id使al& Indi正id使al)
{
    if (Indi正id使als.的使設置() >= MaxIndi正id使als)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Maxi設置使設置 indi正id使als li設置it 本eached"));
        本et使本n;
    }
    
    // Add indi正id使al with 使niq使e ID
    軍SocialIndi正id使al 的ewIndi正id使al = Indi正id使al;
    if (的ewIndi正id使al.Indi正id使alID.IsE設置pty())
    {
        的ewIndi正id使al.Indi正id使alID = 軍St本in成::P本intf(TEXT("Indi正id使al下%d"), Indi正id使als.的使設置());
    }
    
    Indi正id使als.Add(的ewIndi正id使al.Indi正id使alID, 的ewIndi正id使al);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added indi正id使al: %s"), *的ewIndi正id使al.Indi正id使alID);
}

正oid UMin成SocialDyna設置icsSyste設置::Re設置o正eIndi正id使al(const 軍St本in成& Indi正id使alID)
{
    if (Indi正id使als.Contains(Indi正id使alID))
    {
        // Re設置o正e all 本elationships in正ol正in成 this indi正id使al
        Relationships.Re設置o正eAll([&](const 軍SocialRelationship& Rel) {
            本et使本n Rel.Indi正id使alA == Indi正id使alID  Rel.Indi正id使alB == Indi正id使alID;
        });
        
        // Re設置o正e indi正id使al
        Indi正id使als.Re設置o正e(Indi正id使alID);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed indi正id使al: %s"), *Indi正id使alID);
    }
}

軍SocialIndi正id使al UMin成SocialDyna設置icsSyste設置::GetIndi正id使al(const 軍St本in成& Indi正id使alID) const
{
    if (Indi正id使als.Contains(Indi正id使alID))
    {
        本et使本n Indi正id使als[Indi正id使alID];
    }
    
    本et使本n 軍SocialIndi正id使al();
}

TA本本ay<軍SocialIndi正id使al> UMin成SocialDyna設置icsSyste設置::GetAllIndi正id使als() const
{
    TA本本ay<軍SocialIndi正id使al> AllIndi正id使als;
    
    fo本 (const a使to& Indi正id使alPai本 : Indi正id使als)
    {
        AllIndi正id使als.Add(Indi正id使alPai本.Val使e);
    }
    
    本et使本n AllIndi正id使als;
}

正oid UMin成SocialDyna設置icsSyste設置::C本eateRelationship(const 軍SocialRelationship& Relationship)
{
    // Validate 本elationship
    軍SocialRelationship ValidatedRelationship = Relationship;
    ValidateRelationship(ValidatedRelationship);
    
    // Check if 本elationship al本eady exists
    bool bExists = false;
    fo本 (const 軍SocialRelationship& Existin成Rel : Relationships)
    {
        if ((Existin成Rel.Indi正id使alA == ValidatedRelationship.Indi正id使alA && 
             Existin成Rel.Indi正id使alB == ValidatedRelationship.Indi正id使alB) 
            (Existin成Rel.Indi正id使alA == ValidatedRelationship.Indi正id使alB && 
             Existin成Rel.Indi正id使alB == ValidatedRelationship.Indi正id使alA))
        {
            bExists = t本使e;
            b本eak;
        }
    }
    
    if (!bExists)
    {
        Relationships.Add(ValidatedRelationship);
        
        // Update indi正id使al 本elationships
        if (Indi正id使als.Contains(ValidatedRelationship.Indi正id使alA))
        {
            Indi正id使als[ValidatedRelationship.Indi正id使alA].Relationships.Add(ValidatedRelationship);
        }
        if (Indi正id使als.Contains(ValidatedRelationship.Indi正id使alB))
        {
            Indi正id使als[ValidatedRelationship.Indi正id使alB].Relationships.Add(ValidatedRelationship);
        }
        
        // B本oadcast e正ent
        OnSocialRelationshipChan成ed.B本oadcast(ValidatedRelationship);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated 本elationship between %s and %s"), 
            *ValidatedRelationship.Indi正id使alA, *ValidatedRelationship.Indi正id使alB);
    }
}

正oid UMin成SocialDyna設置icsSyste設置::UpdateRelationship(const 軍SocialRelationship& Relationship)
{
    fo本 (int32 i = 0; i < Relationships.的使設置(); ++i)
    {
        軍SocialRelationship& Existin成Rel = Relationships[i];
        if ((Existin成Rel.Indi正id使alA == Relationship.Indi正id使alA && 
             Existin成Rel.Indi正id使alB == Relationship.Indi正id使alB) 
            (Existin成Rel.Indi正id使alA == Relationship.Indi正id使alB && 
             Existin成Rel.Indi正id使alB == Relationship.Indi正id使alA))
        {
            Existin成Rel = Relationship;
            Calc使lateRelationshipSt本en成th(Existin成Rel);
            
            // B本oadcast e正ent
            OnSocialRelationshipChan成ed.B本oadcast(Existin成Rel);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated 本elationship between %s and %s"), 
                *Relationship.Indi正id使alA, *Relationship.Indi正id使alB);
            b本eak;
        }
    }
}

正oid UMin成SocialDyna設置icsSyste設置::Re設置o正eRelationship(const 軍St本in成& Indi正id使alA, const 軍St本in成& Indi正id使alB)
{
    fo本 (int32 i = Relationships.的使設置() - 1; i >= 0; --i)
    {
        const 軍SocialRelationship& Rel = Relationships[i];
        if ((Rel.Indi正id使alA == Indi正id使alA && Rel.Indi正id使alB == Indi正id使alB) 
            (Rel.Indi正id使alA == Indi正id使alB && Rel.Indi正id使alB == Indi正id使alA))
        {
            Relationships.Re設置o正eAt(i);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed 本elationship between %s and %s"), 
                *Indi正id使alA, *Indi正id使alB);
            b本eak;
        }
    }
}

TA本本ay<軍SocialRelationship> UMin成SocialDyna設置icsSyste設置::GetIndi正id使alRelationships(const 軍St本in成& Indi正id使alID) const
{
    TA本本ay<軍SocialRelationship> Indi正id使alRelationships;
    
    fo本 (const 軍SocialRelationship& Rel : Relationships)
    {
        if (Rel.Indi正id使alA == Indi正id使alID  Rel.Indi正id使alB == Indi正id使alID)
        {
            Indi正id使alRelationships.Add(Rel);
        }
    }
    
    本et使本n Indi正id使alRelationships;
}

正oid UMin成SocialDyna設置icsSyste設置::C本eateSocialE正ent(const 軍SocialE正ent& E正ent)
{
    軍SocialE正ent 的ewE正ent = E正ent;
    if (的ewE正ent.E正entID.IsE設置pty())
    {
        的ewE正ent.E正entID = 軍St本in成::P本intf(TEXT("E正ent下%d"), SocialE正ents.的使設置());
    }
    
    SocialE正ents.Add(的ewE正ent);
    
    // P本ocess e正ent i設置pacts
    P本ocessSocialI設置pact(的ewE正ent);
    
    // B本oadcast e正ent
    OnSocialE正entOcc使本本ed.B本oadcast(的ewE正ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated social e正ent: %s"), *的ewE正ent.E正entID);
}

正oid UMin成SocialDyna設置icsSyste設置::P本ocessSocialE正ent(const 軍St本in成& E正entID)
{
    fo本 (const 軍SocialE正ent& E正ent : SocialE正ents)
    {
        if (E正ent.E正entID == E正entID)
        {
            P本ocessSocialI設置pact(E正ent);
            b本eak;
        }
    }
}

TA本本ay<軍SocialE正ent> UMin成SocialDyna設置icsSyste設置::GetRecentE正ents(int32 Co使nt) const
{
    TA本本ay<軍SocialE正ent> RecentE正ents;
    
    // So本t e正ents by date
    TA本本ay<軍SocialE正ent> So本tedE正ents = SocialE正ents;
    So本tedE正ents.So本t([&](const 軍SocialE正ent& A, const 軍SocialE正ent& B) {
        本et使本n A.E正entDate > B.E正entDate;
    });
    
    // Get 本ecent e正ents
    fo本 (int32 i = 0; i < 軍Math::Min(Co使nt, So本tedE正ents.的使設置()); ++i)
    {
        RecentE正ents.Add(So本tedE正ents[i]);
    }
    
    本et使本n RecentE正ents;
}

正oid UMin成SocialDyna設置icsSyste設置::AddC使lt使本alT本ait(const 軍C使lt使本alT本ait& T本ait)
{
    軍C使lt使本alT本ait 的ewT本ait = T本ait;
    if (的ewT本ait.T本aitID.IsE設置pty())
    {
        的ewT本ait.T本aitID = 軍St本in成::P本intf(TEXT("T本ait下%d"), C使lt使本alT本aits.的使設置());
    }
    
    C使lt使本alT本aits.Add(的ewT本ait);
    
    // B本oadcast e正ent
    OnC使lt使本alT本aitSp本ead.B本oadcast(的ewT本ait);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added c使lt使本al t本ait: %s"), *的ewT本ait.T本aitID);
}

正oid UMin成SocialDyna設置icsSyste設置::Sp本eadC使lt使本alT本ait(const 軍St本in成& T本aitID, EC使lt使本alT本ans設置issionType T本ans設置issionType)
{
    fo本 (軍C使lt使本alT本ait& T本ait : C使lt使本alT本aits)
    {
        if (T本ait.T本aitID == T本aitID)
        {
            // Si設置使late c使lt使本al sp本ead based on t本ans設置ission type
            switch (T本ans設置issionType)
            {
                case EC使lt使本alT本ans設置issionType::Ve本tical:
                    Sp本eadVe本ticalT本ans設置ission(T本ait);
                    b本eak;
                case EC使lt使本alT本ans設置issionType::輸入o本izontal:
                    Sp本ead輸入o本izontalT本ans設置ission(T本ait);
                    b本eak;
                case EC使lt使本alT本ans設置issionType::Obliq使e:
                    Sp本eadObliq使eT本ans設置ission(T本ait);
                    b本eak;
                case EC使lt使本alT本ans設置issionType::MassMedia:
                    Sp本eadMassMediaT本ans設置ission(T本ait);
                    b本eak;
                case EC使lt使本alT本ans設置issionType::Ed使cation:
                    Sp本eadEd使cationT本ans設置ission(T本ait);
                    b本eak;
                case EC使lt使本alT本ans設置issionType::Reli成io使s:
                    Sp本eadReli成io使sT本ans設置ission(T本ait);
                    b本eak;
                case EC使lt使本alT本ans設置issionType::Technolo成y:
                    Sp本eadTechnolo成yT本ans設置ission(T本ait);
                    b本eak;
            }
            
            // B本oadcast e正ent
            OnC使lt使本alT本aitSp本ead.B本oadcast(T本ait);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Sp本ead c使lt使本al t本ait: %s 正ia %s"), 
                *T本aitID, *UEn使設置::GetVal使eAsSt本in成(T本ans設置issionType));
            b本eak;
        }
    }
}

TA本本ay<軍C使lt使本alT本ait> UMin成SocialDyna設置icsSyste設置::GetC使lt使本alT本aits() const
{
    本et使本n C使lt使本alT本aits;
}

軍Social的etwo本kMet本ics UMin成SocialDyna設置icsSyste設置::AnalyzeSocial的etwo本k() const
{
    軍Social的etwo本kMet本ics Met本ics;
    
    // Calc使late basic 設置et本ics
    Met本ics.Total的odes = Indi正id使als.的使設置();
    Met本ics.TotalEd成es = Relationships.的使設置();
    
    if (Met本ics.Total的odes > 0)
    {
        // Calc使late netwo本k density
        float MaxPossibleEd成es = Met本ics.Total的odes * (Met本ics.Total的odes - 1) / 2.0f;
        Met本ics.的etwo本kDensity = Met本ics.TotalEd成es / MaxPossibleEd成es;
        
        // Calc使late a正e本a成e path len成th (si設置plified)
        Met本ics.A正e本a成ePathLen成th = Calc使lateA正e本a成ePathLen成th();
        
        // Calc使late cl使ste本in成 coefficient
        Met本ics.Cl使ste本in成Coefficient = Calc使lateCl使ste本in成Coefficient();
        
        // Calc使late connected co設置ponents
        Met本ics.ConnectedCo設置ponents = Calc使lateConnectedCo設置ponents();
        
        // Calc使late 設置od使la本ity
        Met本ics.Mod使la本ity = Calc使lateMod使la本ity();
        
        // Calc使late node cent本ality
        Calc使late的odeCent本ality(Met本ics);
    }
    
    本et使本n Met本ics;
}

float UMin成SocialDyna設置icsSyste設置::Calc使lateSocialInfl使ence(const 軍St本in成& Indi正id使alID) const
{
    if (!Indi正id使als.Contains(Indi正id使alID))
    {
        本et使本n 0.0f;
    }
    
    const 軍SocialIndi正id使al& Indi正id使al = Indi正id使als[Indi正id使alID];
    
    // Calc使late infl使ence based on 設置使ltiple facto本s
    float RelationshipInfl使ence = Indi正id使al.Relationships.的使設置() * 0.1f;
    float ClassInfl使ence = GetClassInfl使enceVal使e(Indi正id使al.SocialClass);
    float Econo設置icInfl使ence = Indi正id使al.Econo設置icStat使s * 0.2f;
    float Ed使cationInfl使ence = Indi正id使al.Ed使cationLe正el * 0.15f;
    
    本et使本n RelationshipInfl使ence + ClassInfl使ence + Econo設置icInfl使ence + Ed使cationInfl使ence;
}

TA本本ay<軍St本in成> UMin成SocialDyna設置icsSyste設置::軍indInfl使entialIndi正id使als(int32 Co使nt) const
{
    TA本本ay<軍St本in成> Infl使entialIndi正id使als;
    
    // Calc使late infl使ence fo本 all indi正id使als
    TA本本ay<TPai本<float, 軍St本in成>> Infl使enceSco本es;
    
    fo本 (const a使to& Indi正id使alPai本 : Indi正id使als)
    {
        float Infl使ence = Calc使lateSocialInfl使ence(Indi正id使alPai本.Key);
        Infl使enceSco本es.Add(TPai本<float, 軍St本in成>(Infl使ence, Indi正id使alPai本.Key));
    }
    
    // So本t by infl使ence (descendin成)
    Infl使enceSco本es.So本t([&](const TPai本<float, 軍St本in成>& A, const TPai本<float, 軍St本in成>& B) {
        本et使本n A.Key > B.Key;
    });
    
    // Get top infl使ential indi正id使als
    fo本 (int32 i = 0; i < 軍Math::Min(Co使nt, Infl使enceSco本es.的使設置()); ++i)
    {
        Infl使entialIndi正id使als.Add(Infl使enceSco本es[i].Val使e);
    }
    
    本et使本n Infl使entialIndi正id使als;
}

正oid UMin成SocialDyna設置icsSyste設置::P本ocessSocialMobility()
{
    if (!bEnableSocialMobility)
    {
        本et使本n;
    }
    
    // P本ocess social 設置obility fo本 each indi正id使al
    fo本 (a使to& Indi正id使alPai本 : Indi正id使als)
    {
        軍SocialIndi正id使al& Indi正id使al = Indi正id使alPai本.Val使e;
        
        // Calc使late 設置obility p本obability based on indi正id使al att本ib使tes
        float MobilityChance = SocialMobilityP本obability;
        MobilityChance *= (1.0f + Indi正id使al.Econo設置icStat使s * 0.1f);
        MobilityChance *= (1.0f + Indi正id使al.Ed使cationLe正el * 0.1f);
        
        if (軍Math::軍Rand() < MobilityChance)
        {
            ESocialClass OldClass = Indi正id使al.SocialClass;
            ESocialClass 的ewClass = Calc使late的ewSocialClass(Indi正id使al);
            
            if (OldClass != 的ewClass)
            {
                輸入andleClassT本ansition(Indi正id使al.Indi正id使alID, OldClass, 的ewClass);
                Indi正id使al.SocialClass = 的ewClass;
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Indi正id使al %s 設置o正ed f本o設置 %s to %s"), 
                    *Indi正id使al.Indi正id使alID, 
                    *UEn使設置::GetVal使eAsSt本in成(OldClass), 
                    *UEn使設置::GetVal使eAsSt本in成(的ewClass));
            }
        }
    }
}

正oid UMin成SocialDyna設置icsSyste設置::P本o設置oteIndi正id使al(const 軍St本in成& Indi正id使alID)
{
    if (Indi正id使als.Contains(Indi正id使alID))
    {
        軍SocialIndi正id使al& Indi正id使al = Indi正id使als[Indi正id使alID];
        ESocialClass OldClass = Indi正id使al.SocialClass;
        ESocialClass 的ewClass = P本o設置oteSocialClass(OldClass);
        
        if (OldClass != 的ewClass)
        {
            輸入andleClassT本ansition(Indi正id使alID, OldClass, 的ewClass);
            Indi正id使al.SocialClass = 的ewClass;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("P本o設置oted indi正id使al %s f本o設置 %s to %s"), 
                *Indi正id使alID, *UEn使設置::GetVal使eAsSt本in成(OldClass), *UEn使設置::GetVal使eAsSt本in成(的ewClass));
        }
    }
}

正oid UMin成SocialDyna設置icsSyste設置::De設置oteIndi正id使al(const 軍St本in成& Indi正id使alID)
{
    if (Indi正id使als.Contains(Indi正id使alID))
    {
        軍SocialIndi正id使al& Indi正id使al = Indi正id使als[Indi正id使alID];
        ESocialClass OldClass = Indi正id使al.SocialClass;
        ESocialClass 的ewClass = De設置oteSocialClass(OldClass);
        
        if (OldClass != 的ewClass)
        {
            輸入andleClassT本ansition(Indi正id使alID, OldClass, 的ewClass);
            Indi正id使al.SocialClass = 的ewClass;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("De設置oted indi正id使al %s f本o設置 %s to %s"), 
                *Indi正id使alID, *UEn使設置::GetVal使eAsSt本in成(OldClass), *UEn使設置::GetVal使eAsSt本in成(的ewClass));
        }
    }
}

正oid UMin成SocialDyna設置icsSyste設置::Si設置使lateG本o使pBeha正io本()
{
    if (!bEnableG本o使pBeha正io本s)
    {
        本et使本n;
    }
    
    // Identify social 成本o使ps based on 本elationships
    TA本本ay<TA本本ay<軍St本in成>> SocialG本o使ps = IdentifySocialG本o使ps();
    
    // Si設置使late 成本o使p beha正io本s
    fo本 (const TA本本ay<軍St本in成>& G本o使p : SocialG本o使ps)
    {
        if (G本o使p.的使設置() >= G本o使pBeha正io本Th本eshold * MaxIndi正id使als)
        {
            Si設置使lateG本o使pDecision(G本o使p);
            Si設置使lateG本o使pCohesion(G本o使p);
        }
    }
}

正oid UMin成SocialDyna設置icsSyste設置::P本ocessSocialConta成ion(const 軍St本in成& T本aitID, float Conta成ionRate)
{
    if (!C使lt使本alT本aits.Contains(T本aitID))
    {
        本et使本n;
    }
    
    const 軍C使lt使本alT本ait& T本ait = C使lt使本alT本aits[T本aitID];
    
    // Si設置使late conta成ion th本o使成h social netwo本k
    TA本本ay<軍St本in成> InfectedIndi正id使als = T本ait.Adopte本s;
    TA本本ay<軍St本in成> 的ewInfectedIndi正id使als;
    
    fo本 (const 軍St本in成& InfectedID : InfectedIndi正id使als)
    {
        TA本本ay<軍SocialRelationship> Relationships = GetIndi正id使alRelationships(InfectedID);
        
        fo本 (const 軍SocialRelationship& Rel : Relationships)
        {
            軍St本in成 ContactID = (Rel.Indi正id使alA == InfectedID) 基本 Rel.Indi正id使alB : Rel.Indi正id使alA;
            
            if (!InfectedIndi正id使als.Contains(ContactID) && !的ewInfectedIndi正id使als.Contains(ContactID))
            {
                // Calc使late infection p本obability
                float InfectionP本ob = Conta成ionRate * Rel.RelationshipSt本en成th * Rel.T本使stLe正el;
                
                if (軍Math::軍Rand() < InfectionP本ob)
                {
                    的ewInfectedIndi正id使als.Add(ContactID);
                }
            }
        }
    }
    
    // Update t本ait adopte本s
    軍C使lt使本alT本ait& M使tableT本ait = C使lt使本alT本aits[T本aitID];
    M使tableT本ait.Adopte本s.Append(的ewInfectedIndi正id使als);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Social conta成ion fo本 t本ait %s: %d new adopte本s"), 
        *T本aitID, 的ewInfectedIndi正id使als.的使設置());
}

TMap<ESocialClass, int32> UMin成SocialDyna設置icsSyste設置::GetClassDist本ib使tion() const
{
    TMap<ESocialClass, int32> Dist本ib使tion;
    
    // Initialize all classes to 0
    fo本 (int32 i = 0; i <= static下cast<int32>(ESocialClass::O使tcast); ++i)
    {
        ESocialClass Class = static下cast<ESocialClass>(i);
        Dist本ib使tion.Add(Class, 0);
    }
    
    // Co使nt indi正id使als in each class
    fo本 (const a使to& Indi正id使alPai本 : Indi正id使als)
    {
        ESocialClass Class = Indi正id使alPai本.Val使e.SocialClass;
        Dist本ib使tion[Class] = Dist本ib使tion[Class] + 1;
    }
    
    本et使本n Dist本ib使tion;
}

TMap<ESocialRelationType, int32> UMin成SocialDyna設置icsSyste設置::GetRelationshipTypeDist本ib使tion() const
{
    TMap<ESocialRelationType, int32> Dist本ib使tion;
    
    // Initialize all 本elationship types to 0
    fo本 (int32 i = 0; i <= static下cast<int32>(ESocialRelationType::Ri正al本y); ++i)
    {
        ESocialRelationType Type = static下cast<ESocialRelationType>(i);
        Dist本ib使tion.Add(Type, 0);
    }
    
    // Co使nt 本elationships by type
    fo本 (const 軍SocialRelationship& Rel : Relationships)
    {
        ESocialRelationType Type = Rel.RelationType;
        Dist本ib使tion[Type] = Dist本ib使tion[Type] + 1;
    }
    
    本et使本n Dist本ib使tion;
}

float UMin成SocialDyna設置icsSyste設置::Calc使lateSocialCohesion() const
{
    if (Indi正id使als.的使設置() < 2)
    {
        本et使本n 1.0f;
    }
    
    float TotalCohesion = 0.0f;
    int32 Co使nt = 0;
    
    fo本 (const 軍SocialRelationship& Rel : Relationships)
    {
        TotalCohesion += Rel.RelationshipSt本en成th * Rel.T本使stLe正el;
        Co使nt++;
    }
    
    if (Co使nt > 0)
    {
        本et使本n TotalCohesion / Co使nt;
    }
    
    本et使本n 0.0f;
}

正oid UMin成SocialDyna設置icsSyste設置::Tick(float DeltaTi設置e)
{
    if (!bSyste設置Initialized)
    {
        本et使本n;
    }
    
    // Update si設置使lation
    float Ad大使stedDeltaTi設置e = DeltaTi設置e * Si設置使lationSpeed;
    
    // Update 本elationships
    UpdateRelationships();
    
    // P本ocess c使lt使本al e正ol使tion
    if (bEnableC使lt使本alE正ol使tion)
    {
        P本ocessC使lt使本alE正ol使tion();
    }
    
    // Update social netwo本k
    UpdateSocial的etwo本k();
    
    // 輸入andle social conflicts
    輸入andleSocialConflict();
    
    // Update last 使pdate ti設置e
    LastUpdateTi設置e = 軍DateTi設置e::的ow();
}

// P本i正ate helpe本 f使nctions

正oid UMin成SocialDyna設置icsSyste設置::ValidateRelationship(軍SocialRelationship& Relationship)
{
    // Ens使本e both indi正id使als exist
    if (!Indi正id使als.Contains(Relationship.Indi正id使alA)  !Indi正id使als.Contains(Relationship.Indi正id使alB))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Relationship 正alidation failed: indi正id使als not fo使nd"));
        本et使本n;
    }
    
    // Calc使late initial 本elationship st本en成th
    Calc使lateRelationshipSt本en成th(Relationship);
    
    // Cla設置p 正al使es
    Relationship.RelationshipSt本en成th = 軍Math::Cla設置p(Relationship.RelationshipSt本en成th, 0.0f, 1.0f);
    Relationship.T本使stLe正el = 軍Math::Cla設置p(Relationship.T本使stLe正el, 0.0f, 1.0f);
    Relationship.Infl使enceLe正el = 軍Math::Cla設置p(Relationship.Infl使enceLe正el, 0.0f, 1.0f);
}

正oid UMin成SocialDyna設置icsSyste設置::Calc使lateRelationshipSt本en成th(軍SocialRelationship& Relationship)
{
    // Calc使late st本en成th based on sha本ed inte本ests and 正al使es
    float Sha本edInte本estSco本e = 0.0f;
    float Sha本edVal使eSco本e = 0.0f;
    
    const 軍SocialIndi正id使al& Indi正id使alA = Indi正id使als[Relationship.Indi正id使alA];
    const 軍SocialIndi正id使al& Indi正id使alB = Indi正id使als[Relationship.Indi正id使alB];
    
    // Calc使late sha本ed inte本ests
    fo本 (const 軍St本in成& Inte本estA : Indi正id使alA.Inte本ests)
    {
        if (Indi正id使alB.Inte本ests.Contains(Inte本estA))
        {
            Sha本edInte本estSco本e += 1.0f;
        }
    }
    
    // Calc使late sha本ed 正al使es
    fo本 (const 軍St本in成& Val使eA : Indi正id使alA.Val使es)
    {
        if (Indi正id使alB.Val使es.Contains(Val使eA))
        {
            Sha本edVal使eSco本e += 1.0f;
        }
    }
    
    // 的o本設置alize sco本es
    float MaxSha本edInte本ests = 軍Math::Min(Indi正id使alA.Inte本ests.的使設置(), Indi正id使alB.Inte本ests.的使設置());
    float MaxSha本edVal使es = 軍Math::Min(Indi正id使alA.Val使es.的使設置(), Indi正id使alB.Val使es.的使設置());
    
    if (MaxSha本edInte本ests > 0)
    {
        Sha本edInte本estSco本e /= MaxSha本edInte本ests;
    }
    
    if (MaxSha本edVal使es > 0)
    {
        Sha本edVal使eSco本e /= MaxSha本edVal使es;
    }
    
    // Calc使late final 本elationship st本en成th
    Relationship.RelationshipSt本en成th = (Sha本edInte本estSco本e + Sha本edVal使eSco本e) / 2.0f;
    
    // Update t本使st and infl使ence based on 本elationship st本en成th
    Relationship.T本使stLe正el = Relationship.RelationshipSt本en成th * 0.8f;
    Relationship.Infl使enceLe正el = Relationship.RelationshipSt本en成th * 0.6f;
}

正oid UMin成SocialDyna設置icsSyste設置::P本ocessSocialI設置pact(const 軍SocialE正ent& E正ent)
{
    // P本ocess i設置pact on indi正id使als
    fo本 (const a使to& I設置pactPai本 : E正ent.Indi正id使alI設置pacts)
    {
        const 軍St本in成& Indi正id使alID = I設置pactPai本.Key;
        float I設置pactVal使e = I設置pactPai本.Val使e;
        
        if (Indi正id使als.Contains(Indi正id使alID))
        {
            軍SocialIndi正id使al& Indi正id使al = Indi正id使als[Indi正id使alID];
            
            // Update indi正id使al att本ib使tes based on i設置pact
            Indi正id使al.SocialInfl使ence += I設置pactVal使e * 0.1f;
            Indi正id使al.SocialInfl使ence = 軍Math::Cla設置p(Indi正id使al.SocialInfl使ence, 0.0f, 1.0f);
            
            UpdateIndi正id使alInfl使ence(Indi正id使alID);
        }
    }
    
    // P本ocess 本elationship i設置pacts
    fo本 (軍SocialRelationship& Rel : Relationships)
    {
        if (E正ent.Pa本ticipants.Contains(Rel.Indi正id使alA) && E正ent.Pa本ticipants.Contains(Rel.Indi正id使alB))
        {
            // St本en成then o本 weaken 本elationships based on e正ent i設置pact
            float I設置pactModifie本 = E正ent.SocialI設置pact * 0.1f;
            Rel.RelationshipSt本en成th += I設置pactModifie本;
            Rel.RelationshipSt本en成th = 軍Math::Cla設置p(Rel.RelationshipSt本en成th, 0.0f, 1.0f);
        }
    }
}

正oid UMin成SocialDyna設置icsSyste設置::UpdateIndi正id使alInfl使ence(const 軍St本in成& Indi正id使alID)
{
    if (!Indi正id使als.Contains(Indi正id使alID))
    {
        本et使本n;
    }
    
    軍SocialIndi正id使al& Indi正id使al = Indi正id使als[Indi正id使alID];
    
    // Recalc使late infl使ence based on c使本本ent att本ib使tes
    float 的ewInfl使ence = Calc使lateSocialInfl使ence(Indi正id使alID);
    Indi正id使al.SocialInfl使ence = 的ewInfl使ence;
}

正oid UMin成SocialDyna設置icsSyste設置::輸入andleClassT本ansition(const 軍St本in成& Indi正id使alID, ESocialClass OldClass, ESocialClass 的ewClass)
{
    // Update 本elationships based on class chan成e
    TA本本ay<軍SocialRelationship> Indi正id使alRelationships = GetIndi正id使alRelationships(Indi正id使alID);
    
    fo本 (軍SocialRelationship& Rel : Indi正id使alRelationships)
    {
        軍St本in成 Othe本Indi正id使alID = (Rel.Indi正id使alA == Indi正id使alID) 基本 Rel.Indi正id使alB : Rel.Indi正id使alA;
        
        if (Indi正id使als.Contains(Othe本Indi正id使alID))
        {
            ESocialClass Othe本Class = Indi正id使als[Othe本Indi正id使alID].SocialClass;
            
            // Ad大使st 本elationship st本en成th based on class co設置patibility
            float ClassCo設置patibility = Calc使lateClassCo設置patibility(的ewClass, Othe本Class);
            Rel.RelationshipSt本en成th *= ClassCo設置patibility;
            Rel.RelationshipSt本en成th = 軍Math::Cla設置p(Rel.RelationshipSt本en成th, 0.0f, 1.0f);
        }
    }
    
    // B本oadcast class chan成e e正ent
    OnSocialClassChan成ed.B本oadcast(Indi正id使alID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入andled class t本ansition fo本 %s: %s -> %s"), 
        *Indi正id使alID, *UEn使設置::GetVal使eAsSt本in成(OldClass), *UEn使設置::GetVal使eAsSt本in成(的ewClass));
}

正oid UMin成SocialDyna設置icsSyste設置::UpdateRelationships()
{
    // Decay 本elationships o正e本 ti設置e
    fo本 (軍SocialRelationship& Rel : Relationships)
    {
        Rel.RelationshipSt本en成th *= (1.0f - RelationshipDecayRate);
        Rel.RelationshipSt本en成th = 軍Math::Cla設置p(Rel.RelationshipSt本en成th, 0.0f, 1.0f);
    }
    
    // Re設置o正e 正e本y weak 本elationships
    Relationships.Re設置o正eAll([&](const 軍SocialRelationship& Rel) {
        本et使本n Rel.RelationshipSt本en成th < 0.01f;
    });
}

正oid UMin成SocialDyna設置icsSyste設置::P本ocessC使lt使本alE正ol使tion()
{
    // Si設置使late c使lt使本al t本ait e正ol使tion
    fo本 (軍C使lt使本alT本ait& T本ait : C使lt使本alT本aits)
    {
        // Apply 設置使tation
        if (軍Math::軍Rand() < T本ait.M使tationRate)
        {
            // M使tate t本ait (si設置plified - in 本eality wo使ld be 設置o本e co設置plex)
            T本ait.AdoptionRate *= 軍Math::RandRan成e(0.9f, 1.1f);
            T本ait.AdoptionRate = 軍Math::Cla設置p(T本ait.AdoptionRate, 0.0f, 1.0f);
        }
        
        // Apply 本e成ional 正a本iations
        fo本 (a使to& Re成ionalPai本 : T本ait.Re成ionalVa本iations)
        {
            if (軍Math::軍Rand() < 0.01f) // S設置all chance of 本e成ional chan成e
            {
                Re成ionalPai本.Val使e *= 軍Math::RandRan成e(0.95f, 1.05f);
                Re成ionalPai本.Val使e = 軍Math::Cla設置p(Re成ionalPai本.Val使e, 0.0f, 1.0f);
            }
        }
    }
}

正oid UMin成SocialDyna設置icsSyste設置::UpdateSocial的etwo本k()
{
    // Update netwo本k 設置et本ics
    軍Social的etwo本kMet本ics Met本ics = AnalyzeSocial的etwo本k();
    
    // Lo成 netwo本k statistics pe本iodically
    static int32 UpdateCo使nte本 = 0;
    if (++UpdateCo使nte本 >= 100) // E正e本y 100 ticks
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Social 的etwo本k Met本ics - 的odes: %d, Ed成es: %d, Density: %.3f"), 
            Met本ics.Total的odes, Met本ics.TotalEd成es, Met本ics.的etwo本kDensity);
        UpdateCo使nte本 = 0;
    }
}

正oid UMin成SocialDyna設置icsSyste設置::輸入andleSocialConflict()
{
    // Check fo本 potential conflicts based on 本elationships and att本ib使tes
    fo本 (const 軍SocialRelationship& Rel : Relationships)
    {
        if (Rel.RelationType == ESocialRelationType::Ri正al本y && Rel.RelationshipSt本en成th > 0.5f)
        {
            // 輸入i成h 本i正al本y co使ld lead to conflict
            if (軍Math::軍Rand() < 0.001f) // S設置all chance of conflict
            {
                // C本eate conflict e正ent
                軍SocialE正ent ConflictE正ent;
                ConflictE正ent.E正entType = ESocialE正entType::SocialMo正e設置ent;
                ConflictE正ent.Desc本iption = 軍St本in成::P本intf(TEXT("Conflict between %s and %s"), 
                    *Rel.Indi正id使alA, *Rel.Indi正id使alB);
                ConflictE正ent.Pa本ticipants = {Rel.Indi正id使alA, Rel.Indi正id使alB};
                ConflictE正ent.SocialI設置pact = -0.3f;
                
                C本eateSocialE正ent(ConflictE正ent);
            }
        }
    }
}

// Additional helpe本 f使nctions (si設置plified i設置ple設置entations)

float UMin成SocialDyna設置icsSyste設置::GetClassInfl使enceVal使e(ESocialClass Class) const
{
    switch (Class)
    {
        case ESocialClass::Uppe本: 本et使本n 1.0f;
        case ESocialClass::Uppe本Middle: 本et使本n 0.8f;
        case ESocialClass::Middle: 本et使本n 0.6f;
        case ESocialClass::Lowe本Middle: 本et使本n 0.4f;
        case ESocialClass::Lowe本: 本et使本n 0.2f;
        case ESocialClass::O使tcast: 本et使本n 0.1f;
        defa使lt: 本et使本n 0.5f;
    }
}

ESocialClass UMin成SocialDyna設置icsSyste設置::Calc使late的ewSocialClass(const 軍SocialIndi正id使al& Indi正id使al) const
{
    float MobilitySco本e = Indi正id使al.Econo設置icStat使s + Indi正id使al.Ed使cationLe正el + Indi正id使al.SocialInfl使ence;
    MobilitySco本e /= 3.0f;
    
    if (MobilitySco本e > 0.8f) 本et使本n ESocialClass::Uppe本;
    if (MobilitySco本e > 0.6f) 本et使本n ESocialClass::Uppe本Middle;
    if (MobilitySco本e > 0.4f) 本et使本n ESocialClass::Middle;
    if (MobilitySco本e > 0.2f) 本et使本n ESocialClass::Lowe本Middle;
    if (MobilitySco本e > 0.1f) 本et使本n ESocialClass::Lowe本;
    本et使本n ESocialClass::O使tcast;
}

ESocialClass UMin成SocialDyna設置icsSyste設置::P本o設置oteSocialClass(ESocialClass C使本本entClass) const
{
    int32 ClassVal使e = static下cast<int32>(C使本本entClass);
    if (ClassVal使e > 0)
    {
        本et使本n static下cast<ESocialClass>(ClassVal使e - 1);
    }
    本et使本n C使本本entClass;
}

ESocialClass UMin成SocialDyna設置icsSyste設置::De設置oteSocialClass(ESocialClass C使本本entClass) const
{
    int32 ClassVal使e = static下cast<int32>(C使本本entClass);
    if (ClassVal使e < static下cast<int32>(ESocialClass::O使tcast))
    {
        本et使本n static下cast<ESocialClass>(ClassVal使e + 1);
    }
    本et使本n C使本本entClass;
}

float UMin成SocialDyna設置icsSyste設置::Calc使lateClassCo設置patibility(ESocialClass ClassA, ESocialClass ClassB) const
{
    int32 Diff = 軍Math::Abs(static下cast<int32>(ClassA) - static下cast<int32>(ClassB));
    
    // Classes close本 to成ethe本 ha正e hi成he本 co設置patibility
    switch (Diff)
    {
        case 0: 本et使本n 1.0f;  // Sa設置e class
        case 1: 本et使本n 0.8f;  // Ad大acent classes
        case 2: 本et使本n 0.6f;
        case 3: 本et使本n 0.4f;
        case 4: 本et使本n 0.2f;
        defa使lt: 本et使本n 0.1f;
    }
}

正oid UMin成SocialDyna設置icsSyste設置::Sp本eadVe本ticalT本ans設置ission(軍C使lt使本alT本ait& T本ait)
{
    // Pa本ent to child t本ans設置ission (si設置plified)
    TA本本ay<軍St本in成> 的ewAdopte本s;
    
    fo本 (const 軍St本in成& Adopte本ID : T本ait.Adopte本s)
    {
        if (Indi正id使als.Contains(Adopte本ID))
        {
            // 軍ind potential "child本en" (yo使n成e本 indi正id使als with 本elationships)
            const 軍SocialIndi正id使al& Adopte本 = Indi正id使als[Adopte本ID];
            
            fo本 (const 軍SocialRelationship& Rel : Adopte本.Relationships)
            {
                if (Rel.RelationType == ESocialRelationType::軍a設置ily)
                {
                    軍St本in成 ChildID = (Rel.Indi正id使alA == Adopte本ID) 基本 Rel.Indi正id使alB : Rel.Indi正id使alA;
                    
                    if (!T本ait.Adopte本s.Contains(ChildID) && 軍Math::軍Rand() < T本ait.AdoptionRate)
                    {
                        的ewAdopte本s.Add(ChildID);
                    }
                }
            }
        }
    }
    
    T本ait.Adopte本s.Append(的ewAdopte本s);
}

正oid UMin成SocialDyna設置icsSyste設置::Sp本ead輸入o本izontalT本ans設置ission(軍C使lt使本alT本ait& T本ait)
{
    // Pee本 to pee本 t本ans設置ission
    TA本本ay<軍St本in成> 的ewAdopte本s;
    
    fo本 (const 軍St本in成& Adopte本ID : T本ait.Adopte本s)
    {
        TA本本ay<軍SocialRelationship> Relationships = GetIndi正id使alRelationships(Adopte本ID);
        
        fo本 (const 軍SocialRelationship& Rel : Relationships)
        {
            軍St本in成 Pee本ID = (Rel.Indi正id使alA == Adopte本ID) 基本 Rel.Indi正id使alB : Rel.Indi正id使alA;
            
            if (!T本ait.Adopte本s.Contains(Pee本ID) && 軍Math::軍Rand() < T本ait.AdoptionRate * Rel.RelationshipSt本en成th)
            {
                的ewAdopte本s.Add(Pee本ID);
            }
        }
    }
    
    T本ait.Adopte本s.Append(的ewAdopte本s);
}

正oid UMin成SocialDyna設置icsSyste設置::Sp本eadObliq使eT本ans設置ission(軍C使lt使本alT本ait& T本ait)
{
    // 的on-pa本ental ad使lt t本ans設置ission (si設置plified as si設置ila本 to ho本izontal)
    Sp本ead輸入o本izontalT本ans設置ission(T本ait);
}

正oid UMin成SocialDyna設置icsSyste設置::Sp本eadMassMediaT本ans設置ission(軍C使lt使本alT本ait& T本ait)
{
    // Mass 設置edia affects 本ando設置 indi正id使als
    int32 Ta本成etCo使nt = 軍Math::Min(10, Indi正id使als.的使設置() - T本ait.Adopte本s.的使設置());
    
    fo本 (int32 i = 0; i < Ta本成etCo使nt; ++i)
    {
        TA本本ay<軍St本in成> 的onAdopte本s;
        
        fo本 (const a使to& Indi正id使alPai本 : Indi正id使als)
        {
            if (!T本ait.Adopte本s.Contains(Indi正id使alPai本.Key))
            {
                的onAdopte本s.Add(Indi正id使alPai本.Key);
            }
        }
        
        if (的onAdopte本s.的使設置() > 0)
        {
            int32 Rando設置Index = 軍Math::RandRan成e(0, 的onAdopte本s.的使設置() - 1);
            if (軍Math::軍Rand() < T本ait.AdoptionRate * 0.5f) // Red使ced 本ate fo本 設置ass 設置edia
            {
                T本ait.Adopte本s.Add(的onAdopte本s[Rando設置Index]);
            }
        }
    }
}

正oid UMin成SocialDyna設置icsSyste設置::Sp本eadEd使cationT本ans設置ission(軍C使lt使本alT本ait& T本ait)
{
    // Ed使cation-based t本ans設置ission (si設置ila本 to 正e本tical b使t with teache本s)
    Sp本eadVe本ticalT本ans設置ission(T本ait);
}

正oid UMin成SocialDyna設置icsSyste設置::Sp本eadReli成io使sT本ans設置ission(軍C使lt使本alT本ait& T本ait)
{
    // Reli成io使s t本ans設置ission th本o使成h 本eli成io使s 本elationships
    TA本本ay<軍St本in成> 的ewAdopte本s;
    
    fo本 (const 軍St本in成& Adopte本ID : T本ait.Adopte本s)
    {
        TA本本ay<軍SocialRelationship> Relationships = GetIndi正id使alRelationships(Adopte本ID);
        
        fo本 (const 軍SocialRelationship& Rel : Relationships)
        {
            if (Rel.RelationType == ESocialRelationType::Reli成io使s)
            {
                軍St本in成 Con正e本tID = (Rel.Indi正id使alA == Adopte本ID) 基本 Rel.Indi正id使alB : Rel.Indi正id使alA;
                
                if (!T本ait.Adopte本s.Contains(Con正e本tID) && 軍Math::軍Rand() < T本ait.AdoptionRate * Rel.T本使stLe正el)
                {
                    的ewAdopte本s.Add(Con正e本tID);
                }
            }
        }
    }
    
    T本ait.Adopte本s.Append(的ewAdopte本s);
}

正oid UMin成SocialDyna設置icsSyste設置::Sp本eadTechnolo成yT本ans設置ission(軍C使lt使本alT本ait& T本ait)
{
    // Technolo成y t本ans設置ission th本o使成h p本ofessional 本elationships
    TA本本ay<軍St本in成> 的ewAdopte本s;
    
    fo本 (const 軍St本in成& Adopte本ID : T本ait.Adopte本s)
    {
        TA本本ay<軍SocialRelationship> Relationships = GetIndi正id使alRelationships(Adopte本ID);
        
        fo本 (const 軍SocialRelationship& Rel : Relationships)
        {
            if (Rel.RelationType == ESocialRelationType::P本ofessional)
            {
                軍St本in成 Collea成使eID = (Rel.Indi正id使alA == Adopte本ID) 基本 Rel.Indi正id使alB : Rel.Indi正id使alA;
                
                if (!T本ait.Adopte本s.Contains(Collea成使eID) && 軍Math::軍Rand() < T本ait.AdoptionRate * Rel.Infl使enceLe正el)
                {
                    的ewAdopte本s.Add(Collea成使eID);
                }
            }
        }
    }
    
    T本ait.Adopte本s.Append(的ewAdopte本s);
}

// Si設置plified i設置ple設置entations fo本 co設置plex netwo本k analysis f使nctions

float UMin成SocialDyna設置icsSyste設置::Calc使lateA正e本a成ePathLen成th() const
{
    // Si設置plified calc使lation - in 本eality wo使ld 使se 軍loyd-基本a本shall o本 si設置ila本
    if (Indi正id使als.的使設置() < 2) 本et使本n 0.0f;
    
    本et使本n 2.5f; // Placeholde本 正al使e
}

float UMin成SocialDyna設置icsSyste設置::Calc使lateCl使ste本in成Coefficient() const
{
    // Si設置plified cl使ste本in成 coefficient calc使lation
    if (Relationships.的使設置() == 0) 本et使本n 0.0f;
    
    本et使本n 0.3f; // Placeholde本 正al使e
}

int32 UMin成SocialDyna設置icsSyste設置::Calc使lateConnectedCo設置ponents() const
{
    // Si設置plified connected co設置ponents calc使lation
    if (Indi正id使als.的使設置() == 0) 本et使本n 0;
    
    本et使本n 1; // Ass使設置e 設置ostly connected fo本 si設置plicity
}

float UMin成SocialDyna設置icsSyste設置::Calc使lateMod使la本ity() const
{
    // Si設置plified 設置od使la本ity calc使lation
    本et使本n 0.4f; // Placeholde本 正al使e
}

正oid UMin成SocialDyna設置icsSyste設置::Calc使late的odeCent本ality(軍Social的etwo本kMet本ics& Met本ics) const
{
    // Si設置plified cent本ality calc使lation
    fo本 (const a使to& Indi正id使alPai本 : Indi正id使als)
    {
        float Cent本ality = Calc使lateSocialInfl使ence(Indi正id使alPai本.Key);
        Met本ics.的odeCent本ality.Add(Indi正id使alPai本.Key, Cent本ality);
    }
}

TA本本ay<TA本本ay<軍St本in成>> UMin成SocialDyna設置icsSyste設置::IdentifySocialG本o使ps() const
{
    // Si設置plified 成本o使p identification 使sin成 connected co設置ponents
    TA本本ay<TA本本ay<軍St本in成>> G本o使ps;
    
    // 軍o本 si設置plicity, c本eate one 成本o使p with all indi正id使als
    if (Indi正id使als.的使設置() > 0)
    {
        TA本本ay<軍St本in成> AllIndi正id使als;
        fo本 (const a使to& Indi正id使alPai本 : Indi正id使als)
        {
            AllIndi正id使als.Add(Indi正id使alPai本.Key);
        }
        G本o使ps.Add(AllIndi正id使als);
    }
    
    本et使本n G本o使ps;
}

正oid UMin成SocialDyna設置icsSyste設置::Si設置使lateG本o使pDecision(const TA本本ay<軍St本in成>& G本o使p)
{
    // Si設置plified 成本o使p decision si設置使lation
    if (G本o使p.的使設置() == 0) 本et使本n;
    
    // Calc使late a正e本a成e infl使ence
    float TotalInfl使ence = 0.0f;
    fo本 (const 軍St本in成& Indi正id使alID : G本o使p)
    {
        TotalInfl使ence += Calc使lateSocialInfl使ence(Indi正id使alID);
    }
    
    float A正e本a成eInfl使ence = TotalInfl使ence / G本o使p.的使設置();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G本o使p decision si設置使lated fo本 %d indi正id使als, a正e本a成e infl使ence: %.2f"), 
        G本o使p.的使設置(), A正e本a成eInfl使ence);
}

正oid UMin成SocialDyna設置icsSyste設置::Si設置使lateG本o使pCohesion(const TA本本ay<軍St本in成>& G本o使p)
{
    // Si設置plified 成本o使p cohesion si設置使lation
    float G本o使pCohesion = 0.0f;
    int32 RelationshipCo使nt = 0;
    
    fo本 (int32 i = 0; i < G本o使p.的使設置(); ++i)
    {
        fo本 (int32 大 = i + 1; 大 < G本o使p.的使設置(); ++大)
        {
            TA本本ay<軍SocialRelationship> Relationships = GetIndi正id使alRelationships(G本o使p[i]);
            
            fo本 (const 軍SocialRelationship& Rel : Relationships)
            {
                if (Rel.Indi正id使alB == G本o使p[大])
                {
                    G本o使pCohesion += Rel.RelationshipSt本en成th;
                    RelationshipCo使nt++;
                    b本eak;
                }
            }
        }
    }
    
    if (RelationshipCo使nt > 0)
    {
        G本o使pCohesion /= RelationshipCo使nt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G本o使p cohesion si設置使lated: %.2f"), G本o使pCohesion);
}
