#include <iostream>
#include <map>
#include <memory>

#include "orderbook/Order.hpp"
#include "orderbook/LimitOrder.hpp"
#include "orderbook/MarketOrder.hpp"
#include "orderbook/StopOrder.hpp"
#include "orderbook/StopLimitOrder.hpp"
#include "factory/OrderFactory.hpp"      // Day 3: template factory

int main()
{
    std::cout << "Trading Engine — Day 3: Template-Based Order Factory\n\n";

    // ============================================================
    // Day 3 change: OrderFactory replaces manual make_unique calls
    //
    // OLD (Day 2):
    //   orderRegistry[1] = std::make_unique<LimitOrder>(1, 100.5, 10, Side::BUY);
    //                                                   ^
    //                                                   manual ID — error prone
    //
    // NEW (Day 3):
    //   auto order = OrderFactory::create<LimitOrder>(100.5, 10, Side::BUY);
    //                                                 ^
    //                                                 no ID — factory handles it
    // ============================================================

    std::map<int, std::unique_ptr<Order>> orderRegistry;

    // Helper lambda to add order to registry
    // The factory creates the order, we extract the ID, then move it into the map
    //
    // SIMPLE VERSION — this lambda is the same as writing this function:
    //
    //   void addOrder(std::unique_ptr<Order> order)
    //   {
    //       int id = order->getId();
    //       orderRegistry[id] = std::move(order);
    //   }
    //
    // We use a lambda because it can "capture" orderRegistry from outside.
    // [&orderRegistry] means "I need access to orderRegistry by reference"
    //
    // WHY std::move?
    //   unique_ptr CANNOT be copied (only one owner allowed).
    //   std::move says "transfer ownership from 'order' to the map".
    //   After move, 'order' becomes nullptr — the map now owns it.
    //   Without std::move, this line would NOT compile.
    auto addOrder = [&orderRegistry](std::unique_ptr<Order> order)
    {
        int id = order->getId();
        orderRegistry[id] = std::move(order);
    };

    // --- Create orders through the factory ---
    // Notice: NO manual IDs anywhere. Factory assigns 1, 2, 3...

    // LimitOrders
    // Day 2 way: orderRegistry[1] = std::make_unique<LimitOrder>(1, 100.5, 10, Side::BUY);
    // Day 3 way: factory handles the ID and make_unique for you
    addOrder(OrderFactory::create<LimitOrder>(100.5, 10, Side::BUY));    // gets ID 1
    addOrder(OrderFactory::create<LimitOrder>(102.0, 5, Side::BUY));     // gets ID 2
    addOrder(OrderFactory::create<LimitOrder>(101.0, 3, Side::SELL));    // gets ID 3
    addOrder(OrderFactory::create<LimitOrder>(105.0, 8, Side::SELL));    // gets ID 4

    // MarketOrder — note: fewer args (no price), same create() function
    // Day 2 way: orderRegistry[5] = std::make_unique<MarketOrder>(5, 4, Side::BUY);
    // Day 3 way: just pass qty and side, factory adds the ID
    addOrder(OrderFactory::create<MarketOrder>(4, Side::BUY));           // gets ID 5

    // StopOrders
    // Day 2 way: orderRegistry[6] = std::make_unique<StopOrder>(6, 95.0, 10, Side::SELL);
    addOrder(OrderFactory::create<StopOrder>(95.0, 10, Side::SELL));     // gets ID 6
    addOrder(OrderFactory::create<StopOrder>(105.0, 8, Side::BUY));      // gets ID 7

    // StopLimitOrders — most args (trigger + limit + qty + side), still same create()
    // Day 2 way: orderRegistry[8] = std::make_unique<StopLimitOrder>(8, 95.0, 94.5, 10, Side::SELL);
    addOrder(OrderFactory::create<StopLimitOrder>(95.0, 94.5, 10, Side::SELL));   // gets ID 8
    addOrder(OrderFactory::create<StopLimitOrder>(105.0, 105.5, 12, Side::BUY)); // gets ID 9

    // ---- Print all orders ----
    std::cout << "All Orders (created via OrderFactory):\n";
    for (const auto& [id, order] : orderRegistry)
    {
        std::cout << "  " << order->describe() << "\n";
    }
    std::cout << "\n";

    // ---- Prove the factory auto-increments ----
    std::cout << "Factory state:\n";
    std::cout << "  Next ID will be: " << OrderFactory::getNextId() << "\n";
    std::cout << "  Total orders created: " << orderRegistry.size() << "\n\n";

    // ---- Create more orders later — IDs continue from where we left off ----
    addOrder(OrderFactory::create<LimitOrder>(99.0, 20, Side::BUY));     // gets ID 10
    addOrder(OrderFactory::create<MarketOrder>(15, Side::SELL));          // gets ID 11

    std::cout << "Added 2 more orders:\n";
    std::cout << "  " << orderRegistry[10]->describe() << "\n";
    std::cout << "  " << orderRegistry[11]->describe() << "\n\n";

    // ---- dynamic_cast still works exactly the same ----
    std::cout << "dynamic_cast check (still works with factory-created orders):\n";
    for (const auto& [id, order] : orderRegistry)
    {
        if (auto* limit = dynamic_cast<LimitOrder*>(order.get()))
        {
            std::cout << "  ID " << limit->getId()
                      << " → LimitOrder @ " << limit->getPrice() << "\n";
        }
    }
    std::cout << "\n";

    // ---- Partial fill (Day 2 feature, still works) ----
    std::cout << "Partial fill (Day 2 feature still works):\n";
    Order* o1 = orderRegistry[1].get();
    std::cout << "  Before: " << o1->describe() << "\n";
    o1->setQuantity(4);
    o1->setStatus(OrderStatus::PARTIALLY_FILLED);
    std::cout << "  After:  " << o1->describe() << "\n\n";

    // ---- Reset demo ----
    std::cout << "Factory reset demo:\n";
    std::cout << "  Before reset, next ID: " << OrderFactory::getNextId() << "\n";
    OrderFactory::resetIdCounter();
    std::cout << "  After reset, next ID: " << OrderFactory::getNextId() << "\n";
    // In production you wouldn't reset, but it's useful for testing.

    return 0;
}