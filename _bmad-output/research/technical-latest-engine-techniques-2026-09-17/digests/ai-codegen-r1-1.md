# Digest: ai-codegen r1 (subagent dbf99a47)

## Findings — local models
- Qwen3-Coder tiers: 480B-A35B (256K ctx, SOTA agentic), 30B-A3B-Instruct (Apache-2.0, 262K), Qwen3-Coder-Next 80B/3B for coding agents + official GGUF. [github.com/QwenLM/Qwen3-Coder, HF] (high, landscape)
- DeepSeek-Coder-V2: 16B Lite (2.4B active, >100 tok/s RTX4090) & 236B; commercial-OK. (high specs / med speed, version)
- Codestral 22B: MNPL non-production license — unusable in shipped product. (med, landscape)
- StarCoder2 (3B/7B/15B, OpenRAIL-M): ~180 tok/s midrange, standard autocomplete tier; Devstral-2 22B Apache-2.0 reportedly 52.3% SWE-bench. (med, performance)

## Findings — structured decoding
- llama.cpp: GBNF grammar sampling + JSON-schema→GBNF conversion + llguidance backend. (high, pattern)
- XGrammar: default backend for vLLM/SGLang/TRT-LLM/MLC; 100% structural correctness, near-zero JSON overhead. [MLC-AI, arXiv 2411.15100] (high, performance)
- Caveat: over-restrictive grammars degrade output quality (CRANE ICML'25, GAD NeurIPS'24). (high, pattern)
- llama.cpp /infill FIM endpoint; FIM needs base Coder models, instruct models hallucinate. (high, pattern)

## Findings — codebase RAG
- Reference arch: tree-sitter symbol chunking, hybrid dense+BM25 + RRF, metadata filters, cross-encoder rerank top50→5. [llmversus.com] (med-high, pattern)
- Anthropic: Claude Code avoids embedding indexes — agentic grep + LSP; staleness = core failure mode. [claude.com blog] (high, pattern)
- Cursor: +12.5% agent accuracy adding semantic search alongside grep → converged = hybrid. (med, performance)
- nomic-embed-code (7B, Apache-2.0): SOTA CodeSearchNet, GGUF for llama.cpp; 137M int8 CodeRankEmbed for CPU-only. **C++ not in training langs — must validate.** (high, landscape)

## Findings — agentic patterns
- MCP is converged integration surface (VS Code/Cursor/Claude Code, spec 2025-11-25). (high, pattern)
- Narrow typed tool surface: search_symbol/find_tests/read_api_contract returning path+excerpt+confidence. (med-high, pattern)
- Loop runtimes: external verifier decides DONE, standing orders re-injected, escalation triggers, branch isolation. (med, pattern)

## Leads
- llama.cpp llama.vim FIM-in-editor reference; Cursor semantic-search primary post; CRANE for grammar quality

## Not found
- C++-specific codegen head-to-head benchmarks; Qwen3-Coder-Next license unconfirmed; Outlines/Guidance status

## Adoption shortlist
1. llama.cpp + GGUF coder models tiered (base for /infill FIM, 30B-A3B-Instruct for chat)
2. GBNF/JSON-schema constrain LLM output to structured patch/AST spec (not full-C++ grammar)
3. Hybrid retrieval: existing KG as symbol layer + grep + optional nomic embeddings (validate C++ first)
4. MCP-shaped narrow typed internal tools (search_symbol/get_build_errors/run_tests)
5. External-verifier loop — build/test results as DONE check
