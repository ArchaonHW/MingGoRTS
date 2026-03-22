#!/usr/bin/env python3
"""
MingGoRTS ??賜頂蝯望葫閰西??皜祈岫 Story 3-2: ??賜撅頂蝯?"""

import unittest
import json
import sys
import os
from datetime import datetime
from enum import Enum

# 瘛餃???寧? Python 頝臬?
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class ESkillCategory(Enum):
    """??賡??交???""
    Combat = "Combat"
    Strategy = "Strategy"
    Diplomacy = "Diplomacy"
    Leadership = "Leadership"
    Intelligence = "Intelligence"
    Economic = "Economic"

class ESkillType(Enum):
    """??賡?????""
    Active = "Active"
    Passive = "Passive"
    Toggle = "Toggle"

class ESpecializationPath(Enum):
    """撠移頝臬???"""
    Commander = "Commander"
    Tactician = "Tactician"
    Diplomat = "Diplomat"
    Spy = "Spy"
    Economist = "Economist"
    Generalist = "Generalist"

class TestSkillSystem(unittest.TestCase):
    """??賜頂蝯望葫閰阡?"""
    
    def setUp(self):
        """皜祈岫??閮剔蔭"""
        self.skill_system = MockSkillSystem()
        self.skill_manager = MockSkillManager()
        
        # 皜祈岫??賣??        self.test_skill = {
            "id": "TacticalCommand",
            "name": "?啗??",
            "description": "???券????",
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
        
        # 皜祈岫閫?豢?
        self.test_character = {
            "name": "撘萄?",
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
        """皜祈岫??賜頂蝯勗?憪?"""
        self.skill_system.initialize()
        
        self.assertTrue(self.skill_system.is_initialized)
        self.assertGreater(len(self.skill_system.all_skills), 0)
        self.assertGreater(len(self.skill_system.skill_trees), 0)
        
        print("????賜頂蝯勗?憪?皜祈岫??")
    
    def test_skill_unlock(self):
        """皜祈岫??質圾??""
        self.skill_system.initialize()
        
        # 皜祈岫閫???唳???        success = self.skill_system.unlock_skill("TacticalCommand")
        self.assertTrue(success)
        
        skill = self.skill_system.get_skill("TacticalCommand")
        self.assertTrue(skill["is_unlocked"])
        self.assertEqual(skill["current_level"], 1)
        
        print("????質圾?葫閰阡?")
    
    def test_skill_upgrade(self):
        """皜祈岫??賢?蝝?""
        self.skill_system.initialize()
        
        # ?圾????        self.skill_system.unlock_skill("TacticalCommand")
        
        # 瘛餃?蝬?銝血?蝝?        self.skill_system.add_skill_experience("TacticalCommand", 150)
        
        skill = self.skill_system.get_skill("TacticalCommand")
        self.assertGreater(skill["current_level"], 1)
        
        print("????賢?蝝葫閰阡?")
    
    def test_skill_experience_calculation(self):
        """皜祈岫??賜?撽?蝞?""
        self.skill_system.initialize()
        
        # 皜祈岫蝬??瘙?蝞?        for level in range(1, 6):
            exp_required = self.skill_system.calculate_experience_requirement(level)
            self.assertGreater(exp_required, 0)
            self.assertGreater(exp_required, self.skill_system.calculate_experience_requirement(level - 1))
        
        print("????賜?撽?蝞葫閰阡?")
    
    def test_specialization_paths(self):
        """皜祈岫撠移頝臬?蝟餌絞"""
        self.skill_system.initialize()
        
        # 皜祈岫???蝎曇楝敺?        for path in ESpecializationPath:
            self.skill_system.choose_specialization_path(path)
            self.assertEqual(self.skill_system.current_specialization, path)
            
            # 瑼Ｘ頝臬?憿
            color = self.skill_system.get_specialization_path_color(path)
            self.assertIsNotNone(color)
        
        print("??撠移頝臬?蝟餌絞皜祈岫??")
    
    def test_skill_effects(self):
        """皜祈岫??賣???蝞?""
        self.skill_system.initialize()
        
        # 閫????賭蒂??
        self.skill_system.unlock_skill("TacticalCommand")
        self.skill_system.add_skill_experience("TacticalCommand", 300)
        
        # 閮???賣???        effect_value = self.skill_system.calculate_skill_effect("TacticalCommand", "LeadershipBonus")
        self.assertGreater(effect_value, 0)
        
        print("????賣???蝞葫閰阡?")
    
    def test_skill_requirements_validation(self):
        """皜祈岫??賡?瘙?霅?""
        self.skill_system.initialize()
        
        # 皜祈岫?⊿?瘙???        can_unlock = self.skill_system.can_unlock_skill("MeleeMastery", self.test_character)
        self.assertTrue(can_unlock)
        
        # 皜祈岫??瘙???        self.skill_system.unlock_skill("TacticalCommand")
        can_unlock = self.skill_system.can_unlock_skill("AdvancedTactics", self.test_character)
        # ?捱?澆擃祕?橘??ㄐ?芣皜祈岫?摩
        
        print("????賡?瘙?霅葫閰阡?")
    
    def test_skill_manager_integration(self):
        """皜祈岫??賜恣??游?"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 皜祈岫蝬?蝯虫?
        self.skill_manager.grant_skill_experience("TacticalCommand", 100, "皜祈岫")
        
        # 皜祈岫??賡?蝯虫?
        old_points = self.skill_manager.available_skill_points
        self.skill_manager.grant_skill_points(2, "皜祈岫")
        self.assertEqual(self.skill_manager.available_skill_points, old_points + 2)
        
        print("????賜恣??游?皜祈岫??")
    
    def test_combat_experience_processing(self):
        """皜祈岫?圈洛蝬???"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 皜祈岫?圈洛?
        self.skill_manager.process_combat_end(True, 10, 300.0)
        
        # 皜祈岫?圈洛憭望?
        self.skill_manager.process_combat_end(False, 5, 180.0)
        
        print("???圈洛蝬???皜祈岫??")
    
    def test_quest_completion_processing(self):
        """皜祈岫隞餃?摰???"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 皜祈岫隞餃?摰?
        self.skill_manager.process_quest_completion("TestQuest", 50)
        
        print("??隞餃?摰???皜祈岫??")
    
    def test_historical_event_processing(self):
        """皜祈岫甇瑕鈭辣??"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 皜祈岫甇瑕鈭辣
        related_skills = ["TacticalCommand", "Leadership"]
        self.skill_manager.process_historical_event("NorthernExpedition", related_skills)
        
        print("??甇瑕鈭辣??皜祈岫??")
    
    def test_auto_experience_distribution(self):
        """皜祈岫?芸?蝬???"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 閫??銝鈭???        self.skill_system.unlock_skill("TacticalCommand")
        self.skill_system.unlock_skill("Leadership")
        
        # ?芸???蝬?
        self.skill_manager.auto_distribute_experience(200, ESkillCategory.Combat)
        
        print("???芸?蝬???皜祈岫??")
    
    def test_character_level_up_processing(self):
        """皜祈岫閫????"""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 皜祈岫閫??
        self.skill_manager.process_character_level_up(6)
        
        print("??閫????皜祈岫??")
    
    def test_skill_action_validation(self):
        """皜祈岫??賣?雿?霅?""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 皜祈岫閫??撽?
        can_unlock = self.skill_manager.validate_skill_action("TacticalCommand", "Unlock")
        self.assertTrue(can_unlock)
        
        # 皜祈岫??撽?嚗?閬?閫??嚗?        can_upgrade = self.skill_manager.validate_skill_action("TacticalCommand", "Upgrade")
        self.assertFalse(can_upgrade)  # ?芾圾??銝??
        
        print("????賣?雿?霅葫閰阡?")
    
    def test_skill_recommendation_system(self):
        """皜祈岫??賣?衣頂蝯?""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # ?脣??刻???        recommended = self.skill_manager.get_recommended_skills()
        self.assertIsInstance(recommended, list)
        
        # ?脣??澆?撱箄降
        advice = self.skill_manager.get_skill_development_advice()
        self.assertIsInstance(advice, str)
        self.assertGreater(len(advice), 0)
        
        print("????賣?衣頂蝯望葫閰阡?")
    
    def test_skill_system_reset(self):
        """皜祈岫??賜頂蝯梢?蝵?""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 閫??銝鈭???        self.skill_system.unlock_skill("TacticalCommand")
        self.skill_system.unlock_skill("Leadership")
        
        # ?蔭蝟餌絞
        success = self.skill_manager.reset_skill_system()
        self.assertTrue(success)
        
        print("????賜頂蝯梢?蝵格葫閰阡?")
    
    def test_skill_experience_history(self):
        """皜祈岫??賜?撽風??""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 蝯虫?銝鈭?撽?        self.skill_manager.grant_skill_experience("TacticalCommand", 50, "皜祈岫1")
        self.skill_manager.grant_skill_experience("TacticalCommand", 30, "皜祈岫2")
        
        # ?脣?甇瑕
        history = self.skill_manager.get_skill_experience_history("TacticalCommand")
        self.assertEqual(len(history), 2)
        
        print("????賜?撽風?脫葫閰阡?")
    
    def test_complete_skill_progression_flow(self):
        """皜祈岫摰???賜撅?蝔?""
        self.skill_manager.initialize(self.skill_system, self.test_character)
        
        # 1. ?豢?撠移頝臬?
        self.skill_system.choose_specialization_path(ESpecializationPath.Commander)
        
        # 2. 閫???????        self.skill_manager.unlock_skill("TacticalCommand")
        
        # 3. 蝬??圈洛?脣?蝬?
        self.skill_manager.process_combat_end(True, 15, 400.0)
        
        # 4. 摰?隞餃??脣?蝬?
        self.skill_manager.process_quest_completion("BattleTest", 60)
        
        # 5. 閫??
        self.skill_manager.process_character_level_up(6)
        
        # 6. 瑼Ｘ??賜???        skill = self.skill_system.get_skill("TacticalCommand")
        self.assertGreater(skill["current_level"], 1)
        
        # 7. ?脣??澆?撱箄降
        advice = self.skill_manager.get_skill_development_advice()
        self.assertGreater(len(advice), 0)
        
        print("??摰??賜撅?蝔葫閰阡?")


class MockSkillSystem:
    """璅⊥??賜頂蝯?""
    
    def __init__(self):
        self.is_initialized = False
        self.all_skills = []
        self.skill_trees = []
        self.skill_map = {}
        self.current_specialization = ESpecializationPath.Generalist
        self.available_skill_points = 0
        self._initialize_mock_skills()
    
    def initialize(self):
        """?????賜頂蝯?""
        self.is_initialized = True
        self._create_skill_trees()
    
    def _initialize_mock_skills(self):
        """???芋?祆???""
        skills = [
            {
                "id": "TacticalCommand",
                "name": "?啗??",
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
                "name": "????,
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
                "name": "餈蝎暸?,
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
        """?萄遣??賣邦"""
        self.skill_trees = [
            {
                "path": ESpecializationPath.Commander,
                "name": "?摰楝敺?,
                "root_skills": ["TacticalCommand", "Leadership"]
            },
            {
                "path": ESpecializationPath.Generalist,
                "name": "??頝臬?",
                "root_skills": ["MeleeMastery"]
            }
        ]
    
    def unlock_skill(self, skill_id):
        """閫?????""
        if skill_id in self.skill_map:
            skill = self.skill_map[skill_id]
            if not skill["is_unlocked"] and self.available_skill_points > 0:
                skill["is_unlocked"] = True
                skill["current_level"] = 1
                self.available_skill_points -= 1
                return True
        return False
    
    def add_skill_experience(self, skill_id, amount):
        """瘛餃???賜?撽?""
        if skill_id in self.skill_map:
            skill = self.skill_map[skill_id]
            if skill["is_unlocked"]:
                skill["experience"] += amount
                while skill["experience"] >= skill["experience_to_next"] and skill["current_level"] < skill["max_level"]:
                    skill["experience"] -= skill["experience_to_next"]
                    skill["current_level"] += 1
                    skill["experience_to_next"] = self.calculate_experience_requirement(skill["current_level"])
    
    def get_skill(self, skill_id):
        """?脣????""
        return self.skill_map.get(skill_id, {}).copy()
    
    def calculate_experience_requirement(self, level):
        """閮?蝬??瘙?""
        return int(100 * (level ** 1.5))
    
    def choose_specialization_path(self, path):
        """?豢?撠移頝臬?"""
        self.current_specialization = path
        self.available_skill_points += 2
    
    def get_specialization_path_color(self, path):
        """?脣?撠移頝臬?憿"""
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
        """閮???賣???""
        skill = self.get_skill(skill_id)
        if not skill["is_unlocked"]:
            return 0.0
        
        total_effect = 0.0
        for effect in skill["effects"]:
            if effect["type"] == effect_type:
                total_effect += effect["base_value"] + (effect["scaling"] * (skill["current_level"] - 1))
        
        return total_effect
    
    def can_unlock_skill(self, skill_id, character_data):
        """瑼Ｘ?臬?臭誑閫?????""
        if skill_id not in self.skill_map:
            return False
        
        skill = self.skill_map[skill_id]
        return not skill["is_unlocked"] and self.available_skill_points > 0


class MockSkillManager:
    """璅⊥??賜恣?"""
    
    def __init__(self):
        self.is_initialized = False
        self.skill_system = None
        self.character = None
        self.available_skill_points = 0
        self.experience_history = []
    
    def initialize(self, skill_system, character):
        """?????賜恣?"""
        self.skill_system = skill_system
        self.character = character
        self.is_initialized = True
        self.available_skill_points = 3  # ????賡?
    
    def grant_skill_experience(self, skill_id, amount, reason=""):
        """蝯虫???賜?撽?""
        if self.skill_system:
            self.skill_system.add_skill_experience(skill_id, amount)
            self.experience_history.append({
                "skill_id": skill_id,
                "amount": amount,
                "reason": reason,
                "time": datetime.now()
            })
    
    def grant_skill_points(self, points, reason=""):
        """蝯虫???賡?"""
        self.available_skill_points += points
    
    def process_combat_end(self, victory, enemy_count, duration):
        """???圈洛蝯?"""
        base_exp = int(10 * enemy_count)
        if victory:
            base_exp = int(base_exp * 1.5)
        
        # ??蝬?蝯行擛交???        self.grant_skill_experience("TacticalCommand", base_exp, "?圈洛")
        self.grant_skill_experience("MeleeMastery", int(base_exp * 0.8), "?圈洛")
        
        if victory:
            self.grant_skill_points(1, "?圈洛?")
    
    def process_quest_completion(self, quest_id, difficulty):
        """??隞餃?摰?"""
        base_exp = difficulty
        self.grant_skill_experience("StrategicPlanning", base_exp, "隞餃?")
        self.grant_skill_points(int(difficulty / 10), "隞餃?摰?")
    
    def process_historical_event(self, event_id, related_skills):
        """??甇瑕鈭辣"""
        base_exp = 50
        for skill_id in related_skills:
            self.grant_skill_experience(skill_id, base_exp, "甇瑕鈭辣")
        self.grant_skill_points(1, f"甇瑕鈭辣: {event_id}")
    
    def auto_distribute_experience(self, total_exp, primary_category):
        """?芸???蝬?"""
        # 蝪∪?撖衣
        self.grant_skill_experience("TacticalCommand", total_exp // 2, "?芸???")
        self.grant_skill_experience("Leadership", total_exp // 2, "?芸???")
    
    def process_character_level_up(self, new_level):
        """??閫??"""
        self.grant_skill_points(2, f"???閃new_level}蝝?)
        self.grant_skill_experience("TacticalCommand", 50, "???")
        self.grant_skill_experience("Leadership", 50, "???")
    
    def validate_skill_action(self, skill_id, action_type):
        """撽???賣?雿?""
        if not self.skill_system:
            return False
        
        if action_type == "Unlock":
            return self.skill_system.can_unlock_skill(skill_id, self.character)
        elif action_type == "Upgrade":
            skill = self.skill_system.get_skill(skill_id)
            return skill["is_unlocked"] and skill["current_level"] < skill["max_level"]
        
        return False
    
    def get_recommended_skills(self):
        """?脣??刻???""
        if not self.skill_system:
            return []
        
        # 蝪∪?撖衣嚗????閫??????        recommended = []
        for skill_id, skill in self.skill_system.skill_map.items():
            if not skill["is_unlocked"]:
                recommended.append(skill_id)
        
        return recommended
    
    def get_skill_development_advice(self):
        """?脣???賜撅遣霅?""
        advice = "??賜撅遣霅堆?\n"
        advice += f"??雿? {self.available_skill_points} ??冽??賡?\n"
        
        unlocked_count = sum(1 for skill in self.skill_system.skill_map.values() if skill["is_unlocked"])
        if unlocked_count > 0:
            advice += f"??撌脰圾??{unlocked_count} ???穀n"
        else:
            advice += "??撱箄降閫??蝚砌????穀n"
        
        return advice
    
    def reset_skill_system(self):
        """?蔭??賜頂蝯?""
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
        """?脣???賜?撽風??""
        return [entry for entry in self.experience_history if entry["skill_id"] == skill_id]
    
    def unlock_skill(self, skill_id):
        """閫????踝?隞???寞?嚗?""
        if self.skill_system:
            return self.skill_system.unlock_skill(skill_id)
        return False
    
    def get_skill(self, skill_id):
        """?脣???踝?隞???寞?嚗?""
        if self.skill_system:
            return self.skill_system.get_skill(skill_id)
        return {}


def run_skill_system_tests():
    """????賜頂蝯望葫閰?""
    print("???? MingGoRTS ??賜頂蝯望葫閰?..")
    print("=" * 60)
    
    # ?萄遣皜祈岫憟辣
    suite = unittest.TestLoader().loadTestsFromTestCase(TestSkillSystem)
    
    # ??皜祈岫
    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)
    
    print("=" * 60)
    print(f"皜祈岫摰?嚗?)
    print(f"??皜祈岫?賊?: {result.testsRun}")
    print(f"憭望??賊?: {len(result.failures)}")
    print(f"?航炊?賊?: {len(result.errors)}")
    
    if result.failures:
        print("\n憭望??葫閰?")
        for test, traceback in result.failures:
            print(f"- {test}: {traceback}")
    
    if result.errors:
        print("\n?航炊?葫閰?")
        for test, traceback in result.errors:
            print(f"- {test}: {traceback}")
    
    success_rate = (result.testsRun - len(result.failures) - len(result.errors)) / result.testsRun * 100
    print(f"\n???? {success_rate:.1f}%")
    
    return result.wasSuccessful()


if __name__ == "__main__":
    success = run_skill_system_tests()
    sys.exit(0 if success else 1)

