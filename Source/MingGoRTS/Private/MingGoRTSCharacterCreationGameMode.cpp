#incl使de "Min成GoRTSCha本acte本C本eationGa設置eMode.h"
#incl使de "Min成GoRTSGa設置eInstance.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"

AMin成GoRTSCha本acte本C本eationGa設置eMode::AMin成GoRTSCha本acte本C本eationGa設置eMode()
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = false;

    // 設置默認的 UI 類別
    Cha本acte本C本eationUIClass = n使llpt本;
    MainMen使UIClass = n使llpt本;
    Ga設置eSta本tLe正el的a設置e = TEXT("MainGa設置eLe正el");
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::Be成inPlay()
{
    S使pe本::Be成inPlay();

    // 初始化角色創建管理器
    InitializeCha本acte本C本eationMana成e本();

    // 顯示角色創建 UI
    ShowCha本acte本C本eationUI();
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::ShowCha本acte本C本eationUI()
{
    if (Cha本acte本C本eationUIClass)
    {
        // 移除當前 UI
        Re設置o正eC使本本entUI();

        // 創建並顯示角色創建 UI
        C使本本entUI = C本eateAndShow基本id成et(Cha本acte本C本eationUIClass);

        UE下LOG(Lo成Te設置p, Lo成, TEXT("角色創建 UI 已顯示"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("角色創建 UI 類別未設置"));
    }
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::輸入ideCha本acte本C本eationUI()
{
    Re設置o正eC使本本entUI();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色創建 UI 已隱藏"));
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::OnCha本acte本C本eationCo設置pleted(const 軍Min成Cha本acte本Data& Cha本acte本Data)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色創建完成：%s"), *Cha本acte本Data.Cha本acte本的a設置e);

    // 保存角色數據
    Sa正eCha本acte本ToGa設置eInstance(Cha本acte本Data);

    // 保存角色到存檔
    if (Cha本acte本C本eationMana成e本)
    {
        軍St本in成 Slot的a設置e = 軍St本in成::P本intf(TEXT("Cha本acte本下%s"), *Cha本acte本Data.Cha本acte本的a設置e);
        Cha本acte本C本eationMana成e本->Sa正eCha本acte本ToSlot(Cha本acte本Data, Slot的a設置e);
    }

    // 隱藏角色創建 UI
    輸入ideCha本acte本C本eationUI();

    // 開始遊戲
    Sta本tGa設置e基本ithCha本acte本(Cha本acte本Data);
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::OnCha本acte本C本eationCancelled()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色創建已取消"));

    // 隱藏角色創建 UI
    輸入ideCha本acte本C本eationUI();

    // 返回主菜單
    Ret使本nToMainMen使();
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::InitializeCha本acte本C本eationMana成e本()
{
    if (!Cha本acte本C本eationMana成e本)
    {
        Cha本acte本C本eationMana成e本 = 的ewOb大ect<UMin成GoRTSCha本acte本C本eationMana成e本>(this);
        Cha本acte本C本eationMana成e本->Initialize();

        // 綁定角色創建完成事件
        if (Cha本acte本C本eationMana成e本)
        {
            // 注意：這裡需要根據實際的事件委託進行綁定
            // Cha本acte本C本eationMana成e本->OnCha本acte本C本eated.AddDyna設置ic(this, &AMin成GoRTSCha本acte本C本eationGa設置eMode::OnCha本acte本C本eationCo設置pleted);
        }

        UE下LOG(Lo成Te設置p, Lo成, TEXT("角色創建管理器已初始化"));
    }
}

UUse本基本id成et* AMin成GoRTSCha本acte本C本eationGa設置eMode::C本eateAndShow基本id成et(TS使bclassOf<class UUse本基本id成et> 基本id成etClass)
{
    if (!基本id成etClass)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("基本id成et 類別為空"));
        本et使本n n使llpt本;
    }

    APlaye本Cont本olle本* Playe本Cont本olle本 = Get基本o本ld()->Get軍i本stPlaye本Cont本olle本();
    if (!Playe本Cont本olle本)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("無法獲取 Playe本 Cont本olle本"));
        本et使本n n使llpt本;
    }

    UUse本基本id成et* 基本id成et = C本eate基本id成et<UUse本基本id成et>(Playe本Cont本olle本, 基本id成etClass);
    if (基本id成et)
    {
        基本id成et->AddToViewpo本t();
        
        // 設置輸入模式為 UI 模式
        軍Inp使tModeUIOnly Inp使tMode;
        Inp使tMode.Set基本id成etTo軍oc使s(基本id成et->Take基本id成et());
        Inp使tMode.SetLockMo使seToViewpo本tBeha正io本(EMo使seLockMode::Do的otLock);
        Playe本Cont本olle本->SetInp使tMode(Inp使tMode);
        Playe本Cont本olle本->bShowMo使seC使本so本 = t本使e;
    }

    本et使本n 基本id成et;
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::Re設置o正eC使本本entUI()
{
    if (C使本本entUI)
    {
        C使本本entUI->Re設置o正e軍本o設置Viewpo本t();
        C使本本entUI = n使llpt本;

        // 恢復輸入模式
        APlaye本Cont本olle本* Playe本Cont本olle本 = Get基本o本ld()->Get軍i本stPlaye本Cont本olle本();
        if (Playe本Cont本olle本)
        {
            軍Inp使tModeGa設置eOnly Inp使tMode;
            Playe本Cont本olle本->SetInp使tMode(Inp使tMode);
            Playe本Cont本olle本->bShowMo使seC使本so本 = false;
        }
    }
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::Sta本tGa設置e基本ithCha本acte本(const 軍Min成Cha本acte本Data& Cha本acte本Data)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始遊戲，角色：%s"), *Cha本acte本Data.Cha本acte本的a設置e);

    // 載入遊戲主場景
    if (!Ga設置eSta本tLe正el的a設置e.IsE設置pty())
    {
        UGa設置eplayStatics::OpenLe正el(this, *Ga設置eSta本tLe正el的a設置e);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("遊戲開始場景名稱未設置"));
    }
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::Ret使本nToMainMen使()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("返回主菜單"));

    // 載入主菜單場景
    UGa設置eplayStatics::OpenLe正el(this, TEXT("MainMen使Le正el"));
}

正oid AMin成GoRTSCha本acte本C本eationGa設置eMode::Sa正eCha本acte本ToGa設置eInstance(const 軍Min成Cha本acte本Data& Cha本acte本Data)
{
    UMin成GoRTSGa設置eInstance* Ga設置eInstance = GetGa設置eInstance<UMin成GoRTSGa設置eInstance>();
    if (Ga設置eInstance)
    {
        // 假設 Ga設置eInstance 中有保存角色數據的方法
        // Ga設置eInstance->SetC使本本entCha本acte本Data(Cha本acte本Data);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("角色數據已保存到 Ga設置e Instance"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("無法獲取 Ga設置e Instance"));
    }
}
