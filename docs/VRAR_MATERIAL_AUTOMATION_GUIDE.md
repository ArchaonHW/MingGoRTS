# VR/AR Material Creation - Automation Binding Guide

## Overview

This guide explains how to use the automation binding system for creating VR/AR materials automatically in Unreal Engine 5.

## Automation Methods

### Method 1: Editor Startup Auto-Creation (Recommended)

**How it works:**
- Automatically checks and creates materials when UE5 editor finishes loading
- Only creates if materials don't already exist
- Shows notification when complete

**Setup:**
1. Edit `Config/DefaultVRARAutomation.ini`:
```ini
[VRARAutomation]
bAutoCreateMaterials=true
```

2. Compile the project:
```powershell
.\BUILD_OPTIMIZED.cmd
```

3. Open UE5 Editor - materials will be created automatically

**Toolbar Button:**
- Look for "VR/AR Materials" button in Level Editor toolbar
- Click to manually trigger creation anytime

---

### Method 2: Build Hooks (Pre/Post Build)

**How it works:**
- Pre-build: Checks if materials exist before compiling
- Post-build: Verifies materials were created successfully

**Setup:**
1. Enable in `Config/DefaultVRARAutomation.ini`:
```ini
[VRARBuildAutomation]
bPreBuildCheck=true
bPostBuildVerification=true
bAutoCreateBeforeBuild=true
```

2. In C++ code, call before/after build:
```cpp
#include "VRAR/Automation/MingVRARBuildHooks.h"

// Pre-build check
bool CanBuild = UMingVRARBuildHooks::PreBuildMaterialCheck();

// Post-build verification  
bool Success = UMingVRARBuildHooks::PostBuildMaterialVerification();
```

**Status Check:**
```cpp
FString Status = UMingVRARBuildHooks::GetBuildAutomationStatus();
UE_LOG(LogTemp, Log, TEXT("%s"), *Status);
```

---

### Method 3: Map Open Auto-Binding

**How it works:**
- Automatically creates materials when opening VR/AR related maps
- Triggered by map name containing "VRAR" or "MaterialTest"

**Setup:**
1. Enable in config:
```ini
[VRARAutomation]
bAutoCreateOnMapOpen=true
```

2. Open any map with "VRAR" in the name to trigger creation

---

### Method 4: PowerShell Automation Script

**Usage:**
```powershell
# Auto-detect best method
.\Tools\build\AutomateMaterialCreation.ps1

# Specify method
.\Tools\build\AutomateMaterialCreation.ps1 -Method Python
.\Tools\build\AutomateMaterialCreation.ps1 -Method C++
.\Tools\build\AutomateMaterialCreation.ps1 -Method Editor

# Verify only (don't create)
.\Tools\build\AutomateMaterialCreation.ps1 -VerifyOnly

# Force recreate
.\Tools\build\AutomateMaterialCreation.ps1 -ForceRecreate

# Silent mode
.\Tools\build\AutomateMaterialCreation.ps1 -Silent
```

---

## Configuration Options

### Main Settings (`Config/DefaultVRARAutomation.ini`)

```ini
[VRARAutomation]
; Master switch for auto-creation
bAutoCreateMaterials=true

; Create when opening specific maps
bAutoCreateOnMapOpen=true

; Show toolbar button
bShowToolbarButton=true

; Enable build hooks
bEnableBuildHooks=true

; Log level (0=Error, 1=Warning, 2=Log, 3=Verbose)
LogVerbosity=2

; Delay before auto-creation (seconds)
AutoCreateDelaySeconds=5.0

; Force recreate even if exist (USE WITH CAUTION)
bForceRecreate=false
```

### Build Automation Settings

```ini
[VRARBuildAutomation]
; Check materials before building
bPreBuildCheck=true

; Verify after building
bPostBuildVerification=true

; Fail build if materials missing
bFailBuildOnMissingMaterials=false

; Auto-create if missing before build
bAutoCreateBeforeBuild=true
```

---

## Automation Files

| File | Purpose |
|------|---------|
| `Source/MingGoRTS/Public/VRAR/Automation/MingVRARMaterialAutomationModule.h` | Editor module interface |
| `Source/MingGoRTS/Private/VRAR/Automation/MingVRARMaterialAutomationModule.cpp` | Auto-creation on startup |
| `Source/MingGoRTS/Public/VRAR/Automation/MingVRARBuildHooks.h` | Build hooks interface |
| `Source/MingGoRTS/Private/VRAR/Automation/MingVRARBuildHooks.cpp` | Pre/post build checks |
| `Config/DefaultVRARAutomation.ini` | Configuration |
| `Tools/build/AutomateMaterialCreation.ps1` | PowerShell automation |

---

## Workflow Examples

### Workflow 1: Fresh Project Setup
```
1. Clone/Open project
2. Build project (auto-creation runs during editor startup)
3. Verify materials in Content Browser
```

### Workflow 2: CI/CD Integration
```powershell
# Pre-build check
.\Tools\build\AutomateMaterialCreation.ps1 -VerifyOnly
if ($LASTEXITCODE -ne 0) {
    .\Tools\build\AutomateMaterialCreation.ps1 -Method C++
}

# Build project
.\BUILD_OPTIMIZED.cmd

# Post-build verify
.\Tools\build\AutomateMaterialCreation.ps1 -VerifyOnly
```

### Workflow 3: Manual Trigger
```
1. Open UE5 Editor
2. Click "VR/AR Materials" toolbar button
3. Materials created immediately
```

---

## Troubleshooting

### Issue: Materials not auto-creating
**Check:**
1. Verify `bAutoCreateMaterials=true` in config
2. Check that MaterialConfigs.json files exist
3. Check Output Log for errors
4. Try manual creation via toolbar button

### Issue: Build hooks not working
**Check:**
1. Verify `bEnableBuildHooks=true` in config
2. Ensure build hooks are called in build script
3. Check that materials directories are writable

### Issue: Toolbar button not showing
**Check:**
1. Verify `bShowToolbarButton=true` in config
2. Recompile project
3. Restart UE5 Editor

---

## Advanced: Custom Automation

### Create Custom Event Binding
```cpp
#include "VRAR/MingVRARMaterialFactory.h"

// Bind to any event
void OnMyCustomEvent()
{
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    Factory->CreateVRMaterials();
    Factory->CreateARMaterials();
}
```

### Custom Verification Logic
```cpp
bool VerifySpecificMaterials()
{
    // Check specific materials you need
    TArray<FString> RequiredMaterials = {
        TEXT("/Game/VR/Materials/M_VRController_Body"),
        TEXT("/Game/AR/Materials/M_ARCursor_Default")
    };
    
    for (const FString& Path : RequiredMaterials)
    {
        if (!UEditorAssetLibrary::DoesAssetExist(Path))
        {
            return false;
        }
    }
    return true;
}
```

---

## Summary

The automation system provides multiple ways to ensure VR/AR materials are always created:
- **Automatic**: Editor startup, map open events
- **Manual**: Toolbar button, PowerShell script
- **Build Integration**: Pre/post build hooks
- **Flexible**: Configurable via INI files

All methods use the same core `MingVRARMaterialFactory` class, ensuring consistent results.
