# JSON Flexibility Analysis and Recommendations

## Current State Analysis

After reviewing the project documentation and JSON configurations, I've identified several areas where the JSON structure can be made more flexible to handle different scenarios of data access, integration, preparation, and analysis.

### Current JSON Structure Issues

1. **Rigid Schema Design**: The current `cognos.schema.json` is tightly coupled to LlamaCpp-specific parameters
2. **Limited Data Source Support**: No abstraction for different data sources (APIs, databases, files, streams)
3. **Hardcoded Parameter Names**: Inconsistent naming conventions across endpoints (e.g., `gene`, `geneId`, `gene_ids`)
4. **Static Configuration**: No support for dynamic parameter resolution or conditional logic
5. **Single-Purpose Schemas**: Each JSON serves only one specific use case

## Proposed Flexible JSON Architecture

### 1. Data Source Abstraction Layer

Create a flexible data source configuration that can handle multiple types of data access:

```json
{
  "data_sources": {
    "genomics_api": {
      "type": "rest_api",
      "base_url": "https://api.genomics.example.com",
      "authentication": {
        "type": "bearer_token",
        "token_source": "${ENV:API_TOKEN}"
      },
      "rate_limiting": {
        "requests_per_minute": 100,
        "burst_limit": 10
      }
    },
    "local_database": {
      "type": "sqlite",
      "connection_string": "./data/genomics.db",
      "connection_pool": {
        "min_connections": 1,
        "max_connections": 5
      }
    },
    "file_system": {
      "type": "file",
      "base_path": "./data",
      "supported_formats": ["json", "csv", "tsv", "vcf"]
    },
    "streaming_data": {
      "type": "kafka",
      "brokers": ["localhost:9092"],
      "topics": ["genomics-updates", "research-publications"]
    }
  }
}
```

### 2. Flexible Parameter Resolution System

Implement a parameter system that supports:
- Environment variable substitution
- Dynamic value resolution
- Conditional parameters
- Parameter validation and transformation

```json
{
  "parameter_templates": {
    "gene_search": {
      "gene_identifiers": {
        "type": "array",
        "items": {
          "oneOf": [
            {"type": "string", "pattern": "^[A-Z0-9]+$"},
            {"type": "string", "pattern": "^rs[0-9]+$"},
            {"type": "string", "pattern": "^chr[0-9XY]+:[0-9]+-[0-9]+$"}
          ]
        },
        "aliases": ["gene", "geneId", "gene_ids", "symbols"],
        "validation": {
          "min_items": 1,
          "max_items": 100
        }
      },
      "confidence_level": {
        "type": "string",
        "enum": ["high", "medium", "low", "all"],
        "default": "${CONFIG:default_confidence_level}",
        "transform": "lowercase"
      }
    }
  }
}
```

### 3. Workflow Configuration System

Create a workflow-based approach that can chain different operations:

```json
{
  "workflows": {
    "mental_health_analysis": {
      "description": "Complete mental health genomics analysis pipeline",
      "steps": [
        {
          "name": "data_acquisition",
          "type": "parallel",
          "operations": [
            {
              "endpoint": "getMentalHealthGenes",
              "data_source": "genomics_api",
              "parameters": {
                "condition": "${INPUT:condition}",
                "confidence_level": "high"
              },
              "output_key": "candidate_genes"
            },
            {
              "endpoint": "getResearchAssociations",
              "data_source": "local_database",
              "parameters": {
                "mental_health_terms": ["${INPUT:condition}"],
                "publication_years": {
                  "start_year": "${CALC:current_year - 5}",
                  "end_year": "${CALC:current_year}"
                }
              },
              "output_key": "recent_research"
            }
          ]
        },
        {
          "name": "data_integration",
          "type": "sequential",
          "operations": [
            {
              "type": "merge",
              "inputs": ["candidate_genes", "recent_research"],
              "merge_strategy": "inner_join",
              "join_key": "gene_id",
              "output_key": "integrated_data"
            }
          ]
        },
        {
          "name": "analysis",
          "type": "conditional",
          "condition": "${LENGTH:integrated_data} > 0",
          "operations": [
            {
              "endpoint": "getPathwayAnalysis",
              "parameters": {
                "gene_list": "${EXTRACT:integrated_data.gene_id}",
                "mental_health_focus": true,
                "significance_threshold": 0.01
              },
              "output_key": "pathway_results"
            }
          ]
        }
      ],
      "error_handling": {
        "retry_policy": {
          "max_retries": 3,
          "backoff_strategy": "exponential"
        },
        "fallback_operations": [
          {
            "condition": "data_source_unavailable",
            "action": "use_cached_data"
          }
        ]
      }
    }
  }
}
```

### 4. Dynamic Schema Validation

Implement context-aware schema validation:

```json
{
  "validation_rules": {
    "endpoint_specific": {
      "getMentalHealthGenes": {
        "required_if": {
          "condition": "at_least_one",
          "parameters": ["condition", "study_type", "population"]
        },
        "parameter_dependencies": {
          "include_drug_targets": {
            "requires": ["condition"],
            "conflicts_with": ["study_type"]
          }
        }
      },
      "getPolygeneticRiskScores": {
        "conditional_required": {
          "variant_data": {
            "required_when": "prs_method != 'population_average'"
          }
        }
      }
    },
    "data_quality": {
      "gene_identifiers": {
        "validation_endpoint": "validateGeneIds",
        "cache_duration": "24h"
      },
      "allele_frequency": {
        "range_validation": {
          "min_frequency": {"min": 0, "max": 1},
          "max_frequency": {"min": 0, "max": 1},
          "constraint": "min_frequency <= max_frequency"
        }
      }
    }
  }
}
```

### 5. Multi-Format Data Processing

Support for different data formats and transformations:

```json
{
  "data_processors": {
    "genomics_vcf": {
      "input_format": "vcf",
      "output_format": "json",
      "transformations": [
        {
          "type": "filter",
          "criteria": "QUAL > 30 AND DP > 10"
        },
        {
          "type": "annotate",
          "annotation_source": "dbSNP",
          "fields": ["rs_id", "clinical_significance"]
        }
      ]
    },
    "expression_matrix": {
      "input_format": "csv",
      "output_format": "json",
      "transformations": [
        {
          "type": "normalize",
          "method": "log2_tpm"
        },
        {
          "type": "filter_genes",
          "criteria": "mean_expression > 1"
        }
      ]
    }
  }
}
```

## Implementation Recommendations

### 1. Enhanced C++ JSON Logic

Extend the current `JsonValue` class to support:

```cpp
class FlexibleJsonValue : public JsonValue {
public:
    // Template resolution
    std::string resolveTemplate(const std::string& template_str, 
                               const std::map<std::string, std::string>& context);
    
    // Dynamic validation
    bool validateAgainstSchema(const JsonValue& schema);
    
    // Parameter transformation
    JsonValue transformParameters(const JsonValue& transformation_rules);
    
    // Conditional evaluation
    bool evaluateCondition(const std::string& condition, 
                          const std::map<std::string, JsonValue>& context);
};
```

### 2. Configuration Manager

Create a configuration management system:

```cpp
class ConfigurationManager {
private:
    std::map<std::string, JsonValue> schemas;
    std::map<std::string, JsonValue> workflows;
    std::map<std::string, DataSource*> data_sources;
    
public:
    bool loadConfiguration(const std::string& config_path);
    JsonValue resolveParameters(const std::string& endpoint, 
                               const JsonValue& input_params);
    std::vector<Operation> buildWorkflow(const std::string& workflow_name);
    bool validateRequest(const std::string& endpoint, 
                        const JsonValue& parameters);
};
```

### 3. Data Source Abstraction

Implement a plugin-based data source system:

```cpp
class DataSource {
public:
    virtual ~DataSource() = default;
    virtual JsonValue execute(const std::string& operation, 
                             const JsonValue& parameters) = 0;
    virtual bool isAvailable() const = 0;
    virtual std::string getType() const = 0;
};

class RestApiDataSource : public DataSource {
    // Implementation for REST API calls
};

class DatabaseDataSource : public DataSource {
    // Implementation for database queries
};

class FileSystemDataSource : public DataSource {
    // Implementation for file operations
};
```

## Benefits of This Approach

1. **Scalability**: Easy to add new data sources and endpoints
2. **Maintainability**: Centralized configuration management
3. **Flexibility**: Support for different data scenarios without code changes
4. **Robustness**: Built-in error handling and validation
5. **Performance**: Caching and optimization strategies
6. **Extensibility**: Plugin-based architecture for custom data sources

## Migration Strategy

1. **Phase 1**: Implement enhanced JSON parsing and validation
2. **Phase 2**: Create configuration manager and parameter resolution
3. **Phase 3**: Implement data source abstraction layer
4. **Phase 4**: Add workflow support and advanced features
5. **Phase 5**: Migrate existing endpoints to new flexible system

This flexible JSON architecture will enable the system to handle diverse data scenarios while maintaining backward compatibility and providing a foundation for future enhancements.
