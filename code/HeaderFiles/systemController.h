/*
* Author(s): Ahmed Sinjab, Omar Hossain
 * Creation Date: December 17, 2024
 * Purpose: Central controller for the Inventory Management System.
 *          Coordinates between GUI, business logic, and data persistence.
 *          Provides authentication, authorization, validation, and automation.
 *
 * Dependencies:
 *  - inventory.h : Inventory management
 *  - userManager.h : User/Admin persistence
 *  - fileManager.h : Inventory persistence
 *  - user.h, admin.h : User objects
 */

#ifndef SYSTEMCONTROLLER_H
#define SYSTEMCONTROLLER_H

#include "inventory.h"
#include "userManager.h"
#include "fileManager.h"
#include "user.h"
#include "admin.h"
#include "product.h"
#include "supplier.h"
#include <vector>
#include <string>
#include <set>

class SystemController {
private:
    // Core components
    Inventory inventory;
    UserManager userManager;
    FileManager fileManager;

    // User management
    std::vector<User*> users;
    std::vector<Admin*> admins;
    User* currentUser;

    // Automation settings
    int operationsSinceLastSave;
    static const int AUTO_SAVE_THRESHOLD = 5;

    // Status messages
    std::string lastErrorMessage;
    std::string lastSuccessMessage;

    // Helper methods
    bool checkAdminPrivileges();
    void incrementOperationCounter();
    void setError(std::string message);
    void setSuccess(std::string message);

public:
    SystemController();
    ~SystemController();

    // ==========================================
    // SYSTEM INITIALIZATION
    // ==========================================
    bool initializeSystem();
    bool shutdownSystem();

    // ==========================================
    // AUTHENTICATION
    // ==========================================
    bool loginUser(std::string username, std::string password);
    bool loginAdmin(std::string username, std::string password);
    bool logout();
    bool isUserLoggedIn() const;
    bool isCurrentUserAdmin() const;
    std::string getCurrentUsername() const;

    // ==========================================
    // USER OPERATIONS
    // ==========================================
    std::set<Product*> viewInventory();
    Product* searchProduct(std::string productId);
    bool purchaseProduct(std::string productId, int quantity);
    bool useProduct(std::string productId, int quantity);
    bool changePassword(std::string oldPass, std::string newPass);

    // ==========================================
    // ADMIN OPERATIONS
    // ==========================================
    bool addProduct(Product* product, Supplier* supplier = nullptr);
    bool removeProduct(std::string productId);
    bool modifyProductPrice(std::string productId, double newPrice);
    bool addSupplier(Supplier* supplier);
    bool removeSupplier(Supplier* supplier);
    std::vector<User*> getAllUsers();
    std::vector<Admin*> getAllAdmins();
    bool createUser(std::string username, std::string password, std::string email);
    bool createAdmin(std::string username, std::string password, std::string email);
    bool deleteUser(std::string username);
    std::string generateInventoryReport();
    void checkLowStock(int threshold = 10);

    // ==========================================
    // SUPPLIER OPERATIONS
    // ==========================================
    std::set<Supplier*> getAllSuppliers();
    Supplier* getSupplierForProduct(std::string productId);
    std::set<Product*> getProductsFromSupplier(Supplier* supplier);

    // ==========================================
    // AUTOMATED OPERATIONS
    // ==========================================
    void autoSaveData();
    void autoCheckLowStock();
    void autoBackup();

    // ==========================================
    // STATUS MESSAGES
    // ==========================================
    std::string getLastErrorMessage() const;
    std::string getLastSuccessMessage() const;

    // ==========================================
    // INVENTORY ACCESS (for display purposes)
    // ==========================================
    Inventory* getInventory();
};

#endif // SYSTEMCONTROLLER_H
