#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Advanced syntax error fixer with specific patterns for UE5 header files"""

import re
import os

def fix_eof_and_braces(file_path):
    """Fix EOF errors by ensuring proper brace and parenthesis matching"""
    if not os.path.exists(file_path):
        return False, "File not found"
    
    try:
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            lines = f.readlines()
        
        fixed_lines = []
        in_multiline_comment = False
        changes = []
        
        for i, line in enumerate(lines):
            original = line
            
            # Track multiline comments
            if '/*' in line and '*/' not in line:
                in_multiline_comment = True
            if '*/' in line:
                in_multiline_comment = False
            
            # Skip comment lines
            if line.strip().startswith('//') or in_multiline_comment:
                fixed_lines.append(line)
                continue
            
            # Fix 1: Function parameters ending with }; instead of );
            # Match: void FunctionName(Type Param};
            if re.search(r'\([^)]*\w+\s+\w*}\s*;\s*$', line):
                line = re.sub(r'(\([^)]*)}(\s*;\s*)$', r'\1)\2', line)
                if original != line:
                    changes.append(f"Line {i+1}: Fixed }}; to );")
            
            # Fix 2: Missing closing parenthesis in function declarations
            # Match: void FunctionName(Params;
            if re.search(r'\b(UFUNCTION|void|bool|int|float|FString)\b.*\([^)]*\w+\s+\w*;\s*$', line):
                # Check if this is a function declaration missing )
                if '(' in line and ')' not in line:
                    line = line.rstrip().rstrip(';') + ');\n'
                    if original != line:
                        changes.append(f"Line {i+1}: Added missing )")
            
            fixed_lines.append(line)
        
        # Write back
        with open(file_path, 'w', encoding='utf-8-sig') as f:
            f.writelines(fixed_lines)
        
        if changes:
            return True, f"Fixed {len(changes)} issues"
        else:
            return False, "No changes needed"
            
    except Exception as e:
        return False, f"Error: {str(e)}"

# Files with specific errors to fix
files_with_eof_errors = [
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\Tests\MingStressTestSuite.h',
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\Decision\MingPoliticalConsequenceCalculator.h',
]

files_with_param_errors = [
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingSelectionWidget.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingSquadAI.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingRTSPlayerController.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingTacticalBlueprintLibrary.h',
    r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingUnitBlueprintLibrary.h',
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\Tests\MingUserExperienceTest.h',
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\UI\MingUIManager.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\VRAR\MingRTSARSupport.h',
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Security\MingRTSServerValidation.h',
]

print("Fixing EOF errors...")
print("=" * 60)
for file_path in files_with_eof_errors:
    if os.path.exists(file_path):
        fixed, message = fix_eof_and_braces(file_path)
        status = "[FIXED]" if fixed else "[INFO]"
        print(f"{status} {os.path.basename(file_path)}: {message}")

print("\nFixing parameter errors...")
print("=" * 60)
for file_path in files_with_param_errors:
    if os.path.exists(file_path):
        fixed, message = fix_eof_and_braces(file_path)
        status = "[FIXED]" if fixed else "[INFO]"
        print(f"{status} {os.path.basename(file_path)}: {message}")

print("=" * 60)
print("Done!")
