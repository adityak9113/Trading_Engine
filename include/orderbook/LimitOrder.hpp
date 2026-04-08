#pragma once
#include "orderbook/Order.h"
#include <string>

class LimitOrder : public Order
{
public:
    LimitOrder(int id, double price, int quantity, Side side)
        : Order(id, quantity, side)
    {
        price_ = price;
    }

    double getPrice() const
    {
        return price_;
    }

    std::string describe() const override
    {
        return "LimitOrder [" + sideToString() + "] ID: "
               + std::to_string(getId())
               + " Price: " + std::to_string(price_)
               + " Qty: " + std::to_string(getQuantity());
    }

private:
    double price_;
};