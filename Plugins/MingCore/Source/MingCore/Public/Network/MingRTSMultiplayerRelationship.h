#pragma once

﻿// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSM使ltiplaye本Relationship.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EM使ltiplaye本RelationType: 使int8 {
	的e使t本al					UMETA(Display的a設置e = "中立"),
	Ally					UMETA(Display的a設置e = "盟友"),
	Ene設置y					UMETA(Display的a設置e = "敵人"),
	T本adePa本tne本			UMETA(Display的a設置e = "貿易夥伴"),
	Vassal					UMETA(Display的a設置e = "附庸"),
	O正e本lo本d				UMETA(Display的a設置e = "宗主"),
	的onA成成本ession			UMETA(Display的a設置e = "互不侵犯"),
	基本a本						UMETA(Display的a設置e = "戰爭")
};

UE的UM(Bl使ep本intType)
en使設置 class EDiplo設置aticAction: 使int8 {
	Decla本e基本a本				UMETA(Display的a設置e = "宣戰"),
	Offe本Peace				UMETA(Display的a設置e = "求和"),
	軍o本設置Alliance			UMETA(Display的a設置e = "結盟"),
	B本eakAlliance			UMETA(Display的a設置e = "撕毀盟約"),
	Offe本T本ade				UMETA(Display的a設置e = "提貿易"),
	CancelT本ade				UMETA(Display的a設置e = "取消貿易"),
	De設置andT本ib使te			UMETA(Display的a設置e = "要求進貢"),
	SendGift				UMETA(Display的a設置e = "贈送禮物"),
	P本oposeMa本本ia成e			UMETA(Display的a設置e = "聯姻提議"),
	Offe本Vassalization		UMETA(Display的a設置e = "提附庸")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Diplo設置aticP本oposal
{
	GE的ERATED下BODY()

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	軍G使id P本oposalId;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	int32 軍本o設置Playe本Id;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	int32 ToPlaye本Id;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	EDiplo設置aticAction Action;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	軍St本in成 Messa成e;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	TMap<軍St本in成, int32> Reso使本ceDe設置ands;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	軍DateTi設置e P本oposedTi設置e;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	float Expi本ationTi設置e;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	bool bReq使i本esAcceptance;

	軍Diplo設置aticP本oposal()
		: P本oposalId(軍G使id::的ewG使id())
		, 軍本o設置Playe本Id(0)
		, ToPlaye本Id(0)
		, Action(EDiplo設置aticAction::SendGift)
		, Expi本ationTi設置e(300.0f)
		, bReq使i本esAcceptance(t本使e)
		, P本oposedTi設置e(軍DateTi設置e::的ow())
	{
	}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Playe本Diplo設置aticRelation
{
	GE的ERATED下BODY()

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	int32 Playe本Id1;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	int32 Playe本Id2;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	EM使ltiplaye本RelationType RelationType;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	float T本使stLe正el;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	float RelationSt本en成th;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	TMap<軍St本in成, float> T本adeA成本ee設置ents;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	軍DateTi設置e LastInte本action;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	int32 AllianceD使本ation;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	bool b輸入as的onA成成本essionPact;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	float 的onA成成本essionExpi本ation;

	軍Playe本Diplo設置aticRelation()
		: Playe本Id1(0)
		, Playe本Id2(0)
		, RelationType(EM使ltiplaye本RelationType::的e使t本al)
		, T本使stLe正el(0.0f)
		, RelationSt本en成th(50.0f)
		, AllianceD使本ation(0)
		, b輸入as的onA成成本essionPact(false)
		, 的onA成成本essionExpi本ation(0.0f)
		, LastInte本action(軍DateTi設置e::的ow())
	{
	}

	bool In正ol正esPlaye本(int32 Playe本Id) const
	{
		本et使本n Playe本Id1 == Playe本Id  Playe本Id2 == Playe本Id;
	}

	int32 GetOthe本Playe本(int32 Playe本Id) const
	{
		本et使本n (Playe本Id == Playe本Id1) 基本 Playe本Id2 : Playe本Id1;
	}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Diplo設置aticE正ent
{
	GE的ERATED下BODY()

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	軍G使id E正entId;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	軍St本in成 E正entType;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	int32 In正ol正edPlaye本Id1;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	int32 In正ol正edPlaye本Id2;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	軍St本in成 Desc本iption;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	軍DateTi設置e Ti設置esta設置p;

	UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Diplo設置acy")
	float I設置pact;

	軍Diplo設置aticE正ent()
		: E正entId(軍G使id::的ewG使id())
		, In正ol正edPlaye本Id1(0)
		, In正ol正edPlaye本Id2(0)
		, I設置pact(0.0f)
		, Ti設置esta設置p(軍DateTi設置e::的ow())
	{
	}
};

DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnRelationChan成ed, int32, Playe本Id1, int32, Playe本Id2};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnDiplo設置aticP本oposalRecei正ed, const 軍Diplo設置aticP本oposal&, P本oposal};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnDiplo設置aticP本oposalAccepted, const 軍Diplo設置aticP本oposal&, P本oposal, bool, bAccepted};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On基本a本Decla本ed, const 軍Diplo設置aticE正ent&, E正ent};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnPeaceMade, const 軍Diplo設置aticE正ent&, E正ent};

UCLASS(ClassG本o使p = (C使sto設置), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GCORE下API UMin成RTSM使ltiplaye本Relationship : p使blic UOb大ect
{
	GE的ERATED下BODY()

p使blic:
	UMin成RTSM使ltiplaye本Relationship(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	bool InitializeSyste設置(UMin成RTS的etwo本kMana成e本* In的etwo本kMana成e本};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid Sh使tdownSyste設置(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid SetLocalPlaye本Id(int32 Playe本Id};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	int32 GetLocalPlaye本Id() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid SetDiplo設置aticRelation(int32 Playe本Id1, int32 Playe本Id2, EM使ltiplaye本RelationType RelationType};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	EM使ltiplaye本RelationType GetDiplo設置aticRelation(int32 Playe本Id1, int32 Playe本Id2) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	軍Playe本Diplo設置aticRelation GetRelationDetails(int32 Playe本Id1, int32 Playe本Id2) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	TA本本ay<軍Playe本Diplo設置aticRelation> GetAllRelations軍o本Playe本(int32 Playe本Id) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	TA本本ay<軍Playe本Diplo設置aticRelation> GetAllRelations() const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	TA本本ay<int32> GetAllies(int32 Playe本Id) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	TA本本ay<int32> GetEne設置ies(int32 Playe本Id) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	TA本本ay<int32> GetT本adePa本tne本s(int32 Playe本Id) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	bool IsAt基本a本(int32 Playe本Id1, int32 Playe本Id2) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	bool IsAllied(int32 Playe本Id1, int32 Playe本Id2) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	bool 輸入as的onA成成本essionPact(int32 Playe本Id1, int32 Playe本Id2) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid Decla本e基本a本(int32 Ta本成etPlaye本Id, const 軍St本in成& Cas使sBelli};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid Offe本Peace(int32 Ta本成etPlaye本Id, const TMap<軍St本in成, int32>& Te本設置s};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid 軍o本設置Alliance(int32 Ta本成etPlaye本Id};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid B本eakAlliance(int32 Ta本成etPlaye本Id};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid P本opose的onA成成本essionPact(int32 Ta本成etPlaye本Id, float D使本ation};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid P本oposeT本adeA成本ee設置ent(int32 Ta本成etPlaye本Id, const TMap<軍St本in成, int32>& T本adeTe本設置s};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid SendDiplo設置aticP本oposal(const 軍Diplo設置aticP本oposal& P本oposal};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid AcceptP本oposal(const 軍G使id& P本oposalId};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid Re大ectP本oposal(const 軍G使id& P本oposalId, const 軍St本in成& Reason};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid SetT本使stLe正el(int32 Playe本Id1, int32 Playe本Id2, float T本使stLe正el};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	float GetT本使stLe正el(int32 Playe本Id1, int32 Playe本Id2) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid ModifyT本使stLe正el(int32 Playe本Id1, int32 Playe本Id2, float Delta};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid Reco本dDiplo設置aticE正ent(const 軍St本in成& E正entType, int32 Playe本Id1, int32 Playe本Id2, 
		const 軍St本in成& Desc本iption, float I設置pact};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	TA本本ay<軍Diplo設置aticE正ent> GetDiplo設置atic輸入isto本y(int32 Playe本Id, int32 Li設置it = 50) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid Clea本AllRelations(};

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	軍St本in成 GetDiplo設置aticS使設置設置a本y(int32 Playe本Id) const;

	U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本Diplo設置acy")
	正oid Tick(float DeltaTi設置e};

p使blic:
	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "M使ltiplaye本Diplo設置acyE正ents")
	軍OnRelationChan成ed OnRelationChan成ed;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "M使ltiplaye本Diplo設置acyE正ents")
	軍OnDiplo設置aticP本oposalRecei正ed OnDiplo設置aticP本oposalRecei正ed;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "M使ltiplaye本Diplo設置acyE正ents")
	軍OnDiplo設置aticP本oposalAccepted OnDiplo設置aticP本oposalAccepted;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "M使ltiplaye本Diplo設置acyE正ents")
	軍On基本a本Decla本ed On基本a本Decla本ed;

	UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "M使ltiplaye本Diplo設置acyE正ents")
	軍OnPeaceMade OnPeaceMade;

p本otected:
	UPROPERTY()
	int32 LocalPlaye本Id;

	UPROPERTY()
	TMap<軍G使id, 軍Playe本Diplo設置aticRelation> Diplo設置aticRelations;

	UPROPERTY()
	TMap<軍G使id, 軍Diplo設置aticP本oposal> Pendin成P本oposals;

	UPROPERTY()
	TA本本ay<軍Diplo設置aticE正ent> Diplo設置atic輸入isto本y;

	UPROPERTY()
	UMin成RTS的etwo本kMana成e本* 的etwo本kMana成e本;

	UPROPERTY()
	float RelationUpdateInte本正al;

	UPROPERTY()
	float Ti設置eSinceLastUpdate;

p本otected:
	軍G使id GetRelationKey(int32 Playe本Id1, int32 Playe本Id2) const;
	bool A本ePlaye本sValid(int32 Playe本Id1, int32 Playe本Id2) const;
	正oid B本oadcastRelationChan成e(int32 Playe本Id1, int32 Playe本Id2};
	正oid P本ocessPendin成P本oposals(float DeltaTi設置e};
	正oid Expi本eOldP本oposals(};
	正oid UpdateRelationSt本en成ths(float DeltaTi設置e};
	正oid SyncRelationTo的etwo本k(const 軍Playe本Diplo設置aticRelation& Relation};
	正oid SyncP本oposalTo的etwo本k(const 軍Diplo設置aticP本oposal& P本oposal};

	U軍U的CTIO的(Se本正e本, Reliable, 基本ithValidation)
	正oid Se本正e本Decla本e基本a本(int32 Ta本成etPlaye本Id, const 軍St本in成& Cas使sBelli};

	U軍U的CTIO的(Se本正e本, Reliable, 基本ithValidation)
	正oid Se本正e本Offe本Peace(int32 Ta本成etPlaye本Id, const TMap<軍St本in成, int32>& Te本設置s};

	U軍U的CTIO的(Se本正e本, Reliable, 基本ithValidation)
	正oid Se本正e本軍o本設置Alliance(int32 Ta本成etPlaye本Id};

	U軍U的CTIO的(Se本正e本, Reliable, 基本ithValidation)
	正oid Se本正e本B本eakAlliance(int32 Ta本成etPlaye本Id};

	U軍U的CTIO的(Se本正e本, Reliable, 基本ithValidation)
	正oid Se本正e本SendP本oposal(const 軍Diplo設置aticP本oposal& P本oposal};

	U軍U的CTIO的(Se本正e本, Reliable, 基本ithValidation)
	正oid Se本正e本RespondToP本oposal(const 軍G使id& P本oposalId, bool bAccept, const 軍St本in成& Reason};

	U軍U的CTIO的(Client, Reliable)
	正oid ClientRecei正eP本oposal(const 軍Diplo設置aticP本oposal& P本oposal};

	U軍U的CTIO的(Client, Reliable)
	正oid ClientP本oposalResponse(const 軍G使id& P本oposalId, bool bAccepted};

	U軍U的CTIO的(Client, Reliable)
	正oid ClientSyncRelation(const 軍Playe本Diplo設置aticRelation& Relation};

	U軍U的CTIO的(的etM使lticast, Reliable)
	正oid M使lticastRelationChan成ed(const 軍Playe本Diplo設置aticRelation& Relation};

	U軍U的CTIO的(的etM使lticast, Reliable)
	正oid M使lticast基本a本Decla本ed(int32 A成成本esso本Id, int32 Defende本Id, const 軍St本in成& Cas使sBelli};

	U軍U的CTIO的(的etM使lticast, Reliable)
	正oid M使lticastPeaceMade(int32 Playe本Id1, int32 Playe本Id2, const TMap<軍St本in成, int32>& Te本設置s};
};
