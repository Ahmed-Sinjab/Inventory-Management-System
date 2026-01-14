/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description:
 *     Implements the Perishable class, which inherits from the Product interface.
 *     This class represents products that have an expiration date and require
 *     specific storage conditions. It provides functionality to manage quantities,
 *     pricing, and checks for expiration (simplified).
 *
 * Dependencies:
 *     - Perishable.h : Declares the Perishable class and its interface.
 *     - <iostream>   : Used for console-based display output.
 */

#include "Perishable.h"
#include <iostream>
using namespace std;

// Constructor: initializes a perishable product with basic details such as
// ID, name, price, and supplier. Sets quantity to 0 by default.
Perishable::Perishable(string id, string name, double price, string Supplier)
    : id(id), name(name), price(price), quantity(0), minQuantity(5), maxQuantity(1), supplier(Supplier) {}

// Returns the unique identifier for this perishable product.
string Perishable::getId() const {
    return id;
}

// Returns the name of the perishable product.
string Perishable::getName() const {
    return name;
}

// Returns the price per unit of the product.
double Perishable::getPrice() const {
    return price;
}

// ✅ ADDED: Returns the supplier name for this product.
string Perishable::getSupplier() const {
    return supplier;
}

// Updates the product ID.
void Perishable::setId(string id) {
    this->id = id;
}

// Updates the product name.
void Perishable::setName(string name) {
    this->name = name;
}

// Updates the product price.
void Perishable::setPrice(double price) {
    this->price = price;
}

// Sets the available quantity if the input value is valid (>= 0).
bool Perishable::setQuantity(int quantity) {
    if (quantity >= 0) {
        this->quantity = quantity;
        return true;
    }
    return false;
}

// Returns the minimum quantity threshold before a restock alert is needed.
int Perishable::getMinQuantity() const {
    return minQuantity;
}

// Returns the maximum allowed stock quantity.
int Perishable::getMaxQuantity() const {
    return maxQuantity;
}

// Returns the current quantity in stock.
int Perishable::getQuantity() const {
    return quantity;
}

// Sets the total quantity limit for the product.
void Perishable::setTotalQuantity(int quantity) {
    this->maxQuantity = quantity;
}

// Sets the minimum quantity limit (used for restocking decisions).
void Perishable::setMinimumQuantity(int quantity) {
    this->minQuantity = quantity;
}

// Displays product details, including expiration date and storage conditions.
void Perishable::display() const {
    cout << "\n=== Perishable Product ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Price: $" << price << endl;
    cout << "Supplier: " << supplier << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Expiration Date: " << expirationDate << endl;
    cout << "Storage Condition: " << storageCondition << endl;
}

// Returns the expiration date for this product.
string Perishable::getExpirationDate() const {
    return expirationDate;
}

// Updates the expiration date of the product.
void Perishable::setExpirationDate(string date) {
    expirationDate = date;
}

// ✅ ADDED: Returns the storage condition for this product.
string Perishable::getStorageCondition() const {
    return storageCondition;
}

// Updates the storage condition for this product (e.g., "Keep refrigerated").
void Perishable::setStorageCondition(string condition) {
    storageCondition = condition;
}

// Placeholder for expiration logic — currently returns false (not expired).
bool Perishable::isExpired() const {
    // Future implementation could compare current date with expirationDate
    return false;
}

// Reduces the quantity when some of the product is used.
// Returns true if enough stock is available to fulfill the request.
bool Perishable::useItem(double quantityUsed) {
    if (quantityUsed > 0 && quantityUsed <= quantity) {
        quantity -= quantityUsed;
        return true;
    }
    return false;
}

// Increases stock quantity when new product is purchased.
// Returns true if total does not exceed the maxQuantity limit.
bool Perishable::buyItem(double quantityPurchased) {
    if (quantityPurchased > 0 && (quantity + quantityPurchased <= maxQuantity)) {
        quantity += quantityPurchased;
        return true;
    }
    return false;
}
