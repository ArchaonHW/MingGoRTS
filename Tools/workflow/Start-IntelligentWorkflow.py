#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
智能工作流啟動器 - BMAD (Brain-Modulated Agile Development)
為 MingGoRTS 項目提供自動化開發任務管理
"""

import sys
import json
import time
import argparse
from datetime import datetime
from pathlib import Path
from typing import List, Dict, Optional
from dataclasses import dataclass, asdict
from enum import Enum

class TaskPriority(Enum):
    CRITICAL = 0
    HIGH = 1
    MEDIUM = 2
    LOW = 3
    BACKGROUND = 4

class TaskType(Enum):
    COMPILATION_FIX = "compilation_fix"
    FEATURE_DEV = "feature_dev"
    OPTIMIZATION = "optimization"
    TESTING = "testing"
    DOCUMENTATION = "documentation"
    REFACTORING = "refactoring"

@dataclass
class WorkflowTask:
    id: int
    title: str
    description: str
    task_type: TaskType
    priority: TaskPriority
    estimated_hours: int
    affected_files: List[str]
    dependencies: List[int]
    state: str = "pending"
    actual_hours: int = 0
    created_at: str = ""
    
    def __post_init__(self):
        if not self.created_at:
            self.created_at = datetime.now().isoformat()

class TaskLoader:
    """從不同來源加載任務"""
    
    @staticmethod
    def load_from_json(filepath: str) -> List[WorkflowTask]:
        """從JSON文件加載任務"""
        tasks = []
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                data = json.load(f)
                for item in data:
                    task = WorkflowTask(
                        id=item.get('id', 0),
                        title=item.get('title', ''),
                        description=item.get('description', ''),
                        task_type=TaskType(item.get('type', 'feature_dev')),
                        priority=TaskPriority(item.get('priority', 2)),
                        estimated_hours=item.get('estimated_hours', 1),
                        affected_files=item.get('affected_files', []),
                        dependencies=item.get('dependencies', [])
                    )
                    tasks.append(task)
        except Exception as e:
            print(f"加載任務失敗: {e}")
        return tasks
    
    @staticmethod
    def load_compilation_errors(project_root: str) -> List[WorkflowTask]:
        """從編譯錯誤日誌生成任務"""
        tasks = []
        # 這裡可以解析編譯錯誤日誌
        return tasks
    
    @staticmethod
    def load_sage_command_tasks() -> List[WorkflowTask]:
        """加載至聖者指揮學開發任務"""
        tasks = []
        
        # Phase 1: 核心系統
        tasks.extend([
            WorkflowTask(1, "角色類型系統-聖者實現", "實現聖者角色類型及其能力系統", 
                        TaskType.FEATURE_DEV, TaskPriority.HIGH, 8, [], []),
            WorkflowTask(2, "角色類型系統-魔王實現", "實現魔王角色類型及其能力系統", 
                        TaskType.FEATURE_DEV, TaskPriority.HIGH, 8, [], [1]),
            WorkflowTask(3, "三權指揮系統-道權", "實現道權指揮系統", 
                        TaskType.FEATURE_DEV, TaskPriority.HIGH, 12, [], []),
            WorkflowTask(4, "三權指揮系統-策權", "實現策權指揮系統", 
                        TaskType.FEATURE_DEV, TaskPriority.HIGH, 12, [], [3]),
            WorkflowTask(5, "三權指揮系統-兵權", "實現兵權指揮系統", 
                        TaskType.FEATURE_DEV, TaskPriority.HIGH, 12, [], [3]),
        ])
        
        # Phase 2: 策略系統
        tasks.extend([
            WorkflowTask(6, "正逆六策-立國", "實現立國策略", 
                        TaskType.FEATURE_DEV, TaskPriority.MEDIUM, 6, [], []),
            WorkflowTask(7, "正逆六策-立制", "實現立制策略", 
                        TaskType.FEATURE_DEV, TaskPriority.MEDIUM, 6, [], [6]),
            WorkflowTask(8, "正逆六策-立人", "實現立人策略", 
                        TaskType.FEATURE_DEV, TaskPriority.MEDIUM, 6, [], [6]),
            WorkflowTask(9, "五行節奏-木階段", "實現木階段節奏效果", 
                        TaskType.FEATURE_DEV, TaskPriority.MEDIUM, 8, [], []),
            WorkflowTask(10, "五行節奏-火階段", "實現火階段節奏效果", 
                        TaskType.FEATURE_DEV, TaskPriority.MEDIUM, 8, [], [9]),
        ])
        
        return tasks

class WorkflowReporter:
    """工作流報告生成器"""
    
    @staticmethod
    def generate_report(tasks: List[WorkflowTask], output_path: str):
        """生成HTML報告"""
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        
        html = f"""<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>MingGoRTS 智能工作流報告</title>
    <style>
        body {{ font-family: Arial, sans-serif; margin: 20px; background: #f5f5f5; }}
        .header {{ background: #2c3e50; color: white; padding: 20px; border-radius: 5px; }}
        .stats {{ display: flex; gap: 20px; margin: 20px 0; }}
        .stat-card {{ background: white; padding: 15px; border-radius: 5px; flex: 1; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }}
        .task-list {{ background: white; padding: 20px; border-radius: 5px; }}
        .task {{ padding: 10px; margin: 5px 0; border-left: 4px solid; }}
        .priority-critical {{ border-color: #e74c3c; }}
        .priority-high {{ border-color: #e67e22; }}
        .priority-medium {{ border-color: #f1c40f; }}
        .priority-low {{ border-color: #3498db; }}
        .state-pending {{ background: #ecf0f1; }}
        .state-in-progress {{ background: #fff3cd; }}
        .state-completed {{ background: #d4edda; }}
    </style>
</head>
<body>
    <div class="header">
        <h1>🎮 MingGoRTS 智能工作流報告</h1>
        <p>生成時間: {timestamp}</p>
    </div>
"""
        
        # 統計數據
        total = len(tasks)
        pending = len([t for t in tasks if t.state == "pending"])
        completed = len([t for t in tasks if t.state == "completed"])
        in_progress = len([t for t in tasks if t.state == "in_progress"])
        
        total_estimated = sum(t.estimated_hours for t in tasks)
        total_actual = sum(t.actual_hours for t in tasks)
        
        html += f"""
    <div class="stats">
        <div class="stat-card">
            <h3>總任務數</h3>
            <p style="font-size: 24px; color: #2c3e50;">{total}</p>
        </div>
        <div class="stat-card">
            <h3>完成率</h3>
            <p style="font-size: 24px; color: #27ae60;">{completed}/{total} ({completed/total*100:.1f}%)</p>
        </div>
        <div class="stat-card">
            <h3>預估工時</h3>
            <p style="font-size: 24px; color: #3498db;">{total_estimated}h</p>
        </div>
        <div class="stat-card">
            <h3>實際工時</h3>
            <p style="font-size: 24px; color: #9b59b6;">{total_actual}h</p>
        </div>
    </div>
"""
        
        # 任務列表
        html += '<div class="task-list"><h2>任務詳情</h2>'
        for task in tasks:
            priority_class = f"priority-{task.priority.name.lower()}"
            state_class = f"state-{task.state.replace('-', '_')}"
            html += f"""
        <div class="task {priority_class} {state_class}">
            <strong>[{task.id}] {task.title}</strong><br>
            <small>類型: {task.task_type.value} | 優先級: {task.priority.name} | 狀態: {task.state}</small><br>
            <small>預估: {task.estimated_hours}h | 實際: {task.actual_hours}h</small>
        </div>
"""
        html += '</div></body></html>'
        
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(html)
        
        print(f"報告已生成: {output_path}")

class IntelligentWorkflowLauncher:
    """智能工作流啟動器"""
    
    def __init__(self, project_root: str = "."):
        self.project_root = Path(project_root)
        self.tasks: List[WorkflowTask] = []
        self.running = False
    
    def load_default_tasks(self):
        """加載默認任務集"""
        print("🔍 加載任務...")
        
        # 加載至聖者指揮學任務
        sage_tasks = TaskLoader.load_sage_command_tasks()
        self.tasks.extend(sage_tasks)
        
        print(f"✅ 已加載 {len(self.tasks)} 個任務")
    
    def start_workflow(self, mode: str = "interactive"):
        """啟動工作流"""
        print(f"🚀 啟動智能工作流 (模式: {mode})")
        self.running = True
        
        # 模擬工作流運行
        print("\n📊 任務概覽:")
        print("-" * 60)
        
        for priority in [TaskPriority.CRITICAL, TaskPriority.HIGH, TaskPriority.MEDIUM, TaskPriority.LOW]:
            priority_tasks = [t for t in self.tasks if t.priority == priority]
            if priority_tasks:
                print(f"\n{priority.name} 優先級 ({len(priority_tasks)} 個任務):")
                for task in priority_tasks[:5]:  # 只顯示前5個
                    print(f"  [{task.id}] {task.title} ({task.estimated_hours}h)")
                if len(priority_tasks) > 5:
                    print(f"  ... 還有 {len(priority_tasks) - 5} 個任務")
        
        print("\n" + "=" * 60)
        print("工作流已啟動，監控面板運行中...")
        print("按 Ctrl+C 停止")
        
        try:
            while self.running:
                time.sleep(1)
        except KeyboardInterrupt:
            self.stop_workflow()
    
    def stop_workflow(self):
        """停止工作流"""
        print("\n🛑 停止工作流...")
        self.running = False
        
        # 生成報告
        report_path = self.project_root / "Reports" / "WorkflowReport.html"
        report_path.parent.mkdir(parents=True, exist_ok=True)
        WorkflowReporter.generate_report(self.tasks, str(report_path))
        
        print("✅ 工作流已停止")
    
    def export_tasks(self, filepath: str):
        """導出任務到JSON"""
        data = []
        for task in self.tasks:
            data.append({
                'id': task.id,
                'title': task.title,
                'description': task.description,
                'type': task.task_type.value,
                'priority': task.priority.value,
                'estimated_hours': task.estimated_hours,
                'actual_hours': task.actual_hours,
                'state': task.state,
                'affected_files': task.affected_files,
                'dependencies': task.dependencies,
                'created_at': task.created_at
            })
        
        with open(filepath, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2, ensure_ascii=False)
        
        print(f"任務已導出: {filepath}")

def main():
    parser = argparse.ArgumentParser(description='MingGoRTS 智能工作流啟動器')
    parser.add_argument('--mode', '-m', choices=['interactive', 'automatic', 'monitor', 'batch'],
                       default='interactive', help='工作流模式')
    parser.add_argument('--tasks', '-t', help='任務JSON文件路徑')
    parser.add_argument('--export', '-e', help='導出任務到JSON文件')
    parser.add_argument('--project-root', '-p', default='f:/MingGoRTS', help='項目根目錄')
    
    args = parser.parse_args()
    
    launcher = IntelligentWorkflowLauncher(args.project_root)
    
    # 加載任務
    if args.tasks:
        launcher.tasks = TaskLoader.load_from_json(args.tasks)
    else:
        launcher.load_default_tasks()
    
    # 導出任務
    if args.export:
        launcher.export_tasks(args.export)
        return
    
    # 啟動工作流
    launcher.start_workflow(args.mode)

if __name__ == "__main__":
    main()
