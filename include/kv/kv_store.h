#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include "snapshot.h"
#include "WAL.h"
#include <mutex>
#include <shared_mutex>

class KVStore {
private:
    std::unordered_map<std::string, std::string> store_;
    Snapshot snapshot_;
    WAL wal_;
    mutable std::shared_mutex mutex_;

public:

    explicit KVStore(const std::string snapshot_filename,const std::string wal_filename);

    void set(std::string key,std::string value);

    std::optional<std::string> get(std::string key) const;

    bool remove(const std::string& key);

    bool exists(std::string key) const;

    std::size_t size() const;

    void clear();

    bool saveSnapshot();
};