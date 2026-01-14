/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description:
 *     Implements the Inventory class, which manages a collection of Product and Supplier objects.
 *     The class supports adding, removing, searching, purchasing, selling, and reporting on
 *     inventory items. It uses sets to store unique products and suppliers, ensuring efficient lookups
 *     and avoiding duplicates.
 *
 * Dependencies:
 *     - inventory.h : Declares the Inventory class and its interface.
 *     - <iostream>  : Provides console output for displaying reports and messages.
 *     - <algorithm> : Used for searching and managing data structures.
 */

#include "inventory.h"
#include <iostream>
#include <algorithm>
using namespace std;

// Constructor: Initializes the inventory system and prints a startup message.
Inventory::Inventory() {
    cout << "Inventory system initialized." << endl;
}

// Destructor: Cleans up resources (if any were dynamically allocated in the future).
Inventory::~Inventory() {}

// Adds a product to the inventory (without supplier association).
void Inventory::addProduct(Product* product) {
    if (product == nullptr) {
        cout << "Error: Cannot add null product." << endl;
        return;
    }

    // Inserts the product into the ownedProductList set.
    ownedProductList.insert(product);
    cout << "Product added: " << product->getName() << " (ID: " << product->getId() << ")" << endl;
}

// Adds a product and associates it with a supplier.
void Inventory::addProductWithSupplier(Product* product, Supplier* supplier) {
    if (product == nullptr || supplier == nullptr) {
        cout << "Error: Cannot add null product or supplier." << endl;
        return;
    }

    // Add the product to the inventory
    addProduct(product);

    // Supplier records that it provides this product
    supplier->addProductSupplied(product->getId());

    // Add supplier to the system (set prevents duplicates)
    addSupplier(supplier);
}

// Searches for a product by its ID and returns a pointer if found.
Product* Inventory::searchProduct(string productId) const {
    for (Product* p : ownedProductList) {
        if (p->getId() == productId) {
            return p;
        }
    }
    return nullptr;
}

// Displays all products currently in the inventory, including supplier info if available.
void Inventory::displayAllProducts() const {
    cout << "\n========================================" << endl;
    cout << "        INVENTORY - ALL PRODUCTS        " << endl;
    cout << "========================================" << endl;

    if (ownedProductList.empty()) {
        cout << "No products in inventory." << endl;
        return;
    }

    // Iterate and display each product’s information
    for (Product* p : ownedProductList) {
        cout << "\n---" << endl;
        cout << "ID: " << p->getId() << " | Name: " << p->getName()
             << " | Qty: " << p->getQuantity() << " | Price: $" << p->getPrice() << endl;

        Supplier* supplier = getSupplierForProduct(p->getId());
        if (supplier != nullptr) {
            cout << "Supplier: Rating " << supplier->getQualityRating()
            << " | Reliability: " << supplier->getDeliveryReliability() << endl;
        } else {
            cout << "Supplier: None assigned" << endl;
        }
    }
}

// Returns the set of all products in the inventory.
set<Product*> Inventory::getProductList() const {
    return ownedProductList;
}

// Adds a supplier to the inventory system.
void Inventory::addSupplier(Supplier* supplier) {
    if (supplier == nullptr) {
        cout << "Error: Cannot add null supplier." << endl;
        return;
    }

    // The set automatically handles duplicates
    auto result = supplierList.insert(supplier);
    if (result.second) { // result.second == true means new insertion
        cout << "Supplier added to system." << endl;
    }
}
std::set<Supplier*> Inventory::getSupplierList() const {
    return supplierList;
}
// Displays all registered suppliers and their associated products.
void Inventory::displayAllSuppliers() const {
    cout << "\n========================================" << endl;
    cout << "        INVENTORY - ALL SUPPLIERS       " << endl;
    cout << "========================================" << endl;

    if (supplierList.empty()) {
        cout << "No suppliers registered." << endl;
        return;
    }

    int count = 1;
    for (Supplier* s : supplierList) {
        cout << "\n--- Supplier #" << count++ << " ---" << endl;
        s->displaySupplierInfo();

        // Show which products are supplied by this supplier
        set<Product*> products = getProductsFromSupplier(s);
        if (!products.empty()) {
            cout << "Products in inventory from this supplier:" << endl;
            for (Product* p : products) {
                cout << "  - " << p->getName() << " (Qty: " << p->getQuantity() << ")" << endl;
            }
        }
    }
}

// Finds the supplier associated with a given product ID.
Supplier* Inventory::getSupplierForProduct(string productId) const {
    for (Supplier* s : supplierList) {
        if (s->suppliesProduct(productId)) {
            return s;
        }
    }
    return nullptr;
}

// Retrieves all Product objects supplied by a specific Supplier.
set<Product*> Inventory::getProductsFromSupplier(Supplier* supplier) const {
    set<Product*> products;

    if (supplier == nullptr) {
        return products;
    }

    // Match supplier product IDs with those in the inventory
    set<string> productIds = supplier->getProductIdsSupplied();

    for (const string& id : productIds) {
        Product* p = searchProduct(id);
        if (p != nullptr) {
            products.insert(p);
        }
    }

    return products;
}

// Purchases (adds) quantity of a product and updates inventory and supplier info.
bool Inventory::buyProduct(string productId, int quantity) {
    Product* product = searchProduct(productId);

    if (product == nullptr) {
        cout << "Error: Product not found (ID: " << productId << ")" << endl;
        return false;
    }

    if (quantity <= 0) {
        cout << "Error: Quantity must be positive!" << endl;
        return false;
    }

    Supplier* supplier = getSupplierForProduct(productId);

    // Display supplier info or warning if missing
    if (supplier == nullptr) {
        cout << "Warning: No supplier assigned for " << product->getName() << endl;
        cout << "Proceeding with purchase anyway..." << endl;
    } else {
        cout << "Ordering from supplier" << endl;
        cout << "Supplier Rating: " << supplier->getQualityRating() << "/5.0" << endl;
        cout << "Delivery Reliability: " << supplier->getDeliveryReliability() << endl;
    }

    int currentQty = product->getQuantity();

    if (product->buyItem(quantity)) {
        cout << "Success: Purchased " << quantity << " units of " << product->getName() << endl;
        cout << "Previous stock: " << currentQty << endl;
        cout << "New stock: " << product->getQuantity() << endl;
        return true;
    } else {
        cout << "Failed to purchase product." << endl;
        return false;
    }
}

// Sells product units (reduces quantity) and triggers low-stock check.
bool Inventory::sellProduct(string productId, int quantity) {
    Product* product = searchProduct(productId);

    if (product == nullptr) {
        cout << "Error: Product not found (ID: " << productId << ")" << endl;
        return false;
    }

    int currentQty = product->getQuantity();
    if (product->useItem(quantity)) {
        cout << "Success: Sold " << quantity << " units of " << product->getName() << endl;
        cout << "Remaining stock: " << product->getQuantity() << endl;
        checkLowStock();
        return true;
    } else {
        cout << "Error: Insufficient stock for " << product->getName() << endl;
        cout << "Available: " << currentQty << ", Requested: " << quantity << endl;

        Supplier* supplier = getSupplierForProduct(productId);
        if (supplier != nullptr) {
            cout << "Suggestion: Reorder from supplier (Rating: "
                 << supplier->getQualityRating() << ")" << endl;
        }
        return false;
    }
}

// Checks inventory for products below minimum quantity threshold.
void Inventory::checkLowStock(int threshold) {
    cout << "\n=== Low Stock Alert (Threshold: " << threshold << ") ===" << endl;
    bool foundLowStock = false;

    for (Product* p : ownedProductList) {
        if (p->getQuantity() < p->getMinQuantity()) {
            cout << "LOW STOCK: " << p->getName()
            << " (ID: " << p->getId() << ") - Quantity: "
            << p->getQuantity() << endl;

            Supplier* supplier = getSupplierForProduct(p->getId());
            if (supplier != nullptr) {
                cout << "  Supplier: Rating " << supplier->getQualityRating()
                << " | Reliability: " << supplier->getDeliveryReliability() << endl;
            } else {
                cout << "  Warning: No supplier assigned!" << endl;
            }
            foundLowStock = true;
        }
    }

    if (!foundLowStock) {
        cout << "All products are adequately stocked." << endl;
    }
}

// Generates a full inventory report, grouping products by supplier.
void Inventory::generateInventoryReport() const {
    cout << "\n========================================" << endl;
    cout << "      COMPREHENSIVE INVENTORY REPORT    " << endl;
    cout << "========================================" << endl;

    cout << "\nTotal Products: " << ownedProductList.size() << endl;
    cout << "Total Suppliers: " << supplierList.size() << endl;

    cout << "\n--- Products by Supplier ---" << endl;
    for (Supplier* supplier : supplierList) {
        cout << "\nSupplier (Rating: " << supplier->getQualityRating()
        << ", Reliability: " << supplier->getDeliveryReliability() << "):" << endl;

        set<Product*> products = getProductsFromSupplier(supplier);
        if (products.empty()) {
            cout << "  No products in inventory from this supplier." << endl;
        } else {
            for (Product* p : products) {
                cout << "  - " << p->getName() << " (ID: " << p->getId()
                << ", Qty: " << p->getQuantity() << ")" << endl;
            }
        }
    }

    cout << "\n--- Products Without Supplier ---" << endl;
    bool foundUnassigned = false;
    for (Product* p : ownedProductList) {
        if (getSupplierForProduct(p->getId()) == nullptr) {
            cout << "  - " << p->getName() << " (ID: " << p->getId() << ")" << endl;
            foundUnassigned = true;
        }
    }
    if (!foundUnassigned) {
        cout << "  All products have assigned suppliers." << endl;
    }
}

// Removes a product from inventory and from its supplier's record.
bool Inventory::removeProduct(string productId) {
    Product* product = searchProduct(productId);

    if (product == nullptr) {
        cout << "Error: Product not found (ID: " << productId << ")" << endl;
        return false;
    }

    auto it = ownedProductList.find(product);
    if (it != ownedProductList.end()) {
        ownedProductList.erase(it);
        cout << "Success: Product removed from inventory: " << product->getName()
             << " (ID: " << productId << ")" << endl;

        // Remove it from supplier’s record
        Supplier* supplier = getSupplierForProduct(productId);
        if (supplier != nullptr) {
            supplier->removeProductSupplied(productId);
            cout << "  - Product also removed from supplier's list" << endl;
        }
        return true;
    }

    return false;
}

// Removes a supplier and unlinks their associated products.
bool Inventory::removeSupplier(Supplier* supplier) {
    if (supplier == nullptr) {
        cout << "Error: Cannot remove null supplier." << endl;
        return false;
    }

    auto it = supplierList.find(supplier);
    if (it != supplierList.end()) {
        set<Product*> supplierProducts = getProductsFromSupplier(supplier);

        if (!supplierProducts.empty()) {
            cout << "Warning: Supplier provides " << supplierProducts.size()
            << " product(s) in inventory:" << endl;
            for (Product* p : supplierProducts) {
                cout << "  - " << p->getName() << " (ID: " << p->getId() << ")" << endl;
            }
            cout << "These products will no longer have an assigned supplier." << endl;
        }

        supplierList.erase(it);
        cout << "Success: Supplier removed from system." << endl;
        return true;
    }

    cout << "Error: Supplier not found in system." << endl;
    return false;
}
