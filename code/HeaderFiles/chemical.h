#ifndef CHEMICAL_H
#define CHEMICAL_H
#include "Perishable.h"
#include <set>
class Chemical : public Perishable
{
protected:
    std::set<std::string> safetyWarnings;//safety warning set
    std::string disposalType;
    bool childSafe;
    bool ventilationRequired;
    int hazardLevel;

public:
    // Constructor
    Chemical(std::string id, std::string name, double price,std::string Supplier);

    // Override display to add chemical-specific info
    void display() const override;



    // Getters
    std::set<std::string> getSafetyWarnings() const;
    std::string getDisposalType() const;
    bool isChildSafe() const;
    bool requiresVentilation() const;
    int getHazardLevel() const;

    // Setters
    void addSafetyWarning(std::string warning);
    void removeSafetyWarning(std::string warning);
    void clearSafetyWarnings();
    void setDisposalType(std::string type);
    void setChildSafe(bool safe);
    void setVentilationRequired(bool required);
    void setHazardLevel(int level);

    //helper method
    bool hasSafetyWarning(std::string warning) const;


};

#endif // CHEMICAL_H

