#include "../flexible_json_logic.h"
#include "../json_logic.h"
#include <iostream>
#include <fstream>

// Example demonstrating flexible JSON configuration usage
int main() {
    std::cout << "=== Flexible JSON Configuration Example ===" << std::endl;
    
    // 1. Initialize Configuration Manager
    ConfigurationManager config_manager;
    
    // Load the flexible configuration
    if (!config_manager.loadConfiguration("../json/flexible_config_example.json")) {
        std::cerr << "Failed to load configuration" << std::endl;
        return 1;
    }
    
    std::cout << "Configuration loaded successfully" << std::endl;
    
    // 2. Example: Parameter Resolution and Validation
    std::cout << "\n=== Parameter Resolution Example ===" << std::endl;
    
    // Create input parameters with various formats
    JsonValue input_params = JsonValue::makeObject();
    
    // Gene identifiers in different formats
    JsonValue gene_array = JsonValue::makeArray();
    gene_array.array_value.push_back(JsonValue::makeString("COMT"));        // Gene symbol
    gene_array.array_value.push_back(JsonValue::makeString("1312"));        // Entrez ID
    gene_array.array_value.push_back(JsonValue::makeString("ENSG00000196569")); // Ensembl ID
    input_params.object_value["gene_ids"] = gene_array;
    
    // Mental health condition
    input_params.object_value["condition"] = JsonValue::makeString("depression");
    
    // Confidence level using alias
    input_params.object_value["confidence"] = JsonValue::makeString("strong"); // Will be mapped to "high"
    
    // Resolve and validate parameters
    JsonValue resolved_params = config_manager.resolveParameters("getMentalHealthGenes", input_params);
    
    std::cout << "Original parameters: " << input_params.serialize() << std::endl;
    std::cout << "Resolved parameters: " << resolved_params.serialize() << std::endl;
    
    // Validate the request
    bool is_valid = config_manager.validateRequest("getMentalHealthGenes", resolved_params);
    std::cout << "Parameters valid: " << (is_valid ? "Yes" : "No") << std::endl;
    
    // 3. Example: Workflow Execution
    std::cout << "\n=== Workflow Execution Example ===" << std::endl;
    
    WorkflowEngine workflow_engine(&config_manager);
    
    // Prepare workflow input
    JsonValue workflow_input = JsonValue::makeObject();
    workflow_input.object_value["condition"] = JsonValue::makeString("depression");
    workflow_input.object_value["analysis_depth"] = JsonValue::makeString("comprehensive");
    
    // Add sample patient variants
    JsonValue variants = JsonValue::makeArray();
    JsonValue variant1 = JsonValue::makeObject();
    variant1.object_value["variant_id"] = JsonValue::makeString("rs4680");
    variant1.object_value["genotype"] = JsonValue::makeString("1/1");
    variant1.object_value["chromosome"] = JsonValue::makeString("22");
    variant1.object_value["position"] = JsonValue::makeNumber(19963748);
    variant1.object_value["ref_allele"] = JsonValue::makeString("G");
    variant1.object_value["alt_allele"] = JsonValue::makeString("A");
    variants.array_value.push_back(variant1);
    
    workflow_input.object_value["patient_variants"] = variants;
    
    std::cout << "Workflow input: " << workflow_input.serialize() << std::endl;
    
    // Execute workflow (this would normally make actual API calls)
    std::cout << "Executing comprehensive mental health analysis workflow..." << std::endl;
    
    // For demonstration, we'll simulate the workflow execution
    JsonValue workflow_result = JsonValue::makeObject();
    
    // Simulate workflow steps
    std::cout << "Step 1: Gene discovery (parallel execution)" << std::endl;
    std::cout << "  - Fetching known genes for depression..." << std::endl;
    std::cout << "  - Searching recent literature..." << std::endl;
    
    std::cout << "Step 2: Variant analysis (conditional execution)" << std::endl;
    std::cout << "  - Annotating patient variants..." << std::endl;
    std::cout << "  - Filtering relevant variants..." << std::endl;
    
    std::cout << "Step 3: Pathway analysis (sequential execution)" << std::endl;
    std::cout << "  - Running enrichment analysis..." << std::endl;
    std::cout << "  - Building interaction network..." << std::endl;
    
    std::cout << "Step 4: Pharmacogenomics (conditional execution)" << std::endl;
    std::cout << "  - Analyzing drug-gene interactions..." << std::endl;
    std::cout << "  - Checking polypharmacy warnings..." << std::endl;
    
    std::cout << "Step 5: Report generation" << std::endl;
    std::cout << "  - Compiling results..." << std::endl;
    std::cout << "  - Generating visualizations..." << std::endl;
    
    // Create mock results
    JsonValue summary = JsonValue::makeObject();
    summary.object_value["condition"] = JsonValue::makeString("depression");
    summary.object_value["genes_found"] = JsonValue::makeNumber(45);
    summary.object_value["variants_analyzed"] = JsonValue::makeNumber(1);
    summary.object_value["pathways_enriched"] = JsonValue::makeNumber(12);
    summary.object_value["drug_interactions"] = JsonValue::makeNumber(8);
    
    JsonValue metadata = JsonValue::makeObject();
    metadata.object_value["execution_time"] = JsonValue::makeNumber(23.5);
    JsonValue data_sources = JsonValue::makeArray();
    data_sources.array_value.push_back(JsonValue::makeString("ncbi_api"));
    data_sources.array_value.push_back(JsonValue::makeString("local_genomics_db"));
    data_sources.array_value.push_back(JsonValue::makeString("pubmed_cache"));
    metadata.object_value["data_sources_used"] = data_sources;
    metadata.object_value["cache_hits"] = JsonValue::makeNumber(15);
    
    workflow_result.object_value["summary"] = summary;
    workflow_result.object_value["metadata"] = metadata;
    
    std::cout << "\nWorkflow completed successfully!" << std::endl;
    std::cout << "Results summary: " << summary.serialize() << std::endl;
    std::cout << "Execution metadata: " << metadata.serialize() << std::endl;
    
    // 4. Example: Data Source Flexibility
    std::cout << "\n=== Data Source Flexibility Example ===" << std::endl;
    
    std::vector<std::string> available_sources = config_manager.getAvailableDataSources();
    std::cout << "Available data sources:" << std::endl;
    for (const auto& source : available_sources) {
        std::cout << "  - " << source << std::endl;
    }
    
    // 5. Example: Quick Gene Lookup Workflow
    std::cout << "\n=== Quick Gene Lookup Example ===" << std::endl;
    
    JsonValue quick_input = JsonValue::makeObject();
    JsonValue quick_genes = JsonValue::makeArray();
    quick_genes.array_value.push_back(JsonValue::makeString("HTR2A"));
    quick_input.object_value["genes"] = quick_genes;
    
    std::cout << "Quick lookup input: " << quick_input.serialize() << std::endl;
    std::cout << "Executing quick gene lookup workflow..." << std::endl;
    
    // Simulate quick lookup
    JsonValue quick_result = JsonValue::makeObject();
    JsonValue gene_info = JsonValue::makeObject();
    gene_info.object_value["gene_id"] = JsonValue::makeString("3356");
    gene_info.object_value["symbol"] = JsonValue::makeString("HTR2A");
    gene_info.object_value["name"] = JsonValue::makeString("5-hydroxytryptamine receptor 2A");
    gene_info.object_value["chromosome"] = JsonValue::makeString("13");
    gene_info.object_value["location"] = JsonValue::makeString("13q14.2");
    
    quick_result.object_value["gene_info"] = gene_info;
    
    std::cout << "Quick lookup result: " << quick_result.serialize() << std::endl;
    
    // 6. Example: Error Handling and Validation
    std::cout << "\n=== Error Handling Example ===" << std::endl;
    
    // Try invalid parameters
    JsonValue invalid_params = JsonValue::makeObject();
    invalid_params.object_value["confidence_level"] = JsonValue::makeString("invalid_level");
    
    std::vector<std::string> validation_errors = config_manager.getValidationErrors("getMentalHealthGenes", invalid_params);
    
    if (!validation_errors.empty()) {
        std::cout << "Validation errors found:" << std::endl;
        for (const auto& error : validation_errors) {
            std::cout << "  - " << error << std::endl;
        }
    }
    
    // 7. Example: Template Resolution
    std::cout << "\n=== Template Resolution Example ===" << std::endl;
    
    // Simulate template resolution
    std::map<std::string, std::string> template_context;
    template_context["INPUT:condition"] = "depression";
    template_context["CONFIG:default_confidence_level"] = "medium";
    template_context["CALC:current_year"] = "2025";
    template_context["ENV:NCBI_API_KEY"] = "demo_api_key_12345";
    
    std::cout << "Template context:" << std::endl;
    for (const auto& kv : template_context) {
        std::cout << "  " << kv.first << " = " << kv.second << std::endl;
    }
    
    // Example template strings
    std::vector<std::string> templates = {
        "${INPUT:condition}",
        "${CONFIG:default_confidence_level|medium}",
        "${CALC:current_year - 3}",
        "${ENV:NCBI_API_KEY}"
    };
    
    std::cout << "\nTemplate resolution examples:" << std::endl;
    FlexibleJsonValue flexible_json;
    for (const auto& template_str : templates) {
        std::string resolved = flexible_json.resolveTemplate(template_str, template_context);
        std::cout << "  " << template_str << " -> " << resolved << std::endl;
    }
    
    std::cout << "\n=== Example Complete ===" << std::endl;
    std::cout << "This example demonstrates the flexible JSON architecture's capabilities:" << std::endl;
    std::cout << "1. Parameter resolution and validation with aliases" << std::endl;
    std::cout << "2. Complex workflow execution with multiple steps" << std::endl;
    std::cout << "3. Multiple data source support" << std::endl;
    std::cout << "4. Error handling and validation" << std::endl;
    std::cout << "5. Template resolution for dynamic configuration" << std::endl;
    std::cout << "6. Caching and performance optimization" << std::endl;
    
    return 0;
}

// Additional utility functions for demonstration

void demonstrateDataSourceScenarios() {
    std::cout << "\n=== Data Source Scenarios ===" << std::endl;
    
    // Scenario 1: API becomes unavailable, fallback to cache
    std::cout << "Scenario 1: API fallback to cache" << std::endl;
    std::cout << "  Primary: NCBI API (unavailable)" << std::endl;
    std::cout << "  Fallback: Local cache (success)" << std::endl;
    std::cout << "  Result: Data served from cache with warning" << std::endl;
    
    // Scenario 2: Multiple data sources for comprehensive analysis
    std::cout << "\nScenario 2: Multi-source data integration" << std::endl;
    std::cout << "  Source 1: NCBI API (gene information)" << std::endl;
    std::cout << "  Source 2: Local database (pathway data)" << std::endl;
    std::cout << "  Source 3: VCF files (variant annotations)" << std::endl;
    std::cout << "  Source 4: Expression data files (tissue-specific expression)" << std::endl;
    std::cout << "  Result: Integrated analysis across all data types" << std::endl;
    
    // Scenario 3: Real-time streaming data
    std::cout << "\nScenario 3: Streaming data integration" << std::endl;
    std::cout << "  Stream 1: Real-time genomics updates" << std::endl;
    std::cout << "  Stream 2: New research publications" << std::endl;
    std::cout << "  Result: Always up-to-date analysis results" << std::endl;
}

void demonstrateWorkflowFlexibility() {
    std::cout << "\n=== Workflow Flexibility Examples ===" << std::endl;
    
    // Different analysis depths
    std::cout << "Analysis Depth Variations:" << std::endl;
    std::cout << "  Basic: Gene lookup + basic annotations" << std::endl;
    std::cout << "  Comprehensive: + pathway analysis + drug interactions" << std::endl;
    std::cout << "  Research: + network analysis + polypharmacy + visualizations" << std::endl;
    
    // Conditional execution
    std::cout << "\nConditional Execution:" << std::endl;
    std::cout << "  If patient_variants provided -> run variant analysis" << std::endl;
    std::cout << "  If gene_count > 5 -> run pathway enrichment" << std::endl;
    std::cout << "  If analysis_depth == 'research' -> generate visualizations" << std::endl;
    
    // Parallel vs Sequential
    std::cout << "\nExecution Strategies:" << std::endl;
    std::cout << "  Parallel: Gene discovery + literature search (independent)" << std::endl;
    std::cout << "  Sequential: Variant annotation -> filtering (dependent)" << std::endl;
    std::cout << "  Conditional: Only if previous steps succeeded" << std::endl;
}
