#!/usr/bin/env python3
"""
MingGoRTS 歷史角色扮演測試腳本
測試 Story 3-4: 歷史角色扮演
"""

import unittest
import json
import sys
import os
from datetime import datetime
from enum import Enum

# 添加項目根目錄到 Python 路徑
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class ERoleplayType(Enum):
    """角色扮演類型枚舉"""
    PoliticalDecision = "PoliticalDecision"
    MilitaryStrategy = "MilitaryStrategy"
    DiplomaticNegotiation = "DiplomaticNegotiation"
    EconomicPolicy = "EconomicPolicy"
    SocialReform = "SocialReform"
    PersonalChoice = "PersonalChoice"

class EDecisionAlignment(Enum):
    """決策對齊枚舉"""
    Historical = "Historical"
    Alternative = "Alternative"
    CounterFactual = "CounterFactual"
    PlayerChoice = "PlayerChoice"

class EDecisionImpact(Enum):
    """決策影響枚舉"""
    Minimal = "Minimal"
    Local = "Local"
    Regional = "Regional"
    National = "National"
    International = "International"
    Historical = "Historical"

class ESimulationType(Enum):
    """模擬類型枚舉"""
    Political = "Political"
    Military = "Military"
    Economic = "Economic"
    Social = "Social"
    Cultural = "Cultural"
    International = "International"

class ESimulationPhase(Enum):
    """模擬階段枚舉"""
    Setup = "Setup"
    Execution = "Execution"
    Analysis = "Analysis"
    Results = "Results"

class TestHistoricalRoleplay(unittest.TestCase):
    """歷史角色扮演測試類"""
    
    def setUp(self):
        """測試前的設置"""
        self.roleplay_system = MockHistoricalRoleplay()
        self.simulation_system = MockHistoricalSimulation()
        
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
        
        # 測試情境數據
        self.test_situation = {
            "id": "POL_NorthernExpedition",
            "title": "北伐統一決策",
            "description": "1926年，國民革命軍準備北伐，作為蔣介石，你需要決定北伐的策略和路線。",
            "historical_context": "國民革命軍北伐是統一中國的重要軍事行動",
            "time_period": "1926-1928",
            "location": "廣州/南京",
            "roleplay_type": ERoleplayType.PoliticalDecision,
            "character_role": "國民革命軍總司令",
            "key_figures": ["ChiangKaiShek", "ZhouEnlai", "ZhangZuolin"],
            "available_resources": ["國民革命軍", "蘇聯援助", "民眾支持"],
            "constraints": ["軍閥割據", "資源有限", "政治分歧"],
            "decision_options": [
                {
                    "text": "採取激進策略，快速進攻",
                    "description": "集中兵力快速進攻，爭取在短期內統一中國",
                    "alignment": EDecisionAlignment.Historical,
                    "impact": EDecisionImpact.National,
                    "historical_accuracy": 0.9,
                    "difficulty_rating": 0.7,
                    "required_skills": ["Leadership", "TacticalCommand"],
                    "consequences": ["快速統一", "高傷亡"],
                    "risk_factors": ["過度擴張"],
                    "is_available": True
                },
                {
                    "text": "採取穩妥策略，逐步推進",
                    "description": "分階段推進，先穩固後方再向北進攻",
                    "alignment": EDecisionAlignment.Alternative,
                    "impact": EDecisionImpact.National,
                    "historical_accuracy": 0.6,
                    "difficulty_rating": 0.5,
                    "required_skills": ["Leadership", "StrategicPlanning"],
                    "consequences": ["穩定發展", "時間延長"],
                    "risk_factors": ["機會錯失"],
                    "is_available": True
                }
            ],
            "is_critical_moment": True,
            "can_repeat": False
        }
        
        # 測試模擬數據
        self.test_simulation = {
            "id": "POL_NorthernExpedition",
            "title": "北伐統一模擬",
            "description": "模擬1926-1928年北伐戰爭的政治決策和統一進程",
            "simulation_type": ESimulationType.Political,
            "historical_period": "1926-1928",
            "geographic_scope": "中國全境",
            "initial_parameters": [
                {
                    "name": "MilitaryStrength",
                    "value": 60.0,
                    "min_value": 0.0,
                    "max_value": 100.0,
                    "unit": "%",
                    "description": "軍事實力"
                },
                {
                    "name": "PoliticalSupport",
                    "value": 70.0,
                    "min_value": 0.0,
                    "max_value": 100.0,
                    "unit": "%",
                    "description": "政治支持度"
                },
                {
                    "name": "EconomicResources",
                    "value": 50.0,
                    "min_value": 0.0,
                    "max_value": 100.0,
                    "unit": "%",
                    "description": "經濟資源"
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
        """測試角色扮演系統初始化"""
        self.roleplay_system.initialize()
        
        self.assertTrue(self.roleplay_system.is_initialized)
        self.assertGreater(len(self.roleplay_system.all_situations), 0)
        self.assertGreater(len(self.roleplay_system.situation_map), 0)
        
        print("✅ 角色扮演系統初始化測試通過")
    
    def test_roleplay_session_start(self):
        """測試開始角色扮演會話"""
        self.roleplay_system.initialize()
        
        # 開始會話
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        self.assertIsInstance(session_id, str)
        self.assertGreater(len(session_id), 0)
        self.assertIn("ChiangKaiShek", session_id)
        
        # 檢查會話狀態
        current_session = self.roleplay_system.get_current_session()
        self.assertEqual(current_session["character_id"], "ChiangKaiShek")
        self.assertTrue(current_session["is_active"])
        
        print("✅ 開始角色扮演會話測試通過")
    
    def test_situation_availability(self):
        """測試情境可用性"""
        self.roleplay_system.initialize()
        
        # 獲取可用情境
        available_situations = self.roleplay_system.get_available_situations("ChiangKaiShek")
        self.assertGreater(len(available_situations), 0)
        
        # 檢查情境屬性
        for situation in available_situations:
            self.assertIn("ChiangKaiShek", situation["key_figures"])
            self.assertIsInstance(situation["roleplay_type"], ERoleplayType)
        
        print("✅ 情境可用性測試通過")
    
    def test_situation_start(self):
        """測試開始情境"""
        self.roleplay_system.initialize()
        
        # 開始會話
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        # 開始情境
        success = self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        self.assertTrue(success)
        
        # 檢查當前情境
        current_situation = self.roleplay_system.get_current_situation(session_id)
        self.assertEqual(current_situation["id"], "POL_NorthernExpedition")
        self.assertEqual(current_situation["title"], "北伐統一決策")
        
        print("✅ 開始情境測試通過")
    
    def test_decision_making(self):
        """測試決策制定"""
        self.roleplay_system.initialize()
        
        # 開始會話和情境
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        
        # 獲取可用決策選項
        available_options = self.roleplay_system.get_available_decision_options(session_id)
        self.assertGreater(len(available_options), 0)
        
        # 做出決策
        success = self.roleplay_system.make_decision(session_id, 0)
        self.assertTrue(success)
        
        # 檢查決策歷史
        decision_history = self.roleplay_system.get_session_history(session_id)
        self.assertGreater(len(decision_history), 0)
        
        print("✅ 決策制定測試通過")
    
    def test_decision_evaluation(self):
        """測試決策評估"""
        self.roleplay_system.initialize()
        
        # 開始會話和情境
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        
        # 評估決策
        evaluation_score = self.roleplay_system.evaluate_decision(session_id, 0)
        self.assertGreaterEqual(evaluation_score, 0.0)
        self.assertLessEqual(evaluation_score, 100.0)
        
        print("✅ 決策評估測試通過")
    
    def test_session_scoring(self):
        """測試會話評分"""
        self.roleplay_system.initialize()
        
        # 開始會話
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        # 檢查初始評分
        historical_accuracy = self.roleplay_system.get_historical_accuracy_score(session_id)
        decision_quality = self.roleplay_system.get_decision_quality_score(session_id)
        immersion = self.roleplay_system.get_immersion_score(session_id)
        
        self.assertEqual(historical_accuracy, 0.0)
        self.assertEqual(decision_quality, 0.0)
        self.assertEqual(immersion, 0.0)
        
        # 做出決策後檢查評分
        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        self.roleplay_system.make_decision(session_id, 0)
        
        new_historical_accuracy = self.roleplay_system.get_historical_accuracy_score(session_id)
        new_decision_quality = self.roleplay_system.get_decision_quality_score(session_id)
        
        self.assertGreater(new_historical_accuracy, 0.0)
        self.assertGreater(new_decision_quality, 0.0)
        
        print("✅ 會話評分測試通過")
    
    def test_session_statistics(self):
        """測試會話統計"""
        self.roleplay_system.initialize()
        
        # 開始會話
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        # 獲取統計數據
        statistics = self.roleplay_system.get_roleplay_statistics(session_id)
        
        self.assertIsInstance(statistics, dict)
        self.assertIn("HistoricalAccuracy", statistics)
        self.assertIn("DecisionQuality", statistics)
        self.assertIn("Immersion", statistics)
        self.assertIn("SituationsCompleted", statistics)
        self.assertIn("DecisionsMade", statistics)
        
        print("✅ 會話統計測試通過")
    
    def test_session_end(self):
        """測試結束會話"""
        self.roleplay_system.initialize()
        
        # 開始會話
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        
        # 結束會話
        success = self.roleplay_system.end_roleplay_session(session_id)
        self.assertTrue(success)
        
        # 檢查會話狀態
        current_session = self.roleplay_system.get_current_session()
        self.assertFalse(current_session["is_active"])
        
        print("✅ 結束會話測試通過")
    
    def test_simulation_system_initialization(self):
        """測試模擬系統初始化"""
        self.simulation_system.initialize()
        
        self.assertTrue(self.simulation_system.is_initialized)
        self.assertGreater(len(self.simulation_system.all_simulations), 0)
        self.assertGreater(len(self.simulation_system.simulation_map), 0)
        
        print("✅ 模擬系統初始化測試通過")
    
    def test_simulation_start(self):
        """測試開始模擬"""
        self.simulation_system.initialize()
        
        # 設置玩家決策
        player_decisions = {
            "MilitaryStrength": 75.0,
            "PoliticalSupport": 80.0,
            "EconomicResources": 60.0
        }
        
        # 開始模擬
        success = self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        self.assertTrue(success)
        
        # 檢查模擬狀態
        phase = self.simulation_system.get_simulation_phase("POL_NorthernExpedition")
        self.assertEqual(phase, ESimulationPhase.Execution)
        
        # 檢查當前參數
        current_parameters = self.simulation_system.get_current_parameters("POL_NorthernExpedition")
        self.assertGreater(len(current_parameters), 0)
        
        print("✅ 開始模擬測試通過")
    
    def test_simulation_progress(self):
        """測試模擬進度"""
        self.simulation_system.initialize()
        
        # 開始模擬
        player_decisions = {"MilitaryStrength": 75.0, "PoliticalSupport": 80.0, "EconomicResources": 60.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        
        # 獲取進度
        progress = self.simulation_system.get_simulation_progress("POL_NorthernExpedition")
        self.assertGreaterEqual(progress, 0.0)
        self.assertLessEqual(progress, 1.0)
        
        print("✅ 模擬進度測試通過")
    
    def test_simulation_intervention(self):
        """測試模擬干預"""
        self.simulation_system.initialize()
        
        # 開始模擬
        player_decisions = {"MilitaryStrength": 75.0, "PoliticalSupport": 80.0, "EconomicResources": 60.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        
        # 干預模擬
        success = self.simulation_system.intervene_in_simulation("POL_NorthernExpedition", "MilitaryStrength", 85.0)
        self.assertTrue(success)
        
        # 檢查參數變化
        current_parameters = self.simulation_system.get_current_parameters("POL_NorthernExpedition")
        for param in current_parameters:
            if param["name"] == "MilitaryStrength":
                self.assertEqual(param["value"], 85.0)
                break
        
        print("✅ 模擬干預測試通過")
    
    def test_simulation_prediction(self):
        """測試模擬預測"""
        self.simulation_system.initialize()
        
        # 設置參數變化
        parameter_changes = {
            "MilitaryStrength": 10.0,
            "PoliticalSupport": 5.0,
            "EconomicResources": -5.0
        }
        
        # 預測結果
        predictions = self.simulation_system.predict_simulation_outcome("POL_NorthernExpedition", parameter_changes)
        
        self.assertIsInstance(predictions, list)
        self.assertEqual(len(predictions), 3)  # 成功率、百分比、失敗率
        
        # 檢查預測值範圍
        for prediction in predictions:
            self.assertGreaterEqual(prediction, 0.0)
            self.assertLessEqual(prediction, 100.0)
        
        print("✅ 模擬預測測試通過")
    
    def test_simulation_result(self):
        """測試模擬結果"""
        self.simulation_system.initialize()
        
        # 開始模擬
        player_decisions = {"MilitaryStrength": 90.0, "PoliticalSupport": 85.0, "EconomicResources": 70.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        
        # 停止模擬以生成結果
        self.simulation_system.stop_simulation("POL_NorthernExpedition")
        
        # 獲取結果
        result = self.simulation_system.get_simulation_result("POL_NorthernExpedition")
        
        self.assertIsInstance(result, dict)
        self.assertIn("success", result)
        self.assertIn("final_score", result)
        self.assertIn("historical_accuracy", result)
        
        print("✅ 模擬結果測試通過")
    
    def test_historical_accuracy_comparison(self):
        """測試歷史準確性比較"""
        self.simulation_system.initialize()
        
        # 開始模擬
        player_decisions = {"MilitaryStrength": 85.0, "PoliticalSupport": 80.0, "EconomicResources": 65.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        self.simulation_system.stop_simulation("POL_NorthernExpedition")
        
        # 比較歷史準確性
        accuracy = self.simulation_system.compare_with_historical_baseline("POL_NorthernExpedition")
        self.assertGreaterEqual(accuracy, 0.0)
        self.assertLessEqual(accuracy, 1.0)
        
        print("✅ 歷史準確性比較測試通過")
    
    def test_simulation_statistics(self):
        """測試模擬統計"""
        self.simulation_system.initialize()
        
        # 開始模擬
        player_decisions = {"MilitaryStrength": 80.0, "PoliticalSupport": 75.0, "EconomicResources": 60.0}
        self.simulation_system.start_simulation("POL_NorthernExpedition", player_decisions)
        
        # 獲取統計數據
        statistics = self.simulation_system.get_simulation_statistics("POL_NorthernExpedition")
        
        self.assertIsInstance(statistics, dict)
        self.assertIn("Progress", statistics)
        self.assertIn("HistoricalAccuracy", statistics)
        
        print("✅ 模擬統計測試通過")
    
    def test_complete_roleplay_flow(self):
        """測試完整的角色扮演流程"""
        self.roleplay_system.initialize()
        self.simulation_system.initialize()
        
        # 1. 開始角色扮演會話
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        self.assertIsInstance(session_id, str)
        
        # 2. 獲取可用情境
        available_situations = self.roleplay_system.get_available_situations("ChiangKaiShek")
        self.assertGreater(len(available_situations), 0)
        
        # 3. 開始情境
        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        
        # 4. 獲取決策選項
        decision_options = self.roleplay_system.get_available_decision_options(session_id)
        self.assertGreater(len(decision_options), 0)
        
        # 5. 做出決策
        self.roleplay_system.make_decision(session_id, 0)
        
        # 6. 評估決策
        evaluation = self.roleplay_system.evaluate_decision(session_id, 0)
        self.assertGreater(evaluation, 0.0)
        
        # 7. 檢查評分
        historical_accuracy = self.roleplay_system.get_historical_accuracy_score(session_id)
        decision_quality = self.roleplay_system.get_decision_quality_score(session_id)
        self.assertGreater(historical_accuracy, 0.0)
        self.assertGreater(decision_quality, 0.0)
        
        # 8. 獲取統計
        statistics = self.roleplay_system.get_roleplay_statistics(session_id)
        self.assertIsInstance(statistics, dict)
        
        # 9. 結束會話
        self.roleplay_system.end_roleplay_session(session_id)
        
        # 10. 檢查最終狀態
        current_session = self.roleplay_system.get_current_session()
        self.assertFalse(current_session["is_active"])
        
        print("✅ 完整角色扮演流程測試通過")
    
    def test_historical_authenticity(self):
        """測試歷史真實性"""
        self.roleplay_system.initialize()
        
        # 檢查北伐情境的歷史準確性
        northern_expedition = self.roleplay_system.get_situation("POL_NorthernExpedition")
        self.assertEqual(northern_expedition["time_period"], "1926-1928")
        self.assertEqual(northern_expedition["location"], "廣州/南京")
        self.assertIn("ChiangKaiShek", northern_expedition["key_figures"])
        self.assertTrue(northern_expedition["is_critical_moment"])
        
        # 檢查西安事變情境
        xian_incident = self.roleplay_system.get_situation("MIL_XianIncident")
        self.assertEqual(xian_incident["time_period"], "1936-12")
        self.assertEqual(xian_incident["location"], "西安")
        self.assertIn("ChiangKaiShek", xian_incident["key_figures"])
        self.assertIn("ZhangXueliang", xian_incident["key_figures"])
        
        print("✅ 歷史真實性測試通過")
    
    def test_decision_alignment_system(self):
        """測試決策對齊系統"""
        self.roleplay_system.initialize()
        
        # 開始會話和情境
        session_id = self.roleplay_system.start_roleplay_session("ChiangKaiShek", self.test_character)
        self.roleplay_system.start_situation(session_id, "POL_NorthernExpedition")
        
        # 獲取決策選項
        decision_options = self.roleplay_system.get_available_decision_options(session_id)
        
        # 檢查決策對齊類型
        alignments = set()
        for option in decision_options:
            alignments.add(option["alignment"])
        
        self.assertIn(EDecisionAlignment.Historical, alignments)
        
        # 檢查歷史準確性
        for option in decision_options:
            self.assertGreaterEqual(option["historical_accuracy"], 0.0)
            self.assertLessEqual(option["historical_accuracy"], 1.0)
        
        print("✅ 決策對齊系統測試通過")
    
    def test_simulation_types(self):
        """測試模擬類型"""
        self.simulation_system.initialize()
        
        # 檢查所有模擬類型
        all_simulations = self.simulation_system.get_all_simulations()
        simulation_types = set()
        
        for simulation in all_simulations:
            simulation_types.add(simulation["simulation_type"])
        
        # 應該包含多種模擬類型
        self.assertIn(ESimulationType.Political, simulation_types)
        self.assertIn(ESimulationType.Military, simulation_types)
        self.assertIn(ESimulationType.Economic, simulation_types)
        
        print("✅ 模擬類型測試通過")


class MockHistoricalRoleplay:
    """模擬歷史角色扮演系統"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_situations = []
        self.situation_map = {}
        self.character_situation_map = {}
        self.active_sessions = {}
        self.current_player_character = {}
        self._initialize_mock_situations()
    
    def initialize(self):
        """初始化角色扮演系統"""
        self.is_initialized = True
        self._build_situation_maps()
    
    def _initialize_mock_situations(self):
        """初始化模擬情境"""
        situations = [
            {
                "id": "POL_NorthernExpedition",
                "title": "北伐統一決策",
                "description": "1926年，國民革命軍準備北伐，作為蔣介石，你需要決定北伐的策略和路線。",
                "historical_context": "國民革命軍北伐是統一中國的重要軍事行動",
                "time_period": "1926-1928",
                "location": "廣州/南京",
                "roleplay_type": ERoleplayType.PoliticalDecision,
                "character_role": "國民革命軍總司令",
                "key_figures": ["ChiangKaiShek", "ZhouEnlai", "ZhangZuolin"],
                "available_resources": ["國民革命軍", "蘇聯援助", "民眾支持"],
                "constraints": ["軍閥割據", "資源有限", "政治分歧"],
                "decision_options": [
                    {
                        "text": "採取激進策略，快速進攻",
                        "description": "集中兵力快速進攻，爭取在短期內統一中國",
                        "alignment": EDecisionAlignment.Historical,
                        "impact": EDecisionImpact.National,
                        "historical_accuracy": 0.9,
                        "difficulty_rating": 0.7,
                        "required_skills": ["Leadership", "TacticalCommand"],
                        "consequences": ["快速統一", "高傷亡"],
                        "risk_factors": ["過度擴張"],
                        "is_available": True
                    }
                ],
                "is_critical_moment": True,
                "can_repeat": False
            },
            {
                "id": "MIL_XianIncident",
                "title": "西安事變決策",
                "description": "1936年12月，張學良和楊虎城在西安扣留蔣介石，要求停止內戰一致抗日。",
                "historical_context": "西安事變是中國現代史的重要轉折點",
                "time_period": "1936-12",
                "location": "西安",
                "roleplay_type": ERoleplayType.MilitaryStrategy,
                "character_role": "國民政府主席",
                "key_figures": ["ChiangKaiShek", "ZhangXueliang", "YangHucheng", "ZhouEnlai"],
                "decision_options": [
                    {
                        "text": "接受停止內戰，一致抗日",
                        "description": "同意張學良的要求，建立抗日民族統一戰線",
                        "alignment": EDecisionAlignment.Historical,
                        "impact": EDecisionImpact.Historical,
                        "historical_accuracy": 0.95,
                        "difficulty_rating": 0.8,
                        "required_skills": ["Diplomacy", "Leadership"],
                        "consequences": ["抗日統一", "國共合作"],
                        "risk_factors": ["權力削弱"],
                        "is_available": True
                    }
                ],
                "is_critical_moment": True,
                "can_repeat": False
            }
        ]
        
        self.all_situations = situations
    
    def _build_situation_maps(self):
        """構建情境映射"""
        for situation in self.all_situations:
            self.situation_map[situation["id"]] = situation
            
            for key_figure in situation["key_figures"]:
                if key_figure not in self.character_situation_map:
                    self.character_situation_map[key_figure] = []
                self.character_situation_map[key_figure].append(situation["id"])
    
    def start_roleplay_session(self, character_id, player_character):
        """開始角色扮演會話"""
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
        """結束角色扮演會話"""
        if session_id in self.active_sessions:
            session = self.active_sessions[session_id]
            session["is_active"] = False
            session["end_time"] = datetime.now()
            session["immersion_score"] = self._calculate_immersion_score(session_id)
            return True
        return False
    
    def get_current_session(self):
        """獲取當前會話"""
        for session in self.active_sessions.values():
            if session["is_active"]:
                return session
        return {"is_active": False}
    
    def get_available_situations(self, character_id):
        """獲取可用情境"""
        available_situations = []
        
        if character_id in self.character_situation_map:
            situation_ids = self.character_situation_map[character_id]
            for situation_id in situation_ids:
                if situation_id in self.situation_map:
                    available_situations.append(self.situation_map[situation_id])
        
        return available_situations
    
    def start_situation(self, session_id, situation_id):
        """開始情境"""
        if session_id in self.active_sessions and situation_id in self.situation_map:
            self.active_sessions[session_id]["current_situation_id"] = situation_id
            return True
        return False
    
    def get_current_situation(self, session_id):
        """獲取當前情境"""
        if session_id in self.active_sessions:
            session = self.active_sessions[session_id]
            situation_id = session["current_situation_id"]
            if situation_id in self.situation_map:
                return self.situation_map[situation_id]
        return {}
    
    def make_decision(self, session_id, decision_index):
        """做出決策"""
        if session_id not in self.active_sessions:
            return False
        
        session = self.active_sessions[session_id]
        current_situation = self.get_current_situation(session_id)
        
        if decision_index < 0 or decision_index >= len(current_situation["decision_options"]):
            return False
        
        selected_option = current_situation["decision_options"][decision_index]
        
        # 記錄決策
        session["decision_history"].append(selected_option["text"])
        
        # 更新評分
        self._update_session_scores(session_id, decision_index)
        
        # 添加到已完成情境
        session["completed_situations"].append(session["current_situation_id"])
        
        return True
    
    def get_available_decision_options(self, session_id):
        """獲取可用決策選項"""
        current_situation = self.get_current_situation(session_id)
        if current_situation:
            return current_situation["decision_options"]
        return []
    
    def evaluate_decision(self, session_id, decision_index):
        """評估決策"""
        current_situation = self.get_current_situation(session_id)
        if decision_index < 0 or decision_index >= len(current_situation["decision_options"]):
            return 0.0
        
        selected_option = current_situation["decision_options"][decision_index]
        
        # 計算評分
        historical_score = selected_option["historical_accuracy"] * 100
        quality_score = (1.0 - selected_option["difficulty_rating"]) * 100
        
        return (historical_score + quality_score) / 2.0
    
    def get_historical_accuracy_score(self, session_id):
        """獲取歷史準確性評分"""
        if session_id in self.active_sessions:
            return self.active_sessions[session_id]["historical_accuracy_score"]
        return 0.0
    
    def get_decision_quality_score(self, session_id):
        """獲取決策質量評分"""
        if session_id in self.active_sessions:
            return self.active_sessions[session_id]["decision_quality_score"]
        return 0.0
    
    def get_immersion_score(self, session_id):
        """獲取沉浸感評分"""
        if session_id in self.active_sessions:
            return self.active_sessions[session_id]["immersion_score"]
        return 0.0
    
    def get_session_history(self, session_id):
        """獲取會話歷史"""
        if session_id in self.active_sessions:
            return self.active_sessions[session_id]["decision_history"]
        return []
    
    def get_roleplay_statistics(self, session_id):
        """獲取角色扮演統計"""
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
        """獲取情境"""
        return self.situation_map.get(situation_id, {})
    
    def _update_session_scores(self, session_id, decision_index):
        """更新會話評分"""
        if session_id not in self.active_sessions:
            return
        
        session = self.active_sessions[session_id]
        current_situation = self.get_current_situation(session_id)
        selected_option = current_situation["decision_options"][decision_index]
        
        # 更新歷史準確性
        historical_score = selected_option["historical_accuracy"] * 100
        session["historical_accuracy_score"] = (session["historical_accuracy_score"] + historical_score) / 2.0
        
        # 更新決策質量
        quality_score = (1.0 - selected_option["difficulty_rating"]) * 100
        session["decision_quality_score"] = (session["decision_quality_score"] + quality_score) / 2.0
    
    def _calculate_immersion_score(self, session_id):
        """計算沉浸感評分"""
        if session_id not in self.active_sessions:
            return 0.0
        
        session = self.active_sessions[session_id]
        decision_count = len(session["decision_history"])
        average_quality = (session["historical_accuracy_score"] + session["decision_quality_score"]) / 2.0
        
        return (decision_count * 10 + average_quality) / 2.0


class MockHistoricalSimulation:
    """模擬歷史模擬系統"""
    
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
        """初始化模擬系統"""
        self.is_initialized = True
        self._build_simulation_maps()
    
    def _initialize_mock_simulations(self):
        """初始化模擬"""
        simulations = [
            {
                "id": "POL_NorthernExpedition",
                "title": "北伐統一模擬",
                "description": "模擬1926-1928年北伐戰爭的政治決策和統一進程",
                "simulation_type": ESimulationType.Political,
                "historical_period": "1926-1928",
                "geographic_scope": "中國全境",
                "initial_parameters": [
                    {
                        "name": "MilitaryStrength",
                        "value": 60.0,
                        "min_value": 0.0,
                        "max_value": 100.0,
                        "unit": "%",
                        "description": "軍事實力"
                    },
                    {
                        "name": "PoliticalSupport",
                        "value": 70.0,
                        "min_value": 0.0,
                        "max_value": 100.0,
                        "unit": "%",
                        "description": "政治支持度"
                    },
                    {
                        "name": "EconomicResources",
                        "value": 50.0,
                        "min_value": 0.0,
                        "max_value": 100.0,
                        "unit": "%",
                        "description": "經濟資源"
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
        """構建模擬映射"""
        for simulation in self.all_simulations:
            self.simulation_map[simulation["id"]] = simulation
    
    def start_simulation(self, simulation_id, player_decisions):
        """開始模擬"""
        if simulation_id not in self.simulation_map:
            return False
        
        simulation = self.simulation_map[simulation_id]
        
        # 初始化參數
        initial_parameters = simulation["initial_parameters"].copy()
        for param in initial_parameters:
            if param["name"] in player_decisions:
                param["value"] = player_decisions[param["name"]]
        
        self.active_simulations[simulation_id] = ESimulationPhase.Execution
        self.simulation_parameters[simulation_id] = initial_parameters
        
        # 初始化歷史
        self.simulation_histories[simulation_id] = [f"模擬開始：{simulation_id}"]
        
        return True
    
    def pause_simulation(self, simulation_id):
        """暫停模擬"""
        if simulation_id in self.active_simulations:
            if self.active_simulations[simulation_id] == ESimulationPhase.Execution:
                self.active_simulations[simulation_id] = ESimulationPhase.Analysis
                return True
        return False
    
    def resume_simulation(self, simulation_id):
        """繼續模擬"""
        if simulation_id in self.active_simulations:
            if self.active_simulations[simulation_id] == ESimulationPhase.Analysis:
                self.active_simulations[simulation_id] = ESimulationPhase.Execution
                return True
        return False
    
    def stop_simulation(self, simulation_id):
        """停止模擬"""
        if simulation_id in self.active_simulations:
            self._generate_simulation_result(simulation_id)
            self.active_simulations[simulation_id] = ESimulationPhase.Results
            self.simulation_histories[simulation_id].append("模擬停止")
            return True
        return False
    
    def intervene_in_simulation(self, simulation_id, parameter_name, new_value):
        """干預模擬"""
        if simulation_id not in self.simulation_map:
            return False
        
        simulation = self.simulation_map[simulation_id]
        if not simulation["can_intervene"]:
            return False
        
        if simulation_id in self.simulation_parameters:
            for param in self.simulation_parameters[simulation_id]:
                if param["name"] == parameter_name:
                    param["value"] = max(param["min_value"], min(param["max_value"], new_value))
                    self.simulation_histories[simulation_id].append(f"玩家干預：{parameter_name} = {new_value}")
                    return True
        
        return False
    
    def get_simulation_phase(self, simulation_id):
        """獲取模擬狀態"""
        return self.active_simulations.get(simulation_id, ESimulationPhase.Setup)
    
    def get_current_parameters(self, simulation_id):
        """獲取當前參數"""
        return self.simulation_parameters.get(simulation_id, [])
    
    def get_simulation_progress(self, simulation_id):
        """獲取模擬進度"""
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
        """獲取模擬結果"""
        return self.simulation_results.get(simulation_id, {})
    
    def predict_simulation_outcome(self, simulation_id, parameter_changes):
        """預測模擬結果"""
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
        """比較歷史基準"""
        if simulation_id in self.simulation_results:
            result = self.simulation_results[simulation_id]
            return result.get("historical_accuracy", 0.0)
        return 0.0
    
    def get_simulation_statistics(self, simulation_id):
        """獲取模擬統計"""
        return {
            "Progress": self.get_simulation_progress(simulation_id),
            "HistoricalAccuracy": self.compare_with_historical_baseline(simulation_id)
        }
    
    def get_all_simulations(self):
        """獲取所有模擬"""
        return self.all_simulations.copy()
    
    def _generate_simulation_result(self, simulation_id):
        """生成模擬結果"""
        if simulation_id not in self.simulation_parameters:
            return
        
        parameters = self.simulation_parameters[simulation_id]
        simulation = self.simulation_map[simulation_id]
        
        # 檢查成功條件
        success = self._check_success_conditions(simulation_id, parameters)
        
        # 計算最終分數
        total_score = sum(param["value"] for param in parameters)
        final_score = total_score / len(parameters)
        
        # 計算歷史準確性
        historical_accuracy = 0.8 if success else 0.3
        historical_accuracy += (final_score / 100.0) * 0.2
        
        result = {
            "success": success,
            "final_score": final_score,
            "historical_accuracy": historical_accuracy,
            "final_parameters": parameters,
            "outcome_description": f"模擬結果：{'成功' if success else '失敗'}"
        }
        
        self.simulation_results[simulation_id] = result
    
    def _check_success_conditions(self, simulation_id, parameters):
        """檢查成功條件"""
        if simulation_id not in self.simulation_map:
            return False
        
        simulation = self.simulation_map[simulation_id]
        param_values = {param["name"]: param["value"] for param in parameters}
        
        # 檢查成功條件
        for condition in simulation["success_conditions"]:
            if ">" in condition:
                parts = condition.split(">")
                if len(parts) == 2:
                    param_name = parts[0].strip()
                    required_value = float(parts[1].strip())
                    if param_name in param_values:
                        if param_values[param_name] <= required_value:
                            return False
        
        # 檢查失敗條件
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
    """運行歷史角色扮演測試"""
    print("開始運行 MingGoRTS 歷史角色扮演測試...")
    print("=" * 60)
    
    # 創建測試套件
    suite = unittest.TestLoader().loadTestsFromTestCase(TestHistoricalRoleplay)
    
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
    success = run_historical_roleplay_tests()
    sys.exit(0 if success else 1)
