#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * MinGoRTS Game Module
 */
class FMingGoRTSGameModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
