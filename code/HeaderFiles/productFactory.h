#ifndef PRODUCTFACTORY_H
#define PRODUCTFACTORY_H

#include "product.h"
#include "rawMaterial.h"
#include "nonPerishable.h"
#include "perishable.h"
#include "food.h"
#include "chemical.h"
#include <string>

enum class productType {
    RAW_MATERIAL,
    NON_PERISHABLE,
    PERISHABLE,
    FOOD,
    CHEMICAL
};

class productFactory {
public:
    static Product* createProduct(productType type,
                                  std::string id,
                                  std::string name,
                                  double price,
                                  std::string supplier);
};

#endif // PRODUCTFACTORY_H
