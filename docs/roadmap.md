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

## Phase 3
- [ ] Persistence
- [ ] Write-Ahead Log (WAL)
- [ ] Snapshotting

---

## Phase 4
- [ ] Thread Safety
- [ ] std::mutex
- [ ] std::shared_mutex
- [ ] Multi-threaded tests

---

## Phase 5
- [ ] TCP Server
- [ ] Client protocol
- [ ] Command parser

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