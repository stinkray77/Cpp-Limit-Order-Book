#include <cassert> 
#include <iostream> // REQUIRED for printing (cout, cerr)
#include <fstream>  // Required for File I/O
#include <sstream>
#include "OrderBook.h"

int main() {
    OrderBook book;
    std::string filename = "big_market_data.csv";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Run python script to generate " << filename << " first!\n";
        return 1;
    }

    std::cout << "Loading 1,000,000 orders... (Memory Ingestion)\n";

    // Pre-load data into memory so we measure Matching speed, not hardware speed
    struct RawOrder {int id; bool buy; int price; int qty; };
    std::vector<RawOrder> raw_data;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> row;
        while (std::getline(ss, segment, ',')) row.push_back(segment);

        // Convert string to int/bool
        raw_data.push_back({
            std::stoi(row[0]),
            row[1] == "1",
            std::stoi(row[2]),
            std::stoi(row[3])
        });
    }
    file.close();

    std::cout << "Data loaded. Starting Engine Benchmark...\n";

    // --- START TIMER --- 
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& o : raw_data) {
        book.add_order(o.id, o.buy, o.price, o.qty);
    }

    // --- STOP TIMER ---
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "------------------------------------------------\n";
    std::cout << "PROCESSED " << raw_data.size() << " ORDERS IN " << duration.count() << "ms\n";
    std::cout << "------------------------------------------------\n";

    return 0;
}