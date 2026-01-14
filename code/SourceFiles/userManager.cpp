/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Description: Implements the UserManager class for saving and loading
 * User and Admin accounts to/from text files WITHOUT encryption.
 */

#include "userManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

UserManager::UserManager() {}

bool UserManager::saveAllAccounts(const vector<User*>& users,
                                  const vector<Admin*>& admins,
                                  const string& filename) {
    ofstream outFile(filename);

    if (!outFile.is_open()) {
        cout << "Error: Could not create/open file for writing: " << filename << endl;
        return false;
    }

    cout << "Saving user accounts to file: " << filename << endl;

    // ========== SAVE REGULAR USERS ==========
    outFile << "USERS" << endl;
    outFile << users.size() << endl;

    for (User* u : users) {
        outFile << u->getUserName() << "|"
                << u->getPassword() << "|"  // ✅ Plain text password
                << u->getEmail() << "|"
                << (u->getIsLoggedIn() ? "1" : "0") << endl;
    }

    // ========== SAVE ADMINS ==========
    outFile << "ADMINS" << endl;
    outFile << admins.size() << endl;

    for (Admin* a : admins) {
        outFile << a->getUserName() << "|"
                << a->getPassword() << "|"  // ✅ Plain text password
                << a->getEmail() << "|"
                << (a->getIsLoggedIn() ? "1" : "0") << endl;
    }

    outFile.close();
    cout << "Successfully saved " << users.size() << " users and "
         << admins.size() << " admins to " << filename << endl;

    return true;
}

bool UserManager::loadAllAccounts(vector<User*>& users,
                                  vector<Admin*>& admins,
                                  const string& filename) {
    ifstream inFile(filename);

    if (!inFile.is_open()) {
        cout << "Warning: Could not open file for reading: " << filename << endl;
        cout << "No saved accounts found. This is normal for first run." << endl;
        return false;
    }

    cout << "Loading user accounts from file: " << filename << endl;

    string line;

    // ========== LOAD REGULAR USERS ==========
    getline(inFile, line);
    if (line != "USERS") {
        cout << "Error: Invalid file format (expected USERS)" << endl;
        inFile.close();
        return false;
    }

    int numUsers;
    inFile >> numUsers;
    inFile.ignore();

    for (int i = 0; i < numUsers; i++) {
        getline(inFile, line);
        stringstream ss(line);
        string username, password, email, loggedInStr;

        getline(ss, username, '|');
        getline(ss, password, '|');  // ✅ Plain text password
        getline(ss, email, '|');
        getline(ss, loggedInStr);

        // Create new user with loaded data
        User* user = new User(username, password, email);
        users.push_back(user);
    }

    // ========== LOAD ADMINS ==========
    getline(inFile, line);
    if (line != "ADMINS") {
        cout << "Error: Invalid file format (expected ADMINS)" << endl;
        inFile.close();
        return false;
    }

    int numAdmins;
    inFile >> numAdmins;
    inFile.ignore();

    for (int i = 0; i < numAdmins; i++) {
        getline(inFile, line);
        stringstream ss(line);
        string username, password, email, loggedInStr;

        getline(ss, username, '|');
        getline(ss, password, '|');  //
        getline(ss, email, '|');
        getline(ss, loggedInStr);

        // Create new admin with loaded data
        Admin* admin = new Admin(username, password, email);

        // Grant some default permissions
        admin->grantPermission("VIEW_INVENTORY");
        admin->grantPermission("MANAGE_INVENTORY");
        admin->grantPermission("VIEW_REPORTS");

        admins.push_back(admin);
    }

    inFile.close();
    cout << "Successfully loaded " << numUsers << " users and "
         << numAdmins << " admins from " << filename << endl;

    return true;
}
