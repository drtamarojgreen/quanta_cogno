# Application Enhancement Plan

This document outlines potential enhancements for the Mental Health Genomics API and its C++ implementation. The focus is on improvements that can be achieved without introducing new external library dependencies, leveraging standard language features and existing architecture.

## 1. API Design and Consistency

Refinements to the API contract to improve clarity, consistency, and prevent misuse.

*   **Mandatory Search Parameters**: For endpoints that perform broad searches, require at least one filter parameter to be present. This prevents overly broad and expensive queries that could strain system resources.
    *   **Affected endpoints**: `getResearchAssociations`, `getDrugGeneInteractions`, `getPolygeneticRiskScores`.
    *   **Action**: Update the API definition (`mental_health_genomics_api.json`) to enforce that at least one optional parameter is supplied for these functions. The server-side logic should validate this.

*   **Consistent Naming Conventions**: Standardize parameter and function names for better predictability.
    *   **Function Rename**: Rename `getGeneByAccession` to `searchGenes` to more accurately reflect its capability to search by symbols, locations, and text, not just accession IDs.
    *   **Parameter Naming**: Unify gene identifier parameters. For example, use `gene_ids` consistently instead of `gene`, `geneId`, and `gene_ids`.

*   **Strict Enum Types**: For parameters that accept a limited set of values, define them as enums in the API specification.
    *   **Example**: The `confidence_level` parameter in `getMentalHealthGenes` should be restricted to `high`, `medium`, `low`, `all`.
    *   **Action**: Implement server-side validation to reject requests with invalid values for these parameters.

## 2. Performance and Reliability

Enhancements to ensure the API is fast, scalable, and robust.

*   **Response Caching**: Implement an in-memory caching mechanism for frequently requested, static data. This can be a simple time-based cache managed within the application to reduce database load.
    *   **Cacheable endpoints**: `getGene`, `getGeneOntology` for popular genes.
    *   **Implementation**: Use a standard hash map with expiration timestamps. No external caching system (like Redis) is needed.

*   **Request Throttling and Rate Limiting**: Introduce a basic rate-limiting mechanism to protect the API from abuse and ensure fair usage.
    *   **Implementation**: This can be implemented in-memory, tracking request counts per IP address or API key over a specific time window.

*   **Standardized Error Responses**: Define and implement a consistent JSON structure for error responses across all endpoints. This helps client-side developers handle errors more effectively.
    *   **Example Structure**:
        ```json
        {
          "error": {
            "code": 400,
            "message": "Invalid parameter: 'confidence_level' must be one of [high, medium, low, all].",
            "requestId": "abc-123"
          }
        }
        ```

## 3. C++ Implementation Enhancements

Improvements for the C++ application/client that interacts with the API.

*   **Asynchronous API Client**: Refactor the C++ client code to use asynchronous operations for API calls.
    *   **Implementation**: Utilize `std::async` or `std::thread` from the C++ standard library to perform network requests in the background. This will prevent blocking the main thread in a client application (e.g., a GUI or an interactive CLI).

*   **Command-Line Interface (CLI) Tool**: Develop a full-featured CLI tool in C++ for interacting with the genomics API.
    *   **Features**:
        *   Sub-commands for each API function (e.g., `genomics-cli get-gene --gene_id ...`).
        *   Argument parsing (can be implemented with minimal code without a library).
        *   Formatted output (e.g., JSON, simple tables).
    *   **Benefit**: Provides a powerful tool for bioinformaticians and researchers to script and automate queries directly from the terminal.

*   **Improved Data Mapping**: Enhance the C++ structs/classes that map to the API's JSON objects.
    *   **Action**: Implement validation logic within the C++ data structures (e.g., in constructors or setter methods) to ensure data integrity on the client side. For example, an `AgeRange` class could ensure `min_age` is not greater than `max_age`.

## 4. Developer Experience

Focus on making the API easier to use and maintain.

*   **Automated Documentation Generation**: Create a script to generate user-friendly HTML documentation from the `mental_health_genomics_api.json` specification. This script can be written in a language like Python (which is often available in dev environments) or even as a C++ utility.

*   **Enhanced Logging**: Implement more structured and detailed logging on the server side.
    *   **Details**: Log key information for each request, such as the endpoint, parameters, response time, and a unique request ID. This aids in debugging and performance monitoring.

## 5. Scientific and Clinical Capabilities

Enhancements focused on providing deeper analytical capabilities for researchers and clinicians, particularly in studying annotations and assessing therapeutic responses.

### Annotation Analysis

*   **Differential Annotation Analysis**: Introduce a new endpoint, `compareGeneAnnotations`, that accepts two distinct lists of gene IDs. The function would perform an enrichment analysis to identify annotations (e.g., GO terms, pathways) that are significantly over-represented in one set compared to the other. This is crucial for comparing experimental results, such as case vs. control gene sets.

*   **GO Term Semantic Similarity**: Enhance the `getGeneOntology` endpoint with an option to calculate and return semantic similarity scores between the GO terms associated with the input genes. This helps identify functionally related genes even if they don't share identical annotations. The similarity calculation can be pre-computed and stored based on the GO graph structure, avoiding complex real-time graph traversal.

*   **Evidence Code Grouping**: In `getGeneOntology`, allow the `evidence_codes` parameter to accept predefined group aliases like `experimental` (covering EXP, IDA, IPI, IMP, etc.) or `computational` (covering IEA, ISS, etc.). This simplifies queries for users who want to filter by the quality of evidence without needing to know every individual code.

### Therapeutic Interaction Assessment

*   **Structured Data for Network Visualization**: Add a new endpoint, `getInteractionNetwork`, that returns protein-protein or drug-gene interaction data as a structured list of nodes and edges. This format is directly consumable by client-side graphing libraries (e.g., D3.js, Cytoscape.js) for creating interactive network visualizations.
    *   **Example Node/Edge format**:
        ```json
        {
          "nodes": [{"id": "CYP2D6", "type": "gene"}, {"id": "Risperidone", "type": "drug"}],
          "edges": [{"source": "CYP2D6", "target": "Risperidone", "interaction": "metabolism"}]
        }
        ```

*   **Polypharmacy Interaction Warnings**: Introduce a new endpoint, `getPolypharmacyAdvisory`. It would accept a list of patient-specific gene variants (e.g., in CYP2D6, CYP2C19) and a list of prescribed drugs. The API would return potential gene-drug and drug-drug-gene interactions, flagging combinations that could lead to altered metabolism or adverse effects.

*   **Detailed Dosing Guidelines**: Expand the response of `getDrugGeneInteractions` when `include_guidelines` is `true`. Instead of a simple flag, return a structured object containing specific recommendations from sources like CPIC or PharmGKB. This object should include details on dosage adjustments, alternative medication suggestions, and the strength of the recommendation.