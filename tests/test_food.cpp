/**
 * Unit Tests for Food Class
 * 
 * Tests cover:
 * - Constructor and basic getters
 * - Allergen management (add, remove, clear)
 * - Temperature and nutritional info
 * - Edge cases and boundary conditions
 */

#include <gtest/gtest.h>
#include "food.h"
#include <set>
#include <string>

// ============================================
// TEST FIXTURE - Sets up common test data
// ============================================
class FoodTest : public ::testing::Test {
protected:
    // This runs before each test
    void SetUp() override {
        // Create a test food item
        testFood = new Food("F001", "Apple", 1.99, "Fresh Farms");
    }
    
    // This runs after each test
    void TearDown() override {
        delete testFood;
    }
    
    // Test data available to all tests
    Food* testFood;
};

// ============================================
// CONSTRUCTOR AND BASIC GETTERS
// ============================================

TEST_F(FoodTest, ConstructorInitializesCorrectly) {
    // Test that constructor sets values properly
    EXPECT_EQ(testFood->getId(), "F001");
    EXPECT_EQ(testFood->getName(), "Apple");
    EXPECT_EQ(testFood->getPrice(), 1.99);
    EXPECT_EQ(testFood->getSupplier(), "Fresh Farms");
}

TEST_F(FoodTest, DefaultTemperatureIsZero) {
    // Default temperature should be 0
    EXPECT_EQ(testFood->getTemperatureRequired(), 0.0);
}

TEST_F(FoodTest, DefaultNutritionalInfoIsNotSpecified) {
    // Default nutritional info should be "Not Specified"
    EXPECT_EQ(testFood->getNutritionalInfo(), "Not Specified");
}

TEST_F(FoodTest, DefaultAllergensIsEmpty) {
    // Default allergens set should be empty
    std::set<std::string> allergens = testFood->getAllergens();
    EXPECT_TRUE(allergens.empty());
    EXPECT_EQ(allergens.size(), 0);
}

// ============================================
// ALLERGEN MANAGEMENT TESTS
// ============================================

TEST_F(FoodTest, AddSingleAllergen) {
    // Add one allergen
    testFood->addAllergen("Peanuts");
    
    std::set<std::string> allergens = testFood->getAllergens();
    EXPECT_EQ(allergens.size(), 1);
    EXPECT_TRUE(allergens.find("Peanuts") != allergens.end());
}

TEST_F(FoodTest, AddMultipleAllergens) {
    // Add multiple allergens
    testFood->addAllergen("Peanuts");
    testFood->addAllergen("Dairy");
    testFood->addAllergen("Gluten");
    
    std::set<std::string> allergens = testFood->getAllergens();
    EXPECT_EQ(allergens.size(), 3);
    EXPECT_TRUE(allergens.find("Peanuts") != allergens.end());
    EXPECT_TRUE(allergens.find("Dairy") != allergens.end());
    EXPECT_TRUE(allergens.find("Gluten") != allergens.end());
}

TEST_F(FoodTest, AddDuplicateAllergenDoesNotIncrease) {
    // std::set should prevent duplicates
    testFood->addAllergen("Peanuts");
    testFood->addAllergen("Peanuts");
    testFood->addAllergen("Peanuts");
    
    std::set<std::string> allergens = testFood->getAllergens();
    EXPECT_EQ(allergens.size(), 1); // Should only have 1, not 3
}

TEST_F(FoodTest, RemoveAllergen) {
    // Add and then remove an allergen
    testFood->addAllergen("Peanuts");
    testFood->addAllergen("Dairy");
    
    testFood->removeAllergen("Peanuts");
    
    std::set<std::string> allergens = testFood->getAllergens();
    EXPECT_EQ(allergens.size(), 1);
    EXPECT_TRUE(allergens.find("Peanuts") == allergens.end()); // Should not exist
    EXPECT_TRUE(allergens.find("Dairy") != allergens.end());   // Should still exist
}

TEST_F(FoodTest, RemoveNonExistentAllergen) {
    // Removing allergen that doesn't exist should not cause issues
    testFood->addAllergen("Peanuts");
    testFood->removeAllergen("Dairy"); // This doesn't exist
    
    std::set<std::string> allergens = testFood->getAllergens();
    EXPECT_EQ(allergens.size(), 1);
    EXPECT_TRUE(allergens.find("Peanuts") != allergens.end());
}

TEST_F(FoodTest, ClearAllAllergens) {
    // Add multiple allergens then clear all
    testFood->addAllergen("Peanuts");
    testFood->addAllergen("Dairy");
    testFood->addAllergen("Gluten");
    
    EXPECT_EQ(testFood->getAllergens().size(), 3);
    
    testFood->clearAllergens();
    
    EXPECT_TRUE(testFood->getAllergens().empty());
    EXPECT_EQ(testFood->getAllergens().size(), 0);
}

// ============================================
// TEMPERATURE TESTS
// ============================================

TEST_F(FoodTest, SetPositiveTemperature) {
    testFood->setTemperatureRequired(4.0); // Typical refrigerator temp
    EXPECT_EQ(testFood->getTemperatureRequired(), 4.0);
}

TEST_F(FoodTest, SetNegativeTemperature) {
    testFood->setTemperatureRequired(-18.0); // Freezer temp
    EXPECT_EQ(testFood->getTemperatureRequired(), -18.0);
}

TEST_F(FoodTest, SetZeroTemperature) {
    testFood->setTemperatureRequired(0.0);
    EXPECT_EQ(testFood->getTemperatureRequired(), 0.0);
}

TEST_F(FoodTest, SetRoomTemperature) {
    testFood->setTemperatureRequired(20.0);
    EXPECT_EQ(testFood->getTemperatureRequired(), 20.0);
}

// ============================================
// NUTRITIONAL INFO TESTS
// ============================================

TEST_F(FoodTest, SetNutritionalInfo) {
    std::string nutrition = "Calories: 95, Carbs: 25g, Fiber: 4g";
    testFood->setNutritionalInfo(nutrition);
    
    EXPECT_EQ(testFood->getNutritionalInfo(), nutrition);
}

TEST_F(FoodTest, SetEmptyNutritionalInfo) {
    testFood->setNutritionalInfo("");
    EXPECT_EQ(testFood->getNutritionalInfo(), "");
}

TEST_F(FoodTest, UpdateNutritionalInfo) {
    // Set initial info
    testFood->setNutritionalInfo("Initial info");
    EXPECT_EQ(testFood->getNutritionalInfo(), "Initial info");
    
    // Update it
    testFood->setNutritionalInfo("Updated info");
    EXPECT_EQ(testFood->getNutritionalInfo(), "Updated info");
}

// ============================================
// QUANTITY MANAGEMENT TESTS (Inherited from Perishable)
// ============================================

TEST_F(FoodTest, BuyItemIncreasesQuantity) {
    testFood->setTotalQuantity(100); // Set max quantity
    testFood->setQuantity(10);       // Current quantity
    
    bool success = testFood->buyItem(5);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(testFood->getQuantity(), 15);
}

TEST_F(FoodTest, BuyItemRespectMaxQuantity) {
    testFood->setTotalQuantity(20);  // Max is 20
    testFood->setQuantity(18);        // Current is 18
    
    bool success = testFood->buyItem(5); // Try to add 5 (would be 23)
    
    EXPECT_FALSE(success); // Should fail
    EXPECT_EQ(testFood->getQuantity(), 18); // Should remain unchanged
}

TEST_F(FoodTest, UseItemDecreasesQuantity) {
    testFood->setQuantity(10);
    
    bool success = testFood->useItem(3);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(testFood->getQuantity(), 7);
}

TEST_F(FoodTest, UseItemCannotExceedAvailable) {
    testFood->setQuantity(5);
    
    bool success = testFood->useItem(10); // Try to use more than available
    
    EXPECT_FALSE(success);
    EXPECT_EQ(testFood->getQuantity(), 5); // Should remain unchanged
}

TEST_F(FoodTest, UseItemExactAmount) {
    testFood->setQuantity(10);
    
    bool success = testFood->useItem(10); // Use exactly what's available
    
    EXPECT_TRUE(success);
    EXPECT_EQ(testFood->getQuantity(), 0);
}

// ============================================
// EDGE CASES
// ============================================

TEST_F(FoodTest, AddEmptyStringAllergen) {
    // Adding empty string - std::set should handle this
    testFood->addAllergen("");
    
    std::set<std::string> allergens = testFood->getAllergens();
    EXPECT_EQ(allergens.size(), 1);
    EXPECT_TRUE(allergens.find("") != allergens.end());
}

TEST_F(FoodTest, LongNutritionalInfoString) {
    // Test with very long string
    std::string longInfo = "This is a very long nutritional information string that contains "
                           "detailed information about calories, proteins, fats, carbohydrates, "
                           "vitamins, minerals, and other dietary components.";
    
    testFood->setNutritionalInfo(longInfo);
    EXPECT_EQ(testFood->getNutritionalInfo(), longInfo);
}

TEST_F(FoodTest, VeryLowTemperature) {
    // Test extreme low temperature
    testFood->setTemperatureRequired(-273.15); // Absolute zero
    EXPECT_EQ(testFood->getTemperatureRequired(), -273.15);
}

TEST_F(FoodTest, VeryHighTemperature) {
    // Test extreme high temperature
    testFood->setTemperatureRequired(1000.0);
    EXPECT_EQ(testFood->getTemperatureRequired(), 1000.0);
}

// ============================================
// INTEGRATION TEST - Realistic Scenario
// ============================================

TEST_F(FoodTest, RealisticFoodItemSetup) {
    // Create a realistic food item with all attributes
    Food* milk = new Food("F100", "Whole Milk", 3.99, "Dairy Co");
    
    // Set realistic values
    milk->setTemperatureRequired(4.0);
    milk->setNutritionalInfo("Calories: 150, Fat: 8g, Protein: 8g, Calcium: 30% DV");
    milk->addAllergen("Dairy");
    milk->addAllergen("Lactose");
    milk->setTotalQuantity(50);
    milk->setMinimumQuantity(10);
    milk->setQuantity(25);
    milk->setExpirationDate("2024-12-31");
    milk->setStorageCondition("Keep refrigerated");
    
    // Verify everything
    EXPECT_EQ(milk->getId(), "F100");
    EXPECT_EQ(milk->getName(), "Whole Milk");
    EXPECT_EQ(milk->getPrice(), 3.99);
    EXPECT_EQ(milk->getTemperatureRequired(), 4.0);
    EXPECT_EQ(milk->getAllergens().size(), 2);
    EXPECT_EQ(milk->getQuantity(), 25);
    EXPECT_EQ(milk->getMinQuantity(), 10);
    EXPECT_EQ(milk->getMaxQuantity(), 50);
    
    delete milk;
}