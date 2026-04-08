#pragma once
#include "orderbook/Order.h"
#include <string>

class MarketOrder : public Order
{
public:
    MarketOrder(int id, int quantity, Side side)
        : Order(id, quantity, side)
    {
    }

    std::string describe() const override
    {
        return "MarketOrder [" + sideToString() + "] ID: "
               + std::to_string(getId())
               + " Qty: " + std::to_string(getQuantity())
               + " @ MARKET";
    }
};