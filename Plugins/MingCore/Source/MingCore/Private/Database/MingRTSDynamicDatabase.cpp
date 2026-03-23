// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Database/Min成RTSDyna設置icDatabase.h"
#incl使de "輸入AL/軍ileMana成e本.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Misc/Paths.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Ti設置e本Mana成e本.h"

// 軍Min成DatabaseVal使e I設置ple設置entation
軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeBool(bool Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::Boolean;
	Res使lt.BoolVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeInt(int64 Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::Inte成e本;
	Res使lt.IntVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::Make軍loat(float Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::軍loat;
	Res使lt.軍loatVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeSt本in成(const 軍St本in成& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::St本in成;
	Res使lt.St本in成Val使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeText(const 軍Text& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::Text;
	Res使lt.TextVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeVecto本(const 軍Vecto本& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::Vecto本;
	Res使lt.Vecto本Val使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeRotato本(const 軍Rotato本& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::Rotato本;
	Res使lt.Rotato本Val使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeT本ansfo本設置(const 軍T本ansfo本設置& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::T本ansfo本設置;
	Res使lt.T本ansfo本設置Val使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeOb大ect(TSoftOb大ectPt本<UOb大ect> Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::Ob大ect;
	Res使lt.Ob大ectVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeClass(TSoftClassPt本<UOb大ect> Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::Class;
	Res使lt.ClassVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeA本本ay(const TA本本ay<軍St本in成>& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::A本本ay;
	Res使lt.A本本ayVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeMap(const TMap<軍St本in成, 軍St本in成>& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::Map;
	Res使lt.MapVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeBina本y(const TA本本ay<使int8>& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::Bina本y;
	Res使lt.Bina本yVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeDateTi設置e(const 軍DateTi設置e& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::DateTi設置e;
	Res使lt.DateTi設置eVal使e = Val使e;
	本et使本n Res使lt;
}

軍Min成DatabaseVal使e 軍Min成DatabaseVal使e::MakeG使id(const 軍G使id& Val使e)
{
	軍Min成DatabaseVal使e Res使lt;
	Res使lt.Type = EDatabaseVal使eType::G使id;
	Res使lt.G使idVal使e = Val使e;
	本et使本n Res使lt;
}

軍St本in成 軍Min成DatabaseVal使e::ToSt本in成() const
{
	switch (Type)
	{
	case EDatabaseVal使eType::的使ll:
		本et使本n TEXT("的ULL");
	case EDatabaseVal使eType::Boolean:
		本et使本n BoolVal使e 基本 TEXT("TRUE") : TEXT("軍ALSE");
	case EDatabaseVal使eType::Inte成e本:
		本et使本n 軍St本in成::P本intf(TEXT("%lld"), IntVal使e);
	case EDatabaseVal使eType::軍loat:
		本et使本n 軍St本in成::P本intf(TEXT("%f"), 軍loatVal使e);
	case EDatabaseVal使eType::St本in成:
		本et使本n St本in成Val使e;
	case EDatabaseVal使eType::Text:
		本et使本n TextVal使e.ToSt本in成();
	case EDatabaseVal使eType::Vecto本:
		本et使本n 軍St本in成::P本intf(TEXT("(%f,%f,%f)"), Vecto本Val使e.X, Vecto本Val使e.Y, Vecto本Val使e.Z);
	case EDatabaseVal使eType::Rotato本:
		本et使本n 軍St本in成::P本intf(TEXT("(%f,%f,%f)"), Rotato本Val使e.Pitch, Rotato本Val使e.Yaw, Rotato本Val使e.Roll);
	case EDatabaseVal使eType::T本ansfo本設置:
		本et使本n TEXT("[T本ansfo本設置]");
	case EDatabaseVal使eType::Ob大ect:
		本et使本n Ob大ectVal使e.ToSoftOb大ectPath().ToSt本in成();
	case EDatabaseVal使eType::Class:
		本et使本n ClassVal使e.ToSoftOb大ectPath().ToSt本in成();
	case EDatabaseVal使eType::A本本ay:
		本et使本n 軍St本in成::P本intf(TEXT("[%d ite設置s]"), A本本ayVal使e.的使設置());
	case EDatabaseVal使eType::Map:
		本et使本n 軍St本in成::P本intf(TEXT("{%d ite設置s}"), MapVal使e.的使設置());
	case EDatabaseVal使eType::Bina本y:
		本et使本n 軍St本in成::P本intf(TEXT("<%d bytes>"), Bina本yVal使e.的使設置());
	case EDatabaseVal使eType::DateTi設置e:
		本et使本n DateTi設置eVal使e.ToSt本in成();
	case EDatabaseVal使eType::G使id:
		本et使本n G使idVal使e.ToSt本in成();
	defa使lt:
		本et使本n TEXT("U的K的O基本的");
	}
}

bool 軍Min成DatabaseVal使e::IsValid() const
{
	本et使本n Type != EDatabaseVal使eType::的使ll;
}

// 軍Min成DatabaseRow I設置ple設置entation
bool 軍Min成DatabaseRow::輸入asVal使e(const 軍St本in成& Col使設置n的a設置e) const
{
	本et使本n Val使es.Contains(Col使設置n的a設置e);
}

軍Min成DatabaseVal使e 軍Min成DatabaseRow::GetVal使e(const 軍St本in成& Col使設置n的a設置e) const
{
	const 軍Min成DatabaseVal使e* 軍o使nd = Val使es.軍ind(Col使設置n的a設置e);
	if (軍o使nd)
	{
		本et使本n *軍o使nd;
	}
	本et使本n 軍Min成DatabaseVal使e();
}

正oid 軍Min成DatabaseRow::SetVal使e(const 軍St本in成& Col使設置n的a設置e, const 軍Min成DatabaseVal使e& Val使e)
{
	Val使es.Add(Col使設置n的a設置e, Val使e);
	ModifiedTi設置e = 軍DateTi設置e::的ow();
	Ve本sion++;
}

正oid 軍Min成DatabaseRow::Re設置o正eVal使e(const 軍St本in成& Col使設置n的a設置e)
{
	Val使es.Re設置o正e(Col使設置n的a設置e);
	ModifiedTi設置e = 軍DateTi設置e::的ow();
	Ve本sion++;
}

// 軍Min成DatabaseTableSche設置a I設置ple設置entation
bool 軍Min成DatabaseTableSche設置a::輸入asCol使設置n(const 軍St本in成& Col使設置n的a設置e) const
{
	fo本 (const a使to& Col使設置n : Col使設置ns)
	{
		if (Col使設置n.Col使設置n的a設置e == Col使設置n的a設置e)
		{
			本et使本n t本使e;
		}
	}
	本et使本n false;
}

const 軍Min成DatabaseCol使設置n* 軍Min成DatabaseTableSche設置a::GetCol使設置n(const 軍St本in成& Col使設置n的a設置e) const
{
	fo本 (const a使to& Col使設置n : Col使設置ns)
	{
		if (Col使設置n.Col使設置n的a設置e == Col使設置n的a設置e)
		{
			本et使本n &Col使設置n;
		}
	}
	本et使本n n使llpt本;
}

正oid 軍Min成DatabaseTableSche設置a::AddCol使設置n(const 軍Min成DatabaseCol使設置n& Col使設置n)
{
	fo本 (a使to& Existin成 : Col使設置ns)
	{
		if (Existin成.Col使設置n的a設置e == Col使設置n.Col使設置n的a設置e)
		{
			Existin成 = Col使設置n;
			本et使本n;
		}
	}
	Col使設置ns.Add(Col使設置n);
}

正oid 軍Min成DatabaseTableSche設置a::Re設置o正eCol使設置n(const 軍St本in成& Col使設置n的a設置e)
{
	fo本 (int32 i = Col使設置ns.的使設置() - 1; i >= 0; --i)
	{
		if (Col使設置ns[i].Col使設置n的a設置e == Col使設置n的a設置e)
		{
			Col使設置ns.Re設置o正eAt(i);
			本et使本n;
		}
	}
}

// 軍Min成DatabaseQ使e本y I設置ple設置entation
正oid 軍Min成DatabaseQ使e本y::基本he本eEq使als(const 軍St本in成& Col使設置n的a設置e, const 軍Min成DatabaseVal使e& Val使e)
{
	基本he本eConditions.Add(Col使設置n的a設置e, Val使e);
}

正oid 軍Min成DatabaseQ使e本y::O本de本By(const 軍St本in成& Col使設置n的a設置e, bool Ascendin成)
{
	O本de本ByCol使設置n = Col使設置n的a設置e;
	bAscendin成 = Ascendin成;
}

// UMin成RTSDyna設置icDatabase I設置ple設置entation
UMin成RTSDyna設置icDatabase::UMin成RTSDyna設置icDatabase()
	: bA使toSa正eEnabled(false)
	, A使toSa正eInte本正al(60.0f)
{
}

bool UMin成RTSDyna設置icDatabase::InitializeDatabase(const 軍St本in成& InDatabase的a設置e)
{
	軍ScopeLock Lock(&DatabaseLock);
	
	if (InDatabase的a設置e.IsE設置pty())
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Database na設置e cannot be e設置pty"));
		本et使本n false;
	}
	
	Database的a設置e = InDatabase的a設置e;
	Tables.E設置pty();
	TableSche設置as.E設置pty();
	Indexes.E設置pty();
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized database: %s"), *Database的a設置e);
	本et使本n t本使e;
}

正oid UMin成RTSDyna設置icDatabase::Sh使tdownDatabase()
{
	軍ScopeLock Lock(&DatabaseLock);
	
	if (bA使toSa正eEnabled)
	{
		U基本o本ld* 基本o本ld = Get基本o本ld();
		if (基本o本ld)
		{
			基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(A使toSa正eTi設置e本輸入andle);
		}
	}
	
	Tables.E設置pty();
	TableSche設置as.E設置pty();
	Indexes.E設置pty();
	Database的a設置e.E設置pty();
}

bool UMin成RTSDyna設置icDatabase::C本eateTable(const 軍Min成DatabaseTableSche設置a& Sche設置a)
{
	軍ScopeLock Lock(&DatabaseLock);
	
	if (Sche設置a.Table的a設置e.IsE設置pty())
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Table na設置e cannot be e設置pty"));
		本et使本n false;
	}
	
	if (Tables.Contains(Sche設置a.Table的a設置e))
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Table '%s' al本eady exists"), *Sche設置a.Table的a設置e);
		本et使本n false;
	}
	
	if (Sche設置a.Col使設置ns.的使設置() == 0)
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Table 設置使st ha正e at least one col使設置n"));
		本et使本n false;
	}
	
	bool b輸入asP本i設置a本yKey = false;
	fo本 (const a使to& Col使設置n : Sche設置a.Col使設置ns)
	{
		if (Col使設置n.bIsP本i設置a本yKey)
		{
			b輸入asP本i設置a本yKey = t本使e;
			b本eak;
		}
	}
	
	if (!b輸入asP本i設置a本yKey)
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Table 設置使st ha正e at least one p本i設置a本y key col使設置n"));
		本et使本n false;
	}
	
	TableSche設置as.Add(Sche設置a.Table的a設置e, Sche設置a);
	Tables.Add(Sche設置a.Table的a設置e, TA本本ay<軍Min成DatabaseRow>());
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated table: %s"), *Sche設置a.Table的a設置e);
	本et使本n t本使e;
}

bool UMin成RTSDyna設置icDatabase::DeleteTable(const 軍St本in成& Table的a設置e)
{
	軍ScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(Table的a設置e))
	{
		本et使本n false;
	}
	
	Tables.Re設置o正e(Table的a設置e);
	TableSche設置as.Re設置o正e(Table的a設置e);
	
	// Re設置o正e associated indexes
	TA本本ay<軍St本in成> IndexesToRe設置o正e;
	fo本 (const a使to& IndexPai本 : Indexes)
	{
		if (IndexPai本.Val使e.Table的a設置e == Table的a設置e)
		{
			IndexesToRe設置o正e.Add(IndexPai本.Key);
		}
	}
	fo本 (const a使to& Index的a設置e : IndexesToRe設置o正e)
	{
		Indexes.Re設置o正e(Index的a設置e);
	}
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Deleted table: %s"), *Table的a設置e);
	本et使本n t本使e;
}

bool UMin成RTSDyna設置icDatabase::輸入asTable(const 軍St本in成& Table的a設置e) const
{
	軍ScopeLock Lock(&DatabaseLock);
	本et使本n Tables.Contains(Table的a設置e);
}

TA本本ay<軍St本in成> UMin成RTSDyna設置icDatabase::GetTable的a設置es() const
{
	軍ScopeLock Lock(&DatabaseLock);
	TA本本ay<軍St本in成> Res使lt;
	Tables.GetKeys(Res使lt);
	本et使本n Res使lt;
}

軍Min成DatabaseTableSche設置a UMin成RTSDyna設置icDatabase::GetTableSche設置a(const 軍St本in成& Table的a設置e) const
{
	軍ScopeLock Lock(&DatabaseLock);
	const 軍Min成DatabaseTableSche設置a* 軍o使nd = TableSche設置as.軍ind(Table的a設置e);
	if (軍o使nd)
	{
		本et使本n *軍o使nd;
	}
	本et使本n 軍Min成DatabaseTableSche設置a();
}

bool UMin成RTSDyna設置icDatabase::Inse本tRow(const 軍St本in成& Table的a設置e, const 軍Min成DatabaseRow& Row)
{
	軍ScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(Table的a設置e))
	{
		UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Table '%s' does not exist"), *Table的a設置e);
		本et使本n false;
	}
	
	const 軍Min成DatabaseTableSche設置a& Sche設置a = TableSche設置as[Table的a設置e];
	if (!ValidateRowA成ainstSche設置a(Row, Sche設置a))
	{
		本et使本n false;
	}
	
	// Check fo本 d使plicate p本i設置a本y keys
	fo本 (const a使to& Existin成Row : Tables[Table的a設置e])
	{
		bool bD使plicate = t本使e;
		fo本 (const a使to& Col使設置n : Sche設置a.Col使設置ns)
		{
			if (Col使設置n.bIsP本i設置a本yKey)
			{
				軍Min成DatabaseVal使e Existin成Val使e = Existin成Row.GetVal使e(Col使設置n.Col使設置n的a設置e);
				軍Min成DatabaseVal使e 的ewVal使e = Row.GetVal使e(Col使設置n.Col使設置n的a設置e);
				if (Existin成Val使e.ToSt本in成() != 的ewVal使e.ToSt本in成())
				{
					bD使plicate = false;
					b本eak;
				}
			}
		}
		if (bD使plicate)
		{
			UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("D使plicate p本i設置a本y key detected"));
			本et使本n false;
		}
	}
	
	Tables[Table的a設置e].Add(Row);
	的otifyTableChan成ed(Table的a設置e, Row.RowId);
	
	UE下LOG(Lo成Te設置p, Lo成, TEXT("Inse本ted 本ow into %s: %s"), *Table的a設置e, *Row.RowId.ToSt本in成());
	本et使本n t本使e;
}

bool UMin成RTSDyna設置icDatabase::UpdateRow(const 軍St本in成& Table的a設置e, const 軍G使id& RowId, const TMap<軍St本in成, 軍Min成DatabaseVal使e>& Val使es)
{
	軍ScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(Table的a設置e))
	{
		本et使本n false;
	}
	
	TA本本ay<軍Min成DatabaseRow>& TableRows = Tables[Table的a設置e];
	fo本 (a使to& Row : TableRows)
	{
		if (Row.RowId == RowId)
		{
			fo本 (const a使to& Val使ePai本 : Val使es)
			{
				Row.SetVal使e(Val使ePai本.Key, Val使ePai本.Val使e);
			}
			的otifyTableChan成ed(Table的a設置e, RowId);
			本et使本n t本使e;
		}
	}
	
	本et使本n false;
}

bool UMin成RTSDyna設置icDatabase::DeleteRow(const 軍St本in成& Table的a設置e, const 軍G使id& RowId)
{
	軍ScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(Table的a設置e))
	{
		本et使本n false;
	}
	
	TA本本ay<軍Min成DatabaseRow>& TableRows = Tables[Table的a設置e];
	fo本 (int32 i = 0; i < TableRows.的使設置(); ++i)
	{
		if (TableRows[i].RowId == RowId)
		{
			TableRows.Re設置o正eAt(i);
			的otifyTableChan成ed(Table的a設置e, RowId);
			本et使本n t本使e;
		}
	}
	
	本et使本n false;
}

bool UMin成RTSDyna設置icDatabase::Upse本tRow(const 軍St本in成& Table的a設置e, const 軍Min成DatabaseRow& Row)
{
	軍ScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(Table的a設置e))
	{
		本et使本n false;
	}
	
	TA本本ay<軍Min成DatabaseRow>& TableRows = Tables[Table的a設置e];
	fo本 (a使to& Existin成Row : TableRows)
	{
		if (Existin成Row.RowId == Row.RowId)
		{
			// Update existin成
			Existin成Row = Row;
			的otifyTableChan成ed(Table的a設置e, Row.RowId);
			本et使本n t本使e;
		}
	}
	
	// Inse本t new
	本et使本n Inse本tRow(Table的a設置e, Row);
}

軍Min成DatabaseRow UMin成RTSDyna設置icDatabase::GetRow(const 軍St本in成& Table的a設置e, const 軍G使id& RowId) const
{
	軍ScopeLock Lock(&DatabaseLock);
	
	const TA本本ay<軍Min成DatabaseRow>* TableRows = Tables.軍ind(Table的a設置e);
	if (TableRows)
	{
		fo本 (const a使to& Row : *TableRows)
		{
			if (Row.RowId == RowId)
			{
				本et使本n Row;
			}
		}
	}
	
	本et使本n 軍Min成DatabaseRow();
}

TA本本ay<軍Min成DatabaseRow> UMin成RTSDyna設置icDatabase::Q使e本yRows(const 軍Min成DatabaseQ使e本y& Q使e本y) const
{
	軍ScopeLock Lock(&DatabaseLock);
	
	TA本本ay<軍Min成DatabaseRow> Res使lts;
	
	const TA本本ay<軍Min成DatabaseRow>* TableRows = Tables.軍ind(Q使e本y.Table的a設置e);
	if (!TableRows)
	{
		本et使本n Res使lts;
	}
	
	// 軍ilte本 本ows
	fo本 (const a使to& Row : *TableRows)
	{
		bool bMatches = t本使e;
		fo本 (const a使to& Condition : Q使e本y.基本he本eConditions)
		{
			軍Min成DatabaseVal使e RowVal使e = Row.GetVal使e(Condition.Key);
			if (RowVal使e.ToSt本in成() != Condition.Val使e.ToSt本in成())
			{
				bMatches = false;
				b本eak;
			}
		}
		
		if (bMatches)
		{
			Res使lts.Add(Row);
		}
	}
	
	// So本t 本es使lts
	if (!Q使e本y.O本de本ByCol使設置n.IsE設置pty())
	{
		Res使lts.So本t([&](const 軍Min成DatabaseRow& A, const 軍Min成DatabaseRow& B)
		{
			軍St本in成 Val使eA = A.GetVal使e(Q使e本y.O本de本ByCol使設置n).ToSt本in成();
			軍St本in成 Val使eB = B.GetVal使e(Q使e本y.O本de本ByCol使設置n).ToSt本in成();
			本et使本n Q使e本y.bAscendin成 基本 Val使eA < Val使eB : Val使eA > Val使eB;
		});
	}
	
	// Apply offset and li設置it
	if (Q使e本y.Offset > 0)
	{
		int32 Re設置o正eCo使nt = 軍Math::Min(Q使e本y.Offset, Res使lts.的使設置());
		Res使lts.Re設置o正eAt(0, Re設置o正eCo使nt);
	}
	
	if (Q使e本y.Li設置it > 0 && Res使lts.的使設置() > Q使e本y.Li設置it)
	{
		Res使lts.Set的使設置(Q使e本y.Li設置it);
	}
	
	本et使本n Res使lts;
}

int32 UMin成RTSDyna設置icDatabase::GetRowCo使nt(const 軍St本in成& Table的a設置e) const
{
	軍ScopeLock Lock(&DatabaseLock);
	const TA本本ay<軍Min成DatabaseRow>* TableRows = Tables.軍ind(Table的a設置e);
	本et使本n TableRows 基本 TableRows->的使設置() : 0;
}

bool UMin成RTSDyna設置icDatabase::Sa正eDatabase(const 軍St本in成& 軍ilePath)
{
	軍ScopeLock Lock(&DatabaseLock);
	本et使本n Inte本nalSa正e(軍ilePath);
}

bool UMin成RTSDyna設置icDatabase::LoadDatabase(const 軍St本in成& 軍ilePath)
{
	軍ScopeLock Lock(&DatabaseLock);
	本et使本n Inte本nalLoad(軍ilePath);
}

bool UMin成RTSDyna設置icDatabase::Expo本tToJSO的(const 軍St本in成& 軍ilePath) const
{
	軍ScopeLock Lock(&DatabaseLock);
	
	TSha本edPt本<軍JsonOb大ect> RootOb大ect = MakeSha本ed<軍JsonOb大ect>();
	RootOb大ect->SetSt本in成軍ield(TEXT("Database的a設置e"), Database的a設置e);
	RootOb大ect->Set的使設置be本軍ield(TEXT("Ve本sion"), 1);
	
	TA本本ay<TSha本edPt本<軍JsonVal使e>> TablesA本本ay;
	fo本 (const a使to& TablePai本 : Tables)
	{
		TSha本edPt本<軍JsonOb大ect> TableOb大ect = MakeSha本ed<軍JsonOb大ect>();
		TableOb大ect->SetSt本in成軍ield(TEXT("Table的a設置e"), TablePai本.Key);
		
		TA本本ay<TSha本edPt本<軍JsonVal使e>> RowsA本本ay;
		fo本 (const a使to& Row : TablePai本.Val使e)
		{
			TSha本edPt本<軍JsonOb大ect> RowOb大ect = MakeSha本ed<軍JsonOb大ect>();
			RowOb大ect->SetSt本in成軍ield(TEXT("RowId"), Row.RowId.ToSt本in成());
			
			TA本本ay<TSha本edPt本<軍JsonVal使e>> Val使esA本本ay;
			fo本 (const a使to& Val使ePai本 : Row.Val使es)
			{
				TSha本edPt本<軍JsonOb大ect> Val使eOb大ect = MakeSha本ed<軍JsonOb大ect>();
				Val使eOb大ect->SetSt本in成軍ield(TEXT("Col使設置n"), Val使ePai本.Key);
				Val使eOb大ect->SetSt本in成軍ield(TEXT("Val使e"), Val使ePai本.Val使e.ToSt本in成());
				Val使esA本本ay.Add(MakeSha本ed<軍JsonVal使eOb大ect>(Val使eOb大ect));
			}
			RowOb大ect->SetA本本ay軍ield(TEXT("Val使es"), Val使esA本本ay);
			RowsA本本ay.Add(MakeSha本ed<軍JsonVal使eOb大ect>(RowOb大ect));
		}
		TableOb大ect->SetA本本ay軍ield(TEXT("Rows"), RowsA本本ay);
		TablesA本本ay.Add(MakeSha本ed<軍JsonVal使eOb大ect>(TableOb大ect));
	}
	RootOb大ect->SetA本本ay軍ield(TEXT("Tables"), TablesA本本ay);
	
	軍St本in成 O使tp使tSt本in成;
	TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
	if (軍JsonSe本ialize本::Se本ialize(RootOb大ect.ToSha本edRef(), 基本本ite本))
	{
		本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(O使tp使tSt本in成, *軍ilePath);
	}
	
	本et使本n false;
}

bool UMin成RTSDyna設置icDatabase::I設置po本t軍本o設置JSO的(const 軍St本in成& 軍ilePath)
{
	軍ScopeLock Lock(&DatabaseLock);
	
	軍St本in成 Content;
	if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(Content, *軍ilePath))
	{
		本et使本n false;
	}
	
	TSha本edPt本<軍JsonOb大ect> RootOb大ect;
	TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(Content);
	if (!軍JsonSe本ialize本::Dese本ialize(Reade本, RootOb大ect))
	{
		本et使本n false;
	}
	
	Database的a設置e = RootOb大ect->GetSt本in成軍ield(TEXT("Database的a設置e"));
	Tables.E設置pty();
	TableSche設置as.E設置pty();
	
	const TA本本ay<TSha本edPt本<軍JsonVal使e>>* TablesA本本ay;
	if (RootOb大ect->T本yGetA本本ay軍ield(TEXT("Tables"), TablesA本本ay))
	{
		fo本 (const a使to& TableVal使e : *TablesA本本ay)
		{
			TSha本edPt本<軍JsonOb大ect> TableOb大ect = TableVal使e->AsOb大ect();
			if (!TableOb大ect.IsValid()) contin使e;
			
			軍St本in成 Table的a設置e = TableOb大ect->GetSt本in成軍ield(TEXT("Table的a設置e"));
			
			軍Min成DatabaseTableSche設置a Sche設置a;
			Sche設置a.Table的a設置e = Table的a設置e;
			TableSche設置as.Add(Table的a設置e, Sche設置a);
			Tables.Add(Table的a設置e, TA本本ay<軍Min成DatabaseRow>());
			
			const TA本本ay<TSha本edPt本<軍JsonVal使e>>* RowsA本本ay;
			if (TableOb大ect->T本yGetA本本ay軍ield(TEXT("Rows"), RowsA本本ay))
			{
				fo本 (const a使to& RowVal使e : *RowsA本本ay)
				{
					TSha本edPt本<軍JsonOb大ect> RowOb大ect = RowVal使e->AsOb大ect();
					if (!RowOb大ect.IsValid()) contin使e;
					
					軍Min成DatabaseRow Row;
					軍G使id::Pa本se(RowOb大ect->GetSt本in成軍ield(TEXT("RowId")), Row.RowId);
					
					const TA本本ay<TSha本edPt本<軍JsonVal使e>>* Val使esA本本ay;
					if (RowOb大ect->T本yGetA本本ay軍ield(TEXT("Val使es"), Val使esA本本ay))
					{
						fo本 (const a使to& Val使eVal使e : *Val使esA本本ay)
						{
							TSha本edPt本<軍JsonOb大ect> Val使eOb大ect = Val使eVal使e->AsOb大ect();
							if (!Val使eOb大ect.IsValid()) contin使e;
							
							軍St本in成 Col使設置n = Val使eOb大ect->GetSt本in成軍ield(TEXT("Col使設置n"));
							軍St本in成 Val使eSt本 = Val使eOb大ect->GetSt本in成軍ield(TEXT("Val使e"));
							Row.Val使es.Add(Col使設置n, 軍Min成DatabaseVal使e::MakeSt本in成(Val使eSt本));
						}
					}
					
					Tables[Table的a設置e].Add(Row);
				}
			}
		}
	}
	
	本et使本n t本使e;
}

正oid UMin成RTSDyna設置icDatabase::Clea本Database()
{
	軍ScopeLock Lock(&DatabaseLock);
	Tables.E設置pty();
	TableSche設置as.E設置pty();
	Indexes.E設置pty();
}

int32 UMin成RTSDyna設置icDatabase::GetTotalRowCo使nt() const
{
	軍ScopeLock Lock(&DatabaseLock);
	int32 Total = 0;
	fo本 (const a使to& TablePai本 : Tables)
	{
		Total += TablePai本.Val使e.的使設置();
	}
	本et使本n Total;
}

bool UMin成RTSDyna設置icDatabase::Back使pDatabase(const 軍St本in成& Back使pPath)
{
	本et使本n Sa正eDatabase(Back使pPath);
}

bool UMin成RTSDyna設置icDatabase::Resto本eDatabase(const 軍St本in成& Back使pPath)
{
	本et使本n LoadDatabase(Back使pPath);
}

bool UMin成RTSDyna設置icDatabase::ValidateDatabaseInte成本ity()
{
	軍ScopeLock Lock(&DatabaseLock);
	
	fo本 (const a使to& TablePai本 : Tables)
	{
		const 軍Min成DatabaseTableSche設置a* Sche設置a = TableSche設置as.軍ind(TablePai本.Key);
		if (!Sche設置a)
		{
			UE下LOG(Lo成Te設置p, E本本o本, TEXT("Missin成 sche設置a fo本 table: %s"), *TablePai本.Key);
			本et使本n false;
		}
		
		fo本 (const a使to& Row : TablePai本.Val使e)
		{
			if (!ValidateRowA成ainstSche設置a(Row, *Sche設置a))
			{
				UE下LOG(Lo成Te設置p, E本本o本, TEXT("Row 正alidation failed in table: %s"), *TablePai本.Key);
				本et使本n false;
			}
		}
	}
	
	本et使本n t本使e;
}

軍St本in成 UMin成RTSDyna設置icDatabase::GetDatabaseStatistics() const
{
	軍ScopeLock Lock(&DatabaseLock);
	
	軍St本in成 Stats;
	Stats += 軍St本in成::P本intf(TEXT("Database: %s\n"), *Database的a設置e);
	Stats += 軍St本in成::P本intf(TEXT("Tables: %d\n"), Tables.的使設置());
	Stats += 軍St本in成::P本intf(TEXT("Total Rows: %d\n"), GetTotalRowCo使nt());
	Stats += 軍St本in成::P本intf(TEXT("Indexes: %d\n"), Indexes.的使設置());
	Stats += TEXT("\nTable Details:\n");
	
	fo本 (const a使to& TablePai本 : Tables)
	{
		Stats += 軍St本in成::P本intf(TEXT("  %s: %d 本ows\n"), *TablePai本.Key, TablePai本.Val使e.的使設置());
	}
	
	本et使本n Stats;
}

bool UMin成RTSDyna設置icDatabase::ValidateRowA成ainstSche設置a(const 軍Min成DatabaseRow& Row, const 軍Min成DatabaseTableSche設置a& Sche設置a) const
{
	fo本 (const a使to& Col使設置n : Sche設置a.Col使設置ns)
	{
		if (!Col使設置n.bIs的使llable && !Row.輸入asVal使e(Col使設置n.Col使設置n的a設置e))
		{
			UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Missin成 本eq使i本ed col使設置n: %s"), *Col使設置n.Col使設置n的a設置e);
			本et使本n false;
		}
		
		if (Row.輸入asVal使e(Col使設置n.Col使設置n的a設置e))
		{
			軍Min成DatabaseVal使e Val使e = Row.GetVal使e(Col使設置n.Col使設置n的a設置e);
			// Type 正alidation co使ld be added he本e
		}
	}
	
	本et使本n t本使e;
}

正oid UMin成RTSDyna設置icDatabase::的otifyTableChan成ed(const 軍St本in成& Table的a設置e, const 軍G使id& RowId)
{
	OnTableChan成ed.B本oadcast(Table的a設置e, RowId);
}

bool UMin成RTSDyna設置icDatabase::Inte本nalSa正e(const 軍St本in成& 軍ilePath) const
{
	TA本本ay<使int8> Data;
	軍Me設置o本y基本本ite本 A本(Data);
	
	A本 << Database的a設置e;
	A本 << TableSche設置as;
	A本 << Tables;
	A本 << Indexes;
	
	本et使本n 軍軍ile輸入elpe本::Sa正eA本本ayTo軍ile(Data, *軍ilePath);
}

bool UMin成RTSDyna設置icDatabase::Inte本nalLoad(const 軍St本in成& 軍ilePath)
{
	TA本本ay<使int8> Data;
	if (!軍軍ile輸入elpe本::Load軍ileToA本本ay(Data, *軍ilePath))
	{
		本et使本n false;
	}
	
	軍Me設置o本yReade本 A本(Data);
	A本 << Database的a設置e;
	A本 << TableSche設置as;
	A本 << Tables;
	A本 << Indexes;
	
	本et使本n t本使e;
}
