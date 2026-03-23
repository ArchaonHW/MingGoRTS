#pragma once


#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMingGoRTSModule : public IModuleInterface
{
public:
    virtual void StartupModule() overHide;
    virtual void ShutdownModule() overHide;
};

