/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description:
 *     Implements the NonPerishable class, which represents durable products that do not spoil over time.
 *     This class inherits from the Product interface and manages attributes such as fragility, quantity limits,
 *     and supplier details. It includes functionality for purchasing, using, and displaying product information.
 *
 * Dependencies:
 *     - nonperishable.h : Declares the NonPerishable class interface.
 *     - <iostream>      : Provides console-based output for product details.
 *     - <string>        : Provides string data handling for product attributes.
 */

#include "nonperishable.h"
#include <iostream>
#include <string>
using namespace std;

// Constructor: Initializes a NonPerishable product with given details.
// Sets default values for quantity and quantity limits.
NonPerishable::NonPerishable(string id, string name, double price, string Supplier)
    : id(id), name(name), price(price), quantity(0), minQuantity(5), maxQuantity(1), supplier(Supplier) {}

// Returns the unique product identifier.
string NonPerishable::getId() const {
    return id;
}

// Returns the name of the product.
string NonPerishable::getName() const {
    return name;
}

// Returns the price of the product.
double NonPerishable::getPrice() const {
    return price;
}

// Updates the product ID.
void NonPerishable::setId(string id) {
    this->id = id;
}

// Updates the product name.
void NonPerishable::setName(string name) {
    this->name = name;
}

// Updates the product price.
void NonPerishable::setPrice(double price) {
    this->price = price;
}

// Sets the current quantity if it is within the allowed range (0 to maxQuantity).
bool NonPerishable::setQuantity(int quantity) {
    if (quantity >= 0 && quantity <= maxQuantity) {
        this->quantity = quantity;
        return true;
    }
    return false;
}

// Returns the current product quantity.
int NonPerishable::getQuantity() const {
    return quantity;
}

// Updates the maximum allowed stock if the new total quantity is higher than current.
void NonPerishable::setTotalQuantity(int totalQuantity) {
    if (totalQuantity > 0 && quantity < totalQuantity) {
        this->maxQuantity = totalQuantity;
    }
}

// Sets the minimum quantity threshold for reordering.
void NonPerishable::setMinimumQuantity(int quantity) {
    this->quantity = quantity;
}

// Returns the minimum quantity threshold.
int NonPerishable::getMinQuantity() const {
    return minQuantity;
}

// Returns the maximum quantity allowed for this product.
int NonPerishable::getMaxQuantity() const {
    return maxQuantity;
}

// Reduces the stock when product is used, if enough quantity is available.
bool NonPerishable::useItem(double quantityUsed) {
    if (quantityUsed > 0 && quantityUsed <= quantity) {
        quantity -= quantityUsed;
        return true;
    }
    return false;
}

// Increases stock when product is purchased, ensuring it does not exceed maxQuantity.
bool NonPerishable::buyItem(double quantityPurchased) {
    if (quantityPurchased > 0 && (quantity + quantityPurchased <= maxQuantity)) {
        quantity += quantityPurchased;
        return true;
    }
    return false;
}

// Displays product details, including fragility status.
void NonPerishable::display() const {
    cout << "\n=== Non-Perishable Product ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Price: $" << price << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Fragile: " << isFragile << endl;
}

// Sets whether the product is fragile (true/false).
void NonPerishable::setFragile(bool fragile) {
    isFragile = fragile;
}

// Returns whether the product is fragile.
bool NonPerishable::getFragile() {
    return isFragile;
}
string NonPerishable::getSupplier() const {
    return supplier;
}
