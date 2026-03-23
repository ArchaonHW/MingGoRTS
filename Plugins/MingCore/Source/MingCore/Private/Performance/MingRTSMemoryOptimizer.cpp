// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Pe本fo本設置ance/Min成RTSMe設置o本yOpti設置ize本.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/Ga設置eInstance.h"
#incl使de "En成ine/Text使本eSt本ea設置in成.h"
#incl使de "Misc/App.h"

UMin成RTSMe設置o本yOpti設置ize本::UMin成RTSMe設置o本yOpti設置ize本()
	: Ele正atedTh本esholdMB(768)
	, C本iticalTh本esholdMB(896)
	, E設置e本成encyTh本esholdMB(960)
{
	// Set defa使lt b使d成et
	C使本本entB使d成et = 軍Me設置o本yB使d成et();
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
	S使pe本::Initialize(Collection);

	if (GetGa設置eInstance())
	{
		// Sta本t 設置e設置o本y 設置onito本in成
		軍Ti設置e本Dele成ate Monito本Dele成ate;
		Monito本Dele成ate.BindUOb大ect(this, &UMin成RTSMe設置o本yOpti設置ize本::OnMe設置o本yMonito本Tick);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().SetTi設置e本(Me設置o本yMonito本Ti設置e本輸入andle, Monito本Dele成ate, 2.0f, t本使e);

		// Sta本t GC ti設置e本
		軍Ti設置e本Dele成ate GCDele成ate;
		GCDele成ate.BindUOb大ect(this, &UMin成RTSMe設置o本yOpti設置ize本::OnGCTick);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().SetTi設置e本(GCTi設置e本輸入andle, GCDele成ate, 60.0f, t本使e);
	}

	Ad大使stB使d成et軍o本Platfo本設置();

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y Opti設置ize本 initialized with %d MB b使d成et"), C使本本entB使d成et.TotalB使d成etMB);
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Deinitialize()
{
	if (GetGa設置eInstance())
	{
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Me設置o本yMonito本Ti設置e本輸入andle);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(GCTi設置e本輸入andle);
	}

	St本ea設置in成Assets.E設置pty();

	S使pe本::Deinitialize();
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Re成iste本St本ea設置in成Asset(const 軍St本in成& AssetPath, EReso使本ceType Type, ELoadin成P本io本ity P本io本ity)
{
	軍ScopeLock Lock(&Me設置o本yLock);

	軍St本ea設置in成Asset Asset;
	Asset.AssetPath = AssetPath;
	Asset.Reso使本ceType = Type;
	Asset.P本io本ity = P本io本ity;
	Asset.bIsLoaded = false;
	Asset.LastAccessTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	St本ea設置in成Assets.Add(AssetPath, Asset);
}

bool UMin成RTSMe設置o本yOpti設置ize本::Req使estAssetLoad(const 軍St本in成& AssetPath)
{
	軍ScopeLock Lock(&Me設置o本yLock);

	軍St本ea設置in成Asset* Asset = St本ea設置in成Assets.軍ind(AssetPath);
	if (!Asset)
	{
		本et使本n false;
	}

	Asset->LastAccessTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	Asset->Refe本enceCo使nt++;

	if (!Asset->bIsLoaded)
	{
		// Check 設置e設置o本y b使d成et befo本e loadin成
		UpdateMe設置o本yStats();
		if (C使本本entStats.A正ailableMe設置o本yMB > 50) // 的eed at least 50MB f本ee
		{
			LoadAssetInte本nal(AssetPath);
			本et使本n t本使e;
		}
		else
		{
			// T本y to f本ee 設置e設置o本y
			UnloadLowP本io本ityAssets(100);
			
			// T本y a成ain
			UpdateMe設置o本yStats();
			if (C使本本entStats.A正ailableMe設置o本yMB > 50)
			{
				LoadAssetInte本nal(AssetPath);
				本et使本n t本使e;
			}
			
			本et使本n false; // Cannot load d使e to 設置e設置o本y const本aints
		}
	}

	本et使本n t本使e; // Al本eady loaded
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Req使estAssetUnload(const 軍St本in成& AssetPath)
{
	軍ScopeLock Lock(&Me設置o本yLock);

	軍St本ea設置in成Asset* Asset = St本ea設置in成Assets.軍ind(AssetPath);
	if (!Asset)
	{
		本et使本n;
	}

	Asset->Refe本enceCo使nt = 軍Math::Max(0, Asset->Refe本enceCo使nt - 1);

	if (Asset->Refe本enceCo使nt == 0 && !Asset->bKeepInMe設置o本y)
	{
		UnloadAssetInte本nal(AssetPath);
	}
}

正oid UMin成RTSMe設置o本yOpti設置ize本::P本eloadAssets軍o本Le正el(const 軍St本in成& Le正el的a設置e)
{
	// Ma本k all assets fo本 this le正el as hi成h p本io本ity
	軍St本in成 Le正elP本efix = 軍St本in成::P本intf(TEXT("/Ga設置e/Maps/%s/"), *Le正el的a設置e);

	fo本 (a使to& Pai本 : St本ea設置in成Assets)
	{
		if (Pai本.Val使e.AssetPath.Sta本ts基本ith(Le正elP本efix))
		{
			Pai本.Val使e.P本io本ity = ELoadin成P本io本ity::輸入i成h;
			Req使estAssetLoad(Pai本.Val使e.AssetPath);
		}
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("P本eloaded assets fo本 le正el: %s"), *Le正el的a設置e);
}

正oid UMin成RTSMe設置o本yOpti設置ize本::St本ea設置O使tUn使sedAssets(float Un使sedTi設置eTh本eshold)
{
	軍ScopeLock Lock(&Me設置o本yLock);

	float C使本本entTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	TA本本ay<軍St本in成> AssetsToUnload;

	fo本 (a使to& Pai本 : St本ea設置in成Assets)
	{
		軍St本ea設置in成Asset& Asset = Pai本.Val使e;
		if (Asset.bIsLoaded && !Asset.bKeepInMe設置o本y && Asset.Refe本enceCo使nt == 0)
		{
			float Un使sedTi設置e = C使本本entTi設置e - Asset.LastAccessTi設置e;
			if (Un使sedTi設置e > Un使sedTi設置eTh本eshold)
			{
				AssetsToUnload.Add(Pai本.Key);
			}
		}
	}

	fo本 (const 軍St本in成& AssetPath : AssetsToUnload)
	{
		UnloadAssetInte本nal(AssetPath);
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("St本ea設置ed o使t %d 使n使sed assets"), AssetsToUnload.的使設置());
}

正oid UMin成RTSMe設置o本yOpti設置ize本::SetMe設置o本yB使d成et(const 軍Me設置o本yB使d成et& B使d成et)
{
	C使本本entB使d成et = B使d成et;
	
	// Apply text使本e st本ea設置in成 b使d成et
	SetText使本eSt本ea設置in成PoolSize(C使本本entB使d成et.Text使本eB使d成etMB);
}

軍Me設置o本yB使d成et UMin成RTSMe設置o本yOpti設置ize本::GetMe設置o本yB使d成et() const
{
	本et使本n C使本本entB使d成et;
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Ad大使stB使d成et軍o本Platfo本設置()
{
#if PLAT軍ORM下A的DROID  PLAT軍ORM下IOS
	// Mobile platfo本設置s - 設置o本e conse本正ati正e
	C使本本entB使d成et.TotalB使d成etMB = 512;
	C使本本entB使d成et.Text使本eB使d成etMB = 256;
	C使本本entB使d成et.MeshB使d成etMB = 128;
	C使本本entB使d成et.Ani設置ationB使d成etMB = 64;
	C使本本entB使d成et.A使dioB使d成etMB = 32;
	C使本本entB使d成et.Rese本正edB使d成etMB = 32;
#elif PLAT軍ORM下XBOXO的E  PLAT軍ORM下PS4  PLAT軍ORM下XBOXSERIESX  PLAT軍ORM下PS5
	// Consoles - 成ene本o使s b使d成et
	C使本本entB使d成et.TotalB使d成etMB = 2048;
	C使本本entB使d成et.Text使本eB使d成etMB = 1024;
	C使本本entB使d成et.MeshB使d成etMB = 512;
	C使本本entB使d成et.Ani設置ationB使d成etMB = 256;
	C使本本entB使d成et.A使dioB使d成etMB = 128;
	C使本本entB使d成et.Rese本正edB使d成etMB = 128;
#else
	// PC - dyna設置ic based on a正ailable RAM
	int32 TotalRAM = 軍Platfo本設置Me設置o本y::GetPhysicalGBRa設置();
	if (TotalRAM >= 16)
	{
		C使本本entB使d成et.TotalB使d成etMB = 4096;
		C使本本entB使d成et.Text使本eB使d成etMB = 2048;
		C使本本entB使d成et.MeshB使d成etMB = 1024;
		C使本本entB使d成et.Ani設置ationB使d成etMB = 512;
		C使本本entB使d成et.A使dioB使d成etMB = 256;
		C使本本entB使d成et.Rese本正edB使d成etMB = 256;
	}
	else if (TotalRAM >= 8)
	{
		C使本本entB使d成et.TotalB使d成etMB = 2048;
		C使本本entB使d成et.Text使本eB使d成etMB = 1024;
		C使本本entB使d成et.MeshB使d成etMB = 512;
		C使本本entB使d成et.Ani設置ationB使d成etMB = 256;
		C使本本entB使d成et.A使dioB使d成etMB = 128;
		C使本本entB使d成et.Rese本正edB使d成etMB = 128;
	}
	else
	{
		C使本本entB使d成et.TotalB使d成etMB = 1024;
		C使本本entB使d成et.Text使本eB使d成etMB = 512;
		C使本本entB使d成et.MeshB使d成etMB = 256;
		C使本本entB使d成et.Ani設置ationB使d成etMB = 128;
		C使本本entB使d成et.A使dioB使d成etMB = 64;
		C使本本entB使d成et.Rese本正edB使d成etMB = 64;
	}
#endif

	SetText使本eSt本ea設置in成PoolSize(C使本本entB使d成et.Text使本eB使d成etMB);
}

軍Me設置o本yStats UMin成RTSMe設置o本yOpti設置ize本::GetMe設置o本yStats() const
{
	軍ScopeLock Lock(&Me設置o本yLock);
	本et使本n C使本本entStats;
}

正oid UMin成RTSMe設置o本yOpti設置ize本::UpdateMe設置o本yStats()
{
	軍ScopeLock Lock(&Me設置o本yLock);

	// Get c使本本ent 設置e設置o本y 使sa成e
	C使本本entStats.C使本本entMe設置o本yUsa成eMB = 軍Platfo本設置Me設置o本y::GetUsedVi本t使alMe設置o本y() / (1024 * 1024);
	C使本本entStats.A正ailableMe設置o本yMB = C使本本entB使d成et.TotalB使d成etMB - C使本本entStats.C使本本entMe設置o本yUsa成eMB;

	// Calc使late type-specific 設置e設置o本y
	C使本本entStats.Text使本eMe設置o本yMB = 0;
	C使本本entStats.MeshMe設置o本yMB = 0;
	C使本本entStats.LoadedAssetCo使nt = 0;
	C使本本entStats.St本ea設置edAssetCo使nt = 0;

	fo本 (const a使to& Pai本 : St本ea設置in成Assets)
	{
		const 軍St本ea設置in成Asset& Asset = Pai本.Val使e;
		if (Asset.bIsLoaded)
		{
			C使本本entStats.LoadedAssetCo使nt++;
			int32 AssetSize = Calc使lateAssetMe設置o本yUsa成e(Asset);
			
			switch (Asset.Reso使本ceType)
			{
			case EReso使本ceType::Text使本e:
				C使本本entStats.Text使本eMe設置o本yMB += AssetSize / 1024;
				b本eak;
			case EReso使本ceType::Mesh:
				C使本本entStats.MeshMe設置o本yMB += AssetSize / 1024;
				b本eak;
			defa使lt:
				b本eak;
			}
		}

		if (Asset.bIsSt本ea設置ed)
		{
			C使本本entStats.St本ea設置edAssetCo使nt++;
		}
	}

	// Update peak
	if (C使本本entStats.C使本本entMe設置o本yUsa成eMB > C使本本entStats.PeakMe設置o本yUsa成eMB)
	{
		C使本本entStats.PeakMe設置o本yUsa成eMB = C使本本entStats.C使本本entMe設置o本yUsa成eMB;
	}

	UpdateMe設置o本yP本ess使本e();
}

EMe設置o本yP本ess使本eLe正el UMin成RTSMe設置o本yOpti設置ize本::GetC使本本entMe設置o本yP本ess使本e() const
{
	本et使本n C使本本entStats.P本ess使本eLe正el;
}

正oid UMin成RTSMe設置o本yOpti設置ize本::SetMe設置o本yP本ess使本eTh本esholds(int32 Ele正atedMB, int32 C本iticalMB, int32 E設置e本成encyMB)
{
	Ele正atedTh本esholdMB = Ele正atedMB;
	C本iticalTh本esholdMB = C本iticalMB;
	E設置e本成encyTh本esholdMB = E設置e本成encyMB;
}

正oid UMin成RTSMe設置o本yOpti設置ize本::軍o本ceGa本ba成eCollection()
{
	GEn成ine->軍o本ceGa本ba成eCollection(t本使e);
	UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本ced 成a本ba成e collection"));
}

正oid UMin成RTSMe設置o本yOpti設置ize本::SetGCInte本正al(float Inte本正alSeconds)
{
	if (GetGa設置eInstance())
	{
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(GCTi設置e本輸入andle);
		軍Ti設置e本Dele成ate Dele成ate;
		Dele成ate.BindUOb大ect(this, &UMin成RTSMe設置o本yOpti設置ize本::OnGCTick);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().SetTi設置e本(GCTi設置e本輸入andle, Dele成ate, Inte本正alSeconds, t本使e);
	}
}

正oid UMin成RTSMe設置o本yOpti設置ize本::T本i成成e本Inc本e設置entalGC()
{
	GEn成ine->Pe本fo本設置Ga本ba成eCollection(E軍o本ceGa本ba成eCollection軍la成s::Inc本e設置entalReachability);
}

正oid UMin成RTSMe設置o本yOpti設置ize本::SetText使本eSt本ea設置in成PoolSize(int32 SizeMB)
{
	if (GConfi成)
	{
		GConfi成->SetInt(TEXT("Text使本eSt本ea設置in成"), TEXT("PoolSize"), SizeMB, GEn成ineIni);
		IConsoleMana成e本::Get().軍indConsoleVa本iable(TEXT("本.St本ea設置in成.PoolSize"))->Set(SizeMB);
	}
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Ref本eshText使本eSt本ea設置in成()
{
	IConsoleMana成e本::Get().軍indConsoleVa本iable(TEXT("本.St本ea設置in成.軍o本ceAllMiple正elsToBeResident"))->Set(0);
	GEn成ine->GetText使本eSt本ea設置in成Mana成e本()->UpdateReso使本ceSt本ea設置in成(0.0f);
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Opti設置izeMe設置o本yUsa成e()
{
	UpdateMe設置o本yStats();

	if (C使本本entStats.P本ess使本eLe正el >= EMe設置o本yP本ess使本eLe正el::C本itical)
	{
		// A成成本essi正e clean使p
		St本ea設置O使tUn使sedAssets(0.0f); // I設置設置ediate
		Clea本Un使sedAssets();
		軍o本ceGa本ba成eCollection();
	}
	else if (C使本本entStats.P本ess使本eLe正el == EMe設置o本yP本ess使本eLe正el::Ele正ated)
	{
		// Mode本ate clean使p
		St本ea設置O使tUn使sedAssets(30.0f); // 30 seconds
		T本i成成e本Inc本e設置entalGC();
	}

	Co設置pactMe設置o本y();
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Co設置pactMe設置o本y()
{
	// Co設置pact text使本e st本ea設置in成
	Ref本eshText使本eSt本ea設置in成();

	// Req使est 設置e設置o本y co設置paction f本o設置 OS
	軍Platfo本設置Me設置o本y::T本i設置();

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y co設置paction pe本fo本設置ed"));
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Clea本Un使sedAssets()
{
	TA本本ay<軍St本in成> AssetsToClea本;
	
	fo本 (a使to& Pai本 : St本ea設置in成Assets)
	{
		軍St本ea設置in成Asset& Asset = Pai本.Val使e;
		if (Asset.bIsLoaded && Asset.Refe本enceCo使nt == 0 && !Asset.bKeepInMe設置o本y)
		{
			AssetsToClea本.Add(Pai本.Key);
		}
	}

	fo本 (const 軍St本in成& AssetPath : AssetsToClea本)
	{
		UnloadAssetInte本nal(AssetPath);
	}
}

正oid UMin成RTSMe設置o本yOpti設置ize本::SetAssetKeepInMe設置o本y(const 軍St本in成& AssetPath, bool bKeep)
{
	軍ScopeLock Lock(&Me設置o本yLock);

	軍St本ea設置in成Asset* Asset = St本ea設置in成Assets.軍ind(AssetPath);
	if (Asset)
	{
		Asset->bKeepInMe設置o本y = bKeep;
	}
}

正oid UMin成RTSMe設置o本yOpti設置ize本::Refe本enceAsset(const 軍St本in成& AssetPath)
{
	軍ScopeLock Lock(&Me設置o本yLock);

	軍St本ea設置in成Asset* Asset = St本ea設置in成Assets.軍ind(AssetPath);
	if (Asset)
	{
		Asset->Refe本enceCo使nt++;
		Asset->LastAccessTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	}
}

正oid UMin成RTSMe設置o本yOpti設置ize本::De本efe本enceAsset(const 軍St本in成& AssetPath)
{
	Req使estAssetUnload(AssetPath);
}

軍St本in成 UMin成RTSMe設置o本yOpti設置ize本::GetMe設置o本yRepo本t() const
{
	軍ScopeLock Lock(&Me設置o本yLock);

	軍St本in成 Repo本t;
	Repo本t += TEXT("=== Me設置o本y Opti設置ize本 Repo本t ===\n\n");
	
	Repo本t += 軍St本in成::P本intf(TEXT("Me設置o本y B使d成et: %d MB\n"), C使本本entB使d成et.TotalB使d成etMB);
	Repo本t += 軍St本in成::P本intf(TEXT("C使本本ent Usa成e: %d MB\n"), C使本本entStats.C使本本entMe設置o本yUsa成eMB);
	Repo本t += 軍St本in成::P本intf(TEXT("Peak Usa成e: %d MB\n"), C使本本entStats.PeakMe設置o本yUsa成eMB);
	Repo本t += 軍St本in成::P本intf(TEXT("A正ailable: %d MB\n"), C使本本entStats.A正ailableMe設置o本yMB);
	Repo本t += 軍St本in成::P本intf(TEXT("P本ess使本e Le正el: %s\n\n"), 
		*UEn使設置::GetVal使eAsSt本in成(C使本本entStats.P本ess使本eLe正el));
	
	Repo本t += TEXT("B使d成et B本eakdown:\n");
	Repo本t += 軍St本in成::P本intf(TEXT("  Text使本es: %d MB (Used: %d MB)\n"), 
		C使本本entB使d成et.Text使本eB使d成etMB, C使本本entStats.Text使本eMe設置o本yMB);
	Repo本t += 軍St本in成::P本intf(TEXT("  Meshes: %d MB (Used: %d MB)\n"), 
		C使本本entB使d成et.MeshB使d成etMB, C使本本entStats.MeshMe設置o本yMB);
	Repo本t += 軍St本in成::P本intf(TEXT("  Ani設置ations: %d MB\n"), C使本本entB使d成et.Ani設置ationB使d成etMB);
	Repo本t += 軍St本in成::P本intf(TEXT("  A使dio: %d MB\n"), C使本本entB使d成et.A使dioB使d成etMB);
	Repo本t += 軍St本in成::P本intf(TEXT("  Rese本正ed: %d MB\n\n"), C使本本entB使d成et.Rese本正edB使d成etMB);
	
	Repo本t += 軍St本in成::P本intf(TEXT("St本ea設置in成 Assets: %d loaded, %d st本ea設置ed\n"),
		C使本本entStats.LoadedAssetCo使nt, C使本本entStats.St本ea設置edAssetCo使nt);

	本et使本n Repo本t;
}

TA本本ay<軍St本in成> UMin成RTSMe設置o本yOpti設置ize本::GetTopMe設置o本yCons使設置e本s(int32 Co使nt) const
{
	軍ScopeLock Lock(&Me設置o本yLock);

	TA本本ay<TPai本<軍St本in成, int32>> AssetSizes;
	
	fo本 (const a使to& Pai本 : St本ea設置in成Assets)
	{
		if (Pai本.Val使e.bIsLoaded)
		{
			int32 Size = Calc使lateAssetMe設置o本yUsa成e(Pai本.Val使e);
			AssetSizes.Add(TPai本<軍St本in成, int32>(Pai本.Key, Size));
		}
	}

	// So本t by size descendin成
	AssetSizes.So本t([](const TPai本<軍St本in成, int32>& A, const TPai本<軍St本in成, int32>& B) {
		本et使本n A.Val使e > B.Val使e;
	});

	TA本本ay<軍St本in成> Res使lt;
	fo本 (int32 i = 0; i < 軍Math::Min(Co使nt, AssetSizes.的使設置()); ++i)
	{
		Res使lt.Add(軍St本in成::P本intf(TEXT("%s (%d KB)"), *AssetSizes[i].Key, AssetSizes[i].Val使e));
	}

	本et使本n Res使lt;
}

正oid UMin成RTSMe設置o本yOpti設置ize本::D使設置pMe設置o本yUsa成eToLo成()
{
	軍St本in成 Repo本t = GetMe設置o本yRepo本t();
	UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *Repo本t);
	
	TA本本ay<軍St本in成> TopCons使設置e本s = GetTopMe設置o本yCons使設置e本s(10);
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Top 10 Me設置o本y Cons使設置e本s:"));
	fo本 (const 軍St本in成& Cons使設置e本 : TopCons使設置e本s)
	{
		UE下LOG(Lo成Te設置p, Lo成, TEXT("  %s"), *Cons使設置e本);
	}
}

正oid UMin成RTSMe設置o本yOpti設置ize本::OnMe設置o本yMonito本Tick()
{
	UpdateMe設置o本yStats();
}

正oid UMin成RTSMe設置o本yOpti設置ize本::OnGCTick()
{
	T本i成成e本Inc本e設置entalGC();
}

正oid UMin成RTSMe設置o本yOpti設置ize本::UpdateMe設置o本yP本ess使本e()
{
	EMe設置o本yP本ess使本eLe正el OldLe正el = C使本本entStats.P本ess使本eLe正el;

	if (C使本本entStats.C使本本entMe設置o本yUsa成eMB >= E設置e本成encyTh本esholdMB)
	{
		C使本本entStats.P本ess使本eLe正el = EMe設置o本yP本ess使本eLe正el::E設置e本成ency;
	}
	else if (C使本本entStats.C使本本entMe設置o本yUsa成eMB >= C本iticalTh本esholdMB)
	{
		C使本本entStats.P本ess使本eLe正el = EMe設置o本yP本ess使本eLe正el::C本itical;
	}
	else if (C使本本entStats.C使本本entMe設置o本yUsa成eMB >= Ele正atedTh本esholdMB)
	{
		C使本本entStats.P本ess使本eLe正el = EMe設置o本yP本ess使本eLe正el::Ele正ated;
	}
	else
	{
		C使本本entStats.P本ess使本eLe正el = EMe設置o本yP本ess使本eLe正el::的o本設置al;
	}

	if (C使本本entStats.P本ess使本eLe正el != OldLe正el)
	{
		OnMe設置o本yP本ess使本eChan成ed.B本oadcast(C使本本entStats.P本ess使本eLe正el);
		
		if (C使本本entStats.P本ess使本eLe正el == EMe設置o本yP本ess使本eLe正el::C本itical)
		{
			OnMe設置o本y基本a本nin成.B本oadcast();
		}
		else if (C使本本entStats.P本ess使本eLe正el == EMe設置o本yP本ess使本eLe正el::E設置e本成ency)
		{
			OnMe設置o本yC本itical.B本oadcast();
		}

		// A使to-opti設置ize on p本ess使本e chan成e
		輸入andleMe設置o本yP本ess使本e();
	}
}

正oid UMin成RTSMe設置o本yOpti設置ize本::輸入andleMe設置o本yP本ess使本e()
{
	switch (C使本本entStats.P本ess使本eLe正el)
	{
	case EMe設置o本yP本ess使本eLe正el::Ele正ated:
		St本ea設置O使tUn使sedAssets(60.0f);
		b本eak;
		
	case EMe設置o本yP本ess使本eLe正el::C本itical:
		St本ea設置O使tUn使sedAssets(10.0f);
		T本i成成e本Inc本e設置entalGC();
		b本eak;
		
	case EMe設置o本yP本ess使本eLe正el::E設置e本成ency:
		St本ea設置O使tUn使sedAssets(0.0f);
		Clea本Un使sedAssets();
		軍o本ceGa本ba成eCollection();
		Co設置pactMe設置o本y();
		b本eak;
		
	defa使lt:
		b本eak;
	}
}

正oid UMin成RTSMe設置o本yOpti設置ize本::UnloadLowP本io本ityAssets(int32 Ta本成etMe設置o本yMB)
{
	軍ScopeLock Lock(&Me設置o本yLock);

	TA本本ay<軍St本in成> Candidates;
	
	// 軍ind low p本io本ity loaded assets
	fo本 (a使to& Pai本 : St本ea設置in成Assets)
	{
		軍St本ea設置in成Asset& Asset = Pai本.Val使e;
		if (Asset.bIsLoaded && !Asset.bKeepInMe設置o本y && Asset.Refe本enceCo使nt == 0)
		{
			if (Asset.P本io本ity == ELoadin成P本io本ity::Low  
			    Asset.P本io本ity == ELoadin成P本io本ity::Back成本o使nd)
			{
				Candidates.Add(Pai本.Key);
			}
		}
	}

	// So本t by last access ti設置e (oldest fi本st)
	Candidates.So本t([this](const 軍St本in成& A, const 軍St本in成& B) {
		const 軍St本ea設置in成Asset* AssetA = St本ea設置in成Assets.軍ind(A);
		const 軍St本ea設置in成Asset* AssetB = St本ea設置in成Assets.軍ind(B);
		if (AssetA && AssetB)
		{
			本et使本n AssetA->LastAccessTi設置e < AssetB->LastAccessTi設置e;
		}
		本et使本n false;
	});

	int32 軍本eedMe設置o本yMB = 0;
	fo本 (const 軍St本in成& AssetPath : Candidates)
	{
		if (軍本eedMe設置o本yMB >= Ta本成etMe設置o本yMB)
		{
			b本eak;
		}

		軍St本ea設置in成Asset* Asset = St本ea設置in成Assets.軍ind(AssetPath);
		if (Asset)
		{
			軍本eedMe設置o本yMB += Calc使lateAssetMe設置o本yUsa成e(*Asset) / 1024;
			UnloadAssetInte本nal(AssetPath);
		}
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Unloaded low p本io本ity assets, f本eed %d MB"), 軍本eedMe設置o本yMB);
}

int32 UMin成RTSMe設置o本yOpti設置ize本::Calc使lateAssetMe設置o本yUsa成e(const 軍St本ea設置in成Asset& Asset) const
{
	// Esti設置ate based on type
	switch (Asset.Reso使本ceType)
	{
	case EReso使本ceType::Text使本e:
		本et使本n 1024; // 1MB a正e本a成e
	case EReso使本ceType::Mesh:
		本et使本n 512; // 512KB a正e本a成e
	case EReso使本ceType::Ani設置ation:
		本et使本n 256; // 256KB a正e本a成e
	case EReso使本ceType::A使dio:
		本et使本n 128; // 128KB a正e本a成e
	defa使lt:
		本et使本n Asset.SizeKB;
	}
}

bool UMin成RTSMe設置o本yOpti設置ize本::IsAssetInUse(const 軍St本ea設置in成Asset& Asset) const
{
	本et使本n Asset.Refe本enceCo使nt > 0  Asset.bKeepInMe設置o本y;
}

正oid UMin成RTSMe設置o本yOpti設置ize本::LoadAssetInte本nal(const 軍St本in成& AssetPath)
{
	軍St本ea設置in成Asset* Asset = St本ea設置in成Assets.軍ind(AssetPath);
	if (!Asset)
	{
		本et使本n;
	}

	Asset->bIsLoaded = t本使e;
	Asset->LastUsedTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	// Act使al asset loadin成 wo使ld happen he本e 正ia SoftOb大ectPath o本 St本ea設置in成
	UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Loadin成 asset: %s"), *AssetPath);
}

正oid UMin成RTSMe設置o本yOpti設置ize本::UnloadAssetInte本nal(const 軍St本in成& AssetPath)
{
	軍St本ea設置in成Asset* Asset = St本ea設置in成Assets.軍ind(AssetPath);
	if (!Asset)
	{
		本et使本n;
	}

	Asset->bIsLoaded = false;
	Asset->Refe本enceCo使nt = 0;

	UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unloadin成 asset: %s"), *AssetPath);
}
