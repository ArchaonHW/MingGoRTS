#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSpectatorSystem.generated.h"

/**
 * ËßÄ?∞Ê®°Âº? */
UENUM(BlueprintType)
enum class ESpectatorMode : uint8
{
    FreeCamera        UMETA(DisplayName = "?™Áî±Ë¶ñË?"),
    FollowPlayer      UMETA(DisplayName = "Ë∑üÈö®?©ÂÆ∂"),
    FollowUnit        UMETA(DisplayName = "Ë∑üÈö®?Æ‰?"),
    TacticalView      UMETA(DisplayName = "?∞Ë?Ë¶ñË?"),
    PictureInPicture  UMETA(DisplayName = "?´‰∏≠??),
    BroadcastDirector UMETA(DisplayName = "Â∞éÊí≠Ê®°Â?")
};

/**
 * ËßÄ?∞ËÄÖ‰ø°?? */
USTRUCT(BlueprintType)
struct FSpectatorInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SpectatorID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESpectatorMode CurrentMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetPlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetUnitID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CameraLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator CameraRotation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ZoomLevel;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsChatEnabled;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString JoinTime;
    
    FSpectatorInfo()
        : CurrentMode(ESpectatorMode::FreeCamera)
        , TargetUnitID(-1)
        , ZoomLevel(1.0f)
        , bIsChatEnabled(true)
    {}
};

/**
 * ËßÄ?∞Êàø?? */
USTRUCT(BlueprintType)
struct FSpectatorRoom
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RoomID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MatchID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RoomName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxSpectators;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentSpectators;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPasswordProtected;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Password;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAllowChat;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAllowRecording;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSpectatorInfo> Spectators;
    
    FSpectatorRoom()
        : MaxSpectators(100)
        , CurrentSpectators(0)
        , bIsPasswordProtected(false)
        , bAllowChat(true)
        , bAllowRecording(true)
    {}
};

/**
 * ËßÄ?∞Á≥ªÁµ?- ÁÆ°Á??äÊà≤ËßÄ?∞Â??? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSSpectatorSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSSpectatorSystem(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void InitializeSpectatorSystem(};
    
    // ?µÂª∫ËßÄ?∞Êàø??    UFUNCTION(BlueprintCallable, Category = "Spectator")
    FString CreateSpectatorRoom(const FString& MatchID, const FString& RoomName, int32 MaxSpectators, bool bPasswordProtected, const FString& Password};
    
    // ?úÈ?ËßÄ?∞Êàø??    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void CloseSpectatorRoom(const FString& RoomID};
    
    // ?†ÂÖ•ËßÄ??    UFUNCTION(BlueprintCallable, Category = "Spectator")
    bool JoinSpectatorRoom(const FString& RoomID, const FSpectatorInfo& Spectator, const FString& Password};
    
    // ?¢È?ËßÄ??    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void LeaveSpectatorRoom(const FString& RoomID, const FString& SpectatorID};
    
    // ?≤Â?ËßÄ?∞Êàø?ìÂ?Ë°?    UFUNCTION(BlueprintCallable, Category = "Spectator")
    TArray<FSpectatorRoom> GetAvailableRooms() const;
    
    // ?≤Â?ËßÄ?∞Êàø?ì‰ø°??    UFUNCTION(BlueprintCallable, Category = "Spectator")
    bool GetRoomInfo(const FString& RoomID, FSpectatorRoom& OutRoom) const;
    
    // Ë®≠ÁΩÆËßÄ?∞Ê®°Âº?    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetSpectatorMode(const FString& SpectatorID, ESpectatorMode Mode};
    
    // Ë∑üÈö®?©ÂÆ∂
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void FollowPlayer(const FString& SpectatorID, const FString& TargetPlayerID};
    
    // Ë∑üÈö®?Æ‰?
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void FollowUnit(const FString& SpectatorID, int32 TargetUnitID};
    
    // Ë®≠ÁΩÆ?ùÂ?Ê©ü‰?ÁΩÆÂ??ãË?
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetCameraTransform(const FString& SpectatorID, const FVector& Location, const FRotator& Rotation};
    
    // Ë®≠ÁΩÆÁ∏ÆÊîæ
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetZoomLevel(const FString& SpectatorID, float ZoomLevel};
    
    // ?áÊ??´‰∏≠??    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void TogglePictureInPicture(const FString& SpectatorID, bool bEnable};
    
    // ?ºÈÄÅË??∞Ë?Â§©Ê???    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SendSpectatorChat(const FString& RoomID, const FString& SpectatorID, const FString& Message};
    
    // ?≤Â?ËßÄ?∞ËÄÖÂ?Ë°?    UFUNCTION(BlueprintCallable, Category = "Spectator")
    TArray<FSpectatorInfo> GetSpectatorsInRoom(const FString& RoomID) const;
    
    // ?≤Â?ËßÄ?∞ËÄÖÊï∏??    UFUNCTION(BlueprintCallable, Category = "Spectator")
    int32 GetSpectatorCount(const FString& RoomID) const;
    
    // ?ØÂê¶?ÅË®±ËßÄ??    UFUNCTION(BlueprintCallable, Category = "Spectator")
    bool IsSpectatingEnabled(const FString& MatchID) const;
    
    // ?üÁî®/Á¶ÅÁî®ËßÄ??    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetSpectatingEnabled(const FString& MatchID, bool bEnabled};
    
    // Ë®≠ÁΩÆ?øÈ?ÂØÜÁ¢º
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetRoomPassword(const FString& RoomID, const FString& NewPassword};
    
    // Ë∏¢Âá∫ËßÄ?∞ËÄ?    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void KickSpectator(const FString& RoomID, const FString& SpectatorID, const FString& Reason};
    
    // Á¶ÅË?ËßÄ?∞ËÄ?    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void MuteSpectator(const FString& RoomID, const FString& SpectatorID, int32 DurationMinutes};
    
    // ‰∫ã‰ª∂ÂßîË?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectatorJoined, const FString&, RoomID, const FSpectatorInfo&, Spectator};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectatorLeft, const FString&, RoomID, const FString&, SpectatorID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSpectatorChat, const FString&, RoomID, const FString&, SpectatorID, const FString&, Message};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectatorModeChanged, const FString&, SpectatorID, ESpectatorMode, NewMode};
    
    UPROPERTY(BlueprintAssignable, Category = "Spectator|Events")
    FOnSpectatorJoined OnSpectatorJoined;
    
    UPROPERTY(BlueprintAssignable, Category = "Spectator|Events")
    FOnSpectatorLeft OnSpectatorLeft;
    
    UPROPERTY(BlueprintAssignable, Category = "Spectator|Events")
    FOnSpectatorChat OnSpectatorChat;
    
    UPROPERTY(BlueprintAssignable, Category = "Spectator|Events")
    FOnSpectatorModeChanged OnSpectatorModeChanged;
    
private:
    UPROPERTY()
    TMap<FString, FSpectatorRoom> ActiveRooms;
    
    UPROPERTY()
    TMap<FString, FSpectatorInfo> ActiveSpectators;
    
    UPROPERTY()
    TSet<FString> SpectatingEnabledMatches;
    
    FString GenerateRoomID(};
    void BroadcastToRoom(const FString& RoomID, const FString& Message};
    void UpdateSpectatorInfo(const FString& SpectatorID, const FSpectatorInfo& Info};
};

