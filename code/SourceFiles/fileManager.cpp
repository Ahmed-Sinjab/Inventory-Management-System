/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description: Implements FileManager for saving/loading complete inventory data
 * Works with public Inventory interface only (no friend class needed)
 */

#include "fileManager.h"
#include "rawmaterial.h"
#include "chemical.h"
#include "food.h"
#include "nonperishable.h"
#include "perishable.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

FileManager::FileManager() {}

bool FileManager::saveInventory(Inventory* inventory, const string& filename) {
    if (inventory == nullptr) {
        cout << "Error: Cannot save null inventory." << endl;
        return false;
    }

    ofstream outFile(filename);

    if (!outFile.is_open()) {
        cout << "Error: Could not create/open file for writing: " << filename << endl;
        return false;
    }

    cout << "Saving inventory to file: " << filename << endl;
    outFile << fixed << setprecision(2);

    // Get products using public interface
    set<Product*> products = inventory->getProductList();

    // Collect unique suppliers by checking each product
    set<Supplier*> uniqueSuppliers;
    for (Product* p : products) {
        Supplier* s = inventory->getSupplierForProduct(p->getId());
        if (s != nullptr) {
            uniqueSuppliers.insert(s);
        }
    }

    // ========== SAVE SUPPLIERS ==========
    outFile << "SUPPLIERS" << endl;
    outFile << uniqueSuppliers.size() << endl;

    for (Supplier* s : uniqueSuppliers) {
        outFile << s->getQualityRating() << "|"
                << s->getDeliveryReliability() << "|";

        set<string> productIds = s->getProductIdsSupplied();
        outFile << productIds.size();
        for (const string& id : productIds) {
            outFile << "|" << id;
        }
        outFile << endl;
    }

    // ========== SAVE PRODUCTS ==========
    outFile << "PRODUCTS" << endl;
    outFile << products.size() << endl;

    for (Product* p : products) {
        string productType = getProductType(p);

        if (productType == "RawMaterial") {
            RawMaterial* rm = dynamic_cast<RawMaterial*>(p);
            outFile << "RawMaterial|"
                    << rm->getId() << "|"
                    << rm->getName() << "|"
                    << rm->getPrice() << "|"
                    << rm->getSupplier() << "|"
                    << rm->getQuantity() << "|"
                    << rm->getMinQuantity() << "|"
                    << rm->getMaxQuantity() << "|"
                    << rm->getUnit() << "|"
                    << rm->getPurity() << endl;
        }
        else if (productType == "Chemical") {
            Chemical* ch = dynamic_cast<Chemical*>(p);
            outFile << "Chemical|"
                    << ch->getId() << "|"
                    << ch->getName() << "|"
                    << ch->getPrice() << "|"
                    << ch->getSupplier() << "|"
                    << ch->getQuantity() << "|"
                    << ch->getMinQuantity() << "|"
                    << ch->getMaxQuantity() << "|"
                    << ch->getExpirationDate() << "|"
                    << ch->getStorageCondition() << "|"
                    << ch->getHazardLevel() << "|"
                    << (ch->isChildSafe() ? "1" : "0") << "|"
                    << (ch->requiresVentilation() ? "1" : "0") << "|"
                    << ch->getDisposalType() << "|";

            set<string> warnings = ch->getSafetyWarnings();
            outFile << warnings.size();
            for (const string& warning : warnings) {
                outFile << "|" << warning;
            }
            outFile << endl;
        }
        else if (productType == "Food") {
            Food* f = dynamic_cast<Food*>(p);
            outFile << "Food|"
                    << f->getId() << "|"
                    << f->getName() << "|"
                    << f->getPrice() << "|"
                    << f->getSupplier() << "|"
                    << f->getQuantity() << "|"
                    << f->getMinQuantity() << "|"
                    << f->getMaxQuantity() << "|"
                    << f->getExpirationDate() << "|"
                    << f->getStorageCondition() << "|"
                    << f->getTemperatureRequired() << "|"
                    << f->getNutritionalInfo() << "|";

            set<string> allergens = f->getAllergens();
            outFile << allergens.size();
            for (const string& allergen : allergens) {
                outFile << "|" << allergen;
            }
            outFile << endl;
        }
        else if (productType == "NonPerishable") {
            NonPerishable* np = dynamic_cast<NonPerishable*>(p);
            outFile << "NonPerishable|"
                    << np->getId() << "|"
                    << np->getName() << "|"
                    << np->getPrice() << "|"
                    << np->getSupplier() << "|"
                    << np->getQuantity() << "|"
                    << np->getMinQuantity() << "|"
                    << np->getMaxQuantity() << "|"
                    << (np->getFragile() ? "1" : "0") << endl;
        }
        else if (productType == "Perishable") {
            Perishable* per = dynamic_cast<Perishable*>(p);
            outFile << "Perishable|"
                    << per->getId() << "|"
                    << per->getName() << "|"
                    << per->getPrice() << "|"
                    << per->getSupplier() << "|"
                    << per->getQuantity() << "|"
                    << per->getMinQuantity() << "|"
                    << per->getMaxQuantity() << "|"
                    << per->getExpirationDate() << "|"
                    << per->getStorageCondition() << endl;
        }
    }

    outFile.close();
    cout << "Successfully saved " << products.size() << " products and "
         << uniqueSuppliers.size() << " suppliers to " << filename << endl;

    return true;
}

bool FileManager::loadInventory(Inventory* inventory, const string& filename) {
    if (inventory == nullptr) {
        cout << "Error: Cannot load into null inventory." << endl;
        return false;
    }

    ifstream inFile(filename);

    if (!inFile.is_open()) {
        cout << "Warning: Could not open file for reading: " << filename << endl;
        cout << "File may not exist yet. This is normal for first run." << endl;
        return false;
    }

    cout << "Loading inventory from file: " << filename << endl;

    string line;
    vector<Supplier*> loadedSuppliers;
    productSupplierMap.clear();

    // ========== LOAD SUPPLIERS ==========
    getline(inFile, line);
    if (line != "SUPPLIERS") {
        cout << "Error: Invalid file format (expected SUPPLIERS)" << endl;
        inFile.close();
        return false;
    }

    int numSuppliers;
    inFile >> numSuppliers;
    inFile.ignore();

    for (int i = 0; i < numSuppliers; i++) {
        getline(inFile, line);
        stringstream ss(line);
        string reliability, token;
        double rating;
        int numProducts;

        ss >> rating;
        ss.ignore();
        getline(ss, reliability, '|');
        ss >> numProducts;
        ss.ignore();

        Supplier* supplier = new Supplier();
        supplier->setQualityRating(rating);
        supplier->setDeliveryReliability(reliability);

        // Store product IDs that this supplier provides
        vector<string> productIdsForThisSupplier;
        for (int j = 0; j < numProducts; j++) {
            getline(ss, token, '|');
            productIdsForThisSupplier.push_back(token);
        }

        // Add supplier using public interface
        inventory->addSupplier(supplier);
        loadedSuppliers.push_back(supplier);

        // Mark these products as supplied by this supplier
        for (const string& prodId : productIdsForThisSupplier) {
            supplier->addProductSupplied(prodId);
        }
    }

    // ========== LOAD PRODUCTS ==========
    getline(inFile, line);
    if (line != "PRODUCTS") {
        cout << "Error: Invalid file format (expected PRODUCTS)" << endl;
        inFile.close();
        return false;
    }

    int numProducts;
    inFile >> numProducts;
    inFile.ignore();

    for (int i = 0; i < numProducts; i++) {
        getline(inFile, line);
        Product* product = createProductFromLine(line);

        if (product != nullptr) {
            // Add product using public interface
            inventory->addProduct(product);
        }
    }

    inFile.close();
    cout << "Successfully loaded " << numProducts << " products and "
         << numSuppliers << " suppliers from " << filename << endl;

    return true;
}

string FileManager::getProductType(Product* product) const {
    if (dynamic_cast<RawMaterial*>(product)) return "RawMaterial";
    if (dynamic_cast<Chemical*>(product)) return "Chemical";
    if (dynamic_cast<Food*>(product)) return "Food";
    if (dynamic_cast<NonPerishable*>(product)) return "NonPerishable";
    if (dynamic_cast<Perishable*>(product)) return "Perishable";
    return "Unknown";
}

Product* FileManager::createProductFromLine(const string& line) {
    stringstream ss(line);
    string type;
    getline(ss, type, '|');

    // ========== RAW MATERIAL ==========
    if (type == "RawMaterial") {
        string id, name, supplier, unit;
        double price;
        int quantity, minQty, maxQty, purity;

        getline(ss, id, '|');
        getline(ss, name, '|');
        ss >> price;
        ss.ignore();
        getline(ss, supplier, '|');
        ss >> quantity;
        ss.ignore();
        ss >> minQty;
        ss.ignore();
        ss >> maxQty;
        ss.ignore();
        getline(ss, unit, '|');
        ss >> purity;

        Product* p = factory.createProduct(productType::RAW_MATERIAL, id, name, price, supplier);
        RawMaterial* rm = dynamic_cast<RawMaterial*>(p);

        if (rm != nullptr) {
            rm->setQuantity(quantity);
            rm->setMinimumQuantity(minQty);
            rm->setTotalQuantity(maxQty);
            rm->setUnit(unit);
            rm->setPurity(purity);
        }

        return p;
    }

    // ========== CHEMICAL ==========
    else if (type == "Chemical") {
        string id, name, supplier, expDate, storage, disposal, token;
        double price;
        int quantity, hazard, childSafeInt, ventReqInt, minQty, maxQty, numWarnings;

        getline(ss, id, '|');
        getline(ss, name, '|');
        ss >> price;
        ss.ignore();
        getline(ss, supplier, '|');
        ss >> quantity;
        ss.ignore();
        ss >> minQty;
        ss.ignore();
        ss >> maxQty;
        ss.ignore();
        getline(ss, expDate, '|');
        getline(ss, storage, '|');
        ss >> hazard;
        ss.ignore();
        ss >> childSafeInt;
        ss.ignore();
        ss >> ventReqInt;
        ss.ignore();
        getline(ss, disposal, '|');
        ss >> numWarnings;
        ss.ignore();

        Product* p = factory.createProduct(productType::CHEMICAL, id, name, price, supplier);
        Chemical* ch = dynamic_cast<Chemical*>(p);

        if (ch != nullptr) {
            ch->setQuantity(quantity);
            ch->setMinimumQuantity(minQty);
            ch->setTotalQuantity(maxQty);
            ch->setExpirationDate(expDate);
            ch->setStorageCondition(storage);
            ch->setHazardLevel(hazard);
            ch->setChildSafe(childSafeInt == 1);
            ch->setVentilationRequired(ventReqInt == 1);
            ch->setDisposalType(disposal);

            for (int i = 0; i < numWarnings; i++) {
                getline(ss, token, '|');
                ch->addSafetyWarning(token);
            }
        }

        return p;
    }

    // ========== FOOD ==========
    else if (type == "Food") {
        string id, name, supplier, expDate, storage, nutrition, token;
        double price, temp;
        int quantity, minQty, maxQty, numAllergens;

        getline(ss, id, '|');
        getline(ss, name, '|');
        ss >> price;
        ss.ignore();
        getline(ss, supplier, '|');
        ss >> quantity;
        ss.ignore();
        ss >> minQty;
        ss.ignore();
        ss >> maxQty;
        ss.ignore();
        getline(ss, expDate, '|');
        getline(ss, storage, '|');
        ss >> temp;
        ss.ignore();
        getline(ss, nutrition, '|');
        ss >> numAllergens;
        ss.ignore();

        Product* p = factory.createProduct(productType::FOOD, id, name, price, supplier);
        Food* f = dynamic_cast<Food*>(p);

        if (f != nullptr) {
            f->setQuantity(quantity);
            f->setMinimumQuantity(minQty);
            f->setTotalQuantity(maxQty);
            f->setExpirationDate(expDate);
            f->setStorageCondition(storage);
            f->setTemperatureRequired(temp);
            f->setNutritionalInfo(nutrition);

            for (int i = 0; i < numAllergens; i++) {
                getline(ss, token, '|');
                f->addAllergen(token);
            }
        }

        return p;
    }

    // ========== NON-PERISHABLE ==========
    else if (type == "NonPerishable") {
        string id, name, supplier;
        double price;
        int quantity, minQty, maxQty, fragileInt;

        getline(ss, id, '|');
        getline(ss, name, '|');
        ss >> price;
        ss.ignore();
        getline(ss, supplier, '|');
        ss >> quantity;
        ss.ignore();
        ss >> minQty;
        ss.ignore();
        ss >> maxQty;
        ss.ignore();
        ss >> fragileInt;

        Product* p = factory.createProduct(productType::NON_PERISHABLE, id, name, price, supplier);
        NonPerishable* np = dynamic_cast<NonPerishable*>(p);

        if (np != nullptr) {
            np->setQuantity(quantity);
            np->setMinimumQuantity(minQty);
            np->setTotalQuantity(maxQty);
            np->setFragile(fragileInt == 1);
        }

        return p;
    }

    // ========== PERISHABLE ==========
    else if (type == "Perishable") {
        string id, name, supplier, expDate, storage;
        double price;
        int quantity, minQty, maxQty;

        getline(ss, id, '|');
        getline(ss, name, '|');
        ss >> price;
        ss.ignore();
        getline(ss, supplier, '|');
        ss >> quantity;
        ss.ignore();
        ss >> minQty;
        ss.ignore();
        ss >> maxQty;
        ss.ignore();
        getline(ss, expDate, '|');
        getline(ss, storage);

        Product* p = factory.createProduct(productType::PERISHABLE, id, name, price, supplier);
        Perishable* per = dynamic_cast<Perishable*>(p);

        if (per != nullptr) {
            per->setQuantity(quantity);
            per->setMinimumQuantity(minQty);
            per->setTotalQuantity(maxQty);
            per->setExpirationDate(expDate);
            per->setStorageCondition(storage);
        }

        return p;
    }

    return nullptr;
}
