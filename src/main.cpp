#include <iostream>
#include "kv/kv_store.h"
int main()
{   
    std::cout << "KV Store Started !!" << std::endl;
    KVStore store("test_snapshot.txt", "test_wal.txt");
    // store.set("name", "Vishal");
    // store.set("age", "22");
    // store.set("city", "Noida");
    // store.saveSnapshot();
    // store.set("country", "India");
    // store.set("name", "Vishal Kumar");
    auto value = store.get("country");
    if (value.has_value())
    {
        std::cout << "Country: " << value.value() << std::endl;
    }
    else
    {
        std::cout << "Country not found!" << std::endl;
    }
    auto name_value = store.get("name");
    if (name_value.has_value())
    {
        std::cout << "Name: " << name_value.value() << std::endl;
    }
    else
    {
        std::cout << "Name not found!" << std::endl;
    }
    auto size = store.size();
    std::cout << "Size of the store: " << size << std::endl;
    return 0;
}