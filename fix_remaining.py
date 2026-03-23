import re
import os

# Fix UENUM errors - comprehensive list
uenum_files = [
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Innovation\MingRTSEmotionalAIEngine.h', 'EEmotionType'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Innovation\MingRTSEvolutionaryAISystem.h', 'EEvolutionPhase'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSProductionSystem.h', 'EProductionCategory'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Optimization\MingRTSPerformanceProfiler.h', 'EPerformanceLevel'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Localization\MingRTSCulturalAdaptationSystem.h', 'ECulturalRegion'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Quality\MingStaticAnalysisIntegration.h', 'EAnalysisIssueSeverity'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\QuickStart\MingRTSQuickStart.h', 'EQuickStartStep'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Replay\MingRTSReplaySystem.h', 'EReplayEventType'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Network\MingClientServerManager.h', 'EServerState'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Network\MingNetworkSecurityManager.h', 'EEncryptionLevel'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Risk\MingRiskAlertSystem.h', 'EAlertType'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Risk\MingRiskAssessment.h', 'EAssessmentStatus'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Risk\MingRiskDashboard.h', 'ERiskCategory'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Risk\MingRiskTrendAnalysis.h', 'ETrendDirection'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\SelfLearning\MingRTSAILearningIntegration.h', 'EPlayerStrategyPattern'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRARM\MingARContentOverlay.h', 'EARDeviceType'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRARM\MingVRSupportSystem.h', 'EVRDeviceType'),
    (r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Stats\MingRTSStatisticsSystem.h', 'EStatType'),
]

for filepath, enum_name in uenum_files:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        content = re.sub(rf'UENUM\([^)]*\)\s*enum\s+class\s+{enum_name}\s*:\s*uint8\s*\(', f'UENUM(BlueprintType)\nenum class {enum_name} : uint8 {{', content, flags=re.DOTALL)
        content = re.sub(rf'enum\s+class\s+{enum_name}\s*:\s*uint8\s*\(', f'enum class {enum_name} : uint8 {{', content)
        content = re.sub(r'\);(\s*\n|$)', r'};\1', content)
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed UENUM: {filepath}')

# Fix include order
include_files = [
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Memory\MingRTSMemoryManager.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Kernel\MingRTSKernel.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Process\MingRTSProcessManager.h',
    r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Services\MingRTSServiceRegistry.h',
]

for filepath in include_files:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
        generated_line = None
        other_lines = []
        for line in lines:
            if '.generated.h' in line:
                generated_line = line
            else:
                other_lines.append(line)
        if generated_line:
            insert_pos = 0
            for i, line in enumerate(other_lines):
                if line.strip().startswith('#include'):
                    insert_pos = i + 1
            other_lines.insert(insert_pos, generated_line)
            with open(filepath, 'w', encoding='utf-8') as f:
                f.writelines(other_lines)
            print(f'Fixed include: {filepath}')

# Fix void* in MemoryManager
memory_file = r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\Memory\MingRTSMemoryManager.h'
if os.path.exists(memory_file):
    with open(memory_file, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    content = re.sub(r'UPROPERTY\([^)]*\)\s*\n\s*void\*\s+(\w+)', r'// UPROPERTY()\n// void* \1 // Not supported', content)
    content = re.sub(r'void\*\s+(\w+)\s*;', r'// void* \1; // Not supported', content)
    with open(memory_file, 'w', encoding='utf-8') as f:
        f.write(content)
    print(f'Fixed void*: {memory_file}')

print('Done!')
