---
title: 'Render-to-Training-Data Pipeline (first verifiable path)'
type: 'feature'
created: '2026-09-17'
status: 'done'
baseline_commit: '0c61fbd6261fe0c3ee3dcf6da882b5b5a7c3448a'
context:
  - 'AGENTS.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** PotatoEngine can render 2D/3D scenes and has an in-tree MLP (`AI::NeuralNetwork`), but there is no path connecting them: rendering cannot produce labelled data, so nothing in the engine can be trained from its own output.

**Approach:** Add one verifiable end-to-end pipeline: a new example executable `SynthDataDemo` that (1) offscreen-renders a procedurally generated labelled scene — a colored 3D cube placed on a 3×3 grid under an orthographic camera — (2) captures RGBA pixels via FBO + `glReadPixels`, writes a dataset (PNG images + `labels.csv` + `dataset.json` manifest) to `output/synth_dataset/` relative to CWD, and (3) feeds the captured frames into `AI::NeuralNetwork` (256→48→9, sigmoid output, MSE loss) to prove a network can learn the label from rendered pixels. Deterministic seeded RNG makes the dataset reproducible.

## Boundaries & Constraints

**Always:**
- Headless-safe: if `glfwInit`/`glfwCreateWindow`/`gladLoadGLLoader` fails, print `[SKIP]` and exit 0 (GLSmokeTest convention).
- Reuse existing facilities: hidden GLFW window + FBO pattern from `Examples/PortraitRenderer.cpp`, `ImageCodec::WritePNGFile`, `AI::NeuralNetwork`, engine `Mesh`/`AdvancedShader`, `Matrix4::Orthographic`/`LookAt`.
- Deterministic: `std::mt19937` with fixed seed written into `dataset.json`; same seed must produce identical labels.
- Safe file handling: `std::filesystem::create_directories`, no banned C functions (`sprintf`/`scanf`/etc.), output confined under `output/synth_dataset/`.
- Register in CMake: new target links `PotatoEngine glad glfw`; add to `POTATO_TESTS` so ctest picks it up. Must compile under MSVC, MinGW, and Linux g++.

**Ask First:**
- Adding a new dependency or dataset format library.
- Writing outside `output/` or committing generated dataset files to git.

**Never:**
- No generalized dataset framework, no depth/segmentation passes, no training-loop framework — single task only (9-class grid position).
- No changes to `external/` or `build/`.
- Do not break existing tests or modify `NeuralNetwork` internals to make the demo pass.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| HAPPY_PATH | `SynthDataDemo` run with GL available | Prints dataset stats; writes 180 PNGs + labels.csv + dataset.json under `output/synth_dataset/`; trains NN; prints train/test accuracy; exits 0 | N/A |
| NO_DISPLAY | No GL context available | Prints `[SKIP]`, exits 0 | Graceful skip |
| OUTPUT_DIR_UNWRITABLE | CWD not writable | Prints error, exits non-zero | Return 1 |
| NN_UNDERFIT | Training completes but test accuracy < 0.80 | Prints `[FAIL]`, exits non-zero | Return 1 |

</frozen-after-approval>

## Code Map

- `Examples/PortraitRenderer.cpp` — reference: hidden GLFW window, `CreateFBO` (RGBA8 + DEPTH24_STENCIL8), `glReadPixels`, row-flip, `WritePNGFile`.
- `Rendering/OpenGLRenderer.h` — `Mesh`/`Vertex` (`SetVertices`/`SetIndices`/`Draw`), attribute layout loc 0=pos, 1=normal, 2=uv.
- `Rendering/Shader.h` — `AdvancedShader::LoadFromSource`, `SetMat4`, `SetVec3`.
- `Rendering/ImageCodec.h` — `ImageCodec::WritePNGFile(path,w,h,rgba,err)`.
- `AI/NeuralNetwork.h/.cpp` — `AddLayer`, `Build`, `Train`, `PredictClass`; activations: sigmoid/relu/tanh/leaky_relu only (no softmax/linear); losses: "mse", "cross_entropy". Note: `Build()` assigns `layerActivations[i]` to the layer mapping `size[i]→size[i+1]`, so `AddLayer(256,"relu"), AddLayer(48,"sigmoid"), AddLayer(9)` yields hidden=relu, output=sigmoid.
- `MathUtils/Matrix4.h` — `Orthographic`, `LookAt`, `Translate`/`Scale` (check exact names while implementing).
- `Examples/GLSmokeTest.cpp` — headless `[SKIP]` convention.
- `CMakeLists.txt` (~line 517–525, 563–588) — target + `POTATO_TESTS` registration.

## Tasks & Acceptance

**Execution:**
- [x] `Examples/SynthDataDemo.cpp` -- NEW: implement pipeline described in Intent (scene gen → FBO capture → dataset export → NN train/eval) -- sole deliverable.
- [x] `CMakeLists.txt` -- add `SynthDataDemo` target (mirror PortraitRenderer block) and append to `POTATO_TESTS` -- ctest registration.
- [x] `.gitignore` -- add `output/synth_dataset/` if not already covered by an `output/` rule -- generated artifacts must not be committed.

**Acceptance Criteria:**
- Given a GL-capable machine, when `SynthDataDemo` runs, then it writes a 180-sample dataset (PNG per sample, labels.csv, dataset.json with seed/dims/class names) and the held-out test accuracy is ≥ 0.80.
- Given a headless environment, when `SynthDataDemo` runs, then it prints `[SKIP]` and exits 0 without failing ctest.
- Given MSVC and MinGW builds, when compiled, then `SynthDataDemo` builds with no errors and ctest still passes for all tests.

## Design Notes

- Label = grid cell index 0–8 (one-hot). Sample i uses cell `i % 9` with seeded jitter: in-cell position offset, yaw rotation, slight color variation — enough noise that the network must generalize, not memorize.
- Frame pipeline: render 64×64 RGBA → downsample to 16×16 grayscale → 256 float inputs normalized [0,1]. Small enough for the in-tree MLP.
- Split 144 train / 36 test (stratified per class: 16/4), 200 epochs, lr 0.15 — sigmoid+MSE saturates early, needs the longer schedule to escape the ~0.1 loss plateau (80 epochs only reached 77.8%).
- The ortho-camera cube doubles as the 2D-path proof: same code path renders flat quads by swapping mesh/vertices; keep the sample generator data-driven so a future depth/segmentation pass plugs in at the capture step.

## Verification

**Commands:**
- `cmake --build build --config Debug --target SynthDataDemo` -- expected: 0 errors (MSVC and MinGW toolchains).
- `build/bin/Debug/SynthDataDemo.exe` -- expected: dataset stats printed, `[PASS]` training summary, exit 0.
- `cd build && ctest -C Debug -R SynthDataDemo` -- expected: pass.
- `cd build && ctest -C Debug` -- expected: all tests pass (no regressions).

**Manual checks:**
- Open a few PNGs under `output/synth_dataset/` — cube visibly sits in the labelled grid cell.

## Suggested Review Order

- Pipeline entry point and overall flow (header comment describes all 5 stages).
  [`SynthDataDemo.cpp:1`](../../Examples/SynthDataDemo.cpp#L1)
- Main orchestration: context → dataset gen → split → train → eval gate.
  [`SynthDataDemo.cpp:182`](../../Examples/SynthDataDemo.cpp#L182)

**Scene generation & capture**

- Cube geometry — winding fixed so outward faces rasterize under cull-face.
  [`SynthDataDemo.cpp:83`](../../Examples/SynthDataDemo.cpp#L83)
- FBO + ortho top-down camera setup.
  [`SynthDataDemo.cpp:124`](../../Examples/SynthDataDemo.cpp#L124)
- Sample loop: seeded jitter → render → readback+flip → PNG + labels.csv.
  [`SynthDataDemo.cpp:251`](../../Examples/SynthDataDemo.cpp#L251)
- Manifest write (seed, dims, class_names) with open-failure check.
  [`SynthDataDemo.cpp:330`](../../Examples/SynthDataDemo.cpp#L330)

**Training & evaluation**

- 64×64 RGBA → 16×16 grayscale block-average feature extraction.
  [`SynthDataDemo.cpp:160`](../../Examples/SynthDataDemo.cpp#L160)
- Deterministic Xavier init overrides random_device-seeded weights.
  [`SynthDataDemo.cpp:390`](../../Examples/SynthDataDemo.cpp#L390)
- Stratified 144/36 split and the ≥80% accuracy PASS/FAIL gate.
  [`SynthDataDemo.cpp:352`](../../Examples/SynthDataDemo.cpp#L352)

**Peripherals**

- Build fix: missing `<glad/glad.h>` + `<cstddef>` in untracked engine file.
  [`SkinnedMesh.cpp:1`](../../Rendering/SkinnedMesh.cpp#L1)
- Target + ctest registration.
  [`CMakeLists.txt:560`](../../CMakeLists.txt#L560)
- Generated-dataset ignore rule.
  [`.gitignore:130`](../../.gitignore#L130)
