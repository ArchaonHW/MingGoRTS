#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Comprehensive syntax error fixer for UE5 header files"""

import re
import os
import glob

def fix_file(file_path):
    """Fix all common syntax errors in a header file"""
    if not os.path.exists(file_path):
        return False, "File not found"
    
    try:
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            content = f.read()
        
        original = content
        changes = []
        
        # Fix 1: Function declarations ending with }; instead of );
        # Pattern: Type FunctionName(Params};
        new_content = re.sub(r'(\([^)]*\)}\s*;)', r')\1[0:-1];', content)
        if new_content != content:
            content = new_content
            changes.append("Fixed }; to ); in function declarations")
        
        # Fix 2: Delegate declarations with }; instead of );
        # Match DECLARE_DYNAMIC_MULTICAST_DELEGATE macros
        new_content = re.sub(r'(DECLARE_DYNAMIC_MULTICAST_DELEGATE[^}]*\([^)]*\)};)', r')\1[0:-1];', content)
        if new_content != content:
            content = new_content
            changes.append("Fixed delegate declarations")
        
        # Fix 3: Fix typo in delegate declaration (lowercase 'delegate')
        new_content = re.sub(r'DECLARE_DYNAMIC_MULTICAST_Delegate', r'DECLARE_DYNAMIC_MULTICAST_DELEGATE', content)
        if new_content != content:
            content = new_content
            changes.append("Fixed delegate case typo")
        
        # Fix 4: UENUM entries with missing comma
        # Pattern: EnumValue UMETA(...) without comma before next value
        new_content = re.sub(r'(\w+)\s+UMETA\(([^)]+)\)\s*\n\s*(\w+)', r'\1 UMETA(\2),\n    \3', content)
        if new_content != content:
            content = new_content
            changes.append("Added missing commas in UENUM")
        
        # Fix 5: Fix unterminated character constants (Unicode issues)
        # Replace garbled characters in string literals
        new_content = re.sub(r'"[^"]*[\x00-\x08\x0b-\x0c\x0e-\x1f][^"]*"', r'""', content)
        if new_content != content:
            content = new_content
            changes.append("Fixed unterminated character constants")
        
        if content != original:
            with open(file_path, 'w', encoding='utf-8-sig') as f:
                f.write(content)
            return True, " | ".join(changes)
        else:
            return False, "No changes"
            
    except Exception as e:
        return False, f"Error: {e}"

# List of files with known errors
error_files = [
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingSelectionWidget.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingLargeScaleCombatTest.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\GameMode\OneClick\MyOneClickGameMode.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Network\MingNetworkSyncOptimizer.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingSquadAI.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingMultiUnitBlueprintLibrary.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingRTSPlayerController.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingTacticalBlueprintLibrary.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Optimization\MingRTSOptimizationCompiler.h',
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\Tests\MingBoundaryTestSuite.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingTacticalCombatSystem.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Optimization\MingRTSOptimizationSystemManager.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Network\MingNetworkSecurityManager.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Optimization\MingRTSPerformanceProfiler.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingUnitController.h',
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\Tests\MingReleaseIntegrationTest.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingUnitBlueprintLibrary.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\AI\MingBaseCombatAI.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\SageCommand\MingWuXingRhythmSystem.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\AI\MingNationalRevolutionaryArmyAI.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\VRAR\MingRTSARSupport.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Stats\MingRTSStatisticsSystem.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Security\MingRTSAntiCheat.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Security\MingRTSServerValidation.h',
]

print("Fixing syntax errors in header files...")
print("=" * 70)

fixed_count = 0
error_count = 0

for file_path in error_files:
    if os.path.exists(file_path):
        fixed, message = fix_file(file_path)
        status = "[FIXED]" if fixed else "[INFO]"
        print(f"{status} {os.path.basename(file_path)}: {message}")
        if fixed:
            fixed_count += 1
        elif "Error" in message:
            error_count += 1
    else:
        print(f"[SKIP] {os.path.basename(file_path)}: File not found")

print("=" * 70)
print(f"Summary: {fixed_count} files fixed, {error_count} errors")
