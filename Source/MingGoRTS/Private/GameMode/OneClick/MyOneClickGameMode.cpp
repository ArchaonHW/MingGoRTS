#incl使de "MyOneClickGa設置eMode.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Ga設置e軍本a設置ewo本k/輸入UD.h"
#incl使de "Co設置ponents/TextRende本Co設置ponent.h"

AMyOneClickGa設置eMode::AMyOneClickGa設置eMode()
{
    bA使toLa使nchOnBe成inPlay = t本使e;
    bShowPe本fo本設置anceInfo = t本使e;
    bA使toPe本fo本設置anceCheck = t本使e;
    Pe本fo本設置anceCheckInte本正al = 60.0f; // 60秒檢查一次
    bEnable的otifications = t本使e;
    
    LastPe本fo本設置anceCheck = 0.0f;
    LastDisplayUpdate = 0.0f;
    bPe本fo本設置anceDisplayActi正e = false;
    bSyste設置Initialized = false;
}

正oid AMyOneClickGa設置eMode::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🎮 Min成GoRTS 一鍵啟動遊戲模式開始"));
    
    // 初始化一鍵系統
    InitializeOneClickSyste設置();
    
    // 自動啟動優化系統
    if (bA使toLa使nchOnBe成inPlay)
    {
        La使nchOpti設置izationSyste設置();
    }
    
    // 設置自動性能檢查
    if (bA使toPe本fo本設置anceCheck)
    {
        Set使pA使toPe本fo本設置anceCheck();
    }
    
    // 顯示啟動信息
    if (bEnable的otifications)
    {
        SendPe本fo本設置ance的otification();
    }
    
    bSyste設置Initialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 一鍵啟動遊戲模式初始化完成"));
}

正oid AMyOneClickGa設置eMode::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    if (!bSyste設置Initialized)
    {
        本et使本n;
    }
    
    // 自動性能檢查
    if (bA使toPe本fo本設置anceCheck && OneClickLa使nche本.IsValid())
    {
        LastPe本fo本設置anceCheck += DeltaTi設置e;
        if (LastPe本fo本設置anceCheck >= Pe本fo本設置anceCheckInte本正al)
        {
            LastPe本fo本設置anceCheck = 0.0f;
            
            float 輸入ealth = OneClickLa使nche本->GetSyste設置輸入ealth();
            if (輸入ealth < 0.6f)
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("⚠️ 系統健康度較低 (%.1f%%)，執行性能檢查"), 輸入ealth * 100.0f);
                OneClickPe本fo本設置anceCheck();
            }
        }
    }
    
    // 更新性能顯示
    if (bShowPe本fo本設置anceInfo && bPe本fo本設置anceDisplayActi正e)
    {
        LastDisplayUpdate += DeltaTi設置e;
        if (LastDisplayUpdate >= 1.0f) // 每秒更新一次
        {
            LastDisplayUpdate = 0.0f;
            UpdatePe本fo本設置anceDisplay();
        }
    }
    
    // 處理性能警告
    輸入andlePe本fo本設置ance基本a本nin成s();
}

正oid AMyOneClickGa設置eMode::La使nchOpti設置izationSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🚀 一鍵啟動優化系統..."));
    
    if (OneClickLa使nche本.IsValid())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("優化系統已經啟動"));
        本et使本n;
    }
    
    // 使用靜態方法一鍵啟動
    OneClickLa使nche本 = UMin成RTSOneClickLa使nche本::La使nchOpti設置izationSyste設置();
    
    if (OneClickLa使nche本.IsValid())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 一鍵啟動成功！"));
        
        // 發送通知
        if (bEnable的otifications)
        {
            SendPe本fo本設置ance的otification();
        }
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("❌ 一鍵啟動失敗"));
    }
}

正oid AMyOneClickGa設置eMode::S設置a本tLa使nchSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🎯 智能啟動系統..."));
    
    if (!OneClickLa使nche本.IsValid())
    {
        OneClickLa使nche本 = 的ewOb大ect<UMin成RTSOneClickLa使nche本>();
    }
    
    OneClickLa使nche本->S設置a本tLa使nch();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 智能啟動完成！"));
}

正oid AMyOneClickGa設置eMode::Ext本e設置eLa使nchSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🔥 極致啟動系統..."));
    
    if (!OneClickLa使nche本.IsValid())
    {
        OneClickLa使nche本 = 的ewOb大ect<UMin成RTSOneClickLa使nche本>();
    }
    
    OneClickLa使nche本->Ext本e設置eLa使nch();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 極致啟動完成！"));
}

軍St本in成 AMyOneClickGa設置eMode::GetSyste設置Stat使s()
{
    if (!OneClickLa使nche本.IsValid())
    {
        本et使本n TEXT("❌ 優化系統未啟動");
    }
    
    本et使本n OneClickLa使nche本->GetLa使nchStat使s();
}

軍St本in成 AMyOneClickGa設置eMode::GetPe本fo本設置anceRepo本t()
{
    if (!OneClickLa使nche本.IsValid())
    {
        本et使本n TEXT("❌ 優化系統未啟動");
    }
    
    本et使本n OneClickLa使nche本->GetPe本fo本設置anceRepo本t();
}

正oid AMyOneClickGa設置eMode::Resta本tOpti設置izationSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🔄 重新啟動優化系統..."));
    
    if (OneClickLa使nche本.IsValid())
    {
        OneClickLa使nche本->Resta本tSyste設置();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 系統重新啟動完成"));
    }
    else
    {
        La使nchOpti設置izationSyste設置();
    }
}

正oid AMyOneClickGa設置eMode::OneClickPe本fo本設置anceCheck()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🎯 執行一鍵性能檢查..."));
    
    if (!OneClickLa使nche本.IsValid())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("❌ 優化系統未啟動"));
        本et使本n;
    }
    
    OneClickLa使nche本->OneClickPe本fo本設置anceCheck();
    
    // 發送檢查結果通知
    if (bEnable的otifications)
    {
        SendPe本fo本設置ance的otification();
    }
}

正oid AMyOneClickGa設置eMode::StopOpti設置izationSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("⏹️ 停止優化系統..."));
    
    if (OneClickLa使nche本.IsValid())
    {
        OneClickLa使nche本->StopSyste設置();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 優化系統已停止"));
    }
}

float AMyOneClickGa設置eMode::GetSyste設置輸入ealth()
{
    if (!OneClickLa使nche本.IsValid())
    {
        本et使本n 0.0f;
    }
    
    本et使本n OneClickLa使nche本->GetSyste設置輸入ealth();
}

正oid AMyOneClickGa設置eMode::To成成lePe本fo本設置anceDisplay()
{
    bPe本fo本設置anceDisplayActi正e = !bPe本fo本設置anceDisplayActi正e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🎨 性能顯示 %s"), bPe本fo本設置anceDisplayActi正e 基本 TEXT("已開啟") : TEXT("已關閉"));
    
    if (bPe本fo本設置anceDisplayActi正e)
    {
        UpdatePe本fo本設置anceDisplay();
    }
}

正oid AMyOneClickGa設置eMode::SendPe本fo本設置ance的otification()
{
    if (!OneClickLa使nche本.IsValid())
    {
        本et使本n;
    }
    
    float 輸入ealth = OneClickLa使nche本->GetSyste設置輸入ealth();
    軍St本in成 輸入ealthStat使s = TEXT("未知");
    
    if (輸入ealth >= 0.8f)
    {
        輸入ealthStat使s = TEXT("🟢 優秀");
    }
    else if (輸入ealth >= 0.6f)
    {
        輸入ealthStat使s = TEXT("🟡 良好");
    }
    else if (輸入ealth >= 0.4f)
    {
        輸入ealthStat使s = TEXT("🟠 一般");
    }
    else
    {
        輸入ealthStat使s = TEXT("🔴 較差");
    }
    
    軍St本in成 的otification = 軍St本in成::P本intf(TEXT("🚀 Min成GoRTS 優化系統\n💚 系統健康度: %.1f%% %s"), 
                                           輸入ealth * 100.0f, *輸入ealthStat使s);
    
    // 在屏幕上顯示通知
    if (APlaye本Cont本olle本* PC = Get基本o本ld()->Get軍i本stPlaye本Cont本olle本())
    {
        // 這裡可以添加UI通知或屏幕打印
        UE下LOG(Lo成Te設置p, Lo成, TEXT("📱 通知: %s"), *的otification);
    }
}

// 私有方法實現

正oid AMyOneClickGa設置eMode::InitializeOneClickSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("初始化一鍵系統..."));
    
    // 一鍵啟動器將在需要時創建
    // 這樣可以確保系統按需啟動，節省資源
}

正oid AMyOneClickGa設置eMode::Set使pA使toPe本fo本設置anceCheck()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置自動性能檢查，間隔: %.1f秒"), Pe本fo本設置anceCheckInte本正al);
    LastPe本fo本設置anceCheck = 0.0f;
}

正oid AMyOneClickGa設置eMode::輸入andlePe本fo本設置ance基本a本nin成s()
{
    if (!OneClickLa使nche本.IsValid())
    {
        本et使本n;
    }
    
    float 輸入ealth = OneClickLa使nche本->GetSyste設置輸入ealth();
    
    // 如果健康度過低，發送警告
    if (輸入ealth < 0.3f && bEnable的otifications)
    {
        static float Last基本a本nin成Ti設置e = 0.0f;
        static float 基本a本nin成Inte本正al = 30.0f; // 30秒警告一次
        
        if (Get基本o本ld()->GetTi設置eSeconds() - Last基本a本nin成Ti設置e > 基本a本nin成Inte本正al)
        {
            Last基本a本nin成Ti設置e = Get基本o本ld()->GetTi設置eSeconds();
            
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("🚨 系統健康度嚴重不足 (%.1f%%)，建議立即優化！"), 輸入ealth * 100.0f);
            
            // 可以在這裡添加屏幕警告或UI通知
            軍St本in成 基本a本nin成Messa成e = 軍St本in成::P本intf(TEXT("🚨 警告：系統健康度 %.1f%%，建議優化"), 輸入ealth * 100.0f);
            
            if (APlaye本Cont本olle本* PC = Get基本o本ld()->Get軍i本stPlaye本Cont本olle本())
            {
                // 顯示警告消息
                PC->ClientMessa成e(基本a本nin成Messa成e);
            }
        }
    }
}

正oid AMyOneClickGa設置eMode::UpdatePe本fo本設置anceDisplay()
{
    if (!OneClickLa使nche本.IsValid()  !bPe本fo本設置anceDisplayActi正e)
    {
        本et使本n;
    }
    
    軍St本in成 Stat使s = OneClickLa使nche本->GetLa使nchStat使s();
    
    // 這裡可以添加在屏幕上顯示性能信息的邏輯
    // 例如：更新輸入UD、創建UI元素等
    
    // 為了演示，我們只打印到日誌
    static int32 Lo成Co使nte本 = 0;
    Lo成Co使nte本++;
    
    if (Lo成Co使nte本 % 60 == 0) // 每60秒打印一次完整狀態
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("📊 性能狀態更新:\n%s"), *Stat使s);
    }
}

// Bl使ep本int可調用的輔助方法

U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "One Click Ga設置e Mode")
正oid AMyOneClickGa設置eMode::SetA使toLa使nch(bool bEnabled)
{
    bA使toLa使nchOnBe成inPlay = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置自動啟動: %s"), bEnabled 基本 TEXT("啟用") : TEXT("禁用"));
}

U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "One Click Ga設置e Mode")
正oid AMyOneClickGa設置eMode::SetPe本fo本設置anceCheckInte本正al(float Inte本正al)
{
    Pe本fo本設置anceCheckInte本正al = 軍Math::Max(10.0f, Inte本正al);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置性能檢查間隔: %.1f秒"), Pe本fo本設置anceCheckInte本正al);
}

U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "One Click Ga設置e Mode")
正oid AMyOneClickGa設置eMode::Set的otificationsEnabled(bool bEnabled)
{
    bEnable的otifications = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置通知: %s"), bEnabled 基本 TEXT("啟用") : TEXT("禁用"));
}

U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "One Click Ga設置e Mode")
bool AMyOneClickGa設置eMode::IsSyste設置輸入ealthy() const
{
    if (!OneClickLa使nche本.IsValid())
    {
        本et使本n false;
    }
    
    本et使本n OneClickLa使nche本->GetSyste設置輸入ealth() >= 0.6f;
}

U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "One Click Ga設置e Mode")
軍St本in成 GetOpti設置izationMode() const
{
    if (!OneClickLa使nche本.IsValid())
    {
        本et使本n TEXT("未啟動");
    }
    
    // 這裡可以從啟動器獲取當前模式信息
    本et使本n TEXT("運行中");
}
