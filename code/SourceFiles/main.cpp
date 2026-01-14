/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Purpose: Main entry point for Inventory Management System Qt Application
 */
#include <QApplication>
#include <QMessageBox>
#include "systemController.h"
#include "logindialog.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create and initialize SystemController
    SystemController* systemController = new SystemController();

    if (!systemController->initializeSystem()) {
        QMessageBox::critical(nullptr, "System Error",
                              "Failed to initialize system. Please check data files.");
        delete systemController;
        return 1;
    }

    while (true) {
        // Show login dialog with system controller
        LoginDialog loginDialog(systemController);

        if (loginDialog.exec() == QDialog::Accepted && loginDialog.isAuthenticated()) {
            // Get the authenticated user
            User* authenticatedUser = loginDialog.getAuthenticatedUser();

            // Show main dialog with the logged-in user and system controller
            Dialog mainDialog(authenticatedUser, systemController);
            mainDialog.exec();

            // Logout after dialog closes
            systemController->logout();

        } else {
            // User cancelled login or closed dialog
            break;
        }
    }

    // Shutdown system and cleanup
    systemController->shutdownSystem();
    delete systemController;

    return 0;
}
