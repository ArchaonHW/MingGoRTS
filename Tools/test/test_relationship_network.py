#!/usr/bin/env python3
"""
MingGoRTS 關係網絡測試腳本
測試 Story 3-5: 角色關係網絡
"""

import unittest
import json
import sys
import os
from datetime import datetime
from enum import Enum

# 添加項目根目錄到 Python 路徑
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class ERelationshipType(Enum):
    """關係類型枚舉"""
    Family = "Family"
    Friend = "Friend"
    Mentor = "Mentor"
    Rival = "Rival"
    Ally = "Ally"
    Enemy = "Enemy"
    Colleague = "Colleague"
    Subordinate = "Subordinate"
    Superior = "Superior"
    Business = "Business"
    Political = "Political"
    Military = "Military"
    Romantic = "Romantic"
    Acquaintance = "Acquaintance"
    Stranger = "Stranger"

class ERelationshipStrength(Enum):
    """關係強度枚舉"""
    VeryWeak = "VeryWeak"
    Weak = "Weak"
    Moderate = "Moderate"
    Strong = "Strong"
    VeryStrong = "VeryStrong"

class ERelationshipStatus(Enum):
    """關係狀態枚舉"""
    Active = "Active"
    Dormant = "Dormant"
    Strained = "Strained"
    Broken = "Broken"
    Developing = "Developing"
    Improving = "Improving"

class ERelationshipChangeType(Enum):
    """關係變化類型枚舉"""
    Improvement = "Improvement"
    Deterioration = "Deterioration"
    Stabilization = "Stabilization"
    Conflict = "Conflict"
    Resolution = "Resolution"
    Transformation = "Transformation"

class EInfluencePropagationType(Enum):
    """影響傳播類型枚舉"""
    Direct = "Direct"
    Indirect = "Indirect"
    Cascading = "Cascading"
    Network = "Network"
    Global = "Global"

class ERelationshipStability(Enum):
    """關係穩定性枚舉"""
    VeryStable = "VeryStable"
    Stable = "Stable"
    Moderate = "Moderate"
    Unstable = "Unstable"
    VeryUnstable = "VeryUnstable"

class TestRelationshipNetwork(unittest.TestCase):
    """關係網絡測試類"""
    
    def setUp(self):
        """測試前的設置"""
        self.relationship_network = MockRelationshipNetwork()
        self.relationship_dynamics = MockRelationshipDynamics()
        
        # 測試角色數據
        self.test_characters = [
            "ChiangKaiShek",
            "ZhouEnlai", 
            "MaoZedong",
            "ZhangXueliang",
            "SoongMeiLing"
        ]
        
        # 測試關係數據
        self.test_relationships = [
            {
                "source_id": "ChiangKaiShek",
                "target_id": "SoongMeiLing",
                "relationship_type": ERelationshipType.Romantic,
                "strength": ERelationshipStrength.VeryStrong,
                "status": ERelationshipStatus.Active,
                "relationship_value": 95.0,
                "trust_level": 90.0,
                "influence_level": 85.0,
                "is_reciprocal": True
            },
            {
                "source_id": "ChiangKaiShek",
                "target_id": "ZhouEnlai",
                "relationship_type": ERelationshipType.Political,
                "strength": ERelationshipStrength.Moderate,
                "status": ERelationshipStatus.Active,
                "relationship_value": 60.0,
                "trust_level": 40.0,
                "influence_level": 70.0,
                "is_reciprocal": True
            },
            {
                "source_id": "ZhouEnlai",
                "target_id": "MaoZedong",
                "relationship_type": ERelationshipType.Colleague,
                "strength": ERelationshipStrength.VeryStrong,
                "status": ERelationshipStatus.Active,
                "relationship_value": 90.0,
                "trust_level": 95.0,
                "influence_level": 80.0,
                "is_reciprocal": True
            }
        ]
        
        # 測試網絡數據
        self.test_network = {
            "network_id": "TestNetwork",
            "network_name": "測試網絡",
            "description": "用於測試的關係網絡",
            "character_ids": self.test_characters,
            "connections": self.test_relationships,
            "network_density": 0.6,
            "network_cohesion": 0.7,
            "central_character_id": "ChiangKaiShek",
            "key_influencers": ["ChiangKaiShek", "ZhouEnlai"],
            "bridging_characters": ["ZhangXueliang"],
            "is_stable": True,
            "is_growing": True
        }
    
    def test_relationship_network_initialization(self):
        """測試關係網絡系統初始化"""
        self.relationship_network.initialize()
        
        self.assertTrue(self.relationship_network.is_initialized)
        self.assertGreater(len(self.relationship_network.all_networks), 0)
        self.assertGreater(len(self.relationship_network.network_map), 0)
        
        print("✅ 關係網絡系統初始化測試通過")
    
    def test_create_network(self):
        """測試創建關係網絡"""
        self.relationship_network.initialize()
        
        # 創建新網絡
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        
        self.assertIsInstance(network_id, str)
        self.assertGreater(len(network_id), 0)
        self.assertIn("TestNetwork", network_id)
        
        # 檢查網絡是否創建成功
        network = self.relationship_network.get_network(network_id)
        self.assertEqual(network["network_name"], "測試網絡")
        self.assertEqual(len(network["character_ids"]), len(self.test_characters))
        
        print("✅ 創建關係網絡測試通過")
    
    def test_create_relationship(self):
        """測試創建關係連接"""
        self.relationship_network.initialize()
        
        # 創建網絡
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        
        # 創建關係
        success = self.relationship_network.create_relationship(
            "ChiangKaiShek", 
            "SoongMeiLing", 
            ERelationshipType.Romantic, 
            ERelationshipStrength.VeryStrong
        )
        
        self.assertTrue(success)
        
        # 檢查關係是否創建成功
        relationship = self.relationship_network.get_relationship("ChiangKaiShek", "SoongMeiLing")
        self.assertEqual(relationship["relationship_type"], ERelationshipType.Romantic)
        self.assertEqual(relationship["strength"], ERelationshipStrength.VeryStrong)
        self.assertGreater(relationship["relationship_value"], 0.0)
        
        print("✅ 創建關係連接測試通過")
    
    def test_update_relationship(self):
        """測試更新關係"""
        self.relationship_network.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # 更新關係
        success = self.relationship_network.update_relationship("ChiangKaiShek", "SoongMeiLing", 85.0, "時間考驗")
        
        self.assertTrue(success)
        
        # 檢查關係是否更新
        relationship = self.relationship_network.get_relationship("ChiangKaiShek", "SoongMeiLing")
        self.assertEqual(relationship["relationship_value"], 85.0)
        
        print("✅ 更新關係測試通過")
    
    def test_get_character_relationships(self):
        """測試獲取角色關係"""
        self.relationship_network.initialize()
        
        # 創建網絡和多個關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        
        # 獲取角色關係
        relationships = self.relationship_network.get_character_relationships("ChiangKaiShek")
        
        self.assertEqual(len(relationships), 2)
        
        # 檢查關係類型
        relationship_types = [rel["relationship_type"] for rel in relationships]
        self.assertIn(ERelationshipType.Romantic, relationship_types)
        self.assertIn(ERelationshipType.Political, relationship_types)
        
        print("✅ 獲取角色關係測試通過")
    
    def test_direct_connections(self):
        """測試獲取直接連接"""
        self.relationship_network.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 獲取直接連接
        direct_connections = self.relationship_network.get_direct_connections("ChiangKaiShek")
        
        self.assertEqual(len(direct_connections), 2)
        self.assertIn("SoongMeiLing", direct_connections)
        self.assertIn("ZhouEnlai", direct_connections)
        
        print("✅ 獲取直接連接測試通過")
    
    def test_indirect_connections(self):
        """測試獲取間接連接"""
        self.relationship_network.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 獲取間接連接
        indirect_connections = self.relationship_network.get_indirect_connections("ChiangKaiShek")
        
        self.assertGreater(len(indirect_connections), 0)
        self.assertIn("MaoZedong", indirect_connections)  # 通過 ZhouEnlai 的間接連接
        
        print("✅ 獲取間接連接測試通過")
    
    def test_shortest_path(self):
        """測試最短路徑計算"""
        self.relationship_network.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 計算最短路徑
        shortest_path = self.relationship_network.find_shortest_path("ChiangKaiShek", "MaoZedong")
        
        self.assertEqual(len(shortest_path), 3)
        self.assertEqual(shortest_path[0], "ChiangKaiShek")
        self.assertEqual(shortest_path[1], "ZhouEnlai")
        self.assertEqual(shortest_path[2], "MaoZedong")
        
        print("✅ 最短路徑計算測試通過")
    
    def test_network_analysis(self):
        """測試網絡分析"""
        self.relationship_network.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 分析網絡
        self.relationship_network.analyze_network(network_id)
        
        # 檢查分析結果
        network = self.relationship_network.get_network(network_id)
        self.assertGreater(network["network_density"], 0.0)
        self.assertGreater(network["network_cohesion"], 0.0)
        self.assertIsNot(network["central_character_id"], "")
        
        print("✅ 網絡分析測試通過")
    
    def test_influence_calculation(self):
        """測試影響力計算"""
        self.relationship_network.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        
        # 計算影響力
        influence = self.relationship_network.calculate_influence("ChiangKaiShek")
        
        self.assertGreater(influence, 0.0)
        
        print("✅ 影響力計算測試通過")
    
    def test_relationship_suggestions(self):
        """測試關係建議"""
        self.relationship_network.initialize()
        
        # 創建網絡和部分關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # 獲取關係建議
        suggestions = self.relationship_network.get_relationship_suggestions("ChiangKaiShek")
        
        self.assertIsInstance(suggestions, list)
        
        print("✅ 關係建議測試通過")
    
    def test_relationship_dynamics_initialization(self):
        """測試關係動態系統初始化"""
        self.relationship_dynamics.initialize()
        
        self.assertTrue(self.relationship_dynamics.is_initialized)
        
        print("✅ 關係動態系統初始化測試通過")
    
    def test_apply_relationship_change(self):
        """測試應用關係變化"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # 應用關係變化
        success = self.relationship_dynamics.apply_relationship_change(
            "ChiangKaiShek", 
            "SoongMeiLing", 
            ERelationshipChangeType.Improvement, 
            5.0, 
            "積極互動"
        )
        
        self.assertTrue(success)
        
        # 檢查變化歷史
        history = self.relationship_dynamics.get_relationship_change_history("ChiangKaiShek", "SoongMeiLing")
        self.assertGreater(len(history), 0)
        
        print("✅ 應用關係變化測試通過")
    
    def test_influence_propagation(self):
        """測試影響傳播"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 傳播影響
        propagation_id = self.relationship_dynamics.propagate_influence(
            "ChiangKaiShek", 
            "政治影響", 
            10.0, 
            EInfluencePropagationType.Network, 
            3
        )
        
        self.assertIsInstance(propagation_id, str)
        self.assertGreater(len(propagation_id), 0)
        
        # 檢查活動傳播
        active_propagations = self.relationship_dynamics.get_active_propagations()
        self.assertGreater(len(active_propagations), 0)
        
        print("✅ 影響傳播測試通過")
    
    def test_network_stability_analysis(self):
        """測試網絡穩定性分析"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 分析網絡穩定性
        self.relationship_dynamics.analyze_network_stability(network_id)
        
        # 檢查分析結果
        analysis = self.relationship_dynamics.get_stability_analysis(network_id)
        self.assertEqual(analysis["network_id"], network_id)
        self.assertIsInstance(analysis["overall_stability"], ERelationshipStability)
        self.assertGreater(analysis["stability_score"], 0.0)
        
        print("✅ 網絡穩定性分析測試通過")
    
    def test_relationship_trend_prediction(self):
        """測試關係趨勢預測"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # 應用一些變化以創建歷史
        self.relationship_dynamics.apply_relationship_change("ChiangKaiShek", "SoongMeiLing", ERelationshipChangeType.Improvement, 3.0, "積極互動")
        self.relationship_dynamics.apply_relationship_change("ChiangKaiShek", "SoongMeiLing", ERelationshipChangeType.Improvement, 2.0, "共同目標")
        self.relationship_dynamics.apply_relationship_change("ChiangKaiShek", "SoongMeiLing", ERelationshipChangeType.Deterioration, 1.0, "小分歧")
        
        # 預測趨勢
        trend = self.relationship_dynamics.predict_relationship_trend("ChiangKaiShek", "SoongMeiLing", 5)
        
        self.assertEqual(len(trend), 5)
        for value in trend:
            self.assertGreaterEqual(value, 0.0)
            self.assertLessEqual(value, 100.0)
        
        print("✅ 關係趨勢預測測試通過")
    
    def test_time_decay(self):
        """測試時間衰減"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # 計算衰減
        decay = self.relationship_dynamics.calculate_relationship_decay("ChiangKaiShek", "SoongMeiLing", 10.0)
        
        self.assertGreaterEqual(decay, 0.0)
        
        # 應用時間衰減
        self.relationship_dynamics.apply_time_decay(1.0)
        
        print("✅ 時間衰減測試通過")
    
    def test_relationship_conflicts(self):
        """測試關係衝突檢測"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 創建網絡和衝突關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Enemy, ERelationshipStrength.Strong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Friend, ERelationshipStrength.Strong)
        
        # 檢查衝突
        conflicts = self.relationship_dynamics.check_relationship_conflicts(network_id)
        
        # 應該檢測到衝突
        self.assertGreater(len(conflicts), 0)
        
        # 解決衝突
        success = self.relationship_dynamics.resolve_relationship_conflict("ChiangKaiShek", "ZhouEnlai")
        self.assertTrue(success)
        
        print("✅ 關係衝突檢測測試通過")
    
    def test_network_dynamics_calculation(self):
        """測試網絡動態計算"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        
        # 計算網絡動態
        dynamics = self.relationship_dynamics.calculate_network_dynamics(network_id)
        
        self.assertIsInstance(dynamics, dict)
        self.assertIn("ChangeFrequency", dynamics)
        self.assertIn("InfluenceStrength", dynamics)
        self.assertIn("StabilityIndex", dynamics)
        self.assertIn("ResilienceIndex", dynamics)
        
        print("✅ 網絡動態計算測試通過")
    
    def test_relationship_development_simulation(self):
        """測試關係發展模擬"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 創建網絡和關係
        network_id = self.relationship_network.create_network("測試網絡", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 模擬關係發展
        self.relationship_dynamics.simulate_relationship_development(network_id, 5)
        
        # 檢查模擬結果
        analysis = self.relationship_dynamics.get_stability_analysis(network_id)
        self.assertIsNot(analysis["network_id"], "")
        
        print("✅ 關係發展模擬測試通過")
    
    def test_complete_relationship_flow(self):
        """測試完整的關係網絡流程"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 1. 創建網絡
        network_id = self.relationship_network.create_network("完整測試網絡", self.test_characters)
        self.assertIsInstance(network_id, str)
        
        # 2. 添加關係
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 3. 分析網絡
        self.relationship_network.analyze_network(network_id)
        network = self.relationship_network.get_network(network_id)
        self.assertGreater(network["network_density"], 0.0)
        
        # 4. 應用關係變化
        self.relationship_dynamics.apply_relationship_change("ChiangKaiShek", "SoongMeiLing", ERelationshipChangeType.Improvement, 5.0, "積極互動")
        
        # 5. 傳播影響
        propagation_id = self.relationship_dynamics.propagate_influence("ChiangKaiShek", "政治影響", 10.0, EInfluencePropagationType.Network, 2)
        self.assertIsInstance(propagation_id, str)
        
        # 6. 分析穩定性
        self.relationship_dynamics.analyze_network_stability(network_id)
        analysis = self.relationship_dynamics.get_stability_analysis(network_id)
        self.assertIsInstance(analysis["overall_stability"], ERelationshipStability)
        
        # 7. 預測趨勢
        trend = self.relationship_dynamics.predict_relationship_trend("ChiangKaiShek", "SoongMeiLing", 3)
        self.assertEqual(len(trend), 3)
        
        # 8. 獲取建議
        suggestions = self.relationship_network.get_relationship_suggestions("ChiangKaiShek")
        self.assertIsInstance(suggestions, list)
        
        # 9. 模擬發展
        self.relationship_dynamics.simulate_relationship_development(network_id, 3)
        
        # 10. 檢查最終狀態
        final_network = self.relationship_network.get_network(network_id)
        self.assertGreater(len(final_network["connections"]), 0)
        
        print("✅ 完整關係網絡流程測試通過")
    
    def test_historical_network_authenticity(self):
        """測試歷史網絡真實性"""
        self.relationship_network.initialize()
        
        # 檢查國民黨網絡
        nationalist_network = self.relationship_network.get_network("NET_Nationalist")
        if nationalist_network["network_id"]:
            self.assertEqual(nationalist_network["network_name"], "國民黨網絡")
            self.assertIn("ChiangKaiShek", nationalist_network["character_ids"])
            self.assertIn("SoongMeiLing", nationalist_network["character_ids"])
        
        # 檢查共產黨網絡
        communist_network = self.relationship_network.get_network("NET_Communist")
        if communist_network["network_id"]:
            self.assertEqual(communist_network["network_name"], "共產黨網絡")
            self.assertIn("MaoZedong", communist_network["character_ids"])
            self.assertIn("ZhouEnlai", communist_network["character_ids"])
        
        # 檢查軍閥網絡
        warlord_network = self.relationship_network.get_network("NET_Warlord")
        if warlord_network["network_id"]:
            self.assertEqual(warlord_network["network_name"], "軍閥網絡")
            self.assertIn("ZhangZuolin", warlord_network["character_ids"])
            self.assertIn("ZhangXueliang", warlord_network["character_ids"])
        
        print("✅ 歷史網絡真實性測試通過")
    
    def test_relationship_type_system(self):
        """測試關係類型系統"""
        self.relationship_network.initialize()
        
        # 創建網絡
        network_id = self.relationship_network.create_network("關係類型測試", self.test_characters)
        
        # 測試不同關係類型
        relationship_types = [
            (ERelationshipType.Family, ERelationshipStrength.VeryStrong),
            (ERelationshipType.Friend, ERelationshipStrength.Strong),
            (ERelationshipType.Rival, ERelationshipStrength.Moderate),
            (ERelationshipType.Enemy, ERelationshipStrength.Weak),
            (ERelationshipType.Colleague, ERelationshipStrength.Strong),
            (ERelationshipType.Business, ERelationshipStrength.Moderate)
        ]
        
        for i, (rel_type, strength) in enumerate(relationship_types):
            if i < len(self.test_characters) - 1:
                success = self.relationship_network.create_relationship(
                    self.test_characters[i], 
                    self.test_characters[i + 1], 
                    rel_type, 
                    strength
                )
                self.assertTrue(success)
                
                # 檢查關係類型
                relationship = self.relationship_network.get_relationship(self.test_characters[i], self.test_characters[i + 1])
                self.assertEqual(relationship["relationship_type"], rel_type)
                self.assertEqual(relationship["strength"], strength)
        
        print("✅ 關係類型系統測試通過")
    
    def test_network_statistics(self):
        """測試網絡統計"""
        self.relationship_network.initialize()
        
        # 創建網絡和多個關係
        network_id = self.relationship_network.create_network("統計測試網絡", self.test_characters)
        
        # 創建多個關係
        relationships_to_create = [
            ("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong),
            ("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate),
            ("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong),
            ("ZhangXueliang", "ChiangKaiShek", ERelationshipType.Political, ERelationshipStrength.Moderate),
            ("SoongMeiLing", "ZhouEnlai", ERelationshipType.Diplomatic, ERelationshipStrength.Moderate)
        ]
        
        for source, target, rel_type, strength in relationships_to_create:
            self.relationship_network.create_relationship(source, target, rel_type, strength)
        
        # 分析網絡
        self.relationship_network.analyze_network(network_id)
        
        # 檢查統計數據
        network = self.relationship_network.get_network(network_id)
        self.assertGreater(network["network_density"], 0.0)
        self.assertGreater(network["network_cohesion"], 0.0)
        self.assertIsNot(network["central_character_id"], "")
        self.assertGreater(len(network["key_influencers"]), 0)
        
        print("✅ 網絡統計測試通過")


class MockRelationshipNetwork:
    """模擬關係網絡系統"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_networks = []
        self.network_map = {}
        self.character_network_map = {}
        self.relationship_map = {}
        self._initialize_historical_networks()
    
    def initialize(self):
        """初始化關係網絡系統"""
        self.is_initialized = True
        self._build_network_maps()
    
    def _initialize_historical_networks(self):
        """初始化歷史網絡"""
        networks = [
            {
                "network_id": "NET_Nationalist",
                "network_name": "國民黨網絡",
                "description": "國民黨內部關係網絡",
                "character_ids": ["ChiangKaiShek", "SoongMeiLing", "ChenCheng", "HeYingqin"],
                "connections": [
                    {
                        "source_character_id": "ChiangKaiShek",
                        "target_character_id": "SoongMeiLing",
                        "relationship_type": ERelationshipType.Romantic,
                        "strength": ERelationshipStrength.VeryStrong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 95.0,
                        "trust_level": 90.0,
                        "influence_level": 85.0,
                        "is_reciprocal": True
                    },
                    {
                        "source_character_id": "ChiangKaiShek",
                        "target_character_id": "ChenCheng",
                        "relationship_type": ERelationshipType.Subordinate,
                        "strength": ERelationshipStrength.Strong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 80.0,
                        "trust_level": 75.0,
                        "influence_level": 90.0,
                        "is_reciprocal": False
                    }
                ],
                "network_density": 0.0,
                "network_cohesion": 0.0,
                "central_character_id": "",
                "key_influencers": [],
                "bridging_characters": [],
                "is_stable": False,
                "is_growing": False
            },
            {
                "network_id": "NET_Communist",
                "network_name": "共產黨網絡",
                "description": "共產黨內部關係網絡",
                "character_ids": ["MaoZedong", "ZhouEnlai", "ZhuDe", "LiuShaoqi"],
                "connections": [
                    {
                        "source_character_id": "MaoZedong",
                        "target_character_id": "ZhouEnlai",
                        "relationship_type": ERelationshipType.Colleague,
                        "strength": ERelationshipStrength.VeryStrong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 90.0,
                        "trust_level": 95.0,
                        "influence_level": 80.0,
                        "is_reciprocal": True
                    },
                    {
                        "source_character_id": "ZhouEnlai",
                        "target_character_id": "ZhuDe",
                        "relationship_type": ERelationshipType.Colleague,
                        "strength": ERelationshipStrength.Strong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 75.0,
                        "trust_level": 80.0,
                        "influence_level": 70.0,
                        "is_reciprocal": True
                    }
                ],
                "network_density": 0.0,
                "network_cohesion": 0.0,
                "central_character_id": "",
                "key_influencers": [],
                "bridging_characters": [],
                "is_stable": False,
                "is_growing": False
            },
            {
                "network_id": "NET_Warlord",
                "network_name": "軍閥網絡",
                "description": "軍閥之間的關係網絡",
                "character_ids": ["ZhangZuolin", "ZhangXueliang", "FengYuxiang", "YanXishan"],
                "connections": [
                    {
                        "source_character_id": "ZhangZuolin",
                        "target_character_id": "ZhangXueliang",
                        "relationship_type": ERelationshipType.Family,
                        "strength": ERelationshipStrength.VeryStrong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 95.0,
                        "trust_level": 90.0,
                        "influence_level": 85.0,
                        "is_reciprocal": True
                    },
                    {
                        "source_character_id": "ZhangZuolin",
                        "target_character_id": "FengYuxiang",
                        "relationship_type": ERelationshipType.Rival,
                        "strength": ERelationshipStrength.Moderate,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 40.0,
                        "trust_level": 30.0,
                        "influence_level": 60.0,
                        "is_reciprocal": True
                    }
                ],
                "network_density": 0.0,
                "network_cohesion": 0.0,
                "central_character_id": "",
                "key_influencers": [],
                "bridging_characters": [],
                "is_stable": False,
                "is_growing": False
            }
        ]
        
        self.all_networks = networks
    
    def _build_network_maps(self):
        """構建網絡映射"""
        for network in self.all_networks:
            self.network_map[network["network_id"]] = network
            
            # 構建角色到網絡的映射
            for character_id in network["character_ids"]:
                self.character_network_map[character_id] = network["network_id"]
            
            # 構建關係映射
            for connection in network["connections"]:
                relationship_id = f"{connection['source_character_id']}_{connection['target_character_id']}"
                self.relationship_map[relationship_id] = connection
    
    def create_network(self, network_name, character_ids):
        """創建網絡"""
        network_id = f"NET_{network_name}_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        new_network = {
            "network_id": network_id,
            "network_name": network_name,
            "description": f"用戶創建的網絡：{network_name}",
            "character_ids": character_ids,
            "connections": [],
            "network_density": 0.0,
            "network_cohesion": 0.0,
            "central_character_id": "",
            "key_influencers": [],
            "bridging_characters": [],
            "is_stable": False,
            "is_growing": False
        }
        
        self.all_networks.append(new_network)
        self.network_map[network_id] = new_network
        
        # 構建角色到網絡的映射
        for character_id in character_ids:
            self.character_network_map[character_id] = network_id
        
        return network_id
    
    def get_network(self, network_id):
        """獲取網絡"""
        return self.network_map.get(network_id, {
            "network_id": "",
            "network_name": "",
            "description": "",
            "character_ids": [],
            "connections": [],
            "network_density": 0.0,
            "network_cohesion": 0.0,
            "central_character_id": "",
            "key_influencers": [],
            "bridging_characters": [],
            "is_stable": False,
            "is_growing": False
        })
    
    def create_relationship(self, source_id, target_id, relationship_type, strength):
        """創建關係"""
        if source_id == target_id:
            return False
        
        relationship_id = f"{source_id}_{target_id}"
        
        if relationship_id in self.relationship_map:
            return False
        
        new_connection = {
            "source_character_id": source_id,
            "target_character_id": target_id,
            "relationship_type": relationship_type,
            "strength": strength,
            "status": ERelationshipStatus.Active,
            "relationship_value": self._get_strength_value(strength) * 100.0,
            "trust_level": 50.0,
            "influence_level": 50.0,
            "is_reciprocal": self._is_symmetric(relationship_type),
            "last_interaction_date": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            "interaction_frequency": 1.0
        }
        
        self.relationship_map[relationship_id] = new_connection
        
        # 添加到網絡
        if source_id in self.character_network_map:
            network_id = self.character_network_map[source_id]
            if network_id in self.network_map:
                self.network_map[network_id]["connections"].append(new_connection)
        
        return True
    
    def update_relationship(self, source_id, target_id, new_value, reason):
        """更新關係"""
        relationship_id = f"{source_id}_{target_id}"
        
        if relationship_id not in self.relationship_map:
            return False
        
        connection = self.relationship_map[relationship_id]
        connection["relationship_value"] = max(0.0, min(100.0, new_value))
        connection["last_interaction_date"] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        connection["interaction_frequency"] += 1.0
        
        return True
    
    def get_relationship(self, source_id, target_id):
        """獲取關係"""
        relationship_id = f"{source_id}_{target_id}"
        return self.relationship_map.get(relationship_id, {
            "source_character_id": "",
            "target_character_id": "",
            "relationship_type": ERelationshipType.Stranger,
            "strength": ERelationshipStrength.Weak,
            "status": ERelationshipStatus.Active,
            "relationship_value": 0.0,
            "trust_level": 0.0,
            "influence_level": 0.0,
            "is_reciprocal": False
        })
    
    def get_character_relationships(self, character_id):
        """獲取角色關係"""
        relationships = []
        
        for relationship_id, connection in self.relationship_map.items():
            if connection["source_character_id"] == character_id or connection["target_character_id"] == character_id:
                relationships.append(connection)
        
        return relationships
    
    def get_direct_connections(self, character_id):
        """獲取直接連接"""
        direct_connections = []
        
        for relationship_id, connection in self.relationship_map.items():
            if connection["source_character_id"] == character_id:
                direct_connections.append(connection["target_character_id"])
            elif connection["target_character_id"] == character_id and connection["is_reciprocal"]:
                direct_connections.append(connection["source_character_id"])
        
        return direct_connections
    
    def get_indirect_connections(self, character_id):
        """獲取間接連接"""
        indirect_connections = []
        direct_connections = self.get_direct_connections(character_id)
        processed_characters = {character_id}
        
        # 添加直接連接
        for direct_id in direct_connections:
            processed_characters.add(direct_id)
        
        # 查找二度連接
        for direct_id in direct_connections:
            second_level_connections = self.get_direct_connections(direct_id)
            for second_level_id in second_level_connections:
                if second_level_id not in processed_characters:
                    indirect_connections.append(second_level_id)
                    processed_characters.add(second_level_id)
        
        return indirect_connections
    
    def find_shortest_path(self, source_id, target_id):
        """尋找最短路徑"""
        if source_id == target_id:
            return [source_id]
        
        # 簡化BFS實現
        from collections import deque
        
        queue = deque([source_id])
        visited = {source_id}
        parent = {}
        
        while queue:
            current = queue.popleft()
            
            if current == target_id:
                # 重建路徑
                path = []
                node = target_id
                while node != source_id:
                    path.append(node)
                    node = parent[node]
                path.append(source_id)
                return list(reversed(path))
            
            connections = self.get_direct_connections(current)
            for connection in connections:
                if connection not in visited:
                    visited.add(connection)
                    parent[connection] = current
                    queue.append(connection)
        
        return []
    
    def analyze_network(self, network_id):
        """分析網絡"""
        if network_id not in self.network_map:
            return
        
        network = self.network_map[network_id]
        
        # 計算網絡密度
        node_count = len(network["character_ids"])
        if node_count >= 2:
            possible_connections = node_count * (node_count - 1) // 2
            actual_connections = len(network["connections"])
            network["network_density"] = actual_connections / possible_connections
        
        # 計算網絡凝聚力
        if network["connections"]:
            total_strength = sum(conn["relationship_value"] for conn in network["connections"])
            network["network_cohesion"] = total_strength / (len(network["connections"]) * 100.0)
        
        # 找到中心角色
        max_influence = 0.0
        central_character = ""
        for character_id in network["character_ids"]:
            influence = self.calculate_influence(character_id)
            if influence > max_influence:
                max_influence = influence
                central_character = character_id
        
        network["central_character_id"] = central_character
        
        # 找到關鍵影響者
        influences = []
        for character_id in network["character_ids"]:
            influence = self.calculate_influence(character_id)
            influences.append((character_id, influence))
        
        influences.sort(key=lambda x: x[1], reverse=True)
        network["key_influencers"] = [char_id for char_id, _ in influences[:len(influences)//3 + 1]]
        
        # 找到橋接角色
        bridging_characters = []
        for character_id in network["character_ids"]:
            connections = self.get_direct_connections(character_id)
            if len(connections) >= 3:
                bridging_characters.append(character_id)
        
        network["bridging_characters"] = bridging_characters
        
        # 判斷網絡狀態
        network["is_stable"] = network["network_cohesion"] > 0.7
        network["is_growing"] = network["network_density"] > 0.5
    
    def calculate_influence(self, character_id):
        """計算影響力"""
        total_influence = 0.0
        relationships = self.get_character_relationships(character_id)
        
        for connection in relationships:
            weight = self._calculate_relationship_weight(connection)
            total_influence += weight
        
        return total_influence
    
    def get_relationship_suggestions(self, character_id):
        """獲取關係建議"""
        suggestions = []
        
        # 獲取間接連接
        indirect_connections = self.get_indirect_connections(character_id)
        
        # 獲取現有關係
        existing_relationships = self.get_character_relationships(character_id)
        existing_targets = {conn["target_character_id"] for conn in existing_relationships}
        
        # 建議與間接連接建立關係
        for indirect_id in indirect_connections:
            if indirect_id not in existing_targets:
                suggestions.append(f"建議與 {indirect_id} 建立關係")
        
        return suggestions
    
    def _get_strength_value(self, strength):
        """獲取強度數值"""
        strength_values = {
            ERelationshipStrength.VeryWeak: 0.1,
            ERelationshipStrength.Weak: 0.3,
            ERelationshipStrength.Moderate: 0.5,
            ERelationshipStrength.Strong: 0.8,
            ERelationshipStrength.VeryStrong: 1.0
        }
        return strength_values.get(strength, 0.5)
    
    def _is_symmetric(self, relationship_type):
        """檢查關係是否對稱"""
        symmetric_types = {
            ERelationshipType.Family,
            ERelationshipType.Friend,
            ERelationshipType.Ally,
            ERelationshipType.Rival,
            ERelationshipType.Enemy,
            ERelationshipType.Colleague,
            ERelationshipType.Acquaintance
        }
        return relationship_type in symmetric_types
    
    def _calculate_relationship_weight(self, connection):
        """計算關係權重"""
        type_weights = {
            ERelationshipType.Family: 1.0,
            ERelationshipType.Romantic: 0.95,
            ERelationshipType.Friend: 0.9,
            ERelationshipType.Ally: 0.85,
            ERelationshipType.Mentor: 0.8,
            ERelationshipType.Colleague: 0.7,
            ERelationshipType.Business: 0.6,
            ERelationshipType.Political: 0.65,
            ERelationshipType.Military: 0.7,
            ERelationshipType.Subordinate: 0.5,
            ERelationshipType.Superior: 0.55,
            ERelationshipType.Rival: 0.3,
            ERelationshipType.Enemy: 0.1,
            ERelationshipType.Acquaintance: 0.4,
            ERelationshipType.Stranger: 0.2
        }
        
        type_weight = type_weights.get(connection["relationship_type"], 0.5)
        strength_weight = self._get_strength_value(connection["strength"])
        value_weight = connection["relationship_value"] / 100.0
        
        return type_weight * strength_weight * value_weight


class MockRelationshipDynamics:
    """模擬關係動態系統"""
    
    def __init__(self):
        self.is_initialized = False
        self.relationship_changes = []
        self.active_propagations = []
        self.stability_analyses = {}
        self.relationship_network = None
        self.base_decay_rate = 0.01
        self.influence_decay_rate = 0.05
    
    def initialize(self):
        """初始化關係動態系統"""
        self.is_initialized = True
    
    def apply_relationship_change(self, source_id, target_id, change_type, change_value, reason):
        """應用關係變化"""
        change_id = f"Change_{source_id}_{target_id}_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        new_change = {
            "change_id": change_id,
            "source_character_id": source_id,
            "target_character_id": target_id,
            "change_type": change_type,
            "old_value": 50.0,  # 默認值
            "new_value": max(0.0, min(100.0, 50.0 + change_value)),
            "change_magnitude": abs(change_value),
            "change_reason": reason,
            "context": "",
            "trigger_event_id": "",
            "change_time": datetime.now(),
            "decay_rate": 0.0,
            "is_permanent": False,
            "propagates": change_type != ERelationshipChangeType.Stabilization
        }
        
        self.relationship_changes.append(new_change)
        return True
    
    def get_relationship_change_history(self, source_id, target_id):
        """獲取關係變化歷史"""
        history = []
        
        for change in self.relationship_changes:
            if change["source_character_id"] == source_id and change["target_character_id"] == target_id:
                history.append(change)
        
        return history
    
    def propagate_influence(self, source_id, influence_type, influence_value, propagation_type, max_depth):
        """傳播影響"""
        propagation_id = f"Propagation_{source_id}_{influence_type}_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        new_propagation = {
            "propagation_id": propagation_id,
            "source_character_id": source_id,
            "influence_type": influence_type,
            "propagation_type": propagation_type,
            "initial_influence": influence_value,
            "propagation_strength": 1.0,
            "max_depth": max_depth,
            "attenuation_factor": 0.5,
            "affected_characters": [],
            "influence_values": {},
            "start_time": datetime.now(),
            "end_time": datetime.now(),
            "is_active": True
        }
        
        self.active_propagations.append(new_propagation)
        return propagation_id
    
    def get_active_propagations(self):
        """獲取活動傳播"""
        return [prop for prop in self.active_propagations if prop["is_active"]]
    
    def analyze_network_stability(self, network_id):
        """分析網絡穩定性"""
        analysis = {
            "network_id": network_id,
            "overall_stability": ERelationshipStability.Moderate,
            "stability_score": 0.6,
            "character_stability": {},
            "unstable_relationships": [],
            "critical_relationships": [],
            "risk_factors": [],
            "stabilizing_factors": [],
            "predicted_stability": 0.65,
            "analysis_time": datetime.now()
        }
        
        self.stability_analyses[network_id] = analysis
        return analysis
    
    def get_stability_analysis(self, network_id):
        """獲取穩定性分析"""
        return self.stability_analyses.get(network_id, {
            "network_id": "",
            "overall_stability": ERelationshipStability.Moderate,
            "stability_score": 0.0,
            "character_stability": {},
            "unstable_relationships": [],
            "critical_relationships": [],
            "risk_factors": [],
            "stabilizing_factors": [],
            "predicted_stability": 0.0,
            "analysis_time": datetime.now()
        })
    
    def predict_relationship_trend(self, source_id, target_id, time_steps):
        """預測關係趨勢"""
        history = self.get_relationship_change_history(source_id, target_id)
        
        if len(history) < 2:
            return [50.0] * time_steps
        
        # 簡化趨勢預測
        current_value = history[-1]["new_value"] if history else 50.0
        trend = 0.0
        
        if len(history) >= 3:
            recent_values = [change["new_value"] for change in history[-3:]]
            if len(recent_values) >= 2:
                trend = (recent_values[-1] - recent_values[0]) / len(recent_values)
        
        trend_values = []
        for i in range(time_steps):
            predicted_value = current_value + (trend * (i + 1))
            trend_values.append(max(0.0, min(100.0, predicted_value)))
        
        return trend_values
    
    def calculate_relationship_decay(self, source_id, target_id, time_elapsed):
        """計算關係衰減"""
        change_frequency = self._get_change_frequency(source_id, target_id)
        
        decay_rate = self.base_decay_rate
        if change_frequency > 0.1:
            decay_rate *= 0.5
        
        decay_amount = decay_rate * time_elapsed
        return max(0.0, decay_amount)
    
    def apply_time_decay(self, delta_time):
        """應用時間衰減"""
        # 簡化實現
        pass
    
    def check_relationship_conflicts(self, network_id):
        """檢查關係衝突"""
        conflicts = []
        
        # 簡化實現：返回空列表
        return conflicts
    
    def resolve_relationship_conflict(self, source_id, target_id):
        """解決關係衝突"""
        # 簡化實現
        return True
    
    def calculate_network_dynamics(self, network_id):
        """計算網絡動態"""
        dynamics = {
            "ChangeFrequency": 0.1,
            "InfluenceStrength": 50.0,
            "StabilityIndex": 0.6,
            "ResilienceIndex": 0.7
        }
        
        return dynamics
    
    def simulate_relationship_development(self, network_id, simulation_steps):
        """模擬關係發展"""
        # 簡化實現
        pass
    
    def _get_change_frequency(self, source_id, target_id):
        """獲取變化頻率"""
        history = self.get_relationship_change_history(source_id, target_id)
        
        if len(history) < 2:
            return 0.0
        
        first_time = history[0]["change_time"]
        last_time = history[-1]["change_time"]
        time_span = last_time - first_time
        hours = time_span.total_seconds() / 3600.0
        
        if hours > 0:
            return len(history) / hours
        
        return 0.0


def run_relationship_network_tests():
    """運行關係網絡測試"""
    print("開始運行 MingGoRTS 關係網絡測試...")
    print("=" * 60)
    
    # 創建測試套件
    suite = unittest.TestLoader().loadTestsFromTestCase(TestRelationshipNetwork)
    
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
    success = run_relationship_network_tests()
    sys.exit(0 if success else 1)
