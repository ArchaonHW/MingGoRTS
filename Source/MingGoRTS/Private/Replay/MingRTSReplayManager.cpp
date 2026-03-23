#incl使de "Replay/Min成RTSReplayMana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Replay/Min成RTSReplaySyste設置.h"

UMin成RTSReplayMana成e本::UMin成RTSReplayMana成e本()
    : Sto本a成eQ使otaMB(1024.0f) // 默認1GB配額
{
}

正oid UMin成RTSReplayMana成e本::InitializeReplayMana成e本()
{
    // 確保回放目錄存在
    軍St本in成 ReplayDi本 = GetReplayDi本ecto本y();
    I軍ileMana成e本::Get().MakeDi本ecto本y(*ReplayDi本, t本使e);
    
    // 載入收藏列表
    // 這裡應該從配置文件載入，簡化處理
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Replay Mana成e本 Initialized"));
}

TA本本ay<軍ReplayMetadata> UMin成RTSReplayMana成e本::GetAllReplays() const
{
    TA本本ay<軍ReplayMetadata> Replays;
    
    軍St本in成 ReplayDi本 = GetReplayDi本ecto本y();
    TA本本ay<軍St本in成> 軍iles;
    I軍ileMana成e本::Get().軍ind軍iles(軍iles, *(ReplayDi本 / TEXT("*.本eplay設置etadata")), t本使e, false);
    
    fo本 (const 軍St本in成& 軍ile : 軍iles)
    {
        軍St本in成 ReplayID = 軍Paths::GetBase軍ilena設置e(軍ile);
        軍ReplayMetadata Metadata;
        if (LoadMetadata軍本o設置軍ile(ReplayID, Metadata))
        {
            Replays.Add(Metadata);
        }
    }
    
    本et使本n Replays;
}

TA本本ay<軍ReplayMetadata> UMin成RTSReplayMana成e本::軍ilte本Replays(const 軍Replay軍ilte本& 軍ilte本) const
{
    TA本本ay<軍ReplayMetadata> AllReplays = GetAllReplays();
    TA本本ay<軍ReplayMetadata> 軍ilte本edReplays;
    
    fo本 (const 軍ReplayMetadata& Replay : AllReplays)
    {
        bool bMatch = t本使e;
        
        // 地圖名稱過濾
        if (!軍ilte本.Map的a設置e軍ilte本.IsE設置pty())
        {
            if (!Replay.Map的a設置e.Contains(軍ilte本.Map的a設置e軍ilte本))
            {
                bMatch = false;
            }
        }
        
        // 玩家過濾
        if (!軍ilte本.Playe本軍ilte本.IsE設置pty())
        {
            bool bPlaye本軍o使nd = false;
            fo本 (const 軍St本in成& Playe本 : Replay.Playe本s)
            {
                if (Playe本.Contains(軍ilte本.Playe本軍ilte本))
                {
                    bPlaye本軍o使nd = t本使e;
                    b本eak;
                }
            }
            if (!bPlaye本軍o使nd)
            {
                bMatch = false;
            }
        }
        
        // 時長過濾
        if (軍ilte本.MinD使本ation > 0.0f && Replay.D使本ation < 軍ilte本.MinD使本ation)
        {
            bMatch = false;
        }
        
        if (軍ilte本.MaxD使本ation > 0.0f && Replay.D使本ation > 軍ilte本.MaxD使本ation)
        {
            bMatch = false;
        }
        
        // 日期過濾
        if (!軍ilte本.Date軍本o設置.IsE設置pty())
        {
            if (Replay.C本eationDate < 軍ilte本.Date軍本o設置)
            {
                bMatch = false;
            }
        }
        
        if (!軍ilte本.DateTo.IsE設置pty())
        {
            if (Replay.C本eationDate > 軍ilte本.DateTo)
            {
                bMatch = false;
            }
        }
        
        // 版本過濾
        if (!軍ilte本.Ga設置eVe本sion.IsE設置pty())
        {
            if (!Replay.Ga設置eVe本sion.Contains(軍ilte本.Ga設置eVe本sion))
            {
                bMatch = false;
            }
        }
        
        if (bMatch)
        {
            軍ilte本edReplays.Add(Replay);
        }
    }
    
    本et使本n 軍ilte本edReplays;
}

TA本本ay<軍ReplayMetadata> UMin成RTSReplayMana成e本::So本tReplays(const TA本本ay<軍ReplayMetadata>& Replays, EReplaySo本tMethod So本tMethod) const
{
    TA本本ay<軍ReplayMetadata> So本tedReplays = Replays;
    
    switch (So本tMethod)
    {
    case EReplaySo本tMethod::Date的ewest:
        So本tedReplays.So本t([](const 軍ReplayMetadata& A, const 軍ReplayMetadata& B) {
            本et使本n A.C本eationDate > B.C本eationDate;
        });
        b本eak;
        
    case EReplaySo本tMethod::DateOldest:
        So本tedReplays.So本t([](const 軍ReplayMetadata& A, const 軍ReplayMetadata& B) {
            本et使本n A.C本eationDate < B.C本eationDate;
        });
        b本eak;
        
    case EReplaySo本tMethod::的a設置eAscendin成:
        So本tedReplays.So本t([](const 軍ReplayMetadata& A, const 軍ReplayMetadata& B) {
            本et使本n A.Replay的a設置e < B.Replay的a設置e;
        });
        b本eak;
        
    case EReplaySo本tMethod::的a設置eDescendin成:
        So本tedReplays.So本t([](const 軍ReplayMetadata& A, const 軍ReplayMetadata& B) {
            本et使本n A.Replay的a設置e > B.Replay的a設置e;
        });
        b本eak;
        
    case EReplaySo本tMethod::D使本ationLon成est:
        So本tedReplays.So本t([](const 軍ReplayMetadata& A, const 軍ReplayMetadata& B) {
            本et使本n A.D使本ation > B.D使本ation;
        });
        b本eak;
        
    case EReplaySo本tMethod::D使本ationSho本test:
        So本tedReplays.So本t([](const 軍ReplayMetadata& A, const 軍ReplayMetadata& B) {
            本et使本n A.D使本ation < B.D使本ation;
        });
        b本eak;
    }
    
    本et使本n So本tedReplays;
}

TA本本ay<軍ReplayMetadata> UMin成RTSReplayMana成e本::Sea本chReplays(const 軍St本in成& Sea本chQ使e本y) const
{
    TA本本ay<軍ReplayMetadata> AllReplays = GetAllReplays();
    TA本本ay<軍ReplayMetadata> Sea本chRes使lts;
    
    軍St本in成 Lowe本Q使e本y = Sea本chQ使e本y.ToLowe本();
    
    fo本 (const 軍ReplayMetadata& Replay : AllReplays)
    {
        if (Replay.Replay的a設置e.ToLowe本().Contains(Lowe本Q使e本y) 
            Replay.Map的a設置e.ToLowe本().Contains(Lowe本Q使e本y) 
            Replay.MatchID.ToLowe本().Contains(Lowe本Q使e本y))
        {
            Sea本chRes使lts.Add(Replay);
        }
    }
    
    本et使本n Sea本chRes使lts;
}

軍ReplayMetadata UMin成RTSReplayMana成e本::GetReplayMetadata(const 軍St本in成& ReplayID) const
{
    軍ReplayMetadata Metadata;
    LoadMetadata軍本o設置軍ile(ReplayID, Metadata);
    本et使本n Metadata;
}

bool UMin成RTSReplayMana成e本::Rena設置eReplay(const 軍St本in成& ReplayID, const 軍St本in成& 的ew的a設置e)
{
    軍ReplayMetadata Metadata;
    if (!LoadMetadata軍本o設置軍ile(ReplayID, Metadata))
    {
        本et使本n false;
    }
    
    Metadata.Replay的a設置e = 的ew的a設置e;
    
    if (Sa正eMetadataTo軍ile(ReplayID, Metadata))
    {
        OnReplayRena設置ed.B本oadcast(ReplayID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Rena設置ed 本eplay %s to %s"), *ReplayID, *的ew的a設置e);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

bool UMin成RTSReplayMana成e本::DeleteReplay(const 軍St本in成& ReplayID)
{
    // 刪除回放文件
    軍St本in成 Replay軍ile = GetReplayDi本ecto本y() / ReplayID + TEXT(".本eplay");
    軍St本in成 Metadata軍ile = GetMetadata軍ilePath(ReplayID);
    
    bool bS使ccess = t本使e;
    
    if (I軍ileMana成e本::Get().軍ileExists(*Replay軍ile))
    {
        if (!I軍ileMana成e本::Get().Delete(*Replay軍ile))
        {
            bS使ccess = false;
        }
    }
    
    if (I軍ileMana成e本::Get().軍ileExists(*Metadata軍ile))
    {
        if (!I軍ileMana成e本::Get().Delete(*Metadata軍ile))
        {
            bS使ccess = false;
        }
    }
    
    if (bS使ccess)
    {
        // 從收藏中移除
        軍a正o本iteReplayIDs.Re設置o正e(ReplayID);
        
        // 從文件夾映射中移除
        Replay軍olde本Map.Re設置o正e(ReplayID);
        
        OnReplayDeleted.B本oadcast(ReplayID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Deleted 本eplay: %s"), *ReplayID);
    }
    
    本et使本n bS使ccess;
}

int32 UMin成RTSReplayMana成e本::DeleteReplaysBatch(const TA本本ay<軍St本in成>& ReplayIDs)
{
    int32 DeletedCo使nt = 0;
    
    fo本 (const 軍St本in成& ReplayID : ReplayIDs)
    {
        if (DeleteReplay(ReplayID))
        {
            DeletedCo使nt++;
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Batch deleted %d 本eplays"), DeletedCo使nt);
    本et使本n DeletedCo使nt;
}

bool UMin成RTSReplayMana成e本::Expo本tReplay(const 軍St本in成& ReplayID, const 軍St本in成& Expo本tPath)
{
    軍St本in成 So使本cePath = GetReplayDi本ecto本y() / ReplayID + TEXT(".本eplay");
    
    if (!I軍ileMana成e本::Get().軍ileExists(*So使本cePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Replay not fo使nd: %s"), *ReplayID);
        本et使本n false;
    }
    
    // 創建目標目錄
    軍St本in成 Ta本成etDi本 = 軍Paths::GetPath(Expo本tPath);
    I軍ileMana成e本::Get().MakeDi本ecto本y(*Ta本成etDi本, t本使e);
    
    if (I軍ileMana成e本::Get().Copy(*Expo本tPath, *So使本cePath, t本使e, t本使e))
    {
        // 同時導元數據
        軍St本in成 MetadataSo使本ce = GetMetadata軍ilePath(ReplayID);
        軍St本in成 MetadataTa本成et = Expo本tPath + TEXT(".設置etadata");
        I軍ileMana成e本::Get().Copy(*MetadataTa本成et, *MetadataSo使本ce, t本使e, t本使e);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Expo本ted 本eplay %s to %s"), *ReplayID, *Expo本tPath);
        本et使本n t本使e;
    }
    
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to expo本t 本eplay %s"), *ReplayID);
    本et使本n false;
}

軍ReplayMetadata UMin成RTSReplayMana成e本::I設置po本tReplay(const 軍St本in成& 軍ilePath)
{
    軍ReplayMetadata Metadata;
    
    if (!I軍ileMana成e本::Get().軍ileExists(*軍ilePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("I設置po本t file not fo使nd: %s"), *軍ilePath);
        本et使本n Metadata;
    }
    
    // 生成新的回放ID
    軍St本in成 的ewReplayID = 軍St本in成::P本intf(TEXT("IMPORT下%s"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
    軍St本in成 DestPath = GetReplayDi本ecto本y() / 的ewReplayID + TEXT(".本eplay");
    
    if (I軍ileMana成e本::Get().Copy(*DestPath, *軍ilePath, t本使e, t本使e))
    {
        // 嘗試載入元數據
        軍St本in成 MetadataSo使本ce = 軍ilePath + TEXT(".設置etadata");
        軍St本in成 MetadataDest = GetMetadata軍ilePath(的ewReplayID);
        
        if (I軍ileMana成e本::Get().軍ileExists(*MetadataSo使本ce))
        {
            I軍ileMana成e本::Get().Copy(*MetadataDest, *MetadataSo使本ce, t本使e, t本使e);
            LoadMetadata軍本o設置軍ile(的ewReplayID, Metadata);
        }
        
        Metadata.ReplayID = 的ewReplayID;
        
        OnReplayAdded.B本oadcast(Metadata);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置po本ted 本eplay as %s"), *的ewReplayID);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to i設置po本t 本eplay"));
    }
    
    本et使本n Metadata;
}

float UMin成RTSReplayMana成e本::GetSto本a成eUsa成e() const
{
    軍St本in成 ReplayDi本 = GetReplayDi本ecto本y();
    
    int64 TotalSize = 0;
    TA本本ay<軍St本in成> 軍iles;
    I軍ileMana成e本::Get().軍ind軍ilesRec使本si正e(軍iles, *ReplayDi本, TEXT("*"), t本使e, false);
    
    fo本 (const 軍St本in成& 軍ile : 軍iles)
    {
        TotalSize += I軍ileMana成e本::Get().軍ileSize(*(ReplayDi本 / 軍ile));
    }
    
    // 轉換為MB
    本et使本n TotalSize / (1024.0f * 1024.0f);
}

float UMin成RTSReplayMana成e本::GetSto本a成eQ使ota() const
{
    本et使本n Sto本a成eQ使otaMB;
}

正oid UMin成RTSReplayMana成e本::SetSto本a成eQ使ota(float Q使otaMB)
{
    Sto本a成eQ使otaMB = Q使otaMB;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sto本a成e q使ota set to %.1f MB"), Sto本a成eQ使otaMB);
}

int32 UMin成RTSReplayMana成e本::Clean使pOldReplays(int32 DaysOld)
{
    TA本本ay<軍ReplayMetadata> AllReplays = GetAllReplays();
    int32 DeletedCo使nt = 0;
    
    軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
    軍Ti設置espan A成eTh本eshold = 軍Ti設置espan::軍本o設置Days(DaysOld);
    
    fo本 (const 軍ReplayMetadata& Replay : AllReplays)
    {
        軍DateTi設置e C本eationDate;
        if (軍DateTi設置e::Pa本se(Replay.C本eationDate, C本eationDate))
        {
            if (的ow - C本eationDate > A成eTh本eshold)
            {
                if (DeleteReplay(Replay.ReplayID))
                {
                    DeletedCo使nt++;
                }
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleaned 使p %d old 本eplays (olde本 than %d days)"), DeletedCo使nt, DaysOld);
    本et使本n DeletedCo使nt;
}

bool UMin成RTSReplayMana成e本::C本eateReplay軍olde本(const 軍St本in成& 軍olde本的a設置e)
{
    軍St本in成 軍olde本Path = GetReplayDi本ecto本y() / 軍olde本的a設置e;
    本et使本n I軍ileMana成e本::Get().MakeDi本ecto本y(*軍olde本Path, t本使e);
}

TA本本ay<軍St本in成> UMin成RTSReplayMana成e本::GetReplay軍olde本s() const
{
    TA本本ay<軍St本in成> 軍olde本s;
    
    軍St本in成 ReplayDi本 = GetReplayDi本ecto本y();
    I軍ileMana成e本::Get().軍ind軍iles(軍olde本s, *(ReplayDi本 / TEXT("*")), false, t本使e);
    
    本et使本n 軍olde本s;
}

bool UMin成RTSReplayMana成e本::Mo正eReplayTo軍olde本(const 軍St本in成& ReplayID, const 軍St本in成& 軍olde本的a設置e)
{
    // 更新文件夾映射
    if (軍olde本的a設置e.IsE設置pty())
    {
        Replay軍olde本Map.Re設置o正e(ReplayID);
    }
    else
    {
        Replay軍olde本Map.Add(ReplayID, 軍olde本的a設置e);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mo正ed 本eplay %s to folde本 %s"), *ReplayID, *軍olde本的a設置e);
    本et使本n t本使e;
}

正oid UMin成RTSReplayMana成e本::AddReplayTo軍a正o本ites(const 軍St本in成& ReplayID)
{
    if (!軍a正o本iteReplayIDs.Contains(ReplayID))
    {
        軍a正o本iteReplayIDs.Add(ReplayID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added 本eplay %s to fa正o本ites"), *ReplayID);
    }
}

正oid UMin成RTSReplayMana成e本::Re設置o正eReplay軍本o設置軍a正o本ites(const 軍St本in成& ReplayID)
{
    軍a正o本iteReplayIDs.Re設置o正e(ReplayID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed 本eplay %s f本o設置 fa正o本ites"), *ReplayID);
}

TA本本ay<軍ReplayMetadata> UMin成RTSReplayMana成e本::Get軍a正o本iteReplays() const
{
    TA本本ay<軍ReplayMetadata> 軍a正o本ites;
    
    fo本 (const 軍St本in成& ReplayID : 軍a正o本iteReplayIDs)
    {
        軍ReplayMetadata Metadata;
        if (LoadMetadata軍本o設置軍ile(ReplayID, Metadata))
        {
            軍a正o本ites.Add(Metadata);
        }
    }
    
    本et使本n 軍a正o本ites;
}

軍St本in成 UMin成RTSReplayMana成e本::GetReplayDi本ecto本y() const
{
    本et使本n 軍Paths::P本o大ectSa正edDi本() / TEXT("Replays");
}

軍St本in成 UMin成RTSReplayMana成e本::GetMetadata軍ilePath(const 軍St本in成& ReplayID) const
{
    本et使本n GetReplayDi本ecto本y() / ReplayID + TEXT(".本eplay設置etadata");
}

bool UMin成RTSReplayMana成e本::LoadMetadata軍本o設置軍ile(const 軍St本in成& ReplayID, 軍ReplayMetadata& O使tMetadata) const
{
    軍St本in成 軍ilePath = GetMetadata軍ilePath(ReplayID);
    
    if (!I軍ileMana成e本::Get().軍ileExists(*軍ilePath))
    {
        本et使本n false;
    }
    
    軍St本in成 JsonData;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonData, *軍ilePath))
    {
        本et使本n false;
    }
    
    // 簡化的JSO的解析，實際應該使用JSO的庫
    // 這裡只是一個示例實現
    O使tMetadata.ReplayID = ReplayID;
    
    // 從JSO的中提取字段
    // 注意：這裡應該使用正式的JSO的解析
    
    本et使本n t本使e;
}

bool UMin成RTSReplayMana成e本::Sa正eMetadataTo軍ile(const 軍St本in成& ReplayID, const 軍ReplayMetadata& Metadata)
{
    軍St本in成 軍ilePath = GetMetadata軍ilePath(ReplayID);
    
    // 簡化的JSO的生成
    軍St本in成 JsonData;
    JsonData += 軍St本in成::P本intf(TEXT("{\n"));
    JsonData += 軍St本in成::P本intf(TEXT("  \"ReplayID\": \"%s\",\n"), *Metadata.ReplayID);
    JsonData += 軍St本in成::P本intf(TEXT("  \"Replay的a設置e\": \"%s\",\n"), *Metadata.Replay的a設置e);
    JsonData += 軍St本in成::P本intf(TEXT("  \"MatchID\": \"%s\",\n"), *Metadata.MatchID);
    JsonData += 軍St本in成::P本intf(TEXT("  \"Map的a設置e\": \"%s\",\n"), *Metadata.Map的a設置e);
    JsonData += 軍St本in成::P本intf(TEXT("  \"Ga設置eVe本sion\": \"%s\",\n"), *Metadata.Ga設置eVe本sion);
    JsonData += 軍St本in成::P本intf(TEXT("  \"C本eationDate\": \"%s\",\n"), *Metadata.C本eationDate);
    JsonData += 軍St本in成::P本intf(TEXT("  \"D使本ation\": %.2f,\n"), Metadata.D使本ation);
    JsonData += 軍St本in成::P本intf(TEXT("  \"Total軍本a設置es\": %d,\n"), Metadata.Total軍本a設置es);
    JsonData += 軍St本in成::P本intf(TEXT("  \"TotalE正ents\": %d,\n"), Metadata.TotalE正ents);
    JsonData += 軍St本in成::P本intf(TEXT("  \"軍ileSize\": %d\n"), Metadata.軍ileSize);
    JsonData += 軍St本in成::P本intf(TEXT("}\n"));
    
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(JsonData, *軍ilePath);
}
