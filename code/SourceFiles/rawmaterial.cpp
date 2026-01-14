/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description:
 *     Implements the RawMaterial class, which represents raw materials used in inventory systems.
 *     This class extends the Product interface and includes additional fields such as supplier name,
 *     purity level, and shipping cost calculations.
 *
 * Dependencies:
 *     - RawMaterial.h  : Declares the RawMaterial class interface.
 *     - <iostream>     : Used for console output.
 *     - <string>       : Used for string data types and manipulation.
 */

#include "RawMaterial.h"
#include <iostream>
#include <string>
using namespace std;

// Constructor: initializes all member variables for a new raw material item.
RawMaterial::RawMaterial(string id, string name, double price, string Supplier)
    : id(id), name(name), price(price), quantity(0), minQuantity(5), maxQuantity(1), supplier(Supplier), purity(0) {}

// Returns the unique identifier for this raw material.
string RawMaterial::getId() const {
    return id;
}

// Returns the name of the raw material.
string RawMaterial::getName() const {
    return name;
}

// Returns the price per unit of the raw material.
double RawMaterial::getPrice() const {
    return price;
}

// Updates the unique identifier for this material.
void RawMaterial::setId(string id) {
    this->id = id;
}

// Updates the name of the raw material.
void RawMaterial::setName(string name) {
    this->name = name;
}

// Updates the price of the raw material.
void RawMaterial::setPrice(double price) {
    this->price = price;
}

// Sets the current stock quantity. Returns true if successful.
bool RawMaterial::setQuantity(int quantity) {
    if (quantity >= 0) {
        this->quantity = quantity;
        return true;
    }
    return false;
}

// Returns the current stock quantity.
int RawMaterial::getQuantity() const {
    return quantity;
}

// ✅ FIXED: Sets the maximum quantity limit for inventory (was setting quantity instead)
void RawMaterial::setTotalQuantity(int quantity) {
    if (quantity >= 0) {
        this->maxQuantity = quantity;  // ✅ Changed from this->quantity
    }
}

// ✅ FIXED: Sets the minimum quantity threshold before reordering (was setting quantity instead)
void RawMaterial::setMinimumQuantity(int quantity) {
    this->minQuantity = quantity;  // ✅ Changed from this->quantity
}

// Returns the minimum stock quantity allowed.
int RawMaterial::getMinQuantity() const {
    return minQuantity;
}

// Returns the maximum stock quantity allowed.
int RawMaterial::getMaxQuantity() const {
    return maxQuantity;
}

// Displays all key details about this raw material.
void RawMaterial::display() const {
    cout << "\n=== Raw Material ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Price: $" << price << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Unit: " << unitOfMeasurement << endl;
    cout << "Purity Level: " << purity << "%" << endl;
    cout << "Supplier: " << supplier << endl;
    cout << "Min Quantity: " << minQuantity << endl;
    cout << "Max Quantity: " << maxQuantity << endl;
}

// Calculates the estimated shipping cost based on quantity (example logic).
double RawMaterial::calculateShippingCost() {
    return quantity * 0.5;  // $0.50 per unit
}

// Returns the material's purity level.
double RawMaterial::checkPurityLevel() {
    return purity;
}

// Decreases quantity when material is used. Returns true if successful.
bool RawMaterial::useItem(double quantityUsed) {
    if (quantityUsed > 0 && quantityUsed <= quantity) {
        quantity -= quantityUsed;
        return true;
    }
    return false;
}

// Increases quantity when more material is purchased. Returns true if within stock limit.
bool RawMaterial::buyItem(double quantityPurchased) {
    if (quantityPurchased > 0 && (quantity + quantityPurchased <= maxQuantity)) {
        quantity += quantityPurchased;
        return true;
    }
    return false;
}

// Checks whether the current stock is above zero.
bool RawMaterial::checkCurrentStock() {
    return quantity > 0;
}

// Returns the supplier name for this raw material.
std::string RawMaterial::getSupplier() const {
    return supplier;
}

// Returns the unit of measurement (e.g., "kg", "liters").
std::string RawMaterial::getUnit() const {
    return unitOfMeasurement;
}

// Sets the unit of measurement.
void RawMaterial::setUnit(std::string unit) {
    unitOfMeasurement = unit;
}

// Returns the purity level as a percentage.
int RawMaterial::getPurity() const {
    return purity;
}

// Sets the purity level (0-100%).
void RawMaterial::setPurity(int purity) {
    this->purity = purity;
}
