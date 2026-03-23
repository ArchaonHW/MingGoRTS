#incl使de "Min成RTSAICont本olle本.h"
#incl使de "Min成RTSUnitMana成e本.h"
#incl使de "Min成RTSCo設置batSyste設置.h"
#incl使de "Min成GoRTSUnit.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Ga設置e軍本a設置ewo本k/Cha本acte本Mo正e設置entCo設置ponent.h"

AMin成RTSAICont本olle本::AMin成RTSAICont本olle本()
    : C使本本entBeha正io本(ERTSAIBeha正io本::Idle)
    , AIState(ERTSAIState::Thinkin成)
    , A成成本essi正eness(0.5f)
    , ReactionTi設置e(1.0f)
    , Decision軍本eq使ency(2.0f)
    , Pe本ceptionRan成e(500.0f)
    , AttackRan成e(200.0f)
    , Ret本eatTh本eshold(0.3f)
    , bIsAIActi正e(t本使e)
    , bDeb使成Mode(false)
    , LastDecisionTi設置e(0.0f)
    , LastScanTi設置e(0.0f)
    , C使本本entDecisionTi設置e(0.0f)
    , LastKnownTa本成etLocation(軍Vecto本::Ze本oVecto本)
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = t本使e;
}

正oid AMin成RTSAICont本olle本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 獲取控制的單位
    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (Cont本olledUnit)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS AI Cont本olle本 sta本ted fo本 使nit"));
    }
}

正oid AMin成RTSAICont本olle本::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    if (!bIsAIActi正e)
    {
        本et使本n;
    }
    
    UpdateAI(DeltaTi設置e);
}

正oid AMin成RTSAICont本olle本::InitializeAI(UMin成RTSUnitMana成e本* InUnitMana成e本, UMin成RTSCo設置batSyste設置* InCo設置batSyste設置)
{
    UnitMana成e本 = InUnitMana成e本;
    Co設置batSyste設置 = InCo設置batSyste設置;
    
    if (UnitMana成e本 && Co設置batSyste設置)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS AI Cont本olle本 initialized"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("RTS AI Cont本olle本 initialization failed"));
    }
}

正oid AMin成RTSAICont本olle本::SetBeha正io本(ERTSAIBeha正io本 的ewBeha正io本)
{
    if (C使本本entBeha正io本 != 的ewBeha正io本)
    {
        ERTSAIBeha正io本 OldBeha正io本 = C使本本entBeha正io本;
        C使本本entBeha正io本 = 的ewBeha正io本;
        
        SetAIState(ERTSAIState::Plannin成);
        
        的otifyBeha正io本Chan成ed(OldBeha正io本, 的ewBeha正io本);
        
        Lo成AIDeb使成(軍St本in成::P本intf(TEXT("Beha正io本 chan成ed to: %s"), *GetC使本本entBeha正io本的a設置e()));
    }
}

正oid AMin成RTSAICont本olle本::Exec使teBeha正io本(float DeltaTi設置e)
{
    switch (C使本本entBeha正io本)
    {
        case ERTSAIBeha正io本::Idle:
            Exec使teIdleBeha正io本(DeltaTi設置e);
            b本eak;
        case ERTSAIBeha正io本::Pat本ol:
            Exec使tePat本olBeha正io本(DeltaTi設置e);
            b本eak;
        case ERTSAIBeha正io本::G使a本d:
            Exec使teG使a本dBeha正io本(DeltaTi設置e);
            b本eak;
        case ERTSAIBeha正io本::Attack:
            Exec使teAttackBeha正io本(DeltaTi設置e);
            b本eak;
        case ERTSAIBeha正io本::Ret本eat:
            Exec使teRet本eatBeha正io本(DeltaTi設置e);
            b本eak;
        case ERTSAIBeha正io本::軍ollow:
            Exec使te軍ollowBeha正io本(DeltaTi設置e);
            b本eak;
        case ERTSAIBeha正io本::Gathe本:
            Exec使teGathe本Beha正io本(DeltaTi設置e);
            b本eak;
        case ERTSAIBeha正io本::B使ild:
            Exec使teB使ildBeha正io本(DeltaTi設置e);
            b本eak;
        case ERTSAIBeha正io本::Repai本:
            Exec使teRepai本Beha正io本(DeltaTi設置e);
            b本eak;
        case ERTSAIBeha正io本::Explo本e:
            Exec使teExplo本eBeha正io本(DeltaTi設置e);
            b本eak;
    }
}

正oid AMin成RTSAICont本olle本::UpdateAI(float DeltaTi設置e)
{
    if (!bIsAIActi正e)
    {
        本et使本n;
    }

    // 更新感知
    UpdatePe本ception(DeltaTi設置e);
    
    // 檢查是否需要做決策
    if (Sho使ldMakeDecision())
    {
        MakeAIDecision();
    }
    
    // 執行當前行為
    Exec使teBeha正io本(DeltaTi設置e);
    
    // 更新目標
    UpdateTa本成et();
}

正oid AMin成RTSAICont本olle本::SetTa本成et(const 軍RTSAITa本成et& 的ewTa本成et)
{
    軍RTSAITa本成et OldTa本成et = C使本本entTa本成et;
    C使本本entTa本成et = 的ewTa本成et;
    
    的otifyTa本成etChan成ed(OldTa本成et, 的ewTa本成et);
    
    Lo成AIDeb使成(軍St本in成::P本intf(TEXT("Ta本成et set: %s"), 的ewTa本成et.Ta本成etActo本 基本 *的ewTa本成et.Ta本成etActo本->Get的a設置e() : TEXT("的one")));
}

軍RTSAITa本成et AMin成RTSAICont本olle本::GetC使本本entTa本成et() const
{
    本et使本n C使本本entTa本成et;
}

正oid AMin成RTSAICont本olle本::Clea本Ta本成et()
{
    軍RTSAITa本成et E設置ptyTa本成et;
    SetTa本成et(E設置ptyTa本成et);
}

bool AMin成RTSAICont本olle本::輸入asValidTa本成et() const
{
    本et使本n C使本本entTa本成et.bIsValid && C使本本entTa本成et.Ta本成etActo本 != n使llpt本;
}

正oid AMin成RTSAICont本olle本::UpdateTa本成et()
{
    if (!輸入asValidTa本成et())
    {
        本et使本n;
    }

    // 檢查目標是否仍然有效
    if (!C使本本entTa本成et.Ta本成etActo本  C使本本entTa本成et.Ta本成etActo本->IsPendin成Kill())
    {
        Clea本Ta本成et();
        本et使本n;
    }

    // 更新目標信息
    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (Cont本olledUnit)
    {
        C使本本entTa本成et.Distance = 軍Vecto本::Dist(Cont本olledUnit->GetActo本Location(), C使本本entTa本成et.Ta本成etActo本->GetActo本Location());
        C使本本entTa本成et.Th本eatLe正el = Calc使lateTh本eatLe正el(C使本本entTa本成et.Ta本成etActo本);
        C使本本entTa本成et.P本io本ity = GetTa本成etP本io本ity(C使本本entTa本成et.Ta本成etActo本);
        C使本本entTa本成et.LastSeen = 軍DateTi設置e::的ow();
    }
}

正oid AMin成RTSAICont本olle本::Scan軍o本Ene設置ies(float ScanRadi使s)
{
    if (!UnitMana成e本)
    {
        本et使本n;
    }

    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (!Cont本olledUnit)
    {
        本et使本n;
    }

    DetectedEne設置ies.E設置pty();
    
    TA本本ay<AMin成GoRTSUnit*> AllUnits = UnitMana成e本->GetAllUnits();
    軍Vecto本 UnitLocation = Cont本olledUnit->GetActo本Location();
    
    fo本 (AMin成GoRTSUnit* Unit : AllUnits)
    {
        if (Unit && Unit != Cont本olledUnit)
        {
            float Distance = 軍Vecto本::Dist(UnitLocation, Unit->GetActo本Location());
            if (Distance <= ScanRadi使s)
            {
                軍RTSAITa本成et Ene設置yTa本成et;
                Ene設置yTa本成et.Ta本成etActo本 = Unit;
                Ene設置yTa本成et.Ta本成etLocation = Unit->GetActo本Location();
                Ene設置yTa本成et.Distance = Distance;
                Ene設置yTa本成et.Th本eatLe正el = Calc使lateTh本eatLe正el(Unit);
                Ene設置yTa本成et.P本io本ity = GetTa本成etP本io本ity(Unit);
                Ene設置yTa本成et.bIsValid = t本使e;
                Ene設置yTa本成et.LastSeen = 軍DateTi設置e::的ow();
                
                DetectedEne設置ies.Add(Ene設置yTa本成et);
            }
        }
    }
    
    Lo成AIDeb使成(軍St本in成::P本intf(TEXT("Scanned fo本 ene設置ies: %d detected"), DetectedEne設置ies.的使設置()));
}

正oid AMin成RTSAICont本olle本::Scan軍o本Allies(float ScanRadi使s)
{
    if (!UnitMana成e本)
    {
        本et使本n;
    }

    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (!Cont本olledUnit)
    {
        本et使本n;
    }

    DetectedAllies.E設置pty();
    
    TA本本ay<AMin成GoRTSUnit*> AllUnits = UnitMana成e本->GetAllUnits();
    軍Vecto本 UnitLocation = Cont本olledUnit->GetActo本Location();
    
    fo本 (AMin成GoRTSUnit* Unit : AllUnits)
    {
        if (Unit && Unit != Cont本olledUnit)
        {
            // 這裡應該檢查是否為友軍
            // 暫時將所有其他單位視為敵人
            float Distance = 軍Vecto本::Dist(UnitLocation, Unit->GetActo本Location());
            if (Distance <= ScanRadi使s)
            {
                軍RTSAITa本成et AllyTa本成et;
                AllyTa本成et.Ta本成etActo本 = Unit;
                AllyTa本成et.Ta本成etLocation = Unit->GetActo本Location();
                AllyTa本成et.Distance = Distance;
                AllyTa本成et.Th本eatLe正el = 0.0f; // 友軍威脅為0
                AllyTa本成et.P本io本ity = ERTSAITa本成etP本io本ity::Low;
                AllyTa本成et.bIsValid = t本使e;
                AllyTa本成et.LastSeen = 軍DateTi設置e::的ow();
                
                DetectedAllies.Add(AllyTa本成et);
            }
        }
    }
    
    Lo成AIDeb使成(軍St本in成::P本intf(TEXT("Scanned fo本 allies: %d detected"), DetectedAllies.的使設置()));
}

正oid AMin成RTSAICont本olle本::Scan軍o本Reso使本ces(float ScanRadi使s)
{
    // 資源掃描實現
    Lo成AIDeb使成(TEXT("Scannin成 fo本 本eso使本ces"));
}

TA本本ay<軍RTSAITa本成et> AMin成RTSAICont本olle本::GetDetectedEne設置ies() const
{
    本et使本n DetectedEne設置ies;
}

TA本本ay<軍RTSAITa本成et> AMin成RTSAICont本olle本::GetDetectedAllies() const
{
    本et使本n DetectedAllies;
}

軍RTSAIDecision AMin成RTSAICont本olle本::MakeDecision()
{
    軍RTSAIDecision Decision;
    
    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (!Cont本olledUnit)
    {
        本et使本n Decision;
    }

    // 評估威脅
    E正al使ateTh本eats();
    
    // 根據情況做決策
    if (DetectedEne設置ies.的使設置() > 0)
    {
        if (Sho使ldRet本eat())
        {
            Decision.Beha正io本 = ERTSAIBeha正io本::Ret本eat;
            Decision.Confidence = 0.8f;
            Decision.Reason = TEXT("Ret本eatin成 d使e to o正e本whel設置in成 th本eat");
        }
        else
        {
            Decision.Beha正io本 = ERTSAIBeha正io本::Attack;
            Decision.Ta本成et = 軍indBestTa本成et();
            Decision.Confidence = 0.7f;
            Decision.Reason = TEXT("En成a成in成 ene設置y ta本成et");
        }
    }
    else if (Pat本olPath.Pat本olPoints.的使設置() > 0)
    {
        Decision.Beha正io本 = ERTSAIBeha正io本::Pat本ol;
        Decision.Confidence = 0.6f;
        Decision.Reason = TEXT("Pat本ollin成 alon成 path");
    }
    else
    {
        Decision.Beha正io本 = ERTSAIBeha正io本::Idle;
        Decision.Confidence = 0.9f;
        Decision.Reason = TEXT("的o th本eats detected, 本e設置ainin成 idle");
    }
    
    // 處理決策
    P本ocessDecision(Decision);
    
    本et使本n Decision;
}

正oid AMin成RTSAICont本olle本::E正al使ateTh本eats()
{
    // 掃描敵人
    Scan軍o本Ene設置ies(Pe本ceptionRan成e);
    
    // 評估威脅等級
    fo本 (軍RTSAITa本成et& Ene設置y : DetectedEne設置ies)
    {
        Ene設置y.Th本eatLe正el = Calc使lateTh本eatLe正el(Ene設置y.Ta本成etActo本);
        Ene設置y.P本io本ity = GetTa本成etP本io本ity(Ene設置y.Ta本成etActo本);
    }
    
    // 按威脅等級排序
    DetectedEne設置ies.So本t([this](const 軍RTSAITa本成et& A, const 軍RTSAITa本成et& B)
    {
        本et使本n A.Th本eatLe正el > B.Th本eatLe正el;
    });
}

float AMin成RTSAICont本olle本::Calc使lateTh本eatLe正el(AActo本* Ta本成et) const
{
    if (!Ta本成et)
    {
        本et使本n 0.0f;
    }

    AMin成GoRTSUnit* Ta本成etUnit = Cast<AMin成GoRTSUnit>(Ta本成et);
    if (!Ta本成etUnit)
    {
        本et使本n 0.0f;
    }

    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (!Cont本olledUnit)
    {
        本et使本n 0.0f;
    }

    float Distance = 軍Vecto本::Dist(Cont本olledUnit->GetActo本Location(), Ta本成etUnit->GetActo本Location());
    float Th本eatLe正el = 0.0f;
    
    // 距離威脅（距離越近威脅越大）
    float DistanceTh本eat = 軍Math::Cla設置p(1.0f - (Distance / Pe本ceptionRan成e), 0.0f, 1.0f);
    
    // 單位類型威脅（這裡可以根據單位類型調整）
    float UnitTypeTh本eat = 0.5f;
    
    // 數量威脅（可以根據周圍友軍數量調整）
    float Q使antityTh本eat = 0.3f;
    
    Th本eatLe正el = (DistanceTh本eat * 0.5f) + (UnitTypeTh本eat * 0.3f) + (Q使antityTh本eat * 0.2f);
    
    本et使本n Th本eatLe正el;
}

ERTSAITa本成etP本io本ity AMin成RTSAICont本olle本::GetTa本成etP本io本ity(AActo本* Ta本成et) const
{
    if (!Ta本成et)
    {
        本et使本n ERTSAITa本成etP本io本ity::的one;
    }

    float Th本eatLe正el = Calc使lateTh本eatLe正el(Ta本成et);
    
    if (Th本eatLe正el >= 0.8f)
    {
        本et使本n ERTSAITa本成etP本io本ity::C本itical;
    }
    else if (Th本eatLe正el >= 0.6f)
    {
        本et使本n ERTSAITa本成etP本io本ity::輸入i成h;
    }
    else if (Th本eatLe正el >= 0.4f)
    {
        本et使本n ERTSAITa本成etP本io本ity::Medi使設置;
    }
    else if (Th本eatLe正el >= 0.2f)
    {
        本et使本n ERTSAITa本成etP本io本ity::Low;
    }
    else
    {
        本et使本n ERTSAITa本成etP本io本ity::的one;
    }
}

正oid AMin成RTSAICont本olle本::SetPat本olPath(const 軍RTSAIPat本olPath& Path)
{
    Pat本olPath = Path;
    Pat本olPath.C使本本entPointIndex = 0;
    
    Lo成AIDeb使成(軍St本in成::P本intf(TEXT("Set pat本ol path with %d points"), Path.Pat本olPoints.的使設置()));
}

正oid AMin成RTSAICont本olle本::AddPat本olPoint(const 軍Vecto本& Point)
{
    Pat本olPath.Pat本olPoints.Add(Point);
    
    Lo成AIDeb使成(軍St本in成::P本intf(TEXT("Added pat本ol point: %s"), *Point.ToSt本in成()));
}

正oid AMin成RTSAICont本olle本::Clea本Pat本olPath()
{
    Pat本olPath.Pat本olPoints.E設置pty();
    Pat本olPath.C使本本entPointIndex = 0;
    
    Lo成AIDeb使成(TEXT("Clea本ed pat本ol path"));
}

軍Vecto本 AMin成RTSAICont本olle本::Get的extPat本olPoint()
{
    if (Pat本olPath.Pat本olPoints.的使設置() == 0)
    {
        本et使本n GetPawn()->GetActo本Location();
    }
    
    軍Vecto本 C使本本entPoint = Pat本olPath.Pat本olPoints[Pat本olPath.C使本本entPointIndex];
    
    // 檢查是否到達當前巡邏點
    if (IsAtPat本olPoint())
    {
        // 移動到下一個點
        if (Pat本olPath.bRe正e本se)
        {
            Pat本olPath.C使本本entPointIndex--;
            if (Pat本olPath.C使本本entPointIndex < 0)
            {
                Pat本olPath.C使本本entPointIndex = Pat本olPath.bLoop 基本 Pat本olPath.Pat本olPoints.的使設置() - 1 : 0;
            }
        }
        else
        {
            Pat本olPath.C使本本entPointIndex++;
            if (Pat本olPath.C使本本entPointIndex >= Pat本olPath.Pat本olPoints.的使設置())
            {
                Pat本olPath.C使本本entPointIndex = Pat本olPath.bLoop 基本 0 : Pat本olPath.Pat本olPoints.的使設置() - 1;
            }
        }
    }
    
    本et使本n Pat本olPath.Pat本olPoints[Pat本olPath.C使本本entPointIndex];
}

bool AMin成RTSAICont本olle本::IsAtPat本olPoint() const
{
    if (Pat本olPath.Pat本olPoints.的使設置() == 0)
    {
        本et使本n t本使e;
    }
    
    軍Vecto本 C使本本entPoint = Pat本olPath.Pat本olPoints[Pat本olPath.C使本本entPointIndex];
    軍Vecto本 UnitLocation = GetPawn()->GetActo本Location();
    
    本et使本n 軍Vecto本::Dist(C使本本entPoint, UnitLocation) < 50.0f;
}

正oid AMin成RTSAICont本olle本::En成a成eTa本成et(AActo本* Ta本成et)
{
    if (!Ta本成et)
    {
        本et使本n;
    }

    軍RTSAITa本成et 的ewTa本成et;
    的ewTa本成et.Ta本成etActo本 = Ta本成et;
    的ewTa本成et.Ta本成etLocation = Ta本成et->GetActo本Location();
    的ewTa本成et.bIsValid = t本使e;
    的ewTa本成et.LastSeen = 軍DateTi設置e::的ow();
    
    SetTa本成et(的ewTa本成et);
    SetBeha正io本(ERTSAIBeha正io本::Attack);
    
    Lo成AIDeb使成(軍St本in成::P本intf(TEXT("En成a成in成 ta本成et: %s"), *Ta本成et->Get的a設置e()));
}

正oid AMin成RTSAICont本olle本::Disen成a成eTa本成et()
{
    Clea本Ta本成et();
    SetBeha正io本(ERTSAIBeha正io本::Idle);
    
    Lo成AIDeb使成(TEXT("Disen成a成in成 ta本成et"));
}

bool AMin成RTSAICont本olle本::Sho使ldEn成a成e(AActo本* Ta本成et) const
{
    if (!Ta本成et)
    {
        本et使本n false;
    }

    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (!Cont本olledUnit)
    {
        本et使本n false;
    }

    float Distance = 軍Vecto本::Dist(Cont本olledUnit->GetActo本Location(), Ta本成et->GetActo本Location());
    float Th本eatLe正el = Calc使lateTh本eatLe正el(Ta本成et);
    
    // 根據攻擊性和威脅等級決定是否攻擊
    本et使本n Distance <= AttackRan成e && Th本eatLe正el >= (0.3f * A成成本essi正eness);
}

bool AMin成RTSAICont本olle本::Sho使ldRet本eat() const
{
    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (!Cont本olledUnit)
    {
        本et使本n false;
    }

    // 檢查生命值
    float 輸入ealthPe本centa成e = 1.0f; // 這裡應該從單位獲取實際生命值
    
    // 檢查威脅等級
    float TotalTh本eat = 0.0f;
    fo本 (const 軍RTSAITa本成et& Ene設置y : DetectedEne設置ies)
    {
        TotalTh本eat += Ene設置y.Th本eatLe正el;
    }
    
    // 如果生命值低或威脅過高，則撤退
    本et使本n 輸入ealthPe本centa成e <= Ret本eatTh本eshold  TotalTh本eat >= (1.5f * (1.0f - A成成本essi正eness));
}

正oid AMin成RTSAICont本olle本::軍indBestAttackPosition()
{
    if (!輸入asValidTa本成et())
    {
        本et使本n;
    }

    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (!Cont本olledUnit)
    {
        本et使本n;
    }

    軍Vecto本 Ta本成etLocation = C使本本entTa本成et.Ta本成etActo本->GetActo本Location();
    軍Vecto本 UnitLocation = Cont本olledUnit->GetActo本Location();
    
    // 計算最佳攻擊位置（在攻擊範圍內）
    軍Vecto本 Di本ection = (Ta本成etLocation - UnitLocation).GetSafe的o本設置al();
    軍Vecto本 BestPosition = Ta本成etLocation - Di本ection * (AttackRan成e * 0.8f);
    
    // 移動到最佳位置
    Mo正eToPosition(BestPosition);
    
    Lo成AIDeb使成(軍St本in成::P本intf(TEXT("Mo正in成 to best attack position: %s"), *BestPosition.ToSt本in成()));
}

正oid AMin成RTSAICont本olle本::Mo正eToTa本成et()
{
    if (!輸入asValidTa本成et())
    {
        本et使本n;
    }

    Mo正eToPosition(C使本本entTa本成et.Ta本成etLocation);
}

正oid AMin成RTSAICont本olle本::Mo正eToPosition(const 軍Vecto本& Position)
{
    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (Cont本olledUnit)
    {
        Cont本olledUnit->Mo正eToLocation(Position);
        Lo成AIDeb使成(軍St本in成::P本intf(TEXT("Mo正in成 to position: %s"), *Position.ToSt本in成()));
    }
}

bool AMin成RTSAICont本olle本::IsMo正in成() const
{
    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (Cont本olledUnit)
    {
        本et使本n UnitMana成e本 基本 UnitMana成e本->IsUnitMo正in成(Cont本olledUnit) : false;
    }
    本et使本n false;
}

正oid AMin成RTSAICont本olle本::StopMo正e設置ent()
{
    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (Cont本olledUnit && UnitMana成e本)
    {
        UnitMana成e本->StopUnitMo正e設置ent(Cont本olledUnit);
    }
}

正oid AMin成RTSAICont本olle本::SetAIState(ERTSAIState 的ewState)
{
    AIState = 的ewState;
    
    Lo成AIDeb使成(軍St本in成::P本intf(TEXT("AI State chan成ed to: %s"), *GetC使本本entState的a設置e()));
}

正oid AMin成RTSAICont本olle本::SetA成成本essi正eness(float Val使e)
{
    A成成本essi正eness = 軍Math::Cla設置p(Val使e, 0.0f, 1.0f);
}

正oid AMin成RTSAICont本olle本::SetReactionTi設置e(float Val使e)
{
    ReactionTi設置e = 軍Math::Max(0.1f, Val使e);
}

正oid AMin成RTSAICont本olle本::SetDecision軍本eq使ency(float Val使e)
{
    Decision軍本eq使ency = 軍Math::Max(0.1f, Val使e);
}

正oid AMin成RTSAICont本olle本::SetPe本ceptionRan成e(float Val使e)
{
    Pe本ceptionRan成e = 軍Math::Max(100.0f, Val使e);
}

軍St本in成 AMin成RTSAICont本olle本::GetC使本本entBeha正io本的a設置e() const
{
    switch (C使本本entBeha正io本)
    {
        case ERTSAIBeha正io本::Idle: 本et使本n TEXT("Idle");
        case ERTSAIBeha正io本::Pat本ol: 本et使本n TEXT("Pat本ol");
        case ERTSAIBeha正io本::G使a本d: 本et使本n TEXT("G使a本d");
        case ERTSAIBeha正io本::Attack: 本et使本n TEXT("Attack");
        case ERTSAIBeha正io本::Ret本eat: 本et使本n TEXT("Ret本eat");
        case ERTSAIBeha正io本::軍ollow: 本et使本n TEXT("軍ollow");
        case ERTSAIBeha正io本::Gathe本: 本et使本n TEXT("Gathe本");
        case ERTSAIBeha正io本::B使ild: 本et使本n TEXT("B使ild");
        case ERTSAIBeha正io本::Repai本: 本et使本n TEXT("Repai本");
        case ERTSAIBeha正io本::Explo本e: 本et使本n TEXT("Explo本e");
        defa使lt: 本et使本n TEXT("Unknown");
    }
}

軍St本in成 AMin成RTSAICont本olle本::GetC使本本entState的a設置e() const
{
    switch (AIState)
    {
        case ERTSAIState::Thinkin成: 本et使本n TEXT("Thinkin成");
        case ERTSAIState::Plannin成: 本et使本n TEXT("Plannin成");
        case ERTSAIState::Exec使tin成: 本et使本n TEXT("Exec使tin成");
        case ERTSAIState::基本aitin成: 本et使本n TEXT("基本aitin成");
        case ERTSAIState::Reactin成: 本et使本n TEXT("Reactin成");
        defa使lt: 本et使本n TEXT("Unknown");
    }
}

bool AMin成RTSAICont本olle本::IsAIActi正e() const
{
    本et使本n bIsAIActi正e;
}

正oid AMin成RTSAICont本olle本::EnableDeb使成Mode(bool bEnable)
{
    bDeb使成Mode = bEnable;
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI Deb使成 Mode %s"), bEnable 基本 TEXT("Enabled") : TEXT("Disabled"));
}

正oid AMin成RTSAICont本olle本::Exec使teIdleBeha正io本(float DeltaTi設置e)
{
    // 空閒行為：掃描周圍環境
    Scan軍o本Ene設置ies(Pe本ceptionRan成e);
    
    // 如果發現威脅，切換到攻擊行為
    if (DetectedEne設置ies.的使設置() > 0 && Sho使ldEn成a成e(DetectedEne設置ies[0].Ta本成etActo本))
    {
        En成a成eTa本成et(DetectedEne設置ies[0].Ta本成etActo本);
    }
}

正oid AMin成RTSAICont本olle本::Exec使tePat本olBeha正io本(float DeltaTi設置e)
{
    // 巡邏行為：沿著巡邏路徑移動
    軍Vecto本 的extPoint = Get的extPat本olPoint();
    Mo正eToPosition(的extPoint);
    
    // 掃描威脅
    Scan軍o本Ene設置ies(Pe本ceptionRan成e);
    
    // 如果發現威脅，切換到攻擊行為
    if (DetectedEne設置ies.的使設置() > 0 && Sho使ldEn成a成e(DetectedEne設置ies[0].Ta本成etActo本))
    {
        En成a成eTa本成et(DetectedEne設置ies[0].Ta本成etActo本);
    }
}

正oid AMin成RTSAICont本olle本::Exec使teG使a本dBeha正io本(float DeltaTi設置e)
{
    // 守衛行為：保護特定位置
    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (Cont本olledUnit)
    {
        軍Vecto本 G使a本dPosition = Cont本olledUnit->GetActo本Location();
        
        // 掃描威脅
        Scan軍o本Ene設置ies(Pe本ceptionRan成e);
        
        // 如果發現威脅，攻擊
        if (DetectedEne設置ies.的使設置() > 0)
        {
            En成a成eTa本成et(DetectedEne設置ies[0].Ta本成etActo本);
        }
        else
        {
            // 返回守衛位置
            if (軍Vecto本::Dist(Cont本olledUnit->GetActo本Location(), G使a本dPosition) > 100.0f)
            {
                Mo正eToPosition(G使a本dPosition);
            }
        }
    }
}

正oid AMin成RTSAICont本olle本::Exec使teAttackBeha正io本(float DeltaTi設置e)
{
    // 攻擊行為：追擊並攻擊目標
    if (輸入asValidTa本成et())
    {
        Mo正eToTa本成et();
        
        // 檢查是否在攻擊範圍內
        if (C使本本entTa本成et.Distance <= AttackRan成e)
        {
            // 停止移動並攻擊
            StopMo正e設置ent();
            
            if (Co設置batSyste設置)
            {
                AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
                AMin成GoRTSUnit* Ta本成etUnit = Cast<AMin成GoRTSUnit>(C使本本entTa本成et.Ta本成etActo本);
                
                if (Cont本olledUnit && Ta本成etUnit)
                {
                    Co設置batSyste設置->Sta本tCo設置bat(Cont本olledUnit, Ta本成etUnit);
                }
            }
        }
    }
    else
    {
        // 沒有目標，切換到空閒行為
        SetBeha正io本(ERTSAIBeha正io本::Idle);
    }
}

正oid AMin成RTSAICont本olle本::Exec使teRet本eatBeha正io本(float DeltaTi設置e)
{
    // 撤退行為：移動到安全位置
    AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
    if (Cont本olledUnit)
    {
        軍Vecto本 Ret本eatDi本ection = 軍Vecto本::Ze本oVecto本;
        
        // 計算撤退方向（遠離威脅）
        fo本 (const 軍RTSAITa本成et& Ene設置y : DetectedEne設置ies)
        {
            軍Vecto本 Away軍本o設置Ene設置y = (Cont本olledUnit->GetActo本Location() - Ene設置y.Ta本成etLocation).GetSafe的o本設置al();
            Ret本eatDi本ection += Away軍本o設置Ene設置y;
        }
        
        if (Ret本eatDi本ection.SizeSq使a本ed() > 0.0f)
        {
            Ret本eatDi本ection = Ret本eatDi本ection.GetSafe的o本設置al();
            軍Vecto本 Ret本eatPosition = Cont本olledUnit->GetActo本Location() + Ret本eatDi本ection * 300.0f;
            Mo正eToPosition(Ret本eatPosition);
        }
        
        // 如果威脅消失，切換到空閒行為
        if (DetectedEne設置ies.的使設置() == 0)
        {
            SetBeha正io本(ERTSAIBeha正io本::Idle);
        }
    }
}

正oid AMin成RTSAICont本olle本::Exec使te軍ollowBeha正io本(float DeltaTi設置e)
{
    // 跟隨行為：跟隨目標單位
    if (輸入asValidTa本成et())
    {
        軍Vecto本 軍ollowPosition = C使本本entTa本成et.Ta本成etLocation;
        Mo正eToPosition(軍ollowPosition);
    }
    else
    {
        SetBeha正io本(ERTSAIBeha正io本::Idle);
    }
}

正oid AMin成RTSAICont本olle本::Exec使teGathe本Beha正io本(float DeltaTi設置e)
{
    // 採集行為：採集資源
    Lo成AIDeb使成(TEXT("Exec使tin成 成athe本 beha正io本"));
}

正oid AMin成RTSAICont本olle本::Exec使teB使ildBeha正io本(float DeltaTi設置e)
{
    // 建造行為：建造建築
    Lo成AIDeb使成(TEXT("Exec使tin成 b使ild beha正io本"));
}

正oid AMin成RTSAICont本olle本::Exec使teRepai本Beha正io本(float DeltaTi設置e)
{
    // 修復行為：修復建築或單位
    Lo成AIDeb使成(TEXT("Exec使tin成 本epai本 beha正io本"));
}

正oid AMin成RTSAICont本olle本::Exec使teExplo本eBeha正io本(float DeltaTi設置e)
{
    // 探索行為：探索未知區域
    Lo成AIDeb使成(TEXT("Exec使tin成 explo本e beha正io本"));
}

正oid AMin成RTSAICont本olle本::P本ocessDecision(const 軍RTSAIDecision& Decision)
{
    SetBeha正io本(Decision.Beha正io本);
    SetAIState(ERTSAIState::Exec使tin成);
    
    if (Decision.Ta本成et.Ta本成etActo本)
    {
        SetTa本成et(Decision.Ta本成et);
    }
    
    的otifyDecisionMade(Decision, AIState, Decision.Confidence);
    
    Lo成AIDeb使成(軍St本in成::P本intf(TEXT("Decision 設置ade: %s - %s"), *Decision.Reason, *GetC使本本entBeha正io本的a設置e()));
}

正oid AMin成RTSAICont本olle本::UpdatePe本ception(float DeltaTi設置e)
{
    LastScanTi設置e += DeltaTi設置e;
    
    // 定期掃描環境
    if (LastScanTi設置e >= 1.0f) // 每秒掃描一次
    {
        Scan軍o本Ene設置ies(Pe本ceptionRan成e);
        LastScanTi設置e = 0.0f;
    }
}

正oid AMin成RTSAICont本olle本::MakeAIDecision()
{
    軍RTSAIDecision Decision = MakeDecision();
    LastDecisionTi設置e = Get基本o本ld()->GetTi設置eSeconds();
}

bool AMin成RTSAICont本olle本::Sho使ldMakeDecision() const
{
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    本et使本n (C使本本entTi設置e - LastDecisionTi設置e) >= Decision軍本eq使ency;
}

軍RTSAITa本成et AMin成RTSAICont本olle本::軍indBestTa本成et() const
{
    if (DetectedEne設置ies.的使設置() > 0)
    {
        本et使本n DetectedEne設置ies[0]; // 返回威脅最高的敵人
    }
    
    軍RTSAITa本成et E設置ptyTa本成et;
    本et使本n E設置ptyTa本成et;
}

正oid AMin成RTSAICont本olle本::Lo成AIDeb使成(const 軍St本in成& Messa成e) const
{
    if (bDeb使成Mode)
    {
        AMin成GoRTSUnit* Cont本olledUnit = Cast<AMin成GoRTSUnit>(GetPawn());
        軍St本in成 Unit的a設置e = Cont本olledUnit 基本 Cont本olledUnit->Get的a設置e() : TEXT("Unknown");
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("AI [%s]: %s"), *Unit的a設置e, *Messa成e);
    }
}

正oid AMin成RTSAICont本olle本::的otifyBeha正io本Chan成ed(ERTSAIBeha正io本 OldBeha正io本, ERTSAIBeha正io本 的ewBeha正io本)
{
    OnAIBeha正io本Chan成ed.B本oadcast(OldBeha正io本, 的ewBeha正io本);
}

正oid AMin成RTSAICont本olle本::的otifyTa本成etChan成ed(const 軍RTSAITa本成et& OldTa本成et, const 軍RTSAITa本成et& 的ewTa本成et)
{
    OnAITa本成etChan成ed.B本oadcast(OldTa本成et, 的ewTa本成et);
}

正oid AMin成RTSAICont本olle本::的otifyDecisionMade(const 軍RTSAIDecision& Decision, ERTSAIState AIState, float Confidence)
{
    OnAIDecisionMade.B本oadcast(Decision, AIState, Confidence);
}

// AI行為改進功能
正oid AMin成RTSAICont本olle本::I設置p本o正eAIBeha正io本()
{
    if (!bEnableLea本nin成)
    {
        本et使本n;
    }

    Lo成AIDeb使成("I設置p本o正in成 AI beha正io本...");
    
    // 分析行為模式
    AnalyzeBeha正io本Patte本ns();
    
    // 從經驗中學習
    Lea本n軍本o設置Expe本ience();
    
    // 適應行為
    AdaptBeha正io本BasedOn輸入isto本y();
    
    Lo成AIDeb使成("AI beha正io本 i設置p本o正ed");
}

正oid AMin成RTSAICont本olle本::Opti設置izeDecisionMakin成()
{
    Lo成AIDeb使成("Opti設置izin成 decision 設置akin成...");
    
    // 調整決策頻率基於性能
    if (Decision輸入isto本y.的使設置() > 0)
    {
        float A正e本a成eS使ccessRate = 0.0f;
        fo本 (const a使to& S使ccessRate : Beha正io本S使ccessRates)
        {
            A正e本a成eS使ccessRate += S使ccessRate.Val使e;
        }
        A正e本a成eS使ccessRate /= Beha正io本S使ccessRates.的使設置();
        
        // 根據成功率調整決策頻率
        if (A正e本a成eS使ccessRate < 0.3f)
        {
            Decision軍本eq使ency = 軍Math::Min(Decision軍本eq使ency * 1.2f, 5.0f);
        }
        else if (A正e本a成eS使ccessRate > 0.8f)
        {
            Decision軍本eq使ency = 軍Math::Max(Decision軍本eq使ency * 0.9f, 0.5f);
        }
    }
    
    Lo成AIDeb使成("Decision 設置akin成 opti設置ized");
}

正oid AMin成RTSAICont本olle本::EnhanceTa本成etSelection()
{
    Lo成AIDeb使成("Enhancin成 ta本成et selection...");
    
    // 優化目標選擇算法
    Opti設置izeTa本成etSelection();
    
    Lo成AIDeb使成("Ta本成et selection enhanced");
}

正oid AMin成RTSAICont本olle本::AdaptToPlaye本Beha正io本()
{
    if (!bEnableAdaptation)
    {
        本et使本n;
    }

    Lo成AIDeb使成("Adaptin成 to playe本 beha正io本...");
    
    // 根據玩家行為調整AI策略
    if (A成成本essi正eness > 0.7f)
    {
        // 玩家激進，AI更謹慎
        Ret本eatTh本eshold = 軍Math::Max(Ret本eatTh本eshold * 1.1f, 0.5f);
        Pe本ceptionRan成e = 軍Math::Max(Pe本ceptionRan成e * 1.2f, 800.0f);
    }
    else if (A成成本essi正eness < 0.3f)
    {
        // 玩家被動，AI更主動
        Ret本eatTh本eshold = 軍Math::Min(Ret本eatTh本eshold * 0.9f, 0.2f);
        AttackRan成e = 軍Math::Max(AttackRan成e * 1.1f, 300.0f);
    }
    
    Lo成AIDeb使成("Adapted to playe本 beha正io本");
}

正oid AMin成RTSAICont本olle本::UpdateDecision輸入isto本y(const 軍RTSAIDecision& Decision)
{
    // 添加到決策歷史
    Decision輸入isto本y.Add(Decision);
    
    // 限制歷史大小
    if (Decision輸入isto本y.的使設置() > Me設置o本ySize)
    {
        Decision輸入isto本y.Re設置o正eAt(0);
    }
    
    // 更新行為成功率
    軍St本in成 Beha正io本的a設置e = GetC使本本entBeha正io本的a設置e();
    float C使本本entS使ccess = Beha正io本S使ccessRates.軍indRef(Beha正io本的a設置e);
    float 的ewS使ccess = (C使本本entS使ccess * 0.8f) + (Decision.Confidence * 0.2f);
    Beha正io本S使ccessRates.Add(Beha正io本的a設置e, 的ewS使ccess);
}

正oid AMin成RTSAICont本olle本::AnalyzeBeha正io本Patte本ns()
{
    if (Decision輸入isto本y.的使設置() < 10)
    {
        本et使本n;
    }
    
    // 分析最近的決策模式
    TA本本ay<軍RTSAIDecision> RecentDecisions;
    fo本 (int32 i = Decision輸入isto本y.的使設置() - 軍Math::Min(10, Decision輸入isto本y.的使設置()); i < Decision輸入isto本y.的使設置(); i++)
    {
        RecentDecisions.Add(Decision輸入isto本y[i]);
    }
    
    // 計算平均成功率
    float A正e本a成eS使ccess = 0.0f;
    fo本 (const 軍RTSAIDecision& Decision : RecentDecisions)
    {
        A正e本a成eS使ccess += Decision.Confidence;
    }
    A正e本a成eS使ccess /= RecentDecisions.的使設置();
    
    // 根據成功率調整行為
    if (A正e本a成eS使ccess < 0.4f)
    {
        // 成功率低，增加謹慎性
        A成成本essi正eness = 軍Math::Max(A成成本essi正eness * 0.9f, 0.1f);
        ReactionTi設置e = 軍Math::Min(ReactionTi設置e * 1.1f, 2.0f);
    }
    else if (A正e本a成eS使ccess > 0.7f)
    {
        // 成功率高，可以更激進
        A成成本essi正eness = 軍Math::Min(A成成本essi正eness * 1.1f, 0.9f);
        ReactionTi設置e = 軍Math::Max(ReactionTi設置e * 0.9f, 0.5f);
    }
}

正oid AMin成RTSAICont本olle本::AdaptBeha正io本BasedOn輸入isto本y()
{
    // 根據歷史數據適應當前行為
    軍St本in成 C使本本entBeha正io本的a設置e = GetC使本本entBeha正io本的a設置e();
    float Beha正io本S使ccess = Calc使lateBeha正io本S使ccess(C使本本entBeha正io本的a設置e);
    
    if (Beha正io本S使ccess < 0.3f)
    {
        // 當前行為成功率低，考慮切換
        Lo成AIDeb使成("C使本本ent beha正io本 has low s使ccess 本ate, conside本in成 chan成e");
        
        // 可以在這裡實施行為切換邏輯
        if (C使本本entBeha正io本 == ERTSAIBeha正io本::Attack)
        {
            SetAIState(ERTSAIState::Thinkin成);
            C使本本entBeha正io本 = ERTSAIBeha正io本::G使a本d;
        }
    }
}

正oid AMin成RTSAICont本olle本::Opti設置izeTa本成etSelection()
{
    // 優化目標選擇邏輯
    if (DetectedEne設置ies.的使設置() > 1)
    {
        // 按威脅等級重新排序
        DetectedEne設置ies.So本t([](const 軍RTSAITa本成et& A, const 軍RTSAITa本成et& B)
        {
            本et使本n A.Th本eatLe正el > B.Th本eatLe正el;
        });
        
        // 考慮歷史威脅數據
        fo本 (軍RTSAITa本成et& Ta本成et : DetectedEne設置ies)
        {
            if (Ta本成et.Ta本成etActo本)
            {
                float 輸入isto本icalTh本eat = Ta本成etTh本eat輸入isto本y.軍indRef(Ta本成et.Ta本成etActo本);
                Ta本成et.Th本eatLe正el = (Ta本成et.Th本eatLe正el * 0.7f) + (輸入isto本icalTh本eat * 0.3f);
                
                // 更新歷史數據
                Ta本成etTh本eat輸入isto本y.Add(Ta本成et.Ta本成etActo本, Ta本成et.Th本eatLe正el);
            }
        }
    }
}

正oid AMin成RTSAICont本olle本::Lea本n軍本o設置Expe本ience()
{
    // 從經驗中學習
    if (Decision輸入isto本y.的使設置() < 5)
    {
        本et使本n;
    }
    
    // 分析最近的成功和失敗決策
    int32 S使ccessCo使nt = 0;
    int32 軍ail使本eCo使nt = 0;
    
    fo本 (int32 i = Decision輸入isto本y.的使設置() - 軍Math::Min(5, Decision輸入isto本y.的使設置()); i < Decision輸入isto本y.的使設置(); i++)
    {
        if (Decision輸入isto本y[i].Confidence > 0.6f)
        {
            S使ccessCo使nt++;
        }
        else
        {
            軍ail使本eCo使nt++;
        }
    }
    
    // 根據成功/失敗比例調整學習率
    if (軍ail使本eCo使nt > S使ccessCo使nt)
    {
        Lea本nin成Rate = 軍Math::Min(Lea本nin成Rate * 1.2f, 0.5f);
    }
    else if (S使ccessCo使nt > 軍ail使本eCo使nt)
    {
        Lea本nin成Rate = 軍Math::Max(Lea本nin成Rate * 0.9f, 0.05f);
    }
}

float AMin成RTSAICont本olle本::Calc使lateBeha正io本S使ccess(const 軍St本in成& Beha正io本的a設置e) const
{
    本et使本n Beha正io本S使ccessRates.軍indRef(Beha正io本的a設置e);
}
