# Digest: rendering r1 (subagent 133b55e8)

## Findings
- GPU-driven pipeline (compute cull + Hi-Z + LOD + compaction → MDI) fully practical on GL 4.3–4.6 core; NV_command_list NV-only but compute+MDI portable. [NVIDIA GTC 2015] (high, pattern)
- azdo repo: GL reference impl — Hi-Z occlusion, temporal two-phase, LOD atomics, single MDI. [github.com/potato3d/azdo] (high, pattern)
- GL 4.6 voxel renderer: 1 MDI/frame, ~181µs cull dispatch, ~6ms GPU frame. Caveat (imgtec): GL works but optimal on VK/DX12 (explicit barriers). (high, performance)
- ARB_bindless_texture: never core; NV/AMD discrete OK. Without NV_gpu_shader5 sampler handles must be dynamically uniform — gl_DrawID flat-varying preserves this (AMD bug fixed 21.12.1). (high, pattern)
- Clustered shading: compute grid-build + light-cull, minimal intrusion into forward, scales to 1000s lights; Godot 4's primary renderer is clustered forward. [HPG 2012, DaveH355, Godot docs] (high, pattern+landscape)
- Frame graph: adopt pass-declaration + transient-aliasing half; barrier compiler pointless on GL. [Babylon.js docs] (med, pattern)
- FSR 3.1: GLSL 4.50 shader target (GL_EXT_samplerless_texture_functions, EXT_shader_image_load_formatted); official API backends DX12+VK only → custom backend needed. FSR4=RDNA4+DX12 binary; XeSS no GL. FSR2 community GL port exists. (high, version)
- Meshlets w/o mesh shaders: GL_NV_mesh_shader NV-only; portable path = meshoptimizer offline build + compute cull + one indirect draw per meshlet. (high, pattern)
- Visibility buffer spreading (The Forge "Forward++", I3D 2026); counter-evidence: Cinevva 2026 vis-buffer NOT faster than forward — benchmark first. (high landscape / med performance)
- ARB_sparse_texture/2: partial VT backport; residency feedback only, engine does the rest; high effort. (med, pattern)

## Adoption shortlist
1. GPU-driven culling + MDI (GL 4.3 core) — natural evolution of existing frustum culling; compute-shader on-ramp
2. Clustered forward lighting — canonical forward upgrade
3. Bindless textures — modest change, unlocks per-draw materials in MDI
4. FSR 3.1 custom backend or FSR2 GL port — only viable upscaler path (needs motion vectors + jitter)
5. Lightweight render graph — pass declaration + transient pooling only
6. Defer: meshlets, vis-buffer, sparse VT — benchmark first

## Leads
- niagara (zeux) — reference GPU culling arch; azdo — copyable GL impl; FidelityFX-FSR2-OpenGL; FSR3Unity
- Contradictions: GL compute optimal-but-less-predictable vs VK/DX12; Cinevva vis-buffer counter-example
- Unverified: Intel GPU bindless support; MDI count-ext coverage per vendor

## Not found
- Work graphs on GL (D3D12-only); C++ GL-native render-graph shipping example; XeSS GL statement
