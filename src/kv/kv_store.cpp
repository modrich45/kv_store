#include <kv/kv_store.h>

KVStore::KVStore(const std::string snapshot_filename, const std::string wal_filename) : snapshot_(snapshot_filename), wal_(wal_filename)
{
    snapshot_.load(store_);
    wal_.replay(store_);
    wal_.clear();
    snapshot_.save(store_);
}

void KVStore::set(std::string key, std::string value)
{
    if (key.empty())
    {
        return;
    }
    if (key.find(' ') != std::string::npos)
    {
        return;
    }
    if (key.find('=') != std::string::npos)
    {
        return;
    }
    if (key.find('\n') != std::string::npos)
    {
        return;
    }
    if (value.empty())
    {
        return;
    }
    if (value.find('\n') != std::string::npos)
    {
        return;
    }
    if (!wal_.logSet(key, value))
    {
        return;
    }
    std::unique_lock<std::shared_mutex> lock(mutex_);
    store_[key] = value;
}

std::optional<std::string> KVStore::get(std::string key) const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = store_.find(key);
    if (it != store_.end())
    {
        return it->second;
    }
    return std::nullopt;
}

bool KVStore::remove(const std::string &key)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    auto it = store_.find(key);

    if (!wal_.logRemove(key))
    {
        return false;
    }

    if (it != store_.end())
    {
        store_.erase(it);
        return true;
    }
    return false;
}

bool KVStore::exists(const std::string key) const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return store_.find(key) != store_.end();
}

std::size_t KVStore::size() const
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return store_.size();
}

void KVStore::clear()
{
    std::unique_lock<std::shared_mutex> lock(mutex_);
    if (!wal_.logClear())
    {
        return;
    }
    store_.clear();
}

bool KVStore::saveSnapshot()
{
    if (!snapshot_.save(store_))
        return false;

    if (!wal_.clear())
        return false;

    return true;
}
