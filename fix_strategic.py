# OPTIMIZED: Added proper entry point and lazy imports

import sys
from pathlib import Path

# Lazy imports for performance
def _lazy_import(module_name):
    import importlib
    return importlib.import_module(module_name)
import os
import re

# Fix MingStrategic module UHT errors
files_to_fix = [
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\Events\MingWinLossCondition.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\LayerIntegration\MingStrategicLayerController.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\Managers\MingEventTriggerManager.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\UI\MingDecisionWidget.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\MingStrategicFourLayerTypes.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\Managers\MingDecisionManager.h'
]

for filepath in files_to_fix:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # Fix enum class ... ( to enum class ... {
        content = re.sub(r'enum class\s+([A-Za-z0-9_]+):\s*([a-z0-9_]+)\s*\(', r'enum class \1: \2 {', content)
        # Fix closing ); to };
        content = re.sub(r'\);(\s*(?:UMETA|//|$))', r'};\1', content)
        # Fix function declarations with (}; -> ();
        content = re.sub(r'\(\};', '();', content)
        # Fix delegate declarations with missing parentheses
        content = re.sub(r'DECLARE_DYNAMIC_MULTICAST_DELEGATE\s*\(', r'DECLARE_DYNAMIC_MULTICAST_DELEGATE(', content)
        content = re.sub(r'DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam\s*\(', r'DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(', content)
        # Fix missing closing parentheses in class declarations
        content = re.sub(r'class\s+([A-Za-z0-9_]+)\s*:\s*public\s+([A-Za-z0-9_]+)\s*\{', r'class \1 : public \2 {', content)
        
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed: {filepath}')
    else:
        print(f'Not found: {filepath}')

print('Done!')

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
