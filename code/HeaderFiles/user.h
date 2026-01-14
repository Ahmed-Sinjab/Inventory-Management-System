/*
*Author: Ahmed Sinjab, Omar Hossain
*Start Date: November 6, 2025
*Last Edit: November 6, 2025
*Purpose:This is a header file for class User.cpp
*Dependencies: Class Product
*
*/


#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
class Product;
class User
{
private:
    //private user attributes:
    std::string userName;
    std::string password;
    std::string email;
    bool isLoggedIn;//keeps track if this user is currently logged in

public:
    // Constructor
    User(std::string uName, std::string pwd, std::string mail);
    virtual ~User();

    //User Methods:
    bool login(std::string uName, std::string pwd);//Logs the user into the system
    bool logout();//logs the user out of the system
    bool changePassword(std::string oldPassword, std::string newPassword);//changes the user's password
    std::vector<Product*> viewInventory();//Allows the user to view the current inventory
    bool purchaseItem(Product* product, int quantity);//allows the user to purchase an item
    bool useItem(Product* product, int quantity);//allows the user to use an item/product

    // Getters
    std::string getUserName() const;
    bool getIsLoggedIn() const;
    std::string getEmail()const;
    std::string getPassword() const;
};


#endif // USER_H
