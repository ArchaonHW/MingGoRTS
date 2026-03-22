#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fix syntax errors in header files - replace }; with ); in function declarations"""

import re
import os

# Files to fix with their specific patterns
files_to_fix = [
    r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Security\MingRTSServerValidation.h',
]

for file_path in files_to_fix:
    if not os.path.exists(file_path):
        print(f"File not found: {file_path}")
        continue
    
    # Read file with utf-8-sig to handle BOM
    with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
        content = f.read()
    
    original_content = content
    
    # Replace }; with ); in function declarations
    # Pattern: function declaration ending with }; instead of );
    content = re.sub(r'(\([^)]*\)};', r'\1);', content)
    
    if content != original_content:
        with open(file_path, 'w', encoding='utf-8-sig') as f:
            f.write(content)
        print(f"Fixed: {file_path}")
    else:
        print(f"No changes needed: {file_path}")

print("\nDone!")
