#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Batch fix remaining compilation errors"""

import re
import os
import glob

def batch_fix_file(file_path):
    """Apply all fixes to a single file"""
    if not os.path.exists(file_path):
        return False, "Not found"
    
    try:
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            content = f.read()
        
        original = content
        changes = []
        
        # Fix 1: Replace }; with ); in function declarations
        # Match: Type Name(params};
        new_content = re.sub(
            r'(\([^)]*\w+\s+\w*[^)]*)}\s*;',
            r'\1);',
            content
        )
        if new_content != content:
            content = new_content
            changes.append("Fixed }; to );")
        
        # Fix 2: Replace }; with ); at end of lines with function params
        lines = content.split('\n')
        fixed_lines = []
        for i, line in enumerate(lines):
            # Fix lines ending with }; that have function parameters
            if re.search(r'\([^)]*\w+.*}\s*;\s*$', line) and ');' not in line:
                line = re.sub(r'(\([^)]*)}(\s*;\s*)$', r'\1)\2', line)
            fixed_lines.append(line)
        content = '\n'.join(fixed_lines)
        if content != original:
            changes.append("Fixed param lines")
        
        # Fix 3: Fix UENUM with ( instead of proper syntax
        # Some UENUM entries might have ( instead of ,
        new_content = re.sub(
            r'UENUM\([^)]*\)\s*enum class\s+(\w+)\s*:\s*(\w+)\s*{([^}]*)}',
            lambda m: f"UENUM(BlueprintType)\nenum class {m.group(1)}: {m.group(2)} {{" + re.sub(r'(\w+)\s*\(\s*UMETA', r'\1 UMETA', m.group(3)) + "}",
            content
        )
        if new_content != content:
            content = new_content
            changes.append("Fixed UENUM")
        
        if content != original:
            with open(file_path, 'w', encoding='utf-8-sig') as f:
                f.write(content)
            return True, " | ".join(changes)
        return False, "No changes"
        
    except Exception as e:
        return False, f"Error: {e}"

# Get all header files
header_files = glob.glob(r'c:\HW\MingGoRTS\Source\**\*.h', recursive=True)
header_files += glob.glob(r'c:\HW\MingGoRTS\Plugins\**\*.h', recursive=True)

print(f"Scanning {len(header_files)} header files...")
print("=" * 70)

fixed = 0
errors = 0
unchanged = 0

for file_path in header_files:
    is_fixed, msg = batch_fix_file(file_path)
    if is_fixed:
        print(f"[FIXED] {os.path.basename(file_path)}: {msg}")
        fixed += 1
    elif "Error" in msg:
        print(f"[ERROR] {os.path.basename(file_path)}: {msg}")
        errors += 1
    else:
        unchanged += 1

print("=" * 70)
print(f"Summary: {fixed} fixed, {errors} errors, {unchanged} unchanged")
print("Done!")
