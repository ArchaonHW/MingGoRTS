import os
import re

# Fix function declarations using "};" instead of ");"
files_to_fix = [
    r'C:\HW\MingGoRTS\Plugins\MingBuilding\Source\MingBuilding\Public\MingBuildingActor.h',
    r'C:\HW\MingGoRTS\Plugins\MingBuilding\Source\MingBuilding\Public\MingBuildingManager.h',
    r'C:\HW\MingGoRTS\Plugins\MingBuilding\Source\MingBuilding\Public\MingBuildingBlueprintLibrary.h'
]

for filepath in files_to_fix:
    if os.path.exists(filepath):
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # Fix function declarations ending with (}; -> ();
        # Match patterns like "FunctionName(params};" -> "FunctionName(params);"
        content = re.sub(r'\(\s*\}\s*\)\s*;', '();', content)
        # Also fix (}; -> (); for parameterless functions
        content = re.sub(r'\(\};', '();', content)
        # Fix (const FString& BuildingID}; -> (const FString& BuildingID);
        content = re.sub(r'\}\s*\);', ');', content)
        
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f'Fixed: {filepath}')
    else:
        print(f'Not found: {filepath}')

print('Done!')
