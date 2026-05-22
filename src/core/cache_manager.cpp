#include "cache_manager.h"
#include <fstream>
#include <sstream>

namespace qc::core {

CacheManager::CacheManager(const std::string& cache_dir) : cache_dir(cache_dir) {
    std::filesystem::create_directories(cache_dir);
}

void CacheManager::set(const std::string& key, const io::JsonValue& value) {
    std::ofstream f(get_path(key));
    // Simple serialization (placeholder until io::JsonEmitter implemented)
    // For now, we'll just write a message or use a basic string builder
    // TODO: Implement JsonEmitter
    f << "{ \"status\": \"cached\" }"; 
}

std::optional<io::JsonValue> CacheManager::get(const std::string& key) const {
    std::string path = get_path(key);
    if (!std::filesystem::exists(path)) return std::nullopt;
    
    std::ifstream f(path);
    std::stringstream buffer;
    buffer << f.rdbuf();
    
    auto res = io::JsonParser::parse(buffer.str());
    if (std::holds_alternative<io::JsonValue>(res)) {
        return std::get<io::JsonValue>(res);
    }
    return std::nullopt;
}

void CacheManager::clear() {
    std::filesystem::remove_all(cache_dir);
    std::filesystem::create_directories(cache_dir);
}

std::string CacheManager::get_path(const std::string& key) const {
    return cache_dir + "/" + key + ".json";
}

} // namespace qc::core
