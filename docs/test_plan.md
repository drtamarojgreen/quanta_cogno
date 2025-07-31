# Application Test Plan

This document outlines the testing strategy for the Mental Health Genomics API. The goal is to ensure code quality, correctness, and robustness through a multi-layered testing approach. All test frameworks are implemented from scratch in C++ without external dependencies.

## 1. Testing Philosophy

We will employ two primary forms of testing:

*   **Unit Tests**: To verify the smallest, isolated parts of the application (individual functions or classes).
*   **Behavior-Driven Development (BDD) Tests**: To verify that the application behaves correctly from a user's perspective, testing end-to-end feature scenarios.

This combination ensures that the low-level components work as expected and that they are integrated correctly to deliver the features described in the enhancement plan.

## 2. Unit Test Strategy

Unit tests are written in C++ using a lightweight, custom-built testing framework (`testing_framework.h`). They are focused on testing logic in isolation.

### Layers to be Tested:

*   **JSON Logic (`json_logic`)**:
    *   **Goal**: Ensure robust JSON parsing and serialization.
    *   **Tests**:
        *   Serialize `JsonValue` objects to strings.
        *   Parse valid JSON strings into `JsonValue` objects.
        *   Handle malformed or invalid JSON strings gracefully.
        *   Test all data types: string, number, bool, null, object, array.

*   **API Simulation Logic (`api_logic`)**:
    *   **Goal**: Verify the logic for interacting with the underlying (simulated) API.
    *   **Tests**:
        *   Correctly build a JSON request from an `ApiConfig` struct.
        *   Save and load `JsonValue` objects to/from the filesystem.

*   **Core Scientific Logic (`core_logic`)**:
    *   **Goal**: Test the business logic for the new scientific and clinical capabilities. This layer is stateless and performs pure data transformation.
    *   **Tests (scaffolding)**:
        *   `compareGeneAnnotations`: Verify that it correctly identifies over-represented annotations between two gene sets.
        *   `getPolypharmacyAdvisory`: Test with known gene variants and drug lists to ensure correct warnings are generated.
        *   `getInteractionNetwork`: Ensure the output format (nodes and edges) is correctly structured for visualization tools.

*   **Server/Endpoint Logic (`server_logic`)**:
    *   **Goal**: Test the logic that handles incoming requests, validation, and routing before hitting the core logic.
    *   **Tests (scaffolding)**:
        *   **Parameter Validation**:
            *   Reject requests to search endpoints (`getResearchAssociations`) with no filter parameters.
            *   Reject requests with invalid enum values (e.g., `confidence_level="invalid"`).
        *   **Rate Limiting**:
            *   Verify that requests from a single source are blocked after exceeding a defined limit.
        *   **Caching**:
            *   Ensure a second identical request to a cacheable endpoint (e.g., `getGene`) is served faster and returns the cached response.

## 3. Behavior-Driven Development (BDD) Test Strategy

BDD tests ensure that the application's features work as intended from end to end. We use a custom BDD framework that parses human-readable `.feature` files and executes corresponding C++ step definitions.

### Feature Files:

*   **`api_consistency.feature`**:
    *   **Goal**: Verify the API design and consistency enhancements from `plan.md`.
    *   **Scenarios**:
        *   Searching for genes using the new `searchGenes` function name.
        *   Attempting to call a search endpoint without any parameters and receiving an error.
        *   Using a consistent `gene_ids` parameter across different endpoints.

*   **`error_handling.feature`**:
    *   **Goal**: Ensure standardized, helpful error messages are returned.
    *   **Scenarios**:
        *   Providing an invalid value for an enum parameter (e.g., `confidence_level`).
        *   Making a request with a missing required parameter.
        *   The system returns a JSON error object with `code` and `message` fields.

*   **`scientific_analysis.feature`**:
    *   **Goal**: Test the new scientific and clinical endpoints.
    *   **Scenarios**:
        *   Requesting a polypharmacy advisory for a patient with a specific gene variant and drug list, then checking for the expected warning in the response.
        *   Comparing two gene lists and verifying that the differential annotation analysis identifies a known, over-represented pathway.
        *   Requesting an interaction network and ensuring the response contains the correct nodes and edges for visualization.

*   **`performance.feature`**:
    *   **Goal**: Test performance and reliability features.
    *   **Scenarios**:
        *   Making multiple requests from one client to trigger the rate limiter.
        *   Requesting the same gene data twice to confirm the cache is working.

## 4. Test Execution

A single test runner executable will be created by compiling `main_test_runner.cpp` along with all `_test.cpp` and `_steps.cpp` files.

*   The runner will first execute all registered unit tests, reporting suite-by-suite results.
*   It will then parse all `.feature` files in a `features/` directory.
*   For each scenario, it will find and execute the matching C++ step definitions.
*   The runner will exit with a non-zero status code if any unit test or BDD scenario fails.

This approach provides a comprehensive, self-contained, and automated testing process to validate the application's functionality at all levels.