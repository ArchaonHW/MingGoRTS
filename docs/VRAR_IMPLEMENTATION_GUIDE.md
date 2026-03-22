# Epic 9.1: VR/AR Support System

## System Overview

Epic 9.1 introduces comprehensive Virtual Reality (VR) and Augmented Reality (AR) support for MingGoRTS, enabling immersive RTS gameplay across multiple VR/AR platforms.

## Core Components

### 1. VR Support System (`MingRTSVRSupport`)
**Files**: `MingRTSVRSupport.h/cpp`

**Features**:
- **7 VR Device Types**: Oculus, SteamVR (Index/Vive), PlayStation VR, Windows Mixed Reality, Meta Quest, Pico, Apple Vision Pro
- **4 Tracking Spaces**: Stationary, Room Scale, Local Floor, Local
- **Comfort Settings**: Teleport movement, vignette effects, snap turning, height adjustment
- **UI Adaptation**: World-space UI panels, gaze interaction, gaze dwell selection
- **Device Management**: Automatic device detection, enable/disable VR, tracking space configuration

**Key APIs**:
```cpp
// Initialize VR support
UMingRTSVRSupport* VRSupport = GEngine->GetEngineSubsystem<UMingRTSVRSupport>();
VRSupport->InitializeVRSupport();

// Enable VR mode
VRSupport->EnableVR();

// Set comfort settings
FVRComfortSettings Comfort;
Comfort.MovementMode = EVRMovementMode::Teleport;
Comfort.bVignetteDuringMovement = true;
VRSupport->SetComfortSettings(Comfort);
```

### 2. VR Controller (`MingRTSVRController`)
**Files**: `MingRTSVRController.h/cpp`

**Features**:
- **2 Controller Types**: Left Hand, Right Hand
- **3 Interaction Modes**: Pointer, Direct Touch, Grab
- **RTS-Specific Commands**: Unit selection, unit movement, command attack, open radial menu
- **Input Handling**: Trigger, grip, thumbstick, button inputs
- **Haptic Feedback**: Intensity and duration control

**Key APIs**:
```cpp
// Initialize controller
UMingRTSVRController* Controller = NewObject<UMingRTSVRController>(Owner);
Controller->InitializeController(EVRControllerType::RightHand);

// Select unit at pointer
Controller->SelectUnitAtPointer();

// Move selected units
Controller->MoveSelectedUnits();

// Play haptic feedback
Controller->PlayHapticEffect(0.5f, 0.1f);
```

### 3. AR Support System (`MingRTSARSupport`)
**Files**: `MingRTSARSupport.h/cpp`

**Features**:
- **4 AR Device Types**: ARCore (Android), ARKit (iOS), Microsoft HoloLens, Magic Leap
- **Plane Detection**: Horizontal, vertical, both orientations
- **Content Overlay**: Game world, UI, minimap, unit info, building info overlays
- **Light Estimation**: Environment color and intensity capture
- **Session Management**: Start, stop, pause, resume AR sessions

**Key APIs**:
```cpp
// Initialize AR support
UMingRTSARSupport* ARSupport = GEngine->GetEngineSubsystem<UMingRTSARSupport>();
ARSupport->InitializeARSupport();

// Start AR session
FARSessionConfig Config;
Config.PlaneDetectionMode = EARPlaneDetectionMode::Both;
Config.bEnableLightEstimation = true;
ARSupport->StartARSession(Config);

// Spawn game world on detected plane
TArray<FARTrackedPlane> Planes = ARSupport->GetDetectedPlanes();
if (Planes.Num() > 0)
{
    ARSupport->SpawnGameWorldOnPlane(Planes[0], FVector::ZeroVector);
}
```

### 4. AR Controller (`MingRTSARController`)
**Files**: `MingRTSARController.h/cpp`

**Features**:
- **10 Gesture Types**: Tap, double tap, long press, swipe, pinch, pan, rotate
- **6 Interaction Modes**: Touch, air tap, hand gesture, controller, voice, eye tracking
- **Virtual Object Management**: Register, track, interact with virtual objects
- **Camera Control**: Pan, zoom, rotate for RTS camera
- **Gesture Recognition**: Real-time gesture detection and handling

**Key APIs**:
```cpp
// Initialize AR controller
UMingRTSARController* ARController = NewObject<UMingRTSARController>(Owner);
ARController->InitializeController();

// Process touch input
ARController->ProcessTouchInput(ScreenPosition, bIsPressed);

// Select unit at touch
ARController->SelectUnitAtTouch(ScreenPosition);

// Move selected units
ARController->MoveSelectedUnits(ScreenPosition);
```

### 5. Performance Optimizer (`MingRTSVARPerformanceOptimizer`)
**Files**: `MingRTSVARPerformanceOptimizer.h/cpp`

**Features**:
- **4 Quality Levels**: Low, Medium, High, Ultra
- **5 Frame Rate Targets**: 30, 60, 72, 90, 120 FPS
- **Adaptive Quality**: Automatic quality adjustment based on performance
- **VR-Specific**: Foveated rendering, ASW (Asynchronous Spacewarp), motion smoothing
- **AR-Specific**: Camera resolution control, tracking frequency optimization
- **Performance Monitoring**: Real-time FPS, frame time, memory usage tracking

**Key APIs**:
```cpp
// Initialize optimizer
UMingRTSVARPerformanceOptimizer* Optimizer = GEngine->GetEngineSubsystem<UMingRTSVARPerformanceOptimizer>();
Optimizer->InitializeOptimizer();

// Apply performance preset
Optimizer->ApplyPCVRPreset(); // or MobileVRPreset(), HoloLensPreset(), etc.

// Enable adaptive quality
Optimizer->EnableAdaptiveQuality(true);

// Get performance metrics
FVARPerformanceMetrics Metrics = Optimizer->GetCurrentMetrics();
```

### 6. Asset Manager (`MingRTSVARAssetManager`)
**Files**: `MingRTSVARAssetManager.h/cpp`

**Features**:
- **14 Asset Types**: VR controllers, hands, pointers, AR cursors, plane materials, etc.
- **Asset Pooling**: Efficient reuse of frequently used assets
- **Memory Management**: Budget control, automatic unloading
- **Platform-Specific Assets**: Quest, PSVR, SteamVR, ARCore, ARKit assets
- **Async Loading**: Non-blocking asset loading

**Key APIs**:
```cpp
// Initialize asset manager
UMingRTSVARAssetManager* AssetManager = GEngine->GetEngineSubsystem<UMingRTSVARAssetManager>();
AssetManager->InitializeAssetManager();

// Load asset
UObject* ControllerMesh = AssetManager->LoadAsset(TEXT("VR_RightController"));

// Get from pool
UObject* Pointer = AssetManager->AcquireAssetFromPool(EVARAssetType::VRPointer);

// Set memory budget
AssetManager->SetMemoryBudget(512); // 512 MB
```

### 7. Test Suite (`MingRTSVARSystemTest`)
**Files**: `MingRTSVARSystemTest.h/cpp`

**Features**:
- **11 Test Categories**: Initialization, VR/AR device detection, controllers, tracking, interaction, performance, assets, integration, stress tests
- **30+ Test Cases**: Comprehensive coverage of all VR/AR functionality
- **Automated Testing**: Run all tests or by category
- **Performance Metrics**: Track execution time, pass rates
- **Report Generation**: Export test reports to file

**Key APIs**:
```cpp
// Initialize test suite
UMingRTSVARSystemTest* TestSuite = NewObject<UMingRTSVARSystemTest>();
TestSuite->InitializeTestSuite();

// Run all tests
TestSuite->RunAllTests();

// Run specific category
TestSuite->RunTestsByCategory(EVARTestCategory::VRController);

// Get results
FVARTestSuiteSummary Summary = TestSuite->GetTestSummary();
```

## Configuration

Configuration is managed through `Config/DefaultVRAR.ini`:

```ini
[VR.General]
bEnableVR=true
DefaultVRDevice=Oculus
VRTargetFrameRate=72
bEnableComfortVignette=true

[AR.General]
bEnableAR=true
DefaultARPlatform=ARCore
bEnablePlaneDetection=true
PlaneDetectionMode=Both

[Performance.General]
PerformanceLevel=High
bEnableAdaptiveQuality=true
TargetFrameRate=72

[Assets.General]
MemoryBudgetMB=512
bPreloadAssets=true
```

## Platform Support

### VR Platforms
- **Meta Quest 2/3/Pro**: Full support with hand tracking
- **SteamVR (Index/Vive)**: Full support with room-scale
- **PlayStation VR/PSVR2**: Full support with DualSense integration
- **Windows Mixed Reality**: Full support
- **Pico**: Full support
- **Apple Vision Pro**: Basic support (spatial computing)

### AR Platforms
- **Android (ARCore)**: Full support with depth API
- **iOS (ARKit)**: Full support with LiDAR and people occlusion
- **Microsoft HoloLens 2**: Full support with spatial mapping
- **Magic Leap 2**: Basic support

## Integration with RTS Systems

### Unit Selection (VR)
```cpp
// Left controller trigger selects units
Controller->OnUnitSelected.AddDynamic(this, &AMyGameMode::OnUnitSelectedVR);

void AMyGameMode::OnUnitSelectedVR(AActor* SelectedUnit)
{
    // Add to selection
    SelectedUnits.Add(SelectedUnit);
}
```

### Unit Movement (VR)
```cpp
// Right controller trigger moves selected units
Controller->OnUnitsMoved.AddDynamic(this, &AMyGameMode::OnUnitsMovedVR);

void AMyGameMode::OnUnitsMovedVR(const FVector& Location, const TArray<AActor*>& Units)
{
    for (AActor* Unit : Units)
    {
        // Move unit to location
        Unit->MoveToLocation(Location);
    }
}
```

### AR Plane Interaction
```cpp
// Place buildings on detected AR planes
ARSupport->OnARPlaneDetected.AddDynamic(this, &AMyGameMode::OnPlaneDetected);

void AMyGameMode::OnPlaneDetected(const FARTrackedPlane& Plane)
{
    // Show building placement preview on plane
    ShowBuildingPreview(Plane);
}
```

## Performance Guidelines

### VR Performance Targets
- **Mobile VR (Quest)**: 72 FPS minimum, 72Hz refresh
- **PC VR**: 90 FPS minimum, 90Hz refresh
- **High-end PC VR**: 120 FPS target, 120Hz refresh

### AR Performance Targets
- **Mobile AR**: 60 FPS minimum
- **HoloLens**: 60 FPS minimum with spatial mapping

### Optimization Tips
1. Use adaptive quality for consistent frame rates
2. Enable foveated rendering on supported devices
3. Use LOD system for distant units
4. Enable occlusion culling
5. Monitor memory usage with asset manager

## Testing

### Running Tests
```cpp
// In editor or runtime
UMingRTSVARSystemTest* TestSuite = NewObject<UMingRTSVARSystemTest>();
TestSuite->InitializeTestSuite();
TestSuite->RunAllTests();
```

### Test Categories
1. **Initialization**: System startup tests
2. **VR Device Detection**: HMD detection and connection
3. **AR Device Detection**: AR platform detection
4. **VR Controller**: Controller tracking and input
5. **AR Controller**: Touch and gesture recognition
6. **Tracking**: VR/AR tracking quality
7. **Interaction**: RTS-specific interactions
8. **Performance**: Frame rate and memory tests
9. **Assets**: Asset loading and pooling
10. **Integration**: Full system integration
11. **Stress Tests**: Long duration and heavy load tests

## Troubleshooting

### Common Issues

**VR not initializing**
- Check VR headset is connected and powered on
- Verify VR runtime (Oculus, SteamVR) is running
- Check `bEnableVR=true` in config

**Tracking issues**
- Ensure proper lighting for tracking
- Clean tracking sensors/cameras
- Reset tracking space if needed

**Performance issues**
- Reduce quality level in config
- Enable adaptive quality
- Check memory budget settings

**AR not detecting planes**
- Ensure sufficient lighting
- Point device at textured surfaces
- Move device slowly for initialization

## Future Enhancements

### Planned Features
- Hand tracking for all VR controllers
- Eye tracking integration
- Multiplayer VR/AR support
- Haptic gloves support
- Brain-computer interface (research)

### Research Areas
- Neural network-based gesture recognition
- Predictive tracking algorithms
- Cloud-based AR persistence
- Cross-platform VR/AR networking

## API Reference

See header files for complete API documentation:
- `Public/VRAR/MingRTSVRSupport.h`
- `Public/VRAR/MingRTSVRController.h`
- `Public/VRAR/MingRTSARSupport.h`
- `Public/VRAR/MingRTSARController.h`
- `Public/VRAR/MingRTSVARPerformanceOptimizer.h`
- `Public/VRAR/MingRTSVARAssetManager.h`
- `Public/VRAR/MingRTSVARSystemTest.h`

## Technical Support

For technical issues or questions:
1. Check configuration in `DefaultVRAR.ini`
2. Review logs for error messages
3. Run test suite to identify issues
4. Verify platform SDK versions
5. Check device compatibility list

## Credits

Epic 9.1 VR/AR Support System developed for MingGoRTS.

---

**Version**: 1.0.0  
**Last Updated**: 2026-03-23  
**Status**: Complete
