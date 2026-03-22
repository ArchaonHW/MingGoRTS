#pragma once

#include "CoreMinimal.h"
#include "EDifficultyLevel.generated.h"

/**
 * 遊戲難度等級枚舉 | Game Difficulty Level Enum
 * 定義四個標準難度等級和自定義選項 | Define four standard difficulty levels and custom option
 */
UENUM(BlueprintType)
enum class EDifficultyLevel : uint8
{
    Easy       UMETA(DisplayName = "簡單 | Easy"),    // Easy
    Normal     UMETA(DisplayName = "普通 | Normal"),    // Normal
    Hard       UMETA(DisplayName = "困難 | Hard"),    // Hard
    Expert     UMETA(DisplayName = "專家 | Expert"),    // Expert
    Custom     UMETA(DisplayName = "自定義 | Custom")   // Custom
};

/**
 * 難度管理器 | Difficulty Manager
 * 管理遊戲難度配置和動態調整 | Manage game difficulty configuration and dynamic adjustment
 * 
 * 功能 | Features：
 * - 難度等級管理 | Difficulty Level Management (Easy/Normal/Hard/Expert/Custom)
 * - 動態難度調整 | Dynamic Difficulty Adjustment
 * - 難度事件觸發 | Difficulty Event Triggering
 * - 平滑過渡算法 | Smooth Transition Algorithm
 */
/**
 * 難度參數類型枚舉 | Difficulty Parameter Type Enum
 * 用於動態調整具體遊戲參數 | Used for dynamically adjusting specific game parameters
 */
UENUM(BlueprintType)
enum class EDifficultyParameter : uint8
{
    // AI 相關參數 | AI Related Parameters
    AIReactionSpeed      UMETA(DisplayName = "AI反應速度 | AI Reaction Speed"),  // AI Reaction Speed
    AIIntelligence       UMETA(DisplayName = "AI智能程度 | AI Intelligence"),  // AI Intelligence
    AIUnitStrength       UMETA(DisplayName = "AI單位強度 | AI Unit Strength"),  // AI Unit Strength
    
    // 資源相關參數 | Resource Related Parameters
    ResourceCollection   UMETA(DisplayName = "資源收集效率 | Resource Collection"), // Resource Collection
    ResourceConsumption  UMETA(DisplayName = "資源消耗率 | Resource Consumption"),  // Resource Consumption
    
    // 戰鬥相關參數 | Combat Related Parameters
    PlayerDamageTaken    UMETA(DisplayName = "玩家受到傷害 | Player Damage Taken"), // Player Damage Taken
    PlayerDamageDealt    UMETA(DisplayName = "玩家造成傷害 | Player Damage Dealt"), // Player Damage Dealt
    UnitHealth           UMETA(DisplayName = "單位生命值 | Unit Health"),    // Unit Health
    
    // 任務相關參數 | Mission Related Parameters
    MissionTimeLimit     UMETA(DisplayName = "任務時間限制 | Mission Time Limit"), // Mission Time Limit
    MissionObjective     UMETA(DisplayName = "任務目標難度 | Mission Objective Difficulty"), // Mission Objective Difficulty
    
    // 懲罰相關參數 | Penalty Related Parameters
    DeathPenalty         UMETA(DisplayName = "死亡懲罰 | Death Penalty"),     // Death Penalty
    FailureCost          UMETA(DisplayName = "失敗代價 | Failure Cost")      // Failure Cost
};

/**
 * 難度變化事件 | Difficulty Changed Event
 */
UENUM(BlueprintType)
enum class EDifficultyChangeDirection : uint8
{
    Increase    UMETA(DisplayName = "增加难度 | Increase Difficulty"),
    Decrease    UMETA(DisplayName = "降低难度 | Decrease Difficulty"),
    Maintain    UMETA(DisplayName = "维持当前 | Maintain Current")
};

/**
 * 玩家表现评级枚举
 */
UENUM(BlueprintType)
enum class EPlayerPerformanceRating : uint8
{
    Excellent   UMETA(DisplayName = "优秀"),
    Good        UMETA(DisplayName = "良好"),
    Average     UMETA(DisplayName = "一般"),
    Poor        UMETA(DisplayName = "较差"),
    Critical    UMETA(DisplayName = "危急")
};
