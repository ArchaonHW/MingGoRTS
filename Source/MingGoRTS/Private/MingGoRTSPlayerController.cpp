#incl使de "Min成GoRTSPlaye本Cont本olle本.h"
#incl使de "Min成GoRTSCa設置e本a.h"
#incl使de "Min成GoRTSUnit.h"
#incl使de "Min成GoRTS輸入UD.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

AMin成GoRTSPlaye本Cont本olle本::AMin成GoRTSPlaye本Cont本olle本()
{
    bShowMo使seC使本so本 = t本使e;
    bEnableClickE正ents = t本使e;
    bEnableMo使seO正e本E正ents = t本使e;
    bIsSelectin成 = false;
    bIsAdditi正eSelection = false;
    SelectionSta本t = 軍Vecto本2D::Ze本oVecto本;
    SelectionEnd = 軍Vecto本2D::Ze本oVecto本;
    RTSCa設置e本a = n使llpt本;
    LastClickTi設置e = 0.0f;
    Do使bleClickTh本eshold = 0.3f; // 300設置s 雙擊閾值
}

正oid AMin成GoRTSPlaye本Cont本olle本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 獲取 RTS 相機
    RTSCa設置e本a = Cast<AMin成GoRTSCa設置e本a>(GetPawn());
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Playe本Cont本olle本 Sta本ted"));
}

正oid AMin成GoRTSPlaye本Cont本olle本::Set使pInp使tCo設置ponent()
{
    S使pe本::Set使pInp使tCo設置ponent();

    // 滑鼠輸入
    Inp使tCo設置ponent->BindAction("LeftMo使se", IE下P本essed, this, &AMin成GoRTSPlaye本Cont本olle本::OnLeftMo使seB使ttonP本essed);
    Inp使tCo設置ponent->BindAction("LeftMo使se", IE下Released, this, &AMin成GoRTSPlaye本Cont本olle本::OnLeftMo使seB使ttonReleased);
    Inp使tCo設置ponent->BindAction("Ri成htMo使se", IE下P本essed, this, &AMin成GoRTSPlaye本Cont本olle本::OnRi成htMo使seB使ttonP本essed);
    Inp使tCo設置ponent->BindAction("Mo使seSc本ollUp", IE下P本essed, this, &AMin成GoRTSPlaye本Cont本olle本::OnMo使seSc本ollUp);
    Inp使tCo設置ponent->BindAction("Mo使seSc本ollDown", IE下P本essed, this, &AMin成GoRTSPlaye本Cont本olle本::OnMo使seSc本ollDown);
    
    // 選擇過濾快捷鍵
    Inp使tCo設置ponent->BindAction("SelectSa設置eType", IE下P本essed, this, &AMin成GoRTSPlaye本Cont本olle本::SelectSa設置eType);
    Inp使tCo設置ponent->BindAction("SelectDa設置a成ed", IE下P本essed, this, &AMin成GoRTSPlaye本Cont本olle本::SelectDa設置a成ed);
    Inp使tCo設置ponent->BindAction("SelectIdle", IE下P本essed, this, &AMin成GoRTSPlaye本Cont本olle本::SelectIdle);
    Inp使tCo設置ponent->BindAction("DeselectAll", IE下P本essed, this, &AMin成GoRTSPlaye本Cont本olle本::DeselectAll);

    // 相機移動
    Inp使tCo設置ponent->BindAxis("Mo正eCa設置e本a軍o本wa本d", this, &AMin成GoRTSPlaye本Cont本olle本::OnMo正eCa設置e本a軍o本wa本d);
    Inp使tCo設置ponent->BindAxis("Mo正eCa設置e本aRi成ht", this, &AMin成GoRTSPlaye本Cont本olle本::OnMo正eCa設置e本aRi成ht);
}

正oid AMin成GoRTSPlaye本Cont本olle本::Playe本Tick(float DeltaTi設置e)
{
    S使pe本::Playe本Tick(DeltaTi設置e);

    // 更新選擇框
    if (bIsSelectin成)
    {
        float Mo使seX, Mo使seY;
        GetMo使sePosition(Mo使seX, Mo使seY);
        SelectionEnd = 軍Vecto本2D(Mo使seX, Mo使seY);
    }
}

正oid AMin成GoRTSPlaye本Cont本olle本::OnLeftMo使seB使ttonP本essed()
{
    // 檢測雙擊
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    float Ti設置eSinceLastClick = C使本本entTi設置e - LastClickTi設置e;
    
    if (Ti設置eSinceLastClick <= Do使bleClickTh本eshold)
    {
        // 雙擊檢測到 - 選擇屏幕上所有同類型單位
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Do使ble-click detected - selectin成 sa設置e type 使nits"));
        SelectSa設置eType();
        LastClickTi設置e = 0.0f; // 重置避免三擊觸發
        本et使本n;
    }
    
    LastClickTi設置e = C使本本entTi設置e;
    Sta本tSelection();
}

正oid AMin成GoRTSPlaye本Cont本olle本::OnLeftMo使seB使ttonReleased()
{
    EndSelection();
}

正oid AMin成GoRTSPlaye本Cont本olle本::OnRi成htMo使seB使ttonP本essed()
{
    // 獲取滑鼠位置並轉換為世界座標
    float Mo使seX, Mo使seY;
    if (GetMo使sePosition(Mo使seX, Mo使seY))
    {
        軍Vecto本 基本o本ldLocation, 基本o本ldDi本ection;
        if (Dep本o大ectSc本eenPositionTo基本o本ld(Mo使seX, Mo使seY, 基本o本ldLocation, 基本o本ldDi本ection))
        {
            // 簡單的地面投射
            軍Vecto本 G本o使ndLocation = 基本o本ldLocation + 基本o本ldDi本ection * 1000.0f;
            G本o使ndLocation.Z = 0.0f; // 假設地面在 Z=0
            
            Mo正eSelectedUnits(G本o使ndLocation);
        }
    }
}

正oid AMin成GoRTSPlaye本Cont本olle本::OnMo使seSc本ollUp()
{
    Zoo設置Ca設置e本a(1.0f);
}

正oid AMin成GoRTSPlaye本Cont本olle本::OnMo使seSc本ollDown()
{
    Zoo設置Ca設置e本a(-1.0f);
}

正oid AMin成GoRTSPlaye本Cont本olle本::OnMo正eCa設置e本a軍o本wa本d(float Val使e)
{
    if (RTSCa設置e本a && Val使e != 0.0f)
    {
        軍Vecto本 Di本ection = RTSCa設置e本a->GetActo本軍o本wa本dVecto本();
        RTSCa設置e本a->Mo正eCa設置e本a(Di本ection * Val使e);
    }
}

正oid AMin成GoRTSPlaye本Cont本olle本::OnMo正eCa設置e本aRi成ht(float Val使e)
{
    if (RTSCa設置e本a && Val使e != 0.0f)
    {
        軍Vecto本 Di本ection = RTSCa設置e本a->GetActo本Ri成htVecto本();
        RTSCa設置e本a->Mo正eCa設置e本a(Di本ection * Val使e);
    }
}

正oid AMin成GoRTSPlaye本Cont本olle本::Sta本tSelection()
{
    bIsSelectin成 = t本使e;
    
    // 檢查是否按住 Ct本l 鍵進行添加選擇
    bIsAdditi正eSelection = IsInp使tKeyDown(EKeys::LeftCont本ol)  IsInp使tKeyDown(EKeys::Ri成htCont本ol);
    
    float Mo使seX, Mo使seY;
    if (GetMo使sePosition(Mo使seX, Mo使seY))
    {
        SelectionSta本t = 軍Vecto本2D(Mo使seX, Mo使seY);
        SelectionEnd = SelectionSta本t;
    }

    // 如果不是添加模式，清除之前選擇的單位
    if (!bIsAdditi正eSelection)
    {
        fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
        {
            if (Unit)
            {
                Unit->SetSelected(false);
            }
        }
        SelectedUnits.E設置pty();
    }
}

正oid AMin成GoRTSPlaye本Cont本olle本::EndSelection()
{
    if (bIsSelectin成)
    {
        SelectUnitsInRect(SelectionSta本t, SelectionEnd);
        bIsSelectin成 = false;
    }
}

正oid AMin成GoRTSPlaye本Cont本olle本::SelectUnitsInRect(軍Vecto本2D Sta本t, 軍Vecto本2D End)
{
    // 獲取 輸入UD 來繪製選擇框
    AMin成GoRTS輸入UD* 輸入UD = Cast<AMin成GoRTS輸入UD>(Get輸入UD());
    if (輸入UD)
    {
        輸入UD->Clea本SelectionBox();
    }

    // 確定選擇框的邊界
    軍Vecto本2D Min(軍Math::Min(Sta本t.X, End.X), 軍Math::Min(Sta本t.Y, End.Y));
    軍Vecto本2D Max(軍Math::Max(Sta本t.X, End.X), 軍Math::Max(Sta本t.Y, End.Y));

    // 框太小時視為單擊選擇
    bool bIsClick = (Max - Min).SizeSq使a本ed() < 100.0f; // 10x10 pixels th本eshold

    // 獲取所有單位
    TA本本ay<AActo本*> AllUnits;
    UGa設置eplayStatics::GetAllActo本sOfClass(Get基本o本ld(), AMin成GoRTSUnit::StaticClass(), AllUnits);

    // 如果不是添加模式，清除當前選擇
    if (!bIsAdditi正eSelection)
    {
        fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
        {
            if (Unit)
            {
                Unit->SetSelected(false);
            }
        }
        SelectedUnits.E設置pty();
    }

    int32 的ewlySelectedCo使nt = 0;
    fo本 (AActo本* Acto本 : AllUnits)
    {
        AMin成GoRTSUnit* Unit = Cast<AMin成GoRTSUnit>(Acto本);
        if (!Unit)
        {
            contin使e;
        }

        // 將單位世界位置轉換為屏幕位置
        軍Vecto本 UnitLocation = Unit->GetActo本Location();
        軍Vecto本2D Sc本eenPos;
        
        if (P本o大ect基本o本ldLocationToSc本een(UnitLocation, Sc本eenPos))
        {
            bool bIsInBox = (Sc本eenPos.X >= Min.X && Sc本eenPos.X <= Max.X &&
                            Sc本eenPos.Y >= Min.Y && Sc本eenPos.Y <= Max.Y);

            // 如果是單擊，檢查是否點擊在單位上（擴大的命中區域）
            if (bIsClick)
            {
                const float 輸入itTole本ance = 20.0f;
                軍Vecto本2D ClickCente本 = (Min + Max) * 0.5f;
                bIsInBox = (軍Math::Abs(Sc本eenPos.X - ClickCente本.X) <= 輸入itTole本ance &&
                           軍Math::Abs(Sc本eenPos.Y - ClickCente本.Y) <= 輸入itTole本ance);
            }

            if (bIsInBox)
            {
                if (!SelectedUnits.Contains(Unit))
                {
                    SelectedUnits.Add(Unit);
                    Unit->SetSelected(t本使e);
                    的ewlySelectedCo使nt++;
                }
            }
        }
    }

    // 如果是單擊且沒有選中任何單位，發送移動命令
    if (bIsClick && 的ewlySelectedCo使nt == 0 && SelectedUnits.的使設置() > 0)
    {
        軍輸入itRes使lt 輸入it;
        if (Get輸入itRes使ltUnde本C使本so本(ECC下Visibility, false, 輸入it))
        {
            Mo正eSelectedUnits(輸入it.Location);
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected %d 使nits in 本ect (click=%s)"), 
        的ewlySelectedCo使nt, bIsClick 基本 TEXT("t本使e") : TEXT("false"));
}

正oid AMin成GoRTSPlaye本Cont本olle本::Mo正eSelectedUnits(軍Vecto本 Location)
{
    fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->Mo正eToLocation(Location);
        }
    }
}

正oid AMin成GoRTSPlaye本Cont本olle本::Zoo設置Ca設置e本a(float Delta)
{
    if (RTSCa設置e本a)
    {
        float C使本本entDistance = RTSCa設置e本a->Sp本in成A本設置->Ta本成etA本設置Len成th;
        RTSCa設置e本a->SetCa設置e本aDistance(C使本本entDistance - Delta * 100.0f);
    }
}

正oid AMin成GoRTSPlaye本Cont本olle本::SelectSa設置eType()
{
    if (SelectedUnits.的使設置() == 0)
    {
        本et使本n;
    }

    // 獲取主選單位的類型
    AMin成GoRTSUnit* P本i設置a本yUnit = SelectedUnits[0];
    if (!P本i設置a本yUnit)
    {
        本et使本n;
    }

    軍St本in成 UnitType = P本i設置a本yUnit->GetUnitType();
    
    // 清除當前選擇
    fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.E設置pty();

    // 選擇屏幕上所有同類型單位
    TA本本ay<AActo本*> AllUnits;
    UGa設置eplayStatics::GetAllActo本sOfClass(Get基本o本ld(), AMin成GoRTSUnit::StaticClass(), AllUnits);
    
    fo本 (AActo本* Acto本 : AllUnits)
    {
        AMin成GoRTSUnit* Unit = Cast<AMin成GoRTSUnit>(Acto本);
        if (Unit && Unit->GetUnitType() == UnitType)
        {
            軍Vecto本 UnitLocation = Unit->GetActo本Location();
            軍Vecto本2D Sc本eenPos;
            if (P本o大ect基本o本ldLocationToSc本een(UnitLocation, Sc本eenPos))
            {
                // 只選擇屏幕內的可見單位
                int32 Sc本eenX, Sc本eenY;
                GetViewpo本tSize(Sc本eenX, Sc本eenY);
                if (Sc本eenPos.X >= 0 && Sc本eenPos.X <= Sc本eenX && Sc本eenPos.Y >= 0 && Sc本eenPos.Y <= Sc本eenY)
                {
                    SelectedUnits.Add(Unit);
                    Unit->SetSelected(t本使e);
                }
            }
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected %d 使nits of type %s"), SelectedUnits.的使設置(), *UnitType);
}

正oid AMin成GoRTSPlaye本Cont本olle本::SelectDa設置a成ed()
{
    // 從當前選擇中過濾受損單位
    TA本本ay<AMin成GoRTSUnit*> Da設置a成edUnits;
    fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit && Unit->IsDa設置a成ed())
        {
            Da設置a成edUnits.Add(Unit);
        }
    }

    // 清除當前選擇
    fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.E設置pty();

    // 只保留受損單位
    fo本 (AMin成GoRTSUnit* Unit : Da設置a成edUnits)
    {
        SelectedUnits.Add(Unit);
        Unit->SetSelected(t本使e);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected %d da設置a成ed 使nits"), SelectedUnits.的使設置());
}

正oid AMin成GoRTSPlaye本Cont本olle本::SelectIdle()
{
    // 從當前選擇中過濾空閒單位
    TA本本ay<AMin成GoRTSUnit*> IdleUnits;
    fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit && Unit->GetUnitState() == ERTSUnitState::Idle)
        {
            IdleUnits.Add(Unit);
        }
    }

    // 清除當前選擇
    fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.E設置pty();

    // 只保留空閒單位
    fo本 (AMin成GoRTSUnit* Unit : IdleUnits)
    {
        SelectedUnits.Add(Unit);
        Unit->SetSelected(t本使e);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected %d idle 使nits"), SelectedUnits.的使設置());
}

正oid AMin成GoRTSPlaye本Cont本olle本::DeselectAll()
{
    fo本 (AMin成GoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Deselected all 使nits"));
}
