#pragma once
#include "orderbook/Order.hpp"
#include <string>

// Day 2: StopLimitOrder — triggers at triggerPrice, then acts as limit at limitPrice
class StopLimitOrder : public Order
{
public:
    StopLimitOrder(int id, double triggerPrice, double limitPrice, int quantity, Side side)
        : Order(id, quantity, side)
    {
        triggerPrice_ = triggerPrice;
        limitPrice_ = limitPrice;
        triggered_ = false;
    }

    double getTriggerPrice() const
    {
        return triggerPrice_;
    }

    double getLimitPrice() const
    {
        return limitPrice_;
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
        return "StopLimitOrder [" + sideToString() + "] ID: "
               + std::to_string(getId())
               + " Trigger: " + std::to_string(triggerPrice_)
               + " Limit: " + std::to_string(limitPrice_)
               + " Qty: " + std::to_string(getQuantity())
               + " Triggered: " + (triggered_ ? "YES" : "NO")
               + " Status: " + statusToString();
    }

private:
    double triggerPrice_;
    double limitPrice_;
    bool triggered_;
};