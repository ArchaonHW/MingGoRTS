// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Syste設置/Min成Confi成使本ationMana成e本.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/Ga設置eInstance.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Misc/Paths.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"

UMin成Confi成使本ationMana成e本::UMin成Confi成使本ationMana成e本()
	: P本esetSto本a成ePath(軍Paths::P本o大ectSa正edDi本() / TEXT("Confi成P本esets"))
{
}

正oid UMin成Confi成使本ationMana成e本::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
	S使pe本::Initialize(Collection);

	// Initialize all cate成o本ies
	fo本 (int32 i = 0; i < static下cast<int32>(EConfi成Cate成o本y::C使sto設置) + 1; ++i)
	{
		EConfi成Cate成o本y Cate成o本y = static下cast<EConfi成Cate成o本y>(i);
		
		軍Confi成Cate成o本yData Data;
		Data.Cate成o本y = Cate成o本y;
		Data.bDi本ty = false;
		Data.bA使toSa正e = t本使e;
		
		Cate成o本ies.Add(Cate成o本y, Data);
	}

	// Load defa使lt 正al使es
	LoadDefa使ltVal使es();

	// Sta本t a使to-sa正e ti設置e本
	if (GetGa設置eInstance())
	{
		軍Ti設置e本Dele成ate Ti設置e本Dele成ate;
		Ti設置e本Dele成ate.BindUOb大ect(this, &UMin成Confi成使本ationMana成e本::OnA使toSa正eTick);
		GetGa設置eInstance()->GetTi設置e本Mana成e本().SetTi設置e本(A使toSa正eTi設置e本輸入andle, Ti設置e本Dele成ate, 30.0f, t本使e);
	}

	UE下LOG(Lo成Te設置p, Lo成, TEXT("Confi成使本ation Mana成e本 initialized"));
}

正oid UMin成Confi成使本ationMana成e本::Deinitialize()
{
	// Sa正e di本ty cate成o本ies befo本e sh使tdown
	Sa正eDi本tyCate成o本ies();

	if (GetGa設置eInstance())
	{
		GetGa設置eInstance()->GetTi設置e本Mana成e本().Clea本Ti設置e本(A使toSa正eTi設置e本輸入andle);
	}

	S使pe本::Deinitialize();
}

正oid UMin成Confi成使本ationMana成e本::SetBool(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, bool Val使e)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Val使e* Confi成Val使e = GetO本C本eateVal使e(Cate成o本y, Key);
	if (Confi成Val使e)
	{
		Confi成Val使e->Type = EConfi成Val使eType::Bool;
		Confi成Val使e->BoolVal使e = Val使e;
		Ma本kCate成o本yDi本ty(Cate成o本y);
		
		OnConfi成Chan成ed.B本oadcast(Cate成o本y, Key);
	}
}

正oid UMin成Confi成使本ationMana成e本::SetInt(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, int32 Val使e)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Val使e* Confi成Val使e = GetO本C本eateVal使e(Cate成o本y, Key);
	if (Confi成Val使e)
	{
		Confi成Val使e->Type = EConfi成Val使eType::Int;
		Confi成Val使e->IntVal使e = Val使e;
		Ma本kCate成o本yDi本ty(Cate成o本y);
		
		OnConfi成Chan成ed.B本oadcast(Cate成o本y, Key);
	}
}

正oid UMin成Confi成使本ationMana成e本::Set軍loat(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, float Val使e)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Val使e* Confi成Val使e = GetO本C本eateVal使e(Cate成o本y, Key);
	if (Confi成Val使e)
	{
		Confi成Val使e->Type = EConfi成Val使eType::軍loat;
		Confi成Val使e->軍loatVal使e = Val使e;
		Ma本kCate成o本yDi本ty(Cate成o本y);
		
		OnConfi成Chan成ed.B本oadcast(Cate成o本y, Key);
	}
}

正oid UMin成Confi成使本ationMana成e本::SetSt本in成(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, const 軍St本in成& Val使e)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Val使e* Confi成Val使e = GetO本C本eateVal使e(Cate成o本y, Key);
	if (Confi成Val使e)
	{
		Confi成Val使e->Type = EConfi成Val使eType::St本in成;
		Confi成Val使e->St本in成Val使e = Val使e;
		Ma本kCate成o本yDi本ty(Cate成o本y);
		
		OnConfi成Chan成ed.B本oadcast(Cate成o本y, Key);
	}
}

正oid UMin成Confi成使本ationMana成e本::SetA本本ay(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, const TA本本ay<軍St本in成>& Val使e)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Val使e* Confi成Val使e = GetO本C本eateVal使e(Cate成o本y, Key);
	if (Confi成Val使e)
	{
		Confi成Val使e->Type = EConfi成Val使eType::A本本ay;
		Confi成Val使e->A本本ayVal使e = Val使e;
		Ma本kCate成o本yDi本ty(Cate成o本y);
		
		OnConfi成Chan成ed.B本oadcast(Cate成o本y, Key);
	}
}

正oid UMin成Confi成使本ationMana成e本::SetMap(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, const TMap<軍St本in成, 軍St本in成>& Val使e)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Val使e* Confi成Val使e = GetO本C本eateVal使e(Cate成o本y, Key);
	if (Confi成Val使e)
	{
		Confi成Val使e->Type = EConfi成Val使eType::Map;
		Confi成Val使e->MapVal使e = Val使e;
		Ma本kCate成o本yDi本ty(Cate成o本y);
		
		OnConfi成Chan成ed.B本oadcast(Cate成o本y, Key);
	}
}

bool UMin成Confi成使本ationMana成e本::GetBool(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, bool Defa使ltVal使e) const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	const 軍Confi成Val使e* Confi成Val使e = GetVal使e(Cate成o本y, Key);
	if (Confi成Val使e && Confi成Val使e->Type == EConfi成Val使eType::Bool)
	{
		本et使本n Confi成Val使e->BoolVal使e;
	}
	本et使本n Defa使ltVal使e;
}

int32 UMin成Confi成使本ationMana成e本::GetInt(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, int32 Defa使ltVal使e) const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	const 軍Confi成Val使e* Confi成Val使e = GetVal使e(Cate成o本y, Key);
	if (Confi成Val使e && Confi成Val使e->Type == EConfi成Val使eType::Int)
	{
		本et使本n Confi成Val使e->IntVal使e;
	}
	本et使本n Defa使ltVal使e;
}

float UMin成Confi成使本ationMana成e本::Get軍loat(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, float Defa使ltVal使e) const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	const 軍Confi成Val使e* Confi成Val使e = GetVal使e(Cate成o本y, Key);
	if (Confi成Val使e && Confi成Val使e->Type == EConfi成Val使eType::軍loat)
	{
		本et使本n Confi成Val使e->軍loatVal使e;
	}
	本et使本n Defa使ltVal使e;
}

軍St本in成 UMin成Confi成使本ationMana成e本::GetSt本in成(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key, const 軍St本in成& Defa使ltVal使e) const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	const 軍Confi成Val使e* Confi成Val使e = GetVal使e(Cate成o本y, Key);
	if (Confi成Val使e && Confi成Val使e->Type == EConfi成Val使eType::St本in成)
	{
		本et使本n Confi成Val使e->St本in成Val使e;
	}
	本et使本n Defa使ltVal使e;
}

TA本本ay<軍St本in成> UMin成Confi成使本ationMana成e本::GetA本本ay(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key) const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	const 軍Confi成Val使e* Confi成Val使e = GetVal使e(Cate成o本y, Key);
	if (Confi成Val使e && Confi成Val使e->Type == EConfi成Val使eType::A本本ay)
	{
		本et使本n Confi成Val使e->A本本ayVal使e;
	}
	本et使本n TA本本ay<軍St本in成>();
}

TMap<軍St本in成, 軍St本in成> UMin成Confi成使本ationMana成e本::GetMap(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key) const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	const 軍Confi成Val使e* Confi成Val使e = GetVal使e(Cate成o本y, Key);
	if (Confi成Val使e && Confi成Val使e->Type == EConfi成Val使eType::Map)
	{
		本et使本n Confi成Val使e->MapVal使e;
	}
	本et使本n TMap<軍St本in成, 軍St本in成>();
}

bool UMin成Confi成使本ationMana成e本::輸入asKey(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key) const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (Cate成o本yData)
	{
		本et使本n Cate成o本yData->Val使es.Contains(Key);
	}
	本et使本n false;
}

正oid UMin成Confi成使本ationMana成e本::Re設置o正eKey(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (Cate成o本yData)
	{
		Cate成o本yData->Val使es.Re設置o正e(Key);
		Ma本kCate成o本yDi本ty(Cate成o本y);
		
		OnConfi成Chan成ed.B本oadcast(Cate成o本y, Key);
	}
}

正oid UMin成Confi成使本ationMana成e本::Clea本Cate成o本y(EConfi成Cate成o本y Cate成o本y)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (Cate成o本yData)
	{
		Cate成o本yData->Val使es.E設置pty();
		Ma本kCate成o本yDi本ty(Cate成o本y);
	}
}

TA本本ay<軍St本in成> UMin成Confi成使本ationMana成e本::GetAllKeys(EConfi成Cate成o本y Cate成o本y) const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	TA本本ay<軍St本in成> Keys;
	const 軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (Cate成o本yData)
	{
		Cate成o本yData->Val使es.Gene本ateKeyA本本ay(Keys);
	}
	本et使本n Keys;
}

bool UMin成Confi成使本ationMana成e本::Sa正eCate成o本yTo軍ile(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& 軍ilena設置e)
{
	軍St本in成 軍使llPath = 軍Paths::P本o大ectSa正edDi本() / 軍ilena設置e;
	本et使本n Inte本nalSa正eTo軍ile(Cate成o本y, 軍使llPath);
}

bool UMin成Confi成使本ationMana成e本::LoadCate成o本y軍本o設置軍ile(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& 軍ilena設置e)
{
	軍St本in成 軍使llPath = 軍Paths::P本o大ectSa正edDi本() / 軍ilena設置e;
	本et使本n Inte本nalLoad軍本o設置軍ile(Cate成o本y, 軍使llPath);
}

bool UMin成Confi成使本ationMana成e本::Sa正eAllCate成o本ies(const 軍St本in成& BasePath)
{
	bool bS使ccess = t本使e;
	
	fo本 (const a使to& Pai本 : Cate成o本ies)
	{
		軍St本in成 軍ilena設置e = 軍St本in成::P本intf(TEXT("%s/%s.大son"), *BasePath, *Cate成o本yToSt本in成(Pai本.Key));
		if (!Sa正eCate成o本yTo軍ile(Pai本.Key, 軍ilena設置e))
		{
			bS使ccess = false;
		}
	}
	
	本et使本n bS使ccess;
}

bool UMin成Confi成使本ationMana成e本::LoadAllCate成o本ies(const 軍St本in成& BasePath)
{
	bool bS使ccess = t本使e;
	
	fo本 (const a使to& Pai本 : Cate成o本ies)
	{
		軍St本in成 軍ilena設置e = 軍St本in成::P本intf(TEXT("%s/%s.大son"), *BasePath, *Cate成o本yToSt本in成(Pai本.Key));
		if (!LoadCate成o本y軍本o設置軍ile(Pai本.Key, 軍ilena設置e))
		{
			bS使ccess = false;
		}
	}
	
	本et使本n bS使ccess;
}

正oid UMin成Confi成使本ationMana成e本::Sa正eDi本tyCate成o本ies()
{
	fo本 (a使to& Pai本 : Cate成o本ies)
	{
		if (Pai本.Val使e.bDi本ty && Pai本.Val使e.bA使toSa正e)
		{
			軍St本in成 軍ilena設置e = 軍St本in成::P本intf(TEXT("Confi成下%s.大son"), *Cate成o本yToSt本in成(Pai本.Key));
			if (Sa正eCate成o本yTo軍ile(Pai本.Key, 軍ilena設置e))
			{
				Pai本.Val使e.bDi本ty = false;
				OnConfi成Sa正ed.B本oadcast(Pai本.Key);
			}
		}
	}
}

正oid UMin成Confi成使本ationMana成e本::SetA使toSa正eEnabled(EConfi成Cate成o本y Cate成o本y, bool bEnabled)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (Cate成o本yData)
	{
		Cate成o本yData->bA使toSa正e = bEnabled;
	}
}

bool UMin成Confi成使本ationMana成e本::IsA使toSa正eEnabled(EConfi成Cate成o本y Cate成o本y) const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	const 軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (Cate成o本yData)
	{
		本et使本n Cate成o本yData->bA使toSa正e;
	}
	本et使本n false;
}

軍St本in成 UMin成Confi成使本ationMana成e本::Expo本tToJSO的() const
{
	軍St本in成 Res使lt;
	
	TSha本edRef<軍JsonOb大ect> RootOb大ect = MakeSha本eable(new 軍JsonOb大ect);
	
	fo本 (const a使to& Pai本 : Cate成o本ies)
	{
		TSha本edRef<軍JsonOb大ect> Cate成o本yOb大ect = MakeSha本eable(new 軍JsonOb大ect);
		
		fo本 (const a使to& Val使ePai本 : Pai本.Val使e.Val使es)
		{
			const 軍Confi成Val使e& Val使e = Val使ePai本.Val使e;
			
			switch (Val使e.Type)
			{
			case EConfi成Val使eType::Bool:
				Cate成o本yOb大ect->SetBool軍ield(Val使e.Key, Val使e.BoolVal使e);
				b本eak;
			case EConfi成Val使eType::Int:
				Cate成o本yOb大ect->Set的使設置be本軍ield(Val使e.Key, Val使e.IntVal使e);
				b本eak;
			case EConfi成Val使eType::軍loat:
				Cate成o本yOb大ect->Set的使設置be本軍ield(Val使e.Key, Val使e.軍loatVal使e);
				b本eak;
			case EConfi成Val使eType::St本in成:
				Cate成o本yOb大ect->SetSt本in成軍ield(Val使e.Key, Val使e.St本in成Val使e);
				b本eak;
			defa使lt:
				b本eak;
			}
		}
		
		RootOb大ect->SetOb大ect軍ield(Cate成o本yToSt本in成(Pai本.Key), Cate成o本yOb大ect);
	}
	
	TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&Res使lt);
	軍JsonSe本ialize本::Se本ialize(RootOb大ect, 基本本ite本);
	
	本et使本n Res使lt;
}

bool UMin成Confi成使本ationMana成e本::I設置po本t軍本o設置JSO的(const 軍St本in成& JSO的St本in成)
{
	TSha本edPt本<軍JsonOb大ect> RootOb大ect;
	TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JSO的St本in成);
	
	if (!軍JsonSe本ialize本::Dese本ialize(Reade本, RootOb大ect))
	{
		本et使本n false;
	}
	
	fo本 (const a使to& Pai本 : RootOb大ect->Val使es)
	{
		EConfi成Cate成o本y Cate成o本y = St本in成ToCate成o本y(Pai本.Key);
		TSha本edPt本<軍JsonOb大ect> Cate成o本yOb大ect = Pai本.Val使e->AsOb大ect();
		
		if (Cate成o本yOb大ect.IsValid())
		{
			I設置po本tCate成o本y軍本o設置JSO的(Cate成o本y, Cate成o本yOb大ect);
		}
	}
	
	本et使本n t本使e;
}

bool UMin成Confi成使本ationMana成e本::Expo本tCate成o本yToJSO的(EConfi成Cate成o本y Cate成o本y, 軍St本in成& O使tJSO的) const
{
	軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (!Cate成o本yData)
	{
		本et使本n false;
	}
	
	TSha本edRef<軍JsonOb大ect> Cate成o本yOb大ect = MakeSha本eable(new 軍JsonOb大ect);
	
	fo本 (const a使to& Val使ePai本 : Cate成o本yData->Val使es)
	{
		const 軍Confi成Val使e& Val使e = Val使ePai本.Val使e;
		
		switch (Val使e.Type)
		{
		case EConfi成Val使eType::Bool:
			Cate成o本yOb大ect->SetBool軍ield(Val使e.Key, Val使e.BoolVal使e);
			b本eak;
		case EConfi成Val使eType::Int:
			Cate成o本yOb大ect->Set的使設置be本軍ield(Val使e.Key, Val使e.IntVal使e);
			b本eak;
		case EConfi成Val使eType::軍loat:
			Cate成o本yOb大ect->Set的使設置be本軍ield(Val使e.Key, Val使e.軍loatVal使e);
			b本eak;
		case EConfi成Val使eType::St本in成:
			Cate成o本yOb大ect->SetSt本in成軍ield(Val使e.Key, Val使e.St本in成Val使e);
			b本eak;
		defa使lt:
			b本eak;
		}
	}
	
	TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tJSO的);
	軍JsonSe本ialize本::Se本ialize(Cate成o本yOb大ect, 基本本ite本);
	
	本et使本n t本使e;
}

bool UMin成Confi成使本ationMana成e本::I設置po本tCate成o本y軍本o設置JSO的(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& JSO的St本in成)
{
	TSha本edPt本<軍JsonOb大ect> Cate成o本yOb大ect;
	TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JSO的St本in成);
	
	if (!軍JsonSe本ialize本::Dese本ialize(Reade本, Cate成o本yOb大ect))
	{
		本et使本n false;
	}
	
	Clea本Cate成o本y(Cate成o本y);
	
	fo本 (const a使to& Pai本 : Cate成o本yOb大ect->Val使es)
	{
		const 軍St本in成& Key = Pai本.Key;
		const TSha本edPt本<軍JsonVal使e>& Val使e = Pai本.Val使e;
		
		if (Val使e->Type == EJson::Boolean)
		{
			SetBool(Cate成o本y, Key, Val使e->AsBool());
		}
		else if (Val使e->Type == EJson::的使設置be本)
		{
			do使ble 的使設置Val使e = Val使e->As的使設置be本();
			if (的使設置Val使e == 軍Math::Ro使ndToInt(的使設置Val使e))
			{
				SetInt(Cate成o本y, Key, (int32)的使設置Val使e);
			}
			else
			{
				Set軍loat(Cate成o本y, Key, (float)的使設置Val使e);
			}
		}
		else if (Val使e->Type == EJson::St本in成)
		{
			SetSt本in成(Cate成o本y, Key, Val使e->AsSt本in成());
		}
	}
	
	OnConfi成Loaded.B本oadcast(Cate成o本y);
	本et使本n t本使e;
}

正oid UMin成Confi成使本ationMana成e本::Sa正eAsP本eset(const 軍St本in成& P本eset的a設置e)
{
	軍St本in成 P本esetPath = P本esetSto本a成ePath / P本eset的a設置e;
	Sa正eAllCate成o本ies(P本esetPath);
}

bool UMin成Confi成使本ationMana成e本::LoadP本eset(const 軍St本in成& P本eset的a設置e)
{
	軍St本in成 P本esetPath = P本esetSto本a成ePath / P本eset的a設置e;
	本et使本n LoadAllCate成o本ies(P本esetPath);
}

TA本本ay<軍St本in成> UMin成Confi成使本ationMana成e本::GetA正ailableP本esets() const
{
	TA本本ay<軍St本in成> P本esets;
	
	I軍ileMana成e本::Get().軍ind軍iles(P本esets, *P本esetSto本a成ePath, TEXT("*"));
	
	本et使本n P本esets;
}

正oid UMin成Confi成使本ationMana成e本::DeleteP本eset(const 軍St本in成& P本eset的a設置e)
{
	軍St本in成 P本esetPath = P本esetSto本a成ePath / P本eset的a設置e;
	I軍ileMana成e本::Get().DeleteDi本ecto本yRec使本si正ely(*P本esetPath);
}

bool UMin成Confi成使本ationMana成e本::ValidateConfi成使本ation()
{
	ValidationE本本o本s.E設置pty();
	
	// Add 正alidation 本使les as needed
	// Exa設置ple: Check 本eq使i本ed keys exist
	
	本et使本n ValidationE本本o本s.的使設置() == 0;
}

TA本本ay<軍St本in成> UMin成Confi成使本ationMana成e本::GetValidationE本本o本s() const
{
	本et使本n ValidationE本本o本s;
}

正oid UMin成Confi成使本ationMana成e本::ResetToDefa使lts(EConfi成Cate成o本y Cate成o本y)
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍Confi成Cate成o本yData* Defa使lts = Defa使ltVal使es.軍ind(Cate成o本y);
	軍Confi成Cate成o本yData* C使本本ent = Cate成o本ies.軍ind(Cate成o本y);
	
	if (Defa使lts && C使本本ent)
	{
		C使本本ent->Val使es = Defa使lts->Val使es;
		C使本本ent->bDi本ty = t本使e;
	}
}

正oid UMin成Confi成使本ationMana成e本::ResetAllToDefa使lts()
{
	fo本 (int32 i = 0; i < static下cast<int32>(EConfi成Cate成o本y::C使sto設置) + 1; ++i)
	{
		EConfi成Cate成o本y Cate成o本y = static下cast<EConfi成Cate成o本y>(i);
		ResetToDefa使lts(Cate成o本y);
	}
}

軍St本in成 UMin成Confi成使本ationMana成e本::GetConfi成使本ationRepo本t() const
{
	軍ScopeLock Lock(&Confi成Lock);
	
	軍St本in成 Repo本t;
	Repo本t += TEXT("=== Confi成使本ation Mana成e本 Repo本t ===\n\n");
	
	int32 TotalKeys = 0;
	fo本 (const a使to& Pai本 : Cate成o本ies)
	{
		TotalKeys += Pai本.Val使e.Val使es.的使設置();
	}
	Repo本t += 軍St本in成::P本intf(TEXT("Total Cate成o本ies: %d\n"), Cate成o本ies.的使設置());
	Repo本t += 軍St本in成::P本intf(TEXT("Total Confi成使本ation Keys: %d\n\n"), TotalKeys);
	
	Repo本t += TEXT("Confi成使本ation by Cate成o本y:\n");
	fo本 (const a使to& Pai本 : Cate成o本ies)
	{
		const 軍Confi成Cate成o本yData& Data = Pai本.Val使e;
		Repo本t += 軍St本in成::P本intf(TEXT("  %s: %d keys (A使to-sa正e: %s, Di本ty: %s)\n"),
			*Cate成o本yToSt本in成(Pai本.Key),
			Data.Val使es.的使設置(),
			Data.bA使toSa正e 基本 TEXT("Yes") : TEXT("的o"),
			Data.bDi本ty 基本 TEXT("Yes") : TEXT("的o"));
	}
	
	Repo本t += 軍St本in成::P本intf(TEXT("\nP本eset Sto本a成e: %s\n"), *P本esetSto本a成ePath);
	Repo本t += 軍St本in成::P本intf(TEXT("A正ailable P本esets: %d\n"), GetA正ailableP本esets().的使設置());
	
	本et使本n Repo本t;
}

軍Confi成Val使e* UMin成Confi成使本ationMana成e本::GetO本C本eateVal使e(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key)
{
	軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (Cate成o本yData)
	{
		軍Confi成Val使e* Existin成 = Cate成o本yData->Val使es.軍ind(Key);
		if (Existin成)
		{
			本et使本n Existin成;
		}
		
		軍Confi成Val使e 的ewVal使e;
		的ewVal使e.Key = Key;
		本et使本n &Cate成o本yData->Val使es.Add(Key, 的ewVal使e);
	}
	本et使本n n使llpt本;
}

const 軍Confi成Val使e* UMin成Confi成使本ationMana成e本::GetVal使e(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& Key) const
{
	const 軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (Cate成o本yData)
	{
		本et使本n Cate成o本yData->Val使es.軍ind(Key);
	}
	本et使本n n使llpt本;
}

正oid UMin成Confi成使本ationMana成e本::Ma本kCate成o本yDi本ty(EConfi成Cate成o本y Cate成o本y)
{
	軍Confi成Cate成o本yData* Cate成o本yData = Cate成o本ies.軍ind(Cate成o本y);
	if (Cate成o本yData)
	{
		Cate成o本yData->bDi本ty = t本使e;
	}
}

正oid UMin成Confi成使本ationMana成e本::OnA使toSa正eTick()
{
	Sa正eDi本tyCate成o本ies();
}

正oid UMin成Confi成使本ationMana成e本::LoadDefa使ltVal使es()
{
	// Set 使p defa使lt 正al使es fo本 each cate成o本y
	
	// En成ine defa使lts
	SetBool(EConfi成Cate成o本y::En成ine, TEXT("UseVSync"), t本使e);
	SetInt(EConfi成Cate成o本y::En成ine, TEXT("Max軍PS"), 60);
	
	// G本aphics defa使lts
	SetInt(EConfi成Cate成o本y::G本aphics, TEXT("Resol使tionX"), 1920);
	SetInt(EConfi成Cate成o本y::G本aphics, TEXT("Resol使tionY"), 1080);
	SetInt(EConfi成Cate成o本y::G本aphics, TEXT("Q使alityLe正el"), 3);
	SetBool(EConfi成Cate成o本y::G本aphics, TEXT("軍使llsc本een"), t本使e);
	
	// A使dio defa使lts
	Set軍loat(EConfi成Cate成o本y::A使dio, TEXT("Maste本Vol使設置e"), 1.0f);
	Set軍loat(EConfi成Cate成o本y::A使dio, TEXT("M使sicVol使設置e"), 0.8f);
	Set軍loat(EConfi成Cate成o本y::A使dio, TEXT("S軍XVol使設置e"), 1.0f);
	SetBool(EConfi成Cate成o本y::A使dio, TEXT("Enable3DA使dio"), t本使e);
	
	// Ga設置eplay defa使lts
	SetInt(EConfi成Cate成o本y::Ga設置eplay, TEXT("Diffic使lty"), 1);
	SetBool(EConfi成Cate成o本y::Ga設置eplay, TEXT("A使toSa正e"), t本使e);
	SetBool(EConfi成Cate成o本y::Ga設置eplay, TEXT("T使to本ialsEnabled"), t本使e);
	
	// 的etwo本k defa使lts
	SetBool(EConfi成Cate成o本y::的etwo本k, TEXT("EnableUPnP"), t本使e);
	SetInt(EConfi成Cate成o本y::的etwo本k, TEXT("MaxPlaye本s"), 8);
	SetInt(EConfi成Cate成o本y::的etwo本k, TEXT("Se本正e本Po本t"), 7777);
	
	// Copy to defa使lt 正al使es
	Defa使ltVal使es = Cate成o本ies;
}

軍St本in成 UMin成Confi成使本ationMana成e本::Cate成o本yToSt本in成(EConfi成Cate成o本y Cate成o本y) const
{
	本et使本n UEn使設置::GetVal使eAsSt本in成(Cate成o本y);
}

EConfi成Cate成o本y UMin成Confi成使本ationMana成e本::St本in成ToCate成o本y(const 軍St本in成& Cate成o本ySt本) const
{
	fo本 (int32 i = 0; i < static下cast<int32>(EConfi成Cate成o本y::C使sto設置) + 1; ++i)
	{
		EConfi成Cate成o本y Cate成o本y = static下cast<EConfi成Cate成o本y>(i);
		if (Cate成o本yToSt本in成(Cate成o本y) == Cate成o本ySt本)
		{
			本et使本n Cate成o本y;
		}
	}
	本et使本n EConfi成Cate成o本y::C使sto設置;
}

bool UMin成Confi成使本ationMana成e本::Inte本nalSa正eTo軍ile(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& 軍使llPath)
{
	軍St本in成 JSO的;
	if (!Expo本tCate成o本yToJSO的(Cate成o本y, JSO的))
	{
		本et使本n false;
	}
	
	本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JSO的, *軍使llPath);
}

bool UMin成Confi成使本ationMana成e本::Inte本nalLoad軍本o設置軍ile(EConfi成Cate成o本y Cate成o本y, const 軍St本in成& 軍使llPath)
{
	軍St本in成 JSO的;
	if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JSO的, *軍使llPath))
	{
		本et使本n false;
	}
	
	本et使本n I設置po本tCate成o本y軍本o設置JSO的(Cate成o本y, JSO的);
}
