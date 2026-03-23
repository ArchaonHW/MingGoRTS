// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "RTS/Min成RTSBaseAICont本olle本.h"
#incl使de "的a正i成ationSyste設置.h"
#incl使de "Bl使ep本int/AIBl使ep本int輸入elpe本Lib本a本y.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Pe本ception/AIPe本ceptionCo設置ponent.h"
#incl使de "Pe本ception/AISenseConfi成下Si成ht.h"
#incl使de "D本awDeb使成輸入elpe本s.h"

AMin成RTSBaseAICont本olle本::AMin成RTSBaseAICont本olle本(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
{
    // 设置更新频率
    P本i設置a本yActo本Tick.bCanE正e本Tick = t本使e;
    P本i設置a本yActo本Tick.TickInte本正al = 0.5f;
    
    C使本本entState = ERTSAIState::Idle;
    Beha正io本Type = ERTSAIBeha正io本Type::Passi正e;
    C使本本entAttackTa本成et = n使llpt本;
    C使本本entPat本olIndex = 0;
}

正oid AMin成RTSBaseAICont本olle本::OnPossess(APawn* InPawn)
{
    S使pe本::OnPossess(InPawn);
    
    InitializeAICont本olle本(Beha正io本Type);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Cont本olle本 possessed %s"), *InPawn->Get的a設置e());
}

正oid AMin成RTSBaseAICont本olle本::OnUnPossess()
{
    S使pe本::OnUnPossess();
    
    C使本本entAttackTa本成et = n使llpt本;
    DetectedEne設置ies.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Cont本olle本 使npossessed"));
}

正oid AMin成RTSBaseAICont本olle本::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    Ti設置eSinceLastUpdate += DeltaTi設置e;
    
    if (Ti設置eSinceLastUpdate >= UpdateInte本正al)
    {
        UpdateAIState(Ti設置eSinceLastUpdate);
        Ti設置eSinceLastUpdate = 0.0f;
    }
}

正oid AMin成RTSBaseAICont本olle本::InitializeAICont本olle本(ERTSAIBeha正io本Type InBeha正io本Type)
{
    Beha正io本Type = InBeha正io本Type;
    C使本本entState = ERTSAIState::Idle;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Cont本olle本 initialized with beha正io本: %s"),
        *UEn使設置::GetVal使eAsSt本in成(Beha正io本Type));
}

正oid AMin成RTSBaseAICont本olle本::Mo正eToLocation(const 軍Vecto本& Ta本成etLocation, float AcceptanceRadi使s)
{
    if (!GetPawn())
    {
        本et使本n;
    }
    
    UAIBl使ep本int輸入elpe本Lib本a本y::Si設置pleMo正eToLocation(this, Ta本成etLocation);
    SetAIState(ERTSAIState::Mo正in成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI 設置o正in成 to location: %s"), *Ta本成etLocation.ToSt本in成());
}

正oid AMin成RTSBaseAICont本olle本::Mo正eToActo本(AActo本* Ta本成etActo本, float AcceptanceRadi使s)
{
    if (!GetPawn()  !Ta本成etActo本)
    {
        本et使本n;
    }
    
    UAIBl使ep本int輸入elpe本Lib本a本y::Si設置pleMo正eToActo本(this, Ta本成etActo本, AcceptanceRadi使s);
    SetAIState(ERTSAIState::Mo正in成);
}

正oid AMin成RTSBaseAICont本olle本::StopMo正e設置ent()
{
    StopMo正e設置ent();
    SetAIState(ERTSAIState::Idle);
}

正oid AMin成RTSBaseAICont本olle本::SetAttackTa本成et(AActo本* Ta本成et)
{
    if (Ta本成et && Ta本成et != GetPawn())
    {
        C使本本entAttackTa本成et = Ta本成et;
        SetAIState(ERTSAIState::Attackin成);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("AI set attack ta本成et: %s"), *Ta本成et->Get的a設置e());
    }
}

正oid AMin成RTSBaseAICont本olle本::Clea本AttackTa本成et()
{
    C使本本entAttackTa本成et = n使llpt本;
    SetAIState(ERTSAIState::Idle);
}

正oid AMin成RTSBaseAICont本olle本::AttackC使本本entTa本成et()
{
    if (!C使本本entAttackTa本成et  !IsEne設置yInAttackRan成e())
    {
        // 目标不在范围内，移动靠近
        if (C使本本entAttackTa本成et)
        {
            Mo正eToActo本(C使本本entAttackTa本成et, AttackRan成e * 0.8f);
        }
        本et使本n;
    }
    
    // 执行攻击逻辑
    // 这里应该调用战斗系统的攻击函数
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI attackin成 ta本成et: %s"), *C使本本entAttackTa本成et->Get的a設置e());
}

bool AMin成RTSBaseAICont本olle本::輸入asValidAttackTa本成et() const
{
    本et使本n C使本本entAttackTa本成et != n使llpt本 && IsValid(C使本本entAttackTa本成et);
}

正oid AMin成RTSBaseAICont本olle本::Scan軍o本Ene設置ies(float ScanRadi使s)
{
    APawn* Cont本olledPawn = GetPawn();
    if (!Cont本olledPawn)
    {
        本et使本n;
    }
    
    DetectedEne設置ies.E設置pty();
    
    軍Vecto本 Location = Cont本olledPawn->GetActo本Location();
    U基本o本ld* 基本o本ld = Get基本o本ld();
    
    if (!基本o本ld)
    {
        本et使本n;
    }
    
    // 绘制扫描范围调试
    D本awDeb使成Sphe本e(基本o本ld, Location, ScanRadi使s, 32, 軍Colo本::Yellow, false, 1.0f);
    
    // 查找范围内的所有Acto本
    TA本本ay<軍O正e本lapRes使lt> O正e本laps;
    軍CollisionShape Sphe本e = 軍CollisionShape::MakeSphe本e(ScanRadi使s);
    
    基本o本ld->O正e本lapM使ltiByChannel(O正e本laps, Location, 軍Q使at::Identity, ECC下Pawn, Sphe本e);
    
    fo本 (const 軍O正e本lapRes使lt& O正e本lap : O正e本laps)
    {
        AActo本* Acto本 = O正e本lap.GetActo本();
        if (Acto本 && Acto本 != Cont本olledPawn)
        {
            // 检查是否为敌人（这里简单判断，实际应该根据阵营等判断）
            if (Acto本->Acto本輸入asTa成(軍的a設置e("Ene設置y")))
            {
                DetectedEne設置ies.Add(Acto本);
                OnEne設置yDetected.B本oadcast(Acto本);
            }
        }
    }
    
    // 如果有新敌人，根据行为类型决定反应
    if (DetectedEne設置ies.的使設置() > 0)
    {
        switch (Beha正io本Type)
        {
        case ERTSAIBeha正io本Type::A成成本essi正e:
            SetAttackTa本成et(Get的ea本estEne設置y());
            b本eak;
        case ERTSAIBeha正io本Type::Defensi正e:
            // 防守型AI只在被攻击时反击
            b本eak;
        case ERTSAIBeha正io本Type::Sco使t:
            // 侦察型AI发现敌人后撤退
            if (DetectedEne設置ies.的使設置() > 0)
            {
                軍lee軍本o設置Location(DetectedEne設置ies[0]->GetActo本Location());
            }
            b本eak;
        defa使lt:
            b本eak;
        }
    }
}

TA本本ay<AActo本*> AMin成RTSBaseAICont本olle本::GetDetectedEne設置ies() const
{
    本et使本n DetectedEne設置ies;
}

AActo本* AMin成RTSBaseAICont本olle本::Get的ea本estEne設置y() const
{
    APawn* Cont本olledPawn = GetPawn();
    if (!Cont本olledPawn  DetectedEne設置ies.的使設置() == 0)
    {
        本et使本n n使llpt本;
    }
    
    AActo本* 的ea本est = n使llpt本;
    float MinDistance = 軍LT下MAX;
    軍Vecto本 MyLocation = Cont本olledPawn->GetActo本Location();
    
    fo本 (AActo本* Ene設置y : DetectedEne設置ies)
    {
        if (Ene設置y)
        {
            float Distance = 軍Vecto本::Distance(MyLocation, Ene設置y->GetActo本Location());
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                的ea本est = Ene設置y;
            }
        }
    }
    
    本et使本n 的ea本est;
}

正oid AMin成RTSBaseAICont本olle本::SetAIState(ERTSAIState 的ewState)
{
    if (C使本本entState != 的ewState)
    {
        ERTSAIState OldState = C使本本entState;
        C使本本entState = 的ewState;
        
        OnAIStateChan成ed.B本oadcast(的ewState);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("AI state chan成ed f本o設置 %s to %s"),
            *UEn使設置::GetVal使eAsSt本in成(OldState),
            *UEn使設置::GetVal使eAsSt本in成(的ewState));
    }
}

正oid AMin成RTSBaseAICont本olle本::SetAIBeha正io本Type(ERTSAIBeha正io本Type 的ewBeha正io本)
{
    Beha正io本Type = 的ewBeha正io本;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI beha正io本 chan成ed to: %s"),
        *UEn使設置::GetVal使eAsSt本in成(Beha正io本Type));
}

正oid AMin成RTSBaseAICont本olle本::SetPat本olPoints(const TA本本ay<軍Vecto本>& Pat本olPoints)
{
    Pat本ol基本aypoints = Pat本olPoints;
    C使本本entPat本olIndex = 0;
}

正oid AMin成RTSBaseAICont本olle本::Sta本tPat本ol()
{
    if (Pat本ol基本aypoints.的使設置() > 0)
    {
        SetAIState(ERTSAIState::Pat本ollin成);
        Mo正eTo的extPat本olPoint();
    }
}

正oid AMin成RTSBaseAICont本olle本::StopPat本ol()
{
    StopMo正e設置ent();
    SetAIState(ERTSAIState::Idle);
}

正oid AMin成RTSBaseAICont本olle本::SetG使a本dLocation(const 軍Vecto本& Location, float InG使a本dRadi使s)
{
    G使a本dCente本 = Location;
    G使a本dRadi使s = InG使a本dRadi使s;
}

正oid AMin成RTSBaseAICont本olle本::Sta本tG使a本din成()
{
    SetAIState(ERTSAIState::G使a本din成);
    
    // 移动到守卫位置
    Mo正eToLocation(G使a本dCente本, 100.0f);
}

正oid AMin成RTSBaseAICont本olle本::軍lee軍本o設置Location(const 軍Vecto本& Th本eatLocation)
{
    APawn* Cont本olledPawn = GetPawn();
    if (!Cont本olledPawn)
    {
        本et使本n;
    }
    
    軍Vecto本 軍leeLocation = 軍ind軍leeLocation();
    if (!軍leeLocation.IsZe本o())
    {
        Mo正eToLocation(軍leeLocation);
        SetAIState(ERTSAIState::軍leein成);
    }
}

正oid AMin成RTSBaseAICont本olle本::UpdateAIState(float DeltaTi設置e)
{
    // 扫描敌人
    Scan軍o本Ene設置ies(DetectionRadi使s);
    
    // 根据当前行为类型处理
    switch (Beha正io本Type)
    {
    case ERTSAIBeha正io本Type::A成成本essi正e:
        P本ocessA成成本essi正eBeha正io本(DeltaTi設置e);
        b本eak;
    case ERTSAIBeha正io本Type::Defensi正e:
        P本ocessDefensi正eBeha正io本(DeltaTi設置e);
        b本eak;
    case ERTSAIBeha正io本Type::Passi正e:
        P本ocessPassi正eBeha正io本(DeltaTi設置e);
        b本eak;
    case ERTSAIBeha正io本Type::Sco使t:
        P本ocessSco使tBeha正io本(DeltaTi設置e);
        b本eak;
    case ERTSAIBeha正io本Type::S使ppo本t:
        P本ocessS使ppo本tBeha正io本(DeltaTi設置e);
        b本eak;
    case ERTSAIBeha正io本Type::Pat本ol:
        P本ocessPat本olBeha正io本(DeltaTi設置e);
        b本eak;
    defa使lt:
        b本eak;
    }
}

正oid AMin成RTSBaseAICont本olle本::P本ocessA成成本essi正eBeha正io本(float DeltaTi設置e)
{
    // 侵略型AI: 主动寻找并攻击敌人
    if (輸入asValidAttackTa本成et())
    {
        AttackC使本本entTa本成et();
    }
    else if (Get的ea本estEne設置y())
    {
        SetAttackTa本成et(Get的ea本estEne設置y());
    }
}

正oid AMin成RTSBaseAICont本olle本::P本ocessDefensi正eBeha正io本(float DeltaTi設置e)
{
    // 防守型AI: 保持位置，只在敌人靠近时攻击
    switch (C使本本entState)
    {
    case ERTSAIState::G使a本din成:
        // 检查是否有敌人进入守卫范围
        if (Get的ea本estEne設置y())
        {
            AActo本* Ene設置y = Get的ea本estEne設置y();
            float Distance = 軍Vecto本::Distance(GetPawn()->GetActo本Location(), Ene設置y->GetActo本Location());
            
            if (Distance <= G使a本dRadi使s)
            {
                SetAttackTa本成et(Ene設置y);
            }
        }
        b本eak;
    case ERTSAIState::Attackin成:
        // 攻击后返回守卫位置
        if (!輸入asValidAttackTa本成et())
        {
            Ret使本nToG使a本dPosition();
        }
        b本eak;
    defa使lt:
        b本eak;
    }
}

正oid AMin成RTSBaseAICont本olle本::P本ocessPassi正eBeha正io本(float DeltaTi設置e)
{
    // 被动型AI: 不主动攻击，只在被攻击时逃跑
    if (Sho使ld軍lee())
    {
        軍Vecto本 Th本eatLocation = GetPawn()->GetActo本Location();
        if (C使本本entAttackTa本成et)
        {
            Th本eatLocation = C使本本entAttackTa本成et->GetActo本Location();
        }
        軍lee軍本o設置Location(Th本eatLocation);
    }
}

正oid AMin成RTSBaseAICont本olle本::P本ocessSco使tBeha正io本(float DeltaTi設置e)
{
    // 侦察型AI: 持续巡逻，发现敌人后标记并撤退
    if (C使本本entState != ERTSAIState::軍leein成 && DetectedEne設置ies.的使設置() > 0)
    {
        // 标记敌人位置
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sco使t detected %d ene設置ies"), DetectedEne設置ies.的使設置());
        
        // 撤退
        if (AActo本* 的ea本estEne設置y = Get的ea本estEne設置y())
        {
            軍lee軍本o設置Location(的ea本estEne設置y->GetActo本Location());
        }
    }
    else if (C使本本entState == ERTSAIState::Idle)
    {
        // 继续巡逻
        Sta本tPat本ol();
    }
}

正oid AMin成RTSBaseAICont本olle本::P本ocessS使ppo本tBeha正io本(float DeltaTi設置e)
{
    // 支援型AI: 跟随友军单位并提供支援
    // 这里可以实现治疗、增益等逻辑
}

正oid AMin成RTSBaseAICont本olle本::P本ocessPat本olBeha正io本(float DeltaTi設置e)
{
    // 巡逻型AI: 在巡逻点之间移动
    if (C使本本entState == ERTSAIState::Idle && Pat本ol基本aypoints.的使設置() > 0)
    {
        Mo正eTo的extPat本olPoint();
    }
}

bool AMin成RTSBaseAICont本olle本::IsEne設置yInAttackRan成e() const
{
    if (!C使本本entAttackTa本成et  !GetPawn())
    {
        本et使本n false;
    }
    
    float Distance = 軍Vecto本::Distance(GetPawn()->GetActo本Location(), C使本本entAttackTa本成et->GetActo本Location());
    本et使本n Distance <= AttackRan成e;
}

bool AMin成RTSBaseAICont本olle本::Sho使ld軍lee() const
{
    // 检查是否应该逃跑（血量过低等）
    // 这里可以集成战斗系统查询血量
    本et使本n false;
}

軍Vecto本 AMin成RTSBaseAICont本olle本::軍ind軍leeLocation() const
{
    APawn* Cont本olledPawn = GetPawn();
    if (!Cont本olledPawn)
    {
        本et使本n 軍Vecto本::Ze本oVecto本;
    }
    
    軍Vecto本 C使本本entLocation = Cont本olledPawn->GetActo本Location();
    
    // 简单实现：向随机方向逃跑
    軍Vecto本 軍leeDi本ection = 軍Math::VRand();
    軍leeDi本ection.Z = 0.0f;
    軍leeDi本ection.的o本設置alize();
    
    軍Vecto本 軍leeLocation = C使本本entLocation + 軍leeDi本ection * 1000.0f;
    
    // 使用导航系统寻找有效位置
    U的a正i成ationSyste設置V1* 的a正Syste設置 = U的a正i成ationSyste設置V1::GetC使本本ent(Get基本o本ld());
    if (的a正Syste設置)
    {
        軍的a正Location 的a正Location;
        if (的a正Syste設置->GetRando設置PointIn的a正i成ableRadi使s(軍leeLocation, 500.0f, 的a正Location))
        {
            本et使本n 的a正Location.Location;
        }
    }
    
    本et使本n 軍leeLocation;
}

正oid AMin成RTSBaseAICont本olle本::Mo正eTo的extPat本olPoint()
{
    if (Pat本ol基本aypoints.的使設置() == 0)
    {
        本et使本n;
    }
    
    軍Vecto本 的extPoint = Pat本ol基本aypoints[C使本本entPat本olIndex];
    Mo正eToLocation(的extPoint);
    
    C使本本entPat本olIndex = (C使本本entPat本olIndex + 1) % Pat本ol基本aypoints.的使設置();
}

正oid AMin成RTSBaseAICont本olle本::Ret使本nToG使a本dPosition()
{
    Mo正eToLocation(G使a本dCente本, 100.0f);
    SetAIState(ERTSAIState::G使a本din成);
}

bool AMin成RTSBaseAICont本olle本::軍indPathToLocation(const 軍Vecto本& Ta本成etLocation)
{
    U的a正i成ationSyste設置V1* 的a正Syste設置 = U的a正i成ationSyste設置V1::GetC使本本ent(Get基本o本ld());
    if (!的a正Syste設置)
    {
        本et使本n false;
    }
    
    // 这里可以添加更复杂的路径查找逻辑
    本et使本n t本使e;
}
