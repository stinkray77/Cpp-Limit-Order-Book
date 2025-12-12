#include <cassert> 
#include <iostream> // REQUIRED for printing (cout, cerr)
#include <fstream>  // Required for File I/O
#include <map>
#include <sstream>  // Required for splitting strings
#include <vector>   // Required for std::vector
#include <algorithm> // for sorting
#include <string>   // Required for std::string

struct Order {
    int id;             // Unique ID for the order
    bool is_buy;        // true = Buy (Bid), false = Sell (Ask)
    int price;          // In cents (e.g., $100.00 -> 10000)
    int quantity;       // Number of shares
};

class OrderBook {
    // Bids : High to Low (Standard map sorts Low to High, so I use std::greater)
    std::map<int, std::vector<Order>, std::greater<int>> bids;

    // Asks: Low to High (Default behavior)
    std::map<int, std::vector<Order>> asks;

public:

    void print_book() {
        std::cout << "\n--- ORDER BOOK ---\n";
        
        // Print Asks (Sells) - We want to see them from High to Low
        // Maps are sorted Low to High, so we iterate backwards (rbegin to rend)
        for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
            int price = it->first;
            int total_qty = 0;
            for (const auto& o : it->second) total_qty += o.quantity;
            std::cout << "ASK  $" << price / 100.0 << " | Qty: " << total_qty << "\n";
        }

        std::cout << "- - - - - - - - -\n";

        // Print Bids (Buys) - Maps are sorted High to Low (because of std::greater), so iterate normally
        for (auto it = bids.begin(); it != bids.end(); ++it) {
            int price = it->first;
            int total_qty = 0;
            for (const auto& o : it->second) total_qty += o.quantity;
            std::cout << "BID  $" << price / 100.0 << " | Qty: " << total_qty << "\n";
        }
        std::cout << "-----------------\n\n";
    }


    // Function to add an order
    void add_order(int id, bool is_buy, int price, int quantity) {
        Order new_order = {id, is_buy, price, quantity};

        if (is_buy) {
            bids[price].push_back(new_order); // O(log N) insertion
        } else {
            asks[price].push_back(new_order); // O(log N) insertion
        }

        match_orders(); // Check if a trade can happen immediately
    }

    // The "Engine" Logic
    void match_orders() {
        // While we have both bids and asks, and the prices cross
        while (!bids.empty() && !asks.empty()) {
            auto best_bid_iter = bids.begin();
            auto best_ask_iter = asks.begin();

            // Check if prices cross
            if (best_bid_iter->first < best_ask_iter->first) {
                break; // No trade possible
            }

            // Get the actual lists of orders at these prices
            std::vector<Order>& bid_list = best_bid_iter->second;
            std::vector<Order>& ask_list = best_ask_iter->second;

            Order& bidder = bid_list.front();
            Order& seller = ask_list.front();

            // Execution logic
            int trade_qty = std::min(bidder.quantity, seller.quantity);
            // printing this out is too slow
            // std::cout << "TRADE: " << trade_qty << " @ " << seller.price << "\n";

            bidder.quantity -= trade_qty;
            seller.quantity -= trade_qty;

            // Clean up empty orders
            if (bidder.quantity == 0) bid_list.erase(bid_list.begin());
            if (seller.quantity == 0) ask_list.erase(ask_list.begin());

            // Clean up empty price levels
            if (bid_list.empty()) bids.erase(best_bid_iter);
            if (ask_list.empty()) asks.erase(best_ask_iter);
        }
    }
};

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