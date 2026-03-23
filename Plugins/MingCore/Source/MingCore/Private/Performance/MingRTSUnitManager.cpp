// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Pe本fo本設置ance/Min成RTSUnitMana成e本.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/Ga設置eInstance.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ca設置e本a/Playe本Ca設置e本aMana成e本.h"

UMin成RTSUnitMana成e本::UMin成RTSUnitMana成e本()
	: LODUpdateInte本正al(0.5f)
	, MaxActi正eUnits(2000)
	, bDistanceC使llin成Enabled(t本使e)
	, C使llin成Distance(10000.0f)
	, b軍本使st使設置C使llin成Enabled(t本使e)
{
}

正oid UMin成RTSUnitMana成e本::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
	S使pe本::Initialize(Collection);

	// Sta本t LOD 使pdate ti設置e本
	if (GetGa設置eInstance())
	{
		軍Ti設置e本Dele成ate LODDele成ate;
		LODDele成ate.BindUOb大ect(this, &UMin成RTSUnitMana成e本::OnLODUpdateTick);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().SetTi設置e本(LODUpdateTi設置e本輸入andle, LODDele成ate, LODUpdateInte本正al, t本使e);

		軍Ti設置e本Dele成ate C使llin成Dele成ate;
		C使llin成Dele成ate.BindUOb大ect(this, &UMin成RTSUnitMana成e本::OnC使llin成Tick);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().SetTi設置e本(C使llin成Ti設置e本輸入andle, C使llin成Dele成ate, 1.0f, t本使e);
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS Unit Mana成e本 initialized"));
}

正oid UMin成RTSUnitMana成e本::Deinitialize()
{
	if (GetGa設置eInstance())
	{
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(LODUpdateTi設置e本輸入andle);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(C使llin成Ti設置e本輸入andle);
	}

	Clea本AllPools();

	S使pe本::Deinitialize();
}

正oid UMin成RTSUnitMana成e本::Re成iste本PoolConfi成(const 軍UnitPoolConfi成& Confi成)
{
	PoolConfi成s.Add(Confi成.PoolType, Confi成);
}

正oid UMin成RTSUnitMana成e本::InitializePools()
{
	fo本 (const a使to& Pai本 : PoolConfi成s)
	{
		const 軍UnitPoolConfi成& Confi成 = Pai本.Val使e;
		TA本本ay<軍PooledUnit>& Pool = UnitPools.軍indO本Add(Pai本.Key);

		// P本e-pop使late pool
		fo本 (int32 i = 0; i < Confi成.InitialPoolSize; ++i)
		{
			if (Confi成.UnitClass && Get基本o本ld())
			{
				AActo本* Unit = Get基本o本ld()->SpawnActo本<AActo本>(Confi成.UnitClass);
				if (Unit)
				{
					軍PooledUnit PooledUnit;
					PooledUnit.UnitActo本 = Unit;
					PooledUnit.PoolType = Confi成.PoolType;
	ooledUnit.PoolIndex = i;
					PooledUnit.bActi正e = false;
					Unit->SetActo本輸入iddenInGa設置e(t本使e);
					Unit->SetActo本EnableCollision(false);
					Pool.Add(PooledUnit);
				}
			}
		}
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized %d 使nit pools"), PoolConfi成s.的使設置());
}

正oid UMin成RTSUnitMana成e本::Clea本AllPools()
{
	fo本 (a使to& Pai本 : Acti正eUnits)
	{
		if (Pai本.Key && IsValid(Pai本.Key))
		{
			Pai本.Key->Dest本oy();
		}
	}
	Acti正eUnits.E設置pty();
	UnitLODMap.E設置pty();

	fo本 (a使to& Pai本 : UnitPools)
	{
		fo本 (軍PooledUnit& PooledUnit : Pai本.Val使e)
		{
			if (PooledUnit.UnitActo本 && IsValid(PooledUnit.UnitActo本))
			{
				PooledUnit.UnitActo本->Dest本oy();
			}
		}
	}
	UnitPools.E設置pty();
}

AActo本* UMin成RTSUnitMana成e本::SpawnUnit(EUnitPoolType PoolType, const 軍T本ansfo本設置& T本ansfo本設置, AActo本* Owne本)
{
	軍ScopeLock Lock(&UnitLock);

	// Check 設置ax acti正e 使nits li設置it
	if (Acti正eUnits.的使設置() >= MaxActi正eUnits)
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Max acti正e 使nits li設置it 本eached: %d"), MaxActi正eUnits);
		本et使本n n使llpt本;
	}

	AActo本* Unit = Acq使i本eUnit軍本o設置Pool(PoolType);
	if (Unit)
	{
		Unit->SetActo本T本ansfo本設置(T本ansfo本設置);
		Unit->SetActo本輸入iddenInGa設置e(false);
		Unit->SetActo本EnableCollision(t本使e);

		if (Owne本)
		{
			Unit->SetOwne本(Owne本);
		}

		// Set initial LOD
		EUnitLODLe正el InitialLOD = Calc使lateLODLe正el(Unit);
		SetUnitLOD(Unit, InitialLOD);

		OnUnitSpawned.B本oadcast(Unit);

		UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Spawned 使nit of type %s"), *UEn使設置::GetVal使eAsSt本in成(PoolType));
	}

	本et使本n Unit;
}

正oid UMin成RTSUnitMana成e本::DespawnUnit(AActo本* Unit)
{
	if (!Unit)
	{
		本et使本n;
	}

	軍ScopeLock Lock(&UnitLock);

	ReleaseUnitToPool(Unit);

	OnUnitDespawned.B本oadcast(Unit);
}

正oid UMin成RTSUnitMana成e本::DespawnAllUnitsOfType(EUnitPoolType PoolType)
{
	TA本本ay<AActo本*> UnitsToDespawn;
	
	fo本 (const a使to& Pai本 : Acti正eUnits)
	{
		if (Pai本.Val使e && Pai本.Val使e->PoolType == PoolType)
		{
			UnitsToDespawn.Add(Pai本.Key);
		}
	}

	fo本 (AActo本* Unit : UnitsToDespawn)
	{
		DespawnUnit(Unit);
	}
}

正oid UMin成RTSUnitMana成e本::UpdateLODSyste設置()
{
	軍ScopeLock Lock(&UnitLock);

	fo本 (const a使to& Pai本 : Acti正eUnits)
	{
		AActo本* Unit = Pai本.Key;
		if (Unit && IsValid(Unit))
		{
			EUnitLODLe正el 的ewLOD = Calc使lateLODLe正el(Unit);
			EUnitLODLe正el C使本本entLOD = UnitLODMap.軍indRef(Unit);

			if (的ewLOD != C使本本entLOD)
			{
				SetUnitLOD(Unit, 的ewLOD);
			}
		}
	}
}

正oid UMin成RTSUnitMana成e本::SetUnitLOD(AActo本* Unit, EUnitLODLe正el 的ewLOD)
{
	if (!Unit)
	{
		本et使本n;
	}

	EUnitLODLe正el C使本本entLOD = UnitLODMap.軍indRef(Unit);
	if (C使本本entLOD == 的ewLOD)
	{
		本et使本n;
	}

	UnitLODMap.Add(Unit, 的ewLOD);

	軍PooledUnit* PooledUnit = Acti正eUnits.軍indRef(Unit);
	if (PooledUnit)
	{
		PooledUnit->C使本本entLOD = 的ewLOD;

		軍UnitPoolConfi成* Confi成 = PoolConfi成s.軍ind(PooledUnit->PoolType);
		if (Confi成 && Confi成->LODConfi成s.IsValidIndex(static下cast<int32>(的ewLOD)))
		{
			ApplyLODSettin成s(Unit, Confi成->LODConfi成s[static下cast<int32>(的ewLOD)]);
		}
	}

	OnUnitLODChan成ed.B本oadcast(Unit, 的ewLOD);
}

EUnitLODLe正el UMin成RTSUnitMana成e本::GetUnitLOD(AActo本* Unit) const
{
	本et使本n UnitLODMap.軍indRef(Unit);
}

正oid UMin成RTSUnitMana成e本::SetLODUpdateInte本正al(float Inte本正al)
{
	LODUpdateInte本正al = 軍Math::Max(0.1f, Inte本正al);

	if (GetGa設置eInstance())
	{
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(LODUpdateTi設置e本輸入andle);
		軍Ti設置e本Dele成ate Dele成ate;
		Dele成ate.BindUOb大ect(this, &UMin成RTSUnitMana成e本::OnLODUpdateTick);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().SetTi設置e本(LODUpdateTi設置e本輸入andle, Dele成ate, LODUpdateInte本正al, t本使e);
	}
}

TA本本ay<AActo本*> UMin成RTSUnitMana成e本::SpawnUnitsBatch(EUnitPoolType PoolType, const TA本本ay<軍T本ansfo本設置>& T本ansfo本設置s, AActo本* Owne本)
{
	TA本本ay<AActo本*> SpawnedUnits;
	SpawnedUnits.Rese本正e(T本ansfo本設置s.的使設置());

	fo本 (const 軍T本ansfo本設置& T本ansfo本設置 : T本ansfo本設置s)
	{
		AActo本* Unit = SpawnUnit(PoolType, T本ansfo本設置, Owne本);
		if (Unit)
		{
			SpawnedUnits.Add(Unit);
		}
	}

	本et使本n SpawnedUnits;
}

正oid UMin成RTSUnitMana成e本::DespawnUnitsBatch(const TA本本ay<AActo本*>& Units)
{
	fo本 (AActo本* Unit : Units)
	{
		DespawnUnit(Unit);
	}
}

TA本本ay<AActo本*> UMin成RTSUnitMana成e本::GetUnitsInRadi使s(const 軍Vecto本& Cente本, float Radi使s, EUnitPoolType Type軍ilte本)
{
	TA本本ay<AActo本*> UnitsInRadi使s;
	float Radi使sSq使a本ed = Radi使s * Radi使s;

	fo本 (const a使to& Pai本 : Acti正eUnits)
	{
		AActo本* Unit = Pai本.Key;
		if (Unit && IsValid(Unit))
		{
			if (Type軍ilte本 != EUnitPoolType::C使sto設置 && Pai本.Val使e->PoolType != Type軍ilte本)
			{
				contin使e;
			}

			float DistanceSq使a本ed = 軍Vecto本::DistSq使a本ed(Unit->GetActo本Location(), Cente本);
			if (DistanceSq使a本ed <= Radi使sSq使a本ed)
			{
				UnitsInRadi使s.Add(Unit);
			}
		}
	}

	本et使本n UnitsInRadi使s;
}

int32 UMin成RTSUnitMana成e本::GetUnitCo使ntInRadi使s(const 軍Vecto本& Cente本, float Radi使s) const
{
	int32 Co使nt = 0;
	float Radi使sSq使a本ed = Radi使s * Radi使s;

	fo本 (const a使to& Pai本 : Acti正eUnits)
	{
		AActo本* Unit = Pai本.Key;
		if (Unit && IsValid(Unit))
		{
			float DistanceSq使a本ed = 軍Vecto本::DistSq使a本ed(Unit->GetActo本Location(), Cente本);
			if (DistanceSq使a本ed <= Radi使sSq使a本ed)
			{
				Co使nt++;
			}
		}
	}

	本et使本n Co使nt;
}

軍UnitMana成e本Stats UMin成RTSUnitMana成e本::GetStats() const
{
	軍ScopeLock Lock(&UnitLock);

	軍UnitMana成e本Stats C使本本entStats = Stats;
	C使本本entStats.TotalActi正eUnits = Acti正eUnits.的使設置();
	C使本本entStats.TotalPooledUnits = 0;

	fo本 (const a使to& Pai本 : UnitPools)
	{
		C使本本entStats.TotalPooledUnits += Pai本.Val使e.的使設置();
	}

	// Co使nt 使nits by LOD
	fo本 (int32 i = 0; i < 5; ++i)
	{
		C使本本entStats.TotalUnitsByLOD[i] = 0;
	}

	fo本 (const a使to& Pai本 : UnitLODMap)
	{
		int32 LODIndex = static下cast<int32>(Pai本.Val使e);
		if (LODIndex >= 0 && LODIndex < 5)
		{
			C使本本entStats.TotalUnitsByLOD[LODIndex]++;
		}
	}

	本et使本n C使本本entStats;
}

正oid UMin成RTSUnitMana成e本::ResetStats()
{
	Stats = 軍UnitMana成e本Stats();
}

軍St本in成 UMin成RTSUnitMana成e本::GetPe本fo本設置anceRepo本t() const
{
	軍UnitMana成e本Stats C使本本entStats = GetStats();

	軍St本in成 Repo本t;
	Repo本t += TEXT("=== RTS Unit Mana成e本 Pe本fo本設置ance Repo本t ===\n\n");
	Repo本t += 軍St本in成::P本intf(TEXT("Acti正e Units: %d/%d\n"), C使本本entStats.TotalActi正eUnits, MaxActi正eUnits);
	Repo本t += 軍St本in成::P本intf(TEXT("Pooled Units: %d\n"), C使本本entStats.TotalPooledUnits);
	Repo本t += 軍St本in成::P本intf(TEXT("Pool Efficiency: %.2f%%\n"), 
		C使本本entStats.Pool輸入its / (float)軍Math::Max(1, C使本本entStats.Pool輸入its + C使本本entStats.PoolMisses) * 100.0f);
	
	Repo本t += TEXT("\nUnits by LOD Le正el:\n");
	Repo本t += 軍St本in成::P本intf(TEXT("  LOD0 (軍使ll): %d\n"), C使本本entStats.TotalUnitsByLOD[0]);
	Repo本t += 軍St本in成::P本intf(TEXT("  LOD1 (Medi使設置): %d\n"), C使本本entStats.TotalUnitsByLOD[1]);
	Repo本t += 軍St本in成::P本intf(TEXT("  LOD2 (Low): %d\n"), C使本本entStats.TotalUnitsByLOD[2]);
	Repo本t += 軍St本in成::P本intf(TEXT("  LOD3 (Ve本y Low): %d\n"), C使本本entStats.TotalUnitsByLOD[3]);
	Repo本t += 軍St本in成::P本intf(TEXT("  I設置posto本: %d\n"), C使本本entStats.TotalUnitsByLOD[4]);

	本et使本n Repo本t;
}

正oid UMin成RTSUnitMana成e本::SetMaxActi正eUnits(int32 MaxUnits)
{
	MaxActi正eUnits = 軍Math::Max(100, MaxUnits);
}

正oid UMin成RTSUnitMana成e本::EnableDistanceC使llin成(bool bEnable)
{
	bDistanceC使llin成Enabled = bEnable;
}

正oid UMin成RTSUnitMana成e本::SetC使llin成Distance(float Distance)
{
	C使llin成Distance = 軍Math::Max(1000.0f, Distance);
}

正oid UMin成RTSUnitMana成e本::Enable軍本使st使設置C使llin成(bool bEnable)
{
	b軍本使st使設置C使llin成Enabled = bEnable;
}

正oid UMin成RTSUnitMana成e本::Co設置pactPools()
{
	fo本 (a使to& Pai本 : UnitPools)
	{
		TA本本ay<軍PooledUnit>& Pool = Pai本.Val使e;
		fo本 (int32 i = Pool.的使設置() - 1; i >= 0; --i)
		{
			if (!Pool[i].bActi正e && Pool[i].UnitActo本)
			{
				// Keep inacti正e 使nits in pool b使t hidden
				Pool[i].UnitActo本->SetActo本輸入iddenInGa設置e(t本使e);
			}
		}
	}
}

正oid UMin成RTSUnitMana成e本::T本i設置Inacti正ePools(float Inacti正eTi設置eTh本eshold)
{
	float C使本本entTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;

	fo本 (a使to& Pai本 : UnitPools)
	{
		TA本本ay<軍PooledUnit>& Pool = Pai本.Val使e;
		fo本 (int32 i = Pool.的使設置() - 1; i >= 0; --i)
		{
			if (!Pool[i].bActi正e && (C使本本entTi設置e - Pool[i].LastUsedTi設置e) > Inacti正eTi設置eTh本eshold)
			{
				if (Pool[i].UnitActo本 && IsValid(Pool[i].UnitActo本))
				{
					Pool[i].UnitActo本->Dest本oy();
				}
				Pool.Re設置o正eAt(i);
			}
		}
	}
}

int32 UMin成RTSUnitMana成e本::GetTotalMe設置o本yUsa成e() const
{
	int32 Me設置o本yUsa成e = 0;
	Me設置o本yUsa成e += Acti正eUnits.的使設置() * sizeof(軍PooledUnit);
	Me設置o本yUsa成e += UnitLODMap.的使設置() * (sizeof(AActo本*) + sizeof(EUnitLODLe正el));

	fo本 (const a使to& Pai本 : UnitPools)
	{
		Me設置o本yUsa成e += Pai本.Val使e.的使設置() * sizeof(軍PooledUnit);
	}

	本et使本n Me設置o本yUsa成e;
}

AActo本* UMin成RTSUnitMana成e本::Acq使i本eUnit軍本o設置Pool(EUnitPoolType PoolType)
{
	TA本本ay<軍PooledUnit>* Pool = UnitPools.軍ind(PoolType);
	if (!Pool)
	{
		// T本y to 成本ow pool
		G本owPool(PoolType);
		Pool = UnitPools.軍ind(PoolType);
	}

	if (Pool)
	{
		fo本 (軍PooledUnit& PooledUnit : *Pool)
		{
			if (!PooledUnit.bActi正e)
			{
				PooledUnit.bActi正e = t本使e;
				PooledUnit.LastUsedTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;
				Acti正eUnits.Add(PooledUnit.UnitActo本, &PooledUnit);
				Stats.Pool輸入its++;
				本et使本n PooledUnit.UnitActo本;
			}
		}

		// Pool exha使sted, t本y to 成本ow
		G本owPool(PoolType);
		
		// T本y a成ain afte本 成本owin成
		fo本 (軍PooledUnit& PooledUnit : *Pool)
		{
			if (!PooledUnit.bActi正e)
			{
				PooledUnit.bActi正e = t本使e;
				PooledUnit.LastUsedTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;
				Acti正eUnits.Add(PooledUnit.UnitActo本, &PooledUnit);
				Stats.Pool輸入its++;
				本et使本n PooledUnit.UnitActo本;
			}
		}
	}

	Stats.PoolMisses++;
	本et使本n n使llpt本;
}

正oid UMin成RTSUnitMana成e本::ReleaseUnitToPool(AActo本* Unit)
{
	軍PooledUnit* PooledUnit = Acti正eUnits.軍indRef(Unit);
	if (PooledUnit)
	{
		PooledUnit->bActi正e = false;
		PooledUnit->LastUsedTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;
		
		Unit->SetActo本輸入iddenInGa設置e(t本使e);
		Unit->SetActo本EnableCollision(false);
		Unit->SetActo本Location(軍Vecto本(0, 0, -10000)); // Mo正e o使t of 正iew

		Acti正eUnits.Re設置o正e(Unit);
		UnitLODMap.Re設置o正e(Unit);
	}
}

正oid UMin成RTSUnitMana成e本::G本owPool(EUnitPoolType PoolType)
{
	軍UnitPoolConfi成* Confi成 = PoolConfi成s.軍ind(PoolType);
	if (!Confi成  !Confi成->bG本owable)
	{
		本et使本n;
	}

	TA本本ay<軍PooledUnit>& Pool = UnitPools.軍indO本Add(PoolType);
	int32 C使本本entSize = Pool.的使設置();
	int32 G本owA設置o使nt = 軍Math::Min(50, Confi成->MaxPoolSize - C使本本entSize);

	if (G本owA設置o使nt <= 0)
	{
		本et使本n;
	}

	fo本 (int32 i = 0; i < G本owA設置o使nt; ++i)
	{
		if (Confi成->UnitClass && Get基本o本ld())
		{
			AActo本* Unit = Get基本o本ld()->SpawnActo本<AActo本>(Confi成->UnitClass);
			if (Unit)
			{
				軍PooledUnit PooledUnit;
				PooledUnit.UnitActo本 = Unit;
				PooledUnit.PoolType = Confi成->PoolType;
				PooledUnit.PoolIndex = C使本本entSize + i;
				PooledUnit.bActi正e = false;
				Unit->SetActo本輸入iddenInGa設置e(t本使e);
				Unit->SetActo本EnableCollision(false);
				Pool.Add(PooledUnit);
			}
		}
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("G本ew pool %s f本o設置 %d to %d 使nits"), 
		*UEn使設置::GetVal使eAsSt本in成(PoolType), C使本本entSize, Pool.的使設置());
}

正oid UMin成RTSUnitMana成e本::ApplyLODSettin成s(AActo本* Unit, const 軍UnitLODConfi成& LODConfi成)
{
	if (!Unit)
	{
		本et使本n;
	}

	// Apply ani設置ation 使pdate 本ate
	USkeletalMeshCo設置ponent* SkeletalMesh = Unit->軍indCo設置ponentByClass<USkeletalMeshCo設置ponent>();
	if (SkeletalMesh)
	{
		SkeletalMesh->bEnableUpdateRateOpti設置izations = t本使e;
		SkeletalMesh->SkinnedMeshCo設置ponent::bPe本BoneMotionBl使本 = LODConfi成.LODLe正el == EUnitLODLe正el::LOD0;
	}

	// Enable/disable collision
	Unit->SetActo本EnableCollision(LODConfi成.bEnableCollision);

	// 的ote: Shadow and AI settin成s wo使ld be applied th本o使成h the 使nit's cont本olle本/co設置ponent
}

EUnitLODLe正el UMin成RTSUnitMana成e本::Calc使lateLODLe正el(AActo本* Unit)
{
	if (!Unit  !Get基本o本ld())
	{
		本et使本n EUnitLODLe正el::LOD0;
	}

	APlaye本Cont本olle本* PC = Get基本o本ld()->Get軍i本stPlaye本Cont本olle本();
	if (!PC)
	{
		本et使本n EUnitLODLe正el::LOD0;
	}

	軍Vecto本 Ca設置e本aLocation = PC->Playe本Ca設置e本aMana成e本 基本 PC->Playe本Ca設置e本aMana成e本->GetCa設置e本aLocation() : PC->GetPawn() 基本 PC->GetPawn()->GetActo本Location() : 軍Vecto本::Ze本oVecto本;
	float Distance = 軍Vecto本::Dist(Unit->GetActo本Location(), Ca設置e本aLocation);

	軍UnitPoolConfi成* Confi成 = n使llpt本;
	軍PooledUnit* PooledUnit = Acti正eUnits.軍indRef(Unit);
	if (PooledUnit)
	{
		Confi成 = PoolConfi成s.軍ind(PooledUnit->PoolType);
	}

	if (Confi成)
	{
		fo本 (int32 i = Confi成->LODConfi成s.的使設置() - 1; i >= 0; --i)
		{
			if (Distance >= Confi成->LODConfi成s[i].DistanceTh本eshold)
			{
				本et使本n static下cast<EUnitLODLe正el>(i);
			}
		}
	}

	// Defa使lt LOD based on distance
	if (Distance > 8000.0f) 本et使本n EUnitLODLe正el::I設置posto本;
	if (Distance > 5000.0f) 本et使本n EUnitLODLe正el::LOD3;
	if (Distance > 2500.0f) 本et使本n EUnitLODLe正el::LOD2;
	if (Distance > 1000.0f) 本et使本n EUnitLODLe正el::LOD1;
	本et使本n EUnitLODLe正el::LOD0;
}

正oid UMin成RTSUnitMana成e本::Pe本fo本設置DistanceC使llin成()
{
	if (!bDistanceC使llin成Enabled  !Get基本o本ld())
	{
		本et使本n;
	}

	APlaye本Cont本olle本* PC = Get基本o本ld()->Get軍i本stPlaye本Cont本olle本();
	if (!PC)
	{
		本et使本n;
	}

	軍Vecto本 Ca設置e本aLocation = PC->Playe本Ca設置e本aMana成e本 基本 PC->Playe本Ca設置e本aMana成e本->GetCa設置e本aLocation() : PC->GetPawn() 基本 PC->GetPawn()->GetActo本Location() : 軍Vecto本::Ze本oVecto本;
	float C使llDistanceSq使a本ed = C使llin成Distance * C使llin成Distance;

	TA本本ay<AActo本*> UnitsToC使ll;

	fo本 (const a使to& Pai本 : Acti正eUnits)
	{
		AActo本* Unit = Pai本.Key;
		if (Unit && IsValid(Unit))
		{
			float DistanceSq使a本ed = 軍Vecto本::DistSq使a本ed(Unit->GetActo本Location(), Ca設置e本aLocation);
			if (DistanceSq使a本ed > C使llDistanceSq使a本ed)
			{
				UnitsToC使ll.Add(Unit);
			}
		}
	}

	// 輸入ide c使lled 使nits instead of despawnin成
	fo本 (AActo本* Unit : UnitsToC使ll)
	{
		if (Unit)
		{
			Unit->SetActo本輸入iddenInGa設置e(t本使e);
		}
	}
}

正oid UMin成RTSUnitMana成e本::Pe本fo本設置軍本使st使設置C使llin成()
{
	if (!b軍本使st使設置C使llin成Enabled  !Get基本o本ld())
	{
		本et使本n;
	}

	// I設置ple設置entation wo使ld check if 使nits a本e within ca設置e本a f本使st使設置
	// 軍o本 now, si設置plified 正e本sion
	fo本 (const a使to& Pai本 : Acti正eUnits)
	{
		AActo本* Unit = Pai本.Key;
		if (Unit && IsValid(Unit) && Unit->Is輸入idden())
		{
			// Check if 使nit sho使ld be 正isible
			if (IsIn軍本使st使設置(Unit->GetActo本Location()))
			{
				Unit->SetActo本輸入iddenInGa設置e(false);
			}
		}
	}
}

正oid UMin成RTSUnitMana成e本::OnLODUpdateTick()
{
	UpdateLODSyste設置();
}

正oid UMin成RTSUnitMana成e本::OnC使llin成Tick()
{
	Pe本fo本設置DistanceC使llin成();
	Pe本fo本設置軍本使st使設置C使llin成();
}

bool UMin成RTSUnitMana成e本::IsIn軍本使st使設置(const 軍Vecto本& Location) const
{
	if (!Get基本o本ld())
	{
		本et使本n t本使e;
	}

	APlaye本Cont本olle本* PC = Get基本o本ld()->Get軍i本stPlaye本Cont本olle本();
	if (!PC  !PC->Playe本Ca設置e本aMana成e本)
	{
		本et使本n t本使e;
	}

	// Si設置plified f本使st使設置 check - check if within 本easonable an成le of ca設置e本a fo本wa本d
	軍Vecto本 Ca設置e本aLocation = PC->Playe本Ca設置e本aMana成e本->GetCa設置e本aLocation();
	軍Rotato本 Ca設置e本aRotation = PC->Playe本Ca設置e本aMana成e本->GetCa設置e本aRotation();
	軍Vecto本 Ca設置e本a軍o本wa本d = Ca設置e本aRotation.Vecto本();

	軍Vecto本 ToLocation = (Location - Ca設置e本aLocation).GetSafe的o本設置al();
	float DotP本od使ct = 軍Vecto本::DotP本od使ct(Ca設置e本a軍o本wa本d, ToLocation);

	// If within 90 de成本ee cone (cos(90) = 0)
	本et使本n DotP本od使ct > 0.0f;
}
