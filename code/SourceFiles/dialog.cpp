#include "dialog.h"
#include "ui_dialog.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QCloseEvent>
#include <QTimer>
#include <QDate>
#include <iostream>

// Constructor with logged-in user and SystemController
Dialog::Dialog(User* user, SystemController* controller, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , currentUser(user)
    , systemController(controller)
{
    ui->setupUi(this);

    // Get pointer to SystemController's inventory
    inventory = systemController->getInventory();

    // Create FileManager for manual save/load buttons
    fileManager = new FileManager();

    // Initialize low stock check timer
    lowStockCheckTimer = new QTimer(this);
    connect(lowStockCheckTimer, &QTimer::timeout, this, &Dialog::checkAndNotifyLowStock);
    lowStockCheckTimer->start(30000); // Check every 30 seconds

    // Set default values
    ui->spinBoxQuantity->setMinimum(0);
    ui->spinBoxQuantity->setValue(1);
    ui->spinBoxBuySellQuantity->setMinimum(1);
    ui->spinBoxBuySellQuantity->setValue(1);

    // Set default expiration date to 30 days from now
    ui->dateEditExpiration->setDate(QDate::currentDate().addDays(30));

    updateFieldVisibility();
    connectSignals();

    if (currentUser) {
        setWindowTitle(QString("Inventory Management System - Logged in as: %1")
                           .arg(QString::fromStdString(currentUser->getUserName())));
    }
    refreshProductList();
}

Dialog::~Dialog()
{
    delete ui;
    delete fileManager;
    delete lowStockCheckTimer;
    // Don't delete inventory - SystemController owns it!
}

void Dialog::connectSignals()
{
    connect(ui->buttonAddProduct, &QPushButton::clicked,
            this, &Dialog::onAddProductClicked);

    connect(ui->buttonDeleteProduct, &QPushButton::clicked,
            this, &Dialog::onDeleteProductClicked);

    connect(ui->comboBoxProductType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Dialog::onProductTypeChanged);

    // Inventory operations
    connect(ui->buttonViewAll, &QPushButton::clicked,
            this, &Dialog::onViewAllClicked);

    connect(ui->buttonBuyProduct, &QPushButton::clicked,
            this, &Dialog::onBuyProductClicked);

    connect(ui->buttonSellProduct, &QPushButton::clicked,
            this, &Dialog::onSellProductClicked);

    connect(ui->buttonCheckLowStock, &QPushButton::clicked,
            this, &Dialog::onCheckLowStockClicked);

    connect(ui->buttonViewSuppliers, &QPushButton::clicked,
            this, &Dialog::onViewSuppliersClicked);

    connect(ui->buttonGenerateReport, &QPushButton::clicked,
            this, &Dialog::onGenerateReportClicked);

    // User management buttons
    connect(ui->buttonLogout, &QPushButton::clicked,
            this, &Dialog::onLogoutClicked);

    connect(ui->buttonChangePassword, &QPushButton::clicked,
            this, &Dialog::onChangePasswordClicked);

    // File I/O buttons
    connect(ui->buttonSaveInventory, &QPushButton::clicked,
            this, &Dialog::onSaveInventoryClicked);

    connect(ui->buttonLoadInventory, &QPushButton::clicked,
            this, &Dialog::onLoadInventoryClicked);
}

void Dialog::onProductTypeChanged(int index)
{
    updateFieldVisibility();
}

void Dialog::closeEvent(QCloseEvent *event)
{
    // Ask for confirmation before closing
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Exit",
        "Are you sure you want to exit?\nAll data will be saved.",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // Save data before closing
        std::cout << "[DIALOG] Window closing via X button - saving data..." << std::endl;
        systemController->autoSaveData();

        // Logout current user
        if (currentUser && currentUser->getIsLoggedIn()) {
            currentUser->logout();
        }

        // Accept the close event
        event->accept();
    } else {
        // Ignore the close event (don't close)
        event->ignore();
    }
}

void Dialog::updateFieldVisibility()
{
    QString productType = ui->comboBoxProductType->currentText();

    // Show/hide fields based on product type
    bool showPerishableFields = (productType == "Perishable" ||
                                 productType == "Food" ||
                                 productType == "Chemical");

    ui->dateEditExpiration->setVisible(showPerishableFields);
    ui->lineEditStorageCondition->setVisible(showPerishableFields);
    ui->labelExpirationDate->setVisible(showPerishableFields);
    ui->labelStorageCondition->setVisible(showPerishableFields);

    // Set default expiration date to 30 days from now when showing perishable fields
    if (showPerishableFields) {
        ui->dateEditExpiration->setDate(QDate::currentDate().addDays(30));
    }
}

void Dialog::onAddProductClicked()
{
    // Get values from UI
    QString productType = ui->comboBoxProductType->currentText();
    QString name = ui->lineEditProductName->text();
    QString priceText = ui->lineEditPrice->text();
    int quantity = ui->spinBoxQuantity->value();

    // Validate basic input
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a product name!");
        return;
    }

    bool ok;
    double price = priceText.toDouble(&ok);
    if (!ok || price < 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid price!");
        return;
    }

    // Get supplier from UI
    QString supplierText = ui->lineEditSupplier->text();
    std::string supplierName = supplierText.isEmpty() ? "Default Supplier" : supplierText.toStdString();

    // Create a Supplier object
    Supplier* supplier = new Supplier();
    supplier->setQualityRating(5.0);
    supplier->setDeliveryReliability("Reliable");

    // Create product based on type
    // Generate base ID (factory adds prefix)
    std::string id = std::to_string(inventory->getProductList().size() + 1);

    // Create product using Factory Method
    Product* newProduct = productFactory::createProduct(
        getProductTypeFromUI(),      // enum from UI
        id,
        name.toStdString(),
        price,
        supplierName
        );

    if (!newProduct) {
        QMessageBox::warning(this, "Error", "Failed to create product!");
        return;
    }

    // Set quantity and thresholds
    newProduct->setTotalQuantity(1000);
    newProduct->setMinimumQuantity(10);
    newProduct->setQuantity(quantity);

    // Set expiration date for perishable products
    if (productType == "Perishable" || productType == "Food" || productType == "Chemical") {
        QDate expirationDate = ui->dateEditExpiration->date();
        std::string dateStr = expirationDate.toString("yyyy-MM-dd").toStdString();

        // Set expiration date based on product type
        if (Perishable* perishable = dynamic_cast<Perishable*>(newProduct)) {
            perishable->setExpirationDate(dateStr);

            // Set storage condition if provided
            QString storageCondition = ui->lineEditStorageCondition->text();
            if (!storageCondition.isEmpty()) {
                perishable->setStorageCondition(storageCondition.toStdString());
            }
        }
    }

    // Add product with supplier to Inventory
    inventory->addProductWithSupplier(newProduct, supplier);

    // Clear input fields
    ui->lineEditProductName->clear();
    ui->lineEditPrice->clear();
    ui->spinBoxQuantity->setValue(1);
    ui->dateEditExpiration->setDate(QDate::currentDate().addDays(30)); // Reset to 30 days from now
    ui->lineEditStorageCondition->clear();
    ui->lineEditSupplier->clear();

    refreshProductList();
    triggerAutoSave(); // Auto-save after adding

    QMessageBox::information(this, "Success",
                             QString("Product '%1' added successfully!").arg(name));

    // Check for low stock after adding
    checkAndNotifyLowStock();
}

void Dialog::onDeleteProductClicked()
{
    int currentRow = ui->listWidgetProducts->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a product to delete!");
        return;
    }

    // Get the product at the selected row
    std::set<Product*> products = inventory->getProductList();

    if (currentRow >= static_cast<int>(products.size())) {
        QMessageBox::warning(this, "Error", "Invalid selection!");
        return;
    }

    auto it = products.begin();
    std::advance(it, currentRow);
    Product* productToDelete = *it;

    // Store product info before deletion (since the object will be deleted)
    std::string productName = productToDelete->getName();
    std::string productId = productToDelete->getId();

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                  QString("Are you sure you want to delete:\n\n%1 (ID: %2)?")
                                      .arg(QString::fromStdString(productName))
                                      .arg(QString::fromStdString(productId)),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Use your Inventory removeProduct method
        bool success = inventory->removeProduct(productId);

        if (success) {
            QMessageBox::information(this, "Success",
                                     QString("Product '%1' deleted successfully!")
                                         .arg(QString::fromStdString(productName)));

            // Refresh the product list
            refreshProductList();
            triggerAutoSave(); // Auto-save after deleting
            checkAndNotifyLowStock(); // Check low stock after deleting
        } else {
            QMessageBox::warning(this, "Error",
                                 "Failed to delete product. Please check console for details.");
        }
    }
}

void Dialog::refreshProductList()
{
    ui->listWidgetProducts->clear();

    std::set<Product*> products = inventory->getProductList();

    for (Product* product : products) {
        QString displayText = QString::fromStdString(
            product->getId() + " - " +
            product->getName() +
            " | $" + std::to_string(product->getPrice()) +
            " | Qty: " + std::to_string(product->getQuantity())
            );

        QListWidgetItem* item = new QListWidgetItem(displayText);

        // Highlight low stock items in RED
        if (product->getQuantity() < product->getMinQuantity()) {
            item->setForeground(Qt::red);
            item->setFont(QFont("Arial", 9, QFont::Bold));
        }

        ui->listWidgetProducts->addItem(item);
    }
}

productType Dialog::getProductTypeFromUI()
{
    QString type = ui->comboBoxProductType->currentText();

    if (type == "Raw Materials")   return productType::RAW_MATERIAL;
    if (type == "NonPerishable") return productType::NON_PERISHABLE;
    if (type == "Perishable")    return productType::PERISHABLE;
    if (type == "Food")          return productType::FOOD;
    if (type == "Chemical")      return productType::CHEMICAL;

    return productType::NON_PERISHABLE; // safe default
}

void Dialog::onViewAllClicked()
{
    std::set<Product*> products = inventory->getProductList();

    if (products.empty()) {
        QMessageBox::information(this, "Inventory", "No products in inventory.");
        return;
    }

    QString details = "=== ALL PRODUCTS ===\n\n";

    for (Product* p : products) {
        details += QString("ID: %1\nName: %2\nPrice: $%3\nQuantity: %4\n")
        .arg(QString::fromStdString(p->getId()))
            .arg(QString::fromStdString(p->getName()))
            .arg(p->getPrice())
            .arg(p->getQuantity());

        Supplier* supplier = inventory->getSupplierForProduct(p->getId());
        if (supplier) {
            details += QString("Supplier Rating: %1/5.0\n")
            .arg(supplier->getQualityRating());
        }

        details += "\n---\n\n";
    }

    QMessageBox::information(this, "All Products", details);
}

void Dialog::onBuyProductClicked()
{
    QString productId = ui->lineEditProductId->text();
    int quantity = ui->spinBoxBuySellQuantity->value();

    if (productId.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a Product ID!");
        return;
    }

    bool success = inventory->buyProduct(productId.toStdString(), quantity);

    if (success) {
        QMessageBox::information(this, "Success",
                                 QString("Purchased %1 units of %2!")
                                     .arg(quantity).arg(productId));
        refreshProductList();
        triggerAutoSave(); // Auto-save after buying
    } else {
        QMessageBox::warning(this, "Failed", "Could not complete purchase.");
    }
}

void Dialog::onSellProductClicked()
{
    QString productId = ui->lineEditProductId->text();
    int quantity = ui->spinBoxBuySellQuantity->value();

    if (productId.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a Product ID!");
        return;
    }

    bool success = inventory->sellProduct(productId.toStdString(), quantity);

    if (success) {
        QMessageBox::information(this, "Success",
                                 QString("Sold %1 units of %2!")
                                     .arg(quantity).arg(productId));
        refreshProductList();
        triggerAutoSave(); // Auto-save after selling
        checkAndNotifyLowStock(); // Check immediately after selling
    } else {
        QMessageBox::warning(this, "Failed", "Could not complete sale.");
    }
}

void Dialog::onCheckLowStockClicked()
{
    std::set<Product*> products = inventory->getProductList();
    QString lowStockMessage = "=== LOW STOCK ALERT ===\n\n";
    bool foundLowStock = false;

    for (Product* p : products) {
        if (p->getQuantity() < p->getMinQuantity()) {
            foundLowStock = true;
            lowStockMessage += QString("️ %1 (ID: %2)\nCurrent: %3 | Min: %4\n\n")
                                   .arg(QString::fromStdString(p->getName()))
                                   .arg(QString::fromStdString(p->getId()))
                                   .arg(p->getQuantity())
                                   .arg(p->getMinQuantity());
        }
    }

    if (!foundLowStock) {
        lowStockMessage += "All products adequately stocked! ✓";
    }

    QMessageBox::information(this, "Low Stock Check", lowStockMessage);
}

void Dialog::onViewSuppliersClicked()
{
    QString suppliersInfo = "=== ALL SUPPLIERS ===\n\n";

    std::set<Product*> products = inventory->getProductList();
    std::set<Supplier*> uniqueSuppliers;

    for (Product* p : products) {
        Supplier* supplier = inventory->getSupplierForProduct(p->getId());
        if (supplier) {
            uniqueSuppliers.insert(supplier);
        }
    }

    if (uniqueSuppliers.empty()) {
        QMessageBox::information(this, "Suppliers", "No suppliers registered.");
        return;
    }

    int count = 1;
    for (Supplier* s : uniqueSuppliers) {
        suppliersInfo += QString("Supplier #%1\nRating: %2/5.0\nReliability: %3\n\n")
        .arg(count++)
            .arg(s->getQualityRating())
            .arg(QString::fromStdString(s->getDeliveryReliability()));

        std::set<Product*> supplierProducts = inventory->getProductsFromSupplier(s);
        suppliersInfo += "Products:\n";
        for (Product* p : supplierProducts) {
            suppliersInfo += QString("  • %1 (Qty: %2)\n")
                                 .arg(QString::fromStdString(p->getName()))
                                 .arg(p->getQuantity());
        }
        suppliersInfo += "\n";
    }

    QMessageBox::information(this, "Suppliers", suppliersInfo);
}

void Dialog::onGenerateReportClicked()
{
    std::set<Product*> products = inventory->getProductList();

    QString report = "=== INVENTORY REPORT ===\n\n";
    report += QString("Total Products: %1\n\n").arg(products.size());

    int rawMat = 0, nonPer = 0, per = 0, food = 0, chem = 0;
    double totalValue = 0.0;

    for (Product* p : products) {
        totalValue += p->getPrice() * p->getQuantity();

        std::string id = p->getId();
        if (id.find("RM") == 0) rawMat++;
        else if (id.find("NP") == 0) nonPer++;
        else if (id.find("P") == 0) per++;
        else if (id.find("F") == 0) food++;
        else if (id.find("C") == 0) chem++;
    }

    report += "By Type:\n";
    report += QString("Raw Materials: %1\n").arg(rawMat);
    report += QString("Non-Perishables: %1\n").arg(nonPer);
    report += QString("Perishables: %1\n").arg(per);
    report += QString("Food: %1\n").arg(food);
    report += QString("Chemicals: %1\n\n").arg(chem);

    report += QString("Total Value: $%1\n\n").arg(totalValue, 0, 'f', 2);

    report += "Low Stock Items:\n";
    bool hasLowStock = false;
    for (Product* p : products) {
        if (p->getQuantity() < p->getMinQuantity()) {
            report += QString("%1 (Qty: %2)\n")
                          .arg(QString::fromStdString(p->getName()))
                          .arg(p->getQuantity());
            hasLowStock = true;
        }
    }
    if (!hasLowStock) {
        report += "None ✓\n";
    }

    QMessageBox::information(this, "Report", report);
}

void Dialog::showProductDetails(Product* product)
{
    if (!product) return;

    QString details = QString("ID: %1\nName: %2\nPrice: $%3\nQuantity: %4")
                          .arg(QString::fromStdString(product->getId()))
                          .arg(QString::fromStdString(product->getName()))
                          .arg(product->getPrice())
                          .arg(product->getQuantity());

    QMessageBox::information(this, "Product Details", details);
}

void Dialog::onLogoutClicked()
{
    if (!currentUser) {
        QMessageBox::warning(this, "Error", "No user is currently logged in!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Logout",
                                  QString("Are you sure you want to logout, %1?")
                                      .arg(QString::fromStdString(currentUser->getUserName())),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        systemController->autoSaveData();

        if (currentUser->logout()) {
            QMessageBox::information(this, "Logged Out",
                                     "You have been logged out successfully!");

            // Use reject() to signal logout
            this->reject();  // returns to main.cpp loop
        }
    }
}

void Dialog::onChangePasswordClicked()
{
    if (!currentUser) {
        QMessageBox::warning(this, "Error", "No user is currently logged in!");
        return;
    }

    // Check if user is logged in
    if (!currentUser->getIsLoggedIn()) {
        QMessageBox::warning(this, "Error", "You must be logged in to change password!");
        return;
    }

    // Create input dialog for old password
    bool ok;
    QString oldPassword = QInputDialog::getText(this,
                                                "Change Password",
                                                "Enter your current password:",
                                                QLineEdit::Password,
                                                "",
                                                &ok);

    if (!ok || oldPassword.isEmpty()) {
        return;  // User cancelled
    }

    // Create input dialog for new password
    QString newPassword = QInputDialog::getText(this,
                                                "Change Password",
                                                "Enter your new password:",
                                                QLineEdit::Password,
                                                "",
                                                &ok);

    if (!ok || newPassword.isEmpty()) {
        return;  // User cancelled
    }

    // Confirm new password
    QString confirmPassword = QInputDialog::getText(this,
                                                    "Change Password",
                                                    "Confirm your new password:",
                                                    QLineEdit::Password,
                                                    "",
                                                    &ok);

    if (!ok || confirmPassword.isEmpty()) {
        return;  // User cancelled
    }

    // Check if new passwords match
    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "Error", "New passwords do not match!");
        return;
    }

    // Check password strength (optional)
    if (newPassword.length() < 6) {
        QMessageBox::warning(this, "Weak Password",
                             "Password must be at least 6 characters long!");
        return;
    }

    // Use your User class changePassword method
    bool success = currentUser->changePassword(oldPassword.toStdString(),
                                               newPassword.toStdString());

    if (success) {
        QMessageBox::information(this, "Success",
                                 "Password changed successfully!");
    } else {
        QMessageBox::warning(this, "Failed",
                             "Failed to change password. Please check your current password.");
    }
}

void Dialog::onSaveInventoryClicked()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Save Inventory",
        "inventory.txt",
        "Text Files (*.txt);;All Files (*)"
        );

    if (filename.isEmpty()) {
        return;  // User cancelled
    }

    bool success = fileManager->saveInventory(inventory, filename.toStdString());

    if (success) {
        QMessageBox::information(this, "Success",
                                 QString("Inventory saved successfully to:\n%1")
                                     .arg(filename));
    } else {
        QMessageBox::warning(this, "Error",
                             "Failed to save inventory. Check console for details.");
    }
}

void Dialog::onLoadInventoryClicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Load Inventory",
        "",
        "Text Files (*.txt);;All Files (*)"
        );

    if (filename.isEmpty()) {
        return;  // User cancelled
    }

    bool success = fileManager->loadInventory(inventory, filename.toStdString());

    if (success) {
        refreshProductList();
        QMessageBox::information(this, "Success",
                                 QString("Inventory loaded successfully from:\n%1")
                                     .arg(filename));
    } else {
        QMessageBox::warning(this, "Warning",
                             "Could not load inventory file. File may not exist yet.");
    }
}

// Trigger auto-save
void Dialog::triggerAutoSave()
{
    systemController->autoSaveData();
}

// Check and notify low stock
void Dialog::checkAndNotifyLowStock()
{
    std::set<Product*> products = inventory->getProductList();
    std::vector<Product*> lowStockProducts;

    for (Product* p : products) {
        if (p->getQuantity() < p->getMinQuantity()) {
            lowStockProducts.push_back(p);
        }
    }

    if (!lowStockProducts.empty()) {
        showLowStockNotification(lowStockProducts);
    }
}

// Show low stock notification popup
void Dialog::showLowStockNotification(const std::vector<Product*>& lowStockProducts)
{
    QString message = " LOW STOCK ALERT \n\n";
    message += QString("You have %1 product(s) running low:\n\n").arg(lowStockProducts.size());

    int count = 0;
    for (Product* p : lowStockProducts) {
        if (count >= 5) {
            message += QString("\n... and %1 more products").arg(lowStockProducts.size() - 5);
            break;
        }
        message += QString("• %1 (ID: %2)\n  Current: %3 | Min: %4\n\n")
                       .arg(QString::fromStdString(p->getName()))
                       .arg(QString::fromStdString(p->getId()))
                       .arg(p->getQuantity())
                       .arg(p->getMinQuantity());
        count++;
    }

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Low Stock Alert");
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
