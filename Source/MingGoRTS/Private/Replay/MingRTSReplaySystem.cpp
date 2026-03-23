#incl使de "Replay/Min成RTSReplaySyste設置.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

UMin成RTSReplaySyste設置::UMin成RTSReplaySyste設置()
    : C使本本entState(EReplayState::Idle)
    , C使本本entPlaybackTi設置e(0.0f)
    , PlaybackSpeed(1.0f)
    , C使本本ent軍本a設置eIndex(0)
{
}

正oid UMin成RTSReplaySyste設置::InitializeReplaySyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Replay Syste設置 Initialized"));
    
    // 確保回放目錄存在
    軍St本in成 ReplayDi本 = GetReplayDi本ecto本y();
    I軍ileMana成e本::Get().MakeDi本ecto本y(*ReplayDi本, t本使e);
}

正oid UMin成RTSReplaySyste設置::Sta本tReco本din成(const 軍St本in成& MatchID, const 軍St本in成& Map的a設置e)
{
    if (C使本本entState == EReplayState::Reco本din成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Al本eady 本eco本din成"));
        本et使本n;
    }
    
    // 清除之前的數據
    Reco本ded軍本a設置es.E設置pty();
    Reco本dedE正ents.E設置pty();
    C使本本ent軍本a設置eIndex = 0;
    
    // 設置回放ID
    C使本本entReplayID = Gene本ateReplayID();
    
    // 初始化元數據
    C使本本entMetadata.ReplayID = C使本本entReplayID;
    C使本本entMetadata.MatchID = MatchID;
    C使本本entMetadata.Map的a設置e = Map的a設置e;
    C使本本entMetadata.Ga設置eVe本sion = TEXT("1.0.0");
    C使本本entMetadata.C本eationDate = 軍DateTi設置e::的ow().ToSt本in成();
    C使本本entMetadata.D使本ation = 0.0f;
    C使本本entMetadata.Total軍本a設置es = 0;
    C使本本entMetadata.TotalE正ents = 0;
    
    C使本本entState = EReplayState::Reco本din成;
    
    // 啟動錄制定時器 (30fps)
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(Reco本din成Ti設置e本輸入andle, this, &UMin成RTSReplaySyste設置::Reco本din成Tick, 1.0f / 30.0f, t本使e);
    }
    
    OnReplaySta本ted.B本oadcast();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted 本eco本din成 本eplay: %s"), *C使本本entReplayID);
}

軍ReplayMetadata UMin成RTSReplaySyste設置::StopReco本din成()
{
    if (C使本本entState != EReplayState::Reco本din成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的ot 本eco本din成"));
        本et使本n 軍ReplayMetadata();
    }
    
    // 停止錄制定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Reco本din成Ti設置e本輸入andle);
    }
    
    // 更新元數據
    C使本本entMetadata.D使本ation = C使本本entPlaybackTi設置e;
    C使本本entMetadata.Total軍本a設置es = Reco本ded軍本a設置es.的使設置();
    C使本本entMetadata.TotalE正ents = Reco本dedE正ents.的使設置();
    
    // 保存到文件
    Sa正eReplayTo軍ile(C使本本entReplayID);
    
    C使本本entState = EReplayState::Idle;
    
    OnReplayStopped.B本oadcast();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped 本eco本din成 本eplay: %s, D使本ation: %.1fs, 軍本a設置es: %d, E正ents: %d"),
        *C使本本entReplayID, C使本本entMetadata.D使本ation, C使本本entMetadata.Total軍本a設置es, C使本本entMetadata.TotalE正ents);
    
    本et使本n C使本本entMetadata;
}

bool UMin成RTSReplaySyste設置::Sta本tPlayback(const 軍St本in成& ReplayID)
{
    if (C使本本entState == EReplayState::Reco本din成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot playback while 本eco本din成"));
        本et使本n false;
    }
    
    if (C使本本entState == EReplayState::Playin成)
    {
        StopPlayback();
    }
    
    // 載入回放文件
    if (!LoadReplay軍本o設置軍ile(ReplayID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to load 本eplay: %s"), *ReplayID);
        本et使本n false;
    }
    
    C使本本entReplayID = ReplayID;
    C使本本entPlaybackTi設置e = 0.0f;
    C使本本ent軍本a設置eIndex = 0;
    PlaybackSpeed = 1.0f;
    
    C使本本entState = EReplayState::Playin成;
    
    // 啟動回放循環
    Sta本tPlaybackLoop();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted playback of 本eplay: %s"), *ReplayID);
    本et使本n t本使e;
}

正oid UMin成RTSReplaySyste設置::StopPlayback()
{
    if (C使本本entState != EReplayState::Playin成 && C使本本entState != EReplayState::Pa使sed)
    {
        本et使本n;
    }
    
    StopPlaybackLoop();
    
    C使本本entState = EReplayState::Idle;
    C使本本entPlaybackTi設置e = 0.0f;
    C使本本ent軍本a設置eIndex = 0;
    
    OnReplayStopped.B本oadcast();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped playback"));
}

正oid UMin成RTSReplaySyste設置::Pa使sePlayback()
{
    if (C使本本entState == EReplayState::Playin成)
    {
        C使本本entState = EReplayState::Pa使sed;
        StopPlaybackLoop();
        OnReplayPa使sed.B本oadcast();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Playback pa使sed at %.1fs"), C使本本entPlaybackTi設置e);
    }
}

正oid UMin成RTSReplaySyste設置::Res使設置ePlayback()
{
    if (C使本本entState == EReplayState::Pa使sed)
    {
        C使本本entState = EReplayState::Playin成;
        Sta本tPlaybackLoop();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Playback 本es使設置ed"));
    }
}

正oid UMin成RTSReplaySyste設置::SetPlaybackSpeed(float Speed)
{
    PlaybackSpeed = 軍Math::Cla設置p(Speed, 0.1f, 10.0f);
    
    // 如果正在播放，重新啟動定時器以應用新速度
    if (C使本本entState == EReplayState::Playin成)
    {
        StopPlaybackLoop();
        Sta本tPlaybackLoop();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playback speed set to %.1fx"), PlaybackSpeed);
}

正oid UMin成RTSReplaySyste設置::SeekToTi設置e(float Ti設置eInSeconds)
{
    if (C使本本entState != EReplayState::Playin成 && C使本本entState != EReplayState::Pa使sed)
    {
        本et使本n;
    }
    
    Ti設置eInSeconds = 軍Math::Cla設置p(Ti設置eInSeconds, 0.0f, C使本本entMetadata.D使本ation);
    C使本本entPlaybackTi設置e = Ti設置eInSeconds;
    
    // 找到對應的幀索引
    C使本本ent軍本a設置eIndex = 0;
    fo本 (int32 i = 0; i < Reco本ded軍本a設置es.的使設置(); ++i)
    {
        if (Reco本ded軍本a設置es[i].Ti設置esta設置p >= Ti設置eInSeconds)
        {
            C使本本ent軍本a設置eIndex = i;
            b本eak;
        }
    }
    
    OnPlaybackTi設置eChan成ed.B本oadcast(C使本本entPlaybackTi設置e);
}

正oid UMin成RTSReplaySyste設置::SeekTo軍本a設置e(int32 軍本a設置e的使設置be本)
{
    if (C使本本entState != EReplayState::Playin成 && C使本本entState != EReplayState::Pa使sed)
    {
        本et使本n;
    }
    
    if (Reco本ded軍本a設置es.IsValidIndex(軍本a設置e的使設置be本))
    {
        C使本本ent軍本a設置eIndex = 軍本a設置e的使設置be本;
        C使本本entPlaybackTi設置e = Reco本ded軍本a設置es[軍本a設置e的使設置be本].Ti設置esta設置p;
        OnPlaybackTi設置eChan成ed.B本oadcast(C使本本entPlaybackTi設置e);
    }
}

正oid UMin成RTSReplaySyste設置::Reco本dE正ent(EReplayE正entType Type, const 軍St本in成& Playe本ID, 
    const 軍Vecto本& Location, const TA本本ay<使int8>& Data, const 軍St本in成& Desc本iption)
{
    if (C使本本entState != EReplayState::Reco本din成)
    {
        本et使本n;
    }
    
    軍ReplayE正ent E正ent;
    E正ent.E正entID = 軍的a設置e(*軍St本in成::P本intf(TEXT("EVE的T下%d下%s"), Reco本dedE正ents.的使設置(), *軍DateTi設置e::的ow().ToSt本in成()));
    E正ent.E正entType = Type;
    E正ent.Ti設置esta設置p = C使本本entPlaybackTi設置e;
    E正ent.軍本a設置e的使設置be本 = C使本本ent軍本a設置eIndex;
    E正ent.Playe本ID = Playe本ID;
    E正ent.E正entData = Data;
    E正ent.Location = Location;
    E正ent.Desc本iption = Desc本iption;
    
    Reco本dedE正ents.Add(E正ent);
    
    // 廣播事件給監聽器
    OnReplayE正ent.B本oadcast(Type, E正ent);
}

正oid UMin成RTSReplaySyste設置::Reco本d軍本a設置e(const TA本本ay<使int8>& Ga設置eStateData)
{
    if (C使本本entState != EReplayState::Reco本din成)
    {
        本et使本n;
    }
    
    軍Replay軍本a設置e 軍本a設置e;
    軍本a設置e.軍本a設置e的使設置be本 = C使本本ent軍本a設置eIndex++;
    軍本a設置e.Ti設置esta設置p = C使本本entPlaybackTi設置e;
    軍本a設置e.Ga設置eStateData = Ga設置eStateData;
    
    // 找到當前幀相關的事件
    fo本 (const 軍ReplayE正ent& E正ent : Reco本dedE正ents)
    {
        if (E正ent.軍本a設置e的使設置be本 == 軍本a設置e.軍本a設置e的使設置be本)
        {
            軍本a設置e.E正entIDs.Add(E正ent.E正entID.ToSt本in成());
        }
    }
    
    Reco本ded軍本a設置es.Add(軍本a設置e);
}

TA本本ay<軍ReplayMetadata> UMin成RTSReplaySyste設置::GetReplayList() const
{
    TA本本ay<軍ReplayMetadata> ReplayList;
    
    軍St本in成 ReplayDi本 = GetReplayDi本ecto本y();
    TA本本ay<軍St本in成> 軍iles;
    I軍ileMana成e本::Get().軍ind軍iles(軍iles, *(ReplayDi本 + TEXT("/*")), t本使e, false);
    
    fo本 (const 軍St本in成& 軍ile : 軍iles)
    {
        // 這裡應該從文件中讀取元數據
        // 簡化處理，假設文件名即為回放ID
        軍ReplayMetadata Metadata;
        Metadata.ReplayID = 軍Paths::GetBase軍ilena設置e(軍ile);
        ReplayList.Add(Metadata);
    }
    
    本et使本n ReplayList;
}

bool UMin成RTSReplaySyste設置::DeleteReplay(const 軍St本in成& ReplayID)
{
    軍St本in成 軍ilePath = GetReplayDi本ecto本y() / ReplayID + TEXT(".本eplay");
    
    if (I軍ileMana成e本::Get().Delete(*軍ilePath))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Deleted 本eplay: %s"), *ReplayID);
        本et使本n t本使e;
    }
    
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to delete 本eplay: %s"), *ReplayID);
    本et使本n false;
}

bool UMin成RTSReplaySyste設置::Expo本tReplay(const 軍St本in成& ReplayID, const 軍St本in成& 軍ilePath)
{
    軍St本in成 So使本cePath = GetReplayDi本ecto本y() / ReplayID + TEXT(".本eplay");
    
    if (I軍ileMana成e本::Get().Copy(*軍ilePath, *So使本cePath, t本使e, t本使e))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Expo本ted 本eplay %s to %s"), *ReplayID, *軍ilePath);
        本et使本n t本使e;
    }
    
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to expo本t 本eplay %s"), *ReplayID);
    本et使本n false;
}

軍ReplayMetadata UMin成RTSReplaySyste設置::I設置po本tReplay(const 軍St本in成& 軍ilePath)
{
    軍ReplayMetadata Metadata;
    
    if (!I軍ileMana成e本::Get().軍ileExists(*軍ilePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Replay file not fo使nd: %s"), *軍ilePath);
        本et使本n Metadata;
    }
    
    // 生成新的回放ID
    軍St本in成 的ewReplayID = Gene本ateReplayID();
    軍St本in成 DestPath = GetReplayDi本ecto本y() / 的ewReplayID + TEXT(".本eplay");
    
    if (I軍ileMana成e本::Get().Copy(*DestPath, *軍ilePath, t本使e, t本使e))
    {
        // 載入元數據
        LoadReplay軍本o設置軍ile(的ewReplayID);
        Metadata = C使本本entMetadata;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置po本ted 本eplay as %s"), *的ewReplayID);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to i設置po本t 本eplay"));
    }
    
    本et使本n Metadata;
}

EReplayState UMin成RTSReplaySyste設置::GetReplayState() const
{
    本et使本n C使本本entState;
}

float UMin成RTSReplaySyste設置::GetC使本本entPlaybackTi設置e() const
{
    本et使本n C使本本entPlaybackTi設置e;
}

int32 UMin成RTSReplaySyste設置::GetC使本本entPlayback軍本a設置e() const
{
    本et使本n C使本本ent軍本a設置eIndex;
}

float UMin成RTSReplaySyste設置::GetTotalD使本ation() const
{
    本et使本n C使本本entMetadata.D使本ation;
}

正oid UMin成RTSReplaySyste設置::Sta本tPlaybackLoop()
{
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        float TickInte本正al = (1.0f / 30.0f) / PlaybackSpeed;
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(PlaybackTi設置e本輸入andle, this, &UMin成RTSReplaySyste設置::PlaybackTick, TickInte本正al, t本使e);
    }
}

正oid UMin成RTSReplaySyste設置::StopPlaybackLoop()
{
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(PlaybackTi設置e本輸入andle);
    }
}

正oid UMin成RTSReplaySyste設置::PlaybackTick()
{
    if (C使本本ent軍本a設置eIndex >= Reco本ded軍本a設置es.的使設置())
    {
        // 回放完成
        StopPlayback();
        OnPlayback軍inished.B本oadcast();
        本et使本n;
    }
    
    // 處理當前幀
    const 軍Replay軍本a設置e& 軍本a設置e = Reco本ded軍本a設置es[C使本本ent軍本a設置eIndex];
    P本ocess軍本a設置e(軍本a設置e);
    
    // 更新時間
    C使本本entPlaybackTi設置e = 軍本a設置e.Ti設置esta設置p;
    OnPlaybackTi設置eChan成ed.B本oadcast(C使本本entPlaybackTi設置e);
    
    // 處理該幀的事件
    fo本 (const 軍St本in成& E正entID : 軍本a設置e.E正entIDs)
    {
        fo本 (const 軍ReplayE正ent& E正ent : Reco本dedE正ents)
        {
            if (E正ent.E正entID.ToSt本in成() == E正entID)
            {
                P本ocessE正ent(E正ent);
                b本eak;
            }
        }
    }
    
    C使本本ent軍本a設置eIndex++;
}

正oid UMin成RTSReplaySyste設置::Reco本din成Tick()
{
    if (C使本本entState != EReplayState::Reco本din成)
    {
        本et使本n;
    }
    
    // 更新錄製時間
    C使本本entPlaybackTi設置e += 1.0f / 30.0f;
}

軍St本in成 UMin成RTSReplaySyste設置::Gene本ateReplayID()
{
    本et使本n 軍St本in成::P本intf(TEXT("REPLAY下%s"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
}

軍St本in成 UMin成RTSReplaySyste設置::GetReplayDi本ecto本y() const
{
    本et使本n 軍Paths::P本o大ectSa正edDi本() / TEXT("Replays");
}

bool UMin成RTSReplaySyste設置::Sa正eReplayTo軍ile(const 軍St本in成& ReplayID)
{
    軍St本in成 軍ilePath = GetReplayDi本ecto本y() / ReplayID + TEXT(".本eplay");
    
    // 這裡應該實現序列化邏輯
    // 簡化處理，僅作為示例
    
    軍St本in成 JsonData;
    JsonData += 軍St本in成::P本intf(TEXT("{\n"));
    JsonData += 軍St本in成::P本intf(TEXT("  \"ReplayID\": \"%s\",\n"), *C使本本entMetadata.ReplayID);
    JsonData += 軍St本in成::P本intf(TEXT("  \"MatchID\": \"%s\",\n"), *C使本本entMetadata.MatchID);
    JsonData += 軍St本in成::P本intf(TEXT("  \"Map的a設置e\": \"%s\",\n"), *C使本本entMetadata.Map的a設置e);
    JsonData += 軍St本in成::P本intf(TEXT("  \"Ga設置eVe本sion\": \"%s\",\n"), *C使本本entMetadata.Ga設置eVe本sion);
    JsonData += 軍St本in成::P本intf(TEXT("  \"C本eationDate\": \"%s\",\n"), *C使本本entMetadata.C本eationDate);
    JsonData += 軍St本in成::P本intf(TEXT("  \"D使本ation\": %.2f,\n"), C使本本entMetadata.D使本ation);
    JsonData += 軍St本in成::P本intf(TEXT("  \"Total軍本a設置es\": %d,\n"), C使本本entMetadata.Total軍本a設置es);
    JsonData += 軍St本in成::P本intf(TEXT("  \"TotalE正ents\": %d\n"), C使本本entMetadata.TotalE正ents);
    JsonData += 軍St本in成::P本intf(TEXT("}\n"));
    
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JsonData, *軍ilePath);
}

bool UMin成RTSReplaySyste設置::LoadReplay軍本o設置軍ile(const 軍St本in成& ReplayID)
{
    軍St本in成 軍ilePath = GetReplayDi本ecto本y() / ReplayID + TEXT(".本eplay");
    
    if (!I軍ileMana成e本::Get().軍ileExists(*軍ilePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Replay file not fo使nd: %s"), *軍ilePath);
        本et使本n false;
    }
    
    // 這裡應該實現反序列化邏輯
    // 簡化處理，僅作為示例
    
    軍St本in成 JsonData;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonData, *軍ilePath))
    {
        本et使本n false;
    }
    
    // 重置數據
    Reco本ded軍本a設置es.E設置pty();
    Reco本dedE正ents.E設置pty();
    
    // 解析JSO的並填充數據
    // 這裡應該使用JSO的解析器
    
    C使本本entMetadata.ReplayID = ReplayID;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded 本eplay f本o設置 file: %s"), *軍ilePath);
    本et使本n t本使e;
}

正oid UMin成RTSReplaySyste設置::P本ocess軍本a設置e(const 軍Replay軍本a設置e& 軍本a設置e)
{
    // 在這裡處理回放幀數據
    // 例如：重建遊戲狀態、更新單位位置等
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("P本ocessin成 f本a設置e %d at %.2fs"), 軍本a設置e.軍本a設置e的使設置be本, 軍本a設置e.Ti設置esta設置p);
}

正oid UMin成RTSReplaySyste設置::P本ocessE正ent(const 軍ReplayE正ent& E正ent)
{
    // 在這裡處理回放事件
    // 例如：播放特效、顯示提示等
    
    OnReplayE正ent.B本oadcast(E正ent.E正entType, E正ent);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("P本ocessin成 e正ent %s of type %d at %.2fs"), 
        *E正ent.E正entID.ToSt本in成(), (int32)E正ent.E正entType, E正ent.Ti設置esta設置p);
}
