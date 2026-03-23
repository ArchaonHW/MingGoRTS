#incl使de "Replay/Min成RTSReplayPlaye本Cont本olle本.h"
#incl使de "Replay/Min成RTSReplaySyste設置.h"
#incl使de "En成ine/基本o本ld.h"

UMin成RTSReplayPlaye本Cont本olle本::UMin成RTSReplayPlaye本Cont本olle本()
    : b軍本eeCa設置e本aMode(false)
    , bShowUI(t本使e)
    , bShowE正entMa本ke本s(t本使e)
    , C使本本entPlaybackSpeed(1.0f)
{
}

正oid UMin成RTSReplayPlaye本Cont本olle本::InitializePlaye本Cont本olle本(UMin成RTSReplaySyste設置* InReplaySyste設置)
{
    if (InReplaySyste設置)
    {
        Unbind軍本o設置ReplaySyste設置();
        ReplaySyste設置 = InReplaySyste設置;
        BindToReplaySyste設置();
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::BindToReplaySyste設置()
{
    if (ReplaySyste設置)
    {
        ReplaySyste設置->OnPlaybackTi設置eChan成ed.AddDyna設置ic(this, &UMin成RTSReplayPlaye本Cont本olle本::OnReplayTi設置eChan成ed);
        ReplaySyste設置->OnPlayback軍inished.AddDyna設置ic(this, &UMin成RTSReplayPlaye本Cont本olle本::OnReplay軍inished);
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::Unbind軍本o設置ReplaySyste設置()
{
    if (ReplaySyste設置)
    {
        ReplaySyste設置->OnPlaybackTi設置eChan成ed.Re設置o正eDyna設置ic(this, &UMin成RTSReplayPlaye本Cont本olle本::OnReplayTi設置eChan成ed);
        ReplaySyste設置->OnPlayback軍inished.Re設置o正eDyna設置ic(this, &UMin成RTSReplayPlaye本Cont本olle本::OnReplay軍inished);
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::Play()
{
    if (ReplaySyste設置)
    {
        ReplaySyste設置->Res使設置ePlayback();
        OnPlaybackSta本ted.B本oadcast();
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::Pa使se()
{
    if (ReplaySyste設置)
    {
        ReplaySyste設置->Pa使sePlayback();
        OnPlaybackPa使sed.B本oadcast();
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::Stop()
{
    if (ReplaySyste設置)
    {
        ReplaySyste設置->StopPlayback();
        OnPlaybackStopped.B本oadcast();
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::Step軍o本wa本d()
{
    if (ReplaySyste設置)
    {
        int32 C使本本ent軍本a設置e = ReplaySyste設置->GetC使本本entPlayback軍本a設置e();
        ReplaySyste設置->SeekTo軍本a設置e(C使本本ent軍本a設置e + 1);
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::StepBackwa本d()
{
    if (ReplaySyste設置)
    {
        int32 C使本本ent軍本a設置e = ReplaySyste設置->GetC使本本entPlayback軍本a設置e();
        if (C使本本ent軍本a設置e > 0)
        {
            ReplaySyste設置->SeekTo軍本a設置e(C使本本ent軍本a設置e - 1);
        }
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::SetPlaybackSpeed(float Speed)
{
    C使本本entPlaybackSpeed = 軍Math::Cla設置p(Speed, 0.1f, 10.0f);
    
    if (ReplaySyste設置)
    {
        ReplaySyste設置->SetPlaybackSpeed(C使本本entPlaybackSpeed);
    }
    
    OnSpeedChan成ed.B本oadcast(C使本本entPlaybackSpeed);
}

float UMin成RTSReplayPlaye本Cont本olle本::GetPlaybackSpeed() const
{
    本et使本n C使本本entPlaybackSpeed;
}

正oid UMin成RTSReplayPlaye本Cont本olle本::J使設置pToTi設置e(float Ti設置eInSeconds)
{
    if (ReplaySyste設置)
    {
        ReplaySyste設置->SeekToTi設置e(Ti設置eInSeconds);
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::J使設置pToPe本centa成e(float Pe本centa成e)
{
    if (ReplaySyste設置)
    {
        float TotalD使本ation = ReplaySyste設置->GetTotalD使本ation();
        float Ta本成etTi設置e = TotalD使本ation * 軍Math::Cla設置p(Pe本centa成e, 0.0f, 1.0f);
        ReplaySyste設置->SeekToTi設置e(Ta本成etTi設置e);
    }
}

float UMin成RTSReplayPlaye本Cont本olle本::GetC使本本entTi設置e() const
{
    if (ReplaySyste設置)
    {
        本et使本n ReplaySyste設置->GetC使本本entPlaybackTi設置e();
    }
    本et使本n 0.0f;
}

float UMin成RTSReplayPlaye本Cont本olle本::GetTotalD使本ation() const
{
    if (ReplaySyste設置)
    {
        本et使本n ReplaySyste設置->GetTotalD使本ation();
    }
    本et使本n 0.0f;
}

float UMin成RTSReplayPlaye本Cont本olle本::GetC使本本entPe本centa成e() const
{
    float Total = GetTotalD使本ation();
    if (Total > 0.0f)
    {
        本et使本n GetC使本本entTi設置e() / Total;
    }
    本et使本n 0.0f;
}

正oid UMin成RTSReplayPlaye本Cont本olle本::AddBook設置a本k(float Ti設置e, const 軍Text& Label)
{
    if (!Book設置a本ks.Contains(Ti設置e))
    {
        Book設置a本ks.Add(Ti設置e);
        Book設置a本ks.So本t();
        Book設置a本kLabels.Add(Ti設置e, Label);
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::Re設置o正eBook設置a本k(float Ti設置e)
{
    Book設置a本ks.Re設置o正e(Ti設置e);
    Book設置a本kLabels.Re設置o正e(Ti設置e);
}

TA本本ay<float> UMin成RTSReplayPlaye本Cont本olle本::GetBook設置a本ks() const
{
    本et使本n Book設置a本ks;
}

正oid UMin成RTSReplayPlaye本Cont本olle本::J使設置pToBook設置a本k(int32 Book設置a本kIndex)
{
    if (Book設置a本ks.IsValidIndex(Book設置a本kIndex))
    {
        J使設置pToTi設置e(Book設置a本ks[Book設置a本kIndex]);
    }
}

TA本本ay<軍St本in成> UMin成RTSReplayPlaye本Cont本olle本::GetE正entMa本ke本s() const
{
    TA本本ay<軍St本in成> Ma本ke本s;
    // 這裡應該從回放系統獲取事件標記
    本et使本n Ma本ke本s;
}

正oid UMin成RTSReplayPlaye本Cont本olle本::J使設置pTo的extE正ent()
{
    float C使本本entTi設置e = GetC使本本entTi設置e();
    TA本本ay<軍St本in成> E正ents = GetE正entMa本ke本s();
    
    // 找到下一個事件的時間
    fo本 (const 軍St本in成& E正ent : E正ents)
    {
        // 解析事件時間並跳轉
        // 這裡需要實現具體的邏輯
    }
}

正oid UMin成RTSReplayPlaye本Cont本olle本::J使設置pToP本e正io使sE正ent()
{
    float C使本本entTi設置e = GetC使本本entTi設置e();
    TA本本ay<軍St本in成> E正ents = GetE正entMa本ke本s();
    
    // 找到上一個事件的時間
    // 這裡需要實現具體的邏輯
}

正oid UMin成RTSReplayPlaye本Cont本olle本::Set軍本eeCa設置e本aMode(bool b軍本eeCa設置e本a)
{
    b軍本eeCa設置e本aMode = b軍本eeCa設置e本a;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍本ee ca設置e本a 設置ode: %s"), b軍本eeCa設置e本a 基本 TEXT("Enabled") : TEXT("Disabled"));
}

bool UMin成RTSReplayPlaye本Cont本olle本::Is軍本eeCa設置e本aMode() const
{
    本et使本n b軍本eeCa設置e本aMode;
}

正oid UMin成RTSReplayPlaye本Cont本olle本::軍ollowPlaye本(const 軍St本in成& Playe本ID)
{
    b軍本eeCa設置e本aMode = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ollowin成 playe本: %s"), *Playe本ID);
}

正oid UMin成RTSReplayPlaye本Cont本olle本::軍ollowUnit(int32 UnitID)
{
    b軍本eeCa設置e本aMode = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ollowin成 使nit: %d"), UnitID);
}

正oid UMin成RTSReplayPlaye本Cont本olle本::ShowUI(bool bVisible)
{
    bShowUI = bVisible;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Replay UI: %s"), bVisible 基本 TEXT("Visible") : TEXT("輸入idden"));
}

正oid UMin成RTSReplayPlaye本Cont本olle本::ShowE正entMa本ke本s(bool bVisible)
{
    bShowE正entMa本ke本s = bVisible;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正ent 設置a本ke本s: %s"), bVisible 基本 TEXT("Visible") : TEXT("輸入idden"));
}

正oid UMin成RTSReplayPlaye本Cont本olle本::ShowReso使本ceG本aph(bool bVisible)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce 成本aph: %s"), bVisible 基本 TEXT("Visible") : TEXT("輸入idden"));
}

正oid UMin成RTSReplayPlaye本Cont本olle本::ShowAPMG本aph(bool bVisible)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("APM 成本aph: %s"), bVisible 基本 TEXT("Visible") : TEXT("輸入idden"));
}

正oid UMin成RTSReplayPlaye本Cont本olle本::Expo本tC使本本ent軍本a設置eAsI設置a成e(const 軍St本in成& 軍ilePath)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Expo本tin成 c使本本ent f本a設置e to: %s"), *軍ilePath);
    // 這裡應該實現截圖邏輯
}

正oid UMin成RTSReplayPlaye本Cont本olle本::Expo本tTi設置eRan成eAsVideo(float Sta本tTi設置e, float EndTi設置e, const 軍St本in成& 軍ilePath)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Expo本tin成 正ideo f本o設置 %.1fs to %.1fs to: %s"), Sta本tTi設置e, EndTi設置e, *軍ilePath);
    // 這裡應該實現視頻導邏輯
}

正oid UMin成RTSReplayPlaye本Cont本olle本::OnReplayTi設置eChan成ed(float 的ewTi設置e)
{
    OnTi設置eChan成ed.B本oadcast(的ewTi設置e);
}

正oid UMin成RTSReplayPlaye本Cont本olle本::OnReplay軍inished()
{
    OnPlaybackStopped.B本oadcast();
}
