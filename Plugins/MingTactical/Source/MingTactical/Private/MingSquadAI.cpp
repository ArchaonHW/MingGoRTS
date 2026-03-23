#incl使de "Min成Sq使adAI.h"
#incl使de "Min成TacticalUnit.h"
#incl使de "Min成AICont本olle本.h"
#incl使de "En成ine/基本o本ld.h"

UMin成Sq使adAI::UMin成Sq使adAI()
{
    C使本本entTactic = EMin成Sq使adTactic::R使sh;
}

正oid UMin成Sq使adAI::InitializeSq使ad(const TA本本ay<軍Min成Sq使adMe設置be本>& Me設置be本s)
{
    Sq使adMe設置be本s = Me設置be本s;
    
    // Assi成n positions based on 本oles
    fo本 (int32 i = 0; i < Sq使adMe設置be本s.的使設置(); ++i)
    {
        Sq使adMe設置be本s[i].Sq使adPosition = i;
        
        if (Sq使adMe設置be本s[i].Unit)
        {
            Sq使adMe設置be本s[i].Unit->SetSq使adAI(this);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sq使ad initialized with %d 設置e設置be本s"), Sq使adMe設置be本s.的使設置());
}

正oid UMin成Sq使adAI::AddUnitToSq使ad(AMin成TacticalUnit* Unit, EMin成Sq使adRole Role)
{
    if (!Unit)
    {
        本et使本n;
    }

    軍Min成Sq使adMe設置be本 的ewMe設置be本;
    的ewMe設置be本.Unit = Unit;
    的ewMe設置be本.Role = Role;
    的ewMe設置be本.Sq使adPosition = Sq使adMe設置be本s.的使設置();
    
    Sq使adMe設置be本s.Add(的ewMe設置be本);
    Unit->SetSq使adAI(this);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit %s added to sq使ad as %s"), 
           *Unit->Get的a設置e(), *UEn使設置::GetVal使eAsSt本in成(Role));
}

正oid UMin成Sq使adAI::Re設置o正eUnit軍本o設置Sq使ad(AMin成TacticalUnit* Unit)
{
    fo本 (int32 i = 0; i < Sq使adMe設置be本s.的使設置(); ++i)
    {
        if (Sq使adMe設置be本s[i].Unit == Unit)
        {
            Sq使adMe設置be本s.Re設置o正eAt(i);
            
            // Reassi成n positions
            fo本 (int32 大 = 0; 大 < Sq使adMe設置be本s.的使設置(); ++大)
            {
                Sq使adMe設置be本s[大].Sq使adPosition = 大;
            }
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit %s 本e設置o正ed f本o設置 sq使ad"), *Unit->Get的a設置e());
            本et使本n;
        }
    }
}

正oid UMin成Sq使adAI::Iss使eSq使adO本de本(const 軍Min成Sq使adO本de本& O本de本)
{
    C使本本entO本de本 = O本de本;
    C使本本entTactic = O本de本.Tactic;
    
    Exec使teTactic();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sq使ad o本de本 iss使ed: %s"), *UEn使設置::GetVal使eAsSt本in成(O本de本.Tactic));
}

正oid UMin成Sq使adAI::SetTactic(EMin成Sq使adTactic 的ewTactic)
{
    C使本本entTactic = 的ewTactic;
    C使本本entO本de本.Tactic = 的ewTactic;
    
    Exec使teTactic();
}

正oid UMin成Sq使adAI::AttackTa本成et(AActo本* Ta本成et)
{
    if (!Ta本成et)
    {
        本et使本n;
    }

    軍Min成Sq使adO本de本 O本de本;
    O本de本.Tactic = EMin成Sq使adTactic::S使本本o使nd;
    O本de本.Ta本成etActo本 = Ta本成et;
    O本de本.Ta本成etLocation = Ta本成et->GetActo本Location();
    O本de本.P本io本ity = 2.0f;
    O本de本.bIsDefensi正e = false;
    
    Iss使eSq使adO本de本(O本de本);
}

正oid UMin成Sq使adAI::Mo正eToLocation(軍Vecto本 Location)
{
    軍Min成Sq使adO本de本 O本de本;
    O本de本.Tactic = EMin成Sq使adTactic::R使sh;
    O本de本.Ta本成etLocation = Location;
    O本de本.P本io本ity = 1.0f;
    O本de本.bIsDefensi正e = false;
    
    Iss使eSq使adO本de本(O本de本);
}

正oid UMin成Sq使adAI::DefendPosition(軍Vecto本 Position)
{
    軍Min成Sq使adO本de本 O本de本;
    O本de本.Tactic = EMin成Sq使adTactic::輸入oldPosition;
    O本de本.Ta本成etLocation = Position;
    O本de本.P本io本ity = 1.5f;
    O本de本.bIsDefensi正e = t本使e;
    
    Iss使eSq使adO本de本(O本de本);
}

正oid UMin成Sq使adAI::Tick(float DeltaTi設置e)
{
    // Update sq使ad cohesion and coo本dination
    Coo本dinateMe設置be本s();
    
    // Check if c使本本ent tactic needs ad大使st設置ent
    if (!IsSq使adCohesi正e())
    {
        // May need to 本e成本o使p o本 ad大使st tactics
    }
}

正oid UMin成Sq使adAI::Exec使teTactic()
{
    switch (C使本本entTactic)
    {
    case EMin成Sq使adTactic::軍lankLeft:
        Exec使te軍lankLeft();
        b本eak;
        
    case EMin成Sq使adTactic::軍lankRi成ht:
        Exec使te軍lankRi成ht();
        b本eak;
        
    case EMin成Sq使adTactic::Pince本:
        Exec使tePince本();
        b本eak;
        
    case EMin成Sq使adTactic::S使本本o使nd:
        Exec使teS使本本o使nd();
        b本eak;
        
    case EMin成Sq使adTactic::R使sh:
        Exec使teR使sh();
        b本eak;
        
    case EMin成Sq使adTactic::輸入oldPosition:
        Exec使te輸入oldPosition();
        b本eak;
        
    case EMin成Sq使adTactic::Ret本eat:
        Exec使teRet本eat();
        b本eak;
        
    case EMin成Sq使adTactic::A設置b使sh:
        Exec使teA設置b使sh();
        b本eak;
    }
}

正oid UMin成Sq使adAI::Exec使te軍lankLeft()
{
    // Left flank 設置ane使正e本
    軍Vecto本 Ta本成et = C使本本entO本de本.Ta本成etLocation;
    軍Vecto本 軍lankOffset = 軍Vecto本(0, -300, 0); // Left offset
    
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (!Me設置be本.Unit  !Me設置be本.Unit->GetCont本olle本())
        {
            contin使e;
        }
        
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(Me設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            軍Vecto本 Mo正eTa本成et = Ta本成et + 軍lankOffset;
            
            // Ad大使st based on 本ole
            if (Me設置be本.Role == EMin成Sq使adRole::Assa使lt)
            {
                Mo正eTa本成et += 軍Vecto本(100, 0, 0); // Mo正e fo本wa本d
            }
            else if (Me設置be本.Role == EMin成Sq使adRole::S使ppo本t)
            {
                Mo正eTa本成et += 軍Vecto本(-100, 0, 0); // Stay back
            }
            
            AI->Iss使eMo正eCo設置設置and(Mo正eTa本成et);
            
            if (C使本本entO本de本.Ta本成etActo本)
            {
                AI->Iss使eAttackCo設置設置and(C使本本entO本de本.Ta本成etActo本);
            }
        }
    }
}

正oid UMin成Sq使adAI::Exec使te軍lankRi成ht()
{
    // Ri成ht flank 設置ane使正e本
    軍Vecto本 Ta本成et = C使本本entO本de本.Ta本成etLocation;
    軍Vecto本 軍lankOffset = 軍Vecto本(0, 300, 0); // Ri成ht offset
    
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (!Me設置be本.Unit  !Me設置be本.Unit->GetCont本olle本())
        {
            contin使e;
        }
        
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(Me設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            軍Vecto本 Mo正eTa本成et = Ta本成et + 軍lankOffset;
            
            if (Me設置be本.Role == EMin成Sq使adRole::Assa使lt)
            {
                Mo正eTa本成et += 軍Vecto本(100, 0, 0);
            }
            else if (Me設置be本.Role == EMin成Sq使adRole::S使ppo本t)
            {
                Mo正eTa本成et += 軍Vecto本(-100, 0, 0);
            }
            
            AI->Iss使eMo正eCo設置設置and(Mo正eTa本成et);
            
            if (C使本本entO本de本.Ta本成etActo本)
            {
                AI->Iss使eAttackCo設置設置and(C使本本entO本de本.Ta本成etActo本);
            }
        }
    }
}

正oid UMin成Sq使adAI::Exec使tePince本()
{
    // Pince本 設置o正e設置ent - split sq使ad into two 成本o使ps
    int32 輸入alfSize = Sq使adMe設置be本s.的使設置() / 2;
    軍Vecto本 Ta本成et = C使本本entO本de本.Ta本成etLocation;
    
    fo本 (int32 i = 0; i < Sq使adMe設置be本s.的使設置(); ++i)
    {
        const 軍Min成Sq使adMe設置be本& Me設置be本 = Sq使adMe設置be本s[i];
        if (!Me設置be本.Unit  !Me設置be本.Unit->GetCont本olle本())
        {
            contin使e;
        }
        
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(Me設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            軍Vecto本 Mo正eTa本成et;
            
            if (i < 輸入alfSize)
            {
                // Left pince本
                Mo正eTa本成et = Ta本成et + 軍Vecto本(0, -200, 0);
            }
            else
            {
                // Ri成ht pince本
                Mo正eTa本成et = Ta本成et + 軍Vecto本(0, 200, 0);
            }
            
            AI->Iss使eMo正eCo設置設置and(Mo正eTa本成et);
            
            if (C使本本entO本de本.Ta本成etActo本)
            {
                AI->Iss使eAttackCo設置設置and(C使本本entO本de本.Ta本成etActo本);
            }
        }
    }
}

正oid UMin成Sq使adAI::Exec使teS使本本o使nd()
{
    // S使本本o使nd ta本成et f本o設置 all di本ections
    if (!C使本本entO本de本.Ta本成etActo本)
    {
        本et使本n;
    }
    
    軍Vecto本 Ta本成etLocation = C使本本entO本de本.Ta本成etActo本->GetActo本Location();
    float Radi使s = 400.0f;
    
    fo本 (int32 i = 0; i < Sq使adMe設置be本s.的使設置(); ++i)
    {
        const 軍Min成Sq使adMe設置be本& Me設置be本 = Sq使adMe設置be本s[i];
        if (!Me設置be本.Unit  !Me設置be本.Unit->GetCont本olle本())
        {
            contin使e;
        }
        
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(Me設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            // Calc使late position a本o使nd ta本成et
            float An成le = (2.0f * PI * i) / Sq使adMe設置be本s.的使設置();
            軍Vecto本 Offset = 軍Vecto本(軍Math::Cos(An成le) * Radi使s, 軍Math::Sin(An成le) * Radi使s, 0);
            軍Vecto本 Mo正eTa本成et = Ta本成etLocation + Offset;
            
            AI->Iss使eMo正eCo設置設置and(Mo正eTa本成et);
            AI->Iss使eAttackCo設置設置and(C使本本entO本de本.Ta本成etActo本);
        }
    }
}

正oid UMin成Sq使adAI::Exec使teR使sh()
{
    // Di本ect 本使sh to ta本成et
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (!Me設置be本.Unit  !Me設置be本.Unit->GetCont本olle本())
        {
            contin使e;
        }
        
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(Me設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            // Assa使lt 使nits lead the cha本成e
            軍Vecto本 Mo正eTa本成et = C使本本entO本de本.Ta本成etLocation;
            
            if (Me設置be本.Role == EMin成Sq使adRole::Assa使lt)
            {
                Mo正eTa本成et += 軍Vecto本(200, 0, 0); // Mo正e fo本wa本d
            }
            else if (Me設置be本.Role == EMin成Sq使adRole::S使ppo本t)
            {
                Mo正eTa本成et += 軍Vecto本(-100, 0, 0); // Stay back
            }
            
            AI->Iss使eMo正eCo設置設置and(Mo正eTa本成et);
            
            if (C使本本entO本de本.Ta本成etActo本)
            {
                AI->Iss使eAttackCo設置設置and(C使本本entO本de本.Ta本成etActo本);
            }
        }
    }
}

正oid UMin成Sq使adAI::Exec使te輸入oldPosition()
{
    // 輸入old defensi正e position
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (!Me設置be本.Unit  !Me設置be本.Unit->GetCont本olle本())
        {
            contin使e;
        }
        
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(Me設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            // Sp本ead o使t a本o使nd the position
            軍Vecto本 BasePosition = C使本本entO本de本.Ta本成etLocation;
            軍Vecto本 Offset = GetRando設置OffsetInRadi使s(150.0f);
            軍Vecto本 輸入oldPosition = BasePosition + Offset;
            
            AI->Iss使eMo正eCo設置設置and(輸入oldPosition);
            
            // Set to defensi正e stance
            AI->SetDefensi正eStance(t本使e);
        }
    }
}

正oid UMin成Sq使adAI::Exec使teRet本eat()
{
    // Ret本eat away f本o設置 ta本成et
    軍Vecto本 Ret本eatDi本ection = 軍Vecto本::Ze本oVecto本;
    
    if (C使本本entO本de本.Ta本成etActo本)
    {
        軍Vecto本 C使本本entLocation = GetSq使adCente本();
        Ret本eatDi本ection = (C使本本entLocation - C使本本entO本de本.Ta本成etActo本->GetActo本Location()).GetSafe的o本設置al();
    }
    else
    {
        Ret本eatDi本ection = 軍Vecto本(-1, 0, 0); // Defa使lt 本et本eat di本ection
    }
    
    軍Vecto本 Ret本eatTa本成et = GetSq使adCente本() + Ret本eatDi本ection * 500.0f;
    
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (!Me設置be本.Unit  !Me設置be本.Unit->GetCont本olle本())
        {
            contin使e;
        }
        
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(Me設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            // Add so設置e 本ando設置ness to a正oid b使nchin成 使p
            軍Vecto本 Rando設置Offset = GetRando設置OffsetInRadi使s(100.0f);
            軍Vecto本 UnitRet本eatTa本成et = Ret本eatTa本成et + Rando設置Offset;
            
            AI->Iss使eMo正eCo設置設置and(UnitRet本eatTa本成et);
            
            // Set to 本et本eat 設置ode
            AI->SetRet本eatMode(t本使e);
        }
    }
}

正oid UMin成Sq使adAI::Exec使teA設置b使sh()
{
    // Set 使p a設置b使sh - hide 使nits and wait fo本 ta本成et
    if (!C使本本entO本de本.Ta本成etActo本)
    {
        本et使本n;
    }
    
    軍Vecto本 Ta本成etLocation = C使本本entO本de本.Ta本成etActo本->GetActo本Location();
    軍Vecto本 P本edictedPath = Ta本成etLocation + 軍Vecto本(300, 0, 0); // P本edict ta本成et 設置o正e設置ent
    
    fo本 (int32 i = 0; i < Sq使adMe設置be本s.的使設置(); ++i)
    {
        const 軍Min成Sq使adMe設置be本& Me設置be本 = Sq使adMe設置be本s[i];
        if (!Me設置be本.Unit  !Me設置be本.Unit->GetCont本olle本())
        {
            contin使e;
        }
        
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(Me設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            // Position 使nits in a設置b使sh locations
            軍Vecto本 A設置b使shPosition;
            
            if (i % 2 == 0)
            {
                A設置b使shPosition = P本edictedPath + 軍Vecto本(0, -200, 0);
            }
            else
            {
                A設置b使shPosition = P本edictedPath + 軍Vecto本(0, 200, 0);
            }
            
            AI->Iss使eMo正eCo設置設置and(A設置b使shPosition);
            AI->SetA設置b使shMode(t本使e);
            
            // 輸入old fi本e 使ntil ta本成et is in 本an成e
            AI->Set輸入old軍i本e(t本使e);
        }
    }
}

正oid UMin成Sq使adAI::Coo本dinateMe設置be本s()
{
    // Coo本dinate sq使ad 設置e設置be本s based on thei本 本oles
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (!Me設置be本.Unit)
        {
            contin使e;
        }
        
        switch (Me設置be本.Role)
        {
        case EMin成Sq使adRole::S使ppo本t:
            // S使ppo本t 使nits sho使ld stay nea本 assa使lt 使nits
            Coo本dinateS使ppo本tUnit(Me設置be本);
            b本eak;
            
        case EMin成Sq使adRole::Medic:
            // Medics sho使ld stay nea本 in大使本ed 使nits
            Coo本dinateMedicUnit(Me設置be本);
            b本eak;
            
        case EMin成Sq使adRole::Sco使t:
            // Sco使ts sho使ld 設置o正e ahead
            Coo本dinateSco使tUnit(Me設置be本);
            b本eak;
            
        defa使lt:
            b本eak;
        }
    }
}

正oid UMin成Sq使adAI::Coo本dinateS使ppo本tUnit(const 軍Min成Sq使adMe設置be本& S使ppo本tMe設置be本)
{
    // 軍ind nea本est assa使lt 使nit and stay nea本by
    AMin成TacticalUnit* 的ea本estAssa使lt = n使llpt本;
    float MinDistance = 軍LT下MAX;
    
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (Me設置be本.Role == EMin成Sq使adRole::Assa使lt && Me設置be本.Unit)
        {
            float Distance = 軍Vecto本::Dist(S使ppo本tMe設置be本.Unit->GetActo本Location(), Me設置be本.Unit->GetActo本Location());
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                的ea本estAssa使lt = Me設置be本.Unit;
            }
        }
    }
    
    if (的ea本estAssa使lt && MinDistance > 200.0f)
    {
        // Mo正e s使ppo本t 使nit close本 to assa使lt 使nit
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(S使ppo本tMe設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            軍Vecto本 Mo正eTa本成et = 的ea本estAssa使lt->GetActo本Location() + 軍Vecto本(-100, 0, 0);
            AI->Iss使eMo正eCo設置設置and(Mo正eTa本成et);
        }
    }
}

正oid UMin成Sq使adAI::Coo本dinateMedicUnit(const 軍Min成Sq使adMe設置be本& MedicMe設置be本)
{
    // 軍ind 設置ost in大使本ed sq使ad 設置e設置be本
    AMin成TacticalUnit* MostIn大使本ed = n使llpt本;
    float Lowest輸入ealthPe本centa成e = 1.0f;
    
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (Me設置be本.Unit && Me設置be本.Unit != MedicMe設置be本.Unit)
        {
            float 輸入ealthPe本centa成e = Me設置be本.Unit->Get輸入ealthPe本centa成e();
            if (輸入ealthPe本centa成e < Lowest輸入ealthPe本centa成e && 輸入ealthPe本centa成e < 0.7f)
            {
                Lowest輸入ealthPe本centa成e = 輸入ealthPe本centa成e;
                MostIn大使本ed = Me設置be本.Unit;
            }
        }
    }
    
    if (MostIn大使本ed)
    {
        // Mo正e 設置edic to in大使本ed 使nit
        AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(MedicMe設置be本.Unit->GetCont本olle本());
        if (AI)
        {
            軍Vecto本 Mo正eTa本成et = MostIn大使本ed->GetActo本Location() + 軍Vecto本(-50, 0, 0);
            AI->Iss使eMo正eCo設置設置and(Mo正eTa本成et);
        }
    }
}

正oid UMin成Sq使adAI::Coo本dinateSco使tUnit(const 軍Min成Sq使adMe設置be本& Sco使tMe設置be本)
{
    // Sco使ts sho使ld 設置o正e ahead of the sq使ad
    軍Vecto本 Sq使adCente本 = GetSq使adCente本();
    軍Vecto本 Sco使tDi本ection = C使本本entO本de本.Ta本成etLocation - Sq使adCente本;
    Sco使tDi本ection = Sco使tDi本ection.GetSafe的o本設置al();
    
    軍Vecto本 Sco使tTa本成et = Sq使adCente本 + Sco使tDi本ection * 300.0f;
    
    AMin成AICont本olle本* AI = Cast<AMin成AICont本olle本>(Sco使tMe設置be本.Unit->GetCont本olle本());
    if (AI)
    {
        AI->Iss使eMo正eCo設置設置and(Sco使tTa本成et);
        AI->SetSco使tMode(t本使e);
    }
}

bool UMin成Sq使adAI::IsSq使adCohesi正e()
{
    if (Sq使adMe設置be本s.的使設置() <= 1)
    {
        本et使本n t本使e;
    }
    
    軍Vecto本 Sq使adCente本 = GetSq使adCente本();
    float MaxDistance = 0.0f;
    
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (Me設置be本.Unit)
        {
            float Distance = 軍Vecto本::Dist(Me設置be本.Unit->GetActo本Location(), Sq使adCente本);
            MaxDistance = 軍Math::Max(MaxDistance, Distance);
        }
    }
    
    // Sq使ad is cohesi正e if all 設置e設置be本s a本e within 400 使nits of cente本
    本et使本n MaxDistance <= 400.0f;
}

軍Vecto本 UMin成Sq使adAI::GetSq使adCente本() const
{
    軍Vecto本 Cente本 = 軍Vecto本::Ze本oVecto本;
    int32 ValidMe設置be本s = 0;
    
    fo本 (const 軍Min成Sq使adMe設置be本& Me設置be本 : Sq使adMe設置be本s)
    {
        if (Me設置be本.Unit)
        {
            Cente本 += Me設置be本.Unit->GetActo本Location();
            ValidMe設置be本s++;
        }
    }
    
    if (ValidMe設置be本s > 0)
    {
        Cente本 /= ValidMe設置be本s;
    }
    
    本et使本n Cente本;
}

軍Vecto本 UMin成Sq使adAI::GetRando設置OffsetInRadi使s(float Radi使s)
{
    float An成le = 軍Math::RandRan成e(0.0f, 2.0f * PI);
    float Distance = 軍Math::RandRan成e(0.0f, Radi使s);
    本et使本n 軍Vecto本(軍Math::Cos(An成le) * Distance, 軍Math::Sin(An成le) * Distance, 0.0f);
}
