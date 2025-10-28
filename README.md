# Mental Health Genomics API

This project is a C++ application that provides a simulated API for exploring the complex relationships between genomics and mental health. It is designed to be a tool for researchers, clinicians, and developers, offering a range of endpoints for querying genetic data, analyzing pathways, and assessing therapeutic interactions.

The entire application, including its testing frameworks, is built from scratch in C++ with no external library dependencies.

## Core Features

The API provides a comprehensive suite of functions to interrogate genomic data. Key capabilities include:

*   **Gene & Variant Data**: Retrieve detailed information about specific genes (`getGene`), their variants (`getGeneVariants`), and expression patterns (`getGeneExpression`).
*   **Mental Health Context**: Find genes associated with specific mental health conditions (`getMentalHealthGenes`) and related research publications (`getResearchAssociations`).
*   **Pathway Analysis**: Analyze biological pathways for a given set of genes (`getPathwayAnalysis`) and explore Gene Ontology annotations (`getGeneOntology`).
*   **Pharmacogenomics**: Investigate drug-gene interactions (`getDrugGeneInteractions`) and retrieve polygenic risk scores (`getPolygeneticRiskScores`).

## Planned Enhancements

This project is actively being enhanced. The development roadmap is detailed in `docs/plan.md` and focuses on the following areas:

1.  **API Design and Consistency**:
    *   Standardizing function and parameter names (e.g., `getGeneByAccession` -> `searchGenes`).
    *   Enforcing mandatory parameters for broad search endpoints to prevent expensive queries.
    *   Implementing strict enum types for parameters with a fixed set of values.

2.  **Performance and Reliability**:
    *   Implementing in-memory response caching for frequently accessed static data.
    *   Introducing request rate-limiting to ensure API stability and fair usage.
    *   Standardizing JSON error responses for predictable error handling.

3.  **Scientific and Clinical Capabilities**:
    *   **Differential Annotation Analysis**: A new `compareGeneAnnotations` endpoint to find over-represented annotations between two gene sets.
    *   **Polypharmacy Advisory**: A new `getPolypharmacyAdvisory` endpoint to check for potential adverse interactions between a patient's genetic variants and prescribed medications.
    *   **Network Visualization**: A new `getInteractionNetwork` endpoint to provide data structured for use in client-side visualization libraries.

## Project Structure

```
.
├── docs/
│   ├── implementation_roadmap.md
│   ├── json_flexibility_analysis.md
│   ├── plan.md
│   └── test_plan.md
├── json/
│   ├── cognos.schema.json
│   ├── flexible_config_example.json
│   ├── mental_health_genomics_api.json
│   └── sample_request.json
├── src/
│   ├── examples/
│   ├── api_handler.cpp
│   ├── api_handler.h
│   ├── api_logic.cpp
│   ├── api_logic.h
│   ├── flexible_json_logic.cpp
│   ├── flexible_json_logic.h
│   ├── json_logic.cpp
│   ├── json_logic.h
│   ├── main.cpp
│   ├── test_runner_main.cpp
│   ├── testing_framework.h
│   └── unit_tests.cpp
├── tests/
│   ├── bdd_style_tests.cpp
│   ├── new_unit_tests.cpp
│   └── unit_tests.cpp
├── README.md
└── implementation_summary.md
```

## Testing Strategy

The project employs a robust, multi-layered testing strategy to ensure code quality and correctness, as detailed in `docs/test_plan.md`.

### Unit Tests

Low-level components and logic layers are tested using a custom, header-only unit testing framework (`testing_framework.h`). These tests verify the correctness of individual functions and classes in isolation.

### Behavior-Driven Development (BDD) Tests

End-to-end features are tested using a BDD approach.

1.  **Features**: Scenarios are defined in human-readable `.feature` files located in the `features/` directory. These describe how the API should behave from a user's perspective.
2.  **Steps**: The steps in the feature files are implemented in C++ (`bdd_steps.cpp`) using a lightweight BDD framework (`bdd_framework.h`).
3.  **Execution**: A test runner executable ties everything together, parsing the feature files and executing the corresponding C++ step definitions to validate the application's behavior.

## Building and Testing

This project is built with C++17. You will need a compatible C++ compiler, such as `g++`.

### Compiling the Main Application

To compile the main application, run the following command from the root directory:

```bash
g++ -std=c++17 -I./src -o mental_health_api src/main.cpp src/api_handler.cpp src/api_logic.cpp src/flexible_json_logic.cpp src/json_logic.cpp
```

### Compiling and Running Tests

To compile the test runner, which includes all unit and BDD-style tests, run the following command:

```bash
g++ -std=c++17 -I./src -o test_runner src/test_runner_main.cpp src/api_handler.cpp src/api_logic.cpp src/flexible_json_logic.cpp src/json_logic.cpp src/unit_tests.cpp tests/unit_tests.cpp tests/bdd_style_tests.cpp tests/new_unit_tests.cpp
```

To run the tests, execute the compiled test runner:

```bash
./test_runner
```