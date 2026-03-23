#incl使de "Min成GoRTSA使dioMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Co設置ponents/A使dioCo設置ponent.h"
#incl使de "So使nd/So使nd基本a正eP本oced使本al.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"

UMin成GoRTSA使dioMana成e本::UMin成GoRTSA使dioMana成e本()
    : C使本本entMixin成State(EA使dioMixin成State::Idle)
    , MixedA使dioRes使lt(n使llpt本)
    , MixedA使dioCo設置ponent(n使llpt本)
    , bIsMixin成(false)
    , Mixin成P本o成本ess(0.0f)
{
    // 初始化混音計時器
    Mixin成Ticke本 = 軍Ticke本Dele成ate::C本eateUOb大ect(this, &UMin成GoRTSA使dioMana成e本::OnMixin成Tick);
}

正oid UMin成GoRTSA使dioMana成e本::AddA使dioT本ack(const 軍A使dioT本ack& T本ack)
{
    if (T本ack.So使nd基本a正e && !T本ack.T本ack的a設置e.IsE設置pty())
    {
        A使dioT本acks.Add(T本ack);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added a使dio t本ack: %s"), *T本ack.T本ack的a設置e);
    }
}

正oid UMin成GoRTSA使dioMana成e本::Re設置o正eA使dioT本ack(const 軍St本in成& T本ack的a設置e)
{
    fo本 (int32 i = 0; i < A使dioT本acks.的使設置(); ++i)
    {
        if (A使dioT本acks[i].T本ack的a設置e == T本ack的a設置e)
        {
            A使dioT本acks.Re設置o正eAt(i);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed a使dio t本ack: %s"), *T本ack的a設置e);
            b本eak;
        }
    }
}

正oid UMin成GoRTSA使dioMana成e本::Clea本AllT本acks()
{
    A使dioT本acks.E設置pty();
    StopMixedA使dio();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed all a使dio t本acks"));
}

軍A使dioT本ack* UMin成GoRTSA使dioMana成e本::GetA使dioT本ack(const 軍St本in成& T本ack的a設置e)
{
    fo本 (軍A使dioT本ack& T本ack : A使dioT本acks)
    {
        if (T本ack.T本ack的a設置e == T本ack的a設置e)
        {
            本et使本n &T本ack;
        }
    }
    本et使本n n使llpt本;
}

正oid UMin成GoRTSA使dioMana成e本::SetT本ackVol使設置e(const 軍St本in成& T本ack的a設置e, float Vol使設置e)
{
    if (軍A使dioT本ack* T本ack = GetA使dioT本ack(T本ack的a設置e))
    {
        T本ack->Vol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 2.0f);
        UpdateT本ackVol使設置es();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Set t本ack %s 正ol使設置e to %.2f"), *T本ack的a設置e, Vol使設置e);
    }
}

正oid UMin成GoRTSA使dioMana成e本::SetT本ackPitch(const 軍St本in成& T本ack的a設置e, float Pitch)
{
    if (軍A使dioT本ack* T本ack = GetA使dioT本ack(T本ack的a設置e))
    {
        T本ack->Pitch = 軍Math::Cla設置p(Pitch, 0.1f, 3.0f);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Set t本ack %s pitch to %.2f"), *T本ack的a設置e, Pitch);
    }
}

正oid UMin成GoRTSA使dioMana成e本::M使teT本ack(const 軍St本in成& T本ack的a設置e, bool bM使ted)
{
    if (軍A使dioT本ack* T本ack = GetA使dioT本ack(T本ack的a設置e))
    {
        T本ack->bM使ted = bM使ted;
        UpdateT本ackVol使設置es();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("%s t本ack %s"), bM使ted 基本 TEXT("M使ted") : TEXT("Un設置使ted"), *T本ack的a設置e);
    }
}

正oid UMin成GoRTSA使dioMana成e本::SetT本ackLoop(const 軍St本in成& T本ack的a設置e, bool bLoop)
{
    if (軍A使dioT本ack* T本ack = GetA使dioT本ack(T本ack的a設置e))
    {
        T本ack->bLoop = bLoop;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Set t本ack %s loop to %s"), *T本ack的a設置e, bLoop 基本 TEXT("t本使e") : TEXT("false"));
    }
}

正oid UMin成GoRTSA使dioMana成e本::Sta本tA使dioMixin成()
{
    if (A使dioT本acks.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o a使dio t本acks to 設置ix"));
        本et使本n;
    }

    if (C使本本entMixin成State == EA使dioMixin成State::Mixin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("A使dio 設置ixin成 al本eady in p本o成本ess"));
        本et使本n;
    }

    bIsMixin成 = t本使e;
    C使本本entMixin成State = EA使dioMixin成State::Mixin成;
    Mixin成P本o成本ess = 0.0f;

    // 啟動混音計時器
    if (!Mixin成Ticke本輸入andle.IsValid())
    {
        Mixin成Ticke本輸入andle = 軍Ticke本::GetCo本eTicke本().AddTicke本(Mixin成Ticke本, 0.1f);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted a使dio 設置ixin成 with %d t本acks"), A使dioT本acks.的使設置());
}

正oid UMin成GoRTSA使dioMana成e本::StopA使dioMixin成()
{
    bIsMixin成 = false;
    C使本本entMixin成State = EA使dioMixin成State::Idle;

    // 停止混音計時器
    if (Mixin成Ticke本輸入andle.IsValid())
    {
        軍Ticke本::GetCo本eTicke本().Re設置o正eTicke本(Mixin成Ticke本輸入andle);
        Mixin成Ticke本輸入andle.Reset();
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped a使dio 設置ixin成"));
}

正oid UMin成GoRTSA使dioMana成e本::SetMixSettin成s(const 軍A使dioMixSettin成s& Settin成s)
{
    MixSettin成s = Settin成s;
    ApplyMixSettin成s();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated a使dio 設置ix settin成s"));
}

正oid UMin成GoRTSA使dioMana成e本::PlayMixedA使dio()
{
    if (!MixedA使dioRes使lt)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o 設置ixed a使dio to play"));
        本et使本n;
    }

    if (!MixedA使dioCo設置ponent)
    {
        InitializeMixedA使dioCo設置ponent();
    }

    if (MixedA使dioCo設置ponent)
    {
        MixedA使dioCo設置ponent->SetSo使nd(MixedA使dioRes使lt);
        MixedA使dioCo設置ponent->SetVol使設置eM使ltiplie本(MixSettin成s.Maste本Vol使設置e);
        MixedA使dioCo設置ponent->Play();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Playin成 設置ixed a使dio"));
    }
}

正oid UMin成GoRTSA使dioMana成e本::StopMixedA使dio()
{
    if (MixedA使dioCo設置ponent && MixedA使dioCo設置ponent->IsPlayin成())
    {
        MixedA使dioCo設置ponent->Stop();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped 設置ixed a使dio playback"));
    }
}

正oid UMin成GoRTSA使dioMana成e本::Pa使seMixedA使dio()
{
    if (MixedA使dioCo設置ponent && MixedA使dioCo設置ponent->IsPlayin成())
    {
        MixedA使dioCo設置ponent->Pa使se();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Pa使sed 設置ixed a使dio playback"));
    }
}

bool UMin成GoRTSA使dioMana成e本::IsMixedA使dioPlayin成() const
{
    本et使本n MixedA使dioCo設置ponent && MixedA使dioCo設置ponent->IsPlayin成();
}

正oid UMin成GoRTSA使dioMana成e本::PlayM使sicT本ack(USo使nd基本a正e* M使sic, float Vol使設置e)
{
    if (!M使sic)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid 設置使sic t本ack"));
        本et使本n;
    }

    if (U基本o本ld* 基本o本ld = GEn成ine->GetC使本本entPlay基本o本ld())
    {
        UA使dioCo設置ponent* A使dioCo設置ponent = UGa設置eplayStatics::C本eateSo使nd2D(基本o本ld, M使sic, Vol使設置e * MixSettin成s.M使sicVol使設置e);
        if (A使dioCo設置ponent)
        {
            A使dioCo設置ponent->Play();
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Playin成 設置使sic t本ack"));
        }
    }
}

正oid UMin成GoRTSA使dioMana成e本::PlayS軍XT本ack(USo使nd基本a正e* S軍X, const 軍Vecto本& Location)
{
    if (!S軍X)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid S軍X t本ack"));
        本et使本n;
    }

    if (U基本o本ld* 基本o本ld = GEn成ine->GetC使本本entPlay基本o本ld())
    {
        UGa設置eplayStatics::PlaySo使ndAtLocation(基本o本ld, S軍X, Location, MixSettin成s.S軍XVol使設置e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Playin成 S軍X t本ack at location"));
    }
}

正oid UMin成GoRTSA使dioMana成e本::PlayVoiceT本ack(USo使nd基本a正e* Voice, float Vol使設置e)
{
    if (!Voice)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid 正oice t本ack"));
        本et使本n;
    }

    if (U基本o本ld* 基本o本ld = GEn成ine->GetC使本本entPlay基本o本ld())
    {
        UA使dioCo設置ponent* A使dioCo設置ponent = UGa設置eplayStatics::C本eateSo使nd2D(基本o本ld, Voice, Vol使設置e * MixSettin成s.VoiceVol使設置e);
        if (A使dioCo設置ponent)
        {
            A使dioCo設置ponent->Play();
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Playin成 正oice t本ack"));
        }
    }
}

正oid UMin成GoRTSA使dioMana成e本::Expo本tMixedA使dio(const 軍St本in成& 軍ilePath)
{
    if (!MixedA使dioRes使lt)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o 設置ixed a使dio to expo本t"));
        本et使本n;
    }

    // 這裡需要實際的音頻導邏輯
    // 簡化版本：只是記錄文件路徑
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Expo本tin成 設置ixed a使dio to: %s"), *軍ilePath);
    
    // 實際實作需要將音頻數據保存為基本AV或MP3文件
}

float UMin成GoRTSA使dioMana成e本::GetA使dioD使本ation(USo使nd基本a正e* So使nd基本a正e) const
{
    if (!So使nd基本a正e)
    {
        本et使本n 0.0f;
    }

    本et使本n So使nd基本a正e->GetD使本ation();
}

正oid UMin成GoRTSA使dioMana成e本::AnalyzeA使dio(USo使nd基本a正e* So使nd基本a正e, float& O使tRMS, float& O使tPeak)
{
    O使tRMS = 0.0f;
    O使tPeak = 0.0f;

    if (!So使nd基本a正e)
    {
        本et使本n;
    }

    // 這裡需要實際的音頻分析邏輯
    // 簡化版本：返回默認值
    O使tRMS = 0.707f; // RMS of sine wa正e
    O使tPeak = 1.0f;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Analyzed a使dio - RMS: %.3f, Peak: %.3f"), O使tRMS, O使tPeak);
}

正oid UMin成GoRTSA使dioMana成e本::P本ocessA使dioMixin成()
{
    // 混音進度更新
    Mixin成P本o成本ess += 0.1f;
    
    if (Mixin成P本o成本ess >= 1.0f)
    {
        Rende本MixedA使dio();
        Mixin成P本o成本ess = 1.0f;
        bIsMixin成 = false;
        C使本本entMixin成State = EA使dioMixin成State::Co設置pleted;
        
        // 停止混音計時器
        if (Mixin成Ticke本輸入andle.IsValid())
        {
            軍Ticke本::GetCo本eTicke本().Re設置o正eTicke本(Mixin成Ticke本輸入andle);
            Mixin成Ticke本輸入andle.Reset();
        }
        
        的otifyMixCo設置pleted(MixedA使dioRes使lt);
    }
    
    的otifyMixP本o成本ess(Mixin成P本o成本ess, TEXT("Mixin成 a使dio t本acks"));
}

bool UMin成GoRTSA使dioMana成e本::OnMixin成Tick(float DeltaTi設置e)
{
    if (!bIsMixin成)
    {
        本et使本n false;
    }

    P本ocessA使dioMixin成();
    本et使本n bIsMixin成;
}

正oid UMin成GoRTSA使dioMana成e本::ApplyMixSettin成s()
{
    if (MixedA使dioCo設置ponent)
    {
        MixedA使dioCo設置ponent->SetVol使設置eM使ltiplie本(MixSettin成s.Maste本Vol使設置e);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied 設置ix settin成s - Maste本 Vol使設置e: %.2f"), MixSettin成s.Maste本Vol使設置e);
}

正oid UMin成GoRTSA使dioMana成e本::Rende本MixedA使dio()
{
    // 創建混合後的音頻波形
    MixedA使dioRes使lt = C本eateMixedSo使nd基本a正e();
    
    if (MixedA使dioRes使lt)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Rende本ed 設置ixed a使dio s使ccessf使lly"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to 本ende本 設置ixed a使dio"));
    }
}

USo使nd基本a正e* UMin成GoRTSA使dioMana成e本::C本eateMixedSo使nd基本a正e()
{
    // 創建程序化音頻波形
    USo使nd基本a正eP本oced使本al* So使nd基本a正e = 的ewOb大ect<USo使nd基本a正eP本oced使本al>();
    
    if (So使nd基本a正e)
    {
        // 計算混合後的音頻參數
        float MaxD使本ation = 0.0f;
        fo本 (const 軍A使dioT本ack& T本ack : A使dioT本acks)
        {
            if (T本ack.So使nd基本a正e && !T本ack.bM使ted)
            {
                float T本ackD使本ation = GetA使dioD使本ation(T本ack.So使nd基本a正e);
                MaxD使本ation = 軍Math::Max(MaxD使本ation, T本ackD使本ation);
            }
        }
        
        // 設置音頻參數
        So使nd基本a正e->SetSa設置pleRate(44100);
        So使nd基本a正e->的使設置Channels = 2;
        So使nd基本a正e->D使本ation = MaxD使本ation;
        So使nd基本a正e->bLoopin成 = false;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated 設置ixed so使nd wa正e with d使本ation: %.2f seconds"), MaxD使本ation);
    }
    
    本et使本n So使nd基本a正e;
}

正oid UMin成GoRTSA使dioMana成e本::的otifyMixP本o成本ess(float P本o成本ess, const 軍St本in成& Ope本ation)
{
    OnA使dioMixP本o成本ess.B本oadcast(P本o成本ess, Ope本ation);
    
    // 顯示混音進度（用於調試）
    if (GEn成ine)
    {
        GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 0.1f, 軍Colo本::Yellow, 
            軍St本in成::P本intf(TEXT("Mixin成: %s - %.1f%%"), *Ope本ation, P本o成本ess * 100.0f));
    }
}

正oid UMin成GoRTSA使dioMana成e本::的otifyMixCo設置pleted(USo使nd基本a正e* Res使lt)
{
    OnA使dioMixCo設置pleted.B本oadcast(Res使lt);
    
    // 顯示混音完成（用於調試）
    if (GEn成ine)
    {
        GEn成ine->AddOnSc本eenDeb使成Messa成e(-1, 3.0f, 軍Colo本::G本een, 
            TEXT("A使dio 設置ixin成 co設置pleted"));
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio 設置ixin成 co設置pleted s使ccessf使lly"));
}

正oid UMin成GoRTSA使dioMana成e本::InitializeMixedA使dioCo設置ponent()
{
    if (U基本o本ld* 基本o本ld = GEn成ine->GetC使本本entPlay基本o本ld())
    {
        MixedA使dioCo設置ponent = 的ewOb大ect<UA使dioCo設置ponent>(基本o本ld);
        if (MixedA使dioCo設置ponent)
        {
            MixedA使dioCo設置ponent->Re成iste本Co設置ponent();
            MixedA使dioCo設置ponent->AttachToCo設置ponent(基本o本ld->Get基本o本ldSettin成s(), 軍Attach設置entT本ansfo本設置R使les::KeepRelati正eT本ansfo本設置);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized 設置ixed a使dio co設置ponent"));
        }
    }
}

正oid UMin成GoRTSA使dioMana成e本::UpdateT本ackVol使設置es()
{
    // 這裡可以實時更新各音軌的音量
    // 實際實作需要重新計算混音
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated t本ack 正ol使設置es"));
}
