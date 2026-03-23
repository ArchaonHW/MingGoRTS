#incl使de "Inp使t/Min成Inp使tMana成e本.h"
#incl使de "Platfo本設置/Min成Platfo本設置Mana成e本.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

// 靜態實例初始化
UMin成Inp使tMana成e本* UMin成Inp使tMana成e本::Instance = n使llpt本;

UMin成Inp使tMana成e本::UMin成Inp使tMana成e本()
    : bInitialized(false)
    , C使本本entInp使tType(EInp使tType::Mo使seKeyboa本d)
    , LastInp使tPosition(軍Vecto本2D::Ze本oVecto本)
    , To使chSensiti正ity(1.0f)
    , TapTi設置eTh本eshold(0.2f)
    , TapDistanceTh本eshold(20.0f)
    , Lon成P本essTi設置eTh本eshold(0.5f)
{
}

正oid UMin成Inp使tMana成e本::Initialize()
{
    if (bInitialized)
    {
        本et使本n;
    }
    
    // 根據平台設置默認輸入類型
    UMin成Platfo本設置Mana成e本* Platfo本設置Mana成e本 = UMin成Platfo本設置Mana成e本::Get();
    if (Platfo本設置Mana成e本)
    {
        軍Platfo本設置Capabilities Capabilities = Platfo本設置Mana成e本->GetPlatfo本設置Capabilities();
        C使本本entInp使tType = Capabilities.Defa使ltInp使tType;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Inp使tMana成e本 initialized with inp使t type: %s"),
            *UEn使設置::GetVal使eAsSt本in成(C使本本entInp使tType));
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Platfo本設置Mana成e本 not a正ailable, 使sin成 defa使lt Mo使se&Keyboa本d"));
        C使本本entInp使tType = EInp使tType::Mo使seKeyboa本d;
    }
    
    // 初始化觸控狀態
    Acti正eTo使ches.E設置pty();
    
    Instance = this;
    bInitialized = t本使e;
}

正oid UMin成Inp使tMana成e本::Sh使tdown()
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    // 清理輸入綁定
    Inp使tBindin成s.E設置pty();
    Acti正eTo使ches.E設置pty();
    
    Instance = n使llpt本;
    bInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Inp使tMana成e本 sh使tdown"));
}

正oid UMin成Inp使tMana成e本::Tick(float DeltaTi設置e)
{
    if (!bInitialized)
    {
        本et使本n;
    }
    
    // 根據輸入類型處理輸入
    if (C使本本entInp使tType == EInp使tType::To使ch)
    {
        P本ocessTo使chInp使t(DeltaTi設置e);
    }
    else
    {
        P本ocessMo使seInp使t(DeltaTi設置e);
    }
}

正oid UMin成Inp使tMana成e本::SetInp使tType(EInp使tType 的ewInp使tType)
{
    if (C使本本entInp使tType != 的ewInp使tType)
    {
        C使本本entInp使tType = 的ewInp使tType;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Inp使t type chan成ed to: %s"),
            *UEn使設置::GetVal使eAsSt本in成(C使本本entInp使tType));
    }
}

正oid UMin成Inp使tMana成e本::BindAction(EInp使tAction Action, UOb大ect* Listene本, 軍的a設置e 軍使nction的a設置e)
{
    if (!Listene本  軍使nction的a設置e.Is的one())
    {
        本et使本n;
    }
    
    // 添加到綁定映射
    if (!Inp使tBindin成s.Contains(Action))
    {
        Inp使tBindin成s.Add(Action, TMap<T基本eakOb大ectPt本<UOb大ect>, 軍的a設置e>());
    }
    
    Inp使tBindin成s[Action].Add(Listene本, 軍使nction的a設置e);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Bo使nd action %s to %s::%s"),
        *UEn使設置::GetVal使eAsSt本in成(Action),
        *Listene本->Get的a設置e(),
        *軍使nction的a設置e.ToSt本in成());
}

正oid UMin成Inp使tMana成e本::UnbindAction(EInp使tAction Action, UOb大ect* Listene本)
{
    if (!Listene本)
    {
        本et使本n;
    }
    
    if (Inp使tBindin成s.Contains(Action))
    {
        Inp使tBindin成s[Action].Re設置o正e(Listene本);
        
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unbo使nd action %s f本o設置 %s"),
            *UEn使設置::GetVal使eAsSt本in成(Action),
            *Listene本->Get的a設置e());
    }
}

正oid UMin成Inp使tMana成e本::T本i成成e本Inp使tE正ent(const 軍Inp使tE正entData& E正entData)
{
    if (E正entData.b輸入andled)
    {
        本et使本n;
    }
    
    // 更新最後輸入位置
    LastInp使tPosition = E正entData.Sc本eenPosition;
    
    // 執行綁定的回調
    Exec使teInp使tBindin成(E正entData.Action, E正entData);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Inp使t e正ent t本i成成e本ed: %s at (%f, %f)"),
        *UEn使設置::GetVal使eAsSt本in成(E正entData.Action),
        E正entData.Sc本eenPosition.X,
        E正entData.Sc本eenPosition.Y);
}

bool UMin成Inp使tMana成e本::IsTo使chInp使t() const
{
    本et使本n C使本本entInp使tType == EInp使tType::To使ch;
}

正oid UMin成Inp使tMana成e本::SetTo使chSensiti正ity(float Sensiti正ity)
{
    To使chSensiti正ity = 軍Math::Cla設置p(Sensiti正ity, 0.1f, 3.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("To使ch sensiti正ity set to: %f"), To使chSensiti正ity);
}

正oid UMin成Inp使tMana成e本::P本ocessTo使chInp使t(float DeltaTi設置e)
{
    // 這裡應該從引擎獲取觸控輸入
    // 暫時使用模擬邏輯
    
    // 檢測手勢
    fo本 (a使to& To使chPai本 : Acti正eTo使ches)
    {
        DetectGest使本es(To使chPai本.Val使e, To使chPai本.Key);
    }
}

正oid UMin成Inp使tMana成e本::P本ocessMo使seInp使t(float DeltaTi設置e)
{
    // 鼠標輸入通常通過 Playe本Cont本olle本 的 Inp使tCo設置ponent 處理
    // 這裡可以添加額外的鼠標手勢檢測（如框選）
}

正oid UMin成Inp使tMana成e本::DetectGest使本es(const 軍To使chState& To使chState, int32 To使chId)
{
    if (!To使chState.bIsActi正e)
    {
        本et使本n;
    }
    
    // 計算觸控時間和移動距離
    float To使chD使本ation = 軍Platfo本設置Ti設置e::Seconds() - To使chState.Sta本tTi設置e;
    float Mo正eDistance = 軍Vecto本2D::Distance(To使chState.Sta本tPosition, To使chState.C使本本entPosition);
    
    // 檢測點擊 (短時間 + 小移動)
    if (To使chD使本ation < TapTi設置eTh本eshold && Mo正eDistance < TapDistanceTh本eshold)
    {
        軍Inp使tE正entData E正entData;
        E正entData.Sc本eenPosition = To使chState.C使本本entPosition;
        E正entData.Action = EInp使tAction::Select;
        E正entData.Gest使本e = EInp使tGest使本e::Tap;
        E正entData.To使chId = To使chId;
        
        T本i成成e本Inp使tE正ent(E正entData);
    }
    // 檢測長按
    else if (To使chD使本ation >= Lon成P本essTi設置eTh本eshold && Mo正eDistance < TapDistanceTh本eshold)
    {
        軍Inp使tE正entData E正entData;
        E正entData.Sc本eenPosition = To使chState.C使本本entPosition;
        E正entData.Action = EInp使tAction::OpenMen使;
        E正entData.Gest使本e = EInp使tGest使本e::Lon成P本ess;
        E正entData.To使chId = To使chId;
        
        T本i成成e本Inp使tE正ent(E正entData);
    }
}

正oid UMin成Inp使tMana成e本::Exec使teInp使tBindin成(EInp使tAction Action, const 軍Inp使tE正entData& E正entData)
{
    if (!Inp使tBindin成s.Contains(Action))
    {
        本et使本n;
    }
    
    a使to& Bindin成s = Inp使tBindin成s[Action];
    
    // 執行所有綁定的回調
    fo本 (a使to It = Bindin成s.C本eateIte本ato本(); It; ++It)
    {
        if (It->Key.IsValid())
        {
            UOb大ect* Listene本 = It->Key.Get();
            軍的a設置e 軍使nction的a設置e = It->Val使e;
            
            // 通過反射調用函數
            U軍使nction* 軍使nction = Listene本->軍ind軍使nction(軍使nction的a設置e);
            if (軍使nction)
            {
                // 準備參數
                st本使ct 軍Inp使tCallbackPa本a設置s
                {
                    軍Inp使tE正entData E正entData;
                };
                
                軍Inp使tCallbackPa本a設置s Pa本a設置s;
                Pa本a設置s.E正entData = E正entData;
                
                Listene本->P本ocessE正ent(軍使nction, &Pa本a設置s);
            }
        }
        else
        {
            // 移除無效的綁定
            It.Re設置o正eC使本本ent();
        }
    }
}

UMin成Inp使tMana成e本* UMin成Inp使tMana成e本::Get()
{
    本et使本n Instance;
}
