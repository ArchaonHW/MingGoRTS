import os
import re

# Fix additional UHT errors
files_to_fix = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\UserFeedback\UserFeedbackSystem.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRAR\MingRTSARSupport.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRAR\MingRTSVRSupport.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Optimization\MingRTSAlgorithmOptimizer.h'
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
        # Fix (}; to (); for parameters with (}; 
        content = re.sub(r'\(\};', '();', content)
        # Fix missing semicolon in struct
        content = re.sub(r'\}(\s*UPROPERTY|\s*UFUNCTION|\s*\n\s*GENERATED)', r'};\1', content)
        
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed: {filepath}')
    else:
        print(f'Not found: {filepath}')

print('Done!')
