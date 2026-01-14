#ifndef FOOD_H
#define FOOD_H

#include "Perishable.h"
#include <set>

class Food : public Perishable {

protected:
    std::set<std::string> allergens;//safety warning set
    double temperatureRequired;
    std::string nutritionalInfo;

public:
    Food(std::string id, std::string name, double price,std::string Supplier);

    // Override display to add chemical-specific info
    void display() const override;


    // Getters
    std::set<std::string> getAllergens() const;
    double getTemperatureRequired() const;
    std::string getNutritionalInfo() const;

    // Setters
    void addAllergen(std::string allergen);
    void removeAllergen(std::string allergen);
    void clearAllergens();
    void setTemperatureRequired(double temperature);
    void setNutritionalInfo(std::string info);

};

#endif // FOOD_H
