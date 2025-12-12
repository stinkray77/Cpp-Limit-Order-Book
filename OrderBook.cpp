 #include "OrderBook.h" // Link to header
 
void OrderBook::print_book() {
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
void OrderBook::add_order(int id, bool is_buy, int price, int quantity, OrderType type) {
    // If is market order, willing to pay anything to buy anything
    if (type == OrderType::MARKET) {
        if (is_buy) price = std::numeric_limits<int>::max();
        else        price = 0;
    }
    
    // normal order
    Order new_order = {id, type, is_buy, price, quantity};

    if (is_buy) {
        bids[price].push_back(new_order); // O(log N) insertion
    } else {
        asks[price].push_back(new_order); // O(log N) insertion
    }

    // try to match immediately
    match_orders(); // Check if a trade can happen immediately

    // clean up if not fully filled
    if (type == OrderType::MARKET) {
        if (is_buy) {
            auto it = bids.find(price);
            if (it != bids.end()) {
                // assume last added 
                bids.erase(it);
            }
        } else {
            // same for sells
            auto it = asks.find(price);
            if (it != asks.end()) {
                asks.erase(it);
            }
        }
    }
}

// The "Engine" Logic
void OrderBook::match_orders() {
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
