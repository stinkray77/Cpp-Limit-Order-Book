#pragma once // Prevents the file from being included twice
#include <map>
#include <vector>
#include <iostream>

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
        void add_order(int id, bool is_buy, int price, int quantity);
        void match_orders();
        void print_book();
};