#incl使de "Min成BlockchainSettin成s.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"
#incl使de "Misc/Paths.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"

UMin成BlockchainSettin成s::UMin成BlockchainSettin成s()
{
    Confi成Ve本sion = TEXT("1.0.0");
    Settin成s軍ile的a設置e = TEXT("BlockchainSettin成s.大son");
    bSettin成sModified = false;
}

正oid UMin成BlockchainSettin成s::Be成inPlay()
{
    S使pe本::Be成inPlay();
    InitializeSettin成s();
}

正oid UMin成BlockchainSettin成s::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (bSettin成sModified)
    {
        Sa正eSettin成s();
    }
    
    S使pe本::EndPlay(EndPlayReason);
}

正oid UMin成BlockchainSettin成s::InitializeSettin成s()
{
    Initialize的etwo本kConfi成s();
    
    if (!LoadSettin成s())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainSettin成s: Co使ld not load settin成s, 使sin成 defa使lts"));
        C使本本entConfi成 = GetDefa使ltConfi成();
        Sa正eSettin成s();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Settin成s initialized"));
}

軍BlockchainConfi成 UMin成BlockchainSettin成s::GetBlockchainConfi成() const
{
    本et使本n C使本本entConfi成;
}

bool UMin成BlockchainSettin成s::UpdateBlockchainConfi成(const 軍BlockchainConfi成& 的ewConfi成)
{
    if (!ValidateConfi成(的ewConfi成))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: In正alid confi成使本ation"));
        本et使本n false;
    }

    C使本本entConfi成 = 的ewConfi成;
    bSettin成sModified = t本使e;
    ApplyConfi成使本ationChan成es();
    的otifyConfi成使本ationChan成ed();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Confi成使本ation 使pdated"));
    本et使本n t本使e;
}

軍的etwo本kConfi成 UMin成BlockchainSettin成s::Get的etwo本kConfi成(EBlockchain的etwo本k 的etwo本k) const
{
    if (const 軍的etwo本kConfi成* Confi成 = 的etwo本kConfi成s.軍ind(的etwo本k))
    {
        本et使本n *Confi成;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainSettin成s: 的etwo本k %d not fo使nd"), (int32)的etwo本k);
    本et使本n 軍的etwo本kConfi成();
}

正oid UMin成BlockchainSettin成s::SetDefa使lt的etwo本k(EBlockchain的etwo本k 的etwo本k)
{
    C使本本entConfi成.Defa使lt的etwo本k = 的etwo本k;
    bSettin成sModified = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Defa使lt netwo本k set to %d"), (int32)的etwo本k);
}

EBlockchain的etwo本k UMin成BlockchainSettin成s::GetC使本本ent的etwo本k() const
{
    本et使本n C使本本entConfi成.Defa使lt的etwo本k;
}

正oid UMin成BlockchainSettin成s::SetGasP本iceSt本ate成y(EGasP本iceSt本ate成y St本ate成y)
{
    C使本本entConfi成.GasP本iceSt本ate成y = St本ate成y;
    bSettin成sModified = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Gas p本ice st本ate成y set to %d"), (int32)St本ate成y);
}

EGasP本iceSt本ate成y UMin成BlockchainSettin成s::GetGasP本iceSt本ate成y() const
{
    本et使本n C使本本entConfi成.GasP本iceSt本ate成y;
}

正oid UMin成BlockchainSettin成s::SetSec使本ityLe正el(ESec使本ityLe正el Le正el)
{
    C使本本entConfi成.Sec使本ityLe正el = Le正el;
    bSettin成sModified = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Sec使本ity le正el set to %d"), (int32)Le正el);
}

ESec使本ityLe正el UMin成BlockchainSettin成s::GetSec使本ityLe正el() const
{
    本et使本n C使本本entConfi成.Sec使本ityLe正el;
}

正oid UMin成BlockchainSettin成s::SetMinin成Enabled(bool bEnabled)
{
    C使本本entConfi成.bMinin成Enabled = bEnabled;
    bSettin成sModified = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Minin成 %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

bool UMin成BlockchainSettin成s::IsMinin成Enabled() const
{
    本et使本n C使本本entConfi成.bMinin成Enabled;
}

正oid UMin成BlockchainSettin成s::SetPlayToEa本nEnabled(bool bEnabled)
{
    C使本本entConfi成.bPlayToEa本nEnabled = bEnabled;
    bSettin成sModified = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Play-to-ea本n %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

bool UMin成BlockchainSettin成s::IsPlayToEa本nEnabled() const
{
    本et使本n C使本本entConfi成.bPlayToEa本nEnabled;
}

正oid UMin成BlockchainSettin成s::Set的軍TSyste設置Enabled(bool bEnabled)
{
    C使本本entConfi成.b的軍TSyste設置Enabled = bEnabled;
    bSettin成sModified = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: 的軍T syste設置 %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

bool UMin成BlockchainSettin成s::Is的軍TSyste設置Enabled() const
{
    本et使本n C使本本entConfi成.b的軍TSyste設置Enabled;
}

TA本本ay<EBlockchain的etwo本k> UMin成BlockchainSettin成s::GetS使ppo本ted的etwo本ks() const
{
    TA本本ay<EBlockchain的etwo本k> 的etwo本ks;
    的etwo本ks.Add(EBlockchain的etwo本k::Ethe本e使設置Mainnet);
    的etwo本ks.Add(EBlockchain的etwo本k::Ethe本e使設置Testnet);
    的etwo本ks.Add(EBlockchain的etwo本k::Poly成onMainnet);
    的etwo本ks.Add(EBlockchain的etwo本k::Poly成onTestnet);
    的etwo本ks.Add(EBlockchain的etwo本k::BSCMainnet);
    的etwo本ks.Add(EBlockchain的etwo本k::BSCTestnet);
    的etwo本ks.Add(EBlockchain的etwo本k::Local的etwo本k);
    的etwo本ks.Add(EBlockchain的etwo本k::C使sto設置的etwo本k);
    
    本et使本n 的etwo本ks;
}

軍St本in成 UMin成BlockchainSettin成s::Get的etwo本k的a設置e(EBlockchain的etwo本k 的etwo本k) const
{
    軍的etwo本kConfi成 Confi成 = Get的etwo本kConfi成(的etwo本k);
    本et使本n Confi成.的etwo本k的a設置e;
}

軍St本in成 UMin成BlockchainSettin成s::Get的etwo本kRPCURL(EBlockchain的etwo本k 的etwo本k) const
{
    軍的etwo本kConfi成 Confi成 = Get的etwo本kConfi成(的etwo本k);
    本et使本n Confi成.RPCURL;
}

bool UMin成BlockchainSettin成s::ValidateConfi成(const 軍BlockchainConfi成& Confi成) const
{
    // Validate netwo本k confi成使本ation
    if (!Validate的etwo本kConfi成(Get的etwo本kConfi成(Confi成.Defa使lt的etwo本k)))
    {
        本et使本n false;
    }

    // Validate 成as settin成s
    if (Confi成.C使sto設置GasP本ice <= 0.0f  Confi成.MaxGasP本ice <= 0.0f)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: In正alid 成as p本ices"));
        本et使本n false;
    }

    if (Confi成.C使sto設置GasP本ice > Confi成.MaxGasP本ice)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: C使sto設置 成as p本ice exceeds 設置axi設置使設置"));
        本et使本n false;
    }

    // Validate 設置inin成 settin成s
    if (Confi成.MaxConc使本本entMinin成Sessions <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: In正alid 設置ax conc使本本ent 設置inin成 sessions"));
        本et使本n false;
    }

    // Validate play-to-ea本n settin成s
    if (Confi成.MaxE正entsPe本Day <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: In正alid 設置ax e正ents pe本 day"));
        本et使本n false;
    }

    // Validate 的軍T settin成s
    if (Confi成.Ma本ketplace軍eePe本centa成e < 0.0f  Confi成.Ma本ketplace軍eePe本centa成e > 1.0f)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: In正alid 設置a本ketplace fee pe本centa成e"));
        本et使本n false;
    }

    // Validate t本ansaction settin成s
    if (Confi成.MaxRet本yAtte設置pts < 0  Confi成.T本ansactionTi設置eo使t <= 0.0f)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: In正alid t本ansaction settin成s"));
        本et使本n false;
    }

    // Check sec使本ity 本eq使i本e設置ents
    if (!MeetsSec使本ityReq使i本e設置ents(Confi成, Confi成.Sec使本ityLe正el))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: Confi成使本ation does not 設置eet sec使本ity 本eq使i本e設置ents"));
        本et使本n false;
    }

    本et使本n t本使e;
}

正oid UMin成BlockchainSettin成s::ResetToDefa使lts()
{
    C使本本entConfi成 = GetDefa使ltConfi成();
    bSettin成sModified = t本使e;
    ApplyConfi成使本ationChan成es();
    的otifyConfi成使本ationChan成ed();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Reset to defa使lt settin成s"));
}

bool UMin成BlockchainSettin成s::Sa正eSettin成s()
{
    if (Sa正eConfi成使本ationTo軍ile())
    {
        bSettin成sModified = false;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Settin成s sa正ed s使ccessf使lly"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: 軍ailed to sa正e settin成s"));
    本et使本n false;
}

bool UMin成BlockchainSettin成s::LoadSettin成s()
{
    if (LoadConfi成使本ation軍本o設置軍ile())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Settin成s loaded s使ccessf使lly"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainSettin成s: Co使ld not load settin成s"));
    本et使本n false;
}

軍St本in成 UMin成BlockchainSettin成s::Expo本tSettin成s() const
{
    本et使本n Confi成ToJSO的(C使本本entConfi成);
}

bool UMin成BlockchainSettin成s::I設置po本tSettin成s(const 軍St本in成& JSO的St本in成)
{
    軍BlockchainConfi成 I設置po本tedConfi成 = Confi成軍本o設置JSO的(JSO的St本in成);
    
    if (ValidateConfi成(I設置po本tedConfi成))
    {
        C使本本entConfi成 = I設置po本tedConfi成;
        bSettin成sModified = t本使e;
        ApplyConfi成使本ationChan成es();
        的otifyConfi成使本ationChan成ed();

        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Settin成s i設置po本ted s使ccessf使lly"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: 軍ailed to i設置po本t settin成s - in正alid confi成使本ation"));
    本et使本n false;
}

軍St本in成 UMin成BlockchainSettin成s::GetSettin成s軍ilePath() const
{
    本et使本n 軍Paths::P本o大ectSa正edDi本() / Settin成s軍ile的a設置e;
}

bool UMin成BlockchainSettin成s::IsDeb使成ModeEnabled() const
{
    本et使本n C使本本entConfi成.bDeb使成Mode;
}

正oid UMin成BlockchainSettin成s::SetDeb使成ModeEnabled(bool bEnabled)
{
    C使本本entConfi成.bDeb使成Mode = bEnabled;
    C使本本entConfi成.bEnableVe本boseLo成成in成 = bEnabled;
    bSettin成sModified = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Deb使成 設置ode %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

軍St本in成 UMin成BlockchainSettin成s::GetConfi成Ve本sion() const
{
    本et使本n Confi成Ve本sion;
}

正oid UMin成BlockchainSettin成s::Initialize的etwo本kConfi成s()
{
    // Ethe本e使設置 Mainnet
    軍的etwo本kConfi成 Ethe本e使設置Mainnet;
    Ethe本e使設置Mainnet.的etwo本kType = EBlockchain的etwo本k::Ethe本e使設置Mainnet;
    Ethe本e使設置Mainnet.的etwo本k的a設置e = TEXT("Ethe本e使設置 Mainnet");
    Ethe本e使設置Mainnet.RPCURL = TEXT("https://設置ainnet.inf使本a.io/正3/YOUR下PROJECT下ID");
    Ethe本e使設置Mainnet.ChainID = 1;
    Ethe本e使設置Mainnet.C使本本encySy設置bol = TEXT("ET輸入");
    Ethe本e使設置Mainnet.BlockExplo本e本URL = TEXT("https://ethe本scan.io");
    Ethe本e使設置Mainnet.IsTestnet = false;
    的etwo本kConfi成s.Add(EBlockchain的etwo本k::Ethe本e使設置Mainnet, Ethe本e使設置Mainnet);

    // Ethe本e使設置 Testnet (Goe本li)
    軍的etwo本kConfi成 Ethe本e使設置Testnet;
    Ethe本e使設置Testnet.的etwo本kType = EBlockchain的etwo本k::Ethe本e使設置Testnet;
    Ethe本e使設置Testnet.的etwo本k的a設置e = TEXT("Ethe本e使設置 Testnet");
    Ethe本e使設置Testnet.RPCURL = TEXT("https://成oe本li.inf使本a.io/正3/YOUR下PROJECT下ID");
    Ethe本e使設置Testnet.ChainID = 5;
    Ethe本e使設置Testnet.C使本本encySy設置bol = TEXT("ET輸入");
    Ethe本e使設置Testnet.BlockExplo本e本URL = TEXT("https://成oe本li.ethe本scan.io");
    Ethe本e使設置Testnet.IsTestnet = t本使e;
    的etwo本kConfi成s.Add(EBlockchain的etwo本k::Ethe本e使設置Testnet, Ethe本e使設置Testnet);

    // Poly成on Mainnet
    軍的etwo本kConfi成 Poly成onMainnet;
    Poly成onMainnet.的etwo本kType = EBlockchain的etwo本k::Poly成onMainnet;
    Poly成onMainnet.的etwo本k的a設置e = TEXT("Poly成on Mainnet");
    Poly成onMainnet.RPCURL = TEXT("https://poly成on-本pc.co設置");
    Poly成onMainnet.ChainID = 137;
    Poly成onMainnet.C使本本encySy設置bol = TEXT("MATIC");
    Poly成onMainnet.BlockExplo本e本URL = TEXT("https://poly成onscan.co設置");
    Poly成onMainnet.IsTestnet = false;
    的etwo本kConfi成s.Add(EBlockchain的etwo本k::Poly成onMainnet, Poly成onMainnet);

    // Poly成on Testnet (M使設置bai)
    軍的etwo本kConfi成 Poly成onTestnet;
    Poly成onTestnet.的etwo本kType = EBlockchain的etwo本k::Poly成onTestnet;
    Poly成onTestnet.的etwo本k的a設置e = TEXT("Poly成on Testnet");
    Poly成onTestnet.RPCURL = TEXT("https://本pc-設置使設置bai.設置atic正i成il.co設置");
    Poly成onTestnet.ChainID = 80001;
    Poly成onTestnet.C使本本encySy設置bol = TEXT("MATIC");
    Poly成onTestnet.BlockExplo本e本URL = TEXT("https://設置使設置bai.poly成onscan.co設置");
    Poly成onTestnet.IsTestnet = t本使e;
    的etwo本kConfi成s.Add(EBlockchain的etwo本k::Poly成onTestnet, Poly成onTestnet);

    // BSC Mainnet
    軍的etwo本kConfi成 BSCMainnet;
    BSCMainnet.的etwo本kType = EBlockchain的etwo本k::BSCMainnet;
    BSCMainnet.的etwo本k的a設置e = TEXT("BSC Mainnet");
    BSCMainnet.RPCURL = TEXT("https://bsc-dataseed.binance.o本成");
    BSCMainnet.ChainID = 56;
    BSCMainnet.C使本本encySy設置bol = TEXT("B的B");
    BSCMainnet.BlockExplo本e本URL = TEXT("https://bscscan.co設置");
    BSCMainnet.IsTestnet = false;
    的etwo本kConfi成s.Add(EBlockchain的etwo本k::BSCMainnet, BSCMainnet);

    // BSC Testnet
    軍的etwo本kConfi成 BSCTestnet;
    BSCTestnet.的etwo本kType = EBlockchain的etwo本k::BSCTestnet;
    BSCTestnet.的etwo本k的a設置e = TEXT("BSC Testnet");
    BSCTestnet.RPCURL = TEXT("https://data-seed-p本ebsc-1-s1.binance.o本成:8545");
    BSCTestnet.ChainID = 97;
    BSCTestnet.C使本本encySy設置bol = TEXT("B的B");
    BSCTestnet.BlockExplo本e本URL = TEXT("https://testnet.bscscan.co設置");
    BSCTestnet.IsTestnet = t本使e;
    的etwo本kConfi成s.Add(EBlockchain的etwo本k::BSCTestnet, BSCTestnet);

    // Local 的etwo本k
    軍的etwo本kConfi成 Local的etwo本k;
    Local的etwo本k.的etwo本kType = EBlockchain的etwo本k::Local的etwo本k;
    Local的etwo本k.的etwo本k的a設置e = TEXT("Local 的etwo本k");
    Local的etwo本k.RPCURL = TEXT("http://localhost:8545");
    Local的etwo本k.ChainID = 1337;
    Local的etwo本k.C使本本encySy設置bol = TEXT("ET輸入");
    Local的etwo本k.BlockExplo本e本URL = TEXT("");
    Local的etwo本k.IsTestnet = t本使e;
    的etwo本kConfi成s.Add(EBlockchain的etwo本k::Local的etwo本k, Local的etwo本k);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: 的etwo本k confi成使本ations initialized"));
}

軍BlockchainConfi成 UMin成BlockchainSettin成s::GetDefa使ltConfi成() const
{
    軍BlockchainConfi成 Defa使ltConfi成;
    Defa使ltConfi成.Defa使lt的etwo本k = EBlockchain的etwo本k::Ethe本e使設置Mainnet;
    Defa使ltConfi成.C使sto設置的etwo本kRPC = TEXT("http://localhost:8545");
    Defa使ltConfi成.Defa使ltChainID = 1;
    Defa使ltConfi成.GasP本iceSt本ate成y = EGasP本iceSt本ate成y::Standa本d;
    Defa使ltConfi成.C使sto設置GasP本ice = 20.0f;
    Defa使ltConfi成.MaxGasP本ice = 1000.0f;
    Defa使ltConfi成.Sec使本ityLe正el = ESec使本ityLe正el::Medi使設置;
    Defa使ltConfi成.bMinin成Enabled = t本使e;
    Defa使ltConfi成.Defa使ltMinin成Diffic使lty = 1.0f;
    Defa使ltConfi成.MaxConc使本本entMinin成Sessions = 1;
    Defa使ltConfi成.bPlayToEa本nEnabled = t本使e;
    Defa使ltConfi成.GlobalRewa本dM使ltiplie本 = 1.0f;
    Defa使ltConfi成.MaxE正entsPe本Day = 100;
    Defa使ltConfi成.b的軍TSyste設置Enabled = t本使e;
    Defa使ltConfi成.Ma本ketplace軍eePe本centa成e = 0.025f;
    Defa使ltConfi成.MaxMa本ketplaceListin成s = 1000;
    Defa使ltConfi成.bA使toRet本yEnabled = t本使e;
    Defa使ltConfi成.MaxRet本yAtte設置pts = 3;
    Defa使ltConfi成.T本ansactionTi設置eo使t = 300.0f;
    Defa使ltConfi成.bA使toConnect基本allet = false;
    Defa使ltConfi成.Defa使lt基本alletP本o正ide本 = TEXT("MetaMask");
    Defa使ltConfi成.bDeb使成Mode = false;
    Defa使ltConfi成.bEnableVe本boseLo成成in成 = false;

    本et使本n Defa使ltConfi成;
}

bool UMin成BlockchainSettin成s::Validate的etwo本kConfi成(const 軍的etwo本kConfi成& Confi成) const
{
    本et使本n !Confi成.的etwo本k的a設置e.IsE設置pty() &&
           !Confi成.RPCURL.IsE設置pty() &&
           ValidateRPCURL(Confi成.RPCURL) &&
           Confi成.ChainID > 0 &&
           !Confi成.C使本本encySy設置bol.IsE設置pty();
}

正oid UMin成BlockchainSettin成s::ApplyConfi成使本ationChan成es()
{
    // Apply confi成使本ation chan成es to all blockchain syste設置s
    // This wo使ld typically b本oadcast confi成使本ation chan成e e正ents
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Applied confi成使本ation chan成es"));
}

正oid UMin成BlockchainSettin成s::的otifyConfi成使本ationChan成ed()
{
    // 的otify othe本 syste設置s that confi成使本ation has chan成ed
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Confi成使本ation chan成ed notification sent"));
}

bool UMin成BlockchainSettin成s::Sa正eConfi成使本ationTo軍ile()
{
    軍St本in成 軍ilePath = GetSettin成s軍ilePath();
    軍St本in成 JSO的St本in成 = Confi成ToJSO的(C使本本entConfi成);

    if (!軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JSO的St本in成, *軍ilePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成BlockchainSettin成s: 軍ailed to sa正e settin成s to %s"), *軍ilePath);
        本et使本n false;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Settin成s sa正ed to %s"), *軍ilePath);
    本et使本n t本使e;
}

bool UMin成BlockchainSettin成s::LoadConfi成使本ation軍本o設置軍ile()
{
    軍St本in成 軍ilePath = GetSettin成s軍ilePath();
    軍St本in成 JSO的St本in成;

    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JSO的St本in成, *軍ilePath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainSettin成s: Co使ld not load settin成s f本o設置 %s"), *軍ilePath);
        本et使本n false;
    }

    C使本本entConfi成 = Confi成軍本o設置JSO的(JSO的St本in成);
    
    // Validate loaded confi成使本ation
    if (!ValidateConfi成(C使本本entConfi成))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainSettin成s: Loaded confi成使本ation is in正alid, 使sin成 defa使lts"));
        C使本本entConfi成 = GetDefa使ltConfi成();
        本et使本n false;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainSettin成s: Settin成s loaded f本o設置 %s"), *軍ilePath);
    本et使本n t本使e;
}

軍St本in成 UMin成BlockchainSettin成s::Confi成ToJSO的(const 軍BlockchainConfi成& Confi成) const
{
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect = MakeSha本eable(new 軍JsonOb大ect);
    
    JsonOb大ect->SetSt本in成軍ield(TEXT("正e本sion"), Confi成Ve本sion);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("defa使lt的etwo本k"), (int32)Confi成.Defa使lt的etwo本k);
    JsonOb大ect->SetSt本in成軍ield(TEXT("c使sto設置的etwo本kRPC"), Confi成.C使sto設置的etwo本kRPC);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("defa使ltChainID"), Confi成.Defa使ltChainID);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("成asP本iceSt本ate成y"), (int32)Confi成.GasP本iceSt本ate成y);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("c使sto設置GasP本ice"), Confi成.C使sto設置GasP本ice);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("設置axGasP本ice"), Confi成.MaxGasP本ice);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("sec使本ityLe正el"), (int32)Confi成.Sec使本ityLe正el);
    JsonOb大ect->SetBool軍ield(TEXT("設置inin成Enabled"), Confi成.bMinin成Enabled);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("defa使ltMinin成Diffic使lty"), Confi成.Defa使ltMinin成Diffic使lty);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("設置axConc使本本entMinin成Sessions"), Confi成.MaxConc使本本entMinin成Sessions);
    JsonOb大ect->SetBool軍ield(TEXT("playToEa本nEnabled"), Confi成.bPlayToEa本nEnabled);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("成lobalRewa本dM使ltiplie本"), Confi成.GlobalRewa本dM使ltiplie本);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("設置axE正entsPe本Day"), Confi成.MaxE正entsPe本Day);
    JsonOb大ect->SetBool軍ield(TEXT("nftSyste設置Enabled"), Confi成.b的軍TSyste設置Enabled);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("設置a本ketplace軍eePe本centa成e"), Confi成.Ma本ketplace軍eePe本centa成e);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("設置axMa本ketplaceListin成s"), Confi成.MaxMa本ketplaceListin成s);
    JsonOb大ect->SetBool軍ield(TEXT("a使toRet本yEnabled"), Confi成.bA使toRet本yEnabled);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("設置axRet本yAtte設置pts"), Confi成.MaxRet本yAtte設置pts);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("t本ansactionTi設置eo使t"), Confi成.T本ansactionTi設置eo使t);
    JsonOb大ect->SetBool軍ield(TEXT("a使toConnect基本allet"), Confi成.bA使toConnect基本allet);
    JsonOb大ect->SetSt本in成軍ield(TEXT("defa使lt基本alletP本o正ide本"), Confi成.Defa使lt基本alletP本o正ide本);
    JsonOb大ect->SetBool軍ield(TEXT("deb使成Mode"), Confi成.bDeb使成Mode);
    JsonOb大ect->SetBool軍ield(TEXT("enableVe本boseLo成成in成"), Confi成.bEnableVe本boseLo成成in成);

    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(JsonOb大ect.ToSha本edRef(), 基本本ite本);

    本et使本n O使tp使tSt本in成;
}

軍BlockchainConfi成 UMin成BlockchainSettin成s::Confi成軍本o設置JSO的(const 軍St本in成& JSO的St本in成) const
{
    軍BlockchainConfi成 Confi成 = GetDefa使ltConfi成();

    TSha本edPt本<軍JsonOb大ect> JsonOb大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JSO的St本in成);

    if (軍JsonSe本ialize本::Dese本ialize(Reade本, JsonOb大ect) && JsonOb大ect.IsValid())
    {
        // Check 正e本sion co設置patibility
        軍St本in成 LoadedVe本sion = JsonOb大ect->GetSt本in成軍ield(TEXT("正e本sion"));
        if (LoadedVe本sion != Confi成Ve本sion)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainSettin成s: Confi成使本ation 正e本sion 設置is設置atch (loaded: %s, expected: %s)"), 
                   *LoadedVe本sion, *Confi成Ve本sion);
        }

        // Load confi成使本ation 正al使es
        if (JsonOb大ect->輸入as軍ield(TEXT("defa使lt的etwo本k")))
            Confi成.Defa使lt的etwo本k = (EBlockchain的etwo本k)JsonOb大ect->GetInte成e本軍ield(TEXT("defa使lt的etwo本k"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("c使sto設置的etwo本kRPC")))
            Confi成.C使sto設置的etwo本kRPC = JsonOb大ect->GetSt本in成軍ield(TEXT("c使sto設置的etwo本kRPC"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("defa使ltChainID")))
            Confi成.Defa使ltChainID = JsonOb大ect->GetInte成e本軍ield(TEXT("defa使ltChainID"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("成asP本iceSt本ate成y")))
            Confi成.GasP本iceSt本ate成y = (EGasP本iceSt本ate成y)JsonOb大ect->GetInte成e本軍ield(TEXT("成asP本iceSt本ate成y"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("c使sto設置GasP本ice")))
            Confi成.C使sto設置GasP本ice = JsonOb大ect->Get的使設置be本軍ield(TEXT("c使sto設置GasP本ice"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("設置axGasP本ice")))
            Confi成.MaxGasP本ice = JsonOb大ect->Get的使設置be本軍ield(TEXT("設置axGasP本ice"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("sec使本ityLe正el")))
            Confi成.Sec使本ityLe正el = (ESec使本ityLe正el)JsonOb大ect->GetInte成e本軍ield(TEXT("sec使本ityLe正el"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("設置inin成Enabled")))
            Confi成.bMinin成Enabled = JsonOb大ect->GetBool軍ield(TEXT("設置inin成Enabled"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("defa使ltMinin成Diffic使lty")))
            Confi成.Defa使ltMinin成Diffic使lty = JsonOb大ect->Get的使設置be本軍ield(TEXT("defa使ltMinin成Diffic使lty"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("設置axConc使本本entMinin成Sessions")))
            Confi成.MaxConc使本本entMinin成Sessions = JsonOb大ect->GetInte成e本軍ield(TEXT("設置axConc使本本entMinin成Sessions"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("playToEa本nEnabled")))
            Confi成.bPlayToEa本nEnabled = JsonOb大ect->GetBool軍ield(TEXT("playToEa本nEnabled"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("成lobalRewa本dM使ltiplie本")))
            Confi成.GlobalRewa本dM使ltiplie本 = JsonOb大ect->Get的使設置be本軍ield(TEXT("成lobalRewa本dM使ltiplie本"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("設置axE正entsPe本Day")))
            Confi成.MaxE正entsPe本Day = JsonOb大ect->GetInte成e本軍ield(TEXT("設置axE正entsPe本Day"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("nftSyste設置Enabled")))
            Confi成.b的軍TSyste設置Enabled = JsonOb大ect->GetBool軍ield(TEXT("nftSyste設置Enabled"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("設置a本ketplace軍eePe本centa成e")))
            Confi成.Ma本ketplace軍eePe本centa成e = JsonOb大ect->Get的使設置be本軍ield(TEXT("設置a本ketplace軍eePe本centa成e"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("設置axMa本ketplaceListin成s")))
            Confi成.MaxMa本ketplaceListin成s = JsonOb大ect->GetInte成e本軍ield(TEXT("設置axMa本ketplaceListin成s"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("a使toRet本yEnabled")))
            Confi成.bA使toRet本yEnabled = JsonOb大ect->GetBool軍ield(TEXT("a使toRet本yEnabled"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("設置axRet本yAtte設置pts")))
            Confi成.MaxRet本yAtte設置pts = JsonOb大ect->GetInte成e本軍ield(TEXT("設置axRet本yAtte設置pts"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("t本ansactionTi設置eo使t")))
            Confi成.T本ansactionTi設置eo使t = JsonOb大ect->Get的使設置be本軍ield(TEXT("t本ansactionTi設置eo使t"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("a使toConnect基本allet")))
            Confi成.bA使toConnect基本allet = JsonOb大ect->GetBool軍ield(TEXT("a使toConnect基本allet"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("defa使lt基本alletP本o正ide本")))
            Confi成.Defa使lt基本alletP本o正ide本 = JsonOb大ect->GetSt本in成軍ield(TEXT("defa使lt基本alletP本o正ide本"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("deb使成Mode")))
            Confi成.bDeb使成Mode = JsonOb大ect->GetBool軍ield(TEXT("deb使成Mode"));
        
        if (JsonOb大ect->輸入as軍ield(TEXT("enableVe本boseLo成成in成")))
            Confi成.bEnableVe本boseLo成成in成 = JsonOb大ect->GetBool軍ield(TEXT("enableVe本boseLo成成in成"));
    }

    本et使本n Confi成;
}

軍St本in成 UMin成BlockchainSettin成s::Get的etwo本kConfi成Key(EBlockchain的etwo本k 的etwo本k) const
{
    switch (的etwo本k)
    {
        case EBlockchain的etwo本k::Ethe本e使設置Mainnet: 本et使本n TEXT("ethe本e使設置下設置ainnet");
        case EBlockchain的etwo本k::Ethe本e使設置Testnet: 本et使本n TEXT("ethe本e使設置下testnet");
        case EBlockchain的etwo本k::Poly成onMainnet:   本et使本n TEXT("poly成on下設置ainnet");
        case EBlockchain的etwo本k::Poly成onTestnet:   本et使本n TEXT("poly成on下testnet");
        case EBlockchain的etwo本k::BSCMainnet:       本et使本n TEXT("bsc下設置ainnet");
        case EBlockchain的etwo本k::BSCTestnet:       本et使本n TEXT("bsc下testnet");
        case EBlockchain的etwo本k::Local的etwo本k:     本et使本n TEXT("local下netwo本k");
        case EBlockchain的etwo本k::C使sto設置的etwo本k:    本et使本n TEXT("c使sto設置下netwo本k");
        defa使lt: 本et使本n TEXT("使nknown");
    }
}

bool UMin成BlockchainSettin成s::ValidateRPCURL(const 軍St本in成& URL) const
{
    if (URL.IsE設置pty())
    {
        本et使本n false;
    }

    // Basic URL 正alidation
    if (URL.Sta本ts基本ith(TEXT("http://"))  URL.Sta本ts基本ith(TEXT("https://"))  URL.Sta本ts基本ith(TEXT("ws://"))  URL.Sta本ts基本ith(TEXT("wss://")))
    {
        本et使本n t本使e;
    }

    本et使本n false;
}

TA本本ay<軍St本in成> UMin成BlockchainSettin成s::GetSec使本ityLe正elReq使i本e設置ents(ESec使本ityLe正el Le正el) const
{
    TA本本ay<軍St本in成> Req使i本e設置ents;

    switch (Le正el)
    {
        case ESec使本ityLe正el::Low:
            Req使i本e設置ents.Add(TEXT("Basic t本ansaction 正alidation"));
            b本eak;

        case ESec使本ityLe正el::Medi使設置:
            Req使i本e設置ents.Add(TEXT("Enhanced t本ansaction 正alidation"));
            Req使i本e設置ents.Add(TEXT("Gas p本ice li設置its"));
            Req使i本e設置ents.Add(TEXT("Basic f本a使d detection"));
            b本eak;

        case ESec使本ityLe正el::輸入i成h:
            Req使i本e設置ents.Add(TEXT("Ad正anced t本ansaction 正alidation"));
            Req使i本e設置ents.Add(TEXT("St本ict 成as p本ice li設置its"));
            Req使i本e設置ents.Add(TEXT("Enhanced f本a使d detection"));
            Req使i本e設置ents.Add(TEXT("M使lti-si成nat使本e s使ppo本t"));
            b本eak;

        case ESec使本ityLe正el::Maxi設置使設置:
            Req使i本e設置ents.Add(TEXT("Maxi設置使設置 t本ansaction 正alidation"));
            Req使i本e設置ents.Add(TEXT("Ve本y st本ict 成as p本ice li設置its"));
            Req使i本e設置ents.Add(TEXT("Ad正anced f本a使d detection"));
            Req使i本e設置ents.Add(TEXT("M使lti-si成nat使本e 本eq使i本ed"));
            Req使i本e設置ents.Add(TEXT("輸入a本dwa本e wallet 本eq使i本e設置ent"));
            b本eak;
    }

    本et使本n Req使i本e設置ents;
}

bool UMin成BlockchainSettin成s::MeetsSec使本ityReq使i本e設置ents(const 軍BlockchainConfi成& Confi成, ESec使本ityLe正el Le正el) const
{
    switch (Le正el)
    {
        case ESec使本ityLe正el::Low:
            本et使本n t本使e; // Always 設置eets low 本eq使i本e設置ents

        case ESec使本ityLe正el::Medi使設置:
            本et使本n Confi成.MaxGasP本ice <= 500.0f && Confi成.bA使toRet本yEnabled;

        case ESec使本ityLe正el::輸入i成h:
            本et使本n Confi成.MaxGasP本ice <= 200.0f && 
                   Confi成.MaxRet本yAtte設置pts <= 5 && 
                   Confi成.T本ansactionTi設置eo使t <= 600.0f;

        case ESec使本ityLe正el::Maxi設置使設置:
            本et使本n Confi成.MaxGasP本ice <= 100.0f && 
                   Confi成.MaxRet本yAtte設置pts <= 3 && 
                   Confi成.T本ansactionTi設置eo使t <= 300.0f &&
                   Confi成.Sec使本ityLe正el == ESec使本ityLe正el::Maxi設置使設置;

        defa使lt:
            本et使本n false;
    }
}
