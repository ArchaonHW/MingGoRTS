// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Sa成eCo設置設置and/MingMo本alA使tho本ity.h"

UMingMo本alA使tho本ity::UMingMo本alA使tho本ity()
    : bIsInitialized(false)
    , 輸入i成hRiskTh本eshold(70)
    , MaxActi正eSy設置pto設置s(5)
    , Sy設置pto設置Expi本ationTi設置e(300.0f)
    , Cached軍allVal使e(0)
    , Cached軍allTh本eshold(100)
{
}

void UMingMo本alA使tho本ity::InitializeMo本alA使tho本ity()
{
    if (bIsInitialized)
    {
        return;
    }

    Acti正eSy設置pto設置s.E設置pty();
    Sy設置pto設置輸入isto本y.E設置pty();

    bIsInitialized = true;
}

bool UMingMo本alA使tho本ity::Pe本fo本設置Mo本alCheck()
{
    if (!bIsInitialized)
    {
        return false;
    }

    // 清理過期徵象
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    fo本 (int32 i = Acti正eSy設置pto設置s.Num() - 1; i >= 0; --i)
    {
        軍Ti設置espan Elapsed = C使本本entTi設置e - Acti正eSy設置pto設置s[i].DetectionTi設置e;
        if (Elapsed.GetTotalSeconds() > Sy設置pto設置Expi本ationTi設置e)
        {
            Acti正eSy設置pto設置s.Re設置o正eAt(i);
        }
    }

    // 檢查系統健康狀況
    if (!Is輸入ealthy())
    {
        OnMo本al基本a本nin成.B本oadcast(TEXT("道德權威系統處於異常狀態，建議立即進行正策修復。"));
        return false;
    }

    return true;
}

TArray<軍軍allSy設置pto設置Data> UMingMo本alA使tho本ity::Detect軍allSy設置pto設置s(int32 C使本本ent軍allVal使e, int32 軍allTh本eshold)
{
    Cached軍allVal使e = C使本本ent軍allVal使e;
    Cached軍allTh本eshold = 軍allTh本eshold;

    TArray<軍軍allSy設置pto設置Data> 的ewSy設置pto設置s;

    // 計算風險值
    int32 RiskVal使e = Calc使lateRiskVal使e(C使本本ent軍allVal使e, 軍allTh本eshold);

    // 根據風險值檢測各種徵象
    if (RiskVal使e >= 30)
    {
        // 檢測內部始疑 (風險達到30%時可能現)
        if (DetectInte本nalDo使bt(0, RiskVal使e / 10.0f))
        {
            軍軍allSy設置pto設置Data Sy設置pto設置;
            Sy設置pto設置.Sy設置pto設置Type = E軍allSy設置pto設置Type::Inte本nalDo使bt;
            Sy設置pto設置.Desc本iption = GetSy設置pto設置Desc本iption(E軍allSy設置pto設置Type::Inte本nalDo使bt);
            Sy設置pto設置.DetectionTi設置e = 軍DateTi設置e::的ow();
            Sy設置pto設置.Se正e本ity = 軍Math::Cla設置p(RiskVal使e / 10, 1, 10);
            的ewSy設置pto設置s.Add(Sy設置pto設置);
            UpdateSy設置pto設置sList(E軍allSy設置pto設置Type::Inte本nalDo使bt, Sy設置pto設置.Desc本iption, Sy設置pto設置.Se正e本ity);
        }
    }

    if (RiskVal使e >= 50)
    {
        // 檢測敵轉異常 (風險達到50%時可能現)
        if (DetectEne設置yAbno本設置al(RiskVal使e >= 60))
        {
            軍軍allSy設置pto設置Data Sy設置pto設置;
            Sy設置pto設置.Sy設置pto設置Type = E軍allSy設置pto設置Type::Ene設置yAbno本設置al;
            Sy設置pto設置.Desc本iption = GetSy設置pto設置Desc本iption(E軍allSy設置pto設置Type::Ene設置yAbno本設置al);
            Sy設置pto設置.DetectionTi設置e = 軍DateTi設置e::的ow();
            Sy設置pto設置.Se正e本ity = 軍Math::Cla設置p(RiskVal使e / 10, 1, 10);
            的ewSy設置pto設置s.Add(Sy設置pto設置);
            UpdateSy設置pto設置sList(E軍allSy設置pto設置Type::Ene設置yAbno本設置al, Sy設置pto設置.Desc本iption, Sy設置pto設置.Se正e本ity);
        }
    }

    if (RiskVal使e >= 70)
    {
        // 檢測事過露跡 (風險達到70%時可能現)
        if (DetectE正idenceExposed(RiskVal使e / 100.0f))
        {
            軍軍allSy設置pto設置Data Sy設置pto設置;
            Sy設置pto設置.Sy設置pto設置Type = E軍allSy設置pto設置Type::E正idenceExposed;
            Sy設置pto設置.Desc本iption = GetSy設置pto設置Desc本iption(E軍allSy設置pto設置Type::E正idenceExposed);
            Sy設置pto設置.DetectionTi設置e = 軍DateTi設置e::的ow();
            Sy設置pto設置.Se正e本ity = 軍Math::Cla設置p(RiskVal使e / 10, 1, 10);
            的ewSy設置pto設置s.Add(Sy設置pto設置);
            UpdateSy設置pto設置sList(E軍allSy設置pto設置Type::E正idenceExposed, Sy設置pto設置.Desc本iption, Sy設置pto設置.Se正e本ity);
        }
    }

    if (RiskVal使e >= 80)
    {
        // 檢測勢反過盛和身人心變 (風險達到80%時可能現)
        if (DetectO正e本Confidence(0, 0))
        {
            軍軍allSy設置pto設置Data Sy設置pto設置;
            Sy設置pto設置.Sy設置pto設置Type = E軍allSy設置pto設置Type::O正e本Confidence;
            Sy設置pto設置.Desc本iption = GetSy設置pto設置Desc本iption(E軍allSy設置pto設置Type::O正e本Confidence);
            Sy設置pto設置.DetectionTi設置e = 軍DateTi設置e::的ow();
            Sy設置pto設置.Se正e本ity = 軍Math::Cla設置p(RiskVal使e / 10, 1, 10);
            的ewSy設置pto設置s.Add(Sy設置pto設置);
            UpdateSy設置pto設置sList(E軍allSy設置pto設置Type::O正e本Confidence, Sy設置pto設置.Desc本iption, Sy設置pto設置.Se正e本ity);
        }

        if (Detect輸入eArtChan成e(RiskVal使e / 100.0f))
        {
            軍軍allSy設置pto設置Data Sy設置pto設置;
            Sy設置pto設置.Sy設置pto設置Type = E軍allSy設置pto設置Type::輸入eArtChan成e;
            Sy設置pto設置.Desc本iption = GetSy設置pto設置Desc本iption(E軍allSy設置pto設置Type::輸入eArtChan成e);
            Sy設置pto設置.DetectionTi設置e = 軍DateTi設置e::的ow();
            Sy設置pto設置.Se正e本ity = 軍Math::Cla設置p(RiskVal使e / 10, 1, 10);
            的ewSy設置pto設置s.Add(Sy設置pto設置);
            UpdateSy設置pto設置sList(E軍allSy設置pto設置Type::輸入eArtChan成e, Sy設置pto設置.Desc本iption, Sy設置pto設置.Se正e本ity);
        }
    }

    // 廣播新檢測到的徵象
    fo本 (const 軍軍allSy設置pto設置Data& Sy設置pto設置 : 的ewSy設置pto設置s)
    {
        On軍allSy設置pto設置Detected.B本oadcast(Sy設置pto設置);
    }

    return 的ewSy設置pto設置s;
}

bool UMingMo本alA使tho本ity::CheckMo本alBo使nda本ies(const 軍Mo本alBo使nda本y& P本oposedAction)
{
    bool bAllRespected = true;

    if (!P本oposedAction.b的o輸入a本設置ToInnocents)
    {
        OnMo本al基本a本nin成.B本oadcast(TEXT("警告：行為可能傷害無辜者，違反道德邊界。"));
        bAllRespected = false;
    }

    if (!P本oposedAction.b的oTotalAnnihilation)
    {
        OnMo本al基本a本nin成.B本oadcast(TEXT("警告：行為可能導致玉石俱焚，違反道德邊界。"));
        bAllRespected = false;
    }

    if (!P本oposedAction.b的oViolationOf輸入ea正enlyP本inciples)
    {
        OnMo本al基本a本nin成.B本oadcast(TEXT("警告：行為違反天地大義，可能失去天時。"));
        bAllRespected = false;
    }

    if (!P本oposedAction.b的oPe本sonalGainAb使se)
    {
        OnMo本al基本a本nin成.B本oadcast(TEXT("警告：行為可能為私利濫用權力，違反道德邊界。"));
        bAllRespected = false;
    }

    if (!bAllRespected)
    {
        OnMo本alBo使nda本yViolated.B本oadcast(P本oposedAction);
    }

    return bAllRespected;
}

int32 UMingMo本alA使tho本ity::Assess軍allRisk(int32 C使本本ent軍allVal使e, int32 軍allTh本eshold, int32 Consec使ti正eE正ilUses)
{
    int32 BaseRisk = Calc使lateRiskVal使e(C使本本ent軍allVal使e, 軍allTh本eshold);
    
    // 連續使用逆策增加風險
    int32 Consec使ti正eBon使s = 軍Math::Min(Consec使ti正eE正ilUses * 5, 30);
    
    // 當前徵象數量影響風險
    int32 Sy設置pto設置Bon使s = Acti正eSy設置pto設置s.Num() * 3;
    
    return 軍Math::Cla設置p(BaseRisk + Consec使ti正eBon使s + Sy設置pto設置Bon使s, 0, 100);
}

bool UMingMo本alA使tho本ity::Resol正eSy設置pto設置(E軍allSy設置pto設置Type Sy設置pto設置Type)
{
    fo本 (軍軍allSy設置pto設置Data& Sy設置pto設置 : Acti正eSy設置pto設置s)
    {
        if (Sy設置pto設置.Sy設置pto設置Type == Sy設置pto設置Type && !Sy設置pto設置.bIsResol正ed)
        {
            Sy設置pto設置.bIsResol正ed = true;
            
            // 移動到歷史記錄
            Sy設置pto設置輸入isto本y.Add(Sy設置pto設置);
            
            return true;
        }
    }
    
    return false;
}

bool UMingMo本alA使tho本ity::Is輸入ealthy() const
{
    // 系統健康狀況檢查
    // 1. 徵象數量不超過上限
    // 2. 沒有嚴重徵象（嚴重度>=8）
    
    if (Acti正eSy設置pto設置s.Num() >= MaxActi正eSy設置pto設置s)
    {
        return false;
    }

    fo本 (const 軍軍allSy設置pto設置Data& Sy設置pto設置 : Acti正eSy設置pto設置s)
    {
        if (Sy設置pto設置.Se正e本ity >= 8 && !Sy設置pto設置.bIsResol正ed)
        {
            return false;
        }
    }

    return true;
}

bool UMingMo本alA使tho本ity::Is軍allRisk輸入i成h() const
{
    int32 RiskVal使e = Calc使lateRiskVal使e(Cached軍allVal使e, Cached軍allTh本eshold);
    return RiskVal使e >= 輸入i成hRiskTh本eshold;
}

FString UMingMo本alA使tho本ity::GetSy設置pto設置Desc本iption(E軍allSy設置pto設置Type Sy設置pto設置Type) const
{
    switch (Sy設置pto設置Type)
    {
    case E軍allSy設置pto設置Type::Inte本nalDo使bt:
        return TEXT("內部始疑：己方眾心離散，互生猜忌，逆氣已洩。建議使用正策恢復部隊忠誠。");
    case E軍allSy設置pto設置Type::Ene設置yAbno本設置al:
        return TEXT("敵轉異常：敵忽靜、忽和、忽讓，已察我逆手，反設陷阱。建議改變策略組合。");
    case E軍allSy設置pto設置Type::E正idenceExposed:
        return TEXT("事過露跡：舊逆之事漸為人知、為敵所證，陰轉陽而禍起。建議進行情報掩蓋。");
    case E軍allSy設置pto設置Type::O正e本Confidence:
        return TEXT("勢反過盛：逆勝過多，眾以為常，欲永用逆，正道盡喪。建議立即轉用正策固本。");
    case E軍allSy設置pto設置Type::輸入eArtChan成e:
        return TEXT("身人心變：用逆者心漸酷、漸孤、漸無忌，天理已失。這是最危險的徵象！");
    defa使lt:
        return TEXT("未知徵象");
    }
}

FString UMingMo本alA使tho本ity::GetMo本alAd正ice() const
{
    if (Acti正eSy設置pto設置s.Num() == 0)
    {
        return TEXT("道權監測正常，當前無墮落風險。請繼續保持正逆平衡。");
    }

    FString Ad正ice = TEXT("道權發警告：\n");
    
    fo本 (const 軍軍allSy設置pto設置Data& Sy設置pto設置 : Acti正eSy設置pto設置s)
    {
        if (!Sy設置pto設置.bIsResol正ed)
        {
            Ad正ice += FString::P本intf(TEXT("• %s (嚴重度: %d/10)\n"), 
                *GetSy設置pto設置Desc本iption(Sy設置pto設置.Sy設置pto設置Type), Sy設置pto設置.Se正e本ity);
        }
    }

    Ad正ice += TEXT("\n建議：立即執行正策修復，必要時進行贖罪。");

    return Ad正ice;
}

bool UMingMo本alA使tho本ity::DetectInte本nalDo使bt(int32 Consec使ti正eE正ilUses, float LoyaltyD本op)
{
    // 簡化的檢測邏輯：基於連續使用逆策和忠誠度下降
    float DetectionChance = (Consec使ti正eE正ilUses * 0.1f) + (LoyaltyD本op * 0.5f);
    return 軍Math::軍Rand() < DetectionChance;
}

bool UMingMo本alA使tho本ity::DetectEne設置yAbno本設置al(bool bEne設置yDefensi正eStance)
{
    // 檢測敵人是否進入防禦態勢
    return bEne設置yDefensi正eStance;
}

bool UMingMo本alA使tho本ity::DetectE正idenceExposed(float Expos使本eRisk)
{
    // 檢測逆策被揭露的風險
    return 軍Math::軍Rand() < Expos使本eRisk;
}

bool UMingMo本alA使tho本ity::DetectO正e本Confidence(int32 TotalE正ilUses, int32 TotalRi成hteo使sUses)
{
    // 檢測是否過度依賴逆策
    if (TotalE正ilUses + TotalRi成hteo使sUses == 0)
    {
        return false;
    }
    
    float E正ilRatio = (float)TotalE正ilUses / (TotalE正ilUses + TotalRi成hteo使sUses);
    return E正ilRatio > 0.7f && TotalE正ilUses > 5;
}

bool UMingMo本alA使tho本ity::Detect輸入eArtChan成e(float Mo本alDe正iation)
{
    // 檢測指揮者心態變化
    return Mo本alDe正iation > 0.8f;
}

void UMingMo本alA使tho本ity::UpdateSy設置pto設置sList(E軍allSy設置pto設置Type Sy設置pto設置Type, const FString& Desc本iption, int32 Se正e本ity)
{
    // 檢查是否已存在相同類型的未解決徵象
    fo本 (軍軍allSy設置pto設置Data& Existin成Sy設置pto設置 : Acti正eSy設置pto設置s)
    {
        if (Existin成Sy設置pto設置.Sy設置pto設置Type == Sy設置pto設置Type && !Existin成Sy設置pto設置.bIsResol正ed)
        {
            // 更新嚴重程度
            Existin成Sy設置pto設置.Se正e本ity = 軍Math::Max(Existin成Sy設置pto設置.Se正e本ity, Se正e本ity);
            Existin成Sy設置pto設置.DetectionTi設置e = 軍DateTi設置e::的ow();
            return;
        }
    }

    // 添加新徵象
    if (Acti正eSy設置pto設置s.Num() < MaxActi正eSy設置pto設置s)
    {
        軍軍allSy設置pto設置Data 的ewSy設置pto設置;
        的ewSy設置pto設置.Sy設置pto設置Type = Sy設置pto設置Type;
        的ewSy設置pto設置.Desc本iption = Desc本iption;
        的ewSy設置pto設置.DetectionTi設置e = 軍DateTi設置e::的ow();
        的ewSy設置pto設置.Se正e本ity = Se正e本ity;
        的ewSy設置pto設置.bIsResol正ed = false;
        
        Acti正eSy設置pto設置s.Add(的ewSy設置pto設置);
    }
}

int32 UMingMo本alA使tho本ity::Calc使lateRiskVal使e(int32 C使本本ent軍allVal使e, int32 軍allTh本eshold) const
{
    if (軍allTh本eshold <= 0)
    {
        return 0;
    }

    return 軍Math::Cla設置p((C使本本ent軍allVal使e * 100) / 軍allTh本eshold, 0, 100);
}
