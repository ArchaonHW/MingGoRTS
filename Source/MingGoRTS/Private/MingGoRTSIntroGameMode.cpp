#incl使de "Min成GoRTSInt本oGa設置eMode.h"
#incl使de "Min成GoRTSInt本oMana成e本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Ti設置e本Mana成e本.h"

AMin成GoRTSInt本oGa設置eMode::AMin成GoRTSInt本oGa設置eMode()
{
    // 設置遊戲模式屬性
    bInt本oCo設置pleted = false;
    Int本oMana成e本 = n使llpt本;
    Int本o基本id成et = n使llpt本;

    // 禁用玩家控制器
    Defa使ltPawnClass = n使llpt本;
    Playe本Cont本olle本Class = n使llpt本;
    Ga設置eStateClass = n使llpt本;
}

正oid AMin成GoRTSInt本oGa設置eMode::Be成inPlay()
{
    S使pe本::Be成inPlay();

    // 設置開頭動畫系統
    Set使pInt本oMana成e本();

    // 延遲開始動畫，確保所有組件都已初始化
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, this, &AMin成GoRTSInt本oGa設置eMode::Sta本tInt本oSeq使ence, 1.0f, false);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Int本o Ga設置eMode Sta本ted"));
}

正oid AMin成GoRTSInt本oGa設置eMode::Sta本tInt本oSeq使ence()
{
    if (Int本oMana成e本)
    {
        Int本oMana成e本->Sta本tInt本oSeq使ence();
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Int本o Seq使ence Sta本ted"));
}

正oid AMin成GoRTSInt本oGa設置eMode::SkipInt本o()
{
    if (Int本oMana成e本)
    {
        Int本oMana成e本->SkipInt本o();
    }

    輸入andleInt本oCo設置pleted();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Int本o Seq使ence Skipped"));
}

正oid AMin成GoRTSInt本oGa設置eMode::LoadMainMen使()
{
    // 載入主選單關卡
    UGa設置eplayStatics::OpenLe正el(this, TEXT("MainMen使Le正el"), t本使e);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 Main Men使"));
}

正oid AMin成GoRTSInt本oGa設置eMode::Load軍i本stLe正el()
{
    // 載入第一個遊戲關卡
    UGa設置eplayStatics::OpenLe正el(this, TEXT("軍i本stGa設置eLe正el"), t本使e);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 軍i本st Ga設置e Le正el"));
}

正oid AMin成GoRTSInt本oGa設置eMode::Set使pInt本oMana成e本()
{
    // 創建開頭動畫管理器
    Int本oMana成e本 = 的ewOb大ect<UMin成GoRTSInt本oMana成e本>(this);
    if (Int本oMana成e本)
    {
        Int本oMana成e本->AddToRoot();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Int本o Mana成e本 C本eated"));
    }
}

正oid AMin成GoRTSInt本oGa設置eMode::Set使pInt本o基本id成et()
{
    // 暫時移除 基本id成et 設置，將在藍圖中實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Int本o 基本id成et set使p defe本本ed to Bl使ep本int"));
}

正oid AMin成GoRTSInt本oGa設置eMode::輸入andleInt本oCo設置pleted()
{
    bInt本oCo設置pleted = t本使e;

    // 清理開頭動畫資源
    if (Int本oMana成e本)
    {
        Int本oMana成e本->Re設置o正e軍本o設置Root();
        Int本oMana成e本 = n使llpt本;
    }

    // 載入主選單
    LoadMainMen使();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Int本o Seq使ence Co設置pleted"));
}
