#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fix remaining compilation errors"""

import re
import os

def fix_file(file_path, fix_type):
    """Fix specific errors in a file"""
    if not os.path.exists(file_path):
        return False, "File not found"
    
    try:
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            content = f.read()
        
        original = content
        changes = []
        
        if fix_type == "eof_class":
            # Count and fix missing closing braces for class
            lines = content.split('\n')
            fixed_lines = []
            brace_count = 0
            for line in lines:
                brace_count += line.count('{') - line.count('}')
                fixed_lines.append(line)
            
            # Add missing closing braces
            if brace_count > 0:
                fixed_lines.extend(['};'] * brace_count)
                changes.append(f"Added {brace_count} closing braces")
            content = '\n'.join(fixed_lines)
            
        elif fix_type == "function_param":
            # Fix function declarations ending with }; instead of );
            lines = content.split('\n')
            fixed_lines = []
            for i, line in enumerate(lines):
                orig = line
                if re.search(r'\([^)]*\w+\s*\w*}\s*;\s*$', line):
                    line = re.sub(r'(\([^)]*)}(\s*;\s*)$', r'\1)\2', line)
                    if orig != line:
                        changes.append(f"Line {i+1}")
                fixed_lines.append(line)
            content = '\n'.join(fixed_lines)
            
        elif fix_type == "endif":
            # Fix unmatched #endif by counting #if and #endif
            if_count = content.count('#if') + content.count('#ifdef') + content.count('#ifndef')
            endif_count = content.count('#endif')
            if if_count > endif_count:
                content += '\n' + '#endif\n' * (if_count - endif_count)
                changes.append(f"Added {if_count - endif_count} #endif")
                
        elif fix_type == "uenum":
            # Fix UENUM entries with ( instead of ,
            new_content = re.sub(r'(\w+)\s*\(\s*UMETA', r'\1 UMETA', content)
            if new_content != content:
                content = new_content
                changes.append("Fixed UENUM syntax")
        
        if content != original:
            with open(file_path, 'w', encoding='utf-8-sig') as f:
                f.write(content)
            return True, " | ".join(changes) if changes else "Fixed"
        else:
            return False, "No changes"
            
    except Exception as e:
        return False, f"Error: {str(e)}"

# Files to fix
files_to_fix = [
    (r'c:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\MingRTSUnitSelector.h', 'eof_class'),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\VRAR\MingRTSARSupport.h', 'function_param'),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\SelfLearning\MingRTSSelfLearningSystemTest.h', 'endif'),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSTechTreeUI.h', 'function_param'),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSAIFilmGenerator.h', 'function_param'),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Stats\MingRTSStatisticsSystem.h', 'uenum'),
]

print("Fixing remaining errors...")
print("=" * 60)

for file_path, fix_type in files_to_fix:
    if os.path.exists(file_path):
        fixed, message = fix_file(file_path, fix_type)
        status = "[FIXED]" if fixed else "[INFO]"
        print(f"{status} {os.path.basename(file_path)}: {message}")
    else:
        print(f"[SKIP] {os.path.basename(file_path)}: Not found")

print("=" * 60)
print("Done!")
