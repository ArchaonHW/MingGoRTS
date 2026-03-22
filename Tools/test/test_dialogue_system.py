#!/usr/bin/env python3
"""
MingGoRTS 撠店蝟餌絞皜祈岫?單
皜祈岫 Story 3-3: ??撠店蝟餌絞
"""

import unittest
import json
import sys
import os
from datetime import datetime
from enum import Enum

# 瘛餃???寧? Python 頝臬?
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class EDialogueType(Enum):
    """撠店憿???"""
    Conversation = "Conversation"
    HistoricalEvent = "HistoricalEvent"
    MilitaryBriefing = "MilitaryBriefing"
    DiplomaticTalk = "DiplomaticTalk"
    PersonalStory = "PersonalStory"
    QuestDialogue = "QuestDialogue"

class EDialogueAttitude(Enum):
    """撠店?漲??"""
    Friendly = "Friendly"
    Neutral = "Neutral"
    Suspicious = "Suspicious"
    Hostile = "Hostile"
    Respectful = "Respectful"
    Dismissive = "Dismissive"

class EDialogueOutcome(Enum):
    """撠店蝯???"""
    NoEffect = "NoEffect"
    RelationshipChange = "RelationshipChange"
    QuestTrigger = "QuestTrigger"
    InformationGain = "InformationGain"
    SkillGain = "SkillGain"
    EventTrigger = "EventTrigger"

class ECharacterRole(Enum):
    """閫憿???"""
    Leader = "Leader"
    Military = "Military"
    Political = "Political"
    Diplomatic = "Diplomatic"
    Intelligence = "Intelligence"
    Economic = "Economic"
    Cultural = "Cultural"
    Civilian = "Civilian"

class EHistoricalFaction(Enum):
    """甇瑕瘣曄頂??"""
    Nationalist = "Nationalist"
    Communist = "Communist"
    Warlord = "Warlord"
    Independent = "Independent"
    Foreign = "Foreign"

class TestDialogueSystem(unittest.TestCase):
    """撠店蝟餌絞皜祈岫憿?""
    
    def setUp(self):
        """皜祈岫??閮剔蔭"""
        self.dialogue_system = MockDialogueSystem()
        self.historical_characters = MockHistoricalCharacters()
        
        # 皜祈岫撠店?豢?
        self.test_dialogue = {
            "id": "ChiangKaiShek_FirstMeeting",
            "speaker_id": "ChiangKaiShek",
            "text": "撟渲?鈭綽???雿除摰??∴??喳??舫????∠?蝎曇??,
            "type": EDialogueType.HistoricalEvent,
            "portrait": "/Game/Portraits/ChiangKaiShek.ChiangKaiShek",
            "options": [
                {
                    "text": "憿?振蝯曹?憟??嚗?,
                    "attitude": EDialogueAttitude.Respectful,
                    "next_dialogue": "ChiangKaiShek_LoyalResponse",
                    "outcomes": [
                        {
                            "type": EDialogueOutcome.RelationshipChange,
                            "target": "ChiangKaiShek",
                            "value": 20.0,
                            "description": "?脣?????喃縑隞?
                        }
                    ]
                },
                {
                    "text": "??閬銝銝??霅啜?,
                    "attitude": EDialogueAttitude.Neutral,
                    "next_dialogue": "ChiangKaiShek_NeutralResponse",
                    "outcomes": [
                        {
                            "type": EDialogueOutcome.RelationshipChange,
                            "target": "ChiangKaiShek",
                            "value": 5.0,
                            "description": "靽?銝剔???"
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
        
        # 皜祈岫甇瑕鈭箇?豢?
        self.test_historical_character = {
            "id": "ChiangKaiShek",
            "name": "?????,
            "courtesy_name": "隞",
            "birth_date": "1887-10-31",
            "death_date": "1975-04-05",
            "role": ECharacterRole.Leader,
            "faction": EHistoricalFaction.Nationalist,
            "title": "???踹?銝餃葉",
            "description": "銝剛瘞???暺函蜇鋆?,
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
        """皜祈岫撠店蝟餌絞????""
        self.dialogue_system.initialize()
        
        self.assertTrue(self.dialogue_system.is_initialized)
        self.assertGreater(len(self.dialogue_system.all_dialogues), 0)
        self.assertGreater(len(self.dialogue_system.character_dialogue_map), 0)
        
        print("??撠店蝟餌絞???葫閰阡?")
    
    def test_dialogue_start(self):
        """皜祈岫??撠店"""
        self.dialogue_system.initialize()
        
        # 皜祈岫??撠店
        success = self.dialogue_system.start_dialogue("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertTrue(success)
        
        # 瑼Ｘ?嗅?撠店
        current_dialogue = self.dialogue_system.current_dialogue
        self.assertEqual(current_dialogue["id"], "ChiangKaiShek_FirstMeeting")
        self.assertEqual(current_dialogue["speaker_id"], "ChiangKaiShek")
        
        print("????撠店皜祈岫??")
    
    def test_dialogue_option_selection(self):
        """皜祈岫撠店?賊??豢?"""
        self.dialogue_system.initialize()
        
        # ??撠店
        self.dialogue_system.start_dialogue("ChiangKaiShek_FirstMeeting", self.test_character)
        
        # ?豢?蝚砌????        success = self.dialogue_system.select_dialogue_option(0)
        self.assertTrue(success)
        
        # 瑼Ｘ撠店甇瑕
        self.assertIn("ChiangKaiShek_FirstMeeting", self.dialogue_system.dialogue_history)
        
        print("??撠店?賊??豢?皜祈岫??")
    
    def test_dialogue_availability_check(self):
        """皜祈岫撠店?舐?扳炎??""
        self.dialogue_system.initialize()
        
        # 皜祈岫?舐撠店
        is_available = self.dialogue_system.is_dialogue_available("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertTrue(is_available)
        
        # 皜祈岫銝??函?撠店
        is_available = self.dialogue_system.is_dialogue_available("NonExistentDialogue", self.test_character)
        self.assertFalse(is_available)
        
        print("??撠店?舐?扳炎?交葫閰阡?")
    
    def test_dialogue_option_availability(self):
        """皜祈岫撠店?賊??舐??""
        self.dialogue_system.initialize()
        
        # 皜祈岫?舐?賊?
        is_available = self.dialogue_system.is_dialogue_option_available("ChiangKaiShek_FirstMeeting", 0, self.test_character)
        self.assertTrue(is_available)
        
        # 皜祈岫?⊥??賊?蝝Ｗ?
        is_available = self.dialogue_system.is_dialogue_option_available("ChiangKaiShek_FirstMeeting", 99, self.test_character)
        self.assertFalse(is_available)
        
        print("??撠店?賊??舐?扳葫閰阡?")
    
    def test_dialogue_outcomes_calculation(self):
        """皜祈岫撠店蝯?閮?"""
        self.dialogue_system.initialize()
        
        # 閮?撠店蝯?
        outcomes = self.dialogue_system.calculate_dialogue_outcomes("ChiangKaiShek_FirstMeeting", 0)
        self.assertGreater(len(outcomes), 0)
        
        # 瑼Ｘ蝯?憿?
        for outcome in outcomes:
            self.assertIn(outcome["type"], [e.value for e in EDialogueOutcome])
        
        print("??撠店蝯?閮?皜祈岫??")
    
    def test_dialogue_history(self):
        """皜祈岫撠店甇瑕"""
        self.dialogue_system.initialize()
        
        # ??撠店
        self.dialogue_system.start_dialogue("ChiangKaiShek_FirstMeeting", self.test_character)
        
        # 瑼Ｘ甇瑕
        history = self.dialogue_system.get_dialogue_history()
        self.assertIn("ChiangKaiShek_FirstMeeting", history)
        
        # 皜征甇瑕
        self.dialogue_system.clear_dialogue_history()
        history = self.dialogue_system.get_dialogue_history()
        self.assertEqual(len(history), 0)
        
        print("??撠店甇瑕皜祈岫??")
    
    def test_character_dialogues(self):
        """皜祈岫閫撠店"""
        self.dialogue_system.initialize()
        
        # ?脣?閫撠店
        character_dialogues = self.dialogue_system.get_character_dialogues("ChiangKaiShek")
        self.assertGreater(len(character_dialogues), 0)
        
        # 瑼Ｘ撠店雓?        for dialogue in character_dialogues:
            self.assertEqual(dialogue["speaker_id"], "ChiangKaiShek")
        
        print("??閫撠店皜祈岫??")
    
    def test_important_dialogues(self):
        """皜祈岫??撠店"""
        self.dialogue_system.initialize()
        
        # ?脣???撠店
        important_dialogues = self.dialogue_system.get_important_dialogues()
        self.assertGreater(len(important_dialogues), 0)
        
        # 瑼Ｘ??撠店璅?
        for dialogue in important_dialogues:
            self.assertTrue(dialogue["is_important"])
        
        print("????撠店皜祈岫??")
    
    def test_historical_characters_initialization(self):
        """皜祈岫甇瑕鈭箇蝟餌絞????""
        self.historical_characters.initialize()
        
        self.assertTrue(self.historical_characters.is_initialized)
        self.assertGreater(len(self.historical_characters.all_characters), 0)
        self.assertGreater(len(self.historical_characters.character_map), 0)
        
        print("??甇瑕鈭箇蝟餌絞???葫閰阡?")
    
    def test_character_retrieval(self):
        """皜祈岫閫?脣?"""
        self.historical_characters.initialize()
        
        # ?脣??孵?閫
        character = self.historical_characters.get_character("ChiangKaiShek")
        self.assertEqual(character["name"], "?????)
        self.assertEqual(character["role"], ECharacterRole.Leader)
        
        # 皜祈岫銝??函?閫
        character = self.historical_characters.get_character("NonExistentCharacter")
        self.assertEqual(character["name"], "")
        
        print("??閫?脣?皜祈岫??")
    
    def test_character_relationships(self):
        """皜祈岫閫??"""
        self.historical_characters.initialize()
        
        # ?脣?????
        relationship = self.historical_characters.get_character_relationship("ChiangKaiShek")
        self.assertEqual(relationship, 50.0)  # 暺?銝剔?
        
        # ?湔??
        self.historical_characters.update_character_relationship("ChiangKaiShek", 20.0)
        new_relationship = self.historical_characters.get_character_relationship("ChiangKaiShek")
        self.assertEqual(new_relationship, 70.0)
        
        print("??閫??皜祈岫??")
    
    def test_character_availability(self):
        """皜祈岫閫?舐??""
        self.historical_characters.initialize()
        
        # 瑼Ｘ?舐閫
        is_available = self.historical_characters.is_character_available("ChiangKaiShek")
        self.assertTrue(is_available)
        
        # 閮剔蔭銝??        self.historical_characters.set_character_availability("ChiangKaiShek", False)
        is_available = self.historical_characters.is_character_available("ChiangKaiShek")
        self.assertFalse(is_available)
        
        print("??閫?舐?扳葫閰阡?")
    
    def test_character_factions(self):
        """皜祈岫閫瘣曄頂"""
        self.historical_characters.initialize()
        
        # ?脣???暺刻???        nationalist_characters = self.historical_characters.get_characters_by_faction(EHistoricalFaction.Nationalist)
        self.assertGreater(len(nationalist_characters), 0)
        
        # 瑼Ｘ瘣曄頂
        for character in nationalist_characters:
            self.assertEqual(character["faction"], EHistoricalFaction.Nationalist)
        
        print("??閫瘣曄頂皜祈岫??")
    
    def test_character_roles(self):
        """皜祈岫閫憿?"""
        self.historical_characters.initialize()
        
        # ?脣???閫
        leader_characters = self.historical_characters.get_characters_by_role(ECharacterRole.Leader)
        self.assertGreater(len(leader_characters), 0)
        
        # 瑼Ｘ閫憿?
        for character in leader_characters:
            self.assertEqual(character["role"], ECharacterRole.Leader)
        
        print("??閫憿?皜祈岫??")
    
    def test_character_response_generation(self):
        """皜祈岫閫????"""
        self.historical_characters.initialize()
        
        # ??閫??
        response = self.historical_characters.generate_character_response("ChiangKaiShek", EDialogueAttitude.Respectful, "FirstMeeting")
        self.assertIsInstance(response, str)
        self.assertGreater(len(response), 0)
        
        # 皜祈岫銝??函?閫
        response = self.historical_characters.generate_character_response("NonExistentCharacter", EDialogueAttitude.Friendly, "Test")
        self.assertEqual(response, "?瘜?閫??????)
        
        print("??閫????皜祈岫??")
    
    def test_character_influence_calculation(self):
        """皜祈岫閫敶梢??蝞?""
        self.historical_characters.initialize()
        
        # 閮?閫敶梢??        influence = self.historical_characters.calculate_character_influence("ChiangKaiShek")
        self.assertGreater(influence, 0)
        self.assertLessEqual(influence, 100)
        
        print("??閫敶梢??蝞葫閰阡?")
    
    def test_character_dialogue_recommendations(self):
        """皜祈岫閫撠店?刻"""
        self.historical_characters.initialize()
        
        # ?脣??刻撠店
        recommended_dialogues = self.historical_characters.get_recommended_dialogues("ChiangKaiShek", self.test_character)
        self.assertIsInstance(recommended_dialogues, list)
        
        print("??閫撠店?刻皜祈岫??")
    
    def test_complete_dialogue_flow(self):
        """皜祈岫摰撠店瘚?"""
        self.dialogue_system.initialize()
        self.historical_characters.initialize()
        
        # 1. 瑼Ｘ閫?舐??        is_character_available = self.historical_characters.is_character_available("ChiangKaiShek")
        self.assertTrue(is_character_available)
        
        # 2. 瑼Ｘ撠店?舐??        is_dialogue_available = self.dialogue_system.is_dialogue_available("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertTrue(is_dialogue_available)
        
        # 3. ??撠店
        success = self.dialogue_system.start_dialogue("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertTrue(success)
        
        # 4. ?脣??舐?賊?
        available_options = self.dialogue_system.get_available_dialogue_options("ChiangKaiShek_FirstMeeting", self.test_character)
        self.assertGreater(len(available_options), 0)
        
        # 5. ?豢??賊?
        success = self.dialogue_system.select_dialogue_option(0)
        self.assertTrue(success)
        
        # 6. 瑼Ｘ蝯?
        outcomes = self.dialogue_system.calculate_dialogue_outcomes("ChiangKaiShek_FirstMeeting", 0)
        self.assertGreater(len(outcomes), 0)
        
        # 7. ?蝯?
        self.dialogue_system.apply_dialogue_outcomes(outcomes)
        
        # 8. 瑼Ｘ??霈?
        new_relationship = self.historical_characters.get_character_relationship("ChiangKaiShek")
        self.assertGreater(new_relationship, 50.0)  # ?府????
        
        # 9. 瑼Ｘ甇瑕閮?
        history = self.dialogue_system.get_dialogue_history()
        self.assertIn("ChiangKaiShek_FirstMeeting", history)
        
        print("??摰撠店瘚?皜祈岫??")
    
    def test_historical_accuracy(self):
        """皜祈岫甇瑕皞Ⅱ??""
        self.historical_characters.initialize()
        
        # 瑼Ｘ????喟?甇瑕靽⊥
        chiang = self.historical_characters.get_character("ChiangKaiShek")
        self.assertEqual(chiang["birth_date"], "1887-10-31")
        self.assertEqual(chiang["death_date"], "1975-04-05")
        self.assertEqual(chiang["faction"], EHistoricalFaction.Nationalist)
        self.assertEqual(chiang["role"], ECharacterRole.Leader)
        
        # 瑼Ｘ瘥黎?梁?甇瑕靽⊥
        mao = self.historical_characters.get_character("MaoZedong")
        self.assertEqual(mao["birth_date"], "1893-12-26")
        self.assertEqual(mao["death_date"], "1976-09-09")
        self.assertEqual(mao["faction"], EHistoricalFaction.Communist)
        self.assertEqual(mao["role"], ECharacterRole.Leader)
        
        print("??甇瑕皞Ⅱ?扳葫閰阡?")
    
    def test_dialogue_types(self):
        """皜祈岫撠店憿?"""
        self.dialogue_system.initialize()
        
        # 瑼Ｘ銝?憿???閰?        all_dialogues = self.dialogue_system.get_all_dialogues()
        dialogue_types = set()
        
        for dialogue in all_dialogues:
            dialogue_types.add(dialogue["type"])
        
        # ?府?憭車撠店憿?
        self.assertIn(EDialogueType.HistoricalEvent, dialogue_types)
        self.assertIn(EDialogueType.MilitaryBriefing, dialogue_types)
        self.assertIn(EDialogueType.DiplomaticTalk, dialogue_types)
        
        print("??撠店憿?皜祈岫??")
    
    def test_attitude_system(self):
        """皜祈岫?漲蝟餌絞"""
        self.historical_characters.initialize()
        
        # 皜祈岫銝??漲????        attitudes = [
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
        
        print("???漲蝟餌絞皜祈岫??")


class MockDialogueSystem:
    """璅⊥撠店蝟餌絞"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_dialogues = []
        self.character_dialogue_map = {}
        self.dialogue_map = {}
        self.current_dialogue = {}
        self.dialogue_history = []
        self._initialize_mock_dialogues()
    
    def initialize(self):
        """????閰梁頂蝯?""
        self.is_initialized = True
        self._build_dialogue_maps()
    
    def _initialize_mock_dialogues(self):
        """???芋?砍?閰?""
        dialogues = [
            {
                "id": "ChiangKaiShek_FirstMeeting",
                "speaker_id": "ChiangKaiShek",
                "text": "撟渲?鈭綽???雿除摰??∴??喳??舫????∠?蝎曇??,
                "type": EDialogueType.HistoricalEvent,
                "portrait": "/Game/Portraits/ChiangKaiShek.ChiangKaiShek",
                "options": [
                    {
                        "text": "憿?振蝯曹?憟??嚗?,
                        "attitude": EDialogueAttitude.Respectful,
                        "next_dialogue": "ChiangKaiShek_LoyalResponse",
                        "outcomes": [
                            {
                                "type": EDialogueOutcome.RelationshipChange,
                                "target": "ChiangKaiShek",
                                "value": 20.0,
                                "description": "?脣?????喃縑隞?
                            }
                        ]
                    },
                    {
                        "text": "??閬銝銝??霅啜?,
                        "attitude": EDialogueAttitude.Neutral,
                        "next_dialogue": "ChiangKaiShek_NeutralResponse",
                        "outcomes": [
                            {
                                "type": EDialogueOutcome.RelationshipChange,
                                "target": "ChiangKaiShek",
                                "value": 5.0,
                                "description": "靽?銝剔???"
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
                "text": "??嚗???敹憭拐?嚗?敹??血之?整?,
                "type": EDialogueType.HistoricalEvent,
                "portrait": "/Game/Portraits/MaoZedong.MaoZedong",
                "options": [
                    {
                        "text": "?????仿?賭?璆哨?",
                        "attitude": EDialogueAttitude.Friendly,
                        "next_dialogue": "MaoZedong_RevolutionaryResponse",
                        "outcomes": [
                            {
                                "type": EDialogueOutcome.QuestTrigger,
                                "target": "RevolutionaryQuest",
                                "value": 1.0,
                                "description": "閫貊?拙隞餃?蝺?
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
        """瑽遣撠店??"""
        for dialogue in self.all_dialogues:
            self.dialogue_map[dialogue["id"]] = dialogue
            
            if dialogue["speaker_id"] not in self.character_dialogue_map:
                self.character_dialogue_map[dialogue["speaker_id"]] = []
            self.character_dialogue_map[dialogue["speaker_id"]].append(dialogue["id"])
    
    def start_dialogue(self, dialogue_id, player_character):
        """??撠店"""
        if dialogue_id in self.dialogue_map:
            self.current_dialogue = self.dialogue_map[dialogue_id].copy()
            self.add_to_history(dialogue_id)
            return True
        return False
    
    def select_dialogue_option(self, option_index):
        """?豢?撠店?賊?"""
        if self.current_dialogue and option_index < len(self.current_dialogue["options"]):
            selected_option = self.current_dialogue["options"][option_index]
            
            # ?蝯?
            if "outcomes" in selected_option:
                self.apply_dialogue_outcomes(selected_option["outcomes"])
            
            # 瑼Ｘ?臬??銝??閰?            if "next_dialogue" in selected_option and selected_option["next_dialogue"]:
                return self.start_dialogue(selected_option["next_dialogue"], {})
            else:
                # 撠店蝯?
                self.current_dialogue = {}
                return True
        return False
    
    def is_dialogue_available(self, dialogue_id, player_character):
        """瑼Ｘ撠店?臬?舐"""
        return dialogue_id in self.dialogue_map
    
    def is_dialogue_option_available(self, dialogue_id, option_index, player_character):
        """瑼Ｘ撠店?賊??臬?舐"""
        if dialogue_id in self.dialogue_map:
            dialogue = self.dialogue_map[dialogue_id]
            return option_index < len(dialogue["options"])
        return False
    
    def get_available_dialogue_options(self, dialogue_id, player_character):
        """?脣??舐撠店?賊?"""
        if dialogue_id in self.dialogue_map:
            dialogue = self.dialogue_map[dialogue_id]
            return dialogue["options"].copy()
        return []
    
    def calculate_dialogue_outcomes(self, dialogue_id, option_index):
        """閮?撠店蝯?"""
        outcomes = []
        
        if dialogue_id in self.dialogue_map:
            dialogue = self.dialogue_map[dialogue_id]
            
            # 瘛餃??芸?蝯?
            if "auto_outcomes" in dialogue:
                outcomes.extend(dialogue["auto_outcomes"])
            
            # 瘛餃??賊?蝯?
            if option_index < len(dialogue["options"]):
                option = dialogue["options"][option_index]
                if "outcomes" in option:
                    outcomes.extend(option["outcomes"])
        
        return outcomes
    
    def apply_dialogue_outcomes(self, outcomes):
        """?撠店蝯?"""
        # 蝪∪?撖衣嚗閮?蝯?
        for outcome in outcomes:
            print(f"?蝯?: {outcome['type']} - {outcome['description']}")
    
    def get_dialogue_history(self):
        """?脣?撠店甇瑕"""
        return self.dialogue_history.copy()
    
    def add_to_history(self, dialogue_id):
        """瘛餃??唳風??""
        if dialogue_id not in self.dialogue_history:
            self.dialogue_history.append(dialogue_id)
    
    def clear_dialogue_history(self):
        """皜征撠店甇瑕"""
        self.dialogue_history.clear()
    
    def get_all_dialogues(self):
        """?脣????閰?""
        return self.all_dialogues.copy()
    
    def get_character_dialogues(self, character_id):
        """?脣?閫撠店"""
        if character_id in self.character_dialogue_map:
            dialogue_ids = self.character_dialogue_map[character_id]
            return [self.dialogue_map[did] for did in dialogue_ids if did in self.dialogue_map]
        return []
    
    def get_important_dialogues(self):
        """?脣???撠店"""
        return [dialogue for dialogue in self.all_dialogues if dialogue.get("is_important", False)]


class MockHistoricalCharacters:
    """璅⊥甇瑕鈭箇蝟餌絞"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_characters = []
        self.character_map = {}
        self.character_relationships = {}
        self.character_availability = {}
        self._initialize_mock_characters()
    
    def initialize(self):
        """???風?脖犖?拍頂蝯?""
        self.is_initialized = True
        self._build_character_maps()
    
    def _initialize_mock_characters(self):
        """???芋?祆風?脖犖??""
        characters = [
            {
                "id": "ChiangKaiShek",
                "name": "?????,
                "courtesy_name": "隞",
                "birth_date": "1887-10-31",
                "death_date": "1975-04-05",
                "role": ECharacterRole.Leader,
                "faction": EHistoricalFaction.Nationalist,
                "title": "???踹?銝餃葉",
                "description": "銝剛瘞???暺函蜇鋆?,
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
                "name": "瘥黎??,
                "courtesy_name": "瞏支?",
                "birth_date": "1893-12-26",
                "death_date": "1976-09-09",
                "role": ECharacterRole.Leader,
                "faction": EHistoricalFaction.Communist,
                "title": "銝剖??梁暺其蜓撣?,
                "description": "銝剖??梁暺典憪犖銋?",
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
                "name": "?冽靘?,
                "courtesy_name": "蝧?",
                "birth_date": "1898-03-05",
                "death_date": "1976-01-08",
                "role": ECharacterRole.Diplomatic,
                "faction": EHistoricalFaction.Communist,
                "title": "???Ｙ蜇??,
                "description": "銝剖??梁暺券?閬?撠犖",
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
        """瑽遣閫??"""
        for character in self.all_characters:
            self.character_map[character["id"]] = character.copy()
            self.character_relationships[character["id"]] = 50.0  # 暺?銝剔?
            self.character_availability[character["id"]] = character["is_available"]
    
    def get_character(self, character_id):
        """?脣?閫"""
        if character_id in self.character_map:
            return self.character_map[character_id].copy()
        return {"name": "", "id": ""}
    
    def get_character_relationship(self, character_id):
        """?脣?閫??"""
        return self.character_relationships.get(character_id, 50.0)
    
    def update_character_relationship(self, character_id, relationship_change):
        """?湔閫??"""
        current = self.character_relationships.get(character_id, 50.0)
        new_relationship = max(0.0, min(100.0, current + relationship_change))
        self.character_relationships[character_id] = new_relationship
    
    def is_character_available(self, character_id):
        """瑼Ｘ閫?臬?舐"""
        return self.character_availability.get(character_id, False)
    
    def set_character_availability(self, character_id, is_available):
        """閮剔蔭閫?舐??""
        self.character_availability[character_id] = is_available
    
    def get_characters_by_faction(self, faction):
        """?脣??孵?瘣曄頂閫"""
        return [character for character in self.all_characters if character["faction"] == faction and character["is_available"]]
    
    def get_characters_by_role(self, role):
        """?脣??孵?閫憿?"""
        return [character for character in self.all_characters if character["role"] == role and character["is_available"]]
    
    def generate_character_response(self, character_id, attitude, context):
        """??閫??"""
        if character_id not in self.character_map:
            return "?瘜?閫??????
        
        character = self.character_map[character_id]
        
        # ?寞?閫??摨衣?????        if attitude == EDialogueAttitude.Respectful:
            if character["role"] == ECharacterRole.Leader:
                return "雿??嚗?隤雿??祆????鈭?
            else:
                return "雓?雿?撠???
        elif attitude == EDialogueAttitude.Friendly:
            return "??擃??質?雿漱瘚?
        elif attitude == EDialogueAttitude.Hostile:
            return "雿??漲霈??銝???
        else:
            return "???賭?????
    
    def calculate_character_influence(self, character_id):
        """閮?閫敶梢??""
        if character_id not in self.character_map:
            return 0.0
        
        character = self.character_map[character_id]
        base_influence = character["influence"]
        relationship_bonus = (self.get_character_relationship(character_id) - 50.0) * 0.2
        
        return max(0.0, min(100.0, base_influence + relationship_bonus))
    
    def get_recommended_dialogues(self, character_id, player_character):
        """?脣??刻撠店"""
        # 蝪∪?撖衣
        relationship = self.get_character_relationship(character_id)
        
        if relationship > 70.0:
            return [f"{character_id}_FriendlyDialogue"]
        elif relationship < 30.0:
            return [f"{character_id}_FormalDialogue"]
        else:
            return [f"{character_id}_NeutralDialogue"]


def run_dialogue_system_tests():
    """??撠店蝟餌絞皜祈岫"""
    print("???? MingGoRTS 撠店蝟餌絞皜祈岫...")
    print("=" * 60)
    
    # ?萄遣皜祈岫憟辣
    suite = unittest.TestLoader().loadTestsFromTestCase(TestDialogueSystem)
    
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
    success = run_dialogue_system_tests()
    sys.exit(0 if success else 1)

