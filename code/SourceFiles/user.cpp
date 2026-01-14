/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description:
 *     This file implements the User class, responsible for user account creation,
 *     login/logout functionality, and secure password management.
 *
 * Dependencies:
 *     - user.h       : Declares the User class interface.
 *     - <iostream>   : Used for console-based user feedback and status messages.
 */

#include "user.h"
#include <iostream>
using namespace std;

// Constructor: initializes a new user with the given username, password, and email.
// Sets login status to false initially (user starts logged out).
User::User(string userName, string password, string email)
    : userName(userName), password(password), email(email), isLoggedIn(false) {
    cout << "User account created: " << userName << endl;
}

// Destructor: currently empty since no dynamic memory allocation is used.
User::~User() {}

// Attempts to log the user in by checking if the provided credentials match
// the stored username and password. If successful, sets isLoggedIn = true.
bool User::login(string userName, string password) {
    if (userName == this->userName && password == this->password) {
        isLoggedIn = true;
        cout << "Login successful! Welcome, " << userName << "!" << endl;
        return true;
    }
    cout << "Login failed! Invalid credentials." << endl;
    return false;
}

// Logs the user out only if they are currently logged in.
// Returns true if logout is successful, otherwise prints an error message.
bool User::logout() {
    if (isLoggedIn) {
        isLoggedIn = false;
        cout << userName << " logged out successfully." << endl;
        return true;
    }
    cout << "Error: You are not logged in." << endl;
    return false;
}

// Allows a logged-in user to change their password.
// Verifies the old password before allowing the update.
bool User::changePassword(string oldPassword, string newPassword) {
    if (!isLoggedIn) {
        cout << "Error: You must be logged in to change password." << endl;
        return false;
    }

    if (oldPassword == password) {
        password = newPassword;
        cout << "Password changed successfully!" << endl;
        cout << "Password changed successfully! The New Password is ----------------> " << password << endl;
        return true;
    }

    cout << "Error: Incorrect old password." << endl;
    return false;
}

// Getter: returns the username of the current user.
string User::getUserName() const {
    return userName;
}

// Getter: returns whether the user is currently logged in.
bool User::getIsLoggedIn() const {
    return isLoggedIn;
}

std::string User::getPassword() const {
    return password;
}
// Getter: returns the user's registered email address.
string User::getEmail() const {
    return email;
}
