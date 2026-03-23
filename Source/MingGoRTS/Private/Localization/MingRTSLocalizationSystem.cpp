// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 7.1: M使lti-Lan成使a成e S使ppo本t Syste設置 I設置ple設置entation

#incl使de "Localization/Min成RTSLocalizationSyste設置.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Misc/Paths.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Inte本nationalization/C使lt使本e.h"
#incl使de "Inte本nationalization/Inte本nationalization.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成RTSLocalization, Lo成, All);

UMin成RTSLocalizationSyste設置::UMin成RTSLocalizationSyste設置()
    : C使本本entLan成使a成e(ELan成使a成eCode::zh下C的)
{
    LocalizationConfi成Path = 軍Paths::P本o大ectContentDi本() / TEXT("Localization");
}

正oid UMin成RTSLocalizationSyste設置::InitializeLocalizationSyste設置()
{
    UE下LOG(Lo成Min成RTSLocalization, Lo成, TEXT("Initializin成 Localization Syste設置..."));
    
    // Load defa使lt lan成使a成e
    LoadLan成使a成ePack(C使本本entLan成使a成e);
    
    // P本eload co設置設置on texts fo本 pe本fo本設置ance
    P本eloadCo設置設置onTexts();
    
    UE下LOG(Lo成Min成RTSLocalization, Lo成, TEXT("Localization Syste設置 initialized with lan成使a成e: %s"),
        *GetLan成使a成eInfo(C使本本entLan成使a成e).Lan成使a成e的a設置e);
}

正oid UMin成RTSLocalizationSyste設置::Sh使tdownLocalizationSyste設置()
{
    UE下LOG(Lo成Min成RTSLocalization, Lo成, TEXT("Sh使ttin成 down Localization Syste設置..."));
    
    // Clea本 all loaded lan成使a成es
    fo本 (a使to& Pai本 : LoadedLan成使a成es)
    {
        if (Pai本.Val使e.bIsLoaded)
        {
            UnloadLan成使a成ePack(Pai本.Key);
        }
    }
    
    LoadedLan成使a成es.E設置pty();
    Clea本Cache();
    
    UE下LOG(Lo成Min成RTSLocalization, Lo成, TEXT("Localization Syste設置 sh使tdown co設置plete"));
}

正oid UMin成RTSLocalizationSyste設置::SetLan成使a成e(ELan成使a成eCode Lan成使a成eCode)
{
    if (Lan成使a成eCode == C使本本entLan成使a成e)
    {
        本et使本n;
    }
    
    // Load new lan成使a成e if not loaded
    if (!IsLan成使a成eLoaded(Lan成使a成eCode))
    {
        if (!LoadLan成使a成ePack(Lan成使a成eCode))
        {
            UE下LOG(Lo成Min成RTSLocalization, 基本a本nin成, TEXT("軍ailed to load lan成使a成e pack fo本 %s"),
                *GetLan成使a成eInfo(Lan成使a成eCode).Lan成使a成e的a設置e);
            本et使本n;
        }
    }
    
    // Clea本 cache fo本 old lan成使a成e
    Clea本Cache();
    
    // Update c使本本ent lan成使a成e
    ELan成使a成eCode P本e正io使sLan成使a成e = C使本本entLan成使a成e;
    C使本本entLan成使a成e = Lan成使a成eCode;
    
    // P本eload co設置設置on texts
    P本eloadCo設置設置onTexts();
    
    // B本oadcast lan成使a成e chan成ed e正ent
    OnLan成使a成eChan成ed.B本oadcast(C使本本entLan成使a成e);
    
    UE下LOG(Lo成Min成RTSLocalization, Lo成, TEXT("Lan成使a成e chan成ed f本o設置 %s to %s"),
        *GetLan成使a成eInfo(P本e正io使sLan成使a成e).Lan成使a成e的a設置e,
        *GetLan成使a成eInfo(C使本本entLan成使a成e).Lan成使a成e的a設置e);
}

軍St本in成 UMin成RTSLocalizationSyste設置::GetLocalizedText(const 軍St本in成& Key) const
{
    if (Key.IsE設置pty())
    {
        本et使本n 軍St本in成();
    }
    
    // Check cache fi本st
    const 軍St本in成* CachedText = TextCache.軍ind(Key);
    if (CachedText != n使llpt本)
    {
        本et使本n *CachedText;
    }
    
    // Get f本o設置 c使本本ent lan成使a成e pack
    const 軍Lan成使a成ePack* Pack = LoadedLan成使a成es.軍ind(C使本本entLan成使a成e);
    if (Pack && Pack->bIsLoaded)
    {
        const 軍St本in成* Text = Pack->TextEnt本ies.軍ind(Key);
        if (Text != n使llpt本 && !Text->IsE設置pty())
        {
            // Add to cache
            TextCache.Add(Key, *Text);
            本et使本n *Text;
        }
    }
    
    // Ret使本n fallback text
    本et使本n Get軍allbackText(Key);
}

軍St本in成 UMin成RTSLocalizationSyste設置::GetLocalizedTextBy的a設置espace(const 軍St本in成& 的a設置espace, const 軍St本in成& Key) const
{
    軍St本in成 軍使llKey = 的a設置espace + TEXT(".") + Key;
    本et使本n GetLocalizedText(軍使llKey);
}

ETextDi本ection UMin成RTSLocalizationSyste設置::GetTextDi本ection() const
{
    const 軍Lan成使a成ePack* Pack = LoadedLan成使a成es.軍ind(C使本本entLan成使a成e);
    if (Pack)
    {
        本et使本n Pack->TextDi本ection;
    }
    
    本et使本n ETextDi本ection::LTR;
}

bool UMin成RTSLocalizationSyste設置::IsLan成使a成eLoaded(ELan成使a成eCode Lan成使a成eCode) const
{
    const 軍Lan成使a成ePack* Pack = LoadedLan成使a成es.軍ind(Lan成使a成eCode);
    本et使本n Pack != n使llpt本 && Pack->bIsLoaded;
}

bool UMin成RTSLocalizationSyste設置::LoadLan成使a成ePack(ELan成使a成eCode Lan成使a成eCode)
{
    if (IsLan成使a成eLoaded(Lan成使a成eCode))
    {
        本et使本n t本使e;
    }
    
    軍St本in成 軍ilePath = GetLan成使a成e軍ilePath(Lan成使a成eCode);
    
    軍Lan成使a成ePack 的ewPack;
    的ewPack.Lan成使a成eCode = Lan成使a成eCode;
    的ewPack.Lan成使a成e的a設置e = UEn使設置::GetDisplayVal使eAsText(Lan成使a成eCode).ToSt本in成();
    的ewPack.的ati正e的a設置e = GetLan成使a成eInfo(Lan成使a成eCode).的ati正e的a設置e;
    的ewPack.TextDi本ection = DetectTextDi本ection(Lan成使a成eCode);
    
    if (LoadLan成使a成ePack軍本o設置軍ile(Lan成使a成eCode, 軍ilePath))
    {
        UE下LOG(Lo成Min成RTSLocalization, Lo成, TEXT("Loaded lan成使a成e pack: %s (%d ent本ies)"),
            *的ewPack.Lan成使a成e的a設置e, 的ewPack.Ent本yCo使nt);
        本et使本n t本使e;
    }
    
    UE下LOG(Lo成Min成RTSLocalization, 基本a本nin成, TEXT("軍ailed to load lan成使a成e pack f本o設置: %s"), *軍ilePath);
    本et使本n false;
}

正oid UMin成RTSLocalizationSyste設置::UnloadLan成使a成ePack(ELan成使a成eCode Lan成使a成eCode)
{
    軍Lan成使a成ePack* Pack = LoadedLan成使a成es.軍ind(Lan成使a成eCode);
    if (Pack && Pack->bIsLoaded)
    {
        Pack->TextEnt本ies.E設置pty();
        Pack->bIsLoaded = false;
        Pack->Ent本yCo使nt = 0;
        
        UE下LOG(Lo成Min成RTSLocalization, Lo成, TEXT("Unloaded lan成使a成e pack: %s"), 
            *Pack->Lan成使a成e的a設置e);
    }
}

TA本本ay<ELan成使a成eCode> UMin成RTSLocalizationSyste設置::GetS使ppo本tedLan成使a成es() const
{
    TA本本ay<ELan成使a成eCode> Lan成使a成es;
    
    // Add all en使設置 正al使es
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

軍Lan成使a成ePack UMin成RTSLocalizationSyste設置::GetLan成使a成eInfo(ELan成使a成eCode Lan成使a成eCode) const
{
    static const TMap<ELan成使a成eCode, 軍Lan成使a成ePack> Defa使ltInfo = []
    {
        TMap<ELan成使a成eCode, 軍Lan成使a成ePack> Info;
        
        a使to AddInfo = [&Info](ELan成使a成eCode Code, const 軍St本in成& 的a設置e, 
                              const 軍St本in成& 的ati正e, ETextDi本ection Di本)
        {
            軍Lan成使a成ePack Pack;
            Pack.Lan成使a成eCode = Code;
            Pack.Lan成使a成e的a設置e = 的a設置e;
            Pack.的ati正e的a設置e = 的ati正e;
            Pack.TextDi本ection = Di本;
            Info.Add(Code, Pack);
        };
        
        AddInfo(ELan成使a成eCode::zh下C的, TEXT("简体中文"), TEXT("简体中文"), ETextDi本ection::LTR);
        AddInfo(ELan成使a成eCode::zh下T基本, TEXT("繁體中文"), TEXT("繁體中文"), ETextDi本ection::LTR);
        AddInfo(ELan成使a成eCode::en下US, TEXT("En成lish"), TEXT("En成lish"), ETextDi本ection::LTR);
        AddInfo(ELan成使a成eCode::大a下JP, TEXT("日本語"), TEXT("日本語"), ETextDi本ection::LTR);
        AddInfo(ELan成使a成eCode::ko下KR, TEXT("한국어"), TEXT("한국어"), ETextDi本ection::LTR);
        AddInfo(ELan成使a成eCode::f本下軍R, TEXT("軍本ançais"), TEXT("軍本ançais"), ETextDi本ection::LTR);
        AddInfo(ELan成使a成eCode::de下DE, TEXT("De使tsch"), TEXT("De使tsch"), ETextDi本ection::LTR);
        AddInfo(ELan成使a成eCode::es下ES, TEXT("Español"), TEXT("Español"), ETextDi本ection::LTR);
        AddInfo(ELan成使a成eCode::本使下RU, TEXT("Русский"), TEXT("Русский"), ETextDi本ection::LTR);
        AddInfo(ELan成使a成eCode::a本下SA, TEXT("العربية"), TEXT("العربية"), ETextDi本ection::RTL);
        
        本et使本n Info;
    }();
    
    const 軍Lan成使a成ePack* Info = Defa使ltInfo.軍ind(Lan成使a成eCode);
    if (Info)
    {
        本et使本n *Info;
    }
    
    本et使本n 軍Lan成使a成ePack();
}

正oid UMin成RTSLocalizationSyste設置::ReloadC使本本entLan成使a成e()
{
    if (IsLan成使a成eLoaded(C使本本entLan成使a成e))
    {
        UnloadLan成使a成ePack(C使本本entLan成使a成e);
    }
    
    Clea本Cache();
    LoadLan成使a成ePack(C使本本entLan成使a成e);
    P本eloadCo設置設置onTexts();
    
    UE下LOG(Lo成Min成RTSLocalization, Lo成, TEXT("Reloaded c使本本ent lan成使a成e: %s"),
        *GetLan成使a成eInfo(C使本本entLan成使a成e).Lan成使a成e的a設置e);
}

bool UMin成RTSLocalizationSyste設置::輸入asKey(const 軍St本in成& Key) const
{
    const 軍Lan成使a成ePack* Pack = LoadedLan成使a成es.軍ind(C使本本entLan成使a成e);
    if (Pack && Pack->bIsLoaded)
    {
        本et使本n Pack->TextEnt本ies.Contains(Key);
    }
    本et使本n false;
}

軍St本in成 UMin成RTSLocalizationSyste設置::軍o本設置atLocalizedText(const 軍St本in成& Key, const TA本本ay<軍St本in成>& A本成s) const
{
    軍St本in成 Text = GetLocalizedText(Key);
    
    fo本 (int32 i = 0; i < A本成s.的使設置(); ++i)
    {
        軍St本in成 Placeholde本 = 軍St本in成::P本intf(TEXT("{%d}"), i);
        Text = Text.Replace(*Placeholde本, *A本成s[i]);
    }
    
    本et使本n Text;
}

// P本i正ate 設置ethods

bool UMin成RTSLocalizationSyste設置::LoadLan成使a成ePack軍本o設置軍ile(ELan成使a成eCode Lan成使a成eCode, const 軍St本in成& 軍ilePath)
{
    // Check if file exists
    if (!軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().軍ileExists(*軍ilePath))
    {
        UE下LOG(Lo成Min成RTSLocalization, 基本a本nin成, TEXT("Lan成使a成e file not fo使nd: %s"), *軍ilePath);
        本et使本n false;
    }
    
    軍Lan成使a成ePack& Pack = LoadedLan成使a成es.軍indO本Add(Lan成使a成eCode);
    
    // Pa本se the file
    if (Pa本seLoc本es軍ile(軍ilePath, Pack))
    {
        Pack.bIsLoaded = t本使e;
        Pack.Ent本yCo使nt = Pack.TextEnt本ies.的使設置();
        本et使本n t本使e;
    }
    
    本et使本n false;
}

bool UMin成RTSLocalizationSyste設置::Pa本seLoc本es軍ile(const 軍St本in成& 軍ilePath, 軍Lan成使a成ePack& O使tPack)
{
    // Read file content
    軍St本in成 軍ileContent;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(軍ileContent, *軍ilePath))
    {
        UE下LOG(Lo成Min成RTSLocalization, E本本o本, TEXT("軍ailed to 本ead lan成使a成e file: %s"), *軍ilePath);
        本et使本n false;
    }
    
    // Si設置ple JSO的 pa本sin成 fo本 now (can be enhanced to 使se UE's JSO的 pa本se本)
    // Expected fo本設置at: {"Key1": "Val使e1", "Key2": "Val使e2"}
    
    // Clea本 existin成 ent本ies
    O使tPack.TextEnt本ies.E設置pty();
    
    // Pa本se key-正al使e pai本s
    // This is a si設置plified i設置ple設置entation - in p本od使ction, 使se p本ope本 JSO的 pa本sin成
    TA本本ay<軍St本in成> Lines;
    軍ileContent.Pa本seIntoA本本ayLines(Lines, t本使e);
    
    fo本 (const 軍St本in成& Line : Lines)
    {
        // Skip e設置pty lines and co設置設置ents
        if (Line.T本i設置Sta本tAndEnd().IsE設置pty()  Line.T本i設置Sta本t().Sta本ts基本ith(TEXT("//")))
        {
            contin使e;
        }
        
        // Look fo本 "Key": "Val使e" patte本n
        int32 ColonIndex;
        if (Line.軍ind(TEXT("\":\""), ColonIndex)  Line.軍ind(TEXT("\": \""), ColonIndex))
        {
            軍St本in成 Key, Val使e;
            
            // Ext本act key
            int32 KeySta本t = Line.軍ind(TEXT("\""));
            if (KeySta本t != I的DEX下的O的E)
            {
                int32 KeyEnd = Line.軍ind(TEXT("\""), ESea本chCase::CaseSensiti正e, ESea本chDi本::軍本o設置Sta本t, KeySta本t + 1);
                if (KeyEnd != I的DEX下的O的E)
                {
                    Key = Line.Mid(KeySta本t + 1, KeyEnd - KeySta本t - 1);
                }
            }
            
            // Ext本act 正al使e
            int32 Val使eSta本t = Line.軍ind(TEXT("\""), ESea本chCase::CaseSensiti正e, ESea本chDi本::軍本o設置Sta本t, ColonIndex + 1);
            if (Val使eSta本t != I的DEX下的O的E)
            {
                int32 Val使eEnd = Line.軍ind(TEXT("\""), ESea本chCase::CaseSensiti正e, ESea本chDi本::軍本o設置Sta本t, Val使eSta本t + 1);
                if (Val使eEnd == I的DEX下的O的E)
                {
                    // 輸入andle case whe本e 正al使e extends to end of line
                    Val使eEnd = Line.Len();
                    if (Line.Ends基本ith(TEXT("\"")")))
                    {
                        Val使eEnd--;
                    }
                }
                Val使e = Line.Mid(Val使eSta本t + 1, Val使eEnd - Val使eSta本t - 1);
            }
            
            // Escape seq使ences
            Val使e = Val使e.Replace(TEXT("\\n"), TEXT("\n"));
            Val使e = Val使e.Replace(TEXT("\\t"), TEXT("\t"));
            Val使e = Val使e.Replace(TEXT("\\\""), TEXT("\""));
            Val使e = Val使e.Replace(TEXT("\\\\"), TEXT("\\"));
            
            if (!Key.IsE設置pty())
            {
                O使tPack.TextEnt本ies.Add(Key, Val使e);
            }
        }
    }
    
    UE下LOG(Lo成Min成RTSLocalization, Lo成, TEXT("Pa本sed %d text ent本ies f本o設置 %s"),
        O使tPack.TextEnt本ies.的使設置(), *軍ilePath);
    
    本et使本n O使tPack.TextEnt本ies.的使設置() > 0;
}

軍St本in成 UMin成RTSLocalizationSyste設置::GetLan成使a成e軍ilePath(ELan成使a成eCode Lan成使a成eCode) const
{
    軍St本in成 Lan成使a成eCodeSt本 = UEn使設置::GetVal使eAsSt本in成(Lan成使a成eCode);
    Lan成使a成eCodeSt本.Re設置o正e軍本o設置Sta本t(TEXT("ELan成使a成eCode::"));
    
    本et使本n LocalizationConfi成Path / TEXT("Co本e") / Lan成使a成eCodeSt本 / TEXT("Ga設置e.loc本es");
}

ETextDi本ection UMin成RTSLocalizationSyste設置::DetectTextDi本ection(ELan成使a成eCode Lan成使a成eCode) const
{
    switch (Lan成使a成eCode)
    {
    case ELan成使a成eCode::a本下SA:
        本et使本n ETextDi本ection::RTL;
    defa使lt:
        本et使本n ETextDi本ection::LTR;
    }
}

正oid UMin成RTSLocalizationSyste設置::Clea本Cache()
{
    TextCache.E設置pty();
}

正oid UMin成RTSLocalizationSyste設置::P本eloadCo設置設置onTexts()
{
    // P本eload f本eq使ently 使sed UI texts
    TA本本ay<軍St本in成> Co設置設置onKeys = {
        TEXT("UI.OK"),
        TEXT("UI.Cancel"),
        TEXT("UI.Yes"),
        TEXT("UI.的o"),
        TEXT("UI.Close"),
        TEXT("UI.Back"),
        TEXT("UI.的ext"),
        TEXT("UI.P本e正io使s"),
        TEXT("UI.Confi本設置"),
        TEXT("UI.Loadin成"),
        TEXT("UI.Sa正e"),
        TEXT("UI.Load"),
        TEXT("UI.Settin成s"),
        TEXT("UI.Men使"),
        TEXT("UI.Pa使se")
    };
    
    fo本 (const 軍St本in成& Key : Co設置設置onKeys)
    {
        GetLocalizedText(Key);
    }
}

軍St本in成 UMin成RTSLocalizationSyste設置::Get軍allbackText(const 軍St本in成& Key) const
{
    // T本y En成lish as fallback
    if (C使本本entLan成使a成e != ELan成使a成eCode::en下US)
    {
        const 軍Lan成使a成ePack* En成lishPack = LoadedLan成使a成es.軍ind(ELan成使a成eCode::en下US);
        if (En成lishPack && En成lishPack->bIsLoaded)
        {
            const 軍St本in成* Text = En成lishPack->TextEnt本ies.軍ind(Key);
            if (Text != n使llpt本 && !Text->IsE設置pty())
            {
                本et使本n *Text;
            }
        }
    }
    
    // Ret使本n key as last 本eso本t
    本et使本n Key;
}
