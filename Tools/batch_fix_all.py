#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
批量修復所有剩餘文件的亂碼註釋
"""

from pathlib import Path
import re

# 項目根目錄
PROJECT_ROOT = Path("c:/HW/MingGoRTS")

# 完整的亂碼修復映射 - 基於掃描報告分析的常見模式
ENCODING_FIXES = {
    # 常見亂碼字符映射
    '摧毀': '',  # 刪除這個多餘的亂碼字符
    '?X': '作',  # 常見亂碼組合
    '??X': '系統',  # 常見亂碼組合
    '??': '中',  # 常見亂碼組合
    '?m': '置',
    '?t': '建',
    '?d': '度',
    '?z': '值',
    '?v': '值',
    '?N': '的',
    '?N': '的',
    '?H': '入',
    '?H': '入',
    '?u': '處',
    '?U': '處',
    '?r': '文',
    '?g': '功',
    '?~': '商',
    '?y': '用',
    '?w': '文',
    '?O': '是',
    '?O': '是',
    '?~?': '品',
    '': '出',
    'j': '大',
    '~': '年',
    'W': '上',
    '_': '下',
    'N': '的',
    'v': '正',
    'u': '使',
    'g': '成',
    'r': '本',
    'H': '入',
    '|': '出',
    'ɥ': '基本',
    'H': '輸入',
    '?': '輸',
    '?': '輸出',
    '?X?': '作時間',
    '?X?z': '作狀態',
    '?X???': '作說明',
    '??z': '狀態',
    '?X?X': '作類',
    '?X?': '作類型',
    '???': '出錯誤',
    '???X': '錯誤',
    '??ID': '的ID',
    '?HX': '類型',
    '???X': '的作',
    '?X?N': '作的',
    'm??': '置信',
    'F': '軍',
    '`': '軍事',
    'g?': '成率',
    '~?': '年度',
    'u?': '使用率',
    '?y?': '用率',
    'j?': '大小',
    'ƥ': '基礎',
    'W': '基本',
    '?': '基本',
    '??': '的屬',
    '??': '的配',
    '???': '的錯',
    '??_': '的下',
    '??d': '的速',
    '??': '的配',
    '???X?H': '作需要',
    '?z???': '態描述',
    '???X': '統的',
    '???X': '類的',
    '???': '信息',
    '??': '描述',
    '?X?': '類型',
    '?X?X': '類作',
    '?X?X': '作分',
    '?X???': '作分類',
    'D??': '名稱',
    'H': '輸入',
    'g?X': '成功',
    'm': '設置',
    'v?': '正值',
    '': '是否',
    '': '是否',
    'v??': '正值',
    '??': '出錯',
    '?X??': '作為',
    '': '出',
    '?X': '作',
    '??': '系',
    '?m': '置',
}

def fix_file_encoding(file_path: Path) -> int:
    """
    修復單個文件的編碼問題
    返回修復的行數
    """
    try:
        with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
    except Exception as e:
        print(f"  ✗ 無法讀取 {file_path.name}: {e}")
        return 0
    
    original = content
    fix_count = 0
    
    # 應用所有修復
    for old_pattern, new_pattern in ENCODING_FIXES.items():
        if old_pattern in content:
            # 計算修復次數
            fix_count += content.count(old_pattern)
            content = content.replace(old_pattern, new_pattern)
    
    # 處理特殊情況：純亂碼註釋行
    lines = content.split('\n')
    fixed_lines = []
    for line in lines:
        original_line = line
        # 如果行包含亂碼註釋，嘗試修復
        if '//' in line:
            parts = line.split('//', 1)
            if len(parts) == 2:
                code_part = parts[0]
                comment_part = parts[1]
                
                # 如果註釋部分包含亂碼字符
                if any(c in comment_part for c in ['?', '', '摧毀']):
                    # 嘗試修復
                    fixed_comment = comment_part
                    for old, new in ENCODING_FIXES.items():
                        fixed_comment = fixed_comment.replace(old, new)
                    line = code_part + '//' + fixed_comment
                    if fixed_comment != comment_part:
                        fix_count += 1
        
        fixed_lines.append(line)
    
    content = '\n'.join(fixed_lines)
    
    if content != original:
        try:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            return fix_count
        except Exception as e:
            print(f"  ✗ 無法寫入 {file_path.name}: {e}")
            return 0
    
    return 0

def batch_fix_all_files():
    """批量修復所有文件"""
    # 掃描項目中的所有 .h, .cpp, .cs 文件
    source_dirs = [
        PROJECT_ROOT / "Source",
        PROJECT_ROOT / "Plugins"
    ]
    
    total_files = 0
    total_fixed = 0
    
    for source_dir in source_dirs:
        if not source_dir.exists():
            continue
            
        for ext in ['*.h', '*.cpp', '*.cs']:
            for file_path in source_dir.rglob(ext):
                # 檢查文件是否可能包含亂碼
                try:
                    with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
                        content = f.read()
                        # 如果包含亂碼字符
                        if any(c in content for c in ['?', '', '摧毀']):
                            total_files += 1
                            fixed = fix_file_encoding(file_path)
                            if fixed > 0:
                                total_fixed += 1
                                print(f"✓ 已修復 {file_path.name} ({fixed} 處)")
                            else:
                                print(f"  - 無需修復 {file_path.name}")
                except Exception as e:
                    print(f"  ✗ 處理 {file_path.name} 時出錯: {e}")
    
    print(f"\n========================================")
    print(f"批量修復完成")
    print(f"檢查文件數: {total_files}")
    print(f"成功修復文件數: {total_fixed}")
    print(f"========================================")

if __name__ == "__main__":
    print("開始批量修復所有亂碼註釋...")
    batch_fix_all_files()
