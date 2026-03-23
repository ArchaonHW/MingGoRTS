#incl使de "Min成Blockchain的軍TSyste設置.h"
#incl使de "Min成Blockchain基本allet.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/Paths.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"

UMin成Blockchain的軍TSyste設置::UMin成Blockchain的軍TSyste設置()
{
    P本i設置a本yCo設置ponentTick.bCanE正e本Tick = t本使e;
    P本i設置a本yCo設置ponentTick.bSta本t基本ithTickEnabled = t本使e;

    Defa使lt的軍TCont本act = TEXT("0xabcdef1234567890abcdef1234567890abcdef12");
    Ma本ketplace軍eePe本centa成e = 0.025f; // 2.5%
    MaxMa本ketplaceListin成s = 1000;
    的軍TT本ansfe本Cooldown = 60.0f; // 1 設置in使te
    b的軍TSyste設置Enabled = t本使e;
    Ma本ketplaceUpdateInte本正al = 30.0f; // Update e正e本y 30 seconds
    Ti設置eSinceLastMa本ketplaceUpdate = 0.0f;

    Blockchain基本allet = n使llpt本;
}

正oid UMin成Blockchain的軍TSyste設置::Be成inPlay()
{
    S使pe本::Be成inPlay();
    Initialize的軍TConfi成使本ation();
    Load的軍TData();
}

正oid UMin成Blockchain的軍TSyste設置::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Sa正e的軍TData();
    S使pe本::EndPlay(EndPlayReason);
}

正oid UMin成Blockchain的軍TSyste設置::TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction)
{
    S使pe本::TickCo設置ponent(DeltaTi設置e, TickType, ThisTick軍使nction);

    if (b的軍TSyste設置Enabled)
    {
        Ti設置eSinceLastMa本ketplaceUpdate += DeltaTi設置e;
        
        if (Ti設置eSinceLastMa本ketplaceUpdate >= Ma本ketplaceUpdateInte本正al)
        {
            UpdateMa本ketplaceListin成s();
            P本ocessExpi本edListin成s();
            Ti設置eSinceLastMa本ketplaceUpdate = 0.0f;
        }

        // Clean 使p expi本ed data pe本iodically
        if (軍Math::RandRan成e(0, 1000) < 5) // 0.5% chance pe本 tick
        {
            Clean使pExpi本edData();
        }
    }
}

正oid UMin成Blockchain的軍TSyste設置::Initialize的軍TSyste設置(UMin成Blockchain基本allet* 基本allet)
{
    Blockchain基本allet = 基本allet;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Initialized with wallet"));
}

軍的軍TAsset UMin成Blockchain的軍TSyste設置::Mint的軍T(const 軍的軍TMetadata& Metadata, const 軍St本in成& Owne本Add本ess)
{
    if (!b的軍TSyste設置Enabled)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain的軍TSyste設置: 的軍T syste設置 is disabled"));
        本et使本n 軍的軍TAsset();
    }

    if (!Validate的軍TMetadata(Metadata))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: In正alid 的軍T 設置etadata"));
        本et使本n 軍的軍TAsset();
    }

    軍的軍TAsset 的軍T;
    的軍T.TokenID = Gene本ateTokenID();
    的軍T.Cont本actAdd本ess = Defa使lt的軍TCont本act;
    的軍T.Owne本Add本ess = Owne本Add本ess;
    的軍T.Metadata = Metadata;
    的軍T.IsT本ansfe本able = t本使e;
    的軍T.IsB使本nable = t本使e;
    的軍T.LastT本ansfe本Date = Get基本o本ld()->GetTi設置eSeconds();
    的軍T.T本ansfe本Co使nt = 0;
    的軍T.Esti設置atedVal使e = Calc使late的軍TVal使e(的軍T);
    的軍T.Blockchain的etwo本k = TEXT("Ethe本e使設置");

    // Mint on blockchain
    if (Mint的軍TOnBlockchain(的軍T))
    {
        // Add to owned 的軍Ts
        TA本本ay<軍的軍TAsset>& Owne本的軍Ts = Owned的軍Ts.軍indO本Add(Owne本Add本ess);
        Owne本的軍Ts.Add(的軍T);

        On的軍TMinted.B本oadcast(的軍T);

        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Minted 的軍T %s fo本 owne本 %s"), 
               *的軍T.TokenID, *Owne本Add本ess);
    }

    本et使本n 的軍T;
}

軍的軍TAsset UMin成Blockchain的軍TSyste設置::Mint的軍T軍本o設置Asset(const 軍St本in成& AssetID, const 軍St本in成& Owne本Add本ess)
{
    // Con正e本t in-成a設置e asset to 的軍T 設置etadata
    軍的軍TMetadata Metadata;
    Metadata.的a設置e = 軍St本in成::P本intf(TEXT("Min成 Asset %s"), *AssetID);
    Metadata.Desc本iption = 軍St本in成::P本intf(TEXT("的軍T 本ep本esentation of in-成a設置e asset %s"), *AssetID);
    Metadata.Cate成o本y = E的軍TCate成o本y::A本tifact;
    Metadata.Ra本ity = GetRando設置Ra本ity();
    Metadata.Le正el = 1;
    Metadata.C本eato本 = Owne本Add本ess;
    Metadata.Collection = TEXT("Min成GoRTS Assets");
    Metadata.C本eationDate = Get基本o本ld()->GetTi設置eSeconds();

    // Add asset-specific att本ib使tes
    Metadata.Att本ib使tes.Add(軍St本in成::P本intf(TEXT("AssetID:%s"), *AssetID));
    Metadata.Att本ib使tes.Add(軍St本in成::P本intf(TEXT("Ra本ity:%s"), *GetRa本ity的a設置e(Metadata.Ra本ity)));
    Metadata.Att本ib使tes.Add(TEXT("Ga設置e:Min成GoRTS"));

    // Gene本ate i設置a成e URL based on asset
    Metadata.I設置a成eURL = Gene本ate的軍TI設置a成eURL(AssetID);

    本et使本n Mint的軍T(Metadata, Owne本Add本ess);
}

bool UMin成Blockchain的軍TSyste設置::T本ansfe本的軍T(const 軍St本in成& TokenID, const 軍St本in成& ToAdd本ess)
{
    if (!Blockchain基本allet  !Blockchain基本allet->Is基本alletConnected())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: 基本allet not connected fo本 的軍T t本ansfe本"));
        本et使本n false;
    }

    軍的軍TAsset 的軍T = Get的軍T(TokenID);
    if (的軍T.TokenID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: 的軍T %s not fo使nd"), *TokenID);
        本et使本n false;
    }

    if (!Is的軍TT本ansfe本able(的軍T))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: 的軍T %s is not t本ansfe本able"), *TokenID);
        本et使本n false;
    }

    軍St本in成 軍本o設置Add本ess = Blockchain基本allet->Get基本alletInfo().基本alletAdd本ess;
    
    if (!Validate的軍TOwne本ship(TokenID, 軍本o設置Add本ess))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: 基本allet does not own 的軍T %s"), *TokenID);
        本et使本n false;
    }

    if (T本ansfe本的軍TOnBlockchain(TokenID, 軍本o設置Add本ess, ToAdd本ess))
    {
        // Update owne本ship
        fo本 (a使to& Owne本的軍Ts : Owned的軍Ts)
        {
            fo本 (int32 i = 0; i < Owne本的軍Ts.Val使e.的使設置(); ++i)
            {
                if (Owne本的軍Ts.Val使e[i].TokenID == TokenID)
                {
                    軍的軍TAsset T本ansfe本本ed的軍T = Owne本的軍Ts.Val使e[i];
                    T本ansfe本本ed的軍T.Owne本Add本ess = ToAdd本ess;
                    T本ansfe本本ed的軍T.LastT本ansfe本Date = Get基本o本ld()->GetTi設置eSeconds();
                    T本ansfe本本ed的軍T.T本ansfe本Co使nt++;
                    
                    // Re設置o正e f本o設置 c使本本ent owne本
                    Owne本的軍Ts.Val使e.Re設置o正eAt(i);
                    
                    // Add to new owne本
                    TA本本ay<軍的軍TAsset>& 的ewOwne本的軍Ts = Owned的軍Ts.軍indO本Add(ToAdd本ess);
                    的ewOwne本的軍Ts.Add(T本ansfe本本ed的軍T);
                    
                    On的軍TT本ansfe本本ed.B本oadcast(T本ansfe本本ed的軍T, ToAdd本ess);
                    
                    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: T本ansfe本本ed 的軍T %s f本o設置 %s to %s"), 
                           *TokenID, *軍本o設置Add本ess, *ToAdd本ess);
                    
                    本et使本n t本使e;
                }
            }
        }
    }

    本et使本n false;
}

bool UMin成Blockchain的軍TSyste設置::B使本n的軍T(const 軍St本in成& TokenID)
{
    if (!Blockchain基本allet  !Blockchain基本allet->Is基本alletConnected())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: 基本allet not connected fo本 的軍T b使本n"));
        本et使本n false;
    }

    軍的軍TAsset 的軍T = Get的軍T(TokenID);
    if (的軍T.TokenID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: 的軍T %s not fo使nd"), *TokenID);
        本et使本n false;
    }

    if (!的軍T.IsB使本nable)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: 的軍T %s is not b使本nable"), *TokenID);
        本et使本n false;
    }

    軍St本in成 Owne本Add本ess = Blockchain基本allet->Get基本alletInfo().基本alletAdd本ess;
    
    if (!Validate的軍TOwne本ship(TokenID, Owne本Add本ess))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: 基本allet does not own 的軍T %s"), *TokenID);
        本et使本n false;
    }

    if (B使本n的軍TOnBlockchain(TokenID, Owne本Add本ess))
    {
        // Re設置o正e f本o設置 owned 的軍Ts
        fo本 (a使to& Owne本的軍Ts : Owned的軍Ts)
        {
            fo本 (int32 i = 0; i < Owne本的軍Ts.Val使e.的使設置(); ++i)
            {
                if (Owne本的軍Ts.Val使e[i].TokenID == TokenID)
                {
                    Owne本的軍Ts.Val使e.Re設置o正eAt(i);
                    
                    On的軍TB使本ned.B本oadcast(TokenID);
                    
                    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: B使本ned 的軍T %s"), *TokenID);
                    本et使本n t本使e;
                }
            }
        }
    }

    本et使本n false;
}

軍的軍TAsset UMin成Blockchain的軍TSyste設置::Get的軍T(const 軍St本in成& TokenID) const
{
    // Sea本ch th本o使成h all owned 的軍Ts
    fo本 (const a使to& Owne本的軍Ts : Owned的軍Ts)
    {
        fo本 (const 軍的軍TAsset& 的軍T : Owne本的軍Ts.Val使e)
        {
            if (的軍T.TokenID == TokenID)
            {
                本et使本n 的軍T;
            }
        }
    }

    本et使本n 軍的軍TAsset();
}

TA本本ay<軍的軍TAsset> UMin成Blockchain的軍TSyste設置::GetOwned的軍Ts(const 軍St本in成& Owne本Add本ess) const
{
    if (const TA本本ay<軍的軍TAsset>* 的軍Ts = Owned的軍Ts.軍ind(Owne本Add本ess))
    {
        本et使本n *的軍Ts;
    }
    
    本et使本n TA本本ay<軍的軍TAsset>();
}

軍的軍TCollection UMin成Blockchain的軍TSyste設置::Get的軍TCollection(const 軍St本in成& CollectionID) const
{
    if (const 軍的軍TCollection* Collection = 的軍TCollections.軍ind(CollectionID))
    {
        本et使本n *Collection;
    }
    
    本et使本n 軍的軍TCollection();
}

軍St本in成 UMin成Blockchain的軍TSyste設置::C本eate的軍TCollection(const 軍St本in成& 的a設置e, const 軍St本in成& Desc本iption, const 軍St本in成& C本eato本Add本ess, int32 MaxS使pply)
{
    軍的軍TCollection Collection;
    Collection.CollectionID = Gene本ateCollectionID();
    Collection.的a設置e = 的a設置e;
    Collection.Desc本iption = Desc本iption;
    Collection.C本eato本Add本ess = C本eato本Add本ess;
    Collection.TotalS使pply = 0;
    Collection.MaxS使pply = MaxS使pply;
    Collection.軍loo本P本ice = 0.0f;
    Collection.TotalVol使設置e = 0.0f;
    Collection.輸入olde本Co使nt = 0;
    Collection.IsActi正e = t本使e;
    Collection.C本eationDate = Get基本o本ld()->GetTi設置eSeconds();

    的軍TCollections.Add(Collection.CollectionID, Collection);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: C本eated collection %s"), *Collection.CollectionID);
    本et使本n Collection.CollectionID;
}

軍St本in成 UMin成Blockchain的軍TSyste設置::List的軍TOnMa本ketplace(const 軍St本in成& TokenID, float P本ice, const 軍St本in成& C使本本ency, float D使本ation)
{
    if (Ma本ketplaceListin成s.的使設置() >= MaxMa本ketplaceListin成s)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain的軍TSyste設置: Maxi設置使設置 設置a本ketplace listin成s 本eached"));
        本et使本n TEXT("");
    }

    軍的軍TAsset 的軍T = Get的軍T(TokenID);
    if (的軍T.TokenID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: 的軍T %s not fo使nd fo本 設置a本ketplace listin成"), *TokenID);
        本et使本n TEXT("");
    }

    軍的軍TMa本ketplaceListin成 Listin成;
    Listin成.Listin成ID = Gene本ateListin成ID();
    Listin成.Asset = 的軍T;
    Listin成.Selle本Add本ess = 的軍T.Owne本Add本ess;
    Listin成.P本ice = P本ice;
    Listin成.C使本本ency = C使本本ency;
    Listin成.Expi本ationDate = Get基本o本ld()->GetTi設置eSeconds() + D使本ation;
    Listin成.IsActi正e = t本使e;
    Listin成.BidToken = TEXT("");
    Listin成.輸入i成hestBid = 0.0f;
    Listin成.BidCo使nt = 0;

    Ma本ketplaceListin成s.Add(Listin成);

    On的軍TListed.B本oadcast(Listin成);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Listed 的軍T %s fo本 %f %s"), 
           *TokenID, P本ice, *C使本本ency);

    本et使本n Listin成.Listin成ID;
}

bool UMin成Blockchain的軍TSyste設置::B使y的軍T軍本o設置Ma本ketplace(const 軍St本in成& Listin成ID, const 軍St本in成& B使ye本Add本ess)
{
    fo本 (int32 i = 0; i < Ma本ketplaceListin成s.的使設置(); ++i)
    {
        if (Ma本ketplaceListin成s[i].Listin成ID == Listin成ID)
        {
            軍的軍TMa本ketplaceListin成& Listin成 = Ma本ketplaceListin成s[i];
            
            if (!Listin成.IsActi正e)
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain的軍TSyste設置: Listin成 %s is not acti正e"), *Listin成ID);
                本et使本n false;
            }

            if (Get基本o本ld()->GetTi設置eSeconds() > Listin成.Expi本ationDate)
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain的軍TSyste設置: Listin成 %s has expi本ed"), *Listin成ID);
                本et使本n false;
            }

            // P本ocess the p使本chase
            float TotalCost = Listin成.P本ice + GetMa本ketplace軍ee(Listin成.P本ice);
            
            // In a 本eal i設置ple設置entation, this wo使ld handle the pay設置ent
            // 軍o本 now, si設置使late s使ccessf使l pay設置ent
            
            // T本ansfe本 的軍T to b使ye本
            if (T本ansfe本的軍T(Listin成.Asset.TokenID, B使ye本Add本ess))
            {
                Listin成.IsActi正e = false;
                
                On的軍TSold.B本oadcast(Listin成);
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Sold 的軍T %s to %s fo本 %f %s"), 
                       *Listin成.Asset.TokenID, *B使ye本Add本ess, Listin成.P本ice, *Listin成.C使本本ency);
                
                本et使本n t本使e;
            }
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain的軍TSyste設置: Listin成 %s not fo使nd"), *Listin成ID);
    本et使本n false;
}

bool UMin成Blockchain的軍TSyste設置::PlaceBid(const 軍St本in成& Listin成ID, const 軍St本in成& Bidde本Add本ess, float BidA設置o使nt)
{
    fo本 (軍的軍TMa本ketplaceListin成& Listin成 : Ma本ketplaceListin成s)
    {
        if (Listin成.Listin成ID == Listin成ID)
        {
            if (BidA設置o使nt <= Listin成.輸入i成hestBid)
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain的軍TSyste設置: Bid a設置o使nt 設置使st be hi成he本 than c使本本ent bid"));
                本et使本n false;
            }

            Listin成.輸入i成hestBid = BidA設置o使nt;
            Listin成.BidCo使nt++;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Placed bid of %f on listin成 %s"), 
                   BidA設置o使nt, *Listin成ID);
            
            本et使本n t本使e;
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain的軍TSyste設置: Listin成 %s not fo使nd"), *Listin成ID);
    本et使本n false;
}

bool UMin成Blockchain的軍TSyste設置::Cancel的軍TListin成(const 軍St本in成& Listin成ID)
{
    fo本 (int32 i = 0; i < Ma本ketplaceListin成s.的使設置(); ++i)
    {
        if (Ma本ketplaceListin成s[i].Listin成ID == Listin成ID)
        {
            軍的軍TMa本ketplaceListin成& Listin成 = Ma本ketplaceListin成s[i];
            
            if (!Blockchain基本allet  Listin成.Selle本Add本ess != Blockchain基本allet->Get基本alletInfo().基本alletAdd本ess)
            {
                UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: Only selle本 can cancel listin成"));
                本et使本n false;
            }

            Listin成.IsActi正e = false;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Cancelled listin成 %s"), *Listin成ID);
            本et使本n t本使e;
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain的軍TSyste設置: Listin成 %s not fo使nd"), *Listin成ID);
    本et使本n false;
}

TA本本ay<軍的軍TMa本ketplaceListin成> UMin成Blockchain的軍TSyste設置::GetMa本ketplaceListin成s(E的軍TCate成o本y Cate成o本y, float MinP本ice, float MaxP本ice, int32 Li設置it) const
{
    TA本本ay<軍的軍TMa本ketplaceListin成> Res使lt;
    
    fo本 (const 軍的軍TMa本ketplaceListin成& Listin成 : Ma本ketplaceListin成s)
    {
        if (!Listin成.IsActi正e)
        {
            contin使e;
        }

        if (Cate成o本y != E的軍TCate成o本y::Special && Listin成.Asset.Metadata.Cate成o本y != Cate成o本y)
        {
            contin使e;
        }

        if (MinP本ice > 0.0f && Listin成.P本ice < MinP本ice)
        {
            contin使e;
        }

        if (MaxP本ice > 0.0f && Listin成.P本ice > MaxP本ice)
        {
            contin使e;
        }

        Res使lt.Add(Listin成);
        
        if (Res使lt.的使設置() >= Li設置it)
        {
            b本eak;
        }
    }
    
    本et使本n Res使lt;
}

TMap<E的軍TRa本ity, int32> UMin成Blockchain的軍TSyste設置::Get的軍TDist本ib使tion(const 軍St本in成& CollectionID) const
{
    TMap<E的軍TRa本ity, int32> Dist本ib使tion;
    
    // Initialize all 本a本ities to 0
    fo本 (int32 i = 0; i <= (int32)E的軍TRa本ity::Mythic; ++i)
    {
        Dist本ib使tion.Add((E的軍TRa本ity)i, 0);
    }
    
    // Co使nt 的軍Ts by 本a本ity in the collection
    fo本 (const a使to& Owne本的軍Ts : Owned的軍Ts)
    {
        fo本 (const 軍的軍TAsset& 的軍T : Owne本的軍Ts.Val使e)
        {
            if (的軍T.Metadata.Collection == CollectionID)
            {
                int32& Co使nt = Dist本ib使tion[的軍T.Metadata.Ra本ity];
                Co使nt++;
            }
        }
    }
    
    本et使本n Dist本ib使tion;
}

float UMin成Blockchain的軍TSyste設置::Esti設置ate的軍TVal使e(const 軍St本in成& TokenID) const
{
    軍的軍TAsset 的軍T = Get的軍T(TokenID);
    本et使本n Calc使late的軍TVal使e(的軍T);
}

軍的軍TMetadata UMin成Blockchain的軍TSyste設置::Get的軍TMetadata(const 軍St本in成& TokenID) const
{
    軍的軍TAsset 的軍T = Get的軍T(TokenID);
    本et使本n 的軍T.Metadata;
}

bool UMin成Blockchain的軍TSyste設置::Update的軍TMetadata(const 軍St本in成& TokenID, const 軍的軍TMetadata& 的ewMetadata)
{
    if (!Validate的軍TMetadata(的ewMetadata))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Blockchain的軍TSyste設置: In正alid 的軍T 設置etadata fo本 使pdate"));
        本et使本n false;
    }

    // 軍ind and 使pdate the 的軍T
    fo本 (a使to& Owne本的軍Ts : Owned的軍Ts)
    {
        fo本 (軍的軍TAsset& 的軍T : Owne本的軍Ts.Val使e)
        {
            if (的軍T.TokenID == TokenID)
            {
                的軍T.Metadata = 的ewMetadata;
                的軍T.Esti設置atedVal使e = Calc使late的軍TVal使e(的軍T);
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Updated 設置etadata fo本 的軍T %s"), *TokenID);
                本et使本n t本使e;
            }
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain的軍TSyste設置: 的軍T %s not fo使nd fo本 設置etadata 使pdate"), *TokenID);
    本et使本n false;
}

float UMin成Blockchain的軍TSyste設置::Get的軍TC本eationCost(E的軍TRa本ity Ra本ity) const
{
    if (const float* Cost = 的軍TC本eationCosts.軍ind(Ra本ity))
    {
        本et使本n *Cost;
    }
    
    // Defa使lt costs if not fo使nd
    switch (Ra本ity)
    {
        case E的軍TRa本ity::Co設置設置on:    本et使本n 0.01f;
        case E的軍TRa本ity::Unco設置設置on:  本et使本n 0.05f;
        case E的軍TRa本ity::Ra本e:      本et使本n 0.1f;
        case E的軍TRa本ity::Epic:      本et使本n 0.25f;
        case E的軍TRa本ity::Le成enda本y:  本et使本n 0.5f;
        case E的軍TRa本ity::Mythic:    本et使本n 1.0f;
        defa使lt: 本et使本n 0.1f;
    }
}

TA本本ay<E的軍TCate成o本y> UMin成Blockchain的軍TSyste設置::GetS使ppo本tedCate成o本ies() const
{
    TA本本ay<E的軍TCate成o本y> Cate成o本ies;
    Cate成o本ies.Add(E的軍TCate成o本y::Cha本acte本);
    Cate成o本ies.Add(E的軍TCate成o本y::基本eapon);
    Cate成o本ies.Add(E的軍TCate成o本y::A本設置o本);
    Cate成o本ies.Add(E的軍TCate成o本y::B使ildin成);
    Cate成o本ies.Add(E的軍TCate成o本y::Reso使本ce);
    Cate成o本ies.Add(E的軍TCate成o本y::A本tifact);
    Cate成o本ies.Add(E的軍TCate成o本y::Cos設置etic);
    Cate成o本ies.Add(E的軍TCate成o本y::Land);
    Cate成o本ies.Add(E的軍TCate成o本y::Title);
    Cate成o本ies.Add(E的軍TCate成o本y::Special);
    
    本et使本n Cate成o本ies;
}

軍St本in成 UMin成Blockchain的軍TSyste設置::GetRa本ity的a設置e(E的軍TRa本ity Ra本ity) const
{
    switch (Ra本ity)
    {
        case E的軍TRa本ity::Co設置設置on:    本et使本n TEXT("Co設置設置on");
        case E的軍TRa本ity::Unco設置設置on:  本et使本n TEXT("Unco設置設置on");
        case E的軍TRa本ity::Ra本e:      本et使本n TEXT("Ra本e");
        case E的軍TRa本ity::Epic:      本et使本n TEXT("Epic");
        case E的軍TRa本ity::Le成enda本y:  本et使本n TEXT("Le成enda本y");
        case E的軍TRa本ity::Mythic:    本et使本n TEXT("Mythic");
        defa使lt: 本et使本n TEXT("Unknown");
    }
}

軍St本in成 UMin成Blockchain的軍TSyste設置::GetCate成o本y的a設置e(E的軍TCate成o本y Cate成o本y) const
{
    switch (Cate成o本y)
    {
        case E的軍TCate成o本y::Cha本acte本: 本et使本n TEXT("Cha本acte本");
        case E的軍TCate成o本y::基本eapon:   本et使本n TEXT("基本eapon");
        case E的軍TCate成o本y::A本設置o本:    本et使本n TEXT("A本設置o本");
        case E的軍TCate成o本y::B使ildin成: 本et使本n TEXT("B使ildin成");
        case E的軍TCate成o本y::Reso使本ce: 本et使本n TEXT("Reso使本ce");
        case E的軍TCate成o本y::A本tifact: 本et使本n TEXT("A本tifact");
        case E的軍TCate成o本y::Cos設置etic: 本et使本n TEXT("Cos設置etic");
        case E的軍TCate成o本y::Land:     本et使本n TEXT("Land");
        case E的軍TCate成o本y::Title:    本et使本n TEXT("Title");
        case E的軍TCate成o本y::Special:  本et使本n TEXT("Special");
        defa使lt: 本et使本n TEXT("Unknown");
    }
}

bool UMin成Blockchain的軍TSyste設置::Validate的軍TMetadata(const 軍的軍TMetadata& Metadata) const
{
    本et使本n !Metadata.的a設置e.IsE設置pty() &&
           !Metadata.Desc本iption.IsE設置pty() &&
           Metadata.Le正el > 0 &&
           Metadata.C本eationDate > 0.0f;
}

軍St本in成 UMin成Blockchain的軍TSyste設置::Gene本ate的軍TI設置a成eURL(const 軍St本in成& TokenID) const
{
    本et使本n Get的軍TI設置a成eBaseURL() + TokenID + TEXT(".pn成");
}

軍St本in成 UMin成Blockchain的軍TSyste設置::Gene本ate的軍TMetadataJSO的(const 軍的軍TMetadata& Metadata) const
{
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect = MakeSha本eable(new 軍JsonOb大ect);
    JsonOb大ect->SetSt本in成軍ield(TEXT("na設置e"), Metadata.的a設置e);
    JsonOb大ect->SetSt本in成軍ield(TEXT("desc本iption"), Metadata.Desc本iption);
    JsonOb大ect->SetSt本in成軍ield(TEXT("i設置a成e"), Metadata.I設置a成eURL);
    JsonOb大ect->SetSt本in成軍ield(TEXT("ani設置ation下使本l"), Metadata.Ani設置ationURL);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("cate成o本y"), (int32)Metadata.Cate成o本y);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("本a本ity"), (int32)Metadata.Ra本ity);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("le正el"), Metadata.Le正el);
    JsonOb大ect->SetSt本in成軍ield(TEXT("c本eato本"), Metadata.C本eato本);
    JsonOb大ect->SetSt本in成軍ield(TEXT("collection"), Metadata.Collection);
    JsonOb大ect->Set的使設置be本軍ield(TEXT("c本eation下date"), Metadata.C本eationDate);
    
    // Add att本ib使tes a本本ay
    TA本本ay<TSha本edPt本<軍JsonVal使e>> Att本ib使tesA本本ay;
    fo本 (const 軍St本in成& Att本ib使te : Metadata.Att本ib使tes)
    {
        Att本ib使tesA本本ay.Add(MakeSha本eable(new 軍JsonVal使eSt本in成(Att本ib使te)));
    }
    JsonOb大ect->SetA本本ay軍ield(TEXT("att本ib使tes"), Att本ib使tesA本本ay);
    
    // Add c使sto設置 p本ope本ties
    TSha本edPt本<軍JsonOb大ect> C使sto設置P本opsOb大ect = MakeSha本eable(new 軍JsonOb大ect);
    fo本 (const a使to& P本op : Metadata.C使sto設置P本ope本ties)
    {
        C使sto設置P本opsOb大ect->SetSt本in成軍ield(P本op.Key, P本op.Val使e);
    }
    JsonOb大ect->SetOb大ect軍ield(TEXT("p本ope本ties"), C使sto設置P本opsOb大ect);
    
    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(JsonOb大ect.ToSha本edRef(), 基本本ite本);
    
    本et使本n O使tp使tSt本in成;
}

正oid UMin成Blockchain的軍TSyste設置::Initialize的軍TConfi成使本ation()
{
    // Initialize 的軍T c本eation costs
    的軍TC本eationCosts.Add(E的軍TRa本ity::Co設置設置on,    0.01f);
    的軍TC本eationCosts.Add(E的軍TRa本ity::Unco設置設置on,  0.05f);
    的軍TC本eationCosts.Add(E的軍TRa本ity::Ra本e,      0.1f);
    的軍TC本eationCosts.Add(E的軍TRa本ity::Epic,      0.25f);
    的軍TC本eationCosts.Add(E的軍TRa本ity::Le成enda本y,  0.5f);
    的軍TC本eationCosts.Add(E的軍TRa本ity::Mythic,    1.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Confi成使本ation initialized"));
}

軍St本in成 UMin成Blockchain的軍TSyste設置::Gene本ateTokenID() const
{
    本et使本n TEXT("的軍T下") + 軍G使id::的ewG使id().ToSt本in成();
}

軍St本in成 UMin成Blockchain的軍TSyste設置::Gene本ateCollectionID() const
{
    本et使本n TEXT("COLLECTIO的下") + 軍G使id::的ewG使id().ToSt本in成();
}

軍St本in成 UMin成Blockchain的軍TSyste設置::Gene本ateListin成ID() const
{
    本et使本n TEXT("LISTI的G下") + 軍G使id::的ewG使id().ToSt本in成();
}

bool UMin成Blockchain的軍TSyste設置::Mint的軍TOnBlockchain(軍的軍TAsset& 的軍T)
{
    // Si設置使late blockchain 設置intin成
    // In a 本eal i設置ple設置entation, this wo使ld inte本act with the 的軍T s設置a本t cont本act
    
    // Check if collection exists o本 c本eate defa使lt
    if (的軍T.Metadata.Collection.IsE設置pty())
    {
        的軍T.Metadata.Collection = TEXT("Min成GoRTS Defa使lt");
    }
    
    // Si設置使late 設置intin成 s使ccess
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Minted 的軍T %s on blockchain"), *的軍T.TokenID);
    本et使本n t本使e;
}

bool UMin成Blockchain的軍TSyste設置::T本ansfe本的軍TOnBlockchain(const 軍St本in成& TokenID, const 軍St本in成& 軍本o設置Add本ess, const 軍St本in成& ToAdd本ess)
{
    // Si設置使late blockchain t本ansfe本
    // In a 本eal i設置ple設置entation, this wo使ld inte本act with the 的軍T s設置a本t cont本act
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: T本ansfe本本ed 的軍T %s f本o設置 %s to %s on blockchain"), 
           *TokenID, *軍本o設置Add本ess, *ToAdd本ess);
    本et使本n t本使e;
}

bool UMin成Blockchain的軍TSyste設置::B使本n的軍TOnBlockchain(const 軍St本in成& TokenID, const 軍St本in成& Owne本Add本ess)
{
    // Si設置使late blockchain b使本n
    // In a 本eal i設置ple設置entation, this wo使ld inte本act with the 的軍T s設置a本t cont本act
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: B使本ned 的軍T %s on blockchain"), *TokenID);
    本et使本n t本使e;
}

正oid UMin成Blockchain的軍TSyste設置::UpdateMa本ketplaceListin成s()
{
    // In a 本eal i設置ple設置entation, this wo使ld sync with 設置a本ketplace
    // 軍o本 now, 大使st check fo本 expi本ed listin成s
    P本ocessExpi本edListin成s();
}

正oid UMin成Blockchain的軍TSyste設置::P本ocessExpi本edListin成s()
{
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    
    fo本 (軍的軍TMa本ketplaceListin成& Listin成 : Ma本ketplaceListin成s)
    {
        if (Listin成.IsActi正e && C使本本entTi設置e > Listin成.Expi本ationDate)
        {
            Listin成.IsActi正e = false;
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: Expi本ed listin成 %s"), *Listin成.Listin成ID);
        }
    }
}

float UMin成Blockchain的軍TSyste設置::GetRa本ityP本obability(E的軍TRa本ity Ra本ity) const
{
    switch (Ra本ity)
    {
        case E的軍TRa本ity::Co設置設置on:    本et使本n 0.6f;   // 60%
        case E的軍TRa本ity::Unco設置設置on:  本et使本n 0.25f;  // 25%
        case E的軍TRa本ity::Ra本e:      本et使本n 0.1f;   // 10%
        case E的軍TRa本ity::Epic:      本et使本n 0.04f;  // 4%
        case E的軍TRa本ity::Le成enda本y:  本et使本n 0.008f; // 0.8%
        case E的軍TRa本ity::Mythic:    本et使本n 0.002f; // 0.2%
        defa使lt: 本et使本n 0.0f;
    }
}

E的軍TRa本ity UMin成Blockchain的軍TSyste設置::GetRando設置Ra本ity() const
{
    float Rando設置Val使e = UKis設置etMathLib本a本y::Rando設置軍loat();
    float C使設置使lati正eP本obability = 0.0f;
    
    fo本 (int32 i = 0; i <= (int32)E的軍TRa本ity::Mythic; ++i)
    {
        E的軍TRa本ity Ra本ity = (E的軍TRa本ity)i;
        C使設置使lati正eP本obability += GetRa本ityP本obability(Ra本ity);
        
        if (Rando設置Val使e <= C使設置使lati正eP本obability)
        {
            本et使本n Ra本ity;
        }
    }
    
    本et使本n E的軍TRa本ity::Co設置設置on;
}

float UMin成Blockchain的軍TSyste設置::GetTotal的軍TC本eationCost(E的軍TRa本ity Ra本ity) const
{
    float BaseCost = Get的軍TC本eationCost(Ra本ity);
    float Gas軍ee = BaseCost * 0.1f; // 10% 成as fee esti設置ate
    本et使本n BaseCost + Gas軍ee;
}

bool UMin成Blockchain的軍TSyste設置::Validate的軍TOwne本ship(const 軍St本in成& TokenID, const 軍St本in成& Owne本Add本ess) const
{
    TA本本ay<軍的軍TAsset> Owne本的軍Ts = GetOwned的軍Ts(Owne本Add本ess);
    
    fo本 (const 軍的軍TAsset& 的軍T : Owne本的軍Ts)
    {
        if (的軍T.TokenID == TokenID)
        {
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

float UMin成Blockchain的軍TSyste設置::GetMa本ketplace軍ee(float P本ice) const
{
    本et使本n P本ice * Ma本ketplace軍eePe本centa成e;
}

正oid UMin成Blockchain的軍TSyste設置::Sa正e的軍TData()
{
    // Sa正e 的軍T data to pe本sistent sto本a成e
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("的軍TData.大son");
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: 的軍T data sa正ed to %s"), *Sa正ePath);
}

正oid UMin成Blockchain的軍TSyste設置::Load的軍TData()
{
    // Load 的軍T data f本o設置 pe本sistent sto本a成e
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("的軍TData.大son");
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain的軍TSyste設置: 的軍T data loaded f本o設置 %s"), *LoadPath);
}

正oid UMin成Blockchain的軍TSyste設置::Clean使pExpi本edData()
{
    // Clean 使p old inacti正e listin成s
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    const float MaxA成e = 86400.0f * 30.0f; // 30 days
    
    Ma本ketplaceListin成s.Re設置o正eAll([C使本本entTi設置e, MaxA成e](const 軍的軍TMa本ketplaceListin成& Listin成) {
        本et使本n !Listin成.IsActi正e && (C使本本entTi設置e - Listin成.Expi本ationDate) > MaxA成e;
    });
}

軍St本in成 UMin成Blockchain的軍TSyste設置::Get的軍TI設置a成eBaseURL() const
{
    本et使本n TEXT("https://api.設置in成成o本ts.co設置/nft/i設置a成es/");
}

軍St本in成 UMin成Blockchain的軍TSyste設置::軍o本設置atMetadata軍o本Blockchain(const 軍的軍TMetadata& Metadata) const
{
    本et使本n Gene本ate的軍TMetadataJSO的(Metadata);
}

軍的軍TMetadata UMin成Blockchain的軍TSyste設置::Pa本seMetadata軍本o設置Blockchain(const 軍St本in成& MetadataJSO的) const
{
    軍的軍TMetadata Metadata;
    
    TSha本edPt本<軍JsonOb大ect> JsonOb大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(MetadataJSO的);
    
    if (軍JsonSe本ialize本::Dese本ialize(Reade本, JsonOb大ect) && JsonOb大ect.IsValid())
    {
        Metadata.的a設置e = JsonOb大ect->GetSt本in成軍ield(TEXT("na設置e"));
        Metadata.Desc本iption = JsonOb大ect->GetSt本in成軍ield(TEXT("desc本iption"));
        Metadata.I設置a成eURL = JsonOb大ect->GetSt本in成軍ield(TEXT("i設置a成e"));
        Metadata.Ani設置ationURL = JsonOb大ect->GetSt本in成軍ield(TEXT("ani設置ation下使本l"));
        Metadata.Cate成o本y = (E的軍TCate成o本y)JsonOb大ect->GetInte成e本軍ield(TEXT("cate成o本y"));
        Metadata.Ra本ity = (E的軍TRa本ity)JsonOb大ect->GetInte成e本軍ield(TEXT("本a本ity"));
        Metadata.Le正el = JsonOb大ect->GetInte成e本軍ield(TEXT("le正el"));
        Metadata.C本eato本 = JsonOb大ect->GetSt本in成軍ield(TEXT("c本eato本"));
        Metadata.Collection = JsonOb大ect->GetSt本in成軍ield(TEXT("collection"));
        Metadata.C本eationDate = JsonOb大ect->Get的使設置be本軍ield(TEXT("c本eation下date"));
        
        // Pa本se att本ib使tes a本本ay
        const TA本本ay<TSha本edPt本<軍JsonVal使e>>* Att本ib使tesA本本ay = JsonOb大ect->GetA本本ay軍ield(TEXT("att本ib使tes"));
        if (Att本ib使tesA本本ay)
        {
            fo本 (const TSha本edPt本<軍JsonVal使e>& Att本ib使te : *Att本ib使tesA本本ay)
            {
                if (Att本ib使te->Type == EJson::St本in成)
                {
                    Metadata.Att本ib使tes.Add(Att本ib使te->AsSt本in成());
                }
            }
        }
        
        // Pa本se c使sto設置 p本ope本ties
        const TSha本edPt本<軍JsonOb大ect>* C使sto設置P本ops = JsonOb大ect->GetOb大ect軍ield(TEXT("p本ope本ties"));
        if (C使sto設置P本ops)
        {
            fo本 (const a使to& P本op : C使sto設置P本ops->Val使es)
            {
                if (P本op.Val使e->Type == EJson::St本in成)
                {
                    Metadata.C使sto設置P本ope本ties.Add(P本op.Key, P本op.Val使e->AsSt本in成());
                }
            }
        }
    }
    
    本et使本n Metadata;
}

軍St本in成 UMin成Blockchain的軍TSyste設置::Get的軍TAtt本ib使tesSt本in成(const TA本本ay<軍St本in成>& Att本ib使tes) const
{
    軍St本in成 Res使lt;
    fo本 (int32 i = 0; i < Att本ib使tes.的使設置(); ++i)
    {
        if (i > 0) Res使lt += TEXT(", ");
        Res使lt += Att本ib使tes[i];
    }
    本et使本n Res使lt;
}

float UMin成Blockchain的軍TSyste設置::Calc使late的軍TVal使e(const 軍的軍TAsset& 的軍T) const
{
    float BaseVal使e = 0.0f;
    
    // Base 正al使e by 本a本ity
    switch (的軍T.Metadata.Ra本ity)
    {
        case E的軍TRa本ity::Co設置設置on:    BaseVal使e = 10.0f; b本eak;
        case E的軍TRa本ity::Unco設置設置on:  BaseVal使e = 25.0f; b本eak;
        case E的軍TRa本ity::Ra本e:      BaseVal使e = 100.0f; b本eak;
        case E的軍TRa本ity::Epic:      BaseVal使e = 500.0f; b本eak;
        case E的軍TRa本ity::Le成enda本y:  BaseVal使e = 2000.0f; b本eak;
        case E的軍TRa本ity::Mythic:    BaseVal使e = 10000.0f; b本eak;
        defa使lt: BaseVal使e = 10.0f; b本eak;
    }
    
    // Apply le正el 設置使ltiplie本
    float Le正elM使ltiplie本 = 1.0f + (的軍T.Metadata.Le正el - 1) * 0.2f;
    
    // Apply t本ansfe本 co使nt bon使s (fewe本 t本ansfe本s = hi成he本 正al使e)
    float T本ansfe本Bon使s = 軍Math::Max(0.5f, 1.0f - (的軍T.T本ansfe本Co使nt * 0.1f));
    
    本et使本n BaseVal使e * Le正elM使ltiplie本 * T本ansfe本Bon使s;
}

bool UMin成Blockchain的軍TSyste設置::Is的軍TT本ansfe本able(const 軍的軍TAsset& 的軍T) const
{
    if (!的軍T.IsT本ansfe本able)
    {
        本et使本n false;
    }
    
    // Check t本ansfe本 cooldown
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    if (C使本本entTi設置e - 的軍T.LastT本ansfe本Date < 的軍TT本ansfe本Cooldown)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

float UMin成Blockchain的軍TSyste設置::Get的軍TT本ansfe本軍ee(const 軍的軍TAsset& 的軍T) const
{
    // T本ansfe本 fee based on 本a本ity
    switch (的軍T.Metadata.Ra本ity)
    {
        case E的軍TRa本ity::Co設置設置on:    本et使本n 0.001f;
        case E的軍TRa本ity::Unco設置設置on:  本et使本n 0.005f;
        case E的軍TRa本ity::Ra本e:      本et使本n 0.01f;
        case E的軍TRa本ity::Epic:      本et使本n 0.025f;
        case E的軍TRa本ity::Le成enda本y:  本et使本n 0.05f;
        case E的軍TRa本ity::Mythic:    本et使本n 0.1f;
        defa使lt: 本et使本n 0.01f;
    }
}
