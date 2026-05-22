#include "cache_manager.h"
#include "../api/api_logic.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

CacheManager::CacheManager(const std::string& cache_dir) : cache_dir(cache_dir) {
    ensure_cache_dir();
}

bool CacheManager::has(const std::string& key) const {
    return fs::exists(get_cache_path(key));
}

JsonValue CacheManager::get(const std::string& key) const {
    if (!has(key)) return JsonValue::makeNull();
    return load_from_file(get_cache_path(key));
}

void CacheManager::set(const std::string& key, const JsonValue& value) {
    save_to_file(get_cache_path(key), value);
}

void CacheManager::clear() {
    for (const auto& entry : fs::directory_iterator(cache_dir)) {
        fs::remove(entry.path());
    }
}

std::string CacheManager::get_cache_path(const std::string& key) const {
    return cache_dir + "/" + key + ".json";
}

void CacheManager::ensure_cache_dir() {
    if (!fs::exists(cache_dir)) {
        fs::create_directories(cache_dir);
    }
}
