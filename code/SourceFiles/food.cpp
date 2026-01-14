/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description:
 *     Implements the Food class, which inherits from the Perishable class.
 *     The Food class represents edible perishable products that have specific storage
 *     conditions, nutritional information, and potential allergens.
 *     It expands upon the Perishable base class by adding food-related attributes
 *     and providing additional methods for allergen management.
 *
 * Dependencies:
 *     - food.h        : Declares the Food class interface.
 *     - <iostream>    : Used for console output when displaying product details.
 *     - <string>      : Used for storing and handling text attributes.
 *     - <set>         : Used for maintaining a collection of unique allergens.
 */

#include "food.h"
#include <iostream>
#include <string>
#include <set>
using namespace std;

// Constructor: initializes a Food object and calls the Perishable base class constructor.
// Default values for temperatureRequired and nutritionalInfo are set.
Food::Food(std::string id, std::string name, double price, std::string supplier)
    : Perishable(id, name, price, supplier),   // Call to the Perishable base class constructor
    temperatureRequired(0),
    nutritionalInfo("Not Specified")
{}

// Displays detailed information about the food product.
// Includes inherited perishable info and adds food-specific details such as
// allergens, temperature, and nutritional information.
void Food::display() const {
    Perishable::display();  // Show inherited perishable attributes first
    cout << "Temperature Required: " << temperatureRequired << " Degrees Celsius" << endl;
    cout << "Nutritional Info: " << nutritionalInfo << endl;

    // Display allergen list if available
    cout << "Allergens: ";
    if (allergens.empty()) {
        cout << "None" << endl;
    } else {
        bool first = true;
        for (const auto& allergen : allergens) {
            if (!first) {
                cout << ", ";  // Add commas between allergens
            }
            cout << allergen;
            first = false;
        }
        cout << endl;
    }
}

// ===============================
//           GETTERS
// ===============================

// Returns the set of allergens associated with the food item.
std::set<std::string> Food::getAllergens() const {
    return allergens;
}

// Returns the temperature required for safe food storage.
double Food::getTemperatureRequired() const {
    return temperatureRequired;
}

// Returns the nutritional information associated with the food item.
std::string Food::getNutritionalInfo() const {
    return nutritionalInfo;
}

// ===============================
//           SETTERS
// ===============================

// Adds a new allergen to the allergen set (duplicates are ignored automatically).
void Food::addAllergen(std::string allergen) {
    allergens.insert(allergen);
}

// Removes an allergen from the set if it exists.
void Food::removeAllergen(std::string allergen) {
    allergens.erase(allergen);
}

// Clears all allergens associated with the food item.
void Food::clearAllergens() {
    allergens.clear();
}

// Sets the required storage temperature for this food.
void Food::setTemperatureRequired(double temperature) {
    temperatureRequired = temperature;
}

// Sets the nutritional information description for the food item.
void Food::setNutritionalInfo(std::string info) {
    nutritionalInfo = info;
}
