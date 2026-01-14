/*
*Author: Omar Hossain
*Start Date: November 6, 2025
*Last Modified: December 2024
*Purpose: Implementation file for the LoginDialog class.
*This class handles user authentication for the Inventory Management System.
*It provides a login interface that validates credentials through SystemController
*and grants access to the main dialog.
*Dependencies: logindialog.h, ui_logindialog.h, systemController.h, User class, Admin class.
*/

#include "logindialog.h"
#include "ui_logindialog.h"

/*
* Constructor for the LoginDialog class.
* Initializes the UI and stores a reference to the SystemController.
* All user authentication is now handled through the controller,
* which loads users from persistent storage (accounts.txt).
*/
LoginDialog::LoginDialog(SystemController* controller, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
    , authenticated(false)
    , currentUser(nullptr)
    , systemController(controller)
{
    ui->setupUi(this); // Initialize all UI components defined in the .ui file

    // Set window title for clarity
    setWindowTitle("Login - Inventory Management System");

    // Connect login and cancel buttons to their respective handlers
    connect(ui->buttonLogin, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);

    // Allow pressing the Enter key to trigger the login button
    ui->buttonLogin->setDefault(true);

    // Set password field to hide characters
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
}

/*
* Destructor for LoginDialog.
* Cleans up memory used by the UI.
* Note: currentUser is NOT deleted because it's managed by SystemController.
*/
LoginDialog::~LoginDialog()
{
    delete ui;
}

/*
* Returns true if a user has successfully authenticated.
*/
bool LoginDialog::isAuthenticated() const
{
    return authenticated;
}

/*
* Returns a pointer to the currently authenticated user.
* This allows the main dialog to access user-specific data (e.g., username, email).
*/
User* LoginDialog::getAuthenticatedUser() const
{
    return currentUser;
}

/*
* Slot executed when the "Login" button is clicked.
* Retrieves the entered username and password, validates them,
* and authenticates through SystemController which checks all loaded users.
*/
void LoginDialog::onLoginClicked()
{
    QString username = ui->lineEditUsername->text().trimmed();
    QString password = ui->lineEditPassword->text();

    // Validate input fields
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error",
                             "Please enter both username and password!");
        return;
    }

    // Try to login through SystemController
    // First attempt: Try regular user login
    if (systemController->loginUser(username.toStdString(), password.toStdString())) {
        authenticated = true;

        // Get the authenticated user from SystemController
        std::vector<User*> users = systemController->getAllUsers();
        for (User* u : users) {
            if (u->getUserName() == systemController->getCurrentUsername()) {
                currentUser = u;
                break;
            }
        }

        QMessageBox::information(this, "Success",
                                 "Login successful! Welcome, User!");
        accept(); // Close dialog successfully and return to main
        return;
    }

    // Second attempt: Try admin login
    if (systemController->loginAdmin(username.toStdString(), password.toStdString())) {
        authenticated = true;

        // Get the authenticated admin from SystemController
        std::vector<Admin*> admins = systemController->getAllAdmins();
        for (Admin* a : admins) {
            if (a->getUserName() == systemController->getCurrentUsername()) {
                currentUser = a;
                break;
            }
        }

        QMessageBox::information(this, "Success",
                                 "Login successful! Welcome, Admin!");
        accept(); // Close dialog successfully and return to main
        return;
    }

    // Both login attempts failed
    authenticated = false;
    currentUser = nullptr;

    // Show error message
    QMessageBox::warning(this, "Login Failed",
                         "Invalid username or password!\n\n"
                         "Default credentials:\n"
                         "Admin: username='admin', password='admin123'");

    // Clear password field for security reasons
    ui->lineEditPassword->clear();
    ui->lineEditPassword->setFocus();
}

/*
* Slot executed when the "Cancel" button is clicked.
* Sets authentication status to false and closes the dialog without logging in.
*/
void LoginDialog::onCancelClicked()
{
    authenticated = false;
    currentUser = nullptr;
    reject(); // Close the dialog and signal failure to the main application
}
