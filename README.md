# KV Store

A production-style, in-memory distributed key-value store built from scratch in **Modern C++20**.

The project started as a simple in-memory key-value store and has evolved into a distributed system with **TCP networking, replication, and fault-tolerance capabilities**.

The goal is to understand and implement the core ideas behind systems such as Redis, Dynamo-style distributed stores, and other distributed key-value databases — without relying on external database frameworks.

---

## 🚀 Current Status

Core KV Store          ✅
Command Parser         ✅
Command Executor       ✅
Thread Safety          ✅
TCP Networking         ✅
WAL                    ✅
Snapshots              ✅
Replication            ✅
Sharding               🚧
Failure Detection      🚧
Automatic Failover     🚧
Performance Tuning     🚧

---

## ✨ Features

### In-Memory Storage

The core storage engine provides fast key-value operations directly in memory.

Supported operations include:

```text
SET key value
GET key
DELETE key
CLEAR
EXISTS key
SIZE
```

The storage layer is designed to be independent from the networking layer, making the system easier to extend and test.

---

### 🧵 Thread Safety

The store supports concurrent client access using synchronization primitives provided by modern C++.

The goal is to allow multiple clients to interact with the store safely without corrupting shared state.

---

### 🌐 TCP Server

The KV store exposes a TCP-based server allowing external clients to communicate with the database.

Architecture:

```text
Client
   │
   │ TCP
   ▼
┌───────────────┐
│   TCP Server  │
└───────┬───────┘
        │
        ▼
┌───────────────┐
│ Command Parser│
└───────┬───────┘
        │
        ▼
┌───────────────┐
│Command Executor│
└───────┬───────┘
        │
        ▼
┌───────────────┐
│    KV Store   │
└───────────────┘
```

Multiple clients can connect to the server concurrently.

---

### 🔁 Replication

The system currently supports **primary-replica replication**.

Writes received by the primary node are propagated to replica nodes.

```text
                ┌───────────────┐
                │    Client     │
                └───────┬───────┘
                        │
                        ▼
               ┌─────────────────┐
               │  Primary Node   │
               └───────┬─────────┘
                       │
              ┌────────┴────────┐
              │                 │
              ▼                 ▼
       ┌─────────────┐   ┌─────────────┐
       │   Replica 1 │   │   Replica 2 │
       └─────────────┘   └─────────────┘
```

Replication allows data written to one server to become available on other nodes.

This provides the foundation for **higher availability and fault tolerance** as the system evolves.

---

## 🏗️ Architecture

The project is structured into separate components:

```text
                    ┌──────────────┐
                    │    Client    │
                    └──────┬───────┘
                           │
                           │ TCP
                           ▼
                 ┌───────────────────┐
                 │    TCP Server     │
                 └─────────┬─────────┘
                           │
                           ▼
                 ┌───────────────────┐
                 │  Command Parser   │
                 └─────────┬─────────┘
                           │
                           ▼
                 ┌───────────────────┐
                 │ Command Executor  │
                 └─────────┬─────────┘
                           │
                           ▼
                 ┌───────────────────┐
                 │     KV Store      │
                 └─────────┬─────────┘
                           │
                           │ Replication
                           ▼
              ┌──────────────────────────┐
              │       Replica Nodes      │
              ├────────────┬─────────────┤
              │            │             │
              ▼            ▼             ▼
          Replica 1    Replica 2    Replica N
```

---

## 📂 Project Structure

```text
KV-Store/
│
├── external/
│   └── googletest/
│
├── include/
│   └── kv/
│       ├── command_executor.h
│       ├── command_parser.h
│       ├── kv_store.h
│       ├── replication_manager.h
│       ├── snapshot.h
│       └── WAL.h
│
├── src/
│   └── kv/
│       ├── command_executor.cpp
│       ├── command_parser.cpp
│       ├── kv_store.cpp
│       ├── replication_manager.cpp
│       ├── snapshot.cpp
│       └── WAL.cpp
│
├── tests/
│   ├── command_parser_test.cpp
│   ├── kv_store_test.cpp
│   ├── snapshot_test.cpp
│   └── WAL_test.cpp
│
├── main.cpp
├── TCPClient.cpp
├── TCPServer.cpp
│
├── .gitignore
├── .gitmodules
├── CMakeLists.txt
└── README.md
```
| Directory / File | Purpose                                         |
| ---------------- | ----------------------------------------------- |
| `include/kv/`    | Header files for the core KV store components   |
| `src/kv/`        | Implementations of the core KV store components |
| `tests/`         | Unit tests using GoogleTest                     |
| `external/`      | External dependencies such as GoogleTest        |
| `TCPServer.cpp`  | TCP server implementation                       |
| `TCPClient.cpp`  | TCP client implementation                       |
| `main.cpp`       | Application entry point                         |
| `CMakeLists.txt` | Build configuration                             |


> The exact structure may evolve as additional distributed-system components are introduced.

---

## 🛠️ Tech Stack

* **C++20**
* **STL**
* **Winsock2** for TCP networking
* **CMake**
* Multithreading
* Synchronization primitives
* TCP sockets

---

## ▶️ Running the Project

### Requirements

* C++20 compatible compiler
* CMake
* Windows environment currently, due to the Winsock networking implementation

### Build

```bash
git clone <repository-url>
cd KV-Store

mkdir build
cd build

cmake ..
cmake --build .
```

### Start a Server

Run the server executable:

```bash
server.exe
```

### Connect Using a Client

Start the client and connect to the server:

```text
Client → TCP → Server
```

You can then execute commands such as:

```text
SET city Gurgaon
GET city
DELETE city
```

With replication enabled, the data written to the primary can also be observed from the replica nodes.

---

## 🧪 Example

### Client

```text
> SET city Gurgaon
OK

> GET city
Gurgaon

> DELETE city
OK

> GET city
NULL
```

### Replication

```text
Client
  │
  │ SET city Gurgaon
  ▼
Primary
  │
  ├──────────────► Replica 1
  │                  city = Gurgaon
  │
  └──────────────► Replica 2
                     city = Gurgaon
```

---

## 🎯 Design Goals

This project is being built with a focus on understanding **distributed systems from first principles**.

The major design goals are:

* Keep the storage engine simple and fast
* Separate storage, command processing, and networking
* Support concurrent clients
* Build networking from raw TCP sockets
* Understand replication and distributed state
* Improve availability and fault tolerance
* Avoid unnecessary external dependencies
* Gradually evolve the system toward a production-style architecture

---

## 🔬 What I'm Learning

This project is primarily a hands-on exploration of:

* Data structures and memory management
* Modern C++ and concurrency
* TCP/IP networking
* Client-server architecture
* Distributed systems
* Replication
* Fault tolerance
* Sharding and partitioning
* Consistency vs availability
* Performance engineering

---

## 📈 Long-Term Vision

The eventual goal is to evolve the project into a more complete distributed key-value database supporting:

```text
                    ┌──────────────┐
                    │    Clients   │
                    └───────┬──────┘
                            │
                            ▼
                  ┌──────────────────┐
                  │ Request Routing  │
                  └────────┬─────────┘
                           │
              ┌────────────┴────────────┐
              │                         │
              ▼                         ▼
        ┌───────────┐             ┌───────────┐
        │  Shard 1  │             │  Shard 2  │
        │           │             │           │
        │ Primary   │             │ Primary   │
        │    │      │             │    │      │
        │  ┌─┴─┐    │             │  ┌─┴─┐    │
        │  R1 R2     │             │  R1 R2    │
        └───────────┘             └───────────┘
```

Future work will focus on **sharding, durability, failure recovery, automatic failover, and performance optimization**.

---

## 📌 Project Status

This is an actively evolving project.

The current implementation has progressed from a local in-memory store to a **networked replicated key-value store**, with distributed storage and reliability improvements planned for the next stages.

---

## 👨‍💻 Author

**Vishal Kumar**

Built from scratch in C++20 as a hands-on exploration of systems programming and distributed systems.
