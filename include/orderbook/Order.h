#pragma once
#include <string>

enum class Side
{
    BUY,
    SELL
};

class Order
{
public:
    Order(int id, int quantity, Side side)
    {
        id_ = id;
        quantity_ = quantity;
        side_ = side;
    }

    virtual ~Order() = default;

    virtual std::string describe() const = 0;

    int getId() const
    {
        return id_;
    }

    int getQuantity() const
    {
        return quantity_;
    }

    Side getSide() const
    {
        return side_;
    }

    std::string sideToString() const
    {
        if (side_ == Side::BUY)
            return "BUY";
        return "SELL";
    }

private:
    int id_;
    int quantity_;
    Side side_;
};