# Digest: anti-cheat r1 (subagent 967dfa7b)

## Findings — evasion
- Indirect syscalls + return-address/call-stack spoofing defeat RIP audits (DoomSyscalls PoC). [github.com/SilentisVox/DoomSyscalls] (high, pattern)
- .pdata-aware stack spoofing defeats unwind-based checks (SindriKit 1.3.0). (high, pattern)
- ETW blinding: patch EtwEventWrite*/NtTraceEvent → silences usermode ETW; kernel ETW-TI not silenced. Detection artefact: NtProtectVirtualMemory on ntdll pages. (high, pattern)
- Kernel-assisted VAD hiding (MiInsertChange/unlink/PTE flips) → VAD scans blind. (high, pattern)
- Scanner evasion by memory type: private RX shellcode, pagefile-backed MEM_MAPPED, LdrLoadDll hollowing populates PEB. [forrest-orr.net] (high, pattern)
- HVCI bypassable (BusterCall PFN-swap); KVC silently disables HVCI/DSE at boot — registry/UI state untrustable. (high, pattern)
- DMA cheats (2026): partially detectable — PCIe config-space baselines, Xilinx IP fingerprinting, IOMMU gaps. (med-high, landscape)

## Findings — defense
- **GetRuntimeAttestationReport** — documented Win32 API: signed Secure Kernel report of loaded drivers + CI state; MS docs name anti-cheat use; needs TPM2/SecureBoot/VBS/HVCI/IOMMU. (high, version)
- CET user-mode shadow stack shipped, opt-in via PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY (compat/strict modes, SetProcessDynamicEnforcedCetCompatibleRanges). Non-CETCOMPAT injected code → #CP faults. (high, version)
- ETW-TI NOT available to third parties (needs ELAM cert + PPL-AM, MVI/NDA). Don't design around it. (high, version)
- NtSetInformationProcess(ProcessInstrumentationCallback=0x28): usermode syscall-return interception — catches direct/indirect syscalls by origin; attacker can NULL it → must heartbeat-guard. (high, pattern)
- Vanguard: pre-boot posture enforcement, on-demand driver, VAN:Restriction caught motherboards falsely reporting IOMMU; BattlEye believed PCI config scan; EAC kernel-or-usermode; Ricochet cloud attestation. (high/med, landscape)
- Raven AC heartbeat: signed payload + server nonce + rolling counter + adaptive cadence + inter-arrival analysis. (med, pattern)
- VM detection: RDTSC-CPUID-RDTSC timing baseline; EPT-hook detection via read-latency anomalies. (med-high, pattern)
- Time manipulation: multi-clock cross-check (rdtsc/QPC/GetTickCount64/dspTime). (med, pattern)

## Leads
- Elastic Defend ETW-callstack writeup; 0xflux EDR series; Kernel_VADInjector IoCs
- Verify GetRuntimeAttestationReport min-OS build before shipping

## Not found
- Named herpaderping successor; CET strict deployed by commercial AC (inferred); Windows-native time-manipulation writeup

## Adoption shortlist
1. GetRuntimeAttestationReport polling — catches BYOVD/KVC/DSE-off/HVCI-off, no driver
2. CET shadow stack strict mode — /CETCOMPAT + policy
3. ProcessInstrumentationCallback — syscall-origin audit (self-heartbeat it)
4. Extend hook scan: EtwEventWrite*/NtTraceEvent + RX-private + pathless MEM_MAPPED regions
5. PCI config-space enum + IOMMU-actually-present + multi-clock divergence heartbeat
