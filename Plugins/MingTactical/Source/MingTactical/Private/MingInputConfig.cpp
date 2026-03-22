#include "MingInputConfig.h"
#include "InputAction.h"
#include "InputMappingContext.h"

UMingInputConfig::UMingInputConfig(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

const UInputAction* UMingInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
    for (const FMingInputActionConfig& Config : InputActionConfigs)
    {
        if (Config.InputTag == InputTag.GetTagName())
        {
            return Config.InputAction.LoadSynchronous();
        }
    }
    
    return nullptr;
}
