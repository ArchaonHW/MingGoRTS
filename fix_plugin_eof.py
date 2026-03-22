#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fix EOF and brace matching errors in plugin header files"""

import re
import os

def fix_eof_and_braces(file_path):
    """Fix EOF errors by ensuring proper brace and parenthesis matching"""
    if not os.path.exists(file_path):
        return False, "File not found"
    
    try:
        # Read file
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            lines = f.readlines()
        
        fixed_lines = []
        changes = []
        
        for i, line in enumerate(lines):
            original = line
            # Fix common syntax error: }; after function parameters should be );
            # Match patterns like: void FunctionName(Params};
            line = re.sub(r'(\([^)]*)}\s*;\s*$', r'\1);', line)
            
            if original != line:
                changes.append(f"Line {i+1}: Fixed }; to );")
            
            fixed_lines.append(line)
        
        # Write back
        with open(file_path, 'w', encoding='utf-8-sig') as f:
            f.writelines(fixed_lines)
        
        if changes:
            return True, f"Fixed {len(changes)} lines"
        else:
            return False, "No changes needed"
            
    except Exception as e:
        return False, f"Error: {str(e)}"

# Fix the plugin files
files_to_fix = [
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\MingCharacterGrowthSystem.h',
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\MingCoreEventBus.h'
]

print("Fixing EOF and brace errors in plugin files...")
print("=" * 60)

for file_path in files_to_fix:
    fixed, message = fix_eof_and_braces(file_path)
    status = "[FIXED]" if fixed else "[INFO]"
    print(f"{status} {os.path.basename(file_path)}: {message}")

print("=" * 60)
print("Done!")
