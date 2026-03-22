#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fix EOF errors in plugin header files"""

import os

files_to_check = [
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\MingCharacterGrowthSystem.h',
    r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\MingCoreEventBus.h'
]

for file_path in files_to_check:
    if not os.path.exists(file_path):
        print(f"File not found: {file_path}")
        continue
    
    try:
        # Read file
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            content = f.read()
        
        # Count braces and parentheses
        open_braces = content.count('{')
        close_braces = content.count('}')
        open_parens = content.count('(')
        close_parens = content.count(')')
        
        print(f"\n{os.path.basename(file_path)}:")
        print(f"  Open braces: {open_braces}, Close braces: {close_braces}")
        print(f"  Open parens: {open_parens}, Close parens: {close_parens}")
        
        # Check if file ends properly
        if not content.rstrip().endswith('};'):
            print(f"  Warning: File doesn't end with }};")
            # Check last 5 lines
            lines = content.split('\n')
            print("  Last 5 lines:")
            for i, line in enumerate(lines[-5:], len(lines)-4):
                print(f"    {i}: {repr(line)}")
        else:
            print(f"  File ends correctly")
            
    except Exception as e:
        print(f"Error reading {file_path}: {e}")
