#include "Spectator/MingRTSSpectatorSystem.h"
#include "Misc/DateTime.h"

UMingRTSSpectatorSystem::UMingRTSSpectatorSystem()
{
}

void UMingRTSSpectatorSystem::InitializeSpectatorSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Spectator System Initialized"));
}

FString UMingRTSSpectatorSystem::CreateSpectatorRoom(const FString& MatchID, const FString& RoomName, 
    int32 MaxSpectators, bool bPasswordProtected, const FString& Password)
{
    FString RoomID = GenerateRoomID();
    
    FSpectatorRoom Room;
    Room.RoomID = RoomID;
    Room.MatchID = MatchID;
    Room.RoomName = RoomName.IsEmpty() ? FString::Printf(TEXT("觀戰房間_%s"), *RoomID) : RoomName;
    Room.MaxSpectators = FMath::Clamp(MaxSpectators, 1, 500);
    Room.bIsPasswordProtected = bPasswordProtected;
    Room.Password = bPasswordProtected ? Password : TEXT("");
    
    ActiveRooms.Add(RoomID, Room);
    
    // 默認啟用觀戰
    SpectatingEnabledMatches.Add(MatchID);
    
    UE_LOG(LogTemp, Log, TEXT("Spectator room created: %s for match %s"), *RoomID, *MatchID);
    return RoomID;
}

void UMingRTSSpectatorSystem::CloseSpectatorRoom(const FString& RoomID)
{
    FSpectatorRoom* Room = ActiveRooms.Find(RoomID);
    if (Room)
    {
        // 通知所有觀戰者房間關閉
        BroadcastToRoom(RoomID, TEXT("房間已關閉"));
        
        // 清理觀戰者
        for (const FSpectatorInfo& Spectator : Room->Spectators)
        {
            ActiveSpectators.Remove(Spectator.SpectatorID);
        }
        
        ActiveRooms.Remove(RoomID);
        UE_LOG(LogTemp, Log, TEXT("Spectator room closed: %s"), *RoomID);
    }
}

bool UMingRTSSpectatorSystem::JoinSpectatorRoom(const FString& RoomID, const FSpectatorInfo& Spectator, const FString& Password)
{
    FSpectatorRoom* Room = ActiveRooms.Find(RoomID);
    if (!Room)
    {
        UE_LOG(LogTemp, Warning, TEXT("Room not found: %s"), *RoomID);
        return false;
    }
    
    // 檢查房間是否已滿
    if (Room->CurrentSpectators >= Room->MaxSpectators)
    {
        UE_LOG(LogTemp, Warning, TEXT("Room is full: %s"), *RoomID);
        return false;
    }
    
    // 檢查密碼
    if (Room->bIsPasswordProtected && Room->Password != Password)
    {
        UE_LOG(LogTemp, Warning, TEXT("Incorrect password for room: %s"), *RoomID);
        return false;
    }
    
    // 檢查是否已經在房間中
    for (const FSpectatorInfo& Existing : Room->Spectators)
    {
        if (Existing.SpectatorID == Spectator.SpectatorID)
        {
            UE_LOG(LogTemp, Warning, TEXT("Spectator already in room: %s"), *Spectator.SpectatorID);
            return false;
        }
    }
    
    // 添加觀戰者
    FSpectatorInfo NewSpectator = Spectator;
    NewSpectator.JoinTime = FDateTime::Now().ToString();
    Room->Spectators.Add(NewSpectator);
    Room->CurrentSpectators++;
    
    ActiveSpectators.Add(NewSpectator.SpectatorID, NewSpectator);
    
    OnSpectatorJoined.Broadcast(RoomID, NewSpectator);
    
    UE_LOG(LogTemp, Log, TEXT("Spectator %s joined room %s"), *Spectator.SpectatorID, *RoomID);
    return true;
}

void UMingRTSSpectatorSystem::LeaveSpectatorRoom(const FString& RoomID, const FString& SpectatorID)
{
    FSpectatorRoom* Room = ActiveRooms.Find(RoomID);
    if (Room)
    {
        Room->Spectators.RemoveAll([&SpectatorID](const FSpectatorInfo& Info) {
            return Info.SpectatorID == SpectatorID;
        });
        Room->CurrentSpectators = FMath::Max(0, Room->CurrentSpectators - 1);
        
        ActiveSpectators.Remove(SpectatorID);
        
        OnSpectatorLeft.Broadcast(RoomID, SpectatorID);
        
        UE_LOG(LogTemp, Log, TEXT("Spectator %s left room %s"), *SpectatorID, *RoomID);
    }
}

TArray<FSpectatorRoom> UMingRTSSpectatorSystem::GetAvailableRooms() const
{
    TArray<FSpectatorRoom> Result;
    for (const auto& Pair : ActiveRooms)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

bool UMingRTSSpectatorSystem::GetRoomInfo(const FString& RoomID, FSpectatorRoom& OutRoom) const
{
    const FSpectatorRoom* Room = ActiveRooms.Find(RoomID);
    if (Room)
    {
        OutRoom = *Room;
        return true;
    }
    return false;
}

void UMingRTSSpectatorSystem::SetSpectatorMode(const FString& SpectatorID, ESpectatorMode Mode)
{
    FSpectatorInfo* Spectator = ActiveSpectators.Find(SpectatorID);
    if (Spectator)
    {
        Spectator->CurrentMode = Mode;
        
        // 更新房間中的觀戰者信息
        for (auto& RoomPair : ActiveRooms)
        {
            for (FSpectatorInfo& RoomSpectator : RoomPair.Value.Spectators)
            {
                if (RoomSpectator.SpectatorID == SpectatorID)
                {
                    RoomSpectator.CurrentMode = Mode;
                    break;
                }
            }
        }
        
        OnSpectatorModeChanged.Broadcast(SpectatorID, Mode);
        UE_LOG(LogTemp, Log, TEXT("Spectator %s mode changed to %d"), *SpectatorID, (int32)Mode);
    }
}

void UMingRTSSpectatorSystem::FollowPlayer(const FString& SpectatorID, const FString& TargetPlayerID)
{
    FSpectatorInfo* Spectator = ActiveSpectators.Find(SpectatorID);
    if (Spectator)
    {
        Spectator->TargetPlayerID = TargetPlayerID;
        Spectator->TargetUnitID = -1;
        Spectator->CurrentMode = ESpectatorMode::FollowPlayer;
        
        UpdateSpectatorInfo(SpectatorID, *Spectator);
        OnSpectatorModeChanged.Broadcast(SpectatorID, ESpectatorMode::FollowPlayer);
    }
}

void UMingRTSSpectatorSystem::FollowUnit(const FString& SpectatorID, int32 TargetUnitID)
{
    FSpectatorInfo* Spectator = ActiveSpectators.Find(SpectatorID);
    if (Spectator)
    {
        Spectator->TargetUnitID = TargetUnitID;
        Spectator->TargetPlayerID.Empty();
        Spectator->CurrentMode = ESpectatorMode::FollowUnit;
        
        UpdateSpectatorInfo(SpectatorID, *Spectator);
        OnSpectatorModeChanged.Broadcast(SpectatorID, ESpectatorMode::FollowUnit);
    }
}

void UMingRTSSpectatorSystem::SetCameraTransform(const FString& SpectatorID, const FVector& Location, const FRotator& Rotation)
{
    FSpectatorInfo* Spectator = ActiveSpectators.Find(SpectatorID);
    if (Spectator)
    {
        Spectator->CameraLocation = Location;
        Spectator->CameraRotation = Rotation;
        UpdateSpectatorInfo(SpectatorID, *Spectator);
    }
}

void UMingRTSSpectatorSystem::SetZoomLevel(const FString& SpectatorID, float ZoomLevel)
{
    FSpectatorInfo* Spectator = ActiveSpectators.Find(SpectatorID);
    if (Spectator)
    {
        Spectator->ZoomLevel = FMath::Clamp(ZoomLevel, 0.1f, 5.0f);
        UpdateSpectatorInfo(SpectatorID, *Spectator);
    }
}

void UMingRTSSpectatorSystem::TogglePictureInPicture(const FString& SpectatorID, bool bEnable)
{
    FSpectatorInfo* Spectator = ActiveSpectators.Find(SpectatorID);
    if (Spectator)
    {
        if (bEnable)
        {
            Spectator->CurrentMode = ESpectatorMode::PictureInPicture;
        }
        else
        {
            Spectator->CurrentMode = ESpectatorMode::FreeCamera;
        }
        UpdateSpectatorInfo(SpectatorID, *Spectator);
        OnSpectatorModeChanged.Broadcast(SpectatorID, Spectator->CurrentMode);
    }
}

void UMingRTSSpectatorSystem::SendSpectatorChat(const FString& RoomID, const FString& SpectatorID, const FString& Message)
{
    FSpectatorRoom* Room = ActiveRooms.Find(RoomID);
    if (Room && !Room->bAllowChat)
    {
        return; // 聊天被禁用
    }
    
    OnSpectatorChat.Broadcast(RoomID, SpectatorID, Message);
    UE_LOG(LogTemp, Log, TEXT("Spectator chat in room %s: %s"), *RoomID, *Message);
}

TArray<FSpectatorInfo> UMingRTSSpectatorSystem::GetSpectatorsInRoom(const FString& RoomID) const
{
    const FSpectatorRoom* Room = ActiveRooms.Find(RoomID);
    if (Room)
    {
        return Room->Spectators;
    }
    return TArray<FSpectatorInfo>();
}

int32 UMingRTSSpectatorSystem::GetSpectatorCount(const FString& RoomID) const
{
    const FSpectatorRoom* Room = ActiveRooms.Find(RoomID);
    if (Room)
    {
        return Room->CurrentSpectators;
    }
    return 0;
}

bool UMingRTSSpectatorSystem::IsSpectatingEnabled(const FString& MatchID) const
{
    return SpectatingEnabledMatches.Contains(MatchID);
}

void UMingRTSSpectatorSystem::SetSpectatingEnabled(const FString& MatchID, bool bEnabled)
{
    if (bEnabled)
    {
        SpectatingEnabledMatches.Add(MatchID);
    }
    else
    {
        SpectatingEnabledMatches.Remove(MatchID);
    }
}

void UMingRTSSpectatorSystem::SetRoomPassword(const FString& RoomID, const FString& NewPassword)
{
    FSpectatorRoom* Room = ActiveRooms.Find(RoomID);
    if (Room)
    {
        Room->bIsPasswordProtected = !NewPassword.IsEmpty();
        Room->Password = NewPassword;
    }
}

void UMingRTSSpectatorSystem::KickSpectator(const FString& RoomID, const FString& SpectatorID, const FString& Reason)
{
    FSpectatorRoom* Room = ActiveRooms.Find(RoomID);
    if (Room)
    {
        // 找到並移除觀戰者
        for (int32 i = 0; i < Room->Spectators.Num(); ++i)
        {
            if (Room->Spectators[i].SpectatorID == SpectatorID)
            {
                Room->Spectators.RemoveAt(i);
                Room->CurrentSpectators = FMath::Max(0, Room->CurrentSpectators - 1);
                break;
            }
        }
        
        ActiveSpectators.Remove(SpectatorID);
        
        UE_LOG(LogTemp, Log, TEXT("Spectator %s kicked from room %s. Reason: %s"), 
            *SpectatorID, *RoomID, *Reason);
    }
}

void UMingRTSSpectatorSystem::MuteSpectator(const FString& RoomID, const FString& SpectatorID, int32 DurationMinutes)
{
    FSpectatorInfo* Spectator = ActiveSpectators.Find(SpectatorID);
    if (Spectator)
    {
        Spectator->bIsChatEnabled = false;
        UpdateSpectatorInfo(SpectatorID, *Spectator);
        
        UE_LOG(LogTemp, Log, TEXT("Spectator %s muted for %d minutes"), *SpectatorID, DurationMinutes);
        
        // 這裡應該設置一個定時器，在DurationMinutes後解除禁言
    }
}

FString UMingRTSSpectatorSystem::GenerateRoomID()
{
    return FString::Printf(TEXT("ROOM_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
}

void UMingRTSSpectatorSystem::BroadcastToRoom(const FString& RoomID, const FString& Message)
{
    // 向房間內所有觀戰者廣播消息
    UE_LOG(LogTemp, Log, TEXT("Broadcast to room %s: %s"), *RoomID, *Message);
}

void UMingRTSSpectatorSystem::UpdateSpectatorInfo(const FString& SpectatorID, const FSpectatorInfo& Info)
{
    // 更新活躍觀戰者信息
    if (ActiveSpectators.Contains(SpectatorID))
    {
        ActiveSpectators[SpectatorID] = Info;
    }
    
    // 更新房間內的觀戰者信息
    for (auto& RoomPair : ActiveRooms)
    {
        for (FSpectatorInfo& RoomSpectator : RoomPair.Value.Spectators)
        {
            if (RoomSpectator.SpectatorID == SpectatorID)
            {
                RoomSpectator = Info;
                return;
            }
        }
    }
}
