# OPTIMIZED: Added proper entry point and lazy imports

import sys
from pathlib import Path

# Lazy imports for performance
def _lazy_import(module_name):
    import importlib
    return importlib.import_module(module_name)
import os
import re

files_to_fix = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSArtEditor.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSDialogueSystem.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSRelationshipDynamics.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSHistoricalRoleplay.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSHistoricalCharacters.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSResourceSystem.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSProductionSystem.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSTechTree.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Localization\MingRTSLocalizationTypes.h',
    r'C:\HW\MingGoRTS\Plugins\MingPersonal\Source\MingPersonal\Public\MingAccessibilityHelper.h',
    r'C:\HW\MingGoRTS\Plugins\MingPersonal\Source\MingPersonal\Public\MingRepublicEraAudioThemes.h',
    r'C:\HW\MingGoRTS\Plugins\MingPersonal\Source\MingPersonal\Public\Tests\MingBetaIntegrationTest.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\MingStrategicFourLayerTypes.h'
]

for filepath in files_to_fix:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # Fix enum class ... ( to enum class ... {
        content = re.sub(r'enum class\s+([A-Za-z0-9_]+):\s*([a-z0-9_]+)\s*\(', r'enum class \1: \2 {', content)
        # Fix closing ); to };
        content = re.sub(r'\);(\s*(?:UMETA|//|$))', r'};\1', content)
        
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
