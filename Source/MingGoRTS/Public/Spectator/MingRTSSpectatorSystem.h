#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSpectatorSystem.generated.h"

/**
 * 觀察模式 */
UENUM(BlueprintType)
enum class ESpectatorMode: uint8 {
    FreeCamera        UMETA(DisplayName = "Free Camera"),
    FollowPlayer      UMETA(DisplayName = "Follow Player"),
    FollowUnit        UMETA(DisplayName = "Follow Unit"),
    TacticalView      UMETA(DisplayName = "Tactical View"),
    PictureInPicture  UMETA(DisplayName = "Picture in Picture"),
    BroadcastDirector UMETA(DisplayName = "Broadcast Director")
};

/**
 * 觀察者信息 */
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
 * 觀?�房X */
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
 * 觀?�系�?- 管�X�戲觀?��X */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSSpectatorSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSSpectatorSystem();
    
    // ?��X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void InitializeSpectatorSystem();
    
    // ?�建觀?�房X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    FString CreateSpectatorRoom(const FString& MatchID, const FString& RoomName, int32 MaxSpectators, bool bPasswordProtected, const FString& Password);
    
    // ?��?觀?�房X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void CloseSpectatorRoom(const FString& RoomID);
    
    // ?�入觀X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    bool JoinSpectatorRoom(const FString& RoomID, const FSpectatorInfo& Spectator, const FString& Password);
    
    // ?��?觀X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void LeaveSpectatorRoom(const FString& RoomID, const FString& SpectatorID);
    
    // ?��?觀?�房?��?�?
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    TArray<FSpectatorRoom> GetAvailableRooms() const;
    
    // ?��?觀?�房?�信X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    bool GetRoomInfo(const FString& RoomID, FSpectatorRoom& OutRoom) const;
    
    // 設置觀?�模�?
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetSpectatorMode(const FString& SpectatorID, ESpectatorMode Mode);
    
    // 跟隨?�家
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void FollowPlayer(const FString& SpectatorID, const FString& TargetPlayerID);
    
    // 跟隨?��?
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void FollowUnit(const FString& SpectatorID, int32 TargetUnitID);
    
    // 設置?��?機�?置�X��?
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetCameraTransform(const FString& SpectatorID, const FVector& Location, const FRotator& Rotation);
    
    // 設置縮放
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetZoomLevel(const FString& SpectatorID, float ZoomLevel);
    
    // ?��X�中X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void TogglePictureInPicture(const FString& SpectatorID, bool bEnable);
    
    // ?�送�X��?天�X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SendSpectatorChat(const FString& RoomID, const FString& SpectatorID, const FString& Message);
    
    // ?��?觀?�者�?�?
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    TArray<FSpectatorInfo> GetSpectatorsInRoom(const FString& RoomID) const;
    
    // ?��?觀?�者數X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    int32 GetSpectatorCount(const FString& RoomID) const;
    
    // ?�否?�許觀X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    bool IsSpectatingEnabled(const FString& MatchID) const;
    
    // ?�用/禁用觀X
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetSpectatingEnabled(const FString& MatchID, bool bEnabled);
    
    // 設置?��?密碼
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void SetRoomPassword(const FString& RoomID, const FString& NewPassword);
    
    // 踢出觀?��?
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void KickSpectator(const FString& RoomID, const FString& SpectatorID, const FString& Reason);
    
    // 禁�?觀?��?
    UFUNCTION(BlueprintCallable, Category = "Spectator")
    void MuteSpectator(const FString& RoomID, const FString& SpectatorID, int32 DurationMinutes);
    
    // 事件委�?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectatorJoined, const FString&, RoomID, const FSpectatorInfo&, Spectator);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectatorLeft, const FString&, RoomID, const FString&, SpectatorID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSpectatorChat, const FString&, RoomID, const FString&, SpectatorID, const FString&, Message);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectatorModeChanged, const FString&, SpectatorID, ESpectatorMode, NewMode);
    
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
    
    FString GenerateRoomID();
    void BroadcastToRoom(const FString& RoomID, const FString& Message);
    void UpdateSpectatorInfo(const FString& SpectatorID, const FSpectatorInfo& Info);
};

