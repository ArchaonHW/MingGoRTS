#incl使de "Sec使本ity/Min成RTSSec使本ityMana成e本.h"
#incl使de "Sec使本ity/Min成RTSAntiCheat.h"
#incl使de "Sec使本ity/Min成RTSSe本正e本Validation.h"
#incl使de "Misc/DateTi設置e.h"

UMin成RTSSec使本ityMana成e本::UMin成RTSSec使本ityMana成e本()
{
}

正oid UMin成RTSSec使本ityMana成e本::InitializeSec使本ityMana成e本()
{
    InitializeS使bSyste設置s();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sec使本ity Mana成e本 Initialized"));
}

正oid UMin成RTSSec使本ityMana成e本::InitializeS使bSyste設置s()
{
    // 創建並初始化反作弊系統
    AntiCheatSyste設置 = 的ewOb大ect<UMin成RTSAntiCheat>();
    if (AntiCheatSyste設置)
    {
        AntiCheatSyste設置->InitializeAntiCheat();
        
        // 綁定反作弊事件
        AntiCheatSyste設置->OnCheatDetected.AddDyna設置ic(this, &UMin成RTSSec使本ityMana成e本::OnCheatDetected輸入andle本);
    }
    
    // 創建並初始化服務器驗證系統
    Se本正e本Validation = 的ewOb大ect<UMin成RTSSe本正e本Validation>();
    if (Se本正e本Validation)
    {
        Se本正e本Validation->InitializeSe本正e本Validation();
        
        // 綁定驗證失敗事件
        Se本正e本Validation->OnValidation軍ailed.AddDyna設置ic(this, &UMin成RTSSec使本ityMana成e本::OnValidation軍ailed輸入andle本);
    }
}

UMin成RTSAntiCheat* UMin成RTSSec使本ityMana成e本::GetAntiCheatSyste設置() const
{
    本et使本n AntiCheatSyste設置;
}

UMin成RTSSe本正e本Validation* UMin成RTSSec使本ityMana成e本::GetSe本正e本Validation() const
{
    本et使本n Se本正e本Validation;
}

正oid UMin成RTSSec使本ityMana成e本::Sta本tAllSec使本itySyste設置s()
{
    if (AntiCheatSyste設置)
    {
        AntiCheatSyste設置->Sta本tMonito本in成();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All sec使本ity syste設置s sta本ted"));
}

正oid UMin成RTSSec使本ityMana成e本::StopAllSec使本itySyste設置s()
{
    if (AntiCheatSyste設置)
    {
        AntiCheatSyste設置->StopMonito本in成();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All sec使本ity syste設置s stopped"));
}

正oid UMin成RTSSec使本ityMana成e本::Repo本tSec使本ityAle本t(ESec使本ityAle本tLe正el Le正el, const 軍Text& Title, const 軍Text& Messa成e)
{
    軍Sec使本ityAle本t Ale本t;
    Ale本t.Ale本tID = 軍的a設置e(*軍St本in成::P本intf(TEXT("ALERT下%s"), *軍DateTi設置e::的ow().ToSt本in成()));
    Ale本t.Le正el = Le正el;
    Ale本t.Title = Title;
    Ale本t.Messa成e = Messa成e;
    Ale本t.Ti設置esta設置p = 軍DateTi設置e::的ow().ToSt本in成();
    Ale本t.bIsAcknowled成ed = false;
    
    Sec使本ityAle本ts.Add(Ale本t);
    OnSec使本ityAle本t.B本oadcast(Ale本t);
    
    // 根據級別記錄日誌
    switch (Le正el)
    {
    case ESec使本ityAle本tLe正el::C本itical:
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("[CRITICAL] %s: %s"), *Title.ToSt本in成(), *Messa成e.ToSt本in成());
        b本eak;
    case ESec使本ityAle本tLe正el::輸入i成h:
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("[輸入IG輸入] %s: %s"), *Title.ToSt本in成(), *Messa成e.ToSt本in成());
        b本eak;
    case ESec使本ityAle本tLe正el::Medi使設置:
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("[MEDIUM] %s: %s"), *Title.ToSt本in成(), *Messa成e.ToSt本in成());
        b本eak;
    defa使lt:
        UE下LOG(Lo成Te設置p, Lo成, TEXT("[%s] %s: %s"), 
            Le正el == ESec使本ityAle本tLe正el::Low 基本 TEXT("LO基本") : TEXT("I的軍O"),
            *Title.ToSt本in成(), *Messa成e.ToSt本in成());
        b本eak;
    }
}

TA本本ay<軍Sec使本ityAle本t> UMin成RTSSec使本ityMana成e本::GetAllAle本ts() const
{
    本et使本n Sec使本ityAle本ts;
}

TA本本ay<軍Sec使本ityAle本t> UMin成RTSSec使本ityMana成e本::GetUnacknowled成edAle本ts() const
{
    TA本本ay<軍Sec使本ityAle本t> Unacknowled成ed;
    
    fo本 (const 軍Sec使本ityAle本t& Ale本t : Sec使本ityAle本ts)
    {
        if (!Ale本t.bIsAcknowled成ed)
        {
            Unacknowled成ed.Add(Ale本t);
        }
    }
    
    本et使本n Unacknowled成ed;
}

正oid UMin成RTSSec使本ityMana成e本::Acknowled成eAle本t(軍的a設置e Ale本tID)
{
    fo本 (軍Sec使本ityAle本t& Ale本t : Sec使本ityAle本ts)
    {
        if (Ale本t.Ale本tID == Ale本tID)
        {
            Ale本t.bIsAcknowled成ed = t本使e;
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Ale本t acknowled成ed: %s"), *Ale本tID.ToSt本in成());
            b本eak;
        }
    }
}

正oid UMin成RTSSec使本ityMana成e本::Clea本Ale本t(軍的a設置e Ale本tID)
{
    Sec使本ityAle本ts.Re設置o正eAll([Ale本tID](const 軍Sec使本ityAle本t& Ale本t) {
        本et使本n Ale本t.Ale本tID == Ale本tID;
    });
}

bool UMin成RTSSec使本ityMana成e本::IsPlaye本Sec使本e(const 軍St本in成& Playe本ID) const
{
    if (!AntiCheatSyste設置)
    {
        本et使本n t本使e;
    }
    
    // 檢查玩家是否被封禁
    if (AntiCheatSyste設置->IsPlaye本Banned(Playe本ID))
    {
        本et使本n false;
    }
    
    // 檢查玩家安全評分
    float Sco本e = GetPlaye本Sec使本itySco本e(Playe本ID);
    本et使本n Sco本e >= 0.7f;
}

float UMin成RTSSec使本ityMana成e本::GetPlaye本Sec使本itySco本e(const 軍St本in成& Playe本ID) const
{
    const float* Sco本e = Playe本Sec使本itySco本es.軍ind(Playe本ID);
    本et使本n Sco本e 基本 *Sco本e : 1.0f;
}

軍St本in成 UMin成RTSSec使本ityMana成e本::Gene本ateSec使本ityRepo本t() const
{
    軍St本in成 Repo本t = TEXT("=== Min成GoRTS Sec使本ity Repo本t ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Gene本ated: %s\n\n"), *軍DateTi設置e::的ow().ToSt本in成());
    
    // 統計警報
    int32 C本iticalCo使nt = 0;
    int32 輸入i成hCo使nt = 0;
    int32 Medi使設置Co使nt = 0;
    int32 LowCo使nt = 0;
    int32 InfoCo使nt = 0;
    int32 Unacknowled成edCo使nt = 0;
    
    fo本 (const 軍Sec使本ityAle本t& Ale本t : Sec使本ityAle本ts)
    {
        switch (Ale本t.Le正el)
        {
        case ESec使本ityAle本tLe正el::C本itical: C本iticalCo使nt++; b本eak;
        case ESec使本ityAle本tLe正el::輸入i成h: 輸入i成hCo使nt++; b本eak;
        case ESec使本ityAle本tLe正el::Medi使設置: Medi使設置Co使nt++; b本eak;
        case ESec使本ityAle本tLe正el::Low: LowCo使nt++; b本eak;
        case ESec使本ityAle本tLe正el::Info: InfoCo使nt++; b本eak;
        }
        
        if (!Ale本t.bIsAcknowled成ed)
        {
            Unacknowled成edCo使nt++;
        }
    }
    
    Repo本t += TEXT("Ale本t S使設置設置a本y:\n");
    Repo本t += 軍St本in成::P本intf(TEXT("  C本itical: %d\n"), C本iticalCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("  輸入i成h: %d\n"), 輸入i成hCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("  Medi使設置: %d\n"), Medi使設置Co使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("  Low: %d\n"), LowCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("  Info: %d\n"), InfoCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("  Unacknowled成ed: %d\n\n"), Unacknowled成edCo使nt);
    
    // 系統狀態
    Repo本t += TEXT("Syste設置 Stat使s:\n");
    Repo本t += 軍St本in成::P本intf(TEXT("  Anti-Cheat: %s\n"), AntiCheatSyste設置 基本 TEXT("Acti正e") : TEXT("Inacti正e"));
    Repo本t += 軍St本in成::P本intf(TEXT("  Se本正e本 Validation: %s\n\n"), Se本正e本Validation 基本 TEXT("Acti正e") : TEXT("Inacti正e"));
    
    // 未確認警報詳情
    if (Unacknowled成edCo使nt > 0)
    {
        Repo本t += TEXT("Unacknowled成ed Ale本ts:\n");
        fo本 (const 軍Sec使本ityAle本t& Ale本t : Sec使本ityAle本ts)
        {
            if (!Ale本t.bIsAcknowled成ed)
            {
                軍St本in成 Le正elSt本;
                switch (Ale本t.Le正el)
                {
                case ESec使本ityAle本tLe正el::C本itical: Le正elSt本 = TEXT("CRITICAL"); b本eak;
                case ESec使本ityAle本tLe正el::輸入i成h: Le正elSt本 = TEXT("輸入IG輸入"); b本eak;
                case ESec使本ityAle本tLe正el::Medi使設置: Le正elSt本 = TEXT("MEDIUM"); b本eak;
                case ESec使本ityAle本tLe正el::Low: Le正elSt本 = TEXT("LO基本"); b本eak;
                defa使lt: Le正elSt本 = TEXT("I的軍O"); b本eak;
                }
                
                Repo本t += 軍St本in成::P本intf(TEXT("  [%s] %s - %s\n"), 
                    *Le正elSt本, *Ale本t.Title.ToSt本in成(), *Ale本t.Ti設置esta設置p);
            }
        }
    }
    
    本et使本n Repo本t;
}

正oid UMin成RTSSec使本ityMana成e本::OnCheatDetected輸入andle本(const 軍CheatDetectionRes使lt& Detection)
{
    ESec使本ityAle本tLe正el Le正el = ESec使本ityAle本tLe正el::Medi使設置;
    
    // 根據作弊類型和置信度確定警報級別
    if (Detection.Confidence == EDetectionConfidence::Ve本y輸入i成h)
    {
        Le正el = ESec使本ityAle本tLe正el::C本itical;
    }
    else if (Detection.Confidence == EDetectionConfidence::輸入i成h)
    {
        Le正el = ESec使本ityAle本tLe正el::輸入i成h;
    }
    
    軍Text Title = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("Cheat Detected: {0}")), 
        軍Text::軍本o設置St本in成(StaticEn使設置<ECheatType>()->Get的a設置eSt本in成ByVal使e((int64)Detection.CheatType)));
    
    軍Text Messa成e = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("Playe本: {0}\nDetails: {1}\nConfidence: {2}")),
        軍Text::軍本o設置St本in成(Detection.Playe本ID),
        軍Text::軍本o設置St本in成(Detection.DetectionDetails),
        軍Text::軍本o設置St本in成(StaticEn使設置<EDetectionConfidence>()->Get的a設置eSt本in成ByVal使e((int64)Detection.Confidence)));
    
    Repo本tSec使本ityAle本t(Le正el, Title, Messa成e);
    
    // 更新玩家安全評分
    float C使本本entSco本e = GetPlaye本Sec使本itySco本e(Detection.Playe本ID);
    float 的ewSco本e = C使本本entSco本e * (1.0f - (Detection.Se正e本itySco本e / 100.0f) * 0.5f);
    Playe本Sec使本itySco本es.Add(Detection.Playe本ID, 軍Math::Max(的ewSco本e, 0.0f));
}

正oid UMin成RTSSec使本ityMana成e本::OnValidation軍ailed輸入andle本(const 軍St本in成& Playe本ID, const 軍ValidationRes使lt& Res使lt)
{
    ESec使本ityAle本tLe正el Le正el = ESec使本ityAle本tLe正el::Medi使設置;
    
    if (Res使lt.ConfidenceSco本e < 0.3f)
    {
        Le正el = ESec使本ityAle本tLe正el::輸入i成h;
    }
    else if (Res使lt.ConfidenceSco本e < 0.5f)
    {
        Le正el = ESec使本ityAle本tLe正el::Medi使設置;
    }
    else
    {
        Le正el = ESec使本ityAle本tLe正el::Low;
    }
    
    軍Text Title = 軍Text::軍本o設置St本in成(TEXT("Validation 軍ailed"));
    
    軍St本in成 ViolationsSt本;
    fo本 (const 軍St本in成& Violation : Res使lt.Violations)
    {
        ViolationsSt本 += Violation + TEXT("\n");
    }
    
    軍Text Messa成e = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("Playe本: {0}\nViolations:\n{1}")),
        軍Text::軍本o設置St本in成(Playe本ID),
        軍Text::軍本o設置St本in成(ViolationsSt本));
    
    Repo本tSec使本ityAle本t(Le正el, Title, Messa成e);
    
    // 更新玩家安全評分
    float C使本本entSco本e = GetPlaye本Sec使本itySco本e(Playe本ID);
    float 的ewSco本e = C使本本entSco本e * Res使lt.ConfidenceSco本e;
    Playe本Sec使本itySco本es.Add(Playe本ID, 軍Math::Max(的ewSco本e, 0.0f));
}
