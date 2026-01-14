/*
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description:
 *     This header file defines the RawMaterial class, which represents
 *     raw materials used or stored within the inventory management system.
 *     Each raw material includes key details such as ID, name, supplier,
 *     unit of measurement, quantity, price, and purity level.
 *
 *     The class inherits from the Product interface and implements
 *     all pure virtual methods, while also adding raw-material-specific
 *     functionality such as purity checking and shipping cost calculation.
 *
 * Dependencies:
 *     - product.h : Provides the abstract Product base class.
 *     - <string>  : Used for handling textual data (IDs, names, units).
 *     - <iostream>: Used for display and console output.
 */

#ifndef RAWMATERIAL_H
#define RAWMATERIAL_H

#include "product.h"

class RawMaterial : public Product {

private:
    // ===============================
    //        RAW MATERIAL ATTRIBUTES
    // ===============================

    std::string id;                // Unique identifier for the material
    std::string name;              // Material name
    std::string supplier;          // Supplier providing this material
    std::string unitOfMeasurement; // Unit type (e.g., kg, L, m³)

    double minQuantity;            // Minimum acceptable stock level
    double maxQuantity;            // Maximum allowed stock level
    double price;                  // Price per unit
    double quantity;               // Current quantity in stock

    int purity;                    // Purity level of the material (percentage)

public:
    // ===============================
    //        CONSTRUCTOR
    // ===============================
    RawMaterial(std::string id, std::string name, double price, std::string supplier);

    // ===============================
    //        PRODUCT INTERFACE IMPLEMENTATION
    // ===============================

    std::string getId() const override;             // Returns the material ID
    std::string getName() const override;           // Returns the material name
    double getPrice() const override;               // Returns the price per unit
    void setId(std::string id) override;            // Sets the material ID
    void setName(std::string name) override;        // Sets the material name
    void setPrice(double price) override;           // Sets the price per unit
    bool setQuantity(int quantity) override;        // Updates material quantity (with validation)
    void setTotalQuantity(int quantity) override;   // Defines the total available quantity
    void setMinimumQuantity(int quantity) override; // Defines the minimum quantity threshold
    int getQuantity() const override;               // Returns the current stock quantity
    void display() const override;                  // Displays all raw material details
    bool useItem(double quantityUsed) override;     // Decreases stock when material is used
    bool buyItem(double quantityPurchased) override;// Increases stock when material is purchased



    std::string getSupplier() const override;
    std::string getUnit() const;
    void setUnit(std::string unit);
    int getPurity() const;
    void setPurity(int purity);
    // ===============================
    //        RAW MATERIAL METHODS
    // ===============================

    double calculateShippingCost();  // Calculates shipping cost (based on quantity)
    double checkPurityLevel();       // Returns or verifies the purity level
    bool checkCurrentStock();        // Checks if there is available stock

    // ===============================
    //        STOCK LIMITS
    // ===============================

    int getMinQuantity() const override; // Retrieves the minimum quantity limit
    int getMaxQuantity() const override; // Retrieves the maximum quantity limit


};

#endif // RAWMATERIAL_H
