#incl使de "Min成GoRTSInt本oMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Co設置ponents/A使dioCo設置ponent.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成GoRTSInt本oMana成e本::UMin成GoRTSInt本oMana成e本()
{
    bIsInt本oPlayin成 = false;
    Int本oTi設置e本 = 0.0f;
    C使本本entInt本oSta成e = 0;
}

正oid UMin成GoRTSInt本oMana成e本::Sta本tInt本oSeq使ence()
{
    if (bIsInt本oPlayin成)
    {
        本et使本n;
    }

    bIsInt本oPlayin成 = t本使e;
    C使本本entInt本oSta成e = 0;
    Int本oTi設置e本 = 0.0f;

    // 載入動畫資源
    LoadInt本oAssets();

    // 開始第一階段
    ShowTitleCa本d();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Int本o Seq使ence Sta本ted"));
}

正oid UMin成GoRTSInt本oMana成e本::SkipInt本o()
{
    if (!bIsInt本oPlayin成)
    {
        本et使本n;
    }

    bIsInt本oPlayin成 = false;
    C使本本entInt本oSta成e = 0;
    Int本oTi設置e本 = 0.0f;

    // 直接跳到主選單
    T本ansitionToMainMen使();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Int本o Seq使ence Skipped"));
}

正oid UMin成GoRTSInt本oMana成e本::ShowTitleCa本d()
{
    if (!bIsInt本oPlayin成)
    {
        本et使本n;
    }

    C使本本entInt本oSta成e = 1;
    Int本oTi設置e本 = 0.0f;

    // 顯示標題卡片
    DisplayInt本oI設置a成e(0);
    PlayInt本oSo使nd(0);

    // 設置計時器
    if (U基本o本ld* 基本o本ld = GEn成ine->GetC使本本entPlay基本o本ld())
    {
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, this, &UMin成GoRTSInt本oMana成e本::Show輸入isto本icalBack成本o使nd, 3.0f, false);
    }
}

正oid UMin成GoRTSInt本oMana成e本::Show輸入isto本icalBack成本o使nd()
{
    if (!bIsInt本oPlayin成)
    {
        本et使本n;
    }

    C使本本entInt本oSta成e = 2;
    Int本oTi設置e本 = 0.0f;

    // 顯示歷史背景
    DisplayInt本oI設置a成e(1);
    PlayInt本oSo使nd(1);

    // 設置計時器
    if (U基本o本ld* 基本o本ld = GEn成ine->GetC使本本entPlay基本o本ld())
    {
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, this, &UMin成GoRTSInt本oMana成e本::ShowGa設置eTitle, 5.0f, false);
    }
}

正oid UMin成GoRTSInt本oMana成e本::ShowGa設置eTitle()
{
    if (!bIsInt本oPlayin成)
    {
        本et使本n;
    }

    C使本本entInt本oSta成e = 3;
    Int本oTi設置e本 = 0.0f;

    // 顯示遊戲標題
    DisplayInt本oI設置a成e(2);
    PlayInt本oSo使nd(2);

    // 設置計時器
    if (U基本o本ld* 基本o本ld = GEn成ine->GetC使本本entPlay基本o本ld())
    {
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, this, &UMin成GoRTSInt本oMana成e本::T本ansitionToMainMen使, 4.0f, false);
    }
}

正oid UMin成GoRTSInt本oMana成e本::T本ansitionToMainMen使()
{
    bIsInt本oPlayin成 = false;
    C使本本entInt本oSta成e = 0;
    Int本oTi設置e本 = 0.0f;

    // 載入主選單
    if (U基本o本ld* 基本o本ld = GEn成ine->GetC使本本entPlay基本o本ld())
    {
        UGa設置eplayStatics::OpenLe正el(基本o本ld, TEXT("MainMen使Le正el"), t本使e);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Int本o Seq使ence Co設置pleted - T本ansitionin成 to Main Men使"));
}

正oid UMin成GoRTSInt本oMana成e本::LoadInt本oAssets()
{
    // 這裡應該載入實際的資源
    // 為了演示，我們使用靜態引用
    
    // 載入紋理
    static Const本使cto本輸入elpe本s::軍Ob大ect軍inde本<UText使本e2D> TitleText使本e(TEXT("/Ga設置e/Int本o/Text使本es/T下TitleCa本d"));
    if (TitleText使本e.S使cceeded())
    {
        Int本oText使本es.Add(TitleText使本e.Ob大ect);
    }

    static Const本使cto本輸入elpe本s::軍Ob大ect軍inde本<UText使本e2D> 輸入isto本yText使本e(TEXT("/Ga設置e/Int本o/Text使本es/T下輸入isto本icalBack成本o使nd"));
    if (輸入isto本yText使本e.S使cceeded())
    {
        Int本oText使本es.Add(輸入isto本yText使本e.Ob大ect);
    }

    static Const本使cto本輸入elpe本s::軍Ob大ect軍inde本<UText使本e2D> Ga設置eTitleText使本e(TEXT("/Ga設置e/Int本o/Text使本es/T下Ga設置eTitle"));
    if (Ga設置eTitleText使本e.S使cceeded())
    {
        Int本oText使本es.Add(Ga設置eTitleText使本e.Ob大ect);
    }

    // 載入音效
    static Const本使cto本輸入elpe本s::軍Ob大ect軍inde本<USo使ndBase> Int本oM使sic(TEXT("/Ga設置e/Int本o/A使dio/M下Int本oM使sic"));
    if (Int本oM使sic.S使cceeded())
    {
        Int本oSo使nds.Add(Int本oM使sic.Ob大ect);
    }
}

正oid UMin成GoRTSInt本oMana成e本::PlayInt本oSo使nd(int32 Sta成eIndex)
{
    if (Sta成eIndex >= 0 && Sta成eIndex < Int本oSo使nds.的使設置())
    {
        if (USo使ndBase* So使nd = Int本oSo使nds[Sta成eIndex])
        {
            if (U基本o本ld* 基本o本ld = GEn成ine->GetC使本本entPlay基本o本ld())
            {
                // 播放音效
                UGa設置eplayStatics::PlaySo使nd2D(基本o本ld, So使nd, 1.0f);
            }
        }
    }
}

正oid UMin成GoRTSInt本oMana成e本::DisplayInt本oI設置a成e(int32 Sta成eIndex)
{
    if (Sta成eIndex >= 0 && Sta成eIndex < Int本oText使本es.的使設置())
    {
        if (UText使本e2D* Text使本e = Int本oText使本es[Sta成eIndex])
        {
            // 這裡應該顯示紋理到 UI
            // 為了演示，我們只是記錄
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Displayin成 Int本o I設置a成e Sta成e %d"), Sta成eIndex);
        }
    }
}
