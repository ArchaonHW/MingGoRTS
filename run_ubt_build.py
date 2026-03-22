#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Run UBT compilation and capture errors"""

import subprocess
import sys

print("Starting UBT compilation...")
print("=" * 60)

try:
    # Run UBT build
    result = subprocess.run(
        [
            r'C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat',
            'MingGoRTS',
            'Win64',
            'Development',
            r'-Project=C:\HW\MingGoRTS\MingGoRTS.uproject',
            '-NoHotReload'
        ],
        cwd=r'C:\HW\MingGoRTS',
        capture_output=True,
        text=True,
        encoding='utf-8',
        errors='ignore',
        timeout=600  # 10 minute timeout
    )
    
    print("STDOUT:")
    print(result.stdout)
    print("\nSTDERR:")
    print(result.stderr)
    print(f"\nReturn code: {result.returncode}")
    
    # Check for errors
    if "Error:" in result.stdout or "Error:" in result.stderr:
        print("\n" + "=" * 60)
        print("COMPILATION ERRORS DETECTED")
        print("=" * 60)
        
        # Extract error lines
        for line in (result.stdout + result.stderr).split('\n'):
            if 'Error:' in line or 'error:' in line.lower():
                print(line)
    elif result.returncode == 0:
        print("\n" + "=" * 60)
        print("COMPILATION SUCCESSFUL!")
        print("=" * 60)
    else:
        print(f"\nBuild completed with return code: {result.returncode}")
        
except subprocess.TimeoutExpired:
    print("Build timed out after 10 minutes")
except Exception as e:
    print(f"Error running build: {e}")
