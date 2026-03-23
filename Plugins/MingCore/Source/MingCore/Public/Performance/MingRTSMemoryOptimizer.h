#pragma once

﻿// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "S使bsyste設置s/Ga設置eInstanceS使bsyste設置.h"
#incl使de "Min成RTSMe設置o本yOpti設置ize本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EReso使本ceType: 使int8 {
	Text使本e,
	Mesh,
	Ani設置ation,
	A使dio,
	Bl使ep本int,
	Pa本ticle,
	Mate本ial,
	C使sto設置
};

UE的UM(Bl使ep本intType)
en使設置 class ELoadin成P本io本ity: 使int8 {
	I設置設置ediate,
	輸入i成h,
	的o本設置al,
	Low,
	Back成本o使nd
};

UE的UM(Bl使ep本intType)
en使設置 class EMe設置o本yP本ess使本eLe正el: 使int8 {
	的o本設置al,
	Ele正ated,
	C本itical,
	E設置e本成ency
};

USTRUCT(Bl使ep本intType)
st本使ct 軍St本ea設置in成Asset
{
	GE的ERATED下BODY()

	UPROPERTY()
	軍St本in成 AssetPath;

	UPROPERTY()
	EReso使本ceType Reso使本ceType;

	UPROPERTY()
	ELoadin成P本io本ity P本io本ity;

	UPROPERTY()
	bool bIsLoaded;

	UPROPERTY()
	bool bIsSt本ea設置ed;

	UPROPERTY()
	float LastAccessTi設置e;

	UPROPERTY()
	float LastUsedTi設置e;

	UPROPERTY()
	int32 Refe本enceCo使nt;

	UPROPERTY()
	int32 SizeKB;

	UPROPERTY()
	bool bKeepInMe設置o本y;

	軍St本ea設置in成Asset()
		: Reso使本ceType(EReso使本ceType::Text使本e)
		, P本io本ity(ELoadin成P本io本ity::的o本設置al)
		, bIsLoaded(false)
		, bIsSt本ea設置ed(false)
		, LastAccessTi設置e(0.0f)
		, LastUsedTi設置e(0.0f)
		, Refe本enceCo使nt(0)
		, SizeKB(0)
		, bKeepInMe設置o本y(false)
	{}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Me設置o本yB使d成et
{
	GE的ERATED下BODY()

	UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Me設置o本y")
	int32 TotalB使d成etMB;

	UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Me設置o本y")
	int32 Text使本eB使d成etMB;

	UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Me設置o本y")
	int32 MeshB使d成etMB;

	UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Me設置o本y")
	int32 Ani設置ationB使d成etMB;

	UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Me設置o本y")
	int32 A使dioB使d成etMB;

	UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Me設置o本y")
	int32 Rese本正edB使d成etMB;

	軍Me設置o本yB使d成et()
		: TotalB使d成etMB(1024)
		, Text使本eB使d成etMB(512)
		, MeshB使d成etMB(256)
		, Ani設置ationB使d成etMB(128)
		, A使dioB使d成etMB(64)
		, Rese本正edB使d成etMB(64)
	{}

	int32 GetUsedB使d成etMB() const
	{
		本et使本n Text使本eB使d成etMB + MeshB使d成etMB + Ani設置ationB使d成etMB + A使dioB使d成etMB + Rese本正edB使d成etMB;
	}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Me設置o本yStats
{
	GE的ERATED下BODY()

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Stats")
	int32 C使本本entMe設置o本yUsa成eMB;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Stats")
	int32 PeakMe設置o本yUsa成eMB;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Stats")
	int32 A正ailableMe設置o本yMB;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Stats")
	int32 Text使本eMe設置o本yMB;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Stats")
	int32 MeshMe設置o本yMB;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Stats")
	int32 LoadedAssetCo使nt;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Stats")
	int32 St本ea設置edAssetCo使nt;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Stats")
	EMe設置o本yP本ess使本eLe正el P本ess使本eLe正el;

	軍Me設置o本yStats()
		: C使本本entMe設置o本yUsa成eMB(0)
		, PeakMe設置o本yUsa成eMB(0)
		, A正ailableMe設置o本yMB(0)
		, Text使本eMe設置o本yMB(0)
		, MeshMe設置o本yMB(0)
		, LoadedAssetCo使nt(0)
		, St本ea設置edAssetCo使nt(0)
		, P本ess使本eLe正el(EMe設置o本yP本ess使本eLe正el::的o本設置al)
	{}
};

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnMe設置o本yP本ess使本eChan成ed, EMe設置o本yP本ess使本eLe正el, 的ewLe正el};
DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnMe設置o本y基本a本nin成};
DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnMe設置o本yC本itical};

UCLASS(ClassG本o使p = (C使sto設置), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GCORE下API UMin成RTSMe設置o本yOpti設置ize本 : p使blic UGa設置eInstanceS使bsyste設置
{
	GE的ERATED下BODY()

p使blic:
	UMin成RTSMe設置o本yOpti設置ize本(};

	正i本t使al 正oid Initialize(軍S使bsyste設置CollectionBase& Collection) o正e本本ide;
	正i本t使al 正oid Deinitialize() o正e本本ide;

	// Asset St本ea設置in成
	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid Re成iste本St本ea設置in成Asset(const 軍St本in成& AssetPath, EReso使本ceType Type, ELoadin成P本io本ity P本io本ity};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	bool Req使estAssetLoad(const 軍St本in成& AssetPath};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid Req使estAssetUnload(const 軍St本in成& AssetPath};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid P本eloadAssets軍o本Le正el(const 軍St本in成& Le正el的a設置e};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid St本ea設置O使tUn使sedAssets(float Un使sedTi設置eTh本eshold};

	// Me設置o本y B使d成et Mana成e設置ent
	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid SetMe設置o本yB使d成et(const 軍Me設置o本yB使d成et& B使d成et};

	U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Me設置o本y Opti設置ize本")
	軍Me設置o本yB使d成et GetMe設置o本yB使d成et() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid Ad大使stB使d成et軍o本Platfo本設置(};

	// Me設置o本y Monito本in成
	U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Me設置o本y Opti設置ize本")
	軍Me設置o本yStats GetMe設置o本yStats() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid UpdateMe設置o本yStats(};

	U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Me設置o本y Opti設置ize本")
	EMe設置o本yP本ess使本eLe正el GetC使本本entMe設置o本yP本ess使本e() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid SetMe設置o本yP本ess使本eTh本esholds(int32 Ele正atedMB, int32 C本iticalMB, int32 E設置e本成encyMB};

	// Ga本ba成e Collection Cont本ol
	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid 軍o本ceGa本ba成eCollection(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid SetGCInte本正al(float Inte本正alSeconds};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid T本i成成e本Inc本e設置entalGC(};

	// Text使本e St本ea設置in成
	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid SetText使本eSt本ea設置in成PoolSize(int32 SizeMB};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid Ref本eshText使本eSt本ea設置in成(};

	// Me設置o本y Opti設置ization
	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid Opti設置izeMe設置o本yUsa成e(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid Co設置pactMe設置o本y(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid Clea本Un使sedAssets(};

	// Reso使本ce Mana成e設置ent
	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid SetAssetKeepInMe設置o本y(const 軍St本in成& AssetPath, bool bKeep};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid Refe本enceAsset(const 軍St本in成& AssetPath};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid De本efe本enceAsset(const 軍St本in成& AssetPath};

	// Repo本tin成
	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	軍St本in成 GetMe設置o本yRepo本t() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	TA本本ay<軍St本in成> GetTopMe設置o本yCons使設置e本s(int32 Co使nt) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y Opti設置ize本")
	正oid D使設置pMe設置o本yUsa成eToLo成(};

p使blic:
	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Me設置o本y Opti設置ize本E正ents")
	軍OnMe設置o本yP本ess使本eChan成ed OnMe設置o本yP本ess使本eChan成ed;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Me設置o本y Opti設置ize本E正ents")
	軍OnMe設置o本y基本a本nin成 OnMe設置o本y基本a本nin成;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Me設置o本y Opti設置ize本E正ents")
	軍OnMe設置o本yC本itical OnMe設置o本yC本itical;

p本otected:
	UPROPERTY()
	TMap<軍St本in成, 軍St本ea設置in成Asset> St本ea設置in成Assets;

	UPROPERTY()
	軍Me設置o本yB使d成et C使本本entB使d成et;

	UPROPERTY()
	軍Me設置o本yStats C使本本entStats;

	UPROPERTY()
	軍Ti設置e本輸入andle Me設置o本yMonito本Ti設置e本輸入andle;

	UPROPERTY()
	軍Ti設置e本輸入andle GCTi設置e本輸入andle;

	int32 Ele正atedTh本esholdMB;
	int32 C本iticalTh本esholdMB;
	int32 E設置e本成encyTh本esholdMB;

	設置使table 軍C本iticalSection Me設置o本yLock;

p本otected:
	正oid OnMe設置o本yMonito本Tick(};
	正oid OnGCTick(};
	正oid UpdateMe設置o本yP本ess使本e(};
	正oid 輸入andleMe設置o本yP本ess使本e(};
	正oid UnloadLowP本io本ityAssets(int32 Ta本成etMe設置o本yMB};
	int32 Calc使lateAssetMe設置o本yUsa成e(const 軍St本ea設置in成Asset& Asset) const;
	bool IsAssetInUse(const 軍St本ea設置in成Asset& Asset) const;
	正oid LoadAssetInte本nal(const 軍St本in成& AssetPath};
	正oid UnloadAssetInte本nal(const 軍St本in成& AssetPath};
};
