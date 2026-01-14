/**
*Author: Ahmed Sinjab, Omar Hossain
*Creation Date: November 6, 2025
*Last Edit: December 12, 2025
*Purpose: This is an interface for the product class
*/
#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product
{
public:
    virtual ~Product() {}

    // Pure virtual functions that all products must implement
    virtual std::string getId() const = 0;
    virtual std::string getName() const = 0;
    virtual double getPrice() const = 0;
    virtual std::string getSupplier() const = 0;  // ✅ Must have = 0

    virtual void setId(std::string id) = 0;
    virtual void setName(std::string name) = 0;
    virtual void setPrice(double price) = 0;

    virtual bool setQuantity(int quantity) = 0;
    virtual int getQuantity() const = 0;
    virtual void setTotalQuantity(int quantity) = 0;
    virtual void setMinimumQuantity(int quantity) = 0;
    virtual int getMinQuantity() const = 0;
    virtual int getMaxQuantity() const = 0;

    virtual void display() const = 0;

    virtual bool useItem(double quantityUsed) = 0;
    virtual bool buyItem(double quantityPurchased) = 0;
};

#endif // PRODUCT_H
