#include <iostream>
#include "OrderBook.h"

int main() {
    OrderBook book;

    std::cout << "[Setup] Adding Orders...\n";
    book.add_order(1, true, 10000, 10, OrderType::LIMIT); // Buy @ $100
    book.add_order(2, true, 10000, 20, OrderType::LIMIT); // Buy @ $100
    book.add_order(3, true, 9900, 50, OrderType::LIMIT);  // Buy @ $99

    book.print_book(); 

    std::cout << "\n[Action] Cancelling Order #2 (The middle one)...\n";
    book.cancel_order(2);

    book.print_book(); 
    // EXPECTED: Order #1 (10 shares) and Order #3 (50 shares) remain. 
    // Order #2 is gone.

    return 0;
}