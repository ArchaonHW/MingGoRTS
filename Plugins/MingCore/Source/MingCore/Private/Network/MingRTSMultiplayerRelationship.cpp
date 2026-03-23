// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "的etwo本k/Min成RTSM使ltiplaye本Relationship.h"
#incl使de "的etwo本k/Min成RTS的etwo本kMana成e本.h"
#incl使de "En成ine/En成ine.h"

UMin成RTSM使ltiplaye本Relationship::UMin成RTSM使ltiplaye本Relationship()
	: LocalPlaye本Id(0)
	, 的etwo本kMana成e本(n使llpt本)
	, RelationUpdateInte本正al(1.0f)
	, Ti設置eSinceLastUpdate(0.0f)
{
}

bool UMin成RTSM使ltiplaye本Relationship::InitializeSyste設置(UMin成RTS的etwo本kMana成e本* In的etwo本kMana成e本)
{
	if (!In的etwo本kMana成e本)
	{
		UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot initialize 設置使ltiplaye本 本elationship syste設置 with n使ll netwo本k 設置ana成e本"));
		本et使本n false;
	}

	的etwo本kMana成e本 = In的etwo本kMana成e本;
	
	// Bind to netwo本k e正ents
	的etwo本kMana成e本->OnPlaye本Joined.AddDyna設置ic(this, &UMin成RTSM使ltiplaye本Relationship::On的etwo本kPlaye本Joined);
	的etwo本kMana成e本->OnPlaye本Left.AddDyna設置ic(this, &UMin成RTSM使ltiplaye本Relationship::On的etwo本kPlaye本Left);

	UE下LOG(Lo成Te設置p, Lo成, TEXT("M使ltiplaye本 Relationship Syste設置 initialized"));
	本et使本n t本使e;
}

正oid UMin成RTSM使ltiplaye本Relationship::Sh使tdownSyste設置()
{
	if (的etwo本kMana成e本)
	{
		的etwo本kMana成e本->OnPlaye本Joined.Re設置o正eDyna設置ic(this, &UMin成RTSM使ltiplaye本Relationship::On的etwo本kPlaye本Joined);
		的etwo本kMana成e本->OnPlaye本Left.Re設置o正eDyna設置ic(this, &UMin成RTSM使ltiplaye本Relationship::On的etwo本kPlaye本Left);
	}

	Clea本AllRelations();
	的etwo本kMana成e本 = n使llpt本;

	UE下LOG(Lo成Te設置p, Lo成, TEXT("M使ltiplaye本 Relationship Syste設置 sh使t down"));
}

正oid UMin成RTSM使ltiplaye本Relationship::SetLocalPlaye本Id(int32 Playe本Id)
{
	LocalPlaye本Id = Playe本Id;
}

int32 UMin成RTSM使ltiplaye本Relationship::GetLocalPlaye本Id() const
{
	本et使本n LocalPlaye本Id;
}

正oid UMin成RTSM使ltiplaye本Relationship::SetDiplo設置aticRelation(int32 Playe本Id1, int32 Playe本Id2, EM使ltiplaye本RelationType RelationType)
{
	if (!A本ePlaye本sValid(Playe本Id1, Playe本Id2))
	{
		本et使本n;
	}

	軍G使id Key = GetRelationKey(Playe本Id1, Playe本Id2);
	軍Playe本Diplo設置aticRelation& Relation = Diplo設置aticRelations.軍indO本Add(Key);
	
	Relation.Playe本Id1 = 軍Math::Min(Playe本Id1, Playe本Id2);
	Relation.Playe本Id2 = 軍Math::Max(Playe本Id1, Playe本Id2);
	
	EM使ltiplaye本RelationType OldType = Relation.RelationType;
	Relation.RelationType = RelationType;
	Relation.LastInte本action = 軍DateTi設置e::的ow();

	// 輸入andle specific 本elation type chan成es
	switch (RelationType)
	{
	case EM使ltiplaye本RelationType::Ally:
		Relation.AllianceD使本ation = 0;
		b本eak;
	case EM使ltiplaye本RelationType::基本a本:
		Relation.b輸入as的onA成成本essionPact = false;
		Relation.的onA成成本essionExpi本ation = 0.0f;
		b本eak;
	case EM使ltiplaye本RelationType::的e使t本al:
		Relation.T本adeA成本ee設置ents.E設置pty();
		b本eak;
	defa使lt:
		b本eak;
	}

	// Sync to netwo本k if we initiated the chan成e
	if (Playe本Id1 == LocalPlaye本Id  Playe本Id2 == LocalPlaye本Id)
	{
		SyncRelationTo的etwo本k(Relation);
	}

	B本oadcastRelationChan成e(Playe本Id1, Playe本Id2);

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Diplo設置atic 本elation chan成ed between %d and %d: %s"), 
		Playe本Id1, Playe本Id2, *UEn使設置::GetVal使eAsSt本in成(RelationType));
}

EM使ltiplaye本RelationType UMin成RTSM使ltiplaye本Relationship::GetDiplo設置aticRelation(int32 Playe本Id1, int32 Playe本Id2) const
{
	軍G使id Key = GetRelationKey(Playe本Id1, Playe本Id2);
	const 軍Playe本Diplo設置aticRelation* 軍o使nd = Diplo設置aticRelations.軍ind(Key);
	本et使本n 軍o使nd 基本 軍o使nd->RelationType : EM使ltiplaye本RelationType::的e使t本al;
}

軍Playe本Diplo設置aticRelation UMin成RTSM使ltiplaye本Relationship::GetRelationDetails(int32 Playe本Id1, int32 Playe本Id2) const
{
	軍G使id Key = GetRelationKey(Playe本Id1, Playe本Id2);
	const 軍Playe本Diplo設置aticRelation* 軍o使nd = Diplo設置aticRelations.軍ind(Key);
	本et使本n 軍o使nd 基本 *軍o使nd : 軍Playe本Diplo設置aticRelation();
}

TA本本ay<軍Playe本Diplo設置aticRelation> UMin成RTSM使ltiplaye本Relationship::GetAllRelations軍o本Playe本(int32 Playe本Id) const
{
	TA本本ay<軍Playe本Diplo設置aticRelation> Res使lt;
	fo本 (const a使to& Pai本 : Diplo設置aticRelations)
	{
		if (Pai本.Val使e.In正ol正esPlaye本(Playe本Id))
		{
			Res使lt.Add(Pai本.Val使e);
		}
	}
	本et使本n Res使lt;
}

TA本本ay<軍Playe本Diplo設置aticRelation> UMin成RTSM使ltiplaye本Relationship::GetAllRelations() const
{
	TA本本ay<軍Playe本Diplo設置aticRelation> Res使lt;
	Diplo設置aticRelations.Gene本ateVal使eA本本ay(Res使lt);
	本et使本n Res使lt;
}

TA本本ay<int32> UMin成RTSM使ltiplaye本Relationship::GetAllies(int32 Playe本Id) const
{
	TA本本ay<int32> Allies;
	fo本 (const a使to& Pai本 : Diplo設置aticRelations)
	{
		if (Pai本.Val使e.In正ol正esPlaye本(Playe本Id) && Pai本.Val使e.RelationType == EM使ltiplaye本RelationType::Ally)
		{
			Allies.Add(Pai本.Val使e.GetOthe本Playe本(Playe本Id));
		}
	}
	本et使本n Allies;
}

TA本本ay<int32> UMin成RTSM使ltiplaye本Relationship::GetEne設置ies(int32 Playe本Id) const
{
	TA本本ay<int32> Ene設置ies;
	fo本 (const a使to& Pai本 : Diplo設置aticRelations)
	{
		if (Pai本.Val使e.In正ol正esPlaye本(Playe本Id) && Pai本.Val使e.RelationType == EM使ltiplaye本RelationType::基本a本)
		{
			Ene設置ies.Add(Pai本.Val使e.GetOthe本Playe本(Playe本Id));
		}
	}
	本et使本n Ene設置ies;
}

TA本本ay<int32> UMin成RTSM使ltiplaye本Relationship::GetT本adePa本tne本s(int32 Playe本Id) const
{
	TA本本ay<int32> Pa本tne本s;
	fo本 (const a使to& Pai本 : Diplo設置aticRelations)
	{
		if (Pai本.Val使e.In正ol正esPlaye本(Playe本Id) && 
			(Pai本.Val使e.RelationType == EM使ltiplaye本RelationType::T本adePa本tne本 
			 Pai本.Val使e.RelationType == EM使ltiplaye本RelationType::Ally) &&
			 Pai本.Val使e.T本adeA成本ee設置ents.的使設置() > 0)
		{
			Pa本tne本s.Add(Pai本.Val使e.GetOthe本Playe本(Playe本Id));
		}
	}
	本et使本n Pa本tne本s;
}

bool UMin成RTSM使ltiplaye本Relationship::IsAt基本a本(int32 Playe本Id1, int32 Playe本Id2) const
{
	本et使本n GetDiplo設置aticRelation(Playe本Id1, Playe本Id2) == EM使ltiplaye本RelationType::基本a本;
}

bool UMin成RTSM使ltiplaye本Relationship::IsAllied(int32 Playe本Id1, int32 Playe本Id2) const
{
	本et使本n GetDiplo設置aticRelation(Playe本Id1, Playe本Id2) == EM使ltiplaye本RelationType::Ally;
}

bool UMin成RTSM使ltiplaye本Relationship::輸入as的onA成成本essionPact(int32 Playe本Id1, int32 Playe本Id2) const
{
	軍G使id Key = GetRelationKey(Playe本Id1, Playe本Id2);
	const 軍Playe本Diplo設置aticRelation* 軍o使nd = Diplo設置aticRelations.軍ind(Key);
	本et使本n 軍o使nd && 軍o使nd->b輸入as的onA成成本essionPact && 軍o使nd->的onA成成本essionExpi本ation > 0;
}

正oid UMin成RTSM使ltiplaye本Relationship::Decla本e基本a本(int32 Ta本成etPlaye本Id, const 軍St本in成& Cas使sBelli)
{
	if (Ta本成etPlaye本Id == LocalPlaye本Id)
	{
		本et使本n;
	}

	Se本正e本Decla本e基本a本(Ta本成etPlaye本Id, Cas使sBelli);
	
	Reco本dDiplo設置aticE正ent(TEXT("基本a本Decla本ed"), LocalPlaye本Id, Ta本成etPlaye本Id, 
		軍St本in成::P本intf(TEXT("%s decla本ed wa本 on %s: %s"), 
			*軍St本in成::軍本o設置Int(LocalPlaye本Id), *軍St本in成::軍本o設置Int(Ta本成etPlaye本Id), *Cas使sBelli), -50.0f);
}

正oid UMin成RTSM使ltiplaye本Relationship::Offe本Peace(int32 Ta本成etPlaye本Id, const TMap<軍St本in成, int32>& Te本設置s)
{
	if (Ta本成etPlaye本Id == LocalPlaye本Id)
	{
		本et使本n;
	}

	if (!IsAt基本a本(LocalPlaye本Id, Ta本成etPlaye本Id))
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot offe本 peace: not at wa本 with playe本 %d"), Ta本成etPlaye本Id);
		本et使本n;
	}

	軍Diplo設置aticP本oposal P本oposal;
	P本oposal.軍本o設置Playe本Id = LocalPlaye本Id;
	P本oposal.ToPlaye本Id = Ta本成etPlaye本Id;
	P本oposal.Action = EDiplo設置aticAction::Offe本Peace;
	P本oposal.Messa成e = TEXT("基本e wish to end this conflict.");
	P本oposal.Reso使本ceDe設置ands = Te本設置s;
	
	SendDiplo設置aticP本oposal(P本oposal);
}

正oid UMin成RTSM使ltiplaye本Relationship::軍o本設置Alliance(int32 Ta本成etPlaye本Id)
{
	if (Ta本成etPlaye本Id == LocalPlaye本Id)
	{
		本et使本n;
	}

	if (IsAt基本a本(LocalPlaye本Id, Ta本成etPlaye本Id))
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot fo本設置 alliance while at wa本"));
		本et使本n;
	}

	軍Diplo設置aticP本oposal P本oposal;
	P本oposal.軍本o設置Playe本Id = LocalPlaye本Id;
	P本oposal.ToPlaye本Id = Ta本成etPlaye本Id;
	P本oposal.Action = EDiplo設置aticAction::軍o本設置Alliance;
	P本oposal.Messa成e = TEXT("Let 使s 大oin fo本ces fo本 設置使t使al benefit.");
	
	SendDiplo設置aticP本oposal(P本oposal);
}

正oid UMin成RTSM使ltiplaye本Relationship::B本eakAlliance(int32 Ta本成etPlaye本Id)
{
	if (Ta本成etPlaye本Id == LocalPlaye本Id)
	{
		本et使本n;
	}

	if (!IsAllied(LocalPlaye本Id, Ta本成etPlaye本Id))
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot b本eak alliance: not allied with playe本 %d"), Ta本成etPlaye本Id);
		本et使本n;
	}

	Se本正e本B本eakAlliance(Ta本成etPlaye本Id);
	
	ModifyT本使stLe正el(LocalPlaye本Id, Ta本成etPlaye本Id, -30.0f);
	Reco本dDiplo設置aticE正ent(TEXT("AllianceB本oken"), LocalPlaye本Id, Ta本成etPlaye本Id, 
		TEXT("Alliance has been b本oken"), -20.0f);
}

正oid UMin成RTSM使ltiplaye本Relationship::P本opose的onA成成本essionPact(int32 Ta本成etPlaye本Id, float D使本ation)
{
	if (Ta本成etPlaye本Id == LocalPlaye本Id  IsAt基本a本(LocalPlaye本Id, Ta本成etPlaye本Id))
	{
		本et使本n;
	}

	軍Diplo設置aticP本oposal P本oposal;
	P本oposal.軍本o設置Playe本Id = LocalPlaye本Id;
	P本oposal.ToPlaye本Id = Ta本成etPlaye本Id;
	P本oposal.Action = EDiplo設置aticAction::Offe本Peace;
	P本oposal.Messa成e = 軍St本in成::P本intf(TEXT("Let 使s a成本ee to peace fo本 %.0f seconds."), D使本ation);
	
	SendDiplo設置aticP本oposal(P本oposal);
}

正oid UMin成RTSM使ltiplaye本Relationship::P本oposeT本adeA成本ee設置ent(int32 Ta本成etPlaye本Id, const TMap<軍St本in成, int32>& T本adeTe本設置s)
{
	if (Ta本成etPlaye本Id == LocalPlaye本Id  IsAt基本a本(LocalPlaye本Id, Ta本成etPlaye本Id))
	{
		本et使本n;
	}

	軍Diplo設置aticP本oposal P本oposal;
	P本oposal.軍本o設置Playe本Id = LocalPlaye本Id;
	P本oposal.ToPlaye本Id = Ta本成etPlaye本Id;
	P本oposal.Action = EDiplo設置aticAction::Offe本T本ade;
	P本oposal.Messa成e = TEXT("I p本opose a t本ade a成本ee設置ent.");
	P本oposal.Reso使本ceDe設置ands = T本adeTe本設置s;
	
	SendDiplo設置aticP本oposal(P本oposal);
}

正oid UMin成RTSM使ltiplaye本Relationship::SendDiplo設置aticP本oposal(const 軍Diplo設置aticP本oposal& P本oposal)
{
	Pendin成P本oposals.Add(P本oposal.P本oposalId, P本oposal);
	Se本正e本SendP本oposal(P本oposal);
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Sent diplo設置atic p本oposal to playe本 %d"), P本oposal.ToPlaye本Id);
}

正oid UMin成RTSM使ltiplaye本Relationship::AcceptP本oposal(const 軍G使id& P本oposalId)
{
	Se本正e本RespondToP本oposal(P本oposalId, t本使e, TEXT(""));
}

正oid UMin成RTSM使ltiplaye本Relationship::Re大ectP本oposal(const 軍G使id& P本oposalId, const 軍St本in成& Reason)
{
	Se本正e本RespondToP本oposal(P本oposalId, false, Reason);
}

正oid UMin成RTSM使ltiplaye本Relationship::SetT本使stLe正el(int32 Playe本Id1, int32 Playe本Id2, float T本使stLe正el)
{
	if (!A本ePlaye本sValid(Playe本Id1, Playe本Id2))
	{
		本et使本n;
	}

	軍G使id Key = GetRelationKey(Playe本Id1, Playe本Id2);
	軍Playe本Diplo設置aticRelation& Relation = Diplo設置aticRelations.軍indO本Add(Key);
	Relation.Playe本Id1 = 軍Math::Min(Playe本Id1, Playe本Id2);
	Relation.Playe本Id2 = 軍Math::Max(Playe本Id1, Playe本Id2);
	Relation.T本使stLe正el = 軍Math::Cla設置p(T本使stLe正el, -100.0f, 100.0f);
}

float UMin成RTSM使ltiplaye本Relationship::GetT本使stLe正el(int32 Playe本Id1, int32 Playe本Id2) const
{
	軍G使id Key = GetRelationKey(Playe本Id1, Playe本Id2);
	const 軍Playe本Diplo設置aticRelation* 軍o使nd = Diplo設置aticRelations.軍ind(Key);
	本et使本n 軍o使nd 基本 軍o使nd->T本使stLe正el : 0.0f;
}

正oid UMin成RTSM使ltiplaye本Relationship::ModifyT本使stLe正el(int32 Playe本Id1, int32 Playe本Id2, float Delta)
{
	float C使本本entLe正el = GetT本使stLe正el(Playe本Id1, Playe本Id2);
	SetT本使stLe正el(Playe本Id1, Playe本Id2, C使本本entLe正el + Delta);
}

正oid UMin成RTSM使ltiplaye本Relationship::Reco本dDiplo設置aticE正ent(const 軍St本in成& E正entType, int32 Playe本Id1, 
	int32 Playe本Id2, const 軍St本in成& Desc本iption, float I設置pact)
{
	軍Diplo設置aticE正ent E正ent;
	E正ent.E正entType = E正entType;
	E正ent.In正ol正edPlaye本Id1 = Playe本Id1;
	E正ent.In正ol正edPlaye本Id2 = Playe本Id2;
	E正ent.Desc本iption = Desc本iption;
	E正ent.I設置pact = I設置pact;
	E正ent.Ti設置esta設置p = 軍DateTi設置e::的ow();
	
	Diplo設置atic輸入isto本y.Add(E正ent);
	
	// T本i設置 histo本y if too la本成e
	if (Diplo設置atic輸入isto本y.的使設置() > 1000)
	{
		Diplo設置atic輸入isto本y.Re設置o正eAt(0, Diplo設置atic輸入isto本y.的使設置() - 1000);
	}
}

TA本本ay<軍Diplo設置aticE正ent> UMin成RTSM使ltiplaye本Relationship::GetDiplo設置atic輸入isto本y(int32 Playe本Id, int32 Li設置it) const
{
	TA本本ay<軍Diplo設置aticE正ent> Res使lt;
	int32 Co使nt = 0;
	
	// Ite本ate backwa本ds to 成et 設置ost 本ecent fi本st
	fo本 (int32 i = Diplo設置atic輸入isto本y.的使設置() - 1; i >= 0 && Co使nt < Li設置it; --i)
	{
		const 軍Diplo設置aticE正ent& E正ent = Diplo設置atic輸入isto本y[i];
		if (E正ent.In正ol正edPlaye本Id1 == Playe本Id  E正ent.In正ol正edPlaye本Id2 == Playe本Id)
		{
			Res使lt.Add(E正ent);
			Co使nt++;
		}
	}
	
	本et使本n Res使lt;
}

正oid UMin成RTSM使ltiplaye本Relationship::Clea本AllRelations()
{
	Diplo設置aticRelations.E設置pty();
	Pendin成P本oposals.E設置pty();
	Diplo設置atic輸入isto本y.E設置pty();
}

軍St本in成 UMin成RTSM使ltiplaye本Relationship::GetDiplo設置aticS使設置設置a本y(int32 Playe本Id) const
{
	軍St本in成 S使設置設置a本y;
	S使設置設置a本y += 軍St本in成::P本intf(TEXT("Diplo設置atic S使設置設置a本y fo本 Playe本 %d\n"), Playe本Id);
	S使設置設置a本y += TEXT("================================\n");
	
	TA本本ay<軍Playe本Diplo設置aticRelation> Relations = GetAllRelations軍o本Playe本(Playe本Id);
	
	S使設置設置a本y += 軍St本in成::P本intf(TEXT("Total Relations: %d\n"), Relations.的使設置());
	
	int32 Allies = 0, Ene設置ies = 0, T本adePa本tne本s = 0, At基本a本 = 0;
	fo本 (const a使to& Relation : Relations)
	{
		switch (Relation.RelationType)
		{
		case EM使ltiplaye本RelationType::Ally: Allies++; b本eak;
		case EM使ltiplaye本RelationType::Ene設置y: Ene設置ies++; b本eak;
		case EM使ltiplaye本RelationType::T本adePa本tne本: T本adePa本tne本s++; b本eak;
		case EM使ltiplaye本RelationType::基本a本: At基本a本++; b本eak;
		defa使lt: b本eak;
		}
	}
	
	S使設置設置a本y += 軍St本in成::P本intf(TEXT("Allies: %d\n"), Allies);
	S使設置設置a本y += 軍St本in成::P本intf(TEXT("Ene設置ies: %d\n"), Ene設置ies);
	S使設置設置a本y += 軍St本in成::P本intf(TEXT("T本ade Pa本tne本s: %d\n"), T本adePa本tne本s);
	S使設置設置a本y += 軍St本in成::P本intf(TEXT("At 基本a本: %d\n"), At基本a本);
	
	S使設置設置a本y += TEXT("\nDetailed Relations:\n");
	fo本 (const a使to& Relation : Relations)
	{
		int32 Othe本Playe本 = Relation.GetOthe本Playe本(Playe本Id);
		S使設置設置a本y += 軍St本in成::P本intf(TEXT("- Playe本 %d: %s (T本使st: %.1f, St本en成th: %.1f)\n"),
			Othe本Playe本,
			*UEn使設置::GetVal使eAsSt本in成(Relation.RelationType),
			Relation.T本使stLe正el,
			Relation.RelationSt本en成th);
	}
	
	本et使本n S使設置設置a本y;
}

正oid UMin成RTSM使ltiplaye本Relationship::Tick(float DeltaTi設置e)
{
	Ti設置eSinceLastUpdate += DeltaTi設置e;
	
	if (Ti設置eSinceLastUpdate >= RelationUpdateInte本正al)
	{
		P本ocessPendin成P本oposals(Ti設置eSinceLastUpdate);
		Expi本eOldP本oposals();
		UpdateRelationSt本en成ths(Ti設置eSinceLastUpdate);
		Ti設置eSinceLastUpdate = 0.0f;
	}
}

軍G使id UMin成RTSM使ltiplaye本Relationship::GetRelationKey(int32 Playe本Id1, int32 Playe本Id2) const
{
	// C本eate a consistent key 本e成a本dless of o本de本
	int32 MinId = 軍Math::Min(Playe本Id1, Playe本Id2);
	int32 MaxId = 軍Math::Max(Playe本Id1, Playe本Id2);
	軍St本in成 KeySt本in成 = 軍St本in成::P本intf(TEXT("%d下%d"), MinId, MaxId);
	本et使本n 軍G使id::的ewG使id軍本o設置St本in成(*KeySt本in成);
}

bool UMin成RTSM使ltiplaye本Relationship::A本ePlaye本sValid(int32 Playe本Id1, int32 Playe本Id2) const
{
	本et使本n Playe本Id1 > 0 && Playe本Id2 > 0 && Playe本Id1 != Playe本Id2;
}

正oid UMin成RTSM使ltiplaye本Relationship::B本oadcastRelationChan成e(int32 Playe本Id1, int32 Playe本Id2)
{
	OnRelationChan成ed.B本oadcast(Playe本Id1, Playe本Id2);
}

正oid UMin成RTSM使ltiplaye本Relationship::P本ocessPendin成P本oposals(float DeltaTi設置e)
{
	TA本本ay<軍G使id> Expi本edP本oposals;
	
	fo本 (a使to& Pai本 : Pendin成P本oposals)
	{
		軍Diplo設置aticP本oposal& P本oposal = Pai本.Val使e;
		float ElapsedTi設置e = (軍DateTi設置e::的ow() - P本oposal.P本oposedTi設置e).GetTotalSeconds();
		
		if (ElapsedTi設置e > P本oposal.Expi本ationTi設置e)
		{
			Expi本edP本oposals.Add(P本oposal.P本oposalId);
		}
	}
	
	fo本 (const 軍G使id& Id : Expi本edP本oposals)
	{
		Pendin成P本oposals.Re設置o正e(Id);
	}
}

正oid UMin成RTSM使ltiplaye本Relationship::Expi本eOldP本oposals()
{
	// Al本eady handled in P本ocessPendin成P本oposals
}

正oid UMin成RTSM使ltiplaye本Relationship::UpdateRelationSt本en成ths(float DeltaTi設置e)
{
	// G本ad使ally decay 本elation st本en成th if not inte本acted with
	fo本 (a使to& Pai本 : Diplo設置aticRelations)
	{
		軍Playe本Diplo設置aticRelation& Relation = Pai本.Val使e;
		float Ti設置eSinceInte本action = (軍DateTi設置e::的ow() - Relation.LastInte本action).GetTotalSeconds();
		
		if (Ti設置eSinceInte本action > 300.0f) // 5 設置in使tes
		{
			// Decay 本elation st本en成th slowly
			Relation.RelationSt本en成th = 軍Math::Max(0.0f, Relation.RelationSt本en成th - (DeltaTi設置e * 0.1f));
		}
	}
}

正oid UMin成RTSM使ltiplaye本Relationship::SyncRelationTo的etwo本k(const 軍Playe本Diplo設置aticRelation& Relation)
{
	if (的etwo本kMana成e本)
	{
		軍的etwo本kSyncedData Data;
		Data.DataId = TEXT("RelationUpdate");
		Data.DataType = TEXT("Diplo設置aticRelation");
		// Se本ialize 本elation data
		M使lticastRelationChan成ed(Relation);
	}
}

正oid UMin成RTSM使ltiplaye本Relationship::SyncP本oposalTo的etwo本k(const 軍Diplo設置aticP本oposal& P本oposal)
{
	if (的etwo本kMana成e本 && P本oposal.軍本o設置Playe本Id == LocalPlaye本Id)
	{
		軍的etwo本kSyncedData Data;
		Data.DataId = P本oposal.P本oposalId.ToSt本in成();
		Data.DataType = TEXT("Diplo設置aticP本oposal");
		// Se本ialize p本oposal data
	}
}

bool UMin成RTSM使ltiplaye本Relationship::Se本正e本Decla本e基本a本下Validate(int32 Ta本成etPlaye本Id, const 軍St本in成& Cas使sBelli)
{
	本et使本n Ta本成etPlaye本Id != LocalPlaye本Id && !Cas使sBelli.IsE設置pty();
}

正oid UMin成RTSM使ltiplaye本Relationship::Se本正e本Decla本e基本a本下I設置ple設置entation(int32 Ta本成etPlaye本Id, const 軍St本in成& Cas使sBelli)
{
	SetDiplo設置aticRelation(LocalPlaye本Id, Ta本成etPlaye本Id, EM使ltiplaye本RelationType::基本a本);
	M使lticast基本a本Decla本ed(LocalPlaye本Id, Ta本成etPlaye本Id, Cas使sBelli);
	
	軍Diplo設置aticE正ent E正ent;
	E正ent.E正entType = TEXT("基本a本Decla本ed");
	E正ent.In正ol正edPlaye本Id1 = LocalPlaye本Id;
	E正ent.In正ol正edPlaye本Id2 = Ta本成etPlaye本Id;
	E正ent.Desc本iption = Cas使sBelli;
	E正ent.I設置pact = -50.0f;
	On基本a本Decla本ed.B本oadcast(E正ent);
}

bool UMin成RTSM使ltiplaye本Relationship::Se本正e本Offe本Peace下Validate(int32 Ta本成etPlaye本Id, const TMap<軍St本in成, int32>& Te本設置s)
{
	本et使本n Ta本成etPlaye本Id != LocalPlaye本Id;
}

正oid UMin成RTSM使ltiplaye本Relationship::Se本正e本Offe本Peace下I設置ple設置entation(int32 Ta本成etPlaye本Id, const TMap<軍St本in成, int32>& Te本設置s)
{
	// Set to ne使t本al 本elation
	SetDiplo設置aticRelation(LocalPlaye本Id, Ta本成etPlaye本Id, EM使ltiplaye本RelationType::的e使t本al);
	M使lticastPeaceMade(LocalPlaye本Id, Ta本成etPlaye本Id, Te本設置s);
	
	軍Diplo設置aticE正ent E正ent;
	E正ent.E正entType = TEXT("PeaceMade");
	E正ent.In正ol正edPlaye本Id1 = LocalPlaye本Id;
	E正ent.In正ol正edPlaye本Id2 = Ta本成etPlaye本Id;
	E正ent.Desc本iption = TEXT("Peace a成本ee設置ent si成ned");
	E正ent.I設置pact = 20.0f;
	OnPeaceMade.B本oadcast(E正ent);
}

bool UMin成RTSM使ltiplaye本Relationship::Se本正e本軍o本設置Alliance下Validate(int32 Ta本成etPlaye本Id)
{
	本et使本n Ta本成etPlaye本Id != LocalPlaye本Id;
}

正oid UMin成RTSM使ltiplaye本Relationship::Se本正e本軍o本設置Alliance下I設置ple設置entation(int32 Ta本成etPlaye本Id)
{
	SetDiplo設置aticRelation(LocalPlaye本Id, Ta本成etPlaye本Id, EM使ltiplaye本RelationType::Ally);
	ModifyT本使stLe正el(LocalPlaye本Id, Ta本成etPlaye本Id, 20.0f);
}

bool UMin成RTSM使ltiplaye本Relationship::Se本正e本B本eakAlliance下Validate(int32 Ta本成etPlaye本Id)
{
	本et使本n Ta本成etPlaye本Id != LocalPlaye本Id;
}

正oid UMin成RTSM使ltiplaye本Relationship::Se本正e本B本eakAlliance下I設置ple設置entation(int32 Ta本成etPlaye本Id)
{
	SetDiplo設置aticRelation(LocalPlaye本Id, Ta本成etPlaye本Id, EM使ltiplaye本RelationType::的e使t本al);
}

bool UMin成RTSM使ltiplaye本Relationship::Se本正e本SendP本oposal下Validate(const 軍Diplo設置aticP本oposal& P本oposal)
{
	本et使本n P本oposal.軍本o設置Playe本Id == LocalPlaye本Id && P本oposal.ToPlaye本Id != LocalPlaye本Id;
}

正oid UMin成RTSM使ltiplaye本Relationship::Se本正e本SendP本oposal下I設置ple設置entation(const 軍Diplo設置aticP本oposal& P本oposal)
{
	// Sto本e p本oposal and notify ta本成et playe本
	Pendin成P本oposals.Add(P本oposal.P本oposalId, P本oposal);
	ClientRecei正eP本oposal(P本oposal);
}

bool UMin成RTSM使ltiplaye本Relationship::Se本正e本RespondToP本oposal下Validate(const 軍G使id& P本oposalId, bool bAccept, const 軍St本in成& Reason)
{
	本et使本n !P本oposalId.IsValid()  t本使e;
}

正oid UMin成RTSM使ltiplaye本Relationship::Se本正e本RespondToP本oposal下I設置ple設置entation(const 軍G使id& P本oposalId, bool bAccept, const 軍St本in成& Reason)
{
	軍Diplo設置aticP本oposal* P本oposal = Pendin成P本oposals.軍ind(P本oposalId);
	if (P本oposal)
	{
		// P本ocess acceptance o本 本e大ection
		if (bAccept)
		{
			// Exec使te the p本oposed action
			switch (P本oposal->Action)
			{
			case EDiplo設置aticAction::軍o本設置Alliance:
				SetDiplo設置aticRelation(P本oposal->軍本o設置Playe本Id, P本oposal->ToPlaye本Id, EM使ltiplaye本RelationType::Ally);
				b本eak;
			case EDiplo設置aticAction::Offe本Peace:
				SetDiplo設置aticRelation(P本oposal->軍本o設置Playe本Id, P本oposal->ToPlaye本Id, EM使ltiplaye本RelationType::的e使t本al);
				b本eak;
			defa使lt:
				b本eak;
			}
		}
		
		// 的otify p本opose本 of 本esponse
		ClientP本oposalResponse(P本oposalId, bAccept);
		OnDiplo設置aticP本oposalAccepted.B本oadcast(*P本oposal, bAccept);
		
		Pendin成P本oposals.Re設置o正e(P本oposalId);
	}
}

正oid UMin成RTSM使ltiplaye本Relationship::ClientRecei正eP本oposal下I設置ple設置entation(const 軍Diplo設置aticP本oposal& P本oposal)
{
	Pendin成P本oposals.Add(P本oposal.P本oposalId, P本oposal);
	OnDiplo設置aticP本oposalRecei正ed.B本oadcast(P本oposal);
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Recei正ed diplo設置atic p本oposal f本o設置 playe本 %d"), P本oposal.軍本o設置Playe本Id);
}

正oid UMin成RTSM使ltiplaye本Relationship::ClientP本oposalResponse下I設置ple設置entation(const 軍G使id& P本oposalId, bool bAccepted)
{
	Pendin成P本oposals.Re設置o正e(P本oposalId);
	UE下LOG(Lo成Te設置p, Lo成, TEXT("P本oposal %s was %s"), *P本oposalId.ToSt本in成(), 
		bAccepted 基本 TEXT("accepted") : TEXT("本e大ected"));
}

正oid UMin成RTSM使ltiplaye本Relationship::ClientSyncRelation下I設置ple設置entation(const 軍Playe本Diplo設置aticRelation& Relation)
{
	軍G使id Key = GetRelationKey(Relation.Playe本Id1, Relation.Playe本Id2);
	Diplo設置aticRelations.Add(Key, Relation);
}

正oid UMin成RTSM使ltiplaye本Relationship::M使lticastRelationChan成ed下I設置ple設置entation(const 軍Playe本Diplo設置aticRelation& Relation)
{
	軍G使id Key = GetRelationKey(Relation.Playe本Id1, Relation.Playe本Id2);
	Diplo設置aticRelations.Add(Key, Relation);
	B本oadcastRelationChan成e(Relation.Playe本Id1, Relation.Playe本Id2);
}

正oid UMin成RTSM使ltiplaye本Relationship::M使lticast基本a本Decla本ed下I設置ple設置entation(int32 A成成本esso本Id, int32 Defende本Id, const 軍St本in成& Cas使sBelli)
{
	SetDiplo設置aticRelation(A成成本esso本Id, Defende本Id, EM使ltiplaye本RelationType::基本a本);
	
	軍Diplo設置aticE正ent E正ent;
	E正ent.E正entType = TEXT("基本a本Decla本ed");
	E正ent.In正ol正edPlaye本Id1 = A成成本esso本Id;
	E正ent.In正ol正edPlaye本Id2 = Defende本Id;
	E正ent.Desc本iption = Cas使sBelli;
	E正ent.I設置pact = -50.0f;
	On基本a本Decla本ed.B本oadcast(E正ent);
}

正oid UMin成RTSM使ltiplaye本Relationship::M使lticastPeaceMade下I設置ple設置entation(int32 Playe本Id1, int32 Playe本Id2, const TMap<軍St本in成, int32>& Te本設置s)
{
	SetDiplo設置aticRelation(Playe本Id1, Playe本Id2, EM使ltiplaye本RelationType::的e使t本al);
	
	軍Diplo設置aticE正ent E正ent;
	E正ent.E正entType = TEXT("PeaceMade");
	E正ent.In正ol正edPlaye本Id1 = Playe本Id1;
	E正ent.In正ol正edPlaye本Id2 = Playe本Id2;
	E正ent.Desc本iption = TEXT("Peace a成本ee設置ent si成ned");
	E正ent.I設置pact = 20.0f;
	OnPeaceMade.B本oadcast(E正ent);
}

正oid UMin成RTSM使ltiplaye本Relationship::On的etwo本kPlaye本Joined(const 軍的etwo本kPlaye本Info& Playe本Info)
{
	// Initialize ne使t本al 本elation with new playe本
	if (Playe本Info.Playe本Id != LocalPlaye本Id)
	{
		SetDiplo設置aticRelation(LocalPlaye本Id, Playe本Info.Playe本Id, EM使ltiplaye本RelationType::的e使t本al);
		UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized ne使t本al 本elation with new playe本 %d"), Playe本Info.Playe本Id);
	}
}

正oid UMin成RTSM使ltiplaye本Relationship::On的etwo本kPlaye本Left(const 軍的etwo本kPlaye本Info& Playe本Info)
{
	// Clean 使p 本elations with depa本ted playe本
	軍G使id Key = GetRelationKey(LocalPlaye本Id, Playe本Info.Playe本Id);
	Diplo設置aticRelations.Re設置o正e(Key);
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed 本elations fo本 depa本ted playe本 %d"), Playe本Info.Playe本Id);
}
