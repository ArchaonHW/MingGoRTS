#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSpectatorSystem.generated.h"

/**
 * 觀察模式 */
UENUM(BlueprintType)
enum class ESpectatorMode: uuint8 {
    FreeCamera        UMETA(DisplayName = "Free Camera"),
    FollowPlayer      UMETA(DisplayName = "Follow Player"),
    FollowUnit        UMETA(DisplayName = "Follow Unit"),
    TacticalView      UMETA(DisplayName = "Tactical View"),
    PictureInPicture  UMETA(DisplayName = "Picture in Picture"),
    BroadcastDirector UMETA(DisplayName = "Broadcast Director")
};

/**
 * 觀察者??? */
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
 * 觀ɥr�房X */
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
 * 觀ɥr�系�ɥr- 管�X�戲觀ɥr��X */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableConponent))
class MINGRTS_API UMingRTSSpectatorSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSSpectatorSystem(};
    
    // ɥr��X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void InitializeSpectatorSystem(};
    
    // ɥr�t觀ɥr�房X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    FString CreateSpectatorRoom(const FString& MatchID, const FString& RoomName, int32 MaxSpectators, bool bPasswordProtected, const FString& Password};
    
    // ɥr��觀�房X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void CloseSpectatorRoom(const FString& RoomID};
    
    // ɥr�H觀X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    bool JoinSpectatorRoom(const FString& RoomID, const FSpectatorInfo& Spectator, const FString& Password};
    
    // ɥr��ɥr觀X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void LeaveSpectatorRoom(const FString& RoomID, const FString& SpectatorID};
    
    // ɥr��觀�房ɥr��ɥr�ɥr
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    TArray<FSpectatorRoom> GetAvailableRooms() const;
    
    // ɥr��觀�房ɥr�信X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    bool GetRoomInfo(const FString& RoomID, FSpectatorRoom& OutRoom) const;
    
    // m觀ɥr�模�ɥr
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetSpectatorMode(const FString& SpectatorID, ESpectatorMode Mode};
    
    // 跟隨ɥr�家
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void FollowPlayer(const FString& SpectatorID, const FString& TargetPlayerID};
    
    // 跟隨ɥr��ɥr
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void FollowUnit(const FString& SpectatorID, int32 TargetUnitID};
    
    // mɥr��ɥr機�ɥrm�X��ɥr
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetCameraTransform(const FString& SpectatorID, const FVector& Location, const FRotator& Rotation};
    
    // m縮放
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetZoomLevel(const FString& SpectatorID, float ZoomLevel};
    
    // ɥr��X�中X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void TogglePictureInPicture(const FString& SpectatorID, bool bEnable};
    
    // ɥr�送�X��ɥr天�X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SendSpectatorChat(const FString& RoomID, const FString& SpectatorID, const FString& Message};
    
    // ɥr��觀�者�ɥr�ɥr
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    TArray<FSpectatorInfo> GetSpectatorsInRoom(const FString& RoomID) const;
    
    // ɥr��觀�者數X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    int32 GetSpectatorCount(const FString& RoomID) const;
    
    // ɥr�否ɥr�許觀X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    bool IsSpectatingEnabled(const FString& MatchID) const;
    
    // ɥr�y/禁y觀X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetSpectatingEnabled(const FString& MatchID, bool bEnabled};
    
    // mɥr��ɥr密碼
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetRoomPassword(const FString& RoomID, const FString& NewPassword};
    
    // 踢觀ɥr��ɥr
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void KickSpectator(const FString& RoomID, const FString& SpectatorID, const FString& Reason};
    
    // 禁�觀��ɥr
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void MuteSpectator(const FString& RoomID, const FString& SpectatorID, int32 DurationMinutes};
    
    // 事件委�ɥr
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectatorJoined, const FString&, RoomID, const FSpectatorInfo&, Spectator};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectatorLeft, const FString&, RoomID, const FString&, SpectatorID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSpectatorChat, const FString&, RoomID, const FString&, SpectatorID, const FString&, Message};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectatorModeChanged, const FString&, SpectatorID, ESpectatorMode, NewMode};
    
    UPROPERTY(BlueprintAssignable, Category = "SpectatorEvents")
    FOnSpectatorJoined OnSpectatorJoined;
    
    UPROPERTY(BlueprintAssignable, Category = "SpectatorEvents")
    FOnSpectatorLeft OnSpectatorLeft;
    
    UPROPERTY(BlueprintAssignable, Category = "SpectatorEvents")
    FOnSpectatorChat OnSpectatorChat;
    
    UPROPERTY(BlueprintAssignable, Category = "SpectatorEvents")
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

