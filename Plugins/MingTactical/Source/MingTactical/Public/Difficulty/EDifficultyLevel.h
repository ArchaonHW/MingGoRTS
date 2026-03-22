#pragma once

#include "CoreMinimal.h"
#include "EDifficultyLevel.generated.h"

/**
 * X��X��X | Game Difficulty Level Enum
 * �w�qX��X������X��X�q��X| Define four standard difficulty levels and custom option
 */
UENUM(BlueprintType)
enum class EDifficultyLevel: uint8 {
    Easy       UMETA(DisplayName = "²�� | Easy"),    // Easy
    Normal     UMETA(DisplayName = "X| Normal"),    // Normal
    Hard       UMETA(DisplayName = "X�� | Hard"),    // Hard
    Expert     UMETA(DisplayName = "�M�a | Expert"),    // Expert
    Custom     UMETA(DisplayName = "X| Custom")   // Custom
};

/**
 * X��X| Difficulty Manager
 * ��X��X�mX��X| Manage game difficulty configuration and dynamic adjustment
 * 
 * X�� | FeaturesX * - X��X��X | Difficulty Level Management (Easy/Normal/Hard/Expert/Custom)
 * - X�վ� | Dynamic Difficulty Adjustment
 * - X�ƥ�Ĳ�o | Difficulty Event Triggering
 * - ��X���X | Smooth Transition Algorithm
 */
/**
 * X����X | Difficulty Parameter Type Enum
 * X��X�վ�X��X�� | Used for dynamically adjusting specific game parameters
 */
UENUM(BlueprintType)
enum class EDifficultyParameter: uint8 {
    // AI X�� | AI Related Parameters
    AIReactionSpeed      UMETA(DisplayName = "AIX�� | AI Reaction Speed"),  // AI Reaction Speed
    AIIntelligence       UMETA(DisplayName = "AIX��{�� | AI Intelligence"),  // AI Intelligence
    AIUnitStrength       UMETA(DisplayName = "AIX�j�� | AI Unit Strength"),  // AI Unit Strength
    
    // ��X�� | Resource Related Parameters
    ResourceCollection   UMETA(DisplayName = "��X | Resource Collection"), // Resource Collection
    ResourceConsumption  UMETA(DisplayName = "��X����X | Resource Consumption"),  // Resource Consumption
    
    // X��X�� | Combat Related Parameters
    PlayerDamageTaken    UMETA(DisplayName = "X�aX��X�` | Player Damage Taken"), // Player Damage Taken
    PlayerDamageDealt    UMETA(DisplayName = "X�aX�` | Player Damage Dealt"), // Player Damage Dealt
    UnitHealth           UMETA(DisplayName = "X�RX| Unit Health"),    // Unit Health
    
    // ��X�� | Mission Related Parameters
    MissionTimeLimit     UMETA(DisplayName = "��X�� | Mission Time Limit"), // Mission Time Limit
    MissionObjective     UMETA(DisplayName = "��X | Mission Objective Difficulty"), // Mission Objective Difficulty
    
    // X�@X�� | Penalty Related Parameters
    DeathPenalty         UMETA(DisplayName = "���`X�@ | Death Penalty"),     // Death Penalty
    FailureCost          UMETA(DisplayName = "��X | Failure Cost")      // Failure Cost
};

/**
 * X��X�ƥ� | Difficulty Changed Event
 */
UENUM(BlueprintType)
enum class EDifficultyChangeDirection: uint8 {
    Increase    UMETA(DisplayName = "�WX�� | Increase Difficulty"),
    Decrease    UMETA(DisplayName = "X�� | Decrease Difficulty"),
    Maintain    UMETA(DisplayName = "X | Maintain Current")
};

/**
 * X�a��X
 */
UENUM(BlueprintType)
enum class EPlayerPerformanceRating: uint8 {
    Excellent   UMETA(DisplayName = "ɬX"),
    Good        UMETA(DisplayName = "X�n"),
    Average     UMETA(DisplayName = "�@��"),
    Poor        UMETA(DisplayName = "?�t"),
    Critical    UMETA(DisplayName = "�M��")
};

