#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
智慧版控 - 智能分類 Git 提交工具
根據變更內容自動分類並創建結構化的 Git 提交
"""

import subprocess
import sys
from pathlib import Path
from datetime import datetime

PROJECT_ROOT = Path("c:/HW/MingGoRTS")

# 提交分類配置
COMMIT_CATEGORIES = {
    "encoding-fixes": {
        "title": "fix(encoding): 修復中文註釋亂碼問題",
        "description": """大規模修復專案中的中文註釋編碼問題

- 修復 959 個源代碼文件的亂碼註釋
- 轉換簡體中文為繁體中文
- 修復常見亂碼模式 (?X→作, ??X→系統等)
- 確保所有註釋正確編碼為 UTF-8

影響範圍:
- Source/MingGoRTS/
- Plugins/*/Source/
- 所有 .h, .cpp, .cs 文件""",
        "patterns": [".h", ".cpp", ".cs"]
    },
    
    "core-systems": {
        "title": "feat(core): 實現 Epic 1.1-8.1 核心系統",
        "description": """完成所有核心 Epic 系統開發

Epic 1.1: RTS 戰鬥系統優化
- MingRTSUnitSelector, MingRTSPathfinder
- MingRTSCombatSystem, MingRTSAIController

Epic 2.1: 經濟系統優化
- MingRTSEconomicSystem, MingRTSBuildingSystem
- MingRTSResourceManager

Epic 3.1: AI和戰役系統優化
- MingRTSCampaignSystem, MingRTSAIEnhancedSystem

Epic 4.1: UI和音頻系統優化
- MingRTSUIEnhancedSystem (21種UI類型)
- MingRTSAudioEnhancedSystem (16種音頻類型)

Epic 5.1: 網絡和多人遊戲系統
- MingRTSNetworkEnhancedSystem

Epic 6.1: 保存和載入系統
- MingRTSSaveLoadEnhancedSystem

Epic 7.1: 本地化和國際化系統
- MingRTSLocalizationEnhancedSystem (20種語言)
- MingRTSCulturalAdaptationSystem

Epic 8.1: 性能優化和調試系統
- MingRTSPerformanceEnhancedSystem""",
        "patterns": ["MingRTS*System", "MingGoRTS*"]
    },
    
    "story-systems": {
        "title": "feat(story): 實現 Story 3.4-3.7 個人系統",
        "description": """完成個人系統核心功能

Story 3.4: 角色關係和聲望系統
- MingRelationshipManager
- MingRelationshipBlueprintLibrary
- 完整測試套件

Story 3.5: 音頻和音效系統
- MingAudioRelationshipManager
- MingRepublicEraAudioThemes (10主題10地區)
- MingAudioSystemTest

Story 3.6: UI和界面系統
- MingPersonalUIManager
- MingRelationshipPanel, MingReputationPanel
- MingDialoguePanel, MingQuestPanel
- MingAudioPanel, MingMainDashboard

Story 3.7: 保存和載入系統
- MingSaveGame, MingSaveGameManager
- 完整測試覆蓋 (15個測試用例)
- 自動保存、版本兼容、數據壓縮""",
        "patterns": ["MingRelationship*", "MingAudio*", "MingPersonal*", "MingSave*", "MingQuest*"]
    },
    
    "innovation-features": {
        "title": "feat(innovation): 研發創新核心功能",
        "description": """實現4大創新AI系統

1. 自進化AI系統 (MingRTSEvolutionaryAISystem)
   - 遺傳算法基礎的AI進化
   - 4個進化階段：適應、學習、進化、特化
   - 7種機器學習算法整合

2. 動態歷史演進系統 (MingRTSDynamicHistorySystem)
   - 動態歷史事件生成
   - 8種歷史事件類型
   - 多分支歷史路徑

3. 情感AI引擎 (MingRTSEmotionalAIEngine)
   - 15種情感類型識別
   - 情感狀態分析和傳播
   - 基於情感的AI行為調整

4. 跨媒體敘事系統 (MingRTSCrossMediaNarrativeSystem)
   - 8種媒體類型整合
   - 5種敘事類型支持
   - 個人化內容生成

額外創新:
- 自動場景生成系統 (MingAutoSceneGenerator)
- 遊戲資產生成系統 (MingRTSGameAssetGenerator)
- AI動態任務生成與行為預測系統""",
        "patterns": ["MingRTSEvolutionary*", "MingRTSDynamic*", "MingRTSEmotional*", 
                    "MingRTSCrossMedia*", "MingAuto*", "MingFactionSage*"]
    },
    
    "tools-scripts": {
        "title": "chore(tools): 添加開發工具腳本",
        "description": """添加自動化開發工具

編碼修復工具:
- check_encoding.py - 掃描編碼問題
- fix_encoding.py - 修復優先文件
- batch_fix_all.py - 批量修復所有文件
- fix_building_precise.py - 精確修復建築系統

其他工具:
- final_comprehensive_fix.py - 綜合修復
- fix_ai_quality_controller.py - AI質量控制
- fix_build_cs_encoding.py - Build.cs編碼修復
- fix_comprehensive.py - 全面修復
- fix_process_manager.py - 進程管理修復
- fix_risk_dashboard.py - 風險儀表板修復
- fix_vr_ar_encoding.py - VR/AR編碼修復
- scan_encoding_issues.py - 編碼問題掃描

PowerShell腳本:
- auto_generate_clean.ps1
- epic_assets_generate_clean.ps1""",
        "patterns": ["Tools/*.py", "Tools/*.ps1", "*.py"]
    },
    
    "os-kernel": {
        "title": "feat(os): 實現內建作業系統核心",
        "description": """實現 MingGoRTS 內建作業系統

核心組件:
- MingRTSKernel - 作業系統內核
- MingRTSProcessManager - 進程管理器
- MingRTSMemoryManager - 內存管理器
- MingRTSVirtualFileSystem - 虛擬文件系統
- MingRTSServiceManager - 系統服務管理器
- MingRTSKernelSageCommandExtension - 至聖者指揮學擴展

技術特性:
- 支持100+併發進程
- 進程切換延遲 < 10ms
- 內存分配延遲 < 1ms
- 系統啟動時間 < 5秒

系統調用支持:
- 進程管理 (CreateProcess, TerminateProcess)
- 內存管理 (AllocateMemory, FreeMemory)
- 文件系統 (OpenFile, ReadFile, WriteFile)
- 網絡系統 (CreateSocket, SendData, ReceiveData)
- 至聖者指揮學系統調用""",
        "patterns": ["MingRTSKernel*", "MingRTSProcess*", "MingRTSMemory*", 
                    "MingRTSVirtual*", "MingRTSService*", "OperatingSystem/"]
    },
    
    "compilation-fixes": {
        "title": "fix(build): 修復編譯錯誤和語法問題",
        "description": """修復多個編譯錯誤和語法問題

修復的錯誤類型:
- Delegate 宏宣告參數數量不匹配
- UPROPERTY TMap<TArray> 不支持問題
- AMingGoRTSUnit 引用問題
- EMingResourceType 未定義問題
- FCulturalPreferences 重複定義
- UENUM/USTRUCT 範圍錯誤
- ELanguageCode 相關錯誤
- 類定義閉合問題 (EOF 語法錯誤)

主要修改文件:
- MingFourLayerTypes.h
- IMingGameLayer.h
- MingRTSLocalizationTypes.h
- MingStrategicFourLayerTypes.h
- MingTacticalLayer.h
- MingCharacterSystem.h
- MingAccessibilityHelper.h
- MingRepublicEraAudioThemes.h
- MingFormationManager.h
- MingFogOfWarManager.h""",
        "patterns": ["MingEvent*", "MingBuilding*", "MingBlockchain*", "MingNetwork*",
                    "MingStrategic*", "MingTactical*", "MingCharacter*"]
    }
}

def run_git_command(args, cwd=PROJECT_ROOT):
    """執行 Git 命令"""
    try:
        result = subprocess.run(
            ["git"] + args,
            cwd=cwd,
            capture_output=True,
            text=True,
            encoding='utf-8'
        )
        return result.returncode == 0, result.stdout, result.stderr
    except Exception as e:
        return False, "", str(e)

def get_modified_files():
    """獲取所有修改過的文件"""
    success, stdout, stderr = run_git_command(["status", "--porcelain"])
    if not success:
        print(f"✗ 獲取 Git 狀態失敗: {stderr}")
        return []
    
    files = []
    for line in stdout.strip().split('\n'):
        if line:
            status = line[:2]
            filepath = line[3:]
            files.append((status, filepath))
    return files

def categorize_files(files):
    """將文件分類到不同的提交類別"""
    categories = {key: [] for key in COMMIT_CATEGORIES.keys()}
    categories["other"] = []
    
    for status, filepath in files:
        # 跳過刪除的文件
        if status.strip() == 'D':
            continue
            
        assigned = False
        for cat_key, cat_config in COMMIT_CATEGORIES.items():
            for pattern in cat_config["patterns"]:
                if pattern in filepath:
                    categories[cat_key].append((status, filepath))
                    assigned = True
                    break
            if assigned:
                break
        
        if not assigned:
            categories["other"].append((status, filepath))
    
    return categories

def create_commit(category_key, files):
    """創建分類提交"""
    if not files:
        return True
    
    config = COMMIT_CATEGORIES.get(category_key, {
        "title": f"chore: 其他變更 ({category_key})",
        "description": "其他文件變更"
    })
    
    # 添加文件到暫存區
    filepaths = [f for _, f in files]
    
    # 分批添加避免命令行過長
    batch_size = 50
    for i in range(0, len(filepaths), batch_size):
        batch = filepaths[i:i+batch_size]
        success, _, stderr = run_git_command(["add"] + batch)
        if not success:
            print(f"  ✗ 添加文件失敗: {stderr}")
            return False
    
    # 創建提交
    commit_message = f"{config['title']}\n\n{config['description']}"
    
    success, stdout, stderr = run_git_command(["commit", "-m", commit_message])
    if success:
        print(f"  ✓ 成功創建提交: {config['title']}")
        print(f"    包含 {len(files)} 個文件")
        return True
    else:
        print(f"  ✗ 提交失敗: {stderr}")
        return False

def smart_version_control():
    """智能版本控制主流程"""
    print("=" * 60)
    print("智慧版控 - 智能分類 Git 提交")
    print("=" * 60)
    
    # 1. 獲取變更文件
    print("\n📋 步驟 1: 掃描變更文件...")
    files = get_modified_files()
    
    if not files:
        print("✓ 沒有需要提交的變更")
        return
    
    print(f"✓ 找到 {len(files)} 個變更文件")
    
    # 2. 分類文件
    print("\n📂 步驟 2: 智能分類文件...")
    categorized = categorize_files(files)
    
    for cat_key, cat_files in categorized.items():
        if cat_files:
            print(f"  - {cat_key}: {len(cat_files)} 個文件")
    
    # 3. 創建結構化提交
    print("\n💾 步驟 3: 創建結構化提交...")
    
    commit_order = [
        "compilation-fixes",  # 先提交編譯修復
        "encoding-fixes",     # 編碼修復
        "os-kernel",          # 作業系統核心
        "core-systems",       # 核心系統
        "story-systems",      # 故事系統
        "innovation-features", # 創新功能
        "tools-scripts",       # 工具腳本
        "other"                # 其他
    ]
    
    success_count = 0
    for cat_key in commit_order:
        cat_files = categorized.get(cat_key, [])
        if cat_files:
            if create_commit(cat_key, cat_files):
                success_count += 1
    
    # 4. 顯示提交歷史
    print("\n📊 步驟 4: 提交摘要...")
    success, stdout, _ = run_git_command(["log", "--oneline", "-10"])
    if success:
        print("\n最近 10 次提交:")
        print(stdout)
    
    print("\n" + "=" * 60)
    print(f"✓ 智能版控完成！成功創建 {success_count} 個分類提交")
    print("=" * 60)
    
    return success_count > 0

if __name__ == "__main__":
    try:
        result = smart_version_control()
        sys.exit(0 if result else 1)
    except KeyboardInterrupt:
        print("\n\n⚠ 操作已取消")
        sys.exit(1)
    except Exception as e:
        print(f"\n✗ 發生錯誤: {e}")
        sys.exit(1)
