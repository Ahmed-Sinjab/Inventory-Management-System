/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description:
 *     Implements the Chemical class, which inherits from the Perishable class.
 *     The Chemical class represents perishable chemical products that may have specific
 *     hazard levels, disposal methods, ventilation requirements, and safety warnings.
 *     It extends the Perishable base class by adding attributes and methods related to
 *     chemical safety, environmental handling, and hazard management.
 *
 * Dependencies:
 *     - chemical.h   : Declares the Chemical class interface.
 *     - <iostream>   : Used for console output.
 *     - <string>     : Used for string-based attributes such as disposal type and warnings.
 *     - <set>        : Used to store unique safety warnings without duplicates.
 */

#include "chemical.h"
#include <iostream>
#include <string>
#include <set>
using namespace std;

// Constructor: initializes a Chemical object and calls the Perishable base class constructor.
// Sets ventilationRequired and disposalType to safe default values.
Chemical::Chemical(std::string id, std::string name, double price, std::string supplier)
    : Perishable(id, name, price, supplier), // Base class (Perishable) initialization
    ventilationRequired(false),            // Default: no ventilation required
    disposalType("Not Specified")          // Default disposal type
{}

// ===============================
//           GETTERS
// ===============================

// Returns the chemical disposal method (e.g., recycling, hazardous waste).
string Chemical::getDisposalType() const {
    return disposalType;
}

// Returns the hazard level (e.g., on a scale of 1–5).
int Chemical::getHazardLevel() const {
    return hazardLevel;
}

// Returns all registered safety warnings for this chemical.
std::set<std::string> Chemical::getSafetyWarnings() const {
    return safetyWarnings;
}

// Returns whether the chemical is child-safe.
bool Chemical::isChildSafe() const {
    return childSafe;
}

// Returns whether this chemical requires ventilation for safe handling.
bool Chemical::requiresVentilation() const {
    return ventilationRequired;
}

// ===============================
//           SETTERS
// ===============================

// Adds a safety warning to the list (duplicates automatically ignored by std::set).
void Chemical::addSafetyWarning(std::string warning) {
    safetyWarnings.insert(warning);
}

// Removes a specific warning if it exists in the list.
void Chemical::removeSafetyWarning(std::string warning) {
    safetyWarnings.erase(warning);
}

// Clears all stored safety warnings.
void Chemical::clearSafetyWarnings() {
    safetyWarnings.clear();
}

// Sets the disposal method (e.g., “Hazardous Waste,” “Recycling”).
void Chemical::setDisposalType(std::string type) {
    disposalType = type;
}

// Sets whether the chemical is considered safe for children.
void Chemical::setChildSafe(bool safe) {
    childSafe = safe;
}

// Sets whether proper ventilation is required for safe use or storage.
void Chemical::setVentilationRequired(bool required) {
    ventilationRequired = required;
}

// Sets the hazard level of the chemical.
void Chemical::setHazardLevel(int level) {
    hazardLevel = level;
}

// ===============================
//           DISPLAY
// ===============================

// Displays all information about the chemical, including inherited attributes
// and chemical-specific safety and handling details.
void Chemical::display() const {
    Perishable::display();  // Display base Perishable information first

    cout << "Disposal Type: " << disposalType << endl;
    cout << "Hazard Level: " << hazardLevel << endl;
    cout << "Child Safe: " << (childSafe ? "Yes" : "No") << endl;
    cout << "Ventilation Required: " << (ventilationRequired ? "Yes" : "No") << endl;

    cout << "Safety Warnings: ";
    if (safetyWarnings.empty()) {
        cout << "None";
    } else {
        for (const auto& warning : safetyWarnings) {
            cout << warning << " ";
        }
    }
    cout << endl;
}

// Checks whether a specific safety warning exists in the current set.
bool Chemical::hasSafetyWarning(std::string warning) const {
    return safetyWarnings.find(warning) != safetyWarnings.end();
}
