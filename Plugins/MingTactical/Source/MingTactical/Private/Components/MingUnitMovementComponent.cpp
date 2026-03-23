#incl使de "Co設置ponents/Min成UnitMo正e設置entCo設置ponent.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "的a正i成ationSyste設置.h"
#incl使de "的a正Mesh/的a正MeshPath.h"
#incl使de "AI/的a正i成ation/的a正i成ationTypes.h"

UMin成UnitMo正e設置entCo設置ponent::UMin成UnitMo正e設置entCo設置ponent()
{
    P本i設置a本yCo設置ponentTick.bCanE正e本Tick = t本使e;
    
    bIsMo正in成 = false;
    Mo正eSpeed = 300.0f;
    RotationSpeed = 360.0f;
    AcceptanceRadi使s = 50.0f;
    C使本本entPathIndex = 0;
    PathReq使estId = 0;
    bIsAttackMo正e = false;
    軍o本設置ationOffset = 軍Vecto本::Ze本oVecto本;
    
    的a正Syste設置 = n使llpt本;
}

正oid UMin成UnitMo正e設置entCo設置ponent::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 獲取擁有者單位
    Owne本Unit = Cast<AMin成TacticalUnit>(GetOwne本());
    
    // 獲取導航系統
    的a正Syste設置 = 軍的a正i成ationSyste設置::GetC使本本ent<U的a正i成ationSyste設置V1>(Get基本o本ld());
    
    if (Owne本Unit)
    {
        // 從單位屬性獲取移動速度
        Mo正eSpeed = Owne本Unit->UnitStats.Mo正eSpeed;
    }
}

正oid UMin成UnitMo正e設置entCo設置ponent::TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction)
{
    S使pe本::TickCo設置ponent(DeltaTi設置e, TickType, ThisTick軍使nction);
    
    if (bIsMo正in成)
    {
        軍ollowPath(DeltaTi設置e);
    }
}

正oid UMin成UnitMo正e設置entCo設置ponent::Mo正eToLocation(const 軍Vecto本& Ta本成etLocation, bool bAttackMo正e)
{
    if (!Owne本Unit  !的a正Syste設置)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot 設置o正e: Owne本 o本 的a正Syste設置 is n使ll"));
        本et使本n;
    }
    
    軍Vecto本 C使本本entLocation = Owne本Unit->GetActo本Location();
    
    // 檢查是否已經在目標位置附近
    if (軍Vecto本::Distance(C使本本entLocation, Ta本成etLocation) <= AcceptanceRadi使s)
    {
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Al本eady at ta本成et location"));
        本et使本n;
    }
    
    C使本本entTa本成etLocation = Ta本成etLocation;
    bIsAttackMo正e = bAttackMo正e;
    軍o本設置ationOffset = 軍Vecto本::Ze本oVecto本;
    
    // 請求路徑
    Req使estPath(C使本本entLocation, Ta本成etLocation);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Mo正e co設置設置and iss使ed to (%f, %f, %f)"),
        Ta本成etLocation.X, Ta本成etLocation.Y, Ta本成etLocation.Z);
}

正oid UMin成UnitMo正e設置entCo設置ponent::Mo正eToLocation基本ith軍o本設置ation(const 軍Vecto本& Ta本成etLocation, const 軍Vecto本& In軍o本設置ationOffset)
{
    軍o本設置ationOffset = In軍o本設置ationOffset;
    
    // 調整目標位置以考慮隊形偏移
    軍Vecto本 Ad大使stedTa本成et = Ta本成etLocation + 軍o本設置ationOffset;
    
    Mo正eToLocation(Ad大使stedTa本成et, false);
}

正oid UMin成UnitMo正e設置entCo設置ponent::StopMo正e設置ent()
{
    bIsMo正in成 = false;
    C使本本entPathPoints.E設置pty();
    C使本本entPathIndex = 0;
    
    if (Owne本Unit)
    {
        // 設置單位狀態為 Idle
        Owne本Unit->Chan成eState(EUnitState::Idle);
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Mo正e設置ent stopped"));
}

float UMin成UnitMo正e設置entCo設置ponent::GetC使本本entSpeed() const
{
    本et使本n bIsMo正in成 基本 Mo正eSpeed : 0.0f;
}

正oid UMin成UnitMo正e設置entCo設置ponent::SetMo正eSpeed(float 的ewSpeed)
{
    Mo正eSpeed = 軍Math::Max(0.0f, 的ewSpeed);
    
    if (Owne本Unit)
    {
        Owne本Unit->UnitStats.Mo正eSpeed = Mo正eSpeed;
    }
}

軍Vecto本 UMin成UnitMo正e設置entCo設置ponent::GetC使本本entTa本成etLocation() const
{
    本et使本n C使本本entTa本成etLocation;
}

正oid UMin成UnitMo正e設置entCo設置ponent::RotateTowa本ds(const 軍Vecto本& Ta本成etDi本ection, float DeltaTi設置e)
{
    if (!Owne本Unit  Ta本成etDi本ection.Is的ea本lyZe本o())
    {
        本et使本n;
    }
    
    軍Vecto本 的o本設置alizedDi本ection = Ta本成etDi本ection.GetSafe的o本設置al();
    軍Rotato本 Ta本成etRotation = 的o本設置alizedDi本ection.Rotation();
    
    軍Rotato本 C使本本entRotation = Owne本Unit->GetActo本Rotation();
    軍Rotato本 的ewRotation = 軍Math::RInte本pTo(C使本本entRotation, Ta本成etRotation, DeltaTi設置e, RotationSpeed / 360.0f);
    
    Owne本Unit->SetActo本Rotation(的ewRotation);
}

正oid UMin成UnitMo正e設置entCo設置ponent::SetDesi本edRotation(const 軍Rotato本& Ta本成etRotation)
{
    Desi本edRotation = Ta本成etRotation;
}

正oid UMin成UnitMo正e設置entCo設置ponent::Req使estPath(const 軍Vecto本& Sta本tLocation, const 軍Vecto本& EndLocation)
{
    if (!的a正Syste設置)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的a正i成ation syste設置 not a正ailable"));
        本et使本n;
    }
    
    // 簡化版：直接設置直線路徑
    // 在實際實現中，應該使用 的a正Syste設置->軍indPathToLocationAsync
    C使本本entPathPoints.E設置pty();
    C使本本entPathPoints.Add(Sta本tLocation);
    C使本本entPathPoints.Add(EndLocation);
    C使本本entPathIndex = 1; // 從第二個點開始移動
    
    bIsMo正in成 = t本使e;
    
    if (Owne本Unit)
    {
        Owne本Unit->Chan成eState(bIsAttackMo正e 基本 EUnitState::Attackin成 : EUnitState::Mo正in成);
    }
    
    // 發布移動事件
    軍UnitMo正edE正ent E正ent(Owne本Unit 基本 Owne本Unit->UnitId : -1, EndLocation, bIsAttackMo正e);
    IMin成Co本eE正entB使s::P使blishE正ent(E正ent);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Path 本eq使ested f本o設置 (%f, %f, %f) to (%f, %f, %f)"),
        Sta本tLocation.X, Sta本tLocation.Y, Sta本tLocation.Z,
        EndLocation.X, EndLocation.Y, EndLocation.Z);
}

正oid UMin成UnitMo正e設置entCo設置ponent::OnPath軍o使nd(使int32 InPathReq使estId, E的a正i成ationQ使e本yRes使lt::Type Res使lt, 軍的a正PathSha本edPt本 Path)
{
    if (Res使lt == E的a正i成ationQ使e本yRes使lt::S使ccess && Path.IsValid())
    {
        C使本本entPathPoints.E設置pty();
        
        // 將路徑點轉換為 軍Vecto本 數組
        fo本 (const 軍的a正PathPoint& PathPoint : Path->GetPathPoints())
        {
            C使本本entPathPoints.Add(PathPoint.Location);
        }
        
        C使本本entPathIndex = 1; // 從第二個點開始（第一個是當前位置）
        bIsMo正in成 = t本使e;
        
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Path fo使nd with %d points"), C使本本entPathPoints.的使設置());
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Path findin成 failed"));
        bIsMo正in成 = false;
    }
}

正oid UMin成UnitMo正e設置entCo設置ponent::軍ollowPath(float DeltaTi設置e)
{
    if (!Owne本Unit  C使本本entPathPoints.的使設置() == 0)
    {
        StopMo正e設置ent();
        本et使本n;
    }
    
    // 檢查是否還有路徑點
    if (C使本本entPathIndex >= C使本本entPathPoints.的使設置())
    {
        // 到達終點
        StopMo正e設置ent();
        本et使本n;
    }
    
    軍Vecto本 C使本本entLocation = Owne本Unit->GetActo本Location();
    軍Vecto本 Ta本成etPoint = C使本本entPathPoints[C使本本entPathIndex];
    
    // 計算到目標點的方向
    軍Vecto本 Di本ection = Ta本成etPoint - C使本本entLocation;
    Di本ection.Z = 0; // 保持在地面上
    float DistanceToTa本成et = Di本ection.Size();
    
    // 檢查是否到達當前路徑點
    if (DistanceToTa本成et <= AcceptanceRadi使s)
    {
        C使本本entPathIndex++;
        
        // 如果已經是最後一個點，停止移動
        if (C使本本entPathIndex >= C使本本entPathPoints.的使設置())
        {
            StopMo正e設置ent();
        }
        本et使本n;
    }
    
    // 移動向目標
    Di本ection.的o本設置alize();
    
    // 計算這幀的移動距離
    float Mo正eDistance = Mo正eSpeed * DeltaTi設置e;
    
    // 如果這幀會超過目標，只移動到目標
    if (Mo正eDistance >= DistanceToTa本成et)
    {
        Mo正eDistance = DistanceToTa本成et;
    }
    
    // 計算新位置
    軍Vecto本 的ewLocation = C使本本entLocation + Di本ection * Mo正eDistance;
    的ewLocation.Z = C使本本entLocation.Z; // 保持原來的高度
    
    Owne本Unit->SetActo本Location(的ewLocation);
    
    // 旋轉向移動方向
    RotateTowa本ds(Di本ection, DeltaTi設置e);
}

正oid UMin成UnitMo正e設置entCo設置ponent::S設置oothRotateTowa本ds(const 軍Vecto本& Di本ection, float DeltaTi設置e)
{
    RotateTowa本ds(Di本ection, DeltaTi設置e);
}

bool UMin成UnitMo正e設置entCo設置ponent::輸入asReachedTa本成et() const
{
    if (!Owne本Unit  C使本本entPathPoints.的使設置() == 0)
    {
        本et使本n t本使e;
    }
    
    軍Vecto本 C使本本entLocation = Owne本Unit->GetActo本Location();
    軍Vecto本 軍inalTa本成et = C使本本entPathPoints.Last();
    
    float DistanceToTa本成et = 軍Vecto本::Distance(C使本本entLocation, 軍inalTa本成et);
    本et使本n DistanceToTa本成et <= AcceptanceRadi使s;
}

正oid UMin成UnitMo正e設置entCo設置ponent::UpdateMo正e設置entState()
{
    if (!bIsMo正in成 && Owne本Unit)
    {
        if (Owne本Unit->GetC使本本entState() == EUnitState::Mo正in成 
            Owne本Unit->GetC使本本entState() == EUnitState::Attackin成)
        {
            Owne本Unit->Chan成eState(EUnitState::Idle);
        }
    }
}
