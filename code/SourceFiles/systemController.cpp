/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description: Implementation of the SystemController class.
 *              Provides centralized control for authentication, operations,
 *              validation, and automation in the inventory system.
 */

#include "systemController.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
using namespace std;

// ==========================================
// CONSTRUCTOR & DESTRUCTOR
// ==========================================

SystemController::SystemController()
    : currentUser(nullptr), operationsSinceLastSave(0) {
    cout << "[SYSTEM] SystemController created" << endl;
}

SystemController::~SystemController() {
    cout << "[SYSTEM] SystemController destroyed" << endl;
}

// ==========================================
// SYSTEM INITIALIZATION
// ==========================================

bool SystemController::initializeSystem() {
    cout << "\n========================================" << endl;
    cout << "  INVENTORY MANAGEMENT SYSTEM" << endl;
    cout << "  Initializing..." << endl;
    cout << "========================================\n" << endl;

    // Load inventory from file
    cout << "[SYSTEM] Loading inventory data..." << endl;
    if (fileManager.loadInventory(&inventory, "inventory_data.txt")) {
        cout << "[SYSTEM] ✓ Inventory loaded successfully" << endl;
    } else {
        cout << "[SYSTEM] ! No existing inventory found. Starting fresh." << endl;
    }

    // Load user accounts from file
    cout << "[SYSTEM] Loading user accounts..." << endl;
    if (userManager.loadAllAccounts(users, admins, "accounts.txt")) {
        cout << "[SYSTEM] ✓ User accounts loaded successfully" << endl;
    } else {
        cout << "[SYSTEM] ! No existing accounts found. Creating default admin." << endl;

        // Create default admin account if none exists
        Admin* defaultAdmin = new Admin("admin", "admin123", "admin@system.com");
        defaultAdmin->grantPermission("SUPER_ADMIN");
        admins.push_back(defaultAdmin);

        // Save the default admin
        userManager.saveAllAccounts(users, admins, "accounts.txt");
        cout << "[SYSTEM] ✓ Default admin created (username: admin, password: admin123)" << endl;
    }

    // Display system status
    cout << "\n[SYSTEM] Initialization complete!" << endl;
    cout << "  - Products loaded: " << inventory.getProductList().size() << endl;
    cout << "  - Users loaded: " << users.size() << endl;
    cout << "  - Admins loaded: " << admins.size() << endl;
    cout << "========================================\n" << endl;

    setSuccess("System initialized successfully");
    return true;
}

bool SystemController::shutdownSystem() {
    cout << "\n========================================" << endl;
    cout << "  SYSTEM SHUTDOWN" << endl;
    cout << "========================================\n" << endl;

    // Auto-save all data before shutdown
    cout << "[SYSTEM] Saving all data before shutdown..." << endl;
    autoSaveData();

    // Logout current user if any
    if (currentUser != nullptr) {
        cout << "[SYSTEM] Logging out user: " << currentUser->getUserName() << endl;
        currentUser->logout();
        currentUser = nullptr;
    }

    // Cleanup users and admins
    cout << "[SYSTEM] Cleaning up resources..." << endl;
    for (User* u : users) delete u;
    for (Admin* a : admins) delete a;
    users.clear();
    admins.clear();

    cout << "[SYSTEM] ✓ Shutdown complete. Goodbye!" << endl;
    cout << "========================================\n" << endl;

    return true;
}

// ==========================================
// AUTHENTICATION
// ==========================================

bool SystemController::loginUser(string username, string password) {
    if (currentUser != nullptr) {
        setError("A user is already logged in. Please logout first.");
        return false;
    }

    if (username.empty() || password.empty()) {
        setError("Username and password cannot be empty.");
        return false;
    }

    // Check regular users
    for (User* u : users) {
        if (u->getUserName() == username) {
            if (u->login(username, password)) {
                currentUser = u;
                setSuccess("Welcome, " + username + "!");
                cout << "[AUTH] User logged in: " << username << endl;
                return true;
            } else {
                setError("Invalid password for user: " + username);
                return false;
            }
        }
    }

    setError("User not found: " + username);
    return false;
}

bool SystemController::loginAdmin(string username, string password) {
    if (currentUser != nullptr) {
        setError("A user is already logged in. Please logout first.");
        return false;
    }

    if (username.empty() || password.empty()) {
        setError("Username and password cannot be empty.");
        return false;
    }

    // Check admins
    for (Admin* a : admins) {
        if (a->getUserName() == username) {
            if (a->login(username, password)) {
                currentUser = a;
                setSuccess("Welcome, Administrator " + username + "!");
                cout << "[AUTH] Admin logged in: " << username << endl;
                return true;
            } else {
                setError("Invalid password for admin: " + username);
                return false;
            }
        }
    }

    setError("Admin not found: " + username);
    return false;
}

bool SystemController::logout() {
    if (currentUser == nullptr) {
        setError("No user is currently logged in.");
        return false;
    }

    string username = currentUser->getUserName();
    currentUser->logout();
    currentUser = nullptr;

    setSuccess("Goodbye, " + username + "!");
    cout << "[AUTH] User logged out: " << username << endl;
    return true;
}

bool SystemController::isUserLoggedIn() const {
    return currentUser != nullptr;
}

bool SystemController::isCurrentUserAdmin() const {
    if (currentUser == nullptr) return false;
    return dynamic_cast<Admin*>(currentUser) != nullptr;
}

string SystemController::getCurrentUsername() const {
    if (currentUser == nullptr) return "";
    return currentUser->getUserName();
}

// ==========================================
// USER OPERATIONS
// ==========================================

set<Product*> SystemController::viewInventory() {
    if (!isUserLoggedIn()) {
        setError("You must be logged in to view inventory.");
        return set<Product*>();
    }

    setSuccess("Inventory retrieved successfully");
    return inventory.getProductList();
}

Product* SystemController::searchProduct(string productId) {
    if (!isUserLoggedIn()) {
        setError("You must be logged in to search products.");
        return nullptr;
    }

    if (productId.empty()) {
        setError("Product ID cannot be empty.");
        return nullptr;
    }

    Product* p = inventory.searchProduct(productId);
    if (p == nullptr) {
        setError("Product not found: " + productId);
    } else {
        setSuccess("Product found: " + p->getName());
    }

    return p;
}

bool SystemController::purchaseProduct(string productId, int quantity) {
    if (!isUserLoggedIn()) {
        setError("You must be logged in to purchase products.");
        return false;
    }

    if (productId.empty()) {
        setError("Product ID cannot be empty.");
        return false;
    }

    if (quantity <= 0) {
        setError("Quantity must be positive! (Received: " + to_string(quantity) + ")");
        return false;
    }

    Product* p = inventory.searchProduct(productId);
    if (p == nullptr) {
        setError("Product not found: " + productId);
        return false;
    }

    // Check if purchase would exceed max capacity
    if (p->getQuantity() + quantity > p->getMaxQuantity()) {
        setError("Purchase would exceed maximum capacity!\n" +
                 string("Current: ") + to_string(p->getQuantity()) + "\n" +
                 string("Requesting: ") + to_string(quantity) + "\n" +
                 string("Maximum: ") + to_string(p->getMaxQuantity()) + "\n" +
                 string("Available space: ") + to_string(p->getMaxQuantity() - p->getQuantity()));
        return false;
    }

    if (inventory.buyProduct(productId, quantity)) {
        setSuccess("Successfully purchased " + to_string(quantity) +
                   " units of " + p->getName() + "\nNew quantity: " +
                   to_string(p->getQuantity()));
        incrementOperationCounter();
        cout << "[OPERATION] " << currentUser->getUserName() << " purchased "
             << quantity << " of " << productId << endl;
        return true;
    } else {
        setError("Purchase failed for product: " + productId);
        return false;
    }
}

bool SystemController::useProduct(string productId, int quantity) {
    if (!isUserLoggedIn()) {
        setError("You must be logged in to use products.");
        return false;
    }

    if (productId.empty()) {
        setError("Product ID cannot be empty.");
        return false;
    }

    if (quantity <= 0) {
        setError("Quantity must be positive! (Received: " + to_string(quantity) + ")");
        return false;
    }

    Product* p = inventory.searchProduct(productId);
    if (p == nullptr) {
        setError("Product not found: " + productId);
        return false;
    }

    // Check if enough stock available
    if (p->getQuantity() < quantity) {
        setError("Not enough stock available!\n" +
                 string("Available: ") + to_string(p->getQuantity()) + "\n" +
                 string("Requested: ") + to_string(quantity));
        return false;
    }

    if (inventory.sellProduct(productId, quantity)) {
        setSuccess("Successfully used " + to_string(quantity) +
                   " units of " + p->getName() + "\nRemaining quantity: " +
                   to_string(p->getQuantity()));
        incrementOperationCounter();

        // Auto-check for low stock after using product
        if (p->getQuantity() < p->getMinQuantity()) {
            cout << "[WARNING] Low stock alert for " << p->getName()
            << " (" << p->getQuantity() << "/" << p->getMinQuantity() << ")" << endl;
        }

        cout << "[OPERATION] " << currentUser->getUserName() << " used "
             << quantity << " of " << productId << endl;
        return true;
    } else {
        setError("Failed to use product: " + productId);
        return false;
    }
}

bool SystemController::changePassword(string oldPass, string newPass) {
    if (!isUserLoggedIn()) {
        setError("You must be logged in to change password.");
        return false;
    }

    if (newPass.empty() || newPass.length() < 6) {
        setError("New password must be at least 6 characters long.");
        return false;
    }

    if (currentUser->changePassword(oldPass, newPass)) {
        setSuccess("Password changed successfully!");
        incrementOperationCounter();
        return true;
    } else {
        setError("Failed to change password. Old password may be incorrect.");
        return false;
    }
}

// ==========================================
// ADMIN OPERATIONS
// ==========================================

bool SystemController::addProduct(Product* product, Supplier* supplier) {
    if (!checkAdminPrivileges()) return false;

    if (product == nullptr) {
        setError("Cannot add null product!");
        return false;
    }

    // Check if product ID already exists
    if (inventory.searchProduct(product->getId()) != nullptr) {
        setError("Product with ID " + product->getId() + " already exists!");
        return false;
    }

    if (supplier != nullptr) {
        inventory.addProductWithSupplier(product, supplier);
    } else {
        inventory.addProduct(product);
    }

    setSuccess("Product added successfully: " + product->getName());
    incrementOperationCounter();
    cout << "[ADMIN] " << currentUser->getUserName() << " added product: "
         << product->getId() << endl;
    return true;
}

bool SystemController::removeProduct(string productId) {
    if (!checkAdminPrivileges()) return false;

    if (productId.empty()) {
        setError("Product ID cannot be empty.");
        return false;
    }

    Product* p = inventory.searchProduct(productId);
    if (p == nullptr) {
        setError("Product not found: " + productId);
        return false;
    }

    string productName = p->getName();

    if (inventory.removeProduct(productId)) {
        setSuccess("Product removed successfully: " + productName);
        incrementOperationCounter();
        cout << "[ADMIN] " << currentUser->getUserName() << " removed product: "
             << productId << endl;
        return true;
    } else {
        setError("Failed to remove product: " + productId);
        return false;
    }
}

bool SystemController::modifyProductPrice(string productId, double newPrice) {
    if (!checkAdminPrivileges()) return false;

    if (productId.empty()) {
        setError("Product ID cannot be empty.");
        return false;
    }

    Product* p = inventory.searchProduct(productId);
    if (p == nullptr) {
        setError("Product not found: " + productId);
        return false;
    }

    if (newPrice < 0) {
        setError("Price cannot be negative!");
        return false;
    }

    double oldPrice = p->getPrice();
    p->setPrice(newPrice);

    stringstream ss;
    ss << fixed << setprecision(2);
    ss << "Price updated for " << p->getName() << "\n"
       << "Old price: $" << oldPrice << "\n"
       << "New price: $" << newPrice;

    setSuccess(ss.str());
    incrementOperationCounter();
    cout << "[ADMIN] " << currentUser->getUserName() << " changed price of "
         << productId << " from $" << oldPrice << " to $" << newPrice << endl;
    return true;
}

bool SystemController::addSupplier(Supplier* supplier) {
    if (!checkAdminPrivileges()) return false;

    if (supplier == nullptr) {
        setError("Cannot add null supplier!");
        return false;
    }

    inventory.addSupplier(supplier);
    setSuccess("Supplier added successfully");
    incrementOperationCounter();
    return true;
}

bool SystemController::removeSupplier(Supplier* supplier) {
    if (!checkAdminPrivileges()) return false;

    if (supplier == nullptr) {
        setError("Cannot remove null supplier!");
        return false;
    }

    if (inventory.removeSupplier(supplier)) {
        setSuccess("Supplier removed successfully");
        incrementOperationCounter();
        return true;
    } else {
        setError("Failed to remove supplier");
        return false;
    }
}

std::vector<User*> SystemController::getAllUsers() {
    return users;
}

std::vector<Admin*> SystemController::getAllAdmins() {
    return admins;
}

bool SystemController::createUser(string username, string password, string email) {
    if (!checkAdminPrivileges()) return false;

    if (username.empty() || password.empty()) {
        setError("Username and password cannot be empty.");
        return false;
    }

    // Check if username already exists
    for (User* u : users) {
        if (u->getUserName() == username) {
            setError("Username already exists: " + username);
            return false;
        }
    }

    for (Admin* a : admins) {
        if (a->getUserName() == username) {
            setError("Username already exists: " + username);
            return false;
        }
    }

    User* newUser = new User(username, password, email);
    users.push_back(newUser);

    setSuccess("User created successfully: " + username);
    incrementOperationCounter();
    cout << "[ADMIN] " << currentUser->getUserName() << " created user: "
         << username << endl;
    return true;
}

bool SystemController::createAdmin(string username, string password, string email) {
    if (!checkAdminPrivileges()) return false;

    if (username.empty() || password.empty()) {
        setError("Username and password cannot be empty.");
        return false;
    }

    // Check if username already exists
    for (User* u : users) {
        if (u->getUserName() == username) {
            setError("Username already exists: " + username);
            return false;
        }
    }

    for (Admin* a : admins) {
        if (a->getUserName() == username) {
            setError("Username already exists: " + username);
            return false;
        }
    }

    Admin* newAdmin = new Admin(username, password, email);
    admins.push_back(newAdmin);

    setSuccess("Admin created successfully: " + username);
    incrementOperationCounter();
    cout << "[ADMIN] " << currentUser->getUserName() << " created admin: "
         << username << endl;
    return true;
}

bool SystemController::deleteUser(string username) {
    if (!checkAdminPrivileges()) return false;

    // Prevent deleting yourself
    if (currentUser != nullptr && currentUser->getUserName() == username) {
        setError("Cannot delete the currently logged-in user!");
        return false;
    }

    // Try to find and delete user
    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getUserName() == username) {
            delete *it;
            users.erase(it);
            setSuccess("User deleted successfully: " + username);
            incrementOperationCounter();
            return true;
        }
    }

    // Try to find and delete admin
    for (auto it = admins.begin(); it != admins.end(); ++it) {
        if ((*it)->getUserName() == username) {
            delete *it;
            admins.erase(it);
            setSuccess("Admin deleted successfully: " + username);
            incrementOperationCounter();
            return true;
        }
    }

    setError("User not found: " + username);
    return false;
}

string SystemController::generateInventoryReport() {
    if (!checkAdminPrivileges()) return "";

    stringstream report;
    report << "\n========================================\n";
    report << "      INVENTORY REPORT\n";
    report << "========================================\n\n";

    set<Product*> products = inventory.getProductList();

    report << "Total Products: " << products.size() << "\n\n";

    double totalValue = 0.0;
    int lowStockCount = 0;

    for (Product* p : products) {
        totalValue += p->getPrice() * p->getQuantity();

        if (p->getQuantity() < p->getMinQuantity()) {
            lowStockCount++;
        }
    }

    report << fixed << setprecision(2);
    report << "Total Inventory Value: $" << totalValue << "\n";
    report << "Low Stock Items: " << lowStockCount << "\n";
    report << "\n========================================\n";

    setSuccess("Report generated successfully");
    return report.str();
}

void SystemController::checkLowStock(int threshold) {
    if (!isUserLoggedIn()) {
        setError("You must be logged in to check stock.");
        return;
    }

    inventory.checkLowStock(threshold);
    setSuccess("Low stock check completed");
}

// ==========================================
// SUPPLIER OPERATIONS
// ==========================================

set<Supplier*> SystemController::getAllSuppliers() {
    // Note: You'll need to add this method to your Inventory class
    // For now, return empty set
    return set<Supplier*>();
}

Supplier* SystemController::getSupplierForProduct(string productId) {
    return inventory.getSupplierForProduct(productId);
}

set<Product*> SystemController::getProductsFromSupplier(Supplier* supplier) {
    return inventory.getProductsFromSupplier(supplier);
}

// ==========================================
// AUTOMATED OPERATIONS
// ==========================================

void SystemController::autoSaveData() {
    cout << "\n[AUTO-SAVE] Saving all data..." << endl;

    fileManager.saveInventory(&inventory, "inventory_data.txt");
    userManager.saveAllAccounts(users, admins, "accounts.txt");

    cout << "[AUTO-SAVE] Complete!" << endl;
}

void SystemController::autoCheckLowStock() {
    set<Product*> products = inventory.getProductList();

    int lowStockCount = 0;
    for (Product* p : products) {
        if (p->getQuantity() < p->getMinQuantity()) {
            lowStockCount++;
            cout << "[LOW STOCK] " << p->getName()
                 << " - Current: " << p->getQuantity()
                 << ", Min: " << p->getMinQuantity() << endl;
        }
    }

    if (lowStockCount > 0) {
        cout << "[AUTO-CHECK] Found " << lowStockCount << " products with low stock" << endl;
    }
}

void SystemController::autoBackup() {
    if (!isUserLoggedIn()) return;

    time_t now = time(0);
    tm* ltm = localtime(&now);

    stringstream filename;
    filename << "backup_inventory_"
             << (1900 + ltm->tm_year) << "-"
             << (1 + ltm->tm_mon) << "-"
             << ltm->tm_mday << "_"
             << ltm->tm_hour << "-"
             << ltm->tm_min << ".txt";

    cout << "[AUTO-BACKUP] Creating backup: " << filename.str() << endl;
    fileManager.saveInventory(&inventory, filename.str());
    cout << "[AUTO-BACKUP] Complete!" << endl;
}


// HELPER METHODS
// ==========================================

bool SystemController::checkAdminPrivileges() {
    if (!isUserLoggedIn()) {
        setError("You must be logged in to perform this operation.");
        return false;
    }

    if (!isCurrentUserAdmin()) {
        setError("This operation requires administrator privileges.");
        return false;
    }

    return true;
}

void SystemController::incrementOperationCounter() {
    operationsSinceLastSave++;

    if (operationsSinceLastSave >= AUTO_SAVE_THRESHOLD) {
        autoSaveData();
        operationsSinceLastSave = 0;
    }
}

void SystemController::setError(string message) {
    lastErrorMessage = message;
    lastSuccessMessage = "";
    cerr << "[ERROR] " << message << endl;
}
// Add this at the end of the file
Inventory* SystemController::getInventory() {
    return &inventory;
}
void SystemController::setSuccess(string message) {
    lastSuccessMessage = message;
    lastErrorMessage = "";
    cout << "[SUCCESS] " << message << endl;
}
