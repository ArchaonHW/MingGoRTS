#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
修复所有 Build.cs 文件编码问题
"""

import os

def fix_build_cs_file(filepath):
    """修复单个 Build.cs 文件"""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # 检查是否有编码问题
        if '出' not in content:
            return True, "No encoding issues"
        
        # 由于无法恢复原始内容，我们需要重新生成标准的 Build.cs 内容
        # 提取模块名称
        filename = os.path.basename(filepath)
        module_name = filename.replace('.Build.cs', '')
        
        # 生成标准内容
        new_content = f'''using UnrealBuildTool;
using System.IO;

public class {module_name} : ModuleRules
{{
    public {module_name}(ReadOnlyTargetRules Target) : base(Target)
    {{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {{
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore"
            }}
        );
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {{
                "Projects",
                "Slate",
                "SlateCore"
            }}
        );
    }}
}}
'''
        
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        
        return True, f"Fixed: {filepath}"
    except Exception as e:
        return False, f"Error: {e}"

def find_and_fix_build_cs_files(directory):
    """递归查找并修复所有 Build.cs 文件"""
    fixed_count = 0
    error_count = 0
    
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.Build.cs'):
                filepath = os.path.join(root, file)
                success, msg = fix_build_cs_file(filepath)
                if success:
                    if "No encoding" not in msg:
                        print(f"Fixed: {filepath}")
                        fixed_count += 1
                else:
                    print(f"Error fixing {filepath}: {msg}")
                    error_count += 1
    
    return fixed_count, error_count

def main():
    """主函数"""
    plugins_dir = r'C:\HW\MingGoRTS\Plugins'
    
    print("Fixing Build.cs encoding issues...")
    print("=" * 60)
    
    fixed, errors = find_and_fix_build_cs_files(plugins_dir)
    
    print("=" * 60)
    print(f"Fixed: {fixed} files")
    print(f"Errors: {errors} files")
    
    return 0 if errors == 0 else 1

if __name__ == '__main__':
    exit(main())
