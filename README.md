# KV Store with Sharding and Replication

This project is a small distributed key-value store implemented in C++ using TCP sockets, a consistent-hash router, per-shard storage, and a simple replica model. It is designed as a compact demonstration of how a sharded key-value system can route requests, persist data, and replicate writes to a secondary node.

## Overview

The system is composed of three main layers:

- A client-facing router that accepts commands and routes them to the correct shard
- Multiple shard servers that store and serve key-value data
- Per-shard replica servers that hold a copy of the primary data for replication

The application uses a consistent hash ring to decide which shard owns a given key, which makes the distribution stable as the number of shards changes.

## High-level architecture

```text
                 +----------------------+
                 |      Client          |
                 |  SET/GET/REMOVE     |
                 +----------+-----------+
                            |
                            v
                 +----------------------+
                 |      Router          |
                 |  port 8070          |
                 |  uses hash ring     |
                 +----+---------+------+
                      |
                      | hash(key)
                      v
        +---------------------------+---------------------------+
        |                           |                           |
        v                           v                           v
 +---------------+          +---------------+          +---------------+
 | Shard 0       |          | Shard 1       |          | Shard 2       |
 | port 8080     |          | port 8081     |          | port 8082     |
 | KVStore       |          | KVStore       |          | KVStore       |
 +-------+-------+          +-------+-------+          +-------+-------+
         |                              |                              |
         | replica write                | replica write                | replica write
         v                              v                              v
 +---------------+          +---------------+          +---------------+
 | Replica 0     |          | Replica 1     |          | Replica 2     |
 | port 8090     |          | port 8091     |          | port 8092     |
 | KVStore       |          | KVStore       |          | KVStore       |
 +---------------+          +---------------+          +---------------+
```

## Sharding design

The router is implemented in [src/server/router.cpp](src/server/router.cpp) and uses the consistent hash ring defined in [include/server/consistent_hash.h](include/server/consistent_hash.h).

### Consistent hash ring

The hash ring stores virtual nodes for each shard. Each shard is inserted into the ring multiple times using a virtual-node pattern:

- `VIRTUAL_NODES = 100`
- For each shard and each virtual node, a hash is computed from 

### Routing behavior

The router handles both key-based commands and multi-shard commands:

- `SET`, `GET`, `REMOVE`, `EXISTS`:
  - route to the single shard chosen by the hash of the key
- `SIZE`:
  - query every shard and sum the result
- `CLEAR`:
  - broadcast the clear command to all shards

This routing logic is in [src/server/router.cpp](src/server/router.cpp).

## Replication design

Each primary shard server is paired with a replica server, and every mutation is forwarded to the replica before the write is considered successful.

### Primary server flow

Each shard server is created by a process such as:

- [src/main_server1.cpp](src/main_server1.cpp)
- [src/main_server2.cpp](src/main_server2.cpp)
- [src/main_server3.cpp](src/main_server3.cpp)

These create a `KVStore`, connect to their respective replica, and start a `TCPServer` on ports 8080, 8081, and 8082.

### Replica client

The `Executor` class in [include/server/executor.h](include/server/executor.h) contains a `ReplicaClient*` pointer. On `SET`, `REMOVE`, and `CLEAR`, the executor executes the mutation locally and then forwards the same operation to the replica.

The actual replication calls happen in [src/server/executor.cpp](src/server/executor.cpp):

- `SET` -> `replica_client_->set(key, value)`
- `REMOVE` -> `replica_client_->remove(key)`
- `CLEAR` -> `replica_client_->clear()`

If the replica does not respond correctly, the executor returns an error such as:

```text
ERROR Replica failed
```

### Replica server

Each replica is a separate TCP process:

- [src/main_replica1.cpp](src/main_replica1.cpp)
- [src/main_replica2.cpp](src/main_replica2.cpp)
- [src/main_replica3.cpp](src/main_replica3.cpp)

These run a `KVStore` with their own local snapshot and WAL files, and accept commands from the corresponding shard via `ReplicaClient`.

## Storage and durability

The data layer is implemented in [include/kv/kv_store.h](include/kv/kv_store.h) and [src/kv/kv_store.cpp](src/kv/kv_store.cpp).

### In-memory store

The main data is kept in a `std::unordered_map<std::string, std::string>`.

### Write-ahead log (WAL)

The `WAL` component records mutations before they are applied to the map. This is used to recover data after restart.

- `logSet(...)`
- `logRemove(...)`
- `logClear()`
- `recover(...)`

### Snapshotting

The `Snapshot` component writes the full map to disk and allows the system to restore a previously persisted state.

This is useful for startup recovery, where the store loads:

1. snapshot data
2. WAL operations after the snapshot
3. a fresh snapshot after recovery

## Command format

The parser in [src/parser/command_parser.cpp](src/parser/command_parser.cpp) supports the following commands:

```text
SET key value
GET key
REMOVE key
EXISTS key
SIZE
CLEAR
```

Examples:

```text
SET user:1 Alice
GET user:1
REMOVE user:1
SIZE
CLEAR
```

## Project structure

```text
.
├── CMakeLists.txt
├── compile-project.sh
├── README.md
├── include/
│   ├── kv/
│   │   ├── kv_store.h
│   │   ├── snapshot.h
│   │   └── wal.h
│   ├── parser/
│   │   └── command_parser.h
│   └── server/
│       ├── consistent_hash.h
│       ├── executor.h
│       ├── replica_client.h
│       ├── router.h
│       ├── tcp_client.h
│       └── tcp_server.h
├── src/
│   ├── kv/
│   │   ├── kv_store.cpp
│   │   ├── snapshot.cpp
│   │   └── wal.cpp
│   ├── parser/
│   │   └── command_parser.cpp
│   ├── server/
│   │   ├── consistent_hash.cpp
│   │   ├── executor.cpp
│   │   ├── replica_client.cpp
│   │   ├── router.cpp
│   │   ├── tcp_client.cpp
│   │   └── tcp_server.cpp
│   ├── main.cpp
│   ├── main_server1.cpp
│   ├── main_server2.cpp
│   ├── main_server3.cpp
│   ├── main_replica1.cpp
│   ├── main_replica2.cpp
│   └── main_replica3.cpp
├── tests/
│   ├── command_parser_test.cpp
│   ├── kv_store_test.cpp
│   ├── snapshot_test.cpp
│   └── wal_test.cpp
├── build/
└── external/
```

## How to build and run

Build the project:

```bash
./compile-project.bat (for windows)
./compile-project.sh (for linux)
```

Start the replica processes:

```bash
./build/kv-store-replica1
./build/kv-store-replica2
./build/kv-store-replica3
```

Start the primary shard servers:

```bash
./build/kv-store-server1
./build/kv-store-server2
./build/kv-store-server3
```

Start the router:

```bash
./build/kv-store
```
If you are using vs code, in the .vscode file there are tasks available

Then connect to the router on port `8070` and send commands.

## Notes

This project has a lot of scope for improvement, like string_view can be added, some repeating lines could be
converted to the functions, the replica could be used to improve performance(right now it is only for data loss prevention).

This project is tested for running on distributed system as per the architechture above, and can be deployed.