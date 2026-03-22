#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Final comprehensive fix for all remaining errors"""

import re
import os
import glob

def comprehensive_fix(file_path):
    """Apply all remaining fixes to a file"""
    if not os.path.exists(file_path):
        return False, "Not found"
    
    try:
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            content = f.read()
        
        original = content
        changes = []
        
        # Fix 1: UENUM with ( instead of {
        content = re.sub(
            r'UENUM\([^)]*\)\s*enum class\s+(\w+)\s*:\s*\w+\s*\(',
            r'UENUM(BlueprintType)\nenum class \1 : uint8 {',
            content
        )
        if content != original:
            changes.append("Fixed UENUM brace")
        
        # Fix 2: Add commas between UENUM entries
        content = re.sub(
            r'(\w+)\s*\(\s*UMETA\(([^)]+)\)\s*\)\s*\n\s*(\w+)',
            r'\1(UMETA(\2)),\n    \3',
            content
        )
        
        # Fix 3: Ensure class/struct ends with };
        if not content.rstrip().endswith('};') and ('class ' in content or 'struct ' in content):
            content = content.rstrip() + '\n};\n'
            changes.append("Added closing };")
        
        # Fix 4: Fix unterminated character constants
        new_content = re.sub(
            r'"[^"]*[\x00-\x08\x0b-\x0c\x0e-\x1f\x80-\xff][^"]*"',
            '""',
            content
        )
        if new_content != content:
            content = new_content
            changes.append("Fixed char constants")
        
        # Fix 5: Fix function params with }; instead of );
        content = re.sub(r'\(\s*\}\s*;', '();', content)
        
        # Fix 6: Fix }; to ); in function declarations
        lines = content.split('\n')
        fixed_lines = []
        for line in lines:
            if re.search(r'\([^)]*\w+\s*\w*}\s*;\s*$', line):
                line = re.sub(r'(\([^)]*)}(\s*;\s*)$', r'\1)\2', line)
            fixed_lines.append(line)
        content = '\n'.join(fixed_lines)
        
        # Fix 7: Fix #endif mismatch
        if_count = content.count('#if') + content.count('#ifdef') + content.count('#ifndef')
        endif_count = content.count('#endif')
        if if_count > endif_count:
            content += '\n#endif\n' * (if_count - endif_count)
            changes.append(f"Added {if_count - endif_count} #endif")
        
        if content != original:
            with open(file_path, 'w', encoding='utf-8-sig') as f:
                f.write(content)
            return True, " | ".join(changes)
        return False, "No changes"
        
    except Exception as e:
        return False, f"Error: {e}"

# Get all header files and fix them
header_files = glob.glob(r'c:\HW\MingGoRTS\Source\**\*.h', recursive=True)
header_files += glob.glob(r'c:\HW\MingGoRTS\Plugins\**\*.h', recursive=True)

print(f"Comprehensive fix for {len(header_files)} files...")
print("=" * 70)

fixed = 0
for file_path in header_files:
    is_fixed, msg = comprehensive_fix(file_path)
    if is_fixed:
        print(f"[FIXED] {os.path.basename(file_path)}: {msg}")
        fixed += 1

print("=" * 70)
print(f"Fixed {fixed} files")
print("Done!")
