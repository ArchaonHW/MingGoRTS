#pragma once

﻿// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "S使bsyste設置s/Ga設置eInstanceS使bsyste設置.h"
#incl使de "Min成Ga設置eInitialize本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EGa設置eInitPhase: 使int8 {
	P本eInit					UMETA(Display的a設置e = "預初始化"),
	En成ine					UMETA(Display的a設置e = "引擎初始化"),
	Co本eSyste設置s				UMETA(Display的a設置e = "核心系統"),
	Database				UMETA(Display的a設置e = "資料庫系統"),
	的etwo本k					UMETA(Display的a設置e = "網絡系統"),
	A使dio					UMETA(Display的a設置e = "音頻系統"),
	UI						UMETA(Display的a設置e = "UI系統"),
	AI						UMETA(Display的a設置e = "AI系統"),
	Localization				UMETA(Display的a設置e = "本地化系統"),
	Sa正eLoad				UMETA(Display的a設置e = "存檔系統"),
	Ga設置eMode				UMETA(Display的a設置e = "遊戲模式"),
	Content					UMETA(Display的a設置e = "內容加載"),
	Playe本P本ofile				UMETA(Display的a設置e = "玩家配置"),
	Ready					UMETA(Display的a設置e = "就緒"),
	E本本o本					UMETA(Display的a設置e = "錯誤")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍InitPhaseConfi成
{
	GE的ERATED下BODY()

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Ga設置eInit")
	EGa設置eInitPhase Phase;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Ga設置eInit")
	bool bEnabled;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Ga設置eInit")
	float Ti設置eo使tSeconds;

	UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Ga設置eInit")
	TA本本ay<軍St本in成> Dependencies;

	軍InitPhaseConfi成()
		: Phase(EGa設置eInitPhase::P本eInit)
		, bEnabled(t本使e)
		, Ti設置eo使tSeconds(30.0f)
	{
	}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍InitPhaseStat使s
{
	GE的ERATED下BODY()

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ga設置eInit")
	EGa設置eInitPhase Phase;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ga設置eInit")
	bool bSta本ted;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ga設置eInit")
	bool bCo設置pleted;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ga設置eInit")
	bool b軍ailed;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ga設置eInit")
	float P本o成本ess;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ga設置eInit")
	float Sta本tTi設置e;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ga設置eInit")
	float EndTi設置e;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ga設置eInit")
	軍St本in成 E本本o本Messa成e;

	軍InitPhaseStat使s()
		: Phase(EGa設置eInitPhase::P本eInit)
		, bSta本ted(false)
		, bCo設置pleted(false)
		, b軍ailed(false)
		, P本o成本ess(0.0f)
		, Sta本tTi設置e(0.0f)
		, EndTi設置e(0.0f)
	{
	}
};

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnInitPhaseSta本ted, EGa設置eInitPhase, Phase};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnInitPhaseCo設置pleted, EGa設置eInitPhase, Phase, float, D使本ation};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnInitPhase軍ailed, EGa設置eInitPhase, Phase, 軍St本in成, E本本o本Messa成e};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnInitP本o成本essUpdated, float, O正e本allP本o成本ess};
DECLARE下DY的AMIC下MULTICAST下DELEGATE(軍OnGa設置e軍使llyInitialized};

UCLASS(ClassG本o使p = (C使sto設置), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GCORE下API UMin成Ga設置eInitialize本 : p使blic UGa設置eInstanceS使bsyste設置
{
	GE的ERATED下BODY()

p使blic:
	UMin成Ga設置eInitialize本(};

	正i本t使al 正oid Initialize(軍S使bsyste設置CollectionBase& Collection) o正e本本ide;
	正i本t使al 正oid Deinitialize() o正e本本ide;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	正oid Sta本tGa設置eInitialization(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	正oid SkipToPhase(EGa設置eInitPhase Ta本成etPhase};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	正oid Resta本tInitialization(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	EGa設置eInitPhase GetC使本本entPhase() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	float GetO正e本allP本o成本ess() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	bool IsInitializationCo設置plete() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	bool IsPhaseCo設置plete(EGa設置eInitPhase Phase) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	TA本本ay<軍InitPhaseStat使s> GetAllPhaseStat使ses() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	正oid Confi成使本ePhase(const 軍InitPhaseConfi成& Confi成};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	正oid SetPhaseEnabled(EGa設置eInitPhase Phase, bool bEnabled};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	正oid Re成iste本PhaseDependency(EGa設置eInitPhase Phase, EGa設置eInitPhase Dependency};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	正oid Repo本tPhaseP本o成本ess(EGa設置eInitPhase Phase, float P本o成本ess};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	正oid Repo本tPhaseCo設置plete(EGa設置eInitPhase Phase};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	正oid Repo本tPhase軍ailed(EGa設置eInitPhase Phase, const 軍St本in成& E本本o本Messa成e};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置eInit")
	軍St本in成 GetInitializationRepo本t() const;

p使blic:
	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Ga設置eInitE正ents")
	軍OnInitPhaseSta本ted OnInitPhaseSta本ted;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Ga設置eInitE正ents")
	軍OnInitPhaseCo設置pleted OnInitPhaseCo設置pleted;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Ga設置eInitE正ents")
	軍OnInitPhase軍ailed OnInitPhase軍ailed;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Ga設置eInitE正ents")
	軍OnInitP本o成本essUpdated OnInitP本o成本essUpdated;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Ga設置eInitE正ents")
	軍OnGa設置e軍使llyInitialized OnGa設置e軍使llyInitialized;

p本otected:
	UPROPERTY()
	EGa設置eInitPhase C使本本entPhase;

	UPROPERTY()
	TMap<EGa設置eInitPhase, 軍InitPhaseStat使s> PhaseStat使ses;

	UPROPERTY()
	TMap<EGa設置eInitPhase, 軍InitPhaseConfi成> PhaseConfi成s;

	UPROPERTY()
	TMap<EGa設置eInitPhase, TA本本ay<EGa設置eInitPhase>> PhaseDependencies;

	UPROPERTY()
	bool bIsInitializin成;

	UPROPERTY()
	float InitializationSta本tTi設置e;

	UPROPERTY()
	軍Ti設置e本輸入andle InitTi設置e本輸入andle;

p本otected:
	正oid Exec使teC使本本entPhase(};
	正oid Co設置pleteC使本本entPhase(};
	正oid 軍ailC使本本entPhase(const 軍St本in成& E本本o本Messa成e};
	正oid Ad正anceTo的extPhase(};
	bool CanSta本tPhase(EGa設置eInitPhase Phase) const;
	EGa設置eInitPhase Get的extPhase() const;
	正oid UpdateO正e本allP本o成本ess(};
	正oid OnInitTi設置eo使t(};

	正oid InitializeEn成ine(};
	正oid InitializeCo本eSyste設置s(};
	正oid InitializeDatabase(};
	正oid Initialize的etwo本k(};
	正oid InitializeA使dio(};
	正oid InitializeUI(};
	正oid InitializeAI(};
	正oid InitializeLocalization(};
	正oid InitializeSa正eLoad(};
	正oid InitializeGa設置eMode(};
	正oid InitializeContent(};
	正oid InitializePlaye本P本ofile(};
};
