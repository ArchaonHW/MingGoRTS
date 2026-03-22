#!/usr/bin/env python3
"""
MingGoRTS 甇瑕閫?格?皜祈岫?單
皜祈岫 Story 3-4: 甇瑕閫?格?
"""

import unittest
import json
import sys
import os
from datetime import datetime
from enum import Enum

# 瘛餃???寧? Python 頝臬?
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class ERoleplayType(Enum):
    """閫?格?憿???"""
    PoliticalDecision = "PoliticalDecision"
    MilitaryStrategy = "MilitaryStrategy"
    DiplomaticNegotiation = "DiplomaticNegotiation"
    EconomicPolicy = "EconomicPolicy"
    SocialReform = "SocialReform"
    PersonalChoice = "PersonalChoice"

class EDecisionAlignment(Enum):
    """瘙箇?撠???"""
    Historical = "Historical"
    Alternative = "Alternative"
    CounterFactual = "CounterFactual"
    PlayerChoice = "PlayerChoice"

class EDecisionImpact(Enum):
    """瘙箇?敶梢??"""
    Minimal = "Minimal"
    Local = "Local"
    Regional = "Regional"
    National = "National"
    International = "International"
    Historical = "Historical"

class ESimulationType(Enum):
    """璅⊥憿???"""
    Political = "Political"
    Military = "Military"
    Economic = "Economic"
    Social = "Social"
    Cultural = "Cultural"
    International = "International"

class ESimulationPhase(Enum):
    """璅⊥?挾??"""
    Setup = "Setup"
    Execution = "Execution"
    Analysis = "Analysis"
    Results = "Results"

class TestHistoricalRoleplay(unittest.TestCase):
    """甇瑕閫?格?皜祈岫憿?""
    
    def setUp(self):
        """皜祈岫??閮剔蔭"""
        self.roleplay_system = MockHistoricalRoleplay()
        self.simulation_system = MockHistoricalSimulation()
        
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
        
        # 皜祈岫???豢?
        self.test_situation = {
            "id": "POL_NorthernExpedition",
            "title": "??蝯曹?瘙箇?",
            "description": "1926撟湛????拙頠???隡?雿????喉?雿?閬捱摰?隡?蝑?楝蝺?,
            "historical_context": "???拙頠?隡蝯曹?銝剖???閬?鈭???,
            "time_period": "1926-1928",
            "location": "撱??/?漪",
            "roleplay_type": ERoleplayType.PoliticalDecision,
            "character_role": "???拙頠蜇?訾誘",
            "key_figures": ["ChiangKaiShek", "ZhouEnlai", "ZhangZuolin"],
            "available_resources": ["???拙頠?, "??游", "瘞?舀?"],
            "constraints": ["頠?脫?", "鞈???", "?踵祥?郁"],
            "decision_options": [
                {
                    "text": "?∪?瞈?脩??伐?敹恍脫",
                    "description": "?葉?萄?敹恍脫嚗??剜??抒絞銝銝剖?",
                    "alignment": EDecisionAlignment.Historical,
                    "impact": EDecisionImpact.National,
                    "historical_accuracy": 0.9,
                    "difficulty_rating": 0.7,
                    "required_skills": ["Leadership", "TacticalCommand"],
                    "consequences": ["敹恍絞銝", "擃鈭?],
                    "risk_factors": ["?漲?游撐"],
                    "is_available": True
                },
                {
                    "text": "?∪?蝛拙戎蝑嚗郊?券?,
                    "description": "??畾菜?莎??帘?箏??孵????脫",
                    "alignment": EDecisionAlignment.Alternative,
                    "impact": EDecisionImpact.National,
                    "historical_accuracy": 0.6,
                    "difficulty_rating": 0.5,
                    "required_skills": ["Leadership", "StrategicPlanning"],
                    "consequences": ["蝛拙??澆?", "??撱園"],
                    "risk_factors": ["璈??臬仃"],
                    "is_available": True
                }
            ],
            "is_critical_moment": True,
            "can_repeat": False
        }
        
        # 皜祈岫璅⊥?豢?
        self.test_simulation = {
            "id": "POL_NorthernExpedition",
            "title": "??蝯曹?璅⊥",
            "description": "璅⊥1926-1928撟游?隡?剔??踵祥瘙箇??絞銝?脩?",
            "simulation_type": ESimulationType.Political,
            "historical_period": "1926-1928",
            "geographic_scope": "銝剖??典?",
            "initial_parameters": [
                {
                    "name": "MilitaryStrength",
                    "value": 60.0,
                    "min_value": 0.0,
                    "max_value": 100.0,
                    "unit": "%",
                    "description": "頠?撖血?"
                },
                {
                    "name": "PoliticalSupport",
                    "value": 70.0,
                    "min_value": 0.0,
                    "max_value": 100.0,
                    "unit": "%",
                    "description": "?踵祥?舀?摨?
                },
                {
                    "name": "EconomicResources",
                    "value": 50.0,
                    "min_value": 0.0,
                    "max_value": 100.0,
                    "unit": "%",
                    "description": "蝬?鞈?"
                }
            ],
            "success_conditions": ["MilitaryStrength > 80", "PoliticalSupport > 75"],
            "failure_conditions": ["MilitaryStrength < 30", "PoliticalSupport < 40"],
            "simulation_duration": 24.0,
            "max_iterations": 100,
            "is_real_time": False,
            "can_intervene": True
        }
    
    def test_roleplay_system_initialization(self):
        """皜祈岫閫?格?蝟餌絞????""
        self.roleplay_system.initialize()
        
        self.assertTrue(self.roleplay_system.is_initialized)
        self.assertGreater(len(self.roleplay_system.all_situations), 0)
        self.assertGreater(len(self.roleplay_system.situation_map), 0)
        
        print("??閫?格?蝟餌絞???葫閰阡?")
    
    def test_roleplay_session_start(self):
        """皜祈岫??閫?格??店"""
        self.roleplay_system.initialize()
        
        # ???店
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        self.assertIsInstance(session_id, str)
        self.assertGreater(len(session_id), 0)
        self.assertIn("ChiangKaiShek", session_id)
        
        # 瑼Ｘ?店???        current_session = self.roleplay_system.get_current_session()
        self.assertEqual(current_session["character_id"], "ChiangKaiShek")
        self.assertTrue(current_session["is_active"])
        
        print("????閫?格??店皜祈岫??")
    
    def test_situation_availability(self):
        """皜祈岫???舐??""
        self.roleplay_system.initialize()
        
        # ?脣??舐??
        available_situations = self.roleplay_system.get_available_situations("ChiangKaiShek")
        self.assertGreater(len(available_situations), 0)
        
        # 瑼Ｘ??撅祆?        for situation in available_situations:
            self.assertIn("ChiangKaiShek", situation["key_figures"])
            self.assertIsInstance(situation["roleplay_type"], ERoleplayType)
        
        print("?????舐?扳葫閰阡?")
    
    def test_situation_start(self):
        """皜祈岫????"""
        self.roleplay_system.initialize()
        
        # ???店
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        # ????
        success = self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        self.assertTrue(success)
        
        # 瑼Ｘ?嗅???
        current_situation = self.roleplay_system.get_current_situation(session_id)
        self.assertEqual(current_situation["id"], "POL_NorthernExpedition")
        self.assertEqual(current_situation["title"], "??蝯曹?瘙箇?")
        
        print("??????皜祈岫??")
    
    def test_decision_making(self):
        """皜祈岫瘙箇??嗅?"""
        self.roleplay_system.initialize()
        
        # ???店??憓?        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        
        # ?脣??舐瘙箇??賊?
        available_options = self.roleplay_system.get_available_decision_options(session_id)
        self.assertGreater(len(available_options), 0)
        
        # ?瘙箇?
        success = self.roleplay_system.make_decision(session_id, 0)
        self.assertTrue(success)
        
        # 瑼Ｘ瘙箇?甇瑕
        decision_history = self.roleplay_system.get_session_history(session_id)
        self.assertGreater(len(decision_history), 0)
        
        print("??瘙箇??嗅?皜祈岫??")
    
    def test_decision_evaluation(self):
        """皜祈岫瘙箇?閰摯"""
        self.roleplay_system.initialize()
        
        # ???店??憓?        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        
        # 閰摯瘙箇?
        evaluation_score = self.roleplay_system.evaluate_decision(session_id, 0)
        self.assertGreaterEqual(evaluation_score, 0.0)
        self.assertLessEqual(evaluation_score, 100.0)
        
        print("??瘙箇?閰摯皜祈岫??")
    
    def test_session_scoring(self):
        """皜祈岫?店閰?"""
        self.roleplay_system.initialize()
        
        # ???店
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        # 瑼Ｘ??閰?
        historical_accuracy = self.roleplay_system.get_historical_accuracy_score(session_id)
        decision_quality = self.roleplay_system.get_decision_quality_score(session_id)
        immersion = self.roleplay_system.get_immersion_score(session_id)
        
        self.assertEqual(historical_accuracy, 0.0)
        self.assertEqual(decision_quality, 0.0)
        self.assertEqual(immersion, 0.0)
        
        # ?瘙箇?敺炎?亥???        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        self.roleplay_system.make_decision(session_id, 0)
        
        new_historical_accuracy = self.roleplay_system.get_historical_accuracy_score(session_id)
        new_decision_quality = self.roleplay_system.get_decision_quality_score(session_id)
        
        self.assertGreater(new_historical_accuracy, 0.0)
        self.assertGreater(new_decision_quality, 0.0)
        
        print("???店閰?皜祈岫??")
    
    def test_session_statistics(self):
        """皜祈岫?店蝯梯?"""
        self.roleplay_system.initialize()
        
        # ???店
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        # ?脣?蝯梯??豢?
        statistics = self.roleplay_system.get_roleplay_statistics(session_id)
        
        self.assertIsInstance(statistics, dict)
        self.assertIn("HistoricalAccuracy", statistics)
        self.assertIn("DecisionQuality", statistics)
        self.assertIn("Immersion", statistics)
        self.assertIn("SituationsCompleted", statistics)
        self.assertIn("DecisionsMade", statistics)
        
        print("???店蝯梯?皜祈岫??")
    
    def test_session_end(self):
        """皜祈岫蝯??店"""
        self.roleplay_system.initialize()
        
        # ???店
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        # 蝯??店
        success = self.roleplay_system.end_roleplay_session(session_id)
        self.assertTrue(success)
        
        # 瑼Ｘ?店???        current_session = self.roleplay_system.get_current_session()
        self.assertFalse(current_session["is_active"])
        
        print("??蝯??店皜祈岫??")
    
    def test_simulation_system_initialization(self):
        """皜祈岫璅⊥蝟餌絞????""
        self.simulation_system.initialize()
        
        self.assertTrue(self.simulation_system.is_initialized)
        self.assertGreater(len(self.simulation_system.all_simulations), 0)
        self.assertGreater(len(self.simulation_system.simulation_map), 0)
        
        print("??璅⊥蝟餌絞???葫閰阡?")
    
    def test_simulation_start(self):
        """皜祈岫??璅⊥"""
        self.simulation_system.initialize()
        
        # 閮剔蔭?拙振瘙箇?
        player_decisions = {
            "MilitaryStrength": 75.0,
            "PoliticalSupport": 80.0,
            "EconomicResources": 60.0
        }
        
        # ??璅⊥
        success = self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        self.assertTrue(success)
        
        # 瑼Ｘ璅⊥???        phase = self.simulation_system.get_simulation_phase("POL_NorthernExpedition")
        self.assertEqual(phase, ESimulationPhase.Execution)
        
        # 瑼Ｘ?嗅??
        current_parameters = self.simulation_system.get_current_parameters("POL_NorthernExpedition")
        self.assertGreater(len(current_parameters), 0)
        
        print("????璅⊥皜祈岫??")
    
    def test_simulation_progress(self):
        """皜祈岫璅⊥?脣漲"""
        self.simulation_system.initialize()
        
        # ??璅⊥
        player_decisions = {"MilitaryStrength": 75.0, "PoliticalSupport": 80.0, "EconomicResources": 60.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        
        # ?脣??脣漲
        progress = self.simulation_system.get_simulation_progress("POL_NorthernExpedition")
        self.assertGreaterEqual(progress, 0.0)
        self.assertLessEqual(progress, 1.0)
        
        print("??璅⊥?脣漲皜祈岫??")
    
    def test_simulation_intervention(self):
        """皜祈岫璅⊥撟脤?"""
        self.simulation_system.initialize()
        
        # ??璅⊥
        player_decisions = {"MilitaryStrength": 75.0, "PoliticalSupport": 80.0, "EconomicResources": 60.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        
        # 撟脤?璅⊥
        success = self.simulation_system.intervene_in_simulation("POL_NorthernExpedition", "MilitaryStrength", 85.0)
        self.assertTrue(success)
        
        # 瑼Ｘ?霈?
        current_parameters = self.simulation_system.get_current_parameters("POL_NorthernExpedition")
        for param in current_parameters:
            if param["name"] == "MilitaryStrength":
                self.assertEqual(param["value"], 85.0)
                break
        
        print("??璅⊥撟脤?皜祈岫??")
    
    def test_simulation_prediction(self):
        """皜祈岫璅⊥?葫"""
        self.simulation_system.initialize()
        
        # 閮剔蔭?霈?
        parameter_changes = {
            "MilitaryStrength": 10.0,
            "PoliticalSupport": 5.0,
            "EconomicResources": -5.0
        }
        
        # ?葫蝯?
        predictions = self.simulation_system.predict_simulation_outcome("POL_NorthernExpedition", parameter_changes)
        
        self.assertIsInstance(predictions, list)
        self.assertEqual(len(predictions), 3)  # ??????仃??
        
        # 瑼Ｘ?葫?潛???        for prediction in predictions:
            self.assertGreaterEqual(prediction, 0.0)
            self.assertLessEqual(prediction, 100.0)
        
        print("??璅⊥?葫皜祈岫??")
    
    def test_simulation_result(self):
        """皜祈岫璅⊥蝯?"""
        self.simulation_system.initialize()
        
        # ??璅⊥
        player_decisions = {"MilitaryStrength": 90.0, "PoliticalSupport": 85.0, "EconomicResources": 70.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        
        # ?迫璅⊥隞亦?????        self.simulation_system.stop_simulation("POL_NorthernExpedition")
        
        # ?脣?蝯?
        result = self.simulation_system.get_simulation_result("POL_NorthernExpedition")
        
        self.assertIsInstance(result, dict)
        self.assertIn("success", result)
        self.assertIn("final_score", result)
        self.assertIn("historical_accuracy", result)
        
        print("??璅⊥蝯?皜祈岫??")
    
    def test_historical_accuracy_comparison(self):
        """皜祈岫甇瑕皞Ⅱ?扳?頛?""
        self.simulation_system.initialize()
        
        # ??璅⊥
        player_decisions = {"MilitaryStrength": 85.0, "PoliticalSupport": 80.0, "EconomicResources": 65.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        self.simulation_system.stop_simulation("POL_NorthernExpedition")
        
        # 瘥?甇瑕皞Ⅱ??        accuracy = self.simulation_system.compare_with_historical_baseline("POL_NorthernExpedition")
        self.assertGreaterEqual(accuracy, 0.0)
        self.assertLessEqual(accuracy, 1.0)
        
        print("??甇瑕皞Ⅱ?扳?頛葫閰阡?")
    
    def test_simulation_statistics(self):
        """皜祈岫璅⊥蝯梯?"""
        self.simulation_system.initialize()
        
        # ??璅⊥
        player_decisions = {"MilitaryStrength": 80.0, "PoliticalSupport": 75.0, "EconomicResources": 60.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        
        # ?脣?蝯梯??豢?
        statistics = self.simulation_system.get_simulation_statistics("POL_NorthernExpedition")
        
        self.assertIsInstance(statistics, dict)
        self.assertIn("Progress", statistics)
        self.assertIn("HistoricalAccuracy", statistics)
        
        print("??璅⊥蝯梯?皜祈岫??")
    
    def test_complete_roleplay_flow(self):
        """皜祈岫摰???脫瞍?蝔?""
        self.roleplay_system.initialize()
        self.simulation_system.initialize()
        
        # 1. ??閫?格??店
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        self.assertIsInstance(session_id, str)
        
        # 2. ?脣??舐??
        available_situations = self.roleplay_system.get_available_situations("ChiangKaiShek")
        self.assertGreater(len(available_situations), 0)
        
        # 3. ????
        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        
        # 4. ?脣?瘙箇??賊?
        decision_options = self.roleplay_system.get_available_decision_options(session_id)
        self.assertGreater(len(decision_options), 0)
        
        # 5. ?瘙箇?
        self.roleplay_system.make_decision(session_id, 0)
        
        # 6. 閰摯瘙箇?
        evaluation = self.roleplay_system.evaluate_decision(session_id, 0)
        self.assertGreater(evaluation, 0.0)
        
        # 7. 瑼Ｘ閰?
        historical_accuracy = self.roleplay_system.get_historical_accuracy_score(session_id)
        decision_quality = self.roleplay_system.get_decision_quality_score(session_id)
        self.assertGreater(historical_accuracy, 0.0)
        self.assertGreater(decision_quality, 0.0)
        
        # 8. ?脣?蝯梯?
        statistics = self.roleplay_system.get_roleplay_statistics(session_id)
        self.assertIsInstance(statistics, dict)
        
        # 9. 蝯??店
        self.roleplay_system.end_roleplay_session(session_id)
        
        # 10. 瑼Ｘ?蝯???        current_session = self.roleplay_system.get_current_session()
        self.assertFalse(current_session["is_active"])
        
        print("??摰閫?格?瘚?皜祈岫??")
    
    def test_historical_authenticity(self):
        """皜祈岫甇瑕?祕??""
        self.roleplay_system.initialize()
        
        # 瑼Ｘ?????風?脫?蝣箸?        northern_expedition = self.roleplay_system.get_situation("POL_NorthernExpedition")
        self.assertEqual(northern_expedition["time_period"], "1926-1928")
        self.assertEqual(northern_expedition["location"], "撱??/?漪")
        self.assertIn("ChiangKaiShek", northern_expedition["key_figures"])
        self.assertTrue(northern_expedition["is_critical_moment"])
        
        # 瑼Ｘ镼踹?鈭???
        xian_incident = self.roleplay_system.get_situation("MIL_XianIncident")
        self.assertEqual(xian_incident["time_period"], "1936-12")
        self.assertEqual(xian_incident["location"], "镼踹?")
        self.assertIn("ChiangKaiShek", xian_incident["key_figures"])
        self.assertIn("ZhangXueliang", xian_incident["key_figures"])
        
        print("??甇瑕?祕?扳葫閰阡?")
    
    def test_decision_alignment_system(self):
        """皜祈岫瘙箇?撠?蝟餌絞"""
        self.roleplay_system.initialize()
        
        # ???店??憓?        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        
        # ?脣?瘙箇??賊?
        decision_options = self.roleplay_system.get_available_decision_options(session_id)
        
        # 瑼Ｘ瘙箇?撠?憿?
        alignments = set()
        for option in decision_options:
            alignments.add(option["alignment"])
        
        self.assertIn(EDecisionAlignment.Historical, alignments)
        
        # 瑼Ｘ甇瑕皞Ⅱ??        for option in decision_options:
            self.assertGreaterEqual(option["historical_accuracy"], 0.0)
            self.assertLessEqual(option["historical_accuracy"], 1.0)
        
        print("??瘙箇?撠?蝟餌絞皜祈岫??")
    
    def test_simulation_types(self):
        """皜祈岫璅⊥憿?"""
        self.simulation_system.initialize()
        
        # 瑼Ｘ??芋?祇???        all_simulations = self.simulation_system.get_all_simulations()
        simulation_types = set()
        
        for simulation in all_simulations:
            simulation_types.add(simulation["simulation_type"])
        
        # ?府?憭車璅⊥憿?
        self.assertIn(ESimulationType.Political, simulation_types)
        self.assertIn(ESimulationType.Military, simulation_types)
        self.assertIn(ESimulationType.Economic, simulation_types)
        
        print("??璅⊥憿?皜祈岫??")


class MockHistoricalRoleplay:
    """璅⊥甇瑕閫?格?蝟餌絞"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_situations = []
        self.situation_map = {}
        self.character_situation_map = {}
        self.active_sessions = {}
        self.current_player_character = {}
        self._initialize_mock_situations()
    
    def initialize(self):
        """?????脫瞍頂蝯?""
        self.is_initialized = True
        self._build_situation_maps()
    
    def _initialize_mock_situations(self):
        """???芋?祆?憓?""
        situations = [
            {
                "id": "POL_NorthernExpedition",
                "title": "??蝯曹?瘙箇?",
                "description": "1926撟湛????拙頠???隡?雿????喉?雿?閬捱摰?隡?蝑?楝蝺?,
                "historical_context": "???拙頠?隡蝯曹?銝剖???閬?鈭???,
                "time_period": "1926-1928",
                "location": "撱??/?漪",
                "roleplay_type": ERoleplayType.PoliticalDecision,
                "character_role": "???拙頠蜇?訾誘",
                "key_figures": ["ChiangKaiShek", "ZhouEnlai", "ZhangZuolin"],
                "available_resources": ["???拙頠?, "??游", "瘞?舀?"],
                "constraints": ["頠?脫?", "鞈???", "?踵祥?郁"],
                "decision_options": [
                    {
                        "text": "?∪?瞈?脩??伐?敹恍脫",
                        "description": "?葉?萄?敹恍脫嚗??剜??抒絞銝銝剖?",
                        "alignment": EDecisionAlignment.Historical,
                        "impact": EDecisionImpact.National,
                        "historical_accuracy": 0.9,
                        "difficulty_rating": 0.7,
                        "required_skills": ["Leadership", "TacticalCommand"],
                        "consequences": ["敹恍絞銝", "擃鈭?],
                        "risk_factors": ["?漲?游撐"],
                        "is_available": True
                    }
                ],
                "is_critical_moment": True,
                "can_repeat": False
            },
            {
                "id": "MIL_XianIncident",
                "title": "镼踹?鈭?瘙箇?",
                "description": "1936撟?2??撘萄飛?臬?璆??镼踹????????喉?閬??迫?扳銝?湔??乓?,
                "historical_context": "镼踹?鈭??臭葉?隞???閬???",
                "time_period": "1936-12",
                "location": "镼踹?",
                "roleplay_type": ERoleplayType.MilitaryStrategy,
                "character_role": "???踹?銝餃葉",
                "key_figures": ["ChiangKaiShek", "ZhangXueliang", "YangHucheng", "ZhouEnlai"],
                "decision_options": [
                    {
                        "text": "?亙??迫?扳嚗??湔???,
                        "description": "??撘萄飛?舐?閬?嚗遣蝡??交??絞銝?啁?",
                        "alignment": EDecisionAlignment.Historical,
                        "impact": EDecisionImpact.Historical,
                        "historical_accuracy": 0.95,
                        "difficulty_rating": 0.8,
                        "required_skills": ["Diplomacy", "Leadership"],
                        "consequences": ["?蝯曹?", "???"],
                        "risk_factors": ["甈??摹"],
                        "is_available": True
                    }
                ],
                "is_critical_moment": True,
                "can_repeat": False
            }
        ]
        
        self.all_situations = situations
    
    def _build_situation_maps(self):
        """瑽遣????"""
        for situation in self.all_situations:
            self.situation_map[situation["id"]] = situation
            
            for key_figure in situation["key_figures"]:
                if key_figure not in self.character_situation_map:
                    self.character_situation_map[key_figure] = []
                self.character_situation_map[key_figure].append(situation["id"])
    
    def start_roleplay_session(self, character_id, player_character):
        """??閫?格??店"""
        session_id = f"Session_{character_id}_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        new_session = {
            "session_id": session_id,
            "character_id": character_id,
            "current_situation_id": "",
            "completed_situations": [],
            "decision_history": [],
            "historical_accuracy_score": 0.0,
            "decision_quality_score": 0.0,
            "immersion_score": 0.0,
            "start_time": datetime.now(),
            "is_active": True
        }
        
        self.active_sessions[session_id] = new_session
        self.current_player_character = player_character
        
        return session_id
    
    def end_roleplay_session(self, session_id):
        """蝯?閫?格??店"""
        if session_id in self.active_sessions:
            session = self.active_sessions[session_id]
            session["is_active"] = False
            session["end_time"] = datetime.now()
            session["immersion_score"] = self._calculate_immersion_score(session_id)
            return True
        return False
    
    def get_current_session(self):
        """?脣??嗅??店"""
        for session in self.active_sessions.values():
            if session["is_active"]:
                return session
        return {"is_active": False}
    
    def get_available_situations(self, character_id):
        """?脣??舐??"""
        available_situations = []
        
        if character_id in self.character_situation_map:
            situation_ids = self.character_situation_map[character_id]
            for situation_id in situation_ids:
                if situation_id in self.situation_map:
                    available_situations.append(self.situation_map[situation_id])
        
        return available_situations
    
    def start_situation(self, session_id, situation_id):
        """????"""
        if session_id in self.active_sessions and situation_id in self.situation_map:
            self.active_sessions[session_id]["current_situation_id"] = situation_id
            return True
        return False
    
    def get_current_situation(self, session_id):
        """?脣??嗅???"""
        if session_id in self.active_sessions:
            session = self.active_sessions[session_id]
            situation_id = session["current_situation_id"]
            if situation_id in self.situation_map:
                return self.situation_map[situation_id]
        return {}
    
    def make_decision(self, session_id, decision_index):
        """?瘙箇?"""
        if session_id not in self.active_sessions:
            return False
        
        session = self.active_sessions[session_id]
        current_situation = self.get_current_situation(session_id)
        
        if decision_index < 0 or decision_index >= len(current_situation["decision_options"]):
            return False
        
        selected_option = current_situation["decision_options"][decision_index]
        
        # 閮?瘙箇?
        session["decision_history"].append(selected_option["text"])
        
        # ?湔閰?
        self._update_session_scores(session_id, decision_index)
        
        # 瘛餃??啣歇摰???
        session["completed_situations"].append(session["current_situation_id"])
        
        return True
    
    def get_available_decision_options(self, session_id):
        """?脣??舐瘙箇??賊?"""
        current_situation = self.get_current_situation(session_id)
        if current_situation:
            return current_situation["decision_options"]
        return []
    
    def evaluate_decision(self, session_id, decision_index):
        """閰摯瘙箇?"""
        current_situation = self.get_current_situation(session_id)
        if decision_index < 0 or decision_index >= len(current_situation["decision_options"]):
            return 0.0
        
        selected_option = current_situation["decision_options"][decision_index]
        
        # 閮?閰?
        historical_score = selected_option["historical_accuracy"] * 100
        quality_score = (1.0 - selected_option["difficulty_rating"]) * 100
        
        return (historical_score + quality_score) / 2.0
    
    def get_historical_accuracy_score(self, session_id):
        """?脣?甇瑕皞Ⅱ?扯???""
        if session_id in self.active_sessions:
            return self.active_sessions[session_id]["historical_accuracy_score"]
        return 0.0
    
    def get_decision_quality_score(self, session_id):
        """?脣?瘙箇?鞈芷?閰?"""
        if session_id in self.active_sessions:
            return self.active_sessions[session_id]["decision_quality_score"]
        return 0.0
    
    def get_immersion_score(self, session_id):
        """?脣?瘝絡????""
        if session_id in self.active_sessions:
            return self.active_sessions[session_id]["immersion_score"]
        return 0.0
    
    def get_session_history(self, session_id):
        """?脣??店甇瑕"""
        if session_id in self.active_sessions:
            return self.active_sessions[session_id]["decision_history"]
        return []
    
    def get_roleplay_statistics(self, session_id):
        """?脣?閫?格?蝯梯?"""
        if session_id not in self.active_sessions:
            return {}
        
        session = self.active_sessions[session_id]
        
        return {
            "HistoricalAccuracy": session["historical_accuracy_score"],
            "DecisionQuality": session["decision_quality_score"],
            "Immersion": session["immersion_score"],
            "SituationsCompleted": len(session["completed_situations"]),
            "DecisionsMade": len(session["decision_history"])
        }
    
    def get_situation(self, situation_id):
        """?脣???"""
        return self.situation_map.get(situation_id, {})
    
    def _update_session_scores(self, session_id, decision_index):
        """?湔?店閰?"""
        if session_id not in self.active_sessions:
            return
        
        session = self.active_sessions[session_id]
        current_situation = self.get_current_situation(session_id)
        selected_option = current_situation["decision_options"][decision_index]
        
        # ?湔甇瑕皞Ⅱ??        historical_score = selected_option["historical_accuracy"] * 100
        session["historical_accuracy_score"] = (session["historical_accuracy_score"] + historical_score) / 2.0
        
        # ?湔瘙箇?鞈芷?
        quality_score = (1.0 - selected_option["difficulty_rating"]) * 100
        session["decision_quality_score"] = (session["decision_quality_score"] + quality_score) / 2.0
    
    def _calculate_immersion_score(self, session_id):
        """閮?瘝絡????""
        if session_id not in self.active_sessions:
            return 0.0
        
        session = self.active_sessions[session_id]
        decision_count = len(session["decision_history"])
        average_quality = (session["historical_accuracy_score"] + session["decision_quality_score"]) / 2.0
        
        return (decision_count * 10 + average_quality) / 2.0


class MockHistoricalSimulation:
    """璅⊥甇瑕璅⊥蝟餌絞"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_simulations = []
        self.simulation_map = {}
        self.active_simulations = {}
        self.simulation_parameters = {}
        self.simulation_results = {}
        self.simulation_histories = {}
        self._initialize_mock_simulations()
    
    def initialize(self):
        """???芋?祉頂蝯?""
        self.is_initialized = True
        self._build_simulation_maps()
    
    def _initialize_mock_simulations(self):
        """???芋??""
        simulations = [
            {
                "id": "POL_NorthernExpedition",
                "title": "??蝯曹?璅⊥",
                "description": "璅⊥1926-1928撟游?隡?剔??踵祥瘙箇??絞銝?脩?",
                "simulation_type": ESimulationType.Political,
                "historical_period": "1926-1928",
                "geographic_scope": "銝剖??典?",
                "initial_parameters": [
                    {
                        "name": "MilitaryStrength",
                        "value": 60.0,
                        "min_value": 0.0,
                        "max_value": 100.0,
                        "unit": "%",
                        "description": "頠?撖血?"
                    },
                    {
                        "name": "PoliticalSupport",
                        "value": 70.0,
                        "min_value": 0.0,
                        "max_value": 100.0,
                        "unit": "%",
                        "description": "?踵祥?舀?摨?
                    },
                    {
                        "name": "EconomicResources",
                        "value": 50.0,
                        "min_value": 0.0,
                        "max_value": 100.0,
                        "unit": "%",
                        "description": "蝬?鞈?"
                    }
                ],
                "success_conditions": ["MilitaryStrength > 80", "PoliticalSupport > 75"],
                "failure_conditions": ["MilitaryStrength < 30", "PoliticalSupport < 40"],
                "simulation_duration": 24.0,
                "max_iterations": 100,
                "is_real_time": False,
                "can_intervene": True
            }
        ]
        
        self.all_simulations = simulations
    
    def _build_simulation_maps(self):
        """瑽遣璅⊥??"""
        for simulation in self.all_simulations:
            self.simulation_map[simulation["id"]] = simulation
    
    def start_simulation(self, simulation_id, player_decisions):
        """??璅⊥"""
        if simulation_id not in self.simulation_map:
            return False
        
        simulation = self.simulation_map[simulation_id]
        
        # ??????        initial_parameters = simulation["initial_parameters"].copy()
        for param in initial_parameters:
            if param["name"] in player_decisions:
                param["value"] = player_decisions[param["name"]]
        
        self.active_simulations[simulation_id] = ESimulationPhase.Execution
        self.simulation_parameters[simulation_id] = initial_parameters
        
        # ???風??        self.simulation_histories[simulation_id] = [f"璅⊥??嚗simulation_id}"]
        
        return True
    
    def pause_simulation(self, simulation_id):
        """?怠?璅⊥"""
        if simulation_id in self.active_simulations:
            if self.active_simulations[simulation_id] == ESimulationPhase.Execution:
                self.active_simulations[simulation_id] = ESimulationPhase.Analysis
                return True
        return False
    
    def resume_simulation(self, simulation_id):
        """蝜潛?璅⊥"""
        if simulation_id in self.active_simulations:
            if self.active_simulations[simulation_id] == ESimulationPhase.Analysis:
                self.active_simulations[simulation_id] = ESimulationPhase.Execution
                return True
        return False
    
    def stop_simulation(self, simulation_id):
        """?迫璅⊥"""
        if simulation_id in self.active_simulations:
            self._generate_simulation_result(simulation_id)
            self.active_simulations[simulation_id] = ESimulationPhase.Results
            self.simulation_histories[simulation_id].append("璅⊥?迫")
            return True
        return False
    
    def intervene_in_simulation(self, simulation_id, parameter_name, new_value):
        """撟脤?璅⊥"""
        if simulation_id not in self.simulation_map:
            return False
        
        simulation = self.simulation_map[simulation_id]
        if not simulation["can_intervene"]:
            return False
        
        if simulation_id in self.simulation_parameters:
            for param in self.simulation_parameters[simulation_id]:
                if param["name"] == parameter_name:
                    param["value"] = max(param["min_value"], min(param["max_value"], new_value))
                    self.simulation_histories[simulation_id].append(f"?拙振撟脤?嚗parameter_name} = {new_value}")
                    return True
        
        return False
    
    def get_simulation_phase(self, simulation_id):
        """?脣?璅⊥???""
        return self.active_simulations.get(simulation_id, ESimulationPhase.Setup)
    
    def get_current_parameters(self, simulation_id):
        """?脣??嗅??"""
        return self.simulation_parameters.get(simulation_id, [])
    
    def get_simulation_progress(self, simulation_id):
        """?脣?璅⊥?脣漲"""
        phase = self.get_simulation_phase(simulation_id)
        if phase == ESimulationPhase.Setup:
            return 0.1
        elif phase == ESimulationPhase.Execution:
            return 0.5
        elif phase == ESimulationPhase.Analysis:
            return 0.8
        elif phase == ESimulationPhase.Results:
            return 1.0
        return 0.0
    
    def get_simulation_result(self, simulation_id):
        """?脣?璅⊥蝯?"""
        return self.simulation_results.get(simulation_id, {})
    
    def predict_simulation_outcome(self, simulation_id, parameter_changes):
        """?葫璅⊥蝯?"""
        success_probability = 0.5
        
        for param_name, change in parameter_changes.items():
            if "Military" in param_name:
                success_probability += change * 0.1
            elif "Economic" in param_name:
                success_probability += change * 0.15
            elif "Political" in param_name:
                success_probability += change * 0.2
        
        success_probability = max(0.0, min(1.0, success_probability))
        
        return [
            success_probability,
            success_probability * 100,
            (1.0 - success_probability) * 100
        ]
    
    def compare_with_historical_baseline(self, simulation_id):
        """瘥?甇瑕?箸?"""
        if simulation_id in self.simulation_results:
            result = self.simulation_results[simulation_id]
            return result.get("historical_accuracy", 0.0)
        return 0.0
    
    def get_simulation_statistics(self, simulation_id):
        """?脣?璅⊥蝯梯?"""
        return {
            "Progress": self.get_simulation_progress(simulation_id),
            "HistoricalAccuracy": self.compare_with_historical_baseline(simulation_id)
        }
    
    def get_all_simulations(self):
        """?脣???芋??""
        return self.all_simulations.copy()
    
    def _generate_simulation_result(self, simulation_id):
        """??璅⊥蝯?"""
        if simulation_id not in self.simulation_parameters:
            return
        
        parameters = self.simulation_parameters[simulation_id]
        simulation = self.simulation_map[simulation_id]
        
        # 瑼Ｘ??璇辣
        success = self._check_success_conditions(simulation_id, parameters)
        
        # 閮??蝯???        total_score = sum(param["value"] for param in parameters)
        final_score = total_score / len(parameters)
        
        # 閮?甇瑕皞Ⅱ??        historical_accuracy = 0.8 if success else 0.3
        historical_accuracy += (final_score / 100.0) * 0.2
        
        result = {
            "success": success,
            "final_score": final_score,
            "historical_accuracy": historical_accuracy,
            "final_parameters": parameters,
            "outcome_description": f"璅⊥蝯?嚗'??' if success else '憭望?'}"
        }
        
        self.simulation_results[simulation_id] = result
    
    def _check_success_conditions(self, simulation_id, parameters):
        """瑼Ｘ??璇辣"""
        if simulation_id not in self.simulation_map:
            return False
        
        simulation = self.simulation_map[simulation_id]
        param_values = {param["name"]: param["value"] for param in parameters}
        
        # 瑼Ｘ??璇辣
        for condition in simulation["success_conditions"]:
            if ">" in condition:
                parts = condition.split(">")
                if len(parts) == 2:
                    param_name = parts[0].strip()
                    required_value = float(parts[1].strip())
                    if param_name in param_values:
                        if param_values[param_name] <= required_value:
                            return False
        
        # 瑼Ｘ憭望?璇辣
        for condition in simulation["failure_conditions"]:
            if "<" in condition:
                parts = condition.split("<")
                if len(parts) == 2:
                    param_name = parts[0].strip()
                    required_value = float(parts[1].strip())
                    if param_name in param_values:
                        if param_values[param_name] < required_value:
                            return False
        
        return True


def run_historical_roleplay_tests():
    """??甇瑕閫?格?皜祈岫"""
    print("???? MingGoRTS 甇瑕閫?格?皜祈岫...")
    print("=" * 60)
    
    # ?萄遣皜祈岫憟辣
    suite = unittest.TestLoader().loadTestsFromTestCase(TestHistoricalRoleplay)
    
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
    success = run_historical_roleplay_tests()
    sys.exit(0 if success else 1)

