#pragma once

#include <memory>
#include "orderbook/Order.hpp"

// ============================================================
// OrderFactory — Day 3
//
// Template-based factory that:
//   1. Auto-assigns order IDs (no more manual numbering)
//   2. Creates any Order type through one function
//   3. Uses variadic templates + perfect forwarding
//
// Usage:
//   auto order = OrderFactory::create<LimitOrder>(100.5, 10, Side::BUY);
//   // ID is auto-assigned, no need to pass it
// ============================================================
class OrderFactory
{
public:
    // ---------------------------------------------------------------
    // WITHOUT templates, you'd need a separate function for EACH type:
    //
    //   static std::unique_ptr<Order> createLimitOrder(double price, int qty, Side side)
    //   {
    //       return std::make_unique<LimitOrder>(nextId_++, price, qty, side);
    //   }
    //
    //   static std::unique_ptr<Order> createMarketOrder(int qty, Side side)
    //   {
    //       return std::make_unique<MarketOrder>(nextId_++, qty, side);
    //   }
    //
    //   static std::unique_ptr<Order> createStopOrder(double trigger, int qty, Side side)
    //   {
    //       return std::make_unique<StopOrder>(nextId_++, trigger, qty, side);
    //   }
    //
    //   static std::unique_ptr<Order> createStopLimitOrder(double trigger, double limit, int qty, Side side)
    //   {
    //       return std::make_unique<StopLimitOrder>(nextId_++, trigger, limit, qty, side);
    //   }
    //
    // 4 functions doing the SAME thing! And every new order type = another function.
    // Templates solve this — ONE function handles ALL types:
    // ---------------------------------------------------------------

    // template<typename T>        — T is the order type (LimitOrder, MarketOrder, etc.)
    // typename... Args            — "..." means any number of additional types
    // Args&&... args              — the actual arguments, forwarded perfectly
    //
    // WHAT THE COMPILER ACTUALLY DOES (it writes those 4 functions FOR you):
    //
    //   You write:  OrderFactory::create<LimitOrder>(100.5, 10, Side::BUY)
    //   Compiler:   T = LimitOrder, Args = {double, int, Side}
    //   Becomes:    make_unique<LimitOrder>(nextId_++, 100.5, 10, Side::BUY)
    //
    //   You write:  OrderFactory::create<MarketOrder>(4, Side::BUY)
    //   Compiler:   T = MarketOrder, Args = {int, Side}
    //   Becomes:    make_unique<MarketOrder>(nextId_++, 4, Side::BUY)
    //
    //   You write:  OrderFactory::create<StopLimitOrder>(95.0, 94.5, 10, Side::SELL)
    //   Compiler:   T = StopLimitOrder, Args = {double, double, int, Side}
    //   Becomes:    make_unique<StopLimitOrder>(nextId_++, 95.0, 94.5, 10, Side::SELL)
    //
    // Same function, different types, different number of args. That's the power.

    template<typename T, typename... Args>
    static std::unique_ptr<Order> create(Args&&... args)
    {
        // nextId_++ gives current value THEN increments (1, then 2, then 3...)
        // std::forward<Args>(args)... passes all args exactly as received
        //
        // Simple version of what this line does:
        //   return std::make_unique<T>(nextId_++, arg1, arg2, arg3...);
        return std::make_unique<T>(nextId_++, std::forward<Args>(args)...);
    }

    // Utility: reset counter (useful for testing)
    static void resetIdCounter() { nextId_ = 1; }

    // Utility: peek at next ID without incrementing
    static int getNextId() { return nextId_; }

private:
    // static  — belongs to the class, not any object. Only one copy exists.
    // inline  — C++17 feature. Lets you initialize a static member right here
    //           instead of needing a separate .cpp file.
    static inline int nextId_ = 1;
};