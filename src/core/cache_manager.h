#ifndef CACHE_MANAGER_V2_H
#define CACHE_MANAGER_V2_H

#include "../io/json_parser.h"
#include <string>
#include <filesystem>

namespace qc::core {

class CacheManager {
public:
    CacheManager(const std::string& cache_dir = "./cache");
    
    void set(const std::string& key, const io::JsonValue& value);
    std::optional<io::JsonValue> get(const std::string& key) const;
    void clear();

private:
    std::string cache_dir;
    std::string get_path(const std::string& key) const;
};

} // namespace qc::core

#endif // CACHE_MANAGER_V2_H
