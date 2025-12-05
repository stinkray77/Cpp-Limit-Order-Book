#include <iostream> // REQUIRED for printing (cout, cerr)
#include <fstream>  // Required for File I/O
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
    std::vector<Order> bids; // buy orders
    std::vector<Order> asks; // sell orders

public:
    // Function to add an order
    void add_order(int id, bool is_buy, int price, int quantity) {
        Order new_order = {id, is_buy, price, quantity};

        if (is_buy) {
            bids.push_back(new_order);
            // Sort bids: Highest price first (Descending)
            std::sort(bids.begin(), bids.end(), [](const Order& a, const Order& b) {
                return a.price > b.price;
            });
        } else {
            asks.push_back(new_order);
            // Sort asks: Lowest price first (Ascending)
            std::sort(asks.begin(), asks.end(), [](const Order&a, const Order&b) {
                return a.price < b.price;
            });
        }

        match_orders(); // Check if a trade can happen immediately
    }

    // The "Engine" Logic
    void match_orders() {
        // While we have both bids and asks, and the prices cross
        while (!bids.empty() && !asks.empty() && bids.front().price >= asks.front().price) {
            Order& best_bid = bids.front();
            Order& best_ask = asks.front();

            // Figure out how many we can trade (the minimum of the two)
            int trade_qty = std::min(best_bid.quantity, best_ask.quantity);

            std::cout << "TRADE EXECUTED: " << trade_qty << " shares at $"
                      << best_ask.price / 100.0 << "\n";

            // Update quantitites
            best_bid.quantity -= trade_qty;
            best_ask.quantity -= trade_qty;

            // Remove filled orders
            if (best_bid.quantity == 0) bids.erase(bids.begin());
            if (best_ask.quantity == 0) asks.erase(asks.begin());
        }
    }
};

int main() {
    OrderBook book;

    std::cout << "--- Market Open ---\n";
    
    // 1. Someone wants to sell 100 shares at $101
    std::cout << "[Order] Sell 100 @ $101\n";
    book.add_order(1, false, 10100, 100);

    // 2. Someone wants to buy 50 shares at $100 (No trade, price too low)
    std::cout << "[Order] Buy 50 @ $100\n";
    book.add_order(2, true, 10000, 50);

    // 3. Someone wants to buy 50 shares at $102 (AGGRESSIVE BUY!)
    // This should trigger a match against the $101 seller.
    std::cout << "[Order] Buy 50 @ $102\n";
    book.add_order(3, true, 10200, 50);

    return 0;
}