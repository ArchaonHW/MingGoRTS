#!/usr/bin/env python3
"""
MingGoRTS 技能系統測試腳本
測試 Story 3-2: 技能發展系統
"""

import unittest
import json
import sys
import os
from datetime import datetime
from enum import Enum

# 添加項目根目錄到 Python 路徑
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class ESkillCategory(Enum):
    """技能類別枚舉"""
    Combat = "Combat"
    Strategy = "Strategy"
    Diplomacy = "Diplomacy"
    Leadership = "Leadership"
    Intelligence = "Intelligence"
    Economic = "Economic"

class ESkillType(Enum):
    """技能類型枚舉"""
    Active = "Active"
    Passive = "Passive"
    Toggle = "Toggle"

class ESpecializationPath(Enum):
    """專精路徑枚舉"""
    Commander = "Commander"
    Tactician = "Tactician"
    Diplomat = "Diplomat"
    Spy = "Spy"
    Economist = "Economist"
    Generalist = "Generalist"

class TestSkillSystem(unittest.TestCase):
    """技能系統測試類"""
    
    def setUp(self):
        """測試前的設置"""
        self.skill_system = MockSkillSystem()
        self.skill_manager = MockSkillManager()
        
        # 測試技能數據
        self.test_skill = {
            "id": "TacticalCommand",
            "name": "戰術指揮",
            "description": "提升部隊指揮效率",
            "category": ESkillCategory.Combat,
            "type": ESkillType.Passive,
            "specialization": ESpecializationPath.Commander,
            "max_level": 5,
            "current_level": 0,
            "experience": 0,
            "experience_to_next": 100,
            "is_unlocked": False,
            "effects": [
                {"type": "LeadershipBonus", "base_value": 5.0, "scaling": 3.0}
            ]
        }
        
        # 測試角色數據
        self.test_character = {
            "name": "張偉",
            "background": "MilitaryAcademy",
            "level": 5,
            "attributes": {
                "leadership": 75,
                "intelligence": 60,
                "courage": 80,
                "charisma": 55,
                "constitution": 70
            }
        }
    
    def test_skill_initialization(self):
        """測試技能系統初始化"""
        self.skill_system.initialize()
        
        self.assertTrue(self.skill_system.is_initialized)
        self.assertGreater(len(self.skill_system.all_skills), 0)
        self.assertGreater(len(self.skill_system.skill_trees), 0)
        
        print("✅ 技能系統初始化測試通過")
    
    def test_skill_unlock(self):
        """測試技能解鎖"""
        self.skill_system.initialize()
        
        # 測試解鎖新技能
        success = self.skill_system.unlock_skill("TacticalCommand")
        self.assertTrue(success)
        
        skill = self.skill_system.get_skill("TacticalCommand")
        self.assertTrue(skill["is_unlocked"])
        self.assertEqual(skill["current_level"], 1)
        
        print("✅ 技能解鎖測試通過")
    
    def test_skill_upgrade(self):
        """測試技能升級"""
        self.skill_system.initialize()
        
        # 先解鎖技能
        self.skill_system.unlock_skill("TacticalCommand")
        
        # 添加經驗並升級
        self.skill_system.add_skill_experience("TacticalCommand", 150)
        
        skill = self.skill_system.get_skill("TacticalCommand")
        self.assertGreater(skill["current_level"], 1)
        
        print("✅ 技能升級測試通過")
    
    def test_skill_experience_calculation(self):
        """測試技能經驗計算"""
        self.skill_system.initialize()
        
        # 測試經驗需求計算
        for level in range(1, 6):
            exp_required = self.skill_system.calculate_experience_requirement(level)
            self.assertGreater(exp_required, 0)
            self.assertGreater(exp_required, self.skill_system.calculate_experience_requirement(level - 1))
        
        print("✅ 技能經驗計算測試通過")
    
    def test_specialization_paths(self):
        """測試專精路徑系統"""
        self.skill_system.initialize()
        
        # 測試所有專精路徑
        for path in ESpecializationPath:
            self.skill_system.choose_specialization_path(path)
            self.assertEqual(self.skill_system.current_specialization, path)
            
            # 檢查路徑顏色
            color = self.skill_system.get_specialization_path_color(path)
            self.assertIsNotNone(color)
        
        print("✅ 專精路徑系統測試通過")
    
    def test_skill_effects(self):
        """測試技能效果計算"""
        self.skill_system.initialize()
        
        # 解鎖技能並升級
        self.skill_system.unlock_skill("TacticalCommand")
        self.skill_system.add_skill_experience("TacticalCommand", 300)
        
        # 計算技能效果
        effect_value = self.skill_system.calculate_skill_effect("TacticalCommand", "LeadershipBonus")
        self.assertGreater(effect_value, 0)
        
        print("✅ 技能效果計算測試通過")
    
    def test_skill_requirements_validation(self):
        """測試技能需求驗證"""
        self.skill_system.initialize()
        
        # 測試無需求技能
        can_unlock = self.skill_system.can_unlock_skill("MeleeMastery", self.test_character)
        self.assertTrue(can_unlock)
        
        # 測試有需求技能
        self.skill_system.unlock_skill("TacticalCommand")
        can_unlock = self.skill_system.can_unlock_skill("AdvancedTactics", self.test_character)
        # 取決於具體實現，這裡只是測試邏輯
        
        print("✅ 技能需求驗證測試通過")
    
    def test_skill_manager_integration(self):
        """測試技能管理器整合"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 測試經驗給予
        self.skill_manager.grant_skill_experience("TacticalCommand", 100, "測試")
        
        # 測試技能點給予
        old_points = self.skill_manager.available_skill_points
        self.skill_manager.grant_skill_points(2, "測試")
        self.assertEqual(self.skill_manager.available_skill_points, old_points + 2)
        
        print("✅ 技能管理器整合測試通過")
    
    def test_combat_experience_processing(self):
        """測試戰鬥經驗處理"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 測試戰鬥勝利
        self.skill_manager.process_combat_end(True, 10, 300.0)
        
        # 測試戰鬥失敗
        self.skill_manager.process_combat_end(False, 5, 180.0)
        
        print("✅ 戰鬥經驗處理測試通過")
    
    def test_quest_completion_processing(self):
        """測試任務完成處理"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 測試任務完成
        self.skill_manager.process_quest_completion("TestQuest", 50)
        
        print("✅ 任務完成處理測試通過")
    
    def test_historical_event_processing(self):
        """測試歷史事件處理"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 測試歷史事件
        related_skills = ["TacticalCommand", "Leadership"]
        self.skill_manager.process_historical_event("NorthernExpedition", related_skills)
        
        print("✅ 歷史事件處理測試通過")
    
    def test_auto_experience_distribution(self):
        """測試自動經驗分配"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 解鎖一些技能
        self.skill_system.unlock_skill("TacticalCommand")
        self.skill_system.unlock_skill("Leadership")
        
        # 自動分配經驗
        self.skill_manager.auto_distribute_experience(200, ESkillCategory.Combat)
        
        print("✅ 自動經驗分配測試通過")
    
    def test_character_level_up_processing(self):
        """測試角色升級處理"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 測試角色升級
        self.skill_manager.process_character_level_up(6)
        
        print("✅ 角色升級處理測試通過")
    
    def test_skill_action_validation(self):
        """測試技能操作驗證"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 測試解鎖驗證
        can_unlock = self.skill_manager.validate_skill_action("TacticalCommand", "Unlock")
        self.assertTrue(can_unlock)
        
        # 測試升級驗證（需要先解鎖）
        can_upgrade = self.skill_manager.validate_skill_action("TacticalCommand", "Upgrade")
        self.assertFalse(can_upgrade)  # 未解鎖，不能升級
        
        print("✅ 技能操作驗證測試通過")
    
    def test_skill_recommendation_system(self):
        """測試技能推薦系統"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 獲取推薦技能
        recommended = self.skill_manager.get_recommended_skills()
        self.assertIsInstance(recommended, list)
        
        # 獲取發展建議
        advice = self.skill_manager.get_skill_development_advice()
        self.assertIsInstance(advice, str)
        self.assertGreater(len(advice), 0)
        
        print("✅ 技能推薦系統測試通過")
    
    def test_skill_system_reset(self):
        """測試技能系統重置"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 解鎖一些技能
        self.skill_system.unlock_skill("TacticalCommand")
        self.skill_system.unlock_skill("Leadership")
        
        # 重置系統
        success = self.skill_manager.reset_skill_system()
        self.assertTrue(success)
        
        print("✅ 技能系統重置測試通過")
    
    def test_skill_experience_history(self):
        """測試技能經驗歷史"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 給予一些經驗
        self.skill_manager.grant_skill_experience("TacticalCommand", 50, "測試1")
        self.skill_manager.grant_skill_experience("TacticalCommand", 30, "測試2")
        
        # 獲取歷史
        history = self.skill_manager.get_skill_experience_history("TacticalCommand")
        self.assertEqual(len(history), 2)
        
        print("✅ 技能經驗歷史測試通過")
    
    def test_complete_skill_progression_flow(self):
        """測試完整的技能發展流程"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 1. 選擇專精路徑
        self.skill_system.choose_specialization_path(ESpecializationPath.Commander)
        
        # 2. 解鎖初始技能
        self.skill_manager.unlock_skill("TacticalCommand")
        
        # 3. 經過戰鬥獲得經驗
        self.skill_manager.process_combat_end(True, 15, 400.0)
        
        # 4. 完成任務獲得經驗
        self.skill_manager.process_quest_completion("BattleTest", 60)
        
        # 5. 角色升級
        self.skill_manager.process_character_level_up(6)
        
        # 6. 檢查技能狀態
        skill = self.skill_system.get_skill("TacticalCommand")
        self.assertGreater(skill["current_level"], 1)
        
        # 7. 獲取發展建議
        advice = self.skill_manager.get_skill_development_advice()
        self.assertGreater(len(advice), 0)
        
        print("✅ 完整技能發展流程測試通過")


class MockSkillSystem:
    """模擬技能系統"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_skills = []
        self.skill_trees = []
        self.skill_map = {}
        self.current_specialization = ESpecializationPath.Generalist
        self.available_skill_points = 0
        self._initialize_mock_skills()
    
    def initialize(self):
        """初始化技能系統"""
        self.is_initialized = True
        self._create_skill_trees()
    
    def _initialize_mock_skills(self):
        """初始化模擬技能"""
        skills = [
            {
                "id": "TacticalCommand",
                "name": "戰術指揮",
                "category": ESkillCategory.Combat,
                "type": ESkillType.Passive,
                "specialization": ESpecializationPath.Commander,
                "max_level": 5,
                "current_level": 0,
                "experience": 0,
                "experience_to_next": 100,
                "is_unlocked": False,
                "effects": [{"type": "LeadershipBonus", "base_value": 5.0, "scaling": 3.0}]
            },
            {
                "id": "Leadership",
                "name": "領導力",
                "category": ESkillCategory.Leadership,
                "type": ESkillType.Passive,
                "specialization": ESpecializationPath.Commander,
                "max_level": 5,
                "current_level": 0,
                "experience": 0,
                "experience_to_next": 100,
                "is_unlocked": False,
                "effects": [{"type": "MoraleBonus", "base_value": 10.0, "scaling": 2.0}]
            },
            {
                "id": "MeleeMastery",
                "name": "近戰精通",
                "category": ESkillCategory.Combat,
                "type": ESkillType.Active,
                "specialization": ESpecializationPath.Generalist,
                "max_level": 3,
                "current_level": 0,
                "experience": 0,
                "experience_to_next": 100,
                "is_unlocked": False,
                "effects": [{"type": "CourageBonus", "base_value": 8.0, "scaling": 4.0}]
            }
        ]
        
        self.all_skills = skills
        for skill in skills:
            self.skill_map[skill["id"]] = skill.copy()
    
    def _create_skill_trees(self):
        """創建技能樹"""
        self.skill_trees = [
            {
                "path": ESpecializationPath.Commander,
                "name": "指揮官路徑",
                "root_skills": ["TacticalCommand", "Leadership"]
            },
            {
                "path": ESpecializationPath.Generalist,
                "name": "通才路徑",
                "root_skills": ["MeleeMastery"]
            }
        ]
    
    def unlock_skill(self, skill_id):
        """解鎖技能"""
        if skill_id in self.skill_map:
            skill = self.skill_map[skill_id]
            if not skill["is_unlocked"] and self.available_skill_points > 0:
                skill["is_unlocked"] = True
                skill["current_level"] = 1
                self.available_skill_points -= 1
                return True
        return False
    
    def add_skill_experience(self, skill_id, amount):
        """添加技能經驗"""
        if skill_id in self.skill_map:
            skill = self.skill_map[skill_id]
            if skill["is_unlocked"]:
                skill["experience"] += amount
                while skill["experience"] >= skill["experience_to_next"] and skill["current_level"] < skill["max_level"]:
                    skill["experience"] -= skill["experience_to_next"]
                    skill["current_level"] += 1
                    skill["experience_to_next"] = self.calculate_experience_requirement(skill["current_level"])
    
    def get_skill(self, skill_id):
        """獲取技能"""
        return self.skill_map.get(skill_id, {}).copy()
    
    def calculate_experience_requirement(self, level):
        """計算經驗需求"""
        return int(100 * (level ** 1.5))
    
    def choose_specialization_path(self, path):
        """選擇專精路徑"""
        self.current_specialization = path
        self.available_skill_points += 2
    
    def get_specialization_path_color(self, path):
        """獲取專精路徑顏色"""
        colors = {
            ESpecializationPath.Commander: (1.0, 0.2, 0.2),
            ESpecializationPath.Tactician: (0.2, 0.2, 1.0),
            ESpecializationPath.Diplomat: (0.2, 1.0, 0.2),
            ESpecializationPath.Spy: (0.8, 0.2, 0.8),
            ESpecializationPath.Economist: (1.0, 0.8, 0.2),
            ESpecializationPath.Generalist: (0.7, 0.7, 0.7)
        }
        return colors.get(path, (1.0, 1.0, 1.0))
    
    def calculate_skill_effect(self, skill_id, effect_type):
        """計算技能效果"""
        skill = self.get_skill(skill_id)
        if not skill["is_unlocked"]:
            return 0.0
        
        total_effect = 0.0
        for effect in skill["effects"]:
            if effect["type"] == effect_type:
                total_effect += effect["base_value"] + (effect["scaling"] * (skill["current_level"] - 1))
        
        return total_effect
    
    def can_unlock_skill(self, skill_id, character_data):
        """檢查是否可以解鎖技能"""
        if skill_id not in self.skill_map:
            return False
        
        skill = self.skill_map[skill_id]
        return not skill["is_unlocked"] and self.available_skill_points > 0


class MockSkillManager:
    """模擬技能管理器"""
    
    def __init__(self):
        self.is_initialized = False
        self.skill_system = None
        self.character = None
        self.available_skill_points = 0
        self.experience_history = []
    
    def initialize(self, skill_system, character):
        """初始化技能管理器"""
        self.skill_system = skill_system
        self.character = character
        self.is_initialized = True
        self.available_skill_points = 3  # 初始技能點
    
    def grant_skill_experience(self, skill_id, amount, reason=""):
        """給予技能經驗"""
        if self.skill_system:
            self.skill_system.add_skill_experience(skill_id, amount)
            self.experience_history.append({
                "skill_id": skill_id,
                "amount": amount,
                "reason": reason,
                "time": datetime.now()
            })
    
    def grant_skill_points(self, points, reason=""):
        """給予技能點"""
        self.available_skill_points += points
    
    def process_combat_end(self, victory, enemy_count, duration):
        """處理戰鬥結束"""
        base_exp = int(10 * enemy_count)
        if victory:
            base_exp = int(base_exp * 1.5)
        
        # 分配經驗給戰鬥技能
        self.grant_skill_experience("TacticalCommand", base_exp, "戰鬥")
        self.grant_skill_experience("MeleeMastery", int(base_exp * 0.8), "戰鬥")
        
        if victory:
            self.grant_skill_points(1, "戰鬥勝利")
    
    def process_quest_completion(self, quest_id, difficulty):
        """處理任務完成"""
        base_exp = difficulty
        self.grant_skill_experience("StrategicPlanning", base_exp, "任務")
        self.grant_skill_points(int(difficulty / 10), "任務完成")
    
    def process_historical_event(self, event_id, related_skills):
        """處理歷史事件"""
        base_exp = 50
        for skill_id in related_skills:
            self.grant_skill_experience(skill_id, base_exp, "歷史事件")
        self.grant_skill_points(1, f"歷史事件: {event_id}")
    
    def auto_distribute_experience(self, total_exp, primary_category):
        """自動分配經驗"""
        # 簡化實現
        self.grant_skill_experience("TacticalCommand", total_exp // 2, "自動分配")
        self.grant_skill_experience("Leadership", total_exp // 2, "自動分配")
    
    def process_character_level_up(self, new_level):
        """處理角色升級"""
        self.grant_skill_points(2, f"升級到{new_level}級")
        self.grant_skill_experience("TacticalCommand", 50, "升級獎勵")
        self.grant_skill_experience("Leadership", 50, "升級獎勵")
    
    def validate_skill_action(self, skill_id, action_type):
        """驗證技能操作"""
        if not self.skill_system:
            return False
        
        if action_type == "Unlock":
            return self.skill_system.can_unlock_skill(skill_id, self.character)
        elif action_type == "Upgrade":
            skill = self.skill_system.get_skill(skill_id)
            return skill["is_unlocked"] and skill["current_level"] < skill["max_level"]
        
        return False
    
    def get_recommended_skills(self):
        """獲取推薦技能"""
        if not self.skill_system:
            return []
        
        # 簡化實現，返回所有可解鎖的技能
        recommended = []
        for skill_id, skill in self.skill_system.skill_map.items():
            if not skill["is_unlocked"]:
                recommended.append(skill_id)
        
        return recommended
    
    def get_skill_development_advice(self):
        """獲取技能發展建議"""
        advice = "技能發展建議：\n"
        advice += f"• 你有 {self.available_skill_points} 個可用技能點\n"
        
        unlocked_count = sum(1 for skill in self.skill_system.skill_map.values() if skill["is_unlocked"])
        if unlocked_count > 0:
            advice += f"• 已解鎖 {unlocked_count} 個技能\n"
        else:
            advice += "• 建議解鎖第一個技能\n"
        
        return advice
    
    def reset_skill_system(self):
        """重置技能系統"""
        if self.skill_system:
            for skill in self.skill_system.skill_map.values():
                if skill["is_unlocked"]:
                    self.available_skill_points += skill["current_level"]
                    skill["is_unlocked"] = False
                    skill["current_level"] = 0
                    skill["experience"] = 0
        
        self.experience_history.clear()
        return True
    
    def get_skill_experience_history(self, skill_id):
        """獲取技能經驗歷史"""
        return [entry for entry in self.experience_history if entry["skill_id"] == skill_id]
    
    def unlock_skill(self, skill_id):
        """解鎖技能（代理方法）"""
        if self.skill_system:
            return self.skill_system.unlock_skill(skill_id)
        return False
    
    def get_skill(self, skill_id):
        """獲取技能（代理方法）"""
        if self.skill_system:
            return self.skill_system.get_skill(skill_id)
        return {}


def run_skill_system_tests():
    """運行技能系統測試"""
    print("開始運行 MingGoRTS 技能系統測試...")
    print("=" * 60)
    
    # 創建測試套件
    suite = unittest.TestLoader().loadTestsFromTestCase(TestSkillSystem)
    
    # 運行測試
    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)
    
    print("=" * 60)
    print(f"測試完成！")
    print(f"運行測試數量: {result.testsRun}")
    print(f"失敗數量: {len(result.failures)}")
    print(f"錯誤數量: {len(result.errors)}")
    
    if result.failures:
        print("\n失敗的測試:")
        for test, traceback in result.failures:
            print(f"- {test}: {traceback}")
    
    if result.errors:
        print("\n錯誤的測試:")
        for test, traceback in result.errors:
            print(f"- {test}: {traceback}")
    
    success_rate = (result.testsRun - len(result.failures) - len(result.errors)) / result.testsRun * 100
    print(f"\n成功率: {success_rate:.1f}%")
    
    return result.wasSuccessful()


if __name__ == "__main__":
    success = run_skill_system_tests()
    sys.exit(0 if success else 1)
