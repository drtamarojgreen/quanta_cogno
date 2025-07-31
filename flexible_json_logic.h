#ifndef FLEXIBLE_JSON_LOGIC_H
#define FLEXIBLE_JSON_LOGIC_H

#include "json_logic.h"
#include <map>
#include <vector>
#include <functional>
#include <memory>
#include <regex>
#include <cstddef>

// Forward declarations
class DataSource;
class ConfigurationManager;
class WorkflowEngine;

// Enhanced JSON value with template resolution and validation
class FlexibleJsonValue : public JsonValue {
public:
    // Template resolution with context variables
    std::string resolveTemplate(const std::string& template_str, 
                               const std::map<std::string, std::string>& context) const;
    
    // Dynamic validation against schema
    bool validateAgainstSchema(const JsonValue& schema) const;
    
    // Parameter transformation based on rules
    JsonValue transformParameters(const JsonValue& transformation_rules) const;
    
    // Conditional evaluation
    bool evaluateCondition(const std::string& condition, 
                          const std::map<std::string, JsonValue>& context) const;
    
    // Extract values using JSONPath-like syntax
    std::vector<JsonValue> extractValues(const std::string& path) const;
    
    // Merge multiple JSON values
    static JsonValue merge(const std::vector<JsonValue>& values, 
                          const std::string& strategy = "deep_merge");
    
    // Resolve parameter aliases
    JsonValue resolveAliases(const JsonValue& alias_map) const;
};

// Abstract base class for data sources
class DataSource {
public:
    virtual ~DataSource() = default;
    virtual JsonValue execute(const std::string& operation, 
                             const JsonValue& parameters) = 0;
    virtual bool isAvailable() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getName() const = 0;
    
    // Health check
    virtual bool healthCheck() const { return isAvailable(); }
    
    // Get connection info
    virtual JsonValue getConnectionInfo() const = 0;
};

// REST API data source implementation
class RestApiDataSource : public DataSource {
private:
    std::string base_url_;
    std::map<std::string, std::string> headers_;
    int timeout_seconds_;
    int max_retries_;
    std::string auth_token_;
    
public:
    RestApiDataSource(const JsonValue& config);
    
    JsonValue execute(const std::string& operation, 
                     const JsonValue& parameters) override;
    bool isAvailable() const override;
    std::string getType() const override { return "rest_api"; }
    std::string getName() const override;
    JsonValue getConnectionInfo() const override;
    
private:
    JsonValue makeHttpRequest(const std::string& method, 
                             const std::string& endpoint,
                             const JsonValue& data) const;
    void applyRateLimiting() const;
};

// Database data source implementation
class DatabaseDataSource : public DataSource {
private:
    std::string connection_string_;
    int connection_timeout_;
    int query_timeout_;
    
public:
    DatabaseDataSource(const JsonValue& config);
    
    JsonValue execute(const std::string& operation, 
                     const JsonValue& parameters) override;
    bool isAvailable() const override;
    std::string getType() const override { return "database"; }
    std::string getName() const override;
    JsonValue getConnectionInfo() const override;
    
private:
    JsonValue executeQuery(const std::string& query, 
                          const std::vector<std::string>& params) const;
};

// File system data source implementation
class FileSystemDataSource : public DataSource {
private:
    std::string base_path_;
    std::vector<std::string> supported_formats_;
    
public:
    FileSystemDataSource(const JsonValue& config);
    
    JsonValue execute(const std::string& operation, 
                     const JsonValue& parameters) override;
    bool isAvailable() const override;
    std::string getType() const override { return "file_system"; }
    std::string getName() const override;
    JsonValue getConnectionInfo() const override;
    
private:
    JsonValue readFile(const std::string& filepath) const;
    bool writeFile(const std::string& filepath, const JsonValue& data) const;
    std::vector<std::string> listFiles(const std::string& pattern) const;
};

// Cache data source implementation
class CacheDataSource : public DataSource {
private:
    std::string cache_path_;
    int ttl_seconds_;
    size_t max_size_bytes_;
    
public:
    CacheDataSource(const JsonValue& config);
    
    JsonValue execute(const std::string& operation, 
                     const JsonValue& parameters) override;
    bool isAvailable() const override;
    std::string getType() const override { return "cache"; }
    std::string getName() const override;
    JsonValue getConnectionInfo() const override;
    
private:
    std::string generateCacheKey(const std::string& operation, 
                                const JsonValue& parameters) const;
    bool isCacheValid(const std::string& cache_file) const;
    void cleanupExpiredEntries() const;
};

// Data processor for format transformations
class DataProcessor {
public:
    virtual ~DataProcessor() = default;
    virtual JsonValue process(const JsonValue& input, 
                             const JsonValue& config) const = 0;
    virtual std::string getType() const = 0;
};

// VCF annotation processor
class VcfAnnotationProcessor : public DataProcessor {
public:
    JsonValue process(const JsonValue& input, 
                     const JsonValue& config) const override;
    std::string getType() const override { return "vcf_annotator"; }
    
private:
    JsonValue annotateVariant(const JsonValue& variant, 
                             const std::vector<std::string>& sources) const;
    JsonValue filterVariants(const JsonValue& variants, 
                            const std::string& criteria) const;
};

// Expression data normalizer
class ExpressionNormalizerProcessor : public DataProcessor {
public:
    JsonValue process(const JsonValue& input, 
                     const JsonValue& config) const override;
    std::string getType() const override { return "expression_normalizer"; }
    
private:
    JsonValue normalizeExpression(const JsonValue& data, 
                                 const std::string& method) const;
    JsonValue qualityControl(const JsonValue& data, 
                            const JsonValue& filters) const;
};

// Workflow operation types
enum class OperationType {
    ENDPOINT_CALL,
    CUSTOM_PROCESSOR,
    MERGE,
    FILTER,
    TRANSFORM
};

// Individual workflow operation
struct WorkflowOperation {
    std::string name;
    OperationType type;
    std::string endpoint;
    std::string data_source;
    std::string processor;
    JsonValue parameters;
    std::string output_key;
    JsonValue cache_config;
    JsonValue fallback_config;
    std::string condition;
    int timeout_seconds;
    
    WorkflowOperation() : type(OperationType::ENDPOINT_CALL), timeout_seconds(30) {}
};

// Workflow step containing multiple operations
struct WorkflowStep {
    std::string name;
    std::string execution_type; // "sequential", "parallel", "conditional"
    std::string condition;
    std::vector<WorkflowOperation> operations;
    JsonValue error_handling;
};

// Complete workflow definition
struct Workflow {
    std::string name;
    std::string description;
    JsonValue input_schema;
    JsonValue output_schema;
    std::vector<WorkflowStep> steps;
    JsonValue error_handling;
    int global_timeout;
    
    Workflow() : global_timeout(300) {}
};

// Workflow execution context
class WorkflowContext {
private:
    std::map<std::string, JsonValue> variables_;
    std::map<std::string, JsonValue> outputs_;
    JsonValue input_;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;
    
public:
    void setInput(const JsonValue& input) { input_ = input; }
    JsonValue getInput() const { return input_; }
    
    void setVariable(const std::string& key, const JsonValue& value) {
        variables_[key] = value;
    }
    JsonValue getVariable(const std::string& key) const;
    
    void setOutput(const std::string& key, const JsonValue& value) {
        outputs_[key] = value;
    }
    JsonValue getOutput(const std::string& key) const;
    JsonValue getAllOutputs() const;
    
    void addError(const std::string& error) { errors_.push_back(error); }
    void addWarning(const std::string& warning) { warnings_.push_back(warning); }
    
    std::vector<std::string> getErrors() const { return errors_; }
    std::vector<std::string> getWarnings() const { return warnings_; }
    bool hasErrors() const { return !errors_.empty(); }
};

// Main configuration manager
class ConfigurationManager {
private:
    JsonValue config_;
    std::map<std::string, std::unique_ptr<DataSource>> data_sources_;
    std::map<std::string, std::unique_ptr<DataProcessor>> processors_;
    std::map<std::string, Workflow> workflows_;
    JsonValue parameter_templates_;
    JsonValue validation_rules_;
    
public:
    ConfigurationManager();
    ~ConfigurationManager();
    
    // Configuration loading
    bool loadConfiguration(const std::string& config_path);
    bool loadConfigurationFromJson(const JsonValue& config);
    
    // Data source management
    bool registerDataSource(const std::string& name, 
                           std::unique_ptr<DataSource> source);
    DataSource* getDataSource(const std::string& name) const;
    std::vector<std::string> getAvailableDataSources() const;
    
    // Processor management
    bool registerProcessor(const std::string& name, 
                          std::unique_ptr<DataProcessor> processor);
    DataProcessor* getProcessor(const std::string& name) const;
    
    // Parameter resolution and validation
    JsonValue resolveParameters(const std::string& endpoint, 
                               const JsonValue& input_params) const;
    bool validateRequest(const std::string& endpoint, 
                        const JsonValue& parameters) const;
    JsonValue transformParameters(const JsonValue& params, 
                                 const JsonValue& template_def) const;
    
    // Workflow management
    bool loadWorkflow(const std::string& name, const JsonValue& workflow_def);
    Workflow getWorkflow(const std::string& name) const;
    std::vector<std::string> getAvailableWorkflows() const;
    
    // Validation
    bool validateWorkflow(const Workflow& workflow) const;
    std::vector<std::string> getValidationErrors(const std::string& endpoint,
                                                const JsonValue& params) const;
    
private:
    void initializeBuiltinProcessors();
    void initializeDataSources();
    bool validateParameterTemplate(const JsonValue& template_def) const;
};

// Workflow execution engine
class WorkflowEngine {
private:
    ConfigurationManager* config_manager_;
    std::map<std::string, JsonValue> cache_;
    
public:
    WorkflowEngine(ConfigurationManager* config_manager);
    
    // Workflow execution
    JsonValue executeWorkflow(const std::string& workflow_name, 
                             const JsonValue& input);
    JsonValue executeWorkflowStep(const WorkflowStep& step, 
                                 WorkflowContext& context);
    JsonValue executeOperation(const WorkflowOperation& operation, 
                              WorkflowContext& context);
    
    // Execution modes
    JsonValue executeSequential(const std::vector<WorkflowOperation>& operations,
                               WorkflowContext& context);
    JsonValue executeParallel(const std::vector<WorkflowOperation>& operations,
                             WorkflowContext& context);
    JsonValue executeConditional(const std::vector<WorkflowOperation>& operations,
                                const std::string& condition,
                                WorkflowContext& context);
    
    // Error handling
    JsonValue handleOperationError(const WorkflowOperation& operation,
                                  const std::string& error,
                                  WorkflowContext& context);
    
    // Caching
    void setCacheValue(const std::string& key, const JsonValue& value);
    JsonValue getCacheValue(const std::string& key) const;
    bool hasCacheValue(const std::string& key) const;
    void clearCache();
    
private:
    std::string generateCacheKey(const WorkflowOperation& operation,
                                const JsonValue& resolved_params) const;
    bool shouldUseCache(const WorkflowOperation& operation) const;
    JsonValue applyFallback(const WorkflowOperation& operation,
                           WorkflowContext& context) const;
};

// Utility functions for template resolution
namespace TemplateUtils {
    std::string resolveEnvironmentVariable(const std::string& var_name);
    std::string resolveConfigVariable(const std::string& var_name, 
                                     const JsonValue& config);
    std::string resolveCalculation(const std::string& expression);
    std::vector<std::string> extractTemplateVariables(const std::string& template_str);
    bool isTemplateString(const std::string& str);
}

// Validation utilities
namespace ValidationUtils {
    bool validateJsonSchema(const JsonValue& data, const JsonValue& schema);
    bool validateRegexPattern(const std::string& value, const std::string& pattern);
    bool validateRange(double value, double min, double max);
    bool validateEnum(const std::string& value, const std::vector<std::string>& allowed);
    std::vector<std::string> getValidationErrors(const JsonValue& data, 
                                                const JsonValue& schema);
}

#endif // FLEXIBLE_JSON_LOGIC_H
