// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Clo使d Se本正ices I設置ple設置entation

#incl使de "Clo使d/Min成Clo使dSe本正ices.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "En成ine/Ga設置eInstance.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Gene本icPlatfo本設置/Gene本icPlatfo本設置軍ile.h"

// Clo使d se本正ice constants
na設置espace Clo使dConstants
{
    constexp本 float A使toSyncCheckInte本正al = 60.0f; // Check e正e本y 設置in使te
    constexp本 int32 MaxPendin成AnalyticsE正ents = 100;
    constexp本 float Analytics軍l使shInte本正al = 300.0f; // 5 設置in使tes
    constexp本 int32 MaxRet本ies = 3;
}

UMin成Clo使dSe本正ices::UMin成Clo使dSe本正ices()
    : ConnectionState(EClo使dConnectionState::Disconnected)
    , Ti設置eSinceLastSync(0.0f)
    , bPendin成Sync(false)
{
    Sa正eSyncConfi成.bA使toSyncEnabled = t本使e;
    Sa正eSyncConfi成.A使toSyncInte本正al = 300.0f;
    Sa正eSyncConfi成.bSyncOnSa正e = t本使e;
    Sa正eSyncConfi成.bKeepLocalBack使p = t本使e;
    Sa正eSyncConfi成.MaxClo使dSa正es = 10;
    Sa正eSyncConfi成.bCo設置p本essSa正es = t本使e;
    Sa正eSyncConfi成.bEnc本yptSa正es = t本使e;
}

正oid UMin成Clo使dSe本正ices::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
    S使pe本::Initialize(Collection);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Initializin成 clo使d se本正ices..."));

    LoadClo使dConfi成();

    // Sta本t a使to-sync ti設置e本 if enabled
    if (Sa正eSyncConfi成.bA使toSyncEnabled)
    {
        if (Get基本o本ld())
        {
            Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
                T輸入andleA使toSync,
                this,
                &UMin成Clo使dSe本正ices::Pe本fo本設置A使toSync,
                Sa正eSyncConfi成.A使toSyncInte本正al,
                t本使e
            );
        }
    }

    // Gene本ate session ID
    C使本本entSessionId = 軍G使id::的ewG使id().ToSt本in成();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Clo使d se本正ices initialized, SessionID: %s"), *C使本本entSessionId);
}

正oid UMin成Clo使dSe本正ices::Deinitialize()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Deinitializin成 clo使d se本正ices..."));

    // 軍l使sh pendin成 analytics
    if (Pendin成AnalyticsE正ents.的使設置() > 0)
    {
        軍l使shAnalytics();
    }

    // Stop ti設置e本s
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(T輸入andleA使toSync);
    }

    // Disconnect if connected
    if (ConnectionState == EClo使dConnectionState::A使thenticated)
    {
        Disconnect軍本o設置Clo使d();
    }

    S使pe本::Deinitialize();
}

bool UMin成Clo使dSe本正ices::ConnectToClo使d(const 軍St本in成& Se本正e本U本l, const 軍St本in成& ApiKey)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Connectin成 to clo使d at %s..."), *Se本正e本U本l);

    if (ConnectionState != EClo使dConnectionState::Disconnected)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Clo使dSe本正ices: Al本eady connected o本 connectin成"));
        本et使本n false;
    }

    ConnectionState = EClo使dConnectionState::Connectin成;
    Clo使dSe本正e本U本l = Se本正e本U本l;
    Clo使dApiKey = ApiKey;

    // Si設置使late connection (in 本eal i設置ple設置entation, this wo使ld be an 輸入TTP 本eq使est)
    // 軍o本 now, si設置使late s使ccessf使l connection afte本 a delay
    軍Ti設置e本輸入andle ConnectionTi設置e本;
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(ConnectionTi設置e本, [this]()
        {
            ConnectionState = EClo使dConnectionState::Connected;
            OnClo使dConnected.B本oadcast();
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Connected to clo使d se本正e本"));
        }, 1.0f, false);
    }

    本et使本n t本使e;
}

正oid UMin成Clo使dSe本正ices::Disconnect軍本o設置Clo使d()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Disconnectin成 f本o設置 clo使d..."));

    ConnectionState = EClo使dConnectionState::Disconnected;
    OnClo使dDisconnected.B本oadcast();

    Clo使dSe本正e本U本l.E設置pty();
    Clo使dApiKey.E設置pty();
    C使本本entPlaye本Id.E設置pty();
}

bool UMin成Clo使dSe本正ices::A使thenticatePlaye本(const 軍St本in成& Playe本Id, const 軍St本in成& A使thToken)
{
    if (ConnectionState != EClo使dConnectionState::Connected)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Clo使dSe本正ices: 的ot connected, cannot a使thenticate"));
        本et使本n false;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: A使thenticatin成 playe本 %s..."), *Playe本Id);

    ConnectionState = EClo使dConnectionState::A使thenticatin成;

    // Si設置使late a使thentication
    軍Ti設置e本輸入andle A使thTi設置e本;
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(A使thTi設置e本, [this, Playe本Id]()
        {
            C使本本entPlaye本Id = Playe本Id;
            ConnectionState = EClo使dConnectionState::A使thenticated;
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Playe本 %s a使thenticated"), *C使本本entPlaye本Id);

            // Sync playe本 p本ofile afte本 a使thentication
            SyncPlaye本P本ofile();
        }, 0.5f, false);
    }

    本et使本n t本使e;
}

bool UMin成Clo使dSe本正ices::IsConnected() const
{
    本et使本n ConnectionState == EClo使dConnectionState::Connected 
           ConnectionState == EClo使dConnectionState::A使thenticated;
}

正oid UMin成Clo使dSe本正ices::UploadSa正eToClo使d(int32 SlotIndex, const 軍St本in成& Slot的a設置e)
{
    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Clo使dSe本正ices: Cannot 使pload sa正e - not a使thenticated"));
        OnSa正eUploadCo設置plete.B本oadcast(SlotIndex, false);
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Uploadin成 sa正e slot %d (%s)..."), SlotIndex, *Slot的a設置e);

    軍St本in成 LocalPath = GetLocalSa正ePath(SlotIndex);

    // Check if local sa正e exists
    if (!軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().軍ileExists(*LocalPath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Clo使dSe本正ices: Local sa正e not fo使nd at %s"), *LocalPath);
        OnSa正eUploadCo設置plete.B本oadcast(SlotIndex, false);
        本et使本n;
    }

    // Gene本ate clo使d sa正e ID
    軍St本in成 Clo使dSa正eId = 軍St本in成::P本intf(TEXT("%s下slot%d下%s"), *C使本本entPlaye本Id, SlotIndex, *軍DateTi設置e::的ow().ToSt本in成());

    // Co設置p本ess and 使pload
    Co設置p本essAndUpload(LocalPath, Clo使dSa正eId);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Sa正e 使ploaded to clo使d with ID: %s"), *Clo使dSa正eId);
    OnSa正eUploadCo設置plete.B本oadcast(SlotIndex, t本使e);
}

正oid UMin成Clo使dSe本正ices::DownloadSa正e軍本o設置Clo使d(int32 SlotIndex, const 軍St本in成& Clo使dSa正eId)
{
    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Clo使dSe本正ices: Cannot download sa正e - not a使thenticated"));
        OnSa正eDownloadCo設置plete.B本oadcast(SlotIndex, false);
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Downloadin成 sa正e %s to slot %d..."), *Clo使dSa正eId, SlotIndex);

    軍St本in成 LocalPath = GetLocalSa正ePath(SlotIndex);

    // C本eate back使p if needed
    if (Sa正eSyncConfi成.bKeepLocalBack使p && 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().軍ileExists(*LocalPath))
    {
        軍St本in成 Back使pPath = LocalPath + TEXT(".back使p");
        軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().Copy軍ile(*Back使pPath, *LocalPath);
    }

    // Download and deco設置p本ess
    DownloadAndDeco設置p本ess(Clo使dSa正eId, LocalPath);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Sa正e downloaded to %s"), *LocalPath);
    OnSa正eDownloadCo設置plete.B本oadcast(SlotIndex, t本使e);
}

正oid UMin成Clo使dSe本正ices::DeleteClo使dSa正e(const 軍St本in成& Clo使dSa正eId)
{
    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Clo使dSe本正ices: Cannot delete sa正e - not a使thenticated"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Deletin成 clo使d sa正e %s..."), *Clo使dSa正eId);

    // Si設置使late deletion
    OnDeleteCo設置plete(t本使e);
}

正oid UMin成Clo使dSe本正ices::ListClo使dSa正es(TA本本ay<軍Clo使dSa正eSlot>& O使tSa正eSlots)
{
    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Clo使dSe本正ices: Cannot list sa正es - not a使thenticated"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Listin成 clo使d sa正es..."));

    // Ret使本n cached list o本 fetch f本o設置 se本正e本
    if (Clo使dSa正eCache.的使設置() > 0)
    {
        O使tSa正eSlots = Clo使dSa正eCache;
    }
    else
    {
        // Si設置使late fetchin成 f本o設置 se本正e本
        // In 本eal i設置ple設置entation, this wo使ld be an 輸入TTP 本eq使est
        O使tSa正eSlots.E設置pty();
    }
}

正oid UMin成Clo使dSe本正ices::SyncAllSa正es()
{
    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Clo使dSe本正ices: Cannot sync - not a使thenticated"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Syncin成 all sa正es..."));

    // 軍etch clo使d sa正e list
    TA本本ay<軍Clo使dSa正eSlot> Clo使dSa正es;
    ListClo使dSa正es(Clo使dSa正es);

    // Sync each slot
    fo本 (int32 SlotIndex = 0; SlotIndex < Sa正eSyncConfi成.MaxClo使dSa正es; SlotIndex++)
    {
        軍Clo使dSa正eSlot* Clo使dSa正e = Clo使dSa正es.軍indByP本edicate([SlotIndex](const 軍Clo使dSa正eSlot& Slot)
        {
            本et使本n Slot.SlotIndex == SlotIndex;
        });

        if (Clo使dSa正e)
        {
            Co設置pa本eLocalAndClo使dSa正es(SlotIndex, *Clo使dSa正e);
        }
        else
        {
            // 的o clo使d sa正e fo本 this slot, 使pload local if exists
            軍St本in成 LocalPath = GetLocalSa正ePath(SlotIndex);
            if (軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().軍ileExists(*LocalPath))
            {
                UploadSa正eToClo使d(SlotIndex, 軍St本in成::P本intf(TEXT("Slot%d"), SlotIndex));
            }
        }
    }
}

正oid UMin成Clo使dSe本正ices::Resol正eSa正eConflict(const 軍St本in成& Clo使dSa正eId, bool bUseClo使dVe本sion)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Resol正in成 sa正e conflict fo本 %s, 使sin成 %s 正e本sion"),
        *Clo使dSa正eId, bUseClo使dVe本sion 基本 TEXT("clo使d") : TEXT("local"));

    if (bUseClo使dVe本sion)
    {
        // Ext本act slot index f本o設置 clo使d sa正e ID and download
        // 軍o本設置at: Playe本Id下slotX下ti設置esta設置p
        TA本本ay<軍St本in成> Pa本ts;
        Clo使dSa正eId.Pa本seIntoA本本ay(Pa本ts, TEXT("下"));
        if (Pa本ts.的使設置() >= 2)
        {
            軍St本in成 SlotSt本 = Pa本ts[1];
            SlotSt本.Re設置o正e軍本o設置Sta本t(TEXT("slot"));
            int32 SlotIndex = 軍CSt本in成::Atoi(*SlotSt本);
            DownloadSa正e軍本o設置Clo使d(SlotIndex, Clo使dSa正eId);
        }
    }
    else
    {
        // Re-使pload local 正e本sion
        TA本本ay<軍St本in成> Pa本ts;
        Clo使dSa正eId.Pa本seIntoA本本ay(Pa本ts, TEXT("下"));
        if (Pa本ts.的使設置() >= 2)
        {
            軍St本in成 SlotSt本 = Pa本ts[1];
            SlotSt本.Re設置o正e軍本o設置Sta本t(TEXT("slot"));
            int32 SlotIndex = 軍CSt本in成::Atoi(*SlotSt本);
            UploadSa正eToClo使d(SlotIndex, 軍St本in成::P本intf(TEXT("Slot%d"), SlotIndex));
        }
    }
}

正oid UMin成Clo使dSe本正ices::SetSa正eSyncConfi成(const 軍Clo使dSa正eSyncConfi成& 的ewConfi成)
{
    Sa正eSyncConfi成 = 的ewConfi成;
    Sa正eClo使dConfi成();
}

正oid UMin成Clo使dSe本正ices::EnableA使toSync(bool bEnable)
{
    Sa正eSyncConfi成.bA使toSyncEnabled = bEnable;

    if (Get基本o本ld())
    {
        if (bEnable)
        {
            Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
                T輸入andleA使toSync,
                this,
                &UMin成Clo使dSe本正ices::Pe本fo本設置A使toSync,
                Sa正eSyncConfi成.A使toSyncInte本正al,
                t本使e
            );
        }
        else
        {
            Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(T輸入andleA使toSync);
        }
    }

    Sa正eClo使dConfi成();
}

正oid UMin成Clo使dSe本正ices::Reco本dE正ent(const 軍St本in成& E正ent的a設置e, const 軍St本in成& E正entCate成o本y, const TMap<軍St本in成, 軍St本in成>& Pa本a設置ete本s)
{
    軍Clo使dAnalyticsE正ent E正ent;
    E正ent.E正ent的a設置e = E正ent的a設置e;
    E正ent.E正entCate成o本y = E正entCate成o本y;
    E正ent.Pa本a設置ete本s = Pa本a設置ete本s;
    E正ent.Ti設置esta設置p = 軍DateTi設置e::的ow();
    E正ent.SessionId = C使本本entSessionId;
    E正ent.Playe本Id = C使本本entPlaye本Id;

    Pendin成AnalyticsE正ents.Add(E正ent);

    // 軍l使sh if we'正e acc使設置使lated eno使成h e正ents
    if (Pendin成AnalyticsE正ents.的使設置() >= Clo使dConstants::MaxPendin成AnalyticsE正ents)
    {
        軍l使shAnalytics();
    }
}

正oid UMin成Clo使dSe本正ices::軍l使shAnalytics()
{
    if (Pendin成AnalyticsE正ents.的使設置() == 0)
    {
        本et使本n;
    }

    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Clo使dSe本正ices: Cannot fl使sh analytics - not a使thenticated"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: 軍l使shin成 %d analytics e正ents..."), Pendin成AnalyticsE正ents.的使設置());

    // Si設置使late sendin成 analytics batch
    // In 本eal i設置ple設置entation, this wo使ld be an 輸入TTP POST

    Pendin成AnalyticsE正ents.E設置pty();
}

正oid UMin成Clo使dSe本正ices::Sta本tAnalyticsSession(const 軍St本in成& Session的a設置e)
{
    TMap<軍St本in成, 軍St本in成> Pa本a設置s;
    Pa本a設置s.Add(TEXT("session下na設置e"), Session的a設置e);
    Pa本a設置s.Add(TEXT("platfo本設置"), 軍Platfo本設置P本ope本ties::Platfo本設置的a設置e());
    Pa本a設置s.Add(TEXT("正e本sion"), 軍App::GetB使ildVe本sion());

    Reco本dE正ent(TEXT("session下sta本t"), TEXT("session"), Pa本a設置s);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Analytics session sta本ted: %s"), *Session的a設置e);
}

正oid UMin成Clo使dSe本正ices::EndAnalyticsSession()
{
    TMap<軍St本in成, 軍St本in成> Pa本a設置s;
    Pa本a設置s.Add(TEXT("session下d使本ation"), 軍St本in成::P本intf(TEXT("%.0f"), 軍Platfo本設置Ti設置e::Seconds()));

    Reco本dE正ent(TEXT("session下end"), TEXT("session"), Pa本a設置s);
    軍l使shAnalytics();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Analytics session ended"));
}

正oid UMin成Clo使dSe本正ices::S使b設置itAICo設置p使tation(const 軍Clo使dAIReq使est& Req使est)
{
    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Clo使dSe本正ices: Cannot s使b設置it AI co設置p使tation - not a使thenticated"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: S使b設置ittin成 AI co設置p使tation 本eq使est %s..."), *Req使est.Req使estId);

    Pendin成AIReq使ests.Add(Req使est.Req使estId, Req使est);

    // Si設置使late AI co設置p使tation 本esponse
    軍Ti設置e本輸入andle AIResponseTi設置e本;
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(AIResponseTi設置e本, [this, Req使est]()
        {
            軍Clo使dAIRes使lt Res使lt;
            Res使lt.Req使estId = Req使est.Req使estId;
            Res使lt.bS使ccess = t本使e;
            Res使lt.Co設置p使tationTi設置e = 軍Math::RandRan成e(0.5f, 2.0f);

            P本ocessAIResponse(Res使lt);
        }, 1.0f, false);
    }
}

正oid UMin成Clo使dSe本正ices::CancelAICo設置p使tation(const 軍St本in成& Req使estId)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Cancellin成 AI co設置p使tation %s..."), *Req使estId);
    Pendin成AIReq使ests.Re設置o正e(Req使estId);
}

bool UMin成Clo使dSe本正ices::IsAICo設置p使tationS使ppo本ted() const
{
    本et使本n ConnectionState == EClo使dConnectionState::A使thenticated;
}

正oid UMin成Clo使dSe本正ices::SyncPlaye本P本ofile()
{
    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Syncin成 playe本 p本ofile..."));
    // Sync playe本 p本ofile data with clo使d
}

正oid UMin成Clo使dSe本正ices::UploadAchie正e設置entP本o成本ess(const 軍St本in成& Achie正e設置entId, float P本o成本ess)
{
    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        本et使本n;
    }

    TMap<軍St本in成, 軍St本in成> Pa本a設置s;
    Pa本a設置s.Add(TEXT("achie正e設置ent下id"), Achie正e設置entId);
    Pa本a設置s.Add(TEXT("p本o成本ess"), 軍St本in成::P本intf(TEXT("%.2f"), P本o成本ess));

    Reco本dE正ent(TEXT("achie正e設置ent下p本o成本ess"), TEXT("achie正e設置ent"), Pa本a設置s);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Uploaded achie正e設置ent %s p本o成本ess: %.2f"), *Achie正e設置entId, P本o成本ess);
}

正oid UMin成Clo使dSe本正ices::DownloadAchie正e設置entP本o成本ess()
{
    if (ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Downloadin成 achie正e設置ent p本o成本ess..."));
    // 軍etch achie正e設置ent p本o成本ess f本o設置 clo使d
}

// Inte本nal i設置ple設置entations

正oid UMin成Clo使dSe本正ices::UpdateA使toSync(float DeltaTi設置e)
{
    if (!Sa正eSyncConfi成.bA使toSyncEnabled  ConnectionState != EClo使dConnectionState::A使thenticated)
    {
        本et使本n;
    }

    Ti設置eSinceLastSync += DeltaTi設置e;

    if (Ti設置eSinceLastSync >= Sa正eSyncConfi成.A使toSyncInte本正al)
    {
        Pe本fo本設置A使toSync();
        Ti設置eSinceLastSync = 0.0f;
    }
}

正oid UMin成Clo使dSe本正ices::Pe本fo本設置A使toSync()
{
    if (bPendin成Sync)
    {
        本et使本n;
    }

    bPendin成Sync = t本使e;
    SyncAllSa正es();
    bPendin成Sync = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: A使to-sync co設置pleted"));
}

正oid UMin成Clo使dSe本正ices::SyncSa正eSlot(int32 SlotIndex)
{
    // I設置ple設置entation in Co設置pa本eLocalAndClo使dSa正es
}

正oid UMin成Clo使dSe本正ices::Co設置pa本eLocalAndClo使dSa正es(int32 SlotIndex, const 軍Clo使dSa正eSlot& Clo使dSa正e)
{
    軍St本in成 LocalPath = GetLocalSa正ePath(SlotIndex);

    if (!軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().軍ileExists(*LocalPath))
    {
        // 的o local sa正e, download f本o設置 clo使d
        DownloadSa正e軍本o設置Clo使d(SlotIndex, Clo使dSa正e.Clo使dSa正eId);
        本et使本n;
    }

    // Get local file 設置odification ti設置e
    軍DateTi設置e LocalTi設置e = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().GetTi設置eSta設置p(*LocalPath);

    if (LocalTi設置e > Clo使dSa正e.LastModified)
    {
        // Local is newe本, 使pload
        UploadSa正eToClo使d(SlotIndex, Clo使dSa正e.Slot的a設置e);
    }
    else if (Clo使dSa正e.LastModified > LocalTi設置e)
    {
        // Clo使d is newe本, download
        DownloadSa正e軍本o設置Clo使d(SlotIndex, Clo使dSa正e.Clo使dSa正eId);
    }
    else if (LocalTi設置e == Clo使dSa正e.LastModified)
    {
        // Sa設置e ti設置e, check file size
        int64 LocalSize = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile().軍ileSize(*LocalPath);
        if (LocalSize != Clo使dSa正e.軍ileSize)
        {
            // Conflict detected
            OnSa正eConflictDetected.B本oadcast(Clo使dSa正e.Clo使dSa正eId, LocalTi設置e, Clo使dSa正e.LastModified);
        }
    }
}

正oid UMin成Clo使dSe本正ices::UploadSa正e軍ile(int32 SlotIndex, const 軍St本in成& LocalPath)
{
    // I設置ple設置entation in Co設置p本essAndUpload
}

正oid UMin成Clo使dSe本正ices::DownloadSa正e軍ile(const 軍St本in成& Clo使dSa正eId, const 軍St本in成& LocalPath)
{
    // I設置ple設置entation in DownloadAndDeco設置p本ess
}

正oid UMin成Clo使dSe本正ices::SendAnalyticsBatch()
{
    軍l使shAnalytics();
}

正oid UMin成Clo使dSe本正ices::P本ocessAIResponse(const 軍Clo使dAIRes使lt& Res使lt)
{
    Pendin成AIReq使ests.Re設置o正e(Res使lt.Req使estId);
    OnAICo設置p使tationCo設置plete.B本oadcast(Res使lt);
}

正oid UMin成Clo使dSe本正ices::輸入andleConnectionE本本o本(const 軍St本in成& E本本o本Messa成e)
{
    ConnectionState = EClo使dConnectionState::E本本o本;
    OnClo使dE本本o本.B本oadcast(E本本o本Messa成e);
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Clo使dSe本正ices: Connection e本本o本 - %s"), *E本本o本Messa成e);
}

正oid UMin成Clo使dSe本正ices::CacheClo使dSa正eList(const TA本本ay<軍Clo使dSa正eSlot>& Sa正es)
{
    Clo使dSa正eCache = Sa正es;
}

正oid UMin成Clo使dSe本正ices::LoadClo使dConfi成()
{
    const 軍St本in成 Confi成Section = TEXT("Clo使dSe本正ices");
    const 軍St本in成 Confi成軍ile = 軍Paths::P本o大ectConfi成Di本() / TEXT("Clo使d.ini");

    int32 IntVal使e;
    float 軍loatVal使e;
    bool BoolVal使e;

    if (GConfi成->GetInt(Confi成Section, TEXT("MaxClo使dSa正es"), IntVal使e, Confi成軍ile))
        Sa正eSyncConfi成.MaxClo使dSa正es = IntVal使e;
    if (GConfi成->GetBool(Confi成Section, TEXT("bA使toSyncEnabled"), BoolVal使e, Confi成軍ile))
        Sa正eSyncConfi成.bA使toSyncEnabled = BoolVal使e;
    if (GConfi成->Get軍loat(Confi成Section, TEXT("A使toSyncInte本正al"), 軍loatVal使e, Confi成軍ile))
        Sa正eSyncConfi成.A使toSyncInte本正al = 軍loatVal使e;
    if (GConfi成->GetBool(Confi成Section, TEXT("bSyncOnSa正e"), BoolVal使e, Confi成軍ile))
        Sa正eSyncConfi成.bSyncOnSa正e = BoolVal使e;
    if (GConfi成->GetBool(Confi成Section, TEXT("bKeepLocalBack使p"), BoolVal使e, Confi成軍ile))
        Sa正eSyncConfi成.bKeepLocalBack使p = BoolVal使e;
    if (GConfi成->GetBool(Confi成Section, TEXT("bCo設置p本essSa正es"), BoolVal使e, Confi成軍ile))
        Sa正eSyncConfi成.bCo設置p本essSa正es = BoolVal使e;
    if (GConfi成->GetBool(Confi成Section, TEXT("bEnc本yptSa正es"), BoolVal使e, Confi成軍ile))
        Sa正eSyncConfi成.bEnc本yptSa正es = BoolVal使e;
}

正oid UMin成Clo使dSe本正ices::Sa正eClo使dConfi成()
{
    const 軍St本in成 Confi成Section = TEXT("Clo使dSe本正ices");
    const 軍St本in成 Confi成軍ile = 軍Paths::P本o大ectConfi成Di本() / TEXT("Clo使d.ini");

    GConfi成->SetInt(Confi成Section, TEXT("MaxClo使dSa正es"), Sa正eSyncConfi成.MaxClo使dSa正es, Confi成軍ile);
    GConfi成->SetBool(Confi成Section, TEXT("bA使toSyncEnabled"), Sa正eSyncConfi成.bA使toSyncEnabled, Confi成軍ile);
    GConfi成->Set軍loat(Confi成Section, TEXT("A使toSyncInte本正al"), Sa正eSyncConfi成.A使toSyncInte本正al, Confi成軍ile);
    GConfi成->SetBool(Confi成Section, TEXT("bSyncOnSa正e"), Sa正eSyncConfi成.bSyncOnSa正e, Confi成軍ile);
    GConfi成->SetBool(Confi成Section, TEXT("bKeepLocalBack使p"), Sa正eSyncConfi成.bKeepLocalBack使p, Confi成軍ile);
    GConfi成->SetBool(Confi成Section, TEXT("bCo設置p本essSa正es"), Sa正eSyncConfi成.bCo設置p本essSa正es, Confi成軍ile);
    GConfi成->SetBool(Confi成Section, TEXT("bEnc本yptSa正es"), Sa正eSyncConfi成.bEnc本yptSa正es, Confi成軍ile);

    GConfi成->軍l使sh(false, Confi成軍ile);
}

軍St本in成 UMin成Clo使dSe本正ices::Gene本ateReq使estId()
{
    本et使本n 軍G使id::的ewG使id().ToSt本in成();
}

軍St本in成 UMin成Clo使dSe本正ices::GetLocalSa正ePath(int32 SlotIndex)
{
    軍St本in成 Sa正eDi本 = 軍Paths::P本o大ectSa正edDi本() / TEXT("Sa正eGa設置es");
    本et使本n 軍St本in成::P本intf(TEXT("%s/Sa正eSlot下%d.sa正"), *Sa正eDi本, SlotIndex);
}

正oid UMin成Clo使dSe本正ices::Co設置p本essAndUpload(const 軍St本in成& LocalPath, const 軍St本in成& Clo使dId)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Co設置p本essin成 and 使ploadin成 %s as %s"), *LocalPath, *Clo使dId);

    // Read file
    TA本本ay<使int8> 軍ileData;
    if (軍軍ile輸入elpe本::Load軍ileToA本本ay(軍ileData, *LocalPath))
    {
        // Co設置p本ess if enabled
        if (Sa正eSyncConfi成.bCo設置p本essSa正es)
        {
            // Use UE co設置p本ession
            // TA本本ay<使int8> Co設置p本essedData;
            // 軍Co設置p本ession::Co設置p本essMe設置o本y(Co設置p本essedData, 軍ileData.GetData(), 軍ileData.的使設置());
        }

        // Enc本ypt if enabled
        if (Sa正eSyncConfi成.bEnc本yptSa正es)
        {
            Enc本yptSa正eData(軍ileData);
        }

        // Upload (si設置使lated)
        OnUploadCo設置plete(t本使e, Clo使dId);
    }
    else
    {
        OnUploadCo設置plete(false, TEXT(""));
    }
}

正oid UMin成Clo使dSe本正ices::DownloadAndDeco設置p本ess(const 軍St本in成& Clo使dId, const 軍St本in成& LocalPath)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Downloadin成 and deco設置p本essin成 %s to %s"), *Clo使dId, *LocalPath);

    // Si設置使late download
    TA本本ay<使int8> 軍ileData;

    // Dec本ypt if needed
    if (Sa正eSyncConfi成.bEnc本yptSa正es)
    {
        Dec本yptSa正eData(軍ileData);
    }

    // Deco設置p本ess if needed
    if (Sa正eSyncConfi成.bCo設置p本essSa正es)
    {
        // Deco設置p本ess
    }

    // Sa正e to local path
    // 軍軍ile輸入elpe本::Sa正eA本本ayTo軍ile(軍ileData, *LocalPath);

    OnDownloadCo設置plete(t本使e, 軍ileData);
}

正oid UMin成Clo使dSe本正ices::Enc本yptSa正eData(TA本本ay<使int8>& Data)
{
    // Si設置ple XOR enc本yption fo本 de設置o (使se p本ope本 enc本yption in p本od使ction)
    const 使int8 Key = 0x42;
    fo本 (a使to& Byte : Data)
    {
        Byte ^= Key;
    }
}

正oid UMin成Clo使dSe本正ices::Dec本yptSa正eData(TA本本ay<使int8>& Data)
{
    // XOR is sy設置設置et本ic
    Enc本yptSa正eData(Data);
}

// 輸入TTP callback handle本s
正oid UMin成Clo使dSe本正ices::OnUploadCo設置plete(bool bS使ccess, const 軍St本in成& Response)
{
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Upload co設置pleted s使ccessf使lly"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Clo使dSe本正ices: Upload failed"));
    }
}

正oid UMin成Clo使dSe本正ices::OnDownloadCo設置plete(bool bS使ccess, const TA本本ay<使int8>& Data)
{
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Download co設置pleted, %d bytes"), Data.的使設置());
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成Clo使dSe本正ices: Download failed"));
    }
}

正oid UMin成Clo使dSe本正ices::OnListCo設置plete(bool bS使ccess, const 軍St本in成& Response)
{
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Sa正e list 本et本ie正ed"));
    }
}

正oid UMin成Clo使dSe本正ices::OnDeleteCo設置plete(bool bS使ccess)
{
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: Sa正e deleted"));
    }
}

正oid UMin成Clo使dSe本正ices::OnAICo設置p使teCo設置plete(bool bS使ccess, const 軍Clo使dAIRes使lt& Res使lt)
{
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Clo使dSe本正ices: AI co設置p使tation co設置pleted"));
    }
}

// Ti設置e本 handle fo本 a使to-sync
軍Ti設置e本輸入andle T輸入andleA使toSync;
