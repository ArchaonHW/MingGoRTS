import re
import os

# Fix 1: EOF errors in various files
eof_fixes = [
    (r'C:\HW\MingGoRTS\Plugins\MingCore\Source\MingCore\Public\MingAIAssetBlueprintLibrary.h', [
        (r'UObject\*\s+GenerateCharacterPortrait\s*\([^)]*\}\s*;', r'UObject* GenerateCharacterPortrait(const FString& CharacterName, ECharacterType CharacterType, EQualityLevel Quality);'),
    ]),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Localization\MingRTSCulturalAdaptationSystemTest.h', [
        (r'\(\s*\}\s*;', r'();'),
    ]),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSTechTreeTest.h', [
        (r'\(\s*\}\s*;', r'();'),
    ]),
]

for filepath, patterns in eof_fixes:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        for old, new in patterns:
            content = re.sub(old, new, content)
        # Ensure file ends with }; for class
        if not content.rstrip().endswith('};'):
            content = content.rstrip() + '\n};\n'
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed EOF: {filepath}')

# Fix 2: UENUM parentheses to braces
uenum_fixes = [
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSResourceSystem.h', 'EResourceType'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Localization\MingRTSLocalizationSystem.h', 'ETextDirection'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Network\MingClientServerManager.h', 'EServerState'),
]

for filepath, enum_name in uenum_fixes:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        content = re.sub(rf'enum\s+class\s+{enum_name}\s*:\s*uint8\s*\(', f'enum class {enum_name} : uint8 {{', content)
        content = re.sub(r'\);\s*$', '};', content, flags=re.MULTILINE)
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed UENUM: {filepath}')

# Fix 3: Function parameter errors (void Func(}; -> void Func();)
func_fixes = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\WeaknessFixer\WeaknessFixer.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSAIFilmGenerator.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Mod\MingRTSModSystem.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Localization\MingRTSTextManager.h',
]

for filepath in func_fixes:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        content = re.sub(r'\(\s*\}\s*;', '();', content)
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed func params: {filepath}')

# Fix 4: Unterminated character constants
char_const_fixes = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Security\MingRTSAntiCheat.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Launcher\MingRTSOneClickLauncher.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\GameMode\OneClick\MyOneClickGameMode.h',
]

for filepath in char_const_fixes:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
        fixed_lines = []
        for line in lines:
            # Fix lines with Chinese comments that might have issues
            if "'" in line and not line.strip().endswith("'") and not line.strip().endswith(';'):
                line = line.rstrip() + "'\n"
            fixed_lines.append(line)
        with open(filepath, 'w', encoding='utf-8') as f:
            f.writelines(fixed_lines)
        print(f'Fixed char const: {filepath}')

print("All fixes applied!")
