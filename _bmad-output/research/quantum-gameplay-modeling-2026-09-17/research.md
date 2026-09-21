---
title: 'technical research: quantum gameplay modeling'
type: 'technical'
topic: 'quantum-techniques-for-game-screen-modeling'
decision: 'how quantum computing formalism maps onto MingGoRTS gameplay/visual systems'
source: 'run'
status: 'done'
preset: 'standard'
created: '2026-09-17'
updated: '2026-09-17'
---

# technical research: quantum gameplay modeling

**Decision this research serves:** whether and how quantum technology concepts should be adopted into Potato Engine — as simulation infrastructure, gameplay mechanics, and screen presentation.

## Executive summary

Adopted: a **C++ statevector quantum simulator** (`Quantum/QubitRegister`, `Quantum/Qudit`) powering a **superposition fog-of-war** (`Gameplay/QuantumFog`) where intel points act as measurement operators, plus an ImGui probability-cloud visualization (`QuantumFogDemo`). Real QPU hardware access is **rejected** — latency, availability, and zero gameplay benefit versus exact classical simulation.

## Findings

### 1. What "quantum" honestly means for this engine

| Layer | Reality | Decision |
|---|---|---|
| Quantum hardware (QPU/cloud) | ms–s latency per shot, queue-based, needs network+auth | **Reject** — incompatible with 60fps gameplay; no algorithmic win at game scale |
| Statevector simulation | Exact linear algebra on 2^n amplitudes; n≤8 trivially cheap | **Adopted** — `QubitRegister` implements H/X/Y/Z/Phase/Ry/CNOT/SWAP + Born-rule measurement |
| Qudit (d-dim state) | Candidate spaces need not be powers of two | **Adopted** — `Qudit` models N-position superposition directly |
| Quantum randomness | Simulated measurement still needs classical dice underneath | Hybrid — Born rule is real math; underlying `mt19937_64` seeds the dice. True QRNG (hardware/API) deferred as a `QuantumBitSource` backend seam |

### 2. The mapping that works: superposition fog-of-war

The doctrine-battle design already runs on information asymmetry (情報點 intel economy, 道權觀察 theme). Quantum formalism gives it a precise, already-implemented model:

- **Enemy squad = superposition** over K candidate cells with personality-weighted prior amplitudes (`|a_i|²` = probability).
- **Intel spend = measurement**: `Observe()` spends intel via `BattleResources.SpendIntel`, collapses state to the candidate nearest the true position.
- **Intel expiry = re-superposition**: after `intelDuration`, the entity returns to superposition with its prior distribution — "情報時效" as reverse decoherence.
- **Unobserved drift = decoherence**: `Diffuse()` slowly relaxes probabilities toward uniform while unobserved.

### 3. Screen presentation

`QuantumFogDemo` renders the superposition directly: each candidate cell gets a translucent red disc whose alpha and radius scale with `|a_i|²`; collapse renders a solid marker with a decaying intel-timer ring; a god-view toggle exposes ground truth for debugging. This is the correct level of "quantum visuals" — it visualizes the *mechanic*, not decorative quantum-themed particles.

### 4. What was deliberately not built

- **Quantum pathfinding/QAOA**: no benefit over the existing flow-field (reverse Dijkstra is already O(V+E) and optimal for the grid).
- **Interference mechanics**: amplitude phases exist in `QubitRegister` but no gameplay rule consumes them yet — a doctrine-combo mechanic could exploit constructive/destructive interference later.
- **Real QRNG backend**: `QuantumBitSource` is the seam; plugging a hardware QRNG API is a 20-line change if ever justified.

### 5. Limits

- State space is exponential in qubits — the sim is capped at 16 qubits (65536 amplitudes); per-entity fog uses qudits of dimension = candidate count (typically 3–6).
- Simulated measurement uses seeded `mt19937_64` — deterministic given a seed (good for replays, means it is NOT true quantum randomness).
- Multi-entity correlations (entangled squads — "測 A 即知 B") are supported by `QubitRegister::CNOT` but not yet exposed through `QuantumFog`.

## Artifacts

- `Quantum/QubitRegister.{h,cpp}` — statevector simulator
- `Quantum/Qudit.{h,cpp}` — d-dimensional quantum state
- `Gameplay/QuantumFog.{h,cpp}` — superposition fog-of-war over `BattleResources` intel
- `Examples/QuantumTest.cpp` — headless verification (CTest #25)
- `Examples/QuantumFogDemo.cpp` — ImGui probability-cloud visualization
