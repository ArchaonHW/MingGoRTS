#!/usr/bin/env python3
"""
最終綜合修復腳本 - 修復所有剩餘的 UHT 編譯錯誤
"""
import os
import re

def fix_file(filepath):
    """修復單個文件中的所有語法錯誤"""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
    except:
        return False, 0
    
    original = content
    fixes = 0
    
    # 修復1: DECLARE_DYNAMIC_MULTICAST_DELEGATE 雙分號
    content = re.sub(
        r'(DECLARE_DYNAMIC_MULTICAST_DELEGATE[_\w]*\([^)]+\));\s*;',
        r'\1;',
        content
    )
    
    # 修復2: 結構體/枚舉定義後的雙分號
    content = re.sub(r'(\};)\s*;', r'\1', content)
    
    # 修復3: UCLASS 行尾多餘分號
    content = re.sub(r'(UCLASS\([^)]+\));\s*\n', r'\1\n', content)
    
    # 修復4: GENERATED_BODY() 前有多餘內容
    content = re.sub(r'.+GENERATED_BODY\(\)', '    GENERATED_BODY()', content, flags=re.MULTILINE)
    
    # 修復5: UENUM/USTRUCT 定義錯誤
    content = re.sub(r'UENUM\([^)]*\)\s*enum\s+class\s+(\w+)\s*:\s*uint8\s*\(', 
                     r'UENUM(BlueprintType)\nenum class \1 : uint8 {', content)
    content = re.sub(r'enum\s+class\s+(\w+)\s*:\s*uint8\s*\(', 
                     r'enum class \1 : uint8 {', content)
    content = re.sub(r'\);(\s*\n|$)', r'};\1', content)
    
    # 修復6: 括號不匹配 - 函數結尾
    content = re.sub(r'(\w+\s+\w+\([^)]+\))\s*\)\s*;', r'\1;', content)
    
    # 修復7: void 函數結尾多餘 )
    content = re.sub(r'(void\s+\w+\([^)]+\))\s*\)\s*;', r'\1;', content)
    
    # 計算修復數量
    if content != original:
        fixes = 1
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"✓ 修復: {filepath}")
            return True, fixes
        except:
            return False, 0
    return False, 0

def main():
    search_paths = [
        'Source/MingGoRTS',
        'Plugins/MingCore',
        'Plugins/MingPersonal',
        'Plugins/MingStrategic',
        'Plugins/MingTactical',
        'Plugins/MingUI',
        'Plugins/MingAI',
        'Plugins/MingAudio'
    ]
    
    total_fixed = 0
    
    # 特定文件優先修復
    priority_files = [
        'Source/MingGoRTS/Public/Innovation/MingSocialDynamicsSystem.h',
        'Plugins/MingPersonal/Source/MingPersonal/Public/MingQuestManager.h',
        'Plugins/MingAudio/Source/MingAudio/Public/MingAudioSaveData.h',
        'Source/MingGoRTS/Public/Localization/MingRTSCulturalAdaptationSystem.h',
        'Source/MingGoRTS/Public/Stats/MingRTSStatisticsSystem.h',
    ]
    
    # 優先修復特定文件
    for filepath in priority_files:
        if os.path.exists(filepath):
            fixed, count = fix_file(filepath)
            if fixed:
                total_fixed += count
    
    # 遍歷所有文件
    for base_path in search_paths:
        if not os.path.exists(base_path):
            continue
        for root, dirs, files in os.walk(base_path):
            for file in files:
                if file.endswith('.h'):
                    filepath = os.path.join(root, file)
                    fixed, count = fix_file(filepath)
                    if fixed:
                        total_fixed += count
    
    print(f"\n總計修復: {total_fixed} 個文件")

if __name__ == "__main__":
    main()
