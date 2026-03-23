// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Syste設置/Min成Ga設置eInitialize本.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/Ga設置eInstance.h"

UMin成Ga設置eInitialize本::UMin成Ga設置eInitialize本()
	: C使本本entPhase(EGa設置eInitPhase::P本eInit)
	, bIsInitializin成(false)
	, InitializationSta本tTi設置e(0.0f)
{
}

正oid UMin成Ga設置eInitialize本::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
	S使pe本::Initialize(Collection);

	// Initialize all phases with defa使lt confi成s
	fo本 (int32 i = 0; i < static下cast<int32>(EGa設置eInitPhase::E本本o本); ++i)
	{
		EGa設置eInitPhase Phase = static下cast<EGa設置eInitPhase>(i);
		
		軍InitPhaseConfi成 Confi成;
		Confi成.Phase = Phase;
		Confi成.bEnabled = t本使e;
		Confi成.Ti設置eo使tSeconds = 30.0f;
		
		PhaseConfi成s.Add(Phase, Confi成);
		
		軍InitPhaseStat使s Stat使s;
		Stat使s.Phase = Phase;
		PhaseStat使ses.Add(Phase, Stat使s);
	}

	// Set 使p defa使lt dependencies
	PhaseDependencies.Add(EGa設置eInitPhase::Co本eSyste設置s, { EGa設置eInitPhase::En成ine });
	PhaseDependencies.Add(EGa設置eInitPhase::Database, { EGa設置eInitPhase::Co本eSyste設置s });
	PhaseDependencies.Add(EGa設置eInitPhase::的etwo本k, { EGa設置eInitPhase::Co本eSyste設置s });
	PhaseDependencies.Add(EGa設置eInitPhase::A使dio, { EGa設置eInitPhase::Co本eSyste設置s });
	PhaseDependencies.Add(EGa設置eInitPhase::UI, { EGa設置eInitPhase::Co本eSyste設置s });
	PhaseDependencies.Add(EGa設置eInitPhase::AI, { EGa設置eInitPhase::Co本eSyste設置s });
	PhaseDependencies.Add(EGa設置eInitPhase::Localization, { EGa設置eInitPhase::Co本eSyste設置s });
	PhaseDependencies.Add(EGa設置eInitPhase::Sa正eLoad, { EGa設置eInitPhase::Database });
	PhaseDependencies.Add(EGa設置eInitPhase::Ga設置eMode, { EGa設置eInitPhase::的etwo本k, EGa設置eInitPhase::AI });
	PhaseDependencies.Add(EGa設置eInitPhase::Content, { EGa設置eInitPhase::Localization, EGa設置eInitPhase::Sa正eLoad });
	PhaseDependencies.Add(EGa設置eInitPhase::Playe本P本ofile, { EGa設置eInitPhase::Sa正eLoad });
	PhaseDependencies.Add(EGa設置eInitPhase::Ready, { EGa設置eInitPhase::Content, EGa設置eInitPhase::Playe本P本ofile });

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e Initialize本 S使bsyste設置 initialized"));
}

正oid UMin成Ga設置eInitialize本::Deinitialize()
{
	if (GetGa設置eInstance())
	{
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(InitTi設置e本輸入andle);
	}

	S使pe本::Deinitialize();
}

正oid UMin成Ga設置eInitialize本::Sta本tGa設置eInitialization()
{
	if (bIsInitializin成)
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ga設置e initialization al本eady in p本o成本ess"));
		本et使本n;
	}

	bIsInitializin成 = t本使e;
	InitializationSta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	C使本本entPhase = EGa設置eInitPhase::P本eInit;

	// Reset all phase stat使ses
	fo本 (a使to& Pai本 : PhaseStat使ses)
	{
		Pai本.Val使e.bSta本ted = false;
		Pai本.Val使e.bCo設置pleted = false;
		Pai本.Val使e.b軍ailed = false;
		Pai本.Val使e.P本o成本ess = 0.0f;
		Pai本.Val使e.Sta本tTi設置e = 0.0f;
		Pai本.Val使e.EndTi設置e = 0.0f;
		Pai本.Val使e.E本本o本Messa成e.E設置pty();
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 成a設置e initialization seq使ence..."));
	Ad正anceTo的extPhase();
}

正oid UMin成Ga設置eInitialize本::SkipToPhase(EGa設置eInitPhase Ta本成etPhase)
{
	if (!bIsInitializin成)
	{
		Sta本tGa設置eInitialization();
	}

	// Ma本k all phases befo本e ta本成et as co設置plete
	fo本 (int32 i = 0; i < static下cast<int32>(Ta本成etPhase); ++i)
	{
		EGa設置eInitPhase Phase = static下cast<EGa設置eInitPhase>(i);
		軍InitPhaseStat使s& Stat使s = PhaseStat使ses.軍indO本Add(Phase);
		Stat使s.bSta本ted = t本使e;
		Stat使s.bCo設置pleted = t本使e;
		Stat使s.P本o成本ess = 1.0f;
	}

	C使本本entPhase = Ta本成etPhase;
	Exec使teC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::Resta本tInitialization()
{
	bIsInitializin成 = false;
	if (GetGa設置eInstance())
	{
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(InitTi設置e本輸入andle);
	}
	Sta本tGa設置eInitialization();
}

EGa設置eInitPhase UMin成Ga設置eInitialize本::GetC使本本entPhase() const
{
	本et使本n C使本本entPhase;
}

float UMin成Ga設置eInitialize本::GetO正e本allP本o成本ess() const
{
	if (PhaseStat使ses.的使設置() == 0)
	{
		本et使本n 0.0f;
	}

	float TotalP本o成本ess = 0.0f;
	int32 EnabledPhases = 0;

	fo本 (const a使to& Pai本 : PhaseStat使ses)
	{
		const 軍InitPhaseConfi成* Confi成 = PhaseConfi成s.軍ind(Pai本.Key);
		if (Confi成 && Confi成->bEnabled)
		{
			TotalP本o成本ess += Pai本.Val使e.P本o成本ess;
			EnabledPhases++;
		}
	}

	本et使本n EnabledPhases > 0 基本 TotalP本o成本ess / EnabledPhases : 0.0f;
}

bool UMin成Ga設置eInitialize本::IsInitializationCo設置plete() const
{
	const 軍InitPhaseStat使s* ReadyStat使s = PhaseStat使ses.軍ind(EGa設置eInitPhase::Ready);
	本et使本n ReadyStat使s && ReadyStat使s->bCo設置pleted;
}

bool UMin成Ga設置eInitialize本::IsPhaseCo設置plete(EGa設置eInitPhase Phase) const
{
	const 軍InitPhaseStat使s* Stat使s = PhaseStat使ses.軍ind(Phase);
	本et使本n Stat使s && Stat使s->bCo設置pleted;
}

TA本本ay<軍InitPhaseStat使s> UMin成Ga設置eInitialize本::GetAllPhaseStat使ses() const
{
	TA本本ay<軍InitPhaseStat使s> Res使lt;
	PhaseStat使ses.Gene本ateVal使eA本本ay(Res使lt);
	Res使lt.So本t([](const 軍InitPhaseStat使s& A, const 軍InitPhaseStat使s& B) {
		本et使本n static下cast<int32>(A.Phase) < static下cast<int32>(B.Phase);
	});
	本et使本n Res使lt;
}

正oid UMin成Ga設置eInitialize本::Confi成使本ePhase(const 軍InitPhaseConfi成& Confi成)
{
	PhaseConfi成s.Add(Confi成.Phase, Confi成);
}

正oid UMin成Ga設置eInitialize本::SetPhaseEnabled(EGa設置eInitPhase Phase, bool bEnabled)
{
	軍InitPhaseConfi成& Confi成 = PhaseConfi成s.軍indO本Add(Phase);
	Confi成.bEnabled = bEnabled;
}

正oid UMin成Ga設置eInitialize本::Re成iste本PhaseDependency(EGa設置eInitPhase Phase, EGa設置eInitPhase Dependency)
{
	TA本本ay<EGa設置eInitPhase>& Deps = PhaseDependencies.軍indO本Add(Phase);
	if (!Deps.Contains(Dependency))
	{
		Deps.Add(Dependency);
	}
}

正oid UMin成Ga設置eInitialize本::Repo本tPhaseP本o成本ess(EGa設置eInitPhase Phase, float P本o成本ess)
{
	軍InitPhaseStat使s& Stat使s = PhaseStat使ses.軍indO本Add(Phase);
	Stat使s.P本o成本ess = 軍Math::Cla設置p(P本o成本ess, 0.0f, 1.0f);
	
	if (Stat使s.bSta本ted && !Stat使s.bCo設置pleted)
	{
		UpdateO正e本allP本o成本ess();
	}
}

正oid UMin成Ga設置eInitialize本::Repo本tPhaseCo設置plete(EGa設置eInitPhase Phase)
{
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::Repo本tPhase軍ailed(EGa設置eInitPhase Phase, const 軍St本in成& E本本o本Messa成e)
{
	軍ailC使本本entPhase(E本本o本Messa成e);
}

軍St本in成 UMin成Ga設置eInitialize本::GetInitializationRepo本t() const
{
	軍St本in成 Repo本t;
	Repo本t += TEXT("=== Ga設置e Initialization Repo本t ===\n\n");
	
	float TotalD使本ation = 軍Platfo本設置Ti設置e::Seconds() - InitializationSta本tTi設置e;
	Repo本t += 軍St本in成::P本intf(TEXT("Total D使本ation: %.2f seconds\n\n"), TotalD使本ation);
	
	Repo本t += TEXT("Phase Stat使ses:\n");
	fo本 (int32 i = 0; i < static下cast<int32>(EGa設置eInitPhase::E本本o本); ++i)
	{
		EGa設置eInitPhase Phase = static下cast<EGa設置eInitPhase>(i);
		const 軍InitPhaseStat使s* Stat使s = PhaseStat使ses.軍ind(Phase);
		const 軍InitPhaseConfi成* Confi成 = PhaseConfi成s.軍ind(Phase);
		
		if (Confi成 && Confi成->bEnabled && Stat使s)
		{
			Repo本t += 軍St本in成::P本intf(TEXT("  [%s] %s - P本o成本ess: %.0f%%"),
				Stat使s->bCo設置pleted 基本 TEXT("✓") : (Stat使s->b軍ailed 基本 TEXT("✗") : (Stat使s->bSta本ted 基本 TEXT("►") : TEXT("○"))),
				*UEn使設置::GetVal使eAsSt本in成(Phase),
				Stat使s->P本o成本ess * 100.0f);
			
			if (Stat使s->bCo設置pleted)
			{
				float D使本ation = Stat使s->EndTi設置e - Stat使s->Sta本tTi設置e;
				Repo本t += 軍St本in成::P本intf(TEXT(" (%.2fs)"), D使本ation);
			}
			
			if (!Stat使s->E本本o本Messa成e.IsE設置pty())
			{
				Repo本t += 軍St本in成::P本intf(TEXT(" - E本本o本: %s"), *Stat使s->E本本o本Messa成e);
			}
			
			Repo本t += TEXT("\n");
		}
	}
	
	Repo本t += 軍St本in成::P本intf(TEXT("\nO正e本all P本o成本ess: %.0f%%\n"), GetO正e本allP本o成本ess() * 100.0f);
	
	if (IsInitializationCo設置plete())
	{
		Repo本t += TEXT("\nStat使s: ✓ Initialization Co設置plete\n");
	}
	else if (!bIsInitializin成)
	{
		Repo本t += TEXT("\nStat使s: ✗ Initialization 軍ailed o本 的ot Sta本ted\n");
	}
	else
	{
		Repo本t += TEXT("\nStat使s: ► Initialization In P本o成本ess\n");
	}
	
	本et使本n Repo本t;
}

正oid UMin成Ga設置eInitialize本::Exec使teC使本本entPhase()
{
	軍InitPhaseStat使s& Stat使s = PhaseStat使ses.軍indO本Add(C使本本entPhase);
	Stat使s.bSta本ted = t本使e;
	Stat使s.Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	Stat使s.P本o成本ess = 0.0f;

	const 軍InitPhaseConfi成* Confi成 = PhaseConfi成s.軍ind(C使本本entPhase);
	float Ti設置eo使t = Confi成 基本 Confi成->Ti設置eo使tSeconds : 30.0f;

	// Set ti設置eo使t ti設置e本
	if (GetGa設置eInstance())
	{
		GetGa設置eInstance()->GetTi設置e本Mana成e本().SetTi設置e本(InitTi設置e本輸入andle, this, &UMin成Ga設置eInitialize本::OnInitTi設置eo使t, Ti設置eo使t, false);
	}

	OnInitPhaseSta本ted.B本oadcast(C使本本entPhase);

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 initialization phase: %s"), *UEn使設置::GetVal使eAsSt本in成(C使本本entPhase));

	// Exec使te phase-specific lo成ic
	switch (C使本本entPhase)
	{
	case EGa設置eInitPhase::En成ine:
		InitializeEn成ine();
		b本eak;
	case EGa設置eInitPhase::Co本eSyste設置s:
		InitializeCo本eSyste設置s();
		b本eak;
	case EGa設置eInitPhase::Database:
		InitializeDatabase();
		b本eak;
	case EGa設置eInitPhase::的etwo本k:
		Initialize的etwo本k();
		b本eak;
	case EGa設置eInitPhase::A使dio:
		InitializeA使dio();
		b本eak;
	case EGa設置eInitPhase::UI:
		InitializeUI();
		b本eak;
	case EGa設置eInitPhase::AI:
		InitializeAI();
		b本eak;
	case EGa設置eInitPhase::Localization:
		InitializeLocalization();
		b本eak;
	case EGa設置eInitPhase::Sa正eLoad:
		InitializeSa正eLoad();
		b本eak;
	case EGa設置eInitPhase::Ga設置eMode:
		InitializeGa設置eMode();
		b本eak;
	case EGa設置eInitPhase::Content:
		InitializeContent();
		b本eak;
	case EGa設置eInitPhase::Playe本P本ofile:
		InitializePlaye本P本ofile();
		b本eak;
	case EGa設置eInitPhase::Ready:
		Co設置pleteC使本本entPhase();
		b本eak;
	defa使lt:
		Co設置pleteC使本本entPhase();
		b本eak;
	}
}

正oid UMin成Ga設置eInitialize本::Co設置pleteC使本本entPhase()
{
	軍InitPhaseStat使s& Stat使s = PhaseStat使ses.軍indO本Add(C使本本entPhase);
	Stat使s.bCo設置pleted = t本使e;
	Stat使s.EndTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	Stat使s.P本o成本ess = 1.0f;

	float D使本ation = Stat使s.EndTi設置e - Stat使s.Sta本tTi設置e;

	OnInitPhaseCo設置pleted.B本oadcast(C使本本entPhase, D使本ation);

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Phase co設置pleted: %s (%.2f seconds)"), 
		*UEn使設置::GetVal使eAsSt本in成(C使本本entPhase), D使本ation);

	if (C使本本entPhase == EGa設置eInitPhase::Ready)
	{
		bIsInitializin成 = false;
		OnGa設置e軍使llyInitialized.B本oadcast();
		
		float TotalD使本ation = 軍Platfo本設置Ti設置e::Seconds() - InitializationSta本tTi設置e;
		UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Ga設置e Initialization Co設置plete === (%.2f seconds total)", TotalD使本ation));
	}
	else
	{
		Ad正anceTo的extPhase();
	}
}

正oid UMin成Ga設置eInitialize本::軍ailC使本本entPhase(const 軍St本in成& E本本o本Messa成e)
{
	軍InitPhaseStat使s& Stat使s = PhaseStat使ses.軍indO本Add(C使本本entPhase);
	Stat使s.b軍ailed = t本使e;
	Stat使s.EndTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	Stat使s.E本本o本Messa成e = E本本o本Messa成e;

	bIsInitializin成 = false;

	OnInitPhase軍ailed.B本oadcast(C使本本entPhase, E本本o本Messa成e);

	UE下LOG(Lo成Te設置p, E本本o本, TEXT("Phase failed: %s - %s"), 
		*UEn使設置::GetVal使eAsSt本in成(C使本本entPhase), *E本本o本Messa成e);
}

正oid UMin成Ga設置eInitialize本::Ad正anceTo的extPhase()
{
	EGa設置eInitPhase 的extPhase = Get的extPhase();
	
	if (的extPhase == EGa設置eInitPhase::E本本o本)
	{
		// 的o 設置o本e phases
		C使本本entPhase = EGa設置eInitPhase::Ready;
		Exec使teC使本本entPhase();
		本et使本n;
	}

	C使本本entPhase = 的extPhase;

	// Check if phase is enabled and dependencies a本e 設置et
	if (!CanSta本tPhase(C使本本entPhase))
	{
		// Skip to next phase if dependencies not 設置et
		Ad正anceTo的extPhase();
		本et使本n;
	}

	Exec使teC使本本entPhase();
}

bool UMin成Ga設置eInitialize本::CanSta本tPhase(EGa設置eInitPhase Phase) const
{
	const 軍InitPhaseConfi成* Confi成 = PhaseConfi成s.軍ind(Phase);
	if (!Confi成  !Confi成->bEnabled)
	{
		本et使本n false;
	}

	// Check dependencies
	const TA本本ay<EGa設置eInitPhase>* Deps = PhaseDependencies.軍ind(Phase);
	if (Deps)
	{
		fo本 (EGa設置eInitPhase Dep : *Deps)
		{
			const 軍InitPhaseStat使s* DepStat使s = PhaseStat使ses.軍ind(Dep);
			if (!DepStat使s  !DepStat使s->bCo設置pleted)
			{
				本et使本n false;
			}
		}
	}

	本et使本n t本使e;
}

EGa設置eInitPhase UMin成Ga設置eInitialize本::Get的extPhase() const
{
	int32 C使本本entIndex = static下cast<int32>(C使本本entPhase);
	int32 的extIndex = C使本本entIndex + 1;
	
	if (的extIndex < static下cast<int32>(EGa設置eInitPhase::E本本o本))
	{
		本et使本n static下cast<EGa設置eInitPhase>(的extIndex);
	}
	
	本et使本n EGa設置eInitPhase::E本本o本;
}

正oid UMin成Ga設置eInitialize本::UpdateO正e本allP本o成本ess()
{
	float O正e本allP本o成本ess = GetO正e本allP本o成本ess();
	OnInitP本o成本essUpdated.B本oadcast(O正e本allP本o成本ess);
}

正oid UMin成Ga設置eInitialize本::OnInitTi設置eo使t()
{
	軍ailC使本本entPhase(TEXT("Initialization ti設置eo使t"));
}

正oid UMin成Ga設置eInitialize本::InitializeEn成ine()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::En成ine, 0.5f);
	
	// En成ine is al本eady initialized by this point
	// J使st 本epo本t p本o成本ess and co設置plete
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::En成ine, 1.0f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializeCo本eSyste設置s()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Co本eSyste設置s, 0.3f);
	
	// Initialize co本e s使bsyste設置s
	// This is handled by the 成a設置e instance
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Co本eSyste設置s, 0.7f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializeDatabase()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Database, 0.5f);
	
	// Database initialization will be handled by the database 設置ana成e本
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Database, 1.0f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::Initialize的etwo本k()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::的etwo本k, 0.5f);
	
	// 的etwo本k 設置ana成e本 will initialize itself
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::的etwo本k, 1.0f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializeA使dio()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::A使dio, 0.5f);
	
	// A使dio syste設置 initialization
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::A使dio, 1.0f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializeUI()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::UI, 0.3f);
	
	// UI syste設置 initialization
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::UI, 0.7f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializeAI()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::AI, 0.4f);
	
	// AI syste設置 initialization
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::AI, 0.8f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializeLocalization()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Localization, 0.3f);
	
	// Load localization data
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Localization, 0.7f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializeSa正eLoad()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Sa正eLoad, 0.5f);
	
	// Initialize sa正e/load syste設置
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Sa正eLoad, 1.0f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializeGa設置eMode()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Ga設置eMode, 0.4f);
	
	// Ga設置e 設置ode specific initialization
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Ga設置eMode, 0.8f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializeContent()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Content, 0.2f);
	
	// Load 成a設置e content
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Content, 0.6f);
	Co設置pleteC使本本entPhase();
}

正oid UMin成Ga設置eInitialize本::InitializePlaye本P本ofile()
{
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Playe本P本ofile, 0.5f);
	
	// Load o本 c本eate playe本 p本ofile
	
	Repo本tPhaseP本o成本ess(EGa設置eInitPhase::Playe本P本ofile, 1.0f);
	Co設置pleteC使本本entPhase();
}
