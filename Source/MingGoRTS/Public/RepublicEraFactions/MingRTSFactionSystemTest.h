#pragma once

﻿// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Republic Era Factions - System Test Ineader


#include "CoreMinimal.h"

/**
 * MingRTS Faction System Test
 * 
 * This header provides the interface for running tests on the Republic Era Faction System.
 * Tests cover faction manager initialization, faction creation, attributes, units, mechanics,
 * and gameplay guides.
 */
class MINGRTS_API MingRTSFactionSystemTest
{
public:
    /**
     * Run all faction system tests
     * @return true if all tests pass, false otherwise
     */
    static bool RunAllTests(};

private:
    /**
     * Test faction manager initialization
     */
    static bool TestFactionManagerInitialization(};

    /**
     * Test Beiyang Government faction creation
     */
    static bool TestBeiyangGovernmentCreation(};

    /**
     * Test faction attributes
     */
    static bool TestFactionAttributes(};

    /**
     * Test unique units
     */
    static bool TestUniqueUnits(};

    /**
     * Test unique mechanics
     */
    static bool TestUniqueMechanics(};

    /**
     * Test gameplay guide
     */
    static bool TestGameplayGuide(};
};

/**
 * Global function to run MingRTS faction system tests
 * This can be called from game code or console commands
 * 
 * @return true if all tests pass, false otherwise
 */
bool RunMingRTSFactionSystemTests(};
};
