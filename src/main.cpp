#include <iostream>
#include <vector>
#include <memory>
#include "orderbook/Order.h"
#include "orderbook/LimitOrder.hpp"
#include "orderbook/MarketOrder.hpp"

int main()
{
    std::cout << "Trading Engine Started...\n\n";

    // unique_ptr prevents object slicing
    std::vector<std::unique_ptr<Order>> orders;

    // LimitOrders — have a price
    orders.push_back(std::make_unique<LimitOrder>(1, 100.5, 10, Side::BUY));
    orders.push_back(std::make_unique<LimitOrder>(2, 102.0, 5, Side::BUY));
    orders.push_back(std::make_unique<LimitOrder>(3, 101.0, 3, Side::SELL));
    orders.push_back(std::make_unique<LimitOrder>(4, 105.0, 8, Side::SELL));

    // MarketOrders — no price
    orders.push_back(std::make_unique<MarketOrder>(5, 4, Side::BUY));
    orders.push_back(std::make_unique<MarketOrder>(6, 6, Side::SELL));

    // Polymorphism — same describe() call, different output
    std::cout << "All Orders:\n";
    for (const auto& order : orders)
    {
        std::cout << order->describe() << "\n";
    }

    return 0;
}