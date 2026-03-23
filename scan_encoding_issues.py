#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
批量修复所有头文件编码问题
"""

import os
import re

def fix_encoding_issues(directory):
    """递归修复目录中的所有.h文件"""
    fixed_count = 0
    error_count = 0
    
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.h'):
                filepath = os.path.join(root, file)
                try:
                    # 尝试读取文件
                    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
                        content = f.read()
                    
                    # 检查是否有编码问题（出字重复）
                    if '出' in content and content.count('出') > 10:
                        print(f"Found encoding issues: {filepath}")
                        # 这里我们标记需要修复，但实际修复需要重新生成文件
                        # 由于无法自动恢复原始内容，我们记录这些文件
                        error_count += 1
                except Exception as e:
                    print(f"Error reading {filepath}: {e}")
    
    return fixed_count, error_count

def main():
    source_dir = r'C:\HW\MingGoRTS\Source'
    plugins_dir = r'C:\HW\MingGoRTS\Plugins'
    
    print("Scanning for encoding issues...")
    print("=" * 60)
    
    fixed1, errors1 = fix_encoding_issues(source_dir)
    fixed2, errors2 = fix_encoding_issues(plugins_dir)
    
    total_fixed = fixed1 + fixed2
    total_errors = errors1 + errors2
    
    print("=" * 60)
    print(f"Scan complete!")
    print(f"Files with encoding issues: {total_errors}")
    
    return 0

if __name__ == '__main__':
    exit(main())
