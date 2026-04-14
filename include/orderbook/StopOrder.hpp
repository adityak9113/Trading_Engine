#pragma once
#include "orderbook/Order.hpp"
#include <string>

// Day 2: StopOrder — triggers at triggerPrice, becomes market order
class StopOrder : public Order
{
public:
    StopOrder(int id, double triggerPrice, int quantity, Side side)
        : Order(id, quantity, side)
    {
        triggerPrice_ = triggerPrice;
        triggered_ = false;
    }

    double getTriggerPrice() const
    {
        return triggerPrice_;
    }

    bool isTriggered() const
    {
        return triggered_;
    }

    void trigger()
    {
        triggered_ = true;
    }

    std::string describe() const override
    {
        return "StopOrder [" + sideToString() + "] ID: "
               + std::to_string(getId())
               + " Trigger: " + std::to_string(triggerPrice_)
               + " Qty: " + std::to_string(getQuantity())
               + " Triggered: " + (triggered_ ? "YES" : "NO")
               + " Status: " + statusToString();
    }

private:
    double triggerPrice_;
    bool triggered_;
};