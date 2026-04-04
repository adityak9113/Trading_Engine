# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

```bash
# Configure (first time or after CMakeLists changes)
cmake -S . -B build

# Build
cmake --build build

# Run
./build/Trading_Engine
```

## Architecture

This is an early-stage C++17 trading engine. Current structure:

- `include/orderbook/` — header-only domain types. `Order.h` defines the `Order` class and `Side` enum (BUY/SELL).
- `src/main.cpp` — entry point; currently contains a hardcoded demo that separates orders by side, finds best bid/ask, and checks for a trade match.
- `tests/` — reserved for tests (empty).

The intended direction is a proper order book: `Order` objects will be managed by an `OrderBook` class that maintains sorted BUY (descending price) and SELL (ascending price) queues and runs a matching engine.

## CMake conventions

`CMakeLists.txt` uses `file(GLOB_RECURSE SOURCES src/*.cpp)` — adding a new `.cpp` under `src/` is automatically picked up on the next `cmake --build build`. New header directories under `include/` need a corresponding `include_directories()` call if they are outside the already-included `include/` root.
