# DESIGN.md - Quanta Cogno v2

## 1. Overview
Quanta Cogno v2 is a high-performance, modular C++17 toolkit designed for genomic simulation and visualization. It prioritizes deterministic execution, zero-dependency reliability, and ANSI-driven terminal visualization.

## 2. High-Level Architecture
The system follows a strict modular design to ensure separation of concerns and testability.

```

├── src/
│   ├── core/             # Genomic simulation engine, SNP interaction, pathways
│   ├── io/               # Zero-dependency parsers (JSON, XML, CSV, YAML)
│   ├── visualization/    # ANSI/ASCII rendering (Sequence, Heatmap, Graph)
│   ├── app/              # Orchestration, CLI/TUI, State/Cache Management
│   ├── utils/            # Math, logging, error handling, shared primitives
│   └── main.cpp
├── tests/
│   ├── unit/             # Component-level verification
│   ├── bdd/              # Behavioral/Workflow verification
│   ├── e2e/              # Full pipeline validation
│   └── sdd/              # Structural/Schema audits
├── schemas/              # Data and configuration schemas
├── rules/                # Externalized simulation and UI rules
├── docs/                 # Verification reports and logs
├── Makefile              # Reproducible build system
└── DESIGN.md             # This document
```

## 3. Core Modules & Component Contracts

### 3.1 IO Subsystem (`io/`)
Hand-rolled parsers for complex text formats.
- **Contract**: `std::optional<JsonValue> JsonParser::parse(const std::string& input);`
- **Features**: Error reporting with line/column tracking, zero-copy optimization where possible.

### 3.2 Genomic Simulation Engine (`core/`)
Manages stateful genomic models.
- **SNP Interaction**: Models epistatic relationships and phenotype propagation.
- **Pathway Propagation**: Discrete-time simulation of signal transduction through gene networks.
- **Contract**: `void SimulationEngine::step(double delta_time);`

### 3.3 Visualization Pipeline (`visualization/`)
ANSI-based terminal rendering.
- **Virtual Terminal**: 2D buffer-based rendering to minimize flickering.
- **Themes**: Declarative color palettes stored in `rules/`.
- **Modes**:
    - `SequenceView`: ANSI-colorized A/C/G/T/U scrolling.
    - `HeatmapView`: 2D intensity map for gene expression.
    - `NetworkView`: ASCII node-edge graph for pathways.

### 3.4 Application Layer (`app/`)
- **StateManager**: Asynchronous auto-save with state versioning.
- **CacheManager**: Incremental frame caching using local JSON persistence.
- **Interface**: Unified dispatch for CLI commands and TUI events.

## 4. Data Formats

### 4.1 Genomic Simulation State (JSON)
```json
{
  "generation": 450,
  "genes": [
    {"id": "HTR2A", "expression": 0.85, "variants": ["rs6311"]},
    {"id": "COMT", "expression": 0.42, "variants": ["rs4680"]}
  ],
  "active_pathways": ["serotonin_signaling"]
}
```

## 5. Build System
- **Compiler**: C++17 (MSVC on Windows, GCC/Clang on Linux).
- **Dependencies**: None.
- **Build Targets**: 
    - `all`: Full application.
    - `tests`: Test runner executable.
    - `verify`: Automated run of the full verification suite.

## 6. Testing & Verification Strategy

### 6.1 Multi-Layered Testing
- **Unit Tests**: Coverage of every parser and math utility.
- **BDD Tests**: Verification of genomic workflows (e.g., "Given a SNP mutation, when simulated, then expression changes").
- **E2E Tests**: Validating the flow from a CSV input file to a specific ASCII heatmap output.
- **SDD Audits**: Automated checks that `src/` follows the package structure and `DESIGN.md` contracts.

### 6.2 Audit Logs
All verification results are written to:
- `docs/testing_logs.md`
- `docs/verification_reports.md`
- `docs/runtime_audits.md`

## 7. Performance Targets
- **Parsing**: >50MB/s for JSON/CSV on standard hardware.
- **Simulation**: 10k+ gene interactions at 60Hz.
- **Visualization**: 30+ FPS terminal refresh rate using double-buffering.
