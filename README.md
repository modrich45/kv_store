# KV Store

A production-style in-memory key-value store built in Modern C++20.

## Features (Planned)

- In-memory storage
- Persistence (WAL)
- Thread safety
- TCP Server
- Replication
- Sharding

## Format

- **Snapshot**
    key=value

- **WAL**
    SET key value
    REMOVE key
    CLEAR