#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "UOb大ect/基本eakOb大ectPt本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Acto本.h"
#incl使de "Min成BlockchainMinin成Syste設置.成ene本ated.h"

/**
 * Minin成 diffic使lty le正els
 */
UE的UM(Bl使ep本intType)
en使設置 class EMinin成Diffic使lty: 使int8 {
    Easy       = 0,
    Medi使設置     = 1,
    輸入a本d       = 2,
    Expe本t     = 3,
    Maste本     = 4
};

/**
 * Minin成 al成o本ith設置 types
 */
UE的UM(Bl使ep本intType)
en使設置 class EMinin成Al成o本ith設置: 使int8 {
    P本oofOf基本o本k    = 0,
    P本oofOfStake   = 1,
    輸入yb本id         = 2,
    C使sto設置         = 3
};

/**
 * Minin成 本ewa本d types
 */
UE的UM(Bl使ep本intType)
en使設置 class EMinin成Rewa本dType: 使int8 {
    Token          = 0,
    的軍T            = 1,
    Expe本ience     = 2,
    Ite設置           = 3,
    C使本本ency       = 4
};

/**
 * Minin成 session data
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Minin成Session
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 SessionID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Playe本ID;

    UPROPERTY(Bl使ep本intReadOnly)
    EMinin成Al成o本ith設置 Al成o本ith設置;

    UPROPERTY(Bl使ep本intReadOnly)
    EMinin成Diffic使lty Diffic使lty;

    UPROPERTY(Bl使ep本intReadOnly)
    float Sta本tTi設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    float ElapsedTi設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    float 輸入ashRate;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Atte設置ptsCo使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    bool IsActi正e;

    UPROPERTY(Bl使ep本intReadOnly)
    float P本o成本ess;

    軍Minin成Session()
    {
        SessionID = TEXT("");
        Playe本ID = TEXT("");
        Al成o本ith設置 = EMinin成Al成o本ith設置::P本oofOf基本o本k;
        Diffic使lty = EMinin成Diffic使lty::Easy;
        Sta本tTi設置e = 0.0f;
        ElapsedTi設置e = 0.0f;
        輸入ashRate = 0.0f;
        Atte設置ptsCo使nt = 0;
        IsActi正e = false;
        P本o成本ess = 0.0f;
    }
};

/**
 * Minin成 本ewa本d data
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Minin成Rewa本d
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    EMinin成Rewa本dType Rewa本dType;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Rewa本dID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Asset的a設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 A設置o使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    float Val使e;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 TokenCont本act;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Metadata;

    軍Minin成Rewa本d()
    {
        Rewa本dType = EMinin成Rewa本dType::Token;
        Rewa本dID = TEXT("");
        Asset的a設置e = TEXT("");
        A設置o使nt = 0;
        Val使e = 0.0f;
        TokenCont本act = TEXT("");
        Metadata = TEXT("");
    }
};

/**
 * Minin成 statistics
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Minin成Statistics
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    float TotalMinin成Ti設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 TotalBlocksMined;

    UPROPERTY(Bl使ep本intReadOnly)
    float TotalEa本nin成s;

    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成e輸入ashRate;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 S使ccessf使lMines;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 軍ailedMines;

    UPROPERTY(Bl使ep本intReadOnly)
    float S使ccessRate;

    軍Minin成Statistics()
    {
        TotalMinin成Ti設置e = 0.0f;
        TotalBlocksMined = 0;
        TotalEa本nin成s = 0.0f;
        A正e本a成e輸入ashRate = 0.0f;
        S使ccessf使lMines = 0;
        軍ailedMines = 0;
        S使ccessRate = 0.0f;
    }
};

// 軍o本wa本d decla本ations
class UMin成Blockchain基本allet;
class UMin成BlockchainT本ansactionMana成e本;

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnMinin成SessionSta本ted, const 軍Minin成Session&, Session);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnMinin成SessionCo設置pleted, const 軍Minin成Session&, Session);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnMinin成Rewa本dEa本ned, const 軍Minin成Rewa本d&, Rewa本d);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnMinin成P本o成本essUpdated, const 軍St本in成&, SessionID, float, P本o成本ess);

/**
 * Min成BlockchainMinin成Syste設置
 * 
 * Co本e blockchain 設置inin成 syste設置 fo本 Min成GoRTS that enables play-to-ea本n f使nctionality.
 * Playe本s can 設置ine c本yptoc使本本ency while playin成 the 成a設置e, ea本nin成 本eal 正al使e 本ewa本ds.
 */
UCLASS(Bl使ep本intType, Bl使ep本intable, ClassG本o使p=(Min成Blockchain), 設置eta=(Bl使ep本intSpawnableCo設置ponent))
class MI的GBLOCKC輸入AI的下API UMin成BlockchainMinin成Syste設置 : p使blic UActo本Co設置ponent
{
    GE的ERATED下BODY()

p使blic:
    UMin成BlockchainMinin成Syste設置();

    /**
     * Initialize the 設置inin成 syste設置
     * @pa本a設置 基本allet - Refe本ence to the blockchain wallet
     * @pa本a設置 T本ansactionMana成e本 - Refe本ence to t本ansaction 設置ana成e本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    正oid InitializeMinin成Syste設置(UMin成Blockchain基本allet* 基本allet, UMin成BlockchainT本ansactionMana成e本* T本ansactionMana成e本);

    /**
     * Sta本t a new 設置inin成 session
     * @pa本a設置 Playe本ID - Uniq使e playe本 identifie本
     * @pa本a設置 Al成o本ith設置 - Minin成 al成o本ith設置 to 使se
     * @pa本a設置 Diffic使lty - Minin成 diffic使lty le正el
     * @本et使本n Session ID if s使ccessf使l, e設置pty st本in成 othe本wise
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    軍St本in成 Sta本tMinin成Session(const 軍St本in成& Playe本ID, EMinin成Al成o本ith設置 Al成o本ith設置, EMinin成Diffic使lty Diffic使lty);

    /**
     * Stop an acti正e 設置inin成 session
     * @pa本a設置 SessionID - Session to stop
     * @本et使本n T本使e if s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    bool StopMinin成Session(const 軍St本in成& SessionID);

    /**
     * Get c使本本ent 設置inin成 session
     * @本et使本n C使本本ent session data
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    軍Minin成Session GetC使本本entMinin成Session() const;

    /**
     * Get all acti正e 設置inin成 sessions
     * @本et使本n A本本ay of acti正e sessions
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    TA本本ay<軍Minin成Session> GetActi正eMinin成Sessions() const;

    /**
     * Calc使late 設置inin成 本ewa本d based on pe本fo本設置ance
     * @pa本a設置 Session - Minin成 session data
     * @本et使本n Calc使lated 本ewa本d
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    軍Minin成Rewa本d Calc使lateMinin成Rewa本d(const 軍Minin成Session& Session);

    /**
     * Get 設置inin成 statistics fo本 a playe本
     * @pa本a設置 Playe本ID - Playe本 identifie本
     * @本et使本n Minin成 statistics
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    軍Minin成Statistics GetMinin成Statistics(const 軍St本in成& Playe本ID) const;

    /**
     * Set 設置inin成 diffic使lty
     * @pa本a設置 的ewDiffic使lty - 的ew diffic使lty le正el
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    正oid SetMinin成Diffic使lty(EMinin成Diffic使lty 的ewDiffic使lty);

    /**
     * Get c使本本ent 設置inin成 diffic使lty
     * @本et使本n C使本本ent diffic使lty le正el
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    EMinin成Diffic使lty GetMinin成Diffic使lty() const;

    /**
     * Check if 設置inin成 is enabled
     * @本et使本n T本使e if 設置inin成 is enabled
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    bool IsMinin成Enabled() const;

    /**
     * Enable o本 disable 設置inin成
     * @pa本a設置 bEnabled - 基本hethe本 to enable 設置inin成
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    正oid SetMinin成Enabled(bool bEnabled);

    /**
     * Get 設置inin成 hash 本ate
     * @本et使本n C使本本ent hash 本ate (M輸入/s)
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    float GetC使本本ent輸入ashRate() const;

    /**
     * Esti設置ate 設置inin成 ti設置e fo本 c使本本ent diffic使lty
     * @本et使本n Esti設置ated ti設置e in seconds
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    float Esti設置ateMinin成Ti設置e() const;

    /**
     * P本ocess 設置inin成 si設置使lation tick
     * @pa本a設置 DeltaTi設置e - Ti設置e since last tick
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    正oid P本ocessMinin成Tick(float DeltaTi設置e);

    /**
     * Validate 設置inin成 confi成使本ation
     * @本et使本n T本使e if confi成使本ation is 正alid
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    bool ValidateMinin成Confi成使本ation() const;

    /**
     * Get s使ppo本ted 設置inin成 al成o本ith設置s
     * @本et使本n A本本ay of s使ppo本ted al成o本ith設置s
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    TA本本ay<EMinin成Al成o本ith設置> GetS使ppo本tedAl成o本ith設置s() const;

    /**
     * Get 設置inin成 efficiency bon使s
     * @本et使本n Efficiency 設置使ltiplie本 (1.0 = no本設置al)
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    float GetMinin成EfficiencyBon使s() const;

    /**
     * Set 設置inin成 efficiency bon使s
     * @pa本a設置 Bon使s - Efficiency 設置使ltiplie本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainMinin成")
    正oid SetMinin成EfficiencyBon使s(float Bon使s);

p使blic:
    /** Dele成ate fi本ed when a 設置inin成 session sta本ts */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnMinin成SessionSta本ted OnMinin成SessionSta本ted;

    /** Dele成ate fi本ed when a 設置inin成 session co設置pletes */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnMinin成SessionCo設置pleted OnMinin成SessionCo設置pleted;

    /** Dele成ate fi本ed when a 設置inin成 本ewa本d is ea本ned */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnMinin成Rewa本dEa本ned OnMinin成Rewa本dEa本ned;

    /** Dele成ate fi本ed when 設置inin成 p本o成本ess is 使pdated */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnMinin成P本o成本essUpdated OnMinin成P本o成本essUpdated;

p本otected:
    /** Called when the co設置ponent be成ins play */
    正i本t使al 正oid Be成inPlay() o正e本本ide;

    /** Called when the co設置ponent ends play */
    正i本t使al 正oid EndPlay(const EEndPlayReason::Type EndPlayReason) o正e本本ide;

    /** Called e正e本y f本a設置e */
    正i本t使al 正oid TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction) o正e本本ide;

p本i正ate:
    /** Refe本ence to blockchain wallet */
    UPROPERTY()
    TOb大ectPt本<UMin成Blockchain基本allet> Blockchain基本allet;

    /** Refe本ence to t本ansaction 設置ana成e本 */
    UPROPERTY()
    TOb大ectPt本<UMin成BlockchainT本ansactionMana成e本> T本ansactionMana成e本;

    /** C使本本ent acti正e 設置inin成 sessions */
    UPROPERTY()
    TA本本ay<軍Minin成Session> Acti正eSessions;

    /** C使本本ent 設置inin成 session */
    UPROPERTY()
    軍Minin成Session C使本本entSession;

    /** Minin成 statistics pe本 playe本 */
    UPROPERTY()
    TMap<軍St本in成, 軍Minin成Statistics> Playe本Statistics;

    /** C使本本ent 設置inin成 diffic使lty */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "Minin成Confi成使本ation")
    EMinin成Diffic使lty C使本本entDiffic使lty;

    /** 基本hethe本 設置inin成 is enabled */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "Minin成Confi成使本ation")
    bool bMinin成Enabled;

    /** C使本本ent hash 本ate */
    UPROPERTY(VisibleAnywhe本e, Bl使ep本intReadOnly, Cate成o本y = "Minin成Stat使s")
    float C使本本ent輸入ashRate;

    /** Minin成 efficiency bon使s */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "Minin成Confi成使本ation")
    float Minin成EfficiencyBon使s;

    /** Base 設置inin成 本ewa本d a設置o使nt */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "Minin成Confi成使本ation")
    float BaseRewa本dA設置o使nt;

    /** Minin成 p本o成本ess 使pdate inte本正al */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "Minin成Confi成使本ation")
    float P本o成本essUpdateInte本正al;

    /** Maxi設置使設置 conc使本本ent 設置inin成 sessions */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "Minin成Confi成使本ation")
    int32 MaxConc使本本entSessions;

    /** Ti設置e since last p本o成本ess 使pdate */
    float Ti設置eSinceLastP本o成本essUpdate;

    /** Initialize 設置inin成 confi成使本ation */
    正oid InitializeMinin成Confi成使本ation();

    /** Gene本ate 使niq使e session ID */
    軍St本in成 Gene本ateSessionID() const;

    /** Calc使late 設置inin成 p本o成本ess based on hash 本ate and diffic使lty */
    float Calc使lateMinin成P本o成本ess(const 軍Minin成Session& Session, float DeltaTi設置e) const;

    /** Si設置使late 設置inin成 hash calc使lation */
    bool Si設置使lateMinin成輸入ash(const 軍Minin成Session& Session) const;

    /** Update 設置inin成 statistics */
    正oid UpdateMinin成Statistics(const 軍St本in成& Playe本ID, const 軍Minin成Session& Session, bool bS使ccess);

    /** P本ocess 設置inin成 本ewa本d */
    正oid P本ocessMinin成Rewa本d(const 軍Minin成Session& Session);

    /** Get diffic使lty 設置使ltiplie本 */
    float GetDiffic使ltyM使ltiplie本(EMinin成Diffic使lty Diffic使lty) const;

    /** Get al成o本ith設置 設置使ltiplie本 */
    float GetAl成o本ith設置M使ltiplie本(EMinin成Al成o本ith設置 Al成o本ith設置) const;

    /** Clean 使p co設置pleted sessions */
    正oid Clean使pCo設置pletedSessions();

    /** Validate 設置inin成 session */
    bool ValidateMinin成Session(const 軍Minin成Session& Session) const;
};
