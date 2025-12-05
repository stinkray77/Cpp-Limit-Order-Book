# C++ Limit Order Book

A simulated high-frequency trading matching engine written in C++.

## Features
- **Price-Time Priority:** Matches orders based on best price, then arrival time.
- **Order Types:** Supports Limit Buy and Sell orders.
- **Execution:** Auto-matches crossing orders and updates the book state.

## Tech Stack
- **Language:** C++ (Standard 14/17)
- **Data Structures:** Vectors (Moving to Red-Black Trees for optimization)
- **Build System:** g++/clang++

## How to Run
```bash
g++ main.cpp -o engine
./engine
```

## Roadmap
[x] Basic Matching Logic (Vectors)
[ ] Optimization: Replace O(N log N) sorting with O(log N) Maps
[ ] Feature: File-based Order Ingestion (CSV)
[ ] Benchmark: Throughput latency testing
