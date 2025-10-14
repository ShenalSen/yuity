#include "user.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Constructor
User::User(string uname, string pwd, string r) {
    username = uname;
    password = pwd;
    role = r;
}

// Default constructor
User::User() {
    username = "";
    password = "";
    role = "staff";
}

// Getters
string User::getUsername() const {
    return username;
}

string User::getPassword() const {
    return password;
}

string User::getRole() const {
    return role;
}

// Setters
void User::setUsername(string uname) {
    username = uname;
}

void User::setPassword(string pwd) {
    password = pwd;
}

void User::setRole(string r) {
    role = r;
}

// String representation for file storage
string User::toString() const {
    ostringstream oss;
    oss << username << "|" << password << "|" << role;
    return oss.str();
}

// Create user from string (read from file)
User User::fromString(string str) {
    User user;
    stringstream ss(str);
    string token;
    
    // Parse username
    if (getline(ss, token, '|')) {
        user.setUsername(token);
    }
    
    // Parse password
    if (getline(ss, token, '|')) {
        user.setPassword(token);
    }
    
    // Parse role
    if (getline(ss, token, '|')) {
        user.setRole(token);
    }
    
    return user;
}

// Load users from file
vector<User> loadUsersFromFile() {
    vector<User> users;
    ifstream file("users.txt");
    string line;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                User user = User::fromString(line);
                users.push_back(user);
            }
        }
        file.close();
    } else {
        cout << "Warning: Could not open users.txt. Creating default admin user." << endl;
        // Create default admin user if file doesn't exist
        User defaultAdmin("admin", "admin123", "admin");
        users.push_back(defaultAdmin);
        saveUsersToFile(users);
    }
    
    return users;
}

// Save users to file
void saveUsersToFile(const vector<User>& users) {
    ofstream file("users.txt");
    
    if (file.is_open()) {
        for (const auto& user : users) {
            file << user.toString() << endl;
        }
        file.close();
    } else {
        cout << "Error: Could not open users.txt for writing." << endl;
    }
}

// Validate user credentials
bool validateUser(const string& username, const string& password, string& role) {
    vector<User> users = loadUsersFromFile();
    
    for (const auto& user : users) {
        if (user.getUsername() == username && user.getPassword() == password) {
            role = user.getRole();
            return true;
        }
    }
    
    return false;
}