#include "admin.h"
#include <iostream>
#include <algorithm>
using namespace std;

Admin::Admin(string userName, string password, string email)
    : User(userName, password, email) {
    cout << "Admin account created: " << getUserName() << endl;

    adminPermissions.push_back("VIEW_INVENTORY");
    adminPermissions.push_back("VIEW_REPORTS");
}

void Admin::grantPermission(string permission) {
    if (!hasPermission(permission)) {
        adminPermissions.push_back(permission);
        cout << "Permission granted to " << getUserName() << ": " << permission << endl;
    } else {
        cout << "Admin " << getUserName() << " already has permission: " << permission << endl;
    }
}

void Admin::revokePermission(string permission) {
    auto it = find(adminPermissions.begin(), adminPermissions.end(), permission);
    if (it != adminPermissions.end()) {
        adminPermissions.erase(it);
        cout << "Permission revoked from " << getUserName() << ": " << permission << endl;
    } else {
        cout << "Admin " << getUserName() << " doesn't have permission: " << permission << endl;
    }
}

bool Admin::hasPermission(string permission) const {
    if (find(adminPermissions.begin(), adminPermissions.end(), "SUPER_ADMIN")
        != adminPermissions.end()) {
        return true;
    }

    return find(adminPermissions.begin(), adminPermissions.end(), permission)
           != adminPermissions.end();
}

void Admin::displayPermissions() const {
    cout << "\n=== Admin Permissions for " << getUserName() << " ===" << endl;

    if (adminPermissions.empty()) {
        cout << "No permissions granted." << endl;
        return;
    }

    for (const auto& perm : adminPermissions) {
        cout << "  ✓ " << perm << endl;
    }
}

void Admin::viewSystemLogs() const {
    if (!getIsLoggedIn()) {
        cout << "Error: Must be logged in to view system logs." << endl;
        return;
    }

    if (!hasPermission("VIEW_LOGS")) {
        cout << "Access Denied! Admin " << getUserName()
        << " lacks VIEW_LOGS permission." << endl;
        return;
    }

    cout << "\n=== System Logs ===" << endl;
    cout << "Admin " << getUserName() << " accessed system logs." << endl;
    cout << "Recent activities:" << endl;
    cout << "  - Products added/removed from inventory" << endl;
    cout << "  - User login activities" << endl;
    cout << "  - Stock level changes" << endl;
}
