#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Cleanup UBT Mutex and processes"""

import subprocess
import sys
import time

print("=== UBT 互斥锁冲突解决方案 ===\n")

# 1. 终止 UBT 相关进程
processes = [
    "UnrealBuildTool.exe",
    "UE5Editor.exe", 
    "UE4Editor.exe",
    "MSBuild.exe",
    "cl.exe",
    "link.exe"
]

print("1. 终止 UBT 相关进程...")
killed = 0
for proc in processes:
    try:
        result = subprocess.run(
            ['taskkill', '/F', '/IM', proc],
            capture_output=True,
            creationflags=subprocess.CREATE_NO_WINDOW
        )
        if result.returncode == 0:
            print(f"   ✓ 已终止: {proc}")
            killed += 1
    except Exception:
        pass

if killed == 0:
    print("   ℹ 没有找到运行的 UBT 进程")
else:
    print(f"   ✓ 共终止 {killed} 个进程")

# 2. 等待进程完全退出
print("\n2. 等待进程清理...")
time.sleep(2)
print("   ✓ 完成")

# 3. 清理 Intermediate 和 Build 目录
print("\n3. 清理编译缓存...")
import shutil
import os

dirs_to_clean = [
    r"C:\HW\MingGoRTS\Intermediate",
    r"C:\HW\MingGoRTS\Build",
    r"C:\HW\MingGoRTS\.vs"
]

cleaned = 0
for dir_path in dirs_to_clean:
    try:
        if os.path.exists(dir_path):
            shutil.rmtree(dir_path, ignore_errors=True)
            print(f"   ✓ 已清理: {os.path.basename(dir_path)}")
            cleaned += 1
    except Exception as e:
        print(f"   ⚠ 清理失败 {os.path.basename(dir_path)}: {e}")

print(f"\n=== 清理完成 ===")
print(f"终止进程: {killed}")
print(f"清理目录: {cleaned}")
print("\n现在可以重新运行编译验证:")
print("  .\Tools\build\AutoCompileDebugger.ps1 -VerifyCompile")
