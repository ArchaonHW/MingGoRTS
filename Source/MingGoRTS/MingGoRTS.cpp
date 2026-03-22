#include "MingGoRTS.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FMingGoRTSModule, MingGoRTS)

void FMingGoRTSModule::StartupModule()
{
    // 最小化啟動 - 不進行任何複雜初始化
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Module Started Safely"));
}

void FMingGoRTSModule::ShutdownModule()
{
    // 安全關閉
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Module Shutdown"));
}
