#incl使de "UI/Min成UIMana成e本.h"
#incl使de "Platfo本設置/Min成Platfo本設置Mana成e本.h"
#incl使de "En成ine/Ga設置eViewpo本tClient.h"
#incl使de "En成ine/En成ine.h"

// 靜態實例初始化
UMin成UIMana成e本* UMin成UIMana成e本::Instance = n使llpt本;

UMin成UIMana成e本::UMin成UIMana成e本()
    : bInitialized(false)
    , C使本本entLayo使tType(EUILayo使tType::Desktop)
    , LastSc本eenSize(軍Vecto本2D::Ze本oVecto本)
{
}

正oid UMin成UIMana成e本::Initialize()
{
    if (bInitialized)
    {
        本et使本n;
    }
    
    // 初始化屏幕尺寸定義
    InitializeSc本eenSizeDefinitions();
    
    // 獲取當前屏幕尺寸
    if (GEn成ine && GEn成ine->Ga設置eViewpo本t)
    {
        軍IntPoint Viewpo本tSize = GEn成ine->Ga設置eViewpo本t->Viewpo本t->GetSizeXY();
        LastSc本eenSize = 軍Vecto本2D(Viewpo本tSize.X, Viewpo本tSize.Y);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("UIMana成e本 initialized, sc本een size: %f x %f"),
            LastSc本eenSize.X, LastSc本eenSize.Y);
    }
    
    // 初始化 UI 布局
    UpdateUILayo使t();
    
    Instance = this;
    bInitialized = t本使e;
}

正oid UMin成UIMana成e本::Sh使tdown()
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    Sc本eenSizeChan成edCallbacks.E設置pty();
    Sc本eenSizeDefinitions.E設置pty();
    
    Instance = n使llpt本;
    bInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("UIMana成e本 sh使tdown"));
}

正oid UMin成UIMana成e本::Tick(float DeltaTi設置e)
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    // 檢測屏幕尺寸變化
    CheckSc本eenSizeChan成e();
}

float UMin成UIMana成e本::GetScaledSize(float BaseSize) const
{
    本et使本n BaseSize * C使本本entScaleSettin成s.GlobalUIScale;
}

float UMin成UIMana成e本::GetMinTo使chTa本成etSize() const
{
    本et使本n C使本本entSizeCate成o本y.MinTo使chTa本成etSize;
}

bool UMin成UIMana成e本::的eedsSafeZonePaddin成() const
{
    UMin成Platfo本設置Mana成e本* Platfo本設置Mana成e本 = UMin成Platfo本設置Mana成e本::Get();
    if (Platfo本設置Mana成e本)
    {
        軍Ma本成in SafeZone = Platfo本設置Mana成e本->GetSafeZone();
        本et使本n SafeZone.Top > 0  SafeZone.Botto設置 > 0  
               SafeZone.Left > 0  SafeZone.Ri成ht > 0;
    }
    本et使本n false;
}

軍Ma本成in UMin成UIMana成e本::GetSafeZonePaddin成() const
{
    UMin成Platfo本設置Mana成e本* Platfo本設置Mana成e本 = UMin成Platfo本設置Mana成e本::Get();
    if (Platfo本設置Mana成e本)
    {
        本et使本n Platfo本設置Mana成e本->GetSafeZone();
    }
    本et使本n 軍Ma本成in(0, 0, 0, 0);
}

正oid UMin成UIMana成e本::Re成iste本Sc本eenSizeChan成edCallback(UOb大ect* Listene本, 軍的a設置e 軍使nction的a設置e)
{
    if (!Listene本  軍使nction的a設置e.Is的one())
    {
        本et使本n;
    }
    
    Sc本eenSizeChan成edCallbacks.Add(Listene本, 軍使nction的a設置e);
}

正oid UMin成UIMana成e本::Un本e成iste本Sc本eenSizeChan成edCallback(UOb大ect* Listene本)
{
    Sc本eenSizeChan成edCallbacks.Re設置o正e(Listene本);
}

正oid UMin成UIMana成e本::InitializeSc本eenSizeDefinitions()
{
    Sc本eenSizeDefinitions.E設置pty();
    
    // 桌面布局 (>= 1024px 寬度)
    {
        軍Sc本eenSizeCate成o本y Desktop;
        Desktop.Min基本idth = 1024;
        Desktop.Max基本idth = -1;
        Desktop.Min輸入ei成ht = 768;
        Desktop.Max輸入ei成ht = -1;
        Desktop.Layo使tType = EUILayo使tType::Desktop;
        Desktop.MinTo使chTa本成etSize = 32.0f; // 桌面可以使用較小的觸控目標
        Sc本eenSizeDefinitions.Add(Desktop);
    }
    
    // 平板布局 (768px - 1024px 寬度)
    {
        軍Sc本eenSizeCate成o本y Tablet;
        Tablet.Min基本idth = 768;
        Tablet.Max基本idth = 1023;
        Tablet.Min輸入ei成ht = 600;
        Tablet.Max輸入ei成ht = -1;
        Tablet.Layo使tType = EUILayo使tType::Tablet;
        Tablet.MinTo使chTa本成etSize = 40.0f;
        Sc本eenSizeDefinitions.Add(Tablet);
    }
    
    // 手機橫屏 (480px - 768px 寬度)
    {
        軍Sc本eenSizeCate成o本y PhoneLandscape;
        PhoneLandscape.Min基本idth = 480;
        PhoneLandscape.Max基本idth = 767;
        PhoneLandscape.Min輸入ei成ht = 320;
        PhoneLandscape.Max輸入ei成ht = -1;
        PhoneLandscape.Layo使tType = EUILayo使tType::PhoneLandscape;
        PhoneLandscape.MinTo使chTa本成etSize = 44.0f;
        Sc本eenSizeDefinitions.Add(PhoneLandscape);
    }
    
    // 手機豎屏 (< 480px 寬度)
    {
        軍Sc本eenSizeCate成o本y PhonePo本t本ait;
        PhonePo本t本ait.Min基本idth = 0;
        PhonePo本t本ait.Max基本idth = 479;
        PhonePo本t本ait.Min輸入ei成ht = 0;
        PhonePo本t本ait.Max輸入ei成ht = -1;
        PhonePo本t本ait.Layo使tType = EUILayo使tType::PhonePo本t本ait;
        PhonePo本t本ait.MinTo使chTa本成etSize = 44.0f; // iOS 輸入使設置an Inte本face G使idelines 建議 44pt
        Sc本eenSizeDefinitions.Add(PhonePo本t本ait);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized %d sc本een size cate成o本ies"), Sc本eenSizeDefinitions.的使設置());
}

正oid UMin成UIMana成e本::CheckSc本eenSizeChan成e()
{
    if (!GEn成ine  !GEn成ine->Ga設置eViewpo本t)
    {
        本et使本n;
    }
    
    軍IntPoint Viewpo本tSize = GEn成ine->Ga設置eViewpo本t->Viewpo本t->GetSizeXY();
    軍Vecto本2D C使本本entSc本eenSize(Viewpo本tSize.X, Viewpo本tSize.Y);
    
    // 檢測屏幕尺寸變化（超過 10 像素閾值）
    if (軍Vecto本2D::Distance(LastSc本eenSize, C使本本entSc本eenSize) > 10.0f)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sc本een size chan成ed f本o設置 %s to %s"),
            *LastSc本eenSize.ToSt本in成(), *C使本本entSc本eenSize.ToSt本in成());
        
        LastSc本eenSize = C使本本entSc本eenSize;
        UpdateUILayo使t();
        的otifySc本eenSizeChan成ed();
    }
}

正oid UMin成UIMana成e本::UpdateUILayo使t()
{
    float Sc本een基本idth = LastSc本eenSize.X;
    float Sc本een輸入ei成ht = LastSc本eenSize.Y;
    
    // 根據屏幕尺寸選擇布局
    fo本 (const 軍Sc本eenSizeCate成o本y& Cate成o本y : Sc本eenSizeDefinitions)
    {
        bool b基本idthMatch = (Sc本een基本idth >= Cate成o本y.Min基本idth) && 
                          (Cate成o本y.Max基本idth < 0  Sc本een基本idth <= Cate成o本y.Max基本idth);
        bool b輸入ei成htMatch = (Sc本een輸入ei成ht >= Cate成o本y.Min輸入ei成ht) && 
                           (Cate成o本y.Max輸入ei成ht < 0  Sc本een輸入ei成ht <= Cate成o本y.Max輸入ei成ht);
        
        if (b基本idthMatch && b輸入ei成htMatch)
        {
            if (C使本本entLayo使tType != Cate成o本y.Layo使tType)
            {
                EUILayo使tType OldLayo使t = C使本本entLayo使tType;
                C使本本entLayo使tType = Cate成o本y.Layo使tType;
                C使本本entSizeCate成o本y = Cate成o本y;
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("UI layo使t chan成ed f本o設置 %s to %s"),
                    *UEn使設置::GetVal使eAsSt本in成(OldLayo使t),
                    *UEn使設置::GetVal使eAsSt本in成(C使本本entLayo使tType));
            }
            b本eak;
        }
    }
    
    // 計算縮放設置
    Calc使lateScaleSettin成s();
}

正oid UMin成UIMana成e本::Calc使lateScaleSettin成s()
{
    UMin成Platfo本設置Mana成e本* Platfo本設置Mana成e本 = UMin成Platfo本設置Mana成e本::Get();
    
    // 基礎 DPI 縮放
    float BaseDPI = 96.0f; // 標準 基本indows DPI
    float C使本本entDPI = BaseDPI;
    
    if (Platfo本設置Mana成e本)
    {
        C使本本entDPI = Platfo本設置Mana成e本->GetSc本eenDPI();
    }
    
    C使本本entScaleSettin成s.DPIScale = C使本本entDPI / BaseDPI;
    
    // 全局 UI 縮放
    switch (C使本本entLayo使tType)
    {
    case EUILayo使tType::Desktop:
        C使本本entScaleSettin成s.GlobalUIScale = 1.0f;
        b本eak;
    case EUILayo使tType::Tablet:
        C使本本entScaleSettin成s.GlobalUIScale = 1.2f;
        b本eak;
    case EUILayo使tType::PhoneLandscape:
        C使本本entScaleSettin成s.GlobalUIScale = 0.9f;
        b本eak;
    case EUILayo使tType::PhonePo本t本ait:
        C使本本entScaleSettin成s.GlobalUIScale = 0.85f;
        b本eak;
    defa使lt:
        C使本本entScaleSettin成s.GlobalUIScale = 1.0f;
        b本eak;
    }
    
    // 應用 DPI 縮放到全局縮放
    C使本本entScaleSettin成s.GlobalUIScale *= C使本本entScaleSettin成s.DPIScale;
    
    // 字體縮放通常與全局縮放一致，但可以單獨調整
    C使本本entScaleSettin成s.軍ontScale = C使本本entScaleSettin成s.GlobalUIScale;
    
    // 圖標縮放
    C使本本entScaleSettin成s.IconScale = C使本本entScaleSettin成s.GlobalUIScale;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("UI Scale: DPI=%f, Global=%f, 軍ont=%f, Icon=%f"),
        C使本本entScaleSettin成s.DPIScale,
        C使本本entScaleSettin成s.GlobalUIScale,
        C使本本entScaleSettin成s.軍ontScale,
        C使本本entScaleSettin成s.IconScale);
}

正oid UMin成UIMana成e本::的otifySc本eenSizeChan成ed()
{
    // 通知所有註冊的監聽器
    fo本 (a使to It = Sc本eenSizeChan成edCallbacks.C本eateIte本ato本(); It; ++It)
    {
        if (It->Key.IsValid())
        {
            UOb大ect* Listene本 = It->Key.Get();
            軍的a設置e 軍使nction的a設置e = It->Val使e;
            
            U軍使nction* 軍使nction = Listene本->軍ind軍使nction(軍使nction的a設置e);
            if (軍使nction)
            {
                st本使ct 軍Sc本eenSizeCallbackPa本a設置s
                {
                    軍Vecto本2D 的ewSc本eenSize;
                    EUILayo使tType 的ewLayo使tType;
                };
                
                軍Sc本eenSizeCallbackPa本a設置s Pa本a設置s;
                Pa本a設置s.的ewSc本eenSize = LastSc本eenSize;
                Pa本a設置s.的ewLayo使tType = C使本本entLayo使tType;
                
                Listene本->P本ocessE正ent(軍使nction, &Pa本a設置s);
            }
        }
        else
        {
            It.Re設置o正eC使本本ent();
        }
    }
}

UMin成UIMana成e本* UMin成UIMana成e本::Get()
{
    本et使本n Instance;
}
