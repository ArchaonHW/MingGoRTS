#pragma once


#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * MingGoRTS game module
 */
class FMingGoRTSGameModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};


