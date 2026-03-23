#!/usr/bin/env python3
"""
MingGoRTS Comprehensive Syntax Auto-Fix Script
Fixes brace mismatches, missing GENERATED_BODY, and missing include guards
"""

import os
import re
import sys
from pathlib import Path

def fix_file_syntax(filepath):
    """Fix syntax issues in a single file"""
    issues_fixed = []
    
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        original = content
    except Exception as e:
        return [(filepath, f"Cannot read: {e}")]
    
    # Fix 1: Add missing GENERATED_BODY() after UCLASS if not present
    # Pattern: UCLASS(...) followed by class declaration without GENERATED_BODY
    uclass_pattern = r'(UCLASS\([^)]*\)\s*\n\s*class\s+\w+)'
    
    def add_generated_body(match):
        return match.group(1) + '\n{\n\tGENERATED_BODY()'
    
    # Only add if GENERATED_BODY is not already present
    if 'UCLASS' in content and 'GENERATED_BODY' not in content:
        content = re.sub(uclass_pattern + r'\s*\n\s*\{', add_generated_body, content)
        if 'GENERATED_BODY' in content and 'GENERATED_BODY' not in original:
            issues_fixed.append("Added GENERATED_BODY()")
    
    # Fix 2: Add missing #pragma once at beginning of header files
    if filepath.endswith('.h'):
        if '#pragma once' not in content and '#ifndef' not in content:
            content = '#pragma once\n\n' + content
            issues_fixed.append("Added #pragma once")
    
    # Fix 3: Fix brace mismatches - add missing closing braces at EOF
    open_count = content.count('{')
    close_count = content.count('}')
    
    if open_count > close_count:
        # Check if file ends properly
        stripped = content.rstrip()
        if not stripped.endswith('}'):
            # Add missing closing braces
            diff = open_count - close_count
            # Check if last line has content that might be cut off
            lines = content.split('\n')
            last_non_empty = None
            for line in reversed(lines):
                if line.strip():
                    last_non_empty = line
                    break
            
            if last_non_empty and not last_non_empty.strip().endswith('}'):
                # Add closing braces
                content = content.rstrip() + '\n' + '\n'.join(['}' for _ in range(diff)]) + '\n'
                issues_fixed.append(f"Added {diff} missing closing brace(s)")
    
    # Fix 4: Fix extra closing braces (more } than {)
    if close_count > open_count:
        diff = close_count - open_count
        # Remove extra closing braces at end of file
        lines = content.split('\n')
        new_lines = []
        removed = 0
        for line in reversed(lines):
            if removed < diff and line.strip() == '}':
                removed += 1
                continue
            new_lines.insert(0, line)
        
        if removed > 0:
            content = '\n'.join(new_lines)
            issues_fixed.append(f"Removed {removed} extra closing brace(s)")
    
    # Fix 5: Fix class declarations that don't end with semicolon before closing brace
    # Pattern: }; should end class declarations
    # This is usually correct in UE, so we'll skip this fix
    
    # Write back if changes made
    if content != original and issues_fixed:
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            return [(filepath, ', '.join(issues_fixed))]
        except Exception as e:
            return [(filepath, f"Cannot write: {e}")]
    
    return []

def scan_and_fix(directory):
    """Scan directory and fix all files"""
    all_fixes = []
    files_checked = 0
    files_fixed = 0
    
    print(f"Scanning and fixing: {directory}")
    print("=" * 70)
    
    for root, dirs, files in os.walk(directory):
        # Skip build directories
        dirs[:] = [d for d in dirs if d not in ['Intermediate', 'Build', 'Binaries', '.git']]
        
        for file in files:
            if file.endswith(('.h', '.hpp', '.cpp')):
                filepath = os.path.join(root, file)
                files_checked += 1
                
                fixes = fix_file_syntax(filepath)
                if fixes:
                    all_fixes.extend(fixes)
                    files_fixed += 1
                    print(f"✓ Fixed {file}: {fixes[0][1]}")
    
    return all_fixes, files_checked, files_fixed

def main():
    base_path = r'c:\HW\MingGoRTS'
    
    # Fix Plugins
    plugins_path = os.path.join(base_path, 'Plugins')
    fixes1, checked1, fixed1 = scan_and_fix(plugins_path)
    
    # Fix Source
    source_path = os.path.join(base_path, 'Source')
    fixes2, checked2, fixed2 = scan_and_fix(source_path)
    
    total_checked = checked1 + checked2
    total_fixed = fixed1 + fixed2
    all_fixes = fixes1 + fixes2
    
    print(f"\n{'='*70}")
    print(f"COMPREHENSIVE FIX COMPLETE")
    print(f"{'='*70}")
    print(f"Files checked: {total_checked}")
    print(f"Files fixed: {total_fixed}")
    print(f"Total fixes: {len(all_fixes)}")
    
    if all_fixes:
        print(f"\nFixes applied:")
        for filepath, fix in all_fixes[:50]:  # Show first 50
            print(f"  {os.path.basename(filepath)}: {fix}")
        if len(all_fixes) > 50:
            print(f"  ... and {len(all_fixes) - 50} more")
    
    return 0

if __name__ == '__main__':
    sys.exit(main())
