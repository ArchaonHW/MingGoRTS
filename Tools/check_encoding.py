#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
檢查並修復項目源代碼文件中的中文編碼問題
將亂碼註釋轉換為正確的繁體中文或英文
"""

import os
import re
import sys
from pathlib import Path
from typing import List, Tuple

# 定義項目路徑
PROJECT_ROOT = Path("c:/HW/MingGoRTS")
SOURCE_PATHS = [
    PROJECT_ROOT / "Source",
    PROJECT_ROOT / "Plugins",
]

# 文件擴展名
SOURCE_EXTENSIONS = {'.h', '.cpp', '.cs', '.ini', '.uplugin'}

# 常見亂碼模式及其修復映射
# 注意：這些是針對特定編碼損壞的修復規則
GARBLED_PATTERNS = {
    # Building System 常見亂碼
    r'// \?\?X\b': '// 住宅',
    r'// X\?~\b': '// 商業',
    r'// \?u\?~\b': '// 工業',
    r'// \?xX\b': '// 軍事',
    r'// \?A\?~\b': '// 農業',
    r'// X\?\|': '// 教育',
    r'// X\?x\b': '// 倉儲',
    r'// X\?m\b': '// 防禦',
    r'// \?\?\?\?X\b': '// 資源建築',
    r'// X\?s\b': '// 研究',
    r'// \?TX\b': '// 娛樂',
    r'// \?vX\b': '// 宗教',
    r'// \?\?w\?\?v\b': '// 自定義建築類型',
    r'// \?W摧毀\b': '// 規劃中',
    r'// \?\?\]\?\?\b': '// 建造中',
    r'// \?B\?\}\b': '// 運作中',
    r'// \?\?\?l\b': '// 受損',
    r'// \?\?_\?\?\b': '// 維修中',
    r'// 摧毀\b': '// 升級中',
    r'// \?\?\?\b': '// 拆除中',
    r'// \?o\?\?\b': '// 廢棄',
    r'// \?pX\b': '// 小型',
    r'// \?\?X\b': '// 中型',
    r'// \?jX\b': '// 大型',
    r'// \?W\?j\?\?\b': '// 超大型',
}

def find_source_files() -> List[Path]:
    """查找所有源代碼文件"""
    files = []
    for base_path in SOURCE_PATHS:
        if base_path.exists():
            for ext in SOURCE_EXTENSIONS:
                files.extend(base_path.rglob(f"*{ext}"))
    return files

def check_file_encoding_issues(file_path: Path) -> Tuple[bool, List[str]]:
    """
    檢查文件是否包含編碼問題
    返回: (是否有問題, 問題行列表)
    """
    issues = []
    has_issues = False
    
    try:
        # 嘗試用 UTF-8 讀取
        with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
            lines = f.readlines()
    except Exception as e:
        try:
            # 嘗試用系統默認編碼
            with open(file_path, 'r', encoding='mbcs', errors='replace') as f:
                lines = f.readlines()
        except Exception as e2:
            return False, [f"無法讀取文件: {e2}"]
    
    for i, line in enumerate(lines, 1):
        # 檢查常見亂碼模式
        if re.search(r'\?\?X|\?u\?~|\?xX|\?A\?~|X\?~|X\?\||X\?x|\?TX|\?vX|\?W摧毀|\?\?\]\?\?', line):
            issues.append(f"Line {i}: {line.strip()[:80]}")
            has_issues = True
        # 檢查其他亂碼模式
        elif re.search(r'[\?\ufffd][\?\ufffd\w]', line) and '//' in line:
            issues.append(f"Line {i}: {line.strip()[:80]}")
            has_issues = True
    
    return has_issues, issues

def generate_report():
    """生成編碼問題報告"""
    print("=" * 80)
    print("MingGoRTS 項目中文編碼檢查報告")
    print("=" * 80)
    
    files = find_source_files()
    total_files = len(files)
    files_with_issues = []
    
    print(f"\n掃描文件總數: {total_files}")
    print("-" * 80)
    
    for file_path in files:
        has_issues, issues = check_file_encoding_issues(file_path)
        if has_issues:
            files_with_issues.append((file_path, issues))
    
    # 按問題數量排序
    files_with_issues.sort(key=lambda x: len(x[1]), reverse=True)
    
    print(f"\n發現編碼問題的文件數: {len(files_with_issues)}")
    print("=" * 80)
    
    # 顯示最嚴重的 20 個文件
    for i, (file_path, issues) in enumerate(files_with_issues[:20], 1):
        print(f"\n{i}. {file_path}")
        print(f"   問題行數: {len(issues)}")
        for issue in issues[:5]:  # 只顯示前 5 個問題
            print(f"   {issue}")
        if len(issues) > 5:
            print(f"   ... 還有 {len(issues) - 5} 個問題行")
    
    if len(files_with_issues) > 20:
        print(f"\n... 還有 {len(files_with_issues) - 20} 個文件")
    
    # 輸出完整報告到文件
    report_path = PROJECT_ROOT / "EncodingIssuesReport.txt"
    with open(report_path, 'w', encoding='utf-8') as f:
        f.write("=" * 80 + "\n")
        f.write("MingGoRTS 項目中文編碼問題完整報告\n")
        f.write("=" * 80 + "\n\n")
        f.write(f"掃描文件總數: {total_files}\n")
        f.write(f"問題文件數: {len(files_with_issues)}\n\n")
        
        for file_path, issues in files_with_issues:
            f.write(f"\n{'='*80}\n")
            f.write(f"文件: {file_path}\n")
            f.write(f"問題數: {len(issues)}\n")
            f.write("-" * 80 + "\n")
            for issue in issues:
                f.write(f"{issue}\n")
    
    print(f"\n\n完整報告已保存到: {report_path}")
    
    return files_with_issues

if __name__ == "__main__":
    files_with_issues = generate_report()
