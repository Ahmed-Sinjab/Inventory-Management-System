#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>
#include "user.h"
#include "admin.h"
#include "inventory.h"
#include "fileManager.h"
#include "productFactory.h"
#include "systemController.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(User* user, SystemController* controller, QWidget *parent = nullptr);
    ~Dialog();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onAddProductClicked();
    void onDeleteProductClicked();
    void onProductTypeChanged(int index);
    void onViewAllClicked();
    void onBuyProductClicked();
    void onSellProductClicked();
    void onCheckLowStockClicked();
    void onViewSuppliersClicked();
    void onGenerateReportClicked();
    void onLogoutClicked();
    void onChangePasswordClicked();
    void onSaveInventoryClicked();
    void onLoadInventoryClicked();

    // Auto-check low stock periodically
    void checkAndNotifyLowStock();

private:
    Ui::Dialog *ui;
    User* currentUser;
    Inventory* inventory;
    FileManager* fileManager;
    SystemController* systemController;

    // Timer for periodic low stock checks
    QTimer* lowStockCheckTimer;

    void connectSignals();
    void updateFieldVisibility();
    void refreshProductList();
    productType getProductTypeFromUI();
    void showProductDetails(Product* product);

    // Auto-save after inventory changes
    void triggerAutoSave();

    // Show low stock notification
    void showLowStockNotification(const std::vector<Product*>& lowStockProducts);
};

#endif // DIALOG_H
