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
- [x] Thread Safety
- [x] std::mutex
- [x] std::shared_mutex
- [x] Multi-threaded tests

---

## Phase 5
- [x] TCP Server
- [ ] Client protocol
- [ ] Command parser1

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