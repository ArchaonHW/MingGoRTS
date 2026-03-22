#pragma once

#include "CoreMinimal.h"
#include "EDifficultyLevel.generated.h"

/**
 * ?Šæˆ²??º¦ç­‰ç??šè? | Game Difficulty Level Enum
 * å®šç¾©?›å€‹æ?æº–é›£åº¦ç?ç´šå??ªå?ç¾©é¸??| Define four standard difficulty levels and custom option
 */
UENUM(BlueprintType)
enum class EDifficultyLevel : uint8
{
    Easy       UMETA(DisplayName = "ç°¡å–® | Easy"),    // Easy
    Normal     UMETA(DisplayName = "?®é€?| Normal"),    // Normal
    Hard       UMETA(DisplayName = "?°é›£ | Hard"),    // Hard
    Expert     UMETA(DisplayName = "å°ˆå®¶ | Expert"),    // Expert
    Custom     UMETA(DisplayName = "?ªå?ç¾?| Custom")   // Custom
};

/**
 * ??º¦ç®¡ç???| Difficulty Manager
 * ç®¡ç??Šæˆ²??º¦?ç½®?Œå??‹èª¿??| Manage game difficulty configuration and dynamic adjustment
 * 
 * ?Ÿèƒ½ | Featuresï¼? * - ??º¦ç­‰ç?ç®¡ç? | Difficulty Level Management (Easy/Normal/Hard/Expert/Custom)
 * - ?•æ???º¦èª¿æ•´ | Dynamic Difficulty Adjustment
 * - ??º¦äº‹ä»¶è§¸ç™¼ | Difficulty Event Triggering
 * - å¹³æ??æ¸¡ç®—æ? | Smooth Transition Algorithm
 */
/**
 * ??º¦?ƒæ•¸é¡å??šè? | Difficulty Parameter Type Enum
 * ?¨æ–¼?•æ?èª¿æ•´?·é??Šæˆ²?ƒæ•¸ | Used for dynamically adjusting specific game parameters
 */
UENUM(BlueprintType)
enum class EDifficultyParameter : uint8
{
    // AI ?¸é??ƒæ•¸ | AI Related Parameters
    AIReactionSpeed      UMETA(DisplayName = "AI?æ??Ÿåº¦ | AI Reaction Speed"),  // AI Reaction Speed
    AIIntelligence       UMETA(DisplayName = "AI?ºèƒ½ç¨‹åº¦ | AI Intelligence"),  // AI Intelligence
    AIUnitStrength       UMETA(DisplayName = "AI?®ä?å¼·åº¦ | AI Unit Strength"),  // AI Unit Strength
    
    // è³‡æ??¸é??ƒæ•¸ | Resource Related Parameters
    ResourceCollection   UMETA(DisplayName = "è³‡æ??¶é??ˆç? | Resource Collection"), // Resource Collection
    ResourceConsumption  UMETA(DisplayName = "è³‡æ?æ¶ˆè€—ç? | Resource Consumption"),  // Resource Consumption
    
    // ?°é¬¥?¸é??ƒæ•¸ | Combat Related Parameters
    PlayerDamageTaken    UMETA(DisplayName = "?©å®¶?—åˆ°?·å®³ | Player Damage Taken"), // Player Damage Taken
    PlayerDamageDealt    UMETA(DisplayName = "?©å®¶? æ??·å®³ | Player Damage Dealt"), // Player Damage Dealt
    UnitHealth           UMETA(DisplayName = "?®ä??Ÿå‘½??| Unit Health"),    // Unit Health
    
    // ä»»å??¸é??ƒæ•¸ | Mission Related Parameters
    MissionTimeLimit     UMETA(DisplayName = "ä»»å??‚é??åˆ¶ | Mission Time Limit"), // Mission Time Limit
    MissionObjective     UMETA(DisplayName = "ä»»å??®æ???º¦ | Mission Objective Difficulty"), // Mission Objective Difficulty
    
    // ?²ç½°?¸é??ƒæ•¸ | Penalty Related Parameters
    DeathPenalty         UMETA(DisplayName = "æ­»äº¡?²ç½° | Death Penalty"),     // Death Penalty
    FailureCost          UMETA(DisplayName = "å¤±æ?ä»?ƒ¹ | Failure Cost")      // Failure Cost
};

/**
 * ??º¦è®Šå?äº‹ä»¶ | Difficulty Changed Event
 */
UENUM(BlueprintType)
enum class EDifficultyChangeDirection : uint8
{
    Increase    UMETA(DisplayName = "å¢å??¾åº¦ | Increase Difficulty"),
    Decrease    UMETA(DisplayName = "?ä??¾åº¦ | Decrease Difficulty"),
    Maintain    UMETA(DisplayName = "ç»´æ?å½“å? | Maintain Current")
};

/**
 * ?©å®¶è¡¨ç°è¯„çº§?šä¸¾
 */
UENUM(BlueprintType)
enum class EPlayerPerformanceRating : uint8
{
    Excellent   UMETA(DisplayName = "ä¼˜ç?"),
    Good        UMETA(DisplayName = "?¯å¥½"),
    Average     UMETA(DisplayName = "ä¸€??),
    Poor        UMETA(DisplayName = "è¾ƒå·®"),
    Critical    UMETA(DisplayName = "?±æ€?)
};
