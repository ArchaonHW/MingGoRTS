#incl使de "Min成的ia成a本aEffectsSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "En成ine/En成ine.h"
#incl使de "的ia成a本aDataInte本faceA本本ay軍使nctionLib本a本y.h"

UMin成的ia成a本aEffectsSyste設置::UMin成的ia成a本aEffectsSyste設置()
    : MaxConc使本本entEffects(100)
    , Acti正eEffectCo使nt(0)
    , C使本本ent基本eathe本Effect(n使llpt本)
{
}

正oid UMin成的ia成a本aEffectsSyste設置::Initialize的ia成a本aSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 的ia成a本a Effects Syste設置"));
    
    InitializeEffectTe設置plates();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的ia成a本a Effects Syste設置 initialized with 設置ax %d conc使本本ent effects"), MaxConc使本本entEffects);
}

正oid UMin成的ia成a本aEffectsSyste設置::Sh使tdown的ia成a本aSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down 的ia成a本a Effects Syste設置"));
    
    // Stop all acti正e effects
    fo本 (a使to* Effect : Acti正eEffects)
    {
        if (Effect)
        {
            Effect->Deacti正ate();
        }
    }
    
    Acti正eEffects.E設置pty();
    Acti正eEffectCo使nt = 0;
    
    // Clea本 weathe本 effect
    if (C使本本ent基本eathe本Effect)
    {
        C使本本ent基本eathe本Effect->Deacti正ate();
        C使本本ent基本eathe本Effect = n使llpt本;
    }
}

正oid UMin成的ia成a本aEffectsSyste設置::InitializeEffectTe設置plates()
{
    // Set 使p defa使lt 的ia成a本a syste設置 paths
    的ia成a本aSyste設置Paths.Add(TEXT("Explosion"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下Explosion"));
    的ia成a本aSyste設置Paths.Add(TEXT("M使zzle軍lash"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下M使zzle軍lash"));
    的ia成a本aSyste設置Paths.Add(TEXT("B使lletT本ail"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下B使lletT本ail"));
    的ia成a本aSyste設置Paths.Add(TEXT("I設置pact"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下I設置pact"));
    的ia成a本aSyste設置Paths.Add(TEXT("軍i本e"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下軍i本e"));
    的ia成a本aSyste設置Paths.Add(TEXT("S設置oke"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下S設置oke"));
    的ia成a本aSyste設置Paths.Add(TEXT("D使st"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下D使st"));
    的ia成a本aSyste設置Paths.Add(TEXT("Spa本k"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下Spa本k"));
    的ia成a本aSyste設置Paths.Add(TEXT("Rain"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下Rain"));
    的ia成a本aSyste設置Paths.Add(TEXT("Snow"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下Snow"));
    的ia成a本aSyste設置Paths.Add(TEXT("軍o成"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下軍o成"));
    的ia成a本aSyste設置Paths.Add(TEXT("Selection"), TEXT("/Ga設置e/Effects/的ia成a本a/的S下Selection"));
}

// Explosion Effects
U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnExplosion(const 軍Vecto本& Location, const 軍Min成ExplosionPa本a設置s& Pa本a設置s)
{
    U的ia成a本aCo設置ponent* Explosion = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::Explosion, Location);
    
    if (Explosion)
    {
        // Set explosion pa本a設置ete本s
        Explosion->Set軍loatPa本a設置ete本(TEXT("Scale"), Pa本a設置s.Scale);
        Explosion->Set軍loatPa本a設置ete本(TEXT("D使本ation"), Pa本a設置s.D使本ation);
        Explosion->Set軍loatPa本a設置ete本(TEXT("Pa本ticleCo使nt"), static下cast<float>(Pa本a設置s.Pa本ticleCo使nt));
        Explosion->SetColo本Pa本a設置ete本(TEXT("軍i本eColo本"), Pa本a設置s.軍i本eColo本);
        Explosion->SetColo本Pa本a設置ete本(TEXT("S設置okeColo本"), Pa本a設置s.S設置okeColo本);
        Explosion->SetBoolPa本a設置ete本(TEXT("bSpa本ks"), Pa本a設置s.bSpa本ks);
        Explosion->SetBoolPa本a設置ete本(TEXT("bShockwa正e"), Pa本a設置s.bShockwa正e);
        Explosion->SetBoolPa本a設置ete本(TEXT("bDeb本is"), Pa本a設置s.bDeb本is);
        
        Explosion->Acti正ate(t本使e);
        On的ia成a本aEffectSpawned.B本oadcast(EMin成的ia成a本aEffectType::Explosion, Explosion);
    }
    
    本et使本n Explosion;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnG本enadeExplosion(const 軍Vecto本& Location, float Scale)
{
    軍Min成ExplosionPa本a設置s Pa本a設置s;
    Pa本a設置s.ExplosionType = EMin成ExplosionType::S設置all;
    Pa本a設置s.Scale = Scale;
    Pa本a設置s.Pa本ticleCo使nt = 50;
    Pa本a設置s.D使本ation = 1.5f;
    Pa本a設置s.bShockwa正e = false;
    
    本et使本n SpawnExplosion(Location, Pa本a設置s);
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnA本tille本yExplosion(const 軍Vecto本& Location, float Scale)
{
    軍Min成ExplosionPa本a設置s Pa本a設置s;
    Pa本a設置s.ExplosionType = EMin成ExplosionType::A本tille本y;
    Pa本a設置s.Scale = Scale;
    Pa本a設置s.Pa本ticleCo使nt = 300;
    Pa本a設置s.D使本ation = 4.0f;
    Pa本a設置s.bShockwa正e = t本使e;
    Pa本a設置s.bDeb本is = t本使e;
    Pa本a設置s.軍i本eColo本 = 軍Linea本Colo本(1.0f, 0.4f, 0.05f);
    
    本et使本n SpawnExplosion(Location, Pa本a設置s);
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnBa本本elExplosion(const 軍Vecto本& Location)
{
    軍Min成ExplosionPa本a設置s Pa本a設置s;
    Pa本a設置s.ExplosionType = EMin成ExplosionType::Medi使設置;
    Pa本a設置s.Scale = 1.5f;
    Pa本a設置s.Pa本ticleCo使nt = 150;
    Pa本a設置s.D使本ation = 3.0f;
    Pa本a設置s.bShockwa正e = t本使e;
    
    本et使本n SpawnExplosion(Location, Pa本a設置s);
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnVehicleExplosion(const 軍Vecto本& Location)
{
    軍Min成ExplosionPa本a設置s Pa本a設置s;
    Pa本a設置s.ExplosionType = EMin成ExplosionType::La本成e;
    Pa本a設置s.Scale = 3.0f;
    Pa本a設置s.Pa本ticleCo使nt = 500;
    Pa本a設置s.D使本ation = 5.0f;
    Pa本a設置s.bShockwa正e = t本使e;
    Pa本a設置s.bDeb本is = t本使e;
    Pa本a設置s.S設置okeColo本 = 軍Linea本Colo本(0.1f, 0.1f, 0.1f);
    
    本et使本n SpawnExplosion(Location, Pa本a設置s);
}

// M使zzle 軍lash Effects
U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnM使zzle軍lash(const 軍Vecto本& Location, const 軍Vecto本& Di本ection, const 軍Min成M使zzle軍lashPa本a設置s& Pa本a設置s)
{
    軍Q使at Rotation = 軍Q使at::軍indBetween的o本設置als(軍Vecto本::軍o本wa本dVecto本, Di本ection.GetSafe的o本設置al());
    U的ia成a本aCo設置ponent* M使zzle軍lash = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::M使zzle軍lash, Location, Rotation);
    
    if (M使zzle軍lash)
    {
        M使zzle軍lash->Set軍loatPa本a設置ete本(TEXT("Calibe本"), Pa本a設置s.Calibe本);
        M使zzle軍lash->Set軍loatPa本a設置ete本(TEXT("Scale"), Pa本a設置s.Scale);
        M使zzle軍lash->Set軍loatPa本a設置ete本(TEXT("D使本ation"), Pa本a設置s.D使本ation);
        M使zzle軍lash->SetColo本Pa本a設置ete本(TEXT("軍lashColo本"), Pa本a設置s.軍lashColo本);
        M使zzle軍lash->SetBoolPa本a設置ete本(TEXT("bS設置oke"), Pa本a設置s.bS設置oke);
        
        M使zzle軍lash->Acti正ate(t本使e);
        On的ia成a本aEffectSpawned.B本oadcast(EMin成的ia成a本aEffectType::M使zzle軍lash, M使zzle軍lash);
    }
    
    本et使本n M使zzle軍lash;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnRifleM使zzle軍lash(const 軍Vecto本& Location, const 軍Vecto本& Di本ection)
{
    軍Min成M使zzle軍lashPa本a設置s Pa本a設置s;
    Pa本a設置s.Calibe本 = 7.62f;
    Pa本a設置s.Scale = 1.0f;
    Pa本a設置s.D使本ation = 0.05f;
    Pa本a設置s.bS設置oke = t本使e;
    
    本et使本n SpawnM使zzle軍lash(Location, Di本ection, Pa本a設置s);
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnPistolM使zzle軍lash(const 軍Vecto本& Location, const 軍Vecto本& Di本ection)
{
    軍Min成M使zzle軍lashPa本a設置s Pa本a設置s;
    Pa本a設置s.Calibe本 = 9.0f;
    Pa本a設置s.Scale = 0.7f;
    Pa本a設置s.D使本ation = 0.04f;
    Pa本a設置s.bS設置oke = t本使e;
    Pa本a設置s.軍lashColo本 = 軍Linea本Colo本(1.0f, 0.8f, 0.3f);
    
    本et使本n SpawnM使zzle軍lash(Location, Di本ection, Pa本a設置s);
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnMachineG使nM使zzle軍lash(const 軍Vecto本& Location, const 軍Vecto本& Di本ection)
{
    軍Min成M使zzle軍lashPa本a設置s Pa本a設置s;
    Pa本a設置s.Calibe本 = 7.62f;
    Pa本a設置s.Scale = 1.2f;
    Pa本a設置s.D使本ation = 0.08f;
    Pa本a設置s.bS設置oke = t本使e;
    Pa本a設置s.軍lashColo本 = 軍Linea本Colo本(1.0f, 0.7f, 0.2f);
    
    本et使本n SpawnM使zzle軍lash(Location, Di本ection, Pa本a設置s);
}

// B使llet T本ail Effects
U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnB使lletT本ail(const 軍Vecto本& Sta本tLocation, const 軍Vecto本& EndLocation, const 軍Min成T本ailPa本a設置s& Pa本a設置s)
{
    軍Vecto本 Di本ection = EndLocation - Sta本tLocation;
    float Distance = Di本ection.Size();
    軍Q使at Rotation = 軍Q使at::軍indBetween的o本設置als(軍Vecto本::軍o本wa本dVecto本, Di本ection.GetSafe的o本設置al());
    
    // Spawn at 設置idpoint
    軍Vecto本 Midpoint = (Sta本tLocation + EndLocation) * 0.5f;
    U的ia成a本aCo設置ponent* T本ail = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::B使lletT本ail, Midpoint, Rotation);
    
    if (T本ail)
    {
        T本ail->Set軍loatPa本a設置ete本(TEXT("Thickness"), Pa本a設置s.Thickness);
        T本ail->Set軍loatPa本a設置ete本(TEXT("Len成th"), Distance);
        T本ail->Set軍loatPa本a設置ete本(TEXT("D使本ation"), Pa本a設置s.D使本ation);
        T本ail->Set軍loatPa本a設置ete本(TEXT("軍adeTi設置e"), Pa本a設置s.軍adeTi設置e);
        T本ail->SetColo本Pa本a設置ete本(TEXT("T本ailColo本"), Pa本a設置s.Colo本);
        
        T本ail->Acti正ate(t本使e);
        On的ia成a本aEffectSpawned.B本oadcast(EMin成的ia成a本aEffectType::B使lletT本ail, T本ail);
    }
    
    本et使本n T本ail;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnT本ace本Ro使nd(const 軍Vecto本& Sta本tLocation, const 軍Vecto本& EndLocation, 軍Linea本Colo本 Colo本)
{
    軍Min成T本ailPa本a設置s Pa本a設置s;
    Pa本a設置s.Thickness = 4.0f;
    Pa本a設置s.Colo本 = Colo本;
    Pa本a設置s.D使本ation = 0.3f;
    Pa本a設置s.軍adeTi設置e = 0.2f;
    
    本et使本n SpawnB使lletT本ail(Sta本tLocation, EndLocation, Pa本a設置s);
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnP本o大ectileT本ail(const 軍Vecto本& Sta本tLocation, const 軍Vecto本& EndLocation, float Thickness)
{
    軍Min成T本ailPa本a設置s Pa本a設置s;
    Pa本a設置s.Thickness = Thickness;
    Pa本a設置s.Colo本 = 軍Linea本Colo本(0.5f, 0.5f, 0.5f, 0.5f);
    Pa本a設置s.D使本ation = 1.0f;
    Pa本a設置s.軍adeTi設置e = 0.5f;
    
    本et使本n SpawnB使lletT本ail(Sta本tLocation, EndLocation, Pa本a設置s);
}

// I設置pact Effects
U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnB使lletI設置pact(const 軍Vecto本& Location, const 軍Vecto本& 的o本設置al, float Calibe本)
{
    軍Q使at Rotation = 軍Q使at::軍indBetween的o本設置als(軍Vecto本::UpVecto本, 的o本設置al.GetSafe的o本設置al());
    U的ia成a本aCo設置ponent* I設置pact = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::I設置pact, Location, Rotation);
    
    if (I設置pact)
    {
        I設置pact->Set軍loatPa本a設置ete本(TEXT("Calibe本"), Calibe本);
        I設置pact->Set軍loatPa本a設置ete本(TEXT("I設置pact軍o本ce"), Calibe本 / 10.0f);
        I設置pact->Acti正ate(t本使e);
        On的ia成a本aEffectSpawned.B本oadcast(EMin成的ia成a本aEffectType::I設置pact, I設置pact);
    }
    
    本et使本n I設置pact;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnG本o使ndI設置pact(const 軍Vecto本& Location, float Scale)
{
    U的ia成a本aCo設置ponent* I設置pact = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::I設置pact, Location);
    
    if (I設置pact)
    {
        I設置pact->Set軍loatPa本a設置ete本(TEXT("Scale"), Scale);
        I設置pact->SetColo本Pa本a設置ete本(TEXT("D使stColo本"), 軍Linea本Colo本(0.4f, 0.35f, 0.3f));
        I設置pact->Acti正ate(t本使e);
    }
    
    本et使本n I設置pact;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnBloodI設置pact(const 軍Vecto本& Location, float Scale)
{
    U的ia成a本aCo設置ponent* I設置pact = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::I設置pact, Location);
    
    if (I設置pact)
    {
        I設置pact->Set軍loatPa本a設置ete本(TEXT("Scale"), Scale);
        I設置pact->SetColo本Pa本a設置ete本(TEXT("BloodColo本"), 軍Linea本Colo本(0.6f, 0.05f, 0.05f));
        I設置pact->SetBoolPa本a設置ete本(TEXT("bBlood"), t本使e);
        I設置pact->Acti正ate(t本使e);
    }
    
    本et使本n I設置pact;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnSpa本ks(const 軍Vecto本& Location, int32 Co使nt)
{
    U的ia成a本aCo設置ponent* Spa本ks = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::Spa本k, Location);
    
    if (Spa本ks)
    {
        Spa本ks->SetIntPa本a設置ete本(TEXT("Spa本kCo使nt"), Co使nt);
        Spa本ks->Acti正ate(t本使e);
        On的ia成a本aEffectSpawned.B本oadcast(EMin成的ia成a本aEffectType::Spa本k, Spa本ks);
    }
    
    本et使本n Spa本ks;
}

// 軍i本e and S設置oke Effects
U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::Spawn軍i本e(const 軍Vecto本& Location, float Scale, float D使本ation)
{
    U的ia成a本aCo設置ponent* 軍i本e = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::軍i本e, Location);
    
    if (軍i本e)
    {
        軍i本e->Set軍loatPa本a設置ete本(TEXT("Scale"), Scale);
        軍i本e->Set軍loatPa本a設置ete本(TEXT("D使本ation"), D使本ation);
        軍i本e->SetColo本Pa本a設置ete本(TEXT("軍i本eColo本"), 軍Linea本Colo本(1.0f, 0.5f, 0.1f));
        軍i本e->Acti正ate(t本使e);
        On的ia成a本aEffectSpawned.B本oadcast(EMin成的ia成a本aEffectType::軍i本e, 軍i本e);
    }
    
    本et使本n 軍i本e;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnS設置oke(const 軍Vecto本& Location, float Scale, 軍Linea本Colo本 Colo本)
{
    U的ia成a本aCo設置ponent* S設置oke = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::S設置oke, Location);
    
    if (S設置oke)
    {
        S設置oke->Set軍loatPa本a設置ete本(TEXT("Scale"), Scale);
        S設置oke->SetColo本Pa本a設置ete本(TEXT("S設置okeColo本"), Colo本);
        S設置oke->Set軍loatPa本a設置ete本(TEXT("D使本ation"), 10.0f);
        S設置oke->Acti正ate(t本使e);
        On的ia成a本aEffectSpawned.B本oadcast(EMin成的ia成a本aEffectType::S設置oke, S設置oke);
    }
    
    本et使本n S設置oke;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnD使st(const 軍Vecto本& Location, float Scale)
{
    U的ia成a本aCo設置ponent* D使st = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::D使st, Location);
    
    if (D使st)
    {
        D使st->Set軍loatPa本a設置ete本(TEXT("Scale"), Scale);
        D使st->SetColo本Pa本a設置ete本(TEXT("D使stColo本"), 軍Linea本Colo本(0.5f, 0.45f, 0.4f));
        D使st->Acti正ate(t本使e);
        On的ia成a本aEffectSpawned.B本oadcast(EMin成的ia成a本aEffectType::D使st, D使st);
    }
    
    本et使本n D使st;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnB使本nin成Ob大ect(const 軍Vecto本& Location, float Scale)
{
    U的ia成a本aCo設置ponent* 軍i本e = Spawn軍i本e(Location, Scale, 60.0f);
    
    // Also spawn s設置oke
    軍Vecto本 S設置okeLocation = Location + 軍Vecto本(0, 0, 100 * Scale);
    SpawnS設置oke(S設置okeLocation, Scale * 2.0f);
    
    本et使本n 軍i本e;
}

// 基本eathe本 Effects
U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::Spawn基本eathe本Effect(const 軍Min成基本eathe本Pa本a設置s& Pa本a設置s)
{
    // Stop existin成 weathe本 effect
    Stop基本eathe本Effect();
    
    軍Vecto本 Location = 軍Vecto本::Ze本oVecto本;
    U的ia成a本aCo設置ponent* 基本eathe本 = n使llpt本;
    
    switch (Pa本a設置s.基本eathe本Type)
    {
    case EMin成基本eathe本Type::Rain:
        基本eathe本 = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::基本eathe本, Location);
        b本eak;
    case EMin成基本eathe本Type::Snow:
        基本eathe本 = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::基本eathe本, Location);
        b本eak;
    case EMin成基本eathe本Type::軍o成:
        基本eathe本 = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::基本eathe本, Location);
        b本eak;
    defa使lt:
        基本eathe本 = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::基本eathe本, Location);
        b本eak;
    }
    
    if (基本eathe本)
    {
        基本eathe本->Set軍loatPa本a設置ete本(TEXT("Intensity"), Pa本a設置s.Intensity);
        基本eathe本->Set軍loatPa本a設置ete本(TEXT("基本indSpeed"), Pa本a設置s.基本indSpeed);
        基本eathe本->SetVecto本Pa本a設置ete本(TEXT("基本indDi本ection"), Pa本a設置s.基本indDi本ection);
        基本eathe本->SetColo本Pa本a設置ete本(TEXT("基本eathe本Colo本"), Pa本a設置s.Colo本);
        基本eathe本->Set軍loatPa本a設置ete本(TEXT("Co正e本a成e"), Pa本a設置s.Co正e本a成e);
        基本eathe本->Acti正ate(t本使e);
        
        C使本本ent基本eathe本Effect = 基本eathe本;
    }
    
    本et使本n 基本eathe本;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnRain(float Intensity)
{
    軍Min成基本eathe本Pa本a設置s Pa本a設置s;
    Pa本a設置s.基本eathe本Type = EMin成基本eathe本Type::Rain;
    Pa本a設置s.Intensity = Intensity;
    Pa本a設置s.Colo本 = 軍Linea本Colo本(0.6f, 0.65f, 0.7f);
    
    本et使本n Spawn基本eathe本Effect(Pa本a設置s);
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnSnow(float Intensity)
{
    軍Min成基本eathe本Pa本a設置s Pa本a設置s;
    Pa本a設置s.基本eathe本Type = EMin成基本eathe本Type::Snow;
    Pa本a設置s.Intensity = Intensity;
    Pa本a設置s.Colo本 = 軍Linea本Colo本(0.95f, 0.95f, 0.98f);
    
    本et使本n Spawn基本eathe本Effect(Pa本a設置s);
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::Spawn軍o成(float Density)
{
    軍Min成基本eathe本Pa本a設置s Pa本a設置s;
    Pa本a設置s.基本eathe本Type = EMin成基本eathe本Type::軍o成;
    Pa本a設置s.Intensity = Density;
    Pa本a設置s.Colo本 = 軍Linea本Colo本(0.7f, 0.72f, 0.75f);
    
    本et使本n Spawn基本eathe本Effect(Pa本a設置s);
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnD使stSto本設置(float Intensity)
{
    軍Min成基本eathe本Pa本a設置s Pa本a設置s;
    Pa本a設置s.基本eathe本Type = EMin成基本eathe本Type::D使stSto本設置;
    Pa本a設置s.Intensity = Intensity;
    Pa本a設置s.Colo本 = 軍Linea本Colo本(0.6f, 0.55f, 0.45f);
    Pa本a設置s.基本indSpeed = 50.0f;
    
    本et使本n Spawn基本eathe本Effect(Pa本a設置s);
}

正oid UMin成的ia成a本aEffectsSyste設置::Stop基本eathe本Effect()
{
    if (C使本本ent基本eathe本Effect)
    {
        C使本本ent基本eathe本Effect->Deacti正ate();
        C使本本ent基本eathe本Effect = n使llpt本;
    }
}

// En正i本on設置ent Effects
U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnLea正es(const 軍Vecto本& Location, float Co使nt)
{
    U的ia成a本aCo設置ponent* Lea正es = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::En正i本on設置ent, Location);
    
    if (Lea正es)
    {
        Lea正es->Set軍loatPa本a設置ete本(TEXT("Pa本ticleCo使nt"), Co使nt);
        Lea正es->SetColo本Pa本a設置ete本(TEXT("Pa本ticleColo本"), 軍Linea本Colo本(0.3f, 0.5f, 0.2f));
        Lea正es->Acti正ate(t本使e);
    }
    
    本et使本n Lea正es;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnB使tte本flies(const 軍Vecto本& Location, float Co使nt)
{
    U的ia成a本aCo設置ponent* B使tte本flies = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::En正i本on設置ent, Location);
    
    if (B使tte本flies)
    {
        B使tte本flies->Set軍loatPa本a設置ete本(TEXT("Pa本ticleCo使nt"), Co使nt);
        B使tte本flies->SetColo本Pa本a設置ete本(TEXT("Pa本ticleColo本"), 軍Linea本Colo本(0.8f, 0.6f, 0.2f));
        B使tte本flies->Set軍loatPa本a設置ete本(TEXT("Speed"), 2.0f);
        B使tte本flies->Acti正ate(t本使e);
    }
    
    本et使本n B使tte本flies;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::Spawn軍i本eflies(const 軍Vecto本& Location, float Co使nt)
{
    U的ia成a本aCo設置ponent* 軍i本eflies = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::En正i本on設置ent, Location);
    
    if (軍i本eflies)
    {
        軍i本eflies->Set軍loatPa本a設置ete本(TEXT("Pa本ticleCo使nt"), Co使nt);
        軍i本eflies->SetColo本Pa本a設置ete本(TEXT("Pa本ticleColo本"), 軍Linea本Colo本(0.9f, 1.0f, 0.3f));
        軍i本eflies->Set軍loatPa本a設置ete本(TEXT("Glow"), 1.0f);
        軍i本eflies->Acti正ate(t本使e);
    }
    
    本et使本n 軍i本eflies;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnPollen(const 軍Vecto本& Location, float Density)
{
    U的ia成a本aCo設置ponent* Pollen = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::En正i本on設置ent, Location);
    
    if (Pollen)
    {
        Pollen->Set軍loatPa本a設置ete本(TEXT("Pa本ticleCo使nt"), Density * 100.0f);
        Pollen->SetColo本Pa本a設置ete本(TEXT("Pa本ticleColo本"), 軍Linea本Colo本(0.95f, 0.9f, 0.7f));
        Pollen->Acti正ate(t本使e);
    }
    
    本et使本n Pollen;
}

// UI/Selection Effects
U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnSelectionRin成(const 軍Vecto本& Location, float Radi使s, 軍Linea本Colo本 Colo本)
{
    U的ia成a本aCo設置ponent* Selection = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::Selection, Location);
    
    if (Selection)
    {
        Selection->Set軍loatPa本a設置ete本(TEXT("Radi使s"), Radi使s);
        Selection->SetColo本Pa本a設置ete本(TEXT("Rin成Colo本"), Colo本);
        Selection->Set軍loatPa本a設置ete本(TEXT("Thickness"), 5.0f);
        Selection->Acti正ate(t本使e);
        On的ia成a本aEffectSpawned.B本oadcast(EMin成的ia成a本aEffectType::Selection, Selection);
    }
    
    本et使本n Selection;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnTa本成etMa本ke本(const 軍Vecto本& Location, float Scale)
{
    U的ia成a本aCo設置ponent* Ma本ke本 = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::UI, Location);
    
    if (Ma本ke本)
    {
        Ma本ke本->Set軍loatPa本a設置ete本(TEXT("Scale"), Scale);
        Ma本ke本->SetColo本Pa本a設置ete本(TEXT("Ma本ke本Colo本"), 軍Linea本Colo本(1.0f, 0.0f, 0.0f));
        Ma本ke本->Acti正ate(t本使e);
    }
    
    本et使本n Ma本ke本;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::SpawnCo設置設置andMa本ke本(const 軍Vecto本& Location, 軍Linea本Colo本 Colo本)
{
    U的ia成a本aCo設置ponent* Ma本ke本 = Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType::UI, Location);
    
    if (Ma本ke本)
    {
        Ma本ke本->Set軍loatPa本a設置ete本(TEXT("Scale"), 1.5f);
        Ma本ke本->SetColo本Pa本a設置ete本(TEXT("Ma本ke本Colo本"), Colo本);
        Ma本ke本->Set軍loatPa本a設置ete本(TEXT("D使本ation"), 2.0f);
        Ma本ke本->Acti正ate(t本使e);
    }
    
    本et使本n Ma本ke本;
}

// Utility 軍使nctions
正oid UMin成的ia成a本aEffectsSyste設置::SetEffectScale(U的ia成a本aCo設置ponent* 的ia成a本aCo設置ponent, float Scale)
{
    if (的ia成a本aCo設置ponent)
    {
        的ia成a本aCo設置ponent->Set軍loatPa本a設置ete本(TEXT("Scale"), Scale);
    }
}

正oid UMin成的ia成a本aEffectsSyste設置::SetEffectColo本(U的ia成a本aCo設置ponent* 的ia成a本aCo設置ponent, 軍Linea本Colo本 Colo本)
{
    if (的ia成a本aCo設置ponent)
    {
        的ia成a本aCo設置ponent->SetColo本Pa本a設置ete本(TEXT("Colo本"), Colo本);
    }
}

正oid UMin成的ia成a本aEffectsSyste設置::SetEffectD使本ation(U的ia成a本aCo設置ponent* 的ia成a本aCo設置ponent, float D使本ation)
{
    if (的ia成a本aCo設置ponent)
    {
        的ia成a本aCo設置ponent->Set軍loatPa本a設置ete本(TEXT("D使本ation"), D使本ation);
    }
}

正oid UMin成的ia成a本aEffectsSyste設置::StopEffect(U的ia成a本aCo設置ponent* 的ia成a本aCo設置ponent)
{
    if (的ia成a本aCo設置ponent)
    {
        的ia成a本aCo設置ponent->Deacti正ate();
    }
}

正oid UMin成的ia成a本aEffectsSyste設置::Dest本oyEffect(U的ia成a本aCo設置ponent* 的ia成a本aCo設置ponent)
{
    if (的ia成a本aCo設置ponent)
    {
        的ia成a本aCo設置ponent->Dest本oyCo設置ponent();
        Acti正eEffects.Re設置o正e(的ia成a本aCo設置ponent);
        Acti正eEffectCo使nt--;
    }
}

正oid UMin成的ia成a本aEffectsSyste設置::AttachEffectToCo設置ponent(U的ia成a本aCo設置ponent* 的ia成a本aCo設置ponent, USceneCo設置ponent* Pa本entCo設置ponent, 軍的a設置e Socket的a設置e)
{
    if (的ia成a本aCo設置ponent && Pa本entCo設置ponent)
    {
        if (Socket的a設置e != 的AME下的one)
        {
            的ia成a本aCo設置ponent->AttachToCo設置ponent(Pa本entCo設置ponent, 軍Attach設置entT本ansfo本設置R使les::SnapToTa本成et的otIncl使din成Scale, Socket的a設置e);
        }
        else
        {
            的ia成a本aCo設置ponent->AttachToCo設置ponent(Pa本entCo設置ponent, 軍Attach設置entT本ansfo本設置R使les::SnapToTa本成et的otIncl使din成Scale);
        }
    }
}

// Syste設置 Mana成e設置ent
正oid UMin成的ia成a本aEffectsSyste設置::P本eloadEffectSyste設置s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本eloadin成 的ia成a本a effect syste設置s..."));
    
    // Load all effect te設置plates
    fo本 (const a使to& Pai本 : 的ia成a本aSyste設置Paths)
    {
        // Load the 的ia成a本a syste設置 asset
        // This wo使ld 使se LoadOb大ect<U的ia成a本aSyste設置>() in act使al i設置ple設置entation
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本eloadin成: %s f本o設置 %s"), *Pai本.Key, *Pai本.Val使e);
    }
}

正oid UMin成的ia成a本aEffectsSyste設置::Clea本EffectPool()
{
    fo本 (a使to* Effect : Acti正eEffects)
    {
        if (Effect)
        {
            Effect->Deacti正ate();
            Effect->Dest本oyCo設置ponent();
        }
    }
    
    Acti正eEffects.E設置pty();
    Acti正eEffectCo使nt = 0;
}

正oid UMin成的ia成a本aEffectsSyste設置::SetMaxConc使本本entEffects(int32 MaxCo使nt)
{
    MaxConc使本本entEffects = MaxCo使nt;
    
    // Clean 使p excess effects if needed
    while (Acti正eEffects.的使設置() > MaxConc使本本entEffects)
    {
        if (Acti正eEffects[0])
        {
            Acti正eEffects[0]->Deacti正ate();
            Acti正eEffects[0]->Dest本oyCo設置ponent();
        }
        Acti正eEffects.Re設置o正eAt(0);
        Acti正eEffectCo使nt--;
    }
}

int32 UMin成的ia成a本aEffectsSyste設置::GetActi正eEffectCo使nt() const
{
    本et使本n Acti正eEffectCo使nt;
}

bool UMin成的ia成a本aEffectsSyste設置::IsEffectSyste設置Ready() const
{
    本et使本n EffectTe設置plates.的使設置() > 0;
}

// Batch Ope本ations
正oid UMin成的ia成a本aEffectsSyste設置::SpawnExplosion軍ield(const TA本本ay<軍Vecto本>& Locations, float ExplosionScale, float DelayBetween)
{
    // This wo使ld 使se a ti設置e本 to spawn explosions seq使entially
    fo本 (int32 i = 0; i < Locations.的使設置(); i++)
    {
        軍Min成ExplosionPa本a設置s Pa本a設置s;
        Pa本a設置s.Scale = ExplosionScale;
        
        // In act使al i設置ple設置entation, 使se 軍Ti設置e本Mana成e本 fo本 delays
        SpawnExplosion(Locations[i], Pa本a設置s);
    }
}

正oid UMin成的ia成a本aEffectsSyste設置::SpawnA本tille本yBa本本a成e(const 軍Vecto本& Sta本tLocation, const 軍Vecto本& EndLocation, int32 ShotCo使nt, float Inte本正al)
{
    軍Vecto本 Di本ection = (EndLocation - Sta本tLocation) / 軍Math::Max(ShotCo使nt - 1, 1);
    
    fo本 (int32 i = 0; i < ShotCo使nt; i++)
    {
        軍Vecto本 Ta本成etLocation = Sta本tLocation + Di本ection * i;
        
        // Add so設置e 本ando設置ness to i設置pact location
        Ta本成etLocation += 軍Vecto本(軍Math::RandRan成e(-100.0f, 100.0f), 軍Math::RandRan成e(-100.0f, 100.0f), 0);
        
        // In act使al i設置ple設置entation, 使se 軍Ti設置e本Mana成e本 fo本 inte本正als
        SpawnA本tille本yExplosion(Ta本成etLocation, 2.0f);
    }
}

// Inte本nal f使nctions
U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::Spawn的ia成a本aEffectAtLocation(EMin成的ia成a本aEffectType Type, const 軍Vecto本& Location, const 軍Q使at& Rotation)
{
    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld)
    {
        本et使本n n使llpt本;
    }
    
    // Check conc使本本ent li設置it
    if (Acti正eEffectCo使nt >= MaxConc使本本entEffects)
    {
        Clean使p軍inishedEffects();
        
        if (Acti正eEffectCo使nt >= MaxConc使本本entEffects)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Max conc使本本ent effects 本eached (%d)"), MaxConc使本本entEffects);
            本et使本n n使llpt本;
        }
    }
    
    U的ia成a本aSyste設置* Te設置plate = GetEffectTe設置plate(Type);
    if (!Te設置plate)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o 的ia成a本a te設置plate fo使nd fo本 effect type: %d"), static下cast<int32>(Type));
        本et使本n n使llpt本;
    }
    
    軍Acto本SpawnPa本a設置ete本s SpawnPa本a設置s;
    SpawnPa本a設置s.SpawnCollision輸入andlin成O正e本本ide = ESpawnActo本Collision輸入andlin成Method::AlwaysSpawn;
    
    U的ia成a本aCo設置ponent* 的ia成a本aCo設置ponent = U的ia成a本a軍使nctionLib本a本y::SpawnSyste設置AtLocation(
        基本o本ld,
        Te設置plate,
        Location,
        Rotation.Rotato本(),
        軍Vecto本(1.0f),
        t本使e,
        t本使e,
        E的CPoolMethod::的one,
        t本使e
    );
    
    if (的ia成a本aCo設置ponent)
    {
        Acti正eEffects.Add(的ia成a本aCo設置ponent);
        Acti正eEffectCo使nt++;
        
        // Bind to finish e正ent fo本 clean使p
        // 的ia成a本aCo設置ponent->OnSyste設置軍inished.AddDyna設置ic(this, &UMin成的ia成a本aEffectsSyste設置::OnEffect軍inished);
    }
    
    本et使本n 的ia成a本aCo設置ponent;
}

U的ia成a本aCo設置ponent* UMin成的ia成a本aEffectsSyste設置::Spawn的ia成a本aEffectAttached(EMin成的ia成a本aEffectType Type, USceneCo設置ponent* AttachCo設置ponent, 軍的a設置e Socket的a設置e)
{
    if (!AttachCo設置ponent)
    {
        本et使本n n使llpt本;
    }
    
    U的ia成a本aSyste設置* Te設置plate = GetEffectTe設置plate(Type);
    if (!Te設置plate)
    {
        本et使本n n使llpt本;
    }
    
    U的ia成a本aCo設置ponent* 的ia成a本aCo設置ponent = U的ia成a本a軍使nctionLib本a本y::SpawnSyste設置Attached(
        Te設置plate,
        AttachCo設置ponent,
        Socket的a設置e,
        軍Vecto本::Ze本oVecto本,
        軍Rotato本::Ze本oRotato本,
        EAttachLocation::SnapToTa本成et,
        t本使e,
        t本使e,
        E的CPoolMethod::的one,
        t本使e
    );
    
    if (的ia成a本aCo設置ponent)
    {
        Acti正eEffects.Add(的ia成a本aCo設置ponent);
        Acti正eEffectCo使nt++;
    }
    
    本et使本n 的ia成a本aCo設置ponent;
}

U的ia成a本aSyste設置* UMin成的ia成a本aEffectsSyste設置::GetEffectTe設置plate(EMin成的ia成a本aEffectType Type)
{
    U的ia成a本aSyste設置** 軍o使ndTe設置plate = EffectTe設置plates.軍ind(Type);
    if (軍o使ndTe設置plate && *軍o使ndTe設置plate)
    {
        本et使本n *軍o使ndTe設置plate;
    }
    
    // T本y to load f本o設置 path
    軍St本in成 PathKey;
    switch (Type)
    {
    case EMin成的ia成a本aEffectType::Explosion: PathKey = TEXT("Explosion"); b本eak;
    case EMin成的ia成a本aEffectType::M使zzle軍lash: PathKey = TEXT("M使zzle軍lash"); b本eak;
    case EMin成的ia成a本aEffectType::B使lletT本ail: PathKey = TEXT("B使lletT本ail"); b本eak;
    case EMin成的ia成a本aEffectType::I設置pact: PathKey = TEXT("I設置pact"); b本eak;
    case EMin成的ia成a本aEffectType::軍i本e: PathKey = TEXT("軍i本e"); b本eak;
    case EMin成的ia成a本aEffectType::S設置oke: PathKey = TEXT("S設置oke"); b本eak;
    case EMin成的ia成a本aEffectType::D使st: PathKey = TEXT("D使st"); b本eak;
    case EMin成的ia成a本aEffectType::Spa本k: PathKey = TEXT("Spa本k"); b本eak;
    case EMin成的ia成a本aEffectType::基本eathe本: PathKey = TEXT("Rain"); b本eak;
    case EMin成的ia成a本aEffectType::En正i本on設置ent: PathKey = TEXT("Lea正es"); b本eak;
    case EMin成的ia成a本aEffectType::UI: PathKey = TEXT("Selection"); b本eak;
    case EMin成的ia成a本aEffectType::Selection: PathKey = TEXT("Selection"); b本eak;
    defa使lt: PathKey = TEXT("Explosion"); b本eak;
    }
    
    軍St本in成* AssetPath = 的ia成a本aSyste設置Paths.軍ind(PathKey);
    if (AssetPath)
    {
        // In act使al i設置ple設置entation:
        // U的ia成a本aSyste設置* LoadedSyste設置 = LoadOb大ect<U的ia成a本aSyste設置>(n使llpt本, **AssetPath);
        // if (LoadedSyste設置)
        // {
        //     EffectTe設置plates.Add(Type, LoadedSyste設置);
        //     本et使本n LoadedSyste設置;
        // }
    }
    
    本et使本n n使llpt本;
}

正oid UMin成的ia成a本aEffectsSyste設置::OnEffect軍inished(U的ia成a本aCo設置ponent* 的ia成a本aCo設置ponent)
{
    if (的ia成a本aCo設置ponent)
    {
        Acti正eEffects.Re設置o正e(的ia成a本aCo設置ponent);
        Acti正eEffectCo使nt = 軍Math::Max(0, Acti正eEffectCo使nt - 1);
    }
}

正oid UMin成的ia成a本aEffectsSyste設置::Clean使p軍inishedEffects()
{
    fo本 (int32 i = Acti正eEffects.的使設置() - 1; i >= 0; i--)
    {
        if (!Acti正eEffects[i]  !Acti正eEffects[i]->IsActi正e())
        {
            Acti正eEffects.Re設置o正eAt(i);
            Acti正eEffectCo使nt--;
        }
    }
    
    Acti正eEffectCo使nt = 軍Math::Max(0, Acti正eEffectCo使nt);
}

U基本o本ld* UMin成的ia成a本aEffectsSyste設置::Get基本o本ld() const
{
    if (基本o本ldContext.IsValid())
    {
        本et使本n 基本o本ldContext.Get();
    }
    
    if (GEn成ine)
    {
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
