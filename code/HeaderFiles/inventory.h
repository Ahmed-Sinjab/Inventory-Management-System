#ifndef INVENTORY_H
#define INVENTORY_H

#include "product.h"
#include "supplier.h"
#include <set>

class Inventory {
private:
    std::set<Product*> ownedProductList;
    std::set<Supplier*> supplierList;

public:
    Inventory();
    ~Inventory();

    // Product management
    void addProduct(Product* product);
    void addProductWithSupplier(Product* product, Supplier* supplier);
    Product* searchProduct(std::string productId) const;
    void displayAllProducts() const;
    std::set<Product*> getProductList() const;
    bool removeProduct(std::string productId);
    bool removeSupplier(Supplier* supplier);

    // Supplier management
    void addSupplier(Supplier* supplier);
    void displayAllSuppliers() const;

    // Relationship queries
    Supplier* getSupplierForProduct(std::string productId) const;
    std::set<Supplier*> getSupplierList() const;
    std::set<Product*> getProductsFromSupplier(Supplier* supplier) const;

    // Operations
    bool buyProduct(std::string productId, int quantity);
    bool sellProduct(std::string productId, int quantity);
    void checkLowStock(int threshold = 10);
    void generateInventoryReport() const;
};

#endif
