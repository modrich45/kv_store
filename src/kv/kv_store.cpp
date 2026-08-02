#include <kv/kv_store.h>

KVStore::KVStore(const std::string& filename, const std:: string& wal_filename) : snapshot_(filename),wal_(wal_filename) {
    snapshot_.load(store_);
    wal_.recover(store_);
}

void KVStore::set(std::string key, std::string value)
{   
    if(key.empty()){
        return;
    }
    if(key.find('=') != std::string::npos){
        return;
    }
    if(key.find('\n') != std::string::npos){
        return;
    }
    if(value.empty()){
        return;
    }
    if(value.find('\n') != std::string::npos){
        return;
    }

    // need to handle key and values with spaces in wal.

    if(!wal_.logSet(key,value)) return;

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

bool KVStore::remove(const std::string& key)
{
    auto it = store_.find(key);

    if (it != store_.end())
    {
        if(!wal_.logRemove(key)) return false;
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
    if(!wal_.logClear()) return;

    store_.clear();
}

bool KVStore::saveSnapshot()
{
    if(!snapshot_.save(store_)) return false;

    return wal_.clear();
}
