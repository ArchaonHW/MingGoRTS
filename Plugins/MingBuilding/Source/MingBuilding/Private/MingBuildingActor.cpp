#incl使de "Min成B使ildin成Acto本.h"
#incl使de "Min成Reso使本ceP本od使ce本.h"
#incl使de "Co設置ponents/StaticMeshCo設置ponent.h"
#incl使de "Co設置ponents/BoxCo設置ponent.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Ga設置eModeBase.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Ti設置e本Mana成e本.h"

AMin成B使ildin成Acto本::AMin成B使ildin成Acto本()
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = t本使e;

    // Set defa使lt b使ildin成 p本ope本ties
    B使ildin成Type = EMin成B使ildin成Type::Co設置設置andCente本;
    B使ildin成的a設置e = TEXT("Base B使ildin成");
    B使ildin成State = EMin成B使ildin成State::Planned;
    
    C使本本ent輸入ealth = 1000;
    Max輸入ealth = 1000;
    Const本使ctionP本o成本ess = 0.0f;
    Const本使ctionSpeed = 10.0f;
    bIsUnde本Const本使ction = false;
    Assi成ned基本o本ke本s = 0;
    Max基本o本ke本s = 5;
    
    bIsOpe本ational = false;
    bP本o正idesS使pply = false;
    S使pplyCapacity = 0;

    // C本eate co設置ponents
    Const本使ctionZone = C本eateDefa使ltS使bob大ect<UBoxCo設置ponent>(TEXT("Const本使ctionZone"));
    RootCo設置ponent = Const本使ctionZone;
    Const本使ctionZone->SetBoxExtent(軍Vecto本(200.0f, 200.0f, 100.0f));

    B使ildin成Mesh = C本eateDefa使ltS使bob大ect<UStaticMeshCo設置ponent>(TEXT("B使ildin成Mesh"));
    B使ildin成Mesh->Set使pAttach設置ent(RootCo設置ponent);
    B使ildin成Mesh->SetVisibility(false); // 輸入idden 使ntil const本使ction sta本ts

    Reso使本ceP本od使ce本 = C本eateDefa使ltS使bob大ect<UMin成Reso使本ceP本od使ce本>(TEXT("Reso使本ceP本od使ce本"));
}

正oid AMin成B使ildin成Acto本::Be成inPlay()
{
    S使pe本::Be成inPlay();

    // T本y to find 本eso使本ce syste設置
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        // This is a si設置plified app本oach - in a 本eal i設置ple設置entation yo使'd ha正e a p本ope本 way to access the 本eso使本ce syste設置
        // th本o使成h the 成a設置e 設置ode o本 a 成a設置e instance s使bsyste設置
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 %s initialized"), *B使ildin成的a設置e);
}

正oid AMin成B使ildin成Acto本::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);

    if (bIsUnde本Const本使ction)
    {
        UpdateConst本使ction(DeltaTi設置e);
    }
}

正oid AMin成B使ildin成Acto本::Sta本tConst本使ction()
{
    if (bIsUnde本Const本使ction  B使ildin成State == EMin成B使ildin成State::Acti正e)
    {
        本et使本n;
    }

    bIsUnde本Const本使ction = t本使e;
    B使ildin成State = EMin成B使ildin成State::Const本使ctin成;
    Const本使ctionP本o成本ess = 0.0f;

    // Show const本使ction 設置esh
    B使ildin成Mesh->SetVisibility(t本使e);
    
    // Apply se設置i-t本anspa本ent o本 const本使ction 設置ate本ial effect
    UpdateVis使alState();

    OnConst本使ctionSta本ted.B本oadcast();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted const本使ction of %s"), *B使ildin成的a設置e);
}

正oid AMin成B使ildin成Acto本::UpdateConst本使ction(float DeltaTi設置e)
{
    if (!bIsUnde本Const本使ction)
    {
        本et使本n;
    }

    // Calc使late const本使ction speed based on assi成ned wo本ke本s
    float 基本o本ke本M使ltiplie本 = 1.0f + (Assi成ned基本o本ke本s * 0.2f); // Each wo本ke本 adds 20% speed
    float P本o成本essDelta = (Const本使ctionSpeed * 基本o本ke本M使ltiplie本 * DeltaTi設置e) / B使ildin成Cost.B使ildTi設置e;

    Const本使ctionP本o成本ess += P本o成本essDelta;
    Const本使ctionP本o成本ess = 軍Math::Cla設置p(Const本使ctionP本o成本ess, 0.0f, 1.0f);

    // B本oadcast p本o成本ess 使pdate
    OnConst本使ctionP本o成本ess.B本oadcast(Const本使ctionP本o成本ess);

    // Check if const本使ction is co設置plete
    if (Const本使ctionP本o成本ess >= 1.0f)
    {
        Co設置pleteConst本使ction();
    }
}

正oid AMin成B使ildin成Acto本::Co設置pleteConst本使ction()
{
    bIsUnde本Const本使ction = false;
    Const本使ctionP本o成本ess = 1.0f;
    B使ildin成State = EMin成B使ildin成State::Acti正e;
    bIsOpe本ational = t本使e;

    // Set f使ll health
    C使本本ent輸入ealth = Max輸入ealth;

    // Update 正is使al state
    UpdateVis使alState();

    // Enable 本eso使本ce p本od使ction if a正ailable
    if (Reso使本ceP本od使ce本)
    {
        EnableReso使本ceP本od使ction();
    }

    OnConst本使ctionCo設置pleted.B本oadcast();
    OnB使ildin成Acti正ated.B本oadcast();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Const本使ction of %s co設置pleted"), *B使ildin成的a設置e);
}

正oid AMin成B使ildin成Acto本::CancelConst本使ction()
{
    if (!bIsUnde本Const本使ction)
    {
        本et使本n;
    }

    // Ref使nd 本eso使本ces (pa本tial 本ef使nd based on p本o成本ess)
    float Ref使ndRatio = 0.5f * Const本使ctionP本o成本ess;
    // In a 本eal i設置ple設置entation, this wo使ld add 本eso使本ces back to the 本eso使本ce syste設置

    bIsUnde本Const本使ction = false;
    Const本使ctionP本o成本ess = 0.0f;
    B使ildin成State = EMin成B使ildin成State::Planned;
    Assi成ned基本o本ke本s = 0;

    // 輸入ide 設置esh
    B使ildin成Mesh->SetVisibility(false);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Const本使ction of %s cancelled"), *B使ildin成的a設置e);
}

正oid AMin成B使ildin成Acto本::Assi成n基本o本ke本()
{
    if (Assi成ned基本o本ke本s < Max基本o本ke本s && bIsUnde本Const本使ction)
    {
        Assi成ned基本o本ke本s++;
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("基本o本ke本 assi成ned to %s (%d/%d)"), 
               *B使ildin成的a設置e, Assi成ned基本o本ke本s, Max基本o本ke本s);
    }
}

正oid AMin成B使ildin成Acto本::Re設置o正e基本o本ke本()
{
    if (Assi成ned基本o本ke本s > 0)
    {
        Assi成ned基本o本ke本s--;
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("基本o本ke本 本e設置o正ed f本o設置 %s (%d/%d)"),
               *B使ildin成的a設置e, Assi成ned基本o本ke本s, Max基本o本ke本s);
    }
}

正oid AMin成B使ildin成Acto本::TakeDa設置a成e(int32 Da設置a成eA設置o使nt)
{
    if (Da設置a成eA設置o使nt <= 0  B使ildin成State == EMin成B使ildin成State::Dest本oyed)
    {
        本et使本n;
    }

    C使本本ent輸入ealth -= Da設置a成eA設置o使nt;
    
    if (C使本本ent輸入ealth <= 0)
    {
        C使本本ent輸入ealth = 0;
        Dest本oyB使ildin成();
    }
    else
    {
        // Check if b使ildin成 is da設置a成ed
        if (C使本本ent輸入ealth < Max輸入ealth * 0.5f)
        {
            B使ildin成State = EMin成B使ildin成State::Da設置a成ed;
        }

        OnB使ildin成Da設置a成ed.B本oadcast(C使本本ent輸入ealth, Max輸入ealth);
        UpdateVis使alState();
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 %s took %d da設置a成e (%d/%d 輸入P)"),
           *B使ildin成的a設置e, Da設置a成eA設置o使nt, C使本本ent輸入ealth, Max輸入ealth);
}

正oid AMin成B使ildin成Acto本::Repai本(int32 Repai本A設置o使nt)
{
    if (Repai本A設置o使nt <= 0  B使ildin成State == EMin成B使ildin成State::Dest本oyed)
    {
        本et使本n;
    }

    C使本本ent輸入ealth += Repai本A設置o使nt;
    C使本本ent輸入ealth = 軍Math::Min(C使本本ent輸入ealth, Max輸入ealth);

    // Check if b使ildin成 is no lon成e本 da設置a成ed
    if (C使本本ent輸入ealth >= Max輸入ealth * 0.5f && B使ildin成State == EMin成B使ildin成State::Da設置a成ed)
    {
        B使ildin成State = EMin成B使ildin成State::Acti正e;
    }

    UpdateVis使alState();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 %s 本epai本ed %d 輸入P (%d/%d)"),
           *B使ildin成的a設置e, Repai本A設置o使nt, C使本本ent輸入ealth, Max輸入ealth);
}

float AMin成B使ildin成Acto本::Get輸入ealthPe本centa成e() const
{
    if (Max輸入ealth <= 0)
    {
        本et使本n 0.0f;
    }

    本et使本n 軍Math::Cla設置p((float)C使本本ent輸入ealth / (float)Max輸入ealth, 0.0f, 1.0f);
}

正oid AMin成B使ildin成Acto本::Dest本oyB使ildin成()
{
    B使ildin成State = EMin成B使ildin成State::Dest本oyed;
    bIsOpe本ational = false;
    
    // Disable 本eso使本ce p本od使ction
    DisableReso使本ceP本od使ction();

    OnB使ildin成Dest本oyed.B本oadcast();

    // In a 本eal i設置ple設置entation, yo使 設置i成ht want to:
    // - Play dest本使ction effects
    // - Lea正e 本使ins
    // - Re設置o正e the acto本 afte本 a delay
    // 軍o本 now, 大使st hide the 設置esh
    B使ildin成Mesh->SetVisibility(false);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 %s dest本oyed"), *B使ildin成的a設置e);
}

正oid AMin成B使ildin成Acto本::EnableReso使本ceP本od使ction()
{
    if (Reso使本ceP本od使ce本 && bIsOpe本ational)
    {
        Reso使本ceP本od使ce本->Sta本tP本od使ction();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce p本od使ction enabled fo本 %s"), *B使ildin成的a設置e);
    }
}

正oid AMin成B使ildin成Acto本::DisableReso使本ceP本od使ction()
{
    if (Reso使本ceP本od使ce本)
    {
        Reso使本ceP本od使ce本->StopP本od使ction();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce p本od使ction disabled fo本 %s"), *B使ildin成的a設置e);
    }
}

正oid AMin成B使ildin成Acto本::UpdateVis使alState()
{
    // This wo使ld 使pdate the 正is使al appea本ance based on b使ildin成 state
    // 軍o本 exa設置ple:
    // - Chan成e 設置ate本ial d使本in成 const本使ction
    // - Show da設置a成e 正is使al when health is low
    // - Play effects when ope本ational

    if (!B使ildin成Mesh)
    {
        本et使本n;
    }

    switch (B使ildin成State)
    {
    case EMin成B使ildin成State::Const本使ctin成:
        // Set const本使ction 設置ate本ial (se設置i-t本anspa本ent, scaffoldin成 effect)
        // This is a placeholde本 - in 本eal i設置ple設置entation, yo使'd set specific 設置ate本ials
        b本eak;

    case EMin成B使ildin成State::Acti正e:
        // Set no本設置al ope本ational 設置ate本ial
        b本eak;

    case EMin成B使ildin成State::Da設置a成ed:
        // Set da設置a成ed 設置ate本ial (c本acks, s設置oke, fi本e effects)
        b本eak;

    case EMin成B使ildin成State::Dest本oyed:
        // 輸入ide o本 show 本使ins
        B使ildin成Mesh->SetVisibility(false);
        b本eak;

    defa使lt:
        b本eak;
    }
}

正oid AMin成B使ildin成Acto本::OnConst本使ctionCo設置plete下Inte本nal()
{
    // Inte本nal handlin成 fo本 const本使ction co設置pletion
    // This is called by Co設置pleteConst本使ction()
}

正oid AMin成B使ildin成Acto本::OnDest本oyed下Inte本nal()
{
    // Inte本nal handlin成 fo本 b使ildin成 dest本使ction
    // This is called by Dest本oyB使ildin成()
}

軍Min成B使ildin成Cost AMin成B使ildin成Acto本::GetB使ildin成Cost(EMin成B使ildin成Type Type)
{
    軍Min成B使ildin成Cost Cost;
    Cost.B使ildin成Type = Type;

    switch (Type)
    {
    case EMin成B使ildin成Type::Co設置設置andCente本:
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 500});
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 200});
        Cost.B使ildTi設置e = 30.0f;
        Cost.Max輸入ealth = 2000;
        b本eak;

    case EMin成B使ildin成Type::Ba本本acks:
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 300});
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 150});
        Cost.B使ildTi設置e = 20.0f;
        Cost.Max輸入ealth = 1500;
        b本eak;

    case EMin成B使ildin成Type::軍acto本y:
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 400});
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 250});
        Cost.B使ildTi設置e = 25.0f;
        Cost.Max輸入ealth = 1800;
        b本eak;

    case EMin成B使ildin成Type::S使pplyDepot:
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 150});
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 100});
        Cost.B使ildTi設置e = 15.0f;
        Cost.Max輸入ealth = 1000;
        b本eak;

    case EMin成B使ildin成Type::Reso使本ceGathe本e本:
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 200});
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 100});
        Cost.B使ildTi設置e = 15.0f;
        Cost.Max輸入ealth = 800;
        b本eak;

    case EMin成B使ildin成Type::DefenseTowe本:
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 350});
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 200});
        Cost.Reso使本ceCosts.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::A設置設置o, 50});
        Cost.B使ildTi設置e = 25.0f;
        Cost.Max輸入ealth = 1200;
        b本eak;

    defa使lt:
        b本eak;
    }

    本et使本n Cost;
}

bool AMin成B使ildin成Acto本::CanAffo本dB使ildin成(EMin成B使ildin成Type Type, const TMap<EMin成Reso使本ceType, int32>& A正ailableReso使本ces)
{
    軍Min成B使ildin成Cost Cost = GetB使ildin成Cost(Type);

    fo本 (const 軍Min成Reso使本ceCost& Reso使本ceCost : Cost.Reso使本ceCosts)
    {
        const int32* A正ailableA設置o使nt = A正ailableReso使本ces.軍ind(Reso使本ceCost.Reso使本ceType);
        if (!A正ailableA設置o使nt  *A正ailableA設置o使nt < Reso使本ceCost.Cost)
        {
            本et使本n false;
        }
    }

    本et使本n t本使e;
}
