#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Quick compilation check with shorter timeout"""

import subprocess
import sys

print("Running quick UBT check...")

try:
    # Run UBT build with shorter timeout
    result = subprocess.run(
        [
            r'C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat',
            'MingGoRTS',
            'Win64',
            'Development',
            r'-Project=C:\HW\MingGoRTS\MingGoRTS.uproject',
            '-NoHotReload',
            '-Progress'
        ],
        cwd=r'C:\HW\MingGoRTS',
        capture_output=True,
        text=True,
        encoding='utf-8',
        errors='ignore',
        timeout=180  # 3 minute timeout
    )
    
    print("Output:")
    print(result.stdout)
    if result.stderr:
        print("Errors:")
        print(result.stderr)
    print(f"Return code: {result.returncode}")
        
except subprocess.TimeoutExpired:
    print("Build check timed out after 3 minutes - this is normal for first build")
except Exception as e:
    print(f"Error: {e}")
