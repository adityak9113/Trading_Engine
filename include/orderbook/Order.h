#pragma once

enum class Side {
    BUY,
    SELL
};

class Order {
public:
    Order(int id, double price, int quantity, Side side)
        : id_(id), price_(price), quantity_(quantity), side_(side) {}

    int getId() const { return id_; }
    double getPrice() const { return price_; }
    int getQuantity() const { return quantity_; }
    Side getSide() const { return side_; }

private:
    int id_;
    double price_;
    int quantity_;
    Side side_;
};