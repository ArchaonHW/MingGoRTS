#include "MingBlockchain.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#define LOCTEXT_NAMESPACE "FMingBlockchainModule"

void FMingBlockchainModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    UE_LOG(LogTemp, Warning, TEXT("MingBlockchain: Module started"));

    // Initialize HTTP request manager
    HttpRequestManager = &FHttpModule::Get();
    
    // Log blockchain module initialization
    UE_LOG(LogTemp, Log, TEXT("MingBlockchain: HTTP Request Manager initialized"));
}

void FMingBlockchainModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    UE_LOG(LogTemp, Warning, TEXT("MingBlockchain: Module shutdown"));

    // Clean up HTTP request manager
    if (HttpRequestManager)
    {
        HttpRequestManager = nullptr;
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMingBlockchainModule, MingBlockchain)
