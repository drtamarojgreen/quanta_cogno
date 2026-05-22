#ifndef CACHE_MANAGER_H
#define CACHE_MANAGER_H

#include "json_logic.h"
#include <string>
#include <map>

class CacheManager {
public:
    CacheManager(const std::string& cache_dir = "./cache");
    
    bool has(const std::string& key) const;
    JsonValue get(const std::string& key) const;
    void set(const std::string& key, const JsonValue& value);
    void clear();

private:
    std::string cache_dir;
    std::string get_cache_path(const std::string& key) const;
    void ensure_cache_dir();
};

#endif // CACHE_MANAGER_H
