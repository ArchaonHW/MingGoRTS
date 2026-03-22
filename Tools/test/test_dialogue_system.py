#!/usr/bin/env python3
"""
MingGoRTS 對話系統測試腳本
測試 Story 3-3: 敘事對話系統
"""

import unittest
import json
import sys
import os
from datetime import datetime
from enum import Enum

# 添加項目根目錄到 Python 路徑
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class EDialogueType(Enum):
    """對話類型枚舉"""
    Conversation = "Conversation"
    HistoricalEvent = "HistoricalEvent"
    MilitaryBriefing = "MilitaryBriefing"
    DiplomaticTalk = "DiplomaticTalk"
    PersonalStory = "PersonalStory"
    QuestDialogue = "QuestDialogue"

class EDialogueAttitude(Enum):
    """對話態度枚舉"""
    Friendly = "Friendly"
    Neutral = "Neutral"
    Suspicious = "Suspicious"
    Hostile = "Hostile"
    Respectful = "Respectful"
    Dismissive = "Dismissive"

class EDialogueOutcome(Enum):
    """對話結果枚舉"""
    NoEffect = "NoEffect"
    RelationshipChange = "RelationshipChange"
    QuestTrigger = "QuestTrigger"
    InformationGain = "InformationGain"
    SkillGain = "SkillGain"
    EventTrigger = "EventTrigger"

class ECharacterRole(Enum):
    """角色類型枚舉"""
    Leader = "Leader"
    Military = "Military"
    Political = "Political"
    Diplomatic = "Diplomatic"
    Intelligence = "Intelligence"
    Economic = "Economic"
    Cultural = "Cultural"
    Civilian = "Civilian"

class EHistoricalFaction(Enum):
    """歷史派系枚舉"""
    Nationalist = "Nationalist"
    Communist = "Communist"
    Warlord = "Warlord"
    Independent = "Independent"
    Foreign = "Foreign"

class TestDialogueSystem(unittest.TestCase):
    """對話系統測試類"""
    
    def setUp(self):
        """測試前的設置"""
        self.dialogue_system = MockDialogueSystem()
        self.historical_characters = MockHistoricalCharacters()
        
        # 測試對話數據
        self.test_dialogue = {
            "id": "ChiangKaiShek_FirstMeeting",
            "speaker_id": "ChiangKaiShek",
            "text": "年輕人，我看你氣宇不凡，想必是黃埔軍校的精英。",
            "type": EDialogueType.HistoricalEvent,
            "portrait": "/Game/Portraits/ChiangKaiShek.ChiangKaiShek",
            "options": [
                {
                    "text": "願為國家統一奉獻力量！",
                    "attitude": EDialogueAttitude.Respectful,
                    "next_dialogue": "ChiangKaiShek_LoyalResponse",
                    "outcomes": [
                        {
                            "type": EDialogueOutcome.RelationshipChange,
                            "target": "ChiangKaiShek",
                            "value": 20.0,
                            "description": "獲得蔣介石信任"
                        }
                    ]
                },
                {
                    "text": "我需要考慮一下您的提議。",
                    "attitude": EDialogueAttitude.Neutral,
                    "next_dialogue": "ChiangKaiShek_NeutralResponse",
                    "outcomes": [
                        {
                            "type": EDialogueOutcome.RelationshipChange,
                            "target": "ChiangKaiShek",
                            "value": 5.0,
                            "description": "保持中立關係"
                        }
                    ]
                }
            ],
            "trigger_conditions": [],
            "auto_outcomes": [],
            "historical_context": "NorthernExpedition",
            "is_important": True,
            "can_be_repeated": False
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
        
        # 測試歷史人物數據
        self.test_historical_character = {
            "id": "ChiangKaiShek",
            "name": "蔣介石",
            "courtesy_name": "介石",
            "birth_date": "1887-10-31",
            "death_date": "1975-04-05",
            "role": ECharacterRole.Leader,
            "faction": EHistoricalFaction.Nationalist,
            "title": "國民政府主席",
            "description": "中華民國國民黨總裁",
            "personality": {
                "openness": 0.6,
                "conscientiousness": 0.7,
                "extraversion": 0.6,
                "agreeableness": 0.5,
                "neuroticism": 0.4,
                "patriotism": 0.9,
                "ambition": 0.8
            },
            "is_alive": True,
            "is_available": True,
            "influence": 95.0
        }
    
    def test_dialogue_system_initialization(self):
        """測試對話系統初始化"""
        self.dialogue_system.initialize()
        
        self.assertTrue(self.dialogue_system.is_initialized)
        self.assertGreater(len(self.dialogue_system.all_dialogues), 0)
        self.assertGreater(len(self.dialogue_system.character_dialogue_map), 0)
        
        print("✅ 對話系統初始化測試通過")
    
    def test_dialogue_start(self):
        """測試開始對話"""
        self.dialogue_system.initialize()
        
        # 測試開始對話
        success = self.dialogue_system.start_dialogue("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertTrue(success)
        
        # 檢查當前對話
        current_dialogue = self.dialogue_system.current_dialogue
        self.assertEqual(current_dialogue["id"], "ChiangKaiShek_FirstMeeting")
        self.assertEqual(current_dialogue["speaker_id"], "ChiangKaiShek")
        
        print("✅ 開始對話測試通過")
    
    def test_dialogue_option_selection(self):
        """測試對話選項選擇"""
        self.dialogue_system.initialize()
        
        # 開始對話
        self.dialogue_system.start_dialogue("ChiangKaiShek_FirstMeeting", self.test_character)
        
        # 選擇第一個選項
        success = self.dialogue_system.select_dialogue_option(0)
        self.assertTrue(success)
        
        # 檢查對話歷史
        self.assertIn("ChiangKaiShek_FirstMeeting", self.dialogue_system.dialogue_history)
        
        print("✅ 對話選項選擇測試通過")
    
    def test_dialogue_availability_check(self):
        """測試對話可用性檢查"""
        self.dialogue_system.initialize()
        
        # 測試可用對話
        is_available = self.dialogue_system.is_dialogue_available("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertTrue(is_available)
        
        # 測試不存在的對話
        is_available = self.dialogue_system.is_dialogue_available("NonExistentDialogue", self.test_character)
        self.assertFalse(is_available)
        
        print("✅ 對話可用性檢查測試通過")
    
    def test_dialogue_option_availability(self):
        """測試對話選項可用性"""
        self.dialogue_system.initialize()
        
        # 測試可用選項
        is_available = self.dialogue_system.is_dialogue_option_available("ChiangKaiShek_FirstMeeting", 0, self.test_character)
        self.assertTrue(is_available)
        
        # 測試無效選項索引
        is_available = self.dialogue_system.is_dialogue_option_available("ChiangKaiShek_FirstMeeting", 99, self.test_character)
        self.assertFalse(is_available)
        
        print("✅ 對話選項可用性測試通過")
    
    def test_dialogue_outcomes_calculation(self):
        """測試對話結果計算"""
        self.dialogue_system.initialize()
        
        # 計算對話結果
        outcomes = self.dialogue_system.calculate_dialogue_outcomes("ChiangKaiShek_FirstMeeting", 0)
        self.assertGreater(len(outcomes), 0)
        
        # 檢查結果類型
        for outcome in outcomes:
            self.assertIn(outcome["type"], [e.value for e in EDialogueOutcome])
        
        print("✅ 對話結果計算測試通過")
    
    def test_dialogue_history(self):
        """測試對話歷史"""
        self.dialogue_system.initialize()
        
        # 開始對話
        self.dialogue_system.start_dialogue("ChiangKaiShek_FirstMeeting", self.test_character)
        
        # 檢查歷史
        history = self.dialogue_system.get_dialogue_history()
        self.assertIn("ChiangKaiShek_FirstMeeting", history)
        
        # 清空歷史
        self.dialogue_system.clear_dialogue_history()
        history = self.dialogue_system.get_dialogue_history()
        self.assertEqual(len(history), 0)
        
        print("✅ 對話歷史測試通過")
    
    def test_character_dialogues(self):
        """測試角色對話"""
        self.dialogue_system.initialize()
        
        # 獲取角色對話
        character_dialogues = self.dialogue_system.get_character_dialogues("ChiangKaiShek")
        self.assertGreater(len(character_dialogues), 0)
        
        # 檢查對話講者
        for dialogue in character_dialogues:
            self.assertEqual(dialogue["speaker_id"], "ChiangKaiShek")
        
        print("✅ 角色對話測試通過")
    
    def test_important_dialogues(self):
        """測試重要對話"""
        self.dialogue_system.initialize()
        
        # 獲取重要對話
        important_dialogues = self.dialogue_system.get_important_dialogues()
        self.assertGreater(len(important_dialogues), 0)
        
        # 檢查重要對話標記
        for dialogue in important_dialogues:
            self.assertTrue(dialogue["is_important"])
        
        print("✅ 重要對話測試通過")
    
    def test_historical_characters_initialization(self):
        """測試歷史人物系統初始化"""
        self.historical_characters.initialize()
        
        self.assertTrue(self.historical_characters.is_initialized)
        self.assertGreater(len(self.historical_characters.all_characters), 0)
        self.assertGreater(len(self.historical_characters.character_map), 0)
        
        print("✅ 歷史人物系統初始化測試通過")
    
    def test_character_retrieval(self):
        """測試角色獲取"""
        self.historical_characters.initialize()
        
        # 獲取特定角色
        character = self.historical_characters.get_character("ChiangKaiShek")
        self.assertEqual(character["name"], "蔣介石")
        self.assertEqual(character["role"], ECharacterRole.Leader)
        
        # 測試不存在的角色
        character = self.historical_characters.get_character("NonExistentCharacter")
        self.assertEqual(character["name"], "")
        
        print("✅ 角色獲取測試通過")
    
    def test_character_relationships(self):
        """測試角色關係"""
        self.historical_characters.initialize()
        
        # 獲取初始關係
        relationship = self.historical_characters.get_character_relationship("ChiangKaiShek")
        self.assertEqual(relationship, 50.0)  # 默認中立
        
        # 更新關係
        self.historical_characters.update_character_relationship("ChiangKaiShek", 20.0)
        new_relationship = self.historical_characters.get_character_relationship("ChiangKaiShek")
        self.assertEqual(new_relationship, 70.0)
        
        print("✅ 角色關係測試通過")
    
    def test_character_availability(self):
        """測試角色可用性"""
        self.historical_characters.initialize()
        
        # 檢查可用角色
        is_available = self.historical_characters.is_character_available("ChiangKaiShek")
        self.assertTrue(is_available)
        
        # 設置不可用
        self.historical_characters.set_character_availability("ChiangKaiShek", False)
        is_available = self.historical_characters.is_character_available("ChiangKaiShek")
        self.assertFalse(is_available)
        
        print("✅ 角色可用性測試通過")
    
    def test_character_factions(self):
        """測試角色派系"""
        self.historical_characters.initialize()
        
        # 獲取國民黨角色
        nationalist_characters = self.historical_characters.get_characters_by_faction(EHistoricalFaction.Nationalist)
        self.assertGreater(len(nationalist_characters), 0)
        
        # 檢查派系
        for character in nationalist_characters:
            self.assertEqual(character["faction"], EHistoricalFaction.Nationalist)
        
        print("✅ 角色派系測試通過")
    
    def test_character_roles(self):
        """測試角色類型"""
        self.historical_characters.initialize()
        
        # 獲取領袖角色
        leader_characters = self.historical_characters.get_characters_by_role(ECharacterRole.Leader)
        self.assertGreater(len(leader_characters), 0)
        
        # 檢查角色類型
        for character in leader_characters:
            self.assertEqual(character["role"], ECharacterRole.Leader)
        
        print("✅ 角色類型測試通過")
    
    def test_character_response_generation(self):
        """測試角色回應生成"""
        self.historical_characters.initialize()
        
        # 生成角色回應
        response = self.historical_characters.generate_character_response("ChiangKaiShek", EDialogueAttitude.Respectful, "FirstMeeting")
        self.assertIsInstance(response, str)
        self.assertGreater(len(response), 0)
        
        # 測試不存在的角色
        response = self.historical_characters.generate_character_response("NonExistentCharacter", EDialogueAttitude.Friendly, "Test")
        self.assertEqual(response, "我無法理解你的意思。")
        
        print("✅ 角色回應生成測試通過")
    
    def test_character_influence_calculation(self):
        """測試角色影響力計算"""
        self.historical_characters.initialize()
        
        # 計算角色影響力
        influence = self.historical_characters.calculate_character_influence("ChiangKaiShek")
        self.assertGreater(influence, 0)
        self.assertLessEqual(influence, 100)
        
        print("✅ 角色影響力計算測試通過")
    
    def test_character_dialogue_recommendations(self):
        """測試角色對話推薦"""
        self.historical_characters.initialize()
        
        # 獲取推薦對話
        recommended_dialogues = self.historical_characters.get_recommended_dialogues("ChiangKaiShek", self.test_character)
        self.assertIsInstance(recommended_dialogues, list)
        
        print("✅ 角色對話推薦測試通過")
    
    def test_complete_dialogue_flow(self):
        """測試完整對話流程"""
        self.dialogue_system.initialize()
        self.historical_characters.initialize()
        
        # 1. 檢查角色可用性
        is_character_available = self.historical_characters.is_character_available("ChiangKaiShek")
        self.assertTrue(is_character_available)
        
        # 2. 檢查對話可用性
        is_dialogue_available = self.dialogue_system.is_dialogue_available("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertTrue(is_dialogue_available)
        
        # 3. 開始對話
        success = self.dialogue_system.start_dialogue("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertTrue(success)
        
        # 4. 獲取可用選項
        available_options = self.dialogue_system.get_available_dialogue_options("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertGreater(len(available_options), 0)
        
        # 5. 選擇選項
        success = self.dialogue_system.select_dialogue_option(0)
        self.assertTrue(success)
        
        # 6. 檢查結果
        outcomes = self.dialogue_system.calculate_dialogue_outcomes("ChiangKaiShek_FirstMeeting", 0)
        self.assertGreater(len(outcomes), 0)
        
        # 7. 應用結果
        self.dialogue_system.apply_dialogue_outcomes(outcomes)
        
        # 8. 檢查關係變化
        new_relationship = self.historical_characters.get_character_relationship("ChiangKaiShek")
        self.assertGreater(new_relationship, 50.0)  # 應該有所提升
        
        # 9. 檢查歷史記錄
        history = self.dialogue_system.get_dialogue_history()
        self.assertIn("ChiangKaiShek_FirstMeeting", history)
        
        print("✅ 完整對話流程測試通過")
    
    def test_historical_accuracy(self):
        """測試歷史準確性"""
        self.historical_characters.initialize()
        
        # 檢查蔣介石的歷史信息
        chiang = self.historical_characters.get_character("ChiangKaiShek")
        self.assertEqual(chiang["birth_date"], "1887-10-31")
        self.assertEqual(chiang["death_date"], "1975-04-05")
        self.assertEqual(chiang["faction"], EHistoricalFaction.Nationalist)
        self.assertEqual(chiang["role"], ECharacterRole.Leader)
        
        # 檢查毛澤東的歷史信息
        mao = self.historical_characters.get_character("MaoZedong")
        self.assertEqual(mao["birth_date"], "1893-12-26")
        self.assertEqual(mao["death_date"], "1976-09-09")
        self.assertEqual(mao["faction"], EHistoricalFaction.Communist)
        self.assertEqual(mao["role"], ECharacterRole.Leader)
        
        print("✅ 歷史準確性測試通過")
    
    def test_dialogue_types(self):
        """測試對話類型"""
        self.dialogue_system.initialize()
        
        # 檢查不同類型的對話
        all_dialogues = self.dialogue_system.get_all_dialogues()
        dialogue_types = set()
        
        for dialogue in all_dialogues:
            dialogue_types.add(dialogue["type"])
        
        # 應該包含多種對話類型
        self.assertIn(EDialogueType.HistoricalEvent, dialogue_types)
        self.assertIn(EDialogueType.MilitaryBriefing, dialogue_types)
        self.assertIn(EDialogueType.DiplomaticTalk, dialogue_types)
        
        print("✅ 對話類型測試通過")
    
    def test_attitude_system(self):
        """測試態度系統"""
        self.historical_characters.initialize()
        
        # 測試不同態度的回應
        attitudes = [
            EDialogueAttitude.Friendly,
            EDialogueAttitude.Respectful,
            EDialogueAttitude.Neutral,
            EDialogueAttitude.Hostile,
            EDialogueAttitude.Suspicious,
            EDialogueAttitude.Dismissive
        ]
        
        for attitude in attitudes:
            response = self.historical_characters.generate_character_response("ChiangKaiShek", attitude, "Test")
            self.assertIsInstance(response, str)
            self.assertGreater(len(response), 0)
        
        print("✅ 態度系統測試通過")


class MockDialogueSystem:
    """模擬對話系統"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_dialogues = []
        self.character_dialogue_map = {}
        self.dialogue_map = {}
        self.current_dialogue = {}
        self.dialogue_history = []
        self._initialize_mock_dialogues()
    
    def initialize(self):
        """初始化對話系統"""
        self.is_initialized = True
        self._build_dialogue_maps()
    
    def _initialize_mock_dialogues(self):
        """初始化模擬對話"""
        dialogues = [
            {
                "id": "ChiangKaiShek_FirstMeeting",
                "speaker_id": "ChiangKaiShek",
                "text": "年輕人，我看你氣宇不凡，想必是黃埔軍校的精英。",
                "type": EDialogueType.HistoricalEvent,
                "portrait": "/Game/Portraits/ChiangKaiShek.ChiangKaiShek",
                "options": [
                    {
                        "text": "願為國家統一奉獻力量！",
                        "attitude": EDialogueAttitude.Respectful,
                        "next_dialogue": "ChiangKaiShek_LoyalResponse",
                        "outcomes": [
                            {
                                "type": EDialogueOutcome.RelationshipChange,
                                "target": "ChiangKaiShek",
                                "value": 20.0,
                                "description": "獲得蔣介石信任"
                            }
                        ]
                    },
                    {
                        "text": "我需要考慮一下您的提議。",
                        "attitude": EDialogueAttitude.Neutral,
                        "next_dialogue": "ChiangKaiShek_NeutralResponse",
                        "outcomes": [
                            {
                                "type": EDialogueOutcome.RelationshipChange,
                                "target": "ChiangKaiShek",
                                "value": 5.0,
                                "description": "保持中立關係"
                            }
                        ]
                    }
                ],
                "trigger_conditions": [],
                "auto_outcomes": [],
                "historical_context": "NorthernExpedition",
                "is_important": True,
                "can_be_repeated": False
            },
            {
                "id": "MaoZedong_FirstMeeting",
                "speaker_id": "MaoZedong",
                "text": "同志，我看你心懷天下，關心勞苦大眾。",
                "type": EDialogueType.HistoricalEvent,
                "portrait": "/Game/Portraits/MaoZedong.MaoZedong",
                "options": [
                    {
                        "text": "我願意加入革命事業！",
                        "attitude": EDialogueAttitude.Friendly,
                        "next_dialogue": "MaoZedong_RevolutionaryResponse",
                        "outcomes": [
                            {
                                "type": EDialogueOutcome.QuestTrigger,
                                "target": "RevolutionaryQuest",
                                "value": 1.0,
                                "description": "觸發革命任務線"
                            }
                        ]
                    }
                ],
                "trigger_conditions": [],
                "auto_outcomes": [],
                "historical_context": "RevolutionaryMovement",
                "is_important": True,
                "can_be_repeated": False
            }
        ]
        
        self.all_dialogues = dialogues
    
    def _build_dialogue_maps(self):
        """構建對話映射"""
        for dialogue in self.all_dialogues:
            self.dialogue_map[dialogue["id"]] = dialogue
            
            if dialogue["speaker_id"] not in self.character_dialogue_map:
                self.character_dialogue_map[dialogue["speaker_id"]] = []
            self.character_dialogue_map[dialogue["speaker_id"]].append(dialogue["id"])
    
    def start_dialogue(self, dialogue_id, player_character):
        """開始對話"""
        if dialogue_id in self.dialogue_map:
            self.current_dialogue = self.dialogue_map[dialogue_id].copy()
            self.add_to_history(dialogue_id)
            return True
        return False
    
    def select_dialogue_option(self, option_index):
        """選擇對話選項"""
        if self.current_dialogue and option_index < len(self.current_dialogue["options"]):
            selected_option = self.current_dialogue["options"][option_index]
            
            # 應用結果
            if "outcomes" in selected_option:
                self.apply_dialogue_outcomes(selected_option["outcomes"])
            
            # 檢查是否有下一個對話
            if "next_dialogue" in selected_option and selected_option["next_dialogue"]:
                return self.start_dialogue(selected_option["next_dialogue"], {})
            else:
                # 對話結束
                self.current_dialogue = {}
                return True
        return False
    
    def is_dialogue_available(self, dialogue_id, player_character):
        """檢查對話是否可用"""
        return dialogue_id in self.dialogue_map
    
    def is_dialogue_option_available(self, dialogue_id, option_index, player_character):
        """檢查對話選項是否可用"""
        if dialogue_id in self.dialogue_map:
            dialogue = self.dialogue_map[dialogue_id]
            return option_index < len(dialogue["options"])
        return False
    
    def get_available_dialogue_options(self, dialogue_id, player_character):
        """獲取可用對話選項"""
        if dialogue_id in self.dialogue_map:
            dialogue = self.dialogue_map[dialogue_id]
            return dialogue["options"].copy()
        return []
    
    def calculate_dialogue_outcomes(self, dialogue_id, option_index):
        """計算對話結果"""
        outcomes = []
        
        if dialogue_id in self.dialogue_map:
            dialogue = self.dialogue_map[dialogue_id]
            
            # 添加自動結果
            if "auto_outcomes" in dialogue:
                outcomes.extend(dialogue["auto_outcomes"])
            
            # 添加選項結果
            if option_index < len(dialogue["options"]):
                option = dialogue["options"][option_index]
                if "outcomes" in option:
                    outcomes.extend(option["outcomes"])
        
        return outcomes
    
    def apply_dialogue_outcomes(self, outcomes):
        """應用對話結果"""
        # 簡化實現，只記錄結果
        for outcome in outcomes:
            print(f"應用結果: {outcome['type']} - {outcome['description']}")
    
    def get_dialogue_history(self):
        """獲取對話歷史"""
        return self.dialogue_history.copy()
    
    def add_to_history(self, dialogue_id):
        """添加到歷史"""
        if dialogue_id not in self.dialogue_history:
            self.dialogue_history.append(dialogue_id)
    
    def clear_dialogue_history(self):
        """清空對話歷史"""
        self.dialogue_history.clear()
    
    def get_all_dialogues(self):
        """獲取所有對話"""
        return self.all_dialogues.copy()
    
    def get_character_dialogues(self, character_id):
        """獲取角色對話"""
        if character_id in self.character_dialogue_map:
            dialogue_ids = self.character_dialogue_map[character_id]
            return [self.dialogue_map[did] for did in dialogue_ids if did in self.dialogue_map]
        return []
    
    def get_important_dialogues(self):
        """獲取重要對話"""
        return [dialogue for dialogue in self.all_dialogues if dialogue.get("is_important", False)]


class MockHistoricalCharacters:
    """模擬歷史人物系統"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_characters = []
        self.character_map = {}
        self.character_relationships = {}
        self.character_availability = {}
        self._initialize_mock_characters()
    
    def initialize(self):
        """初始化歷史人物系統"""
        self.is_initialized = True
        self._build_character_maps()
    
    def _initialize_mock_characters(self):
        """初始化模擬歷史人物"""
        characters = [
            {
                "id": "ChiangKaiShek",
                "name": "蔣介石",
                "courtesy_name": "介石",
                "birth_date": "1887-10-31",
                "death_date": "1975-04-05",
                "role": ECharacterRole.Leader,
                "faction": EHistoricalFaction.Nationalist,
                "title": "國民政府主席",
                "description": "中華民國國民黨總裁",
                "personality": {
                    "openness": 0.6,
                    "conscientiousness": 0.7,
                    "extraversion": 0.6,
                    "agreeableness": 0.5,
                    "neuroticism": 0.4,
                    "patriotism": 0.9,
                    "ambition": 0.8
                },
                "is_alive": True,
                "is_available": True,
                "influence": 95.0
            },
            {
                "id": "MaoZedong",
                "name": "毛澤東",
                "courtesy_name": "潤之",
                "birth_date": "1893-12-26",
                "death_date": "1976-09-09",
                "role": ECharacterRole.Leader,
                "faction": EHistoricalFaction.Communist,
                "title": "中國共產黨主席",
                "description": "中國共產黨創始人之一",
                "personality": {
                    "openness": 0.7,
                    "conscientiousness": 0.8,
                    "extraversion": 0.6,
                    "agreeableness": 0.6,
                    "neuroticism": 0.5,
                    "patriotism": 0.9,
                    "ambition": 0.8
                },
                "is_alive": True,
                "is_available": True,
                "influence": 90.0
            },
            {
                "id": "ZhouEnlai",
                "name": "周恩來",
                "courtesy_name": "翔宇",
                "birth_date": "1898-03-05",
                "death_date": "1976-01-08",
                "role": ECharacterRole.Diplomatic,
                "faction": EHistoricalFaction.Communist,
                "title": "國務院總理",
                "description": "中國共產黨重要領導人",
                "personality": {
                    "openness": 0.7,
                    "conscientiousness": 0.9,
                    "extraversion": 0.7,
                    "agreeableness": 0.8,
                    "neuroticism": 0.4,
                    "patriotism": 0.8,
                    "ambition": 0.6
                },
                "is_alive": True,
                "is_available": True,
                "influence": 85.0
            }
        ]
        
        self.all_characters = characters
    
    def _build_character_maps(self):
        """構建角色映射"""
        for character in self.all_characters:
            self.character_map[character["id"]] = character.copy()
            self.character_relationships[character["id"]] = 50.0  # 默認中立
            self.character_availability[character["id"]] = character["is_available"]
    
    def get_character(self, character_id):
        """獲取角色"""
        if character_id in self.character_map:
            return self.character_map[character_id].copy()
        return {"name": "", "id": ""}
    
    def get_character_relationship(self, character_id):
        """獲取角色關係"""
        return self.character_relationships.get(character_id, 50.0)
    
    def update_character_relationship(self, character_id, relationship_change):
        """更新角色關係"""
        current = self.character_relationships.get(character_id, 50.0)
        new_relationship = max(0.0, min(100.0, current + relationship_change))
        self.character_relationships[character_id] = new_relationship
    
    def is_character_available(self, character_id):
        """檢查角色是否可用"""
        return self.character_availability.get(character_id, False)
    
    def set_character_availability(self, character_id, is_available):
        """設置角色可用性"""
        self.character_availability[character_id] = is_available
    
    def get_characters_by_faction(self, faction):
        """獲取特定派系角色"""
        return [character for character in self.all_characters if character["faction"] == faction and character["is_available"]]
    
    def get_characters_by_role(self, role):
        """獲取特定角色類型"""
        return [character for character in self.all_characters if character["role"] == role and character["is_available"]]
    
    def generate_character_response(self, character_id, attitude, context):
        """生成角色回應"""
        if character_id not in self.character_map:
            return "我無法理解你的意思。"
        
        character = self.character_map[character_id]
        
        # 根據角色和態度生成回應
        if attitude == EDialogueAttitude.Respectful:
            if character["role"] == ECharacterRole.Leader:
                return "作為領袖，我認為你的敬意我感受到了。"
            else:
                return "謝謝你的尊重。"
        elif attitude == EDialogueAttitude.Friendly:
            return "我很高興能與你交流。"
        elif attitude == EDialogueAttitude.Hostile:
            return "你的態度讓我感到不安。"
        else:
            return "我明白你的意思。"
    
    def calculate_character_influence(self, character_id):
        """計算角色影響力"""
        if character_id not in self.character_map:
            return 0.0
        
        character = self.character_map[character_id]
        base_influence = character["influence"]
        relationship_bonus = (self.get_character_relationship(character_id) - 50.0) * 0.2
        
        return max(0.0, min(100.0, base_influence + relationship_bonus))
    
    def get_recommended_dialogues(self, character_id, player_character):
        """獲取推薦對話"""
        # 簡化實現
        relationship = self.get_character_relationship(character_id)
        
        if relationship > 70.0:
            return [f"{character_id}_FriendlyDialogue"]
        elif relationship < 30.0:
            return [f"{character_id}_FormalDialogue"]
        else:
            return [f"{character_id}_NeutralDialogue"]


def run_dialogue_system_tests():
    """運行對話系統測試"""
    print("開始運行 MingGoRTS 對話系統測試...")
    print("=" * 60)
    
    # 創建測試套件
    suite = unittest.TestLoader().loadTestsFromTestCase(TestDialogueSystem)
    
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
    success = run_dialogue_system_tests()
    sys.exit(0 if success else 1)
