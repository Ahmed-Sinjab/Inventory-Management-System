#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"
#include <vector>
#include <string>

class Admin : public User {
private:
    std::vector<std::string> adminPermissions;

public:
    Admin(std::string userName, std::string password, std::string email);

    // Permission management
    void grantPermission(std::string permission);
    void revokePermission(std::string permission);
    bool hasPermission(std::string permission) const;
    void displayPermissions() const;

    // Admin operations
    void viewSystemLogs() const;
};

#endif
