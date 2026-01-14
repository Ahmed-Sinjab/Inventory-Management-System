/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description: This file implements the Supplier class, which represents a supplier
 * within the inventory management system. Each supplier maintains a list of
 * product IDs they provide, contract items, a quality rating, and delivery reliability
 * information. The class supports adding and removing supplied products and contracts,
 * as well as displaying supplier details.
 *
 * Dependencies:
 *  - supplier.h : Declares the Supplier class and its members.
 *  - <iostream> : Used for console-based input/output operations.
 *  - <set> : Used to store unique product IDs and contract items.
 *  - <algorithm> : Used for searching and erasing items in sets.
 */

#include "supplier.h"
#include <iostream>
using namespace std;

// Default constructor initializes quality rating to 0.0
Supplier::Supplier() : qualityRating(0.0) {}

// Adds a product ID to the set of supplied products if not already present
void Supplier::addProductSupplied(string productId) {
    if (!suppliesProduct(productId)) {
        productIdsSupplied.insert(productId);
        cout << "Supplier now provides product: " << productId << endl;
    }
}

// Removes a product ID from the set of supplied products if it exists
void Supplier::removeProductSupplied(string productId) {
    auto it = find(productIdsSupplied.begin(), productIdsSupplied.end(), productId);
    if (it != productIdsSupplied.end()) {
        productIdsSupplied.erase(it);
        cout << "Supplier no longer provides product: " << productId << endl;
    }
}

// Checks if the supplier provides a specific product by its ID
bool Supplier::suppliesProduct(string productId) const {
    return find(productIdsSupplied.begin(), productIdsSupplied.end(), productId)
    != productIdsSupplied.end();
}

// Returns the set of all product IDs supplied by this supplier
set<string> Supplier::getProductIdsSupplied() const {
    return productIdsSupplied;
}

// Adds a new contract item to the supplier's set of contract terms
void Supplier::addContractItem(string item) {
    contractItems.insert(item);
}

// Removes a contract item from the supplier's contract list if found
void Supplier::removeContractItem(string item) {
    auto it = find(contractItems.begin(), contractItems.end(), item);
    if (it != contractItems.end()) {
        contractItems.erase(it);
    }
}

// Sets the supplier’s quality rating (valid range: 0.0–5.0)
void Supplier::setQualityRating(double rating) {
    if (rating >= 0.0 && rating <= 5.0) {
        qualityRating = rating;
    }
}

// Sets the supplier’s delivery reliability description (e.g., “Very Reliable”)
void Supplier::setDeliveryReliability(string reliability) {
    deliveryReliability = reliability;
}

// Returns the supplier’s quality rating
double Supplier::getQualityRating() const {
    return qualityRating;
}

// Returns the supplier’s delivery reliability description
string Supplier::getDeliveryReliability() const {
    return deliveryReliability;
}

// Displays all supplier information, including rating, reliability, and products
void Supplier::displaySupplierInfo() const {
    cout << "Quality Rating: " << qualityRating << "/5.0" << endl;
    cout << "Delivery Reliability: " << deliveryReliability << endl;

    // Display the list of supplied products
    if (!productIdsSupplied.empty()) {
        cout << "Products Supplied (IDs): ";
        for (const auto& id : productIdsSupplied) {
            cout << id << " ";
        }
        cout << endl;
    }

    // Display contract items if any exist
    if (!contractItems.empty()) {
        cout << "Contract Items:" << endl;
        for (const auto& item : contractItems) {
            cout << "  - " << item << endl;
        }
    }
}
