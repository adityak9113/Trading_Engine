#pragma once
#include <string>

enum class Side
{
    BUY,
    SELL
};

enum class OrderStatus
{
    OPEN,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED
};

class Order
{
public:
    Order(int id, int quantity, Side side)
    {
        id_ = id;
        quantity_ = quantity;
        side_ = side;
        status_ = OrderStatus::OPEN;
    }

    virtual ~Order() = default;

    virtual std::string describe() const = 0;

    int getId() const { return id_; }
    int getQuantity() const { return quantity_; }
    Side getSide() const { return side_; }
    OrderStatus getStatus() const { return status_; }

    void setQuantity(int qty) { quantity_ = qty; }
    void setStatus(OrderStatus s) { status_ = s; }

    bool isActive() const
    {
        return status_ == OrderStatus::OPEN || status_ == OrderStatus::PARTIALLY_FILLED;
    }

    std::string sideToString() const
    {
        return side_ == Side::BUY ? "BUY" : "SELL";
    }

    std::string statusToString() const
    {
        switch (status_)
        {
            case OrderStatus::OPEN:             return "OPEN";
            case OrderStatus::PARTIALLY_FILLED: return "PARTIALLY_FILLED";
            case OrderStatus::FILLED:           return "FILLED";
            case OrderStatus::CANCELLED:        return "CANCELLED";
        }
        return "UNKNOWN";
    }

private:
    int id_;
    int quantity_;
    Side side_;
    OrderStatus status_;
};
