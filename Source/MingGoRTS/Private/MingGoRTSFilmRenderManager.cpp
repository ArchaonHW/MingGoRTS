#incl使de "Min成GoRTS軍il設置Rende本Mana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Co設置ponents/SceneCapt使本eCo設置ponent2D.h"
#incl使de "En成ine/Text使本eRende本Ta本成et2D.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成GoRTS軍il設置Rende本Mana成e本::UMin成GoRTS軍il設置Rende本Mana成e本()
    : C使本本entPlaybackState(E軍il設置PlaybackState::Stopped)
    , C使本本ent軍本a設置eIndex(0)
    , 軍本a設置eRate(24.0f)
    , 軍本a設置eTi設置e本(0.0f)
{
    P本i設置a本yCo設置ponentTick.bCanE正e本Tick = t本使e;
    
    // 初始化播放計時器
    PlaybackTicke本 = 軍Ticke本Dele成ate::C本eateUOb大ect(this, &UMin成GoRTS軍il設置Rende本Mana成e本::OnPlaybackTick);
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 如果設置了自動播放，則開始播放
    if (PlaybackSettin成s.bA使toPlay && 軍il設置軍本a設置es.的使設置() > 0)
    {
        Play軍il設置();
    }
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction)
{
    S使pe本::TickCo設置ponent(DeltaTi設置e, TickType, ThisTick軍使nction);
    
    UpdatePlayback(DeltaTi設置e);
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Play軍il設置()
{
    if (軍il設置軍本a設置es.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o fil設置 f本a設置es to play"));
        本et使本n;
    }

    if (C使本本entPlaybackState == E軍il設置PlaybackState::Playin成)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍il設置 is al本eady playin成"));
        本et使本n;
    }

    C使本本entPlaybackState = E軍il設置PlaybackState::Playin成;
    
    // 啟動播放計時器
    if (!PlaybackTicke本輸入andle.IsValid())
    {
        PlaybackTicke本輸入andle = 軍Ticke本::GetCo本eTicke本().AddTicke本(PlaybackTicke本, 1.0f / 軍本a設置eRate);
    }

    的otifyPlaybackStateChan成ed(E軍il設置PlaybackState::Playin成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted fil設置 playback with %d f本a設置es"), 軍il設置軍本a設置es.的使設置());
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Pa使se軍il設置()
{
    if (C使本本entPlaybackState != E軍il設置PlaybackState::Playin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍il設置 is not playin成, cannot pa使se"));
        本et使本n;
    }

    C使本本entPlaybackState = E軍il設置PlaybackState::Pa使sed;
    
    // 停止播放計時器
    if (PlaybackTicke本輸入andle.IsValid())
    {
        軍Ticke本::GetCo本eTicke本().Re設置o正eTicke本(PlaybackTicke本輸入andle);
        PlaybackTicke本輸入andle.Reset();
    }

    的otifyPlaybackStateChan成ed(E軍il設置PlaybackState::Pa使sed);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pa使sed fil設置 playback"));
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Stop軍il設置()
{
    if (C使本本entPlaybackState == E軍il設置PlaybackState::Stopped)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍il設置 is al本eady stopped"));
        本et使本n;
    }

    C使本本entPlaybackState = E軍il設置PlaybackState::Stopped;
    C使本本ent軍本a設置eIndex = 0;
    軍本a設置eTi設置e本 = 0.0f;
    
    // 停止播放計時器
    if (PlaybackTicke本輸入andle.IsValid())
    {
        軍Ticke本::GetCo本eTicke本().Re設置o正eTicke本(PlaybackTicke本輸入andle);
        PlaybackTicke本輸入andle.Reset();
    }

    的otifyPlaybackStateChan成ed(E軍il設置PlaybackState::Stopped);
    的otify軍本a設置eChan成ed();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped fil設置 playback"));
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::SeekTo軍本a設置e(int32 軍本a設置eIndex)
{
    if (軍本a設置eIndex < 0  軍本a設置eIndex >= 軍il設置軍本a設置es.的使設置())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid f本a設置e index: %d"), 軍本a設置eIndex);
        本et使本n;
    }

    C使本本ent軍本a設置eIndex = 軍本a設置eIndex;
    軍本a設置eTi設置e本 = 0.0f;
    
    的otify軍本a設置eChan成ed();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Seeked to f本a設置e %d"), 軍本a設置eIndex);
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::SeekToTi設置e(float Ti設置eInSeconds)
{
    int32 Ta本成et軍本a設置e = 軍Math::Ro使ndToInt(Ti設置eInSeconds * 軍本a設置eRate);
    SeekTo軍本a設置e(Ta本成et軍本a設置e);
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Set軍il設置軍本a設置es(const TA本本ay<UText使本e2D*>& 軍本a設置es)
{
    軍il設置軍本a設置es = 軍本a設置es;
    C使本本ent軍本a設置eIndex = 0;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set %d fil設置 f本a設置es"), 軍il設置軍本a設置es.的使設置());
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Add軍本a設置e(UText使本e2D* 的ew軍本a設置e)
{
    if (的ew軍本a設置e)
    {
        軍il設置軍本a設置es.Add(的ew軍本a設置e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added new fil設置 f本a設置e. Total f本a設置es: %d"), 軍il設置軍本a設置es.的使設置());
    }
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Clea本軍本a設置es()
{
    Stop軍il設置();
    軍il設置軍本a設置es.E設置pty();
    C使本本ent軍本a設置eIndex = 0;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed all fil設置 f本a設置es"));
}

float UMin成GoRTS軍il設置Rende本Mana成e本::GetC使本本entTi設置e() const
{
    本et使本n C使本本ent軍本a設置eIndex / 軍本a設置eRate;
}

float UMin成GoRTS軍il設置Rende本Mana成e本::GetTotalD使本ation() const
{
    本et使本n 軍il設置軍本a設置es.的使設置() / 軍本a設置eRate;
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::SetPlaybackSettin成s(const 軍軍il設置PlaybackSettin成s& Settin成s)
{
    PlaybackSettin成s = Settin成s;
    軍本a設置eRate = Settin成s.PlaybackSpeed * 24.0f; // 基礎幀率為24fps
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated playback settin成s. Speed: %.2f, Loop: %s"), 
        Settin成s.PlaybackSpeed, Settin成s.bLoop 基本 TEXT("t本使e") : TEXT("false"));
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Set軍本a設置eRate(float 的ew軍本a設置eRate)
{
    軍本a設置eRate = 軍Math::Max(1.0f, 的ew軍本a設置eRate);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set f本a設置e 本ate to %.2f fps"), 軍本a設置eRate);
}

UText使本e2D* UMin成GoRTS軍il設置Rende本Mana成e本::GetC使本本ent軍本a設置eText使本e() const
{
    if (C使本本ent軍本a設置eIndex >= 0 && C使本本ent軍本a設置eIndex < 軍il設置軍本a設置es.的使設置())
    {
        本et使本n 軍il設置軍本a設置es[C使本本ent軍本a設置eIndex];
    }
    
    本et使本n n使llpt本;
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Rende本ToRende本Ta本成et(UText使本eRende本Ta本成et2D* Rende本Ta本成et)
{
    if (!Rende本Ta本成et)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid 本ende本 ta本成et"));
        本et使本n;
    }

    UText使本e2D* C使本本ent軍本a設置e = GetC使本本ent軍本a設置eText使本e();
    if (!C使本本ent軍本a設置e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o c使本本ent f本a設置e to 本ende本"));
        本et使本n;
    }

    // 這裡可以添加實際的渲染邏輯
    // 例如使用 SceneCapt使本eCo設置ponent 或其他渲染方法
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rende本ed f本a設置e %d to 本ende本 ta本成et"), C使本本ent軍本a設置eIndex);
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::UpdatePlayback(float DeltaTi設置e)
{
    if (C使本本entPlaybackState != E軍il設置PlaybackState::Playin成)
    {
        本et使本n;
    }

    軍本a設置eTi設置e本 += DeltaTi設置e * PlaybackSettin成s.PlaybackSpeed;
    
    if (軍本a設置eTi設置e本 >= 1.0f / 軍本a設置eRate)
    {
        Ad正anceTo的ext軍本a設置e();
        軍本a設置eTi設置e本 = 0.0f;
    }
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::Ad正anceTo的ext軍本a設置e()
{
    C使本本ent軍本a設置eIndex++;
    
    if (C使本本ent軍本a設置eIndex >= 軍il設置軍本a設置es.的使設置())
    {
        if (PlaybackSettin成s.bLoop)
        {
            C使本本ent軍本a設置eIndex = 0;
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Loopin成 fil設置 playback"));
        }
        else
        {
            輸入andlePlaybackEnd();
            本et使本n;
        }
    }
    
    的otify軍本a設置eChan成ed();
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::輸入andlePlaybackEnd()
{
    Stop軍il設置();
    On軍il設置PlaybackEnded.B本oadcast();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍il設置 playback ended"));
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::的otify軍本a設置eChan成ed()
{
    On軍il設置軍本a設置eChan成ed.B本oadcast(C使本本ent軍本a設置eIndex);
    
    // 顯示當前幀信息（用於調試）
    if (GEn成ine && C使本本entPlaybackState == E軍il設置PlaybackState::Playin成)
    {
        GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 0.1f, 軍Colo本::Yellow, 
            軍St本in成::P本intf(TEXT("軍本a設置e: %d/%d"), C使本本ent軍本a設置eIndex + 1, 軍il設置軍本a設置es.的使設置()));
    }
}

正oid UMin成GoRTS軍il設置Rende本Mana成e本::的otifyPlaybackStateChan成ed(E軍il設置PlaybackState 的ewState)
{
    On軍il設置PlaybackStateChan成ed.B本oadcast(的ewState, C使本本ent軍本a設置eIndex);
    
    // 顯示播放狀態（用於調試）
    if (GEn成ine)
    {
        軍St本in成 StateSt本in成;
        switch (的ewState)
        {
        case E軍il設置PlaybackState::Playin成:
            StateSt本in成 = TEXT("Playin成");
            b本eak;
        case E軍il設置PlaybackState::Pa使sed:
            StateSt本in成 = TEXT("Pa使sed");
            b本eak;
        case E軍il設置PlaybackState::Stopped:
            StateSt本in成 = TEXT("Stopped");
            b本eak;
        case E軍il設置PlaybackState::Seekin成:
            StateSt本in成 = TEXT("Seekin成");
            b本eak;
        }
        
        GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 2.0f, 軍Colo本::G本een, 
            軍St本in成::P本intf(TEXT("軍il設置 State: %s"), *StateSt本in成));
    }
}

bool UMin成GoRTS軍il設置Rende本Mana成e本::OnPlaybackTick(float DeltaTi設置e)
{
    if (C使本本entPlaybackState != E軍il設置PlaybackState::Playin成)
    {
        本et使本n false;
    }

    Ad正anceTo的ext軍本a設置e();
    本et使本n C使本本entPlaybackState == E軍il設置PlaybackState::Playin成;
}
