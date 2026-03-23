#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Final comprehensive garbled text fix - Redefine Traditional Chinese/English encoding
"""

import os
import re
import shutil
from pathlib import Path

# Comprehensive pattern mapping for all garbled characters
codec_patterns = {
    # Single character fixes
    "?X?": "務",
    "??": "務", 
    "?X": "務",
    "X?": "務",
    "??": "型",
    "??": "態",
    "??": "標",
    "??": "題",
    "??": "述",
    "??": "類",
    "??": "數",
    "??": "量",
    "??": "度",
    "??": "制",
    "??": "件",
    "??": "務",
    "??": "殺",
    "??": "禦",
    "??": "索",
    "??": "動",
    "??": "領",
    "??": "領",
    "??": "毀",
    "??": "性",
    "??": "擇",
    "??": "件",
    "??": "機",
    "??": "德",
    "??": "略",
    "??": "個",
    "??": "項",
    "??": "權",
    "??": "重",
    "??": "態",
    "??": "進",
    "??": "棄",
    "??": "鎖",
    "??": "集",
    "??": "交",
    "??": "送",
    "??": "防",
    "??": "探",
    "??": "互",
    "??": "生",
    "??": "佔",
    "??": "摧",
    "??": "接",
    "??": "行",
    "??": "已",
    "??": "失",
    "??": "放",
    "??": "鎖",
    "??": "擊",
    "?X?": "池",
    "??": "路",
    "??": "圖",
    "??": "置",
    "??": "接",
    "??": "獲",
    "??": "檢",
    "??": "驗",
    "??": "計",
    "??": "總",
    "??": "用",
    "??": "移",
    "??": "新",
    "??": "查",
    "??": "空",
    "??": "間",
    "??": "隨",
    "??": "價",
    "??": "稀",
    "??": "有",
    "??": "度",
    "??": "顏",
    "??": "色",
    "??": "更",
    "??": "損",
    "??": "壞",
    "??": "整",
    "??": "理",
    "??": "合",
    "??": "併",
    "??": "分",
    "??": "離",
    "??": "算",
    "??": "率",
    "??": "消",
    "??": "耗",
    "??": "添",
    "??": "加",
    "??": "結",
    "??": "果",
    "??": "統",
    "??": "計",
    "??": "保",
    "??": "存",
    "??": "載",
    "??": "入",
    "??": "啟",
    "??": "動",
    "??": "默",
    "??": "認",
    "??": "據",
    "??": "據",
    "??": "據",
    "??": "節",
    "??": "點",
    "??": "管",
    "??": "器",
    "??": "初",
    "??": "始",
    "??": "化",
    "??": "系",
    "??": "受",
    "??": "拒",
    "??": "絕",
    "??": "放",
    "??": "完",
    "??": "成",
    "??": "出",
    "??": "可",
    "??": "活",
    "??": "已",
    "??": "特",
    "??": "定",
    "??": "條",
    "??": "度",
    "??": "解",
    "??": "置",
    "??": "置",
    "??": "前",
    "??": "解",
    "??": "鎖",
    "??": "更",
    "??": "新",
    "??": "處",
    "??": "理",
    "??": "後",
    "??": "體",
    "??": "校",
    "??": "園",
    "??": "導",
    "??": "覽",
    "??": "學",
    "??": "術",
    "??": "紹",
    "??": "生",
    "??": "活",
    "??": "科",
    "??": "研",
    "??": "示",
    "??": "職",
    "??": "業",
    "??": "指",
    "??": "導",
    "??": "歷",
    "??": "史",
    "??": "文",
    "??": "化",
    "??": "動",
    "??": "技",
    "??": "演",
    "??": "互",
    "??": "動",
}

def try_read_file(filepath):
    """Try to read file with multiple encodings"""
    encodings = ['utf-8', 'utf-8-sig', 'gbk', 'big5', 'gb2312', 'gb18030', 'cp950', 'latin1']
    
    for enc in encodings:
        try:
            with open(filepath, 'r', encoding=enc) as f:
                content = f.read()
            return content, enc
        except (UnicodeDecodeError, UnicodeError):
            continue
    return None, None

def fix_garbled_text(content):
    """Fix garbled characters in content"""
    modified = False
    
    # Pattern-based replacements
    for old, new in codec_patterns.items():
        if old in content:
            content = content.replace(old, new)
            modified = True
    
    # Regex patterns for common garbled sequences
    patterns = [
        (r'\?([a-zA-Z])\?', r'\1'),
        (r'\?([\u4e00-\u9fff])\?', r'\1'),
    ]
    
    for pattern, replacement in patterns:
        new_content = re.sub(pattern, replacement, content)
        if new_content != content:
            content = new_content
            modified = True
    
    return content, modified

def process_file(filepath, backup_dir):
    """Process a single file"""
    content, enc = try_read_file(filepath)
    if content is None:
        return False, "encoding_error"
    
    fixed_content, modified = fix_garbled_text(content)
    
    if modified:
        # Backup original
        filename = os.path.basename(filepath)
        backup_path = os.path.join(backup_dir, filename + ".bak")
        try:
            shutil.copy2(filepath, backup_path)
        except:
            pass
        
        # Write fixed content
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(fixed_content)
        
        return True, "fixed"
    
    return False, "no_change"

def main():
    root = r"C:\HW\MingGoRTS"
    backup_dir = os.path.join(root, "Backup_Final_GarbledFix")
    os.makedirs(backup_dir, exist_ok=True)
    
    # Find all .h and .cpp files
    source_files = []
    for ext in ['*.h', '*.cpp']:
        source_files.extend(Path(root).rglob(ext))
    
    total = len(source_files)
    fixed = 0
    errors = 0
    no_change = 0
    
    print(f"Processing {total} source files...")
    print("=" * 60)
    
    for i, fp in enumerate(source_files, 1):
        if i % 100 == 0:
            print(f"  Progress: {i}/{total} ({i/total*100:.1f}%)")
        
        try:
            was_fixed, status = process_file(str(fp), backup_dir)
            if was_fixed:
                fixed += 1
                print(f"  Fixed: {fp.name}")
            elif status == "encoding_error":
                errors += 1
            else:
                no_change += 1
        except Exception as e:
            errors += 1
            print(f"  Error: {fp} - {e}")
    
    print("=" * 60)
    print(f"Complete!")
    print(f"  Total: {total}")
    print(f"  Fixed: {fixed}")
    print(f"  No change: {no_change}")
    print(f"  Errors: {errors}")
    print(f"Backup: {backup_dir}")

if __name__ == "__main__":
    main()
