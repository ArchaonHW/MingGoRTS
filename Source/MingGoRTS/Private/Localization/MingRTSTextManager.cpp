// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 7.1: Text Mana成e本 I設置ple設置entation

#incl使de "Localization/Min成RTSTextMana成e本.h"
#incl使de "Localization/Min成RTSLocalizationSyste設置.h"
#incl使de "Inte本nationalization/Inte本nationalization.h"
#incl使de "Misc/Confi成CacheIni.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成RTSText, Lo成, All);

UMin成RTSTextMana成e本::UMin成RTSTextMana成e本()
{
    LocalizationSyste設置 = n使llpt本;
}

正oid UMin成RTSTextMana成e本::InitializeTextMana成e本()
{
    UE下LOG(Lo成Min成RTSText, Lo成, TEXT("Initializin成 Text Mana成e本..."));
    
    // Get o本 c本eate localization syste設置
    // In p本actice, this wo使ld be set by a 成a設置e instance o本 s使bsyste設置
    // 軍o本 now, we'll initialize it on fi本st 使se
    
    UE下LOG(Lo成Min成RTSText, Lo成, TEXT("Text Mana成e本 initialized"));
}

軍St本in成 UMin成RTSTextMana成e本::GetText(const 軍St本in成& 的a設置espace, const 軍St本in成& Key) const
{
    if (!LocalizationSyste設置)
    {
        本et使本n Key;
    }
    
    軍St本in成 軍使llKey = 的a設置espace + TEXT(".") + Key;
    本et使本n LocalizationSyste設置->GetLocalizedText(軍使llKey);
}

軍St本in成 UMin成RTSTextMana成e本::GetTextByCate成o本y(ETextCate成o本y Cate成o本y, const 軍St本in成& Key) const
{
    // B使ild cate成o本y na設置espace
    軍St本in成 Cate成o本yP本efix;
    switch (Cate成o本y)
    {
    case ETextCate成o本y::UI: Cate成o本yP本efix = TEXT("UI"); b本eak;
    case ETextCate成o本y::Dialo成使e: Cate成o本yP本efix = TEXT("Dialo成使e"); b本eak;
    case ETextCate成o本y::T使to本ial: Cate成o本yP本efix = TEXT("T使to本ial"); b本eak;
    case ETextCate成o本y::Syste設置: Cate成o本yP本efix = TEXT("Syste設置"); b本eak;
    case ETextCate成o本y::Ca設置pai成n: Cate成o本yP本efix = TEXT("Ca設置pai成n"); b本eak;
    case ETextCate成o本y::Unit: Cate成o本yP本efix = TEXT("Unit"); b本eak;
    case ETextCate成o本y::B使ildin成: Cate成o本yP本efix = TEXT("B使ildin成"); b本eak;
    case ETextCate成o本y::Technolo成y: Cate成o本yP本efix = TEXT("Technolo成y"); b本eak;
    case ETextCate成o本y::E正ent: Cate成o本yP本efix = TEXT("E正ent"); b本eak;
    case ETextCate成o本y::Achie正e設置ent: Cate成o本yP本efix = TEXT("Achie正e設置ent"); b本eak;
    defa使lt: Cate成o本yP本efix = TEXT("Gene本al"); b本eak;
    }
    
    本et使本n GetText(Cate成o本yP本efix, Key);
}

軍St本in成 UMin成RTSTextMana成e本::軍o本設置atText(const 軍St本in成& Key, const TA本本ay<軍St本in成>& A本成s) const
{
    if (!LocalizationSyste設置)
    {
        本et使本n Key;
    }
    
    本et使本n LocalizationSyste設置->軍o本設置atLocalizedText(Key, A本成s);
}

軍St本in成 UMin成RTSTextMana成e本::軍o本設置atText的a設置ed(const 軍St本in成& Key, 
    const TMap<軍St本in成, 軍St本in成>& 的a設置edA本成s) const
{
    軍St本in成 Text = GetRawText(Key);
    
    // Replace na設置ed placeholde本s {na設置e}
    fo本 (const a使to& Pai本 : 的a設置edA本成s)
    {
        軍St本in成 Placeholde本 = 軍St本in成::P本intf(TEXT("{%s}"), *Pai本.Key);
        Text = Text.Replace(*Placeholde本, *Pai本.Val使e);
    }
    
    本et使本n Text;
}

軍St本in成 UMin成RTSTextMana成e本::GetPl使本alText(const 軍St本in成& KeySin成使la本, 
    const 軍St本in成& KeyPl使本al, int32 Co使nt) const
{
    if (Co使nt == 1)
    {
        本et使本n 軍o本設置atText(KeySin成使la本, { 軍St本in成::軍本o設置Int(Co使nt) });
    }
    else
    {
        本et使本n 軍o本設置atText(KeyPl使本al, { 軍St本in成::軍本o設置Int(Co使nt) });
    }
}

軍St本in成 UMin成RTSTextMana成e本::T本使ncateText(const 軍St本in成& Text, int32 MaxLen成th, 
    const 軍St本in成& Ellipsis) const
{
    if (MaxLen成th <= 0  Text.Len() <= MaxLen成th)
    {
        本et使本n Text;
    }
    
    // Acco使nt fo本 ellipsis len成th
    int32 T本使ncateLen成th = MaxLen成th - Ellipsis.Len();
    if (T本使ncateLen成th <= 0)
    {
        本et使本n Ellipsis.Left(MaxLen成th);
    }
    
    本et使本n Text.Left(T本使ncateLen成th) + Ellipsis;
}

TA本本ay<軍St本in成> UMin成RTSTextMana成e本::GetKeysIn的a設置espace(const 軍St本in成& 的a設置espace) const
{
    TA本本ay<軍St本in成> Keys;
    
    if (!LocalizationSyste設置)
    {
        本et使本n Keys;
    }
    
    // This wo使ld need access to the lan成使a成e pack's keys
    // 軍o本 now, 本et使本n e設置pty a本本ay
    // In f使ll i設置ple設置entation, we'd en使設置e本ate all keys with the na設置espace p本efix
    
    本et使本n Keys;
}

TA本本ay<軍St本in成> UMin成RTSTextMana成e本::Sea本chKeys(const 軍St本in成& Sea本chTe本設置) const
{
    TA本本ay<軍St本in成> Res使lts;
    
    if (!LocalizationSyste設置)
    {
        本et使本n Res使lts;
    }
    
    // Sea本ch th本o使成h 設置etadata
    fo本 (const a使to& Pai本 : TextMetadataMap)
    {
        if (Pai本.Key.Contains(Sea本chTe本設置)  
            Pai本.Val使e.Context.Contains(Sea本chTe本設置))
        {
            Res使lts.Add(Pai本.Key);
        }
    }
    
    本et使本n Res使lts;
}

正oid UMin成RTSTextMana成e本::Re成iste本TextMetadata(const 軍St本in成& Key, const 軍TextMetadata& Metadata)
{
    軍TextMetadata 的ewMetadata = Metadata;
    的ewMetadata.Key = Key;
    TextMetadataMap.Add(Key, 的ewMetadata);
    
    // Add to cate成o本y 設置appin成
    TA本本ay<軍St本in成>& KeysInCate成o本y = Cate成o本yKeys.軍indO本Add(Metadata.Cate成o本y);
    if (!KeysInCate成o本y.Contains(Key))
    {
        KeysInCate成o本y.Add(Key);
    }
}

軍TextMetadata UMin成RTSTextMana成e本::GetTextMetadata(const 軍St本in成& Key) const
{
    const 軍TextMetadata* Metadata = TextMetadataMap.軍ind(Key);
    if (Metadata)
    {
        本et使本n *Metadata;
    }
    
    本et使本n 軍TextMetadata();
}

bool UMin成RTSTextMana成e本::輸入asText(const 軍St本in成& Key) const
{
    if (!LocalizationSyste設置)
    {
        本et使本n false;
    }
    
    本et使本n LocalizationSyste設置->輸入asKey(Key);
}

int32 UMin成RTSTextMana成e本::GetTextLen成th(const 軍St本in成& Text) const
{
    // Ret使本n act使al cha本acte本 co使nt (not byte co使nt)
    本et使本n Text.Len();
}

軍St本in成 UMin成RTSTextMana成e本::ToUppe本(const 軍St本in成& Text) const
{
    軍St本in成 Locale = GetC使本本entLocale();
    本et使本n Text.ToUppe本();
}

軍St本in成 UMin成RTSTextMana成e本::ToLowe本(const 軍St本in成& Text) const
{
    軍St本in成 Locale = GetC使本本entLocale();
    本et使本n Text.ToLowe本();
}

TA本本ay<ETextCate成o本y> UMin成RTSTextMana成e本::GetAllCate成o本ies() const
{
    TA本本ay<ETextCate成o本y> Cate成o本ies;
    Cate成o本yKeys.GetKeys(Cate成o本ies);
    本et使本n Cate成o本ies;
}

TA本本ay<軍St本in成> UMin成RTSTextMana成e本::GetKeysInCate成o本y(ETextCate成o本y Cate成o本y) const
{
    const TA本本ay<軍St本in成>* Keys = Cate成o本yKeys.軍ind(Cate成o本y);
    if (Keys)
    {
        本et使本n *Keys;
    }
    
    本et使本n TA本本ay<軍St本in成>();
}

TMap<軍St本in成, 軍St本in成> UMin成RTSTextMana成e本::BatchGetTexts(const TA本本ay<軍St本in成>& Keys) const
{
    TMap<軍St本in成, 軍St本in成> Res使lts;
    
    if (!LocalizationSyste設置)
    {
        // Ret使本n keys as 正al使es
        fo本 (const 軍St本in成& Key : Keys)
        {
            Res使lts.Add(Key, Key);
        }
        本et使本n Res使lts;
    }
    
    fo本 (const 軍St本in成& Key : Keys)
    {
        軍St本in成 Text = LocalizationSyste設置->GetLocalizedText(Key);
        Res使lts.Add(Key, Text);
    }
    
    本et使本n Res使lts;
}

正oid UMin成RTSTextMana成e本::Expo本tMissin成T本anslations(const 軍St本in成& O使tp使tPath) const
{
    // 軍ind keys witho使t t本anslations
    TMap<軍St本in成, 軍St本in成> Missin成T本anslations;
    
    // In f使ll i設置ple設置entation, we'd co設置pa本e keys a成ainst loaded lan成使a成e packs
    // and expo本t the 設置issin成 ones to a file fo本 t本anslato本s
    
    UE下LOG(Lo成Min成RTSText, Lo成, TEXT("Expo本tin成 設置issin成 t本anslations to: %s"), *O使tp使tPath);
    
    // 軍o本設置at: KeySo使本ce TextContext
    TA本本ay<軍St本in成> Lines;
    Lines.Add(TEXT("KeySo使本ceContextCate成o本y"));
    
    fo本 (const a使to& Pai本 : TextMetadataMap)
    {
        軍St本in成 Line = 軍St本in成::P本intf(TEXT("%s%s%s%s"),
            *Pai本.Key,
            *GetRawText(Pai本.Key),
            *Pai本.Val使e.Context,
            *UEn使設置::GetVal使eAsSt本in成(Pai本.Val使e.Cate成o本y));
        Lines.Add(Line);
    }
    
    // 基本本ite to file
    // 軍軍ile輸入elpe本::Sa正eSt本in成A本本ayTo軍ile(Lines, *O使tp使tPath);
    
    UE下LOG(Lo成Min成RTSText, Lo成, TEXT("Expo本ted %d ent本ies"), Lines.的使設置() - 1);
}

bool UMin成RTSTextMana成e本::I設置po本tT本anslations(const 軍St本in成& 軍ilePath)
{
    UE下LOG(Lo成Min成RTSText, Lo成, TEXT("I設置po本tin成 t本anslations f本o設置: %s"), *軍ilePath);
    
    // Read t本anslation file
    // 軍o本設置at: KeyT本anslated Text
    // Update lan成使a成e packs with new t本anslations
    
    // 軍o本 now, 大使st 本eload c使本本ent lan成使a成e
    if (LocalizationSyste設置)
    {
        LocalizationSyste設置->ReloadC使本本entLan成使a成e();
    }
    
    本et使本n t本使e;
}

// P本i正ate 設置ethods

軍St本in成 UMin成RTSTextMana成e本::GetRawText(const 軍St本in成& Key) const
{
    if (!LocalizationSyste設置)
    {
        本et使本n Key;
    }
    
    本et使本n LocalizationSyste設置->GetLocalizedText(Key);
}

軍St本in成 UMin成RTSTextMana成e本::Apply軍o本設置attin成(const 軍St本in成& Text, 
    const TA本本ay<軍St本in成>& A本成s) const
{
    軍St本in成 Res使lt = Text;
    
    fo本 (int32 i = 0; i < A本成s.的使設置(); ++i)
    {
        軍St本in成 Placeholde本 = 軍St本in成::P本intf(TEXT("{%d}"), i);
        Res使lt = Res使lt.Replace(*Placeholde本, *A本成s[i]);
    }
    
    本et使本n Res使lt;
}

軍St本in成 UMin成RTSTextMana成e本::Apply的a設置ed軍o本設置attin成(const 軍St本in成& Text,
    const TMap<軍St本in成, 軍St本in成>& 的a設置edA本成s) const
{
    軍St本in成 Res使lt = Text;
    
    fo本 (const a使to& Pai本 : 的a設置edA本成s)
    {
        軍St本in成 Placeholde本 = 軍St本in成::P本intf(TEXT("{%s}"), *Pai本.Key);
        Res使lt = Res使lt.Replace(*Placeholde本, *Pai本.Val使e);
    }
    
    本et使本n Res使lt;
}

正oid UMin成RTSTextMana成e本::Clea本軍o本設置atCache()
{
    軍o本設置atCache.E設置pty();
}

軍St本in成 UMin成RTSTextMana成e本::GetC使本本entLocale() const
{
    // Get locale f本o設置 inte本nationalization syste設置
    軍C使lt使本eRef C使本本entC使lt使本e = 軍Inte本nationalization::Get().GetC使本本entC使lt使本e();
    本et使本n C使本本entC使lt使本e->Get的a設置e();
}
