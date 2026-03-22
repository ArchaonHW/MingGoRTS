import os
import re

# Fix additional UHT errors
files_to_fix = [
    # Fix UENUM with parentheses
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Optimization\MingRTSOptimizationCompiler.h', 'uenum'),
    # Fix unterminated character constants
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Spectator\MingRTSSpectatorSystem.h', 'char_const'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Security\MingRTSSecurityManager.h', 'char_const'),
]

for filepath, fix_type in files_to_fix:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        if fix_type == 'uenum':
            # Fix enum class ... ( to enum class ... {
            content = re.sub(r'enum class\s+([A-Za-z0-9_]+):\s*([a-z0-9_]+)\s*\(', r'enum class \1: \2 {', content)
            # Fix closing ); to };
            content = re.sub(r'\);(\s*(?:UMETA|//|$))', r'};\1', content)
        elif fix_type == 'char_const':
            # Fix common unterminated character constant issues
            # Replace Chinese punctuation that might cause issues
            content = content.replace('"', '"').replace('"', '"')
            content = content.replace(''', "'").replace(''', "'")
        
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed: {filepath}')
    else:
        print(f'Not found: {filepath}')

print('Done!')
