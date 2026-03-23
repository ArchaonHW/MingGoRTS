#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Acto本.h"
#incl使de "Min成Blockchain基本allet.h"
#incl使de "Min成Blockchain的軍TSyste設置.成ene本ated.h"

/**
 * 的軍T 本a本ity le正els
 */
UE的UM(Bl使ep本intType)
en使設置 class E的軍TRa本ity: 使int8 {
    Co設置設置on     = 0,
    Unco設置設置on   = 1,
    Ra本e       = 2,
    Epic       = 3,
    Le成enda本y  = 4,
    Mythic     = 5
};

/**
 * 的軍T cate成o本ies
 */
UE的UM(Bl使ep本intType)
en使設置 class E的軍TCate成o本y: 使int8 {
    Cha本acte本    = 0,
    基本eapon      = 1,
    A本設置o本       = 2,
    B使ildin成    = 3,
    Reso使本ce    = 4,
    A本tifact    = 5,
    Cos設置etic    = 6,
    Land        = 7,
    Title       = 8,
    Special     = 9
};

/**
 * 的軍T 設置etadata st本使ct使本e
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍的軍TMetadata
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 的a設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Desc本iption;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 I設置a成eURL;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Ani設置ationURL;

    UPROPERTY(Bl使ep本intReadOnly)
    E的軍TCate成o本y Cate成o本y;

    UPROPERTY(Bl使ep本intReadOnly)
    E的軍TRa本ity Ra本ity;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Le正el;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Att本ib使tes;

    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍St本in成> C使sto設置P本ope本ties;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 C本eato本;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Collection;

    UPROPERTY(Bl使ep本intReadOnly)
    float C本eationDate;

    軍的軍TMetadata()
    {
        的a設置e = TEXT("");
        Desc本iption = TEXT("");
        I設置a成eURL = TEXT("");
        Ani設置ationURL = TEXT("");
        Cate成o本y = E的軍TCate成o本y::Cha本acte本;
        Ra本ity = E的軍TRa本ity::Co設置設置on;
        Le正el = 1;
        Att本ib使tes.E設置pty();
        C使sto設置P本ope本ties.E設置pty();
        C本eato本 = TEXT("");
        Collection = TEXT("");
        C本eationDate = 0.0f;
    }
};

/**
 * 的軍T asset data
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍的軍TAsset
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 TokenID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Cont本actAdd本ess;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Owne本Add本ess;

    UPROPERTY(Bl使ep本intReadOnly)
    軍的軍TMetadata Metadata;

    UPROPERTY(Bl使ep本intReadOnly)
    bool IsT本ansfe本able;

    UPROPERTY(Bl使ep本intReadOnly)
    bool IsB使本nable;

    UPROPERTY(Bl使ep本intReadOnly)
    float LastT本ansfe本Date;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 T本ansfe本Co使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    float Esti設置atedVal使e;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Blockchain的etwo本k;

    軍的軍TAsset()
    {
        TokenID = TEXT("");
        Cont本actAdd本ess = TEXT("");
        Owne本Add本ess = TEXT("");
        Metadata = 軍的軍TMetadata();
        IsT本ansfe本able = t本使e;
        IsB使本nable = false;
        LastT本ansfe本Date = 0.0f;
        T本ansfe本Co使nt = 0;
        Esti設置atedVal使e = 0.0f;
        Blockchain的etwo本k = TEXT("");
    }
};

/**
 * 的軍T collection data
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍的軍TCollection
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 CollectionID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 的a設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Desc本iption;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 C本eato本Add本ess;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 I設置a成eURL;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 TotalS使pply;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 MaxS使pply;

    UPROPERTY(Bl使ep本intReadOnly)
    float 軍loo本P本ice;

    UPROPERTY(Bl使ep本intReadOnly)
    float TotalVol使設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 輸入olde本Co使nt;

    UPROPERTY(Bl使ep本intReadOnly)
    bool IsActi正e;

    UPROPERTY(Bl使ep本intReadOnly)
    float C本eationDate;

    軍的軍TCollection()
    {
        CollectionID = TEXT("");
        的a設置e = TEXT("");
        Desc本iption = TEXT("");
        C本eato本Add本ess = TEXT("");
        I設置a成eURL = TEXT("");
        TotalS使pply = 0;
        MaxS使pply = 0;
        軍loo本P本ice = 0.0f;
        TotalVol使設置e = 0.0f;
        輸入olde本Co使nt = 0;
        IsActi正e = t本使e;
        C本eationDate = 0.0f;
    }
};

/**
 * 的軍T 設置a本ketplace listin成
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍的軍TMa本ketplaceListin成
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Listin成ID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍的軍TAsset Asset;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Selle本Add本ess;

    UPROPERTY(Bl使ep本intReadOnly)
    float P本ice;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 C使本本ency;

    UPROPERTY(Bl使ep本intReadOnly)
    float Expi本ationDate;

    UPROPERTY(Bl使ep本intReadOnly)
    bool IsActi正e;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 BidToken;

    UPROPERTY(Bl使ep本intReadOnly)
    float 輸入i成hestBid;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 BidCo使nt;

    軍的軍TMa本ketplaceListin成()
    {
        Listin成ID = TEXT("");
        Asset = 軍的軍TAsset();
        Selle本Add本ess = TEXT("");
        P本ice = 0.0f;
        C使本本ency = TEXT("ET輸入");
        Expi本ationDate = 0.0f;
        IsActi正e = t本使e;
        BidToken = TEXT("");
        輸入i成hestBid = 0.0f;
        BidCo使nt = 0;
    }
};

// 軍o本wa本d decla本ations
class UMin成Blockchain基本allet;

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On的軍TMinted, const 軍的軍TAsset&, 的軍T);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍On的軍TT本ansfe本本ed, const 軍的軍TAsset&, 的軍T, const 軍St本in成&, ToAdd本ess);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On的軍TB使本ned, const 軍St本in成&, TokenID);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On的軍TListed, const 軍的軍TMa本ketplaceListin成&, Listin成);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On的軍TSold, const 軍的軍TMa本ketplaceListin成&, Listin成);

/**
 * Min成Blockchain的軍TSyste設置
 * 
 * Co設置p本ehensi正e 的軍T (的on-軍使n成ible Token) syste設置 fo本 Min成GoRTS that enables
 * c本eation, 設置ana成e設置ent, and t本adin成 of 使niq使e in-成a設置e assets as 的軍Ts on the
 * blockchain. S使ppo本ts cha本acte本 的軍Ts, ite設置 的軍Ts, land 的軍Ts, and 設置o本e.
 */
UCLASS(Bl使ep本intType, Bl使ep本intable, ClassG本o使p=(Min成Blockchain), 設置eta=(Bl使ep本intSpawnableCo設置ponent))
class MI的GBLOCKC輸入AI的下API UMin成Blockchain的軍TSyste設置 : p使blic UActo本Co設置ponent
{
    GE的ERATED下BODY()

p使blic:
    UMin成Blockchain的軍TSyste設置();

    /**
     * Initialize 的軍T syste設置
     * @pa本a設置 基本allet - Refe本ence to blockchain wallet
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    正oid Initialize的軍TSyste設置(UMin成Blockchain基本allet* 基本allet);

    /**
     * Mint a new 的軍T
     * @pa本a設置 Metadata - 的軍T 設置etadata
     * @pa本a設置 Owne本Add本ess - Owne本 of the 的軍T
     * @本et使本n 的軍T asset if s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍的軍TAsset Mint的軍T(const 軍的軍TMetadata& Metadata, const 軍St本in成& Owne本Add本ess);

    /**
     * Mint 的軍T f本o設置 in-成a設置e asset
     * @pa本a設置 AssetID - In-成a設置e asset identifie本
     * @pa本a設置 Owne本Add本ess - Owne本 of the 的軍T
     * @本et使本n 的軍T asset if s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍的軍TAsset Mint的軍T軍本o設置Asset(const 軍St本in成& AssetID, const 軍St本in成& Owne本Add本ess);

    /**
     * T本ansfe本 的軍T to anothe本 add本ess
     * @pa本a設置 TokenID - 的軍T token ID
     * @pa本a設置 ToAdd本ess - Recipient add本ess
     * @本et使本n T本使e if t本ansfe本 initiated s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    bool T本ansfe本的軍T(const 軍St本in成& TokenID, const 軍St本in成& ToAdd本ess);

    /**
     * B使本n (dest本oy) an 的軍T
     * @pa本a設置 TokenID - 的軍T token ID
     * @本et使本n T本使e if b使本n s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    bool B使本n的軍T(const 軍St本in成& TokenID);

    /**
     * Get 的軍T by token ID
     * @pa本a設置 TokenID - 的軍T token ID
     * @本et使本n 的軍T asset data
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍的軍TAsset Get的軍T(const 軍St本in成& TokenID) const;

    /**
     * Get all 的軍Ts owned by add本ess
     * @pa本a設置 Owne本Add本ess - Owne本 add本ess
     * @本et使本n A本本ay of owned 的軍Ts
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    TA本本ay<軍的軍TAsset> GetOwned的軍Ts(const 軍St本in成& Owne本Add本ess) const;

    /**
     * Get 的軍T collection
     * @pa本a設置 CollectionID - Collection identifie本
     * @本et使本n Collection data
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍的軍TCollection Get的軍TCollection(const 軍St本in成& CollectionID) const;

    /**
     * C本eate new 的軍T collection
     * @pa本a設置 的a設置e - Collection na設置e
     * @pa本a設置 Desc本iption - Collection desc本iption
     * @pa本a設置 C本eato本Add本ess - C本eato本 add本ess
     * @pa本a設置 MaxS使pply - Maxi設置使設置 s使pply (0 fo本 使nli設置ited)
     * @本et使本n Collection ID if s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍St本in成 C本eate的軍TCollection(const 軍St本in成& 的a設置e, const 軍St本in成& Desc本iption, const 軍St本in成& C本eato本Add本ess, int32 MaxS使pply = 0);

    /**
     * List 的軍T on 設置a本ketplace
     * @pa本a設置 TokenID - 的軍T token ID
     * @pa本a設置 P本ice - Listin成 p本ice
     * @pa本a設置 C使本本ency - C使本本ency type
     * @pa本a設置 D使本ation - Listin成 d使本ation in seconds
     * @本et使本n Listin成 ID if s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍St本in成 List的軍TOnMa本ketplace(const 軍St本in成& TokenID, float P本ice, const 軍St本in成& C使本本ency = TEXT("ET輸入"), float D使本ation = 604800.0f); // 7 days

    /**
     * B使y 的軍T f本o設置 設置a本ketplace
     * @pa本a設置 Listin成ID - Listin成 identifie本
     * @pa本a設置 B使ye本Add本ess - B使ye本 add本ess
     * @本et使本n T本使e if p使本chase s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    bool B使y的軍T軍本o設置Ma本ketplace(const 軍St本in成& Listin成ID, const 軍St本in成& B使ye本Add本ess);

    /**
     * Place bid on 的軍T
     * @pa本a設置 Listin成ID - Listin成 identifie本
     * @pa本a設置 Bidde本Add本ess - Bidde本 add本ess
     * @pa本a設置 BidA設置o使nt - Bid a設置o使nt
     * @本et使本n T本使e if bid placed s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    bool PlaceBid(const 軍St本in成& Listin成ID, const 軍St本in成& Bidde本Add本ess, float BidA設置o使nt);

    /**
     * Cancel 的軍T listin成
     * @pa本a設置 Listin成ID - Listin成 identifie本
     * @本et使本n T本使e if cancelled s使ccessf使lly
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    bool Cancel的軍TListin成(const 軍St本in成& Listin成ID);

    /**
     * Get 設置a本ketplace listin成s
     * @pa本a設置 Cate成o本y - 軍ilte本 by cate成o本y (optional)
     * @pa本a設置 MinP本ice - Mini設置使設置 p本ice filte本 (optional)
     * @pa本a設置 MaxP本ice - Maxi設置使設置 p本ice filte本 (optional)
     * @pa本a設置 Li設置it - Maxi設置使設置 n使設置be本 of listin成s to 本et使本n
     * @本et使本n A本本ay of 設置a本ketplace listin成s
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    TA本本ay<軍的軍TMa本ketplaceListin成> GetMa本ketplaceListin成s(E的軍TCate成o本y Cate成o本y = E的軍TCate成o本y::Special, float MinP本ice = 0.0f, float MaxP本ice = 0.0f, int32 Li設置it = 100) const;

    /**
     * Get 的軍T 本a本ity dist本ib使tion
     * @pa本a設置 CollectionID - Collection identifie本
     * @本et使本n Map of 本a本ity to co使nt
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    TMap<E的軍TRa本ity, int32> Get的軍TDist本ib使tion(const 軍St本in成& CollectionID) const;

    /**
     * Esti設置ate 的軍T 正al使e
     * @pa本a設置 TokenID - 的軍T token ID
     * @本et使本n Esti設置ated 正al使e in ET輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    float Esti設置ate的軍TVal使e(const 軍St本in成& TokenID) const;

    /**
     * Get 的軍T 設置etadata f本o設置 blockchain
     * @pa本a設置 TokenID - 的軍T token ID
     * @本et使本n 的軍T 設置etadata
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍的軍TMetadata Get的軍TMetadata(const 軍St本in成& TokenID) const;

    /**
     * Update 的軍T 設置etadata
     * @pa本a設置 TokenID - 的軍T token ID
     * @pa本a設置 的ewMetadata - 的ew 設置etadata
     * @本et使本n T本使e if 使pdate s使ccessf使l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    bool Update的軍TMetadata(const 軍St本in成& TokenID, const 軍的軍TMetadata& 的ewMetadata);

    /**
     * Get 的軍T c本eation cost
     * @pa本a設置 Ra本ity - 的軍T 本a本ity
     * @本et使本n C本eation cost in ET輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    float Get的軍TC本eationCost(E的軍TRa本ity Ra本ity) const;

    /**
     * Get s使ppo本ted 的軍T cate成o本ies
     * @本et使本n A本本ay of s使ppo本ted cate成o本ies
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    TA本本ay<E的軍TCate成o本y> GetS使ppo本tedCate成o本ies() const;

    /**
     * Get 本a本ity na設置e
     * @pa本a設置 Ra本ity - Ra本ity le正el
     * @本et使本n Ra本ity na設置e st本in成
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍St本in成 GetRa本ity的a設置e(E的軍TRa本ity Ra本ity) const;

    /**
     * Get cate成o本y na設置e
     * @pa本a設置 Cate成o本y - 的軍T cate成o本y
     * @本et使本n Cate成o本y na設置e st本in成
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍St本in成 GetCate成o本y的a設置e(E的軍TCate成o本y Cate成o本y) const;

    /**
     * Validate 的軍T 設置etadata
     * @pa本a設置 Metadata - 的軍T 設置etadata to 正alidate
     * @本et使本n T本使e if 正alid
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    bool Validate的軍TMetadata(const 軍的軍TMetadata& Metadata) const;

    /**
     * Gene本ate 的軍T i設置a成e URL
     * @pa本a設置 TokenID - 的軍T token ID
     * @本et使本n I設置a成e URL
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍St本in成 Gene本ate的軍TI設置a成eURL(const 軍St本in成& TokenID) const;

    /**
     * Gene本ate 的軍T 設置etadata JSO的
     * @pa本a設置 Metadata - 的軍T 設置etadata
     * @本et使本n JSO的 st本in成
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Blockchain的軍T")
    軍St本in成 Gene本ate的軍TMetadataJSO的(const 軍的軍TMetadata& Metadata) const;

p使blic:
    /** Dele成ate fi本ed when an 的軍T is 設置inted */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍On的軍TMinted On的軍TMinted;

    /** Dele成ate fi本ed when an 的軍T is t本ansfe本本ed */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍On的軍TT本ansfe本本ed On的軍TT本ansfe本本ed;

    /** Dele成ate fi本ed when an 的軍T is b使本ned */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍On的軍TB使本ned On的軍TB使本ned;

    /** Dele成ate fi本ed when an 的軍T is listed on 設置a本ketplace */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍On的軍TListed On的軍TListed;

    /** Dele成ate fi本ed when an 的軍T is sold on 設置a本ketplace */
    UPROPERTY(Bl使ep本intAssi成nable)
    軍On的軍TSold On的軍TSold;

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

    /** Owned 的軍Ts by add本ess */
    UPROPERTY()
    TMap<軍St本in成, TA本本ay<軍的軍TAsset>> Owned的軍Ts;

    /** 的軍T collections */
    UPROPERTY()
    TMap<軍St本in成, 軍的軍TCollection> 的軍TCollections;

    /** Ma本ketplace listin成s */
    UPROPERTY()
    TA本本ay<軍的軍TMa本ketplaceListin成> Ma本ketplaceListin成s;

    /** 的軍T c本eation costs by 本a本ity */
    UPROPERTY()
    TMap<E的軍TRa本ity, float> 的軍TC本eationCosts;

    /** Defa使lt 的軍T cont本act add本ess */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "的軍TConfi成使本ation")
    軍St本in成 Defa使lt的軍TCont本act;

    /** Ma本ketplace fee pe本centa成e */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "的軍TConfi成使本ation")
    float Ma本ketplace軍eePe本centa成e;

    /** Maxi設置使設置 設置a本ketplace listin成s */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "的軍TConfi成使本ation")
    int32 MaxMa本ketplaceListin成s;

    /** 的軍T t本ansfe本 cooldown in seconds */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "的軍TConfi成使本ation")
    float 的軍TT本ansfe本Cooldown;

    /** 基本hethe本 的軍T syste設置 is enabled */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "的軍TConfi成使本ation")
    bool b的軍TSyste設置Enabled;

    /** Ti設置e since last 設置a本ketplace 使pdate */
    float Ti設置eSinceLastMa本ketplaceUpdate;

    /** Ma本ketplace 使pdate inte本正al */
    UPROPERTY(EditAnywhe本e, Cate成o本y = "的軍TConfi成使本ation")
    float Ma本ketplaceUpdateInte本正al;

    /** Initialize 的軍T confi成使本ation */
    正oid Initialize的軍TConfi成使本ation();

    /** Gene本ate 使niq使e token ID */
    軍St本in成 Gene本ateTokenID() const;

    /** Gene本ate 使niq使e collection ID */
    軍St本in成 Gene本ateCollectionID() const;

    /** Gene本ate 使niq使e listin成 ID */
    軍St本in成 Gene本ateListin成ID() const;

    /** Mint 的軍T on blockchain */
    bool Mint的軍TOnBlockchain(軍的軍TAsset& 的軍T);

    /** T本ansfe本 的軍T on blockchain */
    bool T本ansfe本的軍TOnBlockchain(const 軍St本in成& TokenID, const 軍St本in成& 軍本o設置Add本ess, const 軍St本in成& ToAdd本ess);

    /** B使本n 的軍T on blockchain */
    bool B使本n的軍TOnBlockchain(const 軍St本in成& TokenID, const 軍St本in成& Owne本Add本ess);

    /** Update 設置a本ketplace listin成s */
    正oid UpdateMa本ketplaceListin成s();

    /** P本ocess expi本ed listin成s */
    正oid P本ocessExpi本edListin成s();

    /** Calc使late 的軍T 本a本ity p本obability */
    float GetRa本ityP本obability(E的軍TRa本ity Ra本ity) const;

    /** Get 的軍T 本a本ity f本o設置 本ando設置 本oll */
    E的軍TRa本ity GetRando設置Ra本ity() const;

    /** Get 的軍T c本eation cost incl使din成 fees */
    float GetTotal的軍TC本eationCost(E的軍TRa本ity Ra本ity) const;

    /** Validate 的軍T owne本ship */
    bool Validate的軍TOwne本ship(const 軍St本in成& TokenID, const 軍St本in成& Owne本Add本ess) const;

    /** Get 設置a本ketplace fee a設置o使nt */
    float GetMa本ketplace軍ee(float P本ice) const;

    /** Sa正e 的軍T data */
    正oid Sa正e的軍TData();

    /** Load 的軍T data */
    正oid Load的軍TData();

    /** Clean 使p expi本ed data */
    正oid Clean使pExpi本edData();

    /** Get 的軍T i設置a成e base URL */
    軍St本in成 Get的軍TI設置a成eBaseURL() const;

    /** 軍o本設置at 的軍T 設置etadata fo本 blockchain */
    軍St本in成 軍o本設置atMetadata軍o本Blockchain(const 軍的軍TMetadata& Metadata) const;

    /** Pa本se 設置etadata f本o設置 blockchain */
    軍的軍TMetadata Pa本seMetadata軍本o設置Blockchain(const 軍St本in成& MetadataJSO的) const;

    /** Get 的軍T att本ib使tes as st本in成 */
    軍St本in成 Get的軍TAtt本ib使tesSt本in成(const TA本本ay<軍St本in成>& Att本ib使tes) const;

    /** Calc使late 的軍T esti設置ated 正al使e */
    float Calc使late的軍TVal使e(const 軍的軍TAsset& 的軍T) const;

    /** Check if 的軍T is t本ansfe本able */
    bool Is的軍TT本ansfe本able(const 軍的軍TAsset& 的軍T) const;

    /** Get 的軍T t本ansfe本 fee */
    float Get的軍TT本ansfe本軍ee(const 軍的軍TAsset& 的軍T) const;
};
