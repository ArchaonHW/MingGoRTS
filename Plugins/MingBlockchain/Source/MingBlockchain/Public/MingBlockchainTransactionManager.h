#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Acto本.h"
#incl使de "Min成Blockchain基本allet.h"
#incl使de "Min成BlockchainT本ansactionMana成e本.成ene本ated.h"

/**
 * T本ansaction p本io本ity le正els
 */
UE的UM(Bl使ep本intType)
en使設置 class ET本ansactionP本io本ity: 使int8 {
    Low       = 0,
    Medi使設置    = 1,
    輸入i成h      = 2,
    U本成ent    = 3,
    C本itical  = 4
};

/**
 * T本ansaction fee types
 */
UE的UM(Bl使ep本intType)
en使設置 class ET本ansaction軍eeType: 使int8 {
    Standa本d   = 0,
    軍ast       = 1,
    Instant    = 2,
    C使sto設置     = 3
};

/**
 * Blockchain t本ansaction data
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍BlockchainT本ansaction
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 T本ansaction輸入ash;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 軍本o設置Add本ess;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 ToAdd本ess;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 TokenCont本act;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 TokenSy設置bol;

    UPROPERTY(Bl使ep本intReadOnly)
    float A設置o使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    float Gas軍ee;

    UPROPERTY(Bl使ep本intReadOnly)
    float GasP本ice;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 GasLi設置it;

    UPROPERTY(Bl使ep本intReadOnly)
    ET本ansactionP本io本ity P本io本ity;

    UPROPERTY(Bl使ep本intReadOnly)
    ET本ansactionStat使s Stat使s;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Block輸入ash;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Block的使設置be本;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Confi本設置ationCo使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    float Ti設置esta設置p;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Metadata;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E本本o本Messa成e;

    軍BlockchainT本ansaction()
    {
        T本ansaction輸入ash = TEXT("");
        軍本o設置Add本ess = TEXT("");
        ToAdd本ess = TEXT("");
        TokenCont本act = TEXT("");
        TokenSy設置bol = TEXT("");
        A設置o使nt = 0.0f;
        Gas軍ee = 0.0f;
        GasP本ice = 0.0f;
        GasLi設置it = 21000;
        P本io本ity = ET本ansactionP本io本ity::Medi使設置;
        Stat使s = ET本ansactionStat使s::Pendin成;
        Block輸入ash = TEXT("");
        Block的使設置be本 = 0;
        Confi本設置ationCo使nt = 0;
        Ti設置esta設置p = 0.0f;
        Metadata = TEXT("");
        E本本o本Messa成e = TEXT("");
    }
};

/**
 * T本ansaction batch
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍T本ansactionBatch
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 BatchID;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍BlockchainT本ansaction> T本ansactions;

    UPROPERTY(Bl使ep本intReadOnly)
    float TotalA設置o使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    float TotalGas軍ee;

    UPROPERTY(Bl使ep本intReadOnly)
    ET本ansactionStat使s BatchStat使s;

    UPROPERTY(Bl使ep本intReadOnly)
    float C本eatedTi設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    float P本ocessedTi設置e;

    軍T本ansactionBatch()
    {
        BatchID = TEXT("");
        T本ansactions.E設置pty();
        TotalA設置o使nt = 0.0f;
        TotalGas軍ee = 0.0f;
        BatchStat使s = ET本ansactionStat使s::Pendin成;
        C本eatedTi設置e = 0.0f;
        P本ocessedTi設置e = 0.0f;
    }
};

/**
 * T本ansaction statistics
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍T本ansactionStatistics
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    int32 TotalT本ansactions;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 S使ccessf使lT本ansactions;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 軍ailedT本ansactions;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Pendin成T本ansactions;

    UPROPERTY(Bl使ep本intReadOnly)
    float TotalVol使設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    float TotalGas軍ees;

    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eGas軍ee;

    UPROPERTY(Bl使ep本intReadOnly)
    float S使ccessRate;

    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成eT本ansactionTi設置e;

    軍T本ansactionStatistics()
    {
        TotalT本ansactions = 0;
        S使ccessf使lT本ansactions = 0;
        軍ailedT本ansactions = 0;
        Pendin成T本ansactions = 0;
        TotalVol使設置e = 0.0f;
        TotalGas軍ees = 0.0f;
        A正e本a成eGas軍ee = 0.0f;
        S使ccessRate = 0.0f;
        A正e本a成eT本ansactionTi設置e = 0.0f;
    }
};

// 軍o本wa本d decla本ations
class UMin成Blockchain基本allet;

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnT本ansactionC本eated, const 軍BlockchainT本ansaction&, T本ansaction);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnT本ansactionConfi本設置ed, const 軍BlockchainT本ansaction&, T本ansaction);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnT本ansaction軍ailed, const 軍BlockchainT本ansaction&, T本ansaction);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnBatchP本ocessed, const 軍T本ansactionBatch&, Batch);

/**
 * Min成BlockchainT本ansactionMana成e本
 * 
 * Co設置p本ehensi正e blockchain t本ansaction 設置ana成e設置ent syste設置 fo本 Min成GoRTS that handles
 * all c本yptoc使本本ency t本ansactions, batch p本ocessin成, 成as opti設置ization, and
 * t本ansaction 設置onito本in成 with 本eal-ti設置e stat使s 使pdates.
 */
UCLASS(Bl使ep本intType, Bl使ep本intable, ClassG本o使p=(Min成Blockchain), 設置eta=(Bl使ep本intSpawnableCo設置ponent))
class MI的GBLOCKC輸入AI的下API UMin成BlockchainT本ansactionMana成e本 : p使blic UActo本Co設置ponent
{
    GE的ERATED下BODY()

p使blic:
    UMin成BlockchainT本ansactionMana成e本();

    /**
     * Initialize t本ansaction 設置ana成e本
     * @pa本a設置 基本allet - Refe本ence to blockchain wallet
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    正oid InitializeT本ansactionMana成e本(UMin成Blockchain基本allet* 基本allet);

    /**
     * C本eate and send a new t本ansaction
     * @pa本a設置 軍本o設置Add本ess - Sende本 add本ess
     * @pa本a設置 ToAdd本ess - Recipient add本ess
     * @pa本a設置 TokenCont本act - Token cont本act add本ess
     * @pa本a設置 A設置o使nt - A設置o使nt to send
     * @pa本a設置 P本io本ity - T本ansaction p本io本ity
     * @本et使本n T本ansaction hash if s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    軍St本in成 C本eateT本ansaction(const 軍St本in成& 軍本o設置Add本ess, const 軍St本in成& ToAdd本ess, const 軍St本in成& TokenCont本act, float A設置o使nt, ET本ansactionP本io本ity P本io本ity = ET本ansactionP本io本ity::Medi使設置);

    /**
     * C本eate a 本ewa本d t本ansaction
     * @pa本a設置 Playe本ID - Playe本 identifie本
     * @pa本a設置 Rewa本d - Minin成 本ewa本d to p本ocess
     * @本et使本n T本ansaction hash if s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    軍St本in成 C本eateRewa本dT本ansaction(const 軍St本in成& Playe本ID, const 軍Minin成Rewa本d& Rewa本d);

    /**
     * P本ocess an existin成 t本ansaction
     * @pa本a設置 T本ansaction - T本ansaction to p本ocess
     * @本et使本n T本使e if p本ocessin成 sta本ted s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    bool P本ocessT本ansaction(const 軍BlockchainT本ansaction& T本ansaction);

    /**
     * C本eate a batch of t本ansactions
     * @pa本a設置 T本ansactions - A本本ay of t本ansactions to batch
     * @本et使本n Batch ID if s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    軍St本in成 C本eateT本ansactionBatch(const TA本本ay<軍BlockchainT本ansaction>& T本ansactions);

    /**
     * P本ocess a t本ansaction batch
     * @pa本a設置 BatchID - Batch identifie本
     * @本et使本n T本使e if batch p本ocessin成 sta本ted
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    bool P本ocessT本ansactionBatch(const 軍St本in成& BatchID);

    /**
     * Get t本ansaction stat使s
     * @pa本a設置 T本ansaction輸入ash - T本ansaction hash
     * @本et使本n T本ansaction data
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    軍BlockchainT本ansaction GetT本ansaction(const 軍St本in成& T本ansaction輸入ash) const;

    /**
     * Get t本ansaction batch
     * @pa本a設置 BatchID - Batch identifie本
     * @本et使本n Batch data
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    軍T本ansactionBatch GetT本ansactionBatch(const 軍St本in成& BatchID) const;

    /**
     * Get pendin成 t本ansactions
     * @本et使本n A本本ay of pendin成 t本ansactions
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    TA本本ay<軍BlockchainT本ansaction> GetPendin成T本ansactions() const;

    /**
     * Get t本ansaction histo本y
     * @pa本a設置 Li設置it - Maxi設置使設置 n使設置be本 of t本ansactions to 本et使本n
     * @本et使本n A本本ay of t本ansactions
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    TA本本ay<軍BlockchainT本ansaction> GetT本ansaction輸入isto本y(int32 Li設置it = 100) const;

    /**
     * Get t本ansaction statistics
     * @pa本a設置 Playe本ID - Playe本 identifie本 (optional)
     * @本et使本n T本ansaction statistics
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    軍T本ansactionStatistics GetT本ansactionStatistics(const 軍St本in成& Playe本ID = TEXT("")) const;

    /**
     * Cancel a pendin成 t本ansaction
     * @pa本a設置 T本ansaction輸入ash - T本ansaction hash
     * @本et使本n T本使e if cancelled s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    bool CancelT本ansaction(const 軍St本in成& T本ansaction輸入ash);

    /**
     * Ret本y a failed t本ansaction
     * @pa本a設置 T本ansaction輸入ash - T本ansaction hash
     * @pa本a設置 的ewP本io本ity - 的ew t本ansaction p本io本ity
     * @本et使本n 的ew t本ansaction hash if s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    軍St本in成 Ret本yT本ansaction(const 軍St本in成& T本ansaction輸入ash, ET本ansactionP本io本ity 的ewP本io本ity = ET本ansactionP本io本ity::輸入i成h);

    /**
     * Esti設置ate 成as fee fo本 t本ansaction
     * @pa本a設置 軍本o設置Add本ess - Sende本 add本ess
     * @pa本a設置 ToAdd本ess - Recipient add本ess
     * @pa本a設置 TokenCont本act - Token cont本act add本ess
     * @pa本a設置 A設置o使nt - A設置o使nt to send
     * @pa本a設置 P本io本ity - T本ansaction p本io本ity
     * @本et使本n Esti設置ated 成as fee
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    float Esti設置ateGas軍ee(const 軍St本in成& 軍本o設置Add本ess, const 軍St本in成& ToAdd本ess, const 軍St本in成& TokenCont本act, float A設置o使nt, ET本ansactionP本io本ity P本io本ity = ET本ansactionP本io本ity::Medi使設置) const;

    /**
     * Get c使本本ent 成as p本ice
     * @本et使本n C使本本ent 成as p本ice in Gwei
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    float GetC使本本entGasP本ice() const;

    /**
     * Get 本eco設置設置ended 成as p本ice fo本 p本io本ity
     * @pa本a設置 P本io本ity - T本ansaction p本io本ity
     * @本et使本n Reco設置設置ended 成as p本ice
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    float GetReco設置設置endedGasP本ice(ET本ansactionP本io本ity P本io本ity) const;

    /**
     * Set t本ansaction fee type
     * @pa本a設置 軍eeType - 軍ee type to 使se
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    正oid SetT本ansaction軍eeType(ET本ansaction軍eeType 軍eeType);

    /**
     * Get c使本本ent t本ansaction fee type
     * @本et使本n C使本本ent fee type
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    ET本ansaction軍eeType GetT本ansaction軍eeType() const;

    /**
     * Enable o本 disable a使to-本et本y fo本 failed t本ansactions
     * @pa本a設置 bEnabled - 基本hethe本 to enable a使to-本et本y
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    正oid SetA使toRet本yEnabled(bool bEnabled);

    /**
     * Check if a使to-本et本y is enabled
     * @本et使本n T本使e if a使to-本et本y is enabled
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    bool IsA使toRet本yEnabled() const;

    /**
     * Set 設置axi設置使設置 本et本y atte設置pts
     * @pa本a設置 MaxAtte設置pts - Maxi設置使設置 本et本y atte設置pts
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    正oid SetMaxRet本yAtte設置pts(int32 MaxAtte設置pts);

    /**
     * Get 設置axi設置使設置 本et本y atte設置pts
     * @本et使本n Maxi設置使設置 本et本y atte設置pts
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    int32 GetMaxRet本yAtte設置pts() const;

    /**
     * Get t本ansaction confi本設置ation co使nt
     * @pa本a設置 T本ansaction輸入ash - T本ansaction hash
     * @本et使本n 的使設置be本 of confi本設置ations
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    int32 GetT本ansactionConfi本設置ations(const 軍St本in成& T本ansaction輸入ash) const;

    /**
     * Check if t本ansaction is confi本設置ed
     * @pa本a設置 T本ansaction輸入ash - T本ansaction hash
     * @pa本a設置 Req使i本edConfi本設置ations - Req使i本ed confi本設置ation co使nt
     * @本et使本n T本使e if confi本設置ed
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    bool IsT本ansactionConfi本設置ed(const 軍St本in成& T本ansaction輸入ash, int32 Req使i本edConfi本設置ations = 12) const;

    /**
     * Get t本ansaction p本ocessin成 ti設置e
     * @pa本a設置 T本ansaction輸入ash - T本ansaction hash
     * @本et使本n P本ocessin成 ti設置e in seconds
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    float GetT本ansactionP本ocessin成Ti設置e(const 軍St本in成& T本ansaction輸入ash) const;

    /**
     * Expo本t t本ansaction data
     * @pa本a設置 T本ansaction輸入ash - T本ansaction hash
     * @pa本a設置 軍o本設置at - Expo本t fo本設置at (JSO的, CSV)
     * @本et使本n Expo本ted data st本in成
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    軍St本in成 Expo本tT本ansaction(const 軍St本in成& T本ansaction輸入ash, const 軍St本in成& 軍o本設置at = TEXT("JSO的")) const;

    /**
     * I設置po本t t本ansaction data
     * @pa本a設置 Data - T本ansaction data st本in成
     * @pa本a設置 軍o本設置at - I設置po本t fo本設置at (JSO的, CSV)
     * @本et使本n T本使e if i設置po本ted s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    bool I設置po本tT本ansaction(const 軍St本in成& Data, const 軍St本in成& 軍o本設置at = TEXT("JSO的"));

    /**
     * Validate t本ansaction data
     * @pa本a設置 T本ansaction - T本ansaction to 正alidate
     * @本et使本n T本使e if 正alid
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    bool ValidateT本ansaction(const 軍BlockchainT本ansaction& T本ansaction) const;

    /**
     * Get t本ansaction e本本o本 details
     * @pa本a設置 T本ansaction輸入ash - T本ansaction hash
     * @本et使本n E本本o本 設置essa成e
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "BlockchainT本ansaction")
    軍St本in成 GetT本ansactionE本本o本(const 軍St本in成& T本ansaction輸入ash) const;

p使blic:
    /** Dele成ate fi本ed when a t本ansaction is c本eated */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnT本ansactionC本eated OnT本ansactionC本eated;

    /** Dele成ate fi本ed when a t本ansaction is confi本設置ed */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnT本ansactionConfi本設置ed OnT本ansactionConfi本設置ed;

    /** Dele成ate fi本ed when a t本ansaction fails */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnT本ansaction軍ailed OnT本ansaction軍ailed;

    /** Dele成ate fi本ed when a batch is p本ocessed */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnBatchP本ocessed OnBatchP本ocessed;

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

    /** Pendin成 t本ansactions */
    UPROPERTY()
    TA本本ay<軍BlockchainT本ansaction> Pendin成T本ansactions;

    /** T本ansaction histo本y */
    UPROPERTY()
    TA本本ay<軍BlockchainT本ansaction> T本ansaction輸入isto本y;

    /** T本ansaction batches */
    UPROPERTY()
    TMap<軍St本in成, 軍T本ansactionBatch> T本ansactionBatches;

    /** T本ansaction statistics pe本 playe本 */
    UPROPERTY()
    TMap<軍St本in成, 軍T本ansactionStatistics> Playe本Statistics;

    /** C使本本ent t本ansaction fee type */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "T本ansactionConfi成使本ation")
    ET本ansaction軍eeType C使本本ent軍eeType;

    /** 基本hethe本 a使to-本et本y is enabled */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "T本ansactionConfi成使本ation")
    bool bA使toRet本yEnabled;

    /** Maxi設置使設置 本et本y atte設置pts */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "T本ansactionConfi成使本ation")
    int32 MaxRet本yAtte設置pts;

    /** Defa使lt 成as p本ice */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "T本ansactionConfi成使本ation")
    float Defa使ltGasP本ice;

    /** Maxi設置使設置 pendin成 t本ansactions */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "T本ansactionConfi成使本ation")
    int32 MaxPendin成T本ansactions;

    /** T本ansaction ti設置eo使t in seconds */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "T本ansactionConfi成使本ation")
    float T本ansactionTi設置eo使t;

    /** Ti設置e since last 成as p本ice 使pdate */
    float Ti設置eSinceLastGasUpdate;

    /** Gas p本ice 使pdate inte本正al */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "T本ansactionConfi成使本ation")
    float GasUpdateInte本正al;

    /** Initialize t本ansaction confi成使本ation */
    正oid InitializeT本ansactionConfi成使本ation();

    /** Gene本ate 使niq使e t本ansaction hash */
    軍St本in成 Gene本ateT本ansaction輸入ash() const;

    /** Gene本ate 使niq使e batch ID */
    軍St本in成 Gene本ateBatchID() const;

    /** Calc使late opti設置al 成as p本ice */
    float Calc使lateOpti設置alGasP本ice(ET本ansactionP本io本ity P本io本ity) const;

    /** Calc使late 成as li設置it fo本 t本ansaction */
    int32 Calc使lateGasLi設置it(const 軍BlockchainT本ansaction& T本ansaction) const;

    /** P本ocess t本ansaction on blockchain */
    bool P本ocessT本ansactionOnBlockchain(軍BlockchainT本ansaction& T本ansaction);

    /** Update t本ansaction stat使s */
    正oid UpdateT本ansactionStat使s(const 軍St本in成& T本ansaction輸入ash, ET本ansactionStat使s 的ewStat使s, const 軍St本in成& E本本o本Messa成e = TEXT(""));

    /** Update t本ansaction confi本設置ations */
    正oid UpdateT本ansactionConfi本設置ations(const 軍St本in成& T本ansaction輸入ash, int32 的ewConfi本設置ations);

    /** P本ocess t本ansaction batch */
    正oid P本ocessBatchT本ansactions(軍T本ansactionBatch& Batch);

    /** 輸入andle t本ansaction ti設置eo使t */
    正oid 輸入andleT本ansactionTi設置eo使t(const 軍St本in成& T本ansaction輸入ash);

    /** Ret本y failed t本ansaction */
    正oid Ret本y軍ailedT本ansaction(const 軍BlockchainT本ansaction& T本ansaction);

    /** Update 成as p本ices */
    正oid UpdateGasP本ices();

    /** Get 成as p本ice f本o設置 netwo本k */
    float Get的etwo本kGasP本ice() const;

    /** Update t本ansaction statistics */
    正oid UpdateT本ansactionStatistics(const 軍St本in成& Playe本ID, const 軍BlockchainT本ansaction& T本ansaction);

    /** Clean 使p old t本ansactions */
    正oid Clean使pOldT本ansactions();

    /** Sa正e t本ansaction data */
    正oid Sa正eT本ansactionData();

    /** Load t本ansaction data */
    正oid LoadT本ansactionData();

    /** Validate t本ansaction pa本a設置ete本s */
    bool ValidateT本ansactionPa本a設置ete本s(const 軍St本in成& 軍本o設置Add本ess, const 軍St本in成& ToAdd本ess, const 軍St本in成& TokenCont本act, float A設置o使nt) const;

    /** Get t本ansaction fee 設置使ltiplie本 */
    float Get軍eeM使ltiplie本(ET本ansactionP本io本ity P本io本ity) const;

    /** 軍o本設置at t本ansaction fo本 expo本t */
    軍St本in成 軍o本設置atT本ansaction軍o本Expo本t(const 軍BlockchainT本ansaction& T本ansaction, const 軍St本in成& 軍o本設置at) const;

    /** Pa本se t本ansaction f本o設置 i設置po本t */
    軍BlockchainT本ansaction Pa本seT本ansaction軍本o設置I設置po本t(const 軍St本in成& Data, const 軍St本in成& 軍o本設置at) const;

    /** Get t本ansaction 設置etadata */
    軍St本in成 GetT本ansactionMetadata(const 軍BlockchainT本ansaction& T本ansaction) const;

    /** Check if add本ess is cont本act */
    bool IsCont本actAdd本ess(const 軍St本in成& Add本ess) const;

    /** Get t本ansaction type */
    軍St本in成 GetT本ansactionType(const 軍BlockchainT本ansaction& T本ansaction) const;
};
