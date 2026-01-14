#ifndef NONPERISHABLE_H
#define NONPERISHABLE_H
// /*
// *Author: Ahmed Sinjab, Omar Hossain
// *Start Date: November 6, 2025
// *Last Edit: November 6, 2025
// *Purpose:This is a header file for NonPerishable class
// *Dependencies: product.h
// *
// */


#include "product.h"

class NonPerishable : public Product
{
protected:
    //Main product Attributes
    std::string id;
    std::string name;
    double price;
    int quantity;
    //Supporter attributes:
    std::string supplier;
    bool isFragile;
    int maxQuantity;
    int minQuantity;



public:
    NonPerishable(std::string id, std::string name,double price,std::string supplier);

    // Implement all Product interface methods
    std::string getId() const override;
    std::string getName() const override;
    double getPrice() const override;
    void setId(std::string id) override;
    void setName(std::string name) override;
    void setPrice(double price) override;
    bool setQuantity(int quantity) override;
    void setTotalQuantity(int quantity) override; // Updated to take an int
    void setMinimumQuantity(int quantity)override;
    int getQuantity() const override;
    void display() const override;
    bool useItem(double quantityUsed)override;
    bool buyItem(double quantityPurchased)override;
    // NonPerishable specific methods
    void setFragile(bool isFragile);
    bool getFragile();
    int getMinQuantity() const override;
    int getMaxQuantity() const override;
    std::string getSupplier() const override;

};

#endif // NONPERISHABLE_H
