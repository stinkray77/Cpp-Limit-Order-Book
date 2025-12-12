#include <cassert> 
#include <iostream> // REQUIRED for printing (cout, cerr)
#include <fstream>  // Required for File I/O
#include <sstream>
#include "OrderBook.h"

int main() {
    OrderBook book;

    std::cout << "[Setup] 10 Sells @ $100, 20 Sells @ $105\n";
    book.add_order(1, false, 10000, 10, OrderType::LIMIT);
    book.add_order(2, false, 10500, 20, OrderType::LIMIT);
    
    book.print_book();

    std::cout << "\n[Action] Market Buy for 25 shares\n";
    // Price doesn't matter for Market Order, putting 0 is fine (logic overrides it)
    book.add_order(3, true, 0, 25, OrderType::MARKET);

    book.print_book(); 

    return 0;
}