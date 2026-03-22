import re
import os

# Fix 1: UENUM parentheses to braces (remaining files)
uenum_files = [
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\SelfLearning\MingRTSAILearningIntegration.h', 'EPlayerStrategyPattern'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Stats\MingRTSStatisticsSystem.h', 'EStatType'),
]

for filepath, enum_name in uenum_files:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        content = re.sub(rf'enum\s+class\s+{enum_name}\s*:\s*uint8\s*\(', f'enum class {enum_name} : uint8 {{', content)
        content = re.sub(r'\);\s*$', '};', content, flags=re.MULTILINE)
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed UENUM: {filepath}')

# Fix 2: Function parameter errors
func_files = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRAR\MingRTSARSupport.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRAR\MingRTSVRSupport.h',
]

for filepath in func_files:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        # Fix void Func(}; -> void Func();
        content = re.sub(r'\(\s*\}\s*;', '();', content)
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed func: {filepath}')

# Fix 3: Unmatched #endif
endif_file = r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\SelfLearning\MingRTSSelfLearningSystemTest.h'
if os.path.exists(endif_file):
    with open(endif_file, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    # Count #if, #ifdef, #ifndef
    if_count = content.count('#if ') + content.count('#ifdef ') + content.count('#ifndef ')
    # Count #endif
    endif_count = content.count('#endif')
    if if_count > endif_count:
        # Add missing #endif
        content = content.rstrip() + '\n#endif\n'
        with open(endif_file, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed #endif: {endif_file}')

# Fix 4: Delegate issues in MingSocialDynamicsSystem.h
delegate_file = r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Innovation\MingSocialDynamicsSystem.h'
if os.path.exists(delegate_file):
    with open(delegate_file, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    # Fix common delegate issues
    content = content.replace('DECLARE_DYNAMIC_MULTICAST_Delegate', 'DECLARE_DYNAMIC_MULTICAST_DELEGATE')
    content = content.replace('DECLARE_DYNAMIC_MULTICAST_Delegate_OneParam', 'DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam')
    content = content.replace('DECLARE_DYNAMIC_MULTICAST_Delegate_TwoParams', 'DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams')
    with open(delegate_file, 'w', encoding='utf-8') as f:
        f.write(content)
    print(f'Fixed delegate: {delegate_file}')

print("All fixes applied!")
