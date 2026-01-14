/*
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description:
 *     This header file defines the Supplier class, which represents suppliers
 *     in the inventory management system. Each supplier is responsible for
 *     providing certain products, maintaining contractual obligations, and
 *     tracking their performance through quality and reliability metrics.
 *
 *     The class provides functionality for managing:
 *       - Products the supplier provides
 *       - Contractual items
 *       - Supplier ratings and reliability
 *       - Displaying supplier information
 *
 * Dependencies:
 *     - <string>    : Used for handling textual supplier data (names, reliability, etc.)
 *     - <set>       : Used to store unique items and product IDs (no duplicates).
 *     - <algorithm> : Used for searching and managing elements in sets.
 */

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <string>
#include <set>
#include <algorithm>

class Supplier {
private:
    // ===============================
    //        SUPPLIER ATTRIBUTES
    // ===============================

    std::set<std::string> contractItems;        // Set of unique contract items associated with the supplier
    std::set<std::string> productIdsSupplied;   // Set of unique product IDs supplied by this supplier
    double qualityRating;                       // Supplier quality rating (range 0.0 - 5.0)
    std::string deliveryReliability;            // Supplier delivery performance (e.g., "Reliable", "Average")

public:
    // ===============================
    //        CONSTRUCTOR
    // ===============================
    Supplier(); // Default constructor initializes empty sets and quality rating to 0.0

    // ===============================
    //        PRODUCT MANAGEMENT
    // ===============================

    void addProductSupplied(std::string productId);      // Adds a new product ID to the supplier’s provided list
    void removeProductSupplied(std::string productId);   // Removes a product ID from the supplier’s list
    bool suppliesProduct(std::string productId) const;   // Checks if supplier provides a given product
    std::set<std::string> getProductIdsSupplied() const; // Retrieves all product IDs supplied by the supplier

    // ===============================
    //        CONTRACT MANAGEMENT
    // ===============================

    void addContractItem(std::string item);              // Adds a new item to the supplier’s contractual list
    void removeContractItem(std::string item);           // Removes an item from the supplier’s contract list

    // ===============================
    //        ACCESSORS & MUTATORS
    // ===============================

    void setQualityRating(double rating);                // Sets supplier’s quality rating (validated between 0.0–5.0)
    void setDeliveryReliability(std::string reliability);// Sets supplier’s delivery reliability description
    double getQualityRating() const;                     // Retrieves supplier’s quality rating
    std::string getDeliveryReliability() const;          // Retrieves supplier’s reliability description

    // ===============================
    //        DISPLAY FUNCTION
    // ===============================

    void displaySupplierInfo() const;                    // Displays supplier details and all related data
};

#endif // SUPPLIER_H
