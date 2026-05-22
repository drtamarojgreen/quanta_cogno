# Quanta Cogno v2 - Genomic Simulation Toolkit

## Overview
Quanta Cogno v2 is a clean-sheet, high-performance C++17 toolkit for genomic simulation and visualization. It features zero external dependencies, hand-rolled parsers for genomic data formats, and a robust ANSI-driven terminal user interface.

## Core Features
- **High-Performance IO**: Zero-dependency parsers for JSON, CSV, XML, and YAML.
- **Deterministic Simulation**: Discrete-time modeling of SNP interactions and gene expression pathways.
- **ANSI Visualization**: Flicker-free terminal rendering for genomic sequences and heatmaps.
- **Reliability Layer**: Asynchronous state management with periodic auto-save and crash recovery.
- **Comprehensive Testing**: Multi-layered suite covering Unit, BDD, and E2E validation.

## Building and Running
Refer to [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) for platform-specific details.

```bash
# Build the application
g++ -std=c++17 src/core/*.cpp src/io/*.cpp src/visualization/*.cpp src/app/*.cpp src/main.cpp -Isrc -o quanta_cogno_v2 -pthread

# Run the TUI
./quanta_cogno_v2
```

## Supported Formats
- **Import**: JSON, CSV, XML, YAML
- **Export**: BMP (Heatmaps), SVG (Networks), MIDI (Sonification)
