# VR/AR Asset Style Guide for MingGoRTS

## Epic 9.1: VR/AR Support System - Asset Creation Guidelines

## Overview

This style guide defines the visual and technical standards for creating VR/AR assets in MingGoRTS. Following these guidelines ensures consistency, performance, and optimal user experience across all VR/AR platforms.

---

## Design Principles

### 1. Clarity and Readability
- Assets must be immediately recognizable at a glance
- Use high contrast colors for important interactive elements
- Avoid visual clutter that can cause eye strain in VR
- Maintain consistent visual hierarchy

### 2. Performance Optimization
- Prioritize low-poly meshes for real-time rendering
- Use efficient shader models (avoid heavy post-processing)
- Optimize texture sizes (512x512 to 1024x1024 for most assets)
- Implement LOD systems for complex assets

### 3. Comfort and Safety
- Follow VR comfort guidelines to prevent motion sickness
- Use stable reference points in the visual design
- Maintain consistent scale relationships
- Avoid rapid flashes or strobing effects

### 4. Platform Adaptability
- Design assets that work across different VR/AR devices
- Consider varying field of view requirements
- Support both hand tracking and controller-based interaction
- Account for different tracking spaces

---

## Color Palette

### Primary Colors

| Name | Hex | RGB | Usage |
|------|-----|-----|-------|
| Cyan Primary | #00CCFF | 0, 204, 255 | Main interactive elements, pointers |
| Green Secondary | #00FF88 | 0, 255, 136 | Success states, valid selections |
| Amber Accent | #FFCC00 | 255, 204, 0 | Warnings, important highlights |
| White Neutral | #FFFFFF | 255, 255, 255 | Text, UI elements |
| Dark Base | #1A1A26 | 26, 26, 38 | Backgrounds, panels |

### State Colors

| State | Color | Opacity | Usage |
|-------|-------|---------|-------|
| Valid/Success | #00FF00 | 80% | Valid teleport destinations, confirmed actions |
| Invalid/Error | #FF0000 | 80% | Invalid destinations, errors |
| Hover/Active | #00FFFF | 90% | Hover states, active selections |
| Disabled | #808080 | 50% | Disabled UI elements |
| Neutral | #808080 | 100% | Background elements |

### AR-Specific Colors

| Element | Color | Opacity | Notes |
|---------|-------|---------|-------|
| Plane Grid | #00CCFF | 30% | Semi-transparent for real-world visibility |
| Plane Boundary | #00CCFF | 80% | Solid line for clear edge definition |
| Anchor | #00FF88 | 100% | Bright green for visibility against environment |
| Cursor | #00CCFF | 100% | Match primary for consistency |
| Content Overlay | #00CCFF | 90% | Slightly transparent to blend with reality |

---

## Mesh Specifications

### VR Controller Assets

#### Controller Body
- **Poly Count**: 1,500-3,000 triangles
- **Texture Size**: 1024x1024 (diffuse, normal, roughness)
- **Dimensions**: Match real-world controller proportions
  - Meta Quest: 12cm x 5cm x 5cm
  - Valve Index: 14cm x 7cm x 6cm
  - HTC Vive: 20cm x 13cm x 10cm

#### Controller Buttons
- **Poly Count**: 100-300 triangles per button
- **Texture Size**: 256x256 shared texture atlas
- **Features**: Slightly raised profile for tactile visibility

#### Hand Meshes (Hand Tracking)
- **Poly Count**: 2,000-4,000 triangles per hand
- **Skeleton**: UE5 VR Hand Skeleton standard
- **Blend Shapes**: Support for gesture recognition
- **Material**: Single material with subsurface scattering for skin

### VR Pointer Assets

#### Laser Pointer
- **Mesh**: Cylinder, 50cm length default
- **Poly Count**: 12-24 segments
- **Material**: Emissive, additive blend
- **Width**: 0.2cm-0.5cm
- **Texture**: Procedural gradient or 64x256 texture

#### Cursor
- **Poly Count**: 100-200 triangles
- **Size**: 1cm-2cm diameter at 2m distance
- **Animation**: Subtle pulse (1.0x to 1.2x scale)
- **States**: Default, Hover, Active, Valid, Invalid

### VR UI Assets

#### Panels
- **Poly Count**: 100-500 triangles (depending on curvature)
- **Curvature**: 30-degree arc for comfortable viewing
- **Distance from Camera**: 1m-2m
- **Thickness**: 0.5cm-1cm

#### Buttons
- **Poly Count**: 50-100 triangles
- **Size**: 3cm x 1cm minimum for comfortable targeting
- **States**: Normal, Hover, Pressed, Disabled
- **Feedback**: Visual depression (2-5mm), haptic pulse

#### Radial Menu
- **Poly Count**: 200-400 triangles
- **Segment Count**: 4-12 segments
- **Radius**: 5cm-10cm
- **Height**: 0.5cm-1cm

### AR Assets

#### Plane Visualization
- **Grid Mesh**: 10x10 grid lines
- **Line Width**: 0.5cm-1cm
- **Boundary**: 2cm-3cm thick border
- **Corner Markers**: 3cm x 3cm squares

#### Anchors
- **Poly Count**: 500-1,000 triangles
- **Height**: 10cm-20cm above plane
- **Animation**: Gentle bobbing (2-5cm), slow rotation
- **Glow**: Soft bloom effect, 5-10cm radius

#### Cursor
- **Poly Count**: 200-400 triangles
- **Size**: 1cm-2cm
- **Trail**: Optional 10cm trail with fade
- **States**: Touch, Pinch, Drag, Select

---

## Material Guidelines

### VR Controller Materials

```
M_VRController_Body:
  - Base Color: Dark gray/blue (#1A1A26)
  - Roughness: 0.3 (smooth plastic feel)
  - Metallic: 0.1 (subtle reflection)
  - Normal: Fine surface texture

M_VRController_Button:
  - Base Color: Matches primary cyan
  - Roughness: 0.2
  - Emissive: 0.5 intensity when active
  - Fresnel: Subtle edge glow
```

### VR Pointer Materials

```
M_VRPointer_Laser:
  - Base Color: Cyan primary
  - Emissive: 2.0 intensity
  - Blend Mode: Additive
  - Opacity: 80%
  - Fresnel: Strong for volumetric look

M_VRPointer_Cursor:
  - Base Color: Green secondary
  - Emissive: 1.5 intensity
  - Roughness: 0.0
  - Two-sided: Enabled
```

### AR Plane Materials

```
M_ARPlane_Grid:
  - Base Color: Transparent cyan
  - Opacity: 30%
  - Blend Mode: Translucent
  - Grid Texture: Procedural or 256x256
  - Fade Distance: 5m from camera

M_ARPlane_Boundary:
  - Base Color: Solid cyan
  - Emissive: 0.8 intensity
  - Opacity: 80%
  - Animation: Slow pulse (optional)
```

---

## Animation Specifications

### VR Controller Animations

#### Trigger Animation
- **Range**: 15-20 degrees rotation
- **Speed**: Instant response (target matching)
- **Easing**: None (direct mapping)

#### Grip Animation
- **Range**: 10-15 degrees rotation
- **Speed**: Instant response
- **Haptic**: 0.1s pulse at full grip

#### Button Press
- **Depth**: 2-5mm depression
- **Speed**: 50ms down, 100ms up
- **Easing**: Ease-out for release

### Pointer Animations

#### Cursor Pulse
- **Scale**: 1.0x to 1.2x
- **Speed**: 2 seconds per cycle
- **Easing**: Sine wave (smooth)

#### Trail Effect
- **Length**: 10-20cm
- **Fade**: Linear over 0.5 seconds
- **Width**: Tapers from 0.5cm to 0cm

### AR Animations

#### Anchor Bobbing
- **Amplitude**: 2-5cm vertical
- **Speed**: 1-2 seconds per cycle
- **Easing**: Sine wave

#### Plane Pulse
- **Opacity**: 30% to 50%
- **Speed**: 2-3 seconds per cycle
- **Easing**: Sine wave

---

## Audio Specifications

### UI Sounds

| Sound | Frequency | Duration | Volume | Haptic |
|-------|-----------|----------|--------|--------|
| Hover | 800Hz | 50ms | -12dB | 10ms @ 0.3 |
| Select | 1000Hz | 100ms | -10dB | 50ms @ 0.5 |
| Error | 200Hz | 200ms | -8dB | 100ms @ 0.7 |
| Success | 1200Hz | 150ms | -10dB | 50ms @ 0.5 |
| Teleport | 500Hz → 0Hz | 300ms | -8dB | 200ms @ 0.6 |

### Spatial Audio
- **UI Sounds**: 2D (head-locked)
- **World Sounds**: 3D spatialized
- **Attenuation**: Logarithmic, 1m-10m range
- **Reverb**: Disabled for UI, enabled for world

---

## Particle Effects

### VR Effects

#### Selection Burst
- **Particles**: 20-50
- **Lifetime**: 0.5-1.0 seconds
- **Size**: 0.5cm-2cm
- **Velocity**: 10-50cm/s outward
- **Color**: Match interaction color

#### Teleport Effect
- **Start**: Implosion (particles gather)
- **End**: Explosion (particles disperse)
- **Duration**: 0.3-0.5 seconds
- **Count**: 100-200 particles

### AR Effects

#### Plane Detection
- **Particles**: Floating dust-like
- **Count**: 50-100
- **Lifetime**: 2-5 seconds
- **Movement**: Slow drift upward
- **Trigger**: On plane detection

#### Anchor Placement
- **Style**: Ring expansion
- **Rings**: 3-5 expanding circles
- **Speed**: Fast expansion, slow fade
- **Color**: Cyan to transparent

---

## Performance Targets

### VR Performance (72-120 FPS)

| Asset Type | Max Draw Calls | Max Tris/Frame | Texture Memory |
|------------|----------------|----------------|----------------|
| Controllers | 2 per controller | 5,000 | 4MB |
| Pointers | 3 | 500 | 512KB |
| UI Panels | 5 | 2,000 | 2MB |
| Full Scene | 50 | 100,000 | 50MB |

### AR Performance (60 FPS)

| Asset Type | Max Draw Calls | Max Tris/Frame | Texture Memory |
|------------|----------------|----------------|----------------|
| Planes | 3 per plane | 1,000 | 1MB |
| Anchors | 2 per anchor | 1,000 | 512KB |
| Content | 10 | 10,000 | 10MB |
| Full Scene | 30 | 50,000 | 30MB |

---

## Platform-Specific Guidelines

### Meta Quest
- **Controller Models**: Use Touch Pro controllers
- **Hand Tracking**: Full support
- **Optimization**: Aggressive LOD for standalone
- **Textures**: Max 1024x1024

### Valve Index
- **Controller Models**: Use Index controllers with finger tracking
- **Finger Animations**: Individual finger support
- **Grip Variations**: Support full grip range
- **Haptics**: High-fidelity feedback

### PlayStation VR
- **Controller Models**: DualSense controllers
- **Tracking**: Support both inside-out and outside-in
- **Optimization**: Balance quality/performance
- **Colors**: Slightly more saturated for OLED displays

### HoloLens
- **UI Distance**: 1-2m optimal
- **Colors**: High contrast for see-through display
- **Bloom**: Minimize (can cause eye strain)
- **Anchors**: Must be highly visible against real world

---

## Naming Conventions

### Mesh Assets
```
SM_VR[AssetType]_[Name]_[Variant]
SM_VRController_Body_Oculus
SM_VRPointer_Laser_Default
SM_ARPlane_Grid_Floor
```

### Material Assets
```
M_VR[AssetType]_[Name]_[State]
M_VRController_Button_Active
M_VRPointer_Cursor_Valid
M_ARPlane_Boundary_Default
```

### Texture Assets
```
T_VR[AssetType]_[Name]_[Type]
T_VRController_Body_Diffuse
T_VRUI_Panel_Normal
T_ARPlane_Grid_Mask
```

### Blueprint Assets
```
ABP_VR[AssetType]_[Name]
WBP_VR[UIType]_[Name]
ABP_VRController_Oculus
WBP_VRMainMenu_Default
```

### Sound Assets
```
S_VR[AssetType]_[Event]
S_VRController_Hover
S_VRTeleport_Start
S_ARPlane_Detected
```

### Particle Assets
```
PS_VR[AssetType]_[Event]
PS_VRController_Select
PS_VRTeleport_End
PS_ARAnchor_Placed
```

---

## File Organization

```
Content/
├── VR/
│   ├── Controllers/
│   │   ├── SM_VRController_*.uasset
│   │   └── ABP_VRController.uasset
│   ├── Hands/
│   │   ├── SK_VRHand_*.uasset
│   │   └── ABP_VRHand.uasset
│   ├── Pointers/
│   │   ├── SM_VRPointer_*.uasset
│   │   └── PS_VRPointer_*.uasset
│   ├── UI/
│   │   ├── SM_VRUI_*.uasset
│   │   └── WBP_VR*.uasset
│   ├── Materials/
│   │   └── M_VR*.uasset
│   ├── Effects/
│   │   └── PS_VR*.uasset
│   ├── Sounds/
│   │   └── S_VR*.uasset
│   └── Teleport/
│       ├── SM_VRTeleport_*.uasset
│       └── PS_VRTeleport_*.uasset
├── AR/
│   ├── Cursors/
│   │   └── SM_ARCursor_*.uasset
│   ├── Planes/
│   │   ├── SM_ARPlane_*.uasset
│   │   └── PS_ARPlane_*.uasset
│   ├── Anchors/
│   │   ├── SM_ARAnchor_*.uasset
│   │   └── PS_ARAnchor_*.uasset
│   ├── Overlays/
│   │   └── SM_AROverlay_*.uasset
│   ├── Materials/
│   │   └── M_AR*.uasset
│   ├── Effects/
│   │   └── PS_AR*.uasset
│   └── Sounds/
│       └── S_AR*.uasset
```

---

## Quality Checklist

Before finalizing any VR/AR asset, verify:

- [ ] Mesh is optimized (appropriate poly count)
- [ ] UVs are properly unwrapped (no stretching)
- [ ] Textures are power-of-2 dimensions
- [ ] Materials use correct shader models
- [ ] Colors match style guide palette
- [ ] Animations are smooth and performant
- [ ] Audio is spatialized correctly (if applicable)
- [ ] Particles don't impact performance
- [ ] Asset works at target frame rate
- [ ] Asset is tested in VR/AR preview
- [ ] Naming follows convention
- [ ] File is saved in correct directory

---

## Version History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2026-03-23 | MingGoRTS Team | Initial style guide |

---

## References

- Unreal Engine VR Best Practices
- Meta Design Guidelines for VR
- Valve Index Interaction Guidelines
- Microsoft HoloLens Design Guidelines
- Apple Vision Pro Human Interface Guidelines
