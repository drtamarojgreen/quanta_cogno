# Potential Challenges and Future Enhancements

This document outlines potential challenges and areas for future enhancements identified during the development process.

## 1. Build & Test Environment

A major challenge is the current state of the testing environment. The test runner, when executed, times out instead of running the tests and exiting. This prevents any automated verification of code changes.

*   **Immediate Action:** The timeout issue needs to be diagnosed and fixed. This is a high-priority task to ensure code quality and stability for future development.
*   **Future Enhancement:** Consider adopting a standard, well-supported C++ testing framework (like Google Test or Catch2) to avoid maintaining a custom solution.

## 2. Code-Documentation Discrepancy

There is a significant mismatch between the project's documentation, which describes a "Mental Health Genomics API", and the initial C++ implementation, which was a "Cognitive Modeling Config Tool". This can be a source of confusion for developers.

*   **Immediate Action:** The unrelated code (`main.cpp`, `api_logic.cpp`) should be phased out or moved to a separate project if it's still needed.
*   **Future Enhancement:** Ensure that documentation is always kept in sync with the implementation.

## 3. Incomplete API Infrastructure

The current API is a simulation. The logic exists in a standalone function (`process_api_request`) but is not connected to any web server or infrastructure.

*   **Future Enhancement:** A full implementation will require building out a proper backend, including:
    *   A web server to handle HTTP requests.
    *   Request routing to call the correct API logic functions.
    *   Data persistence (a database or file-based storage for the genomics data).

## 4. Reliance on Custom Frameworks

The project uses custom, header-only frameworks for functionalities like testing. While this avoids external dependencies, it comes with its own challenges.

*   **Challenge:** These custom frameworks may have bugs (like the one fixed in `testing_framework.h`) or lack advanced features found in standard libraries.
*   **Future Enhancement:** For each custom component, a decision should be made whether to continue maintaining it or to replace it with a standard, community-supported library.

## 5. Minimal JSON Parser

The current JSON parser (`JsonValue::parse`) is very basic and was designed for simple, flat objects.

*   **Challenge:** It may not correctly handle the complex, nested data structures defined in the `mental_health_genomics_api.json` specification.
*   **Future Enhancement:** The parser will likely need to be significantly enhanced to support nested objects, arrays of objects, and various data types robustly. Alternatively, a more complete, header-only JSON library could be integrated.
