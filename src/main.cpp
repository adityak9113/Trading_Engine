#include <iostream>
#include <vector>
#include <algorithm>
#include "orderbook/Order.h"

int main() {
    std::cout << "🚀 Trading Engine Started...\n\n";

    // 🔹 Hardcoded Orders
    Order o1(1, 100, 10, Side::BUY);
    Order o2(2, 102, 5, Side::BUY);
    Order o3(3, 101, 3, Side::SELL);
    Order o4(4, 105, 8, Side::SELL);

    // 🔹 Temporary OrderBook (vector-based)
    std::vector<Order> buys;
    std::vector<Order> sells;

    std::vector<Order> allOrders = {o1, o2, o3, o4};

    // 🔹 Separate BUY and SELL
    for (const auto& order : allOrders) {
        if (order.getSide() == Side::BUY) {
            buys.push_back(order);
        } else {
            sells.push_back(order);
        }
    }

    // 🔹 Print BUY orders
    std::cout << "🟢 BUY ORDERS:\n";
    for (const auto& o : buys) {
        std::cout << "OrderID: " << o.getId()
                  << " Price: " << o.getPrice()
                  << " Qty: " << o.getQuantity() << "\n";
    }

    // 🔹 Print SELL orders
    std::cout << "\n🔴 SELL ORDERS:\n";
    for (const auto& o : sells) {
        std::cout << "OrderID: " << o.getId()
                  << " Price: " << o.getPrice()
                  << " Qty: " << o.getQuantity() << "\n";
    }

    // 🔹 Find Best BUY (highest price)
    double bestBuy = 0;
    for (const auto& o : buys) {
        bestBuy = std::max(bestBuy, o.getPrice());
    }

    // 🔹 Find Best SELL (lowest price)
    double bestSell = 1e9;
    for (const auto& o : sells) {
        bestSell = std::min(bestSell, o.getPrice());
    }

    std::cout << "\n📊 Best Buy Price: " << bestBuy;
    std::cout << "\n📊 Best Sell Price: " << bestSell << "\n";

    // 🔹 Matching Logic
    if (bestBuy >= bestSell) {
        std::cout << "\n✅ Trade CAN happen (Best Buy >= Best Sell)\n";
    } else {
        std::cout << "\n❌ No Trade (Best Buy < Best Sell)\n";
    }

    return 0;
}