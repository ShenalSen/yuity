#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

using namespace std;

class User {
private:
    string username;
    string password;
    string role; // admin, staff, etc.

public:
    // Constructor
    User(string uname, string pwd, string r);
    
    // Default constructor
    User();
    
    // Getters
    string getUsername() const;
    string getPassword() const;
    string getRole() const;
    
    // Setters
    void setUsername(string uname);
    void setPassword(string pwd);
    void setRole(string r);
    
    // String representation for file storage
    string toString() const;
    
    // Create user from string (read from file)
    static User fromString(string str);
};

// Function prototypes for user management
vector<User> loadUsersFromFile();
void saveUsersToFile(const vector<User>& users);
bool validateUser(const string& username, const string& password, string& role);

#endif // USER_H