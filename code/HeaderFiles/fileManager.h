/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Creation Date: December 12, 2024
 * Last Edit: December 12, 2024
 * Purpose: Manages saving and loading of inventory data to/from files using ProductFactory
 *
 * Dependencies:
 *  - inventory.h : Inventory class
 *  - productFactory.h : Factory for creating products
 *  - <string>
 */

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "inventory.h"
#include "productFactory.h"
#include <string>
#include <set>
#include <map>

class FileManager {
public:
    FileManager();

    // Save/Load inventory
    bool saveInventory(Inventory* inventory, const std::string& filename);
    bool loadInventory(Inventory* inventory, const std::string& filename);

private:
    productFactory factory;

    // Helper methods
    std::string getProductType(Product* product) const;
    Product* createProductFromLine(const std::string& line);

    // Temporary storage for linking products to suppliers during load
    std::map<std::string, std::string> productSupplierMap;
};

#endif // FILEMANAGER_H
