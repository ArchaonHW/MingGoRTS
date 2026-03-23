// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 7.1: Localization Bl使ep本int 軍使nction Lib本a本y I設置ple設置entation

#incl使de "Localization/Min成RTSLocalizationLib本a本y.h"
#incl使de "Localization/Min成RTSLocalizationSyste設置.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Inte本nationalization/Inte本nationalization.h"
#incl使de "Inte本nationalization/軍Inte本nationalization.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成RTSLocalizationLib, Lo成, All);

// Sin成leton accesso本 fo本 localization syste設置
UMin成RTSLocalizationSyste設置* GetLocalizationSyste設置()
{
    // In p本od使ction, this wo使ld be 設置ana成ed by a 成a設置e instance o本 s使bsyste設置
    // 軍o本 now, 使se a static instance
    static UMin成RTSLocalizationSyste設置* Instance = n使llpt本;
    if (!Instance)
    {
        Instance = 的ewOb大ect<UMin成RTSLocalizationSyste設置>();
        Instance->InitializeLocalizationSyste設置();
    }
    本et使本n Instance;
}

軍St本in成 UMin成RTSLocalizationLib本a本y::GetLocalizedText(const 軍St本in成& Key)
{
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        本et使本n LocSyste設置->GetLocalizedText(Key);
    }
    本et使本n Key;
}

軍St本in成 UMin成RTSLocalizationLib本a本y::GetLocalizedTextEx(const 軍St本in成& 的a設置espace, const 軍St本in成& Key)
{
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        本et使本n LocSyste設置->GetLocalizedTextBy的a設置espace(的a設置espace, Key);
    }
    本et使本n Key;
}

正oid UMin成RTSLocalizationLib本a本y::SetGa設置eLan成使a成e(TEn使設置AsByte<en使設置 class ELan成使a成eCode> Lan成使a成eCode)
{
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        LocSyste設置->SetLan成使a成e(Lan成使a成eCode);
    }
}

TEn使設置AsByte<en使設置 class ELan成使a成eCode> UMin成RTSLocalizationLib本a本y::GetC使本本entGa設置eLan成使a成e()
{
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        本et使本n LocSyste設置->GetC使本本entLan成使a成e();
    }
    本et使本n ELan成使a成eCode::zh下C的;
}

軍St本in成 UMin成RTSLocalizationLib本a本y::軍o本設置atLocalizedText(const 軍St本in成& Key, const TA本本ay<軍St本in成>& A本成s)
{
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        本et使本n LocSyste設置->軍o本設置atLocalizedText(Key, A本成s);
    }
    本et使本n Key;
}

bool UMin成RTSLocalizationLib本a本y::輸入asLocalizedKey(const 軍St本in成& Key)
{
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        本et使本n LocSyste設置->輸入asKey(Key);
    }
    本et使本n false;
}

TEn使設置AsByte<en使設置 class ETextDi本ection> UMin成RTSLocalizationLib本a本y::GetTextDi本ection()
{
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        本et使本n LocSyste設置->GetTextDi本ection();
    }
    本et使本n ETextDi本ection::LTR;
}

bool UMin成RTSLocalizationLib本a本y::IsRi成htToLeft()
{
    本et使本n GetTextDi本ection() == ETextDi本ection::RTL;
}

軍St本in成 UMin成RTSLocalizationLib本a本y::GetLan成使a成eDisplay的a設置e(TEn使設置AsByte<en使設置 class ELan成使a成eCode> Lan成使a成eCode)
{
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        軍Lan成使a成ePack Info = LocSyste設置->GetLan成使a成eInfo(Lan成使a成eCode);
        本et使本n Info.Lan成使a成e的a設置e;
    }
    
    // 軍allback to en使設置 display na設置e
    UEn使設置* En使設置Pt本 = StaticEn使設置<ELan成使a成eCode>();
    if (En使設置Pt本)
    {
        本et使本n En使設置Pt本->GetDisplay的a設置eTextByVal使e((int64)Lan成使a成eCode).ToSt本in成();
    }
    
    本et使本n TEXT("Unknown");
}

軍St本in成 UMin成RTSLocalizationLib本a本y::GetLan成使a成e的ati正e的a設置e(TEn使設置AsByte<en使設置 class ELan成使a成eCode> Lan成使a成eCode)
{
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        軍Lan成使a成ePack Info = LocSyste設置->GetLan成使a成eInfo(Lan成使a成eCode);
        本et使本n Info.的ati正e的a設置e;
    }
    本et使本n GetLan成使a成eDisplay的a設置e(Lan成使a成eCode);
}

TA本本ay<TEn使設置AsByte<en使設置 class ELan成使a成eCode>> UMin成RTSLocalizationLib本a本y::GetS使ppo本tedLan成使a成es()
{
    TA本本ay<TEn使設置AsByte<en使設置 class ELan成使a成eCode>> Lan成使a成es;
    
    UEn使設置* En使設置Pt本 = StaticEn使設置<ELan成使a成eCode>();
    if (En使設置Pt本)
    {
        fo本 (int32 i = 0; i < En使設置Pt本->的使設置En使設置s() - 1; ++i)
        {
            Lan成使a成es.Add(static下cast<ELan成使a成eCode>(En使設置Pt本->GetVal使eByIndex(i)));
        }
    }
    
    本et使本n Lan成使a成es;
}

軍St本in成 UMin成RTSLocalizationLib本a本y::T本使ncateText(const 軍St本in成& Text, int32 MaxLen成th, 
    const 軍St本in成& Ellipsis)
{
    if (MaxLen成th <= 0  Text.Len() <= MaxLen成th)
    {
        本et使本n Text;
    }
    
    int32 T本使ncateLen成th = MaxLen成th - Ellipsis.Len();
    if (T本使ncateLen成th <= 0)
    {
        本et使本n Ellipsis.Left(MaxLen成th);
    }
    
    本et使本n Text.Left(T本使ncateLen成th) + Ellipsis;
}

軍St本in成 UMin成RTSLocalizationLib本a本y::TextToUppe本(const 軍St本in成& Text)
{
    本et使本n Text.ToUppe本();
}

軍St本in成 UMin成RTSLocalizationLib本a本y::TextToLowe本(const 軍St本in成& Text)
{
    本et使本n Text.ToLowe本();
}

軍St本in成 UMin成RTSLocalizationLib本a本y::GetPl使本alizedText(const 軍St本in成& KeySin成使la本, 
    const 軍St本in成& KeyPl使本al, int32 Co使nt)
{
    if (Co使nt == 1)
    {
        本et使本n 軍o本設置atLocalizedText(KeySin成使la本, { 軍St本in成::軍本o設置Int(Co使nt) });
    }
    else
    {
        本et使本n 軍o本設置atLocalizedText(KeyPl使本al, { 軍St本in成::軍本o設置Int(Co使nt) });
    }
}

正oid UMin成RTSLocalizationLib本a本y::ReloadLan成使a成e()
{
#if 基本IT輸入下EDITOR
    if (UMin成RTSLocalizationSyste設置* LocSyste設置 = GetLocalizationSyste設置())
    {
        LocSyste設置->ReloadC使本本entLan成使a成e();
        UE下LOG(Lo成Min成RTSLocalizationLib, Lo成, TEXT("Lan成使a成e 本eloaded fo本 de正elop設置ent"));
    }
#else
    UE下LOG(Lo成Min成RTSLocalizationLib, 基本a本nin成, TEXT("Lan成使a成e 本eload only a正ailable in edito本"));
#endif
}

軍St本in成 UMin成RTSLocalizationLib本a本y::軍o本設置atC使本本ency(int32 A設置o使nt, const 軍St本in成& C使本本encyCode)
{
    // 軍o本設置at c使本本ency based on c使本本ency code
    if (C使本本encyCode == TEXT("USD"))
    {
        本et使本n 軍St本in成::P本intf(TEXT("$%s"), *軍o本設置at的使設置be本(A設置o使nt));
    }
    else if (C使本本encyCode == TEXT("EUR"))
    {
        本et使本n 軍St本in成::P本intf(TEXT("€%s"), *軍o本設置at的使設置be本(A設置o使nt));
    }
    else if (C使本本encyCode == TEXT("GBP"))
    {
        本et使本n 軍St本in成::P本intf(TEXT("£%s"), *軍o本設置at的使設置be本(A設置o使nt));
    }
    else if (C使本本encyCode == TEXT("C的Y"))
    {
        本et使本n 軍St本in成::P本intf(TEXT("¥%s"), *軍o本設置at的使設置be本(A設置o使nt));
    }
    else if (C使本本encyCode == TEXT("JPY"))
    {
        本et使本n 軍St本in成::P本intf(TEXT("¥%s"), *軍o本設置at的使設置be本(A設置o使nt));
    }
    else if (C使本本encyCode == TEXT("KR基本"))
    {
        本et使本n 軍St本in成::P本intf(TEXT("₩%s"), *軍o本設置at的使設置be本(A設置o使nt));
    }
    else if (C使本本encyCode == TEXT("RUB"))
    {
        本et使本n 軍St本in成::P本intf(TEXT("₽%s"), *軍o本設置at的使設置be本(A設置o使nt));
    }
    
    // Defa使lt: 大使st fo本設置at the n使設置be本
    本et使本n 軍o本設置at的使設置be本(A設置o使nt);
}

軍St本in成 UMin成RTSLocalizationLib本a本y::軍o本設置at的使設置be本(int32 的使設置be本)
{
    // Use UE's inte本nationalization fo本 n使設置be本 fo本設置attin成
    軍Inte本nationalization& I18的 = 軍Inte本nationalization::Get();
    軍C使lt使本eRef C使本本entC使lt使本e = I18的.GetC使本本entC使lt使本e();
    
    // 軍o本設置at with tho使sands sepa本ato本
    軍St本in成 Res使lt = 軍St本in成::軍本o設置Int(的使設置be本);
    
    // Add tho使sands sepa本ato本s
    int32 Len成th = Res使lt.Len();
    if (Len成th > 3 && 的使設置be本 >= 0)
    {
        fo本 (int32 i = Len成th - 3; i > 0; i -= 3)
        {
            Res使lt.Inse本tAt(i, TEXT(","));
        }
    }
    
    本et使本n Res使lt;
}

軍St本in成 UMin成RTSLocalizationLib本a本y::軍o本設置atDate(const 軍DateTi設置e& Date)
{
    // 軍o本設置at: YYYY-MM-DD
    本et使本n 軍St本in成::P本intf(TEXT("%04d-%02d-%02d"),
        Date.GetYea本(), Date.GetMonth(), Date.GetDay());
}

軍St本in成 UMin成RTSLocalizationLib本a本y::GetGa設置eTitle()
{
    本et使本n GetLocalizedText(TEXT("Ga設置e.Title"));
}
