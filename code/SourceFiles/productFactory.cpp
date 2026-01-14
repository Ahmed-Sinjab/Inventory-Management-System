#include "productFactory.h"
#include <iostream>

Product* productFactory::createProduct(productType type,
                                       std::string id,
                                       std::string name,
                                       double price,
                                       std::string supplier) {
    std::string finalId;

    switch(type) {
    case productType::RAW_MATERIAL:
        //  Check if ID already has RM prefix
        if (id.length() >= 2 && id.substr(0, 2) == "RM") {
            finalId = id;  // Already has prefix
            std::cout << "[FACTORY] RAW_MATERIAL: ID already has prefix: " << id << std::endl;
        } else {
            finalId = "RM" + id;  // Add prefix
            std::cout << "[FACTORY] RAW_MATERIAL: Adding prefix: " << id << " -> " << finalId << std::endl;
        }
        return new RawMaterial(finalId, name, price, supplier);

    case productType::NON_PERISHABLE:
        //  Check if ID already has NP prefix
        if (id.length() >= 2 && id.substr(0, 2) == "NP") {
            finalId = id;  // Already has prefix
            std::cout << "[FACTORY] NON_PERISHABLE: ID already has prefix: " << id << std::endl;
        } else {
            finalId = "NP" + id;  // Add prefix
            std::cout << "[FACTORY] NON_PERISHABLE: Adding prefix: " << id << " -> " << finalId << std::endl;
        }
        return new NonPerishable(finalId, name, price, supplier);

    case productType::PERISHABLE:
        //  Check if ID already has P prefix (but not NP or RM starting with P)
        if (id.length() >= 1 && id[0] == 'P' &&
            !(id.length() >= 2 && (id[1] == 'N' || id[1] == 'M'))) {
            finalId = id;  // Already has prefix
            std::cout << "[FACTORY] PERISHABLE: ID already has prefix: " << id << std::endl;
        } else {
            finalId = "P" + id;  // Add prefix
            std::cout << "[FACTORY] PERISHABLE: Adding prefix: " << id << " -> " << finalId << std::endl;
        }
        return new Perishable(finalId, name, price, supplier);

    case productType::FOOD:
        //  Check if ID already has F prefix
        if (id.length() >= 1 && id[0] == 'F') {
            finalId = id;  // Already has prefix
            std::cout << "[FACTORY] FOOD: ID already has prefix: " << id << std::endl;
        } else {
            finalId = "F" + id;  // Add prefix
            std::cout << "[FACTORY] FOOD: Adding prefix: " << id << " -> " << finalId << std::endl;
        }
        return new Food(finalId, name, price, supplier);

    case productType::CHEMICAL:
        //  Check if ID already has C prefix
        if (id.length() >= 1 && id[0] == 'C') {
            finalId = id;  // Already has prefix
            std::cout << "[FACTORY] CHEMICAL: ID already has prefix: " << id << std::endl;
        } else {
            finalId = "C" + id;  // Add prefix
            std::cout << "[FACTORY] CHEMICAL: Adding prefix: " << id << " -> " << finalId << std::endl;
        }
        return new Chemical(finalId, name, price, supplier);

    default:
        std::cerr << "[FACTORY] Error: Unknown product type!" << std::endl;
        return nullptr;
    }
}
