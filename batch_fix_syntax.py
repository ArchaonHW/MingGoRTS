#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Batch fix syntax errors - replace }; with ); in function declarations across multiple files"""

import re
import os
import glob

def fix_file_syntax(file_path):
    """Fix }; syntax errors in a single file"""
    if not os.path.exists(file_path):
        return False, "File not found"
    
    try:
        # Read file with utf-8-sig to handle BOM
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            content = f.read()
        
        original_content = content
        changes = []
        
        # Pattern 1: Function declarations ending with }; instead of );
        # Match: Type FunctionName(Params};
        pattern1 = r'(\([^)]*\)};'
        matches1 = re.findall(pattern1, content)
        if matches1:
            content = re.sub(pattern1, r'\1);', content)
            changes.append(f"Fixed {len(matches1)} function declarations")
        
        # Pattern 2: Delegate declarations
        pattern2 = r'(DECLARE_DYNAMIC_MULTICAST_DELEGATE[^}]*\([^)]*\)};'
        matches2 = re.findall(pattern2, content)
        if matches2:
            content = re.sub(pattern2, r'\1);', content)
            changes.append(f"Fixed {len(matches2)} delegate declarations")
        
        if content != original_content:
            with open(file_path, 'w', encoding='utf-8-sig') as f:
                f.write(content)
            return True, " | ".join(changes)
        else:
            return False, "No changes needed"
    except Exception as e:
        return False, f"Error: {str(e)}"

# Find all .h files in the Public directory
header_files = glob.glob(r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\**\*.h', recursive=True)

fixed_count = 0
error_count = 0
no_change_count = 0

print("Scanning and fixing header files...")
print("=" * 60)

for file_path in header_files:
    fixed, message = fix_file_syntax(file_path)
    if fixed:
        print(f"[FIXED] {os.path.basename(file_path)}: {message}")
        fixed_count += 1
    elif "Error" in message:
        print(f"[ERROR] {os.path.basename(file_path)}: {message}")
        error_count += 1
    else:
        no_change_count += 1

print("=" * 60)
print(f"Summary: {fixed_count} files fixed, {error_count} errors, {no_change_count} unchanged")
print("Done!")
