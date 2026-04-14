#include <iostream>
#include <map>
#include <memory>
#include "orderbook/Order.hpp"
#include "orderbook/LimitOrder.hpp"
#include "orderbook/MarketOrder.hpp"
#include "orderbook/StopOrder.hpp"
#include "orderbook/StopLimitOrder.hpp"

int main()
{
    std::cout << "Trading Engine — Day 2\n\n";

    // Day 2: Order Registry — std::map<int, std::unique_ptr<Order>>
    // order ID → owning pointer. This is the "scary pattern":
    //   std::map<int, SomeBase*>
    std::map<int, std::unique_ptr<Order>> orderRegistry;

    // Day 1 types
    orderRegistry[1] = std::make_unique<LimitOrder>(1, 100.5, 10, Side::BUY);
    orderRegistry[2] = std::make_unique<LimitOrder>(2, 102.0, 5, Side::BUY);
    orderRegistry[3] = std::make_unique<LimitOrder>(3, 101.0, 3, Side::SELL);
    orderRegistry[4] = std::make_unique<LimitOrder>(4, 105.0, 8, Side::SELL);
    orderRegistry[5] = std::make_unique<MarketOrder>(5, 4, Side::BUY);

    // Day 2 types
    orderRegistry[6] = std::make_unique<StopOrder>(6, 95.0, 10, Side::SELL);
    orderRegistry[7] = std::make_unique<StopLimitOrder>(7, 95.0, 94.5, 10, Side::SELL);
    orderRegistry[8] = std::make_unique<StopOrder>(8, 105.0, 8, Side::BUY);
    orderRegistry[9] = std::make_unique<StopLimitOrder>(9, 105.0, 105.5, 12, Side::BUY);

    // ---- Print all orders (polymorphism through base pointer) ----
    std::cout << "All Orders in Registry:\n";
    for (const auto& [id, order] : orderRegistry)
    {
        std::cout << "  " << order->describe() << "\n";
    }
    std::cout << "\n";

    // ---- dynamic_cast: find LimitOrders ----
    std::cout << "dynamic_cast — Finding LimitOrders:\n";
    for (const auto& [id, order] : orderRegistry)
    {
        if (auto* limit = dynamic_cast<LimitOrder*>(order.get()))
        {
            std::cout << "  Order " << limit->getId()
                      << " is LimitOrder @ " << limit->getPrice() << "\n";
        }
    }
    std::cout << "\n";

    // ---- dynamic_cast: find StopOrders ----
    std::cout << "dynamic_cast — Finding StopOrders:\n";
    for (const auto& [id, order] : orderRegistry)
    {
        if (auto* stop = dynamic_cast<StopOrder*>(order.get()))
        {
            std::cout << "  Order " << stop->getId()
                      << " is StopOrder trigger @ " << stop->getTriggerPrice() << "\n";
        }
    }
    std::cout << "\n";

    // ---- dynamic_cast: find StopLimitOrders ----
    std::cout << "dynamic_cast — Finding StopLimitOrders:\n";
    for (const auto& [id, order] : orderRegistry)
    {
        if (auto* sl = dynamic_cast<StopLimitOrder*>(order.get()))
        {
            std::cout << "  Order " << sl->getId()
                      << " trigger @ " << sl->getTriggerPrice()
                      << " limit @ " << sl->getLimitPrice() << "\n";
        }
    }
    std::cout << "\n";

    // ---- Partial fill + OrderStatus ----
    std::cout << "Partial Fill Simulation:\n";
    Order* o1 = orderRegistry[1].get();
    std::cout << "  Before: " << o1->describe() << "\n";

    int fillQty = 6;
    int remaining = o1->getQuantity() - fillQty;
    o1->setQuantity(remaining);
    o1->setStatus(OrderStatus::PARTIALLY_FILLED);
    std::cout << "  Filled " << fillQty << ", remaining " << remaining << "\n";
    std::cout << "  After:  " << o1->describe() << "\n";

    o1->setQuantity(0);
    o1->setStatus(OrderStatus::FILLED);
    std::cout << "  Filled remaining " << remaining << "\n";
    std::cout << "  After:  " << o1->describe() << "\n\n";

    // ---- Cancel an order ----
    std::cout << "Order Cancellation:\n";
    Order* o6 = orderRegistry[6].get();
    std::cout << "  Before: " << o6->describe() << "\n";
    o6->setStatus(OrderStatus::CANCELLED);
    std::cout << "  After:  " << o6->describe() << "\n\n";

    // ---- Trigger a stop order ----
    std::cout << "Stop Order Trigger:\n";
    if (auto* stop8 = dynamic_cast<StopOrder*>(orderRegistry[8].get()))
    {
        std::cout << "  Before: " << stop8->describe() << "\n";
        stop8->trigger();
        std::cout << "  After:  " << stop8->describe() << "\n";
    }
    std::cout << "\n";

    // ---- Final state ----
    std::cout << "Final Registry:\n";
    int active = 0, inactive = 0;
    for (const auto& [id, order] : orderRegistry)
    {
        if (order->isActive())
            ++active;
        else
            ++inactive;
        std::cout << "  " << order->describe() << "\n";
    }
    std::cout << "\nActive: " << active << "  Inactive: " << inactive
              << "  Total: " << orderRegistry.size() << "\n";

    return 0;
}