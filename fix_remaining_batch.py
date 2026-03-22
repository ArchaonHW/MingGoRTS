#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Final batch fix for remaining compilation errors"""

import re
import os

def fix_file(file_path, fix_types):
    """Apply multiple fixes to a file"""
    if not os.path.exists(file_path):
        return False, "Not found"
    
    try:
        with open(file_path, 'r', encoding='utf-8-sig', errors='ignore') as f:
            content = f.read()
        
        original = content
        changes = []
        
        for fix_type in fix_types:
            if fix_type == "uenum_paren":
                # Fix UENUM with ( instead of {
                content = re.sub(
                    r'UENUM\([^)]*\)\s*enum class\s+(\w+)\s*:\s*\w+\s*\(',
                    r'UENUM(BlueprintType)\nenum class \1 : uint8 {',
                    content
                )
                # Add commas between enum entries
                content = re.sub(
                    r'(\w+)\s*\(\s*UMETA\(([^)]+)\)\s*\)\s*\n\s*(\w+)',
                    r'\1(UMETA(\2)),\n    \3',
                    content
                )
                changes.append("Fixed UENUM")
                
            elif fix_type == "eof_class":
                if not content.rstrip().endswith('};'):
                    content = content.rstrip() + '\n};\n'
                    changes.append("Added };")
                    
            elif fix_type == "eof_paren":
                lines = content.split('\n')
                if lines and not lines[-1].strip().endswith(');'):
                    lines.append(');')
                    content = '\n'.join(lines)
                    changes.append("Added );")
                    
            elif fix_type == "blueprint_private":
                lines = content.split('\n')
                fixed_lines = []
                in_private = False
                private_line_idx = -1
                has_blueprint = False
                
                for i, line in enumerate(lines):
                    if 'private:' in line.lower():
                        in_private = True
                        private_line_idx = i
                        has_blueprint = False
                    elif 'protected:' in line.lower() or 'public:' in line.lower():
                        if in_private and has_blueprint and private_line_idx >= 0:
                            fixed_lines[private_line_idx] = lines[private_line_idx].replace('private:', 'protected:').replace('Private:', 'protected:')
                        in_private = False
                        has_blueprint = False
                        private_line_idx = -1
                    
                    if in_private and 'BlueprintReadOnly' in line:
                        has_blueprint = True
                    
                    fixed_lines.append(line)
                
                if in_private and has_blueprint and private_line_idx >= 0:
                    fixed_lines[private_line_idx] = fixed_lines[private_line_idx].replace('private:', 'protected:')
                
                content = '\n'.join(fixed_lines)
                if content != original:
                    changes.append("Changed private to protected")
                    
            elif fix_type == "endif":
                if_count = content.count('#if') + content.count('#ifdef') + content.count('#ifndef')
                endif_count = content.count('#endif')
                if if_count > endif_count:
                    content += '\n#endif\n' * (if_count - endif_count)
                    changes.append(f"Added {if_count - endif_count} #endif")
                    
            elif fix_type == "generated_header":
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
                    
            elif fix_type == "char_constant":
                content = re.sub(
                    r'"[^"]*[\x00-\x08\x0b-\x0c\x0e-\x1f\x80-\xff][^"]*"',
                    '""',
                    content
                )
                changes.append("Fixed char constants")
                
            elif fix_type == "function_param":
                content = re.sub(r'\(\s*\}\s*;', '();', content)
                lines = content.split('\n')
                fixed_lines = []
                for line in lines:
                    if re.search(r'\([^)]*\w+\s*\w*}\s*;\s*$', line):
                        line = re.sub(r'(\([^)]*)}(\s*;\s*)$', r'\1)\2', line)
                    fixed_lines.append(line)
                content = '\n'.join(fixed_lines)
                changes.append("Fixed function params")
        
        if content != original:
            with open(file_path, 'w', encoding='utf-8-sig') as f:
                f.write(content)
            return True, " | ".join(changes)
        return False, "No changes"
        
    except Exception as e:
        return False, f"Error: {e}"

# Files to fix
files_to_fix = [
    # UENUM errors
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSSkillManager.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSSkillSystem.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingUnitController.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\SelfLearning\MingRTSAILearningIntegration.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\AI\MingCombatAIManager.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Stats\MingRTSStatisticsSystem.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Cloud\MingCloudAIComputingIntegration.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Cloud\MingCloudSaveSyncSystem.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Innovation\MingRTSEmotionalAIEngine.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Innovation\MingRTSEvolutionaryAISystem.h', ['uenum_paren']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Developer\MingRTSDevelopmentTaskOptimizer.h', ['uenum_paren']),
    
    # EOF errors
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSAIFilmGenerator.h', ['eof_paren']),
    (r'c:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingUnitBlueprintLibrary.h', ['eof_class']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSTechTreeUI.h', ['eof_class']),
    
    # BlueprintReadOnly on private
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\AI\AIContentQualityController.h', ['blueprint_private']),
    
    # Unmatched #endif
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSUnit.h', ['endif']),
    
    # Generated header position
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Services\MingRTSServiceRegistry.h', ['generated_header']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\FileSystem\MingRTSVirtualFileSystem.h', ['generated_header']),
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\Memory\MingRTSMemoryManager.h', ['generated_header']),
    
    # Char constant
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\GameMode\OneClick\MyOneClickGameMode.h', ['char_constant']),
    
    # Function param
    (r'c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSTechTreeUI.h', ['function_param']),
]

print("Final batch fix...")
print("=" * 70)

fixed_count = 0
for file_path, fix_types in files_to_fix:
    if os.path.exists(file_path):
        fixed, msg = fix_file(file_path, fix_types)
        status = "[FIXED]" if fixed else "[INFO]"
        print(f"{status} {os.path.basename(file_path)}: {msg}")
        if fixed:
            fixed_count += 1
    else:
        print(f"[SKIP] {os.path.basename(file_path)}: Not found")

print("=" * 70)
print(f"Fixed {fixed_count} files")
print("Done!")
