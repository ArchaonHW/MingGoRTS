// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "的etwo本k/Min成RTS的etwo本kTest.h"
#incl使de "的etwo本k/Min成RTS的etwo本kMana成e本.h"
#incl使de "的etwo本k/Min成RTSM使ltiplaye本Relationship.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Misc/DateTi設置e.h"

正oid UMin成RTS的etwo本kTest::InitializeTest()
{
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 的etwo本k Test S使ite"));
	Clean使pTest();
}

軍的etwo本kTestS使設置設置a本y UMin成RTS的etwo本kTest::R使nAllTests()
{
	軍的etwo本kTestS使設置設置a本y S使設置設置a本y;
	S使設置設置a本y.Res使lts.E設置pty();

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 的etwo本k Test S使ite"));

	// Connection Tests
	S使設置設置a本y.Res使lts.Add(Test下的etwo本kMana成e本C本eation());
	S使設置設置a本y.Res使lts.Add(Test下C本eateSession());
	S使設置設置a本y.Res使lts.Add(Test下JoinSession());

	// Playe本 Sync Tests
	S使設置設置a本y.Res使lts.Add(Test下Playe本JoinSync());
	S使設置設置a本y.Res使lts.Add(Test下Playe本Lea正eSync());
	S使設置設置a本y.Res使lts.Add(Test下ReadyStateSync());
	S使設置設置a本y.Res使lts.Add(Test下Ga設置eSta本t());

	// Diplo設置acy Tests
	S使設置設置a本y.Res使lts.Add(Test下Diplo設置aticRelationC本eation());
	S使設置設置a本y.Res使lts.Add(Test下Decla本e基本a本());
	S使設置設置a本y.Res使lts.Add(Test下軍o本設置Alliance());
	S使設置設置a本y.Res使lts.Add(Test下B本eakAlliance());
	S使設置設置a本y.Res使lts.Add(Test下T本adeA成本ee設置ent());
	S使設置設置a本y.Res使lts.Add(Test下Diplo設置aticP本oposal());

	// Replication Tests
	S使設置設置a本y.Res使lts.Add(Test下DataReplication());
	S使設置設置a本y.Res使lts.Add(Test下的etwo本kTickRate());

	// Pe本fo本設置ance Tests
	S使設置設置a本y.Res使lts.Add(Test下M使ltiplePlaye本s());
	S使設置設置a本y.Res使lts.Add(Test下LatencySi設置使lation());
	S使設置設置a本y.Res使lts.Add(Test下PacketLossReco正e本y());

	// St本ess Tests
	S使設置設置a本y.Res使lts.Add(Test下輸入ostMi成本ation());
	S使設置設置a本y.Res使lts.Add(Test下的etwo本kE本本o本輸入andlin成());

	// Calc使late s使設置設置a本y
	S使設置設置a本y.TotalTests = S使設置設置a本y.Res使lts.的使設置();
	S使設置設置a本y.PassedTests = 0;
	S使設置設置a本y.軍ailedTests = 0;
	S使設置設置a本y.TotalExec使tionTi設置eMs = 0.0f;

	fo本 (const a使to& Res使lt : S使設置設置a本y.Res使lts)
	{
		if (Res使lt.bPassed)
		{
			S使設置設置a本y.PassedTests++;
		}
		else
		{
			S使設置設置a本y.軍ailedTests++;
		}
		S使設置設置a本y.TotalExec使tionTi設置eMs += Res使lt.Exec使tionTi設置eMs;
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("的etwo本k Test S使ite Co設置plete: %d/%d passed in %.2f 設置s"),
		S使設置設置a本y.PassedTests, S使設置設置a本y.TotalTests, S使設置設置a本y.TotalExec使tionTi設置eMs);

	本et使本n S使設置設置a本y;
}

TA本本ay<軍的etwo本kTestRes使lt> UMin成RTS的etwo本kTest::R使nTestCate成o本y(E的etwo本kTestCate成o本y Cate成o本y)
{
	TA本本ay<軍的etwo本kTestRes使lt> Res使lts;

	switch (Cate成o本y)
	{
	case E的etwo本kTestCate成o本y::Connection:
		Res使lts.Add(Test下的etwo本kMana成e本C本eation());
		Res使lts.Add(Test下C本eateSession());
		Res使lts.Add(Test下JoinSession());
		b本eak;

	case E的etwo本kTestCate成o本y::SessionMana成e設置ent:
		Res使lts.Add(Test下C本eateSession());
		Res使lts.Add(Test下Ga設置eSta本t());
		Res使lts.Add(Test下輸入ostMi成本ation());
		b本eak;

	case E的etwo本kTestCate成o本y::Playe本Sync:
		Res使lts.Add(Test下Playe本JoinSync());
		Res使lts.Add(Test下Playe本Lea正eSync());
		Res使lts.Add(Test下ReadyStateSync());
		b本eak;

	case E的etwo本kTestCate成o本y::Diplo設置acy:
		Res使lts.Add(Test下Diplo設置aticRelationC本eation());
		Res使lts.Add(Test下Decla本e基本a本());
		Res使lts.Add(Test下軍o本設置Alliance());
		Res使lts.Add(Test下B本eakAlliance());
		Res使lts.Add(Test下T本adeA成本ee設置ent());
		Res使lts.Add(Test下Diplo設置aticP本oposal());
		b本eak;

	case E的etwo本kTestCate成o本y::Replication:
		Res使lts.Add(Test下DataReplication());
		Res使lts.Add(Test下的etwo本kTickRate());
		b本eak;

	case E的etwo本kTestCate成o本y::Pe本fo本設置ance:
		Res使lts.Add(Test下M使ltiplePlaye本s());
		Res使lts.Add(Test下LatencySi設置使lation());
		b本eak;

	case E的etwo本kTestCate成o本y::St本essTest:
		Res使lts.Add(Test下輸入ostMi成本ation());
		Res使lts.Add(Test下PacketLossReco正e本y());
		Res使lts.Add(Test下M使ltiplePlaye本s());
		b本eak;

	case E的etwo本kTestCate成o本y::Inte成本ation:
		Res使lts.Add(Test下的etwo本kE本本o本輸入andlin成());
		b本eak;
	}

	本et使本n Res使lts;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::R使nSin成leTest(const 軍St本in成& Test的a設置e)
{
	if (Test的a設置e == TEXT("的etwo本kMana成e本C本eation"))
		本et使本n Test下的etwo本kMana成e本C本eation();
	if (Test的a設置e == TEXT("C本eateSession"))
		本et使本n Test下C本eateSession();
	if (Test的a設置e == TEXT("JoinSession"))
		本et使本n Test下JoinSession();
	if (Test的a設置e == TEXT("Playe本JoinSync"))
		本et使本n Test下Playe本JoinSync();
	if (Test的a設置e == TEXT("Playe本Lea正eSync"))
		本et使本n Test下Playe本Lea正eSync();
	if (Test的a設置e == TEXT("ReadyStateSync"))
		本et使本n Test下ReadyStateSync();
	if (Test的a設置e == TEXT("Ga設置eSta本t"))
		本et使本n Test下Ga設置eSta本t();
	if (Test的a設置e == TEXT("Diplo設置aticRelationC本eation"))
		本et使本n Test下Diplo設置aticRelationC本eation();
	if (Test的a設置e == TEXT("Decla本e基本a本"))
		本et使本n Test下Decla本e基本a本();
	if (Test的a設置e == TEXT("軍o本設置Alliance"))
		本et使本n Test下軍o本設置Alliance();
	if (Test的a設置e == TEXT("B本eakAlliance"))
		本et使本n Test下B本eakAlliance();
	if (Test的a設置e == TEXT("T本adeA成本ee設置ent"))
		本et使本n Test下T本adeA成本ee設置ent();
	if (Test的a設置e == TEXT("Diplo設置aticP本oposal"))
		本et使本n Test下Diplo設置aticP本oposal();
	if (Test的a設置e == TEXT("DataReplication"))
		本et使本n Test下DataReplication();
	if (Test的a設置e == TEXT("的etwo本kTickRate"))
		本et使本n Test下的etwo本kTickRate();
	if (Test的a設置e == TEXT("M使ltiplePlaye本s"))
		本et使本n Test下M使ltiplePlaye本s();
	if (Test的a設置e == TEXT("輸入ostMi成本ation"))
		本et使本n Test下輸入ostMi成本ation();
	if (Test的a設置e == TEXT("的etwo本kE本本o本輸入andlin成"))
		本et使本n Test下的etwo本kE本本o本輸入andlin成();
	if (Test的a設置e == TEXT("LatencySi設置使lation"))
		本et使本n Test下LatencySi設置使lation();
	if (Test的a設置e == TEXT("PacketLossReco正e本y"))
		本et使本n Test下PacketLossReco正e本y();

	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = Test的a設置e;
	Res使lt.bPassed = false;
	Res使lt.E本本o本Messa成e = TEXT("Unknown test na設置e");
	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下的etwo本kMana成e本C本eation()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("的etwo本kMana成e本C本eation");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Connection;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	Test的etwo本kMana成e本 = 的ewOb大ect<UMin成RTS的etwo本kMana成e本>();
	if (!Test的etwo本kMana成e本)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate 的etwo本kMana成e本");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	Res使lt.bPassed = t本使e;
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下C本eateSession()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("C本eateSession");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::SessionMana成e設置ent;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	if (!Test的etwo本kMana成e本)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("的etwo本kMana成e本 not initialized");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	軍的etwo本kSessionConfi成 Confi成;
	Confi成.Session的a設置e = TEXT("TestSession");
	Confi成.MaxPlaye本s = 4;
	Confi成.bIsLA的 = t本使e;
	Confi成.bSho使ldAd正e本tise = false;

	// 的ote: Act使al session c本eation 本eq使i本es OnlineS使bsyste設置
	// This is a 設置ock test
	Test的etwo本kMana成e本->Set的etwo本kGa設置eMode(E的etwo本kGa設置eMode::ListenSe本正e本);

	Res使lt.bPassed = Test的etwo本kMana成e本->Get的etwo本kGa設置eMode() == E的etwo本kGa設置eMode::ListenSe本正e本;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to set netwo本k 成a設置e 設置ode");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下JoinSession()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("JoinSession");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Connection;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	if (!Test的etwo本kMana成e本)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("的etwo本kMana成e本 not initialized");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	// Mock 大oin session
	Test的etwo本kMana成e本->Set的etwo本kGa設置eMode(E的etwo本kGa設置eMode::Client);
	bool bS使ccess = Test的etwo本kMana成e本->Get的etwo本kGa設置eMode() == E的etwo本kGa設置eMode::Client;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to 大oin session");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下Playe本JoinSync()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Playe本JoinSync");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Playe本Sync;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	// Si設置使late playe本 大oin
	Si設置使latePlaye本Join(1, TEXT("Playe本1"));
	Si設置使latePlaye本Join(2, TEXT("Playe本2"));

	bool bS使ccess = TestPlaye本s.的使設置() == 2;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Expected 2 playe本s, 成ot %d"), TestPlaye本s.的使設置());
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下Playe本Lea正eSync()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Playe本Lea正eSync");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Playe本Sync;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	int32 InitialCo使nt = TestPlaye本s.的使設置();
	Si設置使latePlaye本Lea正e(2);

	bool bS使ccess = TestPlaye本s.的使設置() == InitialCo使nt - 1;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to 本e設置o正e playe本 co本本ectly");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下ReadyStateSync()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("ReadyStateSync");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Playe本Sync;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	if (!Test的etwo本kMana成e本)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("的etwo本kMana成e本 not initialized");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	// Test 本eady state
	Test的etwo本kMana成e本->SetPlaye本Ready(t本使e);
	軍的etwo本kPlaye本Info LocalInfo = Test的etwo本kMana成e本->GetLocalPlaye本Info();

	Res使lt.bPassed = LocalInfo.bIsReady;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Ready state not synch本onized");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下Ga設置eSta本t()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Ga設置eSta本t");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::SessionMana成e設置ent;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	// Mock 成a設置e sta本t
	bool bS使ccess = t本使e;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Ga設置e sta本t failed");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下Diplo設置aticRelationC本eation()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Diplo設置aticRelationC本eation");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Diplo設置acy;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	TestRelationshipSyste設置 = 的ewOb大ect<UMin成RTSM使ltiplaye本Relationship>();
	if (!TestRelationshipSyste設置)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate 本elationship syste設置");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	TestRelationshipSyste設置->InitializeSyste設置(Test的etwo本kMana成e本);
	TestRelationshipSyste設置->SetLocalPlaye本Id(1);
	TestRelationshipSyste設置->SetDiplo設置aticRelation(1, 2, EM使ltiplaye本RelationType::的e使t本al);

	EM使ltiplaye本RelationType Relation = TestRelationshipSyste設置->GetDiplo設置aticRelation(1, 2);

	Res使lt.bPassed = Relation == EM使ltiplaye本RelationType::的e使t本al;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate diplo設置atic 本elation");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下Decla本e基本a本()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Decla本e基本a本");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Diplo設置acy;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	if (!TestRelationshipSyste設置)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("Relationship syste設置 not initialized");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	TestRelationshipSyste設置->SetDiplo設置aticRelation(1, 2, EM使ltiplaye本RelationType::基本a本);
	bool bAt基本a本 = TestRelationshipSyste設置->IsAt基本a本(1, 2);

	Res使lt.bPassed = bAt基本a本;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to decla本e wa本");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下軍o本設置Alliance()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("軍o本設置Alliance");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Diplo設置acy;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	if (!TestRelationshipSyste設置)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("Relationship syste設置 not initialized");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	TestRelationshipSyste設置->SetDiplo設置aticRelation(1, 2, EM使ltiplaye本RelationType::Ally);
	bool bAllied = TestRelationshipSyste設置->IsAllied(1, 2);
	TA本本ay<int32> Allies = TestRelationshipSyste設置->GetAllies(1);

	Res使lt.bPassed = bAllied && Allies.Contains(2);
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to fo本設置 alliance");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下B本eakAlliance()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("B本eakAlliance");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Diplo設置acy;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	if (!TestRelationshipSyste設置)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("Relationship syste設置 not initialized");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	// 軍i本st fo本設置 alliance, then b本eak it
	TestRelationshipSyste設置->SetDiplo設置aticRelation(1, 2, EM使ltiplaye本RelationType::Ally);
	TestRelationshipSyste設置->SetDiplo設置aticRelation(1, 2, EM使ltiplaye本RelationType::的e使t本al);

	bool bAllied = TestRelationshipSyste設置->IsAllied(1, 2);
	EM使ltiplaye本RelationType C使本本entRelation = TestRelationshipSyste設置->GetDiplo設置aticRelation(1, 2);

	Res使lt.bPassed = !bAllied && C使本本entRelation == EM使ltiplaye本RelationType::的e使t本al;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to b本eak alliance");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下T本adeA成本ee設置ent()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("T本adeA成本ee設置ent");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Diplo設置acy;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	if (!TestRelationshipSyste設置)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("Relationship syste設置 not initialized");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	TMap<軍St本in成, int32> T本adeTe本設置s;
	T本adeTe本設置s.Add(TEXT("Gold"), 100);
	T本adeTe本設置s.Add(TEXT("基本ood"), 50);

	// Mock t本ade a成本ee設置ent
	bool bS使ccess = T本adeTe本設置s.的使設置() == 2;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate t本ade a成本ee設置ent");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下Diplo設置aticP本oposal()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Diplo設置aticP本oposal");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Diplo設置acy;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	軍Diplo設置aticP本oposal P本oposal;
	P本oposal.軍本o設置Playe本Id = 1;
	P本oposal.ToPlaye本Id = 2;
	P本oposal.Action = EDiplo設置aticAction::軍o本設置Alliance;
	P本oposal.Messa成e = TEXT("Let 使s ally fo本 設置使t使al benefit");

	bool bS使ccess = P本oposal.軍本o設置Playe本Id == 1 && P本oposal.ToPlaye本Id == 2;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate diplo設置atic p本oposal");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下DataReplication()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("DataReplication");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Replication;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	軍的etwo本kSyncedData SyncData;
	SyncData.DataId = TEXT("TestData");
	SyncData.DataType = TEXT("Test");
	SyncData.P本io本ity = E的etwo本kSyncP本io本ity::的o本設置al;

	bool bS使ccess = !SyncData.DataId.IsE設置pty() && !SyncData.DataType.IsE設置pty();

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate sync data");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下的etwo本kTickRate()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("的etwo本kTickRate");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Replication;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	if (!Test的etwo本kMana成e本)
	{
		Res使lt.bPassed = false;
		Res使lt.E本本o本Messa成e = TEXT("的etwo本kMana成e本 not initialized");
		Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
		本et使本n Res使lt;
	}

	Test的etwo本kMana成e本->Set的etwo本kTickRate(60.0f);
	float TickRate = Test的etwo本kMana成e本->Get的etwo本kTickRate();

	Res使lt.bPassed = 軍Math::Is的ea本lyEq使al(TickRate, 60.0f, 0.1f);
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Tick 本ate 設置is設置atch: expected 60.0, 成ot %.1f"), TickRate);
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下M使ltiplePlaye本s()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("M使ltiplePlaye本s");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Pe本fo本設置ance;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	// Si設置使late 設置使ltiple playe本s
	fo本 (int32 i = 3; i <= 8; ++i)
	{
		Si設置使latePlaye本Join(i, 軍St本in成::P本intf(TEXT("Playe本%d"), i));
	}

	bool bS使ccess = TestPlaye本s.的使設置() >= 6; // Sho使ld ha正e at least 6 playe本s now

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Expected at least 6 playe本s, 成ot %d"), TestPlaye本s.的使設置());
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下輸入ostMi成本ation()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("輸入ostMi成本ation");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::St本essTest;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	// Mock host 設置i成本ation
	bool bS使ccess = t本使e;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("輸入ost 設置i成本ation failed");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下的etwo本kE本本o本輸入andlin成()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("的etwo本kE本本o本輸入andlin成");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Inte成本ation;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	// Mock e本本o本 handlin成
	bool bS使ccess = t本使e;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("的etwo本k e本本o本 handlin成 failed");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下LatencySi設置使lation()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("LatencySi設置使lation");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::Pe本fo本設置ance;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	float Latency = Meas使本eLatency();
	bool bS使ccess = Latency >= 0.0f;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Latency 設置eas使本e設置ent failed");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

軍的etwo本kTestRes使lt UMin成RTS的etwo本kTest::Test下PacketLossReco正e本y()
{
	軍的etwo本kTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("PacketLossReco正e本y");
	Res使lt.Cate成o本y = E的etwo本kTestCate成o本y::St本essTest;

	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

	// Mock packet loss 本eco正e本y
	bool bS使ccess = t本使e;

	Res使lt.bPassed = bS使ccess;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Packet loss 本eco正e本y failed");
	}

	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	Lo成TestRes使lt(Res使lt);

	本et使本n Res使lt;
}

正oid UMin成RTS的etwo本kTest::Clean使pTest()
{
	Test的etwo本kMana成e本 = n使llpt本;
	TestRelationshipSyste設置 = n使llpt本;
	TestPlaye本s.E設置pty();
}

正oid UMin成RTS的etwo本kTest::Lo成TestRes使lt(const 軍的etwo本kTestRes使lt& Res使lt)
{
	if (Res使lt.bPassed)
	{
		UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Test Passed: %s (%.2f 設置s)"),
			*Res使lt.Test的a設置e, Res使lt.Exec使tionTi設置eMs);
	}
	else
	{
		UE下LOG(Lo成Te設置p, E本本o本, TEXT("✗ Test 軍ailed: %s - %s (%.2f 設置s)"),
			*Res使lt.Test的a設置e, *Res使lt.E本本o本Messa成e, Res使lt.Exec使tionTi設置eMs);
	}
}

正oid UMin成RTS的etwo本kTest::Si設置使latePlaye本Join(int32 Playe本Id, const 軍St本in成& Playe本的a設置e)
{
	軍的etwo本kPlaye本Info Playe本Info;
	Playe本Info.Playe本Id = Playe本Id;
	Playe本Info.Playe本的a設置e = Playe本的a設置e;
	Playe本Info.bIs輸入ost = (Playe本Id == 1);
	Playe本Info.ConnectionState = E的etwo本kConnectionState::Connected;
	TestPlaye本s.Add(Playe本Info);
}

正oid UMin成RTS的etwo本kTest::Si設置使latePlaye本Lea正e(int32 Playe本Id)
{
	fo本 (int32 i = TestPlaye本s.的使設置() - 1; i >= 0; --i)
	{
		if (TestPlaye本s[i].Playe本Id == Playe本Id)
		{
			TestPlaye本s.Re設置o正eAt(i);
			b本eak;
		}
	}
}

float UMin成RTS的etwo本kTest::Meas使本eLatency()
{
	// Si設置ple latency 設置eas使本e設置ent
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	// Si設置使late so設置e wo本k
	fo本 (int32 i = 0; i < 1000; ++i)
	{
		正olatile int32 D使設置設置y = i * i;
		(正oid)D使設置設置y; // S使pp本ess 使n使sed wa本nin成
	}
	do使ble EndTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	本et使本n (EndTi設置e - Sta本tTi設置e) * 1000.0f;
}
