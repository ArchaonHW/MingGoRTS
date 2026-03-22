#include "Difficulty/FMingDifficultySettings.h"

bool FMingDifficultyParameter::AdjustValue(float Delta)
{
    float NewValue = CurrentValue + Delta;
    SetValue(NewValue);
    
    // 返回是否达到边界
    return (FMath::IsNearlyEqual(CurrentValue, MinValue) || 
            FMath::IsNearlyEqual(CurrentValue, MaxValue));
}

void FMingDifficultyParameter::SetValue(float NewValue)
{
    CurrentValue = FMath::Clamp(NewValue, MinValue, MaxValue);
}

void FMingPresetDifficultyConfig::InitializeDefaultValues()
{
    ParameterValues.Empty();
    
    // 根据难度等级设置默认值
    switch (Level)
    {
    case EDifficultyLevel::Easy:
        ParameterValues.Add(EDifficultyParameter::AIReactionSpeed, 0.6f);
        ParameterValues.Add(EDifficultyParameter::AIIntelligence, 0.5f);
        ParameterValues.Add(EDifficultyParameter::AIUnitStrength, 0.7f);
        ParameterValues.Add(EDifficultyParameter::ResourceCollection, 1.3f);
        ParameterValues.Add(EDifficultyParameter::ResourceConsumption, 0.8f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageTaken, 0.7f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageDealt, 1.3f);
        ParameterValues.Add(EDifficultyParameter::UnitHealth, 1.2f);
        ParameterValues.Add(EDifficultyParameter::MissionTimeLimit, 1.5f);
        ParameterValues.Add(EDifficultyParameter::MissionObjective, 0.7f);
        ParameterValues.Add(EDifficultyParameter::DeathPenalty, 0.5f);
        ParameterValues.Add(EDifficultyParameter::FailureCost, 0.5f);
        DisplayName = TEXT("简单");
        Description = TEXT("适合新手的游戏难度，敌人较弱，资源更丰富");
        break;
        
    case EDifficultyLevel::Normal:
        ParameterValues.Add(EDifficultyParameter::AIReactionSpeed, 1.0f);
        ParameterValues.Add(EDifficultyParameter::AIIntelligence, 1.0f);
        ParameterValues.Add(EDifficultyParameter::AIUnitStrength, 1.0f);
        ParameterValues.Add(EDifficultyParameter::ResourceCollection, 1.0f);
        ParameterValues.Add(EDifficultyParameter::ResourceConsumption, 1.0f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageTaken, 1.0f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageDealt, 1.0f);
        ParameterValues.Add(EDifficultyParameter::UnitHealth, 1.0f);
        ParameterValues.Add(EDifficultyParameter::MissionTimeLimit, 1.0f);
        ParameterValues.Add(EDifficultyParameter::MissionObjective, 1.0f);
        ParameterValues.Add(EDifficultyParameter::DeathPenalty, 1.0f);
        ParameterValues.Add(EDifficultyParameter::FailureCost, 1.0f);
        DisplayName = TEXT("普通");
        Description = TEXT("标准游戏难度，平衡的挑战体验");
        break;
        
    case EDifficultyLevel::Hard:
        ParameterValues.Add(EDifficultyParameter::AIReactionSpeed, 1.3f);
        ParameterValues.Add(EDifficultyParameter::AIIntelligence, 1.2f);
        ParameterValues.Add(EDifficultyParameter::AIUnitStrength, 1.2f);
        ParameterValues.Add(EDifficultyParameter::ResourceCollection, 0.9f);
        ParameterValues.Add(EDifficultyParameter::ResourceConsumption, 1.1f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageTaken, 1.2f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageDealt, 0.9f);
        ParameterValues.Add(EDifficultyParameter::UnitHealth, 0.95f);
        ParameterValues.Add(EDifficultyParameter::MissionTimeLimit, 0.85f);
        ParameterValues.Add(EDifficultyParameter::MissionObjective, 1.2f);
        ParameterValues.Add(EDifficultyParameter::DeathPenalty, 1.2f);
        ParameterValues.Add(EDifficultyParameter::FailureCost, 1.2f);
        DisplayName = TEXT("困难");
        Description = TEXT("更具挑战性的游戏体验，需要战略思考");
        break;
        
    case EDifficultyLevel::Expert:
        ParameterValues.Add(EDifficultyParameter::AIReactionSpeed, 1.6f);
        ParameterValues.Add(EDifficultyParameter::AIIntelligence, 1.4f);
        ParameterValues.Add(EDifficultyParameter::AIUnitStrength, 1.4f);
        ParameterValues.Add(EDifficultyParameter::ResourceCollection, 0.8f);
        ParameterValues.Add(EDifficultyParameter::ResourceConsumption, 1.2f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageTaken, 1.4f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageDealt, 0.8f);
        ParameterValues.Add(EDifficultyParameter::UnitHealth, 0.9f);
        ParameterValues.Add(EDifficultyParameter::MissionTimeLimit, 0.7f);
        ParameterValues.Add(EDifficultyParameter::MissionObjective, 1.4f);
        ParameterValues.Add(EDifficultyParameter::DeathPenalty, 1.5f);
        ParameterValues.Add(EDifficultyParameter::FailureCost, 1.5f);
        DisplayName = TEXT("专家");
        Description = TEXT("最高难度，为资深RTS玩家设计");
        break;
        
    case EDifficultyLevel::Custom:
        // 自定义难度使用普通作为基准
        ParameterValues.Add(EDifficultyParameter::AIReactionSpeed, 1.0f);
        ParameterValues.Add(EDifficultyParameter::AIIntelligence, 1.0f);
        ParameterValues.Add(EDifficultyParameter::AIUnitStrength, 1.0f);
        ParameterValues.Add(EDifficultyParameter::ResourceCollection, 1.0f);
        ParameterValues.Add(EDifficultyParameter::ResourceConsumption, 1.0f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageTaken, 1.0f);
        ParameterValues.Add(EDifficultyParameter::PlayerDamageDealt, 1.0f);
        ParameterValues.Add(EDifficultyParameter::UnitHealth, 1.0f);
        ParameterValues.Add(EDifficultyParameter::MissionTimeLimit, 1.0f);
        ParameterValues.Add(EDifficultyParameter::MissionObjective, 1.0f);
        ParameterValues.Add(EDifficultyParameter::DeathPenalty, 1.0f);
        ParameterValues.Add(EDifficultyParameter::FailureCost, 1.0f);
        DisplayName = TEXT("自定义");
        Description = TEXT("可自由调整各项参数的游戏难度");
        break;
    }
}

FMingDifficultySettings::FMingDifficultySettings()
    : CurrentLevel(EDifficultyLevel::Normal)
    , bEnableDynamicAdjustment(true)
    , EvaluationIntervalSeconds(60.0f)
    , AdjustmentSmoothingFactor(0.1f)
    , bNotifyPlayerOnChange(true)
    , bAllowMidGameChange(true)
    , MinEvaluationSamples(5)
    , OscillationPreventionThreshold(0.3f)
{
    InitializeDefaultParameters();
}

void FMingDifficultySettings::InitializeFromPreset(EDifficultyLevel Level)
{
    CurrentLevel = Level;
    DynamicParameters.Empty();
    
    FMingPresetDifficultyConfig Preset = GetPresetConfig(Level);
    
    for (const auto& Pair : Preset.ParameterValues)
    {
        FMingDifficultyParameter Param(Pair.Key, Pair.Value);
        DynamicParameters.Add(Param);
    }
}

FMingDifficultyParameter* FMingDifficultySettings::GetParameter(EDifficultyParameter Type)
{
    for (auto& Param : DynamicParameters)
    {
        if (Param.ParameterType == Type)
        {
            return &Param;
        }
    }
    return nullptr;
}

const FMingDifficultyParameter* FMingDifficultySettings::GetParameter(EDifficultyParameter Type) const
{
    for (const auto& Param : DynamicParameters)
    {
        if (Param.ParameterType == Type)
        {
            return &Param;
        }
    }
    return nullptr;
}

float FMingDifficultySettings::GetParameterValue(EDifficultyParameter Type) const
{
    const FMingDifficultyParameter* Param = GetParameter(Type);
    return Param ? Param->CurrentValue : 1.0f;
}

void FMingDifficultySettings::SetParameterValue(EDifficultyParameter Type, float Value)
{
    FMingDifficultyParameter* Param = GetParameter(Type);
    if (Param)
    {
        Param->SetValue(Value);
    }
    else
    {
        FMingDifficultyParameter NewParam(Type, Value);
        DynamicParameters.Add(NewParam);
    }
}

bool FMingDifficultySettings::AdjustParameter(EDifficultyParameter Type, float Delta)
{
    FMingDifficultyParameter* Param = GetParameter(Type);
    if (Param)
    {
        return Param->AdjustValue(Delta);
    }
    return false;
}

void FMingDifficultySettings::InitializeDefaultParameters()
{
    InitializeFromPreset(CurrentLevel);
}

FMingPresetDifficultyConfig FMingDifficultySettings::GetPresetConfig(EDifficultyLevel Level)
{
    FMingPresetDifficultyConfig Config;
    Config.Level = Level;
    Config.InitializeDefaultValues();
    return Config;
}

TArray<FMingPresetDifficultyConfig> FMingDifficultySettings::GetAllPresetConfigs()
{
    TArray<FMingPresetDifficultyConfig> Configs;
    
    Configs.Add(GetPresetConfig(EDifficultyLevel::Easy));
    Configs.Add(GetPresetConfig(EDifficultyLevel::Normal));
    Configs.Add(GetPresetConfig(EDifficultyLevel::Hard));
    Configs.Add(GetPresetConfig(EDifficultyLevel::Expert));
    
    return Configs;
}
