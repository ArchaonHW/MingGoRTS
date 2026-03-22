#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
修復項目源代碼文件中的中文編碼亂碼問題
將亂碼註釋轉換為正確的繁體中文或英文
"""

import os
import re
import sys
from pathlib import Path
from typing import Dict, List, Tuple
from datetime import datetime

# 定義項目路徑
PROJECT_ROOT = Path("c:/HW/MingGoRTS")

# 文件擴展名
SOURCE_EXTENSIONS = {'.h', '.cpp', '.cs', '.ini', '.uplugin'}

# ===== 建築系統常見亂碼修復映射 =====
BUILDING_FIXES = {
    # 建築類型
    '// ??X': '// 住宅',
    '// X?~': '// 商業',
    '// ?u?~': '// 工業',
    '// ?xX': '// 軍事',
    '// ?A?~': '// 農業',
    '// X?|': '// 教育',
    '// X': '// 醫療',
    '// X?x': '// 倉儲',
    '// X': '// 電力',
    '// X?m': '// 防禦',
    '// ??X': '// 資源',
    '// X?s': '// 研究',
    '// ?TX': '// 娛樂',
    '// X': '// 政府',
    '// ?vX': '// 宗教',
    '// ??w?q??v': '// 自定義',
    
    # 建築狀態
    '// ?W摧毀': '// 規劃中',
    '// ??]??': '// 建造中',
    '// ?B?}': '// 運作中',
    '// ???l': '// 受損',
    '// ??_??': '// 維修中',
    '// 摧毀': '// 升級中',  
    '// ???': '// 拆除中',
    '// ?o??': '// 廢棄',
    
    # 建築尺寸
    '// ?pX': '// 小型',
    '// ??X': '// 中型',
    '// ?jX': '// 大型',
    '// ?W?j??': '// 超大型',
    
    # 通用註釋
    '// X??X?]?m': '// 建築性能設置',
    '// ??X?t??': '// 建築系統',
    '// ??X??X': '// 建築管理',
    '// ??X???@': '// 建築維護',
    '// ??X??': '// 建築生產',
    '// ?u?H??X': '// 分配工人',
    '// ??X??O': '// 建築模板',
    '// ??X?m': '// 建築佈局',
    '// ??X': '// 連接建築',
    '// ??X??X': '// 建築統計',
    '// ??X???': '// 建築事件',
    '// ?t?V??': '// 內部引用',
    '// ??X': '// 建築',
    '// ?]?m': '// 設置',
    '// ??X??': '// 增強',
    '// X??X': '// 初始化',
    '// ???UX??': '// 內部通知',
    '// X??X??X??': '// 建築優化',
}

# ===== 生產系統常見亂碼修復映射 =====
PRODUCTION_FIXES = {
    '// ??\u0001uUIե': '// 生產線UI組件',
    '//]m??\u0001u\u0001X': '// 設置生產線數據',
    '// ??\u0001s??\u0001??X': '// 更新生產狀態',
    '// ??\u0001s??\u0001??\u0001': '// 更新生產進度',
    '//]m??\u0001??\u0001': '// 設置當前配方',
    '//]m??\u0001_Q\u0001??': '// 設置是否被選中',
    '// ???X?\u0001\u0001uID': '// 獲取生產線ID',
    '// ???X?_Q\u0001??': '// 是否被選中',
    '// UIեޥ': '// UI組件綁定',
    '// ???X?\u0001\u0001u\u0001X': '// 當前生產線',
    '// ???X?\u0001\u0001': '// 當前配方',
    '// ??\u0001_Q\u0001??': '// 是否被選中',
    '// ???X?UI': '// 初始化UI',
    '// ??\u0001sUI\u0001\u0001': '// 更新UI顯示',
    '// ???X??X???': '// 生產線點擊事件',
    '// ???X???\u0001X?': '// 獲取狀態顏色',
    '// ???X???\u0001?X': '// 獲取優先級顏色',
    '//j??ƥ': '// 綁定事件',
    '// ?Xƥ': '// 解綁事件',
    '// ??\u0001\u0001??\u0001?X?\u0001\u0001': '// 生產配方Widget',
    '// ??\u0001\u0001???X???': '// 生產配方列表',
    '// ??\u0001\u0001????UIե': '// 生產隊列UI組件',
    '//]m??\u0001????X???': '// 設置生產隊列',
    '// ??\u0001s???X?\u0001\u0001': '// 更新隊列項目',
    '// \u0001\u0001\u0001???X?\u0001\u0001': '// 移除隊列項目',
    '//M\u0001\u0001????': '// 清除隊列',
    '//]m`\u0001\u0001??': '// 設置循環模式',
    '//]m???X???': '// 設置自動啟動',
    '// ??\u0001\u0001??DUIե': '// 生產管理器UI組件',
    '//]m??\u0001tפޥ': '// 設置生產系統引用',
    '// \u0001\u0001\u0001????\u0001\u0001O???X': '// 顯示配方類別項目',
    '// \u0001\u0001\u0001????X': '// 顯示所有類別',
    '// ??\u0001\u0001??\u0001\u0001u??': '// 選擇生產線',
    '//M\u0001??\u0001\u0001??X': '// 清除選擇',
    '// ??\u0001\u0001???X???': '// 創建新生產線',
    '// \u0001\u0001\u0001??\u0001\u0001\u0001\u0001??': '// 顯示生產統計',
    '//]m??\u0001oX': '// 設置過濾器',
    '//M\u0001??\u0001oX': '// 清除過濾器',
    '// ??\u0001\u0001??UIե': '// 生產統計UI組件',
    '//]m??\u0001\u0001\u0001?X???': '// 設置生產統計數據',
    '// \u0001\u0001\u0008Բ??': '// 顯示詳細統計',
    '// ɥX\u0001?X???': '// 導出統計報告',
}

# ===== 科技樹系統常見亂碼修復映射 =====
TECHTREE_FIXES = {
    '// 科??樹??點UI組件': '// 科技樹節點UI組件',
    '// 設置科??節點數X': '// 設置科技節點數據',
    '// ??新節點?X': '// 更新節點狀態',
    '// ??新??發??度': '// 更新研發進度',
    '// // 设置是否被选中': '// 設置是否被選中',
    '// // 设置节点位置': '// 設置節點位置',
    '// // 获取科技节点ID': '// 獲取科技節點ID',
    '// // 是否被选中': '// 是否被選中',
    '// ???X?否被選??': '// 是否被選中',
    '// UI組件引用': '// UI組件引用',
    '// ????科??節點數X': '// 當前科技節點數據',
    '// ???X?X': '// 當前狀態',
    '// ??否被選??': '// 是否被選中',
    '// 節點????': '// 節點位置',
    '// ???X?UI': '// 初始化UI',
    '// ??新UI顯示': '// 更新UI顯示',
    '// ????點??事件': '// 節點點擊事件',
    '// ???X???事件': '// 節點懸停事件',
    '// ???X??X???事件': '// 節點離開事件',
    '// ???X????X': '// 獲取狀態顏色',
    '// ????類別顏色': '// 獲取類別顏色',
    '// 綁??事件': '// 綁定事件',
    '// ?X事件': '// 解綁事件',
    '// 科??樹????線UI組件': '// 科技樹連接線UI組件',
    '// 設置X????兩??????': '// 設置連接線兩端科技',
    '// 設置X????X': '// 設置連接線狀態',
    '// 設置X??線?X': '// 設置連接線顏色',
    '// ??新X??線????': '// 更新連接線位置',
    '// 源?X?ID': '// 源科技ID',
    '// ????科??ID': '// 目標科技ID',
    '// X????否激??': '// 連接線是否激活',
    '// 科??樹主UI組件': '// 科技樹主UI組件',
    '// 設置科??樹系統?X': '// 設置科技樹系統數據',
    '// 顯示????類別???X???': '// 顯示特定類別科技項目',
    '// 顯示?????X???': '// 顯示所有科技項目',
    '// ??中科??節??': '// 選中科技節點',
    '// 清除??中??X': '// 清除選中狀態',
    '// ???X?發??中???X?': '// 開始研發選中科技',
    '// ??新科??樹顯??': '// 更新科技樹顯示',
    '// 設置??濾X': '// 設置過濾器',
    '// 清除??濾X': '// 清除過濾器',
    '// ???X??X?中???X?': '// 獲取選中科技ID',
    '// ???X???顯示???X': '// 獲取當前顯示類別',
    '// 科??樹系統?X': '// 科技樹系統數據',
    '// ???X?中???X?ID': '// 當前選中科技ID',
    '// ????顯示???X': '// 當前顯示類別',
    '// ???X?濾X': '// 當前過濾器',
    '// 科??節點Widget????': '// 科技節點Widget緩存',
    '// 科?X??Widget????': '// 科技連接Widget緩存',
    '// ??建科??節點Widget': '// 創建科技節點Widget',
    '// ??建X??Widget': '// 創建連接Widget',
    '// ????科??節點?X': '// 點擊科技節點數據',
    '// ????科?X?發????': '// 科技研發開始事件',
    '// ????科?X?發完??': '// 科技研發完成事件',
    '// ????科???X': '// 科技解鎖事件',
    '// ??新??中科??信息': '// 更新選中科技信息',
    '// 佈??科??節??': '// 佈局科技節點',
    '// ??建科?X????': '// 創建科技連接',
    '// ??用??濾X': '// 應用過濾器',
    '// ????節點??局位置': '// 計算節點佈局位置',
    '// 綁??科??樹????': '// 綁定科技樹事件',
    '// ?X科??樹????': '// 解綁科技樹事件',
    '// 清?X???Widget': '// 清除現有Widget',
    '// ??建類別標籤': '// 創建類別標籤',
    '// ??建等?X?濾X': '// 創建等級過濾器',
    '// 科?X?發??板UI組件': '// 科技研發面板UI組件',
    '// 顯示??發??板': '// 顯示研發面板',
    '// ???X?發??板': '// 隱藏研發面板',
    '// ??新活?X?發??表': '// 更新活躍研發列表',
    '// ???X?發': '// 暫停研發',
    '// ???X?發': '// 取消研發',
    '// 設置??發建??': '// 設置研發建築',
    '// ???X?中???X?建??': '// 當前選中建築ID',
    '// ??建??發??目Widget': '// 創建研發項目Widget',
    '// ???X?發??度??新': '// 研發進度更新事件',
    '// ??新統??信息': '// 更新統計信息',
    '// ??新??發建?X?表': '// 更新研發建築列表',
    '// 科??詳?X?板UI組件': '// 科技詳情面板UI組件',
    '// 顯示科??詳??': '// 顯示科技詳情',
    '// ????科??詳??': '// 隱藏科技詳情',
    '// ??建???X?表X': '// 創建效果列表項',
    '// CreatePrerequisiteItem': '// 創建前置條件項',
    '// CreateCostItem': '// 創建成本項',
}

# ===== 通用亂碼修復映射 =====
GENERAL_FIXES = {
    # 常見亂碼字符組合
    '\ufffd\ufffd': '建築',
    '\ufffd\ufffd\ufffd': '生產',
    '\ufffd\ufffd': '科技',
    '\u0001': '',
    '??樹': '技樹',
    '??點': '技節點',
    '??節': '技節',
    '??發': '技研發',
    '??詳': '技詳情',
    '??線': '技線',
    '?X': '數據',
    '?x': '數據',
    '?~': '業',
    '?|': '育',
    '?m': '防禦',
    '?u': '工',
    '?A': '農',
    '?T': '娛',
    '?v': '宗',
    '?H': '商',
    '?B': '運',
    '?W': '規',
    '?p': '小',
    '?j': '大',
    '?o': '廢',
    '?l': '損',
    '?w': '自',
    '?s': '究',
    '摧毀': '進行中',
    '摧毀中': '進行中',
    '??': '進',
}

def fix_file_encoding(file_path: Path) -> Tuple[bool, int, List[str]]:
    """
    修復單個文件的編碼問題
    返回: (是否修改, 修復數量, 修復詳情)
    """
    try:
        # 嘗試用 UTF-8 讀取
        with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
            lines = content.split('\n')
    except Exception as e:
        return False, 0, [f"無法讀取文件: {e}"]
    
    fixed_count = 0
    changes = []
    new_lines = []
    
    for i, line in enumerate(lines, 1):
        original_line = line
        
        # 應用建築系統修復
        for garbled, fixed in BUILDING_FIXES.items():
            if garbled in line:
                line = line.replace(garbled, fixed)
        
        # 應用生產系統修復
        for garbled, fixed in PRODUCTION_FIXES.items():
            if garbled in line:
                line = line.replace(garbled, fixed)
        
        # 應用科技樹系統修復
        for garbled, fixed in TECHTREE_FIXES.items():
            if garbled in line:
                line = line.replace(garbled, fixed)
        
        # 應用通用修復
        for garbled, fixed in GENERAL_FIXES.items():
            if garbled in line:
                line = line.replace(garbled, fixed)
        
        # 修復特定模式
        # 修復 "// ?u?~" -> "// 工業"
        line = re.sub(r'// \?u\?~\b', '// 工業', line)
        # 修復 "// ?x?X" -> "// 軍事"
        line = re.sub(r'// \?x\?X\b', '// 軍事', line)
        # 修復 "// ?A?~" -> "// 農業"
        line = re.sub(r'// \?A\?~\b', '// 農業', line)
        # 修復 "// X?~" -> "// 商業"
        line = re.sub(r'// X\?~\b', '// 商業', line)
        # 修復 "// X?|" -> "// 教育"
        line = re.sub(r'// X\?\|', '// 教育', line)
        # 修復 "// X?x" -> "// 倉儲"
        line = re.sub(r'// X\?x\b', '// 倉儲', line)
        # 修復 "// X?m" -> "// 防禦"
        line = re.sub(r'// X\?m\b', '// 防禦', line)
        # 修復 "// ?TX" -> "// 娛樂"
        line = re.sub(r'// \?TX\b', '// 娛樂', line)
        # 修復 "// ?vX" -> "// 宗教"
        line = re.sub(r'// \?vX\b', '// 宗教', line)
        
        if line != original_line:
            fixed_count += 1
            changes.append(f"Line {i}: {original_line.strip()[:50]}... -> {line.strip()[:50]}...")
        
        new_lines.append(line)
    
    if fixed_count > 0:
        # 寫回文件
        new_content = '\n'.join(new_lines)
        try:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(new_content)
            return True, fixed_count, changes
        except Exception as e:
            return False, 0, [f"無法寫入文件: {e}"]
    
    return False, 0, []

def fix_all_files():
    """修復所有文件的編碼問題"""
    print("=" * 80)
    print("MingGoRTS 項目中文編碼修復工具")
    print("=" * 80)
    
    # 優先修復最嚴重的文件
    priority_files = [
        PROJECT_ROOT / "Plugins/MingCore/Source/MingCore/Public/MingRTSBuildingSystem.h",
        PROJECT_ROOT / "Source/MingGoRTS/Public/MingGoRTSProductionUI.h",
        PROJECT_ROOT / "Source/MingGoRTS/Public/MingGoRTSTechTreeUI.h",
    ]
    
    total_fixed = 0
    files_modified = 0
    
    # 修復優先文件
    print("\n正在修復高優先級文件...")
    for file_path in priority_files:
        if file_path.exists():
            modified, count, changes = fix_file_encoding(file_path)
            if modified:
                files_modified += 1
                total_fixed += count
                print(f"\n✓ {file_path.name} - 修復了 {count} 處")
                for change in changes[:3]:
                    print(f"  {change}")
                if len(changes) > 3:
                    print(f"  ... 還有 {len(changes) - 3} 處修復")
        else:
            print(f"\n✗ 文件不存在: {file_path}")
    
    print("\n" + "=" * 80)
    print(f"修復完成: 修改了 {files_modified} 個文件, 共修復 {total_fixed} 處編碼問題")
    print("=" * 80)
    
    # 生成修復報告
    report_path = PROJECT_ROOT / f"EncodingFixReport_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"
    with open(report_path, 'w', encoding='utf-8') as f:
        f.write("MingGoRTS 中文編碼修復報告\n")
        f.write(f"生成時間: {datetime.now()}\n")
        f.write(f"修復文件數: {files_modified}\n")
        f.write(f"修復問題數: {total_fixed}\n")
    
    print(f"\n修復報告已保存: {report_path}")

if __name__ == "__main__":
    fix_all_files()
