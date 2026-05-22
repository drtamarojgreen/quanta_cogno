# Sorrel‑Driven Development (SDD): Implementation Guide

## Part I: The SDD Philosophy & Direction

SDD is a **card‑based, fact‑grounded, empirical‑verification methodology**. 
The core principle: **every claim about the system must be backed by an executable “card” that derives its inputs from factual ground‑truth files and produces verifiable, empirical outputs**.

## Part II: The Directory Structure

```
tests/sdd/
├── cards/                    # Executable C++ test programs (dispatched via argv)
├── cpp/
│   └── util/                 # Shared C++ utilities (FactReader, MetricReporter)
├── facts/                    # Ground‑truth input files (*.facts)
├── sorrel_checkins.md        # Intent ledger (BDD-style feature statements)
├── sorrel_checkouts.md       # Evidence ledger (observations, measurements)
├── Makefile                  # Automated build & runner
└── run_sdd_suite.sh          # Suite runner script
```

## Part III: File Specifications

### 1. Fact Files (`*.facts`)
```
Situation: Default
Is key = value
```
- Must begin with a `Situation:` header.
- Result facts must be **numeric**.

### 2. Card Files (`*Class.cpp`)
- Standalone C++ programs.
- Use annotations: ` @Card`, ` @Is`, ` @Results`.
- Logic-dispatch via `main(argc, argv)` using `argv[1]`.

## Part IV: The C++ Utility Layer

- **FactReader**: Parses `*.facts` files into `std::map<std::string, std::string>`.
- **MetricReporter**: Outputs `METRIC [key] = value` or `key = value` strings for machine parsing.

## Part V: Implementation Cycle

1. **Scaffold**: Create directory structure.
2. **Utilities**: Implement `FactReader` and `MetricReporter`.
3. **Facts**: Define ground-truth parameters.
4. **Cards**: Write minimum runnable programs to prove claims.
5. **Ledgers**: Document intent in `checkins` and evidence in `checkouts`.
6. **Automation**: Provide Makefile and shell runners.

## Part VI: Key Differences from Traditional Testing

| Aspect | Traditional | SDD |
|--------|-------------|-----|
| **Input** | Hard-coded | External `.facts` |
| **Output** | Pass/Fail | Numeric `key = value` |
| **Dependency** | Mocked | Linked against real `.o` |
| **Agent-Proof** | None | Empirical measurements |
| **Structural** | External lint | Built-in Audit cards |
