#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Acto本.h"
#incl使de "Min成BlockchainSettin成s.成ene本ated.h"

/**
 * Blockchain netwo本k types
 */
UE的UM(Bl使ep本intType)
en使設置 class EBlockchain的etwo本k: 使int8 {
    Ethe本e使設置Mainnet  = 0,
    Ethe本e使設置Testnet  = 1,
    Poly成onMainnet   = 2,
    Poly成onTestnet   = 3,
    BSCMainnet       = 4,
    BSCTestnet       = 5,
    Local的etwo本k     = 6,
    C使sto設置的etwo本k    = 7
};

/**
 * Gas p本ice st本ate成ies
 */
UE的UM(Bl使ep本intType)
en使設置 class EGasP本iceSt本ate成y: 使int8 {
    Slow       = 0,
    Standa本d   = 1,
    軍ast       = 2,
    Instant    = 3,
    C使sto設置     = 4
};

/**
 * Sec使本ity le正els
 */
UE的UM(Bl使ep本intType)
en使設置 class ESec使本ityLe正el: 使int8 {
    Low        = 0,
    Medi使設置     = 1,
    輸入i成h       = 2,
    Maxi設置使設置    = 3
};

/**
 * Blockchain confi成使本ation settin成s
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍BlockchainConfi成
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "的etwo本k")
    EBlockchain的etwo本k Defa使lt的etwo本k;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "的etwo本k")
    軍St本in成 C使sto設置的etwo本kRPC;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "的etwo本k")
    int32 Defa使ltChainID;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "Gas")
    EGasP本iceSt本ate成y GasP本iceSt本ate成y;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "Gas")
    float C使sto設置GasP本ice;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "Gas")
    float MaxGasP本ice;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "Sec使本ity")
    ESec使本ityLe正el Sec使本ityLe正el;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "Minin成")
    bool bMinin成Enabled;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "Minin成")
    float Defa使ltMinin成Diffic使lty;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "Minin成")
    int32 MaxConc使本本entMinin成Sessions;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "PlayToEa本n")
    bool bPlayToEa本nEnabled;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "PlayToEa本n")
    float GlobalRewa本dM使ltiplie本;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "PlayToEa本n")
    int32 MaxE正entsPe本Day;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "的軍T")
    bool b的軍TSyste設置Enabled;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "的軍T")
    float Ma本ketplace軍eePe本centa成e;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "的軍T")
    int32 MaxMa本ketplaceListin成s;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "T本ansaction")
    bool bA使toRet本yEnabled;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "T本ansaction")
    int32 MaxRet本yAtte設置pts;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "T本ansaction")
    float T本ansactionTi設置eo使t;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "基本allet")
    bool bA使toConnect基本allet;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "基本allet")
    軍St本in成 Defa使lt基本alletP本o正ide本;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "Deb使成")
    bool bDeb使成Mode;

    UPROPERTY(Bl使ep本intRead基本本ite, EditAnywhe本e, Cate成o本y = "Deb使成")
    bool bEnableVe本boseLo成成in成;

    軍BlockchainConfi成()
    {
        Defa使lt的etwo本k = EBlockchain的etwo本k::Ethe本e使設置Mainnet;
        C使sto設置的etwo本kRPC = TEXT("http://localhost:8545");
        Defa使ltChainID = 1;
        GasP本iceSt本ate成y = EGasP本iceSt本ate成y::Standa本d;
        C使sto設置GasP本ice = 20.0f;
        MaxGasP本ice = 1000.0f;
        Sec使本ityLe正el = ESec使本ityLe正el::Medi使設置;
        bMinin成Enabled = t本使e;
        Defa使ltMinin成Diffic使lty = 1.0f;
        MaxConc使本本entMinin成Sessions = 1;
        bPlayToEa本nEnabled = t本使e;
        GlobalRewa本dM使ltiplie本 = 1.0f;
        MaxE正entsPe本Day = 100;
        b的軍TSyste設置Enabled = t本使e;
        Ma本ketplace軍eePe本centa成e = 0.025f;
        MaxMa本ketplaceListin成s = 1000;
        bA使toRet本yEnabled = t本使e;
        MaxRet本yAtte設置pts = 3;
        T本ansactionTi設置eo使t = 300.0f;
        bA使toConnect基本allet = false;
        Defa使lt基本alletP本o正ide本 = TEXT("MetaMask");
        bDeb使成Mode = false;
        bEnableVe本boseLo成成in成 = false;
    }
};

/**
 * 的etwo本k confi成使本ation
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍的etwo本kConfi成
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    EBlockchain的etwo本k 的etwo本kType;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 的etwo本k的a設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 RPCURL;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 ChainID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 C使本本encySy設置bol;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 BlockExplo本e本URL;

    UPROPERTY(Bl使ep本intReadOnly)
    bool IsTestnet;

    軍的etwo本kConfi成()
    {
        的etwo本kType = EBlockchain的etwo本k::Ethe本e使設置Mainnet;
        的etwo本k的a設置e = TEXT("");
        RPCURL = TEXT("");
        ChainID = 1;
        C使本本encySy設置bol = TEXT("ET輸入");
        BlockExplo本e本URL = TEXT("");
        IsTestnet = false;
    }
};

/**
 * Min成BlockchainSettin成s
 * 
 * Cent本al confi成使本ation syste設置 fo本 all blockchain-本elated settin成s in Min成GoRTS.
 * Mana成es netwo本k confi成使本ation, sec使本ity settin成s, 設置inin成 pa本a設置ete本s, and 使se本 p本efe本ences.
 */
UCLASS(Bl使ep本intType, Bl使ep本intable, ClassG本o使p=(Min成Blockchain), 設置eta=(Bl使ep本intSpawnableCo設置ponent))
class MI的GBLOCKC輸入AI的下API UMin成BlockchainSettin成s : p使blic UActo本Co設置ponent
{
    GE的ERATED下BODY()

p使blic:
    UMin成BlockchainSettin成s();

    /**
     * Initialize blockchain settin成s
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    正oid InitializeSettin成s();

    /**
     * Get c使本本ent blockchain confi成使本ation
     * @本et使本n C使本本ent confi成使本ation
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    軍BlockchainConfi成 GetBlockchainConfi成() const;

    /**
     * Update blockchain confi成使本ation
     * @pa本a設置 的ewConfi成 - 的ew confi成使本ation
     * @本et使本n T本使e if 使pdated s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    bool UpdateBlockchainConfi成(const 軍BlockchainConfi成& 的ewConfi成);

    /**
     * Get netwo本k confi成使本ation
     * @pa本a設置 的etwo本k - 的etwo本k type
     * @本et使本n 的etwo本k confi成使本ation
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    軍的etwo本kConfi成 Get的etwo本kConfi成(EBlockchain的etwo本k 的etwo本k) const;

    /**
     * Set defa使lt netwo本k
     * @pa本a設置 的etwo本k - Defa使lt netwo本k
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    正oid SetDefa使lt的etwo本k(EBlockchain的etwo本k 的etwo本k);

    /**
     * Get c使本本ent netwo本k
     * @本et使本n C使本本ent netwo本k
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    EBlockchain的etwo本k GetC使本本ent的etwo本k() const;

    /**
     * Set 成as p本ice st本ate成y
     * @pa本a設置 St本ate成y - Gas p本ice st本ate成y
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    正oid SetGasP本iceSt本ate成y(EGasP本iceSt本ate成y St本ate成y);

    /**
     * Get 成as p本ice st本ate成y
     * @本et使本n C使本本ent 成as p本ice st本ate成y
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    EGasP本iceSt本ate成y GetGasP本iceSt本ate成y() const;

    /**
     * Set sec使本ity le正el
     * @pa本a設置 Le正el - Sec使本ity le正el
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    正oid SetSec使本ityLe正el(ESec使本ityLe正el Le正el);

    /**
     * Get sec使本ity le正el
     * @本et使本n C使本本ent sec使本ity le正el
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    ESec使本ityLe正el GetSec使本ityLe正el() const;

    /**
     * Enable/disable 設置inin成
     * @pa本a設置 bEnabled - 基本hethe本 to enable 設置inin成
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    正oid SetMinin成Enabled(bool bEnabled);

    /**
     * Check if 設置inin成 is enabled
     * @本et使本n T本使e if 設置inin成 is enabled
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    bool IsMinin成Enabled() const;

    /**
     * Enable/disable play-to-ea本n
     * @pa本a設置 bEnabled - 基本hethe本 to enable play-to-ea本n
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    正oid SetPlayToEa本nEnabled(bool bEnabled);

    /**
     * Check if play-to-ea本n is enabled
     * @本et使本n T本使e if play-to-ea本n is enabled
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    bool IsPlayToEa本nEnabled() const;

    /**
     * Enable/disable 的軍T syste設置
     * @pa本a設置 bEnabled - 基本hethe本 to enable 的軍T syste設置
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    正oid Set的軍TSyste設置Enabled(bool bEnabled);

    /**
     * Check if 的軍T syste設置 is enabled
     * @本et使本n T本使e if 的軍T syste設置 is enabled
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    bool Is的軍TSyste設置Enabled() const;

    /**
     * Get s使ppo本ted netwo本ks
     * @本et使本n A本本ay of s使ppo本ted netwo本ks
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    TA本本ay<EBlockchain的etwo本k> GetS使ppo本ted的etwo本ks() const;

    /**
     * Get netwo本k na設置e
     * @pa本a設置 的etwo本k - 的etwo本k type
     * @本et使本n 的etwo本k na設置e
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    軍St本in成 Get的etwo本k的a設置e(EBlockchain的etwo本k 的etwo本k) const;

    /**
     * Get netwo本k RPC URL
     * @pa本a設置 的etwo本k - 的etwo本k type
     * @本et使本n RPC URL
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    軍St本in成 Get的etwo本kRPCURL(EBlockchain的etwo本k 的etwo本k) const;

    /**
     * Validate confi成使本ation
     * @pa本a設置 Confi成 - Confi成使本ation to 正alidate
     * @本et使本n T本使e if 正alid
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    bool ValidateConfi成(const 軍BlockchainConfi成& Confi成) const;

    /**
     * Reset to defa使lt settin成s
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    正oid ResetToDefa使lts();

    /**
     * Sa正e settin成s to file
     * @本et使本n T本使e if sa正ed s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    bool Sa正eSettin成s();

    /**
     * Load settin成s f本o設置 file
     * @本et使本n T本使e if loaded s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    bool LoadSettin成s();

    /**
     * Expo本t settin成s to JSO的
     * @本et使本n JSO的 st本in成
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    軍St本in成 Expo本tSettin成s() const;

    /**
     * I設置po本t settin成s f本o設置 JSO的
     * @pa本a設置 JSO的St本in成 - JSO的 st本in成
     * @本et使本n T本使e if i設置po本ted s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    bool I設置po本tSettin成s(const 軍St本in成& JSO的St本in成);

    /**
     * Get settin成s file path
     * @本et使本n 軍ile path
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    軍St本in成 GetSettin成s軍ilePath() const;

    /**
     * Check if deb使成 設置ode is enabled
     * @本et使本n T本使e if deb使成 設置ode is enabled
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    bool IsDeb使成ModeEnabled() const;

    /**
     * Enable/disable deb使成 設置ode
     * @pa本a設置 bEnabled - 基本hethe本 to enable deb使成 設置ode
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    正oid SetDeb使成ModeEnabled(bool bEnabled);

    /**
     * Get c使本本ent confi成使本ation 正e本sion
     * @本et使本n Confi成使本ation 正e本sion
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainSettin成s")
    軍St本in成 GetConfi成Ve本sion() const;

p本otected:
    /** Called when the co設置ponent be成ins play */
    正i本t使al 正oid Be成inPlay() o正e本本ide;

    /** Called when the co設置ponent ends play */
    正i本t使al 正oid EndPlay(const EEndPlayReason::Type EndPlayReason) o正e本本ide;

p本i正ate:
    /** C使本本ent blockchain confi成使本ation */
    UPROPERTY()
    軍BlockchainConfi成 C使本本entConfi成;

    /** 的etwo本k confi成使本ations */
    UPROPERTY()
    TMap<EBlockchain的etwo本k, 軍的etwo本kConfi成> 的etwo本kConfi成s;

    /** Confi成使本ation 正e本sion */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "Settin成s")
    軍St本in成 Confi成Ve本sion;

    /** Settin成s file na設置e */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "Settin成s")
    軍St本in成 Settin成s軍ile的a設置e;

    /** 基本hethe本 settin成s ha正e been 設置odified */
    bool bSettin成sModified;

    /** Initialize netwo本k confi成使本ations */
    正oid Initialize的etwo本kConfi成s();

    /** Get defa使lt confi成使本ation */
    軍BlockchainConfi成 GetDefa使ltConfi成() const;

    /** Validate netwo本k confi成使本ation */
    bool Validate的etwo本kConfi成(const 軍的etwo本kConfi成& Confi成) const;

    /** Apply confi成使本ation chan成es */
    正oid ApplyConfi成使本ationChan成es();

    /** 的otify confi成使本ation chan成ed */
    正oid 的otifyConfi成使本ationChan成ed();

    /** Sa正e confi成使本ation to file */
    bool Sa正eConfi成使本ationTo軍ile();

    /** Load confi成使本ation f本o設置 file */
    bool LoadConfi成使本ation軍本o設置軍ile();

    /** Con正e本t confi成使本ation to JSO的 */
    軍St本in成 Confi成ToJSO的(const 軍BlockchainConfi成& Confi成) const;

    /** Pa本se confi成使本ation f本o設置 JSO的 */
    軍BlockchainConfi成 Confi成軍本o設置JSO的(const 軍St本in成& JSO的St本in成) const;

    /** Get netwo本k confi成使本ation key */
    軍St本in成 Get的etwo本kConfi成Key(EBlockchain的etwo本k 的etwo本k) const;

    /** Validate RPC URL */
    bool ValidateRPCURL(const 軍St本in成& URL) const;

    /** Get sec使本ity le正el 本eq使i本e設置ents */
    TA本本ay<軍St本in成> GetSec使本ityLe正elReq使i本e設置ents(ESec使本ityLe正el Le正el) const;

    /** Check if confi成使本ation 設置eets sec使本ity 本eq使i本e設置ents */
    bool MeetsSec使本ityReq使i本e設置ents(const 軍BlockchainConfi成& Confi成, ESec使本ityLe正el Le正el) const;
};
