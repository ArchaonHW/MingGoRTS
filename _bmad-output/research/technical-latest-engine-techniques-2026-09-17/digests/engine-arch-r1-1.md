# Digest: engine-arch r1 (subagent fa799d87)

## Findings

### ECS
- Sparse-set vs archetype tradeoff quantified (CGVC 2025 C++20 comparison): sparse-set cheap add/remove, archetype fast iteration. [https://github.com/StaffsUniGames/cgvc25-ecs-comparison] (high, performance)
- Industry converged on hybrid storage: Bevy per-component Table vs SparseSet choice; flecs added sparse components. [docs.rs/bevy, ajmmertens.medium.com] (high, pattern)
- flecs FAQ: EnTT wins add/remove + single-comp queries; flecs wins multi-comp/bulk. Neither strictly faster. [flecs FAQ] (med, performance)
- flecs relationships (ChildOf pairs) + observers (OnAdd/OnRemove/OnSet) = flagship differentiators. [flecs docs] (high, pattern)
- EnTT: minimal, paged sparse-set, mixin observers; shipped in Minecraft Legends / D2R / CoD Vanguard. (high, landscape)

### Job systems
- Naughty Dog fibers: mid-job yield on atomic counters, no work stealing; prioritized ergonomics. [GDC 2015] (high, pattern)
- enkiTS: reference small production scheduler (Avoyd, "Mastering Graphics Programming"). (high, landscape)
- 2025 paper: <1000-LOC work-stealing pool comparable to Taskflow. [CSIT 2025-1-9] (med, performance)
- std::execution in C++26 draft; usable via stdexec but heavyweight for small engine. (high, version)

### Data-oriented physics
- Box2D v3: wide SIMD via greedy graph coloring of contact constraints; >2× v2.4 single-threaded. [box2d.org 2024-08] (high, pattern+performance)
- Wide-vs-narrow SIMD: work units across lanes; SAT edge-edge only SIMD-worthy at high edge counts. [box2d.org 2026-07] (high, pattern)
- Jolt CROSS_PLATFORM_DETERMINISTIC: bit-identical floats across arch/compilers, ~8% cost. [Jolt docs] (high, performance)
- RTS lockstep still fixed-point favored (Bannermen, ferrets). (med, pattern)

### Memory
- Frostbite ScopeStack linear allocator w/ ordered dtors; consoles' fixed memory. [EA] (high, pattern)
- std::pmr::monotonic_buffer_resource: bump alloc, no-op free; not thread-safe — fits single-thread. (high, pattern)
- Three-tier model (frame arena / persistent / pooled generational handles) converged. (med, pattern)

## Leads
- flecs "Building an ECS" series part 6 (sparse components)
- Box3D 2026-06 — wide-SIMD solver generalized to 3D
- DART PR #2504 — allocator benchmarks
- enkisoftware devlog — bespoke scheduler blueprint

## Not found
- Hard allocator benchmarks in a specific shipping engine
- Evidence fibers > work-stealing threads for small engines
- Sparse vs archetype numbers for this engine's workload shape

## Adoption shortlist
1. Frame/scratch arena + std::pmr — ~100 LOC, kills malloc in hot loop
2. Per-component storage choice (dense SoA vs sparse-set)
3. Small work-stealing scheduler (enkiTS or bespoke) not fibers
4. Wide-SIMD SoA + graph coloring in contact solver (only where pair counts high)
5. Fixed-tick + deterministic ordering + checksums (only if lockstep needed)
