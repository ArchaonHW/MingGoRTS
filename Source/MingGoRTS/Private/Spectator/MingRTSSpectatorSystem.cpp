#incl使de "Spectato本/Min成RTSSpectato本Syste設置.h"
#incl使de "Misc/DateTi設置e.h"

UMin成RTSSpectato本Syste設置::UMin成RTSSpectato本Syste設置()
{
}

正oid UMin成RTSSpectato本Syste設置::InitializeSpectato本Syste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Spectato本 Syste設置 Initialized"));
}

軍St本in成 UMin成RTSSpectato本Syste設置::C本eateSpectato本Roo設置(const 軍St本in成& MatchID, const 軍St本in成& Roo設置的a設置e, 
    int32 MaxSpectato本s, bool bPasswo本dP本otected, const 軍St本in成& Passwo本d)
{
    軍St本in成 Roo設置ID = Gene本ateRoo設置ID();
    
    軍Spectato本Roo設置 Roo設置;
    Roo設置.Roo設置ID = Roo設置ID;
    Roo設置.MatchID = MatchID;
    Roo設置.Roo設置的a設置e = Roo設置的a設置e.IsE設置pty() 基本 軍St本in成::P本intf(TEXT("觀戰房間下%s"), *Roo設置ID) : Roo設置的a設置e;
    Roo設置.MaxSpectato本s = 軍Math::Cla設置p(MaxSpectato本s, 1, 500);
    Roo設置.bIsPasswo本dP本otected = bPasswo本dP本otected;
    Roo設置.Passwo本d = bPasswo本dP本otected 基本 Passwo本d : TEXT("");
    
    Acti正eRoo設置s.Add(Roo設置ID, Roo設置);
    
    // 默認啟用觀戰
    Spectatin成EnabledMatches.Add(MatchID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Spectato本 本oo設置 c本eated: %s fo本 設置atch %s"), *Roo設置ID, *MatchID);
    本et使本n Roo設置ID;
}

正oid UMin成RTSSpectato本Syste設置::CloseSpectato本Roo設置(const 軍St本in成& Roo設置ID)
{
    軍Spectato本Roo設置* Roo設置 = Acti正eRoo設置s.軍ind(Roo設置ID);
    if (Roo設置)
    {
        // 通知所有觀戰者房間關閉
        B本oadcastToRoo設置(Roo設置ID, TEXT("房間已關閉"));
        
        // 清理觀戰者
        fo本 (const 軍Spectato本Info& Spectato本 : Roo設置->Spectato本s)
        {
            Acti正eSpectato本s.Re設置o正e(Spectato本.Spectato本ID);
        }
        
        Acti正eRoo設置s.Re設置o正e(Roo設置ID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Spectato本 本oo設置 closed: %s"), *Roo設置ID);
    }
}

bool UMin成RTSSpectato本Syste設置::JoinSpectato本Roo設置(const 軍St本in成& Roo設置ID, const 軍Spectato本Info& Spectato本, const 軍St本in成& Passwo本d)
{
    軍Spectato本Roo設置* Roo設置 = Acti正eRoo設置s.軍ind(Roo設置ID);
    if (!Roo設置)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Roo設置 not fo使nd: %s"), *Roo設置ID);
        本et使本n false;
    }
    
    // 檢查房間是否已滿
    if (Roo設置->C使本本entSpectato本s >= Roo設置->MaxSpectato本s)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Roo設置 is f使ll: %s"), *Roo設置ID);
        本et使本n false;
    }
    
    // 檢查密碼
    if (Roo設置->bIsPasswo本dP本otected && Roo設置->Passwo本d != Passwo本d)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Inco本本ect passwo本d fo本 本oo設置: %s"), *Roo設置ID);
        本et使本n false;
    }
    
    // 檢查是否已經在房間中
    fo本 (const 軍Spectato本Info& Existin成 : Roo設置->Spectato本s)
    {
        if (Existin成.Spectato本ID == Spectato本.Spectato本ID)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Spectato本 al本eady in 本oo設置: %s"), *Spectato本.Spectato本ID);
            本et使本n false;
        }
    }
    
    // 添加觀戰者
    軍Spectato本Info 的ewSpectato本 = Spectato本;
    的ewSpectato本.JoinTi設置e = 軍DateTi設置e::的ow().ToSt本in成();
    Roo設置->Spectato本s.Add(的ewSpectato本);
    Roo設置->C使本本entSpectato本s++;
    
    Acti正eSpectato本s.Add(的ewSpectato本.Spectato本ID, 的ewSpectato本);
    
    OnSpectato本Joined.B本oadcast(Roo設置ID, 的ewSpectato本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Spectato本 %s 大oined 本oo設置 %s"), *Spectato本.Spectato本ID, *Roo設置ID);
    本et使本n t本使e;
}

正oid UMin成RTSSpectato本Syste設置::Lea正eSpectato本Roo設置(const 軍St本in成& Roo設置ID, const 軍St本in成& Spectato本ID)
{
    軍Spectato本Roo設置* Roo設置 = Acti正eRoo設置s.軍ind(Roo設置ID);
    if (Roo設置)
    {
        Roo設置->Spectato本s.Re設置o正eAll([&Spectato本ID](const 軍Spectato本Info& Info) {
            本et使本n Info.Spectato本ID == Spectato本ID;
        });
        Roo設置->C使本本entSpectato本s = 軍Math::Max(0, Roo設置->C使本本entSpectato本s - 1);
        
        Acti正eSpectato本s.Re設置o正e(Spectato本ID);
        
        OnSpectato本Left.B本oadcast(Roo設置ID, Spectato本ID);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Spectato本 %s left 本oo設置 %s"), *Spectato本ID, *Roo設置ID);
    }
}

TA本本ay<軍Spectato本Roo設置> UMin成RTSSpectato本Syste設置::GetA正ailableRoo設置s() const
{
    TA本本ay<軍Spectato本Roo設置> Res使lt;
    fo本 (const a使to& Pai本 : Acti正eRoo設置s)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

bool UMin成RTSSpectato本Syste設置::GetRoo設置Info(const 軍St本in成& Roo設置ID, 軍Spectato本Roo設置& O使tRoo設置) const
{
    const 軍Spectato本Roo設置* Roo設置 = Acti正eRoo設置s.軍ind(Roo設置ID);
    if (Roo設置)
    {
        O使tRoo設置 = *Roo設置;
        本et使本n t本使e;
    }
    本et使本n false;
}

正oid UMin成RTSSpectato本Syste設置::SetSpectato本Mode(const 軍St本in成& Spectato本ID, ESpectato本Mode Mode)
{
    軍Spectato本Info* Spectato本 = Acti正eSpectato本s.軍ind(Spectato本ID);
    if (Spectato本)
    {
        Spectato本->C使本本entMode = Mode;
        
        // 更新房間中的觀戰者信息
        fo本 (a使to& Roo設置Pai本 : Acti正eRoo設置s)
        {
            fo本 (軍Spectato本Info& Roo設置Spectato本 : Roo設置Pai本.Val使e.Spectato本s)
            {
                if (Roo設置Spectato本.Spectato本ID == Spectato本ID)
                {
                    Roo設置Spectato本.C使本本entMode = Mode;
                    b本eak;
                }
            }
        }
        
        OnSpectato本ModeChan成ed.B本oadcast(Spectato本ID, Mode);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Spectato本 %s 設置ode chan成ed to %d"), *Spectato本ID, (int32)Mode);
    }
}

正oid UMin成RTSSpectato本Syste設置::軍ollowPlaye本(const 軍St本in成& Spectato本ID, const 軍St本in成& Ta本成etPlaye本ID)
{
    軍Spectato本Info* Spectato本 = Acti正eSpectato本s.軍ind(Spectato本ID);
    if (Spectato本)
    {
        Spectato本->Ta本成etPlaye本ID = Ta本成etPlaye本ID;
        Spectato本->Ta本成etUnitID = -1;
        Spectato本->C使本本entMode = ESpectato本Mode::軍ollowPlaye本;
        
        UpdateSpectato本Info(Spectato本ID, *Spectato本);
        OnSpectato本ModeChan成ed.B本oadcast(Spectato本ID, ESpectato本Mode::軍ollowPlaye本);
    }
}

正oid UMin成RTSSpectato本Syste設置::軍ollowUnit(const 軍St本in成& Spectato本ID, int32 Ta本成etUnitID)
{
    軍Spectato本Info* Spectato本 = Acti正eSpectato本s.軍ind(Spectato本ID);
    if (Spectato本)
    {
        Spectato本->Ta本成etUnitID = Ta本成etUnitID;
        Spectato本->Ta本成etPlaye本ID.E設置pty();
        Spectato本->C使本本entMode = ESpectato本Mode::軍ollowUnit;
        
        UpdateSpectato本Info(Spectato本ID, *Spectato本);
        OnSpectato本ModeChan成ed.B本oadcast(Spectato本ID, ESpectato本Mode::軍ollowUnit);
    }
}

正oid UMin成RTSSpectato本Syste設置::SetCa設置e本aT本ansfo本設置(const 軍St本in成& Spectato本ID, const 軍Vecto本& Location, const 軍Rotato本& Rotation)
{
    軍Spectato本Info* Spectato本 = Acti正eSpectato本s.軍ind(Spectato本ID);
    if (Spectato本)
    {
        Spectato本->Ca設置e本aLocation = Location;
        Spectato本->Ca設置e本aRotation = Rotation;
        UpdateSpectato本Info(Spectato本ID, *Spectato本);
    }
}

正oid UMin成RTSSpectato本Syste設置::SetZoo設置Le正el(const 軍St本in成& Spectato本ID, float Zoo設置Le正el)
{
    軍Spectato本Info* Spectato本 = Acti正eSpectato本s.軍ind(Spectato本ID);
    if (Spectato本)
    {
        Spectato本->Zoo設置Le正el = 軍Math::Cla設置p(Zoo設置Le正el, 0.1f, 5.0f);
        UpdateSpectato本Info(Spectato本ID, *Spectato本);
    }
}

正oid UMin成RTSSpectato本Syste設置::To成成lePict使本eInPict使本e(const 軍St本in成& Spectato本ID, bool bEnable)
{
    軍Spectato本Info* Spectato本 = Acti正eSpectato本s.軍ind(Spectato本ID);
    if (Spectato本)
    {
        if (bEnable)
        {
            Spectato本->C使本本entMode = ESpectato本Mode::Pict使本eInPict使本e;
        }
        else
        {
            Spectato本->C使本本entMode = ESpectato本Mode::軍本eeCa設置e本a;
        }
        UpdateSpectato本Info(Spectato本ID, *Spectato本);
        OnSpectato本ModeChan成ed.B本oadcast(Spectato本ID, Spectato本->C使本本entMode);
    }
}

正oid UMin成RTSSpectato本Syste設置::SendSpectato本Chat(const 軍St本in成& Roo設置ID, const 軍St本in成& Spectato本ID, const 軍St本in成& Messa成e)
{
    軍Spectato本Roo設置* Roo設置 = Acti正eRoo設置s.軍ind(Roo設置ID);
    if (Roo設置 && !Roo設置->bAllowChat)
    {
        本et使本n; // 聊天被禁用
    }
    
    OnSpectato本Chat.B本oadcast(Roo設置ID, Spectato本ID, Messa成e);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Spectato本 chat in 本oo設置 %s: %s"), *Roo設置ID, *Messa成e);
}

TA本本ay<軍Spectato本Info> UMin成RTSSpectato本Syste設置::GetSpectato本sInRoo設置(const 軍St本in成& Roo設置ID) const
{
    const 軍Spectato本Roo設置* Roo設置 = Acti正eRoo設置s.軍ind(Roo設置ID);
    if (Roo設置)
    {
        本et使本n Roo設置->Spectato本s;
    }
    本et使本n TA本本ay<軍Spectato本Info>();
}

int32 UMin成RTSSpectato本Syste設置::GetSpectato本Co使nt(const 軍St本in成& Roo設置ID) const
{
    const 軍Spectato本Roo設置* Roo設置 = Acti正eRoo設置s.軍ind(Roo設置ID);
    if (Roo設置)
    {
        本et使本n Roo設置->C使本本entSpectato本s;
    }
    本et使本n 0;
}

bool UMin成RTSSpectato本Syste設置::IsSpectatin成Enabled(const 軍St本in成& MatchID) const
{
    本et使本n Spectatin成EnabledMatches.Contains(MatchID);
}

正oid UMin成RTSSpectato本Syste設置::SetSpectatin成Enabled(const 軍St本in成& MatchID, bool bEnabled)
{
    if (bEnabled)
    {
        Spectatin成EnabledMatches.Add(MatchID);
    }
    else
    {
        Spectatin成EnabledMatches.Re設置o正e(MatchID);
    }
}

正oid UMin成RTSSpectato本Syste設置::SetRoo設置Passwo本d(const 軍St本in成& Roo設置ID, const 軍St本in成& 的ewPasswo本d)
{
    軍Spectato本Roo設置* Roo設置 = Acti正eRoo設置s.軍ind(Roo設置ID);
    if (Roo設置)
    {
        Roo設置->bIsPasswo本dP本otected = !的ewPasswo本d.IsE設置pty();
        Roo設置->Passwo本d = 的ewPasswo本d;
    }
}

正oid UMin成RTSSpectato本Syste設置::KickSpectato本(const 軍St本in成& Roo設置ID, const 軍St本in成& Spectato本ID, const 軍St本in成& Reason)
{
    軍Spectato本Roo設置* Roo設置 = Acti正eRoo設置s.軍ind(Roo設置ID);
    if (Roo設置)
    {
        // 找到並移除觀戰者
        fo本 (int32 i = 0; i < Roo設置->Spectato本s.的使設置(); ++i)
        {
            if (Roo設置->Spectato本s[i].Spectato本ID == Spectato本ID)
            {
                Roo設置->Spectato本s.Re設置o正eAt(i);
                Roo設置->C使本本entSpectato本s = 軍Math::Max(0, Roo設置->C使本本entSpectato本s - 1);
                b本eak;
            }
        }
        
        Acti正eSpectato本s.Re設置o正e(Spectato本ID);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Spectato本 %s kicked f本o設置 本oo設置 %s. Reason: %s"), 
            *Spectato本ID, *Roo設置ID, *Reason);
    }
}

正oid UMin成RTSSpectato本Syste設置::M使teSpectato本(const 軍St本in成& Roo設置ID, const 軍St本in成& Spectato本ID, int32 D使本ationMin使tes)
{
    軍Spectato本Info* Spectato本 = Acti正eSpectato本s.軍ind(Spectato本ID);
    if (Spectato本)
    {
        Spectato本->bIsChatEnabled = false;
        UpdateSpectato本Info(Spectato本ID, *Spectato本);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Spectato本 %s 設置使ted fo本 %d 設置in使tes"), *Spectato本ID, D使本ationMin使tes);
        
        // 這裡應該設置一個定時器，在D使本ationMin使tes後解除禁言
    }
}

軍St本in成 UMin成RTSSpectato本Syste設置::Gene本ateRoo設置ID()
{
    本et使本n 軍St本in成::P本intf(TEXT("ROOM下%s"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
}

正oid UMin成RTSSpectato本Syste設置::B本oadcastToRoo設置(const 軍St本in成& Roo設置ID, const 軍St本in成& Messa成e)
{
    // 向房間內所有觀戰者廣播消息
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B本oadcast to 本oo設置 %s: %s"), *Roo設置ID, *Messa成e);
}

正oid UMin成RTSSpectato本Syste設置::UpdateSpectato本Info(const 軍St本in成& Spectato本ID, const 軍Spectato本Info& Info)
{
    // 更新活躍觀戰者信息
    if (Acti正eSpectato本s.Contains(Spectato本ID))
    {
        Acti正eSpectato本s[Spectato本ID] = Info;
    }
    
    // 更新房間內的觀戰者信息
    fo本 (a使to& Roo設置Pai本 : Acti正eRoo設置s)
    {
        fo本 (軍Spectato本Info& Roo設置Spectato本 : Roo設置Pai本.Val使e.Spectato本s)
        {
            if (Roo設置Spectato本.Spectato本ID == Spectato本ID)
            {
                Roo設置Spectato本 = Info;
                本et使本n;
            }
        }
    }
}
