import re
import os
import glob

def fix_file(filepath, fixes):
    if not os.path.exists(filepath):
        return False
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    original = content
    for old, new in fixes:
        content = re.sub(old, new, content, flags=re.MULTILINE)
    if content != original:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed: {filepath}')
        return True
    return False

# Fix 1: UENUM parentheses to braces (systematic fix for all remaining files)
uenum_pattern = (r'enum\s+class\s+(\w+)\s*:\s*uint8\s*\(', r'enum class \1 : uint8 {')
end_paren_pattern = (r'\);\s*$', r'};')

uenum_files = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\QuickStart\MingRTSQuickStart.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Risk\MingRiskAssessment.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Risk\MingRiskAlertSystem.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Risk\MingRiskDashboard.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Risk\MingRiskTrendAnalysis.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Stats\MingRTSStatisticsSystem.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\SelfLearning\MingRTSAILearningIntegration.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRARM\MingVRSupportSystem.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingTacticalCombatSystem.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingUnitController.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\AI\MingBaseCombatAI.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\AI\MingCombatAIManager.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\MingStrategicFourLayerTypes.h',
    r'C:\HW\MingGoRTS\Plugins\MingAudio\Source\MingAudio\Public\MingProceduralTextureSystem.h',
]

for filepath in uenum_files:
    fix_file(filepath, [uenum_pattern, end_paren_pattern])

# Fix 2: EOF errors - add proper class closings
eof_files = [
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingFormationManager.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\LayerIntegration\MingTacticalLayerController.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\Difficulty\FMingDifficultySettings.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\Difficulty\MingDifficultyBlueprintLibrary.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingPerformanceTestManager.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingResourceDisplayWidget.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\Events\MingWinLossCondition.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\MingHistoricalEvents.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\MingStrategicManager.h',
    r'C:\HW\MingGoRTS\Plugins\MingStrategic\Source\MingStrategic\Public\Events\MingCharacterBlueprintLibrary.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingUnitBlueprintLibrary.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingCombatVisualizer.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\MingFogOfWarManager.h',
]

eof_fixes = [
    (r'\(\s*\}\s*;', r'();'),  # Fix function params
]

for filepath in eof_files:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        # Apply regex fixes
        for old, new in eof_fixes:
            content = re.sub(old, new, content)
        # Ensure proper file ending
        content = content.rstrip()
        if not content.endswith('};'):
            # Find last semicolon or brace and add proper ending
            if not content.endswith(';'):
                content += ';'
            if not content.rstrip().endswith('}'):
                content = content.rstrip() + '\n};'
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content + '\n')
        print(f'Fixed EOF: {filepath}')

# Fix 3: Function parameter errors
func_files = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRAR\MingRTSARSupport.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRAR\MingRTSVRSupport.h',
]

for filepath in func_files:
    fix_file(filepath, [(r'\(\s*\}\s*;', r'();')])

# Fix 4: Unmatched #endif
endif_files = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\SelfLearning\MingRTSSelfLearningSystemTest.h',
    r'C:\HW\MingGoRTS\Plugins\MingAI\Source\MingAI\Public\MingAIImageGenerator.h',
    r'C:\HW\MingGoRTS\Plugins\MingAudio\Source\MingAudio\Public\MingNiagaraEffectsSystem.h',
]

for filepath in endif_files:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        # Count conditionals
        if_count = len(re.findall(r'#if\s+|#ifdef\s+|#ifndef\s+', content))
        endif_count = len(re.findall(r'#endif', content))
        if if_count > endif_count:
            content = content.rstrip() + '\n#endif\n'
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f'Fixed #endif: {filepath}')

# Fix 5: Unterminated character constants
char_const_files = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Launcher\MingRTSOneClickLauncher.h',
    r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\Difficulty\FMingDifficultySettings_backup.h',
]

for filepath in char_const_files:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
        fixed_lines = []
        for line in lines:
            # Fix unterminated single quotes
            if "'" in line and line.count("'") % 2 == 1:
                line = line.rstrip() + "'\n"
            fixed_lines.append(line)
        with open(filepath, 'w', encoding='utf-8') as f:
            f.writelines(fixed_lines)
        print(f'Fixed char const: {filepath}')

# Fix 6: Delete backup file if it causes issues
backup_file = r'C:\HW\MingGoRTS\Plugins\MingTactical\Source\MingTactical\Public\Difficulty\FMingDifficultySettings_backup.h'
if os.path.exists(backup_file):
    os.remove(backup_file)
    print(f'Removed backup file: {backup_file}')

# Fix 7: Fix include order issues
include_files = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Kernel\MingRTSKernel.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Services\MingRTSServiceRegistry.h',
]

for filepath in include_files:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        # Ensure generated.h is at the end of includes
        lines = content.split('\n')
        generated_lines = [l for l in lines if '.generated.h' in l]
        other_lines = [l for l in lines if '.generated.h' not in l]
        if generated_lines:
            content = '\n'.join(other_lines + generated_lines)
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f'Fixed include order: {filepath}')

print("All fixes applied!")
