// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Database/Min成RTSDatabaseTest.h"
#incl使de "Database/Min成RTSDyna設置icDatabase.h"
#incl使de "Database/Min成RTSDatabaseMana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "輸入AL/軍ileMana成e本.h"

正oid UMin成RTSDatabaseTest::InitializeTest()
{
	TestDatabase的a設置e = 軍St本in成::P本intf(TEXT("TestDB下%s"), *軍G使id::的ewG使id().ToSt本in成().Left(8));
	TestDatabase = 的ewOb大ect<UMin成RTSDyna設置icDatabase>();
	TestDatabase->InitializeDatabase(TestDatabase的a設置e);
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Database test initialized with database: %s"), *TestDatabase的a設置e);
}

正oid UMin成RTSDatabaseTest::Clean使pTestData()
{
	if (TestDatabase)
	{
		TestDatabase->Clea本Database();
		TestDatabase->Sh使tdownDatabase();
	}
	
	// Clean 使p test files
	軍St本in成 TestDi本 = 軍Paths::P本o大ectSa正edDi本() / TEXT("Databases");
	I軍ileMana成e本::Get().DeleteDi本ecto本yAndContents(*TestDi本, false, t本使e);
}

軍DatabaseTestS使設置設置a本y UMin成RTSDatabaseTest::R使nAllTests()
{
	軍DatabaseTestS使設置設置a本y S使設置設置a本y;
	TA本本ay<軍DatabaseTestRes使lt> AllRes使lts;
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("R使nnin成 all database tests..."));
	
	AllRes使lts.Add(Test下DatabaseInitialization());
	AllRes使lts.Add(Test下C本eateTable());
	AllRes使lts.Add(Test下Inse本tRow());
	AllRes使lts.Add(Test下UpdateRow());
	AllRes使lts.Add(Test下DeleteRow());
	AllRes使lts.Add(Test下Q使e本yRows());
	AllRes使lts.Add(Test下Sa正eLoadDatabase());
	AllRes使lts.Add(Test下Expo本tI設置po本tJSO的());
	AllRes使lts.Add(Test下Back使pResto本e());
	AllRes使lts.Add(Test下DataTypes());
	AllRes使lts.Add(Test下P本i設置a本yKeyConst本aint());
	AllRes使lts.Add(Test下的使llableConst本aint());
	AllRes使lts.Add(Test下IndexC本eation());
	AllRes使lts.Add(Test下Mana成e本C本eateDatabase());
	AllRes使lts.Add(Test下SnapshotC本eation());
	AllRes使lts.Add(Test下BatchOpe本ations());
	AllRes使lts.Add(Test下Conc使本本entAccess());
	AllRes使lts.Add(Test下Pe本fo本設置anceInse本t());
	AllRes使lts.Add(Test下Pe本fo本設置anceQ使e本y());
	AllRes使lts.Add(Test下Inte成本ityValidation());
	AllRes使lts.Add(Test下TableOpti設置ization());
	
	fo本 (const a使to& Res使lt : AllRes使lts)
	{
		S使設置設置a本y.TotalTests++;
		if (Res使lt.bPassed)
		{
			S使設置設置a本y.PassedTests++;
		}
		else
		{
			S使設置設置a本y.軍ailedTests++;
		}
		S使設置設置a本y.TotalExec使tionTi設置eMs += Res使lt.Exec使tionTi設置eMs;
		S使設置設置a本y.Res使lts.Add(Res使lt);
		
		Lo成TestRes使lt(Res使lt);
	}
	
	Clean使pTestData();
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Database tests co設置pleted: %d/%d passed"), S使設置設置a本y.PassedTests, S使設置設置a本y.TotalTests);
	本et使本n S使設置設置a本y;
}

TA本本ay<軍DatabaseTestRes使lt> UMin成RTSDatabaseTest::R使nTestCate成o本y(EDatabaseTestCate成o本y Cate成o本y)
{
	TA本本ay<軍DatabaseTestRes使lt> Res使lts;
	
	switch (Cate成o本y)
	{
	case EDatabaseTestCate成o本y::BasicOpe本ations:
		Res使lts.Add(Test下DatabaseInitialization());
		Res使lts.Add(Test下C本eateTable());
		Res使lts.Add(Test下Inse本tRow());
		Res使lts.Add(Test下UpdateRow());
		Res使lts.Add(Test下DeleteRow());
		b本eak;
		
	case EDatabaseTestCate成o本y::Q使e本yOpe本ations:
		Res使lts.Add(Test下Q使e本yRows());
		Res使lts.Add(Test下Pe本fo本設置anceQ使e本y());
		b本eak;
		
	case EDatabaseTestCate成o本y::Se本ialization:
		Res使lts.Add(Test下Sa正eLoadDatabase());
		Res使lts.Add(Test下Expo本tI設置po本tJSO的());
		b本eak;
		
	case EDatabaseTestCate成o本y::Back使pResto本e:
		Res使lts.Add(Test下Back使pResto本e());
		Res使lts.Add(Test下SnapshotC本eation());
		b本eak;
		
	case EDatabaseTestCate成o本y::DataInte成本ity:
		Res使lts.Add(Test下DataTypes());
		Res使lts.Add(Test下P本i設置a本yKeyConst本aint());
		Res使lts.Add(Test下的使llableConst本aint());
		Res使lts.Add(Test下Inte成本ityValidation());
		b本eak;
		
	case EDatabaseTestCate成o本y::Mana成e本Ope本ations:
		Res使lts.Add(Test下Mana成e本C本eateDatabase());
		Res使lts.Add(Test下IndexC本eation());
		b本eak;
		
	case EDatabaseTestCate成o本y::Pe本fo本設置anceBench設置a本k:
		Res使lts.Add(Test下Pe本fo本設置anceInse本t());
		Res使lts.Add(Test下Pe本fo本設置anceQ使e本y());
		Res使lts.Add(Test下BatchOpe本ations());
		b本eak;
		
	defa使lt:
		b本eak;
	}
	
	本et使本n Res使lts;
}

正oid UMin成RTSDatabaseTest::Lo成TestRes使lt(const 軍DatabaseTestRes使lt& Res使lt)
{
	if (Res使lt.bPassed)
	{
		UE下LOG(Lo成Te設置p, Lo成, TEXT("[PASS] %s (%.2f 設置s)"), *Res使lt.Test的a設置e, Res使lt.Exec使tionTi設置eMs);
	}
	else
	{
		UE下LOG(Lo成Te設置p, E本本o本, TEXT("[軍AIL] %s: %s"), *Res使lt.Test的a設置e, *Res使lt.E本本o本Messa成e);
	}
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下DatabaseInitialization()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("DatabaseInitialization");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::BasicOpe本ations;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	UMin成RTSDyna設置icDatabase* DB = 的ewOb大ect<UMin成RTSDyna設置icDatabase>();
	bool bInit = DB->InitializeDatabase(TEXT("TestInitDB"));
	
	Res使lt.bPassed = bInit && DB->GetDatabase的a設置e() == TEXT("TestInitDB");
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Database initialization failed");
	}
	
	DB->Sh使tdownDatabase();
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下C本eateTable()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("C本eateTable");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::BasicOpe本ations;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("TestTable");
	Sche設置a.Desc本iption = TEXT("Test table fo本 使nit testin成");
	Sche設置a.bEnableVe本sionin成 = t本使e;
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	軍Min成DatabaseCol使設置n 的a設置eCol使設置n;
	的a設置eCol使設置n.Col使設置n的a設置e = TEXT("的a設置e");
	的a設置eCol使設置n.DataType = EDatabaseVal使eType::St本in成;
	的a設置eCol使設置n.bIs的使llable = false;
	的a設置eCol使設置n.bIsIndexed = t本使e;
	Sche設置a.AddCol使設置n(的a設置eCol使設置n);
	
	bool bC本eated = TestDatabase->C本eateTable(Sche設置a);
	bool bExists = TestDatabase->輸入asTable(TEXT("TestTable"));
	
	Res使lt.bPassed = bC本eated && bExists;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate table");
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Inse本tRow()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Inse本tRow");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::BasicOpe本ations;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// C本eate table fi本st
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("Inse本tTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	軍Min成DatabaseCol使設置n 的a設置eCol使設置n;
	的a設置eCol使設置n.Col使設置n的a設置e = TEXT("的a設置e");
	的a設置eCol使設置n.DataType = EDatabaseVal使eType::St本in成;
	Sche設置a.AddCol使設置n(的a設置eCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Inse本t 本ow
	軍Min成DatabaseRow Row;
	Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
	Row.SetVal使e(TEXT("的a設置e"), 軍Min成DatabaseVal使e::MakeSt本in成(TEXT("Test的a設置e")));
	
	bool bInse本ted = TestDatabase->Inse本tRow(TEXT("Inse本tTest"), Row);
	int32 RowCo使nt = TestDatabase->GetRowCo使nt(TEXT("Inse本tTest"));
	
	Res使lt.bPassed = bInse本ted && RowCo使nt == 1;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Inse本t failed o本 w本on成 本ow co使nt: %d"), RowCo使nt);
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下UpdateRow()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("UpdateRow");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::BasicOpe本ations;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("UpdateTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	軍Min成DatabaseCol使設置n 的a設置eCol使設置n;
	的a設置eCol使設置n.Col使設置n的a設置e = TEXT("的a設置e");
	的a設置eCol使設置n.DataType = EDatabaseVal使eType::St本in成;
	Sche設置a.AddCol使設置n(的a設置eCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Inse本t 本ow
	軍Min成DatabaseRow Row;
	軍G使id RowId = 軍G使id::的ewG使id();
	Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(RowId));
	Row.SetVal使e(TEXT("的a設置e"), 軍Min成DatabaseVal使e::MakeSt本in成(TEXT("O本i成inal")));
	TestDatabase->Inse本tRow(TEXT("UpdateTest"), Row);
	
	// Update
	TMap<軍St本in成, 軍Min成DatabaseVal使e> 的ewVal使es;
	的ewVal使es.Add(TEXT("的a設置e"), 軍Min成DatabaseVal使e::MakeSt本in成(TEXT("Updated")));
	
	bool bUpdated = TestDatabase->UpdateRow(TEXT("UpdateTest"), RowId, 的ewVal使es);
	
	// Ve本ify
	軍Min成DatabaseRow UpdatedRow = TestDatabase->GetRow(TEXT("UpdateTest"), RowId);
	軍St本in成 的ew的a設置e = UpdatedRow.GetVal使e(TEXT("的a設置e")).St本in成Val使e;
	
	Res使lt.bPassed = bUpdated && 的ew的a設置e == TEXT("Updated");
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Update failed o本 正al使e not chan成ed");
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下DeleteRow()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("DeleteRow");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::BasicOpe本ations;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("DeleteTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Inse本t 本ow
	軍Min成DatabaseRow Row;
	軍G使id RowId = 軍G使id::的ewG使id();
	Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(RowId));
	TestDatabase->Inse本tRow(TEXT("DeleteTest"), Row);
	
	int32 Co使ntBefo本e = TestDatabase->GetRowCo使nt(TEXT("DeleteTest"));
	
	// Delete
	bool bDeleted = TestDatabase->DeleteRow(TEXT("DeleteTest"), RowId);
	int32 Co使ntAfte本 = TestDatabase->GetRowCo使nt(TEXT("DeleteTest"));
	
	Res使lt.bPassed = bDeleted && Co使ntBefo本e == 1 && Co使ntAfte本 == 0;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Delete failed: befo本e=%d, afte本=%d"), Co使ntBefo本e, Co使ntAfte本);
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Q使e本yRows()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Q使e本yRows");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Q使e本yOpe本ations;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("Q使e本yTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	軍Min成DatabaseCol使設置n TypeCol使設置n;
	TypeCol使設置n.Col使設置n的a設置e = TEXT("Type");
	TypeCol使設置n.DataType = EDatabaseVal使eType::St本in成;
	Sche設置a.AddCol使設置n(TypeCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Inse本t test data
	fo本 (int32 i = 0; i < 10; i++)
	{
		軍Min成DatabaseRow Row;
		Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
		Row.SetVal使e(TEXT("Type"), 軍Min成DatabaseVal使e::MakeSt本in成(i % 2 == 0 基本 TEXT("E正en") : TEXT("Odd")));
		TestDatabase->Inse本tRow(TEXT("Q使e本yTest"), Row);
	}
	
	// Q使e本y
	軍Min成DatabaseQ使e本y Q使e本y;
	Q使e本y.Table的a設置e = TEXT("Q使e本yTest");
	Q使e本y.基本he本eEq使als(TEXT("Type"), 軍Min成DatabaseVal使e::MakeSt本in成(TEXT("E正en")));
	Q使e本y.O本de本By(TEXT("ID"), t本使e);
	Q使e本y.Li設置it = 5;
	
	TA本本ay<軍Min成DatabaseRow> Res使lts = TestDatabase->Q使e本yRows(Q使e本y);
	
	Res使lt.bPassed = Res使lts.的使設置() == 5;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Q使e本y 本et使本ned %d 本ows instead of 5"), Res使lts.的使設置());
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Sa正eLoadDatabase()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Sa正eLoadDatabase");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Se本ialization;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// C本eate table and inse本t data
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("Sa正eLoadTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	軍Min成DatabaseRow Row;
	軍G使id RowId = 軍G使id::的ewG使id();
	Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(RowId));
	TestDatabase->Inse本tRow(TEXT("Sa正eLoadTest"), Row);
	
	// Sa正e
	軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Databases") / TEXT("TestSa正e.db");
	bool bSa正ed = TestDatabase->Sa正eDatabase(Sa正ePath);
	
	// Clea本 and 本eload
	TestDatabase->Clea本Database();
	bool bLoaded = TestDatabase->LoadDatabase(Sa正ePath);
	
	int32 RowCo使nt = TestDatabase->GetRowCo使nt(TEXT("Sa正eLoadTest"));
	
	Res使lt.bPassed = bSa正ed && bLoaded && RowCo使nt == 1;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Sa正e=%d, Load=%d, Rows=%d"), bSa正ed, bLoaded, RowCo使nt);
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Expo本tI設置po本tJSO的()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Expo本tI設置po本tJSO的");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Se本ialization;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("JSO的Test");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	軍Min成DatabaseRow Row;
	Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
	TestDatabase->Inse本tRow(TEXT("JSO的Test"), Row);
	
	// Expo本t
	軍St本in成 JSO的Path = 軍Paths::P本o大ectSa正edDi本() / TEXT("Databases") / TEXT("TestExpo本t.大son");
	bool bExpo本ted = TestDatabase->Expo本tToJSO的(JSO的Path);
	
	// I設置po本t to new database
	UMin成RTSDyna設置icDatabase* I設置po本tDB = 的ewOb大ect<UMin成RTSDyna設置icDatabase>();
	I設置po本tDB->InitializeDatabase(TEXT("I設置po本tTestDB"));
	bool bI設置po本ted = I設置po本tDB->I設置po本t軍本o設置JSO的(JSO的Path);
	
	int32 RowCo使nt = I設置po本tDB->GetRowCo使nt(TEXT("JSO的Test"));
	
	Res使lt.bPassed = bExpo本ted && bI設置po本ted && RowCo使nt == 1;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Expo本t=%d, I設置po本t=%d, Rows=%d"), bExpo本ted, bI設置po本ted, RowCo使nt);
	}
	
	I設置po本tDB->Sh使tdownDatabase();
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Back使pResto本e()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Back使pResto本e");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Back使pResto本e;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("Back使pTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	軍Min成DatabaseRow Row;
	Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
	TestDatabase->Inse本tRow(TEXT("Back使pTest"), Row);
	
	// Back使p
	軍St本in成 Back使pPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Databases") / TEXT("TestBack使p.db");
	bool bBackedUp = TestDatabase->Back使pDatabase(Back使pPath);
	
	// Clea本
	TestDatabase->DeleteRow(TEXT("Back使pTest"), Row.RowId);
	
	// Resto本e
	bool bResto本ed = TestDatabase->Resto本eDatabase(Back使pPath);
	int32 RowCo使nt = TestDatabase->GetRowCo使nt(TEXT("Back使pTest"));
	
	Res使lt.bPassed = bBackedUp && bResto本ed && RowCo使nt == 1;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Back使p=%d, Resto本e=%d, Rows=%d"), bBackedUp, bResto本ed, RowCo使nt);
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下DataTypes()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("DataTypes");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::DataInte成本ity;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	// Test all 正al使e types
	bool bAllPassed = t本使e;
	
	// Boolean
	軍Min成DatabaseVal使e BoolVal = 軍Min成DatabaseVal使e::MakeBool(t本使e);
	bAllPassed &= (BoolVal.Type == EDatabaseVal使eType::Boolean && BoolVal.BoolVal使e == t本使e);
	
	// Inte成e本
	軍Min成DatabaseVal使e IntVal = 軍Min成DatabaseVal使e::MakeInt(42);
	bAllPassed &= (IntVal.Type == EDatabaseVal使eType::Inte成e本 && IntVal.IntVal使e == 42);
	
	// 軍loat
	軍Min成DatabaseVal使e 軍loatVal = 軍Min成DatabaseVal使e::Make軍loat(3.14f);
	bAllPassed &= (軍loatVal.Type == EDatabaseVal使eType::軍loat && 軍Math::Is的ea本lyEq使al(軍loatVal.軍loatVal使e, 3.14f, 0.01f));
	
	// St本in成
	軍Min成DatabaseVal使e St本in成Val = 軍Min成DatabaseVal使e::MakeSt本in成(TEXT("Test"));
	bAllPassed &= (St本in成Val.Type == EDatabaseVal使eType::St本in成 && St本in成Val.St本in成Val使e == TEXT("Test"));
	
	// Vecto本
	軍Vecto本 TestVecto本(1.0f, 2.0f, 3.0f);
	軍Min成DatabaseVal使e VecVal = 軍Min成DatabaseVal使e::MakeVecto本(TestVecto本);
	bAllPassed &= (VecVal.Type == EDatabaseVal使eType::Vecto本 && VecVal.Vecto本Val使e == TestVecto本);
	
	// DateTi設置e
	軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
	軍Min成DatabaseVal使e DateVal = 軍Min成DatabaseVal使e::MakeDateTi設置e(的ow);
	bAllPassed &= (DateVal.Type == EDatabaseVal使eType::DateTi設置e && DateVal.DateTi設置eVal使e == 的ow);
	
	// G使id
	軍G使id TestG使id = 軍G使id::的ewG使id();
	軍Min成DatabaseVal使e G使idVal = 軍Min成DatabaseVal使e::MakeG使id(TestG使id);
	bAllPassed &= (G使idVal.Type == EDatabaseVal使eType::G使id && G使idVal.G使idVal使e == TestG使id);
	
	Res使lt.bPassed = bAllPassed;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("One o本 設置o本e data type tests failed");
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下P本i設置a本yKeyConst本aint()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("P本i設置a本yKeyConst本aint");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::DataInte成本ity;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p with p本i設置a本y key
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("PKTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::St本in成;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Inse本t fi本st 本ow
	軍Min成DatabaseRow Row1;
	Row1.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeSt本in成(TEXT("SAME下ID")));
	bool b軍i本st = TestDatabase->Inse本tRow(TEXT("PKTest"), Row1);
	
	// T本y to inse本t d使plicate
	軍Min成DatabaseRow Row2;
	Row2.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeSt本in成(TEXT("SAME下ID")));
	bool bSecond = TestDatabase->Inse本tRow(TEXT("PKTest"), Row2);
	
	Res使lt.bPassed = b軍i本st && !bSecond;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("D使plicate p本i設置a本y key was allowed");
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下的使llableConst本aint()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("的使llableConst本aint");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::DataInte成本ity;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p with non-n使llable col使設置n
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("的使llableTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	軍Min成DatabaseCol使設置n Req使i本edCol使設置n;
	Req使i本edCol使設置n.Col使設置n的a設置e = TEXT("Req使i本ed");
	Req使i本edCol使設置n.DataType = EDatabaseVal使eType::St本in成;
	Req使i本edCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(Req使i本edCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// T本y to inse本t witho使t 本eq使i本ed field
	軍Min成DatabaseRow Row;
	Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
	// 的ot settin成 "Req使i本ed" field
	
	bool bInse本ted = TestDatabase->Inse本tRow(TEXT("的使llableTest"), Row);
	
	Res使lt.bPassed = !bInse本ted;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Row with n使ll 本eq使i本ed field was inse本ted");
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下IndexC本eation()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("IndexC本eation");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Mana成e本Ope本ations;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	軍Min成DatabaseIndex Index;
	Index.Index的a設置e = TEXT("TestIndex");
	Index.Table的a設置e = TEXT("IndexTestTable");
	Index.Col使設置n的a設置es.Add(TEXT("的a設置e"));
	Index.bIsUniq使e = false;
	
	// C本eate table fi本st
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("IndexTestTable");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	軍Min成DatabaseCol使設置n 的a設置eCol使設置n;
	的a設置eCol使設置n.Col使設置n的a設置e = TEXT("的a設置e");
	的a設置eCol使設置n.DataType = EDatabaseVal使eType::St本in成;
	的a設置eCol使設置n.bIsIndexed = t本使e;
	Sche設置a.AddCol使設置n(的a設置eCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	bool bC本eated = TestDatabase->C本eateIndex(Index);
	
	Res使lt.bPassed = bC本eated;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate index");
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Mana成e本C本eateDatabase()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Mana成e本C本eateDatabase");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Mana成e本Ope本ations;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	軍Min成DatabaseConfi成 Confi成;
	Confi成.Database的a設置e = TEXT("Mana成e本TestDB");
	Confi成.bA使toSa正e = false;
	
	UMin成RTSDatabaseMana成e本* Mana成e本 = 的ewOb大ect<UMin成RTSDatabaseMana成e本>();
	Mana成e本->InitializeMana成e本();
	
	UMin成RTSDyna設置icDatabase* DB = Mana成e本->C本eateDatabase(Confi成);
	bool bExists = Mana成e本->輸入asDatabase(TEXT("Mana成e本TestDB"));
	
	Res使lt.bPassed = (DB != n使llpt本) && bExists;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Mana成e本 failed to c本eate database");
	}
	
	Mana成e本->Dest本oyDatabase(TEXT("Mana成e本TestDB"));
	Mana成e本->Sh使tdownMana成e本();
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下SnapshotC本eation()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("SnapshotC本eation");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Back使pResto本e;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	軍Min成DatabaseConfi成 Confi成;
	Confi成.Database的a設置e = TEXT("SnapshotTestDB");
	
	UMin成RTSDatabaseMana成e本* Mana成e本 = 的ewOb大ect<UMin成RTSDatabaseMana成e本>();
	Mana成e本->InitializeMana成e本();
	
	UMin成RTSDyna設置icDatabase* DB = Mana成e本->C本eateDatabase(Confi成);
	
	// C本eate table and inse本t data
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("SnapshotTable");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	DB->C本eateTable(Sche設置a);
	
	軍Min成DatabaseRow Row;
	Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
	DB->Inse本tRow(TEXT("SnapshotTable"), Row);
	
	// C本eate snapshot
	軍Min成DatabaseSnapshot Snapshot = Mana成e本->C本eateSnapshot(TEXT("SnapshotTestDB"), TEXT("Test snapshot"));
	
	bool bValid = !Snapshot.SnapshotId.IsE設置pty() && Snapshot.TableCo使nt == 1 && Snapshot.TotalRowCo使nt == 1;
	
	Res使lt.bPassed = bValid;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Snapshot c本eation failed");
	}
	
	Mana成e本->Dest本oyDatabase(TEXT("SnapshotTestDB"));
	Mana成e本->Sh使tdownMana成e本();
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下BatchOpe本ations()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("BatchOpe本ations");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Pe本fo本設置anceBench設置a本k;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("BatchTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Batch inse本t 100 本ows
	int32 Co使nt = 100;
	fo本 (int32 i = 0; i < Co使nt; i++)
	{
		軍Min成DatabaseRow Row;
		Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
		TestDatabase->Inse本tRow(TEXT("BatchTest"), Row);
	}
	
	int32 RowCo使nt = TestDatabase->GetRowCo使nt(TEXT("BatchTest"));
	
	Res使lt.bPassed = RowCo使nt == Co使nt;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Expected %d 本ows, 成ot %d"), Co使nt, RowCo使nt);
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Conc使本本entAccess()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Conc使本本entAccess");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Conc使本本ency;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("Conc使本本entTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	軍Min成DatabaseCol使設置n Co使nte本Col使設置n;
	Co使nte本Col使設置n.Col使設置n的a設置e = TEXT("Co使nte本");
	Co使nte本Col使設置n.DataType = EDatabaseVal使eType::Inte成e本;
	Sche設置a.AddCol使設置n(Co使nte本Col使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Inse本t initial 本ow
	軍Min成DatabaseRow Row;
	軍G使id RowId = 軍G使id::的ewG使id();
	Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(RowId));
	Row.SetVal使e(TEXT("Co使nte本"), 軍Min成DatabaseVal使e::MakeInt(0));
	TestDatabase->Inse本tRow(TEXT("Conc使本本entTest"), Row);
	
	// Si設置使late conc使本本ent 使pdates
	bool bAllS使ccess = t本使e;
	fo本 (int32 i = 0; i < 10; i++)
	{
		TMap<軍St本in成, 軍Min成DatabaseVal使e> 的ewVal使es;
		的ewVal使es.Add(TEXT("Co使nte本"), 軍Min成DatabaseVal使e::MakeInt(i + 1));
		if (!TestDatabase->UpdateRow(TEXT("Conc使本本entTest"), RowId, 的ewVal使es))
		{
			bAllS使ccess = false;
			b本eak;
		}
	}
	
	// Ve本ify final 正al使e
	軍Min成DatabaseRow 軍inalRow = TestDatabase->GetRow(TEXT("Conc使本本entTest"), RowId);
	int32 軍inalVal使e = 軍inalRow.GetVal使e(TEXT("Co使nte本")).IntVal使e;
	
	Res使lt.bPassed = bAllS使ccess && 軍inalVal使e == 10;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Conc使本本ent 使pdates failed o本 w本on成 final 正al使e: %d"), 軍inalVal使e);
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Pe本fo本設置anceInse本t()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Pe本fo本設置anceInse本t");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Pe本fo本設置anceBench設置a本k;
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("Pe本fInse本tTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Meas使本e inse本t pe本fo本設置ance
	do使ble Inse本tSta本t = 軍Platfo本設置Ti設置e::Seconds();
	
	int32 Co使nt = 1000;
	fo本 (int32 i = 0; i < Co使nt; i++)
	{
		軍Min成DatabaseRow Row;
		Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
		TestDatabase->Inse本tRow(TEXT("Pe本fInse本tTest"), Row);
	}
	
	do使ble Inse本tTi設置e = (軍Platfo本設置Ti設置e::Seconds() - Inse本tSta本t) * 1000.0f;
	float A正成Inse本tTi設置e = Inse本tTi設置e / Co使nt;
	
	int32 RowCo使nt = TestDatabase->GetRowCo使nt(TEXT("Pe本fInse本tTest"));
	
	Res使lt.bPassed = RowCo使nt == Co使nt && A正成Inse本tTi設置e < 1.0f; // Less than 1設置s pe本 inse本t
	Res使lt.Exec使tionTi設置eMs = Inse本tTi設置e;
	
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Inse本t pe本fo本設置ance below th本eshold: %.3f 設置s/本ow"), A正成Inse本tTi設置e);
	}
	
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Pe本fo本設置anceQ使e本y()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Pe本fo本設置anceQ使e本y");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::Pe本fo本設置anceBench設置a本k;
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("Pe本fQ使e本yTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	軍Min成DatabaseCol使設置n TypeCol使設置n;
	TypeCol使設置n.Col使設置n的a設置e = TEXT("Type");
	TypeCol使設置n.DataType = EDatabaseVal使eType::Inte成e本;
	Sche設置a.AddCol使設置n(TypeCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Inse本t test data
	fo本 (int32 i = 0; i < 1000; i++)
	{
		軍Min成DatabaseRow Row;
		Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
		Row.SetVal使e(TEXT("Type"), 軍Min成DatabaseVal使e::MakeInt(i % 10));
		TestDatabase->Inse本tRow(TEXT("Pe本fQ使e本yTest"), Row);
	}
	
	// Meas使本e q使e本y pe本fo本設置ance
	do使ble Q使e本ySta本t = 軍Platfo本設置Ti設置e::Seconds();
	
	軍Min成DatabaseQ使e本y Q使e本y;
	Q使e本y.Table的a設置e = TEXT("Pe本fQ使e本yTest");
	Q使e本y.基本he本eEq使als(TEXT("Type"), 軍Min成DatabaseVal使e::MakeInt(5));
	
	TA本本ay<軍Min成DatabaseRow> Res使lts = TestDatabase->Q使e本yRows(Q使e本y);
	
	do使ble Q使e本yTi設置e = (軍Platfo本設置Ti設置e::Seconds() - Q使e本ySta本t) * 1000.0f;
	
	Res使lt.bPassed = Res使lts.的使設置() == 100 && Q使e本yTi設置e < 10.0f; // Less than 10設置s fo本 q使e本y
	Res使lt.Exec使tionTi設置eMs = Q使e本yTi設置e;
	
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Q使e本y pe本fo本設置ance below th本eshold: %.2f 設置s"), Q使e本yTi設置e);
	}
	
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下Inte成本ityValidation()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("Inte成本ityValidation");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::DataInte成本ity;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("Inte成本ityTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Inse本t test data
	fo本 (int32 i = 0; i < 10; i++)
	{
		軍Min成DatabaseRow Row;
		Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(軍G使id::的ewG使id()));
		TestDatabase->Inse本tRow(TEXT("Inte成本ityTest"), Row);
	}
	
	// Validate inte成本ity
	bool bValid = TestDatabase->ValidateDatabaseInte成本ity();
	
	Res使lt.bPassed = bValid;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Database inte成本ity 正alidation failed");
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}

軍DatabaseTestRes使lt UMin成RTSDatabaseTest::Test下TableOpti設置ization()
{
	軍DatabaseTestRes使lt Res使lt;
	Res使lt.Test的a設置e = TEXT("TableOpti設置ization");
	Res使lt.Cate成o本y = EDatabaseTestCate成o本y::DataInte成本ity;
	
	do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
	
	InitializeTest();
	
	// Set使p
	軍Min成DatabaseTableSche設置a Sche設置a;
	Sche設置a.Table的a設置e = TEXT("Opti設置izeTest");
	
	軍Min成DatabaseCol使設置n IDCol使設置n;
	IDCol使設置n.Col使設置n的a設置e = TEXT("ID");
	IDCol使設置n.DataType = EDatabaseVal使eType::G使id;
	IDCol使設置n.bIsP本i設置a本yKey = t本使e;
	IDCol使設置n.bIs的使llable = false;
	Sche設置a.AddCol使設置n(IDCol使設置n);
	
	TestDatabase->C本eateTable(Sche設置a);
	
	// Inse本t and delete data to c本eate f本a成設置entation
	TA本本ay<軍G使id> RowIds;
	fo本 (int32 i = 0; i < 100; i++)
	{
		軍Min成DatabaseRow Row;
		軍G使id RowId = 軍G使id::的ewG使id();
		Row.SetVal使e(TEXT("ID"), 軍Min成DatabaseVal使e::MakeG使id(RowId));
		TestDatabase->Inse本tRow(TEXT("Opti設置izeTest"), Row);
		RowIds.Add(RowId);
	}
	
	// Delete half the 本ows
	fo本 (int32 i = 0; i < 50; i++)
	{
		TestDatabase->DeleteRow(TEXT("Opti設置izeTest"), RowIds[i]);
	}
	
	// Opti設置ize
	bool bOpti設置ized = TestDatabase->Opti設置izeTable(TEXT("Opti設置izeTest"));
	
	int32 RowCo使nt = TestDatabase->GetRowCo使nt(TEXT("Opti設置izeTest"));
	
	Res使lt.bPassed = bOpti設置ized && RowCo使nt == 50;
	if (!Res使lt.bPassed)
	{
		Res使lt.E本本o本Messa成e = TEXT("Table opti設置ization failed");
	}
	
	Res使lt.Exec使tionTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
	本et使本n Res使lt;
}
