#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fix specific remaining errors"""

import re
import os

def fix_specific_file(file_path, fix_type):
    """Fix specific errors based on file"""
    if not os.path.exists(file_path):
        return False, "File not found"
    
    try:
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            content = f.read()
        
        original = content
        changes = []
        
        if fix_type == "eof_struct":
            # Fix struct EOF errors - ensure proper closing
            lines = content.split('\n')
            fixed_lines = []
            brace_count = 0
            for i, line in enumerate(lines):
                brace_count += line.count('{') - line.count('}')
                fixed_lines.append(line)
            
            # If brace count is positive, we're missing closing braces
            if brace_count > 0:
                fixed_lines.extend(['};'] * brace_count)
                changes.append(f"Added {brace_count} missing closing braces")
            
            content = '\n'.join(fixed_lines)
            
        elif fix_type == "uenum":
            # Fix UENUM entries with ( instead of ,
            # Pattern: EnumValue (
            new_content = re.sub(r'(\w+)\s*\(\s*UMETA', r'\1 UMETA', content)
            if new_content != content:
                content = new_content
                changes.append("Fixed UENUM syntax")
            
            # Ensure commas between enum values
            new_content = re.sub(r'(UMETA\([^)]+\))\s*\n\s*(\w+)', r'\1,\n    \2', content)
            if new_content != content:
                content = new_content
                changes.append("Added missing commas")
                
        elif fix_type == "char_constant":
            # Fix unterminated character constants
            # Replace garbled characters in string literals
            new_content = re.sub(r'"[^"]*[\x00-\x08\x0b-\x0c\x0e-\x1f\x80-\xff][^"]*"', '""', content)
            if new_content != content:
                content = new_content
                changes.append("Fixed character constants")
                
        elif fix_type == "function_param":
            # Fix function declarations ending with }; instead of );
            lines = content.split('\n')
            fixed_lines = []
            for i, line in enumerate(lines):
                original_line = line
                # Match function parameters ending with };
                if re.search(r'\([^)]*\w+\s*\w*}\s*;\s*$', line):
                    line = re.sub(r'(\([^)]*)}(\s*;\s*)$', r'\1)\2', line)
                    if original_line != line:
                        changes.append(f"Line {i+1}: Fixed }};")
                fixed_lines.append(line)
            content = '\n'.join(fixed_lines)
        
        if content != original:
            with open(file_path, 'w', encoding='utf-8-sig') as f:
                f.write(content)
            return True, " | ".join(changes)
        else:
            return False, "No changes"
            
    except Exception as e:
        return False, f"Error: {str(e)}"

# Files to fix with their specific error types
files_to_fix = [
    (r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\MingRTSResourceManager.h', 'eof_struct'),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Risk\MingRiskTrendAnalysis.h', 'uenum'),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Security\MingRTSAntiCheat.h', 'char_constant'),
    (r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\Input\MingInputManager.h', 'function_param'),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Security\MingRTSServerValidation.h', 'function_param'),
]

print("Fixing specific remaining errors...")
print("=" * 60)

for file_path, fix_type in files_to_fix:
    if os.path.exists(file_path):
        fixed, message = fix_specific_file(file_path, fix_type)
        status = "[FIXED]" if fixed else "[INFO]"
        print(f"{status} {os.path.basename(file_path)}: {message}")
    else:
        print(f"[SKIP] {os.path.basename(file_path)}: Not found")

print("=" * 60)
print("Done!")
