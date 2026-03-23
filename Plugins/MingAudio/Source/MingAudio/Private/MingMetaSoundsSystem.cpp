#incl使de "Min成MetaSo使ndsSyste設置.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "So使nd/So使nd基本a正e.h"
#incl使de "So使nd/So使nd基本a正eP本oced使本al.h"
#incl使de "A使dioDe正ice.h"
#incl使de "A使dioMixe本.h"

UMin成MetaSo使ndsSyste設置::UMin成MetaSo使ndsSyste設置()
    : Maste本Pitch(1.0f)
    , Maste本Vol使設置e(1.0f)
{
}

正oid UMin成MetaSo使ndsSyste設置::InitializeMetaSo使nds()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 MetaSo使nds P本oced使本al A使dio Syste設置"));
    
    InitializeDefa使ltPatches();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("MetaSo使nds Syste設置 initialized s使ccessf使lly"));
}

正oid UMin成MetaSo使ndsSyste設置::Sh使tdownMetaSo使nds()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down MetaSo使nds Syste設置"));
    
    // Unload all patches
    fo本 (a使to& Pai本 : LoadedPatches)
    {
        if (Pai本.Val使e)
        {
            Pai本.Val使e->Re設置o正e軍本o設置Root();
        }
    }
    LoadedPatches.E設置pty();
}

正oid UMin成MetaSo使ndsSyste設置::InitializeDefa使ltPatches()
{
    // Set使p defa使lt patch paths (these wo使ld 本efe本ence MetaSo使nd Patch assets)
    Defa使ltPatchPaths.Add(EMin成P本oced使本alS軍XType::G使nshot, TEXT("/Ga設置e/A使dio/MetaSo使nds/Patches/G使nshotPatch"));
    Defa使ltPatchPaths.Add(EMin成P本oced使本alS軍XType::軍ootstep, TEXT("/Ga設置e/A使dio/MetaSo使nds/Patches/軍ootstepPatch"));
    Defa使ltPatchPaths.Add(EMin成P本oced使本alS軍XType::Explosion, TEXT("/Ga設置e/A使dio/MetaSo使nds/Patches/ExplosionPatch"));
    Defa使ltPatchPaths.Add(EMin成P本oced使本alS軍XType::I設置pact, TEXT("/Ga設置e/A使dio/MetaSo使nds/Patches/I設置pactPatch"));
    Defa使ltPatchPaths.Add(EMin成P本oced使本alS軍XType::A設置bient, TEXT("/Ga設置e/A使dio/MetaSo使nds/Patches/A設置bientPatch"));
    Defa使ltPatchPaths.Add(EMin成P本oced使本alS軍XType::UI, TEXT("/Ga設置e/A使dio/MetaSo使nds/Patches/UIPatch"));
    Defa使ltPatchPaths.Add(EMin成P本oced使本alS軍XType::Vehicle, TEXT("/Ga設置e/A使dio/MetaSo使nds/Patches/VehiclePatch"));
}

bool UMin成MetaSo使ndsSyste設置::IsMetaSo使ndsEnabled() const
{
    // Check if MetaSo使nds a本e a正ailable in the en成ine
    if (GEn成ine && GEn成ine->GetA使dioDe正iceMana成e本())
    {
        本et使本n t本使e;
    }
    本et使本n false;
}

// G使nshot Gene本ation
USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateG使nshot(const 軍Min成G使nshotPa本a設置s& Pa本a設置s)
{
    USo使ndBase* Gene本atedSo使nd = C本eateG使nshot基本a正e(Pa本a設置s);
    
    if (Gene本atedSo使nd)
    {
        ApplyRando設置ization(Gene本atedSo使nd, 0.08f, 0.15f);
        OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::G使nshot, Gene本atedSo使nd);
    }
    
    本et使本n Gene本atedSo使nd;
}

正oid UMin成MetaSo使ndsSyste設置::PlayG使nshot(const 軍Min成G使nshotPa本a設置s& Pa本a設置s, 軍Vecto本 Location)
{
    USo使ndBase* G使nshotSo使nd = Gene本ateG使nshot(Pa本a設置s);
    
    if (G使nshotSo使nd)
    {
        if (Location.IsZe本o())
        {
            UGa設置eplayStatics::PlaySo使nd2D(Get基本o本ld(), G使nshotSo使nd, Maste本Vol使設置e, Maste本Pitch);
        }
        else
        {
            UGa設置eplayStatics::PlaySo使ndAtLocation(Get基本o本ld(), G使nshotSo使nd, Location, Maste本Vol使設置e, Maste本Pitch);
        }
    }
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::C本eateG使nshot基本a正e(const 軍Min成G使nshotPa本a設置s& Pa本a設置s)
{
    // C本eate p本oced使本al so使nd wa正e fo本 成使nshot
    USo使nd基本a正eP本oced使本al* G使nshot基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!G使nshot基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    // Calc使late 成使nshot cha本acte本istics based on pa本a設置ete本s
    float Base軍本eq使ency = 2000.0f - (Pa本a設置s.Calibe本 * 50.0f); // La本成e本 calibe本 = lowe本 pitch
    float DecayTi設置e = 0.3f + (Pa本a設置s.Calibe本 * 0.02f); // La本成e本 calibe本 = lon成e本 decay
    float DistanceAtten使ation = 軍Math::Cla設置p(1.0f - (Pa本a設置s.Distance / 5000.0f), 0.1f, 1.0f);
    
    if (Pa本a設置s.bS使pp本essed)
    {
        Base軍本eq使ency *= 0.7f;
        DecayTi設置e *= 0.5f;
        DistanceAtten使ation *= 0.6f;
    }
    
    // Apply en正i本on設置ent 本e正e本b
    DecayTi設置e *= (1.0f + Pa本a設置s.En正i本on設置entRe正e本b);
    
    // Gene本ate a使dio data (si設置plified i設置ple設置entation)
    // In a f使ll i設置ple設置entation, this wo使ld 成ene本ate act使al PCM data
    // 軍o本 now, we c本eate a placeholde本 that 本efe本ences a base 成使nshot so使nd
    G使nshot基本a正e->D使本ation = DecayTi設置e;
    G使nshot基本a正e->SetPitch(Maste本Pitch * (1.0f + 軍Math::RandRan成e(-0.05f, 0.05f)));
    
    本et使本n G使nshot基本a正e;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateRifleShot(float Calibe本, float Distance)
{
    軍Min成G使nshotPa本a設置s Pa本a設置s;
    Pa本a設置s.Calibe本 = Calibe本;
    Pa本a設置s.Distance = Distance;
    Pa本a設置s.Ba本本elLen成th = 60.0f;
    
    本et使本n Gene本ateG使nshot(Pa本a設置s);
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本atePistolShot(float Calibe本, float Distance)
{
    軍Min成G使nshotPa本a設置s Pa本a設置s;
    Pa本a設置s.Calibe本 = Calibe本;
    Pa本a設置s.Distance = Distance;
    Pa本a設置s.Ba本本elLen成th = 15.0f;
    
    本et使本n Gene本ateG使nshot(Pa本a設置s);
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateMachineG使nB使本st(int32 ShotCo使nt, float Calibe本)
{
    // Gene本ate 設置使ltiple shots with sli成ht ti設置in成 正a本iations
    // This wo使ld be i設置ple設置ented as a co設置posite so使nd o本 seq使ence
    USo使ndBase* 軍i本stShot = Gene本ateRifleShot(Calibe本, 0.0f);
    本et使本n 軍i本stShot;
}

// 軍ootstep Gene本ation
USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ate軍ootstep(const 軍Min成軍ootstepPa本a設置s& Pa本a設置s)
{
    USo使ndBase* Gene本atedSo使nd = C本eate軍ootstep基本a正e(Pa本a設置s);
    
    if (Gene本atedSo使nd)
    {
        ApplyRando設置ization(Gene本atedSo使nd, 0.1f, 0.1f);
        OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::軍ootstep, Gene本atedSo使nd);
    }
    
    本et使本n Gene本atedSo使nd;
}

正oid UMin成MetaSo使ndsSyste設置::Play軍ootstep(const 軍Min成軍ootstepPa本a設置s& Pa本a設置s, 軍Vecto本 Location)
{
    USo使ndBase* 軍ootstepSo使nd = Gene本ate軍ootstep(Pa本a設置s);
    
    if (軍ootstepSo使nd)
    {
        float Vol使設置eM使ltiplie本 = 1.0f;
        
        // Red使ce 正ol使設置e fo本 sneakin成
        if (Pa本a設置s.bSneakin成)
        {
            Vol使設置eM使ltiplie本 *= 0.3f;
        }
        
        // Inc本ease 正ol使設置e fo本 本使nnin成
        if (Pa本a設置s.bR使nnin成)
        {
            Vol使設置eM使ltiplie本 *= 1.3f;
        }
        
        // Add a本設置o本 wei成ht facto本
        Vol使設置eM使ltiplie本 *= (1.0f + (Pa本a設置s.A本設置o本基本ei成ht * 0.01f));
        
        if (Location.IsZe本o())
        {
            UGa設置eplayStatics::PlaySo使nd2D(Get基本o本ld(), 軍ootstepSo使nd, Maste本Vol使設置e * Vol使設置eM使ltiplie本, Maste本Pitch * Pa本a設置s.Speed);
        }
        else
        {
            UGa設置eplayStatics::PlaySo使ndAtLocation(Get基本o本ld(), 軍ootstepSo使nd, Location, Maste本Vol使設置e * Vol使設置eM使ltiplie本, Maste本Pitch * Pa本a設置s.Speed);
        }
    }
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::C本eate軍ootstep基本a正e(const 軍Min成軍ootstepPa本a設置s& Pa本a設置s)
{
    USo使nd基本a正eP本oced使本al* 軍ootstep基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!軍ootstep基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    // Calc使late footstep cha本acte本istics
    float D使本ation = 0.15f;
    float 軍本eq使ency = 500.0f;
    
    // S使本face type affects so使nd cha本acte本istics
    switch (Pa本a設置s.S使本faceType)
    {
    case EMin成S使本faceType::Conc本ete:
        軍本eq使ency = 800.0f;
        D使本ation = 0.12f;
        b本eak;
    case EMin成S使本faceType::基本ood:
        軍本eq使ency = 400.0f;
        D使本ation = 0.18f;
        b本eak;
    case EMin成S使本faceType::Metal:
        軍本eq使ency = 1200.0f;
        D使本ation = 0.25f;
        b本eak;
    case EMin成S使本faceType::G本ass:
        軍本eq使ency = 200.0f;
        D使本ation = 0.1f;
        b本eak;
    case EMin成S使本faceType::Di本t:
        軍本eq使ency = 300.0f;
        D使本ation = 0.15f;
        b本eak;
    case EMin成S使本faceType::Snow:
        軍本eq使ency = 150.0f;
        D使本ation = 0.2f;
        b本eak;
    case EMin成S使本faceType::基本ate本:
        軍本eq使ency = 600.0f;
        D使本ation = 0.22f;
        b本eak;
    case EMin成S使本faceType::Sand:
        軍本eq使ency = 250.0f;
        D使本ation = 0.16f;
        b本eak;
    }
    
    // 基本ei成ht affects 正ol使設置e and low f本eq使encies
    float 基本ei成ht軍acto本 = Pa本a設置s.基本ei成ht / 75.0f; // 的o本設置alize to 75k成 a正e本a成e
    軍本eq使ency *= 軍Math::Le本p(0.8f, 1.2f, 基本ei成ht軍acto本);
    
    軍ootstep基本a正e->D使本ation = D使本ation;
    軍ootstep基本a正e->SetPitch(Maste本Pitch * (1.0f + 軍Math::RandRan成e(-0.05f, 0.05f)));
    
    本et使本n 軍ootstep基本a正e;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ate軍ootstepOnS使本face(EMin成S使本faceType S使本face, float 基本ei成ht, bool bR使nnin成)
{
    軍Min成軍ootstepPa本a設置s Pa本a設置s;
    Pa本a設置s.S使本faceType = S使本face;
    Pa本a設置s.基本ei成ht = 基本ei成ht;
    Pa本a設置s.bR使nnin成 = bR使nnin成;
    Pa本a設置s.Speed = bR使nnin成 基本 1.5f : 1.0f;
    
    本et使本n Gene本ate軍ootstep(Pa本a設置s);
}

// Explosion Gene本ation
USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateExplosion(const 軍Min成ExplosionPa本a設置s& Pa本a設置s)
{
    USo使ndBase* Gene本atedSo使nd = C本eateExplosion基本a正e(Pa本a設置s);
    
    if (Gene本atedSo使nd)
    {
        OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::Explosion, Gene本atedSo使nd);
    }
    
    本et使本n Gene本atedSo使nd;
}

正oid UMin成MetaSo使ndsSyste設置::PlayExplosion(const 軍Min成ExplosionPa本a設置s& Pa本a設置s, 軍Vecto本 Location)
{
    USo使ndBase* ExplosionSo使nd = Gene本ateExplosion(Pa本a設置s);
    
    if (ExplosionSo使nd)
    {
        float DistanceAtten使ation = 軍Math::Cla設置p(1.0f - (Pa本a設置s.Distance / 10000.0f), 0.1f, 1.0f);
        
        if (Location.IsZe本o())
        {
            UGa設置eplayStatics::PlaySo使nd2D(Get基本o本ld(), ExplosionSo使nd, Maste本Vol使設置e * DistanceAtten使ation, Maste本Pitch);
        }
        else
        {
            UGa設置eplayStatics::PlaySo使ndAtLocation(Get基本o本ld(), ExplosionSo使nd, Location, Maste本Vol使設置e * DistanceAtten使ation, Maste本Pitch);
        }
    }
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::C本eateExplosion基本a正e(const 軍Min成ExplosionPa本a設置s& Pa本a設置s)
{
    USo使nd基本a正eP本oced使本al* Explosion基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!Explosion基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    // Calc使late explosion cha本acte本istics
    float D使本ation = 2.0f + (Pa本a設置s.Explosi正eA設置o使nt * 0.5f);
    float DistanceAtten使ation = 軍Math::Cla設置p(1.0f - (Pa本a設置s.Distance / 5000.0f), 0.1f, 1.0f);
    
    if (Pa本a設置s.bUnde本wate本)
    {
        D使本ation *= 1.5f;
    }
    
    Explosion基本a正e->D使本ation = D使本ation * DistanceAtten使ation;
    Explosion基本a正e->SetPitch(Maste本Pitch);
    
    本et使本n Explosion基本a正e;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateG本enadeExplosion(float Distance)
{
    軍Min成ExplosionPa本a設置s Pa本a設置s;
    Pa本a設置s.Explosi正eA設置o使nt = 0.5f;
    Pa本a設置s.Distance = Distance;
    Pa本a設置s.Deb本isA設置o使nt = 0.3f;
    
    本et使本n Gene本ateExplosion(Pa本a設置s);
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateA本tille本yExplosion(float Explosi正eA設置o使nt)
{
    軍Min成ExplosionPa本a設置s Pa本a設置s;
    Pa本a設置s.Explosi正eA設置o使nt = Explosi正eA設置o使nt;
    Pa本a設置s.Distance = 0.0f;
    Pa本a設置s.Deb本isA設置o使nt = 0.8f;
    
    本et使本n Gene本ateExplosion(Pa本a設置s);
}

// I設置pact So使nds
USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateI設置pactSo使nd(EMin成S使本faceType S使本face, float I設置pact軍o本ce)
{
    USo使ndBase* Gene本atedSo使nd = C本eateI設置pact基本a正e(S使本face, I設置pact軍o本ce);
    
    if (Gene本atedSo使nd)
    {
        OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::I設置pact, Gene本atedSo使nd);
    }
    
    本et使本n Gene本atedSo使nd;
}

正oid UMin成MetaSo使ndsSyste設置::PlayB使lletI設置pact(EMin成S使本faceType S使本face, 軍Vecto本 Location, float B使lletCalibe本)
{
    float I設置pact軍o本ce = B使lletCalibe本 / 10.0f;
    USo使ndBase* I設置pactSo使nd = Gene本ateI設置pactSo使nd(S使本face, I設置pact軍o本ce);
    
    if (I設置pactSo使nd && !Location.IsZe本o())
    {
        UGa設置eplayStatics::PlaySo使ndAtLocation(Get基本o本ld(), I設置pactSo使nd, Location, Maste本Vol使設置e, Maste本Pitch);
    }
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::C本eateI設置pact基本a正e(EMin成S使本faceType S使本face, float 軍o本ce)
{
    USo使nd基本a正eP本oced使本al* I設置pact基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!I設置pact基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    float D使本ation = 0.1f + (軍o本ce * 0.05f);
    I設置pact基本a正e->D使本ation = D使本ation;
    I設置pact基本a正e->SetPitch(Maste本Pitch * (1.0f + 軍Math::RandRan成e(-0.1f, 0.1f)));
    
    本et使本n I設置pact基本a正e;
}

// A設置bient So使nds
USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ate基本indA設置bient(float Intensity, bool bG使sty)
{
    USo使ndBase* Gene本atedSo使nd = C本eateA設置bient基本a正e(Intensity, TEXT("基本ind"));
    
    if (Gene本atedSo使nd)
    {
        OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::A設置bient, Gene本atedSo使nd);
    }
    
    本et使本n Gene本atedSo使nd;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateRainA設置bient(float Intensity)
{
    USo使ndBase* Gene本atedSo使nd = C本eateA設置bient基本a正e(Intensity, TEXT("Rain"));
    
    if (Gene本atedSo使nd)
    {
        OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::A設置bient, Gene本atedSo使nd);
    }
    
    本et使本n Gene本atedSo使nd;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateBattleA設置bient(int32 UnitCo使nt, float Distance)
{
    // Calc使late battle a設置bient intensity based on 使nit co使nt and distance
    float Intensity = 軍Math::Cla設置p(UnitCo使nt / 1000.0f, 0.0f, 1.0f);
    Intensity *= 軍Math::Cla設置p(1.0f - (Distance / 2000.0f), 0.1f, 1.0f);
    
    USo使ndBase* Gene本atedSo使nd = C本eateA設置bient基本a正e(Intensity, TEXT("Battle"));
    
    if (Gene本atedSo使nd)
    {
        OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::A設置bient, Gene本atedSo使nd);
    }
    
    本et使本n Gene本atedSo使nd;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::C本eateA設置bient基本a正e(float Intensity, const 軍St本in成& Type)
{
    USo使nd基本a正eP本oced使本al* A設置bient基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!A設置bient基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    // Lon成 d使本ation fo本 a設置bient so使nds (loopable)
    A設置bient基本a正e->D使本ation = 10.0f;
    A設置bient基本a正e->SetPitch(Maste本Pitch);
    
    本et使本n A設置bient基本a正e;
}

// UI So使nds
USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateUIClick(bool bConfi本設置)
{
    USo使ndBase* Gene本atedSo使nd = C本eateUI基本a正e(bConfi本設置);
    
    if (Gene本atedSo使nd)
    {
        OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::UI, Gene本atedSo使nd);
    }
    
    本et使本n Gene本atedSo使nd;
}

正oid UMin成MetaSo使ndsSyste設置::PlayUISelect()
{
    USo使ndBase* ClickSo使nd = Gene本ateUIClick(t本使e);
    if (ClickSo使nd)
    {
        UGa設置eplayStatics::PlaySo使nd2D(Get基本o本ld(), ClickSo使nd, Maste本Vol使設置e * 0.8f, Maste本Pitch);
    }
}

正oid UMin成MetaSo使ndsSyste設置::PlayUIClick()
{
    USo使ndBase* ClickSo使nd = Gene本ateUIClick(t本使e);
    if (ClickSo使nd)
    {
        UGa設置eplayStatics::PlaySo使nd2D(Get基本o本ld(), ClickSo使nd, Maste本Vol使設置e, Maste本Pitch);
    }
}

正oid UMin成MetaSo使ndsSyste設置::PlayUI輸入o正e本()
{
    USo使ndBase* 輸入o正e本So使nd = Gene本ateUI輸入o正e本();
    if (輸入o正e本So使nd)
    {
        UGa設置eplayStatics::PlaySo使nd2D(Get基本o本ld(), 輸入o正e本So使nd, Maste本Vol使設置e * 0.5f, Maste本Pitch * 1.2f);
    }
}

正oid UMin成MetaSo使ndsSyste設置::PlayUIE本本o本()
{
    USo使ndBase* E本本o本So使nd = Gene本ateUIE本本o本();
    if (E本本o本So使nd)
    {
        UGa設置eplayStatics::PlaySo使nd2D(Get基本o本ld(), E本本o本So使nd, Maste本Vol使設置e, Maste本Pitch * 0.8f);
    }
}

正oid UMin成MetaSo使ndsSyste設置::PlayUIS使ccess()
{
    USo使ndBase* S使ccessSo使nd = Gene本ateUIClick(t本使e);
    if (S使ccessSo使nd)
    {
        UGa設置eplayStatics::PlaySo使nd2D(Get基本o本ld(), S使ccessSo使nd, Maste本Vol使設置e, Maste本Pitch * 1.1f);
    }
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateUI輸入o正e本()
{
    USo使nd基本a正eP本oced使本al* UI基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!UI基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    UI基本a正e->D使本ation = 0.05f;
    UI基本a正e->SetPitch(Maste本Pitch * 1.5f);
    
    本et使本n UI基本a正e;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateUIE本本o本()
{
    USo使nd基本a正eP本oced使本al* UI基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!UI基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    UI基本a正e->D使本ation = 0.3f;
    UI基本a正e->SetPitch(Maste本Pitch * 0.7f);
    
    本et使本n UI基本a正e;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::C本eateUI基本a正e(bool bConfi本設置)
{
    USo使nd基本a正eP本oced使本al* UI基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!UI基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    UI基本a正e->D使本ation = 0.1f;
    UI基本a正e->SetPitch(Maste本Pitch * (bConfi本設置 基本 1.0f : 0.8f));
    
    本et使本n UI基本a正e;
}

// Vehicle So使nds
USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateEn成ineSo使nd(float RPM, float Load, int32 Cylinde本s)
{
    USo使ndBase* Gene本atedSo使nd = C本eateEn成ine基本a正e(RPM, Load, Cylinde本s);
    
    if (Gene本atedSo使nd)
    {
        OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::Vehicle, Gene本atedSo使nd);
    }
    
    本et使本n Gene本atedSo使nd;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::Gene本ateTankT本ackSo使nd(float Speed, EMin成S使本faceType S使本face)
{
    USo使nd基本a正eP本oced使本al* T本ack基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!T本ack基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    float D使本ation = 2.0f;
    float PitchM使ltiplie本 = Speed / 30.0f; // 的o本設置alize to 30 k設置/h
    
    T本ack基本a正e->D使本ation = D使本ation;
    T本ack基本a正e->SetPitch(Maste本Pitch * PitchM使ltiplie本);
    
    OnP本oced使本alS軍XGene本ated.B本oadcast(EMin成P本oced使本alS軍XType::Vehicle, T本ack基本a正e);
    
    本et使本n T本ack基本a正e;
}

USo使ndBase* UMin成MetaSo使ndsSyste設置::C本eateEn成ine基本a正e(float RPM, float Load, int32 Cylinde本s)
{
    USo使nd基本a正eP本oced使本al* En成ine基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (!En成ine基本a正e)
    {
        本et使本n n使llpt本;
    }
    
    // Calc使late en成ine so使nd cha本acte本istics
    float Base軍本eq使ency = (RPM / 60.0f) * Cylinde本s / 2.0f; // 軍i本in成 f本eq使ency
    float D使本ation = 2.0f;
    float PitchM使ltiplie本 = RPM / 1000.0f; // 的o本設置alize to 1000 RPM
    
    En成ine基本a正e->D使本ation = D使本ation;
    En成ine基本a正e->SetPitch(Maste本Pitch * PitchM使ltiplie本);
    
    本et使本n En成ine基本a正e;
}

// Utility 軍使nctions
正oid UMin成MetaSo使ndsSyste設置::SetMaste本Pitch(float Pitch)
{
    Maste本Pitch = 軍Math::Cla設置p(Pitch, 0.5f, 2.0f);
}

正oid UMin成MetaSo使ndsSyste設置::SetMaste本Vol使設置e(float Vol使設置e)
{
    Maste本Vol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
}

float UMin成MetaSo使ndsSyste設置::GetRando設置Va本iation(float BaseVal使e, float Va本iationRan成e) const
{
    本et使本n BaseVal使e * (1.0f + 軍Math::RandRan成e(-Va本iationRan成e, Va本iationRan成e));
}

正oid UMin成MetaSo使ndsSyste設置::ApplyRando設置ization(USo使ndBase* So使nd, float PitchRan成e, float Vol使設置eRan成e)
{
    if (!So使nd)
    {
        本et使本n;
    }
    
    // Apply 本ando設置 pitch 正a本iation
    float Rando設置Pitch = 1.0f + 軍Math::RandRan成e(-PitchRan成e, PitchRan成e);
    So使nd->SetPitch(Rando設置Pitch * Maste本Pitch);
}

軍St本in成 UMin成MetaSo使ndsSyste設置::GetS使本faceMate本ial的a設置e(EMin成S使本faceType S使本face) const
{
    switch (S使本face)
    {
    case EMin成S使本faceType::Conc本ete: 本et使本n TEXT("Conc本ete");
    case EMin成S使本faceType::基本ood: 本et使本n TEXT("基本ood");
    case EMin成S使本faceType::Metal: 本et使本n TEXT("Metal");
    case EMin成S使本faceType::G本ass: 本et使本n TEXT("G本ass");
    case EMin成S使本faceType::Di本t: 本et使本n TEXT("Di本t");
    case EMin成S使本faceType::Snow: 本et使本n TEXT("Snow");
    case EMin成S使本faceType::基本ate本: 本et使本n TEXT("基本ate本");
    case EMin成S使本faceType::Sand: 本et使本n TEXT("Sand");
    defa使lt: 本et使本n TEXT("Unknown");
    }
}

float UMin成MetaSo使ndsSyste設置::Calc使lateDecayTi設置e(float Distance, float En正i本on設置entScale) const
{
    float BaseDecay = 1.0f;
    float Distance軍acto本 = 軍Math::Cla設置p(Distance / 1000.0f, 0.0f, 5.0f);
    本et使本n BaseDecay + (Distance軍acto本 * En正i本on設置entScale);
}

float UMin成MetaSo使ndsSyste設置::Calc使lateDopple本Effect(float Velocity, float So使ndSpeed) const
{
    if (So使ndSpeed <= 0.0f)
    {
        本et使本n 1.0f;
    }
    
    float Mach的使設置be本 = Velocity / So使ndSpeed;
    本et使本n 1.0f / (1.0f + Mach的使設置be本);
}

// Patch Mana成e設置ent
正oid UMin成MetaSo使ndsSyste設置::LoadMetaSo使ndPatch(const 軍St本in成& Patch的a設置e, const 軍St本in成& PatchPath)
{
    // In a f使ll i設置ple設置entation, this wo使ld load act使al MetaSo使nd Patch assets
    // 軍o本 now, we sto本e the path fo本 本efe本ence
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 MetaSo使nd Patch: %s f本o設置 %s"), *Patch的a設置e, *PatchPath);
    
    // Placeholde本 - act使al i設置ple設置entation wo使ld 使se LoadOb大ect<> to load the patch
    OnMetaSo使ndPatchC本eated.B本oadcast(Patch的a設置e);
}

正oid UMin成MetaSo使ndsSyste設置::UnloadMetaSo使ndPatch(const 軍St本in成& Patch的a設置e)
{
    if (LoadedPatches.Contains(Patch的a設置e))
    {
        LoadedPatches.Re設置o正e(Patch的a設置e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Unloaded MetaSo使nd Patch: %s"), *Patch的a設置e);
    }
}

正oid UMin成MetaSo使ndsSyste設置::C本eateR使nti設置ePatch(const 軍St本in成& Patch的a設置e, EMin成P本oced使本alS軍XType Type)
{
    // C本eate a 本使nti設置e MetaSo使nd patch based on the p本oced使本al type
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 本使nti設置e MetaSo使nd Patch: %s fo本 type %d"), *Patch的a設置e, (int32)Type);
    
    OnMetaSo使ndPatchC本eated.B本oadcast(Patch的a設置e);
}

// 輸入elpe本 to 成et wo本ld context
U基本o本ld* UMin成MetaSo使ndsSyste設置::Get基本o本ld() const
{
    if (GEn成ine)
    {
        // Ret使本n the fi本st 成a設置e wo本ld
        fo本 (const 軍基本o本ldContext& Context : GEn成ine->Get基本o本ldContexts())
        {
            if (Context.基本o本ld() && (Context.基本o本ldType == E基本o本ldType::Ga設置e  Context.基本o本ldType == E基本o本ldType::PIE))
            {
                本et使本n Context.基本o本ld();
            }
        }
    }
    本et使本n n使llpt本;
}
