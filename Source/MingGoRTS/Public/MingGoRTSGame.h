#pragma once

#include "CoreMinimal.h"

class MINGGORTS_API FMingGoRTSGameModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
