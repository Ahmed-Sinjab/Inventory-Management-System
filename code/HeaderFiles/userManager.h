/**
 * Author(s): Ahmed Sinjab, Omar Hossain
 * Creation Date: December 12, 2024
 * Last Edit: December 12, 2024
 * Purpose: Manages saving and loading of User and Admin objects to/from files.
 *
 * Dependencies:
 *  - user.h : User class definition
 *  - admin.h : Admin class definition
 *  - <string>, <vector>
 */

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include "admin.h"
#include <string>
#include <vector>

class UserManager {
public:
    UserManager();

    // Save/Load all users and admins together
    bool saveAllAccounts(const std::vector<User*>& users,
                         const std::vector<Admin*>& admins,
                         const std::string& filename);

    bool loadAllAccounts(std::vector<User*>& users,
                         std::vector<Admin*>& admins,
                         const std::string& filename);

private:
    // Helper methods for simple password encryption
    std::string encryptPassword(const std::string& password) const;
    std::string decryptPassword(const std::string& encrypted) const;
};

#endif // USERMANAGER_H
