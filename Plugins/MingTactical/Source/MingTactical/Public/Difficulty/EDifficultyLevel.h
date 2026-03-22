#pragma once

#include "CoreMinimal.h"
#include "EDifficultyLevel.generated.h"

/**
 * ??戲????等?????? | Game Difficulty Level Enum
 * 定義??個??準難度??級??????義選??| Define four standard difficulty levels and custom option
 */
UENUM(BlueprintType)
enum class EDifficultyLevel : uint8
{
    Easy       UMETA(DisplayName = "簡單 | Easy"),    // Easy
    Normal     UMETA(DisplayName = "????| Normal"),    // Normal
    Hard       UMETA(DisplayName = "??難 | Hard"),    // Hard
    Expert     UMETA(DisplayName = "專家 | Expert"),    // Expert
    Custom     UMETA(DisplayName = "??????| Custom")   // Custom
};

/**
 * ????管????| Difficulty Manager
 * 管????戲??????置??????調??| Manage game difficulty configuration and dynamic adjustment
 * 
 * ??能 | Features?? * - ????等??管?? | Difficulty Level Management (Easy/Normal/Hard/Expert/Custom)
 * - ????????調整 | Dynamic Difficulty Adjustment
 * - ????事件觸發 | Difficulty Event Triggering
 * - 平????渡算?? | Smooth Transition Algorithm
 */
/**
 * ??????數類?????? | Difficulty Parameter Type Enum
 * ??於????調整??????戲??數 | Used for dynamically adjusting specific game parameters
 */
UENUM(BlueprintType)
enum class EDifficultyParameter : uint8
{
    // AI ??????數 | AI Related Parameters
    AIReactionSpeed      UMETA(DisplayName = "AI??????度 | AI Reaction Speed"),  // AI Reaction Speed
    AIIntelligence       UMETA(DisplayName = "AI??能程度 | AI Intelligence"),  // AI Intelligence
    AIUnitStrength       UMETA(DisplayName = "AI????強度 | AI Unit Strength"),  // AI Unit Strength
    
    // 資????????數 | Resource Related Parameters
    ResourceCollection   UMETA(DisplayName = "資?????????? | Resource Collection"), // Resource Collection
    ResourceConsumption  UMETA(DisplayName = "資??消耗?? | Resource Consumption"),  // Resource Consumption
    
    // ??鬥??????數 | Combat Related Parameters
    PlayerDamageTaken    UMETA(DisplayName = "??家??到??害 | Player Damage Taken"), // Player Damage Taken
    PlayerDamageDealt    UMETA(DisplayName = "??家??????害 | Player Damage Dealt"), // Player Damage Dealt
    UnitHealth           UMETA(DisplayName = "??????命??| Unit Health"),    // Unit Health
    
    // 任????????數 | Mission Related Parameters
    MissionTimeLimit     UMETA(DisplayName = "任????????制 | Mission Time Limit"), // Mission Time Limit
    MissionObjective     UMETA(DisplayName = "任?????????? | Mission Objective Difficulty"), // Mission Objective Difficulty
    
    // ??罰??????數 | Penalty Related Parameters
    DeathPenalty         UMETA(DisplayName = "死亡??罰 | Death Penalty"),     // Death Penalty
    FailureCost          UMETA(DisplayName = "失?????? | Failure Cost")      // Failure Cost
};

/**
 * ????變??事件 | Difficulty Changed Event
 */
UENUM(BlueprintType)
enum class EDifficultyChangeDirection : uint8
{
    Increase    UMETA(DisplayName = "增????度 | Increase Difficulty"),
    Decrease    UMETA(DisplayName = "??????度 | Decrease Difficulty"),
    Maintain    UMETA(DisplayName = "?????? | Maintain Current")
};

/**
 * ??家表??????
 */
UENUM(BlueprintType)
enum class EPlayerPerformanceRating : uint8
{
    Excellent   UMETA(DisplayName = "优??"),
    Good        UMETA(DisplayName = "??好"),
    Average     UMETA(DisplayName = "一般"),
    Poor        UMETA(DisplayName = "?差"),
    Critical    UMETA(DisplayName = "危急")
};

