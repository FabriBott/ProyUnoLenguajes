#ifndef USER_HPP
#define USER_HPP

#include "common.hpp"

// Estructura para usuarios
class User {
    public:
        string username;
        string ip;
        int port;
        vector<string> contacts;

        User() : port(DEFAULT_CLIENT_PORT) {}
        
        User(const string& name, const string& ipAddr, int portNum) 
            : username(name), ip(ipAddr), port(portNum) {}

        // Método para convertir a JSON
        json toJson() const {
            json j;
            j["username"] = username;
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
            user.ip = j["ip"];
            user.port = j["port"];
            
            user.contacts.clear();
            for (const auto& contact : j["contacts"]) {
                user.contacts.push_back(contact);
            }
            
            return user;
        }
};

#endif // USER_HPP