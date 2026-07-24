#include <kv/kv_store.h>

void KVStore::set(std::string key, std::string value)
{
    store_[key] = value;
}

std::optional<std::string> KVStore::get(std::string key) const
{
    auto it = store_.find(key);
    if (it != store_.end())
    {
        return it->second;
    }
    return std::nullopt;
}

bool KVStore::remove(const std::string key)
{
    auto it = store_.find(key);

    if (it != store_.end())
    {
        store_.erase(it);
        return true;
    }
    return false;
}

bool KVStore::exists(const std::string key) const
{
    return store_.find(key) != store_.end();
}

std::size_t KVStore::size() const
{
    return store_.size();
}

void KVStore::clear()
{
    store_.clear();
}
