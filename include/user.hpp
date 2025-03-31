#ifndef USER_HPP
#define USER_HPP
#include "common.hpp"
#include <string>
#include <algorithm>

// Estructura para usuarios
class User {
public:
    string username;
    string password;  // New field for password
    string ip;
    int port;
    vector<string> contacts;

    User() : port(DEFAULT_CLIENT_PORT) {}
    
    User(const string& name, const string& pwd, const string& ipAddr, int portNum)
        : username(name), password(pwd), ip(ipAddr), port(portNum) {}
    
    // Método para convertir a JSON
    json toJson() const {
        json j;
        j["username"] = username;
        j["password"] = password;  // Include password in JSON
        j["ip"] = ip;
        j["port"] = port;
        json contactsArray = json::array();
        for (const auto& contact : contacts) {
            contactsArray.push_back(contact);
        }
        j["contacts"] = contactsArray;
        return j;
    }
    
    // Método para cargar desde JSON
    static User fromJson(const json& j) {
        User user;
        user.username = j["username"];
        user.password = j.value("password", "");  // Handle existing users without passwords
        user.ip = j["ip"];
        user.port = j["port"];
        user.contacts.clear();
        if (j.contains("contacts")) {
            for (const auto& contact : j["contacts"]) {
                user.contacts.push_back(contact);
            }
        }
        return user;
    }
    
    // Método para verificar credenciales
    bool checkCredentials(const string& username, const string& password) const {
        return this->username == username && this->password == password;
    }
};
#endif // USER_HPP