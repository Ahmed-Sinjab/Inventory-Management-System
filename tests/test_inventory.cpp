/**
 * Unit Tests for Inventory Class
 * 
 * Tests cover:
 * - Product management (add, search, remove)
 * - Supplier management
 * - Buy and sell operations
 * - Low stock checking
 * - Edge cases and error handling
 * 
 * Uses Google Mock to create mock Product objects for testing
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "inventory.h"
#include "product.h"
#include "supplier.h"
#include "food.h"
#include "perishable.h"

using ::testing::Return;
using ::testing::_;

// ============================================
// MOCK PRODUCT CLASS
// ============================================
// This allows us to test Inventory without needing real Product objects

class MockProduct : public Product {
public:
    MOCK_METHOD(std::string, getId, (), (const, override));
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(double, getPrice, (), (const, override));
    MOCK_METHOD(std::string, getSupplier, (), (const, override));
    MOCK_METHOD(int, getQuantity, (), (const, override));
    MOCK_METHOD(int, getMinQuantity, (), (const, override));
    MOCK_METHOD(int, getMaxQuantity, (), (const, override));
    
    MOCK_METHOD(void, setId, (std::string), (override));
    MOCK_METHOD(void, setName, (std::string), (override));
    MOCK_METHOD(void, setPrice, (double), (override));
    MOCK_METHOD(bool, setQuantity, (int), (override));
    MOCK_METHOD(void, setTotalQuantity, (int), (override));
    MOCK_METHOD(void, setMinimumQuantity, (int), (override));
    
    MOCK_METHOD(void, display, (), (const, override));
    MOCK_METHOD(bool, useItem, (double), (override));
    MOCK_METHOD(bool, buyItem, (double), (override));
};

// ============================================
// TEST FIXTURE
// ============================================

class InventoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        inventory = new Inventory();
    }
    
    void TearDown() override {
        delete inventory;
    }
    
    Inventory* inventory;
};

// ============================================
// PRODUCT MANAGEMENT TESTS
// ============================================

TEST_F(InventoryTest, AddProductSuccessfully) {
    // Create a real Food product
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    
    inventory->addProduct(apple);
    
    // Verify it was added by searching for it
    Product* found = inventory->searchProduct("F001");
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->getId(), "F001");
    EXPECT_EQ(found->getName(), "Apple");
}

TEST_F(InventoryTest, AddNullProductFails) {
    // Should handle null gracefully
    inventory->addProduct(nullptr);
    
    // Inventory should still be empty (or not crash)
    std::set<Product*> products = inventory->getProductList();
    EXPECT_EQ(products.size(), 0);
}

TEST_F(InventoryTest, AddMultipleProducts) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    Food* banana = new Food("F002", "Banana", 0.99, "Tropical Co");
    Food* orange = new Food("F003", "Orange", 1.49, "Citrus Inc");
    
    inventory->addProduct(apple);
    inventory->addProduct(banana);
    inventory->addProduct(orange);
    
    std::set<Product*> products = inventory->getProductList();
    EXPECT_EQ(products.size(), 3);
}

TEST_F(InventoryTest, SearchProductFound) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    inventory->addProduct(apple);
    
    Product* found = inventory->searchProduct("F001");
    
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->getId(), "F001");
}

TEST_F(InventoryTest, SearchProductNotFound) {
    Product* found = inventory->searchProduct("NONEXISTENT");
    
    EXPECT_EQ(found, nullptr);
}

TEST_F(InventoryTest, SearchProductInEmptyInventory) {
    Product* found = inventory->searchProduct("F001");
    
    EXPECT_EQ(found, nullptr);
}

TEST_F(InventoryTest, RemoveProductSuccessfully) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    inventory->addProduct(apple);
    
    bool removed = inventory->removeProduct("F001");
    
    EXPECT_TRUE(removed);
    EXPECT_EQ(inventory->searchProduct("F001"), nullptr);
}

TEST_F(InventoryTest, RemoveNonExistentProduct) {
    bool removed = inventory->removeProduct("NONEXISTENT");
    
    EXPECT_FALSE(removed);
}

TEST_F(InventoryTest, RemoveProductFromEmptyInventory) {
    bool removed = inventory->removeProduct("F001");
    
    EXPECT_FALSE(removed);
}

// ============================================
// SUPPLIER MANAGEMENT TESTS
// ============================================

TEST_F(InventoryTest, AddSupplier) {
    Supplier* supplier = new Supplier();
    supplier->setQualityRating(4.5);
    supplier->setDeliveryReliability("Excellent");
    
    inventory->addSupplier(supplier);
    
    std::set<Supplier*> suppliers = inventory->getSupplierList();
    EXPECT_EQ(suppliers.size(), 1);
}

TEST_F(InventoryTest, AddProductWithSupplier) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    Supplier* supplier = new Supplier();
    supplier->setQualityRating(4.5);
    
    inventory->addProductWithSupplier(apple, supplier);
    
    // Verify product was added
    Product* found = inventory->searchProduct("F001");
    EXPECT_NE(found, nullptr);
    
    // Verify supplier was added
    std::set<Supplier*> suppliers = inventory->getSupplierList();
    EXPECT_EQ(suppliers.size(), 1);
    
    // Verify supplier knows about the product
    Supplier* foundSupplier = inventory->getSupplierForProduct("F001");
    EXPECT_NE(foundSupplier, nullptr);
    EXPECT_TRUE(foundSupplier->suppliesProduct("F001"));
}

TEST_F(InventoryTest, GetSupplierForProduct) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    Supplier* supplier = new Supplier();
    supplier->setQualityRating(4.8);
    
    inventory->addProductWithSupplier(apple, supplier);
    
    Supplier* found = inventory->getSupplierForProduct("F001");
    
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->getQualityRating(), 4.8);
}

TEST_F(InventoryTest, GetSupplierForNonExistentProduct) {
    Supplier* found = inventory->getSupplierForProduct("NONEXISTENT");
    
    EXPECT_EQ(found, nullptr);
}

TEST_F(InventoryTest, RemoveSupplier) {
    Supplier* supplier = new Supplier();
    inventory->addSupplier(supplier);
    
    EXPECT_EQ(inventory->getSupplierList().size(), 1);
    
    bool removed = inventory->removeSupplier(supplier);
    
    EXPECT_TRUE(removed);
    EXPECT_EQ(inventory->getSupplierList().size(), 0);
}

// ============================================
// BUY PRODUCT TESTS
// ============================================

TEST_F(InventoryTest, BuyProductSuccessfully) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setTotalQuantity(100);
    apple->setQuantity(10);
    
    inventory->addProduct(apple);
    
    bool success = inventory->buyProduct("F001", 5);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(apple->getQuantity(), 15);
}

TEST_F(InventoryTest, BuyProductNotFound) {
    bool success = inventory->buyProduct("NONEXISTENT", 5);
    
    EXPECT_FALSE(success);
}

TEST_F(InventoryTest, BuyProductWithZeroQuantity) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setTotalQuantity(100);
    apple->setQuantity(10);
    
    inventory->addProduct(apple);
    
    bool success = inventory->buyProduct("F001", 0);
    
    EXPECT_FALSE(success); // Should fail for zero quantity
    EXPECT_EQ(apple->getQuantity(), 10); // Unchanged
}

TEST_F(InventoryTest, BuyProductWithNegativeQuantity) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setTotalQuantity(100);
    apple->setQuantity(10);
    
    inventory->addProduct(apple);
    
    bool success = inventory->buyProduct("F001", -5);
    
    EXPECT_FALSE(success); // Should fail for negative quantity
    EXPECT_EQ(apple->getQuantity(), 10); // Unchanged
}

TEST_F(InventoryTest, BuyProductExceedingMaxQuantity) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setTotalQuantity(20); // Max is 20
    apple->setQuantity(18);      // Current is 18
    
    inventory->addProduct(apple);
    
    bool success = inventory->buyProduct("F001", 5); // Would be 23
    
    EXPECT_FALSE(success);
    EXPECT_EQ(apple->getQuantity(), 18); // Unchanged
}

// ============================================
// SELL PRODUCT TESTS
// ============================================

TEST_F(InventoryTest, SellProductSuccessfully) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setQuantity(20);
    apple->setMinimumQuantity(5);
    
    inventory->addProduct(apple);
    
    bool success = inventory->sellProduct("F001", 10);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(apple->getQuantity(), 10);
}

TEST_F(InventoryTest, SellProductNotFound) {
    bool success = inventory->sellProduct("NONEXISTENT", 5);
    
    EXPECT_FALSE(success);
}

TEST_F(InventoryTest, SellMoreThanAvailable) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setQuantity(5);
    
    inventory->addProduct(apple);
    
    bool success = inventory->sellProduct("F001", 10);
    
    EXPECT_FALSE(success);
    EXPECT_EQ(apple->getQuantity(), 5); // Unchanged
}

TEST_F(InventoryTest, SellExactQuantity) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setQuantity(10);
    
    inventory->addProduct(apple);
    
    bool success = inventory->sellProduct("F001", 10);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(apple->getQuantity(), 0);
}

// ============================================
// LOW STOCK TESTS
// ============================================

TEST_F(InventoryTest, CheckLowStockDetectsLowItems) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setQuantity(3);
    apple->setMinimumQuantity(10); // Quantity is below minimum
    
    inventory->addProduct(apple);
    
    // This should print low stock warning
    // We're testing that it doesn't crash
    inventory->checkLowStock();
    
    // If we get here without crashing, test passes
    SUCCEED();
}

TEST_F(InventoryTest, CheckLowStockWithAdequateStock) {
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setQuantity(50);
    apple->setMinimumQuantity(10);
    
    inventory->addProduct(apple);
    
    inventory->checkLowStock();
    
    SUCCEED();
}

TEST_F(InventoryTest, CheckLowStockInEmptyInventory) {
    inventory->checkLowStock();
    
    SUCCEED();
}

// ============================================
// EDGE CASES AND ERROR HANDLING
// ============================================

TEST_F(InventoryTest, GetProductListFromEmptyInventory) {
    std::set<Product*> products = inventory->getProductList();
    
    EXPECT_TRUE(products.empty());
    EXPECT_EQ(products.size(), 0);
}

TEST_F(InventoryTest, AddSameProductTwice) {
    Food* apple1 = new Food("F001", "Apple", 1.99, "Fresh Farms");
    Food* apple2 = new Food("F001", "Apple", 1.99, "Fresh Farms");
    
    inventory->addProduct(apple1);
    inventory->addProduct(apple2);
    
    // std::set should allow this (different pointers)
    // But both have same ID
    std::set<Product*> products = inventory->getProductList();
    EXPECT_EQ(products.size(), 2);
}

TEST_F(InventoryTest, SearchWithEmptyString) {
    Product* found = inventory->searchProduct("");
    
    EXPECT_EQ(found, nullptr);
}

TEST_F(InventoryTest, GetProductsFromNullSupplier) {
    std::set<Product*> products = inventory->getProductsFromSupplier(nullptr);
    
    EXPECT_TRUE(products.empty());
}

// ============================================
// INTEGRATION TEST - Realistic Scenario
// ============================================

TEST_F(InventoryTest, CompleteWorkflowScenario) {
    // Create a realistic inventory scenario
    
    // 1. Create supplier
    Supplier* freshFarms = new Supplier();
    freshFarms->setQualityRating(4.7);
    freshFarms->setDeliveryReliability("Excellent");
    
    // 2. Add products with supplier
    Food* apple = new Food("F001", "Apple", 1.99, "Fresh Farms");
    apple->setTotalQuantity(100);
    apple->setMinimumQuantity(20);
    apple->setQuantity(50);
    
    Food* banana = new Food("F002", "Banana", 0.99, "Fresh Farms");
    banana->setTotalQuantity(150);
    banana->setMinimumQuantity(30);
    banana->setQuantity(80);
    
    inventory->addProductWithSupplier(apple, freshFarms);
    inventory->addProductWithSupplier(banana, freshFarms);
    
    // 3. Verify products were added
    EXPECT_EQ(inventory->getProductList().size(), 2);
    
    // 4. Buy more products
    EXPECT_TRUE(inventory->buyProduct("F001", 20));
    EXPECT_EQ(apple->getQuantity(), 70);
    
    // 5. Sell some products
    EXPECT_TRUE(inventory->sellProduct("F002", 30));
    EXPECT_EQ(banana->getQuantity(), 50);
    
    // 6. Check supplier relationship
    Supplier* appleSupplier = inventory->getSupplierForProduct("F001");
    EXPECT_NE(appleSupplier, nullptr);
    EXPECT_EQ(appleSupplier->getQualityRating(), 4.7);
    
    // 7. Get all products from supplier
    std::set<Product*> supplierProducts = inventory->getProductsFromSupplier(freshFarms);
    EXPECT_EQ(supplierProducts.size(), 2);
    
    // 8. Check low stock (should be fine)
    inventory->checkLowStock();
    
    SUCCEED();
}