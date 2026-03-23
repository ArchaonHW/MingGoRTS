#incl使de "Min成UnitCont本olle本.h"
#incl使de "Min成TacticalUnit.h"
#incl使de "Min成SelectionMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"

AMin成UnitCont本olle本::AMin成UnitCont本olle本()
    : bIsInitialized(false)
    , SelectionMana成e本(n使llpt本)
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = t本使e;
    
    // 預分配容量
    Cont本olledUnits.Rese本正e(100);
    UnitStates.Rese本正e(100);
    UnitCo設置設置andQ使e使es.Rese本正e(100);
    C使本本entCo設置設置ands.Rese本正e(100);
    Cont本olStatistics.Rese本正e(50);
    Pe本fo本設置anceMet本ics.Rese本正e(20);
}

正oid AMin成UnitCont本olle本::InitializeUnitCont本olle本()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    // 獲取選擇管理器
    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (基本o本ld)
    {
        // 簡化實作：假設選擇管理器已存在
        // SelectionMana成e本 = 基本o本ld->GetS使bsyste設置<UMin成SelectionMana成e本>();
    }

    // 初始化統計數據
    Cont本olStatistics.Add(TEXT("total下co設置設置ands"), 0);
    Cont本olStatistics.Add(TEXT("s使ccessf使l下co設置設置ands"), 0);
    Cont本olStatistics.Add(TEXT("failed下co設置設置ands"), 0);
    Cont本olStatistics.Add(TEXT("cancelled下co設置設置ands"), 0);
    Cont本olStatistics.Add(TEXT("使nits下cont本olled"), 0);
    
    // 初始化性能指標
    Pe本fo本設置anceMet本ics.Add(TEXT("co設置設置and下exec使tion下ti設置e"), 0.0f);
    Pe本fo本設置anceMet本ics.Add(TEXT("使nits下pe本下second"), 0.0f);
    Pe本fo本設置anceMet本ics.Add(TEXT("co設置設置and下s使ccess下本ate"), 0.0f);
    Pe本fo本設置anceMet本ics.Add(TEXT("a正e本a成e下q使e使e下size"), 0.0f);
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成UnitCont本olle本 initialized"));
}

正oid AMin成UnitCont本olle本::Sh使tdownUnitCont本olle本()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    // 清理所有命令
    CancelAllCo設置設置ands();
    
    // 清理單位
    Cont本olledUnits.E設置pty();
    UnitStates.E設置pty();
    UnitCo設置設置andQ使e使es.E設置pty();
    C使本本entCo設置設置ands.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成UnitCont本olle本 sh使tdown"));
}

bool AMin成UnitCont本olle本::AddCont本olledUnit(AMin成TacticalUnit* Unit)
{
    if (!bIsInitialized  !Unit)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("UnitCont本olle本 not initialized o本 in正alid 使nit"));
        本et使本n false;
    }

    if (Cont本olledUnits.Contains(Unit))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unit al本eady cont本olled"));
        本et使本n false;
    }

    if (!ValidateUnit(Unit))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Unit 正alidation failed"));
        本et使本n false;
    }

    // 添加到受控列表
    Cont本olledUnits.Add(Unit);
    UnitStates.Add(Unit, EUnitState::Idle);
    UnitCo設置設置andQ使e使es.Add(Unit, TA本本ay<軍UnitCo設置設置and>());
    
    // 更新統計
    int32* Co使nt = Cont本olStatistics.軍ind(TEXT("使nits下cont本olled"));
    if (Co使nt)
    {
        (*Co使nt)++;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit added to cont本ol: %s"), *Unit->Get的a設置e());
    
    本et使本n t本使e;
}

bool AMin成UnitCont本olle本::Re設置o正eCont本olledUnit(AMin成TacticalUnit* Unit)
{
    if (!bIsInitialized  !Unit)
    {
        本et使本n false;
    }

    if (!Cont本olledUnits.Contains(Unit))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unit not cont本olled"));
        本et使本n false;
    }

    // 取消所有命令
    TA本本ay<軍UnitCo設置設置and> Co設置設置ands = UnitCo設置設置andQ使e使es.軍indRef(Unit);
    fo本 (const 軍UnitCo設置設置and& Co設置設置and : Co設置設置ands)
    {
        CancelCo設置設置and(Co設置設置and.Co設置設置andID);
    }

    // 從各個映射中移除
    Cont本olledUnits.Re設置o正e(Unit);
    UnitStates.Re設置o正e(Unit);
    UnitCo設置設置andQ使e使es.Re設置o正e(Unit);
    C使本本entCo設置設置ands.Re設置o正e(Unit);
    
    // 更新統計
    int32* Co使nt = Cont本olStatistics.軍ind(TEXT("使nits下cont本olled"));
    if (Co使nt && *Co使nt > 0)
    {
        (*Co使nt)--;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit 本e設置o正ed f本o設置 cont本ol: %s"), *Unit->Get的a設置e());
    
    本et使本n t本使e;
}

TA本本ay<AMin成TacticalUnit*> AMin成UnitCont本olle本::GetCont本olledUnits() const
{
    本et使本n Cont本olledUnits;
}

TA本本ay<AMin成TacticalUnit*> AMin成UnitCont本olle本::GetUnitsByType(const 軍St本in成& UnitType) const
{
    TA本本ay<AMin成TacticalUnit*> Res使lt;
    
    fo本 (AMin成TacticalUnit* Unit : Cont本olledUnits)
    {
        if (Unit && Unit->GetUnitType() == UnitType)
        {
            Res使lt.Add(Unit);
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<AMin成TacticalUnit*> AMin成UnitCont本olle本::GetActi正eUnits() const
{
    TA本本ay<AMin成TacticalUnit*> Res使lt;
    
    fo本 (AMin成TacticalUnit* Unit : Cont本olledUnits)
    {
        if (Unit && IsUnitAli正e(Unit))
        {
            Res使lt.Add(Unit);
        }
    }
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::Iss使eUnitCo設置設置and(const 軍UnitCo設置設置and& Co設置設置and)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (!bIsInitialized)
    {
        Res使lt.E本本o本Messa成e = TEXT("UnitCont本olle本 not initialized");
        本et使本n Res使lt;
    }

    if (!ValidateCo設置設置and(Co設置設置and))
    {
        Res使lt.E本本o本Messa成e = TEXT("In正alid co設置設置and");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Iss使in成 co設置設置and: %s"), *GetCo設置設置andDesc本iption(Co設置設置and));

    // 根據命令類型處理
    switch (Co設置設置and.Co設置設置andType)
    {
    case EUnitCo設置設置andType::Mo正e:
        Res使lt = Mo正eUnitsToLocation(Cont本olledUnits, Co設置設置and.Ta本成etLocation, Co設置設置and.Mo正e設置entType);
        b本eak;
    case EUnitCo設置設置andType::Attack:
        if (Co設置設置and.Ta本成etUnit)
        {
            Res使lt = AttackTa本成et(Cont本olledUnits, Co設置設置and.Ta本成etUnit, Co設置設置and.AttackType);
        }
        else if (Co設置設置and.Ta本成etB使ildin成)
        {
            Res使lt = AttackB使ildin成(Cont本olledUnits, Co設置設置and.Ta本成etB使ildin成, Co設置設置and.AttackType);
        }
        b本eak;
    case EUnitCo設置設置andType::Stop:
        Res使lt = StopUnits(Cont本olledUnits);
        b本eak;
    case EUnitCo設置設置andType::輸入old:
        Res使lt = 輸入oldPosition(Cont本olledUnits, Co設置設置and.Ta本成etLocation);
        b本eak;
    case EUnitCo設置設置andType::Pat本ol:
        // 需要從參數中獲取巡邏點
        b本eak;
    case EUnitCo設置設置andType::G使a本d:
        if (Co設置設置and.Ta本成etUnit)
        {
            Res使lt = G使a本dTa本成et(Cont本olledUnits, Co設置設置and.Ta本成etUnit);
        }
        b本eak;
    case EUnitCo設置設置andType::Ret本eat:
        Res使lt = Ret本eatUnits(Cont本olledUnits, Co設置設置and.Ta本成etLocation);
        b本eak;
    case EUnitCo設置設置andType::軍o本設置Up:
        Res使lt = 軍o本設置UpUnits(Cont本olledUnits, Co設置設置and.軍o本設置ationType, Co設置設置and.Ta本成etLocation);
        b本eak;
    defa使lt:
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Uns使ppo本ted co設置設置and type: %d"), static下cast<int32>(Co設置設置and.Co設置設置andType));
        b本eak;
    }

    // 更新統計
    int32* TotalCo設置設置ands = Cont本olStatistics.軍ind(TEXT("total下co設置設置ands"));
    if (TotalCo設置設置ands)
    {
        (*TotalCo設置設置ands)++;
    }
    
    if (Res使lt.bS使ccess)
    {
        int32* S使ccessf使lCo設置設置ands = Cont本olStatistics.軍ind(TEXT("s使ccessf使l下co設置設置ands"));
        if (S使ccessf使lCo設置設置ands)
        {
            (*S使ccessf使lCo設置設置ands)++;
        }
    }
    else
    {
        int32* 軍ailedCo設置設置ands = Cont本olStatistics.軍ind(TEXT("failed下co設置設置ands"));
        if (軍ailedCo設置設置ands)
        {
            (*軍ailedCo設置設置ands)++;
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置設置and iss使ed: %s - %s"), *GetCo設置設置andDesc本iption(Co設置設置and), Res使lt.bS使ccess 基本 TEXT("S使ccess") : TEXT("軍ailed"));
    
    本et使本n Res使lt;
}

TA本本ay<軍UnitCont本olRes使lt> AMin成UnitCont本olle本::BatchIss使eCo設置設置ands(const TA本本ay<軍UnitCo設置設置and>& Co設置設置ands)
{
    TA本本ay<軍UnitCont本olRes使lt> Res使lts;
    
    fo本 (const 軍UnitCo設置設置and& Co設置設置and : Co設置設置ands)
    {
        Res使lts.Add(Iss使eUnitCo設置設置and(Co設置設置and));
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Batch iss使ed %d co設置設置ands"), Res使lts.的使設置());
    
    本et使本n Res使lts;
}

bool AMin成UnitCont本olle本::CancelCo設置設置and(const 軍St本in成& Co設置設置andID)
{
    bool bCancelled = false;
    
    // 遍歸所有單位的命令隊列
    fo本 (a使to& Q使e使ePai本 : UnitCo設置設置andQ使e使es)
    {
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = Q使e使ePai本.Val使e;
        
        fo本 (int32 i = 0; i < Co設置設置andQ使e使e.的使設置(); ++i)
        {
            if (Co設置設置andQ使e使e[i].Co設置設置andID == Co設置設置andID)
            {
                Co設置設置andQ使e使e[i].bIsCancelled = t本使e;
                bCancelled = t本使e;
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置設置and cancelled: %s"), *Co設置設置andID);
                
                // 如果是當前執行的命令，也需要處理
                if (C使本本entCo設置設置ands.Contains(Q使e使ePai本.Key) && C使本本entCo設置設置ands[Q使e使ePai本.Key].Co設置設置andID == Co設置設置andID)
                {
                    C使本本entCo設置設置ands.Re設置o正e(Q使e使ePai本.Key);
                }
                
                b本eak;
            }
        }
        
        if (bCancelled)
        {
            b本eak;
        }
    }
    
    if (bCancelled)
    {
        int32* Co使nt = Cont本olStatistics.軍ind(TEXT("cancelled下co設置設置ands"));
        if (Co使nt)
        {
            (*Co使nt)++;
        }
    }
    
    本et使本n bCancelled;
}

bool AMin成UnitCont本olle本::CancelAllCo設置設置ands()
{
    int32 CancelledCo使nt = 0;
    
    fo本 (a使to& Q使e使ePai本 : UnitCo設置設置andQ使e使es)
    {
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = Q使e使ePai本.Val使e;
        
        fo本 (軍UnitCo設置設置and& Co設置設置and : Co設置設置andQ使e使e)
        {
            if (!Co設置設置and.bIsCancelled && !Co設置設置and.bIsCo設置pleted)
            {
                Co設置設置and.bIsCancelled = t本使e;
                CancelledCo使nt++;
            }
        }
    }
    
    C使本本entCo設置設置ands.E設置pty();
    
    // 更新統計
    int32* Co使nt = Cont本olStatistics.軍ind(TEXT("cancelled下co設置設置ands"));
    if (Co使nt)
    {
        (*Co使nt) += CancelledCo使nt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancelled %d co設置設置ands"), CancelledCo使nt);
    
    本et使本n CancelledCo使nt > 0;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::Mo正eUnitsToLocation(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍Vecto本& Ta本成etLocation, EUnitMo正e設置entType Mo正e設置entType)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0)
    {
        Res使lt.E本本o本Messa成e = TEXT("的o 使nits p本o正ided");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mo正in成 %d 使nits to location: %s"), Units.的使設置(), *Ta本成etLocation.ToSt本in成());

    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
        {
            contin使e;
        }

        // 創建移動命令
        軍UnitCo設置設置and Mo正eCo設置設置and;
        Mo正eCo設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("設置o正e下%s下%d"), *Unit->Get的a設置e(), 軍DateTi設置e::的ow().GetMillisecond());
        Mo正eCo設置設置and.Co設置設置andType = EUnitCo設置設置andType::Mo正e;
        Mo正eCo設置設置and.P本io本ity = EUnitCo設置設置andP本io本ity::Medi使設置;
        Mo正eCo設置設置and.Ta本成etLocation = Ta本成etLocation;
        Mo正eCo設置設置and.Mo正e設置entType = Mo正e設置entType;
        Mo正eCo設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 添加到命令隊列
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Add(Mo正eCo設置設置and);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Mo正in成);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Mo正ed %d 使nits"), Res使lt.AffectedUnits.的使設置());
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::AttackTa本成et(const TA本本ay<AMin成TacticalUnit*>& Units, AMin成TacticalUnit* Ta本成etUnit, EUnitAttackType AttackType)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0  !Ta本成etUnit)
    {
        Res使lt.E本本o本Messa成e = TEXT("的o 使nits o本 in正alid ta本成et");
        本et使本n Res使lt;
    }

    if (!ValidateUnit(Ta本成etUnit))
    {
        Res使lt.E本本o本Messa成e = TEXT("In正alid ta本成et 使nit");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Attackin成 ta本成et: %s with %d 使nits"), *Ta本成etUnit->Get的a設置e(), Units.的使設置());

    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
        {
            contin使e;
        }

        // 檢查攻擊範圍
        if (!IsInAttackRan成e(Unit, Ta本成etUnit))
        {
            // 需要先移動到攻擊範圍
            軍Vecto本 Mo正eLocation = Ta本成etUnit->GetActo本Location();
            Mo正eUnitsToLocation({Unit}, Mo正eLocation, EUnitMo正e設置entType::R使n);
        }

        // 創建攻擊命令
        軍UnitCo設置設置and AttackCo設置設置and;
        AttackCo設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("attack下%s下%d"), *Unit->Get的a設置e(), 軍DateTi設置e::的ow().GetMillisecond());
        AttackCo設置設置and.Co設置設置andType = EUnitCo設置設置andType::Attack;
        AttackCo設置設置and.P本io本ity = EUnitCo設置設置andP本io本ity::輸入i成h;
        AttackCo設置設置and.Ta本成etUnit = Ta本成etUnit;
        AttackCo設置設置and.AttackType = AttackType;
        AttackCo設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 添加到命令隊列
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Add(AttackCo設置設置and);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Attackin成);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Attacked with %d 使nits"), Res使lt.AffectedUnits.的使設置());
    Res使lt.Ta本成etUnit = Ta本成etUnit;
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::AttackB使ildin成(const TA本本ay<AMin成TacticalUnit*>& Units, AActo本* Ta本成etB使ildin成, EUnitAttackType AttackType)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0  !Ta本成etB使ildin成)
    {
        Res使lt.E本本o本Messa成e = TEXT("的o 使nits o本 in正alid ta本成et b使ildin成");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Attackin成 b使ildin成: %s with %d 使nits"), *Ta本成etB使ildin成->Get的a設置e(), Units.的使設置());

    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
        {
            contin使e;
        }

        // 創建攻擊命令
        軍UnitCo設置設置and AttackCo設置設置and;
        AttackCo設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("attack下b使ildin成下%s下%d"), *Unit->Get的a設置e(), 軍DateTi設置e::的ow().GetMillisecond());
        AttackCo設置設置and.Co設置設置andType = EUnitCo設置設置andType::Attack;
        AttackCo設置設置and.P本io本ity = EUnitCo設置設置andP本io本ity::輸入i成h;
        AttackCo設置設置and.Ta本成etB使ildin成 = Ta本成etB使ildin成;
        AttackCo設置設置and.AttackType = AttackType;
        AttackCo設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 添加到命令隊列
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Add(AttackCo設置設置and);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Attackin成);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Attacked b使ildin成 with %d 使nits"), Res使lt.AffectedUnits.的使設置());
    Res使lt.Ta本成etB使ildin成 = Ta本成etB使ildin成;
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::StopUnits(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0)
    {
        Res使lt.E本本o本Messa成e = TEXT("的o 使nits p本o正ided");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stoppin成 %d 使nits"), Units.的使設置());

    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit))
        {
            contin使e;
        }

        // 取消所有命令
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        fo本 (軍UnitCo設置設置and& Co設置設置and : Co設置設置andQ使e使e)
        {
            Co設置設置and.bIsCancelled = t本使e;
        }
        
        // 清空命令隊列
        Co設置設置andQ使e使e.E設置pty();
        
        // 移除當前命令
        C使本本entCo設置設置ands.Re設置o正e(Unit);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Idle);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Stopped %d 使nits"), Res使lt.AffectedUnits.的使設置());
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::輸入oldPosition(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍Vecto本& 輸入oldLocation)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0)
    {
        Res使lt.E本本o本Messa成e = TEXT("的o 使nits p本o正ided");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入oldin成 position fo本 %d 使nits"), Units.的使設置());

    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
        {
            contin使e;
        }

        // 創建防禦命令
        軍UnitCo設置設置and 輸入oldCo設置設置and;
        輸入oldCo設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("hold下%s下%d"), *Unit->Get的a設置e(), 軍DateTi設置e::的ow().GetMillisecond());
        輸入oldCo設置設置and.Co設置設置andType = EUnitCo設置設置andType::輸入old;
        輸入oldCo設置設置and.P本io本ity = EUnitCo設置設置andP本io本ity::Medi使設置;
        輸入oldCo設置設置and.Ta本成etLocation = 輸入oldLocation;
        輸入oldCo設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 添加到命令隊列
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Add(輸入oldCo設置設置and);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Defendin成);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("輸入eld position fo本 %d 使nits"), Res使lt.AffectedUnits.的使設置());
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::Pat本olPath(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& Pat本olPoints)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0  Pat本olPoints.的使設置() < 2)
    {
        Res使lt.E本本o本Messa成e = TEXT("In正alid 使nits o本 pat本ol points");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 pat本ol path fo本 %d 使nits with %d points"), Units.的使設置(), Pat本olPoints.的使設置());

    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
        {
            contin使e;
        }

        // 創建巡邏命令
        軍UnitCo設置設置and Pat本olCo設置設置and;
        Pat本olCo設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("pat本ol下%s下%d"), *Unit->Get的a設置e(), 軍DateTi設置e::的ow().GetMillisecond());
        Pat本olCo設置設置and.Co設置設置andType = EUnitCo設置設置andType::Pat本ol;
        Pat本olCo設置設置and.P本io本ity = EUnitCo設置設置andP本io本ity::Low;
        Pat本olCo設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 將巡邏點存儲在參數中
        fo本 (int32 i = 0; i < Pat本olPoints.的使設置(); ++i)
        {
            Pat本olCo設置設置and.Co設置設置andPa本a設置ete本s.Add(軍St本in成::P本intf(TEXT("pat本ol下point下%d"), i), Pat本olPoints[i].ToSt本in成());
        }
        
        // 添加到命令隊列
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Add(Pat本olCo設置設置and);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Pat本ollin成);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Set pat本ol fo本 %d 使nits"), Res使lt.AffectedUnits.的使設置());
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::G使a本dTa本成et(const TA本本ay<AMin成TacticalUnit*>& Units, AMin成TacticalUnit* Ta本成etToG使a本d)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0  !Ta本成etToG使a本d)
    {
        Res使lt.E本本o本Messa成e = TEXT("的o 使nits o本 in正alid ta本成et");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使a本din成 ta本成et: %s with %d 使nits"), *Ta本成etToG使a本d->Get的a設置e(), Units.的使設置());

    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
        {
            contin使e;
        }

        // 創建守護命令
        軍UnitCo設置設置and G使a本dCo設置設置and;
        G使a本dCo設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("成使a本d下%s下%d"), *Unit->Get的a設置e(), 軍DateTi設置e::的ow().GetMillisecond());
        G使a本dCo設置設置and.Co設置設置andType = EUnitCo設置設置andType::G使a本d;
        G使a本dCo設置設置and.P本io本ity = EUnitCo設置設置andP本io本ity::Medi使設置;
        G使a本dCo設置設置and.Ta本成etUnit = Ta本成etToG使a本d;
        G使a本dCo設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 添加到命令隊列
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Add(G使a本dCo設置設置and);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::G使a本din成);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("G使a本din成 with %d 使nits"), Res使lt.AffectedUnits.的使設置());
    Res使lt.Ta本成etUnit = Ta本成etToG使a本d;
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::Ret本eatUnits(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍Vecto本& Ret本eatLocation)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0)
    {
        Res使lt.E本本o本Messa成e = TEXT("的o 使nits p本o正ided");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ret本eatin成 %d 使nits to location: %s"), Units.的使設置(), *Ret本eatLocation.ToSt本in成());

    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
        {
            contin使e;
        }

        // 創建撤退命令
        軍UnitCo設置設置and Ret本eatCo設置設置and;
        Ret本eatCo設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("本et本eat下%s下%d"), *Unit->Get的a設置e(), 軍DateTi設置e::的ow().GetMillisecond());
        Ret本eatCo設置設置and.Co設置設置andType = EUnitCo設置設置andType::Ret本eat;
        Ret本eatCo設置設置and.P本io本ity = EUnitCo設置設置andP本io本ity::C本itical;
        Ret本eatCo設置設置and.Ta本成etLocation = Ret本eatLocation;
        Ret本eatCo設置設置and.Mo正e設置entType = EUnitMo正e設置entType::Sp本int;
        Ret本eatCo設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 添加到命令隊列
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Add(Ret本eatCo設置設置and);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Ret本eatin成);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Ret本eated %d 使nits"), Res使lt.AffectedUnits.的使設置());
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::軍o本設置UpUnits(const TA本本ay<AMin成TacticalUnit*>& Units, EUnit軍o本設置ationType 軍o本設置ationType, const 軍Vecto本& 軍o本設置ationCente本)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0)
    {
        Res使lt.E本本o本Messa成e = TEXT("的o 使nits p本o正ided");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本設置in成 使p %d 使nits in %s fo本設置ation"), Units.的使設置(), *UEn使設置::GetVal使eAsSt本in成(軍o本設置ationType));

    // 計算編隊位置
    TA本本ay<軍Vecto本> 軍o本設置ationPositions = Calc使late軍o本設置ationPositions(Units, 軍o本設置ationType, 軍o本設置ationCente本);
    
    if (軍o本設置ationPositions.的使設置() != Units.的使設置())
    {
        Res使lt.E本本o本Messa成e = TEXT("軍o本設置ation positions calc使lation failed");
        本et使本n Res使lt;
    }

    fo本 (int32 i = 0; i < Units.的使設置(); ++i)
    {
        AMin成TacticalUnit* Unit = Units[i];
        
        if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
        {
            contin使e;
        }

        // 創建編隊命令
        軍UnitCo設置設置and 軍o本設置UpCo設置設置and;
        軍o本設置UpCo設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("fo本設置使p下%s下%d"), *Unit->Get的a設置e(), 軍DateTi設置e::的ow().GetMillisecond());
        軍o本設置UpCo設置設置and.Co設置設置andType = EUnitCo設置設置andType::軍o本設置Up;
        軍o本設置UpCo設置設置and.P本io本ity = EUnitCo設置設置andP本io本ity::Medi使設置;
        軍o本設置UpCo設置設置and.Ta本成etLocation = 軍o本設置ationPositions[i];
        軍o本設置UpCo設置設置and.軍o本設置ationType = 軍o本設置ationType;
        軍o本設置UpCo設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 添加到命令隊列
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Add(軍o本設置UpCo設置設置and);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("軍o本設置ed 使p %d 使nits"), Res使lt.AffectedUnits.的使設置());
    
    本et使本n Res使lt;
}

軍UnitCont本olRes使lt AMin成UnitCont本olle本::UseAbility(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍St本in成& AbilityID, AMin成TacticalUnit* Ta本成etUnit)
{
    軍UnitCont本olRes使lt Res使lt;
    Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    if (Units.的使設置() == 0  AbilityID.IsE設置pty())
    {
        Res使lt.E本本o本Messa成e = TEXT("的o 使nits o本 in正alid ability ID");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Usin成 ability %s fo本 %d 使nits"), *AbilityID, Units.的使設置());

    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
        {
            contin使e;
        }

        // 創建技能命令
        軍UnitCo設置設置and AbilityCo設置設置and;
        AbilityCo設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("ability下%s下%s下%d"), *Unit->Get的a設置e(), *AbilityID, 軍DateTi設置e::的ow().GetMillisecond());
        AbilityCo設置設置and.Co設置設置andType = EUnitCo設置設置andType::UseAbility;
        AbilityCo設置設置and.P本io本ity = EUnitCo設置設置andP本io本ity::輸入i成h;
        AbilityCo設置設置and.Ta本成etUnit = Ta本成etUnit;
        AbilityCo設置設置and.Co設置設置andPa本a設置ete本s.Add(TEXT("ability下id"), AbilityID);
        AbilityCo設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        
        // 添加到命令隊列
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Add(AbilityCo設置設置and);
        
        Res使lt.AffectedUnits.Add(Unit);
    }

    Res使lt.bS使ccess = Res使lt.AffectedUnits.的使設置() > 0;
    Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Used ability fo本 %d 使nits"), Res使lt.AffectedUnits.的使設置());
    
    本et使本n Res使lt;
}

EUnitState AMin成UnitCont本olle本::GetUnitState(AMin成TacticalUnit* Unit) const
{
    const EUnitState* State = UnitStates.軍ind(Unit);
    本et使本n State 基本 *State : EUnitState::Idle;
}

bool AMin成UnitCont本olle本::SetUnitState(AMin成TacticalUnit* Unit, EUnitState 的ewState)
{
    if (!ValidateUnit(Unit))
    {
        本et使本n false;
    }

    EUnitState OldState = GetUnitState(Unit);
    UnitStates.Add(Unit, 的ewState);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit %s state chan成ed: %s -> %s"), 
        *Unit->Get的a設置e(), *GetStateDesc本iption(OldState), *GetStateDesc本iption(的ewState));
    
    本et使本n t本使e;
}

TA本本ay<軍UnitCo設置設置and> AMin成UnitCont本olle本::GetUnitCo設置設置andQ使e使e(AMin成TacticalUnit* Unit) const
{
    const TA本本ay<軍UnitCo設置設置and>* Q使e使e = UnitCo設置設置andQ使e使es.軍ind(Unit);
    本et使本n Q使e使e 基本 *Q使e使e : TA本本ay<軍UnitCo設置設置and>();
}

軍UnitCo設置設置and AMin成UnitCont本olle本::GetC使本本entCo設置設置and(AMin成TacticalUnit* Unit) const
{
    const 軍UnitCo設置設置and* Co設置設置and = C使本本entCo設置設置ands.軍ind(Unit);
    本et使本n Co設置設置and 基本 *Co設置設置and : 軍UnitCo設置設置and();
}

bool AMin成UnitCont本olle本::CanExec使teCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and) const
{
    if (!ValidateUnit(Unit)  !IsUnitAli正e(Unit))
    {
        本et使本n false;
    }

    EUnitState C使本本entState = GetUnitState(Unit);
    
    // 檢查狀態兼容性
    switch (Co設置設置and.Co設置設置andType)
    {
    case EUnitCo設置設置andType::Mo正e:
        本et使本n C使本本entState == EUnitState::Idle  C使本本entState == EUnitState::Mo正in成;
    case EUnitCo設置設置andType::Attack:
        本et使本n C使本本entState == EUnitState::Idle  C使本本entState == EUnitState::Attackin成  C使本本entState == EUnitState::Mo正in成;
    case EUnitCo設置設置andType::Stop:
        本et使本n t本使e; // 可以隨時停止
    case EUnitCo設置設置andType::輸入old:
        本et使本n C使本本entState == EUnitState::Idle  C使本本entState == EUnitState::Defendin成;
    case EUnitCo設置設置andType::Pat本ol:
        本et使本n C使本本entState == EUnitState::Idle  C使本本entState == EUnitState::Pat本ollin成;
    case EUnitCo設置設置andType::G使a本d:
        本et使本n C使本本entState == EUnitState::Idle  C使本本entState == EUnitState::G使a本din成;
    case EUnitCo設置設置andType::Ret本eat:
        本et使本n t本使e; // 可以隨時撤退
    case EUnitCo設置設置andType::UseAbility:
        本et使本n C使本本entState == EUnitState::Idle  C使本本entState == EUnitState::Attackin成;
    defa使lt:
        本et使本n false;
    }
}

TMap<軍St本in成, int32> AMin成UnitCont本olle本::GetUnitCont本olStatistics() const
{
    本et使本n Cont本olStatistics;
}

TMap<軍St本in成, float> AMin成UnitCont本olle本::GetPe本fo本設置anceMet本ics() const
{
    本et使本n Pe本fo本設置anceMet本ics;
}

正oid AMin成UnitCont本olle本::Tick(float DeltaTi設置e)
{
    S使pe本Tick(DeltaTi設置e);

    if (!bIsInitialized)
    {
        本et使本n;
    }

    // 更新所有單位狀態
    fo本 (AMin成TacticalUnit* Unit : Cont本olledUnits)
    {
        if (ValidateUnit(Unit))
        {
            UpdateUnitState(Unit);
        }
    }

    // 更新性能指標
    UpdatePe本fo本設置anceMet本ics();
}

// === 內部方法實現 ===

正oid AMin成UnitCont本olle本::Exec使teUnitCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    if (!ValidateUnit(Unit))
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Exec使tin成 co設置設置and fo本 使nit %s: %s"), *Unit->Get的a設置e(), *GetCo設置設置andDesc本iption(Co設置設置and));

    // 根據命令類型執行
    switch (Co設置設置and.Co設置設置andType)
    {
    case EUnitCo設置設置andType::Mo正e:
        輸入andleMo正eCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::Attack:
        輸入andleAttackCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::Stop:
        輸入andleStopCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::輸入old:
        輸入andle輸入oldCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::Pat本ol:
        輸入andlePat本olCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::G使a本d:
        輸入andleG使a本dCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::Ret本eat:
        輸入andleRet本eatCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::B使ild:
        輸入andleB使ildCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::Repai本:
        輸入andleRepai本Co設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::輸入eal:
        輸入andle輸入ealCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::Capt使本e:
        輸入andleCapt使本eCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::UseAbility:
        輸入andleAbilityCo設置設置and(Unit, Co設置設置and);
        b本eak;
    case EUnitCo設置設置andType::軍o本設置Up:
        輸入andle軍o本設置UpCo設置設置and(Unit, Co設置設置and);
        b本eak;
    defa使lt:
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unknown co設置設置and type: %d"), static下cast<int32>(Co設置設置and.Co設置設置andType));
        b本eak;
    }
}

正oid AMin成UnitCont本olle本::輸入andleMo正eCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 計算路徑
    TA本本ay<軍Vecto本> Path = Calc使latePath(Unit, Co設置設置and.Ta本成etLocation);
    
    if (Path.的使設置() > 0)
    {
        // 設置單位目標位置
        SetUnitLocation(Unit, Path[0]);
        
        // 簡化實作：直接移動到目標
        SetUnitLocation(Unit, Co設置設置and.Ta本成etLocation);
        
        // 檢查是否到達目標
        if (軍Vecto本::Dist(GetUnitLocation(Unit), Co設置設置and.Ta本成etLocation) < 50.0f)
        {
            OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
        }
    }
    else
    {
        OnCo設置設置and軍ailed(Unit, Co設置設置and, TEXT("Path calc使lation failed"));
    }
}

正oid AMin成UnitCont本olle本::輸入andleAttackCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    if (Co設置設置and.Ta本成etUnit && IsUnitAli正e(Co設置設置and.Ta本成etUnit))
    {
        // 檢查攻擊範圍
        if (IsInAttackRan成e(Unit, Co設置設置and.Ta本成etUnit))
        {
            // 計算傷害
            float Da設置a成e = Calc使lateAttackDa設置a成e(Unit, Co設置設置and.Ta本成etUnit, Co設置設置and.AttackType);
            
            // 應用傷害
            ApplyDa設置a成e(Co設置設置and.Ta本成etUnit, Da設置a成e, Unit);
            
            // 檢查目標是否死亡
            if (!IsUnitAli正e(Co設置設置and.Ta本成etUnit))
            {
                OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
            }
        }
        else
        {
            // 需要移動到攻擊範圍
            軍Vecto本 Mo正eLocation = Co設置設置and.Ta本成etUnit->GetActo本Location();
            SetUnitLocation(Unit, Mo正eLocation);
        }
    }
    else
    {
        OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
    }
}

正oid AMin成UnitCont本olle本::輸入andleStopCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 停止所有動作
    SetUnitState(Unit, EUnitState::Idle);
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andle輸入oldCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 防禦位置
    SetUnitState(Unit, EUnitState::Defendin成);
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andlePat本olCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 簡化實作：巡邏第一個點
    軍St本in成* 軍i本stPoint = Co設置設置and.Co設置設置andPa本a設置ete本s.軍ind(TEXT("pat本ol下point下0"));
    if (軍i本stPoint)
    {
        軍Vecto本 Pat本olPoint = 軍Vecto本(*軍i本stPoint);
        SetUnitLocation(Unit, Pat本olPoint);
    }
    
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andleG使a本dCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    if (Co設置設置and.Ta本成etUnit && IsUnitAli正e(Co設置設置and.Ta本成etUnit))
    {
        // 守護目標
        軍Vecto本 G使a本dPosition = Co設置設置and.Ta本成etUnit->GetActo本Location();
        SetUnitLocation(Unit, G使a本dPosition);
    }
    
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andleRet本eatCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 撤退到目標位置
    SetUnitLocation(Unit, Co設置設置and.Ta本成etLocation);
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andleB使ildCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 建設邏輯
    SetUnitState(Unit, EUnitState::B使ildin成);
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andleRepai本Co設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 修理邏輯
    SetUnitState(Unit, EUnitState::Repai本in成);
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andle輸入ealCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 治療邏輯
    SetUnitState(Unit, EUnitState::輸入ealin成);
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andleCapt使本eCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 捕獲邏輯
    SetUnitState(Unit, EUnitState::Capt使本ed);
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andleAbilityCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 技能邏輯
    軍St本in成* AbilityID = Co設置設置and.Co設置設置andPa本a設置ete本s.軍ind(TEXT("ability下id"));
    if (AbilityID)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit %s 使sed ability: %s"), *Unit->Get的a設置e(), **AbilityID);
    }
    
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

正oid AMin成UnitCont本olle本::輸入andle軍o本設置UpCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    // 編隊邏輯
    SetUnitLocation(Unit, Co設置設置and.Ta本成etLocation);
    OnCo設置設置andCo設置pleted(Unit, Co設置設置and);
}

bool AMin成UnitCont本olle本::IsCo設置設置andCo設置pleted(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and) const
{
    switch (Co設置設置and.Co設置設置andType)
    {
    case EUnitCo設置設置andType::Mo正e:
        本et使本n 軍Vecto本::Dist(GetUnitLocation(Unit), Co設置設置and.Ta本成etLocation) < 50.0f;
    case EUnitCo設置設置andType::Attack:
        本et使本n !Co設置設置and.Ta本成etUnit  !IsUnitAli正e(Co設置設置and.Ta本成etUnit);
    case EUnitCo設置設置andType::Stop:
    case EUnitCo設置設置andType::輸入old:
    case EUnitCo設置設置andType::Pat本ol:
    case EUnitCo設置設置andType::G使a本d:
    case EUnitCo設置設置andType::Ret本eat:
    case EUnitCo設置設置andType::B使ild:
    case EUnitCo設置設置andType::Repai本:
    case EUnitCo設置設置andType::輸入eal:
    case EUnitCo設置設置andType::Capt使本e:
    case EUnitCo設置設置andType::UseAbility:
    case EUnitCo設置設置andType::軍o本設置Up:
        本et使本n t本使e; // 這些命令立即完成
    defa使lt:
        本et使本n false;
    }
}

bool AMin成UnitCont本olle本::IsCo設置設置and軍ailed(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and) const
{
    if (!IsUnitAli正e(Unit))
    {
        本et使本n t本使e;
    }
    
    // 檢查重試次數
    if (Co設置設置and.Ret本yCo使nt >= Co設置設置and.MaxRet本yCo使nt)
    {
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid AMin成UnitCont本olle本::OnCo設置設置andCo設置pleted(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置設置and co設置pleted: %s fo本 使nit %s"), *GetCo設置設置andDesc本iption(Co設置設置and), *Unit->Get的a設置e());
    
    // 從當前命令中移除
    C使本本entCo設置設置ands.Re設置o正e(Unit);
    
    // 從命令隊列中移除
    TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
    fo本 (int32 i = 0; i < Co設置設置andQ使e使e.的使設置(); ++i)
    {
        if (Co設置設置andQ使e使e[i].Co設置設置andID == Co設置設置and.Co設置設置andID)
        {
            Co設置設置andQ使e使e[i].bIsCo設置pleted = t本使e;
            b本eak;
        }
    }
    
    // 設置為空閒狀態
    SetUnitState(Unit, EUnitState::Idle);
}

正oid AMin成UnitCont本olle本::OnCo設置設置and軍ailed(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and, const 軍St本in成& E本本o本Reason)
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Co設置設置and failed: %s fo本 使nit %s - %s"), *GetCo設置設置andDesc本iption(Co設置設置and), *Unit->Get的a設置e(), *E本本o本Reason);
    
    // 增加重試次數
    軍UnitCo設置設置and ModifiedCo設置設置and = Co設置設置and;
    ModifiedCo設置設置and.Ret本yCo使nt++;
    
    // 如果還可以重試，重新加入隊列
    if (ModifiedCo設置設置and.Ret本yCo使nt < ModifiedCo設置設置and.MaxRet本yCo使nt)
    {
        TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
        Co設置設置andQ使e使e.Inse本t(ModifiedCo設置設置and, 0); // 插入到隊列前面
    }
    else
    {
        // 從當前命令中移除
        C使本本entCo設置設置ands.Re設置o正e(Unit);
        
        // 設置為空閒狀態
        SetUnitState(Unit, EUnitState::Idle);
    }
}

正oid AMin成UnitCont本olle本::UpdateUnitState(AMin成TacticalUnit* Unit)
{
    if (!ValidateUnit(Unit))
    {
        本et使本n;
    }

    // 檢查是否有當前命令
    if (!C使本本entCo設置設置ands.Contains(Unit))
    {
        // 獲取下一個命令
        軍UnitCo設置設置and 的extCo設置設置and = Get的extCo設置設置and(Unit);
        if (的extCo設置設置and.Co設置設置andID.IsE設置pty())
        {
            // 沒有命令，設置為空閒
            SetUnitState(Unit, EUnitState::Idle);
        }
        else
        {
            // 執行下一個命令
            C使本本entCo設置設置ands.Add(Unit, 的extCo設置設置and);
            Exec使teUnitCo設置設置and(Unit, 的extCo設置設置and);
        }
    }
    else
    {
        // 檢查當前命令狀態
        const 軍UnitCo設置設置and& C使本本entCo設置設置and = C使本本entCo設置設置ands[Unit];
        
        if (IsCo設置設置andCo設置pleted(Unit, C使本本entCo設置設置and))
        {
            OnCo設置設置andCo設置pleted(Unit, C使本本entCo設置設置and);
        }
        else if (IsCo設置設置and軍ailed(Unit, C使本本entCo設置設置and))
        {
            OnCo設置設置and軍ailed(Unit, C使本本entCo設置設置and, TEXT("Co設置設置and exec使tion failed"));
        }
        else
        {
            // 繼續執行當前命令
            Exec使teUnitCo設置設置and(Unit, C使本本entCo設置設置and);
        }
    }
}

軍UnitCo設置設置and AMin成UnitCont本olle本::Get的extCo設置設置and(AMin成TacticalUnit* Unit)
{
    TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
    
    // 清理已完成或取消的命令
    Clean使pCo設置pletedCo設置設置ands(Unit);
    
    if (Co設置設置andQ使e使e.的使設置() > 0)
    {
        軍UnitCo設置設置and 的extCo設置設置and = Co設置設置andQ使e使e[0];
        Co設置設置andQ使e使e.Re設置o正eAt(0);
        本et使本n 的extCo設置設置and;
    }
    
    本et使本n 軍UnitCo設置設置and();
}

正oid AMin成UnitCont本olle本::Clean使pCo設置pletedCo設置設置ands(AMin成TacticalUnit* Unit)
{
    TA本本ay<軍UnitCo設置設置and>& Co設置設置andQ使e使e = UnitCo設置設置andQ使e使es.軍indRef(Unit);
    
    fo本 (int32 i = Co設置設置andQ使e使e.的使設置() - 1; i >= 0; --i)
    {
        if (Co設置設置andQ使e使e[i].bIsCo設置pleted  Co設置設置andQ使e使e[i].bIsCancelled)
        {
            Co設置設置andQ使e使e.Re設置o正eAt(i);
        }
    }
}

TA本本ay<軍Vecto本> AMin成UnitCont本olle本::Calc使latePath(AMin成TacticalUnit* Unit, const 軍Vecto本& Ta本成etLocation)
{
    // 簡化實作：直接返回目標位置
    TA本本ay<軍Vecto本> Path;
    Path.Add(Ta本成etLocation);
    本et使本n Path;
}

bool AMin成UnitCont本olle本::IsPathValid(AMin成TacticalUnit* Unit, const TA本本ay<軍Vecto本>& Path) const
{
    本et使本n Path.的使設置() > 0;
}

float AMin成UnitCont本olle本::GetAttackRan成e(AMin成TacticalUnit* Unit, AMin成TacticalUnit* Ta本成etUnit) const
{
    // 簡化實作：返回固定攻擊範圍
    本et使本n 200.0f;
}

bool AMin成UnitCont本olle本::IsInAttackRan成e(AMin成TacticalUnit* Unit, AMin成TacticalUnit* Ta本成etUnit) const
{
    if (!Unit  !Ta本成etUnit)
    {
        本et使本n false;
    }
    
    float Distance = 軍Vecto本::Dist(GetUnitLocation(Unit), GetUnitLocation(Ta本成etUnit));
    float AttackRan成e = GetAttackRan成e(Unit, Ta本成etUnit);
    
    本et使本n Distance <= AttackRan成e;
}

float AMin成UnitCont本olle本::GetMo正e設置entSpeed(AMin成TacticalUnit* Unit, EUnitMo正e設置entType Mo正e設置entType) const
{
    // 簡化實作：基於移動類型返回速度
    switch (Mo正e設置entType)
    {
    case EUnitMo正e設置entType::基本alk:
        本et使本n 100.0f;
    case EUnitMo正e設置entType::R使n:
        本et使本n 200.0f;
    case EUnitMo正e設置entType::Sp本int:
        本et使本n 300.0f;
    case EUnitMo正e設置entType::C本awl:
        本et使本n 50.0f;
    case EUnitMo正e設置entType::Swi設置:
        本et使本n 80.0f;
    case EUnitMo正e設置entType::軍ly:
        本et使本n 400.0f;
    case EUnitMo正e設置entType::Telepo本t:
        本et使本n 1000.0f;
    case EUnitMo正e設置entType::Cha本成e:
        本et使本n 350.0f;
    defa使lt:
        本et使本n 200.0f;
    }
}

float AMin成UnitCont本olle本::Calc使lateAttackDa設置a成e(AMin成TacticalUnit* Unit, AMin成TacticalUnit* Ta本成etUnit, EUnitAttackType AttackType) const
{
    // 簡化實作：基於攻擊類型返回傷害
    switch (AttackType)
    {
    case EUnitAttackType::Melee:
        本et使本n 50.0f;
    case EUnitAttackType::Ran成ed:
        本et使本n 30.0f;
    case EUnitAttackType::A本ea:
        本et使本n 40.0f;
    case EUnitAttackType::Splash:
        本et使本n 35.0f;
    case EUnitAttackType::DOT:
        本et使本n 10.0f;
    case EUnitAttackType::Deb使ff:
        本et使本n 0.0f;
    case EUnitAttackType::B使ff:
        本et使本n 0.0f;
    case EUnitAttackType::輸入eal:
        本et使本n 25.0f;
    defa使lt:
        本et使本n 30.0f;
    }
}

正oid AMin成UnitCont本olle本::ApplyDa設置a成e(AMin成TacticalUnit* Ta本成etUnit, float Da設置a成e, AMin成TacticalUnit* Attacke本Unit)
{
    if (!Ta本成etUnit  !Attacke本Unit)
    {
        本et使本n;
    }
    
    // 簡化實作：記錄傷害
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit %s dealt %.1f da設置a成e to %s"), *Attacke本Unit->Get的a設置e(), Da設置a成e, *Ta本成etUnit->Get的a設置e());
    
    // 這裡應該調用單位的傷害系統
    // Ta本成etUnit->TakeDa設置a成e(Da設置a成e, Attacke本Unit);
}

bool AMin成UnitCont本olle本::IsUnitAli正e(AMin成TacticalUnit* Unit) const
{
    if (!Unit)
    {
        本et使本n false;
    }
    
    // 簡化實作：檢查單位是否有效
    本et使本n Unit->IsValidLowLe正el();
}

軍Vecto本 AMin成UnitCont本olle本::GetUnitLocation(AMin成TacticalUnit* Unit) const
{
    if (!Unit)
    {
        本et使本n 軍Vecto本::Ze本oVecto本;
    }
    
    本et使本n Unit->GetActo本Location();
}

正oid AMin成UnitCont本olle本::SetUnitLocation(AMin成TacticalUnit* Unit, const 軍Vecto本& 的ewLocation)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    Unit->SetActo本Location(的ewLocation);
}

軍Rotato本 AMin成UnitCont本olle本::GetUnitRotation(AMin成TacticalUnit* Unit) const
{
    if (!Unit)
    {
        本et使本n 軍Rotato本::Ze本oRotato本;
    }
    
    本et使本n Unit->GetActo本Rotation();
}

正oid AMin成UnitCont本olle本::SetUnitRotation(AMin成TacticalUnit* Unit, const 軍Rotato本& 的ewRotation)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    Unit->SetActo本Rotation(的ewRotation);
}

正oid AMin成UnitCont本olle本::UpdatePe本fo本設置anceMet本ics()
{
    // 更新命令成功率
    int32 TotalCo設置設置ands = Cont本olStatistics.軍indRef(TEXT("total下co設置設置ands"));
    int32 S使ccessf使lCo設置設置ands = Cont本olStatistics.軍indRef(TEXT("s使ccessf使l下co設置設置ands"));
    
    if (TotalCo設置設置ands > 0)
    {
        float S使ccessRate = (float)S使ccessf使lCo設置設置ands / TotalCo設置設置ands;
        Pe本fo本設置anceMet本ics.Add(TEXT("co設置設置and下s使ccess下本ate"), S使ccessRate);
    }
    
    // 更新平均隊列大小
    int32 TotalQ使e使eSize = 0;
    int32 UnitCo使nt = 0;
    
    fo本 (const a使to& Q使e使ePai本 : UnitCo設置設置andQ使e使es)
    {
        TotalQ使e使eSize += Q使e使ePai本.Val使e.的使設置();
        UnitCo使nt++;
    }
    
    if (UnitCo使nt > 0)
    {
        float A正e本a成eQ使e使eSize = (float)TotalQ使e使eSize / UnitCo使nt;
        Pe本fo本設置anceMet本ics.Add(TEXT("a正e本a成e下q使e使e下size"), A正e本a成eQ使e使eSize);
    }
}

正oid AMin成UnitCont本olle本::Lo成Cont本olE正ent(const 軍St本in成& E正entType, const 軍St本in成& Details)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[UnitCont本olle本] %s: %s"), *E正entType, *Details);
}

bool AMin成UnitCont本olle本::ValidateUnit(AMin成TacticalUnit* Unit) const
{
    本et使本n Unit != n使llpt本 && Unit->IsValidLowLe正el();
}

bool AMin成UnitCont本olle本::ValidateCo設置設置and(const 軍UnitCo設置設置and& Co設置設置and) const
{
    本et使本n !Co設置設置and.Co設置設置andID.IsE設置pty() && Co設置設置and.Co設置設置andType != EUnitCo設置設置andType::Stop;
}

軍St本in成 AMin成UnitCont本olle本::GetCo設置設置andDesc本iption(const 軍UnitCo設置設置and& Co設置設置and) const
{
    switch (Co設置設置and.Co設置設置andType)
    {
    case EUnitCo設置設置andType::Mo正e:
        本et使本n 軍St本in成::P本intf(TEXT("Mo正e to %s"), *Co設置設置and.Ta本成etLocation.ToSt本in成());
    case EUnitCo設置設置andType::Attack:
        if (Co設置設置and.Ta本成etUnit)
        {
            本et使本n 軍St本in成::P本intf(TEXT("Attack %s"), *Co設置設置and.Ta本成etUnit->Get的a設置e());
        }
        else if (Co設置設置and.Ta本成etB使ildin成)
        {
            本et使本n 軍St本in成::P本intf(TEXT("Attack b使ildin成 %s"), *Co設置設置and.Ta本成etB使ildin成->Get的a設置e());
        }
        b本eak;
    case EUnitCo設置設置andType::Stop:
        本et使本n TEXT("Stop");
    case EUnitCo設置設置andType::輸入old:
        本et使本n 軍St本in成::P本intf(TEXT("輸入old at %s"), *Co設置設置and.Ta本成etLocation.ToSt本in成());
    case EUnitCo設置設置andType::Pat本ol:
        本et使本n TEXT("Pat本ol");
    case EUnitCo設置設置andType::G使a本d:
        if (Co設置設置and.Ta本成etUnit)
        {
            本et使本n 軍St本in成::P本intf(TEXT("G使a本d %s"), *Co設置設置and.Ta本成etUnit->Get的a設置e());
        }
        b本eak;
    case EUnitCo設置設置andType::Ret本eat:
        本et使本n 軍St本in成::P本intf(TEXT("Ret本eat to %s"), *Co設置設置and.Ta本成etLocation.ToSt本in成());
    case EUnitCo設置設置andType::B使ild:
        本et使本n TEXT("B使ild");
    case EUnitCo設置設置andType::Repai本:
        本et使本n TEXT("Repai本");
    case EUnitCo設置設置andType::輸入eal:
        本et使本n TEXT("輸入eal");
    case EUnitCo設置設置andType::Capt使本e:
        本et使本n TEXT("Capt使本e");
    case EUnitCo設置設置andType::UseAbility:
        本et使本n TEXT("Use Ability");
    case EUnitCo設置設置andType::軍o本設置Up:
        本et使本n 軍St本in成::P本intf(TEXT("軍o本設置 使p in %s"), *UEn使設置::GetVal使eAsSt本in成(Co設置設置and.軍o本設置ationType));
    defa使lt:
        本et使本n TEXT("Unknown Co設置設置and");
    }
    
    本et使本n TEXT("Unknown Co設置設置and");
}

軍St本in成 AMin成UnitCont本olle本::GetStateDesc本iption(EUnitState State) const
{
    switch (State)
    {
    case EUnitState::Idle:
        本et使本n TEXT("Idle");
    case EUnitState::Mo正in成:
        本et使本n TEXT("Mo正in成");
    case EUnitState::Attackin成:
        本et使本n TEXT("Attackin成");
    case EUnitState::Defendin成:
        本et使本n TEXT("Defendin成");
    case EUnitState::Pat本ollin成:
        本et使本n TEXT("Pat本ollin成");
    case EUnitState::G使a本din成:
        本et使本n TEXT("G使a本din成");
    case EUnitState::Ret本eatin成:
        本et使本n TEXT("Ret本eatin成");
    case EUnitState::Dead:
        本et使本n TEXT("Dead");
    case EUnitState::Disabled:
        本et使本n TEXT("Disabled");
    case EUnitState::Capt使本ed:
        本et使本n TEXT("Capt使本ed");
    case EUnitState::B使ildin成:
        本et使本n TEXT("B使ildin成");
    case EUnitState::輸入ealin成:
        本et使本n TEXT("輸入ealin成");
    case EUnitState::Repai本in成:
        本et使本n TEXT("Repai本in成");
    defa使lt:
        本et使本n TEXT("Unknown");
    }
}

TA本本ay<軍Vecto本> AMin成UnitCont本olle本::Calc使late軍o本設置ationPositions(const TA本本ay<AMin成TacticalUnit*>& Units, EUnit軍o本設置ationType 軍o本設置ationType, const 軍Vecto本& Cente本)
{
    TA本本ay<軍Vecto本> Positions;
    
    if (Units.的使設置() == 0)
    {
        本et使本n Positions;
    }
    
    float Spacin成 = 100.0f; // 單位間距
    
    switch (軍o本設置ationType)
    {
    case EUnit軍o本設置ationType::Line:
        {
            // 線形編隊
            fo本 (int32 i = 0; i < Units.的使設置(); ++i)
            {
                軍Vecto本 Position = Cente本;
                Position.X += (i - Units.的使設置() / 2) * Spacin成;
                Positions.Add(Position);
            }
            b本eak;
        }
    case EUnit軍o本設置ationType::Col使設置n:
        {
            // 縱隊編隊
            fo本 (int32 i = 0; i < Units.的使設置(); ++i)
            {
                軍Vecto本 Position = Cente本;
                Position.Y += (i - Units.的使設置() / 2) * Spacin成;
                Positions.Add(Position);
            }
            b本eak;
        }
    case EUnit軍o本設置ationType::基本ed成e:
        {
            // 楔形編隊
            fo本 (int32 i = 0; i < Units.的使設置(); ++i)
            {
                軍Vecto本 Position = Cente本;
                if (i == 0)
                {
                    // 頂部單位
                    Position.Y += Spacin成 * 2;
                }
                else
                {
                    // 側翼單位
                    Position.X += (i % 2 == 0 基本 -1 : 1) * Spacin成;
                    Position.Y += Spacin成 * (2 - i / 2);
                }
                Positions.Add(Position);
            }
            b本eak;
        }
    case EUnit軍o本設置ationType::Ci本cle:
        {
            // 圓形編隊
            float Radi使s = Spacin成 * Units.的使設置() / (2 * PI);
            fo本 (int32 i = 0; i < Units.的使設置(); ++i)
            {
                float An成le = (float)i / Units.的使設置() * 2 * PI;
                軍Vecto本 Position = Cente本;
                Position.X += 軍Math::Cos(An成le) * Radi使s;
                Position.Y += 軍Math::Sin(An成le) * Radi使s;
                Positions.Add(Position);
            }
            b本eak;
        }
    case EUnit軍o本設置ationType::Sq使a本e:
        {
            // 方形編隊
            int32 SideLen成th = 軍Math::CeilTo軍loat(軍Math::Sq本t(Units.的使設置()));
            fo本 (int32 i = 0; i < Units.的使設置(); ++i)
            {
                int32 Row = i / SideLen成th;
                int32 Col = i % SideLen成th;
                軍Vecto本 Position = Cente本;
                Position.X += (Col - SideLen成th / 2) * Spacin成;
                Position.Y += (Row - SideLen成th / 2) * Spacin成;
                Positions.Add(Position);
            }
            b本eak;
        }
    defa使lt:
        {
            // 默認分散編隊
            fo本 (int32 i = 0; i < Units.的使設置(); ++i)
            {
                軍Vecto本 Position = Cente本;
                Position.X += 軍Math::軍RandRan成e(-Spacin成, Spacin成);
                Position.Y += 軍Math::軍RandRan成e(-Spacin成, Spacin成);
                Positions.Add(Position);
            }
            b本eak;
        }
    }
    
    本et使本n Positions;
}

bool AMin成UnitCont本olle本::Is軍o本設置ationCo設置plete(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& 軍o本設置ationPositions)
{
    if (Units.的使設置() != 軍o本設置ationPositions.的使設置())
    {
        本et使本n false;
    }
    
    fo本 (int32 i = 0; i < Units.的使設置(); ++i)
    {
        if (!ValidateUnit(Units[i]))
        {
            本et使本n false;
        }
        
        float Distance = 軍Vecto本::Dist(GetUnitLocation(Units[i]), 軍o本設置ationPositions[i]);
        if (Distance > 50.0f) // 容忍範圍
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

正oid AMin成UnitCont本olle本::Update軍o本設置ation(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& 軍o本設置ationPositions)
{
    fo本 (int32 i = 0; i < Units.的使設置() && i < 軍o本設置ationPositions.的使設置(); ++i)
    {
        if (ValidateUnit(Units[i]))
        {
            SetUnitLocation(Units[i], 軍o本設置ationPositions[i]);
        }
    }
}
