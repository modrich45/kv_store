# KV Store Roadmap

## Phase 1 ✅
- [x] CMake setup
- [x] Basic project structure
- [x] In-memory KV Store
- [x] SET
- [x] GET
- [x] REMOVE
- [x] EXISTS
- [x] CLEAR

---

## Phase 2 ✅
- [x] Unit Testing (GoogleTest)

---

## Phase 3 ✅
- [x] Persistence
- [x] Write-Ahead Log (WAL)
- [x] Snapshotting

---

## Phase 4 ✅
────────────────────────────
[✓] Thread Safety
[✓] std::mutex
[✓] lock_guard
[✓] unique_lock
[✓] Race conditions
[✓] Data races
[✓] Critical sections
[✓] RAII
[✓] shared_mutex
[✓] shared_lock
[✓] Deadlocks
[✓] Multithreaded tests
[✓] KV Store integration
────────────────────────────
       COMPLETE ✅

---

## Phase 5 ✅
- [x] TCP Server
- [x] Client protocol
- [x] Command parser

                  TCP Client
                     │
                  send()
                     │
                     ▼
              ┌──────────────┐
              │  TCP Server  │
              └──────┬───────┘
                     │
                  accept()
                     │
          ┌──────────┼──────────┐
          ▼          ▼          ▼
       Thread A   Thread B   Thread C
          │          │          │
          └──────────┼──────────┘
                     ▼
                CommandParser
                     │
                     ▼
               CommandExecutor
                     │
                     ▼
                 KVStore
                     │
             ┌───────┴───────┐
             ▼               ▼
            WAL           Snapshot

---

## Phase 6 - Performance Optimizations

- [ ] Heterogeneous lookup
- [ ] TransparentHash
- [ ] TransparentEqual
- [ ] std::string_view lookup
- [ ] Custom allocator (optional)
- [ ] Benchmark against current implementation

---

## Phase 7
- [ ] Replication
- [ ] Sharding
- [ ] Consistent Hashing

## Deferred Decisions

- Use std::string instead of std::string_view for lookups (Phase 1 simplicity).
- Introduce heterogeneous lookup during performance optimization.