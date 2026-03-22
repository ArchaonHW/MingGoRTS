# VR/AR Material Asset Specification

## Overview

This document specifies the UE5 material assets created for the MingGoRTS VR/AR Support System.

**Generated Files:**
- `Tools/build/CreateVRARMaterials.py` - Python script for UE5 Editor
- `Source/MingGoRTS/Public/VRAR/MingVRARMaterialFactory.h` - C++ Factory Header
- `Source/MingGoRTS/Private/VRAR/MingVRARMaterialFactory.cpp` - C++ Factory Implementation

---

## VR Materials (16 Total)

### Controller Materials

#### M_VRController_Body
- **Path:** `/Game/VR/Materials/M_VRController_Body`
- **Type:** Opaque
- **Base Color:** RGB(0.1, 0.1, 0.15) - Dark blue-gray
- **Roughness:** 0.3 - Slightly rough
- **Metallic:** 0.1 - Low metallic
- **Parameters:** BaseColor, Roughness, Metallic

#### M_VRController_Button
- **Path:** `/Game/VR/Materials/M_VRController_Button`
- **Type:** Emissive
- **Base Color:** RGB(0.0, 0.8, 1.0) - Cyan
- **Emissive:** RGB(0.0, 0.8, 1.0) - Self-illuminated cyan
- **Roughness:** 0.2 - Smooth
- **Metallic:** 0.0 - Non-metallic
- **Shading Model:** Unlit

### Pointer Materials

#### M_VRPointer_Laser
- **Path:** `/Game/VR/Materials/M_VRPointer_Laser`
- **Type:** Translucent Emissive
- **Base Color:** RGB(0.0, 0.8, 1.0) - Cyan
- **Emissive:** RGB(0.0, 0.8, 1.0) - Self-illuminated
- **Roughness:** 0.0 - Perfectly smooth
- **Opacity:** Configurable via parameter
- **Blend Mode:** Translucent
- **Shading Model:** Unlit

#### M_VRPointer_Cursor
- **Path:** `/Game/VR/Materials/M_VRPointer_Cursor`
- **Type:** Emissive
- **Base Color:** RGB(0.0, 1.0, 0.5) - Green
- **Emissive:** RGB(0.0, 1.0, 0.5) - Self-illuminated green
- **Roughness:** 0.1 - Very smooth
- **Shading Model:** Unlit

### UI Materials

#### M_VRUI_Panel
- **Path:** `/Game/VR/Materials/M_VRUI_Panel`
- **Type:** Translucent
- **Base Color:** RGB(0.1, 0.1, 0.15) - Dark
- **Opacity:** 0.9 - 90% opaque
- **Roughness:** 0.5 - Medium roughness
- **Metallic:** 0.0 - Non-metallic
- **Blend Mode:** Translucent

#### M_VRUI_Border
- **Path:** `/Game/VR/Materials/M_VRUI_Border`
- **Type:** Emissive
- **Base Color:** RGB(0.0, 0.8, 1.0) - Cyan
- **Emissive:** RGB(0.0, 0.8, 1.0) - Self-illuminated cyan
- **Roughness:** 0.2 - Smooth
- **Shading Model:** Unlit

### Teleport Materials

#### M_VRTeleport_Valid
- **Path:** `/Game/VR/Materials/M_VRTeleport_Valid`
- **Type:** Emissive
- **Base Color:** RGB(0.0, 1.0, 0.0) - Green
- **Emissive:** RGB(0.0, 1.0, 0.0) - Self-illuminated green
- **Roughness:** 0.1 - Smooth
- **Purpose:** Valid teleport destination indicator
- **Shading Model:** Unlit

#### M_VRTeleport_Invalid
- **Path:** `/Game/VR/Materials/M_VRTeleport_Invalid`
- **Type:** Emissive
- **Base Color:** RGB(1.0, 0.0, 0.0) - Red
- **Emissive:** RGB(1.0, 0.0, 0.0) - Self-illuminated red
- **Roughness:** 0.1 - Smooth
- **Purpose:** Invalid teleport destination indicator
- **Shading Model:** Unlit

### Comfort Materials

#### M_VRComfort_Vignette
- **Path:** `/Game/VR/Materials/M_VRComfort_Vignette`
- **Type:** Translucent
- **Base Color:** RGB(0.0, 0.0, 0.0) - Black
- **Opacity:** 0.8 - 80% opaque
- **Blend Mode:** Translucent
- **Purpose:** VR comfort vignette effect

---

## AR Materials (7 Total)

### Plane Materials

#### M_ARPlane_Floor
- **Path:** `/Game/AR/Materials/M_ARPlane_Floor`
- **Type:** Translucent Grid
- **Base Color:** RGBA(0.0, 0.8, 1.0, 0.3) - Transparent cyan
- **Roughness:** 0.8 - Rough
- **Metallic:** 0.0 - Non-metallic
- **Grid Texture:** Yes - Procedural grid pattern
- **Blend Mode:** Translucent

#### M_ARPlane_Wall
- **Path:** `/Game/AR/Materials/M_ARPlane_Wall`
- **Type:** Translucent
- **Base Color:** RGBA(0.8, 0.0, 1.0, 0.3) - Transparent purple
- **Roughness:** 0.8 - Rough
- **Metallic:** 0.0 - Non-metallic
- **Blend Mode:** Translucent

#### M_ARPlane_Boundary
- **Path:** `/Game/AR/Materials/M_ARPlane_Boundary`
- **Type:** Emissive
- **Base Color:** RGB(0.0, 0.8, 1.0) - Cyan
- **Emissive:** RGB(0.0, 0.8, 1.0) - Self-illuminated cyan
- **Roughness:** 0.1 - Smooth
- **Purpose:** Plane boundary visualization
- **Shading Model:** Unlit

### Cursor & Anchor Materials

#### M_ARCursor_Default
- **Path:** `/Game/AR/Materials/M_ARCursor_Default`
- **Type:** Emissive
- **Base Color:** RGB(0.0, 0.8, 1.0) - Cyan
- **Emissive:** RGB(0.0, 0.8, 1.0) - Self-illuminated cyan
- **Roughness:** 0.1 - Smooth
- **Shading Model:** Unlit

#### M_ARAnchor_Default
- **Path:** `/Game/AR/Materials/M_ARAnchor_Default`
- **Type:** Emissive
- **Base Color:** RGB(0.0, 1.0, 0.5) - Green
- **Emissive:** RGB(0.0, 1.0, 0.5) - Self-illuminated green
- **Roughness:** 0.2 - Smooth
- **Purpose:** AR anchor visualization
- **Shading Model:** Unlit

### Overlay Materials

#### M_AROverlay_Content
- **Path:** `/Game/AR/Materials/M_AROverlay_Content`
- **Type:** Translucent
- **Base Color:** RGBA(0.0, 0.8, 1.0, 0.9) - Near-opaque cyan
- **Roughness:** 0.3 - Smooth
- **Blend Mode:** Translucent
- **Purpose:** Content overlay surfaces

#### M_AROverlay_Hologram
- **Path:** `/Game/AR/Materials/M_AROverlay_Hologram`
- **Type:** Translucent Emissive Hologram
- **Base Color:** RGB(0.0, 0.8, 1.0) - Cyan
- **Emissive:** RGB(0.0, 0.8, 1.0) - Self-illuminated cyan
- **Roughness:** 0.2 - Smooth
- **Hologram Effect:** Yes - Animated scanlines
- **Blend Mode:** Translucent
- **Shading Model:** Unlit
- **Special Effects:** Time-based sine wave scanlines

---

## Material Parameter Reference

### Common Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| BaseColor | Vector4 | Varies | Primary surface color (RGB) |
| Roughness | Scalar | Varies | Surface roughness (0-1) |
| Metallic | Scalar | Varies | Metallic property (0-1) |
| EmissiveColor | Vector4 | Varies | Self-illumination color |
| Opacity | Scalar | Varies | Transparency (0-1) |

### Special Effects

| Effect | Materials | Description |
|--------|-----------|-------------|
| Grid Texture | M_ARPlane_Floor | Procedural grid pattern using Frac and If nodes |
| Hologram Scanlines | M_AROverlay_Hologram | Animated scanlines using Time and Sine nodes |
| Emissive Glow | All emissive materials | Self-illuminated with Unlit shading model |
| Translucency | AR planes, UI panels, comfort vignette | Alpha blending with configurable opacity |

---

## Usage Instructions

### Method 1: Python Script (Recommended)

1. Open Unreal Engine 5 Editor
2. Enable Python Editor Script Plugin (Edit > Plugins)
3. Open Python Console: Window > Developer Tools > Python Console
4. Execute:
   ```python
   exec(open(r"C:\HW\MingGoRTS\Tools\build\CreateVRARMaterials.py").read())
   ```
5. Check Output Log for creation report

### Method 2: C++ Factory Class

1. Include the factory header:
   ```cpp
   #include "VRAR/MingVRARMaterialFactory.h"
   ```

2. Create and use the factory:
   ```cpp
   UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
   Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
   
   // Create all VR materials
   Factory->CreateVRMaterials();
   
   // Create all AR materials
   Factory->CreateARMaterials();
   
   // Get report
   FString Report = Factory->GetCreationReport();
   UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
   ```

### Method 3: Blueprint (via C++ Factory)

1. Create a Blueprint function library or Editor Utility Widget
2. Call the factory functions from Blueprint
3. Execute in editor context

---

## File Locations

### Configuration Files
- `Content/VR/Materials/MaterialConfigs.json` - VR material configurations
- `Content/AR/Materials/MaterialConfigs.json` - AR material configurations

### Generated Assets (After Script Execution)
- `Content/VR/Materials/M_*.uasset` - VR material assets
- `Content/AR/Materials/M_*.uasset` - AR material assets

### Source Files
- `Tools/build/CreateVRARMaterials.py` - Python generation script
- `Source/MingGoRTS/Public/VRAR/MingVRARMaterialFactory.h` - C++ header
- `Source/MingGoRTS/Private/VRAR/MingVRARMaterialFactory.cpp` - C++ implementation

---

## Color Palette Reference

### VR Color Palette
- **Primary:** RGB(0.0, 0.8, 1.0) - Cyan
- **Secondary:** RGB(0.0, 1.0, 0.5) - Green
- **Accent:** RGB(1.0, 0.8, 0.0) - Yellow/Orange
- **Valid:** RGB(0.0, 1.0, 0.0) - Green
- **Invalid:** RGB(1.0, 0.0, 0.0) - Red
- **Neutral:** RGB(0.5, 0.5, 0.5) - Gray
- **Dark:** RGB(0.1, 0.1, 0.15) - Dark Blue-Gray

### AR Color Palette
- **Primary:** RGB(0.0, 0.8, 1.0) - Cyan
- **Plane:** RGBA(0.0, 0.8, 1.0, 0.3) - Transparent Cyan
- **Anchor:** RGB(0.0, 1.0, 0.5) - Green
- **Cursor:** RGB(0.0, 0.8, 1.0) - Cyan
- **Overlay:** RGBA(0.0, 0.8, 1.0, 0.9) - Near-opaque Cyan

---

## Performance Considerations

1. **Translucent Materials:** Limit overdraw by using simple geometries
2. **Emissive Materials:** Use Unlit shading model for better performance
3. **Special Effects:** Grid and hologram effects use procedural nodes (no texture memory)
4. **Instancing:** Materials support parameter instancing for variations

---

## Next Steps

1. Run the Python script or C++ factory to generate actual .uasset files
2. Verify materials in Content Browser
3. Test materials with placeholder meshes
4. Fine-tune material parameters based on visual testing
5. Create Material Instances for variations (e.g., different colors for different controller types)

---

*Document Version: 1.0*
*Generated: 2026-03-23*
*For: MingGoRTS Epic 9.1 - VR/AR Support System*
