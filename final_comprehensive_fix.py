#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Comprehensive fix for all remaining compilation errors"""

import re
import os
import glob

def comprehensive_fix(file_path):
    """Apply all possible fixes to a file"""
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
        
        # Fix 2: Add commas between UENUM entries
        content = re.sub(
            r'(\w+)\s*\(\s*UMETA\(([^)]+)\)\s*\)\s*\n\s*(\w+)',
            r'\1(UMETA(\2)),\n    \3',
            content
        )
        
        # Fix 3: Ensure class/struct ends with };
        if not content.rstrip().endswith('};') and ('class ' in content or 'struct ' in content):
            content = content.rstrip() + '\n};\n'
            changes.append("Added };")
        
        # Fix 4: Fix function params with }; instead of );
        content = re.sub(r'\(\s*\}\s*;', '();', content)
        
        # Fix 5: Fix }; to ); in function declarations
        lines = content.split('\n')
        fixed_lines = []
        for line in lines:
            if re.search(r'\([^)]*\w+\s*\w*}\s*;\s*$', line):
                line = re.sub(r'(\([^)]*)}(\s*;\s*)$', r'\1)\2', line)
            fixed_lines.append(line)
        content = '\n'.join(fixed_lines)
        
        # Fix 6: Fix #endif mismatch
        if_count = content.count('#if') + content.count('#ifdef') + content.count('#ifndef')
        endif_count = content.count('#endif')
        if if_count > endif_count:
            content += '\n#endif\n' * (if_count - endif_count)
            changes.append(f"Added {if_count - endif_count} #endif")
        
        # Fix 7: Fix generated header position
        lines = content.split('\n')
        gen_idx = -1
        last_include_idx = -1
        for i, line in enumerate(lines):
            if '.generated.h' in line:
                gen_idx = i
            elif '#include' in line and '.generated.h' not in line:
                last_include_idx = i
        
        if gen_idx > last_include_idx and gen_idx > 0 and last_include_idx >= 0:
            gen_line = lines.pop(gen_idx)
            lines.insert(last_include_idx + 1, gen_line)
            content = '\n'.join(lines)
            changes.append("Moved generated header")
        
        # Fix 8: Fix unterminated character constants
        content = re.sub(
            r'"[^"]*[\x00-\x08\x0b-\x0c\x0e-\x1f\x80-\xff][^"]*"',
            '""',
            content
        )
        
        # Fix 9: Fix void* UPROPERTY declarations - replace with UObject*
        content = re.sub(
            r'UPROPERTY\([^)]*\)\s*\n\s*void\s*\*',
            r'UPROPERTY()\n    UObject*',
            content
        )
        
        if content != original:
            with open(file_path, 'w', encoding='utf-8-sig') as f:
                f.write(content)
            return True, " | ".join(changes) if changes else "Fixed"
        return False, "No changes"
        
    except Exception as e:
        return False, f"Error: {e}"

# Get all header files
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
