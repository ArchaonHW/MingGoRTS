#include "MingGoRTSGameMode.h"
#include "MingGoRTSCamera.h"
#include "MingGoRTSPlayerController.h"
#include "MingGoRTSUnit.h"
#include "Kismet/GameplayStatics.h"

AMingGoRTSGameMode::AMingGoRTSGameMode()
{
    // 設置預設的類別
    DefaultPawnClass = AMingGoRTSCamera::StaticClass();
    PlayerControllerClass = AMingGoRTSPlayerController::StaticClass();
    GameStateClass = nullptr;
    
    RTSCamera = nullptr;
}

void AMingGoRTSGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 獲取 RTS 相機引用
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        RTSCamera = Cast<AMingGoRTSCamera>(PC->GetPawn());
    }
    
    // 生成初始單位
    SpawnInitialUnits();
    
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS GameMode Started"));
}

void AMingGoRTSGameMode::SpawnInitialUnits()
{
    // 生成一些測試單位
    for (int32 i = 0; i < 5; i++)
    {
        FVector SpawnLocation = FVector(i * 200.0f, 0.0f, 100.0f);
        FRotator SpawnRotation = FRotator::ZeroRotator;
        
        if (AMingGoRTSUnit* NewUnit = GetWorld()->SpawnActor<AMingGoRTSUnit>(AMingGoRTSUnit::StaticClass(), SpawnLocation, SpawnRotation))
        {
            GameUnits.Add(NewUnit);
            UE_LOG(LogTemp, Log, TEXT("Spawned unit at location: %s"), *SpawnLocation.ToString());
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Spawned %d initial units"), GameUnits.Num());
}
