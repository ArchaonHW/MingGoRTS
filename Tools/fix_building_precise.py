#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
精確修復建築系統文件中的剩餘亂碼問題
"""

from pathlib import Path

FILE_PATH = Path("c:/HW/MingGoRTS/Plugins/MingCore/Source/MingCore/Public/MingRTSBuildingSystem.h")

def fix_building_system():
    with open(FILE_PATH, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    
    # 精確修復映射
    fixes = [
        # 建築類型修復
        ('Medical,         // 政府', 'Medical,         // 醫療'),
        ('Storage,         // 政府數據', 'Storage,         // 倉儲'),
        ('Power,           // 政府', 'Power,           // 電力'),
        ('Defense,         // 政府防禦', 'Defense,         // 防禦'),
        ('Resource,        // 建築', 'Resource,        // 資源'),
        ('Research,        // 政府究', 'Research,        // 研究'),
        ('Residential,     // 建築', 'Residential,     // 住宅'),
        
        # 建築狀態修復
        ('Operational,     // 運?}', 'Operational,     // 運作中'),
        ('Demolishing,     // ?科技進', 'Demolishing,     // 拆除中'),
        ('Abandoned,       // 廢棄', 'Abandoned,       // 廢棄'),
        ('Destroyed        // 升級中', 'Destroyed        // 已摧毀'),
        
        # 建築尺寸修復
        ('Medium,          // 建築', 'Medium,          // 中型'),
        
        # 通用註釋修復
        ('// X??X?]?m', '// 建築性能設置'),
        ('// ??X?t??', '// 建築系統'),
        ('// ??X??X', '// 建築管理'),
        ('// ??X???@', '// 建築維護'),
        ('// ??X??', '// 建築生產'),
        ('// ?u?H??X', '// 分配工人'),
        ('// ??X??O', '// 建築模板'),
        ('// ??X?m', '// 建築佈局'),
        ('// ??X', '// 連接建築'),
        ('// ??X??X', '// 建築統計'),
        ('// ??X???', '// 建築事件'),
        ('// ?t?V??', '// 內部引用'),
        ('// ??X', '// 建築數據'),
        ('// ?]?m', '// 設置'),
        ('// ??X??', '// 增強功能'),
        ('// X??X', '// 初始化'),
        ('// ???UX??', '// 內部通知'),
        ('// X??X??X??', '// 建築優化'),
    ]
    
    original = content
    for old, new in fixes:
        content = content.replace(old, new)
    
    if content != original:
        with open(FILE_PATH, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"✓ 已修復 {FILE_PATH.name}")
        return True
    return False

if __name__ == "__main__":
    fix_building_system()
