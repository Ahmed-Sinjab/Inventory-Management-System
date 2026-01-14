#ifndef PERISHABLE_H
#define PERISHABLE_H

#include "product.h"

class Perishable : public Product
{
protected:
    std::string id;
    std::string name;
    double price;
    int quantity;
    std::string supplier;
    std::string expirationDate;
    std::string storageCondition;
    int maxQuantity;
    int minQuantity;
    std::string disposalMethod;

public:
    Perishable(std::string id, std::string name, double price, std::string supplier);

    // Writing the methods for the product.h class
    // Getters
    std::string getId() const override;
    std::string getName() const override;
    double getPrice() const override;
    int getQuantity() const override;
    int getMinQuantity() const override;
    int getMaxQuantity() const override;

    // ✅ ADDED: getSupplier() implementation
    std::string getSupplier() const override;

    // Perishable specific getters
    std::string getExpirationDate() const;
    std::string getStorageCondition() const;

    // Setters
    void setId(std::string id) override;
    void setName(std::string name) override;
    void setPrice(double price) override;
    bool setQuantity(int quantity) override;
    void setTotalQuantity(int quantity) override;
    void setMinimumQuantity(int quantity) override;

    // Perishable specific setters
    void setExpirationDate(std::string date);
    void setStorageCondition(std::string condition);

    // Functional methods
    bool useItem(double quantityUsed) override;
    bool buyItem(double quantityPurchased) override;
    void display() const override;

    virtual bool isExpired() const;
};

#endif // PERISHABLE_H
