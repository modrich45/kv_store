#include <iostream>
#include "kv/kv_store.h"
int main()
{   
    std::cout << "KV Store Started !!" << std::endl;
    KVStore store("test_snapshot.txt","test_wal.txt");
    store.set("name", "Vishal");
    store.set("age", "30");
    store.set("city", "New York");
    store.saveSnapshot();
    auto value = store.get("name");
    if (value.has_value())
    {
        std::cout << "Name: " << value.value() << std::endl;
    }
    else
    {
        std::cout << "Name not found!" << std::endl;
    }
    auto size = store.size();
    std::cout << "Size of the store: " << size << std::endl;
    return 0;
}